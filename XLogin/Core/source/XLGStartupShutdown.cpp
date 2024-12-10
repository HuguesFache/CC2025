#include "VCPluginHeaders.h"

// Interface includes:
#include "IStartupShutdownService.h"
#include "ISession.h"

// General includes:
#include "CAlert.h"
#include "LocaleSetting.h"
#include "IXLGPreferences.h"
#include "FileUtils.h"
#include "ICommandSequence.h"
#include "IDialog.h"
#include "IDialogMgr.h"
#include "IApplication.h"
#include "RsrcSpec.h"
#include "LocaleSetting.h"
#include "LocaleIndex.h"
#include "IBoolData.h"
#include "IIntData.h"
#include "StreamUtil.h"
#include "IDataStringUtils.h"
#include "Utils.h"

// Project includes:
#include "XLGID.h"


#include <fstream>
using namespace std;


/** Implements IStartupShutdownService; purpose is to install the idle task.
*/
class XLGStartupShutdown : public CPMUnknown<IStartupShutdownService>
{
public:


	/**	Constructor
		@param boss 
	 */
    XLGStartupShutdown(IPMUnknown* boss );

	/**	Destructor
	 */
	virtual ~XLGStartupShutdown() {}

	/**	Called by the core when app is starting up
	 */
    virtual void Startup();

	/**	Called by the core when app is shutting down
	 */
    virtual void Shutdown();

private :

    // Read configuration file data
	const static PMString configFileName; 
    
    void ReadConfFile();
    

};

CREATE_PMINTERFACE(XLGStartupShutdown, kXLGStartupShutdownImpl)

// Const Static variable initialization
const PMString XLGStartupShutdown::configFileName = PMString("XLogin.ini", PMString::kUnknownEncoding);


/*
*/
XLGStartupShutdown::XLGStartupShutdown(IPMUnknown *boss) : CPMUnknown<IStartupShutdownService>(boss) 
{}

/*
*/
void XLGStartupShutdown::Startup()
{    
        // Initialize preferences
		ReadConfFile();
 
        InterfacePtr<ICommand> openLoginDialogCmd (CmdUtils::CreateCommand(kXLGOpenLoginDialogCmdBoss));
        CmdUtils::ScheduleCommand(openLoginDialogCmd , ICommand::kLowestPriority);


		InterfacePtr<IXLGPreferences> xlgrefs(GetExecutionContextSession(), UseDefaultIID());
		InterfacePtr<IIntData> timerClosingDialog (GetExecutionContextSession(), IID_ITIMERCLOSEDIALOG);
		timerClosingDialog->Set(xlgrefs->GetCloseDialogTimer().GetAsNumber());

}
/*
*/
void XLGStartupShutdown::Shutdown()
{
}

void XLGStartupShutdown::ReadConfFile()
{
	InterfacePtr<IXLGPreferences> xlgPrefs (GetExecutionContextSession(), UseDefaultIID());
	
	IDFile configFile;
	
	FileUtils::GetAppInstallationFolder(&configFile);
	FileUtils::AppendPath(&configFile, configFileName);
	
	PlatformChar equal;
	equal.Set('=');
	
	K2Vector<KeyValuePair<PMString, PMString> >  sectionValues;
	
	do
	{	
		InterfacePtr<IPMStream> input (StreamUtil::CreateFileStreamRead(configFile));	
		if(!input)
			break;
		
		//string line;
		uchar c;
		
		PMString currentSection = kNullString;
		PMString exportFileName = kNullString;
		K2Vector<KeyValuePair<PMString, PMString> > statusMap;
		
		while (input->GetStreamState() == kStreamStateGood)
		{
			
			string line;
			while((input->GetStreamState() == kStreamStateGood) && (input->XferByte(c) != '\r')){
				if(c != '\n')
					line += c;
				
				// GD 7.5.2 ++
				// Lecture ligne Win ET Mac (-> retour chariot '\n' uniquement, pas '\r\n')
				if(c == '\n')
					break;
				// GD 7.5.2 --
			}
			
			PMString str(line.c_str());
			if(!str.IsEmpty())
			{
				if(str[str.NumUTF16TextChars()-1].IsChar('\r'))
					str.Remove(str.NumUTF16TextChars()-1);
				
				str.StripWhiteSpace(PMString::kLeadingAndTrailingWhiteSpace);
				
				if(str.IsEmpty())
					continue;
				
				if( str[0] == PlatformChar('[') && str[str.NumUTF16TextChars()-1] == PlatformChar(']') )
					currentSection = str;
				else
				{
					int32 equalPos = str.IndexOfCharacter(equal);
					if(equalPos >= 0)
					{						 
						K2::scoped_ptr<PMString> key (str.Substring(0, equalPos));
						if(key && !key->IsEmpty())
						{					
							K2::scoped_ptr<PMString> value (str.Substring(equalPos+1));
							// Value is a path
							if(value && !value->IsEmpty()){
#ifdef MACINTOSH
								PMString valueBis = FileUtils::DecomposeUnicode(*value);
#else
								PMString valueBis = *value;
#endif							
							    if(key->Compare(kFalse, "URL")  == 0)
								    xlgPrefs->SetXRail_URL(valueBis);
								
								else if(key->Compare(kFalse, "CloseDialogTimer")  == 0)
								    xlgPrefs->SetCloseDialogTimer(valueBis);
								
							}
						}		
					}
				}	
			}				 
		}
		xlgPrefs->SetStatuses(statusMap);
		input->Close();
		
	} while(kFalse);	
}

