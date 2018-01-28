//---------------------------------------------------------------------------//     NSANNEXE.H
//
//  Contient les définitions des objets annexes de NAUTILUS
//
//  1ere version : PA juillet 94   Dernière modif : 18/07/94
//---------------------------------------------------------------------------
#ifndef __NSV_ANNEXE_H
#define __NSV_ANNEXE_H

#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\static.h>

#include "nsdn\nsdocum.h"
#include "nssavoir\nspatho.h"

//
// Taille des champs du fichier PATCOR
//
#define PCO_PATIENT_LEN  		PAT_NSS_LEN
#define PCO_CORRESPONDANT_LEN COR_CODE_LEN
#define PCO_FONCTION_LEN    	PATHO_CODE_LEN

//
// Indice des champs du fichier PATCOR
//
#define PCO_PATIENT_FIELD  	  1
#define PCO_CORRESPONDANT_FIELD 2
#define PCO_FONCTION_FIELD      3

//
// Taille des champs du fichier ADRESSES
//
#define ADR_CODE_LEN      	 5
#define ADR_ADRESSE1_LEN   35
#define ADR_ADRESSE2_LEN   35
#define ADR_ADRESSE3_LEN   35
#define ADR_CODE_POST_LEN   5
#define ADR_VILLE_LEN      35
#define ADR_TELEPHONE_LEN  15
#define ADR_FAX_LEN			15
#define ADR_MESSAGERIE_LEN 35

//
// Indice des champs du fichier ADRESSES
//
#define ADR_CODE_FIELD       1
#define ADR_ADRESSE1_FIELD   2
#define ADR_ADRESSE2_FIELD   3
#define ADR_ADRESSE3_FIELD   4
#define ADR_CODE_POST_FIELD  5
#define ADR_VILLE_FIELD      6
#define ADR_TELEPHONE_FIELD  7
#define ADR_FAX_FIELD		  8
#define ADR_MESSAGERIE_FIELD 9

//
// Taille des champs du fichier TYPEMIME
//
#define MIM_EXTENSION_LEN		10
#define MIM_LIBELLE_LEN			30
#define MIM_MIME_LEN			30
#define MIM_TYPE_LEN			5

//
// Indice des champs du fichier TYPEMIME
//
#define MIM_EXTENSION_FIELD		1
#define MIM_LIBELLE_FIELD		2
#define MIM_MIME_FIELD			3
#define MIM_TYPE_FIELD			4

//
// Taille des champs du fichier CRITICAL
//
#define CRITIC_BASE_LEN			30      // Nom de la base
#define CRITIC_OCCUPE_LEN		2		// représente le n° de console qui bloque la base
#define CRITIC_DATE_LEN			8       // AAAAMMJJ
#define CRITIC_HEURE_LEN		6       // HHMMSS

//
// Indice des champs du fichier CRITICAL
//
#define CRITIC_BASE_FIELD		1
#define CRITIC_OCCUPE_FIELD		2
#define CRITIC_DATE_FIELD		3
#define CRITIC_HEURE_FIELD		4

//---------------------------------------------------------------------------//  Classe NSPatCor//---------------------------------------------------------------------------

//
// Objet données
//
class NSPatCorData
{
    public :
        //
	    // Variables de stockage des valeurs
	    //
	    char patient[PCO_PATIENT_LEN + 1];
	    char correspondant[PCO_CORRESPONDANT_LEN + 1];
	    char fonction[PCO_FONCTION_LEN + 1];

	    NSPatCorData();
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSPatCor : public NSFiche
{
    public :
	    //
	    // Variables de stockage des valeurs
	    //
	    NSPatCorData* pDonnees;

	    NSPatCor(NSSuper* pSuper);
	    ~NSPatCor();

	    void alimenteFiche();
	    void videFiche();
	    DBIResult open(string pathbase);

        bool Creer(string pathname, char* codePatient, char* codeCorresp, char* codeFonction);
        bool Modifier(string pathname, char* codePatient, char* codeCorresp, char* codeFonction);
        bool Supprimer(string pathname, char* codePatient, char* codeCorresp);
};

//---------------------------------------------------------------------------
//  Classe NSAdresses
//---------------------------------------------------------------------------


//
// Objet données
//
class NSAdressesData
{
    public :
	    //
	    // Variables de stockage des valeurs
	    //
	    char code[ADR_CODE_LEN + 1];
	    char adresse1[ADR_ADRESSE1_LEN + 1];
	    char adresse2[ADR_ADRESSE2_LEN + 1];
	    char adresse3[ADR_ADRESSE3_LEN + 1];
        char code_post[ADR_CODE_POST_LEN + 1];
	    char ville[ADR_VILLE_LEN + 1];
	    char telephone[ADR_TELEPHONE_LEN + 1];
        char fax[ADR_FAX_LEN + 1];
	    char messagerie[ADR_MESSAGERIE_LEN + 1];

	    NSAdressesData();
        NSAdressesData(NSAdressesData& rv);

	    NSAdressesData& operator=(NSAdressesData src);
	    int 			operator==(NSAdressesData& o);

	    void metABlanc();
        void metAZero();
};

class NSAdresses;

class NSAdressesInfo
{
    public :
        //
	  	// Objet qui contient les données
	  	//
	  	NSAdressesData* pDonnees;

     	NSAdressesInfo();
		NSAdressesInfo(NSAdresses*);
		NSAdressesInfo(NSAdressesInfo& rv);
        ~NSAdressesInfo();

        string StringAdresse(bool bEdit = true);

        NSAdressesInfo& operator=(NSAdressesInfo src);
		int operator == ( NSAdressesInfo& o );
};

// type énuméré pour la création / modification des adresses
enum { tAdrPatient=0, tAdrCorresp };

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSAdresses : public NSFiche
{
    public :

	    //
	    // Variables de stockage des valeurs
	    //
	    NSAdressesData* pDonnees;

	    NSAdresses(NSSuper* pSuper);
	    ~NSAdresses();

	    DBIResult open(string pathbase);
	    void metABlanc() { pDonnees->metABlanc(); }
	    void alimenteFiche();
	    void videFiche();

        bool chercheAdressesInfo(string pathname, char* cleAdresse, NSAdressesInfo* pAdresseInfo);
};

//---------------------------------------------------------------------------
//  Classe NSCritical
//---------------------------------------------------------------------------

#ifndef N_TIERS
//
// Objet données
//
class NSCriticalData
{
    public :
	  	//
	  	// Variables de stockage des valeurs
	  	//
	  	char base[CRITIC_BASE_LEN + 1];
     	char occupe[CRITIC_OCCUPE_LEN + 1];
        char date[CRITIC_DATE_LEN + 1];
        char heure[CRITIC_HEURE_LEN + 1];

	  	NSCriticalData();
     	NSCriticalData(NSCriticalData& rv);

	  	NSCriticalData& operator=(NSCriticalData src);
	  	int 			   operator==(NSCriticalData& o);

	  	void metABlanc();
     	void metAZero();
};

class NSCritical;

class NSCriticalInfo
{
    public :
	  	//
	  	// Objet qui contient les données
	  	//
        NSCriticalData* pDonnees;

     	NSCriticalInfo();
		NSCriticalInfo(NSCritical*);
		NSCriticalInfo(NSCriticalInfo& rv);
      	~NSCriticalInfo();

      	NSCriticalInfo& operator=(NSCriticalInfo src);
		int operator == ( NSCriticalInfo& o );
};

//
// Objet dérivé de NSFiche utilisé pour les opérations de base de données
//
class NSCritical : public NSFiche
{
    public :

        //
	    // Variables de stockage des valeurs
	    //
	    NSCriticalData* pDonnees;

	    NSCritical(NSContexte* pCtx);
	    ~NSCritical();

	    DBIResult open(string pathbase);
	    void metAZero() { pDonnees->metAZero(); }
	    void alimenteFiche();
	    void videFiche();
};

// Dialogue de blocage en section critique

enum { RP_ANNULER = 0, RP_REESSAYER, RP_FORCER };

#ifdef _INCLUS
class NSCriticalDialog : public NSDialogWrapper
#else
class NSCriticalDialog : public TDialog
#endif
{
    public:

        int 		reponse;
        TStatic*	pTextBase;
        TStatic*	pTextConsole;
        TStatic*	pTextDate;
        NSCriticalInfo* pInfo;

#ifdef _INCLUS
        NSCriticalDialog(TWindow* parent, NSCriticalInfo* pCritInfo, NSContexte* pCtx);
#else
        NSCriticalDialog(TWindow* parent, NSCriticalInfo* pCritInfo);
#endif
        ~NSCriticalDialog();

        void SetupWindow();
        void CmReessayer();
        void CmForcerBlocage();
        void CmAnnuler();

    DECLARE_RESPONSE_TABLE(NSCriticalDialog);
};
#endif // ifndef N_TIERS
//---------------------------------------------------------------------------
#endif    // __NSANNEXE_H
