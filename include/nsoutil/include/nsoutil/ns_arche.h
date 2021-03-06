//---------------------------------------------------------------------------//     NS_ARCHE.H
//     PA   juillet 95
//  Contient les définitions des objets Matériel
//---------------------------------------------------------------------------
#ifndef __NS_ARCHE_H
#define __NS_ARCHE_H

#include <classlib\arrays.h>
#include "partage\ns_vector.h"

#include <owl\dialog.h>
#include <owl\listbox.h>
#include <owl\listwind.h>
#include <owl\radiobut.h>

#include "nsoutil\nsexport.h"
#include "nssavoir\ns_medic.h"
#include "nsbb\nsutidlg.h"
#include "nsbb\nsednum.h"

#include "pilot\NautilusPilot.hpp"

class NSArcheListWindow ;

class _NSOUTILCLASSE ArchetypesListDialog : public NSUtilDialog
{
	public:

  	int                _iType ;  // 0ARID or
		NSArcheListWindow* _pList ;
		NSDataTreeArray	   _aArray ;
		int	               _iSelectedArchetype ;

		ArchetypesListDialog(TWindow* pere, NSContexte* pCtx, int iType, TModule* module = 0) ;
    ~ArchetypesListDialog();

		void SetupWindow() ;
    bool InitArray() ;
    void InitListe() ;
    void AfficheListe() ;
    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void LVNColumnclick(TLwNotify& lwn) ;

    void CmOk() ;
    void CmCancel() ;

    void sortByID() ;
    void sortByFile() ;

	protected :

		bool _bNaturallySorted ;
		int  _iSortedColumn ;

    void swapSortOrder(int iColumn) ;

	DECLARE_RESPONSE_TABLE(ArchetypesListDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les archétypes)
//
class NSArcheListWindow : public TListWindow
{
	public:

		ArchetypesListDialog* pDlg ;

    NSArcheListWindow(ArchetypesListDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
    }
    ~NSArcheListWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSArcheListWindow) ;
};

//
// Choosing a referential
//

class NSRefChoiceListWindow ;

class RefInfoStorage
{
  public:

  string sObjectId ;
  string sRefId ;
  string sLabel ;
  string sType ;
  string sDomain ;

  RefInfoStorage() ;
  RefInfoStorage(RefInfoStorage& rv) ;
  ~RefInfoStorage() {}

  RefInfoStorage& operator=(RefInfoStorage src) ;
};

typedef vector<RefInfoStorage*> RefInfoStorageArray ;
typedef RefInfoStorageArray::iterator RefInfoStorageIter ;
typedef NTArray<RefInfoStorage> RefInfosStorageArray ;

class _NSOUTILCLASSE ReferentialChoiceDialog : public NSUtilDialog
{
	public:

    enum SORTTYPE { sortNone = 0, sortDrugs, sortRisks } ;

  	string                 _sType ;  // 0ARID or
    string                 _sConcept ;
    SORTTYPE               _sortType ;

		NSRefChoiceListWindow* _pList ;
		RefInfosStorageArray	 _aArray ;

    NSRefChoiceListWindow* _pListTrt ;
    RefInfosStorageArray	 _aArrayTrt ;

    bool                   _bUseSemanticNetwork ;

		ReferentialChoiceDialog(TWindow* pere, NSContexte* pCtx, LDVFRAME iFrame, string sType, SORTTYPE sortType = sortNone, string sConcept = string(""), TModule* module = 0) ;
    ~ReferentialChoiceDialog();

		void SetupWindow() ;
    bool InitArray() ;
    void InitListe() ;

    void AfficheListe() ;    void AfficheListeTrt() ;    void LvnGetDispInfo(TLwDispInfoNotify& dispInfo) ;
    void LVNColumnclick(TLwNotify& lwn) ;
    void SelChanged(TLwNotify& lwn) ;
    void LvnGetDispInfoTrt(TLwDispInfoNotify& dispInfo) ;
    void LVNColumnclickTrt(TLwNotify& lwn) ;
    void SelChangedTrt(TLwNotify& lwn) ;

    void CmOk() ;
    void CmCancel() ;

    void sortByName() ;
    void sortByNameTrt() ;

    RefInfoStorage* getSelected() ;
    RefInfoStorage* getSelectedIn1() ;
    RefInfoStorage* getSelectedIn2() ;

    RefInfoStorage* getSelectedRef() { return _pSelectedRef ; }
    string          getSelectedObjectId() ;
    string          getSelectedRefId() ;

	protected :

		bool _bNaturallySorted ;
		int  _iSortedColumn ;
    bool _bNaturallySortedTrt ;
		int  _iSortedColumnTrt ;

    LDVFRAME _iFrame ;

    RefInfoStorage* _pSelectedRef ;

    void swapSortOrder(int iColumn) ;
    void swapSortOrderTrt(int iColumn) ;

    bool getObjectTree(NSDataTree* pDataTree) ;
    void storeInList(RefInfoStorage* pStorage) ;

    void unselectAll() ;
    void unselectAllTrt() ;

	DECLARE_RESPONSE_TABLE(ReferentialChoiceDialog) ;
};

//
// Objet "ListWindow" avec gestion du double-click (pour les archétypes)
//
class NSRefChoiceListWindow : public TListWindow
{
	public:

		ReferentialChoiceDialog* pDlg ;

    NSRefChoiceListWindow(ReferentialChoiceDialog* pere, int resId) : TListWindow(pere, resId)
    {
    	pDlg = pere ;
    }
    ~NSRefChoiceListWindow() {}

    void EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point) ;
    int  IndexItemSelect() ;

	DECLARE_RESPONSE_TABLE(NSRefChoiceListWindow) ;
};

class _NSOUTILCLASSE ReferentialTitleDialog : public NSUtilDialog
{
	public:

  	string* _pTitle ;
    string* _pID ;
    string* _pConcept ;

		ReferentialTitleDialog(TWindow* pere, NSContexte* pCtx, string* pTitle, string* pID, string* pConcept, TModule* module = 0) ;
    ~ReferentialTitleDialog() ;

		void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

	protected :

		OWL::TEdit*    _pTitleEdit ;
    OWL::TStatic*  _pTitleTxt ;
    OWL::TEdit*    _pIDEdit ;
    OWL::TStatic*  _pIDTxt ;
    NSUtilLexique* _pConceptEdit ;
    OWL::TStatic*  _pConceptTxt ;

	DECLARE_RESPONSE_TABLE(ReferentialTitleDialog) ;
};

class _NSOUTILCLASSE ReferentialPropParamsDialog : public NSUtilDialog
{
	public:

  	string* _pTitle ;
    string* _pGroup ;
    string* _pHelp ;
    string* _pAutoCheck ;
    string* _pEvidenceLevel ;
    string* _pUncheckArchId ;

    string _sLongLabel ;
    string _sShortLabel ;

		ReferentialPropParamsDialog(TWindow* pere, NSContexte* pCtx, string* pTitle, string* pGroup, string* pHelp, string* pShort, string* pLong, string* pAutoCheck, string* pEvidenceLevel, string* pUncheckArchId, TModule* module = 0) ;
    ~ReferentialPropParamsDialog() ;

		void SetupWindow() ;

    void CmOk() ;
    void CmCancel() ;

	protected :

		OWL::TEdit*   _pTitleEdit ;
    OWL::TStatic* _pTitleTxt ;
    OWL::TEdit*   _pGroupEdit ;
    OWL::TStatic* _pGroupTxt ;
    OWL::TEdit*   _pHelpEdit ;
    OWL::TStatic* _pHelpTxt ;
    OWL::TEdit*   _pUncheckEdit ;
    OWL::TStatic* _pUncheckTxt ;

    OWL::TStatic* _pLongTxt ;
    OWL::TStatic* _pShortTxt ;

    OWL::TGroupBox*    _pAutoGroup ;
    OWL::TRadioButton* _pAutoYes ;
    OWL::TRadioButton* _pAutoNo ;

    OWL::TGroupBox*    _pEvidenceGroup ;
    OWL::TRadioButton* _pEvidenceA ;
    OWL::TRadioButton* _pEvidenceB ;
    OWL::TRadioButton* _pEvidenceC ;
    OWL::TRadioButton* _pEvidenceP ;
    OWL::TRadioButton* _pEvidenceU ;

	DECLARE_RESPONSE_TABLE(ReferentialPropParamsDialog) ;
};

#endif    // __NS_ARCHE_H

