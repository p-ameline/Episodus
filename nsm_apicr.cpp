//---------------------------------------------------------------------------
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

#define _MAIN
#define __MAIN

#include "ns_crypt\ns_crypt.h"

USEUNIT("apicrypt\ns_api_dll.cpp");
USELIB("nsmutil.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USERES("nsm_apicr.res");
//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinstDll, unsigned long fdwReason, void*)
{
  switch (fdwReason)
  {
    case DLL_THREAD_ATTACH :
    case DLL_THREAD_DETACH :
      // cas des threads : on ne fait rien
      break ;

    case DLL_PROCESS_ATTACH :
      // 1er appel : on instancie pResMod
      if (NULL == pResMod)
        pResMod = new TModule(0, hinstDll) ;

      // si le new echoue : on renvoie false
      if (NULL == pResMod)
        return 0 ;

      pNSDLLModule = pResMod ;
      break ;

    case DLL_PROCESS_DETACH :
      // dernier appel : on libere pResMod
      if (pResMod)
      {
        delete pResMod ;
        pResMod = (TModule*) 0 ;
      }
      break ;

    default :
      return 0 ;
  }

	return 1 ;
}

