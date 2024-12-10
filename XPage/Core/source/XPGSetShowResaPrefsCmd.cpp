
#include "VCPlugInHeaders.h"

// Interface includes
#include "ISubject.h"
#include "ISession.h"
#include "IWorkspace.h"

// General includes
#include "ErrorUtils.h"
#include "Command.h"

// XPGSetShowResaPrefsCmd includes
#include "IBoolData.h"
#include "XPGID.h"

/**	XPGSetShowResaPrefsCmd
*/
class XPGSetShowResaPrefsCmd : public Command
{
	public:
		/**
			Constructor.
		*/
		XPGSetShowResaPrefsCmd(IPMUnknown* boss);

	protected:
		void Do();
		void DoNotify();
		PMString* CreateName();
};


/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XPGSetShowResaPrefsCmd, kXPGSetShowResaPrefsCmdImpl)


/* Constructor.
*/
XPGSetShowResaPrefsCmd::XPGSetShowResaPrefsCmd(IPMUnknown* boss) :Command(boss)
{
	SetTarget(kSessionWorkspaceTarget);
	SetUndoability(ICommand::kAutoUndo);
}

/* Do
*/
void XPGSetShowResaPrefsCmd::Do()
{
	ErrorCode status = kFailure;

	do
	{
		// Acquire the new prefs data
		InterfacePtr<IBoolData> newPrefs (this, IID_ISHOWRESAPREFS);
		
		// Acquire the current preference data and save its state	
		InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
		if (workspace == nil)
		{
			ASSERT_FAIL("workspace invalid");
			break;
		}

		InterfacePtr<IBoolData> currentPrefs (workspace, IID_ISHOWRESAPREFS);
		if (currentPrefs == nil)
		{
			ASSERT_FAIL("currentPrefs invalid");
			break;
		}

   		// Update prefs
		currentPrefs->Set(newPrefs->Get());

   		status = kSuccess;
   	
	}	while (false);

	// Handle any errors
	if ( status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(status);
	}
}

/* DoNotify
*/
void XPGSetShowResaPrefsCmd::DoNotify()
{
	do
	{
		InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
		if (workspace == nil)
		{
			ASSERT_FAIL("workspace invalid");
			break;
		}

		InterfacePtr<ISubject> subject( workspace, UseDefaultIID());
		if (subject == nil)
		{
			ASSERT_FAIL("subject invalid");
			break;
		}
		
		subject->ModelChange(kXPGSetShowResaPrefsCmdBoss, IID_ISHOWRESAPREFS, this);

	} while (false);
}

/* CreateName
*/
PMString* XPGSetShowResaPrefsCmd::CreateName()
{
	PMString* str = new PMString("Set Show Resa Preferences");
	return str;
}