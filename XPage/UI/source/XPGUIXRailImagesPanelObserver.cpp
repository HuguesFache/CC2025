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
#include "ILayoutUIUtils.h"
#include "IXPageMgrAccessor.h"
#include "IImageDataModel.h"
#include "ITreeViewMgr.h"
#include "IPMUnknownData.h"
#include "ITriStateControlData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "ITreeViewController.h"
#include "ISysFileData.h"
#include "IDocumentList.h"
#include "IXPageSuite.h"
#include "IAssignmentMgr.h"
#include "IAssignment.h"
#include "IAssignedStory.h"
#include "IAssignmentUtils.h"
#include "ITreeNodeIDData.h"
#include "IStringListControlData.h"
#include "IPlacedArticleData.h"
#include "IXPageUtils.h"
#include "IFormeData.h"
#include "IDropDownListController.h"

// General includes
#include "SelectionObserver.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "ErrorUtils.h"
#include "IStringListData.h"
#include "StringNodeID.h"

// Project includes
#include "XPGUIImageNodeID.h"
#include "XPGUIID.h"
#include "XPGID.h"

/** XPGUIXRailImagesPanelObserver

	@author Trias Developpement
*/

class XPGUIXRailImagesPanelObserver : public ActiveSelectionObserver
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPGUIXRailImagesPanelObserver(IPMUnknown* boss);
	/** Destructor
	*/
	virtual ~XPGUIXRailImagesPanelObserver();

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
	void AttachDocList(void);
	void DetachDocList(void);

	void DoApplyIndex();

	IDocument * curFrontDoc;
	bool16 firstInit;
	UID currentFilter;
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIXRailImagesPanelObserver, kXPGUIXRailImagesPanelObserverImpl)

/* Constructor
*/
XPGUIXRailImagesPanelObserver::XPGUIXRailImagesPanelObserver(IPMUnknown* boss) :
	ActiveSelectionObserver(boss), curFrontDoc(nil), firstInit(kTrue), currentFilter(kInvalidUID)
{
	// Get notification when a refresh is required
	InterfacePtr<ISubject> sessionSubject(GetExecutionContextSession(), UseDefaultIID());
	sessionSubject->AttachObserver(this, IID_IREFRESHPROTOCOL);
}

/* Destructor
*/
XPGUIXRailImagesPanelObserver::~XPGUIXRailImagesPanelObserver()
{
}

/* AutoAttach
*/
void XPGUIXRailImagesPanelObserver::AutoAttach()
{
	// set data model for tree view widget		
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	InterfacePtr<IPMUnknownData> dataModel(panelControlData->FindWidget(kXPGUIImagesViewWidgetID), IID_IDATAMODEL);
	InterfacePtr<IXPageMgrAccessor> pageMrgAccessor(GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IImageDataModel> imageDataModel(pageMrgAccessor->QueryImageDataModel());
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

		if (firstInit)
		{
			// Unselect "Black & White" photo checkbox by default
			InterfacePtr<ITriStateControlData> bwCheckBox(panelControlData->FindWidget(kXPGUIBlackWhiteWidgetID), UseDefaultIID());
			bwCheckBox->Deselect();
			firstInit = kFalse;

			// Find dropdown list menu control view from panel data.
			IControlView* pDropDownListControlView = panelControlData->FindWidget(kXPGUIImageChangeTypeWidgetID);
			if (pDropDownListControlView == nil)
			{
				break;
			}

			// Get IDropDownListController interface pointer.
			InterfacePtr<IDropDownListController> pDropDownListController(pDropDownListControlView, UseDefaultIID());
			if (pDropDownListController == nil)
			{
				break;
			}
			pDropDownListController->Select(0);
		}

		// On attache le DocumentList observer, afin de pouvoir detecter un changement de selection de document, et
		// pouvoir reagir (dans le Update de cette classe).
		this->AttachDocList();

		AttachWidget(panelControlData, kXPGUIImageChangeIndexBtnWidgetID, IID_IBOOLEANCONTROLDATA);

		// On initialise le document courant au premier plan
		curFrontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();

		// Update model
		this->HandleSelectionChanged(nil);

	} while (false);

}

/* AutoDetach
*/
void XPGUIXRailImagesPanelObserver::AutoDetach()
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

		this->DetachDocList();
		DetachWidget(panelControlData, kXPGUIImageChangeIndexBtnWidgetID, IID_IBOOLEANCONTROLDATA);

	} while (false);

}

/* Update
*/
void XPGUIXRailImagesPanelObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{
	ActiveSelectionObserver::Update(theChange, theSubject, protocol, changedBy);
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());

	if ((theChange == kSetFrontDocCmdBoss || theChange == kCloseDocCmdBoss) && (protocol == IID_IDOCUMENTLIST))
	{
		IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (frontDoc != curFrontDoc)
		{
			curFrontDoc = frontDoc;

			InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
			this->HandleSelectionChanged(nil);
		}
	}
	else if (theChange == kXPGRefreshMsg && protocol == IID_IREFRESHPROTOCOL)
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		this->HandleSelectionChanged(nil);
	}

	switch (protocol.Get())
	{
		case IID_IBOOLEANCONTROLDATA:
		{
			if (theChange == kTrueStateMessage)
			{
				InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
				if (controlView->GetWidgetID() == kXPGUIImageChangeIndexBtnWidgetID)
				{
					DoApplyIndex();
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
void XPGUIXRailImagesPanelObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
	} while (false);
}

/* DetachWidget
*/
void XPGUIXRailImagesPanelObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
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
	} while (false);
}

/* AttachDocList
*/
void XPGUIXRailImagesPanelObserver::AttachDocList(void)
{
	do
	{
		InterfacePtr<IApplication> app(GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDocumentList> doclist(app->QueryDocumentList());
		InterfacePtr<ISubject> subject(doclist, UseDefaultIID());
		subject->AttachObserver(this, IID_IDOCUMENTLIST);
	} while (false);
}

/* DetachDocList
*/
void XPGUIXRailImagesPanelObserver::DetachDocList(void)
{
	do
	{
		InterfacePtr<IApplication> app(GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDocumentList> doclist(app->QueryDocumentList());
		InterfacePtr<ISubject> subject(doclist, UseDefaultIID());
		subject->DetachObserver(this, IID_IDOCUMENTLIST);
	} while (false);
}

/* HandleSelectionChanged
*/
void XPGUIXRailImagesPanelObserver::HandleSelectionChanged(const ISelectionMessage * message)
{
	PMString idArticle = kNullString;
	IDFile assignFile;
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	do {

		IControlView * imagePanelView = panelControlData->FindWidget(kXPGUIDisplayImagePanelWidgetID);
		InterfacePtr<ITreeViewMgr> treeViewMgr(panelControlData->FindWidget(kXPGUIImagesViewWidgetID), UseDefaultIID());

		InterfacePtr<ISysFileData> imagePanelFileData(imagePanelView, UseDefaultIID());
		imagePanelFileData->Set(IDFile());
		imagePanelView->Invalidate();

		IControlView* displayCreditView = panelControlData->FindWidget(kXPGUICreditTextWidgetID);
		InterfacePtr<ITextControlData> creditPanelView(displayCreditView, UseDefaultIID());
		creditPanelView->SetString(kNullString);

		IControlView* displaylegendView = panelControlData->FindWidget(kXPGUILegendeTextWidgetID);
		InterfacePtr<ITextControlData> legendPanelView(displaylegendView, UseDefaultIID());
		legendPanelView->SetString(kNullString);

		PMString contentIndex = "", indexString = "-1";
		int32 indexitem = 0, typeitem = 0;
		
		InterfacePtr<IXPageSuite> xpgSuite(fCurrentSelection, IID_IXPAGESUITE);
		if (xpgSuite == nil) {
			if (currentFilter == kInvalidUID) {
				treeViewMgr->ClearTree();
				treeViewMgr->ChangeRoot(kTrue);
				currentFilter = kInvalidUID;
			break;
			}
		}
		else {


		UIDList textFrames, graphicFrames, selectedItems;
		xpgSuite->GetSelectedItems(textFrames, graphicFrames, selectedItems);
			if (selectedItems.size() == 0) {
			treeViewMgr->ClearTree();
			treeViewMgr->ChangeRoot(kTrue);
			break;
		}
		if (currentFilter == selectedItems.At(0)) {
			treeViewMgr->ClearTree();
			treeViewMgr->ChangeRoot(kTrue);
			break;
		}
		currentFilter = selectedItems.At(0);
		// Gather all InCopy assignments
		InterfacePtr<IAssignmentMgr> assignMgr(GetExecutionContextSession(), UseDefaultIID());
		UIDList storyFrames(textFrames);
		storyFrames.Append(graphicFrames);
		K2Vector<PMString> assignFilePaths;
		for (int32 i = 0; i < storyFrames.Length(); ++i)
		{
			UIDRef storyRef = storyFrames.GetRef(i);
				InterfacePtr<IFormeData> currentData(storyRef, UseDefaultIID());
				indexitem = currentData->GetPhotoIndex();
				typeitem = currentData->GetType();
				if (currentData) {
					switch (typeitem) {
						case IFormeData::kPhoto:
							contentIndex = "Image ";
							contentIndex.AppendNumber(currentData->GetPhotoIndex());
							break;
						case IFormeData::kCredit:
							contentIndex = "Credit ";
							contentIndex.AppendNumber(currentData->GetPhotoIndex());
							break;
						case IFormeData::kLegend:
							contentIndex = "Legende ";
							contentIndex.AppendNumber(currentData->GetPhotoIndex());
							break;
					default:
						break;
					}
				}
			Utils<IAssignmentUtils>()->NormalizeTextModelRef(storyRef);

			// Get assignment related to this story
			InterfacePtr<IAssignment> assignment(nil);
			InterfacePtr<IAssignedStory> assignedStory(nil);
			assignMgr->QueryAssignmentAndAssignedStory(storyRef, assignment, assignedStory);

			if (assignment == nil || assignedStory == nil)
				continue;

			PMString assignFilePath = assignment->GetFile();
			if (::K2find(assignFilePaths, assignFilePath) == assignFilePaths.end())
				assignFilePaths.push_back(assignFilePath);
		}

		if (assignFilePaths.size() == 1)
		{
			assignFile = FileUtils::PMStringToSysFile(assignFilePaths[0]);
			idArticle = Utils<IXPageUtils>()->GetStoryIDFromAssignmentFile(assignFile);
		}
		// Try with placed story
		if (assignFilePaths.size() == 0)
		{
			InterfacePtr<IPlacedArticleData> placedArticleData(storyFrames.GetRef(0), UseDefaultIID());
			if (placedArticleData)
			{
				if (placedArticleData->GetUniqueId() != kNullString)
				{
					idArticle = placedArticleData->GetUniqueId();
					assignFile = FileUtils::PMStringToSysFile(placedArticleData->GetStoryFolder());
					FileUtils::AppendPath(&assignFile, idArticle + ".OBJRART.xml");
					//CAlert::InformationAlert(FileUtils::SysFileToPMString(assignFile));
				}
			}
		}
		}
		
		InterfacePtr<ITextControlData> indexBloc(panelControlData->FindWidget(kXPGUIImageIndexWidgetID), UseDefaultIID());
		if (indexBloc) {
			indexBloc->SetString(contentIndex);
		}
		InterfacePtr<ITextControlData> indexRollOverBloc(panelControlData->FindWidget(kXPGUIImageChangeIndexWidgetID), UseDefaultIID());
		if (indexRollOverBloc) {
			PMString temp = "";
			temp.AppendNumber(indexitem);
			indexRollOverBloc->SetString(temp);
		}

		// Find dropdown list menu control view from panel data.
		IControlView* pDropDownListControlView = panelControlData->FindWidget(kXPGUIImageChangeTypeWidgetID);
		if (pDropDownListControlView == nil)
		{
			break;
		}

		// Get IDropDownListController interface pointer.
		InterfacePtr<IDropDownListController> pDropDownListController(pDropDownListControlView, UseDefaultIID());
		if (pDropDownListController == nil)
		{
			break;
		}
		pDropDownListController->Select(typeitem);
		
		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor(GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IImageDataModel> model(pageMrgAccessor->QueryImageDataModel());
		if (idArticle == kNullString)
			model->SetFilter(kNullString, kNullString); // Reset filters

		else if (idArticle.Contains("-"))
		{
			int32 extPos = idArticle.LastIndexOfCharacter(PlatformChar('-'));
			if (extPos != -1)
				idArticle.Remove(extPos, idArticle.NumUTF16TextChars() - extPos);
			model->SetFilter(idArticle, FileUtils::SysFileToPMString(assignFile));
		}
		else
			model->SetFilter(idArticle, FileUtils::SysFileToPMString(assignFile));
		model->Rebuild(kNullString);

		treeViewMgr->ClearTree();
		treeViewMgr->ChangeRoot(kTrue);
		InterfacePtr<ITreeViewHierarchyAdapter> imagesHierAdapter(treeViewMgr, UseDefaultIID());
		NodeID imageRootNode = imagesHierAdapter->GetRootNode();
		if (imagesHierAdapter->GetNumChildren(imageRootNode) > 0)
		{
			InterfacePtr<ITreeViewController> imagesListController(imagesHierAdapter, UseDefaultIID());
			imagesListController->Select(imagesHierAdapter->GetNthChild(imagesHierAdapter->GetRootNode(), 0));
		}
	} while (kFalse);

}



/* DoApplyResa
*/
void XPGUIXRailImagesPanelObserver::DoApplyIndex()
{
	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		int32 nouvelIndex = -1;

		InterfacePtr<ITextControlData> indexRollOverBloc(panelControlData->FindWidget(kXPGUIImageChangeIndexWidgetID), UseDefaultIID());
		if (indexRollOverBloc) {
			PMString::ConversionError cError = PMString::kNoError;
			nouvelIndex = indexRollOverBloc->GetString().GetAsNumber(&cError);
		}
		// Find dropdown list menu control view from panel data.
		IControlView* pDropDownListControlView = panelControlData->FindWidget(kXPGUIImageChangeTypeWidgetID);
		if (pDropDownListControlView == nil)
		{
			break;
		}

		// Get IDropDownListController interface pointer.
		InterfacePtr<IDropDownListController> pDropDownListController(pDropDownListControlView, UseDefaultIID());
		if (pDropDownListController == nil)
		{
			break;
		}
		int32 selectedIndex = pDropDownListController->GetSelected();

		InterfacePtr<IXPageSuite> xpgSuite(fCurrentSelection, IID_IXPAGESUITE);
		if (xpgSuite == nil) {
			return;
		}
		else {


			UIDList textFrames, graphicFrames, selectedItems;
			xpgSuite->GetSelectedItems(textFrames, graphicFrames, selectedItems);
			if (selectedItems.size() > 0) {

				UIDList storyFrames(textFrames);
				storyFrames.Append(graphicFrames);
				for (int32 i = 0; i < storyFrames.Length(); ++i)
				{
					UIDRef storyRef = storyFrames.GetRef(i);
					InterfacePtr<IFormeData> currentData(storyRef, UseDefaultIID());
					if (currentData) {
						currentData->SetPhotoIndex(nouvelIndex);
						currentData->SetType(selectedIndex);

					}

				}

			}

		}


	} while (kFalse);
}