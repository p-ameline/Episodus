//// Outil d'édition et de recherche des fils guides - RS Juillet 1999
//---------------------------------------------------------------------------

#ifndef __NSRECHCR_H#define __NSRECHCR_H

//---------------------------------------------------------------------------
#include <regexp.h>

#include <owl\dialog.h>#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>
#include <owl\gauge.h>

class NSEltArray ;
class NSUtilEdit ;
class NSUtilEdit2 ;
class NSUtilEditDate ;
class NSEditNum ;
class NSUtilLexique ;
class NSPersonsAttributesArray ;

#include "nsoutil\nsexport.h"

// #include "nsbb\nsednum.h"
#include "nsbb\nsutidlg.h"
// #include "nsoutil\nsfilgui.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nsperary.h"
#include "partage\nsperson.h"
#include "pilot\NautilusPilot.hpp"

//
// Classe des critères patient
//
class _NSOUTILCLASSE NSCritReqPatient
{
	public :

    NSCritReqPatient() ;
    NSCritReqPatient(const NSCritReqPatient& rv) ;
    ~NSCritReqPatient() ;
    NSCritReqPatient& operator=(const NSCritReqPatient& src) ;

    bool   isValidNom(const string &sN) ;
    bool   isValidPrenom(const string &sN) ;
    bool   isValidSexe(const string &sN) ;
    bool   isValidDate(const string &sN) ;
    bool   isValidSitfam(const string &sN) ;
    bool   isValidCodePost(const string &sN) ;
    bool   isValidVille(const string &sN) ;

    void   reset() ;
    void   resetRegExp() ;
    void   initRegExp() ;
    void   metAZero() ;
    bool   estMasculin() { return ((string("") != sSexe) && ('1' == sSexe[0])) ; }
    bool   estFeminin()  { return ((string("") != sSexe) && ('2' == sSexe[0])) ; }
    void   metMasculin() { sSexe = string("1") ; }
    void   metFeminin()  { sSexe = string("2") ; }

    string getRegExp(string sInfo) ;

    string getNom()      { return sNom ; }
    string getPrenom()   { return sPrenom ; }
    string getSexe()     { return sSexe ; }
    string getDateN1()   { return sDateN1 ; }
    string getDateN2()   { return sDateN2 ; }
    string getSitfam()   { return sSitfam ; }
    string getCodePost() { return sCodePost ; }
    string getVille()    { return sVille ; }

    void   setNom(string sN)      { sNom = sN ; }
    void   setPrenom(string sN)   { sPrenom = sN ; }
    void   setSexe(string sN)     { sSexe = sN ; }
    void   setDateN1(string sN)   { sDateN1 = sN ; }
    void   setDateN2(string sN)   { sDateN2 = sN ; }
    void   setSitfam(string sN)   { sSitfam = sN ; }
    void   setCodePost(string sN) { sCodePost = sN ; }
    void   setVille(string sN)    { sVille = sN ; }

  protected:

    string sNom ;
    string sPrenom ;
    string sSexe ;
    string sDateN1 ;
    string sDateN2 ;
    string sSitfam ;
    string sCodePost ;
    string sVille ;

    TRegexp* pNameRegexp ;
    TRegexp* pScndNameRegexp ;
    TRegexp* pZipCodeRegexp ;
    TRegexp* pTownRegexp ;
};

//
// Dialogue de saisie des critères patient
//
class _NSOUTILCLASSE NSCritReqPatDialog : public NSUtilDialog
{
	public:

		NSCritReqPatient*  pData ;      // données à récupérer
		bool               bEffacer ;

		OWL::TGroupBox*		 pSexe ;
    int                iSexe ;
    OWL::TGroupBox*		 pSitFam ;
    int                iSitFam ;

    NSUtilEdit*        pNom ;
		NSUtilEdit*        pPrenom ;
    OWL::TRadioButton* pSexeM ;
    OWL::TRadioButton* pSexeF ;

    NSUtilEditDate* 	 pDateN1 ;
    NSUtilEditDate* 	 pDateN2 ;
    NSUtilEdit*		     pCodePost ;
    NSUtilEdit*    	   pVille ;

    OWL::TRadioButton* pCel ;
    OWL::TRadioButton* pVeu ;
    OWL::TRadioButton* pMar ;
    OWL::TRadioButton* pCcb ;
    OWL::TRadioButton* pDiv ;
    NSEditNum*			   pNbEnf ;

		NSCritReqPatDialog(TWindow* pere, NSContexte* pCtx, TModule* mod = 0) ;
		~NSCritReqPatDialog() ;

    void SetupWindow() ;
    void CmSexe() ;
    void CmSitFam() ;
    void CmEffacer() ;
    void CmOk() ;
    void CmCancel() ;
    void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSCritReqPatDialog) ;
};

//
// Classe des critères document
//
class _NSOUTILCLASSE NSCritReqDocum
{
	public :

		//
    // Variables de stockage des valeurs
    //
    string sCodeAuteur ;
    string sTitreAuteur ;    // uniquement pour affichage
    string sCodeRoot ;
    string sDate1 ;
    string sDate2 ;

    NSCritReqDocum() ;
    ~NSCritReqDocum() ;
    NSCritReqDocum& operator=(const NSCritReqDocum& src) ;

    void metAZero() ;
};

//
// Dialogue de saisie des critères document
//
class _NSOUTILCLASSE NSCritReqDocDialog : public NSUtilDialog{
	public:

		string          sCodeAuteur ;
		string          sTitreAuteur ;
		NSCritReqDocum* pData ;      // données à récupérer
		bool            bEffacer ;

		TStatic*        pAuteur ;
    NSUtilLexique*	pCodeRoot ;
    NSUtilEditDate* pDate1 ;
    NSUtilEditDate* pDate2 ;

		NSCritReqDocDialog(TWindow* pere, NSContexte* pCtx, TModule* mod = 0) ;
		~NSCritReqDocDialog() ;

    void SetupWindow() ;
    void CmAuteur() ;
    void CmEffacer() ;
    void CmOk() ;
    void CmCancel() ;
    void EvClose() ;

	DECLARE_RESPONSE_TABLE(NSCritReqDocDialog) ;
};

//
// Object that materializes research results for a given boolean sentence
//
class _NSOUTILCLASSE NSRequestResult
{
  private:

 	  static long lObjectCount ;

	public :

		string          _sRequestName ;
		string          _sRequestBooleanLabel ;

  	NSDocHistoArray _aVectDocumentResults ;
		NSPatientArray  _aVectPatientResults ;

    int             _nbPatResult ;
    int             _nbDocResult ;

    bool            _bNouveauPatResult ;

  	NSRequestResult() {}
    NSRequestResult(const NSRequestResult& src) ;
		~NSRequestResult() {}

		string getRequestName()  { return _sRequestName ; }
    string getRequestLabel() { return _sRequestBooleanLabel ; }

    void   setRequestName(string sRN)  { _sRequestName = sRN ; }
    void   setRequestLabel(string sRL) { _sRequestBooleanLabel = sRL ; }

    void   reinitForNewSearch() ;
    void   copyValues(const NSRequestResult* pSrc) ;

    NSRequestResult& operator=(const NSRequestResult& src) ;

    static long getNbInstance()	 { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

typedef vector<NSRequestResult*>      NSReqResArray ;
typedef NSReqResArray::iterator       NSRequestResultIter ;
typedef NSReqResArray::const_iterator NSRequestResultConstIter ;
typedef NTArray<NSRequestResult>      NSRequestResultArray ;

//
//  Outil de gestion des requetes
//
class _NSOUTILCLASSE NSRequeteWindow ;
class _NSOUTILCLASSE NSBooleanRequestsWindow ;
class _NSOUTILCLASSE NSLanceReqDialog ;

class _NSOUTILCLASSE NSRequeteDialog : public NSUtilDialog
{
	public :

		bool                _bDirty ;

    NSEltArray*         _pChemArray ;
    int                 _nbChem ;
    map<string, string> _requests ;

    NSRequestResultArray _aRequestResults ;

    string              _sLibReq ;
    NSDocHistoArray     _VectDocResultat ;    // vecteur des documents résultat
    NSPatientArray      _VectPatResultat ;    // vecteur des patients résultat

    NSUtilEdit2*             _pNomReq ;
    NSRequeteWindow*         _pListeChem ;
    NSBooleanRequestsWindow* _pRequestsList ;

    NSUtilEdit2*        _pEditReqLabel ;    // Boite de saisie du nom de la phrase logique
    NSUtilEdit2*        _pEditReq ;         // Boite de saisie de la phrase logique

    OWL::TGroupBox*		  _pMode ;
    OWL::TRadioButton*	_pModePat ;
    OWL::TRadioButton*  _pModeDoc ;
    string              _sFileName ;
    bool                _bReqModeDoc ;
    bool                _bReqEnCours ;
    int                 _nbPatTotal ;
    int                 _nbPatCritPat ;
    int                 _nbPatCritDoc ;
    int                 _nbPatResult ;
    int                 _nbDocCritPat ;
    int                 _nbDocCritDoc ;
    int                 _nbDocResult ;
    NSDocumentHisto     _Document ;           // Document en cours pour la recherche
    NSPatInfo           _Patient ;            // Patient en cours pour la recherche

    OWL::TCheckBox*		  _pCritPatBox ;
    OWL::TCheckBox*    	_pCritDocBox ;
    bool                _bCritPat ;
    bool                _bCritDoc ;
    NSCritReqPatient*   _pCritPat ;
    NSCritReqDocum*     _pCritDoc ;
    bool                _bCherchePatient ;
    bool                _bChercheDocument ;

    NSLanceReqDialog*         _pLanceur ;
    NSPersonsAttributesArray* _pPersonList ;
    int                       _iPersonListSize ;
    int                       _iCurrentPatient ;
    NSDocHistoArray           _VectDocument ;

		NSRequeteDialog(TWindow* pere, NSContexte* pCtx) ;
		~NSRequeteDialog() ;

		void SetupWindow() ;

    void InitListeChem() ;
    void AfficheListeChem() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;

    void InitSearchList() ;
    void DisplaySearchList() ;
    void LvnGetSearchDispInfo(TLwDispInfoNotify& dispInfo) ;

    void TrouveLibElement(string sElement, string& sLibElement) ;
    bool InitChemArray() ;
    bool InitPersonList() ;
    bool ChargeGraphePatient() ;

    int  CherchePatientSuivant(NSPersonsAttributeIter& iterPatient, NSPatInfo* pPatInfo) ;
    int  ChercheDocumentSuivant(DocumentIter& iterDoc, NSDocumentHisto* pDocHisto) ;
    void ReinitResults(bool bModeDoc = true) ;
    bool CheminDansDocument(int numChemin) ;
    int  Interprete(int numReq) ;
    int  Interprete(string sReq, size_t& cc) ;
    bool PatientVerifieCriteres() ;
    bool DocumentVerifieCriteres() ;
    bool ValeurEquivalente(string sVal1, string sVal2) ;
    void EcrireRequete(string& sOut) ;

    bool ChargerRequete(string sFichier) ;
    bool ChargerRequeteCritPat(string sLib) ;
    bool ChargerRequeteCritDoc(string sLib) ;
    bool ChargerRequeteCritLib(string sLib, string sVerNum) ;
    bool ChargerRequeteCritReq(string sLib) ;

    void CmNouveau() ;
    void CmOuvrir() ;
    void CmEnregistrer() ;
    void CmNewReq() ;
    void CmModReq() ;
    void CmDelReq() ;
    void CmDupReq() ;
    void CmCriteresPatient() ;
    void CmCriteresDocument() ;
    void CmOuvrante() ;
    void CmET() ;
    void CmOU() ;
    void CmFermante() ;
    void CmInsererChemin() ;
    void CmModePat() ;
    void CmModeDoc() ;
    void CmLancement() ;
    void CmLancer() ;
    void CmFermer() ;

    void addToReq(string sCompl) ;

    void CmAddRequest() ;
    void CmEditRequest() ;
    void CmDeleteRequest() ;
    void CmDuplicateRequest() ;
    void CmPutRequestInEditControl() ;

    bool useCritPat() { return _bCritPat ; }
    bool useCritDoc() { return _bCritDoc ; }
    bool useModeDoc() { return _bReqModeDoc ; }

    bool isRunning()  { return _bReqEnCours ; }

    int getNbPatTotal()   { return _nbPatTotal ;   }
    int getNbPatCritPat() { return _nbPatCritPat ; }
    int getNbPatCritDoc() { return _nbPatCritDoc ; }
    int getNbPatResult()  { return _nbPatResult ;  }
    int getNbDocCritPat() { return _nbDocCritPat ; }
    int getNbDocCritDoc() { return _nbDocCritDoc ; }
    int getNbDocResult()  { return _nbDocResult ;  }

    NSDocHistoArray*      getVectDocResultat() { return &_VectDocResultat ; }
    NSPatientArray*       getVectPatResultat() { return &_VectPatResultat ; }
    NSRequestResultArray* getRequestResults()  { return &_aRequestResults ; }

    void setPatientSearch(bool bPS)  { _bCherchePatient  = bPS ; }
    void setDocumentSearch(bool bDS) { _bChercheDocument = bDS ; }

    string getFileName()             { return _sFileName ; }
    void   setFileName(string sFN)   { _sFileName = sFN ; }

    NSCritReqPatient* getCritPat()   { return _pCritPat ; }
    NSCritReqDocum*   getCritDoc()   { return _pCritDoc ; }

    NSPatInfo* getResultPatient(size_t iIndex) ;

	DECLARE_RESPONSE_TABLE(NSRequeteDialog) ;};

//
// Objet "ListWindow" avec gestion du double-click
//
class _NSOUTILCLASSE NSRequeteWindow : public TListWindow
{
	public:

		NSRequeteDialog* pDlg ;

    NSRequeteWindow(NSRequeteDialog* pere, int resId) ;
    ~NSRequeteWindow() ;

		void SetupWindow() ;    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    int  IndexItemSelect() ;
	DECLARE_RESPONSE_TABLE(NSRequeteWindow) ;};

//
// Objet "ListWindow" avec gestion du double-click
//
class _NSOUTILCLASSE NSBooleanRequestsWindow : public TListWindow
{
	public:

		NSRequeteDialog* pDlg ;

    NSBooleanRequestsWindow(NSRequeteDialog* pere, int resId) ;
    ~NSBooleanRequestsWindow() ;

		void SetupWindow() ;    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;

    int  IndexItemSelect() ;
	DECLARE_RESPONSE_TABLE(NSBooleanRequestsWindow) ;};

//// Dialogue de lancement de la requête
//
class _NSOUTILCLASSE NSLanceReqDialog : public NSUtilDialog
{
	public:

  	NSRequeteDialog* 	pReqDlg;      // données à récupérer

    OWL::TGroupBox*		 pCritGroup ;
    OWL::TRadioButton* pCritLib ;
    OWL::TRadioButton* pCritPat ;
    OWL::TRadioButton* pCritDoc ;
    OWL::TStatic*      pStatus ;
    OWL::TGauge*       pProgress ;
    NSUtilEdit*        pEditPat ;
    NSUtilEdit*        pEditDoc ;
    bool               bAvecLib ;
    bool               bAvecPat ;
    bool               bAvecDoc ;

		NSLanceReqDialog(NSRequeteDialog* pere, NSContexte* pCtx, TModule* mod = 0) ;
		~NSLanceReqDialog() ;

    void SetupWindow() ;
    void CmCheckLib() ;
    void CmCheckPat() ;
    void CmCheckDoc() ;
    void CmDemarrer() ;
    void CmArreter() ;
    void EvClose() ;

    void displayStatusMessage(string sMessage) ;
    void displayProgress(int iProgressPercent) ;

	DECLARE_RESPONSE_TABLE(NSLanceReqDialog) ;
};

//// Classe NSEditReqDialog
//
class NSEditReqWindow;

class NSEditReqDialog : public NSUtilDialog
{
	public:

    NSEditReqWindow* pListePere ;
    NSEditReqWindow* pListeFils ;
    NSEltArray*      pPereArray ;
    NSEltArray*      pFilsArray ;
    int					     nbPere ;
    int					     nbFils ;
    string           sCheminPere ;        // pour pouvoir récupérer le nouveau chemin
    string           sCheminRechPere ;    // le chemin est ici un chemin de recherche

    NSEditReqDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~NSEditReqDialog() ;

    void SetupWindow() ;
    void InitListePere() ;
    void AfficheListePere() ;
    // void LvnGetDispInfo(TLwDispInfoNotify& dispInfo);
    void InitListeFils() ;
    void AfficheListeFils() ;
    // void LvnGetDispInfo(TLwDispInfoNotify& dispInfo);
    void TrouveLibelle(string sCode, string& sLibelle, bool bPere) ;
    void TrouveLibelleValeur(const string sCode, string& sLibelle, bool bPere) ;
    void ParsingChemin(string sChem, bool bPere, string sDecal = "+01+01") ;
    void ComposeChemin(string& sChemin, NSEltArray* pArray, bool bRequete = false) ;
    void ComposeCheminBrut(string& sChemin, NSEltArray* pArray) ;
    void RechercheFilsAmmorce(string sAmmorce, string& sFils, string& sDecal) ;
    bool CalculeValeur(string& sCode) ;

    // Controles
    void CmInserer() ;
    void CmModifier() ;
    void CmDetruire() ;
    void CmAjouter() ;

    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSEditReqDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click
//
class NSEditReqWindow : public TListWindow
{
	public:

		NSEditReqDialog* pDlg ;
		int              iRes ;

    NSEditReqWindow(NSEditReqDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
      iRes = resId ;
    }
    ~NSEditReqWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSEditReqWindow) ;
};

//
// Classe ChercheListeUtilDialog -> pour la sélection d'un utilisateur
//
class NSReqListUtilWindow;

class NSReqListUtilDialog : public NSUtilDialog
{
	public:

		NSReqListUtilWindow* pListeUtil ;
    NSUtiliArray*		     pUtilArray ;
    NSUtiliInfo*         pUtilSelect ;

    int					         nbUtil ;
    int					         UtilChoisi ;

    NSReqListUtilDialog(TWindow* pere, NSContexte* pCtx, TModule* module = 0) ;
    ~NSReqListUtilDialog() ;

    void SetupWindow() ;
    bool InitUtilArray() ;
    void InitListe() ;
    void AfficheListe() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;

    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSReqListUtilDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les utilisateurs)
//
class NSReqListUtilWindow : public TListWindow
{
	public:

		NSReqListUtilDialog* pDlg ;

    NSReqListUtilWindow(NSReqListUtilDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
    }
    ~NSReqListUtilWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSReqListUtilWindow) ;
};

//
// Dialogue de saisie des valeurs chiffrées
//
class NSValeurChiffreeDialog : public NSUtilDialog
{
	public:

		NSUtilEdit*		     pValeurExacte ;	//valeur exacte
    NSUtilEdit*		     pValeurSup ;		//valeur sup
    NSUtilEdit*		     pValeurInf ;		//valeur inf
    NSUtilLexique*	   pUnite ;
    OWL::TRadioButton* pAvecInf ;
    OWL::TRadioButton* pAvecSup ;
    string			       sValeurExacte ;
    string			       sValeurSup ;
    bool               bAvecSup ;
    string			       sValeurInf ;
    bool               bAvecInf ;
    string			       sUnite ;
    int 			         MaxInPut ;

		NSValeurChiffreeDialog(TWindow* pere, int MaxEntree, NSContexte* pCtx,
                                TModule* module = 0) ;
		~NSValeurChiffreeDialog() ;

    void SetupWindow() ;
    void CmAvecInf() ;
    void CmAvecSup() ;
    void CmEffacer() ;
    void CmOk() ;
		void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSValeurChiffreeDialog) ;};
#endif
