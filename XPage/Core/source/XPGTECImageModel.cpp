

#include "VCPluginHeaders.h"

// Interface includes
#include "IXPageMgrAccessor.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IImportProvider.h"
#include "IPMStream.h"
#include "IStringData.h"
#include "ISAXServices.h"
#include "ISAXContentHandler.h"
#include "ISAXEntityResolver.h"
#include "ISAXParserOptions.h"
#include "IPointerData.h"
#include "IXPGPreferences.h"
#include "IXPageUtils.h"
#include "IWebServices.h"

// Project includes
#include "IImageDataModel.h"
#include "XPGImageDataNode.h"
#include "XPGImageModelBase.h"
#include "FileUtils.h"
#include "OpenPlaceID.h"
#include "StreamUtil.h"
#include "ImageID.h"
#include "EPSID.h"
#include "PDFID.h"
#include "DCSID.h"
#include "PlatformFolderTraverser.h"
#include "CAlert.h"
#include "ErrorUtils.h"
#include "StringUtils.h"
#include "K2Stack.h"
#include "XMLDefs.h"


/**  Class representing a folder hierarchy. 
*/
class XPGXRailImageModel : XPGImageModelBase
{
public:
	/** Default constructor */
	XPGXRailImageModel(IPMUnknown* boss);

	/**	See IImageDataModel::Rebuild
		Note : no need to provide numeroId for XRail implementation, caller must provide a filter before any call to Rebuild
	 */
	virtual void Rebuild(const PMString& numeroId, ImageType typeImage = kPhoto);
	
	/**	See IImageDataModel::SetFilter
	 */
	virtual void SetFilter(const PMString& filter, const PMString& filterPath);

	/**	See IImageDataModel::GetImageData
	 */
	virtual bool16 GetImageData(const PMString& id, const IDFile& linkedArticleFile, IDFile& brFile, 
								const bool16& wantNB, IDFile& hrFile, PMString& legend, PMString& credit);
	
protected:
	
	/**	See XPGImageModelBase::cacheChildren
	 */
	virtual void cacheChildren(const PMString& id);

	/** Determine if the file is an image xml description file
	*/
	bool16 isImageXMLFile(const PMString& filename);
	
	/** Parse image XML file and add return a new node according to the file content, nil if an error occurred
	*/
	XPGImageDataNode * ParseImageXMLFile(const IDFile& imageXMLFile);
	
private:	
	
	PMString currentFilter, currentFilterPath;
	K2Vector<PMString> imageSuffixes;
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XPGXRailImageModel, kXPGXRailImageModelImpl)


/* Constructor
*/
XPGXRailImageModel::XPGXRailImageModel(IPMUnknown* boss) : 
	XPGImageModelBase(boss), currentFilter(kNullString), currentFilterPath(kNullString){
	
    int i = 1;
	K2::scoped_ptr<PMString> suffix (imgSuffixes.GetItem(";", i));

	while(suffix != nil)
	{
		imageSuffixes.push_back(*suffix);
		++i;
		suffix.reset(imgSuffixes.GetItem(";", i));
	} 	
}


/* SetFilter
*/
void XPGXRailImageModel::SetFilter(const PMString& filter, const PMString& filterPath)
{
	currentFilter = filter;
	currentFilterPath = filterPath;
}

/* Rebuild
*/
void XPGXRailImageModel::Rebuild(const PMString& numeroId, ImageType typeImage)
{
	if(fRootNode != nil) {
		deleteTree();
	}	
	if(currentFilter.IsEmpty() == kFalse) {
				
		// Set image root path to article file's parent directory
		IDFile imageRootFile = FileUtils::PMStringToSysFile(currentFilterPath);	
		FileUtils::GetParentDirectory(imageRootFile,imageRootFile);

		PMString imageRootFilePath = FileUtils::SysFileToPMString(imageRootFile);
		fRootNode =  new XPGImageDataNode(XPGImageDataNode::kRubriqueNode);
		fRootNode->SetID(imageRootFilePath);
		fRootNode->SetData(Utils<IXPageUtils>()->TruncatePath(imageRootFilePath));
		fRootNode->SetParent(nil);
							
		fIdNodeMap.insert( std::pair<PMString, XPGImageDataNode* >(imageRootFilePath, fRootNode));
		this->cacheChildren(imageRootFilePath);
	}
}
/*
*/
void XPGXRailImageModel::cacheChildren(const PMString& parentId)
{
	do
	{
		if(currentFilter == kNullString)
			break;

		XPGImageDataNode* parentNode = nil;
		std::map<PMString, XPGImageDataNode* >::const_iterator iterForParent = fIdNodeMap.find(parentId);
		if(iterForParent == fIdNodeMap.end()) {
			break;
		}
		parentNode = iterForParent->second;
		ASSERT(parentNode);

		IDFile parIDFile = FileUtils::PMStringToSysFile(parentId);
		PlatformFolderTraverser folderTraverser(parIDFile, kFalse, kFalse, kFalse, kTrue);
        IDFile idFile;
		while (folderTraverser.Next(&idFile))
        {
            
			PMString idFilePath = FileUtils::SysFileToPMString(idFile);
			
			bool16 isDir = FileUtils::IsDirectory(idFile);
			if(!isDir && isImageXMLFile(idFile.GetFileName()))
			{
				XPGImageDataNode * newNode = this->ParseImageXMLFile(idFile);		
				if(newNode)
				{
					// Check whether an item with the same id is already there...if so, don't add current item
					// We also check that image is linked to article ID used as a filter
					std::map<PMString, XPGImageDataNode* >::iterator existingIter = fIdNodeMap.find(newNode->GetID());
					if(existingIter != fIdNodeMap.end() || newNode->GetImageData()->articleID != currentFilter) 
					{
						delete newNode;
					}
					else
					{
						newNode->SetParent(parentNode);			
						parentNode->AddChild(static_cast<const XPGImageDataNode& >(*newNode));
						// Add node to the map 		
						fIdNodeMap.insert( std::pair<PMString, XPGImageDataNode* > (newNode->GetID(), newNode));																	
					}						
				}					
			}
			}

	} while(kFalse);
}

bool16 XPGXRailImageModel::isImageXMLFile(const PMString& filename)
{
	int32 nbSuffixes = imageSuffixes.size();	
	for(int32 i = 0 ; i < nbSuffixes ; ++i)
	{
		int32 posSuff = filename.NumUTF16TextChars() - imageSuffixes[i].NumUTF16TextChars();
		if(posSuff > 0)
		{
			if(filename.IndexOfString(imageSuffixes[i]) == posSuff)
			{
				return kTrue;				
			}				
		}
	}
	
	return kFalse;
}

XPGImageDataNode * XPGXRailImageModel::ParseImageXMLFile(const IDFile& imageXMLFile) {

	XPGImageDataNode* newNode = new XPGImageDataNode(XPGImageDataNode::kImageNode);
	newNode->SetID(FileUtils::SysFileToPMString(imageXMLFile)); // Use node ID property temporarly to pass in data to XRail content handler

	// Parse xml file to get related text data
	InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID()); 
	InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss)); 
	
	InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID()); 

	InterfacePtr<ISAXContentHandler> contentHandler(
		::CreateObject2<ISAXContentHandler>(kXPGXMLImageDescriptionHandlerBoss, ISAXContentHandler::kDefaultIID)); 
	
	InterfacePtr<IPointerData> nodeData (contentHandler, IID_INODEDATA); // Pass node to the handler which will deal with text data initialization from xml data
	nodeData->Set(newNode);

	contentHandler->Register(saxServices);

	InterfacePtr<ISAXEntityResolver> entityResolver(
		::CreateObject2<ISAXEntityResolver>(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID));

	InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());
	
	// These commitments: follow 
	// the existing ImportXMLFileCmd

	parserOptions->SetNamespacesFeature(kFalse);
	parserOptions->SetShowWarningAlert(kFalse);

	InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(imageXMLFile));
	saxServices->ParseStream(xmlFileStream, contentHandler);	
	
	if(ErrorUtils::PMGetGlobalErrorCode() != kSuccess) {

		// Error occured while parsing xml file, don't add node, clean up memory
		delete newNode;
		// Reset Error Code
		ErrorUtils::PMSetGlobalErrorCode(kSuccess);
		PMString error(kXPGErrParseImageFailed);
		error.Translate();
		StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(imageXMLFile));
		CAlert::InformationAlert(error);
		return nil;
	}
	else	
		return newNode;	
}

/** GetImageData
*/
bool16 XPGXRailImageModel::GetImageData(const PMString& id, const IDFile& linkedArticleFile, IDFile& brFile, 
									  const bool16& wantNB, IDFile& hrFile, PMString& legend, PMString& credit)
{
	bool16 found = kFalse;
	// Determine image XML file path from article file path
	IDFile imageXMLFile = linkedArticleFile;

	// Try to found out which type it is by appending each of image suffixes
	int32 suffixIndex = 0;
	do
	{
		imageXMLFile.SetFileName(id + imageSuffixes[suffixIndex]);
		suffixIndex++;
	} while(!FileUtils::DoesFileExist(imageXMLFile) && suffixIndex < imageSuffixes.size());

	if(FileUtils::DoesFileExist(imageXMLFile))
	{
		XPGImageDataNode * newNode = this->ParseImageXMLFile(imageXMLFile);
		if(newNode != nil)
		{
			brFile = FileUtils::PMStringToSysFile(newNode->GetImageData()->brFile);
			if(wantNB)
				hrFile = FileUtils::PMStringToSysFile(newNode->GetImageData()->hrFileNB);
			else
				hrFile = FileUtils::PMStringToSysFile(newNode->GetData());

			legend = newNode->GetImageData()->legend;
			credit = newNode->GetImageData()->credit;
			delete newNode;
			found = kTrue;
		}
	}

	return found;
}

//	end, File: XPGXRailImageModel.cpp
