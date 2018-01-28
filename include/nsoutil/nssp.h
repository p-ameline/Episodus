#if !defined(NSSP_H)
#define NSSP_H


#include <classlib\arrays.h>
#include "partage\ns_vector.h"

#include <owl\dialog.h>
#include <owl\listbox.h>

#include "nsoutil\nsexport.h"

//
// Taille des champs du fichier SP
//
#define SP_CODE_LEN				 	 6
#define SP_CODE_INTERNE_LEN 		 6
#define SP_CODE_GENERIQUE_LEN 	   	 6
#define SP_CODE_FORME_PHRMA_LEN 	10
#define SP_CODE_STATUT_LABO_LEN 	10
#define SP_LISTE_SPECIALITE_LEN 	10
#define SP_CODE_CLASSE_ATC_LEN 		10
#define SP_CODE_CLASSE_EPHMRA_LEN 	10
#define SP_CODE_CLASSE_GESTION_LEN 	 6
#define SP_CODE_INTER_PROD_LEN 		 6
#define SP_NOM_SPECIALITE_LEN 	  	36
#define SP_COMPLEMENT_NOM_LEN 	   	40
#define SP_PH_SPECIALITE_LEN		10
#define SP_SOLVANT_SPECIALITE_LEN 	 1
#define SP_CODE_CIP_UCD_LEN 	   	 7
#define SP_DATE_CREATION_LEN		 8
#define SP_DATE_MODIFICATION_LEN 	 8
#define SP_NUMERO_DOSSIER_LEN		10
#define SP_CODE_APHP_LEN		     8
#define SP_INTERNE_REF_LEN		     6
#define SP_DATE_JO_GENERIQUE_LEN	 8
#define SP_ORIGINE_GENERIQUE_LEN	 1

//
// Ordre des champs du fichier SPERIEL
//
#define SP_CODE_FIELD			          1
#define SP_CODE_INTERNE_FIELD             2
#define SP_CODE_GENERIQUE_FIELD 	   	  3
#define SP_CODE_FORME_PHRMA_FIELD 		  4
#define SP_CODE_STATUT_LABO_FIELD 	  	  5
#define SP_LISTE_SPECIALITE_FIELD 	   	  6
#define SP_CODE_CLASSE_ATC_FIELD 		  7
#define SP_CODE_CLASSE_EPHMRA_FIELD 	  8
#define SP_CODE_CLASSE_GESTION_FIELD 	  9
#define SP_CODE_INTER_PROD_FIELD 		 10
#define SP_NOM_SPECIALITE_FIELD 	  	 11
#define SP_COMPLEMENT_NOM_FIELD 	   	 12
#define SP_PH_SPECIALITE_FIELD		 	 13
#define SP_SOLVANT_SPECIALITE_FIELD 	 14
#define SP_CODE_CIP_UCD_FIELD 	   		 15
#define SP_DATE_CREATION_FIELD		 	 16
#define SP_DATE_MODIFICATION_FIELD 	   	 17
#define SP_NUMERO_DOSSIER_FIELD		 	 18
#define SP_CODE_APHP_FIELD		         19
#define SP_INTERNE_REF_FIELD		     20
#define SP_DATE_JO_GENERIQUE_FIELD	     21
#define SP_ORIGINE_GENERIQUE_FIELD	     22

//---------------------------------------------------------------------------
// Classe NSSPData
//---------------------------------------------------------------------------

class NSSPData
{
    public :
        //
	    // Variables de stockage des valeurs
	    //
        char code[SP_CODE_LEN + 1];
	    char codeInterne[SP_CODE_INTERNE_LEN + 1];
	    char codeGenerique[SP_CODE_GENERIQUE_LEN + 1];
        char codeForme[SP_CODE_FORME_PHRMA_LEN + 1];
        char codeStatutLabo[SP_CODE_STATUT_LABO_LEN + 1];
	    char listeSpecialite[SP_LISTE_SPECIALITE_LEN + 1];
        char codeATC[SP_CODE_CLASSE_ATC_LEN + 1];
	    char codeEPHMRA[SP_CODE_CLASSE_EPHMRA_LEN + 1];
        char codeGestion[SP_CODE_CLASSE_GESTION_LEN + 1];
        char codeInterneProduit[SP_CODE_INTER_PROD_LEN + 1];
	    char nomSpecialite[SP_NOM_SPECIALITE_LEN + 1];
        char complementNom[SP_COMPLEMENT_NOM_LEN + 1];
        char PhSpecialite[SP_PH_SPECIALITE_LEN + 1];
	    char solvantSpecialite[SP_SOLVANT_SPECIALITE_LEN + 1];
        char codeCIP_UCD[SP_CODE_CIP_UCD_LEN + 1];
        char dateCreation[SP_DATE_CREATION_LEN + 1];
	    char dateModif[SP_DATE_MODIFICATION_LEN + 1];
        char numDossier[SP_NUMERO_DOSSIER_LEN + 1];
        char codeAPHP[SP_CODE_APHP_LEN + 1];
        char codeInterneRef[SP_INTERNE_REF_LEN + 1];
        char dateJOGenerique[SP_DATE_JO_GENERIQUE_LEN + 1];
        char origineGenerique[SP_ORIGINE_GENERIQUE_LEN + 1];

        NSSPData(NSSPData& rv);
	    NSSPData() { metAZero(); }
        ~NSSPData();

        NSSPData& operator=(NSSPData src);
        int 	  operator==(const NSSPData& o);

        //méthodes
	    void metABlanc();
	    void metAZero();
};


//---------------------------------------------------------------------------
// Classe NSSP
//---------------------------------------------------------------------------
class _NSOUTILCLASSE NSSP : public NSFiche
{
    public :

	    NSSPData* pDonnees;   // Objet qui contient les données

	    NSSP(NSContexte* pCtx);
	    ~NSSP();
        DBIResult open();
	    void metABlanc() { pDonnees->metABlanc(); }
	    void alimenteFiche();
	    void videFiche();
};

//---------------------------------------------------------------------------
// Classe NSSPInfo
//---------------------------------------------------------------------------
class NSSPInfo
{
    public :
		//
		// Objet qui contient les données
		//
        NSSPData* pDonnees;

		NSSPInfo();
		NSSPInfo(NSSP* pNSSP);
		NSSPInfo(NSSPInfo& rv);
        ~NSSPInfo();
		NSSPInfo& operator=(NSSPInfo src);
		int operator == ( const NSSPInfo& o );
};

//---------------------------------------------------------------------------
// Classe NSSpecialite : origine : table SP_SPECIALITE
//---------------------------------------------------------------------------

class _NSOUTILCLASSE NSSpecialite : public NSRoot
{
    public :
	    //
	    // Variables de stockage des valeurs
	    //
        NSSP* 	pNSSP;
        NSSpecialite(NSContexte* pCtx);
        NSSpecialite(NSSpecialite& rv);
        ~NSSpecialite();

        NSSpecialite& operator=(NSSpecialite src);
        int 		  operator==(const NSSpecialite& o);

        //méthodes
        bool IncrementeCode(string* pTexte);
        void Import();
};

// --------------- Fichier CPH (Classes pharmaco-thérapeutiques) ----------
//
// Taille des champs du fichier CPH
//
#define CPH_CODE_LEN				 6
#define CPH_CODE_INTERNE_LEN 		10
#define CPH_CODE_PERE_LEN 	   	    10
#define CPH_NOM_LEN 	   	        40
#define CPH_DATE_CREATION_LEN		 8
#define CPH_DATE_MODIFICATION_LEN 	 8
#define CPH_MODIFIE_LEN 	         1

//
// Ordre des champs du fichier CPH
//
#define CPH_CODE_FIELD			     2
#define CPH_CODE_INTERNE_FIELD       1
#define CPH_CODE_PERE_FIELD 	   	 3
#define CPH_NOM_FIELD 	  	         4
#define CPH_DATE_CREATION_FIELD		 5
#define CPH_DATE_MODIFICATION_FIELD  6
#define CPH_MODIFIE_FIELD            7

//---------------------------------------------------------------------------
// Classe NSCPHData
//---------------------------------------------------------------------------

class NSCPHData
{
    public :
        //
	    // Variables de stockage des valeurs
	    //
        char code[CPH_CODE_LEN + 1];
	    char codeInterne[CPH_CODE_INTERNE_LEN + 1];
	    char codePere[CPH_CODE_PERE_LEN + 1];
	    char nom[CPH_NOM_LEN + 1];
        char dateCreation[CPH_DATE_CREATION_LEN + 1];
	    char dateModif[CPH_DATE_MODIFICATION_LEN + 1];
        char modifie[CPH_MODIFIE_LEN + 1];

        NSCPHData(NSCPHData& rv);
	    NSCPHData() { metAZero(); }
        ~NSCPHData();

        NSCPHData& operator=(NSCPHData src);
        int 	  operator==(const NSCPHData& o);

        //méthodes
	    void metABlanc();
	    void metAZero();
};


//---------------------------------------------------------------------------
// Classe NSCPH
//---------------------------------------------------------------------------
class _NSOUTILCLASSE NSCPH : public NSFiche
{
    public :

	    NSCPHData* pDonnees;   // Objet qui contient les données

	    NSCPH(NSContexte* pCtx);
	    ~NSCPH();
        DBIResult open();
	    void metABlanc() { pDonnees->metABlanc(); }
	    void alimenteFiche();
	    void videFiche();
};

//---------------------------------------------------------------------------
// Classe NSCPHInfo
//---------------------------------------------------------------------------
class NSCPHInfo
{
    public :
		//
		// Objet qui contient les données
		//
        NSCPHData* pDonnees;

		NSCPHInfo();
		NSCPHInfo(NSCPH* pNSCPH);
		NSCPHInfo(NSCPHInfo& rv);
        ~NSCPHInfo();
		NSCPHInfo& operator=(NSCPHInfo src);
		int operator == ( const NSCPHInfo& o );
};

//---------------------------------------------------------------------------
// Classe NSClassePharma : origine : table CPH
//---------------------------------------------------------------------------

class _NSOUTILCLASSE NSClassePharma : public NSRoot
{
    public :
	    //
	    // Variables de stockage des valeurs
	    //
        NSCPH* pConvert;
        NSClassePharma(NSContexte* pCtx);
        ~NSClassePharma();

        //méthodes
        void Import();
};

// --------------- Fichier CPH (Classes pharmaco-thérapeutiques) ----------
//
// Taille des champs du fichier CPH
//
#define PRO_CODE_LEN				 6
#define PRO_CODE_INTERNE_LEN 		 6
#define PRO_CODE_PERE_LEN 	   	     6
#define PRO_NOM_LEN 	   	        40
#define PRO_DATE_CREATION_LEN		 8
#define PRO_DATE_MODIFICATION_LEN 	 8
#define PRO_MODIFIE_LEN 	         1

//
// Ordre des champs du fichier CPH
//
#define PRO_CODE_FIELD			     2
#define PRO_CODE_INTERNE_FIELD       1
#define PRO_CODE_PERE_FIELD 	   	 3
#define PRO_NOM_FIELD 	  	         4
#define PRO_DATE_CREATION_FIELD		 5
#define PRO_DATE_MODIFICATION_FIELD  6
#define PRO_MODIFIE_FIELD            7

//---------------------------------------------------------------------------
// Classe NSCPHData
//---------------------------------------------------------------------------

class NSPRData
{
    public :
        //
	    // Variables de stockage des valeurs
	    //
        char code[PRO_CODE_LEN + 1];
	    char codeInterne[PRO_CODE_INTERNE_LEN + 1];
	    char codePere[PRO_CODE_PERE_LEN + 1];
	    char nom[PRO_NOM_LEN + 1];
        char dateCreation[PRO_DATE_CREATION_LEN + 1];
	    char dateModif[PRO_DATE_MODIFICATION_LEN + 1];
        char modifie[PRO_MODIFIE_LEN + 1];

        NSPRData(NSPRData& rv);
	    NSPRData() { metAZero(); }
        ~NSPRData();

        NSPRData& operator=(NSPRData src);
        int 	  operator==(const NSPRData& o);

        //méthodes
	    void metABlanc();
	    void metAZero();
};


//---------------------------------------------------------------------------
// Classe NSPR
//---------------------------------------------------------------------------
class _NSOUTILCLASSE NSPR : public NSFiche
{
    public :

	    NSPRData* pDonnees;   // Objet qui contient les données

	    NSPR(NSContexte* pCtx);
	    ~NSPR();
        DBIResult open();
	    void metABlanc() { pDonnees->metABlanc(); }
	    void alimenteFiche();
	    void videFiche();
};

//---------------------------------------------------------------------------
// Classe NSPRInfo
//---------------------------------------------------------------------------
class NSPRInfo
{
    public :
		//
		// Objet qui contient les données
		//
        NSPRData* pDonnees;

		NSPRInfo();
		NSPRInfo(NSPR* pNSPR);
		NSPRInfo(NSPRInfo& rv);
        ~NSPRInfo();
		NSPRInfo& operator=(NSPRInfo src);
		int operator == ( const NSPRInfo& o );
};

//---------------------------------------------------------------------------
// Classe NSClasseProduit : origine : table PR
//---------------------------------------------------------------------------

class _NSOUTILCLASSE NSClasseProduit : public NSRoot
{
    public :
	    //
	    // Variables de stockage des valeurs
	    //
        NSPR* pConvert;
        NSClasseProduit(NSContexte* pCtx);
        ~NSClasseProduit();

        //méthodes
        void Import();
};


#endif