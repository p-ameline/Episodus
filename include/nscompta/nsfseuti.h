// NSFseUti : Structures nécéssaires aux dialogues de la fiche Compt, Fact et TPayant//            + Dialogue NSListComptDialog pour les situations
// Rémi SPAAK Avril 99
/////////////////////////////////////////////////////////////////////////////////////

#ifndef __NSFSEUTI_H
#define __NSFSEUTI_H

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>
#include <owl\combobox.h>

#include "nsbb\nsednum.h"
#include "nscompta\nsfse16.h"    // classes NSFse16xx
#include "nscompta\nsagavar.h"   // pour NSVarCompta

class NSBlocFse16
{
  public:

    enum BLOCTYPE { blocUndefined = 0, bloc1610, bloc1620, bloc1630, blocCcam, blocLibre, blocMat } ;

    BLOCTYPE  	    _iTypePrest ;
   	string          _sNumPrest ;

   	NSFse1610Info*  _p1610 ;
   	NSFse1620Info*  _p1620 ;
   	NSFse1630Info*  _p1630 ;
    NSFseCCAMInfo*  _pCCAM ;    NSFseLibreInfo* _pLibre ;    NSFseMatInfo*   _pMat ;
   	NSBlocFse16(const NSFse1610* pFse1610) ;
		NSBlocFse16(const NSFse1610Info* pFse1610Info) ;
   	NSBlocFse16(const NSFse1620* pFse1620);
   	NSBlocFse16(const NSFse1620Info* pFse1620Info) ;
   	NSBlocFse16(const NSFse1630* pFse1630) ;
   	NSBlocFse16(const NSFse1630Info* pFse1630Info) ;
    NSBlocFse16(const NSFseCCAM* pFseCCAM) ;
    NSBlocFse16(const NSFseCCAMInfo* pFseCCAMInfo) ;
    NSBlocFse16(const NSFseLibre* pFseLibre) ;
		NSBlocFse16(const NSFseLibreInfo* pFseLibreInfo) ;
    NSBlocFse16(const NSFseMat* pFseMat) ;
		NSBlocFse16(const NSFseMatInfo* pFseMatInfo) ;
    // constructeur copie
   	NSBlocFse16(const NSBlocFse16& rv) ;
    NSBlocFse16& operator=(const NSBlocFse16& rv) ;
		// destructeur
		~NSBlocFse16() ;

    string exportAsString() ;
    void   importFromString(string sModele) ;

    string   getNumprest()  { return _sNumPrest ; }
    BLOCTYPE getTypePrest() { return _iTypePrest ; }

    NSFse1610Info*  get1610()  { return _p1610 ;  }
   	NSFse1620Info*  get1620()  { return _p1620 ;  }
   	NSFse1630Info*  get1630()  { return _p1630 ;  }
    NSFseCCAMInfo*  getCcam()  { return _pCCAM ;  }    NSFseLibreInfo* getLibre() { return _pLibre ; }    NSFseMatInfo*   getMat()   { return _pMat ;   }

    void   setNumprest(string sNumprest) ;
    void   setNumprest(int    iNumprest) ;

    static string GetSumInUnits(string sSumInCents) ;
};

//// Définition de NSFse16Array (Array de NSFse16xxInfo(s))
//
typedef vector<NSBlocFse16*>           NSFse16xxArray ;
typedef NSFse16xxArray::iterator       NSFse16Iter ;
typedef NSFse16xxArray::const_iterator NSFse16ConstIter ;

class _CLASSELEXI NSFse16Array : public NSFse16xxArray
{
	public :

		// Constructeurs
		NSFse16Array() : NSFse16xxArray() {}
		NSFse16Array(const NSFse16Array& rv);
		// Destructeur
		virtual ~NSFse16Array();

    NSFse16Array& operator=(const NSFse16Array& rv) ;

   	void vider();
};

//
// Fonctions globales de tri pour les tableaux FseArray et FactArray de CreerFicheComptDialog
//
bool blocInferieur(NSBlocFse16* a, NSBlocFse16* b);
bool factAnterieure(NSFactInfo* a, NSFactInfo* b);

// on définit la classe CreerFicheComptDialog utilisée
// par tous les controles intégrés dans des classes
class _CLASSELEXI CreerFicheComptDialog;

// classe NSEditDateC pour permettre l'affichage du dépassement
/////////////////////////////////////////////////////////////////////////////
class _CLASSELEXI NSEditDateC : public NSUtilEditDate
{
	public:

		CreerFicheComptDialog* pDlg ;

		// Constructeur et destructeur
		NSEditDateC(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen = 10) ;
		~NSEditDateC() {}

    void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSEditDateC) ;
};

// classe NSEditSommeDue pour permettre l'affichage du dépassement
/////////////////////////////////////////////////////////////////////////////
class _CLASSELEXI NSEditHeureC : public NSUtilEditHeure
{
	public:

    CreerFicheComptDialog* pDlg ;

		// Constructeur et destructeur
		NSEditHeureC(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen = 5) ;
		~NSEditHeureC() {}

    void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSEditHeureC) ;
};

// classe NSEditSommeDue pour permettre l'affichage du dépassement
/////////////////////////////////////////////////////////////////////////////
class _CLASSELEXI NSEditSommeDue : public NSUtilEditSomme
{
	public:

    CreerFicheComptDialog* pDlg ;

		// Constructeur et destructeur
		NSEditSommeDue(NSContexte *pCtx, CreerFicheComptDialog* pere, int resId, int iTextLen = 8) ;
		~NSEditSommeDue() {}

    void EvKillFocus(HWND hWndGetFocus) ;

	DECLARE_RESPONSE_TABLE(NSEditSommeDue) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les FSE16xx)
//
class _CLASSELEXI NSListFseWindow : public TListWindow
{
	public:

		CreerFicheComptDialog* pDlg ;

		NSListFseWindow(CreerFicheComptDialog* pere, int resId) ;
    ~NSListFseWindow() {}

    void SetupWindow() ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListFseWindow) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les FACT)
//
class _CLASSELEXI NSListFactWindow : public TListWindow
{
	public:

		CreerFicheComptDialog* pDlg ;

		NSListFactWindow(CreerFicheComptDialog* pere, int resId) ;
    ~NSListFactWindow() {}

    void SetupWindow() ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListFactWindow) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les Tiers-payants)
//
class _CLASSELEXI NSListTPWindow : public TListWindow
{
	public:

		CreerFicheComptDialog* pDlg ;

		NSListTPWindow(CreerFicheComptDialog* pere, int resId) ;
    ~NSListTPWindow() {}

    void SetupWindow() ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListTPWindow) ;
};

//
// Objet dialogue pour la situation des fiches compt
//
class _CLASSELEXI NSListComptWindow;

class _CLASSELEXI NSListComptDialog : public NSUtilDialog
{
	public:

		NSPersonInfo*			 pPatient ;
   	string 					   sNumCompt ;
   	int	 					     nbCompt ;
   	NSVarCompta* 			 pVar ;
   	int						     totalDu ;
   	int						     totalPaye ;
   	bool						   bEuro ;
   	bool						   bErreur ;

   	NSListComptWindow* pListeCompt ;
   	NSComptArray*		   pComptArray ;
   	OWL::TGroupBox*		 pMonnaie ;
   	OWL::TRadioButton* pLocal ;
   	OWL::TRadioButton* pEuro ;
   	NSUtilEdit2*			 pTotalDu ;
   	NSUtilEdit2*			 pTotalPaye ;

		NSListComptDialog(TWindow* pere, NSContexte* pCtx, NSPersonInfo* pPat) ;
   	~NSListComptDialog() ;

   	void SetupWindow() ;
   	bool InitComptArray() ;
		void InitListeCompt() ;
   	void AfficheListeCompt() ;
   	void DispInfoListeCompt(TLwDispInfoNotify& dispInfo) ;
    void EvSize(uint sizeType, ClassLib::TSize& size) ;

   	void AfficheTotaux() ;
   	void SwitchLocEuro() ;

   	void CmModifCompt() ;
   	void CmDeleteCompt() ;
   	void CmOk() ;
   	void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSListComptDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les fiches compt à modifier)
//
class _CLASSELEXI NSListComptWindow : public TListWindow
{
	public:

		NSListComptDialog* pDlg;

		NSListComptWindow(NSListComptDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
      Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;
      Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;
    }    ~NSListComptWindow() {}

    void SetupWindow() ;    void EvKeyDown(uint key, uint repeatCount, uint flags) ;
		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListComptWindow) ;
};

#endif

// fin de nsfseuti.h////////////////////////////////////////////////

