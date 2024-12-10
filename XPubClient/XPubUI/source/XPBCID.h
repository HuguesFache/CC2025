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


#ifndef __XPBCID_h__
#define __XPBCID_h__

#include "SDKDef.h"

// Company:
#define kXPBCCompanyKey		"Trias Developpement"		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kXPBCCompanyValue	"Trias Developpement"		// Company name displayed externally.

// Plug-in:
#define kXPBCPluginName		"XPubClient"			// Name of this plug-in.
#define kXPBCPrefixNumber	0x11bb00 				// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kXPBCVersion		"18.0.0"					// Version of this plug-in (for the About Box).
#define kXPBCAuthor			"Trias Developpement"	// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kXPBCPrefixNumber above to modify the prefix.)
#define kXPBCPrefix		RezLong(kXPBCPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kXPBCStringPrefix	SDK_DEF_STRINGIZE(kXPBCPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kXPBCMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kXPBCMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// Variables de compilation
#define STRICT_CHECK 0 // definit si le contr�le de la taille de la pub est bloquant ou non

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kXPBCPluginID, kXPBCPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kXPBCActionComponentBoss, kXPBCPrefix + 0)
DECLARE_PMID(kClassIDSpace, kXPBCSetStringCmdBoss, kXPBCPrefix + 2)
DECLARE_PMID(kClassIDSpace, kXPBCLoginDialogBoss, kXPBCPrefix + 3)
DECLARE_PMID(kClassIDSpace, kXPBCDocEventBoss, kXPBCPrefix + 4)
DECLARE_PMID(kClassIDSpace, kXPBCStartupShutdownBoss, kXPBCPrefix + 5)

DECLARE_PMID(kClassIDSpace, kXPBCAnnoncesPanelWidgetBoss, kXPBCPrefix + 6)
DECLARE_PMID(kClassIDSpace, kXPBCAnnoncesElementWidgetBoss, kXPBCPrefix + 7)
DECLARE_PMID(kClassIDSpace, kXPBCAnnoncesBoxWidgetBoss,     kXPBCPrefix + 10)
DECLARE_PMID(kClassIDSpace, kXPBCAnnoncesNodeWidgetBoss,	kXPBCPrefix + 11)

//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kXPBCBoss, kXPBCPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IBOOL_LOGIN, kXPBCPrefix + 0)
DECLARE_PMID(kInterfaceIDSpace, IID_ISTRINGDATA_IPSERVEUR, kXPBCPrefix + 1)
DECLARE_PMID(kInterfaceIDSpace, IID_ISTRINGDATA_USERNAME, kXPBCPrefix + 2)
DECLARE_PMID(kInterfaceIDSpace, IID_IPUB_ID, kXPBCPrefix + 3)
DECLARE_PMID(kInterfaceIDSpace, IID_IXPUBCLIENTUTILS, kXPBCPrefix + 4)
DECLARE_PMID(kInterfaceIDSpace, IID_IPUB_PREVIEW, kXPBCPrefix + 5)
DECLARE_PMID(kInterfaceIDSpace, IID_ISTRINGDATA_STYLEPDF, kXPBCPrefix + 6)
DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCSHADOWEVENTHANDLER, kXPBCPrefix + 7)
DECLARE_PMID(kInterfaceIDSpace, IID_IPUB_BAT, kXPBCPrefix + 8)
DECLARE_PMID(kInterfaceIDSpace, IID_IANNONCESLIST, kXPBCPrefix + 9)

//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IXPBCINTERFACE, kXPBCPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kXPBCActionComponentImpl, kXPBCPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kXPBCPubListBoxWidgetEHImpl, kXPBCPrefix + 2)
DECLARE_PMID(kImplementationIDSpace, kXPBCLoginDialogControllerImpl, kXPBCPrefix + 3)
DECLARE_PMID(kImplementationIDSpace, kXPBCSetStringCmdImpl, kXPBCPrefix + 4)
DECLARE_PMID(kImplementationIDSpace, kXPBCDocServiceProviderImpl, kXPBCPrefix + 5)
DECLARE_PMID(kImplementationIDSpace, kXPBCDocResponderImpl, kXPBCPrefix + 6)
DECLARE_PMID(kImplementationIDSpace, kXPBCStartupShutdownImpl, kXPBCPrefix + 7)
DECLARE_PMID(kImplementationIDSpace, kXPBCStartupShutdownProviderImpl, kXPBCPrefix + 8)
DECLARE_PMID(kImplementationIDSpace, kXPBCIPServerImpl, kXPBCPrefix + 9)
DECLARE_PMID(kImplementationIDSpace, kXPBCUserNameImpl, kXPBCPrefix + 10)
DECLARE_PMID(kImplementationIDSpace, kXPBCUtilsImpl, kXPBCPrefix + 11)
DECLARE_PMID(kImplementationIDSpace, kXPBCPersistPubPreviewImpl, kXPBCPrefix + 12)
DECLARE_PMID(kImplementationIDSpace, kXPBStylePDFImpl, kXPBCPrefix + 13)
DECLARE_PMID(kImplementationIDSpace, kXPBCPersistPubIDImpl, kXPBCPrefix + 14)
DECLARE_PMID(kImplementationIDSpace, kXPBCPubPersistBatImpl, kXPBCPrefix + 15)

DECLARE_PMID(kImplementationIDSpace, kXPBCAnnoncesPanelObserverImpl, kXPBCPrefix + 20)
DECLARE_PMID(kImplementationIDSpace, kXPBCAnnoncesTreeViewAdapterImpl, kXPBCPrefix + 21)
DECLARE_PMID(kImplementationIDSpace, kXPBCAnnoncesListImpl, kXPBCPrefix + 22)
DECLARE_PMID(kImplementationIDSpace, kXPBCAnnoncesTVWidgetMgrImpl, kXPBCPrefix + 23)
DECLARE_PMID(kImplementationIDSpace, kXPBCAnnonceDataImpl, kXPBCPrefix + 24)

//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kXPBCImpl, kXPBCPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kXPBCAboutActionID, kXPBCPrefix + 0)
DECLARE_PMID(kActionIDSpace, kXPBCAnnoncesPanelWidgetActionID, kXPBCPrefix + 1)
DECLARE_PMID(kActionIDSpace, kXPBCPopupAboutThisActionID, kXPBCPrefix + 2)

//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 4)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kXPBCActionID, kXPBCPrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kXPBCUsernameLblWidgetID, kXPBCPrefix + 1)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnConnect, kXPBCPrefix + 2)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnRequest, kXPBCPrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnOpen, kXPBCPrefix + 6)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnSave, kXPBCPrefix + 8)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnSaveClose, kXPBCPrefix + 10)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnBat, kXPBCPrefix + 12)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnTerminate, kXPBCPrefix + 14)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnDisconnect, kXPBCPrefix + 16)
DECLARE_PMID(kWidgetIDSpace, kPubListParentWidgetId, kXPBCPrefix + 19)
DECLARE_PMID(kWidgetIDSpace, kPubListCol1WidgetID, kXPBCPrefix + 20)
DECLARE_PMID(kWidgetIDSpace, kPubListCol2WidgetID, kXPBCPrefix + 21)
DECLARE_PMID(kWidgetIDSpace, kPubListCol3WidgetID, kXPBCPrefix + 22)
DECLARE_PMID(kWidgetIDSpace, kPubListElementRsrcID, kXPBCPrefix + 23)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_PopTitre, kXPBCPrefix + 24)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_PopEdition, kXPBCPrefix + 25)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_PopEtatPub, kXPBCPrefix + 26)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_PopDateParuDeb, kXPBCPrefix + 27)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_PopDateParuFin, kXPBCPrefix + 28)
DECLARE_PMID(kWidgetIDSpace, kSIDPubEdtWidgetID, kXPBCPrefix + 29)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnSearch, kXPBCPrefix + 30)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblClientName, kXPBCPrefix + 33)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblAdress, kXPBCPrefix + 40)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblTel, kXPBCPrefix + 41)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblPort, kXPBCPrefix + 42)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblEmail, kXPBCPrefix + 43)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblWidth, kXPBCPrefix + 44)
DECLARE_PMID(kWidgetIDSpace, kWidgetID_LblDepth, kXPBCPrefix + 45)
DECLARE_PMID(kWidgetIDSpace, kXPBCWidgetID_BtnOpenPubFolder, kXPBCPrefix + 47)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesPanelWidgetID, kXPBCPrefix + 48)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesBoxWidgetID, kXPBCPrefix + 49)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesParentWidgetId, kXPBCPrefix + 50)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesColIDWidgetID, kXPBCPrefix + 51)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesColStatusWidgetID, kXPBCPrefix + 52)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesColNameWidgetID, kXPBCPrefix + 53)
DECLARE_PMID(kWidgetIDSpace, kXPBCAnnoncesNodeWidgetId, kXPBCPrefix + 189)

// GD 2015 DECLARE_PMID(kWidgetIDSpace, kXPBCLoginDialogResourceID, kXPBCPrefix + 49)
DECLARE_PMID(kWidgetIDSpace, kXPBCLoginDialogWidgetID, kXPBCPrefix + 50)
DECLARE_PMID(kWidgetIDSpace, kLoginUserEdtWidgetID, kXPBCPrefix + 51)
DECLARE_PMID(kWidgetIDSpace, kLoginPasswordEdtWidgetID, kXPBCPrefix + 52)



// "About Plug-ins" sub-menu:
#define kXPBCAboutMenuKey			kXPBCStringPrefix "kXPBCAboutMenuKey"
#define kXPBCAboutMenuPath			kSDKDefStandardAboutMenuPath kXPBCCompanyKey

// "Plug-ins" sub-menu:
#define kXPBCPluginsMenuKey 		kXPBCStringPrefix "kXPBCPluginsMenuKey"
#define kXPBCPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kXPBCCompanyKey kSDKDefDelimitMenuPath kXPBCPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kXPBCAboutBoxStringKey					kXPBCStringPrefix	"kXPBCAboutBoxStringKey"
#define kXPBCPanelTitleKey						kXPBCStringPrefix	"kXPBCPanelTitleKey"
#define kXPBCStaticTextKey						kXPBCStringPrefix	"kXPBCStaticTextKey"
#define kXPBCInternalPopupMenuNameKey			kXPBCStringPrefix	"kXPBCInternalPopupMenuNameKey"
#define kXPBCTargetMenuPath						kXPBCInternalPopupMenuNameKey

#define kXPBCNotConnectedStringKey				kXPBCStringPrefix	"kXPBCNotConnectedStringKey"
#define kXPBCStringKey_EVERYTITLES				kXPBCStringPrefix	"kXPBCStringKey_EVERYTITLES"
#define kXPBCStringKey_EVERYEDITIONS			kXPBCStringPrefix	"kXPBCStringKey_EVERYEDITIONS"
#define kXPBCStringKey_EVERYSTATUS				kXPBCStringPrefix	"kXPBCStringKey_EVERYSTATUS"
#define kXPBCParamDialogTitleKey				kXPBCStringPrefix	"kXPBCParamDialogTitleKey"
#define kParamLblStringKey						kXPBCStringPrefix	"kParamLblStringKey"
#define kXPBCLoginDialogTitleKey				kXPBCStringPrefix	"kXPBCLoginDialogTitleKey"
#define kLoginUserLblStringKey					kXPBCStringPrefix	"kLoginUserLblStringKey"
#define kLoginPasswordLblStringKey				kXPBCStringPrefix	"kLoginPasswordLblStringKey"
#define kXPBCPubAlreadyOpenStringKey			kXPBCStringPrefix	"kXPBCPubAlreadyOpenStringKey"
#define kXPBCErrorOpenStringKey					kXPBCStringPrefix	"kXPBCErrorOpenStringKey"
#define kXPBCErrorCreerPubStringKey				kXPBCStringPrefix	"kXPBCErrorCreerPubStringKey"
#define kXPBCErrorInconsistentDimStringKey		kXPBCStringPrefix	"kXPBCErrorInconsistentDimStringKey"
#define kXPBCErrorMissingCartoucheStringKey		kXPBCStringPrefix	"kXPBCErrorMissingCartoucheStringKey"
#define kXBPCErrorPDFStyleNotFoundKey			kXPBCStringPrefix	"kXBPCErrorPDFStyleNotFoundKey"
#define kXPBCErrorCopyLinkKey					kXPBCStringPrefix	"kXPBCErrorCopyLinkKey"
#define kXPBCErrorSaveStringKey					kXPBCStringPrefix	"kXPBCErrorSaveStringKey"
#define kXPBCErrorCreerCartoucheStringKey		kXPBCStringPrefix	"kXPBCErrorCreerCartoucheStringKey"
#define kXPBCErrorPDFNotCreatedStringKey		kXPBCStringPrefix	"kXPBCErrorPDFNotCreatedStringKey"
#define kXPBCErrorTerminateStringKey			kXPBCStringPrefix	"kXPBCErrorTerminateStringKey"
#define kXPBCErrorPackgeExportStringKey			kXPBCStringPrefix	"kXPBCErrorPackgeExportStringKey"
#define kXPBCErrorINIFileNotFoundKey			kXPBCStringPrefix	"kXPBCErrorINIFileNotFoundKey"

#define kLogErrorMessage1Key					kXPBCStringPrefix	"kLogErrorMessage1Key"
#define kLogErrorMessage2Key					kXPBCStringPrefix	"kLogErrorMessage2Key"
#define kLogErrorMessage3Key					kXPBCStringPrefix	"kLogErrorMessage3Key"
#define kLogErrorMessage4Key					kXPBCStringPrefix	"kLogErrorMessage4Key"
#define kLogErrorMessage5Key					kXPBCStringPrefix	"kLogErrorMessage5Key"
#define kLogErrorMessage6Key					kXPBCStringPrefix	"kLogErrorMessage6Key"

// Webservice strings
#define		kXPBCVerrouStringKey		"Verrou"
#define		kXPBCStatusStringKey		"Status"
#define		kXPBCEnCoursStringKey		"En Cours"
#define		kXPBCBATStringKey			"BAT"
#define		kXPBCFinieStringKey			"Finie"
#define		kXPBCVraiStringKey			"Vrai"
#define		kXPBCFauxStringKey			"Faux"
#define		kXPBCHauteurStringKey		"Hauteur"		

// Menu item positions:

#define	kXPBCSeparator1MenuItemPosition		10.0
#define kXPBCAboutThisMenuItemPosition		11.0

#define kXPBCPanelResourceID 1000

#define kXPBCIconConnectResID                   700
#define kXPBCIconDisconnectResID                701
#define kXPBCIconRequestResID                   702
#define kXPBCIconOpenResID                      703
#define kXPBCIconSaveResID                      704
#define kXPBCIconSaveCloseResID                 705
#define kXPBCIconBatResID                       706
#define kXPBCIconTerminateResID                 707
#define kXPBCIconSearchResID                    708
#define kXPBCIconClientNameResID                709
#define kXPBCIconAdressResID                    710
#define kXPBCIconTelResID                       711
#define kXPBCIconPortResID                      712
#define kXPBCIconEmailResID                     713
#define kXPBCIconWidthResID                     714
#define kXPBCIconHeightResID                    715
#define kXPBCIconOpenPubFolderResID             716
#define kXPBCAnnoncesNodeWidgetRsrcID			717

// Initial data format version numbers
#define kXPBCFirstMajorFormatNumber  RezLong(1)
#define kXPBCFirstMinorFormatNumber  RezLong(0)

// GD 2015 ++
// Ressources ID
#define kXPBCLoginDialogResourceID			kSDKDefPanelResourceID
#define kXPBCAnnoncesBoxResourceID			kSDKDefPanelResourceID + 2
#define kXPBCAnnoncesElementRsrcID			kSDKDefPanelResourceID + 3
// GD 2015 --

// Data format version numbers for the PluginVersion resource 
#define kXPBCCurrentMajorFormatNumber kXPBCFirstMajorFormatNumber
#define kXPBCCurrentMinorFormatNumber kXPBCFirstMinorFormatNumber

#endif // __XPBCID_h__

//  Code generated by DollyXs code generator

