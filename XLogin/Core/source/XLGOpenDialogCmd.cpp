#include "VCPluginHeaders.h"

// General includes
#include "ErrorUtils.h"
#include "Command.h"
#include "IDialogMgr.h"
#include "LocaleSetting.h"
#include "IApplication.h"
#include "RsrcSpec.h"
#include "LocaleIndex.h"

// Interface includes
#include "ISubject.h"
#include "IDialog.h"

#include "XLGID.h"

/** XLGOpenDialogCmd
*/
class XLGOpenDialogCmd : public Command
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XLGOpenDialogCmd(IPMUnknown* boss);

	/**
		Destructor.
	*/
	~XLGOpenDialogCmd();

	/**
		@return kFalse don't use this command if memory is low. 
	*/
	bool16 LowMemIsOK() const { return kFalse; }

protected:
	/**
		The Do method is where all the action happens. 
	*/
	void Do();

	/**
		Set up target to session
	*/
	void SetUpTarget();

	/**
		Gives the name that shows up under the Edit menu and allows the user to undo or redo the command.

		@return pointer to a PMString allocated using the new operator. Deleted by caller.
	*/
	PMString* CreateName();

private :

    IDialog * CreateNewDialog(PluginID pluginID, RsrcID dialogID);

};


CREATE_PMINTERFACE(XLGOpenDialogCmd, kXLGOpenDialogCmdImpl)

/* Constructor
*/
XLGOpenDialogCmd::XLGOpenDialogCmd(IPMUnknown* boss) 
: Command(boss)
{
	SetUndoability(ICommand::kAutoUndo);
}

/* Destructor
*/
XLGOpenDialogCmd::~XLGOpenDialogCmd()
{
}

/* Do
*/
void XLGOpenDialogCmd::Do()
{
	ErrorCode status = kFailure;
	do
	{
		IDialog * dialog = CreateNewDialog(kXLGPluginID, kXLGLoginDialogResourceID);
		if(!dialog)
			break;

		dialog->Open(); 
        //dialog->WaitForDialog(kTrue);

        status = kSuccess;

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}

/* CreateName
*/
PMString * XLGOpenDialogCmd::CreateName()
{
	PMString * name = new PMString("Open login dialog");
	name->SetTranslatable(kFalse);
	return name;
}

/*
	SetUpTarget
*/
void XLGOpenDialogCmd::SetUpTarget()
{
	SetTarget(Command::kSessionTarget);
  
}


IDialog * XLGOpenDialogCmd::CreateNewDialog(PluginID pluginID, RsrcID dialogID)
{
	// Get the application interface and the DialogMgr.	
	InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
	InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());

	// Load the plug-in's resource.
	PMLocaleId nLocale = LocaleSetting::GetLocale();
	RsrcSpec dialogSpec
	(
		nLocale,					// Locale index from PMLocaleIDs.h. 
		pluginID,			        // Our Plug-in ID from BasicDialogID.h. 
		kViewRsrcType,				// This is the kViewRsrcType.
		dialogID,	                // Resource ID for our dialog.
		kTrue						// Initially visible.
	);

	// CreateNewDialog takes the dialogSpec created above, and also
	// the type of dialog being created (kMovableModal).
	IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
	
	return dialog;
}
