//========================================================================================
//  
//  $File: $
//  
//  Owner: Trias
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#ifndef __XRLUIID_h__
#define __XRLUIID_h__

#include "SDKDef.h"

// Company:
#define kXRLUICompanyKey	"Trias Developpement"		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kXRLUICompanyValue	"Trias Developpement"	// Company name displayed externally.

// Plug-in:
#define kXRLUIPluginName	"XRailUI"			// Name of this plug-in.
#define kXRLUIPrefixNumber	0xe1200 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kXRLUIVersion		"19.0.0"				// Version of this plug-in (for the About Box).
#define kXRLUIAuthor		"Trias"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kXRLUIPrefixNumber above to modify the prefix.)
#define kXRLUIPrefix		RezLong(kXRLUIPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kXRLUIStringPrefix	SDK_DEF_STRINGIZE(kXRLUIPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kXRLUIMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kXRLUIMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kXRLUIPluginID, kXRLUIPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kXRLUIActionComponentBoss, kXRLUIPrefix + 0)
DECLARE_PMID(kClassIDSpace, kXRLUIOpenLoginDialogCmdBoss, kXRLUIPrefix + 1)
DECLARE_PMID(kClassIDSpace, kXRLUIPrefsDialogBoss, kXRLUIPrefix + 2)
DECLARE_PMID(kClassIDSpace, kXRLUIChangeStateDialogBoss, kXRLUIPrefix + 3)
DECLARE_PMID(kClassIDSpace, kXRLUILoginDialogBoss, kXRLUIPrefix + 4)
DECLARE_PMID(kClassIDSpace, kXRLUIStartupShutdownBoss, kXRLUIPrefix + 5)
DECLARE_PMID(kClassIDSpace, kXRLUILinkerDialogBoss, kXRLUIPrefix + 6)
DECLARE_PMID(kClassIDSpace, kXRLUIIDDropDownListWidgetBoss, kXRLUIPrefix + 7)

//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kXRLUIBoss, kXRLUIPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IINTLIST, kXRLUIPrefix + 0)

//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXRLINTERFACE, kXRLUIPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kXRLUIActionComponentImpl, kXRLUIPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kXRLUIOpenLoginDialogCmdImpl, kXRLUIPrefix + 1)
DECLARE_PMID(kImplementationIDSpace, kXRLUIChangeStateDialogControllerImpl, kXRLUIPrefix + 2)
DECLARE_PMID(kImplementationIDSpace, kXRLUILoginDialogControllerImpl, kXRLUIPrefix + 3)
DECLARE_PMID(kImplementationIDSpace, kXRLUIDialogControllerImpl, kXRLUIPrefix + 4)
DECLARE_PMID(kImplementationIDSpace, kXRLUIUIStartupShutdownImpl, kXRLUIPrefix + 5)
DECLARE_PMID(kImplementationIDSpace, kXRLUILinkerDialogControllerImpl, kXRLUIPrefix + 6)
DECLARE_PMID(kImplementationIDSpace, kXRLUIIntListImpl, kXRLUIPrefix + 7)
DECLARE_PMID(kImplementationIDSpace, kXRLUILinkerDialogObserverImpl, kXRLUIPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kXRLUIImpl, kXRLUIPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kXRLUIAboutActionID, kXRLUIPrefix + 0)
DECLARE_PMID(kActionIDSpace, kXRLUIPreferencesActionID, kXRLUIPrefix + 1)
DECLARE_PMID(kActionIDSpace, kXRLUIConnectionActionID, kXRLUIPrefix + 2)
DECLARE_PMID(kActionIDSpace, kXRLUIEnablePreviewActionID, kXRLUIPrefix + 7)
DECLARE_PMID(kActionIDSpace, kXRLUIExternalPDFActionID, kXRLUIPrefix + 9)
DECLARE_PMID(kActionIDSpace, kXRLUIXRailLinkActionID, kXRLUIPrefix + 10)
DECLARE_PMID(kActionIDSpace, kXRLUISep1ActionID, kXRLUIPrefix + 11)
DECLARE_PMID(kActionIDSpace, kXRLUISep2ActionID, kXRLUIPrefix + 12)
DECLARE_PMID(kActionIDSpace, kXRLUIXRailUnLinkActionID, kXRLUIPrefix + 15)
DECLARE_PMID(kActionIDSpace, kXRLUISep3ActionID, kXRLUIPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kXRLUIActionID, kXRLUIPrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kXRLUIPrefsDialogWidgetID, kXRLUIPrefix + 0)
DECLARE_PMID(kWidgetIDSpace, kIPField1WidgetID, kXRLUIPrefix + 1)
DECLARE_PMID(kWidgetIDSpace, kPortEditBoxWidgetID, kXRLUIPrefix + 2)
DECLARE_PMID(kWidgetIDSpace, kXRLUILoginDialogWidgetID, kXRLUIPrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kXRLUILoginUserWidgetID, kXRLUIPrefix + 6)
DECLARE_PMID(kWidgetIDSpace, kXRLUIPasswordUserWidgetID, kXRLUIPrefix + 7)
DECLARE_PMID(kWidgetIDSpace, kXRLUIChangeStateDialogWidgetID, kXRLUIPrefix + 8)
DECLARE_PMID(kWidgetIDSpace, kXRLUIStateListBoxWidgetID, kXRLUIPrefix + 9)
DECLARE_PMID(kWidgetIDSpace, kXRLUIListParentWidgetId, kXRLUIPrefix + 10)
DECLARE_PMID(kWidgetIDSpace, kXRLUIListCol1WidgetID, kXRLUIPrefix + 11)
DECLARE_PMID(kWidgetIDSpace, kIPField2WidgetID, kXRLUIPrefix + 12)
DECLARE_PMID(kWidgetIDSpace, kIPField3WidgetID, kXRLUIPrefix + 13)
DECLARE_PMID(kWidgetIDSpace, kIPField4WidgetID, kXRLUIPrefix + 14)
DECLARE_PMID(kWidgetIDSpace, kPrefsTypeWidgetID, kXRLUIPrefix + 16)
DECLARE_PMID(kWidgetIDSpace, kXRLUILinkerDialogWidgetID, kXRLUIPrefix + 17)
DECLARE_PMID(kWidgetIDSpace, kXRLUILevel1ListBoxWidgetID, kXRLUIPrefix + 19)
DECLARE_PMID(kWidgetIDSpace, kXRLUILevel2ListBoxWidgetID, kXRLUIPrefix + 20)
DECLARE_PMID(kWidgetIDSpace, kXRLUILevel3ListBoxWidgetID, kXRLUIPrefix + 21)
DECLARE_PMID(kWidgetIDSpace, kXRLUILevel4ListBoxWidgetID, kXRLUIPrefix + 22)
DECLARE_PMID(kWidgetIDSpace, kXRLUIRefreshWidgetID, kXRLUIPrefix + 34)
DECLARE_PMID(kWidgetIDSpace, kXRLUIDocumentNameWidgetID, kXRLUIPrefix + 35)
DECLARE_PMID(kWidgetIDSpace, kXRLUIMultilineVScrollBarWidgetID, kXRLUIPrefix + 36)
DECLARE_PMID(kWidgetIDSpace, kXRLUIMessageWidgetID, kXRLUIPrefix + 37)

// "About Plug-ins" sub-menu:
#define kXRLUIAboutMenuKey			kXRLUIStringPrefix "kXRLUIAboutMenuKey"
#define kXRLUIAboutMenuPath			kSDKDefStandardAboutMenuPath kXRLUICompanyKey

// "Plug-ins" sub-menu:
#define kXRLUIPluginsMenuKey 		kXRLUIStringPrefix "kXRLUIPluginsMenuKey"
#define kXRLUIPluginsMenuPath		"Main" kSDKDefDelimitMenuPath kXRLUIPluginsMenuKey

// Menu item keys:
#define kXRLUIMenu					kXRLUIStringPrefix "kXRLUIMenu"
#define kXRLUISousMenuPreferences	kXRLUIStringPrefix "kXRLUISousMenuPreferences"
#define kXRLUISousMenuConnection	kXRLUIStringPrefix "kXRLUISousMenuConnection"
#define kXRLUISousMenuDeconnection	kXRLUIStringPrefix "kXRLUISousMenuDeconnection"
#define kXRLUIPrefsIPLabel			kXRLUIStringPrefix "kXRLUIPrefsIPLabel"
#define kXRLUIPrefsPortLabel	    kXRLUIStringPrefix "kXRLUIPrefsPortLabel"

// "Plug-ins" sub-menu item keys:
#define kXRLUIDialogMenuItemKey		kXRLUIStringPrefix "kXRLUIDialogMenuItemKey"

// "Plug-ins" sub-menu item positions:
#define kXRLUIAboutMenuItemPosition				1.0
#define kXRLUIConnectionMenuItemPosition		2.0
#define kXRLUIPreferencesMenuItemPosition		3.0
#define kXRLUISep1MenuItemPosition				11.0
#define kXRLUIExternalPDFMenuItemPosition		20.0
#define kXRLUISep2MenuItemPosition				30.0
#define kXRLUIEnablePreviewMenuItemPosition		31.0
#define kXRLUIXRailLinkMenuItemPosition			32.0
#define kXRLUIXRailUnLinkMenuItemPosition		33.0
#define kXRLUISep3MenuItemPosition				34.0
#define kXRLUISendToServerMenuItemPosition		35.1

// Other string keys
#define kXRLUITargetMenuPath			kXRLUIPluginsMenuPath
#define kXRLUIPrefsDialogTitleKey		kXRLUIStringPrefix "kXRLUIPrefsDialogTitleKey"
#define kXRLUILoginDialogTitleKey		kXRLUIStringPrefix "kXRLUILoginDialogTitleKey"
#define kXRLUIChangeStateDialogTitleKey	kXRLUIStringPrefix "kXRLUIChangeStateDialogTitleKey"
#define kXRLUILinkerDialogTitleKey		kXRLUIStringPrefix "kXRLUILinkerDialogTitleKey"
#define kXRLUIAboutBoxStringKey			kXRLUIStringPrefix "kXRLUIAboutBoxStringKey"
#define kXRLUIErrorWSGetStatusKey 		kXRLUIStringPrefix "kXRLUIErrorWSGetStatusKey"
#define kXRLUIErrorWSSetPageStatusKey 	kXRLUIStringPrefix "kXRLUIErrorWSSetPageStatusKey"
#define kXRLUIErrorWSGetSavePathKey 	kXRLUIStringPrefix "kXRLUIErrorWSGetSavePathKey"
#define kXRLUIErrorBlankLoginKey 		kXRLUIStringPrefix "kXRLUIErrorBlankLoginKey"
#define kXRLUIErrorBlankPasswordKey		kXRLUIStringPrefix "kXRLUIErrorBlankPasswordKey"
#define kXRLUIErrorWSGetLevelKey		kXRLUIStringPrefix "kXRLUIErrorWSGetLevelKey"
#define kXRLUIErrorWSLinkPagesKey		kXRLUIStringPrefix "kXRLUIErrorWSLinkPagesKey"
#define kXRLUILoginTextKey 				kXRLUIStringPrefix "kXRLUILoginTextKey"
#define kXRLUIPasswordTextKey 			kXRLUIStringPrefix "kXRLUIPasswordTextKey"
#define kXRLUIEnablePreviewMenuKey 		kXRLUIStringPrefix "kXRLUIEnablePreviewMenuKey"
#define kXRLUIExternalPDFMenuKey 		kXRLUIStringPrefix "kXRLUIExternalPDFMenuKey"
#define kXRLUIXRailLinkMenuKey	 		kXRLUIStringPrefix "kXRLUIXRailLinkMenuKey"
#define kXRLUIXRailUnLinkMenuKey	 	kXRLUIStringPrefix "kXRLUIXRailUnLinkMenuKey"
#define kXRLUIModeRobotTextKey 			kXRLUIStringPrefix "kXRLUIModeRobotTextKey"
#define kXRLUIMessageLinkerKey 			kXRLUIStringPrefix "kXRLUIMessageLinkerKey"
#define kXRLUIAnnonceurKey 		        kXRLUIStringPrefix "kXRLUIAnnonceurKey"
#define kXRLUIProduitKey 		        kXRLUIStringPrefix "kXRLUIProduitKey"
#define kXRLUITypeSurfaceKey 		    kXRLUIStringPrefix "kXRLUITypeSurfaceKey"
#define kXRLUIEmplacementKey 		    kXRLUIStringPrefix "kXRLUIEmplacementKey"
#define kXRLUIInternalPopupMenuNameKey 	kXRLUIStringPrefix "kXRLUIInternalPopupMenuNameKey"
#define kXRLUISendToServerMenuKey       kXRLUIStringPrefix "kXRLUISendToServerMenuKey"
#define kXRLUIErrorWSKey			    kXRLUIStringPrefix "kXRLUIErrorWSKey"
#define kXRLUIErrorSendToServerKey	    kXRLUIStringPrefix "kXRLUIErrorSendToServerKey"

// Drag & Drop flavor
#define kXRLUIPubDragDropCustomFlavor		PMFlavor('XRUI')

// Initial data format version numbers
#define kXRLUIFirstMajorFormatNumber		RezLong(1)
#define kXRLUIFirstMinorFormatNumber		RezLong(0)

// Ressources ID

#define kXRLUILoginDialogResourceID				kSDKDefPanelResourceID 
#define kXRLUIChangeStateDialogResourceID		kSDKDefPanelResourceID + 2
#define kXRLUILinkerDialogResourceID			kSDKDefPanelResourceID + 3
#define kXRLUIIconRefreshResID					kSDKDefPanelResourceID + 5


// Data format version numbers for the PluginVersion resource 
#define kXRLUICurrentMajorFormatNumber	kXRLUIFirstMajorFormatNumber
#define kXRLUICurrentMinorFormatNumber	kXRLUIFirstMinorFormatNumber

#endif // __XRLUIID_h__

//  Code generated by DollyXs code generator
