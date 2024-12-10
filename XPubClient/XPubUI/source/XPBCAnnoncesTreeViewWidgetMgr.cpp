//========================================================================================
//  
//  $File: //depot/indesign_5.0/gm/source/sdksamples/wlistboxcomposite/WLBCmpTreeViewWidgetMgr.cpp $
//  
//  Owner: Danielle Darling
//  
//  $Author: sstudley $
//  
//  $DateTime: 2007/02/15 13:37:33 $
//  
//  $Revision: #1 $
//  
//  $Change: 505969 $
//  
//  ADOBE CONFIDENTIAL
//  
//  Copyright Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  All information contained herein is, and remains
//  the property of Adobe Systems Incorporated and its suppliers,
//  if any.  The intellectual and technical concepts contained
//  herein are proprietary to Adobe Systems Incorporated and its
//  suppliers and may be covered by U.S. and Foreign Patents,
//  patents in process, and are protected by trade secret or copyright law.
//  Dissemination of this information or reproduction of this material
//  is strictly forbidden unless prior written permission is obtained
//  from Adobe Systems Incorporated.
//  
//========================================================================================
#include "VCPlugInHeaders.h"

#include "IControlView.h"
#include "IInLineEditController.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ITreeViewHierarchyAdapter.h"

#include "CTreeViewWidgetMgr.h"
#include "XPBCAnnonceNodeID.h"
#include "XPBCID.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"

/**
 *  Tree view manager
	@ingroup wlistboxcomposite
 */
class XPBCAnnoncesTVWidgetMgr : public CTreeViewWidgetMgr
{
public:
	XPBCAnnoncesTVWidgetMgr(IPMUnknown *boss);
	virtual ~XPBCAnnoncesTVWidgetMgr()
	{}
    
    /** Create instance of the widget that represents given node in the tree.
     @param node [IN] specifies node of interest
     @return return reference to a newly instantiated object that is suitable for this node
     */
	virtual	IControlView* CreateWidgetForNode(
                                              const NodeID& node) const;
    
	/**
     Retrieve ID of a widget that has the right appearance for the node
     that you're trying to display. Here we use the same widget types for all the nodes,
     but vary the appearance by showing or hiding the expander widget depending on the number
     of children associated with a node.
     @param node [IN] specifies node of interest
     @return the ID of a widget that has the correct appearance for the given node type
     */
	virtual	WidgetID GetWidgetTypeForNode(const NodeID& node) const;
    
	/** Determine how to render the given node to the specified control.
     Figure out how you're going to render it based on properties of the node
     such as whether is has children or not (don't show expander if no children, for instance).
     @param node [IN] refers to the node that we're trying to render
     @param widget [IN] the control into which this node's appearance is going to be rendered
     @param message [IN] specifies ???
     @return kTrue if the node has been rendered successfully, kFalse otherwise
     */
	virtual	bool16 ApplyNodeIDToWidget(
                                       const NodeID& node,
                                       IControlView* widget, 
                                       int32 message = 0) const;
    
    void RegisterStyles()
    {
        RegisterStyleWidget(kLargePaletteRowsTreeStyle, kPubListElementRsrcID, kXPBCPluginID);
    }
};


CREATE_PMINTERFACE(XPBCAnnoncesTVWidgetMgr, kXPBCAnnoncesTVWidgetMgrImpl)

XPBCAnnoncesTVWidgetMgr::XPBCAnnoncesTVWidgetMgr(IPMUnknown *boss) :
CTreeViewWidgetMgr(boss, kList)
{
}

/* CreateWidgetForNode
 */
IControlView* XPBCAnnoncesTVWidgetMgr::CreateWidgetForNode(const NodeID& node) const
{
	IControlView* retval= (IControlView*) ::CreateObject(::GetDataBase(this),
                                                         RsrcSpec(LocaleSetting::GetLocale(),
                                                                  kXPBCPluginID,
                                                                  kViewRsrcType,
                                                                  kXPBCAnnoncesNodeWidgetRsrcID),
                                                         IID_ICONTROLVIEW);
    
	ASSERT(retval);
	return retval;
}



/* GetWidgetTypeForNode
 */
WidgetID XPBCAnnoncesTVWidgetMgr::GetWidgetTypeForNode(const NodeID& node) const
{
	return kXPBCAnnoncesNodeWidgetId;
}


/* ApplyNodeIDToWidget
 */
bool16 XPBCAnnoncesTVWidgetMgr::ApplyNodeIDToWidget(const NodeID& node, IControlView* widget, int32 message) const
{
	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);
	do
	{
		// If this node doesn't have any children, don't show expander Widget
		InterfacePtr<IPanelControlData> panelControlData(widget, UseDefaultIID());
        
		// Set expander widget to hidden state if this node has no kids
		IControlView* expanderWidget = panelControlData->FindWidget(kTreeNodeExpanderWidgetID);
		InterfacePtr<const ITreeViewHierarchyAdapter> adapter(this, UseDefaultIID());
        
		if (expanderWidget)
		{
			if (adapter->GetNumChildren(node) <= 0)
			{
				expanderWidget->HideView();
			}
			else
			{
				expanderWidget->ShowView();
			}
		}
        
		TreeNodePtr<XPBCAnnonceNodeID> nodeID(node);
		
		PMString annonceID = nodeID->GetID();
        PMString status = nodeID->GetStatus();
        PMString name = nodeID->GetName();
        
		annonceID.SetTranslatable( kFalse );
		status.SetTranslatable( kFalse );
		name.SetTranslatable( kFalse );
        
		IControlView* annonceIdView = panelControlData->FindWidget(kXPBCAnnoncesColIDWidgetID);
		InterfacePtr<ITextControlData> annonceIdControlData(annonceIdView, UseDefaultIID());
		annonceIdControlData->SetString(annonceID);
		
		IControlView* statusView = panelControlData->FindWidget(kXPBCAnnoncesColStatusWidgetID);
		InterfacePtr<ITextControlData> statusControlData(statusView, UseDefaultIID());
		statusControlData->SetString(status);
		
        IControlView* nameView = panelControlData->FindWidget(kXPBCAnnoncesColNameWidgetID);
		InterfacePtr<ITextControlData> nameControlData(nameView, UseDefaultIID());
		nameControlData->SetString(name);
		
		
	} while(kFalse);
    
	return kTrue;
}