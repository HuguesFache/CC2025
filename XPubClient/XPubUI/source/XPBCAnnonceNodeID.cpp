//========================================================================================
//  
//  $File: //depot/indesign_5.0/gm/source/sdksamples/wlistboxcomposite/WLBCmpNodeID.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: sstudley $
//  
//  $DateTime: 2007/02/15 13:37:33 $
//  
//  $Revision: #1 $
//  
//  $Change: 505969 $
//  
//  Copyright Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPluginHeaders.h"

// Interface includes:
#include "IPMStream.h"

// Project includes:
#include "XPBCAnnonceNodeID.h"
#include "CAlert.h"

/* Compare
*/
int32 XPBCAnnonceNodeID::Compare(const NodeIDClass* nodeID) const
{
	const XPBCAnnonceNodeID* oneNode = static_cast<const XPBCAnnonceNodeID*>(nodeID);
	ASSERT(nodeID);
	return fAnnonceID.Compare(kFalse, oneNode->GetID());
	
}


/* Clone
*/
NodeIDClass* XPBCAnnonceNodeID::Clone() const
{
	return new XPBCAnnonceNodeID(this->GetID(), this->GetStatus(), this->GetName());
}

/* Read
*/
void XPBCAnnonceNodeID::Read(IPMStream* stream)
{
	fAnnonceID.ReadWrite(stream);
	fAnnonceStatus.ReadWrite(stream);
	fAnnonceName.ReadWrite(stream);
}


/* Write
*/
void XPBCAnnonceNodeID::Write(IPMStream* stream) const
{
	(const_cast<XPBCAnnonceNodeID*>(this)->fAnnonceID).ReadWrite(stream);
	(const_cast<XPBCAnnonceNodeID*>(this)->fAnnonceStatus).ReadWrite(stream);
	(const_cast<XPBCAnnonceNodeID*>(this)->fAnnonceName).ReadWrite(stream);
	
}


PMString XPBCAnnonceNodeID::GetDescription()const
{
	return kNullString;
}
//	end, File:	PnlTrvDataNode.cpp
