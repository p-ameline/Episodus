// DLL EPISODUS
///////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "partage\nsglobal.h"
#include "nsepisod\nsepisod.h"

USEUNIT("nsepisod\eptables.cpp");
USEUNIT("nsepisod\nsepidiv.cpp");
USEUNIT("nsepisod\nssoapdiv.cpp");
USEUNIT("nsepisod\nsclasser.cpp");
USEUNIT("nsepisod\nsCoopChart.cpp");
USEUNIT("nsepisod\nsPrediDlg.cpp");
USEUNIT("nsepisod\flechiesDB.cpp");
USEUNIT("nsepisod\objectif_viewer.cpp");
USEUNIT("nsepisod\nspatdlgknl.cpp");
USEUNIT("nsepisod\nsToDo.cpp");
USEUNIT("nsepisod\NsPapyrus.cpp");
USEUNIT("nsepisod\NsMailManager.cpp");
USEUNIT("nsepisod\NsCsvParser.cpp");
USEUNIT("nsepisod\nsPilotProxy.cpp");
USERC("nsepisod\nspredi.rc");
USERC("nsepisod\nsepidiv.rc");
USERC("nsepisod\objectif_viewer.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("nsmldv.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("nspr4.lib");
USELIB("nsmdecode.lib");
USERES("nsmepisod.res");
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

