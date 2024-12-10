#include "VCPlugInHeaders.h"

// Interface includes:
#include "IPMDataObject.h"
#include "IFlavorData.h"
#include "IPMStream.h"
#include "ISysFileListData.h"
#include "IDataExchangeHandler.h"
#include "IDocument.h"
#include "IControlView.h"
#include "ILayoutUIUtils.h"
#include "IPMStream.h"
#include "IPasteboardUtils.h"
#include "ILayoutControlData.h"
#include "ISpread.h"
#include "IStringData.h"
#include "IXPageMgrAccessor.h"
#include "IMultiColumnTextFrame.h"
#include "ISelectionManager.h"
#include "ISelectionUtils.h"
#include "ITextModel.h"
#include "ITextSelectionSuite.h"
#include "IPlacePIData.h"
#include "IFormeData.h"
#include "IXMLUtils.h"
#include "IWebServices.h"
#include "ILinkFacade.h"
#include "IAssignSetPropsCmdData.h"

// General includes:
#include "CDragDropTargetFlavorHelper.h"
#include "CAlert.h"
#include "PMFlavorTypes.h"
#include "DataObjectIterator.h"
#include "UIDList.h"
#include "PMString.h"
#include "IHierarchy.h"
#include "FileUtils.h"
#include "ErrorUtils.h"
#include "StreamUtil.h"
#include "TransformUtils.h"
#include "StringUtils.h"
#include "OpenPlaceID.h"
#include "IFrameType.h"
#include "IGeometry.h"
#include "Utils.h"
#include "ILayoutUtils.h"
#include "IUnitOfMeasure.h"
#include "IDropDownListController.h"
#include "IWidgetUtils.h"
#include "IStringListData.h"
#include "ITreeViewController.h"
#include "ILinkManager.h"
#include "ILinkUtils.h"
#include "IAssignment.h"
#include "ISnippetExport.h"
#include "Utils.h"
#include "IInCopyWorkFlowUtils.h"
#include "IUIColorUtils.h"
#include "IAssignmentUtils.h"
#include "IAssignmentMgr.h"
#include "InCopyImportID.h"
#include "ISubject.h"
#include "IXPageUtils.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGID.h"
#include "XPageUIUtils.h"
#include "XMLDefs.h"

/** XPGUIXRailArticleWithFormeDropTarget.
	Provides the drop behaviour. Our drag and drop target accepts inds files from XPage Formes palette
*/
class XPGUIXRailArticleWithFormeDropTarget : public CDragDropTargetFlavorHelper
{
	public:

		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XPGUIXRailArticleWithFormeDropTarget(IPMUnknown* boss);

		/**
			this method defines the target response to the drag. It is called when the mouse enters the panel. We
			inidcate we can accept drags with a XPageXMLWithFormeFlavor flavor, that we do not want the default cursor

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

	private :
		
		virtual ErrorCode DoStoryLink(const UIDList& formeItemsToLink, const PMString& idArt, const PMString& rub, const PMString& ssRub, const PMString& articleSubject);

};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIXRailArticleWithFormeDropTarget, kXPGUIXRailArticleWithFormeDropTargetImpl)
DEFINE_HELPER_METHODS(XPGUIXRailArticleWithFormeDropTarget)

/* XPGUIXRailArticleWithFormeDropTarget Constructor
*/
XPGUIXRailArticleWithFormeDropTarget:: XPGUIXRailArticleWithFormeDropTarget(IPMUnknown* boss)
									 : CDragDropTargetFlavorHelper(boss), 
									   HELPER_METHODS_INIT(boss)
{
}


DragDrop::TargetResponse XPGUIXRailArticleWithFormeDropTarget::CouldAcceptTypes(const IDragDropTarget* target, DataObjectIterator* dataIter, const IDragDropSource* fromSource, const IDragDropController* controller) const
{
	DataExchangeResponse response;
	response = dataIter->FlavorExistsWithPriorityInAllObjects(XPageXMLWithFormeFlavor);
	if(response.CanDo()) 	 
 		return DragDrop::TargetResponse(response, DragDrop::kDropWillCopy, DragDrop::kUseDefaultTrackingFeedback, DragDrop::kTargetWillProvideCursorFeedback);
 
	return DragDrop::kWontAcceptTargetResponse;
}

/*
	the drop has occured, we need to handle it.
*/
ErrorCode	
XPGUIXRailArticleWithFormeDropTarget::ProcessDragDropCommand(IDragDropTarget* target , IDragDropController* controller, DragDrop::eCommandType action)
{
	bool16 returnCode = kFailure;	
	do{
		if (action != DragDrop::kDropCommand)
			break;
	
		// we should ensure the drag has been internalized, if we are coming from a 
		// custom source, the handler may not be initialised
		if (controller->InternalizeDrag(XPageXMLWithFormeFlavor, XPageXMLWithFormeFlavor) != kSuccess)	
			break;

		InterfacePtr<IDataExchangeHandler> handler (controller->QuerySourceHandler());			

		InterfacePtr<ISysFileListData> fileData (handler, IID_ISYSFILELISTDATA);		
		IDFile xmlFileToImport = fileData->GetSysFileItem(0);
		IDFile formeFileToImport = fileData->GetSysFileItem(1);
		IDFile matchingFile = fileData->GetSysFileItem(2);		

		InterfacePtr<IStringListData> textData (handler, IID_ISTRINGLISTDATA);		
		PMString artId = textData->GetStringList()[0];
		PMString artType = textData->GetStringList()[1];
		PMString artSnippetFile = textData->GetStringList()[2];
		PMString artSubject = textData->GetStringList()[3];
		PMString artIdStatus = textData->GetStringList()[4];
		PMString artLibelleStatus = textData->GetStringList()[5];
		PMString artCouleurStatus = textData->GetStringList()[6];
		PMString artRubrique = textData->GetStringList()[7];
		PMString artSsRubrique = textData->GetStringList()[8];

		// Get mouse location
		InterfacePtr<IControlView> layoutView (target, UseDefaultIID());
		GSysPoint where = controller->GetDragMouseLocation();
		PMPoint currentPoint = Utils<ILayoutUIUtils>()->GlobalToPasteboard(layoutView, where);
		
		// Get target spread
		InterfacePtr<ISpread> targetSpread (Utils<IPasteboardUtils>()->QueryNearestSpread(layoutView, currentPoint));			

		// Get Current Document and DataBase
		InterfacePtr<ILayoutControlData> layoutCtrlData (layoutView, UseDefaultIID());
		IDocument * doc = layoutCtrlData->GetDocument();
		if(doc == nil)	break;

		IDataBase * db = ::GetDataBase(doc);
		if(db == nil)	break;	

		// Get Story Text type : MarbreGeneral, MarbreParution
		PMString::ConversionError convError;
		int8 kArtType = artType.GetAsNumber(&convError);
		if (convError != PMString::kNoError)
			break;

		// Import snippet 
		if(kArtType == kIncopyStory){
			UIDList formeItemsToLink(db);
			PMString error = kNullString;
			if(Utils<IXPageUtils>()->ImportForme(::GetUIDRef(doc), FileUtils::PMStringToSysFile(artSnippetFile), 
												currentPoint, ::GetUIDRef(targetSpread), matchingFile, error, &formeItemsToLink, kTrue, kTrue)!= kSuccess)
			{
				CAlert::InformationAlert(error);
				break;
			}
			
			// Article Froid InCopy
			// Delete stories List on assignement  
			InterfacePtr<ILinkManager> linkManager(db, db->GetRootUID(), UseDefaultIID());
			UIDList linkRessources(db);
			for(int32 i = 0; i < formeItemsToLink.size(); ++i){
				UIDRef storyRef = formeItemsToLink.GetRef(i);					
				linkRessources.Append(formeItemsToLink[i]);
			}
			if(linkRessources.size() > 0)
				Utils<Facade::ILinkFacade>()->DeleteLinksByObject(linkRessources, kTrue, kTrue);
			
			// Do New Assignment With WS StoryLinkTEC
			DoStoryLink(formeItemsToLink, artId, artRubrique, artSsRubrique, artSubject);
			
			// Recuperer l'utilisateur courant
			InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ILOGINSTRING);
			PMString creatorLogin = gLogin->Get();
			if (creatorLogin == "")
				break;

			InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());	
			InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
			xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL());

			// Mettre a jour les informations manquantes, voire les parametres
			PMString resultWS = kNullString;
			/*if(!xrailConnexion->UpdateStoryStatusChanged(artId, artIdStatus, artLibelleStatus, artCouleurStatus, kNullString, creatorLogin, kNullString, resultWS))
			{
				error = ErrorUtils::PMGetGlobalErrorString();
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::InformationAlert(error);
				break;
			}*/
		}
		
		if(kArtType == kMEPJavaStory){			
			// Article Java, show Dialog to link story 
			XPageUIUtils::DisplayLinkArticleDialog(artId, artSnippetFile, artSubject, matchingFile, currentPoint, artIdStatus, 
												   artLibelleStatus, artCouleurStatus, ::GetUIDRef(targetSpread), 
												   kArtType, FileUtils::SysFileToPMString(xmlFileToImport), artRubrique, artSsRubrique);		
		}			

		// Send notification so that texte panel is updated
		InterfacePtr<ISubject> sessionSubject(GetExecutionContextSession(), UseDefaultIID());
		sessionSubject->Change(kXPGRefreshMsg, IID_IREFRESHPROTOCOL);					
	
		returnCode = kSuccess;

	}while(false);

	return returnCode;
}

void XPGUIXRailArticleWithFormeDropTarget::DoDragEnter(IDragDropTarget * target){
	InterfacePtr<IDragDropController> controller (GetExecutionContextSession(), UseDefaultIID());
	controller->SetTrackingCursorFeedback(CursorSpec(kCrsrItemCreation));
}
		
ErrorCode XPGUIXRailArticleWithFormeDropTarget::DoStoryLink(const UIDList& formeItemsToLink, const PMString& idArt, 
															const PMString& rub, const PMString& ssRub, 
															const PMString& articleSubject)
{
	ErrorCode status = kFailure;
	do{
		IDataBase * db = formeItemsToLink.GetDataBase();
		if(db == nil)
			break;

		//Calcul des cordonnees des items selectionnes			
		PMPoint leftTopMost (kMaxInt32, kMaxInt32), 
			    rightBottomMost (-kMaxInt32, -kMaxInt32);

		UID leftMostSelectedItem = kInvalidUID;		
		for(int32 i = 0 ; i < formeItemsToLink.Length() ; ++i) {			
			
			InterfacePtr<IGeometry> itemGeo ( formeItemsToLink.GetRef(i), UseDefaultIID());
			PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();
			PMPoint rightBottom = itemGeo->GetStrokeBoundingBox().RightBottom();
			 
			::TransformInnerPointToPasteboard (itemGeo, &leftTop);
			::TransformInnerPointToPasteboard (itemGeo, &rightBottom);
	
			if(leftTop.X() < leftTopMost.X()){
				leftMostSelectedItem = formeItemsToLink.At(i);
				leftTopMost.X() = leftTop.X();
			}			
			if(leftTop.Y() < leftTopMost.Y())
				leftTopMost.Y() = leftTop.Y();		

			if(rightBottom.X() > rightBottomMost.X())
				rightBottomMost.X() = rightBottom.X();

			if(rightBottom.Y() > rightBottomMost.Y())
				rightBottomMost.Y() = rightBottom.Y();
		}

		//l'id de la page
		InterfacePtr<IHierarchy> leftMostItemHier (db, leftMostSelectedItem, UseDefaultIID());
		UID ownerPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID(leftMostItemHier);

		InterfacePtr<ISpread> isASpread (db, ownerPageUID, UseDefaultIID());
		if(isASpread != nil)
			// Le bloc le plus a gauche de l'article est hors page, on renvoie l'ID de la page la plus a gauche dans la planche
			ownerPageUID = isASpread->GetNthPageUID(0);	

		// Recuperartion de l'id de la page a partir de Xrail Client
		if(ownerPageUID == kInvalidUID) // Wrong page index
			break;

		InterfacePtr<IXRailPageSlugData> readData (db, ownerPageUID , IID_PAGESLUGDATA);		
        int32 idPage = readData->GetID();

		//Conversion apres le calcul des cordonnees des items	
		PMString posX = kNullString, posY = kNullString, largeur = kNullString, hauteur = kNullString, 
				 storyFullPath = kNullString, resultWS = kNullString;		
		
		// Calcul en millim￨tres des dimensions
		InterfacePtr<IUnitOfMeasure> uom ((IUnitOfMeasure *)::CreateObject(kMillimetersBoss,IID_IUNITOFMEASURE));
		largeur.AppendNumber(uom->PointsToUnits(rightBottomMost.X() - leftTopMost.X()), 3, kTrue, kTrue);
		hauteur.AppendNumber(uom->PointsToUnits(rightBottomMost.Y() - leftTopMost.Y()), 3, kTrue, kTrue);	
		
		// Calcul de l'origine dans le rep￨re de la page
		InterfacePtr<IHierarchy> hier (db, formeItemsToLink[0], UseDefaultIID());
		UID ownerPage  = Utils<ILayoutUtils>()->GetOwnerPageUID(hier);
		
		InterfacePtr<IGeometry> ownerPageGeo (db, ownerPage, UseDefaultIID());
		::TransformPasteboardPointToInner (ownerPageGeo, &leftTopMost);
	
		posX.AppendNumber(uom->PointsToUnits(leftTopMost.X()), 3, kTrue, kTrue);
		posY.AppendNumber(uom->PointsToUnits(leftTopMost.Y()), 3, kTrue, kTrue);

		// Relink with TEC and Update Status
		InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());	
		InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL());

		// Recuperer l'utilisateur courant
		InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ILOGINSTRING);
		PMString creatorLogin = gLogin->Get();
		if (creatorLogin == "")
				break;

		if(!xrailConnexion->StoryLink(creatorLogin, idArt, idPage, kNullString, kNullString, posX, posY, hauteur,
			                          largeur, kIncopyStory, rub, ssRub, articleSubject, creatorLogin, storyFullPath, resultWS))
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::InformationAlert(error);
			break;
		}		

		// Get Path of the assignment	
		storyFullPath.Append(idArt);
		storyFullPath.Append(PMString(kXPGExtensionInCopyKey, PMString::kTranslateDuringCall));
		IDFile assignFile = FileUtils::PMStringToSysFile(storyFullPath);
	
		if(Utils<IXPageUtils>()->DoNewAssignment(formeItemsToLink, assignFile, articleSubject, creatorLogin, kTrue, idArt, kFalse) != kSuccess)
			break;

		// Prevenir TEC Pour qu'il change de parution
		// Redondance : l'occurrence multiple des appels de WS avec les m￪mes param￨tres
		if(!xrailConnexion->StoryParutionChange(idPage, idArt, rub, ssRub, resultWS)){
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::InformationAlert(error + resultWS);
			break;		
		}	
		if(resultWS.GetAsNumber() != 0 ){
			CAlert::InformationAlert("Error WS_StoryParutionChange: " + resultWS);
			break;
		}

	}while(kFalse);
	status = kSuccess;
	return status;
}
// End, XPGUIXRailArticleWithFormeDropTarget.cpp.
