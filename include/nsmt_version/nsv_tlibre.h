//---------------------------------------------------------------------------
#ifndef __NSV_TLIBRE_H
#define __NSV_TLIBRE_H

#include "nsmt_version\nsv_transcode.h"

//tailles des champs pour le fichier Tlibre.db
#define NSV_TLIBRE_CODE_LEN		  5
#define NSV_TLIBRE_TEXTE_LEN	 40
#define NSV_TLIBRE_SUIVANT_LEN	5

//indices des champs pour le fichier Tlibre.db#define NSV_TLIBRE_CODE_FIELD	   1
#define NSV_TLIBRE_TEXTE_FIELD	 2
#define NSV_TLIBRE_SUIVANT_FIELD 3

// **************************************************************************
//
//                            Classes utilitaires
//
// **************************************************************************

//---------------------------------------------------------------------------// Classe NSVTexteLibreData
//---------------------------------------------------------------------------

class NSVTexteLibreData{
	public :

  	//    // Variables de stockage des valeurs
    //

    char code[NSV_TLIBRE_CODE_LEN + 1] ;
    char texte[NSV_TLIBRE_TEXTE_LEN + 1] ;
    char suivant[NSV_TLIBRE_SUIVANT_LEN + 1] ;

    NSVTexteLibreData(NSVTexteLibreData& rv) ;
    NSVTexteLibreData() { metAZero() ; }		~NSVTexteLibreData() ;

    NSVTexteLibreData& operator=(NSVTexteLibreData src) ;		int 			         operator==(const NSVTexteLibreData& o) ;

		//méthodes		void metAZero() ;
};

//---------------------------------------------------------------------------// Classe NSVTexteLibreBase
//---------------------------------------------------------------------------
class  NSVTexteLibreBase : public NSFiche
{
	public :

		NSVTexteLibreData* pDonnees ;   // Objet qui contient les données
		NSVTexteLibreBase(NSSuper* pSuper) ;		~NSVTexteLibreBase() ;

		virtual DBIResult open(string pathbase) = 0 ;    void metAZero() { pDonnees->metAZero() ; }
		void alimenteFiche() ;		void videFiche() ;
};

//---------------------------------------------------------------------------// Classe NSVTLibreInfo
//---------------------------------------------------------------------------
class NSVTLibreInfo
{
	public :

		//		// Objet qui contient les données
		//
		NSVTexteLibreData* pDonnees ;

		NSVTLibreInfo() ;
		NSVTLibreInfo(NSVTexteLibreBase* pNSVTexteLibre) ;
		NSVTLibreInfo(NSVTLibreInfo& rv) ;
  	~NSVTLibreInfo() ;

		NSVTLibreInfo& operator=(NSVTLibreInfo src) ;
		int            operator == ( const NSVTLibreInfo& o ) ;
};

// **************************************************************************//
//                            Classes de base
//
// **************************************************************************

//---------------------------------------------------------------------------// Classe NSVTLibreBase
//---------------------------------------------------------------------------

class NSVTLibreBase : public NSRoot{
	public :
  	//
    // Variables de stockage des valeurs
    //
    NSVTexteLibreBase* pNSVTexteLibre ;

    NSVTLibreBase(NSContexte* pCtx) ;
    NSVTLibreBase(NSVTLibreBase& rv) ;
    ~NSVTLibreBase() ;

    NSVTLibreBase& operator=(NSVTLibreBase src) ;
    int 	      operator==(const NSVTLibreBase& o) ;

    //méthodes
    virtual bool creerCurseur() ;

    bool IncrementeCode(string* pTexte) ;
    bool InsereTexte(string pathname, string sCodePat, string *pCode, string Stexte) ;  //	retourner le code de la première fiche
                                                         //	qui va contenir contenant le texte sTexte
    bool RecupereTexte(string pathname, string sCodePat, string sCode, string* pTexte) ; 	//	mettre le contenu de la fiche de code sCode
     																	 	// dans pTexte
    bool MettreFicheAJour(string pathname, string sCode, string sNouveauTexte) ;
     																		// mettre à jour le texte de la fiche (sCode)
     																		// avec le texte sTexte

    bool DetruireFiche(NSVTexteLibreBase* pNSVTexteLibre, string sCode) ;
    bool CreerNouvelleFiche(NSVTexteLibreBase*  pNSVTexteLibre) ;
    bool ChangerFiche(NSVTexteLibreBase* pNSVTexteLibre, string sCode, string* pNewTexte) ;
};

// **************************************************************************//
//                 Classes des textes libres dans les documents
//
// **************************************************************************

//---------------------------------------------------------------------------
// Classe NSVTexteLibre
//---------------------------------------------------------------------------
class  NSVTexteLibre : public NSVTexteLibreBase
{
	public :

		NSVTexteLibre(NSContexte* pCtx) ;
		~NSVTexteLibre() ;

		DBIResult open(string pathbase) ;
};

//---------------------------------------------------------------------------// Classe NSVTLibre
//---------------------------------------------------------------------------
class NSVTLibre : public NSVTLibreBase
{
	public :

		NSVTLibre(NSContexte* pCtx) ;
    NSVTLibre(NSVTLibre& rv) ;
    ~NSVTLibre() ;

    bool creerCurseur() ;

    NSVTLibre& operator=(NSVTLibre src) ;
    int 	  operator==(const NSVTLibre& o) ;
};

#endif

