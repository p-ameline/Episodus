// -----------------------------------------------------------------------------
// nsBdmDlg.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// Ligne de vie - Drugs database control dialogs
// -----------------------------------------------------------------------------
// PA  - april 2018
// ...
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Programme protégé par une licence Open Source GPL
// -----------------------------------------------------------------------------

#ifndef __NSBDMDLG_H
#define __NSBDMDLG_H

#include <classlib\arrays.h>

#include <owl\dialog.h>
#include <owl\slider.h>
#include <owl\gauge.h>
#include <owl\listwind.h>

#include "partage\ns_vector.h"
#include "partage\NTArray.h"
#include "nsoutil/nsexport.h"
#include "nsbb\nsednum.h"
#include "partage\NTArray.h"
#include "nsbb\nslistwind.h"
#include "nssavoir\NsBdmDriver.h"
#include "nsbb\NTFunctor.h"

class NSEditBdm ;
class NSSimpleNewDrugDlg ;
class NSDrugResearchDlg ;

class NSDrugResearchListWindow : public NSSkinableListWindow
{
  public:

    // constructors / destructor
    NSDrugResearchListWindow() ;
    NSDrugResearchListWindow(NSContexte* pCtx, NSDrugResearchDlg *parent, int iResId, TModule* module = (TModule*) 0) ;
    NSDrugResearchListWindow(NSContexte* pCtx, NSDrugResearchDlg *parent, int id, int x, int y, int w, int h, TModule* module = (TModule*) 0) ;
    ~NSDrugResearchListWindow() ;

    // Loading drugs list
    void    loadDrugs(const NsSelectableDrugArray* pDrugs) ;
    void    fillList() ;

    // réponse aux événements
    void    EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    void    DispInfoListe(TLwDispInfoNotify& dispInfo) ;

    void    sortByLabel() ;
    void    sortByPrice() ;
    void    sortAgain() ;
    void    LVNColumnclick(TLwNotify& lwn) ;
    void    changeSortOrder(int iCol) ;

    void    sortBy(int iCol) ;

    int               IndexItemSelect() ;
    NsSelectableDrug* getSelectedElement() ;

    void    SetupToolBar() ;
    void    SetupWindow() ;
    void    SetupColumns() ;
    void    AfficheListe() ;
    void    EvKillFocus(HWND hWndGetFocus) ;

    void    seedChanged(string sSeed) ;
    void    refreshList() ;

    void    processSelectionChange() ;
    void    selectCurrentDrugInList() ;

    void    CmSelectDrug() ;

    NSDrugResearchDlg* getDrugResearchDlg() { return _pParentDlg ; }

    bool    bSetupToolBar ;

  protected:

    NsSelectableDrugArray _aDrugs ;

    int                   _iPreviousSelection ;

    bool                  _bNaturallySorted ;
    int                   _iSortedColumn ;

    int                   _iRes ;
    NSDrugResearchDlg*    _pParentDlg ;

    void   refreshDrugsList() ;

    void   fillSelectedArray(string sSeed, size_t iRecordIndex = 0) ;
    int    getCutField(const string sSeed) ;
    int    getNewSelectedIndex(NsSelectableDrug* pPreviouslySelected) ;

    void   addToList(NsSelectableDrug* pPersonToAdd) ;

    string getLine(size_t* piStart, const string* psContent) ;

  DECLARE_RESPONSE_TABLE(NSDrugResearchListWindow) ;
} ;

/*
class NSDrugsRechEdit : public OWL::TEdit
{
	public:

		NSDrugResearchDlg* _pParentDlg ;

		// on bloque le premier Return (booleen false dans NSUtilEdit)
    NSDrugsRechEdit(NSDrugResearchDlg* pDRD, int resId) ;
		NSDrugsRechEdit(NSDrugResearchDlg* pDRD, int resId, const char far* text, int x, int y, int w, int h) ;
		~NSDrugsRechEdit() {}

		void EvChar(uint key, uint repeatCount, uint flags) ;
		void EvKeyUp(uint key, uint repeatCount, uint flags) ;
    void EvSetFocus(HWND hWndLostFocus) ;

    string GetTextAsString() ;

	DECLARE_RESPONSE_TABLE(NSDrugsRechEdit) ;
};
*/

//
// Looking for a drug in the BAM
//
class _NSOUTILCLASSE NSDrugResearchDlg : public NSUtilDialog
{
  public:

    // enum DRUGSEARCHBY { searchByUndefined = 0, searchBySubstance, searchByATC, searchByIndication } ;

    NSDrugResearchDlg(TWindow* pView, NSContexte *pCtx, string *pLexicode, NSBdmDriver::BAMTABLETYPE iSearchBy) ;
    ~NSDrugResearchDlg() ;

    void updateList() ;

  protected:

    NSEditBdm*                _pDrugSearchLabel ;
    NSDrugResearchListWindow* _pDrugsList ;
    OWL::TButton*             _pSearchButton ;

    void CmOk() ;
    void CmCancel() ;
    void CmSearch() ;
    void SetupWindow() ;

    void CmDrugDblClk(WPARAM Cmd) ;

    void NewCodeSelected() ;

    int  IndexItemSelect() ;

    string*                   _pLexicode ;
    NSBdmDriver::BAMTABLETYPE _iSearchBy ;

  DECLARE_RESPONSE_TABLE(NSDrugResearchDlg) ;
} ;

#endif // __NSBDMDLG_H

