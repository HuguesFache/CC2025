/*
//	File:	XPGUIPlacedFormeHeaderAdornment.cpp
//
//	Date:	24-Oct-2005
//
//  Author : Trias Developpement
//
*/

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IAdornmentShape.h"
#include "IGeometry.h"
#include "IGraphicsPort.h"
#include "IHierarchy.h"
#include "IInterfaceColors.h"
#include "IShape.h"
#include "IViewPortAttributes.h"
#include "IFontMgr.h"
#include "IPMFont.h"
#include "IFontInstance.h"
#include "ILayoutControlData.h"
#include "IPanorama.h"
#include "ILayoutUIUtils.h"
#include "CAlert.h"
#include "IXPageUtils.h"

// General includes:
#include "TransformUtils.h"
#include "Utils.h"

// Project includes:
#include "XPGUIID.h"
#include "XPGID.h"
#include "IFormeData.h"

/** Implementation of IAdornmentShape to decorate custom datalinked container page 
	items
	@ingroup customdatalink
*/
class XPGUIPlacedFormeHeaderAdornment:public CPMUnknown<IAdornmentShape>{

	public:
		/** Constructor.
			@param boss
		*/
		XPGUIPlacedFormeHeaderAdornment(IPMUnknown* boss);

		/** Destructor.
		*/
		virtual ~XPGUIPlacedFormeHeaderAdornment();	
		
		/** See IAdornmentShape::GetDrawOrderBits
		 */
		virtual AdornmentDrawOrder	GetDrawOrderBits();

			/** See IAdornmentShape::Draw
		 */
		virtual void				DrawAdornment
			(
				IShape* iShape, 
				AdornmentDrawOrder drawOrder, 
				GraphicsData* gd, 
				int32 flags
			);
	
		/** See IAdornmentShape::GetPaintedBBox
		 */
		virtual PMRect				  GetPaintedAdornmentBounds 
			(
				IShape*				iShape,
				AdornmentDrawOrder	drawOrder,
				const PMRect&		itemBounds, // This is the painted bounds of the owing page item
				const PMMatrix&		innertoview	// NOTE: this is inner to view not pb to view
			);

		virtual PMRect GetPrintedAdornmentBounds  
			( 
				IShape *  iShape,  
				AdornmentDrawOrder  drawOrder,  
				const PMRect &  itemBounds,  
				const PMMatrix &  innertoview   
			);
			/** See IAdornmentShape::AddToContentInkBounds
		 */
		virtual void				AddToContentInkBounds
			(
				IShape* iShape, 
				PMRect* inOutBounds
			);
		/** See IAdornmentShape::GetPriority
		 */
		virtual PMReal				GetPriority();

			/** See IAdornmentShape::Inval
		 */
		virtual void				Inval
			(
				IShape* iShape, 
				AdornmentDrawOrder drawOrder, 
				GraphicsData* gd, 
				ClassID reasonForInval, 
				int32 flags
			);

		/** See IAdornmentShape::WillPrint
		 */
		virtual bool16				WillPrint();

		/** See IAdornmentShape::WillDraw
		 */
		virtual bool16				WillDraw
			(
				IShape* iShape,
				AdornmentDrawOrder	drawOrder,	
				GraphicsData*		gd,
				int32				flags
			);


		virtual bool16  HitTest 
			(
				IShape *iShape, 
				AdornmentDrawOrder adornmentDrawOrder, 
				IControlView *layoutView, 
				const PMRect &mouseRect
			);

	private :		
		
		PMString SplitFormeDescription(const PMString& uniqueName);
		PMPoint GetLeftTopPoint(IGeometry * itemGeo);	
};

/*
*/
CREATE_PMINTERFACE(XPGUIPlacedFormeHeaderAdornment, kXPGUIPlacedFormeHeaderAdornmentImpl)

/*
*/
XPGUIPlacedFormeHeaderAdornment::XPGUIPlacedFormeHeaderAdornment(IPMUnknown* boss):CPMUnknown<IAdornmentShape>(boss){
	  
}

/*
*/
XPGUIPlacedFormeHeaderAdornment::~XPGUIPlacedFormeHeaderAdornment(){}

/*
*/
IAdornmentShape::AdornmentDrawOrder XPGUIPlacedFormeHeaderAdornment::GetDrawOrderBits(){
	return kAfterShape;
}

/*
*/
void XPGUIPlacedFormeHeaderAdornment::DrawAdornment(IShape* iShape, IAdornmentShape::AdornmentDrawOrder drawOrder,	GraphicsData* gd, int32 flags){

	do {	
		if (drawOrder != kAfterShape) 
			break;		 

		if (flags & IShape::kPrinting)  		 
			break;		 
		
		// Get the forme name of the item and make it a string		
		InterfacePtr<IFormeData> formeData (iShape, UseDefaultIID());
		if(formeData == nil)
			break;
		
		PMString formeName = kNullString, dummy = kNullString;
		Utils<IXPageUtils>()->SplitFormeDescription(formeData->GetUniqueName(), dummy, formeName);
		if(formeName == kNullString)
			break;

		int32 numUTF16;
		const textchar * buff = formeName.GrabUTF16Buffer(&numUTF16);
	
		// The labels are created using the default font.
		InterfacePtr<IFontMgr> fontMgr(GetExecutionContextSession(), UseDefaultIID());
		if (fontMgr == nil)
			break;

		InterfacePtr<IPMFont> theFont(fontMgr->QueryFont(fontMgr->GetDefaultFontName()));
		InterfacePtr<IGeometry> itemGeometry(iShape, UseDefaultIID());
		InterfacePtr<IHierarchy> itemHier(iShape, UseDefaultIID());
		if (theFont == nil || itemGeometry == nil || itemHier == nil)
			break;
		
		PMReal fontSize = 10.0;
		PMReal xOffset = 1.5, yOffset = 1.5; 
		PMReal xBoxOffset = 4.0, yBoxOffset = 2.0;
		PMReal zoom = 1.0;
 
		// Check current zoom factor and increade font size if necessary
		InterfacePtr<ILayoutControlData> layoutControlData (Utils<ILayoutUIUtils>()->QueryFrontLayoutData());
		InterfacePtr<IPanorama> panorama (layoutControlData, UseDefaultIID());
		if(panorama)
			zoom = panorama->GetXScaleFactor();		
 
		fontSize = fontSize * (1.0 / zoom); // this way, tag has always the same size, whatever the zoom factor is
		xOffset = xOffset * (1.0 / zoom);
		yOffset = yOffset * (1.0 / zoom);

		PMMatrix matrix(	fontSize,			0.0, 
							0.0, 				fontSize, 
							0.0, 				0.0 
						);

		InterfacePtr<IFontInstance> fontInst(fontMgr->QueryFontInstance(theFont, matrix));
		if (fontInst == nil)
			break;

		// Now measure the length of our string using the glyphs from the font.
        PMReal width, height;
        fontInst->MeasureWText(buff, numUTF16, width, &height);
		if(height == 0)
			// From top of ascender to bottom of descender
        	height = fontInst->GetAscent();
		
		// Set up the port.
		IGraphicsPort* gPort = gd->GetGraphicsPort();
		if (gPort == nil)
			break;

		gPort->gsave();

		InterfacePtr<ITransform> itemTransform (itemGeometry, UseDefaultIID());
		PMReal rotationAngle = itemTransform->GetItemRotationAngle();
		PMReal yScale = itemTransform->GetItemScaleY();

		if(yScale < 0){
			gPort->scale(1,-1);
			if(rotationAngle == 180)
				gPort->rotate(-180);
		}

		PMPoint leftTop = itemGeometry->GetStrokeBoundingBox().LeftTop();
		
		gPort->newpath();
		PMReal left = leftTop.X() + xBoxOffset;		
		PMReal top = leftTop.Y() - height - (2 * yOffset);
		PMReal right = left + width + (2 * xOffset);
		PMReal bottom = leftTop.Y() - yBoxOffset;

		PMRect boundsRect = PMRect(left, top, right, bottom);		
		gPort->newpath();
		gPort->setrgbcolor(256.0/256.0, 256.0/256.0, 256.0/256.0); // white fill
		gPort->rectpath(boundsRect);
		gPort->fill();
		
		gPort->rectpath(boundsRect);
		gPort->setrgbcolor(0/256.0, 0/256.0, 0/256.0); // black border
		gPort->setlinewidth(0);
		gPort->stroke();	 
	
		PMReal x = boundsRect.Left() + xOffset; 
		PMReal y = boundsRect.Top() + height + yOffset; 

		// Set the drawing color for the port to a special value for the label
		gPort->setrgbcolor(175/256.0, 16/256.0, 35/256.0); // red text color 

		// Set the font in the port
		gPort->selectfont(theFont, fontSize);

		// Draw the string in the port
		gPort->show(x, y, numUTF16, buff);
		gPort->grestore();

	} while(kFalse);
}

/* Returns the bounds of the adornment in view co-ordinates.
*/
PMRect XPGUIPlacedFormeHeaderAdornment::GetPaintedAdornmentBounds(IShape* iShape, AdornmentDrawOrder	drawOrder, const PMRect& itemBounds, const PMMatrix& innertoview){
	
	PMRect result = itemBounds;
	do{
		InterfacePtr<IFormeData> formeData (iShape, UseDefaultIID());
		if(formeData == nil)
			break;

		PMString formeName = kNullString, dummy = kNullString;
		Utils<IXPageUtils>()->SplitFormeDescription(formeData->GetUniqueName(), dummy, formeName);
		if(formeName == kNullString)
			break;		

		int32 numUTF16;
		const textchar * buff = formeName.GrabUTF16Buffer(&numUTF16);
	
		// The labels are created using the default font.
		InterfacePtr<IFontMgr> fontMgr(GetExecutionContextSession(), UseDefaultIID());
		if (fontMgr == nil)
			break;

		InterfacePtr<IPMFont> theFont(fontMgr->QueryFont(fontMgr->GetDefaultFontName()));
		InterfacePtr<IGeometry> itemGeometry(iShape, UseDefaultIID());
		InterfacePtr<IHierarchy> itemHier(iShape, UseDefaultIID());
		if (!theFont || !itemGeometry || !itemHier)
			break;

		PMReal fontSize = 10;
		PMReal xOffset = 1.5, yOffset = 1.5; 
		PMReal xBoxOffset = 4.0, yBoxOffset = 2.0;
		PMReal zoom = 1.0;
 
		// Check current zoom factor and increade font size if necessary
		InterfacePtr<ILayoutControlData> layoutControlData (Utils<ILayoutUIUtils>()->QueryFrontLayoutData());
		InterfacePtr<IPanorama> panorama (layoutControlData, UseDefaultIID());
		if(panorama)
			zoom = panorama->GetXScaleFactor(); 

		fontSize = fontSize * (1.0 / zoom);
		xOffset = xOffset * (1.0 / zoom);
		yOffset = yOffset * (1.0 / zoom);

		PMMatrix matrix(	fontSize,	0.0, 
							0.0, 		fontSize, 
							0.0, 		0.0 
						);

		InterfacePtr<IFontInstance> fontInst(fontMgr->QueryFontInstance(theFont, matrix));
		if (fontInst == nil)
			break;

		// Now measure the length of our string using the glyphs from the font.
        PMReal width, height;
		fontInst->MeasureWText(buff, numUTF16, width, &height);

		if(height == 0) 
			// From top of ascender to bottom of descender
        	height = fontInst->GetAscent();		 

		result.Right() = itemBounds.Left() -  width - (2 * xOffset) - xBoxOffset - 1;
		result.Bottom() = itemBounds.Top() - height - (2 * yOffset) - yBoxOffset - 1;

	} while(kFalse);

	return result;
}

PMRect XPGUIPlacedFormeHeaderAdornment::GetPrintedAdornmentBounds(IShape *  iShape, AdornmentDrawOrder  drawOrder, const PMRect &  itemBounds,  const PMMatrix &  innertoview)
{
	 PMRect result = itemBounds; 
     return result;
}

/*
*/
void XPGUIPlacedFormeHeaderAdornment::AddToContentInkBounds(IShape* iShape, PMRect* inOutBounds){
}

/*	Returning a default value of 0.0 means the order is unimportant relative to 
	other "standard" priority adornments.
*/
PMReal XPGUIPlacedFormeHeaderAdornment::GetPriority(){
	return 0.0;
}

/*	
*/
void XPGUIPlacedFormeHeaderAdornment::Inval(IShape* iShape, IAdornmentShape::AdornmentDrawOrder drawOrder, GraphicsData* gd, ClassID reasonForInval, int32 flags){
}

/*
*/
bool16 XPGUIPlacedFormeHeaderAdornment::WillPrint(){
	return kFalse;
}

/*
*/
bool16 XPGUIPlacedFormeHeaderAdornment::WillDraw(IShape* iShape, AdornmentDrawOrder drawOrder, GraphicsData* gd, int32 flags){
	bool16 result = kFalse;
	do {
		if (drawOrder != kAfterShape)  
			break;		

		if (flags & IShape::kPrinting)  
			break;			
		result = kTrue;

	} while(false);

	return result;
}

PMPoint XPGUIPlacedFormeHeaderAdornment::GetLeftTopPoint(IGeometry * itemGeo){

	PMRect rect = itemGeo->GetStrokeBoundingBox();
	
	InterfacePtr<ITransform> itemTransform (itemGeo, UseDefaultIID());
	PMReal rotationAngle = itemTransform->GetItemRotationAngle();
	PMReal yScale = itemTransform->GetItemScaleY();

	PMPoint leftTop;
	if(rotationAngle == 360)
		rotationAngle = 0;
	
	if(rotationAngle >= 0 && rotationAngle < 90)
		leftTop = rect.LeftTop();	
	else if(rotationAngle >= 90 && rotationAngle < 180)
		leftTop = rect.LeftBottom();	
	else if(rotationAngle >= 180 && rotationAngle < 270)
		leftTop = rect.RightBottom();	
	else 
		leftTop = rect.RightTop();
	return leftTop;
}
/*
	@desc Get forme name from unique forme description
*/

PMString XPGUIPlacedFormeHeaderAdornment::SplitFormeDescription(const PMString& uniqueName){

	PMString formeName = kNullString;
	do{			
		K2::scoped_ptr<PMString> name (uniqueName.GetItem ("_", 2)); // Forme name in second position 
		if(name == nil)
			break;

		formeName = PMString(*name);

	}while(kFalse);
	return formeName;
 }  
 
bool16 XPGUIPlacedFormeHeaderAdornment::HitTest(IShape *iShape,AdornmentDrawOrder adornmentDrawOrder, IControlView *layoutView, const PMRect &mouseRect)
{
	return kFalse;
}