
#include "VCPluginHeaders.h"

// Project includes
#include "XPageUIUtils.h"

// Interface includes
#include "IDialogMgr.h"
#include "IDialog.h"
#include "IApplication.h"
#include "ISysFileData.h"
#include "IStringData.h"
#include "IStringListControlData.h"
#include "IStringListData.h"
#include "IPanelControlData.h"
#include "ITextFrameColumn.h"
#include "IDropDownListController.h"
#include "XMLDefs.h"
// General includes
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "CoreResTypes.h"
#include "IClassIDData.h"
#include "UIDList.h"
#include "Utils.h"
#include "IWaxIterator.h"
#include "IAssignmentUtils.h"
#include "ITextStrand.h"
#include "ITextModel.h"
#include "IFrameList.h"
#include "IBoolData.h"
#include "IWaxLine.h"
#include "AppUIID.h"
#include "SpellPanelID.h"
#include "TextEditorModelID.h"
#include "IWaxStrand.h"
#include "IXPGPreferences.h"
#include "IWebServices.h"
#include "FileUtils.h"
#include "CAlert.h"
#include "XPGUIID.h"
#include "IPMPointData.h"
#include "IUIDData.h"
#include "IIntData.h"
#include "ITextControlData.h"

ErrorCode XPageUIUtils::DisplayChooseMatchingDialog(IDFile& matchingFile)
{
	ErrorCode status = kFailure;
	do
	{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
		if (application == nil)
			break;

		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		if (dialogMgr == nil)
			break;

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
			(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			kXPGUIPluginID,				// Plug-in ID
			kViewRsrcType,				// This is the kViewRsrcType.
			kXPGUIChooseMatchingDialogResourceID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
			);

		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).

		IDialog * dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		dialog->Open();
		dialog->WaitForDialog();

		InterfacePtr<ISysFileData> fileData (dialog->GetDialogPanel(), UseDefaultIID());
		IDFile selectedFile = fileData->GetSysFile();

		if(selectedFile == IDFile())
			break;

		matchingFile = selectedFile;

		status = kSuccess;

	} while(kFalse);

	return status;
}

void XPageUIUtils::DisplayNewFormeDialog(const PMString& classeurName, bool16 assemblageDialog)
{
	do
	{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
		if (application == nil)
			break;

		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		if (dialogMgr == nil)
			break;

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcID dialogResourceID = assemblageDialog ? kXPGUINewAssemblageDialogResourceID : kXPGUINewFormeDialogResourceID;
		RsrcSpec dialogSpec
			(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			kXPGUIPluginID,				// Plug-in ID
			kViewRsrcType,				// This is the kViewRsrcType.
			dialogResourceID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
			);

		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).		
		IDialog * dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kModeless);

		if(!assemblageDialog)
		{
			InterfacePtr<IStringData> classeurData (dialog->GetDialogPanel(), IID_ICLASSEURNAMEDATA);
			classeurData->Set(classeurName);
		}

		dialog->Open();
		dialog->WaitForDialog();

	} while(kFalse);
}

void XPageUIUtils::DisplayLinkArticleDialog(const PMString& articleId, const PMString& articleSnippetFile,
											const PMString& articleSubject, IDFile& matchingFile, PMPoint currentPoint, 
											const PMString& idStatus, const PMString& libelleStatus,
											const PMString& couleurStatus, 
											UIDRef targetSpread, const int32 typeArt, 
											const PMString& articleXMLFile, const PMString& artRub, const PMString& artSubRub)
{
	do
	{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());
		if (application == nil)
			break;

		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		if (dialogMgr == nil)
			break;

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,						// Locale index from PMLocaleIDs.h. 
			kXPGUIPluginID,					// Plug-in ID
			kViewRsrcType,					// This is the kViewRsrcType.
			kXPGUILinkArtDialogResourceID,	// Resource ID for our dialog.
			kTrue							// Initially visible.
		);

		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).		
		IDialog * dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);

		InterfacePtr<IIntData> articleTypeData (dialog->GetDialogPanel(), IID_IARTICLETYPEDATA);
		articleTypeData->Set(typeArt);

		InterfacePtr<IStringData> articleData (dialog->GetDialogPanel(), IID_IARTICLEIDDATA);
		articleData->Set(articleId);

		InterfacePtr<IStringData> articleSubjectData (dialog->GetDialogPanel(), IID_IARTICLETOPICDATA);
		articleSubjectData->Set(articleSubject);

		InterfacePtr<IStringData> articleSnippetFileData (dialog->GetDialogPanel(), IID_IARTICLESNIPPETFILEDATA);
		articleSnippetFileData->Set(articleSnippetFile);

		InterfacePtr<IStringData> articleXMLFileData (dialog->GetDialogPanel(), IID_IARTICLEXMLFILEDATA);
		articleXMLFileData->Set(articleXMLFile);		
		
		InterfacePtr<IStringData> articleIdStatusData (dialog->GetDialogPanel(), IID_IARTICLEIDSTATUS);
        articleIdStatusData->Set(idStatus);
	  
	    InterfacePtr<IStringData> articleLibelleStatusData (dialog->GetDialogPanel(), IID_IARTICLELIBELLESTATUS);
		articleLibelleStatusData->Set(libelleStatus);

		InterfacePtr<IStringData> articleCouleurStatusData (dialog->GetDialogPanel(), IID_IARTICLECOULEURSTATUS);
		articleCouleurStatusData->Set(couleurStatus);

		InterfacePtr<IStringData> articleRubData (dialog->GetDialogPanel(), IID_IARTICLERUBDATA);
		articleRubData->Set(artRub);

		InterfacePtr<IStringData> articleSubRubData (dialog->GetDialogPanel(), IID_IARTICLESUBRUBDATA);
		articleSubRubData->Set(artSubRub);

		InterfacePtr<ISysFileData> matchingData (dialog->GetDialogPanel(), UseDefaultIID());
		matchingData->Set(matchingFile);

		InterfacePtr<IPMPointData>  pmPointData (dialog->GetDialogPanel(), UseDefaultIID());
		pmPointData->Set(currentPoint);

		InterfacePtr<IUIDData> uidData(dialog->GetDialogPanel(), UseDefaultIID());
		uidData->Set(targetSpread);
		
		dialog->Open();
		dialog->WaitForDialog();

	} while(kFalse);
}


void XPageUIUtils::FillDropdownlist(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, IDValueList elements, bool16 notify){

	do{
		IControlView * dropdownview = panelCtrlData->FindWidget(dropdownlist);
		if(!dropdownview)
			break;

		InterfacePtr<IStringListControlData> dropDownListData (dropdownview, UseDefaultIID());
		if(!dropDownListData)
			break;

		dropDownListData->Clear();
		InterfacePtr<IStringListData> idListData (dropDownListData, IID_IIDLIST);
		if(!idListData)
			break; 
		
		K2Vector<PMString> idList;
		for(int32 i = 0 ; i < elements.size() ; ++i){
			PMString value = elements[i].Value();
			value.SetTranslatable(kFalse);
			dropDownListData->AddString(value);
			idList.push_back(elements[i].Key());
		}	

		idListData->SetStringList(idList);

		// Select first item in the list, if any
		if(!elements.empty()){
			InterfacePtr<IDropDownListController> dropDownListController (dropDownListData, UseDefaultIID());
			dropDownListController->Select(IDropDownListController::kBeginning, kTrue, notify);
		}

	} while(kFalse);
}

void XPageUIUtils::FillDropdownlist(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, K2Vector<PMString> elements, bool16 notify)
{
	do
	{
		IControlView * dropdownview = panelCtrlData->FindWidget(dropdownlist);
		if(!dropdownview)
			break;

		InterfacePtr<IStringListControlData> dropDownListData (dropdownview, UseDefaultIID());
		dropDownListData->Clear();

		for(int32 i = 0 ; i < elements.size() ; ++i)
		{
			PMString value = elements[i];
			value.SetTranslatable(kFalse);
			dropDownListData->AddString(value);
		}			

		// Select first item in the list, if any
		if(!elements.empty())
		{
			InterfacePtr<IDropDownListController> dropDownListController (dropDownListData, UseDefaultIID());
			dropDownListController->Select(IDropDownListController::kBeginning, kTrue, notify);
		}
	} while(kFalse);
}

void XPageUIUtils::FillDropdownlistAndSelect(const IPanelControlData * panelCtrlData, const WidgetID& dropdownlist, K2Vector<PMString> elements, const PMString& elementToSelect, bool16 notify)
{
	do
	{
		IControlView * dropdownview = panelCtrlData->FindWidget(dropdownlist);
		if(!dropdownview)
			break;

		// Get current selected index
		int32 selectedIndex = IDropDownListController::kNoSelection;
		InterfacePtr<IDropDownListController> dropDownListController (dropdownview, UseDefaultIID());
		if(elementToSelect == kNullString) // No element to select passed in parameter, keep current selection
		{		
			selectedIndex = dropDownListController->GetSelected();
			if(selectedIndex == IDropDownListController::kNoSelection || selectedIndex >= elements.size())
				selectedIndex = IDropDownListController::kBeginning;
		}

		// Populate list widget
		InterfacePtr<IStringListControlData> dropDownListData (dropdownview, UseDefaultIID());
		dropDownListData->Clear();

		for(int32 i = 0 ; i < elements.size() ; ++i)
		{
			PMString value = elements[i];
			value.SetTranslatable(kFalse);
			dropDownListData->AddString(value);
		}	

		// Select the element, if any
		if(elementToSelect != kNullString)
			selectedIndex = dropDownListData->GetIndex(elementToSelect);

		if(selectedIndex != IDropDownListController::kNoSelection && !elements.empty())
		{
			dropDownListController->Select(selectedIndex, kTrue, notify);

			InterfacePtr<ITextControlData> listTxtControlData (dropDownListController, UseDefaultIID());
			listTxtControlData->SetString(elements[selectedIndex], kTrue, kFalse);
		}

	}while(kFalse);
}

bool16 XPageUIUtils::IsValidName(const PMString& fileName)
{
	
	char invalidChars[9] = { '/', '\\', ':', '*', '?', '"', '<', '>', '|' };
	bool16 isValid = kTrue;
	for(int32 i = 0 ; i < 9 ; ++i)
	{
		if(fileName.IndexOfCharacter(invalidChars[i]) >= 0)
		{
			isValid = kFalse;
			break;
		}
	}	

	return isValid;
}