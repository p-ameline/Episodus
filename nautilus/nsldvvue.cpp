// -----------------------------------------------------------------------------
//                  Ligne de vie - Vue du modèle Document/Vue
// -----------------------------------------------------------------------------

#include <vcl.h>
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <classlib\date.h>

#include "nautilus\nssuper.h"
#include "dcodeur\nsdkd.h"

#include "nsepisod\nsldvuti.h"
#include "nautilus\nautilus.rh"
#include "nautilus\nsadmiwd.h"
#include "nautilus\nscqvue.h"
#include "nautilus\decisionTreeDV.h"
#include "nsbb\nstlibre.h"

#include "nsepisod\nsepidiv.h"

#include "nautilus\nshistdo.h"
#include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"

#include <windows.h>
#include <owl\scrollba.h>

#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsconver.h"
#include "nsbb\nsattvaltools.h"
#include "nsbb\nsfltbtnga.h"
#include "dcodeur\nsgen.h"
#include "nautilus\nsldvvue.rh"
#include "nautilus\nsldvvue.h"
#include "nautilus\nsldvvuetech.h"
#include "nautilus\nsFollowUpView.h"
#include "nsepisod\nsToDo.h"
#include "ns_ob1\BB1KS.h"

#include "nautilus\nsldvgoal.h"
#include "nautilus\nsdocview.h"
#include "nautilus\nsCoopChartDV.h"

#include "nsepisod\nsldvstructs.h"
#include "nsepisod\objectif_viewer.h"

long NSLdvTimeToon::lLocalObjectCount = 0 ;
long NSPbModifView::lObjectCount = 0 ;
long NSLigneView::lObjectCount = 0 ;
long NSConcernView::lObjectCount = 0 ;
long NSPreoccupView::lObjectCount = 0 ;
long NSEventView::lObjectCount = 0 ;
long NSDrugLineView::lObjectCount = 0 ;
long NSGoalLineView::lObjectCount = 0 ;
long NSBaseLineView::lObjectCount = 0 ;
long NSSsObjView::lObjectCount = 0 ;
long NSLdvCurvePoint::lObjectCount = 0 ;
long NSLdvObjetView::lObjectCount = 0 ;
long NSQualityOfLifeChartView::lObjectCount = 0 ;
long NSLdvGoalCycleView::lObjectCount = 0 ;
long NSLdvTankView::lObjectCount = 0 ;
long NSTimeZoomLevel::lObjectCount = 0 ;
long NSLdvView::lObjectCount = 0 ;
long NSLdvViewArea::lObjectCount = 0 ;
long NSLdvViewAreaWindow::lObjectCount = 0 ;

long NSLdvToon::lCoreObjectCount = 0 ;

// -----------------------------------------------------------------------------
// -------------------------- METHODES DE NSLdvView ----------------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Définition de la table de réponse de la vue NSLdvView
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvView, TWindowView)
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

NSLdvView::NSLdvView(NSLdvDocument& doc, TWindow* parent)
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

	// nsMenuIniter menuIter(pContexte) ;
	// OWL::TMenuDescr* menuBar = new OWL::TMenuDescr ;
  // menuIter.initMenuDescr(menuBar, "menubar") ;
  // SetViewMenu(menuBar) ;

  // OWL::TMenuDescr menuBar ;
  // menuIter.initMenuDescr(&menuBar, "menubar") ;
  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  // pMyApp->GetMainWindow()->SetMenuDescr(menuBar) ;
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;

	bSetupToolBar   = true ;

	// Construction de la barre de statut
	pSB = new OWL::TStatusBar(this, TGadget::Recessed) ;
}
catch (...)
{
	erreur("Exception NSLdvView ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Constructeur copie
// NSLdvView::NSLdvView(NSLdvView &)
// -----------------------------------------------------------------------------

NSLdvView::NSLdvView(const NSLdvView& src)
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

// -----------------------------------------------------------------------------
//  SetupWindow() : Initialise la fenêtre
// -----------------------------------------------------------------------------
void
NSLdvView::SetupWindow()
{
try
{
  string sTraceText = string("NSLdvView entering SetupWindow.") ;
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

  sTraceText = string("NSLdvView leaving SetupWindow.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;
}
catch (...)
{
	erreur("Exception NSLdvView SetupWindow.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
//  Destructeur
// -----------------------------------------------------------------------------
NSLdvView::~NSLdvView()
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
  erreur("Exception NSLdvView destructor.", standardError, 0) ;
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
NSLdvView::EvHotKey(int idHotKey)
{
}

void
NSLdvView::reinit()
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
  erreur("Exception NSLdvView reinit.", standardError, 0) ;
}
}

void
NSLdvView::disconnectCurves()
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
NSLdvView::reinitCurves()
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
NSLdvView::PreProcessMsg(MSG &msg)
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
NSLdvView::createWorkingArea(string sConcernRef)
{
  NSLdvViewArea* pWorkingArea = new NSLdvViewArea(pContexte, 0, this, sConcernRef, true) ;
  initWorkingArea(pWorkingArea, true) ;
}

void
NSLdvView::initWorkingArea(NSLdvViewArea* pWorkArea, bool bAtCreation)
{
try
{
  if (NULL == pWorkArea)
    return ;

  string sTraceText = string("NSLdvView entering initWorkingArea.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if (false == bAtCreation)
  {
    sTraceText = string("NSLdvView must reinit WorkingArea.") ;
    pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

    pWorkArea->reinit() ;
  }

  // Ligne de base - base line
  NSBaseLineView* pBaseLine = new NSBaseLineView(pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->push_back(pBaseLine) ;

	// Problèmes
  sTraceText = string("NSLdvView init WorkingArea for health issues.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->init(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

	// Objets et sous objets - objects and components
  sTraceText = string("NSLdvView init WorkingArea for objects and QoLs.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  pWorkArea->getToonsArray()->init(_pLdVDoc->getQoLCharts(), pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->init(_pLdVDoc->getObjets(), pContexte, pWorkArea) ;
	pWorkArea->getToonsArray()->init(_pLdVDoc->getSsObjets(), pContexte, pWorkArea) ;

	// Objectifs de santé
  sTraceText = string("NSLdvView init WorkingArea for health goals.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->initGoals(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

  // Treatments
  sTraceText = string("NSLdvView init WorkingArea for treatments.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

	pWorkArea->getToonsArray()->initTreatments(_pLdVDoc->getFrames(), pContexte, pWorkArea) ;

  sTraceText = string("NSLdvView init WorkingArea, after toons init.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;

  pWorkArea->afterToonsInit() ;

  sTraceText = string("NSLdvView init WorkingArea, leaving.") ;
  pContexte->getSuperviseur()->trace(&sTraceText, 1, NSSuper::trDetails) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView::initWorkingArea.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvView::emptyWorkingAreas()
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
NSLdvView::reinitWorkingAreas()
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
// NVLdVTemps	NSLdvView::getDateNaissance()
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
NVLdVTemps
NSLdvView::getDateNaissance()
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
// NSLdvView::EvHScroll(UINT, UINT, HWND)
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSLdvView::EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
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
	erreur("Exception NSLdvView EvHScroll.", standardError, 0) ;
}
}

LRESULT
NSLdvView::EvMouseWheel(WPARAM wParam, LPARAM lParam)
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
	erreur("Exception NSLdvView EvMouseWheel.", standardError, 0) ;
	return 0 ;
}
}

void
NSLdvView::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvKeyDown(key, repeatCount, flags) ;
}

void
NSLdvView::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
	if (key == VK_F1)
		CmHelp() ;

	TWindowView::EvSysKeyDown(key, repeatCount, flags) ;
}

void
NSLdvView::CmHelp()
{
	pContexte->NavigationAideEnLigne() ;
}

void
NSLdvView::UpdatePixelPerUnitRate(uint)
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
NSLdvView::UpdateSeverityThreshold(uint)
{
	NSLdvCommandPannel* pCommandPannel = getLdVCommandPannel() ;
  if (NULL == pCommandPannel)
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
NSLdvView::CanClose()
{
  if ((pContexte->getPatient()) && (pContexte->getPatient()->_bCanCloseHisto))
    return true ;

  return false ;
}

void
NSLdvView::EvDragDrop()
{
}

//---------------------------------------------------------------------------
//fixer la position de la Ligne de vie
//---------------------------------------------------------------------------
void
NSLdvView::SetWindowPosLdv()
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
NSLdvView::EnregistrePosLdv()
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
NSLdvView::SetWindowPosit()
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
	erreur("Exception NSLdvView SetWindowPosit.", standardError, 0) ;
}
}

void
NSLdvView::RecordWindowPosit()
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
	erreur("Exception NSLdvView RecordWindowPosit.", standardError, 0) ;
}
}

void
NSLdvView::reloadView(string sReason, string sReference)
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
NSLdvView::PerformCreate(int menuOrId)
{
    // pLdVDoc->SetTitle("Ligne de vie Episodus");
}
**************************************/

void
NSLdvView::EvTimer(uint timerId)
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
  if (NULL == pWorkArea)
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
NSLdvView::timeBumper()
{
try
{
  NSLdvViewArea* pWorkArea = getActiveWorkingArea() ;
  if (NULL == pWorkArea)
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
	erreur("Exception NSLdvView::timeBumper.", standardError, 0) ;
}
}

void
NSLdvView::pixelUnitChanged()
{
  pCurrentTimeInfo = getTimeZoomInfo(iXunit) ;
  refreshChronoLine() ;
	timeBumper() ;
	Invalidate() ;
}

void
NSLdvView::showMousePositionMessage()
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
NSLdvView::skinSwitchOn(string sSkinName)
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
NSLdvView::skinSwitchOff(string sSkinName)
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
NSLdvView::CmNewPreoccup()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

	if (IDOK == iExecReturn)
    _pLdVDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
	//	pContexte->getPatient()->pDocLdv->getConcerns()->reinit() ;
}
catch (...)
{
	erreur("Exception CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvView::CmNewRisk()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

  if (IDOK == iExecReturn)
    _pLdVDoc->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
}
catch (...)
{
	erreur("Exception CmNewRisk.", standardError, 0) ;
}
}

void
NSLdvView::CmNewSocialConcern()
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

	NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	int iExecReturn = pNPDialog->Execute() ;

  if (IDOK == iExecReturn)
    _pLdVDoc->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
}
catch (...)
{
	erreur("Exception CmNewSocialConcern.", standardError, 0) ;
}
}

void
NSLdvView::CmChgPreoDate(NSConcern* pConcern, LDVFRAME iFrame)
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
NSLdvView::CmChgPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame)
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
NSLdvView::CmEvolPreoType(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
  if (NULL == pConcern)
		return ;

  string sFatherNode = pConcern->getNoeud() ;

	string sNewNode = string("") ;
  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

  if (ldvframeHealth == iFrame)
  {
	  NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
  }
  else if (ldvframeRisk == iFrame)
  {
    NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
  }
  else if (ldvframeSocial == iFrame)
  {
    NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
  }
  else
    return ;

  ArrayConcern* pConcerns = _pLdVDoc->getConcerns(iFrame) ;
	pConcerns->reinit() ;

	NSConcern* pFatherConcern = pConcerns->getConcern(sFatherNode) ;
  NSConcern* pSonConcern    = pConcerns->getConcern(sNewNode) ;

  _pLdVDoc->creeConcernSuite(iFrame, pFatherConcern, pSonConcern) ;
}
catch (...)
{
	erreur("Exception NSLdvView::CmEvolPreoType.", standardError, 0) ;
}
}

void
NSLdvView::CmAddObjectifs(NSConcern* pConcern, LDVFRAME iFrame)
{
try
{
  if ((NSConcern*) NULL == pConcern)
		return ;

	if (false == pConcern->isActiveConcern())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "inactiveProcedure") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

	NSLdvGoal Objectif(pContexte, _pLdVDoc, iFrame) ;

	ObjectifViewerDlg* objDlg = new ObjectifViewerDlg((TWindow*)this, _pLdVDoc->pContexte, true, &Objectif) ;
	objDlg->Execute() ;
	delete objDlg ;

	NSPatPathoArray PathArray(_pLdVDoc->pContexte->getSuperviseur()) ;
	Objectif.setGoalPatho(&PathArray) ;

  // CreeObjectif(NSPatPathoArray *pPatPathoCree, string sDocument, string sRefId, NSPatPathoArray *pPatPathoMotif, string goal = "") ;
	pContexte->getPatient()->CreeObjectif(&PathArray, "", "", "", NULL, iFrame, true, pConcern->_sReference) ;
}
catch (...)
{
	erreur("Exception NSLdvView CmAddObjectifs.", standardError, 0) ;
}
}

void
NSLdvView::CmProject(NSConcern* pConcern, LDVFRAME iFrame, string sKsName, string sKsArchetype)
{
try
{
  if (((NSConcern*) NULL == pConcern) || (string("") == sKsName))
		return ;

	if (false == pConcern->isActiveConcern())
	{
		string sErrorText = pContexte->getSuperviseur()->getText("goalsManagement", "inactiveProcedure") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return ;
	}

  if (string("") != sKsArchetype)
  {
    NSPredi* pPredi = pContexte->getPredi() ;
    if (pPredi)
    {
      TWindow* pOpenedWindow = pPredi->isDecisionTreeOpen(sKsArchetype) ;
      if (pOpenedWindow)
      {
        NSDecisionTreeView *pDTView = dynamic_cast<NSDecisionTreeView*>(pOpenedWindow) ;
        if (pDTView)
          pDTView->getCreatedMdiChild()->BringWindowToTop() ;
        return ;
      }
    }
  }

	NSToDoTask* ptask = new NSToDoTask() ;
  // ptask->sendBB1BBInterface(new BB1BBInterfaceForKs(0, sKsName, sKsArchetype), pContexte->getSuperviseur(), true, true) ;
  ptask->sendBB1BBDecisionTree(new BB1BBInterfaceForKs(0, sKsName, sKsArchetype), pContexte->getSuperviseur(), true, true) ;
}
catch (...)
{
	erreur("Exception NSLdvView CmProject.", standardError, 0) ;
}
}

void
NSLdvView::CmEvolPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame)
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
	erreur("Exception NSLdvView CmEvolPreoType.", standardError, 0) ;
}
}

void
NSLdvView::CmSuppressPreo(NSConcern* pConcern)
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
	erreur("Exception NSLdvView CmSuppressPreo.", standardError, 0) ;
}
}

void
NSLdvView::CmDrugNew()
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
NSLdvView::CmDrugNewForConcern(NSConcern* pRelatedConcern)
{
	VecteurString aRelatedConcerns ;

	if (pRelatedConcern)
    aRelatedConcerns.AddString(pRelatedConcern->_sReference) ;

	_pLdVDoc->DrugNewService(this, string(""), &aRelatedConcerns) ;
}

void
NSLdvView::CmDrugRenew(NSLdvDrug* pDrug)
{
	if ((NSLdvDrug*) NULL == pDrug)
		return ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (string("") != sNodeToAlter)
  	_pLdVDoc->DrugRenewService(this, sNodeToAlter) ;
}

void
NSLdvView::CmDrugModify(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	string sNodeToAlter = pDrug->getNoeud() ;
	if (string("") != sNodeToAlter)
		_pLdVDoc->DrugModifyService(this, sNodeToAlter) ;
}

void
NSLdvView::CmDrugChangePoso(NSLdvDrug* pDrug)
{
	if ((NSLdvDrug*) NULL == pDrug)
		return ;

	_pLdVDoc->DrugChangePosoService(this, pDrug) ;
}

void
NSLdvView::CmDrugStop(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	// string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToStopThisPrescription") ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (sNodeToAlter != string(""))
  	_pLdVDoc->DrugStopService(this, sNodeToAlter) ;
}

void
NSLdvView::CmDrugDelete(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

	// string sWarningMsg = pContexte->getSuperviseur()->getText("drugManagementWarnings", "doYouReallyWantToDeleteThisPrescription") ;

	string sNodeToAlter = pDrug->getNoeud() ;
  if (sNodeToAlter != string(""))
		_pLdVDoc->DrugDeleteService(this, sNodeToAlter) ;
}

void
NSLdvView::CmDrugPrescription()
{
	_pLdVDoc->DrugCreatePrescriptionService(this) ;
}

void
NSLdvView::CmManageRights(NSConcern* pConcern)
{
  if ((NSConcern*) NULL == pConcern)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->manageRights(pFrameInfo->getFrame(), this, pConcern->getNoeud()) ;
}

void
NSLdvView::CmManageRights(NSLdvDrug* pDrug)
{
  if ((NSLdvDrug*) NULL == pDrug)
		return ;

  NSFrameInformation* pFrameInfo = _pLdVDoc->getFrameForNode(pDrug->getNoeud()) ;
  if (pFrameInfo)
    _pLdVDoc->manageRights(pFrameInfo->getFrame(), this, pDrug->getNoeud()) ;
}

void
NSLdvView::CmNewDrugsFromRefForConcern(NSConcern* pConcern)
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
NSLdvView::CmNewGoalsFromRefForConcern(NSConcern* pConcern)
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
NSLdvView::CmChgPreoccup()
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
NSLdvView::CmChgPreoType(NSConcern* pConcern, LDVFRAME iFrame)
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
	erreur("Exception NSLdvView CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvView::CmEvolPreoccup(LDVFRAME iFrame)
{
try
{
	string sNewNode = string("") ;

  NewConcernInfo concernInfo(pContexte->getSuperviseur()) ;

  if (ldvframeHealth == iFrame)
  {
	  NSNewConcernDlg* pNPDialog = new NSNewConcernDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeHealth, sNewNode, &concernInfo) ;
  }
  else if (ldvframeRisk == iFrame)
  {
    NSNewRiskDlg* pNPDialog = new NSNewRiskDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeRisk, sNewNode, &concernInfo) ;
  }
  else if (ldvframeSocial == iFrame)
  {
    NSNewSocialDlg* pNPDialog = new NSNewSocialDlg(this, pContexte, &concernInfo) ;
	  int iExecReturn = pNPDialog->Execute() ;
	  if (IDOK != iExecReturn)
		  return ;

    _pLdVDoc->newLine(ldvframeSocial, sNewNode, &concernInfo) ;
  }
  else
    return ;

  ArrayConcern* pConcerns = _pLdVDoc->getConcerns(iFrame) ;
  if (pConcerns)
    pConcerns->reinit() ;
}
catch (...)
{
	erreur("Exception CmNewPreoccup.", standardError, 0) ;
}
}

void
NSLdvView::CmChange2Cursor()
{
	iMouseMode = modeCursor ;
}

void
NSLdvView::CmCursorEnable(TCommandEnabler &ce)
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
NSLdvView::CmChange2Zoom()
{
	iMouseMode = modeZoom ;
}

void
NSLdvView::CmZoomEnable(TCommandEnabler &ce)
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
NSLdvView::CmLdvZoomIn()
{
try
{
  NSLdvViewArea* pActiveWorkingArea = getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;
  TWindow* pActiveWorkingWindow = pActiveWorkingArea->getInterface() ;
  if (NULL == pActiveWorkingWindow)
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
	erreur("Exception NSLdvView CmLdvZoomIn.", standardError, 0) ;
}
}

void
NSLdvView::CmLdvZoomOut()
{
try
{
  NSLdvViewArea* pActiveWorkingArea = getActiveWorkingArea() ;
  if (NULL == pActiveWorkingArea)
    return ;
  TWindow* pActiveWorkingWindow = pActiveWorkingArea->getInterface() ;
  if (NULL == pActiveWorkingWindow)
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
	erreur("Exception NSLdvView CmLdvZoomOut.", standardError, 0) ;
}
}

void
NSLdvView::CmShowGoalsForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if (NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if (NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if (NULL == pCurrentConcern)
    return ;

  pContexte->getPatient()->goals_show(pCurrentConcern->getNoeud()) ;
}

void
NSLdvView::CmShowDrugsForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if (NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if (NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if (NULL == pCurrentConcern)
    return ;

  pContexte->getPatient()->drugs_show(pCurrentConcern->getNoeud()) ;
}

void
NSLdvView::CmShowFollowUpForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if (NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if (NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if (NULL == pCurrentConcern)
    return ;

  pContexte->getPatient()->followUp_show(pCurrentConcern->getNoeud()) ;
}

void
NSLdvView::CmShowCurvesForConcern()
{
  NSLdvViewArea* pWorkingArea = getActiveWorkingArea() ;
  if (NULL == pWorkingArea)
    return ;

  NSConcernView* pCurrentConcernView = pWorkingArea->getCurrentConcern() ;
  if (NULL == pCurrentConcernView)
    return ;

  NSConcern* pCurrentConcern = pCurrentConcernView->getConcern() ;
  if (NULL == pCurrentConcern)
    return ;

  displayCurvesForBiometricalGoals(pCurrentConcern->getNoeud()) ;
}

void
NSLdvView::CmLdvPixSecond()
{
	iXunit = pixSeconde ;
  pixelUnitChanged() ;
}

void
NSLdvView::CmLdvPixMinute()
{
	iXunit = pixMinute ;
  pixelUnitChanged() ;
}

void
NSLdvView::CmLdvPixHour()
{
	iXunit = pixHeure ;
  pixelUnitChanged() ;
}

void
NSLdvView::CmLdvPixDay()
{
	iXunit = pixJour ;
  pixelUnitChanged() ;
}

void
NSLdvView::CmLdvPixWeek()
{
	iXunit = pixSemaine ;
  pixelUnitChanged() ;
}

void
NSLdvView::CmLdvPixMonth()
{
	iXunit = pixMois ;
  pixelUnitChanged() ;
}

void
NSLdvView::handlePixelUnitChange()
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
NSLdvView::setZoomLevelInfo()
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
NSLdvView::getLdvPixLexiqueCode()
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
NSLdvView::EvSize(uint sizeType, ClassLib::TSize& size)
{
  EvaluateWinToonsPos(this, &_aToons) ;
	Invalidate(true) ;
}

// -----------------------------------------------------------------------------
// Libération de la dernière barre d'outils créee
// -----------------------------------------------------------------------------

void
NSLdvView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
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
  string sErrorText = string("Exception NSLdvView EvSetFocus.") ;
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
  string sErrorText = string("Exception NSLdvView EvSetFocus (bring window to top).") ;
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
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting toolbar).") ;
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
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting caption).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
  pMyApp->setMenu(string("menubar"), &hAccelerator) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting menu).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	pContexte->setAideIndex(pContexte->getSuperviseur()->getText("help", "helpLDVindex"));
	pContexte->setAideCorps(pContexte->getSuperviseur()->getText("help", "helpLDV")) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView EvSetFocus (setting help).") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvView::SetupToolBar()
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
	erreur("Exception NSLdvView SetupToolBar.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// fermeture du document :
// L'historique est averti de la fermeture par le destructeur du document
// car lorsque la derniere vue se ferme, cela tue le document.
// -----------------------------------------------------------------------------

void
NSLdvView::EvClose()
{
	TWindow::EvClose();
}

void
NSLdvView::EvDestroy()
{
	EnregistrePosLdv() ;
	// if (pContexte->getSuperviseur()->getEpisodus()->bAutoSave)
	//	RecordWindowPosit();
}

bool
NSLdvView::EvQueryEndSession()
{
	return TWindow::EvQueryEndSession() ;
}

void
NSLdvView::setRightTimeFromLeftTime(const NVLdVTemps& tLeftTime)
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
NSLdvView::refreshChronoLine()
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
NSLdvView::getTimeZoomInfo(pixUnit iU)
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
NSLdvView::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	showMousePositionMessage() ;

  if (false == _bPainted)
  {
    string sErrorText = string("NSLdvView, dessin initial") ;
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
NSLdvView::calculRectangleGlobal()
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
NSLdvView::EvaluateWinToonsPos(TWindow* pWindow, ArrayOfToons* pToons)
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
  string sErrorText = string("Exception NSLdvView::EvaluateWinToonsPos.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

bool
NSLdvView::getRelativeValue(int* piNewValue, TWindow* pRefWindow, NSLdvWindowToon* pToon, string sReference, string sDirection)
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
NSLdvView::getReferenceAndDirection(string sReferenceString, string sNeededDirection, string* psReference, string* psDirection)
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
NSLdvView::addToToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon)
{
  if ((NULL == pToonsArray) || (NULL == pToon))
		return ;

	pToonsArray->push_back(pToon) ;
}

bool
NSLdvView::removeFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon)
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
NSLdvView::deleteAllFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	if ((NULL == pToonsArray) || pToonsArray->empty())
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
NSLdvView::getLdVTimeToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	NSLdvToon* pToon = getLdVToon(pToonsArray, tType) ;

  if (NULL == pToon)
  	return (NSLdvTimeToon*) 0 ;

	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(pToon, NSLdvTimeToon) ;
  return pTimeToon ;
}

NSLdvWindowToon*
NSLdvView::getLdVWinToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	NSLdvToon* pToon = getLdVToon(pToonsArray, tType) ;

  if (NULL == pToon)
  	return (NSLdvWindowToon*) 0 ;

	NSLdvWindowToon* pTimeToon = TYPESAFE_DOWNCAST(pToon, NSLdvWindowToon) ;
  return pTimeToon ;
}

NSLdvToon*
NSLdvView::getLdVToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType)
{
	if ((NULL == pToonsArray) || pToonsArray->empty())
		return NULL ;

	for (ToonsIter it = pToonsArray->begin(); pToonsArray->end() != it ; it++)
  	if ((*it)->_toonType == tType)
    	return *it ;

	return NULL ;
}

NSLdvChronoLine*
NSLdvView::getLdVChronoLine()
{
	NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonChronoLine) ;
  if (NULL == pWinToon)
  	return NULL ;

	NSLdvChronoLine* pChronoLine = TYPESAFE_DOWNCAST(pWinToon, NSLdvChronoLine) ;
  return pChronoLine ;
}

NSLdvCommandPannel*
NSLdvView::getLdVCommandPannel()
{
	NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonPannel) ;
  if (NULL == pWinToon)
  	return NULL ;

	NSLdvCommandPannel* pCmdPannel = TYPESAFE_DOWNCAST(pWinToon, NSLdvCommandPannel) ;
  return pCmdPannel ;
}

NSLdvViewArea*
NSLdvView::getActiveWorkingArea()
{
  NSLdvWindowToon* pWinToon = getLdVWinToon(&_aToons, NSLdvToon::toonWorkingArea) ;
  if (NULL == pWinToon)
  	return NULL ;

	NSLdvViewArea* pWorkingArea = TYPESAFE_DOWNCAST(pWinToon, NSLdvViewArea) ;
  return pWorkingArea ;
}

void
NSLdvView::afficheStatusMessage(string& sMessage, bool bScroll)
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
  string sErrorText = string("Exception NSLdvView::afficheStatusMessage, getting patient's title.") ;
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
  string sErrorText = string("Exception NSLdvView::afficheStatusMessage, getting age.") ;
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
  string sErrorText = string("Exception NSLdvView::afficheStatusMessage, drawing.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}

try
{
	pContexte->getSuperviseur()->afficheStatusMessage((char*)(sMessage.c_str())) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvView::afficheStatusMessage, displaying status message.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

NS_CLASSLIB::TPoint
NSLdvView::getGlobalPhysicalPoint(NVLdVPoint ldvPoint)
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
NSLdvView::getGlobalPhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getGlobalPhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getGlobalPhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

NS_CLASSLIB::TRect
NSLdvView::getGlobalPhysicalRect(NS_CLASSLIB::TRect ldvWinRect)
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
NSLdvView::getScrollablePhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getScrollablePhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getScrollablePhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}
*/

bool
NSLdvView::convertUnit(double* pdValue, string sResultUnit, string sInitialUnit, string sRelatedTo, bool bVerbose)
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
NSLdvView::getLargeurChronoLine()
{
  NSLdvChronoLine*   pChrono = getLdVChronoLine() ;
  NS_CLASSLIB::TRect vBox    = pChrono->donneVisibleRectangle() ;
  return vBox.Height() ;
}

bool
NSLdvView::addObj(NSLdvObjet *pObj)
{
try
{
	if ((NULL == pObj) || _aToons.empty())
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
	erreur("Exception NSLdvView::addObj.", standardError, 0) ;
	return false ;
}
}

void
NSLdvView::removeObj(NSLdvObjet *pObj)
{
try
{
	if ((NULL == pObj) || _aToons.empty())
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
	erreur("Exception NSLdvView::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvView::addQualityOfLifeChart(NSQualityOfLifeChart *pChart)
{
try
{
	if ((NULL == pChart) || _aToons.empty())
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
	erreur("Exception NSLdvView::addCoopChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvView::removeQualityOfLifeChart(NSQualityOfLifeChart *pChart)
{
try
{
	if ((NULL == pChart) || _aToons.empty())
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
	erreur("Exception NSLdvView::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvView::addQualityOfLifeChart(NSQualityOfLifeChart *pChart, NSConcern *pPb)
{
try
{
	if ((NULL == pChart) || _aToons.empty())
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
	erreur("Exception NSLdvView::addQualityOfLifeChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvView::removeQualityOfLifeChart(NSQualityOfLifeChart *pChart, NSConcern *pPb)
{
try
{
	if ((NULL == pChart) || _aToons.empty())
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
	erreur("Exception NSLdvView::removeObj.", standardError, 0) ;
	return ;
}
}

bool
NSLdvView::addProb(NSConcern *pPb)
{
try
{
  if ((NULL == pPb) || _aToons.empty())
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
  erreur("Exception NSLdvView::addProb.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvView::addSsObj(NSLdvSousObjet *pSsObj)
{
try
{
  if ((NULL == pSsObj) || _aToons.empty())
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
	erreur("Exception NSLdvView::addSsObj.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvView::addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj)
{
  if ((NULL == pPb) || (NULL == pObj) || _aToons.empty())
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
NSLdvView::reorganizeLines()
{
}

/**
 *  Called when the drugs list has changed
 */
void
NSLdvView::reinitDrugs()
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
NSLdvView::displayCurvesForBiometricalGoals(string sReference)
{
	if (string("") == sReference)
		return ;

	// Looking for the NSLdvCurvesManagementPannel toon
  //
  if (_aToons.empty())
		return ;

	NSLdvCurvesManagementPannel* pCurvesPanel = NULL ;
	for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  	if ((*toonIter)->_toonType == NSLdvToon::toonCurvePannel)
    {
			pCurvesPanel = dynamic_cast<NSLdvCurvesManagementPannel *>(*toonIter) ;
      break ;
    }

	if (NULL == pCurvesPanel)
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
NSLdvView::setAppointment()
{
	_pLdVDoc->NewAppointmentService(this) ;
}

NSLdvView&
NSLdvView::operator=(const NSLdvView& src)
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

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvViewArea ----------------------------
// -----------------------------------------------------------------------------

NSLdvViewArea::NSLdvViewArea(NSContexte* pCtx, NSLdvWindowToon* pLdvParent, NSLdvView* pTheView, string sConcernRef, bool bMain)
              :NSLdvWindowToon(pCtx, pLdvParent, pTheView), _GlobalBox(pTheView)
{
  lObjectCount++ ;

  _sConcernReference = sConcernRef ;
  _bMainView         = bMain ;

  _iZOrder    = LEVEL_COMMANDS ;
  _toonType   = toonWorkingArea ;
  _sSkinName  = string("LdvWorkingArea") ;

  _pInterface = (TWindow*) 0 ;

  reinit() ;
}

NSLdvViewArea::NSLdvViewArea(const NSLdvViewArea& rv)
              :NSLdvWindowToon(rv.pContexte, rv.getToonParent(), rv._pView), _GlobalBox(rv._pView)
{
  lObjectCount++ ;

  _bMainView = rv._bMainView ;
	initialiser((NSLdvWindowToon*)(&rv)) ;
}

NSLdvViewArea::~NSLdvViewArea()
{
  lObjectCount-- ;
}

void
NSLdvViewArea::SetupWindow()
{
}

void
NSLdvViewArea::reinit()
{
try
{
  if (false == _aToons.empty())
    _aToons.vider() ;

  _iMaxConcernIndex	= 0 ;
	_iMaxGoalsIndex 	= 0 ;
	_iMaxDrugsIndex 	= 0 ;

  _iLargeurLigne    = 19 ;
	_iLargeurSubLigne =  5 ;
	_iLargeurBaseline = 20 ;
	_iInterligne      = 15 ;
  _iDrugInterLigne  = 25 ;
  _iBottomMargin    =  5 ;

  initDimensions() ;

  if ((TWindow*) NULL == _pInterface)
    _pInterface = new NSLdvViewAreaWindow(pContexte, getParentInterface(), this) ;

  // Don't forget to reinit the GUI because _iInterligne and _iDrugInterLigne
  // are set there
  //
  if (_pInterface)
  {
    NSLdvViewAreaWindow *pWorkAreaWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
    if (pWorkAreaWin)
      pWorkAreaWin->reinit() ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewArea::reinit.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvViewArea::EvRButtonDown()
{
}

void
NSLdvViewArea::showContextHelp()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    pWorkWin->showContextHelp() ;
}

void
NSLdvViewArea::subSignalForHelp()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    pWorkWin->subSignalForHelp() ;
}

void
NSLdvViewArea::afterToonsInit()
{
  calculRectangleGlobal() ;
  setScrollParams() ;
}

void
NSLdvViewArea::setScrollParams()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    pWorkWin->setScrollParams() ;
}

bool
NSLdvViewArea::addSsObj(NSLdvSousObjet *pSsObj)
{
try
{
  if (NULL == pSsObj)
    return false ;

	NSLdvObjetView *pObjV = _aToons.getObjView(pSsObj->sObject) ;
	NSConcernView  *pPbV  = _aToons.getPbView(pSsObj->sConcern) ;

	NSSsObjView *pSsObjView = new NSSsObjView(pContexte, this, pSsObj, pObjV, pPbV) ;
	_aToons.push_back(pSsObjView) ;

	_pInterface->Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewArea::addSsObj.", standardError, 0) ;
  return false;
}
}

bool
NSLdvViewArea::addObj(NSLdvObjet *pObj)
{
try
{
	if (NULL == pObj)
		return false ;

	NSLigneView *pLineView = _aToons.getLigneView(pObj->getConcern()) ;

	NSLdvObjetView *pObjView = new NSLdvObjetView(pContexte, this, pObj, pLineView) ;
	_aToons.push_back(pObjView) ;

	_pInterface->Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewArea::addObj.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewArea::removeObj(NSLdvObjet *pObj)
{
  if (NULL == pObj)
		return ;

  if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonObject == (*toonIter)->getType())
    {
      NSLdvObjetView *pObjView = TYPESAFE_DOWNCAST(*toonIter, NSLdvObjetView) ;
      if (pObjView && pObjView->getObject() && (pObj->getReference() == pObjView->getObject()->getReference()))
      {
        delete *toonIter ;
        _aToons.erase(toonIter) ;
        break ;
      }
    }
  }

  _pInterface->Invalidate(true) ;
}

bool
NSLdvViewArea::addQualityOfLifeChart(NSQualityOfLifeChart *pChart)
{
try
{
	if (NULL == pChart)
		return false ;

	NSLigneView *pLineView = _aToons.getBaseLine() ;

	NSQualityOfLifeChartView *pChartView = new NSQualityOfLifeChartView(pContexte, this, pChart, pLineView) ;
	_aToons.push_back(pChartView) ;

	_pInterface->Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewArea::addQualityOfLifeChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewArea::removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts)
{
  if (NULL == pCharts)
		return ;

  if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
  {
    if (NSLdvToon::toonCoop == (*toonIter)->getType())
    {
      NSQualityOfLifeChartView *pQoLChartView = TYPESAFE_DOWNCAST(*toonIter, NSQualityOfLifeChartView) ;
      if (pQoLChartView && pQoLChartView->getQualityOfLifeChart() &&
          (pQoLChartView->getQualityOfLifeChart()->getDocId() == pCharts->getDocId()))
      {
        delete *toonIter ;
        _aToons.erase(toonIter) ;
        break ;
      }
    }
  }

  _pInterface->Invalidate(true) ;
}

bool
NSLdvViewArea::addQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb)
{
try
{
	if (NULL == pCharts)
		return false ;

	_pInterface->Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewArea::addQualityOfLifeChart.", standardError, 0) ;
	return false ;
}
}

void
NSLdvViewArea::removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb)
{
  if (NULL == pCharts)
		return ;

  if (_aToons.empty())
    return ;

  _pInterface->Invalidate(true) ;
}

bool
NSLdvViewArea::addProb(NSConcern *pPb)
{
try
{
  NSFrameInformation* pFrameInfo = getView()->getDoc()->getFrameForNode(pPb->getNoeud()) ;
  if (NULL == pFrameInfo)
    return false ;

  LDVFRAME iFrame = pFrameInfo->getFrame() ;

	NSConcernView* pProbView = new NSConcernView(pContexte, this, pPb, iFrame, getLigneProb(pPb, 1)) ;
	pProbView->boxFitting() ;
	// pProbView->createComponents(this);
	_aToons.push_back(pProbView) ;

	_pInterface->Invalidate(true) ;
	return true ;
}
catch (...)
{
	erreur("Exception NSLdvViewArea::addProb.", standardError, 0) ;
  return false ;
}
}

bool
NSLdvViewArea::addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj)
{
	if ((NULL == pObj) || (NULL == pPb) || _aToons.empty())
		return false ;

	NSLdvObjetView* pObjView     = 0 ;
	NSConcernView*  pConcernView = 0 ;
	//
	// Recherche des objets "vue" qui conrrepondent à pPb et pObj
	//
	for (ToonsIter toonIter = _aToons.begin() ; toonIter != _aToons.end() ; toonIter++)
	{
  	NSLdvObjetView *pObjV = TYPESAFE_DOWNCAST(*toonIter, NSLdvObjetView) ;
    if (pObjV)
    {
    	if (pObjV->_pObjet == pObj)
      	pObjView = pObjV ;
    }
    else
    {
    	NSConcernView *pCcnV = TYPESAFE_DOWNCAST(*toonIter, NSConcernView) ;
      if (pCcnV && (pCcnV->getConcern() == pPb))
      	pConcernView = pCcnV ;
    }
    if (pObjView && pConcernView)
    	break ;
	}

	if (!pObjView || !pConcernView)
		return false ;

	pObjView->setLigneView((NSLigneView *) pConcernView) ;
	_pInterface->Invalidate(true) ;

	return true ;
}

// Trouver une ligne qui contient une place libre pour cette préoccupation
// Looking for a line with room for this health concern
int
NSLdvViewArea::getLigneProb(NSConcern* pConcern, int iLineBegin)
{
	if ((NULL == pConcern) || (_aToons.empty()))
		return 0 ;

	// Si cette préoccupation est fille d'une autre, elle est sur la même ligne
	// If this health concern is son of another one, put it on same line
	if (string("") != pConcern->_sPrimoPb)
	{
		NSConcernView* pConcView = _aToons.getPbView(pConcern->_sPrimoPb) ;
		if (pConcView)
			return pConcView->getIndex() ;
	}

	NVLdVTemps tDateDebut = pConcern->_tDateOuverture ;
	NVLdVTemps tDateFin   = pConcern->_tDateFermeture ;

	//
	// Si cette préoccupation n'est pas fille d'une autre, on regarde si elle
	// n'a pas de fille : si elle en a, on doit trouver une ligne capable
	// d'accueillir toutes les filles
	// If this health concern is not the son of another one, we have to check
	// wether it is not itself the father of other health concerns ; if it is,
	// we must find a line with room enough for all sons
	//
  NSLdvDocument* pLdVDoc = getView()->getDoc() ;
  if (NULL == pLdVDoc)
    return 0 ;

  NSConcern*    pSonConcern = 0 ;
  ArrayConcern* pConcerns   = 0 ;
  NSFrameInformation* pFrameInfo = pLdVDoc->getFrameForNode(pConcern->getNoeud()) ;
  if (pFrameInfo)
  {
    pConcerns = pFrameInfo->getConcernsArray() ;
    if (pConcerns)
      pSonConcern = pConcerns->getFils(pConcern) ;
  }
	NSConcern* pLastSon = (NSConcern*) 0 ;

	while (pSonConcern)
	{
		pLastSon    = pSonConcern ;
		pSonConcern = pConcerns->getFils(pLastSon) ;
	}
	if (pLastSon)
		tDateFin = pLastSon->_tDateFermeture ;

	//
	// On cherche une ligne susceptible d'accueillir la préoccupation
	// Looking for a line with room enough for this health concern
	//
	int  iLineResult = iLineBegin ;
	bool bNewLine = true ;
	while (bNewLine)
	{
		bNewLine = false ;
		for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
		{
			NSConcernView *pToon = TYPESAFE_DOWNCAST(*toonIter, NSConcernView) ;
			if (pToon)
			{
				bool    bContinue = true ;

				if ((tDateDebut >= pToon->getConcern()->_tDateOuverture) &&
						(tDateDebut <= pToon->getConcern()->_tDateFermeture))
					bContinue = false ;

				if ((tDateFin >= pToon->getConcern()->_tDateOuverture) &&
						(tDateFin <= pToon->getConcern()->_tDateFermeture))
					bContinue = false ;

				if ((pToon->getConcern()->_tDateOuverture >= tDateDebut) &&
						(pToon->getConcern()->_tDateOuverture <= tDateFin))
					bContinue = false ;

				if ((pToon->getConcern()->_tDateFermeture >= tDateDebut) &&
						(pToon->getConcern()->_tDateFermeture <= tDateFin))
					bContinue = false ;

				if ((false == bContinue) && (pToon->getIndex() == iLineResult))
				{
					iLineResult = pToon->getIndex() + 1 ;
					bNewLine = true ;
				}
			}
		}
	}

	// Si l'élément est le premier d'une série de fils, on attribue cette ligne
	// à tous les descendants
	// If this health concern has sons, we set this line for each son

	if (pLastSon)
	{
		pSonConcern = pConcerns->getFils(pConcern) ;

		while (pSonConcern)
		{
			NSConcernView* pConcView = _aToons.getPbView(pSonConcern->getNoeud()) ;
			if (pConcView)
				pConcView->setIndex(iLineResult) ;

			pSonConcern = pConcerns->getFils(pSonConcern) ;
		}
	}

	return (iLineResult) ;
}

void
NSLdvViewArea::calculRectangleGlobal()
{
	NVLdVTemps tDateMin, tDateMax ;

	NVLdVTemps tDebut ;
	tDebut.takeTime() ;

	NVLdVTemps tFin ;
	tFin.takeTime() ;

	long lPlancher = 0 ;
	long lPlafond  = 0 ;
	long lHauMin   = 0 ;
	long lHaumax   = 0 ;

	if (_aToons.empty())
  {
    _GlobalBox.initialise(tDebut, tFin, lPlafond, lPlancher) ;
    return ;
  }

  for (ToonsIter it = _aToons.begin() ; _aToons.end() != it ; it++)
  {
    NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*it, NSLdvTimeToon) ;
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

	_GlobalBox.initialise(tDebut, tFin, lPlafond, lPlancher) ;
}

void
NSLdvViewArea::reinitCurves()
{
  if (_aToons.empty())
    return ;

  for (ToonsIter toonIter = _aToons.begin() ; _aToons.end() != toonIter ; toonIter++)
	{
    if (toonYAxis == (*toonIter)->getType())
    {
  	  NSLdvCurveYAxis *pYaxis = TYPESAFE_DOWNCAST(*toonIter, NSLdvCurveYAxis) ;
      if (pYaxis && pYaxis->getCurve() && pYaxis->getCurve()->getFirstPoint())
        pYaxis->getCurve()->reinit(false) ;
    }
  }
}

NSConcernView*
NSLdvViewArea::getCurrentConcern()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->_pCurrentConcern ;

  return 0 ;
}

void
NSLdvViewArea::increasePosInfDroit(int iDeltaPos)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->increasePosInfDroit(iDeltaPos) ;

  return ;
}

NS_CLASSLIB::TPoint
NSLdvViewArea::getAreaPhysicalPoint(NVLdVPoint ldvPoint)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getAreaPhysicalPoint(ldvPoint) ;

  return NS_CLASSLIB::TPoint(0, 0) ;
}

NS_CLASSLIB::TRect
NSLdvViewArea::getAreaPhysicalRect(NVLdVRect ldvRect)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getAreaPhysicalRect(ldvRect) ;

  return NS_CLASSLIB::TRect(0, 0, 0, 0) ;
}

NS_CLASSLIB::TRect
NSLdvViewArea::getAreaPhysicalRect(NS_CLASSLIB::TRect ldvWinRect)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getAreaPhysicalRect(ldvWinRect) ;

  return NS_CLASSLIB::TRect(0, 0, 0, 0) ;
}

NS_CLASSLIB::TPoint
NSLdvViewArea::getScrollablePhysicalPoint(NVLdVPoint ldvPoint)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getScrollablePhysicalPoint(ldvPoint) ;

  return NS_CLASSLIB::TPoint(0, 0) ;
}

NS_CLASSLIB::TRect
NSLdvViewArea::getScrollablePhysicalRect(NVLdVRect ldvRect)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getScrollablePhysicalRect(ldvRect) ;

  return NS_CLASSLIB::TRect(0, 0, 0, 0) ;
}

NVLdVPoint
NSLdvViewArea::getAreaLogicalPointFromWindowPoint(NS_CLASSLIB::TPoint scrPoint)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getAreaLogicalPointFromWindowPoint(scrPoint) ;

  return NVLdVPoint(getView()) ;
}

NVLdVRect
NSLdvViewArea::getAreaLogicalRectFromWindowRect(NS_CLASSLIB::TRect scrRect)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getAreaLogicalRectFromWindowRect(scrRect) ;

  return NVLdVRect(getView()) ;
}

NS_CLASSLIB::TRect
NSLdvViewArea::getWorkingSpaceRect()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceRect() ;

  return NS_CLASSLIB::TRect(0, 0, 0, 0) ;
}

int
NSLdvViewArea::getWorkingSpaceHeight()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceHeight() ;

  return 0 ;
}

int
NSLdvViewArea::getWorkingSpaceRight()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceRight() ;

  return 0 ;
}

int
NSLdvViewArea::getWorkingSpaceLeft()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceLeft() ;

  return 0 ;
}

int
NSLdvViewArea::getWorkingSpaceTop()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceTop() ;

  return 0 ;
}

int
NSLdvViewArea::getWorkingSpaceBottom()
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->getWorkingSpaceBottom() ;

  return 0 ;
}

long
NSLdvViewArea::preoccupDonneTopLine(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->preoccupDonneTopLine(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::preoccupDonneBottomLine(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->preoccupDonneBottomLine(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::preoccupDonneTopBox(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->preoccupDonneTopBox(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::preoccupDonneBottomBox(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->preoccupDonneBottomBox(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::drugDonneTopBox(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->drugDonneTopBox(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::drugDonneBottomBox(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->drugDonneBottomBox(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::drugDonneTopLine(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->drugDonneTopLine(iIndex) ;

  return long(0) ;
}

long
NSLdvViewArea::drugDonneBottomLine(int iIndex)
{
  NSLdvViewAreaWindow *pWorkWin = TYPESAFE_DOWNCAST(_pInterface, NSLdvViewAreaWindow) ;
  if (pWorkWin)
    return pWorkWin->drugDonneBottomLine(iIndex) ;

  return long(0) ;
}

NSLdvViewArea&
NSLdvViewArea::operator=(const NSLdvViewArea& src)
{
  if (this == &src)
    return *this ;

  _bMainView = src._bMainView ;
  initialiser((NSLdvWindowToon*)(&src)) ;

  return *this ;
}

// -----------------------------------------------------------------------------
// --------------------- METHODES DE NSLdvViewAreaWindow -----------------------
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Définition de la table de réponse de la vue NSLdvViewAreaWindow
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLdvViewAreaWindow, TWindow)
	EV_WM_VSCROLL,																			// scroll vertival
	EV_WM_MOUSEMOVE,                                    // mouvement de la souris
	EV_WM_LBUTTONDOWN,                                  // bouton gauche enfoncé
	EV_WM_RBUTTONDOWN,                                  // bouton droit enfoncé
	EV_WM_LBUTTONUP,                                    // bouton gauche relaché
	EV_WM_LBUTTONDBLCLK,                                // dbl-clicke avec bouton gauche
	EV_WM_TIMER,                                        // timer
	// EV_WM_HOTKEY,                                       // raccourci (hotkey)
  EV_WM_KEYDOWN,
  EV_WM_SYSKEYDOWN,
	EV_WM_CLOSE,                                        // fermeture de la fenêtre
	EV_WM_DESTROY,                                      // destruction de la fenêtre
	EV_WM_SIZE,                                         // redimensionnement de la fenêtre
	EV_WM_SETFOCUS,                                     // focus
	EV_WM_QUERYENDSESSION,
	EV_COMMAND(IDC_NEWPREOCCUP,     CmNewPreoccup),     // nouvelle préoccup.
	EV_COMMAND(IDC_CHG_PREO_TYP,    CmChgPreoType),     // correction type
	EV_COMMAND(IDC_CHG_PREO_DAT,    CmChgPreoDate),     // correction date
	EV_COMMAND(IDC_CHG_PREO_IDX,    CmChgPreoIndx),     // correction sévérite
	EV_COMMAND(IDC_EVOL_PREO_TYP,   CmEvolPreoType),    // évolution type
	EV_COMMAND(IDC_EVOL_PREO_IDX,   CmEvolPreoIndx),    // évolution sévérite
	EV_COMMAND(IDC_SUPPRESS_PREO,   CmSuppressPreo),    // suppression préoccup
  EV_COMMAND(IDC_NEWRISK,         CmNewRisk),         // new risk
  EV_COMMAND(IDC_NEWSOCIAL,       CmNewSocialConcern),
  EV_COMMAND(CM_DRUG_NEW,         CmDrugNew),
  EV_COMMAND(CM_DRUG_RENEW,       CmDrugRenew),
  EV_COMMAND(CM_DRUG_CHANGE,      CmDrugModify),
  EV_COMMAND(CM_DRUG_MODIF_POSO,  CmDrugChangePoso),
  EV_COMMAND(CM_DRUG_STOP,        CmDrugStop),
  EV_COMMAND(CM_DRUG_DELETE,      CmDrugDelete),
  EV_COMMAND(CM_DRUG_FROM_REF,    CmNewDrugsFromRefForConcern),
  EV_COMMAND(IDC_ORDONNANCE,      CmDrugPrescription),
	EV_COMMAND(IDC_ADD_PREO_OBJ,    CmAddObjectifs),     // ajouter un objectif
  EV_COMMAND(CM_GOAL_FROM_REF,    CmNewGoalsFromRefForConcern),
	EV_COMMAND(IDC_MANAGE_RIGHTS,   CmManageRights),    // gestion des droits
	EV_COMMAND(IDC_EVOL_PREOCCUP,   CmEvolPreoccup),    // command CmNewPreoccup
	EV_COMMAND(CM_LDV_ZOOMIN,       CmLdvZoomIn),       // command CmLdvZoomIn
	EV_COMMAND(CM_LDV_ZOOMOUT,      CmLdvZoomOut),      // command CmLdvZoomOut

  EV_COMMAND(ID_SHOW_GOALS,       CmShowGoalsForConcern),
  EV_COMMAND(ID_SHOW_DRUGS,       CmShowDrugsForConcern),
  EV_COMMAND(ID_SHOW_FOLLOW_UP,   CmShowFollowUpForConcern),
  EV_COMMAND(ID_SHOW_CURVES,      CmShowCurvesForConcern),

  EV_COMMAND(ID_SHOW_PROJECT,     CmPreoProject),
  EV_COMMAND(ID_SHOW_QOFLIFE,     CmPreoQualityOfLife),

	// EV_COMMAND(CM_LDV_PIXSECOND,    CmLdvPixSecond),    // pixel == 1 seconde
	// EV_COMMAND(CM_LDV_PIXMINUTE,    CmLdvPixMinute),    // pixel == 1 minute
	// EV_COMMAND(CM_LDV_PIXHOUR,      CmLdvPixHour),      // pixel == 1 heure
	// EV_COMMAND(CM_LDV_PIXDAY,       CmLdvPixDay),       // pixel == 1 jour
	// EV_COMMAND(CM_LDV_PIXWEEK,      CmLdvPixWeek),      // pixel == 1 semaine
	// EV_COMMAND(CM_LDV_PIXMONTH,     CmLdvPixMonth),     // pixel == 1 mois
	// EV_COMMAND_ENABLE(IDI_LDVCURS16,  CmCursorEnable),  // passage en mode Curseur
	// EV_COMMAND_ENABLE(IDI_LDVZOOM16,  CmZoomEnable),    // passage en mode Zoom
	// EV_COMMAND(CM_TIPTIMER_ON,      setTipsTimer),
	// EV_COMMAND(CM_TIPTIMER_OFF,     killTipsTimer),
  // EV_COMMAND(CM_APPOINTMENT,      setAppointment),
  // EV_COMMAND(CM_HELP,             CmHelp),
  // EV_CHILD_NOTIFY_ALL_CODES(ID_PIXEL_PU, UpdatePixelPerUnitRate),
  // EV_CHILD_NOTIFY_ALL_CODES(ID_SEVERITY, UpdateSeverityThreshold),
END_RESPONSE_TABLE;

// -----------------------------------------------------------------------------
//  SetupWindow() : Initialise la fenêtre
// -----------------------------------------------------------------------------

NSLdvViewAreaWindow::NSLdvViewAreaWindow(NSContexte* pCtx, TWindow* parent, NSLdvViewArea* pToon)
                    :TWindow(parent), NSRoot(pCtx), _currentPoint(pToon->getView())
{
  lObjectCount++ ;

  Attr.Style = Attr.Style | WS_VSCROLL ;
  Attr.AccelTable = LDV_ACCEL ;

  _pToon = pToon ;

  SetCaption(_pToon->_sSkinName.c_str()) ;

  _lPosInfDroit = 0 ;

  _bLButtonDown = false ;
	_pToon2Drag   = NULL ;

	_pToolTip     = new NSTitleTip(this, pContexte) ;
  _pMultiTips   = (NSLdvViewIconsShowWindow *) 0 ;
  _pMagnifier   = (NSLdvViewMagnifier*) 0 ;
  _bMustCloseMultitips = false ;
  _bMustCloseMagnifier = false ;
  _bShowTooltipsBeforeMultitips = false ;

  reinit() ;
}

NSLdvViewAreaWindow::~NSLdvViewAreaWindow()
{
  lObjectCount-- ;

  delete _pToolTip ;
}

void
NSLdvViewAreaWindow::SetupWindow()
{
try
{
	TWindow::SetupWindow() ;

  if (true == _pToon->isMainView())
    getView()->timeBumper() ;

  if (false == _pToon->getToonsArray()->empty())
  	for (ToonsIter toonIter = _pToon->getToonsArray()->begin() ; _pToon->getToonsArray()->end() != toonIter ; toonIter++)
    	(*toonIter)->SetupWindow() ;

	_pToolTip->Create() ;
	_pToolTip->Hide() ;
}
catch (...)
{
	erreur("Exception NSLdvView SetupWindow.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::reinit()
{
try
{
  TEXTMETRIC tm ;
	TWindowDC* pWinDC = new TWindowDC(GetParent()) ; // HWnd
	if (pWinDC->GetTextMetrics(tm))
  {
		_pToon->setInterligne(tm.tmHeight + tm.tmInternalLeading + tm.tmExternalLeading + 5) ;
    _pToon->setDrugInterligne(tm.tmHeight + tm.tmInternalLeading + tm.tmExternalLeading + 15) ;
  }
	delete pWinDC ;

  _pHitToon        = 0 ;
	_pCurrentToon    = 0 ;
	_pCurrentConcern = 0 ;

  _pToon->calculRectangleGlobal() ;

  setScrollParams() ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewAreaWindow::reinit.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

bool
NSLdvViewAreaWindow::PreProcessMsg(MSG &msg)
{
  PRECONDITION(GetHandle()) ;
  HACCEL hAccelerator = getView()->getAcceleratorHandle() ;
  return hAccelerator ? ::TranslateAccelerator(GetHandle(), hAccelerator, &msg) : false ;
}

// -----------------------------------------------------------------------------
//  Description: Peint/repeint tout/une partie de la fenêtre
// -----------------------------------------------------------------------------
void
NSLdvViewAreaWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
try
{
  string ps = string("Entering NSLdvViewAreaWindow::Paint")  ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  _pToon->skinDraw(&dc, &RectAPeindre) ;

	if (_pToon->getToonsArray()->empty())
		return ;

  bool bGlobal = (GetClientRect() == RectAPeindre) ;

	int iAncDC = dc.SaveDC() ;

	// Calcul du "rectangle" à repeindre

	NVLdVRect rectARepeindre(getView()) ;
  rectARepeindre = (NVLdVRect&) getAreaLogicalRectFromWindowRect(RectAPeindre) ;

	drawSpecialDates(&dc, &rectARepeindre) ;

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
		for (ToonsIter i = _pToon->getToonsArray()->begin(); _pToon->getToonsArray()->end() != i ; i++)
		{
			if ((*i)->_iZOrder == iLevel)
			{
      	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*i, NSLdvTimeToon) ;
				if (pTimeToon && pTimeToon->_bVisible && pTimeToon->_Box.Touches(rectARepeindre))
        {
        	if (((true == getView()->mustDisplayDrugs()) || (NSLdvToon::toonDrug != (*i)->_toonType)) &&
              ((true == getView()->mustDisplayGoals()) || (NSLdvToon::toonGoal != (*i)->_toonType)) &&
              ((-1 == getView()->getSeverityThreshold()) || (NSLdvToon::toonConcern != (*i)->_toonType) ||
               ((NULL != dynamic_cast<NSConcernView *>(*i)) &&
                (NULL != (dynamic_cast<NSConcernView *>(*i))->getConcern()) &&
                ((dynamic_cast<NSConcernView *>(*i))->getConcern()->getMaxSeverityIndex() > getView()->getSeverityThreshold()))
               )
             )
						(*i)->draw(&dc, &rectARepeindre, bGlobal) ;
        }
			}
			else
			{
				if (((*i)->_iZOrder > iLevel) && ((*i)->_iZOrder < iFuturLevel))
					iFuturLevel = (*i)->_iZOrder ;
			}
		}
		if (iFuturLevel == 10000)
			tourner = false ;
		else
		{
			iLevel = iFuturLevel ;
			iFuturLevel = 10000 ;
		}
	}

	dc.RestoreDC(iAncDC) ;

  ps = string("Leaving NSLdvViewAreaWindow::Paint")  ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewAreaWindow::Paint.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
  // Outil zoom : click droit = dé-zoomer
  if (NSLdvView::modeZoom == getView()->getMouseMode())
  {
    getView()->CmLdvZoomOut() ;
    return ;
  }

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  _currentPoint = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(point) ;

  // begin - add fab
  _pCurrentToon = HitTest(point) ;

  // NSSsObjView     *pSsObjView = TYPESAFE_DOWNCAST(pCurrentToon, NSSsObjView) ;
  NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;

  // Click droit sur une préoccupation existante
  if (pPbView)
  {
    //
    //
    if (_pCurrentConcern != pPbView)
    {
    	_pCurrentConcern = pPbView ;
      Invalidate() ;
    }

    // OWL::TMenu *menu           = new OWL::TMenu(HINSTANCE(*GetApplication()), TResId(IDM_PREOCCUP)) ;
    // OWL::TPopupMenu *popupMenu = new OWL::TPopupMenu(*menu) ;

    string sRefT = pSuper->getText("LigneDeVieManagement", "adaptedTreatment") ;
    string sEvol = pSuper->getText("LigneDeVieManagement", "evolutionsForThisConcern") ;
    string sEvSe = pSuper->getText("LigneDeVieManagement", "severityEvolution") ;
    string sEvNa = pSuper->getText("LigneDeVieManagement", "changeInNature") ;
    string sCorr = pSuper->getText("LigneDeVieManagement", "correctionForThisConcern") ;
    string sCoSe = pSuper->getText("LigneDeVieManagement", "correctionForSeverity") ;
    string sCoTy = pSuper->getText("LigneDeVieManagement", "correctionForType") ;
    string sCoDa = pSuper->getText("LigneDeVieManagement", "correctionForDates") ;
    string sDele = pSuper->getText("LigneDeVieManagement", "deleteThisConcern") ;
    string sRefG = pSuper->getText("LigneDeVieManagement", "adaptedGoals") ;
    string sNewG = pSuper->getText("LigneDeVieManagement", "newGoal") ;
    string sRigh = pSuper->getText("rightsManagement",     "manageRights") ;

    string sGoal = pSuper->getText("functionsLabels", ".episodus:ShowGoals") ;
    string sFoUp = pSuper->getText("functionsLabels", ".episodus:ShowFollowUp") ;
    string sCurv = pSuper->getText("functionsLabels", ".episodus:ShowCurves") ;
    string sDrug = pSuper->getText("functionsLabels", ".episodus:ShowDrugs") ;
    string sProj = pSuper->getText("functionsLabels", ".episodus:ShowProject") ;
    string sQofL = pSuper->getText("functionsLabels", ".episodus:EvaluateQualityOfLife") ;

    NSConcern* pConcern = pPbView->getConcern() ;

    LDVFRAME iFrame = ldvframeNotSpecified ;
    NSFrameInformation* pFrameInfo = getView()->getDoc()->getFrameForNode(pConcern->getNoeud()) ;
    if (pFrameInfo)
      iFrame = pFrameInfo->getFrame() ;

    if (ldvframeRisk == iFrame)
    {
      OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

      menu->AppendMenu(MF_STRING, CM_DRUG_FROM_REF, sRefT.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

      OWL::TPopupMenu *evolMenu = new OWL::TPopupMenu(AutoDelete) ;
      evolMenu->InsertMenu(-1, MF_BYPOSITION, IDC_EVOL_PREO_TYP, sEvNa.c_str()) ;
      menu->AppendMenu(MF_POPUP, OWL::TMenuItem(*evolMenu), sEvol.c_str()) ;

      OWL::TPopupMenu *corrMenu = new OWL::TPopupMenu(AutoDelete) ;
      corrMenu->InsertMenu(-1, MF_BYPOSITION, IDC_CHG_PREO_TYP,  sCoTy.c_str()) ;
      corrMenu->InsertMenu(-1, MF_BYPOSITION, IDC_CHG_PREO_DAT,  sCoDa.c_str()) ;
      menu->AppendMenu(MF_POPUP, OWL::TMenuItem(*corrMenu), sCorr.c_str()) ;

      menu->AppendMenu(MF_STRING, IDC_MANAGE_RIGHTS,  sRigh.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_SUPPRESS_PREO,  sDele.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, CM_GOAL_FROM_REF,   sRefG.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_ADD_PREO_OBJ,   sNewG.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_GOALS,      sGoal.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_FOLLOW_UP,  sFoUp.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_CURVES,     sCurv.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_DRUGS,      sDrug.c_str()) ;

      ClientToScreen(point) ;
      menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;

      delete menu ;
    }
    else
    {
      string sLexique = pConcern->getLexique() ;

      // Is there any project KS
      //
      bool bProjectKsExists = false ;
      std::vector<OB1NKS*> kss ;
      if ((NSLdvDocument::ldvseNoError == pConcern->_pDoc->GetProjectKsForLine(this, &kss, sLexique)) && (false == kss.empty()))
        bProjectKsExists = true ;

      OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

      if (bProjectKsExists)
        menu->AppendMenu(MF_STRING, ID_SHOW_PROJECT, sProj.c_str()) ;

      // Is there a quality of life set cards set for this concept
      //
      string sCodeSens ;
      pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSens) ;
      string sQualityCardFile = pContexte->getEpisodus()->getParam(string("qualityOfLifeCards"), sCodeSens) ;
      if (string("") != sQualityCardFile)
      {
        sQofL += string(" (") + sQualityCardFile + string(")") ;
        menu->AppendMenu(MF_STRING, ID_SHOW_QOFLIFE, sQofL.c_str()) ;
      }

      if (bProjectKsExists || (string("") != sQualityCardFile))
        menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

      menu->AppendMenu(MF_STRING, CM_DRUG_FROM_REF, sRefT.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

      OWL::TPopupMenu *evolMenu = new OWL::TPopupMenu(AutoDelete) ;
      evolMenu->InsertMenu(-1, MF_BYPOSITION, IDC_EVOL_PREO_IDX, sEvSe.c_str()) ;
      evolMenu->InsertMenu(-1, MF_BYPOSITION, IDC_EVOL_PREO_TYP, sEvNa.c_str()) ;
      menu->AppendMenu(MF_POPUP, OWL::TMenuItem(*evolMenu), sEvol.c_str()) ;

      OWL::TPopupMenu *corrMenu = new OWL::TPopupMenu(AutoDelete) ;
      corrMenu->InsertMenu(-1, MF_BYPOSITION, IDC_CHG_PREO_IDX,  sCoSe.c_str()) ;
      corrMenu->InsertMenu(-1, MF_BYPOSITION, IDC_CHG_PREO_TYP,  sCoTy.c_str()) ;
      corrMenu->InsertMenu(-1, MF_BYPOSITION, IDC_CHG_PREO_DAT,  sCoDa.c_str()) ;
      menu->AppendMenu(MF_POPUP, OWL::TMenuItem(*corrMenu), sCorr.c_str()) ;

      menu->AppendMenu(MF_STRING, IDC_MANAGE_RIGHTS,  sRigh.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_SUPPRESS_PREO,  sDele.c_str()) ;
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, CM_GOAL_FROM_REF,   sRefG.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_ADD_PREO_OBJ,   sNewG.c_str()) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_GOALS,      sGoal.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_FOLLOW_UP,  sFoUp.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_CURVES,     sCurv.c_str()) ;
      menu->AppendMenu(MF_STRING, ID_SHOW_DRUGS,      sDrug.c_str()) ;

      ClientToScreen(point) ;
      menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;

      // delete popupMenu ;
      delete menu ;
    }

    return ;
  }

  NSDrugLineView *pDrugView = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;

  if (pDrugView)
  {
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point ;

    OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

    string sNewD = pSuper->getText("drugManagement", "newDrug") ;
    string sStop = pSuper->getText("drugManagement", "stopThisDrug") ;
    string sKill = pSuper->getText("drugManagement", "suppresThisDrug") ;
    string sModi = pSuper->getText("drugManagement", "modifyThisDrug") ;
    string sPoso = pSuper->getText("drugManagement", "modifyPosology") ;
    string sRene = pSuper->getText("drugManagement", "renewThisDrug") ;
    string sPres = pSuper->getText("drugManagement", "buildAPrescription") ;
    string sRigh = pSuper->getText("rightsManagement", "manageRights") ;

    menu->AppendMenu(MF_STRING, CM_DRUG_NEW,    sNewD.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_DRUG_MODIF_POSO, sPoso.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_DRUG_STOP,   sStop.c_str()) ;
    menu->AppendMenu(MF_STRING, CM_DRUG_CHANGE, sModi.c_str()) ;
    menu->AppendMenu(MF_STRING, CM_DRUG_RENEW,  sRene.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, IDC_MANAGE_RIGHTS,  sRigh.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, IDC_ORDONNANCE, sPres.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu->AppendMenu(MF_STRING, CM_DRUG_DELETE, sKill.c_str()) ;

    ClientToScreen(lp) ;
    menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
    delete menu ;

    return ;
  }

  // Click on the base line
  //

  NSBaseLineView *pBaseLineView = TYPESAFE_DOWNCAST(_pCurrentToon, NSBaseLineView) ;

  if (pBaseLineView)
  {
    // création d'un menu popup
    NS_CLASSLIB::TPoint lp = point ;

    OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

    string sCoop = pSuper->getText("functionsLabels", ".episodus:NewCoopChart") ;

    menu->AppendMenu(MF_STRING, IDC_COOP_CHARTS, sCoop.c_str()) ;

    ClientToScreen(lp) ;
    menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
    delete menu ;

    return ;
  }

  // Other cases
  //

  // Calcul du point clické
  NVLdVPoint pointClick(getView()) ;
  pointClick = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(point) ;

  // On cherche l'élément de plus haut zOrder que touche ce point
  // Looking for the highest zOrder object touching the point

  int  iLevel      = 10000 ;
  int  iFuturLevel = -1 ;
  bool tourner     = true ;
  while (tourner)
  {
    for (ToonsIter i = _pToon->getToonsArray()->begin(); i != _pToon->getToonsArray()->end() ; i++)
    {
      if ((*i)->_iZOrder == iLevel)
      {
        NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*i, NSLdvTimeToon) ;
        if (pTimeToon && pTimeToon->_Box.Contains(pointClick))
        {
          if ((*i)->_bInterceptRButtonDown)
          {
            (*i)->EvRButtonDown() ;
            return ;
          }
        }
      }
      else
      {
        if (((*i)->_iZOrder < iLevel) && ((*i)->_iZOrder > iFuturLevel))
          iFuturLevel = (*i)->_iZOrder ;
      }
    }
    if (iFuturLevel == -1)
      tourner = false ;
    else
    {
      iLevel = iFuturLevel ;
      iFuturLevel = -1 ;
    }
  }

  // Si nous sommes là, c'est que le click est en dehors de
  // tout objet
  // If we are there, it means that the click is outside any object

  OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

  string sMenuText = pSuper->getText("drugManagement", "newDrug") ;
  if (pContexte->getPatient()->getReadOnly())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, CM_DRUG_NEW, sMenuText.c_str()) ;

  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

  sMenuText = pSuper->getText("LigneDeVieMenus", "newHealthConcern") ;
  if (pContexte->getPatient()->getReadOnly())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, IDC_NEWPREOCCUP, sMenuText.c_str()) ;

  sMenuText = pSuper->getText("LigneDeVieMenus", "newRisk") ;
  if (pContexte->getPatient()->getReadOnly())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, IDC_NEWRISK, sMenuText.c_str()) ;

  sMenuText = pSuper->getText("LigneDeVieMenus", "newSocialConcern") ;
  if (pContexte->getPatient()->getReadOnly())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, IDC_NEWSOCIAL, sMenuText.c_str()) ;

  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

  sMenuText = pSuper->getText("LigneDeVieMenus", "zoom") + string("Ctrl+Z)") ;

  if (getView()->isZoomMax())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, CM_LDV_ZOOMIN, sMenuText.c_str()) ;

  sMenuText = pSuper->getText("LigneDeVieMenus", "enlarge") + string(" (Ctrl+E)") ;

  if (getView()->isZoomMin())
    menu->AppendMenu(MF_GRAYED, 0, sMenuText.c_str()) ;
  else
    menu->AppendMenu(MF_STRING, CM_LDV_ZOOMOUT, sMenuText.c_str()) ;

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}
catch (...)
{
  erreur("Exception NSLdvViewAreaWindow EvRButtonDown.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::CmLdvZoomIn()
{
  getView()->CmLdvZoomIn() ;
}

void
NSLdvViewAreaWindow::CmLdvZoomOut()
{
  getView()->CmLdvZoomOut() ;
}

void
NSLdvViewAreaWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
  // Make certain that the focus doesn't get stuck in sub-area
  //
  THandle focusHdle = GetFocus() ;
  if (GetHandle() != focusHdle)
    SetFocus() ;

  if (NSLdvView::modeZoom == getView()->getMouseMode())
  {
    getView()->CmLdvZoomIn() ;
    return ;
	}

	NSLdvToon *theToon = HitTest(point) ;

	if      (dynamic_cast<NSConcernView *>(theToon))
	{
  	NSConcernView *pToon = (NSConcernView *)theToon ;
    string stheToonLibelle = pToon->getConcern()->_sTitre ;
    if (_pCurrentConcern != pToon)
    {
    	_pCurrentConcern = pToon ;
      Invalidate() ;
    }
	}
  else if (dynamic_cast<NSSsObjView *>(theToon))
    startDragging(theToon, point, 0) ;
	else if (dynamic_cast<NSLdvObjetView *>(theToon))    startDragging(theToon, point, 1) ;
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow EvLButtonDown.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::startDragging(NSLdvToon *pToon, NS_CLASSLIB::TPoint& point, int iImageIndex)
{
  _bLButtonDown = true ;
	_pToon2Drag   = pToon ;

  // !!!!!!!!!!!!!!!!!
  // in the sequence :
  // ImgDragDrop->BeginDrag(1, 16, 16) ;
  // "16, 16" is the icon's size (the icon we use in the drag n drop mode)
  // it MUST be change if we want to use another icon (with another size)
  // or another object than an icon
  // this parameter corresponds to the size of the image that is used to
  // follow the cursor in drag n drop mode
  // !!!!!!!!!!!!!!!!!

  getView()->getImgDragDrop()->BeginDrag(iImageIndex, 16, 16) ;

  //ClientToScreen(point);
  //NS_CLASSLIB::TRect winRect = Parent->GetWindowRect();
  //ImgDragDrop->DragEnter(*this, point.x - winRect.X(), point.y - winRect.Y()) ;

  getView()->getImgDragDrop()->DragEnter(*this, point.x, point.y) ;
}

void
NSLdvViewAreaWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
	NSLdvToon *theToon = HitTest(point) ;

	// End of drag and drop - Fin de drag and drop

	if (_bLButtonDown && theToon)
	{
		// Drop d'un SsObjet sur une préoccupation
		// Dropping a SubObject on a Health concern
		if (TYPESAFE_DOWNCAST(_pToon2Drag, NSSsObjView) && TYPESAFE_DOWNCAST(theToon, NSConcernView))
		{
			NSSsObjView   *pSsObjV = TYPESAFE_DOWNCAST(_pToon2Drag, NSSsObjView) ;
			NSConcernView *pPbV    = TYPESAFE_DOWNCAST(theToon,    NSConcernView) ;

			bool bContinue = true ;

			if (pSsObjV->_pSsObjet->tDateHeureDebut < pPbV->getConcern()->_tDateOuverture)
				bContinue = false ;

			if (pSsObjV->_pSsObjet->tDateHeureFin > pPbV->getConcern()->_tDateFermeture)
				bContinue = false ;

			if (bContinue)
			{
				pSsObjV->setConcernView(pPbV) ;
				pSsObjV->_pSsObjet->sConcern = pPbV->getConcern()->_sTitre ;
				NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
				pGraphe->etablirLien(pSsObjV->_pSsObjet->sReference, NSRootLink::problemContactElement, pPbV->getConcern()->_sReference) ;

				// Le "réservoir" de sous-objets orphelins est remis à jour lors
				// du rafraichissement de l'affichage

				Invalidate(true) ;
			}
		}

		// Drop d'un Objet sur une préoccupation
		// Dropping an Object on a Health concern
		else
			if (TYPESAFE_DOWNCAST(_pToon2Drag, NSLdvObjetView) && TYPESAFE_DOWNCAST(theToon, NSConcernView))
    {
      NSLdvObjetView *pObjV = TYPESAFE_DOWNCAST(_pToon2Drag, NSLdvObjetView) ;
      NSConcernView  *pPbV  = TYPESAFE_DOWNCAST(theToon, NSConcernView) ;

      bool bContinue = true ;

      if (pObjV->_pObjet->getDateHeureDebut() < pPbV->getConcern()->_tDateOuverture)
        bContinue = false ;

      if (pObjV->_pObjet->getDateHeureFin() > pPbV->getConcern()->_tDateFermeture)
        bContinue = false ;

      if (bContinue)
      {
        pObjV->setLigneView((NSLigneView *) pPbV) ;
        pObjV->_pObjet->setConcern(pPbV->getConcern()->_sTitre) ;

        VecteurString aVecteurString ;

        NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

        pGraphe->TousLesVraisDocument(pObjV->_pObjet->getReference(), NSRootLink::problemRelatedTo, &aVecteurString) ;
        if (!aVecteurString.empty())
          pGraphe->detruireTousLesLiensDocument(pObjV->_pObjet->getReference(), NSRootLink::problemRelatedTo) ;

        pGraphe->etablirLienDocument(pObjV->_pObjet->getReference(), NSRootLink::problemRelatedTo, pPbV->getConcern()->_sReference) ;

        pObjV->_Box.setTop(pPbV->_Box.getTop()) ;
        pObjV->_Box.setBottom(pPbV->_Box.getBottom()) ;

        Invalidate(true) ;
      }
    }
    // Drop d'un Objet sur la ligne de base
    // Dropping an Object on the base line
    else
      if (TYPESAFE_DOWNCAST(_pToon2Drag, NSLdvObjetView) && TYPESAFE_DOWNCAST(theToon, NSBaseLineView))
    {
      NSLdvObjetView *pObjV = TYPESAFE_DOWNCAST(_pToon2Drag, NSLdvObjetView) ;
      NSBaseLineView *pPbV  = TYPESAFE_DOWNCAST(theToon, NSBaseLineView) ;

      pObjV->setLigneView((NSLigneView *) pPbV) ;
      pObjV->_pObjet->setConcern(string("")) ;

      VecteurString aVecteurString ;

      NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
      pGraphe->TousLesVraisDocument(pObjV->_pObjet->getReference(), NSRootLink::problemRelatedTo, &aVecteurString) ;
      if (false == aVecteurString.empty())
        pGraphe->detruireTousLesLiensDocument(pObjV->_pObjet->getReference(), NSRootLink::problemRelatedTo) ;

      pObjV->_Box.setTop(pPbV->_Box.getTop()) ;
      pObjV->_Box.setBottom(pPbV->_Box.getBottom()) ;

      Invalidate(true) ;
    }
	}

	_bLButtonDown = false ;

	getView()->getImgDragDrop()->DragLeave(getView()->GetWindow()->HWindow) ;
	getView()->getImgDragDrop()->EndDrag() ;

	_pToon2Drag = NULL ;
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow EvLButtonUp.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
  if (NSLdvView::modeZoom == getView()->getMouseMode())
    return ;

  NSLdvToon *theToon = HitTest(point) ;

  NSLdvObjetView *pObjV = TYPESAFE_DOWNCAST(theToon, NSLdvObjetView) ;
  if (pObjV && pObjV->_pObjet)
  {
    // pLdVDoc->openLdVObject(pObjV->pObjet) ;

    //
    // On prépare l'ouverture de tous les objets à cet endroit
    //
    ArrayOfToons aToons ;
    int          iObjZorder = pObjV->_iZOrder ;
    HitTestAll(point, &aToons, iObjZorder, false) ;

    // Précaution...
    if (aToons.empty())
      return ;

    // On fabrique l'array d'objets
    //
    ArrayObjets aObjs(getView()->getDoc()) ;
    for (ToonsIter i = aToons.begin() ; aToons.end() != i ; i++)
    {
      NSLdvObjetView *pTheObjV = TYPESAFE_DOWNCAST(*i, NSLdvObjetView) ;
      if (pTheObjV && pTheObjV->_pObjet)
        aObjs.push_back(pTheObjV->_pObjet) ;
    }

    if (false == aObjs.empty())
      getView()->getDoc()->openLdVObjectsInArray(&aObjs, getView()) ;

    // Vider l'array de toons sans détruire les objets
    //
    for (ToonsIter toonIt = aToons.begin() ; aToons.end() != toonIt ; )
      aToons.erase(toonIt) ;

    // Vider l'array d'objets sans détruire les objets
    //
    if (false == aObjs.empty())
      for (ArrayObjIter objIt = aObjs.begin(); aObjs.end() != objIt ; )
        aObjs.erase(objIt) ;

    return ;
  }

  NSConcernView *pCrnV  = TYPESAFE_DOWNCAST(theToon, NSConcernView) ;
  if (pCrnV && pCrnV->getConcern())
  {
    getView()->displayCurvesForBiometricalGoals(pCrnV->getConcern()->getNoeud()) ;
    pCrnV->getConcern()->sendActivationEvent() ;
    getView()->getDoc()->showConcernProperty(pCrnV->getConcern()) ;
    return ;
  }

  NSDrugLineView *pDrgV  = TYPESAFE_DOWNCAST(theToon, NSDrugLineView) ;
  if (pDrgV && pDrgV->getDrug())
  {
    NSLdvDrug* pDrug = pDrgV->getDrug() ;
    getView()->displayCurvesForBiometricalGoals(pDrug->getNoeud()) ;
    pDrug->sendActivationEvent() ;
    getView()->getDoc()->showDrugProperty(pDrug) ;
    return ;
  }

  NSBaseLineView *pBasV  = TYPESAFE_DOWNCAST(theToon, NSBaseLineView) ;
  if (pBasV)
  {
    getView()->getDoc()->showConcernProperty(0) ;
    return ;
  }
}
catch (...)
{
  erreur("Exception NSLdvViewAreaWindow EvLButtonDblClk.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::EvVScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl)
{
try
{
  int iWorkingSpaceHeight = getWorkingSpaceHeight() ;
  int iCurrentYRange      = _pToon->getGlobalBox()->Height() ;

  int iMaxDeltaPos = iCurrentYRange - iWorkingSpaceHeight ;

  if (iMaxDeltaPos <= 0)
    return ;

  switch(scrollCode)
  {
    case SB_LINEUP:
      increasePosInfDroit(5) ;
      break ;
    case SB_LINEDOWN:
      increasePosInfDroit(-5) ;
      break ;
    case SB_PAGEUP:
      increasePosInfDroit(iWorkingSpaceHeight - 5) ;
      break ;
    case SB_PAGEDOWN:
      increasePosInfDroit(-iWorkingSpaceHeight + 5) ;
      break ;
    case SB_THUMBPOSITION:
    {
      // Just remember that when we set thumbPos, we do iThumbPos = iMaxDeltaPos - getPosInfDroit() ;
      int iNewPosInfDroit = iMaxDeltaPos - thumbPos ;
      int iDeltaPos = iNewPosInfDroit - (int)getPosInfDroit() ;
      increasePosInfDroit(iDeltaPos) ;
      break ;
    }
    case SB_THUMBTRACK:
    {
      // Just remember that when we set thumbPos, we do iThumbPos = iMaxDeltaPos - getPosInfDroit() ;
      int iNewPosInfDroit = iMaxDeltaPos - thumbPos ;
      int iDeltaPos = iNewPosInfDroit - (int)getPosInfDroit() ;
      if ((iDeltaPos >= 5) || (iDeltaPos <= -5))
        increasePosInfDroit(iDeltaPos) ;
      break ;
    }
  }

	if (scrollCode != SB_THUMBTRACK)
  {
    _pToon->reinitCurves() ;
		Invalidate() ;
  }
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow EvVScroll.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::setScrollParams()
{
  int iWorkingSpaceHeight = getWorkingSpaceHeight() ;
  int iCurrentYRange      = _pToon->getGlobalBox()->Height() ;

  SCROLLINFO scInfo ;
	scInfo.cbSize = (UINT) sizeof(scInfo) ;
	scInfo.fMask = SIF_ALL ;
	scInfo.nMin  = 0 ;
	scInfo.nMax  = 0 ;
	scInfo.nPage = 0 ;
	scInfo.nPos  = 0 ;

  // Every toon fits inside the workingspace
  //
	if (iCurrentYRange <= iWorkingSpaceHeight)
  {
  	SetScrollInfo(SB_VERT, &scInfo, TRUE) ;
    return ;
  }

  // Since we start from the bottom, we must "reverse" nPos
  //
  int iMaxDeltaPos = iCurrentYRange - iWorkingSpaceHeight ;
  int iThumbPos    = iMaxDeltaPos - getPosInfDroit() ;

	scInfo.nMax  = iCurrentYRange ;
	scInfo.nPage = (UINT) (iWorkingSpaceHeight) ;
	scInfo.nPos  = iThumbPos ;

  SetScrollInfo(SB_VERT, &scInfo, TRUE) ;
}

void
NSLdvViewAreaWindow::closeMultiTips()
{
  if ((NSLdvViewIconsShowWindow *) NULL == _pMultiTips)
    return ;

  _pMultiTips->CloseWindow() ;

  delete _pMultiTips ;
  _pMultiTips = (NSLdvViewIconsShowWindow *) 0 ;
}

void
NSLdvViewAreaWindow::closeMagnifier()
{
  if ((NSLdvViewMagnifier*) NULL == _pMagnifier)
    return ;

  // _pMagnifier->CloseWindow() ;

  delete _pMagnifier ;
  _pMagnifier = (NSLdvViewMagnifier*) 0 ;

  _bMustCloseMagnifier = false ;
}

void
NSLdvViewAreaWindow::EvMouseMove(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  if (false == pMyApp->isForegroundApplication())
    return ;

  getView()->resetNbTimerTicks() ;

	if (_bLButtonDown)
		getView()->getImgDragDrop()->DragMove(point.x, point.y) ;

	if (getView()->getMouseMode() == NSLdvView::modeZoom)
		point.x += GetSystemMetrics(SM_CXCURSOR) / 2 ;

	// Calcul du point clické

	NVLdVPoint pointClick(getView()) ;
  pointClick = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(point) ;

	NVLdVTemps tDateHeure = pointClick.getX() ;

	string sDate  = tDateHeure.donneDate() ;
	string sHeure = tDateHeure.donneHeure() ;

	string sLang = pContexte->getUserLanguage() ;

	string sMessage = donne_date(sDate, sLang) + string(" ") + donne_heure(sHeure) ;
	getView()->afficheStatusMessage(sMessage, false) ;
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow EvMouseMove.", standardError, 0) ;
}
}

void
NSLdvViewAreaWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
  getView()->EvKeyDown(key, repeatCount, flags) ;
}

void
NSLdvViewAreaWindow::EvSysKeyDown(uint key, uint repeatCount, uint flags)
{
  getView()->EvSysKeyDown(key, repeatCount, flags) ;
}

void
NSLdvViewAreaWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  getView()->EvSize(sizeType, size) ;
}

void
NSLdvViewAreaWindow::EvSetFocus(THandle hWndLostFocus)
{
  getView()->EvSetFocus(hWndLostFocus) ;
}

void
NSLdvViewAreaWindow::CmNewPreoccup()
{
  getView()->CmNewPreoccup() ;
}

void
NSLdvViewAreaWindow::CmNewRisk()
{
  getView()->CmNewRisk() ;
}

void
NSLdvViewAreaWindow::CmNewSocialConcern()
{
  getView()->CmNewSocialConcern() ;
}

void
NSLdvViewAreaWindow::CmEvolPreoccup()
{
  NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  getView()->CmEvolPreoccup(pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmShowGoalsForConcern()
{
  getView()->CmShowGoalsForConcern() ;
}

void
NSLdvViewAreaWindow::CmShowDrugsForConcern()
{
  getView()->CmShowDrugsForConcern() ;
}

void
NSLdvViewAreaWindow::CmShowFollowUpForConcern()
{
  getView()->CmShowFollowUpForConcern() ;
}

void
NSLdvViewAreaWindow::CmShowCurvesForConcern()
{
  getView()->CmShowCurvesForConcern() ;
}

void
NSLdvViewAreaWindow::drawSpecialDates(TDC* pDc, NVLdVRect *pRectARepeindre)
{
try
{
	NVLdVTemps tpsBirthDay = getView()->getDateNaissance() ;
	NVLdVTemps tpsNow ;
	tpsNow.takeTime() ;

	if ((pRectARepeindre->getLeft() <= tpsBirthDay) && (pRectARepeindre->getRight() >= tpsBirthDay))
  {
    NVLdVPoint PtBas(getView()) ;
    PtBas.setX(tpsBirthDay) ;
    PtBas.setY(pRectARepeindre->getBottom()) ;
    NS_CLASSLIB::TPoint Bottom = getScrollablePhysicalPoint(PtBas) ;
    pDc->MoveTo(Bottom) ;

    NVLdVPoint PtHaut(PtBas) ;
    PtHaut.setY(pRectARepeindre->getTop() + 1) ;
    NS_CLASSLIB::TPoint Top = getScrollablePhysicalPoint(PtHaut) ;
    pDc->LineTo(Top) ;
  }

  if ((pRectARepeindre->getLeft() <= tpsNow) && (pRectARepeindre->getRight() >= tpsNow))
  {
    NVLdVPoint PtBas(PtBas) ;
    PtBas.setX(tpsNow) ;
    PtBas.setY(pRectARepeindre->getBottom()) ;
    NS_CLASSLIB::TPoint Bottom = getScrollablePhysicalPoint(PtBas) ;
    pDc->MoveTo(Bottom) ;

    NVLdVPoint PtHaut(PtBas) ;
    PtHaut.setY(pRectARepeindre->getTop() + 1) ;
    NS_CLASSLIB::TPoint Top = getScrollablePhysicalPoint(PtHaut) ;
    pDc->LineTo(Top) ;
  }

  return ;
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow drawSpecialDates.", standardError, 0) ;
}
}

NSLdvToon*
NSLdvViewAreaWindow::HitTest(NS_CLASSLIB::TPoint& point)
{
try
{
  if (_pToon->getToonsArray()->empty())
    return (NSLdvToon*) 0 ;

	NVLdVPoint ldvPoint(getView()) ;
  ldvPoint = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(point) ;

	int       iToonZOrder = -1 ;
	NSLdvToon *pToon = 0 ;

	for (ToonsIter theToonsIter = _pToon->getToonsArray()->begin() ; _pToon->getToonsArray()->end() != theToonsIter ; theToonsIter++)
	{
  	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*theToonsIter, NSLdvTimeToon) ;
		if (pTimeToon && (pTimeToon->_VisibleBox.Contains(ldvPoint)) &&
				(pTimeToon->_iZOrder >= iToonZOrder))
		{
			pToon = *theToonsIter ;
			iToonZOrder = (*theToonsIter)->_iZOrder ;
		}
	}

  return (pToon) ;
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow HitTest.", standardError, 0) ;
  return 0 ;
}
}

//
// Put in pAToons all toons within that point (with the proper zOrder)
//
void
NSLdvViewAreaWindow::HitTestAll(NS_CLASSLIB::TPoint& point, ArrayOfToons* pAToons, int iToonZOrder, bool bResetArray)
{
try
{
  if (NULL == pAToons)
    return ;

  if (true == bResetArray)
  {
    // Vider l'array sans détruire les objets
    //
    if (false == pAToons->empty())
      for (ToonsIter i = pAToons->begin(); i != pAToons->end(); )
        pAToons->erase(i) ;
  }

  if (_pToon->getToonsArray()->empty())
    return ;

	NVLdVPoint ldvPoint(getView()) ;
  ldvPoint = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(point) ;

	int iToonZOrder = -1 ;
	for (ToonsIter theToonsIter = _pToon->getToonsArray()->begin() ; _pToon->getToonsArray()->end() != theToonsIter ; theToonsIter++)
  {
  	NSLdvTimeToon* pTimeToon = TYPESAFE_DOWNCAST(*theToonsIter, NSLdvTimeToon) ;
		if (pTimeToon && (pTimeToon->_VisibleBox.Contains(ldvPoint)) &&
            (((*theToonsIter)->_iZOrder == iToonZOrder) || (-1 == iToonZOrder)))
      pAToons->push_back(*theToonsIter) ;
  }
}
catch (...)
{
	erreur("Exception NSLdvViewAreaWindow HitTestAll.", standardError, 0) ;
	return ;
}
}

NSLdvToon*
NSLdvViewAreaWindow::nextHitTest(NS_CLASSLIB::TPoint& point, NSLdvToon* pFormerToon)
{
try
{
	NS_CLASSLIB::TRect  clientRect = GetClientRect() ;
	NS_CLASSLIB::TPoint ptOrigine  = clientRect.BottomRight() ;
	NS_CLASSLIB::TPoint ptTemp     = point ;

	// on construit un rectangle autour du point où se situe le point passé en
	// paramètre, ce rectangle est en faits un carré de 32 pixels (2 fois la
	// taille d'un icône) de côté dont le point passé en paramètre est le centre.
	// L'idée est de tester si ce rectangle à une intersection avec un objet
	// quelconque et de renvoyer cet objet, s'il y a plusieurs objets on
	// retourne le premier objet qui a le ZOrder le plus important.

	// test des dates à une largeur d'icône à gauche, à une largeur d'icône en haut
	ptTemp.x = point.x - ICONWIDTH ;   // les icônes font 16 pixels de largeur
	ptTemp.y = point.y + 1 ;   // les icônes font 16 pixels de hauteur
	NVLdVPoint  HitTestTopLeft(getView()) ;
	HitTestTopLeft = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(ptTemp) ;

	// test des dates à une largeur d'icône à droite
	ptTemp.x = point.x + ICONWIDTH ;   // les icônes font 16 pixels de largeur
	ptTemp.y = point.y - 1 ;   // les icônes font 16 pixels de hauteur
	NVLdVPoint  HitTestBotRigh(getView()) ;
	HitTestBotRigh = (NVLdVPoint&) getAreaLogicalPointFromWindowPoint(ptTemp) ;

	NVLdVRect   rectHitTest = NVLdVRect(getView(), HitTestTopLeft.getX(), HitTestBotRigh.getX(), HitTestTopLeft.getY(), HitTestBotRigh.getY()) ;

//	ArrayOfToons    *pToons = new ArrayOfToons() ;
	int             iToonZOrder = -1 ;
	NSLdvToon       *pToon = 0;

	for (ToonsIter theToonsIter = _pToon->getToonsArray()->begin() ; _pToon->getToonsArray()->end() != theToonsIter ; theToonsIter++)
	{
		if (*theToonsIter == pFormerToon)
			return (pToon) ;

		if (TYPESAFE_DOWNCAST(*theToonsIter, NSBaseLineView))
		{
			NSBaseLineView *pPbV = TYPESAFE_DOWNCAST(*theToonsIter, NSBaseLineView) ;
			if (pPbV->_Box.Touches(rectHitTest))
			{
				if ((*theToonsIter)->_iZOrder >= iToonZOrder)
				{
					pToon = *theToonsIter ;
					iToonZOrder = (*theToonsIter)->_iZOrder ;
				}
			}
		}
		else
			if (TYPESAFE_DOWNCAST(*theToonsIter, NSConcernView))
			{
				NSConcernView *pPbV = TYPESAFE_DOWNCAST(*theToonsIter, NSConcernView) ;
				if (pPbV->_Box.Touches(rectHitTest))
				{
					if ((*theToonsIter)->_iZOrder >= iToonZOrder)
					{
						pToon = *theToonsIter ;
						iToonZOrder = (*theToonsIter)->_iZOrder ;
					}
				}
			}

			else
				if (TYPESAFE_DOWNCAST(*theToonsIter, NSLdvObjetView))
				{
					NSLdvObjetView *pObjV = TYPESAFE_DOWNCAST(*theToonsIter, NSLdvObjetView) ;
					if (pObjV->_Box.Touches(rectHitTest))
					{
						if ((*theToonsIter)->_iZOrder >= iToonZOrder)
						{
							pToon = *theToonsIter ;
							iToonZOrder = (*theToonsIter)->_iZOrder ;
						}
					}
				}

				else
					if (TYPESAFE_DOWNCAST(*theToonsIter, NSSsObjView))
					{
						NSSsObjView *pSsObjV = TYPESAFE_DOWNCAST(*theToonsIter, NSSsObjView) ;
						if (pSsObjV->_Box.Touches(rectHitTest))
						{
							if ((*theToonsIter)->_iZOrder >= iToonZOrder)
							{
								pToon = *theToonsIter ;
								iToonZOrder = (*theToonsIter)->_iZOrder ;
							}
						}
					}
	}

	return (pToon) ;
}
catch (...)
{
	erreur("Exception NSLdvView nextHitTest.", standardError, 0) ;
	return 0;
}
}

void
NSLdvViewAreaWindow::CmChgPreoType()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  getView()->CmChgPreoType(pPbView->getConcern(), pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmChgPreoDate()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  getView()->CmChgPreoDate(pPbView->getConcern(), pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmChgPreoIndx()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  getView()->CmChgPreoIndx(pPbView->getConcern(), _currentPoint.getX(), pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmAddObjectifs()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  NSConcern* pConcern = pPbView->getConcern() ;
  if (NULL == pConcern)
    return ;

  NSFrameInformation* pFrameInfo = getView()->getDoc()->getFrameForNode(pConcern->getNoeud()) ;
  if (NULL == pFrameInfo)
    return ;

  LDVFRAME iFrame = iFrame = pFrameInfo->getFrame() ;

  getView()->CmAddObjectifs(pPbView->getConcern(), iFrame) ;
}

void
NSLdvViewAreaWindow::CmPreoProject()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  NSConcern* pConcern = pPbView->getConcern() ;
  if (NULL == pConcern)
    return ;

  NSFrameInformation* pFrameInfo = getView()->getDoc()->getFrameForNode(pConcern->getNoeud()) ;
  if (NULL == pFrameInfo)
    return ;

  LDVFRAME iFrame = iFrame = pFrameInfo->getFrame() ;

  std::vector<OB1NKS*> kss ;
  if ((NSLdvDocument::ldvseNoError != pConcern->_pDoc->GetProjectKsForLine(this, &kss, pConcern->getLexique())) || (kss.empty()))
    return ;

  if (kss.size() == 1)
  {
    std::vector<OB1NKS*>::iterator iter = kss.begin() ;
    string sKsName    = (*iter)->getKSName() ;
    string sArchetype = (*iter)->getKS()->Value("archetype", NULLSTRING) ;
    getView()->CmProject(pPbView->getConcern(), iFrame, sKsName, sArchetype) ;
    return ;
  }
}

void
NSLdvViewAreaWindow::CmPreoQualityOfLife()
{
  NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  NSConcern* pConcern = pPbView->getConcern() ;
  if (NULL == pConcern)
    return ;

  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&(pConcern->getLexique()), &sCodeSens) ;
  string sQualityCardFile = pContexte->getEpisodus()->getParam(string("qualityOfLifeCards"), sCodeSens) ;

  NSCoopChartDocument* pDocNewCoopChart = new NSCoopChartDocument(0, pContexte, sQualityCardFile, pConcern->getNoeud()) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pDocNewCoopChart, "Coop Chart") ;
}

void
NSLdvViewAreaWindow::CmEvolPreoIndx()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  getView()->CmEvolPreoIndx(pPbView->getConcern(), _currentPoint.getX(), pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmEvolPreoType()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  getView()->CmEvolPreoType(pPbView->getConcern(), pPbView->getFrame()) ;
}

void
NSLdvViewAreaWindow::CmSuppressPreo()
{
	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

	// Recherche de cette préoccupation dans l'Array
	// Looking for this health upset in the Array

	if (NULL == pPbView->getConcern())
		return ;

  string sTitleText = pContexte->getSuperviseur()->getText("LigneDeVieManagement", "ligneDeVie") ;
  string sMenuText  = pContexte->getSuperviseur()->getText("LigneDeVieManagement", "doYouReallyWantToDeleteThisHealthConcern?") ;

  int retVal = MessageBox(sMenuText.c_str(), sTitleText.c_str(), MB_YESNO) ;
	if (retVal != IDYES)
		return ;

  getView()->CmSuppressPreo(pPbView->getConcern()) ;
}

void
NSLdvViewAreaWindow::CmDrugNew()
{
  NSConcern* pRelatedConcern = 0 ;

  if (_pCurrentToon && (NSLdvToon::toonConcern == _pCurrentToon->_toonType))
  {
		NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
		if (pPbView)
      pRelatedConcern = pPbView->getConcern() ;
  }

  getView()->CmDrugNewForConcern(pRelatedConcern) ;
}

void
NSLdvViewAreaWindow::CmDrugRenew()
{
  if ((NULL == _pCurrentToon) || (NSLdvToon::toonDrug != _pCurrentToon->_toonType))
		return ;

	NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
	if ((NULL == pDrugLine) || (NULL == pDrugLine->getDrug()))
		return ;

  getView()->CmDrugRenew(pDrugLine->getDrug()) ;
}

void
NSLdvViewAreaWindow::CmDrugModify()
{
  if ((NULL == _pCurrentToon) || (NSLdvToon::toonDrug != _pCurrentToon->_toonType))
		return ;

	NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
	if ((NULL == pDrugLine) || (NULL == pDrugLine->getDrug()))
		return ;

  getView()->CmDrugModify(pDrugLine->getDrug()) ;
}

void
NSLdvViewAreaWindow::CmDrugChangePoso()
{
  if ((NULL == _pCurrentToon) || (NSLdvToon::toonDrug != _pCurrentToon->_toonType))
		return ;

	NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
	if ((NULL == pDrugLine) || (NULL == pDrugLine->getDrug()))
		return ;

  getView()->CmDrugChangePoso(pDrugLine->getDrug()) ;
}

void
NSLdvViewAreaWindow::CmDrugStop()
{
  if ((NULL == _pCurrentToon) || (NSLdvToon::toonDrug != _pCurrentToon->_toonType))
		return ;

	NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
	if ((NULL == pDrugLine) || (NULL == pDrugLine->getDrug()))
		return ;

  getView()->CmDrugStop(pDrugLine->getDrug()) ;
}

void
NSLdvViewAreaWindow::CmDrugDelete()
{
  if ((NULL == _pCurrentToon) || (NSLdvToon::toonDrug != _pCurrentToon->_toonType))
		return ;

	NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
	if ((NULL == pDrugLine) || (NULL == pDrugLine->getDrug()))
		return ;

  getView()->CmDrugDelete(pDrugLine->getDrug()) ;
}

void
NSLdvViewAreaWindow::CmNewDrugsFromRefForConcern()
{
  if (NULL == _pCurrentToon)
		return ;

	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  getView()->CmNewDrugsFromRefForConcern(pPbView->getConcern()) ;
}

void
NSLdvViewAreaWindow::CmNewGoalsFromRefForConcern()
{
  if (NULL == _pCurrentToon)
		return ;

	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (NULL == pPbView)
		return ;

  getView()->CmNewGoalsFromRefForConcern(pPbView->getConcern()) ;
}

void
NSLdvViewAreaWindow::CmDrugPrescription()
{
  getView()->CmDrugPrescription() ;
}

void
NSLdvViewAreaWindow::CmManageRights()
{
  if (NULL == _pCurrentToon)
    return ;

	NSConcernView *pPbView = TYPESAFE_DOWNCAST(_pCurrentToon, NSConcernView) ;
	if (pPbView)
  {
		if (NULL == pPbView->getConcern())
			return ;

    getView()->CmManageRights(pPbView->getConcern()) ;

    return ;
	}

  NSDrugLineView *pDrugLine = TYPESAFE_DOWNCAST(_pCurrentToon, NSDrugLineView) ;
  if (pDrugLine)
  {
    if (NULL == pDrugLine->getDrug())
      return ;

    getView()->CmManageRights(pDrugLine->getDrug()) ;

    return ;
  }
}

int
NSLdvViewAreaWindow::getPixelPosAboveChronoline(int iYPos)
{
  return iYPos ;
}

int
NSLdvViewAreaWindow::getPixelPosAboveBaseline(int iYPos)
{
	//
  // Return a point above ChronoLine and above Base Line
  //
	return iYPos + getLargeurBaseline() + getInterligne() ;
}

NS_CLASSLIB::TRect
NSLdvViewAreaWindow::getClientRectAboveChronoline()
{
	NS_CLASSLIB::TRect clientRect = GetClientRect() ;

  int iBottom = getPixelPosAboveChronoline(0) ;
  int iTop    = clientRect.Height() ;
  int iLeft   = 0 ;
  int iRight  = clientRect.Width() ;

/*
  NSLdvWindowToon* pLdvWinToon = getLdVWinToon(NSLdvToon::toonYAxis) ;
  if (NULL != pLdvWinToon)
  	// iLeft += pLdvWinToon->wVisibleBox.Width() ;
    iLeft += pLdvWinToon->donneVisibleRectangle().Width() ;
*/

  return NS_CLASSLIB::TRect(iLeft, iBottom, iRight, iTop) ;
}

NS_CLASSLIB::TRect
NSLdvViewAreaWindow::getClientRectAboveBaseline()
{
	NS_CLASSLIB::TRect cR = getClientRectAboveChronoline() ;
  return NS_CLASSLIB::TRect(cR.Left(), getPixelPosAboveBaseline(0), cR.Right(), cR.Bottom()) ;
}

int
NSLdvViewAreaWindow::getProccupGrossHeight()
{
	return getLargeurLigne() + getInterligne() ;
}

long
NSLdvViewAreaWindow::preoccupDonneBottomBox(int iIndex)
{
	// Memo : Preoccup 1 -> iIndex == 0
	// return (getPixelPosAboveBaseline(0) + iIndex * getProccupGrossHeight()) ;
  return preoccupDonneBottomLine(iIndex) ;
}

long
NSLdvViewAreaWindow::preoccupDonneTopBox(int iIndex)
{
	// Memo : Preoccup 1 -> iIndex == 0
	return preoccupDonneBottomBox(iIndex) + getProccupGrossHeight() ;
}

long
NSLdvViewAreaWindow::preoccupDonneBottomLine(int iIndex)
{
	// Memo : Preoccup 1 -> iIndex == 0
  //        Baseline   -> iIndex == -1

  if (iIndex >= 0)
	  return (getPixelPosAboveBaseline(0) + iIndex * getProccupGrossHeight()) ;
  else
    return _pToon->getBottomMargin() ;
}

long
NSLdvViewAreaWindow::preoccupDonneTopLine(int iIndex)
{
	// Memo : Preoccup 1 -> iIndex == 0
	return (preoccupDonneBottomLine(iIndex) + getLargeurLigne()) ;
}

int
NSLdvViewAreaWindow::getGoalBasePosition()
{
	return (preoccupDonneTopBox(getMaxConcernIndex()) + getInterligne()) ;
}

int
NSLdvViewAreaWindow::getGoalGrossHeight()
{
	return getLargeurSubLigne() + getInterligne() ;
}

long
NSLdvViewAreaWindow::goalDonneBottomBox(int iIndex)
{
	return (getGoalBasePosition() + iIndex * getGoalGrossHeight()) ;
}

long
NSLdvViewAreaWindow::goalDonneTopBox(int iIndex)
{
	return (goalDonneBottomBox(iIndex) + getGoalGrossHeight()) ;
}

long
NSLdvViewAreaWindow::goalDonneBottomLine(int iIndex)
{
	return (getGoalBasePosition() + iIndex * getGoalGrossHeight()) ;
}

long
NSLdvViewAreaWindow::goalDonneTopLine(int iIndex)
{
	return (goalDonneBottomLine(iIndex) + getLargeurSubLigne()) ;
}

int
NSLdvViewAreaWindow::getDrugBasePosition()
{
	return (goalDonneTopBox(getMaxGoalIndex()) + getInterligne()) ;
}

int
NSLdvViewAreaWindow::getDrugGrossHeight()
{
	return getLargeurSubLigne() + getDrugInterligne() ;
}

long
NSLdvViewAreaWindow::drugDonneBottomBox(int iIndex)
{
	return (getDrugBasePosition() + iIndex * getDrugGrossHeight()) ;
}

long
NSLdvViewAreaWindow::drugDonneTopBox(int iIndex)
{
	return (drugDonneBottomBox(iIndex) + getDrugGrossHeight()) ;
}

long
NSLdvViewAreaWindow::drugDonneBottomLine(int iIndex)
{
	return (getDrugBasePosition() + iIndex * getDrugGrossHeight()) ;
}

long
NSLdvViewAreaWindow::drugDonneTopLine(int iIndex)
{
	return (drugDonneBottomLine(iIndex) + getLargeurSubLigne() ) ;
}

void
NSLdvViewAreaWindow::showContextHelp()
{
  actionsOnIconHalt(false) ;
}

void
NSLdvViewAreaWindow::subSignalForHelp()
{
  actionsOnIconHalt(true) ;
}

void
NSLdvViewAreaWindow::actionsOnIconHalt(bool bIsSubSignal)
{
  // _pMultiTips asked to be shut
  //
  if (_bMustCloseMultitips)
  {
    _bMustCloseMultitips = false ;
    closeMultiTips() ;
  }

  // _pMagnifier asked to be shut
  //
  if (_bMustCloseMagnifier)
  {
    _bMustCloseMagnifier = false ;
    closeMagnifier() ;
  }

  NS_CLASSLIB::TPoint curPos ;
	GetCursorPos(curPos) ;

  // If Multitips window is opened and cursor is within, then do nothing
  //
  // Take care to call containsPoint before any ScreenToClient
  //
  if (_pMultiTips && _pMultiTips->containsPoint(curPos))
    return ;

	ScreenToClient(curPos) ;

	NSLdvToon* pNewHitToon = HitTest(curPos) ;

  // Cursor is not on any icon, hide tooltip and leave
  //
  if (NULL == pNewHitToon)
  {
    _pToolTip->Hide() ;
    closeOpenObject() ;

    _pHitToon      = (NSLdvToon*) 0 ;
    _hitToonStatus = statusNone ;

    return ;
  }

  // Cursor not on same icon, reset previous icon
  //
	if (pNewHitToon != _pHitToon)
	{
    _pToolTip->Hide() ;
    closeOpenObject() ;

    _pHitToon      = (NSLdvToon*) 0 ;
    _hitToonStatus = statusNone ;
	}

  // Cursor is on a document (or several documents)
  //
  NSLdvObjetView *pObject = dynamic_cast<NSLdvObjetView*>(pNewHitToon) ;
	if (pObject)
	{
		string stheToonLibelle = pObject->_pObjet->getTitre() ;

    bool bOtherObjectFound = false ;

		NSLdvToon* pOtherToon = nextHitTest(curPos, pNewHitToon) ;
		while (pOtherToon)
		{
			if (dynamic_cast<NSLdvObjetView *>(pOtherToon))
			{
				NSLdvObjetView *pOtherObject = (NSLdvObjetView *)pOtherToon ;
				stheToonLibelle += string(1, '\n') + pOtherObject->_pObjet->getTitre() ;

        bOtherObjectFound = true ;
			}
			pOtherToon = nextHitTest(curPos, pOtherToon) ;
		}

    // Not on previously selected document... show tooltip... or when multiple
    //                                                maybe directly multitips
    //
    if (NULL == _pHitToon)
	  {
      _pHitToon = pNewHitToon ;

      if ((false == bOtherObjectFound) || _bShowTooltipsBeforeMultitips)
      {
        NS_CLASSLIB::TRect rectDoc = _pHitToon->donneRectangle() ;
		    rectDoc.right += ICONWIDTH ;
        _pToolTip->Show(rectDoc, &stheToonLibelle, 0, -1, NULL, NULL, 10) ;

        _hitToonStatus = statusName ;
      }
      else if (false == bIsSubSignal)
        showMultitips(pObject, pNewHitToon, curPos) ;
    }

    // On previously selected document
    //
    else
    {
      // On previously selected document and no other, open it
      //
      if (false == bOtherObjectFound)
      {
        // Pointed object still opening, check its position
        //
        if (statusOpening == _hitToonStatus)
        {
          setPosForActiveObject() ;
          return ;
        }
        else
        {
          _pToolTip->Hide() ;
           if ((statusOpening  != _hitToonStatus) &&
               (statusPutFront != _hitToonStatus) &&
               (statusOpened   != _hitToonStatus))
            _hitToonStatus = openObject(pObject) ;
        }
      }
      // On previously selected document and others, open icons selection window
      //
      else if (NULL == _pMultiTips)
      {
        showMultitips(pObject, pNewHitToon, curPos) ;

/*        _pMultiTips = new NSLdvViewIconsShowWindow(pContexte, this) ;
        _pMultiTips->addObject(pObject) ;

        int iObjNb = 1 ;

        NSLdvToon* pOtherToon = nextHitTest(curPos, pNewHitToon) ;
        while (pOtherToon)
        {
          if (dynamic_cast<NSLdvObjetView *>(pOtherToon))
          {
            NSLdvObjetView *pOtherObject = (NSLdvObjetView *) pOtherToon ;
            _pMultiTips->addObject(pOtherObject) ;
            iObjNb++ ;
          }
          pOtherToon = nextHitTest(curPos, pOtherToon) ;
        }

        int iClientWidth  = iObjNb * 16 + (iObjNb - 1) * 4 ;
        int iClientHeight = 40 ;

        _pMultiTips->SetWindowPos(HWindow, curPos.X() - iClientWidth / 2, curPos.Y() - iClientHeight + 2, iClientWidth, iClientHeight, SWP_NOZORDER) ;
        _pMultiTips->Create() ; */
      }
    }

		// NS_CLASSLIB::TRect rectDoc = _pHitToon->donneRectangle() ;
		// rectDoc.right += ICONWIDTH ;
		// _pToolTip->Show(rectDoc, &stheToonLibelle, 0, -1, NULL, NULL, 10) ;

    return ;
	}
  else
    _pToolTip->Hide() ;

  // Cursor is on a line
  //
  NSLigneView *pLine = dynamic_cast<NSLigneView*>(pNewHitToon) ;
	if (pLine)
	{
    NVLdVPoint logicalPoint = getAreaLogicalPointFromWindowPoint(curPos) ;

    NSConcernView *pConcernView = dynamic_cast<NSConcernView*>(pNewHitToon) ;
	  if (pConcernView)
	  {
      NSConcern* pConcern = pConcernView->getConcern() ;
      if (pConcern && pConcern->getQualityOfLifeChartAtTime(logicalPoint.getX()))
      {
        if (_pMagnifier && (false == _bMustCloseMagnifier))
          return ;

        if (_pMagnifier)
          closeMagnifier() ;

        _pMagnifier = new NSLdvViewMagnifier(pContexte, this, logicalPoint, pLine) ;

        int iMagnifierWindowLeft  = curPos.X() - _pMagnifier->getMagnifiedWidth() / 2 ;
        int iMagnifierWindowRight = curPos.Y() - _pMagnifier->getMagnifiedHeight() / 2 ;
        _pMagnifier->SetWindowPos(HWindow, iMagnifierWindowLeft, iMagnifierWindowRight, _pMagnifier->getMagnifiedWidth(), _pMagnifier->getMagnifiedHeight(), SWP_NOZORDER) ;

        _pMagnifier->Create() ;
      }
    }
  }
}

void
NSLdvViewAreaWindow::showMultitips(NSLdvObjetView* pObject, NSLdvToon* pFirstHitToon, NS_CLASSLIB::TPoint curPos)
{
try
{
  if ((NULL == pObject) || (NULL == pFirstHitToon))
    return ;

  // Make sure there is a single Multitips window opened at a time
  //
  if (_pMultiTips)
    closeMultiTips() ;

  _pMultiTips = new NSLdvViewIconsShowWindow(pContexte, this) ;
  _pMultiTips->addObject(pObject) ;

  int iObjNb = 1 ;

  // Get all documents whose icons are currently under cursor
  //
  NS_CLASSLIB::TRect firstToonRect = pFirstHitToon->donneVisibleRectangle() ;

  NSLdvToon* pOtherToon = nextHitTest(curPos, pFirstHitToon) ;
  while (pOtherToon)
  {
    if (dynamic_cast<NSLdvObjetView *>(pOtherToon))
    {
      NSLdvObjetView *pOtherObject = (NSLdvObjetView *) pOtherToon ;
      _pMultiTips->addObject(pOtherObject) ;
      iObjNb++ ;
    }
    pOtherToon = nextHitTest(curPos, pOtherToon) ;
  }

  // Organize Multitips window size and position
  //
  int iClientWidth = _pMultiTips->getDiameter() ;

  // int iClientHeight = 40 ;
  int iNeededSlotForHeight = iObjNb / 2 ;
  if (2 * iNeededSlotForHeight < iObjNb)
    iNeededSlotForHeight += 1 ;
  int iClientHeight = iNeededSlotForHeight * ICONWIDTH  ; // + (iNeededSlotForHeight - 1) * 2 ;

  int iTipsWindowLeft = firstToonRect.Left() ; // + (firstToonRect.Width() / 2) ; // curPos.X() - iClientWidth / 2

  int iTipsWindowVisibleBottom = firstToonRect.Top() - 2 ;                // must be above icon
  int iTipsWindowBottomMargin  = curPos.Y() - iTipsWindowVisibleBottom + 1 ;  // cursor must be inside window
  int iTipsWindowTop           = iTipsWindowVisibleBottom - iClientHeight ;  // curPos.Y() - iClientHeight + 2

  iClientHeight += iTipsWindowBottomMargin ;

  _pMultiTips->SetWindowPos(HWindow, iTipsWindowLeft, iTipsWindowTop, iClientWidth, iClientHeight, SWP_NOZORDER) ;

  _pMultiTips->setLeftSideWidth(iClientWidth / 2) ;
  _pMultiTips->setRightSideWidth(iClientWidth - _pMultiTips->getLeftSideWidth()) ;
  _pMultiTips->setBottomMargin(iTipsWindowBottomMargin) ;

  _pMultiTips->Create() ;

/*
  int iExpectedWidthAfterDrawing = _pMultiTips->getExpectedLeftSideWidth() +
                                   _pMultiTips->getExpectedRightSideWidth() ;

  if (iExpectedWidthAfterDrawing > iClientWidth)
  {
    int iExpectedLeftWidth  = _pMultiTips->getExpectedLeftSideWidth() ;
    int iExpectedRightWidth = _pMultiTips->getExpectedRightSideWidth() ;

    _pMultiTips->SetWindowPos(HWindow, curPos.X() - iExpectedLeftWidth, curPos.Y() - iClientHeight + 2, iExpectedWidthAfterDrawing, iClientHeight, SWP_NOZORDER) ;

    _pMultiTips->setLeftSideWidth(iExpectedLeftWidth) ;
    _pMultiTips->setRightSideWidth(iExpectedRightWidth) ;

    _pMultiTips->Invalidate() ;
  }
*/
}
catch (...)
{
  string sErrorText = string("Exception NSLdvViewAreaWindow::showMultitips.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

NS_CLASSLIB::TPoint
NSLdvViewAreaWindow::getAreaPhysicalPoint(NVLdVPoint ldvPoint)
{
	// On place le point en bas à droite
	// Instantiating the point as clientRect bottom right point
  //
  NS_CLASSLIB::TRect clientRect = GetClientRect() ;
	NS_CLASSLIB::TPoint physicalPoint = clientRect.BottomRight() ;

	// Translation du point
	// Translating the point
	NVLdVRect ldvRect(getView(),
										ldvPoint.getX(),
										getView()->getDateHeureInfDroit(),
										0, // lPosInfDroit,
										ldvPoint.getY()) ;

  adaptPhysicalPointToChronoLine(physicalPoint) ;

	physicalPoint.x -= getView()->getPhysicalWidthFromTimeUnit(ldvRect.Width()) ;

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
NSLdvViewAreaWindow::getAreaPhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getAreaPhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getAreaPhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

NS_CLASSLIB::TRect
NSLdvViewAreaWindow::getAreaPhysicalRect(NS_CLASSLIB::TRect ldvWinRect)
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

NS_CLASSLIB::TPoint
NSLdvViewAreaWindow::getScrollablePhysicalPoint(NVLdVPoint ldvPoint)
{
	// On place le point en bas à droite
	// Instantiating the point as clientRect bottom right point
  //
	NS_CLASSLIB::TRect  clientRect    = GetClientRect() ;
	NS_CLASSLIB::TPoint physicalPoint = clientRect.BottomRight() ;

	// Translation du point
	// Translating the point
	NVLdVRect ldvRect(getView(),
										ldvPoint.getX(),
										getView()->getDateHeureInfDroit(),
										_lPosInfDroit,
										ldvPoint.getY()) ;

  adaptPhysicalPointToChronoLine(physicalPoint) ;

	physicalPoint.x -= getView()->getPhysicalWidthFromTimeUnit(ldvRect.Width()) ;

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
NSLdvViewAreaWindow::getScrollablePhysicalRect(NVLdVRect ldvRect)
{
	NS_CLASSLIB::TPoint ptTopLeft = getScrollablePhysicalPoint(ldvRect.TopLeft()) ;
	NS_CLASSLIB::TPoint ptBotRigh = getScrollablePhysicalPoint(ldvRect.BottomRight()) ;

  return NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
}

/*
** inboxPoint is the point that was calculated supposing that NSLdvViewAreaWindow
** and ChronoLine share the same right border. But, for example, it is not the
** case when a Vertical scroll appears in NSLdvViewAreaWindow. We have to
** move inboxPoint accordingly
*/
void
NSLdvViewAreaWindow::adaptPhysicalPointToChronoLine(NS_CLASSLIB::TPoint &inboxPoint)
{
  inboxPoint.x += adjustPhysicalPointToChronoLine() ;
}

int
NSLdvViewAreaWindow::adjustPhysicalPointToChronoLine()
{
  NSLdvChronoLine* pChronoLine = getView()->getLdVChronoLine() ;
  if ((NULL == pChronoLine) && (NULL == pChronoLine->getInterface()))
    return false ;

  NS_CLASSLIB::TRect chronoClientRect = pChronoLine->getInterface()->GetClientRect() ;
  NS_CLASSLIB::TRect clientRect       = GetClientRect() ;

  return chronoClientRect.Width() - clientRect.Width() ;
}

NVLdVPoint
NSLdvViewAreaWindow::getAreaLogicalPointFromWindowPoint(NS_CLASSLIB::TPoint winPoint)
{
  NVLdVPoint resultPt(getView()) ;

  NS_CLASSLIB::TRect clientRect = GetClientRect() ;
  NS_CLASSLIB::TPoint ptOrigine = clientRect.BottomRight() ;

  int iPhysicalDistance = winPoint.X() - ptOrigine.X() ;
  iPhysicalDistance -= adjustPhysicalPointToChronoLine() ;

  NVLdVTemps tX = getView()->getDateHeureInfDroit() ;
	//
	// Calcul de la différence en pixels sur l'axe des X (temps)
	//
	int iDelta = getView()->getTimeUnitFromPhysicalWidth(iPhysicalDistance) ;
	//
	// Conversion en AAAAMMJJhhmmss
	//
	switch (getView()->getXunit())
	{
		case pixAns :
			tX.ajouteAns(iDelta) ;
			break ;
		case pixMois :
			tX.ajouteMois(iDelta) ;
			break ;
		case pixSemaine :
			tX.ajouteJours(iDelta * 7) ;
			break ;
		case pixJour :
			tX.ajouteJours(iDelta) ;
			break ;
		case pixHeure :
			tX.ajouteHeures(iDelta) ;
			break ;
		case pixMinute :
			tX.ajouteMinutes(iDelta) ;
			break ;
		case pixSeconde :
			tX.ajouteSecondes(iDelta) ;
			break ;
	}
  resultPt.setX(tX) ;

	//
	// Calcul de la différence en pixels sur l'axe des Y
	//
	long dY = long(ptOrigine.Y() - winPoint.Y()) ;

  // Baseline doesn't scroll
  //
  if (dY > getPixelPosAboveBaseline(0))
    dY += _pToon->getPosInfDroit() ;

  resultPt.setY(dY) ;

  return resultPt ;
}

NVLdVRect
NSLdvViewAreaWindow::getAreaLogicalRectFromWindowRect(NS_CLASSLIB::TRect scrRect)
{
  // fixme : if a point is in Baseline (no scroll) and another above (scroll),
  //         then there is a problem

  NVLdVPoint ptTopLeft = getAreaLogicalPointFromWindowPoint(scrRect.TopLeft()) ;
	NVLdVPoint ptBotRigh = getAreaLogicalPointFromWindowPoint(scrRect.BottomRight()) ;

  return NVLdVRect(getView(), &ptTopLeft, &ptBotRigh) ;
}

void
NSLdvViewAreaWindow::increasePosInfDroit(int iDeltaPos)
{
	if (0 == iDeltaPos)
		return ;
	if ((0 >= _lPosInfDroit) && (iDeltaPos < 0))
		return ;

	int iMaxPos = _pToon->getGlobalBox()->Height() - getWorkingSpaceHeight() ;
  if ((iMaxPos <= _lPosInfDroit) && (iDeltaPos > 0))
		return ;

	if (_lPosInfDroit + iDeltaPos <= 0)
  	_lPosInfDroit = 0 ;
	else if (_lPosInfDroit + iDeltaPos >= iMaxPos)
  	_lPosInfDroit = iMaxPos ;
  else
  	_lPosInfDroit += iDeltaPos ;

  setScrollParams() ;

	InvalidateRect(getWorkingSpaceRect(), true) ;

	return ;
}

void
NSLdvViewAreaWindow::setPosInfDroit(long lNewPosition)
{
	if (lNewPosition < 0)
  	_lPosInfDroit = 0 ;
	else if (lNewPosition > _pToon->getGlobalBox()->Height() - getWorkingSpaceHeight())
  	_lPosInfDroit = _pToon->getGlobalBox()->Height() - getWorkingSpaceHeight() ;
  else
  	_lPosInfDroit = lNewPosition ;

  setScrollParams() ;

	Invalidate() ;
	return ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceTop()
{
	NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Top() ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceBottom()
{
	NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Bottom() ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceRight()
{
	NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Right() ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceLeft()
{
	NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Left() ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceHeight()
{
  NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Height() ;
}

int
NSLdvViewAreaWindow::getWorkingSpaceWidth()
{
  NS_CLASSLIB::TRect clientRect = getWorkingSpaceRect() ;
	return clientRect.Width() ;
}

NS_CLASSLIB::TRect
NSLdvViewAreaWindow::getWorkingSpaceRect()
{
	NS_CLASSLIB::TRect LdvFrameRect = getClientRectAboveBaseline() ;
  int iClientRectHeight = GetClientRect().Height() ;

	return NS_CLASSLIB::TRect(LdvFrameRect.Left(),
                            iClientRectHeight - LdvFrameRect.Bottom(),
                            LdvFrameRect.Right(),
                            iClientRectHeight - LdvFrameRect.Top()) ;
}

NSHISTODocument*
NSLdvViewAreaWindow::getDocumentsManager()
{
  if (NULL == pContexte)
    return (NSHISTODocument*) 0 ;

  NSPatientChoisi *pPatient = pContexte->getPatient() ;
  if (NULL == pPatient)
    return (NSHISTODocument*) 0 ;

  return pPatient->getDocHis() ;
}

NSDocumentInfo*
NSLdvViewAreaWindow::getDocumentInfo(string sDocCode)
{
  if (string("") == sDocCode)
    return (NSDocumentInfo*) 0 ;

  // Get document in document manager
  //
  NSHISTODocument* pHistoDoc = getDocumentsManager() ;
  if (NULL == pHistoDoc)
    return (NSDocumentInfo*) 0 ;

  DocumentIter docIter = pHistoDoc->TrouveDocHisto(sDocCode) ;
  if (NULL == docIter)
    return (NSDocumentInfo*) 0 ;

  NSDocumentHisto* pDocHis = *docIter ;
  if (NULL == pDocHis)
    return (NSDocumentInfo*) 0 ;

  return (NSDocumentInfo*) pDocHis ;
}

NSDocumentInfo*
NSLdvViewAreaWindow::getDocumentInfo(NSLdvObjetView* pObjectView)
{
  if (NULL == pObjectView)
    return (NSDocumentInfo*) 0 ;

  NSLdvObjet* pLdvObject = pObjectView->getObject() ;
  if (NULL == pLdvObject)
    return (NSDocumentInfo*) 0 ;

  string sDocCode = pLdvObject->getReference() ;
  if (string("") == sDocCode)
    return (NSDocumentInfo*) 0 ;

  return getDocumentInfo(sDocCode) ;
}

TWindow*
NSLdvViewAreaWindow::getDocViewWindow(NSNoyauDocument *pDocument)
{
  if (NULL == pDocument)
    return (TWindow*) 0 ;

  TView* pView = pDocument->GetViewList() ;
  if (NULL == pView)
  	return (TWindow*) 0 ;

	do
	{
		TWindowView* pWindowsView = TYPESAFE_DOWNCAST(pView, TWindowView) ;
		if (pWindowsView)
			return (TWindow*) pWindowsView ;

		pView = pDocument->NextView(pView) ;
	}
  while (pView) ;

  return (TWindow*) 0 ;
}

hitToonStatus
NSLdvViewAreaWindow::openObject(NSLdvObjetView *pObject)
{
  // If no object or object that was already opened, nothing to do
  //
  if ((NSLdvObjetView*) NULL == pObject)
    return statusNone ;

  // Get Documents manager
  //
  NSHISTODocument* pDocHis = getDocumentsManager() ;
  if ((NSHISTODocument*) NULL == pDocHis)
    return statusNone ;

  // Get open object in documents manager
  //
  NSDocumentInfo* pDocInfo = getDocumentInfo(pObject) ;
  if ((NSDocumentInfo*) NULL == pDocInfo)
    return statusNone ;

  // Check if document is already opened
  //
  NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(pDocInfo) ;

  // Ask document manager to open document (or put it front if already opened)
  //
  pDocHis->AutoriserOuverture(pDocInfo) ;

  if (pDocNoy)
    return statusPutFront ;
  else
    return statusOpening ;
}

void
NSLdvViewAreaWindow::setPosForActiveObject()
{
  // If no object or object that was already opened, nothing to do
  //
  if ((NULL == _pHitToon) || (statusPutFront == _hitToonStatus) || (statusOpened == _hitToonStatus))
    return ;

  // Get Documents manager
  //
  NSHISTODocument* pDocHis = getDocumentsManager() ;
  if (NULL == pDocHis)
    return ;

  // Get open object in documents manager
  //
  NSDocumentInfo* pDocInfo = getDocumentInfo((NSLdvObjetView *) _pHitToon) ;
  if (NULL == pDocInfo)
    return ;

  // Check if document is already opened
  //
  NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(pDocInfo) ;

  TWindow* pOpenedWindows = getDocViewWindow(pDocNoy) ;
  if ((TWindow*) NULL == pOpenedWindows)
    return ;

  while (pOpenedWindows)
  {
    TMDIChild *pMdiChild = TYPESAFE_DOWNCAST(pOpenedWindows, TMDIChild) ;
		if (pMdiChild)
    {
      setPosForActiveObjectWindow(pMdiChild) ;
      return ;
    }

    pOpenedWindows = pOpenedWindows->Parent ;
  }
}

void
NSLdvViewAreaWindow::setPosForActiveObjectWindow(TWindow* pMdiWindow)
{
  if ((TWindow*) NULL == pMdiWindow)
    return ;

  // Get screen coordinates for icon
  //
  NS_CLASSLIB::TRect iconsWinRect = _pHitToon->donneVisibleRectangle() ;
  NS_CLASSLIB::TRect viewAreaRect = GetWindowRect() ;
  iconsWinRect = iconsWinRect.OffsetBy(viewAreaRect.X(), viewAreaRect.Y()) ;

  NS_CLASSLIB::TRect viewWinRect  = pMdiWindow->GetWindowRect() ;
  if (viewWinRect.Touches(iconsWinRect))
  {
    int iNewX = viewWinRect.X() ;
    int iNewY = iconsWinRect.Y() - viewWinRect.Height() ;
    int iNewH = 200 ;  // viewWinRect.Height() ;
    int iNewW = 500 ; // viewWinRect.Width() ;
    if (iNewY < 0)
    {
      iNewY = 5 ;
      iNewH = iconsWinRect.Y() - iNewY ;
    }
    pMdiWindow->MoveWindow(iNewX, iNewY, iNewW, iNewH, true) ;
  }

  _hitToonStatus = statusOpened ;
}

void
NSLdvViewAreaWindow::closeOpenObject()
{
  // If no object or object that was not opened, nothing to do
  //
  if (NULL == _pHitToon)
    return ;

  if (statusOpened != _hitToonStatus)
    return ;

  // Get Documents manager
  //
  NSHISTODocument* pDocHis = getDocumentsManager() ;
  if (NULL == pDocHis)
    return ;

  // Get open object in documents manager
  //
  NSDocumentInfo* pDocInfo = getDocumentInfo((NSLdvObjetView *) _pHitToon) ;
  if (NULL == pDocInfo)
    return ;

  // Get opened document
  //
  NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(pDocInfo) ;
  if (NULL == pDocNoy)
    return ;

  TWindow* pOpenedWindows = getDocViewWindow(pDocNoy) ;
  if (NULL == pOpenedWindows)
    return ;

  pOpenedWindows->CloseWindow() ;

  pDocHis->FermetureDocument(pDocInfo) ;
}

// -----------------------------------------------------------------------------
// --------------------- METHODES DE NSTimeZoomLevel ---------------------------
// -----------------------------------------------------------------------------

NSTimeZoomLevel::NSTimeZoomLevel(pixUnit iPixUnit, int iNbUPP, int iLowerUPP, int iUpperUPP)
{
  lObjectCount++ ;

	_iPixUnit                   = iPixUnit ;
	_iNumberOfUnitPerPixel      = iNbUPP ;
	_iUpperLimitForUnitPerPixel = iUpperUPP ;
	_iLowerLimitForUnitPerPixel = iLowerUPP ;
}

NSTimeZoomLevel::~NSTimeZoomLevel()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// ------------------------ METHODES DE NSLdvToon ------------------------------
// -----------------------------------------------------------------------------

NSLdvToon::NSLdvToon(NSContexte* pCtx, NSLdvView* pTheView)	        :NSRoot(pCtx)
{
  lCoreObjectCount++ ;

	_pView    = pTheView ;

	_sLibelle = string("") ;
	_iZOrder  = 0 ;

  _bVisible = true ;

  _bInterceptRButtonDown = true ;
}

NSLdvToon::NSLdvToon(const NSLdvToon& rv)
          :NSRoot(rv.pContexte)
{
  lCoreObjectCount++ ;

	initialiser(&rv) ;
}

void
NSLdvToon::initialiser(const NSLdvToon* pSrc)
{
	_pView        = pSrc->_pView ;

	_sLibelle     = pSrc->_sLibelle ;
	_iZOrder      = pSrc->_iZOrder ;
	_toonType     = pSrc->_toonType ;
  _toonCategory = pSrc->_toonCategory ;
  _bVisible     = pSrc->_bVisible ;

  _bInterceptRButtonDown = pSrc->_bInterceptRButtonDown ;
}

NSLdvToon&
NSLdvToon::operator=(const NSLdvToon& src)
{
	if (this == &src)
		return *this ;

	initialiser(&src) ;

	return *this ;
}

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvTimeToon ----------------------------
// -----------------------------------------------------------------------------

NSLdvTimeToon::NSLdvTimeToon(NSContexte* pCtx, NSLdvViewArea *pWorkArea, bool bVScrollEnabled)
              :NSLdvToon(pCtx, pWorkArea->getView()), _Box(pWorkArea->getView()), _VisibleBox(pWorkArea->getView())
{
  lLocalObjectCount++ ;

	_toonCategory           = timeToon ;

	_iFixedWidthInPixels    = 0 ;
  _bVerticalScrollEnabled = bVScrollEnabled ;
  _pWorkingArea           = pWorkArea ;
}


NSLdvTimeToon::NSLdvTimeToon(const NSLdvTimeToon& rv)
              :NSLdvToon(rv.pContexte, rv._pView), _Box(rv._pView), _VisibleBox(rv._pView)
{
  lLocalObjectCount++ ;

	initialiser(&rv) ;
}

void
NSLdvTimeToon::initialiser(const NSLdvTimeToon* pSrc)
{
  NSLdvToon::initialiser((NSLdvToon*)pSrc) ;

	_Box         = pSrc->_Box ;
	_VisibleBox  = pSrc->_VisibleBox ;

  _iFixedWidthInPixels    = pSrc->_iFixedWidthInPixels ;
  _bVerticalScrollEnabled = pSrc->_bVerticalScrollEnabled ;
  _pWorkingArea           = pSrc->_pWorkingArea ;
}

NSLdvTimeToon&
NSLdvTimeToon::operator=(const NSLdvTimeToon& src)
{
	if (this == &src)
		return *this ;

	initialiser(&src);

	return *this;
}

void
NSLdvTimeToon::fixeRectangle(	NVLdVTemps tDeb,
													NVLdVTemps tFin,
													long lPlancher,
													long lPlafond)
{
	_Box.initialise(tDeb, tFin, lPlancher, lPlafond) ;
}

NS_CLASSLIB::TRect
NSLdvTimeToon::donneRectangle()
{
	NVLdVPoint ldvTL = _Box.TopLeft() ;
	NVLdVPoint ldvBR = _Box.BottomRight() ;

  NS_CLASSLIB::TPoint ptTL ;
  NS_CLASSLIB::TPoint ptBR ;

	if (_bVerticalScrollEnabled)
	{
  	ptTL = _pWorkingArea->getScrollablePhysicalPoint(ldvTL) ;
  	ptBR = _pWorkingArea->getScrollablePhysicalPoint(ldvBR) ;
	}
  else
	{
  	ptTL = _pWorkingArea->getAreaPhysicalPoint(ldvTL) ;
  	ptBR = _pWorkingArea->getAreaPhysicalPoint(ldvBR) ;
  }

	return NS_CLASSLIB::TRect(ptTL, ptBR) ;
}

NS_CLASSLIB::TRect
NSLdvTimeToon::donneVisibleRectangle()
{
	NVLdVPoint ldvTL = _VisibleBox.TopLeft() ;
	NVLdVPoint ldvBR = _VisibleBox.BottomRight() ;

  NS_CLASSLIB::TPoint ptTL ;
	NS_CLASSLIB::TPoint ptBR ;

	if (_bVerticalScrollEnabled)
	{
		ptTL = _pWorkingArea->getScrollablePhysicalPoint(ldvTL) ;
  	ptBR = _pWorkingArea->getScrollablePhysicalPoint(ldvBR) ;
	}
  else
	{
  	ptTL = _pWorkingArea->getAreaPhysicalPoint(ldvTL) ;
  	ptBR = _pWorkingArea->getAreaPhysicalPoint(ldvBR) ;
  }

	return NS_CLASSLIB::TRect(ptTL, ptBR) ;
}

void
NSLdvTimeToon::donneRectangle(NVLdVTemps* ptDeb,
													NVLdVTemps* ptFin,
													long* plPlancher,
													long* plPlafond)
{
	*ptDeb      = _Box.getLeft() ;
	*ptFin      = _Box.getRight() ;
	*plPlancher = _Box.getTop() ;
	*plPlafond  = _Box.getBottom() ;
}

// If an object has a fixed size, for example an icon, its NVLdVRect width
// depends on the time unit scale, so, we have to adjust it each time the
// time unit changes
//
void
NSLdvTimeToon::adjustToPixelTimeUnit()
{
	if (_iFixedWidthInPixels <= 0)
		return ;

	NS_CLASSLIB::TRect pixelRect ;
	if (_bVerticalScrollEnabled)
		pixelRect = _pWorkingArea->getScrollablePhysicalRect(_Box) ;
  else
  	pixelRect = _pWorkingArea->getAreaPhysicalRect(_Box) ;

  int iExcedingPixels = _iFixedWidthInPixels - pixelRect.Width() ;

  if (iExcedingPixels <= 0)
  {
  	_VisibleBox = _Box ;
    return ;
  }

  int iExcedingPixelsLeft  = iExcedingPixels / 2 ;
  int iExcedingPixelsRight = iExcedingPixels - iExcedingPixelsLeft ;

  string sPixelUnitCode = _pView->getLdvPixLexiqueCode() ;
  if (sPixelUnitCode == string(""))
		return ;

  NVLdVTemps startingTime = _Box.getLeft() ;
  if (iExcedingPixelsLeft != 0)
  	startingTime.ajouteTemps(-iExcedingPixelsLeft, sPixelUnitCode, pContexte) ;

  NVLdVTemps endingTime   = _Box.getRight() ;
  if (iExcedingPixelsRight != 0)
  	endingTime.ajouteTemps(iExcedingPixelsRight, sPixelUnitCode, pContexte) ;

  _VisibleBox.setLeft(startingTime) ;
  _VisibleBox.setRight(endingTime) ;
}

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSPbModifView ----------------------------// -----------------------------------------------------------------------------

NSPbModifView::NSPbModifView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcernModifier* pModif)              :NSLdvTimeToon(pCtx, pWorkArea, true)

{
  lObjectCount++ ;

	pModifier = pModif;

	_Box.setLeft(pModifier->tDateHeureDeb);
	_Box.setRight(pModifier->tDateHeureFin);

	_iZOrder  = LEVEL_PREOCCUP + RELPRLEVEL_PBEVENT;
	_toonType = toonConcernEvent;
}

NSPbModifView::NSPbModifView(const NSPbModifView& rv)
	            :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv));
}

NSPbModifView::~NSPbModifView()
{
  lObjectCount-- ;
}

void
NSPbModifView::SetupWindow()
{
}

void
NSPbModifView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
}

void
NSPbModifView::EvRButtonDown()
{
}

void
NSPbModifView::createComponents(ArrayOfToons* pToonsArray)
{
}

NSPbModifView&
NSPbModifView::operator=(const NSPbModifView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src));

  return *this;
}

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSConcernView ----------------------------
// -----------------------------------------------------------------------------

NSConcernView::NSConcernView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, LDVFRAME iFrame, int iIdx /*= -1*/)
              :NSLigneView(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_pConcern = pPb ;
  _iFrame   = iFrame ;

	// Recherche de la ligne capable d'héberger cette préoccupation de santé
	_iIndex = iIdx ;

	if (pWorkArea->getLargeurLigne() >= 30)
	{
		_iLeftMargin   = 2 ;
		_iRigthMargin  = 2 ;
		_iTopMargin    = 2 ;
		_iBottomMargin = 2 ;
	}
	else
	{
		_iLeftMargin   = 1 ;
		_iRigthMargin  = 1 ;
		_iTopMargin    = 1 ;
		_iBottomMargin = 1 ;
	}

	_Box.setLeft(_pConcern->_tDateOuverture) ;

	if (false == _pConcern->_tDateFermeture.estVide())
		_Box.setRight(_pConcern->_tDateFermeture) ;
	else
	{
		NVLdVTemps tNoLimit ;
		tNoLimit.setNoLimit() ;
		_Box.setRight(tNoLimit) ;
	}

	_iZOrder  = LEVEL_PREOCCUP + RELPRLEVEL_LINE ;
	_toonType = toonConcern ;

/*
	pConcern = pPb;

	Box.setLeft(pConcern->tDateDebut);

	if (pConcern->tDateFermeture.estVide())
		Box.setRight(pConcern->tDateFermeture);
	else
		if (!(pConcern->tDateAutoClose.estVide()))
			Box.setRight(pConcern->tDateAutoClose);
		else
		{
			NVLdVTemps tNoLimit;
			tNoLimit.setNoLimit();
			Box.setRight(tNoLimit);
		}

	iZOrder  = LEVEL_PREOCCUP + RELPRLEVEL_PROBLEM;
	toonType = toonConcern;
*/
}

NSConcernView::NSConcernView(const NSConcernView& rv)
              :NSLigneView(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv)) ;

	_pConcern      = rv._pConcern ;
  _iFrame        = rv._iFrame ;
	_iIndex        = rv._iIndex ;
	_iLeftMargin   = rv._iLeftMargin ;
	_iRigthMargin  = rv._iRigthMargin ;
	_iTopMargin    = rv._iTopMargin ;
	_iBottomMargin = rv._iBottomMargin ;
}

NSConcernView::~NSConcernView()
{
  lObjectCount-- ;
}

void
NSConcernView::SetupWindow()
{
}

void
NSConcernView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
try
{
	OWL::TBrush* pPinceau ;
	OWL::TPen*   pPenEclaire ;
	OWL::TPen*   pPenOmbre ;
	OWL::TPen*   pPenTransition ;

	// Sauvegarde des éléments en cours (pen et brush)
	HPEN   oldPen   = (HPEN)   pDc->GetCurrentObject(OBJ_PEN) ;
	HBRUSH oldBrush = (HBRUSH) pDc->GetCurrentObject(OBJ_BRUSH) ;

  initPens(&pPinceau, &pPenEclaire, &pPenOmbre, &pPenTransition) ;

	// Rectangle à repeindre pour l'objet lui même
	// Object's rectangle to repaint
	NVLdVPoint ldvTopLeft(_pView) ;

	// Left time (either left side of repaint rect or left side of concern)
  //
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (_pConcern->_tDateOuverture > tLeftTime)
		tLeftTime = _pConcern->_tDateOuverture ;

	ldvTopLeft.setX(tLeftTime) ;
	ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_iIndex)) ;

	NVLdVPoint ldvBottomRight(_pView);

	// Right time (either right side of repaint rect or right side of concern)
  //
	NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
	if (_pConcern->_tDateFermeture < tRightTime)
		tRightTime = _pConcern->_tDateFermeture ;

	ldvBottomRight.setX(tRightTime) ;
	ldvBottomRight.setY(_pWorkingArea->preoccupDonneBottomLine(_iIndex)) ;

	NS_CLASSLIB::TPoint ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pWorkingArea->getScrollablePhysicalPoint(ldvBottomRight) ;

	// On colore le fond
	// Filling the rectangle
  if (ldvframeRisk != _iFrame)
	  pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y, *pPinceau) ;

	delete pPinceau ;

	// On fixe la visibleBox
  if (bGlobal)
  {
	  NS_CLASSLIB::TRect PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
	  _VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;
  }

	if (false == _pConcern->_aModificateurs.empty())
	{
		for (ArrayModifierIter ModIt = _pConcern->_aModificateurs.begin() ;
						_pConcern->_aModificateurs.end() != ModIt ;
						ModIt++)
		{
			// Modificateur situé au moins en partie dans le rectangle en cours
			if (((*ModIt)->tDateHeureDeb < tRightTime) && ((*ModIt)->tDateHeureFin > tLeftTime))
			{
				NVLdVPoint severityTopLeft(ldvTopLeft) ;
				NVLdVPoint severityBottomRight(ldvBottomRight) ;
				if ((*ModIt)->tDateHeureDeb > tLeftTime)
					severityTopLeft.setX((*ModIt)->tDateHeureDeb) ;
				if ((*ModIt)->tDateHeureFin < tRightTime)
					severityBottomRight.setX((*ModIt)->tDateHeureFin) ;

				NS_CLASSLIB::TPoint ptTL = _pWorkingArea->getScrollablePhysicalPoint(severityTopLeft) ;
				NS_CLASSLIB::TPoint ptBR = _pWorkingArea->getScrollablePhysicalPoint(severityBottomRight) ;

				//OWL::TBrush PinceauS1(NS_CLASSLIB::TColor::LtCyan);
				//OWL::TBrush PinceauS2(NS_CLASSLIB::TColor::LtGreen);
				//OWL::TBrush PinceauS3(NS_CLASSLIB::TColor::LtYellow);
				//OWL::TBrush PinceauS4(NS_CLASSLIB::TColor::LtRed);

				if ((*ModIt)->iSeverite > 0)
				{
          pPinceau = getBrushForSeverity((*ModIt)->iSeverite) ;

          if (pPinceau)
          {
					  pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
					  delete pPinceau ;
          }
				}
				if ((*ModIt)->iRisque > 0)
				{
                    /*
					ptTL.y = ptBR.y + (ptTL.y - ptBR.y) / 2 + 1;
					ptBR.y = ptTL.y - 3;

					if ((*ModIt)->iRisque < 10)
						pPinceau = new OWL::TBrush(pView->pPinceauS1->GetHandle());
					else
						if ((*ModIt)->iRisque < 15)
							pPinceau = new OWL::TBrush(pView->pPinceauS2->GetHandle());
						else
							if ((*ModIt)->iRisque < 20)
								pPinceau = new OWL::TBrush(pView->pPinceauS3->GetHandle());
							else
								pPinceau = new OWL::TBrush(pView->pPinceauS4->GetHandle());

					pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau);

					delete pPinceau;

					// pDc->SelectObject(*pPenEclaire);
					::SelectObject(pDc->GetHDC(), *pPenEclaire);
					pDc->MoveTo(ptTL.x, ptTL.y);
					pDc->LineTo(ptBR.x, ptTL.y);

					// pDc->SelectObject(*pPenOmbre);
					::SelectObject(pDc->GetHDC(), *pPenOmbre);
					pDc->MoveTo(ptTL.x, ptBR.y - 1);
					pDc->LineTo(ptBR.x, ptBR.y - 1);
                    */
				}
			}
		}
	}

  // Drawing Quality of Life scores
  //
  NSQualityOfLifeChartArray* pQoLarray = _pConcern->getQualityOfLifeCharts() ;
  if (false == pQoLarray->empty())
  {
    for (QualityOfLifeChartIter chartIt = pQoLarray->begin() ; pQoLarray->end() != chartIt ; chartIt++)
    {
      // QoL span time is within the area that needs redraw
      //
      if (((*chartIt)->getTpsInfo() > tLeftTime) && ((*chartIt)->getTpsStart() < tRightTime))
      {
        drawQolPattern(pDc, pRectARepeindre, *chartIt, &ldvTopLeft, &ldvBottomRight) ;

/*
        if ((*chartIt)->getMainScore() >= 0)
        {
          // HPEN currentPen = (HPEN) pDc->GetCurrentObject(OBJ_PEN) ;

          OWL::TPen* pPenForScore = getPenForScore((*chartIt)->getMainScore()) ;
          ::SelectObject(pDc->GetHDC(), *pPenForScore) ;

          NVLdVPoint QofL_TopLeft(ldvTopLeft) ;
				  NVLdVPoint QofL_BottomRight(ldvBottomRight) ;
				  if ((*chartIt)->getTpsStart() > tLeftTime)
					  QofL_TopLeft.setX((*chartIt)->getTpsStart()) ;
				  if ((*chartIt)->getTpsInfo() < tRightTime)
					  QofL_BottomRight.setX((*chartIt)->getTpsInfo()) ;

				  NS_CLASSLIB::TPoint ptQoL_TL = _pWorkingArea->getScrollablePhysicalPoint(QofL_TopLeft) ;
				  NS_CLASSLIB::TPoint ptQoL_BR = _pWorkingArea->getScrollablePhysicalPoint(QofL_BottomRight) ;

          pDc->MoveTo(ptQoL_TL.x, ptQoL_TL.y) ;
			    pDc->LineTo(ptQoL_BR.x, ptQoL_TL.y) ;
          pDc->LineTo(ptQoL_BR.x, ptQoL_BR.y) ;
          pDc->LineTo(ptQoL_TL.x, ptQoL_BR.y) ;
          pDc->LineTo(ptQoL_TL.x, ptQoL_TL.y) ;

          int iY = ptQoL_TL.y + 4 ;
          while (iY < ptQoL_BR.y)
          {
            pDc->MoveTo(ptQoL_TL.x, iY) ;
            pDc->LineTo(ptQoL_BR.x, iY) ;
            iY += 4 ;
          }
        }
*/
      }
    }
  }

	// Décorations 3D
	// 3D borders
  //
  if (ldvframeRisk != _iFrame)
    draw3DBorders(pDc, pRectARepeindre, pPenEclaire, pPenOmbre, pPenTransition, &ptTopLeft, &ptBotRigh) ;
  else
    draw3DBorders(pDc, pRectARepeindre, pPenTransition, pPenOmbre, pPenTransition, &ptTopLeft, &ptBotRigh) ;

	// affichage du titre
  //
  drawTitle(pDc, pRectARepeindre, &ptTopLeft) ;

	// -----------------------------------------------------------------------
	// Affichage des objectifs
	// -----------------------------------------------------------------------

  drawGoals(pDc, pRectARepeindre, pPenEclaire, pPenOmbre, pPenTransition, &ldvTopLeft, &ldvBottomRight, &tLeftTime, &tRightTime) ;

	// drawBaddestGoal(pDc, pRectARepeindre) ;


	// ---------------------------------------------------------------------------
	// Restauration des objets initiaux (pen et brush)
	::SelectObject(pDc->GetHDC(), oldPen) ;
	::SelectObject(pDc->GetHDC(), oldBrush) ;
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSConcernView::drawQolPattern(TDC* pDc, NVLdVRect* pRectARepeindre, NSQualityOfLifeChart* pQolChart, NVLdVPoint* pLdvTopLeft, NVLdVPoint* pLdvBottomRight)
{
try
{
  if ((NULL == pDc) || (NULL == pQolChart))
    return ;

  if ((NULL == pLdvTopLeft) || (NULL == pLdvBottomRight) || (NULL == pRectARepeindre))
    return ;

  // Rectangle to draw QoL decorations into
  //
  NVLdVPoint QofL_TopLeft(*pLdvTopLeft) ;
  if (pQolChart->getTpsStart() > pRectARepeindre->getLeft())
    QofL_TopLeft.setX(pQolChart->getTpsStart()) ;
  QofL_TopLeft.setY(pLdvTopLeft->getY() - getQolTopMargin()) ;

  NVLdVPoint QofL_BottomRight(*pLdvBottomRight) ;
  if (pQolChart->getTpsInfo() < pRectARepeindre->getRight())
    QofL_BottomRight.setX(pQolChart->getTpsInfo()) ;
  QofL_BottomRight.setY(pLdvBottomRight->getY()) ;

  // Location of this rectangle inside working area
  //
  NS_CLASSLIB::TPoint ptQoL_TL = _pWorkingArea->getScrollablePhysicalPoint(QofL_TopLeft) ;
  NS_CLASSLIB::TPoint ptQoL_BR = _pWorkingArea->getScrollablePhysicalPoint(QofL_BottomRight) ;

  NSQualityOfLifeInformation* pQolInfo = pQolChart->getQolInfo() ;

  if (pQolInfo)
  {
    NSCoopLineArray* pLines = pQolInfo->getLines() ;
    if (pLines && (false == pLines->empty()))
    {
      ldvBrush* pBrush ;
      int iLineNb = 0 ;
      for (CoopLineIter lineIt = pLines->begin() ; pLines->end() != lineIt ; lineIt++, iLineNb++)
      {
        int iEquivScore = 0 ;
        switch ((*lineIt)->getLineColor())
        {
          case colorBlue   : iEquivScore = 1 ; break ;
          case colorGreen  : iEquivScore = 2 ; break ;
          case colorYellow : iEquivScore = 3 ; break ;
          case colorOrange : iEquivScore = 4 ; break ;
          case colorRed    : iEquivScore = 5 ; break ;
        }
        pBrush = getBrushForScore(iEquivScore) ;

        int iTopY = getYForQolLine(ptQoL_TL.y, iLineNb) ;

        pDc->FillRect(ptQoL_TL.x, iTopY, ptQoL_BR.x, iTopY + getQolLineHeight(), OWL::TBrush(pBrush->GetHandle())) ;
      }
      return ;
    }
  }

  if (pQolChart->getMainScore() >= 0)
  {
    // HPEN currentPen = (HPEN) pDc->GetCurrentObject(OBJ_PEN) ;

    OWL::TPen* pPenForScore = getPenForScore(pQolChart->getMainScore()) ;
    ::SelectObject(pDc->GetHDC(), *pPenForScore) ;

    pDc->MoveTo(ptQoL_TL.x, ptQoL_TL.y) ;
    pDc->LineTo(ptQoL_BR.x, ptQoL_TL.y) ;
    pDc->LineTo(ptQoL_BR.x, ptQoL_BR.y) ;
    pDc->LineTo(ptQoL_TL.x, ptQoL_BR.y) ;
    pDc->LineTo(ptQoL_TL.x, ptQoL_TL.y) ;

    int iY = ptQoL_TL.y + 4 ;
    while (iY < ptQoL_BR.y)
    {
      pDc->MoveTo(ptQoL_TL.x, iY) ;
      pDc->LineTo(ptQoL_BR.x, iY) ;
      iY += 4 ;
    }
  }
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::drawQolPattern.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

ldvBrush*
NSConcernView::getBrushForScore(int iScore)
{
  switch (iScore)
  {
    case 1 : return _pView->pPinceauS1 ;
    case 2 : return _pView->pPinceauS2 ;
    case 3 : return _pView->pPinceauS3 ;
    case 4 : return _pView->pPinceauS34 ;
    case 5 : return _pView->pPinceauS4 ;
  }

  return _pView->pPinceauBleu ;
}

void
NSConcernView::drawTitle(TDC* pDc, NVLdVRect* pRectARepeindre, NS_CLASSLIB::TPoint* pPtTopLeft)
{
try
{
  if (NULL == pDc)
    return ;

  NS_CLASSLIB::TRect  clientRect = _pWorkingArea->GetClientRect() ;
	NS_CLASSLIB::TPoint tptBotLeft  = clientRect.BottomLeft() ;
	NS_CLASSLIB::TPoint tptBotRight = clientRect.BottomRight() ;

	NVLdVPoint  ldvLeft(_pView) ;
	ldvLeft = (NVLdVPoint&) _pWorkingArea->getAreaLogicalPointFromWindowPoint(tptBotLeft) ;

	NVLdVPoint  ldvRight(_pView) ;
	ldvRight = (NVLdVPoint&) _pWorkingArea->getAreaLogicalPointFromWindowPoint(tptBotRight) ;

	NVLdVTemps tNoLimit;
	tNoLimit.setNoLimit();

	if ((ldvRight.getX() > _pConcern->_tDateFermeture) && (false == _pConcern->_tDateFermeture.estNoLimit()))
		ldvRight.setX(_pConcern->_tDateFermeture) ;

	if (ldvLeft.getX() < _pConcern->_tDateOuverture)
		ldvLeft.setX(_pConcern->_tDateOuverture) ;

	tptBotLeft  = _pWorkingArea->getScrollablePhysicalPoint(ldvLeft) ;
	tptBotRight = _pWorkingArea->getScrollablePhysicalPoint(ldvRight) ;

	NS_CLASSLIB::TRect rectDisplay = NS_CLASSLIB::TRect(tptBotLeft.X(), pPtTopLeft->y - (_pWorkingArea->getInterligne() - 5), tptBotRight.X(), pPtTopLeft->y) ;
	NS_CLASSLIB::TRect rect2Compare = NS_CLASSLIB::TRect(rectDisplay) ;

	pDc->DrawText(_pConcern->_sTitre.c_str(), -1, rect2Compare, DT_CALCRECT) ;
	if (rectDisplay.Width() < rect2Compare.Width())
	{
		int decal = rect2Compare.Width() - rectDisplay.Width() ;
		rectDisplay.left -= decal ;
		rectDisplay.right += decal ;
	}

	pDc->DrawText(_pConcern->_sTitre.c_str(), -1, rectDisplay, DT_CENTER) ;
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::drawTitle.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSConcernView::draw3DBorders(TDC* pDc, NVLdVRect* pRectARepeindre, OWL::TPen* pLightPen, OWL::TPen* pDarkPen, OWL::TPen* pTransitionPen, NS_CLASSLIB::TPoint* pPtTopLeft, NS_CLASSLIB::TPoint* pPtBotRight)
{
try
{
  if (((TDC*) NULL == pDc) || ((NVLdVRect*) NULL == pRectARepeindre))
    return ;

  if (((OWL::TPen*) NULL == pLightPen) || ((OWL::TPen*) NULL == pDarkPen) ||
      ((OWL::TPen*) NULL == pTransitionPen))
    return ;

  if (((NS_CLASSLIB::TPoint*) NULL == pPtTopLeft) ||
      ((NS_CLASSLIB::TPoint*) NULL == pPtBotRight))
    return ;

  bool bLeftHedgeDraw ;
	if (_pConcern->_tDateOuverture > pRectARepeindre->getLeft())
		bLeftHedgeDraw = true;
	else
		bLeftHedgeDraw = false;
	bool bRightHedgeDraw;	if (_pConcern->_tDateFermeture < pRectARepeindre->getRight())		bRightHedgeDraw = true;
	else
		bRightHedgeDraw = false;

	for (int i = 0; i < _iLeftMargin; i++)
	{
		// Partie éclairée
		// Highlighted part

		// pDc->SelectObject(*pPenEclaire);
		::SelectObject(pDc->GetHDC(), *pLightPen) ;

		// Doit-on dessiner le bord gauche de la preoccupation
		// Shall-we have to draw health issue's left hedge
		if (bLeftHedgeDraw)
		{
			pDc->MoveTo(pPtTopLeft->x + i, pPtBotRight->y + i + 1);
			// pDc->SelectObject(*pPenEclaire);
			::SelectObject(pDc->GetHDC(), *pLightPen) ;
			pDc->LineTo(pPtTopLeft->x + i, pPtTopLeft->y - i) ;
		}
		else
			pDc->MoveTo(pPtTopLeft->x, pPtTopLeft->y - i);

		pDc->LineTo(pPtBotRight->x - i, pPtTopLeft->y - i);

		// Doit-on dessiner le bord droit de la préoccupation
		// Shall-we have to draw health issue's right hedge
		if (bRightHedgeDraw)
		{
			// Point de transition
			// Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pTransitionPen) ;
			pDc->LineTo(pPtBotRight->x - i, pPtTopLeft->y - i - 1) ;

			// Partie ombrée
			// Shadow part
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pDarkPen) ;
			pDc->LineTo(pPtBotRight->x - i, pPtBotRight->y + i) ;
		}
		else
		{
			pDc->LineTo(pPtBotRight->x + 1, pPtTopLeft->y - i) ;
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pDarkPen) ;
			pDc->MoveTo(pPtBotRight->x, pPtBotRight->y + i) ;
		}

		pDc->LineTo(pPtTopLeft->x + i, pPtBotRight->y + i) ;

		if (bLeftHedgeDraw)
		{
			// Point de transition
			// Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pTransitionPen) ;
			pDc->LineTo(pPtTopLeft->x + i, pPtBotRight->y + i + 1) ;
		}
		else
			pDc->LineTo(pPtBotRight->x -1, pPtBotRight->y + i) ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::draw3DBorders.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSConcernView::drawGoals(TDC* pDc, NVLdVRect* pRectARepeindre, OWL::TPen* pLightPen, OWL::TPen* pDarkPen, OWL::TPen* pTransitionPen, NVLdVPoint* pLdvTopLeft, NVLdVPoint* pLdvBottomRight, NVLdVTemps* pTLeftTime, NVLdVTemps* pTRightTime)
{
try
{
  if (_pConcern->_aWorstJalons.empty())
    return ;

  GoalInfoIter jalonIt = _pConcern->_aWorstJalons.begin() ;
  for ( ; _pConcern->_aWorstJalons.end() != jalonIt ; jalonIt++)
  {
    if (((*jalonIt)->tpsInfo < *pTRightTime) &&
                                      ((*jalonIt)->tpsClosed > *pTLeftTime))
    {
      NVLdVPoint goalTopLeft(*pLdvTopLeft) ;
      NVLdVPoint goalBottomRight(*pLdvBottomRight) ;
      if ((*jalonIt)->tpsInfo > *pTLeftTime)
        goalTopLeft.setX((*jalonIt)->tpsInfo) ;
      if ((*jalonIt)->tpsClosed < *pTRightTime)
        goalBottomRight.setX((*jalonIt)->tpsClosed) ;

      NS_CLASSLIB::TPoint ptTL = _pWorkingArea->getScrollablePhysicalPoint(goalTopLeft) ;
      NS_CLASSLIB::TPoint ptBR = _pWorkingArea->getScrollablePhysicalPoint(goalBottomRight) ;

      ptTL.y = ptBR.y + (ptTL.y - ptBR.y) / 2 + 1 ;
      ptBR.y = ptTL.y - 3 ;

      OWL::TBrush* pPinceau = 0 ;

      switch ((*jalonIt)->iLevel)
      {
        // puisqu'il n'y a rien à faire, est-ce raisonnable d'afficher du rouge ?
        //case NSLdvGoalInfo::AVrouge :
        //    pPinceau = new OWL::TBrush(pView->pPinceauS4->GetHandle()) ;
        //    pDc->FillRect(ptTL.x, ptTL.y - 1, ptBR.x, ptBR.y + 1, *pPinceau) ;
        //    break;
        case NSLdvGoalInfo::AVjaune :
          pPinceau = new OWL::TBrush(_pView->pPinceauS3->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y - 1, ptBR.x, ptBR.y + 1, *pPinceau) ;
          break ;
        case NSLdvGoalInfo::AVvert  :
          pPinceau = new OWL::TBrush(_pView->pPinceauS2->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
          break ;
        case NSLdvGoalInfo::Bleu    :
          pPinceau = new OWL::TBrush(_pView->pPinceauS1->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
          break ;
        case NSLdvGoalInfo::APvert  :
          pPinceau = new OWL::TBrush(_pView->pPinceauS2->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
          break ;
        case NSLdvGoalInfo::APjaune :
          pPinceau = new OWL::TBrush(_pView->pPinceauS3->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
          break ;
        case NSLdvGoalInfo::AProuge :
          pPinceau = new OWL::TBrush(_pView->pPinceauS4->GetHandle()) ;
          pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
          break ;
      }

      if (pPinceau)
        delete pPinceau ;

      // pDc->SelectObject(*pPenEclaire);
      ::SelectObject(pDc->GetHDC(), *pLightPen) ;
      pDc->MoveTo(ptTL.x, ptTL.y) ;
      pDc->LineTo(ptBR.x, ptTL.y) ;

      // pDc->SelectObject(*pPenOmbre);
      ::SelectObject(pDc->GetHDC(), *pDarkPen) ;
      pDc->MoveTo(ptTL.x, ptBR.y - 1) ;
      pDc->LineTo(ptBR.x, ptBR.y - 1) ;
    }
  }
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::drawGoals.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSConcernView::drawBaddestGoal(TDC* pDc, NVLdVRect* pRectARepeindre)
{
try
{
    /*
    ArrayGoals* pGoals = pConcern->pDoc->getGoals();
    if (!pGoals || (pGoals->empty()))
        return ;

    GoalInfoArray* pJalons = pGoals->giveBaddestJalons(pConcern->sReference, pRectARepeindre) ;

    if (pJalons == NULL)
        return ;

    if (pJalons->empty())
    {
        delete pJalons;
        return ;
    }

    // Rectangle à repeindre pour l'objet lui même
	// Object's rectangle to repaint
	NVLdVPoint ldvTopLeft(pView);
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft();
	if (pConcern->tDateOuverture > tLeftTime)
		tLeftTime = pConcern->tDateOuverture;

	ldvTopLeft.setX(tLeftTime);
	ldvTopLeft.setY(pView->preoccupDonneTopLine(iIndex));

	NVLdVPoint ldvBottomRight(pView);
	// Right time
	NVLdVTemps tRightTime = pRectARepeindre->getRight();
	if (pConcern->tDateFermeture < tRightTime)
		tRightTime = pConcern->tDateFermeture;

	ldvBottomRight.setX(tRightTime);
	ldvBottomRight.setY(pView->preoccupDonneBottomLine(iIndex));

	NS_CLASSLIB::TPoint ptTopLeft = pView->getPhysicalPoint(ldvTopLeft);
	NS_CLASSLIB::TPoint ptBotRigh = pView->getPhysicalPoint(ldvBottomRight);

    for (GoalInfoIter jalonIt = pGIarray->begin(); jalonIt != pGIarray->end(); jalonIt++)
    {
        NS_CLASSLIB::TPoint ptTopLeft = pView->getPhysicalPoint(ldvTopLeft);
	    NS_CLASSLIB::TPoint ptBotRigh = pView->getPhysicalPoint(ldvBottomRight);

        NVLdVPoint severityTopLeft(ldvTopLeft);
        NVLdVPoint severityBottomRight(ldvBottomRight);
        if ((*jalonIt)->tpsInfo > tLeftTime)
            severityTopLeft.setX((*ModIt)->tDateHeureDeb);
        if ((*ModIt)->tDateHeureFin < tRightTime)
            severityBottomRight.setX((*ModIt)->tDateHeureFin);

        NS_CLASSLIB::TPoint ptTL = pView->getPhysicalPoint(severityTopLeft);
        NS_CLASSLIB::TPoint ptBR = pView->getPhysicalPoint(severityBottomRight);

        ptTL.y = ptBR.y + (ptTL.y - ptBR.y) / 2 + 1;
        ptBR.y = ptTL.y - 3;

					if ((*ModIt)->iRisque < 10)
						pPinceau = new OWL::TBrush(pView->pPinceauS1->GetHandle());
					else
						if ((*ModIt)->iRisque < 15)
							pPinceau = new OWL::TBrush(pView->pPinceauS2->GetHandle());
						else
							if ((*ModIt)->iRisque < 20)
								pPinceau = new OWL::TBrush(pView->pPinceauS3->GetHandle());
							else
								pPinceau = new OWL::TBrush(pView->pPinceauS4->GetHandle());

					pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau);

					delete pPinceau;

					// pDc->SelectObject(*pPenEclaire);
					::SelectObject(pDc->GetHDC(), *pPenEclaire);
					pDc->MoveTo(ptTL.x, ptTL.y);
					pDc->LineTo(ptBR.x, ptTL.y);

					// pDc->SelectObject(*pPenOmbre);
					::SelectObject(pDc->GetHDC(), *pPenOmbre);
					pDc->MoveTo(ptTL.x, ptBR.y - 1);
					pDc->LineTo(ptBR.x, ptBR.y - 1);
    }

    delete pJalons ;

    return ;    */

  ArrayGoals concernGoals(_pConcern->_pDoc) ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = _pConcern->_pDoc->getFrames()->getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayGoals* pGoals = pFrameInfo->getGoalsArray() ;
      if (pGoals && (false == pGoals->empty()))
		    for (ArrayGoalIter goalIter = pGoals->begin() ; pGoals->end() != goalIter ; goalIter++)
			    if ((*goalIter)->getConcern() == _pConcern->_sReference)
				    concernGoals.push_back(new NSLdvGoal(**goalIter)) ;
    }
  }

	NSLdvGoal *pGoal = concernGoals.makeBaddestProjection() ;
	if (NULL == pGoal)
		return ;

  GoalInfoArray* pGoalsArrays = pGoal->getJalons() ;
  if (pGoalsArrays->empty())
    return ;

	// ---------------------------------------------------------------------------
	// à faire autant de fois qu'il y a d'événement qui font recommencer le cycle
	// pour l'instant on se base juste sur l'objectif par rapport à la date
	// d'origine (date d'ouverture)
	// ---------------------------------------------------------------------------

	NVLdVTemps	tDateOrigine 	= pGoal->_tDateOuverture ;
	NVLdVTemps	tDateLeft		 	= tDateOrigine ;
	NVLdVTemps	tDateRight ; tDateRight.setNoLimit() ;

	NVLdVTemps	tDateDebutAuthMin, tDateDebutConsMin, tDateDebutIdealMin ;
	NVLdVTemps	tDateDebutAuthMax, tDateDebutConsMax, tDateDebutIdealMax ;

	NVLdVTemps	tDateFinAuthMin, tDateFinConsMin, tDateFinIdealMin ;
	NVLdVTemps	tDateFinAuthMax, tDateFinConsMax, tDateFinIdealMax ;

	for (GoalInfoIter iter = pGoalsArrays->begin() ; pGoalsArrays->end() != iter ; iter++)
	{
		string	sCode		= (*iter)->getCode() ;
		string	sValue	= (*iter)->getValue() ;
		string	sUnit		= (*iter)->getUnit() ;
		string	sFormat	= (*iter)->getFormat() ;

		if (strncmp((*iter)->getCode().c_str(), "KMOD", 4) == 0)
		{
			switch ((*iter)->getCode()[4])
			{
				case '1'	:	tDateDebutAuthMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '2'	: tDateDebutConsMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '3'	: tDateDebutIdealMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '4'	: tDateDebutIdealMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '5'	: tDateDebutConsMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '6'	: tDateDebutAuthMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
			}
		}

		if (strncmp((*iter)->getCode().c_str(), "KMOF", 4) == 0)
		{
			switch ((*iter)->getCode()[4])
			{
				case '1'	: tDateFinAuthMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '2'	: tDateFinConsMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '3'	: tDateFinIdealMin = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '4'	: tDateFinIdealMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '5'	: tDateFinConsMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
				case '6'	: tDateFinAuthMax = getGoalInfoDate(*iter, tDateOrigine) ;
										break ;
			}
		}

		// Rectangle à repeindre pour l'objectif lui même
		// Object's rectangle to repaint
		NVLdVPoint ldvLeft(_pView) ;

/*
		// Left time
		NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
		if (tDateOrigine > tLeftTime)
		{
			if (iter == pGoal->pGoalInfos->begin())
				tLeftTime = tDateOrigine ;
		}
*/
		ldvLeft.setX(tDateLeft) ;
		ldvLeft.setY(_pWorkingArea->preoccupDonneTopLine(_iIndex)) ;

		NVLdVPoint ldvRight(_pView) ;
/*
		// Right time
		NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
		if ((pGoal->tDateFermeture < tRightTime) && (!pGoal->tDateFermeture.estNoLimit()))
			tRightTime = pGoal->tDateFermeture ;
*/
		ldvRight.setX(tDateRight) ;
		ldvRight.setY(_pWorkingArea->preoccupDonneBottomLine(_iIndex)) ;

		NS_CLASSLIB::TPoint tptLeft  = _pWorkingArea->getScrollablePhysicalPoint(ldvLeft) ;
		NS_CLASSLIB::TPoint tptRight = _pWorkingArea->getScrollablePhysicalPoint(ldvRight) ;

		pDc->FillRect(tptLeft.X(), tptLeft.Y() - 2, tptRight.X(), tptLeft.Y() - 1, _pView->pPinceauRed->GetHandle()) ;
	}
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::drawBaddestGoal.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSConcernView::initPens(OWL::TBrush** ppBrush, OWL::TPen** ppLightPen, OWL::TPen** ppDarkPen, OWL::TPen** ppTransitionPen)
{
try
{
  if (_pWorkingArea->getCurrentConcern() == this)
	{
		*ppBrush         = new OWL::TBrush(_pView->pPinceauFond->GetHandle()) ;
		*ppLightPen      = _pView->pSelPenEclaire ;
		*ppDarkPen       = _pView->pSelPenOmbre ;
		*ppTransitionPen = _pView->pSelPenTransition ;
    return ;
	}
  if (_pView->useSystemColors())
  {
    *ppBrush         = new OWL::TBrush(_pView->pPinceauFondSys->GetHandle()) ;
    *ppLightPen      = _pView->pPenEclaireSys ;
    *ppDarkPen       = _pView->pPenOmbreSys ;
    *ppTransitionPen = _pView->pPenTransitionSys ;
  }
  else
  {
    *ppBrush         = new OWL::TBrush(_pView->pPinceauFond->GetHandle()) ;
    *ppLightPen      = _pView->pPenEclaire ;
    *ppDarkPen       = _pView->pPenOmbre ;
    *ppTransitionPen = _pView->pPenTransition ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::initPens.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}OWL::TBrush*NSConcernView::getBrushForSeverity(int iSeverity){try{  OWL::TBrush* pPinceau = (OWL::TBrush*) 0 ;  if (iSeverity <= 0)    return pPinceau ;
  if (iSeverity < 25)
  {
    if (ldvframeSocial != _iFrame)
      pPinceau = new OWL::TBrush(_pView->pPinceauS1->GetHandle()) ;
    else
      pPinceau = new OWL::TBrush(_pView->pPinceauSH1->GetHandle()) ;

    return pPinceau ;
  }
  if (iSeverity < 50)
  {
    if (ldvframeSocial != _iFrame)
      pPinceau = new OWL::TBrush(_pView->pPinceauS2->GetHandle()) ;
    else
      pPinceau = new OWL::TBrush(_pView->pPinceauSH2->GetHandle()) ;

    return pPinceau ;
  }
  if (iSeverity < 75)
  {
    if (ldvframeSocial != _iFrame)
      pPinceau = new OWL::TBrush(_pView->pPinceauS3->GetHandle()) ;
    else
      pPinceau = new OWL::TBrush(_pView->pPinceauSH3->GetHandle()) ;

    return pPinceau ;
  }
  if (ldvframeSocial != _iFrame)
    pPinceau = new OWL::TBrush(_pView->pPinceauS4->GetHandle()) ;
  else
    pPinceau = new OWL::TBrush(_pView->pPinceauSH4->GetHandle()) ;

  return pPinceau ;
}
catch (...)
{
  string sErrorText = string("Exception NSConcernView::getBrushForSeverity.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
  return (OWL::TBrush*) 0 ;
}
}OWL::TPen*NSConcernView::getPenForScore(int iScore){  switch (iScore)  {
    case 1 : return _pView->pPenS1 ;
    case 2 : return _pView->pPenS2 ;
    case 3 : return _pView->pPenS3 ;
    case 4 : return _pView->pPenS4 ;
    case 5 : return _pView->pPenS5 ;
  }

  return _pView->pPenEclaire ;}
NVLdVTemps
NSConcernView::getGoalInfoDate(NSLdvGoalInfo *pInfoGoal, NVLdVTemps tDateOrigine)
{
	NVLdVTemps	resultDate ;

	if (pInfoGoal->getFormat()[1] == 'N')
	{
		resultDate = tDateOrigine ;

		string sVal = pInfoGoal->getValue() ;
		int	iVal = 0 ;

		// -------------------------------------------------------------------------
		// conversion atoi sur 2 digits
		// -------------------------------------------------------------------------
		if (strlen(sVal.c_str()) == 2)
			iVal = (sVal[0] - '0') * 10 + (sVal[1] - '0') ;
		if (strlen(sVal.c_str()) == 1)
			iVal = sVal[0] - '0' ;

		// code lexique unité de temps
		// années : 2DAT31 - mois : 2DAT21 - jour : 2DAT01 - heure : 2HE001 - minute : 2MINU1 - seconde : 2SEC01

		if (strncmp(pInfoGoal->getUnit().c_str(), "2DAT", 4) == 0)
		{
			switch (pInfoGoal->getUnit()[4])
			{
				case '3'	: resultDate.ajouteAns(iVal) ;
										break ;
				case '2'	:	resultDate.ajouteMois(iVal) ;
										break ;
				case '0'	:	resultDate.ajouteJours(iVal) ;
										break ;
			}
		}
		else
		{
			switch (pInfoGoal->getUnit()[1])
			{
				case 'H'	:	resultDate.ajouteHeures(iVal) ;
										break ;
				case 'M'	: resultDate.ajouteMinutes(iVal) ;
										break ;
				case 'S'	: resultDate.ajouteSecondes(iVal) ;
										break ;
			}
		}
		return resultDate ;
	}

	if (('D' == pInfoGoal->getFormat()[1]) || ('T' == pInfoGoal->getFormat()[1]))
		resultDate.initFromDate(pInfoGoal->getValue()) ;

	return resultDate ;
}


void
NSConcernView::EvRButtonDown()
{
}

void    NSConcernView::createComponents(ArrayOfToons* pToonsArray)
{
/*
	if (!(pConcern->aModificateurs.empty()))
  {
  	ArrayModifierIter i = pConcern->aModificateurs.begin();
    for (; i != pConcern->aModificateurs.end(); i++)
    {
    	NSPbModifView* pPbModiView = new NSPbModifView(pContexte, pView, *i);
      pToonsArray->push_back((NSLdvToon*)(new NSPbModifView(*pPbModiView)));
      pPbModiView->createComponents(pToonsArray);
      delete pPbModiView;
    }
  }
*/
}

NSConcernView&
NSConcernView::operator=(const NSConcernView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

  _iIndex        = src._iIndex ;
  _pConcern      = src._pConcern ;
  _iFrame        = src._iFrame ;
  _iLeftMargin   = src._iLeftMargin ;
  _iRigthMargin  = src._iRigthMargin ;
  _iTopMargin    = src._iTopMargin ;
  _iBottomMargin = src._iBottomMargin ;

  return *this ;
}

// -----------------------------------------------------------------------------
// ----------------------- METHODES DE NSSocialView -----------------------------
// -----------------------------------------------------------------------------

// NSSocialView::NSSocialView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, int iIdx /*= -1*/)
//             :NSConcernView(pCtx, pWorkArea, pPb, iIdx)
//{
//}

/*
NSSocialView::NSSocialView(NSSocialView& rv)
             :NSConcernView(rv)
{
}

NSSocialView&
NSSocialView::operator=(NSSocialView& src)
{
  if (this == &src)
		return *this ;

  NSConcernView* pConcern1 = this ;
	NSConcernView* pConcern2 = &src ;

	*pConcern1 = *pConcern2 ;

  return *this ;
}
*/

// -----------------------------------------------------------------------------
// ----------------------- METHODES DE NSRiskView -----------------------------
// -----------------------------------------------------------------------------

//NSRiskView::NSRiskView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPb, int iIdx /*= -1*/)
//           :NSConcernView(pCtx, pWorkArea, pPb, iIdx)
//{
//}

/*
NSRiskView::NSRiskView(NSRiskView& rv)
           :NSConcernView(rv)
{
}

void
NSRiskView::draw(TDC* pDc, NVLdVRect* pRectARepeindre)
{
	OWL::TBrush* pPinceau = 0 ;
	OWL::TPen*   pPenEclaire = 0 ;
	OWL::TPen*   pPenOmbre = 0 ;
	OWL::TPen*   pPenTransition = 0 ;

	// Sauvegarde des éléments en cours (pen et brush)
	HPEN oldPen     = (HPEN) pDc->GetCurrentObject(OBJ_PEN) ;
	HBRUSH oldBrush = (HBRUSH) pDc->GetCurrentObject(OBJ_BRUSH) ;

  initPens(&pPinceau, &pPenEclaire, &pPenOmbre, &pPenTransition) ;

	// Rectangle à repeindre pour l'objet lui même
	// Object's rectangle to repaint
	NVLdVPoint ldvTopLeft(pView) ;
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (_pConcern->tDateOuverture > tLeftTime)
		tLeftTime = _pConcern->tDateOuverture ;

	ldvTopLeft.setX(tLeftTime) ;
	ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_iIndex)) ;

	NVLdVPoint ldvBottomRight(pView);
	// Right time
	NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
	if (_pConcern->tDateFermeture < tRightTime)
		tRightTime = _pConcern->tDateFermeture ;

	ldvBottomRight.setX(tRightTime) ;
	ldvBottomRight.setY(_pWorkingArea->preoccupDonneBottomLine(_iIndex)) ;

	NS_CLASSLIB::TPoint ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pWorkingArea->getScrollablePhysicalPoint(ldvBottomRight) ;

	// On colore le fond
	// Filling the rectangle
	// pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y, *pPinceau) ;

  if (pPinceau)
	  delete pPinceau ;

	// On fixe la visibleBox
	NS_CLASSLIB::TRect PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
	VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;

	// Décorations 3D
	// 3D borders
  //
  draw3DBorders(pDc, pRectARepeindre, pPenTransition, pPenOmbre, pPenTransition, &ptTopLeft, &ptBotRigh) ;

	// affichage du titre
  //
  drawTitle(pDc, pRectARepeindre, &ptTopLeft) ;

	// -----------------------------------------------------------------------
	// Affichage des objectifs
	// -----------------------------------------------------------------------
  //
  drawGoals(pDc, pRectARepeindre, pPenEclaire, pPenOmbre, pPenTransition, &ldvTopLeft, &ldvBottomRight, &tLeftTime, &tRightTime) ;

	// ---------------------------------------------------------------------------
	// Restauration des objets initiaux (pen et brush)
	::SelectObject(pDc->GetHDC(), oldPen) ;
	::SelectObject(pDc->GetHDC(), oldBrush) ;
}

NSRiskView&
NSRiskView::operator=(NSRiskView& src)
{
  if (this == &src)
		return *this ;

  NSConcernView* pConcern1 = this ;
	NSConcernView* pConcern2 = &src ;

	*pConcern1 = *pConcern2 ;

  return *this ;
}
*/

// -----------------------------------------------------------------------------
// ----------------------- METHODES DE NSLigneView -----------------------------
// -----------------------------------------------------------------------------

NSLigneView::NSLigneView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, bool bVScrollEnabled)
            :NSLdvTimeToon(pCtx, pWorkArea, bVScrollEnabled)
{
  lObjectCount++ ;
}


NSLigneView::NSLigneView(const NSLigneView& rv)
            :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv));

  _iIndex = rv._iIndex;
}

NSLigneView::~NSLigneView()
{
  lObjectCount-- ;
}

void
NSLigneView::SetupWindow()
{
}

void
NSLigneView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
}

void
NSLigneView::EvRButtonDown()
{
}

void
NSLigneView::boxFitting()
{
	_Box.setTop(_pWorkingArea->preoccupDonneTopBox(_iIndex)) ;
  _Box.setBottom(_pWorkingArea->preoccupDonneBottomBox(_iIndex)) ;
}

NSLigneView&
NSLigneView::operator=(const NSLigneView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;
  _iIndex = src._iIndex ;

	return *this ;
}

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSPreoccupView ---------------------------
// -----------------------------------------------------------------------------

NSPreoccupView::NSPreoccupView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSConcern* pPreoccup, int iIdx)
               :NSLigneView(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_iIndex         = iIdx ;

	if (_pWorkingArea->getLargeurLigne() >= 30)
	{
		_iLeftMargin   = 2 ;
		_iRigthMargin  = 2 ;
		_iTopMargin    = 2 ;
		_iBottomMargin = 2 ;
	}
	else
	{
		_iLeftMargin   = 1 ;
		_iRigthMargin  = 1 ;
		_iTopMargin    = 1 ;
		_iBottomMargin = 1 ;
	}

	pPreoccupation = pPreoccup ;

	_Box.setLeft(pPreoccupation->_tDateOuverture) ;

	if (pPreoccupation->_tDateFermeture.estVide())
		_Box.setRight(pPreoccupation->_tDateFermeture) ;
	else
	{
		NVLdVTemps tNoLimit ;
		tNoLimit.setNoLimit() ;
		_Box.setRight(tNoLimit) ;
	}

	_Box.setTop(_pWorkingArea->preoccupDonneTopBox(_iIndex)) ;
	_Box.setBottom(_pWorkingArea->preoccupDonneBottomBox(_iIndex)) ;

	_iZOrder  = LEVEL_PREOCCUP + RELPRLEVEL_LINE ;
	_toonType = toonLine ;
}

NSPreoccupView::NSPreoccupView(const NSPreoccupView& rv)
               :NSLigneView(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv)) ;

	_iIndex         = rv._iIndex ;
	pPreoccupation  = rv.pPreoccupation ;
	_iLeftMargin    = rv._iLeftMargin ;
	_iRigthMargin   = rv._iRigthMargin ;
	_iTopMargin     = rv._iTopMargin ;
	_iBottomMargin  = rv._iBottomMargin ;
}

NSPreoccupView::~NSPreoccupView()
{
  lObjectCount-- ;
}

void
NSPreoccupView::SetupWindow()
{
}

void
NSPreoccupView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	OWL::TBrush* pPinceau;
	OWL::TPen*   pPenEclaire;
	OWL::TPen*   pPenOmbre;
	OWL::TPen*   pPenTransition;

	// Sauvegarde des éléments en cours (pen et brush)
	HPEN oldPen     = (HPEN) pDc->GetCurrentObject(OBJ_PEN);
	HBRUSH oldBrush = (HBRUSH) pDc->GetCurrentObject(OBJ_BRUSH);

	if (_pView->useSystemColors())
	{
		pPinceau        = new OWL::TBrush(_pView->pPinceauFondSys->GetHandle());
		pPenEclaire     = _pView->pPenEclaireSys;
		pPenOmbre       = _pView->pPenOmbreSys;
		pPenTransition  = _pView->pPenTransitionSys;
	}
	else
	{
		pPinceau        = new OWL::TBrush(_pView->pPinceauFond->GetHandle());
		pPenEclaire     = _pView->pPenEclaire;
		pPenOmbre       = _pView->pPenOmbre;
		pPenTransition  = _pView->pPenTransition;
	}

	// Rectangle à repeindre pour l'objet lui même
	// Object's rectangle to repaint
	NVLdVPoint ldvTopLeft(_pView) ;
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (pPreoccupation->_tDateOuverture > tLeftTime)
		tLeftTime = pPreoccupation->_tDateOuverture ;

	ldvTopLeft.setX(tLeftTime) ;
	ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_iIndex)) ;

	NVLdVPoint ldvBottomRight(_pView) ;
	// Right time
	NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
	if (pPreoccupation->_tDateFermeture < tRightTime)
  	tLeftTime = pPreoccupation->_tDateFermeture ;
	ldvBottomRight.setX(tRightTime) ;	ldvBottomRight.setY(_pWorkingArea->preoccupDonneBottomLine(_iIndex)) ;
	NS_CLASSLIB::TPoint ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pWorkingArea->getScrollablePhysicalPoint(ldvBottomRight);

	// On colore le fond
	// Filling the rectangle
	pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y, *pPinceau);

	delete pPinceau ;

	// On fixe la visibleBox
  if (bGlobal)
  {
	  NS_CLASSLIB::TRect PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
    _VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;
  }

	// Décorations 3D
	// 3D decorating

	bool bLeftHedgeDraw;
	if (pPreoccupation->_tDateOuverture > pRectARepeindre->getLeft())
		bLeftHedgeDraw = true ;
	else
		bLeftHedgeDraw = false ;

	bool bRightHedgeDraw;
	if (pPreoccupation->_tDateFermeture < pRectARepeindre->getRight())
		bRightHedgeDraw = true ;
	else
		bRightHedgeDraw = false ;

	for (int i = 0 ; i < _iLeftMargin ; i++)
	{
		// Partie éclairée
		// Highlighted part

    // pDc->SelectObject(*pPenEclaire);
		::SelectObject(pDc->GetHDC(), *pPenEclaire) ;

		// Doit-on dessiner le bord gauche de la preoccupation
		// Shall-we have to draw health issue's left hedge
		if (bLeftHedgeDraw)
		{
			pDc->MoveTo(ptTopLeft.x + i, ptBotRigh.y + i + 1);
			// pDc->SelectObject(*pPenEclaire);
			::SelectObject(pDc->GetHDC(), *pPenEclaire);
			pDc->LineTo(ptTopLeft.x + i, ptTopLeft.y - i);
		}
		else
			pDc->MoveTo(ptTopLeft.x, ptTopLeft.y - i);

		pDc->LineTo(ptBotRigh.x - i, ptTopLeft.y - i);

		// Doit-on dessiner le bord droit de la préoccupation
		// Shall-we have to draw health issue's right hedge
		if (bRightHedgeDraw)
		{
			// Point de transition
			// Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pPenTransition);
			pDc->LineTo(ptBotRigh.x - i, ptTopLeft.y - i - 1);
			// Partie ombrée
			// Shadow part
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pPenOmbre);
			pDc->LineTo(ptBotRigh.x - i, ptBotRigh.y + i);
		}
		else
		{
			pDc->LineTo(ptBotRigh.x + 1, ptTopLeft.y - i);
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pPenOmbre);
			pDc->MoveTo(ptBotRigh.x, ptBotRigh.y + i);
		}

		pDc->LineTo(ptTopLeft.x + i, ptBotRigh.y + i);

		if (bLeftHedgeDraw)
		{
			// Point de transition
      // Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pPenTransition) ;
			pDc->LineTo(ptTopLeft.x + i, ptBotRigh.y + i + 1) ;
		}
		else
			pDc->LineTo(ptBotRigh.x -1, ptBotRigh.y + i) ;
	}

	// Restauration des objets initiaux (pen et brush)
	::SelectObject(pDc->GetHDC(), oldPen) ;
  ::SelectObject(pDc->GetHDC(), oldBrush) ;
}
catch (...)
{
  string sErrorText = string("Exception NSPreoccupView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSPreoccupView::EvRButtonDown()
{
}

void
NSPreoccupView::createComponents(ArrayOfToons* pToonsArray)
{
/*
  if (!(pPreoccupation->aConcerns.empty()))
  {
  	ArrayConcernIter i = pPreoccupation->aConcerns.begin();
    for (; i != pPreoccupation->aConcerns.end(); i++)
    {
    	NSConcernView* pPbView = new NSConcernView(pContexte, pView, *i);
      pToonsArray->push_back(new NSConcernView(*pPbView));
      pPbView->createComponents(pToonsArray);
      delete pPbView;
    }
  }
*/
}

NSPreoccupView&
NSPreoccupView::operator=(const NSPreoccupView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src));

	_iIndex         = src._iIndex ;
	pPreoccupation  = src.pPreoccupation ;
	_iLeftMargin    = src._iLeftMargin ;
	_iRigthMargin   = src._iRigthMargin ;
	_iTopMargin     = src._iTopMargin ;
	_iBottomMargin  = src._iBottomMargin ;

	return *this ;
}

// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSDrugLineView ---------------------------
// -----------------------------------------------------------------------------

NSDrugLineView::NSDrugLineView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvDrug* pLdvDrug, LDVFRAME iFrame, int iIdx)
               :NSLigneView(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_iIndex         = iIdx ;
  _iFrame         = iFrame ;

	if (_pWorkingArea->getLargeurSubLigne() >= 30)
	{
		_iLeftMargin   = 2 ;
		_iRigthMargin  = 2 ;
		_iTopMargin    = 2 ;
		_iBottomMargin = 2 ;
	}
	else
	{
		_iLeftMargin   = 1 ;
		_iRigthMargin  = 1 ;
		_iTopMargin    = 1 ;
		_iBottomMargin = 1 ;
	}

	_pDrug = pLdvDrug ;

	_Box.setLeft(_pDrug->_tDateOuverture) ;

	if (false == _pDrug->_tDateFermeture.estVide())
		_Box.setRight(_pDrug->_tDateFermeture) ;
	else
	{
		NVLdVTemps tNoLimit ;
		tNoLimit.setNoLimit() ;
		_Box.setRight(tNoLimit) ;
	}

	_Box.setTop(_pWorkingArea->drugDonneTopBox(_iIndex)) ;
	_Box.setBottom(_pWorkingArea->drugDonneBottomBox(_iIndex)) ;

	_iZOrder  = LEVEL_PREOCCUP + RELPRLEVEL_LINE ;
	_toonType = toonDrug ; // was toonLine
}


NSDrugLineView::NSDrugLineView(const NSDrugLineView& rv)
               :NSLigneView(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv)) ;

	_iIndex        = rv._iIndex ;
  _iFrame        = rv._iFrame ;
	_pDrug         = rv._pDrug ;
	_iLeftMargin   = rv._iLeftMargin ;
	_iRigthMargin  = rv._iRigthMargin ;
	_iTopMargin    = rv._iTopMargin ;
	_iBottomMargin = rv._iBottomMargin ;
}

NSDrugLineView::~NSDrugLineView()
{
  lObjectCount-- ;
}

void
NSDrugLineView::SetupWindow()
{
}

void
NSDrugLineView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	OWL::TBrush* pPinceau;
	OWL::TPen*   pPenEclaire;
	OWL::TPen*   pPenOmbre;
	OWL::TPen*   pPenTransition;

	// Sauvegarde des éléments en cours (pen et brush)
	HPEN oldPen     = (HPEN) pDc->GetCurrentObject(OBJ_PEN);
	HBRUSH oldBrush = (HBRUSH) pDc->GetCurrentObject(OBJ_BRUSH);

	if (_pView->useSystemColors())
	{
		pPinceau        = new OWL::TBrush(_pView->pPinceauFondSys->GetHandle());
		pPenEclaire     = _pView->pPenEclaireSys;
		pPenOmbre       = _pView->pPenOmbreSys;
		pPenTransition  = _pView->pPenTransitionSys;
	}
	else
	{
		pPinceau        = new OWL::TBrush(_pView->pPinceauFond->GetHandle());
		pPenEclaire     = _pView->pPenEclaire;
		pPenOmbre       = _pView->pPenOmbre;
		pPenTransition  = _pView->pPenTransition;
	}

	// Rectangle à repeindre pour l'objet lui même
	// Object's rectangle to repaint
	NVLdVPoint ldvTopLeft(_pView) ;
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (_pDrug->_tDateOuverture > tLeftTime)
		tLeftTime = _pDrug->_tDateOuverture ;

	ldvTopLeft.setX(tLeftTime);
	ldvTopLeft.setY(_pWorkingArea->drugDonneTopLine(_iIndex)) ;

	NVLdVPoint ldvBottomRight(_pView) ;
	// Right time
	NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
	if (_pDrug->_tDateFermeture < tRightTime)
  	tRightTime = _pDrug->_tDateFermeture ;
	ldvBottomRight.setX(tRightTime) ;	ldvBottomRight.setY(_pWorkingArea->drugDonneBottomLine(_iIndex)) ;
	NS_CLASSLIB::TPoint ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pWorkingArea->getScrollablePhysicalPoint(ldvBottomRight) ;

	// On colore le fond
	// Filling the rectangle
	pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y, *pPinceau) ;

	delete pPinceau;

	// On fixe la visibleBox
  if (bGlobal)
  {
	  NS_CLASSLIB::TRect PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft, ptBotRigh) ;
    _VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;
  }

	// Décorations 3D
	// 3D decorating

	bool bLeftHedgeDraw ;
	if (_pDrug->_tDateOuverture > pRectARepeindre->getLeft())
		bLeftHedgeDraw = true ;
	else
		bLeftHedgeDraw = false ;

	bool bRightHedgeDraw ;
	if (_pDrug->_tDateFermeture < pRectARepeindre->getRight())
		bRightHedgeDraw = true ;
	else
		bRightHedgeDraw = false ;

	for (int i = 0; i < _iLeftMargin; i++)
	{
		// Partie éclairée
		// Highlighted part

    	// pDc->SelectObject(*pPenEclaire);
		::SelectObject(pDc->GetHDC(), *pPenEclaire) ;

		// Doit-on dessiner le bord gauche de la preoccupation
		// Shall-we have to draw health issue's left hedge
		if (bLeftHedgeDraw)
		{
			pDc->MoveTo(ptTopLeft.x + i, ptBotRigh.y + i + 1) ;
			// pDc->SelectObject(*pPenEclaire);
			::SelectObject(pDc->GetHDC(), *pPenEclaire) ;
			pDc->LineTo(ptTopLeft.x + i, ptTopLeft.y - i) ;
		}
		else
			pDc->MoveTo(ptTopLeft.x, ptTopLeft.y - i) ;

		pDc->LineTo(ptBotRigh.x - i, ptTopLeft.y - i) ;

		// Doit-on dessiner le bord droit de la préoccupation
		// Shall-we have to draw health issue's right hedge
		if (bRightHedgeDraw)
		{
			// Point de transition
			// Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pPenTransition) ;
			pDc->LineTo(ptBotRigh.x - i, ptTopLeft.y - i - 1) ;
			// Partie ombrée
			// Shadow part
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pPenOmbre) ;
			pDc->LineTo(ptBotRigh.x - i, ptBotRigh.y + i) ;
		}
		else
		{
			pDc->LineTo(ptBotRigh.x + 1, ptTopLeft.y - i) ;
			// pDc->SelectObject(*pPenOmbre);
			::SelectObject(pDc->GetHDC(), *pPenOmbre) ;
			pDc->MoveTo(ptBotRigh.x, ptBotRigh.y + i) ;
		}

		pDc->LineTo(ptTopLeft.x + i, ptBotRigh.y + i) ;

		if (bLeftHedgeDraw)
		{
			// Point de transition
      		// Transition point
			// pDc->SelectObject(*pPenTransition);
			::SelectObject(pDc->GetHDC(), *pPenTransition) ;
			pDc->LineTo(ptTopLeft.x + i, ptBotRigh.y + i + 1) ;
		}
		else
			pDc->LineTo(ptBotRigh.x -1, ptBotRigh.y + i) ;
	}

  // affichage du titre
  NS_CLASSLIB::TRect  clientRect  = _pWorkingArea->GetClientRect() ;
	NS_CLASSLIB::TPoint tptBotLeft  = clientRect.BottomLeft() ;
	NS_CLASSLIB::TPoint tptBotRight = clientRect.BottomRight() ;

	NVLdVPoint ldvBotLeft(_pView) ;
	ldvBotLeft = (NVLdVPoint&) _pWorkingArea->getAreaLogicalPointFromWindowPoint(tptBotLeft) ;

	NVLdVPoint ldvBotRight(_pView) ;
	ldvBotRight = (NVLdVPoint&) _pWorkingArea->getAreaLogicalPointFromWindowPoint(tptBotRight) ;

  NVLdVPoint ldvLeft(ldvBotLeft) ;
  NVLdVPoint ldvRight(ldvBotRight) ;

	NVLdVTemps tNoLimit ;
	tNoLimit.setNoLimit();

	if ((ldvRight.getX() > _pDrug->_tDateFermeture) && (false == _pDrug->_tDateFermeture.estNoLimit()))
		ldvRight.setX(_pDrug->_tDateFermeture) ;

	if (ldvLeft.getX() < _pDrug->_tDateOuverture)
		ldvLeft.setX(_pDrug->_tDateOuverture) ;

	tptBotLeft  = _pWorkingArea->getScrollablePhysicalPoint(ldvLeft) ;
	tptBotRight = _pWorkingArea->getScrollablePhysicalPoint(ldvRight) ;

	NS_CLASSLIB::TRect rectDisplay = NS_CLASSLIB::TRect(tptBotLeft.X(), ptTopLeft.y - (_pWorkingArea->getInterligne() - 5), tptBotRight.X(), ptTopLeft.y) ;
	NS_CLASSLIB::TRect rect2Compare = NS_CLASSLIB::TRect(rectDisplay) ;

  // Affichage des doses
  //
  if ((_pView->getXunit() > pixSemaine) && (false == _pDrug->_aPhases.empty()))
  {
  	NSLdvDrugPhaseIter itPhase = _pDrug->_aPhases.begin() ;

    HFONT hOldFont = (HFONT) pDc->GetCurrentObject(OBJ_FONT) ;

    OWL::TFont drugDoseFont("Arial", 12) ;
    pDc->SelectObject(drugDoseFont) ;

    bool bDoseDisplayed = false ;
    int  iDoseTextHeight = 0 ;

  	for (; itPhase != _pDrug->_aPhases.end() ; itPhase++)
    {
    	// Phase indicator

			// Left time
			NVLdVTemps tpsPhaseStart = (*itPhase)->_tDateOuverture ;
      // If start is visible, draw it
      if ((tpsPhaseStart > ldvBotLeft.getX()) && (tpsPhaseStart < ldvBotRight.getX()))
      {
      	NVLdVPoint ldvPhaseTopLeft(_pView) ;
      	NVLdVPoint ldvPhaseBottomLeft(_pView) ;
        ldvPhaseTopLeft.setX(tpsPhaseStart) ;
				ldvPhaseTopLeft.setY(_pWorkingArea->drugDonneTopLine(_iIndex) + _pWorkingArea->getInterligne() / 2) ;
        ldvPhaseBottomLeft.setX(tpsPhaseStart) ;
				ldvPhaseBottomLeft.setY(_pWorkingArea->drugDonneTopLine(_iIndex)) ;
        NS_CLASSLIB::TPoint ptPhaseTopLeft    = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseTopLeft) ;
        NS_CLASSLIB::TPoint ptPhaseBottomLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseBottomLeft) ;
        pDc->MoveTo(ptPhaseBottomLeft.x, ptPhaseBottomLeft.y) ;
        pDc->LineTo(ptPhaseTopLeft.x, ptPhaseTopLeft.y) ;
        // pDc->LineTo(ptPhaseTopLeft.x+1, ptPhaseTopLeft.y) ;
        // pDc->LineTo(ptPhaseBottomLeft.x+1, ptPhaseBottomLeft.y) ;
      }

      // Right time
      NVLdVTemps tpsPhaseEnd = (*itPhase)->_tDateFermeture ;
      if ((tpsPhaseEnd < ldvBotRight.getX()) && (tpsPhaseEnd > ldvBotLeft.getX()))
      {
      	NVLdVPoint ldvPhaseTopRight(_pView) ;
      	NVLdVPoint ldvPhaseBottomRight(_pView) ;
        ldvPhaseTopRight.setX(tpsPhaseEnd) ;
				ldvPhaseTopRight.setY(_pWorkingArea->drugDonneTopLine(_iIndex) + _pWorkingArea->getInterligne() / 2) ;
        ldvPhaseBottomRight.setX(tpsPhaseEnd) ;
				ldvPhaseBottomRight.setY(_pWorkingArea->drugDonneTopLine(_iIndex)) ;
        NS_CLASSLIB::TPoint ptPhaseTopRight    = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseTopRight) ;
        NS_CLASSLIB::TPoint ptPhaseBottomRight = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseBottomRight) ;
        pDc->MoveTo(ptPhaseBottomRight.x, ptPhaseBottomRight.y) ;
        pDc->LineTo(ptPhaseTopRight.x, ptPhaseTopRight.y) ;
        // pDc->LineTo(ptPhaseTopRight.x-1, ptPhaseTopRight.y) ;
        // pDc->LineTo(ptPhaseBottomRight.x-1, ptPhaseBottomRight.y) ;
      }

      if ((tpsPhaseStart < ldvRight.getX()) && (tpsPhaseEnd > ldvLeft.getX()))
      {
      	string sShortTitle = (*itPhase)->_sTitreCourt ;
      	if (sShortTitle != string(""))
      	{
        	NVLdVPoint ldvPhaseTLeft(_pView) ;
      		NVLdVPoint ldvPhaseBRight(_pView) ;

          if (ldvRight.getX() < (*itPhase)->_tDateFermeture)
          	ldvPhaseBRight.setX(ldvRight.getX()) ;
          else
          	ldvPhaseBRight.setX((*itPhase)->_tDateFermeture) ;
          ldvPhaseBRight.setY(_pWorkingArea->drugDonneTopLine(_iIndex)) ;

          if (ldvLeft.getX() > (*itPhase)->_tDateOuverture)
          	ldvPhaseTLeft.setX(ldvLeft.getX()) ;
          else
          	ldvPhaseTLeft.setX((*itPhase)->_tDateOuverture) ;
          ldvPhaseTLeft.setY(_pWorkingArea->drugDonneTopLine(_iIndex) + _pWorkingArea->getInterligne() / 2) ;

        	NS_CLASSLIB::TPoint tptPhaseTLeft  = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseTLeft) ;
					NS_CLASSLIB::TPoint tptPhaseBRight = _pWorkingArea->getScrollablePhysicalPoint(ldvPhaseBRight) ;

          NS_CLASSLIB::TRect rectPhaseDisplay = NS_CLASSLIB::TRect(tptPhaseTLeft.X(), tptPhaseTLeft.Y(), tptPhaseBRight.X(), tptPhaseBRight.Y()) ;
					NS_CLASSLIB::TRect rectPhase2Compare = NS_CLASSLIB::TRect(rectDisplay) ;

          pDc->DrawText(sShortTitle.c_str(), -1, rectPhase2Compare, DT_CALCRECT) ;
          if (rectPhaseDisplay.Width() >= rectPhase2Compare.Width())
          {
          	rectPhaseDisplay = NS_CLASSLIB::TRect(rectPhaseDisplay.Left(), rectPhaseDisplay.Bottom()-rectPhase2Compare.Height(), rectPhaseDisplay.Right(), rectPhaseDisplay.Bottom()) ;
          	pDc->DrawText(sShortTitle.c_str(), -1, rectPhaseDisplay, DT_CENTER) ;
            if (rectPhase2Compare.Height() > iDoseTextHeight)
            	iDoseTextHeight = rectPhase2Compare.Height() ;
            bDoseDisplayed = true ;
          }
        }
      }
    }

    if (bDoseDisplayed)
    	rectDisplay.Set(rectDisplay.Left(),
                      rectDisplay.Top() - iDoseTextHeight,
                      rectDisplay.Right(),
                      rectDisplay.Bottom() - iDoseTextHeight) ;

    ::SelectObject(pDc->GetHDC(), hOldFont) ;
  }

	pDc->DrawText(_pDrug->_sTitreCourt.c_str(), -1, rect2Compare, DT_CALCRECT) ;
	if (rectDisplay.Width() < rect2Compare.Width())
	{
		int decal = rect2Compare.Width() - rectDisplay.Width() ;
		rectDisplay.left -= decal ;
		rectDisplay.right += decal ;
	}

	pDc->DrawText(_pDrug->_sTitreCourt.c_str(), -1, rectDisplay, DT_CENTER) ;

  // -----------------------------------------------------------------------
	// Affichage des objectifs
	// -----------------------------------------------------------------------

	if ((_pDrug->_pWorstJalons) && (false == _pDrug->_pWorstJalons->empty()))
	{
  	GoalInfoIter jalonIt = _pDrug->_pWorstJalons->begin() ;
    for ( ; jalonIt != _pDrug->_pWorstJalons->end(); jalonIt++)
    {
    	if (((*jalonIt)->tpsInfo < tRightTime) &&
                                        ((*jalonIt)->tpsClosed > tLeftTime))
      {
      	NVLdVPoint goalTopLeft(ldvTopLeft) ;
        NVLdVPoint goalBottomRight(ldvBottomRight) ;
        if ((*jalonIt)->tpsInfo > tLeftTime)
        	goalTopLeft.setX((*jalonIt)->tpsInfo) ;
        if ((*jalonIt)->tpsClosed < tRightTime)
        	goalBottomRight.setX((*jalonIt)->tpsClosed) ;

        NS_CLASSLIB::TPoint ptTL = _pWorkingArea->getScrollablePhysicalPoint(goalTopLeft) ;
      	NS_CLASSLIB::TPoint ptBR = _pWorkingArea->getScrollablePhysicalPoint(goalBottomRight) ;

        ptTL.y = ptBR.y + (ptTL.y - ptBR.y) / 2 + 1 ;
        ptBR.y = ptTL.y - 3 ;

        switch ((*jalonIt)->iLevel)
        {
        	// puisqu'il n'y a rien à faire, est-ce raisonnable d'afficher du rouge ?
          //case NSLdvGoalInfo::AVrouge :
          //    pPinceau = new OWL::TBrush(pView->pPinceauS4->GetHandle()) ;
          //    pDc->FillRect(ptTL.x, ptTL.y - 1, ptBR.x, ptBR.y + 1, *pPinceau) ;
          //    break;
          case NSLdvGoalInfo::AVjaune :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS3->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y - 1, ptBR.x, ptBR.y + 1, *pPinceau) ;
            break ;
          case NSLdvGoalInfo::AVvert  :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS2->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
            break ;
          case NSLdvGoalInfo::Bleu    :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS1->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
            break ;
          case NSLdvGoalInfo::APvert  :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS2->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
            break ;
          case NSLdvGoalInfo::APjaune :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS3->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
            break ;
          case NSLdvGoalInfo::AProuge :
          	pPinceau = new OWL::TBrush(_pView->pPinceauS4->GetHandle()) ;
            pDc->FillRect(ptTL.x, ptTL.y, ptBR.x, ptBR.y, *pPinceau) ;
            break ;
        }

        delete pPinceau ;

        // pDc->SelectObject(*pPenEclaire);
        ::SelectObject(pDc->GetHDC(), *pPenEclaire) ;
        pDc->MoveTo(ptTL.x, ptTL.y) ;
        pDc->LineTo(ptBR.x, ptTL.y) ;

        // pDc->SelectObject(*pPenOmbre);
        ::SelectObject(pDc->GetHDC(), *pPenOmbre);
        pDc->MoveTo(ptTL.x, ptBR.y - 1);
        pDc->LineTo(ptBR.x, ptBR.y - 1);
      }
    }
  }

	// Restauration des objets initiaux (pen et brush)
	::SelectObject(pDc->GetHDC(), oldPen);
  ::SelectObject(pDc->GetHDC(), oldBrush);
}
catch (...)
{
  string sErrorText = string("Exception NSDrugLineView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSDrugLineView::EvRButtonDown()
{
}

void
NSDrugLineView::createComponents(ArrayOfToons* pToonsArray)
{
/*
  if (!(pPreoccupation->aConcerns.empty()))
  {
  	ArrayConcernIter i = pPreoccupation->aConcerns.begin();
    for (; i != pPreoccupation->aConcerns.end(); i++)
    {
    	NSConcernView* pPbView = new NSConcernView(pContexte, pView, *i);
      pToonsArray->push_back(new NSConcernView(*pPbView));
      pPbView->createComponents(pToonsArray);
      delete pPbView;
    }
  }
*/
}


NSDrugLineView&
NSDrugLineView::operator=(const NSDrugLineView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

	_iIndex        = src._iIndex ;
  _iFrame        = src._iFrame ;
	_pDrug         = src._pDrug ;
	_iLeftMargin   = src._iLeftMargin ;
	_iRigthMargin  = src._iRigthMargin ;
	_iTopMargin    = src._iTopMargin ;
	_iBottomMargin = src._iBottomMargin ;

	return *this ;
}


// -----------------------------------------------------------------------------
// ---------------------- METHODES DE NSBaseLineView ---------------------------
// -----------------------------------------------------------------------------

NSBaseLineView::NSBaseLineView(NSContexte* pCtx, NSLdvViewArea *pWorkArea)
               :NSLigneView(pCtx, pWorkArea, false)
{
  lObjectCount++ ;

	NVLdVTemps tTemps ;

	// Initialisation de la "boite"
	// Box initialisation

	// Time : from year 0 to year 9999
	tTemps.init();
	_Box.setLeft(tTemps);
	tTemps.setNoLimit();
	_Box.setRight(tTemps);
	// Height : from 0 to NSLdvView::dLargeurLigne
	_Box.setTop(_pWorkingArea->getBottomMargin() + _pWorkingArea->getLargeurBaseline()) ;
	_Box.setBottom(_pWorkingArea->getBottomMargin()) ;

	_iIndex   = -1 ;
	_iZOrder  = LEVEL_BASELINE ;
  _toonType = toonLine ;
}

NSBaseLineView::NSBaseLineView(const NSBaseLineView& rv)
               :NSLigneView(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv));
}

NSBaseLineView::~NSBaseLineView()
{
  lObjectCount-- ;
}

void
NSBaseLineView::SetupWindow()
{
}

voidNSBaseLineView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	NVLdVPoint ldvTopLeft(_pView) ;
	ldvTopLeft.setX(pRectARepeindre->getLeft()) ;
	ldvTopLeft.setY(long(_pWorkingArea->getBottomMargin() + _pWorkingArea->getLargeurBaseline())) ;
	NVLdVPoint ldvBottomRight(_pView) ;
	ldvBottomRight.setX(pRectARepeindre->getRight()) ;
	ldvBottomRight.setY(long(_pWorkingArea->getBottomMargin())) ;

	NS_CLASSLIB::TPoint ptTopLeft = _pWorkingArea->getAreaPhysicalPoint(ldvTopLeft) ;
	NS_CLASSLIB::TPoint ptBotRigh = _pWorkingArea->getAreaPhysicalPoint(ldvBottomRight) ;

	//OWL::TBrush PinceauBleu(NS_CLASSLIB::TColor::LtBlue);
	pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBotRigh.x, ptBotRigh.y, OWL::TBrush(_pView->pPinceauBleu->GetHandle())) ;

	// détermination de la VisibleBox
  if (bGlobal)
	  _VisibleBox.initialise(ldvTopLeft.getX(),
                           ldvBottomRight.getX(),
			   									 ldvBottomRight.getY(),
				  								 ldvTopLeft.getY()) ;

	// pDc->Rectangle(ptTopLeft, ptBotRigh);
}
catch (...)
{
  string sErrorText = string("Exception NSBaseLineView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSBaseLineView::EvRButtonDown()
{
}

NSBaseLineView&
NSBaseLineView::operator=(const NSBaseLineView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

  return *this ;
}

// -----------------------------------------------------------------------------
//                               NSLdvCurvePoint
// -----------------------------------------------------------------------------

NSLdvCurvePoint::NSLdvCurvePoint(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvCurve* pTheCurve, NSHistoryValueManagement* pHistoryValue)
                :NSLdvTimeToon(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_pCurve           = pTheCurve ;

	_pPreviousBrother = NULL ;
  _pNextBrother     = NULL ;

  _value            = *pHistoryValue ;

  _dValue           = 0 ;
  _sUnitSens        = string("") ;

  // When the point is created, the Y axis is not initialized yet (since the
  // axis is configured from the array of points). So the top and bottom
  // information are not set up in the constructor, but in SetupWindow()
  //
  NVLdVTemps pointDate ;
  pointDate.initFromDate(_value.getDate()) ;

  _Box.setLeft(pointDate) ;
  _Box.setRight(pointDate) ;

  _iFixedWidthInPixels = 2 * _pCurve->getPointRadius() ;

	_iZOrder  = LEVEL_CURVES ;
	_toonType = toonCurvePoint ;
}

NSLdvCurvePoint::NSLdvCurvePoint(const NSLdvCurvePoint& rv)
                :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

  _pCurve           = rv._pCurve ;

	_value            = rv._value ;

  _dValue           = rv._dValue ;
  _sUnitSens        = rv._sUnitSens ;

	_pPreviousBrother = rv._pPreviousBrother ;
  _pNextBrother     = rv._pNextBrother ;
}

NSLdvCurvePoint::~NSLdvCurvePoint()
{
  lObjectCount-- ;
}

void
NSLdvCurvePoint::SetupWindow()
{
	bool bSuccess ;
	int iYcoordinate = _pCurve->giveYCoordinateForValue(_dValue, &bSuccess, _sUnitSens) ;

	_Box.setTop(iYcoordinate) ;
	_Box.setBottom(iYcoordinate) ;
}

void
NSLdvCurvePoint::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal)
{
try
{
  string ps = string("Drawing curve point ") + _value.getLabel() + string(" ") + _value.getDate() ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	// Sauvegarde des éléments en cours (pen et brush)
	HPEN oldPen     = (HPEN) pDc->GetCurrentObject(OBJ_PEN) ;
	HBRUSH oldBrush = (HBRUSH) pDc->GetCurrentObject(OBJ_BRUSH) ;

  OWL::TPen curvePen(_pCurve->_color, _pCurve->getDotWidth(), _pCurve->getDotStyle()) ;
  ::SelectObject(pDc->GetHDC(), curvePen) ;

  OWL::TBrush curvePointsBrush(_pCurve->_color) ;
  ::SelectObject(pDc->GetHDC(), curvePointsBrush) ;

	// Drawing the lines
  //
  NVLdVPoint PtThis(_pView) ;
  PtThis.setX(_Box.getLeft()) ;
  PtThis.setY(_Box.getTop()) ;

  NS_CLASSLIB::TPoint physicalThis = _pWorkingArea->getScrollablePhysicalPoint(PtThis) ;

  if (_pPreviousBrother)
  {
  	NVLdVPoint PtPrevious(_pView) ;
    PtPrevious.setX(_pPreviousBrother->_Box.getLeft()) ;
  	PtPrevious.setY(_pPreviousBrother->_Box.getTop()) ;
  	NS_CLASSLIB::TPoint physicalPrevious = _pWorkingArea->getScrollablePhysicalPoint(PtPrevious) ;
    pDc->MoveTo(physicalPrevious) ;
    pDc->LineTo(physicalThis) ;
  }
  else
  	pDc->MoveTo(physicalThis) ;

  if (_pNextBrother)
  {
  	NVLdVPoint PtNext(_pView) ;
    PtNext.setX(_pNextBrother->_Box.getLeft()) ;
  	PtNext.setY(_pNextBrother->_Box.getTop()) ;
  	NS_CLASSLIB::TPoint physicalNext = _pWorkingArea->getScrollablePhysicalPoint(PtNext) ;
    pDc->LineTo(physicalNext) ;
  }

  // Drawing the point
  //
  drawPoint(pDc, physicalThis, &curvePointsBrush, &curvePen) ;

	// Restauration des objets initiaux (pen et brush)
	::SelectObject(pDc->GetHDC(), oldPen) ;
	::SelectObject(pDc->GetHDC(), oldBrush) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCurvePoint::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvCurvePoint::drawPoint(TDC* pDc, NS_CLASSLIB::TPoint screenPoint, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
try
{
	NS_CLASSLIB::TRect pointDrawingBox(screenPoint.X() - _pCurve->getPointRadius(),
                                     screenPoint.Y() - _pCurve->getPointRadius(),
                                     screenPoint.X() + _pCurve->getPointRadius(),
                                     screenPoint.Y() + _pCurve->getPointRadius()) ;

  switch(_pCurve->getPointAspect())
  {
  	case NSLdvCurve::paCircle :
    	drawPointCircle(pDc, pointDrawingBox, pCurvePointsBrush, pCurvePen) ;
      break ;
    case NSLdvCurve::paSquare :
    	drawPointSquare(pDc, pointDrawingBox, pCurvePointsBrush, pCurvePen) ;
      break ;
    case NSLdvCurve::paTriangle :
    	drawPointUpTriangle(pDc, pointDrawingBox, pCurvePointsBrush, pCurvePen) ;
      break ;
    case NSLdvCurve::paStar :
    	drawPointStar(pDc, pointDrawingBox, pCurvePointsBrush, pCurvePen) ;
      break ;
  }
}
catch (...)
{
  string sErrorText = string("Exception NSLdvCurvePoint::drawPoint.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

void
NSLdvCurvePoint::drawPointCircle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
	pDc->Ellipse(pointDrawingBox) ;
}

void
NSLdvCurvePoint::drawPointSquare(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
	pDc->FillRect(pointDrawingBox, *pCurvePointsBrush) ;
}

void
NSLdvCurvePoint::drawPointUpTriangle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
	NS_CLASSLIB::TPoint trianglePoints[3] ;
  trianglePoints[0] = NS_CLASSLIB::TPoint(pointDrawingBox.Left(), pointDrawingBox.Bottom()) ;
  trianglePoints[1] = NS_CLASSLIB::TPoint(pointDrawingBox.Right(), pointDrawingBox.Bottom()) ;
  trianglePoints[2] = NS_CLASSLIB::TPoint(pointDrawingBox.Left() + pointDrawingBox.Width() / 2, pointDrawingBox.Top()) ;

  pDc->Polygon(trianglePoints, 3) ;
}

void
NSLdvCurvePoint::drawPointDownTriangle(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
	NS_CLASSLIB::TPoint trianglePoints[3] ;
  trianglePoints[0] = NS_CLASSLIB::TPoint(pointDrawingBox.Left(), pointDrawingBox.Top()) ;
  trianglePoints[1] = NS_CLASSLIB::TPoint(pointDrawingBox.Right(), pointDrawingBox.Top()) ;
  trianglePoints[2] = NS_CLASSLIB::TPoint(pointDrawingBox.Left() + pointDrawingBox.Width() / 2, pointDrawingBox.Bottom()) ;

  pDc->Polygon(trianglePoints, 3) ;
}

void
NSLdvCurvePoint::drawPointStar(TDC* pDc, NS_CLASSLIB::TRect pointDrawingBox, OWL::TBrush* pCurvePointsBrush, OWL::TPen* pCurvePen)
{
	// Taking the upper 2/3 rect and drawing an Up triangle
  //
	NS_CLASSLIB::TRect upperTriangleDrawingBox(pointDrawingBox.Left(),
                                             pointDrawingBox.Top(),
                                             pointDrawingBox.Right(),
                                             pointDrawingBox.Top() + pointDrawingBox.Height() * 2 / 3) ;
  drawPointUpTriangle(pDc, upperTriangleDrawingBox, pCurvePointsBrush, pCurvePen) ;

  // Taking the lower 2/3 rect and drawing a Down triangle
  //
	upperTriangleDrawingBox = NS_CLASSLIB::TRect(pointDrawingBox.Left(),
                                               pointDrawingBox.Top() + pointDrawingBox.Height() * 2 / 3,
                                               pointDrawingBox.Right(),
                                               pointDrawingBox.Bottom() ) ;
  drawPointDownTriangle(pDc, upperTriangleDrawingBox, pCurvePointsBrush, pCurvePen) ;
}

void
NSLdvCurvePoint::EvRButtonDown()
{
}

void
NSLdvCurvePoint::setActualValues(double dNewValue, string sNewUnitSens)
{
	_dValue    = dNewValue ;
  _sUnitSens = sNewUnitSens ;
}

void
NSLdvCurvePoint::resetActualValues()
{
	_dValue    = 0 ;
  _sUnitSens = string("") ;
}

NSLdvCurvePoint&
NSLdvCurvePoint::operator=(const NSLdvCurvePoint& src)
{
	if (this == &src)
		return *this ;

  _pCurve           = src._pCurve ;

	_value            = src._value ;

  _dValue           = src._dValue ;
  _sUnitSens        = src._sUnitSens ;

	_pPreviousBrother = src._pPreviousBrother ;
  _pNextBrother     = src._pNextBrother ;

  return *this ;
}

// --------------------------------------------------------------------------
// ----------------------- METHODES DE NSSsObjView --------------------------
// --------------------------------------------------------------------------

NSSsObjView::NSSsObjView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvSousObjet* pSsObj, NSLdvObjetView *pObjV, NSConcernView *pPbV)
            :NSLdvTimeToon(pCtx, pWorkArea, false)
{
  lObjectCount++ ;

	_pSsObjet  = pSsObj ;

	_pObjetView   = pObjV ;
	_pConcernView = pPbV ;

  if (_pObjetView)
  	_bVerticalScrollEnabled = _pObjetView->isVerticalScrollEnabled() ;

	_Box.setLeft(_pSsObjet->tDateHeureDebut) ;
	_Box.setRight(_pSsObjet->tDateHeureFin) ;

	if (pPbV)
	{
		_Box.setTop(pPbV->_Box.getTop()) ;
		_Box.setBottom(pPbV->_Box.getBottom()) ;
	}
	else
	{
		_Box.setTop(500) ;
		_Box.setBottom(0) ;
	}

	if (_pObjetView && (LEVEL_BASE_OBJECTS + RELOBLEVEL_OBJECT == _pObjetView->_iZOrder))
  	_iZOrder  = LEVEL_BASE_OBJECTS + RELOBLEVEL_SSOBJT ;
  else
		_iZOrder  = LEVEL_OBJECTS + RELOBLEVEL_SSOBJT ;

	_toonType = toonObjectPart ;
}

NSSsObjView::NSSsObjView(const NSSsObjView& rv)            :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	initialiser((NSLdvTimeToon*)(&rv)) ;

	_pSsObjet     = rv._pSsObjet ;
	_pObjetView   = rv._pObjetView ;
	_pConcernView = rv._pConcernView ;
}

NSSsObjView::~NSSsObjView(){
  lObjectCount-- ;
}

void
NSSsObjView::SetupWindow()
{
}

voidNSSsObjView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	NVLdVPoint ldvTopLeft(_pView) ;
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (_pSsObjet->tDateHeureDebut > tLeftTime)
		tLeftTime = _pSsObjet->tDateHeureDebut ;

	ldvTopLeft.setX(tLeftTime) ;
	if (_pConcernView)
		ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_pConcernView->getIndex()) - 2) ;

	NS_CLASSLIB::TPoint ptTopLeft ;
	if (_bVerticalScrollEnabled)
		ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	else
  	ptTopLeft = _pWorkingArea->getAreaPhysicalPoint(ldvTopLeft) ;

	// choix de l'icône

	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// attention l'icone depend du type de l'objet. Le type de l'objet est donné
	// par la variable "sTypeDoc" du NSLdvObjet correspondant
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// chemin : this->pObjetView->pObjet->sTypeDoc
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	//OWL::TIcon  icObjet(*(pView->GetApplication()), (TResId)IDI_LDVSSOBJ16) ;
	//ICONINFO iconInfo;
	//icObjet.GetIconInfo(&iconInfo);
	//pDc->DrawIcon(ptTopLeft.X() - 8, ptTopLeft.Y(), icObjet, 16, 16) ;

	// On donne à DrawIcon le point en haut à gauche du rectangle qui doit contenir le dessin
	pDc->DrawIcon(ptTopLeft.X() - ICONWIDTH / 2, ptTopLeft.Y(), *(_pView->pLdvSsObject), ICONWIDTH, ICONWIDTH) ;

  if (bGlobal)
  {
	  NS_CLASSLIB::TRect PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft.X() - ICONWIDTH / 2,
																													ptTopLeft.Y(),
																													ptTopLeft.X() + ICONWIDTH / 2,
                                                          ptTopLeft.Y() + ICONWIDTH);

	  _VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;
  }

  // pDc->RestorePen();
}
catch (...)
{
  string sErrorText = string("Exception NSSsObjView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}


void
NSSsObjView::drawInBasket(TDC *pDc, NVLdVRect *pRectARepeindre)
{
}


void
NSSsObjView::EvRButtonDown()
{
}

void
NSSsObjView::createComponents(ArrayOfToons* pToonsArray)
{
}

NSSsObjView&
NSSsObjView::operator=(const NSSsObjView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src));

	_pSsObjet     = src._pSsObjet ;
	_pObjetView   = src._pObjetView ;
	_pConcernView = src._pConcernView ;

  return *this ;
}


// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvObjetView ------------------------
// --------------------------------------------------------------------------

NSLdvObjetView::NSLdvObjetView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSLdvObjet* pObj, NSLigneView *pLineView)
               :NSLdvTimeToon(pCtx, pWorkArea, pLineView->isVerticalScrollEnabled())
{
  lObjectCount++ ;

	_pObjet     = pObj ;
  _pLigneView = pLineView ;

  _Box.setLeft(_pObjet->getDateHeureDebut()) ;
  _Box.setRight(_pObjet->getDateHeureFin()) ;

  _Box.setTop(pLineView->_Box.getTop() - 5) ;
  _Box.setBottom(pLineView->_Box.getBottom() + 5) ;

  NSLigneView* pBaseLine = _pWorkingArea->getToonsArray()->getBaseLine() ;
  if (pBaseLine == _pLigneView)
  	_iZOrder  = LEVEL_BASE_OBJECTS + RELOBLEVEL_OBJECT ;
  else
  	_iZOrder  = LEVEL_OBJECTS + RELOBLEVEL_OBJECT ;

  _toonType = toonObject ;
}


NSLdvObjetView::NSLdvObjetView(const NSLdvObjetView& rv)
               :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

  initialiser((NSLdvTimeToon*)(&rv)) ;

  _pObjet     = rv._pObjet ;
  _pLigneView = rv._pLigneView ;
}

NSLdvObjetView::~NSLdvObjetView()
{
  lObjectCount-- ;
}

void		NSLdvObjetView::createComponents(ArrayOfToons* pToonsArray)
{
/*
	if (!(pObjet->aSousObjets.empty()))
  {
  	ArraySsObjIter i = pObjet->aSousObjets.begin();
    for (; i != pObjet->aSousObjets.end(); i++)
    {
    	NSSsObjView* pSsObjView = new NSSsObjView(pContexte, pView, *i);
      pToonsArray->push_back(new NSSsObjView(*pSsObjView));
      pSsObjView->createComponents(pToonsArray);
      delete pSsObjView;
    }
  }
*/
}

void
NSLdvObjetView::SetupWindow()
{
}

voidNSLdvObjetView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	NVLdVPoint ldvTopLeft(_pView);
	// Left time
	NVLdVTemps tLeftTime = pRectARepeindre->getLeft();
	if (_pObjet->getDateHeureDebut() > tLeftTime)
		tLeftTime = _pObjet->getDateHeureDebut() ;

	ldvTopLeft.setX(tLeftTime);	ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_pLigneView->getIndex()) - 2/*Box.getTop()*/);

  NS_CLASSLIB::TPoint ptTopLeft ;
	if (_bVerticalScrollEnabled)
		ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	else
  	ptTopLeft = _pWorkingArea->getAreaPhysicalPoint(ldvTopLeft) ;

	// choix de l'icône	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// attention l'icone depend du type de l'objet. Le type de l'objet est donné
	// par la variable "sTypeDoc" du NSLdvObjet correspondant
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// chemin : this->pObjet->sTypeDoc
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// OWL::TIcon  icObjet(*(pView->GetApplication()), (TResId)IDI_LDVOBJET16) ;	// ICONINFO iconInfo;
	// icObjet.GetIconInfo(&iconInfo);
	// pDc->DrawIcon(ptTopLeft.X(), ptTopLeft.Y(), icObjet, 16, 16) ;

	NS_CLASSLIB::TRect PixelVisibleBox ;
  _iFixedWidthInPixels = drawIcon(pDc, &ptTopLeft, &PixelVisibleBox) ;

  if (bGlobal)
    _VisibleBox = (NVLdVRect&) _pWorkingArea->getAreaLogicalRectFromWindowRect(PixelVisibleBox) ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvObjetView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

int
NSLdvObjetView::drawIcon(TDC* pDc, NS_CLASSLIB::TPoint* pPtTopLeft, NS_CLASSLIB::TRect* pPixelVisibleBox, bool bDontDraw)
{
try
{
  if ((NULL == pDc) || (NULL == pPtTopLeft))
    return 0 ;

  int iFixedWidthInPixels = 0 ;

  nsSkin* pSkin = (nsSkin*) 0 ;

  // Check if there is a "score specific" icon
  //
  NSQualityOfLifeChart* pChart = _pObjet->getQolChart() ;
  if (pChart)
  {
    string sSkinTitle = pChart->getScoreIcon() ;
    if (string("") != sSkinTitle)
      pSkin = pContexte->getSkins()->donneSkin(sSkinTitle) ;
  }

  if ((NULL == pSkin) && (string("") != _pObjet->getLexique()))
    pSkin = pContexte->getSkins()->donneSkinForLexique(_pObjet->getLexique()) ;

  // There is a skin for this concept
  //
  if (pSkin && pSkin->getBackBmp())
  {
  	OWL::TDib* pBackgrnd = pSkin->getBackBmp() ;

    iFixedWidthInPixels = pBackgrnd->Width() ;

    NS_CLASSLIB::TPoint dibOrigin ;
    // bool bBottomUp ;
    //
    // Bottom up dib : origin lies at the lower left corner
    //
    if (pBackgrnd->Height() > 0)
    {
    	// bBottomUp = true ;
      dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
    }
    //
    // top down dib : origin lies at the upper left corner
    //
    else
    {
    	// bBottomUp = false ;
      dibOrigin = NS_CLASSLIB::TPoint(0, 0) ;
    }

    // NS_CLASSLIB::TRect winRect(ptTopLeft.X(), ptTopLeft.Y(),
    //                                    ptTopLeft.X() + pBackgrnd->Width(),
    //                                    ptTopLeft.Y() + pBackgrnd->Height());

		int iLeftExcess = pBackgrnd->Width() / 2 ;
    int iRightExcess = pBackgrnd->Width() - iLeftExcess ;
    NS_CLASSLIB::TRect winRect(pPtTopLeft->X() - iLeftExcess,
                               pPtTopLeft->Y(),
                               pPtTopLeft->X() + iRightExcess,
                               pPtTopLeft->Y() + pBackgrnd->Height()) ;

    //  Using SetDIBitsToDevice
    //
    // pDc->SetDIBitsToDevice(winRect, dibOrigin, *pBackgrnd) ;

    // Using BitBlt
    //
    if (false == bDontDraw)
    {
      HDC hdcMem = CreateCompatibleDC(pDc->GetHDC()) ;

      HBITMAP bitmap = ::CreateDIBitmap(
               *pDc,
               (LPBITMAPINFOHEADER)pBackgrnd->GetInfoHeader(),
               CBM_INIT,
               (const uint8 far*)pBackgrnd->GetBits(),
               (LPBITMAPINFO)pBackgrnd->GetInfo(),
               pBackgrnd->Usage()
            ) ;

      HGDIOBJ hbmOld = SelectObject(hdcMem, bitmap) ;

      BITMAP bm ;
      GetObject(bitmap, sizeof(bm), &bm) ;
      int x = bm.bmWidth ;
      int y = bm.bmHeight ;

      // pDc->BitBlt(winRect.Left(), winRect.Top(), x, y, hdcMem, 0, 0, SRCCOPY) ;
      TransparentBlt(*pDc, winRect.Left(), winRect.Top(), winRect.Width(), winRect.Height(), hdcMem, 0, 0, x, y, RGB(255, 0, 255)) ;

      DeleteObject(bitmap) ;

      SelectObject(hdcMem, hbmOld) ;
      DeleteDC(hdcMem) ;
    }

    if (pPixelVisibleBox)
      *pPixelVisibleBox = winRect ;

    /*
    PixelVisibleBox = NS_CLASSLIB::TRect(   ptTopLeft.X(),
                                                ptTopLeft.Y(),
                                                ptTopLeft.X() + 16,
                                                ptTopLeft.Y() + 16); */
	}
  //
  // Icon standard
  //
  else
  {
  	// On donne à DrawIcon le point en haut à gauche du rectangle qui doit contenir le dessin
    // pDc->DrawIcon(pPtTopLeft->X(), pPtTopLeft->Y(), *(pView->pLdvObject), 16, 16) ;

    if (false == bDontDraw)
      pDc->DrawIcon(pPtTopLeft->X() - ICONWIDTH / 2, pPtTopLeft->Y(), *(_pView->pLdvObject), ICONWIDTH, ICONWIDTH) ;

    //PixelVisibleBox = NS_CLASSLIB::TRect(ptTopLeft.X(),
    //                                            ptTopLeft.Y(),
    //                                            ptTopLeft.X() + 16,
    //                                            ptTopLeft.Y() + 16) ;
    if (pPixelVisibleBox)
      *pPixelVisibleBox = NS_CLASSLIB::TRect(pPtTopLeft->X() - ICONWIDTH / 2,
                                             pPtTopLeft->Y(),
                                             pPtTopLeft->X() + ICONWIDTH / 2,
                                             pPtTopLeft->Y() + ICONWIDTH) ;
    iFixedWidthInPixels = ICONWIDTH ;
  }

  return iFixedWidthInPixels ;
}
catch (...)
{
  string sErrorText = string("Exception NSLdvObjetView::drawIcon.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
  return 0 ;
}
}

voidNSLdvObjetView::EvRButtonDown(){
}

NSLdvObjetView&NSLdvObjetView::operator=(const NSLdvObjetView& src){
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

	_pObjet = src._pObjet ;

  return *this ;}

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSLdvCoopChartView -----------------------
// --------------------------------------------------------------------------

NSQualityOfLifeChartView::NSQualityOfLifeChartView(NSContexte* pCtx, NSLdvViewArea *pWorkArea, NSQualityOfLifeChart* pCC, NSLigneView *pLineView)
                         :NSLdvTimeToon(pCtx, pWorkArea, pLineView->isVerticalScrollEnabled())
{
  lObjectCount++ ;

	_pQoLChart  = pCC ;
  _pLigneView = pLineView ;

  _Box.setLeft(_pQoLChart->getTpsStart()) ;
  _Box.setRight(_pQoLChart->getTpsInfo()) ;

  _Box.setTop(pLineView->_Box.getTop() - 5) ;
  _Box.setBottom(pLineView->_Box.getBottom() + 5) ;

  NSLigneView* pBaseLine = _pWorkingArea->getToonsArray()->getBaseLine() ;
  if (pBaseLine == _pLigneView)
  	_iZOrder  = LEVEL_BASE_OBJECTS + RELOBLEVEL_COOP ;
  else
  	_iZOrder  = LEVEL_OBJECTS + RELOBLEVEL_COOP ;

  _toonType = toonCoop ;
}

NSQualityOfLifeChartView::NSQualityOfLifeChartView(const NSQualityOfLifeChartView& rv)
                         :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

  initialiser((NSLdvTimeToon*)(&rv)) ;

  _pQoLChart  = rv._pQoLChart ;
  _pLigneView = rv._pLigneView ;
}

NSQualityOfLifeChartView::~NSQualityOfLifeChartView()
{
  lObjectCount-- ;
}

void
NSQualityOfLifeChartView::createComponents(ArrayOfToons* pToonsArray)
{
}

void
NSQualityOfLifeChartView::SetupWindow()
{
}

voidNSQualityOfLifeChartView::draw(TDC* pDc, NVLdVRect* pRectARepeindre, bool bGlobal){
try
{
	// Left time
  //
  NVLdVPoint ldvTopLeft(_pView) ;

	NVLdVTemps tLeftTime = pRectARepeindre->getLeft() ;
	if (_pQoLChart->getTpsStart() > tLeftTime)
		tLeftTime = _pQoLChart->getTpsStart() ;

	ldvTopLeft.setX(tLeftTime);	ldvTopLeft.setY(_pWorkingArea->preoccupDonneTopLine(_pLigneView->getIndex()) - 2/*Box.getTop()*/);

  NS_CLASSLIB::TPoint ptTopLeft ;
	if (_bVerticalScrollEnabled)
		ptTopLeft = _pWorkingArea->getScrollablePhysicalPoint(ldvTopLeft) ;
	else
  	ptTopLeft = _pWorkingArea->getAreaPhysicalPoint(ldvTopLeft) ;

  // Right time
  //
  NVLdVPoint ldvBottomRight(_pView) ;

	NVLdVTemps tRightTime = pRectARepeindre->getRight() ;
	if (_pQoLChart->getTpsInfo() < tRightTime)
		tRightTime = _pQoLChart->getTpsInfo() ;

	ldvBottomRight.setX(tRightTime) ;	ldvBottomRight.setY(_pWorkingArea->preoccupDonneTopLine(_pLigneView->getIndex()) - 7/*Box.getTop()*/);

  NS_CLASSLIB::TPoint ptBottomRight ;
	if (_bVerticalScrollEnabled)
		ptBottomRight = _pWorkingArea->getScrollablePhysicalPoint(ldvBottomRight) ;
	else
  	ptBottomRight = _pWorkingArea->getAreaPhysicalPoint(ldvBottomRight) ;

  NSCoopChart *pCoopChart = TYPESAFE_DOWNCAST(_pQoLChart, NSCoopChart) ;
  if (pCoopChart)
  {
    //
    ldvBrush* pBrush = getBrushForScore(pCoopChart->getPhysicalScore()) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y, ptBottomRight.x, ptTopLeft.y + 2, OWL::TBrush(pBrush->GetHandle())) ;

    pBrush = getBrushForScore(pCoopChart->getEmotionsScore()) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y + 3, ptBottomRight.x, ptTopLeft.y + 5, OWL::TBrush(pBrush->GetHandle())) ;

    pBrush = getBrushForScore(pCoopChart->getDaylyTasksScore()) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y + 6, ptBottomRight.x, ptTopLeft.y + 8, OWL::TBrush(pBrush->GetHandle())) ;

    pBrush = getBrushForScore(pCoopChart->getSocialScore()) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y + 9, ptBottomRight.x, ptTopLeft.y + 11, OWL::TBrush(pBrush->GetHandle())) ;

    pBrush = getBrushForScore(pCoopChart->getHealthScore()) ;
    pDc->FillRect(ptTopLeft.x, ptTopLeft.y + 12, ptBottomRight.x, ptTopLeft.y + 14, OWL::TBrush(pBrush->GetHandle())) ;
  }
  else
  {
    NSQualityOfLifeInformation* pQoL = _pQoLChart->getQolInfo() ;
    if (pQoL)
    {
      NSCoopLineArray* pLines = pQoL->getLines() ;
      if (pLines && (false == pLines->empty()))
      {
        ldvBrush* pBrush ;
        int iLineNb = 0 ;
        for (CoopLineIter lineIt = pLines->begin() ; pLines->end() != lineIt ; lineIt++, iLineNb++)
        {
          int iEquivScore = 0 ;
          switch ((*lineIt)->getLineColor())
          {
            case colorBlue   : iEquivScore = 1 ; break ;
            case colorGreen  : iEquivScore = 2 ; break ;
            case colorYellow : iEquivScore = 3 ; break ;
            case colorOrange : iEquivScore = 4 ; break ;
            case colorRed    : iEquivScore = 5 ; break ;
          }
          pBrush = getBrushForScore(iEquivScore) ;

          int iTopY = ptTopLeft.y + 3 * iLineNb ;

          pDc->FillRect(ptTopLeft.x, iTopY, ptBottomRight.x, iTopY + 2, OWL::TBrush(pBrush->GetHandle())) ;
        }
      }
    }
  }

	// détermination de la VisibleBox
  if (bGlobal)
	  _VisibleBox.initialise(ldvTopLeft.getX(),
                          ldvBottomRight.getX(),
                          ldvBottomRight.getY(),
                          ldvTopLeft.getY()) ;
}
catch (...)
{
  string sErrorText = string("Exception NSQualityOfLifeChartView::draw.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}
voidNSQualityOfLifeChartView::EvRButtonDown(){
}

ldvBrush*
NSQualityOfLifeChartView::getBrushForScore(int iScore)
{
  switch (iScore)
  {
    case 1 : return _pView->pPinceauS1 ;
    case 2 : return _pView->pPinceauS2 ;
    case 3 : return _pView->pPinceauS3 ;
    case 4 : return _pView->pPinceauS34 ;
    case 5 : return _pView->pPinceauS4 ;
  }

  return _pView->pPinceauBleu ;
}

NSQualityOfLifeChartView&NSQualityOfLifeChartView::operator=(const NSQualityOfLifeChartView& src){
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

	_pQoLChart = src._pQoLChart ;

  return *this ;}

// -----------------------------------------------------------------------------// class NSLdvTankView
// -----------------------------------------------------------------------------

NSLdvTankView::NSLdvTankView(NSLdvView *pView, NSContexte *pCtx)
              :NSRoot(pCtx)
{
  lObjectCount++ ;

	_pLdvView = pView ;

  updateTankList() ;
}

void
NSLdvTankView::updateTankList()
{
	if (_pLdvView->_aToons.empty())
		return ;

	for (ToonsIter toonIter = _pLdvView->_aToons.begin() ; _pLdvView->_aToons.end() != toonIter ; toonIter++)
	{
		NSSsObjView *pSsObjView = TYPESAFE_DOWNCAST(*toonIter, NSSsObjView) ;
		if (pSsObjView)
		{
			// L'objet appartient-il à la liste ?
      // Does this sub-object belong to the orphean list
			// bool    bAlreadyExists = false ;
			if (false == _aSsObjetTankArray.empty())
			{
				SsObjViewIterator SsObjVIter = _aSsObjetTankArray.begin() ;
				for ( ; (_aSsObjetTankArray.end() != SsObjVIter) &&  ((*SsObjVIter) != pSsObjView); SsObjVIter++)
					;

				// Il n'appartient pas à la liste
				if (_aSsObjetTankArray.end() == SsObjVIter)
				{
					// S'il est orphelin, on l'ajoute à la liste
					if (string("") == pSsObjView->_pSsObjet->sConcern)
						_aSsObjetTankArray.push_back(pSsObjView) ;
				}

				// Il appartient à la liste
				else
				{
					// S'il n'est pas orphelin, on l'enlève
					if (string("") != pSsObjView->_pSsObjet->sConcern)
					{
						_aSsObjetTankArray.erase(SsObjVIter) ;

						// Si c'était le dernier sous-objet du réservoir et
						// qu'Episodus est en mode "Episode", on passe au stade
            // suivant
						if ((_aSsObjetTankArray.empty()) &&
								(pContexte->getEpisodus()->GetEpisoState() == NSEpisodus::stateEpisod))
							pContexte->getEpisodus()->SetEpisoState(NSEpisodus::stateMessage) ;
					}
				}
			}

			// La liste est vide
			else
			{
      	if (string("") == pSsObjView->_pSsObjet->sConcern)
					_aSsObjetTankArray.push_back(pSsObjView) ;
			}
		}
	}
}

NSLdvTankView::~NSLdvTankView()
{
  lObjectCount-- ;
}

NS_CLASSLIB::TRect
NSLdvTankView::coords()
{
  return _rectCoords ;
}

bool
NSLdvTankView::setCoords(NS_CLASSLIB::TRect rect)
{
	_rectCoords = rect ;
  return true ;
}

bool
NSLdvTankView::setCoords(int left, int top, int right, int bottom)
{
	_rectCoords = NS_CLASSLIB::TRect(left, top, right, bottom) ;
  return true ;
}

void
NSLdvTankView::affiche(TDC *pDc, NS_CLASSLIB::TRect rectARepeindre)
{
try
{
	updateTankList() ;

	if (_aSsObjetTankArray.empty())
  	return;

	NS_CLASSLIB::TPoint ptTopLeft = _rectCoords.TopLeft() ;

	for (SsObjViewIterator SsObjVIter = _aSsObjetTankArray.begin() ; _aSsObjetTankArray.end() != SsObjVIter ; SsObjVIter++)
	{
		OWL::TIcon  icObjet(*(_pLdvView->GetApplication()), (TResId)IDI_LDVSSOBJ16) ;
		ICONINFO iconInfo;
		icObjet.GetIconInfo(&iconInfo);
		pDc->DrawIcon(ptTopLeft.X(), ptTopLeft.Y(), icObjet, ICONWIDTH, ICONWIDTH) ;

		NS_CLASSLIB::TRect  rectDisplay     = NS_CLASSLIB::TRect(ptTopLeft.X() + ICONWIDTH, ptTopLeft.Y(), ptTopLeft.X() + ICONWIDTH, ptTopLeft.Y() + ICONWIDTH) ;
		NS_CLASSLIB::TRect  rect2Compare    = rectDisplay ;
		pDc->DrawText((*SsObjVIter)->_pSsObjet->sTitre.c_str(), -1, rect2Compare, DT_CALCRECT) ;
		if (rectDisplay.Width() < rect2Compare.Width())
		{
			int decal = rect2Compare.Width() ;
//    rectDisplay.left -= decal ;
			rectDisplay.right += decal ;
		}

		// (*SsObjVIter)->Box.initialise(&pLdvView->GetClientRect().BottomRight(), &rectDisplay, _pWorkingArea) ;
		// (*SsObjVIter)->VisibleBox.initialise(&pLdvView->GetClientRect().BottomRight(), &rectDisplay, _pWorkingArea) ;
		// pDc->DrawText((*SsObjVIter)->pSsObjet->sTitre.c_str(), -1, rectDisplay, DT_CENTER) ;

		ptTopLeft.y += ICONWIDTH ;

//  (*SsObjVIter)->drawInBasket(dc, rectarepeindre)
	}
}
catch (...)
{
  string sErrorText = string("Exception NSLdvTankView::affiche.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}
}

bool
NSLdvTankView::addSsObj(NSLdvSousObjet *pSsObj)
{
//	pSsObjetBasketArray->push_back(new NSLdvSousObjet(pSsObj)) ;
  return true ;
}

bool
NSLdvTankView::delSsObj(SsObjViewIterator SsObjIter)
{
	_aSsObjetTankArray.erase(SsObjIter) ;
	return true ;
}

// -----------------------------------------------------------------------------
// Méthodes de NSLdvGoalCycleView
// afichage des objectifs sur la Ligne de Vie
// -----------------------------------------------------------------------------

NSLdvGoalCycleView::NSLdvGoalCycleView(NSContexte *pCtx, NSLdvViewArea *pWorkArea, NSLdvGoal* pGo, NSLigneView *pPbView)
                   :NSLdvTimeToon(pCtx, pWorkArea, true)
{
  lObjectCount++ ;

	_pGoal      = pGo ;
	_pLigneView = pPbView ;

	/*
	Box.setLeft(pObjet->tDateHeureDebut) ;
	Box.setRight(pObjet->tDateHeureFin) ;

	Box.setTop(pLineView->Box.getTop() - 5) ;
	Box.setBottom(pLineView->Box.getBottom() + 5) ;
	*/

	_iZOrder  = LEVEL_BASELINE + RELPRLEVEL_GOAL ;
	_toonType = toonGoal ;
}


NSLdvGoalCycleView::NSLdvGoalCycleView(const NSLdvGoalCycleView& rv)
                   :NSLdvTimeToon(rv.pContexte, rv._pWorkingArea, rv._bVerticalScrollEnabled)
{
  lObjectCount++ ;

	_pGoal      = rv._pGoal ;
	_pLigneView = rv._pLigneView ;
}


NSLdvGoalCycleView::~NSLdvGoalCycleView()
{
  lObjectCount-- ;
}

void
NSLdvGoalCycleView::SetupWindow()
{
}

void
NSLdvGoalCycleView::draw(TDC *pDC, NVLdVRect *pRectARepeindre)
{
}

NSLdvGoalCycleView&
NSLdvGoalCycleView::operator=(const NSLdvGoalCycleView& src)
{
	if (this == &src)
		return *this ;

	initialiser((NSLdvTimeToon*)(&src)) ;

	return *this ;
}


// --------------------------------------------------------------------------
// ----------------------- METHODES DE ArrayOfToons -------------------------
// --------------------------------------------------------------------------

/*
ArrayOfToons::ArrayOfToons(ArrayOfToons& rv)
	:	ArrayLdvToon()
{
	try
	{
  	if (!(rv.empty()))
    	for (ToonsIter i = rv.begin(); i != rv.end(); i++)
      	push_back(new NSLdvToon(*(*i)));
	}
	catch (const exception& e)
	{
  	string sExept = "Exception " + string(e.what());
    erreur(sExept.c_str(), standardError, 0) ;
	}
	catch (...)
	{
    erreur("Exception.", standardError, 0) ;
	}
}
*/


NSLigneView*
ArrayOfToons::getBaseLine()
{
	if (empty())
		return (NSLigneView*) 0 ;

	for (ToonsIter  toonIter = begin() ; toonIter != end() ; toonIter++)
	{
		NSBaseLineView  *pBLView = TYPESAFE_DOWNCAST(*toonIter, NSBaseLineView) ;
		if (pBLView)
			return ((NSLigneView *) (*toonIter)) ;
	}

	return (NSLigneView*) 0 ;
}


NSLigneView*
ArrayOfToons::getLigneView(string sToon)
{
	if (string("") == sToon)
		return (getBaseLine()) ;

	if (empty())
		return (NSLigneView*) 0 ;

	NSConcernView *pPbView = getPbView(sToon) ;

	if (NULL == pPbView)
		return (getBaseLine()) ;
	else
		return ((NSLigneView *)(pPbView)) ;
}


NSConcernView*
ArrayOfToons::getPbView(string sToon)
{
	if ((empty()) || (string("") == sToon))
		return (NSConcernView*) 0 ;

	for (ToonsIter  toonIter = begin() ; end() != toonIter ; toonIter++)
	{
		NSConcernView* pPbView = TYPESAFE_DOWNCAST(*toonIter, NSConcernView) ;
		if ((pPbView) && (sToon == pPbView->getConcern()->_sReference))
			return (pPbView) ;
	}

	return (NSConcernView*) 0 ;
}

NSLdvObjetView*
ArrayOfToons::getObjView(string sToon)
{
	if ((empty()) || (string("") == sToon))
		return (NSLdvObjetView*) 0 ;

	for (ToonsIter  toonIter = begin() ; end() != toonIter ; toonIter++)
	{
		NSLdvObjetView  *pObjView = TYPESAFE_DOWNCAST(*toonIter, NSLdvObjetView) ;
		if ((pObjView) && (sToon == pObjView->_pObjet->getReference()))
			return pObjView ;
	}

	return (NSLdvObjetView*) 0 ;
}

void
ArrayOfToons::init(NSQualityOfLifeChartArray *pCharts, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
	if (pCharts->empty())
		return ;

	for (QualityOfLifeChartIter i = pCharts->begin() ; pCharts->end() != i ; i++)
	{
    NSLigneView *pLineView = getBaseLine() ;

    // If there is no line to plug this object to, then it is not displayed
    //
    if (pLineView)
    {
      NSQualityOfLifeChartView *pChartView = new NSQualityOfLifeChartView(pCtx, pWrk, *i, pLineView) ;
		  push_back(pChartView) ;
    }
	}
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(NSCoopChartArray).", standardError, 0) ;
	return ;
}
}

void
ArrayOfToons::init(ArrayObjets* pObj, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
	if (pObj->empty())
		return ;

	ToonsIter thisIter ;
	for (ArrayObjIter i = pObj->begin() ; pObj->end() != i ; i++)
	{
    bool bVisibleDoc = true ;

    // Management documents (healthteam, health index, etc) are not visible
    //
    string sLexique = (*i)->getLexique() ;
    if (pCtx->getSuperviseur()->isManagementDoc(sLexique))
      bVisibleDoc = false ;

    // Some docs are locally not visible ()
    //
    if (bVisibleDoc)
    {
      string sSemRoot = string("") ;
      pCtx->getDico()->donneCodeSens(&sLexique, &sSemRoot) ;
      if (string("ZADMI") == sSemRoot)
        bVisibleDoc = false ;
    }

    if (bVisibleDoc)
    {
		  NSLigneView *pLineView = getLigneView((*i)->getConcern()) ;

      // If there is no line to plug this object to, then it is not displayed
      //
      if (pLineView)
      {
		    NSLdvObjetView *pObjView = new NSLdvObjetView(pCtx, pWrk, *i, pLineView) ;
		    push_back(pObjView) ;

		    thisIter = end() ;
		    thisIter-- ;
		    pObjView = (NSLdvObjetView*)(*thisIter) ;
		    pObjView->createComponents(this) ;
      }
    }
	}
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(ArrayObjets).", standardError, 0) ;
	return ;
}
}

void
ArrayOfToons::init(ArraySsObjet* pSsObj, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
	if (pSsObj->empty())
		return ;

	ToonsIter thisIter ;
	for (ArraySsObjIter i = pSsObj->begin() ; pSsObj->end() != i ; i++)
	{
		NSLdvObjetView *pObjV = getObjView((*i)->sObject) ;

    if (pObjV)
    {
		  NSConcernView *pPbV = getPbView((*i)->sConcern) ;

		  NSSsObjView *pSsObjView = new NSSsObjView(pCtx, pWrk, *i, pObjV, pPbV) ;
		  push_back(pSsObjView) ;

		  thisIter = end() ;
		  thisIter-- ;
		  pSsObjView = (NSSsObjView *)(*thisIter) ;
		  pSsObjView->createComponents(this) ;
    }
	}
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(ArraySsObjet).", standardError, 0) ;
	return;
}
}

void
ArrayOfToons::initGoals(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk)
// ArrayOfToons::init(ArrayGoals *pGoals, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
/*
	if (pGoals->empty())
		return;

    NVLdVTemps tDebut;
    NVLdVTemps tFin;

	ToonsIter   thisIter ;
	for (ArrayGoalIter i = pGoals->begin() ; i != pGoals->end() ; i++)
	{
        NSLdvGoalCycleView *pGoalView = new NSLdvGoalCycleView(pCtx, pView, *i, pObjV, pPbV) ;

        tDebut = (*i)->tDateOuverture;
        //
        // On cherche les événements qui réinitialisent l'objectif
        //
        VecteurString   *pVecteurString = new VecteurString() ;
        pContexte->getGraphe()->TousLesVrais(sNoeud, NSGraphe::goalReseter, pVecteurString) ;
        if (!pVecteurString->empty())
        {
            pGoal->sConcern =  *(*(pVecteurString->begin()));
            EquiItemIter stringIter = pVecteurString->begin();
		    for (; stringIter != pVecteurString->end(); stringIter++)
            {
                string sResetNode = *(*stringIter);

                string sObjReference = string(sResetNode, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN);
	            // Recherche du document qui contient ce noeud
	            // Looking for the document that contains this node
	            NSHISTODocument* pHistory = pDoc->pContexte->getPatient()->pDocHis;

	            if (!(pHistory->VectDocument.empty()))
	            {
	                DocumentIter iterDoc = pHistory->VectDocument.TrouveDocHisto(sObjReference);
	                // Found
	                if ((iterDoc != NULL) && (iterDoc != pHistory->VectDocument.end()))
	                {
                        tFin.initFromDate(string((*iterDoc)->GetDateDoc()));
                        pGoalView->fixeRectangle(tDebut, tFin, 0, 0);
                        push_back(new NSLdvGoalView(*pGoalView)) ;
		                delete pGoalView ;
                    }
	            }
            }
        }
        delete pVecteurString ;

		NSLigneView *pLineView = getLigneView((*i)->sConcern);


		push_back(new NSSsObjView(*pSsObjView)) ;
		delete pSsObjView ;
		thisIter = end() ;
		thisIter-- ;
		pSsObjView = (NSSsObjView *)(*thisIter) ;
		pSsObjView->createComponents(this) ;
	}
    */
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(ArraySsObjet).", standardError, 0) ;
	return;
}
}

void
ArrayOfToons::initTreatments(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
  if ((NSFrameInformationArray*) NULL == pFrames)
    return ;

  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(iFrame) ;
    if (pFrameInfo)
    {
      ArrayLdvDrugs* pDrugs = pFrameInfo->getTreatmentsArray() ;
      if (pDrugs)
        init(iFrame, pDrugs, pCtx, pWrk) ;
    }
  }
}
catch (...)
{
	erreur("Exception ArrayOfToons::initTreatments.", standardError, 0) ;
	return;
}
}

void
ArrayOfToons::init(LDVFRAME iFrame, ArrayLdvDrugs *pDrugs, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
  if ((NULL == pDrugs) || pDrugs->empty())
		return ;

	int iIndex = 0 ;
	for (drugsIter i = pDrugs->begin() ; pDrugs->end() != i ; i++)
	{
  	// Searching the same drug to put it on the same line
    //
    int iRefIndex = -1 ;
    if (false == empty())
    	for (ToonsIter iToon = begin(); end() != iToon ; iToon++)
    		if (dynamic_cast<NSDrugLineView*>(*iToon))
    		{
        	NSDrugLineView* pDrugLine = (NSDrugLineView*)(*iToon) ;
          //
          // Check that they are refering to the same drug, and have no time overlap
          //
          if (pDrugLine && pDrugLine->getDrug() &&
              (pDrugLine->getDrug()->getLexique() == (*i)->getLexique()) &&
               ((pDrugLine->_Box.getLeft() > (*i)->_tDateOuverture) ||
                (pDrugLine->_Box.getRight() < (*i)->_tDateFermeture))
               )
          	iRefIndex = pDrugLine->getIndex() ;
        }

    if (-1 == iRefIndex)
    {
    	iRefIndex = iIndex ;
      iIndex++ ;
    }

		NSDrugLineView *pdrugLine = new NSDrugLineView(pCtx, pWrk, *i, iFrame, iRefIndex) ;
		push_back(pdrugLine) ;
	}
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(ArrayLdvDrugs).", standardError, 0) ;
	return;
}
}

/**
 *  The list of drugs must be reloaded
 */
void
ArrayOfToons::reinitDrugs(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
	viderType(NSLdvToon::toonDrug) ;

  initTreatments(pFrames, pCtx, pWrk) ;
}

void
ArrayOfToons::init(NSFrameInformationArray *pFrames, NSContexte* pCtx, NSLdvViewArea* pWrk)
{
try
{
  if ((NULL == pFrames) || pFrames->empty())
    return ;

	int iIndex = 0 ;

  int iSeverityThreshold = -1 ;
  if (pWrk && pWrk->getView())
    iSeverityThreshold = pWrk->getView()->getSeverityThreshold() ;

  // Première étape : créer les NSConcernView
	// First step : create all the NSConcernView
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;
    NSFrameInformation* pFrameInfo = pFrames->getFrameInformation(iFrame) ;
    if (pFrameInfo && pFrameInfo->getConcernsArray())
    {
      ArrayConcern* pConcerns = pFrameInfo->getConcernsArray() ;

      if (false == pConcerns->empty())
	      for (ArrayConcernIter i = pConcerns->begin(); i != pConcerns->end(); i++)
        {
          if ((*i)->getMaxSeverityIndex() >= iSeverityThreshold)
          {
  	        NSConcernView* pConcernView = new NSConcernView(pCtx, pWrk, *i, iFrame) ;
		        push_back(pConcernView) ;
          }
        }
    }
  }

	// Deuxième étape : leur attribuer une ligne
	//      Il est obligatoire d'attribuer les lignes après que l'on a créé
	//      les NSConcernView afin de pouvoir attribuer la même ligne aux
	//      préoccupations chaînées
	// Second step : find a line for each one
	//      We have to find the lines after we have created all NSConcernView
	//      in order to put linked health concerns on the same line
	for (ToonsIter toonIt = begin(); toonIt != end(); toonIt++)
	{
		NSConcernView* pConcernView = TYPESAFE_DOWNCAST(*toonIt, NSConcernView);
		if ((pConcernView) && (pConcernView->getIndex() == -1))
    {
      pConcernView->setIndex(pWrk->getLigneProb(pConcernView->getConcern(), iIndex)) ;
      if (pConcernView->getIndex() > pWrk->getMaxConcernIndex())
        pWrk->setMaxConcernIndex(pConcernView->getIndex()) ;
    }
	}

	// Troisième étape : créer leurs composants
	// Third step : create their components
	for (ToonsIter toonIt = begin(); toonIt != end(); toonIt++)
	{
		NSConcernView* pConcernView = TYPESAFE_DOWNCAST(*toonIt, NSConcernView) ;
		if (pConcernView)
		{
			pConcernView->boxFitting() ;
			pConcernView->createComponents(this) ;
		}
	}
}
catch (...)
{
	erreur("Exception ArrayOfToons::init(ArrayConcern).", standardError, 0) ;
  	return ;
}
}

void
ArrayOfToons::vider()
{
	if (empty())
		return ;

	for (ToonsIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

void
ArrayOfToons::viderType(NSLdvToon::TOONTYPE toonTypeToKill)
{
	if (empty())
		return ;

	for (ToonsIter i = begin(); end() != i ; )
	{
  	if ((*i)->_toonType == toonTypeToKill)
    {
			delete *i ;
			erase(i) ;
    }
    else
    	i++ ;
	}
}

ArrayOfToons::~ArrayOfToons()
{
	vider() ;
}


/*
ArrayOfToons&		ArrayOfToons::operator=(ArrayOfToons src)
{
	try
	{
    vider();

    if (!(src.empty()))
    	for (ToonsIter i = src.begin(); i != src.end(); i++)
      	push_back(new NSLdvToon(*(*i)));
    return *this;
	}
	catch (const exception& e)
	{
    string sExept = "Exception " + string(e.what());
    erreur(sExept.c_str(), standardError, 0) ;
    return *this;
	}
	catch (...)
	{
    erreur("Exception.", standardError, 0) ;
    return *this;
	}
}
*/

ldvBrush::ldvBrush(const NS_CLASSLIB::TColor& color)
{
  ShouldDelete = false;

#if 0
  if (color.IsSysColor())
    Handle = ::GetSysColorBrush(color.Index());
  else
#endif
  {
    COLORREF cr = color;
		/*
		if ((Handle = GetBrushCache().Lookup(cr)) != 0)
			return;
		*/
    Handle = ::CreateSolidBrush(cr);
  }
  WARNX(OwlGDI, !Handle, 0, "Cannot create solid TBrush " << hex << color) ;
  CheckValid() ;

  //GetBrushCache().Add(Handle, color);

  // TRACEX(OwlGDI, OWL_CDLEVEL, "TBrush constructed @" << (void*)this <<
  //                            " with color " << (COLORREF)color);
}

ldvBrush::ldvBrush(const NS_CLASSLIB::TColor& color, int iStyle)
{
  ShouldDelete = false;

#if 0
  if (color.IsSysColor())
    Handle = ::GetSysColorBrush(color.Index()) ;
  else
#endif
  {
    COLORREF cr = color ;
		/*
		if ((Handle = GetBrushCache().Lookup(cr)) != 0)
			return;
		*/
    Handle = ::CreateHatchBrush(iStyle, cr) ;
  }
  WARNX(OwlGDI, !Handle, 0, "Cannot create solid TBrush " << hex << color) ;
  CheckValid() ;

  //GetBrushCache().Add(Handle, color);

  // TRACEX(OwlGDI, OWL_CDLEVEL, "TBrush constructed @" << (void*)this <<
  //                            " with color " << (COLORREF)color);
}

