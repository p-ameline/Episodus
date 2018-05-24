// -----------------------------------------------------------------------------
// nsldvvar.h
// -----------------------------------------------------------------------------
// $Revision: 1.11 $
// $Author: pameline $
// $Date: 2015/02/07 14:27:33 $
// -----------------------------------------------------------------------------
// Ligne de vie - Arrays d'objets de type Vue (au sens Document/Vue)
// Ligne de vie - Arrays of View objects (for Document/View model)
// -----------------------------------------------------------------------------
// FLP - aout 2003
// PA  - juillet 2003
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Programme protégé par une licence Open Source GPL
// NAUTILUS - www.nautilus-info.com
// -----------------------------------------------------------------------------

#ifndef __NSLDVVAR_H
#define __NSLDVVAR_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\slider.h>
#include <owl\gauge.h>
#include <owl\listwind.h>

#include "nsbb\nsednum.h"
#include "nautilus\nsldvdoc.h"
#include "partage\NTArray.h"
#include "nsbb\nsmediccontrols.h"
#include "nsbb\nsmedicphase.h"
#include "nsoutil\nsBdmDlg.h"

//#include "nautilus\nsldvvue.h"

class   NSPbModifView ;
class   NSConcernView ;
class   NSPreoccupView ;
class   NSEventView ;
class   NSSsObjView ;
class   NSLdvObjetView ;
class   NSLdvGoal ;
class   NSLdvCurvePoint ;
class   NSLdvCurve ;
class   NewConcernInfo ;

typedef vector<NSPbModifView*> ArrayModifierView ;
typedef ArrayModifierView::iterator       ArrayModifViewIter ;
typedef ArrayModifierView::const_iterator ArrayModifViewConstIter ;
typedef NTArray<NSPbModifView> ArrayModifView ;

typedef vector<NSConcernView*> ArrayPbView ;
typedef ArrayPbView::iterator       ArrayConcernViewIter ;
typedef ArrayPbView::const_iterator ArrayConcernViewConstIter ;
typedef NTArray<NSConcernView> ArrayConcernView ;

typedef vector<NSPreoccupView*> ArrayPreocView ;
typedef ArrayPreocView::iterator       ArrayPreoccupViewIter ;
typedef ArrayPreocView::const_iterator ArrayPreoccupViewConstIter ;

enum hitToonStatus { statusNone = 0, statusName, statusPause, statusPutFront, statusOpening, statusOpened } ;

inline string getHitToonStatusLabel(hitToonStatus iTS)
{
  switch(iTS)
  {
    case statusNone     : return string("statusNone") ;
    case statusName     : return string("statusName") ;
    case statusPause    : return string("statusPause") ;
    case statusPutFront : return string("statusPutFront") ;
    case statusOpening  : return string("statusOpening") ;
    case statusOpened   : return string("statusOpened") ;
  }
  return string("not properly defined") ;
}

class ArrayPreoccupView : public ArrayPreocView
{
  public:

    // Constructeurs - Destructeur
    ArrayPreoccupView() : ArrayPreocView() {}
    ArrayPreoccupView(const ArrayPreoccupView& rv) ;
    ~ArrayPreoccupView() ;

    void vider() ;
    void init(ArrayConcern* pConcerns, NSContexte* pCtx, NSLdvView* pView, int iIdx) ;

    ArrayPreoccupView& operator=(const ArrayPreoccupView& src) ;
};

typedef vector<NSEventView*> ArrayEvtView ;
typedef ArrayEvtView::iterator       ArrayEventViewIter ;
typedef ArrayEvtView::const_iterator ArrayEventViewConstIter ;

class ArrayEventView : public ArrayEvtView
{
  public:

    // Constructeurs - Destructeur
    ArrayEventView() : ArrayEvtView() {}
    ArrayEventView(const ArrayEventView& rv) ;
    ~ArrayEventView() ;

    void    vider() ;
    void    init(NSContexte* pCtx, NSLdvView* pView) ;

    ArrayEventView& operator=(const ArrayEventView& src) ;
};

typedef vector<NSSsObjView*> ArraySsObjView ;
typedef ArraySsObjView::iterator       ArraySsObjViewIter ;
typedef ArraySsObjView::const_iterator ArraySsObjViewConstIter ;
typedef NTArray<NSSsObjView>    ArraySsObjetView ;

typedef vector<NSLdvCurvePoint*> ArrayCurvePts ;
typedef ArrayCurvePts::iterator        ArrayCurvePtsIter ;
typedef ArrayCurvePts::const_iterator  ArrayCurvePtsConstIter ;
typedef NTArray<NSLdvCurvePoint> ArrayCurvePoints ;

typedef vector<NSLdvObjetView*> ArrayObjView ;
typedef ArrayObjView::iterator       ArrayObjViewIter ;
typedef ArrayObjView::const_iterator ArrayObjViewConstIter ;

class ArrayObjetsView : public ArrayObjView
{
  public:

    // Constructeurs - Destructeur
    ArrayObjetsView() : ArrayObjView() {}
    ArrayObjetsView(const ArrayObjetsView& rv) ;
    ~ArrayObjetsView() ;

    void    vider() ;
    void    init(ArrayObjets* pObjets, NSContexte* pCtx, NSLdvView* pView) ;

    ArrayObjetsView& operator=(const ArrayObjetsView& src) ;
};

//
// New health concern creation dialog
// Création d'un nouveau problème de santé
//
class NSNewConcernDlg : public NSUtilDialog
{
  public:

    NSLdvView*          _pLdvView ;
    NSLdvDocument*      _pLdvDoc ;

    NSUtilLexique       *_pType ;
    OWL::TRadioButton   *_pRc ;
    OWL::TStatic        *_pDateDebText ;
    NSUtilEditDateHeure *_pDateDeb ;
    OWL::TGroupBox      *_pGroupFin ;
    NSUtilEditDateHeure *_pDateFin ;
    NSEditNum           *_pNbJours ;
    OWL::TRadioButton   *_pRChronique ;
    OWL::TRadioButton   *_pRDans ;
    OWL::TRadioButton   *_pRDuree ;
    OWL::TStatic        *_pJoursText ;
    OWL::TRadioButton   *_pRLe ;
    OWL::TStatic        *_pSeverityText ;
    OWL::THSlider       *_pGravite ;
    OWL::THSlider       *_pRisque ;
    OWL::TGauge         *_pGravGauge ;
    // OWL::TGauge      *pRiskGauge ;

    OWL::TButton        *_pDusoiButton ;
    bool                _bDusoiWasCalculated ;
    int                 _iDusoiValue ;
    int                 _iSymptomValue ;
    int                 _iComplicationValue ;
    int                 _iPrognosisValue ;
    int                 _iTreatabilityValue ;

    OWL::TButton        *_pCocButton ;
    string              _sClassif ;
    string              _sPostCase ;

    OWL::TButton        *_pCimButton ;

    // string              *psNewNode ;

    // NSNewConcernDlg(NSLdvView* pView, string* psNode, NSContexte* pCtx, NSPatPathoArray* pDetails, string *pCocCode, string *sLabel = new string(""), string *pLibelle = 0, int iSever = 10) ;
    // NSNewConcernDlg(TWindow* pPere, string* psNode, NSLdvDocument* pDoc, NSContexte* pCtx, NSPatPathoArray* pDetails, string *pCocCode, string *pCode = 0, string *psDate = 0) ;
    NSNewConcernDlg(NSLdvView* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    NSNewConcernDlg(TWindow* pPere, NSLdvDocument* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    ~NSNewConcernDlg() ;

    void CreateControls() ;
    void InitFromInfo() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void UserName(WPARAM wParam) ;
    void ResultCons() ;
    void UpdateSeverity(uint) ;
    void EvaluateDusoi() ;

    void SetCocCodeVerbose() ;
    void SetCocCodeNoVerbose() ;
    void DisplayCocCode() ;
    void FindCocCode(bool bVerbose) ;

    void SetCimCodeVerbose() ;
    void SetCimCodeNoVerbose() ;
    void DisplayCimCode() ;
    void FindCimCode(bool bVerbose) ;

    void FindDuration() ;

    // void    UpdateRisk(uint) ;

    void SetDetails() ;

  protected:

    bool            _bDontHandleRadio ;

    NewConcernInfo *_pNewConcernInfo ;
    string          _sCocCode ;
    string          _sCimCode ;

    NSPatPathoArray _PptDetails ;

  DECLARE_RESPONSE_TABLE(NSNewConcernDlg) ;
};

//
// New health risk creation dialog
// Création d'un nouveau risque
//
class NSNewRiskDlg : public NSUtilDialog
{
  public:

    NSLdvView*          pLdvView ;
    NSLdvDocument*      pLdvDoc ;

    NSUtilLexique       *pType ;
    OWL::TRadioButton   *pRc ;
    OWL::TStatic        *pDateDebText ;
    NSUtilEditDateHeure *pDateDeb ;
    OWL::TGroupBox      *pGroupFin ;
    NSUtilEditDateHeure *pDateFin ;
    NSEditNum           *pNbJours ;
    OWL::TRadioButton   *pRChronique ;
    OWL::TRadioButton   *pRDans ;
    OWL::TRadioButton   *pRDuree ;
    OWL::TStatic        *pJoursText ;
    OWL::TRadioButton   *pRLe ;

    OWL::TButton        *pCocButton ;
    string              _sClassif ;
    string              _sPostCase ;

    NSNewRiskDlg(NSLdvView* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    NSNewRiskDlg(TWindow* pPere, NSLdvDocument* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    ~NSNewRiskDlg() ;

    void CreateControls() ;
    void InitFromInfo() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void UserName(WPARAM wParam) ;
    void ResultCons() ;
    void EvaluateDusoi() ;

    void SetCocCodeVerbose() ;
    void SetCocCodeNoVerbose() ;
    void DisplayCocCode() ;
    void FindCocCode(bool bVerbose) ;

    // void    UpdateRisk(uint) ;

    void SetDetails() ;

  protected:

    NewConcernInfo *_pNewConcernInfo ;
    string          _sCocCode ;

    NSPatPathoArray _PptDetails ;

  DECLARE_RESPONSE_TABLE(NSNewRiskDlg) ;
};

//
// New social concern creation dialog
// Création d'une nouvelle préoccupation sociale
//
class NSNewSocialDlg : public NSUtilDialog
{
  public:

    NSLdvView*          pLdvView ;
    NSLdvDocument*      pLdvDoc ;

    NSUtilLexique       *pType ;
    OWL::TRadioButton   *pRc ;
    OWL::TStatic        *pDateDebText ;
    NSUtilEditDateHeure *pDateDeb ;
    OWL::TGroupBox      *pGroupFin ;
    NSUtilEditDateHeure *pDateFin ;
    NSEditNum           *pNbJours ;
    OWL::TRadioButton   *pRChronique ;
    OWL::TRadioButton   *pRDans ;
    OWL::TRadioButton   *pRDuree ;
    OWL::TStatic        *pJoursText ;
    OWL::TRadioButton   *pRLe ;
    OWL::TStatic        *pSeverityText ;
    OWL::THSlider       *pGravite ;
    OWL::THSlider       *pRisque ;
    OWL::TGauge         *pGravGauge ;

    OWL::TButton        *pCocButton ;
    string              _sClassif ;
    string              _sPostCase ;

    NSNewSocialDlg(NSLdvView* pView, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    NSNewSocialDlg(TWindow* pPere, NSLdvDocument* pDoc, NSContexte* pCtx, NewConcernInfo *pNewConcernInfo) ;
    ~NSNewSocialDlg() ;

    void CreateControls() ;
    void InitFromInfo() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void UserName(WPARAM wParam) ;
    void ResultCons() ;
    void UpdateSeverity(uint) ;

    void SetCocCodeVerbose() ;
    void SetCocCodeNoVerbose() ;
    void DisplayCocCode() ;
    void FindCocCode(bool bVerbose) ;

    // void    UpdateRisk(uint) ;

    void SetDetails() ;

  protected:

    NewConcernInfo *_pNewConcernInfo ;
    string          _sCocCode ;

    NSPatPathoArray _PptDetails ;

  DECLARE_RESPONSE_TABLE(NSNewSocialDlg) ;
};

//
// Création d'un nouveau problème de santé
//
class NSSimpleNewDrugDlg : public NSUtilDialog
{
  protected:

    std::string         _sLexiqCode ;     // code lexique du médicament
    std::string         _sPriseUnit ;
    std::string         _sATCCode ;       // Code ATC
    std::string         _sCICode ;        // Code CIS ou CIP
    std::string         _sDateOuverture ; // date d'ouverture
    std::string         _sDateFermeture ; // date de fermeture
    std::string         _sFreeText ;      // texte libre
    std::string         _sALD ;           // ALD ou non
    std::string         _sPositionRepas ; // Position par rapport aux repas

    std::string         _sLexiqEvent ;    // code lexique "en cas de"
    std::string         _sFreeTextEvent ; // texte libre "en cas de"

    VecteurString       _aLinkedIssues ;
    VecteurString       _aUpdatedLinkedIssues ;

    NSphaseMedic*       _pPhase ;

    bool                _bNonSubstituable ;

    std::string         _sReferential ;

    OWL::TGroupBox      *_pTrtGroup ;

    NSUtilLexique       *_pType ;
    OWL::TStatic        *_pUnitePriseTxt ;
    NSUtilLexique       *_pUnitePrise ;
    OWL::TStatic        *_pEventTxt ;
    NSUtilLexique       *_pEvent ;

    OWL::TStatic        *_pDateDebTxt ;
    NSUtilEditDateHeure *_pDateDeb ;
    OWL::TGroupBox      *_pDateFinGroup ;
    OWL::TRadioButton   *_pRChronique ;
    OWL::TRadioButton   *_pRDans ;
    OWL::TRadioButton   *_pRDuree ;
    OWL::TRadioButton   *_pRLe ;
    NSEditNum           *_pNbJours ;
    NSComboBox          *_pCBDureeTtt ;
    NSUtilEditDateHeure *_pDateFin ;

    OWL::TGroupBox      *_pPrescriptionGroup ;

    OWL::TStatic        *_pDureePhaseTxt ;
    NSUpDownEdit        *_pDureePhase ;
    NSComboBox          *_pCBDureePhase ;
    OWL::TStatic        *_pRenouvellementTxt ;
    NSUpDownEdit        *_pRenouvellement ;
    OWL::TStatic        *_pRenouvellementTimeTxt ;
    OWL::TCheckBox      *_pNonRenouvelable ;

    OWL::TStatic        *_pDateDebPrescrTxt ;
    NSUtilEditDateHeure *_pDateDebPrescr ;
    OWL::TStatic        *_pDateFinPrescrTxt ;
    NSUtilEditDateHeure *_pDateFinPrescr ;

    OWL::TGroupBox      *_pPosologieGroup ;

    NSUpDownEdit        *_pPriseMatin, *_pPriseMidi, *_pPriseSoir, *_pPriseCoucher ;
    OWL::TStatic        *_pPriseMatinTxt, *_pPriseMidiTxt, *_pPriseSoirTxt, *_pPriseCoucherTxt ;

    OWL::TCheckBox      *_pALD ;
    OWL::TCheckBox      *_pIssues ;
    OWL::TButton        *_pComplexModeButton ;
    OWL::TCheckBox      *_pNonSubstituable ;
    OWL::TButton        *_pFreeTextButton ;

    NSPatPathoArray     *_pPPT ;

    bool                _bMustSwitchToComplexMode ;

    // BDM related information
    //
    OWL::TButton        *_pDrugInfoButton ;
    OWL::TButton        *_pBdmSearchIndicationButton ;
    OWL::TButton        *_pBdmSearchSubstanceButton ;
    OWL::TButton        *_pBdmSearchAtcButton ;

    NsSelectableDrug    *_pBdmDrugInformation ;

    void getDrugInformation() ;
    void checkSecurityCheckInformation() ;

  public:

    NSSimpleNewDrugDlg(TWindow* pView, NSContexte *pCtx, NSPatPathoArray *pPPTinit) ;
    ~NSSimpleNewDrugDlg() ;

    bool mustSwitchToComplexMode() { return _bMustSwitchToComplexMode ; }
    bool ParseMedicament() ;    // Function qui charge un medicament
    bool ParsePhase(PatPathoIter &pptIter) ;

    void initializeConnectedIssues(string sDrugNode) ;
    void initConnectedIssues(VecteurString* pLinkArray) ;

    VecteurString* getUpdatedLinkedConcern() { return &_aUpdatedLinkedIssues ; }

  protected:

    void createInterfaceElements() ;
    void initInterfaceElements() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void UserName(WPARAM wParam) ;
    void BuildPatpatho() ;
    void BuildPhaseTree(NSPatPathoArray *pPptPhase) ;
    void BuildCycleTree(NSPatPathoArray *pPptCycle) ;

    void ExecutedAfterDrugSelection() ;
    void ExecutedAfterTrtBeginDate() ;
    void ExecutedAfterTrtEndDate() ;
    void ExecutedAfterEventSelection() ;
    void ActualisePhase() ;
    void ActualiseEndOfPrescription() ;

    void switchToComplexMode() ;
    void editFreeText() ;
    void nonRenouvelable() ;
    void drugInformation() ;
    void selectIndication() ;

    void searchInBdmByIndication() ;
    void searchInBdmBySubstance() ;
    void searchInBdmByATC() ;
    void searchInBdm(NSBdmDriver::BAMTABLETYPE iSearchBy) ;

    void getCurrentConcerns(ArrayConcern *pCurrentConcerns) ;

    bool isAConnectedIssue(string sConcernNode) ;
    bool wasAConnectedIssue(string sConcernNode) ;
    bool isAConnected(string sConcernNode, VecteurString* pLinkArray) ;
    void setSelectedIssuesBtnState() ;

    void addConnectedIssue(string sConcernNode) ;

    void updateConnectedIssues(string sDrugNode) ;
    void UnconnectIssue(string sDrugNode, string sConcernNode) ;
    void ConnectIssue(string sDrugNode, string sConcernNode) ;

  DECLARE_RESPONSE_TABLE(NSSimpleNewDrugDlg) ;
} ;

class NSModifConcernTypeDlg : public NSUtilDialog
{
  public:

    NSUtilLexique*  pType ;
    NSPatPathoInfo* pNoeud ;

    NSModifConcernTypeDlg(LDVFRAME iFrame, TWindow* pView, NSPatPathoInfo* pNode, NSContexte* pCtx) ;
    ~NSModifConcernTypeDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSModifConcernTypeDlg) ;
};

class NSModifConcernDateDlg : public NSUtilDialog
{
  public:

    OWL::TStatic        *pDateDebText ;
    NSUtilEditDateHeure *pDateDeb ;
    OWL::TGroupBox      *pGroupFin ;
    NSUtilEditDateHeure *pDateFin ;
    NSEditNum           *pNbJours ;
    OWL::TRadioButton   *pRChronique ;
    OWL::TStatic        *pJoursText ;
    OWL::TRadioButton   *pRDans ;
    OWL::TRadioButton   *pRDuree ;
    OWL::TRadioButton   *pRLe ;

    NVLdVTemps*         ptpsDeb ;
    NVLdVTemps*         ptpsFin ;

    NSModifConcernDateDlg(LDVFRAME iFrame, TWindow* pView, NVLdVTemps* pDeb, NVLdVTemps* pFin, NSContexte* pCtx) ;
    ~NSModifConcernDateDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void adminDates(WPARAM wParam) ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSModifConcernDateDlg) ;
};

class NSModifConcernIndexDlg : public NSUtilDialog
{
  public:

    OWL::TStatic  *pSeverityText ;
    OWL::THSlider *pGravite ;
    OWL::TGauge   *pGravGauge ;

    int*          piSeverite ;

    // NSModifConcernIndexDlg(int* pSev, int* pRisk, NSContexte* pCtx) ;
    NSModifConcernIndexDlg(LDVFRAME iFrame, TWindow* pView, int* pSev, NSContexte* pCtx) ;
    ~NSModifConcernIndexDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    void UpdateSeverity(uint) ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSModifConcernIndexDlg) ;
};

class NSEvolConcernIndexDlg : public NSUtilDialog
{
  public:

    OWL::TStatic        *pDateDebText ;
    NSUtilEditDateHeure	*pDate ;
    OWL::TStatic        *pSeverityText ;
    OWL::THSlider       *pGravite ;
    OWL::TGauge         *pGravGauge ;

    int*            piSeverite ;
    NVLdVTemps*     ptpsDeb ;

    NSEvolConcernIndexDlg(LDVFRAME iFrame, TWindow* pView, NVLdVTemps* pDeb, int* pSev, NSContexte* pCtx) ;
    ~NSEvolConcernIndexDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    void UpdateSeverity(uint) ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSEvolConcernIndexDlg) ;
};

class NSModifConcernDlg : public NSUtilDialog
{
  public:

    NSLdvView*          pLdvView ;

    NSUtilLexique       *pType ;
    OWL::TStatic        *pDateDebTxt ;
    NSUtilEditDateHeure	*pDateDeb ;
    NSUtilEditDateHeure	*pDateFin ;
    OWL::TStatic        *pJoursText ;
    NSEditNum           *pNbJours ;
    OWL::TRadioButton   *pRChronique ;
    OWL::TRadioButton   *pRDans ;
    OWL::TRadioButton   *pRDuree ;
    OWL::TRadioButton   *pRLe ;

    NSConcern*          pConcern ;

    string              sLexique ;

    /*
        string              *sType ;
        string              *sDateDeb ;
        string              *sDateFin ;
        */

    NSModifConcernDlg(LDVFRAME iFrame, NSLdvView* pView, NSContexte* pCtx, NSConcern* pPb) ;
    ~NSModifConcernDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void UserName(WPARAM wParam) ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSModifConcernDlg) ;
};

class NSCurveSampleGroupBox ;

//
// Asking for a new curve
//
class NSNewCurveDlg : public NSUtilDialog
{
  public:

    bool                bModifyMode ;

    NSLdvCurve          *pCurve ;

    NSUtilLexique       *pType ;
    NSUtilLexique       *pUnit ;

    NSUpDownEdit        *pRedValue ;
    NSUpDownEdit        *pGreenValue ;
    NSUpDownEdit        *pBlueValue ;
    NSUpDownEdit        *pWidthValue ;
    NSUpDownEdit        *pMaxAxisValue ;
    NSUpDownEdit        *pMinAxisValue ;
    NSUpDownEdit        *pPointRadiusValue ;

    string              *psCode ;
    string              *psLibelle ;

    OWL::TStatic        *pUnitText ;
    OWL::TStatic        *pRedText ;
    OWL::TStatic        *pGreenText ;
    OWL::TStatic        *pBlueText ;
    OWL::TStatic        *pWidthText ;
    OWL::TStatic        *pMaxAxisText ;
    OWL::TStatic        *pMinAxisText ;
    OWL::TStatic        *pPointRadiusText ;

    OWL::TRadioButton   *pTSolid ;
    OWL::TRadioButton   *pTDash ;
    OWL::TRadioButton   *pTDot ;
    OWL::TRadioButton   *pTDashDot ;
    OWL::TRadioButton   *pTDashDotDot ;

    OWL::TRadioButton   *pRedSampleColorBtn ;
    OWL::TRadioButton   *pGreenSampleColorBtn ;
    OWL::TRadioButton   *pYellowSampleColorBtn ;
    OWL::TRadioButton   *pBlueSampleColorBtn ;
    OWL::TRadioButton   *pMagentaSampleColorBtn ;
    OWL::TRadioButton   *pCyanSampleColorBtn ;

    OWL::TRadioButton   *pIncludeZeroBtn ;
    OWL::TRadioButton   *pDynamicColouringBtn ;

    OWL::TRadioButton   *pPointAspectCircleBtn ;
    OWL::TRadioButton   *pPointAspectSquareBtn ;
    OWL::TRadioButton   *pPointAspectTriangleBtn ;
    OWL::TRadioButton   *pPointAspectStarBtn ;

    OWL::TGroupBox        *pConceptGroup ;
    OWL::TGroupBox        *pLineColourGroup ;
    OWL::TGroupBox        *pLineTypeGroup ;
    OWL::TGroupBox        *pPointTypeGroup ;
    NSCurveSampleGroupBox *pSampleGroup ;
    OWL::TGroupBox        *pAxisPropertiesGroup ;

    NSNewCurveDlg(TWindow* pPere, NSContexte* pCtx, NSLdvCurve* pCurveToEdit, bool bModif = false) ;
    ~NSNewCurveDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void ResultCons() ;

    int  getDotStyle() ;
    int  getPointStyle() ;

    void ColorSample(WPARAM wParam) ;
    void DotStyle(WPARAM wParam) ;
    void PointAspect(WPARAM wParam) ;
    void ZeroIncluded() ;

    void drawExampleLine() ;
    void manualColorChange() ;
    void resetSampleColor() ;

    void Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre) ;

  DECLARE_RESPONSE_TABLE(NSNewCurveDlg) ;
};

class NSCurveSampleGroupBox : public OWL::TGroupBox
{
  public:

    NSCurveSampleGroupBox(NSNewCurveDlg* parent, int resourceId, TModule* module = 0) ;

    void EvPaint() ;

  protected :

    NSNewCurveDlg* _pNewCurveDlg ;

  DECLARE_RESPONSE_TABLE(NSCurveSampleGroupBox) ;
};

//
// Création d'un nouveau problème de santé
//
class NSDrugHistoryDlg : public NSUtilDialog
{
  public:

    NSDrugHistoryDlg(TWindow* pView, NSContexte *pCtx) ;
    ~NSDrugHistoryDlg() ;

    string getDrugCode() ;
    string getDrugLabel() ;
    string getAtcCode() ;
    string getAtcLabel() ;

  protected:

    std::string    _sLexiqCode ;  // code lexique du médicament
    std::string    _sLexiqLabel ;
    std::string    _sAtcCode ;    // code ATC
    std::string    _sAtcLabel ;

    OWL::TStatic*  _pDrugText ;
    NSUtilLexique* _pDrugEdit ;
    OWL::TStatic*  _pAtcText ;
    NSEditBdm*     _pAtcCode ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSDrugHistoryDlg) ;
} ;

class NSMedic
{
  public:

    string  name;
    string  node;
    string  code;
    string  datedeb;
    string  datefin;

    NSMedic();
    NSMedic(string sName, string sNode);
    ~NSMedic();
    NSMedic(const NSMedic& rv);
    NSMedic& operator=(const NSMedic& src);
};

// array de liaison
typedef vector<NSMedic*> NSMedicVector ;
typedef NSMedicVector::iterator       NSMedicIter ;
typedef NSMedicVector::const_iterator NSMedicConstIter ;

class _CLASSELEXI NSMedicArray : public NSMedicVector
{	public :
		// Constructeurs		NSMedicArray() : NSMedicVector() {}		NSMedicArray(const NSMedicArray& rv) ;    NSMedicArray& operator=(const NSMedicArray& src) ;
		// Destructeur		virtual ~NSMedicArray() ;    void vider() ;};

class NSObjectif
{
  public:

    string     _sName ;
    string     _sNode ;
    string     _sCode ;
    int        _iRythme ;
    string     _sDateAuto ;
    string     _sDateCons ;
    string     _sDateIdeal ;
    string     _sDateIdealMax ;
    string     _sDateConsMax ;
    string     _sDateCrit ;
    string     _sDatePrec ;
    NSLdvGoal	*_pCorrespGoal ;

    string     _sValue ;
    int        _iLevel ;

    NSObjectif() ;
    NSObjectif(string sName, string sNode) ;
    NSObjectif(const NSObjectif& rv) ;
    ~NSObjectif() ;

    NSObjectif& operator=(const NSObjectif& src) ;

    string getName()   { return _sName ; }
    string getNode()   { return _sNode ; }
    string getValue()  { return _sValue ; }
    int    getLevel()  { return _iLevel ; }
    int    getRythme() { return _iRythme ; }

    string getDateAuto()     { return _sDateAuto ; }
    string getDateCons()     { return _sDateCons ; }
    string getDateIdeal()    { return _sDateIdeal ; }
    string getDateIdealMax() { return _sDateIdealMax ; }
    string getDateConsMax()  { return _sDateConsMax ; }
    string getDateCrit()     { return _sDateCrit ; }
    string getDatePrec()     { return _sDatePrec ; }
} ;

// array de liaison
typedef vector<NSObjectif*> NSObjectifVector ;
typedef NSObjectifVector::iterator       NSObjectifIter ;
typedef NSObjectifVector::const_iterator NSObjectifConstIter ;
typedef NTArray<NSObjectif> _CLASSELEXI NSObjectifArray ;

class NSConcernPropertyWindow ;

class NSConcernPropertyDlg : public NSUtilDialog
{
  public:

    NSLdvView*          pLdvView;
    NSConcern*          pConcern;
    NSConcern*          pOldConcern;
    NSMedicArray*       pMedicActuel;
    int                 nbMedicActuel;
    NSMedicArray*       pMedicAncien;
    int                 nbMedicAncien;
    NSObjectifArray*    pExams;
    int                 nbExams;
    bool                bGlobal;

    NSConcernPropertyWindow*    pListeActuel;
    NSConcernPropertyWindow*    pListeAncien;
    NSConcernPropertyWindow*    pListeExams;

    OWL::TGroupBox*     pListGroup;
    OWL::TRadioButton*  pGlobal;
    OWL::TRadioButton*  pLocal;
    OWL::TButton*       pNouveau;
    OWL::TButton*       pRenouveler;
    OWL::TButton*       pModifier;
    OWL::TButton*       pArreter;
    OWL::TButton*       pOrdonnance;

    NSConcernPropertyDlg(NSLdvView* pView, NSContexte* pCtx, NSConcern* pPb);
    ~NSConcernPropertyDlg();

    void  SetupWindow() ;
    void 	InitListes() ;
    bool 	InitMedicament(NSMedic* pMedic, string sNoeud) ;
    bool  InitObjectif(NSObjectif* pObj, NSLdvGoal* pGoal) ;
    void  InitDateObjectifPonctuel(string& sDateObj, NSLdvGoal* pGoal, string sDateJalon, double dDelaiJalon, string sUniteJalon) ;
    void 	InitListeActuel() ;
    void 	AfficheListeActuel() ;
    void 	DispInfoListeActuel(TLwDispInfoNotify& dispInfo) ;
    void 	InitListeAncien() ;
    void 	AfficheListeAncien() ;
    void 	DispInfoListeAncien(TLwDispInfoNotify& dispInfo) ;
    void 	InitListeExams() ;
    void 	AfficheListeExams() ;
    void 	DispInfoListeExams(TLwDispInfoNotify& dispInfo) ;
    void  EvDrawItem(uint ctrlId, DRAWITEMSTRUCT far& drawInfo) ;

    void    CmNouveau() ;
    void    CmRenouveler() ;
    void    CmModifier() ;
    void    CmArreter() ;
    void    CmOrdonnance() ;
    void    CmGlobal() ;
    void    CmLocal() ;
    void    CmOk() ;
    void    CmCancel() ;

  protected:

    LDVFRAME _iFrame ;

  DECLARE_RESPONSE_TABLE(NSConcernPropertyDlg) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
//

class NSConcernPropertyWindow : public TListWindow{
  public:

	  NSConcernPropertyDlg* pDlg;    int                   iRes;
	  NSConcernPropertyWindow(NSConcernPropertyDlg* pere, int resId) : TListWindow(pere, resId)    {
      pDlg = pere ;
      iRes = resId ;
    }

    ~NSConcernPropertyWindow() {}
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    int  IndexItemSelect() ;

  DECLARE_RESPONSE_TABLE(NSConcernPropertyWindow) ;};

class NSNewTrt : public NSUtilDialog
{
  public:

    // constructeur/destructeur
    //NSNewTrt(NSConcernPropertyDlg* pere, string* pTypeDocument, string sPreoccup, NSContexte* pCtx) ;
    NSNewTrt(TWindow *pere, string *pTypeDocument, string sPreoccup, NSContexte *pCtx, string sArcCode, string sCaptionDlg = "") ;
    ~NSNewTrt() ;

    // fonctions
    void            CmOk() ;
    void            CmCancel() ;
    void            SetupWindow() ;

    // variables
    NSUtilLexique   *pType ;
    string          *pTypeDocum ;
    string          sPreoccupation ;
    string          sDlgCaption ;
    string          sCodeArc ;
    //  NSConcernPropertyDlg  *pDlg;

  DECLARE_RESPONSE_TABLE(NSNewTrt) ;
} ;

class NSMultiDocsWindow ;

class NSOpenMultiDocsDlg : public NSUtilDialog
{
  public:

    ArrayObjets*        pObjectsArray ;
    NSMultiDocsWindow*  pObjectsList ;
    OWL::TButton*       pSelectAllBtn ;

    NSOpenMultiDocsDlg(NSLdvView* pView, NSContexte* pCtx, ArrayObjets* pObjects) ;
    ~NSOpenMultiDocsDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void CmSelectAll() ;
    void SetupWindow() ;
    void dblClickDoc() ;

    void InitListeDocs() ;
    void AfficheListeDocs() ;
    void DispInfoListeDocs(TLwDispInfoNotify& dispInfo) ;

  DECLARE_RESPONSE_TABLE(NSOpenMultiDocsDlg) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les médicaments à modifier)
//
class NSMultiDocsWindow : public TListWindow
{
  public:

    NSOpenMultiDocsDlg* pView;    int                 iRes;
    NSMultiDocsWindow(NSOpenMultiDocsDlg* pere, int resId, TModule* module = 0) ;
    ~NSMultiDocsWindow() {}    void 	SetupWindow() ;
    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);    int     IndexItemSelect();

    void    EvSetFocus(HWND hWndLostFocus) ;

  DECLARE_RESPONSE_TABLE(NSMultiDocsWindow);};

class NSConvocDlg : public NSUtilDialog
{
  public:

    OWL::TStatic        *pDateDebTxt ;
    NSUtilEditDateHeure *pDateDeb ;

    OWL::TStatic        *pNbJoursTxt ;
    NSEditNum           *pNbJours ;
    NSComboBox          *pCBDureeTtt ;

    NVLdVTemps*         ptpsDeb ;

    NSConvocDlg(TWindow* pPere, NSContexte* pCtx, NVLdVTemps* pDeb) ;
    ~NSConvocDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    void ExecutedAfterDelayChoiceDate() ;

  DECLARE_RESPONSE_TABLE(NSConvocDlg) ;
};

class NSValueInfoDlg : public NSUtilDialog
{
  public:

    OWL::TStatic *_pPath ;
    OWL::TEdit   *_pContent ;

    string       _sPath ;
    string       _sContent ;

    NSValueInfoDlg(TWindow* pPere, NSContexte* pCtx, string *pPath, string *pContent) ;
    ~NSValueInfoDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    string getContent() { return _sContent ; }

  DECLARE_RESPONSE_TABLE(NSValueInfoDlg) ;
};

class NSIssuesListWindow ;

class NSSelectIssuesDlg : public NSUtilDialog
{
  public:

    int*                _pSelectedIssues ;
    size_t              _iSelectedCount ;
    ArrayConcern*       _pIssuesArray ;
    NSIssuesListWindow* _pIssuesList ;

    NSSelectIssuesDlg(TWindow* pPere, NSContexte* pCtx, ArrayConcern* pIssues, int* pSelectedIssues, int iSelectedCount) ;
    ~NSSelectIssuesDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void dblClickOnIssue() ;

    void InitIssuesList() ;
    void DisplayIssuesList() ;
    void DispInfoIssuesList(TLwDispInfoNotify& dispInfo) ;

  DECLARE_RESPONSE_TABLE(NSSelectIssuesDlg) ;
};

//
// "ListWindow" objet with double-click management (to select an issue)
//
class NSIssuesListWindow : public TListWindow
{
  public:

    NSSelectIssuesDlg* _pView ;    int                _iRes ;
    NSIssuesListWindow(NSSelectIssuesDlg* pere, int resId, TModule* module = 0) ;    ~NSIssuesListWindow() {}
    void 	SetupWindow() ;
    void  EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    int   IndexItemSelect() ;

    void  EvSetFocus(HWND hWndLostFocus) ;

  DECLARE_RESPONSE_TABLE(NSIssuesListWindow) ;};

#endif // __NSLDVVAR_H

