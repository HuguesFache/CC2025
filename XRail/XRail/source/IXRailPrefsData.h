
#ifndef __IXRailPrefsData__
#define __IXRailPrefsData__

#include "IPMUnknown.h"
#include "XRLID.h"

/**	Interface for XRail preferences data.
 */
class IXRailPrefsData : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IXRAILPREFSDATA };

	/** 
	*/
	virtual void SetServeurIP(const PMString& name) = 0;
	virtual void SetPort(const PMString& name) = 0;
	virtual void SetLastUser(const PMString& lastUser) = 0;
	virtual void SetModeRobot(const bool16& enable) =0;
#if MULTIBASES == 1
	virtual void SetListeBases(const K2Vector<PMString>liste) = 0;
	virtual void SetIPBases(const K2Vector<PMString>liste) = 0;
#endif
	/**	

	*/
	virtual PMString GetServeurIP() = 0;
	virtual PMString GetPort() = 0;
	virtual PMString GetLastUser() = 0;
	virtual bool16 GetModeRobot() =0;
#if MULTIBASES == 1
	virtual K2Vector<PMString> GetListeBases() =0;
	virtual K2Vector<PMString> GetIPBases() =0;
#endif

	//Preferences globales
	virtual int32 GetImportPub() = 0;
	virtual void SetImportPub(const int32& b) = 0;

	virtual int32 GetPubProp() = 0;
	virtual void SetPubProp(const int32& b) = 0;

	virtual int32 GetPDFMultipages() = 0;
	virtual void SetPDFMultipages(const int32& b) = 0;

	virtual int32 GetExportIDML() = 0;
	virtual void SetExportIDML(const int32& b) = 0;

};

#endif // __ISnapPrefsData__



