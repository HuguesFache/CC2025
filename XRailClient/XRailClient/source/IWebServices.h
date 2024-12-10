
#ifndef __IWebServices__
#define __IWebServices__

#include "IPMUnknown.h"
#include "XRCID.h"
#include "K2Vector.tpp"

#include "K2Vector.h"
#include "KeyValuePair.h"

typedef KeyValuePair<PMString, PMString> IDValuePair;
typedef K2Vector<IDValuePair > IDValueList;

/**	Interface for XRailWebServices.
 */
class IWebServices : public IPMUnknown
{
public:
    enum { kDefaultIID = IID_IWEBSERVICES };
    
    virtual void SetServerAddress(const PMString& address) =0;
    
    virtual bool VerifLogin(const PMString& login, const PMString& pwd, PMString& userID, PMString& errorCode) =0;
    
    virtual bool GetStatusPreflight(const PMString& userID, K2Vector<PMString>& listStatus, K2Vector<bool>& checkError) =0;
    
    virtual bool GetLevel1List(const int32& userID, K2Vector<PMString>& listLevel1Names, K2Vector<int32>& listLevel1ID) =0;
    
    virtual bool GetLevel2List(const int32& level1ID, K2Vector<PMString>& listLevel2Names, K2Vector<int32>& listLevel2ID) =0;
    
    virtual bool GetLevel3List(const int32& level2ID, K2Vector<PMString>& listLevel3Names, K2Vector<int32>& listLevel3ID) =0;
    
    virtual bool GetLevel4List(const int32& level3ID, int32 nbPages, K2Vector<PMString>& listLevel4Names, K2Vector<int32>& listLevel4ID) =0;
    
    virtual bool GetPagesIDS(const int32& pageID, K2Vector<int32>& listPagesIDS) =0;
    
    virtual bool SetPageStatus(const int32& IDPage, const PMString& status, const PMString& userID) =0;
    
    virtual bool SetVerrouPage(const K2Vector<int32>& IDPage, int lock, const PMString& userID, PMString& status, bool errorInDoc) =0;
    
    virtual bool GetPictCommonFolders(const PMString& IDPage, PMString& ClientPictPath, PMString& Scale) = 0;

    virtual bool GetPreviewInfos(const int32& IDPage, const PMString& login, PMString& ClientPictPath, int32& Scale, int32&resolution) = 0;

    virtual bool MoveGeneratedFiles (const K2Vector<PMString>& FilePath, const K2Vector<int32>& IDPage) =0;
    
    virtual bool MovePDFFile (const int32& IDPage, const PMString& fileName, const bool externe, const int32& IDUser) = 0;
    
    virtual bool GetPDFFileName(const int32& IDPage, PMString& profilPDF, PMString& pdfFileName) = 0;
    
    virtual bool GetDocFileNameWithID(const int32& IDPage, PMString& docFileName) = 0;
    
    virtual bool GetListeOrdresRigueur(const K2Vector<int32>& IDPages,  K2Vector<int32>& TabPagesIDS,
                                       K2Vector<int32>& TabFolios, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDS,
                                       K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs, K2Vector<PMReal>& TabX,
                                       K2Vector<PMReal>& TabY, K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabCheminImage, K2Vector<bool16>& TabVerrouPos) =0;
    
    virtual bool MAJPub(const PMString& IDPub, const int32& IDPage,  const PMString& x, const PMString& y, const PMString& l,const PMString& h, const PMString& path ) =0;
    
    virtual bool GetListeOrdresMarbre (const int32& IDPage, K2Vector<PMString>& TabResaIDs, K2Vector<PMString>& TabPubsIDs,
                                       K2Vector<PMString>& TabAnnonceurs, K2Vector<PMString>& TabProds, K2Vector<PMString>& TabSurfaces,
                                       K2Vector<PMString>& TabEmpls, K2Vector<PMReal>& TabHauteurs, K2Vector<PMReal>& TabLargeurs,
                                       K2Vector<PMString>& TabPaths) =0;
    
    virtual bool GetDroitsMEP(const int32 idUser, int& VoirPDF, int& VoirVignettes)=0;
    
    virtual bool GetDocNameForUnlink(const int32& IDPage, PMString& docFileName)=0;
    
    virtual bool StoryLockChanged(const PMString &idArt , const PMString & flagLock)=0;
    
    virtual bool GetStoryStates(	const PMString & idArt ,
                                const PMString & user,
                                IDValueList& StatusValuesList,
                                K2Vector<PMString>& ColorsList,
                                K2Vector<PMString>& StatusOrderList,
                                K2Vector<PMString>& StatusRoleList,
                                K2Vector<PMString>& StatusRegleList,
                                K2Vector<PMString>& CommentaireList,
                                PMString&           result) = 0;
    
    
    virtual bool GetGroupsStatusList(	const PMString & IDStory ,
                                     const PMString & author,
                                     const PMString & status,
                                     IDValueList& StatusValuesList,
                                     PMString& result) = 0;
    
    virtual bool GetUsersStatusList(	const PMString & author ,
                                    const PMString & idArt,
                                    const PMString & status,
                                    const PMString & group,
                                    IDValueList& usersValuesList,
                                    PMString& result) = 0;
    
    virtual bool StoryStatusChanged(	const PMString & storyID ,
                                    const PMString & statusID,
                                    const PMString & userID,
                                    const PMString & groupID,
                                    PMString& result)= 0;
    
    virtual bool GetParutionList(const PMString& idTitle,
                                 IDValueList& parutionList) =0;
    
   virtual bool GetRubriqueList(const PMString& idTitle,
                                 IDValueList& rubList) =0;
    
    virtual bool GetSsRubriqueList(const PMString& idRub,
                                   IDValueList& ssrubList) =0;
    
    virtual bool GetAuthorizedStatus(IDValueList& userNames,
                                     K2Vector<PMString>& usersFullNames,
                                     const PMString& IdStatus,
                                     const int32 IdTitle,
                                     const PMString& IdSection)=0;
    
      
    virtual bool CreateStory(  const PMString& idUser,
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
                             PMString& result)=0;
    
    virtual bool DeleteStory(const PMString& idStory)=0;
    
    virtual bool StoryContentChange(const PMString& idArticle,
                                    const int32 nbSignes) =0;
    
    virtual bool StoryGetInfo( const PMString& idStory,
                              PMString&       section,
                              PMString&       ssSection,
                              PMString&       libStatus,
                              PMString&       colorStatus,
                              PMString&       incaPath,
                              PMString&       sujet,
                              PMString&       proprio,
                              PMString&       reponse) = 0;
    
      virtual bool GetPageParents(const int32 pageID,
                                PMString& titreID,
                                PMString& titreLib,
                                PMString& editionID,
                                PMString& editionLib,
                                PMString& parutionID,
                                PMString& parutionLib)=0;
    
    
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
                                             K2Vector<PMString>& IdSsSection)=0;
    
    virtual bool ModifStory(const PMString & artID ,
                            PMString& X,
                            PMString& Y,
                            PMString& Largeur,
                            PMString& Hauteur,
                            int32& idPage,
                            PMString& result)= 0;
    
    
    virtual bool StoryLink(const PMString& idUser, const PMString& storyID, const int32 IDPage,
                           const PMString& Classeur, const PMString& NomForme,
                           const PMString& PosX, const PMString& PosY,
                           const PMString& Hauteur, const PMString& Largeur,
                           const int32 typeArticle, const PMString& sectionID,
                           const PMString& SousSection, const PMString&     Sujet,
                           const PMString& Auteur,	PMString& storyFullPath,
                           PMString& result)=0;
    
    virtual bool StoryParutionChange(const int32 idPage, 
                                     const PMString& idArt, 
                                     const PMString& idRub, 
                                     const PMString& idSsRub, 
                                     PMString& result)=0;
        
    virtual bool StoryGetLocked(const PMString& idArt, PMString& verrou)=0;
    
    virtual bool GetNewIDResaRedac(int32& id) =0;
      
    virtual bool SaveArticles(const PMString from, const bool16 modifContenu, K2Vector<int32>&ListePages, K2Vector<int32>&TabIDStories, K2Vector<int32>& TabTypes,
                              K2Vector<int32>& TabNbSignes, K2Vector<PMString>& TabClasseurs, K2Vector<PMString>& TabCartons, 
								  K2Vector<int32>& IDStoriesByBloc, K2Vector<PMString>& TabX, K2Vector<PMString>& TabY,
                              K2Vector<PMString>& TabLargeur, K2Vector<PMString>& TabHauteur, K2Vector<int32>& TabIDPage,
                              K2Vector<PMString>& ListeImages, K2Vector<WideString>& ListeLegendes, K2Vector<WideString>& ListeCredits, PMString& result)=0;
    
    virtual bool GetListeIDMS(const K2Vector<int32>& IDPages, K2Vector<int32>& TabIDStories, K2Vector<int32>& TabPagesIDS,
                             K2Vector<PMReal>& TabX, K2Vector<PMReal>& TabY, K2Vector<PMString>& TabIDMSpaths, K2Vector<PMString>& TabXMLStorypaths,K2Vector<PMString>& TabSubjects) =0;
    
    virtual bool GetPrefsXPage(const int32& fromServeur, int32& prefs_ExportXML, int32& prefs_GestionIDMS, int32& prefs_IDMSMAJIDMS, int32& prefs_IDMSALLBLOCS, int32& prefs_ImportLegendes, int32& prefs_ImportCredits) = 0;

    virtual bool GetPrefsXRail(const int32& fromServeur, int32& prefs_ImportPub, int32& prefs_PubProp, int32& prefs_PDFMultipages, int32& prefs_ExportIDML) = 0;

    virtual PMString convert_UTF8_to_PMString (const char *text)=0;

};

#endif // __IWebServices__



