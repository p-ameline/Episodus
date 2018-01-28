//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NS_SGBD_H
#define __NS_SGBD_H

#define _DANSSGBDDLL
#include <bwcc.h>

#ifdef _MAIN
	#ifndef __MAIN
		extern OWL::TModule *pNSResModule ;
	#else
		static OWL::TModule *pResMod ;
		       OWL::TModule *pNSResModule ;
	#endif
#else
	extern OWL::TModule *pNSResModule ;
#endif

#endif
