/*
//	File:	ExportUtils.cpp
//
//	Date:	21-Dec-2005
//
//	Author : Wilfried LEFEVRE
*/

#include "VCPlugInHeaders.h"

// Interface includes
#include "IDocument.h"
#include "IK2ServiceRegistry.h"
#include "IK2ServiceProvider.h"
#include "IExportProvider.h"
//#include "IEpsDocumentName.h"
#include "IEPSExportPrefsCmdData.h"
#include "IEPSExportPreferences.h"
//#include "IPageMakeUpPrefs.h"

#include "IPDFExportPrefs.h"
#include "IPageList.h"
#include "IOutputPages.h"
#include "ISysFileData.h"
#include "IUIFlagData.h"
#include "IStringData.h"
#include "IPDFExptStyleListMgr.h"
#include "IWorkspace.h"
#include "IBook.h"
#include "IBoolData.h"
#include "IBookContentMgr.h"
#include "IDataLink.h"
#include "IApplication.h"
#include "IDocumentList.h"
#include "SDKLayoutHelper.h"

// Global includes
#include "ErrorUtils.h"
#include "FileUtils.h"
#include "K2Vector.tpp"

// Project includes
#include "ExportUtils.h"
//#include "GlobalDefs.h"

#include "CAlert.h"

/*
ErrorCode ExportUtils::EpsExport(IDocument * doc,  bool16 eraseExisting, bool16 allPages, int16 startPage, int16 endPage)
{
	ICommandSequence * seq = nil;
	ErrorCode status = kFailure;
	PMString error(kErrNilInterface);

	PMString formatName("EPS", -1, PMString::kNoTranslate);
	do
	{
		InterfacePtr<IK2ServiceRegistry> k2ServiceRegistry(gSession, UseDefaultIID());
		if (k2ServiceRegistry == nil)
		{
			error = PMString(kErrNilInterface);	
			break;
		}

		// get number of kExportProviderService providers
		int32 exportProviderCount = k2ServiceRegistry->GetServiceProviderCount(kExportProviderService);

		for (int32 exportProviderIndex = 0 ; exportProviderIndex < exportProviderCount ; exportProviderIndex++)
		{
			// get the service provider boss class
			InterfacePtr<IK2ServiceProvider> k2ServiceProvider (k2ServiceRegistry->QueryNthServiceProvider(kExportProviderService, exportProviderIndex));
			if (k2ServiceProvider  == nil)
			{
				error = PMString(kErrNilInterface);	
				break;
			}

			InterfacePtr<IExportProvider> exportProvider(k2ServiceProvider, IID_IEXPORTPROVIDER);
			if (exportProvider == nil)
			{
				error = PMString(kErrNilInterface);	
				break;
			}

			// check to see if the current selection specifier can be exported by this provider
			if(exportProvider->CanExportToFile())
			{
				// check by format name too, as sometimes, CanExportThisFormat ignores the formatName
				int32 formatCount = exportProvider->CountFormats();
				for (int32 formatIndex = 0 ; formatIndex < formatCount ; formatIndex++)
				{
					PMString localFormatName = exportProvider->GetNthFormatName(formatIndex);
					if (localFormatName == formatName)
					{
						// found the right EPS export provider -> start the export process
						
						exportProviderIndex = exportProviderCount; // to go out of the loop after processing the command
						
						PMString fileName, fileExt, fileToExport;
						
						seq = CmdUtils::BeginCommandSequence();
						if(seq == nil)
						{		
							ASSERT_FAIL("Func_ET::IDCall_ET -> seq nil");
							error = PMString(kErrNilInterface);
							break;
						}

						//seq->SetUndoability(ICommand::kAutoUndoWithPrevious);
						
						// Determine EPS filename
						InterfacePtr<IEpsDocumentName> epsDocName (doc, UseDefaultIID());
						if (epsDocName == nil)
						{
							error = PMString(kErrNilInterface);	
							break;
						}
						
						fileName = epsDocName->getEpsFileName();
						fileExt = epsDocName->getEpsFileExt();
						
						if(fileName != kNullString)
							fileToExport.Append(fileName);
						else
						{
							bool16 useExportPath;
							PMString exportPath;
							InterfacePtr<IPageMakeUpPrefs> storedPrefs ((IPageMakeUpPrefs *) gSession->QueryWorkspace(IID_IPAGEMAKEUPPREFS));
							storedPrefs->getSavePrefs(&useExportPath, &exportPath);
							if(exportPath.LastIndexOfCharacter(kDirSeparatorChar) != exportPath.NumUTF16TextChars() - 1)
								exportPath += kDirSeparator;
							fileToExport.Append(exportPath);
							PMString docName;
							doc->GetName(docName);
							
							fileToExport.Append(docName);
						}

						
						fileToExport.Append(fileExt);
						
						// First, apply preferences for page range
						PMString pageRange;
						pageRange.AppendNumber(startPage); pageRange.Append("-"); pageRange.AppendNumber(endPage);
						pageRange.SetTranslatable(kFalse);
						
						InterfacePtr<ICommand> exportPrefCmd (CmdUtils::CreateCommand(kSetEPSExportPrefsCmdBoss));
						if(exportPrefCmd == nil)
						{
							error = PMString(kErrNilInterface);	
							break;
						}
						
						exportPrefCmd->SetItemList(UIDList(::GetUIDRef(doc)));
					
						InterfacePtr<IEPSExportPrefsCmdData> cmdData (exportPrefCmd, IID_IEPSSEPEXPORTPREFSCMDDATA );
						if(cmdData == nil)
						{
							error = PMString(kErrNilInterface);	
							break;
						}

						InterfacePtr<IEPSExportPreferences> docPrefs (doc, UseDefaultIID());

						cmdData->CopyPrefs(docPrefs);	
						
						if(allPages) 
							cmdData->SetExportEPSPageOption(IEPSExportPreferences::kExportAllPages);
						else
						{
							cmdData->SetExportEPSPageOption(IEPSExportPreferences::kExportRanges);
							cmdData->SetExportEPSPageRange(pageRange);
						}

						if(CmdUtils::ProcessCommand(exportPrefCmd) != kSuccess)
						{
							error = PMString(kErrCmdFailed);
							break;
						}
						
						IDFile file = FileUtils::PMStringToSysFile(fileToExport);
						IDFile dummy;
						
						if(FileUtils::GetParentDirectory(file, dummy) == kFalse) // Invalid path
						{
				       		error = PMString(kErrBadPath); 
				            break;
				        }
				        
				        bool8 fileExists = FileUtils::DoesFileExist(file);

						if(fileExists && !eraseExisting)
						{
							break;
						}
						
						// Do the export!
						exportProvider->ExportToFile(file, doc, nil, formatName, K2::kSuppressUI);

						// check error
						status = ErrorUtils::PMGetGlobalErrorCode();
						
						break;		
					}
				}
			}
		}

	} while(kFalse);

	if (seq != nil)
	{
		if (status == kSuccess)
			CmdUtils::EndCommandSequence(seq);
		else
		{
			CmdUtils::AbortCommandSequence(seq);
			ErrorUtils::PMSetGlobalErrorCode(kFailure,kFalse,&error);
		}
	}
	else
		ErrorUtils::PMSetGlobalErrorCode(kFailure,kFalse,&error);

	return status;
}
*/

ErrorCode ExportUtils::PdfExport(IDocument * doc, PMString pdfFullName, bool16 eraseExisting,
								bool16 allPages, int16 startPage, int16 endPage, UIDList pageItemList,
								UIDRef bookRef, PMString bookExportStyle)
{
	ErrorCode status = kFailure;
	PMString error;
	
	// Create a PDF Export Command
	InterfacePtr<ICommand> PDFExportCmd(nil);

	do
	{
		PMString pdfStyleName = "[High Quality Print]";
		IDataBase * db = ::GetDataBase(doc);

		if(!doc)
			break;
		
		// Determine kind of export (document or page item)
		bool16 exportItems = (!allPages && startPage == 0 && endPage == 0);

		if(exportItems)
		{
			InterfacePtr<ICommand> PDFExportCmdTmp(CmdUtils::CreateCommand(kPDFExportItemsCmdBoss));
			// Set items to export
			PDFExportCmdTmp->SetItemList(pageItemList);

			PDFExportCmd = PDFExportCmdTmp;
		}
		else
		{
			InterfacePtr<ICommand> PDFExportCmdTmp(CmdUtils::CreateCommand(kPDFExportCmdBoss));
			// Get the pages to be exported and set them on the command's output pages interface
			InterfacePtr<IPageList> pageList(doc, UseDefaultIID());

			if(allPages)
			{
				startPage = 1;
				endPage =  pageList->GetPageCount();
			}

			UIDList pageUIDList(db);
			for(int32 i= startPage-1 ; i < endPage; i++)
			{
				UID uidPage = pageList->GetNthPageUID(i);
				pageUIDList.Append(uidPage);
			}
			InterfacePtr<IOutputPages> PDFOutputPages(PDFExportCmdTmp, UseDefaultIID());
			PDFOutputPages->InitializeFrom(pageUIDList, kFalse);

			PDFExportCmd = PDFExportCmdTmp;
		}

		// Find the pdf export style UIDRef
		InterfacePtr<IWorkspace> workspace(gSession->QueryWorkspace());
		InterfacePtr<IPDFExptStyleListMgr> PDFExptStyleListMgr(workspace, IID_IPDFEXPORTSTYLELISTMGR);
		int32 nStyleIndex = PDFExptStyleListMgr->GetStyleIndexByName(pdfStyleName);

		if(nStyleIndex == -1)
		{
			error = "Style d'export PDF non trouvé";
			break;
		}

		UIDRef styleRef = PDFExptStyleListMgr->GetNthStyleRef(nStyleIndex);
		
		InterfacePtr<IPDFExportPrefs> PDFExportCmdPrefs(PDFExportCmd, UseDefaultIID());

		// Get the export preferences interface for our style and copy it into the command's interface
		InterfacePtr<IPDFExportPrefs> PDFStylePrefs(styleRef, UseDefaultIID());
		PDFExportCmdPrefs->CopyPrefs(PDFStylePrefs);

		// outputFile is the file to which the PDF will be exported - You have to set its value
		IDFile outputFile;
		if(pdfFullName == "")
			break;

		outputFile = FileUtils::PMStringToSysFile(pdfFullName);
		if(FileUtils::IsDirectory(outputFile))
		{
			break;
		}
				
		bool8 fileExists = FileUtils::DoesFileExist(outputFile);
		if(fileExists && !eraseExisting)
		{
			status = kSuccess;
			break;
		}

		InterfacePtr<ISysFileData> PDFFileData(PDFExportCmd, UseDefaultIID());
		PDFFileData->Set(outputFile);

		// No UI display
		InterfacePtr<IUIFlagData> uiFlagCmdData(PDFExportCmd, IID_IUIFLAGDATA);
		uiFlagCmdData->Set(kSuppressUI);

		bool16 showAlerts = CAlert::GetShowAlerts();
		CAlert::SetShowAlerts(kFalse);
				
		// Process the command
		status = CmdUtils::ProcessCommand(PDFExportCmd);

		CAlert::SetShowAlerts(showAlerts);
		
	} while(kFalse);

	if (status != kSuccess)
		ErrorUtils::PMSetGlobalErrorCode(kFailure,kFalse,&error);
	
	return status;
}
