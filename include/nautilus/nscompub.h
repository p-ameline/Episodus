// NSCOMPUB.H : Classes d�di�es � la composition et � la publication
//////////////////////////////////////////////////////////////////////

#ifndef __NSCOMPUB_H

class NSRefDocument ;
class NSTemplateArray ;
class NSFormuleData ;
class NSFormuleArray ;
class NSLdvDocument ;
class TIndySMTP ;

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\combobox.h>
#include <owl\groupbox.h>
#include <owl\checkbox.h>
#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\scrollba.h>
#include <owl\richedit.h>
#include <owl\eventhan.h>

#include <mapi.h>
// #include "nautilus\nsperary.h"
#include "nsbb\nsednum.h"
#include "nsbb\nsutidlg.h"
// #include "nsdn\nsdocum.h"
// #include "nsdn\nsintrad.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nsvarray.h"
#include "partage\nsperson.h"
// #include "nsbb\nsmpids.h"

//
// de pr�sentation de document
//

class ChoixTemplateDialog : public NSUtilDialog
	public:

		OWL::TListBox* 	 pTemplateBox ;
		string           sTypeDoc ;
		string           sCodeSensRoot ;
		VecteurString    VectTermeEquivalentRoot ;

		UINT             TemplateChoisi ;
		ChoixTemplateDialog(TWindow* pere, NSContexte* pCtx, string typeDoc, string codeSensRoot) ;

		void SetupWindow() ;
		void CmTemplateDblClk(WPARAM Cmd) ;

		// void LanceTemplate(int NumDoc);
	DECLARE_RESPONSE_TABLE(ChoixTemplateDialog) ;

// dialogue utilis� pour choisir un nouveau nom de composition (ou l'ancien)

class NomCompoDialog : public NSUtilDialog
	public:

		OWL::TEdit*        pNomDocHtml ;
    OWL::TRadioButton* pOldCompo ;

    string             sNomDocHtml ;
    NomCompoDialog(TWindow* pere, NSContexte* pCtx) ;

    void SetupWindow() ;
    void CmClickOldCompo() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NomCompoDialog) ;


// Dialogues de publication
//////////////////////////////////////////////////////////////////////////

#define MAXSELECT 100
class NSPublication;
class NSPubliEdit : public NSUtilEdit
	public :

		int     iResId ;
    NSPubliEdit(NSContexte *pCtx, NSUtilDialog* pNSUtilDialog, int resId, int iTextLen = -1,
    ~NSPubliEdit() ;

    void EvChar(uint key, uint repeatCount, uint flags) ;
	DECLARE_RESPONSE_TABLE(NSPubliEdit);

class NSChoixPubli
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
    string getMailAddress() { return _sUrl ;    }
    bool   wantHtmlExport() { return _bHtml ;   }
    bool   wantPdfExport()  { return _bPdf ;    }
    bool   wantLetter()     { return _bLettre ; }

    // char corresp[PAT_NSS_LEN + 1] ;

    string _sCorresp ;
    bool   _bSelect ;
    // Pour l'impression
    bool _bImprimante ;
    int  _iCptDuplicata ;                   // Nbre de duplicatas
    // char nb_expl[WF_NB_PUBLI_LEN + 1] ;
    char _nb_expl[3] ;   // Nbre d'exemplaires th�orique

    // Pour la publication
    bool _bEmail ;
    bool _bHtml ;
    bool _bPdf ;
    bool _bLettre ;
};

class LettreTypeEditDialog : public NSUtilDialog
	public :

    NSFormuleData* _pData ;
    string         _sTexte ;
    bool           _bNew ;
    bool           _bDel ;

    LettreTypeEditDialog(TWindow* pere, NSContexte* pCtx) ;

    void SetupWindow() ;
    void CmDetruire() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(LettreTypeEditDialog) ;

class LettreTypeDialog : public NSUtilDialog
	public :

    LettreTypeDialog(TWindow* pere, NSContexte* pCtx) ;

    void SetupWindow() ;
    void RemplaceTags(string& sTexte) ;
    void AfficheIntro() ;
    void AfficheCorps() ;
    void AffichePolit() ;
    void CmIntroPrec() ;
    void CmIntroSuiv() ;
    void CmCorpsPrec() ;
    void CmCorpsSuiv() ;
    void CmPolitPrec() ;
    void CmPolitSuiv() ;
    void CmClickSansIntro() ;
    void CmClickSansCorps() ;
    void CmClickSansPolit() ;
    void CmEditIntro() ;
    void CmEditCorps() ;
    void CmEditPolit() ;
    void CmEditFormule(NSFormuleData* pFormData, string& sMode) ;

    void CmOk() ;

    string getIntroText()   { return _sIntro ; }
    string getBodyText()    { return _sCorps ; }
    string getPoliteText()  { return _sPolit ; }

    bool	 existsIntro()    { return _choixIntro >= 0 ; }
    bool	 existsBody()     { return _choixCorps >= 0 ; }
    bool	 existsPolite()   { return _choixPolit >= 0 ; }

  protected:
    NSUtilEdit*			   _pIntro ;
    NSUtilEdit*			   _pCorps ;
    NSUtilEdit*			   _pPolit ;

    OWL::TRadioButton* _pSansIntro ;
    OWL::TRadioButton* _pSansPolit ;

    NSFormuleArray*		 _pIntroArray ;
    NSFormuleArray*		 _pCorpsArray ;
    NSFormuleArray*		 _pPolitArray ;

    string				     _sIntro ;
    string				     _sCorps ;
    string			       _sPolit ;

    int					       _choixIntro ;
    int					       _choixPolit ;

    int					       _nbIntro ;
    int					       _nbPolit ;

    string             _sLastCode ;

	DECLARE_RESPONSE_TABLE(LettreTypeDialog) ;

class TLettreType;
class PubliCorrespDialog : public NSUtilDialog
	public :

		OWL::TListBox*	   _pCorrespBox ;
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

    NSPersonArray      _aCorrespArray ;
    NSPersonArray*     _pCorrespBaseArray ;
    UINT	             _CorrespChoisi ;

    PubliCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication) ;

    void   SetupWindow() ;
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
    void   CmOk() ;
    void   CmCancel() ;

    NSPersonInfo* getSelectedPerson() ;
    NSChoixPubli* getSelectedPubli() ;

	DECLARE_RESPONSE_TABLE(PubliCorrespDialog) ;

class PubliSansCorrespDialog : public NSUtilDialog
	public :

    NSUtilEdit*			   _pNbExpl ;
    OWL::TGroupBox*    _pGroupe ;
    OWL::TRadioButton* _pImprimante ;
    OWL::TRadioButton* _pEmail ;
    OWL::TRadioButton* _pIntranet ;
    OWL::TRadioButton* _pPdf ;
    OWL::TRadioButton* _pHtml ;
    OWL::TGroupBox*    _pGroupePat ;
    OWL::TRadioButton* _pPubliPatient ;

    NSPublication*     _pPubli ;
    PubliSansCorrespDialog(TWindow* pere, NSContexte* pCtx,
    ~PubliSansCorrespDialog() ;

    void SetupWindow() ;
    void CmPublierVersPatient() ;
    void CmClickImprimante() ;
    void CmClickEmail() ;
    void CmClickIntranet() ;
    void CmClickHtml() ;
    void CmClickPdf() ;
    void CmClickLettre() ;
    void CmOk() ;
    void CmCancel() ;

	DECLARE_RESPONSE_TABLE(PubliSansCorrespDialog) ;

class ChoixCorrespDialog : public NSUtilDialog
	public :

		OWL::TListBox*	   _pCorrespBaseBox ;
    OWL::TRadioButton* _pAjouterPatient ;
    NSPersonArray*     _pCorrespBaseArray ;

    NSPublication*	   _pPubli ;
    ChoixCorrespDialog(TWindow* pere, NSContexte* pCtx,
    ~ChoixCorrespDialog() ;

    void SetupWindow() ;
    void AfficheSelect() ;
    void CmSelectCorrespBase(WPARAM Cmd) ;
    void CmCorrespBaseDblClk(WPARAM Cmd) ;
    void CmSelectAll() ;
    void CmAjouterPatient() ;

    void CmOk() ;

    int  getPubliIndexFromPersonID(string sPersonID) ;

	DECLARE_RESPONSE_TABLE(ChoixCorrespDialog) ;

/***************************** HOOK ***************************************/
// hook Proc
LRESULT WINAPI CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
// bool�en utilis� par le hook
/***************************************************************************/
//							Classe NSPublication
/***************************************************************************/

class NSPatientChoisi;
class NSMailParams;

class NSMapiCtrl
{
	private:	// D�clarations de l'utilisateur

		bool      	  _bInterrupt ;
    bool          _bApicrypt ;

  	HINSTANCE 	  _MapiInst ;

    MapiMessage   _msg ;
    MapiRecipDesc _emetteur ;
    VectString	  _attachements ;

    static long lObjectCount ;

    void  init() ;
    void  reset() ;

	public:		// D�clarations de l'utilisateur
    NSContexte*    _pContexte ;
    NSMailParams*  _pMail ;
    AnsiString     _encours ;

    NSMapiCtrl(NSContexte* ctx, NSRefDocument* doc, NSMailParams* mail) ;

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

class NSPublication : public NSRoot
  protected:

    static long lObjectCount ;

	public :

		NSPersonArray    _aCorrespBaseArray ;
		NSChoixPubli	   _choixPubli ;

		NSPatientChoisi* _pPatChoisi ;
    NSLdvDocument*	 _pDocMaitreLdv ;
		string 			     _sCodeDoc ;
		string			     _fichTmpl ;
		string			     _enteteTmpl ;
		bool             _bImpressionEnCours ;

		// donnees de publication
		string 			     _sFichHtml[MAXSELECT] ;
		string			     _sLettreHtml[MAXSELECT] ;
		int 			       _numImpress ;
		int 			       _indexSelect ;
		int				       _indexLettre ;
		string			     _sAdresseCorresp ;
		string			     _sObjet ;

    NSLdvPubli       *_pLdvPubliDriver ;

		// Param�tres utilis�s dans tempo.dat
		int              _iTempImp ;
		TIndySMTP*	     _pXSMTP ;

		long (FAR *_pCrypteMsg) (NSContexte far *pCtx, string far *pMessage,

		long (FAR *_pCrypteFic) (NSContexte far *pCtx, string far *pNomFich,

		NSPublication(NSPatientChoisi* pPatientChoisi,
    NSPublication(NSPatientChoisi* pPatientChoisi,
                      NSLdvDocument* pDocEnCours, NSLdvPubli *pDriver,
                      NSContexte* pCtx) ;
		~NSPublication() ;

		string tempPubli(char *typePub) ;
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