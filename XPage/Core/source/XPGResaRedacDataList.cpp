
#include "VCPlugInHeaders.h"

// API includes
#include "CPMUnknown.h"

// Project includes:
#include "XPGID.h" 
#include "IResaRedacDataList.h"
#include "IPMStream.h"
#include "UIDList.h"

// General includes:
#include <map>

/** Persistent implementation of IResaRedacDataList
*/
class XPGResaRedacDataList : public CPMUnknown<IResaRedacDataList>
{
public:
	/**
		Constructor.
		@param boss interface ptr from boss object on which this interface is aggregated.
	*/
	XPGResaRedacDataList(IPMUnknown* boss);
	
	/** Destructor
	 */
	virtual ~XPGResaRedacDataList() {};

	virtual void Clear();
	virtual void AddResaRedac(const PMString& idForme, const ResaRedac& resaRedac);
	virtual void RemoveResaRedac(const PMString& idForme);

	virtual const ResaRedac& GetResaRedac(const PMString& idForme) const;	
	virtual const PMString& GetIDForme(int32 resaRedacID) const;

	virtual int32 GetNbResaRedac();
	virtual const ResaRedac& GetNthResaRedac(int32 nth, PMString& idForme) const;

	virtual void CopyFrom(IResaRedacDataList* otherList);

	/** This is a persistent implementation
	*/
	void ReadWrite(IPMStream *  s, ImplementationID  prop);

private:

	std::map<PMString, ResaRedac> formeToResaMap;
	std::map<int32, PMString> resaIDToFormeMap;
};

/* Makes the implementation available to the application.
*/
CREATE_PERSIST_PMINTERFACE(XPGResaRedacDataList, kXPGResaRedacDataListImpl)

/* Constructor
*/
XPGResaRedacDataList::XPGResaRedacDataList(IPMUnknown* boss) : CPMUnknown<IResaRedacDataList>(boss)
{
	formeToResaMap.clear();
	resaIDToFormeMap.clear();
}

void XPGResaRedacDataList::Clear()
{
	PreDirty(kXPGResaRedacDataListImpl);
	formeToResaMap.clear();
	resaIDToFormeMap.clear();
}

void XPGResaRedacDataList::AddResaRedac(const PMString& idForme, const ResaRedac& resaRedac)
{
	PreDirty(kXPGResaRedacDataListImpl);

	std::map<PMString, ResaRedac>::iterator result = formeToResaMap.find(idForme);
	if(result == formeToResaMap.end())
	{
		formeToResaMap.insert(std::make_pair(idForme, resaRedac));
		resaIDToFormeMap.insert(std::make_pair(resaRedac.id, idForme));
	}
	else
	{
		int32 oldResaRedacID = result->second.id;
		result->second = resaRedac;

		if(oldResaRedacID != resaRedac.id)
		{
			resaIDToFormeMap.insert(std::make_pair(resaRedac.id, idForme));

			std::map<int32, PMString>::iterator result2 = resaIDToFormeMap.find(oldResaRedacID);
			if(result2 != resaIDToFormeMap.end())
				resaIDToFormeMap.erase(result2);
		}
	}	
}

void XPGResaRedacDataList::RemoveResaRedac(const PMString& idForme)
{
	std::map<PMString, ResaRedac>::iterator result = formeToResaMap.find(idForme);
	if(result != formeToResaMap.end())
	{
		PreDirty(kXPGResaRedacDataListImpl);

		ResaRedac resaRedac = result->second;
		formeToResaMap.erase(result);

		std::map<int32, PMString>::iterator result2 = resaIDToFormeMap.find(resaRedac.id);
		if(result2 != resaIDToFormeMap.end())
			resaIDToFormeMap.erase(result2);
	}
}

const ResaRedac& XPGResaRedacDataList::GetResaRedac(const PMString& idForme) const
{
	std::map<PMString, ResaRedac>::const_iterator result = formeToResaMap.find(idForme);
	if(result != formeToResaMap.end())
		return result->second;

	return kInvalidResaRedac;
}

int32 XPGResaRedacDataList::GetNbResaRedac()
{
	return formeToResaMap.size();
}

const ResaRedac& XPGResaRedacDataList::GetNthResaRedac(int32 nth, PMString& idForme) const
{
	idForme = kNullString;

	if(nth < formeToResaMap.size())
	{
		std::map<PMString, ResaRedac>::const_iterator result = formeToResaMap.begin();
		std::advance(result, nth);

		idForme = result->first;
		return result->second;
	}

	return kInvalidResaRedac;
}

const PMString& XPGResaRedacDataList::GetIDForme(int32 resaRedacID) const
{
	std::map<int32, PMString>::const_iterator result = resaIDToFormeMap.find(resaRedacID);
	if(result != resaIDToFormeMap.end())
		return result->second;

	return kNullString;
}

void XPGResaRedacDataList::CopyFrom(IResaRedacDataList* otherList)
{
	XPGResaRedacDataList* otherImpl = static_cast<XPGResaRedacDataList*>(otherList);

	if(otherImpl)
	{
		PreDirty(kXPGResaRedacDataListImpl);
		formeToResaMap = otherImpl->formeToResaMap;
		resaIDToFormeMap = otherImpl->resaIDToFormeMap;
	}
}

void XPGResaRedacDataList::ReadWrite(IPMStream *  s, ImplementationID  prop)
{
	int32 nbResa = formeToResaMap.size();

	s->XferInt32(nbResa);

	if(s->IsReading())
	{
		formeToResaMap.clear();
		resaIDToFormeMap.clear();

		for(int32 i = 0 ; i < nbResa ; ++i)
		{
			PMString idForme;
			ResaRedac resa;
			idForme.ReadWrite(s);
			s->XferInt32(resa.id);
			resa.topic.ReadWrite(s);
			s->XferInt32(resa.nbSigns);
			s->XferInt16(resa.nbImages);
			resa.comment.ReadWrite(s);
			formeToResaMap.insert(std::make_pair(idForme, resa));
			resaIDToFormeMap.insert(std::make_pair(resa.id, idForme));
		}
	}
	else
	{
		for(std::map<PMString, ResaRedac>::iterator iter = formeToResaMap.begin() ; iter != formeToResaMap.end() ; ++iter)
		{
			PMString(iter->first).ReadWrite(s);
			s->XferInt32(iter->second.id);
			iter->second.topic.ReadWrite(s);
			s->XferInt32(iter->second.nbSigns);
			s->XferInt16(iter->second.nbImages);
			iter->second.comment.ReadWrite(s);
		}
	}
}

// End XPGResaRedacDataList.cpp


