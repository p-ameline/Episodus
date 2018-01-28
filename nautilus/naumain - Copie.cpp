// -----------------------------------------------------------------------------
// naumain.cpp
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.26 $
// $Author: pameline $
// $Date: 2012/12/18 21:55:08 $
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2001-2005
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

#include <ostream.h>
#include <checks.h>

#include "nautilus\EMail.h"
#include <owl\pch.h>
#include <owl\applicat.h>
#include <owl\decmdifr.h>

#include <owl\gdiobjec.h>
#include <owl\dialog.h>
#include <owl\controlb.h>
#include <owl\buttonga.h>
#include <owl\statusba.h>
#include <owl\docmanag.h>
#include <owl\docview.h>
#include <owl\olemdifr.h>
#include <owl\oleview.h>
#include <owl\oledoc.h>
#include <stdlib.h>
#include <string.h>
#include <bde.hpp>
#include <time.h>
#include <owl\printer.h>
#include <winuser.h>

#define __NAUTILUS_CPP

#include "nautilus\nautilus.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsepicap.h"
#include "partage\nsglobal.h"
#include <owl\docview.rc>
#include <owl\mdi.rh>
#include "nautilus\nautilus.rh"
#include "nautilus\nsresour.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nsrechd2.h"
#include "partage\nsmatfic.h"

#include "nautilus\nsvisual.h"
#include "nautilus\nsttx.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nscrvue.h"
#include "nautilus\nshistor.h"
#include "nautilus\nsadmiwd.h"
#include "nautilus\nsSOAPview.h"
#include "nscompta\nsagadlg.h"
#include "nscompta\nsnoemie.h"
#include "nscompta\nsdepens.h"
#include "nscompta\nsgendlg.h"
#include "nautilus\nsdocaga.h"
#include "nautilus\nsaddfunctions.h"
#include "nautilus\nscompub.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsldvdoc.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsmanager.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nslistwind.h"
#include "nautilus\nsbrowseSvce.h"
#include "nautilus\nsPilotProxy.h"

//#include "nsoutil/nssmedit.h"
#include "nsoutil/nsoutdlg.h"
#include "nsoutil/nsepiout.h"
#include "nsoutil/nsfilgui.h"
#include "nsoutil/nsrechcr.h"
#include "nautilus/nsepisodview.h"
#include "nautilus/nscqvue.h"
#include "nautilus/nsldvvue.h"
#include "nautilus/nsldvvar.h"
#include "adopt.h"
#include "nsbb\nsdefArch.h"

#include "ns_ob1\nautilus-bbk.h"
#include "ns_ob1\BB1Task.h"
#include "ns_ob1\BB1Types.h"
#include "ns_ob1\InterfaceForKs.h"

#include "nautilus\EnumProc.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsdrugview.h"
#include "nautilus\nsgoalview.h"
#include "nautilus\nsprocessview.h"
#include "nautilus\nsFollowUpView.h"
#include "nautilus\nsrcview.h"
#include "nautilus\nsMailSvce.h"
#include "nautilus\nsTeamDocView.h"
#include "nautilus\nscqdoc.h"
#include "nautilus\decisionTreeDV.h"
#include "docking\stripeDock.h"
#include "nsbb\nsPaneSplitter.h"
#include "nssavoir\nsHealthTeamInterface.h"
#include "nssavoir\GraphicHealthTeam.h"
#include "nsbb\logpass.h"
#include "nsbb\nsattvaltools.h"
#include "pilot\NautilusPilot.hpp"
#include "pilot\NSAgentTools.h"
#include "nsbb\tagNames.h"
#include "nsoutil\ns_arche.h"
#include "nsepisod\nsToDo.h"

#include <tlhelp32.h>
#include <vdmdbg.h>

#include <windows.h>

#define ID_CB2 750
#define ID_CB3 751

DEFINE_RESPONSE_TABLE1(TMyApp, OWL::TApplication)
  EV_OWLVIEW(dnCreate,                          EvNewView),
  EV_OWLVIEW(dnClose,                           EvCloseView),
  EV_WM_DROPFILES,
  EV_WM_HOTKEY,
  EV_COMMAND(CM_CREATDOC,                       CmCreatDoc),
  EV_COMMAND(CM_MODIFDOC,                       CmModifDoc),
  EV_COMMAND(CM_NEW_ENTETE,                     CmNewEnTete),
  EV_COMMAND(CM_MOD_ENTETE,                     CmModEnTete),
  EV_COMMAND(CM_MODIFCOR,                       CmModifCor),
  EV_COMMAND(CM_CREATCOR,                       CmCreatCor),
  EV_COMMAND(CM_HEALTHTEAM_ROSE,                CmHealthTeam),
  EV_COMMAND(CM_HEALTHTEAM_LIST,                CmHealthTeamList),
  EV_COMMAND(IDC_COOP_CHARTS,                   CmCoopChartBox),
  EV_COMMAND(CM_VENUE_LIST,                     CmVenuesList),
  EV_COMMAND(CM_IMPORT_PATIENT,                 CmImportPatientLdv),
  EV_COMMAND(CM_IMPORT_CORRESP,                 CmImportCorrespLdv),
	EV_COMMAND(CM_IMPORT_USER,                    CmImportUserLdv),
  EV_COMMAND(CM_NEWDOC,                         CmNewDoc),
  EV_COMMAND(CM_NEWPAT,                         CmNewPat),
  EV_COMMAND(CM_BUREAU,                         CmBureau),
  EV_COMMAND(CM_MAILBOX,                        CmMailBox),
  EV_COMMAND(CM_OUTILS,                         CmOutils),
  EV_COMMAND(CM_BBK,                            CmBBK),
  EV_COMMAND(CM_GROUPGD,                        CmGroupGd),
  EV_COMMAND(CM_REQUETE,                        CmRequete),
  EV_COMMAND(CM_RESULT,                         CmResult),
  EV_COMMAND(CM_SAUVEGARDE,                     CmSauvegarde),
  EV_COMMAND(CM_REFARCH,                        CmRefArchetype),
  EV_COMMAND(CM_REFEDIT,                        CmEditRef),
  EV_COMMAND(CM_ARCHEDIT,                       CmEditArchetype),
  EV_COMMAND(CM_TPLEDIT,                        CmEditTemplate),
  EV_COMMAND(CM_INTERFACE,                      CmDeviceInterface),
  EV_COMMAND(CM_MATERIELS,                      CmMateriels),
  EV_COMMAND(CM_RECONVOC,                       CmReconvoquer),
  EV_COMMAND(CM_SET_RECONVOC,                   CmSetReconvocDate),
  EV_COMMAND(CM_ABOUT,                          CmAbout),
  EV_COMMAND(CM_CHEMISE,                        CmChemise),
  EV_COMMAND(CM_NEWTTXT,                        CmNewTtext),
  EV_COMMAND(CM_NEWTMPL,                        CmNewTmpl),
  EV_COMMAND(CM_OPENTTXT,                       CmOpenTtext),
  EV_COMMAND(CM_OPENTMPL,                       CmOpenTmpl),
  EV_COMMAND(CM_ENREG_WIN,                      CmEnregWin),
  EV_COMMAND(CM_NEWCR,                          CmNewCr),
  EV_COMMAND(CM_NEWCS,                          CmNewConsult),
  EV_COMMAND(CM_NEWCONSULT,                     CmNewTypedDoc),
  EV_COMMAND(CM_IMPORTE,                        CmImporter),
  EV_COMMAND(CM_IMPORTIMG,                      CmImportImg),
  EV_COMMAND(CM_IMPORT_HTML,                    CmImportHtml),
  EV_COMMAND(CM_IMPORT_PDF,                     CmImportPdf),
  EV_COMMAND(CM_CAPTUREIMG,                     CmCaptureImg),
  EV_COMMAND(CM_CAPTURETWAIN,                   CmCaptureTwain),
  EV_COMMAND(CM_PANNEL,                         CmGenerePannel),
  EV_COMMAND(CM_CVITALE,                        CmCarteVitale),
  EV_COMMAND(CM_CREATCOMPTA,                    CmFicheCompt),
  EV_COMMAND(CM_MODIFCOMPTA,                    CmSituation),
  EV_COMMAND(CM_AGA,                            CmAga),
  EV_COMMAND(CM_IMPAYES,                        CmImpayes),
  EV_COMMAND(CM_LISTACTES,                      CmListeActes),
  EV_COMMAND(CM_SOMACTES,                       CmSommeActes),
  EV_COMMAND(CM_SOMENCAISS,                     CmSommeEncaiss),
  EV_COMMAND(CM_SAISIETP,                       CmSaisieTP),
  EV_COMMAND(CM_FEUILLESOINS,                   CmFeuilleSecu),
  EV_COMMAND(CM_IMPORTNOEMIE,                   CmImportNoemie),
  EV_COMMAND(CM_RECETTE,                        CmRecettes),
  EV_COMMAND(CM_DEPENSE,                        CmDepenses),
  EV_COMMAND(CM_LISTRECETTES,                   CmListeRecettes),
  EV_COMMAND(CM_LISTDEPENSES,                   CmListeDepenses),
  EV_COMMAND(CM_2035,                           Cm2035),
  EV_COMMAND(CM_EXIT,                           CmExit),
  EV_COMMAND(CM_HELP,                           CmHelp),
  EV_COMMAND(CM_EPISO_START,                    CmEpisoStart),
  EV_COMMAND(CM_EPISO_STOP,                     CmEpisoStop),
  EV_COMMAND(CM_EPISO_LEARN,                    CmEpisoLearn),
  EV_COMMAND(CM_EPISO_MODELS,                   CmEpisoModels),
  EV_COMMAND(IDC_EPISOD_PHARE_ON,               IdcEpisodPhare),
  EV_COMMAND(IDC_EPISOD_PHARE_OFF,              IdcEpisodPhare),
  EV_COMMAND(CM_LDV,                            CmOpenLdV),
  EV_COMMAND(CM_LDV_DRUGS,                      CmOpenDrugs),
  EV_COMMAND(CM_PROCESS,                        CmOpenProcess),
  EV_COMMAND(CM_LDV_GOALS,                      CmOpenGoals),
  EV_COMMAND(CM_FOLLOW_UP,                      CmOpenFollowUp),
  EV_COMMAND(CM_SHOW_CURVES,                    CmShowCurves),
  EV_COMMAND(CM_PROMET_MESSAGE,                 CmEpisodusBuildMessage),
  EV_COMMAND(CM_PROMET_SEND,                    CmEpisodusSendMessage),
  EV_COMMAND(CM_PROMET_SURF,                    CmEpisodusSurf),
  EV_COMMAND(CM_PROMET_SURF_PERS,               CmEpisodusSurfPerso),
  EV_COMMAND(CM_PREDI,                          CmPREDI),
  EV_COMMAND(CM_PROJET,                         CmProjet),
  EV_COMMAND(CM_QUESTIONNAIRE,                  CmNewQuestionnaire),
  EV_COMMAND(CM_DIOGENE,                        CmDiogene),
  EV_COMMAND(CM_FUNCTION,                       CmFunctions),
  EV_COMMAND(CM_ADMIN,                          CmOpenAdmin),
  EV_COMMAND(CM_IDENTIFIER,                     CmShowId),
  EV_COMMAND(CM_WEB_EXPORT,                     CmWebExport),
  EV_COMMAND(CM_PARAM_PRINT,                    CmParamPrint),
  EV_COMMAND(CM_SYNCHRO_IMP_THIS_PAT,           CmSynchroImportThisPat),
  EV_COMMAND(CM_SYNCHRO_EXP_THIS_PAT,           CmSynchroExportThisPat),
	EV_COMMAND(CM_PILOT_AGENTS,                   CmPilotAgentsManagement),
  EV_COMMAND(CM_PARAM_INIT,                     CmParamInit),
  EV_COMMAND(CM_PARAM_UPDATE,                   CmParamUpdate),
  EV_COMMAND(CM_USER_1,                         CmUserFunction1),
  EV_COMMAND(CM_USER_2,                         CmUserFunction2),
  EV_COMMAND(CM_USER_3,                         CmUserFunction3),
  EV_COMMAND(CM_USER_4,                         CmUserFunction4),
  EV_COMMAND(CM_USER_5,                         CmUserFunction5),
  EV_COMMAND(CM_USER_6,                         CmUserFunction6),
  EV_COMMAND(CM_USER_7,                         CmUserFunction7),
  EV_COMMAND(CM_USER_8,                         CmUserFunction8),
  EV_COMMAND(CM_USER_9,                         CmUserFunction9),
//  EV_REGISTERED("Episodus Mouse Hook Message",  CmMouseHook),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
// definition pour OB1
const string		 NULLSTRING	        = string("") ;
int							 NULLINT		        = 0 ;
Voidptr					 NULLPTR		        = NULL ;
NSPatPathoArray  *NULLPATHO         = (NSPatPathoArray *) NULL ;
PatPathoIter     *NULLPATPATHOITER  = (PatPathoIter *) NULL;
NautilusQuestion *NULLLNAUTQUEST    = (NautilusQuestion *) NULL ;
AnswerStatus     *NULLLANSWERSTATUS = (AnswerStatus *) NULL ;
PathsList		     *NULLPATHS         = (PathsList *) NULL ;
BB1Object				 *NULLOBJECT	      = (BB1Object *) NULL ;
NautilusEvent		 *NULLNAUTILUSEVENT	= (NautilusEvent *) NULL ;
NSSearchStruct	 *NULLSEARCHSTRUCT  = (NSSearchStruct *) NULL ;
// -----------------------------------------------------------------------------

TMyApp::TMyApp(NSContexte *pCtx)
       :OWL::TApplication("Episodus")
{
	// Object initialisation

	appContext = pCtx ;
  Client     = NULL ;

  frame      = 0 ;
  sb         = 0 ;
  cb         = 0 ;

  Harbor     = 0 ;
  cb1        = 0 ;
  cb2        = 0 ;
  cb3        = 0 ;

  hAccelerator = 0 ;

	// Get current directory as soon as possible
  initCurrentDir() ;

	// Splash window

  pSplashWindow = 0 ;
  pSplashDib    = 0 ;

	// Object initialisation

  sCurrentMenuName = string("") ;

  iCaptureHotKey = 0 ;
  iBabylonHotKey = 0 ;

	appContext->getSuperviseur()->setApplication(this) ;

  SetCmdShow(SW_MAXIMIZE) ;
  SetToolBarWindow(0) ;

  // register hotkey shortcut
  Word        Key ;
  TShiftState Shift ;

  // create the shortcut for the capture (shortcut : [CTRL] + <Espace>
  TShortCut myShortCut = ShortCut(Word(' '), TShiftState() << ssCtrl) ;
  ShortCutToKey(myShortCut, Key, Shift) ;
  Word Mods = 0 ;
  Mods = Mods | MOD_CONTROL ;
  RegisterHotKey(NULL, myShortCut, Mods, Key) ;
  iCaptureHotKey = myShortCut ;

  // create the shortcut for the capture (shortcut : [SHIFT] + <Espace>
  Mods = 0 ;
  TShortCut myShortCut2 = ShortCut(Word(' '), TShiftState() << ssShift) ;
  ShortCutToKey(myShortCut, Key, Shift) ;
  Mods = Mods | MOD_SHIFT ;
  RegisterHotKey(NULL, myShortCut2, Mods, Key) ;
  iBabylonHotKey = myShortCut2 ;
}

// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
TMyApp::~TMyApp()
{
/*
  // --------
  // does anyone have a reason to not do it ?
  // if it is not used, it will not be in the code. .fab
  // --------
  NSSuper* pSuper = appContext->getSuperviseur() ;

	if (pSuper)
  {
		delete pSuper ;
    pSuper = 0 ;
  }

  if (appContext)
  {
		delete appContext ;
    appContext = 0 ;
  }
  // --------
*/

  // unregister hotkey shortcut
  Word        Key ;
  TShiftState Shift ;

  Word Mods = 0 ;
  ShortCutToKey(iCaptureHotKey, Key, Shift) ;
  Mods = Mods | MOD_CONTROL ;
  UnregisterHotKey(NULL, iCaptureHotKey) ;

  Mods = 0 ;
  ShortCutToKey(iBabylonHotKey, Key, Shift) ;
  Mods = Mods | MOD_SHIFT ;
  UnregisterHotKey(NULL, iBabylonHotKey) ;

  if (NULL != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}

void
TMyApp::InitMainWindow()
{
	sendMessageToSplash("Init main window") ;
  setSplashPercentDone(95) ;

  NSSuper* pSuper = appContext->getSuperviseur() ;

  string ps = string("TMyapp Init main window: Entering.") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // HKEY hKeyResult;
  // clé du programme Word.Basic dans HKEY_LOCAL_MACHINE
  // LPCTSTR lpszKey = _T("SOFTWARE\\CLASSES\\Word.Basic");

	// Si on est appelé en temps que serveur OLE, on ne crée pas la
	// fenêtre principale
	// if (GetRegistrar().IsOptionSet(TOcCmdLine::Embedding))

	// if (GetRegistrar().IsOptionSet(amEmbedding))
	// 	nCmdShow = SW_HIDE;

  // if (nCmdShow != SW_HIDE)
  //  nCmdShow = (nCmdShow != SW_SHOWMINNOACTIVE) ? SW_SHOWNORMAL : nCmdShow;

try
{
  HRESULT hOleInitResult = ::OleInitialize(NULL) ;

  bool bInitOk = true ;
  string sMsg = string("Init main window: Ole initialization returns ") ;
  switch (hOleInitResult)
  {
    case S_OK    : sMsg += "Ok" ; break ;
    case S_FALSE : sMsg += "that COM library is already initialized on this apartment" ; break ;
    case OLE_E_WRONGCOMPOBJ :
      sMsg += "that the versions of COMPOBJ.DLL and OLE2.DLL on your machine are incompatible with each other" ;
      bInitOk = false ;
      break ;
    case RPC_E_CHANGED_MODE :
      sMsg += "that a previous call to CoInitializeEx specified a different concurrency model for this apartment" ;
      bInitOk = false ;
      break ;
  }
  pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
  if (false == bInitOk)
    erreur(sMsg.c_str(), standardError, 0) ;
}
catch (...)
{
  string ps = string("Exception Init main window OleInitialize") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;
	erreur(ps.c_str(), standardError, 0) ;
}

  string sNomModule = string("NAUTILUS") ;
  if (pSuper && (pSuper->getPredi()) && (string("") != pSuper->getPredi()->_sNomModule))
  {
    sNomModule = pSuper->getPredi()->_sNomModule ;
    if (string("") != pSuper->getPredi()->_sOEMModule)
      sNomModule += string("/") + pSuper->getPredi()->_sOEMModule ;
  }
  sNomModule += string(" (") + pSuper->getInstanceString() + string(")") ;

	frame = new NSMDIFrame(appContext, sNomModule.c_str(), 0, *(Client = new NSMDIClient(this)), true, this) ;
	// frame->SetOcApp(OcApp) ;

  // frame->Attr.AccelTable = IDA_MDICMNDS ;

  frame->SetIcon(this, EPI_ICON1) ;
  frame->SetIconSm(this, EPI_ICON1) ;

  // pour les barres dockables
  Harbor = new THarborEx(*frame) ;

	// Construction de la barre de statut
	sb = new OWL::TStatusBar(frame, TGadget::Recessed) ;

	// Construction de la barre de contrôle
	cb1 = new TDockableControlBarEx(frame) ;
  cb1->SetCaption("Primary control bar") ;

  /*
  cb1->Insert(*new TButtonGadget(CM_NEWDOC, 		CM_NEWDOC, 	 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_NEWPAT, 		CM_NEWPAT, 	 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_BUREAU, 		CM_BUREAU, 	 	TButtonGadget::Command)) ;
  cb1->Insert(*new TSeparatorGadget);
  cb1->Insert(*new TButtonGadget(CM_IMPORTE, 		CM_IMPORTE, 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_CHEMISE, 		CM_CHEMISE,  	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_IMPORTIMG, 	CM_IMPORTIMG, TButtonGadget::Command)) ;

  DWORD   nBufferLength = 254 ; // size, in characters, of buffer
  char    szResult[255] ;	      // address of buffer for found filename
  LPTSTR  lpFilePart ;          // address of pointer to file component
  if (::SearchPath(NULL, "ns_grab", ".dll", nBufferLength, szResult, &lpFilePart))
    cb1->Insert(*new TButtonGadget(CM_CAPTUREIMG,   CM_CAPTUREIMG, 	TButtonGadget::Command)) ;

  cb1->Insert(*new TSeparatorGadget);
  cb1->Insert(*new TButtonGadget(CM_LDV, 		    CM_LDV,  	    TButtonGadget::Command)) ;
  cb1->Insert(*new TSeparatorGadget);
  cb1->Insert(*new TButtonGadget(CM_NEWTTXT, 		CM_NEWTTXT,  	TButtonGadget::Command)) ;

  // Insertion du bouton Word si Word.Basic existe dans la base de registre
  if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpszKey, (DWORD)0, KEY_READ, &hKeyResult) == ERROR_SUCCESS)
  	cb1->Insert(*new TButtonGadget(CM_NEWTMPL, 		CM_NEWTMPL, TButtonGadget::Command)) ;

  // Fonction d'appel des comptes-rendus pour le modele serveur OLE
  cb1->Insert(*new TButtonGadget(CM_NEWCR, 		  CM_NEWCR, 	 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_NEWCONSULT, CM_NEWCONSULT,TButtonGadget::Command)) ;
  cb1->Insert(*new TSeparatorGadget);
  cb1->Insert(*new TButtonGadget(CM_ENREGISTRE, CM_ENREGISTRE,TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_COMPOSE, 		CM_COMPOSE,  	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_IMPRIME, 		CM_IMPRIME,  	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_VISUAL, 		CM_VISUAL,  	TButtonGadget::Command)) ;
  cb1->Insert(*new TSeparatorGadget);
  cb1->Insert(*new TButtonGadget(CM_MAILBOX, 		CM_MAILBOX, 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_OUTILS, 		CM_OUTILS, 	 	TButtonGadget::Command)) ;
  cb1->Insert(*new TButtonGadget(CM_ABOUT, 		  CM_ABOUT, 	 	TButtonGadget::Command)) ;
  */
	cb1->SetHintMode(TGadgetWindow::EnterHints) ;

  cb2 = new TDockableControlBarEx(ID_CB2, "Secondary control bar", false, frame) ;

  // cb3 = new TDockableControlBarEx(ID_CB3, "Episodus", false, frame) ;
  // cb3->Insert(*new TButtonGadget(CM_EPISO_START, CM_EPISO_START, TButtonGadget::Command)) ;

  Harbor->Insert(*cb1, OWL::alTop) ;
  Harbor->Insert(*cb2, OWL::alTop) ;
  //Harbor->Insert(*cb3, OWL::alTop) ;

  // Insertion des barres (statut et contrôle)
  frame->Insert(*sb, TDecoratedFrame::Bottom) ;

	// Définition de la fenêtre principale et de son menu
	SetMainWindow(frame) ;
  appContext->getSuperviseur()->pExeModule = GetMainWindow()->GetModule() ;

  // Initialization of all menus
  //
  initMenus() ;

  GetMainWindow()->Attr.Style |= WS_MAXIMIZE ;

	// Mise en place du gestionnaire de documents
	// SetDocManager(new TDocManager(dmMDI | dmMenu | dmNoRevert, this));
  SetDocManager(new TDocManager(dmMDI)) ;

	appContext->getSuperviseur()->setMainWindow(GetMainWindow()) ;

  sendMessageToSplash("Go") ;
  setSplashPercentDone(100) ;

  if (pSplashWindow)
  {
    delete pSplashWindow ;
    pSplashWindow = 0 ;
  }

  if (pSplashDib)
  {
    delete pSplashDib ;
    pSplashDib = 0 ;
  }

  char szMain[20] ;
  sprintf(szMain, "%p", GetMainWindow()->HWindow) ;
  ps = string("TMyapp Init main window: Leaving (Main Window: ") + string(szMain) + string(")") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;
}

// Called when a window gains focus and sets its own menu
//
voidTMyApp::setMenu(string sMenuName, HACCEL far *hAccelerator){	string sNewMenuName = string("") ;	if (string("") == sMenuName)		sNewMenuName = sCurrentMenuName ;  else  	sNewMenuName = sMenuName ;  if (string("") == sNewMenuName)		return ;	nsMenuIniter menuIter(appContext) ;	OWL::TMenuDescr menuBar ;
  menuIter.initMenuDescr(&menuBar, sNewMenuName) ;
  GetMainWindow()->SetMenuDescr(menuBar) ;

  *hAccelerator = menuIter.getAccelerator() ;

  sCurrentMenuName = sNewMenuName ;

  // Make certain "Windows" menu is properly defined
  //
  NSMDIClient *pMDIClient = frame->GetClientWindow() ;
  pMDIClient->ClientAttr->hWindowMenu = frame->FindChildMenu(frame->GetMenu());
}voidTMyApp::createSplash()
{
	if (pSplashWindow)
		return ;

	// Splash window

	int SplashStyle = TSplashWindow::MakeStatic | TSplashWindow::MakeGauge |
                                                 TSplashWindow::CaptureMouse ;
	int timeOut = 0 ;

  pSplashDib    = new TDib("splash.bmp") ;
	pSplashWindow = new TSplashWindow(*pSplashDib, pSplashDib->Width(),
                                  pSplashDib->Height() + 100, SplashStyle, timeOut) ;

	pSplashWindow->Create() ;       // create the splash window
	pSplashWindow->UpdateWindow() ; // update the display to show the splash screen
}// -----------------------------------------------------------------------------
// Met à 1 le fichier instance.dat (1ère instance du pgm)
// -----------------------------------------------------------------------------
bool
TMyApp::SetFirstInstance()
{
  ofstream    outFile ;
  string      sOut ;

  sOut = "1\n" ;

  // On écrit 1 dans le fichier instance.dat
  outFile.open("instance.dat") ;
  if (!outFile)
  {
  	// Make certain that error message won't be hidden by the splash screen
		//
  	HWND hWnd = 0 ;
    if (NULL != pSplashWindow)
    	hWnd = pSplashWindow->GetHandle() ;

    erreur("Erreur d'ouverture en écriture du fichier instance.dat.", standardError, 0, hWnd) ;
    return false ;
  }

  for (size_t i = 0 ; i < strlen(sOut.c_str()) ; i++)
    outFile.put(sOut[i]) ;

  outFile.close() ;
  return true ;
}

bool
TMyApp::GetNumInstance(int& numInst)
{
  ifstream inFile ;
  ofstream outFile ;
  string line ;
  string sFichierDat = "" ;
  string sInstance = "" ;
  char   cInstance[5] ;

  inFile.open("instance.dat") ;
  if (!inFile)
  {
    erreur("Erreur d'ouverture du fichier instance.dat.", standardError, 0) ;
    return false ;
  }

  while (!inFile.eof())
  {
    getline(inFile,line) ;
    if (line != "")
      sFichierDat += line + "\n" ;
  }
  inFile.close() ;

  // on récupère le numéro d'instance contenu dans le fichier
  for (size_t i = 0 ; (i < strlen(sFichierDat.c_str())) && (sFichierDat[i] != '\n') ; i++)
    sInstance += sFichierDat[i] ;

  numInst = atoi(sInstance.c_str()) + 1 ;

  sprintf(cInstance, "%d\n", numInst) ;
  sInstance = string(cInstance) ;

  // On écrit le nouveau numéro d'instance dans le fichier instance.dat
  outFile.open("instance.dat") ;
  if (!outFile)
  {
    erreur("Erreur d'ouverture en écriture du fichier instance.dat.", standardError, 0) ;
    return false ;
  }

  for (size_t i = 0 ; i < strlen(sInstance.c_str()) ; i++)
    outFile.put(sInstance[i]) ;

  outFile.close() ;
  return true ;
}

//
// InitInstance is called automatically by myApp.Run()
//
void
TMyApp::InitInstance()
{
	bool bExit = false ;
	int  retVal ;

  // appel de InitMainWindow
	OWL::TApplication::InitInstance() ;

	// Création d'un mutex pour gérer numInstance
	if (CreateMutex(NULL, TRUE, "EpisodusMutex") == NULL)
	{
  	// Make certain that error message won't be hidden by the splash screen
		//
  	HWND hWnd = 0 ;
    if (NULL != pSplashWindow)
    	hWnd = pSplashWindow->GetHandle() ;
		erreur("Echec de la fonction CreateMutex.", standardError, 0, hWnd) ;
		appContext->getSuperviseur()->setInstance(0) ;
		bExit = true ;
	}
	else
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			// on est dans la première instance
			if (SetFirstInstance())
				appContext->getSuperviseur()->setInstance(1) ;
			else
			{
				appContext->getSuperviseur()->setInstance(0) ;
				bExit = true ;
			}
		}
		else
		{
    	int iInstance ;
      bool bGetInstance = GetNumInstance(iInstance) ;
			// instance n (n>1) on récupère le numéro et on l'incrémente
			if (bGetInstance)
			{
      	appContext->getSuperviseur()->setInstance(iInstance) ;
				retVal = ::MessageBox(0, "Episodus : Une session antérieure existe déjà. Voulez-vous continuer ?", "Message Nautilus", MB_YESNO) ;
				if (IDNO == retVal)
					bExit = true ;
			}
			else
			{
				appContext->getSuperviseur()->setInstance(0) ;
				bExit = true ;
			}
		}
	}

	if (bExit) // pour sortir dans MessageLoop
		appContext->setUtilisateur(0) ;
	else
	{
		// Ctl3d DLL
		EnableCtl3d(true) ;
		// BWCC DLL
		EnableBWCC(true, 0xc) ;

		GetMainWindow()->DragAcceptFiles(true) ;

		if (appContext->getUtilisateur() == 0)
		{
			appContext->getSuperviseur()->setInitInstance(true) ;
			appContext->getSuperviseur()->ReferenceTousArchetypes() ;
      appContext->getSuperviseur()->ReferenceNewFilsGuides() ;
			appContext->getSuperviseur()->AppelUtilisateur() ;
		}

		NS_CLASSLIB::TRect rect ;
		cb1->GetRect(rect) ;
		Harbor->Move(*cb2, OWL::alTop, &NS_CLASSLIB::TPoint(rect.left + rect.Width(), rect.top)) ;
		cb2->GetRect(rect) ;
		// Harbor->Move(*cb3, OWL::alTop, &NS_CLASSLIB::TPoint(rect.left + rect.Width(), rect.top));

		// Initialisation du lexique et du réseau sémantique
		appContext->getDico()->Prend() ;
		appContext->getFilGuide()->Prend() ;
		appContext->getFilDecode()->Prend() ;
	}
}


bool
TMyApp::CanClose()
{
	if (GetMainWindow())
	{
		// on doit positionner bCanCloseHisto si un patient est ouvert
		// sinon l'historique empêche le CloseChildren et CanClose renvoie false
		if (appContext->getPatient())
			if (!(appContext->getUtilisateur()->fermePatEnCours()))
				return false ;
		//pContexte->getPatient()->bCanCloseHisto = true;
		prendClient()->CloseChildren() ;
	}
	return OWL::TApplication::CanClose() ;
}

// -----------------------------------------------------------------------------
// MessageLoop : appelée par Run après InitInstance
// -----------------------------------------------------------------------------
int
TMyApp::MessageLoop()
{
	// test : si pas d'utilisateur => sort du programme
	if (appContext->getUtilisateur() == 0)
	{
    if (0 != GetMainWindow())
		  GetMainWindow()->CloseWindow() ;
		return 0 ;
	}

	// on détruit les fichier tmp de perso (fichiers CR)
	// if (appContext->getSuperviseur()->getInstance() == 1)
	//	DetruireFichiersTmp() ;

	return (OWL::TApplication::MessageLoop()) ;
}

void
TMyApp::DetruireFichiersTmp()
{
  WIN32_FIND_DATA FileData ;
  HANDLE          hSearch ;
  char            szMask[255] ;
  bool            bFinish = false ;
  size_t          pos ;
  string          sFileName, sExtension ;
  string          sPathName, sFile ;
  char            extension[255] ;
  DWORD           dwAttr ;

  sPathName = appContext->PathName("FPER") ;
  strcpy(szMask, sPathName.c_str()) ;
  strcat(szMask, "*.*") ;

  hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)
    return ;

  while (!bFinish)
  {
    dwAttr = FileData.dwFileAttributes ;

    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {
      sFileName = string(FileData.cFileName) ;
      // Récupération de l'extension
      pos = sFileName.find(".") ;
    }

    if ((!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) && (pos != string::npos))
    {
      sExtension = string(sFileName, pos + 1, strlen(sFileName.c_str()) - pos - 1) ;
      strcpy(extension, sExtension.c_str()) ;
      sExtension = string(strlwr(extension)) ;

      if (sExtension == "tmp")
      {
        sFile = sPathName + sFileName ;
        if (!DeleteFile(sFile.c_str()))
        {
          string sMsg = "Pb à la destruction du fichier temporaire " + sFileName ;
          erreur(sMsg.c_str(), standardError, 0) ;
        }
      }
    }

    if (!FindNextFile(hSearch, &FileData))
    {
      if (GetLastError() == ERROR_NO_MORE_FILES)
        bFinish = true ;
      else // impossible de trouver le fichier suivant : on sort quand même
        return ;
    }
  }
}


/*
void
TMyApp::RemoveNavBar()
{
	// on enlève les boutons de la control bar
  TGadget* pGadget = cbNav->FirstGadget() ;
  while (pGadget)
  {
   	cbNav->Remove(*pGadget) ;
    cbNav->LayoutSession() ;
    pGadget = cbNav->FirstGadget() ;
  }
}
*/


/*
void
TMyApp::InsertControlBar(TButtonGadget* pGadget)
{
	GadgetArray[nbGadget++] = pGadget ;
	cb->Insert(*pGadget) ;
  cb->LayoutSession() ;
}
*/


/*
void
TMyApp::InsertControlBar(TControlGadget* pGadget)
{
	GadgetArray[nbGadget++] = pGadget ;
	cb->Insert(*pGadget) ;
  cb->LayoutSession() ;
}
*/


/*
void
TMyApp::InsertControlBar(TSeparatorGadget* pGadget)
{
	GadgetArray[nbGadget++] = pGadget ;
	cb->Insert(*pGadget) ;
  cb->LayoutSession() ;
}
*/


// -----------------------------------------------------------------------------
// capture by hotkey
// -----------------------------------------------------------------------------
void
TMyApp::EvHotKey(int idHotKey)
{
  if (idHotKey == iCaptureHotKey)
  {
    NSEpisodus* pEpisod = appContext->getSuperviseur()->getEpisodus() ;
    // update mouse cursor position
    GetCursorPos(pEpisod->getPMousePoint()) ;
    // update information

    HWND hMainWnd = (HWND) GetMainWindow() ;

    OSVERSIONINFO osVer ;
    osVer.dwOSVersionInfoSize = sizeof(OSVERSIONINFO) ;
    GetVersionEx(&osVer) ;

    // Mise d'Episodus au premier plan
    // ::SystemParametersInfo(SPI_SETFOREGROUNDLOCKTIMEOUT, 0, (LPVOID)0, SPIF_SENDWININICHANGE | SPIF_UPDATEINIFILE) ;

    if (osVer.dwMajorVersion < 5)
      ::SetForegroundWindow(hMainWnd) ;
    else
      GetMainWindow()->FlashWindow(TRUE) ;

    pEpisod->Capture(hMainWnd) ;
  }
  else if (idHotKey == iBabylonHotKey)
  {
    NSEpisodus* pEpisod = appContext->getSuperviseur()->getEpisodus() ;
    // update mouse cursor position
    GetCursorPos(pEpisod->getPMousePoint()) ;
    // update information
    pEpisod->CaptureBabylon((HWND) GetMainWindow()) ;
  }
}


void
TMyApp::EvDropFiles(TDropInfo dropInfo)
{
	// si pas d'utilisateur ou pas de patient en cours : on sort
	if ((appContext->getUtilisateur() == 0) || (appContext->getPatient() == 0))
		return ;

  string ps = string("Files dropped") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	int fileCount = dropInfo.DragQueryFileCount() ;
	for (int index = 0 ; index < fileCount ; index++)
	{
		int fileLength = dropInfo.DragQueryFileNameLen(index) + 1 ;
		char* filePath = new char[fileLength] ;
		dropInfo.DragQueryFile(index, filePath, fileLength) ;

    ps = string("Handling file drop for file ") + string(filePath) ;
    appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

		appContext->getPatient()->Importer(filePath) ;

		// TDocTemplate* tpl = GetDocManager()->MatchTemplate(filePath);
		// if (tpl)
		// 	tpl->CreateDoc(filePath);

		delete[] filePath ;
	}
	dropInfo.DragFinish() ;
}

bool
MyConditionChild(TWindow* pWind, void* pCreate)
{
  //NSMUEView* pCr = TYPESAFE_DOWNCAST(&pCreate, NSMUEView) ;
  NSMDIChild* pChild = TYPESAFE_DOWNCAST(pWind, NSMDIChild) ;
  if (pChild)
    if (pChild->getCreateWindow()== pCreate)
      return true ;
  return false ;
}


void
TMyApp::EvNewView(TView& view)
{
try
{
	char nomVue[80] ;
	NS_CLASSLIB::TRect zone ;

	// Dans la version serveur OLE, il faut déterminer quelle est la
	// fenêtre parent (différent lorsqu'il est utilisé in situ - serveur
	// dans son conteneur - ou appelé d'une autre application)
	/********************************************************
	TOleView* ov = TYPESAFE_DOWNCAST(&view, TOleView);
	if (view.GetDocument().IsEmbedded() && ov && !ov->IsOpenEditing())
	{
		TWindow* vw = view.GetWindow() ;
		vw->SetParent(TYPESAFE_DOWNCAST(GetMainWindow(), TOleFrame)->GetRemViewBucket()) ;
		vw->Create() ;
	}
	else
	{
    TMDIChild* child = new TMDIChild(*Client, 0) ;
    if (view.GetViewMenu())
    {
      child->SetMenuDescr(*view.GetViewMenu()) ;
    }

    // on récupère le nom de la vue à créer
    strcpy(nomVue, view.GetViewName()) ;

    child->Create() ;
    child->SetClientWindow(view.GetWindow()) ;
    child->GetClientRect(zone) ;
	}
  if (!ov || !ov->GetOcRemView())
    OcApp->SetOption(amEmbedding, false) ;
  **********************************************************/

	bool bSplitted  = false ;

	NSMDIChild*         child  = 0 ;
  NSCQWindowsView*    pCQVue = 0 ;
  NSDecisionTreeView* pGuidelineVue = 0 ;

	// NSMUEVIEW : fenêtre contenue dans un NSPaneSplitter
	// NSMUEVIEW : windows made to be part of a NSPaneSplitter
	NSMUEView* pMUEView = TYPESAFE_DOWNCAST(&view, NSMUEView) ;
	if (pMUEView)
	{
    // Première fenêtre à s'ouvrir : il faut créer le NSPaneSplitter
    // First window of a group : it means we have to create the NSPaneSplitter
    if (pMUEView->getCreateWindow() == NULL)
    {
      NSMDIChildContent* pChildContent = new NSMDIChildContent(appContext, Client) ;

      NSPaneSplitter* pPanelSplitter = new NSPaneSplitter(appContext, pChildContent) ;
      // si le style de la fenetre ne contient pas les butoons OK, Cancel
      // on n'affiche pas la fenetre gadget avec les buttons
    	NSGadgetWindow*  pGadgetWnd = new NSGadgetWindow (pChildContent, pMUEView->uButtonsStyle) ;
    	pGadgetWnd->setPaneSplitter(pPanelSplitter) ;
/*
      if (pMUEView->uButtonsStyle != 0x00000000L)
        pGadgetWnd = new NSGadgetWindow (pPanelSplitter, true) ;
      else
        pGadgetWnd = new NSGadgetWindow (pPanelSplitter, false) ;
*/
			pPanelSplitter->_pGadgetPanelWindow = pGadgetWnd ;

      pChildContent->setSplitter(pPanelSplitter) ;
			pChildContent->setToolBar(pGadgetWnd) ;

      // child = new NSMDIChild(appContext, *Client, 0, pPanelSplitter) ;
      child = new NSMDIChild(appContext, *Client, 0, pChildContent) ;
      pPanelSplitter->_pMDIChild = (TMDIChild*)child ;
      // child->split((TWindow*)pGadgetWnd, 0, psNone, 0.50) ;

      // TGadgetControl* pControl = new TGadgetControl(pGadgetWnd) ;
      pMUEView->setPaneSplitter(pPanelSplitter) ;

      pGuidelineVue = TYPESAFE_DOWNCAST(&view, NSDecisionTreeView) ;
      pCQVue        = TYPESAFE_DOWNCAST(&view, NSCQWindowsView) ;
      if (pCQVue)
        child->setCreateWindow(pCQVue) ;
      else
        child->setCreateWindow(pMUEView) ;
    }

    // Fenêtre qui vient compléter un groupe
    // Window that comes as a complementary window of a group
    else
    {
      TWindow *winClient = Client->FirstThat(MyConditionChild,(void *)pMUEView->getCreateWindow()) ;
      if (winClient)
      {
        NSMDIChild* child = TYPESAFE_DOWNCAST(winClient, NSMDIChild) ;
        if (child)
        {
          NSMUEView* wndMother = TYPESAFE_DOWNCAST(pMUEView->getSplittedWindow(), NSMUEView) ;
          if (wndMother)
          {
            //pContexte->getSuperviseur()->pNSApplication->prendClient()
            NSSplittingWindowProperty *pSplitProp = appContext->getUtilisateur()->aWinProp.searchProperty(wndMother->getFunction(), pMUEView->getFunction()) ;
            if (pSplitProp)
            {
              pMUEView->setPercent(pSplitProp->getPercent()) ;
              pMUEView->setSplitDirection(pSplitProp->getSplitDirection()) ;
            }

            child->split(pMUEView->getSplittedWindow(),pMUEView, pMUEView->getSplitDirection(),pMUEView->getPercent()) ;
            pMUEView->setPaneSplitter(wndMother->getPaneSplitter()) ;
            bSplitted = true ;
          }
        }
      }
    }
  }

  // En cas de fenêtre complémentaire d'un NSPaneSplitter, le travail est fini
  // In case of a NSPaneSplitter complementary window, work is done
  if (bSplitted)
    return ;

  if (NULL == child)
    child = new NSMDIChild(appContext, *Client, 0, view.GetWindow(), false) ;

  // Mise en place des icones
  NSCsVue* pview = TYPESAFE_DOWNCAST(&view, NSCsVue) ;
  if (pview)
  {
    child->SetIconSm(this, CS_ICON_CS) ;
  }
  else
	{
    NSCRReadOnlyView* pview = TYPESAFE_DOWNCAST(&view, NSCRReadOnlyView) ;
    if (pview)
    {
      child->SetIconSm(this, CR_ICON_CS) ;
    }
    else
    {
      NSTtxView* pview = TYPESAFE_DOWNCAST(&view, NSTtxView) ;
      if (pview)
      {
        child->SetIcon(this, TTX_ICON) ;
        child->SetIconSm(this, TTX_ICON_CS) ;
      }
      else
      {
        NsHistorique* pview = TYPESAFE_DOWNCAST(&view, NsHistorique) ;
        if (pview)
        {
          child->SetIconSm(this, HIST_ICON_CS) ;
        }
        else
        {
          NSVisualView* pview = TYPESAFE_DOWNCAST(&view, NSVisualView) ;
          if (pview)
          {
            child->SetIconSm(this, HTM_ICON_CS) ;
          }
          else
          {
            NSAutoWordView* pView = TYPESAFE_DOWNCAST(&view, NSAutoWordView) ;
            if (pView)
            {
              child->SetIconSm(this, WRD_ICON_CS) ;
            }
            else
            {
              NSEpisodView* pView = TYPESAFE_DOWNCAST(&view, NSEpisodView) ;
              if (pView)
              {
                child->SetIconSm(this, WRD_ICON_CS) ;
              }
              else
              {
                NSCQWindowsView* pView = TYPESAFE_DOWNCAST(&view, NSCQWindowsView) ;
                if (pView)
                {
                  child->SetIconSm(this, CQ_ICON_CS) ;
                }
              }
            }
          }
        }
      }
    }
  }

  if (view.GetViewMenu())
    child->SetMenuDescr(*view.GetViewMenu()) ;

  // on récupère le nom de la vue à créer
  strcpy(nomVue, view.GetViewName()) ;

  string ps = string("EvNewView, before child->create") ;
  appContext->getSuperviseur()->trace(&ps, 1) ;

/*
  if (pCQVue)
  {
    if (!pCQVue->clientWin)
    {
      delete child ;
      return ;
    }

    char far szTitle[256] ;
    int res = pCQVue->clientWin->GetWindowText(szTitle, 255) ;
    if (!res)
      strcpy(szTitle, "Archetype") ;
    //pCQVue->pDoc->SetTitle(szTitle);
  }

  if (pCQVue)
    child->initClient(pCQVue->clientWin) ;
*/

  // if (pCQVue)
  //  child->initClientArchetype(pCQVue) ;

  child->Create() ;

  char szChild[20] ;
  sprintf(szChild, "%p", child->HWindow) ;
  ps = string("EvNewView: NSMDIChild created (") + string(szChild) + string(").") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  // Si la NSMDIChild n'a pas de NSPaneSplitter, on a fini
  // Si the NSMDIChild has no NSPaneSplitter, we are done
  if (NULL == child->getCreateWindow())
    return ;
  if(NULL == pMUEView->getPaneSplitter())
    return ;

  NSGadgetWindow* pGadgetWind = pMUEView->getPaneSplitter()->_pGadgetPanelWindow ;
  if(NULL == pGadgetWind)
    return ;

	// start modif 21/03/07
	pMUEView->setGadgetPanelWindow(pGadgetWind) ;
/*
  child->split(pGadgetWind, 0, psNone, 0.50) ;
  if (pMUEView->uButtonsStyle == 0)
    child->split(pGadgetWind, child->getCreateWindow(), psHorizontal, 0.001) ;
  else
    child->split(pGadgetWind, child->getCreateWindow(), psHorizontal, 0.08) ;
*/
	child->split(child->getCreateWindow(), 0, psNone, 0.50) ;
	// end modif 21/03/07

  child->getCreateWindow()->SetFocus() ;

  ps = string("EvNewView after child->create") ;
  appContext->getSuperviseur()->trace(&ps, 1) ;

  if ((NULL == pCQVue) && (pMUEView))
  {
    TWindow *wndCr = child->getCreateWindow() ;
    NSMUEView* wndCreate = TYPESAFE_DOWNCAST(wndCr, NSMUEView) ;
    NSWindowProperty *pWinProp = appContext->getUtilisateur()->aWinProp.getProperty(wndCreate->getFunction()) ;
    if (pWinProp)
    {
    	// Warning : if application is iconic, then GetWindowRect returns
      //           fake informations (-3000, -3500...)
    	if (false == appContext->GetMainWindow()->IsZoomed())
      {
        ps = string("EvNewView, MoveWindow not zoomed with rect ") + pWinProp->getPosAsString() ;
        appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    		child->MoveWindow(pWinProp->X(), pWinProp->Y(), pWinProp->W(), pWinProp->H(), true) ;
      }
      else
      {
        ps = string("EvNewView, MoveWindow zoomed with rect ") + pWinProp->getPosAsString() ;
        appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

      	NS_CLASSLIB::TRect  clientRect, rectWnd ;
      	Client->GetWindowRect(clientRect) ;
      	rectWnd.left = pWinProp->X() - clientRect.left ;
      	rectWnd.top = pWinProp->Y() - clientRect.top ;
      	child->MoveWindow(rectWnd.left , rectWnd.top, pWinProp->W(), pWinProp->H(), true) ;
      	// string sFunction = pWinProp->sFunction ;
      }
    }
    else
    {
      ps = string("EvNewView, NSWindowProperty not found for function ") + wndCreate->getFunction() ;
      appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    }
  }

  // cas des vues formulaire : on redimentionne la fenetre
  if (pCQVue)
  {
    ps = string("EvNewView de NSCQVue, début du traitement spécifique") ;
    appContext->getSuperviseur()->trace(&ps, 1) ;

    // ?? pCQVue->SetParent((TWindow *)child) ;
    bool bMovingCQVue = false ;

    if (pCQVue->getFunction() != "")
    {
      // copier/coller nscsvue pour "Synthesis"
      // fixer la nouvelle position
      // (on ne tient pas compte de la taille, vu le probleme pour restaurer
      // une fenetre TView, TWindow mise en icone)
      NSWindowProperty *pWinProp = appContext->getUtilisateur()->aWinProp.getProperty(pCQVue->getFunction()) ;
      if (pWinProp)
      {
      	/*
        NS_CLASSLIB::TRect  clientRect, rectWnd ;
        Client->GetWindowRect(clientRect) ;
        rectWnd.top = pWinProp->iX - clientRect.left ;
        rectWnd.left = pWinProp->iY - clientRect.top ;
        child->MoveWindow(rectWnd.top , rectWnd.left, pWinProp->iW, pWinProp->iH, true) ;
        */

        ps = string("EvNewView, MoveWindow for CQVue with rect ") + pWinProp->getPosAsString() ;
        appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

        child->MoveWindow(pWinProp->X(), pWinProp->Y(), pWinProp->W(), pWinProp->H(), true) ;
        bMovingCQVue = true ;
      }
    }

    if (false == bMovingCQVue)
    {
      NS_CLASSLIB::TRect winRect, cliRect ;
      child->GetWindowRect(winRect) ;
      child->GetClientRect(cliRect) ;

      // ATTENTION : contrairement à ce que dit l'aide en ligne, les coordonnées X, Y
      // dans MoveWindow sont relatives à la zone client de la fenetre Parent (ici la MainWindow)
      // On positionne donc ici les formulaires, au départ, dans le coin supérieur gauche
      // (à cause de la taille des dialogues à afficher)

      int newWidth  = (pCQVue->_dlgRect).Width() + (winRect.Width() - cliRect.Width()) ;
      int newHeight = (pCQVue->_dlgRect).Height() + (winRect.Height() - cliRect.Height()) ;

      ps = string("EvNewView de NSCQVue, MoveWindow") ;
      appContext->getSuperviseur()->trace(&ps, 1) ;

      child->MoveWindow(0, 0, newWidth, newHeight, true) ;
    }

    if (pCQVue->_pDoc->iTypeDoc == NSCQDocument::dpio)
    {
      NSPredi* pPredi = appContext->getSuperviseur()->getPredi() ;
      if (pPredi)
      {
        char far szTitle[256];
        // pCQVue->clientWin->GetWindowText(szTitle, 255) ;
        pCQVue->GetWindow()->GetWindowText(szTitle, 255) ;

        ps = string("EvNewView de NSCQVue, référencement du formulaire ") + string(szTitle) ;
        appContext->getSuperviseur()->trace(&ps, 1) ;

        pPredi->referenceFormulaire((TWindow*)child, szTitle) ;
      }
    }

    ps = string("EvNewView de NSCQVue, fin du traitement spécifique") ;
    appContext->getSuperviseur()->trace(&ps, 1) ;
  }

  // Resize NSDecisionTreeView
  //
  if (pGuidelineVue)
  {
    NS_CLASSLIB::TRect winRect, cliRect ;
    child->GetWindowRect(winRect) ;
    child->GetClientRect(cliRect) ;

    // ATTENTION : contrairement à ce que dit l'aide en ligne, les coordonnées X, Y
    // dans MoveWindow sont relatives à la zone client de la fenetre Parent (ici la MainWindow)
    // On positionne donc ici les formulaires, au départ, dans le coin supérieur gauche
    // (à cause de la taille des dialogues à afficher)

    int newWidth  = (pGuidelineVue->getDialogRect()).Width() + (winRect.Width() - cliRect.Width()) ;
    int newHeight = (pGuidelineVue->getDialogRect()).Height() + (winRect.Height() - cliRect.Height()) ;

    ps = string("EvNewView de NSDecisionTreeView, MoveWindow") ;
    appContext->getSuperviseur()->trace(&ps, 1) ;

    child->MoveWindow(0, 0, newWidth, newHeight, true) ;
  }

  // on crée les fenêtres complémentaires cette fenêtre
  // we now create the complementary windows for this window
  if (pMUEView)
  {
    NSMUEView* pMotherWind ;
    NSPaneSplitter* pPanelSplitter = pMUEView->getPaneSplitter() ;
    if (pPanelSplitter && (pMUEView->getFunction() != ""))
    {
      VecteurString VectChildFct ;
      pPanelSplitter->researchChildWindFonction(pMUEView->getFunction(), &VectChildFct) ;
      if (VectChildFct.empty())
        return ;

      for (EquiItemIter iterChilds = VectChildFct.begin() ; VectChildFct.end() != iterChilds ; iterChilds++)
      {
        pMotherWind = pPanelSplitter->researchMotherWindow((**iterChilds), pMUEView->getFunction()) ;
        if (pMotherWind)
          pMotherWind->newWindow((**iterChilds)) ;
      }

      if (pPanelSplitter->_pCurrentFocusedView)
        pPanelSplitter->_pCurrentFocusedView->focusView() ;
    }
  }
}
catch (...)
{
  erreur("Exception TMyApp::EvNewView.", standardError, 0) ;
}
}

void
TMyApp::EvCloseView(TView& /*view*/)
{
	// Rien à faire ici en MDI
}

void
TMyApp::Tempo(double s)
{
  time_t time1, time2 ;

  time(&time1) ;
  do
    time(&time2) ;
  while (difftime(time2, time1) < s) ;
}

bool
TMyApp::isForegroundApplication()
{
  HWND hwnd = GetForegroundWindow() ;
  if (NULL == hwnd)
    return false ;

  DWORD foregroundPid ;
  if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0)
    return false ;

  return (foregroundPid == getpid()) ;
}

void
TMyApp::CmMailBox()
{
	if (appContext->pMailBoxChild != NULL)
	{
  	SetFocus(appContext->pMailBoxChild->GetHandle()) ;
		return ;
	}

  string ps = string("Opening mailbox") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->pMailBoxWindow = new NSMailServiceWindow(appContext, GetMainWindow()) ;
	appContext->pMailBoxChild  = new NSMailServiceChild(appContext, *(prendClient()), "", appContext->pMailBoxWindow) ;
	appContext->pMailBoxChild->Create() ;

/*
	TMailBox*	    pMailBox ;
	NSMailParams* pm ;

	if (!appContext->getUtilisateur()->initMail())
	{
		erreur("Impossible d'ouvrir la boite aux lettres Nautilus.", standardError, 0) ;
		return ;
	}

	pm = appContext->getUtilisateur()->pMail ;
	pMailBox = new TMailBox(NULL, pm->sUrlPOP3, pm->iPortPOP3, pm->sUserPOP3, pm->sPassPOP3, appContext) ;
	pMailBox->Show() ;
*/
}

void
TMyApp::CmOutils()
{
try
{
	string sModuleName = string("") ;

	NsOutilDialog* pNsOutilDialog = new NsOutilDialog(appContext->GetMainWindow(), appContext, &sModuleName) ;
	int idRet = pNsOutilDialog->Execute() ;
  delete pNsOutilDialog ;

  if ((IDOK != idRet) || (sModuleName == string("")))
		return ;

  TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;

  if (NULL == pDCModule)
  {
  	string sErrorMsg = string("Impossible to open DLL ") +  sModuleName ;
    erreur(sErrorMsg.c_str(), standardError, 0) ;
    return ;
  }

  void (FAR *pAdresseFct) (TWindow far *, NSContexte far *) ;

  // Récupération du pointeur sur la fonction // Getting function's pointer
  // (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmToolsAction$qp11OWL@TWindowp10NSContexte") ;
  if (NULL == pAdresseFct)
  {
  	delete pDCModule ;
    return ;
  }

  ((*pAdresseFct)((TWindow far *) appContext->GetMainWindow(), (NSContexte far *) appContext)) ;

  // delete pDCModule ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmOutils : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmOutils.", standardError, 0) ;
}
}

void
TMyApp::CmBBK()
{
  BB1BBInterface* pOB1 = appContext->getBBinterface() ;
  if (pOB1)
    pOB1->MaskBB(false) ;

 //	NSBBKDlg *pBBKDialog = new NSBBKDlg(pContexte->GetMainWindow(), pContexte) ;
 //	pBBKDialog->Execute() ;
}

void
TMyApp::CmGroupGd()
{
try
{
	NSGroupGdDialog* pGroupGdDialog = new NSGroupGdDialog(appContext->GetMainWindow(), appContext) ;
	pGroupGdDialog->Execute() ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmGroupGd : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmGroupGd.", standardError, 0) ;
}
}

void
TMyApp::CmRequete()
{
	if (appContext->getUtilisateur() != 0)
		appContext->getUtilisateur()->CmRequete() ;
}

void
TMyApp::CmResult()
{
	if (appContext->getUtilisateur() != 0)
		appContext->getUtilisateur()->CmResult() ;
}

void
TMyApp::CmSauvegarde()
{
/************************************************
  // création de l'équipe médicale locale
  NSMoralPerson *pMPTeam = new NSMoralPerson(appContext, "", "Equipe médicale locale", "") ;
  pMPTeam->createTree() ;
  NSMoralPersonManager *pMPManager = new NSMoralPersonManager(pMPTeam) ;
  delete pMPTeam ;

  NSPersonsAttributesArray *pAttsList = new NSPersonsAttributesArray() ;

  const char *serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
  NSBasicAttributeArray *pAttrArray = new NSBasicAttributeArray() ;
  pAttrArray->push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;
  bool bListOk = appContext->pPilot->personList(serviceName, pAttsList, pAttrArray) ;
  delete pAttrArray ;

  if ((bListOk) && (!(pAttsList->empty())))
  {
    for (NSPersonsAttributeIter iterPerson = pAttsList->begin() ; iterPerson != pAttsList->end() ; iterPerson++)
    {
      string sUserID = (*iterPerson)->getAttributeValue(PIDS) ;
      NVLdVTemps	ldvCurDate ;	ldvCurDate.takeTime() ;
      NVLdVTemps	ldvNoLimit ;  ldvNoLimit.setNoLimit() ;

      NSHTMMandateArray *pMandatesArray = new NSHTMMandateArray() ;
      pMandatesArray->push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
      pMPManager->addMember(new NSHealthTeamMember(sUserID, NSHealthTeamMember::person, pMandatesArray), NSHealthTeamMember::person) ;
      delete pMandatesArray ;
    }
  }
  delete pAttsList ;

  NSTeamGraphManager* pTeamManager = new NSTeamGraphManager(appContext) ;
  if (pTeamManager->setTeamGraph(pMPManager->getPatho(), appContext->getUtilisateurID(), true))
  {
    MessageBox(NULL, "La création de l'équipe médicale a réussi", "Création de l'Equipe Médicale locale", MB_OK) ;
  }

***********************************************/


/*
 CSVParser *csvParser = new CSVParser("c:\\stucture12_2004.csv", ';', '\0') ;
 bool bReturn = csvParser->run(pContexte) ;
 delete csvParser ;
*/
}

void
TMyApp::CmEditRef()
{
try
{
	ArchetypesListDialog* pListArchDlg = new ArchetypesListDialog(appContext->GetMainWindow(), appContext, NSArcManager::referentiel) ;
	pListArchDlg->Execute() ;
  delete pListArchDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmEditRef : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmEditRef.", standardError, 0) ;
}
}

void
TMyApp::CmEditArchetype()
{
try
{
	ArchetypesListDialog* pListArchDlg = new ArchetypesListDialog(appContext->GetMainWindow(), appContext, NSArcManager::archetype) ;
	pListArchDlg->Execute() ;
  delete pListArchDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmEditArchetype : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmEditArchetype.", standardError, 0) ;
}
}

void
TMyApp::CmEditTemplate()
{
	// if (appContext->getUtilisateur() != 0)
	//	appContext->getUtilisateur()->CmEditTemplate() ;

  NSTemplateWindow* pTplWindow = new NSTemplateWindow(appContext, GetMainWindow()) ;
  TMDIChild*        pMdiChild  = new TMDIChild(*(OWL::TMDIClient*)prendClient(), "", pTplWindow) ;
	pMdiChild->Create() ;
}

void
TMyApp::CmRefArchetype()
{
	if (appContext->getUtilisateur())
		appContext->getUtilisateur()->CmRefArchetype() ;
}

void
TMyApp::CmPREDI()
{
	if ((NULL == appContext->getUtilisateur()) ||
      (NULL == appContext->getPatient()))
		return ;

  string ps = string("User starts Predi") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (appContext->getBBinterface())
	  appContext->getBBinterface()->addNautilusEvent(std::string("GPRED")) ;
}

void
TMyApp::CmProjet()
{
	if ((NULL == appContext->getUtilisateur()) ||
      (NULL == appContext->getPatient()))
		return ;

  string ps = string("User starts project management") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (appContext->getBBinterface())
	  appContext->getBBinterface()->addNautilusEvent(std::string("GPROJ")) ;
}

void
TMyApp::CmNewQuestionnaire()
{
	if ((NULL == appContext->getUtilisateur()) ||
      (NULL == appContext->getPatient()))
		return ;

	appContext->getPatient()->CmNewArchetype() ;
}

void
TMyApp::CmDeviceInterface()
{
	if ((NULL == appContext->getUtilisateur()) ||
      (NULL == appContext->getPatient()))
		return ;

	appContext->getPatient()->CmDeviceInterface() ;
}

void
TMyApp::CmMateriels()
{
try
{
	ListeMaterielDialog* pListeMatDlg = new ListeMaterielDialog(appContext->GetMainWindow(), appContext) ;
	pListeMatDlg->Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmMateriels : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmMateriels.", standardError, 0) ;
}
}

void
TMyApp::CmReconvoquer()
{
  NSDateConvocDialog* pDateDlg = new NSDateConvocDialog(appContext->GetMainWindow(), appContext) ;
  int idRet = pDateDlg->Execute() ;
  if (idRet == IDOK)
  {
    NSVarConvoc* pVarConvoc = new NSVarConvoc(appContext, pDateDlg->sDate1, pDateDlg->sDate2) ;
    NSListeConvocDialog* pListeDlg = new NSListeConvocDialog(appContext->GetMainWindow(), appContext, pVarConvoc) ;
    pListeDlg->Execute() ;
    if (pListeDlg->bImprimerListe)
    {
      NSVoidArray* pVarArray = new NSVoidArray ;
      pVarArray->push_back((void*)(&(pVarConvoc->sDate1))) ;
      pVarArray->push_back((void*)(&(pVarConvoc->sDate2))) ;
      pVarArray->push_back((void*)(&(pVarConvoc->aPatientArray))) ;
      NSPubliRefDocument* pDocPubli = new NSPubliRefDocument(pVarArray, "ZPCVC", appContext) ;
      pDocPubli->Publier(false) ;
    }
    else if (pListeDlg->bImprimerLettre)
    {
      int index ;

      // on instancie le document convoc (lettre de reconvocation)
      NSConvocRefDocument* pDocConvoc = new NSConvocRefDocument(pVarConvoc, pListeDlg->pTabSelect, pListeDlg->nbConvoc, appContext) ;

      // on prépare l'objet de publication du document
      pDocConvoc->setPubli(new NSPublication(0, pDocConvoc, appContext)) ;

      NSPublication* pPubli = pDocConvoc->getPubli() ;

      // on instancie à la main un tableau de correspondants fictifs
      // qui correspondent à la liste des patients à reconvoquer
      for (int i = 0 ; i < pListeDlg->nbConvoc ; i++)
      {
        if (i < MAXSELECT)
        {
        	NSPersonInfo PatInfo(appContext) ;

          // on récupère l'indice du patient à reconvoquer
          index = pListeDlg->pTabSelect[i] ;
          PatInfo = *(pVarConvoc->aPatientArray[index]) ;

          pPubli->aCorrespBaseArray.push_back(new NSPersonInfo(PatInfo)) ;

          // on remplit également la structure de sélection aChoixPubli
          // sprintf(pDocConvoc->pPubli->aChoixPubli[i].corresp, "%03d", i) ;
          strcpy(pPubli->aChoixPubli[i].corresp, PatInfo.getNss().c_str()) ;
          pPubli->aChoixPubli[i].select     = true ;
          pPubli->aChoixPubli[i].imprimante = true ;
          strcpy(pPubli->aChoixPubli[i].nb_expl, "1") ;
          pPubli->aChoixPubli[i].cpt      = 1 ;
          strcpy(pPubli->aChoixPubli[i].url, "") ;
          pPubli->aChoixPubli[i].email    = false ;
          pPubli->aChoixPubli[i].intranet = false ;
          pPubli->aChoixPubli[i].html     = false ;
          pPubli->aChoixPubli[i].lettre   = false ;
        }
        else
        {
          erreur("Le nombre de patients à reconvoquer dépasse le maximum des publications.", standardError, 0) ;
          break ;
        }
      }

      // on lance la publication proprement dite
      pPubli->Publication() ;
    }
    delete pListeDlg ;
    delete pVarConvoc ;
  }

  delete pDateDlg;
}

void
TMyApp::CmSetReconvocDate()
{
	if (NULL == appContext->getPatient())
		return ;

	NSLdvDocument *pDocLdv = appContext->getPatient()->getLdvDocument() ;
  if (NULL == pDocLdv)
		return ;

	NSLdvDocument::LDVSERVICESERROR result = pDocLdv->NewAppointmentService(0) ;
}

void
TMyApp::CmAbout()
{
try
{
  string ps = string("Opening About dialog") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	AboutDialog(&TWindow(GetMainWindow()->GetCommandTarget()), appContext).Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmAbout : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmAbout.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Création d'un utilisateur
// -----------------------------------------------------------------------------
void
TMyApp::CmCreatDoc()
{
  if (appContext->getUtilisateur() != 0)
  {
    if (!appContext->getUtilisateur()->isAdmin())
    {
      erreur("Cette fonction est accessible uniquement à l'administrateur.", standardError, 0) ;
      return ;
    }

    string ps = string("User creates a new user") ;
    appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	 	if (!appContext->getUtilisateur()->Creer(appContext->GetMainWindow()))
      erreur("Erreur à la création d'un nouvel utilisateur", standardError, 0) ;
  }
}

// -----------------------------------------------------------------------------
// Modification d'un utilisateur
// -----------------------------------------------------------------------------
void
TMyApp::CmModifDoc()
{
  NSUtilisateurChoisi* pCurrentUser = appContext->getUtilisateur() ;
  if (NULL == pCurrentUser)
    return ;

  if (pCurrentUser->isAdmin())
  {
    if (false == pCurrentUser->ModifierAutreUtilisateur(appContext->GetMainWindow()))
      erreur("Erreur à la modification d'un nouvel utilisateur", standardError, 0) ;
    return;
  }

  string ps = string("User edits a user") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (false == pCurrentUser->Modifier(appContext->GetMainWindow()))
    erreur("Erreur à la modification d'un nouvel utilisateur", standardError, 0) ;
}

void
TMyApp::CmNewEnTete()
{
	if (appContext->getUtilisateur() != 0)
		appContext->getUtilisateur()->CmNewEnTete() ;
}


void
TMyApp::CmModEnTete()
{
	if (appContext->getUtilisateur() != 0)
		appContext->getUtilisateur()->CmModEnTete() ;
}


void
TMyApp::CmCreatCor()
{
#ifndef N_TIERS

  string ps = string("User creates a new refering professionnal") ;
  pSuper->trace(&ps, 1, NSSuper::trSteps) ;

  NSCorrespondant* pCorresp = new NSCorrespondant(appContext) ;
  pCorresp->lastError = pCorresp->open() ;
  if (pCorresp->lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture du fichier Corresp.db", 0, pCorresp->lastError) ;
    delete pCorresp ;
    return ;
  }

  bool creatOK = pCorresp->Creer(appContext->GetMainWindow()) ;
  pCorresp->lastError = pCorresp->close() ;
  if (pCorresp->lastError != DBIERR_NONE)
  {
    erreur("Erreur à la fermeture du fichier Corresp.db", 0, pCorresp->lastError) ;
    delete pCorresp ;
    return ;
  }

  delete pCorresp ;
#else
  /* NSPersonInfo *pPersonInfo = */ appContext->getPersonArray()->createPerson(pidsCorresp) ;
#endif // N_TIERS
}

void
TMyApp::CmModifCor()
{
  string ps = string("Editing a refering professionnal") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSTPersonListDialog indep(GetMainWindow(), pidsCorresp, false, appContext) ;
  if (indep.Execute() == IDOK)
  {
    if (indep.bCreer)
    {
      /* NSPersonInfo	*pPersonInfo = */ appContext->getPersonArray()->createPerson(pidsCorresp) ;
    }
    else
    {
      NSPersonInfo *temp = indep.pPersonSelect ;
      string sPids  = temp->getNss() ;
      string sRoles = temp->getRoles() ;
      appContext->getPersonArray()->modifyPerson(sPids, sRoles, pidsCorresp) ;
    }
  }
}

void
TMyApp::CmHealthTeam()
{
	if (appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens health team rosace") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->heathTeamRosaceShow() ;

/*
 GraphicHealthTeam* health = new GraphicHealthTeam(GetMainWindow(), appContext, appContext->getPatient()->pHealthTeam) ;
 health->Execute() ;
 delete health ;
*/
}

void
TMyApp::CmHealthTeamList()
{
	if (appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens health team list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  appContext->getPatient()->heathTeamListShow() ;
}

void
TMyApp::CmCoopChartBox()
{
	if (appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens a new Coop chart") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmNewCoopChart() ;
}

void
TMyApp::CmVenuesList()
{
	if (appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens venue list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  appContext->getPatient()->venuesListShow() ;
}

void
TMyApp::CmImportPatientLdv()
{
	// si pas d'utilisateur en cours : on sort
	if (appContext->getUtilisateur() == 0)
		return ;
  //if(!pContexte->pPilot->testServiceStatus(NautilusPilot::SERV_PATIENT_IMPORT))
	if(!appContext->getSuperviseur()->getPilot()->isOperationalLdVAgent())
  {
   //	appContext->pPilot->detailError(NautilusPilot::SERV_PATIENT_IMPORT) ;
   	if(!appContext->getSuperviseur()->getPilot()->traitError(NSAgentStatus::LdV))
    		return ;
  }
	appContext->getUtilisateur()->importPatientLdv() ;
}

void
TMyApp::CmImportUserLdv()
{
  // only on administrator can create a user
	if ((appContext->getUtilisateur() == 0) || (!(appContext->getUtilisateur()->isAdmin())))
  {
		string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "administratorRightsNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

  if(!appContext->getSuperviseur()->getPilot()->isOperationalLdVAgent())
  {
   	if(!appContext->getSuperviseur()->getPilot()->traitError(NSAgentStatus::LdV))
    		return ;
  }
	appContext->getUtilisateur()->importUserLdv() ;
}

void
TMyApp::CmImportCorrespLdv()
{
	// si pas d'utilisateur en cours : on sort
	if (appContext->getUtilisateur() == 0)
		return ;

  if(!appContext->getSuperviseur()->getPilot()->isOperationalLdVAgent())
  {
   	if(!appContext->getSuperviseur()->getPilot()->traitError(NSAgentStatus::LdV))
    		return ;
  }
	appContext->getUtilisateur()->importCorrespLdv() ;
}

void
TMyApp::CmNewDoc()
{
  if ((appContext->getSuperviseur()) && (appContext->getSuperviseur()->getEpisodus()))
    appContext->getSuperviseur()->getEpisodus()->resetCaptureArray() ;

	appContext->getSuperviseur()->AppelUtilisateur() ;
	if (appContext->getUtilisateur() == 0)
    GetMainWindow()->CloseWindow() ;
}

void TMyApp::CmNewPat()
{
  if ((appContext->getSuperviseur()) && (appContext->getSuperviseur()->getEpisodus()))
    appContext->getSuperviseur()->getEpisodus()->resetCaptureArray() ;

  if (appContext->getUtilisateur() == 0)
    return ;

  if (false == appContext->getUtilisateur()->isUser())
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "userOnlyFunction") ;
    erreur(sErrMsg.c_str(), standardError, 0) ;
    return ;
  }

  string ps = string("User opens a patient") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  appContext->getUtilisateur()->AppelPatient() ;
}

void
TMyApp::CmBureau()
{
  if (appContext->getUtilisateur() == 0)
    return ;

  int retVal = ::MessageBox(appContext->GetMainWindow()->GetHandle(), "Voulez-vous vraiment fermer le patient en cours ?", "Message Nautilus", MB_YESNO) ;
  if (IDYES != retVal)
    return ;

  string ps = string("User closes the patient") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if ((appContext->getSuperviseur()) && (appContext->getSuperviseur()->getEpisodus()))
    appContext->getSuperviseur()->getEpisodus()->resetCaptureArray() ;
  appContext->getUtilisateur()->fermePatEnCours() ;
}

void
TMyApp::CmOpenLdV()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens LdV") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->LdV_show() ;
}

void
TMyApp::CmOpenDrugs()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens drugs list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->drugs_show() ;
}

void
TMyApp::CmOpenProcess()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens processes list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->process_show() ;
}

void
TMyApp::CmOpenGoals()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens goals list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->goals_show() ;
}

void
TMyApp::CmOpenFollowUp()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens follow-up list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->followUp_show() ;
}

void
TMyApp::CmShowCurves()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens curves") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->followUp_show() ;
}

void
TMyApp::CmEpisodusBuildMessage()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User builds message for Prometheus") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = appContext->getSuperviseur()->getEpisodus() ;
  if (pEpisodus)
    pEpisodus->appendPrometheFile() ;
}

void
TMyApp::CmEpisodusSendMessage()
{
	if (appContext->getUtilisateur() == 0)
		return ;

  string ps = string("User sends message to Prometheus") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = appContext->getSuperviseur()->getEpisodus() ;
  if (pEpisodus)
    pEpisodus->sendPrometheMessage() ;
}

void
TMyApp::CmDiogene()
{
  NSPredi* pPredi = appContext->getSuperviseur()->getPredi() ;
  if (pPredi)
    pPredi->sendMessage() ;
}

void
TMyApp::CmEpisodusSurf()
{
	if (appContext->getUtilisateur() == 0)
		return ;

  string ps = string("User opens Prometheus web site") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = appContext->getSuperviseur()->getEpisodus() ;
  if ((pEpisodus) && (pEpisodus->sPrometheURL != ""))
    appContext->getSuperviseur()->NavigationEncyclopedie(pEpisodus->sPrometheURL, appContext) ;
}

void
TMyApp::CmEpisodusSurfPerso()
{
	if (appContext->getUtilisateur() == 0)
		return ;

  string ps = string("User opens personal information on Prometheus web site") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = appContext->getSuperviseur()->getEpisodus() ;
  if ((pEpisodus) && (pEpisodus->sPrometheURL != ""))
    appContext->getSuperviseur()->NavigationEncyclopedie(pEpisodus->sPrometheURL, appContext) ;
}

void
TMyApp::CmChemise()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

	appContext->getPatient()->CmChemise() ;
}

// -----------------------------------------------------------------------------
// lancement RichEdit Nautilus
// -----------------------------------------------------------------------------
void
TMyApp::CmNewTtext()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens new text") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmNewTTxt() ;
}

// -----------------------------------------------------------------------------
// lancement Word
// -----------------------------------------------------------------------------
void
TMyApp::CmNewTmpl()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens Word") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmNewTmpl() ;
}

// -----------------------------------------------------------------------------
// ouverture fichier RichEdit Nautilus
// -----------------------------------------------------------------------------
void
TMyApp::CmOpenTtext()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens text") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmOpenTTxt() ;
}

// -----------------------------------------------------------------------------
// ouverture fichier Word
// -----------------------------------------------------------------------------
void
TMyApp::CmOpenTmpl()
{
  if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
    return ;

  string ps = string("User opens Word") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmOpenTmpl() ;
}

// -----------------------------------------------------------------------------
// pour enregistrer la position des fenetres
// -----------------------------------------------------------------------------
void
TMyApp::CmEnregWin()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User record windows position") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  appContext->getUtilisateur()->bEnregWin = true ;
	::MessageBox(appContext->GetMainWindow()->GetHandle(), "La position actuelle des fenêtres sera enregistrée à la fermeture du patient.", "Message Nautilus", MB_OK) ;
}

void
TMyApp::CmImporter()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens import interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmImporter() ;
}

void
TMyApp::CmImportImg()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens images import interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmImportImg() ;
}

void
TMyApp::CmCaptureImg()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts image capture interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmAssistCapture() ;
}

void
TMyApp::CmCaptureTwain()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

	appContext->getPatient()->CmTwainCapture() ;
}

void
TMyApp::CmImportPdf()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens PDF import interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmOpenPdf() ;
}

void
TMyApp::CmImportHtml()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens HTML import interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmOpenHtml() ;
}

void
TMyApp::CmGenerePannel()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens pannel generation") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmGenerePannel() ;
}

void
TMyApp::CmNewCr()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts a new report") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmNewCr() ;
}

void
TMyApp::CmNewConsult()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts a new encounter") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	appContext->getPatient()->CmNewConsult() ;
}

void
TMyApp::CmNewTypedDoc()
{
	if ((appContext->getUtilisateur() == 0) ||
      (appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;
	appContext->getPatient()->CmNewConsult(false) ;
}

void
TMyApp::CmNewMagic()
{
  // fonction libre pour l'instant
}

void
TMyApp::CmCarteVitale()
{
	if (appContext->getUtilisateur() == 0)
    return ;

	if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  // NSComptaPatient* pCompta = new NSComptaPatient(appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(appContext, appContext->getPatient()->getNss()) ;
  NSComptaPatient* pCompta = new NSComptaPatient(appContext, &personInfo) ;

  pCompta->CmCarteVitale() ;
  delete pCompta ;
}

void
TMyApp::CmOpenAdmin()
{
	if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens demographic information") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  appContext->getPatient()->admin_show() ;
}

void
TMyApp::CmShowId()
{
	if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }
  string sPatientId = appContext->getPatient()->getNss() ;
  IdentifierDialog(appContext->GetMainWindow(), appContext, sPatientId).Execute() ;
}

void
TMyApp::CmWebExport()
{
  if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  appContext->getPatient()->CmWebExport() ;
}

void
TMyApp::CmFicheCompt()
{
	if (appContext->getUtilisateur() == 0)
    return ;

	if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens a new accounting form") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // NSComptaPatient* pCompta = new NSComptaPatient(appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(appContext, appContext->getPatient()->getNss()) ;
  NSComptaPatient* pCompta = new NSComptaPatient(appContext, &personInfo) ;

  pCompta->CmFicheCompt() ;
  delete pCompta ;
}

void
TMyApp::CmSituation()
{
  if (appContext->getUtilisateur() == 0)
    return ;

  if (appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens accounting interface") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // NSComptaPatient* pCompta = new NSComptaPatient(appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(appContext, appContext->getPatient()->getNss()) ;
  NSComptaPatient* pCompta = new NSComptaPatient(appContext, &personInfo) ;

  pCompta->CmSituation() ;
  delete pCompta ;
}

void
TMyApp::CmAga()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  NSCriteres Criteres ;

  string ps = string("User opens aga list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(appContext->GetMainWindow(), appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg ;

  while (Criteres.sDateAga1 <= Criteres.sDateAga2)
  {
    // on doit instancier et ouvrir un NSAgaDocument...
    NSAgaDocument* pAgaDoc = new NSAgaDocument(0, appContext) ;

    if (!pAgaDoc->Open(&Criteres))
    {
      erreur("Impossible d'ouvrir le document AGA", standardError, 0, appContext->GetMainWindow()->GetHandle()) ;
      delete pAgaDoc ;
      break ;
    }

    // Note : on n'affiche plus la MessageBox dans le cas nbAga == 0
    if (pAgaDoc->nbAga > 0)
    {
      NSListAgaDialog* pListAgaDlg = new NSListAgaDialog(appContext->GetMainWindow(), appContext, pAgaDoc) ;
      if (pListAgaDlg->Execute() == IDCANCEL)
      {
        pAgaDoc->Close() ;
        delete pAgaDoc ;
        break ;
      }

      if (pAgaDoc->bImprimer)
      {
        // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
        appContext->getUtilisateur()->pDocCompta->pAgaRefDoc = new NSAgaRefDocument(pAgaDoc) ;
        appContext->getUtilisateur()->pDocCompta->pAgaRefDoc->Publier(false) ;
      }
      delete pListAgaDlg ;
    }
    pAgaDoc->Close() ;
    delete pAgaDoc ;
    incremente_date(Criteres.sDateAga1) ;
  }

  ::MessageBox(appContext->GetMainWindow()->GetHandle(), "Liste des AGA terminée.", "Message Nautilus", MB_OK) ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmAga : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmAga.", standardError, 0) ;
}
}

void
TMyApp::CmImpayes()
{
try
{
	if (appContext->getUtilisateur() == 0)
   		return ;

  string ps = string("User opens unpaid list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSImpDocument...
  NSImpDocument ImpDoc(0, appContext) ;
  if (!ImpDoc.Open())
    return ;

  NSListImpDialog* pListImpDlg = new NSListImpDialog(appContext->GetMainWindow(), appContext, &ImpDoc) ;
  pListImpDlg->Execute() ;

  if (ImpDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    appContext->getUtilisateur()->pDocCompta->pImpRefDoc = new NSImpRefDocument(&ImpDoc);
    appContext->getUtilisateur()->pDocCompta->pImpRefDoc->Publier(false);
  }

  ImpDoc.Close() ;
  delete pListImpDlg;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmImpayes : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmImpayes.", standardError, 0) ;
}
}

void
TMyApp::CmListeActes()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  string ps = string("User opens acts list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSActDocument...
  NSActDocument ActDoc(0, appContext) ;
  if (!ActDoc.Open())
    return ;

  NSListActDialog* pListActDlg = new NSListActDialog(appContext->GetMainWindow(), appContext, &ActDoc) ;
  pListActDlg->Execute() ;
  if (ActDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    appContext->getUtilisateur()->pDocCompta->pListActRefDoc = new NSListActRefDocument(&ActDoc) ;
    appContext->getUtilisateur()->pDocCompta->pListActRefDoc->Publier(false) ;
  }

  ActDoc.Close() ;
  delete pListActDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmListeActes : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmListeActes.", standardError, 0) ;
}
}

void
TMyApp::CmSommeActes()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  string ps = string("User opens acts sum form") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSActDocument...
  NSActDocument ActDoc(0, appContext) ;
  if (!ActDoc.Open(1))
    return ;

  NSSomActDialog* pSomActDlg = new NSSomActDialog(appContext->GetMainWindow(), appContext, &ActDoc) ;
  pSomActDlg->Execute() ;
  if (ActDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    appContext->getUtilisateur()->pDocCompta->pSomActRefDoc = new NSSomActRefDocument(&ActDoc) ;
    appContext->getUtilisateur()->pDocCompta->pSomActRefDoc->Publier(false) ;
  }
  ActDoc.Close() ;
  delete pSomActDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmSommeActes() : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmSommeActes().", standardError, 0) ;
}
}

void
TMyApp::CmSommeEncaiss()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  string ps = string("User opens incomes list") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSEncaissDocument...
  NSEncaissDocument EncaissDoc(0, appContext) ;
  if (!EncaissDoc.Open())
    return ;

  NSSomEncaissDialog* pSomEncaissDlg = new NSSomEncaissDialog(appContext->GetMainWindow(), appContext, &EncaissDoc) ;
  pSomEncaissDlg->Execute() ;
  if (EncaissDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    appContext->getUtilisateur()->pDocCompta->pSomEncaissRefDoc = new NSSomEncaissRefDocument(&EncaissDoc) ;
    appContext->getUtilisateur()->pDocCompta->pSomEncaissRefDoc->Publier(false) ;
  }
  EncaissDoc.Close() ;
  delete pSomEncaissDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmSommeEncaiss() : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmSommeEncaiss().", standardError, 0) ;
}
}

void
TMyApp::CmSaisieTP()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  NSSaisieTPDialog* pSaisieTPDlg = new NSSaisieTPDialog(appContext->GetMainWindow(), appContext) ;
  pSaisieTPDlg->Execute() ;
  delete pSaisieTPDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmSaisieTP() : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmSaisieTP().", standardError, 0) ;
}
}

void
TMyApp::CmFeuilleSecu()
{
  if ((appContext->getUtilisateur() == 0) || (appContext->getPatient() == 0))
    return ;
	appContext->getPatient()->CmFeuilleSecu() ;
}

void
TMyApp::CmImportNoemie()
{
try
{
  NSNoemie* pImportNoemieDlg = new NSNoemie(appContext->GetMainWindow(), appContext) ;
  pImportNoemieDlg->Execute() ;
  delete pImportNoemieDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmImportNoemie : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmImportNoemie.", standardError, 0) ;
}
}

void
TMyApp::CmRecettes()
{
try
{
  NSRecettesDlg* pRecettesDlg = new NSRecettesDlg(appContext->GetMainWindow(), appContext) ;
  pRecettesDlg->Execute() ;
  delete pRecettesDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmRecettes : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmRecettes.", standardError, 0) ;
}
}

void
TMyApp::CmDepenses()
{
try
{
  NSDepensDlg* pDepensesDlg = new NSDepensDlg(appContext->GetMainWindow(), appContext) ;
  pDepensesDlg->Execute() ;
  delete pDepensesDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmDepenses : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmDepenses.", standardError, 0) ;
}
}

void
TMyApp::CmListeRecettes()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(appContext->GetMainWindow(), appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  NSCriteres Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg;

  // on doit instancier et ouvrir un NSAgaDocument...
  NSAgaDocument AgaDoc(0, appContext) ;
  if (!AgaDoc.Open(&Criteres, true))
  {
    erreur("Impossible d'ouvrir le document AGA", standardError, 0, appContext->GetMainWindow()->GetHandle()) ;
    return ;
  }

  if (AgaDoc.nbAga > 0)
  {
    NSListAgaDialog* pListAgaDlg = new NSListAgaDialog(appContext->GetMainWindow(), appContext, &AgaDoc) ;
    if (pListAgaDlg->Execute() == IDCANCEL)
    {
      AgaDoc.Close() ;
      delete pListAgaDlg ;
      return ;
    }

    if (AgaDoc.bImprimer)
    {
      // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
      appContext->getUtilisateur()->pDocCompta->pAgaRefDoc = new NSAgaRefDocument(&AgaDoc) ;
      appContext->getUtilisateur()->pDocCompta->pAgaRefDoc->Publier(false) ;
    }
    delete pListAgaDlg ;
  }
  else
    ::MessageBox(appContext->GetMainWindow()->GetHandle(), "Aucune recette ne correspond à la période choisie.", "Message Nautilus", MB_OK) ;

  AgaDoc.Close() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmListeRecettes : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmListeRecettes.", standardError, 0) ;
}
}

void
TMyApp::CmListeDepenses()
{
try
{
	if (appContext->getUtilisateur() == 0)
    return ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(appContext->GetMainWindow(), appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  NSCriteres Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg ;

  // on doit instancier et ouvrir un NSDepensDocument...
  NSDepensDocument DepDoc(0, appContext) ;
  if (!DepDoc.Open(&Criteres))
  {
    erreur("Impossible d'ouvrir le document des dépenses", standardError, 0, appContext->GetMainWindow()->GetHandle()) ;
    return ;
  }

  if (DepDoc.nbDepens > 0)
  {
    NSListDepensDialog* pListDepDlg = new NSListDepensDialog(appContext->GetMainWindow(), appContext, &DepDoc) ;
    if (pListDepDlg->Execute() == IDCANCEL)
    {
      DepDoc.Close() ;
      delete pListDepDlg ;
      return ;
    }

    if (DepDoc.bImprimer)
    {
      // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
      appContext->getUtilisateur()->pDocCompta->pDepRefDoc = new NSDepRefDocument(&DepDoc) ;
      appContext->getUtilisateur()->pDocCompta->pDepRefDoc->Publier(false) ;
    }
    delete pListDepDlg ;
  }
  else
    ::MessageBox(appContext->GetMainWindow()->GetHandle(), "Aucune dépense ne correspond à la période choisie.", "Message Nautilus", MB_OK) ;

  DepDoc.Close() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmListeDepenses : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmListeDepenses.", standardError, 0) ;
}
}

void
TMyApp::Cm2035()
{
try
{
  if (appContext->getUtilisateur() == 0)
    return ;

  NSAnnee2035Dialog* pAnneeDlg = new NSAnnee2035Dialog(appContext->GetMainWindow(), appContext) ;
  if (pAnneeDlg->Execute() == IDOK)
  {
    string sAnnee2035 = pAnneeDlg->sAnnee ;
    if ((sAnnee2035 != "") && (strlen(sAnnee2035.c_str()) == 4))
    {
      NS2035Dialog* pDlg = new NS2035Dialog(appContext->GetMainWindow(), appContext, sAnnee2035) ;
      pDlg->Execute() ;
      delete pDlg ;
    }
    else
    {
      erreur("La date saisie n'est pas correcte.", standardError, 0) ;
    }
  }
  delete pAnneeDlg ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::Cm2035 : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::Cm2035.", standardError, 0) ;
}
}

void
TMyApp::CmExit()
{
  string ps = string("User closes Episodus") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // cas de fermeture historique (autre cas : fermePatEnCours dans nschoisi.cpp)
  // on vérifie que le patient existe si on l'a déjà fermé ...
  if (appContext->getPatient())
    appContext->getPatient()->_bCanCloseHisto = true ;
  if (prendClient()->CloseChildren())
    GetMainWindow()->CloseWindow() ;
}

void
TMyApp::CmHelp()
{
  if (appContext->getSuperviseur())
  {
    string ps = string("User opens help") ;
    appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

    appContext->getSuperviseur()->setAideIndex("") ;
    appContext->getSuperviseur()->setAideCorps("home.htm") ;
    appContext->NavigationAideEnLigne() ;
  }
}

void
TMyApp::CmUserFunction(int iIndex)
{
  if ((iIndex < 0) || (iIndex > 8))
    return ;

try
{
	string sModuleName = appContext->getSuperviseur()->getUserModule(iIndex) ;
  if (string("") == sModuleName)
    return ;

  TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;
  if (NULL == pDCModule)
  {
  	string sErrorMsg = string("Impossible to open DLL ") +  sModuleName ;
    erreur(sErrorMsg.c_str(), standardError, 0) ;
    return ;
  }

  void (FAR *pAdresseFct) (TWindow far *, NSContexte far *) ;

  // Récupération du pointeur sur la fonction // Getting function's pointer
  // (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmUserAction$qp11OWL@TWindowp10NSContexte") ;
  if (NULL == pAdresseFct)                         
  {
  	delete pDCModule ;
    return ;
  }

  ((*pAdresseFct)((TWindow far *) appContext->GetMainWindow(), (NSContexte far *) appContext)) ;

  // delete pDCModule ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmOutils : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmOutils.", standardError, 0) ;
}
}

void
TMyApp::CmUserFunction1()
{
  CmUserFunction(0) ;
}

void
TMyApp::CmUserFunction2()
{
  CmUserFunction(1) ;
}

void
TMyApp::CmUserFunction3()
{
  CmUserFunction(2) ;
}

void
TMyApp::CmUserFunction4()
{
  CmUserFunction(3) ;
}

void
TMyApp::CmUserFunction5()
{
  CmUserFunction(4) ;
}

void
TMyApp::CmUserFunction6()
{
  CmUserFunction(5) ;
}

void
TMyApp::CmUserFunction7()
{
  CmUserFunction(6) ;
}

void
TMyApp::CmUserFunction8()
{
  CmUserFunction(7) ;
}

void
TMyApp::CmUserFunction9()
{
  CmUserFunction(8) ;
}

// -----------------------------------------------------------------------------
//
// Episodus
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// lance Episodus
// -----------------------------------------------------------------------------
void
TMyApp::CmEpisoStart()
{
try
{
  // NSEmailFile* pMailCfg = new NSEmailFile(appContext) ;
  // pMailCfg->Lancer() ;
  // delete pMailCfg ;

  NsMailToolsDialog mailTools(appContext->GetMainWindow(), appContext) ;
  mailTools.Execute() ;

  return ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmEpisoStart : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmEpisoStart.", standardError, 0) ;
}
/*
  // vérifier état de Episodus
  // appContext->getSuperviseur()->pEpisodus->SetStateOn(true) ;

  appContext->getSuperviseur()->pEpisodus->ImportModels() ;
  appContext->getSuperviseur()->pEpisodus->SetMouseHookState(true) ;

  appContext->getSuperviseur()->pEpisodus->lanceHook() ;

  FlushEpisodusControlBar() ;
  cb3->Insert(*new TButtonGadget(CM_EPISO_STOP, CM_EPISO_STOP, TButtonGadget::Command)) ;
  cb3->Insert(*new TButtonGadget(CM_EPISO_LEARN, CM_EPISO_LEARN, TButtonGadget::Command)) ;
  cb3->Insert(*new TButtonGadget(IDC_EPISOD_PHARE_OFF, IDC_EPISOD_PHARE_OFF, TButtonGadget::Command)) ;
  cb3->LayoutSession() ;
*/
}


// -----------------------------------------------------------------------------
// Arrête Episodus
// -----------------------------------------------------------------------------
void
TMyApp::CmEpisoStop()
{
try
{
  NsEpisodusOutilDialog outilDlg(appContext->GetMainWindow(), appContext) ;
  outilDlg.Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmEpisoStop : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmEpisoStop.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Paramètres d'impression
// -----------------------------------------------------------------------------
void
TMyApp::CmParamPrint()
{
try
{
  string ps = string("User opens print params") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSPrintParamsDlg* pDlg = new NSPrintParamsDlg(appContext->GetMainWindow(), appContext) ;
  pDlg->Execute() ;
  return ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmParamPrint : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmParamPrint", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Auto-Update parameters
// -----------------------------------------------------------------------------
void
TMyApp::CmParamUpdate()
{
try
{
  string ps = string("User opens update settings form") ;
  appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSUpdateParamsDlg* pDlg = new NSUpdateParamsDlg(appContext->GetMainWindow(), appContext) ;
  pDlg->Execute() ;
  return ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmParamUpdate : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmParamUpdate", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Init parameters (in chemins.dat)
// -----------------------------------------------------------------------------
void
TMyApp::CmParamInit()
{
try
{
  NSInitParamsDlg* pDlg = new NSInitParamsDlg(appContext->GetMainWindow(), appContext) ;
  pDlg->Execute() ;
  return ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmParamUpdate : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmParamUpdate", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Lance la phase d'apprentissage
// -----------------------------------------------------------------------------
void
TMyApp::CmEpisoLearn()
{
try
{
	NSPrometheFile* pPrometCfg = new NSPrometheFile(appContext) ;
	pPrometCfg->Lancer() ;
	delete pPrometCfg ;
	return ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmEpisoLearn : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmEpisoLearn.", standardError, 0) ;
}
/*
  appContext->getSuperviseur()->pEpisodus->SetLearning(true) ;
  //appContext->getSuperviseur()->pEpisodus->createViewData() ;

  if (appContext->getSuperviseur()->pEpisodus->pCurrentModel)
    appContext->getSuperviseur()->pEpisodus->ExportModels() ;
*/
}


void
TMyApp::CmEpisoModels()
{
try
{
#ifdef _EXT_CAPTURE
	ArrayEpisodusModelData* pM    = appContext->getSuperviseur()->getEpisodus()->pCaptureEngine->pModels ;
	ArrayEpisodusModelData* pSubM = appContext->getSuperviseur()->getEpisodus()->pCaptureEngine->pSubModels ;
#endif

#ifndef _EXT_CAPTURE
	NSModelDlg* pRefDlg = new NSModelDlg(appContext->GetMainWindow(), appContext, NULL) ;
#else
	NSModelDlg* pRefDlg = new NSModelDlg(appContext->GetMainWindow(), NULL, pM, pSubM, appContext->PathName("FGLO")) ;
#endif
	pRefDlg->Execute() ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception TMyApp::CmEpisoModels : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception TMyApp::CmEpisoModels.", standardError, 0) ;
}
}

void
TMyApp::IdcEpisodPhare()
{
  char    buffer[1024] ;
  sprintf(buffer, "toto") ;
}

void
TMyApp::logMessage(string sSpyMessage)
{
  ifstream    inFile ;
  string      sLastSpyMessage ;

  inFile.open("spy.log") ;
  if (inFile)
  {
    while (!inFile.eof())
    {
      string line ;
      getline(inFile, line) ;
      if (line != "")
        sLastSpyMessage += line + "\n" ;
    }
    inFile.close() ;
  }
  else // cas fichier vide
    sLastSpyMessage = "Nautilus log : " ;

  // on ajoute la nouvelle erreur
  sLastSpyMessage += sSpyMessage + "\n" ;

  // On écrit le nouveau fichier reqerror.log
  ofstream    outFile ;
  outFile.open("spy.log") ;
  if (!outFile)
  {
    // cas d'erreur bloquant...
    MessageBox(NULL, "Erreur d'ouverture en écriture du fichier spy.log.", "Erreur NAUTILUS", MB_ICONEXCLAMATION) ;
    return ;
  }

  for (size_t i = 0 ; i < strlen(sLastSpyMessage.c_str()) ; i++)
    outFile.put(sLastSpyMessage[i]) ;
  outFile.close() ;
}


void
TMyApp::CmFunctions()
{
try
{
#ifdef _MUE
  NSAddFunctions* pDlg = new NSAddFunctions(appContext->GetMainWindow(), appContext) ;
  pDlg->Execute() ;
#endif
  return ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmFunctions : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmFunctions.", standardError, 0) ;
}
}

void
TMyApp::CmSynchroImportThisPat()
{
	if (appContext->getUtilisateur() == 0)
		return ;

	if (appContext->getPatient() == 0)
	{
		string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

	string sMsg = appContext->getSuperviseur()->getText("patientSynchronization", "patientSynchronizationWarning") ;
	string sTxt = appContext->getSuperviseur()->getText("patientSynchronization", "patientImportAction") ;
	int retVal = ::MessageBox(0, sMsg.c_str(), sTxt.c_str(), MB_YESNO) ;
	if (retVal == IDNO)
		return ;

	string sGlobalLogin, sGlobalPasswd ;
	if (!appContext->getUtilisateur()->haveGlobalSessionPassword())
	{
		LogPassInterface* pDlgLogPass = new LogPassInterface(appContext->GetMainWindow(), appContext) ;
		if (pDlgLogPass->Execute() == IDCANCEL)
			return ;

		sGlobalLogin = pDlgLogPass->getLogin() ;
		sGlobalPasswd = pDlgLogPass->getPassword() ;
		delete pDlgLogPass ;
	}
	else
	{
		sGlobalLogin = appContext->getUtilisateur()->getGlobalLoginSession() ;
		sGlobalPasswd = appContext->getUtilisateur()->getGlobalPasswordSession() ;
	}

	appContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;
	appContext->getUtilisateur()->fermePatEnCours(true) ;
	appContext->getUtilisateur()->synchroImportThisPat(sGlobalLogin, sGlobalPasswd) ;

	// On lance le patient :
	// Récupération des données rattachées au patient : adresse et correspondants
	appContext->getPatient()->Initialisation() ;
	appContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}


void
TMyApp::CmSynchroExportThisPat()
{
	if (appContext->getUtilisateur() == 0)
		return ;

	if (appContext->getPatient() == 0)
	{
		string sErrMsg = appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

	string sMsg = appContext->getSuperviseur()->getText("patientSynchronization", "patientSynchronizationWarning") ;
	string sTxt = appContext->getSuperviseur()->getText("patientSynchronization", "patientExportAction") ;
	int retVal = ::MessageBox(0, sMsg.c_str(), sTxt.c_str(), MB_YESNO) ;
	if (retVal == IDNO)
		return ;

	string sGlobalLogin, sGlobalPasswd ;
	if (!appContext->getUtilisateur()->haveGlobalSessionPassword())
	{
		LogPassInterface* pDlgLogPass = new LogPassInterface(appContext->GetMainWindow(), appContext) ;
		if (pDlgLogPass->Execute() == IDCANCEL)
			return ;
		sGlobalLogin = pDlgLogPass->getLogin() ;
		sGlobalPasswd = pDlgLogPass->getPassword() ;
		delete pDlgLogPass ;
	}
	else
	{
		sGlobalLogin = appContext->getUtilisateur()->getGlobalLoginSession() ;
		sGlobalPasswd = appContext->getUtilisateur()->getGlobalPasswordSession() ;
	}

	appContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;
	appContext->getUtilisateur()->fermePatEnCours(true) ;
	appContext->getUtilisateur()->synchroExportThisPat(sGlobalLogin, sGlobalPasswd) ;

	// on termine la fermeture du patient
	delete appContext->getPatient() ;
	appContext->setPatient(0) ;

	// on rétablit le titre de l'application sans les infos patient
	appContext->setMainCaption() ;
	appContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}


void
TMyApp::CmPilotAgentsManagement()
{
try
{
	NSAgentsList *pAgWin = new NSAgentsList(appContext->GetMainWindow(), appContext->getSuperviseur()) ;
	pAgWin->Execute() ;
	delete pAgWin ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmPilotAgentsManagement : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmPilotAgentsManagement", standardError, 0) ;
}
}

void
TMyApp::Condemn(TWindow* win)
{
	// OWL::TApplication::Condemn(win) ;

  // TRACEX(OwlApp, 1, "Condemning window @" << (void*)win << *win);
  win->SetParent(0);

// #if 0

  // The following logic is from previous versions of ObjectWindows.
  // It results in LIFO destructions which is somewhat unfair.
  // However, we'll keep this code around in case previous applications
  // relied on this destruction order
  //
//  win->SetNext(CondemnedWindows);
//  CondemnedWindows = win;

// #else

  // Insert the new window to be deleted at the end of the list
  //
  win->SetNext(0) ;
  if (pNSCondemnedWindows)
	{
    TWindow* eol = pNSCondemnedWindows ;
    while (eol->Next())
      eol = eol->Next() ;
    eol->SetNext(win) ;
  }
  else
  {
    pNSCondemnedWindows = win ;
  }

// #endif
}

//
// Remove a condemned window from the list.
//
void
TMyApp::Uncondemn(TWindow* win)
{
	// OWL::TApplication::Uncondemn(win) ;

  if (win && pNSCondemnedWindows)
	{
    TWindow* w = 0 ;
    if (pNSCondemnedWindows != win)
      for (w = pNSCondemnedWindows; w->Next() != win; w = w->Next())
        if (!w->Next())
          return ;

    // TRACEX(OwlApp, 1, "Uncondemning window @" << (void*)win << *win);
    if (w)
      w->SetNext(win->Next()) ;
    else
      pNSCondemnedWindows = win->Next() ;
  }
}

//
// Walk the condemned window list & delete each window. Assumes that the
// windows were constructed using 'new'
//
void
TMyApp::DeleteCondemned()
{
	// OWL::TApplication::DeleteCondemned() ;

  while (pNSCondemnedWindows)
  {
    // TRACEX(OwlApp, 1, "Deleting condemned window @" << CondemnedWindows << *CondemnedWindows);
    TWindow* next = pNSCondemnedWindows->Next() ;
    delete pNSCondemnedWindows ;
    pNSCondemnedWindows = next ;
  }
}

void
TMyApp::initCurrentDir()
{
	const  initbuf = 256 ;
	size_t buflen  = initbuf ;
	char *buf = reinterpret_cast<char *>(malloc(buflen)) ;

	if (buf)
	{
		buflen = GetCurrentDirectory(buflen, buf) ;
		if (buflen > initbuf)
		{
			char *p = reinterpret_cast<char *>(realloc(buf, buflen)) ;
			if (p)
      {
				buf = p ;
        buflen = GetCurrentDirectory(buflen, buf) ;
      }
  	}
	}

	if ((buflen > 0) && (NULL != buf) && (NULL != appContext))
		appContext->setBaseDirectory(string(buf)) ;

	free(buf) ;
}

DEFINE_RESPONSE_TABLE1(NSMDIFrame, TDecoratedMDIFrame)
  EV_WM_QUERYENDSESSION,
  EV_WM_ENDSESSION,
  EV_COMMAND(IDM_TODO, CmToDo),
  EV_WM_TIMER,
END_RESPONSE_TABLE;

NSMDIFrame::NSMDIFrame(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd, bool trackMenuSelection, TModule* module)
           :TDecoratedMDIFrame(title, menuResId, clientWnd, trackMenuSelection, module),
            NSRoot(pCtx)
{
	// Printer = new NSPrinter ;
}

NSMDIFrame::~NSMDIFrame()
{
	// delete Printer;
  // Printer = 0;
}


void
NSMDIFrame::SetupWindow()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("SetupWindow for NSMDIFrame ") + string(szThis) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  TDecoratedMDIFrame::SetupWindow() ;

  TMyApp* pApp = pContexte->getSuperviseur()->getApplication() ;
  char szApp[20] ;
  sprintf(szApp, "%p", pContexte->GetMainWindow()->HWindow) ;
  sMsg = string("- Main Window ") + string(szApp) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szStatusBar[20] ;
  if (pApp->getStatusBar())
    sprintf(szStatusBar, "%p", pApp->getStatusBar()->HWindow) ;
  else
    strcpy(szStatusBar, "not created") ;
  sMsg = string("- Status bar ") + string(szStatusBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szControlBar[20] ;
  if (pApp->getControlBar())
    sprintf(szControlBar, "%p", pApp->getControlBar()->HWindow) ;
  else
    strcpy(szControlBar, "not created") ;
  sMsg = string("- Control bar ") + string(szControlBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szHarbor[20] ;
  if (pApp->getHarbor())
    sprintf(szHarbor, "%p", pApp->getHarbor()->HWindow) ;
  else
    strcpy(szHarbor, "not created") ;
  sMsg = string("- Harbor ") + string(szHarbor) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  InitAutomaticImportTimers() ;

  // On reroute le parent de l'appli VCL vers la frame OWL
  AdoptVCLAppWindow(GetHandle()) ;

  // Create Pilot Proxy window
  //
  NSPilotProxyWindow* pPilotProxyWindow = new NSPilotProxyWindow(pSuper->donneMainWindow(), pSuper) ;
	pSuper->setPilotProxyWindow(pPilotProxyWindow) ;

	NSPilotProxyChild* pPilotProxyChild = new NSPilotProxyChild(pSuper, *(pSuper->getApplication()->prendClient()), 0, pPilotProxyWindow) ;
  pPilotProxyChild->SetCaption("*System* Pilot proxy") ;
  // NSPilotProxyChild* pPilotProxyChild = new NSPilotProxyChild(pSuper, *Client, 0, pPilotProxyWindow) ;

	pPilotProxyChild->Create() ;
	pPilotProxyWindow->SetParent(pPilotProxyChild) ;
  pPilotProxyChild->Show(SW_HIDE) ;
}

void
NSMDIFrame::InitAutomaticImportTimers()
{
  string sSupport = string("") ;
  bool   bVerbose = false ;
  string sDocType = string("") ;

  // Get parameters for "scan"
  //
  string sScanImportDirectory = pContexte->PathName(string("ASCN"), &sSupport, bVerbose, &sDocType) ;
  if (string("") == sScanImportDirectory)
  {
    string sWarning = string("No automatic import directory found for scan (path \"ASCN\" not defined)") ;
    pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
  }
  else
  {
/*
    if (string("") == sDocType)
    {
      string sWarning = string("No document type found for automatic scan import ($TYPE_DOC is not defined for path \"ASCN\")") ;
      pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
    }
    else
    {
      SetTimer(ID_SCAN_TIMER, 5000) ;

      string sWarning = string("Automatic scanned files import process set for document type ") + sDocType + (" in directory ") + sScanImportDirectory ;
      pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
    }
*/

    SetTimer(ID_SCAN_TIMER, 5000) ;

    string sWarning = string("Automatic scanned files import process set in directory ") + sScanImportDirectory ;
    pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
  }

  // Get parameters for "images"
  //
  string sImageImportDirectory = pContexte->PathName(string("AIMG"), &sSupport, bVerbose, &sDocType) ;
  if (string("") == sImageImportDirectory)
  {
    string sWarning = string("No automatic import directory found for images (path \"AIMG\" not defined)") ;
    pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
  }
  else
  {
/*
    if (string("") == sDocType)
    {
      string sWarning = string("No document type found for automatic images import ($TYPE_DOC is not defined for path \"AIMG\")") ;
      pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
    }
    else
    {
      SetTimer(ID_IMAGE_TIMER, 5000) ;

      string sWarning = string("Automatic scanned files import process set for document type ") + sDocType + (" in directory ") + sImageImportDirectory ;
      pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
    }
*/

    SetTimer(ID_IMAGE_TIMER, 5000) ;

    string sWarning = string("Automatic images import process set in directory ") + sImageImportDirectory ;
    pContexte->getSuperviseur()->trace(&sWarning, 1, NSSuper::trSteps) ;
  }
}

void
NSMDIFrame::EvTimer(uint timerId)
{
	if      (ID_SCAN_TIMER == timerId)
    CheckForAutomaticImport(isScan) ;
  else if (ID_IMAGE_TIMER == timerId)
    CheckForAutomaticImport(isImage) ;
}

//
// Look for the MDI submenu in a menubar by looking for the normal MDI
// commands, and return pos if found. Scan from right to left since the Window
// menu is usually near the right.
//
HMENU
NSMDIFrame::FindChildMenu(HMENU menu)
{
  if (menu) {
    int   numItems = ::GetMenuItemCount(menu);
    for (int i = numItems-1; i >= 0; i--) {
      HMENU childMenu = ::GetSubMenu(menu, i);
      if (childMenu &&
          (TUser::GetMenuState(childMenu, CM_CASCADECHILDREN, MF_BYCOMMAND) != (uint)-1 ||
           TUser::GetMenuState(childMenu, CM_TILECHILDREN, MF_BYCOMMAND) != (uint)-1 ||
           TUser::GetMenuState(childMenu, CM_ARRANGEICONS, MF_BYCOMMAND) != (uint)-1)) {
        return childMenu;
      }
    }
  }
  return 0;
}

// Note that windows are enumerated in top-down Z-order, so the
// menu window should always be the first one found.
//
static BOOL CALLBACK GetPopupMenuEnumProc(HWND hwnd, LPARAM lParam)
{
  char buf[16] ;
  GetClassName(hwnd, buf, sizeof(buf)) ;
  if (strcmp(buf, "#32768") == 0)  // special classname for menus
  {
    *((HWND*) lParam) = hwnd ;    // found it
    return FALSE ;
  }
  return TRUE ;
}

// Find the window handle of popup menu
//
HWND
NSMDIFrame::GetPopupMenuWindowHandle()
{
  HWND hwnd = NULL ;
  EnumWindows(GetPopupMenuEnumProc, (LPARAM) &hwnd) ;
  return hwnd ;
}

// Check if new files appear in import directory
//
void
NSMDIFrame::CheckForAutomaticImport(TIMERIMPORTTYPES iImportType)
{
  if (NULL == pContexte->getPatient())
    return ;

  NSPatientChoisi *pPatient = pContexte->getPatient() ;
  if (false == pPatient->isOpen())
    return ;

  string sPathCode = string("") ;

  switch(iImportType)
  {
    case isScan :
      sPathCode = string("ASCN") ;
      break ;
    case isImage :
      sPathCode = string("AIMG") ;
      break ;
    default:
      return ;
  }

  string sSupport = string("") ;
  bool   bVerbose = false ;
  string sDocType = string("") ;

  string sImportDirectory = pContexte->PathName(sPathCode, &sSupport, bVerbose, &sDocType) ;
  if (string("") == sImportDirectory)
    return ;

  //
	// Looking for all files in directory
	//
  string sSearchPath = sImportDirectory + string("*.*") ;

  size_t iPathLength = strlen(sSearchPath.c_str()) + 1 ;
	char* szSearchPattern = new char[iPathLength] ;
  memset(szSearchPattern, 0, sizeof(iPathLength)) ;
  strcpy(szSearchPattern, sSearchPath.c_str()) ;

  // Looking for first file
  //
  WIN32_FIND_DATA stResult ;
	HANDLE hFoundFileHandle = FindFirstFile(szSearchPattern, &stResult) ;
	delete[] szSearchPattern ;

	//
	// If no file can be found, we leave
	//
	if (INVALID_HANDLE_VALUE == hFoundFileHandle)
		return ;  VectString _aFilesInDir ;  VectString *pPreviousFilesInDir = (VectString *) 0 ;  // We have to kill timer if we don't want the same document(s) to pile up  //  switch(iImportType)  {
    case isScan :
      KillTimer(ID_SCAN_TIMER) ;
      pPreviousFilesInDir = &(pPatient->_aFilesInScansDir) ;
      break ;
    case isImage :
      KillTimer(ID_IMAGE_TIMER) ;
      pPreviousFilesInDir = &(pPatient->_aFilesInImagesDir) ;
      break ;
  }

  //
	// While more files can be found
	//
	bool bTrouve = true ;
	while (bTrouve)
	{
		string sFileName = string(stResult.cFileName) ;

    _aFilesInDir.push_back(new string(sFileName)) ;

    if ((string(".") != sFileName) && (string("..") != sFileName) &&
        (false == pPreviousFilesInDir->contains(sFileName)))
    {
      string sNomSeul   = string("") ;
		  string sExtension = string("") ;

		  size_t pos1 = sFileName.find_last_of(".") ;
		  if (pos1 == NPOS)
			  sNomSeul = sFileName ;
		  else
		  {
  		  sNomSeul   = string(sFileName, 0, pos1) ;
    	  sExtension = string(sFileName, pos1+1, strlen(sFileName.c_str())-pos1-1) ;
		  }

      // Get Mime type
      //
/*
      NSTypeMimeInfo infoTypeMime ;
      if (pContexte->getSuperviseur()->chercheTypeMimeInfo(sExtension, &infoTypeMime))
      {
        if (infoTypeMime.getType() == sDocType)
        {
          string ps = string("CheckForAutomaticImport import file ") + string(sFileName) ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

		      pContexte->getPatient()->Importer(sImportDirectory + sFileName) ;
        }
      }
*/

      pContexte->getPatient()->Importer(sImportDirectory + sFileName, true) ;
    }

    bTrouve = FindNextFile(hFoundFileHandle, &stResult) ;
  }

  FindClose(hFoundFileHandle) ;

  *pPreviousFilesInDir = _aFilesInDir ;

  // We can restart timers
  //
  switch(iImportType)
  {
    case isScan :
      SetTimer(ID_SCAN_TIMER, 5000) ;
      break ;
    case isImage :
      SetTimer(ID_IMAGE_TIMER, 5000) ;
      break ;
  }
}

bool
NSMDIFrame::EvQueryEndSession()
{
	if (pContexte->getSuperviseur()->getApplication()->prendClient()->CloseChildren())
	{
		bEndSession = true ;
		return true ;
	}
	else
	{
		bEndSession = false ;
		return false ;
	}
}

void
NSMDIFrame::EvEndSession(bool endSession, bool logOff)
{
  // Si fin de session :
  // on detruit le superviseur, ce qui permet de débloquer le patient en cours
  if (bEndSession)
  {

    NSSuper* pSuper = pContexte->getSuperviseur() ;
    if (pSuper)
    {
      string sMsg = string("NSMDIFrame::EvEndSession Uninitializing Ole") ;
      pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;
    }

    ::OleUninitialize() ;

    if (pSuper)
    {
      delete pSuper ;
      pSuper = (NSSuper*) 0 ;
    }
  }
}

static void
sCloseStripes(TWindow* win, void* HarborAsVoid)
{
  if ((NULL == HarborAsVoid) || (NULL == win))
    return ;

  THarborEx* Harbor = (THarborEx*) HarborAsVoid ;
  if (NULL == Harbor)
  	return ;

  TEdgeSlipEx *wsSlip = TYPESAFE_DOWNCAST(win, TEdgeSlipEx) ;
  if (NULL == wsSlip)
  	return ;

  TWindow* firstChild = wsSlip->GetFirstChild() ;
  if (NULL == firstChild)
  	return ;

  TWindow* child = firstChild ;

  do
  {
    TWindow* nextChild = child->Next() ;

    NsStripe *wsStripe = TYPESAFE_DOWNCAST(child, NsStripe) ;
    if (wsStripe)
    {
      // Harbor->Remove(*wsStripe) ;
      // wsStripe->Destroy() ;
      wsStripe->SendMessage(WM_COMMAND, IDOK) ;
      delete wsStripe ;
    }

    if (nextChild == child)
      return ;

    child = nextChild ;
  }
  while (child && (child != firstChild)) ;
}

void
NSMDIFrame::CloseStripes()
{
  ForEach(sCloseStripes, pContexte->getSuperviseur()->getApplication()->getHarbor()) ;
}

void
NSMDIFrame::CmToDo()
{
try
{
	if (NULL == pContexte)
		return ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;
	if (NULL == pSuper)
		return ;

	string ps ;

	if (pSuper->getToDoArray()->empty())
	{
		ps = string("CmToDo with an empty ToDo array") ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;
	}

	// Pour éviter les problèmes de synchronisation (un todo est ajouté
	// à l'array au milieu du traitement des todo précédents, on fait
	// une copie locale et on vide le réservoir global
  while (pSuper->isToDoLocked())
		pSuper->getApplication()->PumpWaitingMessages() ;

	pSuper->lockToDo() ;

  for (TaskIter iTD = pSuper->getToDoArray()->begin() ; pSuper->getToDoArray()->end() != iTD ; iTD++)
	{
		ps = string("globalToDo ") + (*iTD)->getWhatToDo() + string(" sP1=") + (*iTD)->getParam1() + string(" sP2=") + (*iTD)->getParam2() ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
	}

	NSTaskArray aLocalToDo = *(pSuper->getToDoArray()) ;
	pSuper->getToDoArray()->vider() ;

	pSuper->unlockToDo() ;

	for (TaskIter iTD = aLocalToDo.begin() ; aLocalToDo.end() != iTD ; iTD++)
	{
		ps = string("localToDo ") + (*iTD)->getWhatToDo() + string(" sP1=") + (*iTD)->getParam1() + string(" sP2=") + (*iTD)->getParam2() ;
		pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
	}

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

  if (aLocalToDo.empty() == false)
	for (TaskIter iTD = aLocalToDo.begin() ; aLocalToDo.end() != iTD ; )
	{
		ps = string("début Todo : ") + (*iTD)->getWhatToDo() ;
		pSuper->trace(&ps, 1) ;

		if      ((*iTD)->getWhatToDo() == "Archetype")
    	ToDo_Archetype(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "KS_Archetype")
			ToDo_KS_Archetype(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "KS_DecisionTree")
			ToDo_KS_DecisionTree(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "FermeArchetype")
    	ToDo_FermeArchetype(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "Referential")
    {
      if ((pContexte->getUtilisateur() == 0) || (pContexte->getPatient() == 0))
        return ;

      pContexte->getPatient()->CmReferentiel((*iTD)->getParam1(), "", ldvframeHealth) ;
    }
    else if ((*iTD)->getWhatToDo() == "CreerBibArch")
    	ToDo_CreerBibArch(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "CreerBibChem")
    	ToDo_CreerBibChem(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "FermeDPIO")
    	ToDo_FermeDPIO(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "ActivateMUEView")
    	ToDo_ActivateMUEView(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "NewPreoccup")
    	ToDo_NewPreoccup(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "NewPreoccupProposal")
    	ToDo_NewPreoccupProposal(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "NewPreoccupFromNode")
    	ToDo_NewPreoccupFromNode(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "NewDrugFromNode")
    	ToDo_NewDrugFromNode(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "ChangePreoccupFromNode")
    	ToDo_ChangePreoccupFromNode(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "DisplayAlert")
    	ToDo_Message(*iTD, NsStripe::isAlert) ;

    else if ((*iTD)->getWhatToDo() == "DisplayWarning")
    	ToDo_Message(*iTD, NsStripe::isWarning) ;

    else if ((*iTD)->getWhatToDo() == "DisplayMessage")
    	ToDo_Message(*iTD, NsStripe::isInformation) ;

		// Ouverture d'une Fonction - Function opening
		else if ((*iTD)->getWhatToDo() == "OpenNewWindow")
			ToDo_OpenNewWindow(*iTD) ;

    else if ((*iTD)->getWhatToDo() == "FocusWindow")
    {
    	NSMUEView* pView = (static_cast<NSMUEView *>((*iTD)->getPointer1())) ;
      if (pView)
        pView->SetFocus() ;
    }
    else if ((*iTD)->getWhatToDo() == "NavigateComplete")
    {
      NSVisualView* pView = (static_cast<NSVisualView *>((*iTD)->getPointer1())) ;

      if (pView)
      {
      	// temporisation avant de lancer la publication
      	if (pView && pView->iTempPostNav > 0)
        	pContexte->getSuperviseur()->Delay(pView->iTempPostNav) ;      	// publication [impression] du document (ExecWB)      	pView->CmPublier() ;
      }
    }
    else if ((*iTD)->getWhatToDo() == "AddDatasOnBBK")
    {
      NSPatPathoArray *pPt  = (NSPatPathoArray *)((*iTD)->getPointer1()) ;
      if (pPt)
        addDatasFromNautilusOnBlackboard(pContexte, pPt) ;
    }
    else if ((*iTD)->getWhatToDo() == "AddToSOAP")
    {
    	NSEpisodus* pEpisod = pContexte->getSuperviseur()->getEpisodus() ;
      if (pEpisod)
      {
      	NSCapture *pPt = (NSCapture *)((*iTD)->getPointer1()) ;
      	if (pPt)
        	pEpisod->addToSOAP(pPt) ;
        else
        {
        	SOAPObject *pPt2 = (SOAPObject *)((*iTD)->getPointer1()) ;
      		if (pPt2)
        		pEpisod->addToSOAP(pPt2) ;
        }
      }
    }
		else if ((*iTD)->getWhatToDo() == "RefreshHealthTeamInformation")
    {
    	NSPatientChoisi* pPatient = pContexte->getPatient() ;
      if (pPatient && pPatient->_pHealthDoc)
      	pPatient->_pHealthDoc->invalidateViews((*iTD)->getParam1(), (*iTD)->getParam2()) ;
		}
    else if ((*iTD)->getWhatToDo() == "NavigateEncyclopedia")
    {
      NSTreeNode* pNSTreeNode = (static_cast<NSTreeNode *>((*iTD)->getPointer1())) ;

      if (pNSTreeNode && (string("") != pNSTreeNode->getUrl()))
      {
        string sLink = pNSTreeNode->getUrl() ;

	      string sClassif ;
	      string sTemp = pNSTreeNode->getEtiquette() ;
	      pSuper->getDico()->donneCodeSens(&sTemp, &sClassif);
        //
	      // If it is a classification, we have to check if we must adapt the URL
        //
	      if (pContexte->getSuperviseur()->getFilGuide()->VraiOuFaux(sClassif, "ES", "0CODE"))
        {
          size_t pos = sLink.find(sClassif) ;
          if (string::npos != pos)
          {
            string sCode = pNSTreeNode->getFreeTextCode() ;
            if (strlen(sLink.c_str()) > pos - strlen(sClassif.c_str()))
              sLink = string(sLink, 0, pos) + sCode + string(sLink, pos + strlen(sClassif.c_str()), strlen(sLink.c_str()) - pos - strlen(sClassif.c_str())) ;
            else
              sLink = string(sLink, 0, pos) + sCode ;
          }
        }

        pContexte->getSuperviseur()->NavigationEncyclopedie(sLink, pContexte) ;
      }
    }
    else if ((*iTD)->getWhatToDo() == "ExportDocForDeletedCompt")
    {
      ToDo_ExportDocForDeletedCompt(*iTD) ;
		}
    else if ((*iTD)->getWhatToDo() == "GetPersonAsynch")
    {
      ToDo_GetPersonAsynch(*iTD) ;
		}
    else if ((*iTD)->getWhatToDo() == "OpenDocument")
    {
      ToDo_OpenDocument(*iTD) ;
		}

    ps = string("fin Todo : ") + (*iTD)->getWhatToDo() ;
    pContexte->getSuperviseur()->trace(&ps, 1) ;

    delete (*iTD) ;
    aLocalToDo.erase(iTD) ;
  }
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception NSMDIFrame::CmToDo : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception NSMDIFrame::CmToDo.", standardError, 0) ;
}
}

void
NSMDIFrame::ToDo_Archetype(NSToDoTask* pToDo)
{
  if (NULL == pToDo)
    return ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;
	string ps = string("Todo Archetype : ") + pToDo->getParam1() ;
  pSuper->trace(&ps, 1) ;

  string sArchetype = pToDo->getParam1() ;
  string sTypeArc   = pToDo->getParam2() ;
  int iTypeArc      = atoi(sTypeArc.c_str()) ;

  if (iTypeArc == NSCQDocument::creatpat)
  {
  	// En cas de création d'un nouveau patient
    // on ferme l'eventuel patient en cours
    if (pContexte->getPatient())
    	pContexte->getUtilisateur()->fermePatEnCours() ;
  }

  ps = string("Todo Archetype : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSCQDocument* pCQDoc = new NSCQDocument(0, pContexte, sArchetype, iTypeArc) ;
  if (pCQDoc->bParsingOk)
  {
  	ps = string("Todo Archetype : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pCQDoc, "CQ Format") ;
  }
  else
  {
  	delete pCQDoc ;
    ps = string("Todo Archetype : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }
}

voidNSMDIFrame::ToDo_KS_Archetype(NSToDoTask* pToDo){#ifdef __OB1__  if (NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;	BB1BBInterfaceForKs* pKSinterface = (static_cast<BB1BBInterfaceForKs *>(pToDo->getPointer1())) ;
  string ps ;

  if (NULL == pKSinterface)
  {
  	ps = string("Todo KS_Archetype with wrong BB1BBInterfaceForKs pointer") ;
    pSuper->trace(&ps, 1) ;
    return ;
  }

  BB1BBInterfaceForKs ksInterface = *pKSinterface ;

  string sArchetype = ksInterface.getArchetypeID() ;
  string sKSname    = ksInterface.getKsName() ;

  ps = string("Todo KS_Archetype : ") + sArchetype + string(" for KS : ") + sKSname ;
  pSuper->trace(&ps, 1) ;

  ps = string("Todo Archetype : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSCQDocument* pCQDoc = new NSCQDocument(0, pContexte, &ksInterface) ;
  if (pCQDoc->bParsingOk)
  {
  	ps = string("Todo Archetype : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pCQDoc, "CQ Format") ;
  }
  else
  {
  	delete pCQDoc ;
    ps = string("Todo Archetype : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }

  if (false == pToDo->mustDeleteP1())
  {
  	delete pKSinterface ;
    pToDo->setPointer1(0) ;
  }
#endif
}
voidNSMDIFrame::ToDo_KS_DecisionTree(NSToDoTask* pToDo){#ifdef __OB1__  if (NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;	BB1BBInterfaceForKs* pKSinterface = (static_cast<BB1BBInterfaceForKs *>(pToDo->getPointer1())) ;
  string ps ;

  if (NULL == pKSinterface)
  {
  	ps = string("Todo Decision tree with wrong BB1BBInterfaceForKs pointer") ;
    pSuper->trace(&ps, 1) ;
    return ;
  }

  BB1BBInterfaceForKs ksInterface = *pKSinterface ;

  string sArchetype = ksInterface.getArchetypeID() ;
  string sKSname    = ksInterface.getKsName() ;

  ps = string("Todo Decision tree : ") + sArchetype + string(" for KS : ") + sKSname ;
  pSuper->trace(&ps, 1) ;

  ps = string("Todo Decision tree : ") + sArchetype + string(" création du document") ;
  pSuper->trace(&ps, 1) ;

  // création d'un document vierge (on transmet l'archetype)
  // on lance le modèle document/vues formulaire
  NSDecisionTreeDocument* pDecTreeDoc = new NSDecisionTreeDocument(0, pContexte, &ksInterface) ;
  if (pDecTreeDoc->getParsingOk())
  {
  	ps = string("Todo Decision tree : ") + sArchetype + string(" création de la vue") ;
    pSuper->trace(&ps, 1) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pDecTreeDoc, "Decision Tree") ;
  }
  else
  {
  	delete pDecTreeDoc ;
    ps = string("Todo Decision tree : ") + sArchetype + string(" abort (mauvais Archetype)") ;
    pSuper->trace(&ps, 1) ;
  }

  if (false == pToDo->mustDeleteP1())
  {
  	delete pKSinterface ;
    pToDo->setPointer1(0) ;
  }
#endif
}
voidNSMDIFrame::ToDo_FermeArchetype(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	NSSuper *pSuper = pContexte->getSuperviseur() ;  if (NULL == pSuper)  	return ;	string ps = string("Début Todo : FermeArchetype") ;  pSuper->trace(&ps, 1) ;

  string sLang = string("") ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

  NSSmallBrother  *pBB   = (static_cast<NSSmallBrother *>(pToDo->getPointer1())) ;
  NSCQWindowsView *pView = (static_cast<NSCQWindowsView *>(pToDo->getPointer2())) ;
  string           sName = pToDo->getParam1() ;

  if (NULL == pBB)
    return ;

  // Empty patpatho
  //
  if (pBB->getPatPatho()->empty())
  {
    string ps = string("NSMDIFrame::ToDo_FermeArchetype TMyapp Init main window: Entering.") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    string sWarnMsg = pSuper->getText("documentManagementErrors", "cannotSaveAnEmptyDocument") ;
    erreur(sWarnMsg.c_str(), warningError, 0) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    return ;
  }

  bool existeInfo ;
  NSCQDocument *pDocum = pView->_pDoc ;
  *(pDocum->pPatPathoArray) = *(pBB->getPatPatho()) ;

  if (NSCQDocument::creatpat == pDocum->iTypeDoc)
  {
    NSPatPathoArray aPPt(pContexte->getSuperviseur()) ;
    aPPt = *(pDocum->pPatPathoArray) ;
    pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

    if (pContexte->getUtilisateur()->createPatient(&aPPt))
      pContexte->getPatient()->Initialisation() ;
    else
      pContexte->setPatient(NULL) ;

    pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    pBB->canCloseArchetype(true, false) ;
  }
  else if (NSCQDocument::creatadr == pDocum->iTypeDoc)
  {
    NSAddressGraphManager  *pGraphManager = new NSAddressGraphManager(pSuper) ;
    pGraphManager->setGraphAdr(pDocum->pPatPathoArray, sLang) ;
    delete pGraphManager ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    pBB->canCloseArchetype(true) ;
  }
  else if (pDocum->pDocInfo)
  {
    bool bIsDemographicArchetype = false ;
    if (pContexte->getSuperviseur()->getDemographicArchetypeId() == pDocum->_sDefaultArchetype)
      bIsDemographicArchetype = true ;

    // string sArchetype = string("") ;
    // if ((NULL != pDocum->pPatPathoArray) && (false == pDocum->pPatPathoArray->empty()))
    // 	sArchetype = (*(pDocum->pPatPathoArray->begin()))->getArchetype() ;

    if (true == bIsDemographicArchetype)
    {
      // Manage IPP
      //
      string sIPP = string("") ;
      NSPatPathoArray* pPatPathoAdmin = pDocum->pPatPathoArray ;

      pContexte->getUtilisateur()->getAndRemoveIppFromPatpatho(pPatPathoAdmin, &sIPP) ;

      if ((string("") != sIPP) && (string("") != pSuper->getIppSite()))
        pContexte->getPatient()->updateIPPEnCours(pSuper->getIppSite(), sIPP) ;
    }

    pDocum->enregistrePatPatho() ;
    // on recharge la patpatho pour récupérer les nouveaux nodes
    // Reloading the patpatho in order to freshen nodes
    pDocum->NSNoyauDocument::chargePatPatho() ;
    // string sArchetype = (*(pDocum->pPatPathoArray->begin()))->pDonnees->getArchetype() ;

    pContexte->getPatient()->getDocHis()->Rafraichir(pDocum->pDocInfo, pDocum->pPatPathoArray) ;

    if (pView)
      pView->setWaitingForSystem(false) ;

    if (false == bIsDemographicArchetype)
    {
      ps = string("Fermeture effective du document ") + pDocum->sArchetype ;
      pSuper->trace(&ps, 1) ;
      pBB->canCloseArchetype(true) ;
    }
    else
      pContexte->getPatient()->PatientDataChanged(pDocum->pPatPathoArray) ;
  }
  else
  {
    // cas des nouveaux documents formulaire
    string sLibelleDoc = sName ;
    if (string("") == sLibelleDoc)
    {
      string sCode = (*(pBB->getPatPatho()->begin()))->getLexique() ;
      if (string("") != sCode)
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
    }

    string ps = string("Fermeture formulaire ") + sLibelleDoc ;
    pSuper->trace(&ps, 1) ;

    ps = string("Referencement formulaire ") + sLibelleDoc ;
    pSuper->trace(&ps, 1) ;

    // Référencement sans boite de dialogue
    existeInfo = pDocum->Referencer("ZCQ00", sLibelleDoc, "", "", true, false) ;

    if (existeInfo)
    {
      ps = string("Enregistrement de l'arbre du formulaire ") + sLibelleDoc ;
      pSuper->trace(&ps, 1) ;
      existeInfo = pDocum->enregistrePatPatho() ;
    }

    // on recharge la patpatho pour récupérer les nouveaux nodes
    if (existeInfo)
    {
      existeInfo = pDocum->NSNoyauDocument::chargePatPatho() ;

      // This is done later... and it leads to showing it to BBK twice
      // pContexte->getPatient()->pDocHis->Rafraichir(pDocum->pDocInfo, pDocum->pPatPathoArray) ;
    }

    if (existeInfo)
    {
      pContexte->getPatient()->getLdvDocument()->showNewTree(pDocum->pPatPathoArray, pDocum->GetDateDoc(false)) ;
      pDocum->makeSOAP() ;
    }

    NSPredi *pPredi = pSuper->getPredi() ;

    if (existeInfo)
    {
      string sRacineDoc = pDocum->DonneDocInfo()->getID() ;

      if (pPredi->_bSendPage)
      {
        string sDateDoc  = pDocum->GetDateDoc(false) ;
        string sDateDPIO = string(sDateDoc, 6, 2) + string("/") + string(sDateDoc, 4, 2) + string("/") + string(sDateDoc, 0, 4) ;

        pPredi->_sSendPage += string("<document code=\"") + sRacineDoc + string("\" date=\"") + sDateDPIO + string("\">\r\n") + pDocum->pPatPathoArray->genereXML() + string("</document>\r\n") ;
      }

      // Rafraichir l'historique
      pContexte->getPatient()->getDocHis()->Rafraichir(pDocum->pDocInfo, pDocum->pPatPathoArray/*, NSNoyauDocument* pNouveauDocument*/) ;

      // Archetype ?
      string sNomRef = string("") ;

      if (pBB && pBB->getBBItem() && pBB->getBBItem()->getParseur() && pBB->getBBItem()->getParseur()->getArchetype())
      {
        Carchetype *pArchetype = pBB->getBBItem()->getParseur()->getArchetype() ;

        if (pArchetype->getDialog())
          sNomRef = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_REF) ;

        // Connexion à (ouverture de) la préoccupation
        string sConcernCode = string("") ;
        // bool    bConnected      = false ;

        Creferences *pRef = pArchetype->getReference() ;
        if (pRef)
        {
          Cconcern *pConcern = pRef->getFirstCconcern() ;
          if (pConcern)
          {
            sConcernCode = pConcern->getCode() ;

            bool bCreate = pConcern->getAutoCreate() ;
            int  iSevere = pConcern->getSeverity() ;

            string sFrameCode = pConcern->getFrame() ;

            if ((string("") != sConcernCode) && (pContexte->getPatient()) && (pContexte->getPatient()->getLdvDocument()))
            {
              ps = string("Connection à sa preoccupation du formulaire ") + sLibelleDoc ;
              pContexte->getSuperviseur()->trace(&ps, 1) ;
              NSLdvDocument *pDocLdv = pContexte->getPatient()->getLdvDocument() ;

              if (('I' == sConcernCode[0]) || ('_' == sConcernCode[0]))
              {
                bool bConnected = false ;
                for (int i = 0 ; (i < FRAMECOUNT) && (false == bConnected) ; i++)
                {
                  LDVFRAME iFrame = getFrameForIndex(i) ;
                  bConnected = pDocLdv->connectObjectToDrug(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
                }
              }
              else if ('#' != sConcernCode[0])
              {
                LDVFRAME iFrame = ldvframeHealth ;
                if (string("") != sFrameCode)
                  iFrame = pDocLdv->getFrameTypeFromRoot(sFrameCode) ;

                /*bConnected =*/ pDocLdv->connectObjectToConcern(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
              }
              // Si sConcernCode[0] == '#', c'est qu'on se connecte à
              // la préoccupation lanceuse du formulaire
              else
              {
                string sPreoType = string(sConcernCode, 1, strlen(sConcernCode.c_str()) - 1) ;
                string sQuestion = string("0PRO1/IsA/") + sPreoType ;
                NSPatPathoArray *pAnswer = NULL ;
#ifdef __OB1__
                string sAnswerDate ;
                if ((pContexte->getBBinterface()) && (pContexte->getBBinterface()->getAnswer2Question(sQuestion, "", &pAnswer, sAnswerDate, false)))
#else
                if (pSuper->getBlackboard()->getAnswer2Question(sQuestion, "", &pAnswer, false))
#endif // !__OB1__
                {
                  if (pAnswer && (false == pAnswer->empty()))
                  {
                    PatPathoIter iter = pAnswer->begin() ;
                    string sLaunchingPreoc = (*iter)->getLexique() ;
                    LDVFRAME iFrame = ldvframeNotSpecified ;
                    NSConcern* pConcern = pDocLdv->getConcern((*iter)->getNode(), iFrame) ;
                    if (pConcern)
                      pDocLdv->connectObjectToConcern(iFrame, sLaunchingPreoc, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
                  }
                }
                if (pAnswer)
                  delete pAnswer ;
              }
            }
          }
        }

        // Lancement du référentiel
        if (string("") != sNomRef)
        {
          ps = string("Lancement du referentiel pour le formulaire ") + sLibelleDoc ;
          pSuper->trace(&ps, 1) ;
          pContexte->getPatient()->CmReferentiel(sNomRef, sRacineDoc, ldvframeHealth) ;
        }

        if ((NSCQDocument::dpio == pDocum->iTypeDoc) && pPredi && pPredi->mustLogPage())
        {
          ps = string("Log PréDI du formulaire ") + sLibelleDoc ;
          pSuper->trace(&ps, 1) ;
          // pDPIO->sLogPage += pDocum->donneTexte() ;
          pPredi->AppendLogFile() ;
        }

        // Fermeture du questionnaire
        // A faire en dernier car pDocum disparait à la fermeture de la vue
        ps = string("Fermeture effective du formulaire ") + sLibelleDoc ;
        pSuper->trace(&ps, 1) ;

        if (pView)
          pView->setWaitingForSystem(false) ;

        pBB->canCloseArchetype(true) ;
        // Fait par la vue
        //delete pBB ;
      }
      else if (pBB)
      {
        if (pView)
          pView->setWaitingForSystem(false) ;
        pBB->canCloseArchetype(true) ;
        // Fait par la vue
        //delete pBB ;
      }
    }
  }
}
voidNSMDIFrame::ToDo_CreerBibArch(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;  NSDocumentInfo DocInfo(pContexte) ;
  // Patient : Patient en cours
  DocInfo.setPatient("_------") ;

  // Code : par définition "_00000"
  string sCodeDocum = string("_") + string(DOC_CODE_DOCUM_LEN - 1, '0') ;
  DocInfo.setDocument(sCodeDocum) ;

  // Ne pas passer pDocInfo dans le cteur pour éviter qu'il essaie de
  // charger la patpatho
  NSRefDocument DocLibArch(0, 0, 0, pContexte, false) ;
  DocLibArch.pDocInfo = new NSDocumentInfo(DocInfo) ;
  DocLibArch.pPatPathoArray->ajoutePatho("0LIBA1", 0) ;

	DocLibArch.enregistrePatPatho() ;

  // IMPORTANT : on doit remettre pDocInfo et pHtmlInfo à 0
  // pour éviter que ~NSRefDocument ne remette à jour l'historique
  DocLibArch.pDocInfo = 0 ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
  MessageBox("La bibliothèque d'Archetypes a été créée avec succès.", sCaption.c_str(), MB_OK) ;}voidNSMDIFrame::ToDo_CreerBibChem(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	// Création de l'arbre-bibliothèque de chemises  if (pContexte->getPatient() == 0)
  {
    erreur("Vous devez ouvrir un patient avant de lancer cette option.", standardError, 0) ;
    return ;
  }

  char szDateJour[10] ;
  donne_date_duJour(szDateJour) ;

  NSRefDocument LibChem(0, 0, 0, pContexte, false) ;
  LibChem.pPatPathoArray->ajoutePatho("0LIBC1", 0) ;

  // ajout de la chemise corbeille
  // Note : cette chemise doit être créée en premier afin que la chemise par défaut
  // soit sélectionnée plus tard dans EnregDocDialog (on sélectionne la dernière chemise)
  LibChem.pPatPathoArray->ajoutePatho("0CHEM1", 1) ;

  // Intitulé : nom de la chemise
  LibChem.pPatPathoArray->ajoutePatho("0INTI1", 2) ;
  Message *pMessage = new Message ;
  pMessage->SetTexteLibre("corbeille") ;
  LibChem.pPatPathoArray->ajoutePatho("£?????", pMessage, 3) ;
  delete pMessage ;

  // Date d'ouverture
  LibChem.pPatPathoArray->ajoutePatho("KOUVR1", 2) ;
  string sDateCreation = string(szDateJour) + string("000000") ;
  pMessage = new Message ;
  pMessage->SetUnit("2DA021") ;
  pMessage->SetComplement(sDateCreation.c_str()) ;
  LibChem.pPatPathoArray->ajoutePatho("£T0;19", pMessage, 3) ;
  delete pMessage ;

  // ajout de la chemise par défaut
  LibChem.pPatPathoArray->ajoutePatho("0CHEM1", 1) ;

  // Intitulé : nom de la chemise
  LibChem.pPatPathoArray->ajoutePatho("0INTI1", 2) ;
  pMessage = new Message ;
  pMessage->SetTexteLibre("défaut") ;
  LibChem.pPatPathoArray->ajoutePatho("£?????", pMessage, 3) ;
  delete pMessage ;

  // Date d'ouverture
  LibChem.pPatPathoArray->ajoutePatho("KOUVR1", 2) ;
  pMessage = new Message ;
  pMessage->SetUnit("2DA021") ;
  pMessage->SetComplement(sDateCreation.c_str()) ;
  LibChem.pPatPathoArray->ajoutePatho("£T0;19", pMessage, 3) ;
  delete pMessage ;

  if (false == LibChem.Referencer("ZCS00", "Bibliothèque de chemises", "", "", true, false, "", NSRootLink::personFolderLibrary))
  {
    erreur("Impossible de referencer la bibliothèque de chemises du patient.", standardError, 0) ;
    return ;
  }

  if (false == LibChem.enregistrePatPatho())
  {
    erreur("Impossible d'enregistrer la bibliothèque de chemises du patient.", standardError, 0) ;
    return ;
  }
  LibChem.NSNoyauDocument::chargePatPatho() ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
  MessageBox("La bibliothèque de chemises a été créée avec succès.", sCaption.c_str(), MB_OK) ;}voidNSMDIFrame::ToDo_FermeDPIO(NSToDoTask* pToDo){try{  if ((NULL == pToDo) || (NULL == pToDo->getPointer1()))    return ;  NSPredi *pPredi = pContexte->getSuperviseur()->getPredi() ;  if (NULL == pPredi)    return ;  pPredi->fermeFormulaire(pToDo->getPointer1()) ;
/*  TMDIChild *pView = (static_cast<TMDIChild *>(pToDo->pPointer1)) ;  if (NULL == pView)
  	return ;

  //if (pView->pDoc->iTypeDoc == NSCQDocument::dpio)
  //{
  NSDPIO  *pDPIO = pContexte->getSuperviseur()->getDPIO() ;
  //TMDIChild* pChild = pView->Parent ;
  pDPIO->fermeFormulaire((TWindow*)pView) ;
  //}

  // PA 29/05/09
  // if (pView->IsWindow())
  //   delete pView ;
*/
}
catch (...)
{
  erreur("Exception ToDo_FermeDPIO.", standardError, 0) ;
}
}voidNSMDIFrame::ToDo_ActivateMUEView(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	// NSMUEView* pMueView = NULL ;  // if (pToDo->pPointer1)
  //  pMueView = (static_cast<NSMUEView *>(pToDo->pPointer1)) ;

  NSDialog* pDialog = NULL ;
  if (pToDo->getPointer2())
    pDialog = (static_cast<NSDialog *>(pToDo->getPointer2())) ;

  ::SetForegroundWindow(pContexte->GetMainWindow()->GetHandle());
  if (pDialog)
  {
    pDialog->_bActivateMUEView = false;
    ::SetActiveWindow(pDialog->GetHandle());
  }
  if (pContexte->GetMainWindow()->IsIconic())
    ::SetForegroundWindow(pContexte->GetMainWindow()->GetHandle());}// So far, this only creates a new concern in the health frame//voidNSMDIFrame::ToDo_NewPreoccup(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	// TWindow *pView  = (static_cast<TWindow *>(pToDo->pPointer1)) ;  string sLexique = pToDo->getParam1() ;
  string sDate    = pToDo->getParam2() ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  if (NULL == pLdvDoc)
    return ;

  // On vérifie que la préoccupation n'existe pas déjà
  //
  string sCodeSensConcern ;
  pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSensConcern) ;

  NSConcern* pConcern = pLdvDoc->donneOpenConcern(ldvframeHealth, sCodeSensConcern) ;
  if (NULL == pConcern)
  {
    string sNewNode = string("") ;
    string sCocCode = string("") ;
    NSPatPathoArray *pDetails = NULL ;

    string* psDate = NULL ;
    if (sDate != "")
      psDate = &sDate ;

    TWindow* pView = 0 ;
    NSLdvView *pLdvView = pLdvDoc->getLdvView() ;
    if (NULL != pLdvView)
      pView = pLdvView ;
    else
      pView = this ;

    NSNewConcernDlg *pNPDialog = new NSNewConcernDlg(pView, &sNewNode, pLdvDoc, pContexte, pDetails, &sCocCode, &sLexique, psDate) ;
    /* int iExecReturn = */ pNPDialog->Execute() ;
  }}voidNSMDIFrame::ToDo_NewPreoccupFromNode(NSToDoTask* pToDo){try{  if (NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  if ((NULL == pView) || (NULL == pNode))
  	return ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;

  string sNewNode = string("") ;
  string sLexique = pNode->getEtiquette() ;
  NSPatPathoArray Details(pContexte->getSuperviseur()) ;

  // Get possible CISP code
  //
  string sCocCode = pToDo->getParam1() ;

	NSNewConcernDlg *pNPDialog = 0 ;
  TDialog *pDlg = 0 ;

  TWindow* pControlParent = pView->Parent ;
  if (NULL != pControlParent)
	{
  	pDlg = TYPESAFE_DOWNCAST(pControlParent, TDialog) ;
    if (NULL != pDlg)
    	pNPDialog = new NSNewConcernDlg(pDlg, &sNewNode, pLdvDoc, pContexte, &Details, &sCocCode, &sLexique) ;
  }

	if (NULL == pNPDialog)
		pNPDialog = new NSNewConcernDlg(pView, &sNewNode, pLdvDoc, pContexte, &Details, &sCocCode, &sLexique) ;
  int iExecReturn = pNPDialog->Execute() ;
  delete pNPDialog ;

  if (IDOK == iExecReturn)
  {
    if (sNewNode != "")
      pNode->addTemporaryLink(sNewNode, NSRootLink::problemContactElement, dirFleche) ;
    pLdvDoc->getConcerns(ldvframeHealth)->reinit() ;
  }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception ToDo_NewPreoccupFromNode : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("Exception ToDo_NewPreoccupFromNode.", standardError, 0) ;
}
}// Creation of a proposal stripe//voidNSMDIFrame::ToDo_NewPreoccupProposal(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;  NSConcernToDoInformation *pConcernToDo = (static_cast<NSConcernToDoInformation *>(pToDo->getPointer1())) ;  if (NULL == pConcernToDo)    return ;	// TWndow *pView  = (static_cast<TWindow *>(pToDo->pPointer1)) ;  string sLexique = pConcernToDo->getLexique() ;
  string sDate    = pConcernToDo->getStartingDate() ;

  NSLdvDocument *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  if (NULL == pLdvDoc)
    return ;

  // On vérifie que la préoccupation n'existe pas déjà
  //
  string sCodeSensConcern ;
  pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSensConcern) ;

  NSConcern* pConcern = pLdvDoc->donneOpenConcern(ldvframeHealth, sCodeSensConcern) ;
  if (pConcern)
    return ;

  NsPreoccupStripe* stripe = new NsPreoccupStripe(pContexte, sLexique, sDate, pConcernToDo->getText(), pConcernToDo->getUrl(), this, NsStripe::isWarning) ;
  stripe->Create() ;
  pContexte->getSuperviseur()->getApplication()->getHarbor()->Insert(*stripe, OWL::alBottom) ;
  stripe->LayoutSession() ;
}

void
NSMDIFrame::ToDo_Message(NSToDoTask* pToDo, NsStripe::STRIPETYPE iStripeType)
{
  if (NULL == pToDo)
    return ;  string sText = pToDo->getParam1() ;  string sUrl  = pToDo->getParam2() ;

  NsAlertStripe* stripe = new NsAlertStripe(pContexte, sText, sUrl, 0, this, iStripeType) ;
  stripe->Create() ;
  pContexte->getSuperviseur()->getApplication()->getHarbor()->Insert(*stripe, OWL::alBottom) ;
  stripe->LayoutSession() ;
}
voidNSMDIFrame::ToDo_NewDrugFromNode(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  if ((!pView) || (!pNode))
  	return ;

  string sLexique = pNode->getEtiquette() ;

  NSLdvDocument   *pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
  NSDrugView      *pDrugView = pLdvDoc->getDrugView("") ;
  if (!pDrugView)
  {
    pContexte->getPatient()->drugs_show() ;
    for (int i = 0 ; (i < 50) && !pDrugView ; i++)
      pDrugView = pLdvDoc->getDrugView("") ;
  }

  if (pDrugView)
    pDrugView->CmAddWithCode(sLexique) ;
}voidNSMDIFrame::ToDo_ChangePreoccupFromNode(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	NSTreeWindow *pView = (static_cast<NSTreeWindow *>(pToDo->getPointer1())) ;  NSTreeNode   *pNode = (static_cast<NSTreeNode *>(pToDo->getPointer2())) ;
  string       sCcrn  = pToDo->getParam1() ;
  if ((!pView) || (!pNode))
  	return ;

  NSLdvDocument *pLdvDoc     = pContexte->getPatient()->getLdvDocument() ;
  NSConcern     *pOldConcern = pLdvDoc->getConcerns(ldvframeHealth)->getConcern(sCcrn) ;

  string sNewNode = string("") ;
  string sCocCode = string("") ;
  string sLexique = pNode->getEtiquette() ;
  NSPatPathoArray Details(pContexte->getSuperviseur()) ;

  NSNewConcernDlg *pNPDialog = new NSNewConcernDlg(pView, &sNewNode, pLdvDoc, pContexte, &Details, &sCocCode, &sLexique) ;
  int iExecReturn = pNPDialog->Execute() ;
  delete pNPDialog  ;

  if (IDOK == iExecReturn)
  {
    pNode->addTemporaryLink(sNewNode, NSRootLink::problemContactElement, dirFleche) ;
    pLdvDoc->getConcerns(ldvframeHealth)->reinit() ;

    // On part du principe que la nouvelle préoccupation est en fin d'array

    ArrayConcern  *pConcernArray  = pLdvDoc->getConcerns(ldvframeHealth) ;
    NSConcern     *pNewConcern    = pConcernArray->back() ;

    /* bool bSuccess = */ pLdvDoc->creeConcernSuite(ldvframeHealth, pOldConcern, pNewConcern) ;
  }
}voidNSMDIFrame::ToDo_OpenNewWindow(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;	// sParam1 = fonction - function  // sParam2 = préoccupation - concern
  // pPointer1 = fenêtre splittée - splitted window
  string sDocumentType = "" ;

  // Reservoir SOAP - SOAP tank
  if      (pToDo->getParam1() == "Tanking")
  {
    NSSOAPDocument  *pNewDocSOAP ;
    sDocumentType = "SoapDoc" ;
    NSMUEView       *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (!pSplittedView)
      return ;

    NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;
    NSMUEView       *pWindWithDocType = pPaneSpl->researchDocument("SoapDoc") ;
    if (!pWindWithDocType)
      pNewDocSOAP = new NSSOAPDocument(0, pContexte) ;
    else
      pNewDocSOAP = TYPESAFE_DOWNCAST(pWindWithDocType->pDoc, NSSOAPDocument) ;
    if (!pNewDocSOAP)
      return ;

    NSSOAPTankView      *pNSSOAPTankView = new NSSOAPTankView(*pNewDocSOAP, 0) ;

    if (pSplittedView->getCreateWindow())
      pNSSOAPTankView->setCreateWindow(pSplittedView->getCreateWindow()) ;
    else
      pNSSOAPTankView->setCreateWindow(pSplittedView) ;

    pNSSOAPTankView->setSplittedWindow(pSplittedView) ;

    NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pNewDocSOAP, "SOAP tank", pNSSOAPTankView) ;
  }
  // Paniers SOAP - SOAP baskets
  else if (pToDo->getParam1() == "Soaping")
  {
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (!pSplittedView)
    {
      // createSOAPView();
      NSSOAPDocument  *pNewDocSOAP    = new NSSOAPDocument(0, pContexte) ;
      NSSOAPView      *pNSSOAPView    = new NSSOAPView(*pNewDocSOAP, 0, "Soaping") ;

      pNSSOAPView->setCreateWindow(NULL) ;
      pNSSOAPView->setSplittedWindow(NULL) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pNewDocSOAP, "SOAP baskets", pNSSOAPView) ;
    }
  }
  // Médicaments - Drugs
  else if (pToDo->getParam1() == "DrugManagement")
  {
    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (!pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (!pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->drugs_show(pToDo->getParam2()) ;
    }
    else
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (!pPatEnCours)
      {
        erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
        return ;
      }

      // NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;
      pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if (NULL == pLdvDoc)
        return ;

      NSDrugView *pDrugView = new NSDrugView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pDrugView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pDrugView->setCreateWindow(pSplittedView) ;

      pDrugView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Drug Management", pDrugView) ;
    }
  }
  // Objectifs de santé - Health goals
  else if (pToDo->getParam1() == "GoalManagement")
  {
    NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
    if (!pPatEnCours)
    {
      erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
      return ;
    }
    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (!pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->goals_show(pToDo->getParam2()) ;
    }
    else
    {
      pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if (NULL == pLdvDoc)
        return ;

      NSGoalView *pGoalView = new NSGoalView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pGoalView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pGoalView->setCreateWindow(pSplittedView) ;

      pGoalView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Goal Management", pGoalView) ;
    }
  }
  // Follow Up
  else if (pToDo->getParam1() == "FollowUpManagement")
  {
    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (!pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->drugs_show(pToDo->getParam2()) ;
    }
    else
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (NULL == pPatEnCours)
      {
        erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.", standardError, 0) ;
        return ;
      }

      // NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;
      pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if (NULL == pLdvDoc)
        return ;

      NSFollowUpView *pFUView = new NSFollowUpView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pFUView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pFUView->setCreateWindow(pSplittedView) ;

      pFUView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "FollowUp Management", pFUView) ;
    }
  }
  // Procédures - Processes
  else if (pToDo->getParam1() == "ProcessManagement")
  {
    NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
    if (!pPatEnCours)
    {
      erreur("Imposible d'ouvrir la fenetre demandée ! \nIl n'y a pas de dossier patient actif.\nVeuillez ouvrir un patient.  ", standardError, 0) ;
      return ;
    }

    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;
    if (NULL == pSplittedView)
    {
      NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;
      if (!pPatEnCours)
      {
        erreur("Il n'y a pas de patient actif!", standardError, 0) ;
        return ;
      }
      pPatEnCours->process_show(pToDo->getParam2()) ;
    }
    else
    {
      pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
      if (NULL == pLdvDoc)
        return ;

      NSProcessView *pProcessView = new NSProcessView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pProcessView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pProcessView->setCreateWindow(pSplittedView) ;

      pProcessView->setSplittedWindow(pSplittedView) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pLdvDoc, "Process Management", pProcessView) ;
    }
  }
  // Liste des résultats de consultation
  else if (pToDo->getParam1() == "EpiRCManagement")
  {
    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
    if (NULL == pLdvDoc)
      return ;

    NSDocViewManager dvManager(pContexte) ;

    if (NULL == pSplittedView)
      dvManager.createView(pLdvDoc, "RC view Format") ;
    else
    {
      NSEpisodRCView   *pRCView = new NSEpisodRCView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pRCView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pRCView->setCreateWindow(pSplittedView) ;
      pRCView->setSplittedWindow(pSplittedView) ;

      dvManager.createView(pLdvDoc, "RC view Format", pRCView) ;
    }
  }
  // Historique des résultats de consultation
  else if (pToDo->getParam1() == "RcItemHistory")
  {
    NSLdvDocument *pLdvDoc ;
    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
    if (NULL == pLdvDoc)
      return ;

    NSDocViewManager dvManager(pContexte) ;

    if (NULL == pSplittedView)
      dvManager.createView(pLdvDoc, "RC histo Format") ;
    else
    {
      NSRCHistoryView   *pHistoryView = new NSRCHistoryView(*pLdvDoc, pSplittedView->getConcern()) ;

      if (pSplittedView->getCreateWindow())
        pHistoryView->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pHistoryView->setCreateWindow(pSplittedView) ;

      pHistoryView->setSplittedWindow(pSplittedView) ;
      dvManager.createView(pLdvDoc, "RC histo Format", pHistoryView) ;
    }
  }
  // Fiche administrative -
  else if (pToDo->getParam1() == "AdminData")
  {
    NSMUEView *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    if (NULL == pSplittedView)
    {
      bool bOK = true ;

      NSPatientChoisi *pPat = pContexte->getPatient() ;

      NSDocumentInfo  DocumentInfo(pContexte) ;
      string sCodeDocRoot = pContexte->getPatient()->getszNss() + string("_00000") ;
      string sCodeDocAdmin, sCodeDocAdminData ;

      NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
      VecteurString aVecteurString ;
      pGraphe->TousLesVrais(sCodeDocRoot, NSRootLink::personAdminData, &aVecteurString) ;

      if (false == aVecteurString.empty())
      {
        sCodeDocAdmin = *(*(aVecteurString.begin())) ;
        aVecteurString.vider() ;
        pGraphe->TousLesVrais(sCodeDocAdmin, NSRootLink::docData, &aVecteurString) ;

        if (false == aVecteurString.empty())
        {
          sCodeDocAdminData = *(*(aVecteurString.begin())) ;
          string sCodePat = string(sCodeDocAdminData, 0,            PAT_NSS_LEN) ;
          string sCodeDoc = string(sCodeDocAdminData, PAT_NSS_LEN,  DOC_CODE_DOCUM_LEN) ;

          DocumentInfo.setPatient(sCodePat) ;
          DocumentInfo.setDocument(sCodeDoc) ;
          DocumentInfo.setType("CQ030") ;
        }
        else
        {
          string ps = string("Impossible de retrouver les données de la fiche administrative.") ;
          erreur(ps.c_str(), standardError, 0) ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
          bOK = false ;
        }
      }
      else
      {
        string ps = string("Impossible de retrouver le document fiche administrative du patient.") ;
        erreur(ps.c_str(), standardError, 0) ;
        pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
        bOK = false ;
      }

      if (bOK)
      {
        NSPatPathoArray PatPathoAdmin(pContexte->getSuperviseur()) ;
        pPat->getDocHis()->DonnePatPathoDocument("ZADMI1", &PatPathoAdmin) ;
        pPat->ChargeDonneesPatient(&PatPathoAdmin) ;
        pPat->initCorrespArray(&PatPathoAdmin) ;
        // pPat->PostInitialisation() ;
      }
      // On doit lancer postinitialisation AVANT d'ouvrir la fiche administrative
      // (fabrication du nom long, calcul de l'age du patient, etc.)
      pPat->getDocHis()->AutoriserEdition(&DocumentInfo) ;
    }
  }
  // Document treeWindow - treeWindow document
  else if ((strlen(pToDo->getParam1().c_str()) == BASE_LEXI_LEN + 3) && (string(pToDo->getParam1(), 0, 3) == "Cs:"))
  {
    string sRootLexique = string(pToDo->getParam1(), 3, BASE_LEXI_LEN) ;

    sDocumentType = "LdvDoc" ;
    NSMUEView     *pSplittedView = (NSMUEView *)(pToDo->getPointer1()) ;

    if (!pSplittedView)
    {
      NSCSDocument  *pNewDocCS = new NSCSDocument(0, pContexte, sRootLexique) ;

      NSDocViewManager dvManager(pContexte) ;
      dvManager.createView(pNewDocCS, "CS Format") ;
    }
    else
    {
      /*
      NSPaneSplitter  *pPaneSpl = pSplittedView->getPaneSplitter() ;

      NSCSTemplate    *pCSTemplate = new NSCSTemplate("CS", "", 0, "CS", dtAutoDelete) ;
      NSCSDocument    *pNewDocCS = new NSCSDocument(0, appContext, sRootLexique) ;
      pNewDocCS->SetTemplate(pCSTemplate) ;
      NSCsVue         *pCsVue = new NSCsVue(*pNewDocCS, 0) ;

      if (pSplittedView->getCreateWindow())
        pCsVue->setCreateWindow(pSplittedView->getCreateWindow()) ;
      else
        pCsVue->setCreateWindow(pSplittedView) ;

      pCsVue->setSplittedWindow(pSplittedView) ;

      pCSTemplate->InitView(pCsVue) ;
      */
    }
  }}voidNSMDIFrame::ToDo_ExportDocForDeletedCompt(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;  if ((string("") == pToDo->getParam1()) || (string("") == pToDo->getParam2()))    return ;  if (CPTA_NUMCOMPT_LEN != strlen(pToDo->getParam1().c_str()))    return ;  string sNumCompt = pToDo->getParam1() ;  if (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN != strlen(pToDo->getParam2().c_str()))    return ;  string sPatientId  = string(pToDo->getParam2(), 0, PAT_NSS_LEN) ;  string sDocumentId = string(pToDo->getParam2(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if (NULL == pPatEnCours)    return ;  if (pPatEnCours->getNss() != sPatientId)    return ;  NSHISTODocument* pDocHis = pPatEnCours->getDocHis() ;  if (NULL == pDocHis)    return ;  NSDocHistoArray* pVectDocument = pDocHis->getVectDocument() ;	if ((NULL == pVectDocument) || pVectDocument->empty())
  	return ;  DocumentIter iterDoc = pVectDocument->TrouveDocHisto(sPatientId + sDocumentId) ;	if (NULL == iterDoc)    return ;
  if (pDocHis->EstUnDocumentOuvert(*iterDoc))
    return ;

  if (string("ZCN00") != (*iterDoc)->getTypeSem())
    return ;

  NSDocumentInfo *pDocum = new NSDocumentInfo(pContexte) ;
  (*iterDoc)->initDocumentInfo(pDocum) ;
  NSDocumentInfo *pDocHtml  = 0 ;

  NSCRDocument *pReportDoc = new NSCRDocument(0, pDocum, pDocHtml, pContexte, "", true /* read only */) ;
  int iReussi = pReportDoc->decodeCR(&(pReportDoc->_aCRPhrases)) ;

  NSDocStatus docStatus ;
  docStatus.setStatus(NSDocStatus::StatusUnchanged) ;
  docStatus.setCptaStatus(NSDocStatus::StatusDeleted) ;
  docStatus.setDocId(sPatientId + sDocumentId) ;
  docStatus.setNumCompt(sNumCompt) ;

  pReportDoc->exportReport(&docStatus) ;

  delete pReportDoc ;
}
voidNSMDIFrame::ToDo_GetPersonAsynch(NSToDoTask* pToDo){  if (NULL == pToDo)    return ;  if ((string("") == pToDo->getParam1()) || (string("") == pToDo->getParam2()))    return ;  PIDSTYPE iTypePids = pidsPatient ;  if      (string("pidsPatient")      == pToDo->getParam2())    iTypePids = pidsPatient ;  else if (string("pidsCorresp")      == pToDo->getParam2())    iTypePids = pidsCorresp ;  else if (string("pidsUtilisat")     == pToDo->getParam2())    iTypePids = pidsUtilisat ;  else if (string("pidsPatientGroup") == pToDo->getParam2())    iTypePids = pidsPatientGroup ;  else if (string("pidsAll")          == pToDo->getParam2())    iTypePids = pidsAll ;  NSPilotProxyWindow* pPilotProxy = pContexte->getSuperviseur()->getPilotProxyWindow() ;  if (NULL == pPilotProxy)    return ;  pPilotProxy->getPerson(pToDo->getParam1(), iTypePids) ;}voidNSMDIFrame::ToDo_OpenDocument(NSToDoTask* pToDo){  if ((NULL == pToDo) || (string("") == pToDo->getParam1()))    return ;  NSPatientChoisi *pPatEnCours = pContexte->getPatient() ;  if (NULL == pPatEnCours)    return ;  NSHISTODocument* pDocManager = pPatEnCours->getDocHis() ;  if (NULL == pDocManager)    return ;  DocumentIter docIter = pDocManager->TrouveDocHisto(pToDo->getParam1()) ;  if (NULL == docIter)    return ;  pDocManager->AutoriserOuverture((NSDocumentInfo*) *docIter) ;}void
TMyApp::initMenus()
{
	setMenu("menubar", &hAccelerator) ;
}

void
TMyApp::initMainMenu()
{
	// TMenu* menu = new TMenu(*GetApplication()->GetMainWindow()) ;
  // pMainMenu = new TMenu(menu->GetSubMenu(1));

/*
	pMainMenu = new OWL::TMenu ;
  pMainMenu->InsertMenu(0, MF_BYPOSITION, CM_NEWCR, "Nouveau compte-&rendu") ;
*/
}

DEFINE_RESPONSE_TABLE1(NSMDIClient, TMDIClient)
END_RESPONSE_TABLE;


NSMDIClient::NSMDIClient(TModule* module)
            :OWL::TMDIClient(module)
{
}

NSMDIClient::~NSMDIClient()
{
}

static bool
sCannotCloseButServices(TWindow* win, void*)
{
	NSWebServiceChild *wsChild = TYPESAFE_DOWNCAST(win, NSWebServiceChild);
  if (wsChild)
  	return false ;

	return !win->CanClose() ;
}

static void
sCloseChildButServices(TWindow* win, void*)
{
	NSWebServiceChild *wsChild = TYPESAFE_DOWNCAST(win, NSWebServiceChild) ;
  if (wsChild)
  	return ;

	NSMailServiceChild *mailChild = TYPESAFE_DOWNCAST(win, NSMailServiceChild) ;
  if (mailChild)
  	return ;

  OB1InterfaceContainer *BB1Child = TYPESAFE_DOWNCAST(win, OB1InterfaceContainer) ;
  if (BB1Child)
  	return ;

  NSPilotProxyChild *pilotProxyChild = TYPESAFE_DOWNCAST(win, NSPilotProxyChild) ;
  if (pilotProxyChild)
  	return ;

	win->Destroy() ;
  delete win ;
}

bool
NSMDIClient::CloseChildrenButServicesWindows(NSContexte *pContexte)
{

	if (!FirstThat(sCannotCloseButServices))
  {
    ForEach(sCloseChildButServices) ;

    if (pContexte->getBBinterface())
    {
      if (pContexte->getBBinterface()->getContainer())
        pContexte->getBBinterface()->getContainer()->Destroy() ;
      pContexte->setBBinterface((BB1BBInterface *) 0) ;
    }

    return true ;
  }
  return false ;
}

/*
NSPrinter::NSPrinter(OWL::TApplication* app)
{
  Data = new OWL::TPrintDialog::TData ;
  Error = 0 ;
  Application = app ? app : ::GetApplicationObject() ;
  CHECK(Application) ;
  GetDefaultPrinter() ;
}


NSPrinter::~NSPrinter()
{
  delete Data ;
  Data = 0 ;
}


#define SETCLEAR(flag, set, clear)((flag) = ((flag)&~(clear))|(set))
#define WM_SETNUMBER    WM_USER+100

// -----------------------------------------------------------------------------
// Abort procedure used during printing, called by windows. Returns true to
// continue the print job, false to cancel.
// -----------------------------------------------------------------------------
int CALLBACK __export
TPrinterAbortProc(HDC hDC, int code)
{
  GetApplicationObject()->PumpWaitingMessages() ;

  // UserAbortDC will have been set by the AbortDialog
  if (OWL::TPrinter::GetUserAbort() == hDC || OWL::TPrinter::GetUserAbort() == HDC(-1))
  {
    OWL::TPrinter::SetUserAbort(0) ;
    return false ;
  }
  return (code == 0 || code == SP_OUTOFDISK) ;
}


bool
NSPrinter::Print(TWindow* parent, TPrintout& printout, bool prompt)
{
  PRECONDITION(parent) ;

  // Start from scratch
  Error = 1 ;           // Positive 'Error' means print job is OK
  TPrintDC*   prnDC ;
  HCURSOR hOrigCursor = ::SetCursor(::LoadCursor(0, IDC_WAIT)) ;

  // Get page range & selection range (if any) from document
  int selFromPage ;
  int selToPage ;
  printout.GetDialogInfo(Data->MinPage, Data->MaxPage, selFromPage, selToPage) ;
  if (selFromPage)
    Data->Flags &= ~PD_NOSELECTION ;
  else
    Data->Flags |= PD_NOSELECTION ;
  if (Data->MinPage)
  {
    Data->Flags &= ~PD_NOPAGENUMS ;
    if (Data->FromPage < Data->MinPage)
      Data->FromPage = Data->MinPage ;
    else if (Data->FromPage > Data->MaxPage)
		  Data->FromPage = Data->MaxPage ;
    if (Data->ToPage < Data->MinPage)
      Data->ToPage = Data->MinPage ;
    else if (Data->ToPage > Data->MaxPage)
      Data->ToPage = Data->MaxPage ;
  }
  else
    Data->Flags |= PD_NOPAGENUMS ;

  // Create & execute a TPrintDialog (or derived) and have it return a usable
  // DC if prompt is enabled. If the dialog fails because the default printer
  // changed, clear our device information & try again.
  if (prompt)
  {
    SETCLEAR(Data->Flags, PD_RETURNDC, PD_RETURNDEFAULT|PD_PRINTSETUP) ;
    bool ok = ExecPrintDialog(parent) ;
    if (!ok && Data->Error == PDERR_DEFAULTDIFFERENT)
    {
		  ClearDevice() ;
      ok = ExecPrintDialog(parent) ;
	 }
    if (!ok)
      return false ;

    prnDC = Data->TransferDC() ;   // We now own the DC, let prnDC manage it
  }
  // Construct the DC directly if prompting was not enabled.
  //
  else
  {
    prnDC = new TPrintDC(Data->GetDriverName(), Data->GetDeviceName(), Data->GetOutputName(), Data->GetDevMode()) ;
  }
  if (!prnDC)
    throw(TXPrinter()) ;

  // Get the page size
  PageSize.cx = prnDC->GetDeviceCaps(HORZRES) ;
  PageSize.cy = prnDC->GetDeviceCaps(VERTRES) ;
  printout.SetPrintParams(prnDC, PageSize) ;

  // Create modeless abort dialog using strings
  TWindow* abortWin ;
  try
  {
    abortWin = CreateAbortWindow(parent, printout) ;
  }
  catch (...)
  {
    delete prnDC ;
    prnDC = 0 ;
    // RETHROW ;
  }

  ::SetCursor(hOrigCursor) ;
  parent->EnableWindow(false) ;

  prnDC->SetAbortProc(TPrinterAbortProc) ;

  // Only band if the user requests banding and the printer supports banding
  bool banding = printout.WantBanding() && (prnDC->GetDeviceCaps(RASTERCAPS) & RC_BANDING) ;
  if (banding)
  {
    // Only use BANDINFO if supported.
    UseBandInfo = ToBool(prnDC->QueryEscSupport(BANDINFO)) ;
  }
  else
  {
    // Set the banding rectangle to full page
    BandRect.Set(0, 0, PageSize.cx, PageSize.cy) ;
  }

  // If more than one (uncollated) copy was requested, see if printer can
  // handle it for performance and user convenience.
  int copiesPerPass = 1 ;
  if (!(Data->Flags & PD_COLLATE))
    prnDC->SetCopyCount(Data->Copies, copiesPerPass) ;

  // Figure out which page range to use: Selection, Dialog's from/to,
  // whole doc range or all possible pages.
  int fromPage ;
  int toPage ;
  if (prompt && (Data->Flags & PD_SELECTION) || selFromPage)
  {
    fromPage = selFromPage ;
    toPage = selToPage ;
  }
  else if (prompt && (Data->Flags & PD_PAGENUMS))
  {
    fromPage = Data->FromPage ;
    toPage = Data->ToPage ;
  }
  else if (Data->MinPage)
  {
    fromPage = Data->MinPage ;
    toPage = Data->MaxPage ;
  }
  else
  {
    fromPage = 1 ;
	  toPage = INT_MAX ;
  }

  // Copies loop, one pass per block of document copies.
  printout.BeginPrinting() ;
  for (int copies = Data->Copies ; copies > 0 && Error > 0 ; copies -= copiesPerPass)
  {
    // On last multi-copy pass, may need to adjust copy count
    if (copiesPerPass > 1 && copies < copiesPerPass)
      prnDC->SetCopyCount(copies, copiesPerPass) ;

    // Whole document loop, one pass per page
    Flags = pfBoth ;
    Error = prnDC->StartDoc(printout.GetTitle(), 0) ;  // get PD_PRINTTOFILE ?
	  printout.BeginDocument(fromPage, toPage, Flags) ;

    for (int pageNum = fromPage ; Error > 0 && pageNum <= toPage && printout.HasPage(pageNum) ; pageNum++)
    {
		  abortWin->SendDlgItemMessage(ID_PAGE, WM_SETNUMBER, pageNum) ;

      // Begin the page by getting the first band or calling StartPage()
      if (banding)
      {
        FirstBand = true ;
        Error = prnDC->NextBand(BandRect) ;
      }
      else
        Error = prnDC->StartPage() ;

      // Whole page loop, one pass per band (once when not banding)
      while (Error > 0 && !BandRect.IsEmpty())
      {
        // [Manually call the abort proc between bands or pages]
#if defined(MANUAL_ABORT_CALL)
        prnDC->QueryAbort() ;
#endif // MANUAL_ABORT_CALL
        if (banding)
        {
          CalcBandingFlags(*prnDC) ;
          if (printout.WantForceAllBands() && (Flags & pfBoth) == pfGraphics)
            prnDC->SetPixel(NS_CLASSLIB::TPoint(0, 0), 0) ;  // Some old drivers need this
          prnDC->DPtoLP(BandRect, 2) ;
        }

		    printout.PrintPage(pageNum, BandRect, Flags) ;
        if (banding)
          Error = prnDC->NextBand(BandRect) ;
        else
          break ;
      }

      // EndPage (NEWFRAME) need only called if not banding
      if (Error > 0 && !banding)
      {
        Error = prnDC->EndPage() ;
        if (Error == 0)    // a zero return here is OK for this call
          Error = 1 ;
      }

    }  // End of Whole Document-loop

    // Tell GDI the document is finished
    if (Error > 0)
#if defined(MANUAL_ABORT_CALL)
      if (banding && (UserAbortDC == *prnDC || UserAbortDC == HDC(-1))
        prnDC->AbortDoc() ;
      else
        prnDC->EndDoc() ;
#else
      prnDC->EndDoc() ;
#endif // !MANUAL_ABORT_CALL

    printout.EndDocument() ;
  } // End of Copy-loop
  printout.EndPrinting() ;

  // Set the printer back to 1 copy
  if (copiesPerPass > 1)
    prnDC->SetCopyCount(1, copiesPerPass) ;

  // Re-enable parent and free allocated resources
  parent->EnableWindow(true) ;
  abortWin->Destroy() ;
  delete abortWin ;
  abortWin =0 ;
  //if (UserAbortDC == *prnDC)  // User tried to abort, but it was too late
  //  UserAbortDC = 0 ;
  delete prnDC ;
  prnDC = 0 ;

  // Report error if not already done so by printmgr
  if (Error & SP_NOTREPORTED)
    ReportError(parent, printout) ;

  return (Error > 0) ;
}
*/



