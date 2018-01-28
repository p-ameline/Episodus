//---------------------------------------------------------------------------
//    NSPERSON.CPP
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
#include "nscompta\nsvitale.h"

//***************************************************************************
// Implémentation des méthodes NSVitale1
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSVitale1Data::NSVitale1Data()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale1Data::NSVitale1Data(NSVitale1Data& rv)
{
	strcpy(immatricul, rv.immatricul);
   strcpy(typeImma, 	 rv.typeImma);
   strcpy(cleImma, 	 rv.cleImma);
   strcpy(regime, 	 rv.regime);
   strcpy(caisse, 	 rv.caisse);
   strcpy(centre, 	 rv.centre);
   strcpy(codeGest, 	 rv.codeGest);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale1Data::~NSVitale1Data()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSVitale1Data&
NSVitale1Data::operator=(NSVitale1Data src)
{
	strcpy(immatricul, src.immatricul);
   strcpy(typeImma, 	 src.typeImma);
   strcpy(cleImma, 	 src.cleImma);
   strcpy(regime, 	 src.regime);
   strcpy(caisse, 	 src.caisse);
   strcpy(centre, 	 src.centre);
   strcpy(codeGest, 	 src.codeGest);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale1Data::operator == ( const NSVitale1Data& o )
{
	if ((strcmp(immatricul, o.immatricul) 	== 0) &&
   	 (strcmp(typeImma, 	o.typeImma)   	== 0) &&
       (strcmp(cleImma, 	o.cleImma)   	== 0) &&
       (strcmp(regime, 		o.regime)   	== 0) &&
       (strcmp(caisse, 		o.caisse)   	== 0) &&
       (strcmp(centre, 		o.centre)   	== 0) &&
       (strcmp(codeGest, 	o.codeGest)   	== 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Met à blanc les variables de la fiche
//---------------------------------------------------------------------------
void
NSVitale1Data::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
	memset(immatricul, ' ', CSV1_IMMATRICUL_LEN);
   memset(typeImma, 	 ' ', CSV1_TYPE_IMMA_LEN);
   memset(cleImma, 	 ' ', CSV1_CLE_IMMA_LEN);
   memset(regime, 	 ' ', CSV1_REGIME_LEN);
   memset(caisse, 	 ' ', CSV1_CAISSE_LEN);
   memset(centre, 	 ' ', CSV1_CENTRE_LEN);
   memset(codeGest, 	 ' ', CSV1_CODE_GEST_LEN);
}

//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSVitale1Data::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(immatricul, 0, CSV1_IMMATRICUL_LEN + 1);
   memset(typeImma, 	 0, CSV1_TYPE_IMMA_LEN + 1);
   memset(cleImma, 	 0, CSV1_CLE_IMMA_LEN + 1);
   memset(regime, 	 0, CSV1_REGIME_LEN + 1);
   memset(caisse, 	 0, CSV1_CAISSE_LEN + 1);
   memset(centre, 	 0, CSV1_CENTRE_LEN + 1);
   memset(codeGest, 	 0, CSV1_CODE_GEST_LEN + 1);
}
//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSVitale1::NSVitale1(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSVitale1Data();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale1::NSVitale1(const NSVitale1& rv)
          :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale1Data();
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees);
}


//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale1::~NSVitale1()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSVitale1::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSVitale1::alimenteFiche()
{
   alimenteChamp(pDonnees->immatricul, CSV1_IMMATRICUL_FIELD, CSV1_IMMATRICUL_LEN);
   alimenteChamp(pDonnees->typeImma, 	CSV1_TYPE_IMMA_FIELD,  CSV1_TYPE_IMMA_LEN);
   alimenteChamp(pDonnees->cleImma, 	CSV1_CLE_IMMA_FIELD,   CSV1_CLE_IMMA_LEN);
   alimenteChamp(pDonnees->regime, 	 	CSV1_REGIME_FIELD, 	  CSV1_REGIME_LEN);
   alimenteChamp(pDonnees->caisse, 	 	CSV1_CAISSE_FIELD, 	  CSV1_CAISSE_LEN);
   alimenteChamp(pDonnees->centre, 	 	CSV1_CENTRE_FIELD, 	  CSV1_CENTRE_LEN);
   alimenteChamp(pDonnees->codeGest, 	CSV1_CODE_GEST_FIELD,  CSV1_CODE_GEST_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :   	NSVitale1::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSVitale1::videFiche()
{
	videChamp(pDonnees->immatricul, CSV1_IMMATRICUL_FIELD, CSV1_IMMATRICUL_LEN);
   videChamp(pDonnees->typeImma,   CSV1_TYPE_IMMA_FIELD,  CSV1_TYPE_IMMA_LEN);
   videChamp(pDonnees->cleImma, 	  CSV1_CLE_IMMA_FIELD,   CSV1_CLE_IMMA_LEN);
   videChamp(pDonnees->regime, 	  CSV1_REGIME_FIELD, 	 CSV1_REGIME_LEN);
   videChamp(pDonnees->caisse, 	  CSV1_CAISSE_FIELD, 	 CSV1_CAISSE_LEN);
   videChamp(pDonnees->centre, 	  CSV1_CENTRE_FIELD, 	 CSV1_CENTRE_LEN);
   videChamp(pDonnees->codeGest,   CSV1_CODE_GEST_FIELD,  CSV1_CODE_GEST_LEN);
}
//---------------------------------------------------------------------------//  Fonction :  	NSVitale1::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSVitale1::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
	  return (lastError = ERROR_TABLENOTOPEN) ;
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK) ;

	return lastError ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSVitale1::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSVitale1::open()
{
	char tableName[] = "CARTE_SV1.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSVitale1::Create()
//
//---------------------------------------------------------------------------
bool
NSVitale1::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSVitale1::Modify()
//
//---------------------------------------------------------------------------
bool
NSVitale1::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------NSVitale1&
NSVitale1::operator=(const NSVitale1& src)
{
  if (this == &src)
  	return *this ;

	*pDonnees = *(src.pDonnees);

	return *this;}


//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale1::operator == (const NSVitale1& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Fonction:		NSVitale1Info::NSVitale1Info()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSVitale1Info::NSVitale1Info()
{
	// Crée l'objet de données
	pDonnees = new NSVitale1Data();
}

//---------------------------------------------------------------------------
//  Fonction:		NSVitale1Info::NSVitale1Info(NSVitale1*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSVitale1Info::NSVitale1Info(NSVitale1* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale1Data();
	//
	// Copie les valeurs du NSVitale1
	//
	*pDonnees = *(pTransac->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale1Info::NSVitale1Info(const NSVitale1Info& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale1Data();
	//
	// Copie les valeurs du NSVitale1Info d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale1Info::~NSVitale1Info()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVitale1Info&
NSVitale1Info::operator=(const NSVitale1Info& src)
{  if (this == &src)  	return *this ;
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale1Info::operator == (const NSVitale1Info& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// Implémentation des méthodes NSVitale2
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSVitale2Data::NSVitale2Data()
{
	//
	// Met les champs de données à zéro
	//
	metAZero();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale2Data::NSVitale2Data(const NSVitale2Data& rv)
{
	strcpy(immatricul, rv.immatricul);
   strcpy(numero, 	 rv.numero);
   strcpy(patient,	 rv.patient);
   strcpy(nomPatro, 	 rv.nomPatro);
   strcpy(nomUsuel, 	 rv.nomUsuel);
   strcpy(prenom, 	 rv.prenom);
   strcpy(adresse, 	 rv.adresse);
   strcpy(dateNais, 	 rv.dateNais);
   strcpy(rangGem, 	 rv.rangGem);
   strcpy(qualite, 	 rv.qualite);
   strcpy(droits, 	 rv.droits);
   strcpy(ald, 		 rv.ald);
   strcpy(situation,  rv.situation);
   strcpy(mutuelle, 	 rv.mutuelle);
   strcpy(droitsMut,  rv.droitsMut);
   strcpy(garanties,  rv.garanties);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale2Data::~NSVitale2Data()
{
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSVitale2Data&
NSVitale2Data::operator=(const NSVitale2Data& src)
{  if (this == &src)  	return *this ;
	strcpy(immatricul, src.immatricul);
   strcpy(numero, 	 src.numero);
   strcpy(patient,	 src.patient);
   strcpy(nomPatro, 	 src.nomPatro);
   strcpy(nomUsuel, 	 src.nomUsuel);
   strcpy(prenom, 	 src.prenom);
   strcpy(adresse, 	 src.adresse);
   strcpy(dateNais, 	 src.dateNais);
   strcpy(rangGem, 	 src.rangGem);
   strcpy(qualite, 	 src.qualite);
   strcpy(droits, 	 src.droits);
   strcpy(ald, 		 src.ald);
   strcpy(situation,  src.situation);
   strcpy(mutuelle, 	 src.mutuelle);
   strcpy(droitsMut,  src.droitsMut);
   strcpy(garanties,  src.garanties);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale2Data::operator == (const NSVitale2Data& o)
{
	if ((strcmp(immatricul, o.immatricul) 	== 0) &&
   	 (strcmp(numero, 		o.numero)     	== 0) &&
       (strcmp(patient,	 	o.patient)     == 0) &&
       (strcmp(nomPatro,	o.nomPatro)    == 0) &&
       (strcmp(nomUsuel,	o.nomUsuel)    == 0) &&
       (strcmp(prenom,	 	o.prenom)      == 0) &&
       (strcmp(adresse,	 	o.adresse)     == 0) &&
       (strcmp(dateNais,	o.dateNais)    == 0) &&
       (strcmp(rangGem,	 	o.rangGem)     == 0) &&
       (strcmp(qualite,	 	o.qualite)     == 0) &&
       (strcmp(droits,	 	o.droits)      == 0) &&
       (strcmp(ald,	 		o.ald)     		== 0) &&
       (strcmp(situation,	o.situation)   == 0) &&
       (strcmp(mutuelle,	o.mutuelle)    == 0) &&
       (strcmp(droitsMut,	o.droitsMut)   == 0) &&
       (strcmp(garanties,	o.garanties)   == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSVitale2Data::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(immatricul, 0, CSV2_IMMATRICUL_LEN + 1);
  memset(numero, 	   0, CSV2_NUMERO_LEN + 1);
  memset(patient,	   0, CSV2_PATIENT_LEN + 1);
	memset(nomPatro, 	 0, CSV2_NOM_PATRO_LEN + 1);
	memset(nomUsuel, 	 0, CSV2_NOM_USUEL_LEN + 1);
	memset(prenom, 	   0, CSV2_PRENOM_LEN + 1);
	memset(adresse, 	 0, CSV2_ADRESSE_LEN + 1);
	memset(dateNais, 	 0, CSV2_DATE_NAIS_LEN + 1);
	memset(rangGem, 	 0, CSV2_RANG_GEM_LEN + 1);
	memset(qualite, 	 0, CSV2_QUALITE_LEN + 1);
	memset(droits, 	   0, CSV2_DROITS_LEN + 1);
	memset(ald, 		   0, CSV2_ALD_LEN + 1);
	memset(situation,  0, CSV2_SITUATION_LEN + 1);
	memset(mutuelle, 	 0, CSV2_MUTUELLE_LEN + 1);
	memset(droitsMut,  0, CSV2_DROITS_MUT_LEN + 1);
	memset(garanties,  0, CSV2_GARANTIES_LEN + 1);
}

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSVitale2::NSVitale2(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
	// Crée un objet de données
	//
	pDonnees = new NSVitale2Data();
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale2::NSVitale2(const NSVitale2& rv)
          :NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale2Data();
	//
	// Copie les valeurs du NSTransaction d'origine
	//
	*pDonnees = *(rv.pDonnees);
}


//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale2::~NSVitale2()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSVitale2::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSVitale2::alimenteFiche()
{
   alimenteChamp(pDonnees->immatricul, CSV2_IMMATRICUL_FIELD, CSV2_IMMATRICUL_LEN);
   alimenteChamp(pDonnees->numero, 	 	CSV2_NUMERO_FIELD, 	  CSV2_NUMERO_LEN);
   alimenteChamp(pDonnees->patient,		CSV2_PATIENT_FIELD,	  CSV2_PATIENT_LEN);
	alimenteChamp(pDonnees->nomPatro, 	CSV2_NOM_PATRO_FIELD,  CSV2_NOM_PATRO_LEN);
	alimenteChamp(pDonnees->nomUsuel, 	CSV2_NOM_USUEL_FIELD,  CSV2_NOM_USUEL_LEN);
	alimenteChamp(pDonnees->prenom, 	 	CSV2_PRENOM_FIELD, 	  CSV2_PRENOM_LEN);
	alimenteChamp(pDonnees->adresse, 	CSV2_ADRESSE_FIELD, 	  CSV2_ADRESSE_LEN);
	alimenteChamp(pDonnees->dateNais, 	CSV2_DATE_NAIS_FIELD,  CSV2_DATE_NAIS_LEN);
	alimenteChamp(pDonnees->rangGem, 	CSV2_RANG_GEM_FIELD,   CSV2_RANG_GEM_LEN);
	alimenteChamp(pDonnees->qualite, 	CSV2_QUALITE_FIELD, 	  CSV2_QUALITE_LEN);
	alimenteChamp(pDonnees->droits, 	 	CSV2_DROITS_FIELD, 	  CSV2_DROITS_LEN);
	alimenteChamp(pDonnees->ald, 		 	CSV2_ALD_FIELD, 		  CSV2_ALD_LEN);
	alimenteChamp(pDonnees->situation,  CSV2_SITUATION_FIELD,  CSV2_SITUATION_LEN);
	alimenteChamp(pDonnees->mutuelle, 	CSV2_MUTUELLE_FIELD,   CSV2_MUTUELLE_LEN);
	alimenteChamp(pDonnees->droitsMut,  CSV2_DROITS_MUT_FIELD, CSV2_DROITS_MUT_LEN);
	alimenteChamp(pDonnees->garanties,  CSV2_GARANTIES_FIELD,  CSV2_GARANTIES_LEN);
}

//---------------------------------------------------------------------------
//  Fonction :   	NSVitale2::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSVitale2::videFiche()
{
   videChamp(pDonnees->immatricul, CSV2_IMMATRICUL_FIELD, CSV2_IMMATRICUL_LEN);
   videChamp(pDonnees->numero, 	  CSV2_NUMERO_FIELD, 	 CSV2_NUMERO_LEN);
	videChamp(pDonnees->patient,	  CSV2_PATIENT_FIELD,	 CSV2_PATIENT_LEN);
	videChamp(pDonnees->nomPatro,   CSV2_NOM_PATRO_FIELD,  CSV2_NOM_PATRO_LEN);
	videChamp(pDonnees->nomUsuel,   CSV2_NOM_USUEL_FIELD,  CSV2_NOM_USUEL_LEN);
	videChamp(pDonnees->prenom, 	  CSV2_PRENOM_FIELD, 	 CSV2_PRENOM_LEN);
	videChamp(pDonnees->adresse, 	  CSV2_ADRESSE_FIELD, 	 CSV2_ADRESSE_LEN);
	videChamp(pDonnees->dateNais,   CSV2_DATE_NAIS_FIELD,  CSV2_DATE_NAIS_LEN);
	videChamp(pDonnees->rangGem, 	  CSV2_RANG_GEM_FIELD,   CSV2_RANG_GEM_LEN);
	videChamp(pDonnees->qualite, 	  CSV2_QUALITE_FIELD, 	 CSV2_QUALITE_LEN);
	videChamp(pDonnees->droits, 	  CSV2_DROITS_FIELD, 	 CSV2_DROITS_LEN);
	videChamp(pDonnees->ald, 		  CSV2_ALD_FIELD, 		 CSV2_ALD_LEN);
	videChamp(pDonnees->situation,  CSV2_SITUATION_FIELD,  CSV2_SITUATION_LEN);
	videChamp(pDonnees->mutuelle,   CSV2_MUTUELLE_FIELD,   CSV2_MUTUELLE_LEN);
	videChamp(pDonnees->droitsMut,  CSV2_DROITS_MUT_FIELD, CSV2_DROITS_MUT_LEN);
	videChamp(pDonnees->garanties,  CSV2_GARANTIES_FIELD,  CSV2_GARANTIES_LEN);
}
//---------------------------------------------------------------------------//  Fonction :  	NSVitale2::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSVitale2::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
	  return (lastError = ERROR_TABLENOTOPEN) ;

	//	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK) ;

	return lastError ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSVitale2::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSVitale2::open()
{
	char tableName[] = "CARTE_SV2.DB";
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSVitale2::Create()
//
//---------------------------------------------------------------------------
bool
NSVitale2::Create()
{
	return true;
}

//---------------------------------------------------------------------------
//  Function:  NSVitale2::Modify()
//
//---------------------------------------------------------------------------
bool
NSVitale2::Modify()
{
	return true;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVitale2&
NSVitale2::operator=(const NSVitale2& src)
{  if (this == &src)  	return *this ;
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale2::operator == (const NSVitale2& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//---------------------------------------------------------------------------
//  Fonction:		NSVitale2Info::NSVitale2Info()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSVitale2Info::NSVitale2Info()
{
	// Crée l'objet de données
	pDonnees = new NSVitale2Data();
}

//---------------------------------------------------------------------------
//  Fonction:		NSVitale2Info::NSVitale2Info(NSVitale2*)
//
//  Description:	Constructeur à partir d'un NSTransaction
//---------------------------------------------------------------------------
NSVitale2Info::NSVitale2Info(NSVitale2* pTransac)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale2Data();
	//
	// Copie les valeurs du NSVitale2
	//
	*pDonnees = *(pTransac->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale2Info::NSVitale2Info(const NSVitale2Info& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSVitale2Data();
	//
	// Copie les valeurs du NSVitale2Info d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSVitale2Info::~NSVitale2Info()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSVitale2Info&
NSVitale2Info::operator=(const NSVitale2Info& src)
{  if (this == &src)  	return *this ;
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSVitale2Info::operator == (const NSVitale2Info& o)
{
	 return (*pDonnees == *(o.pDonnees));
}

//***************************************************************************
// 					Implémentation des méthodes NSVitale2Array
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVitale2Array::NSVitale2Array(NSVitale2Array& rv) : NSCV2Array()
{
	for (NSCV2Iter i = rv.begin(); i != rv.end(); i++)
   	push_back(new NSVitale2Info(*(*i)));
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSVitale2Array::vider()
{
	for (NSCV2Iter i = begin(); i != end(); )
   {
   	delete *i;
      erase(i);
   }
}


NSVitale2Array::~NSVitale2Array()
{
	vider();
}

