#include "VCPlugInHeaders.h"

// Interface includes:
#include "ISAXAttributes.h"
#include "ISession.h"
#include "ISAXServices.h"
#include "IXMLServicesError.h"
#include "IPointerData.h"
#include "IStringListData.h"

// General includes:
#include "FileUtils.h"
#include "CSAXContentHandler.h"
#include "CPMUnknown.h"
#include "CAlert.h"
#include "XPGImageDataNode.h"

// Project includes:
#include "XPGID.h"

class XPGXMLImageDescriptionHandler : public CSAXContentHandler 
{ 
public: 
	
	/** Constructor
	*/	
	XPGXMLImageDescriptionHandler(IPMUnknown *boss); 
	
	/** Destructor
	*/
	virtual ~XPGXMLImageDescriptionHandler(); 

	virtual void Register(ISAXServices* saxServices, IPMUnknown *importer);

	virtual bool16 HandlesSubElements() const;

	virtual void Characters(const WideString& chars);

	virtual void StartElement(const WideString& uri, const WideString& localname, const WideString& qname, ISAXAttributes* attrs);

	virtual void EndElement(const WideString& uri, const WideString& localname, const WideString& qname);

private:

	// Tag/attribute string
	static const WideString topLevelElement;
	static const WideString legendElement;
	static const WideString subjectElement;
	static const WideString articleIdElement;
	static const WideString idAttr;
	static const WideString typeElement;
	static const WideString hrElement, hrNBElement;
	static const WideString creditElement;
	
	// Current tag
	WideString currentElt ;

	// Ids
	PMString imageId, articleId;

	// Content variables
	PMString subject, legend, credit, type, hr, hrNB;
}; 

CREATE_PMINTERFACE(XPGXMLImageDescriptionHandler, kXPGXMLImageDescriptionHandlerImpl) 

const WideString XPGXMLImageDescriptionHandler::topLevelElement = WideString("Objet");
const WideString XPGXMLImageDescriptionHandler::legendElement = WideString("Legende");
const WideString XPGXMLImageDescriptionHandler::creditElement = WideString("Credit");
const WideString XPGXMLImageDescriptionHandler::subjectElement = WideString("Nom");
const WideString XPGXMLImageDescriptionHandler::articleIdElement = WideString("Article");
const WideString XPGXMLImageDescriptionHandler::typeElement = WideString("Type");
const WideString XPGXMLImageDescriptionHandler::idAttr = WideString("id");
#ifdef WINDOWS
	const WideString XPGXMLImageDescriptionHandler::hrElement = WideString("CheminHR_PC");
	const WideString XPGXMLImageDescriptionHandler::hrNBElement = WideString("CheminHR_NB_PC");
#else
	const WideString XPGXMLImageDescriptionHandler::hrElement   = WideString("CheminHR");
	const WideString XPGXMLImageDescriptionHandler::hrNBElement = WideString("CheminHR_NB");
#endif


XPGXMLImageDescriptionHandler::XPGXMLImageDescriptionHandler(IPMUnknown *boss): CSAXContentHandler(boss)
{ 
	 subject = legend = credit = type = articleId = imageId = hr = hrNB = kNullString;
	 currentElt = WideString();
} 

XPGXMLImageDescriptionHandler::~XPGXMLImageDescriptionHandler() {} 

void XPGXMLImageDescriptionHandler::Register(ISAXServices* saxServices, IPMUnknown *importer){
	do {
		ASSERT(saxServices);
		if(!saxServices) {
			break;
		}
		saxServices->RegisterElementHandler(WideString(""), topLevelElement, this);

	} while(kFalse);
}

bool16 XPGXMLImageDescriptionHandler::HandlesSubElements( ) const{
	// Yes, we have registered for the ktop-level element and
	// we will handle all its dependents
	return kTrue;
}

void XPGXMLImageDescriptionHandler::StartElement(const WideString& uri, const WideString& localname, const WideString& qname, ISAXAttributes* attrs) { 
	
	currentElt = localname;	
	if(localname.compare(topLevelElement) == 0){
		// Get article id from attribute
		if(HasAttribute(attrs, idAttr)){
			imageId = GetAttributeString(attrs, idAttr);
		}
	}
	else if(localname.compare(articleIdElement) == 0){
		// Get article id from attribute
		if(HasAttribute(attrs, idAttr)){
			articleId = GetAttributeString(attrs, idAttr);
		}
	}	
	else if(localname.compare(legendElement) == 0){
		// Get article id from attribute
		if(HasAttribute(attrs, idAttr)){
			legend = GetAttributeString(attrs, idAttr);
		}
	}	
	else if(localname.compare(creditElement) == 0){
		// Get article id from attribute
		if(HasAttribute(attrs, idAttr)){
			credit = GetAttributeString(attrs, idAttr);
		}
	}	
} 

void XPGXMLImageDescriptionHandler::EndElement(const WideString& uri, const WideString& localname, const WideString& qname) {
	
	if(localname.compare(topLevelElement) == 0){	

		InterfacePtr<IPointerData> dataNode (this, IID_INODEDATA);
		XPGImageDataNode * theNode = (XPGImageDataNode *) dataNode->Get();
		
		// Set image low res file path (determined from image xml file path, not from an xml tag)
		IDFile imageXMLFile = FileUtils::PMStringToSysFile(theNode->GetID()); // Get back image XML file path from node ID property
		IDFile brFile = imageXMLFile;
		brFile.SetFileName(imageId + ".BR.jpg");

		// Set node text data
		theNode->SetID(imageId); // finally node ID is set to image ID
		theNode->SetData(hr);
		theNode->SetImageData(subject, articleId, legend, credit, type, FileUtils::SysFileToPMString(brFile), hrNB);
    }
	currentElt.Clear();
}

void XPGXMLImageDescriptionHandler::Characters(const WideString & chars) {

	if(currentElt.compare(subjectElement) == 0)
		subject += chars;
	else if(currentElt.compare(legendElement) == 0)
		legend += chars;
	else if(currentElt.compare(typeElement) == 0)
		type += chars;
	else if(currentElt.compare(hrElement) == 0)
		hr += chars;
	else if(currentElt.compare(hrNBElement) == 0)
		hrNB += chars;
	else if(currentElt.compare(creditElement) == 0)
		credit += chars;
} 
