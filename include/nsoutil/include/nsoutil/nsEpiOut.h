////  NSEpiOut
//
#ifndef __NSEPIOUT_H
#define __NSEPIOUT_H
class NSUtilEditDate ;#include <classlib\arrays.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\edit.h>

#include "nsepisod\flechiesDB.h"
#include "nsbb\nsutidlg.h"
#include "nsoutil\nsexport.h"

//
// Objet "Boite de dialogue" utilisé pour sélectionner l'utilitaire
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

        NSPrometheFile(NSContexte* pCtx);
        ~NSPrometheFile();

        void Lancer();
        bool EcrireFichier();
};

/*********************************************************************************************************************/
                                   /* NSEmail_DatFile */
/**********************************************************************************************************************/
class  NSEMAIL_DAT_FileDLG : public TDialog
{
    public:

        OWL::TEdit*         pUrlSMTP;
        OWL::TEdit*         pPortSMTP;
        OWL::TEdit*         pUserSMTP;
        OWL::TEdit*         pMailExp;
        OWL::TEdit*         pUrlPOP3;
        OWL::TEdit*         pPortPOP3;
        OWL::TEdit*         pUserPOP3;
        OWL::TEdit*         pPassPOP3;
        OWL::TRadioButton*  pClassique;
        OWL::TRadioButton*  pMAPI;

        string*     psUrlSMTP;
        int*        piPortSMTP;
        string*     psUserSMTP;
        string*     psMailExp;
        string*     psUrlPOP3;
        int*        piPortPOP3;
        string*     psUserPOP3;        string*     psPassPOP3;
        int*        piSendMode;


    NSEMAIL_DAT_FileDLG(TWindow* pere, string*  psUrl, int* piPort,
                        string*  psUser, string*  psMail, string*  psUrlPOP,
                        int* piPortPOP, string*  psUserPOP, string* psPassPOP,
                        int* piMethode);
    ~NSEMAIL_DAT_FileDLG();

    void CmCancel();    void CmOk();
    void SetupWindow();

    DECLARE_RESPONSE_TABLE(NSEMAIL_DAT_FileDLG);
};

/*********************************************************************************************************************/
                                   /* NSEmailFile */
/**********************************************************************************************************************/
class _NSOUTILCLASSE NSEmailFile : public NSRoot
{
    public:

        NSEmailFile(NSContexte* pCtx);
        ~NSEmailFile();

        void Lancer();        bool EcrireFichier();
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

		string     sURL ;
    string     sLogin ;
  	string     sPassword ;
  	string     sProtocol ;
  	string     sUpdateDir ;

  	int        iInterval ;
  	bool       bLastUpdateCheck ;
  	bool       bInterval ;
    NVLdVTemps tLastUpdateCheck ;
    NVLdVTemps tNextUpdateCheck ;

  	bool       bLastUpdate ;
  	NVLdVTemps tLastUpdate ;

    string     sSettingsFile ;

	public:

		NSUpdateFileManager(NSContexte* pCtx) ;
    ~NSUpdateFileManager() ;

    bool readFromFile() ;
    bool writeToFile() ;

    void   setURL(string sU) { sURL = sU ; }
    string getURL()          { return sURL ; }

    void   setLogin(string sL) { sLogin = sL ; }
    string getLogin()          { return sLogin ; }

  	void   setPassword(string sP) { sPassword = sP ; }
    string getPassword()          { return sPassword ; }

  	void   setUpdateDir(string sU) { sUpdateDir = sU ; }
    string getUpdateDir()          { return sUpdateDir ; }

  	void   setInterval(int iI) { iInterval = iI ; }
    int    getInterval()       { return iInterval ; }

    void   setIsInterval(bool bI) { bInterval = bI ; }
    bool   getIsInterval()        { return bInterval ; }
} ;

class _NSOUTILCLASSE NSUpdateParamsDlg : public NSUtilDialog
{
	public:

  	OWL::TGroupBox*    pIDsGroup ;
    OWL::TStatic*      pIDTxt ;
    OWL::TEdit*        pLogin ;
    OWL::TStatic*      pPassTxt ;
    OWL::TEdit*        pPassword ;
    OWL::TGroupBox*    pLocationGroup ;
    OWL::TStatic*      pUrlTxt ;
		OWL::TEdit*        pURL ;
    OWL::TStatic*      pUpdateDirTxt ;
    OWL::TEdit*        pUpdateDir ;
    OWL::TGroupBox*    pAutoUpdateGroup ;
    OWL::TRadioButton* pUpdateYes ;
    OWL::TRadioButton* pUpdateNo ;
    OWL::TStatic*      pEveryTxt ;
    OWL::TEdit*        pInterval ;
    OWL::TStatic*      pDaysTxt ;
    OWL::TEdit*        pLastUpdate ;

    NSUpdateFileManager fileManager ;

    NSUpdateParamsDlg(TWindow* pere, NSContexte* pCtx) ;
    ~NSUpdateParamsDlg() ;

    void CmCancel() ;
    void CmOk() ;
    void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSUpdateParamsDlg) ;
};


#endif
