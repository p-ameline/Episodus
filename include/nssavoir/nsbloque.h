//---------------------------------------------------------------------------
//     NSBLOQUE.H
//
//  Contient les définitions des objets de blocage
//
//  1ere version : PA juin 98   Dernière modif : 02/06/98
//---------------------------------------------------------------------------
#ifndef __NSBLOQUE_H
#define __NSBLOQUE_H

#include <cstring.h>#include "partage\nsperson.h"

#if defined(_DANSLEXIDLL)
	#define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

//
// Taille des champs du fichier BLOQUES
//
#define BLOQ_PATIENT_LEN  		PAT_NSS_LEN
#define BLOQ_UTILISATEUR_LEN    3
#define BLOQ_CONSOLE_LEN		2
#define BLOQ_INSTANCE_LEN       2
#define BLOQ_DATE_HEURE_LEN     14
//
// Ordre des champs du fichier BLOQUES
//
#define BLOQ_PATIENT_FIELD  	1
#define BLOQ_UTILISATEUR_FIELD  2
#define BLOQ_CONSOLE_FIELD		3
#define BLOQ_INSTANCE_FIELD     4
#define BLOQ_DATE_HEURE_FIELD   5

//---------------------------------------------------------------------------
//  Classe NSBloques
//---------------------------------------------------------------------------
//
// Objet données
//
class  NSBloquesData
{
	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
     	char patient[BLOQ_PATIENT_LEN + 1];
     	char utilisateur[BLOQ_UTILISATEUR_LEN + 1];
	  	char console[BLOQ_CONSOLE_LEN + 1];
        char instance[BLOQ_INSTANCE_LEN + 1];
	  	char dateHeure[BLOQ_DATE_HEURE_LEN + 1];

	  	void metABlanc();
	  	void metAZero();

	  	NSBloquesData() { metAZero(); }
	  	NSBloquesData(NSBloquesData& rv);

	  	NSBloquesData& operator=(NSBloquesData src);
	  	int 			  	operator==(const NSBloquesData& o);
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class _CLASSELEXI NSBloques : public NSFiche
{
	public :

        NSBloquesData* pDonnees;

   	    NSBloques(NSContexte* pCtx); // Constructeur ordinaire
	  	~NSBloques();

	  	void alimenteFiche();
	  	void videFiche();
	  	DBIResult open();
};

//---------------------------------------------------------------------------
#endif    // __NSBLOQUE_H
