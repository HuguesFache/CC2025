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
#include "IActiveContext.h"
// General includes:
#include "CDialogController.h"
// Project includes:
#include "XLGID.h"
#include "IXLGPreferences.h"
#include "IWebServices.h"
#include "CAlert.h"
#include "IStringData.h"
#include "TransformUtils.h"
#include "IInCopyBridgeUtils.h"
#include "ErrorUtils.h"
#include "InCopyBridgeUIID.h"
#include "IItemLockData.h"
#include "IInCopyBridgeUtils.h"
#include "Utils.h"
#include "IUserInfoUtils.h"


/** XLGDialogController
	Methods allow for the initialization, validation, and application of dialog widget
	values.
	Implements IDialogController based on the partial implementation CDialogController.

	
	@ingroup xlogin
*/
class XLGDialogController : public CDialogController
{
	public:
		/** Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XLGDialogController(IPMUnknown* boss) : CDialogController(boss), firstInit(kTrue) {}

		/** Destructor.
		*/
		virtual ~XLGDialogController() {}

		/** Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
	    virtual void InitializeDialogFields(IActiveContext* dlgContext);

		/** Validate the values in the widgets.
			By default, the widget with ID kOKButtonWidgetID causes
			ValidateFields to be called. When all widgets are valid,
			ApplyFields will be called.
			@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

		*/
	    virtual WidgetID ValidateDialogFields(IActiveContext* myContext);


		/** Retrieve the values from the widgets and act on them.
			@param widgetId identifies the widget on which to act.
		*/
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);

        //void UserCancelled() ;
        
		bool16 firstInit;
};

CREATE_PMINTERFACE(XLGDialogController, kXLGDialogControllerImpl)

/* ApplyFields
*/
void XLGDialogController::InitializeDialogFields(IActiveContext* dlgContext)
{
	CDialogController::InitializeDialogFields(dlgContext);
	// Put code to initialize widget values here.
}

/* ValidateFields
*/
WidgetID XLGDialogController::ValidateDialogFields(IActiveContext* myContext)
{
	// Put code to validate widget values here.
	WidgetID 		badWidgetID = kInvalidWidgetID;

    PMString userID, WSResult;
    PMString theUser,thePassword;
    do{

  	    theUser = this->GetTextControlData(kXLGLoginUserWidgetID);
	    thePassword = this->GetTextControlData(kXLGPasswordUserWidgetID);

        if (theUser.NumUTF16TextChars() == 0) {
			badWidgetID = kXLGLoginDialogResourceID;
			CAlert::InformationAlert(PMString(kXLGUIErrorBlankLoginKey));
			break;
		}
#if !TEC
		if (thePassword.NumUTF16TextChars () == 0) {
			badWidgetID = kXLGPasswordUserWidgetID;
			CAlert::InformationAlert(PMString(kXLGUIErrorBlankPasswordKey));
			break;
		}
#endif

    } while (kFalse);    

    InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
   	InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
	xrailWebServices->SetServerAddress(xlgrefs->GetXRail_URL());

	bool16 connexionOK = xrailWebServices->VerifLogin(theUser, thePassword, userID, WSResult);
    
    InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ICURRENTUSER);
	
	if(!connexionOK)
		CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));

    if(WSResult == "loginOK")
	{                      
     	gLogin->Set(theUser);
		
		// Set incopy user name
		Utils<IUserInfoUtils>()->SetInCopyUsername(gLogin->Get());

        InterfacePtr<IStringData> currentStory (GetExecutionContextSession(), IID_ICURRENTSTORY);		
		if(currentStory->Get() != "")
		{
			connexionOK = xrailWebServices->StoryLockChanged(currentStory->Get(), theUser);
			if(!connexionOK)
				CAlert::ErrorAlert(PMString(kXLGUIErrorWebServiceKey));        
		}
    }
	else if(WSResult == "badPassword")
	{
		badWidgetID = kXLGPasswordUserWidgetID;
		CAlert::ErrorAlert(PMString(kXLGUIErrorBadPasswordKey));
	}
	else if(WSResult == "privilegemissing")
	{
		badWidgetID = kXLGPasswordUserWidgetID;
		CAlert::ErrorAlert(PMString(kXLGUIErrorPrivilegeKey));
	}
	else if(WSResult == "unknownUser") 
	{
		badWidgetID = kXLGLoginDialogResourceID;
		CAlert::ErrorAlert(PMString(kXLGUIErrorUnknownUserKey));
	}
	else
	{
		badWidgetID = kXLGLoginDialogResourceID;
		CAlert::ErrorAlert(PMString(kXLGUIErrorSOAPKey));
	}
	return badWidgetID;
}

/* ApplyFields
*/
void XLGDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId)
{
	// TODO add code that gathers widget values and applies them.
}

/*
// La commande kQuitCmdBoss plante InCopy lorqu'un article est ouvert

/*void XLGDialogController::UserCancelled() 
{

	// User cancelled  => quit InDesign
	/*InterfacePtr<ICommand> QuitCmd (CmdUtils::CreateCommand(kQuitCmdBoss));
	ErrorCode status = CmdUtils::ScheduleCommand(QuitCmd);
	ASSERT(status == kSuccess);
	


}
*/
//  Code generated by DollyXs code generator
