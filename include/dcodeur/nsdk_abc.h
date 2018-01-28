//----------------------------------------------------------------------------
// Entête commun à tous les décodeurs 
//----------------------------------------------------------------------------
//#include <windows.h>

#ifndef __NSDK_ABC_H
#define __NSDK_ABC_H

#include "nautilus\nssuper.h"
#include "nautilus\nscr_ama.h"
#include "nsbb\nspatpat.h"

// WIN32
bool _export dcodeKE(char Commande, NSPatPathoArray far *pPatPathoArray,
							char far *Fichier, NSContexte far *pCtx,
							NSCRPhraseArray far *pPhra);

#ifdef _MAIN
	#ifndef __MAIN
		extern TModule *pNSDLLModule;
	#else
		NSContexte* pContexte;
		static TModule *pResMod;
		TModule *pNSDLLModule;
	#endif
#else
	extern TModule *pNSDLLModule;
#endif

#endif  // #define __NSDK_ABC_H
