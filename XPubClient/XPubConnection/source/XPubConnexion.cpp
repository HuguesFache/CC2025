/*
//
//	Author: Wilfried LEFEVRE
//
//	Date: 12-Mai-2006
//
//  File : XPubConnexion.cpp
//
*/

#include "VCPluginHeaders.h"

#include "A_USCOREWebServiceBinding.nsmap"
#include <stdlib.h>
#include "StringUtils.h"

// Interface includes:

// General includes:
#include "CPMUnknown.h"

#include "StringUtils.h"
#include "ErrorUtils.h"
#include "Utils.h"

// Project includes:
#include "XPBOID.h"
#include "IXPubConnexion.h"
#include "IXPubClientUtils.h"

using namespace std;

#include "CAlert.h"

/**	XPubConnexion	
*/
class XPubConnexion : public CPMUnknown<IXPubConnexion>
{
	public:
		XPubConnexion(IPMUnknown* boss);

		virtual ~XPubConnexion();

		virtual void SetServerAddress(const PMString& address);

        virtual void SetClientType(const bool16& principal);
    
        virtual bool GetPDFPub(const PMString& IDPub, PMString& pdfPath);
    
        virtual bool Login(const PMString& login, const PMString& password, const PMString& client, PMString& Reponse, PMString& PrenomNom);

		virtual bool ListePub(const PMString& title, const PMString& edition, const PMString& status, const PMString& dateparfrom, const PMString& datepar, const PMString& user, K2Vector<ListePubStruct>& PubList);

		virtual bool ListeTitre(K2Vector<PMString>& listeTitre);

		virtual bool ListeEdition(const PMString& titre, K2Vector<PMString>& listeEdt);

		virtual bool ListeEtat(K2Vector<PMString>& listeEtat);

		virtual bool InfoPub(const PMString& idPub, PubInfoStruct& infoPub);

		virtual bool ModifChampPub(const PMString& idPub, const PMString& champ, const PMString& val, const PMString& user);

		virtual bool ListePubRecherche(const PMString& idPub, const PMString& user, K2Vector<ListePubStruct>& PubList);

		virtual bool ListeDateAvant (K2Vector<PMString>& listeDateDebut, int32& indexSelected);

		virtual bool ListeDateApres   (K2Vector<PMString>& listeDateFin,   int32& indexSelected);

        virtual PMString convert_UTF8_to_PMString (const char *text);

    private:

		PMString serverAddress, clientType;

		void HandleWebServiceError(struct soap * soap);

		struct soap soap;	// allocate gSOAP runtime environment		
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XPubConnexion, kXPubConnexionImpl)

/* Constructor
*/
XPubConnexion::XPubConnexion(IPMUnknown* boss) : 	
CPMUnknown<IXPubConnexion>(boss), serverAddress("http://192.168.75.6:8081/4DSOAP/", PMString::kUnknownEncoding)
{
	//soap_init2(&soap, SOAP_IO_KEEPALIVE, SOAP_IO_KEEPALIVE);   // must initialize
	soap_init(&soap);
	
	// GD 2010 ++
	soap.connect_timeout = 5;	// par defaut vaut 75 sec, trop long.
	// GD 2010 --
}

/* Destructor.
*/
XPubConnexion::~XPubConnexion()
{
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
}

void XPubConnexion::SetServerAddress(const PMString& address)
{
	serverAddress = address;
}

void XPubConnexion::SetClientType(const bool16& principal)
{
	if(principal)
		clientType = "ClientPrincipal";
	else
		clientType = "ClientTiers";
}

void XPubConnexion::HandleWebServiceError(struct soap * soap)
{
	PMString error = kNullString;

	PMString debug("SOAP ERROR:");
	debug.AppendNumber(soap->error);
	debug += "\r\nFault:";
	if(soap->fault)
		debug += soap->fault->faultstring;
	else 
		debug += "NONE";
	//CAlert::ErrorAlert(debug);

	if(soap->error == SOAP_FAULT)
	{
		error = PMString(soap->fault->faultstring, PMString::kUnknownEncoding);
	}
	else
		error = PMString(kXPBOErrConnexionFailed);

	ErrorUtils::PMSetGlobalErrorCode(kFailure, &error);
}

bool XPubConnexion::Login(const PMString& login, const PMString& password, const PMString& client, PMString& Reponse, PMString& PrenomNom)
{
	struct ns1__WS_USCOREIdentificationResponse response;
    
	string loginParam(login.GetPlatformString().c_str());
	string pwdParam(password.GetPlatformString().c_str());
	string clientParam(client.GetPlatformString().c_str());
	
    bool success = (soap_call_ns1__WS_USCOREIdentification(&soap, serverAddress.GetPlatformString().c_str(), NULL, loginParam, pwdParam, clientParam, response) == SOAP_OK);
	
	if(success)
	{
		Reponse = convert_UTF8_to_PMString(response.Reponse.c_str());
		PrenomNom = convert_UTF8_to_PMString(response.PrenomNom.c_str());
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::GetPDFPub(const PMString& IDPub, PMString& pdfPath)
{
	struct soap soap;	// allocate gSOAP runtime environment
    soap_init(&soap);	// must initialize
    
	string id(IDPub.GetPlatformString().c_str());
#ifdef WINDOWS
	string platform = "PC";
#else
	string platform = "MAC";
#endif
    
	string response;
    
	bool success = (soap_call_ns1__WS_USCOREGetPathMEPFilePDF(&soap, serverAddress.GetPlatformString().c_str(), NULL, id, platform, clientType.GetPlatformString().c_str(), response) == 0);
    
	if(success)
	{
		pdfPath = convert_UTF8_to_PMString(response.c_str());
	}
    
	return success;
}


bool XPubConnexion::ListePub(const PMString& title, const PMString& edition, const PMString& status, const PMString& dateparfrom, const PMString& datepar, const PMString& user, K2Vector<ListePubStruct>& PubList)
{
	ns1__WS_USCOREListePub args;
	ns1__WS_USCOREListePubResponse response;

	string titleParam(title.GetPlatformString().c_str());
	string editionParam(edition.GetPlatformString().c_str());
	string statusParam(status.GetPlatformString().c_str());
	string numpageParam("");
	string dateparfromParam(dateparfrom.GetPlatformString().c_str());
	string dateparParam(datepar.GetPlatformString().c_str());
	string UserParam(user.GetPlatformString().c_str());
    
	bool success = (soap_call_ns1__WS_USCOREListePub(&soap, serverAddress.GetPlatformString().c_str(), NULL,
		titleParam, editionParam, statusParam, numpageParam, dateparfromParam, dateparParam,
		UserParam, response) == SOAP_OK);
	
	if(success)
	{
		for(int32 i = 0 ; i < response.NbValAds ; ++i)
		{
			ListePubStruct theAdd;
			theAdd.IDPub = convert_UTF8_to_PMString(response.IDPub->__ptr[i].c_str());
			theAdd.Etat = convert_UTF8_to_PMString(response.Etat->__ptr[i].c_str());
			theAdd.Name = convert_UTF8_to_PMString(response.Soc->__ptr[i].c_str());
			PubList.push_back(theAdd);
		}
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::ListeTitre(K2Vector<PMString>& listeTitre)
{
	struct ns1__WS_USCOREListeTitreResponse response;

	bool success = (soap_call_ns1__WS_USCOREListeTitre(&soap, serverAddress.GetPlatformString().c_str(), NULL, response) == SOAP_OK);

	if(success)
	{		
		for(int32 i = 0 ; i < response.NbValTitres ; ++i)
			listeTitre.push_back(convert_UTF8_to_PMString(response.Titre->__ptr[i].c_str()));
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::ListeEdition(const PMString& titre, K2Vector<PMString>& listeEdt)
{
	string filtreTitre(titre.GetPlatformString().c_str());
	
	struct ns1__WS_USCOREListeEditionsResponse response;

	bool success = (soap_call_ns1__WS_USCOREListeEditions(&soap, serverAddress.GetPlatformString().c_str(), NULL, filtreTitre, response) == SOAP_OK);

	if(success)
	{		
		for(int32 i = 0 ; i < response.NbEditions ; ++i)
			listeEdt.push_back(convert_UTF8_to_PMString(response.Editions->__ptr[i].c_str()));
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::ListeEtat(K2Vector<PMString>& listeEtat)
{
	struct ns1__WS_USCOREListeEtatsPubResponse response;

	bool success = (soap_call_ns1__WS_USCOREListeEtatsPub(&soap, serverAddress.GetPlatformString().c_str(), NULL, response) == SOAP_OK);

	if(success)
	{		
		for(int32 i = 0 ; i < response.NbValEtatsPub ; ++i)
			listeEtat.push_back(convert_UTF8_to_PMString(response.EtatPub->__ptr[i].c_str()));
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::InfoPub(const PMString& idPub, PubInfoStruct& infoPub)
{
	string id(idPub.GetPlatformString().c_str());;
	
#ifdef WINDOWS
	string machine = "PC";
#else
	string machine = "MAC";
#endif
	string client = "xpubclient";

	struct ns1__WS_USCOREPubResponse response;

	bool success = (soap_call_ns1__WS_USCOREPub(&soap, serverAddress.GetPlatformString().c_str(), NULL, id, machine, client, response) == SOAP_OK);

	if(success)
	{
		infoPub.IDPub = idPub;
		infoPub.IDSociete = convert_UTF8_to_PMString(response.Customer.c_str());
		infoPub.ContactCelullar = convert_UTF8_to_PMString(response.MobileCustomer.c_str());
		infoPub.ContactEmail = convert_UTF8_to_PMString(response.EMailCustomer.c_str());
		infoPub.ContactName = convert_UTF8_to_PMString(response.ContactName.c_str());
		infoPub.ContactPhone = convert_UTF8_to_PMString(response.TelephoneCustomer.c_str());
		infoPub.Hauteur = convert_UTF8_to_PMString(response.Hauteur.c_str());
		// Test sur hauteur nulle, ‡ remplacer par une valeur par defaut
		if(Utils<IXPubClientUtils>()->GetAsDouble(infoPub.Hauteur) == 0)
			infoPub.Hauteur = "50";
		infoPub.Largeur = convert_UTF8_to_PMString(response.Largeur.c_str());
		infoPub.BAT = response.BAT;
		infoPub.Dossier = convert_UTF8_to_PMString(response.Dossier.c_str());
		infoPub.CadreRefOrigX = convert_UTF8_to_PMString(response.CadreRefOrigX.c_str());
		infoPub.CadreRefOrigY = convert_UTF8_to_PMString(response.CadreRefOrigY.c_str());
		infoPub.CadreRefAngle = convert_UTF8_to_PMString(response.CadreRefAngle.c_str());
		infoPub.CadreRefLargeur = convert_UTF8_to_PMString(response.CadreRefLargeur.c_str());
		infoPub.CadreRefHauteur = convert_UTF8_to_PMString(response.CadreRefHauteur.c_str());
		infoPub.CadreRefPolice = convert_UTF8_to_PMString(response.CadreRefPolice.c_str());
		infoPub.CadreRefCorps = convert_UTF8_to_PMString(response.CadreRefCorps.c_str());
		infoPub.CadreRefExterieur = response.CadreRefExterieur;
		infoPub.FondPerduH= convert_UTF8_to_PMString(response.EPSMargeHaut.c_str());
		infoPub.FondPerduB= convert_UTF8_to_PMString(response.EPSMargeBas.c_str());
		infoPub.FondPerduG= convert_UTF8_to_PMString(response.EPSMargeGauche.c_str());
		infoPub.FondPerduD= convert_UTF8_to_PMString(response.EPSMargeDroite.c_str());
		infoPub.FichierPreview = convert_UTF8_to_PMString(response.Visu.c_str());
		infoPub.FichierPDF = convert_UTF8_to_PMString(response.FichierEPS.c_str());
		infoPub.CadreAlign = convert_UTF8_to_PMString(response.CadreRefJustifHoriz.c_str());
		infoPub.CadreOffsetX = convert_UTF8_to_PMString(response.CadreRefOffsetX.c_str());
		infoPub.CadreOffsetY = convert_UTF8_to_PMString(response.CadreRefOffsetY.c_str());

// DEBUG
		CharCounter pos = infoPub.FichierPDF.IndexOfString(".eps");
		if(pos != -1)
		{
			infoPub.FichierPDF.Remove(pos, 4);
			infoPub.FichierPDF.Insert(PMString(".pdf",PMString::kUnknownEncoding), pos);
		}

		pos = infoPub.FichierPreview.IndexOfString(".pct");
		if(pos != -1)
		{
			infoPub.FichierPreview.Remove(pos, 4);
			infoPub.FichierPreview.Insert(PMString(".jpg",PMString::kUnknownEncoding), pos);
		}
// DEBUG	

	}
	else
	{
		HandleWebServiceError(&soap);
	}
    return success;
}

bool XPubConnexion::ModifChampPub(const PMString& idPub, const PMString& champ, const PMString& val, const PMString& user)
{
    string id(idPub.GetPlatformString().c_str());
	string field(champ.GetPlatformString().c_str());
	string value(val.GetPlatformString().c_str());
	string user_(user.GetPlatformString().c_str());
	
#ifdef WINDOWS
	string machine = "PC";
#else
	string machine = "MAC";
#endif
	string client = "xpubclient";

	string response;

	bool success = (soap_call_ns1__WS_USCOREModifChamp(&soap, serverAddress.GetPlatformString().c_str(), NULL, id, field, value, user_, response) == SOAP_OK);

	if(success)
	{
		
	}
	else
	{
		HandleWebServiceError(&soap);
	}
	
	return success;
}

bool XPubConnexion::ListePubRecherche(const PMString& idPub, const PMString& user, K2Vector<ListePubStruct>& PubList)
{
	string id(idPub.GetPlatformString().c_str());
    string user_(user.GetPlatformString().c_str());
    
	struct ns1__WS_USCOREListePubSeuleResponse response;

	bool success = (soap_call_ns1__WS_USCOREListePubSeule(&soap, serverAddress.GetPlatformString().c_str(), NULL, id, user_, response) == SOAP_OK);

	if(success)
	{
		ListePubStruct theAdd;
		if(!response.IDPub.empty())	{
			theAdd.IDPub = convert_UTF8_to_PMString(response.IDPub.c_str());
			theAdd.Etat = convert_UTF8_to_PMString(response.Etat.c_str());
			theAdd.Name = convert_UTF8_to_PMString(response.Soc.c_str());
			PubList.push_back(theAdd);
		}
	}
	else	
		HandleWebServiceError(&soap);
	
	
	soap_destroy(&soap);
	soap_end(&soap);
	return success;
}

bool  XPubConnexion::ListeDateAvant (K2Vector<PMString>& listeDateDebut, int32& indexSelected)
{
	struct ns1__WS_USCOREListeDateAvantResponse response;

	bool success = (soap_call_ns1__WS_USCOREListeDateAvant(&soap, serverAddress.GetPlatformString().c_str(), NULL, response) == SOAP_OK);
	if(success){		
		indexSelected = response.Index;
		for(int32 i = 0 ; i < response.StrDateParution->__size; ++i) 
			listeDateDebut.push_back(convert_UTF8_to_PMString(response.StrDateParution->__ptr[i].c_str()));
	}
	else	
		HandleWebServiceError(&soap);	
	
	soap_destroy(&soap);
	soap_end(&soap);
	return success;
}

bool  XPubConnexion::ListeDateApres(K2Vector<PMString>& listeDateFin,   int32& indexSelected)
{
	struct ns1__WS_USCOREListeDateApresResponse response;
	bool success = (soap_call_ns1__WS_USCOREListeDateApres(&soap, serverAddress.GetPlatformString().c_str(), NULL, response) == SOAP_OK);
	if(success){
		indexSelected = response.Index;
		for(int32 i = 0 ; i < response.StrDateParution->__size ; ++i)
			listeDateFin.push_back(convert_UTF8_to_PMString(response.StrDateParution->__ptr[i].c_str()));
	}
	else
		HandleWebServiceError(&soap);
	
	soap_destroy(&soap);
	soap_end(&soap);
    return kFalse;

}


PMString XPubConnexion::convert_UTF8_to_PMString (const char *text)
{
#if WINDOWS
	WideString myWideString(text);
	PMString myString("", PMString::kDontTranslateDuringCall);
	myString.Insert(myWideString);
	return myString;
#else
	PMString myString("",PMString::kDontTranslateDuringCall);
	int32 len = strlen(text);
	wchar_t myWchar[len];
	swprintf(myWchar, strlen(text)+1, L"%s", text);
	WideString myWideString(myWchar);
	myString.Insert(myWideString);
	return myString;
#endif
}


