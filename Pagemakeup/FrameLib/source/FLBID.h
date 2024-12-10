/*
//	File:	FLBID.h
//
//	Author:	Trias
//
//	Date:	16-Jun-2003
//
//	Defines IDs used by the FLB plug-in.
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2003 Trias Developpement. All rights reserved.
//	
//	NOTICE: Adobe permits you to use, modify, and distribute this file in
//	accordance with the terms of the Adobe license agreement accompanying it.
//	If you have received this file from a source other than Adobe, then your
//	use, modification, or distribution of it requires the prior written
//	permission of Adobe.
//
*/

#ifndef __FLBID_h__
#define __FLBID_h__

#include "SDKDef.h"

// Plug-in:
#define kFLBPluginName		"FrameLib"			// Name of this plug-in.
#define kFLBPrefixNumber	0xd0479				// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kFLBVersion			"19.0.0"				// Version of this plug-in (for the About Box).
#define kFLBAuthor			"Trias"	// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kFLBPrefixNumber above to modify the prefix.)
#define kFLBPrefix			RezLong(kFLBPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kFLBStringPrefix	SDK_DEF_STRINGIZE(kFLBPrefixNumber)		// The string equivalent of the unique prefix number for  this plug-in.

// PluginID:
DECLARE_PMID(kPlugInIDSpace, 			kFLBPluginID, 							kFLBPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, 			kFrameHelperBoss, 						kFLBPrefix + 0)

// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, 		IID_IITEMMANAGER, 						kFLBPrefix + 0)
DECLARE_PMID(kInterfaceIDSpace, 		IID_ITEXTFRAMEHELPER, 					kFLBPrefix + 1)
DECLARE_PMID(kInterfaceIDSpace, 		IID_IITEMTRANSFORM, 					kFLBPrefix + 2)

// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, 	kFLBTextFrameHelperImpl, kFLBPrefix + 1)
DECLARE_PMID(kImplementationIDSpace, 	kFLBItemTransformImpl, kFLBPrefix + 2)
DECLARE_PMID(kImplementationIDSpace,	kFLBItemManagerImpl,		kFLBPrefix + 3)


// String keys
#define kFlbNilInterface kFLBStringPrefix "kFlbNilInterface"
#define kTextLinkCmdFailed kFLBStringPrefix "kTextLinkCmdFailed"
#define kTextUnlinkCmdFailed kFLBStringPrefix "kTextUnlinkCmdFailed"
#define kFlbNilParameter kFLBStringPrefix "kFlbNilParameter"
#define kErrNotTextFrame kFLBStringPrefix "kErrNotTextFrame"
#define kInsertCmdFailed kFLBStringPrefix "kInsertCmdFailed"
#define kMoveItemCmdFailed kFLBStringPrefix "kMoveItemCmdFailed"
#define kErrNotSpline kFLBStringPrefix "kErrNotSpline"
#define kMakeInlineCmdFailed kFLBStringPrefix "kMakeInlineCmdFailed"
#define kSetStandoffCmdFailed kFLBStringPrefix "kSetStandoffCmdFailed"
#define kCreateTextFrameCmdFailed kFLBStringPrefix "kCreateTextFrameCmdFailed"
#define kSelectCmdFailed kFLBStringPrefix "kSelectCmdFailed"
#define kErrCannotOpenFile kFLBStringPrefix "kErrCannotOpenFile"
#define kNewPageItemCmdFailed kFLBStringPrefix "kNewPageItemCmdFailed"
#define kErrNotImageFile kFLBStringPrefix "kErrNotImageFile"
#define kImportImageCommandFailed kFLBStringPrefix "kImportImageCommandFailed"
#define kPlaceItemCommandFailed kFLBStringPrefix "kPlaceItemCommandFailed"
#define kGroupCommandFailed kFLBStringPrefix "kGroupCommandFailed"
#define kErrNoSuchPage kFLBStringPrefix "kErrNoSuchPage"
#define kLockCmdFailed kFLBStringPrefix "kLockCmdFailed"
#define kErrUnableToGroup kFLBStringPrefix "kErrUnableToGroup"
#define kApplyCmdFailed kFLBStringPrefix "kApplyCmdFailed"
#define kBringToFrontCmdFailed kFLBStringPrefix "kBringToFrontCmdFailed"
#define kSendToBackCmdFailed kFLBStringPrefix "kSendToBackCmdFailed"
#define kCenterContentCmdFailed kFLBStringPrefix "kCenterContentCmdFailed"
#define kFitContentPropCmdFailed kFLBStringPrefix "kFitContentPropCmdFailed"
#define kFitContentToFrameCmdFailed kFLBStringPrefix "kFitContentToFrameCmdFailed"
#define kResizeCmdFailed kFLBStringPrefix "kResizeCmdFailed"
#define kErrJVImpossible kFLBStringPrefix "kErrJVImpossible"
#define kJVCmdFailed kFLBStringPrefix "kJVCmdFailed"
#define kFlbErrTextBoxRequired kFLBStringPrefix "kFlbErrTextBoxRequired"
#define kFlbErrImageBoxRequired kFLBStringPrefix "kFlbErrImageBoxRequired"

// Initial data format version numbers
#define kFLBFirstMajorFormatNumber  kSDKDef_30_PersistMajorVersionNumber
#define kFLBFirstMinorFormatNumber  kSDKDef_30_PersistMinorVersionNumber

// Data format version numbers for the PluginVersion resource 
#define kFLBCurrentMajorFormatNumber kFLBFirstMajorFormatNumber // most recent major format change
#define kFLBCurrentMinorFormatNumber kFLBFirstMinorFormatNumber // most recent minor format change

#endif // __FLBID_h__
//  Generated by Dolly build 17: template "IfPanelMenu".
// End, FLBID.h.

