// -----------------------------------------------------------------------------// nsepisodview.cpp
// -----------------------------------------------------------------------------
// Document/Vues Capture Données Utilisateur
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: pameline $
// $Date: 2013/07/21 17:05:13 $
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


#define __NS_EPISODVIEW_CPP

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include <owl/uihelper.h>

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"		  // pour les formules
#include "nautilus\nsresour.h"      // pour AutoWordDialog
#include "nautilus\nsvisual.h"
#include "nautilus\nshistdo.h"
#include "nsbb\nsfltbtnga.h"

#include "nautilus\nsepicap.h"
#include "nautilus\nsepicap.rh"
#include "nautilus\nsepisodview.h"


// -----------------------------------------------------------------------------
//
// Méthodes de NSEpisod Document/Vue
//
// -----------------------------------------------------------------------------


// --------------------------------------------------------------------------
//
// Class NSEpisodDocument
//
// --------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSEpisodDocument::NSEpisodDocument(TDocument* parent, EpisodusModel *pMod, NSContexte* pCtx)
  : TDocument(parent),
    NSRoot(pCtx)
{
  pModel = new EpisodusModel(*pMod) ;
}


NSEpisodDocument::~NSEpisodDocument()
{
  delete pModel ;
}


// -----------------------------------------------------------------------------
//
// Class NSEpisodView
//
// -----------------------------------------------------------------------------

const int IDEpisodusList = 0x100 ;

// -----------------------------------------------------------------------------
// Table de réponses de la classe NSEpisodView
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSEpisodView, TWindowView)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(IDEpisodusList, DispInfoEpisodus),
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSEpisodView::NSEpisodView(NSEpisodDocument &doc, TWindow *parent)
  : TWindowView(doc, parent)
{
	pDocEpisod = &doc ;
	pMyListWindow = new NSEpisodListWindow(this, IDEpisodusList, 0, 0, 0, 0) ;

	pDocEpisod->pContexte->getEpisodus()->pViewData = this ;

	pToolBar = 0 ;
	pPrinter = 0 ;
	bSetupToolBar = true ;
}


// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSEpisodView::~NSEpisodView()
{
  if (pPrinter)
    delete pPrinter;
  if (pDocEpisod->pContexte->getEpisodus()->pViewData)
    pDocEpisod->pContexte->getEpisodus()->pViewData = 0 ;
}


// -----------------------------------------------------------------------------
// GetWindow renvoie this (à redéfinir car virtual dans TView)
// -----------------------------------------------------------------------------
TWindow*
NSEpisodView::GetWindow()
{
  return (TWindow *)(this) ;
}
// -----------------------------------------------------------------------------
// Appel de la fonction de remplissage de la vue
// -----------------------------------------------------------------------------
void
NSEpisodView::SetupWindow()
{
  TWindowView::SetupWindow() ;
  SetupColumns() ;
  AfficheListe() ;
}


// -----------------------------------------------------------------------------
// Initialisation des colonnes de la ListWindow
// -----------------------------------------------------------------------------
void
NSEpisodView::SetupColumns()
{
  pMyListWindow->InsertColumn(0, TListWindColumn("ID", 50)) ;
  pMyListWindow->InsertColumn(1, TListWindColumn("Choix", 50)) ;
  pMyListWindow->InsertColumn(2, TListWindColumn("AppClassChild", 50)) ;
  pMyListWindow->InsertColumn(3, TListWindColumn("AppTextChild", 150)) ;
  pMyListWindow->InsertColumn(4, TListWindColumn("Coordinates", 150)) ;
  pMyListWindow->InsertColumn(5, TListWindColumn("Correspondence", 100)) ;
  pMyListWindow->InsertColumn(6, TListWindColumn("Classification", 100)) ;
  pMyListWindow->InsertColumn(7, TListWindColumn("Format", 100)) ;
  pMyListWindow->InsertColumn(8, TListWindColumn("Unit", 100)) ;
  pMyListWindow->InsertColumn(9, TListWindColumn("AppWindow", 50)) ;
  pMyListWindow->InsertColumn(10, TListWindColumn("AppWndChild", 50)) ;
}


// -----------------------------------------------------------------------------// Affichage des éléments de la liste// -----------------------------------------------------------------------------
void
NSEpisodView::AfficheListe()
{
  pMyListWindow->DeleteAllItems() ;
  pMyListWindow->AfficheListe() ;
}


void
NSEpisodView::AfficheModel()
{
  pMyListWindow->DeleteAllItems() ;
  pMyListWindow->AfficheModel() ;
}


void
NSEpisodView::DispInfoEpisodus(TLwDispInfoNotify& dispInfo)
{
	const int       BufLen = 255 ;
	static char     buffer[BufLen] ;
	TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item ;

	int index = dispInfoItem.GetIndex() ;
#ifndef _EXT_CAPTURE
	EpisodusElemData *pElemData = (*(pDocEpisod->pContexte->getEpisodus()->pEpisodusData))[index] ;
#else
	EpisodusElemData *pElemData = (*(pDocEpisod->pContexte->getEpisodus()->pCaptureEngine->pEpisodusData))[index] ;
#endif

  // Affiche les informations en fonction de la colonne
  switch (dispInfoItem.GetSubItem())
  {
    case 1  : // AppWindow
              sprintf(buffer, "%d", pElemData->AppWindow) ;
              dispInfoItem.SetText(buffer) ;
              break ;
    case 2  : // AppWndChild
              sprintf(buffer, "%x", pElemData->AppWndChild) ;
              dispInfoItem.SetText(buffer) ;
              break ;
    case 3  : // AppClassChild
              dispInfoItem.SetText(pElemData->sClassChild.c_str()) ;
              break ;
    case 4  : // AppTextChild
              dispInfoItem.SetText(pElemData->sTextChild.c_str()) ;
              break ;
    case 5  : // Coordinates
              sprintf(buffer, "%d - %d - %d - %d", pElemData->Placement.top, pElemData->Placement.left, pElemData->Placement.right, pElemData->Placement.bottom) ;
              dispInfoItem.SetText(buffer) ;
              break ;
  }
}


// -----------------------------------------------------------------------------
// Fonction CanClose : Appel de CanClose du document
// -----------------------------------------------------------------------------
bool
NSEpisodView::CanClose()
{
try
{
	EpisodusModel* pMod = pDocEpisod->pModel ;
	if ((pMod) && (pMod->bModified))
	{
#ifndef _EXT_CAPTURE
		ArrayEpisodusModelData* pM = pDocEpisod->pContexte->getEpisodus()->pModels ;
#else
		ArrayEpisodusModelData* pM = pDocEpisod->pContexte->getEpisodus()->pCaptureEngine->pModels ;
#endif

    if (pMod->sNom != "")
    {
      // Si le modèle a déjà un nom, c'est qu'il existait déjà
      int retVal = MessageBox("Voulez vous sauvegarder les modifications ?", "Episodus", MB_YESNO) ;
      if (retVal == IDYES)
      {
          ArrayEpisodusModelIter i = pM->begin() ;
          for ( ; (i != pM->end()) && ((*i)->sNom != pMod->sNom) ; i++)
            ;

          if (i == pM->end())
            erreur("Modèle introuvable", standardError, 0) ;
          else
            *(*i) = *pMod ;

          pDocEpisod->pContexte->getEpisodus()->ExportModels() ;
      }
    }
    else
    {
      // Nouveau modèle
#ifndef _EXT_CAPTURE
      NSModelDlg* pRefDlg = new NSModelDlg(this, pDocEpisod->pContexte, pMod) ;
#else
      ArrayEpisodusModelData* pSubM = pDocEpisod->pContexte->getSuperviseur()->getEpisodus()->pCaptureEngine->pSubModels ;
      NSModelDlg* pRefDlg = new NSModelDlg(this, pMod, pM, pSubM, pDocEpisod->pContexte->PathName("FGLO")) ;
#endif
      pRefDlg->Execute() ;
    }
	}

	TMyApp *pMyApp = pDocEpisod->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;
	bSetupToolBar = false ;
	return true ;
}
catch (...)
{
  erreur("Exception NSEpisodView::CanClose.", standardError, 0) ;
  return false ;
}
}


bool
NSEpisodView::VnIsWindow(HWND hWnd)
{
  return HWindow == hWnd ;
}


// -----------------------------------------------------------------------------
// fonction permettant de prendre toute la taille de TWindow par la TListWindow
// -----------------------------------------------------------------------------
void
NSEpisodView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
  pMyListWindow->MoveWindow(GetClientRect(), true) ;
}

// -----------------------------------------------------------------------------// fonction EVSetFocus// -----------------------------------------------------------------------------voidNSEpisodView::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp* pMyApp = pDocEpisod->pContexte->getSuperviseur()->getApplication() ;
	TWindowView::EvSetFocus(hWndLostFocus) ;
	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}
}


// -----------------------------------------------------------------------------
// SetupToolBar
// -----------------------------------------------------------------------------
void
NSEpisodView::SetupToolBar()
{
  TMyApp* pMyApp = pDocEpisod->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_EPISOD_CONTEXT, IDC_EPISOD_CONTEXT, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_EPISOD_CLASSIF, IDC_EPISOD_CLASSIF, TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->LayoutSession() ;
}


DEFINE_RESPONSE_TABLE1(NSEpisodListWindow, TListWindow)
  EV_VN_ISWINDOW,
  EV_LVN_GETDISPINFO(IDEpisodusList, DispInfoEpisodus),
  EV_WM_LBUTTONDOWN,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDC_EPISOD_CLASSIF,  EpisodSetClassif),
  EV_COMMAND(IDC_EPISOD_CONTEXT,  EpisodSetContext),
  EV_COMMAND(IDC_EPISOD_FORMAT,   EpisodSetFormat),
  EV_COMMAND(IDC_EPISOD_UNIT,     EpisodSetUnit),
  EV_COMMAND(IDC_EPISOD_SUB,      CreateSubModel),
  EV_COMMAND(IDC_EPISOD_AN_P,     EpisodSetAnalyseP),
  EV_COMMAND(IDC_EPISOD_AN_O,     EpisodSetAnalyseO),
  EV_COMMAND(IDC_EPISOD_AN_PO,    EpisodSetAnalysePO),
  EV_COMMAND(IDC_EPISOD_AN_L,     EpisodSetAnalyseL),
  EV_COMMAND(IDC_EPISOD_AN_RP,    EpisodSetAnalyseRP),
  EV_COMMAND(IDC_EPISOD_AN_RO,    EpisodSetAnalyseRO),
  EV_COMMAND(IDC_EPISOD_AN_N,     EpisodSetAnalyseN),
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

NSEpisodListWindow::NSEpisodListWindow(NSEpisodView* parent, int id, int x, int y, int w, int h, TModule* module)
  : TListWindow((TWindow *)parent, id, x, y, w, h, module)
{
  pParentWindow = parent ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
}


void
NSEpisodListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}


bool
NSEpisodListWindow::VnIsWindow(HWND hWnd)
{
  return (HWindow == hWnd) ;
}


void
NSEpisodListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point);

  // int index = HitTest(info) ;
/*
  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->GardeNss(index) ;
*/
  // On appelle le Button Down de la classe mère
  TListWindow::EvLButtonDown(modkeys, point) ;
}


void
NSEpisodListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  TListWindow::EvLButtonDown(modkeys,point);

  // int count = GetItemCount();
  TLwHitTestInfo info(point);

  // int index = HitTest(info);

  OWL::TMenu      *menu       = new OWL::TMenu(HINSTANCE(*GetApplication()), TResId(IDM_EPISOD)) ;
  OWL::TPopupMenu *popupMenu  = new OWL::TPopupMenu(*menu) ;
  ClientToScreen(point) ;
  popupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete popupMenu ;
  delete menu ;

/*
  TPopupMenu *menu = new TPopupMenu() ;
  menu->AppendMenu(MF_STRING, IDC_EPISOD_CONTEXT, "Contexte") ;
  menu->AppendMenu(MF_STRING, IDC_EPISOD_FORMAT,  "Format") ;
  menu->AppendMenu(MF_STRING, IDC_EPISOD_UNIT,    "Unité") ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, IDC_EPISOD_CLASSIF, "Classification") ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, IDC_EPISOD_ANALYSE, "Type d'analyse") ;

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
*/
}


voidNSEpisodListWindow::EpisodSetContext()
{
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  int index ;
  if (GetSelIndexes(&index, 1) == 1)
  {
    string sChemin ;
    string sClassif ;
    string sLibelle ;
    EpisodusElemData* pElement = pMod->getElement(index) ;
    if (pElement)
      sLibelle = pElement->sTextChild ;
    else
      return ;

    ChoixCaptureContexte* pChoix = new ChoixCaptureContexte(this, pParentWindow->getDoc()->pContexte, &sChemin, &sClassif, &sLibelle, false) ;
    if (pChoix->Execute() == IDOK)
    {
      pMod->SetElement(index, sChemin) ;
      pMod->SetClassifElement(index, sClassif) ;
    }
    delete pChoix ;
    pParentWindow->AfficheModel() ;
  }
}


voidNSEpisodListWindow::EpisodSetFormat()
{
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  int index ;
  if (GetSelIndexes(&index, 1) == 1)
  {
    string sTitre ;
    string sModele ;
    ChoixCaptureFormat* pChoix = new ChoixCaptureFormat(this, pParentWindow->getDoc()->pContexte, &sTitre, &sModele) ;
    if (pChoix->Execute() == IDOK)
      pMod->SetFormatElement(index, sTitre) ;
    delete pChoix ;
    pParentWindow->AfficheModel() ;
  }
}


void
NSEpisodListWindow::EpisodSetUnit()
{
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  int index ;
  if (GetSelIndexes(&index, 1) == 1)
  {
    string sUnit ;
    ChoixCaptureUnit* pChoix = new ChoixCaptureUnit(this, pParentWindow->getDoc()->pContexte, &sUnit) ;
    if (pChoix->Execute() == IDOK)
      pMod->SetUnitElement(index, sUnit) ;
    delete pChoix ;
    pParentWindow->AfficheModel() ;
  }
}


void
NSEpisodListWindow::EpisodSetAnalyse(int iAnalyseType)
{
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  int iNbElements = GetItemCount() ;
  int* iSelected  = new int[iNbElements+1] ;
  int iNbSelected = GetSelIndexes(iSelected, iNbElements) ;

  for (int i = 0; i < iNbSelected; i++)
  {
    int iIndex = iSelected[i] ;
    pMod->SetAnalyseTypeElement(iIndex, iAnalyseType) ;
  }
  pParentWindow->AfficheModel() ;

  delete[] iSelected ;

/*
  int index ;
  if (GetSelIndexes(&index, 1) == 1)
  {
    pMod->SetAnalyseTypeElement(index, iAnalyseType) ;
    pParentWindow->AfficheModel() ;
  }
*/
}


void
NSEpisodListWindow::CreateSubModel()
{
  NSEpisodDocument* pDoc = pParentWindow->getDoc() ;
  if (!pDoc)
    return ;
  NSEpisodus* pEpisod = pDoc->pContexte->getEpisodus() ;
  if (!pEpisod)
    return ;
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  string sModelName = "" ;
  ChoixCaptureModelName* pNameDlg = new ChoixCaptureModelName(this, pDoc->pContexte, &sModelName) ;
  pNameDlg->Execute() ;
  delete pNameDlg ;

  if (sModelName == "")
    return ;

  EpisodusModel* pSubModel = new EpisodusModel() ;
  pSubModel->sNom = sModelName ;

  int iNbElements = GetItemCount() ;
  int* iSelected  = new int[iNbElements+1] ;
  int iNbSelected = GetSelIndexes(iSelected, iNbElements) ;

  for (int i = 0 ; i < iNbSelected ; i++)
  {
    int iIndex = iSelected[i] ;
    pSubModel->push_back(new EpisodusElemData((*pMod)[iIndex])) ;
  }
  delete[] iSelected ;


#ifndef _EXT_CAPTURE
	pEpisod->pSubModels->push_back(pSubModel) ;
#else
	pEpisod->pCaptureEngine->pSubModels->push_back(pSubModel) ;
#endif
}


void
NSEpisodListWindow::EpisodSetAnalyseP()
{
  EpisodSetAnalyse(EpisodusElemData::analysePlacement) ;
}


void
NSEpisodListWindow::EpisodSetAnalyseO()
{
  EpisodSetAnalyse(EpisodusElemData::analyseOrder) ;
}


void
NSEpisodListWindow::EpisodSetAnalysePO()
{
  EpisodSetAnalyse(EpisodusElemData::analysePlacementAndOrder) ;
}


void
NSEpisodListWindow::EpisodSetAnalyseL()
{
  EpisodSetAnalyse(EpisodusElemData::analyseLibelle) ;
}


void
NSEpisodListWindow::EpisodSetAnalyseRP()
{
  EpisodSetAnalyse(EpisodusElemData::analyseRelativePlacement) ;
}

void
NSEpisodListWindow::EpisodSetAnalyseRO()
{
  EpisodSetAnalyse(EpisodusElemData::analyseRelativeOrder) ;
}

void
NSEpisodListWindow::EpisodSetAnalyseN()
{
  EpisodSetAnalyse(EpisodusElemData::analyseNever) ;
}


void
NSEpisodListWindow::EpisodSetClassif()
{
  EpisodusModel* pMod = pParentWindow->getDoc()->pModel ;
  if (!pMod)
    return ;

  int index ;
  if (GetSelIndexes(&index, 1) == 1)
  {
    pMod->SetClassifElement(index, "6CISP") ;
    pParentWindow->AfficheModel() ;
  }
}


void
NSEpisodListWindow::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp* pMyApp = pParentWindow->getDoc()->pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (pParentWindow->GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar();
		pMyApp->SetToolBarWindow(pParentWindow->GetWindow());
	}
}


void
NSEpisodListWindow::SetupToolBar()
{
	TMyApp* pMyApp = pParentWindow->getDoc()->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_EPISOD_CONTEXT, IDC_EPISOD_CONTEXT, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_EPISOD_CLASSIF, IDC_EPISOD_CLASSIF, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->LayoutSession() ;
}


void
NSEpisodListWindow::DispInfoEpisodus(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem*)&dispInfo.item ;

  string sLang = "" ;
  if (pParentWindow->getDoc()->pContexte)
    sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;

  EpisodusModel* pmyArraySpyData = pParentWindow->getDoc()->pModel ;

  int index = dispInfoItem.GetIndex() ;
  //EpisodusElemData *pElemData = (*(pContexte->getSuperviseur()->pEpisodus->pCurrentModel))[index];
  EpisodusElemData *pElemData = (*pmyArraySpyData)[index] ;

  string sClassif     = "" ;
  string sLibel       = "" ;
  string sUnitLibelle = "" ;

  // Affiche les informations en fonction de la colonne
  switch (dispInfoItem.GetSubItem())
  {
    case 1  : // AppClassChild
              switch(pElemData->iModeAnalyse)
              {
                case EpisodusElemData::analysePlacement         : strcpy(buffer, "P") ;   break ;
                case EpisodusElemData::analyseOrder             : strcpy(buffer, "O") ;   break ;
                case EpisodusElemData::analysePlacementAndOrder : strcpy(buffer, "P+O") ; break ;
                case EpisodusElemData::analyseLibelle           : strcpy(buffer, "L") ;   break ;
                case EpisodusElemData::analyseRelativeOrder     : strcpy(buffer, "R-O") ; break ;
                case EpisodusElemData::analyseRelativePlacement : strcpy(buffer, "R-P") ; break ;
                case EpisodusElemData::analyseNever             : strcpy(buffer, "N") ;   break ;
                default                                         : strcpy(buffer, "") ;    break ;
              }
              dispInfoItem.SetText(buffer) ;
              break ;
    case 2  : // AppClassChild
              dispInfoItem.SetText(pElemData->sClassChild.c_str()) ;
              break ;
    case 3  : // AppTextChild
              dispInfoItem.SetText(pElemData->sTextChild.c_str()) ;
              break ;
    case 4  : // Coordinates
              sprintf(buffer, "%d - %d - %d - %d", pElemData->Placement.top, pElemData->Placement.left, pElemData->Placement.right, pElemData->Placement.bottom) ;
              dispInfoItem.SetText(buffer) ;
              break ;
    case 5  : // Correspondences
              if (pElemData->sCorrespondence != "")
                sLibel = donneContexte(pElemData->sCorrespondence) ;
              dispInfoItem.SetText(sLibel.c_str()) ;
              break ;
    case 6  : // Classification
              if      (pElemData->sClassifier == "6CISP")
                sClassif = "CISP 2" ;
              else if (pElemData->sClassifier == "6CIMA")
                sClassif = "CIM 10" ;
              dispInfoItem.SetText(sClassif.c_str()) ;
              break ;
    case 7  : // Format
              dispInfoItem.SetText(pElemData->sFormat.c_str()) ;
              break ;
    case 8  : // Unité
              if (pElemData->sUnit != "")
              {
                string sLang = "" ;
                if ((pParentWindow->getDoc()->pContexte) && (pParentWindow->getDoc()->pContexte->getUtilisateur()))
                {
                  sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;
                  pParentWindow->getDoc()->pContexte->getDico()->donneLibelle(sLang, &(pElemData->sUnit), &sUnitLibelle) ;
                }
              }
              dispInfoItem.SetText(sUnitLibelle.c_str()) ;
              break ;
    case 9  : // AppWindow
              sprintf(buffer, "%d", pElemData->AppWindow) ;
              dispInfoItem.SetText(buffer) ;
              break ;
    case 10 : // AppWndChild
              sprintf(buffer, "%x", pElemData->AppWndChild) ;
              dispInfoItem.SetText(buffer) ;
              break ;
  } // fin du switch
}


void
NSEpisodListWindow::AfficheListe()
{
	EpisodusModel* pmyArraySpyData = pParentWindow->getDoc()->pModel ;
	// EpisodusData *pmyArraySpyData = pContexte->getSuperviseur()->pEpisodus->pEpisodusData;

	if ((!pmyArraySpyData) || (pmyArraySpyData->empty()))
		return ;

	string sLang = string("") ;
	if (pParentWindow->getDoc()->pContexte)
		sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;

	int i = 0 ;
	for (EpisodusDataIter myData = pmyArraySpyData->begin() ; myData != pmyArraySpyData->end() ; myData++)
	{
		char buffer[1024] ;
		EpisodusElemData *pElemData = (*myData) ;
		sprintf(buffer, "%d", i) ;

    TListWindItem Item(buffer) ;
    Item.SetText(buffer) ;
    int index = AddItem(Item) ;

    switch(pElemData->iModeAnalyse)
    {
      case EpisodusElemData::analysePlacement         : strcpy(buffer, "P") ;   break ;
      case EpisodusElemData::analyseOrder             : strcpy(buffer, "O") ;   break ;
      case EpisodusElemData::analysePlacementAndOrder : strcpy(buffer, "P+O") ; break ;
      case EpisodusElemData::analyseLibelle           : strcpy(buffer, "L") ;   break ;
      case EpisodusElemData::analyseRelativeOrder     : strcpy(buffer, "R-O") ; break ;
      case EpisodusElemData::analyseRelativePlacement : strcpy(buffer, "R-P") ; break ;
      case EpisodusElemData::analyseNever             : strcpy(buffer, "N") ;   break ;
      default                                         : strcpy(buffer, "") ;    break ;
    }
    SetItemText(index, 1, buffer) ;
    SetItemText(index, 2, pElemData->sClassChild.c_str()) ;
    SetItemText(index, 3, pElemData->sTextChild.c_str()) ;
    sprintf(buffer, "%d - %d - %d - %d", pElemData->Placement.top, pElemData->Placement.left, pElemData->Placement.right, pElemData->Placement.bottom) ;
    SetItemText(index, 4, buffer) ;
//    SetItemText(index, 6, pElemData->sCorrespondence.c_str()) ;
    if (pElemData->sCorrespondence != "")
    {
      string sLibel = donneContexte(pElemData->sCorrespondence) ;
      SetItemText(index, 5, sLibel.c_str()) ;
    }
    else
        SetItemText(index, 5, "") ;

    if (pElemData->sClassifier != "")
    {
      if      (pElemData->sClassifier == "6CISP")
        SetItemText(index, 6, "CISP 2") ;
      else if (pElemData->sClassifier == "6CIMA")
        SetItemText(index, 6, "CIM 10") ;
    }
    else
      SetItemText(index, 6, "") ;

    SetItemText(index, 7, pElemData->sFormat.c_str()) ;
    if (pElemData->sUnit != "")
    {
      string sUnitLibelle = "" ;
      string sLang = "" ;
      if ((pParentWindow->getDoc()->pContexte) && (pParentWindow->getDoc()->pContexte->getUtilisateur()))
      {
        sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;
        pParentWindow->getDoc()->pContexte->getDico()->donneLibelle(sLang, &(pElemData->sUnit), &sUnitLibelle) ;
      }
      SetItemText(index, 8, sUnitLibelle.c_str()) ;
    }

    sprintf(buffer, "%d", pElemData->AppWindow) ;
    SetItemText(index, 9, buffer) ;
    sprintf(buffer, "%x", pElemData->AppWndChild) ;
    SetItemText(index, 10, buffer) ;
    i++ ;
  }
}


voidNSEpisodListWindow::AfficheModel(){
  EpisodusModel * pmyArraySpyData = pParentWindow->getDoc()->pModel ;
  if (pmyArraySpyData->empty())
    return ;

  //EpisodusData *myArraySpyData = pContexte->getSuperviseur()->pEpisodus->pCurrentModel;
  string sLang = string("") ;
  if (pParentWindow->getDoc()->pContexte)
    sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;

  int i = 0 ;
  for (EpisodusDataIter myData = pmyArraySpyData->begin() ; myData != pmyArraySpyData->end() ; myData++)
  {
    char buffer[1024] ;
    EpisodusElemData *pElemData = (*myData) ;
    sprintf(buffer, "%d", i) ;

    TListWindItem Item(buffer) ;
    Item.SetText(buffer) ;
    int index = AddItem(Item) ;

    switch(pElemData->iModeAnalyse)
    {
      case EpisodusElemData::analysePlacement         : strcpy(buffer, "P") ;   break ;
      case EpisodusElemData::analyseOrder             : strcpy(buffer, "O") ;   break ;
      case EpisodusElemData::analysePlacementAndOrder : strcpy(buffer, "P+O") ; break ;
      case EpisodusElemData::analyseLibelle           : strcpy(buffer, "L") ;   break ;
      case EpisodusElemData::analyseRelativeOrder     : strcpy(buffer, "R-O") ; break ;
      case EpisodusElemData::analyseRelativePlacement : strcpy(buffer, "R-P") ; break ;
      case EpisodusElemData::analyseNever             : strcpy(buffer, "N") ;   break ;
      default                                         : strcpy(buffer, "") ;    break ;
    }
    SetItemText(index, 1, buffer) ;
    SetItemText(index, 2, pElemData->sClassChild.c_str()) ;
    SetItemText(index, 3, pElemData->sTextChild.c_str()) ;
    sprintf(buffer, "%d - %d - %d - %d", pElemData->Placement.top, pElemData->Placement.left, pElemData->Placement.right, pElemData->Placement.bottom) ;
    SetItemText(index, 4, buffer) ;

    if (pElemData->sCorrespondence != "")
    {
        string sLibel = donneContexte(pElemData->sCorrespondence) ;
        SetItemText(index, 5, sLibel.c_str()) ;
    }
    else
        SetItemText(index, 5, "") ;
  //        SetItemText(index, 6, pElemData->sCorrespondence.c_str());

    if (pElemData->sClassifier != "")
    {
      if      (pElemData->sClassifier == "6CISP")
        SetItemText(index, 6, "CISP 2") ;
      else if (pElemData->sClassifier == "6CIMA")
        SetItemText(index, 6, "CIM 10") ;
    }
    else
      SetItemText(index, 6, "") ;

    SetItemText(index, 7, pElemData->sFormat.c_str()) ;

    if (pElemData->sUnit != "")
    {
      string sUnitLibelle = "" ;
      string sLang = "" ;
      if ((pParentWindow->getDoc()->pContexte) && (pParentWindow->getDoc()->pContexte->getUtilisateur()))
      {
        sLang = pParentWindow->getDoc()->pContexte->getUserLanguage() ;
        pParentWindow->getDoc()->pContexte->getDico()->donneLibelle(sLang, &(pElemData->sUnit), &sUnitLibelle) ;
      }
      SetItemText(index, 8, sUnitLibelle.c_str()) ;
    }

    sprintf(buffer, "%d", pElemData->AppWindow) ;
    SetItemText(index, 9, buffer) ;
    sprintf(buffer, "%x", pElemData->AppWndChild) ;
    SetItemText(index, 10, buffer) ;

    i++ ;
  }
}

stringNSEpisodListWindow::donneContexte(string sContexte)
{
	if (sContexte == "")
		return string("") ;

  NSContexte* pContexte = pParentWindow->getDoc()->pContexte ;
	NSSuper*    pSuper    = pContexte->getSuperviseur() ;

	string sLang = pContexte->getUserLanguage() ;

  string sLibel = "" ;
  string sousChaine ;
  string sLibelleTrouve ;

  size_t fin   = sContexte.find("/") ;
  size_t debut = 0 ;

  while (fin != string::npos)
  {
    sousChaine = string(sContexte, debut, fin - debut) ;
    pContexte->getDico()->donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
    if (sLibel != "")
      sLibel += "/" ;
    sLibel += sLibelleTrouve ;
    debut = fin + 1 ;
    fin   = sContexte.find("/", debut) ;
  }

  sousChaine = string(sContexte, debut, strlen(sContexte.c_str()) - debut) ;
  pContexte->getDico()->donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;

  if (sLibel != "")
    sLibel += "/" ;

  sLibel += sLibelleTrouve ;

  if (sLibel != "")
    sLibel[0] = pseumaj(sLibel[0]) ;

  return sLibel ;
}


// -----------------------------------------------------------------------------
//
// Class NSEpisodScreenView
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------// Table de réponses de la classe NSEpisodScreenView// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSEpisodScreenView, TWindowView)
  EV_VN_ISWINDOW,
  EV_WM_SIZE,
  EV_WM_SETFOCUS,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_EPISO_OCR_START, OCRstart),
  EV_COMMAND(CM_EPISO_BLOC_DEF,  defineBloc),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSEpisodScreenView::NSEpisodScreenView(NSEpisodDocument &doc, TWindow *parent)
                   :TWindowView(doc, parent)
{
  pDocEpisod      = &doc ;
  pointHautGauche = NS_CLASSLIB::TPoint(0, 0) ;
  pScreenMap      = pDocEpisod->pContexte->getEpisodus()->getScreenMap() ;
  pToolBar        = 0 ;
  bSetupToolBar   = true ;
}

// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSEpisodScreenView::~NSEpisodScreenView()
{
}

void
NSEpisodScreenView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	if (!pScreenMap)
		return ;
	pScreenMap->Affiche(&dc, &RectAPeindre, &pointHautGauche) ;
}

// -----------------------------------------------------------------------------
// GetWindow renvoie this (à redéfinir car virtual dans TView)
// -----------------------------------------------------------------------------
TWindow *
NSEpisodScreenView::GetWindow()
{
  return (TWindow *)(this) ;
}

// -----------------------------------------------------------------------------// Appel de la fonction de remplissage de la vue// -----------------------------------------------------------------------------
void
NSEpisodScreenView::SetupWindow()
{
	TWindowView::SetupWindow() ;
}

boolNSEpisodScreenView::CanClose()
{
	TMyApp *pMyApp = pDocEpisod->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;
	bSetupToolBar = false ;
	return true ;
}

boolNSEpisodScreenView::VnIsWindow(HWND hWnd)
{
	return HWindow == hWnd ;
}

// -----------------------------------------------------------------------------// fonction permettant de prendre toute la taille de TWindow par la TListWindow// -----------------------------------------------------------------------------
void
NSEpisodScreenView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TWindow::EvSize(sizeType, size) ;
}

// -----------------------------------------------------------------------------// fonction EVSetFocus// -----------------------------------------------------------------------------
void
NSEpisodScreenView::EvSetFocus(HWND hWndLostFocus)
{
	TMyApp* pMyApp = pDocEpisod->pContexte->getSuperviseur()->getApplication() ;
	TWindowView::EvSetFocus(hWndLostFocus) ;
	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}
}

// -----------------------------------------------------------------------------
// SetupToolBar
// -----------------------------------------------------------------------------
void
NSEpisodScreenView::SetupToolBar()
{
/*
  TMyApp* pMyApp = pDocEpisod->pContexte->getSuperviseur()->pNSApplication ;
  pMyApp->FlushControlBar() ;
  
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_NOM, IDC_EPISOD_NOM, TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_PRENOM, IDC_EPISOD_PRENOM, TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_NAISS, IDC_EPISOD_NAISS, TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_SEXE, IDC_EPISOD_SEXE, TButtonGadget::Command)) ;
  pMyApp->cb2->Insert(*new TButtonGadget(IDC_EPISOD_MEDICAL, IDC_EPISOD_MEDICAL, TButtonGadget::Command)) ;

  pMyApp->cb2->LayoutSession() ;
*/
}


void
NSEpisodScreenView::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
  TWindowView::EvLButtonDown(modkeys, point) ;
  mousePoint = point ;
  TPopupMenu *menu = new TPopupMenu() ;
  menu->AppendMenu(MF_STRING, CM_EPISO_BLOC_DEF,  "Bloc") ;
  menu->AppendMenu(MF_STRING, CM_EPISO_OCR_START, "OCR") ;
  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}


void
NSEpisodScreenView::OCRstart()
{
try
{
	if (Blocs.empty())
	{
		erreur("Aucun bloc n'a été sélectionné.", standardError, -1, HWindow) ;
		return ;
	}

	// On cherche dans quel bloc se situe la souris
	NS_CLASSLIB::TPoint point = mousePoint ;

	blocIter iBlk = Blocs.begin() ;
	for ( ; (iBlk != Blocs.end()) && (!((*iBlk)->BlocRect.Contains(point))) ; iBlk++)
    ;

	if ((iBlk == Blocs.end()))
	{
		erreur("Ce point n'appartient à aucun bloc.", standardError, -1, this->HWindow) ;
		return ;
	}

	string sResultat = "" ;
	bool (FAR *pAdresseFct)(NSContexte far *, NSOCRbloc far *, NSBitmap far *) ;

#ifndef N_TIERS
	TModule* pDCModule = new TModule("ns_ocr.dll", TRUE) ;
#else
	TModule* pDCModule = new TModule("nsm_ocr.dll", TRUE) ;
#endif
	if (!pDCModule)	{
		erreur("Impossible d'ouvrir la DLL ns_ocr.dll.", standardError, -1, pDocEpisod->pContexte->GetMainWindow()->GetHandle()) ;
		pDocEpisod->pContexte->getSuperviseur()->afficheStatusMessage("") ;
		return ;
	}

	(FARPROC)pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
	if (pAdresseFct == NULL)
	{
		delete pDCModule ;
		return ;
	}

	bool bReussi = ((*pAdresseFct)((NSContexte far *)pDocEpisod->pContexte, (NSOCRbloc far *)(*iBlk), (NSBitmap far *)pScreenMap)) ;
	delete pDCModule;
	if ((*iBlk)->sTexte == "")
		return ;

  // Ajout de ce nouvel élément au modèle
	pDocEpisod->pModel->push_back(new EpisodusElemData(0, 0, "OCR", (*iBlk)->sTexte, (*iBlk)->BlocRect, -1, "", EpisodusElemData::captureOCR, EpisodusElemData::analysePlacement)) ;

	// Rafraichissement de la vue "liste"
	TView* pView = pDocEpisod->GetViewList() ;
	do
	{
		NSEpisodView* pListView = TYPESAFE_DOWNCAST(pView, NSEpisodView) ;
		if ( pListView )
			pListView->AfficheListe() ;
		pView = pDocEpisod->NextView(pView) ;
	}
	while (pView) ;
}
catch (...)
{
	erreur("Exception NSEpisodScreenView::OCRstart", standardError, 0) ;
}
}


void
NSEpisodScreenView::defineBloc()
{
  if (!pScreenMap)
    return ;

  bool                bExist ;
  NS_CLASSLIB::TPoint point   = mousePoint ;
  NS_CLASSLIB::TColor cFond   = pScreenMap->getColor(point, &bExist) ;
  int                 iLeft   = point.X() ;
  int                 iRight  = point.X() ;
  int                 iTop    = point.Y() ;
  int                 iBottom = point.Y() ;

  bool                bGrowToTop      = true ;
  bool                bGrowToBottom   = true ;
  bool                bGrowToLeft     = true ;
  bool                bGrowToRight    = true ;

  // Algo : On fait grandir un rectangle à partir du point de départ
  // Lorsque, dans une direction l'ensemble des points ajoutés est différent de
  // la couleur du fond, on suppose qu'on a atteint le bord dans cette direction.
  // On vérifie, lors de la croissance des autres directions, que ce bord reste
  // bien exempt de pixels de la couleur du fond ; si ce n'est pas le cas, on
  // redémarre la progression dans cette direction

  bool bTourner = true ;
  while (bTourner)
  {
    // On fait grandir le rectangle d'un cran
    // Rectangle growing one step
    if ((bGrowToTop) && (iTop > 0))
      iTop-- ;
    if ((bGrowToBottom) && (iBottom < pScreenMap->getBottom()))
      iBottom++ ;
    if ((bGrowToLeft) && (iLeft > 0))
      iLeft-- ;
    if ((bGrowToRight) && (iRight < pScreenMap->getRight()))
      iRight++ ;

    // Inspection en haut - watching top line
    if (iTop > 0)
    {
      // On regarde s'il existe au moins un point de la couleur du fond sur la ligne du haut
      // We are scanning top line in order to see if it contains a pixel of background color
      NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop) ;
      int i ;
      for (i = iLeft ; i <= iRight ; i++)
      {
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && (bExist))
          break ;
        nextPoint = NS_CLASSLIB::TPoint(i, iTop) ;
      }

      // Aucun pixel de la couleur du fond
      // No background colored pixel found
      if (i > iRight)
        bGrowToTop = false ;
      else
        bGrowToTop = true ;
    }

    // Inspection en bas - watching bottom line
    if (iBottom < pScreenMap->getBottom())
    {
      NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iBottom) ;
      int i ;
      for (i = iLeft ; i <= iRight ; i++)
      {
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          break ;
        nextPoint = NS_CLASSLIB::TPoint(i, iBottom) ;
      }
      if (i > iRight)
        bGrowToBottom = false ;
      else
        bGrowToBottom = true ;
    }

    // Inspection à gauche - watching left line
    if (iLeft > 0)
    {
      NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop) ;
      int i ;
      for (i = iTop ; i <= iBottom ; i++)
      {
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          break ;
        nextPoint = NS_CLASSLIB::TPoint(iLeft, i) ;
      }
      if (i > iBottom)
        bGrowToLeft = false ;
      else
        bGrowToLeft = true ;
    }

    // Inspection à droite - watching right line
    if (iRight < pScreenMap->getRight())
    {
      NS_CLASSLIB::TPoint nextPoint = NS_CLASSLIB::TPoint(iRight, iTop) ;
      int i ;
      for (i = iTop ; i <= iBottom ; i++)
      {
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          break ;
        nextPoint = NS_CLASSLIB::TPoint(iRight, i) ;
      }
      if (i > iBottom)
        bGrowToRight = false ;
      else
        bGrowToRight = true ;
    }

    if (((!bGrowToTop)      || (iTop <= 0))                           &&
        ((!bGrowToBottom)   || (iBottom >= pScreenMap->getBottom()))  &&
        ((!bGrowToLeft)     || (iLeft <= 0))                          &&
        ((!bGrowToRight)    || (iRight >= pScreenMap->getRight())))
      bTourner = false ;
  }

  // haut non inclus
  if (!bGrowToTop)
    iTop++ ;
  // bas non inclus
  if (!bGrowToBottom)
    iBottom-- ;
  // haut non inclus
  if (!bGrowToLeft)
    iLeft++ ;
  // bas non inclus
  if (!bGrowToRight)
    iRight-- ;

/*
  // Recherche du bord gauche
  if (iLeft > 0)
  {
    // Recherche vers le haut à gauche
    bool bProgression = true ;
    while (bProgression)
    {
      nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iTop) ;

      // Recherche vers la gauche
      while ((iLeft > 0) && (pScreenMap->getColor(nextPoint, &bExist) == cFond) && (bExist))
      {
        iLeft-- ;
        if (iLeft > 0)
          nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iTop) ;
      }
      if (iTop > 0)
      {
        nextPoint = NS_CLASSLIB::TPoint(iLeft, iTop - 1) ;
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          iTop-- ;
        else
          bProgression = false ;
      }
      else
        bProgression = false ;
    }
    // Recherche vers le bas à gauche
    if (iBottom < pScreenMap->getBottom())
      bProgression = true ;
    else
      bProgression = false ;

    while (bProgression)
    {
      nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iBottom) ;

      // Recherche vers la gauche
      while ((iLeft > 0) && (pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
      {
        iLeft-- ;
        if (iLeft > 0)
          nextPoint = NS_CLASSLIB::TPoint(iLeft - 1, iBottom) ;
      }

      if (iBottom < pScreenMap->getBottom())
      {
        nextPoint = NS_CLASSLIB::TPoint(iLeft, iBottom + 1) ;
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          iBottom++ ;
        else
          bProgression = false ;
      }
      else
        bProgression = false ;
    }
  }

  // Recherche du bord droit
  if (iRight < pScreenMap->getRight())
  {
    // Recherche vers le haut à droite
    bool bProgression = true ;
    while (bProgression)
    {
      nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iTop) ;

      // Recherche vers la droite
      while ((iRight < pScreenMap->getRight()) && (pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
      {
        iRight++ ;
        if (iRight < pScreenMap->getRight())
          nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iTop) ;
      }

      if (iTop > 0)
      {
        nextPoint = NS_CLASSLIB::TPoint(iRight, iTop - 1) ;
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          iTop-- ;
        else
          bProgression = false ;
      }
      else
        bProgression = false ;
    }
    // Recherche vers le bas à droite
    if (iBottom < pScreenMap->getBottom())
      bProgression = true ;
    else
      bProgression = false ;

    while (bProgression)
    {
      nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iBottom) ;

      // Recherche vers la gauche
      while ((iRight < pScreenMap->getRight()) && (pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
      {
        iRight++ ;
        if (iRight > 0)
          nextPoint = NS_CLASSLIB::TPoint(iRight + 1, iBottom) ;
      }

      if (iBottom < pScreenMap->getBottom())
      {
        nextPoint = NS_CLASSLIB::TPoint(iRight, iBottom + 1) ;
        if ((pScreenMap->getColor(nextPoint, &bExist) == cFond) && bExist)
          iBottom++ ;
        else
          bProgression = false ;
      }
      else
        bProgression = false ;
    }
  }
*/

  NS_CLASSLIB::TRect selectRect(iLeft, iTop, iRight, iBottom) ;

  // Affichage du bloc à l'écran
  TClientDC* pWinDC = new TClientDC(HWindow) ;
  pWinDC->DrawFocusRect(selectRect) ;
  delete pWinDC ;

  // On ajoute le bloc à l'array
  Blocs.push_back(new NSOCRbloc(selectRect, cFond)) ;
}

