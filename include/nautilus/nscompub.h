// NSCOMPUB.H : Classes dédiées à la composition et à la publication// RS Septembre 1997
//////////////////////////////////////////////////////////////////////

#ifndef __NSCOMPUB_H#define __NSCOMPUB_H

class NSRefDocument ;
class NSTemplateArray ;
class NSFormuleData ;
class NSFormuleArray ;
class NSLdvDocument ;
class TIndySMTP ;

#include <classlib\arrays.h>

#include <owl\dialog.h>#include <owl\listbox.h>
#include <owl\combobox.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\scrollba.h>
#include <owl\richedit.h>
#include <owl\listwind.h>
#include <owl\eventhan.h>

#include <mapi.h>
// #include "nautilus\nsperary.h"// #include "nautilus\nsdocref.h"
#include "nsbb\nsednum.h"
#include "nsbb\nsutidlg.h"
// #include "nsdn\nsdocum.h"
// #include "nsdn\nsintrad.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nsvarray.h"
#include "partage\nsperson.h"
// #include "nsbb\nsmpids.h"

//// Objet "Boite de dialogue" utilisé pour sélectionner un template HTML
// de présentation de document
//

class ChoixTemplateDialog : public NSUtilDialog{
	public:

		OWL::TListBox* 	 _pTemplateBox ;		NSTemplateArray* _pTemplateArray ;
		string           _sTypeDoc ;
		string           _sCodeSensRoot ;
		VecteurString    _aVectTermeEquivalentRoot ;

		UINT             _iTemplateChoisi ;
		ChoixTemplateDialog(TWindow* pere, NSContexte* pCtx, string typeDoc, string codeSensRoot) ;		~ChoixTemplateDialog() ;

		void SetupWindow() ;		void CmSelectTemplate(WPARAM Cmd) ;		void CmCancel() ;
		void CmTemplateDblClk(WPARAM Cmd) ;

		// void LanceTemplate(int NumDoc);    UINT             getTemplateChoisi() { return _iTemplateChoisi ; }    NSTemplateArray* getTemplateArray()  { return _pTemplateArray ;  }
	DECLARE_RESPONSE_TABLE(ChoixTemplateDialog) ;};

// dialogue utilisé pour choisir un nouveau nom de composition (ou l'ancien)////////////////////////////////////////////////////////////////////////////

class NomCompoDialog : public NSUtilDialog{
	public:

		OWL::TEdit*        _pNomDocHtml ;    OWL::TRadioButton* _pNewCompo ;
    OWL::TRadioButton* _pOldCompo ;

    string             _sNomDocHtml ;
    NomCompoDialog(TWindow* pere, NSContexte* pCtx) ;    ~NomCompoDialog() ;

    void SetupWindow() ;    void CmClickNewCompo() ;
    void CmClickOldCompo() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NomCompoDialog) ;};

//////////////////////////////////////////////////////////////////////////
// Dialogues de publication
//////////////////////////////////////////////////////////////////////////

#define MAXSELECT 100
class NSPublication;
class NSPubliEdit : public NSUtilEdit{
	public :

		int     iResId ;
    NSPubliEdit(NSContexte *pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1,                    bool bReturn = true) ;
    ~NSPubliEdit() ;

    void EvChar(uint key, uint repeatCount, uint flags) ;
	DECLARE_RESPONSE_TABLE(NSPubliEdit);};

class NSChoixPubli{
  private:

    static long lObjectCount ;

	public :

    NSChoixPubli() ;
    NSChoixPubli(const NSChoixPubli& rv) ;
    ~NSChoixPubli() ;

    NSChoixPubli&	operator=(const NSChoixPubli& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }

    string getCorrespId()   { return _sCorresp ; }
    bool   isSelected()     { return _bSelect ;  }

    bool   wantToPrint()    { return _bImprimante ; }
    int    getCopyNb()      { return _iCpt ;     }

    bool   wantMail()       { return _bEmail ;  }
    string getMailAddress() { return _sUrl ;    }    bool   wantIntranet()   { return _bIntranet ; }
    bool   wantHtmlExport() { return _bHtml ;   }
    bool   wantPdfExport()  { return _bPdf ;    }
    bool   wantLetter()     { return _bLettre ; }

    // char corresp[PAT_NSS_LEN + 1] ;

    string _sCorresp ;
    bool   _bSelect ;
    // Pour l'impression
    bool _bImprimante ;    int  _iCpt ;                            // Nbre d'exemplaires réel
    int  _iCptDuplicata ;                   // Nbre de duplicatas
    // char nb_expl[WF_NB_PUBLI_LEN + 1] ;
    char _nb_expl[3] ;   // Nbre d'exemplaires théorique

    // Pour la publication
    bool _bEmail ;    // char url[PIDS_MESSAGERIE_LEN + 1] ;    string _sUrl ;    bool _bIntranet ;
    bool _bHtml ;
    bool _bPdf ;
    bool _bLettre ;
};

class TLettreType;
class BasicPubliCorrespDialog : public NSUtilDialog
{
  public :

    NSPersonArray      _aCorrespArray ;
    NSPersonArray*     _pCorrespBaseArray ;    NSPublication*		 _pPubli ;    TLettreType*		   _pLettreType ;
    UINT	             _iCorrespChoisi ;

    BasicPubliCorrespDialog(TWindow* pere, NSContexte* pCtx, TResId resID, NSPublication* pPublication) ;    virtual ~BasicPubliCorrespDialog() ;

    void   SetupWindow() ;
    void   CmOk() ;
    void   CmCancel() ;

	DECLARE_RESPONSE_TABLE(BasicPubliCorrespDialog) ;
} ;

class PubliCorrespDialog : public BasicPubliCorrespDialog{
	public :

		OWL::TListBox*	   _pCorrespBox ;    NSUtilEdit*		     _pAdresse ;
    NSPubliEdit*		   _pNbExpl ;
    NSPubliEdit*       _pUrl ;
    OWL::TRadioButton* _pImprimante ;
    OWL::TRadioButton* _pEmail ;
    OWL::TRadioButton* _pIntranet ;
    OWL::TRadioButton* _pHtml ;
    OWL::TRadioButton* _pPdf ;
    OWL::TCheckBox*		 _pLettre ;
    OWL::TRadioButton* _pJoindre ;
    OWL::TRadioButton* _pPubliPatient ;
    OWL::TRadioButton* _pPubliBlank ;

    PubliCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication) ;
    ~PubliCorrespDialog() ;

    void   SetupWindow() ;    void   InitCorrespArray() ;
    void   AfficheCorresp() ;
    void   LanceChoixCorrespDialog() ;
    void   SetNbExpl(NSPersonInfo *pCorrespSelect, const char* sNbExpl) ;
    void   SetUrl(NSPersonInfo* pCorrespSelect, const char* sUrl) ;

    void   ResetChoixPubli() ;
    void   AfficheChoixPubli(NSPersonInfo *pCorrespSelect) ;
    void   AfficheAdresse(NSPersonInfo *pCorrespSelect) ;
    string CodeCorrespSelect() ;
    bool   CodeDocLettreSelect(string& sCode) ;
    bool   ExisteLettre(NSPersonInfo *pCorrespSelect) ;

    void   CheckLettre() ;
    void   CmSelectCorresp(WPARAM Cmd) ;
    void   CmCorrespDblClk(WPARAM Cmd) ;
    void   CmAjouterPatient() ;
    void   CmAjouterBlank() ;
    void   CmClickImprimante() ;
    void   CmClickEmail() ;
    void   CmClickIntranet() ;
    void   CmClickPdf() ;
    void   CmClickHtml() ;
    void   CmClickJoindre() ;
    void   CmLettreType() ;

    NSPersonInfo* getSelectedPerson() ;
    NSChoixPubli* getSelectedPubli() ;

	DECLARE_RESPONSE_TABLE(PubliCorrespDialog) ;};

class NewPubliCorrespDialog : public BasicPubliCorrespDialog
{
	public :

		OWL::TListWindow*	_pList ;
    NewPubliCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication) ;
    ~NewPubliCorrespDialog() ;

    void   SetupWindow() ;    void   InitCorrespArray() ;

    NSPersonInfo* getSelectedPerson() ;
    NSChoixPubli* getSelectedPubli() ;

  protected :

    void   SetupColumns() ;
    void   DisplayCorresps() ;
    void   DisplayParams() ;

	DECLARE_RESPONSE_TABLE(NewPubliCorrespDialog) ;};

class PubliSansCorrespDialog : public NSUtilDialog{
	public :

    NSUtilEdit*			   _pNbExpl ;    NSUtilEdit*			   _pUrl ;
    OWL::TGroupBox*    _pGroupe ;
    OWL::TRadioButton* _pImprimante ;
    OWL::TRadioButton* _pEmail ;
    OWL::TRadioButton* _pIntranet ;
    OWL::TRadioButton* _pPdf ;
    OWL::TRadioButton* _pHtml ;
    OWL::TGroupBox*    _pGroupePat ;
    OWL::TRadioButton* _pPubliPatient ;

    NSPublication*     _pPubli ;
    PubliSansCorrespDialog(TWindow* pere, NSContexte* pCtx,                           NSPublication* pPublication) ;
    ~PubliSansCorrespDialog() ;

    void SetupWindow() ;    void AfficheChoixPubli() ;
    void CmPublierVersPatient() ;
    void CmClickImprimante() ;
    void CmClickEmail() ;
    void CmClickIntranet() ;
    void CmClickHtml() ;
    void CmClickPdf() ;
    void CmClickLettre() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(PubliSansCorrespDialog) ;};

class ChoixCorrespDialog : public NSUtilDialog{
	public :

		OWL::TListBox*	   _pCorrespBaseBox ;    OWL::TListBox*	   _pCorrespSelectBox ;
    OWL::TRadioButton* _pAjouterPatient ;
    NSPersonArray*     _pCorrespBaseArray ;

    NSPublication*	   _pPubli ;
    ChoixCorrespDialog(TWindow* pere, NSContexte* pCtx,                           NSPublication* pPublication) ;
    ~ChoixCorrespDialog() ;

    void SetupWindow() ;    void SetSelectBase() ;
    void AfficheSelect() ;
    void CmSelectCorrespBase(WPARAM Cmd) ;
    void CmCorrespBaseDblClk(WPARAM Cmd) ;
    void CmSelectAll() ;
    void CmAjouterPatient() ;

    void CmOk() ;    void CmCancel() ;

    int  getPubliIndexFromPersonID(string sPersonID) ;

	DECLARE_RESPONSE_TABLE(ChoixCorrespDialog) ;};

/***************************** HOOK ***************************************/
// hook Proc
LRESULT WINAPI CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
// booléen utilisé par le hook
/***************************************************************************/
//							Classe NSPublication
/***************************************************************************/

class NSPatientChoisi;class TControlSMTP;
class NSMailParams;

class NSMapiCtrl
{
	private:	// Déclarations de l'utilisateur

		bool      	  _bInterrupt ;    bool      	  _bAttendre ;
    bool          _bApicrypt ;

  	HINSTANCE 	  _MapiInst ;    LHANDLE   	  _session ;

    MapiMessage   _msg ;    MapiRecipDesc _destinataire ;
    MapiRecipDesc _emetteur ;
    VectString	  _attachements ;

    static long lObjectCount ;

    void  init() ;
    void  reset() ;

	public:		// Déclarations de l'utilisateur
    NSContexte*    _pContexte ;    NSRefDocument* _pDoc ;
    NSMailParams*  _pMail ;
    AnsiString     _encours ;

    NSMapiCtrl(NSContexte* ctx, NSRefDocument* doc, NSMailParams* mail) ;    ~NSMapiCtrl() ;

    void  Connect() ;
    void  Disconnect() ;
    bool  Verify() ;
    void  ComposeMessage(string sFromAdr, string sToAdr, string sObjet, string sCodeLettre) ;
    void  AttacheMessage(string sFileName) ;
    ULONG SendMail() ;
    bool  Continuer() ;
    bool  Attendre() ;
    bool  SetApicryptMode() { _bApicrypt = true ; return true ; }

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSLdvPubli
{
  protected:

    static long lObjectCount ;

	public :

    enum PRINTTYPE { printNotSpecified = 0, printLdv, printGoals, printDrugs } ;
    PRINTTYPE _iPrintType ;

    void*     _pPrintArray ;

    string    _sTemplateFile ;
    string    _sHeaderFile ;

    NSLdvPubli() ;
    ~NSLdvPubli() { lObjectCount-- ; }
    NSLdvPubli(const NSLdvPubli& rv) ;

    NSLdvPubli& operator=(const NSLdvPubli& src) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

class NSPublication : public NSRoot{
  protected:

    static long lObjectCount ;

	public :

		NSPersonArray    _aCorrespBaseArray ;		NSChoixPubli	   _aChoixPubli[MAXSELECT] ;
		NSChoixPubli	   _choixPubli ;

		NSPatientChoisi* _pPatChoisi ;		NSRefDocument*	 _pDocMaitre ;
    NSLdvDocument*	 _pDocMaitreLdv ;
		string 			     _sCodeDoc ;
		string			     _fichTmpl ;
		string			     _enteteTmpl ;
		bool             _bImpressionEnCours ;

		// donnees de publication		string 			     _sTemplatePubli ;
		string 			     _sFichHtml[MAXSELECT] ;
		string			     _sLettreHtml[MAXSELECT] ;
		int 			       _numImpress ;
		int 			       _indexSelect ;
		int				       _indexLettre ;
		string			     _sAdresseCorresp ;
		string			     _sObjet ;

    NSLdvPubli       *_pLdvPubliDriver ;

		// Paramètres utilisés dans tempo.dat		bool             _bUseHook ;		bool             _bWaitImp ;
		int              _iTempImp ;
		TIndySMTP*	     _pXSMTP ;		NSMapiCtrl*		   _pMapiSMTP ;

		long (FAR *_pCrypteMsg) (NSContexte far *pCtx, string far *pMessage,								string far *pAdresse, string far *pClef) ;

		long (FAR *_pCrypteFic) (NSContexte far *pCtx, string far *pNomFich,								string far *pAdresse, string far *pClef) ;

		NSPublication(NSPatientChoisi* pPatientChoisi,                      NSRefDocument* pDocEnCours, NSContexte* pCtx) ;
    NSPublication(NSPatientChoisi* pPatientChoisi,
                      NSLdvDocument* pDocEnCours, NSLdvPubli *pDriver,
                      NSContexte* pCtx) ;
		~NSPublication() ;

		string tempPubli(char *typePub) ;		string entetePubli(char *typePub) ;
		string dateSysteme() ;
		string heureSysteme() ;
		bool   ChargeCorrespBase() ;
		bool   ExisteCorrespSelect() ;
#ifndef N_TIERS
		bool   AppendWorkflow(char *dest, char *type, char *nb_expl,
                                                    string entete, string tmpl) ;
		bool   RemplitWorkflow() ;
#endif

		bool   CodeDocLettre(int index, string& sCode) ;
    string getCodeLettre(string sPersonID) ;

		string StringAdresse(string sCodeCorresp) ;
		string NomCorresp(string sCodeCorresp) ;
		void   Publication() ;
    void   GetMailsTypes(bool &bExistStandardMail, bool &bExistCryptedMail) ;
    void   SendNsmtpMails() ;
    void   SendNsmtpStandardMails() ;
    void   SendNsmtpApicryptMails() ;
    void   SendNsmtpSpecificMails(NSMailParams* pMailParams, bool bCrypted) ;
    void   SendMapiMails() ;
    void   SendMapiStandardMails() ;
    void   SendMapiApicryptMails() ;
    void   SendMapiSpecificMails(NSMailParams* pMailParams, bool bCrypted) ;
		bool   ImprimerSuivant() ;
		void   DeleteFichPubli() ;
		void   InstalleHook() ;
		void   LibereHook() ;

		void   ConnectHost(NSMailParams* pMailParams) ;
		void   DisconnectHost() ;

		void   ConnectMapi() ;
		void   DisconnectMapi() ;

		bool   Imprimer(int index = -1) ;
    bool   GetMailAndLetter(string &sMailAddress, string &sLetterId, string &sTempFileName, string &sPlainAddress, int index = -1) ;
		bool   SendMail(string sPathHtml, int index = -1) ;
    bool   SendMapiMail(string sPathHtml, int index = -1) ;
		bool   Exporter(string sPathHtml, bool bHtml, bool bPdf, int index = -1) ;
		void   DeleteVisualViews() ;
		void   initParams() ;
		void   Tempo(int iDelay) ;
		void   Publier() ;
		void   DebutImpression()       { _bImpressionEnCours = true ; }
		void   FinImpression()         { _bImpressionEnCours = false ; }
		bool   ImpressionEnCours()     { return _bImpressionEnCours ; }
		bool   InitCrypteurs(string sURL) ;

    NSChoixPubli* getChoixPubliForPerson(NSPersonInfo *pCorrespSelect) ;

    static long getNbInstance()  { return lObjectCount ; }
    static void initNbInstance() { lObjectCount = 0 ; }
};

#endif
// fin de nscompub.h
