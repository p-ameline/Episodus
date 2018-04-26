#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\button.h>
#include <owl\edit.h>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsBdmDlg.h"
#include "nsoutil\nsDicoBdm.h"
#include "nsoutil\ibdm.h"

#include "nsoutil\nsBdmDlg.rh"

bool
DrugSortByLabelInf(const NsSelectableDrug *pObj1, const NsSelectableDrug *pObj2)
{
  if (((NsSelectableDrug*) NULL == pObj1) || ((NsSelectableDrug*) NULL == pObj2))
    return false ;

	return (pObj1->getLabel() < pObj2->getLabel()) ;
}boolDrugSortByLabelSup(const NsSelectableDrug *pObj1, const NsSelectableDrug *pObj2)
{
  if (((NsSelectableDrug*) NULL == pObj1) || ((NsSelectableDrug*) NULL == pObj2))
    return false ;

	return (pObj1->getLabel() > pObj2->getLabel()) ;
}
bool
DrugSortByPriceInf(const NsSelectableDrug *pObj1, const NsSelectableDrug *pObj2)
{
  if (((NsSelectableDrug*) NULL == pObj1) || ((NsSelectableDrug*) NULL == pObj2))
    return false ;

	return (pObj1->getPrice() < pObj2->getPrice()) ;
}boolDrugSortByPriceSup(const NsSelectableDrug *pObj1, const NsSelectableDrug *pObj2)
{
  if (((NsSelectableDrug*) NULL == pObj1) || ((NsSelectableDrug*) NULL == pObj2))
    return false ;

	return (pObj1->getPrice() > pObj2->getPrice()) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSDrugResearchDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSDrugResearchDlg, NSUtilDialog)
  EV_COMMAND(IDOK,                   CmOk),
  EV_COMMAND(IDCANCEL,               CmCancel),
  EV_COMMAND(IDC_DRGSRCH_SEARCH_BTN, CmSearch),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSDrugResearchDlg::NSDrugResearchDlg(TWindow* pere, NSContexte* pCtx, string *pLexicode, NSBdmDriver::BAMTABLETYPE iSearchBy)
                  :NSUtilDialog(pere, pCtx, "DRUGS_SEARCH_DLG", pNSResModule)
{
  _pLexicode = pLexicode ;
  _iSearchBy = iSearchBy ;

  _pDrugSearchLabel = new NSEditBdm(pCtx, this, IDC_DRGSRCH_EDIT, iSearchBy) ;
  _pDrugsList       = new NSDrugResearchListWindow(pCtx, this, IDC_DRGSRCH_LIST) ;
  _pSearchButton    = new OWL::TButton(this, IDC_DRGSRCH_SEARCH_BTN) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSDrugResearchDlg::~NSDrugResearchDlg()
{
	delete _pDrugSearchLabel ;
	delete _pDrugsList ;
  delete _pSearchButton ;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSDrugResearchDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  _pDrugSearchLabel->setGetCodeFunctor(new MemFunctor<NSDrugResearchDlg>((NSDrugResearchDlg*)this, &NSDrugResearchDlg::NewCodeSelected)) ;
}

// -----------------------------------------------------------------------------
// Get index of first selected item
// -----------------------------------------------------------------------------
int
NSDrugResearchDlg::IndexItemSelect()
{
	return _pDrugsList->IndexItemSelect() ;
}

void
NSDrugResearchDlg::CmOk()
{
  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

  NsSelectableDrug* pDrug = _pDrugsList->getSelectedElement() ;
  if ((NsSelectableDrug*) NULL == pDrug)
    return ;

  string sBamID   = pDrug->getBdmID() ;
  string sCisCode = pDrug->getCIS() ;

  InterfaceBdm IBdm(pContexte) ;
  if (false == IBdm.isCodeCIPInDB(sCisCode))
    return ;

  string sCode = IBdm.pBdm->getLeximed() ;
  pContexte->getDico()->donneCodeComplet(sCode) ;

  *_pLexicode = sCode ;

  NSUtilDialog::CmOk() ;
}

void
NSDrugResearchDlg::CmCancel()
{
  NSUtilDialog::CmCancel() ;
}

void
NSDrugResearchDlg::NewCodeSelected()
{
  CmSearch() ;
}

void
NSDrugResearchDlg::CmSearch()
{
  string sSearchSeed = _pDrugSearchLabel->getCode() ;
  if (string("") == sSearchSeed)
    return ;

  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return ;

  NsSelectableDrugArray aDrugs ;

  pDriver->getDrugsForCriteria(&aDrugs, &sSearchSeed, _iSearchBy) ;

  _pDrugsList->loadDrugs(&aDrugs) ;
}

void
NSDrugResearchDlg::updateList()
{
  string sRoot = _pDrugSearchLabel->GetSelectedCode() ;
  if (string("") == sRoot)
    _pDrugsList->loadDrugs((NsSelectableDrugArray*) 0) ;


}

/**
 * NsSelectableDrug
 *
 * Information from drug database that can be displayed for drug selection
 */

DEFINE_RESPONSE_TABLE1(NSDrugResearchListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
  NS_LVN_GETDISPINFO(DispInfoListe),
  NS_LW_COLUMNCLICK(LVNColumnclick),
END_RESPONSE_TABLE ;

NSDrugResearchListWindow::NSDrugResearchListWindow(NSContexte* pCtx, NSDrugResearchDlg *parent, int id, int x, int y, int w, int h, TModule *module)
                         :NSSkinableListWindow((TWindow *) parent, pCtx, id, x, y, w, h, module)
{
  _pParentDlg = parent ;
  _iRes       = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	_iPreviousSelection = -1 ;
}

NSDrugResearchListWindow::NSDrugResearchListWindow(NSContexte* pCtx, NSDrugResearchDlg *parent, int iResId, TModule *module)
                         :NSSkinableListWindow((TWindow *) parent, pCtx, iResId, module)
{
  _pParentDlg = parent ;
  _iRes       = iResId ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	_iPreviousSelection = -1 ;
}

NSDrugResearchListWindow::~NSDrugResearchListWindow()
{
}

void
NSDrugResearchListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOn("InPatientsListOn") ;

  SetupColumns() ;
}

void
NSDrugResearchListWindow::loadDrugs(const NsSelectableDrugArray* pDrugs)
{
  _aDrugs.vider() ;

  if (((NsSelectableDrugArray*) NULL == pDrugs) || pDrugs->empty())
    return ;

  NsSelectableDrugConstIter drugIt = pDrugs->begin() ;
  for ( ; pDrugs->end() != drugIt ; drugIt++)
    _aDrugs.push_back(new NsSelectableDrug(**drugIt)) ;

  refreshList() ;
}

void
NSDrugResearchListWindow::SetupColumns()
{
  string sLabel = string("Libellé") ;
  InsertColumn(0, TListWindColumn((char far*) sLabel.c_str(), 350, TListWindColumn::Left, 0)) ;

  string sPrice = string("Prix") ;
  InsertColumn(1, TListWindColumn((char far*) sPrice.c_str(), 80, TListWindColumn::Right, 1)) ;
}

void
NSDrugResearchListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
}

void
NSDrugResearchListWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  TListWindItem &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;
  int iCol  = dispInfoItem.GetSubItem() ;

  if (index >= _aDrugs.size())
    return ;

  NsSelectableDrug* pDrug = _aDrugs[index] ;
  if ((NsSelectableDrug*) NULL == pDrug)
    return ;

  static char szBuffer[1024] ;

  string sText = string("") ;

  if      (0 == iCol)
    sText = pDrug->getLabel() ;
  else if (1 == iCol)
    sText = pDrug->getPrice() ;

  dispInfoItem.SetText(sText.c_str()) ;
}

void
NSDrugResearchListWindow::LVNColumnclick(TLwNotify& lwn)
{
  int iCol = lwn.iSubItem ;

  changeSortOrder(iCol) ;
  sortBy(iCol) ;

  refreshList() ;
}

void
NSDrugResearchListWindow::changeSortOrder(int iCol)
{
	if (_iSortedColumn == iCol)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iCol ;
    _bNaturallySorted = true ;
  }
}

void
NSDrugResearchListWindow::sortBy(int iCol)
{
  if (_aDrugs.empty())
    return ;

  if      (0 == iCol)
    sortByLabel() ;
  else if (1 == iCol)
    sortByPrice() ;

  _iSortedColumn = iCol ;
}

void
NSDrugResearchListWindow::sortByLabel()
{
  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), DrugSortByLabelInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), DrugSortByLabelSup) ;
}

void
NSDrugResearchListWindow::sortByPrice()
{
  if (_aDrugs.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aDrugs.begin(), _aDrugs.end(), DrugSortByPriceInf) ;
  else
    sort(_aDrugs.begin(), _aDrugs.end(), DrugSortByPriceSup) ;
}

void
NSDrugResearchListWindow::refreshList()
{
  DeleteAllItems() ;

  if (_aDrugs.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NsSelectableDrugReverseIter drugIt = _aDrugs.rbegin() ;
  do
  {
    string sFirstInfo = (*drugIt)->getLabel() ;
    TListWindItem Item(sFirstInfo.c_str(), 0) ;

    InsertItem(Item) ;

    drugIt++ ;
	}
  while (_aDrugs.rend() != drugIt) ;
}

// -----------------------------------------------------------------------------
// Get index of first selected item
// -----------------------------------------------------------------------------
int
NSDrugResearchListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

NsSelectableDrug*
NSDrugResearchListWindow::getSelectedElement()
{
  int iSelected = IndexItemSelect() ;
  if (-1 == iSelected)
    return (NsSelectableDrug*) 0 ;

  return _aDrugs[iSelected] ;
}

/**
 * NSDrugsRechEdit
 *
 * Get the root information to look for in drugs database
 */

/*
DEFINE_RESPONSE_TABLE1(NSDrugsRechEdit, OWL::TEdit)
  EV_WM_CHAR,
  EV_WM_KEYUP,
END_RESPONSE_TABLE;

NSDrugsRechEdit::NSDrugsRechEdit(NSDrugResearchDlg* pDRD, int resId)
                :OWL::TEdit((TWindow*) pDRD, resId, 0)
{
  _pParentDlg = pDRD ;
}

NSDrugsRechEdit::NSDrugsRechEdit(NSDrugResearchDlg* pDRD, int resId, const char far* text, int x, int y, int w, int h)
                :OWL::TEdit((TWindow*) pDRD, resId, text, x, y, w, h, 0)
{
  _pParentDlg = pDRD ;
}

string
NSDrugsRechEdit::GetTextAsString()
{
  int iBuffLen = GetTextLen() ;
  if (iBuffLen <= 0)
    return string("") ;

  char far* szBuff = new char[iBuffLen + 1] ;
  GetText(szBuff, iBuffLen + 1) ;  string sContenu = string(szBuff) ;

  delete[] szBuff ;

  return sContenu ;
}

//
// On doit intercepter EvChar pour interdire l'espace et le double-espace
//
void
NSDrugsRechEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	string sNom = GetTextAsString() ;

	// on interdit l'espace comme premier caractere
	if ((string("") == sNom) && (VK_SPACE == key))
		return ;

  size_t iNameLen = strlen(sNom.c_str()) ;

	// on interdit le double espace
	if ((iNameLen > 0) && (' ' == sNom[iNameLen - 1]) && (VK_SPACE == key))
		return ;

	OWL::TEdit::EvChar(key, repeatCount, flags) ;
}

//
// On doit intercepter EvKeyUp pour tenir compte de VK_DELETE
// qui n'est pas intercepté par EvChar
//
void
NSDrugsRechEdit::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	OWL::TEdit::EvKeyUp(key, repeatCount, flags) ;

  if (VK_RETURN == key)
    _pParentDlg->updateList() ;
}

*/

