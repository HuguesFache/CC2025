
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IAdornmentShape.h"
#include "IGeometry.h"
#include "IGraphicsPort.h"
#include "IHierarchy.h"
#include "IInterfaceColors.h"
#include "IShape.h"
#include "IViewPortAttributes.h"
#include "IBoolData.h"
#include "IFontMgr.h"
#include "IFontInstance.h"
#include "IPMFont.h"
#include "IDocument.h"
#include "ILayoutControlData.h"
#include "IPanorama.h"
#include "ILayoutUIUtils.h"

// General includes:
#include "AGMGraphicsContext.h"
#include "AutoGSave.h"
#include "TransformUtils.h"
#include "Utils.h"
#ifdef DEBUG
#include "DebugClassUtils.h"
#endif

// Project includes:
#include "XPGID.h"
#include "XPGUIID.h"


/** Implementation of IAdornmentShape to graphic frames in PMP templates
*/
class XPGUIResaRedacAdornmentShape : public CPMUnknown<IAdornmentShape>
{
	public:
		/** Constructor.
			@param boss
		*/
		XPGUIResaRedacAdornmentShape(IPMUnknown* boss);

		/** Destructor.
		*/
		virtual ~XPGUIResaRedacAdornmentShape();	
		
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
		virtual PMRect				GetPaintedAdornmentBounds
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
				IShape* 			iShape,
				AdornmentDrawOrder	drawOrder,	
				GraphicsData*		gd,
				int32				flags
			);

		/** See IAdornmentShape::HitTest
		*/
		virtual bool16 HitTest 
			( 
				IShape*				  iShape,         // The owning page item 
				AdornmentDrawOrder    adornmentDrawOrder,    // In case this adornment is used more than once 
				IControlView *		layoutView,
				const PMRect&         r 
			){ return kFalse; }

};

/*
*/
CREATE_PMINTERFACE(XPGUIResaRedacAdornmentShape, kXPGUIResaRedacAdornmentShapeImpl)

/*
*/
XPGUIResaRedacAdornmentShape::XPGUIResaRedacAdornmentShape(IPMUnknown* boss) : CPMUnknown<IAdornmentShape>(boss)
{
}

/*
*/
XPGUIResaRedacAdornmentShape::~XPGUIResaRedacAdornmentShape()
{
}

/*
*/
IAdornmentShape::AdornmentDrawOrder XPGUIResaRedacAdornmentShape::GetDrawOrderBits()
{
	return kAfterEndShape; // After everything so transparency on item won't affect this adornment
}

/*
*/
void XPGUIResaRedacAdornmentShape::DrawAdornment(IShape* iShape, IAdornmentShape::AdornmentDrawOrder drawOrder, 
			GraphicsData* gd, int32 flags)
{
	do {
		
		if (drawOrder != kAfterEndShape) {
			break;
		}

		if (flags & IShape::kPrinting) {
			break;
		}
		
		// Check prefs for adornments display
		InterfacePtr<IBoolData> showResaPrefs((IBoolData*) GetExecutionContextSession()->QueryWorkspace(IID_ISHOWRESAPREFS));
		if(!(showResaPrefs->Get())) {
			break;
		}

		// Set up the port.
		IGraphicsPort* gPort = gd->GetGraphicsPort();
		AutoGSave autoGSave(gPort);

		InterfacePtr<IGeometry> geometry(iShape, UseDefaultIID());
		ASSERT(geometry);
		if(!geometry) {
			break;
		}
	
		// Get current zoom factor
		PMReal zoom = 1.0;
		InterfacePtr<ILayoutControlData> layoutControlData (Utils<ILayoutUIUtils>()->QueryFrontLayoutData());
		InterfacePtr<IPanorama> panorama (layoutControlData, UseDefaultIID());
		if(panorama)
			zoom = panorama->GetXScaleFactor();	
	
		PMPoint leftTop = geometry->GetStrokeBoundingBox().LeftTop();

		// Draw a 6.0 radius green circle at the left top corner of the item, adjusting size according to current zoom
		gPort->newpath();
		gPort->setrgbcolor(51/255, 255/255, 51/255);

		const PMReal xrad = 6.0/zoom, yrad = 6.0/zoom;
		const PMReal magic = 0.551784;
        PMReal xmagic = xrad * magic;
        PMReal ymagic = yrad * magic;
        
		gPort->translate(leftTop.X(), leftTop.Y());
		gPort->moveto(-xrad, 0);
        gPort->curveto(-xrad, ymagic, -xmagic, yrad, 0, yrad);
        gPort->curveto(xmagic, yrad, xrad, ymagic, xrad, 0);
        gPort->curveto(xrad, -ymagic, xmagic, -yrad, 0, -yrad);
        gPort->curveto(-xmagic, -yrad, -xrad, -ymagic, -xrad, 0);
		gPort->fill();

	} while(false);


}

/* Returns the bounds of the adornment in view co-ordinates.
*/
PMRect XPGUIResaRedacAdornmentShape::GetPaintedAdornmentBounds
(
	IShape*				iShape,
	AdornmentDrawOrder	drawOrder,
	const PMRect&		itemBounds,  
	const PMMatrix&		innertoview
)
{
	PMRect result = itemBounds;

	PMMatrix viewToInner = innertoview.Inverse();
	viewToInner.Transform(&result);

	result.Bottom() = result.Top();
	result.Top() = result.Top() - 10;
	result.Right() = result.Left() + (result.Width()/4);
	
	innertoview.Transform(&result); 
	
	return result;
}

PMRect XPGUIResaRedacAdornmentShape::GetPrintedAdornmentBounds(IShape *  iShape, AdornmentDrawOrder  drawOrder, const PMRect &  itemBounds,  const PMMatrix &  innertoview)
{
	 PMRect result = itemBounds; 
     return result;
}


/*
*/
void XPGUIResaRedacAdornmentShape::AddToContentInkBounds(IShape* iShape, PMRect* inOutBounds)
{
}

/*	Returning a default value of 0.0 means the order is unimportant relative to 
	other "standard" priority adornments.
*/
PMReal XPGUIResaRedacAdornmentShape::GetPriority()
{
	return 0.0;
}

/*	
*/
void XPGUIResaRedacAdornmentShape::Inval(IShape* iShape, IAdornmentShape::AdornmentDrawOrder drawOrder, GraphicsData* gd, 
			ClassID reasonForInval, int32 flags)
{

}

/*
*/
bool16 XPGUIResaRedacAdornmentShape::WillPrint()
{
	return kFalse;
}

/*
*/
bool16 XPGUIResaRedacAdornmentShape::WillDraw
		(
			IShape* 			iShape,
			AdornmentDrawOrder	drawOrder,
			GraphicsData*		gd,
			int32			flags
		)
{
	bool16 result = kFalse;
	do {
		ASSERT(drawOrder == kAfterShape);
		if (drawOrder != kAfterShape) {
			break;
		}

		if (flags & IShape::kPrinting) {
			break;
		}
		
		InterfacePtr<IBoolData> showResaPrefs((IBoolData*) GetExecutionContextSession()->QueryWorkspace(IID_ISHOWRESAPREFS));
		if(!(showResaPrefs->Get())) {
			break;
		}

		result = kTrue;

	} while(false);

	return result;
}
