#ifndef _OWLDLL
#include "ControlPlanKB.h"
#include "BB1BB.h"
#include "BB1KSAR.h"       // for FavorControlKS(...)
#else
#include "ns_ob1\ControlPlanKB.h"
#include "ns_ob1\BB1BB.h"
#include "ns_ob1\BB1KSAR.h"       // for FavorControlKS(...)
#endif


#ifndef __TCONTROLER__
Rating FavorControlKS(const BB1KSAR& ksar)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: FavorControlKS(ksar) const\n\n" ;
#endif

  return (ksar.ControlKSARP() ? Rating(100.0) : Rating(0.0)) ;
}


void loadControlPlanKB(BB1BB& bb)
{

#ifdef _DEBUG_
  cout << "-- DEBUG:: loadControlPlanKB(bb) const\n\n" ;
#endif

  // The Control Plan kb is already defined
  /*
    adapted from bb1-working-memory.favor-control-focus in
    ~bb1/v3-2/src/bb1-working-memory-kb.lisp
  */
  bb.ProposeFocus("FavorControlKS",				// name
		  FavorControlKS,				// criterion
		  NULLGOAL,					// goal
		  true,						// StableP
		  true,						// OperativeP
		  WeightT(10),					// Weight
		  NULL /*NULLAVPSET*/,				// attSpec
		  NULL /*NULLLINKSET*/,				// linkSpec
		  "Favor KSes which modify the control plan."	// Descritpion
		  ) ;
}
#endif
