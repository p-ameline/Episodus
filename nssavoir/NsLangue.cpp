//---------------------------------------------------------------------------//  Implémentation de objets NAUTILUS
//---------------------------------------------------------------------------

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "nautilus\nssuper.h"
#include "nssavoir\nslangue.h"

//***************************************************************************
// Implémentation des méthodes NSLanguesData
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur.
//---------------------------------------------------------------------------
NSLanguesData::NSLanguesData()
{
  memset(code,    0, LANGUES_CODE_LEN + 1) ;
	memset(libelle, 0, LANGUES_LIBEL_LEN + 1) ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSLanguesData::NSLanguesData(const NSLanguesData& rv)
{
	strcpy(code,    rv.code) ;
	strcpy(libelle, rv.libelle) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSLanguesData::~NSLanguesData()
{
}

//---------------------------------------------------------------------------//  Opérateur =
//---------------------------------------------------------------------------
NSLanguesData&
NSLanguesData::operator=(const NSLanguesData& src)
{
  if (this == &src)
    return *this ;

	strcpy(code,    src.code) ;
	strcpy(libelle, src.libelle) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLanguesData::operator == ( const NSLanguesData& o )
{
  if ((strcmp(code,    o.code)    == 0) &&
      (strcmp(libelle, o.libelle) == 0))
		return 1 ;

  return 0 ;
}

//***************************************************************************// Implémentation des méthodes NSLangues
//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur
//---------------------------------------------------------------------------
NSLangues::NSLangues(NSSuper* pSuper) : NSFiche(pSuper)
{
  pDonnees = new NSLanguesData() ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSLangues::NSLangues(const NSLangues& rv) : NSFiche(rv._pSuper)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSLanguesData();
	//
	// Copie les valeurs du NSEncycloInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSLangues::~NSLangues()
{
  delete pDonnees ;
}

//---------------------------------------------------------------------------//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSLangues::alimenteFiche()
{
  alimenteChamp(pDonnees->code,    LANGUES_CODE_FIELD,  LANGUES_CODE_LEN) ;
  alimenteChamp(pDonnees->libelle, LANGUES_LIBEL_FIELD, LANGUES_LIBEL_LEN) ;
}

//---------------------------------------------------------------------------//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSLangues::videFiche()
{
    videChamp(pDonnees->code,    LANGUES_CODE_FIELD,  LANGUES_CODE_LEN);
    videChamp(pDonnees->libelle, LANGUES_LIBEL_FIELD, LANGUES_LIBEL_LEN);
}

//---------------------------------------------------------------------------//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSLangues::open()
{
	char tableName[] = "LANGUES.DB" ;
	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;
	return(lastError) ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSLangues&
NSLangues::operator=(const NSLangues& src)
{
  if (this == &src)
    return *this ;

  *pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLangues::operator==(const NSLangues& o)
{
  return (*pDonnees == *(o.pDonnees)) ;}

//***************************************************************************// Implémentation des méthodes NSLanguesInfo
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSLanguesInfo::NSLanguesInfo()
{
  pDonnees = new NSLanguesData() ;
}

//---------------------------------------------------------------------------//  Constructeur à partir d'un NSPatient
//---------------------------------------------------------------------------
NSLanguesInfo::NSLanguesInfo(const NSLangues* pLangue)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSLanguesData() ;
	//
	// Copie les valeurs du NSLangues
	//
	*pDonnees = *(pLangue->pDonnees) ;
}

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSLanguesInfo::NSLanguesInfo(const NSLanguesInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSLanguesData() ;
	//
	// Copie les valeurs du NSEncycloInfo d'origine
	//
	*pDonnees = *(rv.pDonnees) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSLanguesInfo::~NSLanguesInfo()
{
  delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSLanguesInfo&
NSLanguesInfo::operator=(const NSLanguesInfo& src)
{
  if (this == &src)
    return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLanguesInfo::operator==(const NSLanguesInfo& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

