//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NSCOMPTA_H
#define __NSCOMPTA_H

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

#endif