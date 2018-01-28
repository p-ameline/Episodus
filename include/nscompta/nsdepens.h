//---------------------------------------------------------------------------//     NSDEPENS.H
//
//  Contient les définitions des bases Depenses et Recettes
//
//---------------------------------------------------------------------------

#ifndef __NSDEPENS_H#define __NSDEPENS_H

#include <cstring.h>
#include <owl\edit.h>
#include <owl\checkbox.h>#include <owl\groupbox.h>#include <owl\listbox.h>#include <owl\combobox.h>
#include "ns_sgbd\nsfiche.h"#include "nsoutil\nsoutdlg.h"
#if defined(_DANSCPTADLL)	#define _CLASSELEXI __export#else	#define _CLASSELEXI __import#endif
#include "nscompta\nscompta.h"#include "nscompta\nscompta.rh"#include "nsbb\nsednum.h"
#define BANQUE_ROOT     "512"#define DEPENSE_ROOT    "6"
#define RECETTE_ROOT    "7"

//
// Taille des champs du fichier DEPENSE
//
#define DEPS_NUMDEPS_LEN      9
#define DEPS_DATE_LEN  	      8
#define DEPS_OPERATEUR_LEN  	PIDS_NSS_LEN
#define DEPS_LIBELLE_LEN     75
#define DEPS_MODE_PAIE_LEN  	2

//
// Indice des champs du fichier DEPENSE
//
#define DEPS_NUMDEPS_FIELD    1
#define DEPS_DATE_FIELD 	    2
#define DEPS_OPERATEUR_FIELD 	3
#define DEPS_LIBELLE_FIELD	  4
#define DEPS_MODE_PAIE_FIELD 	5

//// Taille des champs du fichier Recettes
//
#define REC_NUMCOMPT_LEN       9
#define REC_LIBELLE_LEN 	    75

//
// Indice des champs du fichier Recettes
//
#define REC_NUMCOMPT_FIELD      1
#define REC_LIBELLE_FIELD       2

//
// Taille des champs du fichier ECRITURE
//
#define ECRI_ECRITURE_LEN     10
#define ECRI_LIGNE_LEN  	     2
#define ECRI_LIBELLE_LEN      35
#define ECRI_COMPTE_LEN        6
#define ECRI_SENS_LEN          1
#define ECRI_SIGNE_LEN  	     1
#define ECRI_SOMME_LEN  	     9
#define ECRI_ETAT_LEN  	       1

//
// Indice des champs du fichier ECRITURE
//
#define ECRI_ECRITURE_FIELD     1
#define ECRI_LIGNE_FIELD  	    2
#define ECRI_LIBELLE_FIELD      3
#define ECRI_COMPTE_FIELD       4
#define ECRI_SENS_FIELD         5
#define ECRI_SIGNE_FIELD  	    6
#define ECRI_SOMME_FIELD  	    7
#define ECRI_ETAT_FIELD  	      8

//
// Taille des champs du fichier 2035Libs
//
#define LIBS_LIGNE_LEN	        2
#define LIBS_LIBELLE_LEN       75
#define LIBS_CATEGORIE_LEN      2

//
// Indice des champs du fichier 2035Libs
//
#define LIBS_LIGNE_FIELD	      1
#define LIBS_LIBELLE_FIELD      2
#define LIBS_CATEGORIE_FIELD    3

//
// Taille des champs du fichier cptsLibs
//
#define CPTS_CODE_LEN	          6
#define CPTS_OPERATEUR_LEN	    PIDS_NSS_LEN
#define CPTS_LIBELLE_LEN        75
#define CPTS_LIGNE_LEN	        2
#define CPTS_ACTIF_LEN          1

//
// Indice des champs du fichier cptsLibs
//
#define CPTS_CODE_FIELD	        1
#define CPTS_OPERATEUR_FIELD    2
#define CPTS_LIBELLE_FIELD      3
#define CPTS_LIGNE_FIELD	      4
#define CPTS_ACTIF_FIELD	      5

//---------------------------------------------------------------------------
//  Classe ECRITURE
//---------------------------------------------------------------------------
//
// Objet contenant les données
//

class _CLASSELEXI NSEcritureData{
	public :

		// Variables de stockage des valeurs
    //
    char numero[ECRI_ECRITURE_LEN + 1] ;
    char ligne[ECRI_LIGNE_LEN + 1] ;
    char libelle[ECRI_LIBELLE_LEN + 1] ;
    char compte[ECRI_COMPTE_LEN + 1] ;
    char sens[ECRI_SENS_LEN + 1] ;
    char signe[ECRI_SIGNE_LEN + 1] ;
    char somme[ECRI_SOMME_LEN + 1] ;
    char etat[ECRI_ETAT_LEN + 1] ;

    NSEcritureData() ;
    NSEcritureData(const NSEcritureData& rv) ;
    ~NSEcritureData() ;

    NSEcritureData& operator=(const NSEcritureData& src) ;
    int 		        operator==(const NSEcritureData& o) ;

    void metAZero() ;
};

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSEcriture : public NSFiche
{
  public :
    //
    // Objet qui contient les données
    //
    NSEcritureData* pDonnees ;

    NSEcriture(NSSuper* pSuper) ;
    NSEcriture(const NSEcriture& rv) ;
    ~NSEcriture();

    void alimenteFiche();
    void videFiche();
    DBIResult open();

    virtual bool Create();
    virtual bool Modify();

    NSEcriture& operator=(const NSEcriture& src) ;
    int         operator==(const NSEcriture& o);
};

//---------------------------------------------------------------------------//  Classe NSEcritureInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSEcritureInfo
{
  public :
		//
		// Objet qui contient les données
		//
		NSEcritureData* pDonnees ;

		NSEcritureInfo() ;
		NSEcritureInfo(const NSEcriture*) ;
		NSEcritureInfo(const NSEcritureInfo& rv) ;
    ~NSEcritureInfo() ;

		NSEcritureInfo& operator=(const NSEcritureInfo& src) ;
		int             operator==(const NSEcritureInfo& o) ;
};

//
// Définition de NSDepensArray (Array de NSDepensInfo(s))
//
typedef vector<NSEcritureInfo*> NSFicheEcritureArray ;
typedef NSFicheEcritureArray::iterator       NSEcritureIter ;
typedef NSFicheEcritureArray::const_iterator NSEcritureConstIter ;

class NSEcritureArray : public NSFicheEcritureArray
{
  public :

    // Constructeurs
    NSEcritureArray() : NSFicheEcritureArray() {}
    NSEcritureArray(const NSEcritureArray& rv) ;
    // Destructeur
    virtual ~NSEcritureArray() ;
    void vider() ;
};


//---------------------------------------------------------------------------
//  Classe NSDEPENS
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSDepensData
{
	public :

    // Variables de stockage des valeurs
    //
    char numdeps[DEPS_NUMDEPS_LEN + 1] ;
    char date[DEPS_DATE_LEN + 1] ;
    char operateur[DEPS_OPERATEUR_LEN + 1] ;
    char libelle[DEPS_LIBELLE_LEN + 1] ;
    char mode_paie[DEPS_MODE_PAIE_LEN + 1] ;

    NSDepensData() ;
    NSDepensData(const NSDepensData& rv) ;
    ~NSDepensData() ;

    NSDepensData& operator=(const NSDepensData& src) ;
    int           operator==(const NSDepensData& o) ;

    void metAZero() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSDepens : public NSFiche
{
  public :
    //
    // Objet qui contient les données
    //
    NSDepensData* pDonnees ;

    NSDepens(NSSuper* pSuper) ;
    NSDepens(const NSDepens& rv) ;
    ~NSDepens() ;

    void        alimenteFiche();
    void        videFiche();
    DBIResult   open();
    bool        NewNum(bool bCritique = false);

    virtual bool Create() ;
    virtual bool Modify() ;

    NSDepens& operator=(const NSDepens& src) ;
    int       operator==(const NSDepens& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSDepensInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSDepensInfo
{
  public :
		//
		// Objet qui contient les données
		//
		NSDepensData* pDonnees ;

		NSDepensInfo() ;
		NSDepensInfo(const NSDepens*) ;
		NSDepensInfo(const NSDepensInfo& rv) ;
    ~NSDepensInfo() ;

		NSDepensInfo& operator=(const NSDepensInfo& src) ;
		int           operator==(const NSDepensInfo& o) ;
};

//
// Définition de NSDepensArray (Array de NSDepensInfo(s))
//
typedef vector<NSDepensInfo*> NSFicheDepensArray ;
typedef NSFicheDepensArray::iterator       NSDepenstIter ;
typedef NSFicheDepensArray::const_iterator NSDepenstConstIter ;

class NSDepensArray : public NSFicheDepensArray
{
  public :

    // Constructeurs
    NSDepensArray() : NSFicheDepensArray() {}
    NSDepensArray(const NSDepensArray& rv) ;
    // Destructeur
    virtual ~NSDepensArray() ;
    void vider() ;
};

/****************************************************************************/
/******************* Module saisie de données Db 2035Libs.DB ****************/
/****************************************************************************/

class _CLASSELEXI NS2035LibsData
{
	public :
    //
    // Variables de stockage des valeurs
    //
    char ligne[LIBS_LIGNE_LEN + 1] ;
    char libelle[LIBS_LIBELLE_LEN + 1] ;
    char categorie[LIBS_CATEGORIE_LEN + 1] ;

    NS2035LibsData() ;
	  NS2035LibsData(const NS2035LibsData& rv) ;
    ~NS2035LibsData() ;

    NS2035LibsData& operator=(const NS2035LibsData& src) ;
    int             operator==(const NS2035LibsData& o) ;

    void metAZero() ;
    void saisie() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NS2035Libs : public NSFiche
{
  public :
    //
    // Objet qui contient les données
    //
    NS2035LibsData* pDonnees ;

    NS2035Libs(NSSuper* pSuper) ;
    NS2035Libs(const NS2035Libs& rv) ;
    ~NS2035Libs() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open() ;
    DBIResult getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NS2035Libs& operator=(const NS2035Libs& src) ;
    int         operator==(const NS2035Libs & o) ;
};

//---------------------------------------------------------------------------
//  Classe NS2035LibsInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NS2035LibsInfo
{
  public :
		//
		// Objet qui contient les données
		//
		NS2035LibsData* pDonnees ;

		NS2035LibsInfo() ;
		NS2035LibsInfo(const NS2035Libs*) ;
		NS2035LibsInfo(const NS2035LibsInfo& rv) ;
    ~NS2035LibsInfo() ;

		NS2035LibsInfo& operator=(const NS2035LibsInfo& src) ;
		int             operator==(const NS2035LibsInfo& o) ;
};

//
// Définition de NS2035LibsArray (Array de NS2035LibsInfo(s))
//
typedef vector<NS2035LibsInfo*> NSFiche2035LibsArray ;
typedef NSFiche2035LibsArray::iterator       NS2035LibsIter ;
typedef NSFiche2035LibsArray::const_iterator NS2035LibsConstIter ;

class NS2035LibsArray : public NSFiche2035LibsArray
{
  public :

    // Constructeurs
    NS2035LibsArray() : NSFiche2035LibsArray() {}
    NS2035LibsArray(const NS2035LibsArray& rv) ;
    // Destructeur
    virtual ~NS2035LibsArray() ;
    void vider() ;
};

/****************************************************************************/
/******************* Module saisie de données Db cptsLibs.DB ****************/
/****************************************************************************/

class _CLASSELEXI NScptsLibsData
{
	public :
    //
    // Variables de stockage des valeurs
    //
    char code[CPTS_CODE_LEN  + 1] ;
    char operateur[CPTS_OPERATEUR_LEN + 1] ;
    char libelle[CPTS_LIBELLE_LEN + 1] ;
    char ligne[CPTS_LIGNE_LEN  + 1] ;
    char actif[CPTS_ACTIF_LEN  + 1] ;

    NScptsLibsData() ;
    NScptsLibsData(const NScptsLibsData& rv) ;
    ~NScptsLibsData() ;

    NScptsLibsData& operator=(const NScptsLibsData& src) ;
    int             operator==(const NScptsLibsData& o) ;

    void metAZero() ;
    void saisie() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NScptsLibs : public NSFiche
{
  public :
    //    // Objet qui contient les données
    //
    NScptsLibsData* pDonnees ;

    NScptsLibs(NSSuper* pSuper) ;
    NScptsLibs(const NScptsLibs& rv) ;
    ~NScptsLibs() ;

    void alimenteFiche();
    void videFiche();
    DBIResult open();
    DBIResult getPatRecord();

    virtual bool Create();
    virtual bool Modify();

    NScptsLibs& operator=(const NScptsLibs& src) ;
    int         operator==(const NScptsLibs & o) ;
};

//---------------------------------------------------------------------------
//  Classe NScptsLibsInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NScptsLibsInfo
{
  public :
		//
		// Objet qui contient les données
		//
		NScptsLibsData* pDonnees ;

		NScptsLibsInfo() ;
		NScptsLibsInfo(const NScptsLibs*) ;
		NScptsLibsInfo(const NScptsLibsInfo& rv) ;
    ~NScptsLibsInfo() ;

		NScptsLibsInfo& operator=(const NScptsLibsInfo& src) ;
		int             operator==(const NScptsLibsInfo& o) ;
};

//
// Définition de NS2035LibsArray (Array de NS2035LibsInfo(s))
//
typedef vector<NScptsLibsInfo*> NSFicheCptsLibsArray ;
typedef NSFicheCptsLibsArray::iterator       NSCptsLibsIter ;
typedef NSFicheCptsLibsArray::const_iterator NSCptsLibsConstIter ;

class NSCptsLibsArray : public NSFicheCptsLibsArray
{
  public :

    // Constructeurs
    NSCptsLibsArray() : NSFicheCptsLibsArray() {}
    NSCptsLibsArray(const NSCptsLibsArray& rv) ;
    // Destructeur
    virtual ~NSCptsLibsArray() ;
    void vider() ;
};

/*********************************************************************************************************/
/*******************Objet boîte de dialogue DEPENSES***************************************************************/
/*********************************************************************************************************/
class _CLASSELEXI NSDepensDlg :   public NSUtilDialog
{
  public:

    NSUtilEditDate*     pDate;
    OWL::TEdit*         pLibelle;

    NSUtilEditSomme*    pSomme;
    OWL::TRadioButton*  pPlus;
    OWL::TRadioButton*  pMoins;
    //OWL::TButton*       pAdd;
    //OWL::TButton*       pLess;

    OWL::TListBox*      pLigne;
    OWL::TComboBox*     pBanque;
    OWL::TGroupBox*     pMode;
    OWL::TRadioButton*  pButton[4];
    string              sMode[4];    string              sSigne;
    string              sString[2];

    string              sNumDepens;
    NSDepensInfo*       pDepInfo;
    NSEcritureArray*    pEcritures;

    int				    LigneChoisi;
    bool                bTrouve;

    NSCptsLibsArray*    pTypeDepenseArray ;
    NSCptsLibsArray*    pBanquesArray ;

    NSDepensDlg(TWindow* pere, NSContexte* pCtx, string sNumDep = "");
    ~NSDepensDlg();

    void    CmCancel();
    void    CmOk();

    void    SetupWindow();

    void    InitExistingValues();
    void    InitDepensArray();
    string  getLibelle(NScptsLibsData* pFiche);
    void    Remplissage();

    void    Add();
    void    Less();
  DECLARE_RESPONSE_TABLE(NSDepensDlg);
};


/*********************************************************************************************************/
/*******************Objet boîte de dialogue RECETTES***************************************************************/
/*********************************************************************************************************/
class _CLASSELEXI NSRecettes;
class _CLASSELEXI NSRecettesDlg :   public NSUtilDialog
{
  public:

    NSComptData*		pData;    NSFactData*         pFactData;    NSRecettes*         pRecettesListe;    NSUtilEditDate*		pDate;
    OWL::TEdit*         pLibelle;
    NSUtilEditSomme*    pSomme;
    OWL::TGroupBox*     pMode;
    OWL::TRadioButton*  pButton[4];

    OWL::TListBox*      pLigne;
    OWL::TComboBox*     pBanque;
    NSComptInfo*        pComptInfo;
    NSEcritureArray*    pEcritureArray;

    NSCptsLibsArray*    pTypeRecetteArray ;
    NSCptsLibsArray*    pBanquesArray ;

    string              sString[4];
    bool                bTrouve;

    //    // Objet qui contient les données    //
    NSRecettesDlg(TWindow* pere, NSContexte* pCtx);    ~NSRecettesDlg();

    void    CmCancel();    void    CmOk();    void    SetupWindow();
    void    InitRecetteArray();
    void    InitComptes();    string  getLibelle(NScptsLibsData* pFiche);
    bool    ChercheNumCompt(string& sNumCompt);
    void    CmAnalyser();

    DECLARE_RESPONSE_TABLE(NSRecettesDlg);};
//---------------------------------------------------------------------------//  Classe NSRECETTES
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSRecettesData
{
  public :

    //
    // Variables de stockage des valeurs
    //

    char numcompt[REC_NUMCOMPT_LEN + 1];
    char libelle[REC_LIBELLE_LEN + 1];

    NSRecettesData() ;
    NSRecettesData(const NSRecettesData& rv) ;
    ~NSRecettesData() ;

    NSRecettesData& operator=(const NSRecettesData& src) ;
    int             operator==(const NSRecettesData& o) ;

    void metAZero() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSRecettes : public NSFiche
{
  public :
	  //
	  // Objet qui contient les données
	  //
	  NSRecettesData* pDonnees ;

	  NSRecettes(NSSuper* pSuper) ;
    NSRecettes(const NSRecettes& rv) ;
	  ~NSRecettes() ;

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open() ;
    DBIResult getPatRecord() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

    NSRecettes& operator=(const NSRecettes& src) ;
    int         operator==(const NSRecettes& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSRecettesInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSRecettesInfo
{
	public :
		//
		// Objet qui contient les données
		//
		NSRecettesData* pDonnees;

		NSRecettesInfo();
		NSRecettesInfo(const NSRecettes*) ;
		NSRecettesInfo(const NSRecettesInfo& rv) ;
    ~NSRecettesInfo();

		NSRecettesInfo& operator=(const NSRecettesInfo& src) ;
		int             operator==(const NSRecettesInfo& o) ;
};

//
// Définition de NSDepensArray (Array de NSDepensInfo(s))
//
typedef vector<NSRecettesInfo*> NSFicheRecettesArray ;
typedef NSFicheRecettesArray::iterator       NSRecettesIter ;
typedef NSFicheRecettesArray::const_iterator NSRecettesConstIter ;

class NSRecettesArray : public NSFicheRecettesArray
{
  public :

	  // Constructeurs
	  NSRecettesArray() : NSFicheRecettesArray() {}
	  NSRecettesArray(const NSRecettesArray& rv) ;
	  // Destructeur
	  virtual ~NSRecettesArray() ;
    void vider() ;
};

#endif

