#ifndef nsmpidsH
#define nsmpidsH

# include <cstring.h>
# include <owl\window.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "ns_sgbd\nsfiche.h"

//
// Taille des champs du fichier PIDS
//
// #define PIDS_NSS_LEN        7    // defined inside nsglobal.h
#define PIDS_ROOTDOC_LEN    6
#define PIDS_NOM_LEN       35
#define PIDS_PRENOM_LEN    35
#define PIDS_CODE_LEN      20
#define PIDS_SEXE_LEN	    1
#define PIDS_NAISSANCE_LEN  8

//
// Indice des champs du fichier PIDS
//
#define PIDS_NSS_FIELD        1
#define PIDS_ROOTDOC_FIELD    2
#define PIDS_NOM_FIELD        3
#define PIDS_PRENOM_FIELD     4
#define PIDS_CODE_FIELD       5
#define PIDS_SEXE_FIELD		  6
#define PIDS_NAISSANCE_FIELD  7

//---------------------------------------------------------------------------
//  Classe NSPids
//---------------------------------------------------------------------------
//
// Objet contenant les donn�es
//
class NSPidsData
{
 public:
	//
	// Variables de stockage des valeurs
	//
	char nss[PIDS_NSS_LEN + 1];
    char rootdoc[PIDS_ROOTDOC_LEN + 1];
	char nom[PIDS_NOM_LEN + 1];
	char prenom[PIDS_PRENOM_LEN + 1];
	char code[PIDS_CODE_LEN + 1];
	char sexe[PIDS_SEXE_LEN + 1];
	char naissance[PIDS_NAISSANCE_LEN + 1];

	NSPidsData();
	NSPidsData(NSPidsData& rv);

	NSPidsData&	    operator=(NSPidsData src);
	int 		   	operator==(NSPidsData& o);
	void 			metABlanc();
	void 			metAZero();
};

//---------------------------------------------------------------------------
//  Classe NSPidsInfo  (destin�e � �tre stock�e dans une Array)
//---------------------------------------------------------------------------

class  NSPidsInfo : public NSRoot
{
 public:
	//
	// Objet qui contient les donn�es
	//
	NSPidsData* 	pDonnees;

	NSPidsInfo(NSContexte* pCtx);
	NSPidsInfo(NSPidsInfo& rv);
	~NSPidsInfo();

	NSPidsInfo&	operator=(NSPidsInfo src);
	int 		operator==(NSPidsInfo& o);
};

//
// Objet d�riv� de NSFiche servant aux acc�s de base de donn�es
//

#endif

