#ifndef __NSEPIDIV_H#define __NSEPIDIV_H

class SOAPObject;
class NSChapRadioButton;
class classifExpert;

class NSUtilEdit ;
class NSUtilEditDate ;

#include <owl\dialog.h>
#include <owl\button.h>
#include <owl\radiobut.h>

#include "nsbb\nsutidlg.h"
// #include "nsbb\nsednum.h"
#include "nsepisod\eptables.h"
#include "nsepisod\nsldvuti.h"

#if defined(_EPISODUSDLL)
	#define _EPISODUS __export
#else
	#define _EPISODUS __import
#endif

#define NBCHAPCISP 17

class _EPISODUS CISPFindCodeDlg : public NSUtilDialog
{
  public:

    SOAPObject*             pObjet ;

    int                     iSelectionLevel ;

    OWL::TListBox*          pCodeList ;
    OWL::TButton*           pBtnMore ;
    OWL::TButton*           pBtnLess ;
    OWL::TStatic*           pLevelText ;

    string                  sChapitre ;
    ICPC_CATEGORY           iCategory ;
    ICPC_PATHOLOGY          iPathology ;

    NSChapRadioButton*      pChapitres[NBCHAPCISP] ;
    OWL::TRadioButton*      pCategories[3] ;

    string                  sClassification ;
    NSEpiClassifInfoArray*  pArrayClassif ;

    NVLdVTemps              tpsDebut, tpsFin ;
    int                     iInitialLevel ;

    CISPFindCodeDlg(TWindow* parent, NSContexte* pCtx, SOAPObject* pObjet,
                            int iSelLevel, NSEpiClassifInfoArray* pPossibles,
                            string* pControlData) ;
    ~CISPFindCodeDlg() ;

    void    CmDblClk(WPARAM Cmd) ;
    string  getResult() { return sResult ; }

    void    CmOk() ;
    void    CmCancel() ;
    void    SetupWindow() ;
    void    CmMore() ;
    void    CmLess() ;

    void    evSetChapitre(string sChap) ;
    void    evSetCategory(WPARAM wParam) ;
    void    afficheNiveau() ;
    void    listUpdate() ;
    void    changeNiveau(int iIncrement) ;

  protected:
    string*         pCtrlData ;
    string          sResult ;    classifExpert*  pExpert ;

    void    buildCtrlString() ;
    void    setCategoryButtonsState() ;

  DECLARE_RESPONSE_TABLE(CISPFindCodeDlg) ;
} ;

#define NBCHAPCIM 21
class _EPISODUS CIMFindCodeDlg : public NSUtilDialog
{
  public:

    SOAPObject*             pObjet ;

    int                     iSelectionLevel ;

    OWL::TListBox*          pCodeList ;
    OWL::TButton*           pBtnMore ;
    OWL::TButton*           pBtnLess ;
    OWL::TStatic*           pLevelText ;

    string                  sChapitre ;

    NSChapRadioButton*      pChapitres[NBCHAPCISP] ;

    string                  sClassification ;
    NSEpiClassifInfoArray*  pArrayClassif ;

    NVLdVTemps              tpsDebut, tpsFin ;
    int                     iInitialLevel ;

    CIMFindCodeDlg(TWindow* parent, NSContexte* pCtx, SOAPObject* pObjet,
                            int iSelLevel, NSEpiClassifInfoArray* pPossibles,
                            string* pControlData) ;
    ~CIMFindCodeDlg() ;

    void    CmDblClk(WPARAM Cmd) ;
    string  getResult() { return sResult ; }

    void    CmOk() ;
    void    CmCancel() ;
    void    SetupWindow() ;
    void    CmMore() ;
    void    CmLess() ;

    void    evSetChapitre(string sChap) ;
    void    afficheNiveau() ;
    void    listUpdate() ;
    void    changeNiveau(int iIncrement) ;

  protected:

    string*         pCtrlData ;
    string          sResult ;
    classifExpert*  pExpert ;

    void    buildCtrlString() ;

  DECLARE_RESPONSE_TABLE(CIMFindCodeDlg) ;
};

class NSChapRadioButton : public OWL::TRadioButton
{
  public:

    string sChapitre ;

    // Constructeur et destructeur
    NSChapRadioButton(NSUtilDialog* pPere, int resId, string sChap, OWL::TGroupBox* group = 0);
    NSChapRadioButton(NSUtilDialog* pPere, int resId, string sChap,
                            const char far* title, int x, int y, int w, int h,
                            OWL::TGroupBox *group = 0,
                            OWL::TModule* module = 0) ;
    ~NSChapRadioButton() ;

    void SetupWindow() ;
    void initialise(string sSelChap) ;

    // child id notification handled at the child
    //
    void BNClicked() ;  // BN_CLICKED

  protected:

    NSUtilDialog* pDlg ;

	DECLARE_RESPONSE_TABLE(NSChapRadioButton) ;
} ;

class _EPISODUS PrometheComplementDlg : public NSUtilDialog
{
  public:

    string* pPatNaiss;
    string* pPatCP;
    string* pExamCond;
    string* pExamRV;

    NSUtilEdit*        pCodePost ;
    NSUtilEditDate*    pDateN ;

    OWL::TGroupBox*    pTypeRencontre ;
    OWL::TRadioButton* pConsult ;
    OWL::TRadioButton* pVisite ;

    OWL::TGroupBox*    pModeRencontre ;
    OWL::TRadioButton* pSsRV ;
    OWL::TRadioButton* pAvecRV ;
    OWL::TRadioButton* pUrgence ;

    PrometheComplementDlg(TWindow* pere, NSContexte* pCtx,
                                string* pDatN, string* pZip, string* pRType,
                                string* pRMode, TModule* module = 0) ;
    ~PrometheComplementDlg() ;

    void    CmOk() ;
    void    CmCancel() ;
    void    EvClose() ;
    void    SetupWindow() ;

  DECLARE_RESPONSE_TABLE(PrometheComplementDlg) ;
} ;

#endif   // #ifdef __NSEPIDIV_H
