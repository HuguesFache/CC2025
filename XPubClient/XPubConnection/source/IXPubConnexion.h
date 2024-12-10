/*
//
//	Author: Wilfried LEFEVRE
//
//	Date: 12-Mai-2006
//
//  File : IXPubConnexion.h
//
*/

#ifndef __IXPubConnexion__
#define __IXPubConnexion__

#include "IPMUnknown.h"
#include "XPBOID.h"

#include "K2Vector.h"
#include "K2Vector.tpp"
//#include "XPubStub.h"

typedef struct listepubstruct
{
public:

	typedef object_type data_type;

	PMString IDPub;
	PMString Etat;
	PMString Name;

} ListePubStruct;

typedef struct pubinfo
{
public:

	typedef object_type data_type;

	PMString IDPub;
	PMString IDSociete;		
	PMString ContactName;	
	PMString ContactPhone;		
	PMString ContactCelullar;		
	PMString ContactEmail;		
	PMString Largeur, Hauteur;
	PMString Dossier;
	bool16 BAT;
	PMString CadreRefOrigX;
    PMString CadreRefOrigY;
    PMString CadreRefAngle;
    PMString CadreRefLargeur;
    PMString CadreRefHauteur;
    PMString CadreRefPolice;
    PMString CadreRefCorps;
	PMString CadreAlign, CadreOffsetX, CadreOffsetY;
    bool16 CadreRefExterieur;
	PMString FondPerduH, FondPerduB, FondPerduG ,FondPerduD;
	PMString FichierPDF, FichierPreview;

} PubInfoStruct;

/**	Interface for XPub web services.
 */
class IXPubConnexion : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IXPUBCONNEXION };

	virtual void SetServerAddress(const PMString& address) =0;

	virtual void SetClientType(const bool16& principal) =0;
    
	virtual bool Login(const PMString& login, const PMString& password, const PMString& client, PMString& Reponse, PMString& PrenomNom) =0;
	
	virtual bool ListePub(const PMString& title, const PMString& edition, const PMString& status, const PMString& dateparfrom, const PMString& datepar, const PMString& user, K2Vector<ListePubStruct>& PubList) =0;

	virtual bool ListeTitre(K2Vector<PMString>& listeTitre) =0;

	virtual bool ListeEdition(const PMString& titre, K2Vector<PMString>& listeEdt) =0;

	virtual bool ListeEtat(K2Vector<PMString>& listeEtat) =0;

	virtual bool InfoPub(const PMString& idPub, PubInfoStruct& infoPub) =0;

	virtual bool ModifChampPub(const PMString& idPub, const PMString& champ, const PMString& val, const PMString& user) =0;

	virtual bool ListePubRecherche(const PMString& idPub, const PMString& user, K2Vector<ListePubStruct>& PubList) =0;
		
	virtual bool ListeDateAvant (K2Vector<PMString>& listeDateDebut, int32& indexSelected) =0;

	virtual bool ListeDateApres   (K2Vector<PMString>& listeDateFin,   int32& indexSelected) =0;
    
    virtual bool GetPDFPub(const PMString& IDPub, PMString& pdfPath) =0;
	
    virtual PMString convert_UTF8_to_PMString (const char *text)=0;

};

#endif // __IXPubConnexion__