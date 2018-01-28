// NSFseDlg : dialogues de la fiche Compt et fact// Rémi SPAAK Aout 98
//////////////////////////////////////////////////////
#ifndef __NSFSEDLG_H
#define __NSFSEDLG_H

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

#include "nscompta\nsfseuti.h"

//
// Dialogue de saisie de la fiche Compt
//
class _CLASSELEXI CreerTPayantDialog;
class _CLASSELEXI NSUtilExamen;

class _CLASSELEXI CreerFicheComptDialog : public NSUtilDialog{
	public:

   NSComptData*			  pData ;
   NSPersonInfo*			pPatient ;
   NSListFseWindow*   pListeFse ;			 // liste des prestations
   NSFse16Array*		  pFseArray ;
   NSListFactWindow*  pListeFact ;		 // liste des paiements
   NSFactArray*			  pFactArray ;
   NSListTPWindow* 	  pListeTP ;			 // liste des reste-dus par tiers-payant
   NSTPArray*				  pTPArray ;
   bool						    bCreerPaiement ; // sert pour la creation fact0

   int						    nbPrest ;
   int						    nbFact ;
   int						    nbTP ;
   char						    codeCorresp[4] ;
   bool						    bEuro ;
   int						    sommeTheorique ;

   NSVarCompta*				pVar ;
   string					    sCodeAlerte ;

   // variables de saisie
   NSEditDateC*			  pDate ;
   NSEditHeureC*		  pHeure ;
   NSUtilExamen*		  pExamen ;
   OWL::TGroupBox*		pGBMonnaie ;
   OWL::TRadioButton*	pLocal ;
   OWL::TRadioButton*	pEuro ;
   OWL::TGroupBox*		pGBContexte ;
   OWL::TRadioButton*	pExterne ;
   OWL::TRadioButton*	pAmbulatoire ;
   OWL::TRadioButton*	pHospital ;
   NSEditSommeDue*		pSommeDue ;
   NSUtilEditSomme*		pDepass ;
   NSUtilEditSomme*		pPaye ;
   NSUtilEditSomme*		pResteDu ;
   TStatic*				    pCorresp ;

   CreerFicheComptDialog(TWindow* pere, NSContexte* pCtx, NSPersonInfo* pPat, bool bCreation = true, NSComptData* pCompData = (NSComptData*) 0, bool bNoInit = false) ;
   ~CreerFicheComptDialog() ;

   void InitData() ;

   void SetupWindow() ;

   // chargement des arrays
   bool InitFse16Array() ;

   bool InitFse1610Array() ;
   bool InitFse1620Array() ;
   bool InitFse1630Array() ;
   bool InitFseCCAMArray() ;   bool InitFseCLAMArray() ;   bool InitFseLotsArray() ;   bool InitFactArray() ;
   bool InitTPArray() ;

   // gestion des ListWindows
   void InitListeFse() ;
   void AfficheListeFse() ;
   void DispInfoListeFse(TLwDispInfoNotify& dispInfo) ;
   void InitListeFact() ;
   void AfficheListeFact() ;
   void DispInfoListeFact(TLwDispInfoNotify& dispInfo) ;
   void InitListeTP() ;
   void AfficheListeTP() ;
   void DispInfoListeTP(TLwDispInfoNotify& dispInfo) ;

   // interfaces des dialogues fils
   bool InitDataFse1610(NSFse1610Info* pFse1610Info, bool bCreer) ;
   bool InitDataFse1620(NSFse1620Info* pFse1620Info, bool bCreer) ;
   bool InitDataFse1630(NSFse1630Info* pFse1630Info, bool bCreer) ;
   bool InitDataFseCCAM(NSFseCCAMInfo* pFseCCAMInfo, bool bCreer) ;   bool InitDataFseLibre(NSFseLibreInfo* pFseLibreInfo, bool bCreer) ;   bool InitDataFseMat() ;   bool InitDataFact(NSFactInfo* pFactInfo, bool bCreer) ;
   bool InitDataTP(NSTPayantInfo* pTPInfo, bool bCreer) ;

   // stockage des tableaux dans les bases respectives
   bool EnregFseArray(string sNumCompt);
   bool EnregFactArray(string sNumCompt);
   bool EnregTPArray(string sNumCompt);
   void EnregDonneesCompt(string sNumCompt);

   void Export(string sAction, string sNumCompt) ;

   // méthodes de compta
   void AfficheSommeDue() ;
   void AfficheDepass() ;
   void SwitchFFEuro() ;
   void AffichePaye() ;
   void MajDateC() ;
   void MajHeureC() ;
   void RetrouveCorresp(string& sLibelle) ;
   void DecrementeResteDu(char* codeOrga, char* montant, char* unite) ;
   void IncrementeResteDu(char* codeOrga, char* montant, char* unite) ;
   int  SommeResteDu() ;
   bool ExisteExamen(const string sCodePat, const string sDateExam, const string sCodeExam) ;

   // méthodes appelées sur les boutons ou double-click
   void Cm1610();
   void Cm1620();
   void Cm1630();
   void CmCCAM();   void CmLibre();   void CmMat();   void CmModifFse();
   void CmModifFact();
   void CmModifTP();
   void CmPaiement();
   void CmCorresp();
   void CmTiersPayant();
   void CmPaiementTiersPayant(CreerTPayantDialog* pTPDlg);
   void CmCodeConsult();

   void CmOk();
   void CmCancel();

   bool setExamCode() ;

   void addToFseArray(NSBlocFse16 *pBloc) ;
   void removeFromFseArray(NSFse16Iter it) ;

	DECLARE_RESPONSE_TABLE(CreerFicheComptDialog);
};

// classe NSUtilExamen pour pouvoir lancer une alerte quand on saisit le même examen
///////////////////////////////////////////////////////////////////////////////////////
class _CLASSELEXI NSUtilExamen : public NSUtilLexique
{
	public :

    CreerFicheComptDialog* pDialog ;

    NSUtilExamen(NSContexte *pCtx, CreerFicheComptDialog* pDlg, int resId, NSDico* pDico) ;
    ~NSUtilExamen() ;

    void ElementSelectionne() ;
};

//
// Dialogue de la fiche FACT
//
class _CLASSELEXI CreerFicheFactDialog : public NSUtilDialog
{
	public:

		CreerFicheComptDialog* pDlg ;
   	NSFactData*				pData ;
   	NSComboArray*			pModePaieArray ;
   	NSCodeOrgaArray*	pCodeOrgaArray ;
   	int						    nbCodeOrga ;
   	NSTPArray* 				pTPArray ;
   	NSVarCompta*		  pVar ;

   	// variables de saisie
   	NSUtilEditDate*	   pDate ;
   	OWL::TComboBox*	   pOrga ;
   	NSUtilEditSomme*	 pMontant ;
   	OWL::TGroupBox*		 pUnite ;
   	OWL::TRadioButton* pLocal ;
   	OWL::TRadioButton* pEuro ;
   	OWL::TComboBox*		 pModePaie ;

   	CreerFicheFactDialog(CreerFicheComptDialog* pere, NSContexte* pCtx, NSVarCompta* pVarCompt) ;
   	~CreerFicheFactDialog() ;

   	void SetupWindow() ;
   	bool InitCodeOrgaArray() ;
   	void AfficheResteDuPatient() ;
   	void AfficheResteDuOrga(string sCodeOrga) ;
    void EvOrgaChange() ;

   	void CmOk() ;
   	void CmCancel() ;
    void CmImprimer() ;

	DECLARE_RESPONSE_TABLE(CreerFicheFactDialog) ;
};

//// Dialogue de la fiche TPAYANT
//
class _CLASSELEXI CreerTPayantDialog : public NSUtilDialog
{
	public:

    CreerFicheComptDialog* pComptDlg ;

    NSTPayantData*     pData ;
    NSCodeOrgaArray*   pCodeOrgaArray ;
    int                nbCodeOrga ;
    NSVarCompta*       pVar ;

    // variables de saisie
    OWL::TComboBox*    pOrga ;
    NSUtilEditSomme*   pResteDu ;
    OWL::TGroupBox*		 pUnite ;
    OWL::TRadioButton* pLocal ;
    OWL::TRadioButton* pEuro ;

    CreerTPayantDialog(CreerFicheComptDialog* pere, NSContexte* pCtx, NSVarCompta* pVarCompt) ;
    ~CreerTPayantDialog() ;

    void SetupWindow() ;
    bool InitCodeOrgaArray() ;

    void CmPaiement() ;
    void DecrementeResteDu(char* codeOrga, char* montant, char* unite) ;
    void EvOrgaChange() ;
    bool SauveData() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(CreerTPayantDialog) ;
};

#endif // fin du fichier nsfsedlg.h

