// -----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
// -----------------------------------------------------------------------------

#include <vcl.h>
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <classlib\date.h>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nssuper.h"
#include "dcodeur\nsdkd.h"

#include "nsldv\nsldvuti.h"
// #include "nautilus\nautilus.rh"
// #include "nautilus\nsadmiwd.h"
// #include "nautilus\nscqvue.h"
// #include "nautilus\decisionTreeDV.h"
#include "nsbb\nstlibre.h"

#include "nsepisod\nsepidiv.h"

// #include "nautilus\nshistdo.h"
// #include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"

#include <windows.h>
#include <owl\scrollba.h>

#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsconver.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"
#include "dcodeur\nsgen.h"
#include "nautilus\nsldvvue.rh"
#include "nsldv\nsldvvue_base.h"
// #include "nautilus\nsldvvuetech.h"
#include "nsldv\nsldvdrug.h"
#include "nsldv\nsldvvuetoons.h"
// #include "nautilus\nsFollowUpView.h"
#include "nsepisod\nsToDo.h"
// #include "ns_ob1\BB1KS.h"

#include "nsldv\nsldvgoal.h"
// #include "nautilus\nsCoopChartDV.h"

#include "nsldv\nsldvstructs.h"
#include "nsepisod\objectif_viewer.h"

long NSLdvViewBase::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// -------------------------- METHODES DE NSLdvViewBase ----------------------------
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvViewBase, TWindowView)
	EV_WM_VSCROLL,																			// scroll vertival
	EV_WM_HSCROLL,                                      // scroll horizontal
	EV_MESSAGE(WM_MOUSEWHEEL, EvMouseWheel),	          // mouse wheel
	EV_WM_MOUSEMOVE,                                    // mouvement de la souris
	EV_WM_LBUTTONDOWN,                                  // bouton gauche enfoncé
	EV_WM_RBUTTONDOWN,                                  // bouton droit enfoncé
	EV_WM_LBUTTONUP,                                    // bouton gauche relaché
	EV_WM_LBUTTONDBLCLK,                                // dbl-clicke avec bouton gauche
	EV_WM_TIMER,                                        // timer
	EV_WM_HOTKEY,                                       // raccourci (hotkey)
  EV_WM_KEYDOWN,
  EV_WM_SYSKEYDOWN,
	EV_WM_CLOSE,                                        // fermeture de la fenêtre
	EV_WM_DESTROY,                                      // destruction de la fenêtre
	EV_WM_SIZE,                                         // redimensionnement de la fenêtre
	EV_WM_SETFOCUS,                                     // focus
	EV_WM_QUERYENDSESSION,
	EV_COMMAND(IDC_NEWPREOCCUP,     CmNewPreoccup),      // nouvelle préoccup.
  EV_COMMAND(IDC_NEWRISK,         CmNewRisk),          // new risk
  EV_COMMAND(IDC_NEWSOCIAL,       CmNewSocialConcern),
  EV_COMMAND(CM_TIPTIMER_ON,      setTipsTimer),
	EV_COMMAND(CM_TIPTIMER_OFF,     killTipsTimer),
	// EV_COMMAND(IDC_CHG_PREO_TYP,    CmChgPreoType),     // correction type
	// EV_COMMAND(IDC_CHG_PREO_DAT,    CmChgPreoDate),     // correction date
	// EV_COMMAND(IDC_CHG_PREO_IDX,    CmChgPreoIndx),     // correction sévérite
	// EV_COMMAND(IDC_EVOL_PREO_TYP,   CmEvolPreoType),    // évolution type
	// EV_COMMAND(IDC_EVOL_PREO_IDX,   CmEvolPreoIndx),    // évolution sévérite
	// EV_COMMAND(IDC_SUPPRESS_PREO,   CmSuppressPreo),    // suppression préoccup
  EV_COMMAND(CM_DRUG_NEW,         CmDrugNew),
  // EV_COMMAND(CM_DRUG_RENEW,       CmDrugRenew),
  // EV_COMMAND(CM_DRUG_CHANGE,      CmDrugModify),
  // EV_COMMAND(CM_DRUG_MODIF_POSO,  CmDrugChangePoso),
  // EV_COMMAND(CM_DRUG_STOP,        CmDrugStop),
  // EV_COMMAND(CM_DRUG_DELETE,      CmDrugDelete),
  // EV_COMMAND(IDC_ORDONNANCE,      CmDrugPrescription),
	// EV_COMMAND(IDC_ADD_PREO_OBJ,    CmAddObjectifs),     // ajouter un objectif
	// EV_COMMAND(IDC_MANAGE_RIGHTS,   CmManageRights),    // gestion des droits
	// EV_COMMAND(IDC_EVOL_PREOCCUP,   CmEvolPreoccup),    // command CmNewPreoccup
	EV_COMMAND(IDI_LDVCURS16,       CmChange2Cursor),   // command CmChange2Cursor
	EV_COMMAND(IDI_LDVZOOM16,       CmChange2Zoom),     // command CmChange2Zoom
	EV_COMMAND(CM_LDV_ZOOMIN,       CmLdvZoomIn),       // command CmLdvZoomIn
	EV_COMMAND(CM_LDV_ZOOMOUT,      CmLdvZoomOut),      // command CmLdvZoomOut
	EV_COMMAND(CM_LDV_PIXSECOND,    CmLdvPixSecond),    // pixel == 1 seconde
	EV_COMMAND(CM_LDV_PIXMINUTE,    CmLdvPixMinute),    // pixel == 1 minute
	EV_COMMAND(CM_LDV_PIXHOUR,      CmLdvPixHour),      // pixel == 1 heure
	EV_COMMAND(CM_LDV_PIXDAY,       CmLdvPixDay),       // pixel == 1 jour
	EV_COMMAND(CM_LDV_PIXWEEK,      CmLdvPixWeek),      // pixel == 1 semaine
	EV_COMMAND(CM_LDV_PIXMONTH,     CmLdvPixMonth),     // pixel == 1 mois
	EV_COMMAND_ENABLE(IDI_LDVCURS16,  CmCursorEnable),  // passage en mode Curseur
	EV_COMMAND_ENABLE(IDI_LDVZOOM16,  CmZoomEnable),    // passage en mode Zoom
  EV_COMMAND(CM_APPOINTMENT,      setAppointment),
  EV_COMMAND(CM_HELP,             CmHelp),
  EV_CHILD_NOTIFY_ALL_CODES(ID_PIXEL_PU, UpdatePixelPerUnitRate),
  EV_CHILD_NOTIFY_ALL_CODES(ID_SEVERITY, UpdateSeverityThreshold),
END_RESPONSE_TABLE;

// -----------------------------------------------------------------------------
//  Constructeur
//
//  doc 	: NSLdvDocument à afficher
// -----------------------------------------------------------------------------

NSLdvViewBase::NSLdvViewBase(NSLdvDocument& doc, TWindow* parent)
	            :TWindowView(doc, parent),
               NSRoot(doc.pContexte)
{
try
{
  lObjectCount++ ;

  _bPainted = false ;

	_pLdVDoc  = &doc ;

  _sConcern = string("") ;

	// Initialisation des variables de l'objet
	// Initializing object datas

	Attr.Style = Attr.Style | WS_HSCROLL ;
	// Attr.AccelTable = LDV_ACCEL ;

	iZoomFactor = 1;
	skinName = string("ldvView");

  aZoomLevels.push_back(new NSTimeZoomLevel(pixSeconde, 0, 0, 60)) ;
  aZoomLevels.push_back(new NSTimeZoomLevel(pixMinute,  0, 0, 60)) ;
  aZoomLevels.push_back(new NSTimeZoomLevel(pixHeure,   0, 0, 24)) ;
  aZoomLevels.push_back(new NSTimeZoomLevel(pixJour,    0, 0,  7)) ;
  aZoomLevels.push_back(new NSTimeZoomLevel(pixSemaine, 0, 0,  4)) ;
  aZoomLevels.push_back(new NSTimeZoomLevel(pixMois,    0, 0, 12)) ;

	_bSystemColors = true ;
	_bDisplayDrugs = true ;
	_bDisplayGoals = false ;

  _iMinSeverityOfDisplayedConcerns = -1 ;

	// définition de la date du coin inférieur droit
	// on se met à minuit (au soir) du jour courant (ou 0h00 du jour d'après)
	_tDateHeureInfDroit.takeTime() ;
	_tDateHeureInfDroit.placeHeures(0) ;
	_tDateHeureInfDroit.placeMinutes(0) ;
	_tDateHeureInfDroit.placeSecondes(0) ;

  _iNbTimerTicks = 0 ;

	// initialisation de l'échelle de zoom et du mode de curseur
	iXunit = pixHeure;

  string sOpenModeLdV = pContexte->getEpisodus()->sOpenModeLdV ;
	if (string("") != sOpenModeLdV)
	{
		char cZoom = pseumaj(sOpenModeLdV[0]) ;
		switch (cZoom)
		{
    	case 'E' : iXunit = pixSeconde ; break ;
			case 'I' : iXunit = pixMinute ;  break ;
			case 'H' : iXunit = pixHeure ;   break ;
			case 'J' : iXunit = pixJour ;    break ;
			case 'S' : iXunit = pixSemaine ; break ;
			case 'M' : iXunit = pixMois ;    break ;
		}
	}

  pCurrentTimeInfo = getTimeZoomInfo(iXunit) ;

	iMouseMode = modeCursor ;

	// initialisation de la barre de temps
	// à faire absolument avant l'initialisation des autres lignes
	// modification du calcul du placement en y
	// initChronoLine() ;

	// création de la liste d'image pour le drag and drop
	ImgDragDrop = new OWL::TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR16, 0, 0) ;
	ImgDragDrop->SetBkColor(NS_CLASSLIB::TColor::White) ;
	ImgDragDrop->Add(OWL::TIcon(*GetApplication(), (TResId)IDI_LDVSSOBJ16)) ;
	ImgDragDrop->Add(OWL::TIcon(*GetApplication(), (TResId)IDI_LDVOBJET16)) ;

	// Création et stockage de tous les toons
	// Creation and storage of each and every toon

  // définition de la zone où est affiché en temps la position de la souris
	setDateTimeArea(0, 0, 500, 48) ;

  // Command pannel
  /* NSLdvCommandPannel* pPanel = */ new NSLdvCommandPannel(pContexte, 0, this) ;
	// aToons.push_back(pPanel) ;

  // Curves command pannel
  /* NSLdvCurvesManagementPannel* pCurvesPanel = */ new NSLdvCurvesManagementPannel(pContexte, 0, this) ;
	// aToons.push_back(pCurvesPanel) ;

  // Chrono Line
  /* NSLdvChronoLine* pChronoLine = */ new NSLdvChronoLine(pContexte, 0, this) ;
  // iLargeurChronoLine = pChronoLine->wVisibleBox.Height() ;
	// aToons.push_back(pChronoLine) ;

  // Vertical scrollbar
  /* NSLdvVerticalScrollBar* pScrollBar = */ // new NSLdvVerticalScrollBar(pContexte, 0, this) ;
	// aToons.push_back(pScrollBar) ;

  // new NSCurvesProperty(pContexte, 0, this) ;

	// Préoccupations et dérivés - health issues and components
	// aToons.init(doc.getPreoccupations(), pContexte, this);

  createWorkingArea(string("")) ;

	pLdvTankView = new NSLdvTankView(this, pContexte) ;
	pLdvTankView->setCoords(0, 50, 400, 300) ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Constructeur copie
// NSLdvViewBase::NSLdvViewBase(NSLdvViewBase &)
// -----------------------------------------------------------------------------

NSLdvViewBase::NSLdvViewBase(const NSLdvViewBase& src)
              :TWindowView(src.GetDocument(), (TWindow*)src.GetParent()),
               NSRoot(src.pContexte)
{
  lObjectCount++ ;

  _bPainted = false ;

  _pLdVDoc                 = src._pLdVDoc ;
  _sConcern                = src._sConcern ;
	iXunit                   = src.iXunit ;
	iZoomFactor              = src.iZoomFactor ;
	iLargeurChronoLine       = src.iLargeurChronoLine ;
	_bSystemColors           = src._bSystemColors ;
	_bDisplayDrugs				   = src._bDisplayDrugs ;
	_bDisplayGoals 				   = src._bDisplayGoals ;
  _iMinSeverityOfDisplayedConcerns = src._iMinSeverityOfDisplayedConcerns ;

	skinName                 = src.skinName ;
	_tDateHeureInfDroit      = src._tDateHeureInfDroit ;
	ImgDragDrop              = src.ImgDragDrop ;
	_aToons                  = src._aToons ;
  hAccelerator             = 0 ;
}

/**
 *  Initialization function the window opens
 */
void
NSLdvViewBase::SetupWindow()
{
try
{
  string sTraceText = string("NSLdvViewBase entering SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	SetWindowPosLdv() ;
	// SetWindowPosit() ;

	TWindowView::SetupWindow() ;

  EvaluateWinToonsPos(this, &_aToons) ;

	skinSwitchOn(skinName) ;

	pPinceauFond        = new ldvBrush(NS_CLASSLIB::TColor::LtGray) ;
	pPinceauFondSys     = new ldvBrush(NS_CLASSLIB::TColor::Sys3dFace) ;

	pPinceauBlack       = new ldvBrush(NS_CLASSLIB::TColor::Black) ;
	pPinceauRed         = new ldvBrush(NS_CLASSLIB::TColor::LtRed) ;
	pPinceauBleu        = new ldvBrush(NS_CLASSLIB::TColor::LtBlue) ;
	pPinceauS1          = new ldvBrush(NS_CLASSLIB::TColor::LtCyan) ;
	pPinceauS2          = new ldvBrush(NS_CLASSLIB::TColor::LtGreen) ;
	pPinceauS3          = new ldvBrush(NS_CLASSLIB::TColor::LtYellow) ;
  pPinceauS34         = new ldvBrush(NS_CLASSLIB::TColor(255, 204, 0)) ;
	pPinceauS4          = new ldvBrush(NS_CLASSLIB::TColor::LtRed) ;
  pPinceauSH1         = new ldvBrush(NS_CLASSLIB::TColor::LtCyan, HS_DIAGCROSS) ;
	pPinceauSH2         = new ldvBrush(NS_CLASSLIB::TColor::LtGreen, HS_DIAGCROSS) ;
	pPinceauSH3         = new ldvBrush(NS_CLASSLIB::TColor::LtYellow, HS_DIAGCROSS) ;
	pPinceauSH4         = new ldvBrush(NS_CLASSLIB::TColor::LtRed, HS_DIAGCROSS) ;

	pPenEclaire         = new OWL::TPen(NS_CLASSLIB::TColor::White, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pPenEclaireSys      = new OWL::TPen(NS_CLASSLIB::TColor::Sys3dHilight, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pPenOmbre           = new OWL::TPen(NS_CLASSLIB::TColor::Gray, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pPenOmbreSys        = new OWL::TPen(NS_CLASSLIB::TColor::Sys3dDkShadow, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pPenTransition      = new OWL::TPen(NS_CLASSLIB::TColor::LtGray, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pPenTransitionSys   = new OWL::TPen(NS_CLASSLIB::TColor::Sys3dShadow, /* int width */ 1 /*, int style=PS_SOLID*/) ;

	pSelPenEclaire      = new OWL::TPen(NS_CLASSLIB::TColor::LtBlue, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pSelPenOmbre        = new OWL::TPen(NS_CLASSLIB::TColor::Gray, /* int width */ 1 /*, int style=PS_SOLID*/) ;
	pSelPenTransition   = new OWL::TPen(NS_CLASSLIB::TColor(0, 0, 10), /* int width */ 1 /*, int style=PS_SOLID*/) ;

  pPenS1              = new OWL::TPen(NS_CLASSLIB::TColor::LtCyan, /* int width */ 1 /*, int style=PS_SOLID*/) ;
  pPenS2              = new OWL::TPen(NS_CLASSLIB::TColor::LtGreen, /* int width */ 1 /*, int style=PS_SOLID*/) ;
  pPenS3              = new OWL::TPen(NS_CLASSLIB::TColor::LtYellow, /* int width */ 1 /*, int style=PS_SOLID*/) ;
  pPenS4              = new OWL::TPen(NS_CLASSLIB::TColor(255, 204, 0), /* int width */ 1 /*, int style=PS_SOLID*/) ;
  pPenS5              = new OWL::TPen(NS_CLASSLIB::TColor::LtRed, /* int width */ 1 /*, int style=PS_SOLID*/) ;

	pLdvObject          = new OWL::TIcon(*GetApplication(), (TResId)IDI_LDVOBJET16) ;
	pLdvSsObject        = new OWL::TIcon(*GetApplication(), (TResId)IDI_LDVSSOBJ16) ;

	SetScrollRange(SB_HORZ, 0, 100) ;
	SetScrollPos(SB_HORZ, 100) ;

	NS_CLASSLIB::TRect wRect = GetClientRect() ;
	int iTiers = wRect.Width() / 3 ;
	if (iTiers > 10)
	{
  	switch (iXunit)
    {
    	case pixSeconde :
      	_tDateHeureInfDroit.ajouteSecondes(iTiers) ;
        break ;
      case pixMinute :
      	_tDateHeureInfDroit.ajouteMinutes(iTiers) ;
        break ;
    	case pixHeure :
      	_tDateHeureInfDroit.ajouteHeures(iTiers) ;
        break ;
			case pixJour :
      	_tDateHeureInfDroit.ajouteJours(iTiers) ;
        break ;
			case pixSemaine :
      	_tDateHeureInfDroit.ajouteJours(7*iTiers) ;
        break ;
			case pixMois :
      	_tDateHeureInfDroit.ajouteMois(iTiers) ;
        break ;
    }
  }
  // timeBumper() ;

  setTipsTimer() ;

  if (false == _aToons.empty())
  	for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
    	(*toonIter)->SetupWindow() ;

  sTraceText = string("NSLdvViewBase leaving SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase SetupWindow.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//  Destructeur
// -----------------------------------------------------------------------------
NSLdvViewBase::~NSLdvViewBase()
{
try
{
  lObjectCount-- ;

  killTipsTimer() ;

	delete pLdvTankView ;

	delete pSB ;

	ImgDragDrop->RemoveAll() ;
	delete ImgDragDrop ;

	delete pPinceauFond ;
	delete pPinceauFondSys ;

	delete pPinceauBlack ;
	delete pPinceauRed ;
	delete pPinceauBleu ;
	delete pPinceauS1 ;
	delete pPinceauS2 ;
	delete pPinceauS3 ;
  delete pPinceauS34 ;
	delete pPinceauS4 ;
  delete pPinceauSH1 ;
	delete pPinceauSH2 ;
	delete pPinceauSH3 ;
	delete pPinceauSH4 ;

	delete pPenEclaire ;
	delete pPenEclaireSys ;
	delete pPenOmbre ;
	delete pPenOmbreSys ;
	delete pPenTransition ;
	delete pPenTransitionSys ;

	delete pSelPenEclaire ;
	delete pSelPenOmbre ;
	delete pSelPenTransition ;

  delete pPenS1 ;
  delete pPenS2 ;
  delete pPenS3 ;
  delete pPenS4 ;
  delete pPenS5 ;

	delete pLdvObject ;
	delete pLdvSsObject ;

  if (0 != hAccelerator)
  {
    DestroyAcceleratorTable(hAccelerator) ;
    hAccelerator = 0 ;
  }
}
catch (...)
{
  erreur("Exception NSLdvViewBase destructor.", standardError, 0) ;
}
}

//
// Destructor for a TWindow: disposes of each window in its ChildList
// and removes itself from a non-0 parent's list of children
//
// Destroys a still-associated MS-Windows interface element and frees
// the instance thunk used for association of an MS-Windows element
// to the TWindow
//
// Disposes of its Scroller if the TScroller object was constructed
//

static void NSshutDown(TWindow* win, void*)
{
	win = 0;
}

void
NSLdvViewBase::EvHotKey(int idHotKey)
{
}

void
NSLdvViewBase::reinit()
{
try
{
  disconnectCurves() ;

  reinitWorkingAreas() ;
  reinitCurves() ;

  EvaluateWinToonsPos(this, &_aToons) ;
}
catch (...)
{
  erreur("Exception NSLdvViewBase reinit.", standardError, 0) ;
}
}

void
NSLdvViewBase::disconnectCurves()
{
  if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonCurvePannel == (*toonIter)->getType())
    {
      NSLdvCurvesManagementPannel *pCurvesMgr = TYPESAFE_DOWNCAST(*toonIter, NSLdvCurvesManagementPannel) ;
      if (pCurvesMgr)
        pCurvesMgr->disconnectCurves() ;
    }
  }
}

void
NSLdvViewBase::reinitCurves()
{
  if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonCurvePannel == (*toonIter)->getType())
    {
      NSLdvCurvesManagementPannel *pCurvesMgr = TYPESAFE_DOWNCAST(*toonIter, NSLdvCurvesManagementPannel) ;
      if (pCurvesMgr)
        pCurvesMgr->reinit() ;
    }
  }
}

bool
NSLdvViewBase::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;

/*
  if (hAccelerator)
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
*/
}

void
NSLdvViewBase::createWorkingArea(string sConcernRef)
{
  NSLdvViewArea* pWorkingArea = new NSLdvViewArea(pContexte, 0, this, sConcernRef, true) ;
  initWorkingArea(pWorkingArea, true) ;
}

void
NSLdvViewBase::initWorkingArea(NSLdvViewArea* pWorkArea, bool bAtCreation)
{
try
{
  if ((NSLdvViewArea*) NULL == pWorkArea)
    return ;

  string sTraceText = string("NSLdvViewBase entering initWorkingArea.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if (false == bAtCreation)
  {
    sTraceText = string("NSLdvViewBase must reinit WorkingArea.") ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    pWorkArea->reinit() ;
  }

  // Ligne de base - base line
  NSBaseLineView* pBaseLine = new NSBaseLineView(pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->push_back(pBaseLine) ;

	// Problèmes
  sTraceText = string("NSLdvViewBase init WorkingArea for health issues.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->init(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

	// Objets et sous objets - objects and components
  sTraceText = string("NSLdvViewBase init WorkingArea for objects and QoLs.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  pWorkArea->getToonsArray()->init(_pLdVDoc->getQoLCharts(), pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->init(_pLdVDoc->getObjets(), pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->init(_pLdVDoc->getSsObjets(), pContexte, pWorkArea) ;

	// Objectifs de santé
  sTraceText = string("NSLdvViewBase init WorkingArea for health goals.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->initGoals(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

  // Treatments
  sTraceText = string("NSLdvViewBase init WorkingArea for treatments.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->initTreatments(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

  sTraceText = string("NSLdvViewBase init WorkingArea, after toons init.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  pWorkArea->afterToonsInit() ;

  sTraceText = string("NSLdvViewBase init WorkingArea, leaving.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::initWorkingArea.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvViewBase::emptyWorkingAreas()
{
  if (true == _aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea)
        pWorkArea->reinit() ;
    }
  }
}

void
NSLdvViewBase::reinitWorkingAreas()
{
  if (true == _aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea)
        initWorkingArea(pWorkArea) ;
    }
  }
}

// -----------------------------------------------------------------------------
// calcul de la date de naissance du patient en NVLdVTemps
// -----------------------------------------------------------------------------
// NVLdVTemps	NSLdvViewBase::getDateNaissance()
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
NVLdVTemps
NSLdvViewBase::getDateNaissance()
{
	NVLdVTemps  tNaissancePatient ;

	string sDate = pContexte->getPatient()->donneNaissance() ;

	if (string("00000000") == sDate)
	{
  	tNaissancePatient.init() ;
    return tNaissancePatient ;
	}

	//string  sDate   = pContexte->getPatient()->pFicheAdmin->pDateN->sContenuBrut ;

	int     iDay    = atoi(string(sDate, 6, 2).c_str()) ;
	int     iMonth  = atoi(string(sDate, 4, 2).c_str()) ;
	int     iYear   = atoi(string(sDate, 0, 4).c_str()) ;
	if (iDay == 0)
		iDay = 1 ;
	if (iMonth == 0)
		iMonth = 1 ;
	tNaissancePatient = NVLdVTemps(iYear, iMonth, iDay, 0, 0, 0) ;

	return tNaissancePatient ;
}

// -----------------------------------------------------------------------------
// récupération de l'événement déclenché lors d'un scroll horizontal
// -----------------------------------------------------------------------------
// NSLdvViewBase::EvHScroll(UINT, UINT, HWND)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSLdvViewBase::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
try
{
	// récupération de la date de naissance du patient
	NVLdVTemps  tNaissancePatient = getDateNaissance() ;

	NVLdVTemps  tCurrentDate ;
	tCurrentDate.takeTime() ;
	float   fDen = float((tCurrentDate.donneAns() * 12 + tCurrentDate.donneMois()) - (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois())) ;

	string sMessage = string("") ;

	string sLang = pContexte->getUserLanguage() ;

	switch (iXunit)
	{
  	case pixSeconde :

      switch(scrollCode)
      {
			  case SB_LINERIGHT :
				  _tDateHeureInfDroit.ajouteMinutes(1) ;
				  break ;
			  case SB_LINELEFT :
				  _tDateHeureInfDroit.ajouteMinutes(-1) ;
				  break ;
			  case SB_PAGERIGHT :
				  _tDateHeureInfDroit.ajouteHeures(1) ;
				  break ;
			  case SB_PAGELEFT :
				  _tDateHeureInfDroit.ajouteHeures(-1) ;
				  break ;
			  case SB_THUMBPOSITION :
			  {
				  int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				  int iAnnee = iMois / 12 ;
				  iMois = iMois - iAnnee * 12 + 1 ;
				  _tDateHeureInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ;
				  break ;
			  }
			  case SB_THUMBTRACK :
			  {
				  int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				  int iAnnee = iMois / 12 ;
				  iMois = iMois - iAnnee * 12 + 1 ;
				  NVLdVTemps tScrollingDateInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ; // donne tDateHeureInfDroit
				  int iLargeurFenetreEnJours = GetClientRect().Width() / 86400 ;
				  tScrollingDateInfDroit.ajouteJours(-(iLargeurFenetreEnJours / 2)) ;

				  string sDate  = tScrollingDateInfDroit.donneDate();
				  string sHeure = tScrollingDateInfDroit.donneHeure();

				  sMessage = donne_date(sDate, sLang) + string(" ") + donne_heure(sHeure) ;
				  afficheStatusMessage(sMessage, true);
				  break ;
			  }
      }
      break ;

		case pixMinute :

      switch(scrollCode)
      {
			  case SB_LINERIGHT :
				  _tDateHeureInfDroit.ajouteHeures(1) ;
				  break ;
			  case SB_LINELEFT :
				  _tDateHeureInfDroit.ajouteHeures(-1) ;
				  break ;
			  case SB_PAGERIGHT :
				  _tDateHeureInfDroit.ajouteJours(1) ;
				  break ;
			  case SB_PAGELEFT :
				  _tDateHeureInfDroit.ajouteJours(-1) ;
			  	break ;
			  case SB_THUMBPOSITION :
			  {
				  int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				  int iAnnee = iMois / 12 ;
				  iMois = iMois - iAnnee * 12 + 1 ;
				  _tDateHeureInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ;
				  break ;
			  }
			  case SB_THUMBTRACK :
			  {
				  int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				  int iAnnee = iMois / 12 ;
				  iMois = iMois - iAnnee * 12 + 1 ;
				  NVLdVTemps tScrollingDateInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ; // donne tDateHeureInfDroit
				  int iLargeurFenetreEnJours = GetClientRect().Width() / 1440 ;
				  tScrollingDateInfDroit.ajouteJours(-(iLargeurFenetreEnJours / 2)) ;

				  string sDate  = tScrollingDateInfDroit.donneDate();
				  string sHeure = tScrollingDateInfDroit.donneHeure();

				  sMessage = donne_date(sDate, sLang) + string(" ") + donne_heure(sHeure) ;
				  afficheStatusMessage(sMessage, true) ;
				  break ;
			  }
      }
      break ;

		case pixHeure :
			if (scrollCode == SB_LINERIGHT)
			{
				_tDateHeureInfDroit.ajouteJours(1) ;
				break ;
			}
			if (scrollCode == SB_LINELEFT)
			{
				_tDateHeureInfDroit.ajouteJours(-1) ;
				break ;
			}
			if (scrollCode == SB_PAGERIGHT)
			{
				_tDateHeureInfDroit.ajouteMois(1) ;
				break ;
			}
			if (scrollCode == SB_PAGELEFT)
			{
				_tDateHeureInfDroit.ajouteMois(-1) ;
				break ;
			}
			if (scrollCode == SB_THUMBPOSITION)
			{
				int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				int iAnnee = iMois / 12 ;
				iMois = iMois - iAnnee * 12 + 1 ;
				_tDateHeureInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ;
				break ;
			}
			if (scrollCode == SB_THUMBTRACK)
			{
				int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				int iAnnee = iMois / 12 ;
				iMois = iMois - iAnnee * 12 + 1 ;
				NVLdVTemps tScrollingDateInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ; // donne tDateHeureInfDroit
				int iLargeurFenetreEnJours = GetClientRect().Width() / 24 ;
				tScrollingDateInfDroit.ajouteJours(-(iLargeurFenetreEnJours / 2)) ;

				string sDate  = tScrollingDateInfDroit.donneDate() ;
				string sHeure = tScrollingDateInfDroit.donneHeure() ;

				sMessage = donne_date(sDate, sLang) + string(" ") + donne_heure(sHeure) ;
				afficheStatusMessage(sMessage, true) ;
				break ;
			}

		case pixJour :
			if (scrollCode == SB_LINERIGHT)
			{
				_tDateHeureInfDroit.ajouteMois(1) ;
				break ;
			}
			if (scrollCode == SB_LINELEFT)
			{
				_tDateHeureInfDroit.ajouteMois(-1) ;
				break ;
			}
			if (scrollCode == SB_PAGERIGHT)
			{
				_tDateHeureInfDroit.ajouteAns(1) ;
				break ;
			}
			if (scrollCode == SB_PAGELEFT)
			{
				_tDateHeureInfDroit.ajouteAns(-1) ;
				break ;
			}
			if (scrollCode == SB_THUMBPOSITION)
			{
				int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				int iAnnee = iMois / 12 ;
				iMois = iMois - iAnnee * 12 + 1 ;
				_tDateHeureInfDroit = NVLdVTemps(iAnnee, 1, 1, 0, 0, 0) ;
				break ;
			}
			if (scrollCode == SB_THUMBTRACK)
			{
				int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				int iAnnee = iMois / 12 ;
				iMois = iMois - iAnnee * 12 + 1 ;
				NVLdVTemps tScrollingDateInfDroit = NVLdVTemps(iAnnee, iMois, 1, 0, 0, 0) ; // donne tDateHeureInfDroit
				int iLargeurFenetreEnJours = GetClientRect().Width() ;
				tScrollingDateInfDroit.ajouteJours(-(iLargeurFenetreEnJours / 2)) ;

				string sDate  = tScrollingDateInfDroit.donneDate() ;
				string sHeure = tScrollingDateInfDroit.donneHeure() ;

        sMessage = donne_date(sDate, sLang) + string(" ") + donne_heure(sHeure) ;
				afficheStatusMessage(sMessage, true) ;
				break ;
			}

		case pixSemaine :
		case pixMois :

			if (scrollCode == SB_LINERIGHT)
			{
				_tDateHeureInfDroit.ajouteAns(1) ;
				break ;
			}
			if (scrollCode == SB_LINELEFT)
			{
				_tDateHeureInfDroit.ajouteAns(-1) ;
				break ;
			}
			if (scrollCode == SB_PAGERIGHT)
			{
				_tDateHeureInfDroit.ajouteAns(10) ;
				break ;
			}
			if (scrollCode == SB_PAGELEFT)
			{
				_tDateHeureInfDroit.ajouteAns(-10) ;
				break ;
			}
			if (scrollCode == SB_THUMBPOSITION)
			{
				int iMois = ((float(thumbPos) / 100) * fDen) + (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois()) ;
				int iAnnee = iMois / 12 ;
				iMois = iMois - iAnnee * 12 + 1 ;
				_tDateHeureInfDroit = NVLdVTemps(iAnnee, 1, 1, 0, 0, 0) ;
				break ;
			}

			break ;
	}

  timeBumper() ;

	if (_tDateHeureInfDroit < tCurrentDate)
	{
		float   fNum = float((_tDateHeureInfDroit.donneAns() * 12 + _tDateHeureInfDroit.donneMois()) - (tNaissancePatient.donneAns() * 12 + tNaissancePatient.donneMois())) ;
		float   fPosition = (fNum / fDen) * 100 ;

		SetScrollPos(SB_HORZ, int(fPosition)) ;
	}
	else
		SetScrollPos(SB_HORZ, 100) ;

	if (scrollCode != SB_THUMBTRACK)
		Invalidate() ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase EvHScroll.", standardError, 0) ;
}
}

LRESULT
NSLdvViewBase::EvMouseWheel(WPARAM wParam, LPARAM lParam)
{
try
{
	// WORD    fwKeys  = LOWORD(wParam);           // key flags
	short   zDelta  = (short) HIWORD(wParam);   // wheel rotation
	// short   xPos    = (short) LOWORD(lParam);   // horizontal position of pointer
	// short   yPos    = (short) HIWORD(lParam);   // vertical position of pointer

	UINT    scrollCode;

	// A positive value indicates that the wheel was rotated forward,
	// away from the user
	if (zDelta > 0)
		scrollCode = SB_LINERIGHT;

	// a negative value indicates that the wheel was rotated backward,
	// toward the user.
	else
		if (zDelta < 0)
			scrollCode = SB_LINELEFT;

	for (int i = 0; i < 4; i++)
		EvHScroll(scrollCode, standardError, 0) ;

    return 0 ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase EvMouseWheel.", standardError, 0) ;
	return 0 ;
}
}

void
NSLdvViewBase::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSLdvViewBase::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvSysKeyDown(key, repeatCount, flags) ;
}

void
NSLdvViewBase::CmHelp()
{
	pContexte->NavigationAideEnLigne() ;
}

void
NSLdvViewBase::UpdatePixelPerUnitRate(uint)
{
	NSLdvChronoLine* pChronoLine = getLdVChronoLine() ;
  if (NULL == pChronoLine)
		return ;

	int iPpuPos = pChronoLine->getPpuRatePosition() ;
	if (iPpuPos == pCurrentTimeInfo->getUppRate())
		return ;

  if (iPpuPos >= pCurrentTimeInfo->getUpperLimit())
    CmLdvZoomOut() ;
  else
  {
    pCurrentTimeInfo->setUppRate(iPpuPos) ;
    pixelUnitChanged() ;
  }
}

void
NSLdvViewBase::UpdateSeverityThreshold(uint)
{
	NSLdvCommandPannel* pCommandPannel = getLdVCommandPannel() ;
  if ((NSLdvCommandPannel*) NULL == pCommandPannel)
		return ;

  int iThreshold = pCommandPannel->getSeverityThreshold() ;

	if (iThreshold == _iMinSeverityOfDisplayedConcerns)
		return ;

	_iMinSeverityOfDisplayedConcerns = iThreshold ;

  reinitWorkingAreas() ;

  Invalidate() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

bool
NSLdvViewBase::CanClose()
{
  if ((pContexte->getPatient()) && (pContexte->getPatient()->_bCanCloseHisto))
    return true ;

  return false ;
}

void
NSLdvViewBase::EvDragDrop()
{
}

//---------------------------------------------------------------------------
//fixer la position de la Ligne de vie
//---------------------------------------------------------------------------
void
NSLdvViewBase::SetWindowPosLdv()
{
  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty(string("LigneDeVie")) ;
  if ((NSWindowProperty*) NULL == pWinProp)
	{
  	SetWindowPosit() ;
    return ;
	}

  Parent->Show(SW_HIDE) ;

  NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

  //
  // fixer la nouvelle position
  // (on ne tient pas compte de la taille, vu le probleme pour restaurer
  //  une fenetre TView,TWindow mise en icone)
  //
  Parent->SetWindowPos(0, targetRect.Left(), targetRect.Top(), targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  Parent->Show(SW_SHOWNORMAL) ;

  /************************************
  if (sTaille == "I")
      Parent->Show(SW_SHOWMINIMIZED);
  else if (sTaille == "Z")
      Parent->Show(SW_SHOWMAXIMIZED);
  else
      Parent->Show(SW_SHOWNORMAL);
  *************************************/
}

//---------------------------------------------------------------------------
//enregistrer la position de la fiche historique la base UTILISAT.DB
//---------------------------------------------------------------------------
void
NSLdvViewBase::EnregistrePosLdv()
{
	if (false == pContexte->getUtilisateur()->isEnregWin())
  	return ;

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

	string sUserId = pContexte->getUtilisateur()->getNss() ;
	NS_CLASSLIB::TRect  rect(X, Y, X + W, Y + H) ;
  NSWindowProperty wndProp ;
  wndProp.setX(X) ;
  wndProp.setY(Y) ;
  wndProp.setW(W) ;
  wndProp.setH(H) ;
  wndProp.setActivity(NSWindowProperty::undefined) ;
  wndProp.setFunction("LigneDeVie") ;

  pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, false) ;
  pContexte->getUtilisateur()->saveWindowProperty(sUserId, pContexte->PathName("FGLO"), &wndProp, true) ;
  //pContexte->getUtilisateur()->aWinProp.setProperty(sUserId, "History", rect, pContexte->PathName("FGLO")) ;
}

//
// Deprecated
//
void
NSLdvViewBase::SetWindowPosit()
{
try
{
	string sPosition = pContexte->getEpisodus()->sPosLdV ;
	if (string("") == sPosition)
		return ;

	string  sTaille ;
	size_t  debut = 0 ;
	size_t  positVide = sPosition.find("|") ;  //chaîne vide
	int     nbCoords = 0 ;

	vector<string> Coordonnees;  //contient les coordonnées sous forme de string

	while (NPOS != positVide)
	{
		Coordonnees.push_back(string(sPosition, debut, positVide - debut)) ;
		nbCoords++ ;
		debut = positVide + 1 ;
		positVide = sPosition.find("|", debut+1) ;
	}
	Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut));
	nbCoords++;

	// récupérer les coordonnées
	vector<string>::iterator i ;
	i = Coordonnees.begin() ;
	int X = StringToDouble(*i) ;
	i++ ;
	int Y = StringToDouble(*i) ;
	i++ ;
	int W  = StringToDouble(*i) ;
	i++ ;
	int H = StringToDouble(*i) ;

	// cas en icone ou plein ecran
	if (nbCoords > 4)
	{
		i++ ;
		sTaille = *i ;
	}
	else
		sTaille = string("N") ;

	Parent->SetWindowPos(0, X, Y, W, H, SWP_NOZORDER) ;

	if (string("I") == sTaille)
		Parent->Show(SW_SHOWMINIMIZED) ;
	else
		if (string("Z") == sTaille)
    	Parent->Show(SW_SHOWMAXIMIZED) ;
    else
        Parent->Show(SW_SHOWNORMAL) ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase SetWindowPosit.", standardError, 0) ;
}
}

void
NSLdvViewBase::RecordWindowPosit()
{
try
{
	string sPosLigne = pContexte->getEpisodus()->sPosLdV;

	NS_CLASSLIB::TRect rectDlg = Parent->GetWindowRect();//coordonnées par % à l'écran
	NS_CLASSLIB::TPoint point(rectDlg.X(), rectDlg.Y());
	Parent->Parent->ScreenToClient(point);
	int X = point.X() ;
	int Y = point.Y() ;
	// int W = rectDlg.Width() ;
	// int H = rectDlg.Height() ;

	double TamponX  = (double)X;
	double TamponY  = (double)Y;
	double TamponW  = (double)rectDlg.Width();
	double TamponH  = (double)rectDlg.Height();
	string sPosition = DoubleToString(&TamponX, 0 , 0)
												+ "|" + DoubleToString(&TamponY, 0 , 0)
												+ "|" + DoubleToString(&TamponW, 0 , 0)
												+ "|" + DoubleToString(&TamponH, 0 , 0);

	// On regarde si la fenetre est en icone ou en plein ecran
	if ((Parent->IsIconic()) || (Parent->IsZoomed()))
	{
		size_t poslast;

		sPosition = sPosLigne;
		poslast = sPosition.find_last_of("|");
		// si le '|' n'est pas le dernier caractere
		if ((poslast != NPOS) && (poslast < (strlen(sPosition.c_str()) - 1)))
		{
			// si la fenetre était déja en icone ou en zoom :
			// on reprend l'ancienne taille
			if ((sPosition[poslast + 1] == 'I') || (sPosition[poslast + 1] == 'Z'))
				sPosition = string(sPosition, 0, poslast);

			if (Parent->IsIconic())
				sPosition += "|I";
			else
				sPosition += "|Z";
		}
		else
		{
			erreur("Coordonnée éronnée.", standardError, 0, GetHandle());
			// on remet dans ce cas la fenetre a une taille par défaut
			sPosition = "0|0|100|100";
		}
	}
	pContexte->getEpisodus()->sPosLdV = sPosition;
}
catch (...)
{
	erreur("Exception NSLdvViewBase RecordWindowPosit.", standardError, 0) ;
}
}

void
NSLdvViewBase::reloadView(string sReason, string sReference)
{
  if (true == _aToons.empty())
    return ;

  bool bMustReinitView = false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea)
      {
	      if      (string("DRUG_DELETED") == sReason)
	      {
  	      deleteAllFromToonsArray(pWorkArea->getToonsArray(), NSLdvToon::toonDrug) ;
		      pWorkArea->getToonsArray()->initTreatments(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;
        }
        else if (string("DRUG_NEW") == sReason)
  	      bMustReinitView = true ;
      }
    }
  }

  if (bMustReinitView)
    reinit() ;

	Invalidate() ;
}

/*********************************
void
NSLdvViewBase::PerformCreate(int menuOrId)
{
    // pLdVDoc->SetTitle("Ligne de vie Episodus");
}
**************************************/

void
NSLdvViewBase::EvTimer(uint timerId)
{
	if (ID_LDVTIMER != timerId)
		return ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (false == pMyApp->isForegroundApplication())
  {
    _iNbTimerTicks = 0 ;
    return ;
  }

  NS_CLASSLIB::TPoint curLoc ;
  GetCursorPos(curLoc) ;

  if (curLoc != _previousCursorLocation)
  {
    _previousCursorLocation = curLoc ;
    _iNbTimerTicks = 0 ;
    return ;
  }

  _iNbTimerTicks++ ;

  NSLdvViewArea* pWorkArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkArea)
    return ;

	if (_iNbTimerTicks > 3)
	{
		_iNbTimerTicks = 0 ;
		//killTipsTimer();

    pWorkArea->showContextHelp() ;
	}
  else
    pWorkArea->subSignalForHelp() ;
}

void
NSLdvViewBase::timeBumper()
{
try
{
  NSLdvViewArea* pWorkArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkArea)
    return ;

  TWindow* pWinWorkArea = pWorkArea->getInterface() ;
  if (NULL == pWinWorkArea)
    return ;

	NS_CLASSLIB::TRect wRect = pWinWorkArea->GetClientRect() ;
	int iWidth = wRect.Width() ;

	string sRootDate = string("19010101000000") ;

  // Don't do this, because the multiplier, as an int, would not be accurate
  //
  // int iMultiplier = pCurrentTimeInfo->getUpperLimit() / (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) ;

	// pix/month is the only mode that cannot get mapped into seconds
	//
	if (pixMois == iXunit)
  {
  	unsigned int iWidthInMonth = iWidth * pCurrentTimeInfo->getUpperLimit() / (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) ;
    unsigned int iMonthsInfDroit = 12 * (_tDateHeureInfDroit.donneAns() - 1901) + _tDateHeureInfDroit.donneMois() ;

    if (iWidthInMonth > iMonthsInfDroit)
    {
    	_tDateHeureInfDroit.initFromDateHeure(sRootDate) ;
      _tDateHeureInfDroit.ajouteMois(iWidthInMonth + 1, true) ;
    }

    return ;
  }

  unsigned long lWidthInSeconds = 0 ;

  switch (iXunit)
	{
		case pixSeconde :
    	lWidthInSeconds = iWidth ;
      break ;
  	case pixMinute :
    	lWidthInSeconds = 60 * iWidth ;
      break ;
    case pixHeure :
    	lWidthInSeconds = 3600 * iWidth ;
      break ;
  	case pixJour :
    	lWidthInSeconds = 24 * 3600 * iWidth ;
      break ;
    case pixSemaine :
    	lWidthInSeconds = 7 * 24 * 3600 * iWidth ;
      break ;
  }

  if ((pCurrentTimeInfo->getUpperLimit() > pCurrentTimeInfo->getUppRate()))
    lWidthInSeconds = lWidthInSeconds * pCurrentTimeInfo->getUpperLimit() / (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) ;

	ClassLib::TDate tdDate = ClassLib::TDate(_tDateHeureInfDroit.donneJours(), _tDateHeureInfDroit.donneMois(), _tDateHeureInfDroit.donneAns()) ;
	ClassLib::TTime ttTime = ClassLib::TTime(tdDate, _tDateHeureInfDroit.donneHeures(), _tDateHeureInfDroit.donneMinutes(), _tDateHeureInfDroit.donneSecondes()) ;
	ClockTy nbSecSince19010101 = ttTime.Seconds() ;

	if (lWidthInSeconds > nbSecSince19010101)
	{
		_tDateHeureInfDroit.initFromDateHeure(sRootDate) ;
		_tDateHeureInfDroit.ajouteSecondes(lWidthInSeconds + 1) ;
	}

	return ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::timeBumper.", standardError, 0) ;
}
}

void
NSLdvViewBase::pixelUnitChanged()
{
  pCurrentTimeInfo = getTimeZoomInfo(iXunit) ;
  refreshChronoLine() ;
	timeBumper() ;
	Invalidate() ;
}

void
NSLdvViewBase::showMousePositionMessage()
{
/*
	NS_CLASSLIB::TPoint point ;
  GetCursorPos(point) ;

  NS_CLASSLIB::TRect  clientRect = GetClientRect();
  NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight();

  if (iMouseMode == modeZoom)
  	point.x += GetSystemMetrics(SM_CXCURSOR) / 2 ;

  // Calcul du point clické

  NVLdVPoint pointClick(this);
  pointClick.initialise(&ptOrigine, &point);
  NVLdVTemps tDateHeure = pointClick.getX();

  string sDate  = tDateHeure.donneDate();
  string sHeure = tDateHeure.donneHeure();

  char szDate[11];
  string sHeureLib;

  donne_date((char*)(sDate.c_str()),   szDate,  sLang);
  donne_heure((char*)(sHeure.c_str()), sHeureLib);

  string sMessage = string(szDate) + string(" ") + sHeureLib;
  afficheStatusMessage(&sMessage, false);
*/
}

void
NSLdvViewBase::skinSwitchOn(string sSkinName)
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

	/****************************
  if (!bSkinable)
  {
        SetBkgndColor(0x00fff0f0) ; // 0x00bbggrr
  }
  ****************************/

	Invalidate() ;
}

void
NSLdvViewBase::skinSwitchOff(string sSkinName)
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


void
NSLdvViewBase::CmChgPreoDate(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
	if (NULL == pConcern)
		return ;

	NVLdVTemps tDeb = pConcern->_tDateOuverture ;
	NVLdVTemps tFin = pConcern->_tDateFermeture ;

	NSModifConcernDateDlg* pMPDialog =
						new NSModifConcernDateDlg(iFrame, this, &tDeb, &tFin, pContexte) ;
	int iExecReturn = pMPDialog->Execute() ;
	if (IDOK != iExecReturn)
		return ;

	if ((tDeb == pConcern->_tDateOuverture) &&
			(tFin == pConcern->_tDateFermeture))
		return ;


	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pConcern->_pDoc)
		return ;

	if (!(tDeb == pConcern->_tDateOuverture))
		pConcern->_pDoc->changeConcernDateDeb(pConcern, &tDeb) ;

	// Date de fin modifiée - Ending date has been changed
	if (!(tFin == pConcern->_tDateFermeture))
		pConcern->_pDoc->changeConcernDateFin(pConcern, &tFin) ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
  {
    NSDocumentHisto* pDocument = pFrameInfo->findNode(pConcern->getNoeud()) ;
    if (pDocument)
      pConcern->_pDoc->updateIndexDocument(pDocument, false) ;
	  pFrameInfo->getConcernsArray()->reinit() ;
  }

	Invalidate() ;
}
catch (...)
{
	erreur("Exception CmChgPreoDate.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmChgPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame)
{
try
{
  if (NULL == pConcern)
		return ;

	if (pConcern->_aModificateurs.empty())
		return ;

	ArrayModifierIter iModif = pConcern->_aModificateurs.begin() ;
	for ( ; pConcern->_aModificateurs.end() != iModif ; iModif++)
	{
		if (((*iModif)->tDateHeureDeb <= tpsTarget) &&
				((*iModif)->tDateHeureFin >= tpsTarget))
			break;
	}
	if (pConcern->_aModificateurs.end() == iModif)
		return ;

	int iS = (*iModif)->iSeverite ;
	int iR = (*iModif)->iRisque ;

	// NSModifConcernIndexDlg* pMPDialog = new NSModifConcernIndexDlg(	&iS, &iR, pContexte);
  NSModifConcernIndexDlg* pMPDialog = new NSModifConcernIndexDlg(iFrame, this, &iS, pContexte) ;

	int iExecReturn = pMPDialog->Execute() ;
	if (IDOK != iExecReturn)
		return ;

	if ((iS == (*iModif)->iSeverite) && (iR == (*iModif)->iRisque))
  	return ;

	if (iS == (*iModif)->iSeverite)
		iS = -1 ;
	if (iR == (*iModif)->iRisque)
		iR = -1 ;

	_pLdVDoc->changeConcernModifier(pConcern, *iModif, iS, iR) ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
  {
    NSDocumentHisto* pDocument = pFrameInfo->findNode(pConcern->getNoeud()) ;
    if (pDocument)
      pConcern->_pDoc->updateIndexDocument(pDocument, false) ;
  }

	Invalidate() ;
}
catch (...)
{
	erreur("Exception CmChgPreoIndx.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmEvolPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

	int iS = 10 ;
	int iR = 0 ;
	if (false == pConcern->_aModificateurs.empty())
	{
		ArrayModifierIter iModif = pConcern->_aModificateurs.begin() ;
		for ( ; pConcern->_aModificateurs.end() != iModif ; iModif++)
			if (((*iModif)->tDateHeureDeb <= tpsTarget) &&
					((*iModif)->tDateHeureFin >= tpsTarget))
				break ;
		if (pConcern->_aModificateurs.end() != iModif)
		{
			iS = (*iModif)->iSeverite ;
			iR = (*iModif)->iRisque ;
		}
	}

	NVLdVTemps tDeb ;
  tDeb.takeTime() ;

	NSEvolConcernIndexDlg* pMPDialog =
													new NSEvolConcernIndexDlg(iFrame, this, &tDeb, &iS, pContexte) ;
	int iExecReturn = pMPDialog->Execute() ;
	if (iExecReturn != IDOK)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->evolveConcernModifier(pFrameInfo->getFrame(), pConcern, &tDeb, iS, iR) ;

	Invalidate() ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmEvolPreoType.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmSuppressPreo(NSConcern* pConcern)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
	  _pLdVDoc->deleteConcern(pFrameInfo->getFrame(), pConcern) ;

	reinit() ;
	Invalidate() ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmSuppressPreo.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmDrugNew()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if (pWorkingArea)
  {
    TWindow* pWorkInterface = pWorkingArea->getInterface() ;
    if (pWorkInterface)
    {
      NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(pWorkInterface, NSLdvViewAreaWindow) ;
      if (pWorkWin)
      {
        pWorkWin->CmDrugNew() ;
        return ;
      }
    }
  }

  CmDrugNewForConcern(0) ;
}

void
NSLdvViewBase::CmDrugNewForConcern(NSConcern* pRelatedConcern)
{
	VecteurString aRelatedConcerns ;

	if (pRelatedConcern)
    aRelatedConcerns.AddString(pRelatedConcern->_sReference) ;

	_pLdVDoc->DrugNewService(this, string(""), &aRelatedConcerns) ;
}

void
NSLdvViewBase::CmDrugRenew(NSLdvDrug* pDrug)
{
	if ((NSLdvDrug*) NULL == pDrug)
		return ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (string("") != sNodeToAlter)
  	_pLdVDoc->DrugRenewService(this, sNodeToAlter) ;
}

void
NSLdvViewBase::CmDrugModify(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	string sNodeToAlter = pDrug->getNoeud() ;
	if (string("") != sNodeToAlter)
		_pLdVDoc->DrugModifyService(this, sNodeToAlter) ;
}

void
NSLdvViewBase::CmDrugChangePoso(NSLdvDrug* pDrug)
{
	if ((NSLdvDrug*) NULL == pDrug)
		return ;

	_pLdVDoc->DrugChangePosoService(this, pDrug) ;
}

void
NSLdvViewBase::CmDrugStop(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	// string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToStopThisPrescription") ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (sNodeToAlter != string(""))
  	_pLdVDoc->DrugStopService(this, sNodeToAlter) ;
}

void
NSLdvViewBase::CmDrugDelete(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	// string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToDeleteThisPrescription") ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (sNodeToAlter != string(""))
		_pLdVDoc->DrugDeleteService(this, sNodeToAlter) ;
}

void
NSLdvViewBase::CmDrugPrescription()
{
	_pLdVDoc->DrugCreatePrescriptionService(this) ;
}

void
NSLdvViewBase::CmManageRights(NSConcern* pConcern)
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->manageRights(pFrameInfo->getFrame(), this, pConcern->getNoeud()) ;
}

void
NSLdvViewBase::CmManageRights(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pDrug->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->manageRights(pFrameInfo->getFrame(), this, pDrug->getNoeud()) ;
}

void
NSLdvViewBase::CmNewDrugsFromRefForConcern(NSConcern* pConcern)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  _pLdVDoc->DrugSelectProtocolForConcern(this, pConcern->_sReference) ;
}
catch (...)
{
	erreur("Exception CmNewDrugsFromRefForConcern.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmNewGoalsFromRefForConcern(NSConcern* pConcern)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  _pLdVDoc->GoalSelectProtocolForConcern(this, pConcern->_sReference) ;
}
catch (...)
{
	erreur("Exception CmNewGoalsFromRefForConcern.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmChgPreoccup()
{
try
{
	/*
	OWL::TPopupMenu *menu = new OWL::TPopupMenu();

	menu->AppendMenu(MF_STRING, IDC_EVOL_PREOCCUP, "Type");
	menu->AppendMenu(MF_STRING, IDC_CHG_PREOCCUP,  "Dates");
	menu->AppendMenu(MF_STRING, IDC_CHG_PREOCCUP,  "Sévérité");

	ClientToScreen(point);
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow);
	delete menu;
	*/
}
catch (...)
{
	erreur("Exception CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmChgPreoType(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pConcern->_pDoc)
		return ;

	NSPatPathoArray* pPatho ;
	PatPathoIter iter = _pLdVDoc->donnePreoccup(pConcern, &pPatho) ;

	// not found !
	if (NULL == iter)
		return ;

	NSPatPathoInfo noeud = *(*iter) ;

	string sLexique = (*iter)->getLexique() ;

	NSModifConcernTypeDlg* pMPDialog = new NSModifConcernTypeDlg(iFrame, this, &noeud, pContexte) ;
	int iExecReturn = pMPDialog->Execute() ;
	if (IDOK != iExecReturn)
  {
    delete pMPDialog ;
		return ;
  }

  delete pMPDialog ;

	if (noeud == *(*iter))
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->changeConcernType(pFrameInfo->getFrame(), pConcern, &noeud) ;

	Invalidate() ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmChange2Cursor()
{
	iMouseMode = modeCursor ;
}

void
NSLdvViewBase::CmCursorEnable(TCommandEnabler &ce)
{
	if (iMouseMode == modeCursor)
	{
		ce.SetCheck(TCommandEnabler::Checked) ;
		SetCursor(GetModule(), IDC_ARROW) ;
	}
	else
	{
		ce.SetCheck(TCommandEnabler::Unchecked) ;
		SetCursor(GetModule(), IDC_ZOOMCURSOR) ;
	}
}

void
NSLdvViewBase::CmChange2Zoom()
{
	iMouseMode = modeZoom ;
}

void
NSLdvViewBase::CmZoomEnable(TCommandEnabler &ce)
{
	if (iMouseMode == modeZoom)
	{
		ce.SetCheck(TCommandEnabler::Checked) ;
		SetCursor(GetModule(), IDC_ZOOMCURSOR) ;
	}
	else
	{
		ce.SetCheck(TCommandEnabler::Unchecked) ;
		SetCursor(GetModule(), IDC_ARROW) ;
	}
}

void
NSLdvViewBase::CmLdvZoomIn()
{
try
{
  NSLdvViewArea* pActiveWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pActiveWorkingArea)
    return ;
  TWindow* pActiveWorkingWindow = pActiveWorkingArea->getInterface() ;
  if ((TWindow*) NULL == pActiveWorkingWindow)
    return ;

	NS_CLASSLIB::TRect  clientRect = GetClientRect() ;
	NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;
	// int                 iLargeurFenetre = clientRect.Width() ;

  // Take care: GetCursorPos is in screen coordinates
  //
	NS_CLASSLIB::TPoint point ;
	GetCursorPos(point) ;

  // Correction, from screen coordinates to Windows coordinates
  //
  pActiveWorkingWindow->ScreenToClient(point) ;

  // ??? don't do this
	// point.x += GetSystemMetrics(SM_CXCURSOR) / 2 ;

	// Calcul du point où est la souris
	NVLdVPoint pointClick(this) ;
	pointClick.initialise(&ptOrigine, &point, pActiveWorkingArea->getPosInfDroit()) ;
	NVLdVTemps tDateHeure = pointClick.getX() ;

  int iDistance2InfDroit = getTimeUnitFromPhysicalWidth(ptOrigine.X() - point.X()) ;

	switch (iXunit)
	{
  	case pixMinute :
			tDateHeure.ajouteSecondes(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixSecond() ;
			break ;

  	case pixHeure :
			tDateHeure.ajouteMinutes(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixMinute() ;
			break ;

		case pixJour :
			tDateHeure.ajouteHeures(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixHour() ;
			break ;

		case pixSemaine :
			tDateHeure.ajouteJours(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixDay() ;
			break ;

		case pixMois :
			tDateHeure.ajouteJours(iDistance2InfDroit * 7) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixWeek() ;
			break ;
	}
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmLdvZoomIn.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmLdvZoomOut()
{
try
{
  NSLdvViewArea* pActiveWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pActiveWorkingArea)
    return ;
  TWindow* pActiveWorkingWindow = pActiveWorkingArea->getInterface() ;
  if ((TWindow*) NULL == pActiveWorkingWindow)
    return ;

	NS_CLASSLIB::TRect  clientRect = GetClientRect() ;
	NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;
	// int                 iLargeurFenetre = clientRect.Width() ;

  // Take care: GetCursorPos is in screen coordinates
  //
	NS_CLASSLIB::TPoint point ;
	GetCursorPos(point) ;

  // Correction, from screen coordinates to Windows coordinates
  //
  pActiveWorkingWindow->ScreenToClient(point) ;

  // ??? don't do this
	// point.x += GetSystemMetrics(SM_CXCURSOR) / 2 ;

	// Calcul du point où est la souris
	NVLdVPoint pointClick(this) ;
	pointClick.initialise(&ptOrigine, &point, pActiveWorkingArea->getPosInfDroit()) ;
	NVLdVTemps tDateHeure = pointClick.getX() ;

  int iDistance2InfDroit = getTimeUnitFromPhysicalWidth(ptOrigine.X() - point.X()) ;

	switch (iXunit)
	{
  	case pixSeconde :
			tDateHeure.ajouteMinutes(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixMinute() ;
			break ;

  	case pixMinute :
			tDateHeure.ajouteHeures(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixHour() ;
			break ;

		case pixHeure :
			tDateHeure.ajouteJours(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixDay() ;
			break ;

		case pixJour :
			tDateHeure.ajouteJours(iDistance2InfDroit * 7) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixWeek() ;
			break ;

		case pixSemaine :
			tDateHeure.ajouteMois(iDistance2InfDroit) ;
			_tDateHeureInfDroit = tDateHeure ;
      CmLdvPixMonth() ;
			break ;
	}
}
catch (...)
{
	erreur("Exception NSLdvViewBase CmLdvZoomOut.", standardError, 0) ;
}
}

void
NSLdvViewBase::CmShowGoalsForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if ((NSConcernView*) NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if ((NSConcern*) NULL == pCurrentConcern)
    return ;

  // pContexte->getPatient()->goals_show(pCurrentConcern->getNoeud()) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenNewWindow")) ;
  pTask->setParam1(string("GoalManagement")) ;
  pTask->setParam2(pCurrentConcern->getNoeud()) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSLdvViewBase::CmShowDrugsForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if ((NSConcernView*) NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if ((NSConcern*) NULL == pCurrentConcern)
    return ;

  // pContexte->getPatient()->drugs_show(pCurrentConcern->getNoeud()) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenNewWindow")) ;
  pTask->setParam1(string("DrugManagement")) ;
  pTask->setParam2(pCurrentConcern->getNoeud()) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSLdvViewBase::CmShowFollowUpForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if ((NSConcernView*) NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if ((NSConcern*) NULL == pCurrentConcern)
    return ;

  // pContexte->getPatient()->followUp_show(pCurrentConcern->getNoeud()) ;
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenNewWindow")) ;
  pTask->setParam1(string("FollowUpShow")) ;
  pTask->setParam2(pCurrentConcern->getNoeud()) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSLdvViewBase::CmShowCurvesForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if ((NSLdvViewArea*) NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if ((NSConcernView*) NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if ((NSConcern*) NULL == pCurrentConcern)
    return ;

  displayCurvesForBiometricalGoals(pCurrentConcern->getNoeud()) ;
}

void
NSLdvViewBase::CmLdvPixSecond()
{
	iXunit = pixSeconde ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::CmLdvPixMinute()
{
	iXunit = pixMinute ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::CmLdvPixHour()
{
	iXunit = pixHeure ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::CmLdvPixDay()
{
	iXunit = pixJour ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::CmLdvPixWeek()
{
	iXunit = pixSemaine ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::CmLdvPixMonth()
{
	iXunit = pixMois ;
  pixelUnitChanged() ;
}

void
NSLdvViewBase::handlePixelUnitChange()
{
	if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*toonIter, NSLdvTimeToon) ;
    if (pTimeToon)
      pTimeToon->adjustToPixelTimeUnit() ;
  }
}

/*
void
NSLdvViewBase::setZoomLevelInfo()
{
  NSTimeZoomLevel* pTimeZoomInfo = getTimeZoomInfo(iXunit) ;
  if (NULL == pTimeZoomInfo)
    return ;

	iPixelPerUnitRate = pTimeZoomInfo->getUppRate() ;
	iUpperPpuLevel    = pTimeZoomInfo->getUpperLimit() ;
	iLowerPpuLevel    = pTimeZoomInfo->getLowerLimit() ;
}
*/

string
NSLdvViewBase::getLdvPixLexiqueCode()
{
	switch (iXunit)
  {
  	case pixSeconde : return string("2SEC01") ;
    case pixMinute  : return string("2MINU1") ;
    case pixHeure   : return string("2HE001") ;
    case pixJour    : return string("2DAT01") ;
    case pixSemaine : return string("2DAT11") ;
    case pixMois    : return string("2DAT21") ;
    default         : return string("") ;
  }
}

void
NSLdvViewBase::EvSize(uint sizeType, ClassLib::TSize& size)
{
  EvaluateWinToonsPos(this, &_aToons) ;
	Invalidate(true) ;
}

// -----------------------------------------------------------------------------
// Libération de la dernière barre d'outils créee
// -----------------------------------------------------------------------------

void
NSLdvViewBase::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
  if (false == _bPainted)
    return ;

  TMyApp* pMyApp = (TMyApp*) 0 ;

try
{
  TWindowView::EvSetFocus(hWndLostFocus) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase EvSetFocus.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
  // TWindow* pWnd = GetParent() ;
  TMDIChild* pWndChild = TYPESAFE_DOWNCAST(Parent, TMDIChild) ;
  if (pWndChild)
    pWndChild->BringWindowToTop() ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase EvSetFocus (bring window to top).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	pMyApp = pContexte->getSuperviseur()->getApplication() ;

	if (bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))
	{
		SetupToolBar() ;
		pMyApp->SetToolBarWindow(GetWindow());
	}
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase EvSetFocus (setting toolbar).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	string sViewName = pContexte->getSuperviseur()->getText("LigneDeVieManagement", "ligneDeVie") ;
  Parent->SetCaption(sViewName.c_str()) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase EvSetFocus (setting caption).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvViewBase::SetupToolBar()
{
try
{
	TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if ((TMyApp*) NULL == pMyApp)
    return ;

	pMyApp->FlushControlBar() ;

/*
  TDockableControlBarEx* pC2 = pMyApp->getSecondaryControlBar() ;
  if ((TDockableControlBarEx*) NULL == pC2)
    return ;

	pC2->Insert(*new NSFlatButtonGadget(IDI_LDVCURS16, IDI_LDVCURS16, TButtonGadget::Command)) ;
	pC2->Insert(*new NSFlatButtonGadget(IDI_LDVZOOM16, IDI_LDVZOOM16, TButtonGadget::Command)) ;

	pC2->LayoutSession() ;
*/
}
catch (...)
{
	erreur("Exception NSLdvViewBase SetupToolBar.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// fermeture du document :
// L'historique est averti de la fermeture par le destructeur du document
// car lorsque la derniere vue se ferme, cela tue le document.
// -----------------------------------------------------------------------------

void
NSLdvViewBase::EvClose()
{
	TWindow::EvClose();
}

void
NSLdvViewBase::EvDestroy()
{
	EnregistrePosLdv() ;
	// if (pContexte->getSuperviseur()->getEpisodus()->bAutoSave)
	//	RecordWindowPosit();
}

bool
NSLdvViewBase::EvQueryEndSession()
{
	return TWindow::EvQueryEndSession() ;
}

void
NSLdvViewBase::setRightTimeFromLeftTime(const NVLdVTemps& tLeftTime)
{
  NSLdvChronoLine* pChronoLine = getLdVChronoLine() ;
  if ((NSLdvChronoLine*) NULL == pChronoLine)
		return ;

  int iTimeUnitWidth = getTimeUnitFromPhysicalWidth(pChronoLine->GetClientRect().Width()) ;

  _tDateHeureInfDroit = tLeftTime ;

	switch (iXunit)
	{
  	case pixSeconde :
			_tDateHeureInfDroit.ajouteSecondes(iTimeUnitWidth) ;
			break ;
  	case pixMinute :
			_tDateHeureInfDroit.ajouteMinutes(iTimeUnitWidth) ;
			break ;
		case pixHeure :
			_tDateHeureInfDroit.ajouteHeures(iTimeUnitWidth) ;
			break ;
		case pixJour :
			_tDateHeureInfDroit.ajouteJours(iTimeUnitWidth) ;
			break ;
		case pixSemaine :
			_tDateHeureInfDroit.ajouteJours(iTimeUnitWidth * 7) ;
			break ;
		case pixMois :
			_tDateHeureInfDroit.ajouteMois(iTimeUnitWidth) ;
			break ;
  }
}

void
NSLdvViewBase::refreshChronoLine()
{
  NSLdvChronoLine* pChrono = getLdVChronoLine() ;
  if (NULL == pChrono)
    return ;

  pChrono->unitChanging() ;
  pChrono->refresh() ;
  pChrono->unitChanged() ;
}

//
// If iPixelPerUnitRate = 1, there is a 1 pixel to 1 time unit ratio
//                           for example, 1 pixel = 1 month in px/month mode
//
// If iPixelPerUnitRate > 1, the principle is to have the above graduation
//                           get step by step reduced: by example, in pix/hour
//                           mode, a day is 24 pixel width ; it will become
//                           23 pixels wide, then 22, etc, until it becomes
//                           just 1 pixel wide, and the system switches to
//                           the pix/week mode
//
// physicalPoint.x = actual number of pixels
// ldvRect.Width() = number of time units
//
// In px/hour mode, 24 hours should be 24 pixels, but become 24 - iPixelPerUnitRate pixels
// so, x pixels become x * (24 - iPixelPerUnitRate) / 24


NSTimeZoomLevel*
NSLdvViewBase::getTimeZoomInfo(pixUnit iU)
{
  if (aZoomLevels.empty())
		return (NSTimeZoomLevel*) 0 ;

  NSTimeZoomLevelIter zoomLevelIt = aZoomLevels.begin() ;
  for ( ; (aZoomLevels.end() != zoomLevelIt) && ((*zoomLevelIt)->getPixUnit() != iU) ; zoomLevelIt++) ;

  if (aZoomLevels.end() != zoomLevelIt)
    return *zoomLevelIt ;

  return (NSTimeZoomLevel*) 0 ;
}

// -----------------------------------------------------------------------------
//  Function: NSRefVue::Paint(TDC& dc, bool, TRect& RectAPeindre)
//
//  Arguments:   dc 				: device contexte de la fenêtre
//					  RectAPeindre : Rectangle à repeindre
//
//  Description: Peint/repeint tout/une partie de la fenêtre
//
//  Returns:	  Rien
// -----------------------------------------------------------------------------

void
NSLdvViewBase::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	showMousePositionMessage() ;

  if (false == _bPainted)
  {
    string sErrorText = string("NSLdvViewBase, dessin initial") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trSubDetails) ;
  }

  _bPainted = true ;

/*
	if (aToons.empty())
		return ;

	int iAncDC = dc.SaveDC() ;

	NS_CLASSLIB::TRect  clientRect = GetClientRect() ;//this->getServiceAreaMargin() ;
	NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;

	// Calcul du "rectangle" à repeindre

	NVLdVRect rectARepeindre(this) ;
	rectARepeindre.initialise(&ptOrigine, &RectAPeindre) ;

	// drawSpecialDates(&dc, &rectARepeindre) ;

	// On demande aux éléments qui ont une partie dans ce rectangle de
	// se repeindre, dans l'ordre de leur zOrder
	//
	// Asking each and every element touching that rectangle to redraw,
	// following their zOrder

	int  iLevel      = 0 ;
	int  iFuturLevel = 10000 ;
	bool tourner     = true ;
	while (tourner)
	{
		for (ToonsIter i = aToons.begin(); i != aToons.end(); i++)
		{
			if ((*i)->iZOrder == iLevel)
			{
      	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*i, NSLdvTimeToon) ;

				if (pTimeToon && pTimeToon->bVisible && pTimeToon->Box.Touches(rectARepeindre))
        {
        	if ((_bDisplayDrugs || ((*i)->toonType != NSLdvToon::toonDrug)) &&
              (_bDisplayGoals || ((*i)->toonType != NSLdvToon::toonGoal)) &&
              ((-1 == _iMinSeverityOfDisplayedConcerns) || ((*i)->toonType != NSLdvToon::toonConcern) || ((NULL != dynamic_cast<NSConcernView *>(*i)) && (NULL != (dynamic_cast<NSConcernView *>(*i))->pConcern) && ((dynamic_cast<NSConcernView *>(*i))->pConcern->getMaxSeverityIndex() > _iMinSeverityOfDisplayedConcerns)))
             )
						(*i)->draw(&dc, &rectARepeindre) ;
        }
        else
        {
        	NSLdvWindowToon* pWinToon = TYPESAFE_DOWNCAST(*i, NSLdvWindowToon) ;
					// if (pWinToon && pWinToon->bVisible && (pWinToon->Touches(RectAPeindre)))
					// 	(*i)->draw(&dc, &rectARepeindre) ;
        }
			}
			else
			{
				if (((*i)->iZOrder > iLevel) && ((*i)->iZOrder < iFuturLevel))
					iFuturLevel = (*i)->iZOrder;
			}
		}
		if (iFuturLevel == 10000)
			tourner = false;
		else
		{
			iLevel = iFuturLevel;
			iFuturLevel = 10000;
		}
	}

	// Affichage des sous-éléments de contacts en attente
	pLdvTankView->affiche(&dc, RectAPeindre) ;

	dc.RestoreDC(iAncDC);
*/
}

/*
void
NSLdvViewBase::calculRectangleGlobal()
{
	NVLdVTemps  tDateMin, tDateMax;
	long        lHauMin, lHaumax;

	NVLdVTemps tDebut;
	tDebut.takeTime();

	NVLdVTemps tFin;
	tFin.takeTime();

	long lPlancher = 0;
	long lPlafond  = 0;
	lHauMin  = 0;
	lHaumax  = 0;

	if (!(aToons.empty()))
	{
		ToonsIter i = aToons.begin();
		for (; i != aToons.end(); i++)
		{
    	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*i, NSLdvTimeToon) ;
      if (pTimeToon)
				pTimeToon->donneRectangle(&tDateMin, &tDateMax, &lHauMin, &lHaumax) ;

			if (tDateMin < tDebut)
				tDebut = tDateMin ;
			if (tDateMax > tFin)
				tFin = tDateMax ;

			if (lHauMin < lPlancher)
				lPlancher = lHauMin ;
			if (lHaumax > lPlafond)
				lPlafond = lHaumax ;
		}
	}
	GlobalBox.initialise(tDebut, tFin, lPlafond, lPlancher) ;
}
*/

void
NSLdvViewBase::EvaluateWinToonsPos(TWindow* pWindow, ArrayOfToons* pToons)
{
  if ((NULL == pWindow) || (NULL == pToons) || (pToons->empty()))
    return ;

try
{
  NS_CLASSLIB::TRect  clientRect = pWindow->GetClientRect() ;
	NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;

  WinToonsVector aWinToons ;

  // First step : unset them all
  //
  int iUnsetToonsNb = 0 ;
  for (ToonsIter i = pToons->begin() ; pToons->end() != i ; i++)
  {
    NSLdvWindowToon* pWinToon = TYPESAFE_DOWNCAST(*i, NSLdvWindowToon) ;
    if (pWinToon)
    {
      aWinToons.push_back(pWinToon) ;
      pWinToon->resetPos() ;
      iUnsetToonsNb++ ;
    }
  }

  if (aWinToons.empty())
    return ;

  // All the basic coordinates are based on LdvView, the coordinates inside
  // parent are estimated afterward
  //
  bool bNothingNew = false ;

  while (false == bNothingNew)
	{
    bNothingNew = true ;

		for (WinToonsIterator i = aWinToons.begin() ; aWinToons.end() != i ; i++)
		{
      if ((false == (*i)->isPosValid()) && (string("") != (*i)->_sSkinName))
      {
        nsBoxPosition* pBoxPos = 0 ;

        nsSkin* pSkin = pContexte->getSkins()->donneSkin((*i)->_sSkinName) ;
        if (pSkin)
          pBoxPos = pSkin->getBoxPosition() ;

        if (pBoxPos)
        {
          // Evaluate right and left (don't forget that: |<- +Left- -Right+ -> |)
          //
          if (false == (*i)->isLeftSet())
          {
            //
            //
            if ((string("") != pBoxPos->getLeftUnit()) && (string(FOCUS_AUTO) != pBoxPos->getLeftUnit()))
            {
              int iRefPosit = 0 ;
              if (getRelativeValue(&iRefPosit, pWindow, *i, pBoxPos->getLeftPosRef(), string(VIEW_LEFT)))
              {
                (*i)->setLeftValue(iRefPosit + pBoxPos->getLeftValue()) ;
                (*i)->setLeftSet(true) ;
                bNothingNew = false ;

                (*i)->calculateWidthValue() ;
              }
            }
          }
          if (false == (*i)->isRightSet())
          {
            //
            //
            if ((string("") != pBoxPos->getRightUnit()) && (string(FOCUS_AUTO) != pBoxPos->getRightUnit()))
            {
              int iRefPosit = 0 ;
              if (getRelativeValue(&iRefPosit, pWindow, *i, pBoxPos->getRightPosRef(), string(VIEW_RIGHT)))
              {
                (*i)->setRightValue(iRefPosit - pBoxPos->getRightValue()) ;
                (*i)->setRightSet(true) ;
                bNothingNew = false ;

                (*i)->calculateWidthValue() ;
              }
            }
          }
          //
          // Try again, using specs on Width
          //
          if (false == (*i)->isLeftSet())
          {
            (*i)->calculateLeftValue() ;

            if (true == (*i)->isLeftSet())
              bNothingNew = false ;
          }
          if (false == (*i)->isRightSet())
          {
            (*i)->calculateRightValue() ;

            if (true == (*i)->isRightSet())
              bNothingNew = false ;
          }
          //                                             ___            -
          // Evaluate top and bottom  don't forget that:  ^  +     | Bottom
          //                                              | Top    V    +
          //                                                 -    ___
          if (false == (*i)->isBottomSet())
          {
            //
            //
            if ((string("") != pBoxPos->getBottomUnit()) && (string(FOCUS_AUTO) != pBoxPos->getBottomUnit()))
            {
              int iRefPosit = 0 ;
              if (getRelativeValue(&iRefPosit, pWindow, *i, pBoxPos->getBottomPosRef(), string(VIEW_BOTTOM)))
              {
                (*i)->setBottomValue(iRefPosit + pBoxPos->getBottomValue()) ;
                (*i)->setBottomSet(true) ;
                bNothingNew = false ;

                (*i)->calculateHeightValue() ;
              }
            }
          }
          if (false == (*i)->isTopSet())
          {
            //
            //
            if ((string("") != pBoxPos->getTopUnit()) && (string(FOCUS_AUTO) != pBoxPos->getTopUnit()))
            {
              int iRefPosit = 0 ;
              if (getRelativeValue(&iRefPosit, pWindow, *i, pBoxPos->getTopPosRef(), string(VIEW_TOP)))
              {
                (*i)->setTopValue(iRefPosit - pBoxPos->getTopValue()) ;
                (*i)->setTopSet(true) ;
                bNothingNew = false ;

                (*i)->calculateHeightValue() ;
              }
            }
          }
          //
          // Try again, using specs on Height
          //
          if (false == (*i)->isTopSet())
          {
            (*i)->calculateTopValue() ;

            if (true == (*i)->isTopSet())
              bNothingNew = false ;
          }
          if (false == (*i)->isBottomSet())
          {
            (*i)->calculateBottomValue() ;

            if (true == (*i)->isBottomSet())
              bNothingNew = false ;
          }
        }
      }
		}
	}

  // Assign their relative position to windows (position inside parent)
  //
  for (WinToonsIterator i = aWinToons.begin() ; aWinToons.end() != i ; i++)
    (*i)->MoveWindow(true) ;

  // Iterate
  //
  for (WinToonsIterator i = aWinToons.begin() ; aWinToons.end() != i ; i++)
    (*i)->EvaluateWinToonsPos() ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::EvaluateWinToonsPos.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

bool
NSLdvViewBase::getRelativeValue(int* piNewValue, TWindow* pRefWindow, NSLdvWindowToon* pToon, string sReference, string sDirection)
{
  if ((NULL == piNewValue) || (NULL == pToon))
    return false ;

  string sRefName      = string("") ;
  string sRefDirection = string("") ;

  getReferenceAndDirection(sReference, sDirection, &sRefName, &sRefDirection) ;

  if ((string("") == sRefName) || (string("") == sRefDirection))
    return false ;

  // Reference is the LdvView
  //
  if ((string(REF_WINDOW) == sRefName) ||
      ((string(REF_PARENT) == sRefName) && (NULL == pToon->getToonParent())))
  {
    if ((VIEW_LEFT == sRefDirection) || (VIEW_BOTTOM == sRefDirection))
    {
      *piNewValue = 0 ;
      return true ;
    }

    if ((NULL == pRefWindow) || (false == pRefWindow->IsWindow()))
      return false ;

    NS_CLASSLIB::TRect wRect = pRefWindow->GetClientRect() ;

    if (VIEW_RIGHT == sRefDirection)
    {
      *piNewValue = wRect.Width() ;
      return true ;
    }
    if (VIEW_TOP == sRefDirection)
    {
      *piNewValue = wRect.Height() ;
      return true ;
    }
    if (VIEW_HMIDDLE == sRefDirection)
    {
      *piNewValue = wRect.Width() / 2 ;
      return true ;
    }
    if (VIEW_VMIDDLE == sRefDirection)
    {
      *piNewValue = wRect.Height() / 2 ;
      return true ;
    }

    return false ;
  }

  // Reference is the parent
  //
  if (string(REF_PARENT) == sRefName)
  {
    NSLdvWindowToon* pParent = pToon->getToonParent() ;

    if (VIEW_LEFT == sRefDirection)
    {
      if (pParent->isLeftSet())
      {
        *piNewValue = pParent->getLeftValue() ;
        return true ;
      }
      return false ;
    }
    if (VIEW_BOTTOM == sRefDirection)
    {
      if (pParent->isBottomSet())
      {
        *piNewValue = pParent->getBottomValue() ;
        return true ;
      }
      return false ;
    }
    if (VIEW_RIGHT == sRefDirection)
    {
      if (pParent->isRightSet())
      {
        *piNewValue = pParent->getRightValue() ;
        return true ;
      }
      return false ;
    }
    if (VIEW_TOP == sRefDirection)
    {
      if (pParent->isTopSet())
      {
        *piNewValue = pParent->getTopValue() ;
        return true ;
      }
      return false ;
    }
    if (VIEW_HMIDDLE == sRefDirection)
    {
      if (pParent->isRightSet() && pParent->isLeftSet())
      {
        *piNewValue = (pParent->getRightValue() - pParent->getLeftValue()) / 2 ;
        return true ;
      }
      return false ;
    }
    if (VIEW_VMIDDLE == sRefDirection)
    {
      if (pParent->isTopSet() && pParent->isBottomSet())
      {
        *piNewValue = (pParent->getTopValue() - pParent->getBottomValue()) / 2 ;
        return true ;
      }
      return false ;
    }
  }

  return false ;
}

void
NSLdvViewBase::getReferenceAndDirection(string sReferenceString, string sNeededDirection, string* psReference, string* psDirection)
{
  if ((NULL == psReference) && (NULL == psDirection))
    return ;

  // Parent is the default reference
  //
  string sRefName      = string(REF_PARENT) ;
  //
  // The provided direction is the default direction
  //
  string sRefDirection = sNeededDirection ;

  if (string("") != sReferenceString)
  {
    size_t iPos = sReferenceString.find(':') ;
    if (iPos != NPOS)
    {
      // Only direction is specified
      //
      if (0 == iPos)
        sRefDirection = string(sReferenceString, 1, strlen(sReferenceString.c_str()) - 1) ;
      //
      // Only reference window is specified
      //
      else if (strlen(sReferenceString.c_str()) - 1 == iPos)
        sRefName = string(sReferenceString, 0, iPos) ;
      //
      // Both are defined
      //
      else
      {
        sRefName      = string(sReferenceString, 0, iPos) ;
        sRefDirection = string(sReferenceString, iPos+1, strlen(sReferenceString.c_str())-iPos-1) ;
      }
    }
    else
      sRefName = sReferenceString ;
  }
  strip(sRefName, stripBoth) ;
  strip(sRefDirection, stripBoth) ;

  sRefName      = pseumaj(sRefName) ;
  sRefDirection = pseumaj(sRefDirection) ;

  if (psReference)
    *psReference = sRefName ;
  if (psDirection)
    *psDirection = sRefDirection ;

  return ;
}

void
NSLdvViewBase::addToToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon)
{
  if ((NULL == pToonsArray) || (NULL == pToon))
		return ;

	pToonsArray->push_back(pToon) ;
}

bool
NSLdvViewBase::removeFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon)
{
	if ((NULL == pToonsArray) || (NULL == pToon) || pToonsArray->empty())
		return false ;

  ToonsIter tIt = pToonsArray->begin() ;
	for (; (pToonsArray->end() != tIt) && (*tIt != pToon); tIt++) ;

  if (pToonsArray->end() == tIt)
  	return false ;

  pToonsArray->erase(tIt) ;

  return true ;
}

void
NSLdvViewBase::deleteAllFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	if (((ArrayOfToons*) NULL == pToonsArray) || pToonsArray->empty())
		return ;

	for (ToonsIter tIt = pToonsArray->begin() ; pToonsArray->end() != tIt ; )
  {
  	if ((*tIt)->getType() == tType)
    {
    	delete *tIt ;
    	pToonsArray->erase(tIt) ;
    }
    else
    	tIt++ ;
  }
}

NSLdvTimeToon*
NSLdvViewBase::getLdVTimeToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	NSLdvToon* pToon = getLdVToon(pToonsArray, tType) ;

  if ((NSLdvToon*) NULL == pToon)
  	return (NSLdvTimeToon*) 0 ;

	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(pToon, NSLdvTimeToon) ;
  return pTimeToon ;
}

NSLdvWindowToon*
NSLdvViewBase::getLdVWinToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	NSLdvToon* pToon = getLdVToon(pToonsArray, tType) ;
  if ((NSLdvToon*) NULL == pToon)
  	return (NSLdvWindowToon*) 0 ;

	NSLdvWindowToon* pTimeToon = TYPESAFE_DOWNCAST(pToon, NSLdvWindowToon) ;
  return pTimeToon ;
}

NSLdvToon*
NSLdvViewBase::getLdVToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	if (((ArrayOfToons*) NULL == pToonsArray) || pToonsArray->empty())
		return (NSLdvToon*) 0 ;

	for (ToonsIter it = pToonsArray->begin(); pToonsArray->end() != it ; it++)
  	if ((*it)->_toonType == tType)
    	return *it ;

	return (NSLdvToon*) 0 ;
}

NSLdvChronoLine*
NSLdvViewBase::getLdVChronoLine()
{
	NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonChronoLine) ;
  if ((NSLdvWindowToon*) NULL == pWinToon)
  	return (NSLdvChronoLine*) 0 ;

	NSLdvChronoLine* pChronoLine = TYPESAFE_DOWNCAST(pWinToon, NSLdvChronoLine) ;
  return pChronoLine ;
}

NSLdvCommandPannel*
NSLdvViewBase::getLdVCommandPannel()
{
	NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonPannel) ;
  if ((NSLdvWindowToon*) NULL == pWinToon)
  	return (NSLdvCommandPannel*) 0 ;

	NSLdvCommandPannel* pCmdPannel = TYPESAFE_DOWNCAST(pWinToon, NSLdvCommandPannel) ;
  return pCmdPannel ;
}

NSLdvViewArea*
NSLdvViewBase::getActiveWorkingArea()
{
  NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonWorkingArea) ;
  if ((NSLdvWindowToon*) NULL == pWinToon)
  	return (NSLdvViewArea*) 0 ;

	NSLdvViewArea* pWorkingArea = TYPESAFE_DOWNCAST(pWinToon, NSLdvViewArea) ;
  return pWorkingArea ;
}

void
NSLdvViewBase::afficheStatusMessage(string& sMessage, bool bScroll)
{
  string sLangue = string("") ;
  string sTitle  = string("") ;

try
{
	sLangue = pContexte->getUserLanguage() ;
	sTitle  = pContexte->getPatient()->donneTitre(sLangue) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::afficheStatusMessage, getting patient's title.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	NVLdVTemps tNaissance = getDateNaissance() ;
	if (false == tNaissance.estVide())
	{
  	// Get the lib of "years" (plural of "year")
    //
  	string sCodeLexAn = string("2DAT33") ;
    NSPathologData Data ;
    pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexAn, &Data) ;
    GENRE iGenre ;
    Data.donneGenre(&iGenre) ;
    Data.donneGenrePluriel(&iGenre) ;
    string sLibel ;
    NSGenerateur* pGene = pContexte->getDico()->getGenerateur() ;
    pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

    string sCodeLexMois = string("2DAT21") ;
    pContexte->getDico()->trouvePathologData(sLangue, &sCodeLexMois, &Data) ;
    Data.donneGenre(&iGenre) ;
    Data.donneGenrePluriel(&iGenre) ;
    string sLibelMois ;
    pGene->donneLibelleAffiche(&sLibelMois, &Data, iGenre) ;

		string sCurrentDate = donne_date_duJour() ;
		int    iCurrentAge  = donne_age(sCurrentDate, tNaissance.donneDate()) ;

		if (iCurrentAge >= 2)
			sTitle += string(" (") + IntToString(iCurrentAge) + string(" ") + sLibel + string(")") ;
		else if (iCurrentAge >= 0)
		{
    	iCurrentAge = donne_age_mois(sCurrentDate, tNaissance.donneDate()) ;
      sTitle += string(" (") + IntToString(iCurrentAge) + string(" ") + sLibelMois + string(")") ;
    }

    if (string("") != sMessage)
    {
    	string sCurDate = string(sMessage, 6, 4) + string(sMessage, 3, 2) + string(sMessage, 0, 2) ;
      int iAge = donne_age(sCurDate, tNaissance.donneDate()) ;

      if (iAge >= 2)
      {
        string sAge = string("?") ;
      	if (iAge >= 0)
          sAge = IntToString(iAge) ;
      	sMessage = sMessage + string("  ->  ") + sAge + string(" ") + sLibel ;
      }
      else
      {
      	iAge = donne_age_mois(sCurDate, tNaissance.donneDate()) ;
        sMessage = sMessage + string("  ->  ") + IntToString(iAge) + string(" ") + sLibelMois ;
      }
    }
	}
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::afficheStatusMessage, getting age.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	sMessage += string(" - ") + sTitle ;

	TWindowDC* pWinDC = new TWindowDC(HWindow) ; // HWnd

	NS_CLASSLIB::TRect rectDTArea = getDateTimeArea() ;
	pWinDC->FillRect(rectDTArea, NS_CLASSLIB::TColor::White) ;

	NS_CLASSLIB::TColor  textColor ;

	if (bScroll)
		textColor = NS_CLASSLIB::TColor::LtBlue ;
  else
		textColor = NS_CLASSLIB::TColor::Black ;

	pWinDC->SetTextColor(textColor) ;
	pWinDC->DrawText(sMessage.c_str(), -1, rectDTArea, DT_CENTER) ;

	delete pWinDC ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::afficheStatusMessage, drawing.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	pContexte->getSuperviseur()->afficheStatusMessage((char*)(sMessage.c_str())) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewBase::afficheStatusMessage, displaying status message.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

NS_CLASSLIB::TPoint
NSLdvViewBase::getGlobalPhysicalPoint(NVLdVPoint ldvPoint)
{
	// On place le point en bas à droite
	// Instantiating the point as clientRect bottom right point
  //
	NS_CLASSLIB::TRect  clientRect    = GetClientRect() ;
	NS_CLASSLIB::TPoint physicalPoint = clientRect.BottomRight() ;

	// Translation du point
	// Translating the point
	NVLdVRect ldvRect(this,
										ldvPoint.getX(),
										_tDateHeureInfDroit,
										0, // lPosInfDroit,
										ldvPoint.getY()) ;

  // physicalPoint.x -= ldvRect.Width() / iPixelPerUnitRate ;

  physicalPoint.x -= getPhysicalWidthFromTimeUnit(ldvRect.Width()) ;

  if (physicalPoint.x > numeric_limits<int>::max())
  	physicalPoint.x = numeric_limits<int>::max() ;
  if (physicalPoint.x < - numeric_limits<int>::max())
  	physicalPoint.x = - numeric_limits<int>::max() ;

	physicalPoint.y += ldvRect.Height() ;

  if (physicalPoint.y > numeric_limits<int>::max())
  	physicalPoint.y = numeric_limits<int>::max() ;
  if (physicalPoint.y < - numeric_limits<int>::max())
  	physicalPoint.y = - numeric_limits<int>::max() ;

	return physicalPoint ;
}

NS_CLASSLIB::TRect
NSLdvViewBase::getGlobalPhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getGlobalPhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getGlobalPhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

NS_CLASSLIB::TRect
NSLdvViewBase::getGlobalPhysicalRect(NS_CLASSLIB::TRect ldvWinRect)
{
  NS_CLASSLIB::TRect  clientRect    = GetClientRect() ;
	NS_CLASSLIB::TPoint physicalPoint = clientRect.BottomRight() ;

  NS_CLASSLIB::TPoint ptTopLeft = ldvWinRect.TopLeft() ;
  ptTopLeft.y = clientRect.Height() - ptTopLeft.y ;
  if (ptTopLeft.y > numeric_limits<int>::max())
  	ptTopLeft.y = numeric_limits<int>::max() ;
  if (ptTopLeft.y < - numeric_limits<int>::max())
  	ptTopLeft.y = - numeric_limits<int>::max() ;

	NS_CLASSLIB::TPoint ptBotRigh = ldvWinRect.BottomRight() ;
  ptBotRigh.y = clientRect.Height() - ptBotRigh.y ;
  if (ptBotRigh.y > numeric_limits<int>::max())
  	ptBotRigh.y = numeric_limits<int>::max() ;
  if (ptBotRigh.y < - numeric_limits<int>::max())
  	ptBotRigh.y = - numeric_limits<int>::max() ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

/*
NS_CLASSLIB::TRect
NSLdvViewBase::getScrollablePhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getScrollablePhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getScrollablePhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}
*/

bool
NSLdvViewBase::convertUnit(double* pdValue, string sResultUnit, string sInitialUnit, string sRelatedTo, bool bVerbose)
{
	if ((double*) NULL == pdValue)
		return false ;

	if (sResultUnit == sInitialUnit)
		return true ;

	NSCV NsCv(pContexte->getSuperviseur()) ;

  DBIResult result = NsCv.open() ;
	if (result != DBIERR_NONE)
  {
  	if (bVerbose)
    {
    	string sErrorMessage = pContexte->getSuperviseur()->getText("unitConversion", "errorOpeningConversionDatabase") ;
      erreur(sErrorMessage.c_str(), standardError, result) ;
    }
    return false ;
  }

	bool bCvtSuccess = NsCv.ConvertirUnite(pdValue, sResultUnit, sInitialUnit, sRelatedTo) ;

  NsCv.close() ;

  return bCvtSuccess ;
}

int
NSLdvViewBase::getLargeurChronoLine()
{
  NSLdvChronoLine*   pChrono = getLdVChronoLine() ;
  NS_CLASSLIB::TRect vBox    = pChrono->donneVisibleRectangle() ;
  return vBox.Height() ;
}

bool
NSLdvViewBase::addObj(NSLdvObjet *pObj)
{
try
{
	if (((NSLdvObjet*) NULL == pObj) || _aToons.empty())
		return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addObj(pObj) ;
    }
  }

	Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::addObj.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewBase::removeObj(NSLdvObjet *pObj)
{
try
{
	if (((NSLdvObjet*) NULL == pObj) || _aToons.empty())
		return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->removeObj(pObj) ;
    }
  }

	Invalidate(true) ;
	return ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvViewBase::addQualityOfLifeChart(NSQualityOfLifeChart *pChart)
{
try
{
	if (((NSQualityOfLifeChart*) NULL == pChart) || _aToons.empty())
		return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addQualityOfLifeChart(pChart) ;
    }
  }

	Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::addCoopChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewBase::removeQualityOfLifeChart(NSQualityOfLifeChart *pChart)
{
try
{
	if (((NSQualityOfLifeChart *) NULL == pChart) || _aToons.empty())
		return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->removeQualityOfLifeChart(pChart) ;
    }
  }

	Invalidate(true) ;
	return ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvViewBase::addQualityOfLifeChart(NSQualityOfLifeChart *pChart, NSConcern *pPb)
{
try
{
	if (((NSQualityOfLifeChart *) NULL == pChart) || _aToons.empty())
		return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addQualityOfLifeChart(pChart, pPb) ;
    }
  }

	Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::addQualityOfLifeChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewBase::removeQualityOfLifeChart(NSQualityOfLifeChart *pChart, NSConcern *pPb)
{
try
{
	if (((NSQualityOfLifeChart*) NULL == pChart) || _aToons.empty())
		return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->removeQualityOfLifeChart(pChart, pPb) ;
    }
  }

	Invalidate(true) ;
	return ;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvViewBase::addProb(NSConcern *pPb)
{
try
{
  if (((NSConcern*) NULL == pPb) || _aToons.empty())
    return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addProb(pPb) ;
    }
  }

/*
	NSConcernView* pProbView = new NSConcernView(pContexte, this, pPb, getLigneProb(pPb, 1)) ;
	pProbView->boxFitting() ;
	// pProbView->createComponents(this);
	aToons.push_back(pProbView) ;
*/

  reinit() ;

	Invalidate(true) ;
	return true ;
}
catch (...)
{
  erreur("Exception NSLdvViewBase::addProb.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvViewBase::addSsObj(NSLdvSousObjet *pSsObj)
{
try
{
  if (((NSLdvSousObjet*) NULL == pSsObj) || _aToons.empty())
    return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addSsObj(pSsObj) ;
    }
  }

	Invalidate(true) ;
	return true;
}
catch (...)
{
	erreur("Exception NSLdvViewBase::addSsObj.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvViewBase::addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj)
{
  if (((NSConcern *) NULL == pPb) || (NULL == pObj) || _aToons.empty())
		return false ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && (pWorkArea->isMainView()))
        pWorkArea->addObjOnConcern(pPb, pObj) ;
    }
  }

	Invalidate(true) ;

	return true ;
}

void
NSLdvViewBase::reorganizeLines()
{
}

/**
 *  Called when the drugs list has changed
 */
void
NSLdvViewBase::reinitDrugs()
{
  if (_aToons.empty())
		return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonWorkingArea == (*toonIter)->getType())
    {
      NSLdvViewArea *pWorkArea = TYPESAFE_DOWNCAST(*toonIter, NSLdvViewArea) ;
      if (pWorkArea && pWorkArea->isMainView())
        pWorkArea->getToonsArray()->reinitDrugs(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;
    }
  }
}

void
NSLdvViewBase::displayCurvesForBiometricalGoals(string sReference)
{
	if (string("") == sReference)
		return ;

	// Looking for the NSLdvCurvesManagementPannel toon
  //
  if (_aToons.empty())
		return ;

	NSLdvCurvesManagementPannel* pCurvesPanel = (NSLdvCurvesManagementPannel*) 0 ;
	for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  	if ((*toonIter)->_toonType == NSLdvToon::toonCurvePannel)
    {
			pCurvesPanel = dynamic_cast<NSLdvCurvesManagementPannel *>(*toonIter) ;
      break ;
    }

	if ((NSLdvCurvesManagementPannel*) NULL == pCurvesPanel)
		return ;

	// Enumerating goals
	//
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = _pLdVDoc->getFrames()->getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayGoals* pArrayOfGoals = pFrameInfo->getGoalsArray() ;
      if (pArrayOfGoals && (false == pArrayOfGoals->empty()))
      {
	      for (ArrayGoalIter goalIt = pArrayOfGoals->begin() ;
                                pArrayOfGoals->end() != goalIt ; goalIt++)
		      if ((*goalIt)->getConcern() == sReference)
          {
    	      if ((string("") != (*goalIt)->getLexique()) && ('V' == (*goalIt)->getLexique()[0]))
            {
      	      pCurvesPanel->autoCreateNewCurve((*goalIt)->getLexique()) ;
              pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
            }
          }
      }
    }
  }
}

void
NSLdvViewBase::setAppointment()
{
	_pLdVDoc->NewAppointmentService(this) ;
}

NSLdvViewBase&
NSLdvViewBase::operator=(const NSLdvViewBase& src)
{
	if (this == &src)
		return *this ;

  _pLdVDoc                 = src._pLdVDoc ;
  _sConcern                = src._sConcern ;

	iXunit                   = src.iXunit ;
	iZoomFactor              = src.iZoomFactor ;
	_bSystemColors           = src._bSystemColors ;
  _bDisplayDrugs				   = src._bDisplayDrugs ;
  _bDisplayGoals 				   = src._bDisplayGoals ;
  skinName                 = src.skinName ;

  _iMinSeverityOfDisplayedConcerns = src._iMinSeverityOfDisplayedConcerns ;

	// GlobalBox                = src.GlobalBox ;

	_tDateHeureInfDroit      = src._tDateHeureInfDroit ;
	//lPosInfDroit             = src.lPosInfDroit ;

	//iLeftServiceAreaMargin   = src.iLeftServiceAreaMargin ;
	//iRightServiceAreaMargin  = src.iRightServiceAreaMargin ;
	//iTopServiceAreaMargin    = src.iTopServiceAreaMargin ;
	//iBottomServiceAreaMargin = src.iBottomServiceAreaMargin ;

	_aToons                  = src._aToons ;

	return *this ;
}

