//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\applicat.h>
#include <owl\window.h>

#define _MAIN
#define __MAIN

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutdlg.h"

BOOL WINAPI DllEntryPoint(HINSTANCE hinstDll, DWORD fdwReason, LPVOID)
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
      		return FALSE;
            
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
      	return FALSE;
   }

	return TRUE;
}

void
lancerOutils(NSSuper* pSuper)
{
	NsOutilDialog* pOutilDlg = new NsOutilDialog(pSuper->pNSApplication->GetMainWindow(), pSuper);
	pOutilDlg->Execute();
}
