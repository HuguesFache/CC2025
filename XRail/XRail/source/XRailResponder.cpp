//========================================================================================
//  
//  $File: //depot/indesign_3.0/gm/source/sdksamples/docwatch/DocWchResponder.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2003/09/30 15:41:37 $
//  
//  $Revision: #1 $
//  
//  $Change: 223184 $
//  
//  Copyright 1997-2003 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IClassIDData.h"
#include "ISignalMgr.h"
#include "IDocumentSignalData.h"
#include "IUIDData.h"
#include "IDialog.h"
#include "CAlert.h"
#include "ISession.h"
#include "IUIDRefListData.h"
#include "IWebServices.h"
#include "IBoolData.h"
#include "IResaPubList.h"
#include "IPageList.h"
#include "INewLayerCmdData.h"
#include "ILayerList.h"
#include "IUnitOfMeasure.h"
#include "IGeometry.h"
#include "ISpread.h"
#include "IHierarchy.h"
#include "IXRailPageSlugData.h"
#include "IDocumentLayer.h"
#include "ISpreadLayer.h"
#include "SDKLayoutHelper.h"
#include "IStoryList.h"
#include "ITextModel.h"
#include "IWaxStrand.h"
#include "IWaxIterator.h"
#include "IWaxGlyphs.h"
#include "ITextStrand.h"
#include "IAttributeStrand.h"
#include "IItemStrand.h"
#include "IStyleInfo.h"
#include "IXMLStreamUtils.h"
#include "IXMLOutStream.h"
#include "ILinkObject.h"
#include "ILinkManager.h"
#include "IURIUtils.h"
#include "URI.h"
#include "ILayerUtils.h"
#include "IIdleTask.h"
#include "IPreflightFacade.h"
#include "IPreflightManager.h"
#include "IXRailPrefsData.h"
#include "IWorkspace.h"

// Implementation includes:
#include "CreateObject.h"
#include "CResponder.h"
#include "DocUtils.h"
#include "FileUtils.h"
#include "CoreResTypes.h"
#include "PMString.h"
#include "IStringData.h"
#include "ErrorUtils.h"
#include "TransformUtils.h"
#include "IDataLinkReference.h"
#include "IDataLink.h"
#include "IDocument.h"
#include "Utils.h"
#include "ILayoutUIUtils.h"
#include "IWindow.h"
#include "IDocumentUtils.h"

// Project includes
#include "XRLID.h"
#include "XRLUIID.h"
#include "XRCID.h"
#include "TextID.h"
#include "PageItemScrapID.h"
#include "IInCopyBridgeUtils.h"
#include "IFrameList.h"
#include "UIDList.h"

/** XRailResponder
Handles signals related to document file actions.  The file action
signals it receives are dictated by the XRailServiceProvider class.

XRailResponder implements IResponder based on
the partial implementation CResponder.


@ingroup docwatch
@author John Hake
*/
class XRailResponder : public CResponder
{
public:

	/**
	Constructor.
	@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XRailResponder(IPMUnknown* boss);

	/**
	Respond() handles the file action signals when they
	are dispatched by the signal manager.  This implementation
	simply creates alerts to display each signal.

	@param signalMgr Pointer back to the signal manager to get
	additional information about the signal.
	*/
	virtual void Respond(ISignalMgr* signalMgr);

private:

	bool Checkpreflight(UIDRef doc);

	/**
	Get a list of reserves from XRail server, and place them into the document.
	Removed reserves that may be not associated with the parution anymore.
	*/
	void ImportPublicites(UIDRef doc, const K2Vector<int32>& tabIDPages, PMString baseAdress);

	/**
	Send reserves positions back to XRail server
	*/
	void MAJPublicites(UIDRef doc, PMString serverAddress);

	/**
	 Send preview to XRail server
	 */
	void DoMakePreviewToServer(UIDRef doc);

};


/* CREATE_PMINTERFACE
Binds the C++ implementation class onto its ImplementationID
making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XRailResponder, kXRLResponderImpl)

/* XRailResponder Constructor
*/
XRailResponder::XRailResponder(IPMUnknown* boss) :
	CResponder(boss)
{
}

/* Respond
*/
void XRailResponder::Respond(ISignalMgr* signalMgr)
{
	// Get the service ID from the signal manager
	ServiceID serviceTrigger = signalMgr->GetServiceID();

	// Get a UIDRef for the document.  It will be an invalid UIDRef
	// for BeforeNewDoc, BeforeOpenDoc, AfterSaveACopy, and AfterCloseDoc because
	// the document doesn't exist at that point.
	InterfacePtr<IDocumentSignalData> docData(signalMgr, UseDefaultIID());
	if (docData == nil)
	{
		ASSERT_FAIL("Invalid IDocumentSignalData* - XRailResponder::Respond");
		return;
	}
	UIDRef doc = docData->GetDocument();

	// Get userID to interact with XRail server
	InterfacePtr<IStringData> sessionID(GetExecutionContextSession(), IID_ISTRINGDATA_LOGIN);
	PMString userID = sessionID->GetString();
	bool16 isReadOnly = kFalse;
	PMString xrailServer, xrailPort, lastUser;
	bool16 modeRobot;
	K2Vector<PMString> listeBases;
	K2Vector<PMString> IPBases;
	PMString baseAdress;
	PMString serverAddress;

	GetPrefsXRail(xrailServer, xrailPort, lastUser, modeRobot, listeBases, IPBases);
	serverAddress = "http://" + xrailServer + ":" + xrailPort + "/4DSOAP";
	InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
	InterfacePtr<IXRailPrefsData>  xrailPrefsData((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));


	if (!modeRobot) {
		// Get userID to interact with XRail
		userID = sessionID->GetString();
		//si pas de login, on ne fait rien (cas des gens qui ne se sont pas loggué XRail au demarrage d'InDesign)
		if (userID == "") {
			return;
		}
	}

	InterfacePtr<IWebServices> xrailWebServices(::CreateObject2<IWebServices>(kXRCXRailClientBoss));

	// Take action based on the service ID
	switch (serviceTrigger.Get())
	{
	case kDuringOpenDocSignalResponderService: // Lock pages so that no other user can edit them at the same time			
	{
		InterfacePtr<IDocument> theDoc(doc, UseDefaultIID());
		if (theDoc->IsReadOnly()) {
			isReadOnly = kTrue;
			if (!modeRobot) {
				CAlert::InformationAlert(kXRLDocumentReadOnly);
			}
		}
		else {
			int32 nbPages = GetNbPages(doc);
			K2Vector<int32> tabIDPages;
			for (int16 i = 0; i < nbPages; i++) {
				int32 idPage = GetPageID(doc, i, baseAdress);
				if (idPage > 0) {
					tabIDPages.push_back(idPage);
				}
			}
			if (baseAdress != "") {
				serverAddress = GetGoodIP(baseAdress);
			}

			if (!modeRobot) {
				if (tabIDPages.size() > 0) {
					PMString pageStatus;
					xrailWebServices->SetServerAddress(serverAddress.GetPlatformString().c_str());
					bool connexionOK = xrailWebServices->SetVerrouPage(tabIDPages, kTrue, userID, pageStatus, kFalse);
					if (!connexionOK) {
						CAlert::ErrorAlert(PMString(kXRLErrorWSLockPageKey));
						break;
					}
				}
			}
			if (xrailPrefsData->GetImportPub() == 1) {
				if (tabIDPages.size() == nbPages) // we only handle pub if all page IDs are set correctly 
					ImportPublicites(doc, tabIDPages, serverAddress);
			}
		}
		break;
	}

	case kAfterSaveAsDocSignalResponderService:
	case kAfterSaveDocSignalResponderService:
	{
		this->DoMakePreviewToServer(doc);
		break;
	}



	case kBeforeSaveDocSignalResponderService:
	{

		if (xrailPrefsData->GetImportPub() == 1) {
			int32 nbPages = GetNbPages(doc);
			int32 nbXRailPages = 0;
			for (int16 i = 0; i < nbPages; i++) {
				if (GetPageID(doc, i, baseAdress) > 0)
					++nbXRailPages;
			}
			if (baseAdress != "") {
				serverAddress = GetGoodIP(baseAdress);
			}
			if (nbXRailPages == nbPages) // we only handle pub if all page IDs are set correctly 
				MAJPublicites(doc, serverAddress); // We sent back to the server the reserves coordinates
		}
		break;
	}

	case kBeforeCloseDocSignalResponderService:
	{
		// First, unlock pages
		int32 nbPages = GetNbPages(doc);
		bool16 showDlg = kFalse;
		PMString pageStatus;
		K2Vector<int32> listPagesIDS;
		bool16 erreurssurledoc = kFalse;
		if (!modeRobot) {
			erreurssurledoc = Checkpreflight(doc);
			for (int32 i = 0; i < nbPages; ++i) {
				int32 idPage = GetPageID(doc, i, baseAdress);
				if (idPage > 0) {
					//on remplit une liste avec tous les ids de chaque page
					listPagesIDS.push_back(idPage);

				}
			}
			if (baseAdress != "") {
				serverAddress = GetGoodIP(baseAdress);
			}
			if (listPagesIDS.size() > 0) {
				xrailWebServices->SetServerAddress(serverAddress.GetPlatformString().c_str());
				bool connexionOK = xrailWebServices->SetVerrouPage(listPagesIDS, kFalse, userID, pageStatus, erreurssurledoc);
				if (!connexionOK) {
					CAlert::ErrorAlert(PMString(kXRLErrorWSLockPageKey));
				}
				else {
					showDlg = kTrue;
				}
			}
		}

		if (showDlg && !modeRobot && !isReadOnly) {
			//dans tous les cas, en mode normal (non robot) et si il y a des IDS, on demande a l'utilisateur de changer l'etat des pages
			IDialog* dialog = CreateNewDialog(kXRLUIPluginID, kXRLUIChangeStateDialogResourceID);
			if (!dialog)
				break;

			// Get doc Name 
			//IDocument * frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
			PMString docName;
			InterfacePtr<IDocument> theDoc(docData->GetDocument(), UseDefaultIID());
			if (theDoc == nil) {
				break;
			}
			theDoc->GetName(docName);

			// Set Title Window
			InterfacePtr<IWindow> window(dialog, UseDefaultIID());
			PMString titleWindow(kXRLUIChangeStateDialogTitleKey, PMString::kTranslateDuringCall);
			titleWindow.Append(docName);
			titleWindow.SetTranslatable(kFalse);
			window->SetTitle(titleWindow);

			// Set dialog parameter : UIDRef of the document and page status
			InterfacePtr<IUIDData> dialogParameter1(dialog->GetDialogPanel(), UseDefaultIID());
			dialogParameter1->Set(doc);

			InterfacePtr<IStringData> dialogParameter2(dialog->GetDialogPanel(), UseDefaultIID());
			dialogParameter2->Set(pageStatus);

			InterfacePtr<IBoolData> dialogParameter3(dialog->GetDialogPanel(), UseDefaultIID());
			dialogParameter3->Set(erreurssurledoc);

			InterfacePtr<IIdleTask> closeDialogTask(GetExecutionContextSession(), IID_IAUTOCLOSINGDIALOG);
			if (closeDialogTask) {
				int32 execInterval = 60 * 1000;
				closeDialogTask->InstallTask(execInterval);
			}
			// Open the dialog.
			dialog->Open(nil, false);
			dialog->WaitForDialog(kTrue);
			if (closeDialogTask) {
				closeDialogTask->UninstallTask();
			}
		}
	}
	break;
	default:
		break;
	}
}

bool XRailResponder::Checkpreflight(UIDRef docUIDRef)
{
	if (Utils<Facade::IPreflightFacade>()->IsPreflightingOn(docUIDRef.GetDataBase())) {
		bool dataAvail = Utils<Facade::IPreflightFacade>()->ArePreflightResultsAvailable(docUIDRef.GetDataBase());
		if (dataAvail) {
			ScriptListData results;
			Utils<Facade::IPreflightFacade>()->GetPreflightResults(docUIDRef.GetDataBase(), results);
			if (results.size() > 0) {
				ScriptListData listData;
				ScriptData data = results[2];
				data.GetList(listData);
				if (listData.size() > 0) {
					return kTrue;
				}
			}
		}
	}

	return kFalse;
}

void XRailResponder::ImportPublicites(UIDRef doc, const K2Vector<int32>& IDPages, PMString baseAdress) {

	do {
		InterfacePtr<IDocument> frontDoc(doc, UseDefaultIID());

		// Get latest list of resa pub registered in the document
		InterfacePtr<IResaPubList> resaPubListData(doc, IID_IRESAPUBLIST);
		if (resaPubListData == nil) {
			break;
		}
		K2Vector<ResaPub> currentResaPubList = resaPubListData->GetResaPubList();
		IDataBase* db = doc.GetDataBase();
		if (db == nil) {
			break;
		}
		InterfacePtr<IWebServices> xrailWebServices(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		if (xrailWebServices == nil) {
			break;
		}
		xrailWebServices->SetServerAddress(baseAdress.GetPlatformString().c_str());

		// Get list of pubs from xrail
		K2Vector<int32> TabFolios, TabPageID;
		K2Vector<PMString> TabResaIDS, TabPubsIDS, TabAnnonceurs, TabCheminImage;
		K2Vector<PMReal> TabHauteurs, TabLargeurs, TabX, TabY;

		K2Vector<bool16> TabVerrouPos;
		bool connexionOK = xrailWebServices->GetListeOrdresRigueur(IDPages, TabPageID, TabFolios, TabResaIDS, TabPubsIDS, TabHauteurs, TabLargeurs, TabX, TabY, TabAnnonceurs, TabCheminImage, TabVerrouPos);
		if (!connexionOK) {
			//CAlert::ErrorAlert(PMString(kXRLErrorWebServiceKey));
			break;
		}
		int32 nbResaPub = TabPubsIDS.size();
		// Get the list of pages
		InterfacePtr<IPageList> pageList(doc, UseDefaultIID());
		int32 nbPages = pageList->GetPageCount();
		// Create converter (resa dimensions are sent in millimeters)
		InterfacePtr<IUnitOfMeasure> uom((IUnitOfMeasure*)::CreateObject(kMillimetersBoss, IID_IUNITOFMEASURE));
		if (uom == nil)
		{
			break;
		}
		// First step, we create resa that XRail sent
		K2Vector<ResaPub> newResaPubList;
		UIDList pubItemsToDelete(db);
		PMString baseName;
		for (int32 i = 0; i < nbResaPub; ++i) {
			ResaPub aPub;
			aPub.resaID = TabResaIDS[i];

			PMString resaAnnonceur = TabAnnonceurs[i];

			// First, check whether pub is already created in the document or not
			bool16 placed = kTrue;
			K2Vector<ResaPub>::iterator iter = ::K2find(currentResaPubList.begin(), currentResaPubList.end(), aPub);
			if (iter != currentResaPubList.end())
			{
				// Pub is already placed, check whether it's owner page corresponds to XRail data
				InterfacePtr<IHierarchy> pubHier(db, iter->resaUID, UseDefaultIID());
				if (!pubHier) // Pub has been deleted, re-place it
					placed = kFalse;
				else
				{
					InterfacePtr<IXRailPageSlugData> pageSlug(db, Utils<ILayoutUtils>()->GetOwnerPageUID(pubHier), IID_PAGESLUGDATA);
					if (!pageSlug || pageSlug->GetID() != TabPageID[i]) // Pub's page doesn't match XRail data, delete it, then recreate it at the right place
					{
						placed = kFalse;
						pubItemsToDelete.Append(iter->resaUID);
					}
					else
					{
						aPub = *iter;
					}
				}
				currentResaPubList.erase(iter);
			}
			else
				placed = kFalse;

			// Update pub ID, since it may have changed in the meantime
			aPub.pubID = TabPubsIDS[i];

			if (!placed) // Pub not placed yet, create it
			{
				// Get the page where the item for the resa must be created
				UID pageUID = kInvalidUID;
				for (int16 j = 0; j < nbPages; j++)
				{
					aPub.pageID = GetPageID(doc, j, baseName);
					if (aPub.pageID == TabPageID[i])
					{
						pageUID = pageList->GetNthPageUID(j);
						break;
					}
				}

				if (pageUID == kInvalidUID) // page doesn't exist in this document
				{
					CAlert::ErrorAlert(kXRLErrorInvalidResaPageKey);
					continue;
				}

				// Compute resa coordinates in pasteboard coordinates			
				PMPoint leftTop(uom->UnitsToPoints(TabX[i]), uom->UnitsToPoints(TabY[i]));
				InterfacePtr<IGeometry> ownerPageGeo(UIDRef(db, pageUID), UseDefaultIID());
				::TransformInnerPointToPasteboard(ownerPageGeo, &leftTop);

				// Get owner spread layer
				InterfacePtr<IHierarchy> pageHier(ownerPageGeo, UseDefaultIID());
				InterfacePtr<ISpread> ownerSpread(db, pageHier->GetSpreadUID(), UseDefaultIID());
				InterfacePtr<IDocumentLayer> activeLayer(Utils<ILayerUtils>()->QueryDocumentActiveLayer(frontDoc));
				InterfacePtr<ISpreadLayer> ownerSpreadLayer(ownerSpread->QueryLayer(activeLayer));
				//InterfacePtr<ISpreadLayer> ownerSpreadLayer (ownerSpread->QueryLayer(ownerLayer));
				if (CreerResaPub(::GetUIDRef(ownerSpreadLayer), aPub.resaID, TabAnnonceurs[i], leftTop,
					uom->UnitsToPoints(TabLargeurs[i]), uom->UnitsToPoints(TabHauteurs[i]), aPub.resaUID, TabVerrouPos[i]) != kSuccess)
				{
					CAlert::ErrorAlert(kXRLErrorCreateResaPubKey);
					continue;
				}
			}

			// Add it the new list of resa pub
			newResaPubList.push_back(aPub);
			// Check whether pub file is available for import
			PMString pubFilePath = TabCheminImage[i];

			if (pubFilePath == kNullString) // Pub file is not terminated yet, or has been cancelled
			{
				// Convert back to an empty resa, if necessary
				ConvertToResaPub(UIDRef(db, aPub.resaUID), aPub.resaID, resaAnnonceur);
			}
			else
			{
				IDFile pubFile = FileUtils::PMStringToSysFile(pubFilePath);
				if (FileUtils::DoesFileExist(pubFile) && !FileUtils::IsDirectory(pubFile)) {
					ImportPubFile(UIDRef(db, aPub.resaUID), pubFile, pubFilePath);

				}
				else
				{
					// Pub file does not exist anymore (it may occur if removed manually...)
					// So we convert back to an empty resa, if necessary
					ConvertToResaPub(UIDRef(db, aPub.resaUID), aPub.resaID, resaAnnonceur);
				}
			}
		}


		// At the end of the loop, currentResaPubList only contains pub that are not placed anymore, delete them
		for (int32 k = 0; k < currentResaPubList.size(); k++) {

			InterfacePtr<IHierarchy> pubHierToDelete(db, currentResaPubList[k].resaUID, UseDefaultIID());
			if (pubHierToDelete) {
				pubItemsToDelete.Append(currentResaPubList[k].resaUID);
			}
		}

		// Delete removed pubs
		if (pubItemsToDelete.Length() > 0)
		{
			InterfacePtr<ICommand> deleteCmd(CmdUtils::CreateCommand(kDeleteCmdBoss));
			deleteCmd->SetUndoability(ICommand::kAutoUndo);
			deleteCmd->SetItemList(pubItemsToDelete);
			CmdUtils::ProcessCommand(deleteCmd);
		}

		// Store new list of resa pub
		InterfacePtr<ICommand> setResaPubListCmd(CmdUtils::CreateCommand(kXRLSetResaPubListCmdBoss));
		setResaPubListCmd->SetUndoability(ICommand::kAutoUndo);
		setResaPubListCmd->SetItemList(UIDList(doc));
		InterfacePtr<IResaPubList> cmdData(setResaPubListCmd, IID_IRESAPUBLIST);
		cmdData->SetResaPubList(newResaPubList);
		CmdUtils::ProcessCommand(setResaPubListCmd);

	} while (false);
}

void XRailResponder::DoMakePreviewToServer(UIDRef doc)
{
	do
	{
		// Check whether previews are enabled or not0
		InterfacePtr<IBoolData>  makePreview(doc, IID_IMAKEPREVIEW);
		if (!makePreview->Get())
			break;

		int32 nbPages = GetNbPages(doc);
		int32 idPage = 0;
		PMString baseName;
		int32 index = 0;
		do {
			idPage = GetPageID(doc, index, baseName);
			++index;
		} while (index < nbPages && idPage == 0);

		if (index == nbPages && idPage == 0)
			break;

		// First, get vignette repository
		PMString clientPictPath;
		int32 scale, resolution;

		InterfacePtr<IStringData> login(GetExecutionContextSession(), IID_ISTRINGDATA_LOGIN);

		PMString serverAddress = GetGoodIP(baseName);
		InterfacePtr<IWebServices> xrailWebServices(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAddress.GetPlatformString().c_str());
		bool connexionOK = xrailWebServices->GetPreviewInfos(idPage, login->GetString(), clientPictPath, scale, resolution);
		if (!connexionOK)
		{
			CAlert::ErrorAlert(PMString(kXRLErrorWSGenerateVignetteKey));
			break;
		}

		// Create preview for each registered page
		K2Vector<int32> idPages;
		K2Vector<PMString> filePaths;
		InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
		InterfacePtr<IXRailPrefsData>  xrailPrefsData((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));

		for (int16 i = index - 1; i < nbPages; ++i)
		{
			idPage = GetPageID(doc, i, baseName);
			if (idPage > 0)
			{
				// Generate vignette
				PMString jpgName = clientPictPath;
				jpgName.AppendNumber(idPage);
				jpgName += ".jpg";
				IDFile jpgFile = FileUtils::PMStringToSysFile(jpgName);
				DoMakePreview(doc, jpgFile, i, scale, resolution);
				idPages.push_back(idPage);
				filePaths.push_back(jpgName);
				InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
				InterfacePtr<IXRailPrefsData>  xrailPrefsData((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));

				if (xrailPrefsData->GetExportIDML() == 1) {
					if (i == 0) {
						PMString idmlName = clientPictPath;
						idmlName.AppendNumber(idPage);
						idmlName += ".idml";

						IDFile idmlFile = FileUtils::PMStringToSysFile(idmlName);
						DoMakeIDML(doc, idmlFile);
					}
				}
			}
		}

		// Notify server								
		connexionOK = xrailWebServices->MoveGeneratedFiles(filePaths, idPages);
		if (!connexionOK)
		{
			CAlert::ErrorAlert(PMString(kXRLErrorWSGenerateVignetteKey));
			break;
		}

	} while (kFalse);
}

void XRailResponder::MAJPublicites(UIDRef doc, PMString baseAdress)
{
	ErrorCode status = kSuccess;

	do {
		IDataBase* db = doc.GetDataBase();
		if (db == nil) {
			break;
		}

		UIDList toDelete(db);

		InterfacePtr<IWebServices> xrailWebServices(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(baseAdress.GetPlatformString().c_str());

		// Create converter (resa dimensions are in millimeters)
		InterfacePtr<IUnitOfMeasure> uom((IUnitOfMeasure*)::CreateObject(kMillimetersBoss, IID_IUNITOFMEASURE));
		if (uom == nil) {
			break;
		}

		// Get list of resa pub registered in the document
		InterfacePtr<IResaPubList> resaPubListData(doc, IID_IRESAPUBLIST);
		if (resaPubListData == nil)
		{
			break;
		}

		K2Vector<ResaPub> currentResaPubList = resaPubListData->GetResaPubList();
		K2Vector<ResaPub>::iterator iter = currentResaPubList.begin();
		while (iter < currentResaPubList.end())
		{
			PMString resaID = iter->resaID;
			int32 pageID = 0;
			PMReal x = 0, y = 0, l = 0, h = 0;
			PMString imagePath = kNullString;

			if (!db->IsValidUID(iter->resaUID)) // Resa has been removed from the document
			{
				// Remove pub from document resa pub list
				iter = currentResaPubList.erase(iter);
			}
			else
			{
				InterfacePtr<IHierarchy> resaHier(db, iter->resaUID, UseDefaultIID());
				if (resaHier == nil) {
					continue;
				}

				// If we're on a placed image we should have a data link to source item
				InterfacePtr<ILinkManager> iLinkManager(db, db->GetRootUID(), UseDefaultIID());
				if (iLinkManager == nil) {
					break;
				}

				InterfacePtr<ILinkObject> iLinkObject(resaHier, UseDefaultIID());
				if (iLinkObject == nil) {
					break;
				}
				ILinkManager::QueryResult linkQueryResult;
				if (iLinkManager && iLinkManager->QueryLinksByObjectUID(resaHier->GetChildUID(0), linkQueryResult))
				{
					ILinkManager::QueryResult::const_iterator iterLinks = linkQueryResult.begin();
					InterfacePtr<ILink> iLink(db, *iterLinks, UseDefaultIID());
					if (iLink)
					{
						InterfacePtr<ILinkResource> iLinkResource(db, iLink->GetResource(), UseDefaultIID());
						if (iLinkResource)
						{
							IDFile imageFile;
							URI imageURI = iLinkResource->GetId();
							Utils<IURIUtils>()->URIToIDFile(imageURI, imageFile);
							if (FileUtils::DoesFileExist(imageFile) && !FileUtils::IsDirectory(imageFile))
							{
								imagePath = FileUtils::SysFileToPMString(imageFile);
							}
						}
					}
				}

				UID ownerPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID(resaHier);
				InterfacePtr<ISpread> isASpread(db, ownerPageUID, UseDefaultIID());
				if (isASpread)
				{
					// The pub is not placed on a page anymore, delete it
					toDelete.Append(iter->resaUID);

					// Remove pub from document resa pub list
					iter = currentResaPubList.erase(iter);
				}
				else
				{
					// Get page's XRail ID
					InterfacePtr<IXRailPageSlugData> pageSlug(db, ownerPageUID, IID_PAGESLUGDATA);
					if (pageSlug == nil) {
						continue;
					}

					// Update page info
					iter->pageID = pageSlug->GetID();

					// Get top left coordinate of this reserve
					InterfacePtr<IGeometry> resaGeo(resaHier, UseDefaultIID());
					if (resaGeo == nil) {
						continue;
					}

					PMPoint leftTop = resaGeo->GetStrokeBoundingBox().LeftTop();
					PMPoint rigthBottom = resaGeo->GetStrokeBoundingBox().RightBottom();
					::TransformInnerPointToPasteboard(resaGeo, &leftTop);
					::TransformInnerPointToPasteboard(resaGeo, &rigthBottom);

					l = uom->PointsToUnits(rigthBottom.X() - leftTop.X());
					h = uom->PointsToUnits(rigthBottom.Y() - leftTop.Y());
					::TransformPasteboardPointToInner(pageSlug, &leftTop);

					// We'll notify XRail of the position of the reserve
					pageID = iter->pageID;
					x = uom->PointsToUnits(leftTop.X());
					y = uom->PointsToUnits(leftTop.Y());

					++iter;
				}
			}

			PMString stringX, stringY, stringL, stringH;
			stringX.AppendNumber(x, 3, kTrue, kTrue);
			stringY.AppendNumber(y, 3, kTrue, kTrue);
			stringL.AppendNumber(l, 3, kTrue, kTrue);
			stringH.AppendNumber(h, 3, kTrue, kTrue);

			// Send information to XRail		
			bool connexionOK = xrailWebServices->MAJPub(resaID, pageID, stringX, stringY, stringH, stringL, imagePath);
			if (!connexionOK)
			{
				//CAlert::ErrorAlert(PMString(kXRLErrorWebServiceKey));
				status = kFailure;
				break;
			}
		}

		if (status == kSuccess)
		{
			// Delete unnecessary pubs here
			if (!toDelete.IsEmpty())
			{
				InterfacePtr<ICommand> deleteCmd(CmdUtils::CreateCommand(kDeleteCmdBoss));
				deleteCmd->SetUndoability(ICommand::kAutoUndo);
				deleteCmd->SetItemList(toDelete);

				// Process the DeleteCmd
				CmdUtils::ProcessCommand(deleteCmd);
			}

			// Update stored resa pub list
			InterfacePtr<ICommand> setResaPubListCmd(CmdUtils::CreateCommand(kXRLSetResaPubListCmdBoss));
			setResaPubListCmd->SetUndoability(ICommand::kAutoUndo);
			setResaPubListCmd->SetItemList(UIDList(doc));

			InterfacePtr<IResaPubList> cmdData(setResaPubListCmd, IID_IRESAPUBLIST);
			cmdData->SetResaPubList(currentResaPubList);

			CmdUtils::ProcessCommand(setResaPubListCmd);
		}
	} while (false);
}

// End, XRailResponder.cpp.