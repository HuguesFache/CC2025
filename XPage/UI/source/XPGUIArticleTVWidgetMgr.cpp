
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IControlView.h"
#include "IImportProvider.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "ITreeViewHierarchyAdapter.h"
#include "ITextValue.h"

// General includes:
#include "CTreeViewWidgetMgr.h"
#include "CreateObject.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"
#include "SysControlIds.h"
#include "OpenPlaceID.h"
#include "IColorData.h"
#include "ITip.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGID.h"
#include "XMLDefs.h"
#include "XPGUIArticleNodeID.h"
#include "CAlert.h"


class XPGUIArticleTVWidgetMgr: public CTreeViewWidgetMgr
{
public:

	XPGUIArticleTVWidgetMgr(IPMUnknown* boss);
	virtual ~XPGUIArticleTVWidgetMgr() {}

	virtual	IControlView* CreateWidgetForNode(const NodeID& node) const;
	
	virtual	WidgetID GetWidgetTypeForNode(const NodeID& node) const;

	virtual	bool16 ApplyNodeIDToWidget(	const NodeID& node, IControlView* widget, int32 message = 0) const;

	virtual PMReal GetIndentForNode(const NodeID& node) const;

private:
	void indent(const NodeID& node, IControlView* widget, IControlView* staticTextWidget) const;
};	

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its ImplementationID 
 making the C++ code callable by the application.
*/
CREATE_PMINTERFACE(XPGUIArticleTVWidgetMgr, kXPGUIArticleTVWidgetMgrImpl)

/* Constructor
*/
XPGUIArticleTVWidgetMgr::XPGUIArticleTVWidgetMgr(IPMUnknown* boss) :
	CTreeViewWidgetMgr(boss)
{
}


/* CreateWidgetForNode
*/
IControlView* XPGUIArticleTVWidgetMgr::CreateWidgetForNode(const NodeID& node) const{
	IControlView* retval= (IControlView*) ::CreateObject(
		::GetDataBase(this),
		RsrcSpec(LocaleSetting::GetLocale(), 
		kXPGUIPluginID, 
		kViewRsrcType, 
		kXPGUIArticleNodeWidgetRsrcID),
		IID_ICONTROLVIEW);
	ASSERT(retval);
	return retval;
}



/* GetWidgetTypeForNode
*/
WidgetID XPGUIArticleTVWidgetMgr::GetWidgetTypeForNode(
	const NodeID& node) const
{
	return kXPGUIArticleNodeWidgetId;
}


/* ApplyNodeIDToWidget
*/
bool16 XPGUIArticleTVWidgetMgr::ApplyNodeIDToWidget(const NodeID& node, IControlView* widget, int32 message) const
{	
	CTreeViewWidgetMgr::ApplyNodeIDToWidget(node, widget);
	do{
		WidgetID widgetID = widget->GetWidgetID();  

		// If this node doesn't have any children, don't show expander Widget
		InterfacePtr<IPanelControlData> panelControlData(widget, UseDefaultIID());
		
		// Set expander widget to hidden state if this node has no kids
		IControlView* expanderWidget = panelControlData->FindWidget(kTreeNodeExpanderWidgetID);
	
		InterfacePtr<const ITreeViewHierarchyAdapter> adapter(this, UseDefaultIID());
	
		if (expanderWidget) {
			if (adapter->GetNumChildren(node) <= 0) 
				expanderWidget->HideView();			 
			else 
				expanderWidget->ShowView();			 
		}

		TreeNodePtr<XPGUIArticleNodeID> nodeID(node);
		if(nodeID->GetType() == XPGArticleDataNode::kCorpsNode){

			IControlView* iconView = panelControlData->FindWidget(kXPGUIArticleIconWidgetId);			
			iconView->SetRsrcPluginID(kXPGUIPluginID);
			
			if(nodeID->GetArticleData()->artType == kMEPJavaStory)	
				iconView->SetRsrcID(kXPGUIJavaIconResourceID);
			else if(nodeID->GetArticleData()->artType == kIncopyStory)
				iconView->SetRsrcID(kXPGUIInCopyFroidIconResourceID);
			
			IControlView* displayStringView = panelControlData->FindWidget(kXPGUITextSubjectWidgetID);
			InterfacePtr<ITextControlData> textControlData(displayStringView, UseDefaultIID() );

			PMString textSubject = nodeID->GetArticleData()->artSubject;
			if(textSubject.IsEmpty())  
				textSubject="Empty?";	
			textSubject.SetTranslatable(kFalse);
			textControlData->SetString(textSubject);
			
			PMString::ConversionError convError= PMString::kNoError;
			int32 decimalColor = nodeID->GetArticleData()->artCouleurStatus.GetAsNumber(&convError);        
			
			// Conversion  decimal -> RGB          
			PMReal rRed = ((decimalColor & 16711680 ) / 65536);  
			PMReal rGreen = ((decimalColor & 65280) / 256);
			PMReal rBlue = (decimalColor & 255);
			
			// Set ColorArray RGB
			ColorArray colorArray;
            colorArray.push_back(rRed / 256.0);
            colorArray.push_back(rGreen / 256.0);
            colorArray.push_back(rBlue / 256.0);
			
			// RGB color space
			int16 colorSpace = kPMCsCalRGB;

			// Set Color Data 
	        IControlView* displayColorStatusView = panelControlData->FindWidget(kXPGUITextStatusWidgetID );
			InterfacePtr<IColorData> colorControlData (displayColorStatusView, UseDefaultIID());
			colorControlData->SetColorData(colorSpace, colorArray); 

			// Set nb Photos
			if(nodeID->GetArticleData()->artNbPhotos > 0){

				InterfacePtr<ITextValue> nbPhotosValue (panelControlData->FindWidget(kXPGUITextPictureCountWidgetID),UseDefaultIID()) ;
				int32 nbPhotos = nodeID->GetArticleData()->artNbPhotos;
				nbPhotosValue->SetTextFromValue(nbPhotos);

				IControlView* pictureIconView = panelControlData->FindWidget(kXPGUITextPictureIconWidgetID);
				pictureIconView->SetRsrcPluginID(kXPGUIPluginID);
				pictureIconView->SetRsrcID(kXPGUIPictureIconResourceID);
			}

			// Set placed status
			PMString folio = nodeID->GetArticleData()->artFolio;
			if(!folio.IsEmpty()){
				convError= PMString::kNoError;
				int32 folioNumber = folio.GetAsNumber(&convError);   
				if (convError == PMString::kNoError && folioNumber > 0){
					IControlView* textIconView = panelControlData->FindWidget(kXPGUITextPlacedIconWidgetID);
					textIconView->SetRsrcPluginID(kXPGUIPluginID);
					textIconView->SetRsrcID(kXPGUITextPlacedIconResourceID);
				}
			}
			
			// Adjust indenting- this may be temporary
			this->indent(node, widget, displayStringView);
		}		
	} while(kFalse);

	return kTrue;
}


/* GetIndentForNode
*/
PMReal XPGUIArticleTVWidgetMgr::GetIndentForNode(const NodeID& node) const
{
	return 18.0;
}


/* indent
*/
void XPGUIArticleTVWidgetMgr::indent(const NodeID& node, IControlView* widget,	IControlView* staticTextWidget) const{	

	do{
		ASSERT(widget);
		//ASSERT(staticTextWidget);
		if(widget == nil || staticTextWidget == nil) {
			break;
		}
		const PMReal currentindent = this->GetIndent(node);	
		// adjust the size to fit the text 

		PMRect widgetFrame = widget->GetFrame();
		PMReal widthBefore = widgetFrame.Width();

		widgetFrame.Left() = currentindent;
		widget->SetFrame( widgetFrame );
		
		// Call window changed to force FittedStaticText to resize
		staticTextWidget->WindowChanged();
		PMRect staticTextFrame = staticTextWidget->GetFrame();
		staticTextFrame.Right(widgetFrame.Right());

		// Don't at present take account of scroll bar dimension?
		staticTextWidget->SetFrame( staticTextFrame );

	} while(kFalse);
}

//	end, File: XPGUIArticleTVWidgetMgr.cpp
