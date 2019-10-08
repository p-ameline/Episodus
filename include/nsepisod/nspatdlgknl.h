#ifndef __NSPATDLGKNL_H#define __NSPATDLGKNL_H

#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

#include <owl\dialog.h>
#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>

#include "nautilus\nsperary.h"#include "nsbb\nslistwind.h"

class NSNTiersListPatWindow ;
class NSNTiersRechNomEdit ;

//
// Objet "Boite de dialogue" root utilis� pour s�lectionner un patient au sein
// d'une liste en mode NTiers
//
class _EPISODUS NSNTiersListePatDialog : public NSUtilDialog
{
	public:

		int					           nbNom ;
		string			 	         sNssSelect ;

		string			 	         _sNomPat ;
		string			 	         _sPrenomPat ;
    string			 	         _sDateNaiss ;
		string				         _sNumPat ;
		string                 _sCodePat ;

		const char*            serviceName ;
		bool                   bMustInit ;

		NSPatInfo*             pPatEnCours ;
		NSPatientArray*        pPatientsArray ;

		NSNTiersRechNomEdit*	 pNom ;              // recherche par nom
		NSNTiersRechNomEdit*	 pPrenom ;           // et pr�nom
    NSUtilEditDate*        pDateNaiss ;
		NSUtilEdit*			       pNumero ;
		NSUtilEdit*			       pCode ;

    OWL::TRadioButton*     pExactButton ;
    OWL::TRadioButton*     pStartByButton ;

    OWL::TCheckBox*        pUseNom ;
    OWL::TCheckBox*        pUsePrenom ;
    OWL::TCheckBox*        pUseBirthdate ;

    OWL::TStatic*          pLibName ;
    OWL::TStatic*          pLibGivenName ;
    OWL::TStatic*          pLibBirthdate ;
    OWL::TStatic*          pLibID ;
    OWL::TStatic*          pLibCode ;

    OWL::TGlyphButton*     pNameEditParams ;
    OWL::TBitmap*          pParamsBitmap ;

		NSNTiersListPatWindow* pListe ;

    int                    iSortedColumn ;
    bool                   bNaturallySorted ;

    size_t                 _iAutoSearchDelay ;
    size_t                 _iAutoSearchMinChar ;

		NSNTiersListePatDialog(TWindow* parent, NSContexte* pCtx, NSPatInfo* pPat,
                         char* cResId = "IDD_NTLISTPABASE", OWL::TModule* module = 0) ;
		~NSNTiersListePatDialog() ;

		void SetupWindow() ;

		void InitListe() ;
		void AfficheListe() ;
		void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
		void InitPatArray() ;
    void InitPatArrayForNss() ;
    void InitPatArrayForCode() ;
		// void PosLastPatient();
		void Rechercher() ;
    void NameEditParams() ;

    void nameActiver(WPARAM wParam) ;

		void GardeNss(int index) ;
		void RetrouvePatSelect() ;

		bool CanClose() ;
		void CmOk() ;

    void LVNColumnclick(TLwNotify& lwn) ;
    void sortByName(bool bChangeOrder = false) ;
    void sortByBirthday() ;
    void sortBySex() ;

    void prepareString(string& sStr) ;

    void EvTimer(uint id) ;
    void resetTimer() ;

  protected:

    bool existSearchableInformation() ;

	DECLARE_RESPONSE_TABLE(NSNTiersListePatDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click et du scroll (pour les patients)
//
class _EPISODUS NSNTiersListPatWindow : public NSSkinableListWindow
{
  public:

    NSNTiersListePatDialog* pDlg ;

    NSNTiersListPatWindow(NSNTiersListePatDialog* pere, NSContexte* pCtx, int resId) ;
    ~NSNTiersListPatWindow() {}

    void    SetupWindow() ;
    void    EvKeyDown(uint key, uint repeatCount, uint flags) ;
    void    EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int     IndexItemSelect() ;
    void    EvSetFocus(HWND hWndLostFocus) ;    void    EvKillFocus(HWND hWndGetFocus) ;
  DECLARE_RESPONSE_TABLE(NSNTiersListPatWindow) ;};

class _EPISODUS NSNTiersRechNomEdit : public NSUtilEdit
{
	public:

		NSNTiersListePatDialog* pDlg ;

		// on bloque le premier Return (booleen false dans NSUtilEdit)
		NSNTiersRechNomEdit(NSContexte *pCtx, NSNTiersListePatDialog* pere, int resId, UINT iTextLen)
                      :NSUtilEdit(pCtx, pere, resId, iTextLen, false) { pDlg = pere ; }

		~NSNTiersRechNomEdit() {}

		void EvChar(uint key, uint repeatCount, uint flags) ;
		void EvKeyUp(uint key, uint repeatCount, uint flags) ;

	DECLARE_RESPONSE_TABLE(NSNTiersRechNomEdit) ;
};

// Liste correspondants
#define IDC_LISTCOR_LW	        101
#define IDC_LISTCOR_AUTRE	      102

//
// Classe ChercheListePatCorDialog -> pour la s�lection d'un correspondant parmi
//													les correspondants du patient
//

class NSListPatCorWindow;
class _EPISODUS ChercheListePatCorDialog : public NSUtilDialog{
	public:

		NSListPatCorWindow* pListeCorresp ;
    NSPersonArray*      pCorrespArray ;
    NSPersonInfo*       pCorrespSelect ;

    int					 nbCorresp;
    int					 CorrespChoisi;

    ChercheListePatCorDialog(TWindow* pere, NSContexte* pCtx) ;
    ~ChercheListePatCorDialog() ;

    void SetupWindow() ;
    bool InitPatCorArray() ;
    void InitListe() ;
    void AfficheListe() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void CmAutreCorresp() ;

    void CmOk() ;
    void CmCancel() ;

  DECLARE_RESPONSE_TABLE(ChercheListePatCorDialog) ;
};

//// Objet "ListWindow" avec gestion du double-click (pour les correspondants)
//
class _EPISODUS NSListPatCorWindow : public TListWindow
{	public:

		ChercheListePatCorDialog* pDlg ;

    NSListPatCorWindow(ChercheListePatCorDialog* pere, int resId, OWL::TModule* module = 0) ;
    ~NSListPatCorWindow() ;

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListPatCorWindow) ;};

/***************************************************************************/
#endif  // __NSPATDLGKNL_H
