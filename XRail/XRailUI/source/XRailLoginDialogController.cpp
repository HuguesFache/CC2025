
#include "VCPlugInHeaders.h"

// Interface includes:
#include "CIdleTask.h"
#include "IIntData.h"
#include "IStringData.h"
#include "ICoreFilename.h"
#include "IWebServices.h"
#include "IUserInfoUtils.h"
#include "Utils.h"

// General includes:
#include "CAlert.h"
#include "CDialogController.h"
#include "SystemUtils.h"
#include "StreamUtil.h"
#include "DocUtils.h"
#include "SDKFileHelper.h"

// Project includes:
#include "XRLUIID.h"
#include "XRLID.h"
#include "XRCID.h"


/** XPBCLoginDialogController
	Methods allow for the initialization, validation, and application of dialog widget values.
  
	Implements IDialogController based on the partial implementation CDialogController. 
	
	@author GD
*/


class XRailLoginDialogController : public CDialogController
{
	public:
		/**
			Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XRailLoginDialogController(IPMUnknown* boss) : CDialogController(boss), userID(kNullString), theUser(kNullString) {}

		/**
			Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
		virtual void InitializeDialogFields(IActiveContext* dlgContext);
		
		/**
			Validate the values in the widgets. 
			By default, the widget with ID kOKButtonWidgetID causes 
			ValidateFields to be called. When all widgets are valid, 
			ApplyFields will be called.			
			@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

		*/
		virtual WidgetID ValidateDialogFields(IActiveContext* myContext);

		/**
			Retrieve the values from the widgets and act on them.
			@param widgetId identifies the widget on which to act.
		*/
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);

		virtual void UserCancelled(); 

private :
		PMString userID, theUser;
		
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XRailLoginDialogController, kXRLUILoginDialogControllerImpl)

/* ApplyFields
*/
void XRailLoginDialogController::InitializeDialogFields(IActiveContext* dlgContext) 
{
	PMString		ip, port, lastUser;
	PMString 		thePassword ("", PMString::kUnknownEncoding);
	bool16 modeRobot = kTrue;
	K2Vector<PMString> listeBases;
	K2Vector<PMString> IPBases;
		
	CDialogController::InitializeDialogFields(dlgContext);

	// Put code to initialize widget values here.
	do {
		GetPrefsXRail(ip, port, lastUser, modeRobot, listeBases, IPBases);
		lastUser.SetTranslatable(kFalse);
		this->SetTextControlData(kXRLUILoginUserWidgetID, lastUser);
		this->SetTextControlData(kXRLUIPasswordUserWidgetID, thePassword);
	} while (kFalse);	
}

/* ValidateFields
*/
WidgetID XRailLoginDialogController::ValidateDialogFields(IActiveContext* dlgContext) 
{	
	
	WidgetID badWidgetID = kInvalidWidgetID;
	PMString result, thePassword;
	do {
		// Recuperation du texte des champs
		theUser = this->GetTextControlData(kXRLUILoginUserWidgetID);
		thePassword = this->GetTextControlData(kXRLUIPasswordUserWidgetID);

		if (theUser.NumUTF16TextChars() == 0) {
			badWidgetID = kXRLUILoginUserWidgetID;
			CAlert::InformationAlert(PMString(kXRLUIErrorBlankLoginKey));
			break;
		}

		// Connexion a XRail
		PMString ip, port, temp;
		bool16 modeRobot = kFalse;
		K2Vector<PMString> listeBases;
		K2Vector<PMString> IPBases;
		GetPrefsXRail(ip, port, temp, modeRobot, listeBases, IPBases);

		PMString serverAddress = "http://" + ip + ":" + port + "/4DSOAP";
		InterfacePtr<IWebServices> xrailWebServices (::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAddress.GetPlatformString().c_str());

		bool16 connexionOK = xrailWebServices->VerifLogin(theUser, thePassword, userID,result);
		
		if(!connexionOK)
			CAlert::ErrorAlert(PMString(kXRLErrorWebServiceKey));
		else{

			if(result == "loginOK"){

				// Store user ID for the session
				Utils<IUserInfoUtils>()->SetInCopyUsername(theUser);

				InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ISTRINGDATA_LOGIN);
				gLogin->Set(userID);
	
				InterfacePtr<IStringData> gLoginString (GetExecutionContextSession(), IID_ILOGINSTRING);
				gLoginString->Set(theUser);
	
				// Verifier est ce que l'utilisateur a le droit de voir Vignette et PDF au niveau du menu XRail
				int32 theUserIDValue = userID.GetAsNumber();
				int previewResult, pdfResult;
				
				if(! xrailWebServices->GetDroitsMEP(theUserIDValue, pdfResult, previewResult))
					CAlert::ErrorAlert(PMString(kXRLErrorWebServiceKey));
				
				else {
					InterfacePtr<IBoolData> createPreviewBool (GetExecutionContextSession(), IID_ICREATEPREVIEWBOOL);
					InterfacePtr<IBoolData> createPDFBool (GetExecutionContextSession(), IID_ICREATEPDFBOOL);
			
					if(previewResult == 0) 
						createPreviewBool->Set(kFalse);				
					else if(previewResult == 1) 
						createPreviewBool->Set(kTrue);
					if(pdfResult == 0) 
						createPDFBool->Set(kFalse);
					else if(pdfResult == 1) 
						createPDFBool->Set(kTrue);
				}
				// Save login for next identification
				SetXRailPrefs(ip, port, theUser, modeRobot, listeBases, IPBases);
			}
			else if(result == "badPassword"){
				badWidgetID = kXRLUIPasswordUserWidgetID;
				CAlert::ErrorAlert(PMString(kXRLErrorBadPasswordKey));
			}
			else if(result == "privilegemissing"){
				badWidgetID = kXRLUIPasswordUserWidgetID;
				CAlert::ErrorAlert(PMString(kXRLErrorPrivilegeKey));
			}
			else if(result == "unknownUser"){
				badWidgetID = kXRLUILoginUserWidgetID;
				CAlert::ErrorAlert(PMString(kXRLErrorUnknownUserKey));
			}
			else{
				badWidgetID = kXRLUIPasswordUserWidgetID;
				CAlert::ErrorAlert(PMString(kXRLErrorSOAPKey));
			}
		}
		
	} while (kFalse);
	
	return badWidgetID;
}

/* ApplyFields
*/
void XRailLoginDialogController::ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId)
{
	// Store user ID for the session
	InterfacePtr<IStringData> gLoginString (GetExecutionContextSession(), IID_ILOGINSTRING);
	gLoginString->Set(theUser);
}


void XRailLoginDialogController::UserCancelled() 
{
	InterfacePtr<IStringData> gLogin (GetExecutionContextSession(), IID_ISTRINGDATA_LOGIN);
	gLogin->Set("");
	
	InterfacePtr<IStringData> gLoginString (GetExecutionContextSession(), IID_ILOGINSTRING);
	gLoginString->Set("");
}

// End, XRailLoginDialogController.cpp.


