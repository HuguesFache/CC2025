#include "VCPluginHeaders.h"

// Interface includes:
#include "IPMStream.h"

// General includes:
#include "CPMUnknown.h"

// Project includes:
#include "XRCID.h"

#include "PMRect.h"

#include "ErrorUtils.h"
#include <string>
#include "A_USCOREWebServiceBinding.nsmap"
#include "CAlert.h"
#include <stdlib.h>
#include "StringUtils.h"

#include "IWebServices.h"


using namespace std;

/**    XRailWebServices
 */
class XRailWebServices : public CPMUnknown<IWebServices>
{
public:
    XRailWebServices(IPMUnknown* boss);
    
    virtual ~XRailWebServices();
    
    virtual void SetServerAddress(const PMString& address);
    
    virtual bool VerifLogin(const PMString& login, const PMString& pwd, PMString& userID, PMString& errorCode);
    
    virtual bool GetStatusPreflight(const PMString& userID, K2Vector<PMString>& listStatus, K2Vector<bool>& checkError);
    
    virtual bool GetLevel1List(const int32& userID, K2Vector<PMString>& listLevel1Names, K2Vector<int32>& listLevel1ID);
    
    virtual bool SetPageStatus(const int32& IDPage, const PMString& status, const PMString& userID);
    
    virtual bool SetVerrouPage(const K2Vector<int32>& IDPage, int lock, const PMString& userID, PMString& status, bool errorInDoc);
    
    virtual bool GetPictCommonFolders(const PMString& IDPage, PMString& ClientPictPath, PMString& Scale);
    
    virtual bool GetPreviewInfos(const int32& IDPage, const PMString& login, PMString& ClientPictPath, int32& Scale, int32& resolution);
    
    virtual bool MoveGeneratedFiles (const K2Vector<PMString>& FilePath, const K2Vector<int32>& IDPage);
    
    virtual bool MovePDFFile (const int32& IDPage, const PMString& fileName, const bool externe, const int32& IDUser);
    
    virtual bool GetPDFFileName(const int32& IDPage, PMString& profilPDF, PMString& pdfFileName);
    
    virtual bool GetLevel2List(const int32& level1ID, K2Vector<PMString>& listLevel2Names, K2Vector<int32>& listLevel2ID);
    
    virtual bool GetLevel3List(const int32& level2ID, K2Vector<PMString>& listLevel3Names, K2Vector<int32>& listLevel3ID);
    
    virtual bool GetLevel4List(const int32& level3ID, int32 nbPages, K2Vector<PMString>& listLevel4Names, K2Vector<int32>& listLevel4ID);
    
    virtual bool GetPagesIDS(const int32& pageID, K2Vector<int32>& listPagesIDS);
    
    virtual bool GetDocFileNameWithID(const int32& IDPage, PMString& docFileName);
    
    virtual bool GetListeOrdresRigueur(const K2Vector<int32>& IDPages,  K2Vector<int32>& TabPagesIDS,
                                       K2Vector<int32>& TabFolios, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDS,
                                       K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs, K2Vector<PMReal>& TabX,
                                       K2Vector<PMReal>& TabY, K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabCheminImage, K2Vector<bool16>& TabVerrouPos);
    
    virtual bool MAJPub(const PMString& IDPub, const int32& IDPage, const PMString& x, const PMString& y, const PMString& l,const PMString& h, const PMString& path );
    
    virtual bool GetListeOrdresMarbre (const int32& IDPage, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDs,
                                       K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabProds, K2Vector<PMString>& TabSurfaces,
                                       K2Vector<PMString>& TabEmpls, K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs,
                                       K2Vector<PMString>& TabPaths);
        
    virtual bool GetDroitsMEP(const int32 idUser, int& VoirPDF, int& VoirVignettes);
        
    virtual bool GetDocNameForUnlink(const int32& IDPage, PMString& docFileName);
    
	virtual bool StoryLockChanged(const PMString &idArt , const PMString & flagLock);
	
    virtual bool GetStoryStates(    const PMString & idArt ,
                                const PMString & user,
                                IDValueList& StatusValuesList,
                                K2Vector<PMString>& ColorsList,
                                K2Vector<PMString>& StatusOrderList,
                                K2Vector<PMString>& StatusRoleList,
                                K2Vector<PMString>& StatusRegleList,
                                K2Vector<PMString>& CommentaireList,
                                PMString&           result);
    
    virtual bool GetGroupsStatusList(    const PMString & IDStory ,
                                     const PMString & author,
                                     const PMString & status,
                                     IDValueList& StatusValuesList,
                                     PMString& result);
    
    virtual bool GetUsersStatusList(    const PMString & idArt ,
                                    const PMString & author,
                                    const PMString & status,
                                    const PMString & group,
                                    IDValueList& usersValuesList,
                                    PMString& result);
    
	virtual bool StoryStatusChanged(	const PMString & storyID , 
									const PMString & statusID,  	
									const PMString & userID,
									const PMString & groupID,
									PMString& result);
	
    virtual bool GetParutionList(const PMString& idTitle,
                                 IDValueList& parutionList);
    
    virtual bool GetRubriqueList(const PMString& idTitle,
                                 IDValueList& rubList);
    
    virtual bool GetSsRubriqueList(const PMString& idRub,
                                   IDValueList& ssrubList);
    
    virtual    bool GetAuthorizedStatus(IDValueList& userNames,
                                     K2Vector<PMString>& usersFullNames,
                                     const PMString& IdStatus,
                                     const int32 IdTitle,
                                     const PMString& IdSection);
    
        virtual bool CreateStory(const PMString& idUser,
                             const PMString& subject,
                             const PMString& idSection,
                             const PMString& subSection,
                             const PMString& author,
                             const int32 idPage,
                             const PMString& PosX,
                             const PMString& PosY,
                             const PMString& hauteur,
                             const PMString& largeur,
                             const int32 nbPhotos,
                             const PMString& nbSignesEstim,
                             PMString& idStory,
                             PMString& storyFullPath,
                             PMString& result);
    
    virtual bool DeleteStory(const PMString& idStory);
    
    virtual bool StoryContentChange(const PMString& idArticle,
                                    const int32 nbSignes);
    
    virtual bool StoryGetInfo(const PMString&  idStory,
                              PMString&  section,
                              PMString&  ssSection,
                              PMString&  libStatus,
                              PMString&  colorStatus,
                              PMString&  incaPath,
                              PMString&  sujet,
                              PMString&  proprio,
                              PMString&  reponse) ;
    
    virtual bool GetPageParents(const int32 pageID,
                                PMString& titreID,
                                PMString& titreLib,
                                PMString& editionID,
                                PMString& editionLib,
                                PMString& parutionID,
                                PMString& parutionLib);
    
    
    virtual bool GetMEPStoriesListWithIDPage(const int32 idParutionMEP,
                                   const int32 idPage,
                                   const PMString& idSection,
                                   const PMString& idSubSection,
                                   K2Vector<PMString>& IDStories,
                                   K2Vector<PMString>& ListSujets,
                                   K2Vector<PMString>& CheminArticles,
                                   K2Vector<PMString>& CouleurStatus,
                                   K2Vector<PMString>& LibStatus,
                                   K2Vector<PMString>& NomProprietaire,
                                   K2Vector<PMString>& IdSection,
                                   K2Vector<PMString>& IdSsSection);
    
    virtual bool ModifStory(const PMString & artID,
                            PMString& X,
                            PMString& Y,
                            PMString& Largeur,
                            PMString& Hauteur,
                            int32& idPage,
                            PMString& result);
    
    
    virtual bool StoryLink(const PMString& idUser,
                           const PMString& storyID,
                           const int32 IDPage,
                           const PMString& Classeur,
                           const PMString& NomForme,
                           const PMString& PosX,
                           const PMString& PosY,
                           const PMString& Hauteur,
                           const PMString& Largeur,
                           const int32 typeArticle,
                           const PMString& sectionID,
                           const PMString& SousSection,
                           const PMString& Sujet,
                           const PMString& Auteur,
                           PMString& storyFullPath,
                           PMString& result);
    
    
    virtual bool StoryParutionChange(const int32 idPage,
                                     const PMString& idArt,
                                     const PMString& idRub,
                                     const PMString& idSsRub,
                                     PMString& result);
    
	virtual bool StoryGetLocked(const PMString& idArt, 
								PMString& verrou);
	
    virtual bool GetNewIDResaRedac(int32& id);
    
    virtual bool SaveArticles(const PMString from, const bool16 modifContenu, K2Vector<int32>&ListePages, K2Vector<int32>&TabIDStories, K2Vector<int32>& TabTypes,
                              K2Vector<int32>& TabNbSignes, K2Vector<PMString>& TabClasseurs, K2Vector<PMString>& TabCartons,
                              K2Vector<int32>& IDStoriesByBloc, K2Vector<PMString>& TabX, K2Vector<PMString>& TabY,
                              K2Vector<PMString>& TabLargeur, K2Vector<PMString>& TabHauteur, K2Vector<int32>& TabIDPage,
                              K2Vector<PMString>& ListeImages, K2Vector<WideString>& ListeLegendes, K2Vector<WideString>& ListeCredits, PMString& result);
    
    virtual bool GetListeIDMS(const K2Vector<int32>& IDPages, K2Vector<int32>& TabIDStories, K2Vector<int32>& TabPagesIDS,
                             K2Vector<PMReal>& TabX, K2Vector<PMReal>& TabY, K2Vector<PMString>& TabIDMSpaths, K2Vector<PMString>& TabXMLStorypaths, K2Vector<PMString>& TabSubjects);
    
    virtual bool GetPrefsXPage(const int32 &fromServeur, int32& prefs_ExportXML, int32& prefs_GestionIDMS, int32& prefs_IDMSMAJIDMS, int32& prefs_IDMSALLBLOCS, int32& prefs_ImportLegendes, int32& prefs_ImportCredits);

    virtual bool GetPrefsXRail(const int32& fromServeur, int32& prefs_ImportPub, int32& prefs_PubProp, int32& prefs_PDFMultipages, int32& prefs_ExportIDML);

    virtual PMString convert_UTF8_to_PMString (const char *text);
    
private:
    PMString serverAddress;
    struct soap soap;    // allocate gSOAP runtime environment
    void HandleWebServiceError(PMString name);
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
 */
CREATE_PMINTERFACE(XRailWebServices, kXRCWebServicesImpl)

/* Constructor
 */
XRailWebServices::XRailWebServices(IPMUnknown* boss) :
CPMUnknown<IWebServices>(boss), serverAddress("http://192.168.75.6:8080/4DSOAP/",PMString::kUnknownEncoding)
{
    soap_init(&soap);   // must initialize
    soap.connect_timeout = 5;
}

/* Destructor.
 */
XRailWebServices::~XRailWebServices()
{
    soap_destroy(&soap);
    soap_end(&soap);
    soap_done(&soap);
}

void XRailWebServices::SetServerAddress(const PMString& address)
{
    serverAddress = address;
}

//----------------------------------------------------------------------------------------------------------------
//Gestionnaire d'erreur
//----------------------------------------------------------------------------------------------------------------
void XRailWebServices::HandleWebServiceError(PMString methodeName){
    PMString error = kNullString;
    if(soap.error == SOAP_FAULT){
        // Filter error string to remove useless information for the users
        int32 pos = error.IndexOfCharacter(PlatformChar(':'));
        if(pos >= 0){
            error.Remove(0, pos + 1);
            error.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);
        }
    }
    else
        error = "Erreur de connexion: "+ methodeName;
    
    ErrorUtils::PMSetGlobalErrorCode(kFailure, &error);
}

//----------------------------------------------------------------------------------------------------------------
// verification du login et du mot de passe
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::VerifLogin(const PMString& login, const PMString& pwd, PMString& userID, PMString& errorCode)
{
    string loginParam(login.GetPlatformString().c_str());
    string pwdParam(pwd.GetPlatformString().c_str());
    struct ns1__WS_USCOREVerifLoginResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREVerifLogin(&soap, serverAddress.GetPlatformString().c_str(),NULL, loginParam, pwdParam, response) == 0);
    
    if(!success){
        HandleWebServiceError("VerifLogin");
    }
    else {
        userID = convert_UTF8_to_PMString(response.IDPseudo.c_str());
        errorCode = convert_UTF8_to_PMString(response.LogOK.c_str());
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des etats de pages autorises pour cet utilisateur (dlg de changement d'etat XRail)
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetStatusPreflight(const PMString& userID, K2Vector<PMString>& listStatus, K2Vector<bool>& checkError)
{
    string theUserID(userID.GetPlatformString().c_str());
    struct ns1__WS_USCOREListStatesPreflightResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListStatesPreflight(&soap, serverAddress.GetPlatformString().c_str(), NULL,theUserID,response) == 0);
    
    if(!success){
        HandleWebServiceError("GetStatusPreflight");
    }
    else
    {
        for(int32 i = 0 ; i < response.Etats->__size ; ++i)
        {
            listStatus.push_back(convert_UTF8_to_PMString(response.Etats->__ptr[i].c_str()));
            checkError.push_back(response.VerifError->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des titres autorises pour un utilisateur, XRail, dlg de lien d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetLevel1List(const int32& userID, K2Vector<PMString>& listLevel1Names, K2Vector<int32>& listLevel1ID)
{
    
    struct ns1__WS_USCOREListeTitresProdResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListeTitresProd(&soap, serverAddress.GetPlatformString().c_str(), NULL, userID, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetLevel1List");
    }
    else
    {
        for(int32 i = 0 ; i < response.ListeTitresProd_USCORENOMS->__size ; ++i)
        {
            listLevel1Names.push_back(convert_UTF8_to_PMString(response.ListeTitresProd_USCORENOMS->__ptr[i].c_str()));
            listLevel1ID.push_back(response.ListeTitresProd_USCOREIDS->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des parus pour un titre, XRail, dlg de lien d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetLevel2List(const int32& level1ID, K2Vector<PMString>& listLevel2Names, K2Vector<int32>& listLevel2ID)
{
    struct ns1__WS_USCOREListeParutionsProdResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListeParutionsProd(&soap, serverAddress.GetPlatformString().c_str(), NULL, level1ID, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetLevel2List");
    }
    else
    {
        for(int32 i = 0 ; i < response.ListeParutionsProd_USCORENOMS->__size ; ++i)
        {
            listLevel2Names.push_back(convert_UTF8_to_PMString(response.ListeParutionsProd_USCORENOMS->__ptr[i].c_str()));
            listLevel2ID.push_back(response.ListeParutionsProd_USCOREIDS->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des editions pour une paru, XRail, dlg de lien d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetLevel3List(const int32& level2ID, K2Vector<PMString>& listLevel3Names, K2Vector<int32>& listLevel3ID)
{
    
    struct ns1__WS_USCOREListeEditionsProdResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListeEditionsProd(&soap, serverAddress.GetPlatformString().c_str(), NULL, level2ID, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetLevel3List");
    }
    else
    {
        for(int32 i = 0 ; i < response.ListeEditionsProd_USCORENOMS->__size ; ++i)
        {
            listLevel3Names.push_back(convert_UTF8_to_PMString(response.ListeEditionsProd_USCORENOMS->__ptr[i].c_str()));
            listLevel3ID.push_back(response.ListeEditionsProd_USCOREIDS->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des documents pour une edition en fonction du nb de pages du document, XRail, dlg de lien d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetLevel4List(const int32& level3ID, int32 nbPages, K2Vector<PMString>& listLevel4Names, K2Vector<int32>& listLevel4ID)
{
    
    struct ns1__WS_USCOREListePagesProdResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListePagesProd(&soap, serverAddress.GetPlatformString().c_str(), NULL, level3ID, nbPages, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetLevel4List");
    }
    else
    {
        for(int32 i = 0 ; i < response.ListePagesProd_USCORENOMS->__size ; ++i)
        {
            listLevel4Names.push_back(convert_UTF8_to_PMString(response.ListePagesProd_USCORENOMS->__ptr[i].c_str()));
            listLevel4ID.push_back(response.ListePagesProd_USCOREIDS->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des ids de pages a appliquer lors de la liaison d'une page, XRail, dlg de lien d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetPagesIDS(const int32& pageID, K2Vector<int32>& listPagesIDS)
{
    
    struct ns1__WS_USCOREGetListeIDSDocResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREGetListeIDSDoc(&soap, serverAddress.GetPlatformString().c_str(), NULL, pageID, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetPagesIDS");
    }
    else
    {
        for(int32 i = 0 ; i < response.GetListeIDSDoc_USCOREIDS->__size ; ++i)
        {
            listPagesIDS.push_back(response.GetListeIDSDoc_USCOREIDS->__ptr[i]);
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Applique le changement d'etat d'une page, xrail, lors de la fermeture
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::SetPageStatus(const int32& IDPage, const PMString& status, const PMString& userID)
{
    bool    temp;
    WideString wideStatus(status);
    string theUserID(userID.GetPlatformString().c_str());
    string theStatus(wideStatus.begin(), wideStatus.end() );
    
    bool success = (soap_call_ns1__WS_USCORESetPageState(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, theStatus, theUserID, temp) == 0);
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// verrouille ou deverrouille la page dans xrail, ouverture ou fermeture du doc
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::SetVerrouPage(const K2Vector<int32>& IDPage, int lock, const PMString& userID, PMString& status, bool errorInDoc)
{
    
    string theUserID(userID.GetPlatformString().c_str());
    string response;
    ArrayOfint idPageList;
    idPageList.__size = IDPage.size();
    idPageList.__ptr = new int[idPageList.__size];
    for(int32 i = 0 ; i < IDPage.size() ; ++i)
    {
        idPageList.__ptr[i] = IDPage[i];
    }
    
    bool success = (soap_call_ns1__WS_USCORESetVerrouPageWithError(&soap, serverAddress.GetPlatformString().c_str(), NULL, &idPageList, lock, theUserID, errorInDoc, response) == 0);
    if(!success){
        HandleWebServiceError("SetVerrouPage");
    }
    else
    {
        status = convert_UTF8_to_PMString(response.c_str());
    }
    // clean up memory
    if(idPageList.__ptr)
        delete[] idPageList.__ptr;
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie le dossier dans lequel les vignettes doivent etre enregistrees, xrail, lors de la sauvegarde du document
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetPictCommonFolders(const PMString& IDPage, PMString& ClientPictPath, PMString& Scale)
{

    string thePageID(IDPage.GetPlatformString().c_str());
    struct ns1__WS_USCOREGetPictCommonFoldersResponse response;

    bool success = (soap_call_ns1__WS_USCOREGetPictCommonFolders(&soap, serverAddress.GetPlatformString().c_str(), NULL, thePageID, response) == 0);

    if (!success) {
        HandleWebServiceError("GetPictCommonFolders");
    }
    else
    {
        ClientPictPath = convert_UTF8_to_PMString(response.ClientPictPath.c_str());
        Scale = convert_UTF8_to_PMString(response.Scale.c_str());
    }
    return success;
}
//----------------------------------------------------------------------------------------------------------------
// renvoie le dossier dans lequel les vignettes doivent etre enregistrees, xrail, lors de la sauvegarde du document
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetPreviewInfos(const int32& IDPage, const PMString& login, PMString& ClientPictPath, int32& Scale, int32& resolution)
{

    struct ns1__WS_USCOREGetPictureFolderResponse response;

    string theLogin(login.GetPlatformString().c_str());
    bool success = (soap_call_ns1__WS_USCOREGetPictureFolder(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, theLogin, response) == 0);

    if (!success) {
        HandleWebServiceError("GetPreviewInfos");
    }
    else
    {
        ClientPictPath = convert_UTF8_to_PMString(response.ClientPictPath.c_str());
        Scale = response.Scale;
        resolution = response.Resolution;
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// demande a xrail de recuperer toutes les vignettes qui viennent d'etre generees
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::MoveGeneratedFiles (const K2Vector<PMString>& FilePath, const K2Vector<int32>& IDPage)
{
    
    ArrayOfstring filePathList;
    ArrayOfint idPageList;
    
    filePathList.__size = FilePath.size();
    idPageList.__size = IDPage.size();
    filePathList.__ptr = new string[filePathList.__size];
    idPageList.__ptr = new int[idPageList.__size];
    
    for(int32 i = 0 ; i < FilePath.size() ; ++i)
    {
        filePathList.__ptr[i] = string(FilePath[i].GetPlatformString().c_str());
        idPageList.__ptr[i] = IDPage[i];
    }
    
    bool temp;
    bool success = (soap_call_ns1__WS_USCOREMoveFileTableau(&soap, serverAddress.GetPlatformString().c_str(), NULL, &filePathList, &idPageList, temp) == 0);
    
    // clean up memory
    if(idPageList.__ptr)
        delete[] idPageList.__ptr;
    
    if(filePathList.__ptr)
    {
        delete[] filePathList.__ptr;
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// demande a xrail de recuperer le pdf qui vient d'etre genere
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::MovePDFFile (const int32& IDPage, const PMString& fileName, const bool externe, const int32& IDUser)
{
    
    string theFilePath(fileName.GetPlatformString().c_str());
    
    bool temp;
    
    bool success = (soap_call_ns1__WS_USCOREMovePDFFile(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, theFilePath, externe, IDUser, temp) == 0);
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// demande a xrail le nom, l'endroit et le profil pdf pour generer un pdf via le menu
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetPDFFileName(const int32& IDPage, PMString& profilPDF, PMString& pdfFileName)
{
    
    struct ns1__WS_USCOREGetPDFFileNameResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREGetPDFFileName(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetPDFFileName");
    }
    else
    {
        profilPDF = convert_UTF8_to_PMString(response.ProfilPDF.c_str());
        pdfFileName = convert_UTF8_to_PMString(response.PDFFileName.c_str());
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie le nom du document a ecraser avec l'id de la page, pour xrail, dlg de liaison d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetDocFileNameWithID(const int32& IDPage, PMString& docFileName)
{
    string response;
    
    bool success = (soap_call_ns1__WS_USCOREGetDocNameWithID(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetDocFileNameWithID");
    }
    else
    {
        docFileName = convert_UTF8_to_PMString(response.c_str());
    }
    return success;
}


//----------------------------------------------------------------------------------------------------------------
// idem qu'au dessus, avec l'option de rigueur en plus. Le WS au dessus sera supprime a terme
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetListeOrdresRigueur(const K2Vector<int32>& IDPages,  K2Vector<int32>& TabPagesIDS,
                                             K2Vector<int32>& TabFolios, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDS,
                                             K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs, K2Vector<PMReal>& TabX, K2Vector<PMReal>& TabY,
                                             K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabCheminImage, K2Vector<bool16>& TabVerrouPos)
{
    ArrayOfint idPageList;
    idPageList.__size = IDPages.size();
    idPageList.__ptr = new int[idPageList.__size];
    
    for(int32 i = 0 ; i < IDPages.size() ; ++i)
    {
        idPageList.__ptr[i] = IDPages[i];
    }
    
    struct ns1__WS_USCOREGetListeOrdresRigueurResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREGetListeOrdresRigueur(&soap, serverAddress.GetPlatformString().c_str(), NULL, &idPageList, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetListeOrdresRigueur");
    }
    else
    {
        for(int32 i = 0 ; i < response.PubsIDS->__size ; ++i)
        {
            TabResaIDs.push_back(convert_UTF8_to_PMString(response.OrdresIDS->__ptr[i].c_str()));
            TabPubsIDS.push_back(convert_UTF8_to_PMString(response.PubsIDS->__ptr[i].c_str()));
            TabPagesIDS.push_back(response.PageID->__ptr[i]);
            TabFolios.push_back(response.Folios->__ptr[i]);
            TabHauteurs.push_back(PMReal(strtod(response.Hauteurs->__ptr[i].c_str(),NULL)));
            TabLargeurs.push_back(PMReal(strtod(response.Largeurs->__ptr[i].c_str(),NULL)));
            TabX.push_back(PMReal(strtod(response.X->__ptr[i].c_str(),NULL)));
            TabY.push_back(PMReal(strtod(response.Y->__ptr[i].c_str(),NULL)));
            TabAnnonceurs.push_back(convert_UTF8_to_PMString(response.CustomerName->__ptr[i].c_str()));
            TabCheminImage.push_back(convert_UTF8_to_PMString(response.CheminImage->__ptr[i].c_str()));
            TabVerrouPos.push_back(response.Rigueur->__ptr[i]);
        }
    }
    
    // clean up memory
    if(idPageList.__ptr)
        delete[] idPageList.__ptr;
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// met a jour dans xrail les infos d'une pub (coordonnees + path)
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::MAJPub(const PMString& IDPub, const int32& IDPage, const PMString& x, const PMString& y, const PMString& l,const PMString& h, const PMString& path )
{
    string theIDPub(IDPub.GetPlatformString().c_str());
    
    struct ns1__WS_USCOREResaPubMAJPubResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREResaPubMAJPub(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                          theIDPub, IDPage, x.GetPlatformString().c_str(), y.GetPlatformString().c_str(), l.GetPlatformString().c_str(), h.GetPlatformString().c_str(), path.GetPlatformString().c_str(), response) == 0);
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// recupere la liste des pubs qui se trouvent au marbre depuis xrail
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetListeOrdresMarbre (const int32& IDPage, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDs,
                                             K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabProds, K2Vector<PMString>& TabSurfaces,
                                             K2Vector<PMString>& TabEmpls, K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs,
                                             K2Vector<PMString>& TabPaths)

{
    struct ns1__WS_USCOREResaPubGetListeMarbreResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREResaPubGetListeMarbre(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetListeOrdresMarbre");
    }
    else
    {
        for(int32 i = 0 ; i < response.PubGetListeMarbre_USCOREIDTraffic->__size ; ++i)
        {
            TabResaIDs.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCOREIDTraffic->__ptr[i].c_str()));
            TabPubsIDs.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCOREIDPub->__ptr[i].c_str()));
            TabAnnonceurs.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCOREAnnonceurs->__ptr[i].c_str()));
            TabProds.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCOREProduits->__ptr[i].c_str()));
            TabSurfaces.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCORESurfaces->__ptr[i].c_str()));
            TabEmpls.push_back(convert_UTF8_to_PMString(response.PubGetListeMarbre_USCOREEmplacements->__ptr[i].c_str()));
            TabHauteurs.push_back(response.PubGetListeMarbre_USCOREHauteurs->__ptr[i]);
            TabLargeurs.push_back(response.PubGetListeMarbre_USCORELargeurs->__ptr[i]);
            TabPaths.push_back(convert_UTF8_to_PMString(response.CheminImage->__ptr[i].c_str()));
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// renvoie la liste des droits utilisateur pour charger le menu xrail
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetDroitsMEP(const int32 idUser, int& VoirPDF, int& VoirVignettes)
{
    
    struct ns1__WS_USCOREGetDroitsMEPResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetDroitsMEP(&soap, serverAddress.GetPlatformString().c_str(), NULL, idUser, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetDroitsMEP");
    }
    else {
        VoirPDF = response.VoirPDF;
        VoirVignettes = response.VoirVignettes;
    }
    return success;
    
}


//----------------------------------------------------------------------------------------------------------------
// Dans XRail, recupere le nom du fichier a enregistrer si on fait un Unlink
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetDocNameForUnlink(const int32& IDPage, PMString& docFileName)
{
    string response;
    
    bool success = (soap_call_ns1__WS_USCOREGetDocNameForUnlink(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDPage, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetDocNameForUnlink");
    }
    else
    {
        docFileName = convert_UTF8_to_PMString(response.c_str());
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Pour Incopy, verrouille/deverrouille un article
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryLockChanged(const PMString &idArt , const PMString & flagLock)
{    
    string response;
/*	bool success = (soap_call_ns1__WS_USCOREStoryLockChanged(&soap, serverAddress.GetPlatformString().c_str(), NULL,
															 idArt.GetPlatformString().c_str(), flagLock.GetPlatformString().c_str(), response) == SOAP_OK);*/
	
	return true;
	
}

//----------------------------------------------------------------------------------------------------------------
// Pour Incopy, renvoie la liste des etats lors du changement d'etat d'un article. A VERIFIER
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetStoryStates(    const PMString & idArt ,
                                      const PMString & user,
                                      IDValueList& StatusValuesList,
                                      K2Vector<PMString>& ColorsList,
                                      K2Vector<PMString>& StatusOrderList,
                                      K2Vector<PMString>& StatusRoleList,
                                      K2Vector<PMString>& StatusRegleList,
                                      K2Vector<PMString>& CommentaireList,
                                      PMString&           result
                                      )
{
    PMString StatusID = kNullString,
    StatusName = kNullString;
    
    struct ns1__WS_USCOREGetStoryStatusListResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetStoryStatusList(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                               idArt.GetPlatformString().c_str(), user.GetPlatformString().c_str(),
                                                               response) == SOAP_OK);
    if(!success){
        HandleWebServiceError("GetStoryStates");
    }
    else{
        for(int32 i = 0 ; i < response.StatusList->__size ; ++i){
            
            StatusID.Append(convert_UTF8_to_PMString(response.StatusIDList->__ptr[i].c_str()));
            StatusID.SetTranslatable(kFalse);
            StatusName.Append(convert_UTF8_to_PMString(response.StatusList->__ptr[i].c_str()));
            StatusName.SetTranslatable(kFalse);
            StatusValuesList.push_back(IDValuePair(StatusID, StatusName));
            
            StatusID.clear();
            StatusName.clear();
            
            ColorsList.push_back(convert_UTF8_to_PMString(response.ColorsList->__ptr[i].c_str()));
            StatusOrderList.push_back(convert_UTF8_to_PMString(response.StatusOrderList->__ptr[i].c_str()));
            StatusRoleList.push_back(convert_UTF8_to_PMString(response.StatusRoleList->__ptr[i].c_str()));
            StatusRegleList.push_back(convert_UTF8_to_PMString(response.StatusRegleList->__ptr[i].c_str()));
            CommentaireList.push_back(convert_UTF8_to_PMString(response.CommentaireList->__ptr[i].c_str()));
        }
    }
    return success;
    
}

//----------------------------------------------------------------------------------------------------------------
// Pour Incopy, renvoie la liste des groupes lors du changement d'etat d'un article. A VERIFIER
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetGroupsStatusList(    const PMString & idArt ,
                                           const PMString & author,
                                           const PMString & status,
                                           IDValueList& groupsValuesList,
                                           PMString& result)
{
    
    PMString groupID, groupName;
    
    struct ns1__WS_USCOREGetGroupsStatusListResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetGroupsStatusList(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                                author.GetPlatformString().c_str() , idArt.GetPlatformString().c_str(),
                                                                status.GetPlatformString().c_str(), response) == SOAP_OK);
    
    if(!success){
        HandleWebServiceError("GetGroupsStatusList");
    }
    else{
        for(int32 i = 0 ; i < response.GroupDataList->__size ; ++i){
            
            groupID.Append(convert_UTF8_to_PMString(response.GroupDataList->__ptr[i].c_str()));
            groupID.SetTranslatable(kFalse);
            
            groupName.Append(convert_UTF8_to_PMString(response.GroupLabelList->__ptr[i].c_str()));
            groupName.SetTranslatable(kFalse);
            
            groupsValuesList.push_back(IDValuePair(groupID, groupName));
            
            groupID.clear();
            groupName.clear();
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Pour Incopy, renvoie la liste des etats lors du changement d'etat d'un article. A VERIFIER
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetUsersStatusList(    const PMString & author,
                                          const PMString & idArt ,
                                          const PMString & status,
                                          const PMString & group,
                                          IDValueList& usersValuesList,
                                          PMString& result)
{
    
    PMString userID, userName;
    
    struct ns1__WS_USCOREGetUsersStatusListResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetUsersStatusList(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                               author.GetPlatformString().c_str() , idArt.GetPlatformString().c_str(),
                                                               status.GetPlatformString().c_str(), group.GetPlatformString().c_str(),
                                                               response) == SOAP_OK);
    
    if(!success){
        HandleWebServiceError("GetUsersStatusList");
    }
    else{
        for(int32 i = 0 ; i < response.UserDataList->__size ; ++i){
            
            userID.Append(convert_UTF8_to_PMString(response.UserDataList->__ptr[i].c_str()));
            userID.SetTranslatable(kFalse);
            
            userName.Append(convert_UTF8_to_PMString(response.UserLabelList->__ptr[i].c_str()));
            userName.SetTranslatable(kFalse);
            
            usersValuesList.push_back(IDValuePair(userID, userName));
            
            userID.clear();
            userName.clear();
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Pour Incopy, change l'etat d'un article
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryStatusChanged(	const PMString & storyID, const PMString & statusID,  	
										  const PMString & userID, const PMString & groupID, 
										  PMString& result)
{
	
	string response;


	/*bool success = (soap_call_ns1__WS_USCOREStoryStatusChanged(&soap, serverAddress.GetPlatformString().c_str(), NULL,
																		  storyID.GetPlatformString().c_str(),
																		  statusID.GetPlatformString().c_str(), "", "", "", 
																		  userID.GetPlatformString().c_str(), 
																		  "", 
																		  response) == SOAP_OK);*/
	
	return true;
}

//----------------------------------------------------------------------------------------------------------------
// Renvoie la liste des parutions pour un titre donnee
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetParutionList(const PMString& idTitle, IDValueList& parutionList){
    
    
    PMString parutionID=kNullString;
    parutionID.SetTranslatable(kFalse);
    int32 idParution = idTitle.GetAsNumber ();
    struct ns1__WS_USCOREListeParutionsProdResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREListeParutionsProd(&soap, serverAddress.GetPlatformString().c_str(), NULL, idParution , response) == SOAP_OK);
    if(!success){
        HandleWebServiceError("GetParutionList");
    }
    else{
        // On verifie si les tableaux sont paralleles
        if(response.ListeParutionsProd_USCOREIDS->__size == response.ListeParutionsProd_USCORENOMS->__size){
            for(int32 i = 0 ; i < response.ListeParutionsProd_USCORENOMS->__size ; ++i){
                parutionID.AppendNumber(response.ListeParutionsProd_USCOREIDS->__ptr[i]);
                parutionList.push_back(IDValuePair(parutionID, convert_UTF8_to_PMString(response.ListeParutionsProd_USCORENOMS->__ptr[i].c_str())));
                parutionID.clear();
            }
        }
    }
    return success;
}



//----------------------------------------------------------------------------------------------------------------
// A VERIFIER, dans le dlg de creation d'une affectation, donne la liste des utilisateurs en fonction de l'id de l'article
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetAuthorizedStatus(IDValueList& userNames,
                                         K2Vector<PMString>& usersFullNames,
                                         const PMString& IdStatus,
                                         const int32 IdTitle,
                                         const PMString& IdSection)
{
    struct ns1__WS_USCOREGetAuthorizedStatusResponse response;
    
    PMString userID =kNullString;
    PMString userName =kNullString;
    PMString userFullName = kNullString;
    
    bool success = (soap_call_ns1__WS_USCOREGetAuthorizedStatus( &soap,
                                                                serverAddress.GetPlatformString().c_str(),
                                                                NULL, "ADM", IdStatus.GetPlatformString().c_str(), IdTitle,
                                                                IdSection.GetPlatformString().c_str(),
                                                                response) == SOAP_OK);
    
    if(!success){
        HandleWebServiceError("GetAuthorizedStatus");
    }
    else{
        int32 nbRub = response.UserName->__size;
        for(int32 i = 0 ; i < nbRub ; ++i)    {
            
            userID.AppendNumber(response.UserID->__ptr[i]);
            
            userName.Append(convert_UTF8_to_PMString(response.UserLogin->__ptr[i].c_str()));
            userName.SetTranslatable(kFalse);
            userFullName.Append(convert_UTF8_to_PMString(response.UserName->__ptr[i].c_str()));
            userFullName.Append(PMString(" "));
            userFullName.Append(convert_UTF8_to_PMString(response.UserFirstName->__ptr[i].c_str()));
            userFullName.SetTranslatable(kFalse);
            usersFullNames.push_back(userFullName);
            userNames.push_back(IDValuePair(userID, userName));
            userID.clear();
            userName.clear();
            userFullName.clear();
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Renvoie la liste des rubriques du titre
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetRubriqueList(const PMString& idTitle,IDValueList& rubList){
    
    int32 idRubrique = idTitle.GetAsNumber();
    
    struct ns1__WS_USCOREGetSectionsRedacResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetSectionsRedac(&soap, serverAddress.GetPlatformString().c_str(), NULL, idRubrique, response) == SOAP_OK);
    if(!success){
        HandleWebServiceError("GetRubriqueList");
    }
    else{
        // On verifie si les tableaux sont paralleles
        if(response.SectionID->__size == response.SectionName->__size){
            int32 nbRub = response.SectionName->__size;
            for(int32 i = 0 ; i < nbRub ; ++i)    {
                rubList.push_back(IDValuePair(convert_UTF8_to_PMString(response.SectionID->__ptr[i].c_str() ),
                                              convert_UTF8_to_PMString(response.SectionName->__ptr[i].c_str())));
            }
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Renvoie la liste des sous rubriques pour une rubrique
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetSsRubriqueList(const PMString& idRub, IDValueList& ssrubList){
    
    string param;
    param = idRub.GetPlatformString().c_str();
    struct ns1__WS_USCOREGetSousSectionsRedacResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetSousSectionsRedac(&soap, serverAddress.GetPlatformString().c_str(), NULL, param, response) == SOAP_OK);
    if(!success){
        HandleWebServiceError("GetSsRubriqueList");
    }
    else{
        // On verifie si les tableaux sont paralleles
        if(response.SousSectionID->__size == response.SousSectionName->__size){
            int32 nbSsrub = response.SousSectionName->__size;
            for(int32 i = 0 ; i < nbSsrub ; ++i){
                ssrubList.push_back(IDValuePair(convert_UTF8_to_PMString(response.SousSectionID->__ptr[i].c_str()),
                                                convert_UTF8_to_PMString(response.SousSectionName->__ptr[i].c_str())));
            }
        }
    }
    return success;
}


//----------------------------------------------------------------------------------------------------------------
// Creation d'une affectation, appel d'XRail qui va renvoyer l'id et le chemin de l'article
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::CreateStory(const PMString& idUser,
                                 const PMString& subject,
                                 const PMString& idSection,
                                 const PMString& subSection,
                                 const PMString& author,
                                 const int32 idPage,
                                 const PMString& posX,
                                 const PMString& posY,
                                 const PMString& hauteur,
                                 const PMString& largeur,
                                 const int32 nbPhotos,
                                 const PMString& nbSignesEstim,
                                 PMString& idStory,
                                 PMString& storyFullPath,
                                 PMString& result)
{
    
    string IDUser        = idUser.GetPlatformString().c_str();
    WideString wideSubject(subject);
    string Subject( wideSubject.begin(), wideSubject.end() );
    string IDSection    = idSection.GetPlatformString().c_str();
    string SubSection    = subSection.GetPlatformString().c_str();
    string Author        = author.GetPlatformString().c_str();
    string Classeur    = "";
    string Forme        = "";
    string PosX        = posX.GetPlatformString().c_str();
    string PosY        = posY.GetPlatformString().c_str();
    string Largeur     = largeur.GetPlatformString().c_str();
    string Hauteur     = hauteur.GetPlatformString().c_str();
    string Comment     = "";
    string nbSignes    = nbSignesEstim.GetPlatformString().c_str();
    
    struct ns1__WS_USCORENewStoryResponse response;
    bool success = (soap_call_ns1__WS_USCORENewStory(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDUser, Subject,
                                                     IDSection, SubSection, Author, idPage, Classeur, Forme,
                                                     PosX, PosY, 0, Hauteur, Largeur, nbPhotos, Comment,
                                                     nbSignes, response) == SOAP_OK);
    if(!success){
        HandleWebServiceError("CreateStory");
    }
    else{
        idStory = convert_UTF8_to_PMString(response.storyID.c_str());
        storyFullPath = convert_UTF8_to_PMString(response.storyPath.c_str());
        result = convert_UTF8_to_PMString(response.result.c_str());
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Casse le lien d'un article dans xrail et tec
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::DeleteStory(const PMString& idStory){
    
    string response;
    bool success = (soap_call_ns1__WS_USCOREFlex_USCOREStoryDelete(&soap, serverAddress.GetPlatformString().c_str(), NULL, idStory.GetPlatformString().c_str(), response) == SOAP_OK);
    if(!success)
        HandleWebServiceError("DeleteStory");
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Previent xrail et tec que le contenu d'un article vient de changer
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryContentChange(const PMString& idArticle, const int32 nbSignes){
    
    string idArt = idArticle.GetPlatformString().c_str();
    string response;
    bool success = (soap_call_ns1__WS_USCOREStoryContentChanged(&soap, serverAddress.GetPlatformString().c_str(), NULL, idArt, nbSignes, response) == SOAP_OK);
    if(!success)
        HandleWebServiceError("StoryContentChange");
    
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// XPage, Recupere les infos d'etat d'un article depuis xrail
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryGetInfo(const PMString& idStory,
                                  PMString& section,
                                  PMString& ssSection,
                                  PMString& libStatus,
                                  PMString& colorStatus,
                                  PMString& incaPath,
                                  PMString& sujet,
                                  PMString& proprio,
                                  PMString& reponse)
{
    string idArt = idStory.GetPlatformString().c_str();
    struct ns1__WS_USCOREFlex_USCOREStoryGetInfosResponse response;
    bool success = (soap_call_ns1__WS_USCOREFlex_USCOREStoryGetInfos(&soap, serverAddress.GetPlatformString().c_str(), NULL, idArt, response) == SOAP_OK);
    
    if(!success)
        HandleWebServiceError("StoryGetInfo");
    
    else{
        section = convert_UTF8_to_PMString(response.Section.c_str());
        ssSection = convert_UTF8_to_PMString(response.SubSection.c_str());
        libStatus = convert_UTF8_to_PMString(response.LibStatus.c_str());
        colorStatus = convert_UTF8_to_PMString(response.ColorStatus.c_str());
        incaPath = convert_UTF8_to_PMString(response.IncaPath.c_str());
        sujet = convert_UTF8_to_PMString(response.Sujet.c_str());
        proprio = convert_UTF8_to_PMString(response.IDProprietaire.c_str());
        reponse = convert_UTF8_to_PMString(response.result.c_str());
    }
    return success;
}


//----------------------------------------------------------------------------------------------------------------
// Renvoie les parents d'une page (paru, edition, titre, id et noms)
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetPageParents(const int32 pageID,
                                    PMString& titreID,
                                    PMString& titreLib,
                                    PMString& editionID,
                                    PMString& editionLib,
                                    PMString& parutionID,
                                    PMString& parutionLib)
{
    struct ns1__WS_USCOREGetPageParentsResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetPageParents(&soap,serverAddress.GetPlatformString().c_str(), NULL, pageID, response) == SOAP_OK);
    if(!success)
        HandleWebServiceError("GetPageParents");
    
    else{
        // Des entiers
        titreID.AppendNumber(response.titreID);
        editionID.AppendNumber(response.paruID);
        parutionID.AppendNumber(response.paruID);
        
        // Des chaines de caractËres
        parutionLib = convert_UTF8_to_PMString(response.nomParu.c_str());
        editionLib = convert_UTF8_to_PMString(response.nomEdition.c_str());
        titreLib = convert_UTF8_to_PMString(response.nomTitre.c_str());
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// XPage, renvoie les coordonnees d'un article.
//----------------------------------------------------------------------------------------------------------------
bool  XRailWebServices::ModifStory(const PMString& artID,
                                 PMString& X,
                                 PMString& Y,
                                 PMString& Largeur,
                                 PMString& Hauteur,
                                 int32& idPage,
                                 PMString& response)
{
    
    string result;
    
    string posX = X.GetPlatformString().c_str();
    string posY = Y.GetPlatformString().c_str();
    string l = Largeur.GetPlatformString().c_str();
    string h = Hauteur.GetPlatformString().c_str();
    
    bool success = (soap_call_ns1__WS_USCOREFlex_USCOREModifStory(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                                  artID.GetPlatformString().c_str(),
                                                                  atof(posX.c_str()),
                                                                  atof(posY.c_str()),
                                                                  atof(l.c_str()),
                                                                  atof(h.c_str()),
                                                                  idPage,
                                                                  result) == SOAP_OK);
    
    if(!success)
        HandleWebServiceError("ModifStory");
    
    return success;
}


//----------------------------------------------------------------------------------------------------------------
// XPage demande a xrail->tec la liste des articles a mettre en page (+idPage pour gestion edition)
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetMEPStoriesListWithIDPage(const int32 idParutionMEP,
                                       const int32 idPage,
                                       const PMString& idSection,
                                       const PMString& idSubSection,
                                       K2Vector<PMString>& IDStories,
                                       K2Vector<PMString>& ListSujets,
                                       K2Vector<PMString>& CheminArticles,
                                       K2Vector<PMString>& CouleurStatus,
                                       K2Vector<PMString>& LibStatus,
                                       K2Vector<PMString>& NomProprietaire,
                                       K2Vector<PMString>& IdSection,
                                       K2Vector<PMString>& IdSsSection)

{
    struct ns1__WS_USCOREGetMEPStoriesWithIDPageResponse response;
    bool success = (soap_call_ns1__WS_USCOREGetMEPStoriesWithIDPage(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                          idParutionMEP,
                                                          idSection.GetPlatformString().c_str(),
                                                          idSubSection.GetPlatformString().c_str(),
                                                          idPage,
                                                          response) == SOAP_OK);
    
    
    if(!success){
        HandleWebServiceError("GetMEPStoriesListWithIDPage");
    }
    else{
        // On verifie si les tableaux sont paralleles
        if( (response.storyID->__size == response.sujet->__size)
           && (response.storyID->__size == response.storyPath->__size)
           && (response.storyID->__size == response.couleurStatus->__size )
           && (response.storyID->__size == response.libStatus->__size)
           && (response.storyID->__size == response.nomproprietaire->__size)
           && (response.storyID->__size == response.sectionID->__size)
           && (response.storyID->__size == response.subSectionID->__size))
        {
            for(int32 i = 0 ; i < response.storyID->__size ; ++i){
                
                IDStories.push_back(convert_UTF8_to_PMString(response.storyID->__ptr[i].c_str()));
                ListSujets.push_back(convert_UTF8_to_PMString(response.sujet->__ptr[i].c_str()));
                CheminArticles.push_back(convert_UTF8_to_PMString(response.storyPath->__ptr[i].c_str()));
                 CouleurStatus.push_back(convert_UTF8_to_PMString(response.couleurStatus->__ptr[i].c_str()));
                LibStatus.push_back(convert_UTF8_to_PMString(response.libStatus->__ptr[i].c_str()));
                NomProprietaire.push_back(convert_UTF8_to_PMString(response.nomproprietaire->__ptr[i].c_str()));
                IdSection.push_back(convert_UTF8_to_PMString(response.sectionID->__ptr[i].c_str()));
                IdSsSection.push_back(convert_UTF8_to_PMString(response.subSectionID->__ptr[i].c_str()));
            }
        }
    }
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Lors de la creation d'une affectation, apres la creation, previent xrail->tec que le lien est fait entre l'id et l'affectation
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryLink(    const PMString& idUser, const PMString& storyID,
                               const int32 IDPage, const PMString& Classeur,
                               const PMString& NomForme, const PMString& PosX,
                               const PMString& PosY, const PMString& Hauteur,
                               const PMString& Largeur, const int32 typeArticle,
                               const PMString& sectionID, const PMString& SousSection,
                               const PMString& Sujet, const PMString& Auteur,
                               PMString& storyFullPath, PMString& result)
{
    
    string sIDUser = idUser.GetPlatformString().c_str();
    string sstoryID = storyID.GetPlatformString().c_str();
    string sClasseur = Classeur.GetPlatformString().c_str();
    string sNomForme = NomForme.GetPlatformString().c_str();
    string sPosX = PosX.GetPlatformString().c_str();
    string sPosY = PosY.GetPlatformString().c_str();
    string sHauteur = Hauteur.GetPlatformString().c_str();
    string sLargeur = Largeur.GetPlatformString().c_str();
    string ssectionID = sectionID.GetPlatformString().c_str();
    string sSousSection = SousSection.GetPlatformString().c_str();
    string sSujet = Sujet.GetPlatformString().c_str();
    string sAuteur = Auteur.GetPlatformString().c_str();
    
    
    struct ns1__WS_USCOREStoryLinkResponse response;
    bool success = (soap_call_ns1__WS_USCOREStoryLink(&soap, serverAddress.GetPlatformString().c_str(), NULL,
                                                      sIDUser, sstoryID, IDPage, sClasseur,
                                                      sNomForme, sPosX, sPosY, sHauteur,
                                                      sLargeur, typeArticle, ssectionID, sSousSection,
                                                      sSujet,    sAuteur, response) == SOAP_OK);
    
    if(!success){
        HandleWebServiceError("StoryLink");
    }
    else{
        storyFullPath = convert_UTF8_to_PMString(response.pathArticle.c_str());
        result = convert_UTF8_to_PMString(response.result.c_str());
    }
    return success;
}


//----------------------------------------------------------------------------------------------------------------
// ??? Lors du placement au marbre par xpage, changement de paru d'un article
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryParutionChange(const int32 idPage, const PMString& idArt, const PMString& idRub, const PMString& idSsRub, PMString& result){
    
    string response;
    bool success = (soap_call_ns1__WS_USCOREStoryParutionChange(&soap, serverAddress.GetPlatformString().c_str(), NULL, idPage, idArt.GetPlatformString().c_str(), idRub.GetPlatformString().c_str(), idSsRub.GetPlatformString().c_str(), response) == SOAP_OK);
    if(!success)
        HandleWebServiceError("StoryParutionChange");
    else
        result = convert_UTF8_to_PMString(response.c_str());
    
    soap_destroy(&soap);
    soap_end(&soap);
    return success;
    
}

//----------------------------------------------------------------------------------------------------------------
// Demande si l'article est verrouille
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::StoryGetLocked(const PMString& idArt, PMString& verrou){
	
	/*struct ns1__WS_USCOREStoryGetLockedResponse response;
	bool success = (soap_call_ns1__WS_USCOREStoryGetLocked(&soap, serverAddress.GetPlatformString().c_str(), NULL, idArt.GetPlatformString().c_str(), response) == SOAP_OK);
	if(!success)
		HandleWebServiceError("StoryGetLocked");
	else 
		verrou = convert_UTF8_to_PMString(response.Verrou.c_str());				
	*/
	return true;
}

//----------------------------------------------------------------------------------------------------------------
// Recupere l'id d'une reserve redactionnelle.
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetNewIDResaRedac(int32& id)
{
    string IDType = "IDResaRedac";
    bool success = (soap_call_ns1__WS_USCOREGetNewID(&soap, serverAddress.GetPlatformString().c_str(), NULL, IDType, (int&)id) == SOAP_OK);
    if(!success)
        HandleWebServiceError("GetNewIDResaRedac");
    
    soap_destroy(&soap);
    soap_end(&soap);
    return success;
}

//----------------------------------------------------------------------------------------------------------------
// Sauvegarde de tous les articles d'une page
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::SaveArticles(const PMString from, const bool16 modifContenu, K2Vector<int32>&ListePages, K2Vector<int32>&TabIDStories, K2Vector<int32>& TabTypes,
                          K2Vector<int32>& TabNbSignes, K2Vector<PMString>& TabClasseurs, K2Vector<PMString>& TabCartons,
                          K2Vector<int32>& IDStoriesByBloc, K2Vector<PMString>& TabX, K2Vector<PMString>& TabY,
                          K2Vector<PMString>& TabLargeur, K2Vector<PMString>& TabHauteur, K2Vector<int32>& TabIDPage,
                          K2Vector<PMString>& ListeImages, K2Vector<WideString>& ListeLegendes, K2Vector<WideString>& ListeCredits, PMString& result)
{
    
    //Liste des pages du document
    ArrayOfint arrayListePages;
    arrayListePages.__ptr = new int[ListePages.size()];
    arrayListePages.__size = ListePages.size();
    for(int32 i = 0 ; i < ListePages.size() ; ++i)
        arrayListePages.__ptr[i] = ListePages[i];
    
    //Id des articles sauvegardes
    ArrayOfint arrayIDStories;
    arrayIDStories.__ptr = new int[TabIDStories.size()];
    arrayIDStories.__size = TabIDStories.size();
    for(int32 i = 0 ; i < TabIDStories.size() ; ++i)
        arrayIDStories.__ptr[i] = TabIDStories[i];
    
    //type des articles
    ArrayOfint arrayTypes;
    arrayTypes.__ptr = new int[TabTypes.size()];
    arrayTypes.__size = TabTypes.size();
    for(int32 i = 0 ; i < TabTypes.size() ; ++i)
        arrayTypes.__ptr[i] = TabTypes[i];
    
    //nb de signes
    ArrayOfint arrayNbSignes;
    arrayNbSignes.__ptr = new int[TabNbSignes.size()];
    arrayNbSignes.__size = TabNbSignes.size();
    for(int32 i = 0 ; i < TabNbSignes.size() ; ++i)
        arrayNbSignes.__ptr[i] = TabNbSignes[i];
    
    //classeurs
    ArrayOfstring arrayClasseurs;
    arrayClasseurs.__ptr = new std::string[TabClasseurs.size()];
    arrayClasseurs.__size = TabClasseurs.size();
    for(int32 i = 0 ; i < TabClasseurs.size() ; ++i)
        arrayClasseurs.__ptr[i] = string(TabClasseurs[i].GetPlatformString().c_str());
    
    //formes
    ArrayOfstring arrayFormes;
    arrayFormes.__ptr = new std::string[TabCartons.size()];
    arrayFormes.__size = TabCartons.size();
    for(int32 i = 0 ; i < TabCartons.size() ; ++i)
        arrayFormes.__ptr[i] = string(TabCartons[i].GetPlatformString().c_str());
    
    //Id des articles pour chaque bloc
    ArrayOfint arrayIDStoriesByBloc;
    arrayIDStoriesByBloc.__ptr = new int[IDStoriesByBloc.size()];
    arrayIDStoriesByBloc.__size = IDStoriesByBloc.size();
    for(int32 i = 0 ; i < IDStoriesByBloc.size() ; ++i)
        arrayIDStoriesByBloc.__ptr[i] = IDStoriesByBloc[i];

    //positions X
    ArrayOfstring arrayX;
    arrayX.__ptr = new std::string[TabX.size()];
    arrayX.__size = TabX.size();
    for(int32 i = 0 ; i < TabX.size() ; ++i)
        arrayX.__ptr[i] = string(TabX[i].GetPlatformString().c_str());
    
    //positions Y
    ArrayOfstring arrayY;
    arrayY.__ptr = new std::string[TabY.size()];
    arrayY.__size = TabY.size();
    for(int32 i = 0 ; i < TabY.size() ; ++i)
        arrayY.__ptr[i] = string(TabY[i].GetPlatformString().c_str());
    
    //largeurs
    ArrayOfstring arrayLargeur;
    arrayLargeur.__ptr = new std::string[TabLargeur.size()];
    arrayLargeur.__size = TabLargeur.size();
    for(int32 i = 0 ; i < TabLargeur.size() ; ++i)
        arrayLargeur.__ptr[i] = string(TabLargeur[i].GetPlatformString().c_str());
    
    //hauteurs
    ArrayOfstring arrayHauteur;
    arrayHauteur.__ptr = new std::string[TabHauteur.size()];
    arrayHauteur.__size = TabHauteur.size();
    for(int32 i = 0 ; i < TabHauteur.size() ; ++i)
        arrayHauteur.__ptr[i] = string(TabHauteur[i].GetPlatformString().c_str());
    
    //Idpages
    ArrayOfint arrayIDPage;
    arrayIDPage.__ptr = new int[TabIDPage.size()];
    arrayIDPage.__size = TabIDPage.size();
    for(int32 i = 0 ; i < TabIDPage.size() ; ++i)
        arrayIDPage.__ptr[i] = TabIDPage[i];
    
    //images
    ArrayOfstring arrayImages;
    arrayImages.__ptr = new std::string[ListeImages.size()];
    arrayImages.__size = ListeImages.size();
    for(int32 i = 0 ; i < ListeImages.size() ; ++i)
        arrayImages.__ptr[i] = string(ListeImages[i].GetPlatformString().c_str());
    
    //legendes
    ArrayOfstring arrayLegendes;
    arrayLegendes.__ptr = new std::string[ListeLegendes.size()];
    arrayLegendes.__size = ListeLegendes.size();
    for(int32 i = 0 ; i < ListeLegendes.size() ; ++i) {
        std::string s( ListeLegendes[i].begin(), ListeLegendes[i].end() );
        arrayLegendes.__ptr[i] = s;
    }
    
    //credits
    ArrayOfstring arrayCredits;
    arrayCredits.__ptr = new std::string[ListeCredits.size()];
    arrayCredits.__size = ListeCredits.size();
    for(int32 i = 0 ; i < ListeCredits.size() ; ++i) {
        std::string s( ListeCredits[i].begin(), ListeCredits[i].end() );
        arrayCredits.__ptr[i] = s;
    }
    
    string response;
    string fromWhere(from.GetPlatformString().c_str());
    bool success = (soap_call_ns1__WS_USCORESaveArticles(&soap, serverAddress.GetPlatformString().c_str(), NULL, fromWhere, modifContenu, &arrayListePages, &arrayIDStories, &arrayTypes, &arrayNbSignes, &arrayClasseurs, &arrayFormes,
                                                         &arrayIDStoriesByBloc, &arrayX, &arrayY, &arrayLargeur, &arrayHauteur, &arrayIDPage, &arrayImages, &arrayLegendes, &arrayCredits, response) == SOAP_OK);

    if(!success)
    {
        HandleWebServiceError("SaveArticles");
    }
    
    delete[] arrayIDStories.__ptr;
    delete[] arrayTypes.__ptr;
    delete[] arrayNbSignes.__ptr;
    delete[] arrayClasseurs.__ptr;
    delete[] arrayFormes.__ptr;
    delete[] arrayIDStoriesByBloc.__ptr;
    
    delete[] arrayX.__ptr;
    delete[] arrayY.__ptr;
    delete[] arrayLargeur.__ptr;
    delete[] arrayHauteur.__ptr;
    delete[] arrayIDPage.__ptr;
    
    delete[] arrayImages.__ptr;
    delete[] arrayLegendes.__ptr;
    delete[] arrayCredits.__ptr;

    return success;
}

//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------
bool XRailWebServices::GetListeIDMS(const K2Vector<int32>& IDPages, K2Vector<int32>& TabIDStories, K2Vector<int32>& TabPagesIDS,
                                   K2Vector<PMReal>& TabX, K2Vector<PMReal>& TabY, K2Vector<PMString>& TabIDMSpaths, K2Vector<PMString>& TabXMLStorypaths, K2Vector<PMString>& TabSubjects)
{
    ArrayOfint idPageList;
    idPageList.__size = IDPages.size();
    idPageList.__ptr = new int[idPageList.__size];
    
    for(int32 i = 0 ; i < IDPages.size() ; ++i)
    {
        idPageList.__ptr[i] = IDPages[i];
    }
    
    struct ns1__WS_USCOREGetListeIDMSResponse response;
    
    bool success = (soap_call_ns1__WS_USCOREGetListeIDMS(&soap, serverAddress.GetPlatformString().c_str(), NULL, &idPageList, response) == 0);
    
    if(!success){
        HandleWebServiceError("GetListeIDMS");
    }
    else
    {
        for(int32 i = 0 ; i < response.IDArticle->__size ; ++i)
        {
            TabIDStories.push_back(response.IDArticle->__ptr[i]);
            TabPagesIDS.push_back(response.PageID->__ptr[i]);
            TabX.push_back(PMReal(strtod(response.X->__ptr[i].c_str(),NULL)));
            TabY.push_back(PMReal(strtod(response.Y->__ptr[i].c_str(),NULL)));
            TabIDMSpaths.push_back(convert_UTF8_to_PMString(response.Path->__ptr[i].c_str()));
            TabXMLStorypaths.push_back(convert_UTF8_to_PMString(response.xmlpath->__ptr[i].c_str()));
            TabSubjects.push_back(convert_UTF8_to_PMString(response.sujet->__ptr[i].c_str()));
        }
    }
    
    // clean up memory
    if(idPageList.__ptr)
        delete[] idPageList.__ptr;
    
    return success;
}
//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------

bool XRailWebServices::GetPrefsXRail(const int32& fromServeur, int32& prefs_ImportPub, int32& prefs_PubProp, int32& prefs_PDFMultipages, int32& prefs_ExportIDML)
{
    struct ns1__WS_USCOREGetPluginsPrefsResponse response;

    bool success = (soap_call_ns1__WS_USCOREGetPluginsPrefs(&soap, serverAddress.GetPlatformString().c_str(), NULL, fromServeur, response) == 0);

    if (!success) {
        //HandleWebServiceError("GetPrefsXRail");
    }
    else
    {
        prefs_ImportPub = response.ImportPub;
        prefs_PubProp = response.PubProp;
        prefs_PDFMultipages = response.PDFMultipages;
        prefs_ExportIDML = response.ExportIDML;
    }

    return success;
 }
//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------

 bool XRailWebServices::GetPrefsXPage(const int32 &fromServeur, int32& prefs_ExportXML, int32& prefs_GestionIDMS, int32& prefs_IDMSMAJIDMS, int32& prefs_IDMSALLBLOCS, int32& prefs_ImportLegendes, int32& prefs_ImportCredits)
 {
     struct ns1__WS_USCOREGetPluginsPrefsResponse response;

     bool success = (soap_call_ns1__WS_USCOREGetPluginsPrefs(&soap, serverAddress.GetPlatformString().c_str(), NULL, fromServeur, response) == 0);

     if (!success) {
        // HandleWebServiceError("GetPrefsXPage");
     }
     else
     {
         prefs_ExportXML = response.ExportXML;
         prefs_GestionIDMS = response.GestionIDMS;
         prefs_IDMSMAJIDMS = response.IDMSMAJIDMS;
         prefs_IDMSALLBLOCS = response.IDMSALLBLOCS;
         prefs_ImportLegendes = response.ImportLegendes;
         prefs_ImportCredits = response.ImportCredits;
     }

     return success;
 }

//----------------------------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------------------------
PMString XRailWebServices::convert_UTF8_to_PMString (const char *text)
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



