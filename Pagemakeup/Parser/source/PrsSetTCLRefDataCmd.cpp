/*
//
//	File:	PrsSetTCLRefDataCmd.cpp
//
//  Author: Trias
//
//	Date:	29-nov-2006
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2006 Trias Developpement. All rights reserved.
//
*/

#include "VCPluginHeaders.h"

// General includes
#include "ErrorUtils.h"
#include "Command.h"

// Interface includes
#include "ISubject.h"
#include "PMUTypes.h"

// Project includes
#include "PrsID.h"

/** PrsSetTCLRefDataCmd
*/
class PrsSetTCLRefDataCmd : public Command
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	PrsSetTCLRefDataCmd(IPMUnknown* boss);

	/**
		Destructor.
	*/
	~PrsSetTCLRefDataCmd();
	
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
		Notify interested observer about the change we have done in this command.
	*/
	void DoNotify();

	/**
		Gives the name that shows up under the Edit menu and allows the user to undo or redo the command.

		@return pointer to a PMString allocated using the new operator. Deleted by caller.
	*/
	PMString* CreateName();

private :

};


CREATE_PMINTERFACE(PrsSetTCLRefDataCmd, kPrsSetTCLRefDataCmdImpl)

/* Constructor
*/
PrsSetTCLRefDataCmd::PrsSetTCLRefDataCmd(IPMUnknown* boss) 
: Command(boss)
{
}

/* Destructor
*/
PrsSetTCLRefDataCmd::~PrsSetTCLRefDataCmd()
{
}

/* Do
*/
void PrsSetTCLRefDataCmd::Do()
{
	ErrorCode status = kFailure;

	do
	{
		if(fItemList.Length() != 1)
			break;

		InterfacePtr<ITCLRefData> tclRefData (fItemList.GetRef(0), IID_ITCLREFDATA);
		if(tclRefData == nil)
			break;

		// Get the command data and apply new reference
		InterfacePtr<ITCLRefData> cmdData (this, IID_ITCLREFDATA);
		tclRefData->Set(cmdData->Get());

		status = kSuccess;

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}

/* DoNotify
*/
void PrsSetTCLRefDataCmd::DoNotify()
{
	ErrorCode status = kFailure;
			
	do
	{
		if (fItemList.Length() != 1)
			break;
		else
		{
			InterfacePtr<ISubject> subject (fItemList.GetRef(0), UseDefaultIID());

			// Notify the subject of the change
			subject->Change(kPrsSetTCLRefDataCmdBoss, IID_ITCLREFDATA, this);

			status = kSuccess;
		}

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}


/* CreateName
*/
PMString * PrsSetTCLRefDataCmd::CreateName()
{
	PMString * name = new PMString("Set TCL Reference");
	name->SetTranslatable(kFalse);
	return name;
}
