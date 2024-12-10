
#include "VCPlugInHeaders.h"

// Interface includes:
#include "CIdleTask.h"
#include "IIntData.h"
#include "IStringData.h"
#include "ICoreFilename.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IControlView.h"
#include "ITextControlData.h"
#include "IUIDData.h"
#include "IWebServices.h"
#include "IDocument.h"
#include "ILayoutUIUtils.h"

// General includes:
#include "CAlert.h"
#include "CDialogController.h"
#include "SystemUtils.h"
#include "FileUtils.h"
#include "StreamUtil.h"
#include "DocUtils.h"
#include "WidgetID.h"
#include "PMString.h"
#include "IIntList.h"

// Project includes:
#include "XRLUIID.h"
#include "XRLID.h"
#include "XRCID.h"

/** XPBCLoginDialogController
	Methods allow for the initialization, validation, and application of dialog widget values.
  
	Implements IDialogController based on the partial implementation CDialogController. 
	
	@author GD
*/


class XRailLinkerDialogController : public CDialogController
{
	public:
		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XRailLinkerDialogController(IPMUnknown* boss);

		/**
			Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
		virtual void InitializeDialogFields(IActiveContext* dlgContext);
		
		/**
			Validate the values in the widgets. 
			By default, the widget with ID kOKButtonWidgetID causes 
			ValidateFields to be called. When all widgets are valid, 
			ApplyFields will be called.			
			@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

		*/
		virtual WidgetID ValidateDialogFields(IActiveContext* myContext);

		/**
			Retrieve the values from the widgets and act on them.
			@param widgetId identifies the widget on which to act.
		*/
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);

		PMString baseName, serverAdress;
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XRailLinkerDialogController, kXRLUILinkerDialogControllerImpl)

/* Constructor
*/
XRailLinkerDialogController::XRailLinkerDialogController(IPMUnknown* boss) : CDialogController(boss)
{
}

/* ApplyFields
*/
void XRailLinkerDialogController::InitializeDialogFields(IActiveContext* dlgContext) 
{
	//on recupere l'utilisateur
	InterfacePtr<IStringData> sessionID (GetExecutionContextSession(), IID_ISTRINGDATA_LOGIN);
	PMString userStr = sessionID->GetString();
	PMString::ConversionError convError = PMString::kNoError;
	int32 userID = userStr.GetAsNumber(&convError);
	
	do {
		// Initialize status list, getting it from XRail server
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		
		//on recupere la premiere liste
		IControlView* controlView = panelControlData->FindWidget(kXRLUILevel1ListBoxWidgetID);		
		
		InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());		
		stringListControlData->Clear(kFalse, kFalse);

		IDocument* theDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();	

		int32 idPage = GetPageID(::GetUIDRef(theDoc), 0, baseName);
		serverAdress = GetGoodIP(baseName);

		InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAdress.GetPlatformString().c_str());
		
		K2Vector<PMString> listLevel1Names;
		K2Vector<int32> listLevel1ID;
		
		bool connexionOK = xrailWebServices->GetLevel1List(userID, listLevel1Names, listLevel1ID);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSGetLevelKey);
			break;
		}

		// Get page status
		InterfacePtr<IStringData> pageStatus (this, UseDefaultIID());

		int32 selectedIndex = IDropDownListController::kBeginning;
		for (int32 i = 0 ; i < listLevel1Names.size() ; ++i) 
		{
			PMString level1Name = listLevel1Names[i];

			stringListControlData->AddString(level1Name, 
							IStringListControlData::kEnd, 	
							kFalse, 	// Invalidate?					
							kFalse);	// Notify of change?
		}	
			
		InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
		pageListID->SetIntList(listLevel1ID);
			
		InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
		
		if(stringListControlData->Length() > 0)
			dropDownListController->Select(selectedIndex);
		
		//on vide les listes 2, 3 et 4
		IControlView* controlView2 = panelControlData->FindWidget(kXRLUILevel2ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData2(controlView2, UseDefaultIID());		
		stringListControlData2->Clear(kFalse, kFalse);
		InterfacePtr<IIntList> pageListID2 (controlView2, IID_IINTLIST);
		pageListID2->Clear();

		IControlView* controlView3 = panelControlData->FindWidget(kXRLUILevel3ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData3(controlView3, UseDefaultIID());		
		stringListControlData3->Clear(kFalse, kFalse);
		InterfacePtr<IIntList> pageListID3 (controlView3, IID_IINTLIST);
		pageListID3->Clear();

		IControlView* controlView4 = panelControlData->FindWidget(kXRLUILevel4ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData4(controlView4, UseDefaultIID());		
		stringListControlData4->Clear(kFalse, kFalse);
		InterfacePtr<IIntList> pageListID4 (controlView4, IID_IINTLIST);
		pageListID4->Clear();

	} while (false);
}

/* ValidateFields
*/
WidgetID XRailLinkerDialogController::ValidateDialogFields(IActiveContext* dlgContext) 
{	
	WidgetID 		badWidgetID = kInvalidWidgetID;

	do {
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		IControlView* controlView1 = panelControlData->FindWidget(kXRLUILevel1ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData1(controlView1, UseDefaultIID());		
		if (stringListControlData1->Length() == 0) 
		{
			badWidgetID = kXRLUILevel1ListBoxWidgetID;
			break;			
		}
	
		IControlView* controlView2 = panelControlData->FindWidget(kXRLUILevel2ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData2(controlView2, UseDefaultIID());		
		if (stringListControlData2->Length() == 0) 
		{
			badWidgetID = kXRLUILevel2ListBoxWidgetID;
			break;			
		}
	
		IControlView* controlView3 = panelControlData->FindWidget(kXRLUILevel3ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData3(controlView3, UseDefaultIID());		
		if (stringListControlData3->Length() == 0) 
		{
			badWidgetID = kXRLUILevel3ListBoxWidgetID;
			break;			
		}
	
		IControlView* controlView4 = panelControlData->FindWidget(kXRLUILevel4ListBoxWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData4(controlView4, UseDefaultIID());		
		if (stringListControlData4->Length() == 0) 
		{
			badWidgetID = kXRLUILevel4ListBoxWidgetID;
			break;			
		}
	
	} while (false);
	return badWidgetID;
}

/* ApplyFields
*/
void XRailLinkerDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId)
{
	// Get webservice client boss and set server address for it
	InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
	xrailWebServices->SetServerAddress(serverAdress.GetPlatformString().c_str());

	do {
		//on commence par recuperer l'ID de la page choisie
		K2Vector<int32> listLevelID;
		int32 selectedLine, pageID;
		
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		IControlView* controlView = panelControlData->FindWidget(kXRLUILevel4ListBoxWidgetID);
		InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
		InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
		selectedLine = dropDownListController->GetSelected();
		listLevelID = pageListID->GetIntList();
		pageID = listLevelID[selectedLine];
						
		//on recupere le document qui est au premier plan
		IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		//on recupere le nb de pages de ce document
		int16 	nbPages;
		UIDRef myRef;
		if(!frontDoc)
		{
			// Get concerned document and update status of its pages with the choice of the user
			InterfacePtr<IUIDData> theDoc (this, UseDefaultIID());	
			myRef = theDoc->GetRef();
		}
		else
			myRef = ::GetUIDRef(frontDoc);
		
		nbPages = GetNbPages(myRef);
		
		//appel du webservice, on recupere la liste de tous les ID
		K2Vector<int32> newIDS;
		bool connexionOK = xrailWebServices->GetPagesIDS(pageID, newIDS);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSLinkPagesKey);
			break;
		}
		//on va maintenant boucler sur toutes les pages, et attribuer son id a chaque page
		for (int16 i = 0; i < nbPages; i++) 
		{
			SetPageID(myRef, i, newIDS[i], baseName);
		}
		
		//toutes les pages ont leurs ids
		//on recupere maintenant le nom du document ou sauvegarder
		PMString newDocName;
		connexionOK = xrailWebServices->GetDocFileNameWithID(pageID, newDocName);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSLinkPagesKey);
			break;
		}
		
		//et on sauvegarde le document
		SaveAsDoc(myRef, newDocName);
		
	} while (false);	
}

// End, XPBCLoginDialogController.cpp.





