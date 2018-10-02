//nsoutil.cpp : DLL de migration de Nautilus V2 à Nautilus V3
//////////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutdlg.h"

USEUNIT("Nsldv\nsldvdoc_base.cpp");
USEUNIT("Nsldv\nsldvvue_base.cpp");
USEUNIT("Nsldv\nsldvdrug.cpp");
USEUNIT("Nsldv\nsldvgoal.cpp");
USEUNIT("Nsldv\nsldvuti.cpp");
USEUNIT("Nsldv\nsldvstructs.cpp");
USEUNIT("Nsldv\nsldvvar.cpp");
USEUNIT("Nsldv\nsldvvuetoons.cpp");
USEUNIT("Nsldv\nsldvvuetech.cpp");
USEUNIT("Nsldv\nsmdiframe_base.cpp");
USEUNIT("Nsldv\adopt.cpp");

USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdecode.lib");
USELIB("nsmdn.lib");
USELIB("nsmepisod.lib");
USELIB("nsmoutil.lib");
USELIB("nsm_ob1.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("nspr4.lib");

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
