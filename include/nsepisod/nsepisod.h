//----------------------------------------------------------------------------// EPISODUS
//----------------------------------------------------------------------------

#ifndef __NSEPISOD_H
#define __NSEPISOD_H

#include <bwcc.h>

#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSResModule;
	#else
		static TModule *pResMod;
		TModule *pNSResModule;
	#endif
#else
	extern TModule *pNSResModule;
#endif

#endif  // __NSEPISOD_H
