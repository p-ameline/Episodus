// -----------------------------------------------------------------------------
// NSCQVUE.CPP		Document/Vues Formulaires
// -----------------------------------------------------------------------------

#define __NSCQVUE_CPP
#include <owl\owlpch.h>#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\nscqvue.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsattval.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsdico.h"
#include "nsbb\ns_skins.h"
#include "nautilus\nscqdoc.h"
#include "nsepisod\nsToDo.h"

#include "nscompta\nscpta.h"
#include "nscompta\nsfse16.h"
#include "nautilus\nsadmiwd.h"

#include "ns_ob1\nautilus-bbk.h"

//------------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Table de réponses de la classe NSCQVue
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSCQWindowsView, NSBBMUEView)
  EV_WM_SETFOCUS,
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  EV_WM_KEYDOWN,
  EV_WM_SYSKEYDOWN,
  EV_WM_KEYUP,
  //EV_WM_DESTROY,
	EV_COMMAND(IDHELP,        CmHelp),
  EV_COMMAND(CM_ENREGISTRE, CmEnregistre),
  //EV_COMMAND(CM_IMPRIME, 	  CmPublier),
  //EV_COMMAND(CM_VISUAL, 	  CmVisualiser),
  EV_COMMAND(CM_FILECLOSE,  EvClose),
END_RESPONSE_TABLE ;
NSCQWindowsView::NSCQWindowsView(NSCQDocument& doc, TWindow *parent)
                :NSBBMUEView(doc.pContexte, &doc, parent), _pDoc(&doc)
{
  Attr.AccelTable = REFVIEW_ACCEL ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;

	string ps = string("Entrée dans le constructeur de NSCQVue") ;
	pSuper->trace(&ps, 1) ;

	// initMUEViewMenu("menubar") ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;

	_pToolBar      = (OWL::TControlBar*) 0 ;
	_pBigBoss      = (NSSmallBrother*) 0 ;
	_bSetupToolBar = true ;

  _bCanClose     = true ;
  _bOkActivated  = false ;

  _bWantToSaveAsked = false ;

	ps = string("Constructeur de NSCQVue, lancement de l'Archetype : ") + _pDoc->getArchetype() ;
	pSuper->trace(&ps, 1) ;

	lanceArchetype() ;

  setViewName() ;

  ps = string("Sortie du constructeur de NSCQVue") ;
	pSuper->trace(&ps, 1) ;
}

NSCQWindowsView::~NSCQWindowsView()
{
/*
    if (sFonction != "")
    {
        NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect() ;//coordonnées par % à l'écran
    int X, Y, W, H ;
    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
    Parent->Parent->ScreenToClient(point) ;
    X = point.X() ;
  	Y = point.Y() ;
    W = rectDlg.Width() ;
    H = rectDlg.Height() ;

        if (Parent->IsIconic())
            return ;

        //string sUserId = string(pContexte->getUtilisateur()->pDonnees->indice) ;
    //NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
    //pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, sFonction, rectDlg, pContexte->PathName("FGLO")) ;
  }
*/

  if (_pBigBoss)
    delete _pBigBoss ;
}

void
NSCQWindowsView::setViewName()
{
	sViewName = string("") ;

	if (NULL == _pDoc->_pDocInfo)
  {
    // Note : on fixe ici le titre du document avec le titre du dialogue
    // si le document est vierge (sinon le titre est fixé par NSNoyauDocument)
    // Il est inutile d'appeler la méthode SetDocTitle, le TDocument::SetTitle suffit
    // car l'affichage du titre dans la vue est fait par la TMDIChild

    string sLang = pContexte->getUserLanguage() ;

    // The right place to look is in the "references" block
    //
    Creferences *pRefBlock = _pDoc->getParser()->getArchetype()->Carchetype::getReference() ;
    if (pRefBlock)
    {
      Chead *pReferenceHead = pRefBlock->getHead(sLang) ;
      if (pReferenceHead)
        sViewName = pReferenceHead->getTitle() ;
    }

    // If not found, have a look in the dialog box
    //
    if (string("") == sViewName)
    {
      Cdialogbox* pDialogBox = _pDoc->getParser()->getArchetype()->getDialogBox(sLang) ;
      if (pDialogBox)
        sViewName = pDialogBox->getCaption() ;
    }

    if (string("") == sViewName)
      sViewName = pContexte->getSuperviseur()->getText("archetypesManagement", "archetype") ;

    addConcernTitle() ;
  }
  else
	{
	}
}

voidNSCQWindowsView::lanceArchetype(){try{  if (string("") == _pDoc->getArchetype())    return ;	// Ici on doit laisser les BBItem "précocher" l'archétype	// à partir de la patpatho du document et non par le blackboard
	//
	// En mode OB1, le document fixe la variable bInitFromBbk dans pDoc->pKSdescriptor
	//
	_pBigBoss = new NSSmallBrother(pContexte, _pDoc->getPatPathoPointer()) ;

  _pBigBoss->setMotherWindow(pContexte->GetMainWindow()) ;

	bool bSuccess = _pBigBoss->lanceBbkArchetypeInView(_pDoc->getArchetype(), _pDoc->getParser(), 0, this, _pDoc->getKsDescriptor()) ;
  if (false == bSuccess)
    return ;

  _pBBItem = _pBigBoss->getBBItem() ;

  if ((_pDoc->getParser()) && (_pDoc->getParser()->getArchetype()))
    sFonction = _pDoc->getParser()->getArchetype()->getFunction() ;

	// pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
}
catch (...)
{
  erreur("Exception NSCQVue::lanceArchetype.", standardError, 0) ;
}
}

TWindow*
NSCQWindowsView::GetWindow()
{
  return (TWindow *) this ;
}

void
NSCQWindowsView::SetupToolBar()
{
	// Ici pas de barre de boutons spécifique :
	// on se contente de virer la barre en cours
	// TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	// pMyApp->FlushControlBar() ;

  pPaneSplitter->FlushControlBar() ;

  pPaneSplitter->_pGadgetPanelWindow->setButtonsStyle(uButtonsStyle) ;
  pPaneSplitter->_pGadgetPanelWindow->insertMainGadgets() ;

/*
  if ((uButtonsStyle & MYWS_OK) == MYWS_OK)
		pPaneSplitter->Insert(*new TButtonGadget(IDC_OK_PANESPLIT,     IDC_OK_PANESPLIT,     TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_CANCEL) == MYWS_CANCEL)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_CANCEL_PANESPLIT, IDC_CANCEL_PANESPLIT, TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_HELP) == MYWS_HELP)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_HELP_PANESPLIT,   IDC_HELP_PANESPLIT,   TButtonGadget::Command)) ;
*/

  pPaneSplitter->LayoutSession() ;
}

void
NSCQWindowsView::PerformCreate(int menuOrId)
{
  string ps = string("NSCQVue, entrée dans PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  if (NULL == _pDoc->_pDocInfo)
  {
    // Note : on fixe ici le titre du document avec le titre du dialogue
    // si le document est vierge (sinon le titre est fixé par NSNoyauDocument)
    // Il est inutile d'appeler la méthode SetDocTitle, le TDocument::SetTitle suffit
    // car l'affichage du titre dans la vue est fait par la TMDIChild
    char far szTitle[256] ;
    strcpy(szTitle, "Archetype") ;
    pPaneSplitter->SetFrameTitle(getFunction(), szTitle) ;
    // pDoc->SetTitle(szTitle) ;
  }

  ps = string("NSCQVue, sortie de PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  NSBBMUEView::PerformCreate(menuOrId) ;
}

void
NSCQWindowsView::SetupWindow()
{
  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string psHeader = string("NSCQVue ") + string(szThis) + string(" : ") ;
  string ps = psHeader + string("Entering SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  NSBBMUEView::SetupWindow() ;

  SetCaption(sViewName.c_str()) ;

  if (pCreateWindow)
  {
    char szCreate[20] ;
    sprintf(szCreate, "%p", pCreateWindow->HWindow) ;
    ps = psHeader + string("Create Window ") + string(szCreate) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }
  if (pSplittedWindow)
  {
    char szSplitted[20] ;
    sprintf(szSplitted, "%p", pSplittedWindow->HWindow) ;
    ps = psHeader + string("Splitted Window ") + string(szSplitted) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }
  if (pPaneSplitter)
  {
    char szPane[20] ;
    sprintf(szPane, "%p", pPaneSplitter->HWindow) ;
    ps = psHeader + string("PaneSplitter ") + string(szPane) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    if (pPaneSplitter->_pMDIChild)
    {
      char szMdiChild[20] ;
      sprintf(szMdiChild, "%p", pPaneSplitter->_pMDIChild->HWindow) ;
      ps = psHeader + string("PaneSplitter's Child ") + string(szMdiChild) ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    }
  }
  if (pGadgetPanelWindow)
  {
    char szGadget[20] ;
    sprintf(szGadget, "%p", pGadgetPanelWindow->HWindow) ;
    ps = psHeader + string("GadgetWindow ") + string(szGadget) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }

  string sLang = pContexte->getUserLanguage() ;

  Cdialogbox* pDialogBox = _pDoc->getParser()->getArchetype()->getDialogBox(sLang) ;
	if (pDialogBox)
    _dlgRect = getWindowRectFromDialogRect(pDialogBox->getX(),
                                           pDialogBox->getY(),
                                           pDialogBox->getW(),
                                           pDialogBox->getH()) ;

  //pDoc->SetTitle(sTitle.c_str()) ;
  //SetDocTitle(sTitle.c_str(), 0) ;
  ps = string("NSCQVue ") + string(szThis) + string(", leaving SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;
}

void
NSCQWindowsView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  int iAncDC = dc.SaveDC() ;

  NS_CLASSLIB::TColor archetypeBackgroundColor ;
  nsSkin* pArchetypeSkin = pContexte->getSkins()->donneSkin("Archetype") ;
	if (pArchetypeSkin && pArchetypeSkin->getBackColor())
  	archetypeBackgroundColor = *(pArchetypeSkin->getBackColor()) ;
  else
  	archetypeBackgroundColor = NS_CLASSLIB::TColor(240, 255, 240) ;

  OWL::TBrush Pinceau(archetypeBackgroundColor) ;

  dc.FillRect(RectangleAPeindre, Pinceau) ;

  dc.RestoreDC(iAncDC) ;

  NSBBMUEView::Paint(dc, false, RectangleAPeindre) ;
}

void
NSCQWindowsView::EvSetFocus(THandle hWndLostFocus)
{
	activateMUEViewMenu() ;

	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

  string sHelpUrl = _pDoc->getHelpUrl() ;
  if (string("") != sHelpUrl)
    pContexte->setAideCorps(sHelpUrl) ;

	NSMUEView::EvSetFocus(hWndLostFocus) ;
  pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;

  pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}

void
NSCQWindowsView::EvClose()
{
	if (NSCQDocument::dpio == _pDoc->getDocType())
	{
		NSPredi *pPredi = pContexte->getPredi() ;
		if (pPredi)
			pPredi->fermeFormulaire((TWindow*) GetParent()) ;
	}
}

void
NSCQWindowsView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSCQWindowsView::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvSysKeyDown(key, repeatCount, flags) ;
}

void
NSCQWindowsView::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSCQWindowsView::CmHelp()
{
  pContexte->NavigationAideEnLigne() ;
}

void
NSCQWindowsView::CmOk()
{
  // If Ok already activated, do nothing
  //
  if (_bOkActivated)
    return ;

  if ((NULL == _pBigBoss) || (NULL == _pBBItem))
    return ;

  _bOkActivated = true ;

  if (false == CanSave())
  {
    _bCanClose    = false ;
    _bOkActivated = false ;
    return ;
  }

  executePreClosingFunctions() ;
  disconnectControls() ;

  // Dirty: disconnect interface so we don't crash later
  //
  // if (pContexte->getBBinterface())
  //  pContexte->getBBinterface()->DisconnectInterface(HWindow) ;

  bool bRootArchetype = false ;
  if (_pBBItem->ouvreArchetype())
  	bRootArchetype = true ;

  //
  // Déclenchement du calcul
  //
  if (bRootArchetype)
  {
  	CmBbkCalcul() ;

    // Form there on, the process is asynchronous, waiting for a KS to answer.
    // Once this KS has answered (or the process has been canceled),
    // CmOkFinalStep() is called
    //
    if (_bWaitingForKsAnswer)
      return ;
  }

  // If we are there, it means that no KS is involved, and that the application
  // is waiting for this user iterface's results.
  //
  if (false == _pBBItem->okFermerDialogue(true, false /* bDetacheCtrls */))
  {
    _bOkActivated = false ;
  	return ;
  }
  _pBigBoss->MetTitre() ;

  executeClosingFunctions() ;

  _bWaitingForSystemAction = true ;
  _bAcceptUpdate           = false ;
  _pBBItem->_pBigBoss->fermeBbkArchetype(IDOK, this) ;
}

void
NSCQWindowsView::CmOkFinalStep()
{
  // Update BigBoss's patpatho
  //
  if (false == _pBBItem->okFermerDialogue(true, false /* bDetacheCtrls */))
  {
    _bOkActivated = false ;
  	return ;
  }
  _pBigBoss->MetTitre() ;

  executeClosingFunctions() ;

  if (_pDoc->enregistre(false, false))
    pPaneSplitter->youCanCloseNow() ;
  else
    _bOkActivated = false ;
}

void
NSCQWindowsView::CmCancel()
{
}

void
NSCQWindowsView::EvDestroy()
{
  //if (sFonction != "")
    //NSMUEView::RecordWindowPosit() ;

  if (NSCQDocument::dpio == _pDoc->getDocType())
	{
    if ((pPaneSplitter) && (pPaneSplitter->_pMDIChild))
    {
      NSSuper *pSuper = pContexte->getSuperviseur() ;
      TWindow *pWinPtr = (TWindow *) pPaneSplitter->_pMDIChild ;
      NSToDoTask* pTask = new NSToDoTask ;
      pTask->setWhatToDo(string("FermeDPIO")) ;
      pTask->setPointer1((void*) pWinPtr, false) ;

      pTask->sendMe(pSuper) ;
    }
  }

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("NSCQWindowsView::EvDestroy disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }
}

// -----------------------------------------------------------------------------// Document notifications// -----------------------------------------------------------------------------
bool
NSCQWindowsView::CanClose()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string ps = string("Entering NSCQWindowsView::CanClose()") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  if (false == _bCanClose)
  {
    _bCanClose = true ;
    ps = string("NSCQWindowsView::CanClose(), leaving (bCanClose false)") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    return false ;
  }

  if ((true == _bWaitingForKsAnswer) || (true == _bWaitingForSystemAction))
  {
    ps = string("NSCQWindowsView::CanClose(), leaving (waiting for answer or action)") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    return false ;
  }

  // Check if changes were made and if they must be saved
  //
  if (false == _bWantToSaveAsked)
  {
    bool bReturnOption = askWantToSave() ;
    if (false == bReturnOption)
    {
      _bWantToSaveAsked = false ;
      return false ;
    }

    _bWantToSaveAsked = true ;
  }

  _pDoc->SetDirty(false) ;

  TMyApp *pMyApp = pSuper->getApplication() ;
  pMyApp->FlushControlBar() ;

  _bSetupToolBar = false ;
  _bClosing      = true ;

  return true ;
}

bool
NSCQWindowsView::askWantToSave()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // Get previous patpatho
  //
  NSPatPathoArray PPt(pContexte->getSuperviseur()) ;
  _pDoc->initFromPatPatho(&PPt) ;

  // Build new pathpatho
  //
  if (false == _pBBItem->okFermerDialogue(true, false /* bDetacheCtrls */))
    return true ;

  NSPatPathoArray PPtNew(pContexte->getSuperviseur()) ;
  _pDoc->initFromPatPatho(&PPtNew) ;

  bool bPptChanged = (false == PPt.estEgal(&PPtNew)) ;

  // Special case, if Archetype is demographic one, then check if IPP is to be
  // removed
  //
  string sIPP = string("") ;
  if (pSuper->getDemographicArchetypeId() == _pDoc->getDefaultArchetype())
  {
    pContexte->getUtilisateur()->getAndRemoveIppFromPatpatho(&PPtNew, &sIPP) ;
    _pDoc->setPatPatho(&PPtNew) ;
  }

  TMyApp *pMyApp = pSuper->getApplication() ;

  NSPatientChoisi *pPatient = pContexte->getPatient() ;
  if (NULL == pPatient)
    return true ;

  bool bIppChanged = false ;
  if ((string("") != sIPP) && (string("") != pSuper->getIppSite()))
  {
    string sPreviousIPP = pPatient->getIPPEnCours(pSuper->getIppSite()) ;
    if (sPreviousIPP != sIPP)
      bIppChanged = true ;
  }

  string ps = string("") ;

  if ((false == bPptChanged) && (false == bIppChanged))
    return true ;

  if (bPptChanged)
  {
    // begin Temporary trace

    PatPathoIter i = PPt.begin() ;
    PatPathoIter j = PPtNew.begin() ;    while ((PPt.end() != i) && (PPtNew.end() != j))    {      if (false == (*i)->estEgal(*j))      {        string sPrevious = (*i)->getLocalisation() + string(", ") +                           (*i)->getType()         + string(", ") +
                           (*i)->getLexique()      + string(", ") +
                           (*i)->getComplement()   + string(", ") +
                           (*i)->getUnit()         + string(", ") +
                           (*i)->getCertitude()    + string(", ") +
                           (*i)->getInteret()      + string(", ") +
                           (*i)->getPluriel()      + string(", ") +
                           (*i)->getVisible()      + string(", ") +
                           (*i)->getTexteLibre()   + string(", ") +
                           (*i)->getArchetype() ;

        string sNew      = (*j)->getLocalisation() + string(", ") +
                           (*j)->getType()         + string(", ") +
                           (*j)->getLexique()      + string(", ") +
                           (*j)->getComplement()   + string(", ") +
                           (*j)->getUnit()         + string(", ") +
                           (*j)->getCertitude()    + string(", ") +
                           (*j)->getInteret()      + string(", ") +
                           (*j)->getPluriel()      + string(", ") +
                           (*j)->getVisible()      + string(", ") +
                           (*j)->getTexteLibre()   + string(", ") +
                           (*j)->getArchetype() ;
        ps = string("NSCQWindowsView::CanClose() ") + sPrevious + string(" -> ") + sNew ;        pSuper->trace(&ps, 1, NSSuper::trDetails) ;        break ;      }      i++ ;      j++ ;    }    // end Temporary trace

    if (_pDoc->GetTitle())
      ps = string("NSCQWindowsView::CanClose(), proposing to save document ") + _pDoc->GetTitle() ;
    else
      ps = string("NSCQWindowsView::CanClose(), proposing to save document ") + _pDoc->getArchetype() ;
  }
  else if (bIppChanged)
    ps = string("NSCQWindowsView::CanClose(), proposing to save IPP ") + sIPP ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  string sText    = string("") ;
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

  int retVal = MessageBox(sText.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
  if (IDCANCEL == retVal)
  {
    ps = string("NSCQWindowsView::CanClose(), user cancels. Leaving.") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    return false ;
  }

  if (IDYES == retVal)
  {
    ps = string("NSCQWindowsView::CanClose(), user agrees to save.") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    if (false == _pBBItem->okFermerDialogue(true, false /* bDetacheCtrls */))
      return false ;

    if (bPptChanged)
      CmEnregistre() ;

    if (bIppChanged)
      pPatient->updateIPPEnCours(pSuper->getIppSite(), sIPP) ;

    pMyApp->FlushControlBar() ;
    bSetupToolBar = false ;
    return true ;
  }
  else
  {
    ps = string("NSCQWindowsView::CanClose(), user says No. Leaving.") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
  }

  return true ;
}

void
NSCQWindowsView::CmPublier()
{
}

void
NSCQWindowsView::CmVisualiser()
{
}

void
NSCQWindowsView::CmEnregistre()
{
  if ((NULL == _pBigBoss) || (NULL == _pBigBoss->getBBItem()))
    return ;

  if (false == CanSave())
    return ;

  // Update BigBoss's patpatho
  //
  if (false == _pBBItem->okFermerDialogue(true, false /* bDetacheCtrls */))
  	return ;
  _pBigBoss->MetTitre() ;

  bool bNewDoc = false ;
  if (NULL == _pDoc->_pDocInfo)
    bNewDoc = true ;

  if (_pDoc->enregistre())
  {
    _pDoc->SetDirty(false) ;

    if (bNewDoc && (pContexte->getSuperviseur()->isComptaAuto()) && (_pDoc->ComptaAuto()))
    {
      string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
      int retVal = MessageBox("Il n'existe pas de fiche comptable pour ce document. Voulez-vous lancer la comptabilité ?", sCaption.c_str(), MB_YESNO) ;
      if (IDYES == retVal)
        Compta() ;
    }
  }
  else
    _pDoc->SetDirty(true) ;
}

void
NSCQWindowsView::CmFileClose()
{
}

// -----------------------------------------------------------------------------
// Appel automatique d'une fiche Compt à partir d'une consultation
// -----------------------------------------------------------------------------
void
NSCQWindowsView::Compta()
{
try
{
  NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
  _pDoc->initFromPatPatho(&PatPatho) ;

	if (PatPatho.empty())
	{
    erreur("Vous devez enregistrer le document avant de le comptabiliser.", warningError, 0, GetHandle()) ;
    return ;
  }

  string			  sCode = "", sExam = "", sSyn = "" ;
  string 			  sLibelleDoc = "", sExtHosp = "" ;
  NSComptInfo   ComptInfo ;
  NSFse1610Info Fse1610Info ;

  if (NULL == _pDoc->getBigBoss())
    _pDoc->setBigBoss(new NSSmallBrother(pContexte, _pDoc->getPatPathoPointer())) ;

  string sDateDoc = PatPatho.GetDocDate() ;

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

  string sDate = string(sDateDoc, 0, 8) ;
	string sHeure = string("0000") ;
	if (strlen(sDateDoc.c_str()) >= 12)
		sHeure = string(sDateDoc, 8, 4) ;

	strcpy(ComptInfo._Donnees.date, sDate.c_str()) ;
	strcpy(ComptInfo._Donnees.heure, sHeure.c_str()) ;
  // on cale la date de la Fse1610 sur celle de la fiche Compt
	strcpy(Fse1610Info._Donnees.date, ComptInfo._Donnees.date) ;
	strcat(Fse1610Info._Donnees.date, ComptInfo._Donnees.heure) ;

  // Externe / Hospitalisé
  PatPatho.CheminDansPatpatho(_pDoc->getBigBoss(), "LADMI/LCONT", &sExtHosp) ;
  if (string("") != sExtHosp)
    sExtHosp = string(sExtHosp, 0, 5) ;

  sCode = (*(PatPatho.begin()))->getLexique() ;
  if (string("") != sCode)
  {
    sExam = string(sCode, 0, 5) ;
    sSyn = string(sCode, 5, 1) ;
  }

  strcpy(ComptInfo._Donnees.examen,   sExam.c_str()) ;
  strcpy(ComptInfo._Donnees.synonyme, sSyn.c_str()) ;
  strcpy(ComptInfo._Donnees.contexte, sExtHosp.c_str()) ;

  NSPersonInfo personInfo(pContexte->getSuperviseur(), pContexte, pContexte->getPatient()->getNss()) ;

  // NSComptaPatient *pCompta = new NSComptaPatient(NSTreeWindow::pContexte, (NSPatInfo *) NSTreeWindow::pContexte->getPatient()) ;
  NSComptaPatient Compta(pContexte, &personInfo) ;
  Compta.CmFicheCompt(&ComptInfo, &Fse1610Info) ;
}
catch (...)
{
	erreur("Exception NSCQWindowsView::Compta", standardError, 0) ;
}
}

bool
NSCQWindowsView::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  if (NULL == hAccelerator)
    return NSBBMUEView::PreProcessMsg(msg) ;

  /* bool bTranslated = */ ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) ;

  // if (bTranslated)
  //   return true ;
  // return false ;

  return NSBBMUEView::PreProcessMsg(msg) ;
}

// -----------------------------------------------------------------------------
// Table de réponses de la classe NSCQVue
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE2(NSCQVue, TView, TWindow)
  EV_WM_SETFOCUS,
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  //EV_WM_DESTROY,
  EV_COMMAND(CM_ENREGISTRE, CmEnregistre),
  //EV_COMMAND(CM_IMPRIME, 	  CmPublier),
  //EV_COMMAND(CM_VISUAL, 	  CmVisualiser),
  EV_COMMAND(CM_FILECLOSE,  EvClose),
END_RESPONSE_TABLE ;
NSCQVue::NSCQVue(NSCQDocument& doc, TWindow *parent)
        :NSMUEView(doc.pContexte, &doc, parent), pDoc(&doc)
{
	NSSuper *pSuper = pContexte->getSuperviseur() ;

	string ps = string("Entrée dans le constructeur de NSCQVue") ;
	pSuper->trace(&ps, 1) ;

	initMUEViewMenu("menubar") ;

	pToolBar  = 0 ;
	pBigBoss  = 0 ;
	clientWin = 0 ;
	bSetupToolBar = true ;

	ps = string("Constructeur de NSCQVue, lancement de l'Archetype : ") + pDoc->getArchetype() ;
	pSuper->trace(&ps, 1) ;
	lanceArchetype() ;
	ps = string("Sortie du constructeur de NSCQVue") ;
	pSuper->trace(&ps, 1) ;

  setViewName() ;
}

NSCQVue::~NSCQVue()
{
/*
    if (sFonction != "")
    {
        NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect() ;//coordonnées par % à l'écran
    int X, Y, W, H ;
    NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y()) ;
    Parent->Parent->ScreenToClient(point) ;
    X = point.X() ;
  	Y = point.Y() ;
    W = rectDlg.Width() ;
    H = rectDlg.Height() ;

        if (Parent->IsIconic())
            return ;

        //string sUserId = string(pContexte->getUtilisateur()->pDonnees->indice) ;
    //NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
    //pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, sFonction, rectDlg, pContexte->PathName("FGLO")) ;
  }
*/

  if (pDialog)
    delete pDialog ;

  if (pBigBoss)
    delete pBigBoss ;
}

void
NSCQVue::setViewName()
{
	sViewName = string("") ;

	if (NULL == pDoc->_pDocInfo)
  {
    // Note : on fixe ici le titre du document avec le titre du dialogue
    // si le document est vierge (sinon le titre est fixé par NSNoyauDocument)
    // Il est inutile d'appeler la méthode SetDocTitle, le TDocument::SetTitle suffit
    // car l'affichage du titre dans la vue est fait par la TMDIChild
    char far szTitle[256] ;
    int res = clientWin->GetWindowText(szTitle, 255) ;
    if (!res)
		{
    	sViewName = pContexte->getSuperviseur()->getText("archetypesManagement", "archetype") ;
			addConcernTitle() ;
		}
    else
    	sViewName = string(szTitle) ;
  }
  else
	{
	}
}

voidNSCQVue::lanceArchetype(){try{  if (string("") == pDoc->getArchetype())    return ;	// Ici on doit laisser les BBItem "précocher" l'archétype	// à partir de la patpatho du document et non par le blackboard
	//
	// En mode OB1, le document fixe la variable bInitFromBbk dans pDoc->pKSdescriptor
	//
	pBigBoss = new NSSmallBrother(pContexte, pDoc->getPatPathoPointer()) ;

  pBigBoss->setMotherWindow(pContexte->GetMainWindow()) ;

	// clientWin = pBigBoss->lanceBbkArchetypeInView(pDoc->sArchetype, pDoc->pParseur, 0, pDoc->pKSdescriptor, false, (TWindow*)this) ;
  // if (!clientWin)
  //  return ;

  pDialog = clientWin ;
  pDialog->_bMereMUEView = true ;

  clientWin->_pView = this ;
  clientWin->GetWindowRect(dlgRect) ;

  if ((pDoc->getParser()) && (pDoc->getParser()->getArchetype()))
    sFonction = pDoc->getParser()->getArchetype()->getFunction() ;

	// pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;
}
catch (...)
{
  erreur("Exception NSCQVue::lanceArchetype.", standardError, 0) ;
}
}
TWindow
*NSCQVue::GetWindow()
{
  return (TWindow *) this ;
}

void
NSCQVue::SetupToolBar()
{
	// Ici pas de barre de boutons spécifique :
	// on se contente de virer la barre en cours
	// TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	// pMyApp->FlushControlBar() ;

  pPaneSplitter->FlushControlBar() ;

  if ((uButtonsStyle & MYWS_OK) == MYWS_OK)
		pPaneSplitter->Insert(*new TButtonGadget(IDC_OK_PANESPLIT,     IDC_OK_PANESPLIT,     TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_CANCEL) == MYWS_CANCEL)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_CANCEL_PANESPLIT, IDC_CANCEL_PANESPLIT, TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_HELP) == MYWS_HELP)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_HELP_PANESPLIT,   IDC_HELP_PANESPLIT,   TButtonGadget::Command)) ;
  if ((uButtonsStyle & MYWS_PREVIOUS) == MYWS_PREVIOUS)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_PREVIOUS_PANESPLIT, IDC_PREVIOUS_PANESPLIT, TButtonGadget::Command)) ;
}

void
NSCQVue::PerformCreate(int menuOrId)
{
  string ps = string("NSCQVue, entrée dans PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  char far szTitle[256] ;

  SetHandle(clientWin->GetHandle()) ;
  clientWin->GetWindowRect(dlgRect) ;

  clientWin->_pView = (TWindowView *) this ;

  if (NULL == pDoc->_pDocInfo)
  {
    // Note : on fixe ici le titre du document avec le titre du dialogue
    // si le document est vierge (sinon le titre est fixé par NSNoyauDocument)
    // Il est inutile d'appeler la méthode SetDocTitle, le TDocument::SetTitle suffit
    // car l'affichage du titre dans la vue est fait par la TMDIChild
    int res = clientWin->GetWindowText(szTitle, 255) ;
    if (!res)
      strcpy(szTitle, "Archetype") ;
    else
    	pPaneSplitter->SetFrameTitle(getFunction(), szTitle) ;
    // pDoc->SetTitle(szTitle) ;
  }

  ps = string("NSCQVue, sortie de PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;
}

void
NSCQVue::SetupWindow()
{
  string ps = string("NSCQVue, entrée dans SetupWindow") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  TWindow::SetupWindow() ;

  //pDoc->SetTitle(sTitle.c_str()) ;
  //SetDocTitle(sTitle.c_str(), 0) ;
  ps = string("NSCQVue, sortie de SetupWindow") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;
}

void
NSCQVue::EvSetFocus(THandle hWndLostFocus)
{
	activateMUEViewMenu() ;

	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

	NSMUEView::EvSetFocus(hWndLostFocus) ;
  pPaneSplitter->SetFrameTitle(getFunction(), sViewName) ;

  if (clientWin)
		clientWin->EvSetFocus(hWndLostFocus) ;
}

void
NSCQVue::EvClose()
{
	if (NSCQDocument::dpio == pDoc->getDocType())
	{
		NSPredi *pPredi = pContexte->getPredi() ;
		if (pPredi)
			pPredi->fermeFormulaire((TWindow*) GetParent()) ;
	}
}

void
NSCQVue::CmOk()
{
  clientWin->CmOk() ;
}

void
NSCQVue::EvDestroy()
{
  //if (sFonction != "")
    //NSMUEView::RecordWindowPosit() ;
}

// -----------------------------------------------------------------------------
// Document notifications// -----------------------------------------------------------------------------
bool
NSCQVue::CanClose()
{
  return true ;
}

void
NSCQVue::CmPublier()
{
}

void
NSCQVue::CmVisualiser()
{
}

void
NSCQVue::CmEnregistre()
{
}

void
NSCQVue::CmFileClose()
{
}

bool
NSCQVue::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  // return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;

  if (NULL != hAccelerator)
  {
    bool bTranslated = ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) ;
    if (bTranslated)
      return true ;

    if (WM_KEYDOWN == msg.message)
    {
      bool bTranslated = ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) ;
      if (bTranslated)
        return true ;
      else
      {
        int cAccelerators = CopyAcceleratorTable(hAccelerator, NULL, 0);
        LPACCEL lpaccelNew = (LPACCEL) LocalAlloc(LPTR, cAccelerators * sizeof(ACCEL)) ;
        if (lpaccelNew != NULL)
        {
          CopyAcceleratorTable(hAccelerator, lpaccelNew, cAccelerators) ;
          LPACCEL lpaccelPt = lpaccelNew ;
          for (int i = 0 ; i < cAccelerators ; i++)
            ACCEL accel = *lpaccelPt++ ;
        }
      }
    }
  }
  return false ;
}

// -----------------------------------------------------------------------------
// fin de nscqvue.cpp
// -----------------------------------------------------------------------------

