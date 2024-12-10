
#include "VCPlugInHeaders.h"

// Interface includes:
#include "ISubject.h"
#include "IControlView.h"
#include "IDataBase.h"
#include "IDocument.h"
#include "ISelectionUtils.h"
#include "ISelectionManager.h"
#include "IGenericTextSelectionSuite.h"
#include "IObjectModel.h"
#include "IControllingUnknown.h"
#include "ITextFocus.h"
#include "ITextFocusManager.h"
#include "ITextModel.h"
#include "IInvalHandler.h"
#include "IStoryList.h"
#include "IBoolData.h"
// General includes:
#include "NoteID.h"
#include "TextEditorID.h"
#include "TextID.h"
#include "InCopySharedID.h"
#include "CObserver.h"
#include "Utils.h"
#include "InCopyCoreID.h"
#include "OpenPlaceID.h" // kEndPlaceCmdBoss
#include "UIDList.h"
#include "RangeData.h"
#include "CAlert.h"
#include "IIdleTask.h"
// Project includes:
#include "XLGID.h"
/**
Implements the IObserver interface using the CObserver helper class 
and listens along the IID_ITEXTMODEL protocol for changes in model. 
Handles AutoAttach, AutoDetach, and Update messages.

@ingroup gotolasttextedit
*/

class XLGTextModelObserver : public CObserver
{
public:
	XLGTextModelObserver(IPMUnknown* boss);
	~XLGTextModelObserver() {};
	virtual void AutoAttach();
	virtual void AutoDetach();
	virtual void Update(const ClassID& theChange, ISubject* theSubject, const PMIID& protocol, void* changedBy);

};

/*
*/
CREATE_PMINTERFACE(XLGTextModelObserver, kXLGTextModelObserverImpl)


/* Constructor
*/
XLGTextModelObserver::XLGTextModelObserver(IPMUnknown* boss)
			   :CObserver(boss){}


/* AutoAttach
*/
void XLGTextModelObserver::AutoAttach(){
	do{
		InterfacePtr<ISubject> subject(this, IID_ISUBJECT);
		if (subject != nil){
			if (subject->IsAttached(ISubject::kRegularAttachment,this, IID_ITEXTMODEL,IID_IXLGTEXTMODELOBSERVER) == kFalse)
				subject->AttachObserver(ISubject::kRegularAttachment,this, IID_ITEXTMODEL,IID_IXLGTEXTMODELOBSERVER);
		}
	} while(kFalse);
}


/* AutoDetach
*/
void XLGTextModelObserver::AutoDetach(){
	do{
		InterfacePtr<ISubject> subject(this, UseDefaultIID());
		if (subject != nil){
			if (subject->IsAttached(ISubject::kRegularAttachment,this, IID_ITEXTMODEL,IID_IXLGTEXTMODELOBSERVER) == kTrue)
				subject->DetachObserver(ISubject::kRegularAttachment,this, IID_ITEXTMODEL,IID_IXLGTEXTMODELOBSERVER);
		}

	} while(kFalse);
}


/* Update
*/
void XLGTextModelObserver::Update(const ClassID& theChange, ISubject* theSubject, 
							const PMIID& protocol, void* changedBy)
{
	ICommand* command = (ICommand*)changedBy;	
	
	PMString commandName;
	command->GetName(&commandName);
	commandName.SetTranslatable(kTrue);
	InterfacePtr<IBoolData> disableCloseDoc  (GetExecutionContextSession(), IID_IDISABLECLOSEDOC);
	InterfacePtr<IIdleTask> initVariableTask (GetExecutionContextSession(), IID_IINITVARIABLE);			
	if( commandName == "PasteUnformatted" || commandName == "Coller sans mise en forme" )
	{		disableCloseDoc->Set(kTrue);			
			if(initVariableTask)
				initVariableTask->InstallTask(1500);		
	}
	//initVariableTask->UninstallTask();

	
}

//	End XLGTextModelObserver.cpp