//---------------------------------------------------------------------------
//     NSPATBD.H
//
//  Contient les définitions des objets de gestion des dociments structurés
//
//  Il existe deux types d'objets :
//
//			- Les objets de la classe patpatho, qui gèrent l'instance du document
//			  pour une transaction donnée (objets utilisés par le noyau)
//			  DANS NSPATPAT.H
//
//			- Les objets patpaDat et patpaLoc qui stockent ces patpatho sur disque
//			  DANS NSPATBD.H
//
//  1ere version : PA juillet 94   Dernière modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSVPATBD_H
#define __NSVPATBD_H

#include "partage\ns_vector.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nspatbas.h"
#include "nssavoir\ns_fic.h"
#include "nssavoir\nstransa.h"

//
// Taille des champs du fichier PATPADAT
//
// #define PPD_NOEUD_LEN  		  4
//
// Indice des champs du fichier PATPADAT
//
#define PPD_PAT_FIELD  		  1
#define PPD_DOCU_FIELD  	  2
#define PPD_NOEUD_FIELD      3
#define PPD_TYPE_FIELD   	  4
#define PPD_LEXIQUE_FIELD    5
#define PPD_COMPLEMENT_FIELD 6
#define PPD_CERTITUDE_FIELD  7
#define PPD_INTERET_FIELD    8
#define PPD_PLURIEL_FIELD    9

//
// Taille des champs du fichier PATPALOC
//
#define PPL_VISIBLE_LEN      1
//
// Indice des champs du fichier PATPALOC
//
#define PPL_PAT_FIELD  	  1
#define PPL_DOCU_FIELD    2
#define PPL_TRAN_FIELD    3
#define PPL_NOEUD_FIELD   4
#define PPL_LOC_FIELD     5
#define PPL_VISIBLE_FIELD 6

//enum PPDTYPE {ppdIndef, ppdActif, ppdHistorique};

//---------------------------------------------------------------------------
//  Classe NSPatPaDat
//
//  La classe NSPatPaDat permet de stocker un noeud de patpatho sur disque
//
//  Pour une transaction donnée, ce noeud peut avoir une localisation
//  (-> il existe dans la patpatho) ou non (-> il n'existe pas).
//
//  Un même noeud peut avoir plusieurs localisations.
//
//  Le champ type permet de savoir si ce noeud existe dans la plus récente
//	 transaction (valeur 'A' pour Actif) ou non (valeur 'H' pour Historique)
//
//---------------------------------------------------------------------------
class NSPatOldDatData
{
    public :

        char codePatient[PAT_NSS_LEN + 1];
        char codeDocument[DOC_CODE_DOCUM_LEN + 1];
        char noeud[PPD_NOEUD_LEN + 1];
        char type[BASE_TYPE_LEN + 1];
   	    char lexique[BASE_LEXIQUE_LEN + 1];
     	char complement[BASE_COMPLEMENT_LEN + 1];
	  	char certitude[BASE_CERTITUDE_LEN + 1];
	  	char interet[BASE_INTERET_LEN + 1];
	  	char pluriel[BASE_PLURIEL_LEN + 1];

		NSPatOldDatData() { metAZero(); }
		NSPatOldDatData(NSPatOldDatData& rv);

		NSPatOldDatData& operator=(NSPatOldDatData src);
		int 			operator==(const NSPatOldDatData& o);

        void metABlanc();
		void metAZero();

        int  getType();
        void setType(int newType);
};

class NSPatPaOldDat : public NSFiche
{
    public :

        NSPatOldDatData* pDonnees;   // Objet qui contient les données

	    NSPatPaOldDat(NSContexte* pCtx);
	    ~NSPatPaOldDat();

        void metABlanc() { pDonnees->metABlanc(); }

	    DBIResult open();
	    void alimenteFiche();
	    void videFiche();

	    short prendStatut() { return statut; }
	    void  metStatut(short nouvStatut) { statut = nouvStatut; }

	    virtual bool Create();
	    virtual bool Modify();
};

class NSPatPaOldDatInfo
{
    public :
        //
		// Objet qui contient les données
		//
		NSPatOldDatData* pDonnees;

		NSPatPaOldDatInfo();
		NSPatPaOldDatInfo(NSPatPaOldDat* pPatPaDat);
		NSPatPaOldDatInfo(NSPatPaOldDatInfo& rv);

		NSPatPaOldDatInfo& operator=(NSPatPaOldDatInfo src);
		int operator == (const NSPatPaOldDatInfo& o);
        int operator == (const NSPatPathoInfo& o);
        ~NSPatPaOldDatInfo();
};

typedef vector<NSPatPaOldDatInfo*> NSPatOldDatInfoArray;
typedef NSPatOldDatInfoArray::iterator PatOldDatIter;

class NSPatOldDatArray : public NSPatOldDatInfoArray, public NSRoot
{
    public :

        //
	    // Constructeurs
        //
	    NSPatOldDatArray(NSContexte* pCtx = 0);
	    NSPatOldDatArray(NSPatOldDatArray& rv);
        //
	    // Destructeur
        //
	    ~NSPatOldDatArray();
        void vider();
        //
        // Opérateurs
        //
        NSPatOldDatArray& operator=(NSPatOldDatArray src);
        //
        // Fonctions de gestion du vecteur
        //
        bool ajouteElement(NSPatPaOldDatInfo* pPatPaDat);
};

//---------------------------------------------------------------------------
//  Classe NSPatOldLoc
//
//  La classe NSPatOldLoc permet de connaitre la localisation
//  d'un noeud de NSPatPaDat pour une transaction donnée
//
//  Le champ visible permet de savoir comment présenter la patpatho
//
//---------------------------------------------------------------------------
class NSPatOldLocData
{
    public :

        char codePatient[PAT_NSS_LEN + 1];
        char codeDocument[DOC_CODE_DOCUM_LEN + 1];
        char transaction[TRAN_CODE_LEN + 1];
        char noeud[PPD_NOEUD_LEN + 1];
        char codeLocalisation[BASE_LOCALISATION_LEN + 1];
        char visible[BASE_VISIBLE_LEN + 1];

		NSPatOldLocData() { metAZero(); }
		NSPatOldLocData(NSPatOldLocData& rv);

		NSPatOldLocData& operator=(NSPatOldLocData src);
		int 			operator==(const NSPatOldLocData& o);

		void metABlanc();
		void metAZero();
};

class NSPatOldLoc : public NSFiche
{
    public :

        NSPatOldLocData* pDonnees;   // Objet qui contient les données

	    NSPatOldLoc(NSContexte* pCtx);
	    ~NSPatOldLoc();

	    void metABlanc() { pDonnees->metABlanc(); }

	    DBIResult open();
	    void alimenteFiche();
	    void videFiche();

	    short prendStatut() { return statut; }
	    void  metStatut(short nouvStatut) { statut = nouvStatut; }

	    virtual bool Create();
	    virtual bool Modify();
};

class NSPatOldLocInfo
{
    public :
        //
		// Objet qui contient les données
		//
		NSPatOldLocData* pDonnees;

		NSPatOldLocInfo();
		NSPatOldLocInfo(NSPatOldLoc* pPatPaLoc);
		NSPatOldLocInfo(NSPatOldLocInfo& rv);

		NSPatOldLocInfo& operator=(NSPatOldLocInfo src);
		int operator == ( const NSPatOldLocInfo& o );
        ~NSPatOldLocInfo();
};

typedef vector<NSPatOldLocInfo*> NSPatOldLocInfoArray;
typedef NSPatOldLocInfoArray::iterator PatPaOldLocIter;

class NSPatOldLocArray : public NSPatOldLocInfoArray, public NSRoot
{
    public :

        //
	    // Constructeurs
        //
	    NSPatOldLocArray(NSContexte* pCtx = 0);
	    NSPatOldLocArray(NSPatOldLocArray& rv);
        //
	    // Destructeur
        //
	    ~NSPatOldLocArray();
        void vider();
        //
        // Opérateurs
        //
        NSPatOldLocArray& operator=(NSPatOldLocArray src);
        //
        // Fonctions de gestion du vecteur
        //
        bool ajouteElement(NSPatOldLocInfo* pPatPaLoc);
};

//---------------------------------------------------------------------------
#endif    // _NSPATBD_H
