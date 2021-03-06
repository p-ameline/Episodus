//// Outil d'�dition et de recherche des fils guides - RS Juillet 1999
//---------------------------------------------------------------------------
#ifndef __NSFILGUI_H
#define __NSFILGUI_H
//---------------------------------------------------------------------------

class NSGroupGdArray ;
class NSGroupGdData ;
class NSUtilLexique ;
class NSUtilEdit ;
class NSUtilEditDate ;
class NSListFGWindow ;
class NSEltArray ;

#include <owl\dialog.h>
#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>
#include <owl\combobox.h>

// #include "nsbb\nsednum.h"
#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"

class NSElement
{
	public:

		string  code ;    string  libelle ;
    string  decalage ;   // pour la cr�ation d'un chemin de recherche
    int     result ;     // uniquement pour la recherche

    NSElement() ;
    NSElement(string sCode, string sLibelle, string sDecalage = "+01+01") ;
    ~NSElement() ;

    NSElement(NSElement& rv) ;
    NSElement& operator=(NSElement src) ;
    int        operator==(NSElement& o) ;
} ;

//// D�finition de NSEltArray (Array de NSElement(s))
//
typedef vector<NSElement*> NSEltVector ;
typedef NSEltVector::iterator NSEltIter ;

class NSEltArray : public NSEltVector
{
	public :

	// Constructeurs
	NSEltArray() : NSEltVector() {}
	NSEltArray(NSEltArray& rv) ;
	// Destructeur
	virtual ~NSEltArray() ;
  void vider() ;
};

//
// Classe NSModifFilGuideDialog
//

class NSModifFilGuideDialog : public NSUtilDialog{
	public:

    bool                bPere ;
    string              sCode ;
    NSUtilLexique*      pEdit ;
    OWL::TRadioButton*	pTouteSequence ;
    OWL::TRadioButton*	pToutElement ;
    OWL::TGroupBox*     pCertitude ;
    OWL::TRadioButton*  pCertSans ;
    OWL::TRadioButton*  pCert00 ;
    OWL::TRadioButton*  pCert25 ;
    OWL::TRadioButton*  pCert50 ;
    OWL::TRadioButton*  pCert75 ;
    OWL::TRadioButton*  pCert100 ;
    OWL::TGroupBox*     pPluriel ;
    OWL::TRadioButton*  pPlur1 ;
    OWL::TRadioButton*  pPlur2 ;
    OWL::TRadioButton*  pSingulier ;

    NSModifFilGuideDialog(TWindow* pere, string code, NSContexte* pCtx,
                            TModule* module = 0, bool bDialoguePere = true) ;
    ~NSModifFilGuideDialog() ;

    void SetupWindow() ;

    void UncheckAll() ;
    void CmTouteSequence() ;
    void CmToutElement() ;
    void CmMajCode() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSModifFilGuideDialog) ;
};

//// Classe NSEditFilGuideDialog
//

class _NSOUTILCLASSE NSListFGWindow ;

class _NSOUTILCLASSE NSEditFilGuideDialog : public NSUtilDialog
{
	public:

    NSUtilEdit*     pEditComment ;
    TStatic*        pEditGroupe ;
    NSListFGWindow* pListePere ;
    NSListFGWindow* pListeFils ;
    NSEltArray*     pPereArray ;
    NSEltArray*     pFilsArray ;
    int		          nbPere ;
    int					    nbFils ;
    string          sCodeGroupe ;
    string          sCodeFilGuide ;  // pour la modification
		string          sCheminPere ;    // pour pouvoir r�cup�rer le nouveau chemin
    bool            bChoixGroupe ;   // pour choisir le groupe (cas TreeNodes)

		NSEditFilGuideDialog(TWindow* pere, NSContexte* pCtx, string codeGroupe = "",
                                string codeFilGuide = "") ;
		~NSEditFilGuideDialog() ;

    void SetupWindow() ;
    void InitListePere() ;
    void AfficheListePere() ;
    // void LvnGetDispInfo(TLwDispInfoNotify& dispInfo);
    void InitListeFils() ;
    void AfficheListeFils() ;
    // void LvnGetDispInfo(TLwDispInfoNotify& dispInfo);
    void TrouveLibelle(string sCode, string& sLibelle, bool bPere) ;
    void ParsingChemin(string sChem, bool bPere) ;
    bool ChargeFilGuide() ;
    void ChoixGroupe() ;
    void AfficheNomGroupe() ;

    // Controles de la boite Pere
    void CmHautPere() ;
    void CmBasPere() ;
    void CmInsPere() ;
    void CmModPere() ;
    void CmDelPere() ;

    // Controles de la boite fils
    void CmHautFils() ;
    void CmBasFils() ;
    void CmInsFils() ;
    void CmModFils() ;
    void CmDelFils() ;

    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSEditFilGuideDialog) ;
};

//// Objet "ListWindow" avec gestion du double-click
//
class _NSOUTILCLASSE NSListFGWindow : public TListWindow
{
	public:

		NSEditFilGuideDialog* pDlg ;
		int                   iRes ;

    NSListFGWindow(NSEditFilGuideDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
      iRes = resId ;
    }
    ~NSListFGWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListFGWindow) ;
};

/***************************************************************************************/
/***                        GESTION DES GROUPES DE FILS GUIDES                       ***/
/***************************************************************************************/

class _NSOUTILCLASSE NSGuidesManager : public NSRoot
{
	public:

		NSGuidesManager(NSContexte* pCtx) ;
		~NSGuidesManager() ;

		bool   deleteGroup(string sGroupName = string("")) ;
    bool   doesGroupExist(string sCode, bool& bError, string& sDate) ;

		bool   importGroup(string sFileCompletePath = string("")) ;
    bool   referenceGroup() ;
    bool   exportGroup(string sFileCompletePath = string("")) ;

    bool   isHeaderValid()  { return bValidHeader ; }
    bool   isSystemGroup()  { return ((string("") != sGroup) && ('$' == sGroup[0])) ; }

    string getFileVersion() { return sFileVersion ; }
		string getGroup()       { return sGroup ; }
		string getLabel()       { return sLabel ; }
		string getGroupDate()   { return sGroupDate ; }
		string getUser()        { return sUser ; }

    void   setImportFile(string sNewFile) ;

	protected:

		bool   bValidHeader ;

		string sFileVersion ;
		string sGroup ;
		string sLabel ;
		string sGroupDate ;
		string sUser ;

		string sFile ;

		void init() ;
    bool readHeader() ;

		bool deleteGuidesFromGroup(string sGroupName) ;
    bool unreferenceGroup(string sGroupName) ;
};

//
// Objet "Boite de dialogue" utilis� pour s�lectionner l'utilitaire
//
class _NSOUTILCLASSE NSGroupGdDialog : public NSUtilDialog
{
	public:

		OWL::TButton*	pImport ;
    OWL::TButton*	pExport ;
    OWL::TButton* pDestroy ;
    OWL::TButton* pGestion ;

	  NSGroupGdDialog(TWindow* pere, NSContexte* pCtx) ;
	  ~NSGroupGdDialog() ;

	  void SetupWindow() ;

    bool ExisteCode(string sCode, bool& bExiste, string& sDate) ;
    bool ExisteLibelle(string sLibelle, bool& bExiste, string& sDate) ;
    bool DetruireGroupe(string sCode) ;
    bool CodeDispo(string& sCode) ;
    void CmImport() ;
    void CmExport() ;
    void CmDestroy() ;
    void CmGestion() ;
    void CmOk() ;
	  void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSGroupGdDialog) ;
};

//
// Objet "Boite de dialogue" utilis� pour s�lectionner un groupe
// de fils guides � exporter ou � d�truire//
class _NSOUTILCLASSE NSChoixGdDialog : public NSUtilDialog
{
	public:

		OWL::TGroupBox*		 pFGGB ;
    OWL::TRadioButton* pFGSys ;
    OWL::TRadioButton* pFGUtil ;
		OWL::TListBox* 		 pGroupBox ;
		NSGroupGdArray*    pGroupArray ;
		int 	             GroupChoisi ;
    bool               bFGSys ;
    bool               bUtilOnly ;

		NSChoixGdDialog(TWindow* pere, NSContexte* pCtx, bool utilOnly = false) ;
		~NSChoixGdDialog() ;

		void SetupWindow() ;

    bool InitListe() ;
    void FilGuideSys() ;
    void FilGuideUtil() ;
    void CmNewGroup() ;
    void CmModGroup() ;
		void CmSelectGroup(WPARAM Cmd) ;
		void CmCancel() ;
		void CmGroupDblClk(WPARAM Cmd) ;

		// void LanceTemplate(int NumDoc);

	DECLARE_RESPONSE_TABLE(NSChoixGdDialog) ;
};

//// Objet "Boite de dialogue" utilis� pour choisir
// le nom du fichier groupe � exporter
//
class _NSOUTILCLASSE NSNomGdDialog : public NSUtilDialog
{
	public:

		TStatic*    pText ;
    NSUtilEdit* pNom ;
    string      sPath ;
    string      sExtension ;
    string      sFichier ;

		NSNomGdDialog(TWindow* pere, string sDir, string sNom, string sExt, NSContexte* pCtx) ;
		~NSNomGdDialog() ;

		void SetupWindow() ;

    void CmOk() ;
		void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSNomGdDialog) ;
};

//
//  Outil de gestion des fils guides et des groupes
//
class NSListChemWindow;

class NSListChemDialog : public NSUtilDialog
{
	public :

    OWL::TComboBox*     pGroupBox ;
    NSListChemWindow*   pListeChem ;
    NSEltArray*         pChemArray ;
    int                 nbChem ;
    NSGroupGdArray*     pGroupArray ;
    int                 GroupChoisi ;

    NSListChemDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~NSListChemDialog() ;

    void SetupWindow() ;
    bool InitGroupBox() ;
    void InitListeChem() ;
    void AfficheListeChem() ;
    void EvGroupChange() ;
    void TrouveLibElement(string sElement, string& sLibElement) ;
    bool InitChemArray() ;
    bool PermuteFG(int indexInf) ;

    void CmNewFG() ;
    void CmModFG() ;
    void CmDupFG() ;
    void CmDelFG() ;
    void CmHaut() ;
    void CmBas() ;
    void CmNewGP() ;
    void CmModGP() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSListChemDialog) ;
};

//// Objet "ListWindow" avec gestion du double-click
//
class NSListChemWindow : public TListWindow
{
	public:

		NSListChemDialog* pDlg ;

    NSListChemWindow(NSListChemDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
    }
    ~NSListChemWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListChemWindow) ;
};

//
// Dialogue de cr�ation / modification des groupes de fils guides
//
class NSCreateGdDialog : public NSUtilDialog
{
	public:

		// donn�es � r�cup�rer
    NSGroupGdData* 	pData ;

    // Champs de saisie
    NSUtilEdit*			pCodeGd ;
		NSUtilEdit*			pLibelleGd ;
    NSUtilEdit*			pUtilGd ;
    NSUtilEditDate* pDateGd ;

		NSCreateGdDialog(TWindow*, NSContexte* pCtx, TModule* module = 0) ;
		~NSCreateGdDialog() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSCreateGdDialog) ;
};

#endif

