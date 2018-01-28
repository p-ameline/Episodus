// -----------------------------------------------------------------------------
// ns_patbox.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: philippe $
// $Date: 2004/03/09 08:20:43 $
// -----------------------------------------------------------------------------
// Récupération des données concernant les bases VIDAL
// -----------------------------------------------------------------------------
// FLP - october 2010
// -----------------------------------------------------------------------------

#ifndef __NS_PATBOX_H__
#define __NS_PATBOX_H__

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

#endif    // __NS_PATBOX_H__

