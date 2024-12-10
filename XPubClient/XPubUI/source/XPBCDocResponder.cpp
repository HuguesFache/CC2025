/*
//	File:	XPBCDocResponder.cpp
//
//	Date:	18-Jul-2003
//
//	
//	Copyright 2003 Trias Developpement. All Rights Reserved.
//	
//	NOTICE: Adobe permits you to use, modify, and distribute this file in
//	accordance with the terms of the Adobe license agreement accompanying it.
//	If you have received this file from a source other than Adobe, then your
//	use, modification, or distribution of it requires the prior written
//	permission of Adobe.
//
*/

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IClassIDData.h"
#include "ISignalMgr.h"
#include "IDocumentSignalData.h"
#include "IPanelControlData.h"
#include "ILayoutUtils.h"
#include "IDocument.h"
#include "IStringData.h"
#include "IXPubConnexion.h"
#include "IXPubClientUtils.h"
#include "IIntData.h"
#include "ICommandMgr.h"
#include "IDocumentCommands.h"

// General includes
#include "CAlert.h"
#include "StreamUtil.h"
#include "ErrorUtils.h"

// Implementation includes:
#include "CreateObject.h"
#include "CResponder.h"
#include "XPBCID.h"

/** XPBCDocResponder
	Handles signals related to document file actions.  The file
	action signals it receives are dictated by the DocWchServiceProvider
	class.

	DocWchResponder implements IResponder based on
	the partial implementation CResponder.


	@author Guillaume DESSAY
*/
class XPBCDocResponder : public CResponder
{
	public:
	
		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XPBCDocResponder(IPMUnknown *boss);

		/**
			Respond() handles the file action signals when they
			are dispatched by the signal manager.  This implementation
			simply creates alerts to display each signal.

			@param signalMgr Pointer back to the signal manager to get
			additional information about the signal.
		*/
		virtual void Respond(ISignalMgr* signalMgr);
		
	private:
		
		//void 	HorlogeActive(UIDRef docUIDRef);
		//void 	HorlogeDesactive(void);
		
		//void	LaunchEventWatcherIfNeeded(void);
		//void	KillEventWatcher(void);

};


/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPBCDocResponder, kXPBCDocResponderImpl)

/* DocWchActionComponent Constructor
*/
XPBCDocResponder::XPBCDocResponder(IPMUnknown* boss) :
	CResponder(boss)
{
}

/* Respond
*/
void XPBCDocResponder::Respond(ISignalMgr* signalMgr)
{
	
	// Get the service ID from the signal manager
	ServiceID serviceTrigger = signalMgr->GetServiceID();

	// Get a UIDRef for the document.  It will be an invalid UIDRef
	// for BeforeNewDoc, BeforeOpenDoc, AfterSaveACopy, and AfterCloseDoc because the
	// document doesn't exist at that point.
	InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());
	if (docData == nil)
	{
		ASSERT_FAIL("Invalid IDocumentSignalData* - TLDocResponder::Respond");
		return;
	}
	UIDRef docUIDRef = docData->GetDocument();

	// Take action based on the service ID
	switch (serviceTrigger.Get())
	{
	
		case kAfterSaveDocSignalResponderService:
		{
			InterfacePtr<IStringData> idPub (docUIDRef, IID_IPUB_ID);
			if(idPub && !idPub->Get().IsEmpty())
			{
				// Make preview if it's an ad
				InterfacePtr<IDocument> theAd (docUIDRef, UseDefaultIID());
				if(Utils<IXPubClientUtils>()->GenererPreview(theAd) != kSuccess)
				{
					PMString error = ErrorUtils::PMGetGlobalErrorString();
					ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				}
			}			
		}
		break;

		case kBeforeCloseDocSignalResponderService:
		{
			// If it's an ad, call service to unlock it
			InterfacePtr<IStringData> idPub (docUIDRef, IID_IPUB_ID);
			if(idPub && !idPub->Get().IsEmpty())
			{
				InterfacePtr<IStringData> currentUser (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);	

				InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
				InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
				xpubConnexion->SetServerAddress(serverAddress->Get());				

				bool16 success = xpubConnexion->ModifChampPub(idPub->Get(), 
															  PMString(kXPBCVerrouStringKey, PMString::kUnknownEncoding), 
															  PMString(kXPBCFauxStringKey, PMString::kUnknownEncoding), 
															  currentUser->Get());
				
				if(!success)
				{
					PMString error = ErrorUtils::PMGetGlobalErrorString();
					ErrorUtils::PMSetGlobalErrorCode(kSuccess);
					CAlert::ErrorAlert(error);
				}
				
				// Suppression de l'IDPub stocke		
				
				// On enlève la gestion du undo pour la modification car sinon cela provoque un crash
				// C'est specifique a ce callback (avant fermeture du doc)
				IDataBase* database = ::GetDataBase(idPub);
				database->BeginTransaction();
				
				InterfacePtr<ICommandMgr> cmdMgr(database, database->GetRootUID(), UseDefaultIID() );
				const bool16 undoSupport = database->GetUndoSupport();
				cmdMgr->SetUndoSupport(kFalse);
				Utils<IXPubClientUtils>()->SetPersistentString(docUIDRef, IID_IPUB_ID, kNullString);
				cmdMgr->SetUndoSupport(undoSupport);

				database->EndTransaction();

				// On sauvegarde le document pour que la modification soit reellement prise en compte
				InterfacePtr<IDocument> iDoc (idPub, UseDefaultIID());
				if(iDoc->IsSaved())
				{
					InterfacePtr<ICommand> saveCmd(Utils<IDocumentCommands>()->CreateSaveCommand(::GetUIDRef(iDoc), kSuppressUI));
					CmdUtils::ProcessCommand(saveCmd);
				} 
			}			
		}
		break;		

		default:
			break;
	}

}

// End, XPBCDocResponder.cpp.



