//---------------------------------------------------------------------------
//     NSSUPER.H
//
//  Contient les définitions du Superviseur
//
//
//  PA Août 1993
//---------------------------------------------------------------------------

#ifndef __NSSUPER_H__
# define __NSSUPER_H__

#ifndef _ENTERPRISE_DLL

class NSTypeMimeInfo ;

class NSFilGuide ;
class NSContexte ;
class NSNoyauDocument ;      // pour OuvreDocument new version
class NSEpisodus ;
class NSPredi ;
class NSGraphe ;
class NSArcManager ;
class skinArray ;
class NSMatrox ;
class NSWebServiceWindow ;
class NSWebServiceChild ;
class NSMailServiceWindow ;
class NSMailServiceChild ;
class NSWebTipWindow ;
class NSLocalChapterArray ;
class TMyApp ;
class	BB1BBInterface ;
class BB1BBInterfaceForKs ;
class NSDico ;
class NSPersonArray ;
class NSCutPaste ;
class NSPatPathoData ;
class NSPatientChoisi ;
class NSUtilisateurChoisi ;
class NSDocumentInfo ;
class NSPersonGraphManager ;
class NSBasicAttributeArray ;
class NautilusPilot ;
class NSPilotProxyWindow ;
class NSInPersonsWindow ;
class NSHtmlToPdfConverter ;
//class	BB1BBInterface ;

//FIXME
class BB1BB ;
class NSToDoTask ;

# include <owl/applicat.h>
# include <cstring.h>
# include <owl/imagelst.h>
# include <dos.h>
# include <time.h>
# include <stdio.h>

# include <owl/docview.rh>
# include <owl/window.rh>
# include <owl/editsear.rh>
# include <owl/editfile.rh>
# include <owl/mdi.rh>

# include "nautilus\nautilus.h"
# include "nautilus\nautilus.rh"
# include "nautilus\nsldvvue.rh"
# include "nautilus\richedap.h"
# include "nautilus\nsLdv_Vars.h"
# include "nautilus\psapi.h"

// # include "nsdn\nsdocum.h"
// # include "nsdn\nsintrad.h"
# include "nautilus/nschoisi.h"
// # include "nssavoir\nspathor.h"
# include "nssavoir/nsfilgd.h"
# include "ns_sgbd/nschemin.h"		// classe NSChemins
// # include "partage\nsdivfct.h"    // fonction erreur
// # include "nsbb\nspatpat.h"
// # include "nautilus\nsannexe.h"   // base Critical.db

# ifdef __OB1__
#  include "ns_ob1\Interface.h"
# endif

# include "nssavoir\nslocalise.h"
// # include "nsbb\nsmanager.h"
# include "partage\NTArray.h"

# include "pilot\NautilusPilot.hpp"

//# include "ns_bbk\BB1BB.h"

# define NB_CHEMINS_BASES	6

# define MONNAIE_EURO 		0
# define MONNAIE_LOCALE 	1

// -----------------------------------------------------------------------------
// Classe NSSuper
// Big Brother principal : "Superviseur"
// -----------------------------------------------------------------------------
class NSSuper
{
 friend class NSContexte ;

 public:

 	enum CLASSDOCTYPES { isTree = 0, isFile, isCompta, isHTML, isText, isImage, isImageFixe, isImageAnimee, isURL } ;

  // Mode Trace
  enum TRACETYPE { trNone = -1, trError = 0, trWarning, trSteps, trSubSteps, trDetails, trSubDetails, trDebug = 9 } ;

  enum INDEXMANAGEMENT { imUndefined = -1, umSingleTree = 0, umAuto, umMultiTrees } ;

 protected:

  short                 _statut ;                      //!< Statut du superviseur : 0 = OK 1 = Anormal
  string                _sPathBase[NB_CHEMINS_BASES] ; //!< tableau des chemins d'accès aux bases de données -- voir aussi nsfiche.h

  NSLocalChapterArray*  _PathDB ;                     //!< Classe contenant les paramètre d'acces au base
  NSLocalChapterArray*  _typeMime ;                   //!< Classe contenant les type mymes en mémoire
  NSLocalChapterArray*  _whereToFind ;                //!< Classe permettant de savoir quel mécanique permet de saisir quelle information

  HANDLE                _m_hStdOut ;                   //!< Console

  NSSupportsArray       _aSupportsArray ;              //!< Array des supports
  NSContexte            *_pSuperContext ;              //!< pointeur sur le contexte

  TMyApp*               _pNSApplication ;              //!< pointeur sur la TApplication en cours
  TWindow*              _pMainWindow ;                 //!< pointeur sur la fenêtre principale

  hDBIDb                _hNSdb ;                       //!< pointeur sur la database active
  bool                  _bJavaOk ;                     //!< does Java be ok ?

  double                _parite ;                      //!< parité Franc/Euro pour la compta
  int                   _monnaieRef ;                  //!< monnaie de référence
  string                _sigle ;                       //!< symbole monétaire (??)
  bool                  _bComptaAuto ;                 //!< does compta is automatic in CR
  bool                  _bNoCCAMforReport ;            //!< don't use CCAM for reports
  bool                  _bNoCCAMforExam ;              //!< don't use CCAM for notes
  string                _indiceConsult ;               //!< indiceConsult
  bool                  _bCodageAuto ;                 //!< codage automatique
  string                _sExport_dll ;                 //!< dll d'Export
  string                _sAccountingExport_dll ;       //!< dll d'Export for accounting information
  string                _sStaysSynchro_dll ;           //!< dll for automatic stays synchronization
  string                _sNomModule ;                  //!< nom du module
  string                _sDiffuseur ;                  //!< diffuseur
  string                _sURLDiffuseur ;               //!< url du diffuseur
  string                _sDefaultFctUnitId ;           //!< Default functionnal unit Id
  string                _sCarePlaceId ;                //!< Default care place Id, ex Finess
  string                _sInPatientsFileDesc ;         //!< File name of in-patients's file descriptor
  string                _sOdbcPatientsFileDesc ;       //!< File name of odbc connector to an external patients repository
  bool                  _bUseIeForPdf ;                //!< Use PDF embedded in IE instead of PDF Com directly
  bool                  _bMustKillPdfProcess ;         //!< Must kill PDF process (when the usual close lags)

  string                _sUserFctDll[9] ;              //!< User specific dll(s)

  string                _sIppSite ;                    //!< site for permanent patient identifier
  string                _sOperationalSite ;            //!< site (sub-site) of the operational unit

  bool                  _bModeClientGroup ;           //!< mode Client/Group
  bool                  _bGroupInterfaceOnly ;        //!< mode Group : juste interface

  bool                  _bSlaveMode ;                 //!< Slave mode: automatic login and patient creation/opening (and refreshening)
  string                _sSlaveDllName ;              //!< Slave mode dll
  std::map<string, string> *pmExecArgs ;              //!< Parsed Exec args in the form field - value

  bool					        _bVerboseErrorMessages ;		  //!< affichage des erreurs systeme

  INDEXMANAGEMENT       _iIndexManagementRule ;       //!< mode de gestion des arbres d'index

  TRACETYPE             _iTrace ;                     //!< mode de trace
  TRACETYPE             _iTraceConsole ;              //!< mode de trace console
  TRACETYPE             _iTraceBBK ;                  //!< mode de trace BBK

  OWL::TModule*         _pGrabModule ;                 //!< pointeur sur le module de capture d'images
  NSMatrox*             _pMatrox ;                     //!< pointeur sur le contexte Matrox

	// Pointeur sur le réseau sémantique et fonctions associées
  NSFilGuide*           _pFilGuide ;	                  //!< codeur
  NSFilGuide*           _pFilDecode ;                  //!< décodeur

  // Pointeur sur l'objet de gestion du Graphe entre noeuds d'arbres
  // Note : ce pointeur est désactivé pour éviter les pbs multitache sur la base NSPatLink
  // NSGraphe*       pGraphe ;
                                                      //!< pointeur sur l'objet de gestion des archetypes
  NSArcManager*         _pArcManager ;

  string                _sDemographicArchetypeId ;
  string                _sAddressArchetypeId ;
  string                _sSimpleAddressArchetypeId ;
  string                _sPatientAddressEditArchetypeId ;
  string                _sProfessionnalAddressEditArchetypeId ;
  string                _sReferalProfessionnalArchetypeId ;
  string                _sReferalProfessionnalEditArchetypeId ;
  string                _sTeamMemberArchetypeId ;

  NSPersonArray*        _pPersonArray ;                //!< Pointeur sur l'objet de gestion des personnes
  string                _sHindex ;                     //!< index de l'aide en ligne
  string                _sHcorps ;                     //!< corps de l'aide en ligne
  NSWebTipWindow*       _pHelpTip ;

  char 	        	      _noConsole[3] ;                //!< numéro de la console
  string                _sAppName ;

  int                   _numInstance ;                 //!< numéro d'instance du program
  bool                  _bInitInstance ;

  OWL::TFont*           _pDialogFont ;

  skinArray*            _paSkins ;                     //!< Skins

  // Localisation
  string                _sLang ;                       //!< langue
  NSLocalChapterArray*  _pLocalStrings ;

  JniManager            _JniManager ;
  NautilusPilot*        _pPilot ;

  NSTaskArray           _aToDo ;
  bool                  _bToDoLocked ;

  NSPilotProxyWindow*   _pPilotProxyWindow ;
  NSInPersonsWindow*    _pInPersonsWindow ;

  NSHtmlToPdfConverter* _pHtmlToPdfConverter ;

 public:

  OWL::TModule*         pExeModule ;

  //! default constructor
  NSSuper() ;

  //! copy constructor
  NSSuper(const NSSuper& srcNSSuper) ;

  //! destructor
	~NSSuper() ;

  //! operator =
  NSSuper& operator=(const NSSuper& srcNSSuper) ;

  NSFilGuide*           getFilGuide()     { return _pFilGuide ; }
  NSFilGuide*           getFilDecode()    { return _pFilDecode ; }
  NSArcManager*         getArcManager()   { return _pArcManager ; }
  NSLocalChapterArray*  getLocalization() { return _pLocalStrings ; }
  NSContexte*           getContexte()     { return _pSuperContext ; }
  NautilusPilot*        getPilot()        { return _pPilot ; }

  void                  registerContexte(NSContexte* pContexte) ;

  NSPersonArray*        getPersonArray() { return _pPersonArray ; }

  skinArray*            getSkins() { return _paSkins ; }
  OWL::TFont*           getDialogFont() { return _pDialogFont ; }

  NSSupportsArray*      getSupports() { return &_aSupportsArray ; }
  NSLocalChapterArray*  getChemins()  { return _PathDB ; }
  NSLocalChapterArray*  getWhereTo()  { return _whereToFind ; }

  OWL::TModule *        getGrabModule() { return _pGrabModule ; }
  void									setGrabModule(OWL::TModule* pGrabMod) { _pGrabModule = pGrabMod ; }
  void                  initGrabModule(string sModule) ;
  NSMatrox*             getMatrox() { return _pMatrox ; }
  void					        initMatrox(NSMatrox* pMtrx) { _pMatrox = pMtrx ; }

  bool                  isJavaOk() { return _bJavaOk ; }

  const int             getInstance()       { return _numInstance ; }
  string                getInstanceString() {return IntToString(_numInstance) ; }
  void					        setInstance(int iInstance) { _numInstance = iInstance ; }
  bool					        getInitInstance()           { return _bInitInstance ; }
  void					        setInitInstance(bool bInit) { _bInitInstance = bInit ; }

  const char *          getConsole()       { return _noConsole ; }
  string                getConsoleString() { return string(_noConsole) ; }
  void                  setConsole(char cConsole[]) { strcpy(_noConsole, cConsole) ; }

  TRACETYPE             getTraceLevel()             { return _iTrace ; }
  void                  setTraceLevel(TRACETYPE iT) { _iTrace = iT ; }

  TRACETYPE             getBbkTraceLevel()             { return _iTraceBBK ; }
  void                  setBbkTraceLevel(TRACETYPE iT) { _iTraceBBK = iT ; }

  TRACETYPE             getTraceLevelFromString(string sTraceLevel) ;
  string                getTraceLevelLabel(TRACETYPE iTraceLevel) ;

  TMyApp *              getApplication() { return _pNSApplication ; }
  void									setApplication(TMyApp* pApp) { _pNSApplication = pApp ;}
//  TMDIClient *          prendClient() { return pNSApplication->prendClient() ; }
//  TFrameWindow *        GetMainWindow() { return pNSApplication->GetMainWindow() ; }
  const string          getAppName() { return _sAppName ; }
  hDBIDb                getDBHandle() { return _hNSdb ; }

	double                getParite()               { return _parite ; }
  int                   getMonnaieRef()           { return _monnaieRef ; }
  string                getSigle()                { return _sigle ; }
  bool                  isComptaAuto()            { return _bComptaAuto ; }
  bool                  isNoCcamForReport()       { return _bNoCCAMforReport ; }
  bool                  isNoCcamForExam()         { return _bNoCCAMforExam ; }
  string                getIndiceConsult()        { return _indiceConsult ; }
  bool                  isCodageAuto()            { return _bCodageAuto ; }
  string                getExportDll()            { return _sExport_dll ; }
  string                getAccountingExportDll()  { return _sAccountingExport_dll ; }
  string                getStaysSynchroDll()      { return _sStaysSynchro_dll ; }
  string                getUserModule(int iIndx)  { return ((iIndx >= 0) && (iIndx < 9)) ? _sUserFctDll[iIndx] : string("") ; }
  string                getNomModule()            { return _sNomModule ; }
  string                getDiffuseur()            { return _sDiffuseur ; }
  string                getURLDiffuseur()         { return _sURLDiffuseur ; }
  string                getIppSite()              { return _sIppSite ; }
  string                getOperationSite()        { return _sOperationalSite ; }
  string                getDefaultFctUnit()       { return _sDefaultFctUnitId ; }
  string                getCarePlaceId()          { return _sCarePlaceId ; }
  bool                  mustUseIeForPdf()         { return _bUseIeForPdf ; }
  bool                  mustKillPdfProcess()      { return _bMustKillPdfProcess ; }
  string                getInPatientsFileDesc()   { return _sInPatientsFileDesc ; }
  string                getOdbcPatientsFileDesc() { return _sOdbcPatientsFileDesc ; }
  INDEXMANAGEMENT       getIndexManagementRule()  { return _iIndexManagementRule ; }

  bool                  isSlaveMode()             { return _bSlaveMode ; }
  string                getSlaveDll()             { return _sSlaveDllName ; }

  string                getDemographicArchetypeId()              { return _sDemographicArchetypeId ; }
  string                getAddressArchetypeId()                  { return _sAddressArchetypeId ; }
  string                getSimpleAddressArchetypeId()            { return _sSimpleAddressArchetypeId ; }
  string                getPatientAddressEditArchetypeId()       { return _sPatientAddressEditArchetypeId ; }
  string                getProfessionnalAddressEditArchetypeId() { return _sProfessionnalAddressEditArchetypeId ; }
  string                getReferalProfessionnalArchetypeId()     { return _sReferalProfessionnalArchetypeId ; }
  string                getReferalProfessionnalEditArchetypeId() { return _sReferalProfessionnalEditArchetypeId ; }
  string                getTeamMemberArchetypeId()               { return _sTeamMemberArchetypeId ; }

  bool                  isToDoLocked()           { return _bToDoLocked ; }
  void                  lockToDo()               { _bToDoLocked = true ; }
  void                  unlockToDo()             { _bToDoLocked = false ; }

  void                  addToDoTask(NSToDoTask *t) { _aToDo.push_back(t) ; }
  NSTaskArray*          getToDoArray()             { return &_aToDo ; }

  std::map<string, string>* getExecArgs() { return pmExecArgs ; }

	void                  setAideIndex(string sAI) { _sHindex = sAI ; }
  void                  setAideCorps(string sAC) { _sHcorps = sAC ; }

  NSPilotProxyWindow*   getPilotProxyWindow()                       { return _pPilotProxyWindow ; }
  void                  setPilotProxyWindow(NSPilotProxyWindow* pW) { _pPilotProxyWindow = pW ; }

  NSInPersonsWindow*    getInPersonsWindow()                        { return _pInPersonsWindow ; }
  void                  setInPersonsWindow(NSInPersonsWindow* pW)   { _pInPersonsWindow = pW ; }

  string                sNumVersion ;

  void                  creePilot() ;

  void                  StartDebugConsole(int nWidth, int nHeight,const char* pszfname) ;
  void                  voidDebugPrintf(TRACETYPE traceLevel, const char *szfmt, ...) ;
  void                  DebugNewLine(void) ;

	// Drag And Drop
  bool			            bDragAndDrop ;
	bool			            offNode ;
	OWL::TImageList*      DragDrop ;
	NSCutPaste*           pBufDragDrop ;
	NSCutPaste*           pBufCopie ;

	// Liaison de noeuds
	bool                  bLiaisonEnCours ;
	NSPatPathoData*       pNoeudALier ;
	void                  lierNoeuds(NSPatPathoData* pNoeudLie) ;

  //! Récupération des données dans Nautilus.ini
  string                donneValeurIni(string chapitre, string libelle, string valeur) ;

  // Pour les chemins
  bool                  estEgal(string* chaine, string* modele/*, int precision = compReseau*/) ;

  bool                  InitialiseJava() { _bJavaOk = _JniManager.InitialiseJava() ; return _bJavaOk ; }
  void                  DestroyJava()    { _JniManager.DestroyJava() ; }
  bool                  ResetJava()      { return _JniManager.ResetJava() ; }

  // Construction et destruction de la database
  bool                  InitialiseDatabase(NSContexte* pCtx) ;
  void                  FermeDatabase() ;
  void                  DeleteLocalDatabaseObjects() ;

  bool                  InitialiseFileDatabase() ;

  TWindow*              donneMainWindow() { return _pMainWindow ; }
  void                  setMainWindow(TWindow* pWnd) { _pMainWindow = pWnd ; }

  // lancement nautilus par code utilisateur
  void                  AppelUtilisateur() ;
  NSUtilisateurChoisi*  GetNewUser() ;
  void                  SettleNewUser(NSUtilisateurChoisi* pNewUser) ;
  void                  GetSlaveUser() ;

  //! Chargement de l'ensemble des supports en mémoire
  bool                  chargeSupportsArray() ;

  //! Fonction pour récupérer les infos d'un support
  bool                  chercheSupportsInfo(string sCodeSupport, NSSupportsInfo* pInfo) ;

  //! Adding/Updating information to support structures
  bool                  setSupport(string sSupportName, string sSupportValue, string sSupportRemoval) ;
  bool                  setPath(string sPathName, string sPathValue, string sPathSupport, string sPathLabel, string sPathDocType) ;

  //! remplie un type mime
  bool                  chercheTypeMimeInfo(string sFichExt, NSTypeMimeInfo* pTypeMimeInfo) ;

  //! Initialise la parite Franc / Euro
  bool                  InitParite() ;

  //! Fonction initialisant les chemins d'accès aux bases
  bool                  InitPathBase(NSContexte* pCtx) ;

  //! Fonction récupérant un chemin dans sPathBase
  string                PathBase(string sCodeBase) ;

  //! Fonction récupérant un chemin dans la base des chemins
  string                PathName(string sCodeChemin, string* pSupport = 0, bool bVerbose = true, string* pDocType = 0) ;

  //! Fonction récupérant un chemin en fonction d'un type fichier
  string                PathNameType(string sTypeFichier, string& sLocalis, string& sUnite, string& sServeur, NSContexte* pCtx) ;

  //! Fonction récupérant un code chemin à partir du chemin
  string                CodeChemin(string sChemin, char carType, NSContexte* pCtx) ;

  //! Fonction de référencement automatique des archetypes et referentiels
  void                  ReferenceTousArchetypes() ;
	void                  ReferenceNewFilsGuides() ;

  //! Fonction de purge du répertoire d'impression
  void                  PurgeImpression() ;

  // Fonctions d'ouverture de documents
  NSNoyauDocument*      OuvreDocument(NSDocumentInfo& Document, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentCR(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentRTF(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentCS(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentHTML(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentIMG(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentCQ(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentQoL(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentTXT(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;
  NSNoyauDocument*      OuvreDocumentPDF(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml, NSContexte* pCtx, bool bReadOnly) ;

  // Get documents as HTML texts
  string                GetDocAsHtml(NSDocumentInfo& Document, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree) ;
  string                GetDocAsHtmlForCR(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree) ;
  string                GetDocAsHtmlForCS(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree) ;
  string                GetDocAsHtmlForCQ(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree) ;
  string                GetDocAsHtmlForQoL(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta, bool bInsertTree) ;
  string                GetDocAsHtmlForRTF(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta) ;
  string                GetDocAsHtmlForTXT(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta) ;
  string                GetDocAsHtmlForPDF(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta) ;
  string                GetDocAsHtmlForHTML(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta) ;
  string                GetDocAsHtmlForIMG(NSDocumentInfo* pDocument, NSContexte* pCtx, bool bInsertMeta) ;

  // Exports documents as HTML texts
  bool                  ExportDocAsHtml(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForCR(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForCS(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForCQ(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForQoL(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForRTF(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForTXT(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForPDF(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForHTML(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;
  bool                  ExportDocAsHtmlForIMG(NSDocumentInfo* pDocument, NSContexte* pCtx, string sDocName) ;

  NSHtmlToPdfConverter* getHtmlToPdfConverter() ;

  // Fonctions de contrôle des acces SGBD (en section critique)
  bool                  Bissextile(int year) ;
  void                  Delay(int centiemes) ;
  bool                  TimeFuture(struct date dateRef, struct time timeRef, int centiemes) ;

  // Fonctions de Navigation
  void NavigationEncyclopedie(string sUrl, NSContexte* pCtx) ;
  void NavigationCertificat(string sUrl, NSContexte* pCtx) ;
  void NavigationAideEnLigne() ;
  void EvTimer(uint id) ;

	// void addToDo(NSToDoTask* pTask, bool bSend = true) ;

  bool TraiteTask() ;

  // Fonctions de localisation
  // Localisation functions
  bool loadLocalisationString(string sLang, NSLocalChapterArray* pLocStrings = (NSLocalChapterArray*) 0) ;
  string getText(string sChapter, string sCode, string sLang = string(""), NSContexte* pCtx = (NSContexte*) 0) ;
  string getCapitalizedText(string sChapter, string sCode, string sLang = string(""), NSContexte* pCtx = (NSContexte*) 0) ;

  // Sémantique des types de document
  bool typeDocument(string sType, CLASSDOCTYPES categ, bool bPumpMessages = true) ;
  uint IDfromFunctionName(string sFctName) ;

  bool isManagementDoc(string sDocRoot) ;
  bool getWhereToFind(string sCodeSens, string &sWhat, string &sWhere) ;

  short donneStatut() ;
  hDBIDb PrendPDatabase() ;

  void trace(string[], int nbString = -1, TRACETYPE iLevel = trDetails, void* pObject = (void*) 0) ;
  void traceAndDisplay(string sChapter, string sMessage, TRACETYPE iLevel = trError, void* pObject = (void*) 0) ;
  void traceAndDisplayError(string sErrorText) ;

  void traceMemory(TRACETYPE iLevel = trDetails) ;

  void afficheStatusMessage(const string sMessage) ;

  void           sendMessageToSplash(string sMessage) ;
  void           setSplashPercentDone(int iPercent) ;
  TSplashWindow* getSplashWindow() ;

  void   parseExecArgs(int argc, char* argv[]) ;
  string getExecArgument(string sField) ;

	void initInstanceCounters() ;
	void traceInstanceCounters() ;

  void CheckZeroObjectCount(string sObjectName, long lInstanceCounter) ;
} ;

class NSCaptureArray ;

class NSUserPatRelation
{
 public:
  // User privileges for this patient
  bool bPatientIsTheUser ;
  bool bUserActiveInHealthTeam ;
  bool bUserPresentInHealthTeam ;
  bool bUserIsAdministrator ;
  bool bUserIsRootAdministrator ;

  int	 adminPosit[24][3] ;
  int	 existPosit[24][3] ;

  NSUserPatRelation() ;

  bool isAdministred(int iAngle, int iDistance) ;
  bool isPresent(int iAngle, int iDistance) ;

  void reinit() ;
  void init(NSContexte* pContexte) ;
} ;

class NSContexte : public NSSuperRoot
{
 protected:

  NSPatientChoisi*       _pPatient ;
  NSUtilisateurChoisi*   _pUtilisateur ;

  // Numéro de la transaction en cours
  string 	               _sTransaction ;

  string                 _sTransactionSys ;
  NSPersonGraphManager*  _pGraphPerson ;
  NSBasicAttributeArray* _pAllTraits ;

  // User privileges for this patient
  NSUserPatRelation 		 _relationship ;

  string                 _sBaseDirectory ;

  NSEpisodus*            _pEpisodus ;
  NSPredi*               _pPredi ;

# ifdef __OB1__
	BB1BBInterface*        _blackboardInterface ;         //!< pointeur sur l'interface du blackboard
  BB1BB*                 _bb ;                          //!< pointeur sur le blackboard
# else
	BB1BB*                 _blackboard ;                  //!< pointeur sur le blackboard
# endif // !__OB1__

  NSTaskArray            _aBBKToDo ;
  bool                   _bBBKToDoLocked ;

  NSDico*                _pDico ;                       //!< Pointeur sur le dictionnaire en cours

 public:

  NSContexte(NSSuper* pSuper) ;

  NSContexte(NSContexte& rv) ;

  ~NSContexte() ;

	NSBasicAttributeArray* getTraitList()							{ return _pAllTraits ; }
  void	setTraitList(NSBasicAttributeArray *pTraits) { _pAllTraits = pTraits ; }

  NSWebServiceWindow*    _pWebServiceWindow ;
  NSWebServiceChild*     _pWebServiceChild ;

  NSMailServiceWindow*   _pMailBoxWindow ;
  NSMailServiceChild*    _pMailBoxChild ;

	TWindow* GetMainWindow() { return _pSuper->donneMainWindow() ; }

  void             setPatient(NSPatientChoisi* pPat) { _pPatient = pPat; }
  NSPatientChoisi* getPatient()                      { return _pPatient; }

  void                 setUtilisateur(NSUtilisateurChoisi* pUti) ;
  NSUtilisateurChoisi* getUtilisateur()              { return _pUtilisateur ; }
  string               getUtilisateurID()            { return _pUtilisateur->getGraph()->getPersonID() ; }

  string               getUserLanguage() ;

  NSEpisodus*          getEpisodus()     { return _pEpisodus ; }
  NSPredi*             getPredi()        { return _pPredi ; }

	enum PERSONCLASSES { isUnknown = 0, isMemory, isLocal, isGlobal } ;

	PERSONCLASSES 			getPersonClass(string sPersonID) ;
	NSUserPatRelation*	getRelationship() { return &_relationship ; }

	void             setSuperviseur(NSSuper* pSup)     { _pSuper = pSup ; }
  NSSuper*         getSuperviseur()                  { return _pSuper ; }

  BB1BBInterface*  getBBinterface() { return _blackboardInterface ; }
  void						 setBBinterface(BB1BBInterface* pBBi) { _blackboardInterface = pBBi ; }

  BB1BB*           getBlackboard()            { return _bb ; }
  void						 setBlackboard(BB1BB* pBB1) { _bb = pBB1 ; }

  NSTaskArray*     getBBKToDo()       { return &_aBBKToDo ; }
  bool             getBBKToDoLocked() { return _bBBKToDoLocked ; }

  void             setTransaction(string sTran) { _sTransaction = sTran ; }
  string           getTransaction()             { return _sTransaction ; }

	void             setTransactionSysteme(string sTranSys) { _sTransactionSys = sTranSys; }
/*
  string           getTransactionSysteme(bool bLoad = true)
  {
  	if (bLoad)
    {
    	if (sTransactionSys != "")
      	return sTransactionSys ;
      else
      {
      	NSTransaction* pTransa = new NSTransaction(this);
        pTransa->nouvelleTransaction(true);
        delete pTransa;
        return sTransactionSys;
      }
    }
    else
    	return sTransactionSys;
  }
*/

  NSPersonGraphManager* getGraphPerson() { return _pGraphPerson ; }
  void resetGraphPerson()
  {
  	if (NULL == _pGraphPerson)
    	_pGraphPerson = new NSPersonGraphManager(this->getSuperviseur()) ;
    else
    	_pGraphPerson->graphReset() ;
  }

  NSDico*         getDico()           { return _pDico ; }
  NSFilGuide*     getFilGuide()       { return _pSuper->getFilGuide() ; }
  NSFilGuide*     getFilDecode()      { return _pSuper->getFilDecode() ; }
  NSArcManager*   getArcManager()     { return _pSuper->getArcManager() ; }
  NSPersonArray*  getPersonArray()    { return _pSuper->getPersonArray() ; }
  skinArray*      getSkins()          { return _pSuper->getSkins() ; }
  // note : la fonction getGraphe() est désactivée

  string          getBaseDirectory()           { return _sBaseDirectory ; }
  void            setBaseDirectory(string bd)  { _sBaseDirectory = bd ; }
  bool            reallocateBaseDirectory() ;

  void setAideIndex(string sAI) { _pSuper->_sHindex = sAI; }
  void setAideCorps(string sAC) { _pSuper->_sHcorps = sAC; }
  void setVerboseErrorMessages(bool bVerbose) { _pSuper->_bVerboseErrorMessages = bVerbose ; }

  void NavigationEncyclopedie(string sUrl) { _pSuper->NavigationEncyclopedie(sUrl, this); }
  void NavigationAideEnLigne()             { _pSuper->NavigationAideEnLigne() ; }

  NSWebTipWindow* getHelpTipPP()                        { return _pSuper->_pHelpTip ; }
  void            setHelpTipPP(NSWebTipWindow* pWebTip) { _pSuper->_pHelpTip = pWebTip ; }

  void addToDoBbk(NSToDoTask* pTask, bool bSend = true) ;
  // Fonctions du Blackboard
  bool BbkAskUser(string sArchetype, int iTypeArc = 0) ;
	bool Ob1AskUser(BB1BBInterfaceForKs* pKsDesc) ;
  bool bbkToDo(int iTypeArc, string ssWhatToDo, string ssParam1, string ssParam2, void* ppPointer1, bool p1, void* ppPointer2, bool p2) ;
  bool BbkShowReferential(string sReferential) ;

  bool chercheSupportsInfo(string sCodeSupport, NSSupportsInfo* pInfo)
          { return _pSuper->chercheSupportsInfo(sCodeSupport, pInfo) ; }
  string PathName(string sCodeChemin, string* pSupport = 0, bool bVerbose = true, string* pDocType = 0)
          { return _pSuper->PathName(sCodeChemin, pSupport, bVerbose, pDocType) ; }
  bool typeDocument(string sType, NSSuper::CLASSDOCTYPES categ, bool bPumpMessages = true)
          { return _pSuper->typeDocument(sType, categ, bPumpMessages) ; }

  bool isClientGroup()
          { return _pSuper->_bModeClientGroup ; }

  bool isGroupInterfaceOnly()
          { return _pSuper->_bGroupInterfaceOnly ; }

  bool isVerboseErrorMessages()
          { return _pSuper->_bVerboseErrorMessages ; }

  string CodeChemin(string sChemin, char carType)
          { return _pSuper->CodeChemin(sChemin, carType, this) ; }

  void traceAndDisplayError(string sErrorText) { _pSuper->traceAndDisplayError(sErrorText) ; }

/*
  bool InitDocumentBrut(NSDocumentInfo** ppDocument, string sCodeDocBrut)
          { return pSuper->InitDocumentBrut(ppDocument, sCodeDocBrut, this) ; }
*/

/*
  bool ChercheComposition(NSDocumentInfo** ppDocument, NSDocumentInfo** ppDocHtml)
          { return pSuper->ChercheComposition(ppDocument, ppDocHtml, this) ; }
*/

  bool captureData(NSCaptureArray *pCapture) ;

  void setMainCaption() ;

		//
		// Gestion des privilèges
		//
		enum	RIGHTSTO
		{			openPatient = 0,      openDocument ,			modifyDocument ,			createDocument ,      suppresDocument ,			modifyLdV ,			viewConcerns ,			createConcern ,			modifyConcern ,			viewDrug ,			createDrug ,			modifyDrug ,			viewGoal ,			createGoal ,			modifyGoal ,			viewProcess ,			createProcess ,			modifyProcess ,			viewSynthesis ,			modifySynthesis ,			viewAdminData ,			modifyAdminData ,			viewProData ,			modifyProData ,			viewHealthTeam ,			createHTMember ,			createMandate ,			modifyMandate ,			createRosace ,			modifyRosace ,			createUser ,		} ;

		bool userHasPrivilege(RIGHTSTO doThat, int i1, int i2, string s1, string s2, void* p1, void* p2) ;
} ;

/*
# ifdef _INCLUS
//***************************************************************************
// Implémentation des méthodes NSContexteArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSContexteArray::NSContexteArray(NSContexteArray& rv)
					:NSContexteVector()
{
try
{
    if (!(rv.empty()))
        for (ContexteIter i = rv.begin(); i != rv.end(); i++)
    	    push_back(new NSContexte(*(*i)));
}
catch (...)
{
    erreur("Exception NSContexteArray copy ctor.", standardError, 0);
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSContexteArray::vider()
{
    if (empty())
        return;
	for (ContexteIter i = begin(); i != end();)
    {
   	    // WARNING : NE PAS DELETER LES CONTEXTES !!!
        // delete *i; (sera fait par bi_nautilus)
        erase(i);
    }
}

NSContexteArray::~NSContexteArray()
{
	vider();
}
# endif
*/

/*
// Déclaration externe et création de l'objet Superviseur
//
# ifndef _INCLUS
#  ifndef _DANSDLL
#   ifndef __MAIN
extern NSContexte* pContexte;
#   else
#    ifndef __DLL__
                NSContexte* pContexte = new NSContexte();
                NSSuper* pNSSuperviseur = new NSSuper(pContexte);
#    else
extern NSContexte* pContexte;
#    endif
#   endif
#  endif
# endif
*/

// Fonction d'accès aux chemins des bases (initialisés par InitPathBase)
string
NSSuper::PathBase(string sCodeBase)
{
  string sPath = "" ;

  // on vérifie que le chemin n'est pas déjà dans un array du superviseur
  if (sCodeBase == string("BCHE"))
  	sPath = _sPathBase[0] ;
  if (sCodeBase == string("BGLO"))
  	sPath = _sPathBase[1] ;
  if (sCodeBase == string("BPAR"))
  	sPath = _sPathBase[2] ;
  if (sCodeBase == string("BPER"))
  	sPath = _sPathBase[3] ;
  if (sCodeBase == string("BCPT"))
  	sPath = _sPathBase[4] ;
  if (sCodeBase == string("BGUI"))
  	sPath = _sPathBase[5] ;

  return sPath ;
}

//
// Fonction chargeSupportsArray déclarée dans nssuper.h pour etre accessible par NS_SGBD
// (dans la fonction open pour l'ouverture des databases)
//
// -----------------------------------------------------------------------------
// Function     : NSSuper::chargeSupportsArray(NSContexte* pCtx)
// Arguments    : Un contexte pour l'ouverture de la base des supports
// Description  : Charge l'array des supports à partir de la base ou supports.dat en MUE
// Returns      : true : OK, false : echec
// -----------------------------------------------------------------------------
bool
NSSuper::chargeSupportsArray()
{
try
{
  // Note : on utilise sPathBase[0] qui contient le répertoire Perso
  // (initialisé dans ::InitPathBase())
  string sFichier = _sPathBase[0] + string("supports.dat") ;

  return _aSupportsArray.initFromFile(sFichier, this) ;

/*
  ifstream inFile ;
  string  line ;
  string  sData = "" ;
  string  sNomSupp = "" ;
  string  sFixSupp = "" ;
  string  sValSupp = "" ;
  size_t  i = 0 ;
  int     nbSupports = 0 ;



  inFile.open(sFichier.c_str()) ;  if (!inFile)
  {
  	string sErrorText = getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" (") + sFichier + string(")") ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  while (!inFile.eof())  {
    getline(inFile,line) ;
    if (line != "")
      sData += line + "\n" ;
  }

  inFile.close() ;
  // boucle de chargement des attributs de supports.dat  while (i < strlen(sData.c_str()))
  {
    // on récupère d'abord l'identifiant du support (1er champ)
    sNomSupp = "" ;
    sFixSupp = "" ;
    sValSupp = "" ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))
      sNomSupp += pseumaj(sData[i++]) ;

    if (sData[i] != '\n')
    {
      // On saute les blancs et les tabs
      while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')) && (sData[i] != '\n'))
        i++ ;

      if (sData[i] != '\n')
      {
        // on trouve ici le champ fixe (1 caractère : 0 ou 1)
        if (i < strlen(sData.c_str()))
          sFixSupp += sData[i++] ;

        // On saute les blancs et les tabs
        //
        while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')) && (sData[i] != '\n'))          i++ ;

        if (sData[i] != '\n')
        {
          // dernier champ : la valeur du support (path associé)
          while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))            sValSupp += sData[i++] ;
        }
      }
    }

    strip(sNomSupp, stripBoth) ;
    strip(sFixSupp, stripBoth) ;
    strip(sValSupp, stripBoth) ;

    if (strlen(sNomSupp.c_str()) > SUP_CODE_LEN)
      sNomSupp = "" ;
    if (strlen(sFixSupp.c_str()) > SUP_FIXE_LEN)
      sFixSupp = "" ;

    if ((sNomSupp != "") && (sFixSupp != "") && (sValSupp != ""))    {      NSSupportsInfo* pSuppInfo = new NSSupportsInfo() ;      strcpy(pSuppInfo->pDonnees->code, sNomSupp.c_str()) ;      strcpy(pSuppInfo->pDonnees->fixe, sFixSupp.c_str()) ;      pSuppInfo->pDonnees->sNomServeur = sValSupp ;      aSupportsArray.push_back(pSuppInfo) ;      nbSupports++ ;    }    else    {      erreur("Le fichier supports.dat est endommagé.", standardError) ;
      return false ;
    }    i++ ;  }

  if (nbSupports == 0)
  {
    erreur("Le fichier supports.dat est vide.", standardError) ;
    return false ;
  }

  return true ;
*/
}
catch (...)
{
  erreur("Exception NSSuper::chargeSupportsArray.", standardError, 0) ;
  return false ;
}
}

//
// Fonction chercheSupportsInfo déclarée dans nssuper.h pour etre accessible par NS_SGBD
// (dans la fonction open pour l'ouverture des databases)
//
//---------------------------------------------------------------------------------------
//  Function: NSSuper::chercheSupportsInfo(string sCodeSupport, NSSupportsInfo* pInfo)
//  Arguments:	  sCodeSupport : code du support à retrouver dans la base
//  Description: Retrouve les infos du support correspondant à sCodeSupport
//  Returns:     true : OK, false : echec
//---------------------------------------------------------------------------------------
bool
NSSuper::chercheSupportsInfo(string sCodeSupport, NSSupportsInfo* pInfo)
{
try
{
	// Si l'array des supports est vide, on le charge depuis la base ou le fichier (en MUE)
	if (_aSupportsArray.empty())
	{
  	bool result = chargeSupportsArray() ;
    if (false == result)
    	return false ;
	}

	for (SupportsIter i = _aSupportsArray.begin(); _aSupportsArray.end() != i ; i++)
  {
  	if (sCodeSupport == (*i)->getCode())
    {
    	// on récupère les infos du Support
      *pInfo = NSSupportsInfo(*(*i)) ;
      return true ;
    }
	}

	erreur("Code support introuvable dans le fichier des supports.", standardError, 0) ;
	return false ;
}
catch (...)
{
	erreur("Exception NSSuper::chercheSupportsInfo.", standardError, 0) ;
	return false ;
}
}

bool
NSSuper::setSupport(string sSupportName, string sSupportValue, string sSupportRemoval)
{
  string sFileName = _sPathBase[0] + string("supports.dat") ;
  return _aSupportsArray.setSupport(sFileName, this, sSupportName, sSupportValue, sSupportRemoval) ;
}

bool
NSSuper::setPath(string sPathName, string sPathValue, string sPathSupport, string sPathLabel, string sPathDocType)
{
  _PathDB->addText(sPathName, string("$NOM"),     sPathLabel) ;
  _PathDB->addText(sPathName, string("$CHEMIN"),  sPathValue) ;
  _PathDB->addText(sPathName, string("$SUPPORT"), sPathSupport) ;

  if (string("") != sPathDocType)
    _PathDB->addText(sPathName, string("$TYPE_DOC"), sPathDocType) ;

  string sFileName = PathBase("BCHE") + string("chemindb.dat") ;
  return _PathDB->writeToFile(sFileName, this) ;
}

//
// Fonction PathName déclarée dans nssuper.h pour etre accessible par NS_SGBD
// (dans la fonction open pour l'ouverture des databases)
//
//---------------------------------------------------------------------------------------
//  Function: NSSuper::PathName(string sCodeChemin, string* pUnite, string* pServeur)
//  Arguments:	  sCodeChemin : code du chemin à retrouver dans la base
//  Description: Retrouve le chemin correspondant à sCodeChemin
//  Returns:     Une string contenant le chemin entier
//---------------------------------------------------------------------------------------
string
NSSuper::PathName(string sCodeChemin, string* pSupport, bool bVerbose, string* pDocType)
{
try
{
	string sPath = "" ;

	// si le code chemin est vide on renvoie un path vide
  if (string("") == sCodeChemin)
  	return sPath ;

	//
	// Création d'une fiche Chemins
  //
	std::string sSupport = _PathDB->getLocalText(sCodeChemin, std::string("$SUPPORT")) ;
  std::string sChemin  = _PathDB->getLocalText(sCodeChemin, std::string("$CHEMIN")) ;

  if (string("") != sSupport)
  {
  	NSSupportsInfo SuppInfo ;
    if (chercheSupportsInfo(sSupport, &SuppInfo))
    {
    	sPath = SuppInfo.getRootPath() ;

      if ((string*) NULL != pSupport)
				*pSupport = sPath ;
    }
  }

  sPath += sChemin ;

  if ((string*) NULL != pDocType)
    *pDocType = _PathDB->getLocalText(sCodeChemin, std::string("$TYPE_DOC")) ;

	return sPath ;
}
catch (...)
{
	if (true == bVerbose)
		erreur("Exception PathName.", standardError, 0) ;
  return string("") ;
}
}

//---------------------------------------------------------------------------------------
//  Function: NSSuper::CodeChemin(string sChemin)
//  Arguments:	  chemin (relatif) dont on veut retrouver le code
//  Description: Retrouve le code chemin de sChemin
//  Returns:     "" si sChemin n'est pas dans la base, son code sinon
//---------------------------------------------------------------------------------------
string
NSSuper::CodeChemin(string sChemin, char carType, NSContexte* /*pCtx*/)
{
try
{
  map<string, NSLocalisedChapter* >::iterator iter ;

  if (_PathDB->empty())
    return "" ;

  for (iter = _PathDB->begin() ; iter != _PathDB->end() ; iter++)
  {
    string sCheminBase = (*iter).second->getLocalText("$CHEMIN") ;
    if (sCheminBase == sChemin)
    {
      string sCodeBase = (*iter).first ;
      if ((sCodeBase != "") && (sCodeBase[0] == carType))
        return sCodeBase ;
    }
  }
  return "" ;
}
catch (...)
{
  erreur("Exception NSSuper::CodeChemin.", standardError, 0) ;
  return "" ;
}
}


//---------------------------------------------------------------------------------------
//  Function: NSSuper::InitDocumentBrut(NSDocumentInfo* pDocument, string sCodeDocBrut)
//  Arguments:	  NSDocumentInfo (à initialiser) et code du document brut
//  Description: Retrouve le document brut et remet à jour pDocument
//  Returns:     true si la recherche s'est bien passée, false sinon
//---------------------------------------------------------------------------------------
/*
bool
NSSuper::InitDocumentBrut(NSDocumentInfo** ppDocument, string sCodeDocBrut, NSContexte* pCtx)
{
try
{
	NSDocumentInfo docInfo(sCodeDocBrut, pCtx) ;
	if (true == docInfo.ParseMetaDonnees())
	{
		*ppDocument = new NSDocumentInfo(docInfo) ;
    return true ;
	}

	return false ;
}
catch (...)
{
	erreur("Exception NSSuper::InitDocumentBrut.", standardError, 0) ;
	return false ;
}
}
*/

//--------------------------------------------------------------------------------------------
//  Fonction :   NSSuper::ChercheComposition(NSDocumentInfo* pDocument, NSDocumentInfo* pDocHtml)
//
//  Arguments:	  ppDocument p de p sur le DOCUMENT HDHTM
//               ppDocHtml  vide
//
//  Description: Retrouve le document brut et le place dans pDocument
//
//  Returns:     true si la recherche s'est bien passée, false sinon
//         		  ppDocument p de p sur le DOCUMENT BRUT
//					  ppDocHtml  p de p sur le DOCUMENT HDHTM
//--------------------------------------------------------------------------------------------
/*
bool
NSSuper::ChercheComposition(NSDocumentInfo** ppDocument, NSDocumentInfo** ppDocHtml, NSContexte* pCtx)
{
try
{
	string sCodeDocHtml ;
	string sCodeDocBrut ;
	string sCodeComposant ;
	bool existePrincipal = false ;

	// au départ **ppDocument contient un document html
	sCodeDocHtml = (*ppDocument)->getID() ;

	//
	// Création d'une fiche CompHtml
	//
	NSCompHtml* pCompHtml = new NSCompHtml(pCtx) ;
	pCompHtml->lastError = pCompHtml->open() ;
	if (pCompHtml->lastError != DBIERR_NONE)
	{
		erreur("Impossible d'ouvrir le fichier CompHtml.", standardError, pCompHtml->lastError);
		delete pCompHtml;
		return false;
	}

    // on recherche le composant principal (document brut)

    sCodeComposant = sCodeDocHtml + string("DOC") + string(CHTML_INDEX_COMP_LEN, ' ');

    pCompHtml->lastError = pCompHtml->chercheClef(&sCodeComposant,
                                                  "",
                                                  0,
                                                  keySEARCHGEQ,
                                                  dbiWRITELOCK);

    if ((pCompHtml->lastError != DBIERR_NONE) && (pCompHtml->lastError != DBIERR_EOF))
    {
		erreur("Erreur à la recherche du composant principal.", standardError, pCompHtml->lastError);
     	pCompHtml->close();
		delete pCompHtml;
		return false;
	}

    while (pCompHtml->lastError != DBIERR_EOF)
    {
   	    pCompHtml->lastError = pCompHtml->getRecord();
   	    if (pCompHtml->lastError != DBIERR_NONE)
   	    {
			erreur("Erreur à la lecture d'un composant html.", standardError, pCompHtml->lastError);
      	    pCompHtml->close();
			delete pCompHtml;
			return false;
		}

        // si on n'est pas sur un des composants document du fichier : on sort
        if (!(string(pCompHtml->pDonnees->code_html) == sCodeDocHtml)) break;
        if (!(string(pCompHtml->pDonnees->type_comp) == string("DOC"))) break;

        // on est sur un composant document, on regarde si c'est le principal

        if (string(pCompHtml->pDonnees->principal) == string("1"))
        {
      	    existePrincipal = true;
            break;
        }

        // sinon on passe au document suivant

        pCompHtml->lastError = pCompHtml->suivant(dbiWRITELOCK);
   	    if ((pCompHtml->lastError != DBIERR_NONE) && (pCompHtml->lastError != DBIERR_EOF))
   	    {
			erreur("Erreur à la recherche du composant html suivant.", standardError, pCompHtml->lastError);
      	    pCompHtml->close();
			delete pCompHtml;
			return false;
		}
    } // fin du while (recherche du composant principal)

    if (existePrincipal)
    {
   	    // on sauvegarde les infos du document html
        *ppDocHtml = new NSDocumentInfo(**ppDocument);

        // on récupère les infos du document brut
        sCodeDocBrut = string(pCompHtml->pDonnees->code_comp);
        InitDocumentBrut(ppDocument, sCodeDocBrut, pCtx);
    }
    else
    {
   	    erreur("Il n'existe pas de document principal.", standardError, 0);
        pCompHtml->close();
		delete pCompHtml;
		return false;
    }

    // on ferme la base CompHtml
    pCompHtml->lastError = pCompHtml->close();
	if (pCompHtml->lastError != DBIERR_NONE)
	{
		erreur("Impossible de fermer le fichier CompHtml.", standardError, pCompHtml->lastError);
		delete pCompHtml;
		return false;
	}

    delete pCompHtml;
    return true;
}
catch (...)
{
    erreur("Exception NSSuper::ChercheComposition.", standardError, 0);
    return false;
}
}
*/

//
// Fonction renvoyant true si l'année est bissextile, false sinon
//
bool
NSSuper::Bissextile(int year)
{
	// Règle : Si l'année est divisible par 4 ET non divisible par 100
	//				OU divisible par 400 ==> elle est bissextile.

	if (((year%4 == 0) && (!(year%100 == 0))) || (year%400 == 0))
		return true ;
	else
		return false ;
}

//
// Fonction permettant la pause du pgm pendant un nombre de centiemes de secondes donné
//
void
NSSuper::Delay(int centiemes)
{
  // on prend d'abord la date et l'heure
  //
  struct date date1 ;
  getdate(&date1) ;
  int year     = date1.da_year ;
  char mon     = date1.da_mon ;
  char day     = date1.da_day ;
  bool bissext = Bissextile(year) ;

  struct time time1 ;
  gettime(&time1) ;
  unsigned char hour = time1.ti_hour ;
  unsigned char min  = time1.ti_min ;
  unsigned char sec  = time1.ti_sec ;
  unsigned char hund = time1.ti_hund ;

  // on calcule la date et l'heure après ajout du délai
  //
  unsigned int iNewhund = hund + centiemes ;

  while (iNewhund >= 100)
  {
    sec += (unsigned char) 1 ;
    iNewhund -= 100 ;
  }
  hund = (unsigned char) iNewhund ;

  while (sec >= 60)
  {
    min += (unsigned char) 1 ;
    sec -= (unsigned char) 60 ;
  }

  while (min >= 60)
  {
    hour += (unsigned char) 1 ;
    min -= (unsigned char) 60 ;
  }

  while (hour >= 24)
  {
    day += (char) 1 ;
    hour -= (unsigned char) 24 ;
  }

  int	lim = 30 ;

  switch (mon)
  {
    case 1  : lim = 31 ; break ;
    case 2  : if (bissext)
    					  lim = 29 ;
        		 else
             	  lim = 28 ;
             break ;
    case 3  : lim = 31 ; break ;
    case 5  : lim = 31 ; break ;
    case 7  : lim = 31 ; break ;
    case 8  : lim = 31 ; break ;
    case 10 : lim = 31 ; break ;
    case 12 : lim = 31 ; break ;
	}

  if (day > lim)
  {
    mon += (char) 1 ;
    day -= (char) lim ;
  }

  if (mon > 12)
  {
    year += 1 ;
    mon -= (char) 12 ;
  }

  // on enregistre la date de fin du délai
	struct time time2 ;
  time2.ti_hund = hund ;
  time2.ti_sec  = sec ;
  time2.ti_min  = min ;
  time2.ti_hour = hour ;
  struct date date2 ;
  date2.da_day  = day ;
  date2.da_mon  = mon ;
  date2.da_year = year ;

	// on boucle jusqu'à ce qu'on dépasse cette date
  while (1)
  {
  	getdate(&date1) ;
    gettime(&time1) ;

    if (date1.da_year == date2.da_year)
    {
      if (date1.da_mon == date2.da_mon)
      {
        if (date1.da_day == date2.da_day)
        {
          if (time1.ti_hour == time2.ti_hour)
          {
            if (time1.ti_min == time2.ti_min)
            {
              if (time1.ti_sec == time2.ti_sec)
              {
                if (time1.ti_hund > time2.ti_hund)
                  return ;
              }
              else if (time1.ti_sec > time2.ti_sec)
                return ;
            }
            else if (time1.ti_min > time2.ti_min)
              return ;
          }
          else if (time1.ti_hour > time2.ti_hour)
            return ;
        }
        else if (date1.da_day > date2.da_day)
          return ;
      }
      else if (date1.da_mon > date2.da_mon)
        return ;
    }
    else if (date1.da_year > date2.da_year)
      return ;

    _pNSApplication->PumpWaitingMessages() ;
	}
}

//
// Fonction permettant de voir si on a dépassé une heure de reference d'un certain délai
//
bool
NSSuper::TimeFuture(struct date dateRef, struct time timeRef, int centiemes)
{
	struct date     date1, date2 ;
    struct time     time1, time2 ;
    int			    year ;
    char            day, mon ;
    unsigned char   hour, min, sec, hund ;
    bool		    bissext ;
    int			    lim ;

    // on recalcule d'abord la date et l'heure de référence
    year    = dateRef.da_year;
    mon     = dateRef.da_mon;
    day     = dateRef.da_day;
    bissext = Bissextile(year);

    hour    = timeRef.ti_hour;
    min     = timeRef.ti_min;
    sec     = timeRef.ti_sec;
    hund    = timeRef.ti_hund;

    // on calcule la date et l'heure après ajout du délai
    unsigned int iNewhund = hund + centiemes ;

    while (iNewhund >= 100)
    {
    	sec += (unsigned char) 1 ;
        iNewhund -= 100;
    }
    hund = (unsigned char) iNewhund ;

    while (sec >= 60)
    {
    	min += (unsigned char) 1 ;
        sec -= (unsigned char) 60 ;
    }

    while (min >= 60)
    {
    	hour += (unsigned char) 1 ;
        min -= (unsigned char) 60;
    }

    while (hour >= 24)
    {
    	day += (char) 1;
        hour -= (unsigned char) 24;
    }

    switch (mon)
    {
    	case 1 : lim = 31; break;
        case 2 : if (bissext) lim = 29;
        		 else lim = 28;
                 break;
        case 3 : lim = 31; break;
        case 4 : lim = 30; break;
        case 5 : lim = 31; break;
        case 6 : lim = 30; break;
        case 7 : lim = 31; break;
        case 8 : lim = 31; break;
        case 9 : lim = 30; break;
        case 10 : lim = 31; break;
        case 11 : lim = 30; break;
        case 12 : lim = 31; break;

        default :
        	erreur("Mois incorrect dans le calcul du délai", standardError, 0);
            return false;
    }

    if (day > lim)
    {
    	mon += (char) 1;
        day = 1;
    }

    if (mon > 12)
    {
    	year += 1;
        mon = 1;
    }

    // on enregistre la date de fin du délai
    time2.ti_hund   = hund;
    time2.ti_sec    = sec;
    time2.ti_min    = min;
    time2.ti_hour   = hour;
    date2.da_day    = day;
    date2.da_mon    = mon;
    date2.da_year   = year;

    // on regarde si on a dépassé cette date
    getdate(&date1);
    gettime(&time1);

    if (date1.da_year == date2.da_year)
    {
        if (date1.da_mon == date2.da_mon)
        {
            if (date1.da_day == date2.da_day)
            {
                if (time1.ti_hour == time2.ti_hour)
                {
                    if (time1.ti_min == time2.ti_min)
                    {
                        if (time1.ti_sec == time2.ti_sec)
                        {
                            if (time1.ti_hund < time2.ti_hund)
                                return false;
                            else // on est arrivé au délai
                                return true;
                        }
                        else if (time1.ti_sec < time2.ti_sec)
                            return false;
                        else
                            return true;
                    }
                    else if (time1.ti_min < time2.ti_min)
                        return false;
                    else
                        return true;
                }
                else if (time1.ti_hour < time2.ti_hour)
                    return false;
                else
                    return true;
            }
            else if (date1.da_day < date2.da_day)
                return false;
            else
                return true;
        }
        else if (date1.da_mon < date2.da_mon)
            return false;
        else
            return true;
    }
    else if (date1.da_year < date2.da_year)
        return false;
    else
        return true;
}

//
// Aide en ligne
//
# include <process.h>

void
NSSuper::NavigationAideEnLigne()
{
try
{
  string sNomModele ;

  if     ((string("") == _sHindex) && (string("") == _sHcorps))
    sNomModele = PathName("HELP") + "manual.htm" ;
  else if (string("") == _sHindex)
  {
    sNomModele = PathName("HELP") + _sHcorps ;

        //
        // FTP ou HTTP ?
        //
        // if ((string(sNomModele, 0, 2) == string("\\\\")) ||
        //    (sNomModele[1] == ':'))
        //    sNomModele = string("file:///") + sNomModele ;
  }
  else
  {
    sNomModele = PathName("HELP") + string("h_modele.htm") ;

    ifstream inFile;
    inFile.open(sNomModele.c_str());
    if (!inFile)
    {
      erreur("Erreur d'ouverture du modèle d'aide.", standardError, 0);
      return;
    }

    string sModele = "";
    string line;

        while (!inFile.eof())
        {
            getline(inFile,line);
            if (line != "")
                sModele += line + "\n";
        }
        inFile.close();

        size_t pos;

        if (_sHindex != "")
        {
    	    pos = sModele.find(string("h_index.htm"));
   	        if (pos != string::npos)
     		    sModele.replace(pos, 11, _sHindex);
        }

        if (_sHcorps != "")
        {
   	        pos = sModele.find(string("h_noyau.htm"));
   	        if (pos != string::npos)
     		    sModele.replace(pos, 11, _sHcorps);
        }

        sNomModele = PathName("HELP") + string("h_") + string(_noConsole)
                                                            + string(".htm");

        ofstream outFile;
        outFile.open(sNomModele.c_str());
        if (!outFile)
        {
    	    erreur("Erreur d'écriture du fichier d'aide.", standardError, 0);
            return;
        }
        outFile << sModele;
        outFile.close();
  }

  ::ShellExecute(donneMainWindow()->HWindow,
                 "open",
                 sNomModele.c_str(),
                 "",
                 "",
                 SW_SHOWNORMAL) ;
}
catch (...)
{
    erreur("Exception NSSuper::NavigationAideEnLigne.", standardError, 0);
}
}

//
// Fonction trace déclarée dans nssuper.h pour être accessible par NS_SGBD
//
//---------------------------------------------------------------------------------------
//  Function: 	  NSSuper::trace(string** pTracePtr, int nbString)
//  Arguments:	  pTracePtr : pointeur sur un tableau de pointeurs sur les string
//					  				  à tracer (si bTrace == true)
//					  nbString :  nombre de chaines - optionnel, la trace s'arrête
//									  au premier pointeur vide
//  Description: Inscrit les strings dans le fichier de trace avec l'heure système
//  Returns:     Rien
//---------------------------------------------------------------------------------------
void
NSSuper::trace(string pTracePtr[], int nbString, TRACETYPE iTraceLevel, void* pObject)
{
try
{
	if (iTraceLevel > _iTrace)
		return ;

  if ((NULL == pTracePtr) || (string("") == pTracePtr[0]))
		return ;

	string sTraceFileName ;
	if (_numInstance > 1)
		sTraceFileName = string("traceNau") + IntToString(_numInstance) + string(".inf") ;
  else
  	sTraceFileName = string("traceNau.inf") ;
	ofstream outFile ;
	outFile.open(sTraceFileName.c_str(), ios::app) ;
	if (!outFile)
		return ;

	// calcul de l'heure système - get system time
  //
  struct  date dateSys ;
	struct  time heureSys ;
	char    msg[255] ;

	getdate(&dateSys) ;
	gettime(&heureSys) ;
	sprintf(msg, "%02d/%02d/%4d - %02d:%02d:%02d,%02d>", dateSys.da_day, dateSys.da_mon,
                dateSys.da_year, heureSys.ti_hour, heureSys.ti_min,
                heureSys.ti_sec, heureSys.ti_hund) ;

	int iNbre = nbString ;
	if (-1 == iNbre)
		iNbre = 1 ;

	outFile << string(msg) ;

  if (pObject)
  {
    sprintf(msg, "%p", pObject) ;
    outFile << ( string(" (") + string(msg) + string(")") ) ;
  }

	for (int i = 0 ; (i < iNbre) && (string("") != pTracePtr[i]) ; i++)
	{
		for (int j = 0 ; j < iTraceLevel ; j++)
    	outFile << string("\t") ;

		outFile << pTracePtr[i] ;
	}

	outFile << string("\n") ;
	outFile.close() ;
}
catch (...)
{
	erreur("Exception NSSuper::trace.", standardError, 0) ;
}
}

void
NSSuper::traceAndDisplay(string sChapter, string sMessage, TRACETYPE iLevel, void* pObject)
{
  string sErrorText = getText(sChapter, sMessage) ;
  trace(&sErrorText, 1, iLevel, pObject) ;
  if (trError == iLevel)
    erreur(sErrorText.c_str(), standardError, 0) ;
  else
    erreur(sErrorText.c_str(), warningError, 0) ;
}

void
NSSuper::traceAndDisplayError(string sErrorText)
{
  if (string("") == sErrorText)
    return ;

  trace(&sErrorText, 1, NSSuper::trError) ;
  erreur(sErrorText.c_str(), standardError, 0) ;
}

void
NSSuper::traceMemory(TRACETYPE iLevel)
{
  // Global memory
  //

  // Use to convert bytes to KB
  #define DIV 1024

  // Specify the width of the field in which to print the numbers.
  // The asterisk in the format specifier "%*I64d" takes an integer
  // argument and uses it to pad and right justify the number.
  #define WIDTH 7

  MEMORYSTATUSEX statex ;
  statex.dwLength = sizeof (statex) ;

  GlobalMemoryStatusEx(&statex) ;

  char szTraceInfo[256] ;

  sprintf(szTraceInfo, "There is  %*ld percent of memory in use.",           WIDTH, statex.dwMemoryLoad) ;
  string sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of physical memory.",  WIDTH, statex.ullTotalPhys/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of physical memory.",   WIDTH, statex.ullAvailPhys/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of paging file.",    WIDTH, statex.ullTotalPageFile/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of paging file.",     WIDTH, statex.ullAvailPageFile/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of virtual memory.", WIDTH, statex.ullTotalVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of virtual memory.",  WIDTH, statex.ullAvailVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;

  // Show the amount of extended memory available.
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of extended memory.", WIDTH, statex.ullAvailExtendedVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;

  // Process memory
  //

  PROCESS_MEMORY_COUNTERS pmc ;

  DWORD processID = GetCurrentProcessId() ;

  // Print the process identifier.

  sprintf(szTraceInfo, "Process ID: %u", processID ) ;

  sTrace = string(szTraceInfo) ;
  trace(&sTrace, 1, iLevel) ;

  // Print information about the memory usage of the process.

  HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                                    FALSE, processID ) ;
  if (NULL == hProcess)
  {
    sTrace = string("Can't query process information.") ;
    trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
  {
    sprintf(szTraceInfo, "PageFaultCount: 0x%08X",             pmc.PageFaultCount) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    // sprintf(szTraceInfo, "PeakWorkingSetSize: 0x%08X",         pmc.PeakWorkingSetSize) ;
    sprintf(szTraceInfo, "Episodus peak use was %u total Kbytes of memory.", pmc.PeakWorkingSetSize/DIV) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    // sprintf(szTraceInfo, "WorkingSetSize: 0x%08X",             pmc.WorkingSetSize) ;
    sprintf(szTraceInfo, "Episodus uses %u total Kbytes of memory.", pmc.WorkingSetSize/DIV) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "QuotaPeakPagedPoolUsage: 0x%08X",    pmc.QuotaPeakPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "QuotaPagedPoolUsage: 0x%08X",        pmc.QuotaPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "QuotaPeakNonPagedPoolUsage: 0x%08X", pmc.QuotaPeakNonPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "QuotaNonPagedPoolUsage: 0x%08X",     pmc.QuotaNonPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "PagefileUsage: 0x%08X",              pmc.PagefileUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
    sprintf(szTraceInfo, "PeakPagefileUsage: 0x%08X",          pmc.PeakPagefileUsage) ;
    sTrace = string(szTraceInfo) ;
    trace(&sTrace, 1, iLevel) ;
  }

  CloseHandle(hProcess) ;
}

void
NSSuper::StartDebugConsole(int nWidth, int nHeight, const char* /*pszfname*/)
{
	if (_m_hStdOut)
  	return ;

  AllocConsole() ;
  SetConsoleTitle("Debug Window") ;
  _m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE) ;

  COORD co = {(short) nWidth, (short) nHeight } ;
  SetConsoleScreenBufferSize(_m_hStdOut, co) ;

  co.X = co.Y = 0 ;
  SetConsoleCursorPosition(_m_hStdOut, co) ;
}

void
NSSuper::voidDebugPrintf(TRACETYPE traceLevel, const char *szfmt,...)
{
	if (_iTraceConsole > traceLevel)
		return ;
  char s[300];
  va_list argptr;

  va_start(argptr, szfmt) ;
  /* int cnt = */ vsprintf(s, szfmt, argptr) ;
  va_end(argptr) ;

  DWORD cCharsWritten ;
  if (_m_hStdOut)
  {
  	DebugNewLine() ;
    WriteConsole(_m_hStdOut, s, strlen(s), &cCharsWritten, NULL) ;
  }
}

void
NSSuper::DebugNewLine()
{
	CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
  if (! GetConsoleScreenBufferInfo(_m_hStdOut, &csbiInfo))
  	return ;
  csbiInfo.dwCursorPosition.X = 0 ;

  // If it is the last line in the screen buffer, scroll
  // the buffer up.
  if ((csbiInfo.dwSize.Y - 1) == csbiInfo.dwCursorPosition.Y)
  {
  	SMALL_RECT srctWindow ;
    if (csbiInfo.srWindow.Top > 0)
    {
      srctWindow.Top    = -1 ;      // move top up by one row
      srctWindow.Bottom = -1 ;   // move bottom up by one row
      srctWindow.Left   = 0 ;      // no change
      srctWindow.Right  = 0 ;     // no change
      if (! SetConsoleWindowInfo(
          _m_hStdOut,      // screen buffer handle
          FALSE,        // deltas, not absolute
          &srctWindow)) // specifies new location
      	return ;
    }
  }
  // Otherwise, advance the cursor to the next line.
  else
  	csbiInfo.dwCursorPosition.Y++ ;

  if (! SetConsoleCursorPosition(_m_hStdOut, csbiInfo.dwCursorPosition))
  	return ;
}

void
NSSuper::afficheStatusMessage(const string sMessage)
{
try
{
	if (NULL == _pNSApplication)
		return ;

	OWL::TStatusBar* pStatus = _pNSApplication->getStatusBar() ;
  if (pStatus)
    pStatus->SetText(sMessage.c_str()) ;

  _pNSApplication->PumpWaitingMessages() ;
}
catch (...)
{
  string sErrorText = string("Exception NSSuper::afficheStatusMessage for message \"") + sMessage + string("\"") ;
  trace(&sErrorText, 1, NSSuper::trError) ;
}
}

/*
void
NSSuper::addToDo(NSToDoTask* pTask, bool bSend)
{
	if (NULL == pTask)
		return ;

	while (bToDoLocked)
		pNSApplication->PumpWaitingMessages() ;

	bToDoLocked = true ;
	aToDo.push_back(pTask) ;
	bToDoLocked = false ;

  string  sWhatToDo ;
  string  sParam1 ;
  string  sParam2 ;

  string ps = string("addToDo ") + pTask->sWhatToDo +
  						string(" sP1=") + pTask->sParam1 +
              string(" sP2=") + pTask->sParam2 ;
	trace(&ps, 1, trSubDetails);

	if (bSend)
		pNSApplication->GetMainWindow()->PostMessage(WM_COMMAND, IDM_TODO) ;
}
*/

string
NSSuper::getExecArgument(string sField)
{
  if ((NULL == pmExecArgs) || (pmExecArgs->empty()) || (string("") == sField))
    return string("") ;

  return (*pmExecArgs)[sField] ;
}

void
NSSuper::CheckZeroObjectCount(string sObjectName, long lInstanceCounter)
{
  if ((long)0 == lInstanceCounter)
    return ;

  char buf[255] = {0} ;
  sprintf(buf, "%d", lInstanceCounter) ;

  string sTrace = string("Memory leak for object ") + sObjectName +
                  string(" (Instance counter = ") + string(buf) + string(")") ;
	trace(&sTrace, 1, trError) ;
}

void
NSContexte::addToDoBbk(NSToDoTask* pTask, bool bSend)
{
try
{
	if (NULL == pTask)
		return ;

#ifdef __OB1__

	while (_bBBKToDoLocked)
    _pSuper->getApplication()->PumpWaitingMessages() ;

	_bBBKToDoLocked = true ;
	_aBBKToDo.push_back(pTask) ;
  _bBBKToDoLocked = false ;

	if (bSend)
		_blackboardInterface->PostMessage(WM_COMMAND, IDM_BBKTODO) ;

#endif
}
catch (...)
{
  string sErrorText = string("Exception NSContexte::addToDoBbk.") ;
  _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
}
}

/*
bool
NSContexte::BbkAskUser(string sArchetype, int iTypeArc)
{
try
{
	// if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
	// 	return false ;

	char szType[10] ;
	itoa(iTypeArc, szType, 10) ;

	NSToDoTask* pTask = new NSToDoTask ;

	pTask->sWhatToDo = "Archetype" ;
	pTask->sParam1   = sArchetype ;
	pTask->sParam2   = string(szType) ;

	pSuper->addToDo(pTask) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSContexte::BbkAskUser.", standardError, 0) ;
	return false ;
}
}
*/

#ifdef __OB1__

/*
bool
NSContexte::Ob1AskUser(BB1BBInterfaceForKs* pKsDesc)
{
try
{
	// if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
	//	return false ;

	NSToDoTask* pTask = new NSToDoTask ;

	pTask->sWhatToDo = "KS_Archetype" ;
  pTask->pPointer1 = pKsDesc ;
  pTask->deleteP1  = false ;

	pSuper->addToDo(pTask) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSContexte::Ob1AskUser.", standardError, 0) ;
	return false ;
}
}
*/

/*
bool
NSContexte::bbkToDo(int iTypeArc, string ssWhatToDo, string ssParam1, string ssParam2, void* ppPointer1, bool p1, void* ppPointer2, bool p2)
{
try
{
	// if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
	// 	return false ;

	char szType[10] ;
	itoa(iTypeArc, szType, 10) ;

	NSToDoTask* pTask = new NSToDoTask ;

	pTask->sWhatToDo = ssWhatToDo ;
	pTask->sParam1   = ssParam1 ;
	pTask->sParam2   = ssParam2 ;
	pTask->pPointer1 = ppPointer1 ;
	pTask->deleteP1  = p1 ;
	pTask->pPointer2 = ppPointer2 ;
	pTask->deleteP2  = p2 ;

  addToDoBbk(pTask) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSContexte::bbkToDo.", standardError, 0) ;
	return false ;
}
}
*/
#endif


/*
bool
NSContexte::BbkShowReferential(string sReferential)
{
try
{
	// if ((!pNSApplication) || (!(pNSApplication->GetMainWindow())))
	// 	return false ;

	NSToDoTask* pTask = new NSToDoTask;

	pTask->sWhatToDo = "Referential";
	pTask->sParam1  = sReferential;

  pSuper->addToDo(pTask) ;

	return true;
}
catch (...)
{
  erreur("Exception NSContexte::BbkShowReferential.", standardError, 0) ;
  return false ;
}
}
*/

bool
NSSuper::loadLocalisationString(string sLangage, NSLocalChapterArray* pLocStrings)
{
  NSLocalChapterArray* pActualLocStrings = _pLocalStrings ;
  if (pLocStrings)
    pActualLocStrings = pLocStrings ;
  else if ((sLangage == _sLang) && (false == _pLocalStrings->empty()))
		return true ;

	ifstream inFile1 ;

	if (string("") != sLangage)
	{
		string sFichier = string("localisation_") + sLangage + string(".dat") ;
		inFile1.open(sFichier.c_str()) ;
	}
	else
	{
		inFile1.open("localisation.dat") ;
		if (!inFile1)
    	return false ;
	}

	if (!inFile1)
	{
		ifstream inFile2 ;

		string sFichier = string("localisation.dat") ;
		inFile2.open(sFichier.c_str()) ;
		if (!inFile2)
			return false ;

    string sErrorMsg = string("") ;
		pActualLocStrings->init(&inFile2, &sErrorMsg) ;

		inFile2.close() ;
	}
	else
	{
    string sErrorMsg = string("") ;
		pActualLocStrings->init(&inFile1, &sErrorMsg) ;

		inFile1.close() ;
	}

  if (pLocStrings)
    _sLang = sLangage ;

	return true ;
}

string
NSSuper::getText(string sChapter, string sCode, string sLang, NSContexte* pCtx)
{
  if (string("") == sLang)
  {
	  if (_pLocalStrings)
		  return _pLocalStrings->getLocalText(sChapter, sCode, true) ;
    else
      return string("") ;
  }

  if (NULL == pCtx)
    return string("") ;

  NSLocalChapterArray aLocStrings ;
  if (false == loadLocalisationString(sLang, &aLocStrings))
    return string("") ;

  return aLocStrings.getLocalText(sChapter, sCode, true) ;
}

string
NSSuper::getCapitalizedText(string sChapter, string sCode, string sLang, NSContexte* pCtx)
{
  string sText = getText(sChapter, sCode, sLang, pCtx) ;
  if (string("") != sText)
    sText[0] = pseumaj(sText[0]) ;

  return sText ;
}

void
NSSuper::initGrabModule(string sModule)
{
	if (_pGrabModule)
  	delete _pGrabModule ;

  // Initialisation du module de la dll de capture
  _pGrabModule = new OWL::TModule(sModule.c_str(), TRUE) ;
}

bool
NSSuper::typeDocument(string sType, CLASSDOCTYPES categ, bool bPumpMessages)
{
	bool res = false ;

  if (false == _pFilGuide->Prend())
    return false ;

  switch (categ)
  {
    case isTree         : res = _pFilGuide->VraiOuFaux(sType, "ES", "0CLDA", "FLECHE", bPumpMessages) ; break ;
    case isFile         : res = _pFilGuide->VraiOuFaux(sType, "ES", "0CLDF", "FLECHE", bPumpMessages) ; break ;
    case isCompta       : res = _pFilGuide->VraiOuFaux(sType, "ES", "0CLDC", "FLECHE", bPumpMessages) ; break ;
    case isHTML         : res = _pFilGuide->VraiOuFaux(sType, "ES", "0CLDH", "FLECHE", bPumpMessages) ; break ;
    case isText         : res = _pFilGuide->VraiOuFaux(sType, "ES", "ZTXT0", "FLECHE", bPumpMessages) ; break ;
    case isImage        : res = _pFilGuide->VraiOuFaux(sType, "ES", "ZIMA0", "FLECHE", bPumpMessages) ; break ;
    case isImageFixe    : res = _pFilGuide->VraiOuFaux(sType, "ES", "ZIF00", "FLECHE", bPumpMessages) ; break ;
    case isImageAnimee  : res = _pFilGuide->VraiOuFaux(sType, "ES", "ZIA00", "FLECHE", bPumpMessages) ; break ;
    case isURL          : res = _pFilGuide->VraiOuFaux(sType, "ES", "0CLRE", "FLECHE", bPumpMessages) ; break ;
  }
  return res ;
}

bool
NSSuper::getWhereToFind(string sCode, string &sWhat, string &sWhere)
{
try
{
  sWhat  = string("") ;
  sWhere = string("") ;

	if ((string("") == sCode) || _whereToFind->empty())
  	return false ;

  string sCodeSens ;
  NSDico::donneCodeSens(&sCode, &sCodeSens) ;

  NSLocalisedChapter* pChapter = _whereToFind->getChapter(sCodeSens) ;
  if (NULL == pChapter)
    return false ;

  sWhat = pChapter->getLocalText(string("type")) ;

  if      (string("QoL") == sWhat)
    sWhere = pChapter->getLocalText(string("file")) ;
  else if (string("Archetype") == sWhat)
    sWhere = pChapter->getLocalText(string("id")) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSSuper::getWhereToFind.", standardError, 0) ;
	return false ;
}
}

void
NSSuper::sendMessageToSplash(string sMessage)
{
	if (_pNSApplication)
	  _pNSApplication->sendMessageToSplash(sMessage) ;
}

void
NSSuper::setSplashPercentDone(int iPercent)
{
	if (_pNSApplication)
	  _pNSApplication->setSplashPercentDone(iPercent) ;
}

TSplashWindow*
NSSuper::getSplashWindow()
{
  if (_pNSApplication)
    return _pNSApplication->getSplashWindow() ;
  return (TSplashWindow*) 0 ;
}

bool
NSSuper::isManagementDoc(string sDocRoot)
{
  if (string("") == sDocRoot)
    return false ;

  string sSemRoot = string("") ;
  NSDico::donneCodeSens(&sDocRoot, &sSemRoot) ;

  if (string("LEQUI") == sSemRoot)  // Health team
    return true ;

  // Health index
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    string sFrameRoot    = getRootForFrame(getFrameForIndex(i)) ;
    string sSemFrameRoot = string("") ;
    NSDico::donneCodeSens(&sFrameRoot, &sSemFrameRoot) ;

    if (sSemFrameRoot == sSemRoot)  // Health team
      return true ;
  }

  return false ;
}

uint
NSSuper::IDfromFunctionName(string sFctName)
{
	if (sFctName == string(".episodus:PickList"))                    return 0 ;
	if (sFctName == string(".episodus:AddDirect"))                   return 0 ;
	if (sFctName == string(".episodus:NewReport"))                   return CM_NEWCR ;
	if (sFctName == string(".episodus:NewExam"))                     return CM_NEWCS ;
	if (sFctName == string(".episodus:NewNote"))                     return CM_NEWCONSULT ;
	if (sFctName == string(".episodus:NewText"))                     return CM_NEWTTXT ;
	if (sFctName == string(".episodus:NewElaboratedText"))           return CM_NEWTMPL ;
	if (sFctName == string(".episodus:NewForm"))                     return CM_QUESTIONNAIRE ;
  if (sFctName == string(".episodus:NewCoopChart"))                return IDC_COOP_CHARTS ;
	if (sFctName == string(".episodus:OpenFromModel"))               return 0 ;
	if (sFctName == string(".episodus:OpenTextFromModel"))           return CM_OPENTTXT ;
	if (sFctName == string(".episodus:OpenElaboratedTextFromModel")) return CM_OPENTMPL ;
	if (sFctName == string(".episodus:Import"))                      return CM_IMPORTE ;
  if (sFctName == string(".episodus:ImportPDF"))                   return CM_IMPORT_PDF ;
  if (sFctName == string(".episodus:ImportHtml"))                  return CM_IMPORT_HTML ;
	if (sFctName == string(".episodus:ImportImages"))                return CM_IMPORTIMG ;
	if (sFctName == string(".episodus:ImagesPannel"))                return CM_PANNEL ;
	if (sFctName == string(".episodus:CloseDoc"))                    return CM_FILECLOSE ;
	if (sFctName == string(".episodus:Save"))                        return CM_ENREGISTRE ;
  if (sFctName == string(".episodus:QuickSave"))                   return CM_QUICKSAVE ;
	if (sFctName == string(".episodus:Compose"))                     return CM_COMPOSE ;
	if (sFctName == string(".episodus:Publish"))                     return CM_IMPRIME ;
	if (sFctName == string(".episodus:View"))                        return CM_VISUAL ;
	if (sFctName == string(".episodus:Leave"))                       return CM_EXIT ;
	if (sFctName == string(".episodus:Persons"))                     return 0 ;
	if (sFctName == string(".episodus:PatientOpen"))                 return CM_NEWPAT ;
  if (sFctName == string(".episodus:PatientPatientDemographics"))  return CM_ADMIN ;
  if (sFctName == string(".episodus:PatientPatientIdentifier"))    return CM_IDENTIFIER ;
  if (sFctName == string(".episodus:PatientWebExport"))            return CM_WEB_EXPORT ;
	if (sFctName == string(".episodus:PatientFile"))                 return CM_CHEMISE ;
	if (sFctName == string(".episodus:PatientClose"))                return CM_BUREAU ;
	if (sFctName == string(".episodus:UserSwitch"))                  return CM_NEWDOC ;
	if (sFctName == string(".episodus:UserNew"))                     return CM_CREATDOC ;
	if (sFctName == string(".episodus:UserModify"))                  return CM_MODIFDOC ;
	if (sFctName == string(".episodus:OuterNew"))                    return CM_CREATCOR ;
	if (sFctName == string(".episodus:OuterModify"))                 return CM_MODIFCOR ;
  if (sFctName == string(".episodus:TeamManagement"))              return CM_HEALTHTEAM ;
	if (sFctName == string(".episodus:TeamManagementRosace"))        return CM_HEALTHTEAM_ROSE ;
  if (sFctName == string(".episodus:TeamManagementList"))          return CM_HEALTHTEAM_LIST ;
  if (sFctName == string(".episodus:VenuesList"))                  return CM_VENUE_LIST ;
	if (sFctName == string(".episodus:PatientImport"))               return CM_IMPORT_PATIENT ;
	if (sFctName == string(".episodus:OuterImport"))                 return CM_IMPORT_CORRESP ;
	if (sFctName == string(".episodus:UserImport"))                  return CM_IMPORT_USER ;
  if (sFctName == string(".episodus:SynchroFromServer"))           return CM_SYNCHRO_IMP_THIS_PAT ;
  if (sFctName == string(".episodus:SynchrToServer"))              return CM_SYNCHRO_EXP_THIS_PAT ;
  if (sFctName == string(".episodus:Functions"))                   return 0 ;
  if (sFctName == string(".episodus:EditFunctionsProperties"))     return CM_FUNCTION ;
  if (sFctName == string(".episodus:StartPredi"))                  return CM_PREDI ;
  if (sFctName == string(".episodus:StartProject"))                return CM_PROJET ;
  if (sFctName == string(".episodus:CaptureMil"))                  return CM_CAPTUREIMG ;
  if (sFctName == string(".episodus:CaptureTwain"))                return CM_CAPTURETWAIN ;
  if (sFctName == string(".episodus:LdV"))                         return 0 ;
  if (sFctName == string(".episodus:ShowLdV"))                     return CM_LDV ;
  if (sFctName == string(".episodus:ShowDrugs"))                   return CM_LDV_DRUGS ;
  if (sFctName == string(".episodus:ShowProcess"))                 return CM_PROCESS ;
  if (sFctName == string(".episodus:ShowGoals"))                   return CM_LDV_GOALS ;
  if (sFctName == string(".episodus:ShowFollowUp"))                return CM_FOLLOW_UP ;
  if (sFctName == string(".episodus:ShowCurves"))                  return CM_SHOW_CURVES ;
  if (sFctName == string(".episodus:BuildPrometheusMessage"))      return CM_PROMET_MESSAGE ;
  if (sFctName == string(".episodus:SendPrometheusMessage"))       return CM_PROMET_SEND ;
  if (sFctName == string(".episodus:BuildDiogeneMessage"))         return CM_DIOGENE ;
  if (sFctName == string(".episodus:NavigateOnPrometServer"))      return CM_PROMET_SURF ;
  if (sFctName == string(".episodus:LdVZoom"))                     return CM_LDV_ZOOMIN ;
  if (sFctName == string(".episodus:LdVEnlarge"))                  return CM_LDV_ZOOMOUT ;
  if (sFctName == string(".episodus:Accounting"))                  return 0 ;
  if (sFctName == string(".episodus:AccountingPersonBilling"))     return CM_CREATCOMPTA ;
  if (sFctName == string(".episodus:AccountingPersonState"))       return CM_MODIFCOMPTA ;
  if (sFctName == string(".episodus:AccountingGlobal"))            return 0 ;
  if (sFctName == string(".episodus:AccountingReceiptEntry"))      return CM_RECETTE ;
  if (sFctName == string(".episodus:AccountingExpenditureEntry"))  return CM_DEPENSE ;
  if (sFctName == string(".episodus:AccountingReceiptList"))       return CM_LISTRECETTES ;
  if (sFctName == string(".episodus:AccountingExpenditureList"))   return CM_LISTDEPENSES ;
  if (sFctName == string(".episodus:AccountingBalanceSheet"))      return CM_2035 ;
  if (sFctName == string(".episodus:AccountingDailyReceipt"))      return CM_AGA ;
  if (sFctName == string(".episodus:AccountingOutstanding"))       return CM_IMPAYES ;
  if (sFctName == string(".episodus:AccountingActivityList"))      return CM_LISTACTES ;
  if (sFctName == string(".episodus:AccountingActivityCount"))     return CM_SOMACTES ;
  if (sFctName == string(".episodus:AccountingReceiptCount"))      return CM_SOMENCAISS ;
  if (sFctName == string(".episodus:AccountingThirdPartyPayers"))  return CM_SAISIETP ;
  if (sFctName == string(".episodus:AccountingClaimForm"))         return CM_FEUILLESOINS ;
  if (sFctName == string(".episodus:AccountingReimbursementIn"))   return CM_IMPORTNOEMIE ;
  if (sFctName == string(".episodus:AccountingDisposables"))       return CM_DISPOSABLES ;
  if (sFctName == string(".episodus:AccountingLocalActs"))         return CM_LOCAL_ACTS ;
  if (sFctName == string(".episodus:AccountingNgap"))              return CM_NGAP ;
  if (sFctName == string(".episodus:Tools"))                       return 0 ;
  if (sFctName == string(".episodus:EquipmentInterface"))          return CM_INTERFACE ;
  if (sFctName == string(".episodus:EquipmentManagement"))         return CM_MATERIELS ;
  if (sFctName == string(".episodus:FilsGuidesManagement"))        return CM_GROUPGD ;
  if (sFctName == string(".episodus:MulticriterionSearch"))        return 0 ;
 	if (sFctName == string(".episodus:MulticriterionSearchNew"))     return CM_REQUETE ;
  if (sFctName == string(".episodus:MulticriterionSearchResults")) return CM_RESULT ;
  if (sFctName == string(".episodus:SystemTools"))                 return CM_OUTILS ;
  if (sFctName == string(".episodus:EpisodusTools"))               return CM_EPISO_STOP ;
  if (sFctName == string(".episodus:EpisodusMailBox"))             return CM_MAILBOX ;
  if (sFctName == string(".episodus:ArchetypeReferencing"))        return CM_REFARCH ;
  if (sFctName == string(".episodus:ArchetypeEditing"))            return CM_ARCHEDIT ;
  if (sFctName == string(".episodus:ReferentialEditing"))          return CM_REFEDIT ;
  if (sFctName == string(".episodus:PublicationTemplateEditing"))  return CM_TPLEDIT ;
  if (sFctName == string(".episodus:Preferences"))                 return 0 ;
  if (sFctName == string(".episodus:PreferencesEpiParameters"))    return CM_EPISO_LEARN ;
  if (sFctName == string(".episodus:PreferencesMailParameters"))   return CM_EPISO_START ;
  if (sFctName == string(".episodus:LetterheadNew"))               return CM_NEW_ENTETE ;
  if (sFctName == string(".episodus:LetterheadModify"))            return CM_MOD_ENTETE ;
  if (sFctName == string(".episodus:CaptureParameters"))           return CM_EPISO_MODELS ;
  if (sFctName == string(".episodus:ParametersWinposSave"))        return CM_ENREG_WIN ;
  if (sFctName == string(".episodus:StationParameters"))           return 0 ;
  if (sFctName == string(".episodus:StationPrintingParameters"))   return CM_PARAM_PRINT ;
  if (sFctName == string(".episodus:StationInitParameters"))       return CM_PARAM_INIT ;
  if (sFctName == string(".episodus:StationUpdateParameters"))     return CM_PARAM_UPDATE ;
  if (sFctName == string(".episodus:StationBackupParameters"))     return CM_PARAM_BACKUP ;
  if (sFctName == string(".episodus:StationDirectoriesParameters"))return CM_PARAM_CHEMINS ;
  if (sFctName == string(".episodus:StationIconsParameters"))      return CM_PARAM_ICONS ;
  if (sFctName == string(".episodus:StationKSParameters"))         return CM_PARAM_KS ;
  if (sFctName == string(".episodus:StationModulesParameters"))    return CM_PARAM_MODULES ;
	if (sFctName == string(".episodus:StationPrediParameters"))      return CM_PARAM_PREDI ;
  if (sFctName == string(".episodus:PilotManagement"))             return CM_PILOT_AGENTS ;
  if (sFctName == string(".episodus:BackupStart"))                 return CM_SAUVEGARDE ;
  if (sFctName == string(".episodus:Windows"))                     return 0 ;
  if (sFctName == string(".episodus:WindowsCascadeChildren"))      return CM_CASCADECHILDREN ;
  if (sFctName == string(".episodus:WindowsTileChildren"))         return CM_TILECHILDREN ;
  if (sFctName == string(".episodus:WindowsArrangeIcons"))         return CM_ARRANGEICONS ;
  if (sFctName == string(".episodus:HelpMenu"))                    return 0 ;
  if (sFctName == string(".episodus:Help"))                        return CM_HELP ;
  if (sFctName == string(".episodus:About"))                       return CM_ABOUT ;
  if (sFctName == string(".episodus:PatientHistory"))              return 0 ;
  if (sFctName == string(".episodus:PatientHistorySeeEverything")) return CM_0P ;
  if (sFctName == string(".episodus:PatientHistorySeeLevel1"))     return CM_1P ;
  if (sFctName == string(".episodus:PatientHistorySeeLevel2"))     return CM_2P ;
  if (sFctName == string(".episodus:PatientHistorySeeLevel3"))     return CM_3P ;
  if (sFctName == string(".episodus:PatientHistorySeeLevel4"))     return CM_4P ;
  if (sFctName == string(".episodus:HTMLView"))                    return 0 ;
  if (sFctName == string(".episodus:HTMLViewEdit"))                return IDC_EDITION_HTM ;
  if (sFctName == string(".episodus:HTMLViewRefresh"))             return IDC_RAFRAICHIR_HTM ;
  if (sFctName == string(".episodus:HTMLViewPrevious"))            return CM_PRECEDENT ;
  if (sFctName == string(".episodus:HTMLViewNext"))                return CM_SUIVANT ;
  if (sFctName == string(".episodus:DocumentNew"))                 return CM_FILENEW ;
  if (sFctName == string(".episodus:DocumentModelOpen"))           return CM_LETTRETYPE ;
  if (sFctName == string(".episodus:Edit"))                        return 0 ;
  if (sFctName == string(".episodus:EditUndo"))                    return CM_EDITUNDO ;
  if (sFctName == string(".episodus:EditCut"))                     return CM_EDITCUT ;
  if (sFctName == string(".episodus:EditCopy"))                    return CM_EDITCOPY ;
  if (sFctName == string(".episodus:EditPaste"))                   return CM_EDITPASTE ;
  if (sFctName == string(".episodus:EditClear"))                   return CM_EDITCLEAR ;
  if (sFctName == string(".episodus:EditDelete"))                  return CM_EDITDELETE ;
  if (sFctName == string(".episodus:EditSearch"))                  return 0 ;
  if (sFctName == string(".episodus:EditFind"))                    return CM_EDITFIND ;
  if (sFctName == string(".episodus:EditReplace"))                 return CM_EDITREPLACE ;
  if (sFctName == string(".episodus:EditFindNext"))                return CM_EDITFINDNEXT ;
  if (sFctName == string(".episodus:EditFormat"))                  return 0 ;
  if (sFctName == string(".episodus:EditFormatChar"))              return 0 ;
  if (sFctName == string(".episodus:EditFormatCharBold"))          return CM_FORMATBOLD ;
  if (sFctName == string(".episodus:EditFormatCharItalic"))        return CM_FORMATITALIC ;
  if (sFctName == string(".episodus:EditFormatCharUnderline"))     return CM_FORMATUNDERLINE ;
  if (sFctName == string(".episodus:EditFormatCharStrikeOut"))     return CM_FORMATSTRIKEOUT ;
  if (sFctName == string(".episodus:EditFormatFont"))              return CM_FORMATFONT ;
  if (sFctName == string(".episodus:MenuDisplay"))                 return 0 ;
  if (sFctName == string(".episodus:MenuDisplayToolBar"))          return CM_VIEWTOOLBAR ;
  if (sFctName == string(".episodus:MenuDisplayStatusBar"))        return CM_VIEWSTATUSBAR ;
  if (sFctName == string(".episodus:MenuDisplayFormatBar"))        return CM_VIEWSTATUSBAR ;
  if (sFctName == string(".episodus:DrugManagement"))              return 0 ;
  if (sFctName == string(".episodus:DrugNew"))                     return CM_DRUG_NEW ;
  if (sFctName == string(".episodus:Referential"))                 return CM_REFERENTIAL ;
  if (sFctName == string(".episodus:DrugStop"))                    return CM_DRUG_STOP ;
  if (sFctName == string(".episodus:DrugDelete"))                  return CM_DRUG_DELETE ;
  if (sFctName == string(".episodus:DrugModify"))                  return CM_DRUG_CHANGE ;
  if (sFctName == string(".episodus:DrugRenew"))                   return CM_DRUG_RENEW ;
  if (sFctName == string(".episodus:DrugPrescription"))            return IDC_ORDONNANCE ;
  if (sFctName == string(".episodus:ProcessManagement"))           return 0 ;
  if (sFctName == string(".episodus:ProcessNew"))                  return CM_PROCESS_NEW ;
  if (sFctName == string(".episodus:ProcessStop"))                 return CM_PROCESS_STOP ;
  if (sFctName == string(".episodus:ProcessModify"))               return CM_PROCESS_CHANGE ;
  if (sFctName == string(".episodus:GoalManagement"))              return 0 ;
  if (sFctName == string(".episodus:GoalNew"))                     return CM_GOAL_NEW ;
  if (sFctName == string(".episodus:GoalStop"))                    return CM_GOAL_STOP ;
  if (sFctName == string(".episodus:GoalModify"))                  return CM_GOAL_CHANGE ;
  if (sFctName == string(".episodus:GoalValidate"))                return CM_GOAL_VALIDATE ;
  if (sFctName == string(".episodus:GoalInvalidate"))              return CM_GOAL_INVALIDATE ;
  if (sFctName == string(".episodus:EpisodeManagement"))           return 0 ;
  if (sFctName == string(".episodus:EpisodeNew"))                  return CM_EPISO_NEW ;
  if (sFctName == string(".episodus:EpisodeClose"))                return CM_EPISO_CLOSE ;
  if (sFctName == string(".episodus:EpisodeEvolve"))               return CM_EPISO_EVOLUER ;
  if (sFctName == string(".episodus:EpisodeNewAppointment"))       return CM_SET_RECONVOC ;
  if (sFctName == string(".episodus:EpisodeManageAppointments"))   return CM_RECONVOC ;
  if (sFctName == string(".episodus:TreesConclusion"))             return IDC_CONCLUSION_CR ;
  if (sFctName == string(".episodus:TreesCoding"))                 return IDC_CODAGE_CR ;
  if (sFctName == string(".episodus:TreesClassification"))         return IDC_CLASSIF ;
  if (sFctName == string(".episodus:TreesEncyclopedia"))           return IDC_ENCYCLOP ;
  if (sFctName == string(".episodus:TreesCertif"))                 return IDC_CERTIF ;
  if (sFctName == string(".episodus:ShowBlackboardInterface"))     return CM_BBK ;

  if (sFctName == string(".episodus:UserFunction1"))               return CM_USER_1 ;
  if (sFctName == string(".episodus:UserFunction2"))               return CM_USER_2 ;
  if (sFctName == string(".episodus:UserFunction3"))               return CM_USER_3 ;
  if (sFctName == string(".episodus:UserFunction4"))               return CM_USER_4 ;
  if (sFctName == string(".episodus:UserFunction5"))               return CM_USER_5 ;
  if (sFctName == string(".episodus:UserFunction6"))               return CM_USER_6 ;
  if (sFctName == string(".episodus:UserFunction7"))               return CM_USER_7 ;
  if (sFctName == string(".episodus:UserFunction8"))               return CM_USER_8 ;
  if (sFctName == string(".episodus:UserFunction9"))               return CM_USER_9 ;

  return 0 ;
}

NSUserPatRelation::NSUserPatRelation()
{
	reinit() ;
}

bool
NSUserPatRelation::isAdministred(int iAngle, int iDistance)
{
	for (int j = 0; j <= iDistance; j++)
  	if (adminPosit[iAngle][j] == 1)
    	return true ;

	return false ;
}

bool
NSUserPatRelation::isPresent(int iAngle, int iDistance)
{
	return (existPosit[iAngle][iDistance] == 1) ;
}

void
NSUserPatRelation::reinit()
{
	bPatientIsTheUser 			= false ;
    bUserActiveInHealthTeam 	= false ;
    bUserPresentInHealthTeam	= false ;
    bUserIsAdministrator 		= false ;
    bUserIsRootAdministrator 	= false ;

    for (int i = 0 ; i < 24 ; i++)
    {
    	for (int j = 0; j < 3; j++)
        {
        	adminPosit[i][j] = 0 ;
        	existPosit[i][j] = 0 ;
    	}
    }
}

void
NSUserPatRelation::init(NSContexte* /*pContexte*/)
{
/*
	reinit() ;

	string sUserId 	= pContexte->getUtilisateurID() ;
	NSPatientChoisi*	pPatChoisi = pContexte->getPatient() ;
	string sPatId	= pPatChoisi->pGraphPerson->getPersonID() ;

	bPatientIsTheUser = (sUserId == sPatId) ;


	NSHealthTeam*		pHealthTeam = pPatChoisi->pHealthTeam ;
	NSHealthTeamMember*	pMember = pHealthTeam->getUserAsMember(pContexte) ;
	if (!pMember)
		return ;

	bUserPresentInHealthTeam = true ;

	NSHTMMandateArray aMandates ;
	pMember->getActiveMandates(&aMandates) ;
	if (!(aMandates.empty()))
	{
		bUserActiveInHealthTeam = true ;

		NSHTMMandateIter iter = aMandates.begin() ;
		for (; iter != aMandates.end() ; )
		{
    	int						iAngle	= (*iter)->getAngle() ;
      DistanceType	iDist		= (*iter)->getDistance() ;

			NSHealthTeamMandate::mandateType iType	= (*iter)->getType() ;

			int iRayon = -1 ;
			switch(iDist)
			{
				case patDist 	: iRayon = 0 ; break ;
				case nearDist	: iRayon = 1 ; break ;
				case farDist 	: iRayon = 2 ; break ;
			}

			if (iRayon >= 0)
			{
				existPosit[iAngle][iRayon] = 1 ;
				if (iType == NSHealthTeamMandate::root)
				{
					adminPosit[iAngle][iRayon] = 1 ;
					bUserIsAdministrator = true ;
					if (iRayon == 0)
						bUserIsRootAdministrator = true ;
				}
			}
      aMandates.erase(iter) ;
		}
	}
*/
}

NSContexte::PERSONCLASSES
NSContexte::getPersonClass(string sPersonID)
{
	if (sPersonID == "")
  	return isUnknown ;

  switch(sPersonID[0])
  {
  	case '#' : return isMemory ;
    case '_' : return isLocal ;		// Historical value
    case '-' : return isLocal ;
  }

  return isGlobal ;
}

bool
NSContexte::reallocateBaseDirectory()
{
	return bool(::SetCurrentDirectory(_sBaseDirectory.c_str())) ;
}

string
NSContexte::getUserLanguage()
{
  if (NULL == _pUtilisateur)
    return string("") ;
  return _pUtilisateur->donneLang() ;
}

bool
NSContexte::userHasPrivilege(RIGHTSTO /*doThat*/, int /*i1*/, int /*i2*/, string /*s1*/, string /*s2*/, void* /*p1*/, void* /*p2*/)
{
	return true ;

/*

	string sPatId	= pPatient->pGraphPerson->getPersonID() ;
  PERSONCLASSES iPatClass = getPersonClass(sPatId) ;

  if (iPatClass == isLocal)
  {
  	switch(doThat)
    {
    	case modifyDocument :
      case suppresDocument :
        if (s1 == "$JustAccessToInterface$")
          return true ;
        return (s1 == getUtilisateurID()) ;
    }
    return true ;
  }
  else
  {
    switch(doThat)
    {
      case openPatient :
          return relationship.bUserPresentInHealthTeam ;
      //
      // Document opening : s1 = document author's ID
      //
      // - in general, must be present
      // - for a given document, if not active, must be the author
      //
      case openDocument :
        if (s1 == "$JustAccessToInterface$")
          return relationship.bUserPresentInHealthTeam ;
        return ((s1 == getUtilisateurID()) || relationship.bUserActiveInHealthTeam) ;
      //
      // Document modification : s1 = document author's ID
      //
      // - in general, must be active
      // - for a given document, must be the author
      //
      case modifyDocument :
      case suppresDocument :
        if (s1 == "$JustAccessToInterface$")
          return relationship.bUserActiveInHealthTeam ;
        return (s1 == getUtilisateurID()) ;
      case createDocument :      case modifyLdV :      case viewConcerns :      case createConcern :      case modifyConcern :      case viewDrug :      case createDrug :      case modifyDrug :      case viewGoal :      case createGoal :      case modifyGoal :      case viewProcess :      case createProcess :      case modifyProcess :      case viewSynthesis :      case modifySynthesis :      case viewAdminData :        return relationship.bUserActiveInHealthTeam ;      case modifyAdminData :        return relationship.bPatientIsTheUser ;      case viewProData :        return true ;      case modifyProData :        return relationship.bPatientIsTheUser ;      case viewHealthTeam :        return relationship.bUserActiveInHealthTeam ;      case createHTMember :      case createMandate :      case modifyMandate :        return relationship.bUserIsAdministrator ;      case createRosace :      case modifyRosace :        return true ;      case createUser :
        return false ;
    }
    return relationship.bUserActiveInHealthTeam ;
  }

*/
}

#endif // !_ENTERPRISE_DLL
#endif // !__NSSUPER_H__

