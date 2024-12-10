
#ifndef __IResaRedacData_h__
#define __IResaRedacData_h__

class PMString;
class IPMUnknown;

#include "XPGID.h"

/** define data structure for resa redac
*/
typedef struct ResaRedac {

	typedef object_type data_type;

	int32 id, nbSigns;
	PMString topic, comment;
	int16 nbImages;

	ResaRedac() : id(-1), nbSigns(0), topic(kNullString), comment(kNullString), nbImages(0) {}

	inline bool operator ==  (const struct ResaRedac& resa) const	{
		return ( this->id == resa.id );		
    }

	inline bool operator !=  (const struct ResaRedac& resa) const	{
		return !(*this == resa);		
    }

	inline bool operator< (const struct ResaRedac& resa) const
	{
		return(this->id < resa.id);
	}

} ResaRedac;

static const ResaRedac kInvalidResaRedac;

/**	data interface that stores resa redac data
*/
class IResaRedacDataList : public IPMUnknown
{
public:
	enum	{kDefaultIID = IID_IRESAREDACDATALIST};

	/** Setters 
	 */
	virtual void Clear() =0;
	virtual void AddResaRedac(const PMString& idForme, const ResaRedac& resaRedac) =0;
	virtual void RemoveResaRedac(const PMString& idForme) =0;

	virtual void CopyFrom(IResaRedacDataList* otherList) =0;

	/** Getters
	*/
	virtual const ResaRedac& GetResaRedac(const PMString& idForme) const =0;
	virtual const PMString& GetIDForme(int32 resaRedacID) const =0;
	
	virtual int32 GetNbResaRedac() =0;
	virtual const ResaRedac& GetNthResaRedac(int32 nth, PMString& idForme) const =0;
};

#endif // __IResaRedacData_h__

// End, IResaRedacDataList.h.



