// -----------------------------------------------------------------------------
// nsarc.cpp
// -----------------------------------------------------------------------------
// Parseur d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2013/07/21 17:55:09 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// RS  - november 2002
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

#include "nsbb\nsarcCtrls.h"
#include "nsbb\nsarc.h"
#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "pre_parseur.h"
#include "nsbb\nsarcParseError.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbsmal.h"

#include "ns_ob1\Interface.h"
#include "ns_ob1\OB1.rh"
#include "nautilus\nautilus.rh"

#define CM_HAUTEUR  701

// -----------------------------------------------------------------------------
//
// Classe nsrefButton
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(nsrefButton, TRadioButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE ;

nsrefButton::nsrefButton(TWindow* parent, NSContexte* pCtx, int resId, const char far* title,                             int x, int y, int w, int h, TGroupBox *group,
                             TModule* module)
  : TRadioButton(parent, resId, title, x, y, w, h, group, module),
    NSRoot(pCtx)
{
  // Attention, le constructeur de TRadioButton employé ici attribue le style
  // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON
  Attr.Style    = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON ;
  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

  pProposition = 0 ;
}

nsrefButton::~nsrefButton()
{
}

void
nsrefButton::BNClicked()
{
try
{
	if (GetCheck() == BF_CHECKED)
  {
  	bool bCanUncheck = true ;
    if ((pProposition) && (pProposition->bInitialStateOn) &&
        (string("") != pProposition->propos->getUncheckArchetype()) &&
        (true == pProposition->propos->getIsAutocheck()))
    {
    	if (NULL == pProposition->pReadyTree)
      	pProposition->pReadyTree = new NSPatPathoArray(pContexte->getSuperviseur()) ;

      TWindow* pParent = Parent ;
#ifdef __OB1__
      NSSmallBrother BigBoss(pContexte, pProposition->pReadyTree, 0) ;
#else
			NSSmallBrother BigBoss(pContexte, pProposition->pReadyTree, 0, true) ;
#endif

      BigBoss.setMotherWindow(pParent) ;
			// string sArchetype = string("admin.goal.refuse.1.0") ;
      string sArchetype = pProposition->propos->getUncheckArchetype() ;
# ifdef __OB1__
    	BB1BBInterfaceForKs InterfaceForKs(-1, sArchetype, "", true) ;
			/* NSDialog *pClientWin = */ BigBoss.lanceBbkArchetypeInDialog(sArchetype, 0, 0, &InterfaceForKs, true /*modal*/) ;
# else
			/* NSDialog *pClientWin = */ BigBoss.lanceBbkArchetypeInDialog(sArchetype, 0, 0, true /*modal*/) ;
# endif

      pParent->SetFocus() ;

      if (pProposition->pReadyTree->empty())
      	bCanUncheck = false ;
    }
    if (bCanUncheck)
    	Uncheck() ;
  }
  else
  	Check() ;

  // TRadioButton::BNClicked() ;
}
catch (...)
{
	erreur("Exception nsrefButton::BNClicked.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//
// Classe nsrefGroup
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(nsrefGroup, TGroupBox)
//  EV_WM_PAINT,
  EV_COMMAND(CM_HAUTEUR, CmHauteur),
END_RESPONSE_TABLE ;

nsrefGroup::nsrefGroup(nsrefDialog* pere, int Id, const char far *text, int x, int y, int w, int h, TModule* module)  : TGroupBox((TWindow*) pere, Id, text, x, y, w, h, module)
{
  pDlg = pere ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  hauteur = h ;
}

nsrefGroup::~nsrefGroup()
{
}

void
nsrefGroup::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect)
{
  NS_CLASSLIB::TRect winRect ;
  GetWindowRect(winRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

  TGroupBox::Paint(dc, erase, rect) ;
}

void
nsrefGroup::CmHauteur()
{
  pDlg->_iVGroupHeight = hauteur ;
}

// -----------------------------------------------------------------------------
//
// Classe NSProposition
//
// -----------------------------------------------------------------------------

NSProposition::NSProposition(Cproposition* prop, nsrefButton* butt)
{
  propos          = prop ;
  button          = butt ;

  bInitialStateOn = false ;
  pReadyTree      = 0 ;
}

NSProposition::~NSProposition()
{
  // on ne delete pas la proposition car
  // elle est rattachée au parseur
  if (button)
    delete button ;
  if (pReadyTree)
    delete pReadyTree ;
}

NSProposition::NSProposition(const NSProposition& rv)
{
try
{
  propos          = rv.propos ;
  button          = rv.button ;
  bInitialStateOn = rv.bInitialStateOn ;

  if (rv.pReadyTree)
    pReadyTree = new NSPatPathoArray(*(rv.pReadyTree)) ;
  else
    pReadyTree = 0 ;
}
catch (...)
{
  erreur("Exception NSProposition copy ctor.", standardError, 0) ;
}
}

NSProposition&
NSProposition::operator=(const NSProposition& src)
{
try
{
	if (this == &src)
		return *this ;

  propos          = src.propos;
  button          = src.button;
  bInitialStateOn = src.bInitialStateOn ;

  if (pReadyTree)
    delete pReadyTree ;
  if (src.pReadyTree)
    pReadyTree = new NSPatPathoArray(*(src.pReadyTree)) ;
  else
    pReadyTree = 0 ;

  return (*this) ;
}
catch (...)
{
  erreur("Exception NSProposition = operator.", standardError, 0) ;
  return (*this) ;
}
}

// -----------------------------------------------------------------------------
//
// Classe NSProposArray
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur copie// ---------------------------------------------------------------------------NSProposArray::NSProposArray(const NSProposArray& rv)
              :NSProposVector()
{try{  if (false == rv.empty())    for (NSProposCIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSProposition(*(*i))) ;}catch (...)
{
  erreur("Exception NSProposArray copy ctor.", standardError, 0) ;
}}
NSProposArray&
NSProposArray::operator=(const NSProposArray& src)
{try{	if (this == &src)		return *this ;  vider() ;  if (false == src.empty())    for (NSProposCIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSProposition(*(*i))) ;  return (*this) ;}catch (...)
{
  erreur("Exception NSProposArray = operator.", standardError, 0) ;
  return (*this) ;
}}

voidNSProposArray::vider(){  if (false == empty())    for (NSProposIter i = begin() ; end() != i ; )    {      delete (*i) ;      erase(i) ;    }}NSProposArray::~NSProposArray(){	vider() ;}

// -----------------------------------------------------------------------------
//
// Classe nsrefDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(nsrefDialog, TDialog)
  EV_COMMAND(IDCANCEL,    CmCancel),
	EV_COMMAND(IDOK,        CmOk),
	EV_COMMAND(IDC_OTHER,   CmOther),
  EV_COMMAND(IDHELP,      CmHelp),
  EV_WM_VSCROLL,
  EV_MESSAGE(WM_MOUSEWHEEL, EvMouseWheel),
  EV_COMMAND(IDM_BBK_EVENT, CmBbkEvent),
END_RESPONSE_TABLE ;

nsrefDialog::nsrefDialog(TWindow* Parent, NSContexte* pCtx, nsrefParseur* parseur, bool bSimple)
            :TDialog(Parent, "DLG_BASE", pNSDLLModule), NSRoot(pCtx)
{
try
{
  _pParseur     = parseur ;
  _pProposArray = new NSProposArray ;
  _bSimplifie   = bSimple ;
  _pScrollBar   = (OWL::TScrollBar *) 0 ;
  _sTitle       = string("") ;
  _sHelp        = string("") ;

  initVars() ;

  _bOngoingSetupWindow = false ;
  _bReinitCalled       = false ;
}
catch (...)
{
  erreur("Exception nsrefDialog ctor.", standardError, 0) ;
}
}

nsrefDialog::~nsrefDialog()
{
  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("nsrefDialog destructor disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(_hWindow) ;
  }

  delete _pProposArray ;
  if (_pScrollBar)
    delete _pScrollBar ;
}

void
nsrefDialog::SetupWindow()
{
try
{
  _hWindow = HWindow ;

  // Prevents any event to start another SetupWindow while this one is at work
  //
  _bOngoingSetupWindow = true ;

  initControls() ;

	// Appel du SetupWindow() de TDialog
	TDialog::SetupWindow() ;

	// Pré-cochage : Appel au blackboard
  PreCochePropositions() ;

	// Redimentionnement
	GetWindowRect(_dlgRect) ;
	NS_CLASSLIB::TRect clientRect ;
	GetClientRect(clientRect) ;

	// On compare le clientRect réel avec les dimensions souhaitées, et on modifie le WindowRect en conséquence	int nouvWindowWidth  = _dlgRect.Width()  + (_dlgSizeRect.Width()  - clientRect.Width()) ;
	int nouvWindowHeight = _dlgRect.Height() + (_dlgSizeRect.Height() - clientRect.Height()) ;

	MoveWindow(_dlgRect.left, _dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;

  // Evaluation du rectangle utilisable à l'écran
  // NS_CLASSLIB::TRect mainRect = Parent->GetWindowRect() ;

  // Taille de l'écran - Screen size
  TScreenDC screenDC ;
  int iHorzRes = screenDC.GetDeviceCaps(HORZRES) ;
  int iVertRes = screenDC.GetDeviceCaps(VERTRES) ;

  _usableRect = NS_CLASSLIB::TRect(_dlgRect.TopLeft(), NS_CLASSLIB::TPoint(iHorzRes, iVertRes)) ;

  // TaskBar de Windows
  // RECT        rect ;
  APPBARDATA  AppBarData ;

  AppBarData.hWnd = ::FindWindow("Shell_TrayWnd", NULL) ;
  if (0 != AppBarData.hWnd)
  {
    AppBarData.cbSize = sizeof(AppBarData) ;
    int iResult = ::SHAppBarMessage(ABM_GETTASKBARPOS, &AppBarData) ;
    if (iResult)
    {
      switch (AppBarData.uEdge)
      {
        case ABE_BOTTOM : _usableRect.bottom = AppBarData.rc.top ;
                          break ;
        case ABE_LEFT   : if (_usableRect.Left() < AppBarData.rc.right)
                            _usableRect.left = AppBarData.rc.right ;
                          break ;
        case ABE_RIGHT  : if (_usableRect.Right() > AppBarData.rc.left)
                            _usableRect.right = AppBarData.rc.left ;
                          break ;
        case ABE_TOP    : break ;
      }
    }
  }

  // Initialisation de l'ascenseur
  _iVSize = nouvWindowHeight ;
  _iHSize = nouvWindowWidth ;
  // Par défaut l'ascenseur est visible : il faut donc le cacher s'il est inutile
  _bVisibleScroll = true ;

  SetSize() ;

  _bOngoingSetupWindow = false ;

  if (_bReinitCalled)
    PostMessage(IDM_BBK_EVENT) ;
}
catch (...)
{
  erreur("Exception nsrefDialog::SetupWindow.", standardError, 0) ;
}
}

void
nsrefDialog::initVars()
{
  _usableRect  = NS_CLASSLIB::TRect(0, 0, 0, 0) ;
  _dlgRect     = NS_CLASSLIB::TRect(0, 0, 0, 0) ;
  _dlgSizeRect = NS_CLASSLIB::TRect(0, 0, 0, 0) ;

  _iVSize         = 0 ;
  _iHSize         = 0 ;
  _iVBoxHeight    = 0 ;
  _iVGroupHeight  = 0 ;
  _iVSizeNoScroll = 0 ;
  _iVSizeBtnBas   = 0 ;
  _iVRealSize     = 0 ;
  _iHRealSize     = 0 ;
  _iDecalageY     = 0 ;
  _iOldDecalY     = 0 ;

  _bVisibleScroll = false ;
}

void
nsrefDialog::initControls()
{
  if ((NULL == _pParseur) || (NULL == _pParseur->getReferentiel()))
    return ;

  Valeur_array* pValArray = _pParseur->getReferentiel()->getValArray() ;
  if ((NULL == pValArray) || (pValArray->empty()))
    return ;

  ValIter ival = pValArray->begin() ;
  for ( ; pValArray->end() != ival ; ival++)
  {    if (LABEL_PROPOSITION == (*ival)->getLabel())    {      Cproposition* pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;      if (pCprop)        pCprop->setTraite(false) ;    }  }

  // Initialisation des variables globales
  //
  Cglobalvars *pGVars = _pParseur->getReferentiel()->getGVars() ;
  if (pGVars)
    pGVars->process(pContexte, true, HWindow) ;

  string sLang = pContexte->getUserLanguage() ;

	NS_CLASSLIB::TRect cvtRect ;

	int iBoxHeight = 10 ;    // hauteur du bouton
	int iBoxWidth  = 300 ;   // largeur du bouton
	int iBoxInterv = 3 ;     // intervalle entre deux boutons
	int iBoxTop    = 4 ;     // haut du 1er bouton à partir du haut du groupbox
	int iBoxTitleTop = 12 ;
	int iBoxLeft   = 5 ;     // gauche des boutons à partir de la gauche du groupbox

	int iGroupTop  = 3 ;     // haut du groupbox
	int iFirstGroupTop = iGroupTop ;
	int iGroupLeft = 2 ;     // gauche du groupbox

	// int iSeparLeft = 3 ;     // gauche du séparateur  int iSepar_Btn = 4 ;     // intervalle entre le séparateur et le bouton	int iGB_Separ  = 3 ;     // intervalle entre le bas du groupbox et le séparateur

	int iBtnHeight = 25 ;    // hauteur d'un bouton Borland
	int iBtnWidth  = 43 ;    // largeur d'un bouton Borland
	int iBtnLeft   = 5 ;     // gauche du premier bouton
	int iBtn_bas   = 7 ;     // intervalle entre le bas du bouton et le bas de la boite

	int iGroupWidth = (2 * iBoxLeft) + iBoxWidth ;	int iTotalWidth = (2 * iGroupLeft) + iGroupWidth ;
	int iGroupHeight = 0 ;
	int iTotalGroupHeight = 0 ;
	int iTopOfBox ;
	int iID = CBUTTONID ;


	VecteurString aExcluded ;
	VecteurString aNeeded ;

	//
	// Première passe : recherche du groupe vide
  //
  string sGroup = string("") ;
	int iNbBoxes = _pParseur->getReferentiel()->getNbPropGroup(pContexte, string(""), false, sLang) ;
	if (iNbBoxes > 0)	{		iGroupHeight = (2 * iBoxTop) + (iNbBoxes * iBoxHeight) + ((iNbBoxes - 1) * iBoxInterv) ;
		iTotalGroupHeight += iGroupHeight ;

		// On crée le groupe		cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iGroupTop, iGroupLeft + iGroupWidth, iGroupTop + iGroupHeight) ;
		MapDialogRect(cvtRect) ;

		/* nsrefGroup *pGroup = */ new nsrefGroup(this, -1, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /* TModule* */ 0) ;
		iTopOfBox = iGroupTop + iBoxTop ;
		for (ival = pValArray->begin() ; pValArray->end() != ival ; ival++)		{			if (LABEL_PROPOSITION == (*ival)->getLabel())			{				Cproposition* pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;				if ((pCprop) && (string("") == pCprop->getGroup(sLang)))				{					pCprop->setTraite(true) ;					// Rectangle du bouton					cvtRect = NS_CLASSLIB::TRect(iGroupLeft + iBoxLeft, iTopOfBox, iGroupLeft + iBoxLeft + iBoxWidth, iTopOfBox + iBoxHeight) ;					MapDialogRect(cvtRect) ;
					// Titre					string sTitre = pCprop->getTitle(sLang) ;					// bouton					nsrefButton *pButton = new nsrefButton(this, pContexte, iID, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;          NSProposition* pPropos = new NSProposition(pCprop, pButton) ;
          pButton->pProposition = pPropos ;

					_pProposArray->push_back(pPropos) ;

					iTopOfBox += iBoxHeight + iBoxInterv ;
					iID++ ;

          pCprop->addExcludedToVector(&aExcluded) ;
          pCprop->addNeededToVector(&aNeeded) ;
				}
			}		}	}	// Deuxième passe : Recherche des autres groupes	while (_pParseur->getReferentiel()->getNextGroup(sGroup, sLang))	{		if (false == _bSimplifie)		{
      // on est surs d'avoir au moins une proposition dans le groupe
      iNbBoxes = _pParseur->getReferentiel()->getNbPropGroup(pContexte, sGroup, false, sLang) ;

			iGroupTop += iGroupHeight + iGB_Separ ;			iGroupHeight = (iBoxTitleTop + iBoxTop) + (iNbBoxes * iBoxHeight) + ((iNbBoxes-1) * iBoxInterv) ;
			iTotalGroupHeight += iGB_Separ + iGroupHeight ;			// On crée le groupe			cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iGroupTop, iGroupLeft + iGroupWidth, iGroupTop + iGroupHeight) ;
			MapDialogRect(cvtRect) ;

			/* nsrefGroup *pGroup = */ new nsrefGroup(this, -1, sGroup.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /* TModule* */ 0) ;			iTopOfBox = iGroupTop + iBoxTitleTop ;
		}
    else      iNbBoxes = _pParseur->getReferentiel()->getNbPropGroup(pContexte, sGroup, true, sLang) ;		bool	bFirstInGroup = true ;		for (ival = pValArray->begin() ; pValArray->end() != ival ; ival++)		{			if (LABEL_PROPOSITION == (*ival)->getLabel())			{				Cproposition* pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;				if ((pCprop) && (false == pCprop->getTraite()) && (pCprop->getGroup(sLang) == sGroup))				{					pCprop->setTraite(true) ;					if (_bSimplifie)					{            bool bValid = true ;            // Does this proposition belong to the excluded list          	//
        		string sPropID = pCprop->getStringAttribute(ATTRIBUT_PROP_ID) ;
						if ((string("") != sPropID) && (aExcluded.ItemDansUnVecteur(sPropID)))
          		bValid = false ;            // Else, if this proposition doesn't belong to the needed list,            // we have to check its validity            //            else if ((string("") == sPropID) || (false == aNeeded.ItemDansUnVecteur(sPropID)))              for (ValIter i = pCprop->getValArray()->begin() ; bValid && (pCprop->getValArray()->end() != i) ; i++)              {                if (LABEL_VALIDITE == (*i)->getLabel())                {                  Ccontrainte	*pCvalidite = dynamic_cast<Ccontrainte *>((*i)->getObject()) ;                  NSValidateur valid(pCvalidite, pContexte) ;                  if (false == valid.Validation())                  {                    bValid = false ;                    break ;                  }                }              }            if (bValid)            {              if (bFirstInGroup)              {                iGroupTop += iGroupHeight + iGB_Separ ;                iGroupHeight = (iBoxTitleTop + iBoxTop) + (iNbBoxes * iBoxHeight) + ((iNbBoxes-1) * iBoxInterv) ;
                iTotalGroupHeight += iGB_Separ + iGroupHeight ;                // On crée le groupe                cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iGroupTop, iGroupLeft + iGroupWidth, iGroupTop + iGroupHeight) ;
                MapDialogRect(cvtRect) ;

                nsrefGroup *pGroup = new nsrefGroup(this, -1, sGroup.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /* TModule* */ 0) ;                pGroup->Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
                iTopOfBox = iGroupTop + iBoxTitleTop ;

                bFirstInGroup = false ;
              }              // Rectangle du bouton              cvtRect = NS_CLASSLIB::TRect(iGroupLeft + iBoxLeft, iTopOfBox, iGroupLeft + iBoxLeft + iBoxWidth, iTopOfBox + iBoxHeight) ;              MapDialogRect(cvtRect) ;
              // Titre              string sTitre = pCprop->getTitle(sLang) ;              // bouton              nsrefButton* pButton = new nsrefButton(this, pContexte, iID, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;              NSProposition* pPropos = new NSProposition(pCprop, pButton) ;
              pButton->pProposition = pPropos ;

              _pProposArray->push_back(pPropos) ;

              iTopOfBox += iBoxHeight + iBoxInterv ;

              // Aide en ligne
              if (string("") == _sHelp)
                _sHelp = pCprop->getHelpUrl(sLang) ;

              pCprop->addExcludedToVector(&aExcluded) ;
              pCprop->addNeededToVector(&aNeeded) ;
            }
					}					else					{						// Rectangle du bouton						cvtRect = NS_CLASSLIB::TRect(iGroupLeft + iBoxLeft, iTopOfBox, iGroupLeft + iBoxLeft + iBoxWidth, iTopOfBox + iBoxHeight) ;						MapDialogRect(cvtRect) ;
						string sTitre = pCprop->getTitle(sLang) ;						nsrefButton* pButton = new nsrefButton(this, pContexte, iID, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;            NSProposition* pPropos = new NSProposition(pCprop, pButton) ;            pButton->pProposition = pPropos ;						_pProposArray->push_back(pPropos) ;

						iTopOfBox += iBoxHeight + iBoxInterv ;

            // Aide en ligne
            if (string("") == _sHelp)
              _sHelp = pCprop->getHelpUrl(sLang) ;
					}

					iID++ ;
				}
			}		}	}  // On fixe la taille de la boite de dialogue	int iTotalHeight = iFirstGroupTop + iTotalGroupHeight + iGB_Separ + iSepar_Btn + iBtnHeight + iBtn_bas ;  int iTotalBtnHeight = iSepar_Btn + iBtnHeight + iBtn_bas ;
  _dlgSizeRect = NS_CLASSLIB::TRect(0, 0, iTotalWidth, iTotalHeight) ;	MapDialogRect(_dlgSizeRect) ;  // on convertit ici un rectangle fictif en pixels pour obtenir la hauteur des boutons + separateur  NS_CLASSLIB::TRect BtnRect(0, 0, iTotalWidth, iTotalBtnHeight) ;  MapDialogRect(BtnRect) ;  _iVSizeNoScroll = BtnRect.Height() ;  NS_CLASSLIB::TRect BtnBasRect(0, 0, iTotalWidth, iBtnHeight + iBtn_bas) ;  MapDialogRect(BtnBasRect) ;  _iVSizeBtnBas = BtnBasRect.Height() ;  // on fait de meme pour récupérer la hauteur des boutons en pixels  NS_CLASSLIB::TRect BoxHeightRect(0, 0, iTotalWidth, iBoxHeight) ;  MapDialogRect(BoxHeightRect) ;  _iVBoxHeight = BoxHeightRect.Height() ;	// On crée le séparateur	int iSepareTop = iGroupHeight + iGroupTop + iGB_Separ ;

	// cvtRect = NS_CLASSLIB::TRect(iGroupLeft, iSepareTop, iGroupLeft + iGroupWidth, iSepareTop + iBtnHeight + (2*iSepar_Btn)) ;	// MapDialogRect(cvtRect) ;
	// TGroupBox* pDip = new TGroupBox(this, IDC_SEPAR, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*TModule**/ 0) ;
  // pDip->Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
	// pDip->Attr.Style |= BSS_HDIP ;

	// On crée les boutons
	int iNbBtn ;
	if (_bSimplifie)
		iNbBtn = 4 ;
	else
		iNbBtn = 3 ;

	int iBtnInterv = (iTotalWidth - (2 * iBtnLeft) - (iNbBtn * iBtnWidth)) / (iNbBtn - 1) ;
	int iBtnTopPos  = iSepareTop + iSepar_Btn ;
  int iBtnLeftPos = iBtnLeft ;

	// Bouton OK
	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtOK = */ new TButton(this, IDOK, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ true, 0) ;
	iBtnLeftPos += iBtnWidth + iBtnInterv ;

	// Bouton CANCEL	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtCn = */ new TButton(this, IDCANCEL, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
	iBtnLeftPos += iBtnWidth + iBtnInterv ;
	// Bouton HELP	cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;	MapDialogRect(cvtRect) ;
	/* TButton* pBtHl = */ new TButton(this, IDHELP, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
	iBtnLeftPos += iBtnWidth + iBtnInterv ;
	if (_bSimplifie)	{		// Bouton AUTRE		cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;		MapDialogRect(cvtRect) ;
		/* TButton* pBtAutre = */ new TButton(this, IDC_OTHER, "Autre", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
		iBtnLeftPos += iBtnWidth + iBtnInterv ;
	}
  // Titre de la boite de dialogue  string sTitle = _pParseur->getReferentiel()->getTitle(sLang) ;  if (string("") != sTitle)
    SetCaption(sTitle.c_str()) ;  else    SetCaption("Référentiel") ;  // Ascenseur  _pScrollBar = new TScrollBar(this, VSCROLLID, 0, 0, SCROLL_WIDTH, 100, false) ;}
void
nsrefDialog::SetSize()
{
  bool bShowScroll = false ;
  bool bHideScroll = false ;

  _iDecalageY = 0 ;
  _iOldDecalY = 0 ;

  if (_iVSize > _usableRect.Height())
  {
    _iVRealSize = _usableRect.Height() ;
    _iHRealSize = _iHSize + SCROLL_WIDTH ;
    if (_dlgRect.left + _iHRealSize > _usableRect.Right())
      _iHRealSize = _usableRect.Right() - _dlgRect.left ;
    _dlgRect.right = _dlgRect.left + _iHRealSize ;
    if (false == _bVisibleScroll)
    {
      bShowScroll = true ;
      _bVisibleScroll = true ;
    }
    SCROLLINFO scInfo ;
    scInfo.cbSize = (UINT) sizeof(scInfo) ;
    scInfo.fMask  = SIF_ALL ;
    scInfo.nMin   = 1 ;
    scInfo.nMax   = _iVSize ;
    scInfo.nPage  = (UINT) _iVRealSize ;
    scInfo.nPos   = 1 ;
    _pScrollBar->SetScrollInfo(&scInfo, TRUE) ;
  }
  else
  {
    _iVRealSize = _iVSize ;
    _iHRealSize = _iHSize ;
    _dlgRect.right = _dlgRect.left + _iHRealSize ;
    if (_bVisibleScroll)
    {
      bHideScroll = true ;
      _bVisibleScroll = false ;
    }
  }

  _dlgRect.bottom = _dlgRect.top + _iVRealSize ;

  // SetWindowPos(Parent->HWindow, Attr.X, Attr.Y, Attr.W, Attr.H, SWP_NOZORDER) ;
  MoveWindow(_dlgRect.left, _dlgRect.top, _dlgRect.Width(), _dlgRect.Height()) ;

  if (_bVisibleScroll)
  {
    _pScrollBar->Attr.H = GetClientRect().Height() - _iVSizeNoScroll ;
    _pScrollBar->Attr.X = GetClientRect().Width() - SCROLL_WIDTH ;
    _pScrollBar->SetWindowPos(HWindow, _pScrollBar->Attr.X, _pScrollBar->Attr.Y, _pScrollBar->Attr.W, _pScrollBar->Attr.H, SWP_NOZORDER) ;
    _pScrollBar->SetRange(0, _iVSize - _iVRealSize, false) ;

    WNDENUMPROC lpEnumDlgFunc1, lpEnumDlgFunc2 ;

    // Préparation de la fonction d'énumération
    lpEnumDlgFunc1 = (WNDENUMPROC) MakeProcInstance((FARPROC) nsrefDialog::DlgScrollCtrl, hInstance) ;
    EnumChildWindows(HWindow, lpEnumDlgFunc1, LPARAM((TWindow *)this)) ;

    NS_CLASSLIB::TRect dlgRect ;
    NS_CLASSLIB::TRect cliRect ;

    GetClientRect(dlgRect) ;
    cliRect = NS_CLASSLIB::TRect(0, dlgRect.Height() - _iVSizeNoScroll, dlgRect.Width(), dlgRect.Height()) ;
    // InvalidateRect(cliRect) ;

    // On recale ensuite les boutons en bas du dialogue
    lpEnumDlgFunc2 = (WNDENUMPROC) MakeProcInstance((FARPROC) nsrefDialog::DlgScrollBtns, hInstance) ;
    EnumChildWindows(HWindow, lpEnumDlgFunc2, LPARAM((TWindow *)this)) ;
  }

  if (bShowScroll)
    _pScrollBar->Show(SW_SHOW) ;

  if (bHideScroll)
    _pScrollBar->Show(SW_HIDE) ;
}

void DeleteControl(TWindow* p, void*)
{
  delete p ;
}

void
nsrefDialog::eraseControls()
{
  _pProposArray->vider() ;
  ForEach(DeleteControl) ;
}

void
nsrefDialog::EvVScroll(UINT scrollCode, UINT /* thumbPos */, HWND /* hWndCtl */)
{
  if (((SB_LINEUP == scrollCode) || (SB_PAGEUP == scrollCode)) && (0 == _iDecalageY))
    return ;

  int iMaxDecal = _iVSize - _iVRealSize ;

  if (((SB_LINEDOWN == scrollCode) || (SB_PAGEDOWN == scrollCode)) && (_iDecalageY >= iMaxDecal))
    return ;

  if      (SB_LINEDOWN == scrollCode)
  {
    if (iMaxDecal > _iDecalageY + 10)
      _iDecalageY += 10 ;
    else
      _iDecalageY = iMaxDecal ;
  }
  else if (SB_PAGEDOWN == scrollCode)
  {
    if (iMaxDecal > _iDecalageY + _iVRealSize)
      _iDecalageY += _iVRealSize ;
    else
      _iDecalageY = iMaxDecal ;
  }
  else if (SB_LINEUP == scrollCode)
  {
    if (_iDecalageY > 10)
      _iDecalageY -= 10 ;
    else
      _iDecalageY = 0 ;
  }
  else if (SB_PAGEUP == scrollCode)
  {
    if (_iDecalageY > _iVRealSize)
      _iDecalageY -= _iVRealSize ;
    else
      _iDecalageY = 0 ;
  }

  _pScrollBar->SetPosition(_iDecalageY) ;

  WNDENUMPROC lpEnumDlgFunc1, lpEnumDlgFunc2 ;

  // Préparation de la fonction d'énumération
  lpEnumDlgFunc1 = (WNDENUMPROC) MakeProcInstance((FARPROC) nsrefDialog::DlgScrollCtrl, hInstance) ;

  // On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
  // this puisque dans les fonctions static on ne peut pas récupérer
  // le pointeur this.
  EnumChildWindows(HWindow, lpEnumDlgFunc1, LPARAM((TWindow *)this)) ;

  NS_CLASSLIB::TRect dlgRect ;
  NS_CLASSLIB::TRect cliRect ;

  GetClientRect(dlgRect) ;
  cliRect = NS_CLASSLIB::TRect(0, dlgRect.Height() - _iVSizeNoScroll, dlgRect.Width(), dlgRect.Height()) ;
  InvalidateRect(cliRect) ;

  // On recale ensuite les boutons en bas du dialogue
  lpEnumDlgFunc2 = (WNDENUMPROC) MakeProcInstance((FARPROC) nsrefDialog::DlgScrollBtns, hInstance) ;

  // On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
  // this puisque dans les fonctions static on ne peut pas récupérer
  // le pointeur this.
  EnumChildWindows(HWindow, lpEnumDlgFunc2, LPARAM((TWindow *)this)) ;

  _iOldDecalY = _iDecalageY ;
}

LRESULT
nsrefDialog::EvMouseWheel(WPARAM /* wParam */, LPARAM /* lParam */)
{
  return 0 ;
}

bool FAR PASCAL _export
nsrefDialog::DlgScrollCtrl(HWND hWnd, LPARAM lParam)
{
try
{
	// Adresse de l'objet boîte de dialogue courante
	TWindow* pWnd = reinterpret_cast<TWindow *>(lParam) ;
	nsrefDialog* pDlg = dynamic_cast<nsrefDialog *>(pWnd) ;

  int ctrlID  = ::GetDlgCtrlID(hWnd) ;
  NS_CLASSLIB::TRect ctrlRect ;
  NS_CLASSLIB::TRect dlgRect ;
  int X, Y ;
  int hScroll, hGroup ;

  if ((VSCROLLID != ctrlID) && (IDOK != ctrlID) && (IDCANCEL != ctrlID) &&
      (IDHELP != ctrlID) && (IDC_OTHER != ctrlID))
  {
    ::GetWindowRect(hWnd, &ctrlRect) ;
    pDlg->GetClientRect(dlgRect) ;
    hScroll = dlgRect.Height() - pDlg->_iVSizeNoScroll ;
    NS_CLASSLIB::TPoint point(dlgRect.X(), dlgRect.Y()) ;
    pDlg->ClientToScreen(point) ;
    ctrlRect.top += (pDlg->_iOldDecalY - pDlg->_iDecalageY) ;
    ctrlRect.bottom += (pDlg->_iOldDecalY - pDlg->_iDecalageY) ;
    X = ctrlRect.left - point.X() ;
    Y = ctrlRect.top - point.Y() ;
    TWindow* ctrlWin = new TWindow(hWnd) ;
    ::SetWindowPos(hWnd, HWND_TOPMOST, X, Y, ctrlRect.Width(), ctrlRect.Height(), SWP_SHOWWINDOW) ;

    if (-1 != ctrlID)
    {
      if (Y + ctrlRect.Height() > hScroll)
        ctrlWin->Show(SW_HIDE) ;
      else
        ctrlWin->Show(SW_SHOW) ;

      ::MoveWindow(hWnd, X, Y, ctrlRect.Width(), ctrlRect.Height(), true) ;
    }
    else
    {
      ::SendMessage(hWnd, WM_COMMAND, CM_HAUTEUR, 0) ;
      int hauteur = pDlg->_iVGroupHeight ;

      if (Y + hauteur > hScroll)
      {
        if (Y < hScroll)
        {
          // le groupe est en partie caché : on recalcule la hauteur
          hGroup = hScroll - Y ;
          ctrlWin->Show(SW_SHOW) ;
        }
        else
        {
          hGroup = ctrlRect.Height() ;
          ctrlWin->Show(SW_HIDE) ;
        }
      }
      else
      {
        hGroup = hauteur ;
        ctrlWin->Show(SW_SHOW) ;
      }

      ::MoveWindow(hWnd, X, Y, ctrlRect.Width(), hGroup, true) ;
    }

    delete ctrlWin ;

    pDlg->Invalidate() ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception nsrefDialog::DlgScrollCtrl.", standardError, 0) ;
  return false ;
}
}

bool FAR PASCAL _export
nsrefDialog::DlgScrollBtns(HWND hWnd, LPARAM lParam)
{
	// Adresse de l'objet boîte de dialogue courante
	TWindow* pWnd = reinterpret_cast<TWindow *>(lParam) ;
	nsrefDialog* pDlg = dynamic_cast<nsrefDialog *>(pWnd) ;

  int ctrlID  = ::GetDlgCtrlID(hWnd) ;
  NS_CLASSLIB::TRect ctrlRect ;
  NS_CLASSLIB::TRect dlgRect ;
  int X, Y ;
  HWND hInsert ;

  if ((ctrlID != VSCROLLID) &&
      ((ctrlID == IDOK) || (ctrlID == IDCANCEL) || (ctrlID == IDHELP) || (ctrlID == IDC_OTHER)))
  {
    ::GetWindowRect(hWnd, &ctrlRect) ;
    pDlg->GetClientRect(dlgRect) ;
    NS_CLASSLIB::TPoint point(dlgRect.X(), dlgRect.Y()) ;
    pDlg->ClientToScreen(point) ;
    X = ctrlRect.left - point.X() ;
    Y = dlgRect.Height() - pDlg->_iVSizeBtnBas ;
    hInsert = HWND_BOTTOM ;
    ::MoveWindow(hWnd, X, Y, ctrlRect.Width(), ctrlRect.Height(), true) ;
    ::SetWindowPos(hWnd, hInsert, X, Y, ctrlRect.Width(), ctrlRect.Height(), SWP_SHOWWINDOW) ;
    pDlg->Invalidate() ;
  }
  return true ;
}

void
nsrefDialog::PreCochePropositions()
{
	if (_pProposArray->empty())
		return ;

	VecteurString aNeeded ;

  NSSuper* pSuper = _pParseur->getSuper() ;
  pSuper->afficheStatusMessage("Précochage : Interrogation du blackboard...") ;

  for (NSProposIter i = _pProposArray->begin() ; _pProposArray->end() != i ; i++)
  {
    bool bValid = true ;

    // Does this proposition belong to the needed list
    //
    string sPropID = (*i)->propos->getStringAttribute(ATTRIBUT_PROP_ID) ;
    if ((string("") == sPropID) || (false == aNeeded.ItemDansUnVecteur(sPropID)))
      for (ValIter ival = (*i)->propos->getValArray()->begin() ; (*i)->propos->getValArray()->end() != ival ; ival++)
      {
        // s'il y a une clause de validité
        if (LABEL_VALIDITE == (*ival)->getLabel())
        {
          // on vérifie l'etat des variable à l'aide du NSValidateur
          Ccontrainte* pCvalidite = dynamic_cast<Ccontrainte *>((*ival)->getObject()) ;
          NSValidateur valid(pCvalidite, pContexte) ;
          // si OK, on coche la proposition
          if (false == valid.Validation())
          {
            bValid = false ;
            break ;
          }
        }
      }

    if (bValid)
		{
      if ((*i)->propos->getIsAutocheck())
			  (*i)->button->SetCheck(BF_CHECKED) ;
			(*i)->bInitialStateOn = true ;

      (*i)->propos->addNeededToVector(&aNeeded) ;
    }
  }

  pSuper->afficheStatusMessage("Précochage : terminé.") ;
}

void
nsrefDialog::CmOk()
{
  if (false == _pProposArray->empty())
    for (NSProposIter i = _pProposArray->begin() ; _pProposArray->end() != i ; i++)
    {
      if ((*i)->button->GetCheck() == BF_CHECKED)
        (*i)->propos->setCheck(true) ;
      else if ((*i)->bInitialStateOn)
      {
        (*i)->propos->bInitialyChecked = true ;
        (*i)->propos->setCheck(false) ;
      }

      if ((*i)->pReadyTree && (false == (*i)->pReadyTree->empty()))
      {
        if (NULL == (*i)->propos->pReasonTree)
          (*i)->propos->pReasonTree = new NSPatPathoArray(pContexte->getSuperviseur()) ;
        *((*i)->propos->pReasonTree) = *((*i)->pReadyTree) ;
      }
    }

  TDialog::CmOk() ;
}

void
nsrefDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

void
nsrefDialog::CmOther()
{
/*
  for (ValIter ival = pParseur->pReferentiel->getValArray()->begin() ; ival != pParseur->pReferentiel->getValArray()->end() ; ival++)
  {    if ((*ival)->sLabel == LABEL_PROPOSITION)    {      Cproposition* pCprop = dynamic_cast<Cproposition *>((*ival)->pObject) ;      pCprop->setTraite(false) ;    }  }*/
/*
  if (!pProposArray->empty())
    for (NSProposIter i = pProposArray->begin() ; i != pProposArray->end() ; i++)
    {
      if ((*i)->button->GetCheck() == BF_CHECKED)
        (*i)->propos->setCheck(true) ;
    }
*/

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("nsrefDialog::CmOther disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }

  TDialog::CloseWindow(IDC_OTHER) ;
}

void
nsrefDialog::CmBbkEvent()
{
  string ps = string("nsrefDialog reloading because of a IDC_BBK_EVENT message") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (false == _bOngoingSetupWindow)
    reinit() ;
  else
    _bReinitCalled = true ;
}

void
nsrefDialog::reinit()
{
  eraseControls() ;
  initVars() ;
  SetupWindow() ;
}

void
nsrefDialog::CmHelp()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;
	if (pSuper)
	{
		if (string("") != _sHelp)
		{
			pSuper->setAideIndex("") ;
			pSuper->setAideCorps(_sHelp) ;
		}
		else
		{
			pSuper->setAideIndex("") ;
			pSuper->setAideCorps("zz_generique.htm") ;
		}
	}
	pContexte->NavigationAideEnLigne() ;
}

// -----------------------------------------------------------------------------
//
// Classe NSControlTextArray
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur copie
// -----------------------------------------------------------------------------NSControlTextArray::NSControlTextArray(const NSControlTextArray& rv)                   :NSControlTextVector(){try{  if (false == rv.empty())    for (NSControlTextCIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSControlText(*(*i))) ;}catch (...)
{
  erreur("Exception NSControlTextArray copy ctor.", standardError, 0) ;
}}
NSControlTextArray&
NSControlTextArray::operator=(const NSControlTextArray& src)
{try{	if (this == &src)		return *this ;  vider() ;  if (false == src.empty())    for (NSControlTextCIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSControlText(*(*i))) ;  return (*this) ;}catch (...)
{
  erreur("Exception NSControlTextArray = operator.", standardError, 0) ;
  return (*this) ;
}}

voidNSControlTextArray::vider(){  if (false == empty())	  for (NSControlTextIter i = begin() ; end() != i ; )    {      delete (*i) ;      erase(i) ;    }}NSControlTextArray::~NSControlTextArray()
{	vider() ;}

// -----------------------------------------------------------------------------
//
// Classe NSControlText
//
// -----------------------------------------------------------------------------

NSControlText::NSControlText(Citem* pCitem)
{
  item    = pCitem ;
  button  = 0 ;
  texte   = 0 ;
  edit    = 0 ;
}


NSControlText::~NSControlText()
{
  // on ne delete pas les items car
  // ils sont rattachés au parseur
  if (button)
    delete button ;
  if (texte)
    delete texte ;
  if (edit)
    delete edit ;
}

NSControlText::NSControlText(const NSControlText& rv)
{
  item    = rv.item ;
  button  = rv.button ;
  texte   = rv.texte ;
  edit    = rv.edit ;
  fils    = rv.fils ;
}

NSControlText&
NSControlText::operator=(const NSControlText& src)
{
	if (this == &src)
		return *this ;

  item    = src.item ;
  button  = src.button ;
  texte   = src.texte ;
  edit    = src.edit ;
  fils    = src.fils ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
//
// Classe nsarcDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(nsarcDialog, TDialog)
  EV_COMMAND(IDCANCEL,     CmCancel),
	EV_COMMAND(IDOK,         CmOk),
END_RESPONSE_TABLE ;


nsarcDialog::nsarcDialog(TWindow* Parent, NSContexte* pCtx, nsarcParseur* parseur)
            :TDialog(Parent, "DLG_BASE", pNSDLLModule), NSRoot(pCtx)
{
try
{
  pParseur = parseur ;
  pControl = new NSControlText(parseur->getArchetype()->getRootItem()) ;
  InitControles(pControl->item, pControl) ;
}
catch (...)
{
  erreur("Exception nsarcDialog ctor.", standardError, 0) ;
}
}

nsarcDialog::~nsarcDialog()
{
  delete pControl ;
}

void
nsarcDialog::SetupWindow()
{
try
{
  NS_CLASSLIB::TRect cvtRect ;

  iBoxHeight = 10 ;    // hauteur du bouton
  iBoxWidth  = 150 ;   // largeur du bouton
  iBoxInterv = 3 ;     // intervalle entre deux boutons
  iBoxTop    = 4 ;     // haut du 1er bouton
  iBoxLeft   = 15 ;    // décalage gauche des boutons

  iSeparLeft = 3 ;     // gauche du séparateur
  iGB_Separ  = 3 ;     // intervalle entre le bas du groupbox et le séparateur
  iSepar_Btn = 8 ;     // intervalle entre le séparateur et le bouton

  iBtnHeight = 25 ;    // hauteur d'un bouton Borland  iBtnWidth  = 43 ;    // largeur d'un bouton Borland
  iBtnLeft   = 3 ;     // gauche du premier bouton
  iBtn_bas   = 3 ;     // intervalle entre le bas du bouton et le bas de la boite

  iTotalWidth = 0 ;  iTopOfBox = iBoxTop ;
  iID = 200 ;

  SetupControles(pControl, 0, iTopOfBox) ;

  // On fixe la taille de la boite de dialogue
  iTotalHeight = iTopOfBox + iGB_Separ + iSepar_Btn + iBtnHeight + iBtn_bas ;  iTotalWidth += iBoxLeft ;
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, iTotalWidth, iTotalHeight) ;  MapDialogRect(dlgSizeRect) ;  // On crée le séparateur  int iSepareTop = iTopOfBox + iGB_Separ ;

  cvtRect = NS_CLASSLIB::TRect(iSeparLeft, iSepareTop, iSeparLeft + iTotalWidth - (2 * iSeparLeft), iSepareTop + 3) ;  MapDialogRect(cvtRect) ;

  TGroupBox* pDip = new TGroupBox(this, -1, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*TModule**/ 0) ;  pDip->Attr.Style |= BSS_HDIP ;

  // On crée les boutons  int iNbBtn = 3 ;
  int iBtnInterv = (iTotalWidth - (2 * iBtnLeft) - (iNbBtn * iBtnWidth)) / (iNbBtn - 1) ;
  int iBtnTopPos  = iSepareTop + iSepar_Btn ;
  int iBtnLeftPos = iBtnLeft ;

  // Bouton OK
  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;  MapDialogRect(cvtRect) ;
  /* TButton* pBtOK = */ new TButton(this, IDOK, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;

  iBtnLeftPos += iBtnWidth + iBtnInterv ;
  // Bouton CANCEL  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;  MapDialogRect(cvtRect) ;
  /* TButton* pBtCn = */ new TButton(this, IDCANCEL, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
  iBtnLeftPos += iBtnWidth + iBtnInterv ;
  // Bouton HELP  cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + iBtnWidth, iBtnTopPos + iBtnHeight) ;  MapDialogRect(cvtRect) ;
  /* TButton* pBtHl = */ new TButton(this, IDHELP, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
  iBtnLeftPos += iBtnWidth + iBtnInterv ;
  // Titre de la boite de dialogue  SetCaption("Archétype") ;  // Appel du SetupWindow() de TDialog  TDialog::SetupWindow() ;

  // Redimentionnemen t
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

  // On compare le clientRect réel avec les dimensions souhaitées,  // et on modifie le WindowRect en conséquence
  int nouvWindowWidth  = dlgRect.Width()  + (dlgSizeRect.Width()  - clientRect.Width()) ;
  int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;

  MoveWindow(dlgRect.left, dlgRect.top, nouvWindowWidth, nouvWindowHeight) ;
}
catch (...)
{
  erreur("Exception nsarcDialog SetupWindow.", standardError, 0) ;
}
}

void
nsarcDialog::InitControles(Citem* item, NSControlText* control)
{
try
{
  if (item->vect_val.empty())
    return ;

  for (ValIter ival = item->vect_val.begin() ; item->vect_val.end() != ival ; ival++)
  {
    if (LABEL_ITEM == (*ival)->getLabel())
    {      // on doit insérer la valeur avant le petit frère      Citem* pCitem = dynamic_cast<Citem*>((*ival)->getObject()) ;
      if (pCitem)
      {
        if (pCitem->getControl() == string(VAL_ATTR_ITEM_CTRL_ACTIF))
        {
          NSControlText* pCont = new NSControlText(pCitem) ;
          control->fils.push_back(pCont) ;
          InitControles(pCitem, pCont) ;
        }
        else
          InitControles(pCitem, control) ;
      }
    }
  }
}
catch (...)
{
  erreur("Exception nsarcDialog InitControles.", standardError, 0) ;
}
}

void
nsarcDialog::SetupControles(NSControlText* control, int left, int& top)
{
try
{
  NS_CLASSLIB::TRect cvtRect ;

  if (control->item->getControl() == string(VAL_ATTR_ITEM_CTRL_ACTIF))
  {
    string sCode = control->item->getCode() ;
    if (sCode.find("£") != string::npos)
    {
      // Rectangle du texte de l'edit
      cvtRect = NS_CLASSLIB::TRect(left, top, left + iBoxWidth, top + iBoxHeight) ;      MapDialogRect(cvtRect) ;
      // Titre      string sTitre = control->item->getText() ;      // static      control->texte = new TStatic(this, -1, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;
      // Rectangle du champ edit
      cvtRect = NS_CLASSLIB::TRect(left, top + iBoxHeight, left + iBoxWidth, top + (2*iBoxHeight)) ;      MapDialogRect(cvtRect) ;
      // champ edit      control->edit = new TEdit(this, iID, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height()) ;      top += (2*iBoxHeight) + iBoxInterv ;
      iID++ ;
    }
    else // (bouton)
    {
      // Rectangle du bouton
      cvtRect = NS_CLASSLIB::TRect(left, top, left + iBoxWidth, top + iBoxHeight) ;      MapDialogRect(cvtRect) ;
      // Titre      string sTitre = control->item->getText() ;      // bouton      control->button = new nsrefButton(this, pContexte, iID, sTitre.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), 0, 0) ;
      top += iBoxHeight + iBoxInterv ;
      iID++ ;
    }
  }

  if (false == control->fils.empty())
  {
    if ((left + iBoxLeft + iBoxWidth) > iTotalWidth)
      iTotalWidth = left + iBoxLeft + iBoxWidth ;

    for (NSControlTextIter i = control->fils.begin() ; control->fils.end() != i ; i++)
      SetupControles(*i, left + iBoxLeft, top) ;
  }
}
catch (...)
{
  erreur("Exception nsarcDialog SetupControles.", standardError, 0) ;
}
}

void
nsarcDialog::CmOk()
{
  TDialog::CmOk() ;
}

void
nsarcDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

