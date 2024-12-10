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

// Project includes
#include "XLGID.h"
#include "CAlert.h"

class XLGAutoClosingDialog : public CIdleTask {
    
    public:

        XLGAutoClosingDialog(IPMUnknown* boss);
	    virtual ~XLGAutoClosingDialog() ;
	    virtual uint32 RunTask(uint32 appFlags, IdleTimer* timeCheck);
	    virtual const char* TaskName();	     

	private: 
		int32 kXLGExecInterval;
};


CREATE_PMINTERFACE(XLGAutoClosingDialog, kXLGAutoClosingDialogImpl)
XLGAutoClosingDialog::XLGAutoClosingDialog(IPMUnknown *boss):CIdleTask(boss){}

XLGAutoClosingDialog::~XLGAutoClosingDialog(){}

uint32 XLGAutoClosingDialog::RunTask(uint32 appFlags, IdleTimer* timeCheck) 
{
	do{
		if(	appFlags & (IIdleTaskMgr::kMouseTracking | 
			            IIdleTaskMgr::kUserActive | 
						IIdleTaskMgr::kMenuUp))
			return kOnFlagChange;
		
		InterfacePtr<IApplication> application ( GetExecutionContextSession()->QueryApplication() );
		InterfacePtr<IDialogMgr> dialogMgr ( application, UseDefaultIID() );
		InterfacePtr<IWindow> window(dialogMgr->GetFrontmostDialogWindow(), UseDefaultIID());
		InterfacePtr<IPanelControlData> panelCtrlData(window,UseDefaultIID());
		IControlView *iCtrlView = panelCtrlData->FindWidget(kCancelButton_WidgetID);
		InterfacePtr<ISubject> iiSubject(iCtrlView, IID_ISUBJECT);
		if(iiSubject)
			iiSubject->Change(kTrueStateMessage,IID_IBOOLEANCONTROLDATA);

		return -1;
	}while(kFalse);
}

const char* XLGAutoClosingDialog::TaskName(){
	return kXLGAutoClosingDialogKey;
}
