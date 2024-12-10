/*
//
//	Author: GD
//
//	Date: 2004
//
*/

#include "VCPluginHeaders.h"

// Interface includes:
#include "IStartupShutdownService.h"
#include "ISession.h"
#include "IPlugInList.h"
#include "IStringData.h"
#include "IWebServices.h"
#include "IPMStream.h"
#include "IDataStringUtils.h"
#include "IXRailPrefsData.h"
#include "IWorkspace.h"
// General includes:
#include "CAlert.h"
#include "LocaleSetting.h"
#include "FileUtils.h"
#include "DocUtils.h"
#include "StreamUtil.h"
#include "Utils.h"
#include "IIdleTask.h"
#include <string>
#include <fstream>

using namespace std;

// Project includes:
#include "XRLID.h"
#include "XRCID.h"


/** Implements IStartupShutdownService; purpose is to install the idle task.
*/
class XRailStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:


	/**	Constructor
		@param boss 
	 */
    XRailStartupShutdown(IPMUnknown* boss );

	/**	Destructor
	 */
	virtual ~XRailStartupShutdown() {}

	/**	Called by the core when app is starting up
	 */
    virtual void Startup();

	/**	Called by the core when app is shutting down
	 */
    virtual void Shutdown();

private:

	void ReadLine(std::string& line, PMString& ip, PMString& port, int32& fromServeur,  K2Vector<PMString> &listeBases, K2Vector<PMString> &IPBases);
};

CREATE_PMINTERFACE(XRailStartupShutdown, kXRLStartupShutdownImpl)

/*
*/
XRailStartupShutdown::XRailStartupShutdown(IPMUnknown *boss) : CPMUnknown<IStartupShutdownService>(boss) 
{
}

/*
*/
void XRailStartupShutdown::Startup()
{
	// Read config file if are running under InDesign Server	
	ErrorCode status = kFailure;
	PMString ip, port;		
	int32 fromServeur = 0;
	K2Vector<PMString> listeBases;
	K2Vector<PMString> IPBases;

	do{
		if(LocaleSetting::GetLocale().GetProductFS() == kInDesignServerProductFS){
			// Display startup message in server version only
			CAlert::InformationAlert("Startup XRail plugin...\r\n");				 
		}

		IDFile configFile;

		// GD 28/07/2022 ++
		IDFile approamingdatafolder, parent1, parent2, parent3, parent4, parent5;
		PMString message;
		PMString subFolderName; // on laisse volontairement vide.
		FileUtils::GetAppRoamingDataFolder(&approamingdatafolder, subFolderName);
		FileUtils::GetParentDirectory(approamingdatafolder, parent1);
		FileUtils::GetParentDirectory(parent1, parent2);
		FileUtils::GetParentDirectory(parent2, parent3);
		FileUtils::GetParentDirectory(parent3, parent4);
		FileUtils::GetParentDirectory(parent4, parent5);
		FileUtils::GetParentDirectory(parent5, configFile);
		FileUtils::AppendPath(&configFile, "XRail.ini");

		if (!FileUtils::DoesFileExist(configFile)) {
			FileUtils::GetAppInstallationFolder(&configFile);
			FileUtils::AppendPath(&configFile, "XRail.ini");
		}

		InterfacePtr<IPMStream> input (StreamUtil::CreateFileStreamRead(configFile));
		
		if(!input)
			break;
		
		uchar c;
		while(input->GetStreamState() == kStreamStateGood){   
			string line;   
			while((input->GetStreamState() == kStreamStateGood) && (input->XferByte(c) != '\n'))
			{
				if(c == '\r')
					break;
				
				//if(c != '\n')
					line += c;
			}
			ReadLine(line, ip, port, fromServeur, listeBases, IPBases);
		}
			
		input->Close();
		status = kSuccess;

	} while(kFalse);		
		
	if(status == kSuccess){
		PMString oldIp, oldPort, user, baseName;
		bool16 modeRobot = kFalse;
		K2Vector<PMString> oldlisteBases;
		K2Vector<PMString> oldIPBases;
		GetPrefsXRail(oldIp, oldPort, user, modeRobot, oldlisteBases, oldIPBases);
		if(LocaleSetting::GetLocale().GetProductFS() == kInDesignServerProductFS)
			modeRobot = kTrue;	

		// Set preferences, if they've changed
		SetXRailPrefs(ip, port, user, modeRobot, listeBases, IPBases);			

		PMString serverAddress = GetGoodIP(baseName);
		InterfacePtr<IWebServices> xrailWebServices(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailWebServices->SetServerAddress(serverAddress.GetPlatformString().c_str());

		int32 prefs_ImportPub = 0, prefs_PubProp = 0, prefs_PDFMultipages = 1, prefs_ExportIDML = 0;
		if (xrailWebServices->GetPrefsXRail(fromServeur, prefs_ImportPub, prefs_PubProp, prefs_PDFMultipages, prefs_ExportIDML))
		{
			InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
			InterfacePtr<IXRailPrefsData>  xrailPrefsData((IXRailPrefsData*)workspace->QueryInterface(IID_IXRAILPREFSDATA));

			xrailPrefsData->SetImportPub(prefs_ImportPub);
			xrailPrefsData->SetPubProp(prefs_PubProp);
			xrailPrefsData->SetPDFMultipages(prefs_PDFMultipages);
			xrailPrefsData->SetExportIDML(prefs_ExportIDML);
		}
		else {
			CAlert::InformationAlert("Les bases sont inaccessibles");
			InterfacePtr<ICommand> QuitCmd(CmdUtils::CreateCommand(kQuitCmdBoss));
			CmdUtils::ScheduleCommand(QuitCmd);
		}
	}
	else if(LocaleSetting::GetLocale().GetProductFS() == kInDesignServerProductFS)
		CAlert::ErrorAlert(PMString(kXRLErrorMissingConfigFileKey));
}

/*
*/
void XRailStartupShutdown::Shutdown()
{
}


void XRailStartupShutdown::ReadLine(std::string& line, PMString& ip, PMString& port, int32& fromServeur, K2Vector<PMString> &listeBases, K2Vector<PMString> &IPBases)
{
	PlatformChar equal;
	equal.Set('=');
	
	PMString str(line.c_str());								
	if(!str.IsEmpty())
	{
		if(str[str.NumUTF16TextChars()-1].IsChar('\r'))
		str.Remove(str.NumUTF16TextChars()-1);

		int32 pos = str.IndexOfCharacter(equal);
		if(pos != -1)
		{
			PMString * keyFound = str.Substring(0,pos);
			if(keyFound)  
			{
				keyFound->StripWhiteSpace();

				PMString * value = str.Substring(pos+1);
				if(value)
				{
					if(!value->IsEmpty())
					{
						value->StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);						
						
						if(keyFound->Compare(kFalse, "IP") == 0)
							ip = *value;										
						else if(keyFound->Compare(kFalse, "PORT") == 0)
							port = *value;
						else if(keyFound->Compare(kFalse, "SERVEUR") == 0)
							fromServeur = Utils<IDataStringUtils>()->StringToInt32(WideString(*value));
#if MULTIBASES == 1
						else if(keyFound->Compare(kFalse, "nomBase") == 0)
							listeBases.push_back(*value);
						else if(keyFound->Compare(kFalse, "ipBase") == 0)
							IPBases.push_back(*value);
#endif
					}

					delete value;						
				}
				delete keyFound;
			}
		}
	}
}

