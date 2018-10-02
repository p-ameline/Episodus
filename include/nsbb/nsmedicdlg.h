// -----------------------------------------------------------------------------
// nsmedicdlg.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------
                                     
#ifndef __NSMEDICDLG_H__
# define __NSMEDICDLG_H__

# include <owl\tabctrl.h>
# include <owl\commctrl.h>
# include <owl\groupbox.h>
# include <owl\listview.h>
# include <vector>

# include "nssavoir\nsBdmDriver.h"

# include "nsbb\nsExport.h"
# include "nsbb\nspatpat.h"
# include "nsbb\nsedilex.h"
# include "nsbb\NSLabel.h"
# include "nsbb\NTListBox.h"

//# include "nssavoir\nsvarray.h"

# include "nsbb\nsmedicdlgbase.h"
# include "nsbb\nsmedicphase.h"
# include "nsbb\nsmediccycle.h"

class NSPosoManagerGroup ;

/*

class _NSBBCLASSE NSPosoInterfaceBlock : public NSRoot
{
	public :

		NSPosoInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoInterfaceBlock() ;

    void             setPatho(NSPatPathoArray *pPpt) ;
    NSPatPathoArray* getPatho() { return &_PPT ; }

    virtual string   getTitle() = 0 ;
    virtual bool     parsePpt(NSPatPathoArray *pPpt) = 0 ;
            string   getNbDose(PatPathoIter& pptIter, PatPathoIter& pptend) ;

            bool     initFromPatPatho() { return parsePpt(&_PPT) ; }
    virtual bool     freshenPatPatho() = 0 ;
            void     addElement(std::string sroot, std::string quant, NSPatPathoArray* pat, int col) ;
            bool     isValidValue(string* psValue) ;

    virtual void     display(bool bVisible) = 0 ;
    virtual void     setupWindow() = 0 ;
    virtual void     moveBlock(TGroupBox *pInsideGroup = 0) = 0 ;

    virtual int      getRecquiredHeight() = 0 ;
    virtual int      getRecquiredWidth() = 0 ;

    virtual void     initPosition(NS_CLASSLIB::TRect theRect) = 0 ;

	protected :

  	NSPosoManagerGroup* _parent ;
		NSPatPathoArray     _PPT ;
} ;

typedef vector<NSPosoInterfaceBlock*> NSPosoInterfaceBlockVector ;
typedef NSPosoInterfaceBlockVector::iterator NSPosoInterfaceBlockIter ;typedef NTArray<NSPosoInterfaceBlock> NSPosoInterfaceBlockArray ;

class NSPosoMMSCInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoMMSCInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoMMSCInterfaceBlock() ;

  	string getTitle()    { return string("morningNoonNight") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

    NTArray<TGroupBox> aGroups ;
    NSUpDownEditArray  aEdits ;
} ;

class NSPosoLMMGSCNInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoLMMGSCNInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoLMMGSCNInterfaceBlock() ;

  	string getTitle()    { return string("periodOfTime") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		NTArray<TGroupBox> aGroups ;
    NSUpDownEditArray  aEdits ;
} ;

class NSPosoSingleTakeInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoSingleTakeInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoSingleTakeInterfaceBlock() ;

  	string getTitle()    { return string("singleTake") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Single take
    TGroupBox     *_pGroupPriseUnique ;
    NSUpDownEdit  *priseUnique ;
} ;

class NSPosoHoursInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoHoursInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoHoursInterfaceBlock() ;

  	string getTitle()    { return string("hours") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

    bool   loadPriseHeure(PatPathoIter& pptIter, PatPathoIter& pptend) ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

    void   saveChangeHour() ;
    void   funcMoveForHour() ;

	protected :

		// --- Specific hours
    TGroupBox     *_pGroupHourQtty, *_pGroupHour ;
    TButton       *_pNewHourlyTake ;
    NSUpDownEditHeureControle *pPriseHeure ;

    NTTList<PriseHeure, NSPosoHoursInterfaceBlock> *ViewPriseHeure ;
    std::vector<PriseHeure*>* _prises ;
} ;

class NSPosoRegularCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoRegularCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoRegularCycleInterfaceBlock() ;

  	string getTitle()    { return string("regularCycle") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Regular rythm (always the same interval of time between each take)
    TGroupBox     *_pGroupNbOfUnit, *_pGroupFrequency ;
    TStatic       *_pStaticEvery ;

    NSUpDownEdit*  _quantRCycle ;
    NSUpDownEdit*  _freqRCycle ;

    NSComboBox*    _RCycleComboF ;
    string         _sInitForCombo ;
} ;

class NSPosoFreeCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoFreeCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoFreeCycleInterfaceBlock() ;

  	string getTitle()    { return string("irregularCycle") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Non regular rythm (take X times, at will within a given interval)
    TGroupBox     *_pGroupFNbOfUnit, *_pGroupFTime, *_pGroupFFreq ;
    TStatic       *_pStaticFEvery ;

		NSUpDownEdit*  _quantFCycle ;
		NSUpDownEdit*  _quantFoisCycle ;
		NSUpDownEdit*  _quantFreqFCycle ;

		NSComboBox*    _FCycleComboF ;
    string         _sInitForCombo ;
} ;

class NSPosoFreeTextInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSPosoFreeTextInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSPosoFreeTextInterfaceBlock() ;

  	string getTitle()    { return string("freeText") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Free text
    NSTexteLibre *pEditTextLibre ;
    string       _sText ;
} ;

class NSEverydayCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSEverydayCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSEverydayCycleInterfaceBlock() ;

  	string getTitle()    { return string("everyDay") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

  	NSPatPathoArray* createRegularRythme(std::string dure_cure, std::string symcure, std::string duree_cycle, std::string SymCycle) ;

		// --- Usual cycles (every day, each other day...)
    //
    NTComboBox<NSPatPathoArray> *CycleSimple ;

    int iSelectedIndex ;
} ;

class NSDaysOfWeekCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSDaysOfWeekCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSDaysOfWeekCycleInterfaceBlock() ;

  	string getTitle()    { return string("daysOfTheWeek") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

  	NTArray<NSCheckBoxControle> aControls ;
} ;

class NSD1D2D3CycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSD1D2D3CycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSD1D2D3CycleInterfaceBlock() ;

  	string getTitle()    { return string("day1Day2Day3") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Day 1, Day 2, Day3...

    NSRadioButtonControle *pJour1, *pJour2 ;
    NSUpDownEdit          *_numJour ;  // Cycle Regulier
    TStatic               *_numJourLabel ;
} ;

class NSRegularCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSRegularCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSRegularCycleInterfaceBlock() ;

  	string getTitle()    { return string("regularRythme") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Regular rythm
    TGroupBox     *_pGroupCure, *_pGroupRCycle ;
    TStatic       *_pStaticCureEvery ;

    NSUpDownEdit  *pDureeCure ;
		NSComboBox    *psymDureeCure ;
    string        _sUnitCure ;

		NSUpDownEdit  *pDureeCycleR ;
		NSComboBox    *psymDureeCycleR ;
    string        _sUnitCycle ;
} ;

class NSFreeCycleInterfaceBlock : public NSPosoInterfaceBlock
{
	public :

		NSFreeCycleInterfaceBlock(NSPosoManagerGroup* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit = 0) ;
		~NSFreeCycleInterfaceBlock() ;

  	string getTitle()    { return string("freeRythme") ; }
    bool   parsePpt(NSPatPathoArray *pPpt) ;
		bool   freshenPatPatho() ;

		void   display(bool bVisible) ;
    void   setupWindow() ;
    void   moveBlock(TGroupBox *pInsideGroup = 0) ;

    int    getRecquiredHeight() { return  54 ; }
    int    getRecquiredWidth()  { return 288 ; }

    void   initPosition(NS_CLASSLIB::TRect theRect) ;

	protected :

		// --- Non regular rythm
    TGroupBox     *_pGroupFCure, *_pGroupFNbre, *_pGroupFCycle ;
    TStatic       *_pStaticFCureEvery ;

		NSUpDownEdit  *pDureeCureF ;
		NSComboBox    *psymDureeCureF ;
    string        _sUnitCure ;

		NSUpDownEdit  *pDureeCureFTime ;

		NSUpDownEdit  *pDureeCycleFreqF ;
		NSComboBox    *psymDureeCycleFreqF ;
    string        _sUnitCycle ;
} ;

class _NSBBCLASSE NSPosoManagerGroup : public NSRoot, public TGroupBox
{
	public :

		NSPosoManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int Id, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;
    NSPosoManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int resourceId, TModule* module = 0) ;
    ~NSPosoManagerGroup() ;

    void       CmOk() ;
    void       CmCancel() ;
    void       SetupWindow() ;
    void       TabSelChangePoso(TNotify far& nm) ;  // Posologie change
    void       TabSelChangeCycle(TNotify far& nm) ; // Cycle change
    void       EvSetFocus(HWND hWndLostFocus) ;
    void       AddHour() ;

    void       updateCycle(int iIndex) ;
    void       updatePosologie(int iIndex) ;

    void       moveBlock(int left, int top) ;
    void       moveControl(int iResId, int left, int top, int w, int h, TGroupBox *pInsideGroup = 0) ;
    void       moveSonControl(int iSonId, int left, int top, int w, int h, TGroupBox *pInsideGroup = 0) { moveControl(getResIdFromSonId(iSonId), left, top, w, h, pInsideGroup) ; }

    int        getIdBase()     { return _iIdBase ; }

    TGroupBox* getGroupPoso()  { return _pGrpPoso ; }
    TGroupBox* getGroupCycle() { return _pGrpCycle ; }

    void       display(bool bVisible) ;
    void       enableControl(int RessourceId, bool visible) ;

    void       initInterfaceElements() ;

	protected :

		// To switch between various modes
  	TTabControl *tabPoso,   *tabCycle ;
    TGroupBox   *_pGrpPoso, *_pGrpCycle ;

		// Interface elements for daily take
    //
    NSPosoInterfaceBlockArray aDailyInterfaces ;
    NSPosoInterfaceBlock*     _pActiveDailyBlock ;

    // Interface elements for establishing the repetition of this day
    //
    NSPosoInterfaceBlockArray aCycleInterfaces ;
    NSPosoInterfaceBlock*     _pActiveCycleBlock ;

		NSPatPathoArray           _PPT ;

    int                       _iIdBase ;

    void createInterfaceElements() ;
    void deleteInterfaceElements() ;
    bool parsePpt() ;
    bool parsePptCircadian(NSPatPathoArray* pPPT) ;
    bool parsePptCycle(NSPatPathoArray* pPPT) ;
    bool recupere(PatPathoIter& pptIter, PatPathoIter& pptend, int Col, NSContexte* pContexte) ;

    void initTabPosologie() ;
    void drawPoso() ;

    void initTabCycle() ;
    void drawCycle() ;

    int  getResIdFromSonId(int iSonId) { return _iIdBase + iSonId ; }

	DECLARE_RESPONSE_TABLE(NSPosoManagerGroup) ;
} ;

typedef vector<NSPosoManagerGroup*> NSPosoManagerGroupVector ;
typedef NSPosoManagerGroupVector::iterator NSPosoManagerGroupIter ;typedef NSPosoManagerGroupVector::reverse_iterator NSPosoManagerGroupReverseIter ;typedef NTArray<NSPosoManagerGroup> NSPosoManagerGroupArray ;

class _NSBBCLASSE NSPhaseManagerGroup : public NSRoot, public TGroupBox
{
	public :

		NSPhaseManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int Id, const char far *text, int x, int y, int w, int h, TModule* module = 0) ;
    NSPhaseManagerGroup(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, int resourceId, int iBaseId, TModule* module = 0) ;
    ~NSPhaseManagerGroup() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    void show() { display(true) ; }
    void hide() { display(false) ; }

    int        getPhaseId()           { return _iIdBase ; }
    NVLdVTemps getPhaseStartingDate() { return _tStartingDate ; }
    NVLdVTemps getPhaseClosingDate()  { return _tClosingDate ; }

    void       moveControl(int iResId, int left, int top, int w, int h) ;

    void       enableControl(int RessourceId, bool visible) ;
    void       moveBlock() ;
    void       initInterfaceElements() ;

	protected :

  	OWL::TStatic        *pDureePhaseTxt ;
		NSUpDownEdit        *pDureePhase ;
		NSComboBox          *pCBDureePhase ;
  	OWL::TStatic        *pRenouvellementTxt ;
		NSUpDownEdit        *pRenouvellement ;
  	OWL::TStatic        *pRenouvellementTimeTxt ;

  	OWL::TStatic        *pDateDebPrescrTxt ;
  	NSUtilEditDateHeure *pDateDebPrescr ;
  	OWL::TStatic        *pDateFinPrescrTxt ;
  	NSUtilEditDateHeure *pDateFinPrescr ;

  	NSPosoManagerGroup  *pPosologieGroup ;

    NSPosoManagerGroupArray _aPosos ;

		NSPatPathoArray     _PPT ;
    int                 _iIdBase ;

    NVLdVTemps          _tStartingDate ;
		NVLdVTemps          _tClosingDate ;

    int                 _iDureeCycle ;
		std::string         _sUnitDureeCycle ;
		int                 _iNbRenouvellement ;

    bool parsePpt() ;
    bool loadDate(PatPathoIter& pptIter, PatPathoIter& pptIEnd, NVLdVTemps* pDate) ;
    bool loadDuree(PatPathoIter& pptIter, PatPathoIter& pptIEnd) ;
		bool loadRenouvellement(PatPathoIter& pptIter, PatPathoIter& pptIEnd) ;
    bool recupereData(PatPathoIter& iter, PatPathoIter& pptEnd) ;

    void createControls() ;

    void actualisePhase() ;
    void actualiseEndOfPrescription() ;

    void display(bool bVisible) ;

    int  getLastPosoId() ;
    int  getNextPosoId() ;
} ;

typedef vector<NSPhaseManagerGroup*> NSPhaseManagerGroupVector ;
typedef NSPhaseManagerGroupVector::iterator NSPhaseManagerGroupIter ;typedef NSPhaseManagerGroupVector::reverse_iterator NSPhaseManagerGroupReverseIter ;typedef NTArray<NSPhaseManagerGroup> NSPhaseManagerGroupArray ;

//-----------------------------------------------------
// Classe de base servant a la prescription d'un
// médicament
//-----------------------------------------------------
class _NSBBCLASSE NSComplexMedicamentDlg : public NSUtilDialog
{
	public:

		NSComplexMedicamentDlg(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit) ;
    ~NSComplexMedicamentDlg() ;

		void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

    void enableControl(int RessourceId, bool visible) ;
    void moveControlWithScreenCoordinates(TControl* cont, NS_CLASSLIB::TRect& rect) ;   // deplacement d'un controle
    void moveControlWithDialogCoordinates(TControl* cont, NS_CLASSLIB::TRect& rect) ;   // deplacement d'un controle

	protected:

		NSPatPathoArray             _PPT ;          // pathpatho racine contenant le médicament

    std::string                 sLexiqCode ;     // code lexique du médicament
		std::string                 sPriseUnit;
		std::string                 sDateOuverture ; // date d'ouverture
		std::string                 sDateFermeture ; // date de fermeture
		std::string                 sEnCasDe_Code ;  // en cas de

    // Interface elements
    //
    OWL::TGroupBox              *pTrtGroup ;

  	NSUtilLexique               *pType ;
    OWL::TStatic                *pUnitePriseTxt ;
    NSUtilLexique               *pUnitePrise ;  // pilule, goutte , comrimé

    OWL::TStatic                *modedepriseTxt ;
		NTComboBox<NSPatPathoArray> *modedeprise ;  // Avant, pendant, après le repas
    OWL::TStatic                *pEnCasDeTxt ;
    NSUtilLexique               *pEnCasDe ;     // En cas de

    NSCheckBoxControle          *substituable, *remboursable ;

		// Date d'ouverture  et de fermeture
    OWL::TStatic                *pDateDebTxt ;
		NSUtilEditDateHeure         *pDateDeb ;
		OWL::TGroupBox              *pDateFinGroup ;
  	OWL::TRadioButton           *pRChronique ;
  	OWL::TRadioButton           *pRDans ;
  	OWL::TRadioButton           *pRDuree ;
  	OWL::TRadioButton           *pRLe ;
  	NSEditNum                   *pNbJours ;
  	NSComboBox                  *pCBDureeTtt ;
  	NSUtilEditDateHeure         *pDateFin ;

    OWL::TGroupBox              *pPrescriptionGroup ;
    NSPhaseManagerGroupArray    _aPhases ;

    NSPhaseManagerGroup         *pActivePhase ;

    TButton                     *ok, *cancel, *help ;

    void createControls() ;

    bool parsePpt() ;
    bool loadDateOuverture(PatPathoIter& iter, PatPathoIter& pptEnd) ; // Methode qui charge la date d'ouverture
    bool loadDateFermeture(PatPathoIter& iter, PatPathoIter& pptEnd) ; // Methode qui charge la date de fermeture
    bool loadUniteDePrise(PatPathoIter& iter, PatPathoIter& pptEnd) ;  // Charge l'unite de prise
    bool loadAdmin(PatPathoIter& iter, PatPathoIter& pptEnd);          // Charge l'unite de prise
    bool recupereData(PatPathoIter& iter, PatPathoIter& pptEnd) ;      // Recupe les donnees

    bool checkValidity() ;
    void createTree() ;

    void initComboModedePrise() ;

    void executedAfterDrugSelection() ;
		void executedAfterTrtBeginDate() ;
		void executedAfterTrtEndDate() ;

    // Phases management
    int                  getLastPhaseId() ;
    NSPhaseManagerGroup* getCurrentPhase() ;
    NSPhaseManagerGroup* getNextPhase(NSPhaseManagerGroup *pCurrentPhase) ;
    NSPhaseManagerGroup* getPrevPhase(NSPhaseManagerGroup *pCurrentPhase) ;
    void                 displayCurrentPhase() ;

	DECLARE_RESPONSE_TABLE(NSComplexMedicamentDlg) ;
} ;

*/

class NSPosologieBlock ;

//-----------------------------------------------------
// Ancienne classe de base servant a la prescription d'un
// médicament
//-----------------------------------------------------
class _NSBBCLASSE NSPosoIncludeDlg : public NSPrescriptionBaseDlg
{
	public:

    NSPosologieBlock* _pPosoBlock ;

    int               _topOfBottomControls ;  //debut du group box des phases

    NSPosoIncludeDlg(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit, TResId resID, TModule* module = 0) ;
    ~NSPosoIncludeDlg() ;

    void  SetupWindow() ;
    void  InitPosoControlsForPhase(NSphaseMedic* pPhase) ;

    bool  loadDateOuverture(PatPathoIter& iter, PatPathoIter& pptEnd) ; // Methode qui charge la date d'ouverture
    bool  loadDateFermeture(PatPathoIter& iter, PatPathoIter& pptEnd) ; // Methode qui charge la date de fermeture

		void  SaveSimpleRythme() ; // Sauvegarde si choix d'un rythme predetermine
    void  EnableControl(int RessourceId, bool visible) ;       // affiche ou fait disparaitre un controle true pour afficher et false pour disparaitre
    void  MoveControl(TControl* cont, int left, int top, int h, int w) ;   // deplacement d'un controle

    void  setTopOfBottomControls(int iTopLevel) { _topOfBottomControls = iTopLevel ; }

    void  saveChangeHour() ; // Sauvegarde le changement d'heure
    void  FuncMoveForHour() ;
    void  updateCyclePreview() ;
    void  EvCycleKey() ;

    void  TabSelChangePso(TNotify far& nm) ;  // Changement de tab dans les posologies
    void  TabSelChangeCyc(TNotify far& nm) ;  // Changement de tab dans les cycles
    void  KeyForCircadien(TTabKeyDown& nmHdr) ;
    void  KeyForRythme(TTabKeyDown& nmHdr) ;

    void  DrawCycleMode() ;
    void  AddHour() ;
    void  AddCRythm() ;
    void  EvListBoxSelChange() ;
    void  EvListBoxSelCycle() ;

    virtual void RedrawForm(bool extend) = 0 ;

    void  setMultiCycleMode(bool bNewMode) ;

	DECLARE_RESPONSE_TABLE(NSPosoIncludeDlg) ;
} ;

//-----------------------------------------------------
// Classe de base de modification de posologie
//-----------------------------------------------------
class _NSBBCLASSE NSPosologieBlock : public NSRoot
{
  friend class NSRPrescription ;
  friend class NSRCircadien ;
  friend class NSparseMedic ;
  friend class NSphaseMedic ;
  friend class NSRythme ;
  friend class DayRythme ;
  friend class Day1Day2Day3Rythme ;
  friend class RythmeRegulier ;
  friend class RythmeFree ;
  friend class CircBasePeriode ;
  friend class CircBaseMMS ;
  friend class NSCircadien ;
  friend class CircBaseText ;
  friend class CircBaseRegular ;
  friend class CircBaseFree ;
  friend class CircBaseHeures ;
  friend class CircUnique ;
  friend class RythmeSimple ;

  protected:

  	NSPosoIncludeDlg*                 _pDialog ;

    bool                              _LoadForChange ;       // Indique si la pat patho a ete charge
    TButton*                          _ChangeCyle ;          // Boutton permettant le changment de mode

    //informations générales
    NTComboBox<NSPatPathoArray>      *_CycleSimple ;

    std::vector<NSMedicCycleGlobal*> *_Cycles ;
    int                               _currentCycle ; // Cyclecourrant
    bool                              _Monocycle ;    // Cycle mono phase

    // ------------ Posologie ---------------

    TGroupBox                        *_PosoGroup ;
    TTabControl                      *_tabPoso ;

    // rythme circadien  normal   (matin, midi, soir , coucher)
    NSUpDownEdit   *_pPriseMatin, *_pPriseMidi, *_pPriseSoir, *_pPriseCoucher ;

    // rythme cyrcadien avance  (Reveil, matin, midi, gouter, coucher, soir, nuit)
    NSUpDownEdit   *_pPriseReveil, *_pPriseMatin2, *_pPriseMidi2, *_pPriseGouter, *_pPriseCoucher2, *_pPriseSoir2,  *_pPriseNuit ;

    NSUpDownEditHeureControle* _pPriseHeure ;   // Solution servant au prise par heure
    NTTList<PriseHeure, NSPosoIncludeDlg>* _ViewPriseHeure ; // Listbox recapitualant les prise médicamenteuse horaires

    NSUpDownEdit*  _quantRCycle ;  // Cycle Regulier
    NSUpDownEdit*  _freqRCycle ;   // Cycle Regulier
    NSComboBox*    _RCycleComboF ; // Cycle Regulier

		NSUpDownEdit*  _quantFCycle ;     // Cycle irregulier
		NSUpDownEdit*  _quantFoisCycle ;  // Cycle irregulier
		NSUpDownEdit*  _quantFreqFCycle ; // Cycle irregulier
		NSComboBox*    _FCycleComboF ;    // Cycel irregulier

    NSUpDownEdit*  _priseUnique ; // Prise unique

    NSTexteLibre*  _pEditTextLibre ;  // texte libre pour les posologie

    // ------------ Cycle ---------------

    TGroupBox      *_CycleGroup ;
    TTabControl    *_tabCycle ;        // Tab
    bool           _extendtedCycle ;  // Mode etendu avec gestion des cycles  A ne pas toucher
    bool           _Cycle ;           // Indique si on est en mode etendu ou en mode simple pour les cycles

    int            _iExtendedCycleGroupY, _iExtendedCycleGroupH ;
    int            _iLimitedCycleGroupY,  _iLimitedCycleGroupH ;

    // Control servant a l'affichage des cycles par jours
    NSCheckBoxControle    *_pLundi, *_pMardi, *_pMercredi, *_pJeudi, *_pVendredi, *_pSamedi, *_pDimanche ;

    NSRadioButtonControle *_pJour1, *_pJour2 ;
    NSUpDownEdit          *_numJour ;  // Cycle Regulier
    TStatic               *_numJourLabel ;

		NSUpDownEdit   *_pDureeCure ;      // rythme régulier
		NSComboBox     *_psymDureeCure ;   // rythme régulier
		NSUpDownEdit   *_pDureeCycleR ;    // rythme régulier
		NSComboBox     *_psymDureeCycleR ; // rythme régulier

		NSUpDownEdit   *_pDureeCureF ;         // rythme libre
		NSComboBox     *_psymDureeCureF ;      // rythme libre
		NSUpDownEdit   *_pDureeCureFTime ;     // rythme libre
		NSUpDownEdit   *_pDureeCycleFreqF ;    // rythme libre
		NSComboBox     *_psymDureeCycleFreqF ; // rythme libre

    NTTList<NSMedicCycleGlobal, NSPosoIncludeDlg> *_viewCycle ;

  protected:

		NSPatPathoArray             *_pPhasePPT ;     // pathpatho racine contenant le médicament

		NSVectPatPathoArray         *_prise_hours ;   // Tableau contenant les prescriptions horaires
		char                        _rythmIndex ;     // index des rythms

  public:

		NSPosologieBlock(NSContexte *pCtx, std::vector<NSMedicCycleGlobal*> *pCycles) ;
    ~NSPosologieBlock() ;

  public:

  	void    SetupWindow() ;

  	// Cycles management
    //
    void    saveCurrentCycle() ;
    void    addCycle() ;

  	void    createControls() ;

    void    TabSelChangePso() ;  // Changement de tab dans les posologies
    void    TabSelChangeCyc() ;  // Changement de tab dans les cycles
    void    EvSetFocus(HWND hWndLostFocus) ;
    void    AddHour() ;
    void    lanceExtended() ;                  // Change la tete de la fenetre pour inclure les phases multiples
    void    EvListBoxSelChange() ;
    void    EvListBoxSelCycle() ;
    void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void    FuncMoveForHour() ;
    void    InitModeSimple() ;
    /*
    ** Fonction qui controle l'aspect globale de la fenetre
    */
    void             KeyForCircadien(int temp) ;
    void             KeyForRythme(int temp) ;

    void             buildPatPatho() ;
    NSPatPathoArray* getPatPatho()                      { return _pPhasePPT ; }
    void             setPatPatho(NSPatPathoArray* pPpt) { _pPhasePPT = pPpt ; }

  // protected:

    void             saveCycle() ;
    bool             chercheNoeud(string sName) ;
    void             lanceCorrespondedTypeCycle(NSPatPathoArray *pPPTInt) ;
    void             initPosoAndCycleForDrug() ;
    void             InitMedicName() ;
    void             InitTabPosologie() ;
    void             InitPosoControls() ;
    void             InitCycleControls(bool bAddMode = false) ;
    void             InitTabCycle() ;
    void             updateCycle(int i) ;
    void             AddCRythm() ;
    void             updatePosologie(int i) ;
    void             PrintTabCycle(bool print) ;
    void             DrawCurrentCycle() ;
    void             DrawCycleMode() ;
    void             DrawCycleExtended(bool bForceRedraw = false) ;
    void             DrawCycleLimited(bool bForceRedraw = false) ;
    bool             RecupereData(PatPathoIter& iter, PatPathoIter& pptEnd) ; // Recupe les donnees
    void             saveChangeHour() ; // Sauvegarde le changement d'heure
    void             updateCyclePreview() ;
    void             EvCycleKey() ;
    void             SaveSimpleRythme() ; // Sauvegarde si choix d'un rythme predetermine

    void             setExtendedCycle(bool bExtCyc) { _extendtedCycle = bExtCyc ; }
    bool             getExtendedCycle()             { return _extendtedCycle ; }

    bool             getCycleMode()                   { return _Cycle ; }
    void             setExtendedCycleMode(bool bMode) { _Cycle = bMode ; }

    void             setCycles(std::vector<NSMedicCycleGlobal*> *pCycles) { _Cycles = pCycles ; }

    NSPosoIncludeDlg* getDialog()                       { return _pDialog ; }
    void              setDialog(NSPosoIncludeDlg* pDlg) { _pDialog = pDlg ; }

    NTTList<NSMedicCycleGlobal, NSPosoIncludeDlg>* getViewCycle() { return _viewCycle ; }

    int              getCurrentCycle() { return _currentCycle ; }

    void             setCycleGroupPositions(int iExtY, int iExtH, int iLimY, int iLimH) ;
};

//-----------------------------------------------------
// Ancienne classe de base servant a la prescription d'un
// médicament
//-----------------------------------------------------
class _NSBBCLASSE NSMedicamentDlg : public NSPosoIncludeDlg
{
  friend class NSRPrescription ;
  friend class NSRCircadien ;
  friend class NSparseMedic ;
  friend class NSphaseMedic ;
  friend class NSRythme ;
  friend class DayRythme ;
  friend class Day1Day2Day3Rythme ;
  friend class RythmeRegulier ;
  friend class RythmeFree ;
  friend class CircBasePeriode ;
  friend class CircBaseMMS ;
  friend class NSCircadien ;
  friend class CircBaseText ;
  friend class CircBaseRegular ;
  friend class CircBaseFree ;
  friend class CircBaseHeures ;
  friend class CircUnique ;
  friend class RythmeSimple ;

  protected:

    std::string                   _sReferential ;
    bool                          _bIsRemboursable ;

    bool                          _LoadForChange ;       // Indique si la pat patho a ete charge

    //informations générales
    OWL::TStatic                  *_pmedicnameTxt ;
    OWL::TStatic                  *_pModePriseTxt ;
    NTComboBox<NSPatPathoArray>   *_modedeprise ;  // Avant, pendant, après le repas
    NSCheckBoxControle            *_remboursable ;

     // variables de phases
    NSUpDownEdit                  *_pDureePhase ;
    NSComboBox                    *_pCBDureePhase ;
    NSUpDownEdit                  *_pRenouvellement ;
    OWL::TStatic                  *_pDateDebPrescrTxt ;
		NSUtilEditDateHeure           *_pDateDebPrescr ;
		OWL::TStatic                  *_pDateFinPrescrTxt ;
		NSUtilEditDateHeure           *_pDateFinPrescr ;

    TButton                       *_ok, *_cancel, *_help ;
    TGroupBox                     *_PhaseGroup ;
    bool                          _extendedForm ;    // Mode etendu avec gestion des phases  A ne pas toucher
    bool                          _Phase ;           // Indique si on est en mode etendu ou en mode simple pour les cycles

    TStatic                       *_idc_phase1,  *_idc_static3, *_idc_static2 ;
    NTTList<NSphaseMedic, NSMedicamentDlg> *_phaseBox ;
    TButton                       *_ModePhase ;
    TButton                       *_AddPhase ;       // Bouton permettatnd 'ajouter une phase
    // NTTList<NSMedicCycleGlobal, NSMedicamentDlg> *viewCycle ;

  protected:

		// Pointeur sur les informations
		NSphaseMedicArray           *_pPhases ;       // Tableau contenant les phases
		NSphaseMedic                *_pCurrentPhase ;  // Phase courante

		NSVectPatPathoArray         *_prise_hours ;    // Tableau contenant les prescriptions horaires
		char                        _phaseIndex ;      // index des phases
		char                        _rythmIndex ;      // index des rythms

    NsSelectableDrug           *_pBdmDrugInformation ;

  public:

		NSMedicamentDlg(TWindow* parent, NSContexte *pCtx, NSPatPathoArray *pPPToInit) ;
    ~NSMedicamentDlg() ;

  public:

    void    SetupWindow() ;
    void    InitControlsForPhase(NSphaseMedic* pPhase) ;
    void    CmOk() ;
    void    CmCancel() ;
    void    EvSetFocus(HWND hWndLostFocus) ;
    void    YAddPhase() ;
    void    lanceExtended() ;                  // Change la tete de la fenetre pour inclure les phases multiples
    void    EvListBoxSelPhase() ;
    void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void    EvListBoxSelCycle() ;
    void    DeletePhase() ;
    void    ExecutedAfterDrugSelection() ;  // Fonction s'executant apres la selection du medicament
    void    ExecutedAfterEventSelection() ;
    void    ExecutedAfterRouteSelection() ;
    void    ExecutedAfterTrtBeginDate() ;
		void    ExecutedAfterTrtEndDate() ;
    void    UserName(WPARAM wParam) ;

    /*
    ** Fonction qui controle l'aspect globale de la fenetre
    */
    void    ControleWindow() ;
    void    KeyForRythme(TTabKeyDown& nmHdr) ;

    NTTList<NSMedicCycleGlobal, NSPosoIncludeDlg>* getViewCycle() { return _pPosoBlock->getViewCycle() ; }

  protected:

    bool    checkValidity();
    void    saveCycle();
    void    CreateTree();
    bool    chercheNoeud(string sName) ;
    void    lanceCorrespondedTypeCycle(NSPatPathoArray *pPPTInt);
    void    setDateOuverture(string sDateBrut)  { _sDateOuverture = sDateBrut ; }  //affiche la date d'ouverture
    void    initPosoAndCycleForDrug() ;
    void    InitComboModedePrise();
    void    RedrawForm(bool extend);
    void    InitMedicName() ;
    void    PrintTabCycle(bool print) ;
    void    FullPriseUnit() ; // Se declenche apres le choix d'une unite de prise
    void    ActualisePhasePreview();
    bool    ParseMedicament() ;    // Function qui charge un medicament

    bool    RecupereData(PatPathoIter& iter, PatPathoIter& pptEnd); // Recupe les donnees
    bool    LoadUniteDePrise(PatPathoIter& iter, PatPathoIter& pptEnd); // Charge l'unite de prise
    bool    LoadAdmin(PatPathoIter& iter, PatPathoIter& pptEnd); // Charge l'unite de prise
    void    LoadPhaseOnViewAndConfigure(); // charge les phase dans la liste box
    void    ActualiseEndOfPrescription(); // Acutalise la date de fin de traitement

  DECLARE_RESPONSE_TABLE(NSMedicamentDlg) ;
};

class _NSBBCLASSE NSRenewMedicDlg : public NSUtilDialog
{
	public:

		NSPatPathoArray     *_pPPT ;
		string              _sLexique ;
		string              _sDateRenouvelement ;
		int                 _iDureeCycle ;
		string              _sUnitDureeCycle ;      // correspond au code lexique
		int                 _iNbRenouvelement ;

		// variables de phases
		NSUpDownEdit        *_pDureeCycle ;
		NSComboBox          *_pCBDureeCycle ;
		NSUpDownEdit        *_pRenouvelement ;

    OWL::TStatic        *_pDateDebPrescrTxt ;
    NSUtilEditDateHeure *_pDateDebPrescr ;
    OWL::TStatic        *_pDateFinPrescrTxt ;
    NSUtilEditDateHeure *_pDateFinPrescr ;

		TButton              *_pOK ;
		TButton              *_pCancel ;
		TButton              *_pHelp ;

		// constructors/destructor
		NSRenewMedicDlg(TWindow *parent, NSContexte *pCtx, NSPatPathoArray *pPPTinit) ;
		~NSRenewMedicDlg() ;

		// fonctions liées à la boite de dialogue
		void    SetupWindow() ;
		bool    chercheNoeud(string sName) ;

		void    CmOk() ;
		void    CmCancel() ;
		void    CmHelp() ;

	DECLARE_RESPONSE_TABLE(NSRenewMedicDlg) ;
} ;

class _NSBBCLASSE NSMedicModifPosoDlg : public NSPosoIncludeDlg
{
	public:

		NSPatPathoArray *_pPPT ;
    NSPatPathoArray *_pPPTGlobal ;
		string          _sLexique ;
		string          _sDateRenouvelement ;
		int             _iDureeCycle ;
		string          _sUnitDureeCycle ;      // correspond au code lexique
		int             _iNbRenouvelement ;

		// variables de phases
		NSUpDownEdit    *_pDureeCycle ;
		NSComboBox      *_pCBDureeCycle ;
		NSUpDownEdit    *_pRenouvelement ;

		NSUtilLexique   *_pUnitePrise ;  // pilule, goutte , comrimé
    NSUtilLexique   *_medicname ;

		TButton         *_pOK ;
		TButton         *_pCancel ;
		TButton         *_pHelp ;

		// constructors/destructor
		NSMedicModifPosoDlg(TWindow *parent, NSContexte *pCtx, NSPatPathoArray *pPPPhaseToModify, NSPatPathoArray *pPPTmedic) ;
		~NSMedicModifPosoDlg() ;

		// fonctions liées à la boite de dialogue
		void    SetupWindow() ;
    void    InitControlsForPhase(NSphaseMedic* pPhase) ;
		bool    chercheNoeud(string sName) ;
    void    RedrawForm(bool extend) ;

		void    CmOk() ;
		void    CmCancel() ;
		void    CmHelp() ;

    void    ExecutedAfterDrugSelection()  {}
    void    ExecutedAfterEventSelection() {}
    void    ExecutedAfterRouteSelection() {}
    void    ExecutedAfterTrtBeginDate()   {}
		void    ExecutedAfterTrtEndDate()     {}

	protected:

		// Pointeur sur les informations
		NSphaseMedicArray *_pPhases ;       // Tableau contenant les phases
		NSphaseMedic      *_pCurrentPhase ;  // Phase courante

    void    createTree() ;

    bool    parsePhase() ;
    bool    ParseMedicament() ;
    bool    LoadUniteDePrise(PatPathoIter& iter, PatPathoIter& pptEnd); // Charge l'unite de prise
    void    ControleWindow() ;

	DECLARE_RESPONSE_TABLE(NSMedicModifPosoDlg) ;
} ;

#endif // __NSMEDICDLG_H__

