// -----------------------------------------------------------------------------
// nsmediccycle.h
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2013/11/25 16:04:30 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#ifndef __NSMEDICCYCLE_H__
# define __NSMEDICCYCLE_H__

# include "nsbb\nsExport.h"
# include "nsbb\nspatpat.h"

# include "nsbb\nsmediccontrols.h"

class NSPosologieBlock ;
class NSMedicDlg ;

bool _NSBBCLASSE isValidValue(string* psValue) ;

void _NSBBCLASSE createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, string  sVal, int iColonne) ;
void _NSBBCLASSE createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, float   fVal, int iColonne) ;
void _NSBBCLASSE createNodeComplement(NSPatPathoArray *pPPT, string sCode, string sUnit, int     iVal, int iColonne) ;
std::string _NSBBCLASSE getValNodeComplement(NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iColonneBase, string sCode, string sUnit) ;

void _NSBBCLASSE func_create_patho_quant(std::string sroot, std::string quant, NSPatPathoArray* pat, int col) ;
/*
** Fonction qui crée un rythme regulier
*/
NSPatPathoArray*  CreateRegularRythme(std::string dure_cure, std::string symcure, std::string duree_cycle, std::string SymCycle, NSContexte* cont);

/*
** Cree un label affichable
*/
std::string CreateLabelForHour(std::string quant, std::string heure);


class NSMedicCycleGlobal;

/*
**   Type de rythme prescrit
*/
enum RythmeType
{
  DayRythm,       // rythme par jour de la semaine
  Day1Day2Day3,   // rythme jour 1, jour 2, jour 3
  RegularRythm,   // rythme regulier
  FreeRythm,      // rythme libre
  RythmSimple
};

class RythmeBase
{
  protected:

    NSMedicCycleGlobal*  _parent ;

  public:

    RythmeBase(NSMedicCycleGlobal* temp) : _parent(temp) {}

    virtual void             Load() = 0 ;
    virtual void             save() = 0 ;
    virtual bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) = 0 ;  // Charge la pate patho
    virtual NSPatPathoArray* SimplifiedTreeForInterpretation() = 0 ;    // cree l'arbre correspondant au rythme

    virtual std::string      IsValid() = 0 ;                            // test si un cycle est complet
    virtual bool             Equal(RythmeBase& temp) = 0 ;
};

class RythmeSimple : public  RythmeBase
{
  protected:

    int _SelectedIndex ; // Index selection er dans la combo

  public:

    RythmeSimple(NSMedicCycleGlobal* temp) : RythmeBase(temp) {_SelectedIndex = 0; }

    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) { return false ; }
    void             save() ;
    NSPatPathoArray* SimplifiedTreeForInterpretation() ;            // cree l'arbre correspondant au rythme

    std::string      IsValid() ;
    bool             Equal(RythmeBase& temp) ;
};

class RythmeRegulier : public RythmeBase
{
  friend class RythmeRegulier ;

  protected:

    std::string _dureeCure ;    // Edit
    std::string _symDureeCure ; // Combo
    std::string _dureeCycle ;
    std::string _SymDureeCycle ;

  public:

    RythmeRegulier(NSMedicCycleGlobal* temp) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;

    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             save() ;
    NSPatPathoArray* SimplifiedTreeForInterpretation() ; // cree l'arbre correspondant au rythme

    std::string      IsValid() ;
    bool             Equal(RythmeBase& temp) ;
};

class RythmeFree : public RythmeBase
{
  protected:

		std::string _pDureeCureF ;
  	std::string _psymDureeCureF ;
  	std::string _pDureeCureFTime ;
  	std::string _pDureeCycleFreqF ;
  	std::string _psymDureeCycleFreqF ;

  public:

    RythmeFree(NSMedicCycleGlobal* temp) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             save() ;
    NSPatPathoArray* SimplifiedTreeForInterpretation() ; // cree l'arbre correspondant au rythme
    std::string      IsValid() ;
    bool             Equal(RythmeBase& temp) ;
};

class DayRythme : public RythmeBase
{
  public:

		DayRythme(NSMedicCycleGlobal* temp) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             save() ;
    NSPatPathoArray* SimplifiedTreeForInterpretation() ; // cree l'arbre correspondant au rythme
    std::string      IsValid() ;
    bool             Equal(RythmeBase& temp) ;

  protected:

    bool _week[7] ;
};

class Day1Day2Day3Rythme : public RythmeBase
{
  public:

    Day1Day2Day3Rythme(NSMedicCycleGlobal* temp) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             save() ;
    NSPatPathoArray* SimplifiedTreeForInterpretation() ; // cree l'arbre correspondant au rythme
    std::string      IsValid() ;
    bool             Equal(RythmeBase& temp) ;

    int              getIndice() { return iIndice ; }
    bool             getIsVoid() { return bVoid ; }

  protected:

    bool bVoid ;
    int  iIndice ;
};

class NSRythme
{
  protected:

    NSMedicCycleGlobal*  _parent ;
    RythmeType           _type ;
    RythmeBase*          _data ; // Information sur les different type

  public:

    NSRythme(NSMedicCycleGlobal* parent) ;

    static void       ReinitDialog(NSPosologieBlock* pPosoBlock) ;         // Reinitialise la fenetre

    NSPosologieBlock* GetWindow() ;
    void              Load(bool test_if_simple) ;
    bool              Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void              save() ;
    NSPatPathoArray*  SimplifiedTreeForInterpretation() ;
    std::string       Decode() ;   // Transforme les informations en langage naturelle
    inline RythmeType Type() { return _type ; } // Retourne le type de rythme

    bool              IsEqual(NSRythme& Rythme) ;
    std::string       IsValid() { return _data->IsValid() ; }
    void              load_cyle_func(bool cycle, int index) ;   // Tools function

    RythmeBase*       getData() { return _data ; }
};

class BaseCirc
{
  protected:

		NSMedicCycleGlobal*  _parent ;

  public:

    BaseCirc(NSMedicCycleGlobal* parent) : _parent(parent) {}

    virtual void             Load() = 0 ;
    virtual void             save() = 0 ;
    virtual bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) = 0 ;
    virtual NSPatPathoArray* CreateTree() = 0 ;

    virtual std::string      IsValid() = 0 ;
};

class CircBasePeriode : public BaseCirc
{
  protected:

    std::string _reveil ;
    std::string _matin ;
    std::string _midi ;
    std::string _gouter ;
    std::string _soir ;
    std::string _coucher ;
    std::string _nuit ;

  public:

    CircBasePeriode(NSMedicCycleGlobal* parent) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    void             Load() ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

class CircBaseMMS : public BaseCirc
{
	protected:

		std::string _matin ;
    std::string _midi ;
    std::string _soir ;
    std::string _coucher ;

  public:

		CircBaseMMS(NSMedicCycleGlobal* parent) ;

		static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
		bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
  	void             Load() ;
  	void             save() ;
  	NSPatPathoArray* CreateTree() ;
  	std::string      IsValid() ;

    std::string      getMatin()   { return _matin ; }
    std::string      getMidi()    { return _midi ; }
    std::string      getSoir()    { return _soir ; }
    std::string      getCoucher() { return _coucher ; }
};

class CircBaseHeures : public BaseCirc
{
  protected:

		std::vector<PriseHeure* >* _prises ;

	public:

  	CircBaseHeures(NSMedicCycleGlobal* parent) ;
    ~CircBaseHeures() ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             Load() ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

class  CircBaseRegular : public BaseCirc
{
  protected:

  	std::string _quantity ; // on le met en string car on peut avoir 1 / 2
    std::string _freq ;
    std::string _codeFreq ;

  public:

    CircBaseRegular(NSMedicCycleGlobal* parent) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             Load() ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

class CircBaseFree : public BaseCirc
{
  protected:

  	std::string _quantity ;
		std::string _fois ;
		std::string _frequence ;
		std::string _codeFreq ;

  public:

    CircBaseFree(NSMedicCycleGlobal* parent) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             Load() ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

class CircUnique : public BaseCirc
{
  protected:

		std::string _quantity ;

  public:

    CircUnique(NSMedicCycleGlobal* parent) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             Load() ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

class CircBaseText : public BaseCirc
{
	protected:

		std::string _text ;

  public:

    CircBaseText(NSMedicCycleGlobal*  parent) ;

    static void      ReinitDialog(NSPosologieBlock* pPosoBlock) ;
    bool             Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void             Load() ;
    void             save() ;
    NSPatPathoArray* CreateTree() ;
    std::string      IsValid() ;
};

/*
** Type de cyce cyrcadien prescrit
*/
enum CircadienCycle
{
  UndefinedCircadien,
  RegularCircadien, // Rythme journalier regulier
  FreeCircadien,    // Rythme regulier libre
  HourCircadien,    // Rythme par heure
  MMS,              // Matin Midi Soir
  Period,           // Par periode
  FreeTxt,          // Texte Libre
  PriseUnique       // Prise unique
};

class _NSBBCLASSE NSCircadien
{
  protected:

    NSMedicCycleGlobal* _parent ;
    BaseCirc*           _CircadienData ;
    CircadienCycle      _Type ;           // Type de cycle

    static long lObjectCount ;

  public:

    NSCircadien(NSMedicCycleGlobal* parent) ;
    NSCircadien(const NSCircadien& rv) ;
    ~NSCircadien() ;

		static void       ReinitDialog(NSPosologieBlock* pPosoBlock) ;

    NSPosologieBlock* GetWindow() ;
    NSPatPathoArray*  CreateTree() ;
    bool              RecupereCycleGlob(PatPathoIter& pptIter, PatPathoIter& pptend, int Col) ; // Recupere le cycle global
    CircadienCycle    FindWichPanelToOpen(PatPathoIter& pptIter, PatPathoIter& pptend, int Col) ;
    void              Load() ;
    bool              Load(PatPathoIter& pptIter, PatPathoIter& pptend) ;
    void              save() ;

    std::string       IsValid() { return _CircadienData->IsValid() ; }
    CircadienCycle    getType() { return _Type ; }
    BaseCirc*         getData() { return _CircadienData ; }

    NSCircadien&      operator=(const NSCircadien& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSphaseMedic ;
/*
** Un NSMedicCycleGlobal est un cycle circadien avec un rythme
*/
class _NSBBCLASSE NSMedicCycleGlobal : public NSRoot
{
  protected:

  	NSphaseMedic     *_parentPhase ;
    NSPosologieBlock *_pControlsBlock ;

    NSCircadien      _cycleCircadien ;      //  Cycle circadien
    NSRythme         _cycleRythme ;         //  Rythme

    static long lObjectCount ;

  public:

  	NSMedicCycleGlobal(NSContexte* pCtx, NSphaseMedic* parentPhase) ;
    NSMedicCycleGlobal(NSContexte* pCtx, NSPosologieBlock* pCtrlsBlock) ;
    NSMedicCycleGlobal(const NSMedicCycleGlobal& rv) ;
    ~NSMedicCycleGlobal() ;

		NSCircadien*     GetCycleCircadien() { return &_cycleCircadien ; }   // Retourne le cycle circadien
		NSRythme*        GetRythme()         { return &_cycleRythme ; }             // Retourn le rythme

    NSPatPathoArray*  CreateTree() ;  // Cree l'arbre
    NSPosologieBlock* GetWindow() ;   // Retourne la fenetre des médicaments

    void              save() ;                                            // Charge un arbre
    void              Load(bool test_if_simple) ;                         // Charge la fenetre avec les donnée
    bool              Load(PatPathoIter& pptIter, PatPathoIter& pptend) ; // Charge un NSMEdicCycle depuis une pathpatho

    static void       ReinitDialog(NSPosologieBlock* pPosoBlock) ;

    NSMedicCycleGlobal& operator=(const NSMedicCycleGlobal& src) ;

    std::string       IsValid() ;
    // NSContexte*      getContext() ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif // __NSMEDICCYCLE_H__

