////  NSEpiOut
//
#ifndef __NSEPIOUT_H
#define __NSEPIOUT_H
class NSUtilEditDate ;#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\radiobut.h>
#include <owl\edit.h>

#include "nsepisod\flechiesDB.h"
#include "nsbb\nsutidlg.h"
#include "nsbb\nsednum.h"
#include "nsoutil\nsexport.h"

//
// Objet "Boite de dialogue" utilis� pour s�lectionner l'utilitaire
//
class _NSOUTILCLASSE NsEpisodusOutilDialog : public NSUtilDialog
{
    public:

        OWL::TCheckBox*	pBuildFlechies ;
        OWL::TCheckBox*	pImportCISP ;
        OWL::TCheckBox*	pImportCIM ;
        OWL::TCheckBox*	pCodeList ;
        OWL::TCheckBox*	pClassCtrl ;

	    NsEpisodusOutilDialog(TWindow* pere, NSContexte* pCtx) ;
	    ~NsEpisodusOutilDialog() ;

	    void SetupWindow() ;

        void lanceBuildFlechies() ;
        void lanceImportCISP() ;
        void lanceImportCIM() ;
        void lanceCodeList() ;
        void gereClassCtrl() ;

    DECLARE_RESPONSE_TABLE(NsEpisodusOutilDialog);
};

//---------------------------------------------------------------
//---------------------------------------------------------------
class _NSOUTILCLASSE NSFlechiesBuildDlg : public NSUtilDialog
{
    public :

        OWL::TEdit*     pCurrentLex;
        OWL::TCheckBox* pEmpty;
        OWL::TCheckBox* pAddLexique;
        OWL::TCheckBox* pAddOrtho;
        OWL::TCheckBox* pAddExpress;
        OWL::TCheckBox* pAddLocal;

        bool            bTourner;

   	    NSFlechiesBuildDlg(TWindow* pere, NSContexte* pCtx);
   	    ~NSFlechiesBuildDlg();

   	    //void CmOk();
   	    //void CmCancel();
   	    void SetupWindow();

        void demarre();
        void stoppe();

        void videTable();
        void importeLexique();
        bool importeSpecifique(int iBase);
        bool importeOrtho()         { return importeSpecifique(NSFlechies::ortho); }
        bool importeExpressions()   { return importeSpecifique(NSFlechies::express); }
        bool importeLocal()         { return importeSpecifique(NSFlechies::local); }

    DECLARE_RESPONSE_TABLE(NSFlechiesBuildDlg);
};

// ***************************************************************************
//                            CLASSE PROMETHE
// ***************************************************************************
// Alexandre Belle - Mars 2002

class NSPrometheParamsDlg : public NSUtilDialog
{
	public:

    OWL::TEdit*         pUserId ;
    OWL::TEdit*         pMail ;
    OWL::TRadioButton*  pAutoFloculeNon ;
    OWL::TRadioButton*  pAutoFloculeOui ;
    OWL::TRadioButton*  pPrincipeCim ;
    OWL::TRadioButton*  pPrincipeCisp ;
    OWL::TRadioButton*  pSend_Now_Non ;
    OWL::TRadioButton*  pSend_Now_Oui ;
    OWL::TRadioButton*  pHash_Oui ;
    OWL::TRadioButton*  pHash_Non ;
    OWL::TRadioButton*  pBabylon_Oui ;
    OWL::TRadioButton*  pBabylon_Non ;
    OWL::TEdit*         pBabylon_Key ;
    OWL::TRadioButton*  pCapture_Oui ;
    OWL::TRadioButton*  pCapture_Non ;
    OWL::TEdit*         pCapture_Key ;
    OWL::TRadioButton*  pAutoSave_Oui ;
    OWL::TRadioButton*  pAutoSave_Non ;
    OWL::TEdit*         pLogin ;
    OWL::TEdit*         pUserPassword ;
    OWL::TEdit*         pServerURL ;
    OWL::TRadioButton*  pOpenMode_Heure ;
    OWL::TRadioButton*  pOpenMode_Jour ;
    OWL::TRadioButton*  pOpenMode_Semaine ;
    OWL::TRadioButton*  pOpenMode_Mois ;
    OWL::TRadioButton*  pAutoOpen_Oui ;
    OWL::TRadioButton*  pAutoOpen_Non ;
    OWL::TRadioButton*  pTLconcen_Oui ;
    OWL::TRadioButton*  pTLconcen_Non ;
    OWL::TEdit*         pServiceUrl ;
    OWL::TEdit*         pServiceTitle ;

    OWL::TStatic*   ptMail ;
    OWL::TStatic*   ptUserId ;
    OWL::TStatic*   ptServerUrl ;
    OWL::TStatic*   ptLogin ;
    OWL::TStatic*   ptPass ;
    OWL::TStatic*   ptServiceUrl ;
    OWL::TStatic*   ptServiceTitle ;
    OWL::TGroupBox* ptClassif ;
    OWL::TGroupBox* ptAutoFlocule ;
    OWL::TGroupBox* ptSendNow ;
    OWL::TGroupBox* ptHash ;
    OWL::TGroupBox* ptSavePos ;
    OWL::TGroupBox* ptGeneral ;
    OWL::TGroupBox* ptCapture ;
    OWL::TStatic*   ptCaptureKey ;
    OWL::TGroupBox* ptBabylon ;
    OWL::TStatic*   ptBabylonKey ;
    OWL::TGroupBox* ptPrincipe ;
    OWL::TGroupBox* ptLdV ;
    OWL::TGroupBox* ptOpenMode ;
    OWL::TGroupBox* ptAutoOpen ;
    OWL::TGroupBox* ptTlConcern ;

    bool*           pbCapture ;
    bool*           pbBabylon ;
    string*         psCaptureKey ;
    string*         psBabylonKey ;

    string*         psLogin ;
    string*         psPrincipe ;
    bool*           pbSendNow ;
    bool*           pbHash ;
    string*         psUserPassword ;
    string*         psUserId ;
    string*         psMailSeveur ;
    string*         psURLServeur ;
    bool*           pbAutoFlocule ;
    bool*           pbAutoSauve ;
    string*         psOpenMode ;
    bool*           pbAutoOpenLdv ;
    bool*           pbTLconcernLdv ;
    string*         psServiceUrl ;
    string*         psServiceTitle ;

    NSPrometheParamsDlg(TWindow* pere, NSContexte* pCtx, string* psLogin, string* psPrincipe,
                        bool* pbSendNow, bool* pbHash, string* psUserPassword,
                        string* psUserId, string* psMailSeveur,
                        bool* psAutoFlocule, string* psURLServeur,
                        bool* pbCapt, string* sCaptKey,
                        bool* pbBabyl, string* sBabylKey, bool* pbAutoSave,
                        string* psOpenMode, bool* pbAutoOpen, bool* pbTLcrn,
                        string* psServUrl, string* psServTitle) ;
    ~NSPrometheParamsDlg() ;


    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSPrometheParamsDlg) ;
};

// ***************************************************************************//                              NSPrometheFile
// ***************************************************************************
// Alexandre Belle - Mars 2002

class _NSOUTILCLASSE NSPrometheFile : public NSRoot
{
  public:

    NSPrometheFile(NSContexte* pCtx) ;
    ~NSPrometheFile() ;

    void Lancer() ;
    bool EcrireFichier(string sUserId) ;
};

//
// Objet "Boite de dialogue" utilis� pour s�lectionner l'utilitaire
//
class _NSOUTILCLASSE NsMailToolsDialog : public NSUtilDialog
{
  public:

    OWL::TButton* pMail ;
    OWL::TButton* pApi ;

    NsMailToolsDialog(TWindow* pere, NSContexte* pCtx) ;
    ~NsMailToolsDialog() ;

    void SetupWindow() ;

    void CmOk();

    void startMailEdit() ;
    void startApicryptEdit() ;

  DECLARE_RESPONSE_TABLE(NsMailToolsDialog) ;
} ;

/*********************************************************************************************************************/
                                   /* NSEmailFile */
/**********************************************************************************************************************/
class _NSOUTILCLASSE NSEmailFile : public NSRoot
{
  public:

    NSEmailFile(NSContexte* pCtx) ;
    ~NSEmailFile() ;

    void Lancer(TWindow* pere = 0) ;    bool EcrireFichier() ;
};

/*********************************************************************************************************************/
                                   /* NSEmail_DatFile */
/**********************************************************************************************************************/
class  NSEMAIL_DAT_FileDLG : public TDialog
{
  public:

    OWL::TEdit*         pUrlSMTP ;
    OWL::TEdit*         pPortSMTP ;
    OWL::TEdit*         pUserSMTP ;
    OWL::TEdit*         pPassSMTP ;
    OWL::TRadioButton*  pPassNo ;
    OWL::TRadioButton*  pPassYes ;
    OWL::TEdit*         pNameFrom ;
    OWL::TEdit*         pMailFrom ;
    OWL::TEdit*         pMailReplyTo ;
    OWL::TEdit*         pUrlPOP3 ;
    OWL::TEdit*         pPortPOP3 ;
    OWL::TEdit*         pUserPOP3 ;
    OWL::TEdit*         pPassPOP3 ;
    OWL::TRadioButton*  pClassique ;
    OWL::TRadioButton*  pMAPI ;

    string*     psUrlSMTP ;
    int*        piPortSMTP ;
    string*     psUserSMTP ;
    string*     psPassSMTP ;
    bool*       pbUsePass ;
    string*     psNameFrom ;
    string*     psMailFrom ;
    string*     psMailReplyTo ;
    string*     psUrlPOP3 ;
    int*        piPortPOP3 ;
    string*     psUserPOP3 ;    string*     psPassPOP3 ;
    int*        piSendMode ;

    NSEMAIL_DAT_FileDLG(TWindow* pere, string*  psUrl, int* piPort,
                        string*  psUser, string*  psMail, string*  psUrlPOP,
                        int* piPortPOP, string*  psUserPOP, string* psPassPOP,
                        int* piMethode, string* psPass, string* psReplyTo,
                        string* psName, bool* pbNeedPass) ;
    ~NSEMAIL_DAT_FileDLG() ;

    void CmCancel() ;    void CmOk() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSEMAIL_DAT_FileDLG) ;
};

class NSEMAILSMTP_DAT_FileDLG : public NSUtilDialog
{
  public:

    OWL::TEdit*         pUrlSMTP ;
    OWL::TEdit*         pPortSMTP ;
    OWL::TEdit*         pUserSMTP ;
    OWL::TEdit*         pPassSMTP ;
    OWL::TEdit*         pNameFrom ;
    OWL::TEdit*         pMailFrom ;
    OWL::TEdit*         pMailReplyTo ;
    OWL::TRadioButton*  pClassique ;
    OWL::TRadioButton*  pMAPI ;
    OWL::TRadioButton*  pPassNo ;
    OWL::TRadioButton*  pPassYes ;

    string*     psUrlSMTP ;
    int*        piPortSMTP ;
    string*     psUserSMTP ;
    string*     psPassSMTP ;
    bool*       pbUsePass ;
    string*     psNameFrom ;
    string*     psMailFrom ;
    string*     psMailReplyTo ;
    int*        piSendMode ;

    NSEMAILSMTP_DAT_FileDLG(TWindow* pere, NSContexte* pCtx, string* psUrl,
                            int* piPort, string* psUser, string* psMail,
                            int* piMethode, string* psPass, string* psReplyTo,
                            string* psName, bool* pbNeedPass,
                            TResId resID = "IDD_EMAIL_SMTP_DLG") ;
    virtual ~NSEMAILSMTP_DAT_FileDLG() ;

    void CmCancel() ;    void CmOk() ;
    void SetupWindow() ;

    void setMailParams() ;

  DECLARE_RESPONSE_TABLE(NSEMAILSMTP_DAT_FileDLG) ;
};

/*********************************************************************************************************************/
                                   /* NSApiEmailFile */
/**********************************************************************************************************************/
class _NSOUTILCLASSE NSApiEmailFile : public NSRoot
{
  public:

    NSApiEmailFile(NSContexte* pCtx) ;
    ~NSApiEmailFile() ;

    void Lancer(TWindow* pere = 0) ;    bool EcrireFichier() ;
};

class  NSAPIEMAIL_DAT_FileDLG : public TDialog
{
  public:

    OWL::TEdit*         pApiUser ;
    OWL::TEdit*         pApiKeyDir ;
    OWL::TEdit*         pUrlSMTP ;
    OWL::TEdit*         pPortSMTP ;
    OWL::TEdit*         pUserSMTP ;
    OWL::TEdit*         pPassSMTP ;
    OWL::TEdit*         pMailFrom ;
    OWL::TEdit*         pMailReplyTo ;
    OWL::TEdit*         pUrlPOP3 ;
    OWL::TEdit*         pPortPOP3 ;
    OWL::TEdit*         pUserPOP3 ;
    OWL::TEdit*         pPassPOP3 ;
    OWL::TRadioButton*  pClassique ;
    OWL::TRadioButton*  pMAPI ;
    OWL::TRadioButton*  pPassNo ;
    OWL::TRadioButton*  pPassYes ;

    string*     psUrlSMTP ;
    int*        piPortSMTP ;
    string*     psUserSMTP ;
    string*     psPassSMTP ;
    bool*       pbUsePassSMTP ;
    string*     psMailFrom ;
    string*     psMailReplyTo ;
    string*     psUrlPOP3 ;
    int*        piPortPOP3 ;
    string*     psUserPOP3 ;    string*     psPassPOP3 ;
    int*        piSendMode ;
    string*     psApicryptUser ;
    string*     psApicryptKeyDir ;

    NSAPIEMAIL_DAT_FileDLG(TWindow* pere, string*  psUrl, int* piPort,
                        string*  psUser, string*  psMail, string*  psUrlPOP,
                        int* piPortPOP, string*  psUserPOP, string* psPassPOP,
                        int* piMethode, string* psApiUser, string* psApiKeyDir,
                        string* psPass, string* psFrom, bool* pbNeedPass) ;
    ~NSAPIEMAIL_DAT_FileDLG() ;

    void CmCancel() ;    void CmOk() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSAPIEMAIL_DAT_FileDLG) ;
};

class NSAPIEMAILSMTP_DAT_FileDLG : public NSEMAILSMTP_DAT_FileDLG
{
  public:

    OWL::TEdit*     _pApiUser ;
    OWL::TEdit*     _pApiKeyDir ;
    OWL::TCheckBox* _pBehindProxy ;

    string*         _psApicryptUser ;
    string*         _psApicryptKeyDir ;
    bool*           _pbBehindProxy ;

    NSAPIEMAILSMTP_DAT_FileDLG(TWindow* pere, NSContexte* pCtx, string* psUrl,
                               int* piPort, string* psUser, string* psPass,
                               bool* pbUsePass, string* psMail, string* psReplyTo,
                               string* psName, int* piMethode, string* psApiUser,
                               string* psApiKeyDir, bool* pBehindProxy) ;
    ~NSAPIEMAILSMTP_DAT_FileDLG() ;

    void CmCancel() ;    void CmOk() ;
    void SetupWindow() ;

  DECLARE_RESPONSE_TABLE(NSAPIEMAILSMTP_DAT_FileDLG) ;
};

//---------------------------------------------------------------
//---------------------------------------------------------------
class _NSOUTILCLASSE NSCodeListDlg : public NSUtilDialog
{
    public :

        NSUtilEditDate*	    pDateDeb;
        NSUtilEditDate*     pDateFin;

        OWL::TRadioButton*  pActesPerso;
   	    OWL::TRadioButton*  pActesTous;

        bool            bTourner;

   	    NSCodeListDlg(TWindow* pere, NSContexte* pCtx);
   	    ~NSCodeListDlg();

   	    void SetupWindow();

        void demarre();
        void stoppe();

    DECLARE_RESPONSE_TABLE(NSCodeListDlg);
};

class _NSOUTILCLASSE NSPrintParamsDlg : public NSUtilDialog
{
	public:

		OWL::TEdit*         pTempo ;
    OWL::TRadioButton*  pHookOui ;
    OWL::TRadioButton*  pHookNon ;
    OWL::TRadioButton*  pVerboseOui ;
    OWL::TRadioButton*  pVerboseNon ;
    OWL::TRadioButton*  pKillTmpNon ;
    OWL::TRadioButton*  pKillTmpOui ;
    OWL::TRadioButton*  pWaitImpOui ;
    OWL::TRadioButton*  pWaitImpNon ;
    OWL::TEdit*         pTempImp ;
    OWL::TRadioButton*  pSimple_Oui;
    OWL::TRadioButton*  pSimple_Non ;
    OWL::TEdit*         pTimePostNav ;

    NSPrintParamsDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSPrintParamsDlg() ;

    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSPrintParamsDlg) ;
};

class _NSOUTILCLASSE NSUpdateFileManager : public NSRoot
{
	protected:

		string     _sURL ;
    string     _sLogin ;
  	string     _sPassword ;
  	string     _sProtocol ;
  	string     _sUpdateDir ;

  	int        _iInterval ;
  	bool       _bLastUpdateCheck ;
  	bool       _bInterval ;
    NVLdVTemps _tLastUpdateCheck ;
    NVLdVTemps _tNextUpdateCheck ;

  	bool       _bLastUpdate ;
  	NVLdVTemps _tLastUpdate ;

    bool       _bSilentMode ;

    string     _sSettingsFile ;

	public:

		NSUpdateFileManager(NSContexte* pCtx) ;
    ~NSUpdateFileManager() ;

    bool readFromFile() ;
    bool writeToFile() ;

    void   setURL(string sU)       { _sURL = sU ; }
    string getURL()                { return _sURL ; }

    void   setLogin(string sL)     { _sLogin = sL ; }
    string getLogin()              { return _sLogin ; }

  	void   setPassword(string sP)  { _sPassword = sP ; }
    string getPassword()           { return _sPassword ; }

  	void   setUpdateDir(string sU) { _sUpdateDir = sU ; }
    string getUpdateDir()          { return _sUpdateDir ; }

  	void   setInterval(int iI)     { _iInterval = iI ; }
    int    getInterval()           { return _iInterval ; }

    void   setIsInterval(bool bI)  { _bInterval = bI ; }
    bool   getIsInterval()         { return _bInterval ; }

    void   setSilentMode(bool bI)  { _bSilentMode = bI ; }
    bool   getIsSilentMode()       { return _bSilentMode ; }
} ;

class _NSOUTILCLASSE NSUpdateParamsDlg : public NSUtilDialog
{
	public:

  	OWL::TGroupBox*    _pIDsGroup ;
    OWL::TStatic*      _pIDTxt ;
    OWL::TEdit*        _pLogin ;
    OWL::TStatic*      _pPassTxt ;
    OWL::TEdit*        _pPassword ;
    OWL::TGroupBox*    _pLocationGroup ;
    OWL::TStatic*      _pUrlTxt ;
		OWL::TEdit*        _pURL ;
    OWL::TStatic*      _pUpdateDirTxt ;
    OWL::TEdit*        _pUpdateDir ;
    OWL::TGroupBox*    _pAutoUpdateGroup ;
    OWL::TRadioButton* _pUpdateYes ;
    OWL::TRadioButton* _pUpdateNo ;
    OWL::TStatic*      _pEveryTxt ;
    OWL::TEdit*        _pInterval ;
    OWL::TStatic*      _pDaysTxt ;
    OWL::TEdit*        _pLastUpdate ;
    OWL::TGroupBox*    _pSilentModeGroup ;
    OWL::TRadioButton* _pSilentYes ;
    OWL::TRadioButton* _pSilentNo ;

    NSUpdateFileManager _fileManager ;

    NSUpdateParamsDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSUpdateParamsDlg() ;

    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSUpdateParamsDlg) ;
};

class _NSOUTILCLASSE NSInitFileManager : public NSRoot
{
	protected:

		string             _sConsole ;
    string             _sIdFileName ;
    string             _sPathBase ;
  	NSSuper::TRACETYPE _iTraceLevel ;
    NSSuper::TRACETYPE _iTraceBBK ;
    NSSuper::TRACETYPE _iConsoleTraceLevel ;
    bool               _bModeClientGroup ;
    bool               _bGroupInterfaceOnly ;
    bool               _bSlaveMode ;
    string             _sSlaveDllName ;

    string             _sSettingsFile ;

    char               getTraceString(NSSuper::TRACETYPE iTrace) ;
    NSSuper::TRACETYPE getTraceLevelFromString(string sTraceLevel) ;
    // NSSuper::TRACETYPE getTraceLevel(string sTraceString) ;

	public:

		NSInitFileManager(NSContexte* pCtx) ;
    ~NSInitFileManager() ;

    bool readFromFile() ;
    bool writeToFile() ;

    void   setConsole(string sU) { _sConsole = sU ; }
    string getConsole()          { return _sConsole ; }

    void   setIdFileName(string sU) { _sIdFileName = sU ; }
    string getIdFileName()          { return _sIdFileName ; }

    void   setPathBase(string sL) { _sPathBase = sL ; }
    string getPathBase()          { return _sPathBase ; }

  	void   setTraceLevel(NSSuper::TRACETYPE iTL) { _iTraceLevel = iTL ; }
    NSSuper::TRACETYPE getTraceLevel()           { return _iTraceLevel ; }

  	void   setConsoleTraceLevel(NSSuper::TRACETYPE iTL) { _iConsoleTraceLevel = iTL ; }
    NSSuper::TRACETYPE getConsoleTraceLevel()           { return _iConsoleTraceLevel ; }

    void setModeClientGroup(bool bB) { _bModeClientGroup = bB ; }
    bool getModeClientGroup()        { return _bModeClientGroup ; }

    void setGroupInterfaceOnly(bool bB) { _bGroupInterfaceOnly = bB ; }
    bool getGroupInterfaceOnly()        { return _bGroupInterfaceOnly ; }

    void setSlaveMode(bool bB) { _bSlaveMode = bB ; }
    bool getSlaveMode()        { return _bSlaveMode ; }

    void   setSlaveDllName(string sL) { _sSlaveDllName = sL ; }
    string getSlaveDllName()          { return _sSlaveDllName ; }
} ;

class _NSOUTILCLASSE NSInitParamsDlg : public NSUtilDialog
{
	public:

  	OWL::TGroupBox*    _pConsoleIDGroup ;
    OWL::TStatic*      _pConsoleIDTxt ;
    OWL::TEdit*        _pConsoleID ;
    OWL::TStatic*      _pConsoleIDTxt2 ;
    OWL::TGroupBox*    _pPathGroup ;
		OWL::TEdit*        _pPath ;
    OWL::TGroupBox*    _pTraceLevelGroup ;
    OWL::TRadioButton* _pTraceLevel1 ;
    OWL::TRadioButton* _pTraceLevel2 ;
    OWL::TRadioButton* _pTraceLevel3 ;
    OWL::TRadioButton* _pTraceLevel4 ;
    OWL::TRadioButton* _pTraceLevel5 ;
    OWL::TRadioButton* _pTraceLevel6 ;

    NSInitFileManager _fileManager ;

    NSInitParamsDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSInitParamsDlg() ;

    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

    void SetTraces(NSSuper::TRACETYPE iTraceLevel) ;
    void ResetTraces() ;

    NSSuper::TRACETYPE getTraceLevel() ;

	DECLARE_RESPONSE_TABLE(NSInitParamsDlg) ;
};

class _NSOUTILCLASSE NSPatSearchEditParamsDlg : public NSUtilDialog
{
	public:

  	OWL::TGroupBox* _pParamsIDGroup ;
    OWL::TStatic*   _pCharCountTxt ;
    NSEditNum*      _pCharCountEdit ;
    OWL::TStatic*   _pIdleTimeTxt ;
		NSEditNum*      _pIdleTimeEdit ;

    NSPatSearchEditParamsDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSPatSearchEditParamsDlg() ;

    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSPatSearchEditParamsDlg) ;
};

#endif
