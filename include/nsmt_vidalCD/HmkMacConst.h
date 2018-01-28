
/* ************************************************************************ */
/* Module : HmkMacConst.H     PROJET : Tous projets liés à HMKMAC.DLL       */
/* ************************************************************************ */
/* FONCTIONNALITES                                                          */
/* Déclaration des constantes d'API HMKMAC.DLL                              */
/* ************************************************************************ */
/* Auteur : (généré par MakeIncude.exe)                                     */
/* Compatible avec HMKMAC.DLL version 1.4.1                                 */
/* ************************************************************************ */


#if !defined(_INC_HMKMACCONST_H)
	#define _INC_HMKMACCONST_H

	//  Niveaux de gravité
	//  -----------------------------------------------
	// Liste_G
	#define G_AprendreEnCompte		2		// à prendre en compte
	#define G_PrecautionEmploi		4		// précaution d'emploi
	#define G_AssociationDeconseillee		6		// association déconseillée
	#define G_ContreIndication		8		// contre-indication

	//  Types de champ
	//  -----------------------------------------------
	// Liste_C
	#define C_Aucun		0		// aucun
	#define C_Reduit		1		// nom réduit
	#define FormeGalenique		2		// forme galénique
	#define C_Hopital		3		// code Ville / Hôpital
	#define C_Prix		4		// prix public TTC
	#define C_Descriptif		5		// nom complet

	//  Types d'index
	//  -----------------------------------------------
	// Liste_I
	#define I_Direct		0		// direct
	#define I_Etendu		1		// étendu
	#define I_MiniDirect		2		// minimum direct
	#define I_MiniEtendu		3		// minimum étendu
	#define I_Complet		1		// complet
	#define I_Minimum		2		// minimum

	//  Fonds documentaires
	//  -----------------------------------------------
	// Liste_D
	#define D_GuideNational		0		// Guide National de Prescription
	#define D_VidalParticulier		1		// Vidal de la Famille
	#define D_Vidal		2		// Dictionnaire Vidal

	//  Rubriques de monographies
	//  -----------------------------------------------
	// Liste_R
	#define R_Toutes		0		// toutes
	#define R0_CI		1		// contre-indications
	#define R_Composition		2		// composition
	#define R_EffetsInd		3		// effets indésirables
	#define R_MiseEnGarde		4		// mise en garde et précautions d'emploi
	#define R_Grossesse		5		// grossesse
	#define R_Indications		6		// indications
	#define R_Interactions		7		// interactions
	#define R_Posologie		8		// posologie
	#define R_Formes		9		// formes et présentations
	#define R_RMO		10		// RMO

	//  Codes hospitaliers
	//  -----------------------------------------------
	// Liste_H
	#define H_ResHop		0		// réservé à l'usage hospitalier
	#define H_IniHop		1		// prescription initiale hospitalière
	#define H_IniHopAR		2		// prescription initiale hospitalière, à renouveler

	//  Codes mise à jour
	//  -----------------------------------------------
	// Liste_J
	#define J_Ok		0		// médicament sur le marché
	#define J_Supprime		1		// médicament supprimé
	#define J_Vigilence		2		// médicament en pharmacovigilence

	//  Résultats d'initialisation
	//  -----------------------------------------------
	// Liste_INI
	#define INI_BASE_ERR		0		// la base est corrompue
	#define INI_TVS_OK		1		// base TVS, initialisation réussie
	#define INI_TVS_DATEINVALIDE		-1		// base TVS, date limite d'utilisation dépassée
	#define INI_VCD_OK		8		// base VCD, initialisation réussie
	#define INI_VCD_DATEINVALIDE		-8		// base Vidal CD, date limite d'utilisation dépassée
	#define INI_TVS_FINDEMO		-101		// base TVS Démonstration, échéance dépassée
	#define INI_TVS_FULLACCESS		-1004		// base TVS, nombre maximal de connexions réseau att
	#define INI_TVS_LICENCE		-1008		// base TVS, numéro de licence invalide
	#define INI_TVS_LOCK		-1016		// base TVS, vérouillée
	#define INI_TVS_LOAD		-1003		// base TVS, erreur de chargement de la librairie
	#define INI_TVS_SERVER		-1014		// base TVS, impossible d'accéder au serveur

	//  Types de qualifiant
	//  -----------------------------------------------
	// Liste_Q
	#define Q_Nom		1		// nom
	#define Q_Substance		2		// substance
	#define Q_Classe		3		// classe thérapeutique
	#define Q_Labo		4		// laboratoire
	#define Q_Tous		5		// tous
	#define Q_Cip		6		// code cip
	#define Q_Excipient		7		// excipient
	#define Q_Ci		8		// contre indication
	#define Q_Hopital		9		// code hospitalier
	#define Q_CodeMarche		10		// code marché
	#define Q_RMO		11		// RMO
	#define Q_CodeDopant		12		// code dopant
	#define Q_Voie		13		// voie d'administration
	#define Q_FormeGal		14		// forme galénique
	#define Q_CodeRem		15		// code remboursement
	#define Q_CodeGenerique		16		// code générique
	#define Q_Collectivite		17		// collectivité
	#define Q_Duree		18		// durée de prescription
	#define Q_Liste		19		// liste
	#define Q_Categorie		20		// catégorie
	#define Q_NomReduit		21		// nom réduit
	#define Q_Monograhie		22		// monographie
	#define Q_Composition		23		// composition
	#define Q_ClassifSemp		24		// classification des produits SEMP
	#define Q_Saumon		25		// pages Saumon du dictionnaire Vidal
	#define Q_indication		27		// Indications
	#define Q_SituPresc		28		// Groupes d'indications
	#define Q_CIM10		29		// Classification Internationale des Maladies et sym
	#define Q_EPhMRA		30		// Classification de l'European Pharmaceutical Marke
	#define Q_ATC		31		// Classification ATC (Anatomique - Thérapeutique - 

	//  Catégories de produits
	//  -----------------------------------------------
	// Liste_P
	#define P_Tous		127		// tous les produits
	#define P_Vidal		64		// médicaments Vidal
	#define P_Diet		32		// produits diététiques Semp
	#define P_Veto		16		// produits vétérinaires Semp
	#define P_Para		8		// produits de parapharmacie Semp
	#define P_Acces		4		// accessoires Semp
	#define P_Divers		2		// divers Semp
	#define P_Homeo		1		// produits homéopathiques

	//  Erreurs d'exécution
	//  -----------------------------------------------
	// Liste_ERR
	#define ERR_OK		0		// ok
	#define ERR_DATEINVALIDE		1		// date invalide
	#define ERR_ENDUSE		2		// fin d'exploitation
	#define ERR_MEMORYOVERFLOW		3		// dépassement mémoire
	#define ERR_OPENFILE		4		// erreur ouverture fichier
	#define ERR_NODOCUMENT		6		// non documenté
	#define ERR_NOANSWER		7		// pas de réponse
	#define ERR_NOMINIGRAPHIE		8		// pas de minigraphie
	#define ERR_NOINTERACTION		9		// pas d'interaction
	#define ERR_NOCONTREINDIC		10		// pas de contre-indication
	#define ERR_NOCLASSIF		11		// pas de classification
	#define ERR_NOMED		12		// pas de médicament sans interaction
	#define ERR_BASE		13		// base corrompue
	#define ERR_FULLACCESS		14		// nombre d'accès au réseau dépassé
	#define ERR_VERSIONFILE		15		// fichier version corrompu
	#define ERR_VERSIONDATA		16		// données inadaptées à cette version de la librairi
	#define ERR_INDEXNONSUPPORTE		17		// Type d’index non supporté

#endif

