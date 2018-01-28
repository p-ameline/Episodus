//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __DPIOCANCERPEAUKS_H__
# define __DPIOCANCERPEAUKS_H__

# include <bwcc.h>

# include "ns_bbk\nsbbk.h"
# include "KS\DPIOKS.h"


class DPIOCancerPeauKS : public DPIOKS
{
 public:
	DPIOCancerPeauKS() { iBlocPhase = -1 ; bDPIOLaunched = false ; bAlreadyAnswered = false ; } ;
	~DPIOCancerPeauKS() { } ;

	// DPIOKS functions
	bool			TriggerCondition(const BB1Event& event) ;
	bool			Precondition(const BB1BB& bb, BB1KSAR& ksar) ;
	bool			ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
	Errcode		Action(BB1BB& bb, BB1KSAR& ksar) ;
	bool			KSAgePrecondition(const BB1BB& bb) ;

	// auxiliary functions - precondition
	int				agePreconditionMelanome(const BB1BB& bb) ;
	int				agePreconditionEpithelioma(const BB1BB& bb) ;
//    bool            answertrueMelanome(const BB1BB& bb, string sPath) ;
//    bool            answertrueEpithelioma(const BB1BB& bb, string sPath) ;
	int				answerfalse(const BB1BB& bb, string sPath) ;
} ;

bool              _export init(string *pInit) ;
KsInitStructure*  _export Publication() ;
bool              _export KSTriggerCondition(const BB1Event& event) ;
bool              _export KSPrecondition(const BB1BB& bb, BB1KSAR& ksar) ;
bool              _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode           _export KSAction(BB1BB& bb, BB1KSAR& ksar) ;

#ifndef __TCONTROLER__
bool		_export TheKSAgePrecondition(const BB1BB& bb) ;
#endif

# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       		*pNSResModule ;
#  else
		 static TModule       		*pResMod ;
		 TModule              		*pNSResModule ;
		 DPIOCancerPeauKS        	ks ;
#  endif
# else
		 extern TModule       		*pNSResModule ;
		 extern DPIOCancerPeauKS	ks ;
# endif

#endif  // __DPIOCANCERSEINKS_H__
