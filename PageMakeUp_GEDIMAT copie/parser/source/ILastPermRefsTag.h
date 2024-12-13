/*
//	File:	ILastPermRefsTag.h
//
//  Author: Wilfried LEFEVRE
//
//	Date:	12-Dec-2005
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2005 Trias Developpement. All rights reserved.
//
*/

#ifndef __ILastPermRefsTag_h__
#define __ILastPermRefsTag_h__

#include "PrsID.h"
#include "ITCLParser.h"


/** ILastPermRefsTag
	Provides method to store registration information
*/
class ILastPermRefsTag : public IPMUnknown {
	public:	

		enum	{ kDefaultIID = IID_ILASTPERMREFSTAG };

		// Getter and setter last Reference
		virtual const PermRefStruct& getLastReference() = 0;	
		virtual void setLastReference(const PermRefStruct& reference) = 0;
};

#endif // __ILastPermRefsTag_h__

		