/*
//	File:	XPBCDocServiceProvider.cpp
//
//	Date:	2004
//
//	
//	Copyright 2004 Trias Developpement. All Rights Reserved.
//	
//	NOTICE: Adobe permits you to use, modify, and distribute this file in
//	accordance with the terms of the Adobe license agreement accompanying it.
//	If you have received this file from a source other than Adobe, then your
//	use, modification, or distribution of it requires the prior written
//	permission of Adobe.
//
*/

#include "VCPlugInHeaders.h"

// Interface includes:

// Implementation includes:
#include "CServiceProvider.h"
#include "K2Vector.h"
#include "DocumentID.h"
#include "XPBCID.h"


/** XPBCDocServiceProvider
	registers as providing the service of responding to a group of document
	file action signals.  See the constructor code for a list of the
	signals this service responds.

	DocWchServiceProvider implements IK2ServiceProvider based on
	the partial implementation CServiceProvider.


	@author Guillaume DESSAY
*/
class XPBCDocServiceProvider : public CServiceProvider
{
	public:

		/**
			Constructor initializes a list of service IDs, one for each file action signal that DocWchResponder will handle.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		XPBCDocServiceProvider(IPMUnknown *boss);
		
		/**
			Destructor.  
		*/
		virtual	~XPBCDocServiceProvider();

		/**
			GetName initializes the name of the service.
			@param pName Ptr to PMString to receive the name.
		*/
		virtual void GetName(PMString * pName);

		/**
			GetServiceID returns a single service ID.  This is required, even though
			GetServiceIDs() will return the complete list initialized in the constructor.
			This method just returns the first service ID in the list.
		*/
		virtual ServiceID GetServiceID();

		/**
			IsDefaultServiceProvider tells the application this service is not the default service.
		*/
		virtual bool16 IsDefaultServiceProvider();
		
		/**
			GetInstantiationPolicy returns a InstancePerX value to indicate that only
			one instance per session is needed.
		*/
		virtual InstancePerX GetInstantiationPolicy();

		/**
			HasMultipleIDs returns kTrue in order to force a call to GetServiceIDs().
		*/
		virtual bool16 HasMultipleIDs() const;

		/**
			GetServiceIDs returns a list of services provided.
			@param serviceIDs List of IDs describing the services that DocWchServiceProvider registers to handle.
		*/
		virtual void GetServiceIDs(K2Vector<ServiceID>& serviceIDs);

	private:

		K2Vector<ServiceID> fSupportedServiceIDs;
};


/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(XPBCDocServiceProvider, kXPBCDocServiceProviderImpl)


/* DocWchActionComponent Constructor
*/
XPBCDocServiceProvider::XPBCDocServiceProvider(IPMUnknown *boss)
	: CServiceProvider(boss)
{
	// Add the service IDs we want the associated responder to handle.
	//  (See DocumentID.h)
	fSupportedServiceIDs.clear();
	
	//	NewDoc
	//fSupportedServiceIDs.Append(kBeforeNewDocSignalResponderService);
	//fSupportedServiceIDs.Append(kDuringNewDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterNewDocSignalResponderService);

	//	OpenDoc
	//fSupportedServiceIDs.Append(kBeforeOpenDocSignalResponderService);
	//fSupportedServiceIDs.Append(kDuringOpenDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterOpenDocSignalResponderService);

	//	SaveDoc
	//fSupportedServiceIDs.Append(kBeforeSaveDocSignalResponderService);
	fSupportedServiceIDs.push_back(kAfterSaveDocSignalResponderService);

	//	SaveAsDoc
	//fSupportedServiceIDs.Append(kBeforeSaveAsDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterSaveAsDocSignalResponderService);

	//	SaveACopyDoc
	//fSupportedServiceIDs.Append(kBeforeSaveACopyDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterSaveACopyDocSignalResponderService);

	//	RevertDoc
	//fSupportedServiceIDs.Append(kBeforeRevertDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterRevertDocSignalResponderService);

	//	CloseDoc
	fSupportedServiceIDs.push_back(kBeforeCloseDocSignalResponderService);
	//fSupportedServiceIDs.Append(kAfterCloseDocSignalResponderService);

	if (fSupportedServiceIDs.size()<=0)
	{
		ASSERT_FAIL("XPBCDocServiceProvider must support at least 1 service ID");
		fSupportedServiceIDs.push_back(kInvalidService);
	}

}

/* DocWchActionComponent Dtor
*/
XPBCDocServiceProvider::~XPBCDocServiceProvider()
{
}

/* GetName
*/
void XPBCDocServiceProvider::GetName(PMString * pName)
{
	pName->SetCString("DocWatch Responder Service");
}

/* GetServiceID
*/
ServiceID XPBCDocServiceProvider::GetServiceID() 
{
	// Should never be called given that HasMultipleIDs() returns kTrue.
	return fSupportedServiceIDs[0];
}

/* IsDefaultServiceProvider
*/
bool16 XPBCDocServiceProvider::IsDefaultServiceProvider()
{
	return kFalse;
}

/* GetInstantiationPolicy
*/
IK2ServiceProvider::InstancePerX XPBCDocServiceProvider::GetInstantiationPolicy()
{
	return IK2ServiceProvider::kInstancePerSession;
}

/* HasMultipleIDs
*/
bool16 XPBCDocServiceProvider::HasMultipleIDs() const
{
	return kTrue;
}

/* GetServiceIDs
*/
void XPBCDocServiceProvider::GetServiceIDs(K2Vector<ServiceID>& serviceIDs)
{
	// Append a service IDs for each service provided. 
	for (int32 i = 0; i<fSupportedServiceIDs.size(); i++)
		serviceIDs.push_back(fSupportedServiceIDs[i]);

}


// End, XPBCDocServiceProvider.cpp.



