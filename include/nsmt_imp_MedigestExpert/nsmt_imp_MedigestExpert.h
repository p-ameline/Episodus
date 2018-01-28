//----------------------------------------------------------------------------// CAPTURE D'IMAGES//----------------------------------------------------------------------------
#ifndef __NSM_IMPORT_ME_H#define __NSM_IMPORT_ME_H
#include <bwcc.h>#include "nautilus\nssuper.h"
void _export nsmtoolsInformation(string far *psToolName, string far *psDescription) ;void _export nsmToolsAction(TWindow far *pere, NSContexte far *pCtx) ;#ifdef _MAIN
	#ifndef __MAIN		extern TModule *pNSResModule;
	#else
		static TModule *pResMod;
		TModule *pNSResModule;
	#endif
#else
	extern TModule *pNSResModule;
#endif

#endif  // __NSM_IMPORT_ME_H

