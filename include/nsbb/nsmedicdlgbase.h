// -----------------------------------------------------------------------------
// nsmedicdlg.h
// -----------------------------------------------------------------------------
// $Revision: 1.2 $
// $Author: pameline $
// $Date: 2009/07/23 17:03:17 $
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// FLP  - novembre 2003
// -----------------------------------------------------------------------------
                                     
#ifndef __NSMEDICDLGBASE_H__
# define __NSMEDICDLGBASE_H__

# include <owl/tabctrl.h>
# include <owl/commctrl.h>
# include <owl/groupbox.h>
# include <owl/listview.h>
# include <owl/radiobut.h>
# include <vector>

class NSEditNum ;
class NSComboBox ;
class NSUtilEditDateHeure ;

# include "nssavoir\nsBdmDriver.h"

# include "nsbb\nsExport.h"
# include "nsbb\nspatpat.h"
# include "nsbb\nsedilex.h"
# include "nsbb\NSLabel.h"
# include "nsbb\NTListBox.h"

/*
** Fonction qui renvoie l'unite de prise en fonction
** du libele du medicament
*/
std::string _NSBBCLASSE initDispUnit(string sLang, string sCode, string sLib, NSContexte* pContexte) ;
std::string _NSBBCLASSE initDispUnitFr(string sCode, string sLib, NSContexte* pContexte) ;

/*
** Cree le label à afficher pour la prescription par heure
*/
std::string CreateLabelForHour(std::string quant, std::string heure) ;

//-----------------------------------------------------
// Base class for prescription dialogs
//-----------------------------------------------------
class _NSBBCLASSE NSPrescriptionBaseDlg : public NSUtilDialog
{
  public:

    NSPrescriptionBaseDlg(TWindow* parent, NSContexte* pCtx, TResId resID, OWL::TModule* module = (OWL::TModule*) 0) ;
    virtual ~NSPrescriptionBaseDlg() ;

    void SetupWindow() ;

    // Issues connection functions
    //
    void initializeConnectedIssues(string sDrugNode) ;
    void initConnectedIssues(VecteurString* pLinkArray) ;

    std::string    getLexiqCode()            { return _sLexiqCode ; }
    VecteurString* getUpdatedLinkedConcern() { return &_aUpdatedLinkedIssues ; }

  protected:

    std::string         _sEditedNodeId ;  // in edit mode, drug's node id
    NSPatPathoArray*    _pPPT ;

    // Parsed information
    //
    std::string         _sLexiqCode ;     // code lexique du médicament
    std::string         _sPriseUnit ;
    std::string         _sATCCode ;       // Code ATC
    std::string         _sCICode ;        // Code CIS ou CIP
    std::string         _sDateOuverture ; // date d'ouverture
    std::string         _sDateFermeture ; // date de fermeture
    std::string         _sFreeText ;      // texte libre
    std::string         _sALD ;           // ALD ou non
    std::string         _sPositionRepas ; // Position par rapport aux repas
    bool                _bNonSubstituable ;

    std::string         _sLexiqRoute ;    // code lexique "voie d'administration"
    std::string         _sFreeTextRoute ; // texte libre "voie d'administration"

    std::string         _sLexiqEvent ;    // code lexique "en cas de"
    std::string         _sFreeTextEvent ; // texte libre "en cas de"

    VecteurString       _aLinkedIssues ;
    VecteurString       _aUpdatedLinkedIssues ;

    // Controls
    //
    NSUtilLexique       *_pDrug ;

    OWL::TStatic        *_pUnitePriseTxt ;
    NSUtilLexique       *_pUnitePrise ;
    OWL::TStatic        *_pRouteTxt ;
    NSUtilLexique       *_pRoute ;
    OWL::TStatic        *_pEventTxt ;
    NSUtilLexique       *_pEvent ;

    OWL::TStatic        *_pDateDebTxt ;
    NSUtilEditDateHeure *_pDateDeb ;
    OWL::TGroupBox      *_pDateFinGroup ;
    OWL::TRadioButton   *_pRChronique ;
    OWL::TRadioButton   *_pRDans ;
    OWL::TRadioButton   *_pRDuree ;
    OWL::TRadioButton   *_pRLe ;
    NSEditNum           *_pNbJours ;
    NSComboBox          *_pCBDureeTtt ;
    NSUtilEditDateHeure *_pDateFin ;

    OWL::TCheckBox      *_pALD ;
    OWL::TCheckBox      *_pIssues ;
    OWL::TCheckBox      *_pNonSubstituable ;

    OWL::TButton        *_pFreeTextButton ;

    // BDM related information
    //
    OWL::TButton        *_pDrugInfoButton ;
    OWL::TButton        *_pBdmSearchIndicationButton ;
    OWL::TButton        *_pBdmSearchSubstanceButton ;
    OWL::TButton        *_pBdmSearchAtcButton ;

    NsSelectableDrug    *_pBdmDrugInformation ;

    void createInterfaceElements() ;
    void initInterfaceElements() ;

    virtual void ExecutedAfterDrugSelection()  = 0 ;
    virtual void ExecutedAfterTrtBeginDate()   = 0 ;
    virtual void ExecutedAfterTrtEndDate()     = 0 ;
    virtual void ExecutedAfterEventSelection() = 0 ;
    virtual void ExecutedAfterRouteSelection() = 0 ;

    void editFreeText() ;

    // Drug database related functions
    //
    void getDrugInformation() ;
    void checkDrugSafety() ;
    void checkSecurityCheckInformation() ;
    void checkSportEnabledDrug() ;
    void checkRedundancy() ;
    void checkAllergies() ;

    void searchInBdmByIndication() ;
    void searchInBdmBySubstance() ;
    void searchInBdmByATC() ;
    void searchInBdm(NSBdmDriver::BAMTABLETYPE iSearchBy) ;

    void drugInformation() ;

    // Issues connection functions
    //
    void selectIndication() ;

    void getCurrentConcerns(ArrayConcern *pCurrentConcerns) ;

    bool isAConnectedIssue(string sConcernNode) ;
    bool wasAConnectedIssue(string sConcernNode) ;
    bool isAConnected(string sConcernNode, VecteurString* pLinkArray) ;
    void setSelectedIssuesBtnState() ;

    void addConnectedIssue(string sConcernNode) ;

    void updateConnectedIssues(string sDrugNode) ;
    void UnconnectIssue(string sDrugNode, string sConcernNode) ;
    void ConnectIssue(string sDrugNode, string sConcernNode) ;
} ;

class NSIssuesListWindow ;

class NSSelectIssuesDlg : public NSUtilDialog
{
  public:

    int*                _pSelectedIssues ;
    size_t              _iSelectedCount ;
    ArrayConcern*       _pIssuesArray ;
    NSIssuesListWindow* _pIssuesList ;

    NSSelectIssuesDlg(TWindow* pPere, NSContexte* pCtx, ArrayConcern* pIssues, int* pSelectedIssues, int iSelectedCount) ;
    ~NSSelectIssuesDlg() ;

    void CmOk() ;
    void CmCancel() ;
    void SetupWindow() ;
    void dblClickOnIssue() ;

    void InitIssuesList() ;
    void DisplayIssuesList() ;
    void DispInfoIssuesList(TLwDispInfoNotify& dispInfo) ;

  DECLARE_RESPONSE_TABLE(NSSelectIssuesDlg) ;
};

//
// "ListWindow" objet with double-click management (to select an issue)
//
class NSIssuesListWindow : public TListWindow
{
  public:

    NSSelectIssuesDlg* _pView ;    int                _iRes ;
    NSIssuesListWindow(NSSelectIssuesDlg* pere, int resId, TModule* module = 0) ;    ~NSIssuesListWindow() {}
    void 	SetupWindow() ;
    void  EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;    int   IndexItemSelect() ;

    void  EvSetFocus(HWND hWndLostFocus) ;

  DECLARE_RESPONSE_TABLE(NSIssuesListWindow) ;};

#endif // __NSMEDICDLGBASE_H__

