// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>#include <vcl.h>
#pragma hdrstop
#include "nautilus\ActiveX.h"

#include <owl\gauge.h>

#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
// #include "WebService.h"#include "nautilus\nsTankSvce.h"#include "nsbb\nslistwind.h"#include "nautilus\nsepicap.h"#include "nautilus\ns_html.h"#include "nautilus\nscsdoc.h"#include "nautilus\nscsvue.h"#include "nautilus\nsdocview.h"#include "nautilus\nsbrowse.h"// #include "partage\ole_utils.h"#include "nssavoir\nsFileCaptur.h"#include "nsldv\nsldvuti.h"#include "partage\nsdivfile.h"#include "nautilus\psapi.h"#include "nautilus\nsMailBox.rh"
 /****************** classe NSImportWindow **************************/
long NSTankPerson::lObjectCount = 0 ;

const int ID_TankList = 0x100 ;
const int ID_TankTree = 0x101 ;

/*
DEFINE_RESPONSE_TABLE1(NSTankServiceWindow, TPaneSplitter)
   EV_WM_CLOSE,
   EV_WM_SIZE,
END_RESPONSE_TABLE;
*/

NSTankServiceWindow::NSTankServiceWindow(NSContexte* pCtx, TWindow* parent, const char far* title, TModule* module)
                    :TPaneSplitter(parent, title, 0, module),
                     NSRoot(pCtx)
{
	Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;

	_pTankListWindow = new NSTankListWindow(pCtx, this, ID_TankList, 0, 0, 0, 0) ;
  _pXmlTreeWindow  = new NSXmlTreeViewerWindow(pCtx, this, ID_TankTree, 0, 0, 0, 0) ;
}

NSTankServiceWindow::~NSTankServiceWindow()
{
	delete _pTankListWindow ;
  delete _pXmlTreeWindow ;
	// delete pMailViewWindow ;

	// if (pContexte->_pMailBoxWindow == this)
	//	pContexte->_pMailBoxWindow = NULL ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSTankServiceWindow::SetupWindow()
{
  string sCaptionText = pContexte->getSuperviseur()->getText("mailBox", "mailBox") ;
  Parent->SetCaption(sCaptionText.c_str()) ;

  TPaneSplitter::SetupWindow() ;

  //splitDir = psVertical, psHorizontal	or psNone
	SplitPane(_pTankListWindow, _pXmlTreeWindow, psVertical) ;
  SetSplitterWidth(5) ;
}

void
NSTankServiceWindow::EvClose()
{
	TWindow::EvClose() ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
void
NSTankServiceWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TPaneSplitter::EvSize(sizeType, size) ;
  setXmlWindowPos() ;
/*
  TLayoutMetrics metrics ;
  bool bGotMetrics = GetChildLayoutMetrics(*pTankListWindow, metrics);
	ClassLib::TRect listRect(metrics.X, metrics.Y, metrics.X + metrics.Width, metrics.Y + metrics.Width) ;
  pTankListWindow->MoveWindow(listRect, true) ;
*/
}

void
NSTankServiceWindow::setXmlWindowPos()
{
  // return ;

  NS_CLASSLIB::TRect rectDlg = _pXmlTreeWindow->GetWindowRect() ; // coordonnées par % à l'écran
  int X, Y ;
  // int W, H ;
  NS_CLASSLIB::TPoint pointTopLeft(rectDlg.X(), rectDlg.Y()) ;
  ScreenToClient(pointTopLeft) ;
  NS_CLASSLIB::TPoint pointBottomRight(rectDlg.X() + rectDlg.Width(), rectDlg.Y() + rectDlg.Height()) ;
  ScreenToClient(pointBottomRight) ;

  NS_CLASSLIB::TRect localRect(pointTopLeft, pointBottomRight) ;

  _pXmlTreeWindow->SetWindowPos(0, localRect, SWP_NOZORDER) ;
}

/*
void
NSTankServiceWindow::traceMemory()
{
  // Global memory
  //

  // Use to convert bytes to KB
  #define DIV 1024

  // Specify the width of the field in which to print the numbers.
  // The asterisk in the format specifier "%*I64d" takes an integer
  // argument and uses it to pad and right justify the number.
  #define WIDTH 7

  MEMORYSTATUSEX statex ;
  statex.dwLength = sizeof (statex) ;

  GlobalMemoryStatusEx(&statex) ;

  char szTraceInfo[256] ;

  sprintf(szTraceInfo, "There is  %*ld percent of memory in use.",           WIDTH, statex.dwMemoryLoad) ;
  string sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of physical memory.",  WIDTH, statex.ullTotalPhys/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of physical memory.",   WIDTH, statex.ullAvailPhys/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of paging file.",    WIDTH, statex.ullTotalPageFile/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of paging file.",     WIDTH, statex.ullAvailPageFile/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d total Kbytes of virtual memory.", WIDTH, statex.ullTotalVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of virtual memory.",  WIDTH, statex.ullAvailVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Show the amount of extended memory available.
  sprintf(szTraceInfo, "There are %*I64d free Kbytes of extended memory.", WIDTH, statex.ullAvailExtendedVirtual/DIV) ;
  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Process memory
  //

  PROCESS_MEMORY_COUNTERS pmc ;

  DWORD processID = GetCurrentProcessId() ;

  // Print the process identifier.

  sprintf(szTraceInfo, "Process ID: %u", processID ) ;

  sTrace = string(szTraceInfo) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Print information about the memory usage of the process.

  HANDLE hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID ) ;
  if (NULL == hProcess)
  {
    sTrace = string("Can't query process information.") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
  {
    sprintf(szTraceInfo, "PageFaultCount: 0x%08X",             pmc.PageFaultCount) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    // sprintf(szTraceInfo, "PeakWorkingSetSize: 0x%08X",         pmc.PeakWorkingSetSize) ;
    sprintf(szTraceInfo, "Episodus peak use was %u total Kbytes of memory.", pmc.PeakWorkingSetSize/DIV) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    // sprintf(szTraceInfo, "WorkingSetSize: 0x%08X",             pmc.WorkingSetSize) ;
    sprintf(szTraceInfo, "Episodus uses %u total Kbytes of memory.", pmc.WorkingSetSize/DIV) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "QuotaPeakPagedPoolUsage: 0x%08X",    pmc.QuotaPeakPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "QuotaPagedPoolUsage: 0x%08X",        pmc.QuotaPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "QuotaPeakNonPagedPoolUsage: 0x%08X", pmc.QuotaPeakNonPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "QuotaNonPagedPoolUsage: 0x%08X",     pmc.QuotaNonPagedPoolUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "PagefileUsage: 0x%08X",              pmc.PagefileUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
    sprintf(szTraceInfo, "PeakPagefileUsage: 0x%08X",          pmc.PeakPagefileUsage) ;
    sTrace = string(szTraceInfo) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
  }

  CloseHandle(hProcess) ;
}
*/

bool
NSTankServiceWindow::isSelectedPatientEqualToCurrentPatient(const NSTankPerson* pTankObject) const
{
	if ((NSTankPerson*) NULL == pTankObject)
		return false ;

	NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
		return false ;

/*
	string sSelectedName = pPatEnCours->getDisplayName() ;
  string sCurrentName  = pTankObject->getCurrentPatientDisplayName() ;

  pseumaj(&sSelectedName) ;
  pseumaj(&sCurrentName) ;

	return (sSelectedName == sCurrentName) ;
*/
  return false ;
}

void
NSTankServiceWindow::openPatient(NSTankPerson* pTankObject)
{
	NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
  if ((NSEpisodus*) NULL == pEpisodus)
  	return ;

	string sProcessLog = string("") ;

	NSCaptureArray  CaptureArray(pContexte) ;

	NSCaptureArray* pNewCaptureArray = &(pEpisodus->newCaptureArray) ;

	// First, erase the information in the capture array.
  // Elsewhere, if we launch "openPatient", but don't really open it
  // ("Cancel" somewhere), then this information will remain and prevent another
  // "openPatient"
	//
/*
	pNewCaptureArray->eraseEntry("ZADMI1/LIDET1/LNOM01") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LIDET1/LNOM21") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LCOOR1/LADRE1/LADL11") ;
	pNewCaptureArray->eraseEntry("ZADMI1/LCOOR1/LADRE1/LADL21") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LCOOR1/LADRE1/LVILL1/LZIP01") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LCOOR1/LADRE1/LVILL1/LCOMU1") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LIDET1/KNAIS1") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LFRAN1/LFRAB1") ;
  pNewCaptureArray->eraseEntry("ZADMI1/LIDET1/LSEXE1") ;
*/
  pNewCaptureArray->viderForOrigin(FromMailBox) ;

	if (CaptureArray.empty())
	{
  	string sPathString      = string("") ;
  	string sValueString     = string("") ;
  	string sUnitString      = string("") ;
  	string sClassifString   = string("") ;
  	string sInformationDate = string("") ;

		if (pTankObject->getPatientName() != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, string("ZADMI1/LIDET1/LNOM01"), pTankObject->getPatientName(), FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
  	if (pTankObject->getPatientSurname() != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, string("ZADMI1/LIDET1/LNOM21"), pTankObject->getPatientSurname(), FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
  	if (pTankObject->getPatientBirth() != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, string("ZADMI1/LIDET1/KNAIS1"), pTankObject->getPatientBirth(), FromMailBox, sClassifString, "2DA011", sInformationDate)) ;
	}
  else
  {
  	for (CaptureIter captIter = CaptureArray.begin() ; captIter != CaptureArray.end() ; )
    {
    	pNewCaptureArray->push_back(*captIter) ;
      CaptureArray.erase(captIter) ;
    }
  }

	pContexte->captureData(pNewCaptureArray) ;
}

bool
NSTankServiceWindow::importXmlFile(NSTankPerson* pTankObject)
{
  if ((NSTankPerson*) NULL == pTankObject)
    return false ;

  string sFileName = pTankObject->getXmlFileName() ;
  if (string("") == sFileName)
    return false ;

  string sCompleteFileName = pContexte->PathName("TANK") + sFileName ;

  ifstream inFile ;
  inFile.open(sCompleteFileName.c_str()) ;
	if (!inFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sCompleteFileName ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return false ;
	}

  string sFileContent = string("") ;
  inFile >> sFileContent ;

  _pXmlTreeWindow->loadContent(sFileContent) ;

  return true ;
}

bool
NSTankServiceWindow::canProceedWithPatient(NSTankPerson* pTankObject)
{
	if (isSelectedPatientEqualToCurrentPatient(pTankObject))
		return true ;

  string sWarningTitle = pContexte->getSuperviseur()->getText("generalLanguage", "warning") ;
	string sWarningText  = pContexte->getSuperviseur()->getText("mailBox", "selectedPatientIsNotCurrentPatient") ;
  sWarningText += string("\n") + pContexte->getSuperviseur()->getText("mailBox", "clickOkToProceedAnywayOrCancelToStop") ;

	int rep = MessageBox(sWarningText.c_str(), sWarningTitle.c_str(), MB_OKCANCEL) ;
  if ((IDCANCEL == rep) || (IDNO == rep))
  	return false ;

  return true ;
}

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSTankServiceChild, TMDIChild)
	 EV_WM_CLOSE,
	 EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;

NSTankServiceChild::NSTankServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSTankServiceWindow* clientWnd)					         :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSTankServiceChild::~NSTankServiceChild()
{
	if (pContexte->_pMailBoxChild == this)
		pContexte->_pMailBoxChild = NULL ;
}

voidNSTankServiceChild::EvClose()
{
/*
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	int retVal = MessageBox("Voulez-vous vraiment fermer ?", sCaption.c_str(), MB_YESNO) ;

	if (retVal == IDYES)
		TWindow::EvClose() ;
*/
	TWindow::EvClose() ;
}

// -----------------------------------------------------------------------------
// Méthodes de NSTankListWindow
// -----------------------------------------------------------------------------
#define CM_OPEN_PAT     100#define CM_IMPORT_TXT   101#define CM_IMPORT_CS    102#define CM_IMPORT_BOTH  103#define CM_DETAILS_VIEW 104#define CM_DETAILS_HIDE 105#define CM_EXPLODE_MM   106#define CM_DELETE_MM    107#define CM_LOG_ON_OFF   108#define CM_EDIT_MM      109DEFINE_RESPONSE_TABLE1(NSTankListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
  // NS_LVN_GETDISPINFO(DispInfoListe),
  NS_LW_COLUMNCLICK(LVNColumnclick),
	EV_COMMAND(CM_OPEN_PAT,     CmOpenPatient),
END_RESPONSE_TABLE ;

NSTankListWindow::NSTankListWindow(NSContexte* pCtx, NSTankServiceWindow *parent, int id, int x, int y, int w, int h, TModule *module)
                 :TListWindow((TWindow *) parent, id, x, y, w, h, module),
                  NSRoot(pCtx)
{
  _pParentWindow = parent ;
  _iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	_iPreviousSelection = -1 ;
  _bDetailsOn = false ;
  _bLogOn     = false ;

  _pProgressDlg = (NSMsgLoadProgressDlg*) 0 ;
}

NSTankListWindow::~NSTankListWindow()
{
}

void
NSTankListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  TListWindow::SetupWindow() ;

  SetupColumns() ;

  loadPatients() ;

  // AfficheListe() ;

  // selectCurrentPatientFirstMessage() ;
}

void
NSTankListWindow::loadPatients()
{
try
{
  _pProgressDlg = new NSMsgLoadProgressDlg(this, pContexte) ;
  _pProgressDlg->Create() ;
  _pProgressDlg->ShowWindow(SW_SHOW) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTankListWindow::loadPatients : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSTankListWindow::loadPatients.", standardError, 0) ;
  return ;
}

  // First, get files from buffer directory
  //
  string sTrace = string("External tank: get list from tank.csv") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  string sFileName = pContexte->PathName("TANK") + string("tank.csv") ;

  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
	if (!inFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFileName ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return ;
	}

	while (!inFile.eof())
	{
    string sLine = string("") ;

		getline(inFile, sLine) ;

    if (string("") != sLine)
      loadPatient(sLine) ;
	}
	inFile.close() ;

  sTrace = string("Mailbox init done") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // pContexte->getSuperviseur()->traceMemory() ;

  _pProgressDlg->Destroy() ;
  delete _pProgressDlg ;
  _pProgressDlg = (NSMsgLoadProgressDlg*) 0 ;
}

void
NSTankListWindow::loadPatient(const string sLine)
{
  if (string("") == sLine)
    return ;

  string sName    = string("") ;
  string sSurName = string("") ;
  string sBirth   = string("") ;
  string sFile    = string("") ;

  size_t iComaPos = sLine.find(",") ;
  if (NPOS == iComaPos)
    return ;
  sName = string(sLine, 0, iComaPos) ;

  size_t iPrevious = iComaPos ;

  iComaPos = sLine.find(",", iPrevious + 1) ;
  if (NPOS == iComaPos)
    return ;
  sSurName = string(sLine, iPrevious + 1, iComaPos - iPrevious - 1) ;

  iPrevious = iComaPos ;

  iComaPos = sLine.find(",", iPrevious + 1) ;
  if (NPOS == iComaPos)
    return ;
  sBirth = string(sLine, iPrevious + 1, iComaPos - iPrevious - 1) ;
  sFile  = string(sLine, iComaPos + 1, strlen(sLine.c_str()) - iComaPos) ;

  _aPatients.push_back(new NSTankPerson(pContexte, sName, sSurName, sBirth, sFile)) ;
}

void
NSTankListWindow::processSelectionChange()
{
	// Don't do that, because if an entry is removed, iCurrentSelection is -1
	int iCurrentSelection = IndexItemSelect() ;
	if (iCurrentSelection < 0)
		return ;

	// Don't do that, because if an entry is removed, select index doesn't change
	//if (iPreviousSelection == iCurrentSelection)
 	//	return ;

	_iPreviousSelection = iCurrentSelection ;
  _bDetailsOn = false ;

  NSTankPerson* pSelectedPerson = _aPatients[_iPreviousSelection] ;

  // Ask the view to display
	displayMessage() ;
}

void
NSTankListWindow::selectCurrentPatient()
{
	if (_aPatients.empty())
    return ;

  string sTrace = string("Mailbox selectCurrentPatientFirstMessage : Entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	sort(_aPatients.begin(), _aPatients.end(), tankObjSortByNameInf) ;

	_bNaturallySorted = true ;
	_iSortedColumn    = 0 ;

  sTrace = string("Tankbox selectCurrentPatient : filling messages list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  AfficheListe() ;

  sTrace = string("Tankbox selectCurrentPatient : messages list filled") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
  {
    sTrace = string("Tankbox selectCurrentPatient : exit (no patient)") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	return ;
  }

	string sName    = pseumaj(pPatEnCours->getNom()) ;
	string sSurname = pseumaj(pPatEnCours->getPrenom()) ;

  size_t iIndexNumber = 0 ;
  NSTankPersonIter itPat = _aPatients.begin() ;

  while ((_aPatients.end() != itPat) &&
         (pseumaj((*itPat)->getPatientSurname()) < sSurname))
  {
    iIndexNumber++ ;
    itPat++ ;
  }
  while ((_aPatients.end() != itPat) &&
         (pseumaj((*itPat)->getPatientSurname()) == sSurname) &&
         (pseumaj((*itPat)->getPatientName()) < sName))
  {
    iIndexNumber++ ;
    itPat++ ;
  }

  SetItemState(iIndexNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
  EnsureVisible(iIndexNumber, false) ;

  // Invalidate() ;

  processSelectionChange() ;

  sTrace = string("Mailbox selectCurrentPatientFirstMessage : exit") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}

void
NSTankListWindow::SetupColumns()
{
	string sPatientName = pContexte->getSuperviseur()->getText("mailBox", "patient") ;
  string sExamDate    = pContexte->getSuperviseur()->getText("mailBox", "date") ;
  // string sFileDate    = pContexte->getSuperviseur()->getText("mailBox", "fileDate") ;

  InsertColumn(0, TListWindColumn("!", 20, TListWindColumn::Left, 0)) ;
  InsertColumn(1, TListWindColumn((char*)sPatientName.c_str(), 300, TListWindColumn::Left, 1)) ;
  InsertColumn(2, TListWindColumn("H'", 25, TListWindColumn::Left, 2)) ;
  InsertColumn(3, TListWindColumn("E", 20, TListWindColumn::Left, 3)) ;
  InsertColumn(4, TListWindColumn((char*)sExamDate.c_str(), 80, TListWindColumn::Left, 4)) ;
  // InsertColumn(2, TListWindColumn((char*)sFileDate.c_str(), 80, TListWindColumn::Left, 1)) ;
}

// Affichage des éléments de la liste
void
NSTankListWindow::AfficheListe()
{
  string sTrace = string("NSTankListWindow::AfficheListe: Entering.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	DeleteAllItems() ;

  if (_aPatients.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSTankPersonIter itPat = _aPatients.end() ;
  do
  {
    itPat-- ;

    string sPatName = (*itPat)->getPatientName() ;
    TListWindItem Item(sPatName.c_str(), 0) ;

    InsertItem(Item) ;
	}
  while (itPat != _aPatients.begin()) ;

  int i = 0 ;
	for (itPat = _aPatients.begin() ; _aPatients.end() != itPat ; i++, itPat++)
	{
    // Col 1
    //
    string sPatName = (*itPat)->getPatientSurname() ;
    TListWindItem Item1(sPatName.c_str(), 1) ;
    Item1.SetIndex(i) ;
    Item1.SetSubItem(1) ;
    SetItem(Item1) ;
	}

  sTrace = string("NSTankListWindow::AfficheListe: Exiting.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSTankListWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
  const int       BufLen = 255 ;
  static char     buffer[BufLen] ;
  TListWindItem   &dispInfoItem = *(TListWindItem *)&dispInfo.item ;
  // char            szDate[25] ;
  // char            szDateDisplay[25] ;
  // char            szHour[25] ;
  string          sHourDisplay ;
  buffer[0] = '\0' ;

  // int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;

/*
  // Affiche les informations en fonction de la colonne  switch (dispInfoItem.GetSubItem())
  {
    case 1  :
    {
    	string sPatName = (aNewMails[index])->getDisplayName() ;
    	dispInfoItem.SetText(sPatName.c_str()) ;
      break ;
    }

  	case 2  :
    {
    	switch ((aNewMails[index])->iType)
    	{
    		case FILETYPE_HPRIM1 :
    		case FILETYPE_HPRIM1_MULTI :
      		dispInfoItem.SetImageIndex(0) ;
        	break ;
      	case FILETYPE_HPRIM2 :
    		case FILETYPE_HPRIM2_MULTI :
      		dispInfoItem.SetImageIndex(1) ;
        	break ;
      	case FILETYPE_UNKNOWN :
      		dispInfoItem.SetImageIndex(2) ;
        	break ;
    	}
      dispInfoItem.mask = LVIF_IMAGE ;
      break ;
    }

    case 3  :
    {
    	string sProcess = (aNewMails[index])->sProcessLog ;
      if (sProcess != string(""))
      {
    		size_t pos = sProcess.find("Error") ;
      	if (pos != NPOS)
      		dispInfoItem.SetImageIndex(10) ;
      }
      break ;
    }

    case 4  : // date début
    	strcpy(szDate, (aNewMails[index])->sExamDate.c_str()) ;
      donne_date(szDate, szDateDisplay, sLang) ;

      dispInfoItem.SetText(szDateDisplay) ;
      break ;

*/
/*
    case 2  : // date fichier
    {
    	strcpy(szDate, (aNewMails[index])->sReceptionDate.c_str()) ;

      // Get hour
      int i ;
      for (i = 0; i < 6; i++)
      	szHour[i] = szDate[i+8] ;
      szHour[i] = '\0' ;
      donne_heure(szHour, sHourDisplay) ;

      // Get date
      szDate[8] = '\0' ;
      donne_date(szDate, szDateDisplay, sLang) ;

      strcat(szDateDisplay, " ") ;
      strcat(szDateDisplay, sHourDisplay.c_str()) ;

      dispInfoItem.SetText(szDateDisplay) ;
      break ;
    }
*/
//  }
}

void
NSTankListWindow::displayMessage()
{
	if (((NSTankServiceWindow*) NULL == _pParentWindow) || (NULL == _pParentWindow->getXmlViewer()))
  	return ;

	NSTankPerson* pSelectedPerson = _aPatients[_iPreviousSelection] ;
  if ((NSTankPerson*) NULL == pSelectedPerson)
    return ;

  string sFileName = pContexte->PathName("TANK") + pSelectedPerson->getXmlFileName() ;

  NSXmlTreeViewerWindow* pXmlViewer = _pParentWindow->getXmlViewer() ;
  if (pXmlViewer)
    pXmlViewer->loadContent(sFileName) ;
}

void
NSTankListWindow::reinitPatients()
{
	_aPatients.vider() ;
	loadPatients() ;
  sortAgain() ;
  AfficheListe() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSTankListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  processSelectionChange() ;

  //if (info.GetFlags() & LVHT_ONITEM)  //	pView->CmModifier() ;
  //else
  //	pView->CmNouveau() ;
}

void
NSTankListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    processSelectionChange() ;    NSTankPerson* pSelectedMail = _aPatients[indexItem] ;    // création d'un menu popup		NS_CLASSLIB::TPoint lp = point ;

		OWL::TPopupMenu menu ;

		NSSuper* pSuper = pContexte->getSuperviseur() ;

		string sOpen = pSuper->getText("mailBox", "openPatient") ;
    menu.AppendMenu(MF_STRING, CM_OPEN_PAT,    sOpen.c_str()) ;

		ClientToScreen(lp) ;
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
		TListWindow::EvRButtonDown(modkeys, point) ;
  }
  else
		TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSTankListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modkeys, point) ;

	processSelectionChange() ;}
void
NSTankListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
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
NSTankListWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt)
{
  NSSuper     *pSuper     = pContexte->getSuperviseur() ;

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
NSTankListWindow::IndexItemSelect()
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
NSTankListWindow::sortAgain()
{
	switch (_iSortedColumn)
  {
  	case 0 : sortByName() ; break ;
    case 1 : sortByBirthdate() ; break ;
	}
}

void
NSTankListWindow::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
		case 0  :
    	changeNameSortOrder() ;
    	sortByName() ;
      break ;
    case 1  :
    	changeBirthdateSortOrder() ;
    	sortByBirthdate() ;
      break ;
  }
}

void
NSTankListWindow::changeSortOrder(int iCol)
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
NSTankListWindow::sortByName()
{
  if (_aPatients.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aPatients.begin(), _aPatients.end(), tankObjSortByNameInf) ;
  else
    sort(_aPatients.begin(), _aPatients.end(), tankObjSortByNameSup) ;

  AfficheListe() ;
}

void
NSTankListWindow::sortByBirthdate()
{
  if (_aPatients.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aPatients.begin(), _aPatients.end(), tankObjSortByBirthdateSup) ;
  else
    sort(_aPatients.begin(), _aPatients.end(), tankObjSortByBirthdateInf) ;

  AfficheListe() ;
}

void
NSTankListWindow::CmOpenPatient()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSTankPerson* pSelectedPerson = _aPatients[iCurrentSelection] ;

	_pParentWindow->openPatient(pSelectedPerson) ;
}

void
NSTankListWindow::EvSetFocus(HWND hWndLostFocus)
{
	SetBkColor(0x00fff0f0) ; // 0x00bbggrr
  SetTextBkColor(0x00fff0f0) ;
  Invalidate();

  // pView->focusFct() ;

  int count = GetItemCount() ;

  for (int i = 0 ; i < count ; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    	return ;

	SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}

void
NSTankListWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate() ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSXmlTreeViewerWindow//
// -----------------------------------------------------------------------------
NSXmlTreeViewerWindow::NSXmlTreeViewerWindow(NSContexte* pCtx, NSTankServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module)
                  :TEditFile(parent, id, "", 0, 0, 0, 0), NSRoot(pCtx)
{
	_pParentWindow = parent ;
}

NSXmlTreeViewerWindow::~NSXmlTreeViewerWindow()
{
}

void
NSXmlTreeViewerWindow::loadContent(string sFileCompleteName)
{
	if (string("") == sFileCompleteName)
		return ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSTankPerson//
// -----------------------------------------------------------------------------
bool
tankObjSortByNameInf(const NSTankPerson *pObj1, const NSTankPerson *pObj2)
{
  if (((NSTankPerson*) NULL == pObj1) || ((NSTankPerson*) NULL == pObj2))
    return false ;

	return (pObj1->getPatientName() < pObj2->getPatientName()) ;
}

bool
tankObjSortByNameSup(const NSTankPerson *pObj1, const NSTankPerson *pObj2)
{
  if (((NSTankPerson*) NULL == pObj1) || ((NSTankPerson*) NULL == pObj2))
    return false ;

	return (pObj1->getPatientName() > pObj2->getPatientName()) ;
}

bool
tankObjSortByBirthdateInf(const NSTankPerson *pObj1, const NSTankPerson *pObj2)
{
  if (((NSTankPerson*) NULL == pObj1) || ((NSTankPerson*) NULL == pObj2))
    return false ;

	return (pObj1->getPatientBirth() < pObj2->getPatientBirth()) ;
}

bool
tankObjSortByBirthdateSup(const NSTankPerson *pObj1, const NSTankPerson *pObj2)
{
  if (((NSTankPerson*) NULL == pObj1) || ((NSTankPerson*) NULL == pObj2))
    return false ;

	return (pObj1->getPatientBirth() > pObj2->getPatientBirth()) ;
}

NSTankPerson::NSTankPerson(NSContexte* pCtx)
{
  lObjectCount++ ;

	_pContexte       = pCtx ;

	_sPatientName    = string("") ;
	_sPatientSurname = string("") ;
	_sPatientBirth   = string("") ;
	_sXmlFileName    = string("") ;
}

NSTankPerson::NSTankPerson(NSContexte* pCtx, const string sName,
                                             const string sSurname,
                                             const string sBirth,
                                             const string sFileName)
{
  lObjectCount++ ;

	_pContexte       = pCtx ;

	_sPatientName    = sName ;
	_sPatientSurname = sSurname ;
	_sPatientBirth   = sBirth ;
	_sXmlFileName    = sFileName ;
}

NSTankPerson::~NSTankPerson()
{
  lObjectCount-- ;
}

NSTankPerson::NSTankPerson(const NSTankPerson& rv)
{
  lObjectCount++ ;

	_pContexte       = rv._pContexte ;

	_sPatientName    = rv._sPatientName ;
	_sPatientSurname = rv._sPatientSurname ;
	_sPatientBirth   = rv._sPatientBirth ;
  _sXmlFileName    = rv._sXmlFileName ;
}

NSTankPerson&
NSTankPerson::operator=(const NSTankPerson& rv)
{
  if (&rv == this)
		return *this ;

  _pContexte       = rv._pContexte ;

	_sPatientName    = rv._sPatientName ;
	_sPatientSurname = rv._sPatientSurname ;
	_sPatientBirth   = rv._sPatientBirth ;
	_sXmlFileName    = rv._sXmlFileName ;

  return *this ;
}

//***********************************************************************//
//							Classe NSEnregProgressDlg
//***********************************************************************//
DEFINE_RESPONSE_TABLE1(NSMsgLoadProgressDlg, NSUtilDialog)
  EV_COMMAND(STOP_BTN, stopPressed),
END_RESPONSE_TABLE;

NSMsgLoadProgressDlg::NSMsgLoadProgressDlg(TWindow* parent, NSContexte* pCtx,
															TModule* /* module */)
                     :NSUtilDialog(parent, pCtx, "LOAD_MAILS")
{
	// pNewProgressBar = new TGauge(this, IDC_BAR_NEW) ;
  pNewProgressBar = new TGauge(this, "%d%%", IDC_BAR_NEW, 217, 12, 190, 32) ;
  pNewProgressBar->SetRange(0, 100) ;
  pNewProgressBar->SetNativeUse(nuNever) ;
  pNewProgressBar->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue

	// pOldProgressBar = new TGauge(this, IDC_BAR_OLD) ;
  pOldProgressBar = new TGauge(this, "%d%%", IDC_BAR_OLD, 217, 68, 190, 32) ;
  pOldProgressBar->SetRange(0, 100) ;
  pOldProgressBar->SetNativeUse(nuNever) ;
  pOldProgressBar->SetColor(NS_CLASSLIB::TColor(0, 0, 128)) ;   // Blue

  pNewText = new OWL::TStatic(this, IDC_TXT_NEW) ;
  pOldText = new OWL::TStatic(this, IDC_TXT_OLD) ;

  pStopButton = new OWL::TButton(this, STOP_BTN) ;

  bKeepLoading = true ;
}

NSMsgLoadProgressDlg::~NSMsgLoadProgressDlg(){
	delete pNewProgressBar ;
	delete pOldProgressBar ;
  delete pNewText ;
  delete pOldText ;
  delete pStopButton ;
}

voidNSMsgLoadProgressDlg::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  resetNewGauge() ;  resetOldGauge() ;

  string sTitleText = pContexte->getSuperviseur()->getText("mailBoxProgress", "loadingMessages") ;
  SetCaption(sTitleText.c_str()) ;

  sTitleText = pContexte->getSuperviseur()->getText("mailBoxProgress", "loadingNewMessages") ;
  pNewText->SetText(sTitleText.c_str()) ;
  sTitleText = pContexte->getSuperviseur()->getText("mailBoxProgress", "loadingExistingMessages") ;
  pOldText->SetText(sTitleText.c_str()) ;

  string sHalt = pContexte->getSuperviseur()->getText("mailBoxProgress", "halt") ;
  pStopButton->SetCaption(sHalt.c_str()) ;
}

void
NSMsgLoadProgressDlg::stopPressed()
{
  bKeepLoading = false ;
}

// -----------------------------------------------------------------------------
//
//  Méthodes de PreEnregTankDocDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(PreEnregTankDocDialog, TDialog)
  EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------// Function     : PreEnregTankDocDialog::PreEnregTankDocDialog()
// Description  : Constructeur
// -----------------------------------------------------------------------------
PreEnregTankDocDialog::PreEnregTankDocDialog(TWindow *pere, NSTankPerson *pMailObj, NSContexte *pCtx)
                   :NSUtilDialog(pere, pCtx, "ID_MSGDOCPARAMS")
{
try
{
	_pMailObject  	= pMailObj ;

  _pNomDoc        = new OWL::TEdit(this, IDC_NOM, 1024) ;
  _pType          = new NSUtilLexique(pContexte, this, IDC_TYPE, pContexte->getDico()) ;
  _pDateCreation  = new NSUtilEditDateHeure(pContexte, this,  IDC_CREATION) ;

  _pHistorique    = new OWL::TRadioButton(this, IDC_HISTOR) ;}
catch (...){
  erreur("Exception PreEnregTankDocDialog ctor.", standardError, 0) ;
}}


// -----------------------------------------------------------------------------
// Function     : PreEnregTankDocDialog::~PreEnregTankDocDialog()
// Description  : Destructeur, enregistre le document
// -----------------------------------------------------------------------------
PreEnregTankDocDialog::~PreEnregTankDocDialog()
{
  // Suppression de tous les objets
  delete _pNomDoc ;
  delete _pType ;
  delete _pDateCreation ;
  delete _pHistorique ;
}

// -----------------------------------------------------------------------------
// Function     : PreEnregTankDocDialog::SetupWindow()
// Arguments    :	Aucun
// Description  : Initialise la boite de dialogue
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregTankDocDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

/*
	// Mise du nom du document dans le contrôle Edit
	if (strspn(_pMailObject->sExamName.c_str(), " ") < strlen(_pMailObject->sExamName.c_str()))
		_pNomDoc->SetText(_pMailObject->sExamName.c_str()) ;
	else
		_pNomDoc->SetText("") ;

  // Type
  if (string("") != _pMailObject->sExamCode)
    _pType->setLabel(_pMailObject->sExamCode) ;

	// Date de création
	_pDateCreation->setDate(_pMailObject->sExamDate) ;

	// Ajout à l'historique
  if (true == _pMailObject->bExamVisible)
  	_pHistorique->Check() ;
  else
  	_pHistorique->Uncheck() ;
*/
}

// -----------------------------------------------------------------------------
// Function     : PreEnregTankDocDialog::CmOk()
// Description  : Réponse au bouton OK
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregTankDocDialog::CmOk()
{
/*
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (_pType->getDico()->getDicoDialog()->EstOuvert())
    _pType->getDico()->getDicoDialog()->InsererElementLexique() ;

	char NomDocu[1025] ;

	// Vérifie qu'un nom de document a bien été saisi
	_pNomDoc->Transfer(NomDocu, tdGetData) ;
	if ((NomDocu[0] == '\0') || (strspn(NomDocu, " ") == strlen(NomDocu)))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "youMustProvideANameForTheDocument") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return ;
	}

  // Type
  //
  // Don't accept free text
  if (_pType->getCode() == string("£?????"))
  {
    string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
    erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
    _pType->SetFocus() ;
    return ;
  }
  _pMailObject->sExamCode = _pType->getCode() ;

	// Transfère le nom dans DocumentInfo
	_pMailObject->sExamName = NomDocu ;

  // Transfère la date de création dans DocumentInfo
  string sDateCreation ;
  _pDateCreation->getDate(&sDateCreation) ;
  _pMailObject->sExamDate = sDateCreation ;
*/
	// Effectue le CmOk
	TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
// Function     : PreEnregTankDocDialog::CmCancel()
// Description  : Réponse au bouton Cancel
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregTankDocDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

////////////////////////// fin de nsMailSvce.cpp ///////////////////////////////
