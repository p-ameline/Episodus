//----------------------------------------------------------------------------// CAPTURE D'IMAGES//----------------------------------------------------------------------------
#ifndef __NS_GRAB_H#define __NS_GRAB_H
#include <bwcc.h>
#ifdef _MAIN	#ifndef __MAIN		extern TModule *pNSResModule ;
	#else
		static TModule *pResMod ;
		TModule *pNSResModule ;
	#endif
#else
	extern TModule *pNSResModule ;
#endif

#endif  // __NS_GRAB_H
