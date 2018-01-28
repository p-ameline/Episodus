// -----------------------------------------------------------------------------
// nsmt_trdrugs.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: philippe $
// $Date: 2004/03/10 16:19:24 $
// -----------------------------------------------------------------------------
// Turkish drug database mapping
// -----------------------------------------------------------------------------
// PA - June 2006
// -----------------------------------------------------------------------------

#ifndef __NS_TRDRUGS_H__
#define __NS_TRDRUGS_H__

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

#endif    // __NS_TRDRUGS_H__

