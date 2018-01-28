/*
** bb1.h
** This header file contains the interface exported to applications
*/

#ifndef _bb1_h
# define _bb1_h 1

# include "AttValPair.h"	// includes "TypedVal.h"
# include "BB1BB.h"		// includes "BB1Agenda.h"
# include "BB1Class.h"		// includes "BB1AppInst.h"
# include "BB1Event.h"		// includes "BB1Object.h"
#ifndef __TCONTROLER__
# include "BB1Focus.h"		// includes "BB1Decision.h"
#endif
# include "BB1KB.h"		// includes "BB1types.h"
# include "BB1KS.h"		// includes "BB1Object.h"
#ifndef __TCONTROLER__
# include "BB1KSAR.h"		// includes "AttValPair.h" and "BB1Object.h"
#else
# include "BB1Object.h"
#endif
# include "BB1Link.h"		// includes "BB1types.h"
# include "BB1Message.h"	// includes "BB1Object.h"

#ifndef __TCONTROLER__
# include "BB1Strategy.h"	// includes "BB1Decision.h"
#endif

#endif // _bb1_h
