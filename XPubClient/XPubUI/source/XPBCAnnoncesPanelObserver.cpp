
#include "VCPlugInHeaders.h"

// Interface includes
#include "ISession.h"
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "ILayoutUIUtils.h"
#include "IWidgetParent.h"
#include "IApplication.h"
#include "IDocumentList.h"
#include "ILayoutUtils.h"
#include "IDocument.h"
#include "IXPubConnexion.h"
#include "IPageList.h"
#include "IStringData.h"
#include "IPointListData.h"
#include "IUnitOfMeasure.h"
#include "ITriStateControlData.h"
#include "IListControlData.h"
#include "IAnnoncesList.h"
#include "ITreeViewController.h"
#include "ITextControlData.h"
#include "ITreeViewMgr.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"
#include "IBoolData.h"
#include "IDocumentCommands.h"
#include "IDocFileHandler.h"
#include "IDocumentUtils.h"
#include "IXPubClientUtils.h"

// General includes
#include "AcquireModalCursor.h"
#include "CAlert.h"
#include "CObserver.h"
#include "XPBCAnnonceNodeID.h"
#include "ErrorUtils.h"
#include "IDialogMgr.h"
#include "IDataStringUtils.h"
#include "LocaleSetting.h"
#include "CoreResTypes.h"
#include "RsrcSpec.h"
#include "FileUtils.h"
// Project includes
#include "XPBCID.h"

/** XRLUIResaPubPanelObserver
	
*/
class XPBCAnnoncesPanelObserver : public CObserver
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPBCAnnoncesPanelObserver( IPMUnknown* boss );
	/** Destructor
	*/
	virtual ~XPBCAnnoncesPanelObserver() ;
	
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
	
	void AttachDocList(void);
	void DetachDocList(void);

	void	doLoadTitlePopup(const bool16& connected);
	void	doLoadEditionPopup(const bool16& connected);
	void	doLoadStatusPopup(const bool16& connected);
	void	doLoadDateRealPopup(const bool16& connected);
	void	doLoadDateRealPopupFin(const bool16& connected);
	void	doRequest(void);
	void	doShowAdInfos(void);
	void	doClearDialog(void);
	void	doShowLoginDialog(void);
	void	doUpdateControls(void);
	void	doOpen();
	void	doSave(bool16 close = kFalse);
	void	doTerminate(bool16 forceBAT = kFalse);
	void	doSearch();
	void	doOpenPubFolder();

	PMString 	PopupGetString(int DropDownListID);
	
	PubInfoStruct curPub;
	IDocument * curFrontDoc;
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPBCAnnoncesPanelObserver, kXPBCAnnoncesPanelObserverImpl)

/* Constructor
*/
XPBCAnnoncesPanelObserver::XPBCAnnoncesPanelObserver( IPMUnknown* boss ) :
	CObserver( boss ), curFrontDoc(nil)
{
}

/* Destructor
*/
XPBCAnnoncesPanelObserver::~XPBCAnnoncesPanelObserver()
{
}

/* AutoAttach
*/
void XPBCAnnoncesPanelObserver::AutoAttach()
{
	do
	{
       		// Initialise widget state.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoAttach() invalid panelControlData");
			break;
		}

		// Attach widget observers.
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnConnect, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnDisconnect, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnRequest, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnOpen, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnSave, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnSaveClose, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnBat, ITriStateControlData::kDefaultIID);	
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnTerminate, ITriStateControlData::kDefaultIID);			
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnOpenPubFolder, ITriStateControlData::kDefaultIID);					
		this->AttachWidget(panelControlData, kWidgetID_PopTitre, IStringListControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kWidgetID_PopEdition, IStringListControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kWidgetID_PopEtatPub, IStringListControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kWidgetID_PopDateParuDeb, IStringListControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kWidgetID_PopDateParuFin, IStringListControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCWidgetID_BtnSearch, ITriStateControlData::kDefaultIID);
		this->AttachWidget(panelControlData, kXPBCAnnoncesBoxWidgetID, IID_ITREEVIEWCONTROLLER);
		
		// On attache le DocumentList observer, afin de pouvoir detecter un changement de selection de document, et
		// pouvoir reagir (dans le Update de cette classe).
		this->AttachDocList();

		// Mise a jour des widgets selon l'etat de connection
		doUpdateControls();	

	} while(false);

}

/* AutoDetach
*/
void XPBCAnnoncesPanelObserver::AutoDetach()
{
	do
	{
		// Detach widget observers.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoDetach() invalid panelControlData");
			break;
		}	

		
        this->DetachWidget(panelControlData, kXPBCWidgetID_BtnConnect, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnDisconnect, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnRequest, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnOpen, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnSave, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnSaveClose, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnBat, ITriStateControlData::kDefaultIID);		
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnTerminate, ITriStateControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnOpenPubFolder, ITriStateControlData::kDefaultIID);		
		this->DetachWidget(panelControlData, kWidgetID_PopTitre, IStringListControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kWidgetID_PopEdition, IStringListControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kWidgetID_PopEtatPub, IStringListControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kWidgetID_PopDateParuDeb, IStringListControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kWidgetID_PopDateParuFin, IStringListControlData::kDefaultIID);
		this->DetachWidget(panelControlData, kXPBCWidgetID_BtnSearch, ITriStateControlData::kDefaultIID);
        this->DetachWidget(panelControlData, kXPBCAnnoncesBoxWidgetID, IID_ITREEVIEWCONTROLLER);

		this->DetachDocList();
		
	} while(false);

}

/* Update
*/
void XPBCAnnoncesPanelObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{	
    if (theChange == kTrueStateMessage)
	{
		InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
		if (controlView != nil)
		{
			WidgetID widgetID = controlView->GetWidgetID();
			switch (widgetID.Get())
			{
				case kXPBCWidgetID_BtnConnect:
					{												
						// Appel du dialogue de connection
						doShowLoginDialog();
						
						// Mise a jour des widgets selon l'etat de connection
						doUpdateControls();					

						InterfacePtr<IBoolData> gLogin (GetExecutionContextSession(), IID_IBOOL_LOGIN);	
						bool16 connected = gLogin->Get();
						
						// Chargement des popups
						doLoadTitlePopup(connected);
						doLoadEditionPopup(connected);
						doLoadStatusPopup(connected);
						doLoadDateRealPopup(connected);
						doLoadDateRealPopupFin(connected);

						if(connected)
						{																					
							// Requete liste des pubs
							doRequest();
						}
					}
					break;
				
				case kXPBCWidgetID_BtnDisconnect:
					{
						// Deconnection de la session
						InterfacePtr<IBoolData> gLogin (GetExecutionContextSession(), IID_IBOOL_LOGIN);						
						gLogin->Set(kFalse);						

						// Nettoyage des composants
						doLoadTitlePopup(kFalse);
						doLoadEditionPopup(kFalse);
						doLoadStatusPopup(kFalse);
						doLoadDateRealPopup(kFalse);
						doLoadDateRealPopupFin(kFalse);
						this->doClearDialog();
						//HF clear la liste
                        InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
                        
                        InterfacePtr<ITreeViewMgr> annoncesListTreeMgr(panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
                        InterfacePtr<IAnnoncesList> annoncesListData (annoncesListTreeMgr, UseDefaultIID());
                        K2Vector<annoncedata> listeAnnonces;
                        
                        annoncesListData->SetAnnoncesList(listeAnnonces);
                        
                        annoncesListTreeMgr->ClearTree(kTrue);
                        annoncesListTreeMgr->ChangeRoot();
                        
						// Mise a jour des widgets selon l'etat de connection
						doUpdateControls();
					}
					break;

				case kXPBCWidgetID_BtnRequest:
					{
						AcquireWaitCursor busyCursor;
						this->doRequest();
					}
					break;
					
				case kXPBCWidgetID_BtnOpen:
					{
						AcquireWaitCursor busyCursor;
						this->doOpen();						
					}
					break;
				
				case kXPBCWidgetID_BtnSave:
					{
						AcquireWaitCursor busyCursor;
						this->doSave();
					}
					break;
					
				case kXPBCWidgetID_BtnSaveClose:
					{
						AcquireWaitCursor busyCursor;
						this->doSave(kTrue);
					}
					break;
					
				case kXPBCWidgetID_BtnBat:
					{
						AcquireWaitCursor busyCursor;
						this->doTerminate(kTrue);
					}
					break;	

				case kXPBCWidgetID_BtnTerminate:
					{
						AcquireWaitCursor busyCursor;
						this->doTerminate();
					}
					break;
				
				case kXPBCWidgetID_BtnSearch:
					{
						AcquireWaitCursor busyCursor;
						this->doSearch();
					}
					break;

				case kXPBCWidgetID_BtnOpenPubFolder:
					{
						AcquireWaitCursor busyCursor;
						this->doOpenPubFolder();
					}
					break;

				default :
					break;
			}		
		}
	}
	else if ((protocol == IID_ISTRINGLISTCONTROLDATA) && (theChange == kPopupChangeStateMessage))
	{
		InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
		if (controlView != nil)
		{
			WidgetID widgetID = controlView->GetWidgetID();
			switch (widgetID.Get())
			{
				case kWidgetID_PopTitre:
					{ 
						AcquireWaitCursor busyCursor;
						this->doLoadEditionPopup(kTrue); // Change are only possible if client is connected
						this->doRequest();
					}
					break;
					
				case kWidgetID_PopEdition:
				case kWidgetID_PopEtatPub:
				case kWidgetID_PopDateParuDeb:
				case kWidgetID_PopDateParuFin:
					{
						AcquireWaitCursor busyCursor;
						this->doRequest();
					}
					break;
			}
		}	
	}
	else if((protocol == IID_ITREEVIEWCONTROLLER) && (theChange == kListSelectionChangedByUserMessage || theChange == kListSelectionChangedMessage))
	{
		InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
		if (controlView != nil)
		{
			WidgetID widgetID = controlView->GetWidgetID();
			
			switch (widgetID.Get())
			{
				case kXPBCAnnoncesBoxWidgetID:
                    {
					this->doShowAdInfos();
                    }
					break;
				
				default :
					break;
			}		
		}		
	}
	else if ((theChange == kSetFrontDocCmdBoss || theChange == kCloseDocCmdBoss) && (protocol == IID_IDOCUMENTLIST))
	{
		IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if(frontDoc != curFrontDoc)
		{
			curFrontDoc = frontDoc;
			this->doUpdateControls();
		}
	}
	
}


/* AttachWidget
*/
void XPBCAnnoncesPanelObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
void XPBCAnnoncesPanelObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
		{
			ASSERT_FAIL("controlView invalid.");
			break;
		}
		
		InterfacePtr<ISubject> subject (controlView, UseDefaultIID());
		if (subject == nil)
		{
			ASSERT_FAIL("subject invalid");
			break;
		}
		subject->DetachObserver(this, interfaceID);
	}
	while (false);
}
// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::AttachDocList(void)
{
	do
	{
		InterfacePtr<IApplication> app (GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDocumentList> doclist (app->QueryDocumentList());
		InterfacePtr<ISubject> subject (doclist, UseDefaultIID());
		subject->AttachObserver(this, IID_IDOCUMENTLIST);
	}
	while (false);
}
// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::DetachDocList(void)
{
	do
	{
		InterfacePtr<IApplication> app (GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDocumentList> doclist (app->QueryDocumentList());
		InterfacePtr<ISubject> subject (doclist, UseDefaultIID());
		subject->DetachObserver(this, IID_IDOCUMENTLIST);
	}
	while (false);
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doLoadTitlePopup(const bool16& connected)
{	
	do 
	{
		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());
	
		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(kWidgetID_PopTitre);
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		InterfacePtr<IStringListControlData> iStringListControlData (iDropDownListController, UseDefaultIID());
	
		iStringListControlData->Clear(kTrue, kFalse);
		iStringListControlData->AddString(kXPBCStringKey_EVERYTITLES, IStringListControlData::kEnd, kFalse, kFalse);
		
		if(connected)
		{
			// GetTitleList
			InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
			InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
			xpubConnexion->SetServerAddress(serverAddress->Get());	

			K2Vector<PMString> listeTitre;
			bool success = xpubConnexion->ListeTitre(listeTitre);
			if(success)
			{
				for (int32 i = 0; i < listeTitre.size(); ++i)
				{			
					listeTitre[i].SetTranslatable(kFalse);
					iStringListControlData->AddString(listeTitre[i], IStringListControlData::kEnd, kFalse, kFalse);
				}
			}
			else
			{
				PMString error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::ErrorAlert(error);
			}				
		}

		// On selectionne la 1ere ligne
		iDropDownListController->Select(0, kTrue, kFalse);
		
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doLoadEditionPopup(const bool16& connected)
{
	do 
	{
		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());
	
		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(kWidgetID_PopEdition);		
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		InterfacePtr<IStringListControlData> iStringListControlData (iDropDownListController, UseDefaultIID());
	
		iStringListControlData->Clear(kTrue, kFalse);
		iStringListControlData->AddString(kXPBCStringKey_EVERYEDITIONS, IStringListControlData::kEnd, kFalse, kFalse);
		
		// On recupere le titre selectionne
		PMString selectedTitle = kNullString;
		selectedTitle = this->PopupGetString(kWidgetID_PopTitre);
		if(selectedTitle.IsEqual(kXPBCStringKey_EVERYTITLES) == kTrue)
			selectedTitle = "#";		
				
		if(connected)
		{
			// GetEditionList
			InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
			InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
			xpubConnexion->SetServerAddress(serverAddress->Get());	

			K2Vector<PMString> listeEdt;
			bool success = xpubConnexion->ListeEdition(selectedTitle, listeEdt);
			if(success)
			{
				for (int32 i = 0; i < listeEdt.size(); ++i)
				{			
					listeEdt[i].SetTranslatable(kFalse);
					iStringListControlData->AddString(listeEdt[i], IStringListControlData::kEnd, kFalse, kFalse);
				}
			}
			else
			{
				PMString error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::ErrorAlert(error);
			}				
		}

		// On selectionne la 1ere ligne
		iDropDownListController->Select(0, kTrue, kFalse);
		
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doLoadStatusPopup(const bool16& connected)
{
	do 
	{
		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());

		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(kWidgetID_PopEtatPub);		
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		InterfacePtr<IStringListControlData> iStringListControlData (iDropDownListController, UseDefaultIID());

		iStringListControlData->Clear(kTrue, kFalse);
		iStringListControlData->AddString(kXPBCStringKey_EVERYSTATUS, IStringListControlData::kEnd, kFalse, kFalse);
		
		if(connected)
		{
			// GetStatusList
			InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
			InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
			xpubConnexion->SetServerAddress(serverAddress->Get());	

			K2Vector<PMString> listeEtat;
			bool success = xpubConnexion->ListeEtat(listeEtat);
			if(success)
			{
				for (int32 i = 0; i < listeEtat.size(); ++i)
				{			
					listeEtat[i].SetTranslatable(kFalse);
					iStringListControlData->AddString(listeEtat[i], IStringListControlData::kEnd, kFalse, kFalse);
				}
			}
			else
			{
				PMString error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::ErrorAlert(error);
			}				
		}

		// On selectionne la 1ere ligne
		iDropDownListController->Select(0, kTrue, kFalse);
		
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doLoadDateRealPopup(const bool16& connected){

	do	{	

		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());

		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(kWidgetID_PopDateParuDeb);
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		InterfacePtr<IStringListControlData> iStringListControlData (iDropDownListController, UseDefaultIID());

		iStringListControlData->Clear(kTrue, kFalse);		
 
		if(connected){
			
			K2Vector<PMString> listParution;
			int32 indexSelected;
		
			// GetDateDebutList
			InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
			InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
			xpubConnexion->SetServerAddress(serverAddress->Get());	
			xpubConnexion->ListeDateAvant(listParution, indexSelected);			
			
			if(listParution.size()>0){
				for(int32 i = 0 ; i < listParution.size(); ++i){
					listParution[i].SetTranslatable(kFalse);
					iStringListControlData->AddString(listParution[i], IStringListControlData::kEnd, kFalse, kFalse);
				}	
				// On selectionne la i - ere ligne
				if(indexSelected == -1)
					iDropDownListController->Select(0, kTrue, kFalse);
				else 	
					iDropDownListController->Select(indexSelected, kTrue, kFalse);
			}else{
			
				GlobalTime localGlobalTime;
				localGlobalTime.CurrentTime();
				PMString sDate;
				localGlobalTime.DateToString(sDate, kTrue);
				sDate.SetTranslatable(kFalse);
				sDate.Remove(sDate.NumUTF16TextChars()- 4, 2);
				iStringListControlData->AddString(sDate, IStringListControlData::kEnd, kFalse, kFalse);
				iDropDownListController->Select(0, kTrue, kFalse);
			}
		}			
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doLoadDateRealPopupFin(const bool16& connected)
{
	do{	

		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());

		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(kWidgetID_PopDateParuFin);
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		InterfacePtr<IStringListControlData> iStringListControlData (iDropDownListController, UseDefaultIID());

		iStringListControlData->Clear(kTrue, kFalse);

		if(connected){

			K2Vector<PMString> listParution;
			int32 indexSelected;
		
			// GetDateDebutList
			InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
			InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
			xpubConnexion->SetServerAddress(serverAddress->Get());	

			xpubConnexion->ListeDateApres(listParution, indexSelected);
				// On selectionne la i-ere ligne
			if(listParution.size()>0)	{
				
				for(int32 i = 0 ; i < listParution.size(); ++i){
					listParution[i].SetTranslatable(kFalse);
					iStringListControlData->AddString(listParution[i], IStringListControlData::kEnd, kFalse, kFalse);
				}	
				// On selectionne la i - ere ligne
				if(indexSelected == -1)
					iDropDownListController->Select(0, kTrue, kFalse);
				else 	
					iDropDownListController->Select(indexSelected, kTrue, kFalse);
			}		
				
			else{
			
				GlobalTime localGlobalTime;
				localGlobalTime.CurrentTime();
				PMString sDate;
				localGlobalTime.DateToString(sDate, kTrue);
				sDate.SetTranslatable(kFalse);
				sDate.Remove(sDate.NumUTF16TextChars()- 4, 2);
				iStringListControlData->AddString(sDate, IStringListControlData::kEnd, kFalse, kFalse);
				iDropDownListController->Select(0, kTrue, kFalse);
			}
		}
					
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doRequest(void)
{

	PMString		sTitle   = kNullString;
	PMString		sEdition = kNullString;
	PMString		sStatus  = kNullString;
	PMString		sDateDeb = kNullString;
	PMString		sDateFin = kNullString;

	do{
		// On nettoie les champs infos
		this->doClearDialog();

		// On met a jour les listes des dates de debut et fin
		//this->doLoadDateRealPopup(kTrue);
		//this->doLoadDateRealPopupFin(kTrue);
		
		// On recupere les valeurs des popups
		sTitle = this->PopupGetString(kWidgetID_PopTitre);
		if(sTitle.IsEqual(kXPBCStringKey_EVERYTITLES) == kTrue) 
			sTitle = "#";

		sEdition = this->PopupGetString(kWidgetID_PopEdition);
		if(sEdition.IsEqual(kXPBCStringKey_EVERYEDITIONS) == kTrue)
			sEdition = "#";

		sStatus = this->PopupGetString(kWidgetID_PopEtatPub);
		if(sStatus.IsEqual(kXPBCStringKey_EVERYSTATUS) == kTrue)
			sStatus = "#";

		sDateDeb = this->PopupGetString(kWidgetID_PopDateParuDeb);
		//sDateDeb.Remove(sDateDeb.NumUTF16TextChars()- 4, 2);
		
		sDateFin = this->PopupGetString(kWidgetID_PopDateParuFin);
		//sDateFin.Remove(sDateFin.NumUTF16TextChars()- 4, 2);
	
		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());

		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);		

		// On recupere le controle de la liste
		IControlView* controlview = ipanel->FindWidget(kXPBCAnnoncesBoxWidgetID);
		
		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		K2Vector<ListePubStruct> PubList;
		bool success = xpubConnexion->ListePub(sTitle, sEdition, sStatus, sDateDeb, sDateFin, currentUser->Get(), PubList);
		if(success) {

			// Fill list on panel
            InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
            
            InterfacePtr<ITreeViewMgr> annoncesListTreeMgr(panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
            InterfacePtr<IAnnoncesList> annoncesListData (annoncesListTreeMgr, UseDefaultIID());
            K2Vector<annoncedata> listeAnnonces;
            
            int32 nbAnnonces = PubList.size();
            for(int32 i = 0 ; i < nbAnnonces ; ++i){
                annoncedata uneAnnonce;
                uneAnnonce.ID = PubList[i].IDPub;
                uneAnnonce.Status = PubList[i].Etat;
                uneAnnonce.Name = PubList[i].Name;
                if(::K2find(listeAnnonces, uneAnnonce) == listeAnnonces.end())
                    listeAnnonces.push_back(uneAnnonce);
            }
            
            // Launch tree repopulation
            annoncesListData->SetAnnoncesList(listeAnnonces);
            
            annoncesListTreeMgr->ClearTree(kTrue);
            annoncesListTreeMgr->ChangeRoot();
            
            // Select first item if any
            if(nbAnnonces > 0){
                InterfacePtr<ITreeViewHierarchyAdapter> annoncesListAdapter (annoncesListTreeMgr, UseDefaultIID());
                InterfacePtr<ITreeViewController> annoncesListController (annoncesListTreeMgr, UseDefaultIID());
                annoncesListController->Select(annoncesListAdapter->GetNthChild(annoncesListAdapter->GetRootNode(), 0));
            }
        } else {
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}
		
	} while(kFalse);

}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doShowAdInfos(void)
{
    this->doClearDialog();
	PMString cn = kNullString;
	PMString ad = kNullString;
	PMString te = kNullString;
	PMString po = kNullString;
	PMString em = kNullString;
	PMString wi = kNullString;
	PMString de = kNullString;
	
	InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
    do
	{
		InterfacePtr<ITreeViewController> annoncesListController (panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
		NodeIDList selectedItems;
		annoncesListController->GetSelectedItems(selectedItems);
        
		if(selectedItems.empty()) {
            break;
        }
        TreeNodePtr<XPBCAnnonceNodeID> nodeID (selectedItems[0]);
		// ID de l'annonce
		PMString idAnnonce = nodeID->GetID();
        
		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);

		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());
        
		bool16 success = xpubConnexion->InfoPub(idAnnonce, curPub);

		if(success) {			
			cn = curPub.IDSociete;						
			ad = curPub.ContactName;			
			te = curPub.ContactPhone;
			po = curPub.ContactCelullar;			
			em = curPub.ContactEmail;
			wi = curPub.Largeur;			
			de = curPub.Hauteur;
		}
		else
		{
			curPub = PubInfoStruct();
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}
		
	} while(kFalse);
	
	// Affichage des infos de l'annonce
    InterfacePtr<ITextControlData> clientnamedata (panelControlData->FindWidget(kWidgetID_LblClientName), UseDefaultIID());
    clientnamedata->SetString(cn);
    InterfacePtr<ITextControlData> adressdata (panelControlData->FindWidget(kWidgetID_LblAdress), UseDefaultIID());
    adressdata->SetString(ad);
    InterfacePtr<ITextControlData> teldata (panelControlData->FindWidget(kWidgetID_LblTel), UseDefaultIID());
    teldata->SetString(te);
    InterfacePtr<ITextControlData> portdata (panelControlData->FindWidget(kWidgetID_LblPort), UseDefaultIID());
    portdata->SetString(po);
    InterfacePtr<ITextControlData> maildata (panelControlData->FindWidget(kWidgetID_LblEmail), UseDefaultIID());
    maildata->SetString(em);
    InterfacePtr<ITextControlData> widthdata (panelControlData->FindWidget(kWidgetID_LblWidth), UseDefaultIID());
    widthdata->SetString(wi);
    InterfacePtr<ITextControlData> heigthdata (panelControlData->FindWidget(kWidgetID_LblDepth), UseDefaultIID());
    heigthdata->SetString(de);
    
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doClearDialog(void)
{
	InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
    
    InterfacePtr<ITextControlData> clientnamedata (panelControlData->FindWidget(kWidgetID_LblClientName), UseDefaultIID());
    clientnamedata->SetString(kNullString);
    InterfacePtr<ITextControlData> adressdata (panelControlData->FindWidget(kWidgetID_LblAdress), UseDefaultIID());
    adressdata->SetString(kNullString);
    InterfacePtr<ITextControlData> teldata (panelControlData->FindWidget(kWidgetID_LblTel), UseDefaultIID());
    teldata->SetString(kNullString);
    InterfacePtr<ITextControlData> portdata (panelControlData->FindWidget(kWidgetID_LblPort), UseDefaultIID());
    portdata->SetString(kNullString);
    InterfacePtr<ITextControlData> maildata (panelControlData->FindWidget(kWidgetID_LblEmail), UseDefaultIID());
    maildata->SetString(kNullString);
    InterfacePtr<ITextControlData> widthdata (panelControlData->FindWidget(kWidgetID_LblWidth), UseDefaultIID());
    widthdata->SetString(kNullString);
    InterfacePtr<ITextControlData> heigthdata (panelControlData->FindWidget(kWidgetID_LblDepth), UseDefaultIID());
    heigthdata->SetString(kNullString);
    
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doShowLoginDialog(void)
{
	do
	{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application (GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDialogMgr> dialogMgr (application, UseDefaultIID());
		
		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			kXPBCPluginID,				// Our Plug-in ID from BasicDialogID.h. 
			kViewRsrcType,				// This is the kViewRsrcType.
			kXPBCLoginDialogResourceID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
		);
		
		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).
		IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		if (dialog == nil)
		{ 
			ASSERT_FAIL("XPBCPanelWidgetObserver::doShowLoginDialog: can't create dialog"); 
			break;
		}
		
		// Open the dialog.
		dialog->Open();
		
		// IMPORTANT : la fonction WaitForDialog permet de bloquer l'execution de la suite du code appelant
		// le dialogue, et d'attendre sa fermeture pour reprise de l'execution.
		dialog->WaitForDialog();
		
	} while (false);
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
void XPBCAnnoncesPanelObserver::doUpdateControls(void)
{		
	bool16 frontDocIsPub = kFalse;
	bool16 connected = kFalse;
	bool16 wantBAT = kFalse;
	bool16 isProject = kFalse;

	do
	{		
		// Test sur gLogin (passee a kTrue si le service de connection a reussi)
		InterfacePtr<IBoolData> gLogin (GetExecutionContextSession(), IID_IBOOL_LOGIN);						
		connected = gLogin->GetBool();
		
		// Voir si on a un document ouvert
		IDocument* theFrontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (theFrontDoc) 
		{			
			InterfacePtr<IStringData> idPub (theFrontDoc, IID_IPUB_ID);		
			PMString theId = idPub->Get();
			frontDocIsPub = !theId.IsEmpty() && connected;

			InterfacePtr<IStringData> BATData (theFrontDoc, IID_IPUB_BAT);
			wantBAT = Utils<IDataStringUtils>()->StringToBool16(WideString(BATData->Get()));

			theId.ToLower();
			isProject = (theId.NumUTF16TextChars() > 2 && theId[0] == 'x' && theId[1] == 'p');
		}
        
         InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
        panelControlData->FindWidget(kXPBCWidgetID_BtnConnect)->Enable(!connected);
        panelControlData->FindWidget(kXPBCWidgetID_BtnDisconnect)->Enable(connected);
        panelControlData->FindWidget(kXPBCWidgetID_BtnRequest)->Enable(connected);
        panelControlData->FindWidget(kWidgetID_PopTitre)->Enable(connected);
        panelControlData->FindWidget(kWidgetID_PopEdition)->Enable(connected);
        panelControlData->FindWidget(kWidgetID_PopEtatPub)->Enable(connected);
        panelControlData->FindWidget(kWidgetID_PopDateParuDeb)->Enable(connected);
        panelControlData->FindWidget(kWidgetID_PopDateParuFin)->Enable(connected);
        panelControlData->FindWidget(kXPBCWidgetID_BtnOpen)->Enable(connected);
        panelControlData->FindWidget(kSIDPubEdtWidgetID)->Enable(connected);
        panelControlData->FindWidget(kXPBCWidgetID_BtnSearch)->Enable(connected);
        panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID)->Enable(connected);
        
        panelControlData->FindWidget(kXPBCWidgetID_BtnSave)->Enable(frontDocIsPub);
        panelControlData->FindWidget(kXPBCWidgetID_BtnSaveClose)->Enable(frontDocIsPub);
        panelControlData->FindWidget(kXPBCWidgetID_BtnBat)->Enable(frontDocIsPub && !isProject);
        panelControlData->FindWidget(kXPBCWidgetID_BtnTerminate)->Enable(frontDocIsPub && !wantBAT && !isProject);
        panelControlData->FindWidget(kXPBCWidgetID_BtnOpenPubFolder)->Enable(frontDocIsPub);
        
        InterfacePtr<ITextControlData> username (panelControlData->FindWidget(kXPBCUsernameLblWidgetID), UseDefaultIID());
        if(!connected)
		{	
            PMString notConnected(kXPBCNotConnectedStringKey);
			notConnected.Translate();
			username->SetString(notConnected);
		}
		else
		{
			InterfacePtr<IStringData> theUserName (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);
			username->SetString(theUserName->Get());
		}
						
	} while (kFalse);
		
}

// ------------------------------------------------------------------------------------
//
// ------------------------------------------------------------------------------------
PMString XPBCAnnoncesPanelObserver::PopupGetString(int DropDownListID)
{
	PMString result = kNullString;

	do{
		InterfacePtr<IPanelControlData> ipanel (this, UseDefaultIID());
	
		// On recupere le controle du popup
		IControlView* controlview = ipanel->FindWidget(DropDownListID);
		if (controlview == nil) {
			ASSERT_FAIL("controlview invalid");
			break;
		}
		
		InterfacePtr<IDropDownListController> iDropDownListController (controlview, UseDefaultIID());
		if (iDropDownListController == nil)
		{
			ASSERT_FAIL("iDropDownListController invalid");
			break;
		}

		InterfacePtr<IStringListControlData> iStringListControlData (controlview, UseDefaultIID());
		int32 row = iDropDownListController->GetSelected();
		if(row == IDropDownListController::kNoSelection)
			break;

		result = iStringListControlData->GetString(row);
		
	} while (false);
	
	return result;
}

void XPBCAnnoncesPanelObserver::doOpen()
{
	do
	{
        InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
        InterfacePtr<ITreeViewController> annoncesListController (panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
		NodeIDList selectedItems;
		annoncesListController->GetSelectedItems(selectedItems);
        
		if(selectedItems.empty())
            break;
        TreeNodePtr<XPBCAnnonceNodeID> nodeID (selectedItems[0]);
		// ID de l'annonce
		PMString idAnnonce = nodeID->GetID();

		// Ouverture de la pub
		if(Utils<IXPubClientUtils>()->OuvrirPub(curPub) != kSuccess) // An error occured while opening the document, don't lock it
			break;

		// Appel au service de verrouillage
		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);	

		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		bool16 success = xpubConnexion->ModifChampPub(idAnnonce,
			PMString(kXPBCStatusStringKey, PMString::kUnknownEncoding), 
			PMString(kXPBCEnCoursStringKey, PMString::kUnknownEncoding), currentUser->Get());

		if(success)
		{
			success = xpubConnexion->ModifChampPub(idAnnonce, 
			PMString(kXPBCVerrouStringKey, PMString::kUnknownEncoding),
			PMString(kXPBCVraiStringKey, PMString::kUnknownEncoding), currentUser->Get());
		}

		if(!success)
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}

		// Mise a jour des contrÙles
		this->doUpdateControls();

	} while(kFalse);
}

void XPBCAnnoncesPanelObserver::doSave(bool16 close)
{
	do
	{
		InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
        // Get selected pub
		InterfacePtr<ITreeViewController> annoncesListController (panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
		NodeIDList selectedItems;
		annoncesListController->GetSelectedItems(selectedItems);
        
		if(selectedItems.empty())
            break;
        TreeNodePtr<XPBCAnnonceNodeID> nodeID (selectedItems[0]);
		// ID de l'annonce
		PMString idAnnonce = nodeID->GetID();
        
        // Sauvegarde du document au premier plan
		IDocument* theFrontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if(!theFrontDoc)
			break;

		UIDRef docUIDRef = ::GetUIDRef(theFrontDoc);
		if(Utils<IDocumentCommands>()->Save(docUIDRef, kSuppressUI) != kSuccess)
		{
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::InformationAlert(kXPBCErrorSaveStringKey);
			break;
		}

		// Mise a jour du statut a "En cours"
		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);

		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		bool16 success = xpubConnexion->ModifChampPub(idAnnonce,
													  PMString(kXPBCStatusStringKey, PMString::kUnknownEncoding),
													  PMString(kXPBCEnCoursStringKey, PMString::kUnknownEncoding), 
													  currentUser->Get());
		if(!success)
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}
		
		if(close) // le deverrouillage est fait dans le DocResponder sur detection de l'evenement fermeture
		{			 
			InterfacePtr<IDocFileHandler> docFileHandler(Utils<IDocumentUtils>()->QueryDocFileHandler(docUIDRef));
			docFileHandler->Close(docUIDRef, kSuppressUI, kFalse, IDocFileHandler::kSchedule);
		}

	} while(kFalse);
}

void XPBCAnnoncesPanelObserver::doTerminate(bool16 forceBAT)
{
	ErrorCode terminateStatus = kFailure;
	do
	{
		IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if(!frontDoc)
			break;
		
		// Verification de l'integrite de la pub
		PubInfoStruct pubInfo;
		if(!Utils<IXPubClientUtils>()->VerifierPub(frontDoc, pubInfo))
			break;

		UIDRef pubRef = ::GetUIDRef(frontDoc);

		// Rassemblement des images (non bloquant si incomplet)
	//	Utils<IXPubClientUtils>()->RassemblerImages(frontDoc);

		// Rassemblement des polices (non bloquant si incomplet)
	//	Utils<IXPubClientUtils>()->RassemblerPolices(frontDoc);

		// Sauvegarde du document (la previsualisation est generee a ce moment la)
		if(Utils<IDocumentCommands>()->Save(pubRef, kSuppressUI) != kSuccess)
		{
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(kXPBCErrorSaveStringKey);
			break;
		}

		// Generation du PDF		
		if(Utils<IXPubClientUtils>()->PdfExport(frontDoc, pubInfo) != kSuccess)	
		{
			break;
		}
		
		
		// Géneration de l'assemblage.
		IDataBase * db = ::GetDataBase(frontDoc);
		if(db)
		{
			IDFile currentFolder; 
			if(db->GetSysFile() != nil)
			{
				// On detecte le dossier du document
				FileUtils::GetParentDirectory (*db->GetSysFile(), currentFolder); 
				if(Utils<IXPubClientUtils>()->PackageExport(frontDoc, currentFolder) != kSuccess)
				{
					ErrorUtils::PMSetGlobalErrorCode(kSuccess);
					CAlert::ErrorAlert(kXPBCErrorPackgeExportStringKey);
					break;
				} 	 
			}		
		}
		
#if WINDOWS
		// Copie dans le dossier TEMP en local pour affichage dans l'application par defaut
		IDFile tempPDF = FileUtils::PMStringToSysFile(PMString(::getenv("TEMP"),PMString::kUnknownEncoding));
		if(!FileUtils::DoesFileExist(tempPDF))
			tempPDF = FileUtils::PMStringToSysFile(PMString(getenv("TMP"),PMString::kUnknownEncoding));

		FileUtils::AppendPath(&tempPDF, pubInfo.IDPub + ".pdf");
		FileUtils::CopyFile(FileUtils::PMStringToSysFile(pubInfo.FichierPDF), tempPDF);

		// Ouverture du PDF dans l'application par defaut
		//TODO::ShellExecute(NULL, NULL, FileUtils::SysFileToPMString(tempPDF).GrabTString(), NULL, NULL, SW_SHOWNORMAL);
#endif
		
		// Renvoi des informations vers XPub
		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);
		
		// On notifie XPub de la hauteur finale de la pub
		bool16 success = xpubConnexion->ModifChampPub(pubInfo.IDPub, 
			PMString(kXPBCHauteurStringKey, PMString::kUnknownEncoding), pubInfo.Hauteur, currentUser->Get());

		// On determine le statut en fonction du type de bouton clique et de la propriete BAT de la pub
		PMString status;		 
		if(pubInfo.BAT)
			status = kXPBCBATStringKey;
		else
		{
			if(forceBAT)
			{
				status = kXPBCBATStringKey;

				// Un BAT a ete demande sur une pub non BAT, on force sa propriete
				success = xpubConnexion->ModifChampPub(pubInfo.IDPub, 
													   PMString(kXPBCBATStringKey, PMString::kUnknownEncoding), 
													   PMString(kXPBCVraiStringKey, PMString::kUnknownEncoding), 
													   currentUser->Get());
			}
			else
				status = kXPBCFinieStringKey;

		}
		status.Translate();
			
		// Mise a jour du statut											
		success = success && xpubConnexion->ModifChampPub(pubInfo.IDPub, 
														  PMString(kXPBCStatusStringKey, PMString::kUnknownEncoding), 
														  status, 
														  currentUser->Get());		
		
		if(!success)
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}

		// Fermeture du doc
		InterfacePtr<IDocFileHandler> docFileHandler(Utils<IDocumentUtils>()->QueryDocFileHandler(pubRef));
		docFileHandler->Close(pubRef, kSuppressUI, kFalse, IDocFileHandler::kSchedule);

		terminateStatus = kSuccess;

	} while(kFalse);	

	if(terminateStatus != kSuccess)	
		CAlert::ErrorAlert(kXPBCErrorTerminateStringKey);	
}

void XPBCAnnoncesPanelObserver::doSearch()
{
	do
	{
		InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
        
        InterfacePtr<ITextControlData> searchData (panelControlData->FindWidget(kSIDPubEdtWidgetID), UseDefaultIID());
        PMString searchString = searchData->GetString();
    
		// Appel du webservice de recherche
		InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);		

		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		K2Vector<ListePubStruct> PubList;
		bool success = xpubConnexion->ListePubRecherche(searchString, currentUser->Get(), PubList);
		if(success) 
		{
			// Fill list on panel
            InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
            
            InterfacePtr<ITreeViewMgr> annoncesListTreeMgr(panelControlData->FindWidget(kXPBCAnnoncesBoxWidgetID), UseDefaultIID());
            InterfacePtr<IAnnoncesList> annoncesListData (annoncesListTreeMgr, UseDefaultIID());
            K2Vector<annoncedata> listeAnnonces;
            
            int32 nbAnnonces = PubList.size();
            for(int32 i = 0 ; i < nbAnnonces ; ++i){
                annoncedata uneAnnonce;
                uneAnnonce.ID = PubList[i].IDPub;
                uneAnnonce.Status = PubList[i].Etat;
                uneAnnonce.Name = PubList[i].Name;
                if(::K2find(listeAnnonces, uneAnnonce) == listeAnnonces.end())
                    listeAnnonces.push_back(uneAnnonce);
            }
            
            // Launch tree repopulation
            annoncesListData->SetAnnoncesList(listeAnnonces);
            
            annoncesListTreeMgr->ClearTree(kTrue);
            annoncesListTreeMgr->ChangeRoot();
            
            // Select first item if any
            if(nbAnnonces > 0){
                InterfacePtr<ITreeViewHierarchyAdapter> annoncesListAdapter (annoncesListTreeMgr, UseDefaultIID());
                InterfacePtr<ITreeViewController> annoncesListController (annoncesListTreeMgr, UseDefaultIID());
                annoncesListController->Select(annoncesListAdapter->GetNthChild(annoncesListAdapter->GetRootNode(), 0));
            }
            
		}
		else
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
		}

	} while(kFalse);
}

void XPBCAnnoncesPanelObserver::doOpenPubFolder()
{
	do
	{
		// On recupËre le dossier de la pub
		if(!curFrontDoc)
			break;
		
		const IDFile * pubFile = ::GetDataBase(curFrontDoc)->GetSysFile();
		if(!pubFile)
			break;

		IDFile pubFolder;
		FileUtils::GetParentDirectory(*pubFile, pubFolder);
		
#ifdef WINDOWS
		//TODO::ShellExecute(NULL,NULL,FileUtils::SysFileToPMString(pubFolder).GrabTString(), NULL,NULL, SW_SHOWNORMAL);
#endif
	} while(kFalse);
	
}