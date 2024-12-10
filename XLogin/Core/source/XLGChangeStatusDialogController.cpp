
#include "VCPlugInHeaders.h"

// Interface includes:
#include "CIdleTask.h"
#include "IIntData.h"
#include "IStringData.h"
#include "ICoreFilename.h"
#include "IStringData.h"
#include "IStringListData.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"
#include "IPanelControlData.h"
#include "IControlView.h"
#include "ITextControlData.h"
#include "IUIDData.h"
#include "IWebServices.h"
#include "IDocument.h"
#include "IActiveContext.h"
#include "IDataBase.h"
#include "ISelectionUtils.h"
#include "ISelectionManager.h"
#include "IHierarchy.h"
#include "IGeometry.h"
#include "IUIDData.h"
#include "IUIDListData.h"
#include "TransformUtils.h"
#include "IPageList.h"
#include "ISpreadLayer.h"
#include "ISpread.h"
#include "IStringListData.h"
#include "IDocument.h"
#include "ILayoutUIUtils.h"
#include "IDocument.h"
#include "Utils.h"
#include "IDocFileHandler.h"
#include "IDocumentUtils.h"

// General includes:
#include "CAlert.h"
#include "CDialogController.h"
#include "SystemUtils.h"
#include "FileUtils.h"
#include "StreamUtil.h"
#include "WidgetID.h"
#include "PMString.h"
#include "ErrorUtils.h"
#include "IAssignedDocument.h"
#include "IAssignment.h"

// Project includes:
#include "IXLGPreferences.h"
#include "XLGID.h"

class XLGChangeStatusDialogController : public CDialogController{
	
	public:
	
        XLGChangeStatusDialogController(IPMUnknown* boss);
        
		virtual void InitializeDialogFields(IActiveContext* dlgContext);		
		virtual WidgetID ValidateDialogFields(IActiveContext* myContext);    	
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);	

	private :
	
		PMString GetAssignmentName(IDocument * doc);
		PMString idStory;
};

CREATE_PMINTERFACE(XLGChangeStatusDialogController, kXLGChangeStatusDialogControllerImpl)

XLGChangeStatusDialogController::XLGChangeStatusDialogController(IPMUnknown* boss) 
                                  :CDialogController(boss), idStory(kNullString)
{}

void XLGChangeStatusDialogController::InitializeDialogFields(IActiveContext* dlgContext) {

	do{
		
		// set artcile Name
		InterfacePtr<IStringData> currentDocName (GetExecutionContextSession(), IID_ICURRENTDOCNAME);
		this->SetTextControlData(kXLGCurrentDocumentTextWidgetID, currentDocName->Get());
		K2Vector<PMString> ColorsList, StatusOrderList,
						   StatusRoleList, StatusRegleList, 
						   CommentaireList;

		IDValueList statusValuesList;
		PMString resultWS;										
		
		InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);
		idStory = currentStory->Get();

		InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ICURRENTUSER);
		PMString currentUser = gLogin->Get();

		InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
        InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
        xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());

		// Initialize status list, getting it from XRail server
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());		
		IControlView* controlView = panelControlData->FindWidget(kXLGStatsListWidgetID);		
		InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());		
		stringListControlData->Clear(kFalse, kFalse);

		if(! xrailWebServices->GetStoryStates(currentUser, idStory, statusValuesList, 
											  ColorsList, StatusOrderList, StatusRoleList, 
											  StatusRegleList, CommentaireList, resultWS)){
			CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));  
			break;
		}        
		
		K2Vector<PMString> statusIdsList;
 		for (int32 i = 0 ; i < statusValuesList.size() ; ++i) {
			statusIdsList.push_back(statusValuesList[i].Key());
			PMString status = statusValuesList[i].Value();		
			stringListControlData->AddString(status, IStringListControlData::kEnd, kFalse, kFalse);	
		}	

		if(statusIdsList.size()>0){
			InterfacePtr<IStringListData> idsListData (GetExecutionContextSession(), IID_IIDSTATUSLIST);
			idsListData->SetStringList(statusIdsList); 
		}
		if (statusValuesList.size() > 0){
				InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());	
				dropDownListController->Select(IDropDownListController::kBeginning,kFalse, kFalse);
		}	
			
	} while (false);
}


WidgetID XLGChangeStatusDialogController::ValidateDialogFields(IActiveContext* dlgContext) 
{	
	WidgetID 		badWidgetID = kInvalidWidgetID;
	return badWidgetID;
}


void XLGChangeStatusDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId){
	
	do {

		InterfacePtr<IStringData> currentGroup(GetExecutionContextSession(), IID_ICURRENTGROUP);
		PMString idGroup = currentGroup->Get();

		InterfacePtr<IStringData> currentStatus (GetExecutionContextSession(), IID_ICURRENTSTATUS);
		PMString idStatus = currentStatus->Get();

		InterfacePtr<IStringData> currentRecepient (GetExecutionContextSession(), IID_ICURRENTERECEPIENT);
		PMString idRecepient =  currentRecepient->Get();

		PMString result;

		InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
        InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
        xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());

		if(!xrailWebServices->StoryStatusChanged(idStory, idStatus, idRecepient, idGroup, result)){
			CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));  
			break;
		}

		InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);
		currentStory->Set("");


	} while (false);	
}


PMString XLGChangeStatusDialogController::GetAssignmentName(IDocument * doc){

    PMString result = kNullString;
	do{		
		 IDataBase * db = ::GetDataBase(doc);
		 if(db == nil)
             break;

		 InterfacePtr<IAssignedDocument> assignedDoc(doc, UseDefaultIID());
		 if(assignedDoc == nil)
             break;
		 
		 InterfacePtr<IAssignment> assignment (db, assignedDoc->GetAssignments()[0], UseDefaultIID());
		 if(assignment == nil)
			 break;

		 result = assignment->GetName();

	}while(kFalse);
    return result;
}