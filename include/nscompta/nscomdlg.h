// nscomdlg.h : header des dialogues compta
#ifndef __NSCOMDLG_H#define __NSCOMDLG_H

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
#include "nscompta\nsvitale.h"
#include "nscompta\nsagavar.h"

// #include "nautilus\nsannexe.h"// #include "nautilus\nsperary.h"

class _CLASSELEXI NSOrganEdit : public NSUtilEdit
{
  public:

    string sCodeOrgan ;

    // Constructeur et destructeur
	  NSOrganEdit(NSContexte *pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1) ;
	  ~NSOrganEdit() ;

    void SetupWindow() ;

    void EvChar(uint key, uint repeatCount, uint flags) ;
    bool GetCode(string&) ;
  //
	// child id notification handled at the child
	//
	DECLARE_RESPONSE_TABLE(NSOrganEdit);
};

//// Dialogue de saisie de la carte vitale (bloc 4)
//
class _CLASSELEXI NSListBenWindow ;

class _CLASSELEXI CreerCarteVitaleDialog : public NSUtilDialog
{
	public:

   NSVitale1Data*		pData ;
   NSListBenWindow* pListeBenef ;
   NSVitale2Array*  pBenefArray ;
   NSVitale2Array*  pOldBenefArray ;		// pour le cas Cancel
   NSVitale2Info*   pBenefSelect ;

   int						  nbBenef;
   int						  BenefChoisi;
   bool						  bContientAssure;

   // variables de saisie
   NSOrganEdit*	 		pCaisse;
   NSUtilEdit2*			pNumImma;
   NSOrganEdit*			pCentre;
   NSUtilEdit2*			pCodeGes;
   NSUtilEdit2*			pCodeReg;
   NSUtilEdit*			pAdrCaisse;

   CreerCarteVitaleDialog(TWindow* pere, NSContexte* pCtx);
   ~CreerCarteVitaleDialog();

   void SetupWindow();
   bool InitBenefArray(bool bMajBase = false);
   void InitListe();
   void AfficheListe();
   void LvnGetDispInfo(TLwDispInfoNotify& dispInfo);

   // bool MajNumerosCV2(); fait par InitBenefArray(true)

   bool InitDataBenef(NSVitale2Info* pBenefInfo, bool bCreer);
   void CmAjouter();
   void CmModifier();
   void CmEnlever();

   void CmOk();
   void CmCancel();

   DECLARE_RESPONSE_TABLE(CreerCarteVitaleDialog);
};

//// Objet "ListWindow" avec gestion du double-click (pour les bénéficiaires)
//
class _CLASSELEXI NSListBenWindow : public TListWindow
{
	public:

	CreerCarteVitaleDialog* pDlg;

	NSListBenWindow(CreerCarteVitaleDialog* pere, int resId) : TListWindow(pere, resId)
   {
   	pDlg = pere;
   }
   ~NSListBenWindow() {}

	void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point);
   int  IndexItemSelect();

   DECLARE_RESPONSE_TABLE(NSListBenWindow);
};

// dialogue de saisie du Num SS (pour la saisie carte vitale)

class _CLASSELEXI NumSSDialog : public NSUtilDialog
{
	public:

   char numImma[CSV1_IMMATRICUL_LEN + 1];

   // variables de saisie
   NSUtilEdit2*			pNumImma;

   NumSSDialog(TWindow* pere, NSContexte* pCtx);
   ~NumSSDialog();

   void SetupWindow();

   void CmOk();
   void CmCancel();

   DECLARE_RESPONSE_TABLE(NumSSDialog);
};

//
// Dialogue de saisie du/des bénéficiaires de la carte vitale (bloc 5)
//
class _CLASSELEXI CreerBeneficiaireDialog : public NSUtilDialog
{
	public:

   NSVitale2Data*			pData;
   NSComboArray*			pCBQualiteArray;

   // variables de saisie
   NSUtilEdit2*	 		pNomPatro;
   NSUtilEdit2*			pNomUsuel;
   NSUtilEdit2*			pPrenom;
   NSUtilEdit2*			pRangGem;
   NSUtilEditDate*		pDateNaiss;
   NSUtilEditHeure*		pHeureNaiss;
   NSUtilEdit2*			pAdrBenef;
   TComboBox*				pCBQualite;   TGroupBox*				pGBAld;
   TRadioButton*			pNoAld;
   TRadioButton*			pDListe1;
   TRadioButton*			pHListe1;
   TRadioButton*			pMult1;
   TRadioButton*			pDListe2;
   TRadioButton*			pHListe2;
   TRadioButton*			pMult2;

   TGroupBox*				pGBAmo;
   TRadioButton*			pAmoOui;
   TRadioButton*			pAmoNon;
   NSUtilEdit2*			pMutuelle;
   TGroupBox*				pGBMutu;
   TRadioButton*			pMutuOui;
   TRadioButton*			pMutuNon;
   NSUtilEdit2*			pCodeSitu;
   NSUtilEdit2*			pGaranties;

   CreerBeneficiaireDialog(TWindow* pere, NSContexte* pCtx);
   ~CreerBeneficiaireDialog();

   void SetupWindow();
   void AfficheDonnees();

   void CmPatEnCours();
   void CmAutrePat();

   void CmOk();
   void CmCancel();

  DECLARE_RESPONSE_TABLE(CreerBeneficiaireDialog) ;
};

#endif // fin de nscomdlg.h
