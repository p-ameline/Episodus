// NSCSVUE.CPP		Document/Vues Consultation// -----------------------------------------

#define __NSCSVUE_CPP

#include <owl\owlpch.h>
#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsbb\nspatpat.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nshistdo.h"
#include "nautilus\ns_html.h"
#include "dcodeur\nsdkd.h"
#include "nautilus\nsadmiwd.h"
#include "nsoutil\nsfilgui.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nstrnode.h"
#include "nsbb\nsbbsmal.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"

// Table de réponses de la classe NSCsVue
////////////////////////////////////////////////////////////////
DEFINE_RESPONSE_TABLE2(NSCsVue, TView, NSTreeWindow)
// DEFINE_RESPONSE_TABLE2(NSCsVue, NSMUEView, NSTreeWindow)
  EV_VN_ISWINDOW,
  EV_WM_SETFOCUS,
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  EV_WM_QUERYENDSESSION,

  EV_WM_SIZE,
  EV_WM_SYSCOMMAND,

  EV_COMMAND(CM_COMPOSE, 	  CmComposer),
  EV_COMMAND(CM_ENREGISTRE, CmEnregistre),
  EV_COMMAND(CM_IMPRIME, 	  CmPublier),
  EV_COMMAND(CM_VISUAL, 	  CmVisualiser),
  EV_COMMAND(CM_FILECLOSE,  EvClose),

  EV_COMMAND(IDC_COMPTA_CR, Compta),

  EV_COMMAND(CM_1P,         Cm1Plus),
  EV_COMMAND(CM_2P,         Cm2Plus),
  EV_COMMAND(CM_3P,         Cm3Plus),
  EV_COMMAND(CM_4P,         Cm4Plus),

  EV_COMMAND_ENABLE(CM_1P,  Ce1Plus),
  EV_COMMAND_ENABLE(CM_2P,  Ce2Plus),
  EV_COMMAND_ENABLE(CM_3P,  Ce3Plus),
  EV_COMMAND_ENABLE(CM_4P,  Ce4Plus),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// Constructeur NSCsVue
// -----------------------------------------------------------------------------
NSCsVue::NSCsVue(NSCSDocument& doc, TWindow *parent)
/*    :NSMUEView(doc.pContexte, &doc),
     NSTreeWindow(parent, doc.pContexte, GetNextViewId(), 0, 0, 0, 0) */
    :TView(doc),
    _pDoc(&doc),
    NSTreeWindow(parent, doc.pContexte, GetNextViewId(), 0, 0, 0, 0)
{
try
{
  NSSuper *pSuper = NSTreeWindow::pContexte->getSuperviseur() ;
  _pMUEViewMenu = (TMenuDescr*) 0 ;

  TMyApp* pMyApp = pSuper->getApplication() ;
  pMyApp->setMenu(string("menubar"), &_hAccelerator) ;

  _pBigBoss      = (NSSmallBrother*) 0 ;
  _pToolBar      = (OWL::TControlBar*) 0 ;
  _bConsultVide  = true ;
  _bSetupToolBar = true ;
}
catch (...)
{
	erreur("Exception NSCsVue ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Destructeur NSCsVue
// -----------------------------------------------------------------------------
NSCsVue::~NSCsVue()
{
  string ps = string("Deleting NSCsVue ") ;
  if (_pDoc && _pDoc->GetTitle())
    ps += string("\"") + string(_pDoc->GetTitle()) + string("\"") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	if (_pBigBoss)
		delete _pBigBoss ;

  if (_pMUEViewMenu)
  	delete _pMUEViewMenu ;

  // Useless. Now done by bigboss destructor
  //
	// deleter les BBItem crées dans la consultation	// NSTreeWindow *pNSTreeWindow = this ;
	// pNSTreeWindow->LibereBBitemLanceur() ;

  if (0 != _hAccelerator)
  {
    DestroyAcceleratorTable(_hAccelerator) ;
    _hAccelerator = 0 ;
  }
}

// -----------------------------------------------------------------------------
// Fonction: 	NSCsVue::SetupWindow()
// -----------------------------------------------------------------------------
void
NSCsVue::SetupWindow()
{
  string sLang = string("") ;
  if (NSTreeWindow::pContexte)
    sLang = NSTreeWindow::pContexte->getUserLanguage() ;

  // titre du document
  if (_pDoc->_pDocInfo)
  {
    // Affiche le titre du document
    NSTreeWindow::SetDocTitle(_pDoc->GetTitle(), 0) ;
  }
  else   //récuperer le type document
  {
    if (string("") != _pDoc->_sTypeDocument)
    {
      string sTitle ;
      NSTreeWindow::pContexte->getDico()->donneLibelle(sLang, &_pDoc->_sTypeDocument, &sTitle) ;
      _pDoc->SetTitle(sTitle.c_str()) ;
      NSTreeWindow::SetDocTitle(sTitle.c_str(), 0) ;
    }
  }

  lanceConsult() ;
  NSTreeWindow::SetupWindow() ;

  NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  // Dispatcher la patpatho
  if (pCSDoc)
  {
    NSPatPathoArray* pDocPpt = pCSDoc->getPatPathoPointer() ;
    if (pDocPpt)
      NSTreeWindow::DispatcherPatPatho(pDocPpt, (NSTreeNode*) 0, (int*) 0, string("")) ;
  }

  if (isSynthesis())
    SetWindowPosSynthse() ;

  if (false == _pBigBoss->getPatPatho()->empty())
    _pBigBoss->MetTitre() ;
}

TWindow
*NSCsVue::GetWindow()
{
	return (TWindow *) this ;
}

bool
NSCsVue::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  return _hAccelerator ? ::TranslateAccelerator(GetHandle(), _hAccelerator, &msg) : false ;
}

// -----------------------------------------------------------------------------
// Fonction: 	NSCsVue::lanceConsult()
// -----------------------------------------------------------------------------
void
NSCsVue::lanceConsult()
{
try
{
	NSTreeWindow *pNSTreeWindow = this ;
	_pBigBoss = new NSSmallBrother(NSTreeWindow::pContexte, _pDoc->getPatPathoPointer()) ;
	_pBigBoss->setMotherWindow(TView::GetWindow()) /*pFenetre*/ ;
	_pBigBoss->lanceConsult(_pDoc->_sTypeDocument, pNSTreeWindow) ;
}
catch (...)
{
	erreur("Exception NSCsVue::lanceConsult", standardError, 0) ;
}
}

void
NSCsVue::CmEnregistre()
{
  NSTreeWindow *pNSTreeWindow = this ;

  // Rebuild patpatho
  //
  pNSTreeWindow->okFermerDialogue() ;

  // Refresh BB patpatho into treeWindow
  //
  pNSTreeWindow->EnregistrePatPatho(_pBigBoss) ;

//  if (!pBigBoss->getPatPatho()->empty())
//  	pBigBoss->MetTitre() ;

  // Add hidden root concept
  //
  _pBigBoss->MetTitre() ;

  NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  if ((NSCSDocument*) NULL == pCSDoc)
    return ;

  bool bNewDoc = false ;
  if ((NSDocumentInfo*) NULL == pCSDoc->_pDocInfo)
    bNewDoc = true ;

  // Record information
  //
  if (pCSDoc->enregistre())
  {
    NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
    _pDoc->initFromPatPatho(&DocPatho) ;

    SetNodesIdFromPatho(&DocPatho) ;

    _pDoc->SetDirty(false) ;
    pNSTreeWindow->SetDirty(false) ;

    if (bNewDoc && (NSTreeWindow::pContexte->getSuperviseur()->isComptaAuto()) && (pCSDoc->ComptaAuto()))
    {
      string sCaption = string("Message ") + NSTreeWindow::pContexte->getSuperviseur()->getAppName().c_str() ;
      int retVal = NSTreeWindow::MessageBox("Il n'existe pas de fiche comptable pour ce document. Voulez-vous lancer la comptabilité ?", sCaption.c_str(), MB_YESNO) ;
      if (IDYES == retVal)
        Compta() ;
    }
  }
  else
    _pDoc->SetDirty(true) ;
}

void
NSCsVue::CmPublier()
{
try
{
	NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  if (NULL == pCSDoc)
    return ;

	if (pCSDoc->_pHtmlCS)
		delete pCSDoc->_pHtmlCS ;

	pCSDoc->_pHtmlCS = new NSHtml(tCS) ; 	// htmlNode racine du html de consultation

	InscrireHtml(pCSDoc->_pHtmlCS) ;

	if (false == _bConsultVide)
		pCSDoc->Publier() ;
	else
		NSTreeWindow::MessageBox("Le document est vide.") ;
}
catch (...)
{
	erreur("Exception NSCsVue::CmPublier", standardError, 0) ;
}
}

void
NSCsVue::CmVisualiser()
{
try
{
	NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  if (NULL == pCSDoc)
    return ;

	if (pCSDoc->_pHtmlCS)
		delete pCSDoc->_pHtmlCS ;

	pCSDoc->_pHtmlCS = new NSHtml(tCS) ; 	// htmlNode racine du html de consultation

	InscrireHtml(pCSDoc->_pHtmlCS) ;

	if (false == _bConsultVide)
		pCSDoc->Visualiser() ;
	else
		NSTreeWindow::MessageBox("Le document de synthese est vide.") ;
}
catch (...)
{
	erreur("Exception NSCsVue::CmVisualiser", standardError, 0) ;
}
}

void
NSCsVue::InscrireHtml(NSHtml *pHtml)
{
	if (_NodeArray.empty())
		return ;

  NSHtml htmlNode(tConsult) ;
  int    iCpt = 0 ;

	for (iterNSTreeNode i = _NodeArray.begin() ; _NodeArray.end() != i ; i++)
	{
		// on prend les éléments de premier niveau
		if ((NSTreeNode*) NULL == (*i)->getFatherNode())
		{
			iCpt++ ;

			if ((*i)->getControl())
			{
				// on "skippe" les noeuds masqués
				if ((*i)->getEtiquette() == string("900001"))
					continue ;

				htmlNode.setText(texteHtml((*i)->GetText())) ;
				if (false == (*i)->getVectLinkedBros()->empty())
				{
					for (iterNSTreeNode j = (*i)->getVectLinkedBros()->begin() ; (*i)->getVectLinkedBros()->end() != j ; j++)
						htmlNode.addText(texteHtml(string(" ") + (*j)->GetText())) ;
				}
				pHtml->addSon(new NSHtml(htmlNode)) ;

				InscrireFils(*i, pHtml->GetLastSon()) ;

				_bConsultVide = false ;
			}
			else if (1 == iCpt)
			{
				_bConsultVide = true ;
				return ;
      }
    }
  }
}

void
NSCsVue::InscrireFils(NSTreeNode *pNode, NSHtml *pHtml)
{
  if (((NSTreeNode*) NULL == pNode) || (pNode->getVectSons()->empty()) || ((NSHtml*) NULL == pHtml))
    return ;

  iterNSTreeNode itSon = pNode->getVectSons()->begin() ;

  // s'il existe des fils "visibles", c'est à dire que l'arbre des fils est développé
  //
  NS_CLASSLIB::TRect rectItem ;
  if (false == (*itSon)->GetItemRect(rectItem))
    return ;

  NSHtml htmlNode(tConsult) ;

  for (iterNSTreeNode i = pNode->getVectSons()->begin() ; pNode->getVectSons()->end() != i ; i++)
  {
    if ((*i)->getControl())
    {
      // on "skippe" les noeuds masqués
      if ((*i)->getEtiquette() == string("900001"))
        continue ;

      htmlNode.setText(texteHtml((*i)->GetText())) ;
      if (false == (*i)->getVectLinkedBros()->empty())
      {
        for (iterNSTreeNode j = (*i)->getVectLinkedBros()->begin() ; (*i)->getVectLinkedBros()->end() != j ; j++)
          htmlNode.addText(texteHtml(string(" ") + (*j)->GetText())) ;
      }
      pHtml->addSon(new NSHtml(htmlNode)) ;
      InscrireFils(*i, pHtml->GetLastSon()) ;
    }
  }
}

// -----------------------------------------------------------------------------
// Libération de la dernière barre d'outils créee
// -----------------------------------------------------------------------------
void
NSCsVue::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
  activateMUEViewMenu() ;

	// NSTreeWindow  *pNSTreeWindow = this ;
  TMyApp *pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
  NSTreeWindow::Initialise(hWndLostFocus) ;
  // NSTreeWindow::EvSetFocus(hWndLostFocus) ;

  pMyApp->setMenu(string("menubar"), &_hAccelerator) ;

  if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
  {
  	SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }
}

void
NSCsVue::SetupToolBar()
{
	TMyApp *pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
  pMyApp->FlushControlBar() ;

  // on met un bouton compta sauf pour la synthèse
  if (false == isSynthesis())
  {
    pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_COMPTA_CR, IDC_COMPTA_CR, TButtonGadget::Command)) ;
    pMyApp->getSecondaryControlBar()->Insert(*new NSFlatSeparatorGadget) ;
  }

  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_1P, CM_1P, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_2P, CM_2P, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_3P, CM_3P, TButtonGadget::Command)) ;
  pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(CM_4P, CM_4P, TButtonGadget::Command)) ;

  pMyApp->getSecondaryControlBar()->LayoutSession() ;
}

// -----------------------------------------------------------------------------
// Appel automatique d'une fiche Compt à partir d'une consultation
// -----------------------------------------------------------------------------
void
NSCsVue::Compta()
{
try
{
	if (NULL == _pDoc->getRootPathoInfo())
	{
    erreur("Vous devez enregistrer le document avant de le comptabiliser.", warningError, 0, NSTreeWindow::GetHandle()) ;
    return ;
  }

  if (NULL == _pDoc->getBigBoss())
    _pDoc->setBigBoss(new NSSmallBrother(NSTreeWindow::pContexte, _pDoc->getPatPathoPointer())) ;

  // date de l'examen
  NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
  _pDoc->initFromPatPatho(&DocPatho) ;

  string sDateDoc = DocPatho.GetDocDate() ;

  if (string("") == sDateDoc)
  {
    if (_pDoc->_pDocInfo) // on prend la date de création (heure = 0)
    	sDateDoc = _pDoc->_pDocInfo->getCreDate() ;
    else // on prend la date et l'heure du jour
    {
    	NVLdVTemps tpsNow ;
    	tpsNow.takeTime() ;
    	sDateDoc = tpsNow.donneDate() ;
    }
  }

  string sDate  = string(sDateDoc, 0, 8) ;
	string sHeure = string("0000") ;
	if (strlen(sDateDoc.c_str()) >= 12)
		sHeure = string(sDateDoc, 8, 4) ;

  NSComptInfo ComptInfo ;

	strcpy(ComptInfo._Donnees.date, sDate.c_str()) ;
	strcpy(ComptInfo._Donnees.heure, sHeure.c_str()) ;

  NSFse1610Info Fse1610Info ;

  // on cale la date de la Fse1610 sur celle de la fiche Compt
	strcpy(Fse1610Info._Donnees.date, ComptInfo._Donnees.date) ;
	strcat(Fse1610Info._Donnees.date, ComptInfo._Donnees.heure) ;

  // Externe / Hospitalisé
  string sExtHosp = string("") ;
  DocPatho.CheminDansPatpatho(_pDoc->getBigBoss(), "LADMI/LCONT", &sExtHosp) ;
  if (string("") != sExtHosp)
    sExtHosp = string(sExtHosp, 0, 5) ;

  string sExam = string("") ;
  string sSyn  = string("") ;

  string sCode = (*(DocPatho.begin()))->getLexique() ;
  if (string("") != sCode)
  {
    sExam = string(sCode, 0, 5) ;
    sSyn  = string(sCode, 5, 1) ;
  }

  strcpy(ComptInfo._Donnees.examen, sExam.c_str()) ;
  strcpy(ComptInfo._Donnees.synonyme, sSyn.c_str()) ;
  strcpy(ComptInfo._Donnees.contexte, sExtHosp.c_str()) ;

  NSPersonInfo personInfo(pContexte->getSuperviseur(), pContexte, pContexte->getPatient()->getNss()) ;

  // NSComptaPatient *pCompta = new NSComptaPatient(NSTreeWindow::pContexte, (NSPatInfo *) NSTreeWindow::pContexte->getPatient()) ;
  NSComptaPatient Compta(NSTreeWindow::pContexte, &personInfo) ;
  Compta.CmFicheCompt(&ComptInfo, &Fse1610Info) ;
}
catch (...)
{
	erreur("Exception NSCsVue::Compta", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// fermeture du document
// -----------------------------------------------------------------------------
void
NSCsVue::EvClose()
{
}

// -----------------------------------------------------------------------------
// Fonction de Windows récupérée pour le changement de taille dans SIZE_RESTORED
// Finalement le bug persistait donc on revient à TWindow::EvSize
// -----------------------------------------------------------------------------
void
NSCsVue::EvSize(uint sizeType, NS_CLASSLIB::TSize& size)
{
  NSTreeWindow::EvSize(sizeType, size) ;
}

// -----------------------------------------------------------------------------
// Fonction que l'on n'arrive pas à intercepter
// -----------------------------------------------------------------------------
void
NSCsVue::EvSysCommand(uint cmdType, NS_CLASSLIB::TPoint& point)
{
  switch (cmdType & 0xFFF0)
  {
    case SC_MINIMIZE  : break ;
    case SC_MAXIMIZE  : break ;
    case SC_SIZE      : break ;
    default           : NSTreeWindow::DefaultProcessing() ;
  }
}

// -----------------------------------------------------------------------------
// mettre à jour la base des registres s'il y a modification dans les postition
// des fiches de Synthèse et historique
// -----------------------------------------------------------------------------
void
NSCsVue::EvDestroy()
{
  if (isSynthesis())
    EnregistrePosSynthse() ;
}

// -----------------------------------------------------------------------------
// Fermeture de Windows
// -----------------------------------------------------------------------------
bool
NSCsVue::EvQueryEndSession()
{
  return NSTreeWindow::EvQueryEndSession() ;
}

// -----------------------------------------------------------------------------
// fixer la position de la Synthèse
// -----------------------------------------------------------------------------
void
NSCsVue::SetWindowPosSynthse()
{
  // fixer la position de la fiche de synthèse selon les valeurs dans UTILISAT.DB
  // Parent->Show(SW_HIDE);

  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty(string("Synthesis")) ;
  if (pWinProp)
  {
    NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

    //
    // fixer la nouvelle position
    // (on ne tient pas compte de la taille, vu le probleme pour restaurer
    //  une fenetre TView,TWindow mise en icone)
    //
    string ps = string("NSCsVue::SetWindowPosSynthse, SetWindowPos with rect ") + pWinProp->getPosAsString() ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	  Parent->SetWindowPos(0, targetRect.Left(), targetRect.Top(), targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  }
  else
  {
    string ps = string("NSCsVue::SetWindowPosSynthse, NSWindowProperty not found for function Synthesis") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }

  NSTreeWindow::Parent->Show(SW_SHOWNORMAL) ;

  /* ********************************
  if      (sTaille == "I")
    Parent->Show(SW_SHOWMINIMIZED) ;
  else if (sTaille == "Z")
    Parent->Show(SW_SHOWMAXIMIZED) ;
  else
    Parent->Show(SW_SHOWNORMAL) ;
  ********************************* */
}

// -----------------------------------------------------------------------------
// enregistrer la position de la Synthèse dans la base Utilisat.db
// -----------------------------------------------------------------------------
void
NSCsVue::EnregistrePosSynthse()
{
	if (false == pContexte->getUtilisateur()->isEnregWin())
  	return ;

  NS_CLASSLIB::TRect rectDlg = NSTreeWindow::Parent->GetWindowRect() ; //coordonnées par % à l'écran

  NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
  NSTreeWindow::Parent->Parent->ScreenToClient(point) ;
  int X = point.X() ;
  int Y = point.Y() ;
  int W = rectDlg.Width() ;
  int H = rectDlg.Height() ;

	if (NSTreeWindow::Parent->IsIconic())
  	return ;

	string sUserId = NSTreeWindow::pContexte->getUtilisateur()->getNss() ;
  NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
  NSWindowProperty wndProp ;
  wndProp.setX(X) ;
  wndProp.setY(Y) ;
  	// wndProp->iW = X + W;
  	// wndProp->iH = Y + H;
  wndProp.setW(W) ;
  wndProp.setH(H) ;
  wndProp.setActivity(NSWindowProperty::undefined) ;
  wndProp.setFunction("Synthesis") ;

  NSTreeWindow::pContexte->getUtilisateur()->saveWindowProperty(sUserId, NSTreeWindow::pContexte->PathName("FGLO"), &wndProp, false) ;
  NSTreeWindow::pContexte->getUtilisateur()->saveWindowProperty(sUserId, NSTreeWindow::pContexte->PathName("FGLO"), &wndProp, true) ;
  //pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, "Synthesis", rect, pContexte->PathName("FGLO")) ;
}

// -----------------------------------------------------------------------------
// Fonction CanClose : Appel de CanClose du document
// -----------------------------------------------------------------------------
bool
NSCsVue::CanClose()
{
  NSTreeWindow *pNSTreeWindow = this ;
  _pDoc->SetDirty(pNSTreeWindow->GetDirty()) ;

  NSSuper* pSuper = _pDoc->pContexte->getSuperviseur() ;

  TMyApp *pMyApp = pSuper->getApplication() ;

  if (false == _pDoc->CanClose())
  {
    string sText = string("") ;
    string ps    = string("NSCsVue::CanClose(), proposing to save") ;
    if (_pDoc->GetTitle())
      ps += string(" document ") + string(_pDoc->GetTitle()) ;
    else
      ps += string(" untitled document") ;

    string sCaption = string("Message ") + pSuper->getAppName().c_str() ;
    if (_pDoc->_pDocInfo)
    {
      ps += string(", because it has been modified.") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;
      sText = pSuper->getText("documentManagement", "doYouWantToSaveChangesForDocument") ;
    }
    else
    {
      ps += string(", because it is new.") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;
      sText = pSuper->getText("documentManagement", "doYouWantToSaveDocument") ;
    }
    if (_pDoc->GetTitle())
      sText += string(" ") + _pDoc->GetTitle() ;
    sText += string(" ?") ;

    int iRetVal = MessageBox(sText.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
    if (IDCANCEL == iRetVal)
      return false ;
    if (IDYES == iRetVal)
    {
      CmEnregistre() ;
      pMyApp->FlushControlBar() ;
      _bSetupToolBar = false ;
      return true ;
    }
  }
  _pDoc->SetDirty(false) ;
  pMyApp->FlushControlBar() ;
  _bSetupToolBar = false ;
  return true ;
}

// -----------------------------------------------------------------------------
// Edition du noeud (pères + fils) sous forme de fil guide
// -----------------------------------------------------------------------------
void
NSCsVue::EditionFilGuide()
{
  // Localisation du noeud qui possède le focus
  TTreeNode noeud = GetSelection() ;

  NSTreeNode *pNSTreeNode = GetNSTreeNode(&noeud) ;
  if ((NSTreeNode*) NULL == pNSTreeNode)
    return ;

  // on appelle okFermerDialogue() pour stocker la patpatho
  // dans le BBFilsItem lanceur (on peut ainsi récupérer le code root)
  okFermerDialogue() ;

  // on "shunte" maintenant EnregistrePatPatho et on récupère directement
  // le chemin racine dans le BBFilsItem lanceur
  BBFilsItem *pBBFilsLanceur = *(pBBitemNSTreeWindow->_aBBItemFils.begin()) ;
  string     sCodeRoot = string(pBBFilsLanceur->getItemData()->chemin) ;

  if (string("") == sCodeRoot)
  {
    erreur("Impossible de récupérer le code racine de l'arbre de description.", standardError, 0, NSTreeWindow::GetHandle()) ;
    return ;
  }

  // on constitue le chemin - codes lexiques - jusqu'au node (lui-même y compris)
  string sCheminNode, sEtiqu ;
  pNSTreeNode->formerCheminLocal(&sCheminNode) ; // chemin des pères

  // on ajoute la racine avant le chemin local
  // pour obtenir un chemin père valide au sens "fil guide"
  sCheminNode = sCodeRoot + string(1, cheminSeparationMARK) + sCheminNode ;

  // on rajoute le node himself
  sEtiqu = pNSTreeNode->getLabel() ;

  if (string("") != sCheminNode)
    sCheminNode += string(1, cheminSeparationMARK) ;
  sCheminNode += sEtiqu ;

  // chemin des fils
  string sEnsembleFils = string("") ;
  // on parcourt l'ensemble des fils existants
  if (false == pNSTreeNode->getVectSons()->empty())
  {
    for (iterNSTreeNode i = pNSTreeNode->getVectSons()->begin() ; pNSTreeNode->getVectSons()->end() != i ; i++)
    {
      // Remarque : on ne fait pas ici le test "if ((*i)->pControle)"
      // qui sert à savoir si le fils est proposé (pControle == 0) ou non
      // car on veut éditer dans le fil guide l'ensemble des fils du node
      sEtiqu = (*i)->getLabel() ;

      // Attention, l'ensemble des fils ne constitue pas un chemin.
      // chaque fils doit être séparé du suivant par un '|', ce qui
      // permet de charger le tableau des fils par ::ParsingChemin
      if (string("") != sEnsembleFils)
        sEnsembleFils += string(1, nodeSeparationMARK) ;
      sEnsembleFils += sEtiqu ;
    }
  }

  // Chargement et appel du dialogue d'édition des fils guides
  NSEditFilGuideDialog *pEditFGDlg = new NSEditFilGuideDialog((NSTreeWindow*)this, NSTreeWindow::pContexte) ;

  // on charge les tableaux Pere / Fils
  pEditFGDlg->ParsingChemin(sCheminNode, true) ;
  pEditFGDlg->ParsingChemin(sEnsembleFils, false) ;
  pEditFGDlg->Execute() ;

  delete pEditFGDlg ;
}

void
NSCsVue::Encyclop()
{
	// Localisation du noeud qui possède le focus
	TTreeNode  noeud        = GetSelection() ;
	NSTreeNode *pNSTreeNode = GetNSTreeNode(&noeud) ;

	if (((NSTreeNode*) NULL == pNSTreeNode) || (string("") == pNSTreeNode->getUrl()))
    return ;

	NSSuper *pSuper = NSTreeWindow::pContexte->getSuperviseur() ;

	string sClassif ;
	string sTemp = pNSTreeNode->getEtiquette() ;
	NSDico::donneCodeSens(&sTemp, &sClassif) ;

	// Si ce n'est pas une classification, on navigue directement
  //
	if (false == pSuper->getFilGuide()->VraiOuFaux(sClassif, "ES", "0CODE"))
	{
		NSTreeWindow::pContexte->getSuperviseur()->NavigationEncyclopedie(pNSTreeNode->getUrl(), NSTreeWindow::pContexte) ;
		return ;
	}

  string sLink = pNSTreeNode->getUrl() ;
  size_t pos = sLink.find(sClassif) ;
  if (string::npos == pos)
  	NSTreeWindow::pContexte->getSuperviseur()->NavigationEncyclopedie(pNSTreeNode->getUrl(), NSTreeWindow::pContexte) ;

  string sCode = pNSTreeNode->getFreeTextCode() ;
  if (strlen(sLink.c_str()) > pos - strlen(sClassif.c_str()))
  	sLink = string(sLink, 0, pos) + sCode + string(sLink, pos + strlen(sClassif.c_str()), strlen(sLink.c_str()) - pos - strlen(sClassif.c_str())) ;
  else
		sLink = string(sLink, 0, pos) + sCode ;

  NSTreeWindow::pContexte->getSuperviseur()->NavigationEncyclopedie(sLink, NSTreeWindow::pContexte) ;
}

void
NSCsVue::Cm1Plus()
{
  CmXPlus(string("B")) ;
}

void
NSCsVue::Cm2Plus()
{
  CmXPlus(string("C")) ;
}

void
NSCsVue::Cm3Plus()
{
  CmXPlus(string("D")) ;
}

void
NSCsVue::Cm4Plus()
{
  CmXPlus(string("E")) ;
}

void
NSCsVue::CmXPlus(string sModifier)
{
  // Getting node with focus
  TTreeNode noeud = GetSelection() ;

  NSTreeNode *pNSTreeNode = GetNSTreeNode(&noeud) ;
  if (NULL == pNSTreeNode)
    return ;

  if (pNSTreeNode->getCertitude() == sModifier)
    pNSTreeNode->fixeInteret("A") ;
  else
    pNSTreeNode->fixeInteret(sModifier) ;
}

void
NSCsVue::Ce1Plus(TCommandEnabler& ce)
{
  uint result = 0 ;

  // Localisation du noeud qui possède le focus
  TTreeNode   noeud         = GetSelection() ;
  NSTreeNode  *pNSTreeNode  = GetNSTreeNode(&noeud) ;
  if (!pNSTreeNode)
    return ;

  if (pNSTreeNode->getCertitude() == "B")
    result = 1 ;

  ce.SetCheck(result  ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}


void
NSCsVue::Ce2Plus(TCommandEnabler& ce)
{
  CeXPlus(ce, string("C")) ;
}


void
NSCsVue::Ce3Plus(TCommandEnabler& ce)
{
  CeXPlus(ce, string("D")) ;
}

void
NSCsVue::Ce4Plus(TCommandEnabler& ce)
{
  CeXPlus(ce, string("E")) ;
}

void
NSCsVue::CeXPlus(TCommandEnabler& ce, string sModifier)
{
  // Getting node with focus
  //
  TTreeNode noeud = GetSelection() ;

  NSTreeNode *pNSTreeNode = GetNSTreeNode(&noeud) ;
  if (NULL == pNSTreeNode)
    return ;

  uint result = 0 ;
  if (pNSTreeNode->getCertitude() == sModifier)
    result = 1 ;

  ce.SetCheck(result ? TCommandEnabler::Checked : TCommandEnabler::Unchecked) ;
}

void
NSCsVue::CmFileClose()
{
  // On détruit dans ce cas la partie NSRefDocument avant de détruire la vue
  NSTreeWindow::GetApplication()->GetDocManager()->CmFileClose() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse à la commande Composer
// -----------------------------------------------------------------------------
void
NSCsVue::CmComposer()
{
	NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  if ((NSCSDocument*) NULL == pCSDoc)
    return ;

  if (pCSDoc->_pHtmlCS)
  	delete pCSDoc->_pHtmlCS ;

  // htmlNode racine du html de consultation
  pCSDoc->_pHtmlCS = new NSHtml(tCS) ;

  InscrireHtml(pCSDoc->_pHtmlCS) ;

  if (false == _bConsultVide)
  	pCSDoc->Composer() ;
  else
  	NSTreeWindow::MessageBox("Le document est vide.") ;
}

void
NSCsVue::initMUEViewMenu(string sMenuName){	if (_pMUEViewMenu)  	delete _pMUEViewMenu ;

	nsMenuIniter menuIter(NSTreeWindow::pContexte) ;
	_pMUEViewMenu = new OWL::TMenuDescr ;
  menuIter.initMenuDescr(_pMUEViewMenu, sMenuName) ;
}
voidNSCsVue::activateMUEViewMenu(){	if (NULL == _pMUEViewMenu)		return ;

	TMyApp* pMyApp = NSTreeWindow::pContexte->getSuperviseur()->getApplication() ;

  if (pMyApp && pMyApp->GetMainWindow())
    pMyApp->GetMainWindow()->SetMenuDescr(*_pMUEViewMenu) ;
}

void
NSCsVue::reloadView(string sReason)
{
  // First, close existing nodes
  //
  if (pBBitemNSTreeWindow)
	  pBBitemNSTreeWindow->okFermerDialogue(false, false) ;

  _NodeArray.vider() ;

  NSCSDocument *pCSDoc = dynamic_cast<NSCSDocument *>(_pDoc) ;
  // Dispatcher la patpatho
  if (pCSDoc)
  {
    NSPatPathoArray* pDocPpt = pCSDoc->getPatPathoPointer() ;
    if (pDocPpt)
      NSTreeWindow::DispatcherPatPatho(pDocPpt, (NSTreeNode*) 0, (int*) 0, string("")) ;
  }
}

bool
NSCsVue::isSynthesis()
{
  if (NULL == _pBigBoss)
    return false ;

  string sLexiqueModule = _pBigBoss->getLexiqueModule() ;
  return (string(sLexiqueModule, 0, 5) == string("ZSYNT")) ;
}

