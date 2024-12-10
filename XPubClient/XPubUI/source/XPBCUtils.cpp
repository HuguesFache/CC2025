/*
//
//	Author: Wilfried LEFEVRE
//
//	Date: 8-Mai-200_
//
//  File : XPBCUtils.cpp
//
*/

#include "VCPluginHeaders.h"

// Interface includes:
#include "INewDocCmdData.h"
#include "IPMLockFileFactory.h"
#include "IPMLockFile.h"
#include "IUnitOfMeasure.h"
#include "IMultiColumnItemData.h"
#include "INewPageItemCmdData.h"
#include "ISpreadList.h"
#include "ISpreadLayer.h"
#include "ISpread.h"
#include "ILayerUtils.h"
#include "IDocumentLayer.h"
#include "IStringData.h"
#include "IIntData.h"
#include "IGeometry.h"
#include "IDocument.h"
#include "IHierarchy.h"
//#include "ITextFrame.h"
#include "ITextModelCmds.h"
#include "ITextAttrRealNumber.h"
#include "ITextAttrFont.h"
#include "ITextAttrUID.h"
#include "ITextAttrAlign.h"
#include "IDocFontMgr.h"
#include "TransformUtils.h" // InnerToPasteboard
#include "IPageList.h"
#include "IPDFExportPrefs.h"
#include "IPDFExptStyleListMgr.h"
#include "IOutputPages.h"
#include "IWorkspace.h"
#include "ISysFileData.h"
#include "IUIFlagData.h"
#include "ILayerList.h"
#include "IGuidePrefs.h"
#include "IMasterSpreadList.h"
#include "IMasterSpread.h"
#include "IDataLinkReference.h"
#include "IDataLink.h"
#include "IDataLinkHelper.h"
#include "IPageItemTypeUtils.h"
#include "IRestoreLinkCmdData.h"
#include "IItemStrand.h"
#include "IUsedFontList.h"
#include "IFontFamily.h"
#include "IPMFont.h"
#include "IDataStringUtils.h"
#include "ITransformFacade.h"
#include "NameInfo.h"

// General includes:
#include "CPMUnknown.h"
#include "Utils.h"
#include "UIDList.h"
#include "LayoutUIID.h"
#include "SplineID.h"
#include "TransformUtils.h"
#include "SnapshotUtils.h"
#include "StreamUtil.h"
#include "StringUtils.h"
#include "IMultiColumnTextFrame.h"
#include "IS4SPPackage.h"
#include "PackageAndPreflightID.h"
#include "ICoreFilename.h"
#include "IDTime.h"

// Project includes:
#include "XPBCID.h"
#include "IXPubClientUtils.h"
#include "IDocumentCommands.h"
#include "FileUtils.h"
#include "SDKLayoutHelper.h"
#include "CAlert.h"

/**	XPBCUtils	
*/
class XPBCUtils : public CPMUnknown<IXPubClientUtils>
{
public:
		XPBCUtils(IPMUnknown* boss);

		virtual ~XPBCUtils();

		virtual ErrorCode OuvrirPub(const PubInfoStruct& pub);

		virtual bool16 VerifierPub(IDocument * pubDoc, PubInfoStruct& pubInfo);

		virtual ErrorCode GenererPreview(IDocument * pubDoc);

		virtual ErrorCode PdfExport(IDocument * doc, const PubInfoStruct& pubInfo);

		virtual ErrorCode RassemblerImages(IDocument * doc);
		virtual ErrorCode RassemblerPolices(IDocument * doc);
		
		virtual ErrorCode SetPersistentString(UIDRef target, PMIID interfaceID, const PMString& value); 

		// Parse les chaÓnes de caractËres contenant un nombre decimal
		virtual double GetAsDouble(const PMString& value);
	
		virtual ErrorCode PackageExport(const IDocument * doc, IDFile& folderFile);

private:
		
		IUnitOfMeasure * uom;// Converter

		// Fonctions destinees a creer le document representant la pub a fabriquer
		UIDRef CreerPub(const PubInfoStruct& pub);
		UIDRef CreerCartouche(const UIDRef& pubDoc, const PubInfoStruct& pub);
		UIDRef GetCartouche(const UIDRef& pubDoc, const PubInfoStruct& pubInfo);
		ErrorCode SetPubID(const UIDRef& txtFrameRef, const PubInfoStruct& pubInfo);

		// Fonctions utilisees lors du rassemblement
		void TraiterLiensSpread(UIDRef spreadRef, int32 startContentLayer, int32 endContentLayer, const IDFile& imageDir);
		void TraiterLienItem(IHierarchy * itemHier, const IDFile& imageDir);
		void CopierLien(UIDRef refLien, const IDFile& imageDir);		
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its 
 ImplementationID making the C++ code callable by the 
 application.
*/
CREATE_PMINTERFACE(XPBCUtils, kXPBCUtilsImpl)

/* Constructor
*/
XPBCUtils::XPBCUtils(IPMUnknown* boss) : 	
CPMUnknown<IXPubClientUtils>(boss), uom(nil)
{
	uom = (IUnitOfMeasure *) ::CreateObject(kMillimetersBoss, IID_IUNITOFMEASURE);
}

/* Destructor.
*/
XPBCUtils::~XPBCUtils()
{
	if(uom)
		uom->Release();
}

/* OuvrirPub
*/
ErrorCode XPBCUtils::OuvrirPub(const PubInfoStruct& pub)
{
	ErrorCode status = kFailure;
	do
	{		
		// On regarde si la pub existe
		IDFile pubFile = FileUtils::PMStringToSysFile(pub.Dossier);

		UIDRef pubUIDRef = UIDRef::gNull;
		if(FileUtils::DoesFileExist(pubFile))
		{
			// On verifie si la pub n'est pas deja ouverte
			InterfacePtr<IPMLockFile> lockFile (Utils<IPMLockFileFactory>()->CreatePMLockFileObject());
			lockFile->SetDocumentName(pubFile);
			if(lockFile->PMLockFileExist())
			{
				CAlert::InformationAlert(kXPBCPubAlreadyOpenStringKey);
				break;
			}

			// Ouverture du document (juste en memoire, on ouvre une fenÍtre a la fin de la fonction
			// pour ne declencher les callbacks de la palette qu'une fois que les proprietes de la pub ont ete stockees
			InterfacePtr<ICommand> openDocumentCmd(Utils<IDocumentCommands>()->CreateOpenCommand(pubFile, kSuppressUI, 
																								 IOpenFileCmdData::kOpenOriginal,
																								 IOpenFileCmdData::kUseLockFile, kFalse));

			if (CmdUtils::ProcessCommand(openDocumentCmd) != kSuccess)
			{
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::ErrorAlert(kXPBCErrorOpenStringKey);
				break;
			}

			pubUIDRef = openDocumentCmd->GetItemList()->GetRef(0);

			// On verifie que le cartouche est correct (c a d present et avec le bon id de pub)
			UIDRef cartoucheRef = GetCartouche(pubUIDRef, pub);
			if(cartoucheRef == UIDRef::gNull)
				CreerCartouche(pubUIDRef, pub);
			else
				SetPubID(cartoucheRef, pub); // On reecrit systematiquement l'ID de pub dans le cartouche
		}
		else
		{
			pubUIDRef = CreerPub(pub);
			if(pubUIDRef == UIDRef::gNull)
			{
				CAlert::ErrorAlert(kXPBCErrorCreerPubStringKey);
				break;
			}

			// On s'assure de l'existence du dossier parent
			IDFile pubFolder;
			FileUtils::GetParentDirectory(pubFile, pubFolder);
			FileUtils::CreateFolderIfNeeded(pubFolder);

			// Sauvegarde
			if(Utils<IDocumentCommands>()->SaveAs(pubUIDRef, pubFile, kSuppressUI) != kSuccess)
			{
				ErrorUtils::PMSetGlobalErrorCode(kSuccess);
				CAlert::InformationAlert(kXPBCErrorSaveStringKey);		

				// Close pub doc before leaving
				Utils<IDocumentCommands>()->Close(pubUIDRef,kSuppressUI);
				break;
			}
		}

		// On stocke de maniËre non persistente l'IDPub dans le document
		InterfacePtr<IStringData> idPubData (pubUIDRef, IID_IPUB_ID);
		idPubData->Set(pub.IDPub);

		// On stocke de maniËre persistente le chemin du fichier de previsualtion et la propriete BAT de la pub dans le document
		Utils<IXPubClientUtils>()->SetPersistentString(pubUIDRef,IID_IPUB_PREVIEW,pub.FichierPreview);
		WideString batValue;
		Utils<IDataStringUtils>()->Bool16ToString(pub.BAT, &batValue);
		Utils<IXPubClientUtils>()->SetPersistentString(pubUIDRef,IID_IPUB_BAT,PMString(batValue));

		// Ouverture d'une fenÍtre sur le document
		InterfacePtr<ICommand> newWinCmd(CmdUtils::CreateCommand(kOpenLayoutCmdBoss));
		newWinCmd->SetItemList(UIDList(pubUIDRef));
		
		status = CmdUtils::ProcessCommand(newWinCmd);
	}
	while(kFalse);

	return status;
}

UIDRef XPBCUtils::CreerPub(const PubInfoStruct& pub)
{
	UIDRef refPub = UIDRef::gNull;

	do
	{
		// Conversion des dimensions en points
		PMReal largeur = uom->UnitsToPoints(GetAsDouble(pub.Largeur));
		PMReal hauteur = uom->UnitsToPoints(GetAsDouble(pub.Hauteur));

		InterfacePtr<ICommand> newDocCmd(CmdUtils::CreateCommand(kNewDocCmdBoss));
		
		InterfacePtr<INewDocCmdData> newDocCmdData(newDocCmd, UseDefaultIID());	
		newDocCmdData->SetWideOrientation(largeur >= hauteur);

		PMRect pageSize = PMRect(0.0, 0.0, largeur, hauteur);
        newDocCmdData->SetNewDocumentPageSize(PMPageSize(pageSize));
        newDocCmdData->SetPagesPerSpread(1);
		newDocCmdData->SetNumPages(1);
		newDocCmdData->SetMargins(0,0,0,0);
		newDocCmdData->SetColumns_4(1,12,IColumns::kVerticalColumnOrientation);
		if (CmdUtils::ProcessCommand(newDocCmd) != kSuccess)	
			break;
	
		// Recuperation de la reference du document
		refPub = newDocCmd->GetItemList()->GetRef(0);

		// Creation du cartouche
		if(CreerCartouche(refPub, pub) == UIDRef::gNull)
			CAlert::InformationAlert(kXPBCErrorCreerCartoucheStringKey);

	} while(false);

	return refPub;
}

UIDRef XPBCUtils::CreerCartouche(const UIDRef& pubDoc, const PubInfoStruct& pub)
{
	UIDRef cartoucheRef = UIDRef::gNull;
	
	do
	{
		IDataBase * db = pubDoc.GetDataBase();
		InterfacePtr<IDocument> doc (pubDoc, UseDefaultIID());

		// Recuperation du spread layer du cartouche
		InterfacePtr<ISpreadList> spreadList (pubDoc, UseDefaultIID());
		InterfacePtr<ISpread> ownerSpread (db, spreadList->GetNthSpreadUID(0), UseDefaultIID());

		InterfacePtr<IDocumentLayer> activeLayer (Utils<ILayerUtils>()->QueryDocumentActiveLayer(doc));
		InterfacePtr<ISpreadLayer> ownerSpreadLayer (ownerSpread->QueryLayer(activeLayer));

		// Calcul des coordonnees en fonction des contraintes XPub
		InterfacePtr<IGeometry> pageGeo (ownerSpread->QueryNthPage(0));
		PMReal pageWidth = pageGeo->GetStrokeBoundingBox().Width();
		PMReal pageHeight = pageGeo->GetStrokeBoundingBox().Height();

		PMReal cartWidth =  uom->UnitsToPoints(GetAsDouble(pub.CadreRefLargeur));
		PMReal cartHeight = uom->UnitsToPoints(GetAsDouble(pub.CadreRefHauteur));

		PMPoint leftTop;
		PMReal angle = pub.CadreRefAngle.GetAsDouble();
		if(pub.CadreRefExterieur)
		{
			if(pub.CadreRefOrigX == "Gauche")
			{
				if(pub.CadreRefOrigY == "Haut")
				{
					leftTop = PMPoint(0,-cartHeight);
					if(angle == 90)
						leftTop = PMPoint(-cartHeight,cartWidth);					
					else if(angle == 270)
						leftTop = PMPoint(0,0);					
				}
				else // "Bas"
				{
					leftTop = PMPoint(0,pageHeight);
					if(angle == 90)
						leftTop = PMPoint(-cartHeight,pageHeight);
					else if(angle == 270)
						leftTop = PMPoint(0,pageHeight-cartWidth);
				}
			}
			else // Droit
			{
				if(pub.CadreRefOrigY == "Haut")
				{
					leftTop = PMPoint(pageWidth-cartWidth,-cartHeight);
					if(angle == 90)
						leftTop = PMPoint(pageWidth,cartWidth);					
					else if(angle == 270)
						leftTop = PMPoint(pageWidth+cartHeight,0);		
				}
				else // "Bas"
				{
					leftTop = PMPoint(pageWidth-cartWidth,pageHeight);
					if(angle == 90)
						leftTop = PMPoint(pageWidth,pageHeight);					
					else if(angle == 270)
						leftTop = PMPoint(pageWidth+cartHeight,pageHeight-cartWidth);	
				}
			}
		}
		else
		{
			if(pub.CadreRefOrigX == "Gauche")
			{
				if(pub.CadreRefOrigY == "Haut")
				{
					leftTop = PMPoint(0,0);
					if(angle == 90)
						leftTop = PMPoint(0,cartWidth);					
					else if(angle == 270)
						leftTop = PMPoint(cartHeight,0);	
				}
				else // "Bas"
				{
					leftTop = PMPoint(0,pageHeight-cartHeight);
					if(angle == 90)
						leftTop = PMPoint(0,pageHeight);					
					else if(angle == 270)
						leftTop = PMPoint(cartHeight,pageHeight-cartWidth);
				}
			}
			else // Droit
			{
				if(pub.CadreRefOrigY == "Haut")
				{
					leftTop = PMPoint(pageWidth-cartWidth,0);
					if(angle == 90)
						leftTop = PMPoint(pageWidth-cartHeight,cartWidth);					
					else if(angle == 270)
						leftTop = PMPoint(pageWidth,0);
				}
				else // "Bas"
				{
					leftTop = PMPoint(pageWidth-cartWidth,pageHeight-cartHeight);
					if(angle == 90)
						leftTop = PMPoint(pageWidth-cartHeight,pageHeight);					
					else if(angle == 270)
						leftTop = PMPoint(pageWidth,pageHeight-cartWidth);
				}
			}
		}

		// Add offsets
		leftTop.X() += uom->UnitsToPoints(GetAsDouble(pub.CadreOffsetX));
		leftTop.Y() += uom->UnitsToPoints(GetAsDouble(pub.CadreOffsetY));

		PMRect cartoucheBounds (leftTop, PMPoint(leftTop.X()+cartWidth, leftTop.Y()+cartHeight));
		::TransformInnerRectToPasteboard(pageGeo, &cartoucheBounds);

		PMPointList bounds;
		bounds.push_back(cartoucheBounds.LeftTop());
		bounds.push_back(cartoucheBounds.RightBottom());

		// Commande pour creer le bloc texte du cartouche
		InterfacePtr<ICommand> createTextFrameCmd(CmdUtils::CreateCommand(kCreateMultiColumnItemCmdBoss));
		createTextFrameCmd->SetUndoability(ICommand::kAutoUndo);	

		InterfacePtr<INewPageItemCmdData> data(createTextFrameCmd, UseDefaultIID());
		data->Set(db,kSplineItemBoss, ::GetUID(ownerSpreadLayer), bounds);	

		if(CmdUtils::ProcessCommand(createTextFrameCmd) != kSuccess)
			break;

		
		cartoucheRef = createTextFrameCmd->GetItemList()->GetRef(0); 

		// Rotation
		if(angle != 0)
		{

		
			if(Utils<Facade::ITransformFacade>()->TransformItems(UIDList(cartoucheRef), 
																 Transform::PasteboardCoordinates(), 
																 cartoucheBounds.LeftTop(), 
																 Transform::RotateTo(angle)) != kSuccess)
			{
				cartoucheRef = UIDRef::gNull;
				break;
			}
		}

		// On ecrit l'ID de pub dans le cartouche
		if(SetPubID(cartoucheRef, pub) != kSuccess)
		{
			cartoucheRef = UIDRef::gNull;
			break;
		}

		// On stocke egalement l'IDPub dans le bloc du cartouche
		Utils<IXPubClientUtils>()->SetPersistentString(cartoucheRef,IID_IPUB_ID,pub.IDPub);				
		
	} while(kFalse);

	return cartoucheRef;
}

bool16 XPBCUtils::VerifierPub(IDocument * pubDoc, PubInfoStruct& pubInfo)
{
	bool16 ok = kFalse;
	do
	{
		InterfacePtr<IStringData> idPub (pubDoc, IID_IPUB_ID);
		if(idPub->Get() == kNullString)
			break;

		// Recuperation des infos de pub sur le serveur
		InterfacePtr<IXPubConnexion> xpubConnexion (::CreateObject2<IXPubConnexion>(kXPubConnexionBoss));
		InterfacePtr<IStringData> serverAddress (GetExecutionContextSession(), IID_ISTRINGDATA_IPSERVEUR);
		xpubConnexion->SetServerAddress(serverAddress->Get());

		bool16 success = xpubConnexion->InfoPub(idPub->Get(), pubInfo);
		if(!success)
		{
			PMString error = ErrorUtils::PMGetGlobalErrorString();
			ErrorUtils::PMSetGlobalErrorCode(kSuccess);
			CAlert::ErrorAlert(error);
			break;
		}

		// Verification des dimensions de la pub

		PMReal largeur = uom->UnitsToPoints(GetAsDouble(pubInfo.Largeur));
		PMReal hauteur = uom->UnitsToPoints(GetAsDouble(pubInfo.Hauteur));

		InterfacePtr<ISpreadList> spreadList (pubDoc, UseDefaultIID());
		InterfacePtr<ISpread> firstSpread (::GetDataBase(pubDoc), spreadList->GetNthSpreadUID(0), UseDefaultIID());
		
		InterfacePtr<IGeometry> pageDim (firstSpread->QueryNthPage(0));

		if(largeur != pageDim->GetStrokeBoundingBox().Width() || hauteur != pageDim->GetStrokeBoundingBox().Height())
		{
			CAlert::InformationAlert(kXPBCErrorInconsistentDimStringKey);
#if STRICT_CHECK
			break;
#endif
		}

		// On met a jour la hauteur (utilisee pour retour vers XPub)
		pubInfo.Hauteur.Clear();
		pubInfo.Hauteur.AppendNumber(uom->PointsToUnits(pageDim->GetStrokeBoundingBox().Height()), 0,kTrue);

		// Verification de la presence du cartouche				
		if(GetCartouche(::GetUIDRef(pubDoc), pubInfo) == UIDRef::gNull)
		{
			CAlert::ErrorAlert(kXPBCErrorMissingCartoucheStringKey);
			break;
		}

		ok = kTrue;

	} while(kFalse);

	return ok;
}

ErrorCode XPBCUtils::SetPubID(const UIDRef& txtFrameRef, const PubInfoStruct& pubInfo)
{
	ErrorCode status = kFailure;
	do
	{
		IDataBase * db = txtFrameRef.GetDataBase();

		// On definit la typo du cartouche puis on y ecrit l'IDPub
		InterfacePtr<IHierarchy> cartoucheHier (txtFrameRef, UseDefaultIID());
		InterfacePtr<IMultiColumnTextFrame> txtFrame (db, cartoucheHier->GetChildUID(0), UseDefaultIID());
		InterfacePtr<ITextModel> txtModel (db, txtFrame->GetTextModelUID(), UseDefaultIID());
		InterfacePtr<ITextModelCmds> txtModelCmds (txtModel, UseDefaultIID());

		InterfacePtr<ITextAttrRealNumber> fontSize(::CreateObject2<ITextAttrRealNumber>(kTextAttrPointSizeBoss));
		fontSize->Set(pubInfo.CadreRefCorps.GetAsDouble());

		InterfacePtr<IDocument> doc (db, db->GetRootUID(), UseDefaultIID());
		InterfacePtr<IDocFontMgr> iDocFontMgr(doc->GetDocWorkSpace(), UseDefaultIID());
		PMString style;	
		UID fontUID = iDocFontMgr->GetFontFamilyUID(pubInfo.CadreRefPolice, style, kTrue);;
		if(fontUID == kInvalidUID)
		{
			fontUID = iDocFontMgr->GetDefaultFamilyUID();
			style = "Regular";
		}
		
		InterfacePtr<ITextAttrUID> fontName((ITextAttrUID*)::CreateObject(kTextAttrFontUIDBoss, IID_ITEXTATTRUID));
		fontName->SetUIDData(fontUID);

		InterfacePtr<ITextAttrFont> fontStyle ((ITextAttrFont *) ::CreateObject(kTextAttrFontStyleBoss, IID_ITEXTATTRFONT));
		fontStyle->SetFontName(style);		

		boost::shared_ptr<AttributeBossList> attrListPtr (new AttributeBossList());
		attrListPtr->ApplyAttribute(fontSize);
		attrListPtr->ApplyAttribute(fontName);
		attrListPtr->ApplyAttribute(fontStyle);
		
		InterfacePtr<ICommand> applyCharAttrCmd(txtModelCmds->ApplyCmd(0,0,attrListPtr,kCharAttrStrandBoss));
		if(CmdUtils::ProcessCommand(applyCharAttrCmd) != kSuccess)
			break;		

		InterfacePtr<ITextAttrAlign> alignment ((ITextAttrAlign *) ::CreateObject(kTextAttrAlignmentBoss, IID_ITEXTATTRALIGN));
		if(pubInfo.CadreAlign == "Gauche")
			alignment->SetAlignment(ICompositionStyle::kTextAlignLeft);		
		else if(pubInfo.CadreAlign == "Droite")
			alignment->SetAlignment(ICompositionStyle::kTextAlignRight);	
		else if(pubInfo.CadreAlign == "Centre")
			alignment->SetAlignment(ICompositionStyle::kTextAlignCenter);	
		else
			alignment->SetAlignment(ICompositionStyle::kTextAlignLeft);

		boost::shared_ptr<AttributeBossList> paraAttrListPtr (new AttributeBossList());
		paraAttrListPtr->ApplyAttribute(alignment);
		InterfacePtr<ICommand> applyParaAttrCmd(txtModelCmds->ApplyCmd(0,0,paraAttrListPtr,kParaAttrStrandBoss));
		if(CmdUtils::ProcessCommand(applyParaAttrCmd) != kSuccess)
			break;	

		boost::shared_ptr<WideString> txtIDPub (new WideString(pubInfo.IDPub));
		InterfacePtr<ICommand> insertCmd(txtModelCmds->ReplaceCmd(0,txtModel->GetPrimaryStoryThreadSpan()-1, txtIDPub));
		if(CmdUtils::ProcessCommand(insertCmd) != kSuccess)
			break;

		status = kSuccess;

	} while(kFalse);

	return status;
}

UIDRef XPBCUtils::GetCartouche(const UIDRef& pubDoc, const PubInfoStruct& pubInfo)
{
	UIDRef cartoucheRef = UIDRef::gNull;
	UIDList items(pubDoc.GetDataBase());
	
	// On parcourt tous les items du premier spread (donc de la premiËre page) a la recherche du cartouche
	InterfacePtr<ISpreadList> spreadList (pubDoc, UseDefaultIID());
	InterfacePtr<ISpread> firstSpread (pubDoc.GetDataBase(), spreadList->GetNthSpreadUID(0), UseDefaultIID());
	firstSpread->GetItemsOnPage(0, &items, kFalse, kTrue);

	int32 nbItems = items.Length();
	for(int32 i = 0 ; i < items.Length() ; ++i)
	{
		InterfacePtr<IStringData> cartoucheData (items.GetRef(i), IID_IPUB_ID);
		if(cartoucheData && cartoucheData->Get() == pubInfo.IDPub)
		{
			cartoucheRef = items.GetRef(i);
			break;
		}
	}
	return cartoucheRef;
}

ErrorCode XPBCUtils::GenererPreview(IDocument * pubDoc)
{
	ErrorCode status = kFailure;
	do
	{
		// Recuperer le chemin du fichier de previsu
		InterfacePtr<IStringData> previewPath (pubDoc, IID_IPUB_PREVIEW);
		if(!previewPath || previewPath->Get() == kNullString)
			break;

		InterfacePtr<IPageList> pageList (pubDoc, UseDefaultIID());
		UIDRef pageToExport = UIDRef(::GetDataBase(pubDoc), pageList->GetNthPageUID(0));

		// Creer un flux de sortie
		InterfacePtr<IPMStream> jpegStream(StreamUtil::CreateFileStreamWrite(FileUtils::PMStringToSysFile(previewPath->Get()), kOpenOut|kOpenTrunc, 'JPEG',  kAnyCreator));
		if (jpegStream == nil) 		
			break;		

		SnapshotUtils * snapshotUtils = new SnapshotUtils(pageToExport, 
										   1,     // X Scale
										   1,     // Y Scale
										   72,    // desired resolution of resulting snapshot
										   72,    // minimum resolution of resulting snapshot
										   0,     // Extend the bounds of the area to be drawn by the given amount of bleed
										   IShape::kPrinting,// Drawing flags (kPrinting suppresses drawing of margins and guides)
										   kTrue,    // kTrue forces images and graphics to draw at full resolution, kFalse to draw proxies
										   kFalse,    // kTrue to draw grayscale, kFalse to draw RGB
										   kFalse);    // kTrue to add an alpha channel, kFalse no alpha channel


		// Exporter en JPEG
		status = snapshotUtils->ExportImageToJPEG(jpegStream, SnapshotUtils::kSSJPEGBaseline,  SnapshotUtils::kSSJPEGGreatQuality);
		delete snapshotUtils;

	} while(kFalse);

	return status;
}

ErrorCode XPBCUtils::PdfExport(IDocument * doc, const PubInfoStruct& pubInfo)
{
	ErrorCode status = kFailure;

	do
	{
		// Get PDF style
		InterfacePtr<IStringData> pdfStyle (GetExecutionContextSession(), IID_ISTRINGDATA_STYLEPDF);
		PMString pdfStyleName = pdfStyle->Get();
		
		IDataBase * db = ::GetDataBase(doc);
		if(!doc)
			break;
		
		// Create export page command
		InterfacePtr<ICommand> PDFExportCmd(CmdUtils::CreateCommand(kPDFExportCmdBoss));
				
		// Export only one page, the first one
		InterfacePtr<IPageList> pageList(doc, UseDefaultIID());
		UIDList pageUIDList(db);
		pageUIDList.Append(pageList->GetNthPageUID(0));
		
		InterfacePtr<IOutputPages> PDFOutputPages(PDFExportCmd, UseDefaultIID());
		PDFOutputPages->InitializeFrom(pageUIDList, kFalse);

		// Find the pdf export style UIDRef
		InterfacePtr<IWorkspace> workspace(GetExecutionContextSession()->QueryWorkspace());
		InterfacePtr<IPDFExptStyleListMgr> PDFExptStyleListMgr(workspace, IID_IPDFEXPORTSTYLELISTMGR);

		int32 nStyleIndex = PDFExptStyleListMgr->GetStyleIndexByName(pdfStyleName);
		if(nStyleIndex == -1)
		{
			CAlert::ErrorAlert(kXBPCErrorPDFStyleNotFoundKey);
			break;
		}

		UIDRef styleRef = PDFExptStyleListMgr->GetNthStyleRef(nStyleIndex);
		
		InterfacePtr<IPDFExportPrefs> PDFExportCmdPrefs(PDFExportCmd, UseDefaultIID());

		// Get the export preferences interface for our style and copy it into the command's interface
		InterfacePtr<IPDFExportPrefs> PDFStylePrefs(styleRef, UseDefaultIID());
		PDFExportCmdPrefs->CopyPrefs(PDFStylePrefs);

		// Apply bleeding
		PDFExportCmdPrefs->SetPDFExBleed(IPDFExportPrefs::kExportBleedON);
		PDFExportCmdPrefs->SetPDFExBleedOutside(uom->UnitsToPoints(GetAsDouble(pubInfo.FondPerduD)));
		PDFExportCmdPrefs->SetPDFExBleedTop(uom->UnitsToPoints(GetAsDouble(pubInfo.FondPerduH)));
		PDFExportCmdPrefs->SetPDFExBleedInside(uom->UnitsToPoints(GetAsDouble(pubInfo.FondPerduG)));
		PDFExportCmdPrefs->SetPDFExBleedBottom(uom->UnitsToPoints(GetAsDouble(pubInfo.FondPerduB))); 

		// outputFile is the file to which the PDF will be exported - You have to set its value
		IDFile outputFile;
		if(pubInfo.FichierPDF == "")
			break;

		outputFile = FileUtils::PMStringToSysFile(pubInfo.FichierPDF);
		if(FileUtils::IsDirectory(outputFile))
		{
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

		// We control that the PDF has been created correctly because we get no warning
		// if it hasn't been written due to write rights not allowed for instance
		if(status != kSuccess || !FileUtils::DoesFileExist(outputFile))
		{
			status = kFailure;
			CAlert::ErrorAlert(kXPBCErrorPDFNotCreatedStringKey);
		}
		
	} while(kFalse);
	
	return status;
}

ErrorCode XPBCUtils::RassemblerImages(IDocument * doc)
{
	ErrorCode status = kFailure;

	do
	{
		if(!doc)
			break;

		IDataBase * db = ::GetDataBase(doc);
		const IDFile * docFile = db->GetSysFile();
		if(!docFile)
			break;

		// Creation du dossier d'assemblage des images
		IDFile imageDir;
		FileUtils::GetParentDirectory(*docFile, imageDir);
		FileUtils::AppendPath(&imageDir, PMString("Links"));
		FileUtils::CreateFolderIfNeeded(imageDir);

		// On parcourt l'ensemble des items pour recuperer information de liens dans les blocs image
		// C'est le seul lien moyen de retrouver les images embarquees (ne figurent pas dans l'interface ILinksManager)
		InterfacePtr<ILayerList> layerList (doc, UseDefaultIID());

		int32 nbLayers = layerList->GetCount();
		// On determine o˘ se trouvent les spreadLayer contenant les items	
		int32 startContentLayer, endContentLayer;
		InterfacePtr<IGuidePrefs> guidePrefs (doc->GetDocWorkSpace(), UseDefaultIID());
		if(guidePrefs && guidePrefs->GetGuidesInBack())
		{			
			startContentLayer = nbLayers + 1;
			endContentLayer = (2 * nbLayers) - 1;			
		}
		else
		{			
			startContentLayer = 2;
			endContentLayer = nbLayers;		
		}
			
		// On traite les maquettes d'abord
		InterfacePtr<IMasterSpreadList> masterSpreadList (doc, UseDefaultIID());

		int32 nbMasters = masterSpreadList->GetMasterSpreadCount();
		for(int32 i = 0 ; i < nbMasters ; ++i)
			TraiterLiensSpread(UIDRef(db, masterSpreadList->GetNthMasterSpreadUID(i)), startContentLayer, endContentLayer, imageDir);

		// Puis on termine par les pages
		InterfacePtr<ISpreadList> spreadList (doc, UseDefaultIID());
		
		int32 nbSpreads = spreadList->GetSpreadCount();
		for(int32 i = 0 ; i < nbSpreads ; ++i)
			TraiterLiensSpread(UIDRef(db, spreadList->GetNthSpreadUID(i)), startContentLayer, endContentLayer, imageDir);		

		status = kSuccess;

	} while(kFalse);
	
	return status;
}

void XPBCUtils::TraiterLiensSpread(UIDRef spreadRef, int32 startContentLayer, int32 endContentLayer, const IDFile& imageDir)
{	
	InterfacePtr<ISpread> spread (spreadRef, UseDefaultIID());
	if(!spread)
		return;

	IDataBase * db = spreadRef.GetDataBase();
			
	InterfacePtr<IHierarchy> spreadHier (spread, UseDefaultIID());
	int32 childCount = spreadHier->GetChildCount();
	for(int32 j = startContentLayer ; j <= endContentLayer ; ++j)
	{
		InterfacePtr<IHierarchy> spreadLayerHier (db, spreadHier->GetChildUID(j), UseDefaultIID());

		int32 nbItems = spreadLayerHier->GetChildCount();
		for(int32 itemIndex = 0 ; itemIndex < nbItems ; ++itemIndex)
		{
			InterfacePtr<IHierarchy> itemHier (db, spreadLayerHier->GetChildUID(itemIndex), UseDefaultIID());
			TraiterLienItem(itemHier, imageDir);					
		}
	}
}

void XPBCUtils::TraiterLienItem(IHierarchy * itemHier, const IDFile& imageDir)
{
	IDataBase * db = ::GetDataBase(itemHier);
	if(Utils<IPageItemTypeUtils>()->IsGraphicFrame(itemHier)) // case graphic frame
	{
		if(itemHier->GetChildCount() == 1)
		{						
			InterfacePtr<IHierarchy> imageHier (itemHier->QueryChild(0));

			InterfacePtr<IDataLinkReference> refLien (imageHier, UseDefaultIID());
			if(refLien->GetUID() != kInvalidUID)
				this->CopierLien(UIDRef(db, refLien->GetUID()), imageDir);
		}
	}
	else if(Utils<IPageItemTypeUtils>()->IsTextFrame(itemHier)) // case text frame
	{
		// Iterate through text model looking for inline frames
		InterfacePtr<IMultiColumnTextFrame> txtFrame (db, itemHier->GetChildUID(0), UseDefaultIID());
		
		InterfacePtr<ITextModel> txtModel (txtFrame->QueryTextModel());
		
		InterfacePtr<IItemStrand> itemStrand ((IItemStrand *) txtModel->QueryStrand(kOwnedItemStrandBoss, IID_IITEMSTRAND));
		
		UIDList inlineItems(db);
		itemStrand->GetAllItems(0, txtModel->GetPrimaryStoryThreadSpan(), &inlineItems);
		
		int32 nbInlines = inlineItems.Length();
		for(int32 i = 0 ; i < nbInlines ; ++i)
		{
			InterfacePtr<IHierarchy> inlineHier (db, inlineItems[i], UseDefaultIID());

			InterfacePtr<IHierarchy> inlineChildHier (db, inlineHier->GetChildUID(0), UseDefaultIID());
			TraiterLienItem(inlineChildHier, imageDir);
		}
		
	}
	else if(Utils<IPageItemTypeUtils>()->IsGroup(itemHier)) // case group item
	{
		int32 nbChild = itemHier->GetChildCount();
		// Iterate through each child
		for(int32 i = 0 ; i < nbChild ; ++i)
		{
			InterfacePtr<IHierarchy> childHier (db, itemHier->GetChildUID(i), UseDefaultIID());
			TraiterLienItem(childHier, imageDir);
		}
	}
}

void XPBCUtils::CopierLien(UIDRef refLien, const IDFile& imageDir)
{
	InterfacePtr<IDataLink> dataLink(refLien, IID_IDATALINK); 

	PMString * linkName = dataLink->GetFullName();

	IDFile linkFile = FileUtils::PMStringToSysFile(*linkName);

	PMString fileName = linkFile.GetFileName();

#ifdef MACINTOSH	
	// Purge filename from unwanted characters
	char invalidChars[] = { '/' , '\\', '\"' } ;
	
	for(int i = 0 ; i < 3 ; ++i)
	{
		int32 pos = -1;
		while( (pos = fileName.IndexOfCharacter(PlatformChar(invalidChars[i]))) >= 0)
			fileName.Remove(pos);
	}
	
#endif

	IDFile linkCopy = imageDir;
	FileUtils::AppendPath(&linkCopy, fileName);

	if(!(dataLink->GetStoredState(nil,nil) ==  IDataLink::kLinkMissing))
	{
#ifdef WINDOWS
		::CopyFile(FileUtils::SysFileToPMString(linkFile).GrabTString(), FileUtils::SysFileToPMString(linkCopy).GrabTString(), kFalse);
#else
		FileUtils::CopyFile(linkFile, linkCopy);
#endif

		// Update the data link to make it point to the related image in the package
		InterfacePtr<IDataLinkHelper> helper(CreateObject2<IDataLinkHelper>(kDataLinkHelperBoss)); 
		InterfacePtr<IDataLink> dataLinkNew(helper->CreateDataLink(linkCopy)); 
		
		if(dataLinkNew == nil)
		{
			PMString error(kXPBCErrorCopyLinkKey);
			error.Translate();
			StringUtils::ReplaceStringParameters(&error, FileUtils::SysFileToPMString(linkFile));
			CAlert::ErrorAlert(error);
			return;
		}	
		
		NameInfo name; 
		PMString type; 
		uint32 filetype; 
		dataLink->GetNameInfo(nil, &type, &filetype);
		dataLinkNew->GetNameInfo(&name, nil, nil); 
		
		InterfacePtr<ICommand> restoreLinkCmd (CmdUtils::CreateCommand(kRestoreLinkCmdBoss));
		InterfacePtr<IRestoreLinkCmdData> restoreLinkCmdData (restoreLinkCmd, UseDefaultIID());
		restoreLinkCmdData->Set(refLien.GetDataBase(), refLien.GetUID(), &name, &type, filetype, dataLink->GetStoredState(nil,nil));
		CmdUtils::ProcessCommand(restoreLinkCmd);
	}
}

ErrorCode XPBCUtils::RassemblerPolices(IDocument * doc)
{
	ErrorCode status = kFailure;

	do
	{
		if(!doc)
			break;

		IDataBase * db = ::GetDataBase(doc);
		const IDFile * docFile = db->GetSysFile();
		if(!docFile)
			break;

		// Creation du dossier d'assemblage des images
		IDFile fontDir;
		FileUtils::GetParentDirectory(*docFile, fontDir);
		FileUtils::AppendPath(&fontDir, PMString("Fonts"));
		FileUtils::CreateFolderIfNeeded(fontDir);

		// Parcours de la liste des polices utilisees
		InterfacePtr<IUsedFontList> fontList (doc, UseDefaultIID());
		int32 fontCount = fontList->CountUsedFontFamilies ();

		for(int32 i = 0 ; i < fontCount ; ++i)
		{
			UID fontFamilyUID =  fontList->GetUsedFontFamily(i);
			InterfacePtr<IFontFamily> fontFamily (::GetDataBase(doc), fontFamilyUID, UseDefaultIID());

			int32 fontStyles = fontList->CountUsedFontVariants(fontFamilyUID);
			for(int32 k = 0 ; k < fontStyles ; ++k)
			{				
				PMString style = *(fontList->GetUsedFontVariant(::GetUID(fontFamily), k));
							
				// Parcours de la liste des styles ("Roman", "Bold", "Light"...) utilise pour cette famille de police
				InterfacePtr<IPMFont> theUsedFont (fontFamily->QueryFace(style));
				if(theUsedFont && theUsedFont->GetFontStatus() == IPMFont::kFontInstalled)
				{
					boost::shared_ptr<K2Vector<PMString> > paths (theUsedFont->GetFullPath());
					if(paths.get())
					{
						for(int32 j = 0 ; j < paths->size() ; ++j)
						{
							IDFile fontFile = FileUtils::PMStringToSysFile((*paths)[j]);						
							IDFile destFontFile = fontDir;
							FileUtils::AppendPath(&destFontFile, fontFile.GetFileName());
#ifdef WINDOWS
			::CopyFile(FileUtils::SysFileToPMString(fontFile).GrabTString(), FileUtils::SysFileToPMString(destFontFile).GrabTString(), kFalse);
#else
			FileUtils::CopyFile(fontFile, destFontFile);
#endif
						}				
					}
				}
			}								
		}

		status = kSuccess;

	} while(kFalse);
	
	return status;
}

double XPBCUtils::GetAsDouble(const PMString& value)
{
	PMString::ConversionError error = PMString::kNoError;
	double number = value.GetAsDouble(&error);

	if(error != PMString::kNoError)
	{
		if(error == PMString::kNotJustNumber) // Must be a decimal number, just replace comma with dot
		{
			PMString toParse = value;
			int32 length = toParse.NumUTF16TextChars();		
			for(int32 i = 0 ; i < length ; ++i)
			{				
				if(toParse.GetChar(i).IsComma())
				{
					toParse.Remove(i);
					toParse.Insert('.',i);
				}
			}
			number = toParse.GetAsDouble(&error);
		}
	}			
	return number;
}

ErrorCode XPBCUtils::SetPersistentString(UIDRef target, PMIID interfaceID, const PMString& value)
{
	InterfacePtr<ICommand> setStringCmd (CmdUtils::CreateCommand(kXPBCSetStringCmdBoss));
	setStringCmd->SetItemList(UIDList(target));
	setStringCmd->SetUndoability(ICommand::kAutoUndo);

	InterfacePtr<IStringData> cmdData (setStringCmd, UseDefaultIID());
	cmdData->Set(value);

	InterfacePtr<IIntData> targetInterface (setStringCmd, UseDefaultIID());
	targetInterface->Set(interfaceID.Get());

	return CmdUtils::ProcessCommand(setStringCmd);
}

/** 
 * Desc : PackageExport
 */
ErrorCode XPBCUtils::PackageExport(const IDocument * doc, IDFile& folderFile)
{
	ErrorCode status = kFailure;
	ICommandSequence* seq = nil;
	PMString error("");
	do
	{
		seq = CmdUtils::BeginCommandSequence();
		if(seq == nil)
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}
		
		seq->SetUndoability(ICommand::kAutoUndo);		
		if(!FileUtils::DoesFileExist(folderFile))
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}			
		if(!FileUtils::IsDirectory(folderFile))
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}
		
		PMString docName;	
		doc->GetName(docName);
		docName.Truncate(5);

		IDTime now;
		now.CurrentTime();

		int32 year = 0, month = 0, day = 0, hour = 0, min = 0, sec = 0;
		now.GetTime(&year, &month, &day, &hour, &min, &sec);

		docName += "_";
		docName.AppendNumber(year);
		docName.AppendNumber(month);
		docName.AppendNumber(day);
		docName.AppendNumber(hour);
		docName.AppendNumber(min);
		docName.AppendNumber(sec);

		FileUtils::AppendPath(&folderFile, docName);
		
		
		FileUtils::CreateFolderIfNeeded(folderFile);
		
			
		InterfacePtr<IS4SPPackage> s4sPPackage((IS4SPPackage*) ::CreateObject(kPackageAPIBoss, IID_IS4SPPACKAGEAPI));
		if (!s4sPPackage)
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}
		
		s4sPPackage->Init(doc);
		
		s4sPPackage->SetHiddenLayersFlag(true);
		s4sPPackage->SetCopyGLinksFlag(true);
		s4sPPackage->SetCopyFontsFlag(true); 
		s4sPPackage->SetUpdateGLinksFlag(true); 
		s4sPPackage->SetViewReportFlag(true);
		s4sPPackage->SetHyphExceptionsFlag(true);
		
		s4sPPackage->SetPackageLocation(folderFile);
		
		if(s4sPPackage->PhysicalPackage() != IS4SPPackage::kPackageOK)
		{
			error = ErrorUtils::PMGetGlobalErrorString();
			break;
		}
		
		status = kSuccess ;	  
		
	} while(kFalse);
	
	if (seq != nil)
	{
		if (status == kSuccess)
		{
			CmdUtils::EndCommandSequence(seq);		 
		}
		else
		{
			ErrorUtils::PMSetGlobalErrorCode(kFailure);
			CmdUtils::EndCommandSequence(seq);	 
		}
	}
	
	return status;
}