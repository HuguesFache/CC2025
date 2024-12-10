//========================================================================================
//  
//  $File: $
//  
//  Owner: Guillaume
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2005 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#ifndef __XPBOID_h__
#define __XPBOID_h__

#include "SDKDef.h"

// Company:
#define kXPBOCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kXPBOCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kXPBOPluginName		"XPubConnection"			// Name of this plug-in.
#define kXPBOPrefixNumber	0x11bc00 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kXPBOVersion		"18.0.0"						// Version of this plug-in (for the About Box).
#define kXPBOAuthor			"Trias Développement"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kXPBOPrefixNumber above to modify the prefix.)
#define kXPBOPrefix		RezLong(kXPBOPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kXPBOStringPrefix	SDK_DEF_STRINGIZE(kXPBOPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kXPBOMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kXPBOMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kXPBOPluginID, kXPBOPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kXPubConnexionBoss, kXPBOPrefix + 0)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kXPBOBoss, kXPBOPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IXPUBCONNEXION, kXPBOPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBOINTERFACE, kXPBOPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kXPubConnexionImpl, kXPBOPrefix + 0 )
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kXPBOImpl, kXPBOPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kXPBOAboutActionID, kXPBOPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kXPBOActionID, kXPBOPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kXPBOWidgetID, kXPBOPrefix + 25)


// "About Plug-ins" sub-menu:
#define kXPBOAboutMenuKey			kXPBOStringPrefix "kXPBOAboutMenuKey"
#define kXPBOAboutMenuPath		kSDKDefStandardAboutMenuPath kXPBOCompanyKey

// "Plug-ins" sub-menu:
#define kXPBOPluginsMenuKey 		kXPBOStringPrefix "kXPBOPluginsMenuKey"
#define kXPBOPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kXPBOCompanyKey kSDKDefDelimitMenuPath kXPBOPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kXPBOTargetMenuPath kXPBOPluginsMenuPath
#define kXPBOErrConnexionFailed	kXPBOStringPrefix "kXPBOErrConnexionFailed"

// Menu item positions:


// Initial data format version numbers
#define kXPBOFirstMajorFormatNumber  RezLong(1)
#define kXPBOFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kXPBOCurrentMajorFormatNumber kXPBOFirstMajorFormatNumber
#define kXPBOCurrentMinorFormatNumber kXPBOFirstMinorFormatNumber

#endif // __XPBOID_h__

//  Code generated by DollyXs code generator
