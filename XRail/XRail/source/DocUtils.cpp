
#include "VCPlugInHeaders.h"

#include "FileUtils.h"
#include "DocUtils.h"
#include "CAlert.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "Utils.h"
#include "UIDList.h"
#include "SnapshotUtils.h"
#include "StreamUtil.h"
#include "XRLID.h"
#include "XRLUIID.h"
#include "IXRailPrefsData.h"
#include "IXRailPageSlugData.h"
#include "IPageList.h"
#include "IDocument.h"
#include "IWorkspace.h"
#include "ISectionCmdData.h"
#include "IDocumentUtils.h"
#include "ErrorUtils.h"
#include "IDocFileHandler.h"
#include "ISectionList.h"
#include "RsrcSpec.h"
#include "IApplication.h"
#include "IDialogMgr.h"
#include "IDialog.h"
#include "IIntData.h"
#include "ITextModelCmds.h"
#include "IMultiColumnTextFrame.h"
#include "IHierarchy.h"
#include "TransformUtils.h"
#include "INewPageItemCmdData.h"
#include "IGraphicAttributeUtils.h"
#include "SplineID.h"
#include "ISwatchUtils.h"
#include "IPageItemTypeUtils.h"
#include "IDataLinkReference.h"
#include "IDataLink.h"
#include "IDataLinkHelper.h"
//#include "IImportFileCmdData.h"
#include "IPlacePIData.h"
#include "ITextAttrUID.h"
#include "PageItemScrapID.h"
#include "openplaceid.h"
#include "ITextAttrRealNumber.h"
#include "IImportResourceCmdData.h"
#include "URI.h"
#include "IURIUtils.h"
#include "ILockPosition.h"
#include "IS4SPPackage.h"
#include "PackageAndPreflightID.h"
#include "IFrameContentUtils.h"

#include "ILinkManager.h"
#include "ILink.h"
#include "ILinkObject.h"
#include "ILinkResource.h"

#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IExportProvider.h"
#include "IPlaceGun.h"
#include "IUIDData.h"

ErrorCode DoMakePreview(const UIDRef& documentUIDRef, IDFile& jpgFile, int32 page, int32 scale, int32 resolution)
{
	ErrorCode status = kFailure;
	
	do {
		InterfacePtr<IDocument> doc(documentUIDRef, UseDefaultIID());
		if (doc == nil)
		{
			ASSERT_FAIL("Could not get IDocument");
			break;
		}

		// Get the page UID
		InterfacePtr<IPageList> pageList(doc, UseDefaultIID());
		
		UID pageUID =  pageList->GetNthPageUID(page);
	
		if (pageUID == kInvalidUID) 
		{
			ASSERT_FAIL("Wrong page index ?");
			break;
		}

		SnapshotUtils* fSnapshotUtils;
		// Create a new snapshot utils instance
		fSnapshotUtils = new SnapshotUtils(UIDRef(documentUIDRef.GetDataBase(), pageUID), 
										   scale/100,     // X Scale
										   scale/100,     // Y Scale
											resolution,    // desired resolution of resulting snapshot
											resolution,    // minimum resolution of resulting snapshot
										   0,     // Extend the bounds of the area to be drawn by the given amount of bleed
										   IShape::kPrinting,// Drawing flags (kPrinting suppresses drawing of margins and guides)
										   kTrue,    // kTrue forces images and graphics to draw at full resolution, kFalse to draw proxies
										   kFalse,    // kTrue to draw grayscale, kFalse to draw RGB
										   kFalse);    // kTrue to add an alpha channel, kFalse no alpha channel


		uint32 mode = kOpenOut|kOpenTrunc;
		OSType fileType = 'JPEG';
		OSType creator = '8BIM';

		// Create a stream to write out
		InterfacePtr<IPMStream> jpegStream(StreamUtil::CreateFileStreamWrite(jpgFile, mode, fileType, creator));
		if (jpegStream == nil) 
		{
			ASSERT_FAIL("Could not Create a stream to write, so we can't export");
			break;
		}

		// export to JPEG
		status = fSnapshotUtils->ExportImageToJPEG(jpegStream);		
        if (fSnapshotUtils)
            delete fSnapshotUtils;
	
	} while (false);


	return status;
}

ErrorCode DoMakeIDML(const UIDRef& documentUIDRef, IDFile& idmlFile)
{
	ICommandSequence * seq = nil;
	ErrorCode status = kFailure;
	
	PMString formatName("InDesignMarkup", PMString::kUnknownEncoding);
	do
	{
		InterfacePtr<IDocument> doc(documentUIDRef, UseDefaultIID());
		if (doc == nil)
		{
			ASSERT_FAIL("Could not get IDocument");
			break;
		}

		InterfacePtr<IK2ServiceRegistry> k2ServiceRegistry(GetExecutionContextSession(), UseDefaultIID());
		if (k2ServiceRegistry == nil)
		{
			break;
		}
		
		// get number of kExportProviderService providers
		int32 exportProviderCount = k2ServiceRegistry->GetServiceProviderCount(kExportProviderService);
		
		for (int32 exportProviderIndex = 0 ; exportProviderIndex < exportProviderCount ; exportProviderIndex++)
		{
			// get the service provider boss class
			InterfacePtr<IK2ServiceProvider> k2ServiceProvider (k2ServiceRegistry->QueryNthServiceProvider(kExportProviderService, exportProviderIndex));
			if (k2ServiceProvider  == nil)
			{
				break;
			}
			
			InterfacePtr<IExportProvider> exportProvider(k2ServiceProvider, IID_IEXPORTPROVIDER);
			if (exportProvider == nil)
			{
				break;
			}
			
			// check to see if the current selection specifier can be exported by this provider
			if(exportProvider->CanExportToFile())
			{
				// check by format name too, as sometimes, CanExportThisFormat ignores the formatName
				int32 formatCount = exportProvider->CountFormats();
				for (int32 formatIndex = 0 ; formatIndex < formatCount ; formatIndex++)
				{
					PMString localFormatName = exportProvider->GetNthFormatName(formatIndex);
					if (localFormatName == formatName)
					{
						// found the right IDML export provider -> start the export process
						
						exportProviderIndex = exportProviderCount; // to go out of the loop after processing the command
						
						PMString fileName, fileExt, fileToExport;
						
						seq = CmdUtils::BeginCommandSequence();
						if(seq == nil)
						{		
							ASSERT_FAIL("Func_EX::IDCall_EX -> seq nil");
							break;
						}
						
						
						
				        // Do the export!
						exportProvider->ExportToFile(idmlFile, doc, nil, formatName, K2::kSuppressUI);
						
						// check error
						status = ErrorUtils::PMGetGlobalErrorCode();
						
						break;		
					}
				}
			}
		}
		
	} while(kFalse);
	
	if (seq != nil)
	{
		if (status != kSuccess)
			ErrorUtils::PMSetGlobalErrorCode(kFailure);
		
		CmdUtils::EndCommandSequence(seq);
	}
	else
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	
	return status;
}



ErrorCode DoSaveDoc(UIDRef documentUIDRef, IDFile sysFile)
{
	ErrorCode result = kFailure;
	do {
		// Save the document to another file.

		// Starting from InDesign 3.0, IDocumentUtils has been added to 
		// the utils boss where it is more logically placed. 
		// The version of IDocumentUtils on the session boss is being deprecated. 
		InterfacePtr<IDocFileHandler> docFileHandler(Utils<IDocumentUtils>()->QueryDocFileHandler(documentUIDRef));
		if (!docFileHandler) {
			break;
		}
		//Try to do SaveAs
		if(docFileHandler->CanSaveAs(documentUIDRef) ) {
			docFileHandler->SaveAs (documentUIDRef, &sysFile, kSuppressUI);
			result = ErrorUtils::PMGetGlobalErrorCode();
			ASSERT_MSG(result == kSuccess, "IDocFileHandler::SaveAs failed");
			if (result != kSuccess) {
				break;
			}
		}
	} while(false);
	return result;
}

void SetXRailPrefs(PMString adresseIP, PMString port, PMString lastUser, bool16 modeRobot, K2Vector<PMString> listeBases, K2Vector<PMString> IPBases)
{
	do
	{
		InterfacePtr<ICommand> setPrefsCmd(CmdUtils::CreateCommand(kXRLSetPrefsCmdBoss));

		InterfacePtr<IXRailPrefsData> prefsData(setPrefsCmd, UseDefaultIID());
		prefsData->SetServeurIP(adresseIP);
		prefsData->SetPort(port);
		prefsData->SetLastUser(lastUser);
		prefsData->SetModeRobot(modeRobot);
#if MULTIBASES == 1
		prefsData->SetListeBases(listeBases);
		prefsData->SetIPBases(IPBases);
#endif
		if (CmdUtils::ProcessCommand(setPrefsCmd) != kSuccess)
		{
			ASSERT_FAIL("command setPrefsCmd failed");
			break;
		}
	} while (false);	
}

void SetPageID(UIDRef docUIDRef, int32 indexofpage, int32 id, PMString baseName)
{
	do {
		
		// On cree la commande
		InterfacePtr<ICommand> writeSlugCmd (CmdUtils::CreateCommand(kXRLSetPageSlugDataCmdBoss));
		writeSlugCmd->SetUndoability(ICommand::kAutoUndo);
		
		// On specifie les parametres
		InterfacePtr<IXRailPageSlugData> slugdata (writeSlugCmd, IID_PAGESLUGDATA);
		slugdata->SetID(id);
#if MULTIBASES == 1
		slugdata->SetBaseName(baseName);
#endif
		InterfacePtr<IIntData> pageIndexData (writeSlugCmd, UseDefaultIID());
		pageIndexData->Set(indexofpage);

		// On precise la cible de la commande
		UIDList List (docUIDRef);
		writeSlugCmd->SetItemList(List);
		
		// On lance la commandePriority
		ErrorCode err = CmdUtils::ProcessCommand(writeSlugCmd);
		
		// Gestion des erreurs
		if (err != kSuccess) {
			ASSERT_FAIL("TLPanelWidgetObserver::WriteStatus -> unable to save data in doc");
		}
		
	} while(false);
}

int32 GetPageID(UIDRef docUIDRef, int32 index, PMString &baseName)
{
	int32 returnID = 0;
	do 
	{
		InterfacePtr<IPageList> pageList(docUIDRef, UseDefaultIID());
		
		UID pageUID = pageList->GetNthPageUID(index);
		if(pageUID == kInvalidUID) // Wrong page index
			break;

		InterfacePtr<IXRailPageSlugData> readdata (docUIDRef.GetDataBase(), pageUID, IID_PAGESLUGDATA);
		
		returnID = readdata->GetID();
#if MULTIBASES == 1
		baseName = readdata->GetBaseName();
#endif
	}
	while (false);
		
	return returnID;
}

void GetPrefsXRail(PMString &adresseIP, PMString &port, PMString &lastUser, bool16& modeRobot, K2Vector<PMString> &listeBases, K2Vector<PMString> &IPBases)
{
	InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());

	InterfacePtr<IXRailPrefsData>  xrailPrefsData ((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));		

	adresseIP = xrailPrefsData->GetServeurIP();
	port = xrailPrefsData->GetPort();
	lastUser = xrailPrefsData->GetLastUser();
	modeRobot = xrailPrefsData->GetModeRobot();
#if MULTIBASES == 1
	listeBases = xrailPrefsData->GetListeBases();
	IPBases = xrailPrefsData->GetIPBases();
#endif
}

PMString GetGoodIP(PMString baseName) 
{
	int32 i = 0;
	PMString ip, port, temp;
	bool16 modeRobot = kTrue;
	bool16 isReadOnly = kFalse;
	K2Vector<PMString> listeBases;
	K2Vector<PMString> IPBases;
	GetPrefsXRail(ip, port, temp, modeRobot, listeBases, IPBases);
	
	PMString serverAddress = "http://" + ip + ":" + port + "/4DSOAP";
#if MULTIBASES == 1
	//si le nom de la base est renseigne
	if (baseName != "") {
		for (i = 0; i < listeBases.size(); i++) {
			if (listeBases[i] == baseName) {
				serverAddress = "http://" + IPBases[i] + "/4DSOAP";
				break;
			}
		}
	}
#endif
	return serverAddress;
}

ErrorCode SaveAsDoc(UIDRef doc, PMString absoluteName)
{
	ErrorCode status = kFailure;
	PMString error;

	do
	{

		// Get an IDocFileHandler interface for the document:
		InterfacePtr<IDocFileHandler> docFileHandler(Utils<IDocumentUtils>()->QueryDocFileHandler(doc));
		if(docFileHandler == nil)
			break;

		// Use IDocFileHandler's SaveAs() command to implement SaveAsDocCmd:
		if(!docFileHandler->CanSaveAs(doc))
		{
			break;
		}		
		
		IDFile file = FileUtils::PMStringToSysFile(absoluteName);
		IDFile parent;
		
		FileUtils::GetParentDirectory(file, parent) ;
			
		if(FileUtils::DoesFileExist (parent) == kFalse)// Invalid path
		{
       		break;
        }

		int16 rep = 1;
		if(FileUtils::DoesFileExist (file) == kTrue)// le fichier existe deja, on demande si on sauvegarde
			rep = CAlert::ModalAlert(kXRLUIMessageLinkerKey, kOKString, kCancelString, kNullString, 2, CAlert::eWarningIcon) ;
		
		if (rep == 1)  {
			// if the user clicked on "OK" button
			DoSaveDoc(doc, file);
			status = ErrorUtils::PMGetGlobalErrorCode();
			if(status != kSuccess)
			{
				ErrorUtils::PMSetGlobalErrorCode(kTrue);
				break;
			}				
		}
		else 
			status = kSuccess; // Not an error if user cancelled

	} while(false);

	if(status != kSuccess)
		ErrorUtils::PMSetGlobalErrorCode(kFalse);
		
	return status;
}

int32 GetNbPages(const UIDRef& documentUIDRef)
{
	int32 nbPages = 0;

	if (documentUIDRef != UIDRef::gNull) 
	{
		InterfacePtr<IPageList> pageList(documentUIDRef, UseDefaultIID());
		nbPages = pageList->GetPageCount();
	}
	return nbPages;
}

ErrorCode SetFolio(const UIDRef& documentUIDRef, int32 indexofpage, int32 folio)
{
	ErrorCode result = kFailure;
	int32 nbPages = 0;
	PMString sectionName;
	
	UID myPage = kInvalidUID, mySectionUID = kInvalidUID;
	do
	{
		InterfacePtr<IPageList> pageList(documentUIDRef, UseDefaultIID());
		if(pageList == nil) break;

		myPage = pageList->GetNthPageUID(indexofpage - 1);
				
		InterfacePtr<ISectionList> sectionList(documentUIDRef, UseDefaultIID());

		// Create a NewSectionCmd:
		InterfacePtr<ICommand> newSectionCmd(CmdUtils::CreateCommand(kNewSectionCmdBoss));
	
		// Get an ISectionCmdData Interface for the NewSectionCmd:
		InterfacePtr<ISectionCmdData> newSectionData(newSectionCmd,IID_ISECTIONCMDDATA);
		
		PMString prefixe = "";
		PMString marker = "";
		
		// Set the ISectionCmdData Interface's data:
		newSectionData->Set(::GetUIDRef(sectionList), myPage, folio);

		// Process the NewSectionCmd:
		if (CmdUtils::ProcessCommand(newSectionCmd) != kSuccess)
			break;
		
		result = kSuccess;

	} while (false);

	return result;
}


IDialog * CreateNewDialog(PluginID pluginID, RsrcID dialogID, IDialog::DialogType dialogType)
{
	IDialog* dialog = nil;

	do{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
		if(!application)
			break;

		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		if(!dialogMgr)
			break;
		
		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			pluginID,			// Our Plug-in ID from BasicDialogID.h. 
			kViewRsrcType,				// This is the kViewRsrcType.
			dialogID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
		);

		// CreateNewDialog takes the dialogSpec Created above, and also
		// the type of dialog being Created (kMovableModal).
		dialog = dialogMgr->CreateNewDialog(dialogSpec, dialogType);

	}while(kFalse);
	
	return dialog;
}

ErrorCode CreerResaPub(UIDRef parent, const PMString& resaID, const PMString& resaAnnonceur,const PMPoint& leftTop, const PMReal& width, const PMReal& height, UID& resaUID, bool16 VerrouPos)
{
	ErrorCode status = kFailure;
	do
	{
		IDataBase * db = parent.GetDataBase();
		
		PMPointList resaBounds;
		resaBounds.push_back(leftTop);
		resaBounds.push_back(PMPoint(leftTop.X() + width, leftTop.Y() + height));

		// Create a text frame
		InterfacePtr<ICommand> CreateTextFrameCmd(CmdUtils::CreateCommand(kCreateMultiColumnItemCmdBoss));								
		CreateTextFrameCmd->SetUndoability(ICommand::kAutoUndo);	
		InterfacePtr<INewPageItemCmdData> data(CreateTextFrameCmd, UseDefaultIID());				
		data->Set(db,kSplineItemBoss, parent.GetUID(), resaBounds);
		
		if(CmdUtils::ProcessCommand(CreateTextFrameCmd) != kSuccess)
			break;
		UIDRef resaRef = CreateTextFrameCmd->GetItemList()->GetRef(0); 
		resaUID = resaRef.GetUID(); // Register resa UID
		
		// GD 7.5.6 ++
		// Ajout de la prise en compte du parametre "verrou position pub"
		if(resaRef != kInvalidUIDRef) {
			if(VerrouPos == kTrue) {
				LockPageItemCmd(resaRef, kTrue, kTrue);
			}
		}
		// GD 7.5.6 --
		InterfacePtr<IHierarchy> resaHier (resaRef, UseDefaultIID());
		InterfacePtr<IMultiColumnTextFrame> txtFrame (db, resaHier->GetChildUID(0), UseDefaultIID());
		InterfacePtr<ITextModelCmds>modelCmds(db, txtFrame->GetTextModelUID(), UseDefaultIID());

		// Set text color to white
		boost::shared_ptr<AttributeBossList> attrList (new AttributeBossList());
		InterfacePtr<ITextAttrUID> textAttrUID_Color(::CreateObject2<ITextAttrUID>(kTextAttrColorBoss));						
		textAttrUID_Color->SetUIDData(Utils<ISwatchUtils>()->GetPaperSwatchUID(db));
		
		// Create a point size, leading and underline attributes.
	    InterfacePtr<ITextAttrRealNumber> textAttrPointSize(::CreateObject2<ITextAttrRealNumber>(kTextAttrPointSizeBoss));
       	textAttrPointSize->Set(PMReal(20.0));

		// Create a point size, leading and underline attributes.
	    InterfacePtr<ITextAttrRealNumber> textAttrLeading(::CreateObject2<ITextAttrRealNumber>(kTextAttrLeadBoss));
       	textAttrLeading->Set(PMReal(20.0));
		
		attrList->ApplyAttribute(textAttrUID_Color);
		attrList->ApplyAttribute(textAttrPointSize);
		attrList->ApplyAttribute(textAttrLeading);
		InterfacePtr<ICommand> applyAttrCmd (modelCmds->ApplyCmd(0, 0, attrList, kCharAttrStrandBoss));
		if(CmdUtils::ProcessCommand(applyAttrCmd) != kSuccess)
			break;
		// Insert ID in resa and Annonceur		
		boost::shared_ptr<WideString> toInsert( new WideString(resaID));
		toInsert->Append(kTextChar_CR);
		toInsert->Append(WideString(resaAnnonceur));
		
		InterfacePtr<ICommand> insertTextCmd(modelCmds->InsertCmd(0, toInsert));
		if(CmdUtils::ProcessCommand(insertTextCmd) != kSuccess)
			break;		
		// Set fill color
		UIDList frameList(resaRef);
		InterfacePtr<ICommand> fillCmd (Utils<IGraphicAttributeUtils>()->CreateFillRenderingCommand(Utils<ISwatchUtils>()->GetBlackSwatchUID(db), &frameList, kTrue, kTrue));
		if(CmdUtils::ProcessCommand(fillCmd) != kSuccess)
			break;
	
		// Set the tint (other method than this used to set the color : IGraphicAttributeUtils)
		int32 trameFond = 30;
		InterfacePtr<ICommand> fillTintCmd (Utils<IGraphicAttributeUtils>()->CreateFillTintCommand(trameFond,&frameList,kTrue,kTrue));
		if(CmdUtils::ProcessCommand(fillTintCmd) != kSuccess)
			break;
		status = kSuccess;

	}while(kFalse);

	return status;
}

ErrorCode ConvertToResaPub(UIDRef resaFrame, const PMString& resaID, const PMString& resaAnnonceur)
{
	ErrorCode status = kFailure;
	do
	{
		UIDList frameList(resaFrame);
		IDataBase * db = resaFrame.GetDataBase();
		if(db == nil)
			break;

		InterfacePtr<IHierarchy> resaHier (resaFrame, UseDefaultIID());
		if(resaHier == nil)
			break;

		boost::shared_ptr<WideString> toInsert( new WideString(resaID));
		toInsert->Append(kTextChar_CR);
		toInsert->Append(WideString(resaAnnonceur));
		// Check whether it's already a text frame or not
		if(Utils<IPageItemTypeUtils>()->IsTextFrame(resaHier))
		{
			// Reset text in it to pub ID
			InterfacePtr<IMultiColumnTextFrame> txtFrame (db, resaHier->GetChildUID(0), UseDefaultIID());
			if(txtFrame == nil)
				break;

			InterfacePtr<ITextModelCmds> modelCmds (db, txtFrame->GetTextModelUID(), UseDefaultIID());
			if(modelCmds == nil)
				break;

			InterfacePtr<ITextModel> txtModel (modelCmds, UseDefaultIID());
			if(txtModel == nil)
				break;

			ErrorUtils::PMSetGlobalErrorCode(kSuccess);

			InterfacePtr<ICommand> replaceTextCmd(modelCmds->ReplaceCmd(0, txtModel->GetPrimaryStoryThreadSpan()-1, toInsert));
			if(CmdUtils::ProcessCommand(replaceTextCmd) != kSuccess)
				break;
		}
		else
		{
			// Convert to text frame
			InterfacePtr<ICommand> convertToTextCmd (CmdUtils::CreateCommand(kConvertItemToTextCmdBoss));
			convertToTextCmd->SetUndoability(ICommand::kAutoUndo);	
			convertToTextCmd->SetItemList(frameList);
			if(CmdUtils::ProcessCommand(convertToTextCmd) != kSuccess)
				break;
					
			InterfacePtr<IMultiColumnTextFrame> txtFrame (db, resaHier->GetChildUID(0), UseDefaultIID());
			if(!txtFrame)
				break;

			InterfacePtr<ITextModelCmds> modelCmds (db, txtFrame->GetTextModelUID(), UseDefaultIID());
			if(modelCmds == nil)
				break;
			
			InterfacePtr<ITextModel> txtModel (modelCmds, UseDefaultIID());
			if(txtFrame == nil)
				break;

			// Set text color to white
			boost::shared_ptr<AttributeBossList> attrList (new AttributeBossList());
			InterfacePtr<ITextAttrUID> textAttrUID_Color(::CreateObject2<ITextAttrUID>(kTextAttrColorBoss));						
			textAttrUID_Color->SetUIDData(Utils<ISwatchUtils>()->GetPaperSwatchUID(db));
			attrList->ApplyAttribute(textAttrUID_Color);

			InterfacePtr<ICommand> applyAttrCmd (modelCmds->ApplyCmd(0, 0, attrList, kCharAttrStrandBoss));
			if(CmdUtils::ProcessCommand(applyAttrCmd) != kSuccess)
				break;

			// Insert ID in resa	
			InterfacePtr<ICommand> insertTextCmd(modelCmds->InsertCmd(0, toInsert));
			if(CmdUtils::ProcessCommand(insertTextCmd) != kSuccess)
				break;			
		}
				
		// Set fill color		
		InterfacePtr<ICommand> fillCmd (Utils<IGraphicAttributeUtils>()->CreateFillRenderingCommand(Utils<ISwatchUtils>()->GetBlackSwatchUID(db),&frameList,kTrue,kTrue));
		if(CmdUtils::ProcessCommand(fillCmd) != kSuccess)
			break;
	
		// Set the tint (other method than this used to set the color : IGraphicAttributeUtils)
		int32 trameFond = 30;
		InterfacePtr<ICommand> fillTintCmd (Utils<IGraphicAttributeUtils>()->CreateFillTintCommand(trameFond,&frameList,kTrue,kTrue));
		if(CmdUtils::ProcessCommand(fillTintCmd) != kSuccess)
			break;

		status = kSuccess;

	}while(kFalse);

	return status;
}

void ImportPubFile(UIDRef resaPub, const IDFile& pubFile, PMString pubFileName)
{
	do
	{
		bool16 reimport = kTrue;

		IDataBase * db = resaPub.GetDataBase();
		if(db == nil)
			break;
		
		InterfacePtr<IHierarchy> currentPubHier (resaPub, UseDefaultIID());
		if(currentPubHier == nil)
			break;

		if(!FileUtils::DoesFileExist(pubFile))
			break;

		// on verifie si il y a deja une pub, et si le nom du fichier est le meme 
		InterfacePtr<IHierarchy> imageHier (currentPubHier->QueryChild(0));
		IDataBase* iDataBase = ::GetDataBase(imageHier);
		InterfacePtr<ILinkManager> iLinkManager(iDataBase, iDataBase->GetRootUID(), UseDefaultIID());
		if(iLinkManager ==nil){
			break;
		}
		
		InterfacePtr<ILinkObject> iLinkObject(imageHier, UseDefaultIID()); // poupee vaudoue...
		
		ILinkManager::QueryResult linkQueryResult;
	
		if (iLinkManager->QueryLinksByObjectUID(::GetUID(imageHier), linkQueryResult))
		{
			ILinkManager::QueryResult::const_iterator iterLinks = linkQueryResult.begin();
			InterfacePtr<ILink> iLink (db, *iterLinks, UseDefaultIID());
			if(iLink)
			{
				InterfacePtr<ILinkResource> iLinkResource(db, iLink->GetResource(), UseDefaultIID());
				if(iLinkResource)
				{
					IDFile imageFile;
					URI imageURI = iLinkResource->GetId();
					Utils<IURIUtils>()->URIToIDFile(imageURI, imageFile); 
					PMString fileNameWS, fileNameDoc;
					FileUtils::GetFileName(imageFile, fileNameWS);
					FileUtils::GetFileName(pubFileName, fileNameDoc);
					//CAlert::InformationAlert("FromXRail: " + fileNameWS);
					//CAlert::InformationAlert("FromINDD: " + fileNameDoc);
					//if(FileUtils::DoesFileExist(imageFile) && !FileUtils::IsDirectory(imageFile))
					//{
						//on ne compare QUE le nom du fichier, pas le chemin !
					//CAlert::InformationAlert(fileNameWS + " **** " + fileNameDoc);
						if (fileNameWS == fileNameDoc) {
							reimport = kFalse;
						}
					//}
				}
			}		
		}
		
		// le fichier pub est le meme, on verifie si il est a jour
		if (reimport == kFalse) {
            // Check whether it has been imported or not
            if(currentPubHier->GetChildCount() == 1)
            {
                // If we're on a placed image we should have a data link to source item
			InterfacePtr<ILinkManager> iLinkManagerPub(db, db->GetRootUID(), UseDefaultIID());
			InterfacePtr<ILinkObject> iLinkObjectPub(db, currentPubHier->GetChildUID(0), UseDefaultIID());	
			if(iLinkObjectPub)
                {
				ILinkManager::QueryResult linkQueryResultPub;
				if (iLinkManagerPub && iLinkManagerPub->QueryLinksByObjectUID(currentPubHier->GetChildUID(0), linkQueryResultPub))
                    {
					ILinkManager::QueryResult::const_iterator iterLinks = linkQueryResultPub.begin();
                        InterfacePtr<ILink> iLink (db, *iterLinks, UseDefaultIID());
                        if(iLink)
                        {
                            // The identifiers are used to get and set the modification state
                            ILink::ResourceModificationState state = iLink->GetResourceModificationState();
							if(state == ILink::kResourceUnmodified){
								reimport = kFalse;
                            }
							else {
								reimport = kTrue;
                            }
                        }
                    }
                }
			}
		}
		if (reimport){
            // Import image
            InterfacePtr<ICommand> importFileCmd(CmdUtils::CreateCommand(kImportResourceCmdBoss));
            if(importFileCmd == nil)
                break;
		
            InterfacePtr<IImportResourceCmdData> importFileCmdData(importFileCmd, IID_IIMPORTRESOURCECMDDATA);
            if(importFileCmdData == nil)
                break;

            // IDFile to URI
            URI pubFileURI;
            Utils<IURIUtils>()->IDFileToURI(pubFile, pubFileURI);

            // Set the file to import
            importFileCmdData->Set(db, pubFileURI, K2::kSuppressUI);
            if(CmdUtils::ProcessCommand(importFileCmd) != kSuccess)
            {
                break;
            }

		// Get the contents of the place gun
		InterfacePtr<IPlaceGun> placeGun(db, db->GetRootUID(), UseDefaultIID());
		ASSERT(placeGun);
		if (!placeGun)
		{
			ErrorUtils::PMSetGlobalErrorCode(kFailure);
			break;
		}
		
		InterfacePtr<ICommand> clearPlaceGun(CmdUtils::CreateCommand(kClearPlaceGunCmdBoss));
		ASSERT(clearPlaceGun);
		if (!clearPlaceGun)
		{
			ErrorUtils::PMSetGlobalErrorCode(kFailure);
			break;
		}
		
		InterfacePtr<IUIDData> uidData(clearPlaceGun, UseDefaultIID());
		ASSERT(uidData);
		if (!uidData)
		{
			ErrorUtils::PMSetGlobalErrorCode(kFailure);
			break;
		}
		uidData->Set(placeGun);
		CmdUtils::ProcessCommand(clearPlaceGun);
			
			

            UIDRef imageItem = UIDRef(db, importFileCmd->GetItemList()->First());

            // BUG FIX :  sometimes item resulting from importation
            // isn't directly an image item (kImageItem, kEPSItem, kPlacedPDFItem), but
            // a kSplineItemBoss, father of an image item
            UIDList deleteList(db);
		InterfacePtr<IHierarchy> imageHierPub (imageItem, UseDefaultIID());
		if(imageHierPub && imageHierPub->GetChildCount() == 1)
            {
                UIDRef splineItem = imageItem;
			imageItem = UIDRef(db,imageHierPub->GetChildUID(0));
                
                // Remove image item from container spline
                InterfacePtr<ICommand> removeFromHierCmd (CmdUtils::CreateCommand(kRemoveFromHierarchyCmdBoss));
                removeFromHierCmd->SetItemList(UIDList(imageItem));
                CmdUtils::ProcessCommand(removeFromHierCmd);

                // Delete useless spline item
                deleteList.Append(splineItem.GetUID());

                // Re-Create datalink cause it was on spline item which has just been deleted
                InterfacePtr<IDataLinkHelper> dlHelper (::CreateObject2<IDataLinkHelper>(kDataLinkHelperBoss));
                InterfacePtr<IDataLink> imageLink (dlHelper->CreateDataLink(pubFile));
                if(imageLink && dlHelper->NewDataLinkUID(db, imageLink) != 0)
                {
                    break;
                }
            }

            // Delete existing image item, if any
            if(currentPubHier && currentPubHier->GetChildCount() > 0)
            {
                deleteList.Append(currentPubHier->GetChildUID(0));
            }

            if(!deleteList.IsEmpty())
            {
                InterfacePtr<ICommand> deletePageItem (CmdUtils::CreateCommand(kDeleteCmdBoss));
                deletePageItem->SetItemList(deleteList);
                if(CmdUtils::ProcessCommand(deletePageItem) != kSuccess)
                    break;
            }

            // Place image in resapub
            InterfacePtr<ICommand> placeItemCmd(CmdUtils::CreateCommand(kPlaceItemInGraphicFrameCmdBoss));
            placeItemCmd->SetUndoability(ICommand::kAutoUndo);

            InterfacePtr<IPlacePIData> placeData(placeItemCmd,IID_IPLACEPIDATA);
            placeData->Set(resaPub,nil,kFalse);
            
            placeItemCmd->SetItemList(UIDList(imageItem));
            if(CmdUtils::ProcessCommand(placeItemCmd) != kSuccess)
                break;

            // Resize reserve frame and reset backgroung color
            UIDList frameList(resaPub);
			InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
			InterfacePtr<IXRailPrefsData>  xrailPrefsData((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));

			if (xrailPrefsData->GetPubProp() == 1) {
				if (!Utils<IFrameContentUtils>()->DoesContainGraphicFrame(&frameList, kFalse))
					break;

				UIDList* contentList = nil;
				contentList = Utils<IFrameContentUtils>()->CreateListOfContent(frameList);
				UIDRef content = UIDRef(db, contentList->At(0));
				InterfacePtr<ICommand> fitContentToFrameCmd(CmdUtils::CreateCommand(kFitContentToFrameCmdBoss));
				fitContentToFrameCmd->SetUndoability(ICommand::kAutoUndo);
				fitContentToFrameCmd->SetItemList(UIDList(content));

				if (CmdUtils::ProcessCommand(fitContentToFrameCmd) != kSuccess)
					break;

				//if (Utils<Facade::IFrameContentFacade>()->FillFrameProp(frameList) != kSuccess)
				//	break;
			}
			else {
				InterfacePtr<ICommand> fitFrameToContentCmd(CmdUtils::CreateCommand(kFitFrameToContentCmdBoss));
				fitFrameToContentCmd->SetUndoability(ICommand::kAutoUndo);
				fitFrameToContentCmd->SetItemList(frameList);
				if (CmdUtils::ProcessCommand(fitFrameToContentCmd) != kSuccess)
					break;
			}
            
            // Reset fill color
            InterfacePtr<ICommand> fillCmd (Utils<IGraphicAttributeUtils>()->CreateFillRenderingCommand(
                Utils<ISwatchUtils>()->GetNoneSwatchUID(db),&frameList,kTrue,kTrue));
            CmdUtils::ProcessCommand(fillCmd);
		}
	} while(kFalse);
}

// Cette fonction renvoie les id des liens ainsi que leurs chemins 
void GetLinksWithFilesOnDoc(UIDRef docUIDRef, UIDList & links, K2Vector<IDFile> & filesList){

 do{

	/* TODO rassemblement des elements d'une page
	 IDataBase * db = docUIDRef.GetDataBase();
		if(db == nil)
			break;
		
		InterfacePtr<ICoreFilename> cfn((ICoreFilename*)::CreateObject(kCoreFilenameBoss, IID_ICOREFILENAME));
		if (!cfn) 
			break;

		 InterfacePtr<ILinkManager> iLinkManager (db, db->GetRootUID(), UseDefaultIID());
		 if (iLinkManager == nil)
			 break;

       	 LinkQuery query;
		 ILinkManager::QueryResult queryResult;
		 iLinkManager->QueryLinks(query, queryResult);

      	 for (int32 i = 0; i < queryResult.size(); i++) {
			 
			 UIDRef linkRef = UIDRef(db, queryResult[i]);
			 InterfacePtr<ILink> aLink(linkRef, UseDefaultIID());
			 if(aLink == nil)
				 continue;
			
			 InterfacePtr<ILinkResource> linkResource(iLinkManager->QueryResourceByUID(aLink->GetResource()));
			
			 // Si le lien est absent, on passe au suivant
			 if (linkResource && linkResource->GetState() == ILinkResource::kMissing)
			 	continue;			

			 IDFile sysfile;				
			 Utils<IURIUtils>()->URIToIDFile(linkResource->GetURI(), sysfile);
			
			 // On ajoute le fichier et le lien
			 filesList.push_back(sysfile);
			 links.Append(linkRef.GetUID());
			
		 }	
	 */
	} while(kFalse);
 
}
	

void LockPageItemCmd(UIDRef itemToSet, bool8 toLock, bool8 allowUndo)
{
	ErrorCode status = kFailure;
	do{
		// Get the ILockPosition of the item to lock or unlock in order to verify its status
		InterfacePtr<ILockPosition> lockPosition(itemToSet,IID_ILOCKPOSITION);
		if (lockPosition == nil) {
			ASSERT_FAIL("XRail -> LockPageItemCmd -> lockPosition nil");
			break;
		}
		
		// Create a SetLockPositionCmd
		InterfacePtr<ICommand> setLPCmd(CmdUtils::CreateCommand(kSetLockPositionCmdBoss));
		if (setLPCmd == nil) {
			ASSERT_FAIL("XRail -> LockPageItemCmd -> setLPCmd nil");
			break;
		}
		
		// Set the SetLockPositionCmd's ItemList
		setLPCmd->SetItemList(UIDList(itemToSet));
		
		// Set command undoability
		if(!allowUndo)
			setLPCmd->SetUndoability(ICommand::kAutoUndo);
		
		// Get an IIntData Interface for the SetLockPositionCmd
		InterfacePtr<IIntData> intData(setLPCmd, IID_IINTDATA);
		if (intData == nil) {
			ASSERT_FAIL("XRail -> LockPageItem -> intData nil");
			break;
		}
		
		// Set the IIntData Interface's data
		if (toLock)
			intData->Set(ILockPosition::kLock);
		else
			intData->Set(ILockPosition::kUnlock);
		
		// Process the SetLockPositionCmd
		status = CmdUtils::ProcessCommand(setLPCmd);
		if (status != kSuccess) {
			ASSERT_FAIL("XRail -> LockPageItem -> command SetLockPositionCmd failed");
			break;
		}
		
	} while(false);
}
ErrorCode DoPackageDocument(IDocument * theDoc, IDFile folderFile)
{
	
	ErrorCode status = kFailure;
	do{
		
		PMString debug;
		FileUtils::IDFileToPMString(folderFile, debug);
		CAlert::InformationAlert(debug);
		
		if(!FileUtils::DoesFileExist(folderFile))
			break;
		
		if(!FileUtils::IsDirectory(folderFile)){
			break;
}

		InterfacePtr<IS4SPPackage> s4sPPackage((IS4SPPackage*)::CreateObject(kPackageAPIBoss, IID_IS4SPPACKAGEAPI));
		if (!s4sPPackage)
			break;
	
		s4sPPackage->Init(theDoc);
	
		//	s4sPPackage->SetHiddenLayersFlag(false);
		//	s4sPPackage->SetViewReportFlag(false);
		//	s4sPPackage->SetHyphExceptionsFlag(false);
		//	s4sPPackage->SetCopyFontsFlag(false);
		
		
		s4sPPackage->SetPackageLocation(folderFile);
		
		if(s4sPPackage->PhysicalPackage() != IS4SPPackage::kPackageOK){
			break;
		}		
		
		status = kSuccess;
	
	}while(kFalse);
	
	return status;
	 
}
