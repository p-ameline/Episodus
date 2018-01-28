//---------------------------------------------------------------------------//     NSFSE.H
//
//  Contient les définitions des objets de gestion des FSE
//
//  1ere version : PA juin 98   Dernière modif : 17/06/98
//---------------------------------------------------------------------------
#ifndef __NSV_FSE16_H
#define __NSV_FSE16_H

#include <cstring.h>

#include "ns_sgbd\nsfiche.h"
#include "partage\nsCIM10.h"
#if defined(_DANSCPTADLL)	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier FSE1610
//
#define NSV_FSE1610_NUMCOMPT_LEN 	  	9
#define NSV_FSE1610_NUMPREST_LEN 	  	4
#define NSV_FSE1610_DATE_LEN 	  	 	 12
#define NSV_FSE1610_LIEU_LEN    		  1
#define NSV_FSE1610_CODE_LEN   			  4
#define NSV_FSE1610_COMPLMNT_LEN 		  1
#define NSV_FSE1610_MONTANT_F_LEN		  7
#define NSV_FSE1610_MONTANT_E_LEN  		7
#define NSV_FSE1610_QUALIF_LEN  	  	1
#define NSV_FSE1610_COEFF_LEN  	  		6
#define NSV_FSE1610_DIVIS_LEN			    2
#define NSV_FSE1610_QUANTITE_LEN  		2
#define NSV_FSE1610_DENOMBR_LEN  	  	2
#define NSV_FSE1610_PRIX_UNIT_F_LEN  	7
#define NSV_FSE1610_PRIX_UNIT_E_LEN  	7
#define NSV_FSE1610_BASE_RMB_LEN  		7
#define NSV_FSE1610_TAUX_LEN  	  		3
#define NSV_FSE1610_RMB_AMO_LEN  	  	7
#define NSV_FSE1610_RMB_AMC_LEN  	  	7
#define NSV_FSE1610_RMO_LEN  	  		  2
#define NSV_FSE1610_CODE_EXO_LEN  		1

//
// Indice des champs du fichier FSE1610
//
#define NSV_FSE1610_NUMCOMPT_FIELD  	1
#define NSV_FSE1610_NUMPREST_FIELD 	  2
#define NSV_FSE1610_DATE_FIELD 	  	 	3
#define NSV_FSE1610_LIEU_FIELD    		4
#define NSV_FSE1610_CODE_FIELD   		  5
#define NSV_FSE1610_COMPLMNT_FIELD 		6
#define NSV_FSE1610_MONTANT_F_FIELD		7
#define NSV_FSE1610_MONTANT_E_FIELD  	8
#define NSV_FSE1610_QUALIF_FIELD  	  9
#define NSV_FSE1610_COEFF_FIELD  	   10
#define NSV_FSE1610_DIVIS_FIELD			 11
#define NSV_FSE1610_QUANTITE_FIELD   12
#define NSV_FSE1610_DENOMBR_FIELD  	 13
#define NSV_FSE1610_PRIX_UNIT_F_FIELD 14
#define NSV_FSE1610_PRIX_UNIT_E_FIELD 15
#define NSV_FSE1610_BASE_RMB_FIELD  	16
#define NSV_FSE1610_TAUX_FIELD  	  	17
#define NSV_FSE1610_RMB_AMO_FIELD  	  18
#define NSV_FSE1610_RMB_AMC_FIELD  	  19
#define NSV_FSE1610_RMO_FIELD  	  		20
#define NSV_FSE1610_CODE_EXO_FIELD  	21

//
// Taille des champs du fichier FSE1620
//
#define NSV_FSE1620_NUMCOMPT_LEN 		9
#define NSV_FSE1620_NUMPREST_LEN 		4
#define NSV_FSE1620_DATE_DEPL_LEN  		12
#define NSV_FSE1620_CODE_IFD_LEN 	  	4
#define NSV_FSE1620_MONTANT_IFD_F_LEN	7
#define NSV_FSE1620_MONTANT_IFD_E_LEN   7
#define NSV_FSE1620_QUANTITE_LEN 		2
#define NSV_FSE1620_PRIX_UNIT_F_LEN		7
#define NSV_FSE1620_PRIX_UNIT_E_LEN		7
#define NSV_FSE1620_BASE_RMB_LEN     	7
#define NSV_FSE1620_TAUX_LEN     		3
#define NSV_FSE1620_RMB_AMO_LEN     	7
#define NSV_FSE1620_RMB_AMC_LEN     	7

//
// Indice des champs du fichier FSE1620
//
#define NSV_FSE1620_NUMCOMPT_FIELD  		1
#define NSV_FSE1620_NUMPREST_FIELD 			2
#define NSV_FSE1620_DATE_DEPL_FIELD  		3
#define NSV_FSE1620_CODE_IFD_FIELD 	  		4
#define NSV_FSE1620_MONTANT_IFD_F_FIELD		5
#define NSV_FSE1620_MONTANT_IFD_E_FIELD   	6
#define NSV_FSE1620_QUANTITE_FIELD 			7
#define NSV_FSE1620_PRIX_UNIT_F_FIELD		8
#define NSV_FSE1620_PRIX_UNIT_E_FIELD		9
#define NSV_FSE1620_BASE_RMB_FIELD     		10
#define NSV_FSE1620_TAUX_FIELD     			11
#define NSV_FSE1620_RMB_AMO_FIELD     		12
#define NSV_FSE1620_RMB_AMC_FIELD     		13

//
// Taille des champs du fichier FSE1630
//
#define NSV_FSE1630_NUMCOMPT_LEN 			9
#define NSV_FSE1630_NUMPREST_LEN 			4
#define NSV_FSE1630_DATE_DEPL_LEN  			12
#define NSV_FSE1630_CODE_IK_LEN 	  		4
#define NSV_FSE1630_NBRE_KM_LEN         	2
#define NSV_FSE1630_MONTANT_IK_F_LEN		7
#define NSV_FSE1630_MONTANT_IK_E_LEN   		7
#define NSV_FSE1630_PRIX_UNIT_F_LEN			7
#define NSV_FSE1630_PRIX_UNIT_E_LEN			7
#define NSV_FSE1630_BASE_RMB_LEN     		7
#define NSV_FSE1630_TAUX_LEN     			3
#define NSV_FSE1630_RMB_AMO_LEN     		7
#define NSV_FSE1630_RMB_AMC_LEN     		7

//
// Indice des champs du fichier FSE1630
//
#define NSV_FSE1630_NUMCOMPT_FIELD  		1
#define NSV_FSE1630_NUMPREST_FIELD 			2
#define NSV_FSE1630_DATE_DEPL_FIELD  		3
#define NSV_FSE1630_CODE_IK_FIELD 	  		4
#define NSV_FSE1630_NBRE_KM_FIELD			5
#define NSV_FSE1630_MONTANT_IK_F_FIELD		6
#define NSV_FSE1630_MONTANT_IK_E_FIELD   	7
#define NSV_FSE1630_PRIX_UNIT_F_FIELD		8
#define NSV_FSE1630_PRIX_UNIT_E_FIELD		9
#define NSV_FSE1630_BASE_RMB_FIELD     		10
#define NSV_FSE1630_TAUX_FIELD     			11
#define NSV_FSE1630_RMB_AMO_FIELD     		12
#define NSV_FSE1630_RMB_AMC_FIELD     		13

//// Taille des champs du fichier FSECCAM
//
#define NSV_FSECCAM_NUMCOMPT_LEN			  9
#define NSV_FSECCAM_NUMPREST_LEN		    4
#define NSV_FSECCAM_DATE_LEN		       12
#define NSV_FSECCAM_CODE_LEN			     10
#define NSV_FSECCAM_POURCENT_LEN        2#define NSV_FSECCAM_MODIFICATEURS_LEN  10#define NSV_FSECCAM_MONTANT_E_LEN			  7

//
// Indice des champs du fichier FSECCAM
//
#define NSV_FSECCAM_NUMCOMPT_FIELD			1
#define NSV_FSECCAM_NUMPREST_FIELD		  2
#define NSV_FSECCAM_DATE_FIELD		      3
#define NSV_FSECCAM_CODE_FIELD			    4
#define NSV_FSECCAM_POURCENT_FIELD      5#define NSV_FSECCAM_MODIFICATEURS_FIELD 6#define NSV_FSECCAM_MONTANT_E_FIELD			7
//
// Taille des champs du fichier CODPREST
//
#define CODPREST_CODE_LEN			4
#define CODPREST_LIBELLE_LEN		80
#define CODPREST_FSE16XX_LEN		4
#define CODPREST_PRIX_F_LEN			7
#define CODPREST_PRIX_E_LEN			7

//
// Indice des champs du fichier CODPREST
//
#define CODPREST_CODE_FIELD			1
#define CODPREST_LIBELLE_FIELD		2
#define CODPREST_FSE16XX_FIELD		3
#define CODPREST_PRIX_F_FIELD		4
#define CODPREST_PRIX_E_FIELD		5

//------------------------------ Fse1610.db -------------------------------------
//
//---------------------------------------------------------------------------
//  Classe NSFse1610Data
//---------------------------------------------------------------------------
//
// Objet contenant les données
//class _CLASSELEXI NSCvtFse1610Data
{
  public :

    //    // Variables de stockage des valeurs
    //
    char numcompt[NSV_FSE1610_NUMCOMPT_LEN + 1];
    char numprest[NSV_FSE1610_NUMPREST_LEN + 1];
    char date[NSV_FSE1610_DATE_LEN + 1];
    char lieu[NSV_FSE1610_LIEU_LEN + 1];
    char code[NSV_FSE1610_CODE_LEN + 1];
    char complmnt[NSV_FSE1610_COMPLMNT_LEN + 1];
    char montant_f[NSV_FSE1610_MONTANT_F_LEN + 1];
    char montant_e[NSV_FSE1610_MONTANT_E_LEN + 1];    char qualif[NSV_FSE1610_QUALIF_LEN + 1];
    char coeff[NSV_FSE1610_COEFF_LEN + 1];
    char divis[NSV_FSE1610_DIVIS_LEN + 1];
    char quantite[NSV_FSE1610_QUANTITE_LEN + 1];
    char denombr[NSV_FSE1610_DENOMBR_LEN + 1];
    char prix_unit_f[NSV_FSE1610_PRIX_UNIT_F_LEN + 1];    char prix_unit_e[NSV_FSE1610_PRIX_UNIT_E_LEN + 1];
    char base_rmb[NSV_FSE1610_BASE_RMB_LEN + 1];
    char taux[NSV_FSE1610_TAUX_LEN + 1];
    char rmb_amo[NSV_FSE1610_RMB_AMO_LEN + 1];
    char rmb_amc[NSV_FSE1610_RMB_AMC_LEN + 1];
    char rmo[NSV_FSE1610_RMO_LEN + 1];
		char code_exo[NSV_FSE1610_CODE_EXO_LEN + 1];

    NSCvtFse1610Data();
    NSCvtFse1610Data(NSCvtFse1610Data& rv);
    ~NSCvtFse1610Data();

    NSCvtFse1610Data& operator=(NSCvtFse1610Data src);
    int 			 operator==(const NSCvtFse1610Data& o);

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFse1610
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCvtFse1610 : public NSFiche
{
  public :
	  	//
	  	// Objet qui contient les données
	  	//
	  	NSCvtFse1610Data* pDonnees;

	  	NSCvtFse1610(NSSuper* pSuper) ;
     	NSCvtFse1610(NSCvtFse1610& rv) ;
	  	~NSCvtFse1610();

	  	void alimenteFiche();
	  	void videFiche();
	  	DBIResult open(string pathbase);

	  	virtual bool Create();
	  	virtual bool Modify();

     	NSCvtFse1610& operator=(NSCvtFse1610 src);
     	int 	  operator==(const NSCvtFse1610& o);
};

//---------------------------------------------------------------------------//  Classe NSFse1610Info  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCvtFse1610Info
{
	public :

		//		// Objet qui contient les données
		//
		NSCvtFse1610Data* pDonnees;

		NSCvtFse1610Info();
		NSCvtFse1610Info(NSCvtFse1610*);
		NSCvtFse1610Info(NSCvtFse1610Info& rv);
    ~NSCvtFse1610Info();

		NSCvtFse1610Info& operator=(NSCvtFse1610Info src);
		int               operator==(const NSCvtFse1610Info& o);
};

//--------------------------- Fse1620.db ------------------------------------
//
//---------------------------------------------------------------------------
//  Classe NSFse1620Data
//---------------------------------------------------------------------------
//
// Objet contenant les données
//

class _CLASSELEXI NSCvtFse1620Data{
	public :
   		//
	  	// Variables de stockage des valeurs
	  	//
     	char numcompt[NSV_FSE1620_NUMCOMPT_LEN + 1];
     	char numprest[NSV_FSE1620_NUMPREST_LEN + 1];
     	char date_depl[NSV_FSE1620_DATE_DEPL_LEN + 1];
     	char code_ifd[NSV_FSE1620_CODE_IFD_LEN + 1];
     	char montant_ifd_f[NSV_FSE1620_MONTANT_IFD_F_LEN + 1];
     	char montant_ifd_e[NSV_FSE1620_MONTANT_IFD_E_LEN + 1];
      char quantite[NSV_FSE1620_QUANTITE_LEN + 1];
      char prix_unit_f[NSV_FSE1620_PRIX_UNIT_F_LEN + 1];
      char prix_unit_e[NSV_FSE1620_PRIX_UNIT_E_LEN + 1];
      char base_rmb[NSV_FSE1620_BASE_RMB_LEN + 1];
      char taux[NSV_FSE1620_TAUX_LEN + 1];
      char rmb_amo[NSV_FSE1620_RMB_AMO_LEN + 1];
      char rmb_amc[NSV_FSE1620_RMB_AMC_LEN + 1];

	  	NSCvtFse1620Data();
	  	NSCvtFse1620Data(NSCvtFse1620Data& rv);
     	~NSCvtFse1620Data();

	  	NSCvtFse1620Data& operator=(NSCvtFse1620Data src);
	  	int               operator==(const NSCvtFse1620Data& o);

     	void metAZero();};

//---------------------------------------------------------------------------//  Classe NSFse1620
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCvtFse1620 : public NSFiche
{
  public :

    //    // Objet qui contient les données
    //
    NSCvtFse1620Data* pDonnees ;

    NSCvtFse1620(NSSuper* pSuper) ;
    NSCvtFse1620(NSCvtFse1620& rv) ;
    ~NSCvtFse1620() ;

    void alimenteFiche();
    void videFiche();
    DBIResult open(string pathbase);

    virtual bool Create();
    virtual bool Modify();

    NSCvtFse1620& operator=(NSCvtFse1620 src) ;
    int           operator==(const NSCvtFse1620& o) ;
};

//---------------------------------------------------------------------------//  Classe NSFse1620Info  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCvtFse1620Info
{
	public :

		//		// Objet qui contient les données
		//
		NSCvtFse1620Data* pDonnees;

		NSCvtFse1620Info();
		NSCvtFse1620Info(NSCvtFse1620*);
		NSCvtFse1620Info(NSCvtFse1620Info& rv);
    ~NSCvtFse1620Info();

		NSCvtFse1620Info& operator=(NSCvtFse1620Info src);
		int               operator==(const NSCvtFse1620Info& o);
};

//--------------------------- Fse1630.db ------------------------------------//
//---------------------------------------------------------------------------
//  Classe NSFse1630Data
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSCvtFse1630Data
{
	public :

    //    // Variables de stockage des valeurs
    //
    char numcompt[NSV_FSE1630_NUMCOMPT_LEN + 1];
    char numprest[NSV_FSE1630_NUMPREST_LEN + 1];
    char date_depl[NSV_FSE1630_DATE_DEPL_LEN + 1];
    char code_ik[NSV_FSE1630_CODE_IK_LEN + 1];
    char nbre_km[NSV_FSE1630_NBRE_KM_LEN + 1];
    char montant_ik_f[NSV_FSE1630_MONTANT_IK_F_LEN + 1];
    char montant_ik_e[NSV_FSE1630_MONTANT_IK_E_LEN + 1];
    char prix_unit_f[NSV_FSE1630_PRIX_UNIT_F_LEN + 1];
    char prix_unit_e[NSV_FSE1630_PRIX_UNIT_E_LEN + 1];
    char base_rmb[NSV_FSE1630_BASE_RMB_LEN + 1];
    char taux[NSV_FSE1630_TAUX_LEN + 1];
    char rmb_amo[NSV_FSE1630_RMB_AMO_LEN + 1];
    char rmb_amc[NSV_FSE1630_RMB_AMC_LEN + 1];

    NSCvtFse1630Data();
	  NSCvtFse1630Data(NSCvtFse1630Data& rv);
    ~NSCvtFse1630Data();

    NSCvtFse1630Data& operator=(NSCvtFse1630Data src);
    int               operator==(const NSCvtFse1630Data& o);

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFse1630
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCvtFse1630 : public NSFiche
{
  public :

    //    // Objet qui contient les données
    //
    NSCvtFse1630Data* pDonnees ;

    NSCvtFse1630(NSSuper* pSuper) ;
    NSCvtFse1630(NSCvtFse1630& rv) ;
    ~NSCvtFse1630() ;

    void alimenteFiche();
    void videFiche();
    DBIResult open(string pathbase);

    virtual bool Create();
    virtual bool Modify();

    NSCvtFse1630& operator=(NSCvtFse1630 src);
    int           operator==(const NSCvtFse1630& o);
};

//---------------------------------------------------------------------------
//  Classe NSFse1630Info  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCvtFse1630Info
{
	public :

		//
		// Objet qui contient les données
		//
		NSCvtFse1630Data* pDonnees;

		NSCvtFse1630Info();
		NSCvtFse1630Info(NSCvtFse1630*);
		NSCvtFse1630Info(NSCvtFse1630Info& rv);
    ~NSCvtFse1630Info();

		NSCvtFse1630Info& operator=(NSCvtFse1630Info src);
		int               operator==(const NSCvtFse1630Info& o);
};
//------------------------------ FseCCAM.db -------------------------------------//
//---------------------------------------------------------------------------
//  Classe NSFseCCAMData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSCvtFseCCAMData
{
	public :

		//
    // Variables de stockage des valeurs
    //
    char numcompt[NSV_FSECCAM_NUMCOMPT_LEN + 1] ;
    char numprest[NSV_FSECCAM_NUMPREST_LEN + 1] ;
    char date[NSV_FSECCAM_DATE_LEN + 1] ;
    char code[NSV_FSECCAM_CODE_LEN + 1] ;
    char pourcent[NSV_FSECCAM_POURCENT_LEN + 1] ;    char modificateurs[NSV_FSECCAM_MODIFICATEURS_LEN + 1] ;    char montant_e[NSV_FSECCAM_MONTANT_E_LEN + 1] ;

    NSCvtFseCCAMData() ;    NSCvtFseCCAMData(NSCvtFseCCAMData& rv) ;    ~NSCvtFseCCAMData() ;

    NSCvtFseCCAMData& operator=(NSCvtFseCCAMData src) ;    int               operator==(const NSCvtFseCCAMData& o) ;

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFseCCAM
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCvtFseCCAM : public NSFiche
{
  public :

		//
    // Objet qui contient les données
    //
    NSCvtFseCCAMData* pDonnees ;

    NSCvtFseCCAM(NSSuper* pSuper) ;
    NSCvtFseCCAM(NSCvtFseCCAM& rv) ;
    ~NSCvtFseCCAM() ;

    void      alimenteFiche() ;
    void      videFiche() ;
    DBIResult open(string pathbase) ;

    virtual bool Create() ;
    virtual bool Modify() ;

    NSCvtFseCCAM& operator=(NSCvtFseCCAM src) ;
    int           operator==(const NSCvtFseCCAM& o) ;
};

//---------------------------------------------------------------------------//  Classe NSFseCCAMInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCvtFseCCAMInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSCvtFseCCAMData* pDonnees ;

		NSCvtFseCCAMInfo() ;
		NSCvtFseCCAMInfo(NSCvtFseCCAM*) ;
		NSCvtFseCCAMInfo(NSCvtFseCCAMInfo& rv) ;
    ~NSCvtFseCCAMInfo() ;

		NSCvtFseCCAMInfo& operator=(NSCvtFseCCAMInfo src) ;
		int               operator==(const NSCvtFseCCAMInfo& o) ;

    string getNumcompt()      { return string(pDonnees->numcompt) ; }
    string getNumprest()      { return string(pDonnees->numprest) ; }
    string getDate()          { return string(pDonnees->date) ; }
    string getCode()          { return string(pDonnees->code) ; }
    string getPourcent()      { return string(pDonnees->pourcent) ; }    string getModificateurs() { return string(pDonnees->modificateurs) ; }    string getMontant_e()     { return string(pDonnees->montant_e) ; }
};

//---------------------------------------------------------------------------#endif    // __NSV_FSE16_H

