//========================================================================================
//  
//  $File: //depot/indesign_5.0/gm/source/sdksamples/wlistboxcomposite/ResaPubMarbreNodeID.h $
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
//  
//  Each C++ implementation class in the plug-in and its corresponding ImplementationID
//  should be registered in this file.
//  
//========================================================================================

#ifndef __ResaPubMarbreNodeID__
#define __ResaPubMarbreNodeID__

#include "NodeID.h"
#include "IPMStream.h"
#include "XPBCID.h"

/** Represents a node in the tree view. It is the list item.
	
*/
class XPBCAnnonceNodeID : public NodeIDClass
{
	public:
		enum { kNodeType = kXPBCAnnoncesBoxWidgetBoss };


		/**	Factory method to create 
		new object of this class
			@return   new instance of this class 
		 */
		static	NodeID_rv Create() { return new XPBCAnnonceNodeID(); }

		/**	Factory method to create new object of this class
			@param path [IN] specifies initial path
			@return  new instance of this class
		 */
		static	NodeID_rv Create(PMString annonceID, PMString annonceStatus, PMString annonceName)
		{ 
			return new XPBCAnnonceNodeID(annonceID, annonceStatus, annonceName);
		}

		/**	Destructor
		 */
		virtual ~XPBCAnnonceNodeID() {}
		

		/**	@return type of this node
		 */
		virtual	NodeType GetNodeType() const { return kNodeType; } 

		/**	Comparator function
			@param NodeID [IN] specifies the node that we should compare against
			@return  Comparison results
		 */
		virtual int32 Compare(const NodeIDClass* NodeID) const;

		/**	Create a copy of this
			@return  
		 */
		virtual NodeIDClass* Clone() const;

		/**	Read this from the given stream
			@param stream [IN] specified stream
			@return  
		 */
		virtual void Read(IPMStream* stream);

		/**	Write this to the given stream
			@param [OUT] stream 
			@return  
		 */
		virtual void Write(IPMStream* stream) const;

		/**	Accessors for the values associated with this node		
		 */
		
		virtual PMString GetDescription()const;

		const PMString& GetID() const { return fAnnonceID; }
        const PMString& GetName() const { return fAnnonceName; }
        const PMString& GetStatus() const { return fAnnonceStatus; }

	private:
		// Note we're keeping the destructor private to force use of the factory methods
		XPBCAnnonceNodeID() {}

		XPBCAnnonceNodeID(PMString annonceID, PMString annonceStatus, PMString annonceName):fAnnonceID(annonceID), fAnnonceName(annonceName), fAnnonceStatus(annonceStatus)
		{
            fAnnonceID.SetTranslatable(kFalse);
			fAnnonceName.SetTranslatable(kFalse);
			fAnnonceStatus.SetTranslatable(kFalse);
		}

		PMString fAnnonceID, fAnnonceName, fAnnonceStatus;
};

#endif // __ResaPubMarbreNodeID__


