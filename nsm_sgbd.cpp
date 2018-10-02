//ns_sgbd.cpp : DLL de gestion des bases de données
////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "partage\nsglobal.h"
#include "ns_sgbd\ns_sgbd.h"

USEUNIT("ns_sgbd\Nsfiche.cpp");
USEUNIT("ns_sgbd\nschemin.cpp");
USEUNIT("ns_sgbd\nsrepare.cpp");
USEUNIT("ns_sgbd\nsbdeuti.cpp");
USEUNIT("ns_sgbd\ns_odbc.cpp");
USEUNIT("ns_sgbd\Nsannexe.cpp");
USEUNIT("pilot\JavaSystem.cpp");
USEUNIT("pilot\Pilot.cpp");
USEUNIT("pilot\NautilusPilot.cpp");
USERC("ns_sgbd\ns_sgbd.rc");
USELIB("nsmutil.lib");
USELIB("nsmbb.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmdn.lib");
USELIB("tutil32.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("jvm.lib");
USEUNIT("pilot\NSAgentTools.cpp");
USERC("pilot\agentList.rc");
USERES("nsm_sgbd.res");
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
//---------------------------------------------------------------------------
