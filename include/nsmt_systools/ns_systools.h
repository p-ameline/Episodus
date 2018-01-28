// -----------------------------------------------------------------------------
// ns_vidal.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2011/03/04 16:53:24 $
// -----------------------------------------------------------------------------
// Récupération des données concernant les bases VIDAL
// -----------------------------------------------------------------------------
// FLP - juillet 2003
// -----------------------------------------------------------------------------

#ifndef __NS_VIDALRS_H__
#define __NS_VIDALRS_H__

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

#endif    // __NS_VIDALRS_H__

