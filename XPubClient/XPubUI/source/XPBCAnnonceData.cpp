/*
//
//	File: XRLResaPubList.cpp
//
//  Author: Trias
//
//	Date:	3-Mar-2008
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


/** XRLResaPubList
	Persistent implementation of ITCLReferencesList, added to the kDocBoss
*/

class XPBCAnnonceData : public CPMUnknown<IAnnoncesList>
{
public:
	/**
		Constructor.

		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPBCAnnonceData(IPMUnknown* boss);


	/** Destructor
	*/
	~XPBCAnnonceData();

	/**
		Accessor methods for ResaPub list
	*/	
	virtual void SetAnnoncesList(const K2Vector<annoncedata>& newList);
	virtual K2Vector<annoncedata> GetAnnoncesList();


	/**
		Because this is a persistent interface, it must support the ReadWrite method. This method is used for 
		both writing information out to the database and reading information in from the database.

		@param stream contains the stream to be written or read.
		@param implementation the implementation ID.
	*/
	void ReadWrite(IPMStream* stream, ImplementationID implementation);

private:
	
	K2Vector<annoncedata> annoncesList;
};

CREATE_PERSIST_PMINTERFACE(XPBCAnnonceData, kXPBCAnnonceDataImpl)

/* Constructor
*/
XPBCAnnonceData::XPBCAnnonceData(IPMUnknown* boss)
: CPMUnknown<IAnnoncesList>(boss)
{
}

/* Destructor
*/
XPBCAnnonceData::~XPBCAnnonceData()
{
}


/* SetResaPubList
*/
void XPBCAnnonceData::SetAnnoncesList(const K2Vector<annoncedata> &newList)
{
	PreDirty();
	annoncesList = newList;
}

/* GetResaPubList
*/
K2Vector<annoncedata> XPBCAnnonceData::GetAnnoncesList()
{
	return annoncesList;
}


/* ReadWrite
*/
void XPBCAnnonceData::ReadWrite(IPMStream* stream, ImplementationID implementation)
{
	if(stream->IsReading()) // Fill the vector with the saved value
	{
		int32 length = 0;
		stream->XferInt32(length);
	
		annoncesList.clear();
		for(int32 i = 0; i < length; i++)
		{
			annoncedata uneAnnonce;
			uneAnnonce.ID.ReadWrite(stream);
			uneAnnonce.Status.ReadWrite(stream);
			uneAnnonce.Name.ReadWrite(stream);
			annoncesList.push_back(uneAnnonce);
		}
	}
	else // Save the item in the vector
	{
		int32 length = annoncesList.size();
		stream->XferInt32(length);

		K2Vector<annoncedata>::iterator iter;
		for (iter = annoncesList.begin(); iter < annoncesList.end(); iter++)
		{
			annoncedata uneAnnonce = *iter;
			uneAnnonce.ID.ReadWrite(stream);
			uneAnnonce.Status.ReadWrite(stream);
			uneAnnonce.Name.ReadWrite(stream);
		}	
	}
}

