//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __SCMGCANCERCOLONKS_H__
#define __SCMGCANCERCOLONKS_H__

#include <bwcc.h>

#include "ns_ob1\nsbbk.h"
#include "ns_ob1\InitStructure.h"  // for KSInitStructure
#include "ns_ob1\abstractKS.h"     // for Abtract
#include "ns_ob1\Transfer.h"        // for TRansfert


class SCMGCancerColonKS : public AbstractKS
{
  public:

    SCMGCancerColonKS() {}

	  bool			          TriggerCondition(const BB1Event& event) ;
    KsInitStructure*    Publication() ;
    ValidityContextType ContextValidity(const BB1BB& bb, Transfert* trans) ;
    bool			          Precondition(const BB1BB& bb, Transfert* trans) ;
	  bool			          ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
    Errcode		          Action(BB1BB& bb, Transfert* trans,bool isOpportuniste) ;

    int iBlocPhase ;
} ;

bool                _export init(string *pInit) ;
KsInitStructure*    _export Publication() ;
ValidityContextType _export ContextValidity(const BB1BB& bb, Transfert* trans) ;
bool                _export KSTriggerCondition(const BB1Event& event) ;
bool                _export KSPrecondition(const BB1BB& bb, Transfert* ksar) ;
bool                _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode             _export KSAction(BB1BB& bb, Transfert* trans, bool isOpportuniste) ;

# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       		*pNSResModule ;
#  else
		 static TModule       		*pResMod ;
		 TModule              		*pNSResModule ;
		 SCMGCancerColonKS        ks ;
#  endif
# else
		 extern TModule       		*pNSResModule ;
		 extern SCMGCancerColonKS	ks ;
# endif

#endif  // __SCMGCANCERCOLONKS_H__
