//========================================================================================
//  
//  $File: //depot/indesign_6.0/highprofile/source/sdksamples/persistentlist/XPGDocObserver.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: Trias Developpement
//  
//  $DateTime: 2010/11/04 15:33:07 $
//  
//  $Revision:  
//  
//  $Change:  
//  
//  Copyright Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//  Plug-in includes
//===========================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IDocument.h"
#include "IHierarchy.h"
#include "ISubject.h"

// API includes:
#include "CAlert.h"
#include "CmdUtils.h"
#include "CObserver.h"
#include "PageItemScrapID.h" // for kDeletePageItemCmdBoss
#include "StandOffID.h"		 // for kDeleteStandOffItemCmdBoss and kDeleteInsetPageItemCmdBoss
#include "UIDList.h"
#include "CAlert.h"
#include "K2Pair.h"
#include "IPageItemTypeUtils.h"
#include "KeyValuePair.h"
#include "IPageItemAdornmentList.h"
#include "IClassIDData.h"
#include "IFormeData.h"
#include "IResaRedacDataList.h"
#include "IWebServices.h"
#include "IPlacedArticleData.h"
#include "IXPageUtils.h"

// Project includes:
#include "XPGID.h"
#include "XPGUIID.h"



class XPGDocObserver : public CObserver{


	public:
		/**	Constructor.
			@param boss IN interface ptr from boss object on which this interface is aggregated.
		*/
		XPGDocObserver(IPMUnknown* boss);

		/**	Destructor.
		*/
		virtual ~XPGDocObserver();

		/** Called by the application to allow the observer to attach to the 
		 * 	subjects to be observed (kDocBoss)
		*/
		void AutoAttach();

		/** Called by the application to allow the observer to detach from the 
		 * 	subjects being observed.
		*/
		void AutoDetach();

 		virtual void Update(const ClassID& theChange, ISubject* theSubject, const PMIID& protocol, void* changedBy);

	protected:
		/**	Attaches this observer to a document.
		 * 	@param iDocument IN The document to which we want to attach.
		*/
		void AttachDocument(IDocument* iDocument);

		/**	Detaches this observer from a document.
		 * 	@param iDocument IN The document from which we want to detach.
		*/
		void DetachDocument(IDocument* iDocument);

		/**	Maintain document consistency :
			1) notifying XRail when all items of a placed story have been deleted
			2) delete resa redacs when all items of a forme have been deleted
		*/
		void HandlePageItemBeingDeleted(const UIDList& items);

		/** Helper methods
		*/
		void HandleItemBeingDeleted(IHierarchy * itemHier, 
									K2Vector<KeyValuePair <PMString, UIDList> >& deletedStories,
									K2Vector<KeyValuePair <PMString, UIDList> >& deletedFormesWithResa);
		void DeleteAdornments(const UIDList& itemList);

	private:

		InterfacePtr<IXPGPreferences> xpgPrefs;	

};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGDocObserver, kXPGDocObserverImpl)

/* XPGDocObserver Constructor
*/
XPGDocObserver::XPGDocObserver(IPMUnknown* boss):CObserver(boss, IID_IDOCOBSERVER){
	xpgPrefs = InterfacePtr<IXPGPreferences>(GetExecutionContextSession(), UseDefaultIID());	
}

/* XPGDocObserver Destructor
*/
XPGDocObserver::~XPGDocObserver(){
	if(xpgPrefs)
		xpgPrefs->Release();

}


/*	XPGDocObserver::AutoAttach
*/
void XPGDocObserver::AutoAttach(){
	CObserver::AutoAttach();
	InterfacePtr<IDocument> iDocument(this, UseDefaultIID());
	if (iDocument)
		this->AttachDocument(iDocument);
}


/*	XPGDocObserver::AutoDetach
*/
void XPGDocObserver::AutoDetach(){
	CObserver::AutoDetach();
	InterfacePtr<IDocument> iDocument(this, UseDefaultIID());
	if (iDocument)
		this->DetachDocument(iDocument);
}

/*	XPGDocObserver::Update
*/
void XPGDocObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID& protocol, void* changedBy){

	do{
		ICommand* iCommand = (ICommand*)changedBy;
		const UIDList itemList = iCommand->GetItemListReference();
		if (protocol == IID_IHIERARCHY_DOCUMENT){
			if (theChange == kDeletePageItemCmdBoss){
				if (itemList.Length() && (iCommand->GetCommandState() == ICommand::kNotDone)){
					
					// GD 29.01.2013 ++
					PMString cmdName;
					iCommand->GetName(&cmdName);
					// Si c'est juste un 'ungroup' on ne fait rien au niveau de notre persistance d'article, ce n'est pas une vraie suppression de blocs d'article
					if(cmdName != "Ungroup" && cmdName != "") {
					// GD GD 29.01.2013 --
					HandlePageItemBeingDeleted(iCommand->GetItemListReference());
					
					} // GD GD 29.01.2013
					
				}
			}
		}
	} while (kFalse);
}

/*	XPGDocObserver::HandlePageItemDeleted
*/
void XPGDocObserver::HandlePageItemBeingDeleted(const UIDList& items){

	do{
		int32 nbItems = items.Length();
		K2Vector<KeyValuePair <PMString, UIDList> > deletedStories;	 
		K2Vector<KeyValuePair <PMString, UIDList> > deletedFormesWithResa;

		// Parcourir les articles a supprimer
		for(int32 i=0; i < nbItems; ++i){	
			InterfacePtr<IHierarchy> itemHier (items.GetRef(i), UseDefaultIID());
			if(!itemHier)
				continue;

			this->HandleItemBeingDeleted(itemHier, deletedStories, deletedFormesWithResa);
		}

		// On supprime definitivement un article lorsque le nb des items selectionnes est equivalent au nb d'items existants en page
		if(deletedStories.size() > 0)
		{
			InterfacePtr<IDocument> theDoc (this, UseDefaultIID());
			K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > placedStoriesList = Utils<IXPageUtils>()->GetPlacedStoriesList(theDoc);
						
			for(int32 i = 0; i < deletedStories.size(); i++){	

				PMString idArt = deletedStories[i].Key();
				if(idArt == kNullString)
					continue;

				int32 index = ::FindLocation(placedStoriesList, idArt); 
				if(index == -1)
					continue;

				UIDList storyItems = deletedStories[i].Value();
				int32 itemsCount = storyItems.size();

				// Si ok , supprime definitivement l'article
				if(itemsCount == placedStoriesList[index].Value().second.size()){
				 
					// Delete data of placed story				
					InterfacePtr<ICommand> deletePlacedArticleDataCmd(CmdUtils::CreateCommand(kXPGSetPlacedArticleDataCmdBoss));
					InterfacePtr<IPlacedArticleData> placedArticleData(deletePlacedArticleDataCmd, IID_IPLACEDARTICLEDATA);
					placedArticleData->SetUniqueId(kNullString);
					placedArticleData->SetStoryFolder(kNullString);
					deletePlacedArticleDataCmd->SetItemList(storyItems);
					if(CmdUtils::ProcessCommand(deletePlacedArticleDataCmd)!= kSuccess)
						continue;					
				
					// Notify xrail
					InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
					xrailConnexion->SetServerAddress(this->xpgPrefs->GetXRail_URL());					
					if(!xrailConnexion->DeleteStory(idArt)){
						PMString error = ErrorUtils::PMGetGlobalErrorString();
						ErrorUtils::PMSetGlobalErrorCode(kSuccess);
						CAlert::InformationAlert(error);
						continue;                        
					}   

					// Delete adromnment from page item with kXPGUIArticleAdornmentBoss
					this->DeleteAdornments(storyItems);	

					// Send notification so that texte panel is updated
					InterfacePtr<ISubject> sessionSubject (GetExecutionContextSession(), UseDefaultIID());
					sessionSubject->Change(kXPGRefreshMsg, IID_IREFRESHPROTOCOL);	
				}
			}			
		}
			// On supprime définitivement une résa rédac quand tous les items de la forme associée ont été supprimés
		if(deletedFormesWithResa.size() > 0)
		{
			IDataBase* db = ::GetDataBase(this);

			for(int32 i = 0; i < deletedFormesWithResa.size(); i++)
			{	
				UIDRef firstDeletedFormeItem = deletedFormesWithResa[i].Value().GetRef(0);
				InterfacePtr<IHierarchy> firstDeleteFormeItemHier (firstDeletedFormeItem, UseDefaultIID());
				InterfacePtr<ISpread> owningSpread (db, firstDeleteFormeItemHier->GetSpreadUID(), UseDefaultIID());

				UIDList allFormeItems(db);
				Utils<IXPageUtils>()->GetAllFormeItemsOnSpread(deletedFormesWithResa[i].Key(), owningSpread, allFormeItems);

				if(allFormeItems.Length() == deletedFormesWithResa[i].Value().Length())
				{
					InterfacePtr<IResaRedacDataList> docResaDataList (this, UseDefaultIID());
					
					// Supprimer la resa
					InterfacePtr<ICommand> setResaRedacListCmdBoss (CmdUtils::CreateCommand(kXPGSetResaRedacDataListCmdBoss));
					setResaRedacListCmdBoss->SetItemList(UIDList(docResaDataList));

					InterfacePtr<IResaRedacDataList> cmdData (setResaRedacListCmdBoss, UseDefaultIID());
					cmdData->CopyFrom(docResaDataList);
					cmdData->RemoveResaRedac(deletedFormesWithResa[i].Key());

					if(CmdUtils::ProcessCommand(setResaRedacListCmdBoss) != kSuccess)
						continue;
				}

			}
		}

	} while (false);
}

/*	XPGDocObserver::AttachDocument
*/
void XPGDocObserver::AttachDocument(IDocument* iDocument){

	do{
		if(iDocument == nil)		
			break;

		InterfacePtr<ISubject> iDocSubject(iDocument, UseDefaultIID());
		if(iDocSubject == nil)		
			break;		

		//	Protocols used for page item model changes
		if (!iDocSubject->IsAttached(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IDOCOBSERVER))	 
			iDocSubject->AttachObserver(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IDOCOBSERVER);
		 
	} while (kFalse);
}


/*	XPGDocObserver::DetachDocument
*/
void XPGDocObserver::DetachDocument(IDocument* iDocument){
	do{
		if (iDocument == nil)		
			break;
	
		InterfacePtr<ISubject> iDocSubject(iDocument, UseDefaultIID());	
		if(iDocSubject == nil)
			break;	

		if (iDocSubject->IsAttached(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IDOCOBSERVER))	
			iDocSubject->DetachObserver(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IDOCOBSERVER);
	
	} while (kFalse);
}

void XPGDocObserver::HandleItemBeingDeleted(IHierarchy * itemHier, 
											K2Vector<KeyValuePair <PMString, UIDList> >& deletedStories,
											K2Vector<KeyValuePair <PMString, UIDList> >& deletedFormesWithResa){
		
	if(Utils<IPageItemTypeUtils>()->IsGroup(itemHier)){
		// If it's a group, handle children item recursively
		for(int32 i = 0 ; i < itemHier->GetChildCount() ; ++i){
			InterfacePtr<IHierarchy> childHier (itemHier->QueryChild(i));
			HandleItemBeingDeleted(childHier, deletedStories, deletedFormesWithResa);
		}
	}
	else{	
		// Check out item if is an InCopy story
		UIDRef itemRef = ::GetUIDRef(itemHier);
		UIDRef storyRef = itemRef;
		InterfacePtr<IPlacedArticleData> placedArticleData(itemRef, UseDefaultIID());
		
		if(placedArticleData){
			PMString idArt = placedArticleData->GetUniqueId();
			PMString pathArt = placedArticleData->GetStoryFolder();
			if(!idArt.IsEmpty() && !pathArt.IsEmpty()){				
				// Add item to the group of items it belongs to
				K2Vector<KeyValuePair <PMString, UIDList> >::iterator iter = ::K2find(deletedStories, idArt);
				if(iter == deletedStories.end()) {
					deletedStories.push_back(KeyValuePair <PMString, UIDList> (idArt, UIDList(itemRef)));
				}
				else if(iter != deletedStories.end()) 
					iter->Value().Append(itemRef);					 					 
			}
		}

		// Check out if this a forme item which is associated with a resa redac
		InterfacePtr<IFormeData> formeData (itemRef, UseDefaultIID());
		if(formeData)
		{
			PMString idForme = formeData->GetUniqueName();
			if(idForme != kNullString)
			{
				InterfacePtr<IResaRedacDataList> resaList (this, UseDefaultIID());
				ResaRedac resa = resaList->GetResaRedac(idForme);
				if(resa != kInvalidResaRedac)
				{
					K2Vector<KeyValuePair <PMString, UIDList> >::iterator iter = ::K2find(deletedFormesWithResa, idForme);
					if(iter == deletedFormesWithResa.end()) {
						deletedFormesWithResa.push_back(KeyValuePair <PMString, UIDList> (idForme, UIDList(itemRef)));
					}
					else if(iter != deletedFormesWithResa.end()) 
						iter->Value().Append(itemRef);	
				}
			}
		}
	}	
}

void XPGDocObserver::DeleteAdornments(const UIDList& itemList){

	do{
		UIDList adorneditemList(itemList.GetDataBase());
		for (int32 i = 0; i < itemList.Length(); i++){		
			InterfacePtr<IPageItemAdornmentList> pageItemAdornmentList(itemList.GetRef(i), IID_IPAGEITEMADORNMENTLIST);
			if (pageItemAdornmentList && pageItemAdornmentList->HasAdornment(kXPGUIArticleAdornmentBoss))
				adorneditemList.Append(itemList[i]);		 
		}

		if (adorneditemList.Length() > 0){
			// Remove the adornment.
			InterfacePtr<ICommand>  removePageItemAdornmentCmd(CmdUtils::CreateCommand(kRemovePageItemAdornmentCmdBoss));         
			if (!removePageItemAdornmentCmd)        
				break;
        
			InterfacePtr<IClassIDData> classIDData(removePageItemAdornmentCmd, UseDefaultIID());        
			if (!classIDData)       
				break;
         
			classIDData->Set(kXPGUIArticleAdornmentBoss);
			removePageItemAdornmentCmd->SetItemList(adorneditemList);
			CmdUtils::ProcessCommand(removePageItemAdornmentCmd);
		}
	}while(kFalse);
}


// End, XPGDocObserver.cpp.

