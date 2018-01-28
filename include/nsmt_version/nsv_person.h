//---------------------------------------------------------------------------
//     NSPERSON.H
//
//  Contient les définitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : KRZISCH Ph. janvier 92   Dernière modif : 12/08/93
//---------------------------------------------------------------------------
#ifndef __NSV_PERSON_H
#define __NSV_PERSON_H

# include <cstring.h>
# include <owl\window.h>

# include "ns_sgbd\nsfiche.h"
# include "partage\NTArray.h"
/********************************
# ifdef _MUE
# include "nsbb\nsmpids.h"
# endif
***********************************/


//
// Taille des champs du fichier PATIENT
//
#define NSV_PAT_NSS_LEN        6
#define NSV_PAT_NOM_LEN       35
#define NSV_PAT_PRENOM_LEN    35
#define NSV_PAT_CODE_LEN      20
#define NSV_PAT_SEXE_LEN	   1
#define NSV_PAT_NAISSANCE_LEN  8
#define NSV_PAT_CONVOC_LEN    20
#define NSV_PAT_ADRESSE_LEN    5
#define NSV_PAT_TELEPOR_LEN   15
#define NSV_PAT_TELEBUR_LEN   15
#define NSV_PAT_SITFAM_LEN     2
#define NSV_PAT_LANG_LEN       2

#define NSV_PAT_NOM_LONG_LEN  80

//
// Indice des champs du fichier PATIENT
//
#define NSV_PAT_NSS_FIELD        1
#define NSV_PAT_NOM_FIELD        2
#define NSV_PAT_PRENOM_FIELD     3
#define NSV_PAT_CODE_FIELD       4
#define NSV_PAT_CONVOC_FIELD     5
#define NSV_PAT_SEXE_FIELD		 6
#define NSV_PAT_ADRESSE_FIELD    7
#define NSV_PAT_NAISSANCE_FIELD  8
#define NSV_PAT_TELEPOR_FIELD 	 9
#define NSV_PAT_TELEBUR_FIELD 	10
#define NSV_PAT_SITFAM_FIELD 	11
#define NSV_PAT_LANG_FIELD 	    12

//
// Taille des champs du fichier UTILISAT
//

# define NSV_UTI_INDICE_LEN      3
# define NSV_UTI_CODE_LEN        5
# define NSV_UTI_NOM_LEN        35
# define NSV_UTI_PRENOM_LEN     35
# define NSV_UTI_ADRESSE_LEN     5
# define NSV_UTI_TELEPHONE_LEN  15
# define NSV_UTI_TYPE_LEN        1
# define NSV_UTI_GROUPE_LEN      2
# define NSV_UTI_SECURITE_LEN   20
# define NSV_UTI_MESSAGERIE_LEN 35
# define NSV_UTI_POS_ADMIN_LEN  19
# define NSV_UTI_POS_HISTO_LEN  19
# define NSV_UTI_POS_SYNTH_LEN  19
# define NSV_UTI_LANG_LEN        2

//
// Indice des champs du fichier UTILISAT
//

# define NSV_UTI_INDICE_FIELD      1
# define NSV_UTI_CODE_FIELD        2
# define NSV_UTI_NOM_FIELD         3
# define NSV_UTI_PRENOM_FIELD      4
# define NSV_UTI_ADRESSE_FIELD     5
# define NSV_UTI_TELEPHONE_FIELD   6
# define NSV_UTI_TYPE_FIELD        7
# define NSV_UTI_GROUPE_FIELD      8
# define NSV_UTI_SECURITE_FIELD    9
# define NSV_UTI_MESSAGERIE_FIELD 10
# define NSV_UTI_POS_ADMIN_FIELD  11
# define NSV_UTI_POS_HISTO_FIELD  12
# define NSV_UTI_POS_SYNTH_FIELD  13
# define NSV_UTI_LANG_FIELD       14

//
// Taille des champs du fichier CORRESP
//
# define NSV_COR_CODE_LEN        3
# define NSV_COR_NOM_LEN        35
# define NSV_COR_PRENOM_LEN     35
# define NSV_COR_SEXE_LEN		1
# define NSV_COR_ADRESSE_LEN     5
# define NSV_COR_DOCTEUR_LEN     1
# define NSV_COR_TELEPOR_LEN    15
# define NSV_COR_TELEBUR_LEN	   15
# define NSV_COR_NB_EXEMP_LEN    2
# define NSV_COR_MESSAGERIE_LEN 50
# define NSV_COR_FONCTION_LEN 	6
# define NSV_COR_ACTIF_LEN 	 	1
# define NSV_COR_LANG_LEN 	 	2

//
// Indice des champs du fichier CORRESP
//
# define NSV_COR_CODE_FIELD        1
# define NSV_COR_NOM_FIELD         2
# define NSV_COR_PRENOM_FIELD      3
# define NSV_COR_SEXE_FIELD		  4
# define NSV_COR_ADRESSE_FIELD     5
# define NSV_COR_DOCTEUR_FIELD     6
# define NSV_COR_TELEPOR_FIELD     7
# define NSV_COR_TELEBUR_FIELD	  8
# define NSV_COR_NB_EXEMP_FIELD    9
# define NSV_COR_MESSAGERIE_FIELD 10
# define NSV_COR_FONCTION_FIELD   11
# define NSV_COR_ACTIF_FIELD   	 12
# define NSV_COR_LANG_FIELD   	 13

//---------------------------------------------------------------------------
//  Classe NSVersionPatient
//---------------------------------------------------------------------------
//
// Objet contenant les données
//
class NSVersionPatientData : public NSRoot
{
 public:
	//
	// Variables de stockage des valeurs
	//
	char nss[NSV_PAT_NSS_LEN + 1];
	char nom[NSV_PAT_NOM_LEN + 1];
	char prenom[NSV_PAT_PRENOM_LEN + 1];
	char code[NSV_PAT_CODE_LEN + 1];
	char convoc[NSV_PAT_CONVOC_LEN + 1];
	char sexe[NSV_PAT_SEXE_LEN + 1];
	char adresse[NSV_PAT_ADRESSE_LEN + 1];
	char naissance[NSV_PAT_NAISSANCE_LEN + 1];
	char telepor[NSV_PAT_TELEPOR_LEN + 1];
	char telebur[NSV_PAT_TELEBUR_LEN + 1];
	char sitfam[NSV_PAT_SITFAM_LEN + 1];
	char lang[NSV_PAT_LANG_LEN + 1];

	char nom_long[NSV_PAT_NOM_LONG_LEN + 1];

	NSVersionPatientData(NSContexte* pCtx) ;
	NSVersionPatientData(NSVersionPatientData& rv) ;

	NSVersionPatientData& operator=(NSVersionPatientData src);
	int                   operator==(NSVersionPatientData& o);

	void 						metAZero();
	void 						fabriqueNomLong(string sLang = "");
  string          fabriqueCivilite(bool bShort, string sLang = "");
	int	 						donneNaissance(char *dateNaiss);
	bool 						estMasculin() { return (sexe[0] == '1'); }
	bool 						estFeminin()  { return (sexe[0] == '2'); }
	void 						metMasculin() { sexe[0] = '1'; }
	void 						metFeminin()  { sexe[0] = '2'; }
	void 						setNbEnfants(int nbenf);   // ajoute le nb d'enfants à sitfam
	int  						getNbEnfants();            // retourne le nb d'enfants d'après sitfam

	// données "traduites" en français
	string 					donneSitFam(string sLang = "");
	string 					donneNbEnfants();
	string 					donneDateNaiss(string sLang = "");
  string 					donneTitre(string sLang = "");
};

//---------------------------------------------------------------------------
//  Classe NSVersionPatInfo  (destinée à être stockée dans une Array)
//---------------------------------------------------------------------------
class NSVersionPatient;
class NSAdressesInfo;
class NSCorrespArray;
class NSPatPathoArray;

class NSVersionCorrespondantInfo;
typedef NTArray<NSVersionCorrespondantInfo> NSVersionCorrespArray;
typedef std::vector<NSVersionCorrespondantInfo*>::iterator VersionCorrespInfoIter;

class  NSVersionPatInfo : public NSRoot
{
 public:
	//
	// Objet qui contient les données
	//
	NSVersionPatientData* 	pDonnees ;

  NSAdressesInfo*         pAdresseInfo ;
	NSVersionCorrespArray*	pCorrespArray ;

	NSVersionPatInfo(NSContexte* pCtx) ;
	NSVersionPatInfo(NSVersionPatient*) ;
	NSVersionPatInfo(NSVersionPatInfo& rv) ;
	~NSVersionPatInfo() ;

	bool   initAdresseInfo(string pathname) ;
	bool   initCorrespArray(string pathname) ;

  void   fabriqueNomLong(string sLang = "");
	int    donneNaissance(char *dateNaiss) { return (pDonnees->donneNaissance(dateNaiss)) ; }
	bool   estMasculin() 									 { return (pDonnees->estMasculin()) ; }
	bool   estFeminin()  									 { return (pDonnees->estFeminin()) ; }
	string donneSitFam()    							 { return (pDonnees->donneSitFam()) ; }
	string donneNbEnfants() 							 { return (pDonnees->donneNbEnfants()) ; }
	string donneDateNaiss() 							 { return (pDonnees->donneDateNaiss()) ; }
	string donneTitre()     							 { return (pDonnees->donneTitre()) ; }
	string donneLang()      							 { return string(pDonnees->lang) ; }

	NSVersionPatInfo& operator=(NSVersionPatInfo src) ;
	int               operator==(NSVersionPatInfo& o) ;
};

//
// Objet dérivé de NSFiche servant aux accès de base de données
//
class NSVersionPatient : public NSVersionPatInfo, public NSFiche
{
 public :

	NSVersionPatient(NSSuper* pSuper);
	~NSVersionPatient();

	void			alimenteFiche();
	void			videFiche();
	DBIResult open(string pathbase);
	DBIResult close();

	DBIResult	chercheNomPrenom(char *nomATrouver, char *prenomATrouver);

	bool			ExisteCode(char* code, NSVersionCorrespArray *pRefCorrespArray);
};

//---------------------------------------------------------------------------
//  Classe NSVersionUtilisateur
//---------------------------------------------------------------------------
string VersionStringTitre(string sTypeTitre, string sLang = "fr") ;
string VersionStringAvantTitre(string sTypeTitre, string sLang = "fr") ;

class NSVersionUtilisateurData
{
 public:
	//
	// Variables de stockage des valeurs
	//
	// Nota : en mode inclus la variable indice
	// contient l'id du Professional

	char indice[NSV_UTI_INDICE_LEN + 1];
	char code[NSV_UTI_CODE_LEN + 1];
	char nom[NSV_UTI_NOM_LEN + 1];
	char prenom[NSV_UTI_PRENOM_LEN + 1];
	char adresse[NSV_UTI_ADRESSE_LEN + 1];
	char telephone[NSV_UTI_TELEPHONE_LEN + 1];
	char type[NSV_UTI_TYPE_LEN + 1];
	char groupe[NSV_UTI_GROUPE_LEN + 1];
	char securite[NSV_UTI_SECURITE_LEN + 1];
	char messagerie[NSV_UTI_MESSAGERIE_LEN + 1];
	char posAdmin[NSV_UTI_POS_ADMIN_LEN + 1];
	char posHisto[NSV_UTI_POS_HISTO_LEN + 1];
	char posSynth[NSV_UTI_POS_SYNTH_LEN + 1];
	char lang[NSV_UTI_LANG_LEN + 1];

	NSVersionUtilisateurData();
	NSVersionUtilisateurData(NSVersionUtilisateurData& rv);

	NSVersionUtilisateurData&  operator=(NSVersionUtilisateurData src);
	int					operator==(NSVersionUtilisateurData& o);

	void				metAZero();
	string				donneTitre(string sLang = "fr");
	string				donneAvantTitre(string sLang = "fr");
	string				donneSignature(string sLang = "fr");
};

class NSVersionUtilisateur;
class NSUtilisateurChoisi;

//
// Classe NSVersionUtiliInfo
//
class NSVersionUtiliInfo : public NSRoot
{
    public:
	    //
	    // Objet qui contient les données
	    //
	    NSVersionUtilisateurData*	pDonnees;
	    NSAdressesInfo*	 	pAdresseInfo;

	    NSVersionUtiliInfo(NSContexte* pCtx);
	    NSVersionUtiliInfo(NSVersionUtilisateur*);
	    NSVersionUtiliInfo(NSVersionUtiliInfo& rv);
	    NSVersionUtiliInfo(NSUtilisateurChoisi*);
	    ~NSVersionUtiliInfo();

	    bool 		    initAdresseInfo(string pathname);

        string          getNss();
        const char*     getszNss();

	    string		    donneLang()      { return string(pDonnees->lang); }

	    NSVersionUtiliInfo&    operator=(NSVersionUtiliInfo src);
	    int 		    operator==(NSVersionUtiliInfo& o);
};

class NSVersionUtilisateur : public NSVersionUtiliInfo, public NSFiche
{
    public :

	    NSVersionUtilisateur(NSSuper* pSuper);
	    ~NSVersionUtilisateur();

	    DBIResult	open(string pathbase);
	    DBIResult   close();
	    void	    alimenteFiche();
	    void	    videFiche();
	    DBIResult	getRecord();
};

//---------------------------------------------------------------------------
//  Fonction:		NSVersionUtiliInfo::getNss()
//  Description:	Recherche le pids de l'utilisateur.
//  Retour:			string sNss ou vide si l'indice n'était pas initialisé
//---------------------------------------------------------------------------
string
NSVersionUtiliInfo::getNss()
{
    return string(pDonnees->indice);
}

//---------------------------------------------------------------------------
//  Fonction:		NSVersionUtiliInfo::getszNss()
//  Description:	idem getNss() mais au format char*.
//  Retour:			char* szNss ou vide si l'indice n'était pas initialisé
//---------------------------------------------------------------------------
const char*
NSVersionUtiliInfo::getszNss()
{
    string sNss = getNss();

    return (sNss.c_str());
}

//---------------------------------------------------------------------------
//  Classe NSVersionCorrespondant
//---------------------------------------------------------------------------
class NSVersionCorrespondantData
{
 public :

	//
	// Variables de stockage des valeurs
	//
	// Nota : le code corresp correspond à un id de professionnel
	char code[NSV_COR_CODE_LEN + 1];
	char nom[NSV_COR_NOM_LEN + 1];
	char prenom[NSV_COR_PRENOM_LEN + 1];
	char sexe[NSV_COR_SEXE_LEN + 1];
	char adresse[NSV_COR_ADRESSE_LEN + 1];
	char docteur[NSV_COR_DOCTEUR_LEN + 1];
	char telepor[NSV_COR_TELEPOR_LEN + 1];
	char telebur[NSV_COR_TELEBUR_LEN + 1];
	char nb_exemp[NSV_COR_NB_EXEMP_LEN + 1];
	char messagerie[NSV_COR_MESSAGERIE_LEN + 1];
	char fonction[NSV_COR_FONCTION_LEN + 1];
	char actif[NSV_COR_ACTIF_LEN + 1];
	char lang[NSV_COR_LANG_LEN + 1];

	NSVersionCorrespondantData();
	NSVersionCorrespondantData(NSVersionCorrespondantData& rv);

	NSVersionCorrespondantData&	operator=(NSVersionCorrespondantData src);
	int 			     				operator==(NSVersionCorrespondantData& o);

	void									metAZero();
	string								donneTitre(string sLang = "fr");
	string								donneAvantTitre(string sLang = "fr");

	bool									estMasculin() { return (sexe[0] == '1'); }
	bool									estFeminin()  { return (sexe[0] == '2'); }
	void									metMasculin() { sexe[0] = '1'; }
	void									metFeminin()  { sexe[0] = '2'; }

	bool									estActif()    { return (actif[0] == '1'); }
	void									metActif()    { actif[0] = '1'; }
	void									metInactif()  { actif[0] = '0'; }
};

class NSVersionCorrespondant;

class NSVersionCorrespondantInfo : public NSRoot
{
 public:
	//
	// Objet qui contient les données
	//

    NSVersionCorrespondantData*	  pDonnees;
	NSAdressesInfo*			  pAdresseInfo;

	NSVersionCorrespondantInfo(NSContexte* pCtx);
	NSVersionCorrespondantInfo(NSVersionCorrespondant*);
	NSVersionCorrespondantInfo(NSVersionCorrespondantInfo& rv);
	~NSVersionCorrespondantInfo();

	bool        initAdresseInfo(string pathname);
	string		donneLang()      { return string(pDonnees->lang); }

	NSVersionCorrespondantInfo&	operator=(NSVersionCorrespondantInfo src);
	int										operator==(NSVersionCorrespondantInfo& o);
};



class NSVersionCorrespondant : public NSVersionCorrespondantInfo, public NSFiche
{
 public:

	NSVersionCorrespondant(NSSuper* pSuper);
	~NSVersionCorrespondant();

	DBIResult open(string pathbase);
	DBIResult close();
	void 		alimenteFiche();
	void 		videFiche();
	DBIResult	getRecord();

	short 		prendStatut() { return statut; }
	void  		metStatut(short nouvStatut) { statut = nouvStatut; }

    bool 		chercheCorrespInfo(string pathname, char *cleCorresp, NSVersionCorrespondantInfo* pCorrespInfo);
};

//---------------------------------------------------------------------------
//  Function:    NSVersionPatientData::donneNaissance(char *dateNaiss)
//
//  Description: Retourne dans dateNaiss la date de naissance du patient
//					  au format AAAAMMJJ
//					  Si le champ existe mais n'a pas été renseigné,
//					  retourne "00000000"
//
//  Returns:     1 si OK, 0 si date de naissance n'est pas un champ du fichier
//---------------------------------------------------------------------------
int
NSVersionPatientData::donneNaissance(char *dateNaiss)
{
    //
    // La date de naissance doit être constituée d'exactement PAT_NAISSANCE_LEN digits
    // Birthdate must have exactly PAT_NAISSANCE_LEN digits
    //
    int i ;
    for (i = 0 ; (i < NSV_PAT_NAISSANCE_LEN) && (naissance[i] >= '0') && (naissance[i] <= '9'); i++) ;
    if (i < NSV_PAT_NAISSANCE_LEN)
    {
        strcpy(dateNaiss, "00000000");
        return 1;
    }

	if (strcmp(naissance, "19000000") == 0)
		strcpy(dateNaiss, "00000000");
    else
        strcpy(dateNaiss, naissance);

	return 1;
}


//---------------------------------------------------------------------------
#endif    // __NSVPERSON_H
