#ifndef __NSRECHDL_H#define __NSRECHDL_H
class NSModeStatic ;class NSUtilEdit ;class NSUtilEdit2 ;class NSUtilEditDate ;class NSDocHistoArray ;// class NSChemiseArray ;#include <classlib\arrays.h>
#include <owl\dialog.h>#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\scrollba.h>
#include <owl\richedit.h>
#include <owl\listwind.h>
#include <owl\tabctrl.h>

#include "nsbb\nsutidlg.h"
// #include "nautilus\nsdochis.h"
#include "nautilus\nsperary.h"
#include "nsdn\nsanxary.h"
// #include "partage\ns_vector.h"
// #include "nsbb\nsednum.h"
// #include "nautilus\nsperary.h"
// #include "nautilus\nsdocaga.h"
#include "nsoutil\nsrechcr.h"

// bool NSTableauNoirRegister();

//
// Objet "Boite de dialogue" utilisé pour demander son code secret
// à l'utilisateur
//
class NSUserWithRole
{
  public:

  	NSUserWithRole() ;
    NSUserWithRole(const NSUserWithRole& rv) ;
    ~NSUserWithRole() {}

    NSUserWithRole& operator=(const NSUserWithRole& src) ;

    string  getLogin()                  { return _sLogin ; }
    void    setLogin(string sBal)       { _sLogin = sBal ; }

    string  getRole()                   { return _sRole ; }
    void    setRole(string sText)       { _sRole = sText ; }

    string  getPasswdType()             { return _sType ; }
    void    setPasswdType(string sText) { _sType = sText ; }

  private:

  	string _sLogin ;
    string _sRole ;
    string _sType ;
}  ;

inline NSUserWithRole::NSUserWithRole()
{
  _sLogin = string("") ;
  _sRole  = string("") ;
  _sType  = string("") ;
}

inline NSUserWithRole::NSUserWithRole(const NSUserWithRole& rv)
{
  _sLogin = rv._sLogin ;
  _sRole  = rv._sRole ;
  _sType  = rv._sType ;
}

inline NSUserWithRole&
NSUserWithRole::operator=(const NSUserWithRole& src)
{
  if (this == &src)
		return *this ;

  _sLogin = src._sLogin ;
  _sRole  = src._sRole ;
  _sType  = src._sType ;

  return *this ;
}

typedef vector<NSUserWithRole*> NSUserWithRoleVector ;
typedef NSUserWithRoleVector::iterator       NSUserWithRoleIter ;typedef NSUserWithRoleVector::const_iterator NSUserWithRoleConstIter ;
class NSUserWithRoleArray : public NSUserWithRoleVector{	public :
  	NSUserWithRoleArray() : NSUserWithRoleVector() {}		NSUserWithRoleArray(const NSUserWithRoleArray& rv) ;
		~NSUserWithRoleArray() ;
    NSUserWithRoleArray& operator=(const NSUserWithRoleArray& src) ;
    void vider() ;};

class UserLoginNTiersDialog : public NSUtilDialog
{
	private:

  	NSUtilisateurChoisi* pUtilChoisi;

 	public:

  	UserLoginNTiersDialog(TWindow *parent, NSContexte *pCtx, NSUtilisateurChoisi* pUtil) ;
  	~UserLoginNTiersDialog() ;

  	void            SetupWindow() ;
  	void            CmOk() ;
  	void            CmLoginDblClk(WPARAM Cmd) ;
    void            CmManual() ;

  	OWL::TListBox       *pLogs ;
  	NSUtilEdit          *pPass ;
  	NSUserWithRoleArray *pUsers ;

    OWL::TStatic        *pPassL ;
    OWL::TStatic        *pId ;
    NSModeStatic        *pCapsLockMode ;
    OWL::TButton        *pQuickStart ;

	DECLARE_RESPONSE_TABLE(UserLoginNTiersDialog) ;
} ;

// -----------------------------------------------------------------
typedef struct XFREBUFFERCHERCHEPAT
{
	char Nom[PAT_NOM_LEN + 1] ;
	char Prenom[PAT_PRENOM_LEN + 1] ;
	char NSS[PAT_NSS_LEN + 1] ;
	char Code[PAT_CODE_LEN + 1] ;
} XBCherchePatient ;

/***************************************************************************///
// Objet "Boite de dialogue" pour l'affichage des propriétés d'un document
//
class NSPropDocDialog : public NSUtilDialog
{
  public :

    NSUtilEdit*     pProp;
    string          sTitre;
    NSDocumentInfo* pInfos;

    NSPropDocDialog(TWindow* parent, string titre, NSDocumentInfo* pDocnfo,
                        NSContexte* pCtx);
    ~NSPropDocDialog();

    void SetupWindow();
    void CmOk();

  DECLARE_RESPONSE_TABLE(NSPropDocDialog) ;
};

class NSDocuBox;

//
// Objet "Boite de dialogue" utilisé pour sélectionner une chemise ou un
// document du dossier patient
//
class  ChoixChemiseDialog : public NSUtilDialog
{
	public:

		OWL::TListBox*  pChemiseBox;
		NSDocuBox*      pDocuBox;
    bool            bEditer;

		NSChemiseArray*  pChemisesArray;
    NSDocHistoArray* pDocusArray;

		UINT	ChemiseChoisie, DocumentChoisi;

		ChoixChemiseDialog(TWindow* parent, NSContexte* pCtx);
		~ChoixChemiseDialog();

		void SetupWindow();

		void CmSelectChemise(WPARAM Cmd);
		void CmSelectDocument(WPARAM Cmd);
		void CmCancel();
		void CmChemiseDblClk(WPARAM Cmd);
		void CmDocumentDblClk(WPARAM Cmd);

		void OuvreDocument(int NumDoc);
        void AjouteDocument(int NumDoc);

	DECLARE_RESPONSE_TABLE(ChoixChemiseDialog);
};

class NSDocuBox : public OWL::TListBox, public NSRoot
{
	public :

		ChoixChemiseDialog* pDlg;

		NSDocuBox(NSContexte* pCtx, ChoixChemiseDialog* pere);
		~NSDocuBox();

		void EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point);
		void Ouvrir();
		void Editer();
		void Ajouter();
		void Parametres();
		void Proprietes();
		void Detruire();

	DECLARE_RESPONSE_TABLE(NSDocuBox);
};

/****************************************************************/
/**      Classes pour la liste des patients à reconvoquer      **/
/****************************************************************/

class NSDateConvocDialog : public NSUtilDialog
{
	public:

		string sDate1 ;
    string sDate2 ;

    OWL::TStatic*   pIntroTxt ;
    OWL::TStatic*   pTxt1 ;
    NSUtilEditDate* pDate1 ;
    OWL::TStatic*   pTxt2 ;
    NSUtilEditDate* pDate2 ;

    NSDateConvocDialog(TWindow* parent, NSContexte* pCtx) ;
    ~NSDateConvocDialog() ;

    void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSDateConvocDialog) ;
};

//
// Objet variables de la liste de reconvocation
// Cet objet est un conteneur des variables à publier
// Il reste résident lors de la publication
//
class NSVarConvoc : public NSSuperRoot
{
	public:

		string        _sDate1 ;
    string        _sDate2 ;
    NSPersonArray _aPatientArray ;
    int           _nbPatient ;

    NSVarConvoc(NSSuper* pSuper) : NSSuperRoot(pSuper), _aPatientArray(pSuper)
    {
    	_nbPatient = 0 ;
    }

    NSVarConvoc(NSSuper* pSuper, string date1, string date2)
                              :NSSuperRoot(pSuper), _aPatientArray(pSuper)
    {
    	_sDate1    = date1 ;
      _sDate2    = date2 ;
      _nbPatient = 0 ;
    }

    NSVarConvoc(const NSVarConvoc& rv)
                    :NSSuperRoot(rv._pSuper), _aPatientArray(rv._aPatientArray)
    {
    	_sDate1    = rv._sDate1 ;
      _sDate2    = rv._sDate2 ;
      _nbPatient = rv._nbPatient ;
    }

    ~NSVarConvoc()
    {}
};

//
// NSListeConvocDialog
//
bool triParDates(NSPersonInfo* a, NSPersonInfo* b);

class NSListeConvocWindow;

class NSListeConvocDialog : public NSUtilDialog
{
	public:

  	OWL::TStatic*        pTxt1 ;
  	NSUtilEditDate*      pDate1 ;
    OWL::TStatic*        pTxt2 ;
    NSUtilEditDate*      pDate2 ;
    NSListeConvocWindow* pListe ;
    int*                 pTabSelect ;
    int                  nbConvoc ;

    NSVarConvoc*         pVar ;
    bool                 bImprimerListe ;
    bool                 bImprimerLettre ;

    string               _sLang ;

    NSListeConvocDialog(TWindow* parent, NSContexte* pCtx, NSVarConvoc* pVarConvoc) ;
    ~NSListeConvocDialog() ;

    void SetupWindow() ;
    bool InitPatientArray() ;
    void InitListe() ;
    void AfficheListe() ;
    void DispInfoListe(TLwDispInfoNotify& dispInfo) ;

    void CmFichePatient(int itemSel) ;
    void CmImprimerListe() ;
    void CmImprimerLettre() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSListeConvocDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les patients)
//
class NSListeConvocWindow : public TListWindow
{
	public:

		NSListeConvocDialog* pDlg ;

    NSListeConvocWindow(NSListeConvocDialog* pere, int resId) ;
    ~NSListeConvocWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;
    int  GetSelIndexes(int* pTabIndex, int tailleTab) ;

	DECLARE_RESPONSE_TABLE(NSListeConvocWindow) ;
};

//
// NSFichePatientDialog
//
class NSFichePatientDialog : public NSUtilDialog{
	public:

  	NSUtilEdit*			pSitFam ;
    NSUtilEdit* 		pNbEnf ;
    NSUtilEdit*			pSexe ;
    NSUtilEditDate* pDateN ;
    NSUtilEdit*			pNss ;
    NSUtilEdit*			pCode ;
    NSUtilEdit*			pTelPor ;
    NSUtilEdit*			pTelBur ;
    NSUtilEditDate*	pDateC ;
    NSUtilEdit*			pAdresse ;
    NSUtilEdit*			pTelDom ;
    NSUtilEdit*			pEMail ;
    NSUtilEdit*			pFax ;
    OWL::TListBox*	pListeCorresp ;

    NSPersonInfo*   pPatInfo ;

    NSFichePatientDialog(TWindow* parent, NSContexte* pCtx, NSPersonInfo* ppi) ;
    ~NSFichePatientDialog() ;

    void SetupWindow() ;

    void AfficheDonneesPatient() ;
    void AfficheDonneesAdresse() ;
    void AfficheDonneesCorresp() ;

    void CmModifDateConvoc() ;
    void CmOk() ;

	DECLARE_RESPONSE_TABLE(NSFichePatientDialog) ;
};

/*************************************************************
    AFFICHAGE DES RESULTATS DE REQUETE (cf nsrechcr.cpp)
**************************************************************/

//
// Classe NSResultReqDialog//
class NSResultReqWindow ;

class NSResultReqDialog : public NSUtilDialog
{
	public:

    NSRequeteDialog*    pReqDlg ;
    NSUtilEdit2*        pNomRes ;
    OWL::TTabControl*   pRequests ;
    NSResultReqWindow*  pListeRes ;
    TStatic*            pPatTotal ;
    TStatic*            pPatCritPat ;
    TStatic*            pPatCritDoc ;
    TStatic*            pPatResult ;
    TStatic*            pDocCritPat ;
    TStatic*            pDocCritDoc ;
    TStatic*            pDocResult ;

    TStatic*            pRequestPatResult ;
    TStatic*            pRequestDocResult ;

    bool                 _bReqModeDoc ;
    int                  _nbPatTotal ;
    int                  _nbPatCritPat ;
    int                  _nbPatCritDoc ;
    int                  _nbPatResult ;
    int                  _nbDocCritPat ;
    int                  _nbDocCritDoc ;
    int                  _nbDocResult ;
    NSDocHistoArray      _VectDocResultat ;    // vecteur des documents résultat
    NSPatientArray       _VectPatResultat ;    // vecteur des patients résultat
    NSRequestResultArray _VectRequestResults ;
    int                  _itemChoisi ;
    string               _sFileNameRes ;

    NSResultReqDialog(TWindow* pere, NSContexte* pCtx,
                            NSRequeteDialog* pRequete = 0) ;
    ~NSResultReqDialog() ;

    void SetupWindow() ;
    void AfficheCompteurs() ;
    void AfficheItem() ;
    void InitListeRes() ;
    void AfficheListeRes() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void EcrireResultats(string &sOut) ;
    bool ChargerResultats(string sFichier) ;
    bool GetResultCounter(string sLine, size_t &pos1, size_t &pos2, int &iNb) ;
    bool ChargerPatResultat() ;
    bool ChargerDocResultat() ;
    void CmFermer() ;
    void CmMasquer() ;
    void CmOuvrir() ;
    void CmEnregistrer() ;
    void CmExcelExport() ;
    void CmTextExport() ;
    void CmImprimer() ;
    void RequestTabKeyDown(TTabKeyDown& nmHdr) ;
    void RequestTabSelChange(TNotify far& nm) ;

    int              getSelectedItem() { return _itemChoisi ; }
    NSPatInfo*       getResultPatient(size_t iIndex) ;
    NSDocumentHisto* getResultDocum(size_t iIndex) ;
    NSDocumentHisto* getSelectedResultDocum() { return getResultDocum(_itemChoisi) ; }
    bool             isReqModeDoc()    { return _bReqModeDoc ;  }
    int              getNbPatTotal()   { return _nbPatTotal ;   }
    int              getNbPatCritPat() { return _nbPatCritPat ; }
    int              getNbPatCritDoc() { return _nbPatCritDoc ; }
    int              getNbPatResult()  { return _nbPatResult ;  }
    int              getNbDocCritPat() { return _nbDocCritPat ; }
    int              getNbDocCritDoc() { return _nbDocCritDoc ; }
    int              getNbDocResult()  { return _nbDocResult ;  }
    NSDocHistoArray* getVectDocResultat() { return &_VectDocResultat ; }  
    NSPatientArray*  getVectPatResultat() { return &_VectPatResultat ; }

	DECLARE_RESPONSE_TABLE(NSResultReqDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click
//
class NSResultReqWindow : public TListWindow
{
	public:

		NSResultReqDialog*   pDlg;

		NSResultReqWindow(NSResultReqDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
    }
    ~NSResultReqWindow() {}

    void SetupWindow() ;
    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);
    int  IndexItemSelect();

	DECLARE_RESPONSE_TABLE(NSResultReqWindow) ;
};

//
// Objet "Boite de dialogue" utilisé Afficher le numéro de version
//
class AboutDialog : public NSUtilDialog
{
	public:

		OWL::TStatic* _pAppName ;
    OWL::TStatic* _pBamName ;
    OWL::TStatic* _pHistory ;
    OWL::TEdit*   _pReleasesViewer ;

		AboutDialog(TWindow* parent, NSContexte* pCtx) ;
    ~AboutDialog() ;

    void SetupWindow() ;
    void CmOk() ;

	DECLARE_RESPONSE_TABLE(AboutDialog) ;
};

//
// Objet "Boite de dialogue" utilisé Afficher le numéro de version
//
class IdentifierDialog : public NSUtilDialog
{
	public:

		TStatic*    _pIdText ;
    OWL::TEdit* _pIdEdit ;

    string   _sPatientId ;

		IdentifierDialog(TWindow* parent, NSContexte* pCtx, string sPatId) ;
    ~IdentifierDialog() ;

    void SetupWindow() ;
    void CmOk() ;

	DECLARE_RESPONSE_TABLE(IdentifierDialog) ;
};


/*************************************************************

	ancienne version

class NSTableauNoir : public TControl
{
	public:

	int	nbNom, iActif;

	TRect	rectFond;

	long HauteurLigne;

	NSTableauNoir(ChercheListePatDialog* parent,
					  const char far* title,
					  int             id,
					  int X, int Y, int W, int H,					  TModule*        module = 0);
	ChercheListePatDialog* pListeDlg;	void SetupWindow();
	void EvPaint();
	void EvLButtonDown(UINT modKeys, TPoint& point);
	void EvLButtonDblClk(UINT modKeys, TPoint& point);
	void DrawItem(DRAWITEMSTRUCT far &drawInfo);

	//
   // message response functions
	//
	bool EvEraseBkgnd(HDC);

	void Paint(TDC& dc, bool, TRect& RectAPeindre);

	DECLARE_RESPONSE_TABLE(NSTableauNoir);
};

**********************************************************************/

#endif

// fin de nsrechdl.h

