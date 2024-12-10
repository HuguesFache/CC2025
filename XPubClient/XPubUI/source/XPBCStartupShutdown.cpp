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
#include "IBoolData.h"
#include "IStringData.h"

// General includes:
#include "CAlert.h"
#include "FileUtils.h"
#include "KeyValuePair.h"
#include "StreamUtil.h"

// Project includes:
#include "XPBCID.h"

#include <string>
#include <fstream>

using namespace std;


/** Implements IStartupShutdownService; purpose is to read configuration file
*/
class XPBCStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:


	/**	Constructor
		@param boss 
	 */
    XPBCStartupShutdown(IPMUnknown* boss );

	/**	Destructor
	 */
	virtual ~XPBCStartupShutdown() {}

	/**	Called by the core when app is starting up
	 */
    virtual void Startup();

	/**	Called by the core when app is shutting down
	 */
    virtual void Shutdown();

private:

	const static PMString configFileName;

	/** Read preferences in config file
	*/
	virtual void ReadPrefs(PMString& ip, PMString& port, PMString& stylePDF);

};

CREATE_PMINTERFACE(XPBCStartupShutdown, kXPBCStartupShutdownImpl)

const PMString XPBCStartupShutdown::configFileName = "XPubClient.ini";

/*
*/
XPBCStartupShutdown::XPBCStartupShutdown(IPMUnknown *boss) : CPMUnknown<IStartupShutdownService>(boss) 
{
	
}

/*
*/
void XPBCStartupShutdown::Startup()
{	
// Initialisation des globales
	InterfacePtr<IBoolData> gLogin (GetExecutionContextSession(), IID_IBOOL_LOGIN);
	gLogin->Set(kFalse);
	
	// Lecture du fichier de preferences
	PMString ip = "192.168.0.1"; // Valeurs par defaut
	PMString port = "8080";
	PMString stylePDF;
	this->ReadPrefs(ip, port, stylePDF);
	
	PMString theServerIP = "http://" + ip + ":" + port + "/4DSOAP/";
	theServerIP.SetTranslatable(kFalse);

	InterfacePtr<IStringData> ServerIP (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
	ServerIP->Set(theServerIP);

	InterfacePtr<IStringData> styleExportPDF (GetExecutionContextSession(), IID_ISTRINGDATA_STYLEPDF);
	styleExportPDF->Set(stylePDF);
	
}

/*
*/
void XPBCStartupShutdown::Shutdown()
{
}

/* GD 2015 : NOTE IMPORTANTE, a partir de la CS5.5 (portage depuis la CS4) le fichier XPubClient.ini doit etre en UTF-8.
*/
void XPBCStartupShutdown::ReadPrefs(PMString& ip, PMString& port, PMString& stylePDF)
{
	do
	{	
		IDFile configFile;
		
		FileUtils::GetAppInstallationFolder(&configFile);
		FileUtils::AppendPath(&configFile, configFileName);
	
		if(!FileUtils::DoesFileExist(configFile))
		{
			CAlert::ErrorAlert(kXPBCErrorINIFileNotFoundKey);
			break;
		}
		
		PlatformChar equal;
		equal.Set('=');
	
		K2Vector<KeyValuePair<PMString, PMString> >  sectionValues;
	
		InterfacePtr<IPMStream> input (StreamUtil::CreateFileStreamRead(configFile));
		if (!input)    
			break;
		
		PMString currentSection = kNullString;		
		
		while(input->GetStreamState() == kStreamStateGood){   
			uchar c;
			string line;
			while((input->GetStreamState() == kStreamStateGood) && (input->XferByte(c) != '\n')){
				if ((c != '\r') && (c != '\n'))
					line += c;
			}
			
			// GD 2015 ++
			//PMString str(line.c_str());
			PMString str(line.c_str(), PMString::kDontTranslateDuringCall);
			// GD 2015 --
			if(!str.IsEmpty())
			{
				if(str[str.NumUTF16TextChars()-1].IsChar('\r'))
					str.Remove(str.NumUTF16TextChars()-1);

				str.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);
			
				if(str.IsEmpty())
					continue;
				
				if( str[0] == PlatformChar('[') && str[str.NumUTF16TextChars()-1] == PlatformChar(']') ) {
					currentSection = str;
				} else {
					int32 equalPos = str.IndexOfCharacter(equal);
					if(equalPos >= 0)
					 {	
						 K2::scoped_ptr<PMString> key (str.Substring(0, equalPos));
						 if(key && !key->IsEmpty())
						 {					
							// GD 2015 ++
							//K2::scoped_ptr<PMString> value (str.Substring(equalPos+1));
							PMString * value = str.Substring(equalPos+1);
							// GD 2015 --
							// Value is a path
							if(value && !value->IsEmpty())
							{
#ifdef MACINTOSH
								// GD 24.08.10 ++
								// Pas de 'DecomposeUnicode' sinon les accents format Mac ne sont pas convertis
								// automatiquement par InDesign en UTF8 a la lecture du fichier ini, ce qui
								// pose des problemes comme par exemple la selection du profil de generation PDF.
								//PMString valueBis = FileUtils::DecomposeUnicode(*value);
								PMString valueBis = *value;
								// GD 24.08.10 --
#else
								PMString valueBis = *value;
#endif						
								if(key->Compare(kFalse, "IP")  == 0 && currentSection.Compare(kFalse, "[SERVER]") == 0)
									ip = valueBis;							
								else if(key->Compare(kFalse, "PORT")  == 0 && currentSection.Compare(kFalse, "[SERVER]") == 0)
									port = valueBis;
								else if(key->Compare(kFalse, "STYLE")  == 0 && currentSection.Compare(kFalse, "[PDF]") == 0)
									stylePDF = valueBis;
							}		
						 }
					 }	
				}				 
			}
		}
		// On referme le fichier de prefs
		input->Close();

	} while(kFalse);	
	
		
}
