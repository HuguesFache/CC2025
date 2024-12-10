/*
//
//	Author: Wilfried LEFEVRE
//
//	Date: 8-Fevrier-2008
//
//  File : IXPubClientUtils.h
//
*/

#ifndef __IXPubClientUtils__
#define __IXPubClientUtils__

#include "IPMUnknown.h"
#include "XPBCID.h"
#include "IXPubConnexion.h"

class IDocument;

/**	Interface for XPubClient utilities
 */
class IXPubClientUtils : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IXPUBCLIENTUTILS };

	virtual ErrorCode OuvrirPub(const PubInfoStruct& pub) =0;

	virtual bool16 VerifierPub(IDocument * pubDoc, PubInfoStruct& pubInfo) =0;

	virtual ErrorCode GenererPreview(IDocument * pubDoc) =0;

	virtual ErrorCode PdfExport(IDocument * doc, const PubInfoStruct& pubInfo) =0;

	virtual ErrorCode RassemblerImages(IDocument * doc) =0;
	
	virtual ErrorCode RassemblerPolices(IDocument * doc) =0;

	virtual ErrorCode SetPersistentString(UIDRef target, PMIID interfaceID, const PMString& value) =0;

	virtual double GetAsDouble(const PMString& value) =0;
	
	virtual ErrorCode PackageExport(const IDocument * doc, IDFile& folderFile) =0;
};

#endif // __IXPubClientUtils__