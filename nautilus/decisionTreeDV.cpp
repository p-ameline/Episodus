// -----------------------------------------------------------------------------
// DECISIONTREEDV.CPP		Document/Views Decision tree
// -----------------------------------------------------------------------------

#include <owl\owlpch.h>#include <owl\validate.h>
#include <owl\inputdia.h>
#include <fstream.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nautilus\decisionTreeDV.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsattval.h"
#include "nsbb\nsattval_glif.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsPaneSplitter.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsdico.h"
#include "nsbb\ns_skins.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nstabwindow.h"
#include "nsbb\nstooltip.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nscqdoc.h"
#include "nsepisod\nsToDo.h"
#include "nssavoir\nsgraphe.h"
#include "dcodeur\decoder.h"

#include "nscompta\nscpta.h"
#include "nscompta\nsfse16.h"
#include "nautilus\nsadmiwd.h"
#include "nautilus\nsCoopChartDV.h"
#include "nautilus\nsDocView.h"

#include "ns_ob1\nautilus-bbk.h"

#include "nsbb\nsbb.rh"

// -----------------------------------------------------------------------------
//                             NSDecisionTreeView
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSDecisionTreeView, NSMUEView)
  EV_WM_SETFOCUS,
  EV_WM_CLOSE,
  EV_WM_DESTROY,
  EV_WM_KEYDOWN,
  EV_WM_SYSKEYDOWN,
  EV_WM_KEYUP,
  EV_COMMAND(IDM_BBK_UPDATE, CmBbkUpdate),
	EV_COMMAND(IDHELP,         CmHelp),
  EV_COMMAND(CM_FILECLOSE,   EvClose),
  EV_MESSAGE(WM_CTLCOLORSTATIC, EvSetBackColor),
END_RESPONSE_TABLE ;
NSDecisionTreeView::NSDecisionTreeView(NSDecisionTreeDocument& doc, TWindow *parent)
                   :NSMUEView(doc.pContexte, &doc, parent), _pDoc(&doc)
{
  Attr.AccelTable = REFVIEW_ACCEL ;

	NSSuper *pSuper = pContexte->getSuperviseur() ;

	string ps = string("Entering NSDecisionTreeView constructor") ;
	pSuper->trace(&ps, 1) ;

	// initMUEViewMenu("menubar") ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar"), &_hAccelerator) ;

	_pToolBar      = (OWL::TControlBar *) 0 ;
	_bSetupToolBar = true ;

  _bCanClose     = true ;
  _bOkActivated  = false ;

  _pGuidelineControls = (NSGuidelineControlVector *) 0 ;

  _pToolTip      = new TTooltip(this) ;
  _pToolTip->Attr.Style |= TTS_BALLOON ;

  nsSkin* pArchetypeSkin = pContexte->getSkins()->donneSkin("DecisionTree") ;
	if (pArchetypeSkin && pArchetypeSkin->getBackColor())
  	_BackgroundColor = *(pArchetypeSkin->getBackColor()) ;
  else
  	_BackgroundColor = NS_CLASSLIB::TColor(TColor::White) ;

  nsSkin *pSkin = pContexte->getSkins()->donneSkin("decisionTree_checked") ;
  if (pSkin)
    _pCheckedIconDib = pSkin->getBackBmp() ;

	ps = string("NSDecisionTreeView constructor, starting Archetype ") + _pDoc->getArchetype() ;
	pSuper->trace(&ps, 1) ;

	lanceArchetype() ;

  setViewName() ;

  _bAcceptUpdate  = false ;
  _bNeedToRefresh = false ;

  ps = string("Leaving NSDecisionTreeView constructor") ;
	pSuper->trace(&ps, 1) ;
}

NSDecisionTreeView::~NSDecisionTreeView()
{
try
{
  if (_pGuidelineControls)
    delete _pGuidelineControls ;
  if (_pToolTip)
    delete _pToolTip ;
}
catch (...)
{
  erreur("Exception NSDecisionTreeView destructor", standardError, 0) ;
}
}

void
NSDecisionTreeView::setViewName()
{
	// sViewName = "Guideline" ;
  _sViewName = _pDoc->findTitle() ;
}

voidNSDecisionTreeView::lanceArchetype(){try{  if (string("") == _pDoc->getArchetype())    return ;}catch (...)
{
  erreur("Exception NSCQVue::lanceArchetype.", standardError, 0) ;
}
}

TWindow*
NSDecisionTreeView::GetWindow()
{
  return (TWindow *) this ;
}

void
NSDecisionTreeView::SetupToolBar()
{
	// Ici pas de barre de boutons spécifique :
	// on se contente de virer la barre en cours
	// TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
	// pMyApp->FlushControlBar() ;

  _pPaneSplitter->FlushControlBar() ;

  _pPaneSplitter->_pGadgetPanelWindow->setButtonsStyle(uButtonsStyle) ;
  _pPaneSplitter->_pGadgetPanelWindow->insertMainGadgets() ;

/*
  if ((uButtonsStyle & MYWS_OK) == MYWS_OK)
		pPaneSplitter->Insert(*new TButtonGadget(IDC_OK_PANESPLIT,     IDC_OK_PANESPLIT,     TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_CANCEL) == MYWS_CANCEL)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_CANCEL_PANESPLIT, IDC_CANCEL_PANESPLIT, TButtonGadget::Command)) ;
	if ((uButtonsStyle & MYWS_HELP) == MYWS_HELP)
  	pPaneSplitter->Insert(*new TButtonGadget(IDC_HELP_PANESPLIT,   IDC_HELP_PANESPLIT,   TButtonGadget::Command)) ;
*/

  _pPaneSplitter->LayoutSession() ;
}

void
NSDecisionTreeView::PerformCreate(int menuOrId)
{
  string ps = string("NSDecisionTreeView, entrée dans PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

/*
  if (pDoc->pDocInfo)
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
*/

  ps = string("NSDecisionTreeView, sortie de PerformCreate") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  NSMUEView::PerformCreate(menuOrId) ;
}

void
NSDecisionTreeView::SetupWindow()
{
try
{
  char szThis[20] ;
  sprintf(szThis, "%p", HWindow) ;
  string psHeader = string("NSCQVue ") + string(szThis) + string(" : ") ;
  string ps = psHeader + string("Entering SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  NSMUEView::SetupWindow() ;

  SetCaption(_sViewName.c_str()) ;

  if (_pCreateWindow)
  {
    char szCreate[20] ;
    sprintf(szCreate, "%p", _pCreateWindow->HWindow) ;
    ps = psHeader + string("Create Window ") + string(szCreate) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }
  if (_pSplittedWindow)
  {
    char szSplitted[20] ;
    sprintf(szSplitted, "%p", _pSplittedWindow->HWindow) ;
    ps = psHeader + string("Splitted Window ") + string(szSplitted) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }
  if (_pPaneSplitter)
  {
    char szPane[20] ;
    sprintf(szPane, "%p", _pPaneSplitter->HWindow) ;
    ps = psHeader + string("PaneSplitter ") + string(szPane) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    if (_pPaneSplitter->_pMDIChild)
    {
      char szMdiChild[20] ;
      sprintf(szMdiChild, "%p", _pPaneSplitter->_pMDIChild->HWindow) ;
      ps = psHeader + string("PaneSplitter's Child ") + string(szMdiChild) ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    }
  }
  if (_pGadgetPanelWindow)
  {
    char szGadget[20] ;
    sprintf(szGadget, "%p", _pGadgetPanelWindow->HWindow) ;
    ps = psHeader + string("GadgetWindow ") + string(szGadget) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }

  string sLang = pContexte->getUserLanguage() ;

  Cdialogbox* pDialogBox = _pDoc->getParseur()->getGuideline()->getDialogBox(sLang) ;
	if (pDialogBox)
  {
    InitBasicMetrics(pDialogBox) ;

    _dlgRect = getWindowRectFromDialogRect(pDialogBox->getX(),
                                          pDialogBox->getY(),
                                          pDialogBox->getW(),
                                          pDialogBox->getH()) ;
  }

  CreerControlesArchetype() ;

  // Hide all windows but the first
	if (_pGuidelineControls && (false == _pGuidelineControls->empty()))
		for (iterNSGuidelineControl i = _pGuidelineControls->begin() ; _pGuidelineControls->end() != i ; i++)
    {
      if ((*i)->getControl())
      {
        // TWindow *pWindow = static_cast<TWindow*>((*i)->getControl()) ;
        // pWindow->Create() ;

        bool bChildIsVisible = (*i)->isVisible() ;

        switch ((*i)->getType())
        {
  	      case isGuidelineStatic :
          {
    	      NSGuidelineStatic* pStatic = static_cast<NSGuidelineStatic*>((*i)->getControl()) ;
            pStatic->Create() ;
            if (bChildIsVisible)
            {
              pStatic->Show(SW_SHOW) ;

              if (string("") != (*i)->getHelpText())
              {
                TToolInfo toolInfo(*this, pStatic->HWindow, (*i)->getHelpText().c_str()) ;
                _pToolTip->AddTool(toolInfo) ;
              }
            }
            else
              pStatic->Show(SW_HIDE) ;
            break ;
          }
          case isActionConcernCheckbox :
          {
    	      NSActionConcernCheckBox* pCheckbox = static_cast<NSActionConcernCheckBox*>((*i)->getControl()) ;
            pCheckbox->Create() ;
            if (bChildIsVisible)
            {
              pCheckbox->Show(SW_SHOW) ;

              if (string("") != (*i)->getHelpText())
              {
                TToolInfo toolInfo(*this, pCheckbox->HWindow, (*i)->getHelpText().c_str()) ;
                _pToolTip->AddTool(toolInfo) ;
              }
            }
            else
              pCheckbox->Show(SW_HIDE) ;
            break ;
          }
          case isDecisionButton :
          {
    	      NSDecisionButton* pButton = static_cast<NSDecisionButton*>((*i)->getControl()) ;
            pButton->Create() ;
            if (bChildIsVisible)
            {
              pButton->Show(SW_SHOW) ;

              if (string("") != (*i)->getHelpText())
              {
                TToolInfo toolInfo(*this, pButton->HWindow, (*i)->getHelpText().c_str()) ;
                _pToolTip->AddTool(toolInfo) ;
              }
            }
            else
              pButton->Show(SW_HIDE) ;
            break ;
          }
          case isDecisionRadioButton :
          {
    	      NSDecisionRadioButton* pRadioButton = static_cast<NSDecisionRadioButton*>((*i)->getControl()) ;
            pRadioButton->Create() ;
            if (bChildIsVisible)
            {
              pRadioButton->Show(SW_SHOW) ;

              if (string("") != (*i)->getHelpText())
              {
                TToolInfo toolInfo(*this, pRadioButton->HWindow, (*i)->getHelpText().c_str()) ;
                _pToolTip->AddTool(toolInfo) ;
              }
            }
            else
              pRadioButton->Show(SW_HIDE) ;
            break ;
          }
          case isSlotGroupBox :
          {
    	      NSSlotGroupBox* pGroupBox = static_cast<NSSlotGroupBox*>((*i)->getControl()) ;
            pGroupBox->Create() ;
            if (bChildIsVisible)
            {
              pGroupBox->Show(SW_SHOW) ;

              if (string("") != (*i)->getHelpText())
              {
                TToolInfo toolInfo(*this, pGroupBox->HWindow, (*i)->getHelpText().c_str()) ;
                _pToolTip->AddTool(toolInfo) ;
              }
            }
            else
              pGroupBox->Show(SW_HIDE) ;
            break ;
          }
          case isLinkGroupBox :
          {
    	      NSLinkGroupBox* pGroupBox = static_cast<NSLinkGroupBox*>((*i)->getControl()) ;
            if (pGroupBox)
            {
              pGroupBox->Create() ;
              if (bChildIsVisible)
              {
                pGroupBox->Show(SW_SHOW) ;

                if (string("") != (*i)->getHelpText())
                {
                  TToolInfo toolInfo(*this, pGroupBox->HWindow, (*i)->getHelpText().c_str()) ;
                  _pToolTip->AddTool(toolInfo) ;
                }
              }
              else
                pGroupBox->Show(SW_HIDE) ;
            }
            break ;
          }
          case isStandardGroup :
          {
    	      OWL::TGroupBox* pGroupBox = static_cast<OWL::TGroupBox*>((*i)->getControl()) ;
            if (pGroupBox)
            {
              pGroupBox->Create() ;
              if (bChildIsVisible)
              {
                pGroupBox->Show(SW_SHOW) ;

                if (string("") != (*i)->getHelpText())
                {
                  TToolInfo toolInfo(*this, pGroupBox->HWindow, (*i)->getHelpText().c_str()) ;
                  _pToolTip->AddTool(toolInfo) ;
                }
              }
              else
                pGroupBox->Show(SW_HIDE) ;
            }
            break ;
          }
        }

        // Reference control for ToolTip
        //
/*               Ne marche pas (???)
        if (bChildIsVisible && (string("") != (*i)->getHelpText()))
        {
          TWindow *pCtrlWin = static_cast<TWindow*>((*i)->getControl()) ;
	        if (pCtrlWin)
          {
            TToolInfo toolInfo(*this, pCtrlWin->HWindow, (*i)->getHelpText().c_str()) ;
            _pToolTip->AddTool(toolInfo) ;
          }
        }
*/

/*
        bool bChildIsVisible = (*i)->isVisible() ;
        if (bChildIsVisible)
          pWindow->Show(SW_SHOW) ;
        else
          pWindow->Show(SW_HIDE) ;
*/
      }
    }

  _pToolTip->Activate() ;

  _bAcceptUpdate = true ;

  // In case some information arrived during SetupWindow
  //
  if (_bNeedToRefresh)
    ::PostMessage(HWindow, WM_COMMAND, IDM_BBK_UPDATE, 0) ;

  _pDoc->run(HWindow) ;

  //pDoc->SetTitle(sTitle.c_str()) ;
  //SetDocTitle(sTitle.c_str(), 0) ;
  ps = string("NSDecisionTreeView ") + string(szThis) + string(", leaving SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;
}
catch (...)
{
  erreur("Exception NSDecisionTreeView::SetupWindow.", standardError, 0) ;
}
}

Cdialogbox*
NSDecisionTreeView::getDialogBox()
{
  if ((NULL == _pDoc->getParseur()) || (NULL == _pDoc->getParseur()->getGuideline()))
    return (Cdialogbox *) 0 ;

  string sLang = pContexte->getUserLanguage() ;

  return _pDoc->getParseur()->getGuideline()->getDialogBox(sLang) ;
}

void
NSDecisionTreeView::CmBbkUpdate()
{
  _bNeedToRefresh = true ;

  if (false == _bAcceptUpdate)
    return ;

  SetCursor(0, IDC_WAIT) ;

  string ps = string("Reseting Decision tree due to asynchronous BBK update") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  ResetControls() ;

  _pDoc->run(HWindow) ;

  Invalidate() ;

  SetCursor(0, IDC_ARROW) ;
}

void
NSDecisionTreeView::CreerControlesArchetype()
{
try
{
  if ((NULL == _pDoc) || (NULL == _pDoc->getParseur()))
    return ;

  nsGuidelineParseur* pGuidelineParser = _pDoc->getParseur() ;

	string sLang = pContexte->getUserLanguage() ;

  Cguideline *pGuideline = pGuidelineParser->getGuideline() ;
  if (NULL == pGuideline)
    return ;

  Cdialogbox * pDialogBox = pGuideline->getDialogBox(sLang) ;
  if (NULL == pDialogBox)
    return ;

  Ccontrol * pControl = pDialogBox->getFirstControl() ;
  if (NULL == pControl)
    return ;

  int             hSysButton = 0 ;
  OWL::TGroupBox *pGroupCurr = 0 ;

  _pGuidelineControls = new NSGuidelineControlVector() ;

  while (NULL != pControl)
  {
    CreateControl(pControl, pGroupCurr, hSysButton, sLang) ;

    string ps = string(" --> Get Next Control ") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

    pControl = pDialogBox->getNextControl(pControl) ;
  }

  // On fixe la caption du dialogue
  //
  string sDlgCaption = pDialogBox->getCaption() ;
  SetCaption(sDlgCaption.c_str()) ;
}
catch (...)
{
  erreur("Exception NSDecisionTreeView::CreerControlesArchetype.", standardError, 0) ;
}
}

TWindow*
NSDecisionTreeView::CreateControl(Ccontrol *pControl, OWL::TGroupBox *pGroupCurr, int &hSysButton, string sLang)
{
  if ((Ccontrol*) NULL == pControl)
    return (TWindow*) 0 ;

try
{
  int    ctrlID   = pControl->getRefId() ;

  // Never install an object for -1 as an Id
  //
  if (-1 == ctrlID)
    return 0 ;

  string sType     = pControl->getType() ;
  string sCoords   = pControl->getCoords() ;
  string sCaption  = pControl->getCaption() ;
  string sFilling  = pControl->getFilling() ;
  string sHelpText = pControl->getHelpText() ;

  // conversion du type en majuscules
  pseumaj(&sType) ;

  int coords[4] ;
  coords[0] = getWindowXFromDialogX(pControl->getX()) ;
  coords[1] = getWindowYFromDialogY(pControl->getY()) ;
  coords[2] = getWindowXFromDialogX(pControl->getW()) ;
  coords[3] = getWindowYFromDialogY(pControl->getH()) ;

  string sIdentity    = pControl->getDataIdentity() ;
  string sDlgFonction = pControl->getDataFunction() ;

  // -------------------------------------------------------------------------
  // Traitement suivant le type de contrôle rencontré

  // Static
  if ((string("STATIC") == sType) || (string("BORSTATIC") == sType))
  {
    // Création de l'objet OWL correspondant à l'objet d'interface
    NSGuidelineStatic* pStatic = new NSGuidelineStatic(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pStatic->Attr.Style = pControl->getIStyle() ;
    DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

    NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
    pControl->setControl(dynamic_cast<void *>(pStatic)) ;
    pControl->setType(pStatic->donneType()) ;
    pControl->setVisible(isControlProperty(pStatic, WS_VISIBLE)) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setView(this) ;
    _pGuidelineControls->push_back(pControl) ;

    pStatic->setControl(pControl) ;
    return pStatic ;
  }

  if ((string("BUTTON") == sType) &&
             (IDOK   != ctrlID) && (IDCANCEL   != ctrlID) &&
             (IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
  {
    // Contrôle Button (à voir pour OK et Cancel)

    // -----------------------------------------------------------------------
    // ATTENTION : Il faut penser à tester du plus grand au plus petit sinon
    // un groupbox répond oui à radiobouton (par exemple)
    // #define BS_PUSHBUTTON       0x00000000L
    // #define BS_DEFPUSHBUTTON    0x00000001L
    // #define BS_CHECKBOX         0x00000002L
    // #define BS_AUTOCHECKBOX     0x00000003L
    // #define BS_RADIOBUTTON      0x00000004L
    // #define BS_3STATE           0x00000005L
    // #define BS_AUTO3STATE       0x00000006L
    // #define BS_GROUPBOX         0x00000007L
    // #define BS_USERBUTTON       0x00000008L
    // #define BS_AUTORADIOBUTTON  0x00000009L

    // Détermination du style
    // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE);
    int nIndex = pControl->getIStyle() ;

    if ((nIndex & BS_AUTORADIOBUTTON) == BS_AUTORADIOBUTTON)    {
      // Contrôle AutoRadioButton
      return (TWindow*) 0 ; // NSBBMUEView::CreateControl(pControl, pGroupCurr, hSysButton, sLang) ;
    }
    if ((nIndex & BS_GROUPBOX) == BS_GROUPBOX)
    {
      // Contrôle Groupbox
      return (TWindow*) 0 ; // NSBBMUEView::CreateControl(pControl, pGroupCurr, hSysButton, sLang) ;
    }
    if ((nIndex & BS_RADIOBUTTON) == BS_RADIOBUTTON)
    {      // Contrôle RadioButton
      NSDecisionRadioButton* pRadiobutton = new NSDecisionRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
      pRadiobutton->Attr.Style = pControl->getIStyle() ;

      DTlink* pLink = _pDoc->getLinkById(sIdentity) ;

      NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pLink, sIdentity) ;
      pControl->setControl(dynamic_cast<void *>(pRadiobutton)) ;
      pControl->setType(pRadiobutton->donneType()) ;
      pControl->setVisible(isControlProperty(pRadiobutton, WS_VISIBLE)) ;
      pControl->setHelpText(sHelpText) ;
      pControl->setView(this) ;
      _pGuidelineControls->push_back(pControl) ;

      pRadiobutton->setControl(pControl) ;
      return pRadiobutton ;
    }
    if (((nIndex & BS_CHECKBOX) == BS_CHECKBOX) ||
        ((nIndex & BS_AUTOCHECKBOX) == BS_AUTOCHECKBOX))
    {
      if (string("CREEPREO") == sDlgFonction)
      {
        // CheckBox control for concern creation
        NSActionConcernCheckBox* pCheckbox = new NSActionConcernCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr, true) ;
        pCheckbox->Attr.Style = pControl->getIStyle() ;
        DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

        NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
        pControl->setControl(dynamic_cast<void *>(pCheckbox)) ;
        pControl->setType(pCheckbox->donneType()) ;
        pControl->setVisible(isControlProperty(pCheckbox, WS_VISIBLE)) ;
        pControl->setHelpText(sHelpText) ;
        pControl->setView(this) ;
        _pGuidelineControls->push_back(pControl) ;

        pCheckbox->setControl(pControl) ;
        return pCheckbox ;
      }
    }

    if (string("ARCHETYPE") == sDlgFonction)
    {
      // Button control that opens an archetype
      //
      NSDecisionButton *pButt = new NSDecisionButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
      pButt->Attr.Style = pControl->getIStyle() ;
      DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

      NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
      pControl->setControl(dynamic_cast<void *>(pButt)) ;
      pControl->setType(pButt->donneType()) ;
      pControl->setVisible(isControlProperty(pButt, WS_VISIBLE)) ;
      pControl->setHelpText(sHelpText) ;
      pControl->setView(this) ;
      _pGuidelineControls->push_back(pControl) ;

      pButt->setControl(pControl) ;

      return pButt ;
    }
  }
  if (string("BUTTON") == sType)
  {
    TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pButt->Attr.Style = pControl->getIStyle() ;

    // Les boutons OK et CANCEL ne sont pas créés dans la vue
    // OK and CANCEL buttons are not created inside the view
    //
    if ((IDOK == ctrlID) || (IDCANCEL == ctrlID))
    {
      if      (IDOK == ctrlID)
        uButtonsStyle |= MYWS_OK ;
      else if (IDCANCEL == ctrlID)
        uButtonsStyle |= MYWS_CANCEL ;
    }
    // IDB_OK and IDB_CANCEL are not created in windows (they are in dialogs)
    //
    else if ((IDB_OK != ctrlID) && (IDB_CANCEL != ctrlID))
    {
      // Bouton systeme : ne pas referencer dans les controles de tabbedWindows
      TButton* pButt = new TButton(this, ctrlID, sCaption.c_str(),
                                            coords[0], coords[1], coords[2], coords[3]);
      pButt->Attr.Style = pControl->getIStyle() ;

      if ((coords[1] + coords[3]) > hSysButton)
        hSysButton = coords[1] + coords[3] ;
    }
    return pButt ;
  }

  // BORLAND : Contrôle CheckBox
  if (string("BORCHECK") == sType)
  {
    NSActionConcernCheckBox* pCheckbox = new NSActionConcernCheckBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr, true) ;
    pCheckbox->Attr.Style = pControl->getIStyle() ;
    DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

    NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
    pControl->setControl(dynamic_cast<void *>(pCheckbox)) ;
    pControl->setType(pCheckbox->donneType()) ;
    pControl->setVisible(isControlProperty(pCheckbox, WS_VISIBLE)) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setView(this) ;
    _pGuidelineControls->push_back(pControl) ;

    pCheckbox->setControl(pControl) ;
    return pCheckbox ;
  }

  if (string("BORRADIO") == sType)
  {
    // BORLAND : Contrôle RadioBouton
    NSDecisionRadioButton* pRadiobutton = new NSDecisionRadioButton(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3], pGroupCurr) ;
    pRadiobutton->Attr.Style = pControl->getIStyle() ;

    DTlink* pLink = _pDoc->getLinkById(sIdentity) ;

    NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pLink, sIdentity) ;
    pControl->setControl(dynamic_cast<void *>(pRadiobutton)) ;
    pControl->setType(pRadiobutton->donneType()) ;
    pControl->setVisible(isControlProperty(pRadiobutton, WS_VISIBLE)) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setView(this) ;
    _pGuidelineControls->push_back(pControl) ;

    pRadiobutton->setControl(pControl) ;
    return pRadiobutton ;
  }

  // BORLAND : Contrôle BorShade
  if (string("BORSHADE") == sType)
  {
    // Détermination du style
    // int nIndex = ::GetWindowLong(hWnd, GWL_STYLE) ;
    int nIndex = pControl->getIStyle() ;

    if (((nIndex & BSS_RGROUP) == BSS_RGROUP) || ((nIndex & WS_GROUP) == WS_GROUP))
    {
      DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

      // Group attached to a Slot
      //
      if (pSlot)
      {
        CguidelineStep *pStep = pSlot->getStep() ;

        // Group attached to a Action slot
        //
        CactionStep *pAction = dynamic_cast<CactionStep *>(pStep) ;
        if (pAction)
        {
          NSActionSlotGroupBox * pGrpBox = new NSActionSlotGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;

          NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
          pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
          pControl->setType(pGrpBox->donneType()) ;
          pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
          pControl->setHelpText(sHelpText) ;
          pControl->setView(this) ;
          _pGuidelineControls->push_back(pControl) ;

          pGrpBox->setControl(pControl) ;
          return pGrpBox ;
        }

        // Group attached to a Decision slot
        //
        CdecisionStep *pDecision = dynamic_cast<CdecisionStep *>(pStep) ;
        if (pDecision)
        {
          NSDecisionSlotGroupBox * pGrpBox = new NSDecisionSlotGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;

          NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
          pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
          pControl->setType(pGrpBox->donneType()) ;
          pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
          pControl->setHelpText(sHelpText) ;
          pControl->setView(this) ;
          _pGuidelineControls->push_back(pControl) ;

          pGrpBox->setControl(pControl) ;
          return pGrpBox ;
        }

        // Group attached to a Synchronization slot
        //
        CsynchronizationStep *pSynchronization = dynamic_cast<CsynchronizationStep *>(pStep) ;
        if (pSynchronization)
        {
          NSSynchroSlotGroupBox * pGrpBox = new NSSynchroSlotGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
          pGrpBox->Attr.Style = pControl->getIStyle() ;

          NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
          pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
          pControl->setType(pGrpBox->donneType()) ;
          pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
          pControl->setHelpText(sHelpText) ;
          pControl->setView(this) ;
          _pGuidelineControls->push_back(pControl) ;

          pGrpBox->setControl(pControl) ;
          return pGrpBox ;
        }

        NSSlotGroupBox * pGrpBox = new NSSlotGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        pGrpBox->Attr.Style = pControl->getIStyle() ;
        DTslot* pSlot = _pDoc->getSlotById(sIdentity) ;

        NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pSlot, sIdentity, sDlgFonction) ;
        pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
        pControl->setType(pGrpBox->donneType()) ;
        pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
        pControl->setHelpText(sHelpText) ;
        pControl->setView(this) ;
        _pGuidelineControls->push_back(pControl) ;

        pGrpBox->setControl(pControl) ;
        return pGrpBox ;
      }

      DTlink* pLink = _pDoc->getLinkById(sIdentity) ;

      // Group attached to a link
      //
      if (pLink)
      {
        DTslot* pFromSlot = pLink->getSlotFrom() ;

        if (pFromSlot)
        {
          CguidelineStep *pStep = pFromSlot->getStep() ;

          CdecisionStep *pDecision = dynamic_cast<CdecisionStep *>(pStep) ;
          if (pDecision)
          {
            NSDecisionStepSlotGroupBox * pGrpBox = new NSDecisionStepSlotGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
            pGrpBox->Attr.Style = pControl->getIStyle() ;

            NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pLink, sIdentity) ;
            pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
            pControl->setType(pGrpBox->donneType()) ;
            pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
            pControl->setHelpText(sHelpText) ;
            pControl->setView(this) ;
            _pGuidelineControls->push_back(pControl) ;

            pGrpBox->setControl(pControl) ;
            return pGrpBox ;
          }
        }
      }
    }
    else if ((nIndex & BSS_VDIP) == BSS_VDIP)
    {
      // Contrôle Group
      if (string("") != sIdentity)
      {
        NSLinkGroupBox * pGrpBox = new NSLinkGroupBox(pContexte, this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
        // pGrpBox->Attr.Style = pControl->getIStyle() ;

        DTlink* pLink = _pDoc->getLinkById(sIdentity) ;

        NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, pLink, sIdentity) ;
        pControl->setControl(dynamic_cast<void *>(pGrpBox)) ;
        pControl->setType(pGrpBox->donneType()) ;
        pControl->setVisible(isControlProperty(pGrpBox, WS_VISIBLE)) ;
        pControl->setHelpText(sHelpText) ;
        pControl->setView(this) ;
        _pGuidelineControls->push_back(pControl) ;

        pGrpBox->setControl(pControl) ;
        return pGrpBox ;
      }
    }

    OWL::TGroupBox* pGroupBox = new OWL::TGroupBox(this, ctrlID, sCaption.c_str(), coords[0], coords[1], coords[2], coords[3]) ;
    pGroupBox->Attr.Style = pControl->getIStyle() ;
    NSGuidelineControl *pControl = new NSGuidelineControl(pContexte, (DTslot*) 0, sIdentity, sDlgFonction) ;

    pControl->setControl(dynamic_cast<void *>(pGroupBox)) ;
    pControl->setType(isStandardGroup) ;
    pControl->setVisible(isControlProperty(pGroupBox, WS_VISIBLE)) ;
    pControl->setHelpText(sHelpText) ;
    pControl->setView(this) ;
    _pGuidelineControls->push_back(pControl) ;

    // aGroups.push_back(pGroupBox) ;
    pGroupCurr = pGroupBox ;
    return pGroupBox ;
  }

  return (TWindow*) 0 ; // NSBBMUEView::CreateControl(pControl, pGroupCurr, hSysButton, sLang) ;
}
catch (...)
{
  string sErrorText = string("Exception NSDecisionTreeView::CreateControl().") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
  return (TWindow*) 0 ;
}
}

void
NSDecisionTreeView::ResetControls()
{
  if ((NULL == _pGuidelineControls) || (_pGuidelineControls->empty()))
    return ;

  for (iterNSGuidelineControl i = _pGuidelineControls->begin() ; _pGuidelineControls->end() != i ; i++)
  {
    if ((*i)->getControl())
    {
      (*i)->activeControle(isBlank) ;

      switch ((*i)->getType())
      {
        case isGuidelineStatic :
            break ;
        case isActionConcernCheckbox :
        {
          NSActionConcernCheckBox* pCheckbox = static_cast<NSActionConcernCheckBox*>((*i)->getControl()) ;
          if (pCheckbox)
            pCheckbox->setActivation(true) ;
          break ;
        }
        case isDecisionButton :
        {
          NSDecisionButton* pButton = static_cast<NSDecisionButton*>((*i)->getControl()) ;
          if (pButton)
            pButton->setActivation(true) ;
          break ;
        }
        case isDecisionRadioButton :
        {
          NSDecisionRadioButton* pRadioButton = static_cast<NSDecisionRadioButton*>((*i)->getControl()) ;
          if (pRadioButton)
            pRadioButton->setActivation(true) ;
          break ;
        }
        case isSlotGroupBox :
        {
          NSSlotGroupBox* pGroupBox = static_cast<NSSlotGroupBox*>((*i)->getControl()) ;
          if (pGroupBox)
            pGroupBox->setActivation(true) ;
          break ;
        }
        case isLinkGroupBox :
        {
          NSLinkGroupBox* pGroupBox = static_cast<NSLinkGroupBox*>((*i)->getControl()) ;
          if (pGroupBox)
            pGroupBox->setActivation(true) ;
          break ;
        }
        case isStandardGroup :
          break ;
      }
    }
  }
}

string
NSDecisionTreeView::getIdentityFrom(string sIdentity)
{
  if (string("") == sIdentity)
    return string("") ;

  size_t iPos = sIdentity.find("-") ;
  if (NPOS == iPos)
    return string("") ;

  string sFrom = string(sIdentity, 0, iPos) ;
  strip(sFrom) ;

  return sFrom ;
}

string
NSDecisionTreeView::getIdentityTo(string sIdentity)
{
  if (string("") == sIdentity)
    return string("") ;

  size_t iPos = sIdentity.find("-") ;
  if ((NPOS == iPos) || (strlen(sIdentity.c_str()) == iPos + 1))
    return string("") ;

  string sTo = string(sIdentity, iPos + 1, strlen(sIdentity.c_str()) - iPos - 1) ;
  strip(sTo) ;

  return sTo ;
}

void
NSDecisionTreeView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  int iAncDC = dc.SaveDC() ;

  OWL::TBrush Pinceau(_BackgroundColor) ;

  dc.FillRect(RectangleAPeindre, Pinceau) ;

  dc.RestoreDC(iAncDC) ;

  NSMUEView::Paint(dc, false, RectangleAPeindre) ;
}

TResult
NSDecisionTreeView::EvSetBackColor(TParam1 param1, TParam2 param2)
{
  HBRUSH hbr = (HBRUSH)DefWindowProc(GetCurrentEvent().Message, param1, param2) ;
  ::DeleteObject(hbr) ;
  SetBkMode((HDC)param1, TRANSPARENT) ;
  return (TResult)::GetStockObject(NULL_BRUSH) ;
}

void
NSDecisionTreeView::EvSetFocus(THandle hWndLostFocus)
{
	activateMUEViewMenu() ;

	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow()) ;
	}

  string sHelpUrl = _pDoc->getHelpUrl() ;
  if (string("") != sHelpUrl)
    pContexte->setAideCorps(sHelpUrl) ;

	NSMUEView::EvSetFocus(hWndLostFocus) ;
  _pPaneSplitter->SetFrameTitle(getFunction(), _sViewName) ;

  pMyApp->setMenu(string("menubar"), &_hAccelerator) ;
}

void
NSDecisionTreeView::CmEnregistre()
{
}

void
NSDecisionTreeView::EvClose()
{
}

void
NSDecisionTreeView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSDecisionTreeView::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvSysKeyDown(key, repeatCount, flags) ;
}

void
NSDecisionTreeView::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSDecisionTreeView::CmHelp()
{
  pContexte->NavigationAideEnLigne() ;
}

void
NSDecisionTreeView::CmOk()
{
  // If Ok already activated, do nothing
  //
  if (_bOkActivated)
    return ;

  _bOkActivated = true ;
}

void
NSDecisionTreeView::CmCancel()
{
}

void
NSDecisionTreeView::EvDestroy()
{
}

// -----------------------------------------------------------------------------// Document notifications// -----------------------------------------------------------------------------

bool
NSDecisionTreeView::CanClose()
{
  string ps = string("Entering NSDecisionTreeView::CanClose()") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  TMyApp *pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;

  _pDoc->SetDirty(false) ;
  pMyApp->FlushControlBar() ;
  _bSetupToolBar = false ;

  if (pContexte->getBBinterface())
  {
    char szHandle[20] ;
    sprintf(szHandle, "%p", HWindow) ;
    string ps = string("NSDecisionTreeView::CanClose disconnecting interface ") + string(szHandle) ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

    pContexte->getBBinterface()->DisconnectInterface(HWindow) ;
  }

  NSPredi *pPredi = pContexte->getPredi() ;
  if (pPredi)
    pPredi->fermeDecisionTree((TWindow*) this) ;

  return true ;
}

bool
NSDecisionTreeView::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;

  if (_pToolTip && _pToolTip->IsWindow())
    _pToolTip->RelayEvent(msg) ;

  if (NULL == _hAccelerator)
    return false ;

  bool bTranslated = ::TranslateAccelerator(GetHandle(), _hAccelerator, &msg) ;
  if (bTranslated)
    return true ;

  return false ;
}

// --------------------------------------------------------------------------
// ----------------- METHODES DE NSDecisionTreeDocument ---------------------
// --------------------------------------------------------------------------

NSDecisionTreeDocument::NSDecisionTreeDocument(TDocument* parent, NSContexte* pCtx, string sArc,
                                               int iTypeFormulaire, string sDefaultArc, bool bDefArch)
                       :TDocument(parent), NSRoot(pCtx)
{
  _iFrame       = ldvframeNotSpecified ;

	_pParseur      = 0 ;
	// Il s'agit ici d'un document vierge : on doit donc transmettre le nom
	// de l'archetype (car la patpatho est vide)
	_sArchetype        = sArc ;
  _sDefaultArchetype = sDefaultArc ;
  _bForceDefaultArch = bDefArch ;

  _pKSdescriptor = NULL ;

	_sHtmlView     = string("") ;
  _sSemDocType   = string("") ;

	_bCreate       = true ;
	_bParsingOk    = ouvreArchetype() ;
  if (_bParsingOk)
  {
    string sDocLib = findTitle() ;
    if (string("") != sDocLib)
      SetTitle(sDocLib.c_str()) ;

    createSlots() ;
    completeLinks() ;
  }

	_iTypeDoc      = iTypeFormulaire ;
}

NSDecisionTreeDocument::NSDecisionTreeDocument(TDocument* parent, NSContexte* pCtx, BB1BBInterfaceForKs* pKSdesc)
                       :TDocument(parent), NSRoot(pCtx)
{
  _iFrame    = ldvframeNotSpecified ;

	_pParseur  = 0 ;
	// Il s'agit ici d'un document vierge : on doit donc transmettre le nom
	// de l'archetype (car la patpatho est vide)
	_sArchetype        = pKSdesc->getArchetypeID() ;
  _sDefaultArchetype = string("") ;
  _bForceDefaultArch = false ;
  _pKSdescriptor     = new BB1BBInterfaceForKs(*pKSdesc) ;
	_sHtmlView         = string("") ;
  _sSemDocType       = string("") ;

	_bCreate       = true ;
  _pKSdescriptor->setInitFromBbk(true) ;

	_bParsingOk    = ouvreArchetype() ;
  if (_bParsingOk)
  {
    string sDocLib = findTitle() ;
    if (string("") != sDocLib)
      SetTitle(sDocLib.c_str()) ;

    createSlots() ;
    completeLinks() ;
  }
}

NSDecisionTreeDocument::NSDecisionTreeDocument(const NSDecisionTreeDocument& rv)
                       :TDocument(rv.GetParentDoc()), NSRoot(rv.pContexte)
{
	_pParseur          = new nsGuidelineParseur(*(rv._pParseur)) ;
	_sArchetype        = rv._sArchetype ;  _sSemDocType       = rv._sSemDocType ;  _sDefaultArchetype = rv._sDefaultArchetype ;  _bForceDefaultArch = rv._bForceDefaultArch ;  _iFrame            = rv._iFrame ;  if (rv._pKSdescriptor)  	_pKSdescriptor = new BB1BBInterfaceForKs(*(rv._pKSdescriptor)) ;  else  	_pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;	_sHtmlView   = rv._sHtmlView ;	_bCreate     = rv._bCreate ;	_bParsingOk  = rv._bParsingOk ;	_iTypeDoc    = rv._iTypeDoc ;}

NSDecisionTreeDocument::~NSDecisionTreeDocument()
{
	if (_pParseur)
		delete _pParseur ;

  if (_pKSdescriptor)
		delete _pKSdescriptor ;
}

NSDecisionTreeDocument&
NSDecisionTreeDocument::operator=(const NSDecisionTreeDocument& src)
{
  if (this == &src)
		return *this ;

	// Appel de l'operateur = de NSRoot
	// (recopie des arguments de la classe NSRoot)
	NSRoot       *dest = this ;
  const NSRoot *source = (NSRoot *) &src ;
	*dest = *source ;

	_pParseur          = new nsGuidelineParseur(*(src._pParseur));
	_sArchetype        = src._sArchetype ;  _sDefaultArchetype = src._sDefaultArchetype ;  _bForceDefaultArch = src._bForceDefaultArch ;  _iFrame            = src._iFrame ;  if (_pKSdescriptor)  	delete _pKSdescriptor ;  if (src._pKSdescriptor)  	_pKSdescriptor = new BB1BBInterfaceForKs(*(src._pKSdescriptor)) ;  else  	_pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;	_sHtmlView   = src._sHtmlView ;  _sSemDocType = src._sSemDocType ;	_bCreate     = src._bCreate ;	_bParsingOk  = src._bParsingOk ;	_iTypeDoc    = src._iTypeDoc ;
	return *this ;}

bool
NSDecisionTreeDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    						string sAdresseCorresp, string sPathDest)
{
  return false ;
}

string
NSDecisionTreeDocument::InitIntitule()
{
	string sIntituleDocument = string("") ;

	return sIntituleDocument ;
}

bool
NSDecisionTreeDocument::ouvreArchetype()
{
	if (string("") == _sArchetype)
    return false ;

	_pParseur = new nsGuidelineParseur(pContexte->getSuperviseur()) ;

	string sArchetypeFile = pContexte->getSuperviseur()->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::decisionTree, _sArchetype) ;
	if (string("") == sArchetypeFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
  	sErrorText += string(" (") + _sArchetype + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	if (false == _pParseur->open(sArchetypeFile))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("archetypesManagement", "archetypeParsingError") ;
  	sErrorText += string(" (") + sArchetypeFile + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  if (_pParseur->getGuideline())
  {
    Creferences *pRef = _pParseur->getGuideline()->getReference() ;
    if (pRef)
    {
      Cconcern  *pConcern = pRef->getFirstCconcern() ;
      if (pConcern)
      {
        string sFrameCode = pConcern->getFrame() ;
        if (string("") != sFrameCode)
        {
          string sFrameSens = string("") ;
          NSDico::donneCodeSens(&sFrameCode, &sFrameSens) ;

          for (int i = 0 ; i < FRAMECOUNT ; i++)
          {
            LDVFRAME iFrame = getFrameForIndex(i) ;
            string sRootForFrame = getRootForFrame(iFrame) ;
            string sSensForFrame = string("") ;
            NSDico::donneCodeSens(&sRootForFrame, &sSensForFrame) ;

            if (sSensForFrame == sFrameSens)
              _iFrame = iFrame ;
          }
        }
      }
    }
  }

  if (ldvframeNotSpecified == _iFrame)
    _iFrame = ldvframeHealth ;

	return true ;
}

// Méthodes de gestion des formulaires
bool
NSDecisionTreeDocument::enregistre(bool bVerbose, bool bSoaping)
{
  string ps = string("Entering NSCQDocument::enregistre") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  ps = string("Quitting NSCQDocument::enregistre") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	return false ;
}

string
NSDecisionTreeDocument::findTitle()
{
  string sLang = pContexte->getUserLanguage() ;

  string sLibelleDoc = string("") ;

  if ((_pParseur) && (_pParseur->getGuideline()))
  {
    Creferences *pReferenceBlock = _pParseur->getGuideline()->getReference() ;
    if (pReferenceBlock)
    {
      Chead *pReferenceHead = pReferenceBlock->getHead(sLang) ;
      if (pReferenceHead)
        sLibelleDoc = pReferenceHead->getTitle() ;
      if (string("") != sLibelleDoc)
        return sLibelleDoc ;
    }
  }

  if (string("") != _sSemDocType)
  {
    string sDocTypeComplet = _sSemDocType ;
    pContexte->getDico()->donneCodeComplet(sDocTypeComplet) ;
    pContexte->getDico()->donneLibelle(sLang, &sDocTypeComplet, &sLibelleDoc) ;
    return sLibelleDoc ;
  }

  return sLibelleDoc ;
}

string
NSDecisionTreeDocument::getHelpUrl()
{
  string sLang = pContexte->getUserLanguage() ;

  string sLibelleDoc = string("") ;

  if ((NULL == _pParseur) || (NULL == _pParseur->getGuideline()))
    return string("") ;

  return _pParseur->getGuideline()->getHelpUrl(sLang) ;
}

void
NSDecisionTreeDocument::openReferential()
{
}

string
NSDecisionTreeDocument::donneTexte()
{
  return string("") ;
}

void
NSDecisionTreeDocument::genereHtmlView()
{
}

// implementation des méthodes virtuelles de TDocument
bool
NSDecisionTreeDocument::Open(int mode, const char far* path)
{
	return _bParsingOk ;
}

bool
NSDecisionTreeDocument::Close()
{
	return true ;
}

void
NSDecisionTreeDocument::SetTitle(LPCSTR title)
{
	TDocument::SetTitle(title) ;
}

CguidelineStep*
NSDecisionTreeDocument::getGuidelineStepFromId(string sStepId)
{
  if ((string("") == sStepId) || (NULL == _pParseur))
    return (CguidelineStep*) 0 ;

  Cguideline* pGuideline = _pParseur->getGuideline() ;
  if (NULL == pGuideline)
    return (CguidelineStep*) 0 ;

  Csteps* pSteps = pGuideline->getSteps() ;
  if (NULL == pSteps)
    return (CguidelineStep*) 0 ;

  if (pSteps->vect_val.empty())
    return (CguidelineStep*) 0 ;

  for (ValIter ival = pSteps->vect_val.begin() ; pSteps->vect_val.end() != ival ; ival++)
  {
    if ((LABEL_STEP_DECISION      == (*ival)->getLabel()) ||
        (LABEL_STEP_ACTION        == (*ival)->getLabel()) ||
        (LABEL_STEP_BRANCH        == (*ival)->getLabel()) ||
        (LABEL_STEP_SYNCHRO       == (*ival)->getLabel()) ||
        (LABEL_STEP_PATIENT_STATE == (*ival)->getLabel()))
    {
      CguidelineStep *pStep = dynamic_cast<CguidelineStep *>((*ival)->getObject()) ;
      if ((pStep) && (pStep->getStepId() == sStepId))
        return pStep ;
    }
  }

  return (CguidelineStep*) 0 ;
}

void
NSDecisionTreeDocument::createSlots()
{
  if (NULL == _pParseur)
    return ;

  Cguideline* pGuideline = _pParseur->getGuideline() ;
  if (NULL == pGuideline)
    return ;

  Csteps* pSteps = pGuideline->getSteps() ;
  if (NULL == pSteps)
    return ;

  if (pSteps->vect_val.empty())
    return ;

  for (ValIter ival = pSteps->vect_val.begin() ; pSteps->vect_val.end() != ival ; ival++)
  {
    if      (LABEL_STEP_DECISION == (*ival)->getLabel())
		{
      CdecisionStep *pDecision = dynamic_cast<CdecisionStep *>((*ival)->getObject()) ;
      if (pDecision)
        createSlotAndLinksForDecision(pDecision) ;
		}
    else if (LABEL_STEP_ACTION == (*ival)->getLabel())
		{
      CactionStep *pAction = dynamic_cast<CactionStep *>((*ival)->getObject()) ;
      if (pAction)
        createSlotAndLinkForAction(pAction) ;
		}
    else if (LABEL_STEP_BRANCH == (*ival)->getLabel())
		{
      CbranchStep *pBranch = dynamic_cast<CbranchStep *>((*ival)->getObject()) ;
      if (pBranch)
        createSlotAndLinksForBranch(pBranch) ;
		}
    else if (LABEL_STEP_SYNCHRO == (*ival)->getLabel())
    {
      CsynchronizationStep *pSynchro = dynamic_cast<CsynchronizationStep *>((*ival)->getObject()) ;
      if (pSynchro)
        createSlotAndLinkForSynchronization(pSynchro) ;
    }
    else if (LABEL_STEP_PATIENT_STATE == (*ival)->getLabel())
    {
      CpatientStateStep *pPatientState = dynamic_cast<CpatientStateStep *>((*ival)->getObject()) ;
      if (pPatientState)
        createSlotAndLinkForPatientState(pPatientState) ;
    }
  }
}

void
NSDecisionTreeDocument::createSlotAndLinksForDecision(CdecisionStep *pDecision)
{
  if (NULL == pDecision)
    return ;

  // Creating slot
  //
  DTslot *pNewSlot = new DTslot(pDecision, pDecision->getStepId()) ;
  _aSlots.push_back(pNewSlot) ;

  // Get first option
  //
  CdecisionStepOption *pOption = pDecision->getNextOption((CdecisionStepOption *) 0) ;

  while (pOption)
  {
    // Creating link
    //
    DTlink *pNewLink = new DTlink(pNewSlot, pOption->getDestination(), pOption->getDecisionOptionId()) ;
    _aLinks.push_back(pNewLink) ;

    pNewLink->setInBetween(pOption) ;

    // Find next option
    //
    pOption = pDecision->getNextOption(pOption) ;
  }
}

void
NSDecisionTreeDocument::createSlotAndLinkForAction(CactionStep *pAction)
{
  if (NULL == pAction)
    return ;

  string sFrom = pAction->getStepId() ;

  // Creating slot
  //
  DTslot *pNewSlot = new DTslot(pAction, sFrom) ;
  _aSlots.push_back(pNewSlot) ;

  // Creating link
  //
  string sTo   = pAction->getNextStepId() ;
  string sLink = buildLinkId(sFrom, sTo) ;

  DTlink *pNewLink = new DTlink(pNewSlot, sTo, sLink) ;
  _aLinks.push_back(pNewLink) ;
}

void
NSDecisionTreeDocument::createSlotAndLinksForBranch(CbranchStep *pBranch)
{
  if (NULL == pBranch)
    return ;

  string sFrom = pBranch->getStepId() ;

  // Creating slot
  //
  DTslot *pNewSlot = new DTslot(pBranch, sFrom) ;
  _aSlots.push_back(pNewSlot) ;

  // Get first option
  //
  CbranchStepBranch *pSubBranch = pBranch->getNextBranch((CbranchStepBranch *) 0) ;

  while (pSubBranch)
  {
    // Creating link
    //
    string sTo   = pSubBranch->getNextStepId() ;
    string sLink = buildLinkId(sFrom, sTo) ;

    DTlink *pNewLink = new DTlink(pNewSlot, sTo, sLink) ;
    _aLinks.push_back(pNewLink) ;

    pNewLink->setInBetween(pSubBranch) ;

    // Find next option
    //
    pSubBranch = pBranch->getNextBranch(pSubBranch) ;
  }
}

void
NSDecisionTreeDocument::createSlotAndLinkForSynchronization(CsynchronizationStep *pSynchro)
{
  if (NULL == pSynchro)
    return ;

  string sFrom = pSynchro->getStepId() ;

  // Creating slot
  //
  DTslot *pNewSlot = new DTslot(pSynchro, sFrom) ;
  _aSlots.push_back(pNewSlot) ;

  // Creating link
  //
  string sTo   = pSynchro->getNextStepId() ;
  string sLink = buildLinkId(sFrom, sTo) ;

  DTlink *pNewLink = new DTlink(pNewSlot, sTo, sLink) ;
  _aLinks.push_back(pNewLink) ;
}

void
NSDecisionTreeDocument::createSlotAndLinkForPatientState(CpatientStateStep *pPatientState)
{
  if (NULL == pPatientState)
    return ;

  string sFrom = pPatientState->getStepId() ;

  // Creating slot
  //
  DTslot *pNewSlot = new DTslot(pPatientState, sFrom) ;
  _aSlots.push_back(pNewSlot) ;

  // Creating link
  //
  string sTo   = pPatientState->getNextStepId() ;
  string sLink = buildLinkId(sFrom, sTo) ;

  DTlink *pNewLink = new DTlink(pNewSlot, sTo, sLink) ;
  _aLinks.push_back(pNewLink) ;
}

// Since Links are created by their originating Slot, we have to connect
// destination Slot(s) at a second time
//
void
NSDecisionTreeDocument::completeLinks()
{
  if (_aLinks.empty())
    return ;

  for (DTlinkIter iter = _aLinks.begin() ; _aLinks.end() != iter ; iter++)
  {
    if (NULL == (*iter)->getSlotTo())
    {
      string sStepTo = (*iter)->getIdStepTo() ;
      if (string("") != sStepTo)
        (*iter)->setSlotTo(getSlotById(sStepTo)) ;
    }
  }
}

void
NSDecisionTreeDocument::run(HWND interfaceHandle)
{
  string ps = string("Running decision tree ") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  _aTokens.vider() ;

  DTslot* pStartingSlot = getSlotById(string("start")) ;
  if (NULL == pStartingSlot)
    return ;

  Cglobalvars *pGVars = _pParseur->getGuideline()->getGVars() ;
  if (pGVars)
  {
    pGVars->resetProcessState() ;
    pGVars->process(pContexte, true, interfaceHandle) ;
  }

  startToken(pStartingSlot, (DTslot*) 0) ;

  bool bProgressMade = runOneCycle() ;
  while (bProgressMade)
    bProgressMade = runOneCycle() ;
}

bool
NSDecisionTreeDocument::runOneCycle()
{
  if (_aTokens.empty())
    return false ;

  bool bProgressMade = false ;

  // Recording _aTokens.end() now so that tokens that may be pushed_back in the
  // loop are not runned this time
  //
  DTtokenIter endTokenIter = _aTokens.end() ;

  for (DTtokenIter tokenIter = _aTokens.begin() ; endTokenIter != tokenIter ; tokenIter++)
  {
    DTtoken *pToken = *tokenIter ;

    // What slot is this token on?
    //
    DTslot* pSlot = pToken->getCurrentSlot() ;
    if (pSlot && (DTtoken::isFree == pToken->getTokenState()))
    {
      CguidelineStep *pStep = pSlot->getStep() ;

      CpatientStateStep *pPatientState = dynamic_cast<CpatientStateStep *>(pStep) ;

      // Token on Patient State slot
      //
      if (pPatientState)
      {
        string ps = string("dtToken ") + pToken->getTokenIdAsString() + string(" is on Patient State slot ") + pPatientState->getStepId() ;

        string sNextStepId = pPatientState->getNextStepId() ;
        DTslot* pNextSlot = getSlotById(sNextStepId) ;
        if (pNextSlot)
        {
          ps += string(" and will move to slot ") + sNextStepId + string(" (") + pNextSlot->getTitle("") + string(")") ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

          bProgressMade = moveToken(pToken, pNextSlot) ;
        }
        else
        {
          ps += string(" and should have moved to missing slot ") + sNextStepId ;
          pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
        }
      }
      else
      {
        CactionStep *pAction = dynamic_cast<CactionStep *>(pStep) ;

        // Token on Action slot
        //
        if (pAction)
        {
          string ps = string("dtToken ") + pToken->getTokenIdAsString() + string(" is on Action slot ") + pAction->getStepId() + string(" (") + pAction->getTitle("") + string(")")  ;

          string sNextStepId = pAction->getNextStepId() ;
          DTslot* pNextSlot = getSlotById(sNextStepId) ;
          if (pNextSlot)
          {
            ps += string(" and will move to slot ") + sNextStepId + string(" (") + pNextSlot->getTitle("") + string(")") ;
            pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

            bProgressMade = moveToken(pToken, pNextSlot) ;
          }
          else
          {
            ps += string(" and should have moved to missing slot ") + sNextStepId ;
            pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
          }
        }
        else
        {
          CdecisionStep *pDecision = dynamic_cast<CdecisionStep *>(pStep) ;

          // Token on Decision slot
          //
          if (pDecision)
          {
            // Options are checked until a valid one is found
            //
            bool bLocalProgressMade = false ;
            pToken->setTokenState(DTtoken::isBlocked) ;

            bool bMultiTokens = false ;
            if (string("inclusive") == pDecision->getType())
              bMultiTokens = true ;

            bool bMainTokenAllocated = false ;

            CdecisionStepOption *pDecisionOption = pDecision->getNextOption((CdecisionStepOption *) 0) ;
            while (pDecisionOption && ((false == bLocalProgressMade) || bMultiTokens))
            {
              string sLinkId = pDecisionOption->getDecisionOptionId() ;

              Ccontrainte* pValidator = pDecisionOption->getValidator() ;
              Cglobalvars *pGVars = _pParseur->getGuideline()->getGVars() ;
              if (pValidator && pGVars)
              {
                NSValidateur valid(pValidator, pContexte) ;
                valid.setGlobalVars(pGVars) ;
                if (valid.Validation())
                {
                  string sNextStepId = pDecisionOption->getDestination() ;

                  string ps = string("dtToken ") + pToken->getTokenIdAsString() + string(" is on Decision slot ") + pDecision->getStepId() + string(" (") + pDecision->getTitle("") + string(")") + string(" and validated option ") + sLinkId ;

                  DTslot* pNextSlot = getSlotById(sNextStepId) ;
                  if (pNextSlot)
                  {
                    // Pushing main token to next step
                    //
                    if (false == bMainTokenAllocated)
                    {
                      ps += string(" and will move to slot ") + sNextStepId + string(" (") + pNextSlot->getTitle("") + string(")") ;
                      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

                      bProgressMade = moveToken(pToken, pNextSlot) ;

                      bMainTokenAllocated = true ;
                    }
                    //
                    // Creating a new token for next step
                    //
                    else
                    {
                      string ps = string("From (inclusive) Decision slot ") + pDecision->getStepId() + string(" (") + pDecision->getTitle("") + string(")") + string(" a new token is created to slot ") + sNextStepId + string(" (") + pNextSlot->getTitle("") + string(")") ;
                      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

                      startToken(pNextSlot, pSlot) ;
                    }
                  }
                  else
                  {
                    ps += string(" and should have moved to missing slot ") + sNextStepId ;
                    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
                  }

                  activateLink(sLinkId, isGreen) ;
                  bLocalProgressMade = true ;
                }
                else
                  activateLink(sLinkId, isRed) ;
              }
              pDecisionOption = pDecision->getNextOption(pDecisionOption) ;
            }
            // Remaining options are painted red
            //
            while (pDecisionOption)
            {
              string sLinkId = pDecisionOption->getDecisionOptionId() ;
              activateLink(sLinkId, isRed) ;
              pDecisionOption = pDecision->getNextOption(pDecisionOption) ;
            }
          }
          else
          {
            CbranchStep *pBranch = dynamic_cast<CbranchStep *>(pStep) ;

            // Token on Branch slot
            //
            if (pBranch)
            {
              bool bLocalProgressMade = false ;

              CbranchStepBranch *pSubBranch = pBranch->getNextBranch((CbranchStepBranch *) 0) ;
              while (pSubBranch)
              {
                string sNextId = pSubBranch->getNextStepId() ;
                DTslot* pNextSlot = getSlotById(sNextId) ;
                if (pNextSlot)
                {
                  if (false == bLocalProgressMade)
                  {
                    string ps = string("dtToken ") + pToken->getTokenIdAsString() + string(" is on Branch slot ") + pBranch->getStepId()+ string(" (") + pBranch->getTitle("") + string(")") + string(" and will move to slot ") + sNextId + string(" (") + pNextSlot->getTitle("") + string(")") ;
                    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

                    bLocalProgressMade = moveToken(pToken, pNextSlot) ;
                  }
                  else
                  {
                    string ps = string("From Branch slot ") + pBranch->getStepId() + string(" (") + pBranch->getTitle("") + string(")") + string(" a new token is created to slot ") + sNextId + string(" (") + pNextSlot->getTitle("") + string(")") ;
                    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

                    startToken(pNextSlot, pSlot) ;
                  }
                }

                string sLinkId = pSubBranch->getSubBranchId() ;
                activateLink(sLinkId, isGreen) ;

                pSubBranch = pBranch->getNextBranch(pSubBranch) ;
              }

              if (bLocalProgressMade)
                bProgressMade = true ;
            }
            else
            {
              CsynchronizationStep *pSynchro = dynamic_cast<CsynchronizationStep *>(pStep) ;

              // Token on Synchronization slot
              //
              if (pSynchro)
              {
                string sLinkId = buildLinkId(pSlot->getSlotId(), pSynchro->getNextStepId()) ;

                if (processSynchro(pSlot))
                {
                  string sStepId     = pSynchro->getStepId() ;
                  string sNextStepId = pSynchro->getNextStepId() ;

                  string ps = string("dtToken ") + pToken->getTokenIdAsString() + string(" is on Synchronization slot ") + sStepId + string(" (") + pSynchro->getTitle("") + string(")")  ;

                  if (string("end") == sStepId)
                  {
                    pToken->setTokenState(DTtoken::isToDelete) ;
                    ps += string(" and is marked for deletion") ;
                    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
                  }
                  else
                  {
                    DTslot* pNextSlot = getSlotById(sNextStepId) ;
                    if (pNextSlot)
                    {
                      ps += string(" and will move to slot ") + sNextStepId + string(" (") + pNextSlot->getTitle("") + string(")") ;
                      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

                      bProgressMade = moveToken(pToken, pNextSlot) ;

                      activateLink(sLinkId, isGreen) ;
                    }
                    else
                    {
                      ps += string(" and should have moved to missing slot ") + sNextStepId ;
                      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
                    }
                  }

                  // Kill other tokens
                  //
                  for (DTtokenIter tokenIter = _aTokens.begin() ; _aTokens.end() != tokenIter ; tokenIter++)
                    if (((*tokenIter) != pToken) && ((*tokenIter)->getCurrentSlot() == pSlot))
                    {
                      (*tokenIter)->setTokenState(DTtoken::isToDelete) ;
                      string ps = string("dtToken ") + (*tokenIter)->getTokenIdAsString() + string(" is marked for deletion") ;
                      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
                    }
                }
                else
                  activateLink(sLinkId, isRed) ;
              }
            }
          }
        }
      }
    }

    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
  }

  // Killing tokens to be deleted (of course not to be done inside previous loop)
  //
  for (DTtokenIter tokenIter = _aTokens.begin() ; _aTokens.end() != tokenIter ; )
  {
    if (DTtoken::isToDelete == (*tokenIter)->getTokenState())
    {
      delete *tokenIter ;
      _aTokens.erase(tokenIter) ;
    }
    else
      tokenIter++ ;
  }

  return bProgressMade ;
}

// -------------------------------------------------------------------------
// Moves a token to a new slot
//
// returns true if the token moved (means new slot != current slot)
// -------------------------------------------------------------------------
bool
NSDecisionTreeDocument::moveToken(DTtoken *pToken, DTslot* pToSlot)
{
  if ((NULL == pToken) || (NULL == pToSlot))
    return false ;

  // Update the "previous slot" information
  //
  DTslot* pCurrentSlot = pToken->getCurrentSlot() ;
  if (pCurrentSlot)
    pToken->setPreviousSlotId(pCurrentSlot->getSlotId()) ;
  else
    pToken->setPreviousSlotId(string("")) ;

  // Attach the token to its new slot
  //
  pToken->setCurrentSlot(pToSlot) ;

  // Activate the slot
  //
  activateSlot(pToSlot) ;

  // Set the state to "free"
  //
  pToken->setTokenState(DTtoken::isFree) ;

  return (pToSlot != pCurrentSlot) ;
}

void
NSDecisionTreeDocument::activateSlot(DTslot* pToSlot)
{
  if (NULL == pToSlot)
    return ;

  pToSlot->setState(isGreen) ;
}

// -------------------------------------------------------------------------
// Puts a new token on a slot
// -------------------------------------------------------------------------
void
NSDecisionTreeDocument::startToken(DTslot* pToSlot, DTslot* pFromSlot)
{
  if (NULL == pToSlot)
    return ;

  DTtoken *pToken = new DTtoken() ;

  // Set token's Id
  //
  if (_aTokens.empty())
    pToken->setTokenId(1) ;
  else
  {
    int iMaxId = 0 ;
    for (DTtokenIter tokenIter = _aTokens.begin() ; _aTokens.end() != tokenIter ; tokenIter++)
      if ((*tokenIter)->getTokenId() > iMaxId)
        iMaxId = (*tokenIter)->getTokenId() ;
    iMaxId++ ;
    pToken->setTokenId(iMaxId) ;
  }

  // Update the "previous slot" information
  //
  if (pFromSlot)
    pToken->setPreviousSlotId(pFromSlot->getSlotId()) ;
  else
    pToken->setPreviousSlotId(string("")) ;

  // Attach the token to its new slot
  //
  pToken->setCurrentSlot(pToSlot) ;

  // Activate the slot
  //
  activateSlot(pToSlot) ;

  // Set the state to "free"
  //
  pToken->setTokenState(DTtoken::isFree) ;

  _aTokens.push_back(pToken) ;
}

bool
NSDecisionTreeDocument::processSynchro(DTslot *pSlot)
{
  if (NULL == pSlot)
    return false ;

  CguidelineStep *pStep = pSlot->getStep() ;
  if (NULL == pStep)
    return false ;

  CsynchronizationStep *pSynchro = dynamic_cast<CsynchronizationStep *>(pStep) ;
  if (NULL == pSynchro)
    return false ;

  string sLogicalExpression = pSynchro->getLogicalExpression() ;

  size_t iIndex = 0 ;
  return Interprete(sLogicalExpression, iIndex, pSlot) ;
}

int
NSDecisionTreeDocument::Interprete(string sLogicalExpression, size_t& cc, DTslot *pSlot)
{
  int  result = 1 ;
  int  result1 ;
  char oper = ' ' ;
  bool bNegation ;

  while (cc < strlen(sLogicalExpression.c_str()))  {
    result1 = 1 ;
    bNegation = false ;

    while ((cc < strlen(sLogicalExpression.c_str())) && (' ' == sLogicalExpression[cc]))      cc++ ;

    // on évalue l'opérande en cours    if (cc < strlen(sLogicalExpression.c_str()))
    {
      // on évalue d'abord la négation
      if ('!' == sLogicalExpression[cc])
      {
        cc++ ;
        bNegation = true ;
      }

      if (cc == strlen(sLogicalExpression.c_str()))
      {
        string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sLogicalExpression ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }

      // on doit avoir ici une '(' ou un nom de contrainte
      if (carAutorise(sLogicalExpression[cc], true))
      {
        string sOriginSlotName = string("") ;
        while ((cc < strlen(sLogicalExpression.c_str())) && carAutorise(sLogicalExpression[cc]))
        {
          sOriginSlotName += string(1, sLogicalExpression[cc]) ;
          cc++ ;
        }
        if (string("") != sOriginSlotName)
          result1 = isTokenOnSlot(sOriginSlotName, pSlot) ;
      }
      else if ('(' == sLogicalExpression[cc])
      {
        cc++ ;
        result1 = Interprete(sLogicalExpression, cc, pSlot) ;
      }
      else
      {
        string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sLogicalExpression ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }
    }

    // cas erreur à l'évaluation de l'opérande    if (-1 == result1)
      return -1 ;
    if (bNegation)
      result1 = !result1 ;

    // on calcule le résultat selon l'opérateur en cours    if      (' ' == oper)
      result = result1 ;
    else if ('|' == oper)
      result = result || result1 ;
    else if ('&' == oper)
      result = result && result1 ;

    // on avance à nouveau    while ((cc < strlen(sLogicalExpression.c_str())) && (' ' == sLogicalExpression[cc]))
      cc++ ;

    // on évalue l'opérateur    if (cc < strlen(sLogicalExpression.c_str()))
    {
      // on doit avoir ici une ')' ou un opérateur
      if (('|' == sLogicalExpression[cc]) || ('&' == sLogicalExpression[cc]))
      {
        oper = sLogicalExpression[cc] ;
        cc++ ;
      }
      else if (')' == sLogicalExpression[cc])
      {
        cc++ ;
        return result ;
      }
      else
      {
        string sErrorText = pContexte->getSuperviseur()->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sLogicalExpression ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }
    }
  }
  return result ;
}

bool
NSDecisionTreeDocument::isTokenOnSlot(string sOriginSlotName, DTslot *pSlot)
{
  if ((NULL == pSlot) || (string("") == sOriginSlotName))
    return false ;

  if (_aTokens.empty())
    return false ;

  for (DTtokenIter tokenIter = _aTokens.begin() ; _aTokens.end() != tokenIter ; tokenIter++)
    if (((*tokenIter)->getCurrentSlot()    == pSlot) &&
        ((*tokenIter)->getPreviousSlotId() == sOriginSlotName))
      return true ;

  return false ;
}

bool
NSDecisionTreeDocument::carAutorise(char c, bool debut)
{
  if      ((!debut) && (c >= '0') && (c <= '9'))
    return true ;
  else if ((c >= 'A') && (c <= 'Z'))
    return true ;
  else if ((c >= 'a') && (c <= 'z'))
    return true ;
  else if ((!debut) && ((c == '_') || (c == '-') || (c == '+')))
    return true ;

  return false ;
}

void
NSDecisionTreeDocument::activateLink(string sLinkId, ACTIVATIONSTATE iState)
{
  if (string("") == sLinkId)
    return ;

  DTlink* pLink = getLinkById(sLinkId) ;
  if (pLink)
    pLink->setState(iState) ;
}

DTslot*
NSDecisionTreeDocument::getSlotById(string sSlotId)
{
  if (_aSlots.empty())
    return (DTslot *) 0 ;

  for (DTslotIter iter = _aSlots.begin() ; _aSlots.end() != iter ; iter++)
    if ((*iter)->getSlotId() == sSlotId)
      return *iter ;

  return (DTslot *) 0 ;
}

DTlink*
NSDecisionTreeDocument::getLinkById(string sLinkId)
{
  if (_aLinks.empty())
    return (DTlink *) 0 ;

  for (DTlinkIter iter = _aLinks.begin() ; _aLinks.end() != iter ; iter++)
    if ((*iter)->getLinkId() == sLinkId)
      return *iter ;

  return (DTlink *) 0 ;
}

string
NSDecisionTreeDocument::buildLinkId(string sFrom, string sTo)
{
  return sFrom + string("-") + sTo ;
}

// -----------------------------------------------------------------------------
//                         NSGuidelineInterfaceElement
// -----------------------------------------------------------------------------

NSGuidelineInterfaceElement::NSGuidelineInterfaceElement(NSContexte* pCtx)
                            :NSRoot(pCtx)
{
  _pControl         = (NSGuidelineControl *) 0 ;
  _iActivationState = isBlank ;
}

NSGuidelineInterfaceElement::~NSGuidelineInterfaceElement()
{
  if ((_pControl) && (_pControl->getControl()))
    _pControl->setControl((void *) 0) ;
}

// -----------------------------------------------------------------------------
//                                   DTslot
// -----------------------------------------------------------------------------

DTslot::DTslot()
{
  _pInterfaceControler = new NSGuidelineControlVector() ;
  _pGuidelineStep      = (CguidelineStep *) 0 ;
  _sSlotId             = string("") ;
}

DTslot::DTslot(CguidelineStep *pStep, string sSlotId)
{
  _pInterfaceControler = new NSGuidelineControlVector() ;
  _pGuidelineStep      = pStep ;
  _sSlotId             = sSlotId ;
}

DTslot::~DTslot()
{
  // Manually empty controler, so that it's content is not destroyed
  //
  if (_pInterfaceControler)
  {
    _pInterfaceControler->emptyWithoutDelete() ;
    delete _pInterfaceControler ;
  }
}

void
DTslot::addControl(NSGuidelineControl *pGuidelineControl)
{
  if ((NULL == pGuidelineControl) || (NULL == _pInterfaceControler))
    return ;

  _pInterfaceControler->push_back(pGuidelineControl) ;
}

void
DTslot::removeControl(NSGuidelineControl *pGuidelineControl)
{
  if (_pInterfaceControler)
    _pInterfaceControler->eraseControl(pGuidelineControl) ;
}

DTslot::DTslot(const DTslot& rv)
{
  _pInterfaceControler = new NSGuidelineControlVector(*(rv._pInterfaceControler)) ;
  _pGuidelineStep      = rv._pGuidelineStep ;
  _sSlotId             = rv._sSlotId ;
}

void
DTslot::setState(ACTIVATIONSTATE iState)
{
  if ((NULL == _pInterfaceControler) || _pInterfaceControler->empty())
    return ;

  for (iterNSGuidelineControl i = _pInterfaceControler->begin() ; _pInterfaceControler->end() != i ; i++)
    (*i)->activeControle(iState) ;
}

DTslot&
DTslot::operator=(const DTslot& src)
{
  if (this == &src)
		return *this ;

  // Manually empty controler, so that it's content is not destroyed
  //
  _pInterfaceControler->emptyWithoutDelete() ;

  *_pInterfaceControler = *(src._pInterfaceControler) ;
  _pGuidelineStep       = src._pGuidelineStep ;
  _sSlotId              = src._sSlotId ;

  return *this ;
}

string
DTslot::getTitle(string sLang)
{
  if (NULL == _pGuidelineStep)
    return string("") ;

  return _pGuidelineStep->getTitle(sLang) ;
}

// -----------------------------------------------------------------------------
//                                   DTlink
// -----------------------------------------------------------------------------

DTlink::DTlink()
{
  _pInterfaceControler = new NSGuidelineControlVector ;
  _pSlotFrom           = (DTslot *) 0 ;
  _pSlotTo             = (DTslot *) 0 ;
  _pInBetween          = (Cbalise *) 0 ;
  _sIdStepFrom         = string("") ;
  _sIdStepTo           = string("") ;
  _sLinkId             = string("") ;
}

DTlink::DTlink(DTslot* pSlotFrom, string sSlotIdTo, string sLinkId)
{
  _pInterfaceControler = new NSGuidelineControlVector ;
  _pSlotTo             = (DTslot *) 0 ;
  _pInBetween          = (Cbalise *) 0 ;

  _pSlotFrom           = pSlotFrom ;
  _sIdStepFrom         = pSlotFrom->getSlotId() ;
  _sIdStepTo           = sSlotIdTo ;
  _sLinkId             = sLinkId ;
}

DTlink::~DTlink()
{
  // Manually empty controler, so that it's content is not destroyed
  //
  if (_pInterfaceControler)
  {
    _pInterfaceControler->emptyWithoutDelete() ;
    delete _pInterfaceControler ;
  }
}

void
DTlink::addControl(NSGuidelineControl *pGuidelineControl)
{
  if ((NULL == pGuidelineControl) || (NULL == _pInterfaceControler))
    return ;

  _pInterfaceControler->push_back(pGuidelineControl) ;
}

void
DTlink::removeControl(NSGuidelineControl *pGuidelineControl)
{
  if (_pInterfaceControler)
    _pInterfaceControler->eraseControl(pGuidelineControl) ;
}

void
DTlink::setState(ACTIVATIONSTATE iState)
{
  if ((NULL == _pInterfaceControler) || _pInterfaceControler->empty())
    return ;

  for (iterNSGuidelineControl i = _pInterfaceControler->begin() ; _pInterfaceControler->end() != i ; i++)
    (*i)->activeControle(iState) ;
}

DTlink::DTlink(const DTlink& rv)
{
  _pInterfaceControler = new NSGuidelineControlVector(*(rv._pInterfaceControler)) ;
  _pSlotFrom           = rv._pSlotFrom ;
  _pSlotTo             = rv._pSlotTo ;
  _pInBetween          = rv._pInBetween ;
  _sIdStepFrom         = rv._sIdStepFrom ;
  _sIdStepTo           = rv._sIdStepTo ;
  _sLinkId             = rv._sLinkId ;
}

DTlink&
DTlink::operator=(const DTlink& src)
{
  if (this == &src)
		return *this ;

  // Manually empty controler, so that it's content is not destroyed
  //
  _pInterfaceControler->emptyWithoutDelete() ;

  *_pInterfaceControler = *(src._pInterfaceControler) ;
  _pSlotFrom            = src._pSlotFrom ;
  _pSlotTo              = src._pSlotTo ;
  _pInBetween           = src._pInBetween ;
  _sIdStepFrom          = src._sIdStepFrom ;
  _sIdStepTo            = src._sIdStepTo ;
  _sLinkId              = src._sLinkId ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                                   DTtoken
// -----------------------------------------------------------------------------

DTtoken::DTtoken()
{
  _pCurrentSlot    = (DTslot*) 0 ;
  _sPreviousSlotId = string("") ;
  _iState          = isFree ;
}

DTtoken::~DTtoken()
{
}

DTtoken::DTtoken(const DTtoken& rv)
{
  _pCurrentSlot    = rv._pCurrentSlot ;
  _sPreviousSlotId = rv._sPreviousSlotId ;
  _iState          = rv._iState ;
}

void
DTtoken::setCurrentSlot(DTslot* pSlot)
{
  _pCurrentSlot = pSlot ;
}

DTtoken&
DTtoken::operator=(const DTtoken& src)
{
  if (this == &src)
		return *this ;

  _pCurrentSlot    = src._pCurrentSlot ;
  _sPreviousSlotId = src._sPreviousSlotId ;
  _iState          = src._iState ;

  return *this ;
}

//***************************************************************************
// Implémentation des méthodes NSGuidelineControl
//***************************************************************************

//
// Constructeur par défaut
//
NSGuidelineControl::NSGuidelineControl(NSContexte* pCtx)
                   :NSRoot(pCtx)
{
  _pView     = (NSDecisionTreeView *) 0 ;
	_pSlot     = (DTslot *) 0 ;
  _pLink     = (DTlink *) 0 ;
	_pNSCtrl   = (TWindow *) 0 ;
	_sIdentity = string("") ;
  _sFctName  = string("") ;
  _bVisible  = true ;
}

////  Constructor for Slot connection
//
NSGuidelineControl::NSGuidelineControl(NSContexte* pCtx, DTslot* pSlot, string sIdent, string sFctName)
                   :NSRoot(pCtx)
{
try
{
	_sIdentity = sIdent ;
  _sFctName  = sFctName ;
	_pSlot     = pSlot ;
  _pLink     = (DTlink *) 0 ;
  _pView     = (NSDecisionTreeView *) 0 ;
	_pNSCtrl   = (TWindow *) 0 ;
  _bVisible  = true ;

  if (_pSlot)
    _pSlot->addControl(this) ;
}
catch (...)
{
	erreur("Exception NSGuidelineControl ctor for slot.", standardError, 0);
}
}

//
//  Constructor for Slot connection
//
NSGuidelineControl::NSGuidelineControl(NSContexte* pCtx, DTlink* pLink, string sIdent)
                   :NSRoot(pCtx)
{
try
{
	_sIdentity = sIdent ;
  _sFctName  = string("") ;
	_pSlot     = (DTslot *) 0 ;
  _pLink     = pLink ;
  _pView     = (NSDecisionTreeView *) 0 ;
	_pNSCtrl   = (TWindow *) 0 ;
  _bVisible  = true ;

  if (_pLink)
    _pLink->addControl(this) ;
}
catch (...)
{
	erreur("Exception NSGuidelineControl ctor.", standardError, 0);
}
}

////  Destructeur
//
NSGuidelineControl::~NSGuidelineControl()
{
  // Disconnect slot and interface element
  //
  if (_pNSCtrl)
  {
    switch (_iType)
    {
    	case isActionConcernCheckbox :
      	(static_cast<NSActionConcernCheckBox*>(_pNSCtrl))->setControl(0) ;
        break ;
      case isDecisionButton :
      	(static_cast<NSDecisionButton*>(_pNSCtrl))->setControl(0) ;
        break ;
      case isDecisionRadioButton :
      	(static_cast<NSDecisionRadioButton*>(_pNSCtrl))->setControl(0) ;
        break ;
      case isGuidelineStatic :
      	(static_cast<NSGuidelineStatic*>(_pNSCtrl))->setControl(0) ;
        break ;
      case isSlotGroupBox :
      	(static_cast<NSSlotGroupBox*>(_pNSCtrl))->setControl(0) ;
        break ;
      case isLinkGroupBox :
      	(static_cast<NSLinkGroupBox*>(_pNSCtrl))->setControl(0) ;
        break ;
    }
	}

	if (_pSlot)
  	_pSlot->removeControl(this) ;

  if (_pLink)
  	_pLink->removeControl(this) ;
}

//
// Can we close the mother dialog box ?
//
bool
NSGuidelineControl::canWeClose()
{
	if (NULL == _pNSCtrl)
		return true ;

	switch (_iType)
	{
  	case isActionConcernCheckbox :
    	return (static_cast<NSActionConcernCheckBox*>(_pNSCtrl))->canWeClose() ;
    case isDecisionButton :
      return (static_cast<NSDecisionButton*>(_pNSCtrl))->canWeClose() ;
    case isDecisionRadioButton :
      return (static_cast<NSDecisionRadioButton*>(_pNSCtrl))->canWeClose() ;
    case isGuidelineStatic :
      return (static_cast<NSGuidelineStatic*>(_pNSCtrl))->canWeClose() ;
    case isSlotGroupBox :
      return (static_cast<NSSlotGroupBox*>(_pNSCtrl))->canWeClose() ;
    case isLinkGroupBox :
      return (static_cast<NSLinkGroupBox*>(_pNSCtrl))->canWeClose() ;
	}

	return true ;
}

HWND
NSGuidelineControl::getHWND()
{
	if (NULL == _pNSCtrl)
  	return 0 ;

  switch (_iType)
  {
    case isActionConcernCheckbox :
    	return (static_cast<NSActionConcernCheckBox*>(_pNSCtrl))->HWindow ;
    case isDecisionButton :
      return (static_cast<NSDecisionButton*>(_pNSCtrl))->HWindow ;
    case isDecisionRadioButton :
      return (static_cast<NSDecisionRadioButton*>(_pNSCtrl))->HWindow ;
    case isGuidelineStatic :
      return (static_cast<NSGuidelineStatic*>(_pNSCtrl))->HWindow ;
    case isSlotGroupBox :
      return (static_cast<NSSlotGroupBox*>(_pNSCtrl))->HWindow ;
    case isLinkGroupBox :
      return (static_cast<NSLinkGroupBox*>(_pNSCtrl))->HWindow ;
  }

	return (HWND) 0 ;
}

//
// Constructeur copie
//
NSGuidelineControl::NSGuidelineControl(const NSGuidelineControl& src)
                   :NSRoot(src.pContexte)
{
try
{
	_pNSCtrl   = src._pNSCtrl ;
  _pView     = src._pView ;
	_pSlot	   = src._pSlot ;
  _pLink     = src._pLink ;
	_sIdentity = src._sIdentity ;
  _sFctName  = src._sFctName ;
	_iType     = src._iType ;
  _bVisible  = src._bVisible ;
}
catch (...)
{
	erreur("Exception NSGuidelineControl copy ctor.", standardError, 0) ;
}
}

//
// Set control's state
//
// Arguments:  activation : state to set
//
void
NSGuidelineControl::activeControle(ACTIVATIONSTATE iState)
{
	if (NULL == _pNSCtrl)
  	return ;

	switch (_iType)  {
  	case isActionConcernCheckbox :
    	(static_cast<NSActionConcernCheckBox*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
    case isDecisionButton :
      (static_cast<NSDecisionButton*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
    case isDecisionRadioButton :
      (static_cast<NSDecisionRadioButton*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
    case isGuidelineStatic :
      (static_cast<NSGuidelineStatic*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
    case isSlotGroupBox :
      (static_cast<NSSlotGroupBox*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
    case isLinkGroupBox :
      (static_cast<NSLinkGroupBox*>(_pNSCtrl))->
                                        activeControle(iState) ;
      break ;
  }
}

//// Surcharge de l'opérateur d'affectation
//
NSGuidelineControl&
NSGuidelineControl::operator = (const NSGuidelineControl& src)
{
	if (this == &src)
		return *this ;

	_pNSCtrl   = src._pNSCtrl ;
  _pView     = src._pView ;
	_pSlot	   = src._pSlot ;
  _pLink     = src._pLink ;
	_sIdentity = src._sIdentity ;
  _sFctName  = src._sFctName ;
	_iType     = src._iType ;
  _bVisible  = src._bVisible ;

	return *this ;
}

//
// Surcharge de l'opérateur ==
//
int
NSGuidelineControl::operator == (const NSGuidelineControl& o)
{
	if	((_pSlot == o._pSlot) && (_pNSCtrl == o._pNSCtrl))
		return 1 ;
	else
		return 0 ;
}

//
// Activer physiquement le controle
//
void
NSGuidelineControl::SetFocus()
{
	if (NULL == _pNSCtrl)
  	return ;

  switch (_iType)
  {
  	case isActionConcernCheckbox :
    	(static_cast<NSActionConcernCheckBox*>(_pNSCtrl))->setActivation(true) ;
      break ;
    case isDecisionButton :
      (static_cast<NSDecisionButton*>(_pNSCtrl))->setActivation(true) ;
      break ;
    case isDecisionRadioButton :
      (static_cast<NSDecisionRadioButton*>(_pNSCtrl))->setActivation(true) ;
      break ;
    case isGuidelineStatic :
      (static_cast<NSGuidelineStatic*>(_pNSCtrl))->setActivation(true) ;
      break ;
    case isSlotGroupBox :
      (static_cast<NSSlotGroupBox*>(_pNSCtrl))->setActivation(true) ;
      break ;
    case isLinkGroupBox :
      (static_cast<NSLinkGroupBox*>(_pNSCtrl))->setActivation(true) ;
      break ;
  }

  (static_cast<TControl*>(_pNSCtrl))->SetFocus() ;
}

// --------------------------------------------------------------------------// -------------------- Méthodes de NSControleVector ------------------------
// --------------------------------------------------------------------------

//
// Constructeur copie
//
NSGuidelineControlVector::NSGuidelineControlVector(const NSGuidelineControlVector& rv)
                         :NSGuidelineControlVectorCtrl()
{
try
{
	if (false == rv.empty())
		for (iterNSGuidelineControlConst i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSGuidelineControl((*i)->pContexte)) ;
}
catch (...)
{
	erreur("Exception NSGuidelineControlVector copy ctor.", standardError, 0) ;
}
}

//
// Destructeur
//
void
NSGuidelineControlVector::vider()
{
	if (empty())
  	return ;

	for (iterNSGuidelineControl i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
  }
}

void
NSGuidelineControlVector::emptyWithoutDelete()
{
	if (false == empty())
	  for (iterNSGuidelineControl i = begin() ; end() != i ; )
      erase(i) ;
}

void
NSGuidelineControlVector::eraseControl(NSGuidelineControl *pGuidelineControl)
{
  if (empty() || (NULL == pGuidelineControl))
    return ;

  for (iterNSGuidelineControl i = begin() ; end() != i ; )
  {
    if (*i == pGuidelineControl)
      erase(i) ;
    else
      i++ ;
  }
}

NSGuidelineControlVector::~NSGuidelineControlVector(){
	vider() ;
}

//// Opérateur d'affectation
//
NSGuidelineControlVector&
NSGuidelineControlVector::operator=(const NSGuidelineControlVector& src)
{
	if (this == &src)
  	return *this ;

try
{
	vider() ;

  if (false == src.empty())  	for (iterNSGuidelineControlConst i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSGuidelineControl((*i)->pContexte)) ;
  return *this ;
}
catch (...)
{
	erreur("Exception NSGuidelineControlVector (=).", standardError, 0) ;
	return *this ;
}
}

// -----------------------------------------------------------------------------
//                             NSDecisionButton
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSDecisionButton, TButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),END_RESPONSE_TABLE;

//
// Constructeur
//
NSDecisionButton::NSDecisionButton(NSContexte *pCtx, TWindow* parent, int resId)
                 :TButton(parent, resId), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;

  _sArchetypeId     = string("") ;
  _sQualityOfLifeId = string("") ;

	DisableTransfer() ;
}

NSDecisionButton::NSDecisionButton(NSContexte *pCtx, TWindow* parent, int resId,
                                   const char far* text, int X, int Y, int W,
                                   int H, bool isDefault, TModule* module)
                 :TButton(parent, resId, text, X, Y, W, H, isDefault, module), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;

  _sArchetypeId     = string("") ;
  _sQualityOfLifeId = string("") ;

	DisableTransfer() ;
}

//
// Destructeur
//
NSDecisionButton::~NSDecisionButton()
{
  if ((_pControl) && (_pControl->getControl()))
    _pControl->setControl((void *) 0) ;
}

void
NSDecisionButton::SetupWindow()
{
  OWL::TButton::SetupWindow() ;
}

void
NSDecisionButton::setControl(NSGuidelineControl* pControl)
{
	NSGuidelineInterfaceElement::setControl(pControl) ;

  _sArchetypeId     = string("") ;
  _sQualityOfLifeId = string("") ;

  if (NULL == pControl)
    return ;

  DTslot* pSlot = pControl->getSlot() ;
  if (NULL == pSlot)
    return ;

  CguidelineStep* pStep = pSlot->getStep() ;
  if (NULL == pStep)
    return ;

  CactionStep *pAction = dynamic_cast<CactionStep *>(pStep) ;
  if (NULL == pAction)
    return ;

  CactionArchetype* pArchetype = pAction->getArchetype() ;
  if (NULL == pArchetype)
    return ;

  _sArchetypeId     = pArchetype->getArchetypeId() ;
  _sQualityOfLifeId = pArchetype->getQualityOfLifeId() ;
}

//
// Fonction utilisée lorsque le bouton est cliqué
//
void
NSDecisionButton::BNClicked()
{
	if (false == _bActiveToi)
  {
    _bActiveToi = true ;
    return ;
  }

  if (string("") != _sArchetypeId)
  {
    NSToDoTask *pTask = new NSToDoTask() ;
    pTask->BbkAskUser(pContexte->getSuperviseur(), _sArchetypeId, NSCQDocument::standard) ;
  }

  if (string("") != _sQualityOfLifeId)
  {
    NSCoopChartDocument* pDocNewCoopChart = new NSCoopChartDocument(0, pContexte, _sQualityOfLifeId /*, pConcern->getNoeud()*/) ;
    NSDocViewManager dvManager(pContexte) ;
	  dvManager.createView(pDocNewCoopChart, "Coop Chart") ;
  }
}

boolNSDecisionButton::canWeClose()
{
  return true ;
}
void
NSDecisionButton::activeControle(ACTIVATIONSTATE iState)
{
}

char far*
NSDecisionButton::GetClassName()
{
  return "BUTTON" ;
}

// -----------------------------------------------------------------------------
//                             NSActionConcernCheckBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSActionConcernCheckBox, TCheckBox)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
  EV_MESSAGE(WM_CTLCOLORSTATIC, EvSetBackColor),
  EV_WM_KEYUP,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSActionConcernCheckBox::NSActionConcernCheckBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, TGroupBox* group, bool bStandardStyle, string sBackColor)
                        :TCheckBox(parent, resId, group), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;
	DisableTransfer() ;

  _bIsEmpty = true ;

  _bStandardStyle = bStandardStyle ;
  _sBackColor     = sBackColor ;

  _pDTView        = parent ;

  emptyConcernInformation() ;
}

NSActionConcernCheckBox::NSActionConcernCheckBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far* title,
                             int x, int y, int w, int h, TGroupBox *group, bool bStandardStyle, string sBackColor,
                             TModule* module)
                        :TCheckBox(parent, resId, title, x, y, w, h, group, module), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;
	DisableTransfer() ;
  //
  // Attention, le constructeur de TCheckBox employé ici attribue le style
  // BS_AUTOCHECKBOX, que nous remplaçons par BS_CHECKBOX
  //
  Attr.Style    = (Attr.Style ^ BS_AUTOCHECKBOX) | BS_CHECKBOX ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;

  _bIsEmpty       = true ;
  _bStandardStyle = bStandardStyle ;
  _sBackColor     = sBackColor ;

  _pDTView        = parent ;

  if ((string("") != sBackColor) || _bStandardStyle)
    Attr.Style = Attr.Style | BS_OWNERDRAW ;

  emptyConcernInformation() ;
}

//
// Destructeur
//
NSActionConcernCheckBox::~NSActionConcernCheckBox()
{
}

void
NSActionConcernCheckBox::SetupWindow()
{
	TCheckBox::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

void
NSActionConcernCheckBox::setControl(NSGuidelineControl* pControl)
{
	NSGuidelineInterfaceElement::setControl(pControl) ;

  emptyConcernInformation() ;

  if (NULL == pControl)
    return ;

  DTslot* pSlot = pControl->getSlot() ;
  if (NULL == pSlot)
    return ;

  CguidelineStep* pStep = pSlot->getStep() ;
  if (NULL == pStep)
    return ;

  CactionStep *pAction = dynamic_cast<CactionStep *>(pStep) ;
  if (NULL == pAction)
    return ;

  Cconcern* pConcern = pAction->getConcernToCreate() ;
  if (NULL == pConcern)
    return ;

  _sConcernCode     = pConcern->getCode() ;
  _sConcernFrame    = pConcern->getFrame() ;
  _iConcernSeverity = pConcern->getSeverity() ;
}

bool
NSActionConcernCheckBox::canWeClose()
{
	return true ;
}

void
NSActionConcernCheckBox::activeControle(ACTIVATIONSTATE iState)
{
  setState(iState) ;

  switch (iState)
  {
    case isGreen :
	    SetCheck(BF_CHECKED) ;
      break ;
    default :
      SetCheck(BF_UNCHECKED) ;
  }
}

//
// Fonction déclenchée lorsque la boite à cocher est activée.
//void
NSActionConcernCheckBox::BNClicked()
{
  _bIsEmpty = false ;

  if (false == _bActiveToi)
  {
    _bActiveToi = true ;
    return ;
  }

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;
  string sDate = tpsNow.donneDate() ;

  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("NewPreoccup")) ;
  pTask->setPointer1((void*) Parent, false) ;
	pTask->setParam1(_sConcernCode) ;
	pTask->setParam2(sDate) ;

	pTask->sendMe(pContexte->getSuperviseur()) ;

	TCheckBox::BNClicked() ;
}

void
NSActionConcernCheckBox::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  TCheckBox::EvKeyUp(key, repeatcount, flags) ;
}

//
// Return name of predefined BWCC or Windows check box class
//
char far*
NSActionConcernCheckBox::GetClassName()
{
  if ((false == _bStandardStyle) && (GetApplication()->BWCCEnabled()))
    return CHECK_CLASS ;

  return "BUTTON";
}

TResult
NSActionConcernCheckBox::EvSetBackColor(TParam1 param1, TParam2 param2)
{
  HBRUSH hbr = (HBRUSH)DefWindowProc(GetCurrentEvent().Message, param1, param2) ;
  ::DeleteObject(hbr) ;
  SetBkMode((HDC)param1, TRANSPARENT) ;
  return (TResult)::GetStockObject(NULL_BRUSH) ;
}

void
NSActionConcernCheckBox::emptyConcernInformation()
{
  _sConcernCode     = string("") ;
  _sConcernFrame    = string("") ;
  _iConcernSeverity = 0 ;
}

// -----------------------------------------------------------------------------
//                             NSDecisionRadioButton
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSDecisionRadioButton, TRadioButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
  EV_MESSAGE(WM_CTLCOLORSTATIC, EvSetBackColor),
  EV_WM_KEYUP,
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeurs
//
NSDecisionRadioButton::NSDecisionRadioButton(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, TGroupBox* group)
                      :TRadioButton(parent, resId, group), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;
	DisableTransfer() ;
  _bIsEmpty = true ;
  _pDTView  = parent ;
}

NSDecisionRadioButton::NSDecisionRadioButton(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far* title,
                             int x, int y, int w, int h, TGroupBox *group,
                             TModule* module)
                      :TRadioButton(parent, resId, title, x, y, w, h, group, module), NSGuidelineInterfaceElement(pCtx)
{
  _bActiveToi = true ;
	DisableTransfer() ;
  _pDTView    = parent ;
  //
  // Attention, le constructeur de TRadioButton employé ici attribue le style
  // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON
  //
  Attr.Style = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON ;
  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  _bIsEmpty = true ;

  Attr.Style = Attr.Style | BS_OWNERDRAW ;
}

char far*
NSDecisionRadioButton::GetClassName()
{
  return RADIO_CLASS ;
}

void
NSDecisionRadioButton::GetWindowClass(WNDCLASS far& wc)
{
  TWindow::GetWindowClass(wc) ;
  // static TBrush yellow(TColor::LtYellow);
  // wc.hbrBackground = yellow;
  // wc.lpszClassName = RADIO_CLASS;
  // wc.style |= BS_RADIOBUTTON;
}

void
NSDecisionRadioButton::SetupWindow()
{
  TRadioButton::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

bool
NSDecisionRadioButton::canWeClose()
{
  return true ;
}

//
// Destructeur
//
NSDecisionRadioButton::~NSDecisionRadioButton()
{
}

//
// Fonction déclenché lorsque la boite à cocher est activée.
//
void
NSDecisionRadioButton::BNClicked()
{
  _bIsEmpty = false ;

	if (false == _bActiveToi)
	{
  	_bActiveToi = true ;
    return ;
	}

	TRadioButton::BNClicked() ;
}

void
NSDecisionRadioButton::activeControle(ACTIVATIONSTATE iState)
{
  setState(iState) ;

  switch (iState)
  {
    case isGreen :
	    SetCheck(BF_CHECKED) ;
      break ;
    default :
      SetCheck(BF_UNCHECKED) ;
  }
}

void
NSDecisionRadioButton::EvKeyUp(uint key, uint repeatcount, uint flags)
{
  TRadioButton::EvKeyUp(key, repeatcount, flags) ;
}

TResult
NSDecisionRadioButton::EvSetBackColor(TParam1 param1, TParam2 param2)
{
  HBRUSH hbr = (HBRUSH)DefWindowProc(GetCurrentEvent().Message, param1, param2) ;
  ::DeleteObject(hbr) ;
  SetBkMode((HDC)param1, TRANSPARENT) ;
  return (TResult)::GetStockObject(NULL_BRUSH) ;
}

void
NSDecisionRadioButton::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

void
NSDecisionRadioButton::EvPaint()
{
  TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

//
// Paint the line into a memory DC and bitblt the final rendering to the
// specified 'dc'. Inspired by TGlyphButton::PaintButton(TDC& dc)
//
void
NSDecisionRadioButton::PaintGroup(TDC& dc)
{
  NS_CLASSLIB::TRect rect ;
  GetClientRect(rect) ;

  // Create compatible bitmap
  //
  OWL::TBitmap memBmp(dc, rect.Width(), rect.Height()) ;

  // Create compatible memory DC
  //
  TMemoryDC memDC(dc) ;

  // Select and init memory bitmap
  //
  memDC.SelectObject(memBmp) ;

  // Save the rectangle for bitblt'ing - 'rect' will be adjusted
  // as we move from defaultFrame, border and face...
  //
  NS_CLASSLIB::TRect bltRect = rect;

  // Paint the button into the memory DC
  //
  //PaintDefaultRect(memDC, rect);
  PaintFrame(memDC, rect);
  PaintFace(memDC, rect);

  // Bitblt the button to the output device
  //
  dc.BitBlt(bltRect, memDC, NS_CLASSLIB::TPoint(bltRect.left, bltRect.top));

  // Cleanup
  //
  memDC.RestoreBitmap();
}

void
NSDecisionRadioButton::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
}

void
NSDecisionRadioButton::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  TBrush brush(TColor::White) ;
  dc.FillRect(rect, brush) ;

  int iIconWidth = 0 ;

  TDib *pCheckedDib = _pDTView->getCheckedDib() ;
  if (pCheckedDib && (BF_CHECKED == GetCheck()))
  {
    iIconWidth = pCheckedDib->Width() ;

    NS_CLASSLIB::TPoint dibOrigin(0, 0) ;

    // Using BitBlt
    //
    HDC hdcMem = CreateCompatibleDC(dc.GetHDC()) ;

    HBITMAP bitmap = ::CreateDIBitmap(dc,
                                    (LPBITMAPINFOHEADER)pCheckedDib->GetInfoHeader(),
                                    CBM_INIT,
                                    (const uint8 far*)pCheckedDib->GetBits(),
                                    (LPBITMAPINFO)pCheckedDib->GetInfo(),
                                    pCheckedDib->Usage()) ;
    if (NULL == bitmap)
      DeleteDC(hdcMem) ;
    else
    {
      HGDIOBJ hbmOld = SelectObject(hdcMem, bitmap) ;

      dc.BitBlt(rect,
              hdcMem,
              rect.TopLeft()) ;
    }
  }

  NS_CLASSLIB::TRect textRect = rect ;
  if (iIconWidth > 0)
    textRect = NS_CLASSLIB::TRect(rect.Left() + iIconWidth, rect.Top(), rect.Right(), rect.Bottom()) ;

  Cdialogbox* pDialogBox = _pDTView->getDialogBox() ;
	if (pDialogBox)
    _pDTView->SetFont(dc, pDialogBox) ;

	dc.DrawText(Title, -1, textRect, DT_CENTER | DT_VCENTER) ;
}

// -----------------------------------------------------------------------------
//                             NSGuidelineStatic
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSGuidelineStatic, TStatic)
	EV_WM_SETFOCUS,
  EV_WM_PAINT,
END_RESPONSE_TABLE;

// Constructeur
NSGuidelineStatic::NSGuidelineStatic(NSContexte* pCtx, NSDecisionTreeView* parent, int resId)
                  :TStatic(parent, resId), NSGuidelineInterfaceElement(pCtx)
{
	DisableTransfer() ;

  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  Attr.Style = Attr.Style | BS_OWNERDRAW ;

  _pDTView = parent ;
}

NSGuidelineStatic::NSGuidelineStatic(NSContexte* pCtx, NSDecisionTreeView* parent, int resId, const char far* title,
                    int x, int y, int w, int h,
                    uint textLimit, TModule* module)
                  :TStatic(parent, resId, title, x, y, w, h, textLimit, module), NSGuidelineInterfaceElement(pCtx)
{
	DisableTransfer() ;

  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  Attr.Style = Attr.Style | BS_OWNERDRAW ;

  _pDTView = parent ;
}

// Destructeur
NSGuidelineStatic::~NSGuidelineStatic()
{
}

void
NSGuidelineStatic::SetupWindow()
{
	TStatic::SetupWindow() ;

  NSSuper*  pSuper  = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

void
NSGuidelineStatic::EvSetFocus(HWND /* hWndLostFocus */)
{
	// si on gagne le focus, on s'en débarrasse aussitôt
	//if (_pControl && _pControl->getNSDialog())
	//	pControle->getNSDialog()->ActiveControlSuivant(pControle) ;
}

bool
NSGuidelineStatic::canWeClose()
{
	return true ;
}

void
NSGuidelineStatic::activeControle(ACTIVATIONSTATE iState)
{
}

void
NSGuidelineStatic::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

void
NSGuidelineStatic::EvPaint()
{
  TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

//
// Paint the line into a memory DC and bitblt the final rendering to the
// specified 'dc'. Inspired by TGlyphButton::PaintButton(TDC& dc)
//
void
NSGuidelineStatic::PaintGroup(TDC& dc)
{
  NS_CLASSLIB::TRect rect ;
  GetClientRect(rect) ;

  // Create compatible bitmap
  //
  OWL::TBitmap memBmp(dc, rect.Width(), rect.Height()) ;

  // Create compatible memory DC
  //
  TMemoryDC memDC(dc) ;

  // Select and init memory bitmap
  //
  memDC.SelectObject(memBmp) ;

  // Save the rectangle for bitblt'ing - 'rect' will be adjusted
  // as we move from defaultFrame, border and face...
  //
  NS_CLASSLIB::TRect bltRect = rect;

  // Paint the button into the memory DC
  //
  //PaintDefaultRect(memDC, rect);
  PaintFrame(memDC, rect);
  PaintFace(memDC, rect);

  // Bitblt the button to the output device
  //
  dc.BitBlt(bltRect, memDC, NS_CLASSLIB::TPoint(bltRect.left, bltRect.top));

  // Cleanup
  //
  memDC.RestoreBitmap();
}

void
NSGuidelineStatic::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
}

void
NSGuidelineStatic::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  TBrush brush(TColor::White) ;
  dc.FillRect(rect, brush) ;

  Cdialogbox* pDialogBox = _pDTView->getDialogBox() ;
	if (pDialogBox)
    _pDTView->SetFont(dc, pDialogBox) ;

	dc.DrawText(Title, -1, rect, DT_CENTER | DT_VCENTER) ;
}

// -----------------------------------------------------------------------------
//                             NSSlotGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSSlotGroupBox, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSSlotGroupBox::NSSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId)
               :TGroupBox((TWindow*) parent, resId), NSGuidelineInterfaceElement(pCtx)
{
	DisableTransfer() ;

  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
  Attr.Style = Attr.Style | BS_OWNERDRAW ;

  _pDTView = parent ;
}

NSSlotGroupBox::NSSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)               :TGroupBox((TWindow*) parent, resId, text, x, y, w, h, module), NSGuidelineInterfaceElement(pCtx){	DisableTransfer() ;  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;  Attr.Style = Attr.Style | BS_OWNERDRAW ;  _pDTView = parent ;}
//// Destructeur
//
NSSlotGroupBox::~NSSlotGroupBox()
{
}

// SetupWindow : on fixe la police du dialogue
void
NSSlotGroupBox::SetupWindow()
{
	TGroupBox::SetupWindow() ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;
  SetWindowFont(*(pSuper->getDialogFont()), false) ;
}

//
// Fonction déclenché lorsque la boite à cocher est activée.
//
void
NSSlotGroupBox::SelectionChanged(int /* controlId */)
{
	//
	// Prévient la boite de dialogue
	//
	#if defined(__WIN32__)
		Parent->HandleMessage(WM_CORRECT, (MAKEWPARAM(Attr.Id, WM_CORRECT)),
									 LPARAM(HWindow));
	#else
		Parent->HandleMessage(WM_CORRECT, Attr.Id, MAKELPARAM(HWindow, WM_CORRECT));
	#endif
	//TGroupBox::EvKillFocus(hWndGetFocus);
}

bool
NSSlotGroupBox::canWeClose()
{
  return true ;
}

void
NSSlotGroupBox::activeControle(ACTIVATIONSTATE iState)
{
  _iActivationState = iState ;
}

void
NSSlotGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSSlotGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Paint the line into a memory DC and bitblt the final rendering to the
// specified 'dc'. Inspired by TGlyphButton::PaintButton(TDC& dc)
//
void
NSSlotGroupBox::PaintGroup(TDC& dc)
{
  NS_CLASSLIB::TRect rect ;
  GetClientRect(rect) ;

  // Create compatible bitmap
  //
  OWL::TBitmap memBmp(dc, rect.Width(), rect.Height()) ;

  // Create compatible memory DC
  //
  TMemoryDC memDC(dc) ;

  // Select and init memory bitmap
  //
  memDC.SelectObject(memBmp) ;

  // Save the rectangle for bitblt'ing - 'rect' will be adjusted
  // as we move from defaultFrame, border and face...
  //
  NS_CLASSLIB::TRect bltRect = rect;

  // Paint the button into the memory DC
  //
  //PaintDefaultRect(memDC, rect);
  PaintBackground(memDC, rect);
  PaintFrame(memDC, rect);
  PaintFace(memDC, rect);

  // Bitblt the button to the output device
  //
  dc.BitBlt(bltRect, memDC, NS_CLASSLIB::TPoint(bltRect.left, bltRect.top));

  // Cleanup
  //
  memDC.RestoreBitmap();
}

//
// Draw the background
//
void
NSSlotGroupBox::PaintBackground(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Fill the background with windows' background color
  //
  TBrush brush(_pDTView->getBackgroundColor()) ;
  dc.FillRect(rect, brush) ;
}


//
// Draw the border
//
void
NSSlotGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Paint the border
  //
  TUIBorder uiBorder(rect, /* IsSet(biPushed) ? TUIBorder::ButtonDn :*/
                                             TUIBorder::ButtonUp);
  uiBorder.Paint(dc);

  // Shrink the rectangle to leave the face
  //
  rect = uiBorder.GetClientRect();
}

//
// Draw a rounded border
//
void
NSSlotGroupBox::PaintRoundFrame(TDC& dc, NS_CLASSLIB::TRect& rect, int iAngleRadius, int iPenWidth)
{
  NS_CLASSLIB::TPoint p1 = rect.TopLeft().OffsetBy(iAngleRadius, iPenWidth-1) ;

  NS_CLASSLIB::TPoint p2 = rect.TopRight().OffsetBy(-iAngleRadius, iPenWidth-1) ;
  NS_CLASSLIB::TPoint p3 = rect.TopRight().OffsetBy(-iPenWidth, iAngleRadius) ;
  NS_CLASSLIB::TPoint c23 = rect.TopRight().OffsetBy(-iAngleRadius-iPenWidth, iAngleRadius+iPenWidth-1) ;

  NS_CLASSLIB::TPoint p4 = rect.BottomRight().OffsetBy(-iPenWidth, -iAngleRadius-iPenWidth+1) ;
  NS_CLASSLIB::TPoint p5 = rect.BottomRight().OffsetBy(-iAngleRadius, 0) ;
  NS_CLASSLIB::TPoint c45 = rect.BottomRight().OffsetBy(-iAngleRadius-iPenWidth+1, -iAngleRadius-iPenWidth) ;

  NS_CLASSLIB::TPoint p6 = rect.BottomLeft().OffsetBy(iAngleRadius+iPenWidth-1, -iPenWidth) ;
  NS_CLASSLIB::TPoint p7 = rect.BottomLeft().OffsetBy(iPenWidth-1, -iAngleRadius-iPenWidth) ;
  NS_CLASSLIB::TPoint c67 = rect.BottomLeft().OffsetBy(iAngleRadius+iPenWidth-1, -iAngleRadius-iPenWidth) ;

  NS_CLASSLIB::TPoint p8 = rect.TopLeft().OffsetBy(iPenWidth-1, iAngleRadius+iPenWidth) ;
  NS_CLASSLIB::TPoint c81 = rect.TopLeft().OffsetBy(iAngleRadius+iPenWidth-1, iAngleRadius+iPenWidth-1) ;

  dc.MoveTo(p1) ;
  dc.LineTo(p2) ;
  dc.AngleArc(c23, iAngleRadius, 90, -90) ;
  dc.LineTo(p4) ;
  dc.AngleArc(c45, iAngleRadius, 0, -90) ;
  dc.LineTo(p6) ;
  dc.AngleArc(c67, iAngleRadius, 270, -90) ;
  dc.LineTo(p8) ;
  dc.AngleArc(c81, iAngleRadius, 180, -90) ;
}

//
// Draw the face
//
void
NSSlotGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Fill the background with the face color
  //
  switch(_iActivationState)
  {
    case (isBlank) :
    {
      TBrush brush(TColor::White);
      dc.FillRect(rect, brush);
      break ;
    }
    case (isGreen) :
    {
      TBrush brush(TColor::LtGreen) ;
      dc.FillRect(rect, brush) ;
      break ;
    }
    case (isRed) :
    {
      TBrush brush(TColor::LtRed);
      dc.FillRect(rect, brush);
      break ;
    }
  }
}

// -----------------------------------------------------------------------------
//                             NSActionSlotGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSActionSlotGroupBox, NSSlotGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSActionSlotGroupBox::NSActionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId)
                     :NSSlotGroupBox(pCtx, parent, resId)
{
}

NSActionSlotGroupBox::NSActionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)                     :NSSlotGroupBox(pCtx, parent, resId, text, x, y, w, h, module){}
//// Destructeur
//
NSActionSlotGroupBox::~NSActionSlotGroupBox()
{
}

// SetupWindow : on fixe la police du dialogue
void
NSActionSlotGroupBox::SetupWindow()
{
	NSSlotGroupBox::SetupWindow() ;
}

void
NSActionSlotGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSActionSlotGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Draw the border
//
void
NSActionSlotGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Paint the border
  //
  TUIBorder uiBorder(rect, /* IsSet(biPushed) ? TUIBorder::ButtonDn :*/
                                             TUIBorder::ButtonUp);
  uiBorder.Paint(dc);

  // Shrink the rectangle to leave the face
  //
  rect = uiBorder.GetClientRect();
}

//
// Draw the face
//
void
NSActionSlotGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Fill the background with the face color
  //
  switch(_iActivationState)
  {
    case (isBlank) :
    {
      TBrush brush(TColor::White);
      dc.FillRect(rect, brush);
      break ;
    }
    case (isGreen) :
    {
      TBrush brush(TColor::LtGreen) ;
      dc.FillRect(rect, brush) ;
      break ;
    }
    case (isRed) :
    {
      TBrush brush(TColor::LtRed);
      dc.FillRect(rect, brush);
      break ;
    }
  }
}

// -----------------------------------------------------------------------------
//                             NSDecisionSlotGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSDecisionSlotGroupBox, NSSlotGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSDecisionSlotGroupBox::NSDecisionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId)
                       :NSSlotGroupBox(pCtx, parent, resId)
{
}

NSDecisionSlotGroupBox::NSDecisionSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)                       :NSSlotGroupBox(pCtx, parent, resId, text, x, y, w, h, module){}
//// Destructeur
//
NSDecisionSlotGroupBox::~NSDecisionSlotGroupBox()
{
}

// SetupWindow : on fixe la police du dialogue
void
NSDecisionSlotGroupBox::SetupWindow()
{
	NSSlotGroupBox::SetupWindow() ;
}

void
NSDecisionSlotGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSDecisionSlotGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Draw the border
//
void
NSDecisionSlotGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Paint the border
  //
  TUIBorder uiBorder(rect, /* IsSet(biPushed) ? TUIBorder::ButtonDn :*/
                                             TUIBorder::ButtonUp);
  uiBorder.Paint(dc);

  // Shrink the rectangle to leave the face
  //
  rect = uiBorder.GetClientRect();
}

//
// Draw the face
//
void
NSDecisionSlotGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Fill the background in white
  //
  TBrush brush(TColor::White);
  dc.FillRect(rect, brush);
}

// -----------------------------------------------------------------------------
//                             NSDecisionStepSlotGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSDecisionStepSlotGroupBox, NSSlotGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSDecisionStepSlotGroupBox::NSDecisionStepSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId)
                           :NSSlotGroupBox(pCtx, parent, resId)
{
}

NSDecisionStepSlotGroupBox::NSDecisionStepSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)                           :NSSlotGroupBox(pCtx, parent, resId, text, x, y, w, h, module){}
//// Destructeur
//
NSDecisionStepSlotGroupBox::~NSDecisionStepSlotGroupBox()
{
}

// SetupWindow : on fixe la police du dialogue
void
NSDecisionStepSlotGroupBox::SetupWindow()
{
	NSSlotGroupBox::SetupWindow() ;
}

void
NSDecisionStepSlotGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSDecisionStepSlotGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Draw the border
//
void
NSDecisionStepSlotGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Backup current objects (pen and brush)
	HPEN oldPen     = (HPEN) dc.GetCurrentObject(OBJ_PEN) ;
	HBRUSH oldBrush = (HBRUSH) dc.GetCurrentObject(OBJ_BRUSH) ;

  int iAngleRadius = 10 ;
  int iPenWidth    = 1 ;

    // Fill the background with the face color
  //
  switch(_iActivationState)
  {
    case (isBlank) :
    {
      iPenWidth = 1 ;
      TPen pen(TColor::Black, /* int width */ iPenWidth /*, int style=PS_SOLID*/) ;
      dc.SelectObject(pen) ;
      break ;
    }
    case (isGreen) :
    {
      iPenWidth = 2 ;
      TPen pen(TColor::LtGreen, /* int width */ iPenWidth /*, int style=PS_SOLID*/) ;
      dc.SelectObject(pen) ;
      break ;
    }
    case (isRed) :
    {
      iPenWidth = 1 ;
      TPen pen(TColor::LtRed, /* int width */ iPenWidth /*, int style=PS_SOLID*/) ;
      dc.SelectObject(pen) ;
      break ;
    }
  }

  PaintRoundFrame(dc, rect, iAngleRadius, iPenWidth) ;

  // dc.Arc(r45, p6, p5) ;

	// Restore initial objects (pen and brush)
  //
	::SelectObject(dc.GetHDC(), oldPen) ;
	::SelectObject(dc.GetHDC(), oldBrush) ;
}

//
// Draw the face
//
void
NSDecisionStepSlotGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  return ;
}

// -----------------------------------------------------------------------------
//                             NSDecisionStepSlotGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSSynchroSlotGroupBox, NSSlotGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSSynchroSlotGroupBox::NSSynchroSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId)
                      :NSSlotGroupBox(pCtx, parent, resId)
{
}

NSSynchroSlotGroupBox::NSSynchroSlotGroupBox(NSContexte *pCtx, NSDecisionTreeView* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)                      :NSSlotGroupBox(pCtx, parent, resId, text, x, y, w, h, module){}
//// Destructeur
//
NSSynchroSlotGroupBox::~NSSynchroSlotGroupBox()
{
}

// SetupWindow : on fixe la police du dialogue
void
NSSynchroSlotGroupBox::SetupWindow()
{
	NSSlotGroupBox::SetupWindow() ;
}

void
NSSynchroSlotGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSSynchroSlotGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Draw the border
//
void
NSSynchroSlotGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Backup current objects (pen and brush)
	// HPEN oldPen     = (HPEN) dc.GetCurrentObject(OBJ_PEN) ;
	// HBRUSH oldBrush = (HBRUSH) dc.GetCurrentObject(OBJ_BRUSH) ;

  int iAngleRadius = 5 ;
  int iPenWidth    = 1 ;

  TPen pen(TColor::Black, /* int width */ iPenWidth /*, int style=PS_SOLID*/) ;
  dc.SelectObject(pen) ;

  PaintRoundFrame(dc, rect, iAngleRadius, iPenWidth) ;

  // dc.Arc(r45, p6, p5) ;

	// Restore initial objects (pen and brush)
  //
	// ::SelectObject(dc.GetHDC(), oldPen) ;
	// ::SelectObject(dc.GetHDC(), oldBrush) ;
}

//
// Draw the face
//
void
NSSynchroSlotGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  return ;
}


// -----------------------------------------------------------------------------
//                             NSLinkGroupBox
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLinkGroupBox, TGroupBox)
  EV_WM_PAINT,
END_RESPONSE_TABLE;

//
// Constructeur
//
NSLinkGroupBox::NSLinkGroupBox(NSContexte *pCtx, TWindow* parent, int resId)
               :TGroupBox(parent, resId), NSGuidelineInterfaceElement(pCtx)
{
	DisableTransfer() ;
  _bLinkActivated = false ;

  Attr.Style = Attr.Style | BS_OWNERDRAW ;
}

NSLinkGroupBox::NSLinkGroupBox(NSContexte *pCtx, TWindow* parent, int resId, const char far *text, int x, int y, int w, int h, TModule* module)               :TGroupBox(parent, resId, text, x, y, w, h, module), NSGuidelineInterfaceElement(pCtx){	DisableTransfer() ;  _bLinkActivated = false ;  Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;  Attr.Style = Attr.Style | BS_OWNERDRAW ;}
//// Destructeur
//
NSLinkGroupBox::~NSLinkGroupBox()
{
}

void
NSLinkGroupBox::SetupWindow()
{
	TGroupBox::SetupWindow() ;
}

char far*
NSLinkGroupBox::GetClassName()
{
  return "BUTTON" ;
}

void
NSLinkGroupBox::EvPaint(){
	TPaintDC dc(*this) ;
  NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&dc.Ps.rcPaint ;
  Paint(dc, dc.Ps.fErase, rect) ;
}

void
NSLinkGroupBox::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  PaintGroup(dc) ;
}

//
// Paint the line into a memory DC and bitblt the final rendering to the
// specified 'dc'. Inspired by TGlyphButton::PaintButton(TDC& dc)
//
void
NSLinkGroupBox::PaintGroup(TDC& dc)
{
  NS_CLASSLIB::TRect rect ;
  GetClientRect(rect) ;

  // Create compatible bitmap
  //
  OWL::TBitmap memBmp(dc, rect.Width(), rect.Height()) ;

  // Create compatible memory DC
  //
  TMemoryDC memDC(dc) ;

  // Select and init memory bitmap
  //
  memDC.SelectObject(memBmp) ;

  // Save the rectangle for bitblt'ing - 'rect' will be adjusted
  // as we move from defaultFrame, border and face...
  //
  NS_CLASSLIB::TRect bltRect = rect;

  // Paint the button into the memory DC
  //
  //PaintDefaultRect(memDC, rect);
  PaintFrame(memDC, rect);
  PaintFace(memDC, rect);

  // Bitblt the button to the output device
  //
  dc.BitBlt(bltRect, memDC, NS_CLASSLIB::TPoint(bltRect.left, bltRect.top));

  // Cleanup
  //
  memDC.RestoreBitmap();
}

//
// Draw the border
//
void
NSLinkGroupBox::PaintFrame(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Paint the border
  //
  TUIBorder uiBorder(rect, /* IsSet(biPushed) ? TUIBorder::ButtonDn :*/
                                             TUIBorder::ButtonUp);
  uiBorder.Paint(dc);

  // Shrink the rectangle to leave the face
  //
  rect = uiBorder.GetClientRect();
}

//
// Draw the face
//
void
NSLinkGroupBox::PaintFace(TDC& dc, NS_CLASSLIB::TRect& rect)
{
  // Fill the background with the face color
  //
  switch(_iActivationState)
  {
    case (isBlank) :
    {
      TBrush brush(TColor::White);
      dc.FillRect(rect, brush);
      break ;
    }
    case (isGreen) :
    {
      TBrush brush(TColor::LtGreen) ;
      dc.FillRect(rect, brush) ;
      break ;
    }
    case (isRed) :
    {
      TBrush brush(TColor::LtRed);
      dc.FillRect(rect, brush);
      break ;
    }
  }
}

bool
NSLinkGroupBox::canWeClose()
{
  return true ;
}

void
NSLinkGroupBox::activeControle(ACTIVATIONSTATE iState)
{
  setState(iState) ;
}

// -----------------------------------------------------------------------------
// end decisionTreeDV.cpp
// -----------------------------------------------------------------------------

