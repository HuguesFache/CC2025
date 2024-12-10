
#include "VCPlugInHeaders.h"

// Interface includes:
#include "CIdleTask.h"
#include "IIntData.h"
#include "IStringData.h"
#include "IIntData.h"
#include "IXPubConnexion.h"
#include "IBoolData.h"
#include "IXPubClientUtils.h"

// General includes:
#include "CAlert.h"
#include "CDialogController.h"
#include "UIDList.h"
#include "ErrorUtils.h"
#include "Utils.h"
#include "IActiveContext.h" // GD 2015

// Project includes:
#include "XPBCID.h"
#include "XPBOID.h"

/** XPBCLoginDialogController
	Methods allow for the initialization, validation, and application of dialog widget values.
  
	Implements IDialogController based on the partial implementation CDialogController. 
	
	@author GD
*/
class XPBCLoginDialogController : public CDialogController
{
	public:
		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XPBCLoginDialogController(IPMUnknown* boss) : CDialogController(boss) {}

		/**
			Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
		// GD 24.03.04 ++
		// 2.0.2 -> CS
		//virtual void InitializeFields();
		virtual void InitializeDialogFields(IActiveContext* dlgContext);
		// GD 24.03.04 --
		
		/**
			Validate the values in the widgets. 
			By default, the widget with ID kOKButtonWidgetID causes 
			ValidateFields to be called. When all widgets are valid, 
			ApplyFields will be called.			
			@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

		*/
		// GD 24.03.04 ++
		// 2.0.2 -> CS
		//virtual WidgetID ValidateFields();
		virtual WidgetID ValidateDialogFields(IActiveContext* myContext);
		// GD 24.03.04 --

		/**
			Retrieve the values from the widgets and act on them.
			@param widgetId identifies the widget on which to act.
		*/
		// GD 24.03.04 ++
		// 2.0.2 -> CS
		//virtual void ApplyFields(const WidgetID& widgetId);
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
		// GD 24.03.04 --
		
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XPBCLoginDialogController, kXPBCLoginDialogControllerImpl)

/* InitializeDialogFields
*/
void XPBCLoginDialogController::InitializeDialogFields(IActiveContext* dlgContext) 
{
	CDialogController::InitializeDialogFields(dlgContext);
	do {
	
		// Get last connected user		
		InterfacePtr<IStringData> theUserName (GetExecutionContextSession(), IID_ISTRINGDATA_USERNAME);
		
		this->SetTextControlData(kLoginUserEdtWidgetID, theUserName->Get());
		this->SetTextControlData(kLoginPasswordEdtWidgetID, kNullString);
		
	} while (kFalse);

}

/* ValidateFields
*/
WidgetID XPBCLoginDialogController::ValidateDialogFields(IActiveContext* dlgContext) 
{	
	WidgetID 		badWidgetID = kInvalidWidgetID;
	PMString		theUser;
	PMString		thePassword;
	PMString		error;
	PMString		userName;
	PMString 		theClient ("xpubclient", PMString::kUnknownEncoding);
	
	PMString 		message1 (kLogErrorMessage1Key);
	PMString 		message2 (kLogErrorMessage2Key);
	PMString 		message3 (kLogErrorMessage3Key);
	PMString 		message4 (kLogErrorMessage4Key);
	PMString 		message5 (kLogErrorMessage5Key);
	PMString 		message6 (kLogErrorMessage6Key);
	
	do {
		// Recuperation du texte des champs
		theUser = this->GetTextControlData(kLoginUserEdtWidgetID);
		thePassword = this->GetTextControlData(kLoginPasswordEdtWidgetID);
		if (theUser.IsEmpty()) {
			badWidgetID = kLoginUserEdtWidgetID;
			CAlert::InformationAlert(message5);
			break;
		}
		if (thePassword.IsEmpty()) {
			badWidgetID = kLoginPasswordEdtWidgetID;
			CAlert::InformationAlert(message6);
			break;
		}

		// Requete a la base de donnees
		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());	

		bool success = xpubConnexion->Login(theUser, thePassword, theClient, error, userName);
		
		if(success) {
			if (error.Compare(kFalse, PMString("unknownUser")) == 0) {
				CAlert::InformationAlert(message1);
				badWidgetID = kLoginUserEdtWidgetID;
			} else if (error.Compare(kFalse, PMString("badPassword")) == 0) {
				CAlert::InformationAlert(message2);
				badWidgetID = kLoginPasswordEdtWidgetID;
			} else if (error.Compare(kFalse, PMString("privilegemissing")) == 0) {
				CAlert::InformationAlert(message3);
				badWidgetID = kLoginPasswordEdtWidgetID;
			} else if (error.Compare(kFalse, PMString("loginOK")) == 0) {
				// do nothing, badWidgetID is already set to kInvalidWidgetID
			} else {
				CAlert::InformationAlert(message4);
				badWidgetID = kLoginPasswordEdtWidgetID;
			}
		} 
		else
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);

			badWidgetID = kLoginUserEdtWidgetID;
		}
	
	} while (kFalse);

	return badWidgetID;
}

/* ApplyFields
*/
void XPBCLoginDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId)
{
	// Enregistrement de la globale IID_IBOOL_LOGIN
	do {
		InterfacePtr<IBoolData> gLogin (GetExecutionContextSession(), IID_IBOOL_LOGIN);
		gLogin->Set(kTrue);

		// Store last connected user in session preferences
		Utils<IXPubClientUtils>()->SetPersistentString(::GetUIDRef(GetExecutionContextSession()),IID_ISTRINGDATA_USERNAME,GetTextControlData(kLoginUserEdtWidgetID));		
				
	} while (kFalse);
}

// End, XPBCLoginDialogController.cpp.


