//**************************************************************************//  NOM du FICHIER  : nstrnode.CPP
//  Kaddachi Hafedh 20/03/1997
//**************************************************************************

#include "partage\nsdivfct.h"
#include "partage\ns_timer.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbsmal.h"
#include "nssavoir\nspatho.h"
#include "nssavoir\nsconver.h"
#include "nssavoir\nsRightsManager.h"
#include "nsbb\nstrnode.rh"
#include "nsbb\nsbb.rh"
#include "nsbb\nStlibre.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nspardlg.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsednum.h"
#include "nsbb\nsdico.h"
#include "nsbb\nsedcon.h"
#include "nsbb\nsdlg.h"
//#include "nsbb\nsmanager.h"
#include "dcodeur\decoder.h"
#include "nautilus\nssoapview.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"
#include "nsbb\ns_skins.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsencycl.h"
#include "nsepisod\nsToDo.h"
#include "nsbb\nsbbdivfct.h"

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

#include "nsepisod\nsclasser.h"

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//									 Classe NSTreeWindow
//----------------------------------------------------------------------
//----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSTreeWindow, TTreeWindow)
  EV_NOTIFY_AT_CHILD(TVN_SELCHANGED, SelChanged),
  EV_WM_RBUTTONDOWN,
  EV_WM_SETFOCUS,
  EV_WM_KILLFOCUS,
  EV_WM_LBUTTONDBLCLK,  //double click
  EV_WM_MOUSEMOVE,

  //paramètres
  EV_COMMAND(IDC_DELETE,              EvDelete),
  EV_COMMAND(IDC_INSERTFRERE,         InsereAvant),
  EV_COMMAND(IDC_INSERTELEMENTFICTIF, InsereApres),
  EV_COMMAND(IDC_MODIFIER,            Modifier),
  EV_COMMAND(IDC_ABSENT,              Absence),
  EV_COMMAND(IDC_PRESENCE,            Presence),
  EV_COMMAND(IDC_PARAMETRES,          Parametres),
  EV_COMMAND(IDC_FILGUIDE,            EditionFilGuide),
  EV_COMMAND(IDC_ENCYCLOP,            Encyclop),
  EV_COMMAND(IDC_CERTIF,              Certificat),
  EV_COMMAND(IDC_CUT,                 CutNode),
  EV_COMMAND(IDC_COPY,                CopyNode),
  EV_COMMAND(IDC_PASTE,               PasteNode),
  EV_COMMAND(IDC_CLASSIF,             Classifier),
  EV_COMMAND(IDC_TREE_NEWPREOCCUP,    NewPreoccup),
  EV_COMMAND(IDC_CONNECTPREOCCUP,     ConnectToPreoccup),
  EV_COMMAND(IDC_EDITRIGHTS,     		  FixeDroits),
  EV_COMMAND(IDC_PASTE_TL,     		    InsertTlInEdit),
  EV_MESSAGE(IDC_DICO_LOST_FOCUS,     DicoLostFocus),

  //Drag And Drop
  EV_WM_TIMER,
  EV_WM_LBUTTONDOWN,
  EV_WM_LBUTTONUP,
  NS_TVN_BEGINDRAG(EvTvnBeginDrag),

  //TreeView
  NS_TV_KEYDOWN(EvTvKeyDown),
  EV_WM_KEYDOWN,
  EV_WM_KEYUP,
  NS_TVN_BEGINLABELEDIT(TvnBeginLabelEdit),
  NS_TVN_ITEMEXPANDING(TvnItemExpanDing),
  NS_TVN_ITEMEXPANDED(TvnItemExpanded),
  EV_WM_VSCROLL,
  EV_WM_HSCROLL,
  EV_WM_PAINT,
  EV_WM_CTLCOLOR,

END_RESPONSE_TABLE;

#define NODES_COL_BASE 1
#define NODES_LIN_BASE 1

//----------------------------------------------------------------------
//----------------------------------------------------------------------
NSTreeWindow::NSTreeWindow(TWindow* parent,NSContexte* pCtx,
                            int id, int x, int y, int w, int h,
                            TStyle style, TModule* module)
             :TTreeWindow(parent, id, x, y, w, h, style, module), NSRoot(pCtx),
              Encyclo(pCtx->getSuperviseur()), Converter(pCtx->getSuperviseur())
{
try
{
	bIniting                = true ;
  _bLoosingFocus          = false ;
  Aparent                 = parent ;
  _pEditDico              = (NSEditDico*) 0 ;
  ReadOnly                = false ;
  _pControle              = (NSControle*) 0 ;
  pChampEdition           = (ChampEdition*) 0 ;
  pPositionDragAndDrop    = (PositionDragAndDrop*) 0 ;
  pChampDate              = (NSDate*) 0 ;
  bDispatcherPatpatho     = true ;
  bDirty                  = false ;
  bLanceEdit              = false ;
  bPaint                  = false ;
  bEditionDico            = false ;
  bCurseurTexte           = false ;
  AfficheCopieCollee      = true ;
  skinName                = string("treeWindow") ;

  sLang                   = pContexte->getUserLanguage() ;

  openDbTables() ;

  pHitTestInfo = new TTwHitTestInfo ;

  // HINSTANCE Hinstance = pNSDLLModule->GetInstance();
  // HINSTANCE Hinstance1 = *GetApplication();
  pBBitemNSTreeWindow = 0 ;
  ModifierTexteLibre	= false ;
  Images = 0 ;

    /*
    style = GetStyle();
    style = TTreeWindow::TStyle(style | TTreeWindow::twsHasLines | TTreeWindow::twsHasButtons | TTreeWindow::twsEditLabels);
    #if defined(BI_PLAT_WIN32)
        style = TTreeWindow::TStyle(style | TTreeWindow::twsLinesAtRoot);
	#endif
    SetStyle(style);
    */

	uint32 ui32Style = GetStyle() ;
  ui32Style |= TTreeWindow::twsHasLines | TTreeWindow::twsHasButtons | TTreeWindow::twsEditLabels ;
#if defined(BI_PLAT_WIN32)
	ui32Style |= TTreeWindow::twsLinesAtRoot ;
#endif
	SetStyle(ui32Style) ;
}
catch (...)
{
	erreur("Exception (NSTreeWindow ctor).", standardError, 0);
}
}

//----------------------------------------------------------------------//			Constructeur
//----------------------------------------------------------------------
NSTreeWindow::NSTreeWindow(TWindow* parent, NSContexte* pCtx, int resourceId,
                           TModule* module)
             :TTreeWindow(parent, resourceId, module), NSRoot(pCtx),
              Encyclo(pCtx->getSuperviseur()), Converter(pCtx->getSuperviseur())
{
try
{
  bIniting            = true ;
  _bLoosingFocus      = false ;
  Aparent             = parent ;
  ReadOnly            = false ;
  _pControle          = (NSControle*) 0 ;
  _pEditDico          = (NSEditDico*) 0 ;

  bDirty              = false ;
  AfficheCopieCollee  = true ;
  bLanceEdit          = false ;
  bPaint              = false ;
  bEditionDico        = false ;
  bCurseurTexte       = false ;
  skinName            = string("treeWindow") ;

  sLang               = pContexte->getUserLanguage() ;

  openDbTables() ;

  pHitTestInfo         = new TTwHitTestInfo ;

  pBBitemNSTreeWindow  = (BBItem*) 0 ;
  Images               = (OWL::TImageList*) 0 ;
  pChampEdition        = (ChampEdition*) 0 ;
  pPositionDragAndDrop = (PositionDragAndDrop*) 0 ;
  pChampDate           = (NSDate*) 0 ;
  bDispatcherPatpatho  = true ;
  ModifierTexteLibre   = false ;
  // pNodeArray           = new NSTreeNodeArray() ;

  /*
  TStyle style = GetStyle();
  style = TTreeWindow::TStyle(style | TTreeWindow::twsHasLines | TTreeWindow::twsHasButtons | TTreeWindow::twsEditLabels);
  #if defined(BI_PLAT_WIN32)
    style = TTreeWindow::TStyle(style | TTreeWindow::twsLinesAtRoot);
#endif
  SetStyle(style);
  */

  uint32 ui32Style = GetStyle() ;
  ui32Style |= TTreeWindow::twsHasLines | TTreeWindow::twsHasButtons | TTreeWindow::twsEditLabels ;
  #if defined(BI_PLAT_WIN32)
      ui32Style |= TTreeWindow::twsLinesAtRoot ;
#endif
  SetStyle(ui32Style) ;
}
catch (...)
{
	erreur("Exception (NSTreeWindow ctor).", standardError, 0);
}
}

//----------------------------------------------------------------------
//			Destructeur
//----------------------------------------------------------------------
NSTreeWindow::~NSTreeWindow()
{
  string sMsg = string("Entering NSTreeWindow destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	if (_pControle)
 	{
    _pControle->setControle((void*) 0) ;
 		delete _pControle ;
    _pControle = 0 ;
 	}
  if (pHitTestInfo)
  {
    delete pHitTestInfo ;
    pHitTestInfo = 0 ;
  }
  if (Images)
  {
    delete Images ;
    Images = 0 ;
  }
  if (pChampEdition)
  {
    delete pChampEdition ;
    pChampEdition = 0 ;
  }
  if (pChampDate)
  {
    delete pChampDate;
    pChampDate = 0;
  }
  if (_pEditDico)
  {
    delete _pEditDico ;
    _pEditDico = (NSEditDico*) 0 ;
  }

  closeDbTables() ;

  if (pPositionDragAndDrop)
    delete pPositionDragAndDrop ;

  sMsg = string("Leaving NSTreeWindow destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
}

//---------------------------------------------------------------------------
//  Description:	Constructeur copie
//---------------------------------------------------------------------------
NSTreeWindow::NSTreeWindow(const NSTreeWindow& src)
             :TTreeWindow((TWindow *)src.GetParent(), src.GetId(), src.GetModule()),
              NSRoot(src.pContexte), Encyclo(src.pContexte->getSuperviseur()),
              Converter(src.pContexte->getSuperviseur()),
              _NodeArray(src._NodeArray)
{
	bIniting             = true ;
  _bLoosingFocus       = false ;

/*
  if (src.pNodeArray)
    pNodeArray         = new NSTreeNodeArray(*(src.pNodeArray)) ;
  else
    pNodeArray         = 0 ;
*/

  _pControle           = src._pControle ;
  bDirty               = src.bDirty ;
  _pEditDico           = src._pEditDico ;
  pNSTreeNodeEnCours   = src.pNSTreeNodeEnCours ;
  pBBitemNSTreeWindow  = src.pBBitemNSTreeWindow ;
  pChampEdition        = src.pChampEdition ;
  pChampDate           = src.pChampDate ;
  bDispatcherPatpatho  = src.bDispatcherPatpatho ;
  ModifierTexteLibre	 = src.ModifierTexteLibre ;
  pPositionDragAndDrop = src.pPositionDragAndDrop ;
  AfficheCopieCollee   = src.AfficheCopieCollee ;
  bLanceEdit           = src.bLanceEdit ;
  bPaint               = src.bPaint ;
  // to change
  // pNSCV                = src.pNSCV ;
  sLang                = src.sLang ;
  skinName             = src.skinName ;

  openDbTables() ;
}

//---------------------------------------------------------------------------
//  Description:	Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSTreeWindow&
NSTreeWindow::operator=(const NSTreeWindow& src)
{
	if (this == &src)
		return *this ;

	_NodeArray  	  	   = src._NodeArray ;
  pContexte  			     = src.pContexte ;
  bDirty				       = src.bDirty ;
  _pControle  			   = src._pControle ;
  _pEditDico  		     = src._pEditDico ;
  pChampEdition		     = src.pChampEdition ;
  pChampDate			     = src.pChampDate ;
  pNSTreeNodeEnCours 	 = src.pNSTreeNodeEnCours ;
  pBBitemNSTreeWindow  = src.pBBitemNSTreeWindow ;
  bDispatcherPatpatho  = src.bDispatcherPatpatho ;
  ModifierTexteLibre	 = src.ModifierTexteLibre ;
  pPositionDragAndDrop = src.pPositionDragAndDrop ;
  AfficheCopieCollee   = src.AfficheCopieCollee ;
  bLanceEdit 			     = src.bLanceEdit ;
  bPaint				       = src.bPaint ;
  // pNSCV				         = src.pNSCV ;
  sLang                = src.sLang ;
  skinName             = src.skinName ;

  return *this ;
}

void
NSTreeWindow::openDbTables()
{
  DBIResult Resultat = Encyclo.open() ;
  if (DBIERR_NONE != Resultat)
  {
    string sErrMsg = pContexte->getSuperviseur()->getText("treeViewErrors", "cannotOpenEncyclopedia") ;
    pContexte->getSuperviseur()->trace(&sErrMsg, 1, NSSuper::trError) ;
  	erreur(sErrMsg.c_str(), standardError, Resultat) ;
  }

  Resultat = Converter.open() ;
  if (DBIERR_NONE != Resultat)
  {
    string sErrMsg = pContexte->getSuperviseur()->getText("treeViewErrors", "cannotOpenUnitConversionFile") ;
    pContexte->getSuperviseur()->trace(&sErrMsg, 1, NSSuper::trError) ;
  	erreur(sErrMsg.c_str(), standardError, Resultat) ;
  }
}

void
NSTreeWindow::closeDbTables()
{
  string sMsg = string("NSTreeWindow, closing encyclo and converter tables") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  if (Encyclo.isOpened())
    Encyclo.close() ;
  if (Converter.isOpened())
    Converter.close() ;
}

//---------------------------------------------------------------------------
// SetupWindow
//---------------------------------------------------------------------------
void
NSTreeWindow::SetupWindow()
{
try
{
	bIniting = true ;

	TTreeWindow::SetupWindow() ;

	skinSwitchOn(skinName) ;

	Images = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 7, 5) ;  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_0PLUS)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_1PLUS)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_2PLUS)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_3PLUS)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_4PLUS)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_TEXTELIBRE)) ;
  Images->Add(OWL::TBitmap(*pNSDLLModule, IDB_PROP)) ;
  SetImageList(TTreeWindow::Normal, *Images) ;

    //    // Pointeur vers le générateur - Langage generator's pointer
    //
    /* if (!(pContexte->getDico()->pGenerateur))
        TDialog::SetupWindow();
    NSGenerateur* pGene = pContexte->getDico()->pGenerateur;*/

	//
  //afficher les fils de GCONS1 (BBFilsItem lanceur de la consultation)
  //
  if (((BBItem*) NULL == pBBitemNSTreeWindow) ||
                                 (pBBitemNSTreeWindow->_aBBItemFils.empty()))
  {
  	bIniting = false ;
    return ;
  }

  BBFilsItem* GCONS1 = (BBFilsItem*) 0 ;
  if (_pControle && _pControle->getTransfert() &&
                                    _pControle->getTransfert()->getFilsItem())
		GCONS1 = _pControle->getTransfert()->getFilsItem() ;
	else
  	GCONS1 = *(pBBitemNSTreeWindow->_aBBItemFils.begin()) ;

  if (((NSVectFatheredPatPathoArray*) NULL == GCONS1->getPatPatho()) ||
                                             (GCONS1->getPatPatho()->empty()))
  {
  	bIniting = false ;
    return ;
  }
  // NSPatPathoArray* ContenuPatpatho = *(GCONS1->getPatPatho()->begin());

  TTreeNode root = GetRoot() ;
  //
  // Prendre les fis de GCONS1  si la patpatho est vide
  //
  if ((NULL == pBBitemNSTreeWindow->_pPPTEnCours) ||
      (false == pBBitemNSTreeWindow->_pPPTEnCours->empty()))
  {
  	bIniting = false ;
		return ;
	}

	int compteur = NODES_LIN_BASE ;

  BBItem* pBBitem = *(GCONS1->VectorFils.begin()) ;

  if (((BBItem*) NULL == pBBitem) || (pBBitem->_aBBItemFils.empty()))
  {
    bIniting = false ;
    return ;
  }

  BBiter fils = pBBitem->_aBBItemFils.begin() ;
  for (; pBBitem->_aBBItemFils.end() != fils ; fils++)
  {
    string sLabel = (*fils)->getItemLabel() ;
    string sLibelleTrouve ;
    pContexte->getDico()->donneLibelle(sLang, &sLabel, &sLibelleTrouve) ;

    TTreeNode* pAmorce = new TTreeNode(*this, TVI_FIRST) ;
    *pAmorce = root.AddChild(*pAmorce) ;
    NSTreeNode* pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
    // MAJ ligne et colonne
    pNewNSTreeNode->setLigne(compteur) ;
    pNewNSTreeNode->setColonne(NODES_COL_BASE) ;
    pNewNSTreeNode->SetFatherNode((NSTreeNode*) 0) ;
    pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;
    pNewNSTreeNode->SetLexique((*fils)->getItemLabel()) ;
    pNewNSTreeNode->SetNoeud((*fils)->getItemTransfertMsg()->GetNoeud()) ;
    pNewNSTreeNode->SetLabelString(sLibelleTrouve) ;
    pNewNSTreeNode->SetText(pNewNSTreeNode->getLabelString().c_str(), true) ;
    pNewNSTreeNode->SetControl((NSControle*) 0) ;
    pNewNSTreeNode->SetProposed(true) ;
    pNewNSTreeNode->SetImageIndex(6) ;
    pNewNSTreeNode->SetSelectedImageIndex(6, true) ;

    _NodeArray.push_back(pNewNSTreeNode) ;

    // Libellé
    string sChemin ;
    pNewNSTreeNode->formerChemin(&sChemin) ;
    NSCutPaste CutPaste(pContexte->getSuperviseur()) ;
    pNewNSTreeNode->formerPatPatho(&CutPaste) ;

    NSPatPathoArray CutPastePpt(pContexte->getSuperviseur()) ;
    CutPaste.initFromPatPatho(&CutPastePpt) ;

    GlobalDkd Dcode(pContexte, sLang, sChemin, &CutPastePpt) ;
    Dcode.decodeNoeud(sChemin) ;
    pNewNSTreeNode->SetLabelString(Dcode.getDcodeur()) ;

    // pAmorce->SelectItem(TVGN_CARET) ;
    // Valider() ;

    compteur++ ;
    Update() ;

    delete pAmorce ;
  }
//*******************************************************************
  //
  // Validation des noeuds de la colonne 0
  //
  TTreeNode firstNode = GetFirstVisible() ;
  if (NULL != firstNode)
  {
    // Puisque les éléments se désélectionnent au fur et à mesure
    // qu'on descend, il faut d'abord valider les éléments racines
    // du haut en bas, puis ouvrir leurs fils du bas en haut
    //
    firstNode.SelectItem(TVGN_CARET) ;
    Valider() ;

    TTreeNode nextNode = firstNode.GetNextItem(TVGN_NEXT) ;
    while (NULL != nextNode)
    {
      nextNode.SelectItem(TVGN_CARET) ;
      Valider() ;

      nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
    }

    nextNode = firstNode.GetNextItem(TVGN_NEXT) ;
    TTreeNode lastNode(nextNode) ;
    while (NULL != nextNode)
    {
      lastNode = nextNode ;
      nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
    }

    nextNode = lastNode ;
    while (NULL != nextNode)
    {
      nextNode.SelectItem(TVGN_CARET) ;
      Valider(true) ;

      nextNode = nextNode.GetNextItem(TVGN_PREVIOUS) ;
    }
  }

  // Current algorithm push them in a sorted way already
  // It's cleaner to explicitly do it anyway
  //
  _NodeArray.Sort() ;

//****************************************************************/
}
catch (...)
{
	erreur("Exception (NSTreeWindow::SetupWindow).", standardError, 0) ;
}
  bIniting = false ;
}

void
NSTreeWindow::skinSwitchOn(string sSkinName)
{
  // bool bSkinable = false ;

  nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;
  if (pSkin)
  {
    ClassLib::TColor* pBackColor        = pSkin->getBackColor() ;
    // ClassLib::TColor* pBackTextColor    = pSkin->getFontBackColor() ;
    // ClassLib::TColor* pTextColor        = pSkin->getFontColor() ;

    if (pBackColor)
    {
      // bSkinable = true ;
      SetBkgndColor(*pBackColor) ;
    }
  }

    /******************************
    if (!bSkinable)
    {
        SetBkgndColor(0x00fff0f0) ; // 0x00bbggrr
    }
    *******************************/

  Invalidate() ;
}

void
NSTreeWindow::skinSwitchOff(string sSkinName)
{
  bool bSkinable = false ;

  nsSkin* pSkin = pContexte->getSkins()->donneSkin(sSkinName) ;
  if (pSkin)
  {
    ClassLib::TColor* pBackColor        = pSkin->getBackColor() ;
    // ClassLib::TColor* pBackTextColor    = pSkin->getFontBackColor() ;
    // ClassLib::TColor* pTextColor        = pSkin->getFontColor() ;

    if (pBackColor)
    {
      bSkinable = true ;
      SetBkgndColor(*pBackColor) ;
    }
  }

  if (false == bSkinable)
    SetBkgndColor(0x00ffffff) ; // 0x00bbggrr

  Invalidate() ;
}

//-----------------------------------------------------------------
//libérer le BBItem lanceur de la consultation
//-----------------------------------------------------------------
void
NSTreeWindow::LibereBBitemLanceur()
{
  if (pBBitemNSTreeWindow)
  {
    delete pBBitemNSTreeWindow ;
    pBBitemNSTreeWindow = (BBItem*) 0 ;
  }
}

//----------------------------------------------------------------
//former la patpatho globale
//----------------------------------------------------------------
void
NSTreeWindow::okFermerDialogue()
{
  if ((BBItem*) NULL == pBBitemNSTreeWindow)
    return ;

	//false pour ne pas tuer pTransfert lié à pBBitemNSTreeWindow
	pBBitemNSTreeWindow->okFermerDialogue(true, false) ;
}

//----------------------------------------------------------------
//vider l'array des tree nodes du controle TreeWindow
//----------------------------------------------------------------
void
NSTreeWindow::SupprimerTousNoeuds()
{
  if (_NodeArray.empty())
    return ;

  for (iterNSTreeNode iter = _NodeArray.begin() ; _NodeArray.end() != iter ; )
    SupprimerNoeud(*iter) ;
}

//-----------------------------------------------------------------
//
//extraire la patpatho de la consultation : c'est la patpatho
//BBIFilsItem lanceur de la consultation et la donner au pBigBoss
//
//-----------------------------------------------------------------
void
NSTreeWindow::EnregistrePatPatho(NSSmallBrother* pBigBoss)
{
  if ((NSSmallBrother*) NULL == pBigBoss)
    return ;

  NSPatPathoArray* pPPT = pBigBoss->getPatPatho() ;

  BBFilsItem* pBBFilsLanceur = *(pBBitemNSTreeWindow->_aBBItemFils.begin()) ;
  NSFatheredPatPathoArray* pFatheredElement = *(pBBFilsLanceur->getPatPatho()->begin()) ;
  if (pPPT)
    *pPPT = *(pFatheredElement->getPatPatho()) ;

  NSPatPathoInfo* pPptInfo = pFatheredElement->getFatherNode() ;
  if (pPptInfo && (string("") == pBigBoss->_sNoeudModule))
    pBigBoss->_sNoeudModule = pPptInfo->getNodeID() ;
}

//----------------------------------------------------------------------------
// a partir d'une ligne et d'une colonne d'un NStreeNode , on lui trouve son père
//----------------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::TrouverPere(int LigneFils, int ColonneFils)
{
  if (_NodeArray.empty())
    return (NSTreeNode*) 0 ;

  iterNSTreeNode iter = _NodeArray.begin() ;
 	iterNSTreeNode pere = _NodeArray.end() ;
 	iterNSTreeNode jter = _NodeArray.begin() ;

	// On cherche le premier NStreeNode dont la ligne est < à LigneFils
  // et la colonne est = à (ColonneFils - 1)
  //
  bool trouve = false ;

 	for (iter = _NodeArray.begin() ; (_NodeArray.end() != iter) && (false == trouve) ; iter++) 	{
    if (((*iter)->getLigne() < LigneFils) &&
        ((*iter)->getColonne() == (ColonneFils - 1)))
    {
      trouve = true ;
      pere = iter ;
    }
  }

  // Si trouvé
  if (_NodeArray.end() != pere)
  {
    // Chercher éventuellement un autre père dont la ligne est < à celle
    // du premier père trouvé
    jter = pere ;
    jter++ ;
    while (_NodeArray.end() != jter)
    {
      if (((*jter)->getLigne() < LigneFils) &&
                ((*jter)->getColonne() == (ColonneFils - 1)))
        pere = jter ;
      jter++ ;
    }
  }

  if (_NodeArray.end() == pere)
    return (NSTreeNode*) 0 ;

  if ((*pere)->getPilotNode())
    return((*pere)->getPilotNode()) ;

  return(*pere) ;
}

//-----------------------------------------------------------------------
//créer un NSTReeNode en lui attribuant une ligne, une colonne et son père
// s'il existe sinon 0 , son identité , son label, son code texte libre
//-----------------------------------------------------------------------
void
NSTreeWindow::CreerNSTreeNode(NSTreeNode* pNewNSTreeNode, int ligne, int colonne, string sIdentite,
				                     string sLabel, string sCodeTexteLibre)
{
  if ((NSTreeNode*) NULL == pNewNSTreeNode)
    return ;

  pNewNSTreeNode->SetControl((NSControle*) 0) ;
  pNewNSTreeNode->setLigne(ligne) ;
  pNewNSTreeNode->setColonne(colonne) ;
  pNewNSTreeNode->SetLexique(sIdentite) ;
  pNewNSTreeNode->SetFreeTextCode(sCodeTexteLibre) ;
  pNewNSTreeNode->SetLabelString(sLabel) ;

  _NodeArray.AddElement(pNewNSTreeNode) ;

  MiseAjourNSTreeNode(pNewNSTreeNode) ;
}

//-----------------------------------------------------------------------
//dispatcher  ContenuPatpatho qui contient le contenu total de la patpatho :
//Si pNSTreeNodeLanceur est nul, c'est le root qui affiche ContenuPatpatho
//		sinon si sHowToInsert == "AVANTFRERE"
//          	pNSTreeNodeLanceur crée le pathos de colonne 0 en tant que frères
//				si sHowToInsert == "AVANTFILS"
//          	pNSTreeNodeLanceur crée le pathos de colonne 0 en tant que fils directs
//sExpand 	si chaque père expand ses fils ou non, par défaut = ""
//-----------------------------------------------------------------------
void
NSTreeWindow::DispatcherPatPatho(const NSPatPathoArray* ContenuPatpatho,
                                 NSTreeNode* pNSTreeNodeLanceur,
                                 int* pDecalageLigneFils,
                                 string sHowToInsert, string sExpand,
                                 NSCutPastTLArray* pTLArray)
{
try
{
	if (((NSPatPathoArray*) NULL == ContenuPatpatho) || ContenuPatpatho->empty())
	{
  	// Il ne faut pas tenter de dispatcher global par la suite, sinon on
    // obtient des duplication de noeuds lors du okFermerDialog
    //
    bDispatcherPatpatho = false ;
    return ;
	}

	string sIdentitePere = string("") ;  //code lexique  du NStreeNode père
  string sIdentiteFils = string("") ;  //code lexique du NStreeNode fils
  string sNoeud        = string("") ;  //code du noeud
  PatPathoConstIter iterTReeView = ContenuPatpatho->begin() ;

  int LigneFils   = 0 ; //ligne pour les noeuds fils
  int lignePere   = 1 ; //ligne pour les NStreeNodes père

  int ColonneFils = 0 ; //colonne pour les noeuds fils
  int colonnePere = 1 ; //colonne pour les NStreeNodes père

  //
  // Décalage en ligne et colonne des éléments de ContenuPatpatho lorsqu'ils
  // sont intégrés dans la treeWindow
  //
  int decalageLanceurColonne = 0 ;
  int decalageLanceurLigne   = 0 ;
  if (pNSTreeNodeLanceur)
  {
  	int ligneMax = 0 ;
    int colonneMax = 0 ;

    // On insère avant les fictifs, d'où le true
    GetMaxCoordonnees(pNSTreeNodeLanceur, &ligneMax, &colonneMax, true) ;

    // décalage des colonnes
    if      (string("AVANTFRERE") == sHowToInsert)
    	decalageLanceurColonne = pNSTreeNodeLanceur->getColonne() - 1 ;
    else if (string("AVANTFILS")  == sHowToInsert)
    	decalageLanceurColonne = pNSTreeNodeLanceur->getColonne() ;
    else
    	decalageLanceurColonne = pNSTreeNodeLanceur->getColonne() ;

    //décalage des lignes
    if      (string("AVANTFRERE") == sHowToInsert)
    	decalageLanceurLigne = ligneMax + 1 ;
    else if (string("AVANTFILS")  == sHowToInsert)    	decalageLanceurLigne = pNSTreeNodeLanceur->getLigne() ;
    else
    	decalageLanceurLigne = ligneMax ;
	}
  else
  {
    // Means : insert at end
    //
    if (string("AVANTFRERE") == sHowToInsert)
    {
      NSTreeNode* pMaxRootNode = TrouverGrandFrereFilsRoot(NULL, std::numeric_limits<int>::max()) ;
      if (pMaxRootNode)
      {
        int ligneMax   = 0 ;
        int colonneMax = 0 ;
        GetMaxCoordonnees(pMaxRootNode, &ligneMax, &colonneMax, false) ;
    	  decalageLanceurLigne = ligneMax ;
      }
    }
  }

  string pLibelleTrouve  = string("") ;
  string sLabel          = string("") ;

  string sCodeTexteLibre = string("") ;
  string sTexteLibre     = string("") ;
  int NbTexteLibre = 0 ;   //nb de frères fictifs qui vont supporter le texte libre
  bool texteRecupere ;
  //NSTlibre* pNSTlibre = new NSTlibre(pContexte);

  // When inserting several nodes, the reference node must be the previously
  // inserted node, or they will be inserted in reverse order
  //
  NSTreeNode* pNSTreeNodeReference = pNSTreeNodeLanceur ;

  //
  // On passe en revue toute la patpatho
  //
	while (ContenuPatpatho->end() != iterTReeView)
	{
  	//
    // Si on est sur la colonne "0'
    //
    if ((*iterTReeView)->getColonne() == ORIGINE_PATH_PATHO)
    {
      // Modif 23/11/11
    	// colonnePere = (*iterTReeView)->getColonne() + decalageLanceurColonne ;
      // lignePere   = (*iterTReeView)->getLigne()   + decalageLanceurLigne ;
      colonnePere = NODES_COL_BASE + decalageLanceurColonne ;
      lignePere   = (*iterTReeView)->getLigne() - ORIGINE_PATH_PATHO + NODES_LIN_BASE + decalageLanceurLigne ;
      // On tient compte du décalage du aux textes libres multi-lignes      lignePere += NbTexteLibre ;

      TTreeNode* pAmorce = (TTreeNode*) 0 ;
      TTreeNode  root    = GetRoot() ;

      if (((*iterTReeView)->getLexique() != "900001") &&
          ((*iterTReeView)->getLexique() != "900002"))
      	pAmorce = new TTreeNode(*this, TVI_FIRST) ;

      NSTreeNode* pNewNSTreeNode = (NSTreeNode*) 0 ;

      //
      // Si le dispatching se fait à partir d'un noeud, les éléments
      // de la colonne '0' sont créés comme frères de ce noeud si
      // sHowToInsert == "AVANTFRERE" ou comme fils de ce noeud si
      // sHowToInsert == "AVANTFILS"
      //
      if (pNSTreeNodeReference)
      {
      	if (sHowToInsert == "AVANTFRERE")// pNSTreeNodeReference crée son frère
      	{
        	NSTreeNode* pNSTreeFrereCreateur = (NSTreeNode*) 0 ;
          // pas de frères liés : pNSTreeNodeLanceur crée son 1er frère
          if (pNSTreeNodeLanceur->getVectLinkedBros()->empty())
          	pNSTreeFrereCreateur = pNSTreeNodeReference ;
          // sinon, le dernier élément de VectFrereLie crée son frère
          else
          	// pNSTreeFrereCreateur = pNSTreeNodeReference->VectFrereLie.back() ;
            pNSTreeFrereCreateur = pNSTreeNodeLanceur->getVectLinkedBros()->back() ;
          //
          // Création et insertion physique du noeud (TTreeNode)
          //
          if (pAmorce)
          {
          	*pAmorce = pNSTreeFrereCreateur->InsertItem(TTreeNode(*this, "")) ;
            //
            // Création du NSTreeNode à partir du TTreeNode
            //
            pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
          }
          else // constructeur d'un NSTreeNode "private"
          	pNewNSTreeNode = new NSTreeNode(*this, pContexte, "PRIVATE") ;
          //
          // Référencement du père et de lui même dans l'array de fils
          // de son père
          //
          pNewNSTreeNode->SetFatherNode(pNSTreeNodeReference->getFatherNode()) ;
          if (pNewNSTreeNode->getFatherNode())
            pNSTreeNodeReference->getFatherNode()->getVectSons()->Referencer(pNewNSTreeNode) ;

          // The new node becomes the reference in order to create the next node
          //
          pNSTreeNodeReference = pNewNSTreeNode ;
        }
        else if (sHowToInsert == "AVANTFILS")//pNSTreeNodeLanceur crée son fils direct
        {
        	if (pAmorce)
          {
          	// Si le noeud lanceur n'a pas de frère liés, il crée son fils
            if (pNSTreeNodeReference->getVectLinkedBros()->empty()) //pas de frères liés
            	*pAmorce = pNSTreeNodeReference->InsertChild(*pAmorce, TTreeNode::First) ;
            // sinon c'est le dernier frère lié qui le fait
            else
            {
            	NSTreeNode*	pNSTreeFrere = pNSTreeNodeReference->getVectLinkedBros()->back() ; //dérnier élément de VectFrereLie crée son frère
              *pAmorce = pNSTreeFrere->InsertChild(*pAmorce, TTreeNode::First) ;
            }
            //
            // Création du NSTreeNode à partir du TTreeNode
            //
            pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
          }
          else
          	pNewNSTreeNode = new NSTreeNode(*this, pContexte, "PRIVATE") ;
          //
          // Référencement du père et de lui même dans l'array de fils
          // de son père
          //
          pNewNSTreeNode->SetFatherNode(pNSTreeNodeReference) ;
          pNSTreeNodeReference->getVectSons()->Referencer(pNewNSTreeNode) ;

          // The new node becomes the reference in order to create the next node
          // and the mode changes in order to create the next nodes as brothers
          // of the new node
          //
          pNSTreeNodeReference = pNewNSTreeNode ;
          sHowToInsert = string("AVANTFRERE") ;
        }
        else //dernier fils de pNSTreeNodeLanceur
        {
        	NSTreeNode* pNSTreeNodeFrere = (NSTreeNode*) 0 ;

          //
          // On insère après le dernier vrai fils
          //
          if (pNSTreeNodeLanceur->getVectSons()->empty())
          {
          	if (pAmorce)
            	*pAmorce = pNSTreeNodeLanceur->AddChild(*pAmorce) ;
          }
          else
          {
            iterNSTreeNode filsIt = pNSTreeNodeLanceur->getVectSons()->begin() ;
            iterNSTreeNode vraiFilsIt;
            for (; pNSTreeNodeLanceur->getVectSons()->end() != filsIt ; filsIt++)
            {
            	if (((*filsIt)->isProposed()) || ((*filsIt)->estFictif()))
              	break ;
              else
              	vraiFilsIt = filsIt ;
            }
            if      (pNSTreeNodeLanceur->getVectSons()->end() == filsIt)
            {
            	if (pAmorce)
              	*pAmorce = pNSTreeNodeLanceur->AddChild(*pAmorce) ;
            }
            else if (filsIt == pNSTreeNodeLanceur->getVectSons()->begin())
            {
            	if (pAmorce)
              	*pAmorce = pNSTreeNodeLanceur->InsertChild(*pAmorce, TTreeNode::First) ;
              pNSTreeNodeFrere = *filsIt ;
            }
            else
            {
            	if (pAmorce)
              	*pAmorce = (*vraiFilsIt)->InsertItem(*pAmorce) ;
              pNSTreeNodeFrere = *vraiFilsIt ;
            }
          }

          if (pAmorce)
          	pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
          else
          	pNewNSTreeNode = new NSTreeNode(*this, pContexte, "PRIVATE") ;

          pNewNSTreeNode->SetFatherNode(pNSTreeNodeLanceur) ;
          pNSTreeNodeLanceur->getVectSons()->Referencer(pNewNSTreeNode, pNSTreeNodeFrere) ;
        }
      }
      //
      // Dispatching à partir de la racine (root)
      //
      else
      {
      	if (pAmorce)
        {
        	if (sHowToInsert == "Debut")
          	*pAmorce = root.InsertChild(*pAmorce, TTreeNode::First) ;
          else
          	*pAmorce = root.AddChild(*pAmorce) ;
          pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
        }
        else
        	pNewNSTreeNode = new NSTreeNode(*this, pContexte, "PRIVATE") ;
        pNewNSTreeNode->SetFatherNode((NSTreeNode*) 0) ;
      }

      pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;

      //
      // Initialisation des champs du noeud à partir de l'élément
      // créateur de la patpatho
      //

      // Etiquette du BBFilsItem ou £????? pour texte libre
      sIdentitePere   = (*iterTReeView)->getLexique() ;
      sCodeTexteLibre = (*iterTReeView)->getComplement() ;      pNewNSTreeNode->SetNoeud((*iterTReeView)->getNode()) ;
      pNewNSTreeNode->SetUnit((*iterTReeView)->getUnit()) ;
      pNewNSTreeNode->SetInteret((*iterTReeView)->getInteret()) ;
      pNewNSTreeNode->SetPluriel((*iterTReeView)->getPluriel()) ;
      pNewNSTreeNode->SetCertitude((*iterTReeView)->getCertitude()) ;
      pNewNSTreeNode->SetVisible((*iterTReeView)->getVisible()) ;
      pNewNSTreeNode->SetRights((*iterTReeView)->getNodeRight()) ;

      pNewNSTreeNode->SetGlobalText((*iterTReeView)->getTexteLibre()) ;

      // Gestion de la NSCutPastTLArray
      // En cas de copier coller avec texte libre
      //
      if ((string("£?????") == sIdentitePere) && pTLArray)
      {
      	if ((false == pTLArray->empty()) && (string("") != sCodeTexteLibre))
        {
        	CutPastTLIter i ;
          for (i = pTLArray->begin();
                                (pTLArray->end() != i) &&
                                    (sCodeTexteLibre != (*i)->sIndice); i++) ;
          if (pTLArray->end() != i)
          {
          	sCodeTexteLibre = "" ;
            sTexteLibre     = (*i)->sTexte ;
          }
        }
      }
      else
      {
      	pNewNSTreeNode->SetComplement((*iterTReeView)->getComplement()) ;
        sTexteLibre = pNewNSTreeNode->getGlobalText() ;
      }

      // Instancier pNewNSTreeNode
      bool bTexte = pNewNSTreeNode->MettreAjourLabel(/*pNSTlibre*/ 0, &texteRecupere,
         						&NbTexteLibre, lignePere, colonnePere, sTexteLibre,
                                sCodeTexteLibre, sIdentitePere, this, sLang) ;
      if (false == bTexte)
      {
        string sErrMsg = pContexte->getSuperviseur()->getText("treeViewErrors", "errorCreatingANewNode") ;
        pContexte->getSuperviseur()->trace(&sErrMsg, 1, NSSuper::trError) ;
        erreur(sErrMsg.c_str(), standardError, 0) ;
        return ;
      }
      // Pas de texte libre
      if (string("£?????") != sIdentitePere)
      {
      	string sChemin, sCheminLocal ;
        pNewNSTreeNode->formerChemin(&sChemin) ;
        pNewNSTreeNode->formerCheminLocal(&sCheminLocal) ;
        NSCutPaste CP(pContexte->getSuperviseur()) ;
        pNewNSTreeNode->formerPatPatho(&CP) ;

        NSPatPathoArray CutPastePpt(pContexte->getSuperviseur()) ;
        CP.initFromPatPatho(&CutPastePpt) ;

        GlobalDkd Dcode(pContexte, sLang, sChemin, &CutPastePpt) ;
        Dcode.decodeNoeud(sCheminLocal) ;
        pNewNSTreeNode->SetLabelString(Dcode.getDcodeur()) ;

        pNewNSTreeNode->SetText(pNewNSTreeNode->getLabelString().c_str(), true) ;
        // pNewNSTreeNode->InstancieUrl(&Encyclo) ;//Encyclopédie.db
      }

      if (pAmorce)
      	delete pAmorce ;
    }
    //****************************************************************
    // On n'est pas sur la colonne 0, on dispatche donc un fils
    //****************************************************************
    else
    //***************************  fils  *****************************
    {
      // Modif du 23/11/11
    	// ColonneFils = (*iterTReeView)->getColonne() + decalageLanceurColonne ;
      // LigneFils   = (*iterTReeView)->getLigne()   + decalageLanceurLigne ;
      ColonneFils = (*iterTReeView)->getColonne() - ORIGINE_PATH_PATHO + NODES_COL_BASE + decalageLanceurColonne ;
      LigneFils   = (*iterTReeView)->getLigne()   - ORIGINE_PATH_PATHO + NODES_LIN_BASE + decalageLanceurLigne ;

      // On tient compte du décalage du aux textes libres multi-lignes
      LigneFils += NbTexteLibre ;

      string sComplement = string("") ;
      string sLabel      = string("") ;
      sIdentiteFils = (*iterTReeView)->getLexique() ;
      sNoeud        = (*iterTReeView)->getNode() ;

      string sSensFils = (*iterTReeView)->getLexiqueSens() ;

      //
      // cas particulier : si sEtiquettePatpatho contient £C prendre
      // les données dans l'élément suivant de la patpatho qui, lui
      // (controle fictif) contient les vraies données à afficher.
      // Exemple en Echo : Chimiothérapie
      // Dans ce cas afficher l'élément suivant
      PatPathoConstIter iterFictif  = ContenuPatpatho->begin() ;
      if ((sIdentiteFils.find(string("£C;")) != NPOS) ||
          (sIdentiteFils.find(string("/£C;")) != NPOS) ||
          (sIdentiteFils.find(string("£CX")) != NPOS))
      {
        if (string("") == (*iterTReeView)->getTexteLibre())
        {
      	  iterFictif = iterTReeView ;
          iterFictif++ ;
          if ((ContenuPatpatho->end() != iterFictif) &&
              ((*iterFictif)->getColonne() == (*iterTReeView)->getColonne() + 1))
          {
        	  iterTReeView++ ;
            sIdentiteFils = (*iterTReeView)->getLexique() ;
          }
        }
      }

      //
      // Ajout du noeud comme fils de pNewNSTreeNodePere
      //
      TTreeNode* pAmorce = (TTreeNode*) 0 ;
      NSTreeNode* pNewNSTreeNodePere ;
      NSTreeNode* pNewNSTreeNodeFils ;

      if ((string("900001") != sIdentiteFils) &&
          (string("900002") != sIdentiteFils))
      {
      	pAmorce = new TTreeNode(*this, "") ;
        // ATTENTION : l'échelle pour les patpatho commence à 0 alors
        // que pour les NSTreeNodes elle commence à 1
        pNewNSTreeNodePere = TrouverPere(LigneFils, ColonneFils) ;
        if (pNewNSTreeNodePere)
        {
          // S'il n'existe pas de frères liés, c'est le père qui crée son 1er fils
          if (pNewNSTreeNodePere->getVectLinkedBros()->empty())
        	  *pAmorce = pNewNSTreeNodePere->AddChild(*pAmorce/*, TTreeNode::First*/) ;
          // Sinon, le dernier élément de VectFrereLie crée son frère
          else
          {
        	  NSTreeNode*	pNSTreeFrere = pNewNSTreeNodePere->getVectLinkedBros()->back() ;
            *pAmorce = pNSTreeFrere->InsertChild(*pAmorce, TTreeNode::First) ;
          }
        }
        //
        // Création du NSTreeNode à partir du TTreeNode
        //
        pNewNSTreeNodeFils = new NSTreeNode(*pAmorce, pContexte) ;
      }
      else
      {
      	pNewNSTreeNodePere = TrouverPere(LigneFils, ColonneFils ) ;
        pNewNSTreeNodeFils = new NSTreeNode(*this, pContexte, "PRIVATE") ;
      }
      //
      // Référencement du père et de lui même dans l'array de fils
      // de son père
      //
      pNewNSTreeNodeFils->SetFatherNode(pNewNSTreeNodePere) ;
      if (pNewNSTreeNodePere)
        pNewNSTreeNodePere->getVectSons()->Referencer(pNewNSTreeNodeFils) ;
      //
      // Prise en compte du complément
      //
      // Texte libre
      //
      if ((string("£?????") == sIdentiteFils) ||
          (string("£C;") == sSensFils)) //texte libre
      {
      	sCodeTexteLibre = (*iterTReeView)->getComplement() ;
        pNewNSTreeNodeFils->SetComplement(sCodeTexteLibre) ;
        // Gestion de la NSCutPastTLArray
        if ((string("£?????") == sIdentiteFils) && pTLArray)
        {
        	if ((false == pTLArray->empty()) && (string("") != sCodeTexteLibre))
          {
          	CutPastTLIter i ;
            for (i = pTLArray->begin();
                                (pTLArray->end() != i) &&
                                    (sCodeTexteLibre != (*i)->sIndice); i++) ;
            if (pTLArray->end() != i)
            {
              sCodeTexteLibre = "" ;
              sTexteLibre     = (*i)->sTexte ;
            }
          }
        }
        else if (string("£C;") == sSensFils)
          sTexteLibre = (*iterTReeView)->getTexteLibre() ;
      }
      //
      // Pas de texte libre
      //
      else
      {
        sCodeTexteLibre = "" ;
        pNewNSTreeNodeFils->SetComplement((*iterTReeView)->getComplement()) ;
        pNewNSTreeNodeFils->InstancieUrl(&Encyclo) ;//Encyclopédie.db
        //
        // On regarde la catégorie du père
        //
        if (pNewNSTreeNodeFils->getFatherNode())
        {
          string sFatherCategory = NSDico::CodeCategorie(pNewNSTreeNodeFils->getFatherNode()->getEtiquette()) ;

          //
          //date et heure, affiche la date sous forme JJ/MM/AAAA et
          //l'heure sous forme HH:MM
          //
          if ("K" == sFatherCategory)
				  {
        	  string _sUnite = (*iterTReeView)->getUnit() ;
            if ((_sUnite == "2DA011") || (_sUnite == "2HE011"))
          	  pNewNSTreeNodeFils->SetDateTime(true) ;
          }
          // valeur chiffrée
          else if ("V" == sFatherCategory)
          {
        	  //si "CALCUL DYNAMIQUE" recalculer sComplement à partir de convert.db
            if ((sIdentiteFils.find("£ND")  != NPOS) ||
              (sIdentiteFils.find("/£ND") != NPOS))
            {
          	  pNewNSTreeNodeFils->SetRecalcul(true) ;
              string sUniteConvert  = string("") ;
              string sMethodeCalcul = string("") ;
              if (Converter.isOpened())
            	  Converter.CalculValeur(pNewNSTreeNodeFils->getFatherNode()->getEtiquette(), &sComplement,
			                         		&sUniteConvert, &sMethodeCalcul, ContenuPatpatho) ;
              if (string("") != sMethodeCalcul)
            	  pNewNSTreeNodeFils->getFatherNode()->SetComplement(sMethodeCalcul) ;
            }
            pNewNSTreeNodeFils->SetCreateurChampEdit(true) ; //champ édit : poids , taille,...
          }
        }
      }
      //
      // Initialisation des autres variables
      //
      pNewNSTreeNodeFils->SetNoeud(sNoeud) ;
      pNewNSTreeNodeFils->SetUnit((*iterTReeView)->getUnit()) ;
      pNewNSTreeNodeFils->SetInteret((*iterTReeView)->getInteret()) ;
      pNewNSTreeNodeFils->SetPluriel((*iterTReeView)->getPluriel()) ;
      pNewNSTreeNodeFils->SetCertitude((*iterTReeView)->getCertitude()) ;
      pNewNSTreeNodeFils->SetVisible((*iterTReeView)->getVisible()) ;
      pNewNSTreeNodeFils->SetRights((*iterTReeView)->getNodeRight()) ;

      pNewNSTreeNodeFils->SetGlobalText((*iterTReeView)->getTexteLibre()) ;

      //
      // Mise en place du noeud
      //
      sTexteLibre = pNewNSTreeNodeFils->getGlobalText() ;
      bool bTexte = pNewNSTreeNodeFils->MettreAjourLabel(/*pNSTlibre*/ 0,
              &texteRecupere, &NbTexteLibre, LigneFils, ColonneFils,
                   sTexteLibre, sCodeTexteLibre, sIdentiteFils, this, sLang) ;
      if (false == bTexte)
      {
        string sErrMsg = pContexte->getSuperviseur()->getText("treeViewErrors", "errorCreatingANewNode") ;
        pContexte->getSuperviseur()->trace(&sErrMsg, 1, NSSuper::trError) ;
        erreur(sErrMsg.c_str(), standardError, 0) ;
        return ;
      }

      if (sExpand == "EXPAND")
      	pNewNSTreeNodePere->ExpandTousNoeud() ;

      delete pAmorce ;
    }

    pLibelleTrouve  = string("") ;
    sLabel          = string("") ;
    sCodeTexteLibre = string("") ;
    sTexteLibre     = string("") ;
    sIdentitePere   = string("") ; //étiquette du NStreeNode père
    sIdentiteFils   = string("") ; //étiquette du NStreeNode fils

    if (pDecalageLigneFils)
      (*pDecalageLigneFils)++ ;
    // Elément suivant
    if (ContenuPatpatho->end() != iterTReeView)
      iterTReeView++ ;
  }
  bDispatcherPatpatho = false ; //on dispatche une seule fois la patpatho globale

  //
  // Pour chaque noeud , lui mettre à jour son affichage en tenant compte
  // de son champ visible
  //
  if (pNSTreeNodeLanceur)
  	pNSTreeNodeLanceur->InitialiseAffichage() ;
	else
  	AfficheLibelle() ;
}
catch (...)
{
	erreur("Exception (NSTreeWindow::DispatcherPatpatho).", standardError, 0) ;
}
}

// Set node Id of TTreeNodes (sNoeud) according to pContentPatpatho
//
// We assume that :
//   - nodes are in the same order in both patpatho and treeview
//   - all treeview nodes are in patpatho but some patpatho nodes can miss in treeview
//
void
NSTreeWindow::SetNodesIdFromPatho(NSPatPathoArray* pContentPatpatho)
{
  if (((NSPatPathoArray*) NULL == pContentPatpatho) || (pContentPatpatho->empty()))
    return ;

  if (_NodeArray.empty())
    return ;

  iterNSTreeNode nodeIter = _NodeArray.begin() ;

  // Find root path for treeview nodes
  //
/*
  string sRootCode = (*nodeIter)->getEtiquette() ;
  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&sRootCode, &sCodeSens) ;

  string sRootLoc  = (*nodeIter)->getPosition() ;
  string sRootPath = string("") ;
  size_t iCodePos  = sRootLoc.find(sCodeSens) ;
  if (NPOS != iCodePos)
    sRootPath = string(sRootLoc, 0, iCodePos) ;
*/

  PatPathoIter pptIter = pContentPatpatho->begin() ;
  UpdateCommandInformation(*pptIter) ;

  // Root path is just patpatho's root
  //
  string sRootPath = (*pptIter)->getLexiqueSens() + string(1, cheminSeparationMARK) ;

  // Iterating through patpatho
  //
  for ( ; pContentPatpatho->end() != pptIter ; pptIter++)
  {
    string sPptNode = (*pptIter)->getNodeID() ;

    // Getting patpatho item's path
    //
    string sPptPath = pContentPatpatho->donneCheminItem(pptIter) ;
    string sPptPathSens ;
    pContexte->getDico()->donneCodeSens(&sPptPath, &sPptPathSens) ;

    // Creating the path for corresponding treeview node
    //
/*
    string sPseudoPath = sRootPath ;
    if (string("") != sPptPathSens)
      sPseudoPath += sPptPathSens + string(1, cheminSeparationMARK) ;
    sPseudoPath += (*pptIter)->getLexiqueSens(pContexte->getSuperviseur()) ;
*/
    string sPseudoPath = sRootPath + sPptPathSens ;

    iterNSTreeNode previousNodeIter = nodeIter ;

    while (_NodeArray.end() != nodeIter)
    {
      string sNodePath ;
      (*nodeIter)->formerChemin(&sNodePath) ;

      if ((false == (*nodeIter)->estFictif()) &&
          ((*pptIter)->getLexique() == (*nodeIter)->getEtiquette()) &&
          (sNodePath == sPseudoPath))
        break ;

      nodeIter++ ;
    }

    // Node found set it's node Id and go to next one
    //
    if (_NodeArray.end() != nodeIter)
    {
      UpdateCommandInformation(*pptIter, *nodeIter) ;
      nodeIter++ ;
    }
    //
    // Node not found, go back to previous node
    //
    else
      nodeIter = previousNodeIter ;
  }
}

//---------------------------------------------------------------------------//  Function: 		NSTreeWindow::activeControle(int activation, Message* pMessage)
//
//  Reconstruction de la TreeView
//
//	 Arguments:		activation : BF_CHECKED , BF_UNCHECKED
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSTreeWindow::activeControle(int /*activation*/, Message* /*pMessage*/)
{
	if (false == bDispatcherPatpatho)
  	return ;

  // patpatho du BBFilsItem qui a lancé la treeview
  if ((_pControle) && (_pControle->getTransfert()) && (_pControle->getTransfert()->getFilsItem()))
  {
  	NSVectFatheredPatPathoArray* pPatpatho = _pControle->getTransfert()->getFilsItem()->getPatPatho() ;
    if (false == pPatpatho->empty())
    {
      NSFatheredPatPathoArray* pFatheredElement = *(pPatpatho->begin()) ;
    	NSPatPathoArray* ContenuPatpatho = pFatheredElement->getPatPatho() ;
      // Dispatcher la patpatho
      if (false == ContenuPatpatho->empty())
      	DispatcherPatPatho(ContenuPatpatho, 0, 0, "") ;
      else
      // Make certain that the patpatho will not be dispatched by another call to this function
        bDispatcherPatpatho = false ;
    }
  }

/*
  if (pNodeArray && _NodeArray.empty())
  {
  	SetFocus() ;
    Invalidate() ;
  }
*/
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//	retourner un NSTReeNode ayant le même hitem que le TTreeNode sélectionné
//----------------------------------------------------------------------
//----------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::GetNSTreeNode(TTreeNode* pTTreeNode)
{
  if (((TTreeNode*) NULL == pTTreeNode) || (_NodeArray.empty()))
    return (NSTreeNode*) 0 ;

  HTREEITEM TTitem, NSitem ;

  for (iterNSTreeNode i = _NodeArray.begin() ; _NodeArray.end() != i ; i++)
  {
    TTitem = pTTreeNode->operator HTREEITEM() ;
    NSitem = (*i)->operator HTREEITEM() ;
    if (TTitem == NSitem)
      return (*i) ;
  }
  
  return (NSTreeNode*) 0 ;
}

//----------------------------------------------------------------------//----------------------------------------------------------------------
//	retourner un NSTReeNode ayant le même hitem que le TTreeNode sélectionné
//----------------------------------------------------------------------
//----------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::GetNSTreeNode(HTREEITEM TTitem)
{
  if (_NodeArray.empty())
    return (NSTreeNode*) 0 ;

  for (iterNSTreeNode i = _NodeArray.begin() ; _NodeArray.end() != i ; i++)
  {
    HTREEITEM NSitem = (*i)->operator HTREEITEM() ;
    if (TTitem == NSitem)
      return (*i) ;
  }
  return (NSTreeNode*) 0 ;
}

// Get NSTreeNode for a given node Id
//
NSTreeNode*
NSTreeWindow::GetNSTreeNode(string sNodeID)
{
  if (_NodeArray.empty())
    return (NSTreeNode*) 0 ;

  for (iterNSTreeNode i = _NodeArray.begin() ; _NodeArray.end() != i ; i++)
    if ((*i)->getNodeId() == sNodeID)
      return (*i) ;

  return (NSTreeNode*) 0 ;
}

//------------------------------------------------------------------------//mise à jour d'un NSTreeNode
//------------------------------------------------------------------------
void
NSTreeWindow::MiseAjourNSTreeNode(NSTreeNode* pNSTreeNode)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) ||
      (string("") == pNSTreeNode->getEtiquette()))
    return ;

try
{
  NSControle *pCtrl = new NSControle(pContexte, pBBitemNSTreeWindow , pNSTreeNode->getEtiquette(), "") ;

  pNSTreeNode->SetControl(pCtrl) ;
  pNSTreeNode->getControl()->setControle(dynamic_cast<void*>(pNSTreeNode)) ;
  pNSTreeNode->getControl()->setType(pNSTreeNode->donneType()) ;
  if (pBBitemNSTreeWindow)
  {
    pNSTreeNode->getControl()->setNSDialog(pBBitemNSTreeWindow->getDialog()) ;
    pNSTreeNode->getControl()->setMUEView(pBBitemNSTreeWindow->getView()) ;
  }
  pNSTreeNode->pContexte = pContexte ;

  // int colonne = pNSTreeNode->getColonne();
  int ligne = pNSTreeNode->getLigne() ;

  //retrouver pNSTreeNode dans NodeArray
  //
  if (_NodeArray.empty())
    return ;

  iterNSTreeNode EnCours = _NodeArray.begin() ; // pNSTreeNode
  for ( ; (_NodeArray.end() != EnCours) && ((*EnCours) != pNSTreeNode); EnCours++) ;

  // Not found; leaving
  if (_NodeArray.end() == EnCours)
    return ;

  NSTreeNode* pNSTreePere = pNSTreeNode->getFatherNode() ;
  if (pNSTreePere)
  {
    NSTreeNode* pNSTreePetitFrere = TrouverPetitFrere(pNSTreeNode, ligne) ;

    if (pNSTreePetitFrere)
      CreerBBItem(pNSTreeNode, pNSTreePetitFrere) ;
    else
      CreerBBItem(pNSTreeNode, 0) ;
  }
  else
  {
    //chercher le 1er NSTreeNode de colonne = 1 et après pNSTreeNode
    NSTreeNode* pNSTreePetitFrere = TrouverPetitFrereFilsRoot(pNSTreeNode, ligne) ;
    if (pNSTreePetitFrere)
      CreerBBItem(pNSTreeNode, pNSTreePetitFrere) ;
    else
      CreerBBItem(pNSTreeNode, (NSTreeNode*) 0) ;
  }

  //
  // Il est important, après connexion au BBitem, de mettre à jour
  // la structure de message qui lui est attachée, sinon on risque
  // de perdre des données lors d'un transfert Item -> Noeud
  //
  if (pNSTreeNode->getTransfertMessage())
  {
    CTRL_ACTIVITY iActif ;
    pNSTreeNode->Transferer(tdGetData, &iActif, pNSTreeNode->getTransfertMessage()) ;
  }
}
catch (...)
{
	erreur("Exception NSTreeWindow::MiseAjourNSTreeNode.", standardError, 0) ;
}
}

//------------------------------------------------------------------------
//remise à jour d'un NSTreeNode avec le nouveau code pStringCode
//------------------------------------------------------------------------
void
NSTreeWindow::ReMiseAjourNSTreeNode(NSTreeNode* pNSTreeNode, string sStringCode, string sComplement)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) || (NULL == pNSTreeNode->getControl()))
    return ;
  //MAJ
  pNSTreeNode->getControl()->setIdentite(sStringCode) ;
  if (pNSTreeNode->getBBFilsItem())
    pNSTreeNode->getBBFilsItem()->setItemLabel(sStringCode) ;
  if (pNSTreeNode->getTransfertMessage())
    pNSTreeNode->getTransfertMessage()->SetComplement(sComplement) ;
}

//---------------------------------------------------------------------------
//	création d'un BBItem rattaché au NSTreeNode
//---------------------------------------------------------------------------
void
NSTreeWindow::CreerBBItem(NSTreeNode* pNSTreeNode, NSTreeNode* pNSTreeNodePetitFrere)
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	BBFilsItem* pFilsItemPere = (BBFilsItem*) 0 ; //le BBFilsItem rattaché au père de pNSTreeNode

	if ((NSTreeNode*) NULL == pNSTreeNode->getFatherNode())	{  	if (((NSControle*) NULL == _pControle) || (NULL == _pControle->getTransfert()))    	return ;		pFilsItemPere = _pControle->getTransfert()->getFilsItem() ;
    if ((BBFilsItem*) NULL == pFilsItemPere)
    	return ;

    // Colonne 0 :  Avant de créer un nouveau fils, il faut s'assurer qu'il
    //              n'existe pas un fils de même étiquette
    // Column 0 :   Before the creation of a new son, let's check if it
    //              doesn't already have a son with the same label
    //
    if (false == pFilsItemPere->VectorFils.empty())
    {
    	BBItem*     pPremItem = *(pFilsItemPere->VectorFils.begin()) ;
      BBFilsItem* pBonFils  = pPremItem->TrouverFilsAyantBonneEtiquette(pNSTreeNode->getEtiquette(), "") ;
      if (pBonFils && (0 == pBonFils->getItemTransfertData()->getControl()))
      {
      	// Si on est ici, c'est que le mécanisme de création BBItem puis
        // BBFilsItem a déjà été fait : il ne reste qu'à connecter
        // le contrôle au BBFilsItem
        // ne surtout pas faire pBonFils->CreerFilsManuel

        pBonFils->getItemTransfertData()->setControl(pNSTreeNode->getControl()) ;
        pNSTreeNode->getControl()->setTransfert(pBonFils->getItemTransfertData()) ;
        pNSTreeNode->activateFilsItem() ;

        return ;
      }
      else
      {
      	if (pNSTreeNodePetitFrere)
        {
        	pPremItem->CreerFilsManuel(pNSTreeNode->getControl(), pNSTreeNodePetitFrere->getControl()) ;
          pNSTreeNode->activateFilsItem() ;
          return ;
        }
        else
        {
        	pPremItem->CreerFilsManuel(pNSTreeNode->getControl(), 0) ;
          pNSTreeNode->activateFilsItem() ;
          return ;
        }
      }
    }
  }
  else
    pFilsItemPere = pNSTreeNode->getFatherNode()->getBBFilsItem() ;

  if ((BBFilsItem*) NULL == pFilsItemPere)
  {
    string sErr = string("Trying to create a son for a virtual node.") ;
    pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
    erreur(sErr.c_str(), standardError, 0) ;
    return ;
  }

	if (pNSTreeNodePetitFrere)
  	pFilsItemPere->CreerFilsManuel(pNSTreeNode->getControl(), pNSTreeNodePetitFrere->getControl()) ;
	else
		pFilsItemPere->CreerFilsManuel(pNSTreeNode->getControl(), 0) ;

  pNSTreeNode->activateFilsItem() ;
}

//-------------------------------------------------------------------------// 				Absence d'élément lexique
//-------------------------------------------------------------------------
void
NSTreeWindow::Absence()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;
    
  SetDirty(true) ;
  pNSTreeNode->AbsenceLexique() ;
}

//-------------------------------------------------------------------------
// 				Présence d'élément lexique
//-------------------------------------------------------------------------
void
NSTreeWindow::Presence()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  SetDirty(true) ;
  pNSTreeNode->PresenceLexique() ;
}

//-------------------------------------------------------------------------// 		   Edition du noeud (pères + fils) sous forme de fil guide
//-------------------------------------------------------------------------
void
NSTreeWindow::EditionFilGuide()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;
}

//-------------------------------------------------------------------------
// 				Présence de l'élément lexique dans Encyclop.db
//-------------------------------------------------------------------------
void
NSTreeWindow::Encyclop()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("NavigateEncyclopedia")) ;
  pTask->setPointer1((void*) pNSTreeNode, false) ;

	pTask->sendMe(pSuper) ;
}

void
NSTreeWindow::Certificat()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("NavigateCertificat")) ;
  pTask->setPointer1((void*) pNSTreeNode, false) ;

	pTask->sendMe(pSuper) ;
}

//-------------------------------------------------------------------------// 							Paramètres patpatho pour un noeud
//				les textes libres seront seulement concernés par "interêt"
//-------------------------------------------------------------------------
void
NSTreeWindow::Parametres()
{
	TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;
    
  pNSTreeNode->RecupereParametre() ;
}

//-------------------------------------------------------------------------
// 					   Création d'une nouvelle préoccupation
//-------------------------------------------------------------------------
void
NSTreeWindow::NewPreoccup()
{
	TTreeNode noeud = GetSelection() ;
	NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	// Vraie préoccupation ou traitement ?

	string sEtiquette = pNSTreeNode->getEtiquette() ;
	if (string("") == sEtiquette)
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
  NSToDoTask* pTask = new NSToDoTask ;

	if (('I' == sEtiquette[0]) || ('_' == sEtiquette[0]))
	{
		pTask->setWhatToDo(string("NewDrugFromNode")) ;
		pTask->setPointer1((void*) this, false) ;
		pTask->setPointer2((void*) pNSTreeNode, false) ;
	}
	else
	{
    string sContinuityOfCareCode = string("") ;

    // Look for a ICPC code to pass as continuity of care code
    //
    if (false == pNSTreeNode->getVectSons()->empty())
    {
      iterNSTreeNode iterFils = pNSTreeNode->getVectSons()->begin() ;
      for ( ; (pNSTreeNode->getVectSons()->end() != iterFils) && (string("6CISP") != (*iterFils)->getLabelSens()); iterFils++) ;
	    if (pNSTreeNode->getVectSons()->end() != iterFils)
        sContinuityOfCareCode = (*iterFils)->getContenuTransfert() ;
    }

		pTask->setWhatToDo(string("NewPreoccupFromNode")) ;
		pTask->setPointer1((void*) this, false) ;
		pTask->setPointer2((void*) pNSTreeNode, false) ;
    pTask->setParam1(sContinuityOfCareCode) ;
	}

	pTask->sendMe(pSuper) ;
}

void
NSTreeWindow::ChangePreoccup(NSConcern* pConcern)
{
	TTreeNode noeud = GetSelection() ;
	NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
	NSToDoTask* pTask = new NSToDoTask ;
	pTask->setWhatToDo("ChangePreoccupFromNode") ;
	pTask->setPointer1((void*) this, false) ;
	pTask->setPointer2((void*) pNSTreeNode, false) ;
	pTask->setParam1(string(pConcern->getNoeud())) ;

	pTask->sendMe(pSuper) ;
}

/**
* 		   Création d'une nouvelle préoccupation avec ses données
*/
void
NSTreeWindow::NewPreoccupRC(NSPatPathoArray* pContenuPatpatho, string sArchetype)
{
	if (((NSPatPathoArray*) NULL == pContenuPatpatho) || (pContenuPatpatho->empty()))
		return ;

	string sRootLex = (*(pContenuPatpatho->begin()))->getLexique() ;

	NSCutPaste CutPaste(*(pContexte->getSuperviseur()->pBufCopie)) ;
	pContexte->getSuperviseur()->pBufCopie->vider() ;
	pContexte->getSuperviseur()->pBufCopie->setPatPatho(pContenuPatpatho) ;

	//
	// Recherche du noeud "Assesment" pour y ajouter l'arbre
	//
	TTreeNode nextNode = GetFirstVisible() ;
	while (nextNode != NULL)
	{
		NSTreeNode* pNode = GetNSTreeNode(&nextNode) ;

		if (pNode && (pNode->getEtiquette() != ""))
		{
			string sEtiqu = pNode->getEtiquette() ;
			string sCodeSens ;
			pContexte->getDico()->donneCodeSens(&sEtiqu, &sCodeSens) ;
			if (sCodeSens == "0SOA4")
			{
				// Ajout de l'arbre comme fils du noeud assesment
				//
				CopyBranch(pNode, "COPIE") ;
				nextNode = nextNode.GetNextItem(TVGN_CHILD) ;
				while (nextNode != NULL)
				{
					NSTreeNode* pFilsNode = GetNSTreeNode(&nextNode) ;
					if (pFilsNode && (pFilsNode->getEtiquette() == sRootLex))
					{
						nextNode.SelectItem(TVGN_CARET) ;

						if (sArchetype != "")
						{
							string sArchetypeNode = pContexte->getSuperviseur()->getArcManager()->DonneNoeudArchetype(NSArcManager::archetype, sArchetype) ;
							if (sArchetypeNode != "")
								pFilsNode->addTemporaryLink(sArchetypeNode, NSRootLink::archetype, dirFleche) ;
						}

						NewPreoccup() ;
						break ;
					}
					nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
				}
				break ;
			}
		}
		nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
	}
	*(pContexte->getSuperviseur()->pBufCopie) = CutPaste ;
}

//
// Nouvelle description sans changement de nature
//
void
NSTreeWindow::EvolPreoccupRC(NSPatPathoArray* pContenuPatpatho, NSConcern* pConcern, string sArchetype)
{
	if (((NSPatPathoArray*) NULL == pContenuPatpatho) ||
            (pContenuPatpatho->empty()) || ((NSConcern*) NULL == pConcern))
		return ;

	string sRootLex = (*(pContenuPatpatho->begin()))->getLexique() ;

	NSCutPaste CutPaste(*(pContexte->getSuperviseur()->pBufCopie)) ;
  pContexte->getSuperviseur()->pBufCopie->vider() ;
  pContexte->getSuperviseur()->pBufCopie->setPatPatho(pContenuPatpatho) ;

  TTreeNode nextNode = GetFirstVisible() ;
  while (NULL != nextNode)
  {
  	NSTreeNode* pNode = GetNSTreeNode(&nextNode) ;

    if (pNode && (pNode->getEtiquette() != string("")))
    {
    	string sEtiqu = pNode->getEtiquette() ;
      string sCodeSens ;
      pContexte->getDico()->donneCodeSens(&sEtiqu, &sCodeSens) ;
      if (string("0SOA4") == sCodeSens)
      {
      	CopyBranch(pNode, "COPIE") ;
        nextNode = nextNode.GetNextItem(TVGN_CHILD) ;
        while (nextNode != NULL)
        {
        	NSTreeNode* pFilsNode = GetNSTreeNode(&nextNode) ;
          if (pFilsNode && (pFilsNode->getEtiquette() == sRootLex))
          {
          	nextNode.SelectItem(TVGN_CARET) ;
            pFilsNode->addTemporaryLink(pConcern->getNoeud(), NSRootLink::problemContactElement, dirFleche) ;
            break ;
          }
          nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
        }
        break ;
      }
    }
    nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
	}
	*(pContexte->getSuperviseur()->pBufCopie) = CutPaste ;
}

//
// Nouvelle description avec changement de nature
//
void
NSTreeWindow::ChangePreoccupRC(NSPatPathoArray* pContenuPatpatho, NSConcern* pConcern, string sArchetype)
{
    if ((!pContenuPatpatho) || (pContenuPatpatho->empty()) || (!pConcern))
        return ;
    string sRootLex = (*(pContenuPatpatho->begin()))->getLexique() ;

    NSCutPaste CutPaste(*(pContexte->getSuperviseur()->pBufCopie)) ;
    pContexte->getSuperviseur()->pBufCopie->vider() ;
    pContexte->getSuperviseur()->pBufCopie->setPatPatho(pContenuPatpatho) ;

    TTreeNode nextNode = GetFirstVisible() ;
    while (nextNode != NULL)
    {
        NSTreeNode* pNode = GetNSTreeNode(&nextNode) ;

        if (pNode && (pNode->getEtiquette() != ""))
        {
            string sEtiqu = pNode->getEtiquette() ;
            string sCodeSens ;
            pContexte->getDico()->donneCodeSens(&sEtiqu, &sCodeSens) ;
            if (sCodeSens == "0SOA4")
            {
                CopyBranch(pNode, "COPIE") ;
                nextNode = nextNode.GetNextItem(TVGN_CHILD) ;
                while (nextNode != NULL)
                {
                    NSTreeNode* pFilsNode = GetNSTreeNode(&nextNode) ;
                    if (pFilsNode && (pFilsNode->getEtiquette() == sRootLex))
                    {
                        nextNode.SelectItem(TVGN_CARET) ;
                        ChangePreoccup(pConcern) ;
                        break ;
                    }
                    nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
                }
                break ;
            }
        }
        nextNode = nextNode.GetNextItem(TVGN_NEXT) ;
    }
    *(pContexte->getSuperviseur()->pBufCopie) = CutPaste ;
}

//-------------------------------------------------------------------------
// 					 Connection à une préoccupation existante
//-------------------------------------------------------------------------
void
NSTreeWindow::ConnectToPreoccup()
{
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;
}

//-------------------------------------------------------------------------
// 							Classification
//-------------------------------------------------------------------------
void
NSTreeWindow::Classifier()
{
try
{
	TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  BBFilsItem* pFilsItem = pNSTreeNode->getBBFilsItem() ;
  if ((BBFilsItem*) NULL == pFilsItem)
    return ;

  NSEpisodus* pEpisodus = pContexte->getEpisodus() ;

  //
  // Trouver automatiquement la classification à utiliser
  //
  ClassificationPrinciple* pPrincipe = (ClassificationPrinciple*) 0 ;
  string                   sPostCase = string("") ;

  string sChemin = string("") ;

  PrinciplesArray* pPrincipes = pEpisodus->pPrincipes ;
  if ((PrinciplesArray*) NULL == pPrincipes)
  	return ;

  pNSTreeNode->formerChemin(&sChemin) ;
  pPrincipe = pPrincipes->trouvePrincipe(sChemin, sPostCase) ;

  //
  // Ou demander
  //
  if ((ClassificationPrinciple*) NULL == pPrincipe)
  	return ;

  //
  // Puis trouver le code
  //
  string sClassif = pPrincipe->sClassification ;

  string sConcept ;
  string sNodeLabel = pNSTreeNode->getEtiquette() ;
  pContexte->getDico()->donneCodeSens(&sNodeLabel, &sConcept) ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

  string sDomain = string("") ;

  if (string("£??") == sConcept)
  {
  	string sLibelle  = pNSTreeNode->getGlobalText() ;
    string sLocalize = pNSTreeNode->getPosition() ;

    ParseSOAP Parser(pContexte, sClassif, string("")) ;
    Parser.computeParsing(&sLibelle, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

    sDomain = sResP ;
  }
  else
  {
  	Classify ClassifTool(pContexte, &sClassif, &sConcept, &sPostCase) ;
  	ClassifTool.computeParsing(&sResO, &sResP, &sResI) ;

    // If Classer has found 'Mandatory code(s)', then we must take it
    //
    if (string("") != sResO)
      sDomain = sResO ;

    // Else, 3BT is the gold standard, so, we have to use it
    //
    else
    {
      string sClasserDomain = sResP ;

      //
      //
      string sLibelle  = pNSTreeNode->getGlobalText() ;
      string sLocalize = pNSTreeNode->getPosition() ;

      ParseSOAP Parser(pContexte, sClassif, string("")) ;
      Parser.computeParsing(&sLibelle, &sLocalize, &sResO, &sResP, &sResI, &sRes3) ;

      if (string("") != sResP)
        sDomain  = sResP ;
      else
        sDomain  = sClasserDomain ;
    }
  }

  //
  // On trouve le code
  //
  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if ((classifExpert*) NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  ElemSetArray* pElemDomain = 0 ;
  //
  // On instancie le domaine
  // Instanciating the domain
  //

  ParseCategory Parser(pExpert->donneCodeSize(sClassif), sClassif,
                                             pExpert->donnePattern(sClassif)) ;
  pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&(pPrincipe->sCase), &sCaseSens) ;

  pExpert->fillList(sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

  string sGoodCode = "" ;
  //
  //
  //
  if (arrayClassif.size() != 1)
  {
  	SOAPObject SOAPObj(pNSTreeNode->getLabelString(), sResP, sClassif, 0, pFilsItem) ;
#ifndef _EXT_CAPTURE
		NSCapture Capture(pContexte) ;
#else
		NSCapture Capture ;
#endif

		Capture.setClassifResultO(sResO) ;
    Capture.setClassifResultP(sResP) ;
    Capture.setClassifResultI(sResI) ;
    Capture.setClassifResult3(sRes3) ;

    SOAPObj.pCaptElemnt = &Capture ;
    SOAPObj.sCase       = sCaseSens ;

    sGoodCode = pExpert->chooseCode(&SOAPObj) ;
  }
  else
  {
  	sGoodCode = (*(arrayClassif.begin()))->getCode() ;

    string sCtrlData = "" ;
    NVLdVTemps tpsDebut ;
    tpsDebut.takeTime() ;
    pExpert->setControlString(&sCtrlData, sClassif, sCaseSens, pNSTreeNode->getLabelString(),
                                  classifExpert::niveauPreselection, 0,
                                  &tpsDebut, &tpsDebut, sGoodCode, sConcept) ;
    pExpert->storeControlData(sCtrlData) ;
  }

  if (sGoodCode == "")
  	return ;

  //
  // Ajouter à l'arbre
  //
  string sClassifPlein = sClassif + string(1, '1') ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  pSuper->pBufDragDrop->vider() ;
  NSPatPathoArray DragPatho(pSuper) ;

  NSPatPathoInfo pptInfo ;
  pptInfo.setLexique(sClassifPlein) ;
  pptInfo.setComplement(sGoodCode) ;

  DragPatho.push_back(new NSPatPathoInfo(pptInfo)) ;

  pSuper->pBufDragDrop->setPatPatho(&DragPatho) ;

  DragFils(pNSTreeNode, "DRAG") ;
}
catch(...)
{
	erreur("Exception NSTreeWindow::Classifier.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------//	   Traitement de la commande clavier de changement d'"interêt"
//-------------------------------------------------------------------------
void
NSTreeWindow::FixeInteret(char KbKey)
{
	TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;

  if (false == isRealNode(pNSTreeNodeSelection))
    return ;

  Message* pMessage = pNSTreeNodeSelection->getTransfertMessage() ;
  if ((Message*) NULL == pMessage)
    return ;

  if      (('4' == KbKey) || (VK_NUMPAD4 == KbKey)) //max
    pMessage->SetInteret(string("E")) ;
  else if (('3' == KbKey) || (VK_NUMPAD3 == KbKey))
    pMessage->SetInteret(string("D")) ;
  else if (('2' == KbKey) || (VK_NUMPAD2 == KbKey))
    pMessage->SetInteret(string("C")) ;
  else if (('1' == KbKey) || (VK_NUMPAD1 == KbKey))
    pMessage->SetInteret(string("B")) ;
  else if (('0' == KbKey) || (VK_NUMPAD0 == KbKey))
    pMessage->SetInteret(string("A")) ;

  string sNewInteret = pMessage->GetInteret() ;
  pNSTreeNodeSelection->fixeInteret(sNewInteret) ;
}

//-------------------------------------------------------------------------
//	   			Traitement du changement de certitude
//-------------------------------------------------------------------------
void
NSTreeWindow::FixeCertitude(int iCert)
{
	TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;

  if (false == isRealNode(pNSTreeNodeSelection))
    return ;

  Message* pMessage = pNSTreeNodeSelection->getTransfertMessage() ;
  if ((Message*) NULL == pMessage)
    return ;
    
  if 		((iCert >= 0) && (iCert <= 12))
    pMessage->SetCertitude(string("WCE001")) ;
  else if ((iCert > 12) && (iCert <= 37))
    pMessage->SetCertitude(string("WCE251")) ;
  else if ((iCert > 37) && (iCert <= 62))
    pMessage->SetCertitude(string("WCE501")) ;
  else if ((iCert > 62) && (iCert <= 87))
    pMessage->SetCertitude(string("WCE751")) ;
  else
    pMessage->SetCertitude(string("")) ;

  string sNewCertitu = pMessage->GetCertitude() ;
  pNSTreeNodeSelection->fixeCertitude(sNewCertitu) ;
}

void
NSTreeWindow::FixeDroits()
{
  if (ReadOnly)
    return ;

  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;

  if (false == isRealNode(pNSTreeNodeSelection))
    return ;

try
{
	string sNodeRights = pNSTreeNodeSelection->getRights() ;

	RightsDialog* pRightsDlg = new RightsDialog(this, pContexte, &sNodeRights) ;
  int iReturn = pRightsDlg->Execute() ;

  if (IDOK == iReturn)
  {
    pNSTreeNodeSelection->SetRights(sNodeRights) ;

    // mise à jour du bbitem
    if (pNSTreeNodeSelection->getTransfertMessage())
    {
      CTRL_ACTIVITY iActif ;
      pNSTreeNodeSelection->Transferer(tdGetData, &iActif,
                                 pNSTreeNodeSelection->getTransfertMessage()) ;
    }

    SetDirty(true) ;
  }

  delete pRightsDlg ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTreeWindow::FixeDroits : ") + e.why() ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
	erreur(sErr.c_str(), standardError, 0) ;
	return ;
}
catch (...){
	erreur("Exception NSTreeWindow::FixeDroits.", standardError, 0) ;
}
}

//------------------------------------------------------------------------
//trouver le plus proche petit frère d'un NSTreeNode
//------------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::TrouverPetitFrere(NSTreeNode* pNSTreeNode, int ligne)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) ||
                     ((NSTreeNode*) NULL == pNSTreeNode->getFatherNode()))
    return (NSTreeNode*) 0 ;

  if (true == pNSTreeNode->getFatherNode()->getVectSons()->empty())
    return (NSTreeNode*) 0 ;

	int lignEnCours = 1000000 ;
  NSTreeNode* pNSTreePetitFrere = (NSTreeNode*) 0 ;

  // Chercher le frère cadet le plus proche de pNSTreeNode
  //
  iterNSTreeNode fils = pNSTreeNode->getFatherNode()->getVectSons()->begin() ;
  for (; pNSTreeNode->getFatherNode()->getVectSons()->end() != fils ; fils++)
  {
    if ((false == (*fils)->estLie()) &&
        ((*fils)->getLigne() < lignEnCours) && ((*fils)->getLigne() > ligne))
    {
      lignEnCours = (*fils)->getLigne() ;
      pNSTreePetitFrere = *fils ;
    }
  }
  
  return (pNSTreePetitFrere) ;
}

//------------------------------------------------------------------------
//trouver le plus proche grand frère d'un NSTreeNode
//------------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::TrouverGrandFrere(NSTreeNode* pNSTreeNode, int ligne)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) ||
                ((NSTreeNode*) NULL == pNSTreeNode->getFatherNode()))
    return 0 ;

  if (true == pNSTreeNode->getFatherNode()->getVectSons()->empty())
    return 0 ;

	int lignEnCours = 0 ;
	NSTreeNode* pNSTreeGrandFrere = 0 ;

  // Chercher le frère ainé le plus proche de pNSTreeNodeSelection
  //
  iterNSTreeNode fils = pNSTreeNode->getFatherNode()->getVectSons()->begin() ;
  for (; pNSTreeNode->getFatherNode()->getVectSons()->end() != fils ; fils++)
  {
    if ((false == (*fils)->estLie()) &&
        ((*fils)->getLigne() > lignEnCours) && ((*fils)->getLigne() < ligne))
    {
      lignEnCours = (*fils)->getLigne() ;
      pNSTreeGrandFrere = *fils ;    }
  }
  
  return (pNSTreeGrandFrere) ;
}

//------------------------------------------------------------------------// Touver le TTReenode dont le père est le root et qui soit le plus proche de
// l'élément sélectionné (ainé)
//------------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::TrouverGrandFrereFilsRoot(NSTreeNode* /*pNSTreeNode*/, int ligne)
{
  if (_NodeArray.empty())
  	return (NSTreeNode*) 0 ;

  // In order to know what is the "root column", we take 1st node's column
  //
  iterNSTreeNode iter = _NodeArray.begin() ;
  int iRefCol = (*iter)->getColonne() ;

  // NSTreeNode* pNSTreeFrere = NULL ;    // Old code, first node not considered
  NSTreeNode* pNSTreeFrere = *iter ;

  int lignEnCours = 0 ;
  for ( ; _NodeArray.end() != iter ; iter++)
  {
  	if ((*iter)->getColonne() == iRefCol)
    	if (((*iter)->getLigne() > lignEnCours) && ((*iter)->getLigne() < ligne))
      {
      	lignEnCours = (*iter)->getLigne() ;
        pNSTreeFrere = *iter ;
      }
  }
  return (pNSTreeFrere) ;
}

//------------------------------------------------------------------------//Touver le TTReenode dont le père est le root et qui soit le plus proche de
// l'élément sélectionné (cadet)
//------------------------------------------------------------------------
NSTreeNode*
NSTreeWindow::TrouverPetitFrereFilsRoot(NSTreeNode* /*pNSTreeNode*/, int ligne)
{
	if (_NodeArray.empty())
		return (NSTreeNode*) 0 ;

  // In order to know what is the "root column", we take 1st node's column
  //
  iterNSTreeNode iter = _NodeArray.begin() ;
  int iRefCol = (*iter)->getColonne() ;

	int lignEnCours = 1000000 ;
  NSTreeNode* pNSTreePetitFrere = 0 ;
  //chercher le frère cadet le plus proche de pNSTreeNode

  for ( ; _NodeArray.end() != iter ; iter++)
  {
  	if ((*iter)->getColonne() == iRefCol)
    {
    	if ((false == (*iter)->estLie()) &&
          ((*iter)->getLigne() < lignEnCours) && ((*iter)->getLigne() > ligne))
      {
      	lignEnCours = (*iter)->getLigne() ;
        pNSTreePetitFrere = *iter ;
      }
    }
  }
  return (pNSTreePetitFrere) ;
}

NSTreeNode*
NSTreeWindow::GetNodeForPath(string sPath, string separator)
{
  if ((string("") == sPath) || (_NodeArray.empty()))
    return (NSTreeNode*) 0 ;

  ClasseStringVector Vect ;
	DecomposeChaine(&sPath, &Vect, separator) ;

  if (Vect.empty())
		return (NSTreeNode*) 0 ;

  // The principle here is that the first element in sPath is the root concept
  // of the document; hence it doesn't appear as a node
  //
  int iVectorBase = 0 ;
  if (_pControle && _pControle->getTransfert() && _pControle->getTransfert()->getFilsItem())
  {
    BBFilsItem* pFilsItem = _pControle->getTransfert()->getFilsItem() ;
    string sFilsLabel = pFilsItem->getItemLabel() ;
    string sLabelSens = string("") ;
    NSDico::donneCodeSens(&sFilsLabel, &sLabelSens) ;

    if (sLabelSens != Vect[0]->getItem())
      return (NSTreeNode*) 0 ;

    iVectorBase = 1 ;
  }

  // This algorithm expects that NodeArray is sorted; let's make sure it is
  //
  _NodeArray.Sort() ;

  iterNSTreeNode iter = _NodeArray.begin() ;

  //
  size_t iColToSearch = NODES_COL_BASE ;
  string sToSearch    = Vect[iColToSearch - NODES_COL_BASE + iVectorBase]->getItem() ;

  for ( ; _NodeArray.end() != iter ; iter++)
 	{
    // Right information in the right place
    //
    if ((*iter)->getColonne() == (int) iColToSearch)
    {
      if ((*iter)->getLabelSens() == sToSearch)
      {
        // Was it the last item in path: means search is successful
        //
        if (Vect.size() == iColToSearch - NODES_COL_BASE + iVectorBase + 1)
          return *iter ;
        else
        {
          iColToSearch++ ;
          sToSearch = Vect[iColToSearch - NODES_COL_BASE + iVectorBase]->getItem() ;
        }
      }
    }
    else if ((*iter)->getColonne() < (int) iColToSearch)
    {
      if ((*iter)->getLabelSens() == sToSearch)
        iColToSearch = (*iter)->getColonne() + 1 ;
      else
        iColToSearch = (*iter)->getColonne() ;

      sToSearch = Vect[iColToSearch - NODES_COL_BASE + iVectorBase]->getItem() ;
    }
  }

  return (NSTreeNode*) 0 ;
}

//----------------------------------------------------------------------
//								Il s'agit d'un code lexique
// 						on récupére le code et le libellé
//						 on crée un BBFilsTtem rattaché au noeud
//			  on lance une boîte d'édition si le code est une valeur chiffrée
//							on crée un frère et un fils fictifs
//----------------------------------------------------------------------
void
NSTreeWindow::DicoGetCodeLexique()
{
try
{
	if ((NSEditDico*) NULL == _pEditDico)
  	return ;

  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" DicoGetCodeLexique on EditDico ") + getEditDicoHandleAsString() ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  char code[BASE_LEXI_LEN + 1] ;
	_pEditDico->GetCodeLexiqueChoisi(code) ;
  string stringCode = string(code) ;

	if (string("") == stringCode)
	{
    _pEditDico->SetFocus() ;
    return ;
	}

	//MAJ du NSTreeNode à partir d'un TTreeNode
	TTreeNode noeud = GetSelection() ;

	string sChaineRecuperee = string("") ;
	_pEditDico->GetLabelChoisi(&sChaineRecuperee) ;

	noeud.SetText(sChaineRecuperee.c_str(), true) ;

	// La TreeWindow va reprendre le focus ==>
	// on sort du mode "Edition du pEditDico"
	// ATTENTION : Si SortieEditionDico() n'est pas appelé assez tôt, on risque
	//             de planter sur une interception de EvLButtonUp de la TreeWin
	//
	SortieEditionDico() ;

	bool CreerFilsChampEdit = false ; //créer un fils lanceur de la boîte d'édition
	bool bDateHeure = false ; //créer un fils lanceur de la boîte d'édition (date et heure)
	if (noeud.GetItem())
	{
		NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
    if ((NSTreeNode*) NULL == pNSTreeNode)
      return ;

    pNSTreeNode->SetImageIndex(0) ;
    pNSTreeNode->SetSelectedImageIndex(0) ;
    pNSTreeNode->SetInteret("A") ;
    if (pNSTreeNode)
    {
    	if (string("") != pNSTreeNode->getGlobalText())
      {
      	if (_pEditDico)
        {
          string sMsg = string("TreeWindow ") + getHandleAsString() + string(" DicoGetCodeLexique: deleting EditDico.") ;
          pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

          KillDico() ;
        }
      }
      // Si l'ancien NSTreeNode contenait un texte libre avec
      // des frères liés : il faut les tuer
      if (false == pNSTreeNode->getVectLinkedBros()->empty())
      {
      	//décaler les NSTreeNodes qui existent
        // _NodeArray.ShiftLines(pNSTreeNode->getLigne() + 1, -int(pNSTreeNode->VectFrereLie.size())) ;

        iterNSTreeNode it = pNSTreeNode->getVectLinkedBros()->begin() ;
        while (pNSTreeNode->getVectLinkedBros()->end() != it)
        {        	NSTreeNode* pNSTemp = (*it) ;
          pNSTemp->Delete() ;
          _NodeArray.RemoveElement((*it)) ;
          if (pNSTreeNode->getFatherNode())
          	pNSTreeNode->getFatherNode()->getVectSons()->RemoveElement((*it)) ;
          delete pNSTemp ;
          pNSTreeNode->getVectLinkedBros()->erase(it) ;
        }
        pNSTreeNode->getVectLinkedBros()->vider() ;
        ReconstruireFils(pNSTreeNode) ;
        pNSTreeNode->SetGlobalText(string("")) ;
      }

      pNSTreeNode->SetType(string("CC")) ;

      pNSTreeNode->SetLexique(stringCode) ;
      pNSTreeNode->trouveLabel(sLang) ;

      pNSTreeNode->InstancieUrl(&Encyclo) ; //Encyclopédie.db
      string sNodeLabelString = pNSTreeNode->getLabelString() ;
      strip(sNodeLabelString, stripRight) ;
      pNSTreeNode->SetLabelString(sNodeLabelString) ;

      // Position dans l'arbre
      string sCodeSens ;
      string sNodeLabel = pNSTreeNode->getEtiquette() ;
      NSDico::donneCodeSens(&sNodeLabel, &sCodeSens) ;

      // Si le NSTreeNode existe , ne pas le créer : changer seulement son code
      //if(pNSTreeNode->pControle == 0)
      if (pNSTreeNode->estFictif())
      	MiseAjourNSTreeNode(pNSTreeNode) ;
      else
      	ReMiseAjourNSTreeNode(pNSTreeNode, stringCode, "") ;

      BBItem* pBBItem = pNSTreeNode->getBBFilsItem()->getItemFather() ;

      // On montre l'arbre, avec son noeud nouveau ou modifié, au Blackboard
//            pBBItem->pBigBoss->showBB();

			pNSTreeNode->SetPosition(pBBItem->_sLocalisation +
         	                        string(1, cheminSeparationMARK) + sCodeSens) ;

      //
      // Si le code lexique de ce pNSTreeNode commence par V
      // (valeur chiffrée), alors lui créer un fils qui lancera
      // la boîte d'édition et récupérera la valeur donnée et son unité
      //
      if      (NSDico::CodeCategorie(pNSTreeNode->getEtiquette()) == string("V"))
      	CreerFilsChampEdit = true ;
      else if (NSDico::CodeCategorie(pNSTreeNode->getEtiquette()) == string("K"))
      	bDateHeure = true ;

      pNSTreeNode->SetImageIndex(0) ;
      pNSTreeNode->SetSelectedImageIndex(0) ;

      // Ajoute un frère et d'un fils
      AjouteElementFictif(pNSTreeNode, &noeud, CreerFilsChampEdit, bDateHeure) ;

      pNSTreeNode->DicoKillFocus(&stringCode, "CC") ;
      pNSTreeNode->ctrlNotification() ;
      //
      // Contracter pNSTreeNode si valeur chiffrée
      //
      if ((false == pNSTreeNode->getVectSons()->empty()) &&
          (CreerFilsChampEdit || bDateHeure) &&
          ((*(pNSTreeNode->getVectSons()->begin()))->getLabelString() != "" ))
      {
      	string sLabel ;
        //
        // Contracter le père
        //
        NSTreeNode* pFirstSon = *(pNSTreeNode->getVectSons()->begin()) ;

        string sLabelFils = pFirstSon->getLabelString() ;
        if ((sLabelFils.find("<") != NPOS) || (sLabelFils.find(">") != NPOS))
        	sLabel = pNSTreeNode->getLabelString() + string(" ") + pFirstSon->getLabelString() ;
        else
        	sLabel = pNSTreeNode->getLabelString() + string(" : ") + pFirstSon->getLabelString() ;
        pNSTreeNode->SetText(sLabel.c_str(), true) ;
        pNSTreeNode->CollapseTousNoeud() ;
        // Affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
        // parmi ceux de ses descendants
        AfficheInteretMaximum(pNSTreeNode) ;
      }
		}
	}
	if (_pEditDico)
		_pEditDico->getDico()->getDicoDialog()->setAmmorce(string("")) ;
	SetFocus() ;

	SetDirty(true) ;
}
catch (...)
{
	erreur("Exception (NSTreeWindow::DicoGetCodeLexique).", standardError, 0) ;
}
}

string
NSTreeWindow::DicoGetText()
{
  if ((NSEditDico*) NULL == _pEditDico)
    return string("") ;

  int len = _pEditDico->GetTextLen() ;
  char far* texte = new char[len + 1] ;

  _pEditDico->GetText(texte, len + 1) ;
  string sTexte = string(texte) ;

  delete[] texte ;

  return sTexte ;
}

//*********************************************************************
//
//								TRAITEMENT DES TEXTES LIBRES//
//*********************************************************************
void
NSTreeWindow::DicoGetTexteLibre(string* pTexteLibre)
{
	if ((string*) NULL == pTexteLibre)
		return ;

  if (string("") == *pTexteLibre)
  {
    string sMsg = string("TreeWindow ") + getHandleAsString() + string(" DicoGetTexteLibre for empty string on EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    EvDelete() ;
    return ;
  }

  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" DicoGetTexteLibre on EditDico ") + getEditDicoHandleAsString() ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

try
{
	VecteurChaine VecteurTL ;

  // First step, cut text according to carriage returns
  //
  size_t posRN  = pTexteLibre->find("\r\n") ;
  size_t posR   = pTexteLibre->find("\r") ;
  size_t posN   = pTexteLibre->find("\n") ;
  size_t newpos = min(posR, posN) ;
  size_t pos    = 0 ;
  size_t len    = strlen(pTexteLibre->c_str()) ;
  while (NPOS != newpos)
	{
  	string sTxt = string(*pTexteLibre, pos, newpos - pos) ;
    VecteurTL.push_back(new string(sTxt)) ;

    pos = newpos + 1 ;
    if (posRN == newpos)
    	pos++ ;

    if (pos == len)
  		break ;

  	posRN  = pTexteLibre->find("\r\n", pos) ;
  	posR   = pTexteLibre->find("\r", pos) ;
  	posN   = pTexteLibre->find("\n", pos) ;
  	newpos = min(posR, posN) ;
	}
  if (pos < len)
  {
  	string sTxt = string(*pTexteLibre, pos, len - pos) ;
    VecteurTL.push_back(new string(sTxt)) ;
  }

  if (VecteurTL.empty())
		return ;

	TTreeNode noeud = GetSelection() ;
  int tailleVect = 0;
  if (noeud.GetItem())
  {
  	NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
    if (pNSTreeNode)
    {
    	IterString itStr = VecteurTL.begin() ;

    	VecteurChaine VecteurTexte ;   // décomposer pTexteLibre
      DecomposeChaineTL(*itStr, &VecteurTexte) ;

/*
      //
      // Pas de frères liés
      //
      if (string("") == pNSTreeNode->sTexteGlobal)
      {
      	//MAJ
        // _NodeArray.ShiftLines(pNSTreeNode->getLigne() + 1, VecteurTexte.size() - 1) ;
      }
      //
      // Il y a des frères liés
      //
      else
      {
      	if (VecteurTexte.size() > 1)
        {
        	bool OK = true ;
          //décaler les NSTreeNodes qui existent
          for (iterNSTreeNode iter = _NodeArray.begin(); _NodeArray.end() != iter ; iter++)
          {
          	if ((*iter)->FrerePilote)
            {
            	if ((*iter)->FrerePilote != pNSTreeNode)              	OK = true ;
              else
              	OK = false ;
            }
            else
            	OK = true ;

            // if (OK &&
            // 		((*iter)->getLigne() > pNSTreeNode->getLigne()))
            // {
            	//MAJ
            //   (*iter)->setLigne((*iter)->getLigne() - pNSTreeNode->VectFrereLie.size()) ;
            //   (*iter)->setLigne((*iter)->getLigne() + VecteurTexte.size() - 1) ;
            // }
          }
        }
      }
*/

      // RepartirTexteLibre(pNSTreeNode, &VecteurTexte, pTexteLibre, &tailleVect) ;
      RepartirTexteLibre(pNSTreeNode, &VecteurTexte, *itStr, &tailleVect) ;

      string sFreeTextCodeForNode = pNSTreeNode->getFreeTextCode() ;
      pNSTreeNode->SetComplement(sFreeTextCodeForNode) ;
      pNSTreeNode->DicoKillFocus(&sFreeTextCodeForNode, "CL") ;
      pNSTreeNode->SetType(string("CL")) ;
      pNSTreeNode->Interet() ;
    //pNSTreeNode->SetImageIndex(5);
    //pNSTreeNode->SetSelectedImageIndex(5);

    	if (_pControle && _pControle->getFonction())
  		{
  			if (_pControle->getFonction()->containFonction("CLASSIF_FREE"))
    			Classifier() ;
    		else
    			/* int iNotifier = */ _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;
  		}

      if (VecteurTL.size() > 1)
      {
      	NSCutPaste CutPaste(pContexte->getSuperviseur()) ;
        IterString itStr = VecteurTL.begin() ;
        itStr++ ;
        for ( ; VecteurTL.end() != itStr; itStr++)
        {
        	Message Msg ;
          Msg.SetLexique(string("£?????")) ;

          string sIndice = string("00000") ;
					if (false == CutPaste.getTL()->empty())
					{
    				NSCutPastTL* pCPTL = CutPaste.getTL()->back() ;
						sIndice = pCPTL->sIndice ;

      			int i = strlen(sIndice.c_str()) ;
            bool tourner = true;
            while ((tourner) && (i > 0))
            {
              i-- ;
              if (((sIndice[i] >= '0') && (sIndice[i] < '9')) ||                  ((sIndice[i] >= 'A') && (sIndice[i] < 'Z')))
              {
                sIndice[i] = char(sIndice[i] + 1) ;
                tourner = false ;
              }
              else if ('9' == sIndice[i])
              {
                sIndice[i] = 'A' ;
                tourner = false ;
              }
              else
                sIndice[i] = '0' ;
            }
    			}

    			NSCutPastTL CPTexte ;
    			CPTexte.sIndice = sIndice ;
    			CPTexte.sTexte  = **itStr ;
    			CutPaste.getTL()->push_back(new NSCutPastTL(CPTexte)) ;

          NSPatPathoArray DragPatho(pContexte->getSuperviseur()) ;
          CutPaste.initFromPatPatho(&DragPatho) ;

    			Msg.SetComplement(sIndice) ;    			DragPatho.ajoutePatho(string("£?????"), &Msg, 0) ;

          CutPaste.setPatPatho(&DragPatho) ;
        }

      	DragApres(pNSTreeNode, "COPIE", &CutPaste) ;
      }
      else if (false == _bLoosingFocus)
      	AutoriserCreationFrere(pNSTreeNode) ;
      // donner le focus au NSEDitDico sur le frère qui vient d'être crée
      // (uniquement si on en a créé un nouveau)

      // No longer works because NodeArray is now sorted on lines
      //
      // if (_NodeArray.back() != pNSTreeNode)
      //  _NodeArray.back()->SelectItem(TVGN_CARET) ;

      NSTreeNode* pPetitFrere = (NSTreeNode*) 0 ;

      NSTreeNode* pNSTreePere = pNSTreeNode->getFatherNode() ;
      if (pNSTreePere)
        pPetitFrere = TrouverPetitFrere(pNSTreeNode, pNSTreeNode->getLigne()) ;
      else
        pPetitFrere = TrouverPetitFrereFilsRoot(pNSTreeNode, pNSTreeNode->getLigne()) ;

      if (pPetitFrere && pPetitFrere->estFictif())
        pPetitFrere->SelectItem(TVGN_CARET) ;
      else
        KillDico() ;
    }
  }

  if (_pEditDico)
  {
    // We must be certain the editDico will not retain focus
    _pEditDico->AcceptToLooseFocus() ;
  	_pEditDico->getDico()->getDicoDialog()->setAmmorce(string("")) ;
  }
  SetFocus() ;

  // La TreeWindow vient de reprendre le focus ==>
  // on sort de l'edition du pEditDico pour
  // la création d'un texte libre
  SortieEditionDico() ;
  Invalidate() ;
	SetDirty(true) ;}
catch (...)
{
	erreur("Exception NSTreeWindow::DicoGetTexteLibre.", standardError, 0);
}
}

//-------------------------------------------------------------------------// 					  	 décomposer pTexteLibre
// 					en un ensemble de 40 caractères
//-------------------------------------------------------------------------
void
NSTreeWindow::DecomposeChaineTL(string* pChaine, VecteurChaine* pVect)
{
try
{
	size_t LongueurTexte = 2 * TEXTE_LIBRE_LEN ;
	if (strlen(pChaine->c_str()) <= LongueurTexte)
	{
		pVect->push_back(new string (*pChaine) ) ;
    return ;
	}

	size_t taille = 0 ;
	size_t i = LongueurTexte ;
	while ((' ' != (*pChaine)[i]) && (i > 0))
		i-- ;

	if (0 == i)
  {
  	pVect->push_back(new string (*pChaine) ) ;
    return ;
  }

	taille = i + 1 ;
	pVect->push_back(new string((*pChaine), 0, i)) ;
	i = i + LongueurTexte ;

	while (i <= strlen(pChaine->c_str()))
	{
		while ((i > taille) && (' ' != (*pChaine)[i]))
    	i-- ;

		if (taille == i)
    {
    	while ((i <= strlen(pChaine->c_str())) && (' ' != (*pChaine)[i]))
    		i++ ;

      if (i > strlen(pChaine->c_str()))
      	break ;
    }


    pVect->push_back(new string((*pChaine), taille, i - taille)) ;

    taille = i + 1 ;
    i = i + LongueurTexte ;
	}

	pVect->push_back(new string((*pChaine), taille, strlen(pChaine->c_str())- taille)) ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::DecomposeChaineTL.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------// Répartir un texte libre sur plusieurs NSTreeNode.
// pNSTreeNode est le NSTreeNode pilote.
//-------------------------------------------------------------------------
void
NSTreeWindow::RepartirTexteLibre(NSTreeNode* pNSTreeNode,
						         VecteurChaine* VecteurTexte,
                                 string* pTexteLibre, int* taille)
{
	if (((NSTreeNode*) NULL == pNSTreeNode) ||
           ((VecteurChaine*) NULL == VecteurTexte) || (VecteurTexte->empty()))
		return ;

	int ligne = pNSTreeNode->getLigne() ;
	int colonne = pNSTreeNode->getColonne() ;

	IterString iter = VecteurTexte->begin();

	pNSTreeNode->SetType(string("CL")) ;
	pNSTreeNode->SetLabelString(*(*iter)) ;

	pNSTreeNode->SetLexique(string("£?????")) ;
	pNSTreeNode->SetText((*iter)->c_str(), true) ;
	if (pNSTreeNode->estFictif())
		MiseAjourNSTreeNode(pNSTreeNode) ;
	else
  	ReMiseAjourNSTreeNode(pNSTreeNode, string("£?????"), pNSTreeNode->getFreeTextCode()) ;
  string sFreeTextCodeForNode = pNSTreeNode->getFreeTextCode() ;
	pNSTreeNode->SetComplement(sFreeTextCodeForNode) ;
	pNSTreeNode->DicoKillFocus(&sFreeTextCodeForNode, "CL") ;
	iter++ ;
	pNSTreeNode->SetGlobalText(*pTexteLibre) ;
	//
  // Si on ne change pas le nombre de fils liés : on change seulement
  // leurs étiquettes
  //
  if ((VecteurTexte->size() - 1) == pNSTreeNode->getVectLinkedBros()->size())
  {
  	iterNSTreeNode jter = pNSTreeNode->getVectLinkedBros()->begin() ;
    while ((VecteurTexte->end() != iter) &&
               (pNSTreeNode->getVectLinkedBros()->end() != jter))
    {
    	ligne++ ;
      (*jter)->SetLabelString(*(*iter)) ;
      (*jter)->SetText((*(*iter)).c_str()) ;
      iter++ ;
      jter++ ;
    }
	}
  else
  {
  	// Tuer les anciens frères liés qui vont déleter "interface"
    // les fils de pNSTreeNode
    if (false == pNSTreeNode->getVectLinkedBros()->empty())
    {
    	iterNSTreeNode it = pNSTreeNode->getVectLinkedBros()->begin() ;
      while (pNSTreeNode->getVectLinkedBros()->end() != it)
      {
      	NSTreeNode* pNSTemp = (*it) ;
        pNSTemp->Delete() ;
        _NodeArray.RemoveElement((*it)) ;
        if (pNSTreeNode->getFatherNode())
        	pNSTreeNode->getFatherNode()->getVectSons()->RemoveElement((*it)) ;
        pNSTreeNode->getVectLinkedBros()->RemoveElement((*it)) ;
        delete pNSTemp ;
      }
    }
    // Les fils de pNSTreeNode doivent être déletés "interface" puis
    // recrés au niveau interface
    else
    {
      if (false == pNSTreeNode->getVectSons()->empty())
    	  for (iterNSTreeNode it = pNSTreeNode->getVectSons()->begin() ;
				      	pNSTreeNode->getVectSons()->end() != it ; it++)
      	  (*it)->Delete() ;
    }

    pNSTreeNode->getVectLinkedBros()->vider() ;
    while (VecteurTexte->end() != iter)
    {
    	ligne++ ;
      AjouteFrereFictifTexteLibre(pNSTreeNode, ligne, colonne, *(*iter)) ;
      iter++ ;
    }
    // Si pNSTreeNode a des fils et des frères (au dessous de lui),
    // il faut les reconstruire au niveau interface au moyen du
    // dernier frère lié
    //
    ReconstruireFils(pNSTreeNode) ;
	}
  *taille += (VecteurTexte->size() - 1) ;
}

//----------------------------------------------------------------------// 							si pNSTreeNode a des fils ,
//						il faut les reconstruire au niveau interface au
// 							moyen du dernier frère lié
//----------------------------------------------------------------------
void
NSTreeWindow::ReconstruireFils(NSTreeNode* pNSTreeNode)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) || (pNSTreeNode->getVectSons()->empty()))
    return ;

	int ligneFils   = pNSTreeNode->getLigne() ;
	int colonneFils = pNSTreeNode->getColonne() ;
	iterNSTreeNode it = pNSTreeNode->getVectSons()->begin() ;
	int compteur = 0 ; //nombre de fils

	string sIdentite ;
	string sCode ;
	NSControle* pControle = (NSControle*) 0 ;

try
{
	for ( ; pNSTreeNode->getVectSons()->end() != it ; it++)
	{
  	TTreeNode* pAmorce = new TTreeNode(*this, (*it)->getLabelString().c_str()) ;
    sIdentite = (*it)->getEtiquette() ;
    sCode     = (*it)->getFreeTextCode() ;
    pControle = (*it)->getControl() ;
    if (pNSTreeNode->getVectLinkedBros()->empty()) //pas de frères liés
    {
    	*pAmorce    = pNSTreeNode->AddChild(*pAmorce) ;
      ligneFils   = ligneFils + compteur ;
      colonneFils	= colonneFils ;
    }
    else
    {
    	NSTreeNode*	pNSTreeFrere = pNSTreeNode->getVectLinkedBros()->back() ; //dérnier élément de VectFrereLie crée son frère
			*pAmorce    = pNSTreeFrere->AddChild(*pAmorce) ;
      //*(*it) = pNSTreeFrere->AddChild(*(*it));
      ligneFils   = pNSTreeFrere->getLigne() + 1 + compteur ;
      colonneFils = pNSTreeFrere->getColonne() + 1 ;
    }
    (*it)->SetLexique(sIdentite) ;
    (*it)->SetHTreeItem(pAmorce->operator HTREEITEM()) ;
    (*it)->setLigne(ligneFils) ;
    (*it)->setColonne(colonneFils) ;
    (*it)->SetControl(pControle) ;
    compteur++ ;

    ReconstruireFils((*it)) ;

    delete pAmorce ;
	}
}
catch (...)
{
	erreur("Exception NSTreeWindow::ReconstruireFils.", standardError, 0) ;
}
}

//----------------------------------------------------------------------
// 			si pNSTreeNode a des frères (au dessous de lui),
//						il faut les reconstruire au niveau interface au
// 							moyen du dernier frère lié
//----------------------------------------------------------------------
void
NSTreeWindow::ReconstruireFrere(NSTreeNode* pNSTreeNode)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) ||
                   ((NSTreeNode*) NULL == pNSTreeNode->getFatherNode()))
    return ;

  NSFilsTreeNodeArray* pVectSons = pNSTreeNode->getFatherNode()->getVectSons() ;
  if (((NSFilsTreeNodeArray*) NULL == pVectSons) || pVectSons->empty())
    return ;

try
{
	int ligneFils   = pNSTreeNode->getLigne() ;
	int colonneFils = pNSTreeNode->getColonne() ;

	iterNSTreeNode it = pVectSons->begin() ;

	for ( ; (pVectSons->end() != it) && ((*it) != pNSTreeNode); it++)
	{
  	TTreeNode* pAmorce = new TTreeNode(*this, (*it)->getLabelString().c_str()) ;

    if (pNSTreeNode->getVectLinkedBros()->empty()) //pas de frères liés
    	*pAmorce = pNSTreeNode->AddChild(*pAmorce);
    else
    {
    	NSTreeNode*	pNSTreeFrere = pNSTreeNode->getVectLinkedBros()->back() ; //dérnier élément de VectFrereLie crée son frère
			*pAmorce    = pNSTreeFrere->AddChild(*pAmorce) ;
      ligneFils   = pNSTreeFrere->getLigne() + 1 ;
      colonneFils = pNSTreeFrere->getColonne() + 1 ;
    }
    (*it)->SetHTreeItem(pAmorce->operator HTREEITEM()) ;
    (*it)->setLigne(ligneFils) ;
    (*it)->setColonne(colonneFils) ;
    ReconstruireFils((*it)) ;
    delete pAmorce ;
	}
}
catch (...)
{
	erreur("Exception NSTreeWindow::ReconstruireFrere.", standardError, 0) ;
}
}

//*********************************************************************//
//								FIN TRAITEMENT DES TEXTES LIBRES
//
//*********************************************************************

// Fonction appelée par TvnBeginLabelEdit
// Note : le curseur est géré par EvMouseMove
void
NSTreeWindow::EntreeEditionDico()
{
  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" EntreeEditionDico") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	bEditionDico = true ;
}

// Fonction appelée dans quatre cas :
// - Insertion code lexique
// - Fin d'édition d'un texte libre
// - Sortie par VK_DOWN ou VK_UP
void
NSTreeWindow::SortieEditionDico()
{
  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" SortieEditionDico") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	bEditionDico = false ;

  // on rétablit eventuellement le curseur
  if (bCurseurTexte)
  {
  	SetCursor(0, IDC_ARROW) ;
    bCurseurTexte = false ;
	}

  // Invalidate() ;
}

//*********************************************************************
//
//								GESTION DE L'AJOUT DES FILS ET DES FRERES
//
//*********************************************************************

//----------------------------------------------------------------------
//				  Donne les coordonnees maximales pour les fils de pNSTreeNode
// 					si pNSTreeNode a des frères liés : en tenir compte.//// Si bJustReal = true, on s'arrête au premier fils fictif// (uniquement pour la colonne de gauche, bien entendu)////----------------------------------------------------------------------
void
NSTreeWindow::GetMaxCoordonnees(NSTreeNode* pNSTreeNode, int *ligne,
                                                int *colonne, bool bJustReal)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

	// a minima : les coordonnées du noeud lui même
	*ligne   = pNSTreeNode->getLigne() ;
	*colonne = pNSTreeNode->getColonne() ;

	// on prend en compte les frères liés
	if (false == pNSTreeNode->getVectLinkedBros()->empty())
	{
  	*ligne   = (pNSTreeNode->getVectLinkedBros()->back())->getLigne() ;
    *colonne = (pNSTreeNode->getVectLinkedBros()->back())->getColonne() ;
	}

	// on traite les descendants
  for (iterNSTreeNode iter = pNSTreeNode->getVectSons()->begin() ;
  								pNSTreeNode->getVectSons()->end() != iter ; iter++)
	{
		if ((bJustReal) && (((*iter)->isProposed()) || ((*iter)->estFictif())))
    	return ;

    if (*ligne <= (*iter)->getLigne())
    	*ligne = (*iter)->getLigne() ;

    if (*colonne <= (*iter)->getColonne())
    	*colonne = (*iter)->getColonne() ;

    GetMaxCoordonnees((*iter), ligne, colonne) ; // ne pas tenir compte
                                                    // de bJustReal
	}
}

//--------------------------------------------------------------------------// ajout d'un frère fictif à  pNSTreeNode
// bLie montre si le frère qu 'on va créer est lié à pNSTreeNodePilote ou non
//--------------------------------------------------------------------------
void
NSTreeWindow::AjouteFrereFictifTexteLibre(NSTreeNode* pNSTreeNodePilote, int ligne,
									int colonne, string sTexteLibre, bool /*bLie*/)
{
  if ((NSTreeNode*) NULL == pNSTreeNodePilote)
    return ;

	NSTreeNode* pNSTreeFrere = 0 ;

try
{
	if (pNSTreeNodePilote->getVectLinkedBros()->empty()) //pas de frères liés: pNSTreeNodePilote crée son frère
		pNSTreeFrere = pNSTreeNodePilote ;
  else
  	pNSTreeFrere = pNSTreeNodePilote->getVectLinkedBros()->back() ; //dernier élément de VectFrereLie crée son frère

//	pTTreeNodeFrere = &(pNSTreeFrere->InsertItem(TTreeNode(*this, sTexteLibre.c_str())));

	NSTreeNode* pNewNSTreeNode =
    new NSTreeNode(pNSTreeFrere->InsertItem(TTreeNode(*this, sTexteLibre.c_str())), pContexte);

  pNewNSTreeNode->SetImageIndex(-1/*, bool sendNow = true*/) ;
  pNewNSTreeNode->SetSelectedImageIndex(-1) ;
  pNewNSTreeNode->SetControl((NSControle*) 0) ;
  pNewNSTreeNode->SetFatherNode(pNSTreeNodePilote->getFatherNode()) ;

  pNewNSTreeNode->SetPilotNode(pNSTreeNodePilote) ;
  pNewNSTreeNode->SetType(string("CL")) ;
  pNewNSTreeNode->SetLexique(string("£?????")) ;
  pNSTreeNodePilote->getVectLinkedBros()->push_back(pNewNSTreeNode) ;

	pNewNSTreeNode->SetLabelString(sTexteLibre) ;
	//pNewNSTreeNode devient fils du père de  pNSTreeNode
	if (pNSTreeNodePilote->getFatherNode())
    pNSTreeNodePilote->getFatherNode()->getVectSons()->Referencer(pNewNSTreeNode) ;

	//MAJ ligne et colonne
	pNewNSTreeNode->setLigne(ligne) ;
	pNewNSTreeNode->setColonne(colonne) ;

  _NodeArray.AddElement(pNewNSTreeNode) ;

	Update() ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::AjouteFrereFictifTexteLibre.", standardError, 0) ;
}
}

//--------------------------------------------------------------------------// ajout d'un fils fictif à  pNSTreeNodePere
//--------------------------------------------------------------------------
void
NSTreeWindow::AjouteFilsFictif(NSTreeNode* pNSTreeNodePere, TTreeNode* pTTreeNode,
                               int ligne, int colonne, string sIdentite,
                               bool estPropose, bool CreerFilsChampEdit,
                               bool bDateHeure)
{
try
{
  if ((NSTreeNode*) NULL == pNSTreeNodePere)
    return ;

	int ligneFils   = ligne ;
	int colonneFils = colonne ;
	TTreeNode* pAmorce = new TTreeNode(*this, "") ;

	if (pNSTreeNodePere->getVectLinkedBros()->empty()) //pas de frères liés
		*pAmorce = pTTreeNode->InsertChild(*pAmorce, TTreeNode::First) ;
	else
	{
  	NSTreeNode*	pNSTreeFrere = pNSTreeNodePere->getVectLinkedBros()->back() ; //dérnier élément de VectFrereLie crée son frère
		*pAmorce = pNSTreeFrere->InsertChild(*pAmorce, TTreeNode::First) ;
		ligneFils   = pNSTreeFrere->getLigne() + 1 ;
    colonneFils = pNSTreeFrere->getColonne() + 1 ;
	}
  NSTreeNode* pNewNSTreeNodeFils = new NSTreeNode(*pAmorce, pContexte) ;
  pNewNSTreeNodeFils->SetControl((NSControle*) 0) ;
  pNewNSTreeNodeFils->SetPilotNode((NSControle*) 0) ;
  //MAJ ligne et colonne
  pNewNSTreeNodeFils->setLigne(ligneFils) ;
  pNewNSTreeNodeFils->setColonne(colonneFils) ;
  pNewNSTreeNodeFils->SetInteret("A") ;
  //pNewNSTreeNodeFils devient fils de pNSTreeNodePere
  pNewNSTreeNodeFils->SetFatherNode(pNSTreeNodePere) ;
  pNSTreeNodePere->getVectSons()->Referencer(pNewNSTreeNodeFils) ;

  _NodeArray.AddElement(pNewNSTreeNodeFils) ;

	if (estPropose)//suite à la recherche dans le fil guide
	{
  	if (_pEditDico)
    {
      string sMsg = string("TreeWindow ") + getHandleAsString() + string(" AjouteFilsFictif on Fils Guides : deleting EditDico.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      KillDico() ;
    }

    pNewNSTreeNodeFils->SetProposed(true) ;
    pNewNSTreeNodeFils->setLibelle(sIdentite, sLang) ;
    //
    //mettre ? devant les étiquettes des fils
    //
    pNewNSTreeNodeFils->SetImageIndex(6) ;
    pNewNSTreeNodeFils->SetSelectedImageIndex(6,  true) ;

    pNewNSTreeNodeFils->SetLexique(sIdentite) ;
    pNewNSTreeNodeFils->SetText(pNewNSTreeNodeFils->getLabelString().c_str(), true) ;
	}

	else if (CreerFilsChampEdit)//valeur chiffrée
	{
  	if (_pEditDico)
    {
      string sMsg = string("TreeWindow ") + getHandleAsString() + string(" AjouteFilsFictif on num value : deleting EditDico ") + getEditDicoHandleAsString() ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      KillDico() ;
    }
    pNewNSTreeNodeFils->SetImageIndex(0) ;
    pNewNSTreeNodeFils->SetSelectedImageIndex(0, true) ;

    //
    // Chercher dans Convert.db si cette valeur chiffrée a une formule et si
    // tous les paramètres sont instanciés faire le cacul et mettre à jour
    // pNewNSTreeNodeFils->sLabel
    // Passer en paramètre la patpatho en cours
    //
    string sUniteConvert  = string("") ; // Unité dans convert.db
    string sMethodeCalcul = string("") ; // Nom de la méthode de calcul à mettre dans
                                         // le complément de pNewNSTreeNodeFils->pere

    //false pour ne pas tuer pTransfert lié à pBBitemNSTreeWindow
		//pBBitemNSTreeWindow->okFermerDialogue(true, false);

    // Getting instantaneous patpatho
    //
    pBBitemNSTreeWindow->rapatrieTmpPpt(pBBitemNSTreeWindow->_pBigBoss->getTmpPatho(), 0) ;
    NSPatPathoArray NSPatPathoEnCours(pContexte->getSuperviseur()) ;
    NSPatPathoEnCours = *(pBBitemNSTreeWindow->_pBigBoss->getTmpPatho()) ;

    // Processing value
    //
    if (Converter.isOpened())
    {
      string sNewLabel = string("") ;
    	Converter.CalculValeur(pNSTreeNodePere->getEtiquette(), &sNewLabel,
			                         &sUniteConvert, &sMethodeCalcul, &NSPatPathoEnCours) ;
      pNewNSTreeNodeFils->SetLabelString(sNewLabel) ;
    }

    if (pNewNSTreeNodeFils->getLabelString() != string(""))
    	pNewNSTreeNodeFils->SetComplement(pNewNSTreeNodeFils->getLabelString()) ;
    if (string("") != sUniteConvert)
    {
    	pNewNSTreeNodeFils->SetLexique("£N0;03") ;
      pNewNSTreeNodeFils->SetUnit(sUniteConvert + string("1")) ;
    }

    bool bCalcul = false ;//calcul dans convert.db
    if (pNewNSTreeNodeFils->getContenuTransfert() != string(""))
    	bCalcul = true ;
    RecupereValeurChiffree(pNewNSTreeNodeFils, bCalcul) ;

    if (pNewNSTreeNodeFils->estFictif())
    	MiseAjourNSTreeNode(pNewNSTreeNodeFils) ;
    if (pNewNSTreeNodeFils->getEtiquette() != string(""))
    {
      string sNodeTransfertContent = pNewNSTreeNodeFils->getContenuTransfert() ;
    	pNewNSTreeNodeFils->DicoKillFocus(&sNodeTransfertContent, "CL") ;
      pNewNSTreeNodeFils->ctrlNotification() ;
    }

    if (string("") != sMethodeCalcul)    	pNewNSTreeNodeFils->getFatherNode()->SetComplement(sMethodeCalcul) ;
	}
  else if (bDateHeure) //date ou heure
  {
  	if (_pEditDico)
    {
      string sMsg = string("TreeWindow ") + getHandleAsString() + string(" AjouteFilsFictif on date/hour : deleting EditDico ") + getEditDicoHandleAsString() ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

      KillDico() ;
    }
    pNewNSTreeNodeFils->SetImageIndex(0) ;
    pNewNSTreeNodeFils->SetSelectedImageIndex(0, true) ;

    RecupereUniteDateHeure(pNewNSTreeNodeFils) ;

    if (pNewNSTreeNodeFils->estFictif())
    	MiseAjourNSTreeNode(pNewNSTreeNodeFils) ;
    if (pNewNSTreeNodeFils->getEtiquette() != "")
    {
      string sNodeTransfertContent = pNewNSTreeNodeFils->getContenuTransfert() ;
    	pNewNSTreeNodeFils->DicoKillFocus(&sNodeTransfertContent, "CL") ;
      pNewNSTreeNodeFils->ctrlNotification() ;
    }
	}
  else
  	pNewNSTreeNodeFils->SelectItem(TVGN_CARET) ;

	delete pAmorce ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::AjouteFilsFictif.", standardError, 0) ;
}
}

//--------------------------------------------------------------------------// ajout d'un frère fictif (au dessus ) à pNSTreeNode.
//TTreeNode est celui du père de pNSTreeNode
//--------------------------------------------------------------------------
void
NSTreeWindow::AjouteFrereFictifAvant(NSTreeNode* pNSTreeNode, TTreeNode* pTTreeNodePere, int ligne,
							int colonne , string sFaconAjout)
{
try
{
  if (((NSTreeNode*) NULL == pTTreeNodePere) || ((NSTreeNode*) NULL == pNSTreeNode))
    return ;

  TTreeNode* pTTreeNode = new TTreeNode(*this, "") ;

  if (sFaconAjout == "Fin")
    *pTTreeNode = pTTreeNodePere->AddChild(*pTTreeNode) ;
  else if(sFaconAjout == "Debut")
    *pTTreeNode = pTTreeNodePere->InsertChild(*pTTreeNode, TTreeNode::First) ;

  NSTreeNode* pNewNSTreeNode = new NSTreeNode(*pTTreeNode, pContexte) ;
  pNewNSTreeNode->SetControl((NSControle*) 0) ;
  pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;
  pNewNSTreeNode->SetFatherNode(pNSTreeNode->getFatherNode()) ;
  pNewNSTreeNode->SetInteret("A") ;
  //pNewNSTreeNode devient fils du père de  pNSTreeNode
  if (pNSTreeNode->getFatherNode())
    pNSTreeNode->getFatherNode()->getVectSons()->Referencer(pNewNSTreeNode) ;

  //MAJ ligne et colonne
  pNewNSTreeNode->setLigne(ligne) ;
  pNewNSTreeNode->setColonne(colonne) ;

  _NodeArray.AddElement(pNewNSTreeNode) ;

  Update() ;

//??   pTTreeNodePere->GetChild().ExpandItem(TTreeNode::Expand);
    //delete item;

  delete pTTreeNode ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::AjouteFrereFictifAvant.", standardError, 0);
}
}

//----------------------------------------------------------------------
// 	créer  un frère  (en dessous) pour le noeud en question
//----------------------------------------------------------------------
void
NSTreeWindow::AjouteFrereFictifApres(NSTreeNode* pNSTreeFrere, int ligne, int colonne)
{
  if ((NSTreeNode*) NULL == pNSTreeFrere)
    return ;

  NSTreeNode* pNewNSTreeNode = 0 ;
  NSTreeNode* pNSTreeFrereCreateur = 0 ;

try
{
  if (pNSTreeFrere->getVectLinkedBros()->empty()) //pas de frères liés: pNSTreeNodePilote crée son frère
    pNSTreeFrereCreateur = pNSTreeFrere ;
  else
    pNSTreeFrereCreateur = pNSTreeFrere->getVectLinkedBros()->back() ; //dernier élément de VectFrereLie crée son frère

  pNewNSTreeNode =
      new NSTreeNode(pNSTreeFrereCreateur->InsertItem(TTreeNode(*this, "")), pContexte) ;

  pNewNSTreeNode->SetControl((NSControle*) 0) ;
  pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;
  pNewNSTreeNode->SetInteret("A") ;

  // MAJ ligne et colonne
  pNewNSTreeNode->setLigne(ligne) ;
  pNewNSTreeNode->setColonne(colonne) ;

  // pNewNSTreeNode devient fils de pNSTreeNode
  pNewNSTreeNode->SetFatherNode(pNSTreeFrere->getFatherNode()) ;
  if (pNSTreeFrere->getFatherNode())
    pNSTreeFrere->getFatherNode()->getVectSons()->Referencer(pNewNSTreeNode) ;

  _NodeArray.AddElement(pNewNSTreeNode) ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::AjouteFrereFictifApres.", standardError, 0);
}
}

//-----------------------------------------------------------------
//Autoriser la création d'un fils
//-----------------------------------------------------------------
void
NSTreeWindow::AutoriserCreationFils(NSTreeNode* pNSTreeNode, TTreeNode* Noeud)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  int ligne   = pNSTreeNode->getLigne() ;
  int colonne = pNSTreeNode->getColonne() ;

	//
  // Création classique de fils et de frère
  //
  bool bCreationFils = true ;

  // Chercher un élément non instancié : fictif (non lié) et non proposé
  // et dont le code lexique ne commence pas par V (chiffrée)
  //
  if (pNSTreeNode->getVectSons()->empty())
  {
    iterNSTreeNode iter = pNSTreeNode->getVectSons()->begin() ;
    for (; (pNSTreeNode->getVectSons()->end() != iter) && (bCreationFils) ; iter++)
    {
      // bool isRealNode(NSTreeNode* pNSTreeNode) ;
      if ((*iter)->estFictifPur() && (false == (*iter)->isProposed()))
        bCreationFils = false ;
    }
  }

  if (bCreationFils)
  {
    // MAJ des coordonnées des fils et des petits fils de pNSTreeNode
    //
    pNSTreeNode->MettreAJourLigne(1) ;

    // Décaler les NSTreeNodes qui existent et dont la ligne est supérieure
    // à ligneMaxInit
    // for (iterNSTreeNode iter = _NodeArray.begin(); _NodeArray.end() != iter ; iter++)
    // {
    //   if (((*iter)->getLigne() > ligne) && (!(*iter)->Descendant(pNSTreeNode)) &&
    //             ((*iter)->FrerePilote != pNSTreeNode))
    //     (*iter)->setLigne((*iter)->getLigne() + 1) ;
                // DecalageLigneFrere  : éventuellement un
                  //frère et  un fils de plus
    // }
    AjouteFilsFictif(pNSTreeNode, Noeud, ligne + 1, colonne + 1, "",
                            false, false, false) ;
  }
}

//-----------------------------------------------------------------
// Autoriser la création d'un frère
//-----------------------------------------------------------------
void
NSTreeWindow::AutoriserCreationFrere(NSTreeNode* pNSTreeNode)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

	int ligne   = pNSTreeNode->getLigne() ;
  int colonne = pNSTreeNode->getColonne() ;

  int ligneMaxInit = ligne ;
  int colonneMax   = colonne ;
  GetMaxCoordonnees(pNSTreeNode, &ligneMaxInit, &colonneMax) ;
  int ligneMax = ligneMaxInit ;

  //savoir si on peut créer un frère ou non
  //
  bool bCreationFrere = true ;
  // int DecalageLigneFrere = 1 ;

  // On peut décider d'interdire la création d'un frère, lors de la validation
  // d'un noeud, si ce noeud est suivi d'un frère fictif
  //
  bool bInterdireSiFrereFictif = false ;

  if ((bInterdireSiFrereFictif) && (false == _NodeArray.empty()))
  {
    iterNSTreeNode it = _NodeArray.begin() ;
    //chercher un frère (fictif non lié )à pNSTreeNode : même colonne et lignes différentes
    for ( ; (_NodeArray.end() != it) && (bCreationFrere) ; it++)
    {
      if (((*it)->getLigne()      == (pNSTreeNode->getLigne() + 1)) &&
          ((*it)->getColonne()    == pNSTreeNode->getColonne())     &&
          ((*it)->getFatherNode() == pNSTreeNode->getFatherNode())  &&
          ((*it)->estFictifPur()))
      {
        bCreationFrere = false ;      //ne pas créer un frère
        // DecalageLigneFrere = 0 ; //pas de décalage
      }
    }
  }
  // Décaler les NSTreeNodes qui existent et dont la ligne est
  // supérieure à ligneMaxInit
  // if ((DecalageLigneFrere != 0) && (false == _NodeArray.empty()))
  // {
  //  for (iterNSTreeNode iter = _NodeArray.begin(); _NodeArray.end() != iter ; iter++)
  //  {
  //    if (((*iter)->getLigne() > ligneMaxInit) &&
  //           	(!(*iter)->Descendant(pNSTreeNode)) &&
  //           	((*iter)->FrerePilote != pNSTreeNode))
  //       (*iter)->setLigne((*iter)->getLigne() + DecalageLigneFrere) ;
        // DecalageLigneFrere  : éventuellement un
            // frère et un fils de plus
  //   }
  // }
  if (bCreationFrere)
    AjouteFrereFictifApres(pNSTreeNode, ligneMax + 1, colonne) ;
}

//----------------------------------------------------------------------//	ajouter un un frère et un fils fictifs à un pNSTreeNode
// lors de l'ajout d'un fils on traite le cas où le père (pNSTreeNode) est
// ouvreur de boîte de dialogue, le cas où ce père a des fils dans le fil
// guide et le cas classique
//----------------------------------------------------------------------
void
NSTreeWindow::AjouteElementFictif(NSTreeNode* pNSTreeNode, TTreeNode* Noeud, bool CreerFilsChampEdit, bool bDateHeure)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

	int ligne   = pNSTreeNode->getLigne() ;
  int colonne = pNSTreeNode->getColonne() ;

  //************************************//
  //********    AJOUTER UN FILS ********//
  //************************************//

  // coordonnées du fils de pNSTreeNode le plus éloigné avant insertion
  //
  int ligneMaxInit = ligne ;

  // coordonnées du fils de pNSTreeNode le plus éloigné
  //
  int ligneMax   = ligne ;
  int colonneMax = colonne ;

  GetMaxCoordonnees(pNSTreeNode, &ligneMaxInit, &colonneMax) ;

  bool bCreationFils = true ;      //ajouter un fils si il n' y a pas de fils fictif
  int DecalageLigneFils = 0 ; //MAJ des lignes de tous les NSTReeNodes existants en cas de
   							      //création d'un nouveau fils pour pNSTreeNode

  if (false == pNSTreeNode->getVectSons()->empty())
  {
    iterNSTreeNode iter = pNSTreeNode->getVectSons()->begin() ;

   	//chercher un élément non instancié : fictif (non lié) et non proposé
   	for (; (pNSTreeNode->getVectSons()->end() != iter) && (bCreationFils); iter++)
   	{
      if (((*iter)->estFictifPur()) && (false == (*iter)->isProposed()))
      {
        bCreationFils = false ;
        DecalageLigneFils = 0 ;
      }
    }
  }
  //créer un fils fictif pour pNSTreeNode  if (bCreationFils)
    developper(pNSTreeNode, Noeud, &DecalageLigneFils, ligne, colonne, CreerFilsChampEdit, bDateHeure) ;

  GetMaxCoordonnees(pNSTreeNode, &ligneMax, &colonneMax) ;

    					//**************************************//
              //********    AJOUTER UN FRERE   *******//
              //**************************************//

	// Savoir si on peut créer un frère ou non
  //
  bool bCreationFrere = true ;

  // int DecalageLigneFrere = 1;
  iterNSTreeNode it = _NodeArray.begin() ;
  //chercher un frère (fictif non lié )à pNSTreeNode : même colonne et lignes différentes
  for (; (_NodeArray.end() != it) && (bCreationFrere); it++)
  {
    if (((*it)->getLigne()      == (pNSTreeNode->getLigne() + 1)) &&
        ((*it)->getColonne()    == pNSTreeNode->getColonne()) 	  &&
        ((*it)->getFatherNode() == pNSTreeNode->getFatherNode()) 	&&
        ((*it)->estFictifPur()))
    {
      bCreationFrere = false ;      //ne pas créer un frère
      // DecalageLigneFrere = 0 ; //pas de décalage
    }
	}
  //décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMaxInit
  // for (iterNSTreeNode iter = _NodeArray.begin(); _NodeArray.end() != iter ; iter++)
  // {
  //   if (((*iter)->getLigne() > ligneMaxInit) && (!(*iter)->Descendant(pNSTreeNode)) &&
  //          ((*iter)->FrerePilote != pNSTreeNode))
  //     (*iter)->setLigne((*iter)->getLigne() +  DecalageLigneFrere + DecalageLigneFils) ;
            // DecalageLigneFrere + DecalageLigneFils : éventuellement un
            //frère et  un fils de plus
	// }
  if (bCreationFrere)
    AjouteFrereFictifApres(pNSTreeNode, ligneMax + 1, colonne) ;
}

//*********************************************************************
//
//			FIN DE LA GESTION DE L'AJOUT DES FILS ET DES FRERES
//
//*********************************************************************

//-----------------------------------------------------------------------------// faire une recherche filguide pour mettre à jour les données du BBFilsItem
//qui pilote le noeud
//-----------------------------------------------------------------------------
bool
NSTreeWindow::RechercheFilGuide(NSTreeNode* pNSTreeNode, BBItemData* pDonnees, BBFilsItem* pBBfils)
{
	if (((NSTreeNode*) NULL == pNSTreeNode) || ((BBItemData*) NULL == pDonnees)
                                          || ((BBFilsItem*) NULL == pBBfils))
  	return false ;

try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

  //recherche sémantique
  VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
  string sCodeSens ;
  string sLabelNode = pNSTreeNode->getEtiquette() ;
  NSDico::donneCodeSens(&sLabelNode, &sCodeSens) ;
  VecteurSelonCritere.AjouteEtiquette(sCodeSens) ;

  string sLocalisation = string("") ;

  NSTreeNode* pFatherNode = pNSTreeNode->getFatherNode() ;
  if (pFatherNode)
  {
    BBFilsItem* pFatherFilsItem = pFatherNode->getBBFilsItem() ;
    if (pFatherFilsItem && pFatherFilsItem->getItemFather())
    {
      BBItem* pFatherItem = pFatherFilsItem->getItemFather() ;

  	  if (pFatherNode->getPosition() == string(""))
      {
    	  // Position dans l'arbre
        string sCodeSens ;
        string sNodeLabel = pFatherNode->getEtiquette() ;
        NSDico::donneCodeSens(&sNodeLabel, &sCodeSens) ;

        pFatherNode->SetPosition(pFatherItem->_sLocalisation +
      								            string(1,cheminSeparationMARK) + sCodeSens) ;
      }
      sLocalisation = pFatherNode->getPosition() ;
    }
  }
  else
  {
    BBFilsItem* pFilsItem = pNSTreeNode->getBBFilsItem() ;
    if (pFilsItem && pFilsItem->getItemFather())
    sLocalisation = pFilsItem->getItemFather()->_sLocalisation ;
  }

  pSuper->getFilGuide()->chercheChemin(&sLocalisation ,
                             &VecteurSelonCritere, NSFilGuide :: compReseau) ;

  bool trouve;
  VecteurSelonCritere.SetData(sCodeSens, &trouve, pDonnees) ;
  string sEtiquette ;
  if (trouve)
  {
  	//on a trouvé la bonne fiche
    *(pBBfils->getItemData()) = *pDonnees ;

    //si feuille
    size_t pos = string(pBBfils->getItemData()->fils).find_first_not_of(string(" ")) ;
    if (pos)
    	pBBfils->FilsProlongeable = false ;
    else
    	pBBfils->FilsProlongeable = true ;
  }

  return trouve ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::RechercheFilGuide", standardError, 0) ;
	return false ;
}
}

//------------------------------------------------------------------------------
//lors de l'ajout d'un fils à pNSTreeNode, on regardre :
//		  	+ si celui ci a une fiche fil guide :
//					-	s'il est lanceur d'une boîte de dialogue, on développe cette
//						boîte et on affiche le contenu
//       		- sinon on affiche simplement les fils
//			+ sinon on crée un fils classique et unique
//on met à jour le nombre de fils crées
//------------------------------------------------------------------------------
void
NSTreeWindow::developper(NSTreeNode* pNSTreeNode , TTreeNode* Noeud, int* DecalageLigneFils,
							int ligne, int colonne, bool CreerFilsChampEdit, bool bDateHeure,
                            bool bRechercheFilGuide, bool bInit)
{
try
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

  //
  // Si pNSTreeNode a dejà des fils proposés ou c'est un texte libre,
  // il ne faut pas en créer d'autres mais un fils fictif non proposé
  //
  iterNSTreeNode iter;
  if (false == pNSTreeNode->getVectSons()->empty())
  {
    iter = pNSTreeNode->getVectSons()->begin() ;
    for ( ; (pNSTreeNode->getVectSons()->end() != iter) &&
                                   (false == (*iter)->isProposed()) ; iter++) ;
  }
  //
  // Pas de fils proposés, et pas texte libre
  //
  if (((pNSTreeNode->getVectSons()->empty()) ||
           (pNSTreeNode->getVectSons()->end() == iter))
                            && (pNSTreeNode->getEtiquette() != "£?????"))
  {
    BBFilsItem* pBBfils = pNSTreeNode->getBBFilsItem() ;
    if ((BBFilsItem*) NULL == pBBfils)
      return ;

    BBItemData* pDonnees = new BBItemData ;
    // bool bControleNotification = false; //le noeud a été notifié
    //
		//Recherche sémantique
    //
    bool elementFilGuide = false ; //élément se trouvant dans fil guide
    bool trouve ;
    //
    // Tester si ce BBFilsItem a dejà des données ou non
    //
    size_t pos = string(pBBfils->getItemData()->chemin).find_first_not_of(string(" ")) ;
    if (0 == pos)
    {
      trouve = true ;
      *pDonnees = *(pBBfils->getItemData()) ;
    }
    else
    {
      if (false == bRechercheFilGuide)
        trouve = RechercheFilGuide(pNSTreeNode, pDonnees, pBBfils) ;
      else
        trouve = pBBfils->FilsProlongeable ;
    }
    if (trouve)
    {
      //existence de Boîte de dialogue
      // test devenu inutile et générant un bug avec les archetypes
      // en principe si ouvreDialog() est vrai et que ce n'est pas un archétype,
      // il doit exister un pDonnees->nomDialogue
      // size_t pos1 = string(pBBfils->pDonnees->nomDialogue).find_first_not_of(string(" "));      if (pBBfils->ouvreDialog())
      {
        pBBfils->FilsProlongeable = true ;

        // Si le premier élément de VectFils de pBBfils (pBBfils
        // en tant que BBItem) contient un élément dont les données
        // sont vides alors enlever cet élément de ce vecteur
        //
        // TODO : Attention, cet algo supprime des données comme le code CISP
        //        ---------- à repenser ---------
        //
        if (false == pBBfils->VectorFils.empty())
        {
          BBiterFils iter = pBBfils->VectorFils.begin() ;
          while (pBBfils->VectorFils.end() != iter)          {
            size_t pos = (*iter)->getItemSemanticPath().find_first_not_of(string(" ")) ;
            if (pos)
              pBBfils->VectorFils.erase(iter) ;
            else
              iter++ ;
          }
        }
        //
        // Ce BBFilsItem est créateur de boîte de dialogue,
        // si sa patpatho est vide il faudra la remplir (si elle a lieu)
        // depuis la patpatho globale, pour pouvoir la dispatcher à
        // l'ouverture de la boîte de dialogue
        //
        if (pBBfils->PatPtahovide())
        {
          //
          // patpatho globale
          //
          NSVectFatheredPatPathoArray* pPatpatho = _pControle->getTransfert()->getFilsItem()->getPatPatho() ;
          if (false == pPatpatho->empty())
          {
            NSPatPathoArray* pContenuPatpatho = (*(pPatpatho->begin()))->getPatPatho() ;
            if (pContenuPatpatho && (false == pContenuPatpatho->empty()))
            {
              //
              // localiser la position de pNSTreeNode dans la patpatho globale
              //
              PatPathoIter iter = pContenuPatpatho->begin() ;
              bool continuer = true ;
              while ((pContenuPatpatho->end() != iter) && (continuer))
              {
                if (((*iter)->getLexique() == pNSTreeNode->getEtiquette())
                                    && (((*iter)->getLigne() + 1) == ligne))
                  continuer = false ;
                else
                  iter++ ;
              }

              if (pContenuPatpatho->end() != iter)
              {
                NSVectFatheredPatPathoArray* pNSVectPatPathoArray = pBBfils->getPatPatho() ;
                pNSVectPatPathoArray->vider() ;
                NSPatPathoArray* pPPt = pBBfils->getPatpathoActive(pContenuPatpatho, iter) ;
                if (pPPt)
                  delete pPPt ;
              }
            }
          }
        }

        //
        // Tuer le champ NSEditDico et fermer le lexique
        //
        if (_pEditDico)
        {
          string sMsg = string("TreeWindow ") + getHandleAsString() + string(" developper : deleting EditDico ") + getEditDicoHandleAsString() ;
          pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

          KillDico() ;
        }

        if (pNSTreeNode->getTransfert())
          pNSTreeNode->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED,
	            			pNSTreeNode->getTransfertMessage()) ;

        NSVectFatheredPatPathoArray* pPatpatho = pBBfils->getPatPatho() ;
        if (false == pPatpatho->empty())
        {
          //tuer les fils correspondant à l'ancienne patpatho

          int NbNoeudSupprime = 1 ;
          bool PatpthoNonVide = false ;
          SupprimerFils(pNSTreeNode, &NbNoeudSupprime, &PatpthoNonVide) ;

          // 2010-05-03
          // if (PatpthoNonVide)
          // {
            // MAJ lignes et colonnes
          //  _NodeArray.ShiftLines(ligne + 1, -NbNoeudSupprime) ;
          // }

          NSPatPathoArray* ContenuPatpatho = (*(pPatpatho->begin()))->getPatPatho() ;
          //dispatcher la patpatho

          DispatcherPatPatho(ContenuPatpatho, pNSTreeNode, DecalageLigneFils, "") ;
					pNSTreeNode->SetImageIndex(0) ;
          pNSTreeNode->SetSelectedImageIndex(0, true) ;
          elementFilGuide = true ;

          SetDirty(true) ;        }      }
      //
      // Element non créateur de boite de dialogue
      //
      else
      {
        //
        // Affichage de fils proposés
        //
        if (pBBfils->FilsProlongeable)
        {                                      //
          (*DecalageLigneFils) = 0 ;
          bool bAutoriserFilsPropose = true ; //autoriser la création de
                                                     //fils proposés ou non
                                                    //
          //
          // Vérifier qu'aucun ancêtre de pNSTreeNode ne fait partie
          // de ses fils, sinon créer un fils classique.
          // (Exemple: antécédent chrirgicaux)
          //
          NSTreeNode* Pere = pNSTreeNode->getFatherNode() ;
          bool continuer = true ;
          while (Pere && continuer)
          {
            if (string(pDonnees->fils).find(Pere->getEtiquette()) != NPOS)
            {
              bAutoriserFilsPropose = false ;
              continuer = false ;
            }
            else
              Pere = Pere->getFatherNode() ;
          }
          if (bAutoriserFilsPropose)
          {
            //if (pBBfils->pDonnees->ouvreDialogue[0] == 'O')

            //extraire les étiquette des fils
            VecteurChaine Vect ;
            DecomposeChaineTL(&Vect, string(pDonnees->fils)) ;
            //inverser ce tableau!!!!!!!!
            reverse(Vect.begin(), Vect.end()) ;
            // IterString fils = pVect->begin() ;
            int cardinal = 0 ;

            //
            // Nombre de fils fictifs qui vont être affichés
            //
            // for (; pVect->end() != fils ; fils++)
            //   if (false == pNSTreeNode->VectFils.Contains(*(*fils)))
            //     cardinal++ ;
            //
            // Décaler les NSTreeNodes qui existent et dont la
            // ligne est supérieure à ligne
            //
            // _NodeArray.ShiftLines(ligne + 1, cardinal) ;

            IterString fils = Vect.begin() ;
            for (; Vect.end() != fils ; fils++)
            {
              //
              // MAJ des coordonnées des fils et des petits fils
              // de pNSTreeNode
              //pNSTreeNode->MettreAJourLigne(DecalageLigneFils);
              // Verifier si cet élément est l'étiquette d'un fils
              // existant de pNSTreeNode
              //
              if (false == pNSTreeNode->getVectSons()->Contains(*(*fils)))
              {
                AjouteFilsFictif(pNSTreeNode, Noeud, ligne + cardinal + 1, colonne + 1,
			            				*(*fils), true, CreerFilsChampEdit, bDateHeure) ;
                // cardinal-- ;
                cardinal++ ;
                elementFilGuide = true ;
              }
            }
            // Attention : suite à l'astuce hafedhienne d'inverser
            // le tableau de fils du fil guide, il est nécessaire
            // d'inverser le tableau de fils du NSTreeNode
            reverse(pNSTreeNode->getVectSons()->begin(), pNSTreeNode->getVectSons()->end()) ;
            int i = 0 ;
            for (iterNSTreeNode itNode = pNSTreeNode->getVectSons()->begin() ; pNSTreeNode->getVectSons()->end() != itNode ; itNode++, i++)
              (*itNode)->setLigne(ligne + i + 1) ;
          }
          else //création de fils et de frère fictifs
          {
            AutoriserCreationFils(pNSTreeNode, Noeud) ;
            AutoriserCreationFrere(pNSTreeNode) ;
          }
          pBBfils->FilsProlongeable = false ;
          SetDirty(true) ;
        }
      }
    }
    else
      pBBfils->FilsProlongeable = false ;

    delete pDonnees ;
    //
    //création de fils et de frères classiques
    //
    if (!elementFilGuide)
    {
      *DecalageLigneFils = 0 ;
      bool bCreationFils = true ;
      if (pNSTreeNode && (false == pNSTreeNode->getVectSons()->empty()))
      {
        iterNSTreeNode iter = pNSTreeNode->getVectSons()->begin() ;
        // chercher un élément non instancié : fictif (non lié) et non proposé
        // et dont le code lexique ne commence pas par V (chiffrée)

        for (; (pNSTreeNode->getVectSons()->end() != iter) && (bCreationFils); iter++)
        {
          if ((((*iter)->estFictifPur()) && (false == (*iter)->isProposed())) ||
                        ((*iter)->isCreateurChampEdit()))
            bCreationFils = false ;
        }
      }
      if (bCreationFils && (false == bInit))
      {
        SetDirty(true) ;
        *DecalageLigneFils = 1 ;
        // MAJ des coordonnées des fils et des petits fils de pNSTreeNode
        pNSTreeNode->MettreAJourLigne(*DecalageLigneFils) ;
        AjouteFilsFictif(pNSTreeNode, Noeud, ligne + 1,
            		  colonne + 1, "", false, CreerFilsChampEdit, bDateHeure) ;
      }
    }
  }
  //
  // Il existe un/des fil(s) proposé(s), ou c'est un texte libre
  //
  else
  {
    *DecalageLigneFils = 0 ;
    bool bCreationFils = true ;
    if (pNSTreeNode && (false == pNSTreeNode->getVectSons()->empty()))
    {
      iterNSTreeNode iter = pNSTreeNode->getVectSons()->begin() ;
      // Chercher un élément non instancié : fictif (non lié) et non proposé
      // et dont le code lexique ne commence pas par V (chiffrée)
      for (; (pNSTreeNode->getVectSons()->end() != iter) && (bCreationFils); iter++)
      {
        if ((((*iter)->estFictifPur()) && (false == (*iter)->isProposed()))
                    || ((*iter)->isCreateurChampEdit()))
          bCreationFils = false ;
      }
    }
    if (bCreationFils)
    {
      SetDirty(true) ;
      //
      // Création classique de fils et de frère
      //
      *DecalageLigneFils = 1 ;
      // MAJ des coordonnées des fils et des petits fils de pNSTreeNode
      pNSTreeNode->MettreAJourLigne(*DecalageLigneFils) ;
      AjouteFilsFictif(pNSTreeNode, Noeud, ligne + 1,
                  colonne + 1, "", false, CreerFilsChampEdit, bDateHeure) ;
    }
  }
  pNSTreeNode->ExpandTousNoeud() ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::developper", standardError, 0) ;
}
}

//--------------------------------------------------------------------// Supprimer au niveau interface et structure de données les fils et petits
// fils de  pNSTreeNode
//--------------------------------------------------------------------
void
NSTreeWindow::SupprimerFils(NSTreeNode* pNSTreeNode, int* NbNoeudSupprime, bool* PatpthoNonVide)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) || (pNSTreeNode->getVectSons()->empty()))
    return ;

  iterNSTreeNode iterPath = pNSTreeNode->getVectSons()->begin() ;
  while (pNSTreeNode->getVectSons()->end() != iterPath)
  {
    NSTreeNode* pNSTemp = (*iterPath) ;
    (*iterPath)->Delete() ;
    DetruitFils(NbNoeudSupprime, (*iterPath)) ;

    // Enlever (*iter) de l'Array de son père
    pNSTreeNode->getVectSons()->RemoveElement(pNSTemp) ;

    *PatpthoNonVide = true ;
    _NodeArray.RemoveElement(pNSTemp) ;
    delete pNSTemp ;
  }
}

//----------------------------------------------------------------------
// Réponse à une touche du clavier
//----------------------------------------------------------------------
void
NSTreeWindow::EvKeyUp(uint key, uint /* repeatcount */, uint /* flags */){
  // DefaultProcessing() ;

  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;

  //
  // Ces fonctions sont dans le KeyUp et pas dans le KeyDown car, si on crée
  // un NSEditDico dans le KeyDown, c'est lui qui va recevoir le KeyUp
  //
		// touche Inser : créer éventuellement un frère (au dessus)    //                pour le noeud en question
    if (VK_INSERT == key)
    {
      if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
        return ;
      // Ne rien faire s'il s'agit d'un noued lié
      if (pNSTreeNodeSelection->estLie())
        return ;

      InsereAvant() ;
    }
    //
    // touche espace : valider un élément
    //
    else if ((VK_SPACE == key) || (VK_RETURN == key))
    {
      if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
        return ;
      // Ne rien faire s'il s'agit d'un noued lié
      if (pNSTreeNodeSelection->estLie())
        return ;
      DefaultProcessing() ;
      Valider() ;
    }
    //
    // touche suppr  : supprimer un noeud et ses descendants
    //
    else if (VK_DELETE == key)
      EvDelete() ;
    //
    // tab key  : set focus to next control
    //
    else if (VK_TAB == key)
    {
      if (_pControle && _pControle->getNSDialog())
        _pControle->getNSDialog()->ActiveControlSuivant(_pControle) ;
      else if (_pControle && _pControle->getMUEView())
      {
        if (GetKeyState(VK_SHIFT) < 0)
          _pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
        else
          _pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      }
    }
    else if (VK_ESCAPE == key)
    {
      return ;
    }
    else
    {
      // on bloque le clavier si on n'est pas en édition texte libre
      // pour éviter la sélection intempestive des items proposés
      // (ce qui faisait disparaitre les autres items et plantait Nautilus)
      if (bEditionDico)
        DefaultProcessing() ;
    }
}

//----------------------------------------------------------------------// Réponse à une touche du clavier
//----------------------------------------------------------------------
void
NSTreeWindow::EvTvKeyDown(TTwKeyDownNotify& nhmr){
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;

	// Alt + Touche
  if (GetKeyState(VK_MENU) < 0)
  {
    // Alt M : Modifier le noeud
    //
    if ('M' == nhmr.wVKey)
      Modifier() ;
    else if (VK_RETURN == nhmr.wVKey)
      InsereApres() ;
  }
  //
  // Ctrl + Touche
  //
  else if (GetKeyState(VK_CONTROL) < 0)
  {
    if (((NSTreeNode*) NULL == pNSTreeNodeSelection) ||
                               (false == isRealNode(pNSTreeNodeSelection)))
      return ;

    // Ctrl C : Copier
    //
    if      ('C' == nhmr.wVKey)
    {
      ModifierTexteLibre = true ;      CopierNode(pNSTreeNodeSelection) ;
    }
    // Ctrl V : Coller
    //
    else if ('V' == nhmr.wVKey)
    {
      ModifierTexteLibre = true ;      CollerNode(pNSTreeNodeSelection) ;
    }
    // Ctrl X : Couper
    //
    else if ('X' == nhmr.wVKey)
    {      ModifierTexteLibre = true ;
      CouperNode(pNSTreeNodeSelection) ;
    }
    // Sélection au clavier des items 0+,1+,2+,3+,4+
    //    else if ((nhmr.wVKey >= '0') && (nhmr.wVKey <= '4'))
      FixeInteret(nhmr.wVKey) ;
    else if ((nhmr.wVKey >= VK_NUMPAD0) && (nhmr.wVKey <= VK_NUMPAD4))
      FixeInteret(nhmr.wVKey) ;
    //
    // Ctrl N :
    // Sélection au clavier du switch positif/négatif pour la certitude (0/100)    //
    else if ('N' == nhmr.wVKey)
    {
      if (string("WCE001") == pNSTreeNodeSelection->getCertitude())
 				FixeCertitude(100) ;
      else
        FixeCertitude(0) ;
    }
    else if ('J' == nhmr.wVKey)
    {
      Classifier() ;    }
    else if ('L' == nhmr.wVKey)
    {
      NewPreoccup() ;    }
  }
  //
  // Touches ordinaires (ni Alt, ni Ctrl)
  //
  else
    DefaultProcessing() ;

/*
  {
    //
		// touche Inser : créer éventuellement un frère (au dessus)    //                pour le noeud en question
    if (VK_INSERT == nhmr.wVKey)
    {
      if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
        return ;
      // Ne rien faire s'il s'agit d'un noued lié
      if (pNSTreeNodeSelection->estLie())
        return ;

      InsereAvant() ;
    }
    //
    // touche espace : valider un élément
    //
    else if ((VK_SPACE == nhmr.wVKey) || (VK_RETURN == nhmr.wVKey))
    {
      if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
        return ;
      // Ne rien faire s'il s'agit d'un noued lié
      if (pNSTreeNodeSelection->estLie())
        return ;
      DefaultProcessing() ;
      Valider() ;
    }
    //
    // touche suppr  : supprimer un noeud et ses descendants
    //
    else if (VK_DELETE == nhmr.wVKey)
      EvDelete() ;
    //
    // tab key  : set focus to next control
    //
    else if (VK_TAB == nhmr.wVKey)
    {
      if (_pControle && _pControle->getNSDialog())
        _pControle->getNSDialog()->ActiveControlSuivant(_pControle) ;
      else if (_pControle && _pControle->getMUEView())
      {
        if (GetKeyState(VK_SHIFT) < 0)
          _pControle->getMUEView()->SetFocusToPreviousControl((TControl*)this) ;
        else
          _pControle->getMUEView()->SetFocusToNextControl((TControl*)this) ;
      }
    }
    else if (VK_ESCAPE == nhmr.wVKey)
    {
      return ;
    }
    else
    {
      // on bloque le clavier si on n'est pas en édition texte libre
      // pour éviter la sélection intempestive des items proposés
      // (ce qui faisait disparaitre les autres items et plantait Nautilus)
      if (bEditionDico)
        DefaultProcessing() ;
    }
  }
*/
}

//----------------------------------------------------------------------// 	créer éventuellement un frère	(au dessus) pour le noeud en question
//----------------------------------------------------------------------
void
NSTreeWindow::InsereAvant()
{
	TTreeNode 	Noeud = GetSelection() ;  		  // TTreeNode sélectionné
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&Noeud) ; // NSTreeNode correspondant
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;

  NSTreeNode* pNSTreePere = pNSTreeNodeSelection->getFatherNode() ; // père du NSTreeNode
  int ligne  	= pNSTreeNodeSelection->getLigne() ;       // correspondant
  int colonne = pNSTreeNodeSelection->getColonne() ;

  if (pNSTreePere) //si le père existe
  {
    bool bCreationGrandFrere = true ;

    // chercher si le père a un fils fictif qui se trouve avant pNSTreeNodeSelection
    //
    iterNSTreeNode iter = pNSTreePere->getVectSons()->begin() ;
    for (; (pNSTreePere->getVectSons()->end() != iter) && (bCreationGrandFrere) ; iter++)
      if (((*iter)->getLigne() < ligne) && ((*iter)->estFictifPur()))
          bCreationGrandFrere = false ;      //ne pas créer un frère au dessus

    if (bCreationGrandFrere)
    {
      NSTreeNode* pNSTreeFrere = TrouverGrandFrere(pNSTreeNodeSelection, ligne);
      // On a trouvé le frère ainé le plus proche de pNSTreeNodeSelection
      if (pNSTreeFrere)
      {
        // Décaler les NSTreeNodes qui existent et dont la ligne est
        // supérieure à ligneMax
        // _NodeArray.ShiftLines(pNSTreeFrere->getLigne() + 1, 1) ;
        AjouteFrereFictifApres(pNSTreeFrere, pNSTreeFrere->getLigne() + 1, pNSTreeFrere->getColonne());
      }
      else
      {
        // Décaler les NSTreeNodes qui existent
        // _NodeArray.ShiftLines(pNSTreePere->getLigne() + 1, 1) ;
        AjouteFilsFictif(pNSTreePere, pNSTreePere, pNSTreePere->getLigne() + 1 , pNSTreePere->getColonne() + 1, "", false, false, false) ;
      }
    }
  }
  else  // si le père n'existe pas
  {
    // Touver le TTReenode dont le père est le root et qui soit
    // le plus proche de l'élément sélectionné (ainé)
    NSTreeNode* pNSTreeFrere  = TrouverGrandFrereFilsRoot(pNSTreeNodeSelection, ligne) ;

    // On a trouvé le frère ainé le plus proche de pNSTreeNodeSelection
    if (pNSTreeFrere)
    {
      // Décaler les NSTreeNodes qui existent et dont la ligne
      // est supérieure à ligneMax
      // _NodeArray.ShiftLines(ligne, 1) ;
      AjouteFrereFictifApres(pNSTreeFrere, ligne ,colonne) ;
      
      NSTreeNode* pCreatedNode = _NodeArray.GetNode(ligne) ;
      if (pCreatedNode)
        pCreatedNode->SelectItem(TVGN_CARET) ;
    }
    else
    {
      // _NodeArray.ShiftLines(ligne, 1) ;
      TTreeNode root = GetRoot() ;
      AjouteFrereFictifAvant(pNSTreeNodeSelection, &root, ligne, colonne, "Debut") ;

      NSTreeNode* pCreatedNode = _NodeArray.GetNode(ligne) ;
      if (pCreatedNode)
        pCreatedNode->SelectItem(TVGN_CARET) ;
    }
  }
}

//----------------------------------------------------------------------
//	ajouter un un frère et un fils fictifs à un NSTreeNode
//----------------------------------------------------------------------
void
NSTreeWindow::InsereApres()
{
  TTreeNode 	Noeud = GetSelection() ;  						   //TTreeNode sélectionné
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&Noeud); //NSTreeNode correspondant
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;
  //
  // supprimer tous les noeuds fictifs autres que ceux qui viennent d'être ajoutés
  // par pNSTreeNodeSelection
  //
  int NbNoeudSupprime = 0 ; //nombre de noeuds supprimés
  int ligneSelectionnee = pNSTreeNodeSelection->getLigne() ; //ligne de l'élément sélectionné
  int ligneMax = 0 ; //ligne du dernier élément fictif supprimé

  if (false == _NodeArray.empty())
  {
    iterNSTreeNode iter = _NodeArray.begin() ;
    //tuer les noeuds fictifs purs se trouvant après l'élément selectionné
    while (_NodeArray.end() != iter)
    {
      if (((*iter)->estFictifPur()) && ((*iter)->getLigne() > (ligneSelectionnee + 2 ))
                && (false == (*iter)->isProposed()))
      {
        NSTreeNode* pTemp = *iter ;
        int ligne = (*iter)->getLigne() ;

        if (ligneMax <= ligne)
          ligneMax = ligne ;

        (*iter)->Delete() ;

        NbNoeudSupprime++ ;

        //enlever (*iter) de l'Array de son père
        if ((*iter)->getFatherNode())
          (*iter)->getFatherNode()->getVectSons()->RemoveElement(pTemp) ;

        _NodeArray.RemoveElement(pTemp) ;        delete pTemp ;      }
      else
        iter++ ;
    }
  }

  // 2010-05-03
  // if (false == _NodeArray.empty())
  //   _NodeArray.ShiftLines(ligneMax + 1, -NbNoeudSupprime) ;

  AjouteElementFictif(pNSTreeNodeSelection, &Noeud, false, false) ;
}

void
NSTreeWindow::InsertTlInEdit()
{
  if ((false == bEditionDico) || ((NSEditDico*) NULL == _pEditDico))
    return ;

  _pEditDico->CmEditPaste() ;
}

//------------------------------------------------------------------------
// Détruire les fils de pPere , cardinal contient le nombre de fils de pPere
//------------------------------------------------------------------------
void
NSTreeWindow::DetruitFils(int *cardinal, NSTreeNode* pPere)
{
	if (((NSTreeNode*) NULL == pPere) || ((int*) NULL == cardinal) ||
                                               pPere->getVectSons()->empty())
		return ;

  for (iterNSTreeNode iter = pPere->getVectSons()->begin();
  		                        pPere->getVectSons()->end() != iter ; iter++)
  {
    (*cardinal)++ ;
    //tuer les fils
    DetruitFils(cardinal, (*iter)) ;
    //enlever  pNSTreeNodeSuppr de pNodeArray
    _NodeArray.RemoveElement(*iter) ;
  }

  pPere->getVectSons()->vider() ;
}

//----------------------------------------------------------------------
// Détruire un item .
// On détruit tous les fils de cet item et on met à jour les coordonnées des NStreeNodes restants
//----------------------------------------------------------------------
void
NSTreeWindow::EvDelete()
{
try
{
  TTreeNode NoeudSuppr = GetSelection() ;  				   //TTreeNode sélectionné
  NSTreeNode* pNSTreeNodeSuppr = GetNSTreeNode(&NoeudSuppr) ; //NSTreeNode correspondant

  if ((NSTreeNode*) NULL == pNSTreeNodeSuppr)
  {
    NoeudSuppr.Delete() ;
    return ;
  }

  if (pNSTreeNodeSuppr->estLie())
    return ;

  NSTreeNode* pNSTreePere = pNSTreeNodeSuppr->getFatherNode() ; //père du NSTreeNode correspondant
  // int         ligne       = pNSTreeNodeSuppr->getLigne() ;

  if (pNSTreeNodeSuppr->isProposed())
  {
    //
    // créer un pControle "Bidon" pour pouvoir supprimer le noeud fictif
    //
    pNSTreeNodeSuppr->SetControl(new NSControle(pContexte, pBBitemNSTreeWindow, pNSTreeNodeSuppr->getEtiquette(), "")) ;
    NSControle* pNodeControl = pNSTreeNodeSuppr->getControl() ;
    pNodeControl->setControle(dynamic_cast<void*>(pNSTreeNodeSuppr)) ;
    pNodeControl->setType(pNSTreeNodeSuppr->donneType()) ;
    pNodeControl->setNSDialog(pBBitemNSTreeWindow->getDialog()) ;
    pNodeControl->setMUEView(pBBitemNSTreeWindow->getView()) ;
    pNSTreeNodeSuppr->Delete() ;
  }
  else
  {
    if (false == pNSTreeNodeSuppr->estFictif())
    {
      if (pNSTreeNodeSuppr->getTransfert())
      {
        //supprimer les fils fictifs avant de supprimer pNSTreeNodeSuppr
        TuerFilsFictif(pNSTreeNodeSuppr) ;
        Message Msg ;
        pNSTreeNodeSuppr->getTransfert()->ctrlNotification( BF_CHECKED, BF_DELETE, &Msg) ;
      }
      else
        pNSTreeNodeSuppr->Delete() ;
    }
  }

  int cardinal = 1 ; //nb fils et petits fils
  DetruitFils(&cardinal, pNSTreeNodeSuppr) ;
  //enlever  pNSTreeNodeSuppr (et ses frères liés ) de pNodeArray
  _NodeArray.RemoveElement(pNSTreeNodeSuppr) ;

  //enlever  pNSTreeNodeSuppr de l'Array de son père
  if (pNSTreePere)
    pNSTreePere->getVectSons()->RemoveElement(pNSTreeNodeSuppr) ;

  //frères liés
  if (false == pNSTreeNodeSuppr->getVectLinkedBros()->empty())
  {
    iterNSTreeNode iter = pNSTreeNodeSuppr->getVectLinkedBros()->begin() ;
    while (pNSTreeNodeSuppr->getVectLinkedBros()->end() != iter)
    {
      NSTreeNode* pNSTemp = (*iter) ;
      (*iter)->Delete() ;
      _NodeArray.RemoveElement((*iter)) ;
      if (pNSTreePere)
        pNSTreePere->getVectSons()->RemoveElement((*iter)) ;
      pNSTreeNodeSuppr->getVectLinkedBros()->RemoveElement((*iter)) ;
      delete pNSTemp ;
    }
  }
  //base de donnéés : MiseAJour !!!!!!!!!!!!!!!!!!!!!!
  if (false == pNSTreeNodeSuppr->getVectLinkedBros()->empty())
    cardinal += pNSTreeNodeSuppr->getVectLinkedBros()->size() ;

  delete pNSTreeNodeSuppr ;
  NoeudSuppr.Delete() ;

  //mettre à jour les coordonnées des autres NSTreeNodes
  // 2010-05-03
  // if (false == _NodeArray.empty())
  //   _NodeArray.ShiftLines(ligne + 1, -cardinal) ;

  if (_NodeArray.empty())
    EvSetFocus(0) ;

  SetDirty(true) ;
}
catch (...){
	erreur("Exception NSTreeWindow::EvDelete.", standardError, 0) ;
}
}

//---------------------------------------------------------------------//tuer récursivement les fils fictifs d'un NSTreeNode et mettre à jour les lignes des
//éléments qui suivent
//---------------------------------------------------------------------
void
NSTreeWindow::TuerFilsFictif(NSTreeNode* pNSTreeNodePere)
{
	if (((NSTreeNode*) NULL == pNSTreeNodePere) ||
                                    pNSTreeNodePere->getVectSons()->empty())
		return ;

  int ligneMax = 0 ; //ligne du dernier élément fictif supprimé
  iterNSTreeNode fils = pNSTreeNodePere->getVectSons()->begin() ;
  int NbNoeudSupprime = 0 ;
  while (pNSTreeNodePere->getVectSons()->end() != fils)
  {
    if ((*fils)->estFictifPur())
    {
      NSTreeNode* pTemp = *fils ;
      int ligne = (*fils)->getLigne() ;

      if (ligneMax <= ligne)
        ligneMax = ligne ;
      (*fils)->Delete() ;

      NbNoeudSupprime++ ;      //enlever (*iter) de l'Array de son père
      pNSTreeNodePere->getVectSons()->RemoveElement(pTemp) ;

      _NodeArray.RemoveElement(pTemp) ;
      delete pTemp ;
    }
    else
    {
      TuerFilsFictif(*fils) ;
      fils++ ;
    }
  }

  // 2010-05-03
  // if (NbNoeudSupprime && pNodeArray && (false == _NodeArray.empty()))
  //   _NodeArray.ShiftLines(ligneMax + 1, -NbNoeudSupprime) ;
}

//----------------------------------------------------------------------//----------------------------------------------------------------------
void
NSTreeWindow::EvSetFocus(HWND hWndLostFocus)
{
  _bLoosingFocus = false ;

  TWindow::EvSetFocus(hWndLostFocus);
  if (ReadOnly)
    return ;

  //si pas de NSTreeNode
  if (_NodeArray.empty())
    CreeNoeudFictif() ;

  // donner le focus au pEditDico
  if (_pEditDico)
  {
    bLanceEdit = true ;

    // pEditDico->pDico->pDicoDialog->setEdit(pEditDico, pEditDico->sTypeLexique) ;
    // pEditDico->SetPositionLexique() ;

    _pEditDico->SetFocus() ;
    _pEditDico->Invalidate() ;
  }
}

//----------------------------------------------------------------------
//crée premier noeud fictif
//----------------------------------------------------------------------
void
NSTreeWindow::Initialise(HWND hWndLostFocus)
{
  TWindow::EvSetFocus(hWndLostFocus) ;
  CreeNoeudFictif() ;
  if (_pEditDico)
  {
    bLanceEdit = true ;

    // pEditDico->pDico->pDicoDialog->setEdit(pEditDico, pEditDico->sTypeLexique) ;
    // pEditDico->SetPositionLexique() ;

    _pEditDico->SetFocus() ;
    _pEditDico->Invalidate() ;
  }
}

//----------------------------------------------------------------------
//crée premier noeud fictif
//----------------------------------------------------------------------
void
NSTreeWindow::CreeNoeudFictif()
{
  if (ReadOnly)
    return ;
try{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  //SetFocus();
  NSTreeNode* pNewNSTreeNode ;

  // Si pas de NSTreeNode
  if (_NodeArray.empty())
  {
    int rep ;
    //si données dans pSuper->pPatPathoCopie proposer de les afficher
    if (AfficheCopieCollee && (false == pSuper->pBufCopie->empty()))
    {
      AfficheCopieCollee = false ;
      rep = MessageBox("Voulez-vous coller les informations du Presse papier dans la nouvelle fiche?", "Message", MB_YESNO) ;
      AfficheCopieCollee = true ;
      if (IDYES == rep)
      {
        NSPatPathoArray CopiePatho(pContexte->getSuperviseur()) ;
        pSuper->pBufCopie->initFromPatPatho(&CopiePatho) ;

        DispatcherPatPatho(&CopiePatho, 0, 0, "", "EXPAND") ;
        pSuper->pBufCopie->vider() ;
        SetDirty(true) ;
        return ;
      }
    }

    if (AfficheCopieCollee)
    {
      TTreeNode  root    = GetRoot() ;
      TTreeNode* pAmorce = new TTreeNode(*this, TVI_FIRST) ;
      *pAmorce = root.AddChild(*pAmorce) ;

      pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
      pNewNSTreeNode->SetControl((NSControle*) 0) ;

      //MAJ ligne et colonne
      pNewNSTreeNode->setLigne(NODES_LIN_BASE) ;
      pNewNSTreeNode->setColonne(NODES_COL_BASE) ;
      pNewNSTreeNode->SetFatherNode((NSTreeNode*) 0) ;
      pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;

      _NodeArray.AddElement(pNewNSTreeNode) ;

      pNewNSTreeNode->SelectItem(TVGN_CARET) ;
      pNewNSTreeNode->EditLabel() ;
      delete pAmorce ;
    }
  }
}
catch (...)
{
	erreur("Exception NSTreeWindow::CreeNoeudFictif.", standardError, 0) ;
}
}

//----------------------------------------------------------------------// un noeud a été sélectionné : quand on applique la fonction
// SelectItem(TVGN_CARET) sur un noeud
//----------------------------------------------------------------------
void
NSTreeWindow::SelChanged()
{
  TWindow::Invalidate() ;
	if (ReadOnly)
    return ;

  if (_pEditDico)
  {
    string sMsg = string("SelChanged in TreeWindow ") + getHandleAsString() + string(" : deleting EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    KillDico() ;
  }

  //SetFocus();
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  //if((pNSTreeNode->sTexteGlobal != "") || (pNSTreeNode->FrerePilote))
  //	return;

  int NbNoeudSupprime = 0 ;                          //nombre de noeuds supprimés
  int ligneMax = 0 ;                                 //ligne du dernier élément fictif supprimé

  if (_NodeArray.empty())
    return ;

  iterNSTreeNode iter = _NodeArray.begin() ;  // Kill all "fictive" elements situated before selected element  //  // Beware that selected element's line number will decrease when elements are  // removed  //  while (_NodeArray.end() != iter)
  {
    if (((*iter)->estFictifPur()) && ((*iter)->getLigne() < pNSTreeNode->getLigne()))
    {
      NSTreeNode* pTemp = *iter ;
      int ligne = (*iter)->getLigne() ;

      if (ligneMax <= ligne)
        ligneMax = ligne ;

      // Remove from tree
      //
      (*iter)->Delete() ;

      NbNoeudSupprime++ ;

      //enlever (*iter) de l'Array de son père
      if ((*iter)->getFatherNode())
        (*iter)->getFatherNode()->getVectSons()->RemoveElement(pTemp) ;

      _NodeArray.RemoveElement(pTemp) ;
      delete pTemp ;
    }
    else
      iter++ ;
  }

  if (_NodeArray.empty())
    return ;

  // mettre à jour les coordonnées des autres NSTreeNodes
  // 2010-05-03
  // if (NbNoeudSupprime > 0)
  //   _NodeArray.ShiftLines(ligneMax + 1, -NbNoeudSupprime) ;

  if ((pNSTreeNode->estFictifPur()) && (false == pNSTreeNode->isProposed()) && !bIniting)
    pNSTreeNode->EditLabel() ;
}

//----------------------------------------------------------------------// édition
//----------------------------------------------------------------------
bool
NSTreeWindow::TvnBeginLabelEdit(TTwDispInfoNotify& /*nmHdr*/)
{
  if (ReadOnly)
    return true ;
try
{
  string sMsgHeader = string("TreeWindow ") + getHandleAsString() + string(" TvnBeginLabelEdit: ") ;
  string sMsg = sMsgHeader + string("Entering.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  NS_CLASSLIB::TRect rectEdit, rectangle ;
  //
  // tuer le champ NSEditDico et fermer le lexique
  //
  if (_pEditDico)
  {
    sMsg = sMsgHeader + string("deleting EditDico ") + getEditDicoHandleAsString()  ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    KillDico() ;
  }
  // char texte[1000];

  // GetSelection donne uniquement le HTTREEITEM
  TTreeNode noeud = GetSelection() ;
  pNSTreeNodeEnCours = GetNSTreeNode(&noeud) ;

  if ((NSTreeNode*) NULL == pNSTreeNodeEnCours)
  {
    bForceEdit = false ;
    return true ;
  }
  if (pNSTreeNodeEnCours->isProposed())
    return true ;
  if ((pNSTreeNodeEnCours->getEtiquette() != "") &&
      (pNSTreeNodeEnCours->getEtiquette() != "£?????") && (false == bForceEdit))
    return true ;

  if (pNSTreeNodeEnCours->estLie())  {
    pNSTreeNodeEnCours->getPilotNode()->SelectItem(TVGN_CARET) ;
    pNSTreeNodeEnCours->getPilotNode()->EditLabel() ;
    return true ;
  }

  NS_CLASSLIB::TRect rectBoiteMere = Parent->GetWindowRect();
  //
  // On récupère le rectangle dans lequel elle s'inscrit
  // ATTENTION : le repère est fixé sur le TreeView
  //
  pNSTreeNodeEnCours->GetItemRect(rectEdit, true) ;
  //
  // Recalcul du rectangle en prenant le repère de la fenêtre
  // dans la zone client de la fenetre mère.
  // Dans un dialogue, le controle est à X > 0 et Y > 0
  // Dans une fenetre (consultation) le controle est à X = 0 et Y = 0
  // rectEdit.left += Attr.X ;
  // rectEdit.top  += Attr.Y ;
  //
  // s'il s'agit d'un texte libre, donner au champ édit les dimensions
  // d'un edit multiligne
  //
  if (pNSTreeNodeEnCours->getGlobalText() != string("")) //texte libre
  {
    rectEdit.right  = 0 ; //rectBoiteMere.right    - 100;
    rectEdit.bottom = 0 ; //rectBoiteMere.bottom   - 100;
  }
  else
  {
    // rectEdit.right  += Attr.X + 200 ;     //87 42 191 58
    rectEdit.right += 200 ;
    // rectEdit.bottom += Attr.Y ;
  }
  // on garde en mémoire la zone occupée par le NSEditDico
  rectEditDico = rectEdit ;

  //
  // Activation du NSEditDico
  //
  // uint 		 length = 1000;
  string sChaine ;

  if (false == pNSTreeNodeEnCours->getVectLinkedBros()->empty())
    sChaine = pNSTreeNodeEnCours->getGlobalText() ;
  else
    sChaine = pNSTreeNodeEnCours->getLabelString() ;

  strip(sChaine, stripRight) ;

  if ((NSEditDico*) NULL == _pEditDico)
  {
    sMsg = sMsgHeader + string("Creating EditDico.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    // pEditDico = new NSEditDico(pContexte, Parent, TreeEditId++, pContexte->getSuperviseur()->getDico(), "","", rectEdit.left, rectEdit.top,
    //                            rectEdit.Width(),rectEdit.Height(), 0, true) ;
    _pEditDico = new NSEditDico(pContexte, this, TreeEditIdAsConst, pContexte->getDico(), "","", rectEdit.left, rectEdit.top,
                               rectEdit.Width(),rectEdit.Height(), 0, true) ;
    _pEditDico->setRect(rectEdit) ;
    _pEditDico->setGardeFocus(true) ;
    _pEditDico->Create() ;

    sMsg = sMsgHeader + string("EditDico ") + getEditDicoHandleAsString() + string(" created.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;
  }
  else
    _pEditDico->setGardeFocus(true) ;

  sMsg = sMsgHeader + string("Initiating EditDico.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  // message
  bLanceEdit = true ;
  EntreeEditionDico() ;

  _pEditDico->SetpTreeAEditer(this) ;
  _pEditDico->EveilleToi(&sChaine, &rectEdit) ;

  sMsg = sMsgHeader + string("Starting edit process.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  _pEditDico->SetFocus() ;
  // pEditDico->Invalidate() ;

  sMsg = sMsgHeader + string("Leaving.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  _pEditDico->setGardeFocus(false) ;

  bForceEdit = false ;
  return true ;
}
catch (...)
{	erreur("Exception NSTreeWindow::TvnBeginLabelEdit.", standardError, 0) ;
  return true ;
}
}

//----------------------------------------------------------------------
//développer ou non un TReeNode ( + ou -)
//pour chaque noeud , lui mettre à jour son affichage en tenant compte des
//étiquettes de ses fils et de son champ sVisible
//----------------------------------------------------------------------
void
NSTreeWindow::AfficheLibelle()
{
  if (true == _NodeArray.empty())
    return ;

  iterNSTreeNode iterNode = _NodeArray.begin() ;

  //on cache les noeuds non visibles
  for (; _NodeArray.end() != iterNode ; iterNode++)
  {
    string sLabel = (*iterNode)->getLabelString() ;
    if (((*iterNode)->getVisible() == "0") &&
        (false == (*iterNode)->getVectSons()->empty()))
    {
      iterNSTreeNode fils = (*iterNode)->getVectSons()->begin() ;
      //
      // si le fils est lui même père, mettre "..." devant son étiquette
      //
      string sSuite = string("") ;
      if (false == (*fils)->getVectSons()->empty())
        sSuite = "..." ;
      else
        sSuite = "";
      //
      // Il s'agit d'une valeur chiffrée
      //
      if ((*fils)->isCreateurChampEdit())
      {
        if (((*fils)->getLabelString().find("<") != NPOS) ||
                  ((*fils)->getLabelString().find(">") != NPOS))
          sLabel += " " + (*fils)->getLabelString() ;
        else
          sLabel += " : " + (*fils)->getLabelString() ;
        (*iterNode)->SetText(sLabel.c_str(), true) ;
      }
      //
      // Il s'agit d'une date ou d'une heure
      //
      else if ((*fils)->isDateTime())
      {
        sLabel += " : " + (*fils)->getLabelString() ;
        (*iterNode)->SetText(sLabel.c_str(), true) ;
      }
      else
      {
        sLabel += " (" ;
        while ((*iterNode)->getVectSons()->end() != fils)
        {
          if ((false == (*fils)->estLie()) && ((*fils)->getLabelString() != ""))
          {
            if (false == (*fils)->estLie())
            {
              string sLabelAffiche = "" ;
              if ((*fils)->getGlobalText() != string("")) //si texte libre
              {
                //premier " "
                size_t pos  = (*fils)->getLabelString().find(" ") ;
                sLabelAffiche = (*fils)->getLabelString() ;
                if (NPOS != pos)
                {
                  sLabelAffiche = string(((*fils)->getLabelString()), 0, pos) ;
                  sLabelAffiche = sLabelAffiche.c_str() + string("...") ;
                }
              }
              else
                sLabelAffiche = (*fils)->getLabelString() ;
              //
              // si le fils est lui même père, mettre "..."
              // devant son étiquette
              // sauf s'il s'agit de texte libre
              //              string sSuite = "" ;
              if ((false == (*fils)->getVectSons()->empty()) &&
                              (sLabelAffiche.find("...") == NPOS))
              {
                //si au moins un fils de (*fils) a un sLabel non vide
                bool continuer = true ;
                iterNSTreeNode petitFils = (*fils)->getVectSons()->begin() ;
                while (((*fils)->getVectSons()->end() != petitFils) && continuer)
                {
                  if ((*petitFils)->getLabelString() != string(""))
                  {
                    sSuite = "..." ;
                    continuer = false ;
                  }
                  else
                    petitFils++ ;
                }
              }
              else
                sSuite = "" ;

              if ((*fils)->getLabelString() != string(""))
              {
                if (*fils == (*iterNode)->getVectSons()->back()) //le dernier
                  sLabel += sLabelAffiche.c_str() + sSuite + string(")") ;
                else
                  sLabel += sLabelAffiche.c_str() + sSuite + string(", ") ;
              }
            }
          }
          fils++ ;
        }
      }
      (*iterNode)->CollapseTousNoeud() ;
      AfficheInteretMaximum((*iterNode)) ; //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
                          //parmi ceux de ses descendants
      if ((sLabel != " (") &&(sLabel != ((*iterNode)->getLabelString() + " ()")))
        (*iterNode)->SetText(sLabel.c_str(), true) ;
    }
    else
      (*iterNode)->ExpandTousNoeud() ;
  }
}

//----------------------------------------------------------------------
//développer ou non un TReeNode ( + ou -)
//----------------------------------------------------------------------
bool
NSTreeWindow::TvnItemExpanDing(TTwNotify& nmHdr)
{
try
{
	NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(nmHdr.itemNew.hItem) ; //NSTreeNode correspondant
	if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
		return false ;

	NS_CLASSLIB::TRect rectEdit ;
	//
	// Cacher NSEditDico
	//
	if (_pEditDico)
		_pEditDico->Show(SW_HIDE) ;

	string sLabelAnnexe = string("") ;

  /*
		TVE_COLLAPSE      Collapses the list.
    TVE_COLLAPSERESET Collapses the list and removes the child items.
                      Note that TVE_COLLAPSE must also be specified.
		TVE_EXPAND	      Expands the list.
		TVE_TOGGLE	      Collapses the list if it is currently expanded
                      or expands it if it is currently collapsed.
	*/
	//
	if (nmHdr.action == TVE_COLLAPSE) // on clique sur - : prendre les étiquettes
                                      // des fils et les coller
                                      // à celle de leur père
	{
    string sLabel = pNSTreeNodeSelection->getLabelString().c_str() ;
    if (false == pNSTreeNodeSelection->getVectSons()->empty())
    {
      pNSTreeNodeSelection->SetVisible("0") ;
      iterNSTreeNode fils = pNSTreeNodeSelection->getVectSons()->begin() ;
      (*fils)->SetVisible("0") ;

      string sChemin ;
      pNSTreeNodeSelection->formerChemin(&sChemin) ;
      NSCutPaste CP(pContexte->getSuperviseur()) ;
      pNSTreeNodeSelection->formerPatPatho(&CP) ;

      NSPatPathoArray CPPatho(pContexte->getSuperviseur()) ;
      CP.initFromPatPatho(&CPPatho) ;

      GlobalDkd Dcode(pContexte, sLang, sChemin, &CPPatho) ;
      Dcode.decode(pContexte->getDico()->dcTiret) ;
      sLabel = Dcode.getDcodeur() ;

      //
      //Il s'agit d'une valeur chiffrée
      //
      /*if ((*fils)->CreateurChampEdit)
      {
          if (((*fils)->sLabel.find("<") != NPOS) || ((*fils)->sLabel.find(">") != NPOS))
            sLabel += " " + (*fils)->sLabel;
          else
            sLabel += " : " + (*fils)->sLabel;
        pNSTreeNodeSelection->SetText(sLabel.c_str(), true);
          //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
          //parmi ceux de ses descendants
          AfficheInteretMaximum(pNSTreeNodeSelection);
      }
      //
      //Il s'agit d'une date ou d'une heure
      //
      else if((*fils)->DateHeure)
      {
        sLabel += " : " + (*fils)->sLabel;
        pNSTreeNodeSelection->SetText(sLabel.c_str(), true);
          //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
          //parmi ceux de ses descendants
          AfficheInteretMaximum(pNSTreeNodeSelection);
      }
      else
      {
          sLabel += " (";        //
          sLabelAnnexe = sLabel; // Utile pour savoir si les fils ont
                                 // des étiquettes ou non
        while (fils != pNSTreeNodeSelection->VectFils.end())
          {
              if ((!(*fils)->estLie()) && ((*fils)->sLabel != ""))
              {
                  string sLabelAffiche = "";
                  if ((*fils)->sTexteGlobal != "") //si texte libre
                  {
                      //premier " "
                      sLabelAffiche = (*fils)->sLabel;
                      size_t pos  = (*fils)->sLabel.find(" ");
                      if (pos != NPOS)
                      {
                          sLabelAffiche = string(((*fils)->sLabel), 0, pos);
                        sLabelAffiche = sLabelAffiche.c_str() + string("...");
                      }
                  }
                  else
                      sLabelAffiche = (*fils)->sLabel;
                  //
                  // si le fils est lui même père, mettre "..."
                  // devant son étiquette
                  string sSuite = "";
                  if ((!(*fils)->VectFils.empty()) && (sLabelAffiche.find("...") == NPOS))
                  {
                      // Si au moins un fils de (*fils) a un sLabel non vide
                      bool continuer = true;
                      iterNSTreeNode petitFils = (*fils)->VectFils.begin();
                      while (( petitFils != (*fils)->VectFils.end()) && continuer)
                      {
                          if ((*petitFils)->sLabel != "")
                          {
                        sSuite = "...";
                              continuer = false;
                          }
                          else
                            petitFils++;
                      }
                  }
                  else
                      sSuite = "";
                if ((*fils)->sLabel != "")
                  {
                if (*fils == pNSTreeNodeSelection->VectFils.back()) //le dernier
                      sLabel += sLabelAffiche.c_str() + sSuite + string(")");
                    else
                        sLabel += sLabelAffiche.c_str() + sSuite + string(", ");
                  }
              }
              fils++;
          }
          //
          // Vérifier si on a une parenthèse fermante
          //
          if (sLabel.find(")") == NPOS)
          {
            // Si "," est le dernier caractère
              if(sLabel[strlen(sLabel.c_str()) - 1] == ' ')
                  sLabel = string(sLabel, 0, strlen(sLabel.c_str()) - 2) + ")";              else
                  sLabel += ")";
          }
          if ((!(sLabelAnnexe == sLabel)) && (sLabel != (pNSTreeNodeSelection->sLabel + " ()")))
            pNSTreeNodeSelection->SetText(sLabel.c_str(), true);
          AfficheInteretMaximum(pNSTreeNodeSelection); //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
                      //parmi ceux de ses descendants
      } */
      if ((!(sLabelAnnexe == sLabel)) && (sLabel != (pNSTreeNodeSelection->getLabelString() + " ()")))
                pNSTreeNodeSelection->SetText(sLabel.c_str(), true) ;
        AfficheInteretMaximum(pNSTreeNodeSelection) ; //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
                          //parmi ceux de ses descendants
    }
  }
	else if (nmHdr.action == TVE_EXPAND) //	on clique sur + : afficher le label brut
	{
  	AfficheInteretReel(pNSTreeNodeSelection) ;
    pNSTreeNodeSelection->SetText(pNSTreeNodeSelection->getLabelString().c_str(), true) ;
    pNSTreeNodeSelection->SetVisible("1") ;
	}
	//
  // Nouvelles coordonnées de pNSTreeNodeEnCours
  //
  return false ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::TvnItemExpanDing.", standardError, 0);
	return false ;
}
}

//-------------------------------------------------------------------//avertir la treeview que le noeud a été contracté ou développé
//redimensionnement du champ édit
//-------------------------------------------------------------------
bool
NSTreeWindow::TvnItemExpanded(TTwNotify& /*nmHdr*/)
{
  if (_pEditDico)
    adaptEditToTreenode() ;

  return false ;
}

//------------------------------------------------------------------//redimensionnement du champ édit
//------------------------------------------------------------------
void
NSTreeWindow::EvVScroll(uint scrollCode, uint thumbPos, HWND hWndCtl)
{
  TWindow::EvVScroll(scrollCode, thumbPos, hWndCtl);
  if (ReadOnly)
    return ;

  if ((NSEditDico*) NULL == _pEditDico)
    return ;

  adaptEditToTreenode() ;

  bLanceEdit = true ;
  _pEditDico->SetFocus() ;
}

//------------------------------------------------------------------//redimensionnement du champ édit
//------------------------------------------------------------------
void
NSTreeWindow::EvHScroll(uint scrollCode, uint thumbPos, HWND hWndCtl)
{
  TWindow::EvHScroll(scrollCode, thumbPos, hWndCtl) ;
	if (ReadOnly)
    return ;

  if (_pEditDico)
    adaptEditToTreenode() ;
}

void
NSTreeWindow::adaptEditToTreenode()
{
  // _pEditDico->Show(SW_HIDE) ;
  // TWindow::EvVScroll(scrollCode, thumbPos, hWndCtl) ;

  _pEditDico->Show(SW_SHOWNORMAL) ;

  NS_CLASSLIB::TRect rectNoeud ;
  pNSTreeNodeEnCours->GetItemRect(rectNoeud, true) ;

  NS_CLASSLIB::TRect rectEdit = _pEditDico->GetWindowRect() ;
  int W = rectEdit.Width() ;
  int H = rectEdit.Height() ;

  rectEdit.left   = rectNoeud.left ;
  rectEdit.top    = rectNoeud.top ;
  rectEdit.right  = rectEdit.left + W ;
  rectEdit.bottom = rectEdit.top + H ;

  _pEditDico->MoveWindow(rectEdit, true) ;
}

//------------------------------------------------------------------// redimensionnement du champ édit
//------------------------------------------------------------------
void
NSTreeWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& rect)
{
  TWindow::Paint(dc, erase, rect) ;
  bPaint = true ;
}

//------------------------------------------------------------------
//redimensionnement du champ édit
//------------------------------------------------------------------
void
NSTreeWindow::EvPaint()
{
  //TWindow::EvPaint();
  DefaultProcessing() ;
  bPaint = true ;
}

//--------------------------------------------------------//changer la couleur du label du noeud en fonction de son interêt//--------------------------------------------------------
HBRUSH
NSTreeWindow::EvCtlColor(HDC hdc,HWND hwndchild,UINT ctltype)
{
	TTreeNode node = GetSelection() ;
	NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&node) ;
	if (pNSTreeNodeSelection)
	{
		string interet = pNSTreeNodeSelection->getInteret() ;
    SetBkColor(hdc,RGB(0,255,255)) ;
    if (interet == string("A"))
    	SetTextColor(hdc,RGB(255,0,255)) ;
    else if (interet == string("B"))
    	SetTextColor(hdc,RGB(255,0,255)) ;
    else if (interet == string("C"))
    	SetTextColor(hdc,RGB(255,0,255)) ;
    else if (interet == string("D"))
    	SetTextColor(hdc,RGB(0,255,255)) ;
    else
    	SetTextColor(hdc,RGB(0,0,255)) ;
	}
	return TWindow::EvCtlColor(hdc, hwndchild, ctltype) ;
}

//---------------------------------------------------------------------// savoir si on a à enregistrer ce document ou non
//---------------------------------------------------------------------
void
NSTreeWindow::SetDirty(bool bEtat)
{
  // Par convention, dirty n'est pas mis à true pendant l'initialisation
  //
  if (false == bIniting)
    bDirty = bEtat ;
}

bool
NSTreeWindow::GetDirty()
{
  return bDirty ;
}

//----------------------------------------------------------------------
// sélectionner le noeud suivant
//----------------------------------------------------------------------
void
NSTreeWindow::NStreeNodeSuivant(bool bThenSortieEditionDico)
{
  if (_pEditDico)
  {
    string sMsg = string("TreeWindow ") + getHandleAsString() + string(" NStreeNodeSuivant : deleting EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    KillDico() ;
  }

  SetFocus() ;

  //TTreeNode suivant valide
  TTreeNode node = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&node) ;

  if (pNSTreeNodeSelection)
  {
    TTreeNode noeud = pNSTreeNodeSelection->GetNextVisible() ;
    if (noeud)
    {
      NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
      if (pNSTreeNode)
        pNSTreeNode->SelectItem(TVGN_CARET) ;
    }
  }

  if (bThenSortieEditionDico)
    SortieEditionDico() ;
}

//----------------------------------------------------------------------
// sélectionner le noeud précédent
//----------------------------------------------------------------------
void
NSTreeWindow::NStreeNodePrcedent()
{
  if (_pEditDico)
  {
    string sMsg = string("TreeWindow ") + getHandleAsString() + string(" NStreeNodePrcedent : deleting EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    KillDico() ;
  }

  SetFocus() ;

  //TTreeNode suivant valide
  TTreeNode node = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&node) ;
  if (pNSTreeNodeSelection)
  {
    TTreeNode noeud = pNSTreeNodeSelection->GetPrevVisible() ;
    if (noeud)
    {
      NSTreeNode* pNSTreeNode = GetNSTreeNode(&noeud) ;
      if (pNSTreeNode)
        pNSTreeNode->SelectItem(TVGN_CARET) ;
    }
  }
}

//----------------------------------------------------------------------//modifier un NSTreeNode
//----------------------------------------------------------------------
void
NSTreeWindow::Modifier()
{
  bForceEdit = true ;
  TTreeNode noeud = GetSelection() ;
  NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
    return ;

  ModifierTexteLibre = true ;
  if (pNSTreeNodeSelection->estLie())
  {
    pNSTreeNodeSelection->getPilotNode()->SelectItem(TVGN_CARET) ;
    pNSTreeNodeSelection->getPilotNode()->EditLabel() ;
  }
  else
    pNSTreeNodeSelection->EditLabel() ;
}

//----------------------------------------------------------------------//----------------------------------------------------------------------
void
NSTreeWindow::EvKillFocus(HWND hWndGetFocus)
{
  TWindow::EvKillFocus(hWndGetFocus) ;

  //
	//	Quand un contrôle est activé il reçoit :
	//								WM_SETFOCUS
	//								WM_KILLFOCUS
	//                      WM_PAINT
	//								WM_SETFOCUS
  // il faut ignorer les killFocus tant que ce contrôle n'a pas reçu WM_PAINT
  //
  if (false == bPaint)
    return ;
  bPaint = false ;

  // When the EditDico is created, a killFocus is received; we just check that
  // it is done (and the next killFocus is a genuine one)
  //
  if (bLanceEdit)
  {
    bLanceEdit = false ;
    return ;
  }

  if (_pEditDico)
  {
    string sMsgLeft = string("TreeWindow ") + getHandleAsString() + string(" EvKillFocus") ;

    // Means that the dicoDialog is being created
    //
    if (NULL == _pEditDico->getDico()->getDicoDialog())
    {
      string sMsg = sMsgLeft + string(" doing nothing because ") + getEditDicoHandleAsString() + string(" has no DicoDialog.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return ;
    }

    if ((hWndGetFocus == _pEditDico->HWindow) || IsChild(hWndGetFocus))
    {
      string sMsg = sMsgLeft + string(" doing nothing because ") + getEditDicoHandleAsString() + string(" got the focus.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
      return ;
    }

    if ((_pEditDico->getDico()->getDicoDialog()) &&
        (_pEditDico->getDico()->getDicoDialog()->getEdit() == _pEditDico))
    {
      if (_pEditDico->getDico()->getDicoDialog()->doesBelong(hWndGetFocus))
      {
        string sMsg = sMsgLeft + string(" doing nothing because DicoDialog from ") + getEditDicoHandleAsString() + string(" got the focus.") ;
        pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
        return ;
      }
    }

    string sMsg = sMsgLeft + string(" managing content of EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    _pEditDico->TextLibre() ;

    sMsg = sMsgLeft + string(" deleting EditDico ") + getEditDicoHandleAsString() ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

    KillDico() ;
  }

  //tuer le noeud fictif s'il est le seul dans la treeview
    /*if(_NodeArray.size() == 1)
    {
   	    NSTreeNode* pNStreeNode = *(_NodeArray.begin());
        if(pNStreeNode->estFictifPur())
        {
            pNStreeNode->Delete();
            _NodeArray.EnleverElement(pNStreeNode);
            delete pNStreeNode;
            if(pEditDico)
   		    {
   			    delete pEditDico;
      		    pEditDico = 0;
   		    }
        }
    }*/
}

TResult
NSTreeWindow::DicoLostFocus(TParam1 param1, TParam2)
{
  // Seems it works better when doing nothing here
  //
  return 0 ;


  // If we are no longer in edition mode, we have to leave immediately since
  // the EditDico is to be deleted any moment and we risk encountering
  // a null pointer during functions execution
  //
  // if (false == bEditionDico)
  //  return ;

  HWND hSenderDico = (HWND) param1 ;
  char szDico[20] ;
  sprintf(szDico, "%p", hSenderDico) ;

  string sMsgHeader = string("TreeWindow ") + getHandleAsString() + string(" DicoLostFocus from ") + string(szDico) + string(" ") ;
  string sMsg = sMsgHeader + string("Entering.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  _bLoosingFocus = true ;

  if ((NSEditDico*) NULL == _pEditDico)
    return 0 ;

  SortieEditionDico() ;

  if ((NSEditDico*) NULL == _pEditDico)
    return 0 ;

  _pEditDico->TextLibre() ;

  if ((NSEditDico*) NULL == _pEditDico)
    return 0 ;

  sMsg = sMsgHeader + string("Deleting EditDico ") + getEditDicoHandleAsString() ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  KillDico() ;

  sMsg = sMsgHeader + string("Leaving.") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  return 0 ;
}

void
NSTreeWindow::KillDico()
{
  string sEditDicoHandle = getEditDicoHandleAsString() ;

  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" KillDico: deleting EditDico ") + sEditDicoHandle ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  // Destroy() will issue a killfocus; we have to prevent the EditDico
  // to call a DicoLostFocus... in order to do that, we switch off the
  // "editing" flag
  //
  SortieEditionDico() ;

  if ((NSEditDico*) NULL == _pEditDico)
    return ;

  sMsg = string("TreeWindow ") + getHandleAsString() + string(" KillDico: calling Destroy on EditDico ") + sEditDicoHandle ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  _pEditDico->setBeingDestroyed(true) ;

  _pEditDico->Destroy() ;

  sMsg = string("TreeWindow ") + getHandleAsString() + string(" KillDico: deleting EditDico ") + sEditDicoHandle ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  delete _pEditDico ;
  _pEditDico = (NSEditDico*) 0 ;
}

//----------------------------------------------------------------------
// On utilise EvMouseMove pour pouvoir passer le curseur en mode texte
// lorsqu'il passe sur le champ edit en cours (pEditDico)
// Note : on est obligés de le faire au niveau de la TreeWindow car
// la classe NSEditDico n'intercepte pas MouseMove et ne peut changer
// le curseur (on se demande bien pourquoi)
//----------------------------------------------------------------------
void
NSTreeWindow::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  // On prend en compte le décalage de la TreeWindow par rapport à sa mère
  // X et Y sont > 0 dans le cas d'une boite de dialogue mère
  // rectEditDico est exprimé en coordonnées client de la fenetre mère
  NS_CLASSLIB::TSize origin(Attr.X, Attr.Y) ;
  point += origin ;

  // si on est en édition dans le pEditDico en cours
  if (bEditionDico)
  {
    // si le pointeur est dans la zone Edit
    if (rectEditDico.Contains(point))
    {
      if (false == bCurseurTexte)
      {
        // On passe le curseur en mode texte
        SetCursor(0, IDC_IBEAM) ;
        bCurseurTexte = true ;
      }
    }
    else
    {
      if (bCurseurTexte)
      {
        // On passe le curseur en mode normal
        SetCursor(0, IDC_ARROW) ;
        bCurseurTexte = false ;
      }
    }
  }

  TTreeWindow::EvMouseMove(modKeys, point) ;
}

//----------------------------------------------------------------------
//on tue un noeud fictif pur s'il est unique et le pEDitDico perd le focus
//----------------------------------------------------------------------
void
NSTreeWindow::TuerNoeudFictif()
{
  //tuer le noeud fictif s'il est le seul dans la treeview
  if (_NodeArray.size() == 1)
  {
    NSTreeNode* pNStreeNode = *(_NodeArray.begin()) ;
    if (pNStreeNode->estFictifPur())
    {
      pNStreeNode->Delete() ;
      _NodeArray.RemoveElement(pNStreeNode) ;
      delete pNStreeNode ;
    }
  }
}

//-----------------------------------------------------------------------
// After recording, dispatch attributed Id into command objects
//-----------------------------------------------------------------------
void
NSTreeWindow::UpdateCommandInformation(NSPatPathoInfo* pPptInfo, NSTreeNode* pNSTreeNode)
{
  if ((NSPatPathoInfo*) NULL == pPptInfo)
    return ;

  // First, set NSTreeNode's node information
  //
  if (pNSTreeNode)
    pNSTreeNode->SetNoeud(pPptInfo->getNodeID()) ;

  // Then set BBItem's information
  //
  NSControle*	pCtrl = (NSControle*) 0 ;
  if (pNSTreeNode)
    pCtrl = pNSTreeNode->getControl() ;
  else
    pCtrl = _pControle ;

  if ((NSControle*) NULL == pCtrl)
    return ;

  NSTransferInfo *pTransfertInfo = pCtrl->getTransfert() ;
  if ((NSTransferInfo*) NULL == pTransfertInfo)
    return ;

  // Update NSTransferInfo's NSFatheredPatPathoArray
  //
  NSVectFatheredPatPathoArray* pVect = pTransfertInfo->getPatPatho() ;
  if (pVect)
  {
    if (false == pVect->empty())
    {
      NSFatheredPatPathoArray* pFatherPpt = *(pVect->begin()) ;
      if (pFatherPpt)
        pFatherPpt->referenceFatherNode(pPptInfo) ;
    }
    // If we don't do that, then leaves that are extended will lose their Id
    //
    else
    {
      NSFatheredPatPathoArray *pFatherPpt = new NSFatheredPatPathoArray(pContexte->getSuperviseur(), new NSPatPathoInfo(*pPptInfo), new NSPatPathoArray(pContexte->getSuperviseur())) ;
      pVect->push_back(pFatherPpt) ;
    }
  }

  // Update NSTransferInfo's Message
  //
  Message* pTranfertMsg = pTransfertInfo->getTransfertMessage() ;
  if (pTranfertMsg)
  {
    pTranfertMsg->SetTreeID(pPptInfo->getDoc()) ;
    pTranfertMsg->SetNoeud(pPptInfo->getNodeID()) ;
  }
}

//-----------------------------------------------------------------------
// click droit de la souris sur un noeud
//-----------------------------------------------------------------------
void
NSTreeWindow::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
try
{
  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" EvRButtonDown with EditDico ") + getEditDicoHandleAsString() ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	//
	// Si on est en cours d'édition, et que le texte n'est pas vide, on ne
	// fait rien
	//
	if (bEditionDico && _pEditDico)
	{    NS_CLASSLIB::TRect rectEdit = _pEditDico->GetWindowRect() ;    NS_CLASSLIB::TRect rectThis = GetWindowRect();    int iGlobalX = point.X() + rectThis.Left() ;    int iGlobalY = point.Y() + rectThis.Top() ;    // Point inside the Edit    //    if ((iGlobalX > rectEdit.Left()) && (iGlobalX < rectEdit.Right()) &&        (iGlobalY > rectEdit.Top())  && (iGlobalY < rectEdit.Bottom()))    {      NS_CLASSLIB::TPoint lp = point ;
      NSSuper* pSuper = pContexte->getSuperviseur() ;
      string sPaste = pSuper->getText("treeViewManagement", "paste") ;

      OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;
      menu->AppendMenu(MF_STRING, IDC_PASTE_TL, sPaste.c_str()) ;
      ClientToScreen(lp) ;
      menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
      delete menu ;      return ;    }  	// on teste dans ce cas si le texte est vide
    // (cas où on ne doit pas empecher le click)
    //
    string sTexte = DicoGetText() ;
    if (string("") != sTexte)
    	return ;

    SortieEditionDico() ;
	}

	NSSuper* pSuper = pContexte->getSuperviseur() ;
	NS_CLASSLIB::TPoint lp = point ;
	pHitTestInfo->pt = lp ;

	NSTreeNode* pNSTreeNodeSelection = 0;
	pNSTreeNodeSelection = GetNSTreeNode(HitTest(pHitTestInfo));
	if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
		return ;

	TPopupMenu *menu = new TPopupMenu() ;

	pNSTreeNodeSelection->SelectItem(TVGN_CARET) ;

	string sRights = pSuper->getText("treeViewManagement", "rightsManagement") ;
	string sInserB = pSuper->getText("treeViewManagement", "insertBefore") ;
	string sInserA = pSuper->getText("treeViewManagement", "insertAfter") ;
	string sModif  = pSuper->getText("treeViewManagement", "modifyNode") ;
	string sAbsent = pSuper->getText("treeViewManagement", "absenceOf") ;
	string sPresen = pSuper->getText("treeViewManagement", "presenceOf") ;
  string sParams = pSuper->getText("treeViewManagement", "parameters") ;
	string sGuides = pSuper->getText("treeViewManagement", "filGuide") ;
	string sEncycl = pSuper->getText("treeViewManagement", "encyclopedia") ;
	string sDel    = pSuper->getText("treeViewManagement", "delete") ;
	string sCut    = pSuper->getText("treeViewManagement", "cut") ;
	string sCopy   = pSuper->getText("treeViewManagement", "copy") ;
  string sPaste  = pSuper->getText("treeViewManagement", "paste") ;
  string sCreHC  = pSuper->getText("treeViewManagement", "createHealtConcern") ;
  string sClassi = pSuper->getText("treeViewManagement", "classify") ;

	if (false == ReadOnly)
	{
  	menu->AppendMenu(MF_STRING, IDC_EDITRIGHTS, sRights.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	}
	else
	{
  	menu->AppendMenu(MF_GRAYED, 0, sRights.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	}

	if (pNSTreeNodeSelection->estLie()) //NSTreeNode représentant la suite d'un texte libre
	{
  	menu->AppendMenu(MF_GRAYED, 0, sInserB.c_str()) ;
    menu->AppendMenu(MF_GRAYED, 0, sInserA.c_str()) ;
    if (pNSTreeNodeSelection->getEtiquette() == "")
    	menu->AppendMenu(MF_GRAYED, 0, sModif.c_str()) ;
    else
    	menu->AppendMenu(MF_STRING, IDC_MODIFIER, sModif.c_str()) ;

    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_GRAYED, 0, sAbsent.c_str()) ;
    menu->AppendMenu(MF_GRAYED, 0, sParams.c_str()) ;
    menu->AppendMenu(MF_GRAYED, 0, sGuides.c_str()) ;
    menu->AppendMenu(MF_GRAYED, 0, sEncycl.c_str()) ;

    menu->AppendMenu(MF_SEPARATOR, 0, 0);
    menu->AppendMenu(MF_GRAYED, 0, sDel.c_str()) ;
	}
	else
	{
  	// Elément fictif ou texte libre non proposé
    //
    if (pNSTreeNodeSelection->estFictif() &&
            (false == pNSTreeNodeSelection->isProposed()))
    {
    	menu->AppendMenu(MF_GRAYED, 0, sInserB.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sInserA.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sModif.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0);
      menu->AppendMenu(MF_GRAYED, 0, sAbsent.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sParams.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sGuides.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sEncycl.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_GRAYED, 0, sDel.c_str()) ;
    }
    else if (pNSTreeNodeSelection->isProposed())
    {
    	menu->AppendMenu(MF_GRAYED, 0, sInserB.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sInserA.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sModif.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_GRAYED, 0, sAbsent.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sParams.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sGuides.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sEncycl.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_DELETE, sDel.c_str()) ;
    }
    else if (!(pNSTreeNodeSelection->getGlobalText() == string(""))) //frère pilote
    {
    	menu->AppendMenu(MF_STRING, IDC_INSERTFRERE,         sInserB.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_INSERTELEMENTFICTIF, sInserA.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_MODIFIER,            sModif.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_GRAYED, 0, sAbsent.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sParams.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sGuides.c_str()) ;
      menu->AppendMenu(MF_GRAYED, 0, sEncycl.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_DELETE, sDel.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_CUT,    sCut.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_COPY,   sCopy.c_str()) ;

      if (pSuper->pBufCopie->empty())
      	menu->AppendMenu(MF_GRAYED, 0, sPaste.c_str()) ;
      else
      	menu->AppendMenu(MF_STRING, IDC_PASTE, sPaste.c_str()) ;
    }
    else
    {
    	menu->AppendMenu(MF_STRING, IDC_INSERTFRERE,         sInserB.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_INSERTELEMENTFICTIF, sInserA.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_MODIFIER,            sModif.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

      string sCertitude = pNSTreeNodeSelection->Certitude() ;
      if ((sCertitude == "ABSENCE") || (sCertitude == "PRESENCE"))
      {
      	if (pNSTreeNodeSelection->isAbsent())
        	menu->AppendMenu(MF_STRING, IDC_PRESENCE, sPresen.c_str()) ;
        else
        	menu->AppendMenu(MF_STRING, IDC_ABSENT,   sAbsent.c_str()) ;
      }
      else
      	menu->AppendMenu(MF_STRING, IDC_ABSENT, sAbsent.c_str()) ;

      menu->AppendMenu(MF_STRING, IDC_PARAMETRES, sParams.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_FILGUIDE,   sGuides.c_str()) ;

      if (pNSTreeNodeSelection->isEncyclop())
      	menu->AppendMenu(MF_STRING, IDC_ENCYCLOP, sEncycl.c_str()) ;
      else
      	menu->AppendMenu(MF_GRAYED, 0, sEncycl.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;      menu->AppendMenu(MF_STRING, IDC_DELETE, sDel.c_str()) ;      menu->AppendMenu(MF_STRING, IDC_CUT,    sCut.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_COPY,   sCopy.c_str()) ;
      if (pSuper->pBufCopie->empty())
      	menu->AppendMenu(MF_GRAYED, 0, sPaste.c_str()) ;
      else
      	menu->AppendMenu(MF_STRING, IDC_PASTE, sPaste.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_TREE_NEWPREOCCUP, sCreHC.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_CLASSIF,          sClassi.c_str()) ;
    }
  }
  ClientToScreen(lp) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  delete menu ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::EvRButtonDown.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
// 					  		Décomposer pChaine et mettre ses
//							  		items dans le vecteur pVect
//-------------------------------------------------------------------------
void
NSTreeWindow::DecomposeChaineTL(VecteurChaine* pVect, string sChaine  )
{
try
{
	if ((VecteurChaine*) NULL == pVect)
		return ;

	size_t debut = 0 ;
	size_t posit = sChaine.find(string("|")) ;
	if (NPOS == posit)		pVect->push_back(new string(sChaine)) ;
	else
	{
		while ((NPOS != posit) && (posit < strlen(sChaine.c_str())))
		{
    	pVect->push_back(new string(sChaine, debut, posit - debut)) ;
      debut = posit + 1 ;
      posit = sChaine.find(string("|"), debut+1) ;
    }
    pVect->push_back(new string(sChaine, debut, strlen(sChaine.c_str()) - debut)) ;
	}
}
catch (...){
	erreur("Exception NSTreeWindow::DecomposeChaineTL.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
// double click sur un noeud qui supporte :
//						- 	un texte libre : éditer ce texte
//						-  un NSTreeNode proposé : valider le NSTreeNode et le développer
//						-	un NSTreeNode classique : lui créer un frère et un fils
//						-  un NSTreeNode prolongeable : le développer, il peut lancer une
//							boîte de dialogue ou des fils proposés
//------------------------------------------------------------------------
void
NSTreeWindow::EvLButtonDblClk(uint /*a*/, NS_CLASSLIB::TPoint& p)
{
	// ATTENTION : Vérifier que le double click n'est pas effectué dans
  //             le NSEditDico en cours (pour l'instant on applique le même
  //             traitement)
  if (_pEditDico && bEditionDico)
  {
  	ClassLib::TRect rectangleEdit ;
    _pEditDico->GetClientRect(rectangleEdit) ;
    if (rectangleEdit.Contains(p))
    	return ;
    else
    	return ;
  }

	Valider() ;
}

// -----------------------------------------------------------------------
//  Activer le noeud en cours
//
//  Si bInit = false c'est une initialisation automatique au démarrage
// -----------------------------------------------------------------------
voidNSTreeWindow::Valider(bool bInit)
{
try
{
	TTreeNode node = GetSelection() ;	NSTreeNode* pNSTreeNodeDblClk = GetNSTreeNode(&node) ;
	if ((NSTreeNode*) NULL == pNSTreeNodeDblClk)
		return ;

	//TTreeWindow::EvLButtonDblClk(a, p);  //appel TvnItemExpanDing
	int decalage = 0 ;
	// int ligneMax = 0, colonneMax = 0;

	//
	// Analyse du noeud sur lequel est effectué le double click
	//

	// Si le noeud est fictif (pControle == 0), on l'édite normalement
	// sauf si c'est un noeud proposé ou un frère lié de texte libre
	//
	if (pNSTreeNodeDblClk->estFictif())
  	if ((false      == pNSTreeNodeDblClk->isProposed()) &&
        (string("") == pNSTreeNodeDblClk->getGlobalText()) &&
        (NULL       == pNSTreeNodeDblClk->getPilotNode()))
			return ;

	int ligne   = pNSTreeNodeDblClk->getLigne() ;
	int colonne = pNSTreeNodeDblClk->getColonne() ;
	//
	// Valeur chiffrée
	//
	if (pNSTreeNodeDblClk->isCreateurChampEdit() && !bInit)
	{
		bool bCalcul = false ; //calcul dans convert.db
    //
    // Si "CALCUL DYNAMIQUE" recalculer sComplement à partir de convert.db
    //
    if (pNSTreeNodeDblClk->isRecalcul())
    {
      NSPatPathoArray NSPatPathoEnCours(pContexte->getSuperviseur()) ;
      //false pour ne pas tuer pTransfert lié à pBBitemNSTreeWindow
      pBBitemNSTreeWindow->okFermerDialogue(true, false) ;
      BBFilsItem* pBBFilsLanceur = *(pBBitemNSTreeWindow->_aBBItemFils.begin()) ;
      NSFatheredPatPathoArray* pFatheredElement = *(pBBFilsLanceur->getPatPatho()->begin()) ;
      NSPatPathoEnCours = *(pFatheredElement->getPatPatho()) ;

      string sUniteConvert  = string("") ;
      string sMethodeCalcul = string("") ;

      if (Converter.isOpened())
      {
        string sNodeLabel = pNSTreeNodeDblClk->getLabelString() ;
      	Converter.CalculValeur(pNSTreeNodeDblClk->getFatherNode()->getEtiquette(), &sNodeLabel,
			                         		&sUniteConvert, &sMethodeCalcul, &NSPatPathoEnCours) ;
        pNSTreeNodeDblClk->SetLabelString(sNodeLabel) ;
      }

      if (pNSTreeNodeDblClk->getLabelString() != string(""))
      	pNSTreeNodeDblClk->SetComplement(pNSTreeNodeDblClk->getLabelString()) ;
      if (string("") != sUniteConvert)
      	pNSTreeNodeDblClk->SetUnit(sUniteConvert + string("1")) ;
      if (string("") != sMethodeCalcul)
      	pNSTreeNodeDblClk->getFatherNode()->SetComplement(sMethodeCalcul) ;
      bCalcul = true ;
    }
    RecupereValeurChiffree(pNSTreeNodeDblClk, bCalcul) ;
	}
  //
  // Date et heure
  //
  else if (pNSTreeNodeDblClk->isDateTime() && !bInit)
		RecupereUniteDateHeure(pNSTreeNodeDblClk) ;
  //
  // Texte libre : édition
  //
  else if (((pNSTreeNodeDblClk->getGlobalText() != string("")) ||
        		 (pNSTreeNodeDblClk->getPilotNode())) && !bInit)
	{
  	ModifierTexteLibre = true ;
    if (pNSTreeNodeDblClk->estLie()) //si le NSTreeNode est lié : modifier son frère pilote
    {
    	pNSTreeNodeDblClk->getPilotNode()->SelectItem(TVGN_CARET) ;
      pNSTreeNodeDblClk->getPilotNode()->EditLabel() ;
    }
    else
    	pNSTreeNodeDblClk->EditLabel() ;
	}
  //
  // Prolongement avec des fils fil guide
  //
  else if (pNSTreeNodeDblClk->getControl() /* && !bInit */)
	{
  	if (pNSTreeNodeDblClk->getTransfert())
    {
    	//BBFilsItem* rattaché à ce noeud
      BBFilsItem* pBBfils = pNSTreeNodeDblClk->getBBFilsItem() ;
      if (pBBfils && (false == pBBfils->FilsProlongeable))
      {
      	BBItemData* pDonnees = new BBItemData ;
        //Recherche sémantique
        bool trouve = RechercheFilGuide(pNSTreeNodeDblClk, pDonnees, pBBfils) ;
        if (false == trouve)
        	pBBfils->FilsProlongeable = false ;
        delete pDonnees ;
      }
    }
    //
    // exemple lancement de boîte de dialogue
    //
    if (pNSTreeNodeDblClk->getBBFilsItem() &&
        (true == pNSTreeNodeDblClk->getBBFilsItem()->FilsProlongeable))
    {
    	decalage = 0 ;
      developper(pNSTreeNodeDblClk, &node, &decalage, ligne, colonne, false, false, true, bInit) ;
      // if (decalage)
      // {
      	//décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMaxInit
      //   for (iterNSTreeNode iter = _NodeArray.begin();
      //               						_NodeArray.end() != iter ; iter++)
      //   {
      //   	if (((*iter)->getLigne() > ligne) &&
      //                   (!(*iter)->Descendant(pNSTreeNodeDblClk)))
      //     	(*iter)->setLigne((*iter)->getLigne() + decalage) ;
      //   }
      // }
    }
    //
    // un noeud classique : création de frère et fils
    //
    else
    {
    	pNSTreeNodeDblClk->SetImageIndex(0) ;
      pNSTreeNodeDblClk->SetSelectedImageIndex(0, true) ;
      InsereApres() ;
    }
  }
  //
  // élément proposé : valider pNSTreeNodeDblClk et lui creér un fils et
  //							un frère ou des fils proposés
  //
	else if (pNSTreeNodeDblClk->isProposed())
	{
  	string sLibelleTrouve ;
    MiseAjourNSTreeNode(pNSTreeNodeDblClk) ;
    pNSTreeNodeDblClk->SetProposed(false) ;
    pNSTreeNodeDblClk->SetInteret("A") ;
    //localisation
    string sCodeSens ;
    string sNodeLabel = pNSTreeNodeDblClk->getEtiquette() ;
    NSDico::donneCodeSens(&sNodeLabel, &sCodeSens) ;

    if ((BBFilsItem*) NULL == pNSTreeNodeDblClk->getBBFilsItem())
			return ;

    BBItem* pBBItem = pNSTreeNodeDblClk->getBBFilsItem()->getItemFather() ;
		if ((BBItem*) NULL == pBBItem)
			return ;

    // On montre l'arbre, avec son noeud nouveau ou modifié, au Blackboard
    // pBBItem->pBigBoss->showBB();

    pNSTreeNodeDblClk->SetPosition(pBBItem->_sLocalisation +         				                  string(1,cheminSeparationMARK) + sCodeSens) ;

    bool CreerFilsChampEdit = false ;
    bool bDateHeure = false ;
    //
    // Si ce le code lexique de ce pNSTreeNode commence par V (valeur chiffrée)
    // alors lui créer  un fils qui lancera la boîte d'édition et
    // récupérera la valeur donnée et son unité
    //
    if      (NSDico::CodeCategorie(pNSTreeNodeDblClk->getEtiquette()) == string("V"))
    	CreerFilsChampEdit = true ;
    else if (NSDico::CodeCategorie(pNSTreeNodeDblClk->getEtiquette()) == string("K"))
    	bDateHeure = true ;

    decalage = 0 ;
    developper(pNSTreeNodeDblClk ,&node, &decalage, ligne, colonne, CreerFilsChampEdit, bDateHeure, false, bInit) ;
    pNSTreeNodeDblClk->SetImageIndex(0) ;
    pNSTreeNodeDblClk->SetSelectedImageIndex(0, true) ;
    
    sNodeLabel = pNSTreeNodeDblClk->getEtiquette() ;
    pNSTreeNodeDblClk->DicoKillFocus(&sNodeLabel, "CC") ;
    pNSTreeNodeDblClk->ctrlNotification() ;

    //
    // contracter pNSTreeNode si valeur chiffrée non suivi de fils guides
    //
    if (CreerFilsChampEdit || bDateHeure)
    {
    	NSTreeNode* pPremFils = *(pNSTreeNodeDblClk->getVectSons()->begin()) ;
      if ((string("") != pPremFils->getLabelString()) &&
                                       (false == pPremFils->isProposed()))
      {
      	string sLabel ;
        //
        //Contracter le père
        //
        sLabel = pNSTreeNodeDblClk->getLabelString() + " : " + pPremFils->getLabelString() ;
        pNSTreeNodeDblClk->SetText(sLabel.c_str(), true) ;
        pNSTreeNodeDblClk->CollapseTousNoeud() ;
        AfficheInteretMaximum(pNSTreeNodeDblClk) ; //affiche pour pNSTreeNodeDblClk l'interêt le plus élevé
                                        //parmi ceux de ses descendants
      }
    }
    // if (decalage)
    // {
    	//décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMaxInit
    //   for (iterNSTreeNode iter = _NodeArray.begin() ; _NodeArray.end() != iter ; iter++)
    //   	if( ((*iter)->getLigne() > ligne) && (!(*iter)->Descendant(pNSTreeNodeDblClk)))
    //     	(*iter)->setLigne((*iter)->getLigne() + decalage) ;
    // }
	}
}
catch (...)
{
	erreur("Exception NSTreeWindow::Valider.", standardError, 0) ;
}
}

// Virtual node validated by a process (and not the user), don't
// open sub-windows or sub-nodes
//
void
NSTreeWindow::NodeRemoteValidation(NSTreeNode* pNSTreeNode)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  // Creation of BBItem etc
  //
  MiseAjourNSTreeNode(pNSTreeNode) ;

  // If something wrong happened, better leave
  //
  if ((BBFilsItem*) NULL == pNSTreeNode->getBBFilsItem())
    return ;

  pNSTreeNode->SetProposed(false) ;
  pNSTreeNode->SetInteret("A") ;

  BBItem* pBBItem = pNSTreeNode->getBBFilsItem()->getItemFather() ;
  if ((BBItem*) NULL == pBBItem)
    return ;

  //localisation
  string sCodeSens ;
  string sNodeLabel = pNSTreeNode->getEtiquette() ;
  NSDico::donneCodeSens(&sNodeLabel, &sCodeSens) ;

  // On montre l'arbre, avec son noeud nouveau ou modifié, au Blackboard
  // pBBItem->pBigBoss->showBB();
  pNSTreeNode->SetPosition(pBBItem->_sLocalisation +
         				string(1, cheminSeparationMARK) + sCodeSens) ;


  // decalage = 0 ;
  // developper(pNSTreeNodeDblClk ,&node, &decalage, ligne, colonne, CreerFilsChampEdit, bDateHeure, false, bInit) ;

  pNSTreeNode->SetImageIndex(0) ;
  pNSTreeNode->SetSelectedImageIndex(0, true) ;
  // pNSTreeNode->DicoKillFocus(&pNSTreeNodeDblClk->getEtiquette(), "CC") ;
  pNSTreeNode->ctrlNotification() ;
}

//----------------------------------------------------------------------
// affiche pour pNSTreeNode l'interêt le plus élevé parmi ceux de ses descendants
//----------------------------------------------------------------------
void
NSTreeWindow::AfficheInteretMaximum(NSTreeNode* pNSTreeNode)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  string sInteret = pNSTreeNode->getInteret() ; //sauvegarde
  string sInteretMax = string("") ;
  pNSTreeNode->donneIneretMax(&sInteretMax) ;
  pNSTreeNode->SetInteret(sInteretMax) ;
  pNSTreeNode->Interet() ;
  pNSTreeNode->SetInteret(sInteret) ;
}

//----------------------------------------------------------------------
// affiche l'intérêt réel du pNSTreeNode
//----------------------------------------------------------------------
void
NSTreeWindow::AfficheInteretReel(NSTreeNode* pNSTreeNode)
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	pNSTreeNode->Interet() ; //afficher l'interêt réel de ce noeud
}

//----------------------------------------------------------------------
// récuperer une valeur chiffrée avec son unité
// bCalcul : calcul dynamique ?
//----------------------------------------------------------------------
void
NSTreeWindow::RecupereValeurChiffree(NSTreeNode* pNSTreeNode, bool bCalcul)
{
try
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	string sQualifiant = string("") ;
  NS_CLASSLIB::TRect rectEdit ;
  string sLabelUnite ; //label de l'unité (venant du lexique)
  pNSTreeNode->getFatherNode()->GetItemRect(rectEdit, true) ;

  if ((ChampEdition*) NULL == pChampEdition)
    pChampEdition = new ChampEdition(this, "VALEUR_CHIFFREE", 255, pContexte, rectEdit, pNSDLLModule) ;

  pChampEdition->bCalcul = bCalcul;
  if ((pNSTreeNode->getEtiquette().find("£ND")  != NPOS) ||
      (pNSTreeNode->getEtiquette().find("/£ND") != NPOS))
    pChampEdition->sTypeCalcul = "DYNAMIQUE" ;

  // Donner les valeurs par défaut (unité et valeurs) à pChampEdition
  // ces valeurs proviennent de pNSTreeNode ou de nssavoir.db
  //
  if (pNSTreeNode->getLabelString() != string(""))
  {
    size_t posIntervalle = pNSTreeNode->getContenuTransfert().find("<<");
    if (NPOS == posIntervalle) //valeur exacte
    {
      pChampEdition->sValeurExacte = pNSTreeNode->getContenuTransfert() ;
      //
      // Transformer le point en une virgule
      //
      size_t pos = pChampEdition->sValeurExacte.find(".") ;
      if (pos != NPOS)
        pChampEdition->sValeurExacte.replace(pos, 1, ",") ;
      pChampEdition->sValeurInf = "" ;
      pChampEdition->sValeurSup = "" ;
    }
    else
    {
      pChampEdition->sValeurExacte = string("") ;
      pChampEdition->sValeurInf = string(pNSTreeNode->getContenuTransfert(), 0, posIntervalle) ;

      // TODO check this ---- STRANGE
      //
      pChampEdition->sValeurSup = string(pNSTreeNode->getContenuTransfert(), posIntervalle + 2, strlen(pNSTreeNode->getLabelString().c_str())) ;
      size_t pos = pChampEdition->sValeurInf.find(".") ;
      if (NPOS != pos)
        pChampEdition->sValeurInf.replace(pos, 1, ",") ;
      pos = pChampEdition->sValeurSup.find(".") ;
      if (NPOS != pos)
        pChampEdition->sValeurSup.replace(pos, 1, ",") ;
    }
  }

  // Unité ?
  if (pNSTreeNode->getUnit() == "200001")
  {
    sQualifiant = string("") ;
    pChampEdition->pUnite->setCode(string("200001")) ;
  }
  else if (pNSTreeNode->getUnit() != "")
    sQualifiant = pNSTreeNode->getUnit() ;
  else
    // Le père la fournit : exemple poids -> kg
    ////unité dans nssavoir.db
    sQualifiant = TrouverUnite(pNSTreeNode->getFatherNode()->getEtiquette()) ;

  if (sQualifiant != "")
  {
/*
   	    if (strlen(sQualifiant.c_str()) == (BASE_LEXI_LEN - 1))
      	    sQualifiant += "1";
        pContexte->getSuperviseur()->pDico->donneLibelle(sLang, &sQualifiant, &sLabelUnite);
        pChampEdition->pUnite->sCode = sQualifiant;
        pChampEdition->sUnite = sLabelUnite ;
*/
    pChampEdition->setUnit(sQualifiant) ;
  }
  else //unité si elle existe fournie par le lexique ou pas d'unité
  {
    size_t pos = pNSTreeNode->getEtiquette().find("/") ;
    if (NPOS != pos)
    {
      string sCode = string(pNSTreeNode->getEtiquette(), 0, pos) ;
/*
            if (sCode == "200001")//ne pas afficher "~nombre sans unité"
         	    sLabelUnite = "";
            else
      		    pContexte->getSuperviseur()->pDico->donneLibelle(sLang, &sCode, &sLabelUnite);
	        	pChampEdition->sUnite = sLabelUnite ;
            pChampEdition->pUnite->sCode = sCode;
*/
      pChampEdition->setUnit(sCode) ;
    }
  }
  pChampEdition->SetCaption(pNSTreeNode->getFatherNode()->getLabelString().c_str()); //titre , exemple "Poids du patient"
  pChampEdition->sEtiquette = pNSTreeNode->getFatherNode()->getEtiquette() ;
  int execute = pChampEdition->Execute() ;
  if (IDOK == execute)
  {
    //
    // Ne pas accepter les textes libres
    //
    if (string("£?????") == pChampEdition->sUnite)
    {
      string sErrMsg = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
      pContexte->getSuperviseur()->trace(&sErrMsg, 1, NSSuper::trWarning) ;
      erreur(sErrMsg.c_str(), warningError, 0) ;
      pChampEdition->pUnite->SetFocus() ;
      return ;
    }
    string sLivre = "£N0;03" ;
    // Calcul dynamique ou stockage statique
    if ((pChampEdition->sTypeCalcul == "STOCK") ||
            (pChampEdition->sTypeCalcul == ""))
      pNSTreeNode->SetRecalcul(false) ;

    if (pChampEdition->sTypeCalcul == "DYNAMIQUE")
    {
      pNSTreeNode->SetRecalcul(true) ;
      sLivre = "£ND;03" ;
    }
    pNSTreeNode->SetCreateurChampEdit(true) ;
    pNSTreeNode->SetType(string("CL")) ;

      /*string sUnit = "";

      if(pChampEdition->sUnite != "")
      	sUnit = string(" ") + pChampEdition->sUnite;

      if(pChampEdition->sValeurExacte != "")
      	pNSTreeNode->sLabel = pChampEdition->sValeurExacte + sUnit;
      else
      	pNSTreeNode->sLabel = "entre " + pChampEdition->sValeurInf + (" et ") +
 				                       pChampEdition->sValeurSup + sUnit;*/

    if (string("") == pChampEdition->pUnite->getCode())
      pChampEdition->pUnite->setCode(string("200001")) ;

    pNSTreeNode->SetLexique(sLivre) ;
    pNSTreeNode->SetUnit(pChampEdition->pUnite->getCode()) ;

    string sComplement = string("") ;

    if (string("") != pChampEdition->sValeurExacte)
    {
      //
      // Transformer la virgule si elle existe en un point
      //
      sComplement = pChampEdition->sValeurExacte ;
      size_t pos = sComplement.find(",") ;
      if (NPOS != pos)
        sComplement.replace(pos, 1, ".") ;
    }
    else
    {
      //
      // Transformer la virgule si elle existe en un point
      //
      string sInf = pChampEdition->sValeurInf ;
      size_t pos = sInf.find(",") ;
      if (pos != NPOS)
        sInf.replace(pos, 1, ".") ;

      string sSup = pChampEdition->sValeurSup ;
      pos = sSup.find(",") ;
      if (pos != NPOS)
        sSup.replace(pos, 1, ".") ;

      if ((string("") != sInf) || (string("") != sSup))
        sComplement = sInf + ("<<") + sSup;
    }
    pNSTreeNode->SetComplement(sComplement) ;

    string sChemin, sCheminLocal ;
    pNSTreeNode->formerChemin(&sChemin) ;
    pNSTreeNode->formerCheminLocal(&sCheminLocal) ;
    NSCutPaste CP(pContexte->getSuperviseur()) ;
    pNSTreeNode->formerPatPatho(&CP) ;

    NSPatPathoArray CPPatho(pContexte->getSuperviseur()) ;
    CP.initFromPatPatho(&CPPatho) ;

    GlobalDkd Dcode(pContexte, sLang, sChemin, &CPPatho) ;
    Dcode.decodeNoeud(sCheminLocal) ;
    pNSTreeNode->SetLabelString(Dcode.getDcodeur()) ;

    pNSTreeNode->SetText(pNSTreeNode->getLabelString().c_str(), true) ;

    if (false == pNSTreeNode->estFictif())
      ReMiseAjourNSTreeNode(pNSTreeNode, pNSTreeNode->getEtiquette(), sComplement) ;
  }

  pChampEdition->sValeurExacte 	= string("") ;
  pChampEdition->sValeurSup    	= string("") ;
  pChampEdition->sValeurInf    	= string("") ;
  pChampEdition->sUnite        	= string("") ;
  pChampEdition->sTypeCalcul		= string("") ;
  pChampEdition->pUnite->setCode(string("")) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTreeWindow::RecupereValeurChiffree : ") + e.why() ;
  pContexte->getSuperviseur()->trace(&sErr, 1, NSSuper::trError) ;
  erreur(sErr.c_str(), standardError, 0) ;
  return ;
}
catch (...){
	erreur("Exception NSTreeWindow::RecupereValeurChiffree.", standardError, 0);
}
}

//----------------------------------------------------------------------
// récuperer l'unité d'une valeur chiffrée type date_heure
//----------------------------------------------------------------------

voidNSTreeWindow::RecupereUniteDateHeure(NSTreeNode* pNSTreeNode)
{
try
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	string sUnite = string("") ;
	NS_CLASSLIB::TRect rectEdit ;
	string sLabelUnite ; //label de l'unité (venant du lexique)
	pNSTreeNode->getFatherNode()->GetItemRect(rectEdit, true) ;
	string sIdentitePere = pNSTreeNode->getFatherNode()->getEtiquette() ;

	//
	// Chercher dans nssavoir.db le code lexique qui est lié à celui du père
	// de pNewNSTreeNode par le lien "ME" et qui contient l'unité de la valeur
	// chiffrée
	//
	sUnite = TrouverUnite(pNSTreeNode->getFatherNode()->getEtiquette()) ;
	if (string("") == sUnite)
		return ;

	if ((NSDate*) NULL == pChampDate)
	{
  	if      (string("2DA01") == sUnite)
    	pChampDate = new NSDate(this, "Date", pContexte, "DATE", rectEdit, pNSDLLModule) ;
		else if (string("2HE01") == sUnite)
    	pChampDate = new NSDate(this, "Date", pContexte, "HEURE", rectEdit, pNSDLLModule) ;
    else
    	return ;
	}
	else
		pChampDate->Initialise(sUnite);

  //
  // Donner les valeurs par défaut (unité et valeurs) à pChampEdition
  // ces valeurs proviennent de pNSTreeNode ou de nssavoir.db
  //
  // if (string("") != pNSTreeNode->sLabel)
  //   pChampDate->sValeur = pNSTreeNode->sLabel;
  if (string("") != pNSTreeNode->getContenuTransfert())
    pChampDate->sValeur = pNSTreeNode->getContenuTransfert() ;

  pChampDate->SetCaption(pNSTreeNode->getFatherNode()->getLabelString().c_str()) ; //titre
  int execute = pChampDate->Execute() ;
  if (IDOK == execute)
  {
    if (string("") != sUnite) //unité dans nssavoir.db
    {
      pNSTreeNode->SetLabelString(pChampDate->sValeur) ;
      pNSTreeNode->AfficheLabel(sUnite) ;
      if (strlen(sUnite.c_str()) == BASE_SENS_LEN)
      	pContexte->getDico()->donneCodeComplet(sUnite) ;

			pNSTreeNode->SetLexique("£D0;10") ;
			pNSTreeNode->SetUnit(sUnite) ;
			pNSTreeNode->SetText(pNSTreeNode->getLabelString().c_str(), true) ;
      //
      // transformer la virgule, si elle existe, en un point
      //
      string sComplement = pChampDate->sValeur ;
      size_t pos = sComplement.find(",") ;
      if (pos != NPOS)
        sComplement.replace(pos, 1, ".") ;
      pNSTreeNode->SetComplement(sComplement) ;      pNSTreeNode->SetDateTime(true) ;
      pNSTreeNode->SetType(string("CL")) ;
    }
    else
      pNSTreeNode->SetLexique(string("")) ;
	}
  pChampDate->Nettoyer() ;
}
catch (...){
	erreur("Exception NSTreeWindow::RecupereUniteDateHeure.", standardError, 0) ;
}}

//---------------------------------------------------------------------------//Trouver les items équivalents à sQualifie au sens réseau sémantique
//et s'arrêter au premier ayant un lien de type "ME".
//Renvoyer dans string le qualifiant trouvé sinon "".
//---------------------------------------------------------------------------
string
NSTreeWindow::TrouverUnite(string sQualifie)
{
  if (string("") == sQualifie)
    return string("") ;

  string sQualifiant = chercheClefComposite(sQualifie, "ME") ;
  if (string("") != sQualifiant)
    return sQualifiant ;

  //  // recherche sémantique sur les qualifiants liés à sQualifie par le lien
  // sLien et appel recursif sur les sQualifiants
  //
  sQualifiant = chercheClefComposite(sQualifie, "ES") ;
  if (sQualifiant == "")
    return "" ;

  return (TrouverUnite(sQualifiant)) ;
}

//--------------------------------------------------------------------------//recherche composite selon le lien sLien et le qualifié sQualifie, retourner
//le qualifiant
//---------------------------------------------------------------------------
string
NSTreeWindow::chercheClefComposite(string sQualifie, string sLien)
{
try
{
	if ((string("") == sQualifie) || (string("") == sLien))
		return string("") ;

  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&sQualifie, &sCodeSens) ;

  if ((strlen(sLien.c_str()) > SAVOIR_LIEN_LEN) || (strlen(sCodeSens.c_str()) > SAVOIR_QUALIFIE_LEN))
    return string("") ;

	NSSuper*  pSuper  = pContexte->getSuperviseur() ;
  NSSavoir* pSavoir = pSuper->getFilGuide()->getSavoir() ;
  if ((NSSavoir*) NULL == pSavoir)
    return string("") ;

  CURProps curProps ;
	DBIResult lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps) ;
	if (lastError != DBIERR_NONE)
	{
		erreur("Impossible d'acceder aux propriétés de nssavoir.db.", standardError, lastError) ;
		return string("") ;
	}

	Byte* pIndex = new Byte[curProps.iRecBufSize] ;
	memset(pIndex, 0, curProps.iRecBufSize) ;
	DbiPutField(pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD,     pIndex, (Byte*)(sLien.c_str())) ;
	DbiPutField(pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD, pIndex, (Byte*)(sCodeSens.c_str())) ;
	pSavoir->lastError = pSavoir->chercheClefComposite("FLECHE",
                                                     NODEFAULTINDEX,
                                                     keySEARCHEQ,
                                                     dbiWRITELOCK,
                                                     pIndex) ;
  delete[] pIndex ;

  if (pSavoir->lastError != DBIERR_NONE)
		return string("") ;

	pSavoir->lastError = pSavoir->getRecord() ;
	if (pSavoir->lastError != DBIERR_NONE)
	{
		erreur("Le fichier nssavoir.db inutilisable.", standardError, pSavoir->lastError) ;
		return string("") ;
	}

	return pSavoir->getQualifier() ;
}
catch (...){
	erreur("Exception NSTreeWindow::chercheClefComposite.", standardError, 0) ;
	return string("") ;
}
}

//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§//
//		  				fonctions spécifiques au Drag and Drop,
//							au copie collée, coupée collée
//
//§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§§

//---------------------------------------------------------------//On est sur le noeud à "Dragger"
//---------------------------------------------------------------

bool
NSTreeWindow::EvTvnBeginDrag(TTwNotify& nmHdr)
{
	if (ReadOnly)
		return false ;

try
{
	NSTreeNode* pNSTreeNodeDrag = GetNSTreeNode(nmHdr.itemNew.hItem) ; //NSTreeNode correspondant
	if ((NSTreeNode*) NULL == pNSTreeNodeDrag)
		return false ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	if ((pNSTreeNodeDrag->estFictif()) || (pNSTreeNodeDrag->isProposed()))
	{
  	pSuper->bDragAndDrop = false ;
    return false ;
	}

	// s'il s'agit d'un frère lié , prendre son frère pilote
	if (pNSTreeNodeDrag->estLie())
  	pNSTreeNodeDrag = pNSTreeNodeDrag->getPilotNode() ;

  pNSTreeNodeDrag->CopyPatPatho("DRAG") ;

  NS_CLASSLIB::TRect itemRect ;

  pSuper->offNode      = true ;
  pSuper->bDragAndDrop = true ;


  pNSTreeNodeDrag->GetItemRect(itemRect) ;  // Transforme le curseur en curseur de drag  //
  NS_CLASSLIB::TRect curRect(0, 0, nmHdr.ptDrag.x, nmHdr.ptDrag.y) ;
  curRect &= itemRect ; // Get the intersection of the two rectangles.
                        // ( Width()+GetIndent(), Height() ) is the cursor's hotspot
                        // inside the item rect.

  //
  // Start the drag operation
  //
  pSuper->DragDrop = new TImageList(pNSTreeNodeDrag->CreateDragImage()) ;
  pSuper->DragDrop->BeginDrag(0, curRect.Width()+GetIndent(), curRect.Height()) ;
  pSuper->DragDrop->DragEnter(*this, nmHdr.ptDrag.x, nmHdr.ptDrag.y) ;
  SetCapture() ;

  // We want a fast update.  We'll use a timer instead of EvMouseMove
  // since we want to be able to scroll when the last (or first) visible
  // node is targeted (immitating Explorer).  This way resting the drag
  // image over the last (or first) node will continuously scroll the list
  //
  SetTimer(ID_TREETIMER, 60) ;

  return true ;
}
catch (...){
	erreur("Exception NSTreeWindow::EvTvnBeginDrag.", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------
// On empeche la TreeWindow d'écraser le texte libre lorsqu'on
// a cliqué hors de la zone d'édition (définie par rectEditDico)
//---------------------------------------------------------------
void
NSTreeWindow::EvLButtonDown( uint modKeys, NS_CLASSLIB::TPoint& pt )
{
try
{
  string sMsg = string("TreeWindow ") + getHandleAsString() + string(" EvLButtonDown with EditDico ") + getEditDicoHandleAsString() ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  /********* bug lorsqu'on clicke sur du texte situé au dessus d'un node
  if (bEditionDico)
  {
    // si le pointeur est dans la zone Edit
    if (rectEditDico.Contains(pt))
      TTreeWindow::EvLButtonDown(modKeys, pt) ;
  }
  ***************************/

  if ((false == bEditionDico) || ((NSEditDico*) NULL == _pEditDico))
    TTreeWindow::EvLButtonDown(modKeys, pt) ;
  else
  {
    // on teste dans ce cas si le texte est vide
    // (cas où on ne doit pas empecher le click)
    //
    string sTexte = DicoGetText() ;
    if (string("") == sTexte)   // on sort de l'édition du pEditDico
    {
      TTreeWindow::EvLButtonDown(modKeys, pt) ;
      SortieEditionDico() ;
    }
  }
}
catch (...){
	erreur("Exception NSTreeWindow::EvLButtonDown.", standardError, 0);
}
}

//---------------------------------------------------------------
//On a atteint le noeud sur lequel on va faire le "dropp"
//---------------------------------------------------------------
void
NSTreeWindow::EvLButtonUp( uint /*modKeys*/, NS_CLASSLIB::TPoint& pt )
{
	if (ReadOnly)
		return ;
try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	if (bEditionDico && _pEditDico)
	{
    // On prend en compte le décalage de la TreeWindow par rapport à sa mère
    // X et Y sont > 0 dans le cas d'une boite de dialogue mère
    // rectEditDico est exprimé en coordonnées client de la fenetre mère
    NS_CLASSLIB::TSize origin(Attr.X, Attr.Y) ;
    pt += origin ;

    // on récupère le texte
    //
    string sTexte = DicoGetText() ;
    if (string("") != sTexte)
    {
    	// on récupère le IC
      TIC* pIc = new TIC("DISPLAY", 0, 0) ;
      // on suppose constante la hauteur des caractères
      NS_CLASSLIB::TSize size = pIc->GetTextExtent(sTexte.c_str(), 1) ;
      int hauteur = size.Y() ;
      int largeur = 0 ;
      int line  ;
      if (hauteur >= 1)
      	line = (pt.Y() - rectEditDico.top) / hauteur ;
      else
      	line = 0 ;
      int nblines = _pEditDico->GetNumLines() ;
      int nbCars ;
      int lineLen ;

      if (line > (nblines - 1))
      {
      	line = nblines - 1 ;
        nbCars = _pEditDico->GetLineIndex(line) + _pEditDico->GetLineLength(line) ;
      }
      else
      {
      	nbCars  = _pEditDico->GetLineIndex(line) ;
        lineLen = _pEditDico->GetLineLength(line) ;
        string sLigne = string(sTexte, nbCars, lineLen) ;
        int numCar = 0 ;

        while ((numCar <= lineLen) && ((pt.X() - rectEditDico.left) > largeur))
        {
        	numCar++ ;
          size = pIc->GetTextExtent(sLigne.c_str(), numCar) ;
          largeur = size.X() ;
        }

        nbCars += numCar ;
      }
      delete pIc ;
      _pEditDico->SetSelection(nbCars, nbCars) ;
    }
    else
    	DefaultProcessing() ;
  }
  else if (pSuper->bDragAndDrop)
  {
  	NS_CLASSLIB::TPoint pt ;
    GetCursorPos(pt) ;

    NS_CLASSLIB::TRect wndRect = GetWindowRect() ;
    if (wndRect.Contains(pt))
    {
      // Clean up...
      //
      pSuper->DragDrop->DragLeave( *this ) ;
      pSuper->DragDrop->EndDrag() ;
      delete pContexte->getSuperviseur()->DragDrop ;
      pSuper->DragDrop = 0 ;
      ReleaseCapture() ;
      pSuper->bDragAndDrop = false ;

      // Get the node that we dropped on
      TTreeNode NodeDrop = GetDropHilite() ;
      NSTreeNode* pNodeDrop = GetNSTreeNode(&NodeDrop) ;
      if ((NSTreeNode*) NULL == pNodeDrop)
        return ;
      //
      // Un-drophilite the node we dropped on.  If we don't, very strange
      // TreeView node repainting will occur.
      //
      TTreeNode node( *this, (HTREEITEM)NULL ) ;
      node.SelectItem( TTreeNode::DropHilite ) ;

      if ((PositionDragAndDrop*) NULL == pPositionDragAndDrop)
      	pPositionDragAndDrop = new PositionDragAndDrop(this, "DRAG_DROP_COPIE",
                        pContexte, pNSDLLModule) ;

      int execute = pPositionDragAndDrop->Execute() ;
      if (IDOK == execute)
      {
      	if      (pPositionDragAndDrop->getDragType() == PositionDragAndDrop::dragBefore)
        	DragAvant(pNodeDrop, "DRAG") ;
        else if (pPositionDragAndDrop->getDragType() == PositionDragAndDrop::dragAfter)
        	DragApres(pNodeDrop, "DRAG") ;
        else
        	DragFils(pNodeDrop, "DRAG") ;
      }
      pPositionDragAndDrop->resetDragType() ;
      KillTimer(ID_TREETIMER) ;
      Update() ;
    }
    else
    {
    	/* HWND AppWindow = */ WindowFromPoint(pt) ;
    }
  }
  else
  	DefaultProcessing() ;
}catch (...)
{
	erreur("Exception NSTreeWindow::EvLButtonUp.", standardError, 0) ;
}
}

//-------------------------------------------------------------------------
// We'll use a timer message to update our drag image instead of MouseMove
// messages.
//-------------------------------------------------------------------------
void
NSTreeWindow::EvTimer( uint id )
{
  if (ReadOnly)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if (pContexte->getSuperviseur()->bDragAndDrop && (id == ID_TREETIMER))
  {
    NS_CLASSLIB::TPoint pt ;
    GetCursorPos(pt) ;

    NS_CLASSLIB::TRect wndRect = GetWindowRect() ;
    if (wndRect.Contains(pt))
    {
      ScreenToClient(pt) ;

      // Draw the image at the cursor's position
      //
      TTwHitTestInfo hitTestInfo ;
      hitTestInfo.pt = pt ;

      // Are we on a node?
      if (HitTest(&hitTestInfo))
      {
        TTreeNode dropHilite( *this, hitTestInfo.hItem ) ;
        TTreeItem item ;
        dropHilite.GetItem() ;
        item.mask |= TVIF_STATE ;
        dropHilite.GetItem(&item) ;

        if ( !(item.state & TVIS_DROPHILITED) || pSuper->offNode ) //no superfluous repaints
        {
          pSuper->offNode = false ;

          // If it's on an item, DropHilite the item.
          // In order for things to paint correctly, we have to turn off the
          // drag image, repaint the node, and turn the drag image back on.
          //
          pSuper->DragDrop->DragLeave( *this ) ;

          dropHilite.SelectItem( TTreeNode::DropHilite ) ;

          // This code makes the tree scroll if needed
          //
          TTreeNode next = dropHilite.GetNextVisible() ;
          if ( next )
            next.EnsureVisible() ;
          TTreeNode prev = dropHilite.GetPrevVisible() ;
          if ( prev )
            prev.EnsureVisible() ;

          // Turn the list back on
          pSuper->DragDrop->DragEnter( *this, pt.x, pt.y ) ;
        }
      }
      else if (!pSuper->offNode)//We're not on a node
      {
        pSuper->offNode = true; //We won't repaint again unless we need to...
        pSuper->DragDrop->DragLeave( *this );
        ::SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_NO)) );
        pSuper->DragDrop->DragEnter( *this, pt.x, pt.y );
      }
      // Move the drag image over here...
      //
      pSuper->DragDrop->DragMove( pt.x, pt.y );
    }
    else
      ::SetCursor(LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND)) ) ;
  }
  else
    DefaultProcessing() ;
}

void
NSTreeWindow::SynchroInformationForPath(string sPath, NSPatPathoArray* pPpt, string separator)
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  // Is there already information for this path?
  //
  NSTreeNode* pNode = GetNodeForPath(sPath, separator) ;
  if (pNode)
  {
    if (pNode->estFictif())
      NodeRemoteValidation(pNode) ;

    if (pNode->estFictif())
      return ;

    // Synchronize existing information with incoming patpatho
    //
    NSCutPaste CutPaste(pContexte->getSuperviseur()) ;
    CutPaste.setPatPatho(pPpt) ;
    DragFils(pNode, string("SYNCH"), &CutPaste) ;

    return ;
  }

  // Find the longer existing path
  //
  ClasseStringVector Vect ;
	DecomposeChaine(&sPath, &Vect, separator) ;
  if (Vect.empty())
		return ;

  iterClassString pathIt = Vect.end() ;
  pathIt-- ;

  while (((NSTreeNode*) NULL == pNode) && (Vect.begin() != pathIt))
  {
    // Build path up to pathIt
    //
    iterClassString It = Vect.begin() ;
    string sReducedPath = (*It)->getItem() ;
    It++ ;
    for ( ; (Vect.end() != It) && (It != pathIt) ; It++)
      sReducedPath += separator + (*It)->getItem() ;

    // Find if there is a node attached to this path
    //
    pNode = GetNodeForPath(sReducedPath, separator) ;

    if ((NSTreeNode*) NULL == pNode)
      pathIt-- ;
  }

  // Create a patpatho from pPpt with exceeding path appended front
  //
  NSPatPathoArray newPpt(pContexte->getSuperviseur()) ;
  for (int iCol = 0 ; Vect.end() != pathIt ; pathIt++, iCol++)
  {
    string sCodeComplet = (*pathIt)->getItem() ;
    pContexte->getDico()->donneCodeComplet(sCodeComplet) ;
    newPpt.ajoutePatho(sCodeComplet, iCol) ;
  }

  PatPathoIter iterPere = newPpt.end() ;
  iterPere-- ;
  newPpt.InserePatPathoFille(iterPere, pPpt) ;

  // Make all nodes visible
  //
  for (iterPere = newPpt.begin() ; newPpt.end() != iterPere ; iterPere++)
    (*iterPere)->setVisible(string("1")) ;

  // Insert the patpatho in the node
  //
  if (pNode)
  {
    if (pNode->estFictif())
      NodeRemoteValidation(pNode) ;

    if (pNode->estFictif())
      return ;

    // Synchronize existing information with incoming patpatho
    //
    NSCutPaste CutPaste(pContexte->getSuperviseur()) ;
    CutPaste.setPatPatho(&newPpt) ;
    DragFils(pNode, string("DRAG"), &CutPaste) ;

    pNode->ExpandTousNoeud() ;

    return ;
  }
}

void
NSTreeWindow::SynchroInformationForPath(string sPath, string sItemLabel, string sDocID, NSVectFatheredPatPathoArray* pTransPatpatho, HistoryConnector* pHC, string separator)
{
  if ((NSVectFatheredPatPathoArray*) NULL == pTransPatpatho)
    return ;

  if ((HistoryConnector*) NULL == pHC)
    return ;

  string sContextPath = pHC->getHistoryPath() ;

  // Is there already information for this path?
  //
  NSTreeNode* pNode = GetNodeForPath(sPath, separator) ;
  if (pNode)
  {
    if (pNode->estFictif())
      NodeRemoteValidation(pNode) ;

    if (pNode->estFictif())
      return ;

    for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
    {
      NSPatPathoInfo* pFatherItem = (*vectIt)->getFatherNode() ;
      NSTreeNode*     pFatherNode = (NSTreeNode*) 0 ;

      // Looking for a node in synthesis this information is a model of
      //
      VecteurString aNodes ;
      string sModelNodeId = getModelNodeFromDoc(pFatherItem, sDocID, &aNodes) ;
      if ((string("") != sModelNodeId) && (false == aNodes.empty()))
      {
        EquiItemIter vectIter = aNodes.begin() ;
        for ( ; (aNodes.end() != vectIter) && ((NSTreeNode*) NULL == pFatherNode) ; vectIter++)
          pFatherNode = GetNSTreeNode(**vectIter) ;
      }

      NSSuper* pSuper = pContexte->getSuperviseur() ;

      // Build patpatho
      //
      NSPatPathoArray newPpt(pSuper) ;
      Message ConceptMsg ;
      newPpt.parseBlock(sItemLabel, &ConceptMsg) ;
      newPpt.ajoutePatho(ConceptMsg.GetLexique(), &ConceptMsg, 0) ;
      newPpt.InserePatPathoFille(newPpt.begin(), (*vectIt)->getPatPatho()) ;
      newPpt.clearAllIDs() ;

      // Make all nodes visible
      //
      for (PatPathoIter iterPere = newPpt.begin() ; newPpt.end() != iterPere ; iterPere++)
        (*iterPere)->setVisible(string("1")) ;

      // No existing father node: must create new nodes
      //
      if ((NSTreeNode*) NULL == pFatherNode)
      {
        // Insert node in synthesis
        //
        NSCutPaste CutPaste(pSuper) ;
        CutPaste.setPatPatho(&newPpt) ;
        CutPaste.setUnique(pHC->isUnique()) ;
        DragFils(pNode, string("COPIE"), &CutPaste) ;

        pNode->ExpandTousNoeud() ;

        NSToDoTask* pTask = new NSToDoTask ;
        pTask->setWhatToDo(string("SaveTreeviewInformationAndConnectNew")) ;
        pTask->setPointer1(this, false /* mustDelete */) ;
        pTask->setPointer2(*vectIt,  false /* mustDelete */) ;
        pTask->setParam1(sContextPath) ;
        pTask->setParam2(sItemLabel) ;
        pTask->sendMe(pSuper) ;
      }
      //
      // Existing father node: must merge
      //
      else
      {
        // Check if there is something to update
        //
        NSCutPaste TestEqualCutPaste(pSuper) ;
        pFatherNode->formerPatPatho(&TestEqualCutPaste) ;

        NSPatPathoArray TestEqual(pSuper) ;
        TestEqualCutPaste.initFromPatPatho(&TestEqual) ;

        if (false == newPpt.hasSameContent(&TestEqual))
        {
          NSCutPaste CutPaste(pSuper) ;
          CutPaste.setPatPatho(&newPpt) ;
          CutPaste.setUnique(pHC->isUnique()) ;
          DragFils(pFatherNode, string("SYNCH"), &CutPaste) ;

          pFatherNode->ExpandTousNoeud() ;

          if ((pHC->isUnique()) && (false == pHC->getSubPathArray()->empty()))
          {
            // Next, remove all paths from this that are in the list and don't appear in Merging
            //
            vector<string*>::iterator it = (string**) pHC->getSubPathArray()->begin() ;
            for ( ; pHC->getSubPathArray()->end() != it ; it++)
            {
              string sIt = **it ;

              string sSubPath = sContextPath ;
              if (string("") != sIt)
                sSubPath += string(1, cheminSeparationMARK) + sIt ;

              if (string("") != sIt)
              {
                NSTreeNode* pPathNode = GetNodeForPath(sSubPath, string(1, cheminSeparationMARK)) ;
                if (pPathNode && (false == newPpt.CheminDansPatpatho(sIt)))
                  CouperNode(pPathNode) ;
              }
            }
          }

          // Save synthesis
          //
          NSToDoTask* pTask = new NSToDoTask ;
          pTask->setWhatToDo(string("SaveTreeviewInformation")) ;
          pTask->setPointer1(this, false /* mustDelete */) ;
          pTask->sendMe(pSuper) ;
        }
      }
    } // end for

    return ;
  }

  // Find the longer existing path
  //
  ClasseStringVector Vect ;
	DecomposeChaine(&sPath, &Vect, separator) ;
  if (Vect.empty())
		return ;

  iterClassString pathIt = Vect.end() ;
  pathIt-- ;

  while (((NSTreeNode*) NULL == pNode) && (Vect.begin() != pathIt))
  {
    // Build path up to pathIt
    //
    iterClassString It = Vect.begin() ;
    string sReducedPath = (*It)->getItem() ;
    It++ ;
    for ( ; (Vect.end() != It) && (It != pathIt) ; It++)
      sReducedPath += separator + (*It)->getItem() ;

    // Find if there is a node attached to this path
    //
    pNode = GetNodeForPath(sReducedPath, separator) ;

    if ((NSTreeNode*) NULL == pNode)
      pathIt-- ;
  }

  // Create a patpatho from pPpt with exceeding path appended front
  //
  NSPatPathoArray newPpt(pContexte->getSuperviseur()) ;
  for (int iCol = 0 ; Vect.end() != pathIt ; pathIt++, iCol++)
  {
    string sCodeComplet = (*pathIt)->getItem() ;
    pContexte->getDico()->donneCodeComplet(sCodeComplet) ;
    newPpt.ajoutePatho(sCodeComplet, iCol) ;
  }

  for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
  {
    NSPatPathoInfo* pFatherItem = (*vectIt)->getFatherNode() ;
    NSTreeNode*     pFatherNode = (NSTreeNode*) 0 ;

    NSSuper* pSuper = pContexte->getSuperviseur() ;

    // Build patpatho
    //
    NSPatPathoArray newBufferPpt(pSuper) ;
    Message ConceptMsg ;
    newBufferPpt.parseBlock(sItemLabel, &ConceptMsg) ;
    newBufferPpt.ajoutePatho(ConceptMsg.GetLexique(), &ConceptMsg, 0) ;
    newBufferPpt.InserePatPathoFille(newBufferPpt.begin(), (*vectIt)->getPatPatho()) ;

    NSPatPathoArray newLocalPpt = newPpt ;
    PatPathoIter iterPere = newLocalPpt.end() ;
    iterPere-- ;
    newLocalPpt.InserePatPathoFille(iterPere, &newBufferPpt) ;
    newLocalPpt.clearAllIDs() ;

    // Make all nodes visible
    //
    for (iterPere = newLocalPpt.begin() ; newLocalPpt.end() != iterPere ; iterPere++)
      (*iterPere)->setVisible(string("1")) ;

    // Insert the patpatho in the node
    //
    if (pNode)
    {
      if (pNode->estFictif())
        NodeRemoteValidation(pNode) ;

      if (pNode->estFictif())
        return ;

      // Synchronize existing information with incoming patpatho
      //
      NSCutPaste CutPaste(pContexte->getSuperviseur()) ;
      CutPaste.setPatPatho(&newLocalPpt) ;
      DragFils(pNode, string("DRAG"), &CutPaste) ;

      pNode->ExpandTousNoeud() ;

      NSToDoTask* pTask = new NSToDoTask ;
      pTask->setWhatToDo(string("SaveTreeviewInformationAndConnectNew")) ;
      pTask->setPointer1(this, false /* mustDelete */) ;
      pTask->setPointer2(*vectIt,  false /* mustDelete */) ;
      pTask->setParam1(sContextPath) ;
      pTask->setParam2(sItemLabel) ;
      pTask->sendMe(pSuper) ;
    }
  }
}

// Returns first node (if exists) that belongs to a model document (refered by
// sModelDocId) and is linked to CopyNode by a "copyOf" link.
//
// If pNodes is not NULL, it is filled with the whole list of such nodes
//
string
NSTreeWindow::getModelNodeFromDoc(NSPatPathoInfo* pCopyNode, string sModelDocId, VecteurString *pNodes)
{
  if ((NSPatPathoInfo*) NULL == pCopyNode)
    return string("") ;

  size_t iDocIdLen = PAT_NSS_LEN + DOC_CODE_DOCUM_LEN ;

  VecteurString aNodes ;

  // First, have a look in temporary nodes - for newly created nodes
  //
  NSLinkedNodeArray* pTempoLinks = pCopyNode->getTemporaryLinks() ;
  if (pTempoLinks && (false == pTempoLinks->empty()))
  {
    for (NSLinkedNodeIter i = pTempoLinks->begin() ; (pTempoLinks->end() != i) ; i++)
    {
      if ((dirFleche == (*i)->iLinkDirection) &&
                                      (NSRootLink::copyOf == (*i)->iLinkType))
      {
        string sCopyNodeId = (*i)->sOtherNode ;
        if (strlen(sCopyNodeId.c_str()) > iDocIdLen)
        {
          string sNodeDocId = string(sCopyNodeId, 0, iDocIdLen) ;
          if ((sNodeDocId == sModelDocId) && (false == aNodes.ItemDansUnVecteur(sCopyNodeId)))
            aNodes.AddString(sCopyNodeId) ;
        }
      }
    }
  }

  if (false == aNodes.empty())
  {
    if (pNodes)
      *pNodes = aNodes ;

    return *(*(aNodes.begin())) ;
  }

  // Now, have a look in graph
  //
  string sCopyNodeId = pCopyNode->getNode() ;
  if (string("") == sCopyNodeId)
    return string("") ;

  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphPerson)
    return string("") ;

  NSLinkManager* pLinkManager = pGraphPerson->getLinkManager() ;
  if ((NSLinkManager*) NULL == pLinkManager)
    return string("") ;

  VecteurString VString ;
  pLinkManager->TousLesVrais(sCopyNodeId, NSRootLink::copyOf, &aNodes, "FLECHE") ;

  if (aNodes.empty())
    return string("") ;

  if (pNodes)
    *pNodes = aNodes ;

  return *(*(aNodes.begin())) ;
}

//------------------------------------------------------------------------
//sCopie vaut "DRAG" si pSuper->pPatPathoDragDrop "COPIE" si pSuper->pPatPathoCopie
//copie la patpatho sous pNodeDrop
//------------------------------------------------------------------------
void
NSTreeWindow::CopyBranch(NSTreeNode* pNodeDrop, string sCopie, NSCutPaste* pCP)
{
	if ((NSTreeNode*) NULL == pNodeDrop)
		return ;

  int ligneDrop   = pNodeDrop->getLigne() ;
  int colonneDrop = pNodeDrop->getColonne() ;
  // int DecalageLigneFils = 1 ;

  NSCutPaste* pCutPaste = pCP ;
	if ((NSCutPaste*) NULL == pCutPaste)
	{
  	if      ((string("DRAG")  == sCopie) || (string("SYNCH") == sCopie))
    	pCutPaste = pContexte->getSuperviseur()->pBufDragDrop ;
		else if (string("COPIE") == sCopie)
    	pCutPaste = pContexte->getSuperviseur()->pBufCopie ;
	}

  if (string("SYNCH") == sCopie)
  {
    SynchronizeNode(pNodeDrop, pCutPaste) ;
    return ;
  }

  NSCutPastTLArray* pTLArray = pCutPaste->getTL() ;
  NSPatPathoArray   Patho(pContexte->getSuperviseur()) ;
  pCutPaste->initFromPatPatho(&Patho) ;

  // Make certain nodes'id are not duplicated
  //
  Patho.clearAllIDs() ;

  // DecalageLigneFils = Patho.Taille() ;

  int ligneMax   = ligneDrop ;
  int colonneMax = colonneDrop ;

  // On insère avant les fictifs
  GetMaxCoordonnees(pNodeDrop, &ligneMax, &colonneMax, true /*bJustReal*/) ;
  // 2010-05-03  // if (false == _NodeArray.empty())  //  _NodeArray.ShiftLines(ligneMax + 1, DecalageLigneFils) ;  DispatcherPatPatho(&Patho, pNodeDrop, 0, string(""), string("EXPAND"), pTLArray) ;
  SetDirty(true) ;

  Invalidate() ;
}

//-------------------------------------------------------------------
//supprimer un noeud et ses fils et mettre à jour les lignes et les colonnes
//-------------------------------------------------------------------
void
NSTreeWindow::SupprimerNoeud(NSTreeNode* pNSTreeNodeSuppr)
{
	if ((NSTreeNode*) NULL == pNSTreeNodeSuppr)
		return ;

try
{
	// int ligneDrag = pNSTreeNodeSuppr->getLigne();

	TuerFilsFictif(pNSTreeNodeSuppr);

	if (pNSTreeNodeSuppr->getTransfert())
	{
		Message Msg ;
    pNSTreeNodeSuppr->getTransfert()->ctrlNotification(BF_CHECKED, BF_DELETE, &Msg) ;
	}

	int cardinal = 1 ; //nb fils et petits fils
	DetruitFils(&cardinal, pNSTreeNodeSuppr) ;
	//enlever  pNSTreeNodeSuppr (et ses frères liés ) de pNodeArray
	_NodeArray.RemoveElement(pNSTreeNodeSuppr) ;

	//enlever  pNSTreeNodeSuppr de l'Array de son père
	if (pNSTreeNodeSuppr->getFatherNode())
		pNSTreeNodeSuppr->getFatherNode()->getVectSons()->RemoveElement(pNSTreeNodeSuppr) ;

	//frères liés
	iterNSTreeNode iter = pNSTreeNodeSuppr->getVectLinkedBros()->begin() ;
	while (pNSTreeNodeSuppr->getVectLinkedBros()->end() != iter)
	{
  	NSTreeNode* pNSTemp = (*iter) ;
    (*iter)->Delete() ;
    _NodeArray.RemoveElement((*iter)) ;
    if (pNSTreeNodeSuppr->getFatherNode())
    	pNSTreeNodeSuppr->getFatherNode()->getVectSons()->RemoveElement((*iter)) ;
    pNSTreeNodeSuppr->getVectLinkedBros()->RemoveElement((*iter)) ;
    delete pNSTemp ;
	}
	//base de donnéés : MiseAJour !!!!!!!!!!!!!!!!!!!!!!
	if (false == pNSTreeNodeSuppr->getVectLinkedBros()->empty())
		cardinal += pNSTreeNodeSuppr->getVectLinkedBros()->size() ;

	delete pNSTreeNodeSuppr ;

	//mettre à jour les coordonnées des autres NSTreeNodes
  // 2010-05-03
	// if (false == _NodeArray.empty())
  //   _NodeArray.ShiftLines(ligneDrag + 1, -cardinal) ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::SupprimerNoeud.", standardError, 0);
}
}

//-----------------------------------------------------------------------
//copier le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::CopierNode(NSTreeNode* pNSTreeNodeDrag)
{
  if ((NSTreeNode*) NULL == pNSTreeNodeDrag)
		return ;

	pNSTreeNodeDrag->CopyPatPatho("COPIE") ;
}

//-----------------------------------------------------------------------//coller le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::CollerNode(NSTreeNode* pNodeDrop)
{
try
{
	if ((NSTreeNode*) NULL == pNodeDrop)
		return ;

	if ((PositionDragAndDrop*) NULL == pPositionDragAndDrop)
		pPositionDragAndDrop = new PositionDragAndDrop(this, "DRAG_DROP_COPIE",
               		pContexte, pNSDLLModule) ;

	int execute = pPositionDragAndDrop->Execute() ;
	if (execute == IDOK)
	{
		if      (pPositionDragAndDrop->getDragType() == PositionDragAndDrop::dragBefore)
			DragAvant(pNodeDrop, "COPIE") ;
    else if (pPositionDragAndDrop->getDragType() == PositionDragAndDrop::dragAfter)
    	DragApres(pNodeDrop, "COPIE") ;
    else
    	DragFils(pNodeDrop, "COPIE") ;
	}
  
	pPositionDragAndDrop->resetDragType() ;
}
catch (...)
{
	erreur("Exception NSTreeWindow::CollerNode.", standardError, 0) ;
}
}

//-----------------------------------------------------------------------
//couper le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::CouperNode(NSTreeNode* pNSTreeNodeDrag)
{
	if ((NSTreeNode*) NULL == pNSTreeNodeDrag)
		return ;

	pNSTreeNodeDrag->CopyPatPatho("COPIE") ;
	SupprimerNoeud(pNSTreeNodeDrag) ;
}

//-----------------------------------------------------------------------//copier le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::CopyNode()
{
	TTreeNode node = GetSelection() ;
	NSTreeNode* pNSTreeNodeDrag = GetNSTreeNode(&node) ;
	if ((NSTreeNode*) NULL == pNSTreeNodeDrag)
		return ;

	CopierNode(pNSTreeNodeDrag) ;
}

//-----------------------------------------------------------------------//coller le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::PasteNode()
{
	TTreeNode node = GetSelection() ;
	NSTreeNode* pNodeDrop = GetNSTreeNode(&node) ;
  if ((NSTreeNode*) NULL == pNodeDrop)
  	return ;

	CollerNode(pNodeDrop) ;
  pContexte->getSuperviseur()->pBufCopie->vider() ;
}

//-----------------------------------------------------------------------
//couper le noeud pNSTreeNodeDrag (et ses fils )
//----------------------------------------------------------------------
void
NSTreeWindow::CutNode()
{
	TTreeNode node = GetSelection() ;
	NSTreeNode* pNSTreeNodeDrag = GetNSTreeNode(&node) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeDrag)
  	return ;

  CouperNode(pNSTreeNodeDrag) ;
}

//-----------------------------------------------------------------------//drag and drop ou copie avant le noeud destination
//sCopie vaut "DRAG" ou dans pSuper->pPatPathoDragDrop si sCopie
//vaut "COPIE"
//----------------------------------------------------------------------
void
NSTreeWindow::DragAvant(NSTreeNode* pNodeDrop, string sCopie, NSCutPaste* pCP)
{
	if ((NSTreeNode*) NULL == pNodeDrop)
		return ;

	NSCutPaste* pCutPaste = pCP ;
	if ((NSCutPaste*) NULL == pCutPaste)
	{
  	if      (sCopie == "DRAG")
    	pCutPaste = pContexte->getSuperviseur()->pBufDragDrop ;
		else if (sCopie == "COPIE")
    	pCutPaste = pContexte->getSuperviseur()->pBufCopie ;
    else
      return ;
	}

	NSTreeNode* pNSTreePere = pNodeDrop->getFatherNode() ;     //père du NSTreeNode correspondant
	int ligne = pNodeDrop->getLigne() ;
    // int colonne    			= pNodeDrop->getColonne() ;

	int ligneMax ;	int colonneMax = 1 ;

	NSCutPastTLArray* pTLArray = pCutPaste->getTL() ;
	NSPatPathoArray   Patho(pContexte->getSuperviseur()) ;
  pCutPaste->initFromPatPatho(&Patho) ;

  // Make certain nodes'id are not duplicated
  //
  Patho.clearAllIDs() ;

/*
	if      (sCopie == "DRAG")
	{
		Patho    = *(pContexte->getSuperviseur()->pBufDragDrop->pPatPatho) ;
		pTLArray = pContexte->getSuperviseur()->pBufDragDrop->pTextesLibres ;
	}
	else if (sCopie == "COPIE")
	{
		Patho    = *(pContexte->getSuperviseur()->pBufCopie->pPatPatho) ;
		pTLArray = pContexte->getSuperviseur()->pBufCopie->pTextesLibres ;
	}
*/

	// int Taille = Patho.Taille() ;

	if (pNSTreePere) //si le père existe
	{
		// iterNSTreeNode iter = pNSTreePere->VectFils.begin() ;
		NSTreeNode* pNSTreeFrere = TrouverGrandFrere(pNodeDrop, ligne) ;
		//on a trouvé le frère ainé le plus proche de pNodeDrop
		if (pNSTreeFrere)
		{
    	ligneMax = pNSTreeFrere->getLigne() ;
      GetMaxCoordonnees(pNSTreeFrere, &ligneMax, &colonneMax, true) ;
      //décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMax
      // 2010-05-03
      // if (false == _NodeArray.empty())
      //   _NodeArray.ShiftLines(ligneMax + 1, Taille) ;
      DispatcherPatPatho(&Patho, pNSTreeFrere, 0, "AVANTFRERE", "EXPAND", pTLArray);
    }
    else
    {
    	ligneMax = pNSTreePere->getLigne() ;
      //décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMax
      // 2010-05-03
      // if (false == _NodeArray.empty())
      //   _NodeArray.ShiftLines(ligneMax + 1, Taille) ;
      DispatcherPatPatho(&Patho, pNSTreePere, 0, "AVANTFILS", "EXPAND", pTLArray) ;
    }
  }
  else
  {
  	//
    //Touver le TTReenode dont le père est le root et qui soit le plus proche de
    // l'élément sélectionné (ainé)
    //
		NSTreeNode* pNSTreeFrere  = TrouverGrandFrereFilsRoot(pNodeDrop, ligne) ;
    //on a trouvé le frère ainé le plus proche de pNodeDrop
    if (pNSTreeFrere)
    {
    	ligneMax   = pNSTreeFrere->getLigne() ;
      GetMaxCoordonnees(pNSTreeFrere, &ligneMax, &colonneMax, true) ;
      //décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMax
      // 2010-05-03
      // if (false == _NodeArray.empty())
      //   _NodeArray.ShiftLines(ligneMax + 1, Taille) ;
      DispatcherPatPatho(&Patho, pNSTreeFrere, 0, "AVANTFRERE", "EXPAND", pTLArray) ;
    }
    else
    {
    	TTreeNode root = GetRoot() ;
      //décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMax
      // 2010-05-03
      // if (false == _NodeArray.empty())
      //   _NodeArray.ShiftLines(0, Taille) ;
      DispatcherPatPatho(&Patho, 0, 0, "Debut", "EXPAND", pTLArray) ;
    }
  }
  SetDirty(true) ;
}

//-----------------------------------------------------------------------
//sCopie vaut "DRAG" ou dans pSuper->pPatPathoDragDrop si sCopie
//vaut "COPIE"
//----------------------------------------------------------------------
void
NSTreeWindow::DragApres(NSTreeNode* pNodeDrop, string sCopie, NSCutPaste* pCP)
{
	int ligneMax ;
	int colonneMax = 1 ;

  NSCutPaste* pCutPaste = pCP ;
	if ((NSCutPaste*) NULL == pCutPaste)
	{
  	if      (string("DRAG")  == sCopie)
    	pCutPaste = pContexte->getSuperviseur()->pBufDragDrop ;
		else if (string("COPIE") == sCopie)
    	pCutPaste = pContexte->getSuperviseur()->pBufCopie ;
    else
      return ;
	}

	NSCutPastTLArray* pTLArray = pCutPaste->getTL() ;
	NSPatPathoArray Patho(pContexte->getSuperviseur()) ;
  pCutPaste->initFromPatPatho(&Patho) ;

  if (pNodeDrop)
	{
		// int Taille = Patho.Taille() ;		ligneMax   = pNodeDrop->getLigne() ;
		GetMaxCoordonnees(pNodeDrop, &ligneMax, &colonneMax, true) ;
		//décaler les NSTreeNodes qui existent et dont la ligne est supérieure à ligneMax
    // 2010-05-03
    // _NodeArray.ShiftLines(ligneMax + 1, Taille) ;
	}

  // Make certain nodes'id are not duplicated
  //
  Patho.clearAllIDs() ;

	DispatcherPatPatho(&Patho, pNodeDrop, 0, "AVANTFRERE", "EXPAND", pTLArray) ;	SetDirty(true) ;
}

//-----------------------------------------------------------------------//drag and drop ou copie en dessous du noeud destination
//sCopie vaut "DRAG" ou dans pSuper->pPatPathoDragDrop si sCopie
//vaut "COPIE"
//----------------------------------------------------------------------
void
NSTreeWindow::DragFils(NSTreeNode* pNodeDrop, string sCopie, NSCutPaste* pCP)
{
	CopyBranch(pNodeDrop, sCopie, pCP) ;
	// SetDirty(true) ;  Dirty is set inside CopyBranch if changes were made
}


// Content of the Cut&Paste object will update pRefNode and sons
//
void
NSTreeWindow::SynchronizeNode(NSTreeNode* pRefNode, NSCutPaste* pCP)
{
  if (((NSTreeNode*) NULL == pRefNode) ||
      ((NSCutPaste*) NULL == pCP) || pCP->empty())
    return ;

  // Getting cut & paste patho
  //
  NSPatPathoArray Patho(pContexte->getSuperviseur()) ;
  pCP->initFromPatPatho(&Patho) ;

  if (Patho.empty())
    return ;

  // Root concept
  //
  PatPathoIter itFirstNode = Patho.begin() ;

  // If patpatho's root is not equal to node's content, then exit
  //
  string sRootConcept = (*itFirstNode)->getLexique() ;
  if (pRefNode->getEtiquette() != sRootConcept)
    return ;

  // Check if
  //
  NSCutPaste tmpCutPaste(pContexte->getSuperviseur()) ;
  pRefNode->formerPatPatho(&tmpCutPaste) ;

  NSPatPathoArray tmpCutPastePatho(pContexte->getSuperviseur()) ;
  tmpCutPaste.initFromPatPatho(&tmpCutPastePatho) ;

  if (Patho.estEgal(&tmpCutPastePatho))
    return ;

  // In order to distinguish processed and not processed son nodes, we mark
  // all nodes as 'proposed' Those who will still be in this state when
  // process is done are deleted
  //
  SynchroMarkNodes(pRefNode) ;

  // Synchonize
  //
  NSPatPathoArray subPatho(pContexte->getSuperviseur()) ;
  Patho.ExtrairePatPatho(itFirstNode, &subPatho) ;

  synchroNode(pRefNode, &subPatho) ;

  // Now, we delete all nodes that are still marked as "proposed"
  //
  SynchroDeleteNodes(pRefNode) ;

  SetDirty(true) ;
  Invalidate() ;
}

// Mark all sons of a node as "proposed"
//
void
NSTreeWindow::SynchroMarkNodes(NSTreeNode* pRefNode)
{
  if ((NSTreeNode*) NULL == pRefNode)
    return ;

  NSFilsTreeNodeArray* pSonsArray = pRefNode->getVectSons() ;
  if (pSonsArray->empty())
    return ;

  iterNSTreeNode itNodes = pSonsArray->begin() ;
  for ( ; pSonsArray->end() != itNodes ; itNodes++)
  {
    (*itNodes)->SetProposed(true) ;
    SynchroMarkNodes(*itNodes) ;
  }
}

void
NSTreeWindow::synchroNode(NSTreeNode* pRefNode, NSPatPathoArray* pPpt)
{
  if (((NSTreeNode*) NULL == pRefNode) || ((NSPatPathoArray*) NULL == pPpt))
    return ;

  // Get element's path
  //
  // string sPath = pPpt->donneCheminItem(pptIt, intranodeSeparationMARK) ;

  // Fill vector
  //
/*
  ClasseStringVector Vect ;
	DecomposeChaine(&sPath, &Vect, string(1, cheminSeparationMARK), string(1, intranodeSeparationMARK)) ;

  if (Vect.empty())
		return ;
*/

  pRefNode->SetProposed(false) ;

  NSFilsTreeNodeArray* pSonsArray = pRefNode->getVectSons() ;

  // No existing son... just add patho
  //
  if (pSonsArray->empty())
  {
    NSCutPaste tmpCutPaste(pContexte->getSuperviseur()) ;
    tmpCutPaste.setPatPatho(pPpt) ;
    DragFils(pRefNode, string("DRAG"), &tmpCutPaste) ;
    return ;
  }

  PatPathoIter pptIt = pPpt->begin() ;
  while (pptIt && (pPpt->end() != pptIt))
  {
    string sPptInfoLabel = (*pptIt)->getNodeLabel() ;

    iterNSTreeNode itNodes = pSonsArray->begin() ;
    for ( ; (pSonsArray->end() != itNodes) &&
                        ((*itNodes)->getLabel() != sPptInfoLabel) ; itNodes++) ;

    // found a node with same content
    //
    if (pSonsArray->end() != itNodes)
    {
      (*itNodes)->SetProposed(false) ;

      // Not same value, change it (maybe check unit)
      //
      if ((*pptIt)->getComplement() != (*itNodes)->getContenuTransfert())
        (*itNodes)->SetComplement((*pptIt)->getComplement()) ;

      // If not a leaf, iterate
      //
      PatPathoIter itFils =	pPpt->ChercherPremierFils(pptIt) ;
      if ((NULL != itFils) && (pPpt->end() != itFils))
      {
        NSPatPathoArray subPatho(pContexte->getSuperviseur()) ;
        pPpt->ExtrairePatPatho(pptIt, &subPatho) ;

        synchroNode(*itNodes, &subPatho) ;
      }
    }

    // No node with same content, add it
    //
    else
    {
      NSPatPathoArray subPatho(pContexte->getSuperviseur()) ;
      pPpt->ExtrairePatPathoWithRoot(pptIt, &subPatho) ;

      NSCutPaste tmpCutPaste(pContexte->getSuperviseur()) ;
      tmpCutPaste.setPatPatho(&subPatho) ;
      DragFils(pRefNode, string("DRAG"), &tmpCutPaste) ;
    }

    pptIt	= pPpt->ChercherFrereSuivant(pptIt) ;
  }
}

void
NSTreeWindow::SynchroDeleteNodes(NSTreeNode* pRefNode)
{
  if ((NSTreeNode*) NULL == pRefNode)
    return ;

  NSFilsTreeNodeArray* pSonsArray = pRefNode->getVectSons() ;
  if (pSonsArray->empty())
    return ;

  iterNSTreeNode itNodes = pSonsArray->begin() ;
  for ( ; pSonsArray->end() != itNodes ; )
  {
    // Delete marked sons before deleting node
    //
    SynchroDeleteNodes(*itNodes) ;

    if ((*itNodes)->isProposed())
      SupprimerNoeud(*itNodes) ;
    else
      itNodes++ ;
  }
}

// Content of the Cut&Paste object will update sons of pFatherNode
//

/*
void
NSTreeWindow::SynchronizeNode(NSTreeNode* pFatherNode, NSCutPaste* pCP)
{
  if ((NULL == pFatherNode) || (NULL == pCP) || (pCP->empty()))
    return ;

  if (pCP->pPatPatho->empty())
    return ;

  // If pFatherNode has no son, then just paste as sons
  //
  if (pFatherNode->getVectSons()->empty())
  {
    DragFils(pFatherNode, string("DRAG"), pCP) ;
    return ;
  }

  bool bModified = false ;

  // Getting cut & paste patho
  //
  NSPatPathoArray Patho(pContexte->getSuperviseur()) ;
  Patho = *(pCP->pPatPatho) ;

  NSCutPastTLArray* pTLArray = pCP->pTextesLibres ;

  // Root concept
  //
  string sRootConcept = (*(Patho.begin()))->getLexique() ;

  // In order to distinguish processed and not processed son nodes, we mark
  // them all as 'proposed' Those who will still be in this state when
  // process is done are deleted
  //
  // First, remove already existing proposed nodes
  //
  bool bExistingRootNodes   = false ;
  bool bDeleteProposedNodes = true ;
  while (bDeleteProposedNodes)
  {
    iterNSTreeNode itNodes = pFatherNode->getVectSons()->begin() ;
    for ( ; pFatherNode->getVectSons()->end() != itNodes ; itNodes++)
      if ((*itNodes)->getEtiquette() == sRootConcept)
      {
        if ((*itNodes)->isProposed())
          break ;
        else
          bExistingRootNodes = true ;
      }

    if (pFatherNode->getVectSons()->end() != itNodes)
    {
      pFatherNode->getVectSons()->RemoveElement(*itNodes) ;
      bModified = true ;
      if (pFatherNode->getVectSons()->empty())
      {
        DragFils(pFatherNode, string("DRAG"), pCP) ;
        return ;
      }
    }
    else
      bDeleteProposedNodes = false ;
  }
  //
  // If we didn't find any properly rooted node, just drag
  //
  if (false == bExistingRootNodes)
  {
    DragFils(pFatherNode, string("DRAG"), pCP) ;
    return ;
  }
  //
  // Then mark all properly rooted nodes as "proposed"
  //
  iterNSTreeNode itNodes = pFatherNode->getVectSons()->begin() ;
  for ( ; pFatherNode->getVectSons()->end() != itNodes ; itNodes++)
    if ((*itNodes)->getEtiquette() == sRootConcept)
    {
      bExistingRootNodes = true ;
      (*itNodes)->SetProposed(true) ;
    }

  // int ligneDrop   = pFatherNode->getLigne() ;
  // int colonneDrop = pFatherNode->getColonne() ;

  NSFilsTreeNodeArray *pVectFils = pFatherNode->getVectSons() ;

  // The patho coming from cut & paste may contain multiple root nodes,
  // we check them one by one to see if they are already there or not
  //
  // The rule is to synchronize by taking what comes from cut & paste as
  // the new content (updating similar and deleting missing)
  //

  // In first scan, we treat unchanged pathos
  //
  PatPathoIter pptIt = Patho.begin() ;
  while (pptIt && (false == Patho.empty()) && (Patho.end() != pptIt))
  {
    PatPathoIter pptItFrere = Patho.ChercherFrere(pptIt) ;

    NSPatPathoArray elementPatho(pContexte->getSuperviseur()) ;
    NSPatPathoArray *pCurrentPatho = (NSPatPathoArray *) 0 ;

    // If there is a single root, we consider the whole Patho
    //
    if (Patho.end() == pptItFrere)
      pCurrentPatho = &Patho ;
    //
    // In case there are multiple roots, we take the Patho of current one
    //
    else
    {
      Patho.ExtrairePatPathoWithRoot(pptIt, &elementPatho) ;
      pCurrentPatho = &elementPatho ;
    }

    // Scan son nodes that are still marked as "proposed" to see if we can
    // find the same patho
    //
    itNodes = pVectFils->begin() ;
    for ( ; pVectFils->end() != itNodes ; itNodes++)
    {
      if (((*itNodes)->getEtiquette() == sRootConcept) && ((*itNodes)->isProposed()))
      {
        NSCutPaste tmpCutPaste(pContexte->getSuperviseur()) ;
        (*itNodes)->formerPatPatho(&tmpCutPaste) ;
        if (pCurrentPatho->estEgal(tmpCutPaste.pPatPatho))
          break ;
      }
    }
    //
    // We have found the same patpatho. We mark the node as "not proposed" and
    // we delete the patpatho from patho
    //
    if (pVectFils->end() != itNodes)
    {
      (*itNodes)->SetProposed(false) ;
      Patho.SupprimerItem(pptIt) ;
    }

    pptIt = pptItFrere ;
  }

  // Now, we delete all nodes that are still marked as "proposed"
  //
  bDeleteProposedNodes = true ;
  while (bDeleteProposedNodes)
  {
    itNodes = pVectFils->begin() ;
    for ( ; pVectFils->end() != itNodes ; itNodes++)
      if (((*itNodes)->getEtiquette() == sRootConcept) && ((*itNodes)->isProposed()))
        break ;

    if (pVectFils->end() != itNodes)
    {
      pVectFils->RemoveElement(*itNodes) ;
      bModified = true ;
    }
    else
      bDeleteProposedNodes = false ;
  }

  // In second scan, we add modified pathos
  //
  if (false == Patho.empty())
  {
    PatPathoIter pptIt = Patho.begin() ;
    while (pptIt && (Patho.end() != pptIt))
    {
      PatPathoIter pptItFrere = Patho.ChercherFrere(pptIt) ;

      NSPatPathoArray elementPatho(pContexte->getSuperviseur()) ;
      NSPatPathoArray *pCurrentPatho = (NSPatPathoArray *) 0 ;

      // If there is a single root, we consider the whole Patho
      //
      if (Patho.end() == pptItFrere)
        pCurrentPatho = &Patho ;
      //
      // In case there are multiple roots, we take the Patho of current one
      //
      else
      {
        Patho.ExtrairePatPathoWithRoot(pptIt, &elementPatho) ;
        pCurrentPatho = &elementPatho ;
      }

      DispatcherPatPatho(pCurrentPatho, pFatherNode, 0, "", "EXPAND", pTLArray) ;
      bModified = true ;

      pptIt = pptItFrere ;
    }
  }

  if (bModified)
  {
    SetDirty(true) ;
    Invalidate() ;
  }
}
*/

int
NSTreeWindow::donneIndexInteret(char cInteret)
{
	if      ('A' == cInteret)
		return 0 ;
	else if ('B' == cInteret)
		return 1 ;
	else if ('C' == cInteret)
		return 2 ;
	else if ('D' == cInteret)
		return 3 ;
	else if ('E' == cInteret)
		return 4 ;

	return 0 ;
}

bool
NSTreeWindow::canWeClose()
{
	return true ;
}

bool
NSTreeWindow::isRealNode(NSTreeNode* pNSTreeNode)
{
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return false ;

  if ((pNSTreeNode->estFictifPur()) || (pNSTreeNode->isProposed()))
    return false ;

  return true ;
}

string
NSTreeWindow::getHandleAsString()
{
  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  return string(szThis) ;
}

string
NSTreeWindow::getEditDicoHandleAsString()
{
  if ((NSEditDico*) NULL == _pEditDico)
    return string("NULL") ;

  char szDico[20] ;
  sprintf(szDico, "%p", _pEditDico->HWindow) ;
  return string(szDico) ;
}

// fin de nstrewi.cpp
///////////////////////////////////////////////////

