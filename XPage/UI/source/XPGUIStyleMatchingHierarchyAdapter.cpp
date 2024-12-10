
#include "VCPlugInHeaders.h"

// Interface includes:
#include "ListTreeViewAdapter.h"
#include "IWorkspace.h"
#include "ISession.h"
#include "IXPageUtils.h"
#include "IXPageMgrAccessor.h"
#include "IMatchingStylesList.h"
#include "IPMUnknownData.h"

// General includes:
#include "CPMUnknown.h"
#include "CAlert.h"
#include "XPGMatchingStyleDataNode.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGUIStyleMatchingNodeID.h"
#include "XPGID.h"


class XPGUIStyleMatchingHierarchyAdapter : public ListTreeViewAdapter {

	public:		
		XPGUIStyleMatchingHierarchyAdapter(IPMUnknown* boss);	
		virtual ~XPGUIStyleMatchingHierarchyAdapter();
		virtual int32 GetNumListItems() const;
		NodeID_rv GetRootNode() const;	
		virtual NodeID_rv GetNthListItem(const int32& nth) const;
	
};	
CREATE_PMINTERFACE(XPGUIStyleMatchingHierarchyAdapter, kXPGUIStyleMatchingHierarchyAdapterImpl)

XPGUIStyleMatchingHierarchyAdapter::XPGUIStyleMatchingHierarchyAdapter(IPMUnknown* boss)
								   :ListTreeViewAdapter(boss)
{
	K2Vector<MatchingStyle> list;
	InterfacePtr<IMatchingStylesList> listData(this, IID_IMATCHINGSTYLESLIST);
	listData->SetMatchingStylesList(list);
}

XPGUIStyleMatchingHierarchyAdapter::~XPGUIStyleMatchingHierarchyAdapter(){}

int32 XPGUIStyleMatchingHierarchyAdapter::GetNumListItems()const{	
	InterfacePtr<IMatchingStylesList> listData(this, IID_IMATCHINGSTYLESLIST);
	return listData->GetMatchingStylesList().size();
}

NodeID_rv XPGUIStyleMatchingHierarchyAdapter::GetRootNode() const{
	PMString rootString("Root");
	rootString.SetTranslatable(kFalse);
	return XPGUIStyleMatchingNodeID::Create(rootString, "", XPGMatchingStyleDataNode::kMatchingStyleNode, NULL);
}

NodeID_rv XPGUIStyleMatchingHierarchyAdapter::GetNthListItem(const int32& nth) const{
	InterfacePtr<IMatchingStylesList> listData(this, IID_IMATCHINGSTYLESLIST);
	MatchingStyle style = listData->GetMatchingStylesList()[nth];
	return XPGUIStyleMatchingNodeID::Create(style.styleName, style.styleTag, XPGMatchingStyleDataNode::kMatchingStyleNode, NULL);	
}

//	end, File: XPGUIStyleMatchingHierarchyAdapter.cpp
