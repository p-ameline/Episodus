//----------------------------------------------------------------------------// EPISODUS DLL de CAPTURE
//----------------------------------------------------------------------------

#ifndef __NSCAPTURE_H
#define __NSCAPTURE_H

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

#endif  // __NSCAPTURE_H

