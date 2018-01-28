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

# include "nsbb\nsExport.h"
# include "nsbb\nspatpat.h"
# include "nsbb\nsedilex.h"
# include "nsbb\NSLabel.h"
# include "nsbb\NTListBox.h"

//# include "nssavoir\nsvarray.h"

# include "nsbb\nsmedicphase.h"
# include "nsbb\nsmediccycle.h"

/*
** Fonction qui renvoie l'unite de prise en fonction
** du libele du medicament
*/
std::string _NSBBCLASSE initDispUnit(string sLang, string sCode, string sLib, NSContexte* pContexte) ;
std::string _NSBBCLASSE initDispUnitFr(string sCode, string sLib, NSContexte* pContexte) ;

/*
** Cree le label à afficher pour la prescription par heure
*/
std::string CreateLabelForHour(std::string quant, std::string heure);

class NSPosoManagerGroup ;

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
    virtual void     moveBlock() = 0 ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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
    void   moveBlock() ;

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

    void       moveControl(int iResId, int left, int top, int h , int w) ;

    int        getIdBase()     { return _iIdBase ; }

    TGroupBox* getGroupPoso()  { return _pGrpPoso ; }
    TGroupBox* getGroupCycle() { return _pGrpCycle ; }

    void       display(bool bVisible) ;
    void       enableControl(int RessourceId, bool visible) ;

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
    void initInterfaceElements() ;
    void moveBlock() ;

    void actualisePhase() ;
    void actualiseEndOfPrescription() ;

    void display(bool bVisible) ;

    int  getLastPosoId() ;
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

//-----------------------------------------------------
// Ancienne classe de base servant a la prescription d'un
// médicament
//-----------------------------------------------------
class _NSBBCLASSE NSMedicamentDlg : public NSUtilDialog
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

		TTabControl                   *tabPoso, *tabCycle ;  // Control des differentes saisie
    bool                          _LoadForChange ;       // Indique si la pat patho a ete charge
    TButton*                      _ChangeCyle ;          // Boutton permettant le changment de mode

    //informations générales
    NTComboBox<NSPatPathoArray>   *CycleSimple ;
    NTComboBox<NSPatPathoArray>   *modedeprise ;  // Avant, pendant, après le repas
    NSUtilLexique                 *pEnCasDe ;     // En cas de
    NSUtilLexique                 *pUnitePrise ;  // pilule, goutte , comrimé
    NSCheckBoxControle            *substituable, *remboursable ;

    // Control servant a l'affichage des cycles par jours
    NSCheckBoxControle            *pLundi, *pMardi, *pMercredi, *pJeudi, *pVendredi, *pSamedi, *pDimanche ;


    NSRadioButtonControle         *pJour1, *pJour2 ;
    NSUpDownEdit                  *_numJour ;  // Cycle Regulier
    TStatic                       *_numJourLabel ;

    NSTexteLibre                  *pEditTextLibre ;  // texte libre pour les posologie

    NSUpDownEditHeureControle     *pPriseHeure ;   // Solution servant au prise par heure
    NTTList<PriseHeure, NSMedicamentDlg> *ViewPriseHeure ; // Listbox recapitualant les prise médicamenteuse horaires

    // rythme circadien  normal   (matin, midi, soir , coucher)
    NSUpDownEdit   *pPriseMatin, *pPriseMidi, *pPriseSoir, *pPriseCoucher ;

    NSUpDownEdit   *priseUnique ; // Prise unique

    // rythme cyrcadien avance  (Reveil,MATIN, midi, gouter, coucher, soir, nuit)
    NSUpDownEdit   *pPriseReveil, *pPriseMatin2, *pPriseMidi2, *pPriseGouter, *pPriseCoucher2, *pPriseSoir2,  *pPriseNuit ;

    NSUpDownEdit*  _quantRCycle ;  // Cycle Regulier
    NSUpDownEdit*  _freqRCycle ;   // Cycle Regulier
    NSComboBox*    _RCycleComboF ; // Cycle Regulier

		NSUpDownEdit*  _quantFCycle ;     // Cycle irregulier
		NSUpDownEdit*  _quantFoisCycle ;  // Cycle irregulier
		NSUpDownEdit*  _quantFreqFCycle ; // Cycle irregulier
		NSComboBox*    _FCycleComboF ;    // Cycel irregulier

		NSUpDownEdit   *pDureeCure ;      // rythme régulier
		NSComboBox     *psymDureeCure ;   // rythme régulier
		NSUpDownEdit   *pDureeCycleR ;    // rythme régulier
		NSComboBox     *psymDureeCycleR ; // rythme régulier

		NSUpDownEdit   *pDureeCureF ;         // rythme libre
		NSComboBox     *psymDureeCureF ;      // rythme libre
		NSUpDownEdit   *pDureeCureFTime ;     // rythme libre
		NSUpDownEdit   *pDureeCycleFreqF ;    // rythme libre
		NSComboBox     *psymDureeCycleFreqF ; // rythme libre

		NSUpDownEdit   *pPrise10h ;
		NSUpDownEdit   *pFractionUnite ;

		NSUtilLexique  *medicname ;

     // variables de phases
    NSUpDownEdit   *pDureePhase ;
    NSComboBox     *pCBDureePhase ;
    NSUpDownEdit   *pRenouvellement ;

    TButton        *ok, *cancel, *help;
    TGroupBox      *_PhaseGroup, *_CycleGroup;
    bool           _extendedForm;    // Mode etendu avec gestion des phases  A ne pas toucher
    bool           _extendtedCycle;  // Mode etendu avec gestion des cycles  A ne pas toucher
    bool           _Cycle;           // Indique si on est en mode etendu ou en mode simple pour les cycles
    bool           _Phase;           // Indique si on est en mode etendu ou en mode simple pour les cycles
    int            _topPhase;        //debut du group box des phases

    NSUtilEditDateHeure           *pDateOuverture, *pDateFermeture;  // Date d'ouverture  et de fermeture
    TStatic                       *idc_phase1,  *idc_static3, *idc_static2;
    NTTList<NSphaseMedic, NSMedicamentDlg>          *phaseBox;
    TButton                       *ModePhase;
    TButton                       *AddPhase;       // Bouton permettatnd 'ajouter une phase
    NTTList<NSMedicCycleGlobal, NSMedicamentDlg>          *viewCycle;

  protected:

		std::string                 sLexiqCode ;     // code lexique du médicament
		std::string                 sPriseUnit;
		std::string                 sDateOuverture ; // date d'ouverture
		std::string                 sDateFermeture ; // date de fermeture
		std::string                 sEnCasDe_Code ;  // en cas de

		NSPatPathoArray             *pPPT ;          // pathpatho racine contenant le médicament

		// Pointeur sur les informations
		NSphaseMedicArray           *pPhases ;       // Tableau contenant les phases
		NSphaseMedic                *currentPhase ;  // Phase courante

		NSVectPatPathoArray         *prise_hours;    // Tableau contenant les prescriptions horaires
		char                        phaseIndex;      // index des phases
		char                        rythmIndex;      // index des rythms

  public:

		NSMedicamentDlg(TWindow* parent, NSContexte *pCtx,NSPatPathoArray *pPPToInit);
    ~NSMedicamentDlg();

  public:

    void    CmOk() ;
    void    CmCancel() ;
    void    SetupWindow() ;
    void    TabSelChangePso(TNotify far& nm) ;  // Effectue le changment dans les posogie
    void    TabSelChangeCyc(TNotify far& nm) ;  // Effectue les changements dans les cycles
    void    EvSetFocus(HWND hWndLostFocus) ;
    void    AddHour() ;
    void    YAddPhase() ;
    void    lanceExtended() ;                  // Change la tete de la fenetre pour inclure les phases multiples
    void    EvListBoxSelChange() ;
    void    EvListBoxSelPhase() ;
    void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void    EvListBoxSelCycle() ;
    void    AddCRythm() ;
    void    FuncMoveForHour() ;
    void    DeletePhase() ;
    void    EvCycleKey() ;
    void    ExecutedAfterMedicamentSelection() ;  // Fonction s'executant apres la selection du medicament
    void    InitModeSimple() ;
    /*
    ** Fonction qui controle l'aspect globale de la fenetre
    */
    void    ControleWindow() ;
    void    KeyForCircadien(TTabKeyDown& nmHdr);
    void    KeyForRythme(TTabKeyDown& nmHdr);

  protected:

    bool    checkValidity();
    void    saveCycle();
    void    CreateTree();
    bool    chercheNoeud(string sName) ;
    void    lanceCorrespondedTypeCycle(NSPatPathoArray *pPPTInt);
    void    setDateOuverture(string sDateBrut)  { sDateOuverture = sDateBrut ; }  //affiche la date d'ouverture
    void    initPosoAndCycleForDrug() ;       
    void    InitComboModedePrise();
    void    RedrawForm(bool extend);
    void    InitMedicName() ;
    void    InitTabPosologie() ;
    void    InitPosoControls() ;
    void    InitTabCycle() ;
    void    InitCycleControls() ;
    void    updateCycle(int i) ;
    void    updatePosologie(int i) ;
    void    PrintTabCycle(bool print) ;
    void    DrawCycleMode() ;
    void    DrawCycleExtended(bool bForceRedraw = false) ;
    void    DrawCycleLimited(bool bForceRedraw = false) ;
    void    FullPriseUnit() ; // Se declenche apres le choix d'une unite de prise
    void    MoveControl(TControl* cont, int left, int top, int h , int w);   // deplacement d'un controle
    void    EnableControl(int RessourceId, bool visible);       // affiche ou fait disparaitre un controle true pour afficher et false pour disparaitre
    void    ActualisePhasePreview();
    void    AcutaliseCyclePreview();
    bool    ParseMedicament();    // Function qui charge un medicament
    bool    loadDateOuverture(PatPathoIter& iter, PatPathoIter& pptEnd);  // Methode qui charge la date d'ouverture
    bool    loadDateFermeture(PatPathoIter& iter, PatPathoIter& pptEnd); // Methode qui charge la date de fermeture
    bool    RecupereData(PatPathoIter& iter, PatPathoIter& pptEnd); // Recupe les donnees
    bool    LoadUniteDePrise(PatPathoIter& iter, PatPathoIter& pptEnd); // Charge l'unite de prise
    bool    LoadAdmin(PatPathoIter& iter, PatPathoIter& pptEnd); // Charge l'unite de prise
    void    LoadPhaseOnViewAndConfigure(); // charge les phase dans la liste box
    void    SaveSimpleRythme(); // Sauvegarde si choix d'un rythme predetermine
    void    SaveChangeHour(); // Sauvegarde le changement d'heure
    void    ActualiseEndOfPrescription(); // Acutalise la date de fin de traitement

  DECLARE_RESPONSE_TABLE(NSMedicamentDlg) ;
};

class _NSBBCLASSE NSRenewMedicDlg : public NSUtilDialog
{
	public:

		NSPatPathoArray *pPPT ;
		string          sLexique ;
		string          sDateRenouvelement ;
		int             iDureeCycle ;
		string          sUnitDureeCycle ;      // correspond au code lexique
		int             iNbRenouvelement ;

		// variables de phases
		NSUpDownEdit    *pDureeCycle ;
		NSComboBox      *pCBDureeCycle ;
		NSUpDownEdit    *pRenouvelement ;

		TButton         *pOK ;
		TButton         *pCancel ;
		TButton         *pHelp ;

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

class _NSBBCLASSE NSMedicModifPosoDlg : public NSUtilDialog
{
	public:

		NSPatPathoArray *pPPT ;
		string          sLexique ;
		string          sDateRenouvelement ;
		int             iDureeCycle ;
		string          sUnitDureeCycle ;      // correspond au code lexique
		int             iNbRenouvelement ;

		// variables de phases
		NSUpDownEdit    *pDureeCycle ;
		NSComboBox      *pCBDureeCycle ;
		NSUpDownEdit    *pRenouvelement ;

		TButton         *pOK ;
		TButton         *pCancel ;
		TButton         *pHelp ;

		// constructors/destructor
		NSMedicModifPosoDlg(TWindow *parent, NSContexte *pCtx, NSPatPathoArray *pPPTinit) ;
		~NSMedicModifPosoDlg() ;

		// fonctions liées à la boite de dialogue
		void    SetupWindow() ;
		bool    chercheNoeud(string sName) ;

		void    CmOk() ;
		void    CmCancel() ;
		void    CmHelp() ;

	DECLARE_RESPONSE_TABLE(NSMedicModifPosoDlg) ;
} ;

#endif // __NSMEDICDLG_H__

