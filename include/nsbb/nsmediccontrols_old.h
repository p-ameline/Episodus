// -----------------------------------------------------------------------------
// nsmediccontrols.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------

#ifndef __NSMEDICCONTROLS_H__
# define __NSMEDICCONTROLS_H__

# include <owl\edit.h>
# include <owl\validate.h>
# include <owl\updown.h>
# include <owl\combobox.h>
# include <owl\checkbox.h>
# include "nsbb\nsednum.h"
# include "nsbb\nsedit.h"
# include "nsbb\nspatpat.h"
# include  "nsbb\nscomboutil.h"
# include "nsbb\NTListBox.h"

class NSUpDownEdit ;
class NSUpDownHeureEdit ;

class NSUpDown : public OWL::TUpDown
{
 protected:
  Functor* _LostFocusResponse;

 public:
  // constructors/destructor
  NSUpDown(TWindow *windowParent, int resId, NSUpDownEdit *parent) ;
  NSUpDown(TWindow *windowParent, int Id, int x, int y, int w, int h, NSUpDownEdit *parent) ;
  ~NSUpDown() ;

  // functions
  void EvLButtonDown(uint modKeys, ClassLib::TPoint& point) ;
  void SetupWindow() ;
  bool isVisible() ;
  void SetLostFocusResponse(Functor* func) { _LostFocusResponse = func ; }
  void EvKillFocus(HWND hWndGetFocus) ;

 protected:

  // variables
  NSUpDownEdit *pFather ;

  // table de réponses
  DECLARE_RESPONSE_TABLE(NSUpDown) ;
} ;

class _NSBBCLASSE NSEditNumSimpl : public OWL::TEdit
{
 protected :
	Functor* LostFocusResponse ;

 public:
	// constructors/destructor
  NSEditNumSimpl(TWindow *windowParent, int resId, NSUpDownEdit *parent, int iTextLen = 255, string sValidator = "0-9.,/*") ;
  NSEditNumSimpl(TWindow *windowParent, int Id, const char far *text, int x, int y, int w, int h, NSUpDownEdit *parent, int iTextLen = 255, string sValidator = "0-9.,/*") ;
  ~NSEditNumSimpl() ;

  // functions
  void    incremente() ;
  void    decremente() ;
  void    incrementeDiviseur() ;
  void    decrementeDiviseur() ;
  string  getText() ;
  void    setText(string sTxt) ;
  int     getVal() ;
  void    setVal(int iValue) ;
  void    EvKeyUp(uint key, uint repeatcount, uint flags) ;
  void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
  void    EvChar(uint key, uint repeatCount, uint flags) ;
  uint    EvGetDlgCode(MSG far* msg) ;
  void    SetupWindow() ;
  bool    isVisible() ;
  void    SetLostFocusResponse(Functor* temp) { LostFocusResponse = temp; } // Associe une fonction de reponse a la perte du focus
  void    getLeftAndRightFromSlash(string* psText, string* psLeft, string* psRight) ;
  string  getEditContent() ;

  void    setMinValue(int iMinVal) { iMinValue = iMinVal ; }
  void    setMaxValue(int iMaxVal) { iMaxValue = iMaxVal ; }

 protected:
	// variables
  int               iMaxValue ;
  int               iMinValue ;
  bool              bAllowDivider ;

  TFilterValidator* pValidator ;
  NSUpDownEdit*     pFather ;
  TWindow*          pDlgParent ;
  bool              bIntercepte ;
  bool              bWinStd ;

  void              initCommonData() ;
  void              EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSEditNumSimpl) ;
} ;

class _NSBBCLASSE NSUpDownEdit /*: public NSMedicBaseControl    */
{
 public:

 	NSContexte*    pContexte ;

  // constructors/destructor  int Id,
 	NSUpDownEdit(TWindow *windowParent, NSContexte *pCtx, string sRacine, int resEditId, int resUpDownId, /*int resStaticTextId = -1,*/ int iTextLen = 255, string sValidator = "0-9.,/*") ;
  NSUpDownEdit(TWindow *windowParent, NSContexte *pCtx, string sRacine, int EditId, int UpDownId, const char far *text, int x, int y, int w, int h, int iTextLen = 255, string sValidator = "0-9.,/*") ;

	~NSUpDownEdit() ;

 	// functions
  void           setEditNum(NSEditNumSimpl *pEditNum) { pEditNumControl = pEditNum ; }
  void           setUpDown(NSUpDown *pUpDown)         { pUpDownControl = pUpDown ; }
  OWL::TEdit*    GetEdit() ;
  OWL::TUpDown*  GetUpdown() ;

  NSEditNumSimpl *getEditNum()        { return pEditNumControl ; }
  NSUpDown       *getUpDown()         { return pUpDownControl ; }

  int            getValue()           { return pEditNumControl->getVal() ; }
  void           setValue(int iValue) { pEditNumControl->setVal(iValue) ; }

  string         getText()            { return pEditNumControl->getText() ; }
  void           setText(string sTxt) { pEditNumControl->setText(sTxt) ; }

  void           setInit(string sTxt) { sInitialValue = sTxt ; }
  void           setupWindow() ;

  void           Show(int cmdShow) ;
  bool           isVisible() ;
  bool           MoveWindow(int x, int y, int w, int h, bool repaint = false) ;

  string         getNameType()        { return "UpDownEdit" ; }
  std::string    getRoot()            { return sRoot ; }
  std::string    getRootSens() ;

  void           createTree(NSPatPathoArray *pPPT, int colone) ;
  void           createTree(NSPatPathoArray *pPPT, float fVal) ;

  void           initControle(int iVal, string sCode = "")
                                           { pEditNumControl->setVal(iVal) ; }
  void           reinitControle()     { pEditNumControl->setVal(0) ; }

  void           setMinMaxValue(int iMinVal, int iMaxVal = -1) ;

  // variables
 protected:

  NSUpDown       *pUpDownControl ;
  NSEditNumSimpl *pEditNumControl ;

  std::string    sRoot ;
  std::string    sInitialValue ;
} ;

typedef vector<NSUpDownEdit*>        NSUpDownEditVector ;
typedef NSUpDownEditVector::iterator NSUpDownEditIter ;typedef NTArray<NSUpDownEdit>        NSUpDownEditArray ;

//------------------------------------------------------------------------------
//               classes pour les controles edit heure + UpDown
//------------------------------------------------------------------------------

class NSUpDownHeureEdit;

class NSUpDownHeure : public OWL::TUpDown
{
 public:

  // constructors/destructor
  NSUpDownHeure(TWindow *windowParent, int resId, NSUpDownHeureEdit *parent) ;
  NSUpDownHeure(TWindow *windowParent, int Id, int x, int y, int w, int h, NSUpDownHeureEdit *parent) ;
  ~NSUpDownHeure() ;

  void EvLButtonDown(uint modKeys, ClassLib::TPoint& point) ;
  void SetupWindow() ;
  bool isVisible() ;

 protected:

  NSUpDownHeureEdit *pFather ;

  DECLARE_RESPONSE_TABLE(NSUpDownHeure) ;
} ;

class NSEditHeureMedic : public NSEditHeure
{
 public:

  NSEditHeureMedic(TWindow *windowParent, int resId, NSUpDownHeureEdit *parent, string sUserLang) ;
  NSEditHeureMedic(TWindow *windowParent, int Id, const char far *text, int x, int y, int w, int h, NSUpDownHeureEdit *parent, string sUserLang) ;
  ~NSEditHeureMedic() ;

  void                incrementeHeure() ;
  void                decrementeHeure() ;
  string              getVal() ;
  void                setVal(string sValue) ;
  void                EvKeyUp(uint key, uint repeatcount, uint flags) ;
  void                EvKeyDown(uint key, uint repeatCount, uint flags) ;
  uint                EvGetDlgCode(MSG far* msg);
  void                SetupWindow();
  bool                isVisible();

 protected:

  TFilterValidator    *pValidator ;
  NSUpDownHeureEdit   *pFather ;
  TWindow             *pDlgParent ;
  bool                bIntercepte;
  bool                bWinStd ;

  void                initCommonData() ;

  DECLARE_RESPONSE_TABLE(NSEditHeureMedic) ;
};

class NSUpDownHeureEdit
{
 public:
  // constructors/destructor
    NSUpDownHeureEdit(TWindow *windowParent, int resEditId, int resUpDownId, string sUserLang) ;
    NSUpDownHeureEdit(TWindow *windowParent, int resEditId, int resUpDownId, const char far *text, int x, int y, int w, int h, string sUserLang) ;

    ~NSUpDownHeureEdit() ;

    void                setEditHeure(NSEditHeureMedic *pEdit)   {pEditDateControl = pEdit ;}
    void                setUpHeure(NSUpDownHeure *pUpDown)      {pUpDownControl = pUpDown ;}
    NSEditHeureMedic    *getEditHeure()                  { return pEditDateControl ; }
    NSUpDownHeure       *getUpDown()                    { return pUpDownControl ; }

    string              getVal()                        { return pEditDateControl->getVal() ; }
    void                setVal(string sValue)           { pEditDateControl->setVal(sValue) ; }
    void                Show(int cmdShow);
    bool                isVisible();
    void                reinitControle()                { pEditDateControl->setVal("00:00");}

  // variables
 protected:

	string sLang ;       

	NSUpDownHeure       *pUpDownControl ;
	NSEditHeureMedic    *pEditDateControl ;

} ;

class PriseHeure
{
  protected:
    std::string _quantity; // Quantité de médicament
    std::string _heure;    // Heure
  public:
    PriseHeure() {}
    PriseHeure(std::string quantity, std::string heure) : _quantity(quantity), _heure(heure) {}
    void        setHeure(std::string heure)   {  _heure = heure; }
    void        setQuantity(std::string quant){ _quantity = quant;}
    std::string getHeure()    { return _heure; }
    std::string getQuantity() { return _quantity; }
    NSPatPathoArray* CreateTree(NSContexte* cont);

    bool operator ==(PriseHeure& temp)
    {
      return (_heure == temp.getHeure() );
    }
};



//la classe pour les controles composés de : l'edit+upDown pour unités et l'edit+upDown pour l'heure
class NSUpDownEditHeureControle ///: public NSMedicBaseControl
{
 public:

    NSUpDownEditHeureControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int resEditId, int resUpDownId /*,int resStaticTextId = -1*/, int resHeureEdit = -1,
                            int resHeureUpDown = -1, int iTextLen = 255, string sValidator = "0-9.,/*") ;
    NSUpDownEditHeureControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int EditId, int UpDownId, const char far *text, int x, int y, int w, int h, int HeureEditId = -1,
                            int HeureUpDownId = -1, const char far *textH = 0, int xH = 0, int yH = 0, int wH = 0, int hH = 0, int iTextLen = 255, string sValidator = "0-9.,/*") ;

    ~NSUpDownEditHeureControle() ;

    NSUpDownHeureEdit   *getEditHeureControle()                           {return pEditHeure ;}
    void                setEditHeureControle(NSUpDownHeureEdit *pCtrl)    {pEditHeure =  pCtrl ;}

    void                setEditNum(NSUpDownEdit *pEdit)       {pEditNum = pEdit ;}
    NSUpDownEdit        *getEditNum()                         {return pEditNum ; }


    std::string              getHeure()                    {return pEditHeure->getVal() ; }
    void                setHeure(string sValue)       {pEditHeure->getEditHeure()->setVal(sValue) ; }

    std::string          getValue()                    {return pEditNum->getEditNum()->getText() ; }
    void                setValue(int iValue)           {pEditNum->setValue(iValue) ; }

    void                Show(int cmdShow);
    bool                isVisible();

    void                createTree(NSPatPathoArray *pPPT, string sHeure, float fVal) ;

    void                CreateTree(NSPatPathoArray *pPPT);

    void                createTree(NSPatPathoArray *pPPT, string sHeure) ;

    void                initControle(std::string iQuant, std::string sHeure )
    {
        pEditNum->getEditNum()->SetText(iQuant.c_str());
        pEditHeure->getEditHeure()->setVal(sHeure) ;
    }

    void                initFromPriseHeure(PriseHeure* prise)
    {
        pEditNum->getEditNum()->SetText(prise->getQuantity().c_str());
        pEditHeure->getEditHeure()->setVal(prise->getHeure()) ;
    }

    void reinitControle() {
        pEditNum->getEditNum()->SetText("0");
        pEditHeure->getEditHeure()->setVal("0000000") ;
    }


  // variables
 protected:

    NSUpDownHeureEdit *pEditHeure;
    NSUpDownEdit      *pEditNum;
    std::string       sRoot;

} ;


//------------------------------------------------------------------------------
//              classe NSUpDownEditLexiqueControl
//------------------------------------------------------------------------------
class NSUpDownEditLexiqueControl
{
	public:
  // constructors/destructor
    NSUpDownEditLexiqueControl( TWindow *windowParent, NSContexte *pCtx, string sRacine,
                                int resEditId, int resUpDownId, int resStaticTextId = -1,
                                int resEditLexiquId = -1 , int iTextLen = 255, string sValidator = "0-9.,/*") ;
    NSUpDownEditLexiqueControl( TWindow *windowParent, NSContexte *pCtx, string sRacine,
                                int EditId, int UpDownId, const char far *text, int x, int y, int w, int h,
                                int StaticTextId = -1, int EditLexiquId = -1, int iTextLen = 255, string sValidator = "0-9.,/*") ;

    ~NSUpDownEditLexiqueControl() ;

    NSUtilLexique       *getEditUtilLexique()                       {return pEditLexique ;}
    void                setEditUtilLexique(NSUtilLexique *pCtrl)    {pEditLexique =  pCtrl ;}
    void                setEditNum(NSUpDownEdit *pEdit)       {pEditNum = pEdit ;}
    NSUpDownEdit        *getEditNum()                           {return pEditNum ; }


    void                Show(int cmdShow);
    bool                isVisible();

    string              getNameType()                  {return "UpDownEditUtilLexique" ; }

     int                getValue()                    {return pEditNum->getValue() ; }
    void                setValue(int iValue)           {pEditNum->setValue(iValue) ; }

    string              getCode()                    {return pEditLexique->getCode() ; }
    void                setCode(string sValue)       {pEditLexique->setLabel(sValue) ; }


    void                createTree(NSPatPathoArray *pPPT, string sCode, float fVal) ;

    void                initControle(int iQuant, string sCode)  {   pEditNum->setValue(iQuant) ;
                                                                    pEditLexique->setLabel(sCode); }

    void                reinitControle()    {   pEditLexique->setLabel("");
                                                pEditNum->reinitControle();  }

 protected:

    NSUtilLexique       *pEditLexique;
    NSUpDownEdit        *pEditNum;
    std::string              sRoot;
};

//------------------------------------------------------------------------------
//                          classe NSTexteLibre
//------------------------------------------------------------------------------

class NSTexteLibre  : public OWL::TEdit
{
 public:

  NSTexteLibre(TWindow *windowParent, NSContexte *pCtx, string sRacine = "£C;020", int resEditId = -1) ;
  NSTexteLibre(TWindow *windowParent, NSContexte *pCtx, const char far *text, int x, int y, int w, int h, string sRacine = "£C;020", int resEditId = -1) ;
  ~NSTexteLibre() ;

  std::string getTexte() ;
  void        setText(string sValue) { SetText(sValue.c_str()) ; }

 protected:

  std::string sRoot ;
} ;


//------------------------------------------------------------------------------
//              classe NSRythmesRegIregControle
//------------------------------------------------------------------------------
/*
class NSRythmesRegIregControle : public NSMedicBaseControl
{
public:
  // constructors/destructor
    NSRythmesRegIregControle(TWindow *windowParent, NSContexte *pCtx, string sRacine,
                         int resEditTimeId, int resUpDownTimeId,
                         int resComboTimeId, VecteurString *pVecteurStringTime,
                         int resEditFoisId, int resUpDownFoisId,
                         int resEditPeriodId =-1, int resUpDownPeriodId = -1,
                         int resComboPeriodId = -1, VecteurString *pVecteurStringPeriod = NULL,
                         int iTextLen = 255, string sValidator = "0-9") ;

    ~NSRythmesRegIregControle() ;


    NSComboBox          *getTimeUnits()                             {return pTimeUnits ;}
    void                setTimeUnits(NSComboBox *pCtrl)             {pTimeUnits =  pCtrl ;}

    void                setEditNoOfTimes(NSUpDownEdit *pEdit)       {pEditNoOfTimes = pEdit ;}
    NSUpDownEdit        *getEditNoOfTimes()                         {return pEditNoOfTimes ; }

    void                setEditNoFois(NSUpDownEdit *pEdit)          {pEditNoFois = pEdit ;}
    NSUpDownEdit        *getEditNoFois()                            {return pEditNoFois ; }

    void                setEditNoOfPeriods(NSUpDownEdit *pEdit)     {pEditNoOfPeriods = pEdit ;}
    NSUpDownEdit        *getEditNoOfPeriods()                       {return pEditNoOfPeriods ; }

    void                setPeriodUnits(NSComboBox *pEdit)           {pPeriodUnits = pEdit ;}
    NSComboBox          *getPeriodUnits()                           {return pPeriodUnits ; }


  //  string              getSelCode()                            {return pCombo->getSelCode() ; }
  //  void                setCode(string sValue)                  {pCombo->setCode(sValue) ; }

    int                 getValue()                               { return 1;}
    void                setValue(int iValue)                     { }

    void                Show(int cmdShow);
    bool                isVisible();

    string              getNameType()                             {return "RythmesRegIregControle" ; }

    void                createTree(NSPatPathoArray *pPPT, int iNoTime, string sTimeUnit, int iFois, int iNoPeriods, string sPeriodUnit) ;
    void                initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue) ;

    void                reinitControle() ;
    void                initControleVal(ValueControle *pVal) ;


    bool                isValidData();

 private:

    NSUpDownEdit        *pEditNoOfTimes;
    NSComboBox          *pTimeUnits;
    NSUpDownEdit        *pEditNoFois;
    NSUpDownEdit        *pEditNoOfPeriods;
    NSComboBox          *pPeriodUnits ;
    TStatic             *pStaticTextSupl;
} ;       */


//------------------------------------------------------------------------------
//              classe NSCheckBoxControle
//------------------------------------------------------------------------------
class NSCheckBoxControle : public OWL::TCheckBox//,  public NSMedicBaseControl
{
 public:

	NSCheckBoxControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int resId) ;
  NSCheckBoxControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int Id, const char far* title, int x, int y, int w, int h) ;
 	~NSCheckBoxControle() ;

  void setChangeResponse(Functor* temp)  { LostChangeReponse = temp ; }

  std::string getRoot()                  { return sRoot ; }
  std::string getRootSens() ;

  int         getValue()                 { return GetCheck() ; }
  void        setValue(int iValue)       { SetCheck(iValue) ; }

  int         getActiveStatus()          { return iInitialStatus ; }
  void        setActiveStatus(int iAct)  { iInitialStatus = iAct ; }

  void        Show(int cmdShow) ;
  bool        isVisible() ;
  bool        MoveWindow(int x, int y, int w, int h, bool repaint = false) ;

  string      getNameType()    { return "CheckBoxControle" ; }

  void        createTree(NSPatPathoArray *pPPT, uint iVal) ;

  void        setupWindow()    { SetCheck(iInitialStatus) ; }
  void        reinitControle() { SetCheck(BF_UNCHECKED) ; }

  void        BNClicked() ;

	void        EvKillFocus(HWND hWndGetFocus) ; // Permet de gerer la perte de focus

 protected:

	Functor*    LostChangeReponse ;

 	std::string sRoot ;
  NSContexte* pContexte ;

  int         iInitialStatus ;

	DECLARE_RESPONSE_TABLE(NSCheckBoxControle) ;
} ;

//------------------------------------------------------------------------------
//              classe NSRadioButtonControle
//------------------------------------------------------------------------------
class NSRadioButtonControle : public OWL::TRadioButton //,  public NSMedicBaseControl
{
 public:

	NSRadioButtonControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int resId) ;
  NSRadioButtonControle(TWindow *windowParent, NSContexte *pCtx, string sRacine, int Id, const char far* title, int x, int y, int w, int h) ;
 	~NSRadioButtonControle() ;

  void setChangeResponse(Functor* temp) { LostChangeReponse = temp ; }

  std::string getRoot()                 { return sRoot ; }
  std::string getRootSens() ;

  int         getValue()                 { return GetCheck() ; }
  void        setValue(int iValue)       { SetCheck(iValue) ; }

  int         getActiveStatus()          { return iInitialStatus ; }
  void        setActiveStatus(int iAct)  { iInitialStatus = iAct ; }

  void        Show(int cmdShow) ;
  bool        isVisible() ;
  bool        MoveWindow(int x, int y, int w, int h, bool repaint = false) ;

  string      getNameType()    { return "RadioButtonControle" ; }

  void        createTree(NSPatPathoArray *pPPT, uint iVal) ;

  void        setupWindow()    { SetCheck(iInitialStatus) ; }
  void        reinitControle() { SetCheck(BF_UNCHECKED) ; }

  void        BNClicked() ;

	void        EvKillFocus(HWND hWndGetFocus) ; // Permet de gerer la perte de focus

 protected:

 	Functor*    LostChangeReponse ;

 	std::string sRoot ;
  NSContexte* pContexte ;

  int         iInitialStatus ;

	DECLARE_RESPONSE_TABLE(NSRadioButtonControle) ;
} ;


//------------------------------------------------------------------------------
//                          classe NSRythmesRegIregCycle
//------------------------------------------------------------------------------
          /*
class NSRythmesRegIregCycle : public NSMedicBaseControl
{
public:
  // constructors/destructor
    NSRythmesRegIregCycle(TWindow *windowParent, NSContexte *pCtx, string sRacine, string sPos,
                         int resEditTimeId, int resUpDownTimeId,
                         int resComboTimeId, VecteurString *pVecteurStringTime,
                         int resEditFoisId, int resUpDownFoisId,
                         /*int resEditPeriodId =-1, int resUpDownPeriodId = -1,
                         int resEditNoId = -1, int resUpDownNoId = -1,
                         int iTextLen = 255, string sValidator = "0-9") ;

    ~NSRythmesRegIregCycle() ;

  // functions
    NSComboBox          *getTimeUnits()                             {return pTimeUnits ;}
    void                setTimeUnits(NSComboBox *pCtrl)             {pTimeUnits =  pCtrl ;}

    void                setEditNoOfTimes(NSUpDownEdit *pEdit)       {pEditNoOfTimes = pEdit ;}
    NSUpDownEdit        *getEditNoOfTimes()                         {return pEditNoOfTimes ; }

    void                setEditNoMed(NSUpDownEdit *pEdit)          {pEditNoUnitsMed = pEdit ;}
    NSUpDownEdit        *getEditNoMed()                            {return pEditNoUnitsMed ; }

    void                setEditNoOfPeriods(NSUpDownEdit *pEdit)     {pEditNoOfPeriods = pEdit ;}
    NSUpDownEdit        *getEditNoOfPeriods()                       {return pEditNoOfPeriods ; }

    void                setPeriodUnits(NSComboBox *pEdit)           {pPeriodUnits = pEdit ;}
    NSComboBox          *getPeriodUnits()                           {return pPeriodUnits ; }

    void                setEditNoFois(NSUpDownEdit *pEdit)          {pEditNoFois = pEdit ;}
    NSUpDownEdit        *getEditNoFois()                         {return pEditNoFois ; }


  //  string              getSelCode()                            {return pCombo->getSelCode() ; }
  //  void                setCode(string sValue)                  {pCombo->setCode(sValue) ; }

    int                 getValue()                               { return pEditNoUnitsMed->getValue();}
    void                setValue(int iValue)                     { pEditNoUnitsMed->setValue(iValue) ;}

    void                Show(int cmdShow);
    bool                isVisible();
   // bool                MoveWindow(int x, int y, int w, int h, bool repaint = false);

    string              getNameType()                             {return "RythmesRegIregCycle" ; }

    void                createTree(NSPatPathoArray *pPPT) ;
    void                createTree(NSPatPathoArray *pPPT, int iNoTime, string sTimeUnit, int iFois, int iNoPeriods, string sPeriodUnit, float fVal) ;
    void                initFromTree(NSPatPathoArray *pPPT, PatPathoIter *pptIter, string sRoot, ValueControle *pReturnValue) ;

    void                reinitControle() ;
    void                initControleVal(ValueControle *pVal) ;

    string              getPosition()                               { return sPosition ; }
    void                setPosition(string sValue)                  {sPosition = sValue ; }

    bool                isValidData();

  // variables
 private:
    string              sPosition;
    NSUpDownEdit        *pEditNoOfTimes;
    NSComboBox          *pTimeUnits;
    NSUpDownEdit        *pEditNoFois;
    NSUpDownEdit        *pEditNoOfPeriods;
    NSUpDownEdit        *pEditNoUnitsMed;
    NSComboBox          *pPeriodUnits ;
    TStatic             *pStaticTextSupl;
} ;       */

#endif // __NSMEDICCONTROLS_H__

