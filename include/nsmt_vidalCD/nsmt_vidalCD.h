// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: fabrice $
// $Date: 2003/07/10 09:51:55 $
// -----------------------------------------------------------------------------
// FLP - mai 2003
// -----------------------------------------------------------------------------
// DLL de récupération de données médicamenteuses à partir du VIDAL
// -----------------------------------------------------------------------------
#ifndef __NSMT_VIDALCD_H__
# define __NSMT_VIDALCD_H__

# include <bwcc.h>

#include "nautilus\nssuper.h"

void _export nsmtoolsInformation(string far *psToolName, string far *psDescription) ;
void _export nsmToolsAction(TWindow far *pere, NSContexte far *pCtx) ;

#ifdef _MAIN
	#ifndef __MAIN		extern TModule *pNSResModule;
	#else
		static TModule *pResMod;
		TModule *pNSResModule;
	#endif
#else
	extern TModule *pNSResModule;
#endif

#endif  // __NS_VIDAL_H__
