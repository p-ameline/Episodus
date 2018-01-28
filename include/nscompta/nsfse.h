//---------------------------------------------------------------------------//     NSFSE.H
//
//  Contient les définitions des objets de gestion des FSE
//
//  1ere version : PA juin 98   Dernière modif : 17/06/98
//---------------------------------------------------------------------------
#ifndef __NSFSE_H
#define __NSFSE_H

#include <cstring.h>
#include "ns_sgbd\nsfiche.h"

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//// Taille des champs du fichier FSE
//
#define FSE_NUMERO_LEN  	   9
#define FSE_NUMCOMPT_LEN	   9
#define FSE_OPERATION_LEN 	 1
#define FSE_DATE_LEN 	  	  12
#define FSE_SITUATION_LEN    1
#define FSE_NUM_RECTIF_LEN   9
#define FSE_DATE_RECTIF_LEN 12

//
// Indice des champs du fichier FSE
//
#define FSE_NUMERO_FIELD  	  1
#define FSE_NUMCOMPT_FIELD    2
#define FSE_OPERATION_FIELD 	3
#define FSE_DATE_FIELD 	  	 	4
#define FSE_SITUATION_FIELD   5
#define FSE_NUM_RECTIF_FIELD  6
#define FSE_DATE_RECTIF_FIELD 7

//// Taille des champs du fichier FSEComp
//
#define FSECMP_NUMCOMPT_LEN  		  9
#define FSECMP_TIERS_PAYANT_LEN 	1
#define FSECMP_AVANCE_FRAIS_LEN   1
#define FSECMP_MONNAIE_LEN 	  	  3
#define FSECMP_DATE_ACCDC_LEN    12
#define FSECMP_IND_ALSACE_LEN     1
#define FSECMP_DATE_MAT_LEN 		 12
#define FSECMP_DATE_ACCTR_LEN		 12
#define FSECMP_NUM_ACCTR_LEN		  8
#define FSECMP_CLE_ACCTR_LEN      1

//
// Indice des champs du fichier FSEComp
//
#define FSECMP_NUMCOMPT_FIELD  		  1
#define FSECMP_TIERS_PAYANT_FIELD 	2
#define FSECMP_AVANCE_FRAIS_FIELD  	3
#define FSECMP_MONNAIE_FIELD 	  		4
#define FSECMP_DATE_ACCDC_FIELD    	5
#define FSECMP_IND_ALSACE_FIELD    	6
#define FSECMP_DATE_MAT_FIELD 		  7
#define FSECMP_DATE_ACCTR_FIELD		  8
#define FSECMP_NUM_ACCTR_FIELD		  9
#define FSECMP_CLE_ACCTR_FIELD     10

//------------------------------ fse.db -------------------------------------

////---------------------------------------------------------------------------
//  Classe NSFseData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSFseData
{
  public :

    //
    // Variables de stockage des valeurs
    //
    char numero[FSE_NUMERO_LEN + 1] ;
    char numcompt[FSE_NUMCOMPT_LEN + 1] ;
    char operation[FSE_OPERATION_LEN + 1] ;
    char date[FSE_DATE_LEN + 1] ;
    char situation[FSE_SITUATION_LEN + 1] ;
    char num_rectif[FSE_NUM_RECTIF_LEN + 1] ;
    char date_rectif[FSE_DATE_RECTIF_LEN + 1] ;

    NSFseData() ;
    NSFseData(const NSFseData& rv) ;
    ~NSFseData() ;

    NSFseData& operator=(const NSFseData& src) ;
    int 			 operator==(const NSFseData& o) ;

    void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFse
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSFse : public NSFiche
{
  public :

    //
	  // Objet qui contient les données
	  //
	  NSFseData* pDonnees ;

	  NSFse(NSSuper* pSuper) ;
    NSFse(const NSFse& rv) ;
	  ~NSFse() ;

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

    NSFse& operator=(const NSFse& src) ;
    int 	 operator==(const NSFse& o) ;
};

//---------------------------------------------------------------------------//  Classe NSFseInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSFseInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSFseData* pDonnees ;

		NSFseInfo() ;
		NSFseInfo(NSFse*) ;
		NSFseInfo(const NSFseInfo& rv) ;
    ~NSFseInfo() ;

		NSFseInfo& operator=(const NSFseInfo& src) ;
		int 		   operator==(const NSFseInfo& o) ;
};

//--------------------------- fsecomp.db ------------------------------------//
//---------------------------------------------------------------------------
//  Classe NSFseCompData
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class _CLASSELEXI NSFseCompData
{
	public :

   	//
	  	// Variables de stockage des valeurs
	  	//
     	char numcompt[FSECMP_NUMCOMPT_LEN + 1];
     	char tiers_payant[FSECMP_TIERS_PAYANT_LEN + 1];
     	char avance_frais[FSECMP_AVANCE_FRAIS_LEN + 1];
     	char monnaie[FSECMP_MONNAIE_LEN + 1];
     	char date_accdc[FSECMP_DATE_ACCDC_LEN + 1];
     	char ind_alsace[FSECMP_IND_ALSACE_LEN + 1];
      char date_mat[FSECMP_DATE_MAT_LEN + 1];
      char date_acctr[FSECMP_DATE_ACCTR_LEN + 1];
      char num_acctr[FSECMP_NUM_ACCTR_LEN + 1];
      char cle_acctr[FSECMP_CLE_ACCTR_LEN + 1];

	  	NSFseCompData() ;
	  	NSFseCompData(const NSFseCompData& rv) ;
     	~NSFseCompData() ;

	  	NSFseCompData& operator=(const NSFseCompData& src) ;
	  	int 			 operator==(const NSFseCompData& o) ;

     	void metAZero() ;
};

//---------------------------------------------------------------------------//  Classe NSFseComp
//---------------------------------------------------------------------------
//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSFseComp : public NSFiche
{
  public :

	  //
	  // Objet qui contient les données
	  //
	  NSFseCompData* pDonnees ;

	  NSFseComp(NSSuper* pSuper) ;
    NSFseComp(const NSFseComp& rv) ;
	  ~NSFseComp() ;

	  void alimenteFiche() ;
	  void videFiche() ;
	  DBIResult open() ;

	  virtual bool Create() ;
	  virtual bool Modify() ;

    NSFseComp& operator=(const NSFseComp& src) ;
    int 	  operator==(const NSFseComp& o) ;
};

//---------------------------------------------------------------------------//  Classe NSFseCompInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSFseCompInfo
{
	public :

		//
		// Objet qui contient les données
		//
		NSFseCompData* pDonnees;

		NSFseCompInfo();
		NSFseCompInfo(NSFseComp*);
		NSFseCompInfo(const NSFseCompInfo& rv);
    ~NSFseCompInfo();

		NSFseCompInfo& operator=(const NSFseCompInfo& src);
		int 		  operator==(const NSFseCompInfo& o);
};

//---------------------------------------------------------------------------
#endif    // __NSFSE_H
