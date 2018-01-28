// DLL DE CAPTURE D'IMAGES
//////////////////////////////////////////////////
#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "ns_grab\ns_grab.h"

//---------------------------------------------------------------------------
USEUNIT("ns_grab\nsgrabfc.cpp");
USEUNIT("ns_grab\nsgrabtwain.cpp");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("nsmutil.lib");
USELIB("mil.lib");
USELIB("milcronos.lib");
USELIB("nsm_sgbd.lib");
USELIB("FreeImage.lib");
USELIB("eztw32.lib");
USELIB("nsmsavoir.lib");
USERES("nsm_grab.res");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinstDll, unsigned long fdwReason, void*)
{
    switch (fdwReason)
    {
        case DLL_THREAD_ATTACH :
        case DLL_THREAD_DETACH :
      	    // cas des threads : on ne fait rien
      	    break;

        case DLL_PROCESS_ATTACH :
      	    // 1er appel : on instancie pResMod
      	    if (!pResMod)
   			    pResMod = new TModule(0, hinstDll);

            // si le new echoue : on renvoie false
      	    if (!pResMod)
      		    return 0;

            pNSResModule = pResMod;

            break;

        case DLL_PROCESS_DETACH :
      	    // dernier appel : on libere pResMod
       	    if (pResMod)
      	    {
      		    delete pResMod;
         	    pResMod = 0;
      	    }
            break;

        default :
      	    return 0;
    }

	return 1;
}

// fin de ns_grab.cpp
