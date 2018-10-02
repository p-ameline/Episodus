//nscompta.cpp : DLL de comptabilité de Nautilus
/////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#define _MAIN
#define __MAIN

#include "partage\nsglobal.h"
#include "nscompta\nscompta.h"

USEUNIT("nscompta\NsCpta.cpp");
USEUNIT("nscompta\NsFSE.cpp");
USEUNIT("nscompta\NsFSE16.cpp");
USEUNIT("nscompta\NsDepens.cpp");
USEUNIT("nscompta\NsF16Dlg.cpp");
USEUNIT("nscompta\NsVitale.cpp");
USEUNIT("nscompta\NsAgaVar.cpp");
USEUNIT("nscompta\NsAgaDlg.cpp");
USEUNIT("nscompta\Nsimpdlg.cpp");
USEUNIT("nscompta\Nsactdlg.cpp");
USEUNIT("nscompta\Nscomdlg.cpp");
USEUNIT("nscompta\Nsfseuti.cpp");
USEUNIT("nscompta\Nsfsedlg.cpp");
USEUNIT("nscompta\Nsnoemie.cpp");
USEUNIT("nscompta\nsCptaRef.cpp");
USEUNIT("nautilus\nsperary.cpp");
USEUNIT("nautilus\nsmodhtm.cpp");
USEUNIT("nscompta\nsgendlg.cpp");
USEUNIT("nautilus\Nsadmiwd.cpp");
USERC("nscompta\nscompta.rc");
USERC("nscompta\nsdepens.rc");
USERC("nscompta\nsCptaRef.rc");
USERC("nautilus\nsresour.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("nsmldv.lib");
USELIB("nsmdecode.lib");
USELIB("nsmepisod.lib");
USELIB("ns_capture.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USERES("nsmcompta.res");

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
