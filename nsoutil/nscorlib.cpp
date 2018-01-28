//---------------------------------------------------------------------------
//    NSMATFIC.CPP
//    PA   juillet 95
//  Implémentation des objets materiel
//---------------------------------------------------------------------------
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\docmanag.h>
#include <owl\olemdifr.h>

#include <owl\applicat.h>
#include <owl\window.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>

#include "partage\nsglobal.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"


#include "nsoutil\nscorlib.h"

//***************************************************************************
// 				Implémentation des méthodes NSCorLibre
//***************************************************************************

//---------------------------------------------------------------------------
//  metAZero()
//
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSCorLibreData::metAZero()
{
  	//
  	// Met les champs de données à zéro
  	//
  	memset(nss,    0, CLI_NSS_LEN + 1);
  	memset(ligne1, 0, CLI_LIGNE1_LEN + 1);
  	memset(ligne2, 0, CLI_LIGNE2_LEN + 1);
  	memset(ligne3, 0, CLI_LIGNE3_LEN + 1);
}

//---------------------------------------------------------------------------
//  metABlanc()
//
//  Met à blanc les variables de la fiche
//---------------------------------------------------------------------------
void
NSCorLibreData::metABlanc()
{
  	//
  	// Met les champs de données à blanc
  	//
   memset(nss,    ' ', CLI_NSS_LEN);
  	memset(ligne1, ' ', CLI_LIGNE1_LEN);
  	memset(ligne2, ' ', CLI_LIGNE2_LEN);
  	memset(ligne3, ' ', CLI_LIGNE3_LEN);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCorLibreData::NSCorLibreData(NSCorLibreData& rv)
{
	strcpy(nss,		rv.nss);
	strcpy(ligne1,	rv.ligne1);
   strcpy(ligne2,	rv.ligne2);
   strcpy(ligne3,	rv.ligne3);
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCorLibreData& NSCorLibreData::operator=(NSCorLibreData src)
{
	strcpy(nss,		src.nss);
	strcpy(ligne1,	src.ligne1);
   strcpy(ligne2,	src.ligne2);
   strcpy(ligne3,	src.ligne3);

	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCorLibreData::operator == ( const NSCorLibreData& o )
{
	if (strcmp(nss, o.nss) == 0)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Constructeur.
//---------------------------------------------------------------------------
NSCorLibre::NSCorLibre(NSContexte* pCtx)
			  :NSFiche(pCtx)
{
  	//
  	// Crée un objet de données
  	//
  	pDonnees = new NSCorLibreData();
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCorLibre::~NSCorLibre()
{
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  alimenteFiche()
//
//  Transfère le contenu de pRecBuff dans les variables de
//  la fiche
//---------------------------------------------------------------------------
void
NSCorLibre::alimenteFiche()
{
	alimenteChamp(pDonnees->nss, 	  CLI_NSS_FIELD, 	  CLI_NSS_LEN);
  	alimenteChamp(pDonnees->ligne1, CLI_LIGNE1_FIELD, CLI_LIGNE1_LEN);
   alimenteChamp(pDonnees->ligne2, CLI_LIGNE2_FIELD, CLI_LIGNE2_LEN);
   alimenteChamp(pDonnees->ligne3, CLI_LIGNE3_FIELD, CLI_LIGNE3_LEN);
}

//---------------------------------------------------------------------------
//  videFiche()
//
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCorLibre::videFiche()
{
	videChamp(pDonnees->nss, 	 CLI_NSS_FIELD, 	 CLI_NSS_LEN);
  	videChamp(pDonnees->ligne1, CLI_LIGNE1_FIELD, CLI_LIGNE1_LEN);
   videChamp(pDonnees->ligne2, CLI_LIGNE2_FIELD, CLI_LIGNE2_LEN);
   videChamp(pDonnees->ligne3, CLI_LIGNE3_FIELD, CLI_LIGNE3_LEN);
}

//---------------------------------------------------------------------------
//  Function:    NSMateriel::open()
//
//  Description: Ouvre la table primaire et les index secondaires
//
//  Returns:     PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSCorLibre::open()
{
  	char tableName[] = "CORLIBRE.DB";
  	//
  	// Appelle la fonction open() de la classe de base pour ouvrir
  	// l'index primaire
  	//
  	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSCorLibreInfo::NSCorLibreInfo()
{
	// Crée l'objet de données
	pDonnees = new NSCorLibreData();
}

//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSCorLibre
//---------------------------------------------------------------------------
NSCorLibreInfo::NSCorLibreInfo(NSCorLibre* pCorLibre)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCorLibreData();
	//
	// Copie les valeurs du NSMateriel
	//
	*pDonnees = *(pCorLibre->pDonnees);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCorLibreInfo::NSCorLibreInfo(NSCorLibreInfo& rv)
{
	//
	// Crée l'objet de données
	//
	pDonnees = new NSCorLibreData();
	//
	// Copie les valeurs du NSMaterielInfo d'origine
	//
	*pDonnees = *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCorLibreInfo&
NSCorLibreInfo::operator=(NSCorLibreInfo src)
{
	*pDonnees = *(src.pDonnees);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCorLibreInfo::operator == ( const NSCorLibreInfo& o )
{
	return (*pDonnees == *(o.pDonnees));
}


