//---------------------------------------------------------------------------//     NSDOCUM.H
//
//  Contient les définitions des objets document
//
//  1ere version : PA juillet 94   Dernière modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSV_DOCUM_H
#define __NSV_DOCUM_H

#include <classlib\arrays.h>
#include <cstring.h>

#include "partage\nsperson.h"   // pour le code patient
#include "partage\nsmatfic.h"
#include "nsdn\nsdocum.h"
#include "nsmt_version\nsv_intrad.h"

//
// Taille des champs du fichier DOCUMENT
//
#define NSV_DOC_CODE_DOCUM_LEN     5
#define NSV_DOC_CREATEUR_LEN	   3
#define NSV_DOC_CREATION_LEN	   8
#define NSV_DOC_NOM_LEN			  35

#define DOC_CODE_DOCUM_LEN     6
#define DOC_CREATEUR_LEN       PAT_NSS_LEN
#define DOC_CREATION_LEN	     14
#define DOC_NOM_LEN			       1024
#define DOC_ACCES_LEN		       2
#define DOC_TYPE_LEN		       5
#define DOC_LOCALISATION_LEN   4
#define DOC_FICHIER_LEN		     100  // pour stocker les noms longs avec Pathname
#define DOC_ENTETE_LEN		     50
#define DOC_EMPLACEMENT_LEN    14
#define DOC_INTERET_LEN		     1
#define DOC_COMMENTAIRE_LEN	   4
#define DOC_VISIBLE_LEN		     1
#define DOC_TRAN_LEN		       5
#define DOC_CRC_LEN			       4

//// Ordre des champs du fichier DOCUMENT
//
#define DOC_PAT_FIELD  		  	  1
#define DOC_CODE_FIELD  		  2
#define DOC_NOM_FIELD			  3
#define DOC_CREATION_FIELD		  4
#define DOC_ACCES_FIELD			  5
#define DOC_CREATEUR_FIELD		  6
#define DOC_TYPE_FIELD			  7
#define DOC_LOCALISATION_FIELD    8
#define DOC_FICHIER_FIELD		  9
#define DOC_ENTETE_FIELD		 10
#define DOC_EMPLACEMENT_FIELD    11
#define DOC_INTERET_FIELD		 12
#define DOC_COMMENTAIRE_FIELD	 13
#define DOC_VISIBLE_FIELD		 14
#define DOC_TRAN_NEW_FIELD		 15
#define DOC_TRAN_DEL_FIELD		 16
#define DOC_CRC_FIELD	 		 17

//
// Taille des champs du fichier CHEMISES
//
#define NSV_CHE_CODE_LEN  		   9

//
// Taille des champs du fichier CHEM_DOC
//
#define NSV_CDO_CHEMISE_LEN    9
#define NSV_CDO_DOCUMENT_LEN  11class NSPatPathoArray;
//---------------------------------------------------------------------------
//  Classe NSDocument
//---------------------------------------------------------------------------

//
// Objet données
//
class NSVDocumentData
{
	public :
		//
		// Variables de stockage des valeurs
		//
		char codePatient[PAT_NSS_LEN + 1];
		char codeDocument[DOC_CODE_DOCUM_LEN + 1];
		char nom[DOC_NOM_LEN + 1];
		char creation[DOC_CREATION_LEN + 1];
		char acces[DOC_ACCES_LEN + 1];
		char createur[DOC_CREATEUR_LEN + 1];
		char type[DOC_TYPE_LEN + 1];
		char localisation[DOC_LOCALISATION_LEN + 1];
		char fichier[DOC_FICHIER_LEN + 1];
		char entete[DOC_ENTETE_LEN + 1];
		char emplacement[DOC_EMPLACEMENT_LEN + 1];		char interet[DOC_INTERET_LEN + 1];
		char commentaire[DOC_COMMENTAIRE_LEN + 1];
		char visible[DOC_VISIBLE_LEN + 1];
		char tranNew[DOC_TRAN_LEN + 1];
		char tranDel[DOC_TRAN_LEN + 1];
		char crc[DOC_CRC_LEN + 1];

/*
		string sFichier ;
		string sTemplate ;
		string sEnTete ;
		string sTypeContenu ;
		string sTypeContenuSens ;
		string sDateExamen ;

		string 	sRights ;*/		void   metAZero() ;		string donneDateCreation(string sLang = string("")) ;

		bool   estVisible() { return ('1' == visible[0]) ; }
		void   rendVisible()   { visible[0] = '1' ; }		void   rendInvisible() { visible[0] = '0' ; }

		NSVDocumentData() { metAZero() ; }
		NSVDocumentData(NSVDocumentData& rv) ;

		string getID() { return string(codePatient) + string(codeDocument) ; }

		NSVDocumentData& operator=(NSVDocumentData src) ;
		int              operator==(const NSVDocumentData& o) ;
};

//// Objet "Info" correspondant à NSDocument
// (destiné à être utilisé dans les containers)
//

class NSVDocumentInfo : public NSRoot
{
	public:
		NSVDocumentData*  pDonnees ;

    NSPatPathoArray* pMeta ;
    NSPatPathoArray* pPres ;
    string 				   sCodeDocMeta ;    string           sCodeDocPres ;		NSVDocumentInfo(NSContexte* pCtx = 0) ; // on a besoin d'un constructeur par défaut
    NSVDocumentInfo(NSVDocumentInfo& rv) ;
    ~NSVDocumentInfo() ;

/*
		bool getNomFichier(string& sNomFichier) ;
    bool DonnePatPatho(NSPatPathoArray* pPatPathoArray, NSPatInfo* pPatInfo = 0) ;
    bool ChargeDocMeta(NSPatInfo* pPatInfo = 0) ;    bool ParseMetaDonnees() ;
    bool CommitMetaDonnees() ;
    void   rendVisible()   { pDonnees->rendVisible() ; }		void   rendInvisible() { pDonnees->rendInvisible() ; }*/		NSVDocumentInfo& operator=(NSVDocumentInfo src) ;
		int operator == ( const NSVDocumentInfo& o ) ;
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSDocument : public NSVDocumentInfo, public NSFiche
{
	public :

		NSDocument(NSSuper* pSuper) ;     			  // Constructeur ordinaire
    NSDocument(NSSuper* pSuper, hDBICur hCurSrc) ; // Constructeur curseur cloné
    ~NSDocument() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open(string pathbase) ;

    void initialiseDocInfo(NSDocumentInfo* pDest) ;

    bool creerChemise(bool appliquerModele) ;

    string getID() { return pDonnees->getID() ; }
};

//---------------------------------------------------------------------------
//  Classe NSChemise
//---------------------------------------------------------------------------

class NSVChemiseData
{
	public :

    char code[CHE_CODE_LEN + 1] ;
    char nom[CHE_NOM_LEN + 1] ;
    char creation[CHE_CREATION_LEN + 1] ;
    char acces[CHE_ACCES_LEN + 1] ;
    char createur[CHE_CREATEUR_LEN + 1] ;
    char commentaire[CHE_COMMENTAIRE_LEN + 1] ;

    NSVChemiseData() { metAZero() ; }
    NSVChemiseData(const NSVChemiseData& rv) ;

    NSVChemiseData& operator=(const NSVChemiseData& src) ;
    int             operator==(const NSVChemiseData& o) const ;

    void metAZero() ;

    void donneIntitule(string &sIntitule, string sLang) ;
};

class NSVChemiseInfo
{
	public:

		string         _sNodeChemise ;
		NSVChemiseData _Donnees ;

		NSVChemiseInfo() ;

		NSVChemiseInfo(const NSVChemiseInfo& rv) ;
    NSVChemiseInfo(const NSChemiseInfo& rv) ;
		~NSVChemiseInfo() ;

    string getNom()      { return string(_Donnees.nom) ; }
    string getCreation() { return string(_Donnees.creation) ; }

		void donneIntitule(string &sIntitule, string sLang) ;

		NSVChemiseInfo& operator=(const NSVChemiseInfo& src) ;
		int             operator==(const NSVChemiseInfo& o) const ;
};

typedef vector<NSVChemiseInfo*> NSVChemInfoArray;
typedef NSVChemInfoArray::iterator ChemVInfoIter;
typedef NTArray<NSVChemiseInfo>  NSVChemiseArray ;

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSChemise : public NSFiche
{
  public :

    //
    // Variables de stockage des valeurs
    //
    NSVChemiseData* pDonnees ;

    NSChemise(NSSuper* pSuper) ;
    NSChemise(NSSuper* pSuper, hDBICur hCurSrc) ; // Constructeur curseur cloné
    ~NSChemise() ;

    void  donneIntitule(char *intitule);

    void  alimenteFiche();
    void  videFiche();
    DBIResult open(string pathbase);
} ;

//---------------------------------------------------------------------------
//  Classe NSChemDoc
//---------------------------------------------------------------------------

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSChemDoc : public NSFiche
{
  public :

    NSChemDocData* pDonnees ;

    NSChemDoc(NSSuper* pSuper) ;
    NSChemDoc(NSSuper* pSuper, hDBICur hCurSrc) ;
    ~NSChemDoc() ;

    void alimenteFiche() ;
    void videFiche() ;
    DBIResult open(string pathbase) ;

    DBIResult CreeLien() ;
    bool DonneNouvelOrdre() ;
    bool DecrementeOrdre() ;
};

//
// Objet données des Objets Materiel
//
class NSVMaterielData
{
	public :

  	string _sObjectID ;
    //
    // Variables de stockage des valeurs
    //
    char type[MAT_TYPE_LEN + 1] ;
    char utilisateur[MAT_UTILISATEUR_LEN + 1] ;
    char code[MAT_CODE_LEN + 1] ;
    char libelle[MAT_LIBELLE_LEN + 1] ;
    char num_serie[MAT_NUM_SERIE_LEN + 1] ;
    char mise_svce[MAT_MISE_SVCE_LEN + 1] ;
    char actif[MAT_ACTIF_LEN + 1] ;

    string libelle_complet ;

    void metAZero() ;

    NSVMaterielData() { metAZero() ; }
    NSVMaterielData(const NSVMaterielData& rv) ;

    NSVMaterielData& operator=(const NSVMaterielData& src) ;
    int              operator==(const NSVMaterielData& o) const ;
};

class NSVMaterielInfo
{
	public:

		NSVMaterielData* pDonnees ;
    NSPatPathoArray* pPatPathoArray ;

    bool initialiseDepuisObjet(NSSuper* pSuper, string sObjId, string actif = string("")) ;
    bool initialiseDepuisSN(NSSuper* pSuper, string sObjSN, string actif = string("")) ;

    void initFromPatpatho(NSSuper* pSuper, string sActif = string("")) ;

		NSVMaterielInfo() ;
    ~NSVMaterielInfo() ;
		NSVMaterielInfo(const NSVMaterielInfo& rv) ;

    string donneComplement() ;
    bool initialiseDepuisComplement(string sComplement) ;

		NSVMaterielInfo& operator=(const NSVMaterielInfo& src) ;
		int              operator==(const NSVMaterielInfo& o) const ;

    void   reset()    { pDonnees->metAZero() ; }

    string getType()     { return string(pDonnees->type) ; }
    string getUser()     { return string(pDonnees->utilisateur) ; }
    string getCode()     { return string(pDonnees->code) ; }
    string getLabel()    { return string(pDonnees->libelle) ; }
    string getSN()       { return string(pDonnees->num_serie) ; }
    string getDate()     { return string(pDonnees->mise_svce) ; }
    string getActif()    { return string(pDonnees->actif) ; }

    string getObjectID()           { return pDonnees->_sObjectID ; }
    void   setObjectID(string sID) { pDonnees->_sObjectID = sID ; }

    string getFullLabel() { return pDonnees->libelle_complet ; }
    string getLabelAndSN() ;
    void   removeTrailingDotSpace(string &sLabel) ;

    string getLexiqueLabel(NSSuper* pSuper) ;
};

class NSMateriel : public NSFiche
{
  public :

    NSVMaterielData* pDonnees ;	  // Données de la fiche PARADOX

    NSMateriel(NSSuper* pSuper) ;
    ~NSMateriel() ;

    virtual void fabriqueLibelle() ;

    string donneComplement() { return string(pDonnees->type) +
                                          string(pDonnees->utilisateur)  +
                                          string(pDonnees->code) ; }
    bool initialiseDepuisComplement(string sComplement) ;

    DBIResult open(string pathbase) ;
    void alimenteFiche() ;
    void videFiche() ;
};

//---------------------------------------------------------------------------
#endif    // __NSV_DOCUM_H


