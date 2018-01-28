//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __DONNESEXEKS_H__
# define __DONNESEXEKS_H__

# include <bwcc.h>

# include "ns_ob1\nsbbk.h"
#ifndef __TCONTROLER__
# include "KS\DPIOKS.h"
#endif
# include "ns_ob1\InitStructure.h"  // for KSInitStructure
# include "ns_ob1\abstractKS.h"     // for Abtract
# include "ns_ob1\Transfer.h"        // for TRansfert


class DonneSexeKS  : public AbstractKS
{
 public:
	DonneSexeKS() { iBlocPhase = -1 ; } ;
	~DonneSexeKS() { } ;

  bool                TriggerCondition(const BB1Event& event) ;

  KsInitStructure*    Publication();
  ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans);

  #ifdef __TCONTROLER__
  bool			          Precondition(const BB1BB& bb, Transfert* ksar) ;
  #else
	bool			          Precondition(const BB1BB& bb, BB1KSAR& ksar) ;
  #endif

	bool			          ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;

  #ifdef __TCONTROLER__
  Errcode		          Action(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;
  #else
	Errcode		          Action(BB1BB& bb, BB1KSAR& ksar) ;
  #endif

	int					iBlocPhase ;
} ;

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans);
bool                _export KSTriggerCondition(const BB1Event& event) ;

#ifdef __TCNOTROLER__
bool                _export KSPrecondition(const BB1BB& bb, BB1KSAR& ksar) ;
#else
bool                _export KSPrecondition(const BB1BB& bb, Transfert* ksar) ;
#endif

bool                _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;

#ifdef __TCONTROLER__
Errcode             _export KSAction(BB1BB& bb, Transfert* trans, bool isOpp) ;
#else
Errcode             _export KSAction(BB1BB& bb, BB1KSAR& ksar) ;
#endif

# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       		*pNSResModule ;
#  else
		 static TModule       		*pResMod ;
		 TModule              		*pNSResModule ;
		 DonneSexeKS     					ks ;
#  endif
# else
		 extern TModule       		*pNSResModule ;
		 extern DonneSexeKS				ks ;
# endif

#endif  // __DONNESEXEKS_H__
