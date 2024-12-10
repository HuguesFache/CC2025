
#ifndef __IXLGPreferences_h__
#define __IXLGPreferences_h__

#include "IPMUnknown.h"
#include "XLGID.h"
#include "K2Vector.h"
#include "K2Vector.tpp"
#include "KeyValuePair.h"

class IXLGPreferences : public IPMUnknown
{
public:
     
	enum { kDefaultIID = IID_IXLGPREFERENCES };
    
	// TEC section
	virtual PMString GetXRail_URL() const =0;
	virtual void SetXRail_URL(const PMString& str) =0;

    // Pages section
	virtual K2Vector<KeyValuePair<PMString, PMString> > GetStatuses() const =0;
	virtual void SetStatuses(const K2Vector<KeyValuePair<PMString, PMString> >& map) =0;

	// Timer for closing dialog
	virtual PMString GetCloseDialogTimer() const =0;
	virtual void SetCloseDialogTimer(const PMString& str) =0;

};

#endif // __IXLGPreferences_h__
