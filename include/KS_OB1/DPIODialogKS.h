//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __DPIODIALOGKS_H__
# define __DPIODIALOGKS_H__

# include <bwcc.h>

# include "ns_bbk\nsbbk.h"
# include "KS\DPIOKS.h"


class DPIODialogKS : public DPIOKS
{
 public:
	DPIODialogKS()	{ } ;

	~DPIODialogKS() { } ;

	bool			TriggerCondition(const BB1Event& event) ;
	bool			Precondition(const BB1BB& bb, BB1KSAR& ksar) ;
	bool			ObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
	Errcode		Action(BB1BB& bb, BB1KSAR& ksar) ;

	int	    	searchInKB(const BB1BB& bb, string sDPIOitem) ;

	int				iColonConcerned ;
	int				iSeinConcerned ;
	int				iColConcerned ;
	int				iMelanomeConcerned ;
	int				iEpitheliomasConcerned ;
} ;


bool    _export init(string *pInit) ;
bool    _export KSTriggerCondition(const BB1Event& event) ;
bool    _export KSPrecondition(const BB1BB& bb, BB1KSAR& ksar) ;
bool    _export KSObviationCondition(const BB1BB& bb, BB1KSAR& ksar) ;
Errcode _export KSAction(BB1BB& bb, BB1KSAR& ksar) ;


# ifdef _MAIN
#  ifndef __MAIN
		 extern TModule       		*pNSResModule ;
#  else
		 static TModule       		*pResMod ;
		 TModule              		*pNSResModule ;
		 DPIODialogKS        			ks ;
#  endif
# else
		 extern TModule       		*pNSResModule ;
		 extern DPIODialogKS			ks ;
# endif

#endif  // __DPIODIALOGKS_H__
