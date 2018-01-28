
/* ************************************************************************ */
/* Module : HmkMacFn.H          PROJET : Tous projets liés à HMKMAC.DLL     */
/* ************************************************************************ */
/* FONCTIONNALITES                                                          */
/* Déclaration des fonctions d'API HMKMAC.DLL                               */
/* ************************************************************************ */
/* Auteur : (généré par MakeIncude.exe)                                     */
/* Compatible avec HMKMAC.DLL version 1.4.1                                 */
/* ************************************************************************ */


#if !defined(_INC_HMKMACFN_H)
	#define _INC_HMKMACFN_H
#endif
#ifdef __cplusplus
	extern "C" {
#endif

/*  =============================================== */
/*  NOUVELLE BIBLIOTHEQUE VIDAL */
/*  =============================================== */
/*  Mécanismes techniques : Adaptations d'interface */
/*  ----------------------------------------------- */
long pascal VDL_ptr2str2 ( char * lPointeur, long lOffset, long lTailleBuffer, char * pszBuffer );
long pascal VDL_GetLastError ( );
/*  Mécanismes techniques : Session de transactions */
/*  ----------------------------------------------- */
long pascal VDL_Ini ( char * pszChemin );
char * pascal VDL_Version ( );
char * pascal VDL_GetIntruder ( );
char * pascal VDL_Close ( );
/*  Sélection de produits */
/*  --------------------- */
char * pascal VDL_Med ( char * Terme, long TypeQualifiant, char * lTher, long TypeIndex, long TypeChamp, long TypeProduit );
char * pascal VDL_Gen ( char * pszUnCIP );
char * pascal VDL_GetGenGroupCIPs ( char * pszCodeGroupe );
char * pascal VDL_GetDCIGroupCIPs ( char * pszODS );
char * pascal VDL_Iam3 ( char * pszID_Classe );
/*  Sélection de critères : par noms */
/*  -------------------------------- */
char * pascal VDL_Qualif ( char * szRecherche, long TypeQualifiant, long TypeIndex, long TypeProduit );
char * pascal VDL_QualifFull ( long TypeQualifiant );
char * pascal VDL_GetGIs ( char * pszFind );
char * pascal VDL_GetGenGroups ( char * pszFind );
char * pascal VDL_GetDCIGroups ( char * pszFind );
char * pascal VDL_GetGenGroupsAndDCIGroups ( char * pszFind );
/*  Sélection de critères : par liens */
/*  ---------------------------------- */
char * pascal VDL_ClassifByID ( char * pszIdentifiant, long lClassif );
char * pascal VDL_ClassifByLib ( char * pszLibelle, long lClassif );
char * pascal VDL_SousClasse ( char * pszNiveau, long lQualif );
char * pascal VDL_GIToIndics ( char * pszODSgi );
char * pascal VDL_IndicToGIs ( char * pszODSin );
char * pascal VDL_GIToGIs ( char * pszODSgi );
char * pascal VDL_CIMToGIs ( char * pszIdCIM );
char * pascal VDL_GIToCIMs ( char * pszODSgi );
char * pascal VDL_CIMToCIs ( char * pszIdCIM );
char * pascal VDL_CIToCIMs ( char * pszCIcode );
char * pascal VDL_GetDCIGroupTheras ( char * pszODS, char * pszGroupType );
/*  Propriétés d'un produit : Fiches techniques */
/*  ------------------------------------------------- */
char * pascal VDL_Desc ( long lTypeChamp, char * pszPointeurs );
char * pascal VDL_Cip ( char * pszListeCIP );
char * pascal VDL_ptr ( char * ListePointeurs );
/*  Propriétés d'un produit : Liens */
/*  ------------------------------- */
char * pascal VDL_MedToCIs ( char * pszListeCIP );
char * pascal VDL_IamInfo ( char * pszCodeCIP );
char * pascal VDL_MedToSubs ( char * CodeCIP );
char * pascal VDL_MedToExcips ( char * CodeCIP );
char * pascal VDL_GetCompo ( char * pszCodeCIP );
char * pascal VDL_GetClasses ( char * pszCodeProduit, long TypeQualifiant );
char * pascal VDL_VoieAdmi ( char * CodeCIP );
char * pascal VDL_MedToIndics ( char * pszCIP );
char * pascal VDL_MedToGIs ( char * pszCIP );
char * pascal VDL_GetCIPDCIGroup ( char * pszCIP );
char * pascal VDL_GetCIPGenGroup ( char * pszCIP );
char * pascal VDL_GetCIPGenGroupOrDCIGroup ( char * pszCIP );
char * pascal VDL_TherUni ( char * pszCIP );
/*  Monographies */
/*  ------------------- */
long pascal VDL_Doc ( char * pszUnCip, long lUnFondDoc, long lUneRubrique );
/*  Aides à la prescription : Détection d'IAM */
/*  ------------------------------------------------- */
char * pascal VDL_Iam1 ( long lNiveauGravite, char * pszCodeCIP, char * pszListeCIP );
char * pascal VDL_Iam2 ( long lNiveauGravite, char * pszListeCIP );
/*  Aides à la prescription : Contournement d'IAM */
/*  ------------------------------------------------- */
char * pascal VDL_SansIamViaThera ( char * Id_Classe, char * ListeCIP );
char * pascal VDL_SansIamViaIndic ( char * pszODSin, char * ListeCIP );
/*  =============================================== */
/*  BIBLIOTHEQUE COMPATIBLE MEDIAVIDAL */
/*  =============================================== */
/*  Mécanismes techniques : Adaptations d'interface */
/*  ----------------------------------------------- */
char * pascal Hmk_OVPparam ( char * pszFnName, long lParamNr );
char * pascal Hmk_OVPgetTextCode ( char * pszCollection, char * pszCle );
void pascal OVP_ptr2str ( char * PointeurChaine, char * ChaineDest );
long pascal OVP_ptr2str2 ( char * lPointeur, long lOffset, long lTailleBuffer, char * pszBuffer );
long pascal OVP_GetLastError ( );
char * pascal OVP_GetTextError ( long CodeErreur );
/*  Mécanismes techniques : Session de transactions */
/*  ----------------------------------------------- */
long pascal OVP_ini ( char * Chemin );
char * pascal OVP_version ( );
long pascal OVP_nbOfCip ( );
char * pascal OVP_getIntruder ( );
char * pascal OVP_close ( );
/*  Sélection de produits */
/*  --------------------- */
char * pascal OVP_med ( char * Terme, long TypeQualifiant, char * lTher, long TypeIndex, long TypeChamp, long TypeProduit );
char * pascal OVP_gen ( char * unCIP );
char * pascal OVP_iam3 ( char * Id_ClasseIAM );
char * pascal OVP_iam3a ( char * Id_ClasseIAM );
/*  Sélection de critères : par noms */
/*  -------------------------------- */
char * pascal OVP_qualif ( char * Nom, long TypeQualifiant, long TypeIndex, long TypeProduit );
char * pascal OVP_qualifFull ( long TypeQualifiant );
char * pascal HMK_getGI ( char * pszTermeRech );
/*  Sélection de critères : par liens */
/*  -------------------------------- */
char * pascal OVP_classifByID ( char * Identifiant, long TypeIdentifiant );
char * pascal OVP_classifByLib ( char * Libelle, long TypeIdentifiant );
char * pascal OVP_sousClasse ( char * Niveau, long TypeQualifiant );
char * pascal HMK_GIToIndic ( char * pszODSgi );
char * pascal HMK_IndicToGI ( char * pszODSin );
char * pascal HMK_GIToGI ( char * pszODSgi );
char * pascal HMK_CIMToGI ( char * pszIdCIM );
char * pascal HMK_GIToCIM ( char * pszODSgi );
char * pascal HMK_CIMToCI ( char * pszIdCIM );
char * pascal HMK_CIToCIM ( char * pszCIcode );
/*  Propriétés d'un produit : Fiches techniques */
/*  ------------------------------------------------- */
char * pascal OVP_desc ( long TypeChamp, char * ListePointeurs );
char * pascal OVP_cip ( char * ListeCIP );
char * pascal OVP_ptr ( char * ListePointeurs );
/*  Propriétés d'un produit : Liens */
/*  ------------------------------------------------- */
char * pascal OVP_CI ( char * ListeCIP );
char * pascal OVP_iamInfo ( char * CodeCIP );
char * pascal OVP_medSubs ( char * CodeCIP );
char * pascal OVP_medExci ( char * CodeCIP );
char * pascal HMK_GetClasses ( char * CodeProduit, long TypeQualifiant );
char * pascal OVP_voieAdmi ( char * CodeCIP );
char * pascal OVP_therUni ( char * CodeCIP );
/*  Monographies */
/*  ------------------------------------------------- */
long pascal OVP_docForCip ( char * unCIP, long unFondDoc );
long pascal Hmk_Doc ( char * pszUnCip, long lUnFondDoc, long lUneRubrique );
char * pascal OVP_viewDoc ( long hWnd, char * pszUnCip, long lUnFondDoc, long lUneRubrique, int X, int Y, int CX, int CY );
char * pascal OVP_deleteDoc ( );
char * pascal OVP_resizeDoc ( int CX, int CY );
char * pascal OVP_printDoc ( );
/*  Aides à la prescription : Détection d'IAM */
/*  ------------------------------------------------- */
char * pascal OVP_iam1 ( long NiveauGravite, char * CodeCIP, char * ListeCIP );
char * pascal OVP_iam2 ( long NiveauGravite, char * ListeCIP );
char * pascal OVP_iam2a ( long NiveauGravite, char * ListeCIP );
/*  Aides à la prescription : Contournement d'IAM */
/*  ------------------------------------------------- */
char * pascal OVP_sansIam1 ( char * Id_Classe, char * ListeCIP );
/*  Fonctions expérimentales */
/*  ------------------------------------------------- */
char * pascal HMK_medToIndic ( char * pszCIP );
char * pascal HMK_medToGI ( char * pszCIP );
char * pascal OVP_GetCipPoso ( char * pszCIP, char * pszODSindic, long lAge, char * pszUage, long lPoidsGr, char * pszSexe, char * pszTerrain );
#ifdef __cplusplus
	}
#endif
