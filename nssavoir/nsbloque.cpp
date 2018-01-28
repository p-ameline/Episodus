//---------------------------------------------------------------------------//  NSBLOQUES.CPP
//  PA   juin 98
//  Implémentation du fichier de blocage de patients
//---------------------------------------------------------------------------

#include <windows.h>
#include <mem.h>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsdn\nsdocdlg.h"

#include "nssavoir\nsbloque.h"

//---------------------------------------------------------------------------// Implémentation des méthodes NSBloques
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
//  Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSBloquesData::metAZero()
{
	// Met les champs de données à zéro
	memset(patient,	  0, BLOQ_PATIENT_LEN + 1);
	memset(utilisateur, 0, BLOQ_UTILISATEUR_LEN + 1);
	memset(console, 	  0, BLOQ_CONSOLE_LEN + 1);
    memset(instance,    0, BLOQ_INSTANCE_LEN + 1);
    memset(dateHeure,   0, BLOQ_DATE_HEURE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Met à blanc les variables de la fiche
//---------------------------------------------------------------------------
void
NSBloquesData::metABlanc()
{
  	// Met les champs de données à blanc
  	memset(patient,	  ' ', BLOQ_PATIENT_LEN);
  	memset(utilisateur, ' ', BLOQ_UTILISATEUR_LEN);
  	memset(console, 	  ' ', BLOQ_CONSOLE_LEN);
    memset(instance,    ' ', BLOQ_INSTANCE_LEN);
    memset(dateHeure,   ' ', BLOQ_DATE_HEURE_LEN);
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSBloquesData&
NSBloquesData::operator=(NSBloquesData src)
{
	strcpy(patient,	  src.patient);
	strcpy(utilisateur, src.utilisateur);
	strcpy(console, 	  src.console);
    strcpy(instance,    src.instance);
    strcpy(dateHeure,   src.dateHeure);
	return *this;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSBloquesData::operator==(const NSBloquesData& o)
{
	 if ((strcmp(patient,  	  o.patient) 	  == 0) &&
		  (strcmp(utilisateur, o.utilisateur) == 0) &&
		  (strcmp(console, 	  o.console)	  == 0) &&
          (strcmp(instance,   o.instance)     == 0))
		  return 1;
	 else
		  return 0;
}

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBloques::NSBloques(NSContexte* pCtx)
			 :NSFiche(pCtx)
{
	// Crée un objet Données
	pDonnees = new NSBloquesData();
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSBloques::~NSBloques()
{
	// Détruit l'objet Données
	delete pDonnees;
}

//---------------------------------------------------------------------------
//  alimenteFiche()
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSBloques::alimenteFiche()
{
	alimenteChamp(pDonnees->patient, 	 BLOQ_PATIENT_FIELD, 	 BLOQ_PATIENT_LEN);
   alimenteChamp(pDonnees->utilisateur, BLOQ_UTILISATEUR_FIELD, BLOQ_UTILISATEUR_LEN);
   alimenteChamp(pDonnees->console, 	 BLOQ_CONSOLE_FIELD, 	 BLOQ_CONSOLE_LEN);
   alimenteChamp(pDonnees->instance,     BLOQ_INSTANCE_FIELD,    BLOQ_INSTANCE_LEN);
   alimenteChamp(pDonnees->dateHeure, 	 BLOQ_DATE_HEURE_FIELD,  BLOQ_DATE_HEURE_LEN);
}

//---------------------------------------------------------------------------
//  videFiche()
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSBloques::videFiche()
{
	videChamp(pDonnees->patient, 	 BLOQ_PATIENT_FIELD, 	 BLOQ_PATIENT_LEN);
   videChamp(pDonnees->utilisateur, BLOQ_UTILISATEUR_FIELD, BLOQ_UTILISATEUR_LEN);
   videChamp(pDonnees->console, 	 BLOQ_CONSOLE_FIELD, 	 BLOQ_CONSOLE_LEN);
   videChamp(pDonnees->instance,     BLOQ_INSTANCE_FIELD,    BLOQ_INSTANCE_LEN);
   videChamp(pDonnees->dateHeure, 	 BLOQ_DATE_HEURE_FIELD,  BLOQ_DATE_HEURE_LEN);
}

//---------------------------------------------------------------------------
//  open()
//  Ouvre la table
//  Renvoie : DBIResult
//---------------------------------------------------------------------------
DBIResult
NSBloques::open()
{#ifndef _MUE
	char tableName[] = "BLOQUES.DB";#else    char tableName[] = "NSMBLOQUES.DB";
#endif

  	// Appelle la fonction open() de la classe de base
  	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
  	return(lastError);
}

