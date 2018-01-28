//---------------------------------------------------------------------------
// Base contenant l'identifiant patient dans le modele MUE
//---------------------------------------------------------------------------
#include "nautilus\nsmpids.h"

//***************************************************************************
// Implémentation des méthodes NSPids
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSPidsData::NSPidsData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSPidsData::NSPidsData()
{
	metAZero();
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::NSPidsData(NSPidsData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPidsData::NSPidsData(NSPidsData& rv)
{
	strcpy(nss,								rv.nss);
    strcpy(rootdoc,                         rv.rootdoc);
	strcpy(nom,								rv.nom);
	strcpy(prenom,							rv.prenom);
	strcpy(code,							rv.code);
	strcpy(sexe,							rv.sexe);
	strcpy(naissance,						rv.naissance);
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::operator=(NSPidsData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSPidsData&
NSPidsData::operator=(NSPidsData src)
{
	strcpy(nss, 	  					src.nss);
    strcpy(rootdoc,                     src.rootdoc);
	strcpy(nom, 	  					src.nom);
	strcpy(prenom, 	  					src.prenom);
	strcpy(code,   	  					src.code);
	strcpy(sexe,      					src.sexe);
	strcpy(naissance, 					src.naissance);

	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsData::operator==(NSPidsData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSPidsData::operator == ( NSPidsData& o )
{
	if ((strcmp(nss, 		  							o.nss) 							== 0) &&
            (strcmp(rootdoc,                            o.rootdoc)                      == 0) &&
			(strcmp(nom, 		  						o.nom) 							== 0) &&
			(strcmp(prenom, 	  						o.prenom) 						== 0) &&
			(strcmp(code,   	  						o.code) 						== 0) &&
			(strcmp(sexe,      							o.sexe) 						== 0) &&
			(strcmp(naissance, 							o.naissance) 					== 0)
			)
		return 1;
	else
		return 0;
}

//---------------------------------------------------------------------------
//  Function:    NSPidsData::metAZero()
//
//  Description: Met à zéro les variables de la fiche
//---------------------------------------------------------------------------
void
NSPidsData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(nss,       					0,	PIDS_NSS_LEN + 1);
    memset(rootdoc,                     0,  PIDS_ROOTDOC_LEN + 1);
	memset(nom,       					0, 	PIDS_NOM_LEN + 1);
	memset(prenom,    					0, 	PIDS_PRENOM_LEN + 1);
	memset(code,      					0, 	PIDS_CODE_LEN + 1);
	memset(sexe,      					0, 	PIDS_SEXE_LEN + 1);
	memset(naissance, 					0, 	PIDS_NAISSANCE_LEN + 1);
}

//---------------------------------------------------------------------------
//  Function:    NSPidsData::metABlanc()
//
//  Description: Met à blanc les variables de la fiche
//---------------------------------------------------------------------------
void
NSPidsData::metABlanc()
{
	//
	// Met les champs de données à blanc
	//
	memset(nss,       					' ', 	PIDS_NSS_LEN);
    memset(rootdoc,                     ' ',    PIDS_ROOTDOC_LEN);
	memset(nom,       					' ', 	PIDS_NOM_LEN);
	memset(prenom,    					' ', 	PIDS_PRENOM_LEN);
	memset(code,      					' ', 	PIDS_CODE_LEN);
	memset(sexe,      					' ', 	PIDS_SEXE_LEN);
	memset(naissance, 					' ', 	PIDS_NAISSANCE_LEN);
}

/*****************************************************************************/
// 							METHODES DE NSPids
/*****************************************************************************/

#ifndef N_TIERS

//---------------------------------------------------------------------------
//  Function:    NSPids::NSPids(NSSuper* pSuper)
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSPids::NSPids(NSContexte* pCtx, int iType)
	:	NSPidsInfo(pCtx),
		NSFiche(pCtx)
{
    iTypePids = iType;
}

//---------------------------------------------------------------------------
//  Function:  NSPids::~NSPids()
//
//  Arguments:
//
//  Description:
//             Destructeur.
//             Ferme proprement une classe de ce type.
//  Returns:
//             Aucun
//---------------------------------------------------------------------------
NSPids::~NSPids()
{
}

//---------------------------------------------------------------------------
//  Function:  NSPids::alimenteFiche()
//
//  Arguments:
//
//  Description:
//             Transfère le contenu de pRecBuff dans les variables de
//             la fiche
//  Returns:
//             Rien
//---------------------------------------------------------------------------
void NSPids::alimenteFiche()
{
	alimenteChamp(pDonnees->nss, 			PIDS_NSS_FIELD,		 		PIDS_NSS_LEN);
    alimenteChamp(pDonnees->rootdoc,        PIDS_ROOTDOC_FIELD,         PIDS_ROOTDOC_LEN);
	alimenteChamp(pDonnees->nom, 			PIDS_NOM_FIELD,		 		PIDS_NOM_LEN);
	alimenteChamp(pDonnees->prenom, 		PIDS_PRENOM_FIELD,	 		PIDS_PRENOM_LEN);
	alimenteChamp(pDonnees->code, 			PIDS_CODE_FIELD,		 	PIDS_CODE_LEN);
	alimenteChamp(pDonnees->sexe, 			PIDS_SEXE_FIELD,		 	PIDS_SEXE_LEN);
	alimenteChamp(pDonnees->naissance,	    PIDS_NAISSANCE_FIELD,	    PIDS_NAISSANCE_LEN);
}

//---------------------------------------------------------------------------
//  Function:  NSPids::videFiche()
//
//  Description: Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void NSPids::videFiche()
{
	videChamp(pDonnees->nss, 			PIDS_NSS_FIELD,		 		PIDS_NSS_LEN);
    videChamp(pDonnees->rootdoc,        PIDS_ROOTDOC_FIELD,         PIDS_ROOTDOC_LEN);
	videChamp(pDonnees->nom, 			PIDS_NOM_FIELD,		 		PIDS_NOM_LEN);
	videChamp(pDonnees->prenom, 		PIDS_PRENOM_FIELD,	 		PIDS_PRENOM_LEN);
	videChamp(pDonnees->code, 			PIDS_CODE_FIELD,		 	PIDS_CODE_LEN);
	videChamp(pDonnees->sexe, 			PIDS_SEXE_FIELD,		 	PIDS_SEXE_LEN);
	videChamp(pDonnees->naissance,	    PIDS_NAISSANCE_FIELD,	    PIDS_NAISSANCE_LEN);
}

//---------------------------------------------------------------------------
//  Function:  NSPids::open()
//
//  Arguments:
//
//  Description:
//             Ouvre la table primaire et les index secondaires
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSPids::open()
{
    char tableName[255];
    if (iTypePids == pidsPatient)        strcpy(tableName, "NSPIDS.DB");    else        strcpy(tableName, "NSPIDSCOR.DB");

	//
	// Appelle la fonction open() de la classe de base pour ouvrir
	// l'index primaire
	//
	lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL);
    return (lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSPids::close()
//
//  Arguments:
//
//  Description:
//             Ferme la table primaire et l'index secondaire
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSPids::close()
{
	//
	// Appelle la fonction close() de la classe de base pour fermer
	// l'index primaire
	//
	lastError = NSFiche::close();

    return (lastError);
}

#endif // ifndef N_TIERS

//***************************************************************************
//
// Implémentation des méthodes NSPidsInfo
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::NSPidsInfo(NSSuper* pSuper)
//  Description:	Constructeur avec superviseur (pour les accès aux bases)
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(NSContexte* pCtx)
	:	NSRoot(pCtx)
{
	// Crée l'objet de données
	pDonnees 	  	= new NSPidsData();
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::NSPidsInfo(NSPids*)
//  Description:	Constructeur à partir d'un NSPids
//  Retour:			Rien
//---------------------------------------------------------------------------
#ifndef N_TIERS
NSPidsInfo::NSPidsInfo(NSPids* pPids)
	:	NSRoot(pPids->NSPidsInfo::pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees 	   		= new NSPidsData();

	//
	// Copie les données du NSPids
	//
	*pDonnees 	   	= *(pPids->pDonnees);
}
#endif

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::~NSPidsInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPidsInfo::~NSPidsInfo()
{
	delete pDonnees;
  // pas de delete du pSuper
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::NSPidsInfo(NSPidsInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSPidsInfo::NSPidsInfo(NSPidsInfo& rv)
	:	NSRoot(rv.pContexte)
{
	//
	// Crée l'objet de données
	//
	pDonnees 	   		= new NSPidsData();
	//
	// Copie les valeurs du NSPidsInfo d'origine
	//
	*pDonnees 	   	= *(rv.pDonnees);
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::operator=(NSPidsInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSPidsInfo& NSPidsInfo::operator=(NSPidsInfo src)
{
	*pDonnees 	   = *(src.pDonnees);
    pContexte      = src.pContexte;
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction:		NSPidsInfo::operator==(NSPidsInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSPidsInfo::operator == ( NSPidsInfo& o )
{
	int egal = 1;

	if (!(*pDonnees 			== *(o.pDonnees)))
		egal = 0;

    return egal;
}

// fin du fichier nsmpids.cpp
