//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NSHUNSPELL_H
#define __NSHUNSPELL_H

#include <owl/module.h>

#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSResModule ;
	#else
		static TModule *pResMod ;
		TModule *pNSResModule ;
	#endif
#else
	extern TModule *pNSResModule ;
#endif

#endif  // __NSHUNSPELL_H