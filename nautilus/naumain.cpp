// -----------------------------------------------------------------------------
// naumain.cpp
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.54 $
// $Author: pameline $
// $Date: 2017/07/07 21:18:48 $
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

#include "nssavoir\nsBdmDriver.h"
#include "nssavoir\nspathor.h"
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
#include "partage\nsdivfile.h"

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
#include "nscompta\nsCptaRef.h"
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
#include "nsepisod\nsPilotProxy.h"
#include "nautilus\nsAlertSvce.h"
#include "nautilus\nsTankSvce.h"

//#include "nsoutil/nssmedit.h"
#include "nsoutil/nsoutdlg.h"
#include "nsoutil/nsepiout.h"
#include "nsoutil/nsfilgui.h"
#include "nsoutil/nsrechcr.h"
#include "nautilus/nsepisodview.h"
#include "nautilus/nscqvue.h"
#include "nautilus/nsldvvue.h"
#include "nautilus/nsmdiframe.h"
#include "nsldv/nsldvvar.h"
// #include "adopt.h"
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
#include "nautilus\nsCoopChartDV.h"
#include "nsbb\nsWhoIsThere.h"
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
#include "nsldv\nsldvstructs.h"

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
  EV_COMMAND(CM_TANK,                           CmTankBox),
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
  EV_COMMAND(CM_DISPOSABLES,                    CmDisposables),
  EV_COMMAND(CM_LOCAL_ACTS,                     CmLocalActs),
  EV_COMMAND(CM_NGAP,                           CmNgap),
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

	_appContext = pCtx ;
  _Client     = (NSMDIClient*) 0 ;

  _frame      = (NSMDIFrame*)       0 ;
  _sb         = (OWL::TStatusBar*)  0 ;
  _cb         = (OWL::TControlBar*) 0 ;

  _Harbor     = (THarborEx*)             0 ;
  _cb1        = (TDockableControlBarEx*) 0 ;
  _cb2        = (TDockableControlBarEx*) 0 ;
  _cb3        = (TDockableControlBarEx*) 0 ;

  _hAccelerator = 0 ;

	// Get current directory as soon as possible
  initCurrentDir() ;

	// Splash window

  _pSplashWindow = (TSplashWindow*) 0 ;
  _pSplashDib    = (TDib*)          0 ;

  // _pFormsApplication = (Forms::TApplication*) 0 ;

	// Object initialisation

  _sCurrentMenuName = string("") ;

  iCaptureHotKey = 0 ;
  iBabylonHotKey = 0 ;

	_appContext->getSuperviseur()->setApplication(this) ;

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

  if (_appContext)
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

  if (_hAccelerator)
  {
    DestroyAcceleratorTable(_hAccelerator) ;
    _hAccelerator = 0 ;
  }
}

void
TMyApp::InitMainWindow()
{
	sendMessageToSplash("Init main window") ;
  setSplashPercentDone(95) ;

  NSSuper* pSuper = _appContext->getSuperviseur() ;

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
  if (_appContext->getPredi() && (string("") != _appContext->getPredi()->_sNomModule))
  {
    sNomModule = _appContext->getPredi()->_sNomModule ;
    if (string("") != _appContext->getPredi()->_sOEMModule)
      sNomModule += string("/") + _appContext->getPredi()->_sOEMModule ;
  }
  sNomModule += string(" (") + pSuper->getInstanceString() + string(")") ;

	_frame = new NSMDIFrame(_appContext, sNomModule.c_str(), 0, *(_Client = new NSMDIClient(this)), true, this) ;
	// frame->SetOcApp(OcApp) ;

  // frame->Attr.AccelTable = IDA_MDICMNDS ;

  _frame->SetIcon(this, EPI_ICON1) ;
  _frame->SetIconSm(this, EPI_ICON1) ;

  // pour les barres dockables
  _Harbor = new THarborEx(*_frame) ;

	// Construction de la barre de statut
	_sb = new OWL::TStatusBar(_frame, TGadget::Recessed) ;

	// Construction de la barre de contrôle
	_cb1 = new TDockableControlBarEx(_frame) ;
  _cb1->SetCaption("Primary control bar") ;
  _cb1->SetDefault(true) ;

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
	_cb1->SetHintMode(TGadgetWindow::EnterHints) ;

  _cb2 = new TDockableControlBarEx(ID_CB2, "Secondary control bar", false, _frame) ;

  // cb3 = new TDockableControlBarEx(ID_CB3, "Episodus", false, frame) ;
  // cb3->Insert(*new TButtonGadget(CM_EPISO_START, CM_EPISO_START, TButtonGadget::Command)) ;

  _Harbor->Insert(*_cb1, OWL::alTop) ;
  _Harbor->Insert(*_cb2, OWL::alTop) ;
  //Harbor->Insert(*cb3, OWL::alTop) ;

  // Insertion des barres (statut et contrôle)
  _frame->Insert(*_sb, TDecoratedFrame::Bottom) ;

	// Définition de la fenêtre principale et de son menu
	SetMainWindow(_frame) ;
  _appContext->getSuperviseur()->pExeModule = GetMainWindow()->GetModule() ;

  // Initialization of all menus
  //
  initMenus() ;

  GetMainWindow()->Attr.Style |= WS_MAXIMIZE ;

	// Mise en place du gestionnaire de documents
	// SetDocManager(new TDocManager(dmMDI | dmMenu | dmNoRevert, this));
  SetDocManager(new TDocManager(dmMDI)) ;

	_appContext->getSuperviseur()->setMainWindow(GetMainWindow()) ;

  sendMessageToSplash("Go") ;
  setSplashPercentDone(100) ;

  if (_pSplashWindow)
  {
    delete _pSplashWindow ;
    _pSplashWindow = (TSplashWindow*) 0 ;
  }

  if (_pSplashDib)
  {
    delete _pSplashDib ;
    _pSplashDib = (TDib*) 0 ;
  }

  char szMain[30] ;
  sprintf(szMain, "%p", GetMainWindow()->HWindow) ;
  ps = string("TMyapp Init main window: Leaving (Main Window: ") + string(szMain) + string(")") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;
}

// Called when a window gains focus and sets its own menu
//
voidTMyApp::setMenu(string sMenuName, HACCEL far *hAccelerator){  if ((string("") != sMenuName) && (sMenuName == _sCurrentMenuName))    return ;	string sNewMenuName = string("") ;	if (string("") == sMenuName)		sNewMenuName = _sCurrentMenuName ;  else  	sNewMenuName = sMenuName ;  if (string("") == sNewMenuName)		return ;	nsMenuIniter menuIter(_appContext) ;	OWL::TMenuDescr menuBar ;
  menuIter.initMenuDescr(&menuBar, sNewMenuName) ;
  GetMainWindow()->SetMenuDescr(menuBar) ;

  *hAccelerator = menuIter.getAccelerator() ;

  _sCurrentMenuName = sNewMenuName ;

  // Make certain "Windows" menu is properly defined
  //
  NSMDIClient *pMDIClient = _frame->GetMdiClientWindow() ;
  pMDIClient->ClientAttr->hWindowMenu = _frame->FindChildMenu(_frame->GetMenu());
}voidTMyApp::createSplash()
{
	if (_pSplashWindow)
		return ;

	// Splash window

	int SplashStyle = TSplashWindow::MakeStatic | TSplashWindow::MakeGauge |
                                                 TSplashWindow::CaptureMouse ;
	int timeOut = 0 ;

  _pSplashDib    = new TDib("splash.bmp") ;
	_pSplashWindow = new TSplashWindow(*_pSplashDib, _pSplashDib->Width(),
                                  _pSplashDib->Height() + 100, SplashStyle, timeOut) ;

	_pSplashWindow->Create() ;       // create the splash window
	_pSplashWindow->UpdateWindow() ; // update the display to show the splash screen
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
    if (_pSplashWindow)
    	hWnd = _pSplashWindow->GetHandle() ;

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
    if (_pSplashWindow)
    	hWnd = _pSplashWindow->GetHandle() ;
		erreur("Echec de la fonction CreateMutex.", standardError, 0, hWnd) ;
		_appContext->getSuperviseur()->setInstance(0) ;
		bExit = true ;
	}
	else
	{
		if (GetLastError() != ERROR_ALREADY_EXISTS)
		{
			// on est dans la première instance
			if (SetFirstInstance())
				_appContext->getSuperviseur()->setInstance(1) ;
			else
			{
				_appContext->getSuperviseur()->setInstance(0) ;
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
      	_appContext->getSuperviseur()->setInstance(iInstance) ;
				retVal = ::MessageBox(0, "Episodus : Une session antérieure existe déjà. Voulez-vous continuer ?", "Message Nautilus", MB_YESNO) ;
				if (IDNO == retVal)
					bExit = true ;
			}
			else
			{
				_appContext->getSuperviseur()->setInstance(0) ;
				bExit = true ;
			}
		}
	}

	if (bExit) // pour sortir dans MessageLoop
		_appContext->setUtilisateur(0) ;
	else
	{
		// Ctl3d DLL
		EnableCtl3d(true) ;
		// BWCC DLL
		EnableBWCC(true, 0xc) ;

		GetMainWindow()->DragAcceptFiles(true) ;

		if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		{
			_appContext->getSuperviseur()->setInitInstance(true) ;
			_appContext->getSuperviseur()->ReferenceTousArchetypes() ;
      _appContext->getSuperviseur()->ReferenceNewFilsGuides() ;
			_appContext->getSuperviseur()->AppelUtilisateur() ;
		}

		NS_CLASSLIB::TRect rect ;
		_cb1->GetRect(rect) ;
		_Harbor->Move(*_cb2, OWL::alTop, &NS_CLASSLIB::TPoint(rect.left + rect.Width(), rect.top)) ;
		_cb2->GetRect(rect) ;
		// Harbor->Move(*cb3, OWL::alTop, &NS_CLASSLIB::TPoint(rect.left + rect.Width(), rect.top));

		// Initialisation du lexique et du réseau sémantique
		_appContext->getDico()->Prend() ;
		_appContext->getFilGuide()->Prend() ;
		_appContext->getFilDecode()->Prend() ;
	}
}

bool
TMyApp::CanClosePatient()
{
  if ((NSContexte*) NULL == _appContext)
    return true ;

  if ((NSPatientChoisi*) NULL == _appContext->getPatient())
    return true ;

  if ((NSUtilisateurChoisi*) NULL == _appContext->getUtilisateur())
    return false ;

  string sMsg = string("Application is closing the patient") ;
  _appContext->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

  return _appContext->getUtilisateur()->fermePatEnCours() ;
}

bool
TMyApp::CanClose()
{
	if (GetMainWindow())
	{
		// on doit positionner bCanCloseHisto si un patient est ouvert
		// sinon l'historique empêche le CloseChildren et CanClose renvoie false
    NSPatientChoisi*     pOpenedPatient = _appContext->getPatient() ;
    NSUtilisateurChoisi* pPreviousUser  = _appContext->getUtilisateur() ;
    if (pPreviousUser && pOpenedPatient)
    {
      if (false == pPreviousUser->CanClosePatient())
        return false ;

      NSToDoTask *pToDo = new NSToDoTask ;
      pToDo->setWhatToDo(string("CloseMainWindow")) ;

      pPreviousUser->setToDoAfterPatClose(pToDo) ;
      pPreviousUser->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnExecToDo) ;

      return false ;
    }

/*
		if (_appContext->getPatient())
			if (false == appContext->getUtilisateur()->fermePatEnCours())
				return false ;
*/

		//pContexte->getPatient()->bCanCloseHisto = true;

    // If user object still exists, it's time to delete it
    // Détruit l'utilisateur s'il existe encore.
    //
    NSUtilisateurChoisi* pUser = _appContext->getUtilisateur() ;
	  if (pUser)
    {
#ifndef __EPIPUMP__
# ifndef __EPIBRAIN__
      NSEpisodus* pEpisodus = _appContext->getEpisodus() ;
      if (pEpisodus && pEpisodus->bAutoSave)
      {
        // NSPrometheFile PrometCfg(pContexte) ;
        // PrometCfg.EcrireFichier(pUser->getID()) ;
        pEpisodus->saveParams(pUser->getID()) ;
      }
# endif // !__EPIBRAIN__
#endif // !__EPIPUMP__

      string sMsg = string("Deleting the user") ;
      _appContext->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

      delete pUser ;
      _appContext->setUtilisateur((NSUtilisateurChoisi*) 0) ;
	  }

    string sMsg = string("Application is closing all child windows") ;
    _appContext->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSteps) ;

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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
	{
    if (0 != GetMainWindow())
		  GetMainWindow()->CloseWindow() ;
		return 0 ;
	}

	// on détruit les fichier tmp de perso (fichiers CR)
	// if (_appContext->getSuperviseur()->getInstance() == 1)
	//	DetruireFichiersTmp() ;

	return (OWL::TApplication::MessageLoop()) ;
}

void
TMyApp::DetruireFichiersTmp()
{
  WIN32_FIND_DATA FileData ;
  char            szMask[255] ;

  string          sFile ;

  string sPathName = _appContext->PathName("FPER") ;
  strcpy(szMask, sPathName.c_str()) ;
  strcat(szMask, "*.*") ;

  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (INVALID_HANDLE_VALUE == hSearch)
    return ;

  bool bFinish = false ;
  while (false == bFinish)
  {
    string sFileName = string("") ;
    size_t pos       = 0 ;

    DWORD dwAttr = FileData.dwFileAttributes ;

    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {
      sFileName = string(FileData.cFileName) ;
      // Récupération de l'extension
      pos = sFileName.find(".") ;
    }

    if ((!(dwAttr & FILE_ATTRIBUTE_DIRECTORY)) && (string::npos != pos))
    {
      string sExtension = string(sFileName, pos + 1, strlen(sFileName.c_str()) - pos - 1) ;
      char extension[255] ;
      strcpy(extension, sExtension.c_str()) ;
      sExtension = string(strlwr(extension)) ;

      if (sExtension == "tmp")
        NsDeleteTemporaryFile(_appContext->getSuperviseur(), sFile) ;
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
  if (iCaptureHotKey == idHotKey)
  {
    NSEpisodus* pEpisod = _appContext->getEpisodus() ;
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
  else if (iBabylonHotKey == idHotKey)
  {
    NSEpisodus* pEpisod = _appContext->getEpisodus() ;
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
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) || (_appContext->getPatient() == 0))
		return ;

  string ps = string("Files dropped") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	int fileCount = dropInfo.DragQueryFileCount() ;
	for (int index = 0 ; index < fileCount ; index++)
	{
		int fileLength = dropInfo.DragQueryFileNameLen(index) + 1 ;
		char* filePath = new char[fileLength] ;
		dropInfo.DragQueryFile(index, filePath, fileLength) ;

    ps = string("Handling file drop for file ") + string(filePath) ;
    _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

		_appContext->getPatient()->Importer(filePath) ;

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
	// char nomVue[80] ;
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

	bool bSplitted = false ;

	NSMDIChild*         child         = (NSMDIChild*)         0 ;
  NSCQWindowsView*    pCQVue        = (NSCQWindowsView*)    0 ;
  NSDecisionTreeView* pGuidelineVue = (NSDecisionTreeView*) 0 ;

	// NSMUEVIEW : fenêtre contenue dans un NSPaneSplitter
	// NSMUEVIEW : windows made to be part of a NSPaneSplitter
	NSMUEView* pMUEView = TYPESAFE_DOWNCAST(&view, NSMUEView) ;
	if (pMUEView)
	{
    // Première fenêtre à s'ouvrir : il faut créer le NSPaneSplitter
    // First window of a group : it means we have to create the NSPaneSplitter
    if (pMUEView->getCreateWindow() == NULL)
    {
      NSMDIChildContent* pChildContent = new NSMDIChildContent(_appContext, _Client) ;

      NSPaneSplitter* pPanelSplitter = new NSPaneSplitter(_appContext, pChildContent) ;
      // si le style de la fenetre ne contient pas les butoons OK, Cancel
      // on n'affiche pas la fenetre gadget avec les buttons
    	NSGadgetWindow*  pGadgetWnd = new NSGadgetWindow(_appContext, pChildContent, pMUEView->uButtonsStyle) ;
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

      // child = new NSMDIChild(_appContext, *Client, 0, pPanelSplitter) ;
      child = new NSMDIChild(_appContext, *_Client, 0, pChildContent) ;
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
      TWindow *winClient = _Client->FirstThat(MyConditionChild,(void *)pMUEView->getCreateWindow()) ;
      if (winClient)
      {
        NSMDIChild* child = TYPESAFE_DOWNCAST(winClient, NSMDIChild) ;
        if (child)
        {
          NSMUEView* wndMother = TYPESAFE_DOWNCAST(pMUEView->getSplittedWindow(), NSMUEView) ;
          if (wndMother)
          {
            //pContexte->getSuperviseur()->pNSApplication->prendClient()
            NSSplittingWindowProperty *pSplitProp = _appContext->getUtilisateur()->searchWindowProperty(wndMother->getFunction(), pMUEView->getFunction()) ;
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

  if ((NSMDIChild*) NULL == child)
    child = new NSMDIChild(_appContext, *_Client, 0, view.GetWindow(), false) ;

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
  // strcpy(nomVue, view.GetViewName()) ;

  NSSuper *pSuper = _appContext->getSuperviseur() ;

  string ps = string("EvNewView, before child->create for ") + string(view.GetViewName()) ;
  pSuper->trace(&ps, 1) ;

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

  char szChild[30] ;
  sprintf(szChild, "%p", child->HWindow) ;
  ps = string("EvNewView: NSMDIChild created (") + string(szChild) + string(").") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  // Si la NSMDIChild n'a pas de NSPaneSplitter, on a fini
  // Si the NSMDIChild has no NSPaneSplitter, we are done
  //
  if ((TWindow*) NULL == child->getCreateWindow())
    return ;
  if ((NSPaneSplitter*) NULL == pMUEView->getPaneSplitter())
    return ;

  NSGadgetWindow* pGadgetWind = pMUEView->getPaneSplitter()->_pGadgetPanelWindow ;
  if ((NSGadgetWindow*) NULL == pGadgetWind)
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
	child->split(child->getCreateWindow(), (TWindow*) 0, psNone, 0.50) ;
	// end modif 21/03/07

  child->getCreateWindow()->SetFocus() ;

  ps = string("EvNewView after child->create") ;
  pSuper->trace(&ps, 1) ;

  if (((NSCQWindowsView*) NULL == pCQVue) && pMUEView)
  {
    TWindow *wndCr = child->getCreateWindow() ;
    NSMUEView* wndCreate = TYPESAFE_DOWNCAST(wndCr, NSMUEView) ;
    NSWindowProperty *pWinProp = _appContext->getUtilisateur()->getWindowProperty(wndCreate->getFunction()) ;
    if (pWinProp)
    {
    	// Warning : if application is iconic, then GetWindowRect returns
      //           fake informations (-3000, -3500...)
    	if (false == _appContext->GetMainWindow()->IsZoomed())
      {
        ps = string("EvNewView, MoveWindow not zoomed with rect ") + pWinProp->getPosAsString() ;
        pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

    		child->MoveWindow(pWinProp->X(pSuper), pWinProp->Y(pSuper), pWinProp->W(pSuper), pWinProp->H(pSuper), true) ;
      }
      else
      {
        ps = string("EvNewView, MoveWindow zoomed with rect ") + pWinProp->getPosAsString() ;
        pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

/*
      	NS_CLASSLIB::TRect clientRect, rectWnd ;
      	Client->GetWindowRect(clientRect) ;
      	rectWnd.left = pWinProp->X(pSuper) - clientRect.left ;
      	rectWnd.top  = pWinProp->Y(pSuper) - clientRect.top ;
      	child->MoveWindow(rectWnd.left , rectWnd.top, pWinProp->W(pSuper), pWinProp->H(pSuper), true) ;
*/

        child->MoveWindow(pWinProp->X(pSuper), pWinProp->Y(pSuper), pWinProp->W(pSuper), pWinProp->H(pSuper), true) ;
      	// string sFunction = pWinProp->sFunction ;
      }
    }
    else
    {
      ps = string("EvNewView, NSWindowProperty not found for function ") + wndCreate->getFunction() ;
      pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
    }
  }

  // cas des vues formulaire : on redimentionne la fenetre
  if (pCQVue)
  {
    ps = string("EvNewView de NSCQVue, début du traitement spécifique") ;
    pSuper->trace(&ps, 1) ;

    // ?? pCQVue->SetParent((TWindow *)child) ;
    bool bMovingCQVue = false ;

    if (pCQVue->getFunction() != string(""))
    {
      // copier/coller nscsvue pour "Synthesis"
      // fixer la nouvelle position
      // (on ne tient pas compte de la taille, vu le probleme pour restaurer
      // une fenetre TView, TWindow mise en icone)
      NSWindowProperty *pWinProp = _appContext->getUtilisateur()->getWindowProperty(pCQVue->getFunction()) ;
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
        pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

        child->MoveWindow(pWinProp->X(pSuper), pWinProp->Y(pSuper), pWinProp->W(pSuper), pWinProp->H(pSuper), true) ;
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
      pSuper->trace(&ps, 1) ;

      child->MoveWindow(0, 0, newWidth, newHeight, true) ;
    }

    if (pCQVue->_pDoc->getDocType() == NSCQDocument::dpio)
    {
      NSPredi* pPredi = _appContext->getPredi() ;
      if (pPredi)
      {
        char far szTitle[256];
        // pCQVue->clientWin->GetWindowText(szTitle, 255) ;
        pCQVue->GetWindow()->GetWindowText(szTitle, 255) ;

        ps = string("EvNewView de NSCQVue, référencement du formulaire ") + string(szTitle) ;
        pSuper->trace(&ps, 1) ;

        pPredi->referenceFormulaire((TWindow*)child, szTitle) ;
      }
    }

    ps = string("EvNewView for NSCQVue, dedicated processing done") ;
    pSuper->trace(&ps, 1) ;
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
    pSuper->trace(&ps, 1) ;

    child->MoveWindow(0, 0, newWidth, newHeight, true) ;

    NSPredi* pPredi = _appContext->getPredi() ;
    if (pPredi)
    {
      string sArchetype = pGuidelineVue->_pDoc->getArchetype() ;

      ps = string("EvNewView de NSDecisionTreeView, referencing decision tree ") + sArchetype ;
      pSuper->trace(&ps, 1) ;

      pPredi->referenceDecisionTree((TWindow*) pGuidelineVue, sArchetype) ;
    }
  }

  // on crée les fenêtres complémentaires cette fenêtre
  // we now create the complementary windows for this window
  if (pMUEView)
  {
    ps = string("EvNewView for NSMUEView, creating complementary windows") ;
    pSuper->trace(&ps, 1) ;

    NSMUEView* pMotherWind ;
    NSPaneSplitter* pPanelSplitter = pMUEView->getPaneSplitter() ;
    if (pPanelSplitter && (pMUEView->getFunction() != string("")))
    {
      VecteurString VectChildFct ;
      pPanelSplitter->researchChildWindFonction(pMUEView->getFunction(), &VectChildFct) ;
      if (false == VectChildFct.empty())
        for (EquiItemIter iterChilds = VectChildFct.begin() ; VectChildFct.end() != iterChilds ; iterChilds++)
        {
          pMotherWind = pPanelSplitter->researchMotherWindow((**iterChilds), pMUEView->getFunction()) ;
          if (pMotherWind)
            pMotherWind->newWindow((**iterChilds)) ;
        }

      // Temporally commented because it seems it can lead to exceptions
      // when called during patient opening
/*
      if (pPanelSplitter->_pCurrentFocusedView)
        pPanelSplitter->_pCurrentFocusedView->focusView() ;
*/
    }

    ps = string("EvNewView for NSMUEView, complementary windows created") ;
    pSuper->trace(&ps, 1) ;
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

void
TMyApp::CmMailBox()
{
	if (_appContext->_pMailBoxChild)
	{
  	SetFocus(_appContext->_pMailBoxChild->GetHandle()) ;
		return ;
	}

  string ps = string("Opening mailbox") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	_appContext->_pMailBoxWindow = new NSMailServiceWindow(_appContext, GetMainWindow()) ;
	_appContext->_pMailBoxChild  = new NSMailServiceChild(_appContext, *(prendClient()), "", _appContext->_pMailBoxWindow) ;
	_appContext->_pMailBoxChild->Create() ;

/*
	TMailBox*	    pMailBox ;
	NSMailParams* pm ;

	if (!appContext->getUtilisateur()->initMail())
	{
		erreur("Impossible d'ouvrir la boite aux lettres Nautilus.", standardError, 0) ;
		return ;
	}

	pm = _appContext->getUtilisateur()->pMail ;
	pMailBox = new TMailBox(NULL, pm->sUrlPOP3, pm->iPortPOP3, pm->sUserPOP3, pm->sPassPOP3, _appContext) ;
	pMailBox->Show() ;
*/
}

void
TMyApp::CmTankBox()
{
	if (_appContext->_pMailBoxChild)
	{
  	SetFocus(_appContext->_pMailBoxChild->GetHandle()) ;
		return ;
	}

  string ps = string("Opening tankbox") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->_pTankBoxWindow = new NSTankServiceWindow(_appContext, GetMainWindow()) ;
	_appContext->_pTankBoxChild  = new NSTankServiceChild(_appContext, *(prendClient()), "", _appContext->_pTankBoxWindow) ;
	_appContext->_pTankBoxChild->Create() ;

  ps = string("Tankbox opened") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}

void
TMyApp::CmOutils()
{
try
{
	string sModuleName = string("") ;

	NsOutilDialog* pNsOutilDialog = new NsOutilDialog(_appContext->GetMainWindow(), _appContext, &sModuleName) ;
	int idRet = pNsOutilDialog->Execute() ;
  delete pNsOutilDialog ;

  if ((IDOK != idRet) || (string("") == sModuleName))
		return ;

  TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;

  if ((TModule*) NULL == pDCModule)
  {
  	string sErrorMsg = string("Cannot open DLL ") +  sModuleName ;
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

  ((*pAdresseFct)((TWindow far *) _appContext->GetMainWindow(), (NSContexte far *) _appContext)) ;

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
  BB1BBInterface* pOB1 = _appContext->getBBinterface() ;
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
	NSGroupGdDialog* pGroupGdDialog = new NSGroupGdDialog(_appContext->GetMainWindow(), _appContext) ;
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
	if (_appContext->getUtilisateur())
    _appContext->getUtilisateur()->CmRequete() ;
}

void
TMyApp::CmResult()
{
	if (_appContext->getUtilisateur())
    _appContext->getUtilisateur()->CmResult() ;
}

void
TMyApp::CmSauvegarde()
{
/************************************************
  // création de l'équipe médicale locale
  NSMoralPerson *pMPTeam = new NSMoralPerson(_appContext, "", "Equipe médicale locale", "") ;
  pMPTeam->createTree() ;
  NSMoralPersonManager *pMPManager = new NSMoralPersonManager(pMPTeam) ;
  delete pMPTeam ;

  NSPersonsAttributesArray *pAttsList = new NSPersonsAttributesArray() ;

  const char *serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
  NSBasicAttributeArray *pAttrArray = new NSBasicAttributeArray() ;
  pAttrArray->push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;
  bool bListOk = _appContext->pPilot->personList(serviceName, pAttsList, pAttrArray) ;
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

  NSTeamGraphManager* pTeamManager = new NSTeamGraphManager(_appContext) ;
  if (pTeamManager->setTeamGraph(pMPManager->getPatho(), _appContext->getUtilisateurID(), true))
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
	ArchetypesListDialog* pListArchDlg = new ArchetypesListDialog(_appContext->GetMainWindow(), _appContext, NSArcManager::referentiel) ;
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
	ArchetypesListDialog* pListArchDlg = new ArchetypesListDialog(_appContext->GetMainWindow(), _appContext, NSArcManager::archetype) ;
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
	// if (_appContext->getUtilisateur() != 0)
	//	appContext->getUtilisateur()->CmEditTemplate() ;

  NSTemplateWindow* pTplWindow = new NSTemplateWindow(_appContext, GetMainWindow()) ;
  TMDIChild*        pMdiChild  = new TMDIChild(*(OWL::TMDIClient*)prendClient(), "", pTplWindow) ;
	pMdiChild->Create() ;
}

void
TMyApp::CmRefArchetype()
{
	if (_appContext->getUtilisateur())
    _appContext->getUtilisateur()->CmRefArchetype() ;
}

void
TMyApp::CmPREDI()
{
	if (((NSUtilisateurChoisi*) NULL == _appContext->getUtilisateur()) ||
      ((NSPatientChoisi*)     NULL == _appContext->getPatient()))
		return ;

  string ps = string("User starts Predi") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (_appContext->getBBinterface())
	  _appContext->getBBinterface()->addNautilusEvent(std::string("GPRED")) ;
}

void
TMyApp::CmProjet()
{
	if (((NSUtilisateurChoisi*) NULL == _appContext->getUtilisateur()) ||
      ((NSPatientChoisi*)     NULL == _appContext->getPatient()))
		return ;

  string ps = string("User starts project management") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (_appContext->getBBinterface())
	  _appContext->getBBinterface()->addNautilusEvent(std::string("GPROJ")) ;
}

void
TMyApp::CmNewQuestionnaire()
{
	if (((NSUtilisateurChoisi*) NULL == _appContext->getUtilisateur()) ||
      ((NSPatientChoisi*)     NULL == _appContext->getPatient()))
		return ;

  _appContext->getPatient()->CmNewArchetype() ;
}

void
TMyApp::CmDeviceInterface()
{
	if (((NSUtilisateurChoisi*) NULL == _appContext->getUtilisateur()) ||
      ((NSPatientChoisi*)     NULL == _appContext->getPatient()))
		return ;

  _appContext->getPatient()->CmDeviceInterface() ;
}

void
TMyApp::CmMateriels()
{
try
{
	ListeMaterielDialog* pListeMatDlg = new ListeMaterielDialog(_appContext->GetMainWindow(), _appContext) ;
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
  NSDateConvocDialog* pDateDlg = new NSDateConvocDialog(_appContext->GetMainWindow(), _appContext) ;
  int idRet = pDateDlg->Execute() ;
  if (IDOK == idRet)
  {
    NSVarConvoc* pVarConvoc = new NSVarConvoc(_appContext->getSuperviseur(), pDateDlg->sDate1, pDateDlg->sDate2) ;
    NSListeConvocDialog* pListeDlg = new NSListeConvocDialog(_appContext->GetMainWindow(), _appContext, pVarConvoc) ;
    pListeDlg->Execute() ;
    if (pListeDlg->bImprimerListe)
    {
      NSVoidArray* pVarArray = new NSVoidArray ;
      pVarArray->push_back((void*)(&(pVarConvoc->_sDate1))) ;
      pVarArray->push_back((void*)(&(pVarConvoc->_sDate2))) ;
      pVarArray->push_back((void*)(&(pVarConvoc->_aPatientArray))) ;
      NSPubliRefDocument* pDocPubli = new NSPubliRefDocument(pVarArray, "ZPCVC", _appContext) ;
      pDocPubli->Publier(false) ;
    }
    else if (pListeDlg->bImprimerLettre)
    {
      int index ;

      // on instancie le document convoc (lettre de reconvocation)
      NSConvocRefDocument* pDocConvoc = new NSConvocRefDocument(pVarConvoc, pListeDlg->pTabSelect, pListeDlg->nbConvoc, _appContext) ;

      // on prépare l'objet de publication du document
      pDocConvoc->setPubli(new NSPublication(0, pDocConvoc, _appContext)) ;

      NSPublication* pPubli = pDocConvoc->getPubli() ;

      // on instancie à la main un tableau de correspondants fictifs
      // qui correspondent à la liste des patients à reconvoquer
      for (int i = 0 ; i < pListeDlg->nbConvoc ; i++)
      {
        if (i < MAXSELECT)
        {
        	NSPersonInfo PatInfo(_appContext->getSuperviseur()) ;

          // on récupère l'indice du patient à reconvoquer
          index = pListeDlg->pTabSelect[i] ;
          PatInfo = *(pVarConvoc->_aPatientArray[index]) ;

          pPubli->_aCorrespBaseArray.push_back(new NSPersonInfo(PatInfo)) ;

          NSChoixPubli *pPatPubli = &(pPubli->_aChoixPubli[i]) ;

          // on remplit également la structure de sélection aChoixPubli
          // sprintf(pDocConvoc->pPubli->aChoixPubli[i].corresp, "%03d", i) ;
          pPatPubli->_sCorresp    = PatInfo.getNss() ;
          pPatPubli->_bSelect     = true ;
          pPatPubli->_bImprimante = true ;
          strcpy(pPatPubli->_nb_expl, "1") ;
          pPatPubli->_iCpt        = 1 ;
          pPatPubli->_sUrl        = string("") ;
          pPatPubli->_bEmail      = false ;
          pPatPubli->_bIntranet   = false ;
          pPatPubli->_bHtml       = false ;
          pPatPubli->_bLettre     = false ;
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

  delete pDateDlg ;
}

void
TMyApp::CmSetReconvocDate()
{
	if (NULL == _appContext->getPatient())
		return ;

	NSLdvDocument *pDocLdv = _appContext->getPatient()->getLdvDocument() ;
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
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	AboutDialog(&TWindow(GetMainWindow()->GetCommandTarget()), _appContext).Execute() ;
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
  if (_appContext->getUtilisateur())
  {
    if (false == _appContext->getUtilisateur()->isAdmin())
    {
      erreur("Cette fonction est accessible uniquement à l'administrateur.", standardError, 0) ;
      return ;
    }

    string ps = string("User creates a new user") ;
    _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	 	if (false == _appContext->getUtilisateur()->Creer(_appContext->GetMainWindow()))
      erreur("Erreur à la création d'un nouvel utilisateur", standardError, 0) ;
  }
}

// -----------------------------------------------------------------------------
// Modification d'un utilisateur
// -----------------------------------------------------------------------------
void
TMyApp::CmModifDoc()
{
  NSUtilisateurChoisi* pCurrentUser = _appContext->getUtilisateur() ;
  if ((NSUtilisateurChoisi*) NULL == pCurrentUser)
    return ;

  if (pCurrentUser->isAdmin())
  {
    if (false == pCurrentUser->ModifierAutreUtilisateur(_appContext->GetMainWindow()))
      erreur("Erreur à la modification d'un nouvel utilisateur", standardError, 0) ;
    return;
  }

  string ps = string("User edits a user") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (false == pCurrentUser->Modifier(_appContext->GetMainWindow()))
    erreur("Erreur à la modification d'un nouvel utilisateur", standardError, 0) ;
}

void
TMyApp::CmNewEnTete()
{
	if (_appContext->getUtilisateur() != 0)
    _appContext->getUtilisateur()->CmNewEnTete() ;
}

void
TMyApp::CmModEnTete()
{
	if (_appContext->getUtilisateur() != 0)
    _appContext->getUtilisateur()->CmModEnTete() ;
}

void
TMyApp::CmCreatCor()
{
#ifndef N_TIERS

  string ps = string("User creates a new refering professionnal") ;
  pSuper->trace(&ps, 1, NSSuper::trSteps) ;

  NSCorrespondant* pCorresp = new NSCorrespondant(_appContext) ;
  pCorresp->lastError = pCorresp->open() ;
  if (pCorresp->lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture du fichier Corresp.db", 0, pCorresp->lastError) ;
    delete pCorresp ;
    return ;
  }

  bool creatOK = pCorresp->Creer(_appContext->GetMainWindow()) ;
  pCorresp->lastError = pCorresp->close() ;
  if (pCorresp->lastError != DBIERR_NONE)
  {
    erreur("Erreur à la fermeture du fichier Corresp.db", 0, pCorresp->lastError) ;
    delete pCorresp ;
    return ;
  }

  delete pCorresp ;
#else
  /* NSPersonInfo *pPersonInfo = */ _appContext->getPersonArray()->createPerson(_appContext, pidsCorresp) ;
#endif // N_TIERS
}

void
TMyApp::CmModifCor()
{
  string ps = string("Editing a refering professionnal") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSTPersonListDialog indep(GetMainWindow(), pidsCorresp, false, _appContext) ;
  if (indep.Execute() == IDOK)
  {
    if (indep.bCreer)
    {
      /* NSPersonInfo	*pPersonInfo = */ _appContext->getPersonArray()->createPerson(_appContext, pidsCorresp) ;
    }
    else
    {
      NSPersonInfo *temp = indep.pPersonSelect ;
      string sPids  = temp->getNss() ;
      string sRoles = temp->getRoles() ;
      _appContext->getPersonArray()->modifyPerson(_appContext, sPids, sRoles, pidsCorresp) ;
    }
  }
}

void
TMyApp::CmHealthTeam()
{
	if (_appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens health team rosace") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->heathTeamRosaceShow() ;

/*
 GraphicHealthTeam* health = new GraphicHealthTeam(GetMainWindow(), _appContext, _appContext->getPatient()->pHealthTeam) ;
 health->Execute() ;
 delete health ;
*/
}

void
TMyApp::CmHealthTeamList()
{
	if (_appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens health team list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->heathTeamListShow() ;
}

void
TMyApp::CmCoopChartBox()
{
	if (_appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens a new Coop chart") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmNewCoopChart() ;
}

void
TMyApp::CmVenuesList()
{
	if (_appContext->getPatient() == 0)
  	return ;

  string ps = string("User opens venue list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->venuesListShow() ;
}

void
TMyApp::CmImportPatientLdv()
{
	// si pas d'utilisateur en cours : on sort
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;
  //if(!pContexte->pPilot->testServiceStatus(NautilusPilot::SERV_PATIENT_IMPORT))
	if (false == _appContext->getSuperviseur()->getPilot()->isOperationalLdVAgent())
  {
   //	appContext->pPilot->detailError(NautilusPilot::SERV_PATIENT_IMPORT) ;
   	if (false == _appContext->getSuperviseur()->getPilot()->traitError(NSAgentStatus::LdV))
      return ;
  }
	_appContext->getUtilisateur()->importPatientLdv() ;
}

void
TMyApp::CmImportUserLdv()
{
  NSUtilisateurChoisi* pUser = _appContext->getUtilisateur() ;

  // only an administrator can create a user
  //
	if (((NSUtilisateurChoisi*) 0 == pUser) || (false == pUser->isAdmin()))
  {
		string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "administratorRightsNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

  NautilusPilot* pPilot = _appContext->getSuperviseur()->getPilot() ;
  if ((NautilusPilot*) NULL == pPilot)
    return ;

  if (false == pPilot->isOperationalLdVAgent())
  {
   	if (false == pPilot->traitError(NSAgentStatus::LdV))
      return ;
  }

	pUser->importUserLdv() ;
}

void
TMyApp::CmImportCorrespLdv()
{
	// si pas d'utilisateur en cours : on sort
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

  if (false == _appContext->getSuperviseur()->getPilot()->isOperationalLdVAgent())
  {
   	if (false == _appContext->getSuperviseur()->getPilot()->traitError(NSAgentStatus::LdV))
    		return ;
  }
  _appContext->getUtilisateur()->importCorrespLdv() ;
}

void
TMyApp::CmNewDoc()
{
  if (_appContext->getEpisodus())
    _appContext->getEpisodus()->resetCaptureArray() ;

  _appContext->getSuperviseur()->AppelUtilisateur() ;
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    GetMainWindow()->CloseWindow() ;
}

void TMyApp::CmNewPat()
{
  if (_appContext->getEpisodus())
    _appContext->getEpisodus()->resetCaptureArray() ;

  if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  if (false == _appContext->getUtilisateur()->isUser())
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "userOnlyFunction") ;
    erreur(sErrMsg.c_str(), standardError, 0) ;
    return ;
  }

  string sInPatientsFileDesc = _appContext->getSuperviseur()->getInPatientsFileDesc() ;
  if (string("") != sInPatientsFileDesc)
  {
    if (_frame && ((NSInPersonsChild*) NULL == _frame->getInPersonsChild()))
      _frame->openWhoIsThereWindow(sInPatientsFileDesc) ;

    CmBureau() ;

    return ;
  }

  string ps = string("User opens a patient") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getUtilisateur()->AppelPatient() ;
}

void
TMyApp::CmBureau()
{
  if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  int retVal = ::MessageBox(_appContext->GetMainWindow()->GetHandle(), "Voulez-vous vraiment fermer le patient en cours ?", "Message Nautilus", MB_YESNO) ;
  if (IDYES != retVal)
    return ;

  string ps = string("User closes the patient") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if (_appContext->getEpisodus())
    _appContext->getEpisodus()->resetCaptureArray() ;

  // _appContext->getUtilisateur()->fermePatEnCours() ;
  if (_appContext->getUtilisateur()->CanClosePatient())
    _appContext->getUtilisateur()->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnDoNothing) ;
}

void
TMyApp::CmOpenLdV()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens LdV") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->LdV_show() ;
}

void
TMyApp::CmOpenDrugs()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens drugs list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->drugs_show() ;
}

void
TMyApp::CmOpenProcess()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens processes list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->process_show() ;
}

void
TMyApp::CmOpenGoals()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens goals list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->goals_show() ;
}

void
TMyApp::CmOpenFollowUp()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens follow-up list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->followUp_show() ;
}

void
TMyApp::CmShowCurves()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens curves") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->followUp_show() ;
}

void
TMyApp::CmEpisodusBuildMessage()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User builds message for Prometheus") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = _appContext->getEpisodus() ;
  if (pEpisodus)
    pEpisodus->appendPrometheFile() ;
}

void
TMyApp::CmEpisodusSendMessage()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

  string ps = string("User sends message to Prometheus") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = _appContext->getEpisodus() ;
  if (pEpisodus)
    pEpisodus->sendPrometheMessage() ;
}

void
TMyApp::CmDiogene()
{
  NSPredi* pPredi = _appContext->getPredi() ;
  if (pPredi)
    pPredi->sendMessage() ;
}

void
TMyApp::CmEpisodusSurf()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

  string ps = string("User opens Prometheus web site") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = _appContext->getEpisodus() ;
  if ((pEpisodus) && (pEpisodus->sPrometheURL != ""))
    _appContext->getSuperviseur()->NavigationEncyclopedie(pEpisodus->sPrometheURL, _appContext) ;
}

void
TMyApp::CmEpisodusSurfPerso()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

  string ps = string("User opens personal information on Prometheus web site") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSEpisodus* pEpisodus = _appContext->getEpisodus() ;
  if ((pEpisodus) && (pEpisodus->sPrometheURL != ""))
    _appContext->getSuperviseur()->NavigationEncyclopedie(pEpisodus->sPrometheURL, _appContext) ;
}

void
TMyApp::CmChemise()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  _appContext->getPatient()->CmChemise() ;
}

// -----------------------------------------------------------------------------
// lancement RichEdit Nautilus
// -----------------------------------------------------------------------------
void
TMyApp::CmNewTtext()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens new text") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmNewTTxt() ;
}

// -----------------------------------------------------------------------------
// lancement Word
// -----------------------------------------------------------------------------
void
TMyApp::CmNewTmpl()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens Word") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmNewTmpl() ;
}

// -----------------------------------------------------------------------------
// ouverture fichier RichEdit Nautilus
// -----------------------------------------------------------------------------
void
TMyApp::CmOpenTtext()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens text") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmOpenTTxt() ;
}

// -----------------------------------------------------------------------------
// ouverture fichier Word
// -----------------------------------------------------------------------------
void
TMyApp::CmOpenTmpl()
{
  if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
    return ;

  string ps = string("User opens Word") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmOpenTmpl() ;
}

// -----------------------------------------------------------------------------
// pour enregistrer la position des fenetres
// -----------------------------------------------------------------------------
void
TMyApp::CmEnregWin()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User record windows position") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getUtilisateur()->setEnregWin(true) ;
	::MessageBox(_appContext->GetMainWindow()->GetHandle(), "La position actuelle des fenêtres sera enregistrée à la fermeture du patient.", "Message Nautilus", MB_OK) ;
}

void
TMyApp::CmImporter()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens import interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmImporter() ;
}

void
TMyApp::CmImportImg()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens images import interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmImportImg() ;
}

void
TMyApp::CmCaptureImg()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts image capture interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmAssistCapture() ;
}

void
TMyApp::CmCaptureTwain()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  _appContext->getPatient()->CmTwainCapture() ;
}

void
TMyApp::CmImportPdf()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens PDF import interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmOpenPdf() ;
}

void
TMyApp::CmImportHtml()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens HTML import interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmOpenHtml() ;
}

void
TMyApp::CmGenerePannel()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User opens pannel generation") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmGenerePannel() ;
}

void
TMyApp::CmNewCr()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts a new report") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmNewCr() ;
}

void
TMyApp::CmNewConsult()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  string ps = string("User starts a new encounter") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->CmNewConsult() ;
}

void
TMyApp::CmNewTypedDoc()
{
	if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) ||
      (_appContext->getPatient()     == (NSPatientChoisi*) 0))
		return ;

  _appContext->getPatient()->CmNewConsult(false) ;
}

void
TMyApp::CmNewMagic()
{
  // fonction libre pour l'instant
}

void
TMyApp::CmCarteVitale()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

	if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  // NSComptaPatient* pCompta = new NSComptaPatient(_appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(_appContext->getSuperviseur(), _appContext, _appContext->getPatient()->getNss()) ;

  NSComptaPatient Compta(_appContext, &personInfo) ;
  Compta.CmCarteVitale() ;
}

void
TMyApp::CmOpenAdmin()
{
	if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens demographic information") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  _appContext->getPatient()->admin_show() ;
}

void
TMyApp::CmShowId()
{
	if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }
  string sPatientId = _appContext->getPatient()->getNss() ;
  IdentifierDialog(_appContext->GetMainWindow(), _appContext, sPatientId).Execute() ;
}

void
TMyApp::CmWebExport()
{
  if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  _appContext->getPatient()->CmWebExport() ;
}

void
TMyApp::CmFicheCompt()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

	if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens a new accounting form") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // NSComptaPatient* pCompta = new NSComptaPatient(_appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(_appContext->getSuperviseur(), _appContext, _appContext->getPatient()->getNss()) ;

  NSComptaPatient Compta(_appContext, &personInfo) ;
  Compta.CmFicheCompt() ;
}

void
TMyApp::CmSituation()
{
  if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  if (_appContext->getPatient() == (NSPatientChoisi*) 0)
  {
    string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
  }

  string ps = string("User opens accounting interface") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // NSComptaPatient* pCompta = new NSComptaPatient(_appContext, (NSPatInfo *)appContext->getPatient()) ;
  NSPersonInfo personInfo(_appContext->getSuperviseur(), _appContext, _appContext->getPatient()->getNss()) ;

  NSComptaPatient Compta(_appContext, &personInfo) ;
  Compta.CmSituation() ;
}

void
TMyApp::CmAga()
{
try
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  NSCriteres Criteres ;

  string ps = string("User opens aga list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(_appContext->GetMainWindow(), _appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg ;

  while (Criteres.getDateAga1() <= Criteres.getDateAga2())
  {
    // on doit instancier et ouvrir un NSAgaDocument...
    NSAgaDocument AgaDoc((TDocument*) 0, _appContext) ;

    if (!AgaDoc.Open(&Criteres))
    {
      erreur("Impossible d'ouvrir le document AGA", standardError, 0, _appContext->GetMainWindow()->GetHandle()) ;
      break ;
    }

    // Note : on n'affiche plus la MessageBox dans le cas nbAga == 0
    if (AgaDoc.nbAga > 0)
    {
      NSListAgaDialog* pListAgaDlg = new NSListAgaDialog(_appContext->GetMainWindow(), _appContext, &AgaDoc) ;
      int idReturn = pListAgaDlg->Execute() ;
      delete pListAgaDlg ;

      if (IDCANCEL == idReturn)
      {
        AgaDoc.Close() ;
        break ;
      }

      if (AgaDoc.bImprimer)
      {
        // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
        NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
        if (pDocCompta)
        {
          pDocCompta->pAgaRefDoc = new NSAgaRefDocument(&AgaDoc) ;
          pDocCompta->pAgaRefDoc->Publier(false) ;
        }
      }
    }
    AgaDoc.Close() ;

    string sCurrentDate = Criteres.getDateAga1() ;
    incremente_date(sCurrentDate) ;

    if (Criteres.getDateAga1() == sCurrentDate)
      return ;

    Criteres.setDateAga1(sCurrentDate) ;
  }

  ::MessageBox(_appContext->GetMainWindow()->GetHandle(), "Liste des AGA terminée.", "Message Nautilus", MB_OK) ;
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
   		return ;

  string ps = string("User opens unpaid list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSImpDocument...
  NSImpDocument ImpDoc(0, _appContext) ;
  if (!ImpDoc.Open())
    return ;

  NSListImpDialog* pListImpDlg = new NSListImpDialog(_appContext->GetMainWindow(), _appContext, &ImpDoc) ;
  pListImpDlg->Execute() ;

  if (ImpDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
    if (pDocCompta)
    {
      pDocCompta->pImpRefDoc = new NSImpRefDocument(&ImpDoc) ;
      pDocCompta->pImpRefDoc->Publier(false) ;
    }
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  string ps = string("User opens acts list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSActDocument...
  NSActDocument ActDoc(0, _appContext) ;
  if (!ActDoc.Open())
    return ;

  NSListActDialog* pListActDlg = new NSListActDialog(_appContext->GetMainWindow(), _appContext, &ActDoc) ;
  pListActDlg->Execute() ;
  if (ActDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
    if (pDocCompta)
    {
      pDocCompta->pListActRefDoc = new NSListActRefDocument(&ActDoc) ;
      pDocCompta->pListActRefDoc->Publier(false) ;
    }
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  string ps = string("User opens acts sum form") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSActDocument...
  NSActDocument ActDoc(0, _appContext) ;
  if (false == ActDoc.Open(1))
    return ;

  NSSomActDialog* pSomActDlg = new NSSomActDialog(_appContext->GetMainWindow(), _appContext, &ActDoc) ;
  pSomActDlg->Execute() ;
  if (ActDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
    if (pDocCompta)
    {
      pDocCompta->pSomActRefDoc = new NSSomActRefDocument(&ActDoc) ;
      pDocCompta->pSomActRefDoc->Publier(false) ;
    }
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  string ps = string("User opens incomes list") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	// on doit instancier et ouvrir un NSEncaissDocument...
  NSEncaissDocument EncaissDoc(0, _appContext) ;
  if (!EncaissDoc.Open())
    return ;

  NSSomEncaissDialog* pSomEncaissDlg = new NSSomEncaissDialog(_appContext->GetMainWindow(), _appContext, &EncaissDoc) ;
  pSomEncaissDlg->Execute() ;
  if (EncaissDoc.bImprimer)
  {
    // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
    NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
    if (pDocCompta)
    {
      pDocCompta->pSomEncaissRefDoc = new NSSomEncaissRefDocument(&EncaissDoc) ;
      pDocCompta->pSomEncaissRefDoc->Publier(false) ;
    }
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  NSSaisieTPDialog SaisieTPDlg(_appContext->GetMainWindow(), _appContext) ;
  SaisieTPDlg.Execute() ;
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
  if ((_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0) || (_appContext->getPatient() == 0))
    return ;
    
  _appContext->getPatient()->CmFeuilleSecu() ;
}

void
TMyApp::CmImportNoemie()
{
try
{
  NSNoemie ImportNoemieDlg(_appContext->GetMainWindow(), _appContext) ;
  ImportNoemieDlg.Execute() ;
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
TMyApp::CmDisposables()
{
try
{
  LotsManagementDialog DisposableDlg(_appContext->GetMainWindow(), _appContext) ;
  DisposableDlg.Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmDisposables : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmDisposables.", standardError, 0) ;
}
}

void
TMyApp::CmLocalActs()
{
try
{
  LocalActsManagementDialog LocalActsDlg(_appContext->GetMainWindow(), _appContext) ;
  LocalActsDlg.Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmLocalActs : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmLocalActs.", standardError, 0) ;
}
}

void
TMyApp::CmNgap()
{
try
{
  NgapActsManagementDialog NgapDlg(_appContext->GetMainWindow(), _appContext) ;
  NgapDlg.Execute() ;
}
catch(TWindow::TXWindow& e)
{
  string sErr = string("Exception TMyApp::CmNgap : ") + e.why() ;
  erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  erreur("Exception TMyApp::CmNgap.", standardError, 0) ;
}
}

void
TMyApp::CmRecettes()
{
try
{
  NSRecettesDlg RecettesDlg(_appContext->GetMainWindow(), _appContext) ;
  RecettesDlg.Execute() ;
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
  NSDepensDlg* pDepensesDlg = new NSDepensDlg(_appContext->GetMainWindow(), _appContext) ;
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(_appContext->GetMainWindow(), _appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  NSCriteres Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg;

  // on doit instancier et ouvrir un NSAgaDocument...
  NSAgaDocument AgaDoc(0, _appContext) ;
  if (!AgaDoc.Open(&Criteres, true))
  {
    erreur("Impossible d'ouvrir le document AGA", standardError, 0, _appContext->GetMainWindow()->GetHandle()) ;
    return ;
  }

  if (AgaDoc.nbAga > 0)
  {
    NSListAgaDialog* pListAgaDlg = new NSListAgaDialog(_appContext->GetMainWindow(), _appContext, &AgaDoc) ;
    if (pListAgaDlg->Execute() == IDCANCEL)
    {
      AgaDoc.Close() ;
      delete pListAgaDlg ;
      return ;
    }

    if (AgaDoc.bImprimer)
    {
      // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
      NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
      if (pDocCompta)
      {
        pDocCompta->pAgaRefDoc = new NSAgaRefDocument(&AgaDoc) ;
        pDocCompta->pAgaRefDoc->Publier(false) ;
      }
    }
    delete pListAgaDlg ;
  }
  else
    ::MessageBox(_appContext->GetMainWindow()->GetHandle(), "Aucune recette ne correspond à la période choisie.", "Message Nautilus", MB_OK) ;

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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  // on lance d'abord le dialogue des critères
  NSCritAgaDialog* pCritAgaDlg = new NSCritAgaDialog(_appContext->GetMainWindow(), _appContext) ;
  if (pCritAgaDlg->Execute() != IDOK)
  {
    delete pCritAgaDlg ;
    return ;
  }

  NSCriteres Criteres = *(pCritAgaDlg->pCriteres) ;
  delete pCritAgaDlg ;

  // on doit instancier et ouvrir un NSDepensDocument...
  NSDepensDocument DepDoc(0, _appContext) ;
  if (!DepDoc.Open(&Criteres))
  {
    erreur("Impossible d'ouvrir le document des dépenses", standardError, 0, _appContext->GetMainWindow()->GetHandle()) ;
    return ;
  }

  if (DepDoc.nbDepens > 0)
  {
    NSListDepensDialog* pListDepDlg = new NSListDepensDialog(_appContext->GetMainWindow(), _appContext, &DepDoc) ;
    if (pListDepDlg->Execute() == IDCANCEL)
    {
      DepDoc.Close() ;
      delete pListDepDlg ;
      return ;
    }

    if (DepDoc.bImprimer)
    {
      // On ne fait pas de delete du document compta car NSVisualView le fait en fin d'impression
      NSDocCompta* pDocCompta = _appContext->getUtilisateur()->getDocCompta() ;
      if (pDocCompta)
      {
        pDocCompta->pDepRefDoc = new NSDepRefDocument(&DepDoc) ;
        pDocCompta->pDepRefDoc->Publier(false) ;
      }
    }
    delete pListDepDlg ;
  }
  else
    ::MessageBox(_appContext->GetMainWindow()->GetHandle(), "Aucune dépense ne correspond à la période choisie.", "Message Nautilus", MB_OK) ;

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
  if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
    return ;

  NSAnnee2035Dialog AnneeDlg(_appContext->GetMainWindow(), _appContext) ;
  if (IDOK == AnneeDlg.Execute())
  {
    string sAnnee2035 = AnneeDlg.sAnnee ;
    if ((sAnnee2035 != "") && (strlen(sAnnee2035.c_str()) == 4))
    {
      NS2035Dialog Dlg(_appContext->GetMainWindow(), _appContext, sAnnee2035) ;
      Dlg.Execute() ;
    }
    else
    {
      erreur("La date saisie n'est pas correcte.", standardError, 0) ;
    }
  }
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
  string ps = string("User closes Episodus using File - Exit") ;
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // CanClose() ;
  NSPatientChoisi*     pOpenedPatient = _appContext->getPatient() ;
  NSUtilisateurChoisi* pPreviousUser  = _appContext->getUtilisateur() ;
  if (pPreviousUser && pOpenedPatient)
  {
    if (false == pPreviousUser->CanClosePatient())
      return ;

    NSToDoTask *pToDo = new NSToDoTask ;
    pToDo->setWhatToDo(string("CloseMainWindow")) ;

    pPreviousUser->setToDoAfterPatClose(pToDo) ;
    pPreviousUser->ClosePatientAsynchronous(NSUtilisateurChoisi::cpnExecToDo) ;

    return ;
  }

  // cas de fermeture historique (autre cas : fermePatEnCours dans nschoisi.cpp)
  // on vérifie que le patient existe si on l'a déjà fermé ...
  //

  if ((NSPatientChoisi*) NULL == _appContext->getPatient())
    GetMainWindow()->CloseWindow() ;

/*
  if (_appContext->getPatient())
    _appContext->getPatient()->_bCanCloseHisto = true ;
  if (prendClient()->CloseChildren())
    GetMainWindow()->CloseWindow() ;
*/
}

void
TMyApp::CmHelp()
{
  if (_appContext->getSuperviseur())
  {
    string ps = string("User opens help") ;
    _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

    _appContext->getSuperviseur()->setAideIndex("") ;
    _appContext->getSuperviseur()->setAideCorps("home.htm") ;
    _appContext->NavigationAideEnLigne() ;
  }
}

void
TMyApp::CmUserFunction(int iIndex)
{
  if ((iIndex < 0) || (iIndex > 8))
    return ;

try
{
	string sModuleName = _appContext->getSuperviseur()->getUserModule(iIndex) ;
  if (string("") == sModuleName)
    return ;

  TModule* pDCModule = new TModule(sModuleName.c_str(), TRUE) ;
  if ((TModule*) NULL == pDCModule)
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

  ((*pAdresseFct)((TWindow far *) _appContext->GetMainWindow(), (NSContexte far *) _appContext)) ;

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
  // NSEmailFile* pMailCfg = new NSEmailFile(_appContext) ;
  // pMailCfg->Lancer() ;
  // delete pMailCfg ;

  NsMailToolsDialog mailTools(_appContext->GetMainWindow(), _appContext) ;
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
  // _appContext->getSuperviseur()->pEpisodus->SetStateOn(true) ;

  _appContext->getSuperviseur()->pEpisodus->ImportModels() ;
  _appContext->getSuperviseur()->pEpisodus->SetMouseHookState(true) ;

  _appContext->getSuperviseur()->pEpisodus->lanceHook() ;

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
  NsEpisodusOutilDialog outilDlg(_appContext->GetMainWindow(), _appContext) ;
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
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSPrintParamsDlg* pDlg = new NSPrintParamsDlg(_appContext->GetMainWindow(), _appContext) ;
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
  _appContext->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSUpdateParamsDlg* pDlg = new NSUpdateParamsDlg(_appContext->GetMainWindow(), _appContext) ;
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
  NSInitParamsDlg* pDlg = new NSInitParamsDlg(_appContext->GetMainWindow(), _appContext) ;
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
	NSPrometheFile* pPrometCfg = new NSPrometheFile(_appContext) ;
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
  _appContext->getSuperviseur()->pEpisodus->SetLearning(true) ;
  //appContext->getSuperviseur()->pEpisodus->createViewData() ;

  if (_appContext->getSuperviseur()->pEpisodus->pCurrentModel)
    _appContext->getSuperviseur()->pEpisodus->ExportModels() ;
*/
}


void
TMyApp::CmEpisoModels()
{
try
{
#ifdef _EXT_CAPTURE
	ArrayEpisodusModelData* pM    = _appContext->getSuperviseur()->getEpisodus()->pCaptureEngine->pModels ;
	ArrayEpisodusModelData* pSubM = _appContext->getSuperviseur()->getEpisodus()->pCaptureEngine->pSubModels ;
#endif

#ifndef _EXT_CAPTURE
	NSModelDlg* pRefDlg = new NSModelDlg(_appContext->GetMainWindow(), _appContext, NULL) ;
#else
	NSModelDlg* pRefDlg = new NSModelDlg(_appContext->GetMainWindow(), NULL, pM, pSubM, _appContext->PathName("FGLO")) ;
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
  NSAddFunctions* pDlg = new NSAddFunctions(_appContext->GetMainWindow(), _appContext) ;
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
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

	if (_appContext->getPatient() == 0)
	{
		string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

	string sMsg = _appContext->getSuperviseur()->getText("patientSynchronization", "patientSynchronizationWarning") ;
	string sTxt = _appContext->getSuperviseur()->getText("patientSynchronization", "patientImportAction") ;
	int retVal = ::MessageBox(0, sMsg.c_str(), sTxt.c_str(), MB_YESNO) ;
	if (retVal == IDNO)
		return ;

	string sGlobalLogin, sGlobalPasswd ;
	if (false == _appContext->getUtilisateur()->haveGlobalSessionPassword())
	{
		LogPassInterface* pDlgLogPass = new LogPassInterface(_appContext->GetMainWindow(), _appContext) ;
		if (pDlgLogPass->Execute() == IDCANCEL)
			return ;

		sGlobalLogin = pDlgLogPass->getLogin() ;
		sGlobalPasswd = pDlgLogPass->getPassword() ;
		delete pDlgLogPass ;
	}
	else
	{
		sGlobalLogin  = _appContext->getUtilisateur()->getGlobalLoginSession() ;
		sGlobalPasswd = _appContext->getUtilisateur()->getGlobalPasswordSession() ;
	}

  _appContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;
  _appContext->getUtilisateur()->fermePatEnCours(true) ;
  _appContext->getUtilisateur()->synchroImportThisPat(sGlobalLogin, sGlobalPasswd) ;

	// On lance le patient :
	// Récupération des données rattachées au patient : adresse et correspondants
  _appContext->getPatient()->InitialisationAsynchronous() ;
  _appContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

void
TMyApp::CmSynchroExportThisPat()
{
	if (_appContext->getUtilisateur() == (NSUtilisateurChoisi*) 0)
		return ;

	if (_appContext->getPatient() == 0)
	{
		string sErrMsg = _appContext->getSuperviseur()->getText("globalMessages", "patientNeeded") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
	}

	string sMsg = _appContext->getSuperviseur()->getText("patientSynchronization", "patientSynchronizationWarning") ;
	string sTxt = _appContext->getSuperviseur()->getText("patientSynchronization", "patientExportAction") ;
	int retVal = ::MessageBox(0, sMsg.c_str(), sTxt.c_str(), MB_YESNO) ;
	if (retVal == IDNO)
		return ;

	string sGlobalLogin, sGlobalPasswd ;
	if (false == _appContext->getUtilisateur()->haveGlobalSessionPassword())
	{
		LogPassInterface* pDlgLogPass = new LogPassInterface(_appContext->GetMainWindow(), _appContext) ;
		if (pDlgLogPass->Execute() == IDCANCEL)
			return ;
		sGlobalLogin = pDlgLogPass->getLogin() ;
		sGlobalPasswd = pDlgLogPass->getPassword() ;
		delete pDlgLogPass ;
	}
	else
	{
		sGlobalLogin = _appContext->getUtilisateur()->getGlobalLoginSession() ;
		sGlobalPasswd = _appContext->getUtilisateur()->getGlobalPasswordSession() ;
	}

  _appContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;
  _appContext->getUtilisateur()->fermePatEnCours(true) ;
  _appContext->getUtilisateur()->synchroExportThisPat(sGlobalLogin, sGlobalPasswd) ;

	// on termine la fermeture du patient
	delete _appContext->getPatient() ;
  _appContext->setPatient(0) ;

	// on rétablit le titre de l'application sans les infos patient
  _appContext->setMainCaption() ;
  _appContext->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

void
TMyApp::CmPilotAgentsManagement()
{
try
{
	NSAgentsList *pAgWin = new NSAgentsList(_appContext->GetMainWindow(), _appContext->getSuperviseur()) ;
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

	if ((buflen > 0) && (NULL != buf) && (NULL != _appContext))
    _appContext->setBaseDirectory(string(buf)) ;

	free(buf) ;
}

void
TMyApp::initMenus()
{
	setMenu("menubar", &_hAccelerator) ;
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

DEFINE_RESPONSE_TABLE1(NSMDIClient, NSBasicMDIClient)
END_RESPONSE_TABLE;


NSMDIClient::NSMDIClient(TModule* module)
            :NSBasicMDIClient(module)
{
}

NSMDIClient::~NSMDIClient()
{
}

static bool
isPatientDependantWindow(TWindow* win)
{
	NSWebServiceChild *wsChild = TYPESAFE_DOWNCAST(win, NSWebServiceChild) ;
  if (wsChild)
  	return false ;

	NSMailServiceChild *mailChild = TYPESAFE_DOWNCAST(win, NSMailServiceChild) ;
  if (mailChild)
  	return false ;

  OB1InterfaceContainer *BB1Child = TYPESAFE_DOWNCAST(win, OB1InterfaceContainer) ;
  if (BB1Child)
  	return false ;

  NSPilotProxyChild *pilotProxyChild = TYPESAFE_DOWNCAST(win, NSPilotProxyChild) ;
  if (pilotProxyChild)
  	return false ;

  NSInPersonsChild *inPersonsChild = TYPESAFE_DOWNCAST(win, NSInPersonsChild) ;
  if (inPersonsChild)
  	return false ;

  return true ;
}

static bool
isPatientHistoryWindow(TWindow* win)
{
  NSMDIChild *wsChild = TYPESAFE_DOWNCAST(win, NSMDIChild) ;
  if (wsChild)
  {
    TWindow* lastChild = win->GetLastChild() ;
    if (lastChild)
    {
      TWindow* curChild = lastChild->Next() ;
      do
      {
        NsHistorique *pHisto = TYPESAFE_DOWNCAST(curChild, NsHistorique) ;
        if (pHisto)
  	      return true ;
        curChild = lastChild->Next() ;
      }
      while ((curChild != lastChild) && lastChild) ;
    }
  }

  return false ;
}

static bool
isLigneDeVieWindow(TWindow* win)
{
  NSMDIChild *wsChild = TYPESAFE_DOWNCAST(win, NSMDIChild) ;
  if (wsChild)
  {
    TWindow* lastChild = win->GetLastChild() ;
    if (lastChild)
    {
      TWindow* curChild = lastChild->Next() ;
      do
      {
        NSLdvView *pLdV = TYPESAFE_DOWNCAST(curChild, NSLdvView) ;
        if (pLdV)
  	      return true ;
        curChild = lastChild->Next() ;
      }
      while ((curChild != lastChild) && lastChild) ;
    }
  }

  return false ;
}

// Returns true when finding a window that cannot close (skips NSWebServiceChild)
//
static bool
sCannotCloseButServices(TWindow* win, void*)
{
  if (false == isPatientDependantWindow(win))
    return false ;

	return (false == win->CanClose()) ;
}

static bool
sCannotCloseButServicesAndHistory(TWindow* win, void* pVoid)
{
  if (isPatientHistoryWindow(win))
    return false ;

  if (isLigneDeVieWindow(win))
    return false ;

  return sCannotCloseButServices(win, pVoid) ;
}

static void
sCloseChildButServices(TWindow* win, void*)
{
  if (false == isPatientDependantWindow(win))
    return ;

	win->Destroy() ;
  // delete win ;
}

static void
sCloseChildButServicesAndHistory(TWindow* win, void* pVoid)
{
  if (isPatientHistoryWindow(win))
    return ;

  if (isLigneDeVieWindow(win))
    return ;

  sCloseChildButServices(win, pVoid) ;
}

bool
NSMDIClient::CanCloseChildrenButServicesWindows(NSContexte *pContexte)
{
  bool bDeleteHistory = false ;

  if (pContexte && pContexte->getPatient() && pContexte->getPatient()->_bCanCloseHisto)
    bDeleteHistory = true ;

  string sMsg = string("Entering NSMDIClient::CanCloseChildrenButServicesWindows") ;
  if (bDeleteHistory)
    sMsg += string(" with delete history ON") ;
  else
    sMsg += string(" with delete history OFF") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  TWindow* pWillNotClose = (TWindow*) 0 ;

  // Get first window that wouldn't want to close
  //
  if (bDeleteHistory)
    pWillNotClose = FirstThat(sCannotCloseButServices) ;
  else
    pWillNotClose = FirstThat(sCannotCloseButServicesAndHistory) ;

	if ((TWindow*) NULL == pWillNotClose)
  {
    sMsg = string("NSMDIClient::CanCloseChildrenButServicesWindows: all targeted windows are ok to close") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    return true ;
  }

  // If there, it means that some windows cannot close
  //
  sMsg = string("NSMDIClient::CanCloseChildrenButServicesWindows: a targeted windows is not ok to close, leaving") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return false ;
}

/**
 *  Close all patient dependant windows (their documents will close too)
 */
bool
NSMDIClient::CloseChildrenButServicesWindows(NSContexte *pContexte)
{
  bool bDeleteHistory = false ;

  if (pContexte && pContexte->getPatient() && pContexte->getPatient()->_bCanCloseHisto)
    bDeleteHistory = true ;

  string sMsg = string("Entering NSMDIClient::CloseChildrenButServicesWindows") ;
  if (bDeleteHistory)
    sMsg += string(" with delete history ON") ;
  else
    sMsg += string(" with delete history OFF") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  TWindow* pWillNotClose = (TWindow*) 0 ;

  // Get first window that doesn't want to close (in order to let all windows
  // opened if one of them is not Ok to get closed)
  //
  if (bDeleteHistory)
    pWillNotClose = FirstThat(sCannotCloseButServices) ;
  else
    pWillNotClose = FirstThat(sCannotCloseButServicesAndHistory) ;

  // Some windows cannot close
  //
  if (pWillNotClose)
  {
    sMsg = string("NSMDIClient::CloseChildrenButServicesWindows: a targeted windows is not ok to close, leaving") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    return false ;
  }

  sMsg = string("NSMDIClient::CloseChildrenButServicesWindows: all targeted windows are ok to close") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  // Operating Destroy on all windows except non patient specific ones
  //
  if (bDeleteHistory)
    ForEach(sCloseChildButServices) ;
  else
    ForEach(sCloseChildButServicesAndHistory) ;

  // Close Blackboard interface
  //
  if (pContexte && pContexte->getBBinterface())
  {
    sMsg = string("NSMDIClient::CloseChildrenButServicesWindows: closing BBinterface") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    BB1BBInterface* pBBinterface = pContexte->getBBinterface() ;
    pContexte->setBBinterface((BB1BBInterface *) 0) ;

    if (pBBinterface->getContainer())
      pBBinterface->getContainer()->Destroy() ;
  }

  // Close drugs alert windows
  //
  if (pContexte && pContexte->_pAlertBoxChild)
  {
    pContexte->_pAlertBoxChild->Destroy() ;

    pContexte->_pAlertBoxChild  = (NSAlertServiceChild*) 0 ;
    pContexte->_pAlertBoxWindow = (NSAlertServiceWindow*) 0 ;
  }

  sMsg = string("NSMDIClient::CloseChildrenButServicesWindows: done") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return true ;
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



