/*
//
//	File:	PrsRespService.cpp
//
//  Author: Wilfried LEFEVRE
//
//	Date:	28-Oct-2005
//
*/

#include "VCPlugInHeaders.h"

#include "GlobalDefs.h"

// General includes
#include "Utils.h"
#include "FileUtils.h"
// Project includes
#include "PrsID.h"
#include "TextID.h"

// Interface includes:
#include "ISignalMgr.h"
#include "ICommand.h"
#include "IDocumentSignalData.h"
#include "ITCLReferencesList.h"
#include "IObserver.h"
#include "IDocumentCommands.h"
#include "IDocument.h"
#include "IUIFlagData.h"
#include "ISysFileData.h"

// Implementation includes:
#include "CResponder.h"
#include "UIDList.h"

#include "CAlert.h"

class PrsRespService : public CResponder
{
	public:
	
		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		PrsRespService(IPMUnknown* boss);

		/**
			Respond() handles the file action signals when they
			are dispatched by the signal manager.  This implementation
			simply creates alerts to display each signal.

			@param signalMgr Pointer back to the signal manager to get
			additional information about the signal.
		*/
		virtual void Respond(ISignalMgr* signalMgr);

};


/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(PrsRespService, kPrsRespServiceImpl)

/* DocWchActionComponent Constructor
*/
PrsRespService::PrsRespService(IPMUnknown* boss) :
	CResponder(boss)
{
}

/* Respond
*/
void PrsRespService::Respond(ISignalMgr* signalMgr)
{
	// Get the service ID from the signal manager
	ServiceID serviceTrigger = signalMgr->GetServiceID();

	if(serviceTrigger.Get() == kDeleteStoryRespService)
	{
		InterfacePtr<ICommand> theDeleteCommand(signalMgr, UseDefaultIID()); 

		const UIDList& deletedStories = theDeleteCommand->GetItemListReference(); 

		UIDRef doc = UIDRef(deletedStories.GetDataBase(), deletedStories.GetDataBase()->GetRootUID());
		InterfacePtr<ITCLReferencesList> docRefList (doc, UseDefaultIID());
		if(!docRefList)
			return;

		PermRefVector listToUpdate = docRefList->GetStoryList();

		for(int32 i = 0 ; i < deletedStories.Length() ; ++i)
		{
			UID story = deletedStories[i];
			
			// Check if this story is referenced
			PermRefVector::iterator iter = listToUpdate.begin();
			while(iter < listToUpdate.end())
			{
				K2Vector<UID>& storiesUID = iter->Value();
				
				K2Vector<UID>::iterator iter2 = ::K2find(storiesUID.begin(), storiesUID.end(),story);
				if(iter2 != storiesUID.end())
					storiesUID.erase(iter2);

				if(storiesUID.empty()) // PermRef has disappeared
					iter = listToUpdate.erase(iter);
				else
					iter++;
			}
		}
		
		// Update document item list
		InterfacePtr<ICommand> saveItemsCmd (CmdUtils::CreateCommand(kPrsSetDocTCLReferencesListCmdBoss));
	
		saveItemsCmd->SetItemList(UIDList(doc));
		saveItemsCmd->SetUndoability(ICommand::kAutoUndo);

		InterfacePtr<ITCLReferencesList> cmdData (saveItemsCmd, UseDefaultIID());

		cmdData->SetStoryList(listToUpdate);
		cmdData->SetItemList(docRefList->GetItemList());
		cmdData->SetTableList(docRefList->GetTableList());

		CmdUtils::ProcessCommand(saveItemsCmd);
	}
	else if(serviceTrigger.Get() == kDuringOpenDocSignalResponderService)
	{
		InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());

		// Attach paste / import snippet observer
		InterfacePtr<IObserver> docObserver (docData->GetDocument(), IID_PASTEIMPORTITEMOBSERVER);
		docObserver->AutoAttach();

		// Clean up table list, tables may have been removed manually
		UIDRef doc = docData->GetDocument();
		InterfacePtr<ITCLReferencesList> docItemList (doc, UseDefaultIID());

		TableVector tableList = docItemList->GetTableList();

		IDataBase * db = doc.GetDataBase();
		TableVector::iterator iter = tableList.begin();
		while(iter != tableList.end())
		{
			if(!db->IsValidUID(iter->Value()))
			{
				iter = tableList.erase(iter);
			}
			else
				iter++;
		}

		UIDList docToSave(doc);
		InterfacePtr<ICommand> saveItemsCmd (CmdUtils::CreateCommand(kPrsSetDocTCLReferencesListCmdBoss));
		saveItemsCmd->SetUndoability(ICommand::kAutoUndo);
		saveItemsCmd->SetItemList(docToSave);

		InterfacePtr<ITCLReferencesList> cmdData (saveItemsCmd, UseDefaultIID());
		cmdData->SetItemList(docItemList->GetItemList());
		cmdData->SetStoryList(docItemList->GetStoryList());
		cmdData->SetTableList(tableList);
		
		CmdUtils::ScheduleCommand(saveItemsCmd);		
		
#if !REF_AS_STRING // Implementation of IStringData interface aggregated by kSplineItemBoss seems to be persistent ! thus, no need to set again the values
		InterfacePtr<IDocument> theDoc (docData->GetDocument(), UseDefaultIID());
		if(theDoc && theDoc->IsConverted())	
		{
			// Reset TCL data on spline items when doc is converted, because format has changed
			RefVector list = docItemList->GetItemList();
			for(RefVector::iterator iter = list.begin(); iter < list.end(); iter++)
			{				
				TCLRef ref = iter->Key();
				
				InterfacePtr<ITCLRefData> refData (doc.GetDataBase(), iter->Value(), IID_ITCLREFDATA);
				if(refData != nil)
				{
					TCLRef tmp = refData->Get();
					if(tmp != ref)
					{
						InterfacePtr<ICommand> setTCLRefCmd (CmdUtils::CreateCommand(kPrsSetTCLRefDataCmdBoss));
						setTCLRefCmd->SetUndoability(ICommand::kAutoUndo);
						setTCLRefCmd->SetItemList(UIDList(refData));
						
						InterfacePtr<ITCLRefData> cmdData (setTCLRefCmd, IID_ITCLREFDATA);
						cmdData->Set(ref);
						CmdUtils::ProcessCommand(setTCLRefCmd);
					}
				}
			}
		}
#endif		
	}
	else if(serviceTrigger.Get() == kAfterNewDocSignalResponderService)
	{
		InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());

		// Attach paste / import snippet observer
		InterfacePtr<IObserver> docObserver (docData->GetDocument(), IID_PASTEIMPORTITEMOBSERVER);
		docObserver->AutoAttach();
	}
	else if(serviceTrigger.Get() == kBeforeCloseDocSignalResponderService)
	{
		InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());

		// Detach paste / import snippet observer
		InterfacePtr<IObserver> docObserver (docData->GetDocument(), IID_PASTEIMPORTITEMOBSERVER);
		docObserver->AutoDetach();
	}
	else if(serviceTrigger.Get() == kAfterOpenDocSignalResponderService) // Force converted document to be saved if CONVERT_AUTOSAVE flag is enabled
	{
		InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());
		InterfacePtr<IDocument> theDoc (docData->GetDocument(), UseDefaultIID());

		if(theDoc && theDoc->IsConverted())
		{
#if CONVERT_AUTOSAVE
			
			IDFile srcFile = docData->GetFile();

			InterfacePtr<ICommand> saveCmd (Utils<IDocumentCommands>()->CreateSaveAsCommand());
			InterfacePtr<ISysFileData> fileData (saveCmd, UseDefaultIID());
			fileData->Set(srcFile);

			InterfacePtr<IUIFlagData> uiflags (saveCmd, IID_IUIFLAGDATA);
			uiflags->Set(kSuppressUI);

			saveCmd->SetItemList(UIDList(theDoc));

			theDoc.reset(nil);
			CmdUtils::ProcessCommand(saveCmd);
#endif
		}
	}
}