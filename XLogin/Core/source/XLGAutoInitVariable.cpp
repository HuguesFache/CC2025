#include "VCPlugInHeaders.h"

// interface includes
#include "IApplication.h"
#include "IIdleTaskMgr.h"
#include "CIdleTask.h"
#include "GlobalTime.h"
#include "IDialogMgr.h"
#include "RsrcSpec.h"
#include "LocaleSetting.h"
#include "CoreResTypes.h"
#include "CDialogController.h"
#include "CDialogObserver.h"
#include "ICommand.h"
#include "IBoolData.h"
#include "ICloseWinCmdData.h"
#include "IIntData.h"
#include "IWindow.h"
#include "ICloseWinCmdData.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "IBoolData.h"

// Project includes
#include "XLGID.h"
#include "CAlert.h"


const static int XLGAutoInitExecInterval = 1000;

class XLGAutoInitVariable : public CIdleTask {
    
    public:

        XLGAutoInitVariable(IPMUnknown* boss);
	    virtual ~XLGAutoInitVariable() ;
	    virtual uint32 RunTask(uint32 appFlags, IdleTimer* timeCheck);
	    virtual const char* TaskName();	     

	private: 
		int32 kXLGExecInterval;
};


CREATE_PMINTERFACE(XLGAutoInitVariable, kXLGAutoInitVariableImpl)
XLGAutoInitVariable::XLGAutoInitVariable(IPMUnknown *boss):CIdleTask(boss){}

XLGAutoInitVariable::~XLGAutoInitVariable(){}

uint32 XLGAutoInitVariable::RunTask(uint32 appFlags, IdleTimer* timeCheck) 
{
	do{
		if(	appFlags & (IIdleTaskMgr::kMouseTracking | 
			            IIdleTaskMgr::kUserActive | 
						IIdleTaskMgr::kMenuUp))
			return kOnFlagChange;

		
    InterfacePtr<IBoolData> disableCloseDoc  (GetExecutionContextSession(), IID_IDISABLECLOSEDOC);
	disableCloseDoc->Set(kFalse);

	return XLGAutoInitExecInterval;

	}while(kFalse);
}

const char* XLGAutoInitVariable::TaskName(){
	return kXLGAutoInitVariableKey;

}
