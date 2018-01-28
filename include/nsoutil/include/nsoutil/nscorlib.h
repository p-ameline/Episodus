//---------------------------------------------------------------------------
//     NSMATFIC.H
//     PA   juillet 95
//  Contient les définitions des objets Matériel
//---------------------------------------------------------------------------
#ifndef __NSCORLIB_H
#define __NSCORLIB_H

#include <classlib\arrays.h>
#include "partage\ns_vector.h"

#include <owl\dialog.h>
#include <owl\listbox.h>

#include "nssavoir\ns_medic.h"

//
// Taille des champs du fichier CORLIBRE
//
#define CLI_NSS_LEN 		 6
#define CLI_LIGNE1_LEN 	25
#define CLI_LIGNE2_LEN 	25
#define CLI_LIGNE3_LEN 	25

//
// Ordre des champs du fichier MATERIEL
//
#define CLI_NSS_FIELD 		1
#define CLI_LIGNE1_FIELD 	2
#define CLI_LIGNE2_FIELD 	3
#define CLI_LIGNE3_FIELD 	4

//---------------------------------------------------------------------------
//  Classe Materiel
//      ANCETRE : NSFiche
//---------------------------------------------------------------------------
//
// Objet données des Objets Corlibre
//
class NSCorLibreData
{
	public :
   	//
	  	// Variables de stockage des valeurs
	  	//
	  	char nss[CLI_NSS_LEN + 1];
	  	char ligne1[CLI_LIGNE1_LEN + 1];
	  	char ligne2[CLI_LIGNE2_LEN + 1];
	  	char ligne3[CLI_LIGNE3_LEN + 1];

	  	void metABlanc();
	  	void metAZero();

	  	NSCorLibreData() { metAZero(); }
	  	NSCorLibreData(NSCorLibreData& rv);

	  	NSCorLibreData& operator=(NSCorLibreData src);
	  	int   		      operator==(const NSCorLibreData& o);
};

class NSCorLibre : public NSFiche
{
	public :

   	NSCorLibreData* 	pDonnees;	  // Données de la fiche PARADOX

	  	NSCorLibre(NSContexte* pCtx);
	  	~NSCorLibre();

	  	DBIResult open();
	  	void alimenteFiche();
	  	void videFiche();
};

//
// Objet "Info" correspondant à NSCorLibre
// (destiné à être utilisé dans les containers)
//
class NSCorLibreInfo
{
	public:

		NSCorLibreData* pDonnees;

		NSCorLibreInfo();
		NSCorLibreInfo(NSCorLibre*);
		NSCorLibreInfo(NSCorLibreInfo& rv);

		NSCorLibreInfo& operator=(NSCorLibreInfo src);
		int operator == ( const NSCorLibreInfo& o );
};

#endif    // __NSCORLIB_H
