//----------------------------------------------------------------------------
//   Programme protégé par une licence Open Source GPL
//   NAUTILUS - www.nautilus-info.com
//----------------------------------------------------------------------------
#ifndef __NSLDVVUEBASE_H
# define __NSLDVVUEBASE_H

// # include <owl\owlpch.h>
# include <owl\dc.h>
# include <owl\inputdia.h>
# include <owl\chooseco.h>
# include <owl\gdiobjec.h>
# include <owl\docmanag.h>
# include <owl\filedoc.h>
# include <owl\listbox.h>
# include <owl\draglist.h>
# include <owl\imagelst.h>
# include <classlib\arrays.h>

# include "nsbb\nstip.h"
# include "nsbb\nslistwind.h"
# include "nautilus\nsldvdoc.h"
# include "nsldv\nsldvuti.h"
# include "nsldv\nsldvvar.h"
# include "nsldv\nsldvvuetoons.h"
# include "nsldv\nsldvgoal.h"
# include "partage\ns_timer.h"
# include "nsldv\nsldvtoons.h"

# include "nsldv/nsldvexport.h"

#define ICONWIDTH 16

class NSLdvWindowToon ;
class NSLdvChronoLine ;
class NSLdvCommandPannel ;

class _NSLDVCLASSE NSLdvViewBase : public TWindowView, public NSRoot
{
  public:

	  enum mouseMode { modeCursor = 0, modeZoom } ;

  protected:

	  NSLdvDocumentBase    *_pLdVDoc ;

    string               _sConcern ; // concern dedicated view

    // double            dUniteX, dUniteY;
	  pixUnit              iXunit ;
 	  int                  iZoomFactor ;

    NSTimeZoomLevelArray aZoomLevels ;
    NSTimeZoomLevel*     pCurrentTimeInfo ;

	  mouseMode           iMouseMode ;

	  int                 iLargeurChronoLine ;

	  string              skinName ;              // nom de la fenetre de skin

	  // Position du coin inférieur-droit par rapport au coin inf-dt global
	  NVLdVTemps          _tDateHeureInfDroit ;

	  bool                _bSystemColors ;
	  bool                _bDisplayDrugs, _bDisplayGoals ;
    int                 _iMinSeverityOfDisplayedConcerns ;

	  NS_CLASSLIB::TRect  rectDateTimeArea ;
	  NSLdvTankView       *pLdvTankView ;

	  OWL::TStatusBar*    pSB ;
    HACCEL              hAccelerator ;

	  OWL::TImageList     *ImgDragDrop ;

    int                 _iNbTimerTicks ;
    NS_CLASSLIB::TPoint _previousCursorLocation ;

    bool                _bPainted ;

    static long lObjectCount ;

  public:

    //
    // Les objets à afficher sont là
    // Here are the objects to draw
    //
	  ArrayOfToons    _aToons ;

  	bool            bSetupToolBar ;

	  ldvBrush*       pPinceauFond ;
	  ldvBrush*       pPinceauFondSys ;

	  ldvBrush*       pPinceauBlack ;
	  ldvBrush*       pPinceauRed ;
	  ldvBrush*       pPinceauBleu ;
	  ldvBrush*       pPinceauS1 ;
	  ldvBrush*       pPinceauS2 ;
	  ldvBrush*       pPinceauS3 ;
    ldvBrush*       pPinceauS34 ;
	  ldvBrush*       pPinceauS4 ;
    ldvBrush*       pPinceauSH1 ;
	  ldvBrush*       pPinceauSH2 ;
	  ldvBrush*       pPinceauSH3 ;
	  ldvBrush*       pPinceauSH4 ;

    OWL::TPen*      pPenEclaire ;
    OWL::TPen*      pPenEclaireSys ;
    OWL::TPen*      pPenOmbre ;
    OWL::TPen*      pPenOmbreSys ;
    OWL::TPen*      pPenTransition ;
    OWL::TPen*      pPenTransitionSys ;

    OWL::TPen*      pSelPenEclaire ;
    OWL::TPen*      pSelPenOmbre ;
    OWL::TPen*      pSelPenTransition ;

    OWL::TPen*      pPenS1 ;
    OWL::TPen*      pPenS2 ;
    OWL::TPen*      pPenS3 ;
    OWL::TPen*      pPenS4 ;
    OWL::TPen*      pPenS5 ;

    OWL::TIcon*     pLdvObject ;
    OWL::TIcon*     pLdvSsObject ;

    NSLdvViewBase(NSLdvDocument& doc, TWindow* parent = 0);
    NSLdvViewBase(const NSLdvViewBase& rv);
    ~NSLdvViewBase();

    void                reinit() ;
    void                disconnectCurves() ;
    void                reinitCurves() ;

    void                createWorkingArea(string sConcernRef) ;
    void                initWorkingArea(NSLdvViewArea* pWorkArea, bool bAtCreation = false) ;
    void                emptyWorkingAreas() ;
    void                reinitWorkingAreas() ;

    // Fonctions de réponse aux messages
    void                Paint(TDC&, bool, NS_CLASSLIB::TRect& RectangleAPeindre) ;
    void                EvSize(uint sizeType, ClassLib::TSize& size) ;
    void                EvSetFocus(THandle hWndLostFocus /* may be 0 */) ;
    void                EvTimer(uint timerId) ;
    void                EvHScroll(UINT scrollCode, UINT thumbPos, HWND hWndCtl) ;
    LRESULT             EvMouseWheel(WPARAM wparam, LPARAM lparam) ;
    void                EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void                EvSysKeyDown(uint key, uint repeatCount, uint flags) ;
    void                CmHelp() ;

    void                EvaluateWinToonsPos(TWindow* pWindow, ArrayOfToons* pToons) ;

    void                UpdatePixelPerUnitRate(uint) ;
    void                UpdateSeverityThreshold(uint) ;

    void                EvDragDrop() ; // temp -- pour l'instant inutilisé (méthode vide)

    void                EvHotKey(int idHotKey) ;
    HACCEL              getAcceleratorHandle() { return hAccelerator ; }

    void                CmLdvZoomIn() ;
    void                CmLdvZoomOut() ;
    void                change() ;
    void                timeBumper() ;

    void                CmLdvPixSecond() ;
    void                CmLdvPixMinute() ;
    void                CmLdvPixHour() ;
    void                CmLdvPixDay() ;
    void                CmLdvPixWeek() ;
    void                CmLdvPixMonth() ;

    void                handlePixelUnitChange() ;
    void                initZoomLevelInfo() ;
    void                setZoomLevelInfo() ;
    string              getLdvPixLexiqueCode() ;

    void                pixelUnitChanged() ;

    void                showMousePositionMessage() ;
    void                skinSwitchOn(string sSkinName);
    void                skinSwitchOff(string sSkinName);

    //
    // Concern related response functions
    //
    virtual void        CmNewPreoccup() = 0 ;
    virtual void        CmNewRisk() = 0 ;
    virtual void        CmNewSocialConcern() = 0 ;

    void                CmNewDrugsFromRefForConcern(NSConcern* pConcern) ;
    void                CmNewGoalsFromRefForConcern(NSConcern* pConcern) ;

    void                CmChgPreoccup() ;
    void                CmChgPreoType(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmChgPreoDate(NSConcern* pConcern, LDVFRAME iFrame) ;
    void                CmChgPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame) ;
    virtual void        CmAddObjectifs(NSConcern* pConcern, LDVFRAME iFrame) = 0 ;
    virtual void        CmProject(NSConcern* pConcern, LDVFRAME iFrame, string sKsName, string sKsArchetype) = 0 ;

    virtual void        CmEvolPreoccup(LDVFRAME iFrame) = 0 ;
    virtual void        CmEvolPreoType(NSConcern* pConcern, LDVFRAME iFrame) = 0 ;
    void                CmEvolPreoIndx(NSConcern* pConcern, NVLdVTemps tpsTarget, LDVFRAME iFrame) ;

    void                CmSuppressPreo(NSConcern* pConcern) ;

    void                CmManageRights(NSConcern* pConcern) ;
    void                CmManageRights(NSLdvDrug* pDrug) ;

    //
    // Drug related response functions
    //
    void                CmDrugNew() ;
    void                CmDrugNewForConcern(NSConcern* pRelatedConcern) ;
    void                CmDrugRenew(NSLdvDrug* pDrug) ;
    void                CmDrugModify(NSLdvDrug* pDrug) ;
    void                CmDrugChangePoso(NSLdvDrug* pDrug) ;
    void                CmDrugStop(NSLdvDrug* pDrug) ;
    void                CmDrugDelete(NSLdvDrug* pDrug) ;
    void                CmDrugPrescription() ;
    void                CmDrugRights() ;

    //
    // Miscellanous response functions
    //
    void                CmChange2Cursor() ;
    void                CmCursorEnable(TCommandEnabler &ce) ;
    void                CmChange2Zoom() ;
    void                CmZoomEnable(TCommandEnabler &ce) ;
    void                CmSwitchDrugs() ;
    void                CmSwitchGoals() ;

    void                CmShowGoalsForConcern() ;
    void                CmShowDrugsForConcern() ;
    void                CmShowFollowUpForConcern() ;
    void                CmShowCurvesForConcern() ;

    // Implémentation des méthodes virtuelles pures de TView
    static LPCSTR       StaticName()    { return "NSLdvViewBase" ; }
    const char*         GetViewName()   { return StaticName() ; }
    TWindow*            GetWindow()     { return (TWindow*) this ; }

    // Overrides TWindows' in order to process our accelerators table
    bool PreProcessMsg(MSG &msg) ;

    // Fonction d'initialisation
    void                SetupWindow() ;
    void                SetupToolBar() ;
    void                SetWindowPosLdv() ;
    void                EnregistrePosLdv() ;
    void                SetWindowPosit() ;     // deprecated
    void                RecordWindowPosit() ;
    void                reloadView(string sReason, string sReference = string("")) ;

    bool                CanClose() ;
    void                EvClose() ;
    void                EvDestroy() ;
    bool                EvQueryEndSession() ;

    string              getConcern()            { return _sConcern ; }
    pixUnit             getXunit()              { return iXunit ; }
    bool                isZoomMax()             { return (pixSeconde == iXunit) ; }
    bool                isZoomMin()             { return (pixMois == iXunit) ; }
    mouseMode           getMouseMode()          { return iMouseMode ; }
    int                 getSeverityThreshold()  { return _iMinSeverityOfDisplayedConcerns ; }
    int                 getZoomFactor()         { return iZoomFactor ; }
    int                 getPpuRate()            { return pCurrentTimeInfo->getUppRate() ; }
    int                 getUpperPpuLevel()      { return pCurrentTimeInfo->getUpperLimit() ; }
    int                 getLowerPpuLevel()      { return pCurrentTimeInfo->getLowerLimit() ; }
    int                 getLargeurChronoLine() ;

    NVLdVTemps          getDateHeureInfDroit()  { return _tDateHeureInfDroit ; }
    void                setRightTimeFromLeftTime(const NVLdVTemps& tLeftTime) ;

    bool                useSystemColors()       { return _bSystemColors ; }
    OWL::TImageList*    getImgDragDrop()        { return ImgDragDrop ; }
    bool                mustDisplayDrugs()      { return _bDisplayDrugs ; }
    bool                mustDisplayGoals()      { return _bDisplayGoals ; }

    void                setXunit(pixUnit iU)         { iXunit = iU ; }
    void                refreshChronoLine() ;
    void                setSeverityThreshold(int iT) { _iMinSeverityOfDisplayedConcerns = iT ; }
    void                setZoomFactor(int iZ)        { iZoomFactor = iZ ; }

    int                 getPhysicalWidthFromTimeUnit(int iTimeUnitCount) { return (pCurrentTimeInfo->getUppRate() == 0) ? iTimeUnitCount : iTimeUnitCount * (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) / pCurrentTimeInfo->getUpperLimit() ; }
    int                 getTimeUnitFromPhysicalWidth(int iPhysicalWidth) { return (pCurrentTimeInfo->getUppRate() == 0) ? iPhysicalWidth : iPhysicalWidth * pCurrentTimeInfo->getUpperLimit() / (pCurrentTimeInfo->getUpperLimit() - pCurrentTimeInfo->getUppRate()) ; }

    NSTimeZoomLevel*    getTimeZoomInfo(pixUnit iU) ;
    NSTimeZoomLevel*    getCurrentZoomInfo()         { return pCurrentTimeInfo ; }

    int                 getNbTimerTicks()            { return _iNbTimerTicks ; }
    void                resetNbTimerTicks()          { _iNbTimerTicks = 0 ; }

    NSLdvDocumentBase*  getDoc()                     { return _pLdVDoc ; }

    NS_CLASSLIB::TRect  getDateTimeArea() { return rectDateTimeArea ; }
    void                setDateTimeArea(NS_CLASSLIB::TRect rectArea) { rectDateTimeArea = rectArea ; }
    void                setDateTimeArea(int left, int top, int right, int bottom) { setDateTimeArea(NS_CLASSLIB::TRect(left, top, right, bottom)) ; }

    NS_CLASSLIB::TRect  getLdvTankView() { return pLdvTankView->coords() ; }

    HBRUSH              getBlackBrushHandle() { return pPinceauBlack->GetHandle() ; }
    HBRUSH              getRedBrushHandle()   { return pPinceauRed->GetHandle() ; }

    // ajouter des méthodes qui permettent de récupérer des sous-objets
    // objets problèmes préoccupations sans avoir à tout réinitialiser
    // leur liste respectives
    // juste en leur passant un pointeur sur cet "objet" en question
    bool                addSsObj(NSLdvSousObjet *pSsObj) ;
    bool                addObj(NSLdvObjet *pObj) ;
    void                removeObj(NSLdvObjet *pObj) ;
    bool                addQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    void                removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts) ;
    bool                addQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    void                removeQualityOfLifeChart(NSQualityOfLifeChart *pCharts, NSConcern *pPb) ;
    bool                addProb(NSConcern *pPb) ;

    bool                addObjOnConcern(NSConcern *pPb, NSLdvObjet *pObj) ;

    void		            addDrugs() ;
    void		            removeDrugs() ;
    void		            reinitDrugs() ;
    void		            addGoals() ;
    void                removeGoals() ;

    void                reorganizeLines() ;

    void                displayCurvesForBiometricalGoals(string sReference) ;

    void                setAppointment() ;

    //                  ------------- toons management -----------------

    void                addToToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon) ;
    bool                removeFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon* pToon) ;
    void                deleteAllFromToonsArray(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;

    NSLdvTimeToon*      getLdVTimeToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvWindowToon*    getLdVWinToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvToon*          getLdVToon(ArrayOfToons* pToonsArray, NSLdvToon::TOONTYPE tType) ;
    NSLdvChronoLine*    getLdVChronoLine() ;
    NSLdvCommandPannel* getLdVCommandPannel() ;
    NSLdvViewArea*      getActiveWorkingArea() ;

    NVLdVTemps          getDateNaissance() ;

    void                afficheStatusMessage(string& sMessage, bool bScroll) ;

    void                setTipsTimer()  { SetTimer(ID_LDVTIMER, TIMER_VALUE) ; }
    void                killTipsTimer() { KillTimer(ID_LDVTIMER) ; }

    NS_CLASSLIB::TPoint getGlobalPhysicalPoint(NVLdVPoint ldvPoint) ;
    NS_CLASSLIB::TRect  getGlobalPhysicalRect(NVLdVRect ldvRect) ;
    NS_CLASSLIB::TRect  getGlobalPhysicalRect(NS_CLASSLIB::TRect ldvWinRect) ;

    bool                convertUnit(double* pdValue, string sResultUnit, string sInitialUnit, string sRelatedTo = "", bool bVerbose = false) ;

    bool                getRelativeValue(int* piNewValue, TWindow* pRefWindow, NSLdvWindowToon* pToon, string sReference, string sDirection) ;
    void                getReferenceAndDirection(string sReferenceString, string sNeededDirection, string* psReference, string* psDirection) ;

    NSLdvViewBase&          operator=(const NSLdvViewBase& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

 DECLARE_RESPONSE_TABLE(NSLdvViewBase) ;
};

#endif // __NSLDVVUEBASE_H

