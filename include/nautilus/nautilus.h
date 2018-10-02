#ifndef __NAUTILUS_H__# define __NAUTILUS_H__

# include <process.h>
# include <owl\mdi.h># include <owl\decmdifr.h>
# include <owl\controlb.h>
# include <owl\statusba.h>
# include <owl\printer.h>
# include <owl\docking.h>
# include <owl\controlg.h>
# include <owl\buttonga.h>
# include <owl\menu.h>
# include <owl\splashwi.h>

class NSInPersonsChild ;
class NSMDIFrame ;

# include "docking\dockinge.h"
# include "docking\stripeDock.h"
# include "partage\nsdivfct.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "nsbb\nsMdiChild.h"

class NSPrinter : public OWL::TPrinter
{
  public:

    NSPrinter(OWL::TApplication* app = 0) ;
    ~NSPrinter() ;

    bool Print(TWindow* parent, TPrintout& printout, bool prompt) ;
} ;

class NSToDoTask ;

class NSMDIClient : public NSBasicMDIClient
{
  public:

    NSMDIClient(OWL::TModule* module = 0) ;
   	~NSMDIClient() ;

    bool CanCloseChildrenButServicesWindows(NSContexte *pContexte) ;
		bool CloseChildrenButServicesWindows(NSContexte *pContexte) ;
    // bool IsDecisionTreeOpened(NSContexte *pContexte, string sArchetype) ;

	DECLARE_RESPONSE_TABLE(NSMDIClient) ;
} ;

class NSContexte ;

// class TMyApp : public OWL::TApplication, public TOcModule
class TMyApp : public OWL::TApplication
{
  public:

/*
  OWL::TMenu*  pMainMenu ;
  OWL::TMenu*  pPersonMenu ;
  OWL::TMenu*  pFunctionsMenu ;
  OWL::TMenu*  pComptaMenu ;
  OWL::TMenu*  pToolsMenu ;
  OWL::TMenu*  pWindowsMenu ;
 	OWL::TMenu*  pHelpMenu ;
*/

    int     iCaptureHotKey ;
    int     iBabylonHotKey ;

    TMyApp(NSContexte *pCtx) ;
    ~TMyApp() ;

    NSMDIClient*    prendClient() { return _Client ; }

    //void				InsertControlBar(TButtonGadget* pGadget) ;
    //void				InsertControlBar(TControlGadget* pGadget) ;
    //void				InsertControlBar(TSeparatorGadget* pGadget) ;
    bool				   FlushControlBar() ;
    //bool                FlushEpisodusControlBar() ;

    void				   SetToolBarWindow(TWindow* pWindow) { _pToolBarWindow = pWindow ;	}
    TWindow*       GetToolBarWindow() { return _pToolBarWindow ; }

    void           setMenu(string sMenuName, HACCEL far *hAccelerator) ;

    bool           SetFirstInstance() ;
    bool           GetNumInstance(int& numInst) ;
    bool				   CanClose() ;
    bool				   CanClosePatient() ;
    int            BeginModal(TWindow* window, TWindow* notBlockedWindow, int flags) ;
    int 				   MessageLoop() ;
    void           DetruireFichiersTmp() ;
    void           logMessage(string) ;
    void           voidDebugPrintf(const char *szfmt,...) ;

    void           createSplash() ;
    void           sendMessageToSplash(string sMessage) ;
    void           setSplashPercentDone(int iPercent) ;
    TSplashWindow* getSplashWindow() { return _pSplashWindow ; }

    // Automatic deleting of windows objects when GUI windows closes - from TApplication
    void           Condemn(TWindow* win) ;
    void           Uncondemn(TWindow* win) ;
    void           DeleteCondemned() ;
    TWindow*       pNSCondemnedWindows ;

    bool           isForegroundApplication() ;

    NSMDIFrame*       getFrame()      { return _frame ; }
    OWL::TStatusBar* 	getStatusBar()  { return _sb ; }
    OWL::TControlBar* getControlBar() { return _cb ; }

    THarborEx*     			   getHarbor()              { return _Harbor ; }
    TDockableControlBarEx* getMainControlBar()      { return _cb1 ; }
    TDockableControlBarEx* getSecondaryControlBar() { return _cb2 ; }
    TDockableControlBarEx* getTertiary()            { return _cb3 ; }

    // Forms::TApplication* _pFormsApplication ;

    void CmBureau() ;

  protected:

    NSContexte     *_appContext ;
    TWindow*       _pToolBarWindow ;
    HACCEL         _hAccelerator ;

    //
    TSplashWindow* _pSplashWindow ;
    TDib*          _pSplashDib ;

    NSMDIClient*   _Client ;
    string         _sCurrentMenuName ;
    // Frame, Barre de statut et barre de contrôle
    NSMDIFrame* 	 		 _frame ;
    OWL::TStatusBar* 	 _sb ;
    OWL::TControlBar*  _cb ;

    // TGadget*		 		GadgetArray[20] ;
    // int				 		nbGadget ;

    // pour les barres dockables...
    THarborEx*     			_Harbor ;		/* TBorderHarbor* */
    TDockableControlBarEx* 	_cb1 ;		/* TBorderDockable* */
    TDockableControlBarEx* 	_cb2 ;		/* TBorderDockable* */
    TDockableControlBarEx* 	_cb3 ;		/* TBorderDockable* */

    // Override methods of TApplication
    void InitInstance() ;
    void InitMainWindow() ;
    //      bool ProcessMsg(MSG& msg);

    // Event handlers
    void EvNewView(TView& view) ;
    void EvCloseView(TView& view) ;
    void EvDropFiles(TDropInfo dropInfo) ;
    void Tempo(double s) ;
    void CmOpenLdV() ;
    void CmOpenDrugs() ;
    void CmOpenProcess() ;
    void CmOpenGoals() ;
    void CmOpenFollowUp() ;
    void CmShowCurves() ;
    void CmOpenAdmin() ;
    void CmShowId() ;
    void CmWebExport() ;
    void CmOpenHisto() ;
    void CmOpenFunction() ;
    void CmChemise() ;
    void CmImporter() ;
    void CmImportImg() ;
    void CmImportPdf() ;
    void CmImportHtml() ;
    void CmCaptureImg() ;
    void CmCaptureTwain() ;
    void CmGenerePannel() ;
    void CmNewTtext() ;
    void CmNewTmpl() ;
    void CmOpenTtext() ;
    void CmOpenTmpl() ;
    void CmMailBox() ;
    void CmOutils() ;
    void CmBBK() ;
    void CmGroupGd() ;
    void CmRequete() ;
    void CmResult() ;
    void CmRefArchetype() ;
    void CmEditRef() ;
    void CmEditArchetype() ;
    void CmEditTemplate() ;
    void CmDeviceInterface() ;
    void CmMateriels() ;
    void CmReconvoquer() ;
    void CmSetReconvocDate() ;
    void CmAbout() ;
    void CmHelp() ;
    void CmCreatPat() ;
    void CmCreatDoc() ;
    void CmModifDoc() ;
    void CmCreatCor() ;
    void CmModifCor() ;
    void CmCoopChartBox() ;

    void CmHealthTeam() ;
    void CmHealthTeamList() ;
    void CmImportPatientLdv() ;
    void CmImportCorrespLdv() ;
    void CmImportUserLdv() ;
    void CmVenuesList() ;

    void CmNewPat() ;
    void CmNewDoc() ;
    void CmNewEnTete() ;
    void CmModEnTete() ;
    void CmNewCr() ;         // compte-rendu
    void CmNewMagic() ;
    void CmNewConsult() ;    // consultation
    void CmNewTypedDoc() ;   // document de racine X
    void CmPREDI() ;
    void CmProjet() ;
    void CmNewQuestionnaire() ;
    void CmDiogene() ;
    void CmSauvegarde() ;
    void CmEnregWin() ;

    // comptabilité
    void CmCarteVitale() ;    void CmFicheCompt() ;    void CmSituation() ;
    void CmAga() ;
    void CmImpayes() ;
    void CmListeActes() ;
    void CmSommeActes() ;
    void CmSommeEncaiss() ;
    void CmSaisieTP() ;
    void CmFeuilleSecu() ;
    void CmImportNoemie() ;
    void CmDisposables() ;
    void CmLocalActs() ;
    void CmNgap() ;
    void CmRecettes() ;
    void CmDepenses() ;
    void CmListeRecettes() ;
    void CmListeDepenses() ;
    void Cm2035() ;

    void CmExit() ;
    // Episodus    void CmEpisoStart() ;
    void CmEpisoStop() ;
    void CmEpisoLearn() ;
    void CmEpisoModels() ;
    void IdcEpisodPhare() ;
    void CmEpisodusBuildMessage() ;
    void CmEpisodusSendMessage() ;
    void CmEpisodusSurf() ;

    // Perso params
    void CmEpisodusSurfPerso() ;
    void CmEpisoPrincipes() ;
    void CmPersoWinpos() ;

    // Client params
    void CmParamPrint() ;
    void CmParamSupports() ;
    void CmParamChemins() ;
    void CmParamPilot() ;
    void CmParamCompta() ;
    void CmParamCaptFrmt() ;
    void CmParamCaptPath() ;
    void CmParamOCRLearn() ;
    void CmParamOCRSwap() ;
    void CmParamIcons() ;
    void CmParamLettres() ;
    void CmParamUpdate() ;
    void CmParamInit() ;
    void CmParamBackup() ;
    void CmParamPredi() ;
    void CmParamKS() ;
    void CmParamModules() ;

    void CmUserFunction(int iIndex) ;
    void CmUserFunction1() ;
    void CmUserFunction2() ;
    void CmUserFunction3() ;
    void CmUserFunction4() ;
    void CmUserFunction5() ;
    void CmUserFunction6() ;
    void CmUserFunction7() ;
    void CmUserFunction8() ;
    void CmUserFunction9() ;

    void CmParamWysiwyg() ;

    //Fonction
    void CmFunctions() ;

    // Synchro
    void CmSynchroImportThisPat() ;
    void CmSynchroExportThisPat() ;

    void CmPilotAgentsManagement() ;

  //      void    logMessage(string);    LRESULT CmMouseHook(WPARAM wParam, LPARAM lParam);
    int     CM_MOUSE_HOOK;

    void    EvHotKey(int idHotKey) ;
    void    initMenus() ;
    void    initMainMenu() ;
    void    initPersonMenu() ;
    void    initFunctionsMenu() ;
    void    initComptaMenu() ;
    void    initToolsMenu() ;
    void    initWindowsMenu() ;
    void    initHelpMenu() ;
    void    initCurrentDir() ;

  DECLARE_RESPONSE_TABLE(TMyApp) ;} ;


boolTMyApp::FlushControlBar()
{
  if (GetToolBarWindow() != 0)
  {
    // on enlève les boutons de la control bar
    TGadget* pGadget = _cb2->FirstGadget() ;

    while (pGadget)    {
      TGadget* pGadgetRemoved = _cb2->Remove(*pGadget) ;
      if ((TGadget*) NULL == pGadgetRemoved)
      {
        erreur("Erreur à la destruction d'un bouton dans la barre d'outils", standardError) ;
        return false ;
      }
      else
      	delete pGadgetRemoved ;

      pGadget = _cb2->FirstGadget() ;    }
    //cb2->Insert(*new TSeparatorGadget) ;

    // Resizes secondary control bar (here to 0 pixels wide)
    //
    _cb2->LayoutSession() ;

    SetToolBarWindow((TWindow*) 0) ;
    // Harbor->Remove(*cb2) ;
  }
  return true ;
}


/*
boolTMyApp::FlushEpisodusControlBar()
{
  if (GetToolBarWindow() != 0)
  {
    // on enlève les boutons de la control bar
    TGadget* pGadget = cb3->FirstGadget() ;
    TGadget* pGadgetRemoved ;

    while (pGadget)    {
      pGadgetRemoved = cb3->Remove(*pGadget) ;
      if (!pGadgetRemoved)
      {
        erreur("Erreur à la destruction d'un bouton dans la barre d'outils", 0, 0) ;
        return false ;
      }
      else
      	delete pGadgetRemoved ;
      pGadget = cb3->FirstGadget() ;
    }

    //cb2->Insert(*new TSeparatorGadget) ;    cb3->LayoutSession() ;
    SetToolBarWindow(0) ;
    // Harbor->Remove(*cb2) ;
  }
  return true ;
}
*/

void
TMyApp::sendMessageToSplash(string sMessage)
{
	if (_pSplashWindow)
	  _pSplashWindow->SetText(sMessage.c_str()) ;
}

void
TMyApp::setSplashPercentDone(int iPercent)
{
	if (_pSplashWindow)
	  _pSplashWindow->SetPercentDone(iPercent) ;
}

//
// This method, originally from TApplication, is supercharged because, when
// desactivating all windows except the modal one, it also desactivates
// Lexique dialog, hence it is no longer possible to select a term using the mouse
//
// Go modal and enter a message loop until a quit message goes by.
// the flag values determine how window is used:
//
//   MB_APPLMODAL -   window is the owner to disable (if 0, none are disabled)
//   MB_TASKMODAL -   window is the window to make modal,
//                    all other top level windows are disabled
//   MB_SYSTEMMODAL - window is the window to make system modal (16-bit only)
//
// return -1 on errors
//
struct TEnumInfo {
  HWND  ModalWnd ;    // The window being made modal if needed
  short Count ;       // Count of windows in Wnds below
  HWND* Wnds ;        // list of windows that were disabled
  HWND  ExcludedWnd ; // A window not to be inactivated
};

static TEnumInfo* stackTop = 0 ; // points to topmost enuminfo in BeginModal calls

//
// Enum[Thread/Task]Windows callback. Counts or disables given window based on
// whether or not window list has been allocated yet.
//
static bool CALLBACK OWL_EXPORT16
disableWnds(HWND hWnd, TEnumInfo far* ei)
{
  if (hWnd == ei->ExcludedWnd)
    return true ;

  if (!(::GetWindowLong(hWnd, GWL_STYLE) & WS_CHILD)) {
    if (hWnd != ei->ModalWnd && ::IsWindowEnabled(hWnd)) {
      if (!ei->Wnds) {
        ei->Count++ ;  // no buffer yet, we are just counting
      }
      else {
          *(ei->Wnds++) = hWnd ;
          ::EnableWindow(hWnd, false) ;
      }
    }
  }
  return true ;
}

//
// Terminate the modal state initiated by BeginModal. Needs topmost ei block,
// and cleans the block up as needed inorder to be safe to be called twice.
//
static void termModal(TEnumInfo& ei)
{
  // Re-enable window(s) that are disabled in BeginModal()
  //
  if (ei.Wnds) {
    for (HWND* hWnd = ei.Wnds; *hWnd; hWnd++)
      ::EnableWindow(*hWnd, true) ;
    delete[] ei.Wnds ;
    ei.Wnds = 0 ;
  }
  else {
    if (ei.ModalWnd && IsWindow(ei.ModalWnd)) {
      ::EnableWindow(ei.ModalWnd, true) ;
      ei.ModalWnd = 0 ;
    }
  }
}

static void removeFromModalBlocked(TEnumInfo* pEi, HWND hBlockedWnd)
{
  if ((NULL == pEi->Wnds) || (NULL == hBlockedWnd))
    return ;

  // Remove all hBlockedWnd from pEi->Wnds
  //
  int iDecal = 0 ;
  for (HWND* hWnd = pEi->Wnds ; *hWnd ; hWnd++)
  {
    if (*hWnd == hBlockedWnd)
      iDecal++ ;

    if (iDecal > 0)
      *hWnd = *(hWnd + iDecal) ;
  }

  ::EnableWindow(hBlockedWnd, true) ;
}

int
TMyApp::BeginModal(TWindow* window, TWindow* notBlockedWindow, int flags)
{
  TEnumInfo  ei = { 0, 0, 0, 0 } ;
  TEnumInfo* lastStackTop = stackTop ; // Keep last stackTop to restore later
  stackTop = &ei ;                     // Point stackTop to topmost ei

  // The concept of SYSTEM MODAL applies only to the 16-bit environment of
  // Windows. The semantics of TASKMODAL is the closest to SYSMODAL in
  // 32-bit - specially in relation to the meaning of the 'window' parameter.
  // So we'll coerce SYSTEM MODAL to TASK MODAL
  //
  if (flags & MB_SYSTEMMODAL) {
    flags &= ~MB_SYSTEMMODAL ;
    flags |=  MB_TASKMODAL ;
  }

  // Check modal state
  //
  if (flags & MB_TASKMODAL) {
    // Save the window to make modal
    //
    if (window)
      ei.ModalWnd = window->GetHandle() ;
    if (notBlockedWindow)
      ei.ExcludedWnd = notBlockedWindow->GetHandle() ;

    // Count windows to disable
    //
    if (!EnumThreadWindows(GetCurrentThreadId(), (WNDENUMPROC)disableWnds,
                           TParam2((TEnumInfo far*)&ei)))
      return -1 ;

    // Allocate list of windows to disable, disable windows that are
    // enabled and then stuff them into the list.
    //
    HWND* hWnds = ei.Wnds = new HWND[ei.Count + 1] ;
    memset(ei.Wnds, 0, sizeof(TModule::THandle)*(ei.Count + 1)) ;

    EnumThreadWindows(GetCurrentThreadId(), (WNDENUMPROC)disableWnds,
                      TParam2((TEnumInfo far*)&ei)) ;

    ei.Wnds = hWnds ;  // Restore alloc'd pointer since enumerator bumps it

    // now, reactivate the window that should remain activated (usually Lexique)
    //
    // if (notBlockedWindow)
    //  removeFromModalBlocked(&ei, notBlockedWindow->GetHandle()) ;
  }
  else if (window) {

    // In MB_APPMODAL mode, we simply disable the window specified
    //
    ei.ModalWnd = window->GetHandle() ;  // Remember who to re-enable later
    CHECK(ei.ModalWnd) ;
    window->EnableWindow(false) ;
  }

  // Enter message loop, saving & restoring the current status & getting the
  // returned result.
  //
  int result ;
  try {
    result = MessageLoop() ;
  } catch (...) {
    termModal(ei) ;
    stackTop = lastStackTop ;
    throw ;
  }
  stackTop = lastStackTop ;
  termModal(ei) ;            // Just in case termModal was missed in EndModal

  // Return the result from the modal window's EndModal call
  //
  return result ;
}

bool
TMyApp::isForegroundApplication()
{
  HWND hwnd = GetForegroundWindow() ;
  if ((HWND) 0 == hwnd)
    return false ;

  DWORD foregroundPid ;
  if (GetWindowThreadProcessId(hwnd, &foregroundPid) == 0)
    return false ;

  return (foregroundPid == getpid()) ;
}

/*
class NSDecisionTreeView ;

static bool IsDecisionTree(TWindow* pWnd, void* psA)
{
  if ((TWindow*) NULL == pWnd)
    return false ;

	string* psArchetype = reinterpret_cast<string*>(psA) ;
  if ((string*) NULL == psArchetype)
    return false ;

	NSDecisionTreeView* pDTView = dynamic_cast<NSDecisionTreeView*>(pWnd) ;
	if ((NSDecisionTreeView*) NULL == pDTView)
		return false ;

  if ((NSDecisionTreeDocument*)NULL == pDTView->_pDoc)
    return false ;

  if (pDTView->_pDoc->getArchetype() != *psArchetype)
    return false ;

  return true ;
}

bool
NSMDIClient::IsDecisionTreeOpened(NSContexte *pContexte, string sArchetype)
{
  if (string("") == sArchetype)
    return false ;

  TWindow* pDecisionTreeWindow = FirstThat(IsDecisionTree, (void*) &sArchetype) ;
  if (pDecisionTreeWindow)
    return true ;

  return false ;
}
*/

#endif // !__NAUTILUS_H__
