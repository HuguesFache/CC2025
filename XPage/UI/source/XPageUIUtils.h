
#ifndef __XPageUIUtils_h__
#define __XPageUIUtils_h__

#include "IXPageUtils.h"

class IPanelControlData;

//========================================================================================
// CLASS XPageUtils
//========================================================================================

class XPageUIUtils
{
public:

	/** Display Choose Matching File Dialog and return selected matching file
	*/
	static ErrorCode DisplayChooseMatchingDialog(IDFile& matchingFile);

	/** Display Display New Forme Dialog
	*/
	static void DisplayNewFormeDialog(const PMString& classeurName, bool16 assemblageDialog = kFalse);

	/** Fill an XPage dropdownlist (kXPGUIDropDownListWidgetBoss) with the elements in parameters
	*/
	static void FillDropdownlist(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, IDValueList elements, bool16 notify);

	/** Fill a classic dropdownlist  with the elements in parameters, selecting the first element in the list
	*/
	static void FillDropdownlist(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, K2Vector<PMString> elements, bool16 notify);

	/** Fill a classic dropdownlist and manage selection of a particular element
	*/
	static void FillDropdownlistAndSelect(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, K2Vector<PMString> elements, const PMString& elementToSelect, bool16 notify);

	static void DisplayLinkArticleDialog(const PMString& articleId, const PMString& articleSnippetFile,
										 const PMString& articleSubject, IDFile& matchingFile, PMPoint currentPoint, 
										 const PMString& idStatus, const PMString& libelleStatus,
										 const PMString& couleurStatus, 
										 UIDRef targetSpread, const int32 typeArt, const PMString& articleXMLFile,
										 const PMString& artRub, const PMString& artSubRub);

	static bool16 IsValidName(const PMString& fileName);

private:
	XPageUIUtils(); // this is an util class, only to be used with static methods calls
};

#endif // __XPageUIUtils_h__
