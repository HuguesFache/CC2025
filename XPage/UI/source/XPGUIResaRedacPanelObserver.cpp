
#include "VCPluginHeaders.h"

// Interface includes
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ISubject.h"
#include "IApplication.h"
#include "IDocument.h"
#include "IXPageSuite.h"
#include "ISelectionManager.h"
#include "DebugClassUtils.h"
#include "IDialogMgr.h"
#include "IApplication.h"
#include "IDialog.h"
#include "RsrcSpec.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "IFormeData.h"
#include "IResaRedacDataList.h"
#include "IWebServices.h"
#include "IXPGPreferences.h"
#include "IClassIDData.h"
#include "IXPageUtils.h"
#include "IPanelMgr.h"

// General includes
#include "SelectionObserver.h"
#include "UIDList.h"
#include "XPageUIUtils.h"
#include "CAlert.h"
#include "ErrorUtils.h"

// Project includes
#include "XPGUIFormeNodeID.h"
#include "XPGUIID.h"
#include "XPGID.h"

/** XPGUIResaRedacPanelObserver

	@author Trias Developpement
*/

class XPGUIResaRedacPanelObserver : public ActiveSelectionObserver
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPGUIResaRedacPanelObserver( IPMUnknown* boss );
	/** Destructor
	*/
	virtual ~XPGUIResaRedacPanelObserver() ;
	
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

	PMString selectedFormeID;
	UIDList formeTextFrames, formeGraphicFrames;

	void UpdatePanelWidgets();

	void DoApplyResa();
	void DoUnApplyResa();

	void DoComputeCalibrage();
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIResaRedacPanelObserver, kXPGUIResaRedacPanelObserverImpl)

/* Constructor
*/
XPGUIResaRedacPanelObserver::XPGUIResaRedacPanelObserver( IPMUnknown* boss ) :
	ActiveSelectionObserver( boss ), selectedFormeID(kNullString)
{
}

/* Destructor
*/
XPGUIResaRedacPanelObserver::~XPGUIResaRedacPanelObserver()
{
}

/* AutoAttach
*/
void XPGUIResaRedacPanelObserver::AutoAttach()
{
	ActiveSelectionObserver::AutoAttach();

	do
	{		
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
		{
			ASSERT_FAIL("AutoAttach() invalid panelControlData");
			break;
		}
 
		// Observe buttons and change of selection in forme list
		AttachWidget(panelControlData, kXPGUIApplyResaWidgetID,		IID_IBOOLEANCONTROLDATA);
		AttachWidget(panelControlData, kXPGUIUnApplyResaWidgetID,		IID_IBOOLEANCONTROLDATA);
		AttachWidget(panelControlData, kXPGUIComputeCalibrageWidgetID,		IID_ITRISTATECONTROLDATA);

		// Initialise widget state.
		this->HandleSelectionChanged(nil);
		
	} while(false);
	
}

/* AutoDetach
*/
void XPGUIResaRedacPanelObserver::AutoDetach()
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

		DetachWidget(panelControlData, kXPGUIApplyResaWidgetID,		IID_IBOOLEANCONTROLDATA);
		DetachWidget(panelControlData, kXPGUIUnApplyResaWidgetID,		IID_IBOOLEANCONTROLDATA);
		DetachWidget(panelControlData, kXPGUIComputeCalibrageWidgetID,		IID_ITRISTATECONTROLDATA);

	} while(false);
}

/* Update
*/
void XPGUIResaRedacPanelObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{	
	ActiveSelectionObserver::Update(theChange, theSubject, protocol, changedBy);	

	switch(protocol.Get())
	{
		case IID_IBOOLEANCONTROLDATA:
			{
				if(theChange == kTrueStateMessage)
				{
					InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
					if(controlView->GetWidgetID() == kXPGUIApplyResaWidgetID)
					{
						DoApplyResa();
					}
					else if(controlView->GetWidgetID() == kXPGUIUnApplyResaWidgetID)
					{
						DoUnApplyResa();
					}
				}
			}
			break;

		case IID_ITRISTATECONTROLDATA:
			{
				if(theChange == kTrueStateMessage)
				{
					InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
					if(controlView->GetWidgetID() == kXPGUIComputeCalibrageWidgetID)
					{
						DoComputeCalibrage();
					}
				}
			}
			break;


		default:
			break;
	}
}		


/* AttachWidget
*/
void XPGUIResaRedacPanelObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
void XPGUIResaRedacPanelObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
void XPGUIResaRedacPanelObserver::HandleSelectionChanged(const ISelectionMessage * message)
{
	selectedFormeID = kNullString;
	formeTextFrames = UIDList();
	formeGraphicFrames = UIDList();

	InterfacePtr<IXPageSuite> xpgSuite (fCurrentSelection, IID_IXPAGESUITE);
	if(xpgSuite != nil) // nil means no active selection => don't activate
	{
		UIDList formeItems;
		xpgSuite->GetSelectedForme(selectedFormeID, formeTextFrames, formeGraphicFrames, formeItems);		
	}	

	UpdatePanelWidgets();
}

/* UpdatePanelWidgets
*/
void XPGUIResaRedacPanelObserver::UpdatePanelWidgets()
{
	bool16 isFormeSelected = (selectedFormeID != kNullString);
	
	// Enable/Disable all widgets on the panel
	InterfacePtr<IControlView> panelView(this, UseDefaultIID());
	panelView->Enable(isFormeSelected);

	// Load widgets content
	InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());

	InterfacePtr<ITextControlData> topicData (panelControlData->FindWidget(kXPGUITopicWidgetID), UseDefaultIID());
	InterfacePtr<ITextControlData> nbSignsData (panelControlData->FindWidget(kXPGUICalibrageTextWidgetId), UseDefaultIID());
	InterfacePtr<ITextControlData> nbImagesData (panelControlData->FindWidget(kXPGUINbImagesTextWidgetId), UseDefaultIID());
	InterfacePtr<ITextControlData> applyBtnData (panelControlData->FindWidget(kXPGUIApplyResaWidgetID), UseDefaultIID());
	IControlView* unApplyBtnView = panelControlData->FindWidget(kXPGUIUnApplyResaWidgetID);

	bool16 enableUnApply = kFalse;
	PMString applyBtnLabel (kXPGUICreateResaKey);
	PMString topic = kNullString, nbSigns = kNullString, nbImages = kNullString, comment = kNullString;
	if(isFormeSelected)
	{
		InterfacePtr<IResaRedacDataList> docResaDataList (fCurrentSelection->GetDataBase(), fCurrentSelection->GetDataBase()->GetRootUID(), UseDefaultIID());
		ResaRedac assignedResa = (docResaDataList != nil) ? docResaDataList->GetResaRedac(selectedFormeID) : kInvalidResaRedac;

		if(assignedResa != kInvalidResaRedac)
		{
			enableUnApply = kTrue;
			applyBtnLabel = PMString(kXPGUIUpdateResaKey);
			topic = assignedResa.topic;
			nbSigns.AppendNumber(assignedResa.nbSigns);
			nbImages.AppendNumber(assignedResa.nbImages);
			comment = assignedResa.comment;
		}
		else
		{
			// Pre-fill number of images
			nbImages.AppendNumber(formeGraphicFrames.Length());
		}
	}

	topicData->SetString(topic);
	nbSignsData->SetString(nbSigns);
	nbImagesData->SetString(nbImages);
	unApplyBtnView->Enable(enableUnApply);
	applyBtnData->SetString(applyBtnLabel);
}

/* DoApplyResa
*/
void XPGUIResaRedacPanelObserver::DoApplyResa()
{
	do
	{
		// Get new resa data
		ResaRedac resa;

		InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());

		InterfacePtr<ITextControlData> topicData (panelControlData->FindWidget(kXPGUITopicWidgetID), UseDefaultIID());
		InterfacePtr<ITextControlData> nbSignsData (panelControlData->FindWidget(kXPGUICalibrageTextWidgetId), UseDefaultIID());
		InterfacePtr<ITextControlData> nbImagesData (panelControlData->FindWidget(kXPGUINbImagesTextWidgetId), UseDefaultIID());
		
		resa.topic = topicData->GetString();
		if(resa.topic.IsEmpty())
		{
			CAlert::InformationAlert(PMString(kXPGUIInvalidResaTopicKey));
			break;
		}

		PMString::ConversionError cError = PMString::kNoError;
		resa.nbSigns = nbSignsData->GetString().GetAsNumber(&cError);
		if(cError != PMString::kNoError)
		{
			CAlert::InformationAlert(PMString(kXPGUIInvalidResaNbSignsKey));
			break;
		}
		
		resa.nbImages = nbImagesData->GetString().GetAsNumber(&cError);
		if(cError != PMString::kNoError)
		{
			CAlert::InformationAlert(PMString(kXPGUIInvalidResaNbImagesKey));
			break;
		}

		// Check whether it is a new resa or an update
		bool16 newResa = kFalse;
		if(selectedFormeID != kNullString)
		{
			InterfacePtr<IResaRedacDataList> docResaDataList (fCurrentSelection->GetDataBase(), fCurrentSelection->GetDataBase()->GetRootUID(), UseDefaultIID());
			ResaRedac assignedResa = (docResaDataList != nil) ? docResaDataList->GetResaRedac(selectedFormeID) : kInvalidResaRedac;

			resa.id = assignedResa.id;
			newResa = (resa.id == -1);
		}

		if(newResa) // New resa => request an ID from XRail
		{
			InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
			xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL());

			if(!xrailConnexion->GetNewIDResaRedac(resa.id))
			{
				PMString error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::InformationAlert(error);	
				break;
			}
		}

		// Add resa to list
		InterfacePtr<IResaRedacDataList> docResaDataList (fCurrentSelection->GetDataBase(), fCurrentSelection->GetDataBase()->GetRootUID(), UseDefaultIID());
		if(!docResaDataList)
			break;

		InterfacePtr<ICommand> applyResaCmdBoss (CmdUtils::CreateCommand(kXPGSetResaRedacDataListCmdBoss));
		applyResaCmdBoss->SetItemList(UIDList(docResaDataList));

		InterfacePtr<IResaRedacDataList> cmdData (applyResaCmdBoss, UseDefaultIID());
		cmdData->CopyFrom(docResaDataList);
		cmdData->AddResaRedac(selectedFormeID, resa);

		if(CmdUtils::ProcessCommand(applyResaCmdBoss) != kSuccess)
			break;

		if(newResa)
		{
			// Add the adornment to forme items
			UIDList formeSplineItems (formeGraphicFrames);
			formeSplineItems.Append(formeTextFrames);

			InterfacePtr<ICommand>  addPageItemAdornmentCmd(CmdUtils::CreateCommand(kAddPageItemAdornmentCmdBoss));
			addPageItemAdornmentCmd->SetItemList(formeSplineItems);

			InterfacePtr<IClassIDData>  classIDData(addPageItemAdornmentCmd, UseDefaultIID());
			classIDData->Set(kXPGUIResaRedacAdornmentBoss);
			
			if(CmdUtils::ProcessCommand(addPageItemAdornmentCmd) != kSuccess)
				break;
		}

		// Update panel display
		UpdatePanelWidgets();

	}while(kFalse);
}

/* DoUnApplyResa
*/
void XPGUIResaRedacPanelObserver::DoUnApplyResa()
{
	do
	{
		// Remove resa from list
		InterfacePtr<IResaRedacDataList> docResaDataList (fCurrentSelection->GetDataBase(), fCurrentSelection->GetDataBase()->GetRootUID(), UseDefaultIID());
		if(!docResaDataList)
			break;

		InterfacePtr<ICommand> unApplyResaCmdBoss (CmdUtils::CreateCommand(kXPGSetResaRedacDataListCmdBoss));
		unApplyResaCmdBoss->SetItemList(UIDList(docResaDataList));

		InterfacePtr<IResaRedacDataList> cmdData (unApplyResaCmdBoss, UseDefaultIID());
		cmdData->CopyFrom(docResaDataList);
		cmdData->RemoveResaRedac(selectedFormeID);

		if(CmdUtils::ProcessCommand(unApplyResaCmdBoss) != kSuccess)
			break;

		// Remove the adornment from the forme items
		UIDList formeSplineItems (formeGraphicFrames);
		formeSplineItems.Append(formeTextFrames);

		InterfacePtr<ICommand>  removePageItemAdornmentCmd(CmdUtils::CreateCommand(kRemovePageItemAdornmentCmdBoss));
		removePageItemAdornmentCmd->SetItemList(formeSplineItems);

		InterfacePtr<IClassIDData>  classIDData(removePageItemAdornmentCmd, UseDefaultIID());
		classIDData->Set(kXPGUIResaRedacAdornmentBoss);
		
		if(CmdUtils::ProcessCommand(removePageItemAdornmentCmd) != kSuccess)
			break;

		// Update panel display (should clean the fields)
		UpdatePanelWidgets();

	}while(kFalse);
}

/* DoComputeCalibrage
*/
void XPGUIResaRedacPanelObserver::DoComputeCalibrage()
{
	if(selectedFormeID != kNullString)
	{
		int32 nbSignesEstim = 0, nbLinesEstim = 0, nbWordsEstim = 0, nbParaEstim = 0;
		Utils<IXPageUtils>()->GetStatisticsText(formeTextFrames, nbSignesEstim, nbParaEstim, nbWordsEstim, nbLinesEstim);		
		PMString calibrage = kNullString;
		calibrage.AppendNumber(nbSignesEstim);
		calibrage.SetTranslatable(kFalse);

		InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());

		InterfacePtr<ITextControlData> nbSignsData (panelControlData->FindWidget(kXPGUICalibrageTextWidgetId), UseDefaultIID());
		nbSignsData->SetString(calibrage);
	}
}