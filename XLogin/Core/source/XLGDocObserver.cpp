#include "VCPlugInHeaders.h"

// Interface includes:
#include "IDocument.h"
#include "IHierarchy.h"
#include "ISubject.h"

// API includes:
#include "CAlert.h"
#include "CmdUtils.h"
#include "CObserver.h"
#include "PageItemScrapID.h" // for kDeletePageItemCmdBoss
#include "StandoffID.h" // for kDeleteStandOffItemCmdBoss and kDeleteInsetPageItemCmdBoss
#include "UIDList.h"

// Project includes:
#include "XLGID.h"


/** This observer observes the front document's IID_IHIERARCHY_DOCUMENT protocol, so when a page 
	item is deleted from the document, this observer will be notified.  This is needed because we want 
	to be notified _Before_ the page item is about to be deleted so this implementation can have a chance
	to delete the instances of persistent list items associated with the said page item, 
	otherwise if we wait until SelectionAttributeChanged of my CSB gets called, it's too late; 
  	The page item will have been deleted from the hierarchy, and all of instances of persistent list items
	will be left dangling somewhere in memory (= boss leaks)!  This observer gets attached
	with the ISubject of kDocBoss when a document is opened or created, through the use 
  	of a open/new document responder, and this observer is detached when a document is closed, through the
	use of a close document responder.
  
	@ingroup persistentlist
	@see PstLstDocResponder
*/
class XLGDocObserver : public CObserver
{
public:
	/**	Constructor.
		@param boss IN interface ptr from boss object on which this interface is aggregated.
	*/
	XLGDocObserver(IPMUnknown* boss);

	/**	Destructor.
	*/
	virtual ~XLGDocObserver();

	/** Called by the application to allow the observer to attach to the 
	 * 	subjects to be observed (kDocBoss)
	*/
	void AutoAttach();

	/** Called by the application to allow the observer to detach from the 
	 * 	subjects being observed.
	*/
	void AutoDetach();

	/** Update is called for all registered observers, and is the method 
	 * 	through which changes are broadcast. 
	 * 	In this case, the only things we are interested in are if the drop 
	 * 	down list selection has changed and if a page item is deleted from 
	 * 	the document. 
	 * 
	 * 	@param theChange IN specifies the class ID of the change to the subject. 
	 * 		Frequently this is a command ID.
	 * 	@param theSubject IN points to the ISubject interface for the subject 
	 * 		that has changed.
	 * 	@param protocol IN specifies the ID of the changed interface on the 
	 * 		subject boss.
	 * 	@param changedBy IN points to additional data about the change. 
	 * 		Often this pointer indicates the class ID of the command 
	 * 		that has caused the change.
	*/
	virtual void Update(const ClassID& theChange, 
						ISubject* theSubject, 
						const PMIID& protocol, 
						void* changedBy);

protected:
	void AttachDocument(IDocument* iDocument);
	void DetachDocument(IDocument* iDocument);
};

CREATE_PMINTERFACE(XLGDocObserver, kXLGDocObserverImpl)

XLGDocObserver::XLGDocObserver(IPMUnknown* boss) 
	: CObserver(boss, IID_IXLGDOCOBSERVER)
{}

/* XLGDocObserver Destructor
*/
XLGDocObserver::~XLGDocObserver()	
{
}


/*	XLGDocObserver::AutoAttach
*/
void XLGDocObserver::AutoAttach()
{
	CObserver::AutoAttach();

	InterfacePtr<IDocument> iDocument(this, UseDefaultIID());
	if (iDocument != nil)
		this->AttachDocument(iDocument);
}


/*	XLGDocObserver::AutoDetach
*/
void XLGDocObserver::AutoDetach()
{
	CObserver::AutoDetach();

	InterfacePtr<IDocument> iDocument(this, UseDefaultIID());
	if (iDocument != nil)
		this->DetachDocument(iDocument);
}

/*	XLGDocObserver::Update
*/
void XLGDocObserver::Update(const ClassID& theChange, 
							   ISubject* theSubject, 
							   const PMIID& protocol, 
							   void* changedBy)
{
	do
	{
		ICommand* iCommand = (ICommand*)changedBy;
		const UIDList itemList = iCommand->GetItemListReference();


		PMString commandName;
		iCommand->GetName(&commandName);
		commandName.SetTranslatable(kTrue);
		CAlert::InformationAlert("XLGDocObserver : " + commandName);	


	
	} while (kFalse);
}

/*	XLGDocObserver::AttachDocument
*/


void XLGDocObserver::AttachDocument(IDocument* iDocument)
{
	do
	{
		if (iDocument == nil)
		{
			ASSERT_FAIL("no document to attach to");
			break;
		}

		InterfacePtr<ISubject> iDocSubject(iDocument, UseDefaultIID());
		if (iDocSubject == nil)
		{
			ASSERT_FAIL("no document subject to attach the observer");
			break;
		}

		//	Protocols used for page item model changes
		if (!iDocSubject->IsAttached(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IXLGDOCOBSERVER))
		{
			iDocSubject->AttachObserver(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IXLGDOCOBSERVER);
		}
	} while (kFalse);
}


/*	XLGDocObserver::DetachDocument
*/
void XLGDocObserver::DetachDocument(IDocument* iDocument)
{
	do
	{
		if (iDocument == nil)
		{
			ASSERT_FAIL("no document to attach to");
			break;
		}

		InterfacePtr<ISubject>      iDocSubject(iDocument, UseDefaultIID());
		if (iDocSubject == nil)
		{
			ASSERT_FAIL("no document subject to attach the observer");
			break;
		}

		if (iDocSubject->IsAttached(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IXLGDOCOBSERVER))
		{
			iDocSubject->DetachObserver(ISubject::kRegularAttachment,this, IID_IHIERARCHY_DOCUMENT, IID_IXLGDOCOBSERVER);
		}
	} while (kFalse);
}


// End, XLGDocObserver.cpp.

