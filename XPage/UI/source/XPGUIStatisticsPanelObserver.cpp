#include "VCPluginHeaders.h"

// Interface includes

#include "SelectionObserver.h"
#include "IPanelControlData.h"
#include "IControlView.h"
#include "ISubject.h"
#include "ISelectionUtils.h"
#include "ITextControlData.h"
#include "IAssignmentUtils.h"
#include "ITextModel.h"
#include "TextEditorModelID.h"
#include "ICommandSequence.h"
#include "IBoolData.h"
#include "ICommand.h"
#include "IFrameList.h"
#include "ITextUtils.h"
#include "ITextStrand.h"
#include "TextIterator.h"
#include "IActiveContext.h"
#include "IDocument.h"
#include "IDocumentBroadcastMsg.h"
#include "IWaxStrand.h"
#include "IWaxIterator.h"
#include "IWaxLine.h"
#include "SpellPanelID.h"
#include "ITextAttributeSuite.h"
#include "CObserver.h"
#include "IGraphicAttributeSuite.h"
#include "ITool.h"
#include "Utils.h"
#include "IToolBoxUtils.h"
#include "ITextFrameColumn.h"
#include "IXPageUtils.h"

// Project includes
#include "XPGUIID.h"
#include "CAlert.h"
#include "IXPageSuite.h" 
#include "AppUIID.h"



class XPGUIStatisticsPanelObserver : public ActiveSelectionObserver {
	
    public:		
		XPGUIStatisticsPanelObserver( IPMUnknown* boss );
		virtual ~XPGUIStatisticsPanelObserver() ;
 
	protected :
		virtual void  HandleSelectionChanged(const ISelectionMessage * message);
	
    private:
		void UpdatePanel();
		
   
};
CREATE_PMINTERFACE(XPGUIStatisticsPanelObserver, kXPGUIStatisticsPanelObserverImpl)

XPGUIStatisticsPanelObserver::XPGUIStatisticsPanelObserver(IPMUnknown*  boss)
							 :ActiveSelectionObserver(boss){}

XPGUIStatisticsPanelObserver::~XPGUIStatisticsPanelObserver(){}

void XPGUIStatisticsPanelObserver::HandleSelectionChanged(const ISelectionMessage* message)
{	

	InterfacePtr<ITool> activeTool(Utils<IToolBoxUtils>()->QueryActiveTool());
	if(activeTool && !activeTool->IsTextTool())
		this->UpdatePanel();
	
}


void XPGUIStatisticsPanelObserver::UpdatePanel(){

	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	ICommandSequence* sequ = nil;
	ErrorCode status = kFailure;

	do{
		InterfacePtr<ISelectionManager> selectionMgr (Utils<ISelectionUtils>()->QueryActiveSelection ());					
		InterfacePtr<IXPageSuite> xpgSuite (selectionMgr, IID_IXPAGESUITE);
		if(xpgSuite == nil) 
			break ;
		
		IDataBase * db = selectionMgr->GetDataBase();
        if(db == nil)
            break;		
     			
		UIDList txtFrames, graphicFrames, selectedItems;
		xpgSuite->GetSelectedItems(txtFrames, graphicFrames, selectedItems);

		int32 nbCharcters = 0, nbLines = 0, nbParagraphs = 0, nbWords = 0 ; 
		Utils<IXPageUtils>()->GetStatisticsText(txtFrames, nbCharcters, nbParagraphs, nbWords, nbLines);
	
		PMString stringNbCharacters = kNullString;  
		PMString stringNbWords = kNullString;  		
		PMString stringNbLines = kNullString;  		
		PMString stringNbParagraphs = kNullString; 		

		if(nbCharcters == 0 ){
			stringNbCharacters.Append("?");		  
			stringNbWords.Append("?");			
			stringNbLines.Append("?");			
			stringNbParagraphs.Append("?");				
		}
		else {
			stringNbCharacters.AppendNumber(nbCharcters);
			stringNbWords.AppendNumber(nbWords);
			stringNbLines.AppendNumber(nbLines);	
			stringNbParagraphs.AppendNumber(nbParagraphs);
		}

		stringNbCharacters.SetTranslatable(kFalse);
		stringNbWords.SetTranslatable(kFalse);
		stringNbLines.SetTranslatable(kFalse);
		stringNbParagraphs.SetTranslatable(kFalse);	

		InterfacePtr<ITextControlData> charactersControlData(panelControlData->FindWidget(kXPGUIStatisticsNbCharactersValueWidgetID), UseDefaultIID());
		charactersControlData->SetString(stringNbCharacters);
		
		InterfacePtr<ITextControlData> wordsControlData(panelControlData->FindWidget(kXPGUIStatisticsNbWordsValueWidgetID), UseDefaultIID());
		wordsControlData->SetString(stringNbWords);
		
		InterfacePtr<ITextControlData> linesControlData(panelControlData->FindWidget(kXPGUIStatisticsNbLinesValueWidgetID), UseDefaultIID());
		linesControlData->SetString(stringNbLines);
		
		InterfacePtr<ITextControlData> paragraphsControlData ( panelControlData->FindWidget(kXPGUIStatisticsNbParagraphsValueWidgetID), UseDefaultIID());
		paragraphsControlData->SetString(stringNbParagraphs);
		
	}while(kFalse);
}
