/*
//
//	File:	XPBCSetStringCmd.cpp
//
//  Author: Wilfried LEFEVRE
//
//	Date:	21-mai-2007
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2007 Trias Developpement. All rights reserved.
//
*/

#include "VCPluginHeaders.h"

// General includes
#include "ErrorUtils.h"
#include "Command.h"

// Interface includes
#include "ISubject.h"
#include "IStringData.h"
#include "IIntData.h"

// Project includes
#include "XPBCID.h"

/** XPBCSetStringCmd
*/
class XPBCSetStringCmd : public Command
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPBCSetStringCmd(IPMUnknown* boss);

	/**
		Destructor.
	*/
	~XPBCSetStringCmd();
	
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
		Undoes the command, by simply setting the data interfaces back to  the original values.
	*/
	void Undo();

	/**
		Call Do function
	*/
	void Redo();

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

	PMString oldString;
};


CREATE_PMINTERFACE(XPBCSetStringCmd, kXPBCSetStringCmdImpl)

/* Constructor
*/
XPBCSetStringCmd::XPBCSetStringCmd(IPMUnknown* boss) 
: Command(boss)
{
	SetUndoability(kAutoUndo);
}

/* Destructor
*/
XPBCSetStringCmd::~XPBCSetStringCmd()
{
}

/* Do
*/
void XPBCSetStringCmd::Do()
{
	ErrorCode status = kFailure;

	do
	{
		int32 length = fItemList.Length();
		if(length != 1)
			break;

		// Get string data to set
		InterfacePtr<IStringData> cmdData (this, UseDefaultIID());
		PMString theString = cmdData->Get();

		// Get targeted interface's ID to apply
		InterfacePtr<IIntData> interfaceData (this, UseDefaultIID());
		PMIID theInterface = interfaceData->Get();
		
	
		InterfacePtr<IStringData> stringData (fItemList.GetRef(0), theInterface);
		if(stringData == nil)
			break;

		// Store old string data
		oldString = stringData->Get();
		
		// Apply string
		stringData->Set(theString);		

		status = kSuccess;

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}

/* Redo
*/
void XPBCSetStringCmd::Redo()
{
	this->Do();
}

/* Undo
*/
void XPBCSetStringCmd::Undo()
{
	ErrorCode status = kFailure;

	do
	{		
		int32 length = fItemList.Length();
		if(length != 1)
			break;

		// Get targeted interface's ID to apply
		InterfacePtr<IIntData> interfaceData (this, UseDefaultIID());
		PMIID theInterface = interfaceData->Get();
		
		// Apply old type
		InterfacePtr<IStringData> stringData (fItemList.GetRef(0), theInterface);
		if(stringData == nil)
			break;

		stringData->Set(oldString);
		
		status = kSuccess;

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}

/* DoNotify
*/
void XPBCSetStringCmd::DoNotify()
{
	ErrorCode status = kFailure;
			
	do
	{

		int32 length = fItemList.Length();
		if(length != 1)
			break;

			// Get targeted interface's ID to apply
		InterfacePtr<IIntData> interfaceData (this, UseDefaultIID());
		PMIID theInterface = interfaceData->Get();
		
		// Notify the subject of the change
		InterfacePtr<ISubject> subject (fItemList.GetRef(0), UseDefaultIID());
		subject->Change(kXPBCSetStringCmdBoss, theInterface, this);
		
		status = kSuccess;

	} while(false);

	if(status != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode(kFailure);
	}
}


/* CreateName
*/
PMString * XPBCSetStringCmd::CreateName()
{
	PMString * name = new PMString("Set String Data");
	name->SetTranslatable(kFalse);
	return name;
}
