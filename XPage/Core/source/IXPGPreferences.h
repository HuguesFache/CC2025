
#ifndef __IXPGPreferences_h__
#define __IXPGPreferences_h__

#include "IPMUnknown.h"
#include "XPGID.h"
#include "K2Vector.h"
#include "K2Vector.tpp"
#include "KeyValuePair.h"

//========================================================================================
// CLASS IXPGPreferences
//========================================================================================

typedef struct tag
{
	typedef object_type data_type;

	WideString value;
	PMString paraStyleName;
	int32 order;
	tag() : value("DEFAULT"), paraStyleName(kNullString), order(0) {}

} Tag;



typedef K2Vector<Tag> TagList;

typedef struct fontvariants
{
	PMString plain, bold, italic, bolditalic;

	fontvariants(const PMString& plainStyleName,
		const PMString& boldStyleName,
		const PMString& italicStyleName,
		const PMString& bolditalicStyleName)
	{
		plain = plainStyleName;
		bold = boldStyleName;
		italic = italicStyleName;
		bolditalic = bolditalicStyleName;
	}
} FontVariants;

typedef K2Vector<KeyValuePair<PMString, FontVariants> > FontList;


class IXPGPreferences : public IPMUnknown
{
public:

	enum { kDefaultIID = IID_IXPGPREFERENCES };

	// General section
	virtual PMString GetRacineArbo() const = 0;
	virtual void SetRacineArbo(const PMString& str) = 0;

	// Assemblage section
	virtual PMString GetEtatAssemblage() const = 0;
	virtual void SetEtatAssemblage(const PMString& str) = 0;

	virtual PMString GetCheminAssemblage() const = 0;
	virtual void SetCheminAssemblage(const PMString& str) = 0;

	// Textes section
	virtual PMString GetTypoDir() const = 0;
	virtual void SetTypoDir(const PMString& str) = 0;

	virtual PMString GetCheminTextes() const = 0;
	virtual void SetCheminTextes(const PMString& str) = 0;

	virtual PMString GetMatchingParDefaut() const = 0;
	virtual void SetMatchingParDefaut(const PMString& str) = 0;

	virtual Tag GetTagExport(const PMString& styleName) const = 0;
	virtual PMString GetStyleName(const PMString& tag) const = 0;
	virtual void SetListeTagExport(const TagList& l, const Tag& aDefaultTag) = 0;

	// Formes section
	virtual PMString GetCheminFormes() const = 0;
	virtual void SetCheminFormes(const PMString& str) = 0;

	virtual bool16 GetCreationForme() const = 0;
	virtual void SetCreationForme(const bool16& b) = 0;

	virtual void SetCheminAssemblageCartons(const PMString& b) = 0;
	virtual PMString GetCheminAssemblageCartons() = 0;

	// XML section
	virtual PMString GetEtatPlace() const = 0;
	virtual void SetEtatPlace(const PMString& str) = 0;

	virtual PMString GetEtatNonPlace() const = 0;
	virtual void SetEtatNonPlace(const PMString& str) = 0;

	// XRail section
	virtual PMString GetXRail_URL() const = 0;
	virtual void SetXRail_URL(const PMString& str) = 0;

	//Statut des Article a ne pas verrouiller 
	virtual PMString GetStatus() const = 0;
	virtual void SetStatus(const PMString& str) = 0;

	//Import des credits et des legendes
	virtual bool16 GetImportLegende() const = 0;
	virtual void SetImportLegende(const bool16& b) = 0;

	virtual bool16 GetImportCredit() const = 0;
	virtual void SetImportCredit(const bool16& b) = 0;

	// Matching Styles Preferences

	virtual PMString GetBalisesXMLPath() const = 0;
	virtual void SetBalisesXMLPath(const PMString& str) = 0;

	virtual void SetListeMatchingTag(K2Vector<Tag> listTags) = 0;
	virtual K2Vector<Tag> GetListeMatchingTag() = 0;

	//Preferences globales
	virtual int32 GetExportXML() const = 0;
	virtual void SetExportXML(const int32& b) = 0;

	virtual int32 GetGestionIDMS() const = 0;
	virtual void SetGestionIDMS(const int32& b) = 0;

	virtual int32 GetIDMS_MAJIDMS() const = 0;
	virtual void SetIDMS_MAJIDMS(const int32& b) = 0;

	virtual int32 GetIDMSALLBLOCS() const = 0;
	virtual void SetIDMSALLBLOCS(const int32& b) = 0;

    // Don't create update file
    virtual PMString GetNoUpdateFile() const =0;
    virtual void SetNoUpdateFile(const PMString& str) =0;
    
    
	
};

#endif // __IXPGPreferences_h__
