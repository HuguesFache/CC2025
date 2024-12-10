//========================================================================================
//  
//  $File: //depot/indesign_3.0/gm/source/sdksamples/snapshot/SnapPrefsDataPersist.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2003/09/30 15:41:37 $
//  
//  $Revision: #1 $
//  
//  $Change: 223184 $
//  
//  Copyright 1997-2003 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//  
//  SnapPrefsDataPersist is the persistent implementation of ISnapPrefsData. 
//  
//========================================================================================

#include "VCPluginHeaders.h"
 
// Interface includes:
#include "IPMStream.h"
 
// General includes:
#include "TRACE.h"
#include "CPMUnknown.h"

// Project includes:
#include "XRLID.h"
#include "IXRailPrefsData.h"

/**	SnapPrefsDataPersist

	The persistent implementation of ISnapPrefsData.
	@author	Jane Bond
	@ingroup snapshot
*/
class XRailPrefsDataPersist : public CPMUnknown<IXRailPrefsData>
{
	public:
		XRailPrefsDataPersist(IPMUnknown* boss);

		virtual ~XRailPrefsDataPersist();
	
		virtual void SetServeurIP(const PMString& name);
		virtual PMString GetServeurIP();

		virtual void SetPort(const PMString& name);
		virtual PMString GetPort();

		virtual void SetLastUser(const PMString& lastUser);
		virtual PMString GetLastUser();
		
		virtual void SetModeRobot(const bool16& enable);
		virtual bool16 GetModeRobot();

#if MULTIBASES == 1
		virtual void SetListeBases(const K2Vector<PMString>listeBases);
		virtual K2Vector<PMString> GetListeBases();

		virtual void SetIPBases(const K2Vector<PMString>listeBases);
		virtual K2Vector<PMString> GetIPBases();
#endif
		
		virtual void ReadWrite(IPMStream* s, ImplementationID prop);
		
		//Preferences globales
		virtual int32 GetImportPub() ;
		virtual void SetImportPub(const int32& b);

		virtual int32 GetPubProp();
		virtual void SetPubProp(const int32& b);

		virtual int32 GetPDFMultipages();
		virtual void SetPDFMultipages(const int32& b);

		virtual int32 GetExportIDML();
		virtual void SetExportIDML(const int32& b) ;

	private:

		PMString serveurIP;
		PMString port;
		PMString lastUser;
		int32 ImportPub, PubProp, PDFMultipages, ExportIDML;
		bool16 modeRobot;

#if MULTIBASES == 1
		K2Vector<PMString> listeBases;
		K2Vector<PMString> IPBases;
#endif
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PERSIST_PMINTERFACE(XRailPrefsDataPersist, kXRLPrefsDataPersistImpl)

/* Constructor
*/
XRailPrefsDataPersist::XRailPrefsDataPersist(IPMUnknown* boss) : 	
		CPMUnknown<IXRailPrefsData>(boss), serveurIP("0.0.0.0"), port("8080"), lastUser(""),  modeRobot(kFalse) 
{
}

/* Destructor.
*/
XRailPrefsDataPersist::~XRailPrefsDataPersist()
{
}


/* SetName
*/
void XRailPrefsDataPersist::SetServeurIP(const PMString& name)
{
	if (serveurIP != name)
	{
		PreDirty();
		serveurIP = name;		
	}
}

/* GetName
*/
PMString XRailPrefsDataPersist::GetServeurIP()
{
	return serveurIP;
}


void XRailPrefsDataPersist::SetPort(const PMString& name)
{
	if (port != name)
	{
		PreDirty();
		port = name;
	}
}

/* GetName
*/
PMString XRailPrefsDataPersist::GetPort()
{
	return port;
}

/* SetName
*/
void XRailPrefsDataPersist::SetLastUser(const PMString& name)
{
	if (lastUser != name)
	{
		PreDirty();
		lastUser = name;
	}
}

/* GetName
*/
PMString XRailPrefsDataPersist::GetLastUser()
{
	return lastUser;
}

/* SetModeRobot
*/
void XRailPrefsDataPersist::SetModeRobot(const bool16& enable)
{
	PreDirty();
	modeRobot = enable;
}

/* GetModeRobot
*/
bool16 XRailPrefsDataPersist::GetModeRobot()
{
	return modeRobot;
}

/* SetListeBases
*/
#if MULTIBASES == 1
void XRailPrefsDataPersist::SetListeBases(const K2Vector<PMString>liste)
{
	PreDirty();
	listeBases = liste;
}

/* GetListeBases
*/
K2Vector<PMString> XRailPrefsDataPersist::GetListeBases()
{
	return listeBases;
}

/* SetIPBases
*/

void XRailPrefsDataPersist::SetIPBases(const K2Vector<PMString>ip)
{
	PreDirty();
	IPBases = ip;
}

/* GetListeBases
*/
K2Vector<PMString> XRailPrefsDataPersist::GetIPBases()
{
	return IPBases;
}
#endif

void XRailPrefsDataPersist::SetImportPub(const int32& value)
{
	PreDirty();
	ImportPub = value;
}

int32 XRailPrefsDataPersist::GetImportPub()
{
	return ImportPub;
}

void XRailPrefsDataPersist::SetPubProp(const int32& value)
{
	PreDirty();
	PubProp = value;
}

int32 XRailPrefsDataPersist::GetPubProp()
{
	return PubProp;
}

void XRailPrefsDataPersist::SetExportIDML(const int32& value)
{
	PreDirty();
	ExportIDML = value;
}

int32 XRailPrefsDataPersist::GetExportIDML()
{
	return ExportIDML;
}

void XRailPrefsDataPersist::SetPDFMultipages(const int32& value)
{
	PreDirty();
	PDFMultipages = value;
}

int32 XRailPrefsDataPersist::GetPDFMultipages()
{
	return PDFMultipages;
}


/* ReadWrite
*/
void XRailPrefsDataPersist::ReadWrite(IPMStream* s, ImplementationID prop)
{
	int32 numOptions = 0;
	serveurIP.ReadWrite(s);
	port.ReadWrite(s);
	lastUser.ReadWrite(s);
	s->XferBool(modeRobot);

#if MULTIBASES == 1
	numOptions = listeBases.size();
	for(int32 i=0; i < numOptions; i++)
	{
		listeBases[i].ReadWrite(s);
		IPBases[i].ReadWrite(s);
	}
#endif


}

// End, SnapPrefsDataPersist.cpp.


