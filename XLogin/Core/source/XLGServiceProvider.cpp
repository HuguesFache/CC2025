#include "VCPlugInHeaders.h"

// Interface includes:
#include "InCopyBridgeID.h"
#include "InCopyBridgeUIID.h"
#include "CServiceProvider.h"
#include "K2Vector.h"
#include "DocumentID.h"
#include "TextID.h"
#include "XLGID.h"


// General includes
#include "LocaleSetting.h"

class XLGServiceProvider : public CServiceProvider
{
	public:

		XLGServiceProvider(IPMUnknown* boss);
		virtual	~XLGServiceProvider();
		virtual void GetName(PMString* pName);
		virtual ServiceID GetServiceID();
		virtual bool16 IsDefaultServiceProvider();
		virtual InstancePerX GetInstantiationPolicy();
		virtual bool16 HasMultipleIDs() const;
		virtual void GetServiceIDs(K2Vector<ServiceID>& serviceIDs);

	private:

		K2Vector<ServiceID> fSupportedServiceIDs;
};


/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XLGServiceProvider, kXLGServiceProviderImpl)


/* XRailServiceProvider Constructor
*/
XLGServiceProvider::XLGServiceProvider(IPMUnknown* boss) : CServiceProvider(boss){	

	fSupportedServiceIDs.clear();

	fSupportedServiceIDs.push_back(kBeforeCloseDocSignalResponderService);	
    fSupportedServiceIDs.push_back(kBeforeOpenDocSignalResponderService);		
    fSupportedServiceIDs.push_back(kDuringOpenDocSignalResponderService);		
    fSupportedServiceIDs.push_back(kAfterOpenDocSignalResponderService);	
    fSupportedServiceIDs.push_back(kTextScrapSuitePasteHelperService);
    fSupportedServiceIDs.push_back(kBeforeOpenDocSignalResponderService);		
    fSupportedServiceIDs.push_back(kBeforeSaveDocSignalResponderService);		
    fSupportedServiceIDs.push_back(kAfterSaveDocSignalResponderService);	
    fSupportedServiceIDs.push_back(kAfterCloseDocSignalResponderService);	 
		 
		 

	
	if (fSupportedServiceIDs.size()<=0)
		fSupportedServiceIDs.push_back(kInvalidService);
	

}

/* XRailServiceProvider Dtor
*/
XLGServiceProvider::~XLGServiceProvider()
{
}

/* GetName
*/
void XLGServiceProvider::GetName(PMString* pName)
{
	pName->SetCString("XLogin Responder Service");
}

/* GetServiceID
*/
ServiceID XLGServiceProvider::GetServiceID() 
{
	// Should never be called given that HasMultipleIDs() returns kTrue.
	return fSupportedServiceIDs[0];
}

/* IsDefaultServiceProvider
*/
bool16 XLGServiceProvider::IsDefaultServiceProvider()
{
	return kFalse;
}

/* GetInstantiationPolicy
*/
IK2ServiceProvider::InstancePerX XLGServiceProvider::GetInstantiationPolicy()
{
	return IK2ServiceProvider::kInstancePerSession;
}

/* HasMultipleIDs
*/
bool16 XLGServiceProvider::HasMultipleIDs() const
{
	return kTrue;
}

/* GetServiceIDs
*/
void XLGServiceProvider::GetServiceIDs(K2Vector<ServiceID>& serviceIDs)
{
	// push_back a service IDs for each service provided. 
	for (int32 i = 0; i<fSupportedServiceIDs.size(); i++)
		serviceIDs.push_back(fSupportedServiceIDs[i]);

}


// End, DocWchServiceProvider.cpp.



