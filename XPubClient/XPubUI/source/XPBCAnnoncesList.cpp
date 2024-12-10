/*
//
//	File: XRLUIResaPubMarbreList.cpp
//
//  Author: Trias
//
//	Date:	30-Avr-2008
//
//	ADOBE SYSTEMS INCORPORATED
//	Copyright 2008 Trias Developpement. All rights reserved.
//
*/

#include "VCPlugInHeaders.h"

// API includes
#include "CPMUnknown.h"

// Project includes:
#include "IAnnoncesList.h"
#include "IPMStream.h"

#include "CAlert.h"


/** XPBCAnnoncesList
*/

class XPBCAnnoncesList : public CPMUnknown<IAnnoncesList>
{
public:
	/**
		Constructor.

		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPBCAnnoncesList(IPMUnknown* boss);


	/** Destructor
	*/
	~XPBCAnnoncesList();

	/**
		Accessor methods for ResaPub list
	*/	
	virtual void SetAnnoncesList(const K2Vector<annoncedata>& newList);
	virtual K2Vector<annoncedata> GetAnnoncesList();

private:
	
	K2Vector<annoncedata> annoncesList;
};

CREATE_PMINTERFACE(XPBCAnnoncesList, kXPBCAnnoncesListImpl)

/* Constructor
*/
XPBCAnnoncesList::XPBCAnnoncesList(IPMUnknown* boss)
: CPMUnknown<IAnnoncesList>(boss)
{
}

/* Destructor
*/
XPBCAnnoncesList::~XPBCAnnoncesList()
{
}


/* SetResaPubList
*/
void XPBCAnnoncesList::SetAnnoncesList(const K2Vector<annoncedata> &newList)
{	
	annoncesList = newList;
}

/* GetResaPubList
*/
K2Vector<annoncedata> XPBCAnnoncesList::GetAnnoncesList()
{
	return annoncesList;
	
}
