//========================================================================================
//  
//  $File: $
//  
//  Owner: Badr Nasser
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#ifndef __XLGID_h__
#define __XLGID_h__

#include "SDKDef.h"

// Company:
#define kXLGCompanyKey		kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kXLGCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kXLGPluginName	 	"XLogin"					// Name of this plug-in.
#define kXLGPrefixNumber 	0xe9400 					// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kXLGVersion		 	"10.3.0"						// Version of this plug-in (for the About Box).
#define kXLGAuthor		 	"Trias"				// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kXLGPrefixNumber above to modify the prefix.)
#define kXLGPrefix			RezLong(kXLGPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kXLGStringPrefix	SDK_DEF_STRINGIZE(kXLGPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kXLGMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kXLGMissingPluginAlertValue		kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// Precompilation flag
#define TEC 1

//Projects Keys

#define kXLGLoginDialogTitleKey		    kXLGStringPrefix "kXRLUILoginDialogTitleKey"
#define kXLGLoginTextKey		        kXLGStringPrefix "kXRLUILoginTextKey"
#define kXLGPasswordTextKey		        kXLGStringPrefix "kXRLUIPasswordTextKey"
#define kXLGChangeStatusDialogTitleKey  kXLGStringPrefix "kXLGChangeStatusDialogTitleKey"
#define kXLGStatusListTextKey		    kXLGStringPrefix "kXLGStatusListTextKey"
#define kXLGRecipientListTextKey		kXLGStringPrefix "kXLGRecipientListTextKey"
#define kXLGGroupsListTextKey		    kXLGStringPrefix "kXLGGroupsListTextKey"
#define kXLGChooseRecepientTextKey		kXLGStringPrefix "kXLGChooseRecepientTextKey"
#define kXLGAutoClosingDialogKey		kXLGStringPrefix "kXLGAutoClosingDialogKey"
#define kXLGAutoInitVariableKey		    kXLGStringPrefix "kXLGAutoInitVariableKey"



// Login Keys

#define kXLGUIErrorBlankLoginKey		kXLGStringPrefix "kXLGUIErrorBlankLoginKey"
#define kXLGUIErrorBlankPasswordKey		kXLGStringPrefix "kXLGUIErrorBlankPasswordKey"
#define kXLGUIErrorWebServiceKey		kXLGStringPrefix "kXLGUIErrorWebServiceKey"
#define kXLGUIErrorBadPasswordKey		kXLGStringPrefix "kXLGUIErrorBadPasswordKey"
#define kXLGUIErrorPrivilegeKey		    kXLGStringPrefix "kXLGUIErrorPrivilegeKey"
#define kXLGUIErrorUnknownUserKey		kXLGStringPrefix "kXLGUIErrorUnknownUserKey"
#define kXLGUIErrorSOAPKey		        kXLGStringPrefix "kXLGUIErrorSOAPKey"
#define kXLGEveryBodyTextKey		    kXLGStringPrefix "kXLGEveryBodyTextKey"
#define kXLGSubjectTextKey				kXLGStringPrefix "kXLGSubjectTextKey"





// PluginID:
DECLARE_PMID(kPlugInIDSpace, kXLGPluginID, kXLGPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kXLGActionComponentBoss, kXLGPrefix + 0)
DECLARE_PMID(kClassIDSpace, kXLGPanelWidgetBoss, kXLGPrefix + 1)
DECLARE_PMID(kClassIDSpace, kXLGDialogBoss, kXLGPrefix + 2)
DECLARE_PMID(kClassIDSpace, kXLGScriptProviderBoss, kXLGPrefix + 3)
DECLARE_PMID(kClassIDSpace, kXLGStartupShutdownBoss, kXLGPrefix + 4)
DECLARE_PMID(kClassIDSpace, kXLGLoginDialogBoss, kXLGPrefix + 5)
DECLARE_PMID(kClassIDSpace, kXLGOpenLoginDialogCmdBoss, kXLGPrefix + 6)
DECLARE_PMID(kClassIDSpace, kXLGResponderServiceBoss, kXLGPrefix + 7)
DECLARE_PMID(kClassIDSpace, kXLGChangeStatusDialogBoss, kXLGPrefix + 8)
DECLARE_PMID(kClassIDSpace, kXLGUnlockStoriesCmdBoss, kXLGPrefix + 9)
DECLARE_PMID(kClassIDSpace, kXLGDropDownListWidgetBoss, kXLGPrefix + 10)
DECLARE_PMID(kClassIDSpace, kXLGColorDropDownListWidgetBoss, kXLGPrefix + 11)

//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kXLGBoss, kXLGPrefix + 25)


// InterfaceIDs:
//DECLARE_PMID(kInterfaceIDSpace, IID_IXLGSUITE,			kXLGPrefix + 0)
DECLARE_PMID(kInterfaceIDSpace, IID_IXLGFACADE,			kXLGPrefix + 1)
DECLARE_PMID(kInterfaceIDSpace, IID_IXLGPREFERENCES,	kXLGPrefix + 2)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTUSER,		kXLGPrefix + 3)
DECLARE_PMID(kInterfaceIDSpace, IID_IFIRSTSTARTUP,		kXLGPrefix + 4)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTSTORY,		kXLGPrefix + 5)
DECLARE_PMID(kInterfaceIDSpace, IID_IIDLIST,			kXLGPrefix + 6)
DECLARE_PMID(kInterfaceIDSpace, IID_IXLGINTERFACE,		kXLGPrefix + 7)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTSTATUS,		kXLGPrefix + 8)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTGROUP,		kXLGPrefix + 9)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTERECEPIENT, kXLGPrefix + 10)
DECLARE_PMID(kInterfaceIDSpace, IID_IAUTOCLOSINGDIALOG, kXLGPrefix + 11)
DECLARE_PMID(kInterfaceIDSpace, IID_IIDSTATUSLIST,		kXLGPrefix + 12)
DECLARE_PMID(kInterfaceIDSpace, IID_IXLGDOCOBSERVER,	kXLGPrefix + 13)
DECLARE_PMID(kInterfaceIDSpace, IID_IDISABLECLOSEDOC,   kXLGPrefix + 14)
DECLARE_PMID(kInterfaceIDSpace, IID_ICURRENTDOCNAME,    kXLGPrefix + 15)
DECLARE_PMID(kInterfaceIDSpace, IID_ITIMERCLOSEDIALOG,  kXLGPrefix + 16)
DECLARE_PMID(kInterfaceIDSpace, IID_IDISABLECLOSE2DOC,  kXLGPrefix + 17)
DECLARE_PMID(kInterfaceIDSpace, IID_IXLGTEXTMODELOBSERVER,  kXLGPrefix + 18)
DECLARE_PMID(kInterfaceIDSpace, IID_IINITVARIABLE,			kXLGPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXLGINTERFACE, kXLGPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXLGINTERFACE, kXLGPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXLGINTERFACE, kXLGPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXLGINTERFACE, kXLGPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kXLGActionComponentImpl, kXLGPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kXLGDialogControllerImpl, kXLGPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kXLGDialogObserverImpl, kXLGPrefix + 2 )
//DECLARE_PMID(kImplementationIDSpace, kXLGSuiteLayoutCSBImpl, kXLGPrefix + 5 )
//DECLARE_PMID(kImplementationIDSpace, kXLGSuiteTextCSBImpl, kXLGPrefix + 6 )
//DECLARE_PMID(kImplementationIDSpace, kXLGSuiteDefaultsCSBImpl, kXLGPrefix + 7 )
DECLARE_PMID(kImplementationIDSpace, kXLGFacadeImpl, kXLGPrefix + 8 )
DECLARE_PMID(kImplementationIDSpace, kXLGStartupShutdownImpl, kXLGPrefix + 10)
DECLARE_PMID(kImplementationIDSpace, kXLGOpenDialogCmdImpl, kXLGPrefix + 11)
DECLARE_PMID(kImplementationIDSpace, kXLGServiceProviderImpl, kXLGPrefix + 12)
DECLARE_PMID(kImplementationIDSpace, kXLGResponderImpl, kXLGPrefix + 13)
DECLARE_PMID(kImplementationIDSpace, kXLGPreferencesImpl, kXLGPrefix + 14)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentUserImpl, kXLGPrefix + 15)
DECLARE_PMID(kImplementationIDSpace, kXLGFirstStartupImpl, kXLGPrefix + 16)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentStoryImpl, kXLGPrefix + 17)
DECLARE_PMID(kImplementationIDSpace, kXLGCheckOutCmdImpl, kXLGPrefix + 18)
DECLARE_PMID(kImplementationIDSpace, kXLGChangeStatusDialogControllerImpl, kXLGPrefix + 19)
DECLARE_PMID(kImplementationIDSpace, kXLGChangeStatusDialogObserverImpl, kXLGPrefix + 20)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentStatusImpl, kXLGPrefix + 21)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentGroupImpl, kXLGPrefix + 22)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentRecepientImpl, kXLGPrefix + 23)
DECLARE_PMID(kImplementationIDSpace, kXLGAutoClosingDialogImpl, kXLGPrefix + 24)
DECLARE_PMID(kImplementationIDSpace, kXLGIdsStatusListImpl, kXLGPrefix + 25)
DECLARE_PMID(kImplementationIDSpace, kXLGTextModelObserverImpl, kXLGPrefix + 26)
DECLARE_PMID(kImplementationIDSpace, kXPGDisableCloseDocBoolImpl, kXLGPrefix + 27)
DECLARE_PMID(kImplementationIDSpace, kXLGCurrentDocNameImpl, kXLGPrefix + 28)
DECLARE_PMID(kImplementationIDSpace, kXLGCloseDialogTimerImpl, kXLGPrefix + 29)
DECLARE_PMID(kImplementationIDSpace, kXPGDisableCloseDoc2BoolImpl, kXLGPrefix + 30)
DECLARE_PMID(kImplementationIDSpace, kXLGDocObserverImpl, kXLGPrefix + 31)
DECLARE_PMID(kImplementationIDSpace, kXLGAutoInitVariableImpl, kXLGPrefix + 32)



// ActionIDs:
DECLARE_PMID(kActionIDSpace, kXLGAboutActionID, kXLGPrefix + 0)
DECLARE_PMID(kActionIDSpace, kXLGPanelWidgetActionID, kXLGPrefix + 1)
DECLARE_PMID(kActionIDSpace, kXLGSeparator1ActionID, kXLGPrefix + 2)
DECLARE_PMID(kActionIDSpace, kXLGPopupAboutThisActionID, kXLGPrefix + 3)
DECLARE_PMID(kActionIDSpace, kXLGDialogActionID, kXLGPrefix + 4)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kXLGActionID, kXLGPrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kXLGPanelWidgetID, kXLGPrefix + 0)
DECLARE_PMID(kWidgetIDSpace, kXLGDialogWidgetID, kXLGPrefix + 1)
DECLARE_PMID(kWidgetIDSpace, kXLGLoginUserWidgetID, kXLGPrefix + 3)
DECLARE_PMID(kWidgetIDSpace, kXLGPasswordUserWidgetID, kXLGPrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kXLGStatsListWidgetID, kXLGPrefix + 5)
DECLARE_PMID(kWidgetIDSpace, kXLGRecipientStatusListWidgetID, kXLGPrefix + 6)
DECLARE_PMID(kWidgetIDSpace, kXLGChangeStatusDialogWidgetID, kXLGPrefix + 7)
DECLARE_PMID(kWidgetIDSpace, kXLGGroupsStatusListWidgetID, kXLGPrefix + 9)
DECLARE_PMID(kWidgetIDSpace, kXLGGroupsStatusTextWidgetID, kXLGPrefix + 11)
DECLARE_PMID(kWidgetIDSpace, kXLGRecipientStatusTextWidgetID, kXLGPrefix + 12)
DECLARE_PMID(kWidgetIDSpace, kXLGCurrentDocumentTextWidgetID, kXLGPrefix + 13)
DECLARE_PMID(kWidgetIDSpace, kXLGMultilineVScrollBarWidgetID, kXLGPrefix + 15)
DECLARE_PMID(kWidgetIDSpace, kXLGMessageWidgetID, kXLGPrefix + 16)

//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kXLGWidgetID, kXLGPrefix + 25)

//Script Element IDs
//Events
DECLARE_PMID(kScriptInfoIDSpace, kXLGEventScriptElement, kXLGPrefix + 1)
// Properties
DECLARE_PMID(kScriptInfoIDSpace, kXLGPropertyScriptElement, kXLGPrefix + 2)


// "About Plug-ins" sub-menu:
#define kXLGAboutMenuKey		kXLGStringPrefix "kXLGAboutMenuKey"
#define kXLGAboutMenuPath		kSDKDefStandardAboutMenuPath kXLGCompanyKey

// "Plug-ins" sub-menu:
#define kXLGPluginsMenuKey 		kXLGStringPrefix "kXLGPluginsMenuKey"
#define kXLGPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kXLGCompanyKey kSDKDefDelimitMenuPath kXLGPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kXLGAboutBoxStringKey				kXLGStringPrefix	"kXLGAboutBoxStringKey"
#define kXLGPanelTitleKey					kXLGStringPrefix	"kXLGPanelTitleKey"
#define kXLGStaticTextKey					kXLGStringPrefix	"kXLGStaticTextKey"
#define kXLGInternalPopupMenuNameKey		kXLGStringPrefix	"kXLGInternalPopupMenuNameKey"
#define kXLGTargetMenuPath					kXLGInternalPopupMenuNameKey

// Palette resource IDs
#define kXLGLoginDialogResourceID			kSDKDefPanelResourceID + 1
#define kXLGChangeStatusDialogResourceID	kSDKDefPanelResourceID + 2


// Menu item positions:

#define	kXLGSeparator1MenuItemPosition		10.0
#define kXLGAboutThisMenuItemPosition		11.0

#define kXLGDialogTitleKey kXLGStringPrefix "kXLGDialogTitleKey"


// "Plug-ins" sub-menu item key for dialog:
#define kXLGDialogMenuItemKey kXLGStringPrefix "kXLGDialogMenuItemKey"
// "Plug-ins" sub-menu item position for dialog:
#define kXLGDialogMenuItemPosition	12.0


// Initial data format version numbers
#define kXLGFirstMajorFormatNumber  RezLong(1)
#define kXLGFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kXLGCurrentMajorFormatNumber kXLGFirstMajorFormatNumber
#define kXLGCurrentMinorFormatNumber kXLGFirstMinorFormatNumber

#endif // __XLGID_h__

//  Code generated by DollyXs code generator
