#ifndef __NSEDNUM_H#define __NSEDNUM_H

class NSUtilDialog ;

// #include <owl\owlpch.h>// #include <owl\applicat.h>
#include <owl\edit.h>
#include <owl\updown.h>
#include <owl\validate.h>
// #include <string.h>
// #include <cstring.h>

// #include "nsbb\nsbb_dlg.h"
// #include "nsbb\nsbbtran.h"
// #include "nsbb\nsutiDlg.h"
#include "nsbb\nsdicogl.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

#include "nsbb\nsbb_glo.h"
#include "nsbb\nsExport.h"
#include "nsbb\NTFunctor.h"

class _NSBBCLASSE NSUtilEdit : public OWL::TEdit, public NSRoot
{
	protected:

		Functor* _LostFocusFunctor ; // Fonction se declenchant lors de la perte du focus

	public:

		NSUtilDialog* _pNSUtilDialog ;
		bool          _bIntercepte ;

		bool          _bWinStd ;       // Gestion standard windows
		// Constructeur et destructeur		NSUtilEdit(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1, bool bReturn = true, OWL::TModule* module = 0) ;
    NSUtilEdit(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int Id, const char far* text, int x, int y, int w, int h, int iTextLen = -1, bool multiline = false, bool bReturn = true, OWL::TModule* module = 0) ;
		~NSUtilEdit() ;

    void initCommonData() ;
		void SetupWindow() ;

		//fonctions permettant � la touche ENTREE le m�me effet que TAB
		void EvKeyUp(uint key, uint repeatcount, uint flags) ;
		uint EvGetDlgCode(MSG far* ) ;
		void EvChar(uint key, uint repeatCount, uint flags) ;
    void EvKillFocus(HWND hWndGetFocus) ;

    void setLostFocusFunctor(Functor* func) { _LostFocusFunctor = func ; }
    void fireLostFocusFunctor() ;

    string GetText(size_t iSizeLimit = 0) ;
    void   SetText(const string sContent) ;

	//
	// child id notification handled at the child
	//
	DECLARE_RESPONSE_TABLE(NSUtilEdit) ;
};

// classe NSUtilEdit2 pour simplifier les champs de saisie dans les dialogues
/////////////////////////////////////////////////////////////////////////////
class _NSBBCLASSE NSUtilEdit2 : public NSUtilEdit
{
  public:

    string _sTexte ;
    int    _ciTexteLen ;

    // Constructeur et destructeur
    NSUtilEdit2(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1) ;
    ~NSUtilEdit2() ;

    void SetupWindow();
    void GetText(string&);
    //
    // child id notification handled at the child
    //
  DECLARE_RESPONSE_TABLE(NSUtilEdit2) ;
};

// classe NSMultiEdit pour pouvoir saisir dans les Edit multiples
// et revenir � la ligne avec Return sans activer le controle suivant
/////////////////////////////////////////////////////////////////////////////
class _NSBBCLASSE NSMultiEdit : public NSUtilEdit2
{
  public:

    // Constructeur et destructeur    NSMultiEdit(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1) ;
    ~NSMultiEdit() ;

    void SetupWindow() ;
    void EvKeyUp(uint key, uint repeatcount, uint flags) ;
    void EvChar(uint key, uint repeatCount, uint flags) ;

  DECLARE_RESPONSE_TABLE(NSMultiEdit) ;
};

class _NSBBCLASSE NSFilterValidator : public TFilterValidator, public NSRoot
{
  public:

    string _sValidator ;

    NSFilterValidator(NSContexte* pCtx, string sCharSet) ;
    ~NSFilterValidator() ;

    void Error(TWindow* owner) ;
};

class _NSBBCLASSE NSEditNum : public NSUtilEdit
{
	public:

		UINT   _iMaxInput, _iDecimale ;
		string _sContenuBrut, _sContenuTransfert ;
		double _dValeur ;
		NSFilterValidator* _pFilterValidator ;

		// Constructeur et destructeur
		NSEditNum(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId,
              int iTextLen = 255, int iDecimales = 0, string sValidator = "0-9.,") ;
		~NSEditNum() ;

		void donneBrut() ;        // Passe des donn�es saisies aux donn�es
		void donneTransfert() ;   // codables par NAUTILUS et vice versa

		void donneValeur() ;
		void MettreAJourValeur(string sContenu) ;

    void setNum(string sNum) ;
    void setNum(double dNum) ;

    double getValue() { return _dValeur ; }

		// child id notification handled at the child
		//
		void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSEditNum) ;
};

// pour les num�ros de t�l�phone
class _NSBBCLASSE NSEditNumTel : public NSEditNum
{
	public:

    NSEditNumTel(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = 255) ;
    ~NSEditNumTel() ;
};

#ifndef __DATE_SEPARATION
#define __DATE_SEPARATION
const char dateSeparationMARK = '/';
#endif

class _NSBBCLASSE NSUtilEditDate : public NSUtilEdit
{
	public:

		string _sContenuBrut, _sContenuTransfert ;
		string _sMask ;
    string _sFormat ;
    string _sLang ;

		// Constructeur et destructeur
		NSUtilEditDate(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, UINT iTextLen = 10, bool b2000 = true, string sLang = string("")) ;
    NSUtilEditDate(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int Id, int x, int y, int w, int h, int iTextLen = 10, bool b2000 = true, string sLang = string("")) ;
		~NSUtilEditDate() ;

		void SetupWindow() ;

    void setDate(string sAAAAMMJJ) ;
    void getDate(string* pAAAAMMJJ) ;

    void donneBrut() ;        // Passe des donn�es saisies aux donn�es
    void donneTransfert() ;   // codables par NAUTILUS et vice versa

    void EvChar(uint key, uint repeatCount, uint flags) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;

    void initialise(bool b2000) ;

	DECLARE_RESPONSE_TABLE(NSUtilEditDate) ;
};

#ifndef __HEURE_SEPARATION
#define __HEURE_SEPARATION
const char heureSeparationMARK = ':';
#endif

class _NSBBCLASSE NSUtilEditHeure : public NSUtilEdit
{
	public:

		string _sContenuBrut, _sContenuTransfert ;
    string _sFormat ;
    string _sLang ;

		// Constructeur et destructeur
		NSUtilEditHeure(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, UINT iTextLen = 5, string sLang = string("")) ;
    NSUtilEditHeure(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int Id, int x, int y, int w, int h, int iTextLen = 5, string sLang = string("")) ;
		~NSUtilEditHeure() ;

		void SetupWindow() ;

		void setHeure(string sHHMM) ;
		void getHeure(string* pHHMM) ;

		void donneBrut() ;        // Passe des donn�es saisies aux donn�es
		void donneTransfert() ;   // codables par NAUTILUS et vice versa

		void EvChar(uint key, uint repeatCount, uint flags) ;
		void EvKeyDown(uint key, uint repeatCount, uint flags) ;

    void initialise() ;

	DECLARE_RESPONSE_TABLE(NSUtilEditHeure) ;
};

class _NSBBCLASSE NSUtilEditDateHeure : public NSUtilEdit
{
	public:

		string _sContenuBrut, _sContenuTransfert ;
		string _sMask ;
    string _sFormat ;
    string _sLang ;

		// Constructeur et destructeur
		NSUtilEditDateHeure(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, UINT iTextLen = 19, bool b2000 = true, string sLang = string("")) ;
    NSUtilEditDateHeure(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int Id, const char far* text, int x, int y, int w, int h, int iTextLen = 19, bool b2000 = true, string sLang = string("")) ;
		~NSUtilEditDateHeure() ;

    void initCommonData(bool b2000) ;
		void SetupWindow() ;

		void setDate(string sAAAAMMJJ) ;
		void getDate(string* pAAAAMMJJ) ;

		void donneBrut() ;        // Passe des donn�es saisies aux donn�es
		void donneTransfert() ;   // codables par NAUTILUS et vice versa

		void EvChar(uint key, uint repeatCount, uint flags) ;
		void EvKeyDown(uint key, uint repeatCount, uint flags) ;

    bool isFormatChar(char c) ;

	DECLARE_RESPONSE_TABLE(NSUtilEditDateHeure) ;
};

#ifndef __SOMME_SEPARATION
#define __SOMME_SEPARATION
const char sommeSeparationMARK = ',';
#endif

class _NSBBCLASSE NSUtilEditSomme : public NSUtilEdit
{
	public:

		string _sContenuBrut, _sContenuTransfert ;
		string _sZero ;
		int    _textLen ;
    string _sLang ;

		// Constructeur et destructeur
		NSUtilEditSomme(NSContexte* pCtx, NSUtilDialog* pNSUtilDialog, int resId, UINT iTextLen = 8, string sLang = string("")) ;
		~NSUtilEditSomme() ;

		void SetupWindow() ;

		void setSomme(string sSomme) ;
		void getSomme(string* pSomme) ;

		void donneBrut() ;        // Passe des donn�es saisies aux donn�es
		void donneTransfert() ;   // codables par NAUTILUS et vice versa

		void EvChar(uint key, uint repeatCount, uint flags) ;
		void EvKeyDown(uint key, uint repeatCount, uint flags) ;

		bool estValide(string sTest) ;

	DECLARE_RESPONSE_TABLE(NSUtilEditSomme) ;
};

//-----------------------------------------------------------------------
// acc�s g�n�rique au lexique sans avoir besoin d'un pControle
//-----------------------------------------------------------------------
class _NSBBCLASSE NSUtilLexique : public NSEditDicoGlobal
{
	protected:

		Functor* _LostFocusFunctor ; // Fonction se declenchant lors de la perte du focus

	public:

		string        _sContenuTransfert ; //label dans le champ edit
    string        _sCode ;             //code lexique
    NSUtilDialog* _pNSUtilDialog ;
		//
    // Constructeur et destructeur
    //
    NSUtilLexique(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio,
                      uint textLimit = 0, OWL::TModule* module = (OWL::TModule*) 0) ;

    NSUtilLexique(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resourceId, NSDico* pDictio,
                      uint textLimit = 0, OWL::TModule* module = (OWL::TModule*) 0) ;

    NSUtilLexique(NSContexte* pCtx, TWindow* parent, int resourceId, NSDico* pDictio,
                      const char far* text,
                      int x, int y, int w, int h,
                      uint     textLimit = 0,
                      bool     multiline = false,
                      OWL::TModule* module = (OWL::TModule*) 0) ;

		NSUtilLexique(NSContexte* pCtx, NSUtilDialog* pUtilDialog, int resourceId, NSDico* pDictio,
                      const char far* text,
                      int x, int y, int w, int h,
                      uint     textLimit = 0,
                      bool     multiline = false,
                      OWL::TModule* module = (OWL::TModule*) 0) ;

		~NSUtilLexique() ;

		void setLostFocusFunctor(Functor* func) { _LostFocusFunctor = func ; }

		//
		// m�thodes
		//
    string getCode()          { return _sCode ; }
    void   setCode(string sC) { _sCode = sC ; }
    string getCodeSens() ;

    string getLabel()                     { return _sContenuTransfert ; }
    void   setContenuTransfert(string sC) { _sContenuTransfert = sC ; }
    void setLabel(string code, string texteFictif = string("")) ;

    void SetupWindow() ;
    void UpdateDico() ;
    void ElementSelectionne() ;
    void EvChar(uint key, uint repeatCount, uint flags) ;
    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void EvKeyUp(uint key, uint repeatcount, uint flags) ;
    uint EvGetDlgCode(MSG far* ) ;
    void EvKillFocus(HWND hWndGetFocus) ;
    void EvSetFocus(HWND hWndLostFocus) ;

	DECLARE_RESPONSE_TABLE(NSUtilLexique) ;};

class NSUtilUpDownEdit ;

class _NSBBCLASSE NSUtilUpDown : public OWL::TUpDown, public NSRoot
{
	public:

		// constructors/destructor
  	NSUtilUpDown(NSContexte* pCtx, TWindow *windowParent, int resId, NSUtilUpDownEdit *parent) ;
  	~NSUtilUpDown() ;

  	// functions
  	void EvLButtonDown(uint modKeys, ClassLib::TPoint& point) ;
  	void SetupWindow();

	protected:

  	// variables
		NSUtilUpDownEdit *_pFather ;

	// table de r�ponses
  DECLARE_RESPONSE_TABLE(NSUtilUpDown) ;
} ;

class _NSBBCLASSE NSUtilEditNumSimpl : public OWL::TEdit, public NSRoot
{
	public:

		// constructors/destructor
    NSUtilEditNumSimpl(NSContexte* pCtx, TWindow *windowParent, int resId, NSUtilUpDownEdit *parent) ;
    ~NSUtilEditNumSimpl() ;

    // functions
    void    incremente() ;
    void    decremente() ;
    int     getVal() ;
    void    setVal(int iValue) ;
		void    EvKeyUp(uint key, uint repeatcount, uint flags) ;
		void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
		void    EvChar(uint key, uint repeatCount, uint flags) ;
		uint    EvGetDlgCode(MSG far* msg) ;
		void    SetupWindow() ;

	protected:

		// variables
    NSUtilUpDownEdit* _pFather ;
    TWindow*          _pDlgParent ;

	DECLARE_RESPONSE_TABLE(NSUtilEditNumSimpl) ;
} ;

class _NSBBCLASSE NSUtilUpDownEdit : public NSRoot
{
	public:

		// constructors/destructor
 		NSUtilUpDownEdit(TWindow *windowParent, NSContexte *pCtx, int resEditId, int resUpDownId) ;
  	~NSUtilUpDownEdit() ;

		// functions
  	void               setEditNum(NSUtilEditNumSimpl *pEditNum)    { _pEditNumControl = pEditNum ; }
  	void               setUpDown(NSUtilUpDown *pUpDown)            { _pUpDownControl = pUpDown ; }

  	NSUtilEditNumSimpl *getEditNum()        { return _pEditNumControl ; }
  	NSUtilUpDown       *getUpDown()         { return _pUpDownControl ; }

  	int                getValue()           { return _pEditNumControl->getVal() ; }
  	void               setValue(int iValue) { _pEditNumControl->setVal(iValue) ; }

  	// variables
	protected:

		NSUtilUpDown       *_pUpDownControl ;
  	NSUtilEditNumSimpl *_pEditNumControl ;
} ;

#endif

