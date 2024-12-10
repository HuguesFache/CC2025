
#ifndef __DocUtils_h__
#define __DocUtils_h__

#include "RsrcSpec.h"
#include "IDialog.h"

		int32 GetPageID(UIDRef docUIDRef, int32 index, PMString &baseName);
		void SetPageID(UIDRef docUIDRef, int32 indexofpage, int32 id, PMString baseName);
			 
		/** Sauvegarde d'un document*/
		ErrorCode DoSaveDoc(UIDRef documentUIDRef, IDFile sysFile);
		ErrorCode SaveAsDoc(UIDRef doc, PMString absoluteName);
		
		/** Generation de la vignette*/
		ErrorCode DoMakePreview(const UIDRef& documentUIDRef, IDFile& jpgFile, int32 page, int32 scale, int32 resolution);
ErrorCode DoMakeIDML(const UIDRef& documentUIDRef, IDFile& idmlFile);
	
		//Lire et ecrire des preferences
		void SetXRailPrefs(PMString adresseIP, PMString port, PMString lastUser, bool16 modeRobot, K2Vector<PMString> listeBases, K2Vector<PMString> IPBases);
		void GetPrefsXRail(PMString &adresseIP, PMString &port, PMString &lastUser, bool16& modeRobot, K2Vector<PMString> &listeBases, K2Vector<PMString> &IPBases);
		
		PMString GetGoodIP(PMString baseName);
		
		IDialog * CreateNewDialog(PluginID pluginID, RsrcID dialogID, IDialog::DialogType dialogType = IDialog::kMovableModal);
		
		//recuperer nombre de pages du document
		int32 GetNbPages(const UIDRef& documentUIDRef);
		
		//appliquer une section
		ErrorCode SetFolio(const UIDRef& documentUIDRef, int32 indexofpage, int32 folio);

		// creer une reserve de pub
		ErrorCode CreerResaPub(UIDRef parent, const PMString& resaID, const PMString& resaAnnonceur, const PMPoint& leftTop, const PMReal& width, const PMReal& height, UID& resaUID, bool16 VerrouPos);

		// Convert an existing text or graphic frame to resa pub
		ErrorCode ConvertToResaPub(UIDRef resaFrame, const PMString& resaID, const PMString& resaAnnonceur);

		// If the pub file was already imported, check whether its link is out of date before importing		
		void ImportPubFile(UIDRef resaPub, const IDFile& pubFile, PMString pubFileName);
		
		void GetLinksWithFilesOnDoc(UIDRef docUIDRef , UIDList & links, K2Vector<IDFile> & filesList);
		
		// Verrouillage de position du bloc pub
		void LockPageItemCmd(UIDRef itemToSet, const bool8 toLock, const bool8 allowUndo);
		
		ErrorCode DoPackageDocument(IDocument * doc, IDFile theDoc);


#endif // __DocUtils_h__



