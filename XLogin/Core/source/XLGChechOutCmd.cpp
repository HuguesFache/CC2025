
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IDocument.h"
#include "IAssignmentMgr.h"
#include "IAssignmentUtils.h"
#include "IItemLockData.h"
#include "IInCopyBridgeUtils.h"
#include "IUIFlagData.h"
#include "ILiveEditFacade.h"
// General includes:
#include "CmdUtils.h"
#include "Command.h"
#include "ErrorUtils.h"
#include "XLGID.h"

/** Implementation of command to set tag to style map in spline item
*/
class XLGXLGChechOutCmd : public Command
{
public:
	/** Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.*/
	XLGXLGChechOutCmd(IPMUnknown* boss);

	/** Destructor
	*/
	~XLGXLGChechOutCmd();

protected:

	/** Do
	 	@see Command::Do
	*/
	virtual void Do();
	
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
CREATE_PMINTERFACE(XLGXLGChechOutCmd, kXLGCheckOutCmdImpl)

/* Constructor
*/
XLGXLGChechOutCmd::XLGXLGChechOutCmd(IPMUnknown* boss) : Command(boss)	
{
	SetUndoability(ICommand::kAutoUndo);
}

/** Destructor
*/
XLGXLGChechOutCmd::~XLGXLGChechOutCmd()
{
}

/* Do
*/
void XLGXLGChechOutCmd::Do()
{
	do                      
	{
		InterfacePtr<IAssignmentMgr> assignmentMgr (GetExecutionContextSession(), UseDefaultIID());	
		InterfacePtr<IDocument> theDoc (fItemList.GetRef(0), IID_IDOCUMENT);		
		
		// Check whether it's is a saved assignment
		IDataBase * db = fItemList.GetDataBase();
		if(!db->GetSysFile())
				break;

		IDFile assignFile = *db->GetSysFile();

		IAssignedDocument * assignDoc = assignmentMgr->GetAssignedDocument(db);
		if(!assignDoc)
			break;
		
		// Get all assigned stories in the document
		UIDList storyList(db);
		assignmentMgr->GetStoryListOnDoc(assignDoc, storyList);		

		UIDList unlockList(db);
		// Go through stories looking for locked ones
		for(int32 i = 0 ; i < storyList.size() ; ++i){
			InterfacePtr<IItemLockData> lockData (storyList.GetRef(i), UseDefaultIID()); 
			if(lockData->GetInsertLock()) unlockList.Append(storyList.At(i));							
		}		

		if(!unlockList.IsEmpty())
		{
			// Porting CS3
			/*InterfacePtr<ICommand> editStoryCmd (CmdUtils::CreateCommand(kICBridgeEditInPlaceCmdBoss));		
			editStoryCmd->SetItemList(storyList);
			InterfacePtr<IUIFlagData> uiflags (editStoryCmd, UseDefaultIID());
			uiflags->Set(kSuppressUI);
			CmdUtils::ProcessCommand(editStoryCmd);
			*/

			Utils<Facade::ILiveEditFacade>()->CheckOut( storyList, ICommand::kRegularUndo, nil, kFalse, kFullUI );
		}

	} while (kFalse);       
}



/* CreateName
*/
PMString* XLGXLGChechOutCmd::CreateName()
{
	return new PMString("Unlock all stories");
}

// End, XLGXLGChechOutCmd.cpp.


