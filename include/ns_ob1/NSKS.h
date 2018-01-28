#ifndef __NSKS_H__
# define __NSKS_H__

#ifdef _ENTERPRISE_DLL
  #include "nspr.h"
#else
  #include <owl\module.h>
#endif

# include "ns_ob1\BB1types.h"

class NSKS
{
 public:
	NSKS() ;
	~NSKS() ;

	std::string					sLabel ;

#ifdef _ENTERPRISE_DLL
  PRLibrary           *pKSModule ;
#else
	TModule 						*pKSModule ;
#endif

#ifdef __TCONTROLER__
  PublicationT        tpublication ;
  ValidityT           tvalidity ;
#endif

#ifndef __TCONTROLER__
  TriggerConditionT 	tCondition ;
  ObviationConditionT	oCondition ;
#endif

	PreconditionT				pCondition ;
	ActionT							action ;
// -----------------------------------------------------------------------------
// DPIO add
	APConditionT				apCondition ;
// -----------------------------------------------------------------------------
} ;

#endif // __NSKS_H__

