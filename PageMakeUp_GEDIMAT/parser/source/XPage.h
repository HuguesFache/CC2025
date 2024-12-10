/*
//	File:	XPage.h
//
//	Date:	21-Jul-2011
//
//  Author : Trias
//
*/


#ifndef __XPage_h__
#define __XPage_h__


class ITCLParser;

/** Func_AF
	Implement the AF command
*/
class Func_AF
{
public :

	/**
		Constructor
		Read MN command parameters and call IDCall_AF
		@param IN parser : TCL parser
	*/
	Func_AF(ITCLParser *  parser);

	/**
		Destructor
	*/
	~Func_AF() {}

private :
	
	/**
		Import a forme into a page, optionaly creating a resa at the same time
	*/
	void IDCall_AF(ITCLParser *  parser);

	PMReal hPos, vPos;
	PMString formePath;
	int32 idResa;
};

/** Func_AA
	Implement the AA command
*/
class Func_AA
{
public :

	/**
		Constructor
		Read AA command parameters and call IDCall_AA
		@param IN parser : TCL parser
	*/
	Func_AA(ITCLParser *  parser);

	/**
		Destructor
	*/
	~Func_AA() {}

private :
	
	/**
		Convert a resa into an InCopy Story
	*/
	void IDCall_AA(ITCLParser *  parser);

	int32 idResa;
	PMString inCopyAssignmentPath, topic, recipientName;
};

/** Func_AM
	Implement the AM command
*/
class Func_AM
{
public :

	/**
		Constructor
		Read AM command parameters and call IDCall_AM
		@param IN parser : TCL parser
	*/
	Func_AM(ITCLParser *  parser);

	/**
		Destructor
	*/
	~Func_AM() {}

private :
	
	/**
		Convert a resa into an InCopy Story
	*/
	void IDCall_AM(ITCLParser *  parser);

	PMString idStory;
	int16 typeMarbre;
};


/** Func_AB
	Implement the AB command
*/
class Func_AB
{
public :

	/**
		Constructor
		Read AB command parameters and call IDCall_AB
		@param IN parser : TCL parser
	*/
	Func_AB(ITCLParser *  parser);

	/**
		Destructor
	*/
	~Func_AB() {}

private :
	
	/**
		Import an assembly into a page, optionaly creating resa at the same time
	*/
	void IDCall_AB(ITCLParser *  parser);

	PMReal hPos, vPos;
	PMString assemblyPath;
	K2Vector<KeyValuePair<PMString, int32> > resas; // Pair of (ID_Forme, ID_Resas)
};

#endif // __XPage_h__