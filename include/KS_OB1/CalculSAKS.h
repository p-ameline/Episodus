//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __CALCULSAKS_H__
# define __CALCULSAKS_H__

#include <bwcc.h>
#include "ns_ob1\nsbbk.h"
#include "ns_ob1\InitStructure.h"  // for KSInitStructure
#include "ns_ob1\abstractKS.h"     // for Abtract
#include "ns_ob1\Transfer.h"        // for TRansfert


class CalculSAKS : public AbstractKS
{
public:
// -----------------------------------------------------------------------------
// méthode
// -----------------------------------------------------------------------------


	CalculSAKS() { iBlocPhase = -1 ; } ;

	~CalculSAKS() { } ;

	bool			          TriggerCondition(const BB1Event& event) ;

  KsInitStructure*    Publication() ;
  ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;

  bool			          Precondition(const BB1BB& bb, Transfert* ksar) ;
	bool			          ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
  Errcode		          Action(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

	int		iBlocPhase ;
} ;

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans);
bool                _export KSTriggerCondition(const BB1Event& event) ;

#ifdef __TCONTROLER__
bool                _export KSPrecondition(const BB1BB& bb, Transfert* ksar) ;
#else
bool                _export KSPrecondition(const BB1BB& bb, BB1KSAR& ksar) ;
#endif

bool                _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;

#ifdef __TCONTROLER__
Errcode             _export KSAction(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;
#else
Errcode             _export KSAction(BB1BB& bb, BB1KSAR& ksar) ;
#endif

# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       		*pNSResModule ;
#  else
		 static TModule       		*pResMod ;
		 TModule              		*pNSResModule ;
		 CalculSAKS     					ks ;
#  endif
# else
		 extern TModule       		*pNSResModule ;
		 extern CalculSAKS				ks ;
# endif

#endif  // __CALCULSAKS_H__
