#include "VCPluginHeaders.h"

// Interface includes
#include "IControlView.h"
#include "ISubject.h"
#include "IPanelControlData.h"
#include "IDropDownListController.h"
#include "IStringListControlData.h"
#include "IXPageMgrAccessor.h"
#include "IArticleDataModel.h"
#include "IStringListData.h"
#include "IUIDListData.h"
#include "ITriStateControlData.h"
#include "ILayoutSelectionSuite.h"
#include "ISelectionManager.h"
#include "IXPageSuite.h"
#include "IUIDData.h"
#include "IHierarchy.h"
#include "IMultiColumnTextFrame.h"
#include "IDocument.h"
#include "ILayoutUIUtils.h"
#include "IFormeData.h"
#include "DebugClassUtils.h"
#include "IAssignmentUtils.h"
#include "IKeyValueData.h"

// General includes
#include "SelectionObserver.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "ErrorUtils.h"
#include "IAttributeStrand.h"
#include "IStyleInfo.h"
#include "IDialogMgr.h"
#include "IDialog.h"
#include "IApplication.h"
#include "RsrcSpec.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "IXPageUtils.h"

// Project includes
#include "XPGUIID.h"
#include "XPGID.h"


/** XPGUIXRailNewFormeDialogObserver

	@author Trias Developpement
*/

class XPGUIXRailNewFormeDialogObserver : public ActiveSelectionObserver
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPGUIXRailNewFormeDialogObserver( IPMUnknown* boss );
	/** Destructor
	*/
	virtual ~XPGUIXRailNewFormeDialogObserver() ;
	
	/** 
		Initialises widgets and attaches widget observers.
	*/
	void AutoAttach();

	/** 
		Detaches widget observers. 
	*/
	void AutoDetach();

	/**
		Update is called for all registered observers, and is
		the method through which changes are broadcast. 
		@param theChange this is specified by the agent of change; it can be the class ID of the agent,
		or it may be some specialised message ID.
		@param theSubject this provides a reference to the object which has changed; in this case, the
		widget boss objects that are being observed.
		@param protocol the protocol along which the change occurred.
		@param changedBy this can be used to provide additional information about the change or a reference
		to the boss object that caused the change.
	*/
	void Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy);

protected:

	virtual void HandleSelectionChanged(const ISelectionMessage* message);

private:
	void AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
	void DetachWidget(const InterfacePtr<IPanelControlData>&, const WidgetID& widgetID, const PMIID& interfaceID);
     
	void HandleSelectedPhotoChanged(bool16 updateSelection = kFalse);
	void HandleUseCreditLegendStateChanged(const WidgetID& widget, bool16 isSelected);
    void HandleChkBoxFixedContentStateChanged(const WidgetID& widget, bool16 isSelected);
	void HandleDefineLinkedFrame(const WidgetID& widget);
    
    int32 selectedPhotoIndex;
	IDataBase * db; 

};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGUIXRailNewFormeDialogObserver, kXPGUIXRailNewFormeDialogObserverImpl)

/* Constructor
*/
XPGUIXRailNewFormeDialogObserver::XPGUIXRailNewFormeDialogObserver( IPMUnknown* boss ) :
								ActiveSelectionObserver( boss, IID_INEWFORMEOBSERVER ),
								selectedPhotoIndex(-1) {

	db = ::GetDataBase(this);
}

/* Destructor
*/
XPGUIXRailNewFormeDialogObserver::~XPGUIXRailNewFormeDialogObserver(){

	
}

/* AutoAttach
*/
void XPGUIXRailNewFormeDialogObserver::AutoAttach(){
	ActiveSelectionObserver::AutoAttach();
	do{
		// Initialise widget state.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)
			break;
		
		// Reset selected photo index
		selectedPhotoIndex = -1;

		// Get notification when rubrique changes
		AttachWidget(panelControlData, kXPGUIRubriqueListWidgetID, 		IID_ISTRINGLISTCONTROLDATA);
		AttachWidget(panelControlData, kXPGUIPhotoListWidgetID, 		IID_ISTRINGLISTCONTROLDATA);	 	
		AttachWidget(panelControlData, kXPGUIDefineCreditPhotoWidgetID, IID_IBOOLEANCONTROLDATA);
		AttachWidget(panelControlData, kXPGUIDefineLegendPhotoWidgetID, IID_IBOOLEANCONTROLDATA);
		AttachWidget(panelControlData, kXPGUICreditPhotoWidgetID, 		IID_ITRISTATECONTROLDATA);
		AttachWidget(panelControlData, kXPGUILegendPhotoWidgetID, 		IID_ITRISTATECONTROLDATA);
        AttachWidget(panelControlData, kXPGUIFixedContentWidgetID,      IID_ITRISTATECONTROLDATA);
	} while(false);
	
}

/* AutoDetach
*/
void XPGUIXRailNewFormeDialogObserver::AutoDetach()
{
	ActiveSelectionObserver::AutoDetach();

	do{
		// Detach widget observers.
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (panelControlData == nil)		
			break;		

		DetachWidget(panelControlData, kXPGUIRubriqueListWidgetID, IID_ISTRINGLISTCONTROLDATA);			
		DetachWidget(panelControlData, kXPGUIPhotoListWidgetID, IID_ISTRINGLISTCONTROLDATA);	
 		DetachWidget(panelControlData, kXPGUIDefineCreditPhotoWidgetID, IID_IBOOLEANCONTROLDATA);
		DetachWidget(panelControlData, kXPGUIDefineLegendPhotoWidgetID, IID_IBOOLEANCONTROLDATA);
		DetachWidget(panelControlData, kXPGUICreditPhotoWidgetID, IID_ITRISTATECONTROLDATA);
		DetachWidget(panelControlData, kXPGUILegendPhotoWidgetID, IID_ITRISTATECONTROLDATA);
        DetachWidget(panelControlData, kXPGUIFixedContentWidgetID, IID_ITRISTATECONTROLDATA);
	} while(false);
}

/* Update
*/
void XPGUIXRailNewFormeDialogObserver::Update(const ClassID& theChange, ISubject* theSubject, const PMIID &protocol, void* changedBy)
{	
	ActiveSelectionObserver::Update(theChange, theSubject, protocol, changedBy);
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());	
	if ((protocol == IID_ISTRINGLISTCONTROLDATA) && (theChange == kPopupChangeStateMessage)){
		if(controlView != nil){
			WidgetID widgetID = controlView->GetWidgetID();			
			switch (widgetID.Get())	{			 
				
				case kXPGUIPhotoListWidgetID : 
					HandleSelectedPhotoChanged(kTrue);
					break;

				default: 
					break;
			}
		}
	}
	else if ((protocol == IID_IBOOLEANCONTROLDATA) && (theChange == kTrueStateMessage))
	{
		if(controlView != nil){
			WidgetID widgetID = controlView->GetWidgetID();			
			switch (widgetID.Get())	{			
			 
				case kXPGUIDefineCreditPhotoWidgetID : 
				
				case kXPGUIDefineLegendPhotoWidgetID : 
					HandleDefineLinkedFrame(widgetID); 
					break;

				
				default:
					break;
			}
		}
	}
	else if ((protocol == IID_ITRISTATECONTROLDATA)){
		if(controlView != nil){
			WidgetID widgetID = controlView->GetWidgetID();			
			switch (widgetID.Get())	{

				case kXPGUICreditPhotoWidgetID :

				case kXPGUILegendPhotoWidgetID : 
					HandleUseCreditLegendStateChanged(widgetID, theChange == kTrueStateMessage); break;
				 	break;
                    
                case kXPGUIFixedContentWidgetID :
                    HandleChkBoxFixedContentStateChanged(widgetID, theChange == kTrueStateMessage); break;
                    break;
				
				default:
					break;
			}
		}
	}
}		


/* AttachWidget
*/
void XPGUIXRailNewFormeDialogObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID){
	do{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)		
			break;	

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
			break;
		
		subject->AttachObserver(this, interfaceID, IID_INEWFORMEOBSERVER);
	}
	while(kFalse);
}

/* DetachWidget
*/
void XPGUIXRailNewFormeDialogObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID){
	do{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)	
			break;

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
			break;

		subject->DetachObserver(this, interfaceID, IID_INEWFORMEOBSERVER);

	}while(kFalse);
}

void XPGUIXRailNewFormeDialogObserver::HandleSelectionChanged(const ISelectionMessage* message){
	do{
		InterfacePtr<IXPageSuite> xpgSuite (fCurrentSelection, IID_IXPAGESUITE);
		if(xpgSuite == nil) // nil means no active selection
			break;

		UIDList textFrames, graphicFrames, selectedItems;
		xpgSuite->GetSelectedItems(textFrames, graphicFrames, selectedItems);
        
        // GD 27.04.2023 ++
        InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
        InterfacePtr<ITriStateControlData> chkboxFixedContentWidget (panelControlData->FindWidget(kXPGUIFixedContentWidgetID), UseDefaultIID());
        InterfacePtr<IControlView> controlViewFixedContentWidget (panelControlData->FindWidget(kXPGUIFixedContentWidgetID), UseDefaultIID());
        if (((graphicFrames.Length() == 1) && (textFrames.Length() == 0))
            || ((graphicFrames.Length() == 0) && (textFrames.Length() == 1))) {
            
            controlViewFixedContentWidget->Enable();
            
            InterfacePtr<IUIDListData> fixedContentFrameListData(this, IID_IFIXEDCONTENTFRAMELIST);
            int32 indexFound = -1;
            if (graphicFrames.Length() == 1) {
                indexFound = fixedContentFrameListData->GetRef()->Location(graphicFrames[0]);
            } else {
                indexFound = fixedContentFrameListData->GetRef()->Location(textFrames[0]);
            }
            if (indexFound > -1) {
                chkboxFixedContentWidget->Select(kTrue, kFalse);
            } else {
                chkboxFixedContentWidget->Deselect(kTrue, kFalse);
            }
        } else {
            chkboxFixedContentWidget->Deselect(kTrue, kFalse);
            controlViewFixedContentWidget->Disable();
        }
        // GD 27.04.2023 --
        
		if(graphicFrames.Length() != 1)
			break;

		// Get selected graphic frame index
		InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
		int32 index = photoListData->GetRef()->Location(graphicFrames[0]);
		if(index == -1)
			break;

		// Update selected index in photo list
        // GD 27.04.2023 ligne ici remontee plus haut : InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		InterfacePtr<IDropDownListController> photoListController (panelControlData->FindWidget(kXPGUIPhotoListWidgetID), UseDefaultIID());
		photoListController->Select(index, kTrue, kFalse);

		// Update widgets
		HandleSelectedPhotoChanged();

	} while(kFalse);
}

void XPGUIXRailNewFormeDialogObserver::HandleSelectedPhotoChanged(bool16 updateSelection){

	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	do{
		InterfacePtr<IDropDownListController> photoListController (panelControlData->FindWidget(kXPGUIPhotoListWidgetID), UseDefaultIID());	
		selectedPhotoIndex = photoListController->GetSelected();
		if(selectedPhotoIndex == IDropDownListController::kNoSelection)
			break;

		InterfacePtr<IUIDListData> photoListData(this, IID_IPHOTOLIST);
		InterfacePtr<IUIDListData> creditListData(this, IID_ICREDITLIST);
		InterfacePtr<IUIDListData> legendListData(this, IID_ILEGENDLIST);

		

		// Get related photo UID
		UID photoUID = photoListData->GetItemRef(selectedPhotoIndex).GetUID();
		UID creditUID = creditListData->GetItemRef(selectedPhotoIndex).GetUID();
		UID legendUID = legendListData->GetItemRef(selectedPhotoIndex).GetUID();

		// Set widgets state
		InterfacePtr<ITriStateControlData> useCreditWidget (panelControlData->FindWidget(kXPGUICreditPhotoWidgetID), UseDefaultIID());
		InterfacePtr<ITriStateControlData> useLegendWidget (panelControlData->FindWidget(kXPGUILegendPhotoWidgetID), UseDefaultIID());
		IControlView * statusCredit = panelControlData->FindWidget(kXPGUIStatusCreditPhotoWidgetID);
		IControlView * statusLegend = panelControlData->FindWidget(kXPGUIStatusLegendPhotoWidgetID);
		IControlView * defineCredit = panelControlData->FindWidget(kXPGUIDefineCreditPhotoWidgetID);
		IControlView * defineLegend = panelControlData->FindWidget(kXPGUIDefineLegendPhotoWidgetID);
		
		if(creditUID == kInvalidUID){
			useCreditWidget->Deselect(kTrue, kFalse);
			statusCredit->HideView();
			statusCredit->SetRsrcID(kXPGUIKOIconResourceID);	
			defineCredit->HideView();
		}
		else{
			useCreditWidget->Select(kTrue, kFalse);
			statusCredit->ShowView();
			defineCredit->ShowView();
			if(creditUID != photoUID)
				statusCredit->SetRsrcID(kXPGUIOKIconResourceID);	
			else
				statusCredit->SetRsrcID(kXPGUIKOIconResourceID);

			statusCredit->Invalidate();
		}
		if(legendUID == kInvalidUID){
			useLegendWidget->Deselect(kTrue, kFalse);
			statusLegend->HideView();
			statusLegend->SetRsrcID(kXPGUIKOIconResourceID);	
			defineLegend->HideView();
		}
		else{
			useLegendWidget->Select(kTrue, kFalse);
			statusLegend->ShowView();
			defineLegend->ShowView();
			
			if(legendUID != photoUID)
				statusLegend->SetRsrcID(kXPGUIOKIconResourceID);	
			else
				statusLegend->SetRsrcID(kXPGUIKOIconResourceID);
			statusLegend->Invalidate();
		}
		// Select graphic frame that corresponds to the photo
		if(updateSelection)	{
			fCurrentSelection->DeselectAll(nil);
			InterfacePtr<ILayoutSelectionSuite> layoutSuite (fCurrentSelection, UseDefaultIID());			   
			layoutSuite->SelectPageItems(UIDList(photoListData->GetDataBase(), photoUID), Selection::kReplace, Selection::kDontScrollLayoutSelection);
		}

	}while(kFalse);
}

void XPGUIXRailNewFormeDialogObserver::HandleUseCreditLegendStateChanged(const WidgetID& widget, bool16 isSelected){
	
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	do{							
		// Get selected photo UID
		InterfacePtr<IUIDListData> photoListData(this, IID_IPHOTOLIST);
		UID photoUID = photoListData->GetItemRef(selectedPhotoIndex).GetUID();

		InterfacePtr<IUIDListData> frameListData (nil);
		if(widget == kXPGUICreditPhotoWidgetID)
			frameListData = InterfacePtr<IUIDListData> (this, IID_ICREDITLIST);			
		else
			frameListData = InterfacePtr<IUIDListData> (this, IID_ILEGENDLIST);				
		// Update widgets
		HandleSelectedPhotoChanged();
		if(isSelected){			
			if(selectedPhotoIndex != -1){
				UIDList* copy = new UIDList(*frameListData->GetRef());
				copy->Replace(selectedPhotoIndex,photoUID); // Set linked frame UID to the photo UID, which means a linked frame must be set
				frameListData->Set(copy);
			}
		}			
		else{
			if(selectedPhotoIndex != -1){

				UIDList * copy = new UIDList(*frameListData->GetRef());
				UID oldCreditOrLegendStoryUID = frameListData->GetRef()->At(selectedPhotoIndex);
				copy->Replace(selectedPhotoIndex,kInvalidUID);
				frameListData->Set(copy);

				// Update forme data so that the adormnment will not be drawn
				IDataBase * dbFrame = frameListData->GetRef()->GetDataBase();

				InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);				
			
				if(formeMatchingStyleData && oldCreditOrLegendStoryUID != kInvalidUID && oldCreditOrLegendStoryUID != photoListData->GetRef()->At(selectedPhotoIndex)){
					Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(db,oldCreditOrLegendStoryUID), kNullString, IFormeData::kNoType, formeMatchingStyleData->GetKeyValueList());
					InterfacePtr<IDocument> doc(dbFrame, dbFrame->GetRootUID(), UseDefaultIID());
					Utils<ILayoutUIUtils>()->InvalidateViews(doc);
				}		
			}
		}
		// Update widgets
		HandleSelectedPhotoChanged();		

	}while(kFalse);
}

void XPGUIXRailNewFormeDialogObserver::HandleDefineLinkedFrame(const WidgetID& widget){

	do{
		if(selectedPhotoIndex == -1)
			break;

		InterfacePtr<IXPageSuite> xpgSuite (fCurrentSelection, IID_IXPAGESUITE);
		if(xpgSuite == nil) // nil means no active selection
			break;

		UIDList textFrames, graphicFrames, selectedItems;
		xpgSuite->GetSelectedItems(textFrames, graphicFrames, selectedItems);

		// First check whether selection contains only one text frame and one graphic frame
		if(textFrames.Length() != 1 || graphicFrames.Length() != 1){
			CAlert::InformationAlert(kXPGUIInvalidLinkedFrameSelectionKey);
			break;
		}

		// Then check whether selection belongs to the forme being created
		InterfacePtr<IUIDListData> txtFrameListData (this, IID_ITEXTFRAMELIST);
		InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
		
		if(txtFrameListData->GetRef()->Location(textFrames[0]) == -1 || photoListData->GetRef()->Location(graphicFrames[0]) == -1){
			CAlert::InformationAlert(kXPGUISelectionDontBelongToFormeKey);
			break;
		}

		// Finally check whether it is not already defined as credit or legend, or even a story		
		InterfacePtr<IHierarchy> txtFrameHier (textFrames.GetRef(0), UseDefaultIID());
		InterfacePtr<IMultiColumnTextFrame> txtFrame (textFrames.GetDataBase(), txtFrameHier->GetChildUID(0), UseDefaultIID());
		UID selectedStoryUID = txtFrame->GetTextModelUID();

		InterfacePtr<IUIDData> storyData (this, IID_ISTORYDATA);
		if(selectedStoryUID == storyData->GetItemUID()){
			CAlert::InformationAlert(kXPGUITextFrameAlreadyDefined2Key);
			break;
		}
	
		InterfacePtr<IUIDListData> creditListData (this, IID_ICREDITLIST);
		InterfacePtr<IUIDListData> legendListData (this, IID_ILEGENDLIST);

		if( creditListData->GetRef()->Location(selectedStoryUID) != -1 || legendListData->GetRef()->Location(selectedStoryUID) != -1){
			CAlert::InformationAlert(kXPGUITextFrameAlreadyDefinedKey);
			break;
		}

		// Everything is OK, set the credit or legend
		IDataBase * dbFrame = textFrames.GetDataBase();

		if(widget == kXPGUIDefineCreditPhotoWidgetID){	

			UIDList * copy = new UIDList(*creditListData->GetRef());
			UID oldCreditStoryUID = creditListData->GetRef()->At(selectedPhotoIndex);
			copy->Replace(selectedPhotoIndex,selectedStoryUID);
			creditListData->Set(copy);
			
			InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);

			// Update items forme data for drawing forme adornments
			if(oldCreditStoryUID != kInvalidUID && oldCreditStoryUID != photoListData->GetRef()->At(selectedPhotoIndex))
				Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, oldCreditStoryUID), kNullString, IFormeData::kNoType, formeMatchingStyleData->GetKeyValueList());
			
			Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, selectedStoryUID), kNullString, IFormeData::kCredit, formeMatchingStyleData->GetKeyValueList(), selectedPhotoIndex);
		}
		else{

			UIDList * copy = new UIDList(*legendListData->GetRef());
			UID oldLegendStoryUID = legendListData->GetRef()->At(selectedPhotoIndex);
			copy->Replace(selectedPhotoIndex,selectedStoryUID);
			legendListData->Set(copy);

			InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);

			// Update items forme data for drawing forme adornments
			if(oldLegendStoryUID != kInvalidUID && oldLegendStoryUID != photoListData->GetRef()->At(selectedPhotoIndex))
				Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, oldLegendStoryUID), kNullString, IFormeData::kNoType, formeMatchingStyleData->GetKeyValueList());
			
			Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, selectedStoryUID), kNullString, IFormeData::kLegend, formeMatchingStyleData->GetKeyValueList(), selectedPhotoIndex);
		}		

		// Update widgets
		HandleSelectedPhotoChanged();

		// Invalidate document so that forme adornments get paint			
		InterfacePtr<IDocument> doc (dbFrame, dbFrame->GetRootUID(), UseDefaultIID());
		Utils<ILayoutUIUtils>()->InvalidateViews(doc);

	} while(kFalse);
}

// GD 27.04.2023
void XPGUIXRailNewFormeDialogObserver::HandleChkBoxFixedContentStateChanged(const WidgetID& widget, bool16 isSelected){
    
    do{
        InterfacePtr<IXPageSuite> xpgSuite (fCurrentSelection, IID_IXPAGESUITE);
        if(xpgSuite == nil) // nil means no active selection
            break;
        
        UIDList textFrames, graphicFrames, selectedItems;
        xpgSuite->GetSelectedItems(textFrames, graphicFrames, selectedItems);
        
        // First check whether selection contains only one text frame OR one graphic frame
        if (((graphicFrames.Length() == 1) && (textFrames.Length() == 0))
            || ((graphicFrames.Length() == 0) && (textFrames.Length() == 1))) {
            // ok
        } else {
            CAlert::InformationAlert("Vous devez selectionner un seul bloc texte ou image");
            break;
        }
        
        // Then check whether selection belongs to the forme being created
        InterfacePtr<IUIDListData> txtFrameListData (this, IID_ITEXTFRAMELIST);
        InterfacePtr<IUIDListData> photoListData (this, IID_IPHOTOLIST);
        // La verif est faite ci-dessous en fonction du type de bloc
        
        // Recupération de l'UID de la selected frame, son type (texte ou image) et son (eventuel) currentPhotoIndex
        int16 selectedFrameType = IFormeData::kNoType;
        UID selectedFrameUID = kInvalidUID;
        UIDRef selectedFrameUIDRef = kInvalidUIDRef;
        if (textFrames.Length() == 1) {
            if(txtFrameListData->GetRef()->Location(textFrames[0]) == -1){
                CAlert::InformationAlert(kXPGUISelectionDontBelongToFormeKey);
                break;
            }
            selectedFrameType = IFormeData::kArticle;
            selectedFrameUID = textFrames[0];
            selectedFrameUIDRef = textFrames.GetRef(0);
        } else {
            if(photoListData->GetRef()->Location(graphicFrames[0]) == -1){
                CAlert::InformationAlert(kXPGUISelectionDontBelongToFormeKey);
                break;
            }
            selectedFrameType = IFormeData::kPhoto;
            selectedFrameUID = graphicFrames[0];
            selectedFrameUIDRef = graphicFrames.GetRef(0);
        }
        IDataBase * dbFrame = selectedItems.GetDataBase();
        
        InterfacePtr<IFormeData> itemFormeData(selectedFrameUIDRef, UseDefaultIID());
        int currentPhotoIndex = itemFormeData->GetPhotoIndex();
        
        InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);
        InterfacePtr<IUIDListData> fixedContentFrameListData (this, IID_IFIXEDCONTENTFRAMELIST);
        UIDList * copy = new UIDList(*fixedContentFrameListData->GetRef());
        if(isSelected){
            // ajout de l'UID de la frame dans la fixedContentFrameListData
            copy->Append(selectedFrameUID);
            // SetFormeDataOnStory en passant le photoIndex recupéré plus haut et le nouveau type kFixedContent
            Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, selectedFrameUID), kNullString, IFormeData::kFixedContent, formeMatchingStyleData->GetKeyValueList(), currentPhotoIndex);
        } else {
            // suppression de l'UID de la frame dans la fixedContentFrameListData
            copy->Remove(copy->Location(selectedFrameUID));
            // SetFormeDataOnStory en passant le photoIndex recupéré plus haut et le type kArticle ou kPhoto en
            // fonction du type de bloc, texte ou image.
            Utils<IXPageUtils>()->SetFormeDataOnStory(UIDRef(dbFrame, selectedFrameUID), kNullString, selectedFrameType, formeMatchingStyleData->GetKeyValueList(), currentPhotoIndex);
        }
        fixedContentFrameListData->Set(copy);
        
        // Invalidate document so that forme adornments get paint
        InterfacePtr<IDocument> doc (dbFrame, dbFrame->GetRootUID(), UseDefaultIID());
        Utils<ILayoutUIUtils>()->InvalidateViews(doc);
        
    }while(kFalse);
}
