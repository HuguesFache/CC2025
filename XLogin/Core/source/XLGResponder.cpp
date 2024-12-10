#include "VCPlugInHeaders.h"

// Interface includes:
#include "IClassIDData.h"
#include "ISignalMgr.h"
#include "IDocument.h"
#include "IDocumentSignalData.h"
#include "IUIDData.h"
#include "IDialog.h"
#include "CAlert.h"
#include "ISession.h"
#include "IUIDRefListData.h"
#include "IWebServices.h"
#include "IBoolData.h"
#include "IPageList.h"
#include "INewLayerCmdData.h"
#include "ILayerList.h"
#include "IUnitOfMeasure.h"
#include "IGeometry.h"
#include "ISpread.h"
#include "IHierarchy.h"
#include "IDocumentLayer.h"
#include "ISpread.h"
#include "ISpreadLayer.h"
#include "IDialogMgr.h"
#include "IApplication.h"
#include "LocaleSetting.h"
#include "InCopyBridgeUIID.h"
#include "IItemLockData.h"
#include "IInCopyBridgeUtils.h"
#include "ISelectionManager.h"
#include "ISelectionUtils.h"
#include "IAssignmentMgr.h"
#include "IAssignmentUtils.h"
#include "IItemLockData.h"
#include "ITextModel.h"
#include "IUIFlagData.h"
//#include "IInCopyBridgeSelectionSuite.h"
#include "RsrcSpec.h"
#include "IIdleTask.h"
#include "ILayoutUtils.h"
#include "IObserver.h"
#include "IStoryList.h"
#include "ILiveEditFacade.h"
#include "IDocFileHandler.h"
#include "IDocumentUtils.h"
#include "IUserInfoUtils.h"

// Implementation includes:

#include "CreateObject.h"
#include "CResponder.h"
#include "FileUtils.h"
#include "CoreResTypes.h"
#include "PMString.h"
#include "IStringData.h"
#include "ErrorUtils.h"
#include "TransformUtils.h"
#include "IInCopyDocUtils.h"
#include "IInCopyBridgeUtils.h"
#include "IInCopyWorkFlowUtils.h"
#include "IIntData.h"

// Project includes
#include "XLGID.h"
#include "TextID.h"
#include "PageItemScrapID.h"
#include "IXLGPreferences.h"
#include "IWebServices.h"

class XLGResponder : public CResponder
{
	public:

		XLGResponder(IPMUnknown* boss);
		virtual void Respond(ISignalMgr* signalMgr);		
		
	private:		
		void DisplayChangeStatusDialog();
		ErrorCode attachDetachStoryObserver(UIDRef storyUIDRef, bool16 bAttach);
};

CREATE_PMINTERFACE(XLGResponder, kXLGResponderImpl)

XLGResponder::XLGResponder(IPMUnknown* boss) 
			 :CResponder(boss){}


void XLGResponder::Respond(ISignalMgr* signalMgr)
{
	ServiceID serviceTrigger = signalMgr->GetServiceID();
	switch (serviceTrigger.Get()){ 
        case kBeforeOpenDocSignalResponderService:
        {
			// Deverrouilage de l'article
			InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());					
			IDFile incaFile = docData->GetFile();
			PMString idArt = incaFile.GetFileName() ;
			idArt.Truncate(5);
			
			if(idArt.Contains("-")){
				int32 extPos = idArt.LastIndexOfCharacter(PlatformChar('-'));
				if(extPos != -1)
					idArt.Remove(extPos, idArt.NumUTF16TextChars()-extPos);
			}

			// Current Story 
            InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);
			currentStory->Set(idArt);
           
			// Current User
            InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ICURRENTUSER);
			PMString currentUser = gLogin->Get();
		
            // On met ￠ jour l'utisateur dans incopy
            if(currentUser != kNullString){               
				// Verrouilage de l'article        
                InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
                InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		        xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());                
                if(!xrailWebServices->StoryLockChanged(idArt, currentUser))
		            CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey)); 

				// Set Incopy user name
				Utils<IUserInfoUtils>()->SetInCopyUsername(currentUser);  
				
				// Init the current user variable
				currentStory->Set("");
            }
             break;
        }
        case kDuringOpenDocSignalResponderService:
        {              
			InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());
            UIDRef docRef = docData->GetDocument();	

			// Attach the Observer 
			InterfacePtr<IObserver> iDocObserver(docRef, IID_IXLGDOCOBSERVER);
			if (iDocObserver != nil)
				iDocObserver->AutoAttach();
			
            InterfacePtr<IDocument> theDoc (docRef, UseDefaultIID());  
            if(!theDoc)
                break;

            // Check out all stories
            InterfacePtr<ICommand> unlockStoriesCmd (CmdUtils::CreateCommand(kXLGUnlockStoriesCmdBoss));
            unlockStoriesCmd->SetItemList(UIDList(theDoc));
            CmdUtils::ScheduleCommand(unlockStoriesCmd);            
            break;

        }

		case kAfterOpenDocSignalResponderService:
		{
			InterfacePtr<IDocumentSignalData> data(signalMgr, UseDefaultIID());
			if (data == nil)
				break;
			
			InterfacePtr<IDocument> iDocument(data->GetDocument(), UseDefaultIID());
			if (iDocument == nil)
				break;

			// attach all story observers here
			InterfacePtr<IStoryList> iStoryList(iDocument,UseDefaultIID());
			if (iStoryList == nil)
				break;

			for (int32 loop=0;loop<iStoryList->GetUserAccessibleStoryCount();loop++){
				UIDRef nthStoryUIDRef = iStoryList->GetNthUserAccessibleStoryUID(loop);
				this->attachDetachStoryObserver(nthStoryUIDRef,kTrue /*attach*/);
			}

			InterfacePtr<IBoolData> disableCloseDoc (GetExecutionContextSession(), IID_IDISABLECLOSEDOC);
			disableCloseDoc->Set(kFalse);		
			break;
		}

		case kTextScrapSuitePasteHelperService :
		{
			InterfacePtr<IBoolData> disableCloseDoc (GetExecutionContextSession(), IID_IDISABLECLOSEDOC);
			disableCloseDoc->Set(kFalse);
			break;
		}
		case kBeforeCloseDocSignalResponderService:
		{	
			InterfacePtr<IBoolData> disableCloseDoc (GetExecutionContextSession(), IID_IDISABLECLOSEDOC);
			if(disableCloseDoc->Get() == kFalse){
				
				// Deverrouilage de l'article
				InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());
				UIDRef docRef = docData->GetDocument();		
				InterfacePtr<IDocument> theDoc (docRef, UseDefaultIID());
				if(theDoc == nil)
					break;

				// Save Doc for generate Preview and XML File
				Utils<IInCopyDocUtils>()->DoSaveAll(::GetUIDRef(theDoc));
				 				
				IDataBase * db = ::GetDataBase(theDoc);
				if(db == nil)
					break;

				//Doc Name	
				InterfacePtr<IAssignedDocument> assignedDoc(theDoc, UseDefaultIID());
				InterfacePtr<IAssignment> assignment (db, assignedDoc->GetAssignments()[0], UseDefaultIID());
				if(assignment == nil)
					break;

				PMString docName = assignment->GetName();
				InterfacePtr<IStringData> currentDocName (GetExecutionContextSession(), IID_ICURRENTDOCNAME);
				currentDocName->Set(docName);
				
				PMString idArt;
				theDoc->GetName(idArt); 
				idArt.Truncate(5);
				
				if(idArt.Contains("-")){
					int32 extPos = idArt.LastIndexOfCharacter(PlatformChar('-'));
					if(extPos != -1)
						idArt.Remove(extPos, idArt.NumUTF16TextChars()-extPos);
				}

				// Mise ￠ jour de l'id de l'article
				InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);
				currentStory->Set(idArt);

				// Current User
				InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ICURRENTUSER);
				PMString currentUser = gLogin->Get();
	            
				if(currentUser != kNullString){

					// Deverrouilage de l'article        
					InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
					InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
					xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());
					if(!xrailWebServices->StoryLockChanged(idArt, "0"))
						CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));
				}	
				
				// Open Dialog for change status of story
				this->DisplayChangeStatusDialog();

				// Check in stories of assignment before closing document
				UIDList storyList = assignment->GetStories();
				Utils<Facade::ILiveEditFacade>()->CheckIn(storyList, Facade::ILiveEditFacade::CheckInOptions(Facade::ILiveEditFacade::enUnlockAsset| 
																											 Facade::ILiveEditFacade::enSaveVersion| 
																											 Facade::ILiveEditFacade::enExportStory));

				//Init story id
				currentStory->Set("");

				// Detach Observer
				InterfacePtr<IStoryList> iStoryList(theDoc, UseDefaultIID());
				if (iStoryList == nil)
					break;

				for (int32 loop=0; loop<iStoryList->GetUserAccessibleStoryCount(); loop++){
					UIDRef nthStoryUIDRef = iStoryList->GetNthUserAccessibleStoryUID(loop);
					this->attachDetachStoryObserver(nthStoryUIDRef,kFalse);
				}

				// Attach the Observer Doc
				InterfacePtr<IObserver> iDocObserver(docRef, IID_IXLGDOCOBSERVER);
				if (iDocObserver != nil)
					iDocObserver->AutoDetach();
				
			}
			else 
				disableCloseDoc->Set(kFalse);

			InterfacePtr<IIdleTask> initVariableTask (GetExecutionContextSession(), IID_IINITVARIABLE);
			initVariableTask->UninstallTask();

			break;
		}         
   	
		default:            
           break;            
	}
}

void XLGResponder::DisplayChangeStatusDialog(){

	do{	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());				
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec	(
				nLocale,		
				kXLGPluginID,	
				kViewRsrcType,
				kXLGChangeStatusDialogResourceID,	
				kTrue								
		);
		IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		if (dialog == nil)
			break;
	
		// Begin timer 
		InterfacePtr<IIdleTask> closeDialogTask(GetExecutionContextSession(), IID_IAUTOCLOSINGDIALOG);		
		if(!closeDialogTask)
			break;		
		int32 kXLGExecInterval = 60 * 1000;
			closeDialogTask->InstallTask(kXLGExecInterval);		

		dialog->Open(nil, false);
		dialog->WaitForDialog(kTrue);	

		// End timer 
		if(closeDialogTask)
			closeDialogTask->UninstallTask();
	} while(kFalse);
}
/* attachDetachStoryObserver
*/
ErrorCode  XLGResponder::attachDetachStoryObserver(UIDRef storyUIDRef, bool16 bAttach)
{
	ErrorCode status = kFailure;
	do{
		// while we observe all stories (whether they are accessible or not), we only let accessible
		// stories affect the cache. See the observer implementation for this logic.
		InterfacePtr<IObserver> storyTextObserver(storyUIDRef, IID_IXLGTEXTMODELOBSERVER);
		if (storyTextObserver == nil){
			break;
		}
		bAttach ? storyTextObserver->AutoAttach() :storyTextObserver->AutoDetach();
		status = kSuccess;
	}while (kFalse);
	return status;
}