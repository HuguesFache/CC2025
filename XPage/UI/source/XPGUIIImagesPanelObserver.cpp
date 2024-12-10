

#include "VCPluginHeaders.h"

// Interface includes
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ISubject.h"
#include "IApplication.h"
#include "ISelectionManager.h"
#include "ISelectionMessages.h"
#include "ITextSelectionSuite.h"
#include "IDocument.h"
#include "IStringData.h"
#include "IXPageMgrAccessor.h"
#include "IImageDataModel.h"
#include "ITreeViewMgr.h"
#include "IPMUnknownData.h"
#include "ISysFileData.h"
#include "ITriStateControlData.h"
#include "ISelectionManager.h"
#include "ISelectionUtils.h"
#include "IXPageSuite.h"
#include "IStringListData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "ITreeViewController.h"
#include "ISysFileData.h"
#include "IXPageUtils.h"

// General includes
#include "SelectionObserver.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "ErrorUtils.h"

// Project includes
#include "XPGUIImageNodeID.h"
#include "XPGUIID.h"
#include "XPGID.h"
#include "IPlacedArticleData.h"

/** XPGUIImagesPanelObserver

	@author Trias Developpement
*/

class XPGUIImagesPanelObserver : public ActiveSelectionObserver
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPGUIImagesPanelObserver( IPMUnknown* boss );
	/** Destructor
	*/
	virtual ~XPGUIImagesPanelObserver() ;
	
	/** 
		Initialises widgets and attaches widget observers.
	*/
	void AutoAttach();

	/** 
		Detaches widget observers. 
	*/
	void AutoDetach();

	/**
		Update is called for all registered observers, and is
		the method through which changes are broadcast. 
		@param theChange this is specified by the agent of change; it can be the class ID of the agent,
		or it may be some specialised message ID.
		@param theSubject this provides a reference to the object which has changed; in this case, the
		widget boss objects that are being observed.
		@param protocol the protocol along which the change occurred.
		@param changedBy this can be used to provide additional information about the change or a reference
		to the boss object that caused the change.
	*/
	void Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);
	
protected:
	/*
		This method is called whenever selection changed, update image filter according 
		to selected article
	*/
	virtual void HandleSelectionChanged(const ISelectionMessage * message);
	
private:
	void AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
	void DetachWidget(const InterfacePtr<IPanelControlData>&, const WidgetID& widgetID, const PMIID& interfaceID);

	void HandleNumeroChanged();
	
	void RefreshModel(IPanelControlData * panelControlData);
	
	void HandleArticleFilter(bool16 activateFilter);
	
	void ResetVignette(IPanelControlData * panelControlData, const IDFile& imageFile);
		
	PMString currentFilter, currentNumero;
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIImagesPanelObserver, kXPGUIImagesPanelObserverImpl)

/* Constructor
*/
XPGUIImagesPanelObserver::XPGUIImagesPanelObserver( IPMUnknown* boss ) :
	ActiveSelectionObserver( boss ), currentFilter(kNullString), currentNumero(kNullString)
{
	InterfacePtr<ISubject> sessionSubject (GetExecutionContextSession(), UseDefaultIID());
	sessionSubject->AttachObserver(this, IID_IREFRESHPROTOCOL);	
}

/* Destructor
*/
XPGUIImagesPanelObserver::~XPGUIImagesPanelObserver()
{
}

/* AutoAttach
*/
void XPGUIImagesPanelObserver::AutoAttach()
{
	// set data model for tree view widget		
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	InterfacePtr<IPMUnknownData> dataModel (panelControlData->FindWidget(kXPGUIImagesViewWidgetID), IID_IDATAMODEL);
	InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IImageDataModel> imageDataModel (pageMrgAccessor->QueryImageDataModel());
	dataModel->SetPMUnknown(imageDataModel);
		
	ActiveSelectionObserver::AutoAttach();

	do
	{
		// Initialise widget state.
		
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoAttach() invalid panelControlData");
			break;
		}
		
		// Get notification when current number changes
		InterfacePtr<ISubject> sessionSubject (GetExecutionContextSession(), UseDefaultIID());
		sessionSubject->AttachObserver(this, IID_ICURRENTNUMERO);		

		// Get notification when image filter state changes
		InterfacePtr<ISubject> filterWidgetSubject (panelControlData->FindWidget(kXPGUIFilterImageWidgetID), UseDefaultIID());
		filterWidgetSubject->AttachObserver(this, ITriStateControlData::kDefaultIID);
		
		// Update display
		this->HandleNumeroChanged();
		
		InterfacePtr<ITriStateControlData> filterWidget (filterWidgetSubject, UseDefaultIID());
		this->HandleArticleFilter(filterWidget->IsSelected());		

    } while(false);

}

/* AutoDetach
*/
void XPGUIImagesPanelObserver::AutoDetach()
{
	ActiveSelectionObserver::AutoDetach();

	do
	{
		// Detach widget observers.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)		 
		{
			ASSERT_FAIL("AutoDetach() invalid panelControlData");
			break; 
		}	

		InterfacePtr<ISubject> sessionSubject (GetExecutionContextSession(), UseDefaultIID());
		sessionSubject->DetachObserver(this, IID_ICURRENTNUMERO);		
		
		InterfacePtr<ISubject> filterWidgetSubject (panelControlData->FindWidget(kXPGUIFilterImageWidgetID), UseDefaultIID());
		filterWidgetSubject->DetachObserver(this, ITriStateControlData::kDefaultIID);

	} while(false);

}

/* Update
*/
void XPGUIImagesPanelObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy){	

	ActiveSelectionObserver::Update(theChange, theSubject, protocol, changedBy);
	
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	if (controlView != nil){
		WidgetID widgetID = controlView->GetWidgetID();		
		switch (widgetID.Get()){		
			case kXPGUIFilterImageWidgetID :
				this->HandleArticleFilter((theChange == kTrueStateMessage));
				break;				
			
			default:
				break;
		}		
	}
	else if(theChange == kXPGNumeroChangeMsg && protocol == IID_ICURRENTNUMERO){
		this->HandleNumeroChanged();
	}
	else if(theChange == kXPGRefreshMsg && protocol == IID_IREFRESHPROTOCOL){
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		this->RefreshModel(panelControlData);
	}
}		


/* AttachWidget
*/
void XPGUIImagesPanelObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
		{
			ASSERT_FAIL("controlView invalid");
			break;
		}

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
		{
			ASSERT_FAIL("subject invalid");
			break;
		}
		subject->AttachObserver(this, interfaceID);
	}
	while (false);
}

/* DetachWidget
*/
void XPGUIImagesPanelObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
		{
			ASSERT_FAIL("controlView invalid.");
			break;
		}

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
		{
			ASSERT_FAIL("subject invalid");
			break;
		}
		subject->DetachObserver(this, interfaceID);
	}
	while (false);
}

/* HandleSelectionChanged
*/
void XPGUIImagesPanelObserver::HandleSelectionChanged(const ISelectionMessage * message)
{
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());	
	InterfacePtr<ITriStateControlData> filterWidget (panelControlData->FindWidget(kXPGUIFilterImageWidgetID), UseDefaultIID());
	HandleArticleFilter(filterWidget->IsSelected());
}

/* HandleNumeroChanged
*/
void XPGUIImagesPanelObserver::HandleNumeroChanged()
{	
	// Get new numero
	InterfacePtr<IStringData> newNumero (GetExecutionContextSession(), IID_ICURRENTNUMERO);
	PMString numeroId = newNumero->GetString();
	
	if(numeroId != currentNumero)
	{
		currentNumero = numeroId;
		
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		
		// Refresh model
		this->RefreshModel(panelControlData);

		// Check selection
		InterfacePtr<ITreeViewController> treeViewController(panelControlData->FindWidget(kXPGUIImagesViewWidgetID), UseDefaultIID());
	
		NodeIDList selection;
		treeViewController->GetSelectedItems(selection);
		
		if(selection.empty())
		{
			// Reset image panel	
			ResetVignette(panelControlData, IDFile());
		}
	}
}

/* HandleArticleFilter
*/
void XPGUIImagesPanelObserver::HandleArticleFilter(bool16 activateFilter)
{
	// Check whether there is a selected article to filter image
	UIDRef selectedStory = UIDRef::gNull;
	PMString filter = kNullString, filterPath = kNullString;
	
	if(activateFilter)
	{
		InterfacePtr<IXPageSuite> xpageSuite (fCurrentSelection, IID_IXPAGESUITE);
		if(xpageSuite)
			xpageSuite->GetTextFrameStoryRef(selectedStory);
	
		if(selectedStory != UIDRef::gNull)
		{
			InterfacePtr<IStringListData> selectedArticleID (selectedStory, IID_IARTICLEINFO);
			K2Vector<PMString> articleInfo = selectedArticleID->GetStringList();
			if(!articleInfo.empty())
			{
				filter = articleInfo[0]; // article ID is the first stored information 
				filterPath = articleInfo[1]; // article path is the second one 
			}
		}
	}
	

	// Pass filter to image model
	if(filter != currentFilter || filter == kNullString)
	{
		currentFilter = filter;
		
		InterfacePtr<IStringData> newNumero (GetExecutionContextSession(), IID_ICURRENTNUMERO);
		PMString numeroId = newNumero->GetString();
		
		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IImageDataModel> model (pageMrgAccessor->QueryImageDataModel());
		model->SetFilter(filter, filterPath);		
		
		if( selectedStory == UIDRef::gNull || filter != kNullString) // leave tree empty if selected story doesn't come from an imported article
			model->Rebuild(numeroId, IImageDataModel::kPhoto);
		else
			model->Rebuild(kNullString, IImageDataModel::kPhoto);
		
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		InterfacePtr<ITreeViewMgr> treeViewMgr(panelControlData->FindWidget(kXPGUIImagesViewWidgetID), UseDefaultIID());
		treeViewMgr->ClearTree(kTrue);
		treeViewMgr->ChangeRoot(kTrue);
		
		if(currentFilter != kNullString)
		{
			// If filter is active, expand all nodes and select first leave
			InterfacePtr<ITreeViewHierarchyAdapter> treeViewHier (treeViewMgr, UseDefaultIID());
			treeViewMgr->ExpandNode(treeViewHier->GetRootNode(), kTrue);
			
			NodeID nodeId = treeViewHier->GetRootNode();
			TreeNodePtr<XPGUIImageNodeID> nodePtr (nodeId);
			
			while(treeViewHier->GetNumChildren(nodeId) != 0 && nodePtr->GetType() != XPGImageDataNode::kImageNode)
			{
				nodeId = treeViewHier->GetNthChild(nodeId, 0);
				nodePtr.reset(nodeId);
			} 
			
			InterfacePtr<ITreeViewController> treeViewController (treeViewMgr, UseDefaultIID());
			treeViewController->Select(nodeId);
			
			// Refresh vignette
			TreeNodePtr<XPGUIImageNodeID> imageNodeID (nodeId);
			ResetVignette(panelControlData, FileUtils::PMStringToSysFile(imageNodeID->GetData()));
		}	
		else
		{			
			ResetVignette(panelControlData, IDFile());
		}
	}
}

void XPGUIImagesPanelObserver::ResetVignette(IPanelControlData * panelControlData, const IDFile& imageFile)
{
	IControlView * displayImagePanel = panelControlData->FindWidget(kXPGUIDisplayImagePanelWidgetID);
	InterfacePtr<ISysFileData> imagePanelFileData (displayImagePanel, UseDefaultIID());
	imagePanelFileData->Set(imageFile);
	displayImagePanel->Invalidate();
}

void XPGUIImagesPanelObserver::RefreshModel(IPanelControlData * panelControlData)
{
	InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IImageDataModel> model (pageMrgAccessor->QueryImageDataModel());
	model->Rebuild(currentNumero);

	InterfacePtr<ITreeViewMgr> treeViewMgr(panelControlData->FindWidget(kXPGUIImagesViewWidgetID), UseDefaultIID());
	treeViewMgr->ClearTree();
	treeViewMgr->ChangeRoot(kTrue);
}