
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IPMDataObject.h"
#include "IFlavorData.h"
#include "IPMStream.h"
#include "ISysFileListData.h"
#include "IDataExchangeHandler.h"
#include "IDocument.h"
#include "IPMStream.h"
#include "IPlacePIData.h"
#include "IFrameType.h"
#include "IStringData.h"
#include "ISpread.h"
#include "IFormeData.h"
#include "IXMLUtils.h"
#include "IAssignmentMgr.h"
#include "IItemLockData.h"
#include "IImportResourceCmdData.h"
#include "IURIUtils.h"
#include "CAlert.h"

// General includes:
#include "CDragDropTargetFlavorHelper.h"
#include "CAlert.h"
#include "PMFlavorTypes.h"
#include "DataObjectIterator.h"
#include "UIDList.h"
#include "PMString.h"
#include "IHierarchy.h"
#include "FileUtils.h"
#include "IPageItemTypeUtils.h"
#include "ErrorUtils.h"
#include "StreamUtil.h"
#include "PageItemScrapID.h"
#include "OpenPlaceID.h"
#include "ILiveEditFacade.h"
#include "IPlaceGun.h"
#include "IUIDData.h"
#include "ITransform.h"
#include "IGeometry.h"
#include "TransformUtils.h"
#include "IFrameContentFacade.h"
#include "IGraphicFrameData.h"
#include "IStoryUtils.h"
#include "IXPageUtils.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGID.h"

/** XPGUIXRailImageDropTarget
	Provides the drop behaviour. Our drag and drop target accepts xml files from XPage Textes palette
*/
class XPGUIXRailImageDropTarget : public CDragDropTargetFlavorHelper
{
	public:

		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XPGUIXRailImageDropTarget(IPMUnknown* boss);

		/**
			this method defines the target response to the drag. It is called when the mouse enters the panel. We
			inidcate we can accept drags with a XPageImageFlavor flavor, that we do not want the default cursor

			@param dataIter IN iterator providing access to the data objects within the drag.
			@param fromSource IN the source of the drag.
			@param controller IN the drag drop controller mediating the drag.
			@return a target response (either won't accept or drop will copy).

			@see DragDrop::TargetResponse
		*/
		DragDrop::TargetResponse CouldAcceptTypes(const IDragDropTarget* target, DataObjectIterator* dataIter, const IDragDropSource* fromSource, const IDragDropController* controller) const;
		/**
			When the drop is performed, this method is called. We get the xml file and then do the import into the text frame
			@param controller IN the drag drop controller mediating the drag.
			@param type IN drag and drop command type
			@see DragDrop::eCommandType
		*/
		ErrorCode	ProcessDragDropCommand(IDragDropTarget* target , IDragDropController* controller, DragDrop::eCommandType action);
			
		/** Enable target helper to track drag (which means DoDragEnter will get called
		*/
		virtual bool16 HelperWillDoDragTracking() const { return kTrue; }
	
		/** Change cursor appearance when it's above target
		*/
		virtual void DoDragEnter(IDragDropTarget * target);
		
		DECLARE_HELPER_METHODS()

private:

		bool16 CheckIfBelongToAssignment(const IDragDropTarget* target) const;
		UIDRef ImportImageAndLoadPlaceGun (const UIDRef & itemRef, const URI& uri);
	
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIXRailImageDropTarget, kXPGUIXRailImageDropTargetImpl)
DEFINE_HELPER_METHODS(XPGUIXRailImageDropTarget)

/* XPGUIXRailImageDropTarget Constructor
*/
XPGUIXRailImageDropTarget::XPGUIXRailImageDropTarget(IPMUnknown* boss)
: CDragDropTargetFlavorHelper(boss), HELPER_METHODS_INIT(boss)
{
}


DragDrop::TargetResponse XPGUIXRailImageDropTarget::CouldAcceptTypes(const IDragDropTarget* target, DataObjectIterator* dataIter, const IDragDropSource* fromSource, const IDragDropController* controller) const
{
	DataExchangeResponse response;
	response = dataIter->FlavorExistsWithPriorityInAllObjects(XPageImageFlavor);
	if(response.CanDo()) 
	{	
		InterfacePtr<IFrameType> frameType (target, UseDefaultIID());
		if(frameType && frameType->IsGraphicFrame())
		{
#if INCOPY
			if(CheckIfBelongToAssignment(target))
#endif
 				return DragDrop::TargetResponse( response, DragDrop::kDropWillCopy,
 												 DragDrop::kUseDefaultTrackingFeedback,
		 										 DragDrop::kTargetWillProvideCursorFeedback);
		}
	}

	return DragDrop::kWontAcceptTargetResponse;
}

/*
	the drop has occured, we need to handle it.
*/
ErrorCode XPGUIXRailImageDropTarget::ProcessDragDropCommand(IDragDropTarget* target , IDragDropController* controller, DragDrop::eCommandType action)
{
	bool16 returnCode = kFailure;
	UIDRef result = UIDRef::gNull;
	do{
		if (action != DragDrop::kDropCommand)  
			break;
		 
		// we should ensure the drag has been internalized, if we are coming from a 
		// custom source, the handler may not be initialised
		if (controller->InternalizeDrag(XPageImageFlavor, XPageImageFlavor) != kSuccess)	
			break;

		InterfacePtr<IDataExchangeHandler> handler (controller->QuerySourceHandler());	
		InterfacePtr<ISysFileListData> fileData (handler, IID_ISYSFILELISTDATA);
		IDFile imageFileToImport = fileData->GetSysFileItem(0);	

		// Check out item if is an InCopy story
		if(Utils<IStoryUtils>()->IsAStory(::GetUIDRef(target))){
			InterfacePtr<IItemLockData> lockData (target, UseDefaultIID()); 
			if(lockData->GetInsertLock()){
				Utils<Facade::ILiveEditFacade>()->CheckOut(UIDList(target));
			}
		}

		IDataBase * db = ::GetDataBase(target);
		if(db == nil)
			break;

		// Import image
		InterfacePtr<ICommand> importFileCmd(CmdUtils::CreateCommand(kImportResourceCmdBoss));
		InterfacePtr<IImportResourceCmdData> data(importFileCmd, IID_IIMPORTRESOURCECMDDATA);

		// IDFile to URI
		URI imageURI;
		Utils<IURIUtils>()->IDFileToURI(imageFileToImport, imageURI);

        // Set the file to import
        data->Set(db, imageURI, K2::kSuppressUI);
		if(CmdUtils::ProcessCommand(importFileCmd) != kSuccess)
			break;

		UIDRef imageItem = UIDRef(db, importFileCmd->GetItemList()->First());

		// Delete existing image item in target, if any
		InterfacePtr<IHierarchy> containerFrame (target, UseDefaultIID());
		if(containerFrame && containerFrame->GetChildCount() > 0){
			InterfacePtr<IHierarchy> frameToDelete (containerFrame->QueryChild(0));
			InterfacePtr<ICommand> deletePageItem (CmdUtils::CreateCommand(kDeleteCmdBoss));
			deletePageItem->SetItemList(UIDList(frameToDelete));
			if(CmdUtils::ProcessCommand(deletePageItem) != kSuccess)
				break;
		}
	
		// Import Image with commande
		UIDRef parentUID = ::GetUIDRef(target);
		InterfacePtr<IGeometry> itemGeo (parentUID, UseDefaultIID());
		if (itemGeo == nil) 
            break;

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
		placeData->Set(::GetUIDRef(target), &pointList, kFalse);

		// Set the item to place
		placeItemCmd->SetItemList(UIDList(imageItem));
		returnCode = CmdUtils::ProcessCommand(placeItemCmd);	
		
		//Fill Prop picture
		UIDList imageList = UIDList(imageItem);
		Utils<Facade::IFrameContentFacade>()->FitContentProp(imageList);
		
		// Import credit and legend
		InterfacePtr<IStringData> creditData(handler, IID_ICREDITPHOTO);
		InterfacePtr<IStringData> legendData(handler, IID_ILEGENDPHOTO);

		// Get credit and legend frame (look for frames in the same spread)
		InterfacePtr<IFormeData> photoFormeData(parentUID, UseDefaultIID());
		if(photoFormeData == nil)
			break;
	
		PMString formeName = photoFormeData->GetUniqueName();	
		int16 photoIndex = photoFormeData->GetPhotoIndex();		
		InterfacePtr<ISpread> imageSpread (db, containerFrame->GetSpreadUID(), UseDefaultIID());
		if(imageSpread == nil)
			break;

		UIDList allFormeItems(db);
		Utils<IXPageUtils>()->GetAllFormeItemsOnSpread(formeName, imageSpread, allFormeItems);
		int32 nbItems = allFormeItems.Length();
		for(int32 i = 0 ; i < nbItems ; ++i){
			InterfacePtr<IFormeData> itemFormeData (allFormeItems.GetRef(i), UseDefaultIID());
			int currentPhotoIndex = itemFormeData->GetPhotoIndex();
			if(itemFormeData->GetType() == IFormeData::kCredit && currentPhotoIndex == photoIndex){
				// A credit frame associated with the photo has been found, import credit text in it
				UIDRef targetStory = ::GetUIDRef(itemFormeData);					
				Utils<IXMLUtils>()->GetActualContent(targetStory);
				Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, creditData->Get(), kTrue);			
			}				
			else if(itemFormeData->GetType() == IFormeData::kLegend && currentPhotoIndex == photoIndex){
				// A legend frame associated with the photo has been found, import credit text in it
				UIDRef targetStory = ::GetUIDRef(itemFormeData);					
				Utils<IXMLUtils>()->GetActualContent(targetStory);
				Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, legendData->Get(), kFalse);				
			}	
		}
		/*UIDList itemsOnPage(db);
		imageSpread->GetItemsOnPage(0, &itemsOnPage, kFalse, kTrue);

		int32 nbItems = itemsOnPage.Length();
		for(int32 i = 0 ; i < nbItems ; ++i){
			InterfacePtr<IFormeData> itemFormeData (itemsOnPage.GetRef(i), UseDefaultIID());
			if(itemFormeData && itemFormeData->GetUniqueName().Compare(kFalse, formeName) == 0){	
				int currentPhotoIndex = itemFormeData->GetPhotoIndex();				
				if(itemFormeData->GetType() == IFormeData::kCredit && currentPhotoIndex == photoIndex){
					// A credit frame associated with the photo has been found, import credit text in it
					UIDRef targetStory = ::GetUIDRef(itemFormeData);					
					Utils<IXMLUtils>()->GetActualContent(targetStory);
					Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, creditData->Get(), kTrue);			
				}				
				else if(itemFormeData->GetType() == IFormeData::kLegend && currentPhotoIndex == photoIndex){
					// A legend frame associated with the photo has been found, import credit text in it
					UIDRef targetStory = ::GetUIDRef(itemFormeData);					
					Utils<IXMLUtils>()->GetActualContent(targetStory);
					Utils<IXPageUtils>()->ImportCreditOrLegend(targetStory, legendData->Get(), kFalse);				
				}					
			}
		}*/

	}while(false);

	return returnCode;
}

void XPGUIXRailImageDropTarget::DoDragEnter(IDragDropTarget * target) {

	InterfacePtr<IDragDropController> controller (GetExecutionContextSession(), UseDefaultIID());
	controller->SetTrackingCursorFeedback(CursorSpec(kCrsrPlaceImageInto));
}

bool16 XPGUIXRailImageDropTarget::CheckIfBelongToAssignment(const IDragDropTarget* target) const{
	bool16 result = kFalse;
	do{
		// Get target's file (which has to be an assignment file)
		IDataBase * db = ::GetDataBase(target);
		if(db->GetSysFile() == nil)
			break;

		IDFile openedAssignmentFile = *db->GetSysFile();

		// Get the assignment the target belongs to, if any
		InterfacePtr<IAssignmentMgr> assignmentMgr (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IAssignment>  assignment ( nil);		
		InterfacePtr<IAssignedStory >  assignmentStory ( nil);	

		assignmentMgr->QueryAssignmentAndAssignedStory (::GetUIDRef(target), assignment , assignmentStory);
		if(assignment == nil)
			break;

		IDFile targetAssignmentFile = FileUtils::PMStringToSysFile(assignment->GetFile());

		// Compare assignment files
		if(openedAssignmentFile == targetAssignmentFile)	
			result = kTrue;

	} while(kFalse);

	return result;
}

/**
	import image and get item ref
*/

UIDRef XPGUIXRailImageDropTarget::ImportImageAndLoadPlaceGun (const UIDRef & itemRef, const URI& uri){
  
    UIDRef retval = UIDRef::gNull;
    do{
		PMString seqName("XPage : ImportImageAndLoadPlaceGun ");
        CmdUtils::SequenceContext seq(&seqName);

        // If the place gun is already loaded abort it so we can re-load it. 
        InterfacePtr<IPlaceGun> placeGun(itemRef, UseDefaultIID());
        if (!placeGun){
            ErrorUtils::PMSetGlobalErrorCode(kFailure);
            break;
        }
        if (placeGun->IsLoaded()){
            InterfacePtr<ICommand> abortPlaceGunCmd( CmdUtils::CreateCommand(kAbortPlaceGunCmdBoss));           
            if (!abortPlaceGunCmd){
                ErrorUtils::PMSetGlobalErrorCode(kFailure);
                break;
            }
            InterfacePtr<IUIDData> uidData(abortPlaceGunCmd, UseDefaultIID());       
            if (!uidData) {
                ErrorUtils::PMSetGlobalErrorCode(kFailure);
                break;
            }
            uidData->Set(placeGun);
            if (CmdUtils::ProcessCommand(abortPlaceGunCmd) != kSuccess){             
                ErrorUtils::PMSetGlobalErrorCode(kFailure);
                break;
            }
        }

        // Now import the selected file and load it's UID into the place gun.
        InterfacePtr<ICommand> importCmd(CmdUtils::CreateCommand(kImportAndLoadPlaceGunCmdBoss));    
        if(!importCmd){
            ErrorUtils::PMSetGlobalErrorCode(kFailure);
            break;
        }
        IDataBase* db = itemRef.GetDataBase();      
        if(!db){
            ErrorUtils::PMSetGlobalErrorCode(kFailure);
            break;
        }
        InterfacePtr<IImportResourceCmdData> importResourceCmdData(importCmd, IID_IIMPORTRESOURCECMDDATA); // no DefaultIID for this
        if(!importResourceCmdData){
            ErrorUtils::PMSetGlobalErrorCode(kFailure);
            break;
        }            
        importResourceCmdData->Set(db, uri, kMinimalUI);//pageItemUID);
        ErrorCode err = CmdUtils::ProcessCommand(importCmd);      
        if(err != kSuccess) {
            break;
        }   
        // Get the contents of the place gun as our return value
        UIDRef placedItem(db, placeGun->GetFirstPlaceGunItemUID());
        retval = placedItem; 
    } while(kFalse);

    return retval;
}



// End, XPGUIXRailImageDropTarget.cpp.


