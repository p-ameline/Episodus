// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2009/07/23 20:22:16 $
// -----------------------------------------------------------------------------
// Récupération des données concernant les bases VIDAL
// -----------------------------------------------------------------------------
// FLP - juillet 2003
// -----------------------------------------------------------------------------

#ifndef __NS_ARCHEDIT_H__
#define __NS_ARCHEDIT_H__

#include <bwcc.h>

#include "nautilus\nssuper.h"

void _export nsmtoolsInformation(string far *psToolName, string far *psDescription) ;
void _export nsmToolsAction(TWindow far *pere, NSContexte far *pCtx) ;

#ifdef _MAIN
    #ifndef __MAIN
        extern TModule    *pNSResModule ;
    #else   // __MAIN
        static TModule    *pResMod ;
        TModule           *pNSResModule ;
    #endif  // __MAIN
#else    // _MAIN
    extern TModule    *pNSResModule ;
#endif   // _MAIN

#endif    // __NS_ARCHEDIT_H__

