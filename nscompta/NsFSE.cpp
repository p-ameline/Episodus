//---------------------------------------------------------------------------//    NSPERSON.CPP
//    KRZISCH PH.   janvier 92
//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------
#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

//using namespace std;

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nscompta\nsFSE.h"

//***************************************************************************
// Implémentation des méthodes NSFse
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFseData::NSFseData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseData::NSFseData(const NSFseData& rv)
{
	strcpy(numero, 	    rv.numero);
  strcpy(numcompt,	  rv.numcompt);
	strcpy(operation,   rv.operation);
	strcpy(date, 	 	    rv.date);
	strcpy(situation,   rv.situation);
	strcpy(num_rectif,  rv.num_rectif);
	strcpy(date_rectif, rv.date_rectif);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseData::~NSFseData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFseData&
NSFseData::operator=(const NSFseData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(numero, 	    src.numero);
  strcpy(numcompt,	  src.numcompt);
	strcpy(operation,   src.operation);
	strcpy(date, 	 	    src.date);
	strcpy(situation,   src.situation);
	strcpy(num_rectif,  src.num_rectif);
	strcpy(date_rectif, src.date_rectif);

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseData::operator==(const NSFseData& o)
{
	if ((strcmp(numero, o.numero) == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFseData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numero, 	    0, FSE_NUMERO_LEN + 1);
  memset(numcompt,	  0, FSE_NUMCOMPT_LEN + 1);
	memset(operation,   0, FSE_OPERATION_LEN + 1);
	memset(date, 	 	    0, FSE_DATE_LEN + 1);
	memset(situation,   0, FSE_SITUATION_LEN + 1);
	memset(num_rectif,  0, FSE_NUM_RECTIF_LEN + 1);
	memset(date_rectif, 0, FSE_DATE_RECTIF_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFse::NSFse(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSFseData() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFse::NSFse(const NSFse& rv)
      :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFse::~NSFse()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFse::alimenteFiche()
{
   alimenteChamp(pDonnees->numero, 	  	 FSE_NUMERO_FIELD,  		FSE_NUMERO_LEN);
   alimenteChamp(pDonnees->numcompt, 	 FSE_NUMCOMPT_FIELD,  	FSE_NUMCOMPT_LEN);
   alimenteChamp(pDonnees->operation, 	 FSE_OPERATION_FIELD, 	FSE_OPERATION_LEN);
   alimenteChamp(pDonnees->date, 	 	 FSE_DATE_FIELD, 	  		FSE_DATE_LEN);
   alimenteChamp(pDonnees->situation, 	 FSE_SITUATION_FIELD,	FSE_SITUATION_LEN);
   alimenteChamp(pDonnees->num_rectif,  FSE_NUM_RECTIF_FIELD, 	FSE_NUM_RECTIF_LEN);
   alimenteChamp(pDonnees->date_rectif, FSE_DATE_RECTIF_FIELD, FSE_DATE_RECTIF_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :   	NSFse::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFse::videFiche()
{
	videChamp(pDonnees->numero, 	  	 FSE_NUMERO_FIELD,  		FSE_NUMERO_LEN);
   videChamp(pDonnees->numcompt, 	 FSE_NUMCOMPT_FIELD,  	FSE_NUMCOMPT_LEN);
   videChamp(pDonnees->operation, 	 FSE_OPERATION_FIELD, 	FSE_OPERATION_LEN);
   videChamp(pDonnees->date, 	 	 	 FSE_DATE_FIELD, 	  		FSE_DATE_LEN);
   videChamp(pDonnees->situation, 	 FSE_SITUATION_FIELD,	FSE_SITUATION_LEN);
   videChamp(pDonnees->num_rectif,   FSE_NUM_RECTIF_FIELD, 	FSE_NUM_RECTIF_LEN);
   videChamp(pDonnees->date_rectif,  FSE_DATE_RECTIF_FIELD, FSE_DATE_RECTIF_LEN);
}

//---------------------------------------------------------------------------//  Fonction :  	NSFse::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFse::open()
{
	char tableName[] = "FSE.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFse::Create()
//---------------------------------------------------------------------------
bool
NSFse::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSFse::Modify()
//---------------------------------------------------------------------------
bool
NSFse::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFse&
NSFse::operator=(const NSFse& src)
{
  if (this == &src)
  	return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFse::operator == (const NSFse& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Fonction:		NSFseInfo::NSFseInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFseInfo::NSFseInfo()
{
	// Crée l'objet de données
	pDonnees = new NSFseData();
}

//---------------------------------------------------------------------------//  Fonction:		NSFseInfo::NSFseInfo(NSFse*)
////  Description:	Constructeur à partir d'un NSFse
//---------------------------------------------------------------------------
NSFseInfo::NSFseInfo(NSFse* pFse)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseData() ;
	//
	// Copie les valeurs du NSFse
	//
	*pDonnees = *(pFse->pDonnees) ;
}
//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseInfo::NSFseInfo(const NSFseInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseData();
	//
	// Copie les valeurs du NSFseInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseInfo::~NSFseInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseInfo&
NSFseInfo::operator=(const NSFseInfo& src)
{
  if (this == &src)
  	return *this ;

	*pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseInfo::operator == (const NSFseInfo& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************// Implémentation des méthodes NSFseComp
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSFseCompData::NSFseCompData()
{
	//
	// Met les champs de données à zéro
	//
	metAZero() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseCompData::NSFseCompData(const NSFseCompData& rv)
{
  strcpy(numcompt,		 rv.numcompt);
	strcpy(tiers_payant, rv.tiers_payant);
	strcpy(avance_frais, rv.avance_frais);
  strcpy(monnaie,			 rv.monnaie);
	strcpy(date_accdc,   rv.date_accdc);
  strcpy(ind_alsace,	 rv.ind_alsace);
	strcpy(date_mat,  	 rv.date_mat);
	strcpy(date_acctr, 	 rv.date_acctr);
  strcpy(num_acctr,		 rv.num_acctr);
  strcpy(cle_acctr,		 rv.cle_acctr);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseCompData::~NSFseCompData()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSFseCompData&
NSFseCompData::operator=(const NSFseCompData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(numcompt, 	   src.numcompt);
	strcpy(tiers_payant, src.tiers_payant);
	strcpy(avance_frais, src.avance_frais);
  strcpy(monnaie,			 src.monnaie);
	strcpy(date_accdc,   src.date_accdc);
  strcpy(ind_alsace,	 src.ind_alsace);
	strcpy(date_mat,  	 src.date_mat);
	strcpy(date_acctr, 	 src.date_acctr);
  strcpy(num_acctr,		 src.num_acctr);
  strcpy(cle_acctr,		 src.cle_acctr);

	return *this;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseCompData::operator == ( const NSFseCompData& o )
{
	if ((strcmp(numcompt, o.numcompt) == 0))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSFseCompData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(numcompt, 	  	0, FSECMP_NUMCOMPT_LEN + 1);
	memset(tiers_payant,  0, FSECMP_TIERS_PAYANT_LEN + 1);
	memset(avance_frais, 	0, FSECMP_AVANCE_FRAIS_LEN + 1);
	memset(monnaie,   		0, FSECMP_MONNAIE_LEN + 1);
	memset(date_accdc,  	0, FSECMP_DATE_ACCDC_LEN + 1);
	memset(ind_alsace, 		0, FSECMP_IND_ALSACE_LEN + 1);
  memset(date_mat,			0, FSECMP_DATE_MAT_LEN + 1);
  memset(date_acctr,		0, FSECMP_DATE_ACCTR_LEN + 1);
  memset(num_acctr,			0, FSECMP_NUM_ACCTR_LEN + 1);
  memset(cle_acctr,			0, FSECMP_CLE_ACCTR_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSFseComp::NSFseComp(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSFseCompData() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSFseComp::NSFseComp(const NSFseComp& rv)
          :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseCompData() ;
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSFseComp::~NSFseComp()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSFseComp::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSFseComp::alimenteFiche()
{
   alimenteChamp(pDonnees->numcompt, 	  	FSECMP_NUMCOMPT_FIELD,		 FSECMP_NUMCOMPT_LEN);
   alimenteChamp(pDonnees->tiers_payant,	FSECMP_TIERS_PAYANT_FIELD, FSECMP_TIERS_PAYANT_LEN);
   alimenteChamp(pDonnees->avance_frais, 	FSECMP_AVANCE_FRAIS_FIELD, FSECMP_AVANCE_FRAIS_LEN);
   alimenteChamp(pDonnees->monnaie, 	 	  FSECMP_MONNAIE_FIELD,		   FSECMP_MONNAIE_LEN);
   alimenteChamp(pDonnees->date_accdc,  	FSECMP_DATE_ACCDC_FIELD, 	 FSECMP_DATE_ACCDC_LEN);
   alimenteChamp(pDonnees->ind_alsace, 	  FSECMP_IND_ALSACE_FIELD, 	 FSECMP_IND_ALSACE_LEN);
   alimenteChamp(pDonnees->date_mat,		  FSECMP_DATE_MAT_FIELD,		 FSECMP_DATE_MAT_LEN);
   alimenteChamp(pDonnees->date_acctr,		FSECMP_DATE_ACCTR_FIELD,	 FSECMP_DATE_ACCTR_LEN);
   alimenteChamp(pDonnees->num_acctr,		  FSECMP_NUM_ACCTR_FIELD,		 FSECMP_NUM_ACCTR_LEN);
   alimenteChamp(pDonnees->cle_acctr,		  FSECMP_CLE_ACCTR_FIELD,		 FSECMP_CLE_ACCTR_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :   	NSFseComp::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSFseComp::videFiche()
{
	videChamp(pDonnees->numcompt, 	  	FSECMP_NUMCOMPT_FIELD,		FSECMP_NUMCOMPT_LEN);
   videChamp(pDonnees->tiers_payant,	FSECMP_TIERS_PAYANT_FIELD, FSECMP_TIERS_PAYANT_LEN);
   videChamp(pDonnees->avance_frais, 	FSECMP_AVANCE_FRAIS_FIELD, FSECMP_AVANCE_FRAIS_LEN);
   videChamp(pDonnees->monnaie, 	 		FSECMP_MONNAIE_FIELD,		FSECMP_MONNAIE_LEN);
   videChamp(pDonnees->date_accdc,  	FSECMP_DATE_ACCDC_FIELD, 	FSECMP_DATE_ACCDC_LEN);
   videChamp(pDonnees->ind_alsace, 		FSECMP_IND_ALSACE_FIELD, 	FSECMP_IND_ALSACE_LEN);
   videChamp(pDonnees->date_mat,			FSECMP_DATE_MAT_FIELD,		FSECMP_DATE_MAT_LEN);
   videChamp(pDonnees->date_acctr,		FSECMP_DATE_ACCTR_FIELD,	FSECMP_DATE_ACCTR_LEN);
   videChamp(pDonnees->num_acctr,		FSECMP_NUM_ACCTR_FIELD,		FSECMP_NUM_ACCTR_LEN);
   videChamp(pDonnees->cle_acctr,		FSECMP_CLE_ACCTR_FIELD,		FSECMP_CLE_ACCTR_LEN);
}

//---------------------------------------------------------------------------//  Fonction :  	NSFseComp::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFseComp::open()
{
	char tableName[] = "FSECOMP.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_COMPTA);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFseComp::Create()
//---------------------------------------------------------------------------
bool
NSFseComp::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSFseComp::Modify()
//---------------------------------------------------------------------------
bool
NSFseComp::Modify()
{
	return true;}
//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseComp&
NSFseComp::operator=(const NSFseComp& src)
{
  if (this == &src)
  	return *this ;

	*pDonnees = *(src.pDonnees) ;
	return *this ;}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseComp::operator == (const NSFseComp& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Fonction:		NSFseCompInfo::NSFseCompInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSFseCompInfo::NSFseCompInfo()
{
	// Crée l'objet de données
	pDonnees = new NSFseCompData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSFseCompInfo::NSFseCompInfo(NSFseComp*)
//
//  Description:	Constructeur à partir d'un NSFseComp
//---------------------------------------------------------------------------
NSFseCompInfo::NSFseCompInfo(NSFseComp* pFseComp)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseCompData();
	//
	// Copie les valeurs du NSFseComp
	//
	*pDonnees = *(pFseComp->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSFseCompInfo::NSFseCompInfo(const NSFseCompInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSFseCompData();
	//
	// Copie les valeurs du NSFseCompInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSFseCompInfo::~NSFseCompInfo()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFseCompInfo&
NSFseCompInfo::operator=(const NSFseCompInfo& src)
{
  if (this == &src)
  	return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFseCompInfo::operator == (const NSFseCompInfo& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

//////////////////////////// fin du fichier NSFse.cpp

