//---------------------------------------------------------------------------//     NSCHOISI.H
//
//  Contient les définitions des objets PATIENT et UTILISATEUR de NAUTILUS
//
//  1ere version : P.A. Octobre 1994
//---------------------------------------------------------------------------
#ifndef __NSCHOISI_H
# define __NSCHOISI_H

class NSHISTODocument ;
class NSCSDocument ;
class FicheAdminWindow ;
class FicheAdminDPIO ;
class NSDocCompta ;
class NSReqRefDocument ;
class NSHistoRefDocument ;
class NSSecuRefDocument ;
class NSRequeteDialog ;
class NSResultReqDialog ;
//class NSLdvTemplate ;
class NSLdvDocument ;
class NSLdvGoal ;
class AssistCaptureDialog ;
class NSHealthTeamDocument ;
class NSHealthTeam ;
class NSDocumentInfo ;
class VecteurString ;
class NSDocumentHisto ;
class NSVenuesDocument ;
class NSFrameInformation ;
class NSToDoTask ;
class NSCaptureArray ;
class NSDrugView ;
class NSProgressAndMessageDlg ;

# include <map># include <owl\gauge.h># include <owl\static.h># include "partage\ns_vector.h"# include "nsldv\nsldv_vars.h"# include "partage\nsdivfct.h"# include "partage\nsperson.h"
# include "nsbb\nsmview.h"
# include "nsbb\nsutidlg.h"
# include "nsldv\nsldvdoc_base.h"

#ifndef __NSBDMDRIVER_H
  class NsSelectableDrugArray ;
#endif

// # include "nautilus\nsanxary.h"
// # include "nautilus\nsdocref.h"		// pour l'importation
// # include "nautilus\nsldvtpl.h"
// # include "nssavoir\nsHealthTeam.h"
// # include "nautilus\nshistdo.h"
//
//---------------------------------------------------------------------------//  Classe NSPatientChoisi//      ANCETRE : NSPatient
//---------------------------------------------------------------------------
class NSPatientChoisi : public NSPatInfo
{
  enum INITPATSTEP  { ipsUndefined = -1, ipsStart, ipsBlockPat, ipsPurgeCapt,
                      ipsReinitBbk, ipsInitEvent, ipsContrib, ipsHistory,
                      ipsHeathTeam, ipsHistoryView, ipsSynthesys,
                      ipsDemographics, ipsCorreps, ipsPostInit, ipsLDV,
                      ipsOtherViews, ipsOpenLDV, ipsRC, ipsPredi, ipsMailbox,
                      ipsUpdateFromCapture, ipsFinal } ;

  public:

    enum OPENPATNEXT  { opnUndefined = -1, opnDoNothing, opnOpenReqResultDocum, opnUpdateFromCapture } ;

	  // variables
	  bool                 _bCanCloseHisto ;

    // NSHISTODocument		*pDocHis ;
    NSCSDocument			   *_pSynthese ;
    NSCSDocument			   *_pPOMRindex ;
    FicheAdminWindow	   *_pFicheAdmin ;
    FicheAdminDPIO		   *_pFicheAdminDPIO ;
    NSSecuRefDocument	   *_pDocSecu ;

    NSHealthTeamDocument *_pHealthDoc ;
    NSVenuesDocument     *_pVenuesDoc ;

    NSCSDocument			   *_pConsultEnCours ;
    string                _sConsultEnCours ;

    VectString            _aFilesInScansDir ;
    VectString            _aFilesInImagesDir ;

// #ifndef __EPIPUMP__
//	NSLdvTemplate			*pLdVTemplate ;
//#endif

    // constructors/destructor
    NSPatientChoisi(NSContexte *pCtx) : NSPatInfo(pCtx)
    {
      initialize() ;
    }

    NSPatientChoisi(NSPatInfo	*pPat) : NSPatInfo(*pPat)
    {
      initialize() ;    }

    ~NSPatientChoisi() ;

    NSPatientChoisi&	operator=(const NSPatInfo& src) ;

    void initialize()
    {
      _pDocHis 	    	 = (NSHISTODocument*) 0 ;
      _pHealthDoc      = (NSHealthTeamDocument*) 0 ;
      _pVenuesDoc      = (NSVenuesDocument*) 0 ;
      _pSynthese       = (NSCSDocument*) 0 ;
      _pPOMRindex      = (NSCSDocument*) 0 ;
      _pFicheAdmin     = (FicheAdminWindow*) 0 ;
      _pFicheAdminDPIO = (FicheAdminDPIO*) 0 ;
      _pDocSecu        = (NSSecuRefDocument*) 0 ;
      _sContribution	 = string("") ;

      _pConsultEnCours = (NSCSDocument*) 0 ;
      _sConsultEnCours = string("") ;

      _pDocLdv         = (NSLdvDocument*) 0 ;

      _bCanCloseHisto  = false ;
      _bOpeningAdm     = false ;
      _bIsOpen         = false ;
      _bIsClosing      = false ;

      _iCurrentOpeningStep = ipsUndefined ;
      _progressDialog      = (NSProgressAndMessageDlg*) 0 ;
    }

    // methods

    NSLdvDocument*    getLdvDocument() { return _pDocLdv ; }

    bool 							etablirLiensTree(bool& bCreerMetaLien, string sNodeMeta, string sNodeData) ;

    void            	InitialisationSynchronous() ;

    void              InitialisationAsynchronous(OPENPATNEXT iWhatToDoNext = opnDoNothing) ;
    void              InitialisationNextStep() ;
    void              InitialisationContinue() ;
    void              InitialisationStart() ;
    void              InitialisationBlock() ;
    void              InitialisationPurgeCapture() ;
    void              InitialisationReinitBbk() ;
    void              InitialisationAddInitEvent() ;
    void              InitialisationCreateContribution() ;
    void              InitialisationCreateHistory() ;
    void              InitialisationCreateHealthTeam() ;
    void              InitialisationOpenHistoryVue() ;
    void              InitialisationOpenSynthesys() ;
    void              InitialisationUpdateFromCapture() ;
    void              InitialisationInitDemographics() ;
    void              InitialisationInitCorresps() ;
    void              InitialisationPostInitialisation() ;
    void              InitialisationCreateLdVDocument() ;
    void              InitialisationOpenOtherVues() ;
    void              InitialisationOpenLdVVue() ;
    void              InitialisationRCVue() ;
    void              InitialisationPredi() ;
    void              InitialisationAdaptMailBox() ;
    void              InitialisationEnd() ;

    void            	PostInitialisation() ;
    bool              isOpen()    { return _bIsOpen ; }
    bool              isClosing() { return _bIsClosing ; }
    void              switchToClosingStatus() { _bIsClosing = true ; }
    void            	LdV_init() ;
    void              loadFramesDocuments() ;
    bool              updateIndexDocument(NSDocumentHisto* pDocument, bool bUpdateDisplay) ;
    void            	LdV_show		(string sConcern = string("")) ;
    void            	drugs_show	(string sConcern = string("")) ;
    void            	goals_show	(string sConcern = string("")) ;
    void            	process_show(string sConcern = string("")) ;
    void            	followUp_show(string sConcern = string("")) ;
    void            	curves_show(string sConcern = string("")) ;
    void            	admin_show() ;
    void            	histo_show() ;
    void              heathTeamRosaceShow() ;
    void              heathTeamListShow() ;
    void              venuesListShow() ;
    void              CmWebExport() ;
    void            	CmChemise() ;
    void            	CmNewTTxt() ;
    void            	CmNewTmpl() ;
    void            	CmOpenTTxt() ;
    void            	CmOpenPdf() ;
    void            	CmOpenHtml() ;
    void            	CmOpenTmpl() ;
    void            	CmNewCr() ;
    void            	CmNewCoopChart() ;
    void            	CmNewArchetype() ;
    void            	CmDeviceInterface() ;
    void            	CmNewConsult(bool bIsDocCS = true) ;
    void            	CmAssistCapture() ;    void            	CmTwainCapture() ;    void            	CmCapture(AssistCaptureDialog *pAssist = (AssistCaptureDialog*) 0) ;
    void            	CmImporter() ;
    void            	CmImportImg(AssistCaptureDialog *pAssist = (AssistCaptureDialog*) 0) ;
    void            	CmGenerePannel() ;
    void            	CmFeuilleSecu() ;
    void            	CmGenericReferentiel(string sRefId, string sDocument = string(""), string sConcern = string("")) ;
    void            	CmReferentiel(string sRefId, string sDocument, LDVFRAME iFrame) ;

    void            	Importer(string sFileName, bool bAutomaticMode = false) ;
    void            	DetruireDocument(NSDocumentInfo *pDocInfo) ;
    void              DocumentXmlExport(NSDocumentHisto *pDocHisto) ;
    // void            	ChargeDonneesPatient(NSPatPathoArray *pPatPathoArray) ; // now in nsperson.h
    void            	PatientDataChanged(NSPatPathoArray *pPatPathoArray) ;

    bool            	CreeObjectif(NSPatPathoArray *pPatPathoCree, string sDocument, string sRefId, string sGoalId, NSPatPathoArray *pPatPathoMotif, LDVFRAME iFrame, bool bSaveNow, string sGoal = string("")) ;
    bool            	SaveGoalsBatch(LDVFRAME iFrame) ;
    bool            	SaveGoalsBatch(NSDocumentHisto* pDocument, LDVFRAME iFrame) ;
    bool            	UpdateLdvForNewGoal(LDVFRAME iFrame, NSPatPathoArray *pFramePatPatho, int iGoalLine, int iGoalCol) ;
    void            	RegleObjectif(NSLdvGoal *pGoal) ;
    bool            	RefuseObjectif(NSPatPathoArray *pPatPathoCree, string sDocument, string sRefId, NSPatPathoArray *pPatPathoMotif, LDVFRAME iFrame) ;
    bool            	CreeElementFromRef(string sDestinationPath, NSPatPathoArray *pPatPathoCree, string sDocument, string sRefId, string sProposalId, NSPatPathoArray *pPatPathoMotif, string sConcern = string("")) ;
    bool            	CreeTraitement(NSPatPathoArray *pPatPatho, VecteurString* pRelatedConcerns = NULL) ;
    bool            	ArreterElement(VecteurString *pNodeArret, string sDateFin, bool bSave = true) ;
    bool            	RenouvelerTraitement(VecteurString *pNodeMedic, NSPatPathoArray *pPPT) ;
    bool            	SupprimerElement(string sNodeArret, bool bAvecTransa = true) ;
    bool            	SwitchElementLexicon(ArrayLdvDrugs* pModifiedDrugs) ;

    bool            	CreeOrdonnance(bool bGereALD, bool bForceSecured) ;
    bool            	CreeOrdonnanceFromSelection(bool bGereALD, VecteurString* pDrugsRefs, bool bDCI = false, bool bForceSecured = false) ;
    bool              initNewPrescriptionArrays(NSPatPathoArray* pPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD) ;
    bool              initRenewPrescriptionArrays(NSPatPathoArray* pPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD) ;
    bool              buildPrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, bool bGereALD, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD, bool bDCI, bool bMustSecure) ;
    bool              buildBizonePrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, VecteurString* pNewTraitement, VecteurString* pNewLexique, VecteurString* pNewTrtALD, VecteurString* pNewLexALD, bool bDCI) ;
    bool              buildSimplePrescriptionPatpatho(NSFrameInformation* pFrameInfo, NSPatPathoArray* pPrescriPpt, VecteurString* pNewTraitement, VecteurString* pNewLexique, bool bDCI) ;
    void              getDrugsAllergiesPatpatho(NSPatPathoArray* pAllergiesPpt) ;

    bool            	addSubElement(string motherNode, NSPatPathoArray *pPPT, bool bSave = true) ;
    bool            	CreeRootDocs(NSPatPathoArray *pPatPathoAdmin) ;
    bool              CreeRootNode() ;
    bool              CreeRootIndex() ;
    bool              CreeRootIndexExtensions() ;
    bool              CreeRootSynthese() ;
    bool              CreeRootAdmin(NSPatPathoArray *pPatPathoAdmin) ;
    bool              CreeRootLibChem() ;
    bool              CreeRootLibIDs() ;
    bool              CreeRootHealthTeam(string sPersonId) ;
    bool              createIdentifiersLibrary(string sIpp) ;
    bool            	IsDocRoot(string sCodeDoc) ;
    bool            	LocalPatho(NSPatPathoArray *pPptModele, NSPatPathoArray *pPptInstance) ;
    string            getDateForEvent(string sEventCode) ;

    bool              updateDemographicsFromCapture(NSCaptureArray* pCapt) ;
    bool              updateVenuesFromCapture(NSCaptureArray* pCapt) ;

    void            	bloquer() ;
    void            	debloquer() ;

    NSDocumentHisto** getFrameIndex(LDVFRAME iFrame, bool bVerbose = true) ;
    // DocumentIter    	trouveIndexSante() ;
    NSDocumentHisto** findFrameIndex(string sRootCode, bool bVerbose = true) ;
    NSDocumentHisto** trouveIndexSante() ;
    NSDocumentHisto** findRiskIndex() ;
    NSDocumentHisto** findSocialIndex() ;

    NSDrugView*       getDrugView() ;

    void              remplaceTagsChemin(string& sHtml, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, map<string, string> *pTagsBuffer = (map<string, string>*) NULL, map<string, bool> *pCondBuffer = (map<string, bool>*) 0, HWND interfaceHandle = (HWND) 0) ;
    void              remplaceTagsIfElse(string& sHtml, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, map<string, bool> *pCondBuffer = (map<string, bool>*) NULL) ;
    void              remplaceOrdinaryTags(string& sHtml, bool bAvecPrecoche, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, map<string, string> *pTagsBuffer = (map<string, string>*) NULL, HWND interfaceHandle = (HWND) 0) ;
    void              remplaceFreeLabel(string& sHtml, string sChemin, size_t *pPos1, size_t *pPos2) ;
    void              remplaceStandardLabel(string& sHtml, string sTypeTag, string sModifiers, bool bAvecPrecoche, string sChemin, size_t *pPos1, size_t *pPos2, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, map<string, string> *pTagsBuffer = (map<string, string>*) NULL, HWND interfaceHandle = (HWND) 0) ;
    string            getTreeAsHtml(string sPath, NSPatPathoArray* pPatPathoArray) ;
    int               getLogicalStringValue(string sFormula, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL) ;
    bool              resolveComparison(string sLValue, string sOperator, string sRValue, string *psErrorMsg, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL) ;
    double            getValueForPath(string sPath, string sNeededUnit, string sModifiers, string sConcept, string *psErrorMsg, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, HWND interfaceHandle = 0) ;
    bool              resolveExistsComparison(string sLValue, string *psErrorMsg, NSPatPathoArray* pPatPathoArray = (NSPatPathoArray*) NULL, HWND interfaceHandle = (HWND) 0) ;
    bool              initMapForNumericalExpression(string *psFormula, map<string, string> *paAliases, string *psErrorMsg) ;
    // void          	  DonnePathoSynthese(NSPatPathoArray *pPathoSynthese)  ; /* Funtion which fill a patpaho with synthese */
    // void          	  DonnePathoIndex(NSPatPathoArray *pPathoSynthese)  ;    /* Funtion which fill a patpaho with index */
    // bool            	ChercheChemin(string sChemin, NSSearchStruct *pSearchStruct, bool reinit_structure = false) ;
    // bool            	DonneArray(string sNoeud, NSPatPathoArray *pPPT) ;
    // bool            	DonneSousArray(string sNoeud, NSPatPathoArray *pPPT) ;

    bool              updateIPPEnCours(string sSite, string sNewIpp) ;

  protected:

    NSLdvDocument	       *_pDocLdv ;
    bool                  _bIsOpen ;
    bool                  _bIsClosing ;

    bool                  _bOpeningAdm ;

    INITPATSTEP              _iCurrentOpeningStep ;
    OPENPATNEXT              _iWhatToDoAfterOpening ;
    NSProgressAndMessageDlg *_progressDialog ;
} ;

//---------------------------------------------------------------------------
//  Classe NSMailParams : pour envoyer et recevoir des e-mail
//---------------------------------------------------------------------------
#define NSMAIL_NAUTILUS 0  /* Envoi de mails standard Nautilus */
#define NSMAIL_MAPI     1  /* Envoi de mails par MAPI          */

class NSMailParams
{
  public:

    NSMailParams() ;
    ~NSMailParams() {}

    int		  getSendingModality() { return _iTypeEnvoi ; }

    string  getUrlSmtp()         { return _sUrlSMTP ; }
    int     getPortSmtp()        { return _iPortSMTP ; }

    string  getUserSmtp()        { return _sUserSMTP ; }
    string  getPassSmtp()        { return _sPassSMTP ; }
    bool    isPassNeeded()       { return _bUseAuthentification ; }

    string  getName()            { return _sNameFrom ; }
    string  getSender()          { return _sMailSender ; }
    string  getFrom()            { return _sMailFrom ; }
    string  getReplyTo()         { return _sMailReplyTo ; }

    string  getUrlPop3()         { return _sUrlPOP3 ; }
    int     getPortPop3()        { return _iPortPOP3 ; }
    string  getUserPop3()        { return _sUserPOP3 ; }
    string  getPassPop3()        { return _sPassPOP3 ; }

    string  getApiKeyDir()       { return _sApiKeysDir ;  }
    string  getApiUser()         { return _sApiUser ;     }
    bool    mustUseApiProxy()    { return _bUseApiProxy ; }

    void	  setSendingModality(int iSM) { _iTypeEnvoi = iSM ; }

    void    setUrlSmtp(string sUS)      { _sUrlSMTP  = sUS ; }
    void    setPortSmtp(int iPS)        { _iPortSMTP = iPS ; }

    void    setUserSmtp(string sUS)     { _sUserSMTP = sUS ; }
    void    setPassSmtp(string sPS)     { _sPassSMTP = sPS ; }
    void    mustAuthenticate(bool bA)   { _bUseAuthentification = bA ; }

    void    setName(string sN)          { _sNameFrom = sN ; }
    void    setSender(string sS)        { _sMailSender = sS ; }
    void    setFrom(string sF)          { _sMailFrom = sF ; }
    void    setReplyTo(string sF)       { _sMailReplyTo = sF ; }

    void    setUrlPop3(string sUP)      { _sUrlPOP3  = sUP ; }
    void    setPortPop3(int iPP)        { _iPortPOP3 = iPP ; }
    void    setUserPop3(string sUP)     { _sUserPOP3 = sUP ; }
    void    setPassPop3(string sPP)     { _sPassPOP3 = sPP ; }

    void    setApiKeyDir(string sAKD)   { _sApiKeysDir = sAKD ;  }
    void    setApiUser(string sAU)      { _sApiUser = sAU ;      }
    void    setUseApiProxy(bool bUAP)   { _bUseApiProxy = bUAP ; }

  protected:

		int		  _iTypeEnvoi ;

    string  _sUrlSMTP ;
    int     _iPortSMTP ;

    string  _sUserSMTP ;
    string  _sPassSMTP ;
    bool    _bUseAuthentification ;

    string  _sNameFrom ;      
    string  _sMailFrom ;      // From
    string  _sMailSender ;    // Sender (used if sender is not the author)
    string  _sMailReplyTo ;

    string  _sUrlPOP3 ;
    int     _iPortPOP3 ;
    string  _sUserPOP3 ;
    string  _sPassPOP3 ;

    string  _sApiKeysDir ;
    string  _sApiUser ;
    bool    _bUseApiProxy ;
};

//
// Dialog box objet used to wait for history to close
//
//
/*
class NSHistoryClosingDlg : public TWindow, public NSRoot
{
  public:

    NSHistoryClosingDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSHistoryClosingDlg() ;

    void SetupWindow() ;

    void EvTimer(uint timerId) ;

	DECLARE_RESPONSE_TABLE(NSHistoryClosingDlg) ;
};
*/

class NSBasicAttributeArray ;

//---------------------------------------------------------------------------
//  Classe NSUtilisateurChoisi
//---------------------------------------------------------------------------
class NSUtilisateurChoisi : public NSUtiliInfo
{
	public :

		NSUtilisateurChoisi(NSContexte* pCtx) ;
		// NSUtilisateurChoisi(NSContexte* pCtx, NSDataGraph *pDataGraph) ;
    ~NSUtilisateurChoisi() ;

    enum PATIENTSTATUS { psUndefined = -1, psOpening = 0, psClosing } ;
    enum CLOSEPATNEXT  { cpnUndefined = -1, cpnDoNothing, cpnOpenNextPat, cpnExecToDo, cpnOpenFromCapture } ;
    enum CLOSEPATSTEP  { cpsUndefined = -1, cpsStart, cpsCloseEpisodus, cpsClosePredi, cpsCloseHisto, cpsCloseHistoCheck, cpsCloseStrips, cpsGarbageCollector, cpsConnectIrresolvedLinks, cpsCloseContribution, cpsFinal } ;

    //
    // Pointeur sur le patient de recherche
    // Note : Dans MUE le patient devient un PIDS de recherche
    //
		NSPatInfo*          _pPatRech ;

		//
    // Pointeur sur les docs compta appelés par Nautilus.cpp
    //
    NSDocCompta*		    _pDocCompta ;
    NSHistoRefDocument* _pDocRefHisto ;
    NSMailParams*       _pMail ;
    NSMailParams*       _pMailApicrypt ;

    ArrayWinProp        _aWinProp ;
    bool                _bEnregWin ;

    void AppelPatient() ;
    bool OuvrePatient(string sNss, NSPatientChoisi::OPENPATNEXT iWhatToDoNext = NSPatientChoisi::opnDoNothing) ;
    bool OuvrePatient(string sNss, NSPatInfo *pTargetPat) ;
    bool ChangePatientChoisi() ;
    bool getAndRemoveIppFromPatpatho(NSPatPathoArray* pPpt, string* psIpp) ;

    bool fermePatEnCours(bool bWithSynchro = false) ;
    bool CanClosePatient() ;
    void ClosePatientAsynchronous(CLOSEPATNEXT iWhatToDoNext) ;
    void ClosePatientNextStep() ;
    void ClosePatientContinue() ;
    void ClosePatientStart() ;
    void ClosePatientCloseEpisodus() ;
    void ClosePatientClosePredi() ;
    void ClosePatientCloseHisto() ;
    void ClosePatientCloseHistoCheck() ;
    void ClosePatientCloseStrips() ;
    void ClosePatientGarbageCollect() ;
    void ClosePatientConnectIrresolvedLinks() ;
    void ClosePatientCloseContribution() ;
    void ClosePatientEnd() ;

    void CallPatientInSlaveMode() ;
    void AutomaticOpenOrCreate(NSPatInfo* pTargetPat) ;
    bool OpenPatientInSlaveMode(string sPatID) ;
    bool UpdatePatientInSlaveMode() ;
    bool CreatePatientInSlaveMode() ;

    bool initMail() ;
    bool initSpecificMail(string sFileName, NSMailParams* pMailParams) ;
    void CmRequete() ;
    void CmResult() ;
    void CmEditRef() ;
    void CmEditArchetype() ;
    void CmEditTemplate() ;
    void CmNewEnTete() ;
    void CmModEnTete() ;
    void CmRefArchetype() ;
    // bool DetruitPatient(); (reporté à plus tard)

    void debloqueConsole() ;
    bool nouveauCR(string* sCompteRendu) ;
    void RemplirIndex(NSPatPathoArray* pPPT) ;
    void RemplirSynthese(NSPatPathoArray* pPPT) ;
    void RemplirIdentifiant(NSPatPathoArray* pPPT) ;
    void RemplirLibChem(NSPatPathoArray* pPPT) ;
		bool createPatient(NSPatPathoArray* pPatPathoAdmin) ;
    bool createCorrespondant(NSCorrespondantInfo* pNewCorresp) ;
    bool importPatientLdv() ;
    bool importCorrespLdv() ;
    bool importUserLdv() ;
    bool synchroImportThisPat(string sGlobalLogin, string sGlobalPasswd) ;
    bool synchroExportThisPat(string sGlobalLogin, string sGlobalPasswd) ;

    NSPatInfo* getPatientForIpp(string sIPP) ;

		// Creation / modification de l'utilisateur en cours
    bool InitNewData(NSUtiliInfo* pUtilInfo, bool bCreer, TWindow* parent = 0) ;
    bool Creer(TWindow* parent = (TWindow*) 0) ;
    bool Modifier(TWindow* parent = (TWindow*) 0) ;
		bool SetGraphUtil(NSUtiliInfo* pUtilInfo, bool bCreer) ;
    bool GetGraphUtil(NSUtiliInfo* pUtilInfo) ;
		/*Role = "U" Like user
      Role = "A" Like administrater
      Role = "UA" Like user + administrater */
		void SetUtilisateurChoisi(NSBasicAttributeArray *pAttrArray) ;
    bool ModifierAutreUtilisateur(TWindow* parent = (TWindow*) 0) ;

    bool OpenUserFile(ifstream* pInFile, string sFileName, string sShortFileName, string sPersoDir) ;
    bool OpenUserFile(ofstream* pInFile, string sFileName, string sShortFileName, string sPersoDir) ;

    void CreateUserFiles(string sUserID = string("")) ;
    void RenameUserFilesDirectory(string sPreviousID, string sNewID = string("")) ;

    string getVirtualDrug(const string sSpeciality) ;

		void setupToolbar() ;
    int  getIdFromString(string sTextId) ;

		inline std::string  getGlobalLoginSession() 	 { return _global_login_session ; }
    inline std::string  getGlobalPasswordSession() { return _global_password_session ; }
    NSPatInfo*          getPatRech()               { return _pPatRech ; }
    NSResultReqDialog*  getResult()                { return _pResult ; }

    NSMailParams*       getRegularMail()           { return _pMail ; }
    NSMailParams*       getMailApicrypt()          { return _pMailApicrypt ; }

    NSHistoRefDocument* getDocRefHisto()           { return _pDocRefHisto ; }
    void                setDocRefHisto(NSHistoRefDocument* pHRD) { _pDocRefHisto = pHRD ; }

    NSRequeteDialog*    getRequeteDialog()         { return _pRequete ; }
    void                setRequeteDialog(NSRequeteDialog* pRD)   { _pRequete = pRD ; }

    NSReqRefDocument*   getReqRefDoc()             { return _pReqRefDoc ; }
    void                setReqRefDoc(NSReqRefDocument* pRRD)     { _pReqRefDoc = pRRD ; }

    NSDocCompta*		    getDocCompta()             { return _pDocCompta ; }

    void setGlobalPassword(std::string pass) { _global_password_session = pass ;  }
    void setGlobalLogin(std::string login)   { _global_login_session = login ; }
    bool haveGlobalSessionPassword()         { return ((std::string("") != _global_login_session) && (std::string("") != _global_password_session)) ; }
    void setPatRech(NSPatInfo* pPat)         { _pPatRech = pPat ; }

    bool isEnregWin()          { return _bEnregWin ; }
    void setEnregWin(bool bEW) { _bEnregWin = bEW ;  }

    void setToDoAfterPatClose(NSToDoTask* pToDo) ;

    NSWindowProperty*          getWindowProperty(string sPropertyName)       { return _aWinProp.getProperty(sPropertyName) ; }
    NSSplittingWindowProperty* searchWindowProperty(string sMother, string sChild) { return _aWinProp.searchProperty(sMother, sChild) ; }
    void                       saveWindowProperty(string sUserId, string sPersoDirectory, NSWindowProperty *wndProp,  bool bWriteFile) { _aWinProp.saveWindowProperty(sUserId, sPersoDirectory, wndProp, bWriteFile) ; }
    ArrayWinProp*              getWindowPropertyArray() { return &_aWinProp ; }

	protected:

  	std::string   _global_login_session ;   			// login d'identification global de l'utilisateur
    std::string	  _global_password_session ;     // password encrypté global de l'utilisateur

    PATIENTSTATUS _iPatientStatus ;
    CLOSEPATNEXT  _iWhatToDoAfterClosing ;
    CLOSEPATSTEP  _iCurrentClosingStep ;
    NSToDoTask*   _pToDoAfterClosingPat ;

    NSProgressAndMessageDlg *_progressDialog ;

    NSRequeteDialog*    _pRequete ;
    NSResultReqDialog*  _pResult ;
    NSReqRefDocument*   _pReqRefDoc ;
};


/*
// -----------------------------------------------------------------------------
// NSPatientChoisi::ChercheChemin
// besoin dans les KSs donc placée dans .h
// -----------------------------------------------------------------------------

bool
NSPatientChoisi::ChercheChemin(string sChemin, NSSearchStruct* pSearchStruct, bool reinit_structure)
{	if ((!pDocHis) || (pDocHis->VectDocument.empty()) || (!pSearchStruct))		return false ;  if (true == reinit_structure)   // reinit the struct research    pSearchStruct->reinit();  DocumentIter        DocItEnd = pDocHis->VectDocument.end() ;    // Preparing the documents iterator end  std::string         sDateDoc ="";                               // Date du document  std::string         endDate  ="";                               // Date a laquel la recherche doit s'arreter  std::string sCheminPatho;  ComposeNoeudsChemin(sChemin, sCheminPatho);  //  // Recherche dans la synthèse  //  NSPatPathoArray patho_synthese(pContexte) ;   // Tempory patpath  DonnePathoSynthese(&patho_synthese) ;  PatPathoIter pptItBegin = patho_synthese.begin() ;  PatPathoIter pptIt      = patho_synthese.begin() ;   // Get an iterator	while (patho_synthese.CheminDansPatpatho(sCheminPatho, "|", &pptIt, &pptItBegin))	{    if ( (pptIt != NULL) && (pptIt != patho_synthese.end()))    {			std::string date = patho_synthese.getNodeDate(pptIt) ;			if  ((date == "") && (pSearchStruct->accept_date(sDateDoc)))			{				char				sCurrentDate[9] ;				donne_date_duJour(sCurrentDate) ;				sDateDoc = std::string(sCurrentDate) ;				pSearchStruct->aFoundNodes.insert(sDateDoc, std::string((*pptIt)->getNode())) ;			}			else				if (pSearchStruct->accept_date(date))					pSearchStruct->aFoundNodes.insert(date, std::string((*pptIt)->getNode())) ;		}		if ((pptIt == NULL) || (pptIt == patho_synthese.end()))			break ;		pptItBegin = pptIt++ ;		if (pptItBegin == patho_synthese.end())			break ;	}	//	// Recherche dans l'Index de santé - Searching in health index	//  NSPatPathoArray patho_index(pContexte);   // Tempory patpath  DonnePathoIndex(&patho_index) ;  pptItBegin = patho_index.begin() ;  // First case: the searched for pathway starts with ZPOMR -> standard search  //  if (string("ZPOMR") == string(sChemin, 0, 5))  {  	pptIt      = patho_index.begin() ;    // Get an iterator		while (patho_index.CheminDansPatpatho(sCheminPatho, "|", &pptIt, &pptItBegin))		{    	if ( (pptIt != NULL) && (pptIt != patho_index.end()))    	{				std::string date = patho_index.getNodeDate(pptIt);				if  ((date == "") && (pSearchStruct->accept_date(sDateDoc)))				{					char				sCurrentDate[9] ;					donne_date_duJour(sCurrentDate) ;					sDateDoc = std::string(sCurrentDate) ;					pSearchStruct->aFoundNodes.insert(sDateDoc, std::string((*pptIt)->getNode())) ;				}				else					if (pSearchStruct->accept_date(date))						pSearchStruct->aFoundNodes.insert(date, std::string((*pptIt)->getNode())) ;			}			if ((pptIt == NULL) || (pptIt == patho_index.end()))				break ;			pptItBegin = pptIt++ ;			if (pptItBegin == patho_index.end())				break ;		}  }  // Second case, the search is not oriented toward the health index, then take  // care of not searching in the "risk" or "goal" section unless explicitly  // specified  //  else  {    PatPathoIter pptItCol_1 = patho_index.ChercherPremierFils(pptItBegin) ;    bool bSearchInGoals = false ;    if ((strlen(sChemin.c_str()) > 5) && (string("0OBJE") == string(sChemin, 0, 5)))      bSearchInGoals = true ;    if ((strlen(sChemin.c_str()) > 11) && (string("ZPOMR|0OBJE") == string(sChemin, 0, 11)))      bSearchInGoals = true ;    bool bSearchInRisk = false ;    if ((strlen(sChemin.c_str()) > 5) && (string("ORISK") == string(sChemin, 0, 5)))      bSearchInRisk = true ;    if ((strlen(sChemin.c_str()) > 11) && (string("ZPOMR|ORISK") == string(sChemin, 0, 11)))      bSearchInRisk = true ;    // Just look in Goals and Risk if explicitely specified    while ((NULL != pptItCol_1) && (patho_index.end() != pptItCol_1))    {      string sChapter = (*pptItCol_1)->getLexiqueSens(pContexte) ;      if (((string("0OBJE") != sChapter) || bSearchInGoals) &&          ((string("ORISK") != sChapter) || bSearchInRisk))      {        pptItBegin = pptItCol_1 ;        while (patho_index.CheminDansPatpatho(sCheminPatho, "|", &pptIt, &pptItCol_1))        {          if ((pptIt != NULL) && (pptIt != patho_index.end()))          {            std::string date = patho_index.getNodeDate(pptIt);            if  ((date == "") && (pSearchStruct->accept_date(sDateDoc)))            {              char sCurrentDate[9] ;              donne_date_duJour(sCurrentDate) ;              sDateDoc = std::string(sCurrentDate) ;              pSearchStruct->aFoundNodes.insert(sDateDoc, std::string((*pptIt)->getNode())) ;            }            else              if (pSearchStruct->accept_date(date))                pSearchStruct->aFoundNodes.insert(date, std::string((*pptIt)->getNode())) ;          }          if ((pptIt == NULL) || (pptIt == patho_index.end()))            break ;          pptItBegin = pptIt++ ;          if ((pptItBegin == patho_index.end()) || ((*pptItBegin)->getColonne() == 1))            break ;        }      }      pptItCol_1 = patho_index.ChercherFrere(pptItCol_1) ;    }  }  //  // Recherche dans les autres documents  //	for (DocumentIter DocIt = pDocHis->VectDocument.begin(); (DocIt != DocItEnd); DocIt++)	{		sDateDoc = string((*DocIt)->dateDoc);  // Recupere la date du document		if ((pSearchStruct->sEndingDate != "") && (sDateDoc < pSearchStruct->sEndingDate)) // La date finale est passe			break ;		//LA seule facon d'etre sur quon a les n plus recents est de verifier qu'on a n reponse		// plus recents que le documents en cours		if (pSearchStruct->aFoundNodes.getNunberOfResultBeforeDate(sDateDoc) >= pSearchStruct->iNbNodes)   // si la date du document est superieur a la derniere date			break ;    std::string name = (*DocIt)->getContent() ;		if (((*DocIt)->pPatPathoArray) && (!((*DocIt)->pPatPathoArray->empty())) && (name != "ZSYNT1") && (name != "ZPOMR1"))		{			PatPathoIter pptIt      = (*DocIt)->pPatPathoArray->begin();			PatPathoIter pptItBegin = (*DocIt)->pPatPathoArray->begin();			while ((*DocIt)->pPatPathoArray->CheminDansPatpatho(sCheminPatho, "|", &pptIt, &pptItBegin))			{				std::string info_date = (*DocIt)->pPatPathoArray->getNodeDate(pptIt); // recupere l'information sur la date				if (info_date == "")					info_date =  sDateDoc ;				if ((pptIt != NULL) && (pptIt != (*DocIt)->pPatPathoArray->end()) && (pSearchStruct->accept_date(info_date)))					pSearchStruct->aFoundNodes.insert(sDateDoc, std::string((*pptIt)->getNode())) ;				if ((pptIt == NULL) || (pptIt == (*DocIt)->pPatPathoArray->end()))					break ;				pptItBegin = pptIt++ ;				if (pptItBegin == (*DocIt)->pPatPathoArray->end())					break ;			}		}		else if (((*DocIt)->pMeta) && (!((*DocIt)->pMeta->empty())))		{			// on vérifie au préalable qu'il existe un chapitre "données de suivi"      PatPathoIter pptItBegin = (*DocIt)->pMeta->ChercherItem("9SUIV") ;			PatPathoIter pptIt ;      if ((pptItBegin != (*DocIt)->pMeta->end()) && (pptItBegin != NULL))			{				while ((*DocIt)->pMeta->CheminDansPatpatho(sCheminPatho, "|", &pptIt, &pptItBegin))				{					std::string info_date = (*DocIt)->pMeta->getNodeDate(pptIt); // recupere l'information sur la date					if (info_date == "")						info_date =  sDateDoc ;					if ((pptIt != NULL) && (pptIt != (*DocIt)->pMeta->end()) && (pSearchStruct->accept_date(info_date)))						pSearchStruct->aFoundNodes.insert(sDateDoc, std::string((*pptIt)->getNode())) ;					if ((pptIt == NULL) || (pptIt == (*DocIt)->pMeta->end()))						break ;					pptItBegin = pptIt++ ;					if (pptItBegin == (*DocIt)->pMeta->end())						break ;				}			}		}	}	pSearchStruct->create_result() ;	return true ;}
// -----------------------------------------------------------------------------
// Renvoie la patpatho fille du noeud
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
NSPatientChoisi::DonneSousArray(string sNoeud, NSPatPathoArray *pPPT)
{  if (!pPPT)    return false ;	if ((!pDocHis) || (pDocHis->VectDocument.empty()))		return false ;  if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)    return false ;	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;	DocumentIter DocIt = pDocHis->VectDocument.begin() ;	for ( ; DocIt != pDocHis->VectDocument.end() ; DocIt++)	{		if (((*DocIt)->pPatPathoArray) && (!((*DocIt)->pPatPathoArray->empty())))		{			PatPathoIter pptIt = (*DocIt)->pPatPathoArray->begin() ;      if ((*pptIt)->getDoc() == sDocument)      {        for ( ; (pptIt != (*DocIt)->pPatPathoArray->end()) && (sNoeud != (*pptIt)->getNode()) ; pptIt++)          ;        if (pptIt != (*DocIt)->pPatPathoArray->end())        {          (*DocIt)->pPatPathoArray->ExtrairePatPatho(pptIt, pPPT) ;          return true ;        }      }      else      {        if (((*DocIt)->pMeta != NULL) && (!(*DocIt)->pMeta->empty()))        {          PatPathoIter pptMetaIter = (*DocIt)->pMeta->begin() ;          if ((*pptMetaIter)->getDoc() == sDocument)          {            for ( ; (pptMetaIter != (*DocIt)->pMeta->end()) && (sNoeud != (*pptMetaIter)->getNode()) ; pptMetaIter++)              ;            if (pptMetaIter != (*DocIt)->pMeta->end())            {              (*DocIt)->pMeta->ExtrairePatPatho(pptMetaIter, pPPT) ;              return true ;            }          }        }      }		}	}	return false ;}

// -----------------------------------------------------------------------------
// Renvoie une patpatho qui contient le noeud, sa fille et les même données
// pour les frères du noeud qui lui sont identiques
// -----------------------------------------------------------------------------
// ajout fab - 2003/08/13 -- on cherche également dans la patpatho META
// -----------------------------------------------------------------------------
bool
NSPatientChoisi::DonneArray(string sNoeud, NSPatPathoArray *pPPT)
{  if (!pPPT)    return false ;	if ((!pDocHis) || (pDocHis->VectDocument.empty()))		return false ;  if (strlen(sNoeud.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)    return false ;	string sDocument = string(sNoeud, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;	DocumentIter DocIt = pDocHis->VectDocument.begin() ;	for ( ; DocIt != pDocHis->VectDocument.end() ; DocIt++)	{		if (((*DocIt)->pPatPathoArray) && (!((*DocIt)->pPatPathoArray->empty())))		{			PatPathoIter pptIt = (*DocIt)->pPatPathoArray->begin() ;      if ((*pptIt)->getDoc() == sDocument)      {        for ( ; (pptIt != (*DocIt)->pPatPathoArray->end()) && (sNoeud != (*pptIt)->getNode()) ; pptIt++)          ;        if (pptIt != (*DocIt)->pPatPathoArray->end())        {          (*DocIt)->pPatPathoArray->ExtrairePatPathoFreres(pptIt, pPPT) ;          return true ;        }      }      else      {        if (((*DocIt)->pMeta != NULL) && (!(*DocIt)->pMeta->empty()))        {          PatPathoIter pptMetaIter = (*DocIt)->pMeta->begin() ;          if ((*pptMetaIter)->getDoc() == sDocument)          {            for ( ; (pptMetaIter != (*DocIt)->pMeta->end()) && (sNoeud != (*pptMetaIter)->getNode()) ; pptMetaIter++)              ;            if (pptMetaIter != (*DocIt)->pMeta->end())            {              (*DocIt)->pMeta->ExtrairePatPathoFreres(pptMetaIter, pPPT) ;              return true ;            }          }        }      }		}	}	return false ;}*/

bool
NSUtilisateurChoisi::OpenUserFile(ifstream* pInFile, string sFileName, string sShortFileName, string sPersoDir)
{
	if ((NULL == pInFile) || (sFileName == string("")) || (sPersoDir == string("")))
		return false ;

	// Try to open file in user directory
	//
	char cDirSeparator = sPersoDir[strlen(sPersoDir.c_str()) - 1] ;

	string sFile = sPersoDir + getID() + string(1, cDirSeparator) + sFileName ;
  pInFile->open(sFile.c_str()) ;

  if (*pInFile)
		return true ;

  // Try to open user specific file in model directory
  //
  if (string("") != sShortFileName)
  {
    sFile = sShortFileName ;
    size_t pos = sFile.find("[user]") ;
	  while (NPOS != pos)
	  {
      sFile.replace(pos, strlen("[user]"), getID()) ;
		  pos = sFile.find("[user]", pos + 1) ;
    }

    sFile = sPersoDir + sFile ;
    pInFile->open(sFile.c_str()) ;

    if (*pInFile)
		  return true ;
  }

	// Try to open file in model directory
	//
  sFile = sPersoDir + sFileName ;
  pInFile->open(sFile.c_str()) ;

  if (*pInFile)
		return true ;

	return false ;
}

bool
NSUtilisateurChoisi::OpenUserFile(ofstream* pOutFile, string sFileName, string sShortFileName, string sPersoDir)
{
	if ((NULL == pOutFile) || (sFileName == string("")) || (sPersoDir == string("")))
		return false ;

	// Try to open file in user directory
	//
	char cDirSeparator = sPersoDir[strlen(sPersoDir.c_str()) - 1] ;

	string sFile = sPersoDir + getID() + string(1, cDirSeparator) + sFileName ;
  pOutFile->open(sFile.c_str()) ;

  if (*pOutFile)
		return true ;

  // Try to open user specific file in model directory
  //
  if (string("") != sShortFileName)
  {
    sFile = sShortFileName ;
    size_t pos = sFile.find("[user]") ;
	  while (NPOS != pos)
	  {
      sFile.replace(pos, strlen("[user]"), getID()) ;
		  pos = sFile.find("[user]", pos + 1) ;
    }

    sFile = sPersoDir + sFile ;
    pOutFile->open(sFile.c_str()) ;

    if (*pOutFile)
		  return true ;
  }

	// Try to open file in model directory
	//
  sFile = sPersoDir + sFileName ;
  pOutFile->open(sFile.c_str()) ;

  if (*pOutFile)
		return true ;

	return false ;
}

//---------------------------------------------------------------------------
#endif    // __NSCHOISI_H
