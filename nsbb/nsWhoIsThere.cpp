// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>#include <vcl.h>
#pragma hdrstop

#include <owl/gauge.h>
#include <owl/static.h>

#include "nautilus\nssuper.h"
#include "nsepisod\nsCsvParser.h"
#include "nsbb\nsWhoIsThere.h"#include "nautilus\nsdocview.h"#include "nautilus\nsepicap.h"#include "nautilus\nsmdiframe.h"#include "nsoutil\nsoutdlg.h"#include "nsbb\nslistwind.h"#include "nsbb\nsPaneSplitter.h"#include "nssavoir\nscapture.h"#include "ns_sgbd\ns_odbc.h"#include "nsepisod\nsToDo.h"#include "partage\ns_timer.h"#include "partage\nsdivfile.h"#include "nsbb\nsbb.h"long NSInsidePersonInformation::lObjectCount = 0 ;long NSInsidePerson::lObjectCount = 0 ;long NSInPersonField::lObjectCount = 0 ;const int ID_PersonsList = 0x100 ;boolFieldSortOrderInf(NSInPersonField *pObj1, NSInPersonField *pObj2)
{
  if (((NSInPersonField*) NULL == pObj1) || ((NSInPersonField*) NULL == pObj2))
    return false ;

	return (pObj1->getOrder() < pObj2->getOrder()) ;
}boolFieldSortOrderSup(NSInPersonField *pObj1, NSInPersonField *pObj2)
{
  if (((NSInPersonField*) NULL == pObj1) || ((NSInPersonField*) NULL == pObj2))
    return false ;

	return (pObj1->getOrder() > pObj2->getOrder()) ;
}

bool
FieldSortNumberInf(NSInPersonField *pObj1, NSInPersonField *pObj2)
{
  if (((NSInPersonField*) NULL == pObj1) || ((NSInPersonField*) NULL == pObj2))
    return false ;

	return (pObj1->getNumber() < pObj2->getNumber()) ;
}boolFieldSortNumberSup(NSInPersonField *pObj1, NSInPersonField *pObj2)
{
  if (((NSInPersonField*) NULL == pObj1) || ((NSInPersonField*) NULL == pObj2))
    return false ;

	return (pObj1->getNumber() > pObj2->getNumber()) ;
}
/****************** classe NSInPersonsWindow **************************/
#define CM_RECH_EDIT    101
#define CM_RECH_MANUAL  102
#define CM_OPEN_PAT     103
#define CM_REFRESH      104
#define CM_ADD_PATIENT  105

DEFINE_RESPONSE_TABLE1(NSInPersonsWindow, TPaneSplitter)
   EV_WM_SIZE,
END_RESPONSE_TABLE;

NSInPersonsWindow::NSInPersonsWindow(NSContexte* pCtx, TWindow* parent, string sDescFile, const char far* title, TModule* module)
                  :TPaneSplitter(parent, title, 0, module), NSRoot(pCtx)
{
  _pGadgetPanelWindow = (NSInPersonsGadgetWindow*) 0 ;
  _pListWindow        = new NSInPersonsListWindow(pCtx, this, ID_PersonsList, 0, 0, 0, 0) ;

  string sTrace = string("Loading In Persons descriptors from file \"") + sDescFile + string("\"") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  string sInitError   = string("") ;
  _pFileDescriptor    = new NSLocalChapterArray() ;
  _pFileDescriptor->init(sDescFile, &sInitError) ;

  if (string("") != sInitError)
  {
    string sTrace = string("Cannont open InPatients configuration file \"") + sDescFile + string("\" : ") + sInitError ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    erreur(sTrace.c_str(), standardError, 0) ;
    return ;
  }

  string sFileName = _pFileDescriptor->getLocalText(string("global"), string("file")) ;
  _bStaticList = (string("") != sFileName) ;

  string sHl7SiuDirName = _pFileDescriptor->getLocalText(string("global"), string("hl7siuDir")) ;
  _bFromHl7Siu = (string("") != sHl7SiuDirName) ;

  // Check if there is a col in CSV with practitioner information, in case it
  // would be useful to only display the patients for (current user/a given MD)
  //
  NSLocalisedChapter* pChapter = _pFileDescriptor->getChapterWhereText(string("path"), string("DPRAT1")) ;
  _bIncludesPractitioner = ((NSLocalisedChapter*) NULL != pChapter) ;

  initFields() ;
}

NSInPersonsWindow::~NSInPersonsWindow()
{
  delete _pListWindow ;
  delete _pFileDescriptor ;
}

void
NSInPersonsWindow::SetupWindow()
{
  TPaneSplitter::SetupWindow() ;

  // Can't do it there since Parent is not properly defined
  // SetWindowPosit() ;
}

void
NSInPersonsWindow::openPatient(NSInsidePerson* /* pMailObject */)
{
}

/*
    Seed changed (a char was added or removed)
    If the patients list (PL) is static, we have to adapt current selection
    If PL is fetched from the seed, there is nothing to do (before Return key)
*/
void
NSInPersonsWindow::seedChanged(string sSeed)
{
  if (_bStaticList || _bFromHl7Siu)
    _pListWindow->seedChanged(sSeed) ;
}

/*
    User has hit the Return key
    If the patients list (PL) is static, we force its refreshment
    If PL is fetched from the seed, we have to go get a new list
*/
void
NSInPersonsWindow::goGetList(string sSeed)
{
  refreshList() ;
}

void
NSInPersonsWindow::refreshList()
{
  if ((NSInPersonsListWindow*) NULL == _pListWindow)
    return ;

  _pListWindow->refreshList() ;
  _pListWindow->resetTimer() ;
}

void
NSInPersonsWindow::addCurrentPatient()
{
  if ((NSInPersonsListWindow*) NULL == _pListWindow)
    return ;

  _pListWindow->addCurrentPatient() ;
}

// Get the information located at a given col inside the CSV file
//
NSInPersonField*
NSInPersonsWindow::getField(int iNumber)
{
  if (_aFields.empty())
    return (NSInPersonField*) 0 ;

  NSInPersonFieldIter fieldIt = _aFields.begin() ;
  for ( ; _aFields.end() != fieldIt ; fieldIt++)
    if ((*fieldIt)->getNumber() == iNumber)
      return *fieldIt ;

  return (NSInPersonField*) 0 ;
}

// Get the information located at a given col in the view list
//
NSInPersonField*
NSInPersonsWindow::getFieldForOrder(int iOrder)
{
  if (_aFields.empty())
    return (NSInPersonField*) 0 ;

  NSInPersonFieldIter fieldIt = _aFields.begin() ;
  for ( ; _aFields.end() != fieldIt ; fieldIt++)
    if ((*fieldIt)->getOrder() == iOrder)
      return *fieldIt ;

  return (NSInPersonField*) 0 ;
}

NSInPersonField*
NSInPersonsWindow::getFieldForPath(string sPath)
{
  if ((_aFields.empty()) || (string("") == sPath))
    return (NSInPersonField*) 0 ;

  NSInPersonFieldIter fieldIt = _aFields.begin() ;
  for ( ; _aFields.end() != fieldIt ; fieldIt++)
    if ((*fieldIt)->getPath() == sPath)
      return *fieldIt ;

  return (NSInPersonField*) 0 ;
}

NSInPersonField*
NSInPersonsWindow::getFieldForChapter(string sChapter)
{
  if ((_aFields.empty()) || (string("") == sChapter))
    return (NSInPersonField*) 0 ;

  NSInPersonFieldIter fieldIt = _aFields.begin() ;
  for ( ; _aFields.end() != fieldIt ; fieldIt++)
    if ((*fieldIt)->getChapter() == sChapter)
      return *fieldIt ;

  return (NSInPersonField*) 0 ;
}

string
NSInPersonsWindow::getSeed()
{
  if (false == IsWindow())
    return string("") ;

  NSInPersonsRechNomEdit* pNameEdit = getRechNomEdit() ;
  if (pNameEdit && pNameEdit->IsWindow())
    return pNameEdit->GetText() ;

  return string("") ;
}

void
NSInPersonsWindow::resetSeed()
{
  if (false == IsWindow())
    return ;

  NSInPersonsRechNomEdit* pNameEdit = getRechNomEdit() ;
  if (pNameEdit && pNameEdit->IsWindow())
    pNameEdit->SetText("") ;
}

void
NSInPersonsWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TPaneSplitter::EvSize(sizeType, size) ;
  _pListWindow->MoveWindow(GetClientRect(), true) ;
}

void
NSInPersonsWindow::setGadgetWindowLayout()
{
	if ((NSInPersonsGadgetWindow*) NULL == _pGadgetPanelWindow)
		return ;

	int iGadgetsHeight = _pGadgetPanelWindow->getGadgetsHeight() * 2 ;

  TLayoutMetrics layoutMetrics ;
  layoutMetrics.SetMeasurementUnits(lmPixels) ;
  layoutMetrics.X.Absolute(lmLeft, 1) ;
	layoutMetrics.Y.Absolute(lmTop, 1) ;
	layoutMetrics.Width.Set(lmRight, lmLeftOf, lmParent, lmRight, 1) ;
	layoutMetrics.Height.Absolute(iGadgetsHeight) ;

  SetChildLayoutMetrics(*_pGadgetPanelWindow, layoutMetrics) ;
  Layout() ;
}

void
NSInPersonsWindow::initFields()
{
  NSLocalChapterArrayIter itDesc    = _pFileDescriptor->begin() ;
  NSLocalChapterArrayIter itDescEnd = _pFileDescriptor->end() ;

  for ( ; itDescEnd != itDesc ; itDesc++)
  {
    NSLocalisedChapter* pLocalisedChapter = (*itDesc).second ;

    if (pLocalisedChapter)
    {
      NSInPersonField field(pLocalisedChapter) ;

      if (field.isField())
      {
        _aFields.push_back(new NSInPersonField(field)) ;

        if (field.isVisible())
          _aVisibleFields.push_back(new NSInPersonField(field)) ;
      }
    }
  }

  sort(_aVisibleFields.begin(), _aVisibleFields.end(), FieldSortOrderInf) ;
}

void
NSInPersonsWindow::CmManual()
{
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenPatSearchDialog")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSInPersonsWindow::SetFocusForInit()
{
  if ((NSInPersonsGadgetWindow*) NULL == _pGadgetPanelWindow)
    return ;

  NSInPersonsRechNomEdit* pEdit = _pGadgetPanelWindow->getRechNomEdit() ;
  if ((NSInPersonsRechNomEdit*) NULL == pEdit)
    return ;

  pEdit->SetFocus() ;
}

void
NSInPersonsWindow::SetWindowPosit()
{
  NSWindowProperty* pWinProp = pContexte->getUtilisateur()->getWindowProperty(string("InPersonsList")) ;
  if ((NSWindowProperty*) NULL == pWinProp)
    return ;

  Parent->Show(SW_HIDE) ;

  NS_CLASSLIB::TRect targetRect = pWinProp->getRect(pContexte->getSuperviseur()) ;

  //
  // fixer la nouvelle position
  // (on ne tient pas compte de la taille, vu le probleme pour restaurer
  //  une fenetre TView,TWindow mise en icone)
  //
  Parent->SetWindowPos(0, targetRect.Left(), targetRect.Top(), targetRect.Width(), targetRect.Height(), SWP_NOZORDER) ;
  Parent->Show(SW_SHOWNORMAL) ;
}

/****************** classe NSInPersonsChild **************************/
DEFINE_RESPONSE_TABLE1(NSInPersonsChild, TMDIChild)
	 EV_WM_CLOSE,
	 EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSInPersonsChild::NSInPersonsChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, TWindow* clientWnd)                 :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
}

NSInPersonsChild::~NSInPersonsChild()
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;
  if ((NSSuper*) NULL == pSuper)
    return ;

  TMyApp *pApp = pSuper->getApplication() ;
  if ((TMyApp*) NULL == pApp)
    return ;

  NSMDIFrame* pFrame = pApp->getFrame() ;
  if ((NSMDIFrame*) NULL == pFrame)
    return ;

  NSInPersonsChild* pInPersonsChild = pFrame->getInPersonsChild() ;
  if ((NSInPersonsChild*) NULL == pInPersonsChild)
    return ;

  if (this == pInPersonsChild)
    pFrame->setInPersonsChild((NSInPersonsChild*) 0) ;
}

voidNSInPersonsChild::EvClose()
{
	TWindow::EvClose() ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSInPersonsListWindow//
// -----------------------------------------------------------------------------int    __iColToSort ;string __sPathToSort ;boolInPersonSortInf(NSInsidePerson *pObj1, NSInsidePerson *pObj2)
{
  if (((NSInsidePerson*) NULL == pObj1) || ((NSInsidePerson*) NULL == pObj2))
    return false ;

	return (pObj1->getInformationData(__sPathToSort) < pObj2->getInformationData(__sPathToSort)) ;
}boolInPersonSortSup(NSInsidePerson *pObj1, NSInsidePerson *pObj2)
{
  if (((NSInsidePerson*) NULL == pObj1) || ((NSInsidePerson*) NULL == pObj2))
    return false ;

	return (pObj1->getInformationData(__sPathToSort) > pObj2->getInformationData(__sPathToSort)) ;
}boolInPersonSortByNameInf(NSInsidePerson *pObj1, NSInsidePerson *pObj2)
{
  if (((NSInsidePerson*) NULL == pObj1) || ((NSInsidePerson*) NULL == pObj2))
    return false ;

	return (pObj1->getName() < pObj2->getName()) ;
}boolInPersonSortByNameSup(NSInsidePerson *pObj1, NSInsidePerson *pObj2)
{
  if (((NSInsidePerson*) NULL == pObj1) || ((NSInsidePerson*) NULL == pObj2))
    return false ;

	return (pObj1->getName() > pObj2->getName()) ;
}DEFINE_RESPONSE_TABLE1(NSInPersonsListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
  EV_WM_TIMER,
  NS_LVN_GETDISPINFO(DispInfoListe),
  NS_LW_COLUMNCLICK(LVNColumnclick),
	EV_COMMAND(CM_OPEN_PAT, CmOpenPatient),
END_RESPONSE_TABLE ;

NSInPersonsListWindow::NSInPersonsListWindow(NSContexte* pCtx, NSInPersonsWindow *parent, int id, int x, int y, int w, int h, TModule *module)
                      :NSSkinableListWindow((TWindow *) parent, pCtx, id, x, y, w, h, module)
{
  _pParentWindow = parent ;
  _iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	_iPreviousSelection = -1 ;
	_sDirectory         = pContexte->PathName("FGLO") + string("inbox\\") ;

  _sCsvFileFullName   = string("") ;
  _bCsvHasHeader      = false ;
  _cCvsSeparator      = ';' ;

  _sOdbcConnectorName = string("") ;
  _sOdbcRequest       = string("") ;

  _sHl7SiuDir         = string("") ;
  _bExploreSubDir     = false ;

  _iRefreshInterval   = -1 ;
  _sUserFilter        = string("") ;
  _bRemoveWhenOpening = false ;
  _bCanAddToList      = false ;
  _bCloseAfterAdding  = false ;
  _sIcons             = string("") ;
}

NSInPersonsListWindow::~NSInPersonsListWindow()
{
}

void
NSInPersonsListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOn("InPatientsListOn") ;

  SetupColumns() ;

  loadInPersons() ;
  initNameField() ;

  fillSelectedArray(string("")) ;
  sortByName() ;

  AfficheListe() ;
}

void
NSInPersonsListWindow::loadInPersons()
{
/*
try
{
  _pProgressDlg = new NSProgressDlg(pContexte->GetMainWindow(), pContexte) ;
  _pProgressDlg->Create() ;
  _pProgressDlg->ShowWindow(SW_SHOW) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSInPersonsListWindow::loadMessages : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSInPersonsListWindow::loadMessages.", standardError, 0) ;
  return ;
}
*/

  // First, get files from buffer directory
  //
  string sTrace = string("In-patients: loading patients") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // getNewMessagesFromBufferDirectory(string("txt")) ;

  initPersonsList() ;

  sTrace = string("In-patients loaded") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // pContexte->getSuperviseur()->traceMemory() ;

/*
  _pProgressDlg->Destroy() ;
  delete _pProgressDlg ;
  _pProgressDlg = (NSProgressDlg*) 0 ;
*/
}

void
NSInPersonsListWindow::processSelectionChange()
{
	// Don't do that, because if an entry is removed, iCurrentSelection is -1
	int iCurrentSelection = IndexItemSelect() ;
	if (iCurrentSelection < 0)
		return ;

	_iPreviousSelection = iCurrentSelection ;
}

void
NSInPersonsListWindow::selectCurrentPatientInList()
{
	if (_aInPersonsSelected.empty())
    return ;

  string sTrace = string("NSInPersonsListWindow::selectCurrentPatientInList : Entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	sort(_aInPersonsSelected.begin(), _aInPersonsSelected.end(), InPersonSortByNameInf) ;

	_bNaturallySorted = true ;
	_iSortedColumn    = 0 ;

  sTrace = string("NSInPersonsListWindow::selectCurrentPatientInList : filling messages list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  AfficheListe() ;

  sTrace = string("NSInPersonsListWindow::selectCurrentPatientInList : messages list filled") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
  {
    sTrace = string("NSInPersonsListWindow::selectCurrentPatientInList : exit (no patient)") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	return ;
  }

	string sName = pseumaj(pPatEnCours->getNom()) + string(" ") +
                                           pseumaj(pPatEnCours->getPrenom()) ;

  size_t iIndexNumber = 0 ;
  NSInPersonIter itInPerson = _aInPersonsSelected.begin() ;

  while ((_aInPersonsSelected.end() != itInPerson) &&
                             (pseumaj((*itInPerson)->getName()) < sName))
  {
    iIndexNumber++ ;
    itInPerson++ ;
  }

  SetItemState(iIndexNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
  EnsureVisible(iIndexNumber, false) ;

  processSelectionChange() ;

  sTrace = string("NSInPersonsListWindow::selectCurrentPatientInList : exit") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}

void
NSInPersonsListWindow::SetupColumns()
{
  NSInPersonFieldArray* pVisibleFields = _pParentWindow->getVisibleFields() ;

  if (((NSInPersonFieldArray*) NULL == pVisibleFields) || pVisibleFields->empty())
    return ;

  int iCol = 0 ;

  NSInPersonFieldIter fIt = pVisibleFields->begin() ;
  for ( ; pVisibleFields->end() != fIt ; fIt++)
  {
    string sTitle = (*fIt)->getTitle() ;
    InsertColumn(iCol, TListWindColumn((char far*) sTitle.c_str(), (*fIt)->getWidth(), TListWindColumn::Left, iCol)) ;
    iCol++ ;
  }
}

// Return the path for a displayed col
//
// iCol = col (left = 0)
//
string
NSInPersonsListWindow::getPathForCol(int iCol)
{
  NSInPersonField* pPersonField = _pParentWindow->getFieldForOrder(iCol + 1) ;
  if ((NSInPersonField*) NULL == pPersonField)
    return string("") ;

  return pPersonField->getPath() ;
}

// Fill the sub-array of persons that fit the search seed, according to a col
//
// sSeed        : search seed
// iRecordIndex : sorted col
//
void
NSInPersonsListWindow::fillSelectedArray(string sSeed, size_t iRecordIndex)
{
  // Fast processing when seed if blank, selected = full
  //
  if (string("") == sSeed)
  {
    _aInPersonsSelected = _aInPersonsFull ;
    return ;
  }

  _aInPersonsSelected.vider() ;

  if (_aInPersonsFull.empty())
    return ;

  // Sort according to the col
  //
  sortByFull(iRecordIndex) ;

  string sPathForCol = getPathForCol(iRecordIndex) ;

  string sUpperSeed = pseumaj(sSeed) ;
  size_t iSeedLen   = strlen(sUpperSeed.c_str()) ;

  NSInPersonIter itInPerson = _aInPersonsFull.begin() ;
  string sField = pseumaj((*itInPerson)->getInformationDisplayData(sPathForCol)) ;

  // First, discard all persons with a too small attribute
  //
  while ((sField < sUpperSeed) && (_aInPersonsFull.end() != itInPerson))
  {
    itInPerson++ ;
    if (_aInPersonsFull.end() != itInPerson)
      sField = pseumaj((*itInPerson)->getInformationDisplayData(sPathForCol)) ;
  }

  if (_aInPersonsFull.end() == itInPerson)
    return ;

  // Then include all persons with a proper attribute
  //
  while ((strlen(sField.c_str()) >= iSeedLen) &&
                                (string(sField, 0, iSeedLen) == sUpperSeed))
  {
    _aInPersonsSelected.push_back(new NSInsidePerson(**itInPerson)) ;

    itInPerson++ ;

    if (_aInPersonsFull.end() == itInPerson)
    return ;

    sField = pseumaj((*itInPerson)->getInformationDisplayData(sPathForCol)) ;
	}
}

// Populate the list
//
void
NSInPersonsListWindow::AfficheListe()
{
  string sTrace = string("NSInPersonsListWindow::AfficheListe: Entering.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	DeleteAllItems() ;

  if (_aInPersonsSelected.empty())
    return ;

  string sPathForLeftCol = getPathForCol(0) ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSInPersonReverseIter itInPerson = _aInPersonsSelected.rbegin() ;
  do
  {
    string sFirstInfo = (*itInPerson)->getInformationDisplayData(sPathForLeftCol) ;
    TListWindItem Item(sFirstInfo.c_str(), 0) ;

    InsertItem(Item) ;

    itInPerson++ ;
	}
  while (_aInPersonsSelected.rend() != itInPerson) ;

  sTrace = string("NSInPersonsListWindow::AfficheListe: Exiting.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSInPersonsListWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  TListWindItem &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;
  int iCol  = dispInfoItem.GetSubItem() ;

  string sPath = getPathForCol(iCol) ;

  NSInsidePerson* pInPerson = _aInPersonsSelected[index] ;
  if ((NSInsidePerson*) NULL == pInPerson)
    return ;

  static char szBuffer[1024] ;

  string sInformation = pInPerson->getInformationDisplayData(sPath) ;
  strcpy(szBuffer, sInformation.c_str()) ;

  dispInfoItem.SetText(szBuffer) ;
}

void
NSInPersonsListWindow::initPersonsList()
{
  if      (_pParentWindow->isStaticList())
    initPersonsListFromCsv() ;
  else if (_pParentWindow->isFromHl7Siu())
    initPersonsListFromHl7Siu() ;
  else
    initPersonsListFromOdbc() ;
}

// Fill patients list from CSV file
//
void
NSInPersonsListWindow::initPersonsListFromCsv()
{
try
{
  string sTrace = string("NSInPersonsListWindow::initPersonsListFromCsv entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // Process CSV file description (file name, header, separator)
  //
  NSLocalChapterArray* pDescriptor = _pParentWindow->getFileDescriptor() ;
  if ((NSLocalChapterArray*) NULL == pDescriptor)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromCsv file descriptor not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  _sCsvFileFullName = pDescriptor->getLocalText(string("global"), string("file")) ;
  if (string("") == _sCsvFileFullName)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromCsv name of csv file to process not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  string sHasHeader = pDescriptor->getLocalText(string("global"), string("hasHeader")) ;
  if (string("") != sHasHeader)
  {
    if (string("TRUE") == pseumaj(sHasHeader))
      _bCsvHasHeader = true ;
    else
      _bCsvHasHeader = false ;
  }

  string sSeparator = pDescriptor->getLocalText(string("global"), string("separator")) ;
  if (string("") != sSeparator)
    _cCvsSeparator = sSeparator[0] ;

  string sInterval = pDescriptor->getLocalText(string("global"), string("refreshDelay")) ;
  if (string("") != sInterval)
  {
    _iRefreshInterval = StringToInt(sInterval) ;
    SetTimer(ID_WHOISTHERE_TIMER, _iRefreshInterval * 1000) ;
  }

  _sUserFilter        = pDescriptor->getLocalText(string("global"), string("userFilter")) ;
  _bRemoveWhenOpening = pDescriptor->isYes(string("global"), string("removeWhenOpening")) ;
  _bCanAddToList      = pDescriptor->isYes(string("global"), string("canAddToList")) ;
  _bCloseAfterAdding  = pDescriptor->isYes(string("global"), string("closeAfterAdding")) ;
  _sIcons             = pDescriptor->getLocalText(string("global"), string("icons")) ;

  if (string("") != _sUserFilter)
  {
    sTrace = string("User filter: ") + _sUserFilter ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  }
  sTrace = string("Parameters: ") ;
  if (_bRemoveWhenOpening)
    sTrace += string("remove when opening") ;
  else
    sTrace += string("don't remove when opening") ;
  if (_bCanAddToList)
    sTrace += string(", can add to list") ;
  else
    sTrace += string(", cannot add to list") ;
  if (_bCloseAfterAdding)
    sTrace += string(", close after adding") ;
  else
    sTrace += string(", don't close after adding") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  refreshFromCsv() ;

  sTrace = string("NSInPersonsListWindow::initPersonsListFromCsv leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSInPersonsListWindow::initPersonsListFromCsv.", standardError, 0) ;
  return ;
}
}

// Fill patients list from ODBC connector
//
void
NSInPersonsListWindow::initPersonsListFromOdbc()
{
try
{
  string sTrace = string("NSInPersonsListWindow::initPersonsListFromOdbc entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // Process CSV file description (file name, header, separator)
  //
  NSLocalChapterArray* pDescriptor = _pParentWindow->getFileDescriptor() ;
  if ((NSLocalChapterArray*) NULL == pDescriptor)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromOdbc file descriptor not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  _sOdbcConnectorName = pDescriptor->getLocalText(string("global"), string("odbcConnector")) ;
  if (string("") == _sOdbcConnectorName)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromOdbc name of Odbc connector not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  _sOdbcRequest = pDescriptor->getLocalText(string("global"), string("odbcRequest")) ;
  if (string("") == _sOdbcRequest)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromOdbc Odbc request not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  string sInterval = pDescriptor->getLocalText(string("global"), string("refreshDelay")) ;
  if (string("") != sInterval)
  {
    _iRefreshInterval = StringToInt(sInterval) ;
    SetTimer(ID_WHOISTHERE_TIMER, _iRefreshInterval * 1000) ;
  }

  refreshFromOdbc() ;

  sTrace = string("NSInPersonsListWindow::initPersonsListFromOdbc leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSInPersonsListWindow::initPersonsListFromOdbc.", standardError, 0) ;
  return ;
}
}

// Fill patients list from HL7 SIU RDV
//
void
NSInPersonsListWindow::initPersonsListFromHl7Siu()
{
try
{
  string sTrace = string("NSInPersonsListWindow::initPersonsListFromHl7Siu entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // Process CSV file description (file name, header, separator)
  //
  NSLocalChapterArray* pDescriptor = _pParentWindow->getFileDescriptor() ;
  if ((NSLocalChapterArray*) NULL == pDescriptor)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromHl7Siu file descriptor not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  // Get the directory where files are stored
  //
  _sHl7SiuDir = pDescriptor->getLocalText(string("global"), string("hl7siuDir")) ;
  if (string("") == _sHl7SiuDir)
  {
    sTrace = string("NSInPersonsListWindow::initPersonsListFromHl7Siu messages directory not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  string sExploreSubDir = pDescriptor->getLocalText(string("global"), string("exploreSubDir")) ;
  if (string("") != sExploreSubDir)
  {
    if (string("TRUE") == pseumaj(sExploreSubDir))
      _bExploreSubDir = true ;
    else
      _bExploreSubDir = false ;
  }

  string sInterval = pDescriptor->getLocalText(string("global"), string("refreshDelay")) ;
  if (string("") != sInterval)
  {
    _iRefreshInterval = StringToInt(sInterval) ;
    SetTimer(ID_WHOISTHERE_TIMER, _iRefreshInterval * 1000) ;
  }

  refreshFromHl7Siu() ;

  sTrace = string("NSInPersonsListWindow::initPersonsListFromHl7Siu leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSInPersonsListWindow::initPersonsListFromHl7Siu.", standardError, 0) ;
  return ;
}
}

void
NSInPersonsListWindow::resetTimer()
{
  KillTimer(ID_WHOISTHERE_TIMER) ;
  SetTimer(ID_WHOISTHERE_TIMER, _iRefreshInterval * 1000) ;
}

void
NSInPersonsListWindow::refreshFromCsv()
{
  if (string("") == _sCsvFileFullName)
    return ;

  // No need to change cursor since it is usually a background process
  // TODO: change cursor only if focused window
  //
  // HCURSOR hPrevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;

  NSCsvParser cvsParser(pContexte->getSuperviseur()) ;

  cvsParser.setHasHeader(_bCsvHasHeader) ;
  cvsParser.setSeparator(_cCvsSeparator) ;

  // Parse CSV file
  //
  if (false == cvsParser.parse(_sCsvFileFullName, true, true))
    return ;

  // Use "records" from CSV file to fill In Patients list
  //
  NSCsvRecordArray* pRecords = cvsParser.getRecords() ;
  if (((NSCsvRecordArray*) NULL == pRecords) || pRecords->empty())
    return ;

  _aInPersonsFull.vider() ;

  NSCsvRecordIter CsvRecIt = pRecords->begin() ;
  for ( ; pRecords->end() != CsvRecIt ; CsvRecIt++)
  {
    addPatientFromCsvEntry(*CsvRecIt) ;

    if (pContexte->getSuperviseur()->getApplication())
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
  }

  // ::SetCursor(hPrevCursor) ;
}

void
NSInPersonsListWindow::refreshFromOdbc()
{
  _aInPersonsFull.vider() ;

  if ((string("") == _sOdbcConnectorName) || (string("") == _sOdbcRequest))
    return ;

  string sSeed = _pParentWindow->getSeed() ;
  if (string("") == sSeed)
    return ;

  // The request is updated from seed
  //
  string sRequest = _sOdbcRequest ;

  size_t posit = sRequest.find("[$Input$]") ;
  if (NPOS != posit)
    sRequest.replace(posit, 9, sSeed) ;

  // No need to change cursor since it is usually a background process
  // TODO: change cursor only if focused window
  //
  // HCURSOR hPrevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;

  NSOdbcConnector odbcConnector(pContexte->getSuperviseur(), _sOdbcConnectorName) ;

  std::vector<ResultLine> aResults ;
  int iRet = odbcConnector.ExecuteRequest(sRequest, &aResults) ;

  if ((0 != iRet) || aResults.empty())
    return ;

  // Fill a NSCsvRecordArray with the content of results
  //
  NSCsvRecordArray aRecords ;

  vector<ResultLine>::iterator resultIt = aResults.begin() ;
  for ( ; aResults.end() != resultIt ; resultIt++)
  {
    if (false == resultIt->aColumns.empty())
    {
      NSCsvRecord* pRecord = new NSCsvRecord() ;

      vector<string>::iterator colIt = resultIt->aColumns.begin() ;
      for (int i = 0 ; resultIt->aColumns.end() != colIt ; colIt++, i++)
        pRecord->AddField(*colIt, i) ;

      aRecords.push_back(pRecord) ;
    }
  }

  // Use "records" from CSV file to fill In Patients list
  //
  if (aRecords.empty())
    return ;

  NSCsvRecordIter CsvRecIt = aRecords.begin() ;
  for ( ; aRecords.end() != CsvRecIt ; CsvRecIt++)
  {
    addPatientFromCsvEntry(*CsvRecIt) ;

    if (pContexte->getSuperviseur()->getApplication())
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
  }

  _pParentWindow->resetSeed() ;

  // ::SetCursor(hPrevCursor) ;
}

void
NSInPersonsListWindow::refreshFromHl7Siu()
{
  string sTrace = string("NSInPersonsListWindow::refreshFromHl7Siu: entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  _aInPersonsFull.vider() ;

  if (string("") == _sHl7SiuDir)
    return ;

  // Get useful HL7 segments
  //
  VectString aHl7Segments ;

  NSInPersonFieldArray* pFields = _pParentWindow->getFields() ;
  if (((NSInPersonFieldArray*) NULL != pFields) && (false == pFields->empty()))
  {
    for (NSInPersonFieldIter recIt = pFields->begin() ; pFields->end() != recIt ; recIt++)
    {
      string sSegmName = string("") ;
      string sSegmLoca = string("") ;
      splitString((*recIt)->getChapter(), &sSegmName, &sSegmLoca, '.') ;

      if ((string("") != sSegmName) && (false == aHl7Segments.contains(sSegmName)))
        aHl7Segments.add(sSegmName) ;
    }
  }

  if (aHl7Segments.empty())
    return ;

  vector<string> directoriesToParse ;
  directoriesToParse.push_back(_sHl7SiuDir) ;

  // No need to change cursor since it is usually a background process
  // TODO: change cursor only if focused window
  //
  // HCURSOR hPrevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;

  vector<string>::iterator fileIter = directoriesToParse.begin() ;
  while (directoriesToParse.end() != fileIter)
  {
    string sParsedDir = *fileIter ;

    sTrace = string("NSInPersonsListWindow refresh: scanning files in ") + sParsedDir ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

    // Preparing complete file name
    //
    string sCompleteFileName = sParsedDir + string("*.*") ;

    size_t iPathLength = strlen(sCompleteFileName.c_str()) + 1 ;
    char* szFichRecherche = new char[iPathLength] ;
    memset(szFichRecherche, 0, sizeof(iPathLength)) ;

    strcpy(szFichRecherche, sCompleteFileName.c_str()) ;

    size_t iCurrentFileNb = 0 ;

    // Accessing first file in directory
    //
    WIN32_FIND_DATA stSearch ;
    HANDLE			    hFoundFile = FindFirstFile(szFichRecherche, &stSearch) ;
    delete[] szFichRecherche ;

    // If first file cannot be accessed, then leave
    //
    if (INVALID_HANDLE_VALUE == hFoundFile)
    {      sTrace = string("NSInPersonsListWindow refresh: can't access first file, leaving.") ;      pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;      return ;
    }

    // The array of records that will receive captured information
    //
    NSCsvRecordArray aRecords ;

    // Loop while files are found
    //
    bool bFound = true ;
    while (bFound)
    {
      string sFileName = string(stSearch.cFileName) ;

      // Is it a directory?
      //
      DWORD dwFAttr = stSearch.dwFileAttributes ;
      if (dwFAttr & FILE_ATTRIBUTE_DIRECTORY)
      {
        if (_bExploreSubDir)
        {
          if ((string(".") != sFileName) && (string("..") != sFileName))
          {
            string sNewDir = sParsedDir + sFileName + FILESLASH ;
            directoriesToParse.push_back(sNewDir) ;
          }
        }
      }
      // It is a file
      //
      else
      {
        iCurrentFileNb++ ;

        if ((string(".") != sFileName) && (string("..") != sFileName))
          addPatientFromHl7SiuFile(sParsedDir + sFileName, &aRecords, &aHl7Segments) ;

        if (pContexte->getSuperviseur()->getApplication())
          pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
      }

      bFound = FindNextFile(hFoundFile, &stSearch) ;
    }

    // Closing files enumeration structure
    //
    FindClose(hFoundFile) ;

    fileIter++ ;
  }

  sTrace = string("NSInPersonsListWindow refresh: leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}

// Instantiate a new patient from a CSV file record
//
void
NSInPersonsListWindow::addPatientFromCsvEntry(NSCsvRecord* pCsvRecord)
{
  if (((NSCsvRecord*) NULL == pCsvRecord) || pCsvRecord->isEmpty())
    return ;

  NSCsvFieldArray* pFields = pCsvRecord->getFields() ;
  if (((NSCsvFieldArray*) NULL == pFields) || pFields->empty())
    return ;

  NSInsidePerson *pNewPerson = new NSInsidePerson() ;

  NSCsvFieldIter recIt = pFields->begin() ;
  for ( ; pFields->end() != recIt ; recIt++)
  {
    // A NSCsvField contains the col number inside CSV file and the string value
    // We must find the col descriptor prior to processing the value
    //
    int iNumCol = (*recIt)->getCol() ;
    NSInPersonField* pField = _pParentWindow->getField(iNumCol + 1) ;

    // If not found, it just means this col is to be ignored
    //
    if (pField)
    {
      string sValue   = (*recIt)->getValue() ;
      string sDisplay = (*recIt)->getValue() ;

      // If there is a format, apply it
      //
      if (string("") != pField->getFormat())
      {
        NSCaptureFormatArray::format(&sValue, pField->getFormat()) ;
        if (string("") != pField->getDisplay())
        {
          NSCaptureFormatArray::formatForDisplay(&sDisplay, pField->getFormat(), pField->getDisplay()) ;
          if (string("") == sDisplay)
            sDisplay = (*recIt)->getValue() ;
        }
      }
      // If it displays the user, change ID for name
      //
      else if (pField->getPath() == string("DPRAT1"))
      {
        NSPersonInfo* pUtil = pContexte->getPersonArray()->getPerson(pContexte, sValue, pidsUtilisat) ;
        if (pUtil)
          sDisplay = pUtil->getNom() + string(" ") + pUtil->getPrenom() ;
      }

      NSInsidePersonInformation info(iNumCol, (*recIt)->getValue(), pField->getPath(), sValue, sDisplay, pField->getUnit()) ;
      pNewPerson->addInformation(&info) ;
    }
  }

  // In case there is a user filter (and a practitioner information in CSV)
  //
  if ((string("") != _sUserFilter) && _pParentWindow->hasPractitioner())
  {
    string sPractitioner = pNewPerson->getInformationRawData(string("DPRAT1")) ;
    if (string("") == sPractitioner)
    {
      delete pNewPerson ;
      return ;
    }

    // Filter can be set on current user (HUTIL1) of on a given user
    //
    string sFilter = _sUserFilter ;
    if (string("HUTIL1") == _sUserFilter)
      sFilter = pContexte->getUtilisateurID() ;

    if (sFilter != sPractitioner)
    {
      delete pNewPerson ;
      return ;
    }
  }

  _aInPersonsFull.push_back(pNewPerson) ;
}

// Instantiate a new patient from a CSV file record
//
void
NSInPersonsListWindow::addPatientFromHl7SiuFile(const string sFileName, NSCsvRecordArray* pRecords, const VectString* paHl7Segments)
{
  if ((string("") == sFileName) || ((NSCsvRecordArray*) NULL == pRecords) ||
      ((VectString*) NULL == paHl7Segments) || paHl7Segments->empty())
    return ;

  // Getting file content
  //
  string sContent = getFileContent(pContexte->getSuperviseur(), sFileName) ;

  if (string("") == sContent)
    return ;

  size_t iContentLen = strlen(sContent.c_str()) ;

  size_t iNext = 0 ;
  string sLine = getLine(&iNext, &sContent) ;

  // First line must be the MSH (Message Control Header)
  //
  if ((strlen(sLine.c_str()) < 4) || (string("MSH") != string(sLine, 0, 3)))
  {
    string sErrorText = string("Invalid file ") + sFileName + string(" first line is not an MSH segment") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		return ;
  }

  // HL7 Documentation:
  //
  // Field separator (ST) 00001: This field contains the separator between the
  // segment ID and the first real field, MSH-2-encoding characters.
  // As such it serves as the separator and defines the character to be used as
  // a separator for the rest of the message. Recommended value is | (ASCII 124)
  //
  char cSeparator = sLine[3] ;

  // Split segment + content
  //
  string sSegmentName    = string("") ;
  string sSegmentContent = string("") ;
  splitString(sLine, &sSegmentName, &sSegmentContent, cSeparator) ;

  // Split content
  //
  ClasseStringVector aContent ;
  DecomposeChaine(&sSegmentContent, &aContent, string(1, cSeparator)) ;

  if (aContent.empty())
  {
    string sErrorText = string("Invalid file ") + sFileName + string(" MSH segment is empty") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		return ;
  }

  // HL7 Documentation:
  //
  // Encoding characters (ST) 00002: This field contains the four characters in
  // the following order: the component separator, repetition separator,
  // escape character, and subcomponent separator.
  // Recommended values are ^~\&, (ASCII 94, 126, 92, and 38, respectively)
  //
  string sSeparators = aContent[0]->getItem() ;

  if (strlen(sSeparators.c_str()) != 4)
  {
    string sErrorText = string("Invalid file ") + sFileName + string(" Not 4 encoding characters") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		return ;
  }

  // Create a NSInsidePerson to host this patient's information
  //
  NSInsidePerson *pNewPerson = new NSInsidePerson() ;

  // Processing other segments
  //
  while (iNext < iContentLen)
  {
    // Getting new line
    //
    sLine = getLine(&iNext, &sContent) ;

    // Split line into segment name and content
    //
    splitString(sLine, &sSegmentName, &sSegmentContent, cSeparator) ;

    // Checking if this segment contains information
    //
    if (paHl7Segments->contains(sSegmentName))
      getPatientInfoFromHl7SiuSegment(sSegmentName, sSegmentContent, pNewPerson, cSeparator, sSeparators) ;
  }

  if (pNewPerson->isEmpty())
    delete pNewPerson ;
  else
    _aInPersonsFull.push_back(pNewPerson) ;
}

// Get the line that starts at the iStart-th char and
//
string
NSInPersonsListWindow::getLine(size_t* piStart, const string* psContent)
{
  if (((size_t*) NULL == piStart) || ((string*) NULL == psContent))
    return string("") ;

  size_t iContentLen = strlen(psContent->c_str()) ;
  if (*piStart >= iContentLen)
    return string("") ;

  string sResult = string("") ;

  size_t iNext = *piStart ;
  char   cNext = (*psContent)[iNext] ;

  // Fill the line while no CR or LF encountered
  //
  while ((iNext < iContentLen) && ('\n' != cNext) && ('\r' != cNext))
  {
    sResult += string(1, cNext) ;

    iNext++ ;
    if (iNext < iContentLen)
      cNext = (*psContent)[iNext] ;
  }

  // Skip all CR or LF at end of line
  //
  while ((iNext < iContentLen) && (('\n' == cNext) || ('\r' == cNext)))
  {
    iNext++ ;
    if (iNext < iContentLen)
      cNext = (*psContent)[iNext] ;
  }

  *piStart = iNext ;

  return sResult ;
}

// Instantiate a new patient from a CSV file record
//
void
NSInPersonsListWindow::getPatientInfoFromHl7SiuSegment(const string sSegmentName, const string &sSegmentContent, NSInsidePerson *pPerson, const char cSeparator, const string sSeparators)
{
  if ((NSInsidePerson*) NULL == pPerson)
    return ;

  // First split segment content in a vector
  //
  ClasseStringVector aSegments ;
  DecomposeChaine(&sSegmentContent, &aSegments, string(1, cSeparator)) ;

  if (aSegments.empty())
    return ;

  // Process each segment and the sub-segments it contains
  //
  int iSegmId    = 1 ;
  int iSegmCount = aSegments.size() ;

  // Since we will be recursively adding elements to segments array, we
  // must stop we have processed the initial count
  //
  for (iterClassStringConst segIt = aSegments.begin() ;
               (iSegmId <= iSegmCount) && (aSegments.end() != segIt) ; segIt++)
  {
    // Set segment qualifier
    //
    string sCurrentSegmentName = sSegmentName + string(".") + IntToString(iSegmId) ;
    iSegmId++ ;
    (*segIt)->setQualifier(sCurrentSegmentName) ;

    // Process the segment to split the sub information it contains
    //
    string sCurrentContent = (*segIt)->getItem() ;
    if (string("") != sCurrentContent)
      processHl7SiuSegment(sCurrentSegmentName, sCurrentContent, &aSegments, sSeparators) ;
  }

  NSLocalChapterArray* pDescriptor = _pParentWindow->getFileDescriptor() ;
  if ((NSLocalChapterArray*) NULL == pDescriptor)
  {
    string sTrace = string("NSInPersonsListWindow::getPatientInfoFromHl7SiuSegment file descriptor not found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  // Now get information from the segment tank
  //
  for (iterClassStringConst segIt = aSegments.begin() ; aSegments.end() != segIt ; segIt++)
  {
    string sValue   = (*segIt)->getItem() ;
    string sDisplay = (*segIt)->getItem() ;

    if (string("") != sValue)
    {
      // Get the descriptor for this segment
      //
      NSInPersonField* pField = _pParentWindow->getFieldForChapter((*segIt)->getQualifier()) ;
      if (pField)
      {
        if (string("") != pField->getFormat())
        {
          NSCaptureFormatArray::format(&sValue, pField->getFormat()) ;

          if (string("") != pField->getDisplay())
          {
            NSCaptureFormatArray::formatForDisplay(&sDisplay, pField->getFormat(), pField->getDisplay()) ;
            if (string("") == sDisplay)
              sDisplay = (*segIt)->getItem() ;
          }
        }

        NSInsidePersonInformation info(pField->getNumber(), (*segIt)->getItem(), pField->getPath(), sValue, sDisplay, pField->getUnit()) ;
        pPerson->addInformation(&info) ;
      }
    }
  }


  // NSInPersonFieldArray* getFields()
}

void
NSInPersonsListWindow::processHl7SiuSegment(const string sSegmentName, const string &sSegmentContent, ClasseStringVector* pSegmentsTank, const string sSeparators)
{
  if (((ClasseStringVector*) NULL == pSegmentsTank) ||
      (string("") == sSeparators) || (string("") == sSegmentContent))
    return ;

  // Get separator and unstack separators list
  //
  char cCurrentSeparator = sSeparators[0] ;

  size_t iSeparatorLen = strlen(sSeparators.c_str()) ;
  string sNewSeparator = string("") ;
  if (iSeparatorLen > 1)
    sNewSeparator = string(sSeparators, 1, iSeparatorLen - 1) ;

  // First split segment content in a vector
  //
  ClasseStringVector aSegments ;
  DecomposeChaine(&sSegmentContent, &aSegments, string(1, cCurrentSeparator)) ;

  // If there is a single segment, we have to process it anyway since it
  // may contain sub-segments
  //
  if (aSegments.empty())
    return ;

  // Process each segment and the sub-segments it contains
  //
  int iSegmId = 1 ;

  for (iterClassStringConst segIt = aSegments.begin() ; aSegments.end() != segIt ; segIt++)
  {
    // Set segment qualifier
    //
    string sCurrentSegmentName = sSegmentName + string(".") + IntToString(iSegmId) ;
    iSegmId++ ;
    (*segIt)->setQualifier(sCurrentSegmentName) ;

    string sCurrentContent = (*segIt)->getItem() ;

    if (string("") != sCurrentContent)
    {
      // Add information to tank (if not already known)
      //
      if (sCurrentContent != sSegmentContent)
        pSegmentsTank->push_back(new classString(**segIt)) ;

      // Process the segment to split the sub information it contains
      // (even if already known since it can contain sub-level separators)
      //
      if (string("") != sSeparators)
        processHl7SiuSegment(sCurrentSegmentName, sCurrentContent, pSegmentsTank, sNewSeparator) ;
    }
  }
}

void
NSInPersonsListWindow::EvTimer(uint timerId)
{
	if (ID_WHOISTHERE_TIMER != timerId)
		return ;

  refreshList() ;
}

void
NSInPersonsListWindow::refreshList()
{
  // We want to do our best to keep previously selected patient selected again
  //
  bool bWasAPersonSelected = false ;
  NSInsidePerson SelectedPerson ;

  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection >= 0)
  {
    bWasAPersonSelected = true ;
	  SelectedPerson = *(_aInPersonsSelected[iCurrentSelection]) ;
  }

  // First, refresh the full list
  //
  if      (_pParentWindow->isStaticList())
    refreshFromCsv() ;
  else if (_pParentWindow->isFromHl7Siu())
    refreshFromHl7Siu() ;
  else
    refreshFromOdbc() ;

  // Then fill the selected list according to user entry
  //
  string sSeed = string("") ;
  NSInPersonsRechNomEdit* pNameEdit = _pParentWindow->getRechNomEdit() ;
  if (pNameEdit)
    sSeed = pNameEdit->GetText() ;

  fillSelectedArray(sSeed, getCutField(sSeed)) ;

  // Finally sort according to sorted column and display list
  //
  sortBySelected(_iSortedColumn) ;

  AfficheListe() ;

  // If any, select again the previously selected patient
  //
  if (bWasAPersonSelected)
  {
    int iToSelect = getNewSelectedIndex(&SelectedPerson) ;
    if (iToSelect >= 0)
    {
      SetItemState(iToSelect, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
      EnsureVisible(iToSelect, true) ;
    }
  }
}

void
NSInPersonsListWindow::seedChanged(string sSeed)
{
  bool bWasAPersonSelected = false ;
  NSInsidePerson SelectedPerson ;

  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection >= 0)
  {
    bWasAPersonSelected = true ;
	  SelectedPerson = *(_aInPersonsSelected[iCurrentSelection]) ;
  }

  fillSelectedArray(sSeed, getCutField(sSeed)) ;

  // Finally sort according to sorted column and display list
  //
  sortBySelected(_iSortedColumn) ;

  AfficheListe() ;

  if (bWasAPersonSelected)
  {
    int iToSelect = getNewSelectedIndex(&SelectedPerson) ;
    if (iToSelect >= 0)
    {
      SetItemState(iToSelect, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
      EnsureVisible(iToSelect, true) ;
    }
  }
}

//
// This method is called when in-patients list has been refreshed
//
// If the "name" edit field is not empty, the patient fitting this information
// is selected. If edit field is empty, the selected patient becomes the
// one who is closest from previously selected patient
//
int
NSInPersonsListWindow::getNewSelectedIndex(NSInsidePerson* pPreviouslySelected)
{
  if ((NSInsidePerson*) NULL == pPreviouslySelected)
    return -1 ;

  if (_aInPersonsSelected.empty())
    return -1 ;

  // Rule : if name not empty, search by name,
  //
  NSInPersonsRechNomEdit* pNameEdit = _pParentWindow->getRechNomEdit() ;
  if (pNameEdit)
  {
    string sText = pNameEdit->GetText() ;
    int iListCol = _pNameField->getOrder() - 1 ;

    if ((string("") != sText) && (_iSortedColumn == iListCol))
    {
      SelectByName(sText) ;
      return -1 ;
    }
  }
  //
  // If no name entered in edit control find closest depending on sorted column
  //

  // First, try to find exactly the same one
  //
  int iIndex = 0 ;
  NSInPersonIter itInPerson = _aInPersonsSelected.begin() ;
  for ( ; (_aInPersonsSelected.end() != itInPerson) &&
              (**itInPerson != *pPreviouslySelected) ; itInPerson++, iIndex++) ;

  if (_aInPersonsSelected.end() != itInPerson)
    return iIndex ;

  return -1 ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSInPersonsListWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  processSelectionChange() ;

  //if (info.GetFlags() & LVHT_ONITEM)  //	pView->CmModifier() ;
  //else
  //	pView->CmNouveau() ;
}

void
NSInPersonsListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    processSelectionChange() ;    // NSInsidePerson* pSelectedPerson = _aInPersons[indexItem] ;    // création d'un menu popup		NS_CLASSLIB::TPoint lp = point ;

		OWL::TPopupMenu menu ;

		NSSuper* pSuper = pContexte->getSuperviseur() ;

		string sOpen = pSuper->getText("mailBox", "openPatient") ;

    menu.AppendMenu(MF_STRING, CM_OPEN_PAT, sOpen.c_str()) ;

		ClientToScreen(lp) ;
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
		TListWindow::EvRButtonDown(modkeys, point) ;
  }
  else
		TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSInPersonsListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modkeys, point) ;

	processSelectionChange() ;}
void
NSInPersonsListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
/*
	if      (key == VK_DELETE)
  	pView->CmSupprimer() ;
  else if (key == VK_INSERT)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	pView->CmModifier() ;
    else
    	pView->CmNouveau() ;
  }
  else if (key == VK_TAB)
  {
  	if (GetKeyState(VK_SHIFT) < 0)
    	pView->setFocusToPrevSplitterView() ;
    else
    	pView->setFocusToNextSplitterView() ;
  }
  else
*/
  	TListWindow::EvKeyDown(key, repeatCount, flags) ;

	processSelectionChange() ;
}

void
NSInPersonsListWindow::EvLButtonUp(uint /* modKeys */, NS_CLASSLIB::TPoint& /* pt */)
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  if (pSuper->bDragAndDrop)
  {
  	pSuper->DragDrop->DragLeave( *this ) ;
    pSuper->DragDrop->EndDrag() ;
    delete pSuper->DragDrop ;
    pSuper->DragDrop = 0 ;
    pSuper->bDragAndDrop = false ;
  }
}

// -----------------------------------------------------------------------------
// Retourne l'index du premier item sélectionné
// -----------------------------------------------------------------------------
int
NSInPersonsListWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
  int index = -1 ;

	for (int i = 0 ; i < count ; i++)  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;}

void
NSInPersonsListWindow::initNameField()
{
  _pNameField = (NSInPersonField*) 0 ;

  NSInPersonFieldArray* pVisibleFields = _pParentWindow->getVisibleFields() ;
  if (((NSInPersonFieldArray*) NULL == pVisibleFields) || pVisibleFields->empty())
    return ;

  NSInPersonFieldIter fieldsIt = pVisibleFields->begin() ;
  for ( ; (pVisibleFields->end() != fieldsIt) && (string("ZADMI1/LIDET1/LNOM01") != (*fieldsIt)->getPath()) ; fieldsIt++) ;
  if (pVisibleFields->end() == fieldsIt)
    return ;

  _pNameField = *fieldsIt ;
}

void
NSInPersonsListWindow::SelectLine(const int iLineIndex)
{
  if (iLineIndex < 0)
    return ;

  SetItemState(iLineIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
  EnsureVisible(iLineIndex, true) ;
}

// Get the column to be processed depending on user entry format
//
int
NSInPersonsListWindow::getCutField(const string sSeed)
{
  if ((string("") == sSeed) || _aInPersonsFull.empty())
    return 0 ;

  if (IsStayPattern(sSeed))
  {
    NSInPersonField* pPathField = _pParentWindow->getFieldForPath(string("LSEJO1/LNUSE1")) ;
    if ((NSInPersonField*) NULL == pPathField)
      return 0 ;

    return pPathField->getOrder() - 1 ;
  }

  if (IsDatePattern(sSeed))
  {
    NSInPersonField* pPathField = _pParentWindow->getFieldForPath(string("ZADMI1/LIDET1/KNAIS1")) ;
    if ((NSInPersonField*) NULL == pPathField)
      return 0 ;

    return pPathField->getOrder() - 1 ;
  }

  if (_pNameField)
    return _pNameField->getOrder() - 1 ;

  return 0 ;
}

void
NSInPersonsListWindow::SelectByName(const string sName)
{
  if ((string("") == sName) || _aInPersonsSelected.empty())
    return ;

  if (IsStayPattern(sName))
  {
    SelectByStayId(sName) ;
    return ;
  }

  if (IsDatePattern(sName))
  {
    SelectByDate(sName) ;
    return ;
  }

  if ((NSInPersonField*) NULL == _pNameField)
    return ;
  string sPathForName = _pNameField->getPath() ;

  sortByName() ;
  AfficheListe() ;

  string sUpperName = pseumaj(sName) ;

  size_t iIndexNumber = 0 ;
  NSInPersonIter personIt = _aInPersonsSelected.begin() ;

  while ((_aInPersonsSelected.end() != personIt) &&
         (pseumaj((*personIt)->getInformationDisplayData(sPathForName)) < sUpperName))
  {
    iIndexNumber++ ;
    personIt++ ;
  }

  SelectLine(iIndexNumber) ;
}

void
NSInPersonsListWindow::SelectByStayId(const string sStayId)
{
  if (string("") == sStayId)
    return ;

  string sPath = string("LSEJO1/LNUSE1") ;

  size_t iIndexNumber = 0 ;

  NSInPersonIter itInPerson = _aInPersonsSelected.begin() ;
  for ( ; _aInPersonsSelected.end() != itInPerson ; itInPerson++, iIndexNumber++)
  {
    NSInsidePersonInformation* pStayInfo = (*itInPerson)->getInformation(sPath) ;
    if (pStayInfo && (pStayInfo->getRawData() == sStayId))
    {
      SelectLine(iIndexNumber) ;
      return ;
    }
  }
}

void
NSInPersonsListWindow::SelectByDate(const string sDate)
{
  if (string("") == sDate)
    return ;

  string sPath = string("ZADMI1/LIDET1/KNAIS1") ;

  NSInPersonField* pPathField = _pParentWindow->getFieldForPath(sPath) ;
  if ((NSInPersonField*) NULL == pPathField)
    return ;

  int iStayIdCol = pPathField->getOrder() - 1 ;

  if (iStayIdCol != _iSortedColumn)
    sortBySelected(iStayIdCol) ;

  AfficheListe() ;

  size_t iIndexNumber = 0 ;

  NSInPersonIter itInPerson = _aInPersonsSelected.begin() ;
  for ( ; _aInPersonsSelected.end() != itInPerson ; itInPerson++, iIndexNumber++)
  {
    NSInsidePersonInformation* pStayInfo = (*itInPerson)->getInformation(sPath) ;
    if (pStayInfo && (pStayInfo->getRawData() == sDate))
    {
      SelectLine(iIndexNumber) ;
      return ;
    }
  }
}

bool
NSInPersonsListWindow::IsStayPattern(const string &sName)
{
  if (string("") == sName)
    return false ;

  // Digits only - Que des chiffres
  for (size_t i = 0 ; i < strlen(sName.c_str()) ; i++)
    if (!isdigit(sName[i]))
      return false ;

  return true ;
}

bool
NSInPersonsListWindow::IsDatePattern(const string &sName)
{
  string sFormat = pContexte->getSuperviseur()->getText("0localInformation", "dateFormat") ;
  return IsValidDatePattern(sName, sFormat) ;
}

void
NSInPersonsListWindow::sortByName()
{
  if ((NSInPersonField*) NULL == _pNameField)
    return ;

  int iListCol = _pNameField->getOrder() - 1 ;

  if ((_iSortedColumn == iListCol) && _bNaturallySorted)
    return ;

  __iColToSort  = _pNameField->getOrder() - 1 ;
  __sPathToSort = _pNameField->getPath() ;

  sort(_aInPersonsSelected.begin(), _aInPersonsSelected.end(), InPersonSortInf) ;

  _iSortedColumn    = iListCol ;
  _bNaturallySorted = true ;
}

void
NSInPersonsListWindow::LVNColumnclick(TLwNotify& lwn)
{
  int iCol = lwn.iSubItem ;

  changeSortOrder(iCol) ;
  sortBySelected(iCol) ;
  AfficheListe() ;
}

void
NSInPersonsListWindow::changeSortOrder(int iCol)
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

// Sort for a displayed col
//
void
NSInPersonsListWindow::sortByFull(int iCol)
{
  if (_aInPersonsFull.empty())
    return ;

  __sPathToSort = getPathForCol(iCol) ;

  sort(_aInPersonsFull.begin(), _aInPersonsFull.end(), InPersonSortInf) ;
}

void
NSInPersonsListWindow::sortBySelected(int iCol)
{
  if (_aInPersonsSelected.empty())
    return ;

  __sPathToSort = getPathForCol(iCol) ;

  if (_bNaturallySorted)
    sort(_aInPersonsSelected.begin(), _aInPersonsSelected.end(), InPersonSortInf) ;
  else
    sort(_aInPersonsSelected.begin(), _aInPersonsSelected.end(), InPersonSortSup) ;

  _iSortedColumn = iCol ;
}

void
NSInPersonsListWindow::CmOpenPatient()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSInsidePerson* pSelectedPerson = _aInPersonsSelected[iCurrentSelection] ;

  openPatient(pSelectedPerson) ;

  if (_bRemoveWhenOpening)
  {
    removeFromList(pSelectedPerson) ;
    refreshList() ;
  }
}

void
NSInPersonsListWindow::openPatient(NSInsidePerson* pInPerson)
{
  if ((NSInsidePerson*) NULL == pInPerson)
    return ;

	NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
  if ((NSEpisodus*) NULL == pEpisodus)
  	return ;

  pEpisodus->EmptyCaptureTank() ;

  // Before using capture tank, we check if this person already has a LdV ID
  //
  string sLdvId = pInPerson->getInformationRawData(string("£SPID1")) ;
  if (string("") != sLdvId)
  {
    NSToDoTask* pTask = new NSToDoTask ;
    pTask->setWhatToDo(string("OpenFromLdvId")) ;
    pTask->setParam1(sLdvId) ;
    pTask->sendMe(pContexte->getSuperviseur()) ;
    return ;
  }

	string sProcessLog = string("") ;

	NSCaptureArray* pNewCaptureArray = &(pEpisodus->newCaptureArray) ;
  if ((NSCaptureArray*) NULL == pNewCaptureArray)
    return ;

	// First, erase the information in the capture array.
  // Elsewhere, if we launch "openPatient", but don't really open it
  // ("Cancel" somewhere), then this information will remain and prevent another
  // "openPatient"
	//
  pNewCaptureArray->viderForOrigin(FromInPatientsList) ;

/*
  NSInPersonFieldArray* pFields = _pParentWindow->getFields() ;
  if (false == pFields->empty())
    for (NSInPersonFieldConstIter fieldIt = pFields->begin() ; pFields->end() != fieldIt ; fieldIt++)
      pNewCaptureArray->eraseEntry((*fieldIt)->getPath()) ;
*/

/*
	pNewCaptureArray->eraseEntry(string("ZADMI1/LIDET1/LNOM01")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LIDET1/LNOM21")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LCOOR1/LADRE1/LADL11")) ;
	pNewCaptureArray->eraseEntry(string("ZADMI1/LCOOR1/LADRE1/LADL21")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LIDET1/KNAIS1")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LFRAN1/LFRAB1")) ;
  pNewCaptureArray->eraseEntry(string("ZADMI1/LIDET1/LSEXE1")) ;

  pNewCaptureArray->eraseEntry(string("0LIBI1/LSITE1/LIPP01")) ;

  pNewCaptureArray->eraseEntry(string("LSEJO1/LNUSE1")) ;
  pNewCaptureArray->eraseEntry(string("LSEJO1/KOUVR1")) ;
  pNewCaptureArray->eraseEntry(string("LSEJO1/KFERM1")) ;
  pNewCaptureArray->eraseEntry(string("LSEJO1/LUNIF1/0CODE1")) ;
  pNewCaptureArray->eraseEntry(string("LSEJO1/UETAB1/LFINE1")) ;
*/

  NSInPersonInformationArray* pInformations = pInPerson->getInformations() ;
  if (((NSInPersonInformationArray*) NULL == pInformations) || pInformations->empty())
    return ;

  for (NSInPersonInformationIter it = pInformations->begin() ; pInformations->end() != it ; it++)
    pNewCaptureArray->ajouter(new NSCapture(pContexte, (*it)->getPath(), (*it)->getData(), FromInPatientsList, string(""), string(""), string(""))) ;

  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("OpenFromCapture")) ;
  pTask->sendMe(pContexte->getSuperviseur()) ;
}

void
NSInPersonsListWindow::addCurrentPatient()
{
  NSPatientChoisi* pPatient = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatient)
    return ;

  NSInsidePerson PersonToAdd ;
  fillInformationForPatient(&PersonToAdd) ;

  if (areInformationToAsk(&PersonToAdd))
  {
    MissingInformationDialog* pDialog = new MissingInformationDialog(pContexte, this, &PersonToAdd) ;
    int idRet = pDialog->Execute() ;
    delete pDialog ;

    if (IDOK != idRet)
		  return ;
  }

  addToList(&PersonToAdd) ;

  refreshList() ;

  if (_bCloseAfterAdding)
  {
    NSToDoTask* pTask = new NSToDoTask ;
    pTask->setWhatToDo(string("CloseCurrentPatient")) ;
    pTask->sendMe(pContexte->getSuperviseur()) ;
  }
}

void
NSInPersonsListWindow::fillInformationForPatient(NSInsidePerson* pPersonToFill)
{
  if ((NSInsidePerson*) NULL == pPersonToFill)
    return ;

  NSInPersonFieldArray* pFields = _pParentWindow->getFields() ;
  if (((NSInPersonFieldArray*) NULL == pFields) || pFields->empty())
    return ;

  size_t iFieldsCount = pFields->size() ;
  for (size_t iField = 1 ; iField <= iFieldsCount ; iField++)
  {
    NSInPersonField* pField = _pParentWindow->getField(iField) ;
    if (pField)
    {
      string sInformation = getInformationForPath(pField->getPath(), pField->getFormat()) ;

      NSInsidePersonInformation info(iField, sInformation, pField->getPath(), sInformation, sInformation, pField->getUnit()) ;
      pPersonToFill->addInformation(&info) ;
    }
  }
}

// Just get raw information since the goal is only to build the CSV line
//
string
NSInPersonsListWindow::getInformationForPath(const string sPath, const string sFormat)
{
  if (string("") == sPath)
    return string("") ;

  // Special case: when asking for Ldv Id, return it immediately
  //
  if (string("£SPID1") == sPath)
    return pContexte->getPatient()->getNss() ;

  // Regular case: ask the question to the blackboard
  //
  NSPatPathoArray* pPatPathoArray = NULL ;
  string date_fils = string("") ;

  // on pose la question au blackboard
  int res = pContexte->getBBinterface()->precoche(sPath, &pPatPathoArray, HWindow, &date_fils) ;
  if ((1 == res) && (pPatPathoArray) && (false == pPatPathoArray->empty()))
  {
    string sReturn = string("") ;

    // Simplistic approach
    //
    PatPathoIter iter = pPatPathoArray->begin() ;
    iter++ ;
    if (pPatPathoArray->end() != iter)
      sReturn = reverseFormat(*iter, sFormat) ;

    delete pPatPathoArray ;
    return sReturn ;
  }

  if (pPatPathoArray)
    delete pPatPathoArray ;

  return string("") ;
}

string
NSInPersonsListWindow::reverseFormat(const NSPatPathoInfo* pPatPatho, const string sFormat)
{
  if ((NSPatPathoInfo*) NULL == pPatPatho)
    return string("") ;

  // What to return if there is no format, or an unknow format
  //
  string sReturn = pPatPatho->getTexteLibre() ;
  if (string("") == sReturn)
    sReturn = pPatPatho->getComplement() ;

  if (string("") == sFormat)
    return sReturn ;

  size_t iFormatSize = strlen(sFormat.c_str()) ;

  // Format for gender (of the kind $SS|Value for male|Value for female|$
  //
  if ((iFormatSize > 7) && (string("$SS|") == string(sFormat, 0, 4)))
  {
    string sGender = pPatPatho->getLexiqueSens() ;
    if (string("") == sGender)
      return string("") ;

    // Get "Value for male|Value for female" and parse it
    //
    string sValues = string(sFormat, 4, iFormatSize - 6) ;
    size_t iPos = sValues.find("|") ;
    if (NPOS != iPos)
    {
      if (string("HMASC") == sGender)
        return string(sValues, 0, iPos) ;
      else
        return string(sValues, iPos+1, strlen(sValues.c_str())-iPos-1) ;
    }

    return string("") ;
  }

  string sLexiSens = pPatPatho->getLexiqueSens() ;

  // If a date, format it
  //
  if ((string("£D0") == sLexiSens) || (string("£T0") == sLexiSens))
  {
    string sDate = pPatPatho->getComplement() ;
    size_t iDateLen = strlen(sDate.c_str()) ;

    if (iDateLen >= 8)
    {
      string sYear  = string(sDate, 0, 4) ;
      string sMonth = string(sDate, 4, 2) ;
      string sDay   = string(sDate, 6, 2) ;

      sReturn = sFormat ;

      sReturn = replaceAll(sReturn, string("$AAAA$"), sYear) ;
      sReturn = replaceAll(sReturn, string("$MM$"), sMonth) ;
      sReturn = replaceAll(sReturn, string("$JJ$"), sDay) ;

      if (iDateLen >= 12)
      {
        string sHour   = string(sDate,  8, 2) ;
        string sMinute = string(sDate, 10, 2) ;

        sReturn = replaceAll(sReturn, string("$hh$"), sHour) ;
        sReturn = replaceAll(sReturn, string("$mm$"), sMinute) ;
      }

      return sReturn ;
    }

    return string("") ;
  }

  return sReturn ;
}

bool
NSInPersonsListWindow::areInformationToAsk(const NSInsidePerson* pInsidePerson)
{
  if ((NSInsidePerson*) NULL == pInsidePerson)
    return false ;

  NSInPersonFieldArray* pFields = _pParentWindow->getFields() ;
  if (((NSInPersonFieldArray*) NULL == pFields) || pFields->empty())
    return false ;

  size_t iFieldsCount = pFields->size() ;
  for (size_t iField = 1 ; iField <= iFieldsCount ; iField++)
  {
    NSInPersonField* pField = _pParentWindow->getField(iField) ;
    if (pField && pField->mustAskIfMissing())
    {
      NSInsidePersonInformation* pIPI = pInsidePerson->getInformation(iField) ;
      if (pIPI && (string("") == pIPI->getRawData()))
        return true ;
    }
  }

  return false ;
}

void
NSInPersonsListWindow::removeFromList(NSInsidePerson* pPersonToRemove)
{
  if ((NSInsidePerson*) NULL == pPersonToRemove)
  {
    string sTrace = string("Asked to remove a \"null preson\" from In Persons list") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    return ;
  }

  string sLabelPids = pPersonToRemove->getInformationData("£SPID1") ;
  string sLabelName = pPersonToRemove->getInformationData("ZADMI1/LIDET1/LNOM01") + string(" ") + pPersonToRemove->getInformationData("ZADMI1/LIDET1/LNOM21") ;

  string sLabel = string("") ;
  if (string("") != sLabelPids)
    sLabel = sLabelPids + string(" (") + sLabelName + string(")") ;
  else
    sLabel = sLabelName ;

  string sTrace = string("Removing ") + sLabel + string(" from In Persons list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  if (false == createLockFile())
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // Open copy file
  //
  string sTempFileName = getTempFileName() ;
  if (string("") == sTempFileName)
  {
    string sErreur = string("Cannot create the temporary file to remove from list: the temporary file name is empty") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return ;
  }

  ofstream outFile ;
  outFile.open(sTempFileName.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sTempFileName ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  // Open original file
  //
  ifstream inFile ;
  inFile.open(_sCsvFileFullName.c_str()) ;
	if (!inFile)
	{
    string sErrorText = pSuper->getText("fileErrors", "errorOpeningInputFile") + string(" ") + _sCsvFileFullName ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    outFile.close() ;
    return ;
  }

  while (!inFile.eof())
  {
    string sLine = string("") ;

    // Get line from input
    //
    getline(inFile, sLine) ;

    // Write it to output if it is not compatible with the person to be deleted
    //
    if ((string("") != sLine) && (false == isCompatible(sLine, pPersonToRemove)))
      outFile << (sLine + string(NEWLINE)) ;
  }

  inFile.close() ;
  outFile.close() ;

  // Now, it is time to delete previous fila and rename the new one
  //
  NsDeleteTemporaryFile(pSuper, _sCsvFileFullName) ;

  ::MoveFile(sTempFileName.c_str(), _sCsvFileFullName.c_str()) ;

  deleteLockFile() ;
}

void
NSInPersonsListWindow::addToList(NSInsidePerson* pPersonToAdd)
{
  if ((NSInsidePerson*) NULL == pPersonToAdd)
    return ;

  // Prepare string to append
  //
  string sNewString = string("") ;

  NSInPersonFieldArray* pFields = _pParentWindow->getFields() ;
  if (((NSInPersonFieldArray*) NULL == pFields) || pFields->empty())
    return ;

  size_t iFieldsCount = pFields->size() ;
  for (size_t iField = 1 ; iField <= iFieldsCount ; iField++)
  {
    string sToAdd = string("") ;
    NSInsidePersonInformation* pIPI = pPersonToAdd->getInformation(iField) ;
    if (pIPI)
      sToAdd = pIPI->getRawData() ;

    if (iField > 1)
      sNewString += string(1, _cCvsSeparator) ;
    sNewString += sToAdd ;
  }

  if (string("") == sNewString)
    return ;

  // Add at the end of the file
  //
  if (false == createLockFile())
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // Open file in append mode
  //
  ofstream outFile ;
  outFile.open(_sCsvFileFullName.c_str(), ios::out | ios::app) ;
  if (!outFile)
  {
    string sErreur = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + _sCsvFileFullName ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return ;
  }

  outFile << (sNewString + string(NEWLINE)) ;

  outFile.close() ;

  deleteLockFile() ;
}

bool
NSInPersonsListWindow::createLockFile()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sLockFileName = getLockFileName() ;
  if (string("") == sLockFileName)
  {
    string sErreur = string("Cannot create the lock file: the lock file name is empty") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  // Wait while a .lck file already exists
  //
  NVLdVTemps tLimit ;
  tLimit.takeTime() ;
  tLimit.ajouteSecondes(2) ;

  bool   bLookingForOpening = true ;
  size_t iAttemptNumber     = 0 ;

  while (bLookingForOpening)
  {
    if (NsFileExists(sLockFileName))
    {
      iAttemptNumber++ ;

      NVLdVTemps tNow ;
      tNow.takeTime() ;

      if (tNow > tLimit)
      {
        string sErreur = pSuper->getText("fileErrors", "errorOpeningFile") + string(" ") + sLockFileName ;
        pSuper->trace(&sErreur, 1, NSSuper::trError) ;
        erreur(sErreur.c_str(), standardError, 0) ;
        return false ;
      }

      TMyApp* pNSApplication = pSuper->getApplication() ;
      if (pNSApplication)
        pNSApplication->PumpWaitingMessages() ;
    }
    else
      bLookingForOpening = false ;
  }

  // Create the lock file
  //
  ofstream outFile ;
  outFile.open(sLockFileName.c_str(), ios::out) ;
  if (!outFile)
  {
    string sErreur = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sLockFileName ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  string sContent = pSuper->getConsoleString() + string(" - ") + tNow.donneDateHeure() ;

  outFile << (sContent + string(NEWLINE)) ;
  outFile.close() ;

  return true ;
}

bool
NSInPersonsListWindow::deleteLockFile()
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sLockFileName = getLockFileName() ;
  if (string("") == sLockFileName)
  {
    string sErreur = string("Cannot delete the lock file: the lock file name is empty") ;
    pSuper->trace(&sErreur, 1, NSSuper::trError) ;
    return false ;
  }

  return NsDeleteTemporaryFile(pSuper, sLockFileName) ;
}

string
NSInPersonsListWindow::getLockFileName()
{
  if (string("") == _sCsvFileFullName)
    return string("") ;

  string sRoot      = string("") ;
  string sExtension = string("") ;
  NsParseExtension(_sCsvFileFullName, sRoot, sExtension) ;

  return sRoot + string(".lck") ;
}

string
NSInPersonsListWindow::getTempFileName()
{
  if (string("") == _sCsvFileFullName)
    return string("") ;

  string sRoot      = string("") ;
  string sExtension = string("") ;
  NsParseExtension(_sCsvFileFullName, sRoot, sExtension) ;

  return sRoot + string(".tmp") ;
}

bool
NSInPersonsListWindow::isCompatible(const string sLine, const NSInsidePerson* pPerson)
{
  if ((string("") == sLine) || ((NSInsidePerson*) NULL == pPerson))
    return false ;

  int iColNumber = 0 ;

  int    iQuoteCounter = 0 ;
  string sEntry = string("") ;

  for (size_t i = 0 ; i < strlen(sLine.c_str()) ; i++)
  {
    char c = sLine[i] ;

    // Separator that is a genuine separator (not located inside quotes)
    //
    if ((c == _cCvsSeparator) && (0 == iQuoteCounter))
    {
      NSInsidePersonInformation* pInformation = pPerson->getInformation(iColNumber) ;
      if ((NSInsidePersonInformation*) NULL == pInformation)
        return false ;

      // Remove starting and ending quotes (if any)
      //
      size_t iEntryLen = strlen(sEntry.c_str()) ;
      if ((iEntryLen > 2) && ('\"' == sEntry[0]) && ('\"' == sEntry[iEntryLen-1]))
        sEntry = string(sEntry, 1, iEntryLen - 2) ;

      // Compare string with information's raw data
      //
      if (pInformation->getRawData() != sEntry)
        return false ;

      iQuoteCounter++ ;
      sEntry = string("") ;
    }
    else
      sEntry += string(1, c) ;
  }

  return true ;
}

void
NSInPersonsListWindow::EvSetFocus(HWND /* hWndLostFocus */)
{
  if (false == skinSwitch("InPatientsListOn"))
  {
	  SetBkColor(0x00ff6f6f) ; // 0x00bbggrr
    SetTextBkColor(0x00ff6f6f) ;
  }
  Invalidate() ;

  // pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSInPersonsListWindow::EvKillFocus(HWND /* hWndGetFocus */)
{
  if (false == skinSwitch("InPatientsListOff"))
  {
	  SetBkColor(0x00ffffff) ; // 0x00bbggrr
    SetTextBkColor(0x00ffffff) ;
  }
  Invalidate() ;
}

/****************** classe NSInsidePerson **************************/

NSInsidePerson::NSInsidePerson()
{
  _sName = string("") ;

  lObjectCount++ ;
}

NSInsidePerson::~NSInsidePerson()
{
  lObjectCount-- ;
}

NSInsidePerson::NSInsidePerson(const NSInsidePerson& rv)
{
  _aInformation = rv._aInformation ;
  _sName        = rv._sName ;

  lObjectCount++ ;
}

NSInsidePerson&
NSInsidePerson::operator=(const NSInsidePerson& src)
{
  if (&src == this)
		return *this ;

  _aInformation = src._aInformation ;
  _sName        = src._sName ;

  return *this ;
}

int
NSInsidePerson::operator==(const NSInsidePerson& o) const
{
  if (_sName != o._sName)
    return 0 ;

  if (_aInformation.empty())
  {
    if (o._aInformation.empty())
      return 1 ;
    else
      return 0 ;
  }
  if (o._aInformation.empty())
      return 0 ;

  NSInPersonInformationConstIter localIt = _aInformation.begin() ;
  NSInPersonInformationConstIter otherIt = o._aInformation.begin() ;

  for ( ; (_aInformation.end() != localIt) &&
          (o._aInformation.end() != otherIt) && (**otherIt == **localIt) ;
                                                        localIt++, otherIt++) ;

  if ((_aInformation.end() != localIt) || (o._aInformation.end() != otherIt))
    return 0 ;

  return 1 ;
}

NSInsidePersonInformation*
NSInsidePerson::getInformation(const string sPath) const
{
  if ((string("") == sPath) || _aInformation.empty())
    return (NSInsidePersonInformation*) 0 ;

  NSInPersonInformationConstIter localIt = _aInformation.begin() ;
  for ( ; _aInformation.end() != localIt ; localIt++)
    if ((*localIt)->getPath() == sPath)
      return *localIt ;

  return (NSInsidePersonInformation*) 0 ;
}

NSInsidePersonInformation*
NSInsidePerson::getInformation(int iIndex) const
{
  if (_aInformation.empty())
    return (NSInsidePersonInformation*) 0 ;

  NSInPersonInformationConstIter localIt = _aInformation.begin() ;
  for ( ; _aInformation.end() != localIt ; localIt++)
    if ((*localIt)->getFieldIndex() == iIndex)
      return *localIt ;

  return (NSInsidePersonInformation*) 0 ;
}

string
NSInsidePerson::getInformationData(const string sPath) const
{
  NSInsidePersonInformation* pInfo = getInformation(sPath) ;
  if (pInfo)
    return pInfo->getData() ;

  return string("") ;
}

string
NSInsidePerson::getInformationRawData(const string sPath) const
{
  NSInsidePersonInformation* pInfo = getInformation(sPath) ;
  if (pInfo)
    return pInfo->getRawData() ;

  return string("") ;
}

string
NSInsidePerson::getInformationDisplayData(const string sPath) const
{
  NSInsidePersonInformation* pInfo = getInformation(sPath) ;
  if (pInfo)
    return pInfo->getDisplayData() ;

  return string("") ;
}

/****************** classe NSInsidePersonInformation ***********************/

NSInsidePersonInformation::NSInsidePersonInformation()
{
  _sPath        = string("") ;
  _sData        = string("") ;
  _sUnit        = string("") ;
  _sRawData     = string("") ;
  _sDisplayData = string("") ;

  _iFieldIndex = -1 ;

  lObjectCount++ ;
}

NSInsidePersonInformation::NSInsidePersonInformation(int iFieldIndex, string sRawData, string sPath, string sData, string sDisplayData, string sUnit)
{
  _sPath        = sPath ;
  _sData        = sData ;
  _sUnit        = sUnit ;
  _sRawData     = sRawData ;
  _sDisplayData = sDisplayData ;

  _iFieldIndex = iFieldIndex ;

  lObjectCount++ ;
}

NSInsidePersonInformation::~NSInsidePersonInformation()
{
  lObjectCount-- ;
}

NSInsidePersonInformation::NSInsidePersonInformation(const NSInsidePersonInformation& rv)
{
  _sPath        = rv._sPath ;
  _sData        = rv._sData ;
  _sUnit        = rv._sUnit ;
  _sRawData     = rv._sRawData ;
  _sDisplayData = rv._sDisplayData ;

  _iFieldIndex  = rv._iFieldIndex ;

  lObjectCount++ ;
}

NSInsidePersonInformation&
NSInsidePersonInformation::operator=(const NSInsidePersonInformation& src)
{
  if (this == &src)
		return *this ;

  _sPath        = src._sPath ;
  _sData        = src._sData ;
  _sUnit        = src._sUnit ;
  _sRawData     = src._sRawData ;
  _sDisplayData = src._sDisplayData ;

  _iFieldIndex  = src._iFieldIndex ;

  return *this ;
}

int
NSInsidePersonInformation::operator==(const NSInsidePersonInformation& o)
{
  if ((_sPath       == o._sPath) &&
      (_sData       == o._sData) &&
      (_sUnit       == o._sUnit) &&
      (_sRawData    == o._sRawData) &&
      (_iFieldIndex == o._iFieldIndex))
    return 1 ;

  return 0 ;
}

/****************** classe NSInPersonsGadgetWindow **************************/

DEFINE_RESPONSE_TABLE1(NSInPersonsGadgetWindow, TGadgetWindow)
  EV_WM_SETFOCUS,
  EV_COMMAND(CM_OPEN_PAT,    CmOpenPatient),
  EV_COMMAND(CM_RECH_MANUAL, CmManual),
  EV_COMMAND(CM_REFRESH,     CmRefresh),
  EV_COMMAND(CM_ADD_PATIENT, CmAddCurrentPatient),
END_RESPONSE_TABLE ;

NSInPersonsGadgetWindow::NSInPersonsGadgetWindow(NSContexte* pCtx, TWindow* parent, bool /* bAddButtons */, const char far* /* title */)
                        :TGadgetWindow(parent), NSRoot(pCtx)
{
  _pPaneSplitter = (NSInPersonsWindow*) 0 ;
  _pRechNomEdit  = (NSInPersonsRechNomEdit*) 0 ;
}

NSInPersonsGadgetWindow::~NSInPersonsGadgetWindow()
{
}

void
NSInPersonsGadgetWindow::SetupWindow()
{
  TGadgetWindow::SetupWindow() ;

  string sIcons = _pPaneSplitter->getIcons() ;
  if (string("") == sIcons)
  {
    sIcons = string("CM_REFRESH|ED_SEARCH|CM_OPEN|SEPARATOR|CM_BASE") ;

    if (_pPaneSplitter && _pPaneSplitter->getList() && _pPaneSplitter->getList()->canAddToList())
      sIcons += string("|SEPARATOR|CM_ADD") ;
  }

  ClasseStringVector aIcons ;
  DecomposeChaine(&sIcons, &aIcons, string("|")) ;

  if (false == aIcons.empty())
  {
    for (iterClassString it = aIcons.begin() ; aIcons.end() != it ; it++)
    {
      if ((*it)->getItem() == string("CM_REFRESH"))
      {
        NSLocalChapterArray* pDescriptor = _pPaneSplitter->getFileDescriptor() ;
        string sName = pDescriptor->getLocalText(string("global"), string("butnCaption")) ;
        if (string("") == sName)
          sName = pContexte->getSuperviseur()->getText("generalLanguage", "famillyName") ;

        NSGadgetButton* pRefreshButton = new NSGadgetButton(this, CM_REFRESH, sName.c_str(), 0, 0, 100, 20) ;
        pRefreshButton->Create() ;
        Insert(*new TControlGadget(*pRefreshButton)) ;
      }
      else if ((*it)->getItem() == string("ED_SEARCH"))
      {
        _pRechNomEdit = new NSInPersonsRechNomEdit(_pPaneSplitter, this, CM_RECH_EDIT, "", 110, 0, 200, 20) ;
        _pRechNomEdit->Create() ;
        Insert(*new NSInPersonsEditControlGadget(*_pRechNomEdit, NS_CLASSLIB::TRect(0, 0, 200, 30), NS_CLASSLIB::TRect(0, 0, 200, 20))) ;
      }
      else if ((*it)->getItem() == string("CM_OPEN"))
      {
        string sName = pContexte->getSuperviseur()->getText("inPatiensList", "openPatient") ;
        NSGadgetButton* pOpenButton = new NSGadgetButton(this, CM_OPEN_PAT, sName.c_str(), 0, 0, 200, 20) ;
        pOpenButton->Create() ;
        Insert(*new TControlGadget(*pOpenButton)) ;
      }
      else if ((*it)->getItem() == string("CM_BASE"))
      {
        string sName = pContexte->getSuperviseur()->getText("inPatiensList", "searchInBase") ;
        NSGadgetButton* pManualButton = new NSGadgetButton(this, CM_RECH_MANUAL, sName.c_str(), 0, 0, 200, 20) ;
        pManualButton->Create() ;
        Insert(*new TControlGadget(*pManualButton)) ;
      }
      else if ((*it)->getItem() == string("CM_ADD"))
      {
        string sName = pContexte->getSuperviseur()->getText("inPatiensList", "addPatient") ;
        NSGadgetButton* pAddButton = new NSGadgetButton(this, CM_ADD_PATIENT, sName.c_str(), 0, 0, 200, 20) ;
        pAddButton->Create() ;
        Insert(*new TControlGadget(*pAddButton)) ;
      }
      else if ((*it)->getItem() == string("SEPARATOR"))
        Insert(*new TSeparatorGadget(30)) ;
    }
  }

  LayoutSession() ;
}

void
NSInPersonsGadgetWindow::EvSetFocus(HWND hWndLostFocus)
{
  TWindow::EvSetFocus(hWndLostFocus) ;
}

void
NSInPersonsGadgetWindow::CmOpenPatient()
{
  if ((NSInPersonsWindow*) NULL == _pPaneSplitter)
    return ;

  NSInPersonsListWindow* pList = _pPaneSplitter->getList() ;
  if ((NSInPersonsListWindow*) NULL == pList)
    return ;

  pList->CmOpenPatient() ;
}

void
NSInPersonsGadgetWindow::CmManual()
{
  if ((NSInPersonsWindow*) NULL == _pPaneSplitter)
    return ;

  _pPaneSplitter->CmManual() ;
}

void
NSInPersonsGadgetWindow::CmRefresh()
{
  if ((NSInPersonsWindow*) NULL == _pPaneSplitter)
    return ;

  _pPaneSplitter->refreshList() ;
}

void
NSInPersonsGadgetWindow::CmAddCurrentPatient()
{
  if ((NSInPersonsWindow*) NULL == _pPaneSplitter)
    return ;

  _pPaneSplitter->addCurrentPatient() ;
}

void
NSInPersonsGadgetWindow::activateParent()
{
}

// Take care: The size fixed here is the gadget window's container
//            dimension. The size of the "grey area" for controls
//            will depend from gadgets size and shrink settings
//
int
NSInPersonsGadgetWindow::getGadgetsHeight()
{
  return 30 ;
}

/****************** classe NSInPersonField **************************/

NSInPersonField::NSInPersonField()
{
  init() ;

  lObjectCount++ ;
}

NSInPersonField::NSInPersonField(int iNumber, string sTitle, string sPath,
                                 int iOrder, string sFormat, string sDisplay,
                                 int iWidth, string sUnit, string sLexicon,
                                 bool bMustAskIfMissing)
{
  _iNumber       = iNumber ;
  _iOrder        = iOrder ;
  _iWidth        = iWidth ;
  _sChapter      = string("") ;
  _sTitle        = sTitle ;
  _sPath         = sPath ;
  _sFormat       = sFormat ;
  _sDisplay      = sDisplay ;
  _sUnit         = sUnit ;
  _sLexicon      = sLexicon ;
  _bAskIfMissing = bMustAskIfMissing ;

  lObjectCount++ ;
}

NSInPersonField::NSInPersonField(NSLocalisedChapter* pLocalisedChapter)
{
  init() ;

  lObjectCount++ ;

  if ((NSLocalisedChapter*) NULL == pLocalisedChapter)
    return ;

  // A path is always needed
  //
  _sPath = pLocalisedChapter->getLocalText(string("path")) ;
  if (string("") == _sPath)
    return ;

  _sChapter = pLocalisedChapter->getChapter() ;

  string sUpperChapterTitle = pseumaj(_sChapter) ;
  size_t iTitleLen = strlen(sUpperChapterTitle.c_str()) ;
  if ((iTitleLen > 5) && (string("FIELD") == string(sUpperChapterTitle, 0, 5)))
  {
    string sNumber = string(sUpperChapterTitle, 5, iTitleLen - 5) ;
    _iNumber = StringToInt(sNumber) ;
  }

  _sTitle   = pLocalisedChapter->getLocalText(string("title")) ;
  _sFormat  = pLocalisedChapter->getLocalText(string("format")) ;
  _sDisplay = pLocalisedChapter->getLocalText(string("display")) ;

  string sOrder = pLocalisedChapter->getLocalText(string("order")) ;
  if (string("") != sOrder)
    _iOrder = StringToInt(sOrder) ;

  string sWidth = pLocalisedChapter->getLocalText(string("width")) ;
  if (string("") != sWidth)
    _iWidth = StringToInt(sWidth) ;

  _sUnit    = pLocalisedChapter->getLocalText(string("unit")) ;
  _sLexicon = pLocalisedChapter->getLocalText(string("lexicon")) ;

  string sMustAsk = pLocalisedChapter->getLocalText(string("askIfEmpty")) ;
  if (string("") != sMustAsk)
    _bAskIfMissing = IsYes(sMustAsk) ;
}

NSInPersonField::~NSInPersonField()
{
  lObjectCount-- ;
}

NSInPersonField::NSInPersonField(const NSInPersonField& rv)
{
  _iNumber       = rv._iNumber ;
  _iOrder        = rv._iOrder ;
  _iWidth        = rv._iWidth ;
  _sChapter      = rv._sChapter ;
  _sTitle        = rv._sTitle ;
  _sPath         = rv._sPath ;
  _sFormat       = rv._sFormat ;
  _sDisplay      = rv._sDisplay ;
  _sUnit         = rv._sUnit ;
  _sLexicon      = rv._sLexicon ;
  _bAskIfMissing = rv._bAskIfMissing ;

  lObjectCount++ ;
}

void
NSInPersonField::init()
{
  _iNumber       = -1 ;
  _iOrder        = -1 ;
  _iWidth        = 100 ;
  _sTitle        = string("") ;
  _sChapter      = string("") ;
  _sPath         = string("") ;
  _sFormat       = string("") ;
  _sDisplay      = string("") ;
  _sUnit         = string("") ;
  _sLexicon      = string("") ;
  _bAskIfMissing = false ;
}

NSInPersonField&
NSInPersonField::operator=(const NSInPersonField& src)
{
  if (&src == this)
		return *this ;

  _iNumber       = src._iNumber ;
  _iOrder        = src._iOrder ;
  _iWidth        = src._iWidth ;
  _sChapter      = src._sChapter ;
  _sTitle        = src._sTitle ;
  _sPath         = src._sPath ;
  _sFormat       = src._sFormat ;
  _sDisplay      = src._sDisplay ;
  _sUnit         = src._sUnit ;
  _sLexicon      = src._sLexicon ;
  _bAskIfMissing = src._bAskIfMissing ;

  return *this ;
}

/****************** classe NSInPersonsRechNomEdit **************************/

DEFINE_RESPONSE_TABLE1(NSInPersonsRechNomEdit, OWL::TEdit)
	EV_WM_CHAR,
	EV_WM_KEYUP,
  EV_WM_SETFOCUS,
END_RESPONSE_TABLE;

NSInPersonsRechNomEdit::NSInPersonsRechNomEdit(NSInPersonsWindow* pIPW, NSInPersonsGadgetWindow* pere, int resId, const char far* text, int x, int y, int w, int h)
                       :OWL::TEdit((TWindow*) pere, resId, text, x, y, w, h)
{
  pInPersonsWindow = pIPW ;
}

//
// On doit intercepter EvChar pour interdire l'espace et le double-espace
//
void
NSInPersonsRechNomEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	string sNom = GetText() ;

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
NSInPersonsRechNomEdit::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	OWL::TEdit::EvKeyUp(key, repeatCount, flags) ;

  if (VK_RETURN == key)
    GoGetList() ;
  else
    SelectTextInList() ;
}

void
NSInPersonsRechNomEdit::EvSetFocus(HWND hWndLostFocus)
{
  OWL::TEdit::EvSetFocus(hWndLostFocus) ;

  // SelectTextInList() ;
}

string
NSInPersonsRechNomEdit::GetText()
{
  int iBuffLen = GetTextLen() ;
	char far* str = new char[iBuffLen + 1] ;

	OWL::TEdit::GetText(str, iBuffLen + 1) ;
	string sContent = string(str) ;
	delete[] str ;

  return sContent ;
}

void
NSInPersonsRechNomEdit::SelectTextInList()
{
  string sText = GetText() ;

  pInPersonsWindow->seedChanged(sText) ;
}

void
NSInPersonsRechNomEdit::GoGetList()
{
  string sText = GetText() ;

  pInPersonsWindow->goGetList(sText) ;
}

/****************** classe MissingInformationControl *************************/

MissingInformationControl::MissingInformationControl(int iInformationIndex)
{
  _iInformationIndex = iInformationIndex ;

  _pEdit  = (NSUtilEdit*) 0 ;
  _pCombo = (NTComboBox<string>*) 0 ;
}

MissingInformationControl::~MissingInformationControl()
{
  if (_pEdit)
    delete _pEdit ;
  if (_pCombo)
    delete _pCombo ;
}

/****************** classe MissingInformationDialog **************************/

DEFINE_RESPONSE_TABLE1(MissingInformationDialog, OWL::TDialog)
	EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

MissingInformationDialog::MissingInformationDialog(NSContexte* pCtx, NSInPersonsListWindow *AParent, NSInsidePerson* pPersonInfo, TModule *pNSResModule)
                         :NSUtilDialog(AParent, pCtx, "DLG_BASE", pNSDLLModule)
{
  _pParent      = AParent ;
  _pPersonInfo  = pPersonInfo ;

  CreateControls() ;
  CreateButtons() ;
}

MissingInformationDialog::~MissingInformationDialog()
{
  if (false == aControls.empty())
  {
    for (MissingInformationControlIter it = aControls.begin() ; aControls.end() != it ; )
    {
      delete *it ;
      aControls.erase(it) ;
    }
  }
}

void
MissingInformationDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  //
  // Redimentionnement
  //
  NS_CLASSLIB::TRect dlgRect ;
  GetWindowRect(dlgRect) ;
  NS_CLASSLIB::TRect clientRect ;
  GetClientRect(clientRect) ;
  //
  // On compare le clientRect réel avec les dimensions souhaitées,
  // et on modifie le WindowRect en conséquence
  //
  NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(0, 0, 250, _iNextCtrlTop) ;
  MapDialogRect(cvtRect) ;

  int iExternalHeight = dlgRect.Height() - clientRect.Height() ;
  int nouvWindowHeight = cvtRect.Height() + iExternalHeight ;
  MoveWindow(dlgRect.left, dlgRect.top, dlgRect.Width(), nouvWindowHeight) ;

  // Setting the caption
  //
  string sCaption = pContexte->getSuperviseur()->getText("inPatiensList", "complementaryInformation") ;
  SetCaption(sCaption.c_str()) ;

  // Initialize controls
  //
  InitializeControls() ;
}

//
// Create a control for each mandatory field with missing information
//
void
MissingInformationDialog::CreateControls()
{
  _iNextCtrlId  = 101 ;
  _iNextCtrlTop = 4 ;

  _iCtrlInterval   = 30 ;
  _iLeftForStatic  = 4 ;
  _iStaticWidth    = 150 ;
  _iLeftForControl = 154 ;

  // Check if all is properly connected
  //
  if ((NSInsidePerson*) NULL == _pPersonInfo)
    return ;

  NSInPersonFieldArray* pFields = getFields() ;
  if (((NSInPersonFieldArray*) NULL == pFields) || pFields->empty())
    return ;

  // Create a control for each mandatory field with missing information
  //
  size_t iFieldsCount = pFields->size() ;
  for (size_t iField = 1 ; iField <= iFieldsCount ; iField++)
  {
    NSInPersonField* pField = _pParent->getInPersonsWindow()->getField(iField) ;
    if (pField && pField->mustAskIfMissing())
    {
      NSInsidePersonInformation* pIPI = _pPersonInfo->getInformation(iField) ;
      if (pIPI && (string("") == pIPI->getRawData()))
      {
        NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForStatic, _iNextCtrlTop, _iLeftForStatic + _iStaticWidth, 20 + _iNextCtrlTop) ;
        MapDialogRect(cvtRect) ;
        TStatic* pStatic = new TStatic(this, -1, pField->getTitle().c_str(), cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height()) ;

        MissingInformationControl* pCtrl = new MissingInformationControl(iField) ;

        if (string("DPRAT1") == pField->getPath())
          CreateUserCombo(pCtrl) ;
        else if (string("") != pField->getUnit())
          CreateEditFromUnit(pCtrl, pField->getUnit()) ;
        else
          _iNextCtrlTop += _iCtrlInterval ;

        aControls.push_back(pCtrl) ;
      }
    }
  }
}

void
MissingInformationDialog::CreateButtons()
{
  // On crée le séparateur
  //
  NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForStatic, _iNextCtrlTop, 350, _iNextCtrlTop + 1) ;
  MapDialogRect(cvtRect) ;

  OWL::TGroupBox* pDip = new OWL::TGroupBox(this, -1, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*TModule**/ 0) ;
  pDip->Attr.Style |= BSS_HDIP ;

  _iNextCtrlTop += 12 ;

  // On crée les boutons
  //
  cvtRect = NS_CLASSLIB::TRect(45, _iNextCtrlTop, 58, 25 + _iNextCtrlTop) ;
  MapDialogRect(cvtRect) ;
  /*TButton* pBtOK =*/ new OWL::TButton(this, IDOK, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;

  cvtRect = NS_CLASSLIB::TRect(230, _iNextCtrlTop, 243, 25 + _iNextCtrlTop) ;
  MapDialogRect(cvtRect) ;
  /*TButton* pBtCn =*/ new OWL::TButton(this, IDCANCEL, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), /*isDefault*/ false, 0) ;
}

void
MissingInformationDialog::InitializeControls()
{
  // Check if all is properly connected
  //
  if ((NSInsidePerson*) NULL == _pPersonInfo)
    return ;

  NSInPersonFieldArray* pFields = getFields() ;
  if (((NSInPersonFieldArray*) NULL == pFields) || pFields->empty())
    return ;

  // Create a control for each mandatory field with missing information
  //
  size_t iFieldsCount = pFields->size() ;
  for (size_t iField = 1 ; iField <= iFieldsCount ; iField++)
  {
    NSInPersonField* pField = _pParent->getInPersonsWindow()->getField(iField) ;
    if (pField && pField->mustAskIfMissing())
    {
      NSInsidePersonInformation* pIPI = _pPersonInfo->getInformation(iField) ;
      if (pIPI && (string("") == pIPI->getRawData()))
      {
        MissingInformationControl* pControl = getControlForField(iField) ;
        if (pControl)
        {
          if (string("DPRAT1") == pField->getPath())
            InitializeUserCombo(pControl) ;
          else if (string("") != pField->getUnit())
            InitializeEdit(pControl, pField) ;
        }
      }
    }
  }
}

void
MissingInformationDialog::InitializeUserCombo(MissingInformationControl* pControl)
{
  if ((MissingInformationControl*) NULL == pControl)
    return ;

  NTComboBox<string>* pCombo = pControl->getCombo() ;
  if ((NTComboBox<string>*) NULL == pCombo)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  // Get users
  //
  NSBasicAttributeArray AttrArray ;

  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LOGIN)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, PIDS)) ;

  AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;

  NSPersonsAttributesArray AttsList ;
  const char* serviceName = (NautilusPilot::SERV_SEARCH_PERSON).c_str() ;
  bool listOk = pSuper->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;

  if ((false == listOk) || AttsList.empty())
    return ;

  int iCurrentUserIndex = 0 ;
  int iIndex            = 0 ;

  for (NSPersonsAttributeIter iterUser = AttsList.begin() ; AttsList.end() != iterUser ; iterUser++)
  {
    string sLogin = (*iterUser)->getAttributeValue(LOGIN) ;
    string sPids  = (*iterUser)->getAttributeValue(PIDS) ;
    string sRole  = (*iterUser)->getAttributeValue(ROLE) ;

    if (sRole.find("U") != NPOS)
    {
      pCombo->AddElement(sLogin, new string(sPids)) ;

      if (pContexte->getUtilisateurID() == sPids)
        iCurrentUserIndex = iIndex ;

      iIndex++ ;
    }
  }

  pCombo->PrintCombo() ;
  pCombo->SetSelIndex(iCurrentUserIndex) ;
}

void
MissingInformationDialog::InitializeEdit(MissingInformationControl* pControl, NSInPersonField* pField)
{
  if (((MissingInformationControl*) NULL == pControl) || ((NSInPersonField*) NULL == pField))
    return ;

  NSUtilEdit* pEdit = pControl->getEdit() ;
  if ((NSUtilEdit*) NULL == pEdit)
    return ;

  string sUnit = pField->getUnit() ;
  string sUnitSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sUnit, &sUnitSens) ;

  if (string("2HE01") == sUnitSens)
  {
    NSUtilEditHeure* pHeure = TYPESAFE_DOWNCAST(pEdit, NSUtilEditHeure) ;
    if ((NSUtilEditHeure*) NULL == pHeure)
      return ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;

    pHeure->setHeure(string(tpsNow.donneHeure(), 0, 4)) ;
  }
  else if (string("2DA01") == sUnitSens)
  {
    NSUtilEditDate* pDate = TYPESAFE_DOWNCAST(pEdit, NSUtilEditDate) ;
    if ((NSUtilEditDate*) NULL == pDate)
      return ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;

    pDate->setDate(tpsNow.donneDate()) ;
  }
  else if (string("2DA02") == sUnitSens)
  {
    NSUtilEditDateHeure* pDateHeure = TYPESAFE_DOWNCAST(pEdit, NSUtilEditDateHeure) ;
    if ((NSUtilEditDateHeure*) NULL == pDateHeure)
      return ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;

    pDateHeure->setDate(tpsNow.donneDateHeure()) ;
  }
}

void
MissingInformationDialog::CmOk()
{
  if (aControls.empty())
    return ;

  NSInPersonFieldArray* pFields = getFields() ;
  if ((NSInPersonFieldArray*) NULL == pFields)
    return ;

  for (MissingInformationControlIter it = aControls.begin() ; aControls.end() != it ; it++)
  {
    int iField = (*it)->getInformationIndex() ;
    NSInPersonField* pField = (*pFields)[iField - 1] ;

    // Get information to be completed
    //
    NSInsidePersonInformation* pInformation = _pPersonInfo->getInformation(iField) ;

    NTComboBox<string>* pCombo = (*it)->getCombo() ;
    if (pCombo)
    {
      if (pCombo->GetSelIndex() >= 0)
      {
        string* pSelected = pCombo->getSelectedElement()->getElement() ;
        pInformation->setRawData(*pSelected) ;
      }
    }
    else
    {
      NSUtilEdit* pEdit = (*it)->getEdit() ;
      if (pEdit)
      {
        string sUnit = pField->getUnit() ;
        string sUnitSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sUnit, &sUnitSens) ;

        if (string("2HE01") == sUnitSens)
        {
          NSUtilEditHeure* pHeure = TYPESAFE_DOWNCAST(pEdit, NSUtilEditHeure) ;
          if (pHeure)
          {
            string sHeure = string("") ;
            pHeure->getHeure(&sHeure) ;

            if (strlen(sHeure.c_str()) == 4)
            {
              string sRawData = pField->getFormat() ;

              string sHour   = string(sHeure, 0, 2) ;
              string sMinute = string(sHeure, 2, 2) ;

              sRawData = replaceAll(sRawData, string("$hh$"), sHour) ;
              sRawData = replaceAll(sRawData, string("$mm$"), sMinute) ;

              pInformation->setRawData(sRawData) ;
            }
          }
        }
        else if (string("2DA01") == sUnitSens)
        {
          NSUtilEditDate* pDate = TYPESAFE_DOWNCAST(pEdit, NSUtilEditDate) ;
          if (pDate)
          {
            string sDate = string("") ;
            pDate->getDate(&sDate) ;

            if (strlen(sDate.c_str()) == 8)
            {
              string sRawData = pField->getFormat() ;

              string sYear  = string(sDate, 0, 4) ;
              string sMonth = string(sDate, 4, 2) ;
              string sDay   = string(sDate, 6, 2) ;

              sRawData = replaceAll(sRawData, string("$AAAA$"), sYear) ;
              sRawData = replaceAll(sRawData, string("$MM$"),   sMonth) ;
              sRawData = replaceAll(sRawData, string("$JJ$"),   sDay) ;

              pInformation->setRawData(sRawData) ;
            }
          }
        }
        else if (string("2DA02") == sUnitSens)
        {
          NSUtilEditDateHeure* pDateHeure = TYPESAFE_DOWNCAST(pEdit, NSUtilEditDateHeure) ;
          if (pDateHeure)
          {
            string sDate = string("") ;
            pDateHeure->getDate(&sDate) ;

            if (strlen(sDate.c_str()) >= 12)
            {
              string sRawData = pField->getFormat() ;

              string sYear   = string(sDate,  0, 4) ;
              string sMonth  = string(sDate,  4, 2) ;
              string sDay    = string(sDate,  6, 2) ;
              string sHour   = string(sDate,  8, 2) ;
              string sMinute = string(sDate, 10, 2) ;

              sRawData = replaceAll(sRawData, string("$AAAA$"), sYear) ;
              sRawData = replaceAll(sRawData, string("$MM$"),   sMonth) ;
              sRawData = replaceAll(sRawData, string("$JJ$"),   sDay) ;
              sRawData = replaceAll(sRawData, string("$hh$"),   sHour) ;
              sRawData = replaceAll(sRawData, string("$mm$"),   sMinute) ;

              pInformation->setRawData(sRawData) ;
            }
          }
        }
      }
    }
  }

  CloseWindow(IDOK) ;
}

void
MissingInformationDialog::CreateUserCombo(MissingInformationControl* pControl)
{
  if ((MissingInformationControl*) NULL == pControl)
    return ;

  NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForControl, _iNextCtrlTop, _iLeftForControl + 200, _iNextCtrlTop + 150) ;
  MapDialogRect(cvtRect) ;
  NTComboBox<string>* pCombo = new NTComboBox<string>((TWindow*) this, _iNextCtrlId++, cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height(), CBS_DROPDOWNLIST, 0) ;
  _iNextCtrlTop += _iCtrlInterval ;

  pControl->setCombo(pCombo) ;
}

void
MissingInformationDialog::CreateEditFromUnit(MissingInformationControl* pControl, string sUnit)
{
  if ((MissingInformationControl*) NULL == pControl)
    return ;

  string sUnitSens = string("") ;
  pContexte->getDico()->donneCodeSens(&sUnit, &sUnitSens) ;

  if (string("2HE01") == sUnitSens)
  {
    NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForControl, _iNextCtrlTop, _iLeftForControl + 50, _iNextCtrlTop + 12) ;
    MapDialogRect(cvtRect) ;
    NSUtilEditHeure* pHeure = new NSUtilEditHeure(pContexte, this, _iNextCtrlId++, cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height()) ;
    pControl->setEdit(pHeure) ;
  }
  else if (string("2DA01") == sUnitSens)
  {
    NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForControl, _iNextCtrlTop, _iLeftForControl + 100, _iNextCtrlTop + 12) ;
    MapDialogRect(cvtRect) ;
    NSUtilEditDate* pDate = new NSUtilEditDate(pContexte, this, _iNextCtrlId++, cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height()) ;
    pControl->setEdit(pDate) ;
  }
  else if (string("2DA02") == sUnitSens)
  {
    NS_CLASSLIB::TRect cvtRect = NS_CLASSLIB::TRect(_iLeftForControl, _iNextCtrlTop, _iLeftForControl + 150, _iNextCtrlTop + 12) ;
    MapDialogRect(cvtRect) ;
    NSUtilEditDateHeure* pDateHeure = new NSUtilEditDateHeure(pContexte, this, _iNextCtrlId++, "", cvtRect.Left(), cvtRect.Top(), cvtRect.Width(), cvtRect.Height()) ;
    pControl->setEdit(pDateHeure) ;
  }

  _iNextCtrlTop += _iCtrlInterval ;
}

MissingInformationControl*
MissingInformationDialog::getControlForField(int iField)
{
  if (aControls.empty())
    return (MissingInformationControl*) 0 ;

  for (MissingInformationControlIter it = aControls.begin() ; aControls.end() != it ; it++)
    if ((*it)->getInformationIndex() == iField)
      return *it ;

  return (MissingInformationControl*) 0 ;
}

NSInPersonFieldArray*
MissingInformationDialog::getFields()
{
  if ((NSInPersonsListWindow*) NULL == _pParent)
    return (NSInPersonFieldArray*) 0 ;

  NSInPersonsWindow* pMainWindow = _pParent->getInPersonsWindow() ;
  if ((NSInPersonsWindow*) NULL == pMainWindow)
    return (NSInPersonFieldArray*) 0 ;

  return pMainWindow->getFields() ;
}

////////////////////////// fin de nsWhoIsThere.cpp /////////////////////////////
