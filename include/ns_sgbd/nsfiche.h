// -----------------------------------------------------------------------------
// nsfiche.h
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2014/01/29 20:46:16 $
// -----------------------------------------------------------------------------
// FLP - juillet 2003
// ...
// PK  - janvier 1992
// -----------------------------------------------------------------------------
// Contient les definitions des objets NAUTILUS
// -----------------------------------------------------------------------------

#ifndef __NSFICHE_H
# define __NSFICHE_H

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include <string>
# include <cstring>
#ifndef _NO_BDE
# include <bde.hpp>
#endif

class NSContexte ;

# if defined(_DANSSGBDDLL)
#  define _CLASSESGBD __export
# else
#  define _CLASSESGBD __import
# endif

// Erreurs spécifiques retournées par les classes NSFiche
# define ERROR_TABLENOTOPEN -1

// Catégories des fichiers NAUTILUS
# define NSF_PARTAGE_GLOBAL 0     // Tables partagées pures : ex Patients, Comptes rendus
# define NSF_PARTAGE_PARAMS 1     // Tables partagées de paramètres : peuvent être locales en liaison remote
# define NSF_PERSONNEL      2     // Tables personnelles
# define NSF_COMPTA		      3     // Tables de comptabilité
# define NSF_GUIDES		      4	    // Tables GUIDE, LEXIQUE et SAVOIR
# define NSF_UPGRADE          5     // Tables utilisées par la dll d'Upgrade


// -----------------------------------------------------------------------------
// Classe NSFiche
// ANCETRE : Object
// Sert comme ancêtre des objets PARADOX
// -----------------------------------------------------------------------------

enum { tCodePatient1 = 1, tCodePatient2, tCodeUtil, tCodeCorresp, tCodeAdresse, tCodeObjet } ;
#define NBCPT   6

#ifndef __linux__
class _CLASSESGBD NSFiche : public NSSuperRoot
#else
class NSFiche : public NSSuperRoot
#endif
{
  private:

    static long _lNSFicheCount ;

 	protected:

    bool       _isOpen ;       // Indique si la table est ouverte
  	bool       isClone ;      // Indique si l'objet fonctionne avec un curseur clone
  	hDBICur    hCursor ;      // Handle du curseur de la table
  	void      *pLdObj ;       // Language driver object.
  	Byte      *pRecBuff ;     // Pointeur sur le record buffer
  	Byte      *pBookMark[2] ; // Pointeur sur le bookmark

  	DBITBLNAME indexNom ;     // Nom et ID de l'index en cours d'utilisation
  	Word       indexID ;      //

  	string     sNomFichier ;  // Nom de la table (avec chemin et extension)

  	// Statut de la fiche : 0 = OK  1 = Réceptacle vide
  	short      statut ;

	public:

  	DBIResult lastError ;       // Dernière erreur retournée par fonctions membres
  	string    sNomTable ;       // Nom fonctionnel de la table
  	string    sRepareWarning ;  // Message à afficher en cas de réparation

  	// Constructeur et Destructeur
  	NSFiche(NSSuper* pSuper) ;
  	NSFiche(NSSuper* pSuper, hDBICur hCurSrc) ;   // Constructeur pour clonage
  	virtual ~NSFiche() ;

  	// Fonctions de récupération et d'incrément des codes de nauti.lus
  	bool              CodeValide(string code) ;
 		bool              IncrementeCode(string *pTexte) ;

  	// Fonctions d'ouverture et de fermeture de la table
  	DBIResult         open(string sTableName, int tableClasse, bool bOpenShared = true) ;
  	virtual DBIResult close() ;

  	// Fonctions de déplacement de base
  	DBIResult         ChangeIndex(char *IndexNom, Word IndexID, bool MemeRecord) ;

  	// Recherche en précisant uniquement le nom de l'index
  	DBIResult         chercheClef(const string  *clefATrouver,
                                  DBITBLNAME    IndexNom,
                                  DBISearchCond searchMode,
                                  DBILockType   Blocage) ;

  	// Recherche en précisant le nom et l'indice de l'index
  	DBIResult         chercheClef(const string   *clefATrouver,
                                  DBITBLNAME     IndexNom,
                                  Word           IndexID,
                                  DBISearchCond  searchMode,
                                  DBILockType    Blocage) ;

    DBIResult         chercheClef(const unsigned char *clefATrouver,
                                  DBITBLNAME          IndexNom,
                                  Word                IndexID,
                                  DBISearchCond       searchMode,
                                  DBILockType         Blocage) ;

  	DBIResult         chercheClef(short         clefATrouver,
                                int           iIndex,
                                DBITBLNAME    indexNom,
                                Word          IndexID,
                                DBISearchCond searchMode,
                                DBILockType   Blocage) ;

    DBIResult         chercheClef(int         clefATrouver,
                                int           iIndex,
                                DBITBLNAME    indexNom,
                                Word          IndexID,
                                DBISearchCond searchMode,
                                DBILockType   Blocage) ;

  	DBIResult         chercheClefComposite( DBITBLNAME    IndexNom,
                                Word          IndexID,
                                DBISearchCond searchMode,
                                DBILockType   Blocage,
                                Byte          *pIndexRec) ;

  	DBIResult         precedent(DBILockType Blocage) ;
  	DBIResult         suivant(DBILockType Blocage) ;
  	DBIResult         debut(DBILockType Blocage) ;
  	DBIResult         fin(DBILockType Blocage) ;

  	DBIResult         emptyTable() ;

  	// Fonctions de numérotation
  	DBIResult         donneNbTotalRec(int *Nombre) ;
  	DBIResult         donneNumRecEnCours(int *Numero) ;

	// Fonctions de transfert de l'enregistrement courant dans pRecBuff
  	// et vice versa
  	virtual DBIResult getRecord(void) ;
  	DBIResult         getDbiRecord(DBILockType Blocage) ;
  	DBIResult         insertRecord(DBILockType Blocage) ;
  	DBIResult         appendRecord(void) ;
  	DBIResult         modifyRecord(BOOL relacheBlocage) ;
  	DBIResult         deleteRecord(void) ;

  	virtual void 		  alimenteFiche() = 0 ;
  	virtual void 	    videFiche() = 0 ;
  	DBIResult         initialiseRecbuff() ;

	// Fonctions de gestion des BookMarks
  	int 	            creerBookMarks(int nbreBookMarks) ;
  	int 	            detruireBookMarks(int nbreBookMarks) ;
  	DBIResult         placeBookMark(int numBookMark) ;
  	DBIResult         retrouveBookMark(int numBookMark) ;

  	// Fonctions de début/fin de liaison de tables
  	DBIResult         DebutModeLiaison(void) ;
  	DBIResult         FinModeLiaison(void) ;

  	// Fonctions diverses
  	hDBICur           PrendCurseur() { return hCursor ; }
  	Byte             *PrendRecBuff() { return pRecBuff ; }

  	short 	          prendStatut() { return statut ; }
  	void	            metStatut(short nouvStatut) { statut = nouvStatut ; }

  	DBIResult         NativeToAnsi(char *chaineSource, char *chaineCible) ;
  	DBIResult         NativeToAnsi(char *chaine) ;

  	DBIResult         AnsiToNative(char *chaineSource, char *chaineCible) ;
  	DBIResult         AnsiToNative(char *chaine) ;

  	bool	            alimenteChamp(char        *pChamp, int index, int taille) ;
  	bool              alimenteChamp(short       *pChamp, int index) ;
  	bool              alimenteChamp(long int    *pChamp, int index) ;
  	bool              alimenteChamp(bool        *pChamp, int index) ;
  	bool              alimenteBlobChamp(string  *pChamp, int index) ;

  	bool	            videChamp(char            *pChamp,  int index, int taille) ;
  	bool              videChamp(short           pChamp,   int index) ;
  	bool              videChamp(long int        pChamp,   int index) ;
  	bool              videChamp(bool            pChamp,   int index) ;
  	bool              videBlobChamp(string      pChamp,   int index) ;

  	string	          getNomFichier() { return sNomFichier ; }

    bool              isOpened() { return _isOpen ; }

    static long       getNbInstance() { return _lNSFicheCount ; }
    static void       initNbInstance() { _lNSFicheCount = 0 ; }
} ;
#endif    // __NSFICHE_H

