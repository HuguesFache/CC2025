/*
//	File:	PermRefsUtils.h
//
//	Date:	27-oct-2005
//
//	Author : Wilfried LEFEVRE
*/

#ifndef __PermRefsUtils_h__
#define __PermRefsUtils_h__

#include "K2SmartPtr.h"
#include "BaseType.h" 
#include "ITCLParser.h"
#include "K2Pair.h"

class ITextModel;
class WideString;
class ITableFrame;

// This class provides different static functions dealing with PermRefs
class PermRefsUtils 
{

public :

	static ErrorCode ApplyPermRefs(PermRefStruct ref, ITextModel * txtModel, TextIndex start, TextIndex end);

	static ErrorCode UpdatePermRefs(PermRefStruct ref, ITextModel * txtModel, boost::shared_ptr< WideString > replaceString, bool16 deleteNextSpaceorCR = kFalse);

	static ErrorCode LookForPermRefs(PermRefStruct ref, IDataBase * db,  K2Vector<UID> stories, UID& story, TextIndex& start, TextIndex& end);
	
	static ErrorCode GetPermRefs(ITextModel * txtModel, TextIndex& start, TextIndex& end, K2Vector<PermRefStruct>& listRef);
	
	static void		 GetTagBounds(ITextModel * txtModel, TextIndex& leftBound, TextIndex& rightBound);
	
	static ErrorCode ExportPermRefs(const IDocument * doc, const IDFile& exportFile);

	static ErrorCode ExportPermRefsWithContent (const IDocument * doc, const IDFile& exportFile);

	static bool8 	 UpdatePermRefsList(PermRefVector& permrefsList, ITextModel * txtModel, TextIndex start, TextIndex end);
	
	static bool16    IsAnyTagInRange(ITextModel *model,const TextIndex& position, const int32& length, PermRefStruct& permref);

	static bool16    IsSameTagInRange(ITextModel *model,const TextIndex& position, const int32& length, const PermRefStruct& permref);

	static ErrorCode CopyContiguousRef(ITextModel *model, const PermRefStruct& permref, const int32& nbCopy, 
									   const textchar& separator, const int32& nbSep, PermRefVector& permRefsList);
	
	static PMReal 	 GetHeight(PermRefStruct ref, ITextModel * txtModel);
	
	static PMString	 GetTextAttribute(ClassID attrID, const IPMUnknown * inter);

	static ErrorCode DeleteAllPermRefs(const IDocument * doc);

	static ErrorCode AutoTaggingDocumentPermRefs(const IDocument * doc);
	
	static ErrorCode AutoTaggingDocumentPermRefs_GEDIMAT2015(const IDocument * doc); // GD 15.09.2015

    static ErrorCode AutoTaggingDocumentPermRefs_GEDIMAT2022(const IDocument * doc); // GD 02.09.2022
    
	static ErrorCode AutoTaggingDocumentWithReferencePermRefs(const IDocument * doc);

	static PMString  StripString(const PMString& string, int32& numberOfCharsStripped);

	static PermRefStruct GetLastPermRef();

private :

	static K2Vector<UID> currentStories;
	static PermRefStruct currentRef;
	static int32 currentStory;
	static TextIndex startIndex;

};

#endif //__PermRefsUtils_h__
