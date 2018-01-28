//---------------------------------------------------------------------------//     NSPATBD.H
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
#ifndef __NSPATBD_H
#define __NSPATBD_H

class NSPatPathoInfo ;

#include "partage\ns_vector.h"
#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif
#include "nssavoir\nspatbas.h"
#include "nssavoir\nstransa.h"

// #include "nsbb\nsbbtran.h"
// #include "nssavoir\ns_fic.h"

//
// Indice des champs du fichier PATPADAT
//
#define PPD_PAT_FIELD         1
#define PPD_DOCU_FIELD        2
#define PPD_NOEUD_FIELD       3
#define PPD_TYPE_FIELD        4
#define PPD_LEXIQUE_FIELD     5
#define PPD_COMPLEMENT_FIELD  6
#define PPD_CERTITUDE_FIELD   7
#define PPD_PLURIEL_FIELD     8
#define PPD_TRAN_NEW_FIELD    9
#define PPD_TRAN_DEL_FIELD   10
#define PPD_UNIT_FIELD       11

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
#define PPL_INTERET_FIELD 7

enum PPDTYPE {ppdIndef, ppdActif, ppdHistorique};

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
class _NSBBCLASSE NSPatPaDatData
{
  public :

    char codePatient[PIDS_NSS_LEN + 1] ;
    char codeDocument[DOC_CODE_DOCUM_LEN + 1] ;
    char noeud[PPD_NOEUD_LEN + 1] ;
    char type[BASE_TYPE_LEN + 1] ;
    char lexique[BASE_LEXIQUE_LEN + 1] ;
    char complement[BASE_COMPLEMENT_LEN + 1] ;
    char certitude[BASE_CERTITUDE_LEN + 1] ;
    char pluriel[BASE_PLURIEL_LEN + 1] ;
    char tran_new[TRAN_CODE_LEN + 1] ;
    char tran_del[TRAN_CODE_LEN + 1] ;
    char unit[BASE_UNIT_LEN + 1] ;

		NSPatPaDatData() { metAZero() ; }
		NSPatPaDatData(const NSPatPaDatData& rv) ;

		NSPatPaDatData& operator=(const NSPatPaDatData& src) ;
		int             operator==(const NSPatPaDatData& o) ;

		void metAZero() ;

    int  getType() ;
    void setType(int newType) ;

    string getID()   { return string(codePatient) + string(codeDocument) ; }
    string getNode() { return string(codePatient) + string(codeDocument) + string(noeud) ; }
};

class _NSBBCLASSE NSPatPaDat : public NSFiche
{
  public :

    NSPatPaDatData* pDonnees ;   // Objet qui contient les données

    NSPatPaDat(NSSuper* pSuper) ;
    ~NSPatPaDat() ;

    DBIResult open() ;
    void alimenteFiche() ;
    void videFiche() ;

    short prendStatut() { return statut ; }
    void  metStatut(short nouvStatut) { statut = nouvStatut ; }

    virtual bool Create() ;
    virtual bool Modify() ;
};

class _NSBBCLASSE NSPatPaDatInfo
{
  public :
    //
		// Objet qui contient les données
		//
		NSPatPaDatData* pDonnees ;

		NSPatPaDatInfo() ;
		NSPatPaDatInfo(const NSPatPaDat* pPatPaDat) ;
		NSPatPaDatInfo(const NSPatPaDatInfo& rv) ;

    string getID()   { return pDonnees->getID() ; }
    string getNode() { return pDonnees->getNode() ; }

		NSPatPaDatInfo& operator=(const NSPatPaDatInfo& src) ;
		int operator == (const NSPatPaDatInfo& o) ;
    int operator == (const NSPatPathoInfo& o) ;
    ~NSPatPaDatInfo() ;
};

typedef vector<NSPatPaDatInfo*>             NSPatPaDatInfoArray ;
typedef NSPatPaDatInfoArray::iterator       PatPaDatIter ;
typedef NSPatPaDatInfoArray::const_iterator PatPaDatConstIter ;

class _NSBBCLASSE NSPatPaDatArray : public NSPatPaDatInfoArray, public NSSuperRoot
{
  public :

    //
    // Constructeurs
    //
    NSPatPaDatArray(NSSuper* pSuper = 0) ;
    NSPatPaDatArray(const NSPatPaDatArray& rv) ;
    //
    // Destructeur
    //
    ~NSPatPaDatArray() ;
    void vider() ;
    //
    // Opérateurs
    //
    NSPatPaDatArray& operator=(const NSPatPaDatArray& src) ;
    //
    // Fonctions de gestion du vecteur
    //
    bool ajouteElement(NSPatPaDatInfo* pPatPaDat) ;
};

//---------------------------------------------------------------------------
//  Classe NSPatPaLoc
//
//  La classe NSPatPaLoc permet de connaitre la localisation
//  d'un noeud de NSPatPaDat pour une transaction donnée
//
//  Le champ visible permet de savoir comment présenter la patpatho
//
//---------------------------------------------------------------------------
class _NSBBCLASSE NSPatPaLocData
{
  public :

    char codePatient[PIDS_NSS_LEN + 1] ;
    char codeDocument[DOC_CODE_DOCUM_LEN + 1] ;
    char transaction[TRAN_CODE_LEN + 1] ;
    char noeud[PPD_NOEUD_LEN + 1] ;
    char codeLocalisation[BASE_LOCALISATION_LEN + 1] ;
    char visible[BASE_VISIBLE_LEN + 1] ;
    char interet[BASE_INTERET_LEN + 1] ;

		NSPatPaLocData() { metAZero() ; }
		NSPatPaLocData(const NSPatPaLocData& rv) ;

		NSPatPaLocData& operator=(const NSPatPaLocData& src) ;
		int             operator==(const NSPatPaLocData& o) ;

		void metAZero() ;

    string getID() { return string(codePatient) + string(codeDocument) ; }
};

class _NSBBCLASSE NSPatPaLoc : public NSFiche
{
  public :

    NSPatPaLocData* pDonnees ;   // Objet qui contient les données

    NSPatPaLoc(NSSuper* pSuper) ;
    ~NSPatPaLoc() ;

    DBIResult open() ;
    void alimenteFiche() ;
    void videFiche() ;

    short prendStatut() { return statut ; }
    void  metStatut(short nouvStatut) { statut = nouvStatut ; }

    virtual bool Create() ;
    virtual bool Modify() ;
};

class _NSBBCLASSE NSPatPaLocInfo
{
    public :
        //
		// Objet qui contient les données
		//
		NSPatPaLocData* pDonnees ;

		NSPatPaLocInfo() ;
		NSPatPaLocInfo(const NSPatPaLoc* pPatPaLoc) ;
		NSPatPaLocInfo(const NSPatPaLocInfo& rv) ;

		NSPatPaLocInfo& operator=(const NSPatPaLocInfo& src) ;
		int operator == (const NSPatPaLocInfo& o) ;
    ~NSPatPaLocInfo() ;

    string getID() { return pDonnees->getID() ; }
};

typedef vector<NSPatPaLocInfo*>             NSPatPaLocInfoArray ;
typedef NSPatPaLocInfoArray::iterator       PatPaLocIter ;
typedef NSPatPaLocInfoArray::const_iterator PatPaLocConstIter ;

class _NSBBCLASSE NSPatPaLocArray : public NSPatPaLocInfoArray, public NSSuperRoot
{
  public :

    //
    // Constructeurs
    //
    NSPatPaLocArray(NSSuper* pSuper = 0) ;
    NSPatPaLocArray(const NSPatPaLocArray& rv) ;
    //
    // Destructeur
    //
    ~NSPatPaLocArray() ;
    void vider() ;
    //
    // Opérateurs
    //
    NSPatPaLocArray& operator=(const NSPatPaLocArray& src) ;
    //
    // Fonctions de gestion du vecteur
    //
    bool ajouteElement(NSPatPaLocInfo* pPatPaLoc) ;
};

//---------------------------------------------------------------------------
#endif    // _NSPATBD_H

