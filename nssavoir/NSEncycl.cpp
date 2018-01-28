//---------------------------------------------------------------------------//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------

#include "nautilus\nssuper.h"
#include "nssavoir\nsencycl.h"

//***************************************************************************
// Implémentation des méthodes NSEncyclo
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSEncycloData::NSEncycloData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSEncycloData::NSEncycloData()
{
	//
	// Met les champs de données à zéro
	//
	memset(lexique, 0, ENCYCLO_LEXIQUE_LEN + 1) ;
	memset(url, 		0, ENCYCLO_URL_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloData::NSEncycloData(NSEncycloData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSEncycloData::NSEncycloData(const NSEncycloData& rv)
{
	strcpy(lexique, rv.lexique) ;
	strcpy(url, 		rv.url) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloData::~NSEncycloData()
//
//  Description:	Destructeur
//---------------------------------------------------------------------------
NSEncycloData::~NSEncycloData()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloData::operator=(NSEncycloData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSEncycloData&
NSEncycloData::operator=(const NSEncycloData& src)
{
  if (&src == this)
  	return *this ;

	strcpy(lexique, src.lexique) ;
	strcpy(url, 		src.url) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloData::operator=(NSEncycloData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSEncycloData::operator == ( const NSEncycloData& o )
{
	if ((strcmp(lexique, o.lexique) == 0) &&
		 (strcmp(url, 	o.url) 	  == 0))
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSEncyclo::NSEncyclo(NSSuper* pSuper)
          :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSEncyclo::NSEncyclo(const NSEncyclo& rv)
          :NSFiche(rv._pSuper), Donnees(rv.Donnees)
{
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSEncyclo::~NSEncyclo()
{
}

//---------------------------------------------------------------------------//  Fonction :  	NSEncyclo::alimenteFiche()
//
//  Description : Transfère le contenu de pRecBuff dans les variables de
//             	la fiche
//---------------------------------------------------------------------------
void
NSEncyclo::alimenteFiche()
{
  alimenteChamp(Donnees.lexique, ENCYCLO_LEXIQUE_FIELD, ENCYCLO_LEXIQUE_LEN) ;
  alimenteChamp(Donnees.url, 	   ENCYCLO_URL_FIELD,     ENCYCLO_URL_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :   	NSEncyclo::videFiche()
//
//  Description : Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSEncyclo::videFiche()
{
  videChamp(Donnees.lexique, ENCYCLO_LEXIQUE_FIELD, ENCYCLO_LEXIQUE_LEN) ;
  videChamp(Donnees.url,     ENCYCLO_URL_FIELD,     ENCYCLO_URL_LEN) ;
}

//---------------------------------------------------------------------------
//  Fonction :  	NSEncyclo::getRecord()
//
//  Description : Prend l'enregistrement en cours et assigne la valeur des
//             	champs aux variables membres de la classe.
//
//  Returns:   	PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSEncyclo::getPatRecord()
{
	//
	// La table est-elle ouverte ?
	//
	if (false == _isOpen)
	  return(lastError = ERROR_TABLENOTOPEN);
	//
	// Appel de la classe de base pour récupérer l'enregistrement.
	//
	lastError = getDbiRecord(dbiWRITELOCK);

	return(lastError);
}

//---------------------------------------------------------------------------//  Fonction :  	NSPatient::open()
//
//  Description :	Ouvre la table primaire et les index secondaires
//
//  Returns :		PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSEncyclo::open()
{
	char tableName[] = "ENCYCLOP.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//

#ifdef N_TIERS
	lastError = NSFiche::open(tableName, NSF_GUIDES) ;#else	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;#endif
	return(lastError) ;}

//---------------------------------------------------------------------------//  Function:  NSEncyclo::Create()//
//---------------------------------------------------------------------------
bool
NSEncyclo::Create()
{
	return true ;
}

//---------------------------------------------------------------------------//  Function:  NSEncyclo::Modify()
//
//---------------------------------------------------------------------------
bool
NSEncyclo::Modify()
{
	return true ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSEncyclo&
NSEncyclo::operator=(const NSEncyclo& src)
{
  if (&src == this)
  	return *this ;

	Donnees = src.Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSEncyclo::operator==(const NSEncyclo& o)
{
	 return (Donnees == o.Donnees) ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloInfo::NSEncycloInfo()
//
//  Description:	Constructeur par défaut
//---------------------------------------------------------------------------
NSEncycloInfo::NSEncycloInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSEncycloInfo::NSEncycloInfo(NSEncyclo*)
//
//  Description:	Constructeur à partir d'un NSPatient
//---------------------------------------------------------------------------
NSEncycloInfo::NSEncycloInfo(const NSEncyclo* pPatho)
{
	Donnees = pPatho->Donnees ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSEncycloInfo::NSEncycloInfo(const NSEncycloInfo& rv)
{
	Donnees = rv.Donnees ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSEncycloInfo::~NSEncycloInfo()
{
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSEncycloInfo&
NSEncycloInfo::operator=(const NSEncycloInfo& src)
{
  if (&src == this)
  	return *this ;

	Donnees = src.Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison//---------------------------------------------------------------------------
int
NSEncycloInfo::operator==(const NSEncycloInfo& o)
{
	 return (Donnees == o.Donnees) ;
}

