//========================================================================================
//  
//  $File: //depot/indesign_5.0/gm/source/sdksamples/wlistboxcomposite/XRLUIPubListTreeViewAdapter.cpp $
//  
//  Owner: Danielle Darling
//  
//  $Author: sdai $
//  
//  $DateTime: 2007/03/20 09:16:07 $
//  
//  $Revision: #2 $
//  
//  $Change: 520588 $
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
#include "VCPluginHeaders.h"

#include "IAnnoncesList.h"
#include "ListTreeViewAdapter.h"
#include "XPBCAnnonceNodeID.h"
#include "XPBCID.h"
#include "CAlert.h"
/**
 *  TreeViewAdapter.
	@ingroup wlistboxcomposite
 */
class XPBCAnnoncesTreeViewAdapter : public ListTreeViewAdapter
{
	public:

		XPBCAnnoncesTreeViewAdapter(IPMUnknown* boss);
		virtual ~XPBCAnnoncesTreeViewAdapter(){}
		virtual int32 GetNumListItems() const;
		NodeID_rv GetRootNode() const;
		virtual NodeID_rv GetNthListItem(const int32& nth) const;
		virtual bool16  ShouldAddNthChild( const NodeID& node, const int32& nth ) const { return kTrue; }

};

CREATE_PMINTERFACE(XPBCAnnoncesTreeViewAdapter, kXPBCAnnoncesTreeViewAdapterImpl)

XPBCAnnoncesTreeViewAdapter::XPBCAnnoncesTreeViewAdapter(IPMUnknown* boss):ListTreeViewAdapter(boss)
{
	//Initialize the list with an empty list
	K2Vector<annoncedata> list;
	InterfacePtr<IAnnoncesList> listData(this, IID_IANNONCESLIST);
	listData->SetAnnoncesList(list);

}
int32 XPBCAnnoncesTreeViewAdapter::GetNumListItems()const
{
    InterfacePtr<IAnnoncesList> listData(this, IID_IANNONCESLIST);
	return listData->GetAnnoncesList().size();
}

NodeID_rv XPBCAnnoncesTreeViewAdapter::GetRootNode() const
{
	PMString rootString("Root");
	rootString.Translate();
	return XPBCAnnonceNodeID::Create(kNullString, kNullString, kNullString);
}

NodeID_rv XPBCAnnoncesTreeViewAdapter::GetNthListItem(const int32& nth) const
{
	InterfacePtr<IAnnoncesList> listData(this, IID_IANNONCESLIST);
	annoncedata annonce = listData->GetAnnoncesList()[nth];
	return XPBCAnnonceNodeID::Create(annonce.ID, annonce.Status, annonce.Name);

}
