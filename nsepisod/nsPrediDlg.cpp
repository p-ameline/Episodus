// -----------------------------------------------------------------------------// nsPrediDlg.cpp
// -----------------------------------------------------------------------------
// Predi archetypes management components
// -----------------------------------------------------------------------------
// $Revision: 1.1 $
// $Author: pameline $
// $Date: 2013/11/11 23:12:00 $
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
# include <bwcc.h>
# include <owl/groupbox.h>
# include <owl/checkbox.h>
#endif // !__EPIBRAIN__

#include "partage\Nsglobal.h"
#include "nsepisod\nsPrediDlg.h"
#include "nautilus\nsepicap.h"

#include "nsepisod\nsepisod.h"

#include "nsepisod\nspredi.rh"

// -----------------------------------------------------------------------------
//
//		    				  Classe NSDPIODlg
//
// -----------------------------------------------------------------------------

#ifndef __EPIBRAIN__

#define IDFERMER    201
#define IDBTN       101

DEFINE_RESPONSE_TABLE1(NSPrediDlg, TDialog)
  EV_COMMAND(IDFERMER,     fermer),
	EV_COMMAND_AND_ID(IDBTN+0, activer),
  EV_COMMAND_AND_ID(IDBTN+1, activer),
  EV_COMMAND_AND_ID(IDBTN+2, activer),
  EV_COMMAND_AND_ID(IDBTN+3, activer),
  EV_COMMAND_AND_ID(IDBTN+4, activer),
  EV_COMMAND_AND_ID(IDBTN+5, activer),
  EV_COMMAND_AND_ID(IDBTN+6, activer),
  EV_COMMAND_AND_ID(IDBTN+7, activer),
  EV_COMMAND_AND_ID(IDBTN+8, activer),
  EV_COMMAND_AND_ID(IDBTN+9, activer),
  EV_COMMAND_AND_ID(IDBTN+10, activer),
  EV_COMMAND_AND_ID(IDBTN+11, activer),
  EV_COMMAND_AND_ID(IDBTN+12, activer),
END_RESPONSE_TABLE ;

NSPrediDlg::NSPrediDlg(TWindow * pere, NSContexte * pCtx)
           :TDialog(pere, "DLG_BASE_PREDI", pNSResModule),
            NSRoot(pCtx)
{
  _pCurrentWindow  = (TWindow*) 0 ;
  _bDejaSetup      = false ;
  _iNbBtns         = 0 ;

  _iBoxHeight = 12 ;    // hauteur du bouton
  _iBoxWidth  = 250 ;   // largeur du bouton
  _iBoxInterv = 2 ;     // intervalle entre deux boutons
  _iBoxTop    = 4 ;     // haut du 1er bouton à partir du haut du groupbox
  _iBoxLeft   = 5 ;     // gauche des boutons à partir de la gauche du groupbox

  _iGroupTop  = 3 ;     // haut du groupbox
  _iGroupLeft = 2 ;     // gauche du groupbox

  _iSeparLeft = 3 ;     // gauche du séparateur
  _iGB_Separ  = 3 ;     // intervalle entre le bas du groupbox et le séparateur
  _iSepar_Btn = 8 ;     // intervalle entre le séparateur et le bouton

  _iBtnHeight = 25 ;    // hauteur d'un bouton
  _iBtnWidth  = 43 ;    // hauteur d'un bouton
  _iBtnLeft   = 3 ;     // gauche du premier bouton

  _iBtn_bas   = 3 ;     // intervalle entre le bas du bouton et le bas de la boite

  _iWndWidth  = 2 * _iGroupLeft + 2 * _iBoxLeft + _iBoxWidth ;

  int iFermerLeft = (_iWndWidth - _iBtnWidth) / 2 ;
  _pFermer = new OWL::TButton(this, IDFERMER, "Fermer", iFermerLeft, _iBtn_bas, _iBtnWidth, _iBtnHeight, /*isDefault*/ false, 0) ;
  _pDip    = new OWL::TGroupBox(this, -1, "", _iSeparLeft, _iWndWidth - 2 * _iSeparLeft, 2 * _iBtn_bas + _iBtnHeight, /*TModule**/ 0) ;
  _pDip->Attr.Style |= BSS_HDIP ;

  int iHautGrp = 3 * _iBtn_bas + _iBtnHeight ;

  _pGroup = new OWL::TGroupBox(this, -1, "", _iGroupLeft, iHautGrp, _iWndWidth - 2 * _iGroupLeft, 10, /* TModule* */ 0) ;
  _pGroup->Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;

  _iHautBtns = iHautGrp + _iBoxTop ;
}

NSPrediDlg::~NSPrediDlg()
{
  NSPredi *pReferencedPredi = pContexte->getPredi() ;
  if (pReferencedPredi && (pReferencedPredi->_pPrediDlg == this))
    pReferencedPredi->_pPrediDlg = (NSPrediDlg *) 0 ;
}

void
NSPrediDlg::SetupWindow()
{
  TDialog::SetupWindow() ;
  _bDejaSetup = true ;

  /*
  if (iNbBtns < 2)
    ShowWindow(SW_HIDE) ;
  else
    ShowWindow(SW_SHOW) ;
  */
}

void
NSPrediDlg::reaffiche()
{
  if (false == _bDejaSetup)
    return ;

  if (_aData.empty())
  {
    ShowWindow(SW_HIDE) ;
    return ;
  }

  NS_CLASSLIB::TRect cvtRect ;

  int i = 0 ;
  int iXbtn = _iGroupLeft + _iBoxLeft ;
  int iYbtn = _iHautBtns ;

  for (ArrayPrediDataIter it = _aData.begin() ; _aData.end() != it ; it++)
  {
    OWL::TControl* pCtrl = (*it)->getControl() ;

    cvtRect = NS_CLASSLIB::TRect(iXbtn, iYbtn, iXbtn + _iBoxWidth, iYbtn + _iBoxHeight) ;
    MapDialogRect(cvtRect) ;
    pCtrl->SetWindowPos(HWindow, cvtRect.left, cvtRect.top, cvtRect.Width(), cvtRect.Height(), SWP_NOZORDER) ;
    // pCtrl->SetWindowPos(HWindow, iXbtn, iYbtn, pCtrl->Attr.W, pCtrl->Attr.H, SWP_NOZORDER) ;
    iYbtn += _iBoxHeight + _iBoxInterv ;
    i++ ;
  }
  int iGrpHeigth = 2 * _iBoxTop + i * _iBoxHeight + (i - 1) * _iBoxInterv ;

  MapDialogRect(cvtRect) ;

  int iHautGrp = 3 * _iBtn_bas + _iBtnHeight ;

  cvtRect = NS_CLASSLIB::TRect(_iGroupLeft, iHautGrp, _iGroupLeft + _iWndWidth - 2 * _iGroupLeft, iHautGrp + iGrpHeigth) ;
  MapDialogRect(cvtRect) ;
  _pGroup->SetWindowPos(HWindow, cvtRect.left, cvtRect.top, cvtRect.Width(), cvtRect.Height(), SWP_NOZORDER) ;

  //pGroup->SetWindowPos(HWindow, iGroupLeft, iHautGrp, iWndWidth - 2 * iGroupLeft, iGrpHeigth, SWP_NOZORDER) ;

  int iNouvWindowWidth  = _iWndWidth  /* - 2 * iSeparLeft*/ + 2 * _iGroupLeft ;
  //int iNouvWindowWidth  = pGroup->Attr.W ; // + 2 * iGroupLeft ;
  //int iNouvWindowHeight = 2 * iBtn_bas + iBtnHeight + iGrpHeigth + 2 * iGroupTop ; // pGroup->Attr.Y ; // + iGrpHeigth + iGroupTop ;
  int iNouvWindowHeight = 2 * _iBtn_bas + _iBtnHeight + iGrpHeigth + 2 * _iGroupTop + 20 ;

  NS_CLASSLIB::TRect dlgSizeRect(0, 0, iNouvWindowWidth, iNouvWindowHeight) ;
  MapDialogRect(dlgSizeRect) ;

  SetupWindow() ;

  // Redimentionnement
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

  // On compare le clientRect réel avec les dimensions souhaitées, et on modifie le WindowRect en conséquence
  int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
  int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;

  int iLeft ;
  int iTop ;

  if (_pCurrentWindow)
  {
    NS_CLASSLIB::TRect currentRect ;
    _pCurrentWindow->GetWindowRect(currentRect) ;
    iLeft   = currentRect.left + currentRect.Width() + 1 ;
    iTop    = currentRect.top ;
  }
  else
  {
    iLeft   = dlgRect.left ;
    iTop    = dlgRect.top ;
  }

  //MoveWindow(iLeft, iTop, 419, 345, true) ;
  MoveWindow(/*dlgRect.left*/ iLeft, /*dlgRect.top*/ iTop, dlgSizeRect.Width(), dlgSizeRect.Height(), true) ;

  // On actualise les boutons
  ArrayPrediDataIter it = _aData.begin() ;
  for ( ; _aData.end() != it ; it++)
  {
    OWL::TCheckBox * pBox = TYPESAFE_DOWNCAST((*it)->getControl(), OWL::TCheckBox) ;
    if (pBox)
    {
      if ((*it)->getWindow() == _pCurrentWindow)
        pBox->SetCheck(BF_CHECKED) ;
      else
        pBox->SetCheck(BF_UNCHECKED) ;
    }
  }
}

void
NSPrediDlg::fermer()
{
  if (false == _aData.empty())
    for (ArrayPrediDataIter it = _aData.begin() ; _aData.end() != it ; )
    {
      if ((*it)->getWindow())
        (*it)->getWindow()->Destroy() ;

      delete (*it) ;
      _aData.erase(it) ;

      if ((_aData.end() != it) && ((*it)->getWindow()))
        activer(uint((*it)->getControl()->GetDlgCtrlID())) ;
    }

  NSPredi* pReferencedPredi = pContexte->getPredi() ;

  CloseWindow(IDOK) ;

  if (NULL == pReferencedPredi)
    return ;

  pReferencedPredi->_iPrediState = NSPredi::stateMessage ;

  if (pReferencedPredi->_pPrediDlg == this)
    pReferencedPredi->_pPrediDlg = (NSPrediDlg*) 0 ;
}

void
NSPrediDlg::referencerModule(PrediModuleData * pModuleData, bool bReaffiche)
{
  if (NULL == pModuleData)
    return ;

  OWL::TCheckBox * pChkBox = new OWL::TCheckBox(this, IDBTN + _iNbBtns, pModuleData->getTitle().c_str(), _iBoxLeft + _iGroupLeft, 10, _iBoxWidth, _iBoxHeight, _pGroup) ;
  pModuleData->setControl(pChkBox) ;
  _iNbBtns++ ;

  if (_iNbBtns > 1)
    pModuleData->getWindow()->ShowWindow(SW_HIDE) ;
  else
    _pCurrentWindow = pModuleData->getWindow() ;

  _aData.push_back(new PrediModuleData(*pModuleData)) ;
  if (bReaffiche)
    reaffiche() ;
}

void
// NSDPIODlg::fermetureModule(TWindow * pDialog)
NSPrediDlg::fermetureModule(void *pDialog)
{
	if (_aData.empty())
		return ;

  ArrayPrediDataIter it = _aData.begin() ;
  for ( ; (_aData.end() != it) && ((*it)->getWindow() != pDialog) ; it++)
    ;
  if (_aData.end() == it)
    return ;

  PrediModuleData * pActivable = 0 ;
  ArrayPrediDataIter itActivable = it ;
  if (it == _aData.begin())
    itActivable++ ;
  else
    itActivable-- ;

  if (_aData.end() != itActivable)
    pActivable = *itActivable ;

  // Supression
	if ((*it)->getWindow() == _pCurrentWindow)
  	_pCurrentWindow = (TWindow *) 0 ;

  OWL::TControl* pControl = (*it)->getControl() ;
  if (pControl)
    delete pControl ;

  delete *it ;
  _aData.erase(it) ;

  if (pActivable == 0)
  	fermer() ;
  else
  {
  	reaffiche() ;
    uint id = uint(pActivable->getControl()->GetDlgCtrlID()) ;
    activer(id) ;
  }
}

void
NSPrediDlg::activer(uint id)
{
  if (_aData.empty())
    return ;

  // On cherche l'élément qui correspond à cet ID de bouton
  ArrayPrediDataIter it = _aData.begin() ;
  for ( ; _aData.end() != it ; it++)
    if (uint((*it)->getControl()->GetDlgCtrlID()) == id)
      break ;

  if ((_aData.end() == it) || (NULL == (*it)->getWindow()))
    return ;

  // On cache la fenêtre en cours
  if (_pCurrentWindow)
    _pCurrentWindow->ShowWindow(SW_HIDE) ;

  // On affiche la fenêtre sélectionnée
  (*it)->getWindow()->ShowWindow(SW_SHOW) ;
  (*it)->getWindow()->BringWindowToTop() ;
  _pCurrentWindow = (*it)->getWindow() ;

  // On repositionne la popup à doite du formulaire
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect currentRect ;
  _pCurrentWindow->GetWindowRect(currentRect) ;
  int iLeft = currentRect.left + currentRect.Width() + 1 ;
  int iTop  = currentRect.top ;

  MoveWindow(iLeft, iTop, dlgRect.Width(), dlgRect.Height(), true) ;

  // On actualise les boutons
  it = _aData.begin() ;
  for ( ; _aData.end() != it ; it++)
  {
    OWL::TCheckBox * pBox = TYPESAFE_DOWNCAST((*it)->getControl(), OWL::TCheckBox) ;
    if ( pBox )
    {
      if (uint(pBox->GetDlgCtrlID()) == id)
        pBox->SetCheck(BF_CHECKED) ;
      else
        pBox->SetCheck(BF_UNCHECKED) ;
    }
  }
}

#endif // !__EPIBRAIN__

// -----------------------------------------------------------------------------
//
// Classe PrediModuleData
//
// -----------------------------------------------------------------------------

PrediModuleData::PrediModuleData(TWindow* pDlg, string sTitre)
{
  _pWindow  = pDlg ;
  _sTitle   = sTitre ;
  _pControl = (OWL::TControl*) 0 ;
}

PrediModuleData::PrediModuleData(const PrediModuleData& rv)
{
  _pWindow  = rv._pWindow ;
  _sTitle   = rv._sTitle ;
  _pControl = rv._pControl ;
}

PrediModuleData::~PrediModuleData()
{
}

PrediModuleData&
PrediModuleData::operator=(const PrediModuleData& src)
{
	if (this == &src)
  	return *this ;

  _pWindow  = src._pWindow ;
  _sTitle   = src._sTitle ;
  _pControl = src._pControl ;

  return (*this) ;
}

