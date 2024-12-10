#include "VCPlugInHeaders.h"

// Interface includes:
#include "IControlView.h"
#include "IImportProvider.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "IUIFontSpec.h"
#include "IDropDownListController.h"

// General includes:
#include "CTreeViewWidgetMgr.h"
#include "IStringListControlData.h"
#include "IStringListData.h"
#include "CreateObject.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "SysControlIds.h"
#include "OpenPlaceID.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGUIStyleMatchingNodeID.h"
#include "XPageUIUtils.h"
#include "CAlert.h"
#include "IXPGPreferences.h"


class XPGUIStyleMatchingWidgetMgr: public CTreeViewWidgetMgr {

	public:
		
		XPGUIStyleMatchingWidgetMgr(IPMUnknown* boss);
		virtual ~XPGUIStyleMatchingWidgetMgr() {}

		virtual	IControlView* CreateWidgetForNode(const NodeID& node) const;
		virtual	WidgetID GetWidgetTypeForNode(const NodeID& node) const;
		
		virtual	bool16 ApplyNodeIDToWidget(	const NodeID& node, IControlView* widget, int32 message = 0) const;
		virtual PMReal GetIndentForNode(const NodeID& node) const;

	private:
		
		void indent(const NodeID& node, IControlView* widget, IControlView* staticTextWidget) const;	
		const bool16 isFirstInit; 
};	

CREATE_PMINTERFACE(XPGUIStyleMatchingWidgetMgr, kXPGUIStyleMatchingWidgetMgrImpl)

XPGUIStyleMatchingWidgetMgr::XPGUIStyleMatchingWidgetMgr(IPMUnknown* boss) 
							:CTreeViewWidgetMgr(boss), 
							isFirstInit(kTrue){}


/* CreateWidgetForNode
*/
IControlView* XPGUIStyleMatchingWidgetMgr::CreateWidgetForNode(const NodeID& node) const{

	IControlView* retval= (IControlView*) ::CreateObject(::GetDataBase(this),
														RsrcSpec(LocaleSetting::GetLocale(), 
														kXPGUIPluginID, 
														kViewRsrcType, 
														kXPGUIStyleMatchingWidgetRsrcID),
														IID_ICONTROLVIEW);
	return retval;
}


/* GetIndentForNode
*/
PMReal XPGUIStyleMatchingWidgetMgr::GetIndentForNode(const NodeID& node) const
{
	return 18.0;
}

/* GetWidgetTypeForNode
*/
WidgetID XPGUIStyleMatchingWidgetMgr::GetWidgetTypeForNode(const NodeID& node) const
{
	return kXPGUIStyleMatchingWidgetID;
}


/* ApplyNodeIDToWidget
*/
bool16 XPGUIStyleMatchingWidgetMgr::ApplyNodeIDToWidget(const NodeID& node, IControlView* widget, int32 message) const{

	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);
	do{
		// If this node doesn't have any children, don't show expander Widget
		InterfacePtr<IPanelControlData> panelControlData(widget, UseDefaultIID());
	
		// Set expander widget to hidden state if this node has no kids
		IControlView* expanderWidget = panelControlData->FindWidget(kTreeNodeExpanderWidgetID);
		InterfacePtr<const ITreeViewHierarchyAdapter> adapter(this, UseDefaultIID());			
	
		if (expanderWidget) {
			if (adapter->GetNumChildren(node) <= 0)
				expanderWidget->HideView();			
			else 
				expanderWidget->ShowView();			
		}

		// Display Style name
		TreeNodePtr<XPGUIStyleMatchingNodeID> nodeID(node);

		if(nodeID->GetType() == XPGMatchingStyleDataNode::kMatchingStyleNode){

			PMString styleName = nodeID->GetID();
			styleName.SetTranslatable( kFalse );	

			IControlView* displayStringView = panelControlData->FindWidget( kXPGUIStyleNameWidgetID );
			InterfacePtr<ITextControlData> textControlData(displayStringView, UseDefaultIID());
			textControlData->SetString(styleName);
			
			IControlView * dropDownListview = panelControlData->FindWidget(kXPGUIStyleTagListWidgetID);
			InterfacePtr<IStringListControlData> dropDownListData (dropDownListview, UseDefaultIID());
			InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());
			K2Vector<Tag> tagList = xpgPrefs->GetListeMatchingTag();

			for(int32 i = 0; i < tagList.size() ; i++){
				PMString tagName = tagList[i].value;
				tagName.SetTranslatable(kFalse);
				dropDownListData->AddString(tagName);
			}		
		}
		
	} while(kFalse);
	return kTrue;
}

//	end, File: XPGUIStyleMatchingWidgetMgr.cpp
