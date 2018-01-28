//---------------------------------------------------------------------------//     NSCPTA.H
//
//  Contient les définitions des bases Compta, Fact, TPayant
//
//  1ere version : RS Aout 98   Dernière modif : Sept 98
//---------------------------------------------------------------------------
#ifndef __NSV_CPTA_H
#define __NSV_CPTA_H

#include <cstring.h>
#include "ns_sgbd\nsfiche.h"
#include "partage\nsperson.h"

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier COMPT
//

#define NSV_CPTA_NUMCOMPT_LEN    	9
#define NSV_CPTA_NUMFSE_LEN  	    9
#define NSV_CPTA_PATIENT_LEN 	    6
#define NSV_CPTA_DATE_LEN 		    8
#define NSV_CPTA_HEURE_LEN  	    4
#define NSV_CPTA_DUE_F_LEN  	    7
#define NSV_CPTA_PAYE_F_LEN  	    7
#define NSV_CPTA_DEPAS_F_LEN  	 	7
#define NSV_CPTA_DUE_E_LEN  	    7
#define NSV_CPTA_PAYE_E_LEN  	    7
#define NSV_CPTA_DEPAS_E_LEN  	 	7
#define NSV_CPTA_EXAMEN_LEN  	    5
#define NSV_CPTA_SYNONYME_LEN    	1
#define NSV_CPTA_OKPAYE_LEN  	    1
#define NSV_CPTA_PRESCRIPT_LEN		3
#define NSV_CPTA_CONTEXTE_LEN			5
#define NSV_CPTA_CODE_LEN  	     10
#define NSV_CPTA_OPERATEUR_LEN		3

//
// Indice des champs du fichier COMPT
//
#define NSV_CPTA_NUMCOMPT_FIELD     1
#define NSV_CPTA_NUMFSE_FIELD  	    2
#define NSV_CPTA_PATIENT_FIELD 	    3
#define NSV_CPTA_DATE_FIELD 				4
#define NSV_CPTA_HEURE_FIELD  			5
#define NSV_CPTA_DUE_F_FIELD  			6
#define NSV_CPTA_PAYE_F_FIELD  	    7
#define NSV_CPTA_DEPAS_F_FIELD  		8
#define NSV_CPTA_DUE_E_FIELD  			9
#define NSV_CPTA_PAYE_E_FIELD      10
#define NSV_CPTA_DEPAS_E_FIELD     11
#define NSV_CPTA_EXAMEN_FIELD      12
#define NSV_CPTA_SYNONYME_FIELD    13
#define NSV_CPTA_OKPAYE_FIELD      14
#define NSV_CPTA_PRESCRIPT_FIELD   15
#define NSV_CPTA_CONTEXTE_FIELD    16
#define NSV_CPTA_CODE_FIELD  	  	 17
#define NSV_CPTA_OPERATEUR_FIELD   18

//
// Taille des champs du fichier FACT
//

#define NSV_FACT_NUMCOMPT_LEN  	    9
#define NSV_FACT_NUMERO_LEN					2
#define NSV_FACT_OPERATEUR_LEN  		3
#define NSV_FACT_DATE_PAIE_LEN 	    8
#define NSV_FACT_ORGANISME_LEN     11
#define NSV_FACT_LIBELLE_LEN	   	 15
#define NSV_FACT_MONTANT_LEN    		7
#define NSV_FACT_UNITE_LEN    			3
#define NSV_FACT_MODE_PAIE_LEN 	    2

//
// Indice des champs du fichier FACT
//
#define NSV_FACT_NUMCOMPT_FIELD     1
#define NSV_FACT_NUMERO_FIELD	    	2
#define NSV_FACT_OPERATEUR_FIELD    3
#define NSV_FACT_DATE_PAIE_FIELD 		4
#define NSV_FACT_ORGANISME_FIELD    5
#define NSV_FACT_LIBELLE_FIELD			6
#define NSV_FACT_MONTANT_FIELD      7
#define NSV_FACT_UNITE_FIELD    		8
#define NSV_FACT_MODE_PAIE_FIELD 		9

//
// Taille des champs du fichier TPAYANT
//
#define NSV_TPAY_NUMCOMPT_LEN  	    9
#define NSV_TPAY_NUMERO_LEN         2
#define NSV_TPAY_ORGANISME_LEN     11
#define NSV_TPAY_LIBELLE_LEN       15
#define NSV_TPAY_RESTE_DU_LEN       7
#define NSV_TPAY_MONNAIE_LEN        3
#define NSV_TPAY_OKPAYE_LEN         2

//
// Indice des champs du fichier TPAYANT
//
#define NSV_TPAY_NUMCOMPT_FIELD 	  1
#define NSV_TPAY_NUMERO_FIELD		    2
#define NSV_TPAY_ORGANISME_FIELD    3
#define NSV_TPAY_LIBELLE_FIELD		  4
#define NSV_TPAY_RESTE_DU_FIELD     5
#define NSV_TPAY_MONNAIE_FIELD  	  6
#define NSV_TPAY_OKPAYE_FIELD 		  7

//
// Taille des champs du fichier DEPENSE
//
#define NSV_DEPS_NUMDEPS_LEN      9
#define NSV_DEPS_DATE_LEN  	      8
#define NSV_DEPS_OPERATEUR_LEN  	3
#define NSV_DEPS_LIBELLE_LEN     75
#define NSV_DEPS_MODE_PAIE_LEN  	2

//
// Indice des champs du fichier DEPENSE
//
#define NSV_DEPS_NUMDEPS_FIELD    1
#define NSV_DEPS_DATE_FIELD 	    2
#define NSV_DEPS_OPERATEUR_FIELD 	3
#define NSV_DEPS_LIBELLE_FIELD	  4
#define NSV_DEPS_MODE_PAIE_FIELD 	5

//
// Taille des champs du fichier cptsLibs
//
#define NSV_CPTS_CODE_LEN	        6
#define NSV_CPTS_OPERATEUR_LEN	  3
#define NSV_CPTS_LIBELLE_LEN     75
#define NSV_CPTS_LIGNE_LEN	      2
#define NSV_CPTS_ACTIF_LEN        1

//
// Indice des champs du fichier cptsLibs
//
#define NSV_CPTS_CODE_FIELD	      1
#define NSV_CPTS_OPERATEUR_FIELD  2
#define NSV_CPTS_LIBELLE_FIELD    3
#define NSV_CPTS_LIGNE_FIELD	    4
#define NSV_CPTS_ACTIF_FIELD	    5


//---------------------------------------------------------------------------
//  Classe NSCompt
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSCvtComptData
{
	public :

		//
		// Variables de stockage des valeurs
		//
		char numcompt[NSV_CPTA_NUMCOMPT_LEN + 1] ;
		char numfse[NSV_CPTA_NUMFSE_LEN + 1] ;
		char patient[NSV_CPTA_PATIENT_LEN + 1] ;
		char date[NSV_CPTA_DATE_LEN + 1] ;
		char heure[NSV_CPTA_HEURE_LEN + 1] ;
		char duFranc[NSV_CPTA_DUE_F_LEN + 1] ;
		char payeFranc[NSV_CPTA_PAYE_F_LEN + 1] ;
		char depassFranc[NSV_CPTA_DEPAS_F_LEN + 1] ;
		char duEuro[NSV_CPTA_DUE_E_LEN + 1] ;
		char payeEuro[NSV_CPTA_PAYE_E_LEN + 1] ;
		char depassEuro[NSV_CPTA_DEPAS_E_LEN + 1] ;
		char examen[NSV_CPTA_EXAMEN_LEN + 1] ;
		char synonyme[NSV_CPTA_SYNONYME_LEN + 1] ;
		char okPaye[NSV_CPTA_OKPAYE_LEN + 1] ;
		char prescript[NSV_CPTA_PRESCRIPT_LEN + 1] ;
		char contexte[NSV_CPTA_CONTEXTE_LEN + 1] ;
		char code[NSV_CPTA_CODE_LEN + 1] ;
		char operateur[NSV_CPTA_OPERATEUR_LEN + 1] ;

		NSCvtComptData() ;
		NSCvtComptData(NSCvtComptData& rv) ;
		~NSCvtComptData() ;

		NSCvtComptData& operator=(NSCvtComptData src) ;
		int 			      operator==(const NSCvtComptData& o) ;

		void metAZero() ;
};

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSCvtCompt : public NSFiche
{
	public :

		//
    // Objet qui contient les données
    //
    NSCvtComptData* pDonnees ;

		NSCvtCompt(NSSuper* pSuper) ;
    NSCvtCompt(NSCvtCompt& rv) ;
    ~NSCvtCompt() ;

		void         alimenteFiche() ;
		void         videFiche() ;
		DBIResult    open(string pathbase) ;
		DBIResult    getPatRecord() ;

		virtual bool Create() ;
		virtual bool Modify() ;

		NSCvtCompt&  operator=(NSCvtCompt src) ;
		int 	       operator==(const NSCvtCompt& o) ;
};

//---------------------------------------------------------------------------//  Classe NSComptInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSCvtComptInfo
{
	public :

		//		// Objet qui contient les données
		//		NSCvtComptData* pDonnees ;

		NSCvtComptInfo() ;
		NSCvtComptInfo(NSCvtCompt*) ;
		NSCvtComptInfo(NSCvtComptInfo& rv) ;
    ~NSCvtComptInfo() ;

		NSCvtComptInfo& operator=(NSCvtComptInfo src) ;		int 			      operator==(const NSCvtComptInfo& o) ;
};

//// Définition de NSComptArray (Array de NSComptInfo(s))
//
typedef vector<NSCvtComptInfo*> NSCvtFicheComptArray ;
typedef NSCvtFicheComptArray::iterator NSCvtComptIter ;

class NSCvtComptArray : public NSCvtFicheComptArray
{
	public :

		// Constructeurs
    NSCvtComptArray() : NSCvtFicheComptArray() {}
    NSCvtComptArray(NSCvtComptArray& rv) ;
    // Destructeur
    virtual ~NSCvtComptArray() ;
    void vider() ;
};

//------------------------------ Fact.db -------------------------------------//
//---------------------------------------------------------------------------
//  Classe NSFactData
//---------------------------------------------------------------------------

//// Objet contenant les données
//
class NSCvtFactData
{
	public :

		//    // Variables de stockage des valeurs
    //
    char numcompt[NSV_FACT_NUMCOMPT_LEN + 1] ;
    char numero[NSV_FACT_NUMERO_LEN + 1] ;
    char operateur[NSV_FACT_OPERATEUR_LEN + 1] ;
    char date_paie[NSV_FACT_DATE_PAIE_LEN + 1] ;
    char organisme[NSV_FACT_ORGANISME_LEN + 1] ;
    char libelle[NSV_FACT_LIBELLE_LEN + 1] ;
    char montant[NSV_FACT_MONTANT_LEN + 1] ;
    char unite[NSV_FACT_UNITE_LEN + 1] ;
    char mode_paie[NSV_FACT_MODE_PAIE_LEN + 1] ;

    NSCvtFactData() ;
    NSCvtFactData(NSCvtFactData& rv) ;
    ~NSCvtFactData() ;

    NSCvtFactData& operator=(NSCvtFactData src) ;
    int 			     operator==(const NSCvtFactData& o) ;

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFact
//---------------------------------------------------------------------------

//// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSCvtFact : public NSFiche
{
	public :

		//    // Objet qui contient les données
    //
    NSCvtFactData* pDonnees ;

		NSCvtFact(NSSuper* pSuper) ;
    NSCvtFact(NSCvtFact& rv) ;
    ~NSCvtFact() ;

		void alimenteFiche() ;
    void videFiche() ;
    DBIResult open(string pathbase) ;

    virtual bool Create() ;    virtual bool Modify() ;

    NSCvtFact& operator=(NSCvtFact src) ;
    int 	     operator==(const NSCvtFact& o) ;
};

//---------------------------------------------------------------------------//  Classe NSCvtFactInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSCvtFactInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSCvtFactData* pDonnees ;

		NSCvtFactInfo() ;
		NSCvtFactInfo(NSCvtFact*) ;
		NSCvtFactInfo(NSCvtFactInfo& rv) ;
    ~NSCvtFactInfo() ;

		NSCvtFactInfo& operator=(NSCvtFactInfo src) ;
		int 		       operator==(const NSCvtFactInfo& o) ;
};

//// Définition de NSFactArray (Array de NSFactInfo(s))
//
typedef vector<NSCvtFactInfo*> NSCvtFicheFactArray ;
typedef NSCvtFicheFactArray::iterator NSCvtFactIter ;

class NSCvtFactArray : public NSCvtFicheFactArray
{
	public :

		// Constructeurs
		NSCvtFactArray() : NSCvtFicheFactArray() {}
		NSCvtFactArray(NSCvtFactArray& rv);
		// Destructeur
		virtual ~NSCvtFactArray();
   	void vider();
};

//---------------------------- TPayant.db -----------------------------------
//
//---------------------------------------------------------------------------
//  Classe NSTPayantData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSCvtTPayantData
{
	public :

   	//    // Variables de stockage des valeurs
    //
    char numcompt[NSV_TPAY_NUMCOMPT_LEN + 1] ;
    char numero[NSV_TPAY_NUMERO_LEN + 1] ;
    char organisme[NSV_TPAY_ORGANISME_LEN + 1] ;
    char libelle[NSV_TPAY_LIBELLE_LEN + 1] ;
    char reste_du[NSV_TPAY_RESTE_DU_LEN + 1] ;
    char monnaie[NSV_TPAY_MONNAIE_LEN + 1] ;
    char okpaye[NSV_TPAY_OKPAYE_LEN + 1] ;

    NSCvtTPayantData() ;    NSCvtTPayantData(NSCvtTPayantData& rv) ;
    ~NSCvtTPayantData() ;

    NSCvtTPayantData& operator=(NSCvtTPayantData src) ;
    int 			 operator==(const NSCvtTPayantData& o) ;

    void metAZero() ;};
//---------------------------------------------------------------------------
//  Classe NSTPayant
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCvtTPayant : public NSFiche
{
  public :

	  //	  // Objet qui contient les données
	  //
	  NSCvtTPayantData* pDonnees ;

	  NSCvtTPayant(NSSuper* pSuper) ;
		NSCvtTPayant(NSCvtTPayant& rv) ;
	  ~NSCvtTPayant() ;

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open(string pathbase) ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

		NSCvtTPayant& operator=(NSCvtTPayant src) ;
		int 	        operator==(const NSCvtTPayant& o) ;
};

//---------------------------------------------------------------------------//  Classe NSTPayantInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCvtTPayantInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSCvtTPayantData* pDonnees ;

		NSCvtTPayantInfo() ;
		NSCvtTPayantInfo(NSCvtTPayant*) ;
		NSCvtTPayantInfo(NSCvtTPayantInfo& rv) ;
    ~NSCvtTPayantInfo() ;

		NSCvtTPayantInfo& operator=(NSCvtTPayantInfo src) ;
		int 		          operator==(const NSCvtTPayantInfo& o) ;
};


//---------------------------------------------------------------------------
//  Classe NSDEPENS
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSCvtDepensData
{
	public :

		// Variables de stockage des valeurs
    //
    char numdeps[NSV_DEPS_NUMDEPS_LEN  +1] ;
    char date[NSV_DEPS_DATE_LEN  + 1] ;
    char operateur[NSV_DEPS_OPERATEUR_LEN  + 1] ;
    char libelle[NSV_DEPS_LIBELLE_LEN + 1] ;
    char mode_paie[NSV_DEPS_MODE_PAIE_LEN + 1] ;

    NSCvtDepensData() ;
    NSCvtDepensData(NSCvtDepensData& rv) ;
    ~NSCvtDepensData() ;

    NSCvtDepensData& operator=(NSCvtDepensData src) ;
    int              operator==(const NSCvtDepensData& o) ;

    void metAZero() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSCvtDepens : public NSFiche
{
	public :

		//
    // Objet qui contient les données
    //
    NSCvtDepensData* pDonnees ;

    NSCvtDepens(NSSuper* pSuper) ;
    NSCvtDepens(NSCvtDepens& rv) ;
    ~NSCvtDepens() ;

    void        alimenteFiche() ;
    void        videFiche() ;
    DBIResult   open(string pathbase) ;
    bool        NewNum(bool bCritique = false) ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSCvtDepens& operator=(NSCvtDepens src) ;
    int 	       operator==(const NSCvtDepens& o) ;
};

//---------------------------------------------------------------------------
//  Classe NSDepensInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSCvtDepensInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSCvtDepensData* pDonnees ;

		NSCvtDepensInfo() ;
		NSCvtDepensInfo(NSCvtDepens*) ;
		NSCvtDepensInfo(NSCvtDepensInfo& rv) ;
    ~NSCvtDepensInfo() ;

		NSCvtDepensInfo& operator=(NSCvtDepensInfo src) ;
		int              operator==(const NSCvtDepensInfo& o) ;
};

//
// Définition de NSDepensArray (Array de NSDepensInfo(s))
//
typedef vector<NSCvtDepensInfo*> NSCvtFicheDepensArray ;
typedef NSCvtFicheDepensArray::iterator NSCvtDepenstIter ;

class NSCvtDepensArray : public NSCvtFicheDepensArray
{
	public :

		// Constructeurs
    NSCvtDepensArray() : NSCvtFicheDepensArray() {}
    NSCvtDepensArray(NSCvtDepensArray& rv) ;
    // Destructeur
    virtual ~NSCvtDepensArray() ;

  	void vider() ;};
/****************************************************************************/
/******************* Module saisie de données Db cptsLibs.DB ****************/
/****************************************************************************/

class NSCvtcptsLibsData
{
	public :
		//
    // Variables de stockage des valeurs
    //
    char code[NSV_CPTS_CODE_LEN  + 1] ;
    char operateur[NSV_CPTS_OPERATEUR_LEN + 1] ;
    char libelle[NSV_CPTS_LIBELLE_LEN + 1] ;
    char ligne[NSV_CPTS_LIGNE_LEN  + 1] ;
    char actif[NSV_CPTS_ACTIF_LEN  + 1] ;

    NSCvtcptsLibsData() ;
    NSCvtcptsLibsData(NSCvtcptsLibsData& rv) ;
    ~NSCvtcptsLibsData() ;

    NSCvtcptsLibsData& operator=(NSCvtcptsLibsData src) ;
    int                operator==(const NSCvtcptsLibsData& o) ;

    void metAZero() ;
    void saisie() ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSCvtcptsLibs : public NSFiche
{
	public :
  	//
    // Objet qui contient les données
    //
    NSCvtcptsLibsData* pDonnees ;

    NSCvtcptsLibs(NSSuper* pSuper) ;
    NSCvtcptsLibs(NSCvtcptsLibs& rv) ;
    ~NSCvtcptsLibs() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open(string pathbase) ;
    DBIResult getPatRecord() ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSCvtcptsLibs& operator=(NSCvtcptsLibs src) ;
    int            operator==(const NSCvtcptsLibs & o) ;
};

//---------------------------------------------------------------------------
//  Classe NScptsLibsInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSCvtcptsLibsInfo
{
	public :
  	//
		// Objet qui contient les données
		//
		NSCvtcptsLibsData* pDonnees ;

		NSCvtcptsLibsInfo() ;
		NSCvtcptsLibsInfo(NSCvtcptsLibs*) ;
		NSCvtcptsLibsInfo(NSCvtcptsLibsInfo& rv) ;
    ~NSCvtcptsLibsInfo() ;

		NSCvtcptsLibsInfo& operator=(NSCvtcptsLibsInfo src) ;
		int                operator==(const NSCvtcptsLibsInfo& o) ;
};

//
// Définition de NS2035LibsArray (Array de NS2035LibsInfo(s))
//
typedef vector<NSCvtcptsLibsInfo*> NSCvtFicheCptsLibsArray ;
typedef NSCvtFicheCptsLibsArray::iterator NSCvtCptsLibsIter ;

class NSCvtCptsLibsArray : public NSCvtFicheCptsLibsArray
{
	public :

		// Constructeurs
    NSCvtCptsLibsArray() : NSCvtFicheCptsLibsArray() {}
    NSCvtCptsLibsArray(NSCvtCptsLibsArray& rv) ;
    // Destructeur
    virtual ~NSCvtCptsLibsArray() ;
    void vider() ;
};

//---------------------------------------------------------------------------#endif    // __NSV_CPTA_H

