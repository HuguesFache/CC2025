#include "VCPluginHeaders.h"

// Interface includes:
#include "IXLGPreferences.h"

// General includes:
#include "CAlert.h"

// Project includes:
#include "XLGID.h"


/** Implements IStartupShutdownService; purpose is to install the idle task.
*/
class XLGPreferences : public CPMUnknown<IXLGPreferences>
{
public:


	/**	Constructor
		@param boss 
	 */
    XLGPreferences(IPMUnknown* boss );

	/**	Destructor
	 */
	virtual ~XLGPreferences();

	virtual PMString GetXRail_URL() const;
	virtual void SetXRail_URL(const PMString& str);
    
  	virtual K2Vector<KeyValuePair<PMString, PMString> > GetStatuses() const ;
	virtual void SetStatuses(const K2Vector<KeyValuePair<PMString, PMString> >& map);

	virtual PMString GetCloseDialogTimer() const;
	virtual void SetCloseDialogTimer(const PMString& str);

private:
      K2Vector<KeyValuePair<PMString, PMString> > statusMap;
      PMString urlXRail, closeDialogTimer;
};

CREATE_PMINTERFACE(XLGPreferences, kXLGPreferencesImpl)


/**	Constructor
 */
XLGPreferences::XLGPreferences(IPMUnknown* boss )  : CPMUnknown<IXLGPreferences>(boss)
{	
	urlXRail = kNullString;
}

/**	Destructor
 */
XLGPreferences::~XLGPreferences()
{
}

PMString XLGPreferences::GetXRail_URL() const
{
	return urlXRail;
}

void XLGPreferences::SetXRail_URL(const PMString& str)
{
	urlXRail = str;
}

PMString XLGPreferences::GetCloseDialogTimer() const
{
	return closeDialogTimer;
}
void XLGPreferences::SetCloseDialogTimer(const PMString& str)
{
	closeDialogTimer = str;
}

K2Vector<KeyValuePair<PMString, PMString> > XLGPreferences::GetStatuses() const
{
	return statusMap;
}

void XLGPreferences::SetStatuses(const K2Vector<KeyValuePair<PMString, PMString> >& map)
{
	statusMap = map;
}
