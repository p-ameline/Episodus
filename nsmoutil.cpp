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

USEUNIT("Nsoutil\Nsoutdlg.cpp");
USEUNIT("Nsoutil\NsEpiOut.cpp");
USEUNIT("Nsoutil\Nsfilgui.cpp");
USEUNIT("Nsoutil\Nsrechcr.cpp");
USEUNIT("partage\Nsmatfic.cpp");
USEUNIT("nsoutil\ns_arche.cpp");
USEUNIT("nsoutil\mexcel.cpp");
USEUNIT("nsoutil\NsUpdateDlg.cpp");
USEUNIT("nsoutil\NsInterfaces.cpp");
USEUNIT("nsoutil\nsrefedit.cpp");
USEUNIT("nsoutil\nsHtmlToPdf.cpp");
USEUNIT("curl\nsCurl.cpp");
USEUNIT("curl\nsRest.cpp");
USERC("Nsoutil\nsoutil.rc");
USERC("Nsoutil\nsEpiOut.rc");
USERC("Nsoutil\nsimport.rc");
USERC("partage\nsmatfic.rc");
USERC("nsoutil\ns_arche.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdecode.lib");
USELIB("nsmepisod.lib");
USELIB("nsmdn.lib");
USELIB("wkhtmltox.lib");
USELIB("libcurl-4.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USERES("nsmoutil.res");
USEUNIT("nsoutil\bdm_bases.cpp");
USEUNIT("nsoutil\interfaceBdm.cpp");
USEUNIT("nsoutil\nsBdmDlg.cpp");
USERC("nsoutil\nsBdmDlg.rc");
USEUNIT("nsoutil\nsdicobdm.cpp");
USEUNIT("nsoutil\NspathorBdm.cpp");
USEUNIT("nsoutil\nsBdmDrugInfoDlg.cpp");
USEUNIT("nsoutil\nsBdmInfoDlg.cpp");
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

/*
void
lancerOutils(NSContexte* pCtx)
{
	NsOutilDialog* pOutilDlg = new NsOutilDialog(pCtx->GetMainWindow(), pCtx);
	pOutilDlg->Execute();
}
*/
