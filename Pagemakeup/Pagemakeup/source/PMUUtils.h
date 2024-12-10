/*
//	File:	PMUUtils.h
//
//	Date:	18-Apr-2003
//
//	Author : Trias
*/

#ifndef __PMUUtils_h__
#define __PMUUtils_h__

class IDialog;

// This class provides different static functions useful in the projects
class PMUUtils 
{

public :

	/**
		Open an OpenFile dialog
		@param OUT file : file chosen by the user
	*/	
	static bool OpenFileDialog(IDFile& file);

	static void ExecTCL();

	static void ReverseTCL();

	static void ShowTCLPrefs();

	static IDialog * CreateNewDialog(RsrcID rsID);

	static void AddPermRef();

	static void SearchPermRef();

	static void AddTCLRef();

	static void SearchTCLRef();

	static void SearchOversetBox();

	static void UpdatePicture();
	
	static void ExportPermRefs();

	static void ExportPermRefsWithContent();

	static void DeleteAllPermsRefsTCL();

	static void AutoTagDocumentPermsRefsTCL();

	static void AutoTagDocumentWithReferencePermsRefsTCL();
	
	static void ILC_AutoTagDocumentWithReferencePermsRefsTCL();
	
	static void ILC_ExportPermRefs();
};

#endif //__PMUUtils_h__