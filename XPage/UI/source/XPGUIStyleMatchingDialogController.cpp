#include "VCPlugInHeaders.h"

#include "IStyleInfo.h"
#include "CDialogController.h"
#include "ITreeViewMgr.h"
#include "IMatchingStylesList.h"
#include "IUIDListData.h"
#include "IPanelControlData.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "ISAXServices.h"
#include "ISAXContentHandler.h"
#include "ISAXEntityResolver.h"
#include "ISAXParserOptions.h"
#include "StreamUtil.h"
#include "ErrorUtils.h"
#include "IStringListData.h"
#include "FileUtils.h"
#include "IXPGPreferences.h"
#include "ITreeViewHierarchyAdapter.h"
#include "CAlert.h"
#include "XPGUIStyleMatchingNodeID.h"
#include "IControlView.h"
#include "IStringListControlData.h"
#include "IDropDownListController.h"
#include "Utils.h"
#include "IWidgetUtils.h"
#include "LocaleSetting.h"
#include "K2Pair.h"
#include "IDialogMgr.h"
#include "IApplication.h"
#include "IDialog.h"
#include "RsrcSpec.h"
#include "CoreResTypes.h"
#include "IXPageUtils.h"
#include "IKeyValueData.h"

// Project includes
#include "XPGUIID.h"
#include "XPGID.h"



class XPGUIStyleMatchingDialogController : public CDialogController {

	public:

		XPGUIStyleMatchingDialogController(IPMUnknown* boss):CDialogController(boss) {}
		virtual void InitializeDialogFields( IActiveContext* dlgContext);
		virtual WidgetID ValidateDialogFields( IActiveContext* myContext);
		virtual void ApplyDialogFields( IActiveContext* myContext, const WidgetID& widgetId);
};

CREATE_PMINTERFACE(XPGUIStyleMatchingDialogController, kXPGUIStyleMatchingDialogControllerImpl)

void XPGUIStyleMatchingDialogController::InitializeDialogFields( IActiveContext* dlgContext){

	InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
	do{

		InterfacePtr<IUIDListData> formeStylesListData (this, IID_IFORMESTYLESLIST);	
		if(formeStylesListData->GetRef() == nil)
			break;

		UIDList stylesUID (*(formeStylesListData->GetRef()));
		K2Vector<MatchingStyle> matchingStyles;

		PMString defaultStyleName("NormalParagraphStyle",  PMString::kUnknownEncoding);
		PMString noStyleName("No Paragraph Style",  PMString::kUnknownEncoding);

		for(int32 i = 0; i < stylesUID.Length(); i++ ){			
			
			MatchingStyle matchingStyle;
			InterfacePtr<IStyleInfo> styleInfo (stylesUID.GetRef(i), UseDefaultIID());

			matchingStyle.styleName = styleInfo->GetName();
			matchingStyle.styleTag = kNullString;

			if( ::K2find(matchingStyles, matchingStyle) == matchingStyles.end() && 
							    matchingStyle.styleName != defaultStyleName && 
								matchingStyle.styleName != noStyleName)

				matchingStyles.push_back(matchingStyle);
		}
		
		InterfacePtr<ITreeViewMgr> treeViewMgr(panelCtrlData->FindWidget(kXPGUIStyleMatchingViewWidgetID), UseDefaultIID());
		InterfacePtr<IMatchingStylesList> matchingStylesList (treeViewMgr, UseDefaultIID());   
		matchingStylesList->SetMatchingStylesList(matchingStyles);		
		
		treeViewMgr->ClearTree();
		treeViewMgr->ChangeRoot(kTrue);   
		
	} while(kFalse);
}

WidgetID XPGUIStyleMatchingDialogController::ValidateDialogFields( IActiveContext* myContext){

	do {
		InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
		InterfacePtr<ITreeViewMgr> treeViewMgr(panelCtrlData->FindWidget(kXPGUIStyleMatchingViewWidgetID), UseDefaultIID());
		if(treeViewMgr == nil)
			break;
		
		InterfacePtr<ITreeViewHierarchyAdapter> treeViewHierAdapter (treeViewMgr, UseDefaultIID());
		if(treeViewHierAdapter == nil)
			break;

		NodeID styleMatchingRootNode = treeViewHierAdapter->GetRootNode();
		int32 treeViewHierAdapterCount = treeViewHierAdapter->GetNumChildren(styleMatchingRootNode);

		K2Vector<PMString> stylesTags;
		PMString styleTag = kNullString;
		
		for(int32 i = 0; i<treeViewHierAdapterCount; ++i) {

			styleTag.clear();
			PMString tagName = kNullString, styleName = kNullString;

			/* BUG FIX : don't use TreeNodePtr this way, it causes leaks :

			   TreeNodePtr<XPGUIStyleMatchingNodeID> nodePtr (treeViewHierAdapter->GetNthChild(styleMatchingRootNode, i));
			*/
			NodeID childNode = treeViewHierAdapter->GetNthChild(styleMatchingRootNode, i);			 
			TreeNodePtr<XPGUIStyleMatchingNodeID> nodePtr (childNode);
			styleName = nodePtr->GetID();
			
			InterfacePtr<IControlView> nodeView (treeViewMgr->QueryWidgetFromNode(childNode));
			if(nodeView == nil)
				continue;

			InterfacePtr<IDropDownListController> tagListWidgetController ((IDropDownListController*)Utils<IWidgetUtils>()->QueryRelatedWidget(nodeView, kXPGUIStyleTagListWidgetID, IID_IDROPDOWNLISTCONTROLLER ));
			if(tagListWidgetController == nil)
				continue;

			int32 selectedIndex = tagListWidgetController->GetSelected();

			if(selectedIndex != IDropDownListController::kNoSelection){
				InterfacePtr<IStringListControlData> tagListWidgetData (tagListWidgetController, UseDefaultIID());
				tagName = tagListWidgetData->GetString(selectedIndex);
			}
		
			if(tagName == kNullString){
				CAlert::InformationAlert(kXPGUIInvalidMatchingStyleNameKey);
				return kXPGUIStyleTagListWidgetID;
			}	
		}
	} while(kFalse);
	return kNoInvalidWidgets;
}

/* ApplyFields
*/
void XPGUIStyleMatchingDialogController::ApplyDialogFields( IActiveContext* myContext, const WidgetID& widgetId) 
{
	do {

		InterfacePtr<IPanelControlData> panelCtrlData (this, UseDefaultIID());
		InterfacePtr<ITreeViewMgr> treeViewMgr(panelCtrlData->FindWidget(kXPGUIStyleMatchingViewWidgetID), UseDefaultIID());
		if(treeViewMgr == nil)
			break;
		
		InterfacePtr<ITreeViewHierarchyAdapter> treeViewHierAdapter (treeViewMgr, UseDefaultIID());
		if(treeViewHierAdapter == nil)
			break;

		NodeID styleMatchingRootNode = treeViewHierAdapter->GetRootNode();
		int32 treeViewHierAdapterCount = treeViewHierAdapter->GetNumChildren(styleMatchingRootNode);

		K2Vector<IDValuePair> stylesTags;
		PMString tagName = kNullString, styleName = kNullString;

		for(int32 i = 0; i<treeViewHierAdapterCount; ++i) {

			tagName.clear(); 
			styleName.clear();

			/* BUG FIX : don't use TreeNodePtr this way, it causes leaks :

			   TreeNodePtr<XPGUIStyleMatchingNodeID> nodePtr (treeViewHierAdapter->GetNthChild(styleMatchingRootNode, i));
			*/
			NodeID childNode = treeViewHierAdapter->GetNthChild(styleMatchingRootNode, i);			 
			TreeNodePtr<XPGUIStyleMatchingNodeID> nodePtr (childNode);
			styleName = nodePtr->GetID();
			
			InterfacePtr<IControlView> nodeView (treeViewMgr->QueryWidgetFromNode(childNode));
			if(nodeView == nil)
				continue;

			InterfacePtr<IDropDownListController> tagListWidgetController ((IDropDownListController*)Utils<IWidgetUtils>()->QueryRelatedWidget(nodeView, kXPGUIStyleTagListWidgetID, IID_IDROPDOWNLISTCONTROLLER ));
			if(tagListWidgetController == nil)
				continue;

			int32 selectedIndex = tagListWidgetController->GetSelected();			
			if(selectedIndex != IDropDownListController::kNoSelection){
				InterfacePtr<IStringListControlData> tagListWidgetData (tagListWidgetController, UseDefaultIID());
				tagName = tagListWidgetData->GetString(selectedIndex);
			}
			if(tagName != kNullString && styleName != kNullString )
				 stylesTags.push_back(KeyValuePair<PMString, PMString>(tagName, styleName)); 				
					
		}

		InterfacePtr<IKeyValueData> formeMatchingStyleData (this, IID_IFORMEMATCHINGSTYLESLIST);
		formeMatchingStyleData->SetKeyValueList(stylesTags);	

		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application(GetExecutionContextSession()->QueryApplication());			
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec(
			nLocale, // Locale index from PMLocaleIDs.h. 
			kXPGUIPluginID, // Our Plug-in ID  
			kViewRsrcType, // This is the kViewRsrcType.
			kXPGUINewFormeDialogResourceID,	// Resource ID for our dialog.
			kTrue // Initially visible.
		);
	
		IDialog* dialog = dialogMgr->CreateNewDialog(dialogSpec, IDialog::kMovableModal);
		
		InterfacePtr<IKeyValueData> toFormeMatchingStyleData (dialog->GetDialogPanel(), IID_IFORMEMATCHINGSTYLESLIST);
		toFormeMatchingStyleData->SetKeyValueList(formeMatchingStyleData->GetKeyValueList());

		

	}while(kFalse);
}
