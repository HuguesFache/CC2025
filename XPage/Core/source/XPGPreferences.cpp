#include "VCPluginHeaders.h"

// Interface includes:
#include "IXPGPreferences.h"

// General includes:
#include "CAlert.h"

// Project includes:
#include "XPGID.h"


/** Implements IStartupShutdownService; purpose is to install the idle task.
*/
class XPGPreferences : public CPMUnknown<IXPGPreferences>
{
public:

	/**	Constructor
		@param boss
	 */
	XPGPreferences(IPMUnknown* boss);

	/**	Destructor
	 */
	virtual ~XPGPreferences();

	// General section
	virtual PMString GetRacineArbo() const;
	virtual void SetRacineArbo(const PMString& str);

	// Assemblage section
	virtual PMString GetEtatAssemblage() const;
	virtual void SetEtatAssemblage(const PMString& str);

	virtual PMString GetCheminAssemblage() const;
	virtual void SetCheminAssemblage(const PMString& str);

	// Textes section
	virtual PMString GetTypoDir() const;
	virtual void SetTypoDir(const PMString& str);

	virtual PMString GetCheminTextes() const;
	virtual void SetCheminTextes(const PMString& str);

	virtual PMString GetMatchingParDefaut() const;
	virtual void SetMatchingParDefaut(const PMString& str);

	virtual Tag GetTagExport(const PMString& styleName) const;
	virtual PMString GetStyleName(const PMString& tag) const;
	virtual void SetListeTagExport(const TagList& l, const Tag& aDefaultTag);

	// Formes section
	virtual PMString GetCheminFormes() const;
	virtual void SetCheminFormes(const PMString& str);

	virtual bool16 GetCreationForme() const;
	virtual void SetCreationForme(const bool16& b);

	virtual void SetCheminAssemblageCartons(const PMString& b);
	virtual PMString GetCheminAssemblageCartons();

	// XML section
	virtual PMString GetEtatPlace() const;
	virtual void SetEtatPlace(const PMString& str);

	virtual PMString GetEtatNonPlace() const;
	virtual void SetEtatNonPlace(const PMString& str);

	// XRail section
	virtual PMString GetXRail_URL() const;
	virtual void SetXRail_URL(const PMString& str);


	//Statut des Article a ne pas verrouiller 
	virtual PMString GetStatus() const;
	virtual void SetStatus(const PMString& str);
	
	//Import des credits et des legendes
	virtual bool16 GetImportLegende() const;
	virtual void SetImportLegende(const bool16& b);

	virtual bool16 GetImportCredit() const;
	virtual void SetImportCredit(const bool16& b);

	// Matching Styles Preferences

	virtual PMString GetBalisesXMLPath() const;
	virtual void SetBalisesXMLPath(const PMString& str);

	virtual void SetListeMatchingTag(K2Vector<Tag> listTags);
	virtual K2Vector<Tag> GetListeMatchingTag();

	//Preferences globales
	virtual int32 GetExportXML() const;
	virtual void SetExportXML(const int32& b);

	virtual int32 GetGestionIDMS() const;
	virtual void SetGestionIDMS(const int32& b);

	virtual int32 GetIDMS_MAJIDMS() const;
	virtual void SetIDMS_MAJIDMS(const int32& b);

	virtual int32 GetIDMSALLBLOCS() const;
	virtual void SetIDMSALLBLOCS(const int32& b);

    virtual PMString GetNoUpdateFile() const;
    virtual void SetNoUpdateFile(const PMString& str);
    
private:

	PMString racineArbo, defaut1, defaut2, copieDir, noUpdateFile;
	K2Vector<KeyValuePair<PMString, PMString> > statusMap;
	K2Vector<Tag> matchingTagsList;
	TagList tagList;
	Tag defaultTag;
	bool16 creationForme;
	bool16 importCredit, importLegende;
	int32 exportXML, gestionIDMS, majIDMS, idmsAllBlocs;
	PMString etatAssemblage, cheminAssemblage;
	PMString typoDir, cheminTxt, matchingDefaut;
	PMString status;
    PMString cheminFormes, cheminAssemblageCartons;
	PMString etatPlace, etatNonPlace;
	PMString urlXRail;
	PMString balisesXMLPath;
};

CREATE_PMINTERFACE(XPGPreferences, kXPGPreferencesImpl)


/**	Constructor
 */
	XPGPreferences::XPGPreferences(IPMUnknown* boss) : CPMUnknown<IXPGPreferences>(boss)
{
	racineArbo = defaut1 = defaut2 = copieDir = kNullString;
	etatAssemblage = cheminAssemblage = kNullString;
	typoDir = cheminTxt = matchingDefaut = kNullString;
	tagList.clear();
	defaultTag = Tag();
	cheminFormes = etatPlace = etatNonPlace = cheminAssemblageCartons = kNullString;
	creationForme = kFalse;
	urlXRail = kNullString;
	balisesXMLPath = kNullString;
    status=kNullString;
	matchingTagsList.clear();
}

/**	Destructor
 */
XPGPreferences::~XPGPreferences()
{
}

PMString XPGPreferences::GetRacineArbo() const
{
	return racineArbo;
}

void XPGPreferences::SetRacineArbo(const PMString& str)
{
	racineArbo = str;
}

PMString XPGPreferences::GetEtatAssemblage() const
{
	return etatAssemblage;
}

void XPGPreferences::SetEtatAssemblage(const PMString& str)
{
	etatAssemblage = str;
}

PMString XPGPreferences::GetCheminAssemblage() const
{
	return cheminAssemblage;
}

void XPGPreferences::SetCheminAssemblage(const PMString& str)
{
	cheminAssemblage = str;
}

PMString XPGPreferences::GetTypoDir() const
{
	return typoDir;
}

void XPGPreferences::SetTypoDir(const PMString& str)
{
	typoDir = str;
}

PMString XPGPreferences::GetCheminTextes() const
{
	return cheminTxt;
}

void XPGPreferences::SetCheminTextes(const PMString& str)
{
	cheminTxt = str;
}

PMString XPGPreferences::GetMatchingParDefaut() const
{
	return matchingDefaut;
}

void XPGPreferences::SetMatchingParDefaut(const PMString& str)
{
	matchingDefaut = str;
}

Tag XPGPreferences::GetTagExport(const PMString& styleName) const
{
	if (styleName.IsEmpty())
		return defaultTag;

	for (int32 i = 0; i < tagList.size(); ++i)
	{
		if (styleName.Compare(kFalse, tagList[i].paraStyleName) == 0)
		{
			return tagList[i];
		}
	}

	return defaultTag;
}


PMString XPGPreferences::GetStyleName(const PMString& tag) const
{
	if (tag.IsEmpty())
		return kNullString;

	for (int32 i = 0; i < tagList.size(); ++i)
	{
		if (tag.Compare(kFalse, tagList[i].value) == 0)
		{
			return tagList[i].paraStyleName;
		}
	}

	return kNullString;
}


void XPGPreferences::SetListeTagExport(const TagList& l, const Tag& aDefaultTag)
{
	tagList = l;
	defaultTag = aDefaultTag;
}

PMString XPGPreferences::GetCheminFormes() const
{
	return cheminFormes;
}

void XPGPreferences::SetCheminFormes(const PMString& str)
{
	cheminFormes = str;
}

bool16 XPGPreferences::GetCreationForme() const
{
	return creationForme;
}

void XPGPreferences::SetCreationForme(const bool16& b)
{
	creationForme = b;
}

PMString XPGPreferences::GetEtatPlace() const
{
	return etatPlace;
}

void XPGPreferences::SetEtatPlace(const PMString& str)
{
	etatPlace = str;
}

PMString XPGPreferences::GetEtatNonPlace() const
{
	return etatNonPlace;
}

void XPGPreferences::SetEtatNonPlace(const PMString& str)
{
	etatNonPlace = str;
}

PMString XPGPreferences::GetXRail_URL() const
{
	return urlXRail;
}

void XPGPreferences::SetXRail_URL(const PMString& str)
{
	urlXRail = str;
}

PMString XPGPreferences::GetStatus() const
{
	return status;
}

void XPGPreferences::SetStatus(const PMString& str)
{
	status = str;
}

bool16 XPGPreferences::GetImportLegende() const
{
	return importLegende;
}

void XPGPreferences::SetImportLegende(const bool16& b)
{
	importLegende = b;
}
bool16 XPGPreferences::GetImportCredit() const
{
	return importCredit;
}
void XPGPreferences::SetImportCredit(const bool16& b)
{
	importCredit = b;
}


PMString XPGPreferences::GetBalisesXMLPath() const
{
	return balisesXMLPath;
}

void XPGPreferences::SetBalisesXMLPath(const PMString& str)
{
	balisesXMLPath = str;
}

void XPGPreferences::SetListeMatchingTag(K2Vector<Tag> listTags)
{
	this->matchingTagsList = listTags;
}

K2Vector<Tag> XPGPreferences::GetListeMatchingTag()
{
	return matchingTagsList;
}

void XPGPreferences::SetCheminAssemblageCartons(const PMString& b)
{
	cheminAssemblageCartons = b;
}

PMString XPGPreferences::GetCheminAssemblageCartons()
{
	return cheminAssemblageCartons;
}

//Preferences globales
int32 XPGPreferences::GetExportXML() const
{
	return exportXML;
}

void XPGPreferences::SetExportXML(const int32& b)
{
	exportXML = b;
}

int32 XPGPreferences::GetGestionIDMS() const
{
	return gestionIDMS;
}

void XPGPreferences::SetGestionIDMS(const int32& b)
{
	gestionIDMS = b;
}
int32 XPGPreferences::GetIDMS_MAJIDMS() const
{
	return majIDMS;
}

void XPGPreferences::SetIDMS_MAJIDMS(const int32& b)
{
	majIDMS = b;
}
int32 XPGPreferences::GetIDMSALLBLOCS() const
{
	return idmsAllBlocs;
}

void XPGPreferences::SetIDMSALLBLOCS(const int32& b)
{
	idmsAllBlocs = b;
}

PMString XPGPreferences::GetNoUpdateFile() const
{
    return noUpdateFile;
    
}
void XPGPreferences::SetNoUpdateFile(const PMString& str)
{
    noUpdateFile = str;
}
