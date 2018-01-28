
/* ************************************************************************ */
/* Module : MvSmpl32Fn.H        PROJET : Tous projets liés à MVSMPL32.DLL   */
/* ************************************************************************ */
/* FONCTIONNALITES                                                          */
/* Déclaration des fonctions d'API MVSMPL32.DLL                             */
/* ************************************************************************ */
/* Auteur : (généré par MakeIncude.exe)                                     */
/* Compatible avec MVSMPL32.DLL version 1.4.1                               */
/* ************************************************************************ */


#include "windows.h"
#if !defined(_INC_MVSMPL32FN_H)
	#define _INC_MVSMPL32FN_H
#endif
#ifdef __cplusplus
	extern "C" {
#endif

/*  =============================================== */
/*  NOUVELLE BIBLIOTHEQUE VIDAL */
/*  =============================================== */
/*  Mécanismes techniques : Adaptations d'interface */
/*  ----------------------------------------------- */
__declspec(dllimport) long FAR PASCAL VDL_ptr2str2 ( char * lPointeur, long lOffset, long lTailleBuffer, char * pszBuffer );
__declspec(dllimport) long FAR PASCAL VDL_GetLastError ( );
/*  Mécanismes techniques : Session de transactions */
/*  ----------------------------------------------- */
__declspec(dllimport) long FAR PASCAL VDL_Ini ( char * pszChemin );
__declspec(dllimport) char * FAR PASCAL VDL_Version ( );
__declspec(dllimport) char * FAR PASCAL VDL_GetIntruder ( );
__declspec(dllimport) char * FAR PASCAL VDL_Close ( );
/*  Sélection de produits */
/*  --------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_Med ( char * Terme, long TypeQualifiant, char * lTher, long TypeIndex, long TypeChamp, long TypeProduit );
__declspec(dllimport) char * FAR PASCAL VDL_Gen ( char * pszUnCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetGenGroupCIPs ( char * pszCodeGroupe );
__declspec(dllimport) char * FAR PASCAL VDL_GetDCIGroupCIPs ( char * pszODS );
__declspec(dllimport) char * FAR PASCAL VDL_Iam3 ( char * pszID_Classe );
/*  Sélection de critères : par noms */
/*  -------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_Qualif ( char * szRecherche, long TypeQualifiant, long TypeIndex, long TypeProduit );
__declspec(dllimport) char * FAR PASCAL VDL_QualifFull ( long TypeQualifiant );
__declspec(dllimport) char * FAR PASCAL VDL_GetGIs ( char * pszFind );
__declspec(dllimport) char * FAR PASCAL VDL_GetGenGroups ( char * pszFind );
__declspec(dllimport) char * FAR PASCAL VDL_GetDCIGroups ( char * pszFind );
__declspec(dllimport) char * FAR PASCAL VDL_GetGenGroupsAndDCIGroups ( char * pszFind );
/*  Sélection de critères : par liens */
/*  ---------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_ClassifByID ( char * pszIdentifiant, long lClassif );
__declspec(dllimport) char * FAR PASCAL VDL_ClassifByLib ( char * pszLibelle, long lClassif );
__declspec(dllimport) char * FAR PASCAL VDL_SousClasse ( char * pszNiveau, long lQualif );
__declspec(dllimport) char * FAR PASCAL VDL_GIToIndics ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL VDL_IndicToGIs ( char * pszODSin );
__declspec(dllimport) char * FAR PASCAL VDL_GIToGIs ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL VDL_CIMToGIs ( char * pszIdCIM );
__declspec(dllimport) char * FAR PASCAL VDL_GIToCIMs ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL VDL_CIMToCIs ( char * pszIdCIM );
__declspec(dllimport) char * FAR PASCAL VDL_CIToCIMs ( char * pszCIcode );
__declspec(dllimport) char * FAR PASCAL VDL_GetDCIGroupTheras ( char * pszODS, char * pszGroupType );
/*  Propriétés d'un produit : Fiches techniques */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_Desc ( long lTypeChamp, char * pszPointeurs );
__declspec(dllimport) char * FAR PASCAL VDL_Cip ( char * pszListeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_ptr ( char * ListePointeurs );
/*  Propriétés d'un produit : Liens */
/*  ------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_MedToCIs ( char * pszListeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_IamInfo ( char * pszCodeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_MedToSubs ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_MedToExcips ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetCompo ( char * pszCodeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetClasses ( char * pszCodeProduit, long TypeQualifiant );
__declspec(dllimport) char * FAR PASCAL VDL_VoieAdmi ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_MedToIndics ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL VDL_MedToGIs ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetCIPDCIGroup ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetCIPGenGroup ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL VDL_GetCIPGenGroupOrDCIGroup ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL VDL_TherUni ( char * pszCIP );
/*  Monographies */
/*  ------------------- */
__declspec(dllimport) long FAR PASCAL VDL_Doc ( char * pszUnCip, long lUnFondDoc, long lUneRubrique );
/*  Aides à la prescription : Détection d'IAM */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_Iam1 ( long lNiveauGravite, char * pszCodeCIP, char * pszListeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_Iam2 ( long lNiveauGravite, char * pszListeCIP );
/*  Aides à la prescription : Contournement d'IAM */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL VDL_SansIamViaThera ( char * Id_Classe, char * ListeCIP );
__declspec(dllimport) char * FAR PASCAL VDL_SansIamViaIndic ( char * pszODSin, char * ListeCIP );
/*  =============================================== */
/*  BIBLIOTHEQUE COMPATIBLE MEDIAVIDAL */
/*  =============================================== */
/*  Mécanismes techniques : Adaptations d'interface */
/*  ----------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL Hmk_OVPparam ( char * pszFnName, long lParamNr );
__declspec(dllimport) char * FAR PASCAL Hmk_OVPgetTextCode ( char * pszCollection, char * pszCle );
__declspec(dllimport) void FAR PASCAL OVP_ptr2str ( char * PointeurChaine, char * ChaineDest );
__declspec(dllimport) long FAR PASCAL OVP_ptr2str2 ( char * lPointeur, long lOffset, long lTailleBuffer, char * pszBuffer );
__declspec(dllimport) long FAR PASCAL OVP_GetLastError ( );
__declspec(dllimport) char * FAR PASCAL OVP_GetTextError ( long CodeErreur );
/*  Mécanismes techniques : Session de transactions */
/*  ----------------------------------------------- */
__declspec(dllimport) long FAR PASCAL OVP_ini ( char * Chemin );
__declspec(dllimport) char * FAR PASCAL OVP_version ( );
__declspec(dllimport) long FAR PASCAL OVP_nbOfCip ( );
__declspec(dllimport) char * FAR PASCAL OVP_getIntruder ( );
__declspec(dllimport) char * FAR PASCAL OVP_close ( );
/*  Sélection de produits */
/*  --------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_med ( char * Terme, long TypeQualifiant, char * lTher, long TypeIndex, long TypeChamp, long TypeProduit );
__declspec(dllimport) char * FAR PASCAL OVP_gen ( char * unCIP );
__declspec(dllimport) char * FAR PASCAL OVP_iam3 ( char * Id_ClasseIAM );
__declspec(dllimport) char * FAR PASCAL OVP_iam3a ( char * Id_ClasseIAM );
/*  Sélection de critères : par noms */
/*  -------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_qualif ( char * Nom, long TypeQualifiant, long TypeIndex, long TypeProduit );
__declspec(dllimport) char * FAR PASCAL OVP_qualifFull ( long TypeQualifiant );
__declspec(dllimport) char * FAR PASCAL HMK_getGI ( char * pszTermeRech );
/*  Sélection de critères : par liens */
/*  -------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_classifByID ( char * Identifiant, long TypeIdentifiant );
__declspec(dllimport) char * FAR PASCAL OVP_classifByLib ( char * Libelle, long TypeIdentifiant );
__declspec(dllimport) char * FAR PASCAL OVP_sousClasse ( char * Niveau, long TypeQualifiant );
__declspec(dllimport) char * FAR PASCAL HMK_GIToIndic ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL HMK_IndicToGI ( char * pszODSin );
__declspec(dllimport) char * FAR PASCAL HMK_GIToGI ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL HMK_CIMToGI ( char * pszIdCIM );
__declspec(dllimport) char * FAR PASCAL HMK_GIToCIM ( char * pszODSgi );
__declspec(dllimport) char * FAR PASCAL HMK_CIMToCI ( char * pszIdCIM );
__declspec(dllimport) char * FAR PASCAL HMK_CIToCIM ( char * pszCIcode );
/*  Propriétés d'un produit : Fiches techniques */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_desc ( long TypeChamp, char * ListePointeurs );
__declspec(dllimport) char * FAR PASCAL OVP_cip ( char * ListeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_ptr ( char * ListePointeurs );
/*  Propriétés d'un produit : Liens */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_CI ( char * ListeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_iamInfo ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_medSubs ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_medExci ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL HMK_GetClasses ( char * CodeProduit, long TypeQualifiant );
__declspec(dllimport) char * FAR PASCAL OVP_voieAdmi ( char * CodeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_therUni ( char * CodeCIP );
/*  Monographies */
/*  ------------------------------------------------- */
__declspec(dllimport) long FAR PASCAL OVP_docForCip ( char * unCIP, long unFondDoc );
__declspec(dllimport) long FAR PASCAL Hmk_Doc ( char * pszUnCip, long lUnFondDoc, long lUneRubrique );
__declspec(dllimport) char * FAR PASCAL OVP_viewDoc ( long hWnd, char * pszUnCip, long lUnFondDoc, long lUneRubrique, int X, int Y, int CX, int CY );
__declspec(dllimport) char * FAR PASCAL OVP_deleteDoc ( );
__declspec(dllimport) char * FAR PASCAL OVP_resizeDoc ( int CX, int CY );
__declspec(dllimport) char * FAR PASCAL OVP_printDoc ( );
/*  Aides à la prescription : Détection d'IAM */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_iam1 ( long NiveauGravite, char * CodeCIP, char * ListeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_iam2 ( long NiveauGravite, char * ListeCIP );
__declspec(dllimport) char * FAR PASCAL OVP_iam2a ( long NiveauGravite, char * ListeCIP );
/*  Aides à la prescription : Contournement d'IAM */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL OVP_sansIam1 ( char * Id_Classe, char * ListeCIP );
/*  Fonctions expérimentales */
/*  ------------------------------------------------- */
__declspec(dllimport) char * FAR PASCAL HMK_medToIndic ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL HMK_medToGI ( char * pszCIP );
__declspec(dllimport) char * FAR PASCAL OVP_GetCipPoso ( char * pszCIP, char * pszODSindic, long lAge, char * pszUage, long lPoidsGr, char * pszSexe, char * pszTerrain );
#ifdef __cplusplus
	}
#endif
