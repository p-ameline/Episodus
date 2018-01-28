//----------------------------------------------------------------------------
// KNOWLEDGE SOURCE
//----------------------------------------------------------------------------

#ifndef __NS_KS_H
# define __NS_KS_H

# include <bwcc.h>

# include "ns_bbk\nsbbk.h"

class __export NSKS
{
public:
// -----------------------------------------------------------------------------
// méthode
// -----------------------------------------------------------------------------

	NSKS(int	blocPhase = 0) ;

	~NSKS() { } ;

	bool		TriggerCondition(const BB1Event& event) ;
	bool		Precondition(const BB1BB& bb) ;
	bool		ObviationCondition(const BB1BB& bb) ;
	Errcode	Action(BB1BB& bb) ;

// -----------------------------------------------------------------------------
// variables
// -----------------------------------------------------------------------------

	int		iBlocPhase ;
} ;

bool    _export init(NBBControler* pCtrl);
bool    _export close();
bool    _export trigger_condition(const BB1Event& event);
bool    _export pre_condition(const NautilusBB& bb);
bool    _export obviated_condition(const NautilusBB& bb);
Errcode _export action(NautilusBB& bb);

# ifdef _MAIN
#  ifndef __MAIN
		extern TModule* pNSResModule;
#  else
		static TModule* pResMod;
		TModule*        pNSResModule;
//	NBBControler*  pControler;
#  endif
# else
	  extern TModule*         pNSResModule;
//  extern NBBControler*   pControler;
# endif

#endif  // __NS_KS_H
