#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\button.h>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsBdmDrugInfoDlg.h"

#include "nsoutil\nsBdmDlg.rh"

// -----------------------------------------------------------------
//
//  Méthodes de NSGenericBdmInfoDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSGenericBdmInfoDlg, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSGenericBdmInfoDlg::NSGenericBdmInfoDlg(TWindow* pere, NSContexte* pCtx, TResId resID)
                    :NSUtilDialog(pere, pCtx, resID, pNSResModule)
{
  _pInfoList = new NSBdmInfoListWindow(pCtx, this, IDC_DRGINFO_LIST) ;

  _iTopStart       = 210 ;    // Top of first button
  _iButtonHeight   =  12 ;    // Button's height
  _iLeftStart      =   8 ;    // Left margin
  _iButtonWidth    =  70 ;    // Button's width
  _iButtonSepar    =   3 ;    // Interval between 2 buttons on a line
  _iButtonInterval =   3 ;    // Interval between 2 lines
  _iButtonsPerLine =   4 ;    // Number of buttons per line
  _iTotalWidth     = 316 ;
  _iBorBtnHeight   =  25 ;
  _iOkBtnWidth     =  43 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSGenericBdmInfoDlg::~NSGenericBdmInfoDlg()
{
	delete _pInfoList ;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSGenericBdmInfoDlg::SetupWindow()
{
  SetCaption(getTitle().c_str()) ;

  // Create all information buttons
  //
  int iBtnTopPos = createButtons() ;

  // Add a OK button
  //
  int iOkBtnTopPos = iBtnTopPos + 10 ;

  int iOkBtnLeft  = (_iTotalWidth - _iOkBtnWidth) / 2 ;

  if (false == _aButtons.empty())
    iOkBtnTopPos += _iButtonHeight ;

  NS_CLASSLIB::TRect btnRect = NS_CLASSLIB::TRect(iOkBtnLeft, iOkBtnTopPos, iOkBtnLeft + _iOkBtnWidth, iOkBtnTopPos + _iBorBtnHeight) ;
  MapDialogRect(btnRect) ;

  _pOkBtn = new TButton(this, IDOK, "", btnRect.Left(), btnRect.Top(), btnRect.Width(), btnRect.Height(), /*isDefault*/ false, 0) ;

  NSUtilDialog::SetupWindow() ;

  fillList() ;

  resizeForButtons(iBtnTopPos) ;
}

int
NSGenericBdmInfoDlg::createButtons()
{
  int iBtnTopPos = _iTopStart ;

  NsHtmlLinksArray* pLinks = getLinks() ;
  if (((NsHtmlLinksArray*) NULL == pLinks) || pLinks->empty())
    return iBtnTopPos ;

  int iBtnLeftPos = _iLeftStart ;
  int iBtnCount   = -1 ;

  int iBtnResId = 201 ;

  for (NsHtmlLinkIter it = pLinks->begin() ; pLinks->end() != it ; it++)
  {
    if ((*it)->getRelTitle() == NsHtmlLink::relRelated)
    {
      // Increments all positioning information
      //
      iBtnCount++ ;
      if (iBtnCount >= _iButtonsPerLine)
      {
        iBtnCount = 0 ;

        iBtnLeftPos = _iLeftStart ;
        iBtnTopPos += _iButtonHeight + _iButtonInterval ;
      }
      else if (iBtnCount > 0)
        iBtnLeftPos += _iButtonWidth + _iButtonSepar ;

      // Prepare button's location and size
      //
      NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(iBtnLeftPos, iBtnTopPos, iBtnLeftPos + _iButtonWidth, iBtnTopPos + _iButtonHeight) ;
      MapDialogRect(cvtRect) ;

      // Create the button
      //
      string sText = pContexte->getSuperviseur()->getText("drugInformationTags", (*it)->getTitle()) ;

      NSBdmInfoButton* pBtn = new NSBdmInfoButton(pContexte, this, iBtnResId++, sText.c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
      pBtn->setUrl((*it)->getURL()) ;
      pBtn->setTitle(sText) ;

      _aButtons.push_back(pBtn) ;
    }
  }

  return iBtnTopPos ;
}

/**
 * Setting dialog box' height, including the OK button
 */
void
NSGenericBdmInfoDlg::resizeForButtons(int iCurrentBtnTopPos)
{
  int iTotalHeight = iCurrentBtnTopPos + _iButtonHeight + 10 + _iBorBtnHeight + 10 ;
  NS_CLASSLIB::TRect dlgSizeRect(0, 0, _iTotalWidth, iTotalHeight) ;  MapDialogRect(dlgSizeRect) ;

  // Redimentionnemen t
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;

  // On compare le clientRect réel avec les dimensions souhaitées,  // et on modifie le WindowRect en conséquence
  int nouvWindowHeight = dlgRect.Height() + (dlgSizeRect.Height() - clientRect.Height()) ;

  MoveWindow(dlgRect.left, dlgRect.top, dlgRect.Width(), nouvWindowHeight) ;
}

void
NSGenericBdmInfoDlg::relocateOkButton(int iCurrentBtnTopPos)
{
  int iOkBtnTopPos = iCurrentBtnTopPos + 10 ;

  int iOkBtnLeft  = (_iTotalWidth - _iOkBtnWidth) / 2 ;

  if (false == _aButtons.empty())
    iOkBtnTopPos += _iButtonHeight ;

  NS_CLASSLIB::TRect btnRect = NS_CLASSLIB::TRect(iOkBtnLeft, iOkBtnTopPos, iOkBtnLeft + _iOkBtnWidth, iOkBtnTopPos + _iBorBtnHeight) ;

  MapDialogRect(btnRect) ;

  _pOkBtn->MoveWindow(btnRect.left, btnRect.top, btnRect.Width(), btnRect.Height()) ;
}

bool
NSGenericBdmInfoDlg::isValidUrl(string sUrl)
{
  if (string("") == sUrl)
    return false ;

  size_t iUrlSize = strlen(sUrl.c_str()) ;

  if (iUrlSize <= 8)
    return false ;

  if ((string("http://") != string(sUrl, 0, 7)) && (string("https://") != string(sUrl, 0, 8)))
    return false ;

  return true ;
}

void
NSGenericBdmInfoDlg::openUrl(const string sUrl, const string sTitle)
{
  if (string("") == sUrl)
    return ;

  if (isValidUrl(sUrl))
  {
    ::ShellExecute(pContexte->GetMainWindow()->HWindow,
                 "open",
                 sUrl.c_str(),
                 "",
                 "",
                 SW_SHOWNORMAL) ;

    return ;
  }

  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if ((NSBdmDriver*) NULL == pDriver)
    return ;

  NsXmlBlocksArray aBlocks ;

  pDriver->getBlocksFromUrl(sUrl, &aBlocks) ;

  if (aBlocks.empty())
    return ;

  if (aBlocks.size() == 1)
  {
    NSBdmInfoDlg infoDlg(this, pContexte, *(aBlocks.begin())) ;
    infoDlg.Execute() ;
  }
  else
  {
    NSBdmMultiInfoDlg infoDlg(this, pContexte, &aBlocks, sTitle) ;
    infoDlg.Execute() ;
  }
}

void
NSGenericBdmInfoDlg::CmOk()
{
  NSUtilDialog::CmOk() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSBdmInfoDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSBdmInfoDlg, NSGenericBdmInfoDlg)
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBdmInfoDlg::NSBdmInfoDlg(TWindow* pere, NSContexte* pCtx, NsXmlBlock* pBlock)
             :NSGenericBdmInfoDlg(pere, pCtx, "DRUGS_INFO_DLG")
{
  _pBlock      = pBlock ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSBdmInfoDlg::~NSBdmInfoDlg()
{
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSBdmInfoDlg::SetupWindow()
{
  NSGenericBdmInfoDlg::SetupWindow() ;
}

void
NSBdmInfoDlg::fillList()
{
  _pInfoList->fillList(_pBlock->getEntries()) ;
}

void
NSBdmInfoDlg::activatedInformation(int iIndex)
{
  if (iIndex < 0)
    return ;

  // Get the activated value
  //
  NsXmlEntriesArray* pEntries = _pBlock->getEntries() ;
  if (pEntries->empty() || ((size_t) iIndex >= pEntries->size()))
    return ;

  NsXmlEntry* pEntry = (*pEntries)[iIndex] ;
  if ((NsXmlEntry*) NULL == pEntry)
    return ;

  string sValue = pEntry->getValue() ;

  // If it is a valid URL, open the page
  //
  if (isValidUrl(sValue))
    NSGenericBdmInfoDlg::openUrl(sValue, pEntry->getField()) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSBdmMultiInfoDlg
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSBdmMultiInfoDlg, NSGenericBdmInfoDlg)
  EV_LBN_SELCHANGE(IDC_DRGINFO_TITLE_LIST, CmSelChanged),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSBdmMultiInfoDlg::NSBdmMultiInfoDlg(TWindow* pere, NSContexte* pCtx, NsXmlBlocksArray* paBlocks, string sTitle)
                  :NSGenericBdmInfoDlg(pere, pCtx, "DRUGS_INFO_MULTI_DLG")
{
  _paBlocks = paBlocks ;
  _sTitle   = sTitle ;

  // Initialize the selected block as being the first in the list
  //
  if (_paBlocks && (false == _paBlocks->empty()))
    _sSelectedBlockId = (*(_paBlocks->begin()))->getUniqueId() ;

  _pTitlesList = new NSBdmInfoTitlesListBox(this, IDC_DRGINFO_TITLE_LIST) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSBdmMultiInfoDlg::~NSBdmMultiInfoDlg()
{
  delete _pTitlesList ;
}

//---------------------------------------------------------------------------
//  Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NSBdmMultiInfoDlg::SetupWindow()
{
  NSGenericBdmInfoDlg::SetupWindow() ;

  _pTitlesList->fillList(_paBlocks) ;
  _pTitlesList->SetSelIndex(0) ;
}

void
NSBdmMultiInfoDlg::fillList()
{
  NsXmlBlock* pBlock = getSelectedBlock() ;
  if ((NsXmlBlock*) NULL == pBlock)
    return ;

  _pInfoList->fillList(pBlock->getEntries()) ;
}

void
NSBdmMultiInfoDlg::activatedInformation(int iIndex)
{
  if (iIndex < 0)
    return ;

  NsXmlBlock* pBlock = getSelectedBlock() ;
  if ((NsXmlBlock*) NULL == pBlock)
    return ;

  // Get the activated value
  //
  NsXmlEntriesArray* pEntries = pBlock->getEntries() ;
  if (pEntries->empty() || ((size_t) iIndex >= pEntries->size()))
    return ;

  NsXmlEntry* pEntry = (*pEntries)[iIndex] ;
  if ((NsXmlEntry*) NULL == pEntry)
    return ;

  string sValue = pEntry->getValue() ;

  // If it is a valid URL, open the page
  //
  if (isValidUrl(sValue))
    NSGenericBdmInfoDlg::openUrl(sValue, pEntry->getField()) ;
}

void
NSBdmMultiInfoDlg::selectBlock(string sBlockId)
{
  _sSelectedBlockId = sBlockId ;

  fillList() ;

  updateButtons() ;
}

NsHtmlLinksArray*
NSBdmMultiInfoDlg::getLinks()
{
  NsXmlBlock* pBlock = getSelectedBlock() ;

  if ((NsXmlBlock*) NULL == pBlock)
    return (NsHtmlLinksArray*) 0 ;

  return pBlock->getLinks() ;
}

void
NSBdmMultiInfoDlg::updateButtons()
{
  NsXmlBlock* pBlock = getSelectedBlock() ;
  if ((NsXmlBlock*) NULL == pBlock)
    return ;

  NsHtmlLinksArray* pLinks = pBlock->getLinks() ;

  if (pLinks->size() == _aButtons.size())
    updateButtonsContents(pLinks) ;
  else
    updateButtons(pLinks) ;
}

void
NSBdmMultiInfoDlg::updateButtons(NsHtmlLinksArray* pLinks)
{
  if (((NsHtmlLinksArray*) NULL == pLinks) || pLinks->empty())
  {
    _aButtons.vider() ;
    resizeForButtons(_iTopStart) ;
    return ;
  }

  int iTopPos = createButtons() ;
  resizeForButtons(iTopPos) ;
  relocateOkButton(iTopPos) ;
}

void
NSBdmMultiInfoDlg::updateButtonsContents(NsHtmlLinksArray* pLinks)
{
  if (((NsHtmlLinksArray*) NULL == pLinks) || pLinks->empty())
    return ;

  if (_aButtons.empty())
    return ;

  NsHtmlLinkIter lnkIt = pLinks->begin() ;

  for (NSBdmInfoButtonIter btnIt = _aButtons.begin() ; (_aButtons.end() != btnIt) && (pLinks->begin() != lnkIt) ; btnIt++, lnkIt++)
    (*btnIt)->Update((*lnkIt)->getURL(), (*lnkIt)->getTitle()) ;
}

void
NSBdmMultiInfoDlg::removeButtons()
{
  if (_aButtons.empty())
    return ;

  _aButtons.vider() ;
}

void
NSBdmMultiInfoDlg::CmSelChanged()
{
  int index = _pTitlesList->GetSelIndex() ;

	if ((index < 0) || (index >= (int) _paBlocks->size()))    return ;  NsXmlBlock *pBlock = (*_paBlocks)[index] ;  if (pBlock)    selectBlock(pBlock->getUniqueId()) ;
}

NsXmlBlock*
NSBdmMultiInfoDlg::getSelectedBlock()
{
  if (((NsXmlBlocksArray*) NULL == _paBlocks) || _paBlocks->empty())
    return (NsXmlBlock*) 0 ;

  for (NsXmlBlockIter it = _paBlocks->begin() ; _paBlocks->end() != it ; it++)
    if ((*it)->getUniqueId() == _sSelectedBlockId)
      return *it ;

  return (NsXmlBlock*) 0 ;
}

/**
 * NSBdmDrugInfoListWindow
 *
 * Information from drug database that can be displayed for drug selection
 */

DEFINE_RESPONSE_TABLE1(NSBdmInfoListWindow, TListWindow)
  EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE ;

NSBdmInfoListWindow::NSBdmInfoListWindow(NSContexte* pCtx, NSGenericBdmInfoDlg *parent, int id, int x, int y, int w, int h, TModule *module)
                    :NSSkinableListWindow((TWindow *) parent, pCtx, id, x, y, w, h, module)
{
  _pParentDlg = parent ;
  _iRes       = id ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

NSBdmInfoListWindow::NSBdmInfoListWindow(NSContexte* pCtx, NSGenericBdmInfoDlg *parent, int iResId, TModule *module)
                    :NSSkinableListWindow((TWindow *) parent, pCtx, iResId, module)
{
  _pParentDlg = parent ;
  _iRes       = iResId ;
  Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;
}

NSBdmInfoListWindow::~NSBdmInfoListWindow()
{
}

void
NSBdmInfoListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOn("InPatientsListOn") ;

  SetupColumns() ;
}

void
NSBdmInfoListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (_pParentDlg)
    _pParentDlg->activatedInformation(indexItem) ;
}

void
NSBdmInfoListWindow::SetupColumns()
{
  string sLabel = string("Libellé") ;
  InsertColumn(0, TListWindColumn((char far*) sLabel.c_str(), 180, TListWindColumn::Left, 0)) ;

  string sPrice = string("Information") ;
  InsertColumn(1, TListWindColumn((char far*) sPrice.c_str(), 260, TListWindColumn::Left, 1)) ;
}

void
NSBdmInfoListWindow::fillList(NsXmlEntriesArray* paXmlEntriesArray)
{
  DeleteAllItems() ;

  if (((NsXmlEntriesArray*) NULL == paXmlEntriesArray) || paXmlEntriesArray->empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  //
  int iLine = 0 ;

  for (NsXmlEntryIter it = paXmlEntriesArray->begin() ; paXmlEntriesArray->end() != it ; it++)
    InsertItems((*it)->getField(), (*it)->getValue(), iLine++) ;

  Invalidate() ;
}

void
NSBdmInfoListWindow::InsertItems(const string sLabel, const string sValue, int iLine)
{
  string sText = pContexte->getSuperviseur()->getText("drugInformationTags", sLabel) ;
  TListWindItem Item(sText.c_str(), 0) ;
  Item.SetIndex(iLine) ;
  InsertItem(Item) ;

  TListWindItem Item2(sValue.c_str(), 1) ;
  Item2.SetIndex(iLine) ;
  Item2.SetSubItem(1) ;
  SetItem(Item2) ;
}

/**
 * NSBdmInfoTitlesListWindow
 *
 * Titles of blocks to be displayed
 */

DEFINE_RESPONSE_TABLE1(NSBdmInfoTitlesListBox, TListBox)
  EV_LBN_SELCHANGE(IDC_DRGINFO_TITLE_LIST, CmSelChanged),
END_RESPONSE_TABLE ;

NSBdmInfoTitlesListBox::NSBdmInfoTitlesListBox(NSBdmMultiInfoDlg *parent, int id, int x, int y, int w, int h, TModule *module)
                       :TListBox((TWindow *) parent, id, x, y, w, h, module)
{
  _pParentDlg = parent ;
}

NSBdmInfoTitlesListBox::NSBdmInfoTitlesListBox(NSBdmMultiInfoDlg *parent, int iResId, TModule *module)
                       :TListBox((TWindow *) parent, iResId, module)
{
  _pParentDlg = parent ;
}

NSBdmInfoTitlesListBox::~NSBdmInfoTitlesListBox()
{
}

void
NSBdmInfoTitlesListBox::SetupWindow()
{
}

void
NSBdmInfoTitlesListBox::fillList(NsXmlBlocksArray* paBlocks)
{
  ClearList() ;

  _paBlocks = paBlocks ;

  if (((NsXmlBlocksArray*) NULL == _paBlocks) || _paBlocks->empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  //
  for (NsXmlBlockIter it = _paBlocks->begin() ; _paBlocks->end() != it ; it++)
  {
    string sText = (*it)->getTitle() ;
    AddString(sText.c_str()) ;
  }

  Invalidate() ;
}

void
NSBdmInfoTitlesListBox::CmSelChanged()
{
  int index = GetSelIndex() ;

	if ((index < 0) || (index >= (int) _paBlocks->size()))    return ;  NsXmlBlock *pBlock = (*_paBlocks)[index] ;  if (pBlock)    _pParentDlg->selectBlock(pBlock->getUniqueId()) ;
}

DEFINE_RESPONSE_TABLE1(NSBdmInfoButton, TButton)
  EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE ;

NSBdmInfoButton::NSBdmInfoButton(NSContexte *pCtx, NSGenericBdmInfoDlg* parent, int resId)
                :TButton(parent, resId), NSRoot(pCtx)
{
  _sUrl   = string("") ;
  _sTitle = string("") ;

  _pDrugInfoDlg = parent ;

  DisableTransfer() ;
}

NSBdmInfoButton::NSBdmInfoButton(NSContexte *pCtx, NSGenericBdmInfoDlg* parent, int resId,
                                 const char far* text, int X, int Y, int W, int H,
                                 bool isDefault, TModule* module)
                :TButton(parent, resId, text, X, Y, W, H, isDefault, module), NSRoot(pCtx)
{
  _sUrl   = string("") ;
  _sTitle = string("") ;

  _pDrugInfoDlg = parent ;

  DisableTransfer() ;
}

NSBdmInfoButton::~NSBdmInfoButton()
{
}

void
NSBdmInfoButton::BNClicked()
{
  if ((NSGenericBdmInfoDlg*) NULL == _pDrugInfoDlg)
    return ;

  _pDrugInfoDlg->openUrl(_sUrl, _sTitle) ;
}

void
NSBdmInfoButton::SetupWindow()
{
  OWL::TButton::SetupWindow() ;
}

void
NSBdmInfoButton::Update(const string sUrl, const string sTitle)
{
  _sUrl   = sUrl ;
  _sTitle = sTitle ;

  SetCaption(_sTitle.c_str()) ;
}

char far*
NSBdmInfoButton::GetClassName()
{
  return "BUTTON" ;
}

