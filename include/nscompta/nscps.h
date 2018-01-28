//---------------------------------------------------------------------------
//     NSCPS.H
//
//  Contient les définitions des objets de gestion de la carte CPS
//
//  1ere version : PA juin 98   Dernière modif : 17/06/98
//---------------------------------------------------------------------------
#ifndef __NSCPS_H
#define __NSCPS_H

#include <cstring.h>
#include "ns_sgbd\nsfiche.h"

#if defined(_DANSCPTADLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier Carte_PS1
//
#define CPS1_CODE_LEN   	 	5
#define CPS1_TYPE_CARTE_LEN 	2
#define CPS1_TYPE_IDNAT_LEN 	1
#define CPS1_IDNAT_LEN  	  30
#define CPS1_CLE_IDNAT_LEN  	1
#define CPS1_CIVILITE_LEN  	2
#define CPS1_NOM_LEN  	  	  27
#define CPS1_PRENOM_LEN  	  27

//
// Indice des champs du fichier Carte_PS1
//
#define CPS1_CODE_FIELD   	 	1
#define CPS1_TYPE_CARTE_FIELD 2
#define CPS1_TYPE_IDNAT_FIELD 3
#define CPS1_IDNAT_FIELD  	   4
#define CPS1_CLE_IDNAT_FIELD  5
#define CPS1_CIVILITE_FIELD  	6
#define CPS1_NOM_FIELD  	  	7
#define CPS1_PRENOM_FIELD  	8

//
// Taille des champs du fichier Carte_PS2
//
#define CPS2_CODE_LEN   	 	5
#define CPS2_NUM_SIT_LEN   	1
#define CPS2_EXERCICE_LEN   	2
#define CPS2_STATUT_EXER_LEN  3
#define CPS2_SECTEUR_LEN   	2
#define CPS2_TYPE_IDEN_LEN   	1
#define CPS2_NUM_IDEN_LEN    14
#define CPS2_CLE_IDEN_LEN   	1
#define CPS2_RAISON_SOC_LEN  40
#define CPS2_NUM_FACT_LEN		8
#define CPS2_CLE_FACT_LEN   	1
#define CPS2_IDEN_REMPL_LEN  30
#define CPS2_CLE_REMPL_LEN   	1
#define CPS2_CONVENT_LEN   	1
#define CPS2_SPECIALITE_LEN   2
#define CPS2_ZONE_TARIF_LEN   2
#define CPS2_ZONE_IK_LEN   	2
#define CPS2_AGREMENT1_LEN   	1
#define CPS2_AGREMENT2_LEN   	1
#define CPS2_AGREMENT3_LEN   	1
#define CPS2_SIGNE_FSE_LEN   	1
#define CPS2_SIGNE_LOT_LEN   	1

//
// Indice des champs du fichier Carte_PS2
//
#define CPS2_CODE_FIELD   	 	  1
#define CPS2_NUM_SIT_FIELD   	  2
#define CPS2_EXERCICE_FIELD     3
#define CPS2_STATUT_EXER_FIELD  4
#define CPS2_SECTEUR_FIELD   	  5
#define CPS2_TYPE_IDEN_FIELD    6
#define CPS2_NUM_IDEN_FIELD     7
#define CPS2_CLE_IDEN_FIELD     8
#define CPS2_RAISON_SOC_FIELD   9
#define CPS2_NUM_FACT_FIELD	 10
#define CPS2_CLE_FACT_FIELD    11
#define CPS2_IDEN_REMPL_FIELD  12
#define CPS2_CLE_REMPL_FIELD   13
#define CPS2_CONVENT_FIELD   	 14
#define CPS2_SPECIALITE_FIELD  15
#define CPS2_ZONE_TARIF_FIELD  16
#define CPS2_ZONE_IK_FIELD   	 17
#define CPS2_AGREMENT1_FIELD   18
#define CPS2_AGREMENT2_FIELD   19
#define CPS2_AGREMENT3_FIELD   20
#define CPS2_SIGNE_FSE_FIELD   21
#define CPS2_SIGNE_LOT_FIELD   22

//---------------------------------------------------------------------------
//  Classe NSCPS1
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSCPS1Data
{
	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
     	char code[CPS1_CODE_LEN + 1];
		char typeCarte[CPS1_TYPE_CARTE_LEN + 1];
		char typeIdnat[CPS1_TYPE_IDNAT_LEN + 1];
		char idnat[CPS1_IDNAT_LEN + 1];
		char cleIdnat[CPS1_CLE_IDNAT_LEN + 1];
		char civilite[CPS1_CIVILITE_LEN + 1];
		char nom[CPS1_NOM_LEN + 1];
		char prenom[CPS1_PRENOM_LEN + 1];

	  	NSCPS1Data();
	  	NSCPS1Data(const NSCPS1Data& rv);
     	~NSCPS1Data();

	  	NSCPS1Data& operator=(const NSCPS1Data& src);
	  	int 			operator==(const NSCPS1Data& o);

	  	void metABlanc();
     	void metAZero();
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCPS1 : public NSFiche
{
  public :
	  //
	  // Objet qui contient les données
	  //
	  NSCPS1Data* pDonnees;

	  NSCPS1(NSContexte* pCtx);
     NSCPS1(const NSCPS1& rv);
	  ~NSCPS1();

	  void metABlanc() { pDonnees->metABlanc(); }

	  void alimenteFiche();
	  void videFiche();
	  DBIResult open();
     DBIResult getPatRecord();

	  virtual bool Create();
	  virtual bool Modify();

     NSCPS1& operator=(const NSCPS1& src);
     int 	 operator==(const NSCPS1& o);
};

//---------------------------------------------------------------------------
//  Classe NSCPS1Info  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCPS1Info
{
	public :
		//
		// Objet qui contient les données
		//
		NSCPS1Data* pDonnees;

		NSCPS1Info();
		NSCPS1Info(NSCPS1Info*);
		NSCPS1Info(NSCPS1Info& rv);
      ~NSCPS1Info();

		NSCPS1Info& operator=(NSCPS1Info src);
		int 			operator==(const NSCPS1Info& o);
};

//---------------------------------------------------------------------------
//  Classe NSCPS2
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSCPS2Data
{
	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
     	char code[CPS2_CODE_LEN + 1];
     	char numSit[CPS2_NUM_SIT_LEN + 1];
     	char exercice[CPS2_EXERCICE_LEN + 1];
     	char statutExer[CPS2_STATUT_EXER_LEN + 1];
     	char secteur[CPS2_SECTEUR_LEN + 1];
     	char typeIden[CPS2_TYPE_IDEN_LEN + 1];
     	char numIden[CPS2_NUM_IDEN_LEN + 1];
     	char cleIden[CPS2_CLE_IDEN_LEN + 1];
     	char raisonSoc[CPS2_RAISON_SOC_LEN + 1];
     	char numFact[CPS2_NUM_FACT_LEN + 1];
     	char cleFact[CPS2_CLE_FACT_LEN + 1];
     	char idenRempl[CPS2_IDEN_REMPL_LEN + 1];
     	char cleRemp[CPS2_CLE_REMPL_LEN + 1];
     	char convent[CPS2_CONVENT_LEN + 1];
     	char specialite[CPS2_SPECIALITE_LEN + 1];
     	char zoneTarif[CPS2_ZONE_TARIF_LEN + 1];
     	char zoneIk[CPS2_ZONE_IK_LEN + 1];
     	char agrement1[CPS2_AGREMENT1_LEN + 1];
      char agrement2[CPS2_AGREMENT2_LEN + 1];
      char agrement3[CPS2_AGREMENT3_LEN + 1];
     	char signeFse[CPS2_SIGNE_FSE_LEN + 1];
     	char signeLot[CPS2_SIGNE_LOT_LEN + 1];

	  	NSCPS2Data();
	  	NSCPS2Data(const NSCPS2Data& rv);
     	~NSCPS2Data();

	  	NSCPS2Data& operator=(const NSCPS2Data& src);
	  	int 			operator==(const NSCPS2Data& o);

	  	void metABlanc();
     	void metAZero();
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class _CLASSELEXI NSCPS2 : public NSFiche
{
  public :
	  //
	  // Objet qui contient les données
	  //
	  NSCPS2Data* pDonnees;

	  NSCPS2(NSContexte* pCtx);
     NSCPS2(const NSCPS2& rv);
	  ~NSCPS2();

	  void metABlanc() { pDonnees->metABlanc(); }

	  void alimenteFiche();
	  void videFiche();
	  DBIResult open();
     DBIResult getPatRecord();

	  virtual bool Create();
	  virtual bool Modify();

     NSCPS2& operator=(const NSCPS2& src);
     int 	 operator==(const NSCPS2& o);
};

//---------------------------------------------------------------------------
//  Classe NSCPS2Info  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class _CLASSELEXI NSCPS2Info
{
	public :
		//
		// Objet qui contient les données
		//
		NSCPS2Data* pDonnees;

		NSCPS2Info();
		NSCPS2Info(NSCPS2Info*);
		NSCPS2Info(const NSCPS2Info& rv);
      ~NSCPS2Info();

		NSCPS2Info& operator=(const NSCPS2Info& src);
		int 			operator==(const NSCPS2Info& o);
};

//---------------------------------------------------------------------------
#endif    // __NSCPS_H
