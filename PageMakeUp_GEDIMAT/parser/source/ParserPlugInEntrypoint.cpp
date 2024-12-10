/*
//	File:	ParserPlugInEntrypoint.cpp
//
//  Author: Wilfried LEFEVRE
//
//	Date:	2-July-2003
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2003 Trias Developpement. All rights reserved.
//
*/

#include "VCPlugInHeaders.h"
#include "ParserPlugIn.h"

static ParserPlugIn gPlugIn;

/** GetPlugIn
	This is the main entry point from the application to the plug-in.
	The application calls this function when the plug-in is installed 
	or loaded. This function is called by name, so it must be called 
	GetPlugIn, and defined as C linkage.
	@author Jeff Gehman
*/
IPlugIn* GetPlugIn()
{
	return &gPlugIn;
}