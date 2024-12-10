

#include "VCPluginHeaders.h"

// Interface includes
#include "IControlView.h"
#include "ISubject.h"
#include "IPanelControlData.h"
#include "IDropDownListController.h"
#include "IStringListControlData.h"
#include "IXPageMgrAccessor.h"
#include "IArticleDataModel.h"
#include "IStringListData.h"
#include "IStringData.h"
#include "ITextControlData.h"
#include "IWebServices.h"
#include "IXPGPreferences.h"
#include "ITriStateControlData.h"
#include "ISysFileData.h"
#include "ITreeViewMgr.h"
#include "ITreeViewController.h"
#include "IIntData.h"
#include "ISelectionManager.h"
#include "DebugClassUtils.h"
#include "IHierarchy.h"
#include "ISpread.h"
#include "IXPageUtils.h"
#include "IActiveContext.h"
#include "ISelectionUtils.h"
#include "IXPageSuite.h"
#include "IGeometry.h"
#include "TransformTypes.h"
#include "TransformUtils.h"
#include "IDataBase.h"
#include "ILayoutUIUtils.h"
#include "IDocument.h"
#include "ISpreadList.h"
#include "IXRailPageSlugData.h"

// General includes
#include "CDialogObserver.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "ErrorUtils.h"
#include "XMLDefs.h"

// Project includes
#include "XPageUIUtils.h"
#include "XPGUIID.h"
#include "XPGID.h"
#include "XRLID.h"

class XPGUILinkArtDialogObserver : public CDialogObserver{

public:

	XPGUILinkArtDialogObserver( IPMUnknown* boss );
	virtual ~XPGUILinkArtDialogObserver() ;	
	
	void AutoAttach();	
	void AutoDetach();	
	void Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);

private:

	void AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
	void DetachWidget(const InterfacePtr<IPanelControlData>&, const WidgetID& widgetID, const PMIID& interfaceID);

	void HandleFormeSelection();
	
	// Gestion des cartons
	void FillClasseurList();
	void FillCartonList();

	bool8 firstInit;
};

CREATE_PMINTERFACE(XPGUILinkArtDialogObserver, kXPGUILinkArtDialogObserverImpl)

/* Constructor
*/
XPGUILinkArtDialogObserver::XPGUILinkArtDialogObserver( IPMUnknown* boss ) :
							CDialogObserver(boss), firstInit(kTrue)
{
}

/* Destructor
*/
XPGUILinkArtDialogObserver::~XPGUILinkArtDialogObserver()
{
}

/* AutoAttach
*/
void XPGUILinkArtDialogObserver::AutoAttach()
{
	CDialogObserver::AutoAttach();

	do
	{
		// Initialise widget state.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
			break;
	
		// Observes widgets		
		this->AttachWidget(panelControlData, kXPGUIClasseurListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXPGUICartonListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		
		if(firstInit){
			FillClasseurList();
			firstInit = kFalse;
        }

			
	} while(false);
	
}

/* AutoDetach
*/
void XPGUILinkArtDialogObserver::AutoDetach()
{
	CDialogObserver::AutoDetach();

	do
	{
		// Detach widget observers.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
			break;	

		this->DetachWidget(panelControlData, kXPGUIClasseurListWidgetID,	IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXPGUICartonListWidgetID,	IID_ISTRINGLISTCONTROLDATA);
		
	} while(false);
}

/* Update
*/
void XPGUILinkArtDialogObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{	
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);

	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	
	if ((protocol == IID_ISTRINGLISTCONTROLDATA) && (theChange == kPopupChangeStateMessage))
	{
		if(controlView != nil)
		{
			WidgetID widgetID = controlView->GetWidgetID();	
			switch(widgetID.Get())
			{
				case kXPGUIClasseurListWidgetID:
					{	
						FillCartonList();
						HandleFormeSelection();			
					}
					break;

				case kXPGUICartonListWidgetID:						
					this->HandleFormeSelection();				
					break;


				default :
					break;
			}
		}
	}		
}		


/* AttachWidget
*/
void XPGUILinkArtDialogObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
			break;	

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
			break;		
		subject->AttachObserver(this, interfaceID);
	}
	while (false);
}

/* DetachWidget
*/
void XPGUILinkArtDialogObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
			break;	

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
			break;
		subject->DetachObserver(this, interfaceID);
	}
	while (false);
}

void XPGUILinkArtDialogObserver::FillClasseurList(){

	// Access forme model to get classeur list
	InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IFormeDataModel> model (pageMrgAccessor->QueryFormeDataModel());
	K2Vector<PMString> classeurList = model->GetClasseurList();

	// Fill drop down list widget
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	XPageUIUtils::FillDropdownlist(panelControlData,kXPGUIClasseurListWidgetID,classeurList, kFalse);

	// Fill carton list
	FillCartonList();
}

void XPGUILinkArtDialogObserver::FillCartonList(){

	// Get selected classeur
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	 
	InterfacePtr<IDropDownListController> classeurListWidgetController (panelControlData->FindWidget(kXPGUIClasseurListWidgetID), UseDefaultIID());		
	int32 selectedIndex = classeurListWidgetController->GetSelected();

	K2Vector<PMString> formeList;
	if(selectedIndex != IDropDownListController::kNoSelection){
		
        InterfacePtr<IStringListControlData> classeurListData (classeurListWidgetController, UseDefaultIID());		
		PMString selectedClasseur = classeurListData->GetString(selectedIndex);     

		// Access forme model to get forme list
		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IFormeDataModel> model (pageMrgAccessor->QueryFormeDataModel());
		
		formeList = model->GetFormeList(selectedClasseur);
 	}
	// Fill drop down list widget
	XPageUIUtils::FillDropdownlist(panelControlData,kXPGUICartonListWidgetID,formeList, kFalse);
    HandleFormeSelection();
    
}


void XPGUILinkArtDialogObserver::HandleFormeSelection(){

	PMString classeur = kNullString, carton = kNullString, posX, posY;
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());

	do{

		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IFormeDataModel>   model (pageMrgAccessor->QueryFormeDataModel());		

		InterfacePtr<IDropDownListController> classeurListController (panelControlData->FindWidget(kXPGUIClasseurListWidgetID), UseDefaultIID());		
		InterfacePtr<IDropDownListController> cartonListController (panelControlData->FindWidget(kXPGUICartonListWidgetID), UseDefaultIID());			
		
		int32 indexClasseur = classeurListController->GetSelected();
		int32 indexCarton   = cartonListController->GetSelected();
		
		InterfacePtr<IStringListControlData> classeurListData (classeurListController, UseDefaultIID());
		classeur = classeurListData->GetString (indexClasseur);
			
		InterfacePtr<IStringListControlData> cartonListData (cartonListController, UseDefaultIID());
		carton = cartonListData->GetString (indexCarton);

		if(!model->DoesFormeExist(carton, classeur))
			break;

		IDFile formeFile, matchingFile,parentFileForme, parentFilePreviwe;
		if(model->GetForme(carton, classeur,formeFile, matchingFile, posX, posY)){

			FileUtils::GetParentDirectory(formeFile,parentFileForme);
			FileUtils::GetParentDirectory(parentFileForme,parentFilePreviwe);
		
			PMString pathPreview = FileUtils::SysFileToPMString(parentFilePreviwe);
		   //	pathPreviwe.Append("\\Vignette\\");
			pathPreview.Append(FileUtils::GetDirectorySeperator());
			pathPreview.Append("Vignette");
			pathPreview.Append(FileUtils::GetDirectorySeperator());
			pathPreview.Append(carton);
			pathPreview.Append(".jpg");
				
			// On Affiche la vignette  du Carton
			IDFile preview;
			FileUtils::PMStringToIDFile(pathPreview, preview);
			IControlView * displayImagePanel = panelControlData->FindWidget(kXPGUIDisplayImagePanelWidgetID);
			InterfacePtr<ISysFileData> imagePanelFileData (displayImagePanel, UseDefaultIID());
			imagePanelFileData->Set(preview);
			displayImagePanel->Invalidate();	
			
		}
	} while(kFalse);
}