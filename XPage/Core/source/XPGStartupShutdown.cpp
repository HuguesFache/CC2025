/*
//
//	Author: Trias Developpement
//
//	Date: 22-Mar-2007
//
*/
#include "VCPluginHeaders.h"

// Interface includes:
#include "IStartupShutdownService.h"
#include "IDataStringUtils.h"
#include "ISerialNumber.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "ISAXServices.h"		  
#include "ISAXContentHandler.h"
#include "ISAXEntityResolver.h"		  
#include "ISAXParserOptions.h"
#include "IPMStream.h"
#include "IIntData.h"
#include "IWorkspace.h"
#include "IKeyValueData.h"

// General includes:
#include "CAlert.h"
#include "FileUtils.h"
#include "Utils.h"
#include "StreamUtil.h"
#include "ErrorUtils.h"
#include "StringUtils.h"
#include "ISysFileData.h"
#include "IObserver.h"
#include <fstream>

// Project includes:
#include "XPGID.h"
#include "IXPGPreferences.h"
#include "IWebServices.h"

using namespace std;

/** Implements IStartupShutdownService; purpose is to install the idle task.
*/
class XPGStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:
	/**	Constructor
		@param boss 
	 */
    XPGStartupShutdown(IPMUnknown* boss );
	/**	Destructor
	 */
	virtual ~XPGStartupShutdown() {}
	/**	Called by the core when app is starting up
	 */
    virtual void Startup();
	/**	Called by the core when app is shutting down	 */
    virtual void Shutdown();

private:

	// Read configuration file data
	const static PMString configFileName; 	
	void ReadConfFile();

	ErrorCode ReadXMLExportFile(const IDFile& xmlExportFile);
	ErrorCode ReadXMLListTags(const IDFile& xmlTagsFile);

};

CREATE_PMINTERFACE(XPGStartupShutdown, kXPGStartupShutdownImpl)
// Const Static variable initialization
const PMString XPGStartupShutdown::configFileName = PMString("XPage.ini", PMString::kUnknownEncoding);
/*
*/
XPGStartupShutdown::XPGStartupShutdown(IPMUnknown *boss) : CPMUnknown<IStartupShutdownService>(boss){}

/*
*/
void XPGStartupShutdown::Startup()
{	
	do {	
		if(LocaleSetting::GetLocale().GetProductFS() == kInDesignServerProductFS){
			// Display startup message in server version only
			CAlert::InformationAlert("Startup XPage plugin...\r\n");
		}

		// Initialize preferences
		ReadConfFile();

        // Init AdronmentShape
        InterfacePtr<IBoolData> isAdronmentShape (GetExecutionContextSession(), IID_IDISPLAYSHAPE);
		isAdronmentShape->Set(kFalse);       
	} while(kFalse);
}

/* Shutdown
*/
void XPGStartupShutdown::Shutdown()
{
}

void XPGStartupShutdown::ReadConfFile(){

	do{

		InterfacePtr<IXPGPreferences> xpgPrefs (GetExecutionContextSession(), UseDefaultIID());
		IDFile configFile;	
	
		FileUtils::GetAppInstallationFolder(&configFile);	
		FileUtils::AppendPath(&configFile, configFileName);			
		
		K2Vector<KeyValuePair<PMString, PMString> >  sectionValues;	
		
		InterfacePtr<IPMStream> input (StreamUtil::CreateFileStreamRead(configFile));	
		if(!input)
			break;
	
		uchar c;
		PMString exportFileName = kNullString;
		PMString currentSection = kNullString;

		xpgPrefs->SetImportLegende(true);
		xpgPrefs->SetImportCredit(true);

		xpgPrefs->SetExportXML(0);
		xpgPrefs->SetGestionIDMS(1);
		xpgPrefs->SetIDMS_MAJIDMS(1);
		xpgPrefs->SetIDMSALLBLOCS(0);

		while(input->GetStreamState() == kStreamStateGood){
			string line;
			while((input->GetStreamState() == kStreamStateGood) && (input->XferByte(c) != '\r')){
				if(c != '\n')
					line += c;
				
				// GD 7.5.4 ++
				// Lecture ligne Win ET Mac (-> retour chariot '\n' uniquement, pas '\r\n')
				if(c == '\n')
					break;
				// GD 7.5.4 --
			}
		
			PlatformChar equal;	
			equal.Set('=');			
			PMString str(line.c_str());			
			
			if(!str.IsEmpty()){
				
				if(str[str.NumUTF16TextChars()-1].IsChar('\r'))
					str.Remove(str.NumUTF16TextChars()-1);

				str.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);			
				if(str.IsEmpty())
					continue;				
				
				if( str[0] == '[' && str[str.NumUTF16TextChars()-1] == ']') {
					currentSection = str;					
				}
				else{
					 int32 equalPos = str.IndexOfCharacter(equal);
					 if(equalPos >= 0){						 
						 K2::scoped_ptr<PMString> key (str.Substring(0, equalPos));
						 if(key && !key->IsEmpty()) {					
							K2::scoped_ptr<PMString> value (str.Substring(equalPos + 1));
							 // Value is a path
							if(value && !value->IsEmpty()){
#ifdef MACINTOSH
								PMString valueBis = FileUtils::DecomposeUnicode(*value);			
#else
								PMString valueBis = *value;								
#endif														
								if( (key->Compare(kFalse, "RacineRedactions") == 0) || (key->Compare(kFalse, "RacineArbo")  == 0) )
									xpgPrefs->SetRacineArbo(valueBis);
								else if(key->Compare(kFalse, "Etat")  == 0  && currentSection.Compare(kFalse, "[Assemblage]") == 0)
									xpgPrefs->SetEtatAssemblage(valueBis);
								else if(key->Compare(kFalse, "Chemin")  == 0 && currentSection.Compare(kFalse, "[Assemblage]") == 0)
									xpgPrefs->SetCheminAssemblage(valueBis);
								else if(key->Compare(kFalse, "Typo")  == 0 && currentSection.Compare(kFalse, "[Textes]") == 0)
									xpgPrefs->SetTypoDir(valueBis);
								else if(key->Compare(kFalse, "Chemin")  == 0 && currentSection.Compare(kFalse, "[Textes]") == 0)
									xpgPrefs->SetCheminTextes(valueBis);
								else if(key->Compare(kFalse, "MatchingParDefaut")  == 0)
									xpgPrefs->SetMatchingParDefaut(valueBis);
								else if(key->Compare(kFalse, "ExportXML")  == 0)
									exportFileName = valueBis;
								else if(key->Compare(kFalse, "Chemin")  == 0 /*&& currentSection.Compare(kFalse, "[Formes]") == 0*/)
									xpgPrefs->SetCheminFormes(valueBis);									
								else if(key->Compare(kFalse, "CheminAssemblage")  == 0 /*&& currentSection.Compare(kFalse, "[Formes]") == 0*/)
									xpgPrefs->SetCheminAssemblageCartons(valueBis);
								else if(key->Compare(kFalse, "EtatPlace")  == 0)
									xpgPrefs->SetEtatPlace(valueBis);
								else if(key->Compare(kFalse, "EtatNonPlace")  == 0)
									xpgPrefs->SetEtatNonPlace(valueBis);
								else if(key->Compare(kFalse, "URL")== 0)
									xpgPrefs->SetXRail_URL(valueBis);
								else if(key->Compare(kFalse, "UnlockStatus")  == 0)
									xpgPrefs->SetStatus(valueBis);	
                                else if(key->Compare(kFalse, "NoUpdate")  == 0)
                                    xpgPrefs->SetNoUpdateFile(valueBis);
                                else if(key->Compare(kFalse, "CreationForme")  == 0)
									xpgPrefs->SetCreationForme( Utils<IDataStringUtils>()->StringToBool(WideString(valueBis)) );
							    else if(key->Compare(kFalse, "CheminBalises") == 0)
									xpgPrefs->SetBalisesXMLPath(valueBis);  
								
							}
						 }						
					 }						
				}				 
			}
		}
		
		InterfacePtr<IWebServices> xrailConnection(::CreateObject2<IWebServices>(kXRCXRailClientBoss));
		xrailConnection->SetServerAddress(xpgPrefs->GetXRail_URL());
		int32 fromServeur = 0, prefs_ExportXML, prefs_GestionIDMS, prefs_IDMSMAJIDMS, prefs_IDMSALLBLOCS, prefs_ImportLegendes, prefs_ImportCredits;
		if (xrailConnection->GetPrefsXPage(fromServeur, prefs_ExportXML, prefs_GestionIDMS, prefs_IDMSMAJIDMS, prefs_IDMSALLBLOCS, prefs_ImportLegendes, prefs_ImportCredits))
		{
			xpgPrefs->SetExportXML(prefs_ExportXML);
			xpgPrefs->SetGestionIDMS(prefs_GestionIDMS);
			xpgPrefs->SetIDMS_MAJIDMS(prefs_IDMSMAJIDMS);
			xpgPrefs->SetIDMSALLBLOCS(prefs_IDMSALLBLOCS);
			xpgPrefs->SetImportLegende(prefs_ImportLegendes);
			xpgPrefs->SetImportCredit(prefs_ImportCredits);
		}
		else {
			CAlert::InformationAlert("Les bases sont inaccessibles");
			InterfacePtr<ICommand> QuitCmd(CmdUtils::CreateCommand(kQuitCmdBoss));
			CmdUtils::ScheduleCommand(QuitCmd);
		}

		// Alimentation de l'interface de creation de cartons
		if(xpgPrefs->GetBalisesXMLPath() != kNullString )
		{
			IDFile xmlTagsListFile = FileUtils::PMStringToSysFile(xpgPrefs->GetBalisesXMLPath());
			if(FileUtils::DoesFileExist(xmlTagsListFile))
			{
				this->ReadXMLListTags(xmlTagsListFile);
			}
		}		

		//
		if (xpgPrefs->GetExportXML() == 1) {
			if (exportFileName != kNullString)
			{
				IDFile xmlExportFile = FileUtils::PMStringToSysFile(exportFileName);
				if (FileUtils::DoesFileExist(xmlExportFile))
				{
					this->ReadXMLExportFile(xmlExportFile);
				}
				else {
					CAlert::InformationAlert("Le fichier d'export n'est pas accessible. Relancez Indesign: "+ exportFileName);
				}
			}
		}
		// Close File
		input->Close();
	}while(kFalse);
}


ErrorCode XPGStartupShutdown::ReadXMLExportFile(const IDFile& xmlExportFile){

	ErrorCode status = kFailure;
	do{
		InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID()); 
		InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss)); 
		InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID()); 
		InterfacePtr<ISAXContentHandler> contentHandler(::CreateObject2<ISAXContentHandler>(kXPGXMLExportFileHandlerBoss, ISAXContentHandler::kDefaultIID)); 
		contentHandler->Register(saxServices);
		InterfacePtr<ISAXEntityResolver> entityResolver(::CreateObject2<ISAXEntityResolver>(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID));
		InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());
		
		// These commitments: follow 
		// the existing ImportXMLFileCmd
		parserOptions->SetNamespacesFeature(kFalse);
	    parserOptions->SetShowWarningAlert(kFalse);
		
		InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(xmlExportFile));

		saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);		
		if(ErrorUtils::PMGetGlobalErrorCode() != kSuccess){
			// Reset error code and quit
		 	ErrorUtils::PMSetGlobalErrorCode(kSuccess);
		 	PMString error (kXPGErrParseExportXMLFailed);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(xmlExportFile));  
			break;
		}

		status = kSuccess;

	}while(kFalse);

	return status;
}


ErrorCode XPGStartupShutdown::ReadXMLListTags(const IDFile& xmlTagsFile) {	

	ErrorCode status = kFailure;
	do{
		InterfacePtr<IK2ServiceRegistry> serviceRegistry(GetExecutionContextSession(), UseDefaultIID()); 
		InterfacePtr<IK2ServiceProvider> xmlProvider(serviceRegistry->QueryServiceProviderByClassID(kXMLParserService, kXMLParserServiceBoss)); 
	
		InterfacePtr<ISAXServices> saxServices(xmlProvider, UseDefaultIID()); 

		InterfacePtr<ISAXContentHandler> contentHandler(static_cast<ISAXContentHandler*>(::CreateObject(kXPGXMLMatchingStylesListHandlerBoss, ISAXContentHandler::kDefaultIID))); 
		contentHandler->Register(saxServices);

		InterfacePtr<ISAXEntityResolver> entityResolver(static_cast<ISAXEntityResolver*>(::CreateObject(kXMLEmptyEntityResolverBoss, ISAXEntityResolver::kDefaultIID)));
		InterfacePtr<ISAXParserOptions> parserOptions(saxServices, UseDefaultIID());
	
		// These commitments: follow 
		// the existing ImportXMLFileCmd
		parserOptions->SetNamespacesFeature(kFalse);
	    parserOptions->SetShowWarningAlert(kFalse);

		InterfacePtr<IPMStream> xmlFileStream(StreamUtil::CreateFileStreamRead(xmlTagsFile));
		saxServices->ParseStream(xmlFileStream, contentHandler, entityResolver);

		if(ErrorUtils::PMGetGlobalErrorCode() != kSuccess){
			// Reset error code and quit
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);	 		
			break;
		}
		status = kSuccess;

	}while(kFalse);
	return status;
}
