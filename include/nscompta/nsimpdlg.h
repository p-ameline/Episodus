// NSAgaDlg : dialogues des Impayés// Rémi SPAAK Octobre 98
//////////////////////////////////////////////////////

#ifndef __NSIMPDLG_H
#define __NSIMPDLG_H

#if defined(_DANSCPTADLL)
	#define _CLASSECPTA __export
#else
	#define _CLASSECPTA __import
#endif

#include <owl\edit.h>
#include <owl\checkbox.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\scrollba.h>
#include <owl\combobox.h>

#include "nsbb\nsednum.h"
#include "nscompta\nscompta.h"
#include "nscompta\nscpta.h"
#include "nscompta\nsagavar.h"

//
// Classe document IMP (impayés) pour la publication
//
class _CLASSECPTA NSImpDocument : public NSNoyauDocument
{
	public:

  	NSVarCompta*     pVar ;
    NSMultiCriteres* pCriteres ;
    bool						 bImprimer ;
    bool						 bEuro ;

    NSTPArray*       pTPArray ;
    int						   nbTP ;
    NSImpArray*			 pImpArray ;
		int	 					   nbImp ;

    NSImpDocument(TDocument *parent, NSDocumentInfo *pDocumentInfo,
      					NSDocumentInfo* pDocHtmlInfo = 0, NSContexte *pCtx = 0) ;

    NSImpDocument(TDocument *parent = 0, NSContexte *pCtx = 0) ;

   	~NSImpDocument() ;

    // Méthodes virtuelles de TDocument
    bool 		Open(int mode = 0, const char far *path = 0) ;
    bool 		Close() ;

    bool 		LanceCriteres() ;
   	// bool		CherchePatient(string sNumSS, string& sNomPatient, NSPersonInfo& patInfo) ;
   	bool 		InitTPArray(string sNumCompt) ;
    void 		insertInTPArrayForCompt(NSImpData *pImpData, NSComptInfo *pComptInfo) ;
    void    InitImpdataFromCompt(NSImpData *pImpData, NSComptInfo *pComptInfo, string sLang) ;
    void 		InsertTpFromArray(NSImpData* pImpdata, int &iSommeEuro, int &iSommeLoc) ;

    void 		removeFromTPArray(string sNumCompt, NSImpData *pImpData = 0) ;

    bool    SelectionCriteres(NSComptInfo* pComptInfo) ;
    bool 		InitImpArray() ;

    // bool IsOpen();
};

class _CLASSECPTA NSListImpWindow;

class _CLASSECPTA NSListImpDialog : public NSUtilDialog
{
	public:

		NSImpDocument*     pDoc ;

   	NSVarCompta*       pVar ;
   	NSListImpWindow*   pListeImp ;

   	OWL::TGroupBox*    pMonnaie ;
   	OWL::TRadioButton* pLocal ;
   	OWL::TRadioButton* pEuro ;

		NSListImpDialog(TWindow* pere, NSContexte* pCtx, NSImpDocument* pImpDoc) ;
   	~NSListImpDialog() ;

   	void 		SetupWindow() ;

		void 		InitListeImp() ;
   	void 		AfficheListeImp() ;
   	void 		DispInfoListeImp(TLwDispInfoNotify& dispInfo) ;
   	void 		SwitchLocEuro() ;

   	void 		CmModifImp() ;
   	void		CmImprimer() ;
   	void 		CmOk() ;
   	void 		CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSListImpDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les fiches Imp à modifier)
//
class _CLASSECPTA NSListImpWindow : public TListWindow
{
	public:

		NSListImpDialog* pDlg ;

		NSListImpWindow(NSListImpDialog* pere, int resId) : TListWindow(pere, resId)
		{
   		pDlg = pere ;
		}
   	~NSListImpWindow() {}

    void SetupWindow() ;

		void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
   	int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSListImpWindow) ;
};

class _CLASSECPTA NSMultiCritImpDialog : public NSUtilDialog
{
	public:

		NSMultiCriteres* 	 pCriteres ;
   	NSCodeOrgaArray*	 pCodeOrgaArray ;
   	int					       nbCodeOrga ;

   	NSUtilEditDate*	   pDate1 ;
   	NSUtilEditDate*	   pDate2 ;
   	NSUtilLexique*		 pExamen ;
   	TStatic*				   pPrescript ;
   	OWL::TGroupBox*    pActes ;
   	OWL::TRadioButton* pActesPerso ;
   	OWL::TRadioButton* pActesTous ;
   	OWL::TGroupBox*    pCtxt ;
   	OWL::TRadioButton* pCtxtTous ;
    OWL::TRadioButton* pCtxtNP ;
   	OWL::TRadioButton* pCtxtExt ;
    OWL::TRadioButton* pCtxtAmbu ;
   	OWL::TRadioButton* pCtxtHosp ;
   	OWL::TComboBox*    pOrga ;

   	NSMultiCritImpDialog(TWindow* pere, NSContexte* pCtx) ;
   	~NSMultiCritImpDialog() ;

   	void SetupWindow() ;
   	bool InitCodeOrgaArray() ;
   	void CmPrescript() ;

   	void CmOk() ;
   	void CmCancel() ;

	DECLARE_RESPONSE_TABLE(NSMultiCritImpDialog) ;
};

#endif

// fin de Nsimpdlg.h/////////////////////////////////////////////////////////

