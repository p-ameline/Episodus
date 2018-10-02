//nsdn.cpp : DLL de gestion des documents noyau (de base)
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
#include "nsdn\nsdn.h"

USEUNIT("nsdn\Nsdocnoy.cpp");
USEUNIT("nsdn\Nsdocum.cpp");
USEUNIT("nsdn\Nsdocinf.cpp");
USEUNIT("nsdn\nsintrad.cpp");
USEUNIT("nsdn\nsdn_dlg.cpp");
USEUNIT("nsdn\Nsdocdlg.cpp");
USEUNIT("nsdn\NsDocHis.cpp");
USEUNIT("nautilus\nsperary.cpp");
USEUNIT("nsdn\nsframedocs.cpp");
USERC("nsdn\nsdnres.rc");
USERC("nsdn\Nsdocdlg.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmldv.lib");
USELIB("nsmepisod.lib");
USELIB("nsmdecode.lib");
USELIB("nsm_ob1.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("ns_capture.lib");
USELIB("nsmoutil.lib");
USERES("nsmdn.res");

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
