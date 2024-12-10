/*
 //
 //
 //  File : XRLUILinkerDialogObserver.cpp
 //
 */


#include "VCPluginHeaders.h"

// Interface includes
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ISubject.h"
#include "IDropDownListController.h"
#include "IIntList.h"
#include "IStringListControlData.h"
#include "IWebServices.h"
#include "IStringData.h"
#include "ILayoutUIUtils.h"
#include "IDocument.h"
#include "IUIDData.h"

// General includes
#include "CDialogObserver.h"
#include "DocUtils.h"


#include "CAlert.h"
#include "ErrorUtils.h"

// Project includes
#include "XRLUIID.h"

/** XRLUILinkerDialogObserver
 
 @author Trias
 */

class XRLUILinkerDialogObserver : public CDialogObserver
	{
	public:
		/**
		 Constructor.
		 @param boss interface ptr from boss object on which this interface is aggregated.
		 */
		XRLUILinkerDialogObserver( IPMUnknown* boss );
		/** Destructor
		 */
		virtual ~XRLUILinkerDialogObserver() ;
		
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
		
		
	private:
		void AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
		void DetachWidget(const InterfacePtr<IPanelControlData>&, const WidgetID& widgetID, const PMIID& interfaceID);
		void doLoadList2(int32 id);
		void doLoadList3(int32 id);
		void doLoadList4(int32 id);
		PMString baseName, serverAdress;
	};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
 */
CREATE_PMINTERFACE(XRLUILinkerDialogObserver, kXRLUILinkerDialogObserverImpl)

/* Constructor
 */
XRLUILinkerDialogObserver::XRLUILinkerDialogObserver( IPMUnknown* boss ) :
CDialogObserver( boss )
{	
}

/* Destructor
 */
XRLUILinkerDialogObserver::~XRLUILinkerDialogObserver()
{
}

/* AutoAttach
 */
void XRLUILinkerDialogObserver::AutoAttach()
{
	CDialogObserver::AutoAttach();
	
	do
	{
		IDocument* theDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();

		int32 idPage = GetPageID(::GetUIDRef(theDoc), 0, baseName);
		serverAdress = GetGoodIP(baseName);

		// Initialise widget state.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoAttach() invalid panelControlData");
			break;
		}
		
		// Attach to the drop down lists
		this->AttachWidget(panelControlData, kXRLUILevel1ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXRLUILevel2ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXRLUILevel3ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXRLUILevel4ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		
	} while(false);
	
}

/* AutoDetach
 */
void XRLUILinkerDialogObserver::AutoDetach()
{
	CDialogObserver::AutoDetach();
	
	do
	{
		// Detach widget observers.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoDetach() invalid panelControlData");
			break;
		}	
		
		// Detach from the drop down lists
		this->DetachWidget(panelControlData, kXRLUILevel1ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXRLUILevel2ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXRLUILevel3ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXRLUILevel4ListBoxWidgetID, IID_ISTRINGLISTCONTROLDATA);
		
		
	} while(false);
	
}

/* Update
 */
void XRLUILinkerDialogObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);
	
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	if (controlView != nil)
	{
		if (theChange == kPopupChangeStateMessage)
		{
			WidgetID widgetID = controlView->GetWidgetID();
			K2Vector<int32> listLevelID;
			int32 selectedLine, pageID;
			
			switch (widgetID.Get())
			{
					
				case kXRLUILevel1ListBoxWidgetID:
				{
					InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
					
					InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
					
					selectedLine = dropDownListController->GetSelected();
					listLevelID = pageListID->GetIntList();
					pageID = listLevelID[selectedLine];
					doLoadList2(pageID);
					
				}					
					break;
					
				case kXRLUILevel2ListBoxWidgetID:
				{
					InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
					
					InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
					
					selectedLine = dropDownListController->GetSelected();
					listLevelID = pageListID->GetIntList();
					pageID = listLevelID[selectedLine];
					doLoadList3(pageID);
					
				}					
					break;
					
				case kXRLUILevel3ListBoxWidgetID:
				{
					InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
					
					InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
					
					selectedLine = dropDownListController->GetSelected();
					listLevelID = pageListID->GetIntList();
					pageID = listLevelID[selectedLine];
					doLoadList4(pageID);
					
				}					
					break;
					
				default:
					break;
			}
		}
	}
}		


/* AttachWidget
 */
void XRLUILinkerDialogObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
void XRLUILinkerDialogObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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

/* Chargement de la deuxieme liste
 */
void XRLUILinkerDialogObserver::doLoadList2(int32 id)
{
	do
	{
		// Initialize status list, getting it from XRail server
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		
		IControlView* controlView = panelControlData->FindWidget(kXRLUILevel2ListBoxWidgetID);		
		
		InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());		
		stringListControlData->Clear(kFalse, kFalse);
		
		InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAdress.GetPlatformString().c_str());
		
		K2Vector<PMString> listLevel2Names;
		K2Vector<int32> listLevel2ID;
		
		bool connexionOK = xrailWebServices->GetLevel2List(id, listLevel2Names, listLevel2ID);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSGetLevelKey);
			break;
		}
		
		// Get page status
		InterfacePtr<IStringData> pageStatus (this, UseDefaultIID());
		
		int32 selectedIndex = IDropDownListController::kBeginning;
		for (int32 i = 0 ; i < listLevel2Names.size() ; ++i) 
		{
			PMString level2Name = listLevel2Names[i];
			
			stringListControlData->AddString(level2Name, 
											 IStringListControlData::kEnd, 	
											 kFalse, 	// Invalidate?					
											 kFalse);	// Notify of change?
		}	
		
		InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
		pageListID->SetIntList(listLevel2ID);
		
		InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
		
		if(stringListControlData->Length() > 0)
			dropDownListController->Select(selectedIndex);
		
	}
	while (false);
}

/* Chargement de la troisieme liste
 */
void XRLUILinkerDialogObserver::doLoadList3(int32 id)
{
	do
	{
		// Initialize status list, getting it from XRail server
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		
		IControlView* controlView = panelControlData->FindWidget(kXRLUILevel3ListBoxWidgetID);		
		
		InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());		
		stringListControlData->Clear(kFalse, kFalse);
		
		InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAdress.GetPlatformString().c_str());
		
		K2Vector<PMString> listLevel3Names;
		K2Vector<int32> listLevel3ID;
		
		bool connexionOK = xrailWebServices->GetLevel3List(id, listLevel3Names, listLevel3ID);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSGetLevelKey);
			break;
		}
		
		// Get page status
		InterfacePtr<IStringData> pageStatus (this, UseDefaultIID());
		
		int32 selectedIndex = IDropDownListController::kBeginning;
		for (int32 i = 0 ; i < listLevel3Names.size() ; ++i) 
		{
			PMString level3Name = listLevel3Names[i];
			
			stringListControlData->AddString(level3Name, 
											 IStringListControlData::kEnd, 	
											 kFalse, 	// Invalidate?					
											 kFalse);	// Notify of change?
		}	
		
		InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
		pageListID->SetIntList(listLevel3ID);
		
		InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
		
		if(stringListControlData->Length() > 0)
			dropDownListController->Select(selectedIndex);
		
	}
	while (false);
}

/* Chargement de la quatrieme liste
 */
void XRLUILinkerDialogObserver::doLoadList4(int32 id)
{
	do
	{
		
		// Initialize status list, getting it from XRail server
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		
		IControlView* controlView = panelControlData->FindWidget(kXRLUILevel4ListBoxWidgetID);		
		
		InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());		
		stringListControlData->Clear(kFalse, kFalse);
		
		InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAdress.GetPlatformString().c_str());
		
		K2Vector<PMString> listLevel4Names;
		K2Vector<int32> listLevel4ID;
		int16 	nbPages;
		//on recupere le document qui est au premier plan
		IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		UIDRef myRef;
		if(!frontDoc)
		{
			// Get concerned document and update status of its pages with the choice of the user
			InterfacePtr<IUIDData> theDoc (this, UseDefaultIID());	
			myRef = theDoc->GetRef();
		}
		else
			myRef = ::GetUIDRef(frontDoc);
		
		
		//on recupere le nb de pages de ce document
		nbPages = GetNbPages(myRef);
		
		bool connexionOK = xrailWebServices->GetLevel4List(id, nbPages, listLevel4Names, listLevel4ID);
		if(!connexionOK)
		{
			CAlert::ErrorAlert(kXRLUIErrorWSGetLevelKey);
			break;
		}
		
		// Get page status
		InterfacePtr<IStringData> pageStatus (this, UseDefaultIID());
		
		int32 selectedIndex = IDropDownListController::kBeginning;
		for (int32 i = 0 ; i < listLevel4Names.size() ; ++i) 
		{
			PMString level4Name = listLevel4Names[i];
			
			stringListControlData->AddString(level4Name, 
											 IStringListControlData::kEnd, 	
											 kFalse, 	// Invalidate?					
											 kFalse);	// Notify of change?
		}	
		
		InterfacePtr<IIntList> pageListID (controlView, IID_IINTLIST);
		pageListID->SetIntList(listLevel4ID);
		
		InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());
		
		if(stringListControlData->Length() > 0)
			dropDownListController->Select(selectedIndex);
		
	}
	while (false);
}
