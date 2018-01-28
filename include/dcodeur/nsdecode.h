//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NSDK_DLL_H
#define __NSDK_DLL_H

#include <bwcc.h>

/*#include "nsbb\nsbbsmal.h"

BOOL _export BigBrother(NSSmallBrother *pBigBoss, char Commande,

																char *Repertoire, char *pCR);

*/



#ifdef _DKD_DLL

	#ifdef _MAIN

		#ifndef __MAIN

			extern TModule *pNSDLLModule;

			extern TModule *pNSResModule;

		#else

			static TModule *pResMod;

			TModule *pNSDLLModule;

			TModule *pNSResModule;

		#endif

	#else

		extern TModule *pNSDLLModule;

		extern TModule *pNSResModule;

	#endif

#endif



#endif

