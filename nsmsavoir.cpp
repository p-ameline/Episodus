//nssavoir.cpp : DLL de gestion des bases de connaissance
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
#include "nssavoir\nssavoir.h"

//--------------------------------------------------------------------------

USEUNIT("nssavoir\Nsfilgd.cpp");
USEUNIT("nssavoir\Nsgraphe.cpp");
USEUNIT("nssavoir\NsVarray.cpp");
USEUNIT("nssavoir\Nspatbas.cpp");
USEUNIT("nssavoir\Nspatho.cpp");
USEUNIT("nssavoir\Nspathor.cpp");
USEUNIT("partage\Nsperson.cpp");
USEUNIT("nssavoir\Nsguide.cpp");
USEUNIT("nssavoir\nspatlnk.cpp");
USEUNIT("nssavoir\ns_fic.cpp");
USEUNIT("nssavoir\NSEncycl.cpp");
USEUNIT("nssavoir\NsLangue.cpp");
USEUNIT("nssavoir\Nstransa.CPP");
USEUNIT("nssavoir\ns_medic.cpp");
USEUNIT("nssavoir\NsConver.cpp");
USEUNIT("nssavoir\NsCaptur.cpp");
USEUNIT("nssavoir\NsLocalise.cpp");
USERC("nssavoir\nssavoir.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmbb.lib");
USELIB("nsmdn.lib");
USELIB("nsmdecode.lib");
USELIB("nsmepisod.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USEUNIT("nssavoir\nsRightsManager.cpp");
USEUNIT("nssavoir\nsHealthTeam.cpp");
USERC("nssavoir\healthteam.rc");
USEUNIT("nssavoir\nsHealthTeamInterface.cpp");
USEUNIT("nssavoir\nsHealthTeamMemberInterface.cpp");
USEUNIT("nssavoir\GraphicHealthTeam.cpp");
USEUNIT("nssavoir\RosaceDrawUtil.cpp");
USEUNIT("nssavoir\NSHealthTeamMember.cpp");
USEUNIT("nssavoir\NSHealthTeamPosition.cpp");
USEUNIT("nssavoir\NSHealthTeamMandate.cpp");
USELIB("ns_capture.lib");
USEUNIT("nssavoir\NsFileCaptur.cpp");
USERES("nsmsavoir.res");
USEUNIT("partage\ns_search.cpp");
USEUNIT("nssavoir\nsHtmBase.cpp");
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

