// NSAgaDlg : dialogues des AGA des fiches Fact// Rémi SPAAK Sept 98
//////////////////////////////////////////////////////

#ifndef __NSAGADLG_H
#define __NSAGADLG_H

#if defined(_DANSCPTADLL)
	#define _CLASSECPTA __export
#else
	#define _CLASSECPTA __import
#endif

class NSVarCompta ;
class NSCriteres ;
class NSEncaissData ;
class NSEncaissData ;
class NSAgaArray ;
class NSFse16Array ;
class NSEcritureArray ;
class NSTotauxArray ;

class NSUtilEdit2 ;
class NSMultiEdit ;
class NSUtilEditDate ;

#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>
#include <owl\combobox.h>
#include <owl\radiobut.h>

// #include "nsbb\nsednum.h"
// #include "nscompta\nsfse16.h"    // classes NSFse16xx
// #include "nscompta\nsfsedlg.h"   // fiches fse et NSVarCompta, Criteres, etc...
// #include "nscompta\nscompta.h"
#include "nsdn\nsdocnoy.h"
#include "nsbb\nsutidlg.h"

//
// Classe document AGA pour la publication
//
class _CLASSECPTA NSAgaDocument : public NSNoyauDocument
{
	public:

   	NSVarCompta*   pVar ;
    NSCriteres*    pCriteres ;
   	NSEncaissData* pTotaux ;
    NSEncaissData* pPartiels ;
    bool           bImprimer ;
    bool           bAgaCumules ;
    NSAgaArray*    pAgaArray ;
   	NSFse16Array*  pFseArray ;
    int            nbAga ;
   	int            nbPrest ;

    NSAgaDocument(TDocument *parent, NSDocumentInfo *pDocumentInfo,
      					NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

    NSAgaDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;

   	~NSAgaDocument() ;

    // Méthodes virtuelles de TDocument
    bool Open(NSCriteres* pCritAga, bool bCumul = false) ;
    bool Close() ;

    bool InitAgaArray() ;
    void CalculeTotaux() ;
    // bool InitFse1610Array(string sNumCompt) ;
		// bool InitFse1620Array(string sNumCompt) ;
   	// bool InitFse1630Array(string sNumCompt) ;
   	bool ChercheActes(string sNumCompt) ;
   	void SommeActes(string& sActes) ;
   	bool ChercheNumSS(string sNumCompt, string& sNumSS) ;
   	bool CherchePatient(string sNumSS, string& sNomPatient, NSPersonInfo& patInfo) ;
    bool ChercheLibelle(string sNumCompt, string& sLibelle) ;
    bool ChercheEcriture(string sNumEcriture, NSEcritureArray* pEcritureArray) ;
    // bool IsOpen();
};

class _CLASSECPTA NSListAgaWindow;

class _CLASSECPTA NSListAgaDialog : public NSUtilDialog
{
	public:
   	NSAgaDocument*	 pDoc ;    NSTotauxArray*	 pTotauxArray ;    int						   nbTotaux ;
   	NSTotauxArray*	 pPartielsArray ;
   	int						   nbPartiels ;

   	NSVarCompta* 		 pVar ;
   	NSListAgaWindow* pListeAga ;
    TListWindow* 		 pListeLoc ;
    TListWindow*		 pListeEuro ;
   	TListWindow*		 pListeGlobal ;
   	TStatic*				 pMonnaie ;

		NSListAgaDialog(TWindow* pere, NSContexte* pCtx, NSAgaDocument* pAgaDoc) ;
   	~NSListAgaDialog() ;

   	void 		SetupWindow() ;

		void 		InitListeAga() ;   	void 		AfficheListeAga() ;
   	void 		DispInfoListeAga(TLwDispInfoNotify& dispInfo) ;
    void		InitTotauxArrays() ;
    void 		InitListeLoc() ;
   	void 		AfficheListeLoc() ;
   	void 		DispInfoListeLoc(TLwDispInfoNotify& dispInfo) ;
   	void 		InitListeEuro() ;
   	void 		AfficheListeEuro() ;
   	void 		DispInfoListeEuro(TLwDispInfoNotify& dispInfo) ;
   	void 		InitListeGlobal() ;
   	void 		AfficheListeGlobal() ;
   	void 		DispInfoListeGlobal(TLwDispInfoNotify& dispInfo) ;

   	void 		CmModifAga() ;
   	void		CmImprimer() ;
   	void 		CmOk() ;
   	void 		CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSListAgaDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les fiches Aga à modifier)
//
class _CLASSECPTA NSListAgaWindow : public TListWindow
{
	public:

		NSListAgaDialog* pDlg ;

		NSListAgaWindow(NSListAgaDialog* pere, int resId) : TListWindow(pere, resId)
   	{
   		pDlg = pere ;
   	}
   	~NSListAgaWindow() {}

		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
   	int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListAgaWindow) ;
};

//
//	Dialogue de saisie des tiers-payants
//

class _CLASSECPTA NSSaisieTPDialog : public NSUtilDialog
{
	public :

   	NSUtilEdit2* pLibCourt ;
   	NSUtilEdit2* pLibLong ;
   	NSMultiEdit* pAdresse ;

   	NSSaisieTPDialog(TWindow* pere, NSContexte* pCtx) ;
   	~NSSaisieTPDialog() ;

   	void SetupWindow() ;

   	void CmChoixTP() ;
   	void CmOk() ;
   	void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSSaisieTPDialog) ;
};

//
//	Dialogue des criteres Aga
//

class _CLASSECPTA NSCritAgaDialog : public NSUtilDialog
{
	public :

   	NSCriteres*        pCriteres ;

   	NSUtilEditDate*	   pDateAga1 ;
    NSUtilEditDate*    pDateAga2 ;
   	OWL::TGroupBox*		 pActes ;
   	OWL::TRadioButton* pActesPerso ;
   	OWL::TRadioButton* pActesTous ;

   	NSCritAgaDialog(TWindow* pere, NSContexte* pCtx) ;
   	~NSCritAgaDialog() ;

   	void SetupWindow() ;

   	void CmOk() ;
   	void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSCritAgaDialog) ;
};

#endif		// fin de nsagadlg.h
/////////////////////////////////////////////////////////////////////////////

