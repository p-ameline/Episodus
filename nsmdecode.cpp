//nssavoir.cpp : DLL de gestion des bases de connaissance
///////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>
#include <vector.h>

#define _MAIN
#define __MAIN

#include "partage\nsglobal.h"
#include "dcodeur\nsdecode.h"

USEUNIT("dcodeur\nsdKd.CPP");
USEUNIT("dcodeur\Decoder.cpp");
USEUNIT("dcodeur\Nsphrase.cpp");
USEUNIT("dcodeur\NsGen.cpp");
USEUNIT("dcodeur\NsGenFr.cpp");
USEUNIT("dcodeur\NsGenEn.cpp");
USEUNIT("dcodeur\Nsphrobj.cpp");
USEUNIT("dcodeur\Decordon.cpp");
USEUNIT("partage\NsCIM10.cpp");
USEUNIT("nautilus\nsperary.cpp");
USEUNIT("dcodeur\nscr_amx.cpp");
USEUNIT("dcodeur\Nscr_ama.cpp");
USEUNIT("dcodeur\Nscr_anx.cpp");
USEUNIT("dcodeur\Nstxstyl.cpp");
USERC("dcodeur\nsdecode.rc");
USERC("partage\nsCIM10.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmoutil.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("nsmepisod.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("ns_capture.lib");
USERES("nsmdecode.res");
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

      	pNSDLLModule = pResMod;
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

