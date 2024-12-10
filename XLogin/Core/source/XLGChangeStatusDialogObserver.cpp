//========================================================================================
//  
//  $File: $
//  
//  Owner: Badr Nasser
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IControlView.h"
#include "IPanelControlData.h"
#include "ISubject.h"
// General includes:
#include "CDialogObserver.h"
#include "ITriStateControlData.h"
#include "IDropDownListController.h"
#include "IStringListControlData.h"
#include "IStringData.h"
#include "IStringListData.h"
#include "CAlert.h"
// Project includes:
#include "IXLGPreferences.h"
#include "IWebServices.h"
#include "XLGID.h"
#include "K2Vector.h"
#include "K2Vector.tpp"
#include "KeyValuePair.h"

typedef KeyValuePair<PMString, PMString> IDValuePair;
typedef K2Vector<IDValuePair > IDValueList;

class XLGChangeStatusDialogObserver : public CDialogObserver
{
	public:

		XLGChangeStatusDialogObserver(IPMUnknown* boss) : CDialogObserver(boss){

			idStory =kNullString;
			currentUser=kNullString;
			statusSelectedID=kNullString; 
			groupSelectedID=kNullString;
			recepientSelectedID=kNullString;

			usersValuesList.clear();
			groupsValuesList.clear();
			statusValuesList.clear();		
		}

		/** Destructor. */
		virtual ~XLGChangeStatusDialogObserver() {}
		virtual void AutoAttach();
		virtual void AutoDetach();
		virtual void Update(const ClassID& theChange, ISubject* theSubject,	const PMIID& protocol, void* changedBy);

	private :

		void AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
		void DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID);
		void DisplayUsersAndGroupsWidget (bool16 visble);

		PMString idStory, recepientSelectedID, statusSelectedID, groupSelectedID, currentUser;

		IDValueList groupsValuesList;
		IDValueList statusValuesList;
		IDValueList usersValuesList;	

	 protected:		
		void handleSelectionChangedStatus();
		void handleSelectionChangedGroup();
		void handleSelectionChangedUser();

};
CREATE_PMINTERFACE(XLGChangeStatusDialogObserver, kXLGChangeStatusDialogObserverImpl)

/* AutoAttach
*/
void XLGChangeStatusDialogObserver::AutoAttach(){
	CDialogObserver::AutoAttach();
	do{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if(!panelControlData) 
			break;		

		this->AttachWidget(panelControlData, kXLGStatsListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXLGGroupsStatusListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->AttachWidget(panelControlData, kXLGRecipientStatusListWidgetID, IID_ISTRINGLISTCONTROLDATA);

		this->handleSelectionChangedStatus();
		this->handleSelectionChangedGroup();
		this->handleSelectionChangedUser();

		this->DisplayUsersAndGroupsWidget(kFalse);

	} while (kFalse);
}

/* AutoDetach
*/
void XLGChangeStatusDialogObserver::AutoDetach()
{
	CDialogObserver::AutoDetach();
	do{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());		
		if(!panelControlData) 
			break;	

		this->DetachWidget(panelControlData, kXLGStatsListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXLGGroupsStatusListWidgetID, IID_ISTRINGLISTCONTROLDATA);
		this->DetachWidget(panelControlData, kXLGRecipientStatusListWidgetID, IID_ISTRINGLISTCONTROLDATA);

	} while (kFalse);
}
/* AttachWidget
*/
void XLGChangeStatusDialogObserver::AttachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)
			break;		

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)
			break;
	
		subject->AttachObserver(this, interfaceID);
	}
	while (false);
}

/* DetachWidget
*/
void XLGChangeStatusDialogObserver::DetachWidget(const InterfacePtr<IPanelControlData>& panelControlData, const WidgetID& widgetID, const PMIID& interfaceID)
{
	do
	{
		IControlView* controlView = panelControlData->FindWidget(widgetID);
		if (controlView == nil)		
			break;	

		InterfacePtr<ISubject> subject(controlView, UseDefaultIID());
		if (subject == nil)		
			break;
	
		subject->DetachObserver(this, interfaceID);
	}
	while (false);
}


/* Update
*/
void XLGChangeStatusDialogObserver::Update(const ClassID& theChange,ISubject* theSubject,
										   const PMIID& protocol,	void* changedBy){
	CDialogObserver::Update(theChange, theSubject, protocol, changedBy);	
	InterfacePtr<IControlView> controlView(theSubject, UseDefaultIID());
	do{	
		if (theChange == kPopupChangeStateMessage){

			WidgetID widgetID = controlView->GetWidgetID();					
			switch (widgetID.Get())		{
			
				case kXLGStatsListWidgetID:
						handleSelectionChangedStatus();
						break;
					
				case kXLGGroupsStatusListWidgetID:
						handleSelectionChangedGroup();
						break;			
				
				case kXLGRecipientStatusListWidgetID:					
						handleSelectionChangedUser();						
					    break;		
								
				default:
					break;
			}
		}
		
	
	}while (kFalse);
}

void XLGChangeStatusDialogObserver::DisplayUsersAndGroupsWidget (bool16 visible){
	
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());	
	do{
		IControlView * groupsListControlView = panelControlData->FindWidget(kXLGGroupsStatusListWidgetID); 
		IControlView * recipientListControlView = panelControlData->FindWidget(kXLGRecipientStatusListWidgetID);
		IControlView * groupsTextControlView = panelControlData->FindWidget(kXLGGroupsStatusTextWidgetID);
		IControlView * recipientTextControlView = panelControlData->FindWidget(kXLGRecipientStatusTextWidgetID);
																			  
		if(visible){	
				groupsListControlView->HideView ();  
				recipientListControlView->HideView ();
				groupsTextControlView->HideView ();   
				recipientTextControlView->HideView ();
			
		}else{	
				groupsListControlView->ShowView();  
				recipientListControlView->ShowView ();
				groupsTextControlView->ShowView();   
				recipientTextControlView->ShowView ();	
		}
	}while(kFalse);
}


void  XLGChangeStatusDialogObserver:: handleSelectionChangedStatus() {

	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	groupsValuesList.clear();
	statusValuesList.clear();
	do{

		K2Vector<PMString> ColorsList, StatusOrderList, StatusRoleList, StatusRegleList, CommentaireList ;	
		PMString resultWS;										
		
		InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);
		idStory = currentStory->Get();
		
		InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ICURRENTUSER);
		currentUser = gLogin->Get();

	
		InterfacePtr<IStringListData> idsListData (GetExecutionContextSession(), IID_IIDSTATUSLIST);
		if(idsListData && idsListData->GetStringList().size() > 0){

			InterfacePtr<IDropDownListController> statusFirstFilter (panelControlData->FindWidget(kXLGStatsListWidgetID), UseDefaultIID());
			int32 statusSelectedIndex = statusFirstFilter->GetSelected() ;
			if(statusSelectedIndex == IDropDownListController::kNoSelection)
				break;

			statusSelectedID = idsListData->GetStringList()[statusSelectedIndex];	

			InterfacePtr<IStringData> currentStatus (GetExecutionContextSession(), IID_ICURRENTSTATUS);
			currentStatus->Set(statusSelectedID);

			PMString  result;

			InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
			xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());

			if(! xrailWebServices->GetGroupsStatusList(idStory, currentUser, statusSelectedID, groupsValuesList, result)){			
				CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));  
				break;		
			}
			// Initialize groups list, getting it from XRail server
			
			IControlView* controlView = panelControlData->FindWidget(kXLGGroupsStatusListWidgetID);		
			InterfacePtr<IStringListControlData> stringListControlData(controlView, UseDefaultIID());	
			stringListControlData->Clear(kFalse, kFalse);
			
#if !TEC
		// Initialize groups list whith (EveryBody, Tous) 
			groupsValuesList.insert(groupsValuesList.begin(),IDValuePair(PMString("tous", PMString::kTranslateDuringCall), 
																		 PMString(kXLGEveryBodyTextKey, PMString::kTranslateDuringCall)));
#endif
			for (int32 i = 0 ; i < groupsValuesList.size() ; ++i) {
				PMString group = groupsValuesList[i].Value();		
				stringListControlData->AddString(group, IStringListControlData::kEnd, kFalse, kFalse);	
			}	
		
			if (groupsValuesList.size() > 0){
					InterfacePtr<IDropDownListController> dropDownListController(controlView, UseDefaultIID());	
					dropDownListController->Select(IDropDownListController::kBeginning,kFalse, kFalse);
			}		
		}
	}while(kFalse);

	this->handleSelectionChangedGroup();
	
}
void  XLGChangeStatusDialogObserver:: handleSelectionChangedGroup() 
{
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	usersValuesList.clear();

	do{
		if (groupsValuesList.size() > 0){

			InterfacePtr<IDropDownListController> groupFirstFilter (panelControlData->FindWidget(kXLGGroupsStatusListWidgetID), UseDefaultIID());
			int32 groupSelectedIndex = groupFirstFilter->GetSelected() ;
			if(groupSelectedIndex == IDropDownListController::kNoSelection)
				break;

			groupSelectedID = groupsValuesList[groupSelectedIndex].Key();
			InterfacePtr<IStringData> groupStatus (GetExecutionContextSession(), IID_ICURRENTGROUP);
			groupStatus->Set(groupSelectedID);

			PMString result;
			InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
			InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
			xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());

			if(! xrailWebServices->GetUsersStatusList(currentUser, idStory, statusSelectedID, groupSelectedID, usersValuesList, result)){
				CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));  
				break;			
			}        

			IControlView* controlUsersView = panelControlData->FindWidget(kXLGRecipientStatusListWidgetID);		
			InterfacePtr<IStringListControlData> usersListControlData(controlUsersView, UseDefaultIID());		
			usersListControlData->Clear(kFalse, kFalse);
#if !TEC			
			usersValuesList.insert(usersValuesList.begin(),IDValuePair(PMString("tous",PMString::kTranslateDuringCall), 
																	   PMString(kXLGEveryBodyTextKey, PMString::kTranslateDuringCall)));

#endif
			for (int32 i = 0 ; i < usersValuesList.size() ; ++i) {
				PMString user = usersValuesList[i].Value();		
				usersListControlData->AddString(user, IStringListControlData::kEnd, kFalse, kFalse);	
			}	
		
			if (usersValuesList.size() > 0){
					InterfacePtr<IDropDownListController> dropDownListController(controlUsersView, UseDefaultIID());	
					dropDownListController->Select(IDropDownListController::kBeginning, kFalse, kFalse);
			}	
		}

	}while(kFalse);
	this->handleSelectionChangedUser();

}

void  XLGChangeStatusDialogObserver:: handleSelectionChangedUser() 
{
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	
	do{
		if(usersValuesList.size()>0){
			InterfacePtr<IDropDownListController> userFirstFilter (panelControlData->FindWidget(kXLGRecipientStatusListWidgetID), UseDefaultIID());
			int32 userSelectedIndex = userFirstFilter->GetSelected() ;
			if(userSelectedIndex == IDropDownListController::kNoSelection)
				break;

			recepientSelectedID = usersValuesList[userSelectedIndex].Key();
			InterfacePtr<IStringData> currentRecepient (GetExecutionContextSession(), IID_ICURRENTERECEPIENT);
			currentRecepient->Set(recepientSelectedID);	
		}	

	}while(kFalse);
}
//  Code generated by DollyXs code generator
