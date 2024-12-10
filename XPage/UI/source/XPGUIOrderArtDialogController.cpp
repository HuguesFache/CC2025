/*
//	File:	XPGUIOrderArtDialogController.cpp
//
//  Author : Trias Developpement
*/
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IActiveContext.h"
#include "ITextControlData.h"
#include "IControlView.h"
#include "IPanelControlData.h"
#include "IStringListControlData.h"
#include "IStringListData.h"
#include "IStringData.h"
#include "IDropDownListController.h"
#include "ISelectionUtils.h"
#include "ISelectionManager.h"
#include "IXPageSuite.h"
#include "IDocument.h"
#include "IAssignmentMgr.h"
#include "IAssignedDocument.h"
#include "IAssignment.h"
#include "IAssignmentUtils.h"
#include "FileTypeInfo.h"
#include "IDocument.h"
#include "IUIColorUtils.h"
#include "ISnippetExport.h"
#include "IInCopyWorkFlowUtils.h"
#include "StreamUtil.h"
#include "InCopyImportID.h"
#include "IWebServices.h"
#include "ILayoutUtils.h"
#include "IHierarchy.h"
#include "IXPGPreferences.h"
#include "ITextModel.h"
#include "IDocumentCommands.h"
#include "IGeometry.h"
#include "IUIDData.h"
#include "IUIDListData.h"
#include "IUnitOfMeasure.h"
#include "IDocumentSignalData.h"
#include "TransformTypes.h"
#include "TransformUtils.h"
#include "IPageList.h"
#include "ISpreadLayer.h"
#include "ICommand.h"
#include "IDateTimeData.h"
#include "ISpread.h"
#include "ITextModel.h"
#include "ILayoutUtils.h"
#include "IXPageUtils.h"
#include "IXRailPageSlugData.h"
#include "IXPGPreferences.h"
#include "IDataBase.h"
#include "IIntData.h"
#include "IAssignSetPropsCmdData.h"
#include "IInCopyDocUtils.h"
#include "IInCopyWorkflow.h"
#include "IXPageSuite.h"
#include "IXPGPreferences.h"
#include "IFormeData.h"

// General includes:
#include "CDialogController.h"
#include "FileUtils.h"
#include "ErrorUtils.h"
#include "CAlert.h"
#include "StringUtils.h"
#include "FileTypeRegistry.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGID.h"
#include "XRLID.h"
#include "IDTime.h"
#include "XPageUIUtils.h"
#include "IPlacedArticleData.h"

#define artIncopyType 1


class XPGUIOrderArtDialogController : public CDialogController{

	public:
	
        XPGUIOrderArtDialogController(IPMUnknown* boss) : CDialogController(boss),
														  idPage(0), 
														  nbSignesEstimString(kNullString), 
														  nbSignesEstimWS(kNullString),
														  ownerPageUID(kInvalidUID) {}
		virtual ~XPGUIOrderArtDialogController() {}	 
		virtual void InitializeDialogFields(IActiveContext* dlgContext);
		virtual WidgetID ValidateDialogFields(IActiveContext* myContext);	
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
	
	private :
	
		int32 idPage;
		PMString nbSignesEstimString, nbSignesEstimWS;
		UID ownerPageUID;
		UIDList selectedItems, txtFrames, graphicFrames;		
		
};

CREATE_PMINTERFACE(XPGUIOrderArtDialogController, kXPGUIOrderArtDialogControllerImpl)

void XPGUIOrderArtDialogController::InitializeDialogFields(IActiveContext* dlgContext) {
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());

	IDocument * doc = dlgContext->GetContextDocument();
	IDataBase * db = ::GetDataBase(doc);
	
    // Reset all text fields
	// SetTextControlData(kXPGUITopicWidgetID, kNullString);	
    SetTextControlData(kXPGUINbImagesTextWidgetId, "0");	

    do{
		
		// Get selected items and the folio of the page they belong to
		InterfacePtr<ISelectionManager> selectionMgr (Utils<ISelectionUtils>()->QueryActiveSelection ());					
		InterfacePtr<IXPageSuite> xpgSuite (selectionMgr, IID_IXPAGESUITE);
		if(xpgSuite == nil) // xpgSuite == nil means there is no active selection (neither layout nor text)
			break;		
		
		// FIX Bug Version 7.5.6 : modif txtFrames.Clear() -> txtFrames = UIDList()
		selectedItems = UIDList();
		graphicFrames = UIDList();
		txtFrames = UIDList();

		xpgSuite->GetSelectedItems(txtFrames, graphicFrames, selectedItems);
		int32 itemCount = selectedItems.Length();	
		if(itemCount == 0)
			break;   

		//Calcul  nbre caractères estimés , nbParagraphs,  nbWords, nbLines
		PMString nbPhotos = kNullString;
		nbPhotos.AppendNumber(graphicFrames.Length());
		nbPhotos.SetTranslatable(kFalse);
		SetTextControlData(kXPGUINbImagesTextWidgetId, nbPhotos);	
		
		// Calcul de nombre de caractères estimés , nbParagraphs,  nbWords, nbLines
		int32 nbSignesEstim = 0, nbLinesEstim = 0, nbWordsEstim = 0, nbParaEstim = 0;
		Utils<IXPageUtils>()->GetStatisticsText(txtFrames, nbSignesEstim, nbParaEstim, nbWordsEstim, nbLinesEstim);		
		PMString calibrage = kNullString;
		calibrage.AppendNumber(nbSignesEstim);
		calibrage.SetTranslatable(kFalse);
		
		// Set kXPGUICalibrageTextWidgetId
		SetTextControlData(kXPGUICalibrageTextWidgetId, calibrage);
		
		// Clear title field
		nbSignesEstimString.clear();

		// Titre par defaut prend le nombre de signes
		nbSignesEstimString.AppendNumber(nbSignesEstim);
		nbSignesEstimString.Append(PMString(kXPGUINbSignesTitleKey, PMString::kTranslateDuringCall));
		nbSignesEstimString.SetTranslatable(kFalse);
		
		// Set kXPGUITopicWidgetID
		SetTextControlData(kXPGUITopicWidgetID, nbSignesEstimString);	
	
		//ID de la page
		PMReal leftMost = kMaxInt32;
		UID leftMostSelectedItem = kInvalidUID;
		for(int32 i = 0 ; i < selectedItems.Length() ; ++i)
		{			

			InterfacePtr<IGeometry> itemGeo ( selectedItems.GetRef(i), UseDefaultIID());
			PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();			
			::TransformInnerPointToPasteboard (itemGeo, &leftTop);	
			
			if(leftTop.X() < leftMost)
			{
				leftMostSelectedItem = selectedItems.At(i);
				leftMost = leftTop.X();
			}
		}

		InterfacePtr<IHierarchy> leftMostItemHier (db, leftMostSelectedItem, UseDefaultIID());
		ownerPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID(leftMostItemHier);

		InterfacePtr<ISpread> isASpread (db, ownerPageUID, UseDefaultIID());
		if(isASpread != nil)
		{
			// Le bloc le plus à gauche de l'article est hors page, on renvoie l'ID de la page la plus à gauche dans la planche
			ownerPageUID = isASpread->GetNthPageUID(0);
		}

		// Récuperartion de l'id de la page à partir de Xrail Client
		if(ownerPageUID == kInvalidUID) // Wrong page index
			break;

		InterfacePtr<IXRailPageSlugData> readData (db, ownerPageUID , IID_PAGESLUGDATA);		
        idPage = readData->GetID();
    
		InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());	
		InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
    	xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL()); 		

        InterfacePtr<IDropDownListController> firstFilter (panelControlData->FindWidget(kXPGUIFirstFilterWidgetID), UseDefaultIID());
		InterfacePtr<IStringListData> firstFilterIDs (firstFilter, IID_IIDLIST);
		
		PMString rubID = kNullString;
		if(GetTextControlData(kXPGUIFilterTypeWidgetID) == kXPGUIRubriqueModeKey)
		{
			if(firstFilterIDs->GetStringList().size() > 0)
				rubID = firstFilterIDs->GetStringList()[firstFilter->GetSelected()];
		}
		InterfacePtr<IDropDownListController> recipientList (panelControlData->FindWidget(kXPGUIRecipientListWidgetID), UseDefaultIID());
		InterfacePtr<IStringListControlData> recipientNames (recipientList, UseDefaultIID());	
		recipientNames->Clear();

		InterfacePtr<IStringListData> recipientIDs (recipientList, IID_IIDLIST);
		K2Vector<PMString> theIDsUsers;
	
		// Init recipient list
		IDValueList listUsers;
		listUsers.clear();
				
		K2Vector<PMString> listFullNames;
		
		if(!xrailConnexion->GetAuthorizedStatus(listUsers, listFullNames, "", idPage, ""))
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::InformationAlert(error);	
			break;
		}

		for(int32 j = 0 ; j < listFullNames.size() ; ++j)
			recipientNames->AddString(listFullNames[j]);
		
		for(int32 i = 0 ; i < listUsers.size() ; ++i)			
			theIDsUsers.push_back(listUsers[i].Value());
									
		recipientIDs->SetStringList(theIDsUsers);	

		// Select first if any
		if(!listUsers.empty())	
			recipientList->Select(IDropDownListController::kBeginning);
	
	} while(kFalse);
}

// --------------------------------------------------------------------------------------
// ValidateFields
// --------------------------------------------------------------------------------------
WidgetID XPGUIOrderArtDialogController::ValidateDialogFields(IActiveContext* myContext)
{	
	
	WidgetID badWidgetID = CDialogController::ValidateDialogFields(myContext);
	InterfacePtr<IPanelControlData> panelCtrlData(this, UseDefaultIID());
	
	do
	{
		PMString topic = GetTextControlData(kXPGUITopicWidgetID);
		if(topic.IsEmpty())
		{
			badWidgetID = kXPGUITopicWidgetID;
			break;
		}		

		IControlView* controlView3 = panelCtrlData->FindWidget(kXPGUIRecipientListWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData3(controlView3, UseDefaultIID());		
		if (stringListControlData3->Length() == 0)
		{
			badWidgetID = kXPGUIRecipientListWidgetID;	
			break;
		}	

		IControlView* controlView1 = panelCtrlData->FindWidget(kXPGUIFirstFilterWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData1(controlView1, UseDefaultIID());		
		if (stringListControlData1->Length() == 0)
		{
			badWidgetID = kXPGUIFirstFilterWidgetID;	
			break;
		}

		IControlView* controlView2 = panelCtrlData->FindWidget(kXPGUISecondFilterWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData2(controlView2, UseDefaultIID());		
		if (stringListControlData2->Length() == 0)
		{
			badWidgetID = kXPGUISecondFilterWidgetID;	
			break;
		}

	}while(kFalse);		
	return badWidgetID;
}

// --------------------------------------------------------------------------------------
// ApplyFields
// --------------------------------------------------------------------------------------
void XPGUIOrderArtDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId)
{		
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());	
	
	IDocument * doc = myContext->GetContextDocument();
	IDataBase * db = ::GetDataBase(doc);	
	UIDRef docRef = ::GetUIDRef(doc);

	InterfacePtr<IAssignedDocument> assignedDoc (doc, UseDefaultIID());	
	InterfacePtr<IAssignmentMgr> inCopyMgr (GetExecutionContextSession(), UseDefaultIID());	

	// Initialisation de SOAP 
	InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IWebServices> xrailConnexion (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
	xrailConnexion->SetServerAddress(xpgPrefs->GetXRail_URL());
	
	PMString posX = kNullString,
			 posY = kNullString, 
			 largeur = kNullString, 
			 hauteur = kNullString,
			 rubID = kNullString, 
			 ssrubID = kNullString,
			 topic = kNullString, 
			 creatorLogin = kNullString,
			 recipientLogin	= kNullString,
			 nbPhotos = kNullString, 
			 artID = kNullString, 
			 inCopyArticlePath = kNullString,
			 resultWS = kNullString;	
	do
	{
		//  Calcul des cordonnées des items selectionnés			
		PMPoint leftTopMost (kMaxInt32, kMaxInt32), rightBottomMost (-kMaxInt32, -kMaxInt32);	
		
		for(int32 i = 0 ; i < selectedItems.Length() ; ++i) 
		{			
			
			InterfacePtr<IGeometry> itemGeo ( selectedItems.GetRef(i), UseDefaultIID());

			PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();
			PMPoint rightBottom = itemGeo->GetStrokeBoundingBox().RightBottom();
			 
			::TransformInnerPointToPasteboard (itemGeo, &leftTop);
			::TransformInnerPointToPasteboard (itemGeo, &rightBottom);
	
			if(leftTop.X() < leftTopMost.X())
				leftTopMost.X() = leftTop.X();;
			
			if(leftTop.Y() < leftTopMost.Y())
				leftTopMost.Y() = leftTop.Y();		

			if(rightBottom.X() > rightBottomMost.X())
				rightBottomMost.X() = rightBottom.X();

			if(rightBottom.Y() > rightBottomMost.Y())
				rightBottomMost.Y() = rightBottom.Y();
		}

		// Calcul en millimètres des dimensions
		InterfacePtr<IUnitOfMeasure> uom ((IUnitOfMeasure *)::CreateObject(kMillimetersBoss,IID_IUNITOFMEASURE));
		
		largeur.AppendNumber(uom->PointsToUnits(rightBottomMost.X() - leftTopMost.X()), 3, kTrue, kTrue);
		hauteur.AppendNumber(uom->PointsToUnits(rightBottomMost.Y() - leftTopMost.Y()), 3, kTrue, kTrue);	
		
		// Calcul de l'origine dans le rep?re de la page
		InterfacePtr<IHierarchy> hier (db, selectedItems[0], UseDefaultIID());
		ownerPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID(hier);
		InterfacePtr<IGeometry> ownerPageGeo (db, ownerPageUID, UseDefaultIID());
		::TransformPasteboardPointToInner (ownerPageGeo, &leftTopMost);

		posX.AppendNumber(uom->PointsToUnits(leftTopMost.X()), 3, kTrue, kTrue);
		posY.AppendNumber(uom->PointsToUnits(leftTopMost.Y()), 3, kTrue, kTrue);
		
		// Get topic Story
		topic = GetTextControlData(kXPGUITopicWidgetID);
	
		InterfacePtr<IDropDownListController> recipientList  (panelControlData->FindWidget(kXPGUIRecipientListWidgetID), UseDefaultIID());
		InterfacePtr<IStringListData> recipientLogins(recipientList, IID_IIDLIST);
		if(recipientList->GetSelected()!= IDropDownListController::kNoSelection)
		{
			recipientLogin = recipientLogins->GetStringList()[recipientList->GetSelected()];
		}

		// Récuperer l'utilisateur courant
		InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ILOGINSTRING);
		creatorLogin = gLogin->Get();
		if (gLogin->Get() == "")
				break;

		// On récupere le calibrage
		PMString calibrage = GetTextControlData(kXPGUICalibrageTextWidgetId);
		
		// Nombre de photos
		nbPhotos = GetTextControlData(kXPGUINbImagesTextWidgetId);
		int32 nbPhotosValue = nbPhotos.GetAsNumber();

		 // Rubrique et sous rubrique
		InterfacePtr<IDropDownListController> firstFilter (panelControlData->FindWidget(kXPGUIFirstFilterWidgetID), UseDefaultIID());
		InterfacePtr<IStringListData> firstFilterIDs (firstFilter, IID_IIDLIST);  	
		if(firstFilter->GetSelected() != IDropDownListController::kNoSelection)
		{
			rubID = firstFilterIDs->GetStringList()[firstFilter->GetSelected()];	
		}

		InterfacePtr<IDropDownListController> secondFilter (panelControlData->FindWidget(kXPGUISecondFilterWidgetID), UseDefaultIID());
		InterfacePtr<IStringListData> secondFilterIDs (secondFilter, IID_IIDLIST);	
		if(secondFilter->GetSelected() != IDropDownListController::kNoSelection)
		{
			ssrubID = secondFilterIDs->GetStringList()[secondFilter->GetSelected()];
		}		
		//Recalcule photo/legendes/Credits
		if(!xrailConnexion->CreateStory(creatorLogin, topic, rubID, ssrubID, recipientLogin, 
										idPage, posX, posY, hauteur, 
										largeur, nbPhotosValue, 
										calibrage, artID, inCopyArticlePath, resultWS))
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::InformationAlert(error);
			break;
		}

		if(resultWS != "0")
		{
			// Reset error code and quit
		  	PMString error (kXPGErrFailedToCreateInCopyWS);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, resultWS);  
			CAlert::ErrorAlert(error);
			break;
		}

		// Add all items to incopy
		txtFrames.Append(graphicFrames);

		if (xpgPrefs->GetGestionIDMS() == 1) {
			// Save article data
			InterfacePtr<ICommand> placedArticleDataCmd(CmdUtils::CreateCommand(kXPGSetPlacedArticleDataCmdBoss));
			InterfacePtr<IPlacedArticleData> placedArticleData(placedArticleDataCmd, IID_IPLACEDARTICLEDATA);
			placedArticleData->SetUniqueId(artID);
			placedArticleData->SetStoryFolder(inCopyArticlePath);
			placedArticleData->SetStoryTopic(topic);
			placedArticleDataCmd->SetItemList(txtFrames);
			if (CmdUtils::ProcessCommand(placedArticleDataCmd) != kSuccess)
				break;
		}
		else {
			IDFile assignFile = FileUtils::PMStringToSysFile(inCopyArticlePath);
			FileUtils::AppendPath(&assignFile, artID + ".icma");

			if (Utils<IXPageUtils>()->DoNewAssignment(txtFrames, assignFile, topic, recipientLogin, kTrue, artID, kFalse) != kSuccess)
			{
				break;
			}
		}
	}while(kFalse);
}
