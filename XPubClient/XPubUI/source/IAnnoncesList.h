
#ifndef __IAnnoncesList_h__
#define __IAnnoncesList_h__

#include "XPBCID.h"
#include "K2Vector.h"
#include "K2Vector.tpp"

class IPMUnknown;

// Structure pour les reserves de pub
typedef struct annoncedata
{
	typedef object_type data_type; // Pour l'utiliser avec des K2Vector

	PMString ID;
	PMString Status;
	PMString Name;
    
	annoncedata()
	{
		ID = kNullString;
		Status = kNullString;
		Name = kNullString;
	}

	inline bool operator==  (const struct annoncedata& uneAnnonce) const
	{
		return ( this->ID.Compare(kFalse, uneAnnonce.ID) == 0);
	} 
	
} ResaPub;


/** Provides get and set methods to handle a list of ResaPub
*/
class IAnnoncesList : public IPMUnknown
{

	public:	

		enum	{ kDefaultIID = IID_IANNONCESLIST };

		virtual void SetAnnoncesList(const K2Vector<annoncedata>& newList) = 0;
		virtual K2Vector<annoncedata> GetAnnoncesList() = 0;
};

#endif // __IAnnoncesList_h__
