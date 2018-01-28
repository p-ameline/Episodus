// -----------------------------------------------------------------------------
// nsmt_trdrugs.h
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2015/02/07 14:49:41 $
// -----------------------------------------------------------------------------
// Turkish drug database mapping
// -----------------------------------------------------------------------------
// PA - June 2006
// -----------------------------------------------------------------------------

#ifndef __NSMXP_SOAP_H__
#define __NSMXP_SOAP_H__

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

