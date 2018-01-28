// -----------------------------------------------------------------------------
// ns_bbk.cpp : DLL de pilotage Blackboard
// -----------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <shellapi.h>
#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\owlpch.h>

#include <windows.h>

#define _MAIN
#define __MAIN

#include "nautilus\nssuper.h"
#include "ns_ob1\ns_bbk.h"
#include "ns_ob1\ns_bbkdlg.h"

#define _BBKINNAUTILUS_  1

HINSTANCE   hInstanceDll;

USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("nsmutil.lib");
USELIB("nsmbb.lib");
USELIB("nsmepisod.lib");
USELIB("nsmsavoir.lib");
USEUNIT("ns_ob1\AttValPair.cpp");
USEUNIT("ns_ob1\BB1API.cpp");
USEUNIT("ns_ob1\BB1APIconst.cpp");
USEUNIT("ns_ob1\BB1APIdef.cpp");
USEUNIT("ns_ob1\BB1APIfns.cpp");
USEUNIT("ns_ob1\BB1APIget.cpp");
USEUNIT("ns_ob1\BB1APIui.cpp");
USEUNIT("ns_ob1\BB1AppInst.cpp");
USEUNIT("ns_ob1\BB1BBam.cpp");
USEUNIT("ns_ob1\BB1BBcreate.cpp");
USEUNIT("ns_ob1\BB1BBcycle.cpp");
USEUNIT("ns_ob1\BB1BBeval.cpp");
USEUNIT("ns_ob1\BB1BBevent.cpp");
USEUNIT("ns_ob1\BB1BBfns.cpp");
USEUNIT("ns_ob1\BB1BBmmr.cpp");
USEUNIT("ns_ob1\BB1BBmsg.cpp");
USEUNIT("ns_ob1\BB1BBname.cpp");
USEUNIT("ns_ob1\BB1BBphase.cpp");
USEUNIT("ns_ob1\BB1Class.cpp");
USEUNIT("ns_ob1\BB1ClassIndex.cpp");
USEUNIT("ns_ob1\BB1Error.cpp");
USEUNIT("ns_ob1\BB1Event.cpp");
USEUNIT("ns_ob1\BB1Exception.cpp");
USEUNIT("ns_ob1\BB1Execution.cpp");
USEUNIT("ns_ob1\BB1KB.cpp");
USEUNIT("ns_ob1\BB1KS.cpp");
USEUNIT("ns_ob1\BB1KSKB.cpp");
USEUNIT("ns_ob1\BB1Link.cpp");
USEUNIT("ns_ob1\BB1List.cpp");
USEUNIT("ns_ob1\BB1Message.cpp");
USEUNIT("ns_ob1\BB1Object.cpp");
USEUNIT("ns_ob1\BB1Set.cpp");
USEUNIT("ns_ob1\BB1types.cpp");
USEUNIT("ns_ob1\ClassKB.cpp");
USEUNIT("ns_ob1\Controler.cpp");
USEUNIT("ns_ob1\Interface.cpp");
USEUNIT("ns_ob1\InterfaceExt.cpp");
USEUNIT("ns_ob1\Marker.cpp");
USEUNIT("ns_ob1\MarkSet.cpp");
USEUNIT("ns_ob1\nautilus-bbk.cpp");
USEUNIT("ns_ob1\OB1Agenda.cpp");
USEUNIT("ns_ob1\OB1Edge.cpp");
USEUNIT("ns_ob1\OB1Heuristix.cpp");
USEUNIT("ns_ob1\OB1Node.cpp");
USEUNIT("ns_ob1\OB1Strategy.cpp");
USEUNIT("ns_ob1\OB1Token.cpp");
USEUNIT("ns_ob1\Sheduler.cpp");
USEUNIT("ns_ob1\Tgraph.cpp");
USEUNIT("ns_ob1\TypedVal.cpp");
USEUNIT("ns_ob1\NautilusType.cpp");
USEUNIT("ns_ob1\BB1Task.cpp");
USEUNIT("ns_ob1\InitStructure.cpp");
USEUNIT("ns_ob1\Transfer.cpp");
USEUNIT("ns_ob1\toolbox.cpp");
USEUNIT("ns_ob1\AgentManager.cpp");
USEUNIT("ns_ob1\Agent.cpp");
USERC("ns_ob1\OB1.rc");
USERES("nsm_ob1.res");
USEUNIT("ns_ob1\NSKS.cpp");
//---------------------------------------------------------------------------
#ifdef _NSVIDAL
USELIB("ns_vidal.lib") ;
#endif

//---------------------------------------------------------------------------
int WINAPI DllEntryPoint(HINSTANCE hinstDll, unsigned long fdwReason, void*)
{
	hInstanceDll = hinstDll ;

	switch (fdwReason)
	{
		case DLL_THREAD_ATTACH :
		case DLL_THREAD_DETACH :
			// cas des threads : on ne fait rien
			break;

		case DLL_PROCESS_ATTACH :
			// 1er appel : on instancie pResMod
			if (!pResMod)
				pResMod = new TModule(0, hinstDll) ;

			// si le new echoue : on renvoie false
			if (!pResMod)
				return 0 ;

			pNSResModule = pResMod ;
				break ;

		case DLL_PROCESS_DETACH :
			// dernier appel : on libere pResMod
			if (pResMod)
			{
				delete pResMod ;
				pResMod = 0 ;
			}
			break;

		default :
			return 0 ;
	 }

	return 1 ;
}


// -----------------------------------------------------------------------------
// fonction d'écriture des logs
// -----------------------------------------------------------------------------

void    logBBKMessage(string sMessage)
{
	ofstream    outFile ;
	string      line ;
	string      sLastMessage ;
	time_t      t_current_time ;
	struct tm   *tm_current_time ;
	char        *s_current_time ;

	t_current_time = time(NULL) ;
	tm_current_time = localtime(&t_current_time);
	s_current_time = asctime(tm_current_time) ;
	// on ajoute le nouveau log
	sLastMessage = string(s_current_time, 0, strlen(s_current_time) - 1) + " : " + sMessage + "\n" ;

	// On écrit le nouveau fichier ns_bbk.log
	outFile.open("bbk.log", ios::app) ;
	if (!outFile)
	{
		// cas d'erreur bloquant...
		MessageBox(NULL, "Erreur d'ouverture en écriture du fichier bbk.log.", "Erreur NAUTILUS", MB_ICONEXCLAMATION) ;
		return ;
	}

//	for (unsigned int i = 0; i < strlen(sLastMessage.c_str()); i++)
	outFile.write(sLastMessage.c_str(), strlen(sLastMessage.c_str())) ;

  outFile.close() ;
}

