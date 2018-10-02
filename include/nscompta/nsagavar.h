// NSAgaVar : Fichier des variables nécéssaires à l'impression des aga// Rémi SPAAK Octobre 98
////////////////////////////////////////////////////////////////////////

#ifndef __NSAGAVAR_H
#define __NSAGAVAR_H

#include "nscompta\nscpta.h"     // classes NSCompt et NSFact
#include "partage\nsperson.h"	// pour le patient de FicheCompt et de ListCompt
#include "nscompta\nsdepens.h"  // pour le libelle des recettes (cf aga)
#if defined(_DANSCPTADLL)	#define _CLASSECPTA __export
#else
	#define _CLASSECPTA __import
#endif

class NSComboItem
{
  public :

    string sCode ;
    string sLabel ;

    NSComboItem()  {}
    NSComboItem(const NSComboItem& rv) { sCode = rv.sCode; sLabel = rv.sLabel; }

    ~NSComboItem() {}
};

//// Définition de NSComboArray (Array de NSComboItem(s))
//
typedef vector<NSComboItem*> NSComboBoxArray ;
typedef NSComboBoxArray::iterator       NSComboIter ;
typedef NSComboBoxArray::const_iterator NSComboConstIter ;

class NSComboArray : public NSComboBoxArray
{
  public :

	  // Constructeurs
	  NSComboArray() : NSComboBoxArray() {}
	  NSComboArray(const NSComboArray& rv) ;
	  // Destructeur
	  virtual ~NSComboArray() ;
    void vider() ;
};

//// Fonction globale pour l'initialisation des ComboBox
// a partir d'un fichier .dat
//
bool InitComboBox(OWL::TComboBox* pCombo, NSComboArray* pComboArray, string sFichier);

//
// Variables globales de compta
//
class _CLASSECPTA NSVarCompta
{
  public :

    double _parite ;
    int    _monnaieRef ;
    string _sigle ;
    string _indiceConsult ;
    string _sDateC ;
    string _sHeureC ;
    bool   _bCreation ;
    bool   _bFact0 ;

    NSVarCompta(NSContexte* pCtx) ;
    NSVarCompta(const NSVarCompta& rv) ;
    NSVarCompta& operator=(const NSVarCompta& src) ;
    ~NSVarCompta() {}

    double getParite()        { return _parite ; }
    int    getMonnaieRef()    { return _monnaieRef ; }
    string getSigle()         { return _sigle ; }
    string getIndiceConsult() { return _indiceConsult ; }
    string getDateC()         { return _sDateC ; }
    string getHeureC()        { return _sHeureC ; }
    bool   isCreation()       { return _bCreation ; }
    bool   isFact0()          { return _bFact0 ; }

    void   setDateC(string sD)  { _sDateC  = sD ; }
    void   setHeureC(string sH) { _sHeureC = sH ; }
    void   setCreation(bool bC) { _bCreation = bC ; }
    void   setFact0(bool bF)    { _bFact0 = bF ; }

    string getMonnaieSigle() ;

    int    getEuroFromLocal(int iCents) ;
    int    getLocalFromEuro(int iCents) ;
};

#define AGA_ACTES_LEN 100#define AGA_DATE_PAIE_LEN 10
//
// NSAgaData dérive de NSRoot à cause de patInfo
//
class _CLASSECPTA NSAgaData : public NSRoot
{
	public :

		char nomPatient[PAT_NOM_LONG_LEN + 1];
		// libelle recette si pas de patient pour l'aga
    char libelle[REC_LIBELLE_LEN + 1];
    char numCompt[FACT_NUMCOMPT_LEN + 1];
    char actes[AGA_ACTES_LEN];
    char montant[FACT_MONTANT_LEN + 1];
    char unite[FACT_UNITE_LEN + 1];
    char modePaie[FACT_MODE_PAIE_LEN + 1];
    char datePaie[AGA_DATE_PAIE_LEN + 1];

    // NSPatInfo      patInfo;
    NSPersonInfo   patInfo ;
    NSEcritureData ecriture ;
    NScptsLibsData cpt_lib ;

    NSAgaData(NSContexte* pCtx) ;
    NSAgaData(const NSAgaData& rv) ;
    ~NSAgaData() ;

    NSAgaData& operator=(const NSAgaData& src) ;
    int        operator==(const NSAgaData& o) ;

    void metAZero();
};

//// Définition de NSAgaArray (Array de NSAgaData(s))
//
typedef vector<NSAgaData*> NSFicheAgaArray ;
typedef NSFicheAgaArray::iterator       NSAgaIter ;
typedef NSFicheAgaArray::const_iterator NSAgaConstIter ;

class _CLASSECPTA NSAgaArray : public NSFicheAgaArray
{
  public :

    // Constructeurs
    NSAgaArray() : NSFicheAgaArray() {}
    NSAgaArray(const NSAgaArray& rv) ;
    // Destructeur
    virtual ~NSAgaArray() ;
    void vider() ;
};

//// Classe Criteres des AGA
//
class _CLASSECPTA NSCriteres
{
	public :

		string _sDateAga1 ;
    string _sDateAga2 ;
    bool   _bActesPerso ;

    NSCriteres() ;
    NSCriteres(const NSCriteres& rv) ;
    NSCriteres& operator=(const NSCriteres& src) ;
    ~NSCriteres() ;

    string getDateAga1()  { return _sDateAga1 ; }
    string getDateAga2()  { return _sDateAga2 ; }
    bool   isActesPerso() { return _bActesPerso ; }

    void   setDateAga1(string sD) { _sDateAga1 = sD ; }
    void   setDateAga2(string sD) { _sDateAga2 = sD ; }
    void   setActesPerso(bool bA) { _bActesPerso = bA ; }
};

//// Classe NSAffDepensData
//

class _CLASSECPTA NSAffDepensData{	public :

    NSDepensData   _depense ;    NSEcritureData _ecriture ;    NScptsLibsData _cpt_lib ;    NSAffDepensData() ;
    NSAffDepensData(const NSAffDepensData& rv) ;

    ~NSAffDepensData() ;
    NSAffDepensData& operator=(const NSAffDepensData& src) ;    int              operator==(const NSAffDepensData& o) ;

    void metAZero() ;};

//// Définition de NSAffDepensArray (Array de NSAffDepensData(s))
//

typedef vector<NSAffDepensData*> NSFicheAffDepensArray ;typedef NSFicheAffDepensArray::iterator       NSAffDepensIter ;
typedef NSFicheAffDepensArray::const_iterator NSAffDepensConstIter ;

class _CLASSECPTA NSAffDepensArray : public NSFicheAffDepensArray{
	public :

		// Constructeurs		NSAffDepensArray() : NSFicheAffDepensArray() {}
		NSAffDepensArray(const NSAffDepensArray& rv) ;

		// Destructeur		virtual ~NSAffDepensArray() ;
    void vider() ;
};
#define IMP_LIB_EXAM_LEN 	40
//// NSImpData dérive de NSRoot à cause de patInfo
//
class _CLASSECPTA NSImpData : public NSRoot
{
	public :

   	char numCompt[CPTA_NUMCOMPT_LEN + 1] ;
   	char dateCompt[CPTA_DATE_LEN + 1] ;
    char codePatient[PAT_NSS_LEN + 1] ;
		// char nomPatient[PAT_NOM_LONG_LEN + 1] ;
   	// char libExam[IMP_LIB_EXAM_LEN + 1] ;
   	char sommeDueLoc[CPTA_DUE_F_LEN + 1] ;
   	char sommeDueEuro[CPTA_DUE_E_LEN + 1] ;
   	char impayeLoc[TPAY_RESTE_DU_LEN + 1] ;
   	char impayeEuro[TPAY_RESTE_DU_LEN + 1] ;
   	char libOrga[TPAY_LIBELLE_LEN + 1] ;

    string sNomPatient ;
    string sLibExam ;

    bool bPatientLoaded ;

   	// NSPatInfo patInfo ;
    NSPersonInfo patInfo ;

		NSImpData(NSContexte* pCtx) ;
   	NSImpData(const NSImpData& rv) ;
   	~NSImpData() ;

    bool initPatientInfo() ;

   	NSImpData& operator=(const NSImpData& src) ;
   	int        operator==(const NSImpData& o) ;
   	void metAZero() ;
};

//// Définition de NSImpArray (Array de NSImpData(s))
//
typedef vector<NSImpData*> NSFicheImpArray ;
typedef NSFicheImpArray::iterator       NSImpIter ;
typedef NSFicheImpArray::const_iterator NSImpConstIter ;

class _CLASSECPTA NSImpArray : public NSFicheImpArray
{
	public :

		// Constructeurs
		NSImpArray() : NSFicheImpArray() {}
		NSImpArray(const NSImpArray& rv) ;
		// Destructeur
		virtual ~NSImpArray() ;
		void vider() ;
    void initAllPatientInfo() ;
};

class _CLASSECPTA NSSomActData{
  public :

    int _iTotalLoc ;
		int _iTotalEuro ;

   	int depassLoc ;
   	int depassEuro ;
   	int paieLoc ;
   	int paieEuro ;
   	int espLoc ;
   	int espEuro ;
   	int chqLoc ;
   	int chqEuro ;
   	int virLoc ;
   	int virEuro ;
   	int cbLoc ;
   	int cbEuro ;
   	int impayeLoc ;
   	int impayeEuro ;
   	int impTPLoc ;
   	int impTPEuro ;
   	int impAutreLoc ;
   	int impAutreEuro ;

    int _iTotalAvecDepassLoc ;
		int _iTotalAvecDepassEuro ;

    int _iNbTotal ;
    int _iNbTotalAvecDepass ;

   	NSSomActData() ;
   	NSSomActData(const NSSomActData& rv) ;
   	~NSSomActData() ;

   	NSSomActData& operator=(const NSSomActData& src) ;

    string GetCountsSentence() ;
    string GetDepassSentence() ;
};

class NSTotauxData
{
	public :

		string sLibelle ;
   	int    montantLoc ;
   	int    montantEuro ;

		NSTotauxData() ;
   	NSTotauxData(const NSTotauxData& rv) ;
   	~NSTotauxData() ;

   	NSTotauxData& operator=(const NSTotauxData& src) ;
};

//// Définition de NSTotauxArray (Array de NSTotauxData(s))
//
typedef vector<NSTotauxData*> NSFicheTotauxArray ;
typedef NSFicheTotauxArray::iterator       NSTotauxIter ;
typedef NSFicheTotauxArray::const_iterator NSTotauxConstIter ;

class _CLASSECPTA NSTotauxArray : public NSFicheTotauxArray
{
	public :

		// Constructeurs
		NSTotauxArray() : NSFicheTotauxArray() {}
		NSTotauxArray(const NSTotauxArray& rv) ;
		// Destructeur
		virtual ~NSTotauxArray() ;
   	void vider() ;
};

class NSKCodeData{
  public :

    string sKCode ;
    double occur ;

    NSKCodeData() ;
    NSKCodeData(const NSKCodeData& rv) ;
    ~NSKCodeData() ;

    NSKCodeData& operator=(const NSKCodeData& src) ;
    int 			   operator==(const NSKCodeData& o) ;
};

//// Définition de NSKCodeArray (Array de NSKCodeData(s))
//
typedef vector<NSKCodeData*> NSFicheKCodeArray ;
typedef NSFicheKCodeArray::iterator       NSKCodeIter ;
typedef NSFicheKCodeArray::const_iterator NSKCodeConstIter ;

class _CLASSECPTA NSKCodeArray : public NSFicheKCodeArray
{
	public :

		// Constructeurs
		NSKCodeArray() : NSFicheKCodeArray() {}
		NSKCodeArray(const NSKCodeArray& rv) ;
		// Destructeur
		virtual ~NSKCodeArray() ;

		NSKCodeArray& operator=(const NSKCodeArray& src) ;
  	int 			    operator==(const NSKCodeArray& o) ;

		void vider() ;
} ;

class NSCCAMCodeData
{
	public :

		string _sCCAMCode ;
   	string _sCCAMlib ;
   	double _dNbre ;
   	double _dSomTotal ;

   	NSCCAMCodeData() ;
   	NSCCAMCodeData(const NSCCAMCodeData& rv) ;
   	~NSCCAMCodeData() ;

    string getCode()  const { return _sCCAMCode ; }
   	string getLabel() const { return _sCCAMlib ;  }
   	double getNbre()  const { return _dNbre ;     }
   	double getSum()   const { return _dSomTotal ; }

    void   setCode(string sC)  { _sCCAMCode = sC ; }
   	void   setLabel(string sL) { _sCCAMlib  = sL ; }
   	void   setNbre(double dN)  { _dNbre     = dN ; }
   	void   setSum(double dS)   { _dSomTotal = dS ; }

    void   addToNbre(double dN) { _dNbre     += dN ; }
   	void   addToSum(double dS)  { _dSomTotal += dS ; }

   	NSCCAMCodeData& operator=(const NSCCAMCodeData& src) ;
   	int 			      operator==(const NSCCAMCodeData& o) ;
};

//// Définition de NSKCodeArray (Array de NSKCodeData(s))
//
typedef vector<NSCCAMCodeData*> NSFicheCCAMCodeArray ;
typedef NSFicheCCAMCodeArray::iterator       NSCCAMCodeIter ;
typedef NSFicheCCAMCodeArray::const_iterator NSCCAMCodeConstIter ;

class _CLASSECPTA NSCCAMCodeArray : public NSFicheCCAMCodeArray
{
	public :

		// Constructeurs
		NSCCAMCodeArray() : NSFicheCCAMCodeArray() {}
		NSCCAMCodeArray(const NSCCAMCodeArray& rv) ;
		// Destructeur
		virtual ~NSCCAMCodeArray() ;

		NSCCAMCodeArray& operator=(const NSCCAMCodeArray& src) ;
  	int 			       operator==(const NSCCAMCodeArray& o) ;

		void vider() ;
} ;

class _CLASSECPTA NSExamData
{
	public :

		string       sCodeExam ;
   	int      		 nbExam ;
   	NSKCodeArray aKCodeArray ;

   	NSExamData() ;
   	NSExamData(const NSExamData& rv) ;
   	~NSExamData() ;

   	NSExamData& operator=(const NSExamData& src) ;
   	int 			  operator==(const NSExamData& o) ;
};

//// Définition de NSExamArray (Array de NSExamData(s))
//
typedef vector<NSExamData*> NSFicheExamArray ;
typedef NSFicheExamArray::iterator       NSExamIter ;
typedef NSFicheExamArray::const_iterator NSExamConstIter ;

class _CLASSECPTA NSExamArray : public NSFicheExamArray
{
	public :

		// Constructeurs
		NSExamArray() : NSFicheExamArray() {}
		NSExamArray(const NSExamArray& rv) ;
		// Destructeur
		virtual ~NSExamArray() ;
   	void vider() ;
};

//// NSListActData dérive de NSRoot à cause de patInfo
//
class _CLASSECPTA NSListActData : public NSRoot
{
	public :

  	char numCompt[CPTA_NUMCOMPT_LEN + 1] ;
   	char dateCompt[CPTA_DATE_LEN + 1] ;
		char nomPatient[PAT_NOM_LONG_LEN + 1] ;
   	char libExam[IMP_LIB_EXAM_LEN + 1] ;
   	char actes[AGA_ACTES_LEN] ;

   	// NSPatInfo 		patInfo;
    NSPersonInfo patInfo ;

   	NSListActData(NSContexte* pCtx) ;
   	NSListActData(const NSListActData& rv) ;
   	~NSListActData() ;

   	NSListActData& operator=(const NSListActData& src) ;
   	int            operator==(const NSListActData& o) ;

   	void metAZero() ;
};

//
// Définition de NSListActArray (Array de NSListActData(s))
//
typedef vector<NSListActData*> NSFicheListActArray ;
typedef NSFicheListActArray::iterator       NSListActIter ;
typedef NSFicheListActArray::const_iterator NSListActConstIter ;

class _CLASSECPTA NSListActArray : public NSFicheListActArray
{
	public :

		// Constructeurs
		NSListActArray() : NSFicheListActArray() {}
		NSListActArray(const NSListActArray& rv) ;
		// Destructeur
		virtual ~NSListActArray() ;
   	void vider() ;
};

class _CLASSECPTA NSEncaissData
{
	public :

		int	paieLoc ;
   	int	paieEuro ;
   	int	espLoc ;
   	int	espEuro ;
   	int	chqLoc ;
   	int	chqEuro ;
   	int	virLoc ;
   	int	virEuro ;
   	int	cbLoc ;
   	int	cbEuro ;

   	NSEncaissData() ;
   	NSEncaissData(const NSEncaissData& rv) ;
   	~NSEncaissData() ;

   	NSEncaissData& operator=(const NSEncaissData& src) ;
   	int            operator==(const NSEncaissData& o) ;
};

//
// Définition de NSEncaissArray (Array de NSEncaissData(s))
//
typedef vector<NSEncaissData*> NSFicheEncaissArray ;
typedef NSFicheEncaissArray::iterator       NSEncaissIter ;
typedef NSFicheEncaissArray::const_iterator NSEncaissConstIter ;

class _CLASSECPTA NSEncaissArray : public NSFicheEncaissArray{
	public :

		// Constructeurs
		NSEncaissArray() : NSFicheEncaissArray() {}
		NSEncaissArray(const NSEncaissArray& rv) ;
		// Destructeur
		virtual ~NSEncaissArray() ;
   	void vider() ;
};

//// Classe NSMultiCriteres
//
class _CLASSECPTA NSMultiCriteres
{
	public :

  	bool	 bActesPerso ;
   	string sDate1 ;
   	string sDate2 ;
   	string sCodeExamen ;
   	string sSynExamen ;
   	string sCodePrescript ;
   	string sTitrePrescript ;
   	int    iImpayes ;
   	int    iContexte ;
   	string sCodeOrga ;
   	string sLibCourtOrga ;
   	string sLibLongOrga ;

    bool   bInterruptedProcess ;

   	NSMultiCriteres() ;
   	NSMultiCriteres(const NSMultiCriteres& rv) ;
   	NSMultiCriteres& operator=(const NSMultiCriteres& src) ;
   	~NSMultiCriteres() ;

    string getSummaryString(NSContexte* pCtx) ;
};

#endif // fin du fichier nsagavar.h