/*
//	File:	XPage.cpp
//
//	Date:	21-Jul-2011
//
//  Author : Trias
//
*/

#if XPAGE

#include "VCPlugInHeaders.h"

// General includes
#include "CAlert.h"
#include "ErrorUtils.h"
#include "FileUtils.h"

// Interface includes
#include "IDocument.h"
#include "IPageList.h"
#include "ISpread.h"
#include "IMasterPage.h"
#include "IXRailPageSlugData.h"
#include "IRefPointUtils.h"
#include "IUnitOfMeasure.h"
#include "IPageList.h"
#include "IResaRedacDataList.h"
#include "IXPageUtils.h"
#include "IFormeData.h"
#include "IOverrideMasterPageItemData.h"
#include "IGeometry.h"
#include "ILayoutUtils.h"
#include "IHierarchy.h"

// Project includes

#include "XPage.h"
#include "XPGID.h"
#include "IDTime.h"
#include "ITCLParser.h"
#include "ITCLReader.h"
#include "TCLError.h"
#include "DebugClassUtils.h"
#include "TransformUtils.h"

/* Constructor
*/
Func_AF::Func_AF(ITCLParser *  parser)
{
	InterfacePtr<ITCLReader> reader(parser->QueryTCLReader());
	if(reader == nil) throw TCLError(PMString(kErrNilInterface));

	try
	{
		reader->MustEgal();
		hPos = reader->ExtractReel('P','+',0,0);
		vPos = reader->ExtractReel('P','+',0,0);
		formePath = reader->ExtractString(255);
		idResa = reader->ExtractInt(-1, 0, kMaxInt32);
	}
	// Catch TCL error while parameter reading to add parameter number to the error message
	catch(TCLError& error)
	{
		PMString parameter(kParameterKey);
		parameter.Translate(); parameter.Append(" ");
		parameter.AppendNumber(reader->GetNumParam());
		parameter.Append(" : ");

		PMString message(error.getMessage());
		message.Translate();
		message.Insert(parameter);
		error.setMessage(message);
		throw;
	}

	if(reader->IsEndOfCommand())
		IDCall_AF(parser);
	else
		throw TCLError(PMString(kErrNoEndOfCommand));
}

/* IDCall_AF
*/
void Func_AF::IDCall_AF(ITCLParser * parser)
{	
	ErrorCode status = kFailure;
	PMString error("");

	do
	{
		IDocument * doc = parser->getCurDoc();

		// Check if there is a document open
		if(doc == nil)
		{
			error = PMString(kErrNeedDoc);		
			break;
		}

		IDataBase * db = parser->getCurDataBase();

		// Check whether current page is valid
		int32 page = parser->getCurPage();
		
		InterfacePtr<IPageList> pageList (doc, UseDefaultIID());
		if(page > pageList->GetPageCount())
		{
			error = PMString(kErrNumPageTooBig);
			break;
		}
		else if (page < 1)
		{
			error = PMString(kErrNumPageTooSmall);
			break;
		}

		// Transform import position into pasteboard coordinates
		PMPoint insertPos (hPos, vPos);
		InterfacePtr<IGeometry> currentPageGeo (db, pageList->GetNthPageUID(page - 1), UseDefaultIID());
		::TransformInnerPointToPasteboard(currentPageGeo, &insertPos);

		// Get target spread
		InterfacePtr<IHierarchy> currentPageHier (currentPageGeo, UseDefaultIID());
		UIDRef targetSpread (db, currentPageHier->GetSpreadUID());

		// Process the import
		UIDList formeItems(db);
		if(Utils<IXPageUtils>()->ImportForme(::GetUIDRef(doc), FileUtils::PMStringToSysFile(formePath), insertPos, targetSpread, IDFile(), error, &formeItems ) != kSuccess)
		{
			break;
		}

		if(idResa != -1 && !formeItems.IsEmpty())
		{
			// Get forme ID
			InterfacePtr<IFormeData> formeData (formeItems.GetRef(0), UseDefaultIID());
			PMString idForme = formeData->GetUniqueName();

			ResaRedac resa;
			resa.id = idResa;

			// Add resa to list
			InterfacePtr<IResaRedacDataList> docResaDataList (doc, UseDefaultIID());
			if(!docResaDataList)
				break;

			InterfacePtr<ICommand> applyResaCmdBoss (CmdUtils::CreateCommand(kXPGSetResaRedacDataListCmdBoss));
			applyResaCmdBoss->SetItemList(UIDList(docResaDataList));

			InterfacePtr<IResaRedacDataList> cmdData (applyResaCmdBoss, UseDefaultIID());
			cmdData->CopyFrom(docResaDataList);
			cmdData->AddResaRedac(idForme, resa);

			if(CmdUtils::ProcessCommand(applyResaCmdBoss) != kSuccess)
				break;
		}

		status = kSuccess;

	} while(kFalse);

	if(status != kSuccess)
		throw TCLError(error);
}

/* Constructor
*/
Func_AA::Func_AA(ITCLParser *  parser)
{
	InterfacePtr<ITCLReader> reader(parser->QueryTCLReader());
	if(reader == nil) throw TCLError(PMString(kErrNilInterface));

	try
	{
		reader->MustEgal();
		idResa = reader->ExtractInt(0, 0, kMaxInt32);
		inCopyAssignmentPath = reader->ExtractString(255);
		topic = reader->ExtractString(255);
		recipientName = reader->ExtractString(255);		
	}
	// Catch TCL error while parameter reading to add parameter number to the error message
	catch(TCLError& error)
	{
		PMString parameter(kParameterKey);
		parameter.Translate(); parameter.Append(" ");
		parameter.AppendNumber(reader->GetNumParam());
		parameter.Append(" : ");

		PMString message(error.getMessage());
		message.Translate();
		message.Insert(parameter);
		error.setMessage(message);
		throw;
	}

	if(reader->IsEndOfCommand())
		IDCall_AA(parser);
	else
		throw TCLError(PMString(kErrNoEndOfCommand));
}

/* IDCall_AA
*/
void Func_AA::IDCall_AA(ITCLParser * parser)
{	
	ErrorCode status = kFailure;
	PMString error("");

	do
	{
		IDocument * doc = parser->getCurDoc();

		// Check if there is a document open
		if(doc == nil)
		{
			error = PMString(kErrNeedDoc);		
			break;
		}

		IDataBase * db = parser->getCurDataBase();

		// Check whether current page is valid
		int32 page = parser->getCurPage();
		
		InterfacePtr<IPageList> pageList (doc, UseDefaultIID());
		if(page > pageList->GetPageCount())
		{
			error = PMString(kErrNumPageTooBig);
			break;
		}
		else if (page < 1)
		{
			error = PMString(kErrNumPageTooSmall);
			break;
		}

		// Get forme on master
		InterfacePtr<IResaRedacDataList> docResaDataList (doc, UseDefaultIID());
		PMString idForme = docResaDataList->GetIDForme(idResa);
		if(idForme == kNullString)
		{
			error = PMString(kErrInvalidResa);
			break;
		}

		InterfacePtr<IMasterPage> masterPage (db, pageList->GetNthPageUID(page - 1), UseDefaultIID());
		InterfacePtr<ISpread> masterSpread (db, masterPage->GetMasterSpreadUID(), UseDefaultIID());
		
		UIDList formeItems(db);
		if(Utils<IXPageUtils>()->GetAllFormeItemsOnSpread(idForme, masterSpread, formeItems) != kSuccess)
		{
			error = PMString(kErrInvalidResa);
			break;
		}

		if(!formeItems.IsEmpty()) // Found forme on the master,so we check consistency and override the forme items on the page
		{
			// Check whether the whole forme is laying on one page
			PMPoint formeLeftTop = Utils<IRefPointUtils>()->CalculateReferencePoint_3(IReferencePointData::kTopLeftReferencePoint, formeItems/*, nil, &Geometry::OuterStrokeBounds()*/);
			PMPoint formeRightBottom = Utils<IRefPointUtils>()->CalculateReferencePoint_3(IReferencePointData::kBottomRightReferencePoint, formeItems/*, nil, &Geometry::OuterStrokeBounds()*/);
			
			int32 formePageIndex1 = 0, formePageIndex2 = 0;
			InterfacePtr<IGeometry> pageGeo1 (masterSpread->QueryNearestPage(formeLeftTop, &formePageIndex1));
			InterfacePtr<IGeometry> pageGeo2 (masterSpread->QueryNearestPage(formeRightBottom, &formePageIndex2));
			
			if(formePageIndex1 != formePageIndex2)
			{
				error = PMString(kErrInvalidResa);
				break;
			}

			// Check whether forme items actually are on the master of the current page
			if(masterPage->GetMasterSpreadPageIndex() != formePageIndex1)
			{
				error = PMString(kErrInvalidResa);
				break;
			}
			
			// Handle shifting if overridden from a single page master in a facing pages spread (will have a shift of +/- (1/2 page width))
			UID currentPageUID = ::GetUID(masterPage);
			PageType currentPageType = pageList->GetPageType(currentPageUID);

			PMMatrix shift;
			if(currentPageType != kUnisexPage && masterSpread->GetNumPages() == 1)
			{
				PMReal pageWidth = pageGeo1->GetStrokeBoundingBox().Width();
				if(currentPageType == kLeftPage)
					shift.Translate(-pageWidth/2, 0);
				else
					shift.Translate(pageWidth/2, 0);
			}

			// Override forme items

			UIDList pageUIDList(db);
			PMMatrixCollection shiftList;
			for(int32 i = 0 ; i < formeItems.Length() ; ++i)
			{
				pageUIDList.Append(currentPageUID);
				shiftList.push_back(shift);
			}

			InterfacePtr<ICommand> overrideItemCmd (CmdUtils::CreateCommand(kOverrideMasterPageItemCmdBoss));

			InterfacePtr<IOverrideMasterPageItemData> cmdData (overrideItemCmd, IID_IOVERRIDEMASTERPAGEITEMDATA );
			cmdData->SetOverrideMasterPageItemData(formeItems, pageUIDList, shiftList);

			if(CmdUtils::ProcessCommand(overrideItemCmd) != kSuccess)
			{
				error = PMString(kErrInvalidResa);
				break;		
			}
			
			// Update forme items
			formeItems.Clear();
			for(int32 i = 0 ; i < cmdData->GetOverrideCount() ; ++i)
			{
				UIDRef overridenItem, dummy1, dummy2;
				PMMatrix dummyShift;
				cmdData->GetNthOverride(i, &dummy1, &dummy2, &overridenItem, &dummyShift);

				formeItems.Append(overridenItem);
			}	
		}
		else
		{
			// There is no such forme on master, try finding it directly on the current page
			InterfacePtr<IHierarchy> currentPageHier (db, pageList->GetNthPageUID(page - 1), UseDefaultIID());
			InterfacePtr<ISpread> currentSpread (db, currentPageHier->GetSpreadUID(), UseDefaultIID());

			if(Utils<IXPageUtils>()->GetAllFormeItemsOnSpread(idForme, currentSpread, formeItems) != kSuccess)
			{
				error = PMString(kErrInvalidResa);
				break;
			}

			if(formeItems.IsEmpty())
			{
				error = PMString(kErrInvalidResa);
				break;
			}

			// Check whether forme is actually on the current page
			PMPoint formeLeftTop = Utils<IRefPointUtils>()->CalculateReferencePoint_3(IReferencePointData::kTopLeftReferencePoint , formeItems/*, nil, &Geometry::OuterStrokeBounds()*/);
			
			int32 formePageIndex = 0;
			InterfacePtr<IGeometry> pageGeo (currentSpread->QueryNearestPage(formeLeftTop, &formePageIndex));
			
			if(currentSpread->GetPageIndex(::GetUID(currentPageHier)) != formePageIndex)
			{
				error = PMString(kErrInvalidResa);
				break;
			}
		}

		// Create InCopy article
		status = Utils<IXPageUtils>()->DoNewAssignment(formeItems, FileUtils::PMStringToSysFile(inCopyAssignmentPath), topic, recipientName, kTrue);

	} while(kFalse);

	if(status != kSuccess)
		throw TCLError(error);
}


/* Constructor
*/
Func_AM::Func_AM(ITCLParser *  parser)
{
	InterfacePtr<ITCLReader> reader(parser->QueryTCLReader());
	if(reader == nil) throw TCLError(PMString(kErrNilInterface));

	try
	{
		reader->MustEgal();
		idStory = reader->ExtractString(255);
		typeMarbre = reader->ExtractInt(0, 0, 1);
	}
	// Catch TCL error while parameter reading to add parameter number to the error message
	catch(TCLError& error)
	{
		PMString parameter(kParameterKey);
		parameter.Translate(); parameter.Append(" ");
		parameter.AppendNumber(reader->GetNumParam());
		parameter.Append(" : ");

		PMString message(error.getMessage());
		message.Translate();
		message.Insert(parameter);
		error.setMessage(message);
		throw;
	}

	if(reader->IsEndOfCommand())
		IDCall_AM(parser);
	else
		throw TCLError(PMString(kErrNoEndOfCommand));
}


/* IDCall_AM
*/
void Func_AM::IDCall_AM(ITCLParser * parser)
{	
	ErrorCode status = kFailure;
	PMString error("");

	do
	{
		IDocument * doc = parser->getCurDoc();

		// Check if there is a document open
		if(doc == nil)
		{
			error = PMString(kErrNeedDoc);		
			break;
		}

		// Move article to marbre
		status = Utils<IXPageUtils>()->DoStoryToMarbre(::GetUIDRef(doc), idStory, typeMarbre);
		if(status != kSuccess)
		{
			// Log it but don't break processing
			PMString msg(kPrsAMCommandFailed);
			msg.Translate();
			parser->WriteLogFile(msg);

			status = kSuccess;
		}

	} while(kFalse);

	if(status != kSuccess)
		throw TCLError(error);
}


/* Constructor
*/
Func_AB::Func_AB(ITCLParser *  parser)
{
	InterfacePtr<ITCLReader> reader(parser->QueryTCLReader());
	if(reader == nil) throw TCLError(PMString(kErrNilInterface));

	try
	{
		reader->MustEgal();
		hPos = reader->ExtractReel('P','+',0,0);
		vPos = reader->ExtractReel('P','+',0,0);
		assemblyPath = reader->ExtractString(255);

		while (!reader->IsEndOfCommand())
		{
			PMString idForme = reader->ExtractString(255);
			int32 idResa = reader->ExtractInt(-1, 0, kMaxInt32);
			resas.push_back(KeyValuePair<PMString, int32>(idForme, idResa));
		}
	}
	// Catch TCL error while parameter reading to add parameter number to the error message
	catch(TCLError& error)
	{
		PMString parameter(kParameterKey);
		parameter.Translate(); parameter.Append(" ");
		parameter.AppendNumber(reader->GetNumParam());
		parameter.Append(" : ");

		PMString message(error.getMessage());
		message.Translate();
		message.Insert(parameter);
		error.setMessage(message);
		throw;
	}

	if(reader->IsEndOfCommand())
		IDCall_AB(parser);
	else
		throw TCLError(PMString(kErrNoEndOfCommand));
}

/* IDCall_AB
*/
void Func_AB::IDCall_AB(ITCLParser * parser)
{	
	ErrorCode status = kFailure;
	PMString error("");

	do
	{
		IDocument * doc = parser->getCurDoc();

		// Check if there is a document open
		if(doc == nil)
		{
			error = PMString(kErrNeedDoc);		
			break;
		}

		IDataBase * db = parser->getCurDataBase();

		// Check whether current page is valid
		int32 page = parser->getCurPage();
		
		InterfacePtr<IPageList> pageList (doc, UseDefaultIID());
		if(page > pageList->GetPageCount())
		{
			error = PMString(kErrNumPageTooBig);
			break;
		}
		else if (page < 1)
		{
			error = PMString(kErrNumPageTooSmall);
			break;
		}

		// Transform import position into pasteboard coordinates
		PMPoint insertPos (hPos, vPos);
		InterfacePtr<IGeometry> currentPageGeo (db, pageList->GetNthPageUID(page - 1), UseDefaultIID());
		::TransformInnerPointToPasteboard(currentPageGeo, &insertPos);

		// Get target spread
		InterfacePtr<IHierarchy> currentPageHier (currentPageGeo, UseDefaultIID());
		UIDRef targetSpread (db, currentPageHier->GetSpreadUID());

		// Process the import
		UIDList assemblageItems(db);
		if(Utils<IXPageUtils>()->ImportForme(::GetUIDRef(doc), FileUtils::PMStringToSysFile(assemblyPath), insertPos, targetSpread, IDFile(), error, &assemblageItems ) != kSuccess)
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}
			
		status = kSuccess;

		if(!resas.empty())
		{
			K2Vector<PMString> formeIDConvertedToResas;
			for(int32 i = 0 ; i < assemblageItems.Length() && status == kSuccess ; ++i)
			{
				InterfacePtr<IFormeData> formeData (assemblageItems.GetRef(i), UseDefaultIID());
				PMString newIDForme = formeData->GetUniqueName(); // Import process appended a timestamp

				PMString idFormeWithoutTimeStamp = newIDForme;
				int32 timeStampPos = idFormeWithoutTimeStamp.LastIndexOfCharacter(PlatformChar('_'));
				if(timeStampPos != -1)
					idFormeWithoutTimeStamp.Remove(timeStampPos, idFormeWithoutTimeStamp.NumUTF16TextChars()-timeStampPos); 				
			
				int32 pos = ::FindLocation(resas, idFormeWithoutTimeStamp);
				if(pos == -1)
				{
					error = PMString(kErrInvalidFormeID);
					status = kFailure;
					break;
				}

				if(K2find(formeIDConvertedToResas.begin(), formeIDConvertedToResas.end(), idFormeWithoutTimeStamp) == formeIDConvertedToResas.end())
				{
					// Store resa ID in document
					ResaRedac resa;
					resa.id = resas[pos].Value();

					InterfacePtr<IResaRedacDataList> docResaDataList (doc, UseDefaultIID());
					if(!docResaDataList)
						break;

					InterfacePtr<ICommand> applyResaCmdBoss (CmdUtils::CreateCommand(kXPGSetResaRedacDataListCmdBoss));
					applyResaCmdBoss->SetItemList(UIDList(docResaDataList));

					InterfacePtr<IResaRedacDataList> cmdData (applyResaCmdBoss, UseDefaultIID());
					cmdData->CopyFrom(docResaDataList);
					cmdData->AddResaRedac(newIDForme, resa); // Associate new forme ID for resa to work well

					if(CmdUtils::ProcessCommand(applyResaCmdBoss) != kSuccess)
					{
						error = ErrorUtils::PMGetGlobalErrorString();
						status = kFailure;
						break;
					}

					formeIDConvertedToResas.push_back(idFormeWithoutTimeStamp);
				}
			}
		}

	} while(kFalse);

	if(status != kSuccess)
		throw TCLError(error);
}

#endif // XPAGE