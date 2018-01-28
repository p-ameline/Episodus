//----------------------------------------------------------------------------
// EPISODUS
//----------------------------------------------------------------------------

#ifndef __NS_OCR_H
#define __NS_OCR_H

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

#endif  // __NS_OCR_H
