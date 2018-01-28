#if !defined(NSSMEDIT_H)#define NSSMEDIT_H

#include <owl\dialog.h>#include <owl\radiobut.h>
#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\scrollba.h>
#include <owl\panespli.h>
#include <owl\listwind.h>

#include "nsbb\nsednum.h"#include "nsbb\nstrewi.h"
#include "nsbb\nsbbsmal.h"
#include "nsoutil\nssavary.h"#include "nsmt_semedit\semedit_dlg.h"
#define ID_TREE 100#define ID_LIST 101

//// class TPaneListWindow : liste window qui contiendra les équivalents
//									sémantiques d'un item donné
// ~~~~~ ~~~~~~~~~~~~~~~
class NSPaneListWindow : public TListWindow
{
    public:

        NSPaneListWindow(TWindow* parent, int id, int x, int y, int w, int h,
        TModule* module = 0)
        :TListWindow(parent, id, x, y, w, h, module){}
};

//
// class NSsmEditeur : 	fenêtre divisée en 2 parties : la première contient
//								la patpatho de sLexiquePilote et la deuxième contient
//								les équivalents sémantiques de sLexiquePilote.
//
// ~~~~~ ~~~~~~~~~~~~~
//
class NSsmEditeur : public NSRoot, public TPaneSplitter
{	public:
		//
		// Data members
		//
		string            sLexiquePilote ; //élément lexique choisi par l'utilisateur
    string            sTitle ;

		NSPatPathoArray* 	pPatPathoArray ;
		NSSavoirArray*		pSavoirArray ;

		NSSmallBrother* 	pBigBoss ;

		NSTreeWindow* 		pNSTreeWindow ;
		NSPaneListWindow* pListWind ;

		NSsmEditeur(TWindow* parent, NSContexte* pCtx, string sLexiquePilot) ;
		~NSsmEditeur() ;

		void SetupWindow() ;
		void CleanupWindow() ;
		void EvClose() ;

		void CmEnregistre() ;
		bool enregPatPatho() ;

		void initPatPatho() ;
		void initSavoir() ;

		void AfficheListe() ;
		void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;

		void donneRelation(string* pQualificatif, string* pQualifie, string* pEtiquette) ;

	DECLARE_RESPONSE_TABLE(NSsmEditeur) ;
};
// Classe TMDIChild pour la redéfinition de EvClose/////////////////////////////////////////////////////////
class NsFicheEditorChild : public TMDIChild, public NSRoot
{
	public :

		NSsmEditeur* pClient ;

		NsFicheEditorChild(NSContexte* pCtx, TMDIClient& parent, const char far* title = 0, NSsmEditeur* clientWnd = 0) ;
		~NsFicheEditorChild() ;

		void EvClose() ;

	DECLARE_RESPONSE_TABLE(NsFicheEditorChild) ;
};class NSLexiqEditeur : public NSUtilDialog
{
    public:

        OWL::TEdit*         pLexiLibelle ;
        OWL::TComboBox*     pLexiType ;
        OWL::TComboBox*     pLexiGrammaire ;
        OWL::TListWindow*   pLexiListe ;

        OWL::TButton*       pAdd ;
        OWL::TButton*       pReplace ;
        OWL::TButton*       pDelete ;

        string*             psAmmorce ;
        string              sPartLib ;

        NSLexiqEditeur(TWindow* pere, NSContexte* pCtx, string* pAmmorce, string sLibel = "") ;
        ~NSLexiqEditeur() ;

        void            SetupWindow() ;

        void            CmAjouter() ;
        void            trouveAmmorce() ;

        // void            CmOk() ;
        // void            CmCancel() ;

        void            initSynonymsList() ;
        VecteurString   TrouverListeSynonimes(string sCodeTerm) ;

    DECLARE_RESPONSE_TABLE(NSLexiqEditeur);
};

class NSUtilLexiqueSynonimes ;
class NSSemList ;
class NSSynonimesList ;

//---------------------------------------------------------------
//fixer le type de document
//---------------------------------------------------------------
class NSSemEditor : public NSUtilDialog
{
	public :

		NSUtilLexiqueSynonimes* pType ;
		string                  sCode ;
		string                  sRelation ;
		string* 	  		        pTypeDocumComp ;
		TWindow*                pDialog ;
		HANDLE                  hIOFile ;

		OWL::TCheckBox*	            pIsA ;
		OWL::TCheckBox*	            pAt ;
		OWL::TCheckBox*	            pIsAIntrasitif ;
		OWL::TCheckBox*             pPartOf ;
		OWL::TCheckBox*             pMeasure ;
		OWL::TButton*               pChange ;
		OWL::TButton*               pLexEditCurrent ;
		OWL::TButton*               pLexEditOther ;
		OWL::TButton*               pLexEditNew ;

		NSSemList*                  pLink ;
		NSSemList*                  pLinkLeft ;

		NSSynonimesList*            pSynTitleTerm ;
		NSSynonimesList*            pSynNewTerm ;

      /*  OWL::TCheckBox*	        pIsAGr;
        OWL::TCheckBox*	        pAtGr;
        OWL::TCheckBox*	        pIsAIntrasitifGr;
        OWL::TCheckBox*         pPartOfGr;
        OWL::TCheckBox*         pMeasureGr;
        OWL::TCheckBox*         pAtPlusPartOfGr;     */



        OWL::TCheckBox*         pGeneral ;
        OWL::TCheckBox*         pCompose;
        OWL::TCheckBox*         pUnit ;
        OWL::TCheckBox*         pContain ;
        OWL::TCheckBox*         pGeneralIntra ;

        //OWL::TRect*             pRect;
        OWL::TPopupMenu*        pMenu;

        NSSavoirArray           aFiches ;
        NSSavoirArray           aFichesLeft;

        int                     iModifyIndex ;
        int                     iModifyIndexLeft ;
     //   int                     iPos;


   	    NSSemEditor(TWindow* pere, NSContexte* pCtx, string sCodeElement,                     TModule* module = 0);
   	    ~NSSemEditor();

        void donneRelation(string* pQualificatif, string* pQualifie, string* pEtiquette, bool bEndroit) ;

   	    void CmOk() ;
        //void CmCancel();
        void changeTerme() ;
        void getGraph() ;
        void htmlPublish() ;
        void editFiche() ;

        void lanceIsA();
        void lanceMeasureIn();
        void lanceAt();
        void lancePartOf();
        void lanceIsIntrasitif();
        //void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
        //void EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point);
        //void EvListRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
        //void EvKeyDown(uint key, uint repeatCount, uint flags);
        void lanceIsAGr();
        void lanceMeasureInGr();
        void lanceAtGr();
        void lancePartOfGr();
        void lanceIsIntrasitifGr();
        void lanceAtPlusPartOf();
        bool  bAtPlusPartOf;


        void lanceGeneralization();
        void lanceUnit();
        void lanceCompose();
        void lanceContain();
        void lanceGenIntrasitif();

        bool InitSavoirArray() ;
        void InitListe() ;
        void AfficheListe() ;

        bool InitSavoirArrayLeft() ;
        void InitListeLeft() ;
        void AfficheListeLeft() ;

        VecteurString ListeSynonimes(string sTerm);

   	    void SetupWindow();

        void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
        void LvnGetDispLeftInfo(TLwDispInfoNotify& dispInfo) ;

        void introduireNvLien(string sQualificatif);
        void introduireNvLienLeft(string sQualificatif);

        void modifyLiaison(int index, string sQualificatif);
        void modifyLiaisonLeft(int index, string sQualificatif);

        void deleteLiaison(int index);
        void deleteLiaisonLeft(int index);

        void openLiaison(int index);
        void openLiaisonLeft(int index);

        void modifyText(int index);
        void modifyTextLeft(int index);

        int isHomogeneousAT(string sQualifiant,string sQualifier);
        bool chercherNode(string sCodeSensCherche, string sRelation,
                        VecteurString *pListNode,
                        NSSavoir* pSavoir) ;
       void lanceChercherNode(string sRelation, string sTitle, bool bPartOfPlus, string sFileName = "", bool bOpen = true, string sBaseCode = "");

       void lanceLexEditCurrent() ;
       void lanceLexEditOther() ;
       void lanceLexEditNew() ;

    DECLARE_RESPONSE_TABLE(NSSemEditor);
};

class NSSemEditor ;

//-----------------------------------------------------------------------
class NSSemList : public TListWindow
{
	public :

		int iSelectedIndex ;

		NSSemEditor* pDialog ;

		//if left = true, if right = false
		bool iLeft ;

		NSSemList(NSSemEditor* pere, int resource, bool pos, TModule* module =0) ;

		~NSSemList() ;

		void modifyLiaison() ;
		void deleteLiaison() ;
		int IndexItemSelect() ;
		void openLiaison() ;

		void EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point) ;
		void EvKeyDown(uint key, uint repeatCount, uint flags) ;
		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;

		void SetupWindow() ;

	DECLARE_RESPONSE_TABLE(NSSemList) ;
};
//---------------------------------------------------------------
class NSAfficheGraphe : public TDialog, public NSRoot
{
    public :

        NSSemEditor* pDialogPere ;
        bool  bAtPlusPartOf;
        string sCodeTitle;

        OWL::TCheckBox*	        pIsAGr;
        OWL::TCheckBox*	        pAtGr;
        OWL::TCheckBox*	        pIsAIntrasitifGr;
        OWL::TCheckBox*         pPartOfGr;
        OWL::TCheckBox*         pMeasureGr;
        OWL::TCheckBox*         pAtPlusPartOfGr;

        NSAfficheGraphe(NSSemEditor* pere,string sElemTitle,NSContexte* pCtx,TModule* module =0);
        ~NSAfficheGraphe();

        void SetupWindow();
        void lanceIsAGr();
        void lanceMeasureInGr();
        void lanceAtGr();
        void lancePartOfGr();
        void lanceIsIntrasitifGr();
        void lanceAtPlusPartOfGr();
        void CmCancel();

    DECLARE_RESPONSE_TABLE(NSAfficheGraphe);
} ;

//------------------------------------------------------------

class NSSynonimesList : public OWL::TListBox, public NSRoot
{
    public :

           string sCodeS;
           NSSemEditor* pDialog ;

           NSSynonimesList(NSSemEditor* pere, int resource, NSContexte* pCtx, TModule* module =0);

           ~NSSynonimesList();

           void SetupWindow();
           VecteurString TrouverListeSynonimes(string sCodeTerm);
           void AfficheListeSyn(string sCodeTerm);

        DECLARE_RESPONSE_TABLE(NSSynonimesList);
};

//---------------------------------------------------------------
class NSUtilLexiqueSynonimes : public NSUtilLexique
{
  public :

    //string sCodeS;
    //NSSemEditor* pDialog ;

    NSUtilDialog* pNSUtilDialog ;

    NSUtilLexiqueSynonimes(NSContexte *pCtx, NSUtilDialog* pUtilDialog, int resourceId,
                           NSDico* pDictio, uint textLimit = 0, TModule* module = 0) ;
    ~NSUtilLexiqueSynonimes() ;

    void SetupWindow() ;
    void EvKillFocus(HWND hWndGetFocus) ;

  DECLARE_RESPONSE_TABLE(NSUtilLexiqueSynonimes) ;
} ;

#endif
