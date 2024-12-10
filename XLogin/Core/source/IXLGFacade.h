//========================================================================================
//  
//  $File: $
//  
//  Owner: Badr Nasser
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2007 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#pragma once
#ifndef __IXLGFacade_h__
#define __IXLGFacade_h__

#include "IPMUnknown.h"
#include "XLGID.h"

/** IXLGFacade defines the interface for this plug-ins facade.
	A Facade is a functional API layer that sits on top of commands and other complicated model operatoins.  Script providers and 
	selection suites often have to do the same thing.  Putting command and other model specific code into a facade provides
	code reuse.

    
    @ingroup xlogin
*/
class IXLGFacade : public IPMUnknown
{
public:
	enum {kDefaultIID = IID_IXLGFACADE };

	/** Can we  perform operation X on the passed in items?
	@param items IN The target items  
	@return kTrue if operation X is supported by these items.*/
	virtual bool16 CanDoX(const UIDList& items) const = 0;
	
	/** Performs operation X on the passed in items returning an ErrorCode.
	@param items IN The target items  
	@return kSuccess on success, or an appropriate ErrorCode on failure. */
	virtual ErrorCode DoX(const UIDList& items) = 0;

	/** Can we perform operation Y on the passed in items?
	@param items IN The target items  
	@return kTrue if operation Y is supported by these items.*/
	virtual bool16 CanDoY(const UIDList& items) const = 0;

	/** Performs operation Y on the passed in items returning an ErrorCode.
	@param items IN The target items  
	@return kSuccess on success, or an appropriate ErrorCode on failure. */
	virtual ErrorCode DoY(const UIDList& items) = 0;

	/** Gets some data from the specified items.  Instead of returning a value,
	implementations will stuff data into the OUT variable.
	@param items IN The target items  
	@param vector OUT Data Z for the current selection is inserted into vector.*/
	virtual void GetZ( const UIDList& items, K2Vector<int32> & vector) = 0;

	/** Sets some data on the specified items.
	@param items IN The target items  
	@param z IN Data to set.
	@return kSuccess on success, or an appropriate ErrorCode on failure.  */
	virtual ErrorCode SetZ( const UIDList& items, int32 z ) = 0;

};
#endif // __IXLGFacade_h__

//  Code generated by DollyXs code generator