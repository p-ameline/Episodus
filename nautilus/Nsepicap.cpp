// -----------------------------------------------------------------------------// Nsepicap.cpp
// -----------------------------------------------------------------------------
// Capture Données Utilisateur
// -----------------------------------------------------------------------------
// $Revision: 1.23 $
// $Author: pameline $
// $Date: 2017/04/08 09:10:25 $
// -----------------------------------------------------------------------------
// FLP - 08/2001
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

#ifndef __EPIBRAIN__
# include <locale>
# include <sstream>
# include <windows.h>

# include <vcl.h>
// # include "nautilus\nssmtp.h"
# include "nautilus\nsIndySmtp.h"

//#include <dirent.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <direct.h>

# include <bwcc.h>
// # include <windows.h>
# include <wingdi.h>
# include <owl/window.h>

# include <owl/owlpch.h>
# include <owl/splitter.h>

# include <owl/groupbox.h>

# include <vcl/sysutils.hpp>

#endif // !__EPIBRAIN__

//#include <tlhelp32.h>
//#include <vdmdbg.h>

//#if !defined(OWL_LISTWIND_H)
//# include <owl/listwind.h>
//#endif

// #include <owl/dialog.h>

#include "partage\Nsglobal.h"
#include "nautilus\nssuper.h"
#include "nsdn\nsdochis.h"

#include "scktcomp.hpp"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
//#include "ns_hook\ns_hkfct.h"

//#include "nautilus\EnumProc.h"

#include "nautilus\nsepisod.rh"
#include "nautilus\nsepicap.h"
#include "nautilus\nsepisodview.h"
#include "nautilus\nsSOAPview.h"
#include "nsepisod\nsepidiv.h"
#include "nsepisod\nssoapdiv.h"
#include "nsepisod\nsPrediDlg.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nsldvvue.h"
#include "nautilus\nscaptview.h"
#include "nautilus\nscompub.h"
#include "nsbb\nsattvalex.h"
#include "nsbb\nsMView.h"
#include "nsbb\nsmanager.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsbrowse.h"

#include "nsbb\nsPaneSplitter.h"

//USEUNIT("nautilus\EnumProc.cpp");

// -----------------------------------------------------------------------------
//
// Class NSEpisodus
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSEpisodus::NSEpisodus(NSContexte * pCtx)
           :NSRoot(pCtx),
            CaptureArray(pCtx),
            newCaptureArray(pCtx)
{
try
{
#ifndef _EXT_CAPTURE
  pEpisodusData        = new EpisodusData() ;
  iDefaultAnalyse      = EpisodusData::byPlacement ;
  pModels              = new ArrayEpisodusModelData() ;
  pSubModels           = new ArrayEpisodusModelData() ;
#else
  pCaptureEngine       = new NSCaptureEngine() ;
#endif // _EXT_CAPTURE

	pCaptureDoc          = (NSCaptDocument*) 0 ;

  bMouseHookState      = false ; // on le met a (true) quand on lance episodus
  hOldWindowCaptured   = 0 ;

  pViewData            = (NSEpisodView*) 0 ;

#ifndef _EXT_CAPTURE
  pCurrentModel        = (EpisodusModel*) 0 ;
#endif // !_EXT_CAPTURE

  pSOAPView            = (NSSOAPView*) 0 ;
  pSOAPTank            = (NSSOAPTankView*) 0 ;
  pBasketList          = new SOAPBasketArray() ;
  pPrincipes           = new PrinciplesArray(pContexte->getSuperviseur()) ;

  pClassifExpert       = new classifExpert(pContexte) ;

  bActiveBabylon       = true ;
  bActiveGlobal        = true ;

  bAutoFlocule         = true ;
  bSendNow             = false ;
  sAsymetricHash       = string("") ;
  sPromethePrincipe    = string("") ;
  sPrometheMail        = string("") ;
  sPrometheURL         = string("") ;
  sPrometheUserId      = string("") ;
  sPrometheUserLogin   = string("") ;
  sPrometheUserPaswd   = string("") ;
  sBabylonKey          = string(" ") ;
  sCaptureKey          = string(" ") ;

  _iAutoSearchDelay    = 1000 ;
  _iAutoSearchMinChar  = 1 ;

  sPosSOAP             = string("") ;
  sPosTank             = string("") ;
  sPosCapture          = string("") ;
  sPosLdV              = string("") ;
  sOpenModeLdV         = string("") ;
  bAutoOpenLdV         = false ;
  bAllowFreeTextLdV    = false ;
  bAutoSave            = false ;
  _bDrugAddVirtualDrug = true ;
	sServiceUrl          = string("") ;
	sServiceTitle        = string("") ;
}
catch (...)
{
	erreur("Exception NSEpisodus ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSEpisodus::~NSEpisodus()
{
#ifndef _EXT_CAPTURE
	delete pEpisodusData ;
#else
	delete pCaptureEngine ;
#endif // _EXT_CAPTURE

	delete pBasketList ;
	delete pPrincipes ;

  // parcours de la liste des modèles pour voir si tous les modèles modifiés ont été sauvegardé
/*
  if (!(pModels->empty()))
    for (ArrayEpisodusModelIter IterModel = pModels->begin() ; IterModel != pModels->end() ; IterModel++)
      if ((*IterModel)->bModified)
      {
        TWindow * pMainWin = pContexte->getSuperviseur()->donneMainWindow() ;
        int retVal = MessageBox(pMainWin->GetHandle(), "Modèle non sauvegardé, voulez vous le sauvegarder ?", "Episodus", MB_YESNO) ;

        if (retVal == IDYES)
          // (*IterModel)->ExportModel() ;
          pModels->sauvegarder() ;
      }
*/

#ifndef __EPIBRAIN__
	if (pCaptureDoc)
		delete pCaptureDoc ;
#endif

#ifndef _EXT_CAPTURE
	delete pModels ;
	delete pSubModels ;
	delete pClassifExpert ;
#endif // !_EXT_CAPTURE
}

#ifndef _EXT_CAPTURE
// -----------------------------------------------------------------------------
// fonction renvoyant true si une capture est possible à partir des modèles
// présents dans la base de modèle
// -----------------------------------------------------------------------------
bool
NSEpisodus::CaptureHookOK(HWND HWnd)
{
  // Récupération de la fenêtre se trouvant sous la souris
  HWND AppWindow = WindowFromPoint(MousePoint) ;
  if (!AppWindow)
  {
    if (bModelFound)
      SetPhareOff() ;
    pCurrentModel = (EpisodusModel*) 0 ;
    return false ;
  }

  // si la fenêtre sélectionnée est une fenêtre fille, on récupère la fenêtre mère
  // on considère que la fenêtre mère a au moins le style WS_BORDER
  while ((GetWindowLong(AppWindow, GWL_STYLE) & WS_BORDER) != WS_BORDER)
  {
    AppWindow = (HWND) GetWindowLong(AppWindow, GWL_HWNDPARENT) ;
    if (!AppWindow)
    {
      if (bModelFound)
        SetPhareOff() ;
      pCurrentModel = (EpisodusModel*) 0 ;
      return false ;
    }
  }

  MylParam * pSpylParam = new MylParam((TWindow::THandle) AppWindow, (TWindow::THandle) AppWindow, pContexte) ;

  // déclaration de la fonction d'énumération
  WNDENUMPROC lpEnumChildWindows = (WNDENUMPROC)MakeProcInstance((FARPROC)NSEpisodus::SearchChildWindowRecur, (HINSTANCE)hInstance) ;

  // efface les données propres à la capture précédente
  pEpisodusData->clear() ;

  // énumération des "fenêtres" filles
  EnumChildWindows(AppWindow, lpEnumChildWindows, LPARAM(pSpylParam)) ;

  EpisodusModel * pHookModel = pModels->trouveModele(pEpisodusData) ;
  // si aucun modèle ne correspond on ajoute le modèle
  if (NULL == pHookModel)
  {
    if (bModelFound)
      SetPhareOff() ;
    return false ;
  }

  if (false == bModelFound)
    SetPhareOn() ;
  return true ;

/*
  if (hOldWindowCaptured != HWnd)
  {
    hOldWindowCaptured = HWnd ;

    char    sText[1024] ;
    char    sMyMessage[1024] ;

    SendMessage(HWnd, WM_GETTEXT, sizeof(sText), LPARAM(&sText)) ;
    sprintf(sMyMessage, "Window Text : <%s>\n", sText) ;
    logSpyMessage(sMyMessage) ;
  }
*/

  // log
//  sprintf(sMyMessage, "Capture Hook\n") ;
//  logSpyMessage(sMyMessage) ;

/*
  HWND AppWindow = WindowFromPoint(MousePoint) ;
  if (hOldWindowCaptured != AppWindow)
  {
    hOldWindowCaptured = AppWindow ;

    if (bMouseHookState == true)
    {
      WNDENUMPROC     lpEnumChildWindows ;

      // si la fenêtre sélectionnée est une fenêtre fille, on récupère la fenêtre mère
      // on considère que la fenêtre mère a au moins le style WS_BORDER
      while ((GetWindowLong(AppWindow, GWL_STYLE) & WS_BORDER) != WS_BORDER)
      {
        AppWindow = (HWND) GetWindowLong(AppWindow, GWL_HWNDPARENT) ;
      }

      MylParam * pSpylParam = new MylParam((THandle)AppWindow, (THandle)AppWindow, pContexte) ;

      // déclaration de la fonction d'énumération
      lpEnumChildWindows = (WNDENUMPROC)MakeProcInstance((FARPROC)NSEpisodus::SearchChildWindowRecur, (HINSTANCE)hInstance) ;

      // énumération des "fenêtres" filles
      EnumChildWindows(AppWindow, lpEnumChildWindows, LPARAM(pSpylParam)) ;

      // création du modèle en fonction de la capture effectuée
      EpisodusData * theModel = new EpisodusData(pEpisodusData) ;

      bool bFlagNewModel = true ;

      // Comparaison avec les modèles déjà stockés
      for (vector<EpisodusData *>::iterator pModelsIter = pContexte->getSuperviseur()->pEpisodus->pModels->begin() ; pModelsIter != pContexte->getSuperviseur()->pEpisodus->pModels->end() ; pModelsIter++)
      {
        // si on trouve un modèle correspondant on ne crée pas de modèle
        if (theModel->CompareWithModel(*pModelsIter))
          bFlagNewModel = false ;
      }

      // efface les données propres à la capture
      pEpisodusData->clear() ;  // va sûrement faire un problème pour EpisodusModelData::CreateModel

      // si aucun modèle ne correspond on ajoute le modèle
      if (bFlagNewModel == true)
      {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        TMyApp * pMyApp = pContexte->getSuperviseur()->pNSApplication ;
        pMyApp->FlushEpisodusControlBar() ;
        pMyApp->cb3->Insert(*new TButtonGadget(IDC_EPISOD_PHARE, IDC_EPISOD_PHARE, TButtonGadget::Command)) ;
        pMyApp->cb3->LayoutSession() ;
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        return false ;
      }
      else
      {
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        TMyApp * pMyApp = pContexte->getSuperviseur()->pNSApplication ;
        pMyApp->FlushEpisodusControlBar() ;
        pMyApp->cb3->Insert(*new TButtonGadget(IDC_EPISOD_PHARE, IDC_EPISOD_PHARE, TButtonGadget::Command)) ;
        pMyApp->cb3->LayoutSession() ;
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        return true ;
      }
    }
    else
      return false ;
  }
  else
    return false ;
*/
}
#endif // !_EXT_CAPTURE

//
// Put the information from newCaptureArray inside CaptureArray
//
// bEmptyTankFirst : Must empty CaptureArray before
// bResetNew       : Must empty newCaptureArray after
//
void
NSEpisodus::PutNewCaptureInTank(bool bEmptyTankFirst, bool bResetNew)
{
#ifndef __EPIBRAIN__
	if (bEmptyTankFirst)
		CaptureArray.vider() ;

	if (newCaptureArray.empty())
		return ;

	// Warning : we no longer check if there is not already a capture with the
  //           same path
  //
  for (CaptureIter captIter = newCaptureArray.begin() ; newCaptureArray.end() != captIter ; )
  {
    pCaptureDoc->newCapture(&CaptureArray, *captIter) ;

    if (bResetNew)
    {
      delete *captIter ;
    	newCaptureArray.erase(captIter) ;
    }
    else
      captIter++ ;

/*
    if (bResetNew)
    {
      pCaptureDoc->newCapture(&CaptureArray, *captIter) ;
    	newCaptureArray.erase(captIter) ;
    }
    else
    {
      pCaptureDoc->newCapture(&CaptureArray, new NSCapture(**captIter)) ;
      captIter++ ;
    }
*/
  }
#endif
}

// -----------------------------------------------------------------------------
// méthode Capture -- Capture des données de la fenêtre principale
// -----------------------------------------------------------------------------
void
NSEpisodus::Capture(HWND HWnd)
{
try
{
	if (false == bActiveGlobal)
		return ;

  HWND AppWindow = WindowFromPoint(MousePoint) ;
  if (!AppWindow)
    return ;

#ifdef _EXT_CAPTURE
	NSCaptureEngine::CAPTURE_RESULT iCaptResult = pCaptureEngine->Capture(AppWindow) ;
  switch (iCaptResult)
  {
    case NSCaptureEngine::captureOk         : break ;
    case NSCaptureEngine::modelNotFound     : createViewData(pCaptureEngine->getCurrentModel()) ;
                                              pCaptureEngine->clearCurrentModel() ;
                                              break ;
    case NSCaptureEngine::captureFailed     : erreur("La capture a déclenchée une erreur", standardError, 0) ;
                                              break ;
    case NSCaptureEngine::captureCancelled  : break ;
    default                                 : erreur("La capture a renvoyé un résultat inattendu", standardError, 0) ;
                                              break ;
  }
#else

  // si la fenêtre sélectionnée est une fenêtre fille, on récupère la fenêtre mère
  // on considère que la fenêtre mère a au moins le style WS_BORDER

  // If that window is a child window, we look for the interface window
  // (the one with WS_BORDER style)
  while ((GetWindowLong(AppWindow, GWL_STYLE) & WS_BORDER) != WS_BORDER)
  {
    AppWindow = (HWND)GetWindowLong(AppWindow, GWL_HWNDPARENT) ;
    if (!AppWindow)
      return ;
  }

  MylParam * pSpylParam = new MylParam((TWindow::THandle) AppWindow, (TWindow::THandle) AppWindow, pContexte) ;

  // déclaration de la fonction d'énumération
  WNDENUMPROC lpEnumChildWindows = (WNDENUMPROC)MakeProcInstance((FARPROC)NSEpisodus::SearchChildWindowRecur, (HINSTANCE)hInstance) ;

  // efface les données propres à la capture précédente
  pEpisodusData->clear() ;

  // énumération des "fenêtres" filles
  // enumerating all child windows
  EnumChildWindows(AppWindow, lpEnumChildWindows, LPARAM(pSpylParam)) ;
  pEpisodusData->Reorder() ;

  // création du modèle en fonction de la capture effectuée
  EpisodusData Captured(pEpisodusData) ;

  pCurrentModel = pModels->trouveModele(&Captured) ;

  // si aucun modèle ne correspond
  if ((EpisodusModel*) NULL == pCurrentModel)
  {
    ScreenCapture(AppWindow) ;

    if (captureSubModels(&Captured, &newCaptureArray))
    {
      pContexte->captureData(&newCaptureArray) ;
      return ;
    }

    reveilleToi() ;

    int retVal = MessageBox(0, "Cette fenêtre ne correspond à aucun modèle, voulez-vous la référencer ?", "Episodus", MB_YESNO) ;
    if (IDYES != retVal)
      return ;

    // on ajoute le modèle
    pCurrentModel = new EpisodusModel(&Captured) ;
    // variable permettant de savoir si le modèle a été modifié
    pCurrentModel->bModified    = true ;
    pCurrentModel->iModeAnalyse = iDefaultAnalyse ;

    // pModels->push_back(pCurrentModel) ;

    createViewData(pCurrentModel) ;

    delete pCurrentModel ;
    pCurrentModel = (EpisodusModel*) 0 ;
  }
  // Actualisation de la liste à l'écran
  else if (pViewData)
    pViewData->AfficheModel() ;

  // CaptureArray.vider() ;
  if (pCurrentModel)
  {
    if (pCurrentModel->useOCR())
    {
      ScreenCapture(AppWindow) ;
      OCRcapture() ;
    }

    // Récupération des données capturées
    // Storing the newly captured datas
    donneCaptureElemts(&newCaptureArray, pEpisodusData) ;

    // Transmission au NSContexte : qu'en fait-on ?
    // Showing them to the NSContexte : what can we do with them ?
    /* bool bCapt =*/ pContexte->captureData(&newCaptureArray) ;
  }
#endif // !_EXT_CAPTURE
}
catch (...)
{
  erreur("Exception NSEpisodus::Capture.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Cherche s'il existe des sous-modèles à capturer - et effectue la capture
// -----------------------------------------------------------------------------
bool
NSEpisodus::captureSubModels(EpisodusData* pCapturedData, NSCaptureArray * pCaptureArray)
{
  if (pSubModels->empty())
    return false ;

	//
	// Attention : s'assurer que l'algo ne s'arrête pas au premier modèle trouvé
	//             mais les capture tous
	// Warning   : check that we don't stop at the first model found but really
  //             capture them all
	//
	bool bSubModelFound = false ;
  // Première passe : comparaison exacte
  for (ArrayEpisodusModelIter i = pSubModels->begin() ; i != pSubModels->end() ; i++)
    if (captureSubModel(pCapturedData, *i, pCaptureArray))
      bSubModelFound = true ;

  return bSubModelFound ;
}

bool
NSEpisodus::captureSubModel(EpisodusData* pCapturedData, EpisodusData* pCompareData, NSCaptureArray* pCaptureArray)
{
  ArrayEpisodusData aCapturedElements ;
  ArrayEpisodusData aModelElements ;

  if (false == pCapturedData->captureSubModel(pCompareData, pCaptureArray, &aCapturedElements, &aModelElements))
    return false ;

  ArrayEpisodusDataIter iCapt  = aCapturedElements.begin() ;
  ArrayEpisodusDataIter iModel = aModelElements.begin() ;
  for ( ; (iCapt != aCapturedElements.end()) && (iModel != aModelElements.end()) ; iCapt++, iModel++)
    creeCapture(*iCapt, *iModel, pCaptureArray) ;

  return true ;
}

// -----------------------------------------------------------------------------
// Les données capturées se voient attribuer leur chemin, puis sont
// éventuellement rangées/classifiées.
// Captured data are given a path, then they are stored/classified.
// -----------------------------------------------------------------------------
void
NSEpisodus::donneCaptureElemts(NSCaptureArray* pCaptArray, EpisodusData* pData)
{
try
{
  if ((NULL == pCaptArray) || (NULL == pData))
    return ;

  pCaptArray->vider() ;

  if ((pCurrentModel->empty()) || (pData->empty()))
    return ;

  for (EpisodusDataIter iMod = pCurrentModel->begin() ; iMod != pCurrentModel->end() ; iMod++)
  {
    for (EpisodusDataIter iDat = pData->begin() ; iDat != pData->end() ; iDat++)
    {
      if ((*iMod)->CompareWithElementModel(*iDat, pCurrentModel->iModeAnalyse))
      {
        if (((*iDat)->sTextChild != string("")) && ((*iMod)->sCorrespondence != string("")))
          creeCapture(*iDat, *iMod, pCaptArray) ;
      }
    }
  }
}
catch (...)
{
	erreur("Exception NSEpisodus::donneCaptureElemts.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// méthode CaptureBabylon -- Capture des données en cours de sélection
// -----------------------------------------------------------------------------
void
NSEpisodus::CaptureBabylon(HWND HWnd)
{
try
{
  if (!bActiveBabylon)
    return ;

  // Récupération de la fenêtre se trouvant sous la souris
  // First, find the handle of the window just below mouse cursor
  HWND AppWindow = WindowFromPoint(MousePoint) ;
  if (!AppWindow)
    return ;

#ifdef _EXT_CAPTURE
  /*NSCaptureEngine::CAPTURE_RESULT iCaptResult = */ pCaptureEngine->CaptureBabylon(AppWindow) ;
#else
  TWindow captWindow(AppWindow) ;
  NS_CLASSLIB::TRect rectEdition = captWindow.GetWindowRect() ;

  NS_CLASSLIB::TPoint bitmapPoint(MousePoint.x - rectEdition.Left(), MousePoint.y - rectEdition.Top()) ;

  // Capture de la fenêtre / Screen capture
  ScreenCapture(AppWindow) ;

  // Recherche du bloc sélectionné / Finding the selection block
  NSOCRbloc * pBloc = new NSOCRbloc(rectEdition) ;
  if (!(screenMap.trouveBloc(pBloc, &bitmapPoint)))
  {
  	delete pBloc ;
    return ;
  }

  // Chargement de la dll d'OCR / Loading the OCR dll
  TModule * pDCModule = new TModule("nsm_ocr.dll", TRUE) ;

  bool (FAR * pAdresseFct)(NSContexte far *, NSOCRbloc far *, NSBitmap far *) ;

  if (!pDCModule)
  {
    erreur("Impossible d'ouvrir la DLL ns_ocr.dll.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return ;
  }

  // Récupération du pointeur sur la fonction // Getting function's pointer
  (FARPROC)pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  if (pAdresseFct == NULL)
  {
    delete pDCModule ;
    return ;
  }

  // OCR
  bool bReussi = ((*pAdresseFct)((NSContexte far *)pContexte, (NSOCRbloc far *)pBloc, (NSBitmap far *)&screenMap)) ;

  delete pDCModule ;

  if ((!bReussi) || (pBloc->sTexte == ""))
    return ;

	//pCaptureDoc->openView() ;
	pCaptureDoc->newCapture(NULL, pBloc, "") ;

#endif // !_EXT_CAPTURE
}
catch (...)
{
	erreur("Exception CaptureBabylon.", standardError, 0) ;
}
}


void
NSEpisodus::OCRcapture()
{
try
{
#ifdef _EXT_CAPTURE
	pCaptureEngine->OCRcapture() ;
#else
  if (pCurrentModel->empty())
    return ;

  TModule * pDCModule = new TModule("nsm_ocr.dll", TRUE) ;

  bool (FAR * pAdresseFct)(NSContexte far *, NSOCRbloc far *, NSBitmap far *) ;

  if (NULL == pDCModule)
  {
    erreur("Impossible d'ouvrir la DLL ns_ocr.dll.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return ;
  }

  (FARPROC)pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  if (NULL == pAdresseFct)
  {
    delete pDCModule ;
    return ;
  }

  bool bReussi ;
  for (EpisodusDataIter iMod = pCurrentModel->begin() ; iMod != pCurrentModel->end() ; iMod++)
  {
    if ((*iMod)->iModeCapture == EpisodusElemData::captureOCR)
    {
      ClassLib::TRect blocRect = (*iMod)->Placement ;
      NS_CLASSLIB::TColor bColor ;
      if (screenMap.enableCapture(&blocRect, &bColor))
      {
        NSOCRbloc Bloc(blocRect, bColor) ;
        bReussi = ((*pAdresseFct)((NSContexte far *)pContexte, (NSOCRbloc far *)&Bloc, (NSBitmap far *)&screenMap)) ;

        if ((bReussi) && (Bloc.sTexte != ""))
        {
          string sTexteCapt = Bloc.sTexte ;

          // Suppression des '_'
          size_t pos = sTexteCapt.find('_') ;
          while (pos != string::npos)
          {
            if (pos == strlen(sTexteCapt.c_str()) - 1)
              sTexteCapt = string(sTexteCapt, 0, pos) ;
            else
              sTexteCapt = string(sTexteCapt, 0, pos) + string(sTexteCapt, pos + 1, strlen(sTexteCapt.c_str()) - pos - 1) ;
            pos = sTexteCapt.find('_') ;
          }

          pEpisodusData->push_back(new EpisodusElemData(0, 0, "OCR", sTexteCapt, blocRect, (*iMod)->iOrder, "", EpisodusElemData::captureOCR, (*iMod)->iModeAnalyse, "")) ;
        }
      }
    }
  }
  delete pDCModule ;

#endif // !_EXT_CAPTURE
}
catch (...)
{
	erreur("Exception OCRcapture.", standardError, 0) ;
}
}

void
NSEpisodus::ScreenCapture(HWND HWnd)
{
try
{
  // screenMap.vider();
  TWindowDC * pWinDC = new TWindowDC(HWnd) ;
  TWindow myWindow(HWnd) ;
  ClassLib::TRect ScreenCoords = myWindow.GetWindowRect() ;
  screenMap.ScreenCapture(pWinDC, &ScreenCoords) ;
  delete pWinDC ;
}
catch (...)
{
  erreur("Exception ScreenCapture.", standardError, 0) ;
}
}
voidNSEpisodus::createCaptureDoc(){#ifndef __EPIBRAIN__	pCaptureDoc = new NSCaptDocument(0, pContexte) ;#endif}voidNSEpisodus::deleteCaptureDoc(){#ifndef __EPIBRAIN__	if (pCaptureDoc)	{
		delete pCaptureDoc ;
		pCaptureDoc = 0 ;
	}#endif}voidNSEpisodus::createViewData(EpisodusModel * pModel)
{
try
{
	if (pViewData)
		return ;

	NSEpisodDocument * pNewDocEpisodus = new NSEpisodDocument(0, pModel, pContexte) ;

  NSDocViewManager dvManager(pContexte) ;
  dvManager.createView(pNewDocEpisodus, "Captured screen") ;
	dvManager.createView(pNewDocEpisodus, "Captured list") ;
}
catch (...)
{
	erreur("Exception NSEpisodus::createViewData.", standardError, 0) ;
}
}


void
NSEpisodus::creeCapture(EpisodusElemData * pCapture, EpisodusElemData * pModel, NSCaptureArray * pCaptArray)
{
try
{
	if ((!pCaptArray) || (!pCapture) || (!pModel))
		return ;

#ifdef _EXT_CAPTURE
	pCaptureEngine->creeCapture(pCapture, pModel, pCaptArray) ;
#else
	if ((pModel->sCorrespondence == "") || (pCapture->sTextChild == ""))
		return ;

	string  sText = pCapture->sTextChild ;
	if (pModel->sCorrespondence == "GCONS1")
	{
		//pCaptureDoc->openView() ;
		pCaptureDoc->newCapture(pCaptArray, sText, "GCONS1") ;
		return ;
	}
	strip(sText, stripBoth) ;

	// Formatage du texte en fonction du modèle
	if (pModel->sFormat != "")
  {
    NSCaptureFormatArray CaptFmtArray(pContexte) ;
    CaptFmtArray.charger() ;
		CaptFmtArray.formatElement(&sText, pModel->sFormat) ;
  }

	//pCaptureDoc->openView() ;
	pCaptureDoc->newCapture(pCaptArray, sText, pModel->sCorrespondence, pModel->sClassifier, pModel->sUnit) ;

/*
	// Traitement des retours chariot
	size_t iCR = sText.find('\n') ;
	while (iCR != string::npos)
	{
		string sSubText = string(sText, 0, iCR) ;
		strip(sSubText, stripBoth, '\r') ;
		strip(sSubText, stripBoth) ;
		sText = string(sText, iCR+1, strlen(sText.c_str())-iCR-1) ;
		strip(sText, stripBoth, '\r') ;
		strip(sText, stripBoth) ;

		if (sSubText != "")
		{
			NSCapture * pCapt = new NSCapture(pContexte, pModel->sCorrespondence, sSubText, pModel->sClassifier, pModel->sUnit) ;
			pCaptArray->ajouter(pCapt) ;

			// Attention : la fonction "ajouter" peut supprimer pCapt s'il doublonne un élément existant
			if (pCapt && (pCapt->sClassifier != ""))
				addToSOAP(pCapt) ;
		}
		iCR = sText.find('\n') ;
	}

	if (sText != "")
	{
		NSCapture * pCapt = new NSCapture(pContexte, pModel->sCorrespondence, sText, pModel->sClassifier, pModel->sUnit) ;
		pCaptArray->ajouter(pCapt) ;

		// Attention : la fonction "ajouter" peut supprimer pCapt s'il doublonne un élément existant
		if (pCapt && (pCapt->sClassifier != ""))
			addToSOAP(pCapt) ;
	}
*/

#endif // !_EXT_CAPTURE
}
catch (...)
{
	erreur("Exception NSEpisodus::creeCapture.", standardError, 0) ;
}
}

void
NSEpisodus::createSOAPView()
{
try
{
	if (pSOAPView)
		return ;

	NSSOAPDocument * pNewDocSOAP = new NSSOAPDocument(0, pContexte) ;
	NSSOAPView * pNSSOAPView = new NSSOAPView(*pNewDocSOAP, 0, "Soaping") ;

	pNSSOAPView->setCreateWindow(NULL) ;
	pNSSOAPView->setSplittedWindow(NULL) ;

	NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pNewDocSOAP, "SOAP baskets", pNSSOAPView) ;
}
catch (...){
	erreur("Exception NSEpisodus::createSOAPView", standardError, 0) ;
}
}


void
NSEpisodus::addToSOAP(NSCapture * pNewCap)
{
try
{
  if ((!pSOAPView) && (!pSOAPTank))
    createSOAPView() ;

  NSSOAPDocument * pSOAPDoc = 0 ;
  if (pSOAPView)
    pSOAPDoc = pSOAPView->getDoc() ;
  else if (pSOAPTank)
    pSOAPDoc = pSOAPTank->getDoc() ;
  else
    return ;

  if (!pNewCap)
    return ;

  if (!pSOAPDoc)
		return ;

	SOAPObject * pSOAPObj = new SOAPObject(pNewCap) ;
	if (pSOAPObj->iConfidence == 100)
	{
		string sCtrlData = "" ;
		NVLdVTemps tpsDebut ;
		tpsDebut.takeTime() ;
		pClassifExpert->setControlString(&sCtrlData, pSOAPObj, classifExpert::niveauPreselection, 0, &tpsDebut, &tpsDebut, pSOAPObj->sCode) ;
		pClassifExpert->storeControlData(sCtrlData) ;
	}
	pSOAPDoc->addToTank(pSOAPObj) ;
}
catch (...)
{
  erreur("Exception addToSOAP(NSCapture*).", standardError, 0) ;
}
}


voidNSEpisodus::addToSOAP(SOAPObject * pNewObj){try
{
  if ((!pSOAPView) && (!pSOAPTank))
    createSOAPView() ;

  NSSOAPDocument * pSOAPDoc = 0 ;
  if (pSOAPView)
    pSOAPDoc = pSOAPView->getDoc() ;
  else if (pSOAPTank)
    pSOAPDoc = pSOAPTank->getDoc() ;
  else
    return ;

  if (!pNewObj)
    return ;

  if (pSOAPDoc)
    pSOAPDoc->addToTank(pNewObj) ;
}
catch (...)
{
  erreur("Exception addToSOAP(SOAPObject*).", standardError, 0) ;
}
}
// -----------------------------------------------------------------------------// Le SOAPing est terminé - SOAPing ended// -----------------------------------------------------------------------------
void
NSEpisodus::endingSOAP()
{
  SetEpisoState(stateEpisod) ;

  // Fermeture des SOAPview
  if ((pSOAPView != NULL) && (pSOAPView->getPaneSplitter() != NULL))
    pSOAPView->getPaneSplitter()->CmOk() ;
/*
  if (pSOAPView)
    pSOAPView->CloseWindow() ;
  if (pSOAPTank)
    pSOAPTank->CloseWindow() ;
*/

  // Fermeture des NSCaptView
  NSPatientChoisi * pPatient = pContexte->getPatient() ;
  if ((NULL == pPatient) || (NULL == pPatient->getLdvDocument()))
    return ;

  NSLdvDocument* pDocLdv = pPatient->getLdvDocument() ;

  bool bViewFound = false ;
  TView * pView = pDocLdv->GetViewList() ;
  do
  {
    NSLdvView * pLdvView = TYPESAFE_DOWNCAST(pView, NSLdvView) ;
    if (pLdvView)
      bViewFound = true ;
    pView = pDocLdv->NextView(pView) ;
  }
  while ((false == bViewFound) && pView) ;

  if (bViewFound)
  {
    pDocLdv->addFromBasketList() ;
    return ;
  }

  pPatient->LdV_show() ;
  pDocLdv->addFromBasketList() ;
}

void
NSEpisodus::appendPrometheFile()
{
#ifndef __EPIBRAIN__
  if (pBasketList->empty())
  {
    erreur("Il n'existe aucune donnée médicale à transmettre", standardError, 0) ;
    return ;
  }

  string sPatId       = "" ;
  string sPatSexe     = "" ;
  string sPatNaiss    = "" ;
  string sPatCP       = "" ;
  string sExamDate    = donne_date_duJour() ;
  string sExamCond    = "" ;
  string sExamRV      = "" ;

  // On vérifie qu'on possède toutes les informations
  NSPatientChoisi * pPatient = pContexte->getPatient() ;
  if (NULL == pPatient)
    return ;

  // Hachage ?
  sPatId = pPatient->getNss() ;

  // Sexe
  if      (pPatient->estMasculin())
    sPatSexe = "1" ;
  else if (pPatient->estFeminin())
    sPatSexe = "2" ;

  // Date de naissance - Birth date
  string sNaissance = pPatient->donneNaissance() ;
  if (string("00000000") != sNaissance)
    sPatNaiss = sNaissance ;

  // Code postal - Zip code
  if (pPatient->initGraphs())
  {
    string sVille, sPays, sZip, sCedex ;
    if (pPatient->_GraphAdr.trouveInfoVilleAdr(sVille, sPays, sZip, sCedex))
      sPatCP = sZip ;
  }

  HMODULE hModule = ::GetModuleHandle("nsmepisod.dll") ;

  if (hModule == NULL)
  {
    erreur("Erreur interne (dll introuvable)", standardError, 0) ;
    return ;
  }

  TModule * pEpiModule = new TModule("", hModule) ;

  PrometheComplementDlg * pPrometDlg = new PrometheComplementDlg(pContexte->GetMainWindow(), pContexte, &sPatNaiss, &sPatCP, &sExamCond, &sExamRV, pEpiModule) ;
  int iRetour = pPrometDlg->Execute() ;
  delete pPrometDlg ;
  if (iRetour != IDOK)
    return ;

  ofstream    outFile ;
  string      filecontenu = "" ;
  // char        line[1024] ;
  // char        buffer[1024] ;

  // Ouverture du fichier promeXXX.msg - Opening file promeXXX.msg
  // Fichier positionné sur le répertoire global afin de permettre de
  // changer de machines.
  // File is open on the global directory, so that the user can use different
  // machines.

	string sFichier = string("prome") + pContexte->getUtilisateur()->getNss() + string(".msg") ;

  sFichier = pContexte->PathName("BGLO") + sFichier ;
  outFile.open(sFichier.c_str(), ios::ate | ios::app) ;

  if (!outFile)
  {
    string sErrMess = string("Erreur d'ouverture en écriture du fichier ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return ;
  }

  // Identifiant patient
  string sBalise ;

  // Patient
  outFile << (string("<pmed_patient patientid=\"") + sPatId + string("\" >") + string("\n")) ;

  // Données administratives
  sBalise = string("<pmed_admindata") ;
  if (sPatSexe != "")
    sBalise += string(" sexe=\"") + sPatSexe + string("\"") ;
  if (sPatNaiss != "")
    sBalise += string(" naissance=\"") + string(sPatNaiss, 0, 4) + string("-") + string(sPatNaiss, 4, 2) + string("\"") ;
  if (sPatCP != "")
    sBalise += string(" code_post=\"") + sPatCP + string("\"") ;
  sBalise += string("/>") ;
  outFile << (sBalise + string("\n")) ;

  // Données médicales
  outFile << (string("<pmed_healthdata>") + string("\n")) ;

  // Contact
  sBalise = string("<pmed_contact") ;
  if (sExamDate != "")
    sBalise += string(" date=\"") + string(sExamDate, 0, 4) + string("-") + string(sExamDate, 4, 2) + string("-") + string(sExamDate, 6, 2) + string("\"") ;
  sBalise += string(" type_principe=\"SOAP\"") ;
  sBalise += string(">") ;
  outFile << (sBalise + string("\n")) ;

  // Contact type
  sBalise = string("<pmed_contact_type") ;
  if (sExamCond != "")
    sBalise += string(" conditions=\"") + sExamCond + string("\"") ;
  if (sExamRV != "")
    sBalise += string(" rendez_vous=\"") + sExamRV + string("\"") ;
  sBalise += string("/>") ;
  outFile << (sBalise + string("\n")) ;

  // Eléments de contact
  for (SOAPBasketArrayIter baskIter = pBasketList->begin() ; pBasketList->end() != baskIter ; baskIter++)
  {
    SOAPBasket * pBasket = *baskIter ;
    if (false == pBasket->empty())
    {
      // Elément de contact
      sBalise = string("<pmed_contactelement") ;
      sBalise += string(">") ;
      outFile << (sBalise + string("\n")) ;

      for (SOAPBasketIter objsIter = pBasket->begin() ; pBasket->end() != objsIter ; objsIter++)
      {
        SOAPObjectArray * pObjs = *objsIter ;

        string sCase = pObjs->pPrincipe->sCase ;
        string sCaseSens ;
        pContexte->getDico()->donneCodeSens(&sCase, &sCaseSens) ;
        string sCasePromet = "" ;
        if      (sCaseSens == "0MOTI")
          sCasePromet = "M" ;
        else if (sCaseSens == "0SOA0")
          sCasePromet = "S" ;
        else if (sCaseSens == "0SOA2")
          sCasePromet = "O" ;
        else if (sCaseSens == "0SOA5")
          sCasePromet = "1" ;
        else if (sCaseSens == "0SOA4")
          sCasePromet = "A" ;
        else if (sCaseSens == "0SOA3")
          sCasePromet = "2" ;
        else if (sCaseSens == "0SOPR")
          sCasePromet = "2F" ;
        else if (sCaseSens == "0SOPP")
          sCasePromet = "2P" ;

        if ((string("") != sCasePromet) && (false == pObjs->empty()))
        {
          for (ArraySOAPObjectIter Iter = pObjs->begin() ; pObjs->end() != Iter ; Iter++)
          {
            string sClassif = (*Iter)->sClassification;            pContexte->getDico()->donneCodeSens(&sClassif, &sCaseSens) ;
            string sClassifPromet = "" ;
            if      (sCaseSens == "6CISP")
              sClassifPromet = "CISP2" ;
            else if (sCaseSens == "6CIMA")
              sClassifPromet = "CIM10" ;
            else if (sCaseSens == "6CI10")
              sClassifPromet = "CDAM" ;

            string sCode = (*Iter)->sCode ;

            // Vrai code
            if ((sClassifPromet != "") && (sCode != ""))
            {
              // Code
              sBalise = string("<pmed_contactcode") ;
              if (sCasePromet != "")
                sBalise += string(" position=\"") + sCasePromet + string("\"") ;
              if (sClassifPromet != "")
                sBalise += string(" classification=\"") + sClassifPromet + string("\"") ;
              if (sCode != "")
                sBalise += string(" code=\"") + sCode + string("\"") ;
              sBalise += string("/>") ;
              outFile << (sBalise + string("\n")) ;
            }
            else if ((*Iter)->sNoeud != "")
            {
              // Noeud d'arbre
              NSPatPathoArray PPt(pContexte->getSuperviseur()) ;
              string          sDateDoc     = string("") ;

              // On récupère l'arbre
              string sDocId = string((*Iter)->sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
              NSPatientChoisi* pPat = pContexte->getPatient() ;

              if (pPat->getDocHis()->donneArbre(sDocId, &PPt, sDateDoc))
              {
                PatPathoIter pptIt = PPt.ChercherNoeud((*Iter)->sNoeud) ;

                NSPatPathoArray Tree(pContexte->getSuperviseur()) ;
                PPt.ExtrairePatPathoWithRoot(pptIt, &Tree) ;                sBalise = string("<pmed_contacttree") ;
                if (sCasePromet != "")
                  sBalise += string(" position=\"") + sCasePromet + string("\"") ;
                sBalise += string(">") ;
                outFile << (sBalise + string("\n")) ;

                if (false == Tree.empty())
                	for (PatPathoIter itTree = Tree.begin() ; Tree.end() != itTree ; itTree++)
                	{
                  	sBalise = string("<pmed_contactnode") ;

                  	if ((*itTree)->getLocalisation() != "")
                    	sBalise += string(" codeloc=\"") + (*itTree)->getLocalisation() + string("\"") ;
                  	if ((*itTree)->getLexique() != "")
                      sBalise += string(" lexique=\"") + (*itTree)->getLexique() + string("\"") ;
                  	if ((*itTree)->getComplement() != "")
                      sBalise += string(" complement=\"") + (*itTree)->getComplement() + string("\"") ;
                  	if ((*itTree)->getCertitude() != "")
                      sBalise += string(" certitude=\"") + (*itTree)->getCertitude() + string("\"") ;
                  	if ((*itTree)->getPluriel() != "")
                      sBalise += string(" pluriel=\"") + (*itTree)->getPluriel() + string("\"") ;
                  	if ((*itTree)->getUnit() != "")
                      sBalise += string(" unit=\"") + (*itTree)->getUnit() + string("\"") ;
                  	sBalise += string("/>");
                  	outFile << (sBalise + string("\n")) ;
                	}

                sBalise = string("</pmed_contacttree>");
                outFile << (sBalise + string("\n")) ;
              }
            }
          }
        }
      }
      outFile << ( string("</pmed_contactelement>") + string("\n") ) ;
    }
  }
  outFile << (string("</pmed_contact>")     + string("\n")) ;
  outFile << (string("</pmed_healthdata>")  + string("\n")) ;
  outFile << (string("</pmed_patient>")     + string("\n")) ;
  outFile.close() ;
  iEpisoState = stateFinished ;
#endif
}

#ifndef __EPIBRAIN__
boolNSEpisodus::sendPrometheMessage()
{
try
{
  NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
  if ((NSUtilisateurChoisi*) NULL == pUser)
    return false ;

	string sFileName = pContexte->PathName("BGLO") + string("prome") + pUser->getNss() + string(".msg") ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;

  if (!inFile)
  {
    string sErrMess = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrMess += string(" (") + sFileName + string(")") ;
    erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sData = string("") ;

  while (!inFile.eof())
  {
    string sLine ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sData += sLine + string(1, '\n') ;
  }
  inFile.close() ;

  if (string("") == sData)
  {
    erreur("Il n'y a pas de données à envoyer.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return true ;
  }

  // Ajout des balises de début et de fin de message
  string sBaliseDeb = string("<pmed_message senderid=\"") + sPrometheUserId + string("\">") + string("\n") ;
  string sBaliseFin = string("</pmed_message>") + string("\n") ;
  sData = sBaliseDeb + sData + sBaliseFin ;

  // récupération des paramètres de emailXXX.dat
  if (false == pContexte->getUtilisateur()->initMail())
  {
    erreur("Envoi des e-mails impossible.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sCaption = string("Message ") + pSuper->getAppName().c_str() ;
  string sMessage = string("Vous devez être connectés à internet pour que ") + pContexte->getSuperviseur()->getAppName().c_str() + string(" envoie vos e-mails automatiquement. Etes-vous connecté en ce moment ?") ;
  int idRet = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;

  if (idRet != IDYES)
    return false ;

  bool bSuccess ;

  NSMailParams* pMail = pUser->getRegularMail() ;

  if (NSMAIL_MAPI == pMail->getSendingModality())
  {
    NSMapiCtrl *pMapi = new NSMapiCtrl(pContexte, 0, pMail) ;
    pMapi->Connect() ;
    if (false == pMapi->Verify())
    {
      delete pMapi ;
      return false ;
    }

    pMapi->ComposeMessage(pMail->getSender(), sPrometheMail, "Message", sData) ;
    ULONG lResult = pMapi->SendMail() ;
    pMapi->Disconnect() ;
    delete pMapi ;

    if (SUCCESS_SUCCESS == lResult)
      bSuccess = true ;
    else
      bSuccess = false ;
  }
  else
  {
    TIndySMTP * pXSMTP = new TIndySMTP(NULL, pContexte, 0, pMail) ;
    pXSMTP->Show() ;
    pXSMTP->Connect() ;
    if (false == pXSMTP->IsConnected())
    {
      pXSMTP->Close() ;
      delete pXSMTP ;
      return false ;
    }

    pXSMTP->ComposeSimpleMessage(pMail->getSender(), pMail->getName(),
                                 pMail->getFrom(), pMail->getReplyTo(),
                                 sPrometheMail, string("Message"), sData) ;
    pXSMTP->SendMail() ;

    while (pXSMTP->Attendre())
      ;

    if (pXSMTP->Continuer())
      bSuccess = true ;
    else
      bSuccess = false ;

    pXSMTP->Disconnect() ;
    pXSMTP->Close() ;
    delete pXSMTP ;
  }

  sCaption = string("Message ") + pSuper->getAppName() ;
  if (bSuccess)
  {
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail terminé.", sCaption.c_str(), MB_OK) ;
    // vider le fichier
    NsDeleteTemporaryFile(pSuper, sFileName) ;
  }
  else
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail interrompu.", sCaption.c_str(), MB_OK) ;

  return bSuccess ;
}
/*
catch (ESockError& e)
{
  string sMessage = string("Connexion impossible : ") + string(e.Message.c_str()) ;
  erreur(sMessage.c_str(), standardError, 0) ;
  return false ;
}
catch (ESocketError& e)
{
  erreur("Connexion impossible.", standardError, 0) ;
  return false ;
}*/catch (...)
{
  erreur("Le message n'a pu être envoyé.", standardError, 0) ;
  return false ;
}
}
#endif // !__EPIBRAIN__

voidNSEpisodus::reveilleToi()
{
  // Pointeur sur la fenêtre principale
  TWindow * pMainWin = pContexte->getSuperviseur()->donneMainWindow() ;

  WINDOWPLACEMENT placement ;
  placement.length = sizeof(WINDOWPLACEMENT) ;

  if (pMainWin->GetWindowPlacement(&placement))
  {
    if (placement.showCmd == SW_SHOWMINIMIZED)
    {
      placement.showCmd = SW_RESTORE ;
      pMainWin->SetWindowPlacement(&placement) ;
    }
  }
  pMainWin->BringWindowToTop() ;
}


#ifndef _EXT_CAPTURE
// -----------------------------------------------------------------------------
// fonction d'énumération
// -----------------------------------------------------------------------------
bool FAR PASCAL _export
NSEpisodus::SearchChildWindow(HWND hWnd, LPARAM lParam)
{
  char    myClassMessage[1024] ;
  char    myTextMessage[1024] ;
  TWindow::THandle mainWindow = ((MylParam *)(lParam))->theMainWindow ;

  //  Ne surtout pas faire de return false, sinon l'énumération se terminera avant la fin de l'énumération.
  //  Never return false, or the enumeration will end before its finished.
  if (!(GetWindowLong(hWnd, GWL_HWNDPARENT) == (long)((MylParam *)(lParam))->AppWindow))
    return true ;

  NSEpisodus * pEpisod = ((MylParam *)(lParam))->pContexte->getEpisodus() ;

  // "Class name"
  GetClassName(hWnd, myClassMessage, sizeof(myClassMessage)) ;

  // "Text"
  SendMessage(hWnd, WM_GETTEXT, sizeof(myTextMessage), LPARAM(&myTextMessage)) ;

  // Coordonnées du contrôle par rapport à l'écran
  TWindow myWindow(hWnd) ;
  ClassLib::TRect ScreenCoords = myWindow.GetWindowRect() ;

  // Coordonnées de la fenêtre par rapport à l'écran
  TWindow theMainWindow((HWND) mainWindow) ;
  ClassLib::TRect mainWindowCoords = theMainWindow.GetWindowRect() ;

  // Calcul des coordonnées du contrôle par rapport à la fenêtre
  ScreenCoords.left   = ScreenCoords.left - mainWindowCoords.left ;
  ScreenCoords.top    = ScreenCoords.top - mainWindowCoords.top ;
  ScreenCoords.right  = ScreenCoords.right - mainWindowCoords.left ;
  ScreenCoords.bottom = ScreenCoords.bottom - mainWindowCoords.top ;

  // propriétés de la fenêtre fille
  EpisodusElemData * myData = new EpisodusElemData(mainWindow, (TWindow::THandle) hWnd, string(myClassMessage), string(myTextMessage), ScreenCoords, -1) ;

  // ajout des propriétés dans la liste
  pEpisod->pEpisodusData->push_back(myData) ;

//  Log
//  char    sMyMessage[1024];         // log
//  sprintf(sMyMessage, "AppWindow : <%d>\t- AppWndChild   : <%x>\t- AppClassChild : <%s>\t- AppTextChild  : <%s>\n", mainWindow, hWnd, myClassMessage, myTextMessage) ;
//  pEpisod->logSpyMessage(sMyMessage) ;

  return true ;
}

// -----------------------------------------------------------------------------
// Get content of a ListView control
// -----------------------------------------------------------------------------
//
// From http://www.piotrkaluski.com/files/winguitest/docs/ch02s04.html :
//
// If you look at SendMessage documentation you will read that for messages with id below WM_USER (which is 1024) Win32 will do a marshalling.
// For other messages you have to take care of it yourself. What is marshalling? Marshalling means sending data between processes.
// Not only bytes, but also objects, structures. Marshalling is not a technique. It is a notion of sending data.
// As we already know, SendMessage often expects lParam to be a pointer. But physically, lParam is only a long number.
// If you allocate a memory in process A, allocating function will return you the pointer to this memory in the address space of the calling process (i.e. process A).
// That's really important to understand. Remember that in Win32 there is no such thing like a global, universal memory addressing method, which would allow process to refer to any place of physical memory (or at least this mechanism is not promptly provided - I don't know. Waiting for your feedback).
// Each pointer is valid only in address space of one process. So if process A calls SendMessage on window owned by process B and pass a pointer to a memory, this pointer will not be valid in the address space of a process B, which hosts ListView or any other control.
//

#define MAX_LVMSTRING 255

std::string GetListViewItemText(HWND hWnd)
{
  LPVOID lpRemoteBuffer    = (LPVOID) 0 ;
  LPVOID lpStrBufferMemory = (LPVOID) 0 ;
  HANDLE hProcess          = (HANDLE) 0 ;

  std::string sResult = std::string("") ;

try
{
  LRESULT iNbItems = SendMessage(hWnd, LVM_GETITEMCOUNT, WPARAM(0), LPARAM(0)) ;
  if (0 == iNbItems)
    return string("") ;

  int iColumnCount = 0 ;

  // Get header
  //
  HWND hHeader = (HWND) SendMessage(hWnd, LVM_GETHEADER, WPARAM(0), LPARAM(0)) ;
  if (0 != hHeader)
  {
    LRESULT lItemCount = SendMessage(hHeader, HDM_GETITEMCOUNT, WPARAM(0), LPARAM(0)) ;
    iColumnCount = (int) lItemCount ;
  }

  // Get items content
  //
  LV_ITEM lvItem ;

  // Get the process id owning the window
  //
  DWORD dwProcessID = 0 ;
  DWORD threadId = GetWindowThreadProcessId(hWnd, &dwProcessID) ;
  if ((DWORD(0) == threadId) || (0 == dwProcessID))
    // throw new ArgumentException("hWnd");
    return string("") ;

  // Open the process with all access
  HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessID) ;
  if (HANDLE(0) == hProcess)
    // throw new ApplicationException("Failed to access process");
    return string("") ;

  // Allocate a buffer in the remote process for the text
  //
  lpStrBufferMemory = VirtualAllocEx(hProcess, 0, MAX_LVMSTRING, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) ;
  if ((LPVOID) 0 == lpStrBufferMemory)
  {
    CloseHandle(hProcess) ;
    return string("") ;
  }

  // Allocate a buffer in the remote process for LV_ITEM
  //
  lpRemoteBuffer = VirtualAllocEx(hProcess, (LPVOID) 0, sizeof(LV_ITEM), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE) ;
  if ((LPVOID) 0 == lpRemoteBuffer)
  {
    // throw new SystemException("Failed to allocate memory in remote process");
    VirtualFreeEx(hProcess, lpStrBufferMemory, 0, MEM_RELEASE) ;
    CloseHandle(hProcess) ;
    return string("") ;
  }

  for (int iItem = 0 ; iItem < iNbItems ; iItem++)
  {
    if (iItem > 0)
      sResult += "$|$" ;

    for (int iCol = 0 ; iCol < iColumnCount ; iCol++)
    {
      if (iCol > 0)
        sResult += "$/$" ;

      // Fill in the LVITEM struct, this is in your own process
      // Set the pszText member to somewhere in the remote buffer,
      // For the example I used the address imediately following the LVITEM stuct
      //

      lvItem.mask       = LVIF_TEXT ;
      lvItem.iItem      = iItem ;  // items are zero based
      lvItem.iSubItem   = iCol ;   // columns are one based but zero gets item 
      lvItem.state      = 0 ;
      lvItem.stateMask  = 0 ;
      lvItem.pszText    = (char *) lpRemoteBuffer ;
      lvItem.cchTextMax = MAX_LVMSTRING ;
      lvItem.iImage     = 0 ;
      lvItem.lParam     = 0 ;
      lvItem.iIndent    = 0 ;

      // Copy the local LVITEM to the remote buffer
      //
      // lpRemoteBuffer is invalid in address space of our process.
      // But it is valid in the address space of the process which hosts the control we interact with.
      // This is the reason why we need to use WriteProcessMemory
      //
      DWORD copied = 0 ;
      bool bSuccess = WriteProcessMemory(hProcess, lpRemoteBuffer, &lvItem, sizeof(LV_ITEM), &copied) ;
      if (false == bSuccess)
      {
        // throw new SystemException("Failed to write to process memory");
        VirtualFreeEx(hProcess, lpStrBufferMemory, 0, MEM_RELEASE) ;
        VirtualFreeEx(hProcess, lpRemoteBuffer, 0, MEM_RELEASE) ;
        CloseHandle(hProcess) ;
      }

      // Send the message to the remote window with the address of the remote buffer
      //
      LRESULT iTxtSize = SendMessage(hWnd, LVM_GETITEMTEXT, WPARAM(iItem), LPARAM(lpRemoteBuffer)) ;

      if (iTxtSize > 0)
      {
//        wchar_t myTextMessage[MAX_LVMSTRING] ;
        char myTextMessage[MAX_LVMSTRING] ;
        bSuccess = ReadProcessMemory(hProcess, lpStrBufferMemory, myTextMessage, MAX_LVMSTRING, &copied) ;

        if (bSuccess)
        {
/*
          ostringstream stm ;
          const ctype<char>& ctfacet = use_facet< ctype<char> >( stm.getloc() ) ;
          for (size_t i = 0 ; i < wcslen(myTextMessage) ; ++i)
            stm << ctfacet.narrow(myTextMessage[i], 0) ;
          sResult += std::string(stm.str()) ;
*/
          sResult += std::string(myTextMessage) ;
        }
      }
    }
  }

  if (lpRemoteBuffer)
    VirtualFreeEx(hProcess, lpRemoteBuffer, 0, MEM_RELEASE) ;
  if (lpStrBufferMemory)
    VirtualFreeEx(hProcess, lpStrBufferMemory, 0, MEM_RELEASE) ;
  if (hProcess)
    CloseHandle(hProcess) ;

  return sResult ;
}
catch (...)
{
	erreur("Exception GetListViewItemText.", standardError, 0) ;

  if (lpRemoteBuffer)
    VirtualFreeEx(hProcess, lpRemoteBuffer, 0, MEM_RELEASE) ;
  if (lpStrBufferMemory)
    VirtualFreeEx(hProcess, lpStrBufferMemory, 0, MEM_RELEASE) ;
  if (hProcess)
    CloseHandle(hProcess) ;

  return sResult ;
}
}

// -----------------------------------------------------------------------------// fonction d'énumération récursive -- lance la même fonction sur tous ses fils (s'ils existent)// -----------------------------------------------------------------------------
bool FAR PASCAL _export
NSEpisodus::SearchChildWindowRecur(HWND hWnd, LPARAM lParam)
{
  char    myClassMessage[1024];
  char    myTextMessage[1024] ;
  TWindow::THandle mainWindow = ((MylParam *)(lParam))->theMainWindow ;

  //  Ne surtout pas faire de return false, sinon l'énumération se terminera avant la fin de l'énumération
  //  Never return false, or the enumeration will end before its finished
  if (!(GetWindowLong(hWnd, GWL_HWNDPARENT) == (long) ((MylParam *) (lParam))->AppWindow))
    return true ;

  NSEpisodus * pEpisod = ((MylParam *)(lParam))->pContexte->getEpisodus() ;

  // "Class name"
  GetClassName(hWnd, myClassMessage, sizeof(myClassMessage)) ;

  std::string sText = std::string("") ;

  if (0 == strcmp(myClassMessage, "SysListView32"))
    sText = GetListViewItemText(hWnd) ;
  else
  {
    // "Text"
    SendMessage(hWnd, WM_GETTEXT, sizeof(myTextMessage), LPARAM(&myTextMessage)) ;
    sText = std::string(myTextMessage) ;
  }

  // Coordonnées du contrôle par rapport à l'écran
  TWindow myWindow(hWnd) ;
  ClassLib::TRect ScreenCoords = myWindow.GetWindowRect() ;

  // Coordonnées de la fenêtre par rapport à l'écran
  TWindow theMainWindow((HWND) mainWindow) ;
  ClassLib::TRect mainWindowCoords = theMainWindow.GetWindowRect() ;

  // Calcul des coordonnées du contrôle par rapport à la fenêtre principale sélectionnée
  ScreenCoords.left   = ScreenCoords.left - mainWindowCoords.left ;
  ScreenCoords.top    = ScreenCoords.top - mainWindowCoords.top ;
  ScreenCoords.right  = ScreenCoords.right - mainWindowCoords.left ;
  ScreenCoords.bottom = ScreenCoords.bottom - mainWindowCoords.top ;

  // log
//  char    sMyMessage[1024] ;
//  sprintf(sMyMessage, "AppWindow : <%d>\t- AppWndChild   : <%x>\t- AppClassChild : <%s>\t- AppTextChild  : <%s>\n", mainWindow, hWnd, myClassMessage, myTextMessage) ;
//  pEpisod->logSpyMessage(sMyMessage) ;

  EpisodusElemData * myData = new EpisodusElemData(((MylParam *)(lParam))->AppWindow, (TWindow::THandle) hWnd, myClassMessage, sText, ScreenCoords, -1) ;
  pEpisod->pEpisodusData->push_back(myData) ;

  if (GetWindow(hWnd, GW_CHILD))  // sert à savoir si la "fenetre" a des fenetres filles
//  if (hWnd)                       // sert à savoir si la "fenêtre" existe
  {
    WNDENUMPROC lpEnumChildWindows = (WNDENUMPROC)MakeProcInstance((FARPROC) NSEpisodus::SearchChildWindowRecur, (HINSTANCE) hWnd) ;

    MylParam * pNewlParam = new MylParam((TWindow::THandle) hWnd, mainWindow, ((MylParam *)(lParam))->pContexte) ;
    EnumChildWindows(hWnd, lpEnumChildWindows, LPARAM(pNewlParam)) ;
    delete pNewlParam ;
  }

  return true;
}

#endif // !_EXT_CAPTURE


#ifndef _EXT_CAPTURE
// -----------------------------------------------------------------------------
// ancienne version de capture - il y a des bouts de code qui permette d'énumérer// tous les process qui tourne sur la machine ainsi que leurs propriétés
// -----------------------------------------------------------------------------
void
NSEpisodus::CaptureOld(HWND HWnd)
{
  char            sMyMessage[1024] ;
  char            mymessage[1024] ;
  WNDENUMPROC     lpEnumChildWindows ;

  sprintf(sMyMessage, "Capture launched\n") ;
  logSpyMessage(sMyMessage) ;

  HWND AppWindow = GetForegroundWindow() ; // same result as WinDowse - Window - parent handle
  sprintf(sMyMessage, "AppWindow : <%d>\n", AppWindow) ;
  logSpyMessage(sMyMessage) ;

  MylParam * pSpylParam = new MylParam((TWindow::THandle) AppWindow, (TWindow::THandle) AppWindow, pContexte) ;

  lpEnumChildWindows = (WNDENUMPROC)MakeProcInstance((FARPROC)NSEpisodus::SearchChildWindow, (HINSTANCE)hInstance) ;
  EnumChildWindows(AppWindow, lpEnumChildWindows, LPARAM(pSpylParam)) ;
  GetClassName(AppWindow, mymessage, sizeof(mymessage)) ;
  SendMessage(AppWindow, WM_GETTEXT, sizeof(mymessage), (LPARAM(&mymessage))) ;
  delete pSpylParam  ;

  // Actualisation de la liste à l'écran
/*
  if (pViewData)
    pViewData->AfficheListe(pSpyData) ;
*/
  return ;
}



// -----------------------------------------------------------------------------
// lance le Hook de la souris
// -----------------------------------------------------------------------------
void
NSEpisodus::lanceHook()
{
  // HWND    hWndNautilus = pContexte->getSuperviseur()->donneMainWindow()->HWindow ;
  char    sTemp[1024] ;
  sprintf(sTemp, "%s", (pContexte->getSuperviseur()->donneMainWindow())->Title) ;
  logSpyMessage(sTemp) ;
  // InstalleHook(sTemp) ;
}


// -----------------------------------------------------------------------------
// arrête le Hook de la souris
// -----------------------------------------------------------------------------
void
NSEpisodus::fermeHook()
{
	// LibereHook() ;
}
#endif // !_EXT_CAPTURE


// -----------------------------------------------------------------------------
// exporte les modèles
// -----------------------------------------------------------------------------
bool
NSEpisodus::ExportModels()
{
	string sFichier = string("model.xml") ;

#ifdef _EXT_CAPTURE
	return pCaptureEngine->ExportModels(sFichier) ;
#else

  ofstream outFile ;
  bool bFileOpened = pContexte->getUtilisateur()->OpenUserFile(&outFile, sFichier, string("model[user].xml"), pContexte->PathName("FGLO")) ;

  // outFile.open(sFichier.c_str(), ios::out) ;
  if (!bFileOpened || !outFile)
  {
  	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return false ;
  }

  string sBaliseDeb = string("<nscapturemodel>") ;
  string sBaliseFin = string("</nscapturemodel>") ;
  sBaliseDeb += string("\n") ;
  outFile.write(sBaliseDeb.c_str(), strlen(sBaliseDeb.c_str())) ;
  bool bGlobalReturn = true ;

  if (!(pModels->empty()))
  {
    if (!(pModels->sauvegarder(&outFile, "model")))
      bGlobalReturn = false ;
  }

  if (!(pSubModels->empty()))
  {
    if (!(pSubModels->sauvegarder(&outFile, "submodel")))
      bGlobalReturn = false ;
  }

  sBaliseFin += string("\n") ;
  outFile.write(sBaliseFin.c_str(), strlen(sBaliseFin.c_str())) ;
  outFile.close() ;

  return bGlobalReturn  ;
#endif // !_EXT_CAPTURE
}


#ifndef __EPIBRAIN__
void
NSEpisodus::UserChanged()
{
	PatChanged() ;

#ifndef _EXT_CAPTURE
	pModels->vider() ;
	ImportModels(false) ;
#else
	pCaptureEngine->pModels->vider() ;
	ImportModels(false) ;
#endif

  TMyApp * pApp = pContexte->getSuperviseur()->getApplication() ;

  // unregister hotkey shortcut
  Word        Key ;
  Word        Mods = 0 ;
  TShiftState Shift ;
  Mods = 0 ;
  ShortCutToKey(pApp->iCaptureHotKey, Key, Shift) ;
  Mods = Mods | MOD_CONTROL ;
  UnregisterHotKey(NULL, pApp->iCaptureHotKey) ;
  Mods = 0 ;
  ShortCutToKey(pApp->iBabylonHotKey, Key, Shift) ;
  Mods = Mods | MOD_SHIFT ;
  UnregisterHotKey(NULL, pApp->iBabylonHotKey) ;

  // chargement des paramètres personnels de l'utilisateur
  loadParams() ;
  loadEpiParams() ;

  pContexte->resetBdmDriver() ;

  // register hotkey shortcut - add by fab  if ((sCaptureKey != "") || (sBabylonKey != ""))
  {
    // create the shortcut for the capture (shortcut : [CTRL] + <Espace>
    Mods = 0 ;
    TShortCut myShortCut = ShortCut(Word(sCaptureKey[0]), TShiftState() << ssCtrl) ;
    ShortCutToKey(myShortCut, Key, Shift) ;
    Mods = Mods | MOD_CONTROL ;
    RegisterHotKey(NULL, myShortCut, Mods, Key) ;
    pApp->iCaptureHotKey = myShortCut ;
    // create the shortcut for the capture (shortcut : [SHIFT] + <Espace>
    Mods = 0 ;
    TShortCut myShortCut2 = ShortCut(Word(sBabylonKey[0]), TShiftState() << ssShift) ;
    ShortCutToKey(myShortCut, Key, Shift) ;
    Mods = Mods | MOD_SHIFT ;
    RegisterHotKey(NULL, myShortCut2, Mods, Key) ;
    pApp->iBabylonHotKey = myShortCut2 ;
  }

  pPrincipes->vider() ;
  chargerPrincipes() ;
}
#endif // !__EPIBRAIN__


void
NSEpisodus::PatChanged()
{
  // Si un codage a eu lieu, et n'a pas donné lieu à constitution d'un message,
  // on propose la création d'un message.
  // If the user classified datas, but did not elaborate a message, we ask him
  // if he wants to do it now.
  if ((sPrometheUserId != "") && (iEpisoState > stateSOAPing) && (iEpisoState < stateFinished))
  {
    TWindow * pMainWin = pContexte->getSuperviseur()->donneMainWindow() ;
    int retVal = MessageBox(pMainWin->GetHandle(), "Voulez vous élaborer un message pour le serveur d'épidémiologie ?", "Episodus", MB_YESNO) ;
    if (IDYES == retVal)
      appendPrometheFile() ;
  }
  pBasketList->vider() ;

  /*
  bool bAdminData = false ;
  if (!CaptureArray.empty())
  {
    for (CaptureIter captIter = CaptureArray.begin() ; captIter != CaptureArray.end() ; captIter++)
    {
      if ((*captIter)->sChemin.find("ZADMI") != string::npos)
      {
        bAdminData = true ;
        break ;
      }
    }
  }

  if (!bAdminData)
    CaptureArray.vider() ;
  */

  // On vide le réservoir de capture général, on lui ajoute les données
  // nouvellement capturées (qui ont suscitées le changement de patient).
  // We empty the general capture tank, then we add the newly captured datas
  // (they may have caused the patient changing).
  //
  // nb : the same operation is done in ChercheListePatDialog::CmCreer()
  //      in the case there is no former patient open before creation

	// CaptureArray.vider() ;
	// CaptureArray.append(&(newCaptureArray)) ;
	// newCaptureArray.vider() ;

#ifndef __EPIBRAIN__
  pCaptureDoc->PatChanged() ;
#endif
  PutNewCaptureInTank(true /*bEmptyTankFirst*/, true /*bool bResetNew*/) ;

	iEpisoState = state0 ;
}


// -----------------------------------------------------------------------------
// récupère les modèles stockés dans le fichier XML
// -----------------------------------------------------------------------------
bool
NSEpisodus::ImportModels(bool verbose)
{
try
{
	string sFichier = string("model.xml") ;

#ifdef _EXT_CAPTURE
	return pCaptureEngine->ImportModels(sFichier, verbose) ;
#else

	if (!(pContexte->getUtilisateur()))
		return false ;

	ifstream inFile ;
  bool bFileOpened = pContexte->getUtilisateur()->OpenUserFile(&inFile, sFichier, string("model[user].xml"), pContexte->PathName("FGLO")) ;
  if (!bFileOpened || !inFile)
  {
    if (verbose)
    {
    	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
      erreur(sErrMess.c_str(), standardError, 0) ;
    }
    return false ;
  }

  char buffer[1024] ;
  int  buf_size = 1024 ;

	string sFilecontenu = "" ;
  while (inFile.getline(buffer, buf_size))    sFilecontenu += buffer ;
  inFile.close() ;

  string sBaliseDeb = string("<nscapturemodel>") ;
  string sBaliseFin = string("</nscapturemodel>") ;

  // Suppression des balises racines <nscapturemodel> et </nscapturemodel>
  size_t posDeb = sFilecontenu.find(sBaliseDeb) ;
  size_t posFin = sFilecontenu.find(sBaliseFin) ;

  if ((posDeb == string::npos) || (posFin == string::npos) || (posFin < posDeb))
  {
    string sErreur = string("Le fichier des modèles de capture est défectueux") ;
    if      (posDeb == string::npos)
      sErreur += string(" (pas de balise de début)") ;
    else if (posFin == string::npos)
      sErreur += string(" (pas de balise de fin)") ;
    else if (posFin < posDeb)
      sErreur += string(" (balise de fin avant la balise de début)") ;

    sErreur += string(" - fichier ") + sFichier ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  size_t vraiPosDeb = posDeb + strlen(sBaliseDeb.c_str()) ;
  sFilecontenu = string(sFilecontenu, vraiPosDeb, posFin - vraiPosDeb) ;

  // création de la liste des modèles dans NSEpisodus (pModels)
  EpisodusModelXML ModelsXML ;
  ModelsXML.ImportModelXML(sFilecontenu) ;

  if (!ModelsXML.sArrayBalise.empty() && !ModelsXML.sArrayBaliseContenu.empty())
  {
    vector<string>::iterator XMLModelBaliseIter = ModelsXML.sArrayBalise.begin() ;
    vector<string>::iterator XMLModelBaliseContenuIter = ModelsXML.sArrayBaliseContenu.begin() ;
    vector<string>::iterator XMLModelBaliseVariablesIter = ModelsXML.sArrayBaliseVariables.begin() ;

    for ( ; XMLModelBaliseIter != ModelsXML.sArrayBalise.end() ; XMLModelBaliseIter++)
    {
      if (XMLModelBaliseContenuIter == ModelsXML.sArrayBaliseContenu.end())
        return false ;

      if      (*XMLModelBaliseIter == "model")
        pModels->push_back(new EpisodusModel(*XMLModelBaliseContenuIter, *XMLModelBaliseVariablesIter)) ;
      else if (*XMLModelBaliseIter == "submodel")
        pSubModels->push_back(new EpisodusModel(*XMLModelBaliseContenuIter, *XMLModelBaliseVariablesIter)) ;

      XMLModelBaliseContenuIter++ ;
      XMLModelBaliseVariablesIter++ ;
    }
    return true ;
  }
  return false ;
#endif // !_EXT_CAPTURE
}
catch (...)
{
	erreur("Exception NSEpisodus::ImportModels.", standardError, 0) ;
	return false ;
}
}


#ifndef _EXT_CAPTURE

void
NSEpisodus::SetPhareOn()
{
/*
  TMyApp* pMyApp = pContexte->getSuperviseur()->pNSApplication ;
  pMyApp->FlushEpisodusControlBar() ;
  pMyApp->cb3->Insert(*new TButtonGadget(CM_EPISO_STOP, CM_EPISO_STOP, TButtonGadget::Command)) ;
  pMyApp->cb3->Insert(*new TButtonGadget(CM_EPISO_LEARN, CM_EPISO_LEARN, TButtonGadget::Command)) ;
  pMyApp->cb3->Insert(*new TButtonGadget(IDC_EPISOD_PHARE_ON, IDC_EPISOD_PHARE_ON, TButtonGadget::Command, TButtonGadget::Indeterminate)) ;
  pMyApp->cb3->LayoutSession() ;
*/
}


void
NSEpisodus::SetPhareOff()
{
/*
  TMyApp* pMyApp = pContexte->getSuperviseur()->pNSApplication ;
  pMyApp->FlushEpisodusControlBar() ;
  pMyApp->cb3->Insert(*new TButtonGadget(CM_EPISO_STOP, CM_EPISO_STOP, TButtonGadget::Command)) ;
  pMyApp->cb3->Insert(*new TButtonGadget(CM_EPISO_LEARN, CM_EPISO_LEARN, TButtonGadget::Command)) ;
  pMyApp->cb3->Insert(*new TButtonGadget(IDC_EPISOD_PHARE_OFF, IDC_EPISOD_PHARE_OFF, TButtonGadget::Command, TButtonGadget::Indeterminate)) ;
  pMyApp->cb3->LayoutSession() ;
*/
}
#endif // !_EXT_CAPTURE


// -----------------------------------------------------------------------------
//  Description :	Charge les paramètres
// -----------------------------------------------------------------------------
bool
NSEpisodus::loadParams()
{
	sCaptureKey = string(" ") ;
	sBabylonKey = string(" ") ;

	NSUtilisateurChoisi* pUtilisat = pContexte->getUtilisateur() ;
	if ((NSUtilisateurChoisi*) NULL == pUtilisat)
		return false ;

	string sFichier = string("episodus.dat") ;

	ifstream inFile ;
	bool bFileOpened = pUtilisat->OpenUserFile(&inFile, sFichier, string("episo[user].dat"), pContexte->PathName("FGLO")) ;
	if ((false == bFileOpened) || !inFile)
	{
  	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
		erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	string sData = string("") ;
	while (!inFile.eof())
	{
    string sLine = string("") ;
		getline(inFile, sLine) ;
		if (string("") != sLine)
			sData += sLine + string("\n") ;
	}
	inFile.close() ;

	// boucle de chargement des attributs
	size_t i = 0 ;
	while (i < strlen(sData.c_str()))
	{
		string sNomAttrib = string("") ;
		string sValAttrib = string("") ;

		while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]))
			sNomAttrib += pseumaj(sData[i++]) ;
		while ((strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))
			i++ ;

		while ((strlen(sData.c_str())) && ('\n' != sData[i]))
			sValAttrib += sData[i++] ;

		i++ ;

    size_t iValSize = strlen(sValAttrib.c_str()) ;

		if 		((sNomAttrib == "AUTOFLOCULE") && (string("") != sValAttrib))
		{
			if (IsNo(sValAttrib))
				bAutoFlocule = false ;
			else
				bAutoFlocule = true ;
		}
		else if ((sNomAttrib == "CAPTURE_FENETRE") && (string("") != sValAttrib))
		{
      if (IsNo(sValAttrib))
        bActiveGlobal = false ;
      else
        bActiveGlobal = true ;
    }
		else if ((sNomAttrib == "CAPTURE_ANALYSE") && (string("") != sValAttrib))
		{
      sValAttrib = pseumaj(sValAttrib) ;

#ifndef _EXT_CAPTURE
			if      (sValAttrib == string("PLACEMENT"))
				iDefaultAnalyse = EpisodusData::byPlacement ;
			else if (sValAttrib == string("ORDER"))
				iDefaultAnalyse = EpisodusData::byOrder ;
			else if (sValAttrib == string("PLACEMENT+ORDER"))
				iDefaultAnalyse = EpisodusData::byPlacementAndOrder ;
			else
				iDefaultAnalyse = EpisodusData::byPlacement ;
#else
			if      (sValAttrib == string("PLACEMENT"))
				pCaptureEngine->setDefaultAnalyse(EpisodusData::byPlacement) ;
			else if (sValAttrib == string("ORDER"))
				pCaptureEngine->setDefaultAnalyse(EpisodusData::byOrder) ;
			else if (sValAttrib == string("PLACEMENT+ORDER"))
				pCaptureEngine->setDefaultAnalyse(EpisodusData::byPlacementAndOrder) ;
			else
				pCaptureEngine->setDefaultAnalyse(EpisodusData::byPlacement) ;
#endif // _EXT_CAPTURE
		}
    else if ((sNomAttrib == "HOTKEY_FENETRE") && (string("") != sValAttrib))
      sCaptureKey = sValAttrib ;
    else if ((sNomAttrib == "CAPTURE_BABYLON") && (string("") != sValAttrib))
    {
      if (IsNo(sValAttrib))
        bActiveBabylon = false ;
      else
        bActiveBabylon = true ;
    }
    else if ((sNomAttrib == "HOTKEY_BABYLON") && (string("") != sValAttrib))
      sBabylonKey = sValAttrib[0] ;
    else if ((sNomAttrib == "SEND_NOW") && (string("") != sValAttrib))
    {
      if (IsYes(sValAttrib))
        bSendNow = true ;
      else
        bSendNow = false ;
    }
    else if ((sNomAttrib == "ASYMETRIC_HASH") && (string("") != sValAttrib))
    {
      if (IsNo(sValAttrib))
        sAsymetricHash = "";
      else
        sAsymetricHash = sValAttrib ;
    }
    else if ((sNomAttrib == "PRINCIPE_PROMETHE") && (string("") != sValAttrib))
			sPromethePrincipe = sValAttrib ;
    else if ((sNomAttrib == "URL_PROMETHE") && (string("") != sValAttrib))
			sPrometheURL = sValAttrib ;
    else if ((sNomAttrib == "MAIL_PROMETHE") && (string("") != sValAttrib))
			sPrometheMail = sValAttrib ;
    else if ((sNomAttrib == "USERID_PROMETHE") && (string("") != sValAttrib))
			sPrometheUserId = sValAttrib ;
    else if ((sNomAttrib == "USERLOGIN_PROMETHE") && (string("") != sValAttrib))
			sPrometheUserLogin = sValAttrib ;
    else if ((sNomAttrib == "USERPASWD_PROMETHE") && (string("") != sValAttrib))
			sPrometheUserPaswd = sValAttrib ;
    else if ((sNomAttrib == "POS_SOAP") && (string("") != sValAttrib))
			sPosSOAP = sValAttrib ;
    else if ((sNomAttrib == "POS_TANK") && (string("") != sValAttrib))
			sPosTank = sValAttrib ;
    else if ((sNomAttrib == "POS_CAPTURE") && (string("") != sValAttrib))
			sPosCapture = sValAttrib ;
    else if ((sNomAttrib == "POS_AUTOSAVE") && (string("") != sValAttrib))
		{
      if (IsYes(sValAttrib))
        bAutoSave = true ;
      else
        bAutoSave = false ;
    }
    else if ((sNomAttrib == "POS_LDV") && (string("") != sValAttrib))
			sPosLdV = sValAttrib ;
    else if ((sNomAttrib == "OPENMODE_LDV") && (string("") != sValAttrib))
			sOpenModeLdV = sValAttrib ;
    else if ((sNomAttrib == "AUTOOPEN_LDV") && (string("") != sValAttrib))
		{
      if (IsYes(sValAttrib))
        bAutoOpenLdV = true ;
      else
        bAutoOpenLdV = false ;
    }
    else if ((sNomAttrib == "FREECONCERN_LDV") && (string("") != sValAttrib))
		{
      if (IsYes(sValAttrib))
        bAllowFreeTextLdV = true ;
      else
        bAllowFreeTextLdV = false ;
    }
    else if ((sNomAttrib == "SERVICE_URL") && (string("") != sValAttrib))
			sServiceUrl = sValAttrib ;
    else if ((sNomAttrib == "SERVICE_TITLE") && (string("") != sValAttrib))
			sServiceTitle = sValAttrib ;
    else if ((sNomAttrib == "AUTOSEARCH_DELAY") && (string("") != sValAttrib))
    {
      size_t i = 0 ;
      for ( ; (i < iValSize) && (isdigit(sValAttrib[i])) ; i++) ;
      if (i == iValSize)
			  _iAutoSearchDelay = StringToInt(sValAttrib) ;
    }
    else if ((sNomAttrib == "AUTOSEARCH_MINCHAR") && (string("") != sValAttrib))
    {
      size_t i = 0 ;
      for ( ; (i < iValSize) && (isdigit(sValAttrib[i])) ; i++) ;
      if (i == iValSize)
			  _iAutoSearchMinChar = StringToInt(sValAttrib) ;
    }
    else if ((sNomAttrib == "PRESCRIPT_ADD_DCI") && (string("") != sValAttrib))
		{
      if (IsYes(sValAttrib))
        _bDrugAddVirtualDrug = true ;
      else
        _bDrugAddVirtualDrug = false ;
    }
    else if ((sNomAttrib == "VIDAL_APP_ID") && (string("") != sValAttrib))
		{
      pContexte->setBamType(NSContexte::btVidal) ;
      pContexte->setBamApplicationID(sValAttrib) ;
    }
    else if ((sNomAttrib == "VIDAL_APP_KEY") && (string("") != sValAttrib))
		{
      pContexte->setBamType(NSContexte::btVidal) ;
      pContexte->setBamApplicationKey(sValAttrib) ;
    }
  }
  return true ;
}

bool
NSEpisodus::loadEpiParams()
{
  NSUtilisateurChoisi* pUtilisat = pContexte->getUtilisateur() ;
	if ((NSUtilisateurChoisi*) NULL == pUtilisat)
		return false ;

  string sFichier = string("epiParams.dat") ;

	ifstream inFile ;
	bool bFileOpened = pUtilisat->OpenUserFile(&inFile, sFichier, string("epiParams[user].dat"), pContexte->PathName("FGLO")) ;
	if ((false == bFileOpened) || !inFile)
	{
  	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
		erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

  string sErrorMsg = string("") ;
  aEpiParams.init(&inFile, &sErrorMsg) ;

  inFile.close() ;

  return true ;
}

void
NSEpisodus::chargerPrincipes()
{
  ifstream inFile1, inFile2 ;
  string sLine ;
  string sData = "" ;

  string sLang = "" ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  // Ouverture du fichier spécifique de l'utilisateur
  // Opening user specific file
	string sFichier = string("principe-") + pContexte->getUtilisateur()->getNss() + string(".dat") ;

  sFichier = pContexte->PathName("FPER") + sFichier ;
  inFile1.open(sFichier.c_str()) ;
  if (!inFile1)
  {
    // Si le fichier spécifique ne peut être ouvert, ouverture du fichier générique.
    // If user specific file cannot be opened, opening the generic one.
    sFichier = pContexte->PathName("FPER") + string("principe.dat") ;
    inFile2.open(sFichier.c_str()) ;
    if (!inFile2)
    {
      string sErreur = string("Impossible d'ouvrir le fichier de principe de classification (") + sFichier + string(")") ;
      erreur(sErreur.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      // basic SOAP
      pPrincipes->push_back(new ClassificationPrinciple("0SOA01", "6CISP", false, "Motif de rencontre")) ;
      pPrincipes->push_back(new ClassificationPrinciple("0SOA41", "6CISP", true, "Appréciation")) ;
      pPrincipes->push_back(new ClassificationPrinciple("0SOA34", "6CISP", false, "Procédure")) ;
      return ;
    }
    while (!inFile2.eof())
    {
      getline(inFile2, sLine) ;
      if (sLine != "")
        sData += sLine + "\n" ;
    }
    inFile2.close() ;
  }
  else
  {
    while (!inFile1.eof())
    {
      getline(inFile1, sLine) ;
      if (sLine != "")
        sData += sLine + "\n" ;
    }
    inFile1.close() ;
  }

  string sPosition ;
  string sClassification ;
  string sUnicity ;
  bool   bUnicity ;
  string sLibelle ;

  // boucle de chargement des attributs
  size_t i = 0 ;
  while (i < strlen(sData.c_str()))
  {
    sPosition       = "" ;
    sClassification = "" ;
    sUnicity        = "" ;
    sLibelle        = "" ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))
      sPosition += sData[i++] ;

    while ((strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
      i++ ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))
      sClassification += sData[i++] ;

    while ((strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
      i++ ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sUnicity += sData[i++] ;

    while ((strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
      i++ ;

    while ((strlen(sData.c_str())) && (sData[i] != '\n'))
      sLibelle += sData[i++] ;

    i++ ;
    if (sPosition != "")
    {
      if      (sUnicity == "UNIQUE")
        bUnicity = true ;
      else if (sUnicity == "MULTIPLE")
        bUnicity = false ;
      else if (sPosition == "0SOA41")
        bUnicity = true ;
      else
        bUnicity = false ;

      pPrincipes->push_back(new ClassificationPrinciple(sPosition, sClassification, bUnicity, sLibelle)) ;
    }
  }
  return ;
}

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------------------//  Description :	Enregistre les paramètres// -----------------------------------------------------------------------------
bool
NSEpisodus::saveParams()
{
  NSUtilisateurChoisi* pUtilisat = pContexte->getUtilisateur() ;
  if (((NSUtilisateurChoisi*) NULL == pUtilisat) || (NULL == pUtilisat->getGraph()))
    return false ;

  return saveParams(pUtilisat->getGraph()->getAttributeValue(PIDS)) ;
}

bool
NSEpisodus::saveParams(string sUserId)
{
try
{
  string sFileName = string("episo") + sUserId + string(".dat") ;
  string sFichier  = pContexte->PathName("FGLO") + sFileName ;

  ofstream outFile ;
  string sLine ;

  outFile.open(sFichier.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFichier ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

# ifndef _EXT_CAPTURE
  switch(iDefaultAnalyse)
# else
  switch(pCaptureEngine->getDefaultAnalyse())
# endif // !_EXT_CAPTURE
  {
    case EpisodusData::byPlacement          : outFile << (string("CAPTURE_ANALYSE    ") + string("Placement") + string("\n")) ;
                                              break ;
    case EpisodusData::byOrder              : outFile << (string("CAPTURE_ANALYSE    ") + string("Order") + string("\n")) ;
                                              break ;
    case EpisodusData::byPlacementAndOrder  : outFile << (string("CAPTURE_ANALYSE    ") + string("Placement+Order") + string("\n")) ;
                                              break ;
  }

  if (bActiveGlobal == true)
    outFile << (string("CAPTURE_FENETRE    ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("CAPTURE_FENETRE    ") + string("Non") + string("\n")) ;

  outFile << (string("HOTKEY_FENETRE     ") + sCaptureKey + string("\n")) ;

  if (bActiveBabylon == true)
    outFile << (string("CAPTURE_BABYLON    ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("CAPTURE_BABYLON    ") + string("Non") + string("\n")) ;

  outFile << (string("HOTKEY_BABYLON     ") + sBabylonKey + string("\n")) ;

  if (bAutoFlocule == true)
    outFile << (string("AUTOFLOCULE        ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("AUTOFLOCULE        ") + string("Non") + string("\n")) ;

  if (bSendNow  == true)
    outFile << (string("SEND_NOW           ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("SEND_NOW           ") + string("Non") + string("\n")) ;

  if (bAutoSave  == true)
    outFile << (string("POS_AUTOSAVE       ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("POS_AUTOSAVE       ") + string("Non") + string("\n")) ;

  outFile << (string("ASYMETRIC_HASH     ") + string("Non") + string("\n")) ;
  outFile << (string("PRINCIPE_PROMETHE  ") + sPromethePrincipe + string("\n")) ;
  outFile << (string("MAIL_PROMETHE      ") + sPrometheMail + string("\n")) ;
  outFile << (string("USERID_PROMETHE    ") + sPrometheUserId + string("\n")) ;
  outFile << (string("URL_PROMETHE       ") + sPrometheURL + string("\n")) ;
  outFile << (string("USERLOGIN_PROMETHE ") + sPrometheUserLogin + string("\n")) ;
  outFile << (string("USERPASWD_PROMETHE ") + sPrometheUserPaswd + string("\n")) ;

  if (sPosSOAP != "")
    outFile << (string("POS_SOAP           ") + sPosSOAP + string("\n")) ;
  if (sPosTank != "")
    outFile << (string("POS_TANK           ") + sPosTank + string("\n")) ;
  if (sPosCapture != "")
    outFile << (string("POS_CAPTURE        ") + sPosCapture + string("\n")) ;

  if (sPosLdV != "")
    outFile << (string("POS_LDV            ") + sPosLdV + string("\n")) ;
  if (sOpenModeLdV != "")
    outFile << (string("OPENMODE_LDV       ") + sOpenModeLdV + string("\n")) ;

  if (bAutoOpenLdV  == true)
    outFile << (string("AUTOOPEN_LDV       ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("AUTOOPEN_LDV       ") + string("Non") + string("\n")) ;

  if (bAllowFreeTextLdV  == true)
    outFile << (string("FREECONCERN_LDV    ") + string("Oui") + string("\n")) ;
  else
    outFile << (string("FREECONCERN_LDV    ") + string("Non") + string("\n")) ;

	outFile << (string("SERVICE_URL        ") + sServiceUrl + string("\n")) ;
  outFile << (string("SERVICE_TITLE      ") + sServiceTitle + string("\n")) ;

  // BAM parameters
  //
  if (pContexte->getBamType() == NSContexte::btVidal)
  {
    outFile << (string("VIDAL_APP_ID       ") + pContexte->getBamApplicationID() + string("\n")) ;
    outFile << (string("VIDAL_APP_KEY      ") + pContexte->getBamApplicationKey() + string("\n")) ;
  }

  outFile.close() ;

  return true ;
}
catch (...)
{
  erreur("Exception NSEpisodus::saveParams.", standardError, 0) ;
  return false ;
}
}
#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------------------
// on cherche à partir d'un format référencé par sFormat à renvoyer par sText la
// chaîne telle qu'elle peut être traité directement par Nautilus
// pour les dates on doit renvoyer AAAAMMJJ
// -----------------------------------------------------------------------------
/*
void
NSEpisodus::formatElement(string * sText, string sFormat, bool bForced)
{
try
{
  bool    bFound = false ;
  string  sExpr ;

  // on cherche l'expression qui correspond à la référence donné par sFormat
  NSCaptureFormatArray CaptFmtArray(pContexte) ;
  CaptFmtArray.charger() ;
  if (!(CaptFmtArray.empty()))
  {
    for (IterCaptFmt iterCaptFormat = CaptFmtArray.begin() ; (iterCaptFormat != CaptFmtArray.end() && !bFound) ; iterCaptFormat++)
    {
      if ((*iterCaptFormat)->sTitre == sFormat)
      {
        sExpr = (*iterCaptFormat)->sExpression ;
        bFound = true ;
      }
    }
  }

  // si bFound est à true, c'est qu'on a trouvé le format référencé
  if (!bFound || (sExpr == ""))
  {
    *sText = "" ;
    return ;
  }

  // Préparation de la chaîne
  preCompare(sText, &sFormat) ;

  // Traitement en une passe : on progresse en même temps dans le texte et dans
  // le modèle
  size_t  iPosInText  = 0 ;
  size_t  iPosInExpr  = 0 ;
  bool    bContinue   = true ;

  string  sAnnee      = "" ;
  string  sMois       = "" ;
  string  sJour       = "" ;
  string  sSexe       = "" ;
  string  sEntier     = "" ;
  string  sDecimal    = "" ;

  while ((iPosInText < strlen(sText->c_str())) && (iPosInExpr < strlen(sExpr.c_str())) && (bContinue))
  {
    // Début d'un "champ"
    if (sExpr[iPosInExpr] == '$')
    {
      bool    bInSetOfValues = false ;
      iPosInExpr++ ;
      size_t iPosEndSet = sExpr.find("$", iPosInExpr) ;

      // on veut récupérer ce qui correspond à ce champ dans sText pour en constituer un format valide
      if (iPosEndSet != string::npos)
      {
        if (sExpr.find("|", iPosInExpr) < iPosEndSet)
          bInSetOfValues = true ;

        // Cas où on a un ensemble prédéterminé de cas possibles
        if (bInSetOfValues)
        {
          // Type de la donnée
          size_t iFinLabel = sExpr.find("|", iPosInExpr) ;
          string sLabel = string(sExpr, iPosInExpr, iFinLabel-iPosInExpr) ;
          iPosInExpr = iFinLabel + 1 ;

          // Recherche du modèle qui correspond au texte
          bool    bContinue = true ;
          size_t  iPosSepar = sExpr.find("|", iPosInExpr) ;
          int     iNumModele = 1 ;
          string  sModel ;
          while ((iPosSepar != string::npos) && (iPosSepar < iPosEndSet) && bContinue)
          {
            string sModel = string(sExpr, iPosInExpr, iPosSepar - iPosInExpr) ;
            size_t iModelLength = strlen(sModel.c_str()) ;

            // Le texte est-il assez long pour contenir le modèle
            if (iPosInText + iModelLength <= strlen(sText->c_str()))
            {
              string sSubText = string(*sText, iPosInText, iModelLength) ;
              if (smartCompare(&sSubText, &sModel))
              {
                bContinue = false ;
                iPosInText += iModelLength ;
                iPosInExpr = iPosEndSet + 1 ;
              }
            }
            if (bContinue)            {
              // Il reste des modèles à tester
              if (iPosSepar + 1 < iPosEndSet)
              {
                iPosInExpr  = iPosSepar + 1 ;
                iPosSepar   = sExpr.find("|", iPosInExpr) ;
                if ((iPosSepar > iPosEndSet) || (iPosSepar == string::npos))
                  iPosSepar = iPosEndSet ;
                iNumModele++ ;
              }
              // Il ne reste plus de modèles à tester
              else
              {
                if (bForced)
                {
                  *sText = "" ;
                  return ;
                }
                else
                {
                  iNumModele = 0 ;
                  bContinue = false ;
                }
              }
            }
          }

          // Traitement en fonction du type de la donnée

          // jour de la semaine (Lundi, Mardi...)
          if      (sLabel == "j")
          {
            // ignoré
          }

          // Jour calendaire (01, 02... 31)
          else if (sLabel == "JJ")
          {
            char    cJour[3] ;
            sprintf(cJour, "%d", iNumModele) ;            sJour = string(cJour) ;
          }

          // Mois (01, 02... 12)
          else if (sLabel == "MM")
          {
            char    cMois[3] ;
            sprintf(cMois, "%d", iNumModele) ;
            sMois = string(cMois) ;
          }

          // Année (1900...)
          else if (sLabel == "AAAA")
          {
            char    cAnnee[5] ;
            sprintf(cAnnee, "%d", iNumModele) ;
            sAnnee = string(cAnnee) ;
          }
          else if (sLabel == "SS")
          {
            switch (iNumModele)
            {
              case 1  : sSexe = "HMASC1" ;
                        break ;
              case 2  : sSexe = "HFEMI1" ;
                        break ;
              default : sSexe = "1INDE2" ;
            }
          }
        }
        // Ce n'est pas un ensemble prédéterminé de cas possibles
        else
        {
          if (sExpr[iPosInExpr] == 'j')
          {
            while ((iPosInExpr < iPosEndSet) && ((*sText)[iPosInText]))
            {
              if ((*sText)[iPosInText] != ' ')
                iPosInText++ ;
              iPosInExpr++ ;
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'J')
          {
            while((iPosInExpr < iPosEndSet) && ((*sText)[iPosInText]))
            {
              if ((*sText)[iPosInText] == ' ')
                iPosInExpr++ ;
              else
              {
                sJour += (*sText)[iPosInText] ;
                iPosInExpr++ ;
                iPosInText++ ;
              }
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'M')
          {
            while ((iPosInExpr < iPosEndSet) && ((*sText)[iPosInText]))
            {
              if ((*sText)[iPosInText] == ' ')
                iPosInExpr++ ;
              else
              {
                sMois += (*sText)[iPosInText] ;
                iPosInExpr++ ;
                iPosInText++ ;
              }
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'A')
          {
            while ((iPosInExpr < iPosEndSet) && ((*sText)[iPosInText]))
            {
              if ((*sText)[iPosInText] == ' ')
                iPosInExpr++ ;
              else
              {
                sAnnee += (*sText)[iPosInText] ;
                iPosInExpr++ ;
                iPosInText++ ;
              }
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'S')
          {
            while ((iPosInExpr < iPosEndSet) && ((*sText)[iPosInText]))
            {
              if ((*sText)[iPosInText] == ' ')
                iPosInExpr++ ;
              else
              {
                sSexe += (*sText)[iPosInText] ;
                iPosInExpr++ ;
                iPosInText++ ;
              }
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'U')
          {
            iPosInExpr++ ;
            while (((*sText)[iPosInText]) && isdigit((*sText)[iPosInText]))
            {
                sEntier += (*sText)[iPosInText] ;
                iPosInText++ ;
            }
            iPosInExpr++ ;
          }
          if (sExpr[iPosInExpr] == 'd')
          {
            iPosInExpr++ ;
            while (((*sText)[iPosInText]) && isdigit((*sText)[iPosInText]))
            {
                sDecimal += (*sText)[iPosInText] ;
                iPosInText++ ;
            }
            iPosInExpr++ ;
          }
        }
      }
    }
    // Pas le début d'un champ
    else
    {
      if (sExpr[iPosInExpr] == (*sText)[iPosInText])
      {
        iPosInExpr++ ;
        iPosInText++ ;
      }
      else
      {
        *sText = "" ;
        return ;
        // bContinue = false ;
      }
    }
  }
*/
/* ------- Old delete
  int iPosBegDollar = sExpr.find("$") ;
  int iPosEndDollar = string::npos ;
  if (iPosBegDollar + 1 < strlen(sExpr.c_str())
    iPosEndDollar = sExpr.find("$", iPosBegDollar + 1) ;

  bool    bSetOfValues = false ;
  if (iPosEndDollar != string::npos)
  {
    if (sExpr.find("|", iPosBegDollar) < iPosEndDollar)
      bSetOfValues = true ;
  }

  int iPosInText = iPosBegDollar ;
  while ((iPosInText < strlen(sText.c_str())) && (iPosBegDollar != string::npos) && (iPosEndDollar != string::npos))
  {
  }
*/
/*
  // on fait un champs valide si c'est une date valide :
  if ((sAnnee != "") && (sMois != "") && (sJour != ""))
  {
    if (strlen(sMois.c_str()) < 2)
      sMois = "0" + sMois ;

    if (strlen(sJour.c_str()) < 2)
      sJour = "0" + sJour ;

    *sText = sAnnee + sMois + sJour ;
  }

  if (sSexe != "")
    *sText = sSexe ;

  if ((sEntier != "") || (sDecimal != ""))
  {
    *sText = sEntier ;
    if (sDecimal != "")
    {
      if (sEntier != "")
        *sText += string(".") + sDecimal ;
      else
        *sText = string("0.") + sDecimal ;
    }
  }
}
catch (...)
{
	erreur("Exception NSEpisodus::formatElement.", standardError, 0) ;
}
}

void
NSEpisodus::preCompare(string * psText, string * psModel)
{
  if (*psText == "")
    return ;

  // Verrue Axilog : on enlève l'espace après les 1
  for (size_t i = 0 ; i < strlen(psText->c_str()) ; i++)
  {
    if (((*psText)[i] == '1') && (i < strlen(psText->c_str()) - 2))
    {
      if (((*psText)[i + 1] == ' ') && ((*psText)[i + 2] >= '0') && ((*psText)[i + 2] <= '9'))
        *psText = string(*psText, 0, i + 1) + string(*psText, i + 2, strlen(psText->c_str()) - i - 2) ;
    }
  }
}

bool
NSEpisodus::smartCompare(string * psText, string * psModel)
{
  if (*psText == "")
  {
    if (*psModel == "")
      return true ;
    else
      return false ;
  }
  if (*psModel == "")
    return false ;

  if (strlen(psText->c_str()) != strlen(psModel->c_str()))
    return false ;

  for (size_t i = 0 ; i < strlen(psText->c_str()) ; i++)
  {
    if ((*psModel)[i] == 'I')
    {
      if (((*psText)[i] != 'I') && ((*psText)[i] != 'l'))
        return false ;
      (*psText)[i] = 'I' ;
    }
    else if ((*psModel)[i] == 'l')
    {
      if (((*psText)[i] != 'I') && ((*psText)[i] != 'l'))
        return false ;
      (*psText)[i] = 'l' ;
    }
    else if ((*psModel)[i] == '0')
    {
      if (((*psText)[i] != 'O') && ((*psText)[i] != '0'))
        return false ;
      (*psText)[i] = '0' ;
    }
    else if ((*psModel)[i] == 'O')
    {
      if (((*psText)[i] != 'O') && ((*psText)[i] != '0'))
        return false ;
      (*psText)[i] = 'O' ;
    }
    else if ((*psModel)[i] != (*psText)[i])
      return false ;
  }
  return true ;
}

*/


// -----------------------------------------------------------------------------
// fonction d'écriture des logs
// -----------------------------------------------------------------------------
void
NSEpisodus::logSpyMessage(string sSpyMessage)
{
  ifstream    inFile ;
  ofstream    outFile ;
  string      line ;
  string      sLastSpyMessage ;
  time_t      t_current_time ;
  struct tm   *tm_current_time ;
  char        *s_current_time ;

  t_current_time = time(NULL) ;
  tm_current_time = localtime(&t_current_time) ;
  s_current_time = asctime(tm_current_time) ;
  // on ajoute le nouveau log
  sLastSpyMessage = string(s_current_time, 0, strlen(s_current_time) - 1) + " : " + sSpyMessage + "\n" ;

  // On écrit le nouveau fichier spy.log
  outFile.open("spy.log", ios::app) ;
  if (!outFile)
  {
    // cas d'erreur bloquant...
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox(NULL, "Erreur d'ouverture en écriture du fichier spy.log.", sCaption.c_str(), MB_ICONEXCLAMATION) ;
    return ;
  }

  outFile.write(sLastSpyMessage.c_str(), strlen(sLastSpyMessage.c_str()));

  outFile.close();
}

#ifndef __EPIBRAIN__
#ifndef _EXT_CAPTURE
// -----------------------------------------------------------------------------
//
//							Classe NSModelDlg
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSModelDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_BN_CLICKED(IDC_CREER,      creer),
  EV_BN_CLICKED(IDC_REPLACE,    modifier),
  EV_BN_CLICKED(IDC_REMOVE,     supprimer),
  EV_BN_CLICKED(IDC_SUBREMOVE,  sub_supprimer),
  EV_BN_CLICKED(IDC_IMPORT,     importer),
END_RESPONSE_TABLE ;

NSModelDlg::NSModelDlg(TWindow* parent, NSContexte* pCtx, EpisodusModel* pMod)
  : TDialog(parent, "EPISOD_REFERENCE"),
    NSRoot(pCtx)
{
try
{
  pModel          = pMod ;

  pNewLib         = new OWL::TEdit(this, IDC_NOUVEAU) ;
  pCreer          = new OWL::TButton(this, IDC_CREER) ;

  pLibelles       = new OWL::TListBox(this, IDC_LIBELLES) ;
  pModifier       = new OWL::TButton(this, IDC_REPLACE) ;
  pSupprimer      = new OWL::TButton(this, IDC_REMOVE) ;

  pSubLibelles    = new OWL::TListBox(this, IDC_SUBLIBELLES) ;
  pSubSupprimer   = new OWL::TButton(this, IDC_SUBREMOVE) ;

  pBtnImport      = new OWL::TButton(this, IDC_IMPORT) ;

  ArrayEpisodusModelData* pM  = pContexte->getEpisodus()->pModels ;
  ArrayEpisodusModelData* pSM = pContexte->getEpisodus()->pSubModels ;

  pNewModels      = new ArrayEpisodusModelData(*pM) ;
  pNewSubModels   = new ArrayEpisodusModelData(*pSM) ;
}
catch (...)
{
  erreur("Exception NSModelDlg ctor.", standardError, 0) ;
  return ;
}
}


NSModelDlg::~NSModelDlg()
{
  delete pNewModels ;
  delete pNewSubModels ;

  delete pNewLib ;
  delete pCreer ;
  delete pLibelles ;
  delete pModifier ;
  delete pSupprimer ;
  delete pSubLibelles ;
  delete pSubSupprimer ;
  delete pBtnImport ;
}


void
NSModelDlg::SetupWindow()
{
  TDialog::SetupWindow() ;

  if (!pModel)
  {
    uint32 uiStyle = pNewLib->TWindow::GetStyle() ;
    pNewLib->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

    uiStyle = pCreer->TWindow::GetStyle() ;
    pCreer->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

    uiStyle = pModifier->TWindow::GetStyle() ;
    pModifier->TWindow::SetStyle(uiStyle | WS_DISABLED) ;
  }
  rempliListe() ;
}


void
NSModelDlg::rempliListe()
{
  pLibelles->ClearList() ;

  if (!(pNewModels->empty()))
    for (ArrayEpisodusModelIter i = pNewModels->begin() ; i != pNewModels->end() ; i++)
      pLibelles->AddString(((*i)->sNom).c_str()) ;

  pSubLibelles->ClearList() ;

  if (!(pNewSubModels->empty()))
    for (ArrayEpisodusModelIter i = pNewSubModels->begin() ; i != pNewSubModels->end() ; i++)
      pSubLibelles->AddString(((*i)->sNom).c_str()) ;
}


void
NSModelDlg::CmOk()
{
  ArrayEpisodusModelData* pM  = pContexte->getEpisodus()->pModels ;
  if (pM)
    *pM = *pNewModels ;

  ArrayEpisodusModelData* pSM = pContexte->getEpisodus()->pSubModels ;
  if (pSM)
    *pSM = *pNewSubModels ;

  pContexte->getEpisodus()->ExportModels() ;

  CloseWindow(IDOK) ;
}


void
NSModelDlg::CmCancel()
{
	Destroy(IDCANCEL) ;
}


void
NSModelDlg::creer()
{
  if (!pModel)
    return ;

  char far* szAmmorce = new char[1025] ;
  pNewLib->GetText(szAmmorce, 1024) ;
  if (szAmmorce[0] == '\0')
  {
    erreur("Vous devez donner un nom au modèle", standardError, 0) ;
    delete[] szAmmorce ;
    return ;
  }

  pModel->sNom = string(szAmmorce) ;
  delete[] szAmmorce ;

  if (!(pNewModels->empty()))
  {
  	ArrayEpisodusModelIter i = pNewModels->begin() ;
  	for ( ; (i != pNewModels->end()) && ((*i)->sNom != string(szAmmorce)) ; i++)
    ;
  	if (i != pNewModels->end())
  	{
    	erreur("Ce nom a déjà été attribué à un autre modèle", standardError, 0) ;
    	return ;
  	}
  }

  pModel->bModified = false ;
  pNewModels->push_back(new EpisodusModel(*pModel)) ;

  rempliListe() ;
}


void
NSModelDlg::modifier()
{
  if (!pModel)
    return ;

  if (pLibelles->GetSelIndex() < 0)
  {
    erreur("Vous devez sélectionner un modèle", standardError, 0) ;
    return ;
  }

  char far* szAmmorce = new char[1024] ;
  pLibelles->GetSelString(szAmmorce, 1024) ;
  if (szAmmorce[0] == '\0')
  {
    delete[] szAmmorce ;
    return ;
  }

  ArrayEpisodusModelIter i = pNewModels->begin() ;
  for ( ; (i != pNewModels->end()) && ((*i)->sNom != string(szAmmorce)) ; i++)
    ;

  delete[] szAmmorce ;
  if (i == pNewModels->end())
  {
    erreur("Modèle introuvable", standardError, 0) ;
    return ;
  }

  *(*i) = *pModel ;
}


void
NSModelDlg::supprimer()
{
  if (pLibelles->GetSelIndex() < 0)
  {
    erreur("Vous devez sélectionner un modèle", standardError, 0) ;
    return ;
  }

  char far* szAmmorce = new char[1024] ;
  pLibelles->GetSelString(szAmmorce, 1024) ;
  if (szAmmorce[0] == '\0')
  {
    delete[] szAmmorce ;
    return ;
  }

  ArrayEpisodusModelIter i = pNewSubModels->begin() ;
  for ( ; (i != pNewSubModels->end()) && ((*i)->sNom != string(szAmmorce)) ; i++)
    ;

  delete[] szAmmorce ;
  if (i == pNewSubModels->end())
  {
    erreur("Modèle introuvable", standardError, 0) ;
    return ;
  }

  delete (*i) ;
  pNewSubModels->erase(i) ;

  rempliListe() ;
}

void
NSModelDlg::sub_supprimer()
{
  if (pSubLibelles->GetSelIndex() < 0)
  {
    erreur("Vous devez sélectionner un modèle", standardError, 0) ;
    return ;
  }

  char far* szAmmorce = new char[1024] ;
  pSubLibelles->GetSelString(szAmmorce, 1024) ;

  if (szAmmorce[0] == '\0')
  {
    delete[] szAmmorce ;
    return ;
  }

  ArrayEpisodusModelIter i = pNewSubModels->begin() ;
  for ( ; (i != pNewSubModels->end()) && ((*i)->sNom != string(szAmmorce)) ; i++)
    ;

  delete[] szAmmorce ;
  if (i == pNewSubModels->end())
  {
    erreur("Modèle introuvable", standardError, 0) ;
    return ;
  }

  delete (*i) ;
  pNewSubModels->erase(i) ;

  rempliListe() ;
}

void
NSModelDlg::importer()
{
  string sPath = pContexte->PathName("FGLO") ;
  char path[1024] ;
  strcpy(path, sPath.c_str()) ;

  TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST, "*.xml", 0, path, "*.xml") ;
  int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;
  pContexte->reallocateBaseDirectory() ;
	if (IDOK != iUserChoice)
    return ;

  string sFileName = string(initData.FileName) ;
  NSModelImportDlg(this, pContexte, sFileName).Execute() ;
}

void
NSModelDlg::importModel(ArrayEpisodusModelData* pModelImport)
{
  if (!pModelImport || pModelImport->empty())
    return ;

  ArrayEpisodusModelIter iImport = pModelImport->begin() ;
  for ( ; (iImport != pModelImport->end()) ; iImport++)
  {
    ArrayEpisodusModelIter i = pNewModels->begin() ;
    for ( ; (i != pNewModels->end()) && ((*i)->sNom != (*iImport)->sNom) ; i++)
      ;
    if (i != pNewModels->end())
    {
      string sErreur = (*iImport)->sNom + string(" : ") + pContexte->getSuperviseur()->getText("captureModelErrors", "alreadyExists") ;
      erreur(sErreur.c_str(), standardError, 0) ;
    }
    else
      pNewModels->push_back(new EpisodusModel(**iImport)) ;
  }

  rempliListe() ;
}


void
NSModelDlg::importSubModels(ArrayEpisodusModelData* pModelImport)
{
  if ((NULL == pModelImport) || pModelImport->empty())
    return ;

  ArrayEpisodusModelIter iImport = pModelImport->begin() ;
  for ( ; pModelImport->end() != iImport ; iImport++)
  {
    ArrayEpisodusModelIter i = pNewSubModels->begin() ;
    for ( ; (pNewSubModels->end() != i) && ((*i)->sNom != (*iImport)->sNom) ; i++)
      ;
    if (pNewSubModels->end() != i)
    {
      string sErreur = (*iImport)->sNom + string(" : ") + pContexte->getSuperviseur()->getText("captureModelErrors", "alreadyExists") ;
      erreur(sErreur.c_str(), standardError, 0) ;
    }
    else
      pNewSubModels->push_back(new EpisodusModel(**iImport)) ;
  }

  rempliListe() ;
}
#endif // __EPIBRAIN__

// -----------------------------------------------------------------------------
//
//							Classe NSModelImportDlg
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSModelImportDlg, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_BN_CLICKED(IDC_IMPORT, importer),
  EV_BN_CLICKED(IDC_CREER,  importAll),
END_RESPONSE_TABLE;

NSModelImportDlg::NSModelImportDlg(NSModelDlg * parent, NSContexte * pCtx, string sImportFile)
  : TDialog(parent, "EPISOD_REF_IMPORT"),
    NSRoot(pCtx)
{
  sImportedFile   = sImportFile ;

  pNewLib         = new OWL::TEdit(this, IDC_NOUVEAU) ;
  pLibelles       = new OWL::TListBox(this, IDC_LIBELLES) ;
  pSubLibelles    = new OWL::TListBox(this, IDC_SUBLIBELLES) ;
  pBtnImport      = new OWL::TButton(this, IDC_IMPORT) ;
  pBtnImportAll   = new OWL::TButton(this, IDC_CREER) ;

  pModels         = new ArrayEpisodusModelData() ;
  pSubModels      = new ArrayEpisodusModelData() ;
  pModelPere      = parent ;
}

NSModelImportDlg::~NSModelImportDlg()
{
  delete pModels ;
  delete pSubModels ;

  delete pNewLib ;
  delete pLibelles ;
  delete pSubLibelles ;
  delete pBtnImport ;
  delete pBtnImportAll ;
}

void
NSModelImportDlg::SetupWindow()
{
  TDialog::SetupWindow() ;
  openFile() ;
  rempliListe() ;
}

void
NSModelImportDlg::openFile()
{
try
{
  ifstream    inFile ;
  char        buffer[1024] ;
  string      sFilecontenu = "" ;
  int         buf_size = 1024 ;

  // extraction des modèles du fichier de stockage
  inFile.open(sImportedFile.c_str()) ;
  if (!inFile)
  {
    string sErreur = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sImportedFile ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  while (inFile.getline(buffer, buf_size))    sFilecontenu += buffer ;
  inFile.close() ;

  string sBaliseDeb = string("<nscapturemodel>") ;
  string sBaliseFin = string("</nscapturemodel>") ;

  // Suppression des balises racines <nscapturemodel> et </nscapturemodel>
  size_t posDeb = sFilecontenu.find(sBaliseDeb) ;
  size_t posFin = sFilecontenu.find(sBaliseFin) ;

  if ((posDeb == string::npos) || (posFin == string::npos) || (posFin < posDeb))
  {
    string sErreur = string("Le fichier des modèles de capture est défectueux") ;

    if      (posDeb == string::npos)
      sErreur += string(" (pas de balise de début)") ;
    else if (posFin == string::npos)
      sErreur += string(" (pas de balise de fin)") ;
    else if (posFin < posDeb)
      sErreur += string(" (balise de fin avant la balise de début)") ;

    sErreur += string(" - fichier ") + sImportedFile ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  size_t vraiPosDeb = posDeb + strlen(sBaliseDeb.c_str()) ;
  sFilecontenu = string(sFilecontenu, vraiPosDeb, posFin - vraiPosDeb) ;

  // création de la liste des modèles dans NSEpisodus (pModels)
  EpisodusModelXML ModelsXML ;
  ModelsXML.ImportModelXML(sFilecontenu) ;

  if (!ModelsXML.sArrayBalise.empty() && !ModelsXML.sArrayBaliseContenu.empty())
  {
    vector<string>::iterator XMLModelBaliseIter = ModelsXML.sArrayBalise.begin() ;
    vector<string>::iterator XMLModelBaliseContenuIter = ModelsXML.sArrayBaliseContenu.begin() ;
    vector<string>::iterator XMLModelBaliseVariablesIter = ModelsXML.sArrayBaliseVariables.begin() ;

    for ( ; XMLModelBaliseIter != ModelsXML.sArrayBalise.end() ; XMLModelBaliseIter++)
    {
      if (XMLModelBaliseContenuIter == ModelsXML.sArrayBaliseContenu.end())
        return ;

      if      (*XMLModelBaliseIter == "model")
        pModels->push_back(new EpisodusModel(*XMLModelBaliseContenuIter, *XMLModelBaliseVariablesIter)) ;
      else if (*XMLModelBaliseIter == "submodel")
        pSubModels->push_back(new EpisodusModel(*XMLModelBaliseContenuIter, *XMLModelBaliseVariablesIter)) ;

      XMLModelBaliseContenuIter++ ;
      XMLModelBaliseVariablesIter++ ;
    }
    return ;
  }
  return ;
}
catch (...)
{
  erreur("Exception NSEpisodus::openFile.", standardError, 0) ;
}
}

void
NSModelImportDlg::rempliListe()
{
  pLibelles->ClearList() ;

  if (!(pModels->empty()))
    for (ArrayEpisodusModelIter i = pModels->begin() ; i != pModels->end() ; i++)
      pLibelles->AddString(((*i)->sNom).c_str()) ;
  pSubLibelles->ClearList() ;

  if (!(pSubModels->empty()))
    for (ArrayEpisodusModelIter i = pSubModels->begin() ; i != pSubModels->end() ; i++)
      pSubLibelles->AddString(((*i)->sNom).c_str()) ;
}

void
NSModelImportDlg::importAll()
{
  pModelPere->importModel(pModels) ;
  pModelPere->importSubModels(pSubModels) ;
}

void
NSModelImportDlg::importer()
{
  int iModelSelCount    = pLibelles->GetSelCount() ;
  int iSubModelSelCount = pSubLibelles->GetSelCount() ;

  if ((iModelSelCount <= 0) && (iSubModelSelCount <= 0))
    return ;

  if (iModelSelCount > 0)
  {
    char ** pSelected = new char*[iModelSelCount] ;

    for (int i = 0 ; i < iModelSelCount ; i++)
      pSelected[i] = new char[1024] ;

    pLibelles->GetSelStrings(pSelected, iModelSelCount, 1024) ;
    ArrayEpisodusModelData ModelImport ;

    for (int i = 0 ; i < iModelSelCount ; i++)
    {
      ArrayEpisodusModelIter iMod = pModels->begin() ;
      for ( ; (pModels->end() != iMod) && ((*iMod)->sNom != string(pSelected[i])) ; iMod++)
        ;
      if (pModels->end() != iMod)
        ModelImport.push_back(new EpisodusModel(**iMod)) ;
    }

    if (false == ModelImport.empty())
      pModelPere->importModel(&ModelImport) ;

    for (int i = 0 ; i < iModelSelCount ; i++)
      delete[] pSelected[i] ;
    delete[] pSelected ;
  }

  if (iSubModelSelCount > 0)
  {
    char** pSelected = new char*[iSubModelSelCount] ;

    for (int i = 0 ; i < iSubModelSelCount ; i++)
      pSelected[i] = new char[1024] ;

    pSubLibelles->GetSelStrings(pSelected, iSubModelSelCount, 1024) ;
    ArrayEpisodusModelData ModelImport ;

    for (int i = 0 ; i < iSubModelSelCount ; i++)
    {
      ArrayEpisodusModelIter iMod = pSubModels->begin() ;
      for ( ; (pSubModels->end() != iMod) && ((*iMod)->sNom != string(pSelected[i])) ; iMod++)
        ;
      if (iMod != pSubModels->end())
        ModelImport.push_back(new EpisodusModel(**iMod)) ;
    }

    if (false == ModelImport.empty())
      pModelPere->importSubModels(&ModelImport) ;

    for (int i = 0 ; i < iSubModelSelCount ; i++)
      delete[] pSelected[i];
    delete[] pSelected ;
  }
}

void
NSModelImportDlg::CmOk()
{
	CloseWindow(IDOK) ;
}

#endif // !_EXT_CAPTURE

// -----------------------------------------------------------------------------
//
// Classe NSDPIO
//
// -----------------------------------------------------------------------------

#ifndef __EPIBRAIN__

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSPredi::NSPredi(NSContexte * pCtx)
        :NSRoot(pCtx),
         _Papyrus(pCtx, string("predi"), string("Séance"))
{
  _bPrediActif       = false ;
  _bMinimalInterface = false ;
  _bReturnCloses     = false ;
  _bPrediPreCoche    = false ;

  _iPrediState       = state0 ;

  _bLogPage          = false ;
  _pLogMDIWnd        = (NSLogChild *)  0 ;
  _pLogWnd           = (NSLogWindow *) 0 ;

  _bSendPage         = false ;
  _sSendPage         = "" ;
  _sMailAdress       = "" ;

  _iPosX       = 0 ;
  _iPosY       = 0 ;
  _iDeltaPosX  = 0 ;
  _iDeltaPosY  = 0 ;

  _iCurrentX   = 0 ;
  _iCurrentY   = 0 ;

  _sNomModule  = "" ;
  _sOEMModule  = "" ;
  _sOEMWeb     = "" ;

  _pPrediDlg   = (NSPrediDlg*) 0 ;

  // chargement des paramètres généraux // Loading global parameters
  loadParams() ;
}

NSPredi::~NSPredi()
{
}

voidNSPredi::UserChanged(){
  string ps = "Entering Predi UserChanged" ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  // PatChanged() ;

  // chargement des paramètres personnels de l'utilisateur
  loadParams() ;

  ps = "Leaving Predi UserChanged" ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;
}

void
NSPredi::PatOpening()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string ps = pSuper->getText("prevention", "preventionSessionOpening") ;
  pSuper->trace(&ps, 1) ;

try
{
  _Papyrus.ReinitLogPage() ;
  _sSendPage = string("") ;

  _iCurrentX = _iPosX ;
  _iCurrentY = _iPosY ;

	if (_bLogPage)
	{
    ps = pSuper->getText("prevention", "preventionReinitLogFile") ;
  	pSuper->trace(&ps, 1) ;

		_Papyrus.ReinitLogFile() ;
	}
}
catch (...)
{
  erreur("Exception NSPredi::PatOpening.", standardError, 0) ;
}

try
{
  // on crée la fenetre de log
  if (_bLogPage && (_Papyrus.getLogFile() != string("")))
  {
    ps = string("PREDI opening patient: creating Log window") ;
    pSuper->trace(&ps, 1) ;

    string sTitle = pSuper->getText("prevention", "preventionSessionReport") ;

    _pLogWnd    = new NSLogWindow(pContexte->GetMainWindow(), _Papyrus.getLogFile(), pContexte) ;
    _pLogMDIWnd = new NSLogChild(pContexte, *(pSuper->getApplication()->prendClient()), sTitle.c_str(), _pLogWnd) ;
    _pLogMDIWnd->Create() ;
    _pLogMDIWnd->Show(SW_MINIMIZE) ;
  }
  else
  {
    _pLogWnd    = (NSLogWindow *) 0 ;
    _pLogMDIWnd = (NSLogChild *)  0 ;
  }
}
catch(EWin32Error& e)
{
  string sMsg = string("Exception NSPredi::PatOpening when creating log window: ") + string(e.Message.c_str()) ;
  pSuper->trace(&sMsg, 1, NSSuper::trError) ;
}
catch (...)
{
  ps = string("Exception NSPredi::PatOpening when creating log window.") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;
}

  ps = pSuper->getText("prevention", "preventionSessionOpened") ;
  pSuper->trace(&ps, 1) ;
}


void
NSPredi::PatChanged()
{
try
{
  string ps = pContexte->getSuperviseur()->getText("prevention", "preventionSessionPatientChanging") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  // Si un DPIO a eu lieu, et n'a pas donné lieu à constitution d'un message, on
  // créé un message.
  // If the user answered DPIO but did not elaborate a message, we make a message
  if ((pContexte->getEpisodus()->sPrometheUserId != "") && (_iPrediState > stateQuestions) && (_iPrediState < stateFinished))
    AppendSendFile() ;

  // Enregistrement du log en tant que document
  if (_Papyrus.getLogFile() != string(""))
  {
    if (_Papyrus.wasLogPageModified() && (_iPrediState > stateQuestions) && (_iPrediState < stateFinished))
    {
      ps = pContexte->getSuperviseur()->getText("prevention", "preventionSessionReportRecording") ;
      pContexte->getSuperviseur()->trace(&ps, 1);

      string sNomFichier ;
      string sLocalis ;
      NSRefDocument NewDoc(0, pContexte) ;

      string sDocumentName = pContexte->getSuperviseur()->getText("prevention", "preventionSession") ;

      NewDoc.TrouveNomFichier("ZSHTM", "htm", sNomFichier, sLocalis) ;

      _logDocumentFileName = sNomFichier ;

      if (NewDoc.Referencer("ZSHTM", sDocumentName.c_str(), sNomFichier, sLocalis, true, false))
        // ::MoveFileEx(sLogFile.c_str(), (pContexte->PathName(sLocalis) + sNomFichier).c_str(), MOVEFILE_REPLACE_EXISTING) ;
        ::MoveFile(_Papyrus.getLogFile().c_str(), (pContexte->PathName(sLocalis) + sNomFichier).c_str()) ;
    }
    else
    {
      ps = pContexte->getSuperviseur()->getText("prevention", "preventionSessionReportDeletion") ;
      pContexte->getSuperviseur()->trace(&ps, 1) ;
      NsDeleteTemporaryFile(pContexte->getSuperviseur(), _Papyrus.getLogFile()) ;
    }
  }

  _iPrediState = stateFinished ;

  if (_pPrediDlg)
  {
    delete _pPrediDlg ;
    _pPrediDlg = (NSPrediDlg*) 0 ;
  }

  if (_pLogMDIWnd)
  {
    _pLogMDIWnd->Destroy() ;
    delete _pLogMDIWnd ;
    _pLogMDIWnd = (NSLogChild*) 0 ;
  }

  ps = pContexte->getSuperviseur()->getText("prevention", "preventionSessionPatientChanged") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;
}
catch (...)
{
  erreur("Exception NSDPIO::PatChanged.", standardError, 0) ;
}
}

void
NSPredi::ReinitLogFile()
{
  _Papyrus.ReinitLogFile() ;
}

void
NSPredi::AppendLogFile()
{
try
{
  _Papyrus.AppendLogFile() ;

  // Rafraichissement de la fenêtre
  if (_pLogWnd)
    _pLogWnd->Refresh() ;
}
catch (...)
{
  erreur("Exception NSPredi::AppendLogFile.", standardError, 0) ;
}
}

void
NSPredi::AppendSendFile()
{
try
{
  if (false == _bSendPage)
    return ;

  if (string("") == _sSendPage)
    return ;

  string sPatId       = "" ;
  string sPatCode     = "" ;
  string sPatSexe     = "" ;
  string sPatNaiss    = "" ;
  string sPatCP       = "" ;
  string sExamDate    = donne_date_duJour() ;
  string sExamCond    = "" ;
  string sExamRV      = "" ;

  // On vérifie qu'on possède toutes les informations
  NSPatientChoisi * pPatient = pContexte->getPatient() ;
  if (NULL == pPatient)
    return ;

  sPatId   = pPatient->getNss() ;
  sPatCode = pPatient->getCode() ;

  // Sexe
  if      (pPatient->estMasculin())
    sPatSexe = "1" ;
  else if (pPatient->estFeminin())
    sPatSexe = "2" ;

  // Date de naissance - Birth date
  string sNaissance = pPatient->donneNaissance() ;
  if (string("00000000") != sNaissance)
    sPatNaiss = sNaissance ;

  // Code postal - Zip code
  if (pPatient->initGraphs())
  {
    string sVille, sPays, sZip, sCedex ;
    if (pPatient->_GraphAdr.trouveInfoVilleAdr(sVille, sPays, sZip, sCedex))
      sPatCP = sZip ;
  }

  ofstream    outFile ;
  string      filecontenu = "" ;
  // char        line[1024] ;
  // char        buffer[1024] ;

  // Ouverture du fichier dpioXXX.msg - Opening file dpioXXX.msg
  // Fichier positionné sur le répertoire global afin de permettre de changer de
  // machine.
  // File is open on the global directory, so that the user can use different
  // machines.

	string sFichier = string("dpio2-") + pContexte->getUtilisateur()->getNss() + string(".msg") ;

  sFichier = pContexte->PathName("BGLO") + sFichier ;
  outFile.open(sFichier.c_str(), ios::ate | ios::app) ;
  if (!outFile)
  {
    string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return ;
  }

  // Patient
  string sBalise = string("<pmed_patient patientid=\"") + sPatId + string("\"") ;
  if (sPatCode != "")
    sBalise += string(" code=\"") + sPatCode + string("\"") ;
  sBalise += string(" contact=\"") + pContexte->getTransaction() + string("\"") ;
  sBalise += string(">") ;
  outFile << (sBalise + string("\n")) ;

  // Données administratives
  sBalise = string("<pmed_admindata") ;
  if (sPatSexe != "")
    sBalise += string(" sexe=\"") + sPatSexe + string("\"") ;
  if (sPatNaiss != "")
    sBalise += string(" naissance=\"") + string(sPatNaiss, 6, 2) + string("/") + string(sPatNaiss, 4, 2) + string("/") + string(sPatNaiss, 0, 4) + string("\"") ;
  if (sPatCP != "")
    sBalise += string(" code_post=\"") + sPatCP + string("\"") ;
  sBalise += string("/>") ;
  outFile << (sBalise + string("\n")) ;

	string sLang = pContexte->getUserLanguage() ;

  // Données médicales
  string sDate = donne_date_duJour() ;
  string sDateJour    = "" ;
  string sIntro       = "" ;
  donne_date_breve(sDate, &sDateJour, &sIntro, sLang) ;

  sBalise = string("<pmed_healthdata") ;
  if (sDateJour != "")
    sBalise += string(" date=\"") + sDateJour + string("\"") ;
  sBalise += string(">") ;
  outFile << (sBalise + string("\n")) ;

  // Contact
  outFile << _sSendPage ;

  outFile << (string("</pmed_healthdata>") + string("\n")) ;
  outFile << (string("</pmed_patient>") + string("\n")) ;

  outFile.close() ;
}
catch (...)
{
	erreur("Exception NSDPIO::AppendSendFile.", standardError, 0) ;
}
}

void
NSPredi::sendFile()
{
try
{
  NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
  if ((NSUtilisateurChoisi*) NULL == pUser)
    return ;

  ifstream inFile ;

  string sUserID = pContexte->getEpisodus()->sPrometheUserId ;
	string sFileName = pContexte->PathName("BGLO") + string("dpio2-") + pUser->getNss() + string(".msg") ;

  inFile.open(sFileName.c_str()) ;
  if (!inFile)
  {
  	string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile")
                                    + string(" ") + sFileName ;
    erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  string sData = string("") ;

  while (!inFile.eof())
  {
    string sLine ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sData += sLine + string(1, '\n') ;
  }
  inFile.close() ;

  if (string("") == sData)
  {
    erreur("Il n'y a pas de données à envoyer.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // Ajout des balises de début et de fin de message
  string sBaliseDeb = string("<pmed_message senderid=\"") + sUserID + string("\">") + string("\n") ;
  string sBaliseFin = string("</pmed_message>") + string("\n") ;
  sData = sBaliseDeb + sData + sBaliseFin ;

  // récupération des paramètres de emailXXX.dat
  if (false == pUser->initMail())
  {
    erreur("Envoi des e-mails impossible.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  string sMessage = string("Vous devez être connectés à internet pour que ") + pContexte->getSuperviseur()->getAppName().c_str() + string(" envoie vos e-mails automatiquement. Etes-vous connecté en ce moment ?") ;
  int idRet = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
  if (IDYES != idRet)
    return ;

  bool bSuccess ;

  NSMailParams* pMail = pUser->getRegularMail() ;

  if (NSMAIL_MAPI == pMail->getSendingModality())
  {
    NSMapiCtrl Mapi(pContexte, 0, pMail) ;
    Mapi.Connect() ;
    if (false == Mapi.Verify())
      return ;

    Mapi.ComposeMessage(pMail->getSender(), _sMailAdress, "Message", sData) ;
    ULONG lResult = Mapi.SendMail() ;
    Mapi.Disconnect() ;

    if (lResult == SUCCESS_SUCCESS)
      bSuccess = true ;
    else
      bSuccess = false ;
  }
  else
  {
    TIndySMTP * pXSMTP = new TIndySMTP(NULL, pContexte, 0, pMail) ;
    pXSMTP->Show() ;
    pXSMTP->Connect() ;
    if (false == pXSMTP->IsConnected())
    {
      pXSMTP->Close() ;
      delete pXSMTP ;
      return  ;
    }

    pXSMTP->ComposeSimpleMessage(pMail->getSender(), pMail->getName(),
                                 pMail->getFrom(), pMail->getReplyTo(),
                                 _sMailAdress, string("Message"), sData) ;
    pXSMTP->SendMail() ;
    while (pXSMTP->Attendre())
      ;

    if (pXSMTP->Continuer())
      bSuccess = true ;
    else
      bSuccess = false ;

    pXSMTP->Disconnect() ;
    pXSMTP->Close() ;
    delete pXSMTP ;
  }

  sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  if (bSuccess)
  {
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail terminé.", sCaption.c_str(), MB_OK) ;
    // vider le fichier
    // DeleteFile(sFichier.c_str()) ;    string sSaveFile = pContexte->PathName("BGLO") +                       string("dpio") + donne_date_duJour() + donne_heure() +
                       pContexte->getUtilisateur()->getNss() +
                       pContexte->getPatient()->getNss() + string(".msg") ;

    // if (!(::MoveFileEx(sFichier.c_str(), sSaveFile.c_str(), MOVEFILE_REPLACE_EXISTING)))    if (!(::MoveFile(sFileName.c_str(), sSaveFile.c_str())))    {      ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Le fichier de message ne peut être archivé.", sCaption.c_str(), MB_OK) ;      NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFileName) ;    }  }
  else
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail interrompu.", sCaption.c_str(), MB_OK) ;
}
/*
catch (ESockError& e)
{
  string sMessage = string("Connexion impossible : ") + string(e.Message.c_str()) ;
  erreur(sMessage.c_str(), standardError, 0)  ;
}
catch (ESocketError& e)
{
  erreur("Connexion impossible.", standardError, 0) ;
}*/catch (...)
{
  erreur("Le message n'a pu être envoyé.", standardError, 0) ;
}
}

bool
NSPredi::referenceFormulaire(TWindow* pDialog, string sTitle)
{
  string ps = string("Entering NSDPIO::referenceFormulaire : ") + sTitle ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  PrediModuleData ModuleData(pDialog, sTitle) ;
  if (NULL == _pPrediDlg)
  {
    ps = string("DPIO référencement de formulaire : ") + sTitle + string(" ; création de la popup") ;
    pContexte->getSuperviseur()->trace(&ps, 1) ;

try
{
    _pPrediDlg = new NSPrediDlg(pContexte->GetMainWindow(), pContexte) ;
    _pPrediDlg->Create() ;
    _pPrediDlg->referencerModule(&ModuleData, false) ;
    // S'il n'y a qu'un formulaire, ce n'est pas la peine de montrer la popup
    _pPrediDlg->ShowWindow(SW_HIDE) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSPredi::referenceFormulaire when creating NSPrediDlg: ") + e.why() ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
  string sErr = string("Exception NSPredi::referenceFormulaire when creating NSPrediDlg.") ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
	erreur(sErr.c_str(), standardError, 0) ;
}
  }
  else
  {
    ps = string("DPIO référencement de formulaire : ") + sTitle + string(" sur popup existante") ;
    pContexte->getSuperviseur()->trace(&ps, 1) ;

    _pPrediDlg->referencerModule(&ModuleData, true) ;

    // Plusieurs formulaire, il faut montrer la popup
    _pPrediDlg->ShowWindow(SW_SHOW) ;
  }

  _iPrediState = stateQuestions ;

  ps = string("Quitting NSPredi::referenceFormulaire : ") + sTitle ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  return true ;
}

void
// NSDPIO::fermeFormulaire(TWindow * pDialog)
NSPredi::fermeFormulaire(void * pDialog)
{
  if (NULL == _pPrediDlg)
    _iPrediState = stateMessage ;
  else
    _pPrediDlg->fermetureModule(pDialog) ;

  // if (pDialog && pDialog->IsWindow())
  //  pDialog->CloseWindow() ;
}

bool
NSPredi::referenceDecisionTree(TWindow* pDialog, string sTitle)
{
  _aOpenDecisionTrees.push_back(new PrediModuleData(pDialog, sTitle)) ;
  return true ;
}

void
// NSDPIO::fermeFormulaire(TWindow * pDialog)
NSPredi::fermeDecisionTree(void * pDialog)
{
  if (_aOpenDecisionTrees.empty())
		return ;

  ArrayPrediDataIter it = _aOpenDecisionTrees.begin() ;
  for ( ; (_aOpenDecisionTrees.end() != it) && ((*it)->getWindow() != pDialog) ; it++)
    ;
  if (_aOpenDecisionTrees.end() == it)
    return ;

/*
  OWL::TControl* pControl = (*it)->getControl() ;
  if (pControl)
    delete pControl ;
*/

  delete *it ;
  _aOpenDecisionTrees.erase(it) ;
}

// -----------------------------------------------------------------------------
//  Description :	Charge les paramètres
// -----------------------------------------------------------------------------
bool
NSPredi::loadParams()
{
	ifstream inFile ;

	NSUtilisateurChoisi * pUtilisat = pContexte->getUtilisateur() ;
	if (pUtilisat)
	{
    string sFichier = string("dpio.dat") ;

  	bool bFileOpened = pUtilisat->OpenUserFile(&inFile, sFichier, string("dpio[user].dat"), pContexte->PathName("FGLO")) ;
		if ((false == bFileOpened) || !inFile)
		{
  		string sErrMess = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sFichier ;
			erreur(sErrMess.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    	return false ;
		}
	}
	else
	{
		inFile.open("perso\\dpio.dat") ;
		if (!inFile)
			return false ;
	}

	string sData = string("") ;
	while (!inFile.eof())
	{
    string sLine ;
  	getline(inFile, sLine) ;
    if (string("") != sLine)
    	sData += sLine + string(1, '\n') ;
	}
	inFile.close() ;

	// boucle de chargement des attributs
  size_t iLenData = strlen(sData.c_str()) ;

  size_t i = 0 ;
	while (i < iLenData)
	{
		string sNomAttrib = string("") ;
		string sValAttrib = string("") ;

		while ((i < iLenData) && (' ' != sData[i]) && ('\t' != sData[i]))
			sNomAttrib += pseumaj(sData[i++]) ;
		while ((i < iLenData) && ((' ' == sData[i]) || ('\t' == sData[i])))
			i++ ;

		while ((i < iLenData) && ('\n' != sData[i]))
			sValAttrib += sData[i++] ;

		i++ ;

		if 		((sNomAttrib == "INTERFACE_MINIMALE") && (string("") != sValAttrib))
      _bMinimalInterface = IsNotNo(sValAttrib) ;

		else if ((sNomAttrib == "ACTIF") && (string("") != sValAttrib))
      _bPrediActif = IsNotNo(sValAttrib) ;

    else if ((sNomAttrib == "RVDB") && (string("") != sValAttrib))
      _bRVDBactif = IsNotNo(sValAttrib) ;

		else if ((string("LOG_WINDOW") == sNomAttrib) && (string("") != sValAttrib))
      _bLogPage = IsNotNo(sValAttrib) ;

		else if ((sNomAttrib == "GERE_MESSAGES") && (string("") != sValAttrib))
      _bSendPage = IsNotNo(sValAttrib) ;

		else if ((sNomAttrib == "MAIL_MESSAGES") && (string("") != sValAttrib))
			_sMailAdress = sValAttrib ;

		else if ((sNomAttrib == "RETURN_CLOSES") && (string("") != sValAttrib))
      _bReturnCloses = IsNotNo(sValAttrib) ;

		else if ((sNomAttrib == "DPIO_PRECOCHE") && (string("") != sValAttrib))
      _bPrediPreCoche = IsNotNo(sValAttrib) ;

		else if ((sNomAttrib == "X_POS") && (string("") != sValAttrib))
			_iPosX = atoi(sValAttrib.c_str()) ;
		else if ((sNomAttrib == "Y_POS") && (string("") != sValAttrib))
			_iPosY = atoi(sValAttrib.c_str()) ;
		else if ((sNomAttrib == "DELTA_X") && (string("") != sValAttrib))
			_iDeltaPosX = atoi(sValAttrib.c_str()) ;
		else if ((sNomAttrib == "DELTA_Y") && (string("") != sValAttrib))
			_iDeltaPosY = atoi(sValAttrib.c_str()) ;

		else if ((sNomAttrib == "NOM_MODULE") && (string("") != sValAttrib))
			_sNomModule = sValAttrib ;

		else if ((sNomAttrib == "OEM_MODULE") && (string("") != sValAttrib))
			_sOEMModule = sValAttrib ;

		else if ((sNomAttrib == "OEM_LINK") && (string("") != sValAttrib))
			_sOEMWeb = sValAttrib ;
	}

	return true ;
}

bool
NSPredi::sendMessage()
{
try
{
  // Traitement du message ancien format
	string sFich0 = string("dpio") + pContexte->getUtilisateur()->getNss() + string(".msg") ;
  sFich0 = pContexte->PathName("BGLO") + sFich0 ;

  if (NsFileExists(sFich0))
  {
    nsExportParseurOld ExpParseur(pContexte) ;
    if (!(ExpParseur.open(sFich0)))
    {
      erreur("Les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    CmessageExportOld * pMessage = ExpParseur.pMessage ;

    if (pMessage->vect_val.empty())
    {
      erreur("Le fichier est vide ; les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    int iNumActe = 1 ;
    for (ValIter ival = pMessage->vect_val.begin() ; pMessage->vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_EX_PATIENT)
      {
        CpatientExportOld * pPat = dynamic_cast<CpatientExportOld *>((*ival)->getObject()) ;
        if (pPat)
        {
          pPat->createSFMGfiles(iNumActe) ;
          iNumActe++ ;
        }
      }
    }

    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Génération des fichiers txt terminée pour le fichier ancienne version.", sCaption.c_str(), MB_OK) ;

    // On renomme le fichier
    //
    string sSaveFile = pContexte->PathName("BGLO") +
		                   string("dpio") + pContexte->getUtilisateur()->getNss() +
                       string("-") + donne_date_duJour() + donne_heure() +
                       string(".msg") ;
    //if (!(::MoveFileEx(sFich0.c_str(), sSaveFile.c_str(), MOVEFILE_REPLACE_EXISTING)))    if (!(::MoveFile(sFich0.c_str(), sSaveFile.c_str())))    {      ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Le fichier de message ne peut être archivé.", sCaption.c_str(), MB_OK) ;      NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFich0) ;    }
  }

  // Traitement des données nouveau format

  // V1
	string sFichier = string("dpio1-") + pContexte->getUtilisateur()->getNss() + string(".msg") ;
  sFichier = pContexte->PathName("BGLO") + sFichier ;

  if (NsFileExists(sFichier))
  {
    nsExportParseur ExpParseur(pContexte, "1") ;
    if (!(ExpParseur.open(sFichier)))
    {
      erreur("Les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    CmessageExport* pMessage = ExpParseur.pMessage ;

    if (pMessage->vect_val.empty())
    {
      erreur("Le fichier est vide ; les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    for (ValIter ival = pMessage->vect_val.begin() ; pMessage->vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_EX_PATIENT)
      {
        CpatientExport * pPat = dynamic_cast<CpatientExport *>((*ival)->getObject()) ;
        if (pPat)
          pPat->createSFMGfiles() ;
      }
    }

    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Génération des fichiers txt terminée pour le fichier dpio1.", sCaption.c_str(), MB_OK) ;

    // On renomme le fichier
    //
    string sSaveFile = pContexte->PathName("BGLO") +
		                   string("dpio1-") + pContexte->getUtilisateur()->getNss() +
                       string("-") + donne_date_duJour() + donne_heure() +
                       string(".msg") ;

    //if (!(::MoveFileEx(sFichier.c_str(), sSaveFile.c_str(), MOVEFILE_REPLACE_EXISTING)))    if (!(::MoveFile(sFichier.c_str(), sSaveFile.c_str())))    {      ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Le fichier de message ne peut être archivé.", sCaption.c_str(), MB_OK) ;      NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichier) ;    }
  }

  // V2
	sFichier = string("dpio2-") + pContexte->getUtilisateur()->getNss() + string(".msg") ;
  sFichier = pContexte->PathName("BGLO") + sFichier ;

  nsExportParseur ExpParseur(pContexte, "2") ;
  if (!(ExpParseur.open(sFichier)))
  {
    erreur("Les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  CmessageExport* pMessage = ExpParseur.pMessage ;

  if (pMessage->vect_val.empty())
  {
    erreur("Le fichier est vide ; les données n'ont pas été traitées.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  for (ValIter ival = pMessage->vect_val.begin() ; pMessage->vect_val.end() != ival ; ival++)
  {
    if ((*ival)->getLabel() == LABEL_EX_PATIENT)
    {
      CpatientExport * pPat = dynamic_cast<CpatientExport *>((*ival)->getObject()) ;
      if (pPat)
        pPat->createSFMGfiles() ;
    }
  }

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Génération des fichiers txt terminée.", sCaption.c_str(), MB_OK) ;

  // On renomme le fichier
  //
  string sSaveFile = pContexte->PathName("BGLO") +
	                   string("dpio2-") + pContexte->getUtilisateur()->getNss() +
                     string("-") + donne_date_duJour() + donne_heure() +
                     string(".msg") ;

  // if (!(::MoveFileEx(sFichier.c_str(), sSaveFile.c_str(), MOVEFILE_REPLACE_EXISTING)))  if (!(::MoveFile(sFichier.c_str(), sSaveFile.c_str())))  {    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Le fichier de message ne peut être archivé.", sCaption.c_str(), MB_OK) ;    NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichier) ;  }

  return true ;

/*
  // Ajout des balises de début et de fin de message
  string sBaliseDeb = string("<pmed_message senderid=\"") + sPrometheUserId + string("\">") + string("\n") ;
  string sBaliseFin = string("</pmed_message>") + string("\n") ;
  sData = sBaliseDeb + sData + sBaliseFin ;

  // récupération des paramètres de emailXXX.dat
  if (!pContexte->getUtilisateur()->initMail())
  {
    erreur("Envoi des e-mails impossible.", 0, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sCaption = string("Message ") + pContexte->getSuperviseur()->sAppName.c_str() ;
  string sMessage = string("Vous devez être connectés à internet pour que ") + pContexte->getSuperviseur()->sAppName.c_str() + string(" envoie vos e-mails automatiquement. Etes-vous connecté en ce moment ?") ;
  int idRet = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;
  if (idRet != IDYES)
    return false ;

  bool bSuccess ;

  if (pContexte->getUtilisateur()->pMail->iTypeEnvoi == NSMAIL_MAPI)
  {
    NSMapiCtrl * pMapi = new NSMapiCtrl(pContexte, 0, pContexte->getUtilisateur()->pMail) ;
    pMapi->Connect() ;
    if (!(pMapi->Verify()))
    {
      delete pMapi ;
      return false ;
    }
    pMapi->ComposeMessage(pContexte->getUtilisateur()->pMail->sMailExp, sPrometheMail, "Message", sData) ;
    ULONG lResult = pMapi->SendMail() ;
    pMapi->Disconnect() ;
    delete pMapi ;

    if (lResult == SUCCESS_SUCCESS)
      bSuccess = true ;
    else
      bSuccess = false ;
  }
  else
  {
    TControlSMTP * pXSMTP = new TControlSMTP(NULL, pContexte, 0, pContexte->getUtilisateur()->pMail) ;
    pXSMTP->Show() ;
    pXSMTP->Connect() ;
    if (!(pXSMTP->bConnected))
    {
      pXSMTP->Close() ;
      delete pXSMTP ;
      return false ;
    }

    pXSMTP->ComposeSimpleMessage(pContexte->getUtilisateur()->pMail->sMailExp, sPrometheMail, "Message", sData) ;
    pXSMTP->SendMail() ;
    while (pXSMTP->Attendre())
      ;

    if (pXSMTP->Continuer())
      bSuccess = true ;
    else
      bSuccess = false ;

    pXSMTP->Disconnect() ;
    pXSMTP->Close() ;
    delete pXSMTP ;
  }

  sCaption = string("Message ") + pContexte->getSuperviseur()->sAppName.c_str() ;
  if (bSuccess)
  {
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail terminé.", sCaption.c_str(), MB_OK) ;
    // vider le fichier
    DeleteFile(sFichier.c_str()) ;  }
  else
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail interrompu.", sCaption.c_str(), MB_OK) ;
}
catch (ESockError& e)
{
  string sMessage = string("Connexion impossible : ") + string(e.Message.c_str()) ;
  erreur(sMessage.c_str(), standardError, 0) ;
}
catch (ESocketError& e)
{
  erreur("Connexion impossible.", standardError, 0) ;
}*/}catch (...)
{
  erreur("Le message n'a pu être envoyé.", standardError, 0) ;
  return false ;
}
}

void
NSPredi::LogDocument(NSDocumentInfo* pNSDocumentInfo,
                     const NSPatPathoArray* pNSPatPathoArray, bool bIsNew)
{
  if (NULL == pNSDocumentInfo)
    return ;

  // Don't recurse and log the log
  //
  if ((pNSDocumentInfo->getTypeSem() == string("ZSHTM")) &&
      (pNSDocumentInfo->getFichier() == _logDocumentFileName))
    return ;

  string sIndex    = pNSDocumentInfo->getID() ;
  string sHtmlText = pContexte->getSuperviseur()->GetDocAsHtml(*pNSDocumentInfo, pContexte, true, true) ;

  _Papyrus.LogDocument(sIndex, pNSDocumentInfo, pNSPatPathoArray, sHtmlText) ;

  AppendLogFile() ;
}

#endif // !__EPIBRAIN__


