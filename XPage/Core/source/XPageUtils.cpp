#include "VCPluginHeaders.h"

// Interface includes
#include "IXPageUtils.h"
#include "IDOMElement.h"
#include "IScript.h"
#include "IScriptUtils.h"
#include "ITransformFacade.h"
#include "IDocumentCommands.h"
#include "IOpenLayoutCmdData.h"
#include "SDKFileHelper.h"
#include "IScrapItem.h"
#include "IStoryUtils.h"
#include "IWorkspace.h"
#include "IPageList.h"
#include "IOutputPages.h"
#include "IUIFlagData.h"
#include "ISysFileData.h"
#include "IStringData.h"
#include "IXPGTagToStyleMap.h"
#include "IXMLImportOptionsPool.h"
#include "IImportXMLData.h"
#include "IDocument.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "ISAXServices.h"
#include "ISAXContentHandler.h"
#include "ISAXParserOptions.h"
#include "ISAXEntityResolver.h"
#include "IIDXMLElement.h"
#include "IXMLImportOptions.h"
#include "IXMLUtils.h"
#include "IPMUnknownData.h"
#include "IXMLTagCommands.h"
#include "IUIDData.h"
#include "ITextControlData.h"
#include "IXPageMgrAccessor.h"
#include "ISubject.h"
#include "IObserver.h"
#include "ISnippetImport.h"
#include "IUIDListData.h"
#include "IGeometry.h"
#include "XMLDefs.h"
#include "IFrameUtils.h"
#include "IIntData.h"
#include "IMultiColumnTextFrame.h"
#include "IFormeData.h"
#include "IFrameList.h"
#include "ITextUtils.h"
#include "ITextModelCmds.h"
#include "IXMLTagList.h"
#include "IItemLockData.h"
#include "IAssignmentMgr.h"
#include "IInCopyWorkFlowUtils.h"
#include "IDataLink.h"
#include "TransformTypes.h"
#include "ITextFrameColumn.h"
#include "IGroupItemUtils.h"
#include "SnapshotUtils.h"
#include "IHierarchy.h"
#include "ISpreadLayer.h"
#include "IDocumentLayer.h"
#include "IGroupCmdData.h"
#include "IXMLStreamUtils.h"
#include "IXMLOutStream.h"
#include "IAttributeStrand.h"
#include "ITextStrand.h"
#include "IStyleInfo.h"
#include "IItemStrand.h"
#include "IPageItemTypeUtils.h"
#include "IPasteboardUtils.h"
#include "IGeometryFacade.h"
#include "SnapshotUtilsEx.h"
#include "ISelectionUtils.h"
#include "IXPageSuite.h"
#include "IStyleInfo.h"
#include "Utils.h"
#include "IWaxIterator.h"
#include "IAssignmentUtils.h"
#include "ITextStrand.h"
#include "ITextModel.h"
#include "IFrameList.h"
#include "IWaxStrand.h"
#include "IWaxLine.h"
#include "IWebServices.h"
#include "ILiveEditFacade.h"
#include "TextEditorModelID.h"
#include "ISnippetExport.h"
#include "InCopyImportID.h"
#include "IMasterSpreadList.h"
#include "IMasterSpread.h"
#include "INewDocCmdData.h"
#include "ITextStoryThread.h"
#include "IDocFileHandler.h"
#include "IDocumentUtils.h"
#include "IInCopyDocUtils.h"
#include "IAssignSetPropsCmdData.h"
#include "IDataStringUtils.h"
#include "IFrameType.h"
#include "IAssignedDocument.h"
#include "IGraphicFrameData.h"
#include "ITextAttributes.h"
#include "ITextAttrFont.h"
#include "IFontMgr.h"
#include "IPMFont.h"
#include "ITextUtils.h"
#include "ITextAttrUID.h"
#include "IPMFont.h"
#include "IFontFamily.h"
#include "IComposeScanner.h"
#include "IAttrReport.h"
#include "IStyleGroupHierarchy.h"

// General includes
#include "FileTypeRegistry.h"
#include "PageItemScrapID.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "StringUtils.h"
#include "ErrorUtils.h"
#include "StreamUtil.h"
#include "TransformUtils.h"
#include "XPGID.h"
#include "XMLDefs.h"
#include "LocaleSetting.h"
#include "ISpread.h"
#include "IClassIDData.h"
#include "XPGUIID.h"
#include "IPlacedArticleData.h"
#include "Utils.h"
#include "IXMLScriptUtils.h"
#include "TextStoryThreadHelper.h"
#include "IScriptManager.h"
#include "IK2ServiceRegistry.h"
#include "ITextColumnSizer.h"
#include "IUIColorUtils.h"
#include "IDTime.h"
#include <fstream>
#include "ILayoutUIUtils.h"
#include "ILinkManager.h"
#include "ILinkObject.h"
#include "ILayoutSelectionSuite.h"
#include "IInCopyBridgeUtils.h"
#include "XPGArticleDataNode.h"
#include "IPointerData.h"
#include "IImportResourceCmdData.h"
#include "IURIUtils.h"
#include "IPlacePIData.h"
#include "XPGImageDataNode.h"
#include "IFrameContentFacade.h"
#include "PlatformFolderTraverser.h"
#include "ILinkFacade.h"

#ifdef WINDOWS
#define PLATFORM_PATH_DELIMITER kTextChar_ReverseSolidus
#endif

using namespace std;

/** Implements IXPageUtils
*/
class XPageUtils : public CPMUnknown<IXPageUtils>
{
public:

	/**	Constructor
	 */
	XPageUtils(IPMUnknown* boss) : CPMUnknown<IXPageUtils>(boss) {}

	/**	Destructor
	 */
	virtual ~XPageUtils() {}

	/**
		Get list of subdirectories, returning pairs of (file path, file name)
	*/
	virtual IDValueList GetSubDirectories(const PMString& dirPath);

	/** Extract filename from path
	*/
	virtual PMString TruncatePath(const PMString& fromthis);

	/** Import XML article into a target story
	 */
	virtual ErrorCode ImportArticle(UIDList targetStory, IDFile articleFile, IDFile matchingFile, PMString& error, IDFile descriptionFile = IDFile(), PMString articleId = kNullString);

	/** Import XML article into a target story
	 */
	virtual ErrorCode ImportImages(K2Vector<UIDRef> blocsImages, IDFile articleFile, PMString& error);

	/** Import XML article into a target story
		 */
	virtual PMString GetImageFileName(IDataBase* db, UID curPict);

	/** Load all tags in an XML article file into document's tag list
	 */
	virtual ErrorCode LoadTagList(IDFile articleFile, IDocument * destDoc);

	/** Import a forme (snippet file) into the specified spread target at the specified position
	*/
	virtual ErrorCode ImportForme(UIDRef doc, IDFile formeFileToImport, PBPMPoint where, UIDRef targetSpread, IDFile matchingFile, PMString& error, UIDList * formeItems = nil, bool16 doTranslate = kFalse, bool16 setFormeName = kTrue);

	/** Set forme data in a spline item boss
	*/
	virtual ErrorCode SetFormeData(UIDRef splineItem, const PMString& formeUniqueName, const int16& type, const int16& photoIndex = -1, const PMString& posX = kNullString, const PMString& posY = kNullString);

	/** Set forme data in a text frame
	*/
	virtual ErrorCode SetFormeDataOnStory(UIDRef storyRef, const PMString& formeUniqueName, const int16& type, const K2Vector<IDValuePair> matchingStyleList, const int16& photoIndex = -1);

	/** Import credit or legend in target story, applying paragraph style from typo file associated to the story
	*/
	virtual ErrorCode ImportCreditOrLegend(UIDRef storyRef, const PMString& text, const bool16 isCredit);

	/** Debug methods that write log file in InDesign installation directory
	*/
	virtual void WriteLog(const PMString& msg);

	/** Generate preview of items
	*/
	virtual ErrorCode DoSnapShot(UIDList items, IDFile snapShotFile);

	/** Export stories in XML format into specified folder
	*/
	virtual ErrorCode ExportToXml(const UIDList& storyList, IDFile xmlFolder, PMString artID);

	/** Export stories
	*/

	virtual int32 GetNbSignes(const UIDList& storyList, IDataBase * db);

	virtual int32 GetPageId();
	virtual int32 GetPageID(UIDRef docUIDRef, int32 index);

	virtual ErrorCode LinkFramesCmd(UIDRef firstMultiCol, UIDRef secondMultiCol, bool8 allowUndo);

	virtual ErrorCode SetFormeStyleData(UIDList styleList, const PMString& formeUniqueName);

	/** Estimate Number of Charcters
	*/
	virtual void GetStatisticsText(UIDList itemsTexts, int32& nbCharcters, int32& nbParagraphs, int32& nbWords, int32& nbLines);

	virtual ErrorCode LoadStylesNamesFromSnippet(UIDRef doc, const IDFile &  sourceFile);

	virtual ErrorCode DoNewAssignment(UIDList items, IDFile assignmentFile, const PMString& articleName, const PMString& recipientLogin, bool16 exportArticle, const PMString& TECStoryID, const bool16 doJPEG);

	virtual ErrorCode DoAddToAssignment(IAssignment* assignment, const UIDList& items, UIDList& textStories, bool16 updateAssignment = kFalse);

	virtual UIDRef CreateDocument(const UIFlags uiFlags = K2::kFullUI, const PMReal& width = 1200,
		const PMReal& height = 1500, const int32 pages = 2, const int32 pagesPerSpread = 1);

	virtual void ListPath(const IDFile path, const PMString & filter, K2Vector<IDFile> &fileList);

	virtual bool16 validPath(const PMString & p);

	virtual K2Vector<IDValuePair> ParseFileMatching(const IDFile& matchingFile);

	virtual std::map<PMString, UIDList> GetFormeDataList(IDocument* theDoc, bool16 includeMasters = kFalse);

	virtual ErrorCode GetCartonItemsFromTarget(UIDRef itemRef, UIDList& formeItems);

	virtual K2Vector<KeyValuePair<PMString, K2Pair <PMString, UIDList> > > GetPlacedStoriesList(IDocument* theDoc);

	virtual ErrorCode GetPlacedArtItemsFromTarget(UIDRef itemRef, UIDList& placedArticleItems);

	virtual int32 GetPageNumber(const UIDRef &textFrameRef);

	virtual ErrorCode GetAllFormeItemsOnSpread(const PMString& idForme, ISpread* spread, UIDList& formeItems);

	virtual void CategorizeItems(const UIDList& items, UIDList& textFrames, UIDList& graphicFrames);

	virtual void SplitFormeDescription(const PMString& uniqueName, PMString& classeurName, PMString& formeName);

	virtual PMString GetStoryIDFromAssignmentFile(const IDFile& assignmentFile);

#if WINDOWS
	virtual bool16 DeleteDirectory(const TCHAR* sPath);  // utility function to delete a directory and all its subfolders
#else
	virtual bool16 DeleteDirectory(const char* sPath);
#endif

	virtual void GotoStoryUI(const PMString& idStory);

	virtual	UID	TriasFindStyleByName(IStyleGroupHierarchy *charStylesHierarchy, PMString searchStyle);

	virtual void HandleItemGroup(UIDList& storyList, UIDRef itemRef);

private:

	// Structure ad'hoc pour export XML
	typedef struct richtext
	{
		typedef object_type data_type;

		enum RichTextType { kText, kBeginTag, kEndTag };

		RichTextType type;
		WideString value;

		richtext(RichTextType t, WideString v)
		{
			type = t;
			value = v;
		}

	} RichText;

	/** Utility functions used by ExportToXml method
	*/
	void AppendToContent(K2Vector<KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > >& content, const Tag& aTag, const K2Vector<RichText>& richTextContent);

	void LookForStoriesToExport(UIDList& storyList, UIDRef itemRef);

	/** Utility functions used internally
	*/
	PMString  GetCurrentTime();

	void HandlePlacedItem(IHierarchy * itemHier, K2Vector<KeyValuePair<PMString, K2Pair<PMString, UIDList> > >& exportableStories);

	void CollectFormeDataOnSpread(ISpread* spread, int32 startContentLayer, int32 endContentLayer, std::map<PMString, UIDList>& formeDataList);

	void HandleFormeData(IHierarchy * itemHier, std::map<PMString, UIDList>& formeDataList);

	bool16 DoesStoryExistsOnPage(const PMString& idStory);

};

CREATE_PMINTERFACE(XPageUtils, kXPGUtilsImpl)

/**
GetSubDirectories
*/
IDValueList  XPageUtils::GetSubDirectories(const PMString& dirPath)
{

	K2Vector<KeyValuePair<PMString, PMString> >  dirList;

	IDFile searchPath = FileUtils::PMStringToSysFile(dirPath);

	PlatformFolderTraverser folderTraverser(searchPath, kTrue, kFalse, kFalse, kTrue);
	IDFile idFile;
	while (folderTraverser.Next(&idFile)) {
		if (FileUtils::IsDirectory(idFile) && idFile.GetFileName() != ".." && idFile.GetFileName() != ".") {
			dirList.push_back(KeyValuePair<PMString, PMString>(FileUtils::SysFileToPMString(idFile), idFile.GetFileName()));
		}

	}

	std::sort(dirList.begin(), dirList.end());

	return dirList;
}

PMString XPageUtils::TruncatePath(const PMString& fromthis)
{

	if (fromthis.IsEmpty() == kTrue) return PMString("Empty!");

	PMString retval = fromthis;
	int32 lastpos = (-1);

	for (int32 i = 0; i < fromthis.CharCount(); i++) {

		bool16 predicate = (fromthis[i] == PLATFORM_PATH_DELIMITER);
		if (predicate) {
			lastpos = i;
		}
	}
	if (lastpos >= 0) {
		// Suppose we have ../X/Y.gif
		// Then, lastpos would be 4, and we'd want 5 chars from 5 onwards
		int countChars = fromthis.CharCount() - (lastpos + 1);
		int startIndex = lastpos + 1;
		int endIndex = (startIndex + countChars);

		if ((endIndex > startIndex) && (endIndex <= fromthis.CharCount())) {
			K2::scoped_ptr<PMString> ptrRightStr(fromthis.Substring(startIndex, countChars));
			if (ptrRightStr) {
				retval = *ptrRightStr;
			}
		}
	}

	return retval;
}

ErrorCode XPageUtils::ImportArticle(UIDList targetStories, IDFile articleFile, IDFile matchingFile, PMString& error, IDFile descriptionFile, PMString articleId) {

	ErrorCode status = kFailure;
	do {
		IDataBase * db = targetStories.GetDataBase();
		InterfacePtr<IDocument> theDoc(db, db->GetRootUID(), UseDefaultIID());
		UIDRef docWS = theDoc->GetDocWorkSpace();

		// Load tag list so that we will only get style's name associated to tags to be imported
		if (XPageUtils::LoadTagList(articleFile, theDoc) != kSuccess) {
			// Reset error code and quit
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			error = PMString(kXPGErrParseArticleFailed);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(articleFile));
			break;
		}

		// Parse matching file to fill in tag to style mapping table
		InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));
		InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());
		InterfacePtr<ISAXContentHandler> contentHandler(static_cast<ISAXContentHandler*>(::CreateObject(kXPGXMLMatchingFileHandlerBoss, ISAXContentHandler::kDefaultIID)));
		InterfacePtr<IUIDData> docData(contentHandler, UseDefaultIID());
		docData->Set(theDoc);
		contentHandler->Register(saxServices);
		InterfacePtr<ISAXEntityResolver> entityResolver(static_cast<ISAXEntityResolver*>(::CreateObject(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID)));
		InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());
		// These commitments: follow 
		// the existing ImportXMLFileCmd
		parserOptions->SetNamespacesFeature(kFalse);
		parserOptions->SetShowWarningAlert(kFalse);
		InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(matchingFile));
		if (xmlFileStream == nil) {
			error = PMString(kXPGErrMatchingFileNotFound);
			break;
		}
		saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);
		if (ErrorUtils::PMGetGlobalErrorCode() != kSuccess) {
			// Reset error code and quit
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			error = PMString(kXPGErrParseMatchingFailed);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(matchingFile));
			break;
		}

		// Assign mapping table to the target stories
		InterfacePtr<IXPGTagToStyleMap> tag2styleMap(contentHandler, UseDefaultIID());
		InterfacePtr<ICommand> setMapCmd(CmdUtils::CreateCommand(kXPGSetTagToStyleMapCmd));
		setMapCmd->SetUndoability(ICommand::kAutoUndo);
		setMapCmd->SetItemList(targetStories);
		InterfacePtr<IXPGTagToStyleMap> cmdData(setMapCmd, UseDefaultIID());
		cmdData->Copy(tag2styleMap);
		CmdUtils::ProcessCommand(setMapCmd);

		// Process Import 
		for (int32 index = 0; index < targetStories.size(); index++) {
            
			// Process the import
			InterfacePtr<ICommand> importCmd(CmdUtils::CreateCommand(kImportXMLFileCmdBoss));

			// Make the import target the right element		
			InterfacePtr<IIDXMLElement> idRootElem(Utils<IXMLUtils>()->QueryRootElement(db));

			// Verify the context we're importing into
			XMLReference xmlRootRef = idRootElem->GetXMLReference();

			// Set up the import data
			InterfacePtr<IImportXMLData> importData((IImportXMLData*)::CreateObject(kImportXMLDataBoss, IID_IIMPORTXMLDATA));
			importData->Set(db, articleFile, xmlRootRef, kSuppressUI);

			InterfacePtr<IXMLImportOptionsPool> importDataOptions(importData, UseDefaultIID());

			// Set targets keys
			importDataOptions->SetKeyValue(WideString(kXPGXMLImportTargetKey), (int32)targetStories[index].Get()); // store import target in options pool
			importDataOptions->SetKeyValue(WideString(kXPGXMLDescFileKey), WideString(FileUtils::SysFileToPMString(descriptionFile))); // store related description file in options pool (for status updating after import)
			importDataOptions->SetKeyValue(WideString(kXPGXMLArticleIdKey), WideString(articleId)); // store article id
			importDataOptions->SetKeyValue(WideString(kXPGXMLIndexTargetKey), (int32)index); // index of current target
			importDataOptions->SetKeyValue(WideString(kXPGXMLCountTargetKey), (int32)targetStories.size()); // stories count

			// get the document's import options
			InterfacePtr<IXMLImportOptions> docOptions(docWS, UseDefaultIID());

			// set the import options
			InterfacePtr<IXMLImportOptions> importXMLOptions(importData, UseDefaultIID());
			importXMLOptions->Copy(docOptions);

			InterfacePtr<IPMUnknownData> iPMUnknownData(importCmd, UseDefaultIID());
			iPMUnknownData->SetPMUnknown(importData);

			if (CmdUtils::ProcessCommand(importCmd) != kSuccess) {
				error = PMString(kXPGErrImportFailed);
				error.Translate();
				error += ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::InformationAlert(error);
				break;
			}
		}
		// Clean up unused tags
		Utils<IXMLTagCommands>()->DeleteUnusedTags(docWS);
		status = kSuccess;

	} while (kFalse);

	return status;
}


ErrorCode XPageUtils::ImportImages(K2Vector<UIDRef> blocsImages, IDFile articleFile, PMString& error) {

	ErrorCode status;
	if (blocsImages.size() > 0) {
		status = kSuccess;
		do {
			XPGArticleDataNode * newNode = new XPGArticleDataNode(XPGArticleDataNode::kCorpsNode);

			// Parse xml file to get related text data
			InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));

			InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());

			InterfacePtr<ISAXContentHandler> contentHandler(::CreateObject2<ISAXContentHandler>(kXPGXMLParserArticleBoss, ISAXContentHandler::kDefaultIID));
			InterfacePtr<IPointerData> nodeData(contentHandler, IID_INODEDATA); // Pass node to the handler which will deal with text data initialization from xml data
			nodeData->Set(newNode);

			contentHandler->Register(saxServices);
			InterfacePtr<ISAXEntityResolver> entityResolver(::CreateObject2<ISAXEntityResolver>(kXMLEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
			InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());

			// These commitments: follow 
			// the existing ImportXMLFileCmd
			parserOptions->SetNamespacesFeature(kFalse);
			parserOptions->SetShowWarningAlert(kFalse);

			InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(articleFile));
			saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);

			xmlFileStream->Close();

			for (int32 index = 0; index < blocsImages.size(); index++) {
				// Get credit and legend frame (look for frames in the same spread)
				UIDRef parentUID = blocsImages[index];
				IDataBase * db = parentUID.GetDataBase();
				if (db == nil) {
					continue;
				}
                
				InterfacePtr<IFormeData> photoFormeData(parentUID, UseDefaultIID());
				if (photoFormeData != nil) {
                    if (photoFormeData->GetType() == IFormeData::kFixedContent)
                        continue;

					PMString currentPictFileName = GetImageFileName(db, parentUID.GetUID());
					
					PMString formeName = photoFormeData->GetUniqueName();
					int16 photoIndex = photoFormeData->GetPhotoIndex();
					if (photoIndex < newNode->GetArticleData()->images.size()) {
						bool16 doImport = kTrue;

						PMString racineArticle;
						FileUtils::GetPathOnly(articleFile, racineArticle, kTrue);
						IDFile xmlPhotoFile = IDFile(racineArticle + newNode->GetArticleData()->images[photoIndex]);
						if (!FileUtils::DoesFileExist(xmlPhotoFile)) {
							doImport = kFalse;
						}

						//on va maintenant parser le xml de l'article
						XPGImageDataNode * newImageNode = new XPGImageDataNode(XPGImageDataNode::kImageNode);

						// Parse xml file to get related text data
						InterfacePtr<IK2ServiceRegistry> serviceRegistryImage(GetExecutionContextSession(), UseDefaultIID());
						InterfacePtr<IK2ServiceProvider> xmlProviderImage(serviceRegistryImage->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));

						InterfacePtr<ISAXServices> saxServicesImage(xmlProviderImage, UseDefaultIID());

						InterfacePtr<ISAXContentHandler> contentHandlerImage(::CreateObject2<ISAXContentHandler>(kXPGXMLImageDescriptionHandlerBoss, ISAXContentHandler::kDefaultIID));
						InterfacePtr<IPointerData> nodeDataImage(contentHandlerImage, IID_INODEDATA); // Pass node to the handler which will deal with text data initialization from xml data
						nodeDataImage->Set(newImageNode);

						contentHandlerImage->Register(saxServicesImage);
						InterfacePtr<ISAXEntityResolver> entityResolverImage(::CreateObject2<ISAXEntityResolver>(kXMLEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
						InterfacePtr<ISAXParserOptions> parserOptionsImage(saxServicesImage, UseDefaultIID());

						// These commitments: follow 
						// the existing ImportXMLFileCmd
						parserOptionsImage->SetNamespacesFeature(kFalse);
						parserOptionsImage->SetShowWarningAlert(kFalse);

						InterfacePtr<IPMStream> xmlFileStreamImage(StreamUtil::CreateFileStreamRead(xmlPhotoFile));
						saxServicesImage->ParseStream(xmlFileStreamImage, contentHandlerImage, entityResolverImage);

						xmlFileStreamImage->Close();
						PMString currentPictFileName = GetImageFileName(db, parentUID.GetUID());
						if (currentPictFileName == newImageNode->GetData()) {
							//l'image est déjà importée, on réimporte pas
							doImport = kFalse;
						}
						IDFile imageFileToImport(newImageNode->GetData());
						//si le fichier image existe pas, on passe au coup de boucle suivant
						if (!FileUtils::DoesFileExist(imageFileToImport))
							doImport = kFalse;
						// Import credit and legend
						PMString creditData = newImageNode->GetImageData()->credit;
						PMString legendData = newImageNode->GetImageData()->legend;

						InterfacePtr<IHierarchy> containerFrame(parentUID, UseDefaultIID());
						bool16 returnCode = kTrue;

						if (doImport) {
						// Import image
						InterfacePtr<ICommand> importFileCmd(CmdUtils::CreateCommand(kImportResourceCmdBoss));
						InterfacePtr<IImportResourceCmdData> data(importFileCmd, IID_IIMPORTRESOURCECMDDATA);

						// IDFile to URI
						URI imageURI;
						Utils<IURIUtils>()->IDFileToURI(imageFileToImport, imageURI);

						// Set the file to import
						data->Set(db, imageURI, K2::kSuppressUI);
						if (CmdUtils::ProcessCommand(importFileCmd) != kSuccess) {
							CAlert::InformationAlert("Impossible d'importer cette image");
							break;
						}

						UIDRef imageItem = UIDRef(db, importFileCmd->GetItemList()->First());

						// Delete existing image item in target, if any
						if (containerFrame && containerFrame->GetChildCount() > 0) {
							InterfacePtr<IHierarchy> frameToDelete(containerFrame->QueryChild(0));
							InterfacePtr<ICommand> deletePageItem(CmdUtils::CreateCommand(kDeleteCmdBoss));
							deletePageItem->SetItemList(UIDList(frameToDelete));
							if (CmdUtils::ProcessCommand(deletePageItem) != kSuccess)
								continue;
						}

						InterfacePtr<IGeometry> itemGeo(parentUID, UseDefaultIID());
						if (itemGeo == nil)
							continue;

						PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();
						PMPoint rightBottom = itemGeo->GetStrokeBoundingBox().RightBottom();
						::TransformInnerPointToPasteboard(itemGeo, &leftTop);
						::TransformInnerPointToPasteboard(itemGeo, &rightBottom);

						PMPointList pointList;
						pointList.push_back(leftTop);
						pointList.push_back(rightBottom);

						// Place image item in target
						InterfacePtr<ICommand> placeItemCmd(CmdUtils::CreateCommand(kPlaceItemInGraphicFrameCmdBoss));
						placeItemCmd->SetUndoability(ICommand::kAutoUndo);

						InterfacePtr<IPlacePIData> placeData(placeItemCmd, IID_IPLACEPIDATA);
						placeData->Set(parentUID, &pointList, kFalse);

						// Set the item to place
						placeItemCmd->SetItemList(UIDList(imageItem));
							returnCode = CmdUtils::ProcessCommand(placeItemCmd);

						UIDList imageList = UIDList(imageItem);
						Utils<Facade::IFrameContentFacade>()->FitContentProp(imageList);

							
						}

						InterfacePtr<ISpread> imageSpread(db, containerFrame->GetSpreadUID(), UseDefaultIID());
						if (imageSpread == nil)
							continue;

						UIDList allFormeItems(db);
						Utils<IXPageUtils>()->GetAllFormeItemsOnSpread(formeName, imageSpread, allFormeItems);
						int32 nbItems = allFormeItems.Length();
						for (int32 i = 0; i < nbItems; ++i) {
							InterfacePtr<IFormeData> itemFormeData(allFormeItems.GetRef(i), UseDefaultIID());
							int currentPhotoIndex = itemFormeData->GetPhotoIndex();
							if (itemFormeData->GetType() == IFormeData::kCredit && currentPhotoIndex == photoIndex) {
								// A credit frame associated with the photo has been found, import credit text in it
								UIDRef targetStory = ::GetUIDRef(itemFormeData);
								Utils<IXMLUtils>()->GetActualContent(targetStory);
								Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, creditData, kTrue);
							}
							else if (itemFormeData->GetType() == IFormeData::kLegend && currentPhotoIndex == photoIndex) {
								// A legend frame associated with the photo has been found, import credit text in it
								UIDRef targetStory = ::GetUIDRef(itemFormeData);
								Utils<IXMLUtils>()->GetActualContent(targetStory);
								Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, legendData, kFalse);
							}
						}
						status = returnCode;

					}
				}

			}
		} while (kFalse);
	}
	else {
		status = kSuccess;
	}
	return kSuccess;
}


//-----------------------------------------------------------
// Renvoi le chemin d'acces de l'image du bloc selectionne
//-----------------------------------------------------------
PMString XPageUtils::GetImageFileName(IDataBase* db, UID curPict)
{
	PMString result = "";
	// On recupere le ILinkManager
	do {
		InterfacePtr<ILinkManager> iLinkManager(db, db->GetRootUID(), UseDefaultIID());
		if (iLinkManager == nil) {
			break;
		}

		InterfacePtr<IHierarchy> resaHier(db, curPict, UseDefaultIID());
		if (resaHier == nil) {
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
			if (iLink == nil)
				break;
			InterfacePtr<ILinkResource> iLinkResource(db, iLink->GetResource(), UseDefaultIID());
			if (iLinkResource == nil)
				break;
			IDFile imageFile;
			URI imageURI = iLinkResource->GetId();
			Utils<IURIUtils>()->URIToIDFile(imageURI, imageFile);
			if (FileUtils::DoesFileExist(imageFile) && !FileUtils::IsDirectory(imageFile))
			{
				result = FileUtils::SysFileToPMString(imageFile);
			}
		}
	} while (kFalse);
	return result;
}

ErrorCode XPageUtils::LoadTagList(IDFile articleFile, IDocument * destDoc) {

	InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));

	InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());

	InterfacePtr<ISAXContentHandler> contentHandler(::CreateObject2<ISAXContentHandler>(kXPGXMLLoadTagHandlerBoss, ISAXContentHandler::kDefaultIID));
	InterfacePtr<IUIDData> docData(contentHandler, UseDefaultIID());
	docData->Set(destDoc);

	contentHandler->Register(saxServices);

	// Here we don't use kXMLEmptyEntityResolverBoss, which causes the failure of the parsing process whenever the article xml contains an entity such as &nbsp
	// Using kXMLEntityResolverBoss solves this problem
	InterfacePtr<ISAXEntityResolver> entityResolver(::CreateObject2<ISAXEntityResolver>(kXMLEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
	InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());

	// These commitments: follow 
	// the existing ImportXMLFileCmd
	parserOptions->SetNamespacesFeature(kFalse);
	parserOptions->SetShowWarningAlert(kFalse);

	InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(articleFile));

	saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);

	return ErrorUtils::PMGetGlobalErrorCode();
}


ErrorCode XPageUtils::ImportForme(UIDRef doc, IDFile formeFileToImport, PBPMPoint where, UIDRef targetSpread, IDFile matchingFile, PMString& error, UIDList * formeItems, bool16 doTranslate, bool16 setFormeName)
{
	ErrorCode status = kFailure;
	do {

		InterfacePtr<IPMStream> snippetStream(StreamUtil::CreateFileStreamRead(formeFileToImport));
		if (!snippetStream) {
			break;
		}

		InterfacePtr<IDOMElement> rootElement(targetSpread, UseDefaultIID());
		if (!rootElement) {
			break;
		}

		InterfacePtr<ISubject> docSubject(doc, UseDefaultIID());
		if (!docSubject) {
			break;
		}

		InterfacePtr<IObserver> importObserver(doc, IID_IIMPORTSNIPPETOBSERVER);
		if (!importObserver) {
			break;
		}

		// We attach an observer so that we can get back snippet items' UID and store them 
		// in IID_ILASTIMPORTEDSNIPPET interface, aggregated on kDocBoss
		docSubject->AttachObserver(importObserver, IID_IHIERARCHY_DOCUMENT, IID_IIMPORTSNIPPETOBSERVER);

		// BUG FIX FROM ADOBE : resolves insets not taken into account (bug La Tribune)
		ICommandSequence* seq = CmdUtils::BeginCommandSequence();

		ErrorCode importResult = Utils<ISnippetImport>()->ImportFromStream(snippetStream, rootElement);

		CmdUtils::EndCommandSequence(seq);

		// Observer was called during import, we can detach now
		docSubject->DetachObserver(importObserver, IID_IHIERARCHY_DOCUMENT, IID_IIMPORTSNIPPETOBSERVER);

		// Get back uids of newly created page items from import, then clean up list
		InterfacePtr<IUIDListData> importedItems(doc, IID_ILASTIMPORTEDSNIPPET);

		if (importResult != kSuccess || (importedItems->GetRef() == nil)) {
			error = PMString(kXPGErrFormeImportFailed);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(formeFileToImport));
			break;
		}

		UIDList newPageItems(*(importedItems->GetRef()));
		importedItems->Set(nil); // We reset the item list for next snippet import

		// Define a new forme name so that it is unique in the document. To do so, append current time
		IDTime now;
		now.CurrentTime();
		int32 year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
		now.GetTime(&year, &month, &day, &hour, &min, &sec);

		UIDList snippetItems(doc.GetDataBase());
		PMPoint leftTopMost(kMaxInt32, kMaxInt32);
		PMString formeUniqueName = "";
		//on va fabriquer le nom du carton
		for (int32 i = 0; i < newPageItems.Length(); ++i) {
			InterfacePtr<IGeometry> itemGeo(newPageItems.GetRef(i), UseDefaultIID());
			InterfacePtr<IFormeData> itemFormeData(itemGeo, UseDefaultIID());
			formeUniqueName = itemFormeData->GetUniqueName();
			if (formeUniqueName != "") {
				formeUniqueName += "_";
				formeUniqueName.AppendNumber(year);
				if (month < 10) {
					formeUniqueName.AppendNumber(0);
				}
				formeUniqueName.AppendNumber(month);
				if (day < 10) {
					formeUniqueName.AppendNumber(0);
				}
				formeUniqueName.AppendNumber(day);
				if (hour < 10) {
					formeUniqueName.AppendNumber(0);
				}
				formeUniqueName.AppendNumber(hour);
				if (min < 10) {
					formeUniqueName.AppendNumber(0);
				}
				formeUniqueName.AppendNumber(min);
				if (sec < 10) {
					formeUniqueName.AppendNumber(0);
				}
				formeUniqueName.AppendNumber(sec);
				break;
			}
		}

		if (formeUniqueName == "") {
			break;
		}

		for (int32 i = 0; i < newPageItems.Length(); ++i) {
			InterfacePtr<IGeometry> itemGeo(newPageItems.GetRef(i), UseDefaultIID());
			PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();
			::TransformInnerPointToPasteboard(itemGeo, &leftTop);
			if (leftTop.X() < leftTopMost.X())
				leftTopMost.X() = leftTop.X();
			if (leftTop.Y() < leftTopMost.Y())
				leftTopMost.Y() = leftTop.Y();
			if (!Utils<IFrameUtils>()->IsPageItemAnchoredObject(newPageItems.GetRef(i)))
			{
				snippetItems.Append(newPageItems.At(i));
			}
			// for text frames, store matching file path for use in later import
			if (Utils<IFrameUtils>()->IsTextFrame(itemGeo) && matchingFile != IDFile()) {

				IDataBase * db = doc.GetDataBase();
				InterfacePtr<IHierarchy> itemHier(itemGeo, UseDefaultIID());
				InterfacePtr<IMultiColumnTextFrame> txtFrame(db, itemHier->GetChildUID(0), UseDefaultIID());

				// Set info carton with command			
				InterfacePtr<ICommand> setStringCmd(CmdUtils::CreateCommand(kXPGSetStringCmdBoss));
				setStringCmd->SetItemList(UIDList(UIDRef(db, txtFrame->GetTextModelUID())));

				InterfacePtr<IIntData> interfaceData(setStringCmd, UseDefaultIID());
				interfaceData->Set(IID_ITYPOINFO);

				InterfacePtr<IStringData> matchingData(setStringCmd, UseDefaultIID());
				matchingData->Set(FileUtils::SysFileToPMString(matchingFile));

				CmdUtils::ProcessCommand(setStringCmd);
			}
			// Set item's new forme name
			InterfacePtr<IFormeData> itemFormeData(itemGeo, UseDefaultIID());

			if (setFormeName) {
			SetFormeData(::GetUIDRef(itemFormeData), formeUniqueName, itemFormeData->GetType(), itemFormeData->GetPhotoIndex());
		}
		}
		if (doTranslate == kTrue) {
			// Compute delta between required position and current position after importation
			PMPoint delta = where - leftTopMost;
			status = Utils<Facade::ITransformFacade>()->TransformItems(snippetItems, Transform::PasteboardCoordinates(), kZeroPoint, Transform::TranslateBy(delta.X(), delta.Y()));
			
			if (status != kSuccess) {
				error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::InformationAlert(error);
				status = kSuccess;
			}
		}
		else {
			status = kSuccess;
		}

		if (formeItems)
			*formeItems = newPageItems;

		status = kSuccess;
	} while (kFalse);
	return kSuccess;
}

ErrorCode XPageUtils::SetFormeData(UIDRef splineItem, const PMString& formeUniqueName, const int16& type, const int16& photoIndex, const PMString& posX, const PMString& posY)
{

	InterfacePtr<ICommand> setFormeDataCmd(CmdUtils::CreateCommand(kXPGSetFormeDataCmdBoss));
	setFormeDataCmd->SetItemList(UIDList(splineItem));

	InterfacePtr<IFormeData> cmdData(setFormeDataCmd, UseDefaultIID());
	cmdData->SetUniqueName(formeUniqueName);
	cmdData->SetType(type);
	cmdData->SetPhotoIndex(photoIndex);
	return CmdUtils::ProcessCommand(setFormeDataCmd);
}

ErrorCode XPageUtils::SetFormeStyleData(UIDList styleList, const PMString& formeUniqueName)
{

	InterfacePtr<ICommand> setFormeStyleDataCmd(CmdUtils::CreateCommand(kXPGSetFormeStyleDataCmdBoss));
	setFormeStyleDataCmd->SetItemList(styleList);

	InterfacePtr<IFormeData> cmdData(setFormeStyleDataCmd, UseDefaultIID());
	cmdData->SetUniqueName(formeUniqueName);
	cmdData->SetType(IFormeData::kStyle);
	return CmdUtils::ProcessCommand(setFormeStyleDataCmd);
}


ErrorCode XPageUtils::SetFormeDataOnStory(UIDRef storyRef, const PMString& formeUniqueName, const int16& type,
	const K2Vector<IDValuePair> matchingStyleList, const int16& photoIndex)
{
	ErrorCode status = kFailure;
	do {

		UIDList storySplineItems(storyRef.GetDataBase());

		// Collect all text frames from story
		InterfacePtr<ITextModel> txtModel(storyRef, UseDefaultIID());
		if (txtModel == nil)
			break;

		InterfacePtr<IFrameList> frameList(txtModel->QueryFrameList());
		if (frameList == nil)
			break;

		int32 nbFrames = frameList->GetFrameCount();
		for (int32 i = 0; i < nbFrames; ++i) {

			InterfacePtr<ITextFrameColumn> txtFrame(frameList->QueryNthFrame(i));
			InterfacePtr<IHierarchy> splineItem(Utils<ITextUtils>()->QuerySplineFromTextFrame(txtFrame));

			UID splineUID = ::GetUID(splineItem);
			if (storySplineItems.DoesNotContain(splineUID))
				storySplineItems.Append(splineUID);
		}

		// Set forme data with command
		InterfacePtr<ICommand> setFormeDataCmd(CmdUtils::CreateCommand(kXPGSetFormeDataCmdBoss));
		setFormeDataCmd->SetItemList(storySplineItems);
		InterfacePtr<IFormeData> cmdData(setFormeDataCmd, UseDefaultIID());
		cmdData->SetUniqueName(formeUniqueName);
		cmdData->SetType(type);
		cmdData->SetPhotoIndex(photoIndex);
		status = CmdUtils::ProcessCommand(setFormeDataCmd);

	} while (kFalse);

	return status;

}

ErrorCode XPageUtils::ImportCreditOrLegend(UIDRef storyRef, const PMString& text, const bool16 isCredit) {

	ErrorCode status = kFailure;
	InterfacePtr<IXPGPreferences> xpgPrefs(GetExecutionContextSession(), UseDefaultIID());

	if (isCredit && !xpgPrefs->GetImportCredit()) {
		status = kSuccess;
		return status;
	}
	if (!isCredit && !xpgPrefs->GetImportLegende()) {
		status = kSuccess;
		return status;
	}

	PMString error = kNullString;

	InterfacePtr<ITextModel> txtModel(storyRef, UseDefaultIID());
	InterfacePtr<ITextModelCmds> txtModelCmds(txtModel, UseDefaultIID());

	// Get lock state of story and unlock it if necessary
	if (Utils<IStoryUtils>()->IsAStory(::GetUIDRef(txtModel))) {
		InterfacePtr<IItemLockData> lockData(txtModel, UseDefaultIID());
		if (lockData->GetInsertLock())
			Utils<Facade::ILiveEditFacade>()->CheckIn(UIDList(txtModel));
	}

	do {
		if (text == kNullString || text == PMString("")) { // No text to import -> exit with succes		
			status = kSuccess;
			break;
		}

		// Delete previous existing text		
		if (txtModel->GetPrimaryStoryThreadSpan() > 1) {
			InterfacePtr<ICommand> deleteCmd(txtModelCmds->DeleteCmd(0, txtModel->GetPrimaryStoryThreadSpan() - 1));
			CmdUtils::ProcessCommand(deleteCmd);
		}


		// Get typo file associated to the story if any
		InterfacePtr<IStringData> typoData(storyRef, IID_ITYPOINFO);
		if (!typoData) { // Should not occur
			status = kSuccess;
			break;
		}

		IDFile matchingFile = FileUtils::PMStringToSysFile(typoData->Get());

		UID styleUID = kInvalidUID;
		if (FileUtils::DoesFileExist(matchingFile)) {

			IDataBase * db = storyRef.GetDataBase();
			InterfacePtr<IDocument> theDoc(db, db->GetRootUID(), UseDefaultIID());

			// Assign credit or legend tag to the document
			InterfacePtr<IXMLTagList> tagList(theDoc->GetDocWorkSpace(), UseDefaultIID());

			PMString creditOrLegendTag = isCredit ? creditTag : legendTag;
			UID theTag = tagList->GetTag(WideString(creditOrLegendTag));
			if (theTag == kInvalidUID) {
				UID dummy = kInvalidUID;
				Utils<IXMLUtils>()->AssignTagUID(WideString(creditOrLegendTag), db, kInvalidUID, dummy);
			}

			// Parse matching file to get mapping styles
			InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));

			InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());
			InterfacePtr<ISAXContentHandler> contentHandler(::CreateObject2<ISAXContentHandler>(kXPGXMLMatchingFileHandlerBoss, ISAXContentHandler::kDefaultIID));
			InterfacePtr<IUIDData> docData(contentHandler, UseDefaultIID());
			docData->Set(theDoc);
			contentHandler->Register(saxServices);
			InterfacePtr<ISAXEntityResolver> entityResolver(::CreateObject2<ISAXEntityResolver>(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
			InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());

			// These commitments: follow 
			// the existing ImportXMLFileCmd
			parserOptions->SetNamespacesFeature(kFalse);
			parserOptions->SetShowWarningAlert(kFalse);

			InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(matchingFile));
			if (xmlFileStream == nil) {
				error = PMString(kXPGErrMatchingFileNotFound);
				break;
			}

			saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);

			if (ErrorUtils::PMGetGlobalErrorCode() != kSuccess) {
				// Reset error code and quit
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				error = PMString(kXPGErrParseMatchingFailed);
				error.Translate();
				StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(matchingFile));
				break;
			}

			// Get style associated to credit (or legend) tag
			InterfacePtr<IXPGTagToStyleMap> tag2styleMap(contentHandler, UseDefaultIID());
			styleUID = tag2styleMap->GetParaStyleMappedToTag(creditOrLegendTag);
		}

		// Insert new text		
		boost::shared_ptr<WideString> data(new WideString(text));

		InterfacePtr<ICommand> insertCmd(txtModelCmds->InsertCmd(0, data));
		status = CmdUtils::ProcessCommand(insertCmd);

		// Apply paragraph style, if any
		if (styleUID != kInvalidUID && status == kSuccess) {
			InterfacePtr<ICommand> applyParaStyleCmd(txtModelCmds->ApplyStyleCmd(0, data->Length(), styleUID, kParaAttrStrandBoss));
			status = CmdUtils::ProcessCommand(applyParaStyleCmd);
		}

	} while (kFalse);

	if (status != kSuccess)
		CAlert::InformationAlert(error);

	return status;
}

ErrorCode XPageUtils::DoSnapShot(UIDList items, IDFile snapShotFile) {

	ErrorCode status = kFailure;
	SnapshotUtils * snapshotUtils = nil;
	ICommandSequence* seq = nil;
	InterfacePtr<ICommand> groupCmd(nil);
	UIDRef itemToExport = UIDRef::gNull;
	do {
		// Create a command sequence
		seq = CmdUtils::BeginCommandSequence();
		if (seq == nil)
			break;

		seq->SetUndoability(ICommand::kAutoUndo);
		PMReal scale = 1.0, res = 150.0;

		UIDList txtFrames, graphicFrames;
		this->CategorizeItems(items, txtFrames, graphicFrames);

		UIDList allItems(txtFrames);
		allItems.Append(graphicFrames);

		PMRect itemsBound = Utils<Facade::IGeometryFacade>()->GetItemsBounds(allItems, Transform::PasteboardCoordinates(), Geometry::OuterStrokeBounds());

		IDataBase * db = allItems.GetDataBase();
		UIDRef anItem = allItems.GetRef(0);

		InterfacePtr<IHierarchy> itemHier(anItem, UseDefaultIID());
		InterfacePtr<ISpread> spread(db, itemHier->GetSpreadUID(), UseDefaultIID());

		//  In cs3 you can always specify in global pasteboard coords. 
		//	With cs4 I had to convert using i.e. ::TransformPasteboardRectToInner( iSpread, &spreadRelativeRect )
		//  and pass the spreads UIDRef as second parameter in	

		::TransformPasteboardRectToInner(spread, &itemsBound);

		snapshotUtils = new SnapshotUtils(itemsBound,
			PMMatrix(),
			::GetDataBase(itemHier),
			scale,					// X Scale
			scale,					// Y Scale
			res,						// desired resolution of resulting snapshot
			res,						// minimum resolution of resulting snapshot
			0,						// Extend the bounds of the area to be drawn by the given amount of bleed
			IShape::kPrinting,	    // Drawing flags (kPrinting suppresses drawing of margins and guides)
			kTrue,					// kTrue forces images and graphics to draw at full resolution, kFalse to draw proxies
			kFalse,					// kTrue to draw grayscale, kFalse to draw RGB
			kFalse);					// kTrue to add an alpha channel, kFalse no alpha channel

		if ((snapshotUtils == nil) || snapshotUtils->GetStatus() != kSuccess)
			break;

		// Draw each item
		for (int32 i = 0; i < allItems.Length(); ++i)
			snapshotUtils->Draw(allItems[i]);

		// Create a stream to write out
		InterfacePtr<IPMStream> jpegStream(StreamUtil::CreateFileStreamWrite(snapShotFile, kOpenOut | kOpenTrunc, 'JPEG', kAnyCreator));
		if (jpegStream == nil)
			break;

		// Export to JPEG
		if (snapshotUtils->ExportImageToJPEG(jpegStream, SnapshotUtils::kSSJPEGBaseline, SnapshotUtils::kSSJPEGExcellentQuality) != kSuccess)
			break;

		status = kSuccess;
	} while (kFalse);

	if (seq != nil)
	{
		if (status != kSuccess)
			ErrorUtils::PMSetGlobalErrorCode(kFalse);

		CmdUtils::EndCommandSequence(seq);
	}

	if (snapshotUtils != nil)
	{
		delete snapshotUtils;
		snapshotUtils = nil;
	}
	return status;
}



ErrorCode XPageUtils::DoNewAssignment(UIDList items, IDFile assignmentFile, const PMString& articleName, const PMString& recipientLogin, bool16 exportArticle, const PMString& TECStoryID, const bool16 doJPEG) {

	ErrorCode status = kFailure;

	do {

		// Ensure parent directories exist
		IDFile inCopyArticleFolder;
		FileUtils::GetParentDirectory(assignmentFile, inCopyArticleFolder);
		FileUtils::CreateFolderIfNeeded(inCopyArticleFolder, kTrue);

		// Check whether folder was created or not : user may not have write access to this path
		if (!FileUtils::DoesFileExist(inCopyArticleFolder))
		{
			CAlert::ErrorAlert(kXPGErrFailedToCreateInCopyFiles);
			break;
		}

		// Get Database and Front Document
		IDataBase * db = items.GetDataBase();
		if (db == nil)
			break;

		InterfacePtr<IDocument> theDoc(db, db->GetRootUID(), UseDefaultIID());
		if (theDoc == nil)
			break;

		// Assignment Manager
		InterfacePtr<IAssignmentMgr> inCopyMgr(GetExecutionContextSession(), UseDefaultIID());
		if (inCopyMgr == nil)
			break;

		// Assignment Document
		InterfacePtr<IAssignedDocument> assignedDoc(theDoc, UseDefaultIID());
		if (assignedDoc == nil)
			break;

		IAssignment::AssignmentExportOptions options = IAssignment::kAssignmentExportSpreads;
		UID color = Utils<IUIColorUtils>()->GetUIColor(IUIColorUtils::kFirstUIColor, theDoc).GetUID();

		bool16 dirty = kFalse, includeLinked = kTrue;

		//Make new assignment 
		InterfacePtr<ICommand> assignDocCmd(CmdUtils::CreateCommand(kAssignDocCmdBoss));
		UIDList uidl(::GetUIDRef(assignedDoc));
		assignDocCmd->SetItemList(uidl);
		IAssignSetPropsCmdDataPtr data(assignDocCmd, UseDefaultIID());
		PMString assignmentString = FileUtils::SysFileToPMString(assignmentFile);
		data->Set(&articleName, &assignmentString, &recipientLogin, &color, &options, &dirty, &includeLinked);

		ErrorCode err = CmdUtils::ProcessCommand(assignDocCmd);
		if (err != kSuccess)
			break;

		IAssignmentPtr assignment(assignDocCmd->GetItemList()->GetRef(0), UseDefaultIID());
		if (!assignment)
			break;

		UIDList textStories(db);
		if (this->DoAddToAssignment(assignment, items, textStories, kTrue) != kSuccess)
			break;

		if (exportArticle)
		{
			PMString artID = Utils<IXPageUtils>()->GetStoryIDFromAssignmentFile(assignmentFile);

			if (doJPEG) {
				// Create Vignette		
				IDFile artVignette = inCopyArticleFolder;
				FileUtils::AppendPath(&artVignette, artID + ".jpg");
				if (Utils<IXPageUtils>()->DoSnapShot(items, artVignette) != kSuccess) {
					CAlert::InformationAlert(kXPGErrDoSnapShotFailed);
					break;
				}
			}
			// On calcul le nombre de signes des blocs textes
			int32 nbSignes = Utils<IXPageUtils>()->GetNbSignes(textStories, db);

			// Export XML 			
			if (Utils<IXPageUtils>()->ExportToXml(textStories, inCopyArticleFolder, artID) != kSuccess) {
				CAlert::InformationAlert(kXPGErrExportXmlFailed);
				break;
			}

			// Update XML and SnapShot with WS_StoryContentChanged
			InterfacePtr<IXPGPreferences> xpgPrefs(GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IWebServices> xrailConnexion(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
			xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL());

			if (!xrailConnexion->StoryContentChange(artID, nbSignes))
			{
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				break;
			}
		}

		status = kSuccess;

	} while (kFalse);

	return status;
}

ErrorCode XPageUtils::DoAddToAssignment(IAssignment* assignment, const UIDList& items, UIDList& textStories, bool16 updateAssignment)
{
	ErrorCode status = kFailure;
	do
	{
		IDataBase * db = items.GetDataBase();
		if (db == nil)
			break;

		InterfacePtr<IXPGPreferences> xpgPrefs(GetExecutionContextSession(), UseDefaultIID());

		// Handle group items
		UIDList allItems(db);
		for (int32 i = 0; i < items.size(); i++)
		{
			HandleItemGroup(allItems, items.GetRef(i));
		}

		// Sort all items stories with Y position
		for (int32 j = 1; j < allItems.Length(); j++) {

			InterfacePtr<IGeometry> itemGeo1(allItems.GetRef(j), UseDefaultIID());
			PMPoint leftTop1 = itemGeo1->GetStrokeBoundingBox().LeftTop();
			::TransformInnerPointToPasteboard(itemGeo1, &leftTop1);

			int32 i = j - 1;
			InterfacePtr<IGeometry> itemGeo2(allItems.GetRef(i), UseDefaultIID());
			PMPoint leftTop2 = itemGeo2->GetStrokeBoundingBox().LeftTop();
			::TransformInnerPointToPasteboard(itemGeo2, &leftTop2);

			// la valeur doit Ítre insÈrÈe dans le tableau ordonnÈ 0..j-1
			while (i >= 0 && leftTop2.Y() > leftTop1.Y()) {
				allItems[i + 1] = allItems[i];
				i = i - 1;
			}
			allItems[i + 1] = allItems[j];
		}

		IDFile assignmentFile = FileUtils::PMStringToSysFile(assignment->GetFile());
		if (!FileUtils::DoesFileExist(assignmentFile))
			break;

		PMString artID = Utils<IXPageUtils>()->GetStoryIDFromAssignmentFile(assignmentFile);

		IDFile storiesFolder;
		FileUtils::GetParentDirectory(assignmentFile, storiesFolder);
		FileUtils::AppendPath(&storiesFolder, artID);
		FileUtils::CreateFolderIfNeeded(storiesFolder, kTrue);

		InterfacePtr<IAssignmentMgr> inCopyMgr(GetExecutionContextSession(), UseDefaultIID());
		if (inCopyMgr == nil)
			break;

		UIDList createdStories(db);
		textStories = UIDList(db);
		for (int32 i = 0; i < allItems.Length(); ++i) {

			UIDRef itemRef = allItems.GetRef(i);

			// This allows to get UIDRef of text model if selected frame is a text frame
			Utils<IAssignmentUtils>()->NormalizeTextModelRef(itemRef);

			// Determine if it's a text model
			InterfacePtr<ITextModel> isATextModel(itemRef, UseDefaultIID());
			if (isATextModel)
			{
				textStories.Append(itemRef.GetUID());
			}

			if (createdStories.Contains(itemRef.GetUID()))
				// To avoid to create several stories for linked frames which actually consist in one story
				continue;
			else
				createdStories.Append(itemRef.GetUID());

			// Determine story file name
			int32 storiesCount = assignment->GetStories().size();

			PMString storyName = artID + "_";
			PMString extension = PMString(kXPGExtensionAffectationKey, PMString::kTranslateDuringCall);
			storyName.AppendNumber(storiesCount + 1);
			storyName.Append(extension);

			IDFile incopyFile = storiesFolder;
			FileUtils::AppendPath(&incopyFile, storyName);

			int32 lastIncopyFileIndex = storiesCount + 1;

			while (FileUtils::DoesFileExist(incopyFile)) {

				lastIncopyFileIndex++;

				incopyFile = IDFile();
				storyName.clear();

				storyName.Append(artID);
				storyName.Append("_");
				storyName.AppendNumber(lastIncopyFileIndex);
				storyName.Append(extension);

				incopyFile = storiesFolder;
				FileUtils::AppendPath(&incopyFile, storyName);
			}

			UIDList itemListToExport(itemRef);

			OSType creator = FileTypeRegistry::GetCurrentFileCreator(kInCopyXMLMarkupFileTypeInfoID); // Or kInCopyXMLInterchangeFileTypeInfoID ?
			OSType fti = FileTypeRegistry::GetCurrentFileType(kInCopyXMLMarkupFileTypeInfoID);

			InterfacePtr<IPMStream> storyStream(StreamUtil::CreateFileStreamWrite(incopyFile, kOpenOut | kOpenTrunc, fti, creator));
			if (!storyStream || Utils<ISnippetExport>()->ExportInCopyInterchange(storyStream, itemListToExport) != kSuccess) {
				CAlert::InformationAlert(kXPGErrFailedToCreateInCopyFiles);
				break;
			}
			storyStream->Close();

			// Create a link for an InCopy story. 
			UID linkUID(kInvalidUID);
			Utils<IStoryUtils>()->CreateLink(itemRef, incopyFile, kInCopyImportProviderBoss, kInCopyXMLMarkupFileTypeInfoID, linkUID);

			//----- Mark the link as up to date
			InterfacePtr<ITextModel> model(itemRef, IID_ITEXTMODEL);
			if (model) model->SetChanged(kFalse);

			if (inCopyMgr->AddToAssignmentCmd(assignment, itemListToExport) != kSuccess) {
				CAlert::ErrorAlert(kXPGErrFailedToCreateInCopyFiles);
				break;
			}
		}

		// 16.04.2013 ++
		// Correction bug export XML qui dupliquait le texte si les blocs etaient chaines
		textStories = createdStories;
		// 16.04.2013 --

		if (updateAssignment)
		{
			// Update assignment
			if (inCopyMgr->SaveAssignment(assignment) != kSuccess) {
				break;
			}
			inCopyMgr->NotifyPanel();
		}

		status = kSuccess;

	} while (kFalse);

	return status;
}


void XPageUtils::HandleItemGroup(UIDList& storyList, UIDRef itemRef) {

	InterfacePtr<IHierarchy> itemHier(itemRef, UseDefaultIID());
	if (Utils<IPageItemTypeUtils>()->IsGroup(itemHier))
	{
		// Go through each child of the group calling this method recursivly, still looking for text to export
		for (int32 i = 0; i < itemHier->GetChildCount(); ++i) {
			HandleItemGroup(storyList, UIDRef(itemRef.GetDataBase(), itemHier->GetChildUID(i)));
		}
	}
	else {
		storyList.Append(::GetUIDRef(itemHier));
	}
}


void XPageUtils::WriteLog(const PMString& msg)
{
	IDFile logFile;
	FileUtils::GetAppInstallationFolder(&logFile);
	FileUtils::AppendPath(&logFile, PMString("XPage.log", PMString::kUnknownEncoding));

	InterfacePtr<IPMStream> logfileStream(StreamUtil::CreateFileStreamWrite(logFile, kOpenOut | kOpenApp, kTextType, kAnyCreator));
	if (logfileStream == nil)
		return;

	PMString time = GetCurrentTime();
	time = "[" + time + "]";

	PMString newLine;
#ifdef MACINTOSH
	newLine += "\r";
#else
	newLine += "\r\n";
#endif

	logfileStream->XferByte((uchar *)time.GetPlatformString().c_str(), time.GetPlatformString().size());
	logfileStream->XferByte((uchar *)msg.GetPlatformString().c_str(), msg.GetPlatformString().size());
	logfileStream->XferByte((uchar *)newLine.GetPlatformString().c_str(), newLine.GetPlatformString().size());
	logfileStream->Flush();
	logfileStream->Close();
}

PMString  XPageUtils::GetCurrentTime() {

	IDTime current;
	current.CurrentTime();
	int32 year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
	current.GetTime(&year, &month, &day, &hour, &min, &sec);

	PMString time;

	time.AppendNumber(year);
	time += "/";
	if (month < 10) time += "0";
	time.AppendNumber(month);
	time += "/";
	if (day < 10) time += "0";
	time.AppendNumber(day);
	time += " ";
	if (hour < 10) time += "0";
	time.AppendNumber(hour);
	time += ":";
	if (min < 10) time += "0";
	time.AppendNumber(min);
	time += ":";
	if (sec < 10) time += "0";
	time.AppendNumber(sec);

	return time;
}

int32 XPageUtils::GetNbSignes(const UIDList& storyList, IDataBase * db)
{
	UIDList TextFrames(storyList);
	int32 nbSignes = 0;

	//On calcul le nombre de signes des blocs textes
	UIDList textStories(db);
	for (int32 l = 0; l < TextFrames.Length(); ++l)
	{
		InterfacePtr<IFrameType> frameType(db, TextFrames[l], UseDefaultIID());
		if (frameType && frameType->IsTextFrame()) {
			InterfacePtr<IHierarchy> itemHier(frameType, UseDefaultIID());
			InterfacePtr<IMultiColumnTextFrame> txtFrame(db, itemHier->GetChildUID(0), UseDefaultIID());
			if (textStories.Contains(txtFrame->GetTextModelUID()))
				// To avoid to create several stories for linked frames which actually consist in one story
				continue;
			else {
				UIDRef itemRef = TextFrames.GetRef(l);
				Utils<IAssignmentUtils>()->NormalizeTextModelRef(itemRef);
				InterfacePtr<ITextModel> textModel(itemRef, UseDefaultIID());
				InterfacePtr<ITextStoryThread> textStoryThread(textModel, UseDefaultIID());
				if (textStoryThread) {
					nbSignes += textStoryThread->GetTextSpan();
				}
				textStories.Append(txtFrame->GetTextModelUID());
			}

		}
	}
	return nbSignes;
}


void XPageUtils::LookForStoriesToExport(UIDList& storyList, UIDRef itemRef)
{
	InterfacePtr<IHierarchy> itemHier(itemRef, UseDefaultIID());
	if (Utils<IPageItemTypeUtils>()->IsGroup(itemHier))
	{
		// Go through each child of the group calling this method recursivly, still looking for text to export
		for (int32 i = 0; i < itemHier->GetChildCount(); ++i)
			LookForStoriesToExport(storyList, UIDRef(itemRef.GetDataBase(), itemHier->GetChildUID(i)));
	}
	else if (Utils<IPageItemTypeUtils>()->IsTextFrame(itemHier))
	{
		InterfacePtr<IMultiColumnTextFrame> txtFrame(itemRef.GetDataBase(), itemHier->GetChildUID(0), UseDefaultIID());
		storyList.Append(txtFrame->GetTextModelUID());
	}
}

void XPageUtils::AppendToContent(K2Vector<KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > >& content, const Tag& aTag, const K2Vector<RichText>& richTextContent)
{

	K2Pair<WideString, K2Vector<RichText> > textToExport(aTag.value, richTextContent);

	// Take order into account
	K2Vector<KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > >::iterator iter = ::K2find(content, aTag.order);
	if (iter == content.end())
	{
		// No content having this order has been found so far, add it to the current content (sorted in descending order)
		iter = content.begin();
		while (iter != content.end() && aTag.order < iter->Key())
			++iter;

		K2Vector<K2Pair<WideString, K2Vector<RichText> > > someContent;
		someContent.push_back(textToExport);
		KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > newText(aTag.order, someContent);

		if (iter == content.end()) content.push_back(newText);
		else content.insert(iter, newText);
	}
	else iter->Value().push_back(textToExport);

	// BUG FIX : avoid leaks
	textToExport.second.clear();
}

//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
// renvoie l'id xrail de la page active
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------
int32  XPageUtils::GetPageId(void) {

	int32 idPage = -1;
	do
	{
		IDocument * document = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (document == nil)
		{
			break;
		}

		IDataBase * db = ::GetDataBase(document);
		if (db == nil)
		{
			break;
		}

		InterfacePtr<ISpreadList> spreadList(document, UseDefaultIID());
		if (spreadList == nil)
		{
			break;
		}

		int32 spreadCount = spreadList->GetSpreadCount();
		UID ownerPageUID = kInvalidUID;
		int32 spreadIndex = 0;
		while (spreadIndex < spreadCount)
		{
			UIDRef spreadUIDRef(db, spreadList->GetNthSpreadUID(spreadIndex));
			InterfacePtr<ISpread> spread(spreadUIDRef, UseDefaultIID());
			ownerPageUID = spread->GetNthPageUID(spreadIndex);
			if (ownerPageUID != kInvalidUID)
			{
				InterfacePtr<IXRailPageSlugData> readData(db, ownerPageUID, IID_PAGESLUGDATA);
				if (readData && readData->GetID() > 0)
				{
					idPage = readData->GetID();
					spreadIndex = spreadCount;
				}
			}
			++spreadIndex;
		}
	} while (kFalse);
	return idPage;

}

int32 XPageUtils::GetPageID(UIDRef docUIDRef, int32 index)
{
	int32 returnID = 0;
	do
	{
		InterfacePtr<IPageList> pageList(docUIDRef, UseDefaultIID());

		UID pageUID = pageList->GetNthPageUID(index);
		if (pageUID == kInvalidUID) // Wrong page index
			break;

		InterfacePtr<IXRailPageSlugData> readdata(docUIDRef.GetDataBase(), pageUID, IID_PAGESLUGDATA);

		returnID = readdata->GetID();
	} while (false);

	return returnID;
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

/* LinkFramesCmd
*/
ErrorCode XPageUtils::LinkFramesCmd(UIDRef firstMultiCol, UIDRef secondMultiCol, bool8 allowUndo)
{
	ErrorCode status = kFalse;
	do
	{
		if (firstMultiCol.GetDataBase() == nil
			|| firstMultiCol.GetDataBase() != secondMultiCol.GetDataBase()
			|| firstMultiCol.GetUID() == kInvalidUID
			|| secondMultiCol.GetUID() == kInvalidUID)

			break;

		InterfacePtr<IMultiColumnTextFrame> testFirstIsTextFrame(firstMultiCol, UseDefaultIID());
		InterfacePtr<IMultiColumnTextFrame> testSecondIsTextFrame(secondMultiCol, UseDefaultIID());
		if (testFirstIsTextFrame == nil || testSecondIsTextFrame == nil) // not a text frame, cannot insert an inline
			break;

		InterfacePtr<ICommand> textLinkCmd(CmdUtils::CreateCommand(kTextLinkCmdBoss));
		if (textLinkCmd == nil)
			break;

		// Set command undoability
		if (!allowUndo)
			textLinkCmd->SetUndoability(ICommand::kAutoUndo);

		// Fill a list with the frames to link
		UIDList framesToLink(firstMultiCol);
		framesToLink.Append(secondMultiCol.GetUID());

		textLinkCmd->SetItemList(framesToLink);

		status = CmdUtils::ProcessCommand(textLinkCmd);

	} while (false);

	return status;
}

void XPageUtils::GetStatisticsText(UIDList itemsTexts, int32& nbCharcters, int32& nbParagraphs, int32& nbWords, int32& nbLines) {

	IAbortableCmdSeq* sequ = nil;
	IDataBase *db = itemsTexts.GetDataBase();
	UIDList frameListUID(db);

	do {
		sequ = CmdUtils::BeginAbortableCmdSeq();
		if (sequ == nil)
			break;

		for (int32 i = 0; i < itemsTexts.size(); ++i) {

			UIDRef storyRef = itemsTexts.GetRef(i);

			if (storyRef == UIDRef::gNull) {
				ASSERT_FAIL("invalid storyRef UIDRef!");
				continue;
			}


			Utils<IAssignmentUtils>()->NormalizeTextModelRef(storyRef);

			InterfacePtr<ITextModel> textModel(storyRef, UseDefaultIID());
			if (textModel == nil)
				continue;

			// Get frame list from text model
			InterfacePtr<IFrameList> frameList(textModel->QueryFrameList());
			if (frameList == nil)
				continue;

			// Store current frame list for next iteration
			if (frameListUID.DoesNotContain(::GetUID(frameList))) {
				frameListUID.Append(::GetUID(frameList));
			}
			else {
				continue;
			}

			// Fill with the placeholder text with command
			InterfacePtr<ICommand> fillFrameListCmd(CmdUtils::CreateCommand(kFillFrameListCmdBoss));
			fillFrameListCmd->SetUndoability(ICommand::kRegularUndo);
			InterfacePtr<IBoolData> fillFrameListData(fillFrameListCmd, UseDefaultIID());
			fillFrameListData->Set(kTrue);
			fillFrameListCmd->SetItemList(UIDList(frameList));
			CmdUtils::ProcessCommand(fillFrameListCmd);

			InterfacePtr<ITextStoryThread> textStoryThread(textModel, UseDefaultIID());
			if (textStoryThread) {
				nbCharcters += textStoryThread->GetTextSpan();
			}

			// Get number of paragraphs, lines and words
			InterfacePtr<ITextStrand> txtStrand(static_cast<ITextStrand*>(textModel->QueryStrand(kTextDataStrandBoss, IID_ITEXTSTRAND)));
			if (txtStrand == nil)
				break;

			InterfacePtr<IWaxStrand> waxStrand(((IWaxStrand*)textModel->QueryStrand(kFrameListBoss, IID_IWAXSTRAND)));
			if (waxStrand == nil)
				break;

			K2::scoped_ptr<IWaxIterator> waxIterator(waxStrand->NewWaxIterator());
			if (waxIterator == nil)
				break;

			int32 numberOfParagraphs = 0, numberOfLines = 0, numberOfWords = 0;

			IWaxLine* waxLine = waxIterator->GetFirstWaxLine(0);
			while (waxLine != nil && (waxLine->TextOrigin() < textModel->GetPrimaryStoryThreadSpan())) {
				waxLine = waxIterator->GetNextWaxLine();
				++numberOfLines;
			}

			TextIndex index = 0;
			while (index < textModel->GetPrimaryStoryThreadSpan()) {
				int32 length;
				UTF32TextChar prevChar = kTextChar_Null;
				DataWrapper<textchar> chunk = txtStrand->FindChunk(index, &length);
				int32 indexChunk = 0;
				while (indexChunk < length) {
					if (chunk[indexChunk] == kTextChar_Space)
						numberOfWords++;
					else if (chunk[indexChunk] == kTextChar_CR)
						numberOfParagraphs++;
					++indexChunk;
				}
				index += length;
			}
			nbParagraphs += numberOfParagraphs;
			nbLines += numberOfLines;
			nbWords += numberOfWords;

		}
		if (sequ != nil) {
			CmdUtils::AbortCommandSequence(sequ);
		}
	} while (kFalse);

}

ErrorCode XPageUtils::LoadStylesNamesFromSnippet(UIDRef documentUIDRef, const IDFile& cartonDir) {

	ErrorCode status = kFailure;

	do {
		InterfacePtr<IMasterSpreadList> masterSpreadList(documentUIDRef, UseDefaultIID());
		if (masterSpreadList == nil)
			break;

		int32 nbMasters = masterSpreadList->GetMasterSpreadCount();
		if (nbMasters == 0)
			break;

		IDataBase * db = ::GetDataBase(masterSpreadList);
		if (db == nil)
			break;

		UID masterUID = masterSpreadList->GetNthMasterSpreadUID(0);
		InterfacePtr<IMasterSpread> master(db, masterUID, UseDefaultIID());
		if (master == nil)
			break;

		InterfacePtr<ISpread> spread(master, UseDefaultIID());
		if (spread == nil)
			break;

		// Reverse xml
		SDKFileSaveChooser saveDialog;

		PMString defaultFileName("Export.xml");
		defaultFileName.SetTranslatable(kFalse);

		PMString saveDialogFilter("xml");
		saveDialogFilter.SetTranslatable(kFalse);

		PMString filterName("XML Files (*.xml)");
		filterName.SetTranslatable(kFalse);
		saveDialog.SetFilename(defaultFileName);

#ifdef WINDOWS
		saveDialog.AddFilter(0, 0, saveDialogFilter, filterName);
#else
		saveDialog.AddFilter(kAnyCreator, kTextType, saveDialogFilter, filterName);
#endif

		saveDialog.ShowDialog();
		if (saveDialog.IsChosen())
		{
			// Get chosen file
			IDFile exportFile = saveDialog.GetIDFile();
			InterfacePtr<IXMLOutStream> exportStream(Utils<IXMLStreamUtils>()->CreateXMLOutFileStream(exportFile, IXMLOutStream::kUTF8));
			if (exportStream == nil)
				break;

			exportStream->DocHeader(WideString("1.0"), kFalse);
			exportStream->Push(WideString("EXPORT_XML"), kTrue);

			IXMLOutStream::AttributeList attrs;

			// Filter avec l'extention du fichier
			PMString filter = "inds";

			// Lister récursivement des fichiers d'un dossierà partir d'un filtre
			K2Vector<IDFile> fileList;
			XPageUtils::ListPath(cartonDir, filter, fileList);

			// Besoin des preferences, pour recuperer le dossier des typos
			InterfacePtr<IXPGPreferences> xpgPrefs(GetExecutionContextSession(), UseDefaultIID());

			for (int32 i = 0; i < fileList.size(); ++i)
			{
				// Current snippet
				IDFile snipetFile = fileList[i];

				PMString snipetFileName = snipetFile.GetFileName();
				snipetFileName.Truncate(5);

				// Get matching xml file
				IDFile matchingFile;
				FileUtils::GetParentDirectory(snipetFile, matchingFile);
				FileUtils::GetParentDirectory(matchingFile, matchingFile);
				FileUtils::AppendPath(&matchingFile, "Typographie");
				FileUtils::AppendPath(&matchingFile, snipetFileName + ".xml");

				K2Vector<IDValuePair> matchingStyles;
				bool8 matchingFileExist = kFalse;

				// Parse XML and get list of styles and tags
				if (FileUtils::DoesFileExist(matchingFile))
				{
					matchingStyles = XPageUtils::ParseFileMatching(matchingFile);
					matchingFileExist = kTrue;
				}
				else {
					continue;
				}

				// Import snippet 
				UIDList formeItems(db);
				PMString error = kNullString;
				if (XPageUtils::ImportForme(documentUIDRef, snipetFile, PMPoint(0, 0), ::GetUIDRef(spread), IDFile(), error, &formeItems) != kSuccess)
				{
					continue;
				}

				InterfacePtr<IStoryList> storiesList(documentUIDRef, UseDefaultIID());
				UIDList styles(documentUIDRef.GetDataBase());
				K2Vector<PMString> stylesNames;

				for (int32 j = 0; j < storiesList->GetAllTextModelCount(); j++)
				{
					InterfacePtr<ITextModel> txtModel(storiesList->GetNthTextModelUID(j), UseDefaultIID());
					if (!txtModel)
						continue;

					InterfacePtr<IAttributeStrand> paraStrand((IAttributeStrand*)txtModel->QueryStrand(kParaAttrStrandBoss, IID_IATTRIBUTESTRAND));
					if (!paraStrand)
						continue;

					TextIndex pos = 0;
					int32 totalLength = txtModel->TotalLength() - 1;

					while (pos < totalLength)
					{
						int32 count = 0;
						UID paraStyle = paraStrand->GetStyleUID(pos, &count);
						if (styles.DoesNotContain(paraStyle))
						{
							styles.Append(paraStyle);

							InterfacePtr<IStyleInfo> styleInfo(documentUIDRef.GetDataBase(), paraStyle, UseDefaultIID());
							if (styleInfo)
							{
								PMString styleInfoName = styleInfo->GetName();
								if (::K2find(stylesNames, styleInfoName) == stylesNames.end())
								{
									stylesNames.push_back(styleInfoName);
								}
							}
						}
						pos += count;
					}
				}

				WideString defaulValue("TX");
				WideString orderValue("");

				K2Vector<Tag> tagList = xpgPrefs->GetListeMatchingTag();

				for (int32 j = 0; j < stylesNames.size(); j++)
				{
					PMString styleName = stylesNames[j];
					int32 posTagValue = ::FindLocationByValue(matchingStyles, styleName);
					if (posTagValue != -1)
					{
						defaulValue = WideString(matchingStyles[posTagValue].Key());
					}

					for (int32 k = 0; k < tagList.size(); k++)
					{
						Tag aTag = tagList[k];
						if (aTag.value.compare(defaulValue) == 0)
						{
							Utils<IDataStringUtils>()->Int32ToString(aTag.order, &orderValue);
							break;
						}
					}

					attrs.push_back(IXMLOutStream::Attribute(WideString("style"), WideString(stylesNames[j])));
					attrs.push_back(IXMLOutStream::Attribute(WideString("value"), defaulValue));
					attrs.push_back(IXMLOutStream::Attribute(WideString("order"), orderValue));

					exportStream->PushWithAttributes(WideString("TAG"), attrs, kFalse);

					exportStream->Pop(kTrue);
					attrs.clear();
				}

				// Delete items from document
				InterfacePtr<ICommand> deletePageItemCmd(CmdUtils::CreateCommand(kDeleteCmdBoss));
				deletePageItemCmd->SetItemList(formeItems);
				if (CmdUtils::ProcessCommand(deletePageItemCmd) != kSuccess)
				{
					continue;
				}
			}

			exportStream->Pop(kTrue);
			exportStream->Flush();
			exportStream->Close();
			exportStream.reset();

			// Close the current document
			InterfacePtr<IDocFileHandler> docFileHandler(Utils<IDocumentUtils>()->QueryDocFileHandler(documentUIDRef));
			if (docFileHandler && docFileHandler->CanClose(documentUIDRef))
			{
				docFileHandler->Close(documentUIDRef);
			}

			// Export xml is good
			PMString exportOKMsg(kXPGUIExportXMLOKKey);
			exportOKMsg.Translate();
			CAlert::InformationAlert(exportOKMsg);
		}
	} while (kFalse);
	return status;
}

UIDRef XPageUtils::CreateDocument(UIFlags uiflags, const PMReal& width, const PMReal& height, const	int32 numPages, const int32 numPagesPerSpread) {

	UIDRef result = UIDRef::gNull;
	do {
		// Create the command.
		InterfacePtr<ICommand> newDocCmd(Utils<IDocumentCommands>()->CreateNewCommand(K2::kFullUI));
		ASSERT(newDocCmd);
		if (newDocCmd == nil) {
			break;
		}

		// Set the command's parameterised data.
		InterfacePtr<INewDocCmdData> newDocCmdData(newDocCmd, UseDefaultIID());
		ASSERT(newDocCmdData);
		if (newDocCmdData == nil) {
			break;
		}
		newDocCmdData->SetCreateBasicDocument(kFalse); // Override the following defaults.
		PMRect pageSize = PMRect(0.0, 0.0, width, height);
		newDocCmdData->SetNewDocumentPageSize(PMPageSize(pageSize));
		bool16 bWide = kTrue; // landscape orientation.
		if (width < height) {
			bWide = kFalse;
		}

		newDocCmdData->SetWideOrientation(bWide);

		// Size margin proportional to document width and height.
		PMReal horMargin = width / 20;
		PMReal verMargin = height / 20;
		newDocCmdData->SetMargins(horMargin, verMargin, horMargin, verMargin);
		newDocCmdData->SetNumPages(numPages);
		newDocCmdData->SetPagesPerSpread(numPagesPerSpread);

		// Create the new document.
		if (CmdUtils::ProcessCommand(newDocCmd) != kSuccess) {
			break;
		}

		// Pass the UIDRef of the new document back to the caller.
		const UIDList& newDocCmdUIDList = newDocCmd->GetItemListReference();
		result = newDocCmdUIDList.GetRef(0);
		if (result == UIDRef::gNull) {
			ASSERT_FAIL("newDocCmd returned invalid document UIDRef!");
			break;
		}

		// Open a layout view on the document. 
		// Create the command.
		InterfacePtr<ICommand> cmd(CmdUtils::CreateCommand(kOpenLayoutCmdBoss));
		if (cmd == nil)
			break;

		// Pass the command the UIDRef of the document.
		cmd->SetItemList(UIDList(result));

		// You could override the command's default options if necessary...
		InterfacePtr<IOpenLayoutPresentationCmdData> winData(cmd, IID_IOPENLAYOUTCMDDATA);
		if (winData == nil)
			break;

		// Open a window on the document.
		if (CmdUtils::ProcessCommand(cmd) != kSuccess)
			break;

		// Validate the the newly opened window is returned.
		InterfacePtr<IWindow> window(winData->GetResultingPresentation(), UseDefaultIID());
		if (window == nil)
			break;

	} while (false);
	return result;
}

/**
	@desc Liste rÈcursive des fichiers d'un dossier dans une liste
*/
void XPageUtils::ListPath(const IDFile path, const PMString & filter, K2Vector<IDFile> & fileList) {


	PlatformFolderTraverser folderTraverser(path, kTrue, kTrue, kFalse, kTrue);
	IDFile idFile;
	while (folderTraverser.Next(&idFile)) {
		if (FileUtils::IsDirectory(idFile)) {
			PMString truncP = idFile.GetFileName();
			if (XPageUtils::validPath(truncP))
				ListPath(idFile, filter, fileList);
		}
		else {
			PMString ext;
			FileUtils::GetExtension(idFile, ext);
			if (ext.IsEqual(filter))
				fileList.push_back(idFile);
		}
	}
}
/**
*/
bool16 XPageUtils::validPath(const PMString & p) {
	const PMString thisDir(".");
	const PMString parentDir("..");
	return p != thisDir && p != parentDir;
}

K2Vector<IDValuePair> XPageUtils::ParseFileMatching(const IDFile& matchingFile) {

	// Parse matching file to get mapping styles
	InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss));

	InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID());
	InterfacePtr<ISAXContentHandler> contentHandler(::CreateObject2<ISAXContentHandler>(kXPGXMLMatchingFileHandler2Boss, ISAXContentHandler::kDefaultIID));

	contentHandler->Register(saxServices);
	InterfacePtr<ISAXEntityResolver> entityResolver(::CreateObject2<ISAXEntityResolver>(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
	InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());

	// These commitments: follow 
	// the existing ImportXMLFileCmd

	parserOptions->SetNamespacesFeature(kFalse);
	parserOptions->SetShowWarningAlert(kFalse);

	InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(matchingFile));
	saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);
	xmlFileStream->Close();
	xmlFileStream.reset(nil);

	InterfacePtr<IKeyValueData> formeMatchingStyleData(contentHandler, IID_IFORMEMATCHINGSTYLESLIST);
	return formeMatchingStyleData->GetKeyValueList();
}

std::map<PMString, UIDList> XPageUtils::GetFormeDataList(IDocument* theDoc, bool16 includeMasters) {

	std::map<PMString, UIDList> formeDataList;
	do {
		if (theDoc == nil)
			break;

		IDataBase * db = ::GetDataBase(theDoc);
		if (db == nil)
			break;

		InterfacePtr<ILayerList> layerList(theDoc, UseDefaultIID());
		InterfacePtr<IGuidePrefs> guidePrefs(theDoc->GetDocWorkSpace(), UseDefaultIID());
		int32 nbLayers = layerList->GetCount();
		int32 startContentLayer, endContentLayer;

		if (guidePrefs && guidePrefs->GetGuidesInBack()) {
			startContentLayer = nbLayers + 1;
			endContentLayer = (2 * nbLayers) - 1;
		}
		else {
			startContentLayer = 2;
			endContentLayer = nbLayers;
		}

		InterfacePtr<ISpreadList> spreadList(theDoc, UseDefaultIID());
		int32 nbSpreads = spreadList->GetSpreadCount();
		for (int32 i = 0; i < nbSpreads; ++i) {

			InterfacePtr<ISpread> spread(db, spreadList->GetNthSpreadUID(i), UseDefaultIID());
			CollectFormeDataOnSpread(spread, startContentLayer, endContentLayer, formeDataList);
		}

		if (includeMasters)
		{
			InterfacePtr<IMasterSpreadList> masterSpreadList(theDoc, UseDefaultIID());
			int32 nbMasterSpreads = masterSpreadList->GetMasterSpreadCount();
			for (int32 i = 0; i < nbMasterSpreads; ++i) {

				InterfacePtr<ISpread> spread(db, masterSpreadList->GetNthMasterSpreadUID(i), UseDefaultIID());
				CollectFormeDataOnSpread(spread, startContentLayer, endContentLayer, formeDataList);
			}
		}

	} while (kFalse);
	return formeDataList;
}

void XPageUtils::CollectFormeDataOnSpread(ISpread* spread, int32 startContentLayer, int32 endContentLayer, std::map<PMString, UIDList>& formeDataList)
{
	do
	{
		InterfacePtr<IHierarchy> spreadHier(spread, UseDefaultIID());
		if (!spread)
			break;

		IDataBase* db = ::GetDataBase(spread);
		for (int32 j = startContentLayer; j <= endContentLayer; ++j) {
			InterfacePtr<IHierarchy> spreadLayerHier(db, spreadHier->GetChildUID(j), UseDefaultIID());
			if (spreadLayerHier == nil)
				continue;

			int32 nbItems = spreadLayerHier->GetChildCount();
			for (int32 itemIndex = 0; itemIndex < nbItems; ++itemIndex) {
				InterfacePtr<IHierarchy> itemHier(db, spreadLayerHier->GetChildUID(itemIndex), UseDefaultIID());
				if (itemHier) {
					XPageUtils::HandleFormeData(itemHier, formeDataList);
				}
			}
		}
	} while (kFalse);
}

void XPageUtils::HandleFormeData(IHierarchy * itemHier, std::map<PMString, UIDList>& formeDataList) {

	if (Utils<IPageItemTypeUtils>()->IsGroup(itemHier)) {
		// If it's a group, handle children item recursively
		for (int32 i = 0; i < itemHier->GetChildCount(); ++i) {
			InterfacePtr<IHierarchy> childHier(itemHier->QueryChild(i));
			HandleFormeData(childHier, formeDataList);
		}
	}
	else {
		UIDRef itemRef = ::GetUIDRef(itemHier);
		InterfacePtr<IFormeData> formaData(itemRef, UseDefaultIID());
		if (formaData) {
			PMString formeUniqueName = formaData->GetUniqueName();

			if (!formeUniqueName.IsEmpty()) {

				// Add item to the group of items it belongs to
				std::map<PMString, UIDList>::iterator iter = formeDataList.find(formeUniqueName);
				if (iter == formeDataList.end()) {
					formeDataList.insert(std::make_pair(formeUniqueName, UIDList(itemRef)));
				}
				else if (iter != formeDataList.end()) {
					iter->second.Append(itemRef);
				}
			}
		}
	}
}

ErrorCode XPageUtils::GetCartonItemsFromTarget(UIDRef itemRef, UIDList& formeItems) {

	ErrorCode error = kFailure;
	do {
		InterfacePtr<IFormeData> formaData(itemRef, UseDefaultIID());
		if (formaData == nil)
			break;

		PMString uniqueName = formaData->GetUniqueName();
		if (uniqueName == kNullString)
			break;

		IDataBase* db = itemRef.GetDataBase();
		InterfacePtr<IDocument> doc(db, db->GetRootUID(), UseDefaultIID());

		std::map<PMString, UIDList> formeDataList = XPageUtils::GetFormeDataList(doc);
		std::map<PMString, UIDList>::iterator iter = formeDataList.find(uniqueName);

		if (iter != formeDataList.end())
			formeItems.Append(iter->second);
		error = kSuccess;
	} while (kFalse);
	return error;
}


void XPageUtils::HandlePlacedItem(IHierarchy * itemHier, K2Vector<KeyValuePair<PMString, K2Pair<PMString, UIDList> > >& exportableStories) {

	if (Utils<IPageItemTypeUtils>()->IsGroup(itemHier)) {
		// If it's a group, handle children item recursively
		for (int32 i = 0; i < itemHier->GetChildCount(); ++i) {
			InterfacePtr<IHierarchy> childHier(itemHier->QueryChild(i));
			HandlePlacedItem(childHier, exportableStories);
		}
	}
	else {
		// Check out item if is an InCopy story
		UIDRef itemRef = ::GetUIDRef(itemHier);
		UIDRef storyRef = itemRef;
		InterfacePtr<IPlacedArticleData> placedArticleData(itemRef, UseDefaultIID());

		if (placedArticleData) {
			PMString idArt = placedArticleData->GetUniqueId();
			PMString pathArt = placedArticleData->GetStoryFolder();
			if (!idArt.IsEmpty() && !pathArt.IsEmpty()) {

				// Add item to the group of items it belongs to
				K2Vector<KeyValuePair<PMString, K2Pair <PMString, UIDList> > >::iterator iter = ::K2find(exportableStories, idArt);
				if (iter == exportableStories.end())
					exportableStories.push_back(KeyValuePair <PMString, K2Pair <PMString, UIDList> >(idArt, K2Pair<PMString, UIDList>(pathArt, UIDList(itemRef))));
				else if (iter != exportableStories.end())
					iter->Value().second.Append(itemRef);
			}
		}
	}
}

K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > XPageUtils::GetPlacedStoriesList(IDocument* theDoc) {

	K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > exportableItems;
	do {

		if (theDoc == nil)
			break;

		IDataBase * db = ::GetDataBase(theDoc);
		if (db == nil)
			break;

		InterfacePtr<ILayerList> layerList(theDoc, UseDefaultIID());
		InterfacePtr<IGuidePrefs> guidePrefs(theDoc->GetDocWorkSpace(), UseDefaultIID());
		int32 nbLayers = layerList->GetCount();
		int32 startContentLayer, endContentLayer;

		if (guidePrefs && guidePrefs->GetGuidesInBack()) {
			startContentLayer = nbLayers + 1;
			endContentLayer = (2 * nbLayers) - 1;
		}
		else {
			startContentLayer = 2;
			endContentLayer = nbLayers;
		}

		InterfacePtr<ISpreadList> spreadList(theDoc, UseDefaultIID());
		int32 nbSpreads = spreadList->GetSpreadCount();
		for (int32 i = 0; i < nbSpreads; ++i) {

			InterfacePtr<IHierarchy> spreadHier(db, spreadList->GetNthSpreadUID(i), UseDefaultIID());
			if (spreadHier == nil)
				continue;

			for (int32 j = startContentLayer; j <= endContentLayer; ++j) {
				InterfacePtr<IHierarchy> spreadLayerHier(db, spreadHier->GetChildUID(j), UseDefaultIID());
				if (spreadLayerHier == nil)
					continue;

				int32 nbItems = spreadLayerHier->GetChildCount();
				for (int32 itemIndex = 0; itemIndex < nbItems; ++itemIndex) {
					InterfacePtr<IHierarchy> itemHier(db, spreadLayerHier->GetChildUID(itemIndex), UseDefaultIID());
					if (itemHier) {
						XPageUtils::HandlePlacedItem(itemHier, exportableItems);
					}
				}
			}
		}

	} while (kFalse);
	return exportableItems;

}

ErrorCode XPageUtils::GetPlacedArtItemsFromTarget(UIDRef itemRef, UIDList& placedArticleItems) {

	ErrorCode error = kFailure;
	do {
		InterfacePtr<IPlacedArticleData> placedArticleData(itemRef, UseDefaultIID());
		if (placedArticleData == nil)
			break;

		PMString uniqueName = placedArticleData->GetUniqueId();
		if (uniqueName == kNullString)
			break;

		IDataBase* db = itemRef.GetDataBase();
		InterfacePtr<IDocument> theDoc(db, db->GetRootUID(), UseDefaultIID());

		K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > placedArticleList = XPageUtils::GetPlacedStoriesList(theDoc);
		for (int32 i = 0; i < placedArticleList.size(); i++) {
			if (placedArticleList[i].Key().IsEqual(uniqueName)) {
				placedArticleItems.Append(placedArticleList[i].Value().second);
				error = kSuccess;
				break;
			}
		}

	} while (kFalse);
	return error;
}


int32 XPageUtils::GetPageNumber(const UIDRef &textFrameRef) {

	if (textFrameRef == UIDRef::gNull)
		return -1;

	return Utils<ITextUtils>()->GetPageNumber(textFrameRef);

}

ErrorCode XPageUtils::GetAllFormeItemsOnSpread(const PMString& idForme, ISpread* spread, UIDList& formeItems)
{
	ErrorCode status = kFailure;

	do
	{
		if (!spread)
			break;

		IDataBase * db = ::GetDataBase(spread);
		if (db == nil)
			break;

		InterfacePtr<IDocument> theDoc(db, db->GetRootUID(), UseDefaultIID());
		InterfacePtr<ILayerList> layerList(theDoc, UseDefaultIID());
		InterfacePtr<IGuidePrefs> guidePrefs(theDoc->GetDocWorkSpace(), UseDefaultIID());
		int32 nbLayers = layerList->GetCount();
		int32 startContentLayer, endContentLayer;

		if (guidePrefs && guidePrefs->GetGuidesInBack()) {
			startContentLayer = nbLayers + 1;
			endContentLayer = (2 * nbLayers) - 1;
		}
		else {
			startContentLayer = 2;
			endContentLayer = nbLayers;
		}

		// Go through each item on the spread to collect all items within their formes
		std::map<PMString, UIDList> formeDataList;
		CollectFormeDataOnSpread(spread, startContentLayer, endContentLayer, formeDataList);

		// Get the concerned forme items
		std::map<PMString, UIDList>::iterator iter = formeDataList.find(idForme);
		if (iter != formeDataList.end()) {
			formeItems = iter->second;
		}

		status = kSuccess;

	} while (kFalse);

	return status;
}

void XPageUtils::CategorizeItems(const UIDList& items, UIDList& textFrames, UIDList& graphicFrames)
{
	if (textFrames.IsEmpty() && textFrames.GetDataBase() == nil)
		textFrames = UIDList(items.GetDataBase());

	if (graphicFrames.IsEmpty() && graphicFrames.GetDataBase() == nil)
		graphicFrames = UIDList(items.GetDataBase());

	// Look for text and graphic frames within selection
	for (int32 i = 0; i < items.Length(); ++i) {

		UIDRef anItem = items.GetRef(i);
		if (Utils<IPageItemTypeUtils>()->IsGroup(anItem))
		{
			InterfacePtr<IHierarchy> itemHier(anItem, UseDefaultIID());
			UIDList groupItems(items.GetDataBase());

			for (int32 j = 0; j < itemHier->GetChildCount(); ++j)
				groupItems.Append(itemHier->GetChildUID(j));

			CategorizeItems(groupItems, textFrames, graphicFrames);
		}
		else {

			InterfacePtr<IFrameType> frameType(anItem, UseDefaultIID());
			if (!frameType) // Will be nil for guide items, strip them all
				continue;

			if (frameType->IsTextFrame())
				textFrames.Append(items.At(i));
			else if (frameType->IsGraphicFrame()) //ajout pour que les filets ne soient plus consideres comme des images
				graphicFrames.Append(items.At(i));
		}
	}
}

void XPageUtils::SplitFormeDescription(const PMString& uniqueName, PMString& classeurName, PMString& formeName)
{
	classeurName = "";
	formeName = "";

	if (uniqueName != "") {
		int32 index = uniqueName.IndexOfString("_", 0);
		if (index > 0) {
			formeName = uniqueName;
			formeName.Remove(0, index + 1);
			int32 index2 = formeName.LastIndexOfString("_");
			if (index2 > 0) {
				K2::scoped_ptr<PMString> temp(formeName.Substring(0, index2));
				formeName = PMString(*temp);
			}
			PMString sousChaine;
			K2::scoped_ptr<PMString> unique(uniqueName.Substring(0, index));
			sousChaine = PMString(*unique);

			K2::scoped_ptr<PMString> classeur(sousChaine.GetItem("_", 1));
			if (classeur) {
				classeurName = PMString(*classeur);
			}
		}
	}

	/*PMString sousChaine;
		int32 index = uniqueName.IndexOfString("_", 0);
		if (index > 0) {
			K2::scoped_ptr<PMString> unique(uniqueName.Substring(0, index));
	sousChaine = PMString(*unique);
	K2::scoped_ptr<PMString> classeur (sousChaine.GetItem ("_", 1));
			if(classeur) {
		classeurName = PMString(*classeur);
			}
			formeName = PMString(*unique);
			formeName.Remove(0, index);
			//PMString(*sousChaine.Substring(classeurName.CharCount()+1, sousChaine.CharCount()-classeurName.CharCount()));
	}
	}*/

	/*K2::scoped_ptr<PMString> classeur (uniqueName.GetItem ("_", 1));
	if(classeur)
		classeurName = PMString(*classeur);
	else
		classeurName = "";

	K2::scoped_ptr<PMString> forme (uniqueName.GetItem ("_20", 1));
	if(forme)
		formeName = PMString(*forme);
	else
		formeName = "";
	CAlert::InformationAlert(uniqueName);
	CAlert::InformationAlert(classeurName);
	CAlert::InformationAlert(formeName);
	*/
}

PMString XPageUtils::GetStoryIDFromAssignmentFile(const IDFile& assignmentFile)
{
	PMString storyID = kNullString;

	do
	{
		// First, check it's an assignment file
		PMString ext;
		FileUtils::GetExtension(assignmentFile, ext);

		if (!Utils<IAssignmentUtils>()->IsAssignmentFileExtension(ext))
			break;

		storyID = assignmentFile.GetFileName();
		int32 extPos = storyID.LastIndexOfCharacter(PlatformChar('.'));
		if (extPos != -1)
			storyID.Remove(extPos, storyID.NumUTF16TextChars() - extPos);

		// BUG FIX : assignment files are duplicated whenever a document is opened only if it contains missing plugins
		int32 hyphenPos = storyID.LastIndexOfCharacter(PlatformChar('-'));
		if (hyphenPos != -1)
			storyID.Remove(hyphenPos, storyID.NumUTF16TextChars() - hyphenPos);

	} while (kFalse);

	return storyID;
}

#ifdef WINDOWS
bool16 XPageUtils::DeleteDirectory(const TCHAR* sPath)
{
	HANDLE hFind;    // file handle
	WIN32_FIND_DATA FindFileData;

	TCHAR DirPath[MAX_PATH];
	TCHAR FileName[MAX_PATH];

	_tcscpy(DirPath, sPath);
	_tcscat(DirPath, _T("\\*"));    // searching all files
	_tcscpy(FileName, sPath);
	_tcscat(FileName, _T("\\"));

	// find the first file
	hFind = FindFirstFile(DirPath, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) return FALSE;
	_tcscpy(DirPath, FileName);

	bool bSearch = true;
	while (bSearch) {    // until we find an entry
		if (FindNextFile(hFind, &FindFileData)) {
			if (_tcscmp(FindFileData.cFileName, _T(".")) == 0 || _tcscmp(FindFileData.cFileName, _T("..")) == 0) continue;
			_tcscat(FileName, FindFileData.cFileName);
			if ((FindFileData.dwFileAttributes &
				FILE_ATTRIBUTE_DIRECTORY)) {

				// we have found a directory, recurse
				if (!DeleteDirectory(FileName)) {
					FindClose(hFind);
					return FALSE;    // directory couldn't be deleted
				}
				// remove the empty directory
				RemoveDirectory(FileName);
				_tcscpy(FileName, DirPath);
			}
			else {
				if (FindFileData.dwFileAttributes &
					FILE_ATTRIBUTE_READONLY)
					// change read-only file mode
					  // _tchmod(FileName, _S_IWRITE);
					if (!DeleteFile(FileName)) {    // delete the file
						FindClose(hFind);
						return FALSE;
					}
				_tcscpy(FileName, DirPath);
			}
		}
		else {
			// no more files there
			if (GetLastError() == ERROR_NO_MORE_FILES)
				bSearch = false;
			else {
				// some error occurred; close the handle and return FALSE
				FindClose(hFind);
				return FALSE;
			}
		}
	}
	FindClose(hFind);                  // close the file handle

	return RemoveDirectory(sPath);     // remove the empty directory
}
#else
bool16 XPageUtils::DeleteDirectory(const char* sPath)
{
	return 0;
}
#endif


void XPageUtils::GotoStoryUI(const PMString& idStory)
{
	bool16 found = kFalse;
	do
	{
		IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (frontDoc == nil)
		{
			break;
		}

		IDataBase * db = ::GetDataBase(frontDoc);
		if (db == nil)
		{
			break;
		}

		InterfacePtr<IAssignedDocument> assignedDoc(frontDoc, UseDefaultIID());
		if (assignedDoc == nil)
		{
			break;
		}

		UIDList assignmentList = assignedDoc->GetAssignments();
		for (int32 i = 0; i < assignmentList.Length(); ++i)
		{
			InterfacePtr<IAssignment> assignment(assignmentList.GetRef(i), UseDefaultIID());
			if (assignment)
			{
				IDFile assignmentFile = FileUtils::PMStringToSysFile(assignment->GetFile());
				if (!FileUtils::DoesFileExist(assignmentFile))
				{
					continue;
				}

				PMString assignmentID = assignmentFile.GetFileName();
				assignmentID.Truncate(5); // Remove icma extension	
				if (assignmentID.IsEmpty())
				{
					continue;
				}

				if (assignmentID.Compare(kFalse, idStory) == 0)
				{
					const UIDList storyList = assignment->GetStories();
					if (storyList.size() > 0)
					{
						UIDRef firstItemRef = storyList.GetRef(0);
						InterfacePtr<IAssignedStory> assignedStory(firstItemRef, UseDefaultIID());
						InterfacePtr<ILinkObject> linkObject(assignedStory->GetUIDRef(), UseDefaultIID());
						if (linkObject && linkObject->CanGotoLinkedObject())
						{
							linkObject->GotoLinkedObject(nil);
							found = kTrue;
						}
					}
					break;
				}
			}
		}
		//pas d'affectation existante, on cherche dans les articles placés à la main
		if (found == kFalse) {
			K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > placedStories = Utils<IXPageUtils>()->GetPlacedStoriesList(frontDoc);
			for (int32 j = 0; j < placedStories.size(); j++) {
				PMString artID = placedStories[j].Key();
				if (idStory == artID) {
					UIDList storyList = placedStories[j].Value().second;
					if (storyList.size() > 0)
					{
						InterfacePtr<ISelectionManager>	selectionManager(Utils<ISelectionUtils>()->QueryActiveSelection());
						InterfacePtr<ILayoutSelectionSuite> layoutSelectionSuite(selectionManager, UseDefaultIID());

						if (!layoutSelectionSuite)
						{
							ASSERT_FAIL("SelectPageItem -> select nil");
							break;
						}
						selectionManager->DeselectAll(nil);
						layoutSelectionSuite->SelectPageItems(storyList, Selection::kReplace, Selection::kAlwaysCenterInView);
					}
					break;
				}
			}
		}
	}

	while (kFalse);
}


bool16 XPageUtils::DoesStoryExistsOnPage(const PMString& idStory)
{
	bool16 found = kFalse;
	do
	{
		//On cherche si l'id de l'article existe bien dans la page
		//premier passage, on cherche une affectation
		IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (frontDoc == nil)
		{
			break;
		}

		IDataBase * db = ::GetDataBase(frontDoc);
		if (db == nil)
		{
			break;
		}

		InterfacePtr<IAssignedDocument> assignedDoc(frontDoc, UseDefaultIID());
		if (assignedDoc == nil)
		{
			break;
		}

		UIDList assignmentList = assignedDoc->GetAssignments();
		for (int32 i = 0; i < assignmentList.Length(); ++i)
		{
			InterfacePtr<IAssignment> assignment(assignmentList.GetRef(i), UseDefaultIID());
			if (assignment)
			{
				IDFile assignmentFile = FileUtils::PMStringToSysFile(assignment->GetFile());
				if (!FileUtils::DoesFileExist(assignmentFile))
				{
					continue;
				}

				PMString assignmentID = assignmentFile.GetFileName();
				assignmentID.Truncate(5); // Remove icma extension	
				if (assignmentID.IsEmpty())
				{
					continue;
				}
				if (assignmentID.Compare(kFalse, idStory) == 0)
				{
					found = kTrue;
					break;
				}
			}
		}
		//pas d'affectation existante, on cherche dans les articles placés à la main
		if (found == kFalse) {
			K2Vector<KeyValuePair <PMString, K2Pair <PMString, UIDList> > > placedStories = Utils<IXPageUtils>()->GetPlacedStoriesList(frontDoc);
			for (int32 i = 0; i < placedStories.size(); i++) {
				PMString artID = placedStories[i].Key();
				if (idStory == artID) {
					found = kTrue;
					break;
				}
			}
		}
	} while (kFalse);
	return found;
}

UID XPageUtils::TriasFindStyleByName(IStyleGroupHierarchy *stylesHierarchy, PMString searchStyle)
{
	UID styleFound = kInvalidUID;

	do
	{
		int32 nbChild = stylesHierarchy->GetChildCount();
		for (int32 i = 0; i < nbChild; i++) {
			IStyleGroupHierarchy *curStylesHierarchy = stylesHierarchy->QueryChild(i);
			if (curStylesHierarchy->GetHierarchyType() == IStyleGroupHierarchy::kHierarchyTypeStyleGroup) {
				styleFound = this->TriasFindStyleByName(curStylesHierarchy, searchStyle);
				if (styleFound != kInvalidUID) {
					break;
				}
			}
			else if (curStylesHierarchy->GetHierarchyType() == IStyleGroupHierarchy::kHierarchyTypeStyle) {
				if (curStylesHierarchy->GetName() == searchStyle) {
					styleFound = stylesHierarchy->GetChildUID(i);
					break;
				}
			}
		}

	} while (false);

	return styleFound;
}

ErrorCode XPageUtils::ExportToXml(const UIDList& storyList, IDFile xmlFolder, PMString artID) {

	ErrorCode status = kFailure;
	InterfacePtr<IXPGPreferences> xpgPrefs(GetExecutionContextSession(), UseDefaultIID());

	if (xpgPrefs->GetExportXML() == 1) {
		do {
			IDataBase* db = storyList.GetDataBase();
			UIDList currentStoryList(storyList);

			// Create export file
			IDFile xmlFile = xmlFolder;
			FileUtils::AppendPath(&xmlFile, artID + ".OBJRART.xml");
			InterfacePtr<IXMLOutStream> artStream(Utils<IXMLStreamUtils>()->CreateXMLOutFileStream(xmlFile, IXMLOutStream::kUTF8));
			if (artStream == nil)
				break;

			artStream->DocHeader(WideString("1.0"), kFalse);

			IXMLOutStream::AttributeList attrs;

			artStream->Push(WideString("Contenu"), kTrue);

			//on boucle sur chaque bloc
			K2Vector<KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > > content;
			WideString temp;

			for (int32 i = 0; i < currentStoryList.Length(); ++i) {
				UIDRef storyRef = currentStoryList.GetRef(i);

				// Read story
				InterfacePtr<ITextModel> txtModel(storyRef, UseDefaultIID());
				if (txtModel == nil) {
					continue;
				}
				InterfacePtr<IFormeData> itemFormeData(storyRef, UseDefaultIID());
				if (itemFormeData != nil) {
					if ((itemFormeData->GetType() == IFormeData::kCredit) || (itemFormeData->GetType() == IFormeData::kLegend)) {
						continue;
					}
				}

				InterfacePtr<ITextStrand> txtStrand(static_cast<ITextStrand*>(txtModel->QueryStrand(kTextDataStrandBoss, IID_ITEXTSTRAND)));
				InterfacePtr<IAttributeStrand> paraStrand(static_cast<IAttributeStrand*>(txtModel->QueryStrand(kParaAttrStrandBoss, IID_IATTRIBUTESTRAND)));
				InterfacePtr<IAttributeStrand> charStrand(static_cast<IAttributeStrand*>(txtModel->QueryStrand(kCharAttrStrandBoss, IID_IATTRIBUTESTRAND)));
				InterfacePtr<IItemStrand> itemStrand(static_cast<IItemStrand*>(txtModel->QueryStrand(kOwnedItemStrandBoss, IID_IITEMSTRAND)));

				TextIndex index = 0;
				UID currentParaStyle = kInvalidUID, currentCharStyle = kInvalidUID;
				Tag currentTag;
				K2Vector<RichText> richTextContent;
				PMString charStyleName = "", paraStyleName = "";
				int32 currentStyleFlag = -1;
				UID charStyleApplied = kInvalidUID;
				bool16 toClose = false;


				while (index < txtModel->GetPrimaryStoryThreadSpan()) {
					int32 length;
					DataWrapper<textchar> chunk = txtStrand->FindChunk(index, &length);
					int32 indexChunk = 0;
					int32 endPara = 0;
					int32 endCharRun = 0;
					int32 countCharRun, countCharPara;
					//on va boucler sur chaque caractere
					while (indexChunk < length)
					{
						//si on est sur un changement de chunk (para ou char)
						if (indexChunk == endCharRun || indexChunk == endPara) {
							UID paraStyleApplied = paraStrand->GetStyleUID(index + indexChunk, &countCharPara);

							//Changement de style, si on doit fermer les styles caracteres, on ferme
							if ((charStyleApplied != kInvalidUID) && (toClose)) {
								InterfacePtr<IStyleInfo> currentStyleCharInfo(db, charStyleApplied, UseDefaultIID());
								//changement de fds caractere
								charStyleName = currentStyleCharInfo->GetName();
								temp.Append(WideString("[[[/"));
								temp.Append(WideString(charStyleName));
								temp.Append(WideString("]]]"));
								richTextContent.push_back(RichText(RichText::kText, temp));
								temp.clear();
								toClose = false;
							}
							charStyleApplied = charStrand->GetStyleUID(index + indexChunk, &countCharRun);
							if (charStyleApplied != kInvalidUID) {
								//on a un style cara appliqué, on l'ouvre
								InterfacePtr<IStyleInfo> currentStyleCharInfo(db, charStyleApplied, UseDefaultIID());
								//changement de fds caractere
								charStyleName = currentStyleCharInfo->GetName();
								temp.Append(WideString("[[["));
								temp.Append(WideString(charStyleName));
								temp.Append(WideString("]]]"));
								richTextContent.push_back(RichText(RichText::kText, temp));
								temp.clear();
								toClose = true;
							}

							DataWrapper<AttributeBossList> localParaOverrides = paraStrand->GetLocalOverrides(index + indexChunk, nil);
							DataWrapper<AttributeBossList> localCharOverrides = charStrand->GetLocalOverrides(index + indexChunk, nil);

							if (paraStyleApplied != currentParaStyle)
							{
								//on recupere le tag de la fds paragraphe qui vient de changer
								currentParaStyle = paraStyleApplied;
								InterfacePtr<IStyleInfo> currentStyleInfo(db, paraStyleApplied, UseDefaultIID());
								paraStyleName = currentStyleInfo->GetName();
								currentTag = xpgPrefs->GetTagExport(paraStyleName);
							}

							endPara = indexChunk + countCharPara;
							endCharRun = indexChunk + countCharRun;
						}
						if (chunk[indexChunk] == kTextChar_CR) //retour chariot
						{
							//fin de paragraphe, on ecrit
							UID charStyleApplied = charStrand->GetStyleUID(index + indexChunk, &countCharRun);
							if ((charStyleApplied != kInvalidUID) && (toClose)) {
								InterfacePtr<IStyleInfo> currentStyleCharInfo(db, charStyleApplied, UseDefaultIID());
								//changement de fds caractere
								charStyleName = currentStyleCharInfo->GetName();
								temp.Append(WideString("[[[/"));
								temp.Append(WideString(charStyleName));
								temp.Append(WideString("]]]"));
								richTextContent.push_back(RichText(RichText::kText, temp));
								temp.clear();
								charStyleApplied = kInvalidUID;
								toClose = false;
							}

							currentParaStyle = kInvalidUID, currentCharStyle = kInvalidUID;
							endPara = indexChunk + 1;

							//on ajoute tout le para precedent a content, et on vide le richtextcontent
							XPageUtils::AppendToContent(content, currentTag, richTextContent);
							richTextContent.clear();

							if (index + indexChunk < txtModel->GetPrimaryStoryThreadSpan() - 1)
							{
								UID paraStyleApplied = paraStrand->GetStyleUID(index + indexChunk + 1, nil);
								if (paraStyleApplied != currentParaStyle)
								{
									currentParaStyle = paraStyleApplied;
									// Figure out style name to get matching tag via preferences
									InterfacePtr<IStyleInfo> currentStyleInfo(db, currentParaStyle, UseDefaultIID());
									currentTag = xpgPrefs->GetTagExport(currentStyleInfo->GetName());
								}
							}
						}
						else if (chunk[indexChunk] == kTextChar_LF) //Saut de ligne, on remplace par un espace
						{
							temp.Append(kTextChar_Space);
							richTextContent.push_back(RichText(RichText::kText, temp));
							temp.clear();

						}
						else if (chunk[indexChunk] == kTextChar_ObjectReplacementCharacter)
						{
							// Check whether it's an inline item which is anchored in the text
							UID inlineItem = itemStrand->GetOwnedUID(index + indexChunk, kInlineBoss);
							if (inlineItem != kInvalidUID)
							{
								InterfacePtr<IHierarchy> inlineItemHier(db, inlineItem, UseDefaultIID());
								// Look for stories to export in this inline item
								LookForStoriesToExport(currentStoryList, UIDRef(db, inlineItemHier->GetChildUID(0)));
							}
						}
						else
						{
							if (TextChar::IsK2SpecificChar(chunk[indexChunk])) {
								//caractere pose probleme, et peut potentiellement afficher une alerte plus loin: 
								//impossible de transcoder ce caractere. 
							}
							else
								temp.Append(chunk[indexChunk]);
							richTextContent.push_back(RichText(RichText::kText, temp));
							temp.clear();
						}
						++indexChunk;
					}
					index += length;
				}
			}

			// Export content
			WideString currentTagValue;

			K2Vector<KeyValuePair<int32, K2Vector<K2Pair<WideString, K2Vector<RichText> > > > >::iterator contentIter = content.begin();
			while (contentIter != content.end()) {
				K2Vector<K2Pair<WideString, K2Vector<RichText> > >::iterator contentIter2 = contentIter->Value().begin();
				while (contentIter2 != contentIter->Value().end()) {

					if (currentTagValue != contentIter2->first) {
						if (!currentTagValue.IsNull())
							artStream->Pop(kFalse);

						currentTagValue = contentIter2->first;
						artStream->Push(currentTagValue, kFalse);
					}

					artStream->Push(WideString("P"), kFalse);

					K2Vector<RichText> richtextToExport = contentIter2->second;
					K2Vector<RichText>::iterator contentIter3 = richtextToExport.begin();
					while (contentIter3 != richtextToExport.end())
					{
						switch (contentIter3->type) {
						case RichText::kText: artStream->Write(contentIter3->value); break;
						case RichText::kBeginTag: artStream->Push(contentIter3->value, kFalse); break;
						case RichText::kEndTag: artStream->Pop(kFalse); break;
						default: break;
						}
						++contentIter3;
					}


					if (!currentTagValue.IsNull())
						artStream->Pop(kFalse);

					++contentIter2;
				}
				++contentIter;
			}

			if (!currentTagValue.IsNull())
				artStream->Pop(kFalse);

			artStream->Pop(kTrue);
			artStream->Pop(kTrue);
			artStream->Flush();
			artStream->Close();
			artStream.reset();
		} while (kFalse);
	}
	status = kSuccess;

	return status;
}
