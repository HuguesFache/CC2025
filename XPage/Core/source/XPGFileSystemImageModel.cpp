

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
#include "IXPGPreferences.h"
#include "IXPageUtils.h"

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
//#include "SVGPageItemID.h"
#include "CAlert.h"
#include "ErrorUtils.h"
#include "StringUtils.h"
#include "K2Stack.h"
#include "PlatformFolderTraverser.h"

/**  Class representing a folder hierarchy. 
*/
class XPGFileSystemImageModel : XPGImageModelBase
{
public:
	/** Default constructor */
	XPGFileSystemImageModel(IPMUnknown* boss);

	/**	See IImageDataModel::Rebuild
	 */
	virtual void Rebuild(const PMString& numeroId, ImageType typeImage = kPhoto);
	
	/**	See IImageDataModel::SetFilter
	 */
	virtual void SetFilter(const PMString& filter, const PMString& filterPath);

	/**	See IImageDataModel::GetImageData
		Not used by GISI version
	 */
	virtual bool16 GetImageData(const PMString& id, const IDFile& linkedArticleFile, IDFile& brFile, 
								const bool16& wantNB, IDFile& hrFile, PMString& legend, PMString& credit);
	
protected:
	
	/**	See XPGImageModelBase::cacheChildren
	 */
	virtual void cacheChildren(const PMString& id);

	/** Determine if the file is an image file that InDesign can import
	*/
	bool16 isImageFile(const IDFile& file);
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XPGFileSystemImageModel, kXPGFileSystemImageModelImpl)


/* Constructor
*/
XPGFileSystemImageModel::XPGFileSystemImageModel(IPMUnknown* boss) : 
	XPGImageModelBase(boss)
{	
}

/* SetFilter
*/
void XPGFileSystemImageModel::SetFilter(const PMString& filter, const PMString& filterPath)
{
	// Not implemented in GISI version
}

/* Rebuild
*/
void XPGFileSystemImageModel::Rebuild(const PMString& numeroId, ImageType typeImage)
{
	if(fRootNode != nil) {
		deleteTree();
	}
	if(numeroId.IsEmpty() == kFalse) {
		
		IDFile imageRootFile = FileUtils::PMStringToSysFile(numeroId);

		switch (typeImage)
		{
			case kPhoto : FileUtils::AppendPath(&imageRootFile, imageDir); break;
			case kOther : FileUtils::AppendPath(&imageRootFile, imageDir); break;
		}	

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
void XPGFileSystemImageModel::cacheChildren(const PMString& parentId)
{
	do
	{	
		XPGImageDataNode* parentNode = nil;
	
		std::map<PMString, XPGImageDataNode* >::const_iterator iterForParent = fIdNodeMap.find(parentId);
	
		if(iterForParent == fIdNodeMap.end()) {
			break;
		}
		parentNode = iterForParent->second;
		ASSERT(parentNode);

		IDFile parIDFile = FileUtils::PMStringToSysFile(parentId);
		PlatformFolderTraverser folderTraverser(parIDFile, kTrue, kFalse, kFalse, kTrue);
        IDFile idFile;
		while (folderTraverser.Next(&idFile))
        {
        	PMString idFilePath = FileUtils::SysFileToPMString(idFile);
			
			bool16 isDir = FileUtils::IsDirectory(idFile);
			if( isDir && validPath(idFile.GetFileName()))
			{
				XPGImageDataNode* newNode = new XPGImageDataNode(XPGImageDataNode::kRubriqueNode);
				newNode->SetID(idFilePath);
				newNode->SetData(Utils<IXPageUtils>()->TruncatePath(idFilePath));
				newNode->SetParent(parentNode);
				parentNode->AddChild(static_cast<const XPGImageDataNode& >(*newNode));

				fIdNodeMap.insert( std::pair<PMString, XPGImageDataNode* > (idFilePath, newNode));
			}
			else 
			{
				if(!isDir && isImageFile(idFile))
				{	
					XPGImageDataNode* newNode = new XPGImageDataNode(XPGImageDataNode::kImageNode);
					newNode->SetID(idFilePath);
					newNode->SetData(idFilePath);
					newNode->SetImageData(Utils<IXPageUtils>()->TruncatePath(idFilePath), kNullString, kNullString, kNullString);
					newNode->SetParent(parentNode);
					parentNode->AddChild(static_cast<const XPGImageDataNode& >(*newNode));
		
					fIdNodeMap.insert( std::pair<PMString, XPGImageDataNode* > (idFilePath, newNode));			
				}
			}
		
		}

	} while(kFalse);
}

bool16 XPGFileSystemImageModel::isImageFile(const IDFile& file)
{
	bool16 isImage = kFalse;

	InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID()); 

	int32 numProviders = serviceRegistry->GetServiceProviderCount(kImportProviderService); 
	for(int32 i = 0 ; i < numProviders ; ++i)
	{
		InterfacePtr<IK2ServiceProvider> serviceProvider(serviceRegistry->QueryNthServiceProvider(kImportProviderService, i));
		InterfacePtr<IImportProvider> importProvider (serviceProvider, IID_IIMPORTPROVIDER);

		InterfacePtr<IPMStream> fileStream (StreamUtil::CreateFileStreamReadLazy(file));

		if(importProvider->CanImportThisStream(fileStream) == IImportProvider::kFullImport)
		{
			ClassID importProviderID = serviceRegistry->GetNthServiceProviderID(kImportProviderService, i);
			if(importProviderID == kImagePlaceProviderBoss
			 || importProviderID == kPDFPlaceProviderBoss
			 || importProviderID == kEPSPlaceProviderBoss
			 //|| importProviderID == kSVGPlaceProviderBoss)
			 || importProviderID == kDCSPlaceProviderBoss)
			{
				isImage = kTrue;
				break;
			}
		}		
	}
	return isImage;
}

bool16 XPGFileSystemImageModel::GetImageData(const PMString& id, const IDFile& linkedArticleFile, IDFile& brFile, 
								const bool16& wantNB, IDFile& hrFile, PMString& legend, PMString& credit)
{
	// Not implemented for GISI
	return kFalse;
}

//	end, File: XPGFileSystemImageModel.cpp
