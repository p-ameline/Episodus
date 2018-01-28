// -----------------------------------------------------------------------------
// nsperson.h
// -----------------------------------------------------------------------------
// $Revision: 1.18 $
// $Author: pameline $
// $Date: 2015/12/14 08:22:45 $
// -----------------------------------------------------------------------------
// Contient les définitions des objets PATIENT et UTILISATEUR de NAUTILUS
// -----------------------------------------------------------------------------
// PK  - 01/1992
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2001-2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à [rappeler les
// caractéristiques techniques de votre logiciel].
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to [describe
// functionalities and technical features of your software].
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#ifndef __NSPERSON_H
# define __NSPERSON_H

#if defined(_DANSLEXIDLL)
  #define _CLASSELEXI __export
#else
	#define _CLASSELEXI __import
#endif

class NSPatient ;
class NSAdressesInfo ;
class NSCorrespArray ;
class NSPersonGraphManager ;
class NSAddressGraphManager ;
class NSPatPathoArray ;
class NSBasicAttributeArray ;
class NSHISTODocument ;
class NSHealthTeam ;
class NSHealthTeamDocument ;
class NSDocumentHisto ;
class NSSearchStruct ;
class NSFrameDocumentsArray ;

# include <cstring.h>
# include <owl\window.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage/nsglobal.h"
#endif

# include "partage\ns_vector.h"
# include "nsbb\nsmpids.h"
# include "nsbb\nsmanager.h"
# include "nsbb\nspatpat.h"
# include "nautilus\nsanxary.h"

//
// Taille des champs du fichier PATIENT
//
# define PAT_NSS_LEN         PIDS_NSS_LEN
# define PAT_NOM_LEN         PIDS_NOM_LEN
# define PAT_PRENOM_LEN      PIDS_PRENOM_LEN
# define PAT_CODE_LEN        PIDS_CODE_LEN
# define PAT_SEXE_LEN	       PIDS_SEXE_LEN
# define PAT_NAISSANCE_LEN   PIDS_NAISSANCE_LEN

# define PAT_CONVOC_LEN       20
# define PAT_ADRESSE_LEN       5
# define PAT_TELEPOR_LEN      15
# define PAT_TELEBUR_LEN      15
# define PAT_SITFAM_LEN        2
# define PAT_LANG_LEN          2

# define PAT_NOM_LONG_LEN     80

// -----------------------------------------------------------------------------
// Taille des champs du fichier UTILISAT
// -----------------------------------------------------------------------------
# define UTI_INDICE_LEN       3
# define UTI_NSS_LEN         PIDS_NSS_LEN
# define UTI_LOGIN_LEN       35
# define UTI_CODE_LEN        35
# define UTI_SEXE_LEN	       1
# define UTI_NOM_LEN         35
# define UTI_PRENOM_LEN      35
# define UTI_TYPE_LEN         1
# define UTI_MESSAGERIE_LEN  70
# define UTI_LANG_LEN         2
# define UTI_METIER_LEN     255
# define UTI_SPEC_LEN       255


// -----------------------------------------------------------------------------
// Indice des champs du fichier UTILISAT
// -----------------------------------------------------------------------------
# define UTI_INDICE_FIELD       1
# define UTI_NSS_FIELD          2
# define UTI_LOGIN_FIELD        3
# define UTI_CODE_FIELD         4
# define UTI_SEXE_FIELD         5
# define UTI_NOM_FIELD          6
# define UTI_PRENOM_FIELD       7
# define UTI_TYPE_FIELD         8
# define UTI_MESSAGERIE_FIELD   9
# define UTI_LANG_FIELD        10

// -----------------------------------------------------------------------------
// Taille des champs du fichier CORRESP
// -----------------------------------------------------------------------------
# define COR_NSS_LEN           PIDS_NSS_LEN
# define COR_CODE_LEN            3
# define COR_NOM_LEN            35
# define COR_PRENOM_LEN         35
# define COR_SEXE_LEN		         1
# define COR_ADRESSE_LEN         5
# define COR_DOCTEUR_LEN         1
# define COR_TELEPOR_LEN        15
# define COR_TELEBUR_LEN	      15
# define COR_NB_EXEMP_LEN        2
# define COR_MESSAGERIE_LEN     50
# define COR_FONCTION_LEN 	     6
# define COR_ACTIF_LEN 	 	       1
# define COR_LANG_LEN 	 	       2


// -----------------------------------------------------------------------------
// Indice des champs du fichier CORRESP
// -----------------------------------------------------------------------------
# define COR_CODE_FIELD          1
# define COR_NOM_FIELD           2
# define COR_PRENOM_FIELD        3
# define COR_SEXE_FIELD		       4
# define COR_ADRESSE_FIELD       5
# define COR_DOCTEUR_FIELD       6
# define COR_TELEPOR_FIELD       7
# define COR_TELEBUR_FIELD	     8
# define COR_NB_EXEMP_FIELD      9
# define COR_MESSAGERIE_FIELD   10
# define COR_FONCTION_FIELD     11
# define COR_ACTIF_FIELD   	    12
# define COR_LANG_FIELD   	    13


// -----------------------------------------------------------------------------
//
// Classe NSPatient
//
// -----------------------------------------------------------------------------
// Objet contenant les données
// -----------------------------------------------------------------------------
class _CLASSELEXI NSPatientData : public NSRoot
{
  public:

    // Variables de stockage des valeurs
    string _sNss ;
    string _sNom ;
    string _sPrenom ;
    string _sCode ;
    string _sNomJeuneFille ;
    string _sIpp ;
    string _sMergedWith ;
    string _sConvoc ;
    string _sSexe ;
    string _sAdresse ;
    string _sNaissance ;
    string _sTelepor ;
    string _sTelebur ;
    string _sSitfam ;
    string _sLang ;
    string _sCivilite ;
    string _sNomLong ;
    string _sLockedConsole ;
    string _sPatientType ;

    NSPatientData(NSContexte* pCtx) ;
    NSPatientData(const NSPatientData& rv) ;

    NSPatientData& operator=(const NSPatientData& src);
    int            operator==(const NSPatientData& o);

    void           init() ;
    void           initFromPatientData(const NSPatientData *pOther) ;

    void    fabriqueNomLong(string sLang = "") ;
    string  fabriqueCivilite(bool bShort, string sLang = "") const ;
    // int     donneNaissance(char *dateNaiss) ;
    string  donneNaissance() const ;

    // Indique si le patient est local
    inline bool IsLocal() const { return (LocalPatient == _sNss[0]) ; }

    // Indique si le patient est en mémoire
    inline bool IsInMemory() const { return (INMEMORY_CHAR == _sNss[0]) ; }

    // Indique si le patient est collectif
    inline bool IsGlobal() const { return ((false == IsInMemory()) && (false == IsLocal())) ; }

    bool    estMasculin() const { return ('1' == _sSexe[0]) ; }
    bool 		estFeminin()  const { return ('2' == _sSexe[0]) ; }
    void 		metMasculin() { _sSexe = string("1") ; }
    void 		metFeminin()  { _sSexe = string("2") ; }
    bool    haveIpp()     const { return (string("") != _sIpp) ; }
    bool		isMerged()    const { return (string("") != _sMergedWith) ; }

    // _sPatientType : 1 (always) + 2 (when dead) + 4 (when deleted)
    bool    isRegular()  const { return (string("1") == _sPatientType) ; }
    bool    isDead()     const { return ((string("3") == _sPatientType) || (string("7") == _sPatientType)) ; }
    bool    isTrashed()  const { return ((string("5") == _sPatientType) || (string("7") == _sPatientType)) ; }

    void    setDead(bool bDead) ;
    void    setTrashed(bool bDead) ;

    // ajoute le nb d'enfants à sitfam
    void    setNbEnfants(int nbenf) ;
    // retourne le nb d'enfants d'après sitfam
    int  		getNbEnfants() const ;

    // données "traduites" en français
    string  donneSitFam(string sLang = "") const ;
    string 	donneNbEnfants() const ;
    string 	donneDateNaiss(string sLang = "") const ;
    string 	donneTitre(string sLang = "") ;
} ;

// -----------------------------------------------------------------------------
//
//  Classe NSPatInfo
//
// -----------------------------------------------------------------------------
// destinée à être stockée dans une Array
// -----------------------------------------------------------------------------
class _CLASSELEXI NSPatInfo : public NSRoot
{
  private:

 	  static long lObjectCount ;

  protected:

    // Data repository
    //
    NSPatientData          _Donnees ;

    string                 _sContribution ;

    NSPersonGraphManager   _GraphPerson ;
    NSFrameDocumentsArray* _pFramesDocuments ;

  public:

    NSHISTODocument*       _pDocHis ;

    NSAddressGraphManager  _GraphAdr ;
    string                 _sChez ;
    NSPidsArray            _CorrespArray ;

    NSHealthTeam*          _pHealthTeam ;

    NSPatInfo(NSContexte* pCtx) ;
    NSPatInfo(NSPids *) ;

    NSPatInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx) ;

    NSPatInfo(const NSPatInfo& rv) ;
    virtual ~NSPatInfo() ;

    bool    open(const string sNss, bool bLock) ;
    bool    getGraph(const string sNss, bool bLock) ;

    bool    initFromPersonInfo(NSPersonInfo *pPersInfo) ;

    bool    initGraphs() ;
    bool    initCorrespPIDS(NSPidsInfo* pPidsInfo, string sPersonID) ;

    void    ChargeDonneesPatient(NSPatPathoArray *pPatPathoArray) ;
    void    initIPP() ;

    string  getAdminData(NSPatPathoArray* pPpt) ;
    string  getSingletonTree(NSRootLink::NODELINKTYPES iType, NSPatPathoArray* pPpt) ;

    void    debloquer() ;

    bool    getReadOnly() ;
    bool    getADebloquer() ;
    void    setReadOnly(bool bRO) ;
    void    setADebloquer(bool bAD) ;

    NSDocumentHisto* GetDocument(string sRootSens) ;
    NSDocumentHisto* GetSynthesisDocument() { return GetDocument(string("ZSYNT")) ; }
    NSDocumentHisto* GetIndexDocument()     { return GetDocument(string("ZPOMR")) ; }

    void    GetDocPatho(NSPatPathoArray *pPatho, string sRootSens) ;
    void    DonnePathoSynthese(NSPatPathoArray *pPathoSynthese) { GetDocPatho(pPathoSynthese, string("ZSYNT")) ; } /* Fills a patpaho with synthese */
    void    DonnePathoIndex(NSPatPathoArray *pPathoIndex)       { GetDocPatho(pPathoIndex, string("ZPOMR")) ; }    /* Fills a patpaho with index */

    bool    ChercheChemin(string sChemin, NSSearchStruct *pSearchStruct, bool reinit_structure = false) ;
    bool    SearchInSynthesis(string sChemin, NSSearchStruct *pSearchStruct) ;
    bool    SearchInIndexes(string sChemin, NSSearchStruct *pSearchStruct) ;
    bool    SearchInIndexExtensions(string sChemin, vector<NSDocumentHisto*> *pConcernDocs, NSSearchStruct *pSearchStruct) ;
    bool    isValidRegularNode(NSPatPathoArray *pPPT, PatPathoConstIter pptIter, NSSearchStruct *pSearchStruct, string sDateDoc, string &sRefDate) ;
    bool    isValidIndexNode(NSPatPathoArray *pPPT, PatPathoIter pptIter, NSSearchStruct *pSearchStruct, string &sRefDate) ;
    bool    SearchInDocument(string sChemin, NSSearchStruct *pSearchStruct, NSDocumentHisto* pDocument) ;
    bool    IsDocumentAFrameIndex(NSDocumentHisto* pDocument) ;
    bool    IsDocumentASynthesis(NSDocumentHisto* pDocument) ;

    string  getQueryPathModifiers(string& sQueryPath) ;

    bool    DonneArray(string sNoeud, NSPatPathoArray *pPPT) ;
    bool    DonneSousArray(string sNoeud, NSPatPathoArray *pPPT) ;

    bool    initCorrespArray(NSPatPathoArray* pPatPathoArray = 0) ;

    bool    getFoldersPatho(NSPatPathoArray* pPatPathoArray) ;
    bool    getFoldersArray(NSChemiseArray* pChemisesArray) ;

    bool    CreeContribution(bool bInitiale = false) ;
    bool    FermeContribution() ;
    string  getContribution()                { return _sContribution ; }
    void    setContribution(string sContrib) { _sContribution = sContrib ; }

    void 		fabriqueNomLong(string sLang = string("")) ;
    string 	fabriqueCivilite(bool bShort, string sLang = string(""))  { return (_Donnees.fabriqueCivilite(bShort, sLang)) ; }

    string 	fabriqueAgeLabel(string sLang = string("")) ;
    string 	fabriqueAgeLabelYears(string sLang = string("")) ;
    string 	fabriqueAgeLabelMonths(string sLang = string("")) ;
    string 	fabriqueAgeLabelDays(string sLang = string("")) ;
    string 	fabriqueAgeLabelHours(string sLang = string("")) ;

          NSPersonGraphManager* getGraphPerson()            { return &_GraphPerson ; }
    const NSPersonGraphManager* getConstGraphPerson() const { return &_GraphPerson ; }
          NSHISTODocument*      getDocHis()                 { return _pDocHis ; }
    const NSHISTODocument*      getConstDocHis() const      { return _pDocHis ; }
    NSFrameDocumentsArray* getFramesDocuments() { return _pFramesDocuments ; }
    NSHealthTeam*          getHealthTeam()      { return _pHealthTeam ; }

    // int			donneNaissance(char *dateNaiss)	    { return _Donnees.donneNaissance(dateNaiss) ; }
    string	donneNaissance()	    const          { return _Donnees.donneNaissance() ; }
    bool		estMasculin() 				const          { return _Donnees.estMasculin() ; }
    bool		estFeminin()  				const          { return _Donnees.estFeminin() ; }
    string	donneSitFam(string sLang = "")    const { return _Donnees.donneSitFam(sLang) ; }
    string	donneNbEnfants() 		              const { return _Donnees.donneNbEnfants() ; }
    string	donneDateNaiss(string sLang = "") const { return _Donnees.donneDateNaiss(sLang) ; }
    string	donneTitre(string sLang = "")        { return _Donnees.donneTitre(sLang) ; }

    string  donneLang()      			const         { return _Donnees._sLang ; }
    string  getNss()              const         { return _Donnees._sNss ; }
    string  getNom()              const         { return _Donnees._sNom ; }
    string  getPrenom()           const         { return _Donnees._sPrenom ; }
    string  getCode()             const         { return _Donnees._sCode ; }
    string  getConvoc()           const         { return _Donnees._sConvoc ; }
    string  getSexe()             const         { return _Donnees._sSexe ; }
    string  getAdresse()          const         { return _Donnees._sAdresse ; }
    string  getNaissance()        const         { return _Donnees._sNaissance ; }
    string  getTelepor()          const         { return _Donnees._sTelepor ; }
    string  getTelebur()          const         { return _Donnees._sTelebur ; }
    string  getSitfam()           const         { return _Donnees._sSitfam ; }
    string	getCivilite()					const         { return _Donnees._sCivilite ; }
    string  getNomLong()          const         { return _Donnees._sNomLong ; }
    string  getIpp()              const         { return _Donnees._sIpp ; }
    bool    haveIpp()             const         { return _Donnees.haveIpp() ; }
    string	getMergedWith()				const         { return _Donnees._sMergedWith ; }
    bool		isMerged()						const         { return _Donnees.isMerged() ; }
    string	getNomJeuneFille()		const         { return _Donnees._sNomJeuneFille ; }
    string	getLockingConsoleId()	const	 			  { return _Donnees._sLockedConsole ; }
    string  getIPPEnCours(string sSite, string* psOuvre = 0, string* psFerme = 0) ;

    const char *  getszNss()                    { return (_Donnees._sNss).c_str() ; }
    const char *  getszNom()                    { return (_Donnees._sNom).c_str() ; }
    const char *  getszPrenom()                 { return (_Donnees._sPrenom).c_str() ; }
    const char *  getszCode()                   { return (_Donnees._sCode).c_str() ; }
    const char *  getszConvoc()                 { return (_Donnees._sConvoc).c_str() ; }
    const char *  getszSexe()                   { return (_Donnees._sSexe).c_str() ; }
    const char *  getszAdresse()                { return (_Donnees._sAdresse).c_str() ; }
    const char *  getszNaissance()              { return (_Donnees._sNaissance).c_str() ; }
    const char *  getszTelepor()                { return (_Donnees._sTelepor).c_str() ; }
    const char *  getszTelebur()                { return (_Donnees._sTelebur).c_str() ; }
    const char *  getszSitfam()                 { return (_Donnees._sSitfam).c_str() ; }
    const char *	getszCivilite()								{ return (_Donnees._sCivilite).c_str() ; }
    const char *  getszNomLong()                { return (_Donnees._sNomLong).c_str() ; }
    const char *  getszIpp()                    { return (_Donnees._sIpp).c_str() ; }
    const char *  getszNomJeuneFille()					{ return (_Donnees._sNomJeuneFille).c_str() ; }

    // Indique si le patient est local
    inline bool IsLocal()    { return _Donnees.IsLocal() ; }
    // Indique si le patient est en mémoire
    inline bool IsInMemory() { return _Donnees.IsInMemory() ; }
    // Indique si le patient est collectif
    inline bool IsGlobal()   { return _Donnees.IsGlobal() ; }

    void setNss(string sVal)        { _Donnees._sNss           = sVal ; }
    void setNom(string sVal)        { _Donnees._sNom           = sVal ; }
    void setPrenom(string sVal)     { _Donnees._sPrenom        = sVal ; }
    void setCode(string sVal)       { _Donnees._sCode          = sVal ; }
    void setNomJF(string sVal)      { _Donnees._sNomJeuneFille = sVal ; }
    void setIpp(string sVal)        { _Donnees._sIpp           = sVal ; }
    void setMergedWith(string sVal) { _Donnees._sMergedWith    = sVal ; }
    void setConvoc(string sVal)     { _Donnees._sConvoc        = sVal ; }
    void setSexe(string sVal)       { _Donnees._sSexe          = sVal ; }
    void setAdresse(string sVal)    { _Donnees._sAdresse       = sVal ; }
    void setNaissance(string sVal)  { _Donnees._sNaissance     = sVal ; }
    void setTelePor(string sVal)    { _Donnees._sTelepor       = sVal ; }
    void setTeleBur(string sVal)    { _Donnees._sTelebur       = sVal ; }
    void setSitfam(string sVal)     { _Donnees._sSitfam        = sVal ; }
    void setLang(string sVal)       { _Donnees._sLang          = sVal ; }
    void setCivilite(string sVal)   { _Donnees._sCivilite      = sVal ; }
    void setNomLong(string sVal)    { _Donnees._sNomLong       = sVal ; }
    void setLockedCons(string sVal) { _Donnees._sLockedConsole = sVal ; }

    NSPatInfo& operator=(const NSPatInfo& src) ;
    int        operator==(const NSPatInfo& o) ;

    NSPatientData* getData()                  { return &_Donnees ; }
    const NSPatientData* getConstData() const { return &_Donnees ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

// -----------------------------------------------------------------------------
//
// Classe NSUtilisateur
//
// -----------------------------------------------------------------------------
string StringTitre(NSContexte* pContexte, string sTypeTitre, string sLang = "fr") ;
string StringAvantTitre(string sTypeTitre, string sLang = "fr") ;

class _CLASSELEXI NSUtilisateurData
{
	public:

		// Variables de stockage des valeurs
/*
  char nss[UTI_NSS_LEN + 1] ;
  char login[UTI_LOGIN_LEN + 1] ;
  char code[UTI_CODE_LEN + 1] ;
  char sexe[UTI_SEXE_LEN + 1] ;
  char nom[UTI_NOM_LEN + 1] ;
  char prenom[UTI_PRENOM_LEN + 1] ;
  char type[UTI_TYPE_LEN + 1] ;
  char messagerie[UTI_MESSAGERIE_LEN + 1];
  char lang[UTI_LANG_LEN + 1] ;
*/
		string _sNss ;
		string _sLogin ;
		string _sCode ;
		string _sSexe ;
		string _sNom ;
		string _sPrenom ;
		string _sType ;
		string _sMessagerie ;
		string _sLang ;
		string _sMetier ;
  	string _sSpecialite ;
  	string _sVille ;
    string _sProfessionalId ;
    string _sRpps ;

		// Indique si l'utilisateur est local
		inline bool IsLocal()    { return (_sNss[0] == LocalPatient) ; }
  	// Indique si l'utilisateur est en mémoire
  	inline bool IsInMemory() { return (_sNss[0] == INMEMORY_CHAR) ; }
  	// Indique si l'utilisateur est collectif
  	inline bool IsGlobal()   { return (!IsInMemory() && !IsLocal()) ; }

		NSUtilisateurData() ;
		NSUtilisateurData(const NSUtilisateurData& rv) ;

		NSUtilisateurData& operator=(const NSUtilisateurData& src) ;
		int                operator==(const NSUtilisateurData& o) ;

		void	 metAZero() ;
		string donneTitre(NSContexte* pContexte, string sLang = "fr") ;
		string donneAvantTitre(string sLang = "fr") ;
		string donneSignature(NSContexte* pContexte, string sLang = "fr") ;

    bool   estMasculin() { return (string("HMASC") == _sSexe) ; }
		bool	 estFeminin()  { return (string("HFEMI") == _sSexe) ; }
		void	 metMasculin() { _sSexe = string("HMASC") ; }
		void	 metFeminin()  { _sSexe = string("HFEMI") ; }
} ;

class _CLASSELEXI NSCorrespondantInfo : public NSRoot
{
  private:

 	  static long lObjectCount ;

  protected:

	  NSUtilisateurData _Donnees ;

  public:

    NSPersonGraphManager _GraphPerson ;

    bool    GetDataFromGraph(NSPatPathoArray* pPPT = 0) ;

    NSCorrespondantInfo(NSContexte* pCtx) ;
    NSCorrespondantInfo(const NSCorrespondantInfo& rv) ;
    virtual ~NSCorrespondantInfo() ;

    bool    initAdresseInfo() ;

    // Indique si le patient est local
    inline bool IsLocal()          { return (_Donnees.IsLocal()) ; }
    // Indique si le patient est en mémoire
    inline bool IsInMemory()       { return (_Donnees.IsInMemory()) ; }
    // Indique si le patient est collectif
    inline bool IsGlobal()         { return (_Donnees.IsGlobal()) ; }

    string donneLang()             { return _Donnees._sLang ; }
    string getID()                 { return _Donnees._sNss ; }

    string getNss()                { return _Donnees._sNss ; }
    string getOldLogin()           { return _Donnees._sLogin ; }
    string getCode()               { return _Donnees._sCode ; }
    string getSexe()               { return _Donnees._sSexe ; }
    string getNom()                { return _Donnees._sNom ; }
    string getPrenom()             { return _Donnees._sPrenom ; }
    string getType()               { return _Donnees._sType ; }
    string getMail()               { return _Donnees._sMessagerie ; }
    string getLang()               { return _Donnees._sLang ; }
    string getJob()                { return _Donnees._sMetier ; }
    string getSpecialty()          { return _Donnees._sSpecialite ; }
    string getVille()              { return _Donnees._sVille ; }
    string getProId()              { return _Donnees._sProfessionalId ; }
    string getRpps()               { return _Donnees._sRpps ; }

    void setNss(string sVal)       { _Donnees._sNss = sVal ; }
    void setOldLogin(string sVal)  { _Donnees._sLogin = sVal ; }
    void setCode(string sVal)      { _Donnees._sCode = sVal ; }
    void setSexe(string sVal)      { _Donnees._sSexe = sVal ; }
    void setNom(string sVal)       { _Donnees._sNom = sVal ; }
    void setPrenom(string sVal)    { _Donnees._sPrenom = sVal ; }
    void setType(string sVal)      { _Donnees._sType = sVal ; }
    void setMail(string sVal)      { _Donnees._sMessagerie = sVal ; }
    void setLang(string sVal)      { _Donnees._sLang = sVal ; }
    void setJob(string sVal)       { _Donnees._sMetier = sVal ; }
    void setSpecialty(string sVal) { _Donnees._sSpecialite = sVal ; }
    void setVille(string sVal)     { _Donnees._sVille = sVal ; }
    void setProId(string sVal)     { _Donnees._sProfessionalId = sVal ; }
    void setRpps(string sVal)      { _Donnees._sRpps = sVal ; }

    NSUtilisateurData*    getData()  { return &_Donnees ; }
    NSPersonGraphManager* getGraph() { return &_GraphPerson ; }

    bool   estMasculin()           { return _Donnees.estMasculin() ; }
    bool	 estFeminin()            { return _Donnees.estFeminin() ; }
    void	 metMasculin()           { _Donnees.metMasculin() ; }
    void	 metFeminin()            { _Donnees.metFeminin() ; }

    void	 metAZero()              { _Donnees.metAZero() ; }

    string donneTitre(NSContexte* pContexte, string sLang = "fr")     { return _Donnees.donneTitre(pContexte, sLang) ; }
    string donneAvantTitre(string sLang = "fr")                       { return _Donnees.donneAvantTitre(sLang) ; }
    string donneSignature(NSContexte* pContexte, string sLang = "fr") { return _Donnees.donneSignature(pContexte, sLang) ; }

    NSCorrespondantInfo& operator=(const NSCorrespondantInfo& src) ;
    int                  operator==(const NSCorrespondantInfo& o) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
} ;

class NSUtilisateurChoisi ;

// -----------------------------------------------------------------------------
//
// Classe NSUtiliInfo
//
// -----------------------------------------------------------------------------
class _CLASSELEXI NSUtiliInfo : public NSCorrespondantInfo
{
  protected:

     // titre et civilite
    string  _sTitre ;
    string  _sCivilProf ;
    string  _sCivil ;

    // mot de passe
    string  _sLogin ;
    string  _sPasswd ;
    string  _sUserType ;
    string  _sPasswordType ;
    string  _sDatePasswordCreation ;
    string  _sValidityDuration ;

  public:

    // Objet qui contient les données
    NSAdressesInfo*	_pAdresseInfo ;

    NSUtiliInfo(NSContexte* pCtx);

    NSUtiliInfo(NSBasicAttributeArray* pAttribute, NSContexte* pCtx) ;

    NSUtiliInfo(const NSUtiliInfo& rv) ;
    NSUtiliInfo(NSUtilisateurChoisi*) ;
    virtual ~NSUtiliInfo() ;

    bool    initAdresseInfo() ;

    bool    isAdmin() ;
    bool    isUser() ;
    bool    pwdNeedChange() ;
    bool    changePassword() ;

    NSUtiliInfo& operator=(const NSUtiliInfo& src) ;
    int          operator==(const NSUtiliInfo& o) ;

    string getTitre()     { return _sTitre ; }
    string getCivilProf() { return _sCivilProf ; }
    string getCivil()     { return _sCivil ; }
    string getLogin()     { return _sLogin ; }
    string getPasswd()    { return _sPasswd ; }
    string getUserType()  { return _sUserType ; }
    string getPassType()  { return _sPasswordType ; }
    string getPassDate()  { return _sDatePasswordCreation ; }
    string getValidity()  { return _sValidityDuration ; }

    void setTitre(string sVal)     { _sTitre = sVal ; }
    void setCivilProf(string sVal) { _sCivilProf = sVal ; }
    void setCivil(string sVal)     { _sCivil = sVal ; }
    void setLogin(string sVal)     { _sLogin = sVal ; }
    void setPasswd(string sVal)    { _sPasswd = sVal ; }
    void setUserType(string sVal)  { _sUserType = sVal ; }
    void setPassType(string sVal)  { _sPasswordType = sVal ; }
    void setPassDate(string sVal)  { _sDatePasswordCreation = sVal ; }
    void setValidity(string sVal)  { _sValidityDuration = sVal ; }

    void	 metAZero()    { _Donnees.metAZero() ; }
} ;

// -----------------------------------------------------------------------------
// En N_TIERS le nss est retrouvé à partir du graphe
// -----------------------------------------------------------------------------
bool
NSUtiliInfo::isAdmin()
{
  size_t pos = _sUserType.find("A") ;
  if (string::npos != pos)
    return true ;
  return false ;
}

bool
NSUtiliInfo::isUser()
{
  size_t pos = _sUserType.find("U") ;
  if (string::npos != pos)
    return true ;
  return false ;
}

// -----------------------------------------------------------------------------
//
// Classe NSCorrespondant
//
// -----------------------------------------------------------------------------

/*

class NSCorrespondantData
{
 public:

	// Variables de stockage des valeurs
  char nss[COR_NSS_LEN + 1] ;
	// Nota : le code corresp correspond à un id de professionnel
	char code[COR_CODE_LEN + 1] ;
	char nom[COR_NOM_LEN + 1] ;
	char prenom[COR_PRENOM_LEN + 1] ;
	char sexe[COR_SEXE_LEN + 1] ;
	char adresse[COR_ADRESSE_LEN + 1] ;
	char docteur[COR_DOCTEUR_LEN + 1] ;
	char telepor[COR_TELEPOR_LEN + 1] ;
	char telebur[COR_TELEBUR_LEN + 1] ;
	char nb_exemp[COR_NB_EXEMP_LEN + 1] ;
	char messagerie[COR_MESSAGERIE_LEN + 1] ;
	char fonction[COR_FONCTION_LEN + 1] ;
	char actif[COR_ACTIF_LEN + 1] ;
	char lang[COR_LANG_LEN + 1] ;

  string _sNss ;
  string _sCode ;
  string _sNom ;
  string _sPrenom ;
  string _sSexe ;
  string _sAdress ;
  string _sDocteur ;
  string _sTelePor ;
  string _sTeleBur ;
  string _sNbExemp ;
  string _sMessagerie ;
  string _sFonction ;
  string _sActif ;
 	string _sLang ;

	NSCorrespondantData() ;
	NSCorrespondantData(NSCorrespondantData& rv) ;

	NSCorrespondantData& operator=(NSCorrespondantData src) ;
	int                  operator==(NSCorrespondantData& o) ;

	void   metAZero() ;
	string donneTitre(string sLang = "fr") ;
	string donneAvantTitre(string sLang = "fr") ;

	bool   estMasculin() { return (string("HMASC") == _sSexe) ; }
	bool	 estFeminin()  { return (string("HFEMI") == _sSexe) ; }
	void	 metMasculin() { _sSexe = string("HMASC") ; }
	void	 metFeminin()  { _sSexe = string("HFEMI") ; }

	bool	 estActif()    { return (string("1") == _sActif) ; }
	void	 metActif()    { _sActif = string("1") ; }
	void	 metInactif()  { _sActif = string("0") ; }
} ;

*/

string
NSPatientData::donneNaissance() const
{
  // La date de naissance doit être constituée d'exactement PAT_NAISSANCE_LEN digits
  // Birthdate must have exactly PAT_NAISSANCE_LEN digits
  int i ;
  for (i = 0 ; (i < PAT_NAISSANCE_LEN) && (_sNaissance[i] >= '0') && (_sNaissance[i] <= '9') ; i++)
    ;

  if (i < PAT_NAISSANCE_LEN)
    return string("00000000") ;

  if (string("19000000") == _sNaissance)
    return string("00000000") ;
  else
    return _sNaissance ;
}

// -----------------------------------------------------------------------------
// Function:    NSPatientData::donneNaissance(char *dateNaiss)
// Description: Retourne dans dateNaiss la date de naissance du patient
//  					  au format AAAAMMJJ
//	  				  Si le champ existe mais n'a pas été renseigné,
//		  			  retourne "00000000"
// Returns:     1 si OK, 0 si date de naissance n'est pas un champ du fichier
// -----------------------------------------------------------------------------
/*
int
NSPatientData::donneNaissance(char *dateNaiss)
{
  // La date de naissance doit être constituée d'exactement PAT_NAISSANCE_LEN digits
  // Birthdate must have exactly PAT_NAISSANCE_LEN digits
  int i ;
  for (i = 0 ; (i < PAT_NAISSANCE_LEN) && (_sNaissance[i] >= '0') && (_sNaissance[i] <= '9') ; i++)
    ;

  if (i < PAT_NAISSANCE_LEN)
  {
    strcpy(dateNaiss, "00000000") ;
    return 1 ;
  }

  if (string("19000000") == _sNaissance)
    strcpy(dateNaiss, "00000000") ;
  else
    strcpy(dateNaiss, _sNaissance.c_str()) ;

  return 1 ;
}
*/

#endif // !__NSPERSON_H

