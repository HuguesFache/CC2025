
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IDocument.h"
#include "IResaRedacDataList.h"
#include "ISubject.h"

// General includes:
#include "CAlert.h"
#include "CmdUtils.h"
#include "Command.h"
#include "ErrorUtils.h"
#include "XPGID.h"

/** Implementation of command to save assignment passed in parameters
*/
class XPGSetResaRedacDataListCmd : public Command
{
public:
	/** Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.*/
	XPGSetResaRedacDataListCmd(IPMUnknown* boss);

	/** Destructor
	*/
	~XPGSetResaRedacDataListCmd();

protected:

	/** Do
	 	@see Command::Do
	*/
	virtual void Do();
	
	/** DoNotify
	 	@see Command::DoNotify
	*/
	virtual void DoNotify();

	/** CreateName
	 	@see Command::CreateName
	*/
	virtual PMString* CreateName();


};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPGSetResaRedacDataListCmd, kXPGSetResaRedacDataListCmdImpl)

/* Constructor
*/
XPGSetResaRedacDataListCmd::XPGSetResaRedacDataListCmd(IPMUnknown* boss) : Command(boss)	
{
	SetUndoability(ICommand::kAutoUndo);
}

/** Destructor
*/
XPGSetResaRedacDataListCmd::~XPGSetResaRedacDataListCmd()
{
}

/* Do
*/
void XPGSetResaRedacDataListCmd::Do()
{
	ErrorCode status = kFailure;
	do                      
	{
		// Get document to modify
		if(fItemList.IsEmpty())
		{
			ASSERT_FAIL("no document passed to the command");
			break;
		}

		InterfacePtr<IDocument> doc (fItemList.GetRef(0), UseDefaultIID());
		if (doc == nil)
		{
			ASSERT_FAIL("doc invalid");
			break;
		}

		// Get new data list
		InterfacePtr<IResaRedacDataList> newDataList (this, UseDefaultIID());
		
		// Update document
		InterfacePtr<IResaRedacDataList> docList (doc, UseDefaultIID());
		docList->CopyFrom(newDataList);

		status = kSuccess;
	
	} while (kFalse);       

	// Handle any errors
	if (status != kSuccess) {
		ErrorUtils::PMSetGlobalErrorCode(status);
	}
}

/* DoNotify
*/
void XPGSetResaRedacDataListCmd::DoNotify()
{
	do
	{
		if(fItemList.IsEmpty())
		{
			ASSERT_FAIL("no document passed to the command");
			break;
		}

		InterfacePtr<IDocument> doc (fItemList.GetRef(0), UseDefaultIID());
		if (doc == nil)
		{
			ASSERT_FAIL("doc invalid");
			break;
		}

		InterfacePtr<ISubject> subject( doc, UseDefaultIID());
		if (subject == nil)
		{
			ASSERT_FAIL("subject invalid");
			break;
		}
		subject->ModelChange(kXPGSetResaRedacDataListCmdBoss, IID_IRESAREDACDATALIST, this);
	} while (false);
}

/* CreateName
*/
PMString* XPGSetResaRedacDataListCmd::CreateName()
{
	return new PMString("Save resa redac");
}

// End, XPGSetResaRedacDataListCmd.cpp.


