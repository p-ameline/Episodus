// -----------------------------------------------------------------------------
// nsmclient.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.31 $
// $Author: philippe $
// $Date: 2005/05/23 14:41:38 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------


// #include <owl\owlpch.h>
#define WINVER  0x0500

#include <vcl.h>
#pragma hdrstop

USEUNIT("docking\Dockinge.cpp");
USEUNIT("nautilus\nscompub.cpp");
USEUNIT("nautilus\ns_html.cpp");
USEUNIT("nautilus\nsbrowse.cpp");
USEUNIT("nautilus\nsGrabObjects.cpp");
USEUNIT("nautilus\nsbrowseSvce.cpp");
USEFORM("nautilus\ActiveX.cpp", WebProxy);
USEFORM("nautilus\Compos.cpp", WebCompos);
USEFORM("nautilus\Import.cpp", WebImport);
USEFORM("nautilus\log_form.cpp", WebLog);
USEFORM("nautilus\Lettre.cpp", LettreType);
USEFORM("nautilus\EMail.cpp", MailBox);
USEFORM("nautilus\Nssmtp.cpp", ControlSMTP);
USEFORM("nautilus\ImpImg.cpp", ImportImg);
USEUNIT("nautilus\Nsdocref.cpp");
USEUNIT("nautilus\nsmodhtm.cpp");
USEUNIT("nautilus\NstriHis.cpp");
USEUNIT("nautilus\nshistdo.cpp");
USEUNIT("nautilus\nsvisual.cpp");
USEUNIT("nautilus\Nsttx.cpp");
USEUNIT("nautilus\Nschoisi.cpp");
USEUNIT("nautilus\nspatdlg.cpp");
USEUNIT("nautilus\NsHistor.cpp");
USEUNIT("nautilus\nsvueref.cpp");
USEUNIT("nautilus\Nsdocaga.cpp");
USEUNIT("nscompta\NsAgaVar.cpp");
USEUNIT("nautilus\Nsdocview.cpp");
USEUNIT("nautilus\Nscrdoc2.cpp");
USEUNIT("nautilus\Nscrvue.cpp");
USEUNIT("nautilus\nscsdoc.cpp");
USEUNIT("nautilus\nscsvue.cpp");
USEUNIT("nautilus\nsldvdoc.cpp");
USEUNIT("nautilus\nsldvvue.cpp");
USEUNIT("nautilus\nsldvvuetech.cpp");
USEUNIT("nautilus\nsldvvar.cpp");
USEUNIT("nautilus\Nsrechdl.cpp");
USEUNIT("nautilus\Nsrechd2.cpp");
USEUNIT("nautilus\Nsannexe.cpp");
USEUNIT("nautilus\nsperary.cpp");
USEUNIT("nautilus\Nstxstyl.cpp");
USEUNIT("nautilus\Nstxstar.cpp");
USEUNIT("nautilus\Nscr_anx.cpp");
USEUNIT("nautilus\Nscr_ana.cpp");
USEUNIT("nautilus\nscr_amx.cpp");
USEUNIT("nautilus\Nscr_ama.cpp");
USEUNIT("nautilus\Nsepicap.cpp");
USEUNIT("nautilus\nsSOAPview.cpp");
USEUNIT("nautilus\nscaptview.cpp");
USEUNIT("nautilus\Nssuper.cpp");
USEUNIT("nsutil\nscache.cpp");
USERC("nautilus\nautilus.rc");
USERC("nautilus\nauti_icons.rc");
USERC("nautilus\nstypdoc.rc");
USERC("nautilus\nshisto.rc");
USERC("nautilus\Ns_start.rc");
USERC("nautilus\nsresour.rc");
USERC("nautilus\ns_cr.rc");
USERC("nautilus\printer.rc");
USERC("nautilus\inputdia.rc");
USERC("nautilus\nsepisod.rc");
USERC("nautilus\NsSOAP.rc");
USERC("nautilus\NsMailBox.rc");
USELIB("nsmutil.lib");
USELIB("nsm_sgbd.lib");
USELIB("nsmsavoir.lib");
USELIB("nsmbb.lib");
USELIB("nsmdecode.lib");
USELIB("nsmdn.lib");
USELIB("nsmepisod.lib");
USELIB("nsmoutil.lib");
USELIB("nsmcompta.lib");
USELIB("nsm_apicr.lib");
USELIB("memmgr.lib");
USELIB("cp32mti.lib");
USELIB("bidsvi.lib");
USELIB("owlwvi.lib");
USELIB("ocfwvi.lib");
USELIB("jvm.lib");
USELIB("psapi.lib");
USELIB("kernel32.lib");
USELIB("libcurl-4.lib");
USEUNIT("adopt.cpp");
USEUNIT("nautilus\wordbasic.cpp");
USEFORM("nautilus\AutoWord.cpp", AutoWordForm);
USEUNIT("EnumProc.cpp");
USEUNIT("nautilus\nsepisodview.cpp");
USERC("nautilus\nsldvvue.rc");
USEUNIT("nautilus\nsldvgoal.cpp");
USEUNIT("nautilus\nscqdoc.cpp");
USEUNIT("nautilus\nscqvue.cpp");
USEUNIT("nautilus\naumain.cpp");
USEUNIT("nautilus\nsdrugview.cpp");
USEUNIT("nautilus\nsgoalview.cpp");
USEUNIT("nautilus\nsprocessview.cpp");
USEUNIT("nautilus\nsprocess.cpp");
USEUNIT("nautilus\nsbackup.cpp");
USEUNIT("nautilus\nsupdate.cpp");
USEUNIT("curl\nscurl.cpp");
USEUNIT("nautilus\nsrcview.cpp");
USEUNIT("nautilus\nsaddfunctions.cpp");
USERC("nautilus\fonctions.rc");
USEUNIT("nautilus\WebBrowserPrint.cpp");
USEUNIT("partage\nsstrcore.cpp");
USEUNIT("partage\nsstrex.cpp");
USEUNIT("partage\nswinstr.cpp");
USERES("nsmclient.res");
USELIB("nsm_ob1.lib");
USEFORM("export\AutoWordExp.cpp", AutoWordFormEx);
USEUNIT("export\html2word.cpp");
USELIB("ns_capture.lib");
USEFORM("AcroReader.cpp", AcrobatProxy);
USEUNIT("nautilus\nsAcroRead.cpp");
USEFORM("nautilus\WebService.cpp", WebServiceForm);
USEUNIT("nautilus\nsMailSvce.cpp");
USEUNIT("nautilus\nsTeamDocView.cpp");
USEUNIT("nautilus\nsFollowUpView.cpp");
USEUNIT("nautilus\nsVenueDocView.cpp");
USEUNIT("nautilus\NsInterface.cpp");
USEUNIT("nautilus\nsCoopChartDV.cpp");
USEUNIT("docking\stripeDock.cpp");
USERC("docking\stripes_icons.rc");
USEUNIT("nautilus\decisionTreeDV.cpp");
USEUNIT("nautilus\wordObjects.cpp");
USEUNIT("nautilus\nsPilotProxy.cpp");
USEFORM("include\nautilus\ExamHisto.cpp", ExamHistoForm);
USEFORM("nautilus\NsIndySmtp.cpp", IndySMTP);
//---------------------------------------------------------------------------
#include "nautilus\nssuper.h"
#include "ns_ob1\nautilus-bbk.h"
#include "nautilus\nsupdate.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nsbackup.h"
#include "nsoutil\nsupdatedlg.h"
#include "nsoutil\nsrechcr.h"
#include "nsoutil\nsEpiOut.h"
#include "partage\nsdivfile.h"

#include <checks.h>

DIAG_DEFINE_GROUP(OwlApp, 1, 2) ;

int OwlMain(int argc, char* argv[])
{
  NSSuper* pNSSuperviseur = new NSSuper() ;
  NSContexte* pContexte = new NSContexte(pNSSuperviseur) ;

  // pNSSuperviseur->initInstanceCounters() ;    // now done in NSSuper constructor
  // pContexte->setSuperviseur(pNSSuperviseur) ; // now done in NSSuper constructor

  TMyApp myApp(pContexte) ;

  // Adapt base directory to exe directory
  //
  string sInitialBaseDirectory = pContexte->getBaseDirectory() ;

  string sCommand = string("") ;
  if (argc > 0)
  {
    sCommand = string(argv[0]) ;
    if (string("") != sCommand)
    {
      size_t iLastSlashPos = sCommand.find_last_of(FILESLASH) ;
      if (NPOS != iLastSlashPos)
      {
        string sExeDir = string(sCommand, 0, iLastSlashPos) ;

        if (sInitialBaseDirectory != sExeDir)
        {
          pContexte->setBaseDirectory(sExeDir) ;
          pContexte->reallocateBaseDirectory() ;

          // Need to reload localisation string
          //
          pNSSuperviseur->loadLocalisationString("") ;
        }
      }
    }
  }

  // Remise à zéro du fichier de trace - reseting trace file
  //
	string sTraceFileName ;
	if (pNSSuperviseur->getInstance() > 1)
		sTraceFileName = string("traceNau") + pNSSuperviseur->getInstanceString() + string(".inf") ;
  else
  	sTraceFileName = string("traceNau.inf") ;
  if (NsFileExists(sTraceFileName))
    DeleteFile(sTraceFileName.c_str()) ;
  if (NsFileExists("errorPilot.log"))
    DeleteFile("errorPilot.log") ;

  // Reseting blackboard trace file
  //
  string sBbkTraceFileName = string("traceBBK.inf") ;
  if (NsFileExists(sBbkTraceFileName))
    DeleteFile(sBbkTraceFileName.c_str()) ;

  string ps = string("Starting, version ") + pNSSuperviseur->sNumVersion ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trWarning) ;

  ps = string("Command: ") + sCommand ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trWarning) ;

  ps = string("Initial base directory: ") + sInitialBaseDirectory ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trWarning) ;

  string sBaseDirectory = pContexte->getBaseDirectory() ;
  ps = string("Current base directory: ") + sBaseDirectory ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trWarning) ;

  pNSSuperviseur->parseExecArgs(argc, argv) ;

  // Better not use a local object because it would be destroyed after
  // supervisor and would spoil objects counter
  //
  ps = string("Checking for update.") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trWarning) ;

	NSUpdate* pUpdater = new NSUpdate(pContexte) ;
  if (pUpdater->init() && pUpdater->mustCheckForUpdateAvailability() &&
    	pUpdater->isUpdateAvailable())
	{
    TSplashWindow* pSplash = 0 ;
    if (myApp.getSplashWindow())
    	pSplash = myApp.getSplashWindow() ;

/*
  	string MsgHdr = pContexte->getSuperviseur()->getText("updateManagement", "updateManagement") ;
  	string MsgTxt = pContexte->getSuperviseur()->getText("updateManagement", "aMoreRecentVersionExistsDoYouWantToInstallIt") ;
    int retVal = ::MessageBox(hWnd, MsgTxt.c_str(), MsgHdr.c_str(), MB_YESNO) ;
*/

try
{
    pUpdater->setWaitingForUpdate(true) ;

    // NsUpdateYesNoDialog* pUpdate = new NsUpdateYesNoDialog((TWindow*)pSplash, pContexte, &updater) ;
    // int retVal = pUpdate->Execute() ;

    //
    if (false == pUpdater->mustForceUpdate())
    {
      NsUpdateYesNoWindow* pUpdate = new NsUpdateYesNoWindow((TWindow*)pSplash, pContexte, pUpdater) ;
      pUpdate->Create() ;

      while (pUpdater->getWaitingForUpdate())
        myApp.PumpWaitingMessages() ;
    }
    else
      pUpdater->setWantUpdate(true) ;

    if (pUpdater->getWantUpdate())
    {
    	// string splashMsg = pContexte->getSuperviseur()->getText("updateManagement", "downloadingUpdate") ;
    	// myApp.sendMessageToSplash(splashMsg) ;

      NsDownloadWindow* pDownload = new NsDownloadWindow((TWindow*)pSplash, pContexte, pUpdater) ;
      pDownload->Create() ;

    	if (pUpdater->startUpdate(sBaseDirectory))
      {
        delete pUpdater ;
        delete pContexte ;
        delete pNSSuperviseur ;
      	return -1 ;
      }

      pDownload->Destroy() ;
    }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception Main : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception Main.", standardError, 0) ;
}
  }
  
  delete pUpdater ;

  myApp.createSplash() ;

  if (false == pNSSuperviseur->InitialiseDatabase(pContexte))
  	return -1 ;

  pNSSuperviseur->InitialiseJava() ;
  if (false == pNSSuperviseur->isJavaOk())
  {
    ps = string("Java initialization failed, exit") ;
    pNSSuperviseur->trace(&ps, 1, NSSuper::trError) ;
    return -1 ;
  }

  ps = string("Java initialization succeeded, creating trSteps") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trSteps) ;

  pNSSuperviseur->creePilot() ;

//#ifndef __OB1__

  ps = string("Igniting the Blackboard") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trSteps) ;

  runBlackboard(pContexte) ;

  ps = string("Blackboard started, running application") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trSteps) ;

//#endif

	// TMyApp myApp(pContexte) ;

  int iRun = myApp.Run() ;

  ps = string("Application stopped, closing main objects") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trSteps) ;

  // If user object still exists, it's time to delete it
  // Détruit l'utilisateur s'il existe encore.
  //
  NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
	if (pUser)
  {
#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
    NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
    if (pEpisodus && pEpisodus->bAutoSave)
    {
      // NSPrometheFile PrometCfg(pContexte) ;
      // PrometCfg.EcrireFichier(pUser->getID()) ;
      pEpisodus->saveParams(pUser->getID()) ;
    }
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__

    string sMsg = string("Deleting the user") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

    delete pUser ;
    pContexte->setUtilisateur((NSUtilisateurChoisi*) 0) ;
	}

  myApp.PumpWaitingMessages() ;

  #ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
#  ifdef __OB1__

  closeBlackboard(pContexte) ;

#  endif
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__

  // Delete context
  //
  pContexte->reallocateBaseDirectory() ;
  delete pContexte ;

  // Close supervisor related tables
  //
  pNSSuperviseur->DeleteLocalDatabaseObjects() ;
  pNSSuperviseur->CheckZeroObjectCount(string("NSFiche"), NSFiche::getNbInstance()) ;

  // Close IDAPI engine and database.
  //
	pNSSuperviseur->FermeDatabase() ;

  // Backup
  //
  nsbackup(pNSSuperviseur) ;

  ps = string("Closing supervisor") ;
  pNSSuperviseur->trace(&ps, 1, NSSuper::trSteps) ;

  delete pNSSuperviseur ;

  return iRun ;

/*
try
{
  Registrar = new TOcRegistrar(AppReg, TOleDocViewFactory<TMyApp>(), OWL::TApplication::GetCmdLine(), ::DocTemplateStaticHead) ;
  if (Registrar->IsOptionSet(amAnyRegOption))
    return 0 ;

  // If this is an exe server normal run, run the app now. Otherwise, wait
  // until our factory gets a call (don't call us, we'll call you)
  return Registrar->Run() ;
}
catch (xmsg& x)
{
	::MessageBox(0, x.why().c_str(), "Exception", MB_OK);
}
	return -1;
*/
}
#define WinMain

