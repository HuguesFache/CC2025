
#include "VCPlugInHeaders.h"

// Interface includes
#include "ISubject.h"
#include "IControlView.h"
#include "ITreeViewMgr.h"
#include "IPanelControlData.h"
#include "ITreeNodeIDData.h"
#include "ITreeViewController.h"
#include "ISysFileData.h"
#include "IPMStream.h"
#include "IWidgetParent.h"
#include "IWorkspace.h"
#include "IArticleDataModel.h"
#include "ITextControlData.h"

#include "IActionManager.h"
#include "IActionComponent.h"
#include "IStringData.h"
#include "IApplication.h"

// General includes
#include "CObserver.h"
#include "CAlert.h"
#include "K2Vector.tpp" // For NodeIDList to compile
#include "StreamUtil.h"
#include "ITriStateControlData.h"

// Project includes
#include "XPGUIImageNodeID.h"
#include "XPGUIID.h"
#include "XPGID.h"
#include "FileUtils.h"

/** Implements IObserver; initialises the tree when an IObserver::AutoAttach message sent
	and listens for when the node selection changes.
	When the selection changes and there's a non-empty selection list, it takes the first item
	and previews the asset if it can do so (for instance, if it's a GIF, JPEG etc).
	It does this by setting a path on a data interface of the custom-view panel widget and
	invalidating the widget, forcing it to redraw to create a rendering of the new asset.

	 The class is derived from CObserver, and overrides the
	AutoAttach(), AutoDetach(), and Update() methods.
	This class implements the IObserver interface using the CObserver helper class,
	and is listening along the IID_ITREEVIEWCONTROLLER protocol for changes in the tree data model.

	@ingroup paneltreeview
	
*/
class XPGUIImagesViewObserver : public CObserver
{
public:

	XPGUIImagesViewObserver(IPMUnknown* boss);
	~XPGUIImagesViewObserver();
	
	virtual void AutoAttach();
	virtual void AutoDetach();
	virtual void Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);


protected:

	void handleSelectionChanged();
	void HandleNBImageSelection();
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XPGUIImagesViewObserver, kXPGUIImagesViewObserverImpl)


/* Constructor
*/
XPGUIImagesViewObserver::XPGUIImagesViewObserver(IPMUnknown* boss): CObserver(boss){}


/* Destructor
*/
XPGUIImagesViewObserver::~XPGUIImagesViewObserver()
{
}


/* AutoAttach
*/
void XPGUIImagesViewObserver::AutoAttach()
{
	InterfacePtr<ISubject> subj(this, UseDefaultIID()); 
	ASSERT(subj);
	subj->AttachObserver(this, IID_ITREEVIEWCONTROLLER);

	do{
		InterfacePtr<IWidgetParent> widgetParent (this, UseDefaultIID());
		InterfacePtr<IPanelControlData> hilitePanelCtrlData ((IPanelControlData *)widgetParent->QueryParentFor(IID_IPANELCONTROLDATA));
		InterfacePtr<IWidgetParent> hilitePanelParent (hilitePanelCtrlData, UseDefaultIID());
		InterfacePtr<IPanelControlData> panelCtrlData ((IPanelControlData *)hilitePanelParent->QueryParentFor(IID_IPANELCONTROLDATA));
		IControlView * controlView = panelCtrlData->FindWidget(kXPGUIBlackWhiteWidgetID);		
		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil) 
			break;	

		subject->AttachObserver(this, IID_ITRISTATECONTROLDATA);
	
	}while(kFalse);

	this->handleSelectionChanged();
}


/* AutoDetach
*/
void XPGUIImagesViewObserver::AutoDetach()
{
	InterfacePtr<ISubject> subj(this, UseDefaultIID()); 
	subj->DetachObserver(this, IID_ITREEVIEWCONTROLLER);

	do{
		InterfacePtr<IWidgetParent> widgetParent (this, UseDefaultIID());
		InterfacePtr<IPanelControlData> hilitePanelCtrlData ((IPanelControlData *)widgetParent->QueryParentFor(IID_IPANELCONTROLDATA));
		InterfacePtr<IWidgetParent> hilitePanelParent (hilitePanelCtrlData, UseDefaultIID());
		InterfacePtr<IPanelControlData> panelCtrlData ((IPanelControlData *)hilitePanelParent->QueryParentFor(IID_IPANELCONTROLDATA));
		IControlView * controlView = panelCtrlData->FindWidget(kXPGUIBlackWhiteWidgetID);
	
		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil) 
			break;	
		subject->DetachObserver (this, IID_ITRISTATECONTROLDATA);
	
	}while(kFalse);
}


/* Update
*/
void XPGUIImagesViewObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy){
	
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());

	if(theChange == kListSelectionChangedMessage)
		this->handleSelectionChanged();
	
	else if(theChange == kFalseStateMessage  || theChange == kTrueStateMessage)
	{
		if(controlView && controlView->GetWidgetID() == kXPGUIBlackWhiteWidgetID)
			this->handleSelectionChanged();	
	}
}

/* handleSelectionChanged
*/
void XPGUIImagesViewObserver::handleSelectionChanged(){

	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	do{
		InterfacePtr<ITreeViewController> controller(this, UseDefaultIID());

		NodeIDList selectedItems;
		controller->GetSelectedItems(selectedItems);
		
		IDFile imageFile;
        PMString credit =kNullString, legend=kNullString;
       	
		// We've got single selection only
		if(selectedItems.size()>0){
			TreeNodePtr<XPGUIImageNodeID> nodeID(selectedItems[0]);			
			if(!nodeID)
				break;		

			if(nodeID->GetType() == XPGImageDataNode::kImageNode){

				// Set file to display for image panel
				// Want NB preview image
				bool16 wantNB = kFalse;	

				InterfacePtr<IWidgetParent> widgetParent (this, UseDefaultIID());
				InterfacePtr<IPanelControlData> hilitePanelCtrlData ((IPanelControlData *)widgetParent->QueryParentFor(IID_IPANELCONTROLDATA));
				InterfacePtr<IWidgetParent> hilitePanelParent (hilitePanelCtrlData, UseDefaultIID());
				InterfacePtr<IPanelControlData> panelCtrlData ((IPanelControlData *)hilitePanelParent->QueryParentFor(IID_IPANELCONTROLDATA));
				InterfacePtr<ITriStateControlData> bwCheckBox (panelCtrlData->FindWidget(kXPGUIBlackWhiteWidgetID), UseDefaultIID());	
				
				if(bwCheckBox && bwCheckBox->IsSelected())
					wantNB = kTrue;
			
				// Get path to low res file
				if(wantNB)
					imageFile = FileUtils::PMStringToSysFile(nodeID->GetImageData()->hrFileNB);
				else 
					imageFile = FileUtils::PMStringToSysFile(nodeID->GetImageData()->brFile);
				
				InterfacePtr<IControlView> nodeWidget (this, UseDefaultIID()); 
                credit = nodeID->GetImageData()->credit;
                legend = nodeID->GetImageData()->legend;
			}
			else			
				imageFile = IDFile();
		}
							
		InterfacePtr<IWidgetParent> widgetParent (this, UseDefaultIID());

		InterfacePtr<IPanelControlData> hilitePanelCtrlData ((IPanelControlData *)widgetParent->QueryParentFor(IID_IPANELCONTROLDATA));
		InterfacePtr<IWidgetParent> hilitePanelParent (hilitePanelCtrlData, UseDefaultIID());
		InterfacePtr<IPanelControlData> panelCtrlData ((IPanelControlData *)hilitePanelParent->QueryParentFor(IID_IPANELCONTROLDATA));
		IControlView * imagePanelView = panelCtrlData->FindWidget(kXPGUIDisplayImagePanelWidgetID);

		InterfacePtr<ISysFileData> imagePanelFileData (imagePanelView, UseDefaultIID());
		imagePanelFileData->Set(imageFile);
        
        // Credit & Legende
        credit.SetTranslatable(kFalse);
        legend.SetTranslatable(kFalse);	
          
		// Display credit & legend
        IControlView* displayCreditView = panelCtrlData->FindWidget( kXPGUICreditTextWidgetID );
	   	InterfacePtr<ITextControlData> creditPanelView( displayCreditView, UseDefaultIID());
	    creditPanelView->SetString(credit); 
        
        IControlView* displaylegendView = panelCtrlData->FindWidget( kXPGUILegendeTextWidgetID );
        InterfacePtr<ITextControlData> legendPanelView ( displaylegendView, UseDefaultIID());
        legendPanelView->SetString(legend); 
    
        legend.clear();       
		credit.clear();        

		imagePanelView->Invalidate();
        displayCreditView->Invalidate();
        displaylegendView->Invalidate();

	} while(kFalse);
}

void XPGUIImagesViewObserver::HandleNBImageSelection(){
	
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	do{
		IDFile associatedNBImagePreview;

		// Get selected image
		InterfacePtr<ITreeViewController> imageListController (panelControlData->FindWidget(kXPGUIImagesViewWidgetID), UseDefaultIID());
		NodeIDList selectedItems;
		imageListController->GetSelectedItems(selectedItems);

		if(!selectedItems.empty()){

			// Want NB preview image
			bool16 wantNB = kFalse;			
			TreeNodePtr<XPGUIImageNodeID> nodeID(selectedItems[0]);
			InterfacePtr<ITriStateControlData> bwCheckBox (panelControlData->FindWidget(kXPGUIBlackWhiteWidgetID), UseDefaultIID());	
			if(bwCheckBox && bwCheckBox->IsSelected())
				wantNB = kTrue;
			
			// Get path to low res file
			if(wantNB)
				associatedNBImagePreview = FileUtils::PMStringToSysFile(nodeID->GetImageData()->hrFileNB);
			else 
				associatedNBImagePreview = FileUtils::PMStringToSysFile(nodeID->GetImageData()->brFile);			
		}				

		IControlView * imagePanelView = panelControlData->FindWidget(kXPGUIDisplayImagePanelWidgetID);
		InterfacePtr<ISysFileData> imagePanelFileData (imagePanelView, UseDefaultIID());
		imagePanelFileData->Set(associatedNBImagePreview);
		imagePanelView->Invalidate();

	} while(kFalse);
}

//	end, File: XPGUIImagesViewObserver.cpp
