#include "VCPlugInHeaders.h"

// Interface includes:
#include "IControlView.h"
#include "ICommand.h"
#include "ICoreFilename.h"
#include "IPanelControlData.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"
#include "IStringData.h"
#include "IArticleDataModel.h"
#include "IXPageMgrAccessor.h"
#include "IStringListData.h"
#include "IXPageSuite.h"
#include "ISelectionUtils.h"
#include "ISelectionManager.h"
#include "IPMStream.h"
#include "IFormeDataModel.h"
#include "IObserver.h"
#include "ISubject.h"
#include "IActiveContext.h"
#include "IDropDownListController.h"
#include "IUIDData.h"
#include "IUIDListData.h"
#include "IDocument.h"
#include "ILayoutSelectionSuite.h"
#include "IFormeData.h"
#include "IClassIDData.h"
#include "ILayoutUIUtils.h"
#include "IGeometry.h"

// General includes:

#include "CDialogController.h"
#include "IUnitOfMeasure.h"
#include "GenericID.h"
#include "FileUtils.h"
#include "CmdUtils.h"
#include "K2Pair.h"
#include "CAlert.h"
#include "Utils.h"
#include "DBUtils.h"
#include "TransformUtils.h"
#include "ITextModel.h"
#include "IAssignmentUtils.h"
#include "IAttributeStrand.h"
#include "IKeyValueData.h"
#include "IXPageUtils.h"
#include "IXPGPreferences.h"
#include "IWebServices.h"
#include "ILayoutUtils.h"
#include "IHierarchy.h"

// Project includes:
#include "XPageUIUtils.h"
#include "XPGUIID.h"
#include "XPGID.h"

/** 
	Preferences dialog controller
*/
class XPGUIXRailNewFormeDialogController : public CDialogController 
{
	public:

		XPGUIXRailNewFormeDialogController(IPMUnknown* boss) : CDialogController(boss) {}

		virtual void InitializeDialogFields( IActiveContext* dlgContext);
	
		virtual WidgetID ValidateDialogFields( IActiveContext* myContext);

		virtual void ApplyDialogFields( IActiveContext* myContext, const WidgetID& widgetId);

		virtual void ClosingDialog (IActiveContext *myContext);		

	private:
		void EnableDefineCreditLegend(bool16 doEnable);
		void ResetFormeTypes(UIDList formeItems);
		void AddFormeAdornments(UIDList formeItems);
		void RemoveFormeAdornments(UIDList formeItems);
		PMString lastClasseur;
};

CREATE_PMINTERFACE(XPGUIXRailNewFormeDialogController, kXPGUIXRailNewFormeDialogControllerImpl)

/* ApplyFields
*/
void XPGUIXRailNewFormeDialogController::InitializeDialogFields( IActiveContext* dlgContext) 
{

	InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
	do{
		// First get selected forme items
		IDataBase * db = ::GetDataBase(dlgContext->GetContextDocument());		
		UIDList * txtFrameList = new UIDList(db);
		UIDList * photoList = new UIDList(db);	
		UIDList * formeItems = new UIDList(db);
		UIDList * formeStyles = new UIDList(db);
        UIDList * fixedContentFrameList = new UIDList(db);

		InterfacePtr<IXPageSuite> xpgSuite (dlgContext->GetContextSelection(), IID_IXPAGESUITE);
		if(xpgSuite == nil) // nil means no active selection
			break;
			
		xpgSuite->GetSelectedItems(*txtFrameList, *photoList, *formeItems);	

		// Reset forme data in forme items
		UIDList formeDataItems (*txtFrameList);
		formeDataItems.Append(*photoList);
		
		ResetFormeTypes(formeDataItems);

		// Clear forme name
		SetTextControlData(kXPGUIFormeNameWidgetID, kNullString);

		// Clear rubrique file list
		InterfacePtr<IStringListControlData> rubriqueListData (panelCtrlData->FindWidget(kXPGUIRubriqueListWidgetID), UseDefaultIID());
		rubriqueListData->Clear();
		
		// Populate rubrique file list 
		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IFormeDataModel> formeModel (pageMrgAccessor->QueryFormeDataModel()); 
		K2Vector<PMString> classeurList = formeModel->GetClasseurList();	
		int32 selectID = 0;
		for(int32 i = 0 ; i < classeurList.size() ; ++i){
			PMString classeur = classeurList[i];
			if (classeur == lastClasseur) {
				//CAlert::InformationAlert(classeur);
				//CAlert::InformationAlert(lastClasseur);
				//selectID = i;
			}
			classeur.SetTranslatable(kFalse);
			rubriqueListData->AddString(classeur);
		}			

		if(classeurList.size() > 0){
			InterfacePtr<IDropDownListController> rubriqueListController (rubriqueListData, UseDefaultIID());
			rubriqueListController->Select(selectID);
		}	
		else
			CAlert::InformationAlert(PMString(kXPGErrMatchingFileNotFound));		

		
		// Do matching file list initialization through observer
		InterfacePtr<ISubject> rubriqueListSubject (rubriqueListData, UseDefaultIID());
		InterfacePtr<IObserver> dialogObserver (this, IID_INEWFORMEOBSERVER);
		
		dialogObserver->Update(kPopupChangeStateMessage, rubriqueListSubject, IID_ISTRINGLISTCONTROLDATA, nil);				
	
		// Clear photo list
		InterfacePtr<IStringListControlData> photoListStringData (panelCtrlData->FindWidget(kXPGUIPhotoListWidgetID), UseDefaultIID());
		photoListStringData->Clear();
		
		// Fill photo list according to the number of graphic frames found in the selected forme	
		UIDList * creditList = new UIDList(db);		
		UIDList * legendList = new UIDList(db);
		
		for(int32 i = 0 ; i < photoList->Length() ; ++i){
		
			PMString photo (kXPGUIPhotoTextKey, PMString::kTranslateDuringCall);
			photo.AppendNumber(i + 1);
			photo.SetTranslatable(kFalse);
			photoListStringData->AddString(photo);
			
			// Preallocate lists for text frames (credit and legend) associated to photo
			creditList->Append(kInvalidUID); // Same UID as the photo means a credit is required but not set yet, kInvalidUID means it's not required (default)
			legendList->Append(kInvalidUID); // Same UID as the photo means a legend is required but not set yet, kInvalidUID means it's not required (default)
			
			// Update item forme data for drawing forme adornment
			Utils<IXPageUtils>()->SetFormeData(photoList->GetRef(i), kNullString, IFormeData::kPhoto, i);
		}		
	
		// Reset dialog internal data
		InterfacePtr<IUIDData> storyData (this, IID_ISTORYDATA);
		InterfacePtr<IUIDListData> txtFrameListData (this, IID_ITEXTFRAMELIST);
		InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
		InterfacePtr<IUIDListData> creditListData (this, IID_ICREDITLIST);
		InterfacePtr<IUIDListData> legendListData (this, IID_ILEGENDLIST);
		InterfacePtr<IUIDListData> formeItemListData (this, IID_IFORMEITEMLIST);
		InterfacePtr<IUIDListData> formeStylesListData (this, IID_IFORMESTYLESLIST);
        InterfacePtr<IUIDListData> fixedContentFrameListData (this, IID_IFIXEDCONTENTFRAMELIST);

		UIDList textItems (*txtFrameList);
		for(int32 i = 0; i < textItems.Length(); i++ ){
		
			UIDRef storyRef = textItems.GetRef(i);
			Utils<IAssignmentUtils>()->NormalizeTextModelRef(storyRef);
			InterfacePtr<ITextModel> txtModel (storyRef, UseDefaultIID());
			if(txtModel == nil)
				continue;

			InterfacePtr<IAttributeStrand> paraStrand ((IAttributeStrand*) txtModel->QueryStrand(kParaAttrStrandBoss,IID_IATTRIBUTESTRAND));
			TextIndex pos = 0;
			int32 totalLength = txtModel->TotalLength() - 1;
			while(pos < totalLength){
				int32 count = 0;
				UID paraStyle = paraStrand->GetStyleUID(pos, &count);
				if(formeStyles->DoesNotContain(paraStyle))
					formeStyles->Append(paraStyle);
				pos += count;
			}
		}

		formeStylesListData->Set(formeStyles);
		
		storyData->Set(UIDRef::gNull);
		txtFrameListData->Set(txtFrameList);
		
		photoListData->Set(photoList);
		creditListData->Set(creditList);
		legendListData->Set(legendList);
		formeItemListData->Set(formeItems);
        fixedContentFrameListData->Set(fixedContentFrameList);

		// Make observer attach to widgets
		dialogObserver->AutoAttach();
		
		if(photoList->Length() > 0)	{
			EnableDefineCreditLegend(kTrue);
			InterfacePtr<IDropDownListController> photoListController (photoListStringData, UseDefaultIID());
			photoListController->Select(IDropDownListController::kBeginning);			
		}
		else{
			EnableDefineCreditLegend(kFalse);
		}
		
		// Add forme adornments		
		AddFormeAdornments(formeDataItems);

		// Invalidate document for refreshing adornments
		Utils<ILayoutUIUtils>()->InvalidateViews(dlgContext->GetContextDocument());

	} while(kFalse);
}

/* ClosingDialog
*/
void XPGUIXRailNewFormeDialogController::ClosingDialog (IActiveContext* myContext)
{
	// Detach observer
	InterfacePtr<IObserver> dialogObserver (this, IID_INEWFORMEOBSERVER);
	dialogObserver->AutoDetach();
	
	// Remove forme adornments
	InterfacePtr<IUIDListData> txtFrameListData (this, IID_ITEXTFRAMELIST);
	InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
	
	UIDList formeDataItems (*txtFrameListData->GetRef());
	formeDataItems.Append(*photoListData->GetRef());
	
	RemoveFormeAdornments(formeDataItems);	

}


/* ValidateFields
*/
WidgetID XPGUIXRailNewFormeDialogController::ValidateDialogFields( IActiveContext* myContext){

	WidgetID result = kNoInvalidWidgets;
	InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
	// Get forme name
	PMString formeName = GetTextControlData(kXPGUIFormeNameWidgetID);
	if(formeName == kNullString)
		result = kXPGUIFormeNameWidgetID;
	
	if(!XPageUIUtils::IsValidName(formeName)){
		CAlert::InformationAlert(kXPGUIInvalidFormeNameKey);
		return kXPGUIFormeNameWidgetID;
	}
	// Get selected rubrique
	InterfacePtr<IDropDownListController> rubriqueListController (panelCtrlData->FindWidget(kXPGUIRubriqueListWidgetID), UseDefaultIID());
	int32 selectedRubrique = rubriqueListController->GetSelected();
	if(selectedRubrique == IDropDownListController::kNoSelection)
		return kXPGUIRubriqueListWidgetID;	
	
	InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
	int32 nbPhotos = photoListData->GetRef()->Length();
	InterfacePtr<IUIDListData> creditListData (this, IID_ICREDITLIST);
	InterfacePtr<IUIDListData> legendListData (this, IID_ILEGENDLIST);
	IDataBase * db = ::GetDataBase(this);
	for(int32 i = 0 ; i < nbPhotos ; ++i){
		if(creditListData->GetItemRef(i).GetUID() ==  photoListData->GetItemRef(i).GetUID()){
			/*myContext->GetContextSelection()->DeselectAll(nil);
			InterfacePtr<ILayoutSelectionSuite> layoutSuite (myContext->GetContextSelection(), UseDefaultIID());
			if (layoutSuite != nil) {
				layoutSuite->SelectPageItems(UIDList(db, photoListData->GetItemRef(i).GetUID()), Selection::kReplace, Selection::kDontScrollLayoutSelection);
				return kXPGUIDefineCreditPhotoWidgetID;
			}
			else {
				return kXPGUIDefineLegendPhotoWidgetID;
			}*/
			CAlert::InformationAlert("Vous n'avez pas correctement défini les crédits");
			return kXPGUIDefineCreditPhotoWidgetID;
		}
		if(legendListData->GetItemRef(i).GetUID() ==  photoListData->GetItemRef(i).GetUID()){
			/*myContext->GetContextSelection()->DeselectAll(nil);
			InterfacePtr<ILayoutSelectionSuite> layoutSuite (myContext->GetContextSelection(), UseDefaultIID());			
			if (layoutSuite == nil) {
				CAlert::InformationAlert(kXPGUIDefineMatchingErrorKey);
				return kXPGUIDefineLegendPhotoWidgetID;
			}
			layoutSuite->SelectPageItems(UIDList(db,photoListData->GetItemRef(i).GetUID()), Selection::kReplace, Selection::kDontScrollLayoutSelection);				
			return kXPGUIDefineLegendPhotoWidgetID;*/
			CAlert::InformationAlert("Vous n'avez pas correctement défini les légendes");
			return kXPGUIDefineLegendPhotoWidgetID;
		}
	}	
	// Check unicity of forme name 
	InterfacePtr<IXPageMgrAccessor> pageMrgAccessor(GetExecutionContextSession(), UseDefaultIID());
	InterfacePtr<IFormeDataModel> formeModel (pageMrgAccessor->QueryFormeDataModel()); 
	//InterfacePtr<IStringData> classeurData (this, IID_ICLASSEURNAMEDATA);
	InterfacePtr<IStringListControlData> classeurListData(panelCtrlData->FindWidget(kXPGUIRubriqueListWidgetID), UseDefaultIID());
	PMString classeurName = classeurListData->GetString(selectedRubrique);
	//CAlert::InformationAlert(classeurName);
	if(formeModel->DoesFormeExist(formeName, classeurName))	{
		// The forme already exist, ask user if he wants to replace it
		int16 rep = CAlert::ModalAlert(PMString(kXPGUIReplaceFormeKey),PMString(kSDKDefOKButtonApplicationKey),PMString(kSDKDefCancelButtonApplicationKey),kNullString,1,CAlert::eWarningIcon) ;
		if (rep == 2) // if the user clicked on Cancel" button
			return kXPGUIFormeNameWidgetID;
	}
	return result;
}

/* ApplyFields
*/
void XPGUIXRailNewFormeDialogController::ApplyDialogFields( IActiveContext* myContext, const WidgetID& widgetId) 
{
	do
	{
		InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());		
	
		// Get forme name
		PMString formeName = GetTextControlData(kXPGUIFormeNameWidgetID);
		
		InterfacePtr<IUIDListData> txtFrameListData (this, IID_ITEXTFRAMELIST);		
		if(txtFrameListData->GetRef()->IsEmpty()) // Should not occur
			break;

		InterfacePtr<IUIDListData>  photoListData  (this, IID_IPHOTOLIST);
		InterfacePtr<IUIDData>      storyData      (this, IID_ISTORYDATA);
		InterfacePtr<IUIDListData>  creditListData (this, IID_ICREDITLIST);
		InterfacePtr<IUIDListData>  legendListData (this, IID_ILEGENDLIST);
		InterfacePtr<IUIDListData>  formeItemListData (this, IID_IFORMEITEMLIST);
		InterfacePtr<IUIDListData>  formeStylesListData (this, IID_IFORMESTYLESLIST);
        InterfacePtr<IUIDListData>  fixedContentFrameListData  (this, IID_IFIXEDCONTENTFRAMELIST);
			
		// recuperation des extremites de la forme
		PMPoint leftTopMost(kMaxInt32, kMaxInt32), rightBottomMost(-kMaxInt32, -kMaxInt32);
		UIDList newPageItems (*(formeItemListData->GetRef()));
		IDataBase * db = newPageItems.GetDataBase();
		UID ownerPageUID;
		for(int32 i = 0 ; i < newPageItems.Length() ; ++i) {
			
			InterfacePtr<IHierarchy> hier (db, newPageItems[i], UseDefaultIID());
			if(hier == nil)
				continue;
			ownerPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID(hier);
			InterfacePtr<IGeometry> ownerPageGeo (db, ownerPageUID, UseDefaultIID());
			if (ownerPageGeo == nil)
				continue;
		
			
			InterfacePtr<IGeometry> itemGeo ( newPageItems.GetRef(i), UseDefaultIID());
			PMPoint leftTop = itemGeo->GetStrokeBoundingBox().LeftTop();
			PMPoint rightBottom = itemGeo->GetStrokeBoundingBox().RightBottom();
			::TransformInnerPointToPasteboard (itemGeo, &leftTop);
			::TransformInnerPointToPasteboard (itemGeo, &rightBottom);

			if(leftTop.X() < leftTopMost.X())
				leftTopMost.X() = leftTop.X();
			if(leftTop.Y() < leftTopMost.Y())
				leftTopMost.Y() = leftTop.Y();
		
			if(rightBottom.X() > rightBottomMost.X())
				rightBottomMost.X() = rightBottom.X();
			if(rightBottom.Y() > rightBottomMost.Y())
				rightBottomMost.Y() = rightBottom.Y();
		}
		
		// Conversion en millim￨tres des dimensions
		InterfacePtr<IUnitOfMeasure> uom ((IUnitOfMeasure *)::CreateObject(kMillimetersBoss,IID_IUNITOFMEASURE));		

		PMString largeur, hauteur, posx, posy;
		largeur.AppendNumber(uom->PointsToUnits(rightBottomMost.X() - leftTopMost.X()),3,kTrue,kTrue);
		hauteur.AppendNumber(uom->PointsToUnits(rightBottomMost.Y() - leftTopMost.Y()),3,kTrue,kTrue);
		posx.AppendNumber(leftTopMost.X());
		posy.AppendNumber(leftTopMost.Y());

		// Get classeur name
		PMString classeurName = kNullString;
		InterfacePtr<IDropDownListController> classeursListController (panelCtrlData->FindWidget(kXPGUIRubriqueListWidgetID), UseDefaultIID());
		int32 selectedIndex = classeursListController->GetSelected();			
		if(selectedIndex != IDropDownListController::kNoSelection){
			InterfacePtr<IStringListControlData> classeursListData (classeursListController, UseDefaultIID());
			classeurName = classeursListData->GetString(selectedIndex);
		}
		
		// Get matching list styles with tags
		InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);

		// Estimate Numbers of characters 
		int32 nbSignesEstim = 0, nbLinesEstim = 0, nbWordsEstim = 0, nbParaEstim = 0;
		Utils<IXPageUtils>()->GetStatisticsText(*txtFrameListData->GetRef(), nbSignesEstim, nbParaEstim, nbWordsEstim, nbLinesEstim);
		
		// Create the forme
		InterfacePtr<IXPageMgrAccessor> pageMrgAccessor (GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IFormeDataModel>   formeModel (pageMrgAccessor->QueryFormeDataModel()); 
		if(formeModel->CreateForme(formeName, classeurName, formeMatchingStyleData->GetKeyValueList(), 
								   *txtFrameListData->GetRef(), *photoListData->GetRef(), *fixedContentFrameListData->GetRef(),
								   storyData->GetRef(), *creditListData->GetRef(), 
								   *legendListData->GetRef(), *formeItemListData->GetRef(), posx, posy, largeur, hauteur, nbSignesEstim)!= kSuccess)
		{
			
			CAlert::InformationAlert(kXPGUIFailedToCreateFormeKey);
			break;
		}	
		
	} while(kFalse);
}

/* EnableDefineCreditLegend
*/
void XPGUIXRailNewFormeDialogController::EnableDefineCreditLegend(bool16 doEnable)
{
	InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
	IControlView * useCreditWidgetView = panelCtrlData->FindWidget(kXPGUICreditPhotoWidgetID);
	IControlView * useLegendWidgetView = panelCtrlData->FindWidget(kXPGUILegendPhotoWidgetID);	
	useCreditWidgetView->Enable(doEnable);
	useLegendWidgetView->Enable(doEnable);
	IControlView * statusCredit = panelCtrlData->FindWidget(kXPGUIStatusCreditPhotoWidgetID);
	IControlView * statusLegend = panelCtrlData->FindWidget(kXPGUIStatusLegendPhotoWidgetID);	
	IControlView * defineCredit = panelCtrlData->FindWidget(kXPGUIDefineCreditPhotoWidgetID);
	IControlView * defineLegend = panelCtrlData->FindWidget(kXPGUIDefineLegendPhotoWidgetID);
	statusCredit->ShowView(doEnable);
	statusLegend->ShowView(doEnable);
	defineCredit->ShowView(doEnable);
	defineLegend->ShowView(doEnable);
		
	if(!doEnable)
	{
		InterfacePtr<ITriStateControlData> useCreditWidget (useCreditWidgetView, UseDefaultIID());		
		InterfacePtr<ITriStateControlData> useLegendWidget (useLegendWidgetView, UseDefaultIID());
		useCreditWidget->Deselect(kTrue,kFalse);
		useLegendWidget->Deselect(kTrue,kFalse);
	}			
}

/* ResetFormeTypes
*/
void XPGUIXRailNewFormeDialogController::ResetFormeTypes(UIDList formeItems) {
	for(int32 i = 0 ; i < formeItems.Length() ; ++i)
		Utils<IXPageUtils>()->SetFormeData(formeItems.GetRef(i), kNullString, IFormeData::kNoType);
}

/* AddFormeAdornments
*/
void XPGUIXRailNewFormeDialogController::AddFormeAdornments(UIDList formeItems) {
	
	InterfacePtr<ICommand>  addPageItemAdornmentCmd(CmdUtils::CreateCommand(kAddPageItemAdornmentCmdBoss));
	InterfacePtr<IClassIDData>  classIDData(addPageItemAdornmentCmd, UseDefaultIID());
	classIDData->Set(kXPGUIFormeAdornmentBoss);
	addPageItemAdornmentCmd->SetItemList(formeItems);
	CmdUtils::ProcessCommand(addPageItemAdornmentCmd);
}

/* RemoveFormeAdornments
*/
void XPGUIXRailNewFormeDialogController::RemoveFormeAdornments(UIDList formeItems)
{	
	IDataBase * db = formeItems.GetDataBase();
	if(DBUtils::IsValidDataBase(db)) {

		InterfacePtr<ICommand>  removePageItemAdornmentCmd(CmdUtils::CreateCommand(kRemovePageItemAdornmentCmdBoss));
		InterfacePtr<IClassIDData>  classIDData(removePageItemAdornmentCmd, UseDefaultIID());
		classIDData->Set(kXPGUIFormeAdornmentBoss);
		removePageItemAdornmentCmd->SetItemList(formeItems);
		CmdUtils::ProcessCommand(removePageItemAdornmentCmd);
	}
}
