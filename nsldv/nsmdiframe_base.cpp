// -----------------------------------------------------------------------------
// nsmdiframe_base.cpp
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

// #include <ostream.h>
// #include <checks.h>

// #include <owl\pch.h>
// #include <owl\applicat.h>
// #include <owl\decmdifr.h>

// #include <owl\gdiobjec.h>
// #include <owl\dialog.h>
// #include <owl\controlb.h>
// #include <owl\buttonga.h>
// #include <owl\statusba.h>
// #include <owl\docmanag.h>
// #include <owl\docview.h>
// #include <owl\olemdifr.h>
// #include <owl\oleview.h>
// #include <owl\oledoc.h>
// #include <stdlib.h>
// #include <string.h>
#include <time.h>
// #include <winuser.h>

#include "nautilus\nssuper.h"
#include "nsepisod\nsPilotProxy.h"
#include "nsldv\nsmdiframe_base.h"
#include "partage\ns_timer.h"
#include "nsbb\nsWhoIsThere.h"
#include "nsepisod\nsToDo.h"
#include "nsldv\adopt.h"

// #include "nautilus\nautilus.rh"

DEFINE_RESPONSE_TABLE1(NSMDIFrameBase, TDecoratedMDIFrame)
  EV_WM_QUERYENDSESSION,
  EV_WM_ENDSESSION,
  EV_WM_TIMER,
END_RESPONSE_TABLE;

NSMDIFrameBase::NSMDIFrameBase(NSContexte* pCtx, const char far *title, TResId menuResId, TMDIClient &clientWnd, bool trackMenuSelection, TModule* module)
               :TDecoratedMDIFrame(title, menuResId, clientWnd, trackMenuSelection, module),
                NSRoot(pCtx)
{
  _bToDoOnDuty = false ;
}

NSMDIFrameBase::~NSMDIFrameBase()
{
}

void
NSMDIFrameBase::SetupWindow()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  char szThis[30] ;
  sprintf(szThis, "%p", HWindow) ;
  string sMsg = string("SetupWindow for NSMDIFrame ") + string(szThis) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  TDecoratedMDIFrame::SetupWindow() ;

  TMyApp* pApp = pContexte->getSuperviseur()->getApplication() ;
  char szApp[30] ;
  sprintf(szApp, "%p", pContexte->GetMainWindow()->HWindow) ;
  sMsg = string("- Main Window ") + string(szApp) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szStatusBar[30] ;
  if (pApp->getStatusBar())
    sprintf(szStatusBar, "%p", pApp->getStatusBar()->HWindow) ;
  else
    strcpy(szStatusBar, "not created") ;
  sMsg = string("- Status bar ") + string(szStatusBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szControlBar[30] ;
  if (pApp->getControlBar())
    sprintf(szControlBar, "%p", pApp->getControlBar()->HWindow) ;
  else
    strcpy(szControlBar, "not created") ;
  sMsg = string("- Control bar ") + string(szControlBar) ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

  char szHarbor[30] ;
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
  openPilotProxy() ;
}

void
NSMDIFrameBase::openPilotProxy()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  NSPilotProxyWindow* pPilotProxyWindow = new NSPilotProxyWindow(pSuper->donneMainWindow(), pSuper) ;
	pSuper->setPilotProxyWindow(pPilotProxyWindow) ;

	NSPilotProxyChild* pPilotProxyChild = new NSPilotProxyChild(pSuper, *(pSuper->getApplication()->prendClient()), 0, pPilotProxyWindow) ;
  pPilotProxyChild->SetCaption("*system* Pilot proxy") ;
  // NSPilotProxyChild* pPilotProxyChild = new NSPilotProxyChild(pSuper, *Client, 0, pPilotProxyWindow) ;

	pPilotProxyChild->Create() ;
	pPilotProxyWindow->SetParent(pPilotProxyChild) ;

  pPilotProxyChild->SetWindowPos(0, 0, 0, 0, 0, SWP_NOZORDER) ;

  // Doing this bugs Windows list (à la MS KB article Q125435)
  // pPilotProxyChild->Show(SW_HIDE) ;
}

void
NSMDIFrameBase::openWhoIsThereWindow(string sInPatientsFileDesc)
{
  if (string("") == sInPatientsFileDesc)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSMDIClient* pClient = pSuper->getApplication()->prendClient() ;

  string ps = string("Opening the in-patient window.") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  NSMDIChildContent*       pChildContent    = new NSMDIChildContent(pContexte, pClient) ;

  NSInPersonsWindow*       pInPersonsWindow = new NSInPersonsWindow(pContexte, pChildContent, sInPatientsFileDesc) ;
	pSuper->setInPersonsWindow(pInPersonsWindow) ;

  NSInPersonsGadgetWindow* pGadgetWnd       = new NSInPersonsGadgetWindow(pContexte, pChildContent) ;
  pGadgetWnd->setPaneSplitter(pInPersonsWindow) ;

  pInPersonsWindow->setGadgetWindow(pGadgetWnd) ;

  pChildContent->setSplitter(pInPersonsWindow) ;
  pChildContent->setToolBar(pGadgetWnd) ;

  string sTitle = pSuper->getText("inPatiensList", "inPatients") ;

  _pInPersonsChild = new NSInPersonsChild(pContexte, *(pSuper->getApplication()->prendClient()), 0, pChildContent) ;
  _pInPersonsChild->SetCaption(sTitle.c_str()) ;

	_pInPersonsChild->Create() ;
	pInPersonsWindow->SetParent(_pInPersonsChild) ;

  // _pInPersonsChild->SetWindowPos(0, 0, 0, 800, 400, SWP_NOZORDER) ;
  pInPersonsWindow->SetWindowPosit() ;
  pInPersonsWindow->SetFocusForInit() ;
}

void
NSMDIFrameBase::InitAutomaticImportTimers()
{
  // Setting the "main" timer
  //
  SetTimer(ID_FRAME_TIMER, 50) ;

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
NSMDIFrameBase::EvTimer(uint timerId)
{
  bool bMustGarbageCollect = false ;

	if      (ID_SCAN_TIMER == timerId)
  {
    CheckForAutomaticImport(isScan) ;
    bMustGarbageCollect = true ;
  }
  else if (ID_IMAGE_TIMER == timerId)
  {
    CheckForAutomaticImport(isImage) ;
    bMustGarbageCollect = true ;
  }
  else if (ID_FRAME_TIMER == timerId)
    CmToDo() ;
  else if (ID_HISTORY_TIMER == timerId)
  {
    if (((NSPatientChoisi*) NULL == pContexte->getPatient()) ||
        (false == pContexte->getPatient()->_bCanCloseHisto))      KillTimer(ID_HISTORY_TIMER) ;    else    {      time_t timeCheck ;      time(&timeCheck) ;      double dEllapsedTime = difftime(timeCheck, _startOfWaitingTime) ;      bool bTimedOut = (dEllapsedTime > double(30)) ;      NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;      if (((NSHISTODocument*) NULL == pDocHis) || bTimedOut)
      {
        GetMdiClientWindow()->MdiChildrenGarbageCollector() ;

        if (bTimedOut)
        {
          string sErrorMsg = string("Time out when waiting for history to close... keeping on closing patient anyway!") ;
          pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trError) ;
        }

        KillTimer(ID_HISTORY_TIMER) ;

        if (pContexte->getUtilisateur())
        {
          // pContexte->getUtilisateur()->ClosePatientNextStep() ;
          //
          NSToDoTask* pTask = new NSToDoTask ;
          pTask->setWhatToDo(string("ClosePatientNextStep")) ;
          pTask->sendMe(pContexte->getSuperviseur()) ;
        }
      }
      else
        bMustGarbageCollect = true ;
    }
  }

  //
  //
  if (bMustGarbageCollect)
    GetMdiClientWindow()->MdiChildrenGarbageCollector() ;
}

//
// Look for the MDI submenu in a menubar by looking for the normal MDI
// commands, and return pos if found. Scan from right to left since the Window
// menu is usually near the right.
//
HMENU
NSMDIFrameBase::FindChildMenu(HMENU menu)
{
  if (menu)
  {
    int numItems = ::GetMenuItemCount(menu) ;
    for (int i = numItems-1; i >= 0; i--)
    {
      HMENU childMenu = ::GetSubMenu(menu, i) ;
      if (childMenu &&
          (TUser::GetMenuState(childMenu, CM_CASCADECHILDREN, MF_BYCOMMAND) != (uint)-1 ||
           TUser::GetMenuState(childMenu, CM_TILECHILDREN, MF_BYCOMMAND) != (uint)-1 ||
           TUser::GetMenuState(childMenu, CM_ARRANGEICONS, MF_BYCOMMAND) != (uint)-1)) {
        return childMenu ;
      }
    }
  }
  return 0 ;
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
NSMDIFrameBase::GetPopupMenuWindowHandle()
{
  HWND hwnd = NULL ;
  EnumWindows(GetPopupMenuEnumProc, (LPARAM) &hwnd) ;
  return hwnd ;
}

bool
NSMDIFrameBase::EvQueryEndSession()
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

static void
sCloseStripes(TWindow* win, void* HarborAsVoid)
{
  if ((NULL == HarborAsVoid) || ((TWindow*) NULL == win))
    return ;

  THarborEx* Harbor = (THarborEx*) HarborAsVoid ;
  if ((THarborEx*) NULL == Harbor)
  	return ;

  TEdgeSlipEx *wsSlip = TYPESAFE_DOWNCAST(win, TEdgeSlipEx) ;
  if ((TEdgeSlipEx*) NULL == wsSlip)
  	return ;

  TWindow* firstChild = wsSlip->GetFirstChild() ;
  if ((TWindow*) NULL == firstChild)
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
NSMDIFrameBase::CloseStripes()
{
  ForEach(sCloseStripes, pContexte->getSuperviseur()->getApplication()->getHarbor()) ;
}

