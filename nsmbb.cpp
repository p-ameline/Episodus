//nssavoir.cpp : DLL de gestion des bases de connaissance
///////////////////////////////////////////////////////////

#include <vcl.h>
#pragma hdrstop

#include <owl\applicat.h>
#include <owl\window.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>
#include <vector.h>

#define _MAIN
#define __MAIN

//using namespace std;

#include "partage\nsglobal.h"
#include "nsbb\nsbb.h"
#include "nsbb\nstrewi.h"

USEUNIT("nsbb\Nspatpat.cpp");
USEUNIT("nsbb\NspatBd.cpp");
USEUNIT("nsbb\Nsbbitem.cpp");
USEUNIT("nsbb\nsbbsmal.cpp");
USEUNIT("nsbb\Nsdlg.cpp");
USEUNIT("nsbb\Nsbb_dlg.cpp");
USEUNIT("nsbb\Nsbbtran.cpp");
USEUNIT("nsbb\Ns_Multi.cpp");
USEUNIT("nsbb\Nstrewi.cpp");
USEUNIT("nsbb\nstrnode.CPP");
USEUNIT("nsbb\Nsdico.cpp");
USEUNIT("nsbb\nsdicogl.cpp");
USEUNIT("nsbb\nsedilex.cpp");
USEUNIT("nsbb\Nsedit.cpp");
USEUNIT("nsbb\Nsbouton.cpp");
USEUNIT("nsbb\Nsonglet.cpp");
USEUNIT("nsbb\Nschkbox.cpp");
USEUNIT("nsbb\Nsgroupe.cpp");
USEUNIT("nsbb\Nsradiob.cpp");
USEUNIT("nsbb\Nsbitmap.cpp");
USEUNIT("nsbb\NSEdCon.cpp");
USEUNIT("nsbb\nspardlg.cpp");
USEUNIT("nsbb\Nsednum.cpp");
USEUNIT("nsbb\NSutiDlg.cpp");
USEUNIT("nsbb\Ns_skins.cpp");
USEUNIT("nsbb\NSTip.cpp");
USEUNIT("nsbb\ns_cisp.cpp");
USEFORM("nsbb\webTips.cpp", WebTip);
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmdn.lib");
USELIB("nsmdecode.lib");
USELIB("nsmepisod.lib");
USELIB("nsmoutil.lib");
USELIB("memmgr.lib");
USERC("nsbb\nsbb.rc");
USERC("nsbb\Nspathor.rc");
USERC("nsbb\nsdivdlg.rc");
USERC("nsbb\nsdrugdlg.rc");
USERC("nsbb\nsmPids.rc");
USERC("codeur\cod_bmp.rc");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USEUNIT("nsbb\nsarc.cpp");
USEUNIT("nsbb\nsattval.cpp");
USEUNIT("nsbb\nsarcParseError.cpp");
USEUNIT("parseur\attribut.cpp");
USEUNIT("parseur\valeur.cpp");
USEUNIT("parseur\balise.cpp");
USEUNIT("parseur\pre_parseur.cpp");
USERC("nsbb\nsbb_admin.rc");
USEUNIT("nsbb\nsattvalex.cpp");
USEUNIT("nsbb\nsattvaltools.cpp");
USEUNIT("nsbb\nstabwindow.cpp");
USEUNIT("nsbb\NSMView.cpp");
USEUNIT("nsbb\nsbbdivfct.cpp");
USEUNIT("nsbb\nsmanager.cpp");
USEUNIT("nsbb\NsRcDlg.cpp");
USEUNIT("nsbb\nsmpidsobj.cpp");
USEUNIT("nsbb\nsmedicdlg.cpp");
USEUNIT("nsbb\nscombo.cpp");
USEUNIT("nsbb\NslistWind.cpp");
USEUNIT("nsbb\nsPaneSplitter.cpp");
USEUNIT("nsbb\nsmediccycle.cpp");
USEUNIT("nsbb\nsmpids.cpp");
USEUNIT("nsbb\nsmedicphase.cpp");
USEUNIT("nsbb\nsmediccontrols.cpp");
USERC("nsbb\medicament.rc");
USEUNIT("nsbb\NSLabel.cpp");
USEUNIT("nsbb\nscomboutil.cpp");
USEUNIT("nsbb\NTListBox.cpp");
USEUNIT("nsbb\logpass.cpp");
USERC("nsbb\NSHealthTeam.rc");
USERC("nsbb\ns_cisp.rc");
USEUNIT("nsbb\NSHTTeamDialog.cpp");
USEUNIT("nsbb\NSHTMemberControls.cpp");
USERC("nsbb\logpass.rc");
USELIB("nsm_ob1.lib");
USELIB("ns_capture.lib");
USERES("nsmbb.res");
USEUNIT("nsbb\NSTipBrowser.cpp");
USEUNIT("nsbb\docViewMgr.cpp");
USEUNIT("nsbb\nsarcCtrls.cpp");
USEUNIT("nsbb\nsattval_glif.cpp");
USEUNIT("nsbb\nsfltbtnga.cpp");
USEUNIT("nsbb\toolhlpr.cpp");
USEUNIT("nsbb\NsTooltip.cpp");
USEUNIT("nsbb\nsMdiChild.cpp");
USEUNIT("nsbb\nsWhoIsThere.cpp");
USEUNIT("nsbb\NsRichEdit.cpp");
//---------------------------------------------------------------------------
int NSTreeWindow::TreeEditId = 500 ;

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

