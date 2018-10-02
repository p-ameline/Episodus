// NSMailSvce.CPP : Incoming messages processing
////////////////////////////////////////////////////////////
#include <cstring.h>
#include <stdio.h>#include <assert.h>#include <vcl.h>
#pragma hdrstop
#include "nautilus\ActiveX.h"

#include <owl\gauge.h>

#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
// #include "WebService.h"#include "nautilus\nsMailSvce.h"#include "nsbb\nslistwind.h"#include "nautilus\nsepicap.h"#include "nautilus\ns_html.h"#include "nautilus\nscsdoc.h"#include "nautilus\nscsvue.h"#include "nautilus\nsdocview.h"#include "nautilus\nsbrowse.h"// #include "partage\ole_utils.h"#include "nssavoir\nsFileCaptur.h"#include "nsldv\nsldvuti.h"#include "partage\nsdivfile.h"#include "nautilus\psapi.h"#include "nautilus\nsMailBox.rh"
 /****************** classe NSImportWindow **************************/
long NSNewMailObject::lObjectCount = 0 ;

const int ID_MailList = 0x100 ;
const int ID_MailText = 0x101 ;
const int ID_MailHtml = 0x102 ;

/*
DEFINE_RESPONSE_TABLE1(NSMailServiceWindow, TPaneSplitter)
   EV_WM_CLOSE,
   EV_WM_SIZE,
END_RESPONSE_TABLE;
*/

NSMailServiceWindow::NSMailServiceWindow(NSContexte* pCtx, TWindow* parent, const char far* title, TModule* module)
                    :TPaneSplitter(parent, title, 0, module),
                     NSRoot(pCtx)
{
	Attr.Style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;

	pMailListWindow = new NSMailListWindow(pCtx, this, ID_MailList, 0, 0, 0, 0) ;
  pMailTextWindow = new NSTxtViewerWindow(pCtx, this, ID_MailText, 0, 0, 0, 0) ;
  pMailHtmlWindow = NULL ;
  pCurrentViewer = pMailTextWindow ;

	pMailViewWindow = NULL ;
}

NSMailServiceWindow::~NSMailServiceWindow()
{
	delete pMailListWindow ;
  delete pMailTextWindow ;
  if (pMailHtmlWindow)
    delete pMailHtmlWindow ;
	// delete pMailViewWindow ;

	if (pContexte->_pMailBoxWindow == this)
		pContexte->_pMailBoxWindow = NULL ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSMailServiceWindow::SetupWindow()
{
  string sCaptionText = pContexte->getSuperviseur()->getText("mailBox", "mailBox") ;
  Parent->SetCaption(sCaptionText.c_str()) ;

  TPaneSplitter::SetupWindow() ;

  //splitDir = psVertical, psHorizontal	or psNone
	SplitPane(pMailListWindow, pMailTextWindow, psVertical) ;
  SetSplitterWidth(5) ;

  pMailHtmlWindow = new NSHtmlViewerWindow(pContexte, this, ID_MailHtml, 0, 0, 0, 0) ;
  pMailHtmlWindow->Create() ;
  setHtmlWindowPos() ;
}

void
NSMailServiceWindow::EvClose()
{
	TWindow::EvClose() ;
}

// fonction permettant de prendre toute la taille de TWindow par la TListWindow
void
NSMailServiceWindow::EvSize(uint sizeType, ClassLib::TSize& size)
{
  TPaneSplitter::EvSize(sizeType, size) ;
  setHtmlWindowPos() ;
/*
  TLayoutMetrics metrics ;
  bool bGotMetrics = GetChildLayoutMetrics(*pMailListWindow, metrics);
	ClassLib::TRect listRect(metrics.X, metrics.Y, metrics.X + metrics.Width, metrics.Y + metrics.Width) ;
  pMailListWindow->MoveWindow(listRect, true) ;
*/
}

void
NSMailServiceWindow::setHtmlWindowPos()
{
  // return ;

  NS_CLASSLIB::TRect rectDlg = pMailTextWindow->GetWindowRect() ; // coordonnées par % à l'écran
  int X, Y ;
  // int W, H ;
  NS_CLASSLIB::TPoint pointTopLeft(rectDlg.X(), rectDlg.Y()) ;
  ScreenToClient(pointTopLeft) ;
  NS_CLASSLIB::TPoint pointBottomRight(rectDlg.X() + rectDlg.Width(), rectDlg.Y() + rectDlg.Height()) ;
  ScreenToClient(pointBottomRight) ;

  NS_CLASSLIB::TRect localRect(pointTopLeft, pointBottomRight) ;

  pMailHtmlWindow->SetWindowPos(0, localRect, SWP_NOZORDER) ;
}

/*
void
NSMailServiceWindow::traceMemory()
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

void
NSMailServiceWindow::openTextViewer()
{
  if ((pCurrentViewer == pMailTextWindow) || (NULL == pMailHtmlWindow))
    return ;

  pMailHtmlWindow->ShowWindow(SW_HIDE) ;
  pMailTextWindow->ShowWindow(SW_SHOW) ;

  bool bReplaced = ReplacePane(pMailHtmlWindow, pMailTextWindow, TShouldDelete::NoDelete) ;
  if (true == bReplaced)
    pCurrentViewer = pMailTextWindow ;
}

void
NSMailServiceWindow::openHtmlViewer()
{
  if (pCurrentViewer == pMailHtmlWindow)
    return ;

  pMailTextWindow->ShowWindow(SW_HIDE) ;
  pMailHtmlWindow->ShowWindow(SW_SHOW) ;

  bool bReplaced = ReplacePane(pMailTextWindow, pMailHtmlWindow, TShouldDelete::NoDelete) ;
  if (true == bReplaced)
    pCurrentViewer = pMailHtmlWindow ;
}

#define SPLITTER(x) TYPESAFE_DOWNCAST(x,OWL::TSplitter)
#define LAYOUTWINDOW(x) TYPESAFE_DOWNCAST(x,OWL::TLayoutWindow)

bool
NSMailServiceWindow::ReplacePane(TWindow* target, TWindow* newPane,
                           TPaneSplitter::TDelete dt)
{
/*
  TSplitter* split1 = TYPESAFE_DOWNCAST(target->Parent, TSplitter) ;
  // OWL::TSplitter* split1 = SPLITTER(target->Parent) ;
  if (NULL == split1)
    return false ;

  OWL::TSplitter* split2 = SPLITTER(newPane->Parent) ;
  if (NULL != split2)
    return false ;
*/
  // if (!HasPane(target) || HasPane(newPane)) // 'newPane' should not exist.
  //  return false;

  // if (target->CanClose()) {     // 'newPane' takes on target's layout metrics.
    TLayoutMetrics  lmOfTarget;
    TLayoutMetrics  lmOfOther;
    TLayoutWindow*  splitter = LAYOUTWINDOW(target->Parent);
    TWindow*        pane1 = splitter->GetFirstChild();
    TWindow*        pane2 = 0;

    if (SPLITTER(splitter)) {
      pane1 = SPLITTER(splitter)->Pane1();
      pane2 = SPLITTER(splitter)->Pane2();
    }
    splitter->GetChildLayoutMetrics(*target, lmOfTarget);

    if (pane1 == target && pane2) {
      splitter->GetChildLayoutMetrics(*pane2, lmOfOther);
      if (lmOfOther.X.RelWin == target)
        lmOfOther.X.RelWin = newPane;
      else
        lmOfOther.Y.RelWin = newPane;
      splitter->SetChildLayoutMetrics(*pane2, lmOfOther);
    }
    newPane->SetParent(splitter);
    // newPane->Create();
    splitter->SetChildLayoutMetrics(*newPane, lmOfTarget);
    splitter->RemoveChildLayoutMetrics(*target);
    //DestroyPane(target, dt);
    target->SetParent(this);
    splitter->Layout();
    return true;
  // }
  // return false;
}

bool
NSMailServiceWindow::isSelectedPatientEqualToCurrentPatient(NSNewMailObject* pMailObject)
{
	if ((NSNewMailObject*) NULL == pMailObject)
		return false ;

	NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
		return false ;

	string sSelectedName = pMailObject->getDisplayName() ;
  string sCurrentName  = pMailObject->getCurrentPatientDisplayName() ;

  pseumaj(&sSelectedName) ;
  pseumaj(&sCurrentName) ;

	return (sSelectedName == sCurrentName) ;
}

void
NSMailServiceWindow::openPatient(NSNewMailObject* pMailObject)
{
	NSEpisodus* pEpisodus = pContexte->getEpisodus() ;
  if ((NSEpisodus*) NULL == pEpisodus)
  	return ;

	string sProcessLog = string("") ;

	NSCaptureArray  CaptureArray(pContexte) ;

  if ((FILETYPE_EPISODUS_3 != pMailObject->iType) && (FILETYPE_EPISODUS_4 != pMailObject->iType))
  {
    NSCaptureFromHPRIM HprimHunter(pContexte) ;
	  HprimHunter.setFileName(pMailListWindow->sDirectory + pMailObject->sFileName) ;
	  /* ImportedFileType iType = */ HprimHunter.importHPRIMheader(&CaptureArray, &sProcessLog) ;
  }

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

		if (pMailObject->sPatientSurname != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", pMailObject->sPatientSurname, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
  	if (pMailObject->sPatientName != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", pMailObject->sPatientName, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
  	if (pMailObject->sPatientBirth != string(""))
			pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", pMailObject->sPatientBirth, FromMailBox, sClassifString, "2DA011", sInformationDate)) ;
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
NSMailServiceWindow::importTxtDocument(NSNewMailObject* pMailObject)
{
  if ((NSNewMailObject*) NULL == pMailObject)
    return false ;

	NSRefDocument NewDoc((TDocument*) 0, pContexte) ;
  NewDoc.setReadOnly(false) ;

  bool bSuccess = false ;

  if ((pMailObject->iType == FILETYPE_EPISODUS_4) || (pMailObject->iType == FILETYPE_EPISODUS_3))
    bSuccess = NewDoc.ImporterHtml(pMailObject->sFileName) ;
  else
  {
    NSCaptureFromHPRIM HprimHunter(pContexte) ;
    HprimHunter.setFileName(pMailListWindow->sDirectory + pMailObject->sFileName) ;
    string sMessageBody = HprimHunter.getHPRIMBody() ;

    string sDocTitle = string("") ;

    if (string("") != sMessageBody)
    {
      string sTempFileName = nomSansDoublons(pMailListWindow->sDirectory, string("tempo"), string("txt")) ;
      if (string("") != sTempFileName)
      {
        string sFullTempFileName = pMailListWindow->sDirectory + sTempFileName ;

        ofstream outFile ;
        outFile.open(sFullTempFileName.c_str()) ;
        if (!outFile)        {
		      string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFullTempFileName ;
		      erreur(sErrorText.c_str(), standardError, 0) ;
          return false ;
	      }
        outFile << sMessageBody ;

        outFile.close() ;

        bSuccess = NewDoc.ImporterFichier("ZTTXT", sDocTitle, sTempFileName, pMailListWindow->sDirectory) ;

        NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFullTempFileName) ;
      }
    }
    else
      bSuccess = NewDoc.ImporterFichier("ZTTXT", sDocTitle, pMailObject->sFileName, pMailListWindow->sDirectory) ;
  }

  if (false == bSuccess)
  {
		string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
	}

  return bSuccess ;
}

bool
NSMailServiceWindow::importParsedDocument(NSNewMailObject* pMailObject)
{
  if ((NSNewMailObject*) NULL == pMailObject)
    return false ;

  string sFileName = pMailListWindow->sDirectory + pMailObject->sFileName ;

  if ((FILETYPE_EPISODUS_4 == pMailObject->iType) ||
      (FILETYPE_EPISODUS_3 == pMailObject->iType))
  {
    NSRefDocument NewDoc((TDocument*) 0, pContexte) ;
    NewDoc.setReadOnly(false) ;

    bool bSuccess = NewDoc.ImporterHtml(sFileName) ;

    if (false == bSuccess)
    {
		  string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
		  erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
	  }

    return bSuccess ;
  }

	NSCaptureArray  CaptureArray(pContexte) ;
  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

  NSCaptureFromHPRIM HprimHunter(pContexte) ;

	HprimHunter.setFileName(sFileName) ;

  string sProcessLog = string("") ;

  int iAlertLevel = 0 ;
  bool bHeaderValidity ;
  string sRootExam = string("") ;
  bool bSuccess = HprimHunter.importHPRIM2(&CaptureArray, &PPT, &iAlertLevel, &sProcessLog, &bHeaderValidity, sRootExam) ;

  if (false == bSuccess)
  {
  	string sProcessLog = string("") ;

  	CaptureArray.vider() ;
    PPT.vider() ;
		bSuccess = HprimHunter.importHPRIM1(&CaptureArray, &PPT, &sProcessLog, &bHeaderValidity, sRootExam) ;
  }

	if (false == bSuccess)
		return false ;

	NSCSDocument* pCSDoc = new NSCSDocument(0, pContexte, "") ;

  NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
  pCSDoc->initFromPatPatho(&PatPatho) ;

  if (string("") == sRootExam)
    sRootExam = string("GBIOL1") ;
  PatPatho.ajoutePatho(sRootExam, 0) ;

  PatPatho.InserePatPathoFille(PatPatho.begin(), &PPT) ;
  pCSDoc->SetDirty(true) ;
  pCSDoc->setPatPatho(&PatPatho) ;

  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pCSDoc, "CS Format") ;

  // Set the view as "Dirty"
  //
  TView* pView = pCSDoc->GetViewList() ;
  if ((TView*) NULL == pView)
		return false ;

	bool bViewFound = false ;

	do
	{
		NSCsVue* pCsView = TYPESAFE_DOWNCAST(pView, NSCsVue) ;
		if (pCsView)
    {
			pCsView->SetDirty(true) ;
      bViewFound = true ;
    }

		pView = pCSDoc->NextView(pView) ;
	}
  while (pView) ;

	return bViewFound ;
}

bool
NSMailServiceWindow::importBothDocuments(NSNewMailObject* pMailObject)
{
/*
	bool bTxtImp  = importTxtDocument(pMailObject) ;
  bool bTreeImp = importParsedDocument(pMailObject) ;

  return bTxtImp & bTreeImp ;
*/

	string sLang = string("") ;
  if (pContexte)
  	sLang = pContexte->getUserLanguage() ;

  // Must find a way to discover it in the text
  //
	string sRootCode = string("GBIOL1") ;
  string sDocTitle = string("") ;
  pContexte->getDico()->donneLibelle(sLang, &sRootCode, &sDocTitle) ;

	//
  // Import txt document
  //

	NSRefDocument NewDocTxt(0, pContexte) ;
  NewDocTxt.setReadOnly(false) ;

	if (false == NewDocTxt.ImporterFichier("ZTTXT", sDocTitle, pMailObject->sFileName, pMailListWindow->sDirectory, false, false))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
	}

  //
  // Import tree document
  //
  NSCaptureArray  CaptureArray(pContexte) ;
  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  string sFileName = pMailListWindow->sDirectory + pMailObject->sFileName ;

  NSCaptureFromHPRIM HprimHunter(pContexte) ;
	HprimHunter.setFileName(sFileName) ;

  string sProcessLog = string("") ;
  int iAlertLevel = 0 ;
  bool bHeaderValidity ;
  string sRootExam = string("") ;
  bool bSuccess = HprimHunter.importHPRIM2(&CaptureArray, &PPT, &iAlertLevel, &sProcessLog, &bHeaderValidity, sRootExam) ;
  if (false == bSuccess)
  {
  	string sProcessLog = string("") ;

  	CaptureArray.vider() ;
    PPT.vider() ;
		bSuccess = HprimHunter.importHPRIM1(&CaptureArray, &PPT, &sProcessLog, &bHeaderValidity, sRootExam) ;
  }

	if (false == bSuccess)
		return false ;

  NSPatPathoArray PPt(pContexte->getSuperviseur()) ;
  PPt.ajoutePatho(sRootCode, 0) ;
  PPt.InserePatPathoFille(PPt.begin(), &PPT) ;

  bool bImport = NewDocTxt.ReferencerDocumentPatPatho(&NewDocTxt, &PPt, string(""), string("ZCS00")) ;

/*
  NSRefDocument NewDocTree(0, pContexte) ;
  NewDocTree.setReadOnly(false) ;

  NSPatPathoArray* pPatPatho = NewDocTree.pPatPathoArray ;
  pPatPatho->ajoutePatho(sRootCode, 0) ;
  pPatPatho->InserePatPathoFille(pPatPatho->begin(), &PPT) ;

	string sLibelleDoc = string("") ;
  pContexte->getDico()->donneLibelle(sLang, &sRootCode, &sLibelleDoc) ;
  if (string("") != sLibelleDoc)
  	sLibelleDoc[0] = pseumaj(sLibelleDoc[0]) ;

  if (false == NewDocTree.Referencer("ZCS00", sLibelleDoc,
                                    string(""), string(""),  // file and path
                                    true, true,
                                    string(""),              // doc ID
                                    NSRootLink::personDocument, 0,
                                    string("_User_"),
                                    string(""), string(""),  // tpl and heading
                                    string(""), string(""),  // Dest and content
                                    NewDocTxt.sCodeDocMeta,  // Master document
                                    NSRootLink::semantizedDocument))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
		erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
	}

  // On enregistre la patpatho
	bool bSavePpt = NewDocTree.enregistrePatPatho() ;
	if (false == bSavePpt)
		return false ;

	// Si c'est un nouveau compte rendu on vérifie s'il remet à zéro un objectif
	if (pContexte->getPatient()->pDocLdv)
		pContexte->getPatient()->pDocLdv->showNewTree(pPatPatho, NewDocTxt.GetDateDoc(false)) ;

	// On prévient l'historique (fait à part pour les CS et les CN)
	pContexte->getPatient()->pDocHis->Rafraichir(NewDocTree.DonneDocInfo(), pPatPatho, &NewDocTree) ;
*/

  return bImport ;
}

bool
NSMailServiceWindow::explodeMultipleMessages(NSNewMailObject* pMailObject)
{
	string sFileName = pMailListWindow->sDirectory + pMailObject->sFileName ;

  NSCaptureFromHPRIM HprimExploder(pContexte) ;

	HprimExploder.setFileName(sFileName) ;

  return HprimExploder.explodeMultipleMessagesFile() ;
}

bool
NSMailServiceWindow::canProceedWithPatient(NSNewMailObject* pMailObject)
{
	if (isSelectedPatientEqualToCurrentPatient(pMailObject))
		return true ;

  string sWarningTitle = pContexte->getSuperviseur()->getText("generalLanguage", "warning") ;
	string sWarningText  = pContexte->getSuperviseur()->getText("mailBox", "selectedPatientIsNotCurrentPatient") ;
  sWarningText += string("\n") + pContexte->getSuperviseur()->getText("mailBox", "clickOkToProceedAnywayOrCancelToStop") ;

	int rep = MessageBox(sWarningText.c_str(), sWarningTitle.c_str(), MB_OKCANCEL) ;
  if ((IDCANCEL == rep) || (IDNO == rep))
  	return false ;

  return true ;
}

// *************************************************************************
//  NSMailViewWindow
// *************************************************************************

/*

NSMailViewWindow::NSMailViewWindow(NSContexte* pCtx, NSMailServiceWindow *parent)
                 :NSRoot(pCtx)
{
	pParentWindow = parent ;
}

// Fonction SetupWindow
////////////////////////////////////////////////////////////////
void
NSMailViewWindow::SetupWindow()
{
	TWindow::SetupWindow() ;

	ModifyStyle(WS_BORDER, WS_CHILD) ;
	Form->Show() ;
	MakeVisible() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSMailViewWindow::PerformCreate(int menuOrId)
{
	CoInitialize(NULL) ;
	Form = new TWebServiceForm(Parent->GetHandle(), (TWindow*) this) ;
	Form->Visible = false ;
	Form->ParentWindow = Parent->HWindow ;
	SetHandle(Form->Handle) ;
	::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers l'URL du service
	Navigate(sUrl) ;
}

bool
NSMailViewWindow::IsTabKeyMessage(MSG &msg)
{
	if (GetCapture() == NULL)
	{
		if (msg.message == WM_KEYDOWN || msg.message == WM_KEYUP)
    {
    	if (msg.wParam == VK_TAB)
      {
      	SendMessage(CN_BASE + msg.message, msg.wParam, msg.lParam) ;
        return true ;
      }
    }
  }

	return false ;
}

//// Let the form process keystrokes in its own way.  Without
// this method, you can't tab between control on the form.
//
bool
NSMailViewWindow::PreProcessMsg(MSG &msg)
{
	bool result = IsTabKeyMessage(msg) ;
	if (!result)
		result = TWindow::PreProcessMsg(msg) ;

	return result ;
}

// Fonction MakeVisiblevoid
NSMailViewWindow::MakeVisible()
{
  Form->Visible = true ;
}

// Fonction Navigate
////////////////////////////////////////////////////////////////
void
NSMailViewWindow::Navigate(string sStartingURL)
{
	if (sStartingURL == string(""))
		return ;

	wchar_t buff[1024] ;
	Variant Flags(navNoReadFromCache) ;
	TVariant VFlags = Flags.operator TVariant() ;

	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sStartingURL.c_str(), -1, buff, sizeof(buff)) ;
	Form->Control->Navigate(buff, &VFlags) ;
}
*/

/****************** classe NSImportChild **************************/
DEFINE_RESPONSE_TABLE1(NSMailServiceChild, TMDIChild)
	 EV_WM_CLOSE,
	 EV_COMMAND(CM_FILECLOSE, EvClose),
END_RESPONSE_TABLE;


NSMailServiceChild::NSMailServiceChild(NSContexte* pCtx, TMDIClient& parent, const char far* title, NSMailServiceWindow* clientWnd)					         :TMDIChild(parent, title, clientWnd), NSRoot(pCtx)
{
	pClient = clientWnd ;
}

NSMailServiceChild::~NSMailServiceChild()
{
	if (pContexte->_pMailBoxChild == this)
		pContexte->_pMailBoxChild = NULL ;
}

voidNSMailServiceChild::EvClose()
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
//
// Méthodes de NSDrugsPropertyWindow//
// -----------------------------------------------------------------------------#define CM_OPEN_PAT     100#define CM_IMPORT_TXT   101#define CM_IMPORT_CS    102#define CM_IMPORT_BOTH  103#define CM_DETAILS_VIEW 104#define CM_DETAILS_HIDE 105#define CM_EXPLODE_MM   106#define CM_DELETE_MM    107#define CM_LOG_ON_OFF   108#define CM_EDIT_MM      109DEFINE_RESPONSE_TABLE1(NSMailListWindow, TListWindow)  EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_WM_LBUTTONDOWN,                
  EV_WM_KEYDOWN,
  EV_WM_KILLFOCUS,
  EV_WM_SETFOCUS,
  EV_WM_LBUTTONUP,
  // NS_LVN_GETDISPINFO(DispInfoListe),
  NS_LW_COLUMNCLICK(LVNColumnclick),
	EV_COMMAND(CM_OPEN_PAT,     CmOpenPatient),
  EV_COMMAND(CM_IMPORT_TXT,   CmImportTxtFile),
	EV_COMMAND(CM_IMPORT_CS,    CmImportTreeFile),
  // EV_COMMAND(CM_IMPORT_BOTH,  CmImportBothFiles),
	EV_COMMAND(CM_DETAILS_VIEW, CmSetDetailsMode),
  EV_COMMAND(CM_DETAILS_HIDE, CmSetBodyMode),
  EV_COMMAND(CM_EXPLODE_MM,   CmExplodeMultipleMessage),
  EV_COMMAND(CM_DELETE_MM,    CmDeleteMessage),
  EV_COMMAND(CM_EDIT_MM,      CmSetDocParams),
  EV_COMMAND(CM_LOG_ON_OFF,   CmSetLogMode),
END_RESPONSE_TABLE ;

NSMailListWindow::NSMailListWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule *module)
                 :TListWindow((TWindow *) parent, id, x, y, w, h, module),
                  NSRoot(pCtx)
{
  pParentWindow = parent ;
  iRes          = id ;
  Attr.Style    |= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_SINGLESEL ;
//  Attr.ExStyle  |= WS_EX_NOPARENTNOTIFY ;

	iPreviousSelection = -1 ;
  bDetailsOn = false ;
  bLogOn     = false ;

  sBufferDirectory   = pContexte->PathName("FGLO") + string("newbox\\") ;
	sDirectory         = pContexte->PathName("FGLO") + string("inbox\\") ;
  sArchivesDirectory = pContexte->PathName("FGLO") + string("oldbox\\") ;

  pProgressDlg = (NSMsgLoadProgressDlg*) 0 ;
}

NSMailListWindow::~NSMailListWindow()
{
	if (pImages)
		delete pImages ;
}

void
NSMailListWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES) ;
  TListWindow::SetupWindow() ;

	HINSTANCE hInstModule = *GetApplication() ;

	pImages = new OWL::TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;
	pImages->Add(OWL::TBitmap(hInstModule, HPRIM1)) ;
	pImages->Add(OWL::TBitmap(hInstModule, HPRIM2)) ;
	pImages->Add(OWL::TBitmap(hInstModule, UNKNOWN)) ;
  pImages->Add(OWL::TBitmap(hInstModule, MAILNEW)) ;
	pImages->Add(OWL::TBitmap(hInstModule, MAILOLD)) ;
	pImages->Add(OWL::TBitmap(hInstModule, MAILREAD)) ;
  pImages->Add(OWL::TBitmap(hInstModule, ALERTL1)) ;
	pImages->Add(OWL::TBitmap(hInstModule, ALERTL2)) ;
	pImages->Add(OWL::TBitmap(hInstModule, ALERTHI)) ;
  pImages->Add(OWL::TBitmap(hInstModule, PARSE_ERROR)) ;
	SetImageList(*pImages, TListWindow::State) ;

  SetupColumns() ;

  loadMessages() ;

  // AfficheListe() ;

  selectCurrentPatientFirstMessage() ;
}

void
NSMailListWindow::loadMessages()
{
try
{
  pProgressDlg = new NSMsgLoadProgressDlg(this, pContexte) ;
  pProgressDlg->Create() ;
  pProgressDlg->ShowWindow(SW_SHOW) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSMailListWindow::loadMessages : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSMailListWindow::loadMessages.", standardError, 0) ;
  return ;
}

  // First, get files from buffer directory
  //
  string sTrace = string("Mailbox: get new txt messages") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  getNewMessagesFromBufferDirectory(string("txt")) ;

  sTrace = string("Mailbox: get new html messages") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  getNewMessagesFromBufferDirectory(string("htm")) ;

	// We need to restart file process if we find a multi-messages file
  //
  bMustRestartFileProcess = false ;

  sTrace = string("Mailbox: init txt message list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // pContexte->getSuperviseur()->traceMemory() ;

  initMailList() ;
  while (bMustRestartFileProcess)
	{
    sTrace = string("Mailbox: init txt message restart") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  	aNewMails.vider() ;
		initMailList() ;
	}

  sTrace = string("Mailbox: init html message list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // pContexte->getSuperviseur()->traceMemory() ;

  initMailList("htm") ;

  sTrace = string("Mailbox init done") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // pContexte->getSuperviseur()->traceMemory() ;

  pProgressDlg->Destroy() ;
  delete pProgressDlg ;
  pProgressDlg = (NSMsgLoadProgressDlg*) 0 ;
}

void
NSMailListWindow::processSelectionChange()
{
	// Don't do that, because if an entry is removed, iCurrentSelection is -1
	int iCurrentSelection = IndexItemSelect() ;
	if (iCurrentSelection < 0)
		return ;

	// Don't do that, because if an entry is removed, select index doesn't change
	//if (iPreviousSelection == iCurrentSelection)
 	//	return ;

	iPreviousSelection = iCurrentSelection ;
  bDetailsOn = false ;

  NSNewMailObject* pSelectedMail = aNewMails[iPreviousSelection] ;
  string sFileName = sDirectory + pSelectedMail->sFileName ;

	if ((pSelectedMail->iType == FILETYPE_HPRIM1) || (pSelectedMail->iType == FILETYPE_HPRIM2))
		bDetailsOn = false ;
  else
  	bDetailsOn = true ;

  // Ask the view to display
	displayMessage() ;

/*
	// on construit un html temporaire dans le répertoire FPER
	string sPathHtml = pContexte->PathName("FPER") ;

	NSHtmlTXT html(toImporter, 0, sFileName.c_str(), pContexte) ;

	// on trouve le nom du fichier temporaire à visualiser
  string sNomSeul = string("mailBox") ;
	string sHtml = html.nomSansDoublons(sPathHtml, sNomSeul, "htm") ;
	sHtml = sPathHtml + sHtml ;

	// on passe la template des documents TXT
	string sTemplateHtml = pContexte->PathName("NTPL") + string("doctxt.htm") ;

  string sTypeNautilus = string("ZTTXT") ;
	// generation du fichier html (dans le repertoire FPER)
	if (!html.genereHtml(sTemplateHtml, sHtml, sDirectory, pSelectedMail->sFileName, sTypeNautilus))
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "errorCreatingImportHtmlFile") ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return ;
	}

	// Ask the view to navigate
	if (NULL == pParentWindow->getViewer())
  	return ;

	pParentWindow->getViewer()->Navigate(sHtml) ;
*/
}

void
NSMailListWindow::selectCurrentPatientFirstMessage()
{
	if (aNewMails.empty())
    return ;

  string sTrace = string("Mailbox selectCurrentPatientFirstMessage : Entering") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	sort(aNewMails.begin(), aNewMails.end(), mailObjSortByNameInf) ;

	bNaturallySorted = true ;
	iSortedColumn    = 0 ;

  sTrace = string("Mailbox selectCurrentPatientFirstMessage : filling messages list") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  AfficheListe() ;

  sTrace = string("Mailbox selectCurrentPatientFirstMessage : messages list filled") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if ((NSPatientChoisi*) NULL == pPatEnCours)
  {
    sTrace = string("Mailbox selectCurrentPatientFirstMessage : exit (no patient)") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	return ;
  }

	string sSurname = pseumaj(pPatEnCours->getNom()) ;
	string sName    = pseumaj(pPatEnCours->getPrenom()) ;

  size_t iIndexNumber = 0 ;
  NSNewMailObjectIter itMail = aNewMails.begin() ;

  // Skip multiple messages
  //
  while ((aNewMails.end() != itMail) &&
              (((*itMail)->iType == FILETYPE_HPRIM1_MULTI) ||
               ((*itMail)->iType == FILETYPE_HPRIM2_MULTI)))
  {
    iIndexNumber++ ;
    itMail++ ;
  }
  while ((aNewMails.end() != itMail) &&
         (pseumaj((*itMail)->sPatientSurname) < sSurname))
  {
    iIndexNumber++ ;
    itMail++ ;
  }
  while ((aNewMails.end() != itMail) &&
         (pseumaj((*itMail)->sPatientSurname) == sSurname) &&
         (pseumaj((*itMail)->sPatientName) < sName))
  {
    iIndexNumber++ ;
    itMail++ ;
  }

  SetItemState(iIndexNumber, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;
  EnsureVisible(iIndexNumber, false) ;

  // Invalidate() ;

  processSelectionChange() ;

  sTrace = string("Mailbox selectCurrentPatientFirstMessage : exit") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}

void
NSMailListWindow::SetupColumns()
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
NSMailListWindow::AfficheListe()
{
  string sTrace = string("NSMailListWindow::AfficheListe: Entering.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	DeleteAllItems() ;

  if (aNewMails.empty())
    return ;

  // Attention : insert insère au dessus ; il faut inscrire les derniers en premier
  NSNewMailObjectIter itMail = aNewMails.end() ;
  do
  {
    itMail-- ;

    string sPatName = (*itMail)->getDisplayName() ;
    TListWindItem Item(sPatName.c_str(), 0) ;

    // Col 0
    //
    switch ((*itMail)->iAlertLevel)
    {
    	case NSNewMailObject::mailAlertLevel1 :
      	Item.SetStateImage(6) ;
        break ;
      case NSNewMailObject::mailAlertLevel2 :
      	Item.SetStateImage(7) ;
        break ;
      case NSNewMailObject::mailDangerAlert :
      	Item.SetStateImage(8) ;
        break ;
    }
    InsertItem(Item) ;
	}
  while (itMail != aNewMails.begin()) ;

  int i = 0 ;
	for (itMail = aNewMails.begin() ; aNewMails.end() != itMail ; i++, itMail++)
	{
    // Col 1
    //
    string sPatName = (*itMail)->getDisplayName() ;
    TListWindItem Item1(sPatName.c_str(), 1) ;
    Item1.SetIndex(i) ;
    Item1.SetSubItem(1) ;
    SetItem(Item1) ;

    // Col 2
    //
    TListWindItem Item2("", 2) ;

    char szType[3] ;
    szType[0] = '\0' ;

    switch ((*itMail)->iType)
    {
    	case FILETYPE_HPRIM1 :
      case FILETYPE_HPRIM1_MULTI :
      	Item2.SetImageIndex(0) ;
        strcpy(szType, "1") ;
        break ;
      case FILETYPE_HPRIM2 :
      case FILETYPE_HPRIM2_MULTI :
      	Item2.SetImageIndex(1) ;
        strcpy(szType, "2") ;
        break ;
      case FILETYPE_EPISODUS_4 :
      case FILETYPE_EPISODUS_3 :
        Item2.SetImageIndex(2) ;
        strcpy(szType, "E") ;
        break ;
      case FILETYPE_UNKNOWN :
      	Item2.SetImageIndex(2) ;
        strcpy(szType, "?") ;
        break ;
    }

		Item2.SetText(szType) ;
    Item2.mask = LVIF_IMAGE | LVIF_TEXT ;
    Item2.SetIndex(i) ;
    Item2.SetSubItem(2) ;
    SetItem(Item2) ;

    // Col 3
    //
    TListWindItem Item3("", 3) ;
    szType[0] = '\0' ;

    size_t iErrorLevel = (*itMail)->getProcessErrorLevel() ;
    if (iErrorLevel > 0)
    {
    	Item3.SetImageIndex(9) ;
      strcpy(szType, "~") ;
    }

    Item3.SetText(szType) ;
    Item3.mask = LVIF_IMAGE | LVIF_TEXT ;
    Item3.SetIndex(i) ;
    Item3.SetSubItem(3) ;
    SetItem(Item3) ;

    // Col 4
    //
    string sLang = pContexte->getUserLanguage() ;

		string sDateDisplay = string("") ;
    if ((*itMail)->sExamDate != string(""))
    	sDateDisplay = donne_date((*itMail)->sExamDate, sLang) ;

    TListWindItem Item4(sDateDisplay.c_str(), 4) ;

    Item4.SetIndex(i) ;
    Item4.SetSubItem(4) ;
    SetItem(Item4) ;
	}

  sTrace = string("NSMailListWindow::AfficheListe: Exiting.") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSMailListWindow::DispInfoListe(TLwDispInfoNotify& dispInfo)
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

// Copy into inbox all Txt messages found inside the buffer
//
void
NSMailListWindow::getNewMessagesFromBufferDirectory(string sDesiredExtension)
{
try
{
  string sTrace = string("Mailbox getNewMessagesFromBufferDirectory : entering for extension: ") + sDesiredExtension ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  if (string("") == sDesiredExtension)
    return ;

  HCURSOR hPrevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;

	//
	// Préparation d'une ammorce générique du type *.Rsp
	//
  size_t iExtensionLength = strlen(sDesiredExtension.c_str()) ;
  string sJokerSeparator = string("*.") ;
  size_t iJokerLength = strlen(sJokerSeparator.c_str()) ;

	//
	// Préparation du nom de fichier complet
	//
  size_t iPathLength = strlen(sBufferDirectory.c_str()) + iJokerLength + iExtensionLength + 1 ;
	char* fichRecherche = new char[iPathLength] ;
  memset(fichRecherche, 0, sizeof(iPathLength)) ;

	strcpy(fichRecherche, sBufferDirectory.c_str()) ;
	strcat(fichRecherche, sJokerSeparator.c_str()) ;
  strcat(fichRecherche, sDesiredExtension.c_str()) ;

  size_t iTotalFileNb = countFilesInDirectory(fichRecherche) ;
  if (0 == iTotalFileNb)
  {
    delete[] fichRecherche ;
    return ;
  }
  size_t iCurrentFileNb = 0 ;

	//
	// Lancement de la recherche du premier fichier correspondant
	//                  FindFirstFile
  WIN32_FIND_DATA stRecherche ;
	HANDLE			    hFichierTrouve ;

	hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche) ;
	delete[] fichRecherche ;

	//
	// Si le fichier n'existe pas on sort
	//
	if (hFichierTrouve == INVALID_HANDLE_VALUE)
	{    ::SetCursor(hPrevCursor) ;		return ;
	}

  string sUpperExtension = sDesiredExtension ;
  for (size_t i = 0; i < strlen(sUpperExtension.c_str()); i++)
    sUpperExtension[i] = pseumaj(sUpperExtension[i]) ;

  pProgressDlg->resetNewGauge() ;

	//
	// Boucle tant qu'on trouve des fichiers .txt
	//
	bool bTrouve = true ;
	while (bTrouve)
	{
		string sFileName = string(stRecherche.cFileName) ;

    string sNomSeul   = string("") ;
		string sExtension = string("") ;

		size_t pos1 = sFileName.find_last_of(".") ;
		if (NPOS == pos1)
			sNomSeul = sFileName ;
		else
		{
  		sNomSeul   = string(sFileName, 0, pos1) ;
    	sExtension = string(sFileName, pos1+1, strlen(sFileName.c_str())-pos1-1) ;
		}

    for (size_t i = 0 ; i < strlen(sExtension.c_str()) ; i++)
			sExtension[i] = pseumaj(sExtension[i]) ;

		if (sExtension == sUpperExtension)
    {
    	sFileName = sBufferDirectory + sFileName ;
      string sInboxFileName = sDirectory + nomSansDoublons(sDirectory, sNomSeul, sExtension) ;
			::MoveFile(sFileName.c_str(), sInboxFileName.c_str()) ;
    }

    iCurrentFileNb++ ;
    if (pProgressDlg)
    {
      char szProgresLevel[50] ;
      sprintf(szProgresLevel, "%d/%d", iCurrentFileNb, iTotalFileNb) ;
      pProgressDlg->fixePositionNew(100 * iCurrentFileNb / iTotalFileNb) ;
      pProgressDlg->pNewProgressBar->SetCaption(szProgresLevel) ;
      pProgressDlg->pNewProgressBar->Invalidate(false) ;
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
    }

    if (false == pProgressDlg->keepLoading())
      break ;

		bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;
	}
	//
	// On renvoie le numéro de référence
	//
	FindClose(hFichierTrouve) ;

  sTrace = string("Mailbox getNewMessagesFromBufferDirectory : leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  ::SetCursor(hPrevCursor) ;
	return ;
}
catch (...)
{
	erreur("Exception NSMailListWindow::getNewMessagesFromBufferDirectory.", standardError, 0) ;
  return ;
}
}

size_t
NSMailListWindow::countFilesInDirectory(char* szSearchString)
{
try
{
  string sTrace = string("Mailbox countFilesInDirectory : entering for path: ") + string(szSearchString) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  if ((NULL == szSearchString) || ('\0' == szSearchString[0]))
  {
    sTrace = string("Mailbox countFilesInDirectory : empty search string... leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return 0 ;
  }

  size_t iCount = 0 ;

  WIN32_FIND_DATA stRecherche ;
  HANDLE hFichierTrouve = FindFirstFile(szSearchString, &stRecherche) ;

	if (INVALID_HANDLE_VALUE == hFichierTrouve)
  {
    sTrace = string("Mailbox countFilesInDirectory : No file found returning ") + IntToString(iCount) ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
		return iCount ;  }  //	// Boucle tant qu'on trouve des fichiers .txt
	//
	bool bTrouve = true ;
	while (bTrouve)
	{    iCount++ ;    bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;  }  FindClose(hFichierTrouve) ;  sTrace = string("Mailbox countFilesInDirectory : returning ") + IntToString(iCount) ;  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;  return iCount ;}catch (...)
{
	erreur("Exception NSMailListWindow::countFilesInDirectory.", standardError, 0) ;
  return 0 ;
}}

void
NSMailListWindow::initMailList(string sDesiredExtension)
{
try
{
  string sTrace = string("Mailbox initMailList : entering for extension: ") + sDesiredExtension ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

	bMustRestartFileProcess = false ;

	string          sFichierRsp ;
	string          sFichierLog ;

  HCURSOR hPrevCursor = ::SetCursor(::LoadCursor(NULL, IDC_WAIT)) ;

	//
	// Préparation d'une ammorce générique du type *.Rsp
	//
  size_t iExtensionLength = strlen(sDesiredExtension.c_str()) ;

  string sJokerSeparator = string("*.") ;
  size_t iJokerLength = strlen(sJokerSeparator.c_str()) ;
	//
	// Préparation du nom de fichier complet
	//
  size_t iPathLength = strlen(sDirectory.c_str()) + iJokerLength + iExtensionLength + 1 ;
	char* fichRecherche = new char[iPathLength] ;
  memset(fichRecherche, 0, sizeof(iPathLength)) ;

	strcpy(fichRecherche, sDirectory.c_str()) ;
  strcat(fichRecherche, sJokerSeparator.c_str()) ;
  strcat(fichRecherche, sDesiredExtension.c_str()) ;

  size_t iTotalFileNb = countFilesInDirectory(fichRecherche) ;
  if (0 == iTotalFileNb)
  {
    delete[] fichRecherche ;
    return ;
  }
  size_t iCurrentFileNb = 0 ;

	//
	// Lancement de la recherche du premier fichier correspondant
	//                  FindFirstFile
  WIN32_FIND_DATA stRecherche ;
  HANDLE			    hFichierTrouve ;

	hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche) ;
	delete[] fichRecherche ;
	//
	// Si le fichier n'existe pas on renvoie le numéro de référence
	//

	if (hFichierTrouve == INVALID_HANDLE_VALUE)	{  	string sErreur = pContexte->getSuperviseur()->getText("mailBox", "emptyDirectory") ;		sErreur += string(" (") + sDesiredExtension + string(") ") + sDirectory ;
		// erreur(sErreur.c_str(), standardError, 0) ;
    pContexte->getSuperviseur()->trace(&sErreur, 1) ;
    ::SetCursor(hPrevCursor) ;
		return ;
	}

  string sUpperExtension = sDesiredExtension ;
  for (size_t i = 0; i < strlen(sUpperExtension.c_str()); i++)
    sUpperExtension[i] = pseumaj(sUpperExtension[i]) ;
  sUpperExtension = string(".") + sUpperExtension ;

  pProgressDlg->resetOldGauge() ;

	//
	// Boucle tant qu'on trouve des fichiers .txt
	//
	bool bTrouve = true ;
	while (bTrouve && !bMustRestartFileProcess)
	{
		string sFichierRsp = string(stRecherche.cFileName);

		string sSuffixe = string(sFichierRsp, strlen(sFichierRsp.c_str()) - 4, 4) ;
		for (size_t i = 0; i < strlen(sSuffixe.c_str()); i++)
			sSuffixe[i] = pseumaj(sSuffixe[i]) ;

    if (sSuffixe == sUpperExtension)
    {
      string sPrefix = string(sFichierRsp, 0, strlen(sFichierRsp.c_str()) - 4) ;
      initMailFile(hFichierTrouve, sPrefix, sSuffixe) ;
    }

		/*stRecherche.cFileName = sFichierLog.c_str() ;
		/*noemie sur stRecherche.cFileName */

    iCurrentFileNb++ ;
    if (pProgressDlg)
    {
      char szProgresLevel[50] ;
      sprintf(szProgresLevel, "%d/%d", iCurrentFileNb, iTotalFileNb) ;
      pProgressDlg->fixePositionOld(100 * iCurrentFileNb / iTotalFileNb) ;
      pProgressDlg->pOldProgressBar->SetCaption(szProgresLevel) ;
      pProgressDlg->pOldProgressBar->Invalidate(false) ;
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
    }

    if (false == pProgressDlg->keepLoading())
      break ;

		bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;
	}
	//
	// On renvoie le numéro de référence
	//
	FindClose(hFichierTrouve) ;

  sTrace = string("Mailbox initMailList : leaving") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  ::SetCursor(hPrevCursor) ;
	return ;
}
catch (...)
{
	erreur("Exception NSMailListWindow::initMailList.", standardError, 0) ;
  return ;
}
}

void
NSMailListWindow::initMailFile(HANDLE hFile, string sPrefix, string sSuffix)
{
try
{
	if (string("") == sPrefix)
		return ;

  string sTrace = string("Processing file ") + sPrefix + sSuffix ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

	NSNewMailObject* pMailObj = new NSNewMailObject(pContexte) ;

  pMailObj->sFileName = sPrefix ;
  if (sSuffix != string(""))
  	pMailObj->sFileName += sSuffix ;

	// Get file time
  //
  HANDLE hOpenFile = CreateFile(pMailObj->sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL) ;
  if (INVALID_HANDLE_VALUE != hOpenFile)
	{
  	FILETIME creationTime ;
  	FILETIME accessTime ;
  	FILETIME writeTime ;

		/* BOOL bResult = */ ::GetFileTime(hFile, &creationTime, &accessTime, &writeTime) ;
  	SYSTEMTIME stUTC, stLocal ;
  	::FileTimeToSystemTime(&creationTime, &stUTC) ;
  	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

  	NVLdVTemps tpsFileTime ;
  	tpsFileTime.initFromSystemTime(&stLocal) ;

  	pMailObj->sReceptionDate = tpsFileTime.donneDateHeure() ;
  }

  // Look for a log file
  //
	DWORD   nBufferLength = 254 ;	// size, in characters, of buffer
	char    szResult[255] ;	      // address of buffer for found filename
	LPTSTR  lpFilePart ;          // address of pointer to file component

	DWORD iLength = ::SearchPath(sDirectory.c_str(), sPrefix.c_str(), ".wrk",
                                        nBufferLength, szResult, &lpFilePart) ;

	pMailObj->sWorkflowFileName = sPrefix + string(".wrk") ;

	//
	// Le fichier .wrk a été trouvé
	//
	if (iLength > 0)
  {
    sTrace = string("Reading workflow file ") + pMailObj->sWorkflowFileName ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  	bool bParsed = analizeWorkflowFile(pMailObj) ;
    if (bParsed)
    {
      sTrace = string("Workflow file processed") ;
      pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  		return ;
    }
	}

  sTrace = string("Analizing new file ") + pMailObj->sFileName ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  if (string(".TXT") == sSuffix)
	  analizeMailFile(pMailObj) ;
  if (string(".HTM") == sSuffix)
	  analizeHtmlFile(pMailObj) ;

  sTrace = string("New file analized ") + pMailObj->sFileName ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSMailListWindow::initMailFile.", standardError, 0) ;
  return ;
}
}

bool
NSMailListWindow::analizeMailFile(NSNewMailObject* pMailObj, bool bHeaderOnly)
{
	if ((NSNewMailObject*) NULL == pMailObj)
		return false ;

	NSCaptureArray  CaptureArray(pContexte) ;

  NSCaptureFromHPRIM HprimHunter(pContexte) ;
	HprimHunter.setFileName(sDirectory + pMailObj->sFileName) ;

  // We have to parse header first, in order to detect multi-messages files
  //
  string sHprimLog = string("") ;
  pMailObj->iType = HprimHunter.importHPRIMheader(&CaptureArray, &(pMailObj->sProcessLog)) ;

  if (FILETYPE_UNKNOWN == pMailObj->iType)
	{
  	pMailObj->initForUnknownPerson() ;
    aNewMails.push_back(pMailObj) ;

    // Create workflow file
		//
		pMailObj->createWorkflowFile(sDirectory + pMailObj->sWorkflowFileName) ;

		return true ;
	}

	if ((FILETYPE_HPRIM1_MULTI == pMailObj->iType) ||
      (FILETYPE_HPRIM2_MULTI == pMailObj->iType))
  {
  	// bool bSuccess = pParentWindow->explodeMultipleMessages(pMailObj) ;

    string sFileName = sDirectory + pMailObj->sFileName ;
  	NSCaptureFromHPRIM HprimExploder(pContexte) ;
		HprimExploder.setFileName(sFileName) ;
  	bool bSuccess = HprimExploder.explodeMultipleMessagesFile() ;
		if (bSuccess)
			sendToArchives(pMailObj) ;
    //
    // It is of utmost importance to create a workflow file or we enter a endless loop
    //
    else
    {
    	pMailObj->initForUnknownPerson() ;
    	aNewMails.push_back(pMailObj) ;

    	// Create workflow file
			//
			pMailObj->createWorkflowFile(sDirectory + pMailObj->sWorkflowFileName) ;

			return true ;
    }

  	bMustRestartFileProcess = true ;
  	return true ;
  }

	if (false == bHeaderOnly)
	{
		NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

  	bool bSuccess = false ;

    bool bHeaderValidity ;

    if (FILETYPE_HPRIM2 == pMailObj->iType)
    {
    	int iAlertLevel = 0 ;
      string sRootExam = string("") ;
    	bSuccess = HprimHunter.importHPRIM2(&CaptureArray, &(pMailObj->clinicalInformation), &iAlertLevel, &(pMailObj->sProcessLog), &bHeaderValidity, sRootExam) ;

      if (bSuccess && (iAlertLevel > 0))
      {
      	if (iAlertLevel == 1)
        	pMailObj->iAlertLevel = NSNewMailObject::mailAlertLevel1 ;
        if (iAlertLevel == 2)
        	pMailObj->iAlertLevel = NSNewMailObject::mailAlertLevel2 ;
      }
    }

  	if (false == bSuccess)
    {
      string sRootExam = string("") ;
			bSuccess = HprimHunter.importHPRIM1(&CaptureArray, &(pMailObj->clinicalInformation), &(pMailObj->sProcessLog), &bHeaderValidity, sRootExam) ;
    }

		if (false == bSuccess)
			return false ;
	}

	pMailObj->initFromCapture(&CaptureArray) ;
	aNewMails.push_back(pMailObj) ;

  // Check if the message contains a dangerously abnormal information
  //
  checkClinicalAlert(pMailObj) ;

	// Create workflow file
  //
	pMailObj->createWorkflowFile(sDirectory + pMailObj->sWorkflowFileName) ;

  if (pMailObj->iAlertLevel == NSNewMailObject::mailDangerAlert)
	{
  	string sDateBreve ;
    string sDateBreveIntro ;

    string sLang = pContexte->getUserLanguage() ;

  	donne_date_breve(pMailObj->sExamDate, &sDateBreve, &sDateBreveIntro, sLang) ;

  	string sAlertMsg = pMailObj->sPatientName + string(" ") +
                       pMailObj->sPatientSurname + string("\n") +
                       sDateBreveIntro + sDateBreve + string(" : ") +
                       pMailObj->sAlertMessage ;
    MessageBox(sAlertMsg.c_str(), "ALERTE", MB_OK) ;
  }

  return true ;
}

bool
NSMailListWindow::analizeHtmlFile(NSNewMailObject* pMailObj, bool bHeaderOnly)
{
	if ((NSNewMailObject*) NULL == pMailObj)
  {
    string sError = string("Entering HTML file analizis with null object... leaving") ;
    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;

		return false ;
  }

  string sFichierSource = sDirectory + pMailObj->sFileName ;

  ifstream inFile ;
  inFile.open(sFichierSource.c_str()) ;
	if (!inFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFichierSource ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return false ;
	}

  string line = string("") ;
  string scr  = string("") ;

	while (!inFile.eof())
	{
		getline(inFile, line) ;
    if (string("") != line)
    	scr += line + "\n" ;
	}
	inFile.close() ;

  size_t iStartPos = scr.find("{PatPatho") ;
  if (NPOS == iStartPos)
  {
    string sTrace = string("HTML file analizis... cannot find tag PatPatho... leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return false ;
  }

  size_t iEndPos = scr.find("-->", iStartPos) ;
  if (NPOS == iEndPos)
  {
    string sTrace = string("HTML file analizis... cannot find --> tag... leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return false ;
  }

  string sStructuredString = string(scr, iStartPos, iEndPos-iStartPos) ;

  NSCaptureArray CaptureArray(pContexte) ;

  analizeHtmlHeader(&CaptureArray, &sStructuredString, &(pMailObj->iType)) ;

  NSPatPathoArray* pPpt = &(pMailObj->clinicalInformation) ;

  string sPatientID = string("") ;
  if (pContexte->getPatient())
    sPatientID = pContexte->getPatient()->getNss() ;

  pPpt->initialiseDepuisChaine(pContexte, &sStructuredString, sPatientID) ;

  pMailObj->initFromCapture(&CaptureArray) ;

  // Looking for document's date
  //
	if ((false == pPpt->empty()) && (string("") == pMailObj->sExamDate))
    pMailObj->sExamDate = pPpt->GetDocDate() ;

	aNewMails.push_back(pMailObj) ;

  // Check if the message contains a dangerously abnormal information
  //
  checkClinicalAlert(pMailObj) ;

	// Create workflow file
  //
	pMailObj->createWorkflowFile(sDirectory + pMailObj->sWorkflowFileName) ;

  if (pMailObj->iAlertLevel == NSNewMailObject::mailDangerAlert)
	{
  	string sDateBreve ;
    string sDateBreveIntro ;

    string sLang = pContexte->getUserLanguage() ;

  	donne_date_breve(pMailObj->sExamDate, &sDateBreve, &sDateBreveIntro, sLang) ;

  	string sAlertMsg = pMailObj->sPatientName + string(" ") +
                       pMailObj->sPatientSurname + string("\n") +
                       sDateBreveIntro + sDateBreve + string(" : ") +
                       pMailObj->sAlertMessage ;
    MessageBox(sAlertMsg.c_str(), "ALERTE", MB_OK) ;
  }

  return true ;
}

void
NSMailListWindow::analizeHtmlHeader(NSCaptureArray* pCaptureArray, string* pParamStr, ImportedFileType* piType)
{
  if (((string*) NULL == pParamStr) || ((NSCaptureArray*) NULL == pCaptureArray) || ((ImportedFileType*) NULL == piType))
    return ;

  size_t iStartPos = pParamStr->find("{PatPatho") ;
  if (NPOS == iStartPos)
    return ;

  size_t i = iStartPos + 9 ;

  // numéro de version
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] == ' '))
    i++ ;

  // numéro de version
  string sVersion = string("") ;
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] != '|'))
    sVersion += (*pParamStr)[i++] ;
  i++ ;

  if      (string("CN04") == string(sVersion, 0, 4))
    *piType =  FILETYPE_EPISODUS_4 ;
  else if (string("CN03") == string(sVersion, 0, 4))
    *piType =  FILETYPE_EPISODUS_3 ;

  string sUnitString      = string("") ;
  string sClassifString   = string("") ;
  string sInformationDate = string("") ;

  // Name
  //
  string sEntry = string("") ;
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] != '|'))
    sEntry += (*pParamStr)[i++] ;  i++ ;

  if (string("") != sEntry)
    pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", sEntry, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;

  // Surname
  //
  sEntry = string("") ;
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] != '|'))
    sEntry += (*pParamStr)[i++] ;
  i++ ;

  if (string("") != sEntry)
    pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", sEntry, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;

  // Birthdate
  //
  sEntry = string("") ;
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] != '|'))
    sEntry += (*pParamStr)[i++] ;
  i++ ;

  if (string("") != sEntry)
    pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", sEntry, FromMailBox, sClassifString, sUnitString, sInformationDate)) ;

  // Gender
  //
  sEntry = string("") ;
  while ((i < strlen(pParamStr->c_str())) && ((*pParamStr)[i] != '}'))
    sEntry += (*pParamStr)[i++] ;

  if (string("") != sEntry)
  {
    if      (sEntry[0] == '1')
      pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HMASC2", FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
    else if (sEntry[0] == '2')
      pCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LSEXE1", "HFEMI2", FromMailBox, sClassifString, sUnitString, sInformationDate)) ;
  }
}

bool
NSMailListWindow::analizeWorkflowFile(NSNewMailObject* pMailObj)
{
	if (pMailObj->initFromWorkflowFile(sDirectory + pMailObj->sWorkflowFileName))
	{
		aNewMails.push_back(pMailObj) ;
    return true ;
	}
  return false ;
}

void
NSMailListWindow::checkClinicalAlert(NSNewMailObject* pMailObj)
{
	if ((NULL == pMailObj) || (pMailObj->clinicalInformation.empty()))
		return ;

	string sValeur = "" ;
	double dINR = 0 ;

	pMailObj->clinicalInformation.CheminDansPatpatho(0, "VINR0", &sValeur) ;
	if (sValeur != "")
	{
		dINR = StringToDouble(sValeur) ;
    if (dINR > 4.5)
    {
    	pMailObj->iAlertLevel = NSNewMailObject::mailDangerAlert ;
      pMailObj->sAlertMessage = string("INR > 4,5 (") + sValeur + string(")") ;
    }
	}

	return ;
}

void
NSMailListWindow::sendToArchives(NSNewMailObject* pMailObj)
{
	string sFileName = sDirectory + pMailObj->sFileName ;

  string sNomSeul   = string("") ;
  string sExtension = string("") ;

	size_t pos1 = pMailObj->sFileName.find_last_of(".") ;
  if (pos1 == NPOS)
		sNomSeul = pMailObj->sFileName ;
  else
  {
  	sNomSeul   = string(pMailObj->sFileName, 0, pos1) ;
    sExtension = string(pMailObj->sFileName, pos1+1, strlen(pMailObj->sFileName.c_str())-1) ;
  }

  string sArchiveFileName = sArchivesDirectory + nomSansDoublons(sArchivesDirectory, sNomSeul, sExtension) ;

	::MoveFile(sFileName.c_str(), sArchiveFileName.c_str()) ;

  // Archiving workflow file
  //
  if (pMailObj->sWorkflowFileName != string(""))
  {
  	string sWrkArchiveFileName ;
  	string sWrkFileName = sDirectory + pMailObj->sWorkflowFileName ;

  	pos1 = sArchiveFileName.find_last_of(".") ;
  	if (pos1 == NPOS)
			sWrkArchiveFileName = sArchiveFileName + string(".wrk") ;
  	else
  		sWrkArchiveFileName = string(sArchiveFileName, 0, pos1) + string(".wrk") ;

  	::MoveFile(sWrkFileName.c_str(), sWrkArchiveFileName.c_str()) ;
  }

	removeEntry(pMailObj) ;
}

void
NSMailListWindow::removeEntry(NSNewMailObject* pMailObj)
{
	if ((NULL == pMailObj) || (aNewMails.empty()))
		return ;

	int indexItem = 0 ;
	NSNewMailObjectIter objIt = aNewMails.begin() ;
  for (; (objIt != aNewMails.end()) && (*objIt != pMailObj) ; objIt++, indexItem++) ;

  if (objIt == aNewMails.end())
		return ;

	delete (*objIt) ;
	aNewMails.erase(objIt) ;

	AfficheListe() ;

	if (aNewMails.empty())
		return ;

	if (indexItem > 0)
		indexItem-- ;

	SetItemState(indexItem, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
  // SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE) ;

  Invalidate() ;

  processSelectionChange() ;
}

void
NSMailListWindow::displayMessage()
{
	bLogOn = false ;

	if ((NULL == pParentWindow) || (NULL == pParentWindow->getTxtViewer()))
  	return ;

	NSNewMailObject* pSelectedMail = aNewMails[iPreviousSelection] ;
  if (NULL == pSelectedMail)
    return ;

  string sFileName = sDirectory + pSelectedMail->sFileName ;

  ImportedFileType iMailType = pSelectedMail->iType ;

  if ((FILETYPE_EPISODUS_3 == iMailType) || (FILETYPE_EPISODUS_4 == iMailType))
  {
    NSHtmlViewerWindow* pHtmlViewer = pParentWindow->getHtmlViewer() ;
    if (NULL != pHtmlViewer)
      pHtmlViewer->displayFile(sFileName) ;
  }
  else
  {
    NSTxtViewerWindow* pTxtViewer = pParentWindow->getTxtViewer() ;
    if (NULL != pTxtViewer)
    {
	    if (bDetailsOn)
  	    pTxtViewer->displayFile(sFileName) ;
      else
		    pTxtViewer->displayFileBody(sFileName) ;
    }
  }
}

void
NSMailListWindow::reinitMailList()
{
	aNewMails.vider() ;
	initMailList() ;
  sortAgain() ;
  AfficheListe() ;
}

// -----------------------------------------------------------------------------
// Fonction de réponse au double-click
// -----------------------------------------------------------------------------
void
NSMailListWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  HitTest(info) ;
  processSelectionChange() ;

  //if (info.GetFlags() & LVHT_ONITEM)  //	pView->CmModifier() ;
  //else
  //	pView->CmNouveau() ;
}

void
NSMailListWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    processSelectionChange() ;    NSNewMailObject* pSelectedMail = aNewMails[indexItem] ;    // création d'un menu popup		NS_CLASSLIB::TPoint lp = point ;

		OWL::TPopupMenu menu ;

		NSSuper* pSuper = pContexte->getSuperviseur() ;

		string sOpen = pSuper->getText("mailBox", "openPatient") ;
		string sITxt = pSuper->getText("mailBox", "importTxtFile") ;
		string sTree = pSuper->getText("mailBox", "importParsedFile") ;
		string sBoth = pSuper->getText("mailBox", "importBoth") ;
    string sExpl = pSuper->getText("mailBox", "explodeThisMessage") ;
		string sDeta = pSuper->getText("mailBox", "viewDetails") ;
		string sBody = pSuper->getText("mailBox", "viewMessageBodyOnly") ;
    string sDele = pSuper->getText("mailBox", "deleteThisMessage") ;
    string sMess = pSuper->getText("mailBox", "displayMessage") ;
    string sEdit = pSuper->getText("mailBox", "setMessageProperties") ;
    string sLog  = pSuper->getText("mailBox", "displayProcessLog") ;

		if ((pSelectedMail->iType == FILETYPE_HPRIM1_MULTI) || (pSelectedMail->iType == FILETYPE_HPRIM2_MULTI))
    {
    	menu.AppendMenu(MF_STRING, CM_EXPLODE_MM,  sExpl.c_str()) ;
      menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
    }
    else
    {
    	if ((pSelectedMail->iType == FILETYPE_HPRIM1) || (pSelectedMail->iType == FILETYPE_HPRIM2) ||
          (pSelectedMail->iType == FILETYPE_EPISODUS_4) || (pSelectedMail->iType == FILETYPE_EPISODUS_3))
      {
				menu.AppendMenu(MF_STRING, CM_OPEN_PAT,    sOpen.c_str()) ;
        if (NULL != pContexte->getPatient())
					menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
      }
      if (NULL != pContexte->getPatient())
      {
        if ((pSelectedMail->iType != FILETYPE_EPISODUS_4) && (pSelectedMail->iType != FILETYPE_EPISODUS_3))
				  menu.AppendMenu(MF_STRING, CM_IMPORT_TXT,  sITxt.c_str()) ;
				menu.AppendMenu(MF_STRING, CM_IMPORT_CS,   sTree.c_str()) ;
				// menu.AppendMenu(MF_STRING, CM_IMPORT_BOTH, sBoth.c_str()) ;
        menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
      }
    }

    if (bLogOn)
    	menu.AppendMenu(MF_STRING, CM_LOG_ON_OFF, sMess.c_str()) ;
    else
    {
    	if (bDetailsOn)
    		menu.AppendMenu(MF_STRING, CM_DETAILS_HIDE, sBody.c_str()) ;
    	else
    		menu.AppendMenu(MF_STRING, CM_DETAILS_VIEW, sDeta.c_str()) ;

      menu.AppendMenu(MF_STRING, CM_LOG_ON_OFF, sLog.c_str()) ;
    }

    menu.AppendMenu(MF_STRING, CM_EDIT_MM, sEdit.c_str()) ;

    menu.AppendMenu(MF_SEPARATOR, 0, 0);
    menu.AppendMenu(MF_STRING, CM_DELETE_MM, sDele.c_str()) ;

		ClientToScreen(lp) ;
		menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
		TListWindow::EvRButtonDown(modkeys, point) ;
  }
  else
		TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSMailListWindow::EvLButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modkeys, point) ;

	processSelectionChange() ;}
void
NSMailListWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
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
NSMailListWindow::EvLButtonUp(uint modKeys, NS_CLASSLIB::TPoint& pt)
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
NSMailListWindow::IndexItemSelect()
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
NSMailListWindow::sortAgain()
{
	switch (iSortedColumn)
  {
  	case 0 : sortByName() ; break ;
  	case 1 : sortByAlert() ; break ;
    case 2 : sortByExamDate() ; break ;
	}
}

void
NSMailListWindow::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
		case 0  :
    	changeAlertSortOrder() ;
    	sortByAlert() ;
      break ;
    case 1  :
    	changeNameSortOrder() ;
    	sortByName() ;
      break ;
    case 2  :
    	changeTypeSortOrder() ;
    	sortByType() ;
      break ;
    case 3  :
    	changeErrorSortOrder() ;
    	sortByError() ;
      break ;
    case 4  :
    	changeExamDateSortOrder() ;
    	sortByExamDate() ;
      break ;
  }
}

void
NSMailListWindow::changeSortOrder(int iCol)
{
	if (iSortedColumn == iCol)
  {
    if (bNaturallySorted)
      bNaturallySorted = false ;
    else
      bNaturallySorted = true ;
  }
  else
  {
    iSortedColumn = iCol ;
    bNaturallySorted = true ;
  }
}

void
NSMailListWindow::sortByAlert()
{
  if (aNewMails.empty())
    return ;

  if (bNaturallySorted)
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByAlertInf) ;
  else
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByAlertSup) ;

  AfficheListe() ;
}

void
NSMailListWindow::sortByName()
{
  if (aNewMails.empty())
    return ;

  if (bNaturallySorted)
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByNameInf) ;
  else
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByNameSup) ;

  AfficheListe() ;
}

void
NSMailListWindow::sortByType()
{
  if (aNewMails.empty())
    return ;

  if (bNaturallySorted)
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByTypeSup) ;
  else
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByTypeInf) ;

  AfficheListe() ;
}

void
NSMailListWindow::sortByError()
{
  if (aNewMails.empty())
    return ;

  if (bNaturallySorted)
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByErrorSup) ;
  else
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByErrorInf) ;

  AfficheListe() ;
}

void
NSMailListWindow::sortByExamDate()
{
  if (aNewMails.empty())
    return ;

  if (bNaturallySorted)
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByExamDateSup) ;
  else
    sort(aNewMails.begin(), aNewMails.end(), mailObjSortByExamDateInf) ;

  AfficheListe() ;
}

void
NSMailListWindow::CmOpenPatient()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

	pParentWindow->openPatient(pSelectedMail) ;
}

void
NSMailListWindow::CmImportTxtFile()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

  bool bCanProceed = pParentWindow->canProceedWithPatient(pSelectedMail) ;
  if (false == bCanProceed)
		return ;

	bool bSuccess = pParentWindow->importTxtDocument(pSelectedMail) ;

  if (bSuccess)
		sendToArchives(pSelectedMail) ;
}

void
NSMailListWindow::CmImportTreeFile()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

  bool bCanProceed = pParentWindow->canProceedWithPatient(pSelectedMail) ;
  if (false == bCanProceed)
		return ;

	bool bSuccess = pParentWindow->importParsedDocument(pSelectedMail) ;

	if (bSuccess)
		sendToArchives(pSelectedMail) ;
}

void
NSMailListWindow::CmImportBothFiles()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

  bool bCanProceed = pParentWindow->canProceedWithPatient(pSelectedMail) ;
  if (!bCanProceed)
		return ;

	bool bSuccess = pParentWindow->importBothDocuments(pSelectedMail) ;

  if (bSuccess)
		sendToArchives(pSelectedMail) ;
}

void
NSMailListWindow::CmSetDetailsMode()
{
	bDetailsOn = true ;

  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

  NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;
  string sFileName = sDirectory + pSelectedMail->sFileName ;

  // Ask the view to display
	if (NULL == pParentWindow->getTxtViewer())
  	return ;

	pParentWindow->getTxtViewer()->displayFile(sFileName) ;
}

void
NSMailListWindow::CmSetLogMode()
{
	if (true == bLogOn)
	{
  	bLogOn = false ;
  	displayMessage() ;
    return ;
	}

	bLogOn = true ;

  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

  NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;
  if (NULL == pSelectedMail)
		return ;

  // Ask the view to display
	if (NULL == pParentWindow->getTxtViewer())
  	return ;

	pParentWindow->getTxtViewer()->displayText(&(pSelectedMail->sProcessLog)) ;
}

void
NSMailListWindow::CmSetBodyMode()
{
	bDetailsOn = false ;

  int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

  NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;
  string sFileName = sDirectory + pSelectedMail->sFileName ;

  // Ask the view to display
	if (NULL == pParentWindow->getTxtViewer())
  	return ;

	pParentWindow->getTxtViewer()->displayFileBody(sFileName) ;
}

void
NSMailListWindow::CmExplodeMultipleMessage()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

	bool bSuccess = pParentWindow->explodeMultipleMessages(pSelectedMail) ;

  if (bSuccess)
		sendToArchives(pSelectedMail) ;

	reinitMailList() ;
}

void
NSMailListWindow::CmDeleteMessage()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;
	sendToArchives(pSelectedMail) ;
}

void
NSMailListWindow::CmSetDocParams()
{
	int iCurrentSelection = IndexItemSelect() ;
  if (iCurrentSelection < 0)
		return ;

	NSNewMailObject* pSelectedMail = aNewMails[iCurrentSelection] ;

  if (IDOK == PreEnregMailDialog(this, pSelectedMail, pContexte).Execute())
    return ;
}

void
NSMailListWindow::EvSetFocus(HWND hWndLostFocus)
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
NSMailListWindow::EvKillFocus(HWND hWndGetFocus)
{
	SetBkColor(0x00ffffff) ; // 0x00bbggrr
  SetTextBkColor(0x00ffffff) ;
  Invalidate() ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSTxtViewerWindow//
// -----------------------------------------------------------------------------
NSTxtViewerWindow::NSTxtViewerWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module)
                  :TEditFile(parent, id, "", 0, 0, 0, 0), NSRoot(pCtx)
{
	pParentWindow = parent ;
}

NSTxtViewerWindow::~NSTxtViewerWindow()
{
}

void
NSTxtViewerWindow::displayFile(string sFileCompleteName)
{
  pParentWindow->openTextViewer() ;

	if (sFileCompleteName == string(""))
		return ;

	ReplaceWith(sFileCompleteName.c_str()) ;

/*
	ifstream inFile ;
	string sLine ;
	string sText = "" ;

	inFile.open(sFileName.c_str());	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
	}

	while (!inFile.eof())
  {
  	getline(inFile,line) ;
    if (line != "")
    	sText += line + string("\r\n") ;
  }

	// Remplace les \r\n par des \r\r\n : uniquement utile pour envoyer dans un edit
  size_t posCr = sTexte.find("\r\n") ;
  while (posCr != string::npos)
  {
    if ((posCr > 0) && (sTexte[posCr-1] != '\n'))
      sTexte = string(sTexte, 0, posCr) + string(1, '\r') + string(sTexte, posCr, strlen(sTexte.c_str()) - posCr) ;
    posCr = sTexte.find("\r\n", posCr + 2) ;
  }


  inFile.close() ;

  SetText(sText.c_str());
*/
}

void
NSTxtViewerWindow::displayFileBody(string sFileCompleteName)
{
  pParentWindow->openTextViewer() ;

	NSCaptureFromHPRIM HprimHunter(pContexte) ;

	HprimHunter.setFileName(sFileCompleteName) ;

	string sMessageBody = HprimHunter.getHPRIMBody() ;
  texteForEditControl(&sMessageBody) ;

  SetText(sMessageBody.c_str()) ;
}

void
NSTxtViewerWindow::displayText(string* pTxt)
{
	if (NULL == pTxt)
		return ;

	SetText(pTxt->c_str()) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSHtmlViewerWindow//
// -----------------------------------------------------------------------------
NSHtmlViewerWindow::NSHtmlViewerWindow(NSContexte* pCtx, NSMailServiceWindow *parent, int id, int x, int y, int w, int h, TModule* module)
                   :TWindow(parent), NSRoot(pCtx)
{
	pParentWindow = parent ;

  Form = 0 ;
}

NSHtmlViewerWindow::~NSHtmlViewerWindow()
{
  // Delete de la Form
  if (NULL != Form)
	  delete Form ;
	// CoUninitialize();
	// OleUninitialize() ;
}

void
NSHtmlViewerWindow::displayFile(string sFileCompleteName)
{
  if (NULL != pParentWindow)
    pParentWindow->openHtmlViewer() ;

  sFileName = sFileCompleteName ;
  if (string("") != sFileName)
		Navigate() ;
}

//
// Override a TWindow virtual to create the HWND directly.
// NSImportWindow and the VCL TForm1 class both end up
// wrapping the same HWND.
//
void
NSHtmlViewerWindow::PerformCreate(int /*menuOrId*/)
{
  // OleInitialize(NULL) ;    // Use OleInitialize to get clipboard functionality
	// Form = new TWebProxy(NULL == Parent ? 0 : Parent->GetHandle(), 0) ;
  Form = new TWebProxy(GetHandle(), 0) ;

	Form->Visible      = false ;
  Form->BorderStyle  = bsNone ;  // Not resizeable; no visible border line
	Form->ParentWindow = HWindow ; // ( NULL == Parent ? 0 : Parent->HWindow ) ;
  SetHandle(Form->Handle) ;
  // ::SetParent(Forms::Application->Handle, pContexte->GetMainWindow()->HWindow) ;

	// on navigue vers le fichier html à importer
  if (string("") != sFileName)
		Navigate() ;
}

void
NSHtmlViewerWindow::SetupWindow(){	TWindow::SetupWindow() ;	ShowWindow( SW_HIDE ) ;}
void
NSHtmlViewerWindow::Navigate()
{
  if ((NULL == Form) || (NULL == Form->Control))
    return ;

  if (string("") == sFileName)
  {
    string ps = string("NSHtmlViewerWindow::Navigate with empty file name") ;
	  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

  Variant Flags(navNoReadFromCache) ;
  TVariant VFlags = Flags.operator TVariant() ;

  wchar_t buff[1024] ;
  MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, sFileName.c_str(), -1, buff, sizeof(buff)) ;

  Form->Control->Navigate(buff, &VFlags) ;
}

// -----------------------------------------------------------------------------
//
// Méthodes de NSNewMailObject//
// -----------------------------------------------------------------------------
bool
mailObjSortByAlertInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->iAlertLevel < pObj2->iAlertLevel) ;
}

bool
mailObjSortByAlertSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->iAlertLevel > pObj2->iAlertLevel) ;
}

bool
mailObjSortByNameInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->getDisplayName() < pObj2->getDisplayName()) ;
}

bool
mailObjSortByNameSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->getDisplayName() > pObj2->getDisplayName()) ;
}

bool
mailObjSortByExamDateInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->sExamDate < pObj2->sExamDate) ;
}

bool
mailObjSortByExamDateSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->sExamDate > pObj2->sExamDate) ;
}

bool
mailObjSortByErrorInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->getProcessErrorLevel() < pObj2->getProcessErrorLevel()) ;
}

bool
mailObjSortByErrorSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->getProcessErrorLevel() > pObj2->getProcessErrorLevel()) ;
}

bool
mailObjSortByTypeInf(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->iType < pObj2->iType) ;
}

bool
mailObjSortByTypeSup(NSNewMailObject *pObj1, NSNewMailObject *pObj2)
{
  if ((NULL == pObj1) || (NULL == pObj2))
    return false ;

	return (pObj1->iType > pObj2->iType) ;
}

NSNewMailObject::NSNewMailObject(NSContexte* pCtx)
                :demographicInformation(pCtx->getSuperviseur()),
                 clinicalInformation(pCtx->getSuperviseur())
{
  lObjectCount++ ;

	pContexte       = pCtx ;

	sFileName       = string("") ;
	sWorkflowFileName = string("") ;
  iType           = FILETYPE_UNKNOWN ;

	sReceptionDate  = string("") ;

	sPatientName    = string("") ;
	sPatientSurname = string("") ;
	sPatientBirth   = string("") ;
	sExamName       = string("") ;
  sExamDate       = string("") ;
	sExamCode       = string("") ;
	sSender         = string("") ;
	sReceiver       = string("") ;

  sProcessLog     = string("") ;

  iAlertLevel     = mailUndefinedAlertLevel ;
  sAlertMessage   = string("") ;
  iWorkflowState  = mailUnknowState ;
}

NSNewMailObject::~NSNewMailObject()
{
  lObjectCount-- ;
}

NSNewMailObject::NSNewMailObject(const NSNewMailObject& rv)
                :demographicInformation(rv.demographicInformation),
                 clinicalInformation(rv.clinicalInformation)
{
  lObjectCount++ ;

	pContexte       = rv.pContexte ;

	sFileName       = rv.sFileName  ;
	sWorkflowFileName = rv.sWorkflowFileName  ;
  iType           = rv.iType  ;

	sReceptionDate  = rv.sReceptionDate ;

	sPatientName    = rv.sPatientName ;
	sPatientSurname = rv.sPatientSurname ;
	sPatientBirth   = rv.sPatientBirth ;
	sExamName       = rv.sExamName ;
  sExamDate       = rv.sExamDate ;
	sExamCode       = rv.sExamCode ;
	sSender         = rv.sSender ;
	sReceiver       = rv.sReceiver ;

  sProcessLog     = rv.sProcessLog ;

  iAlertLevel     = rv.iAlertLevel ;
  sAlertMessage   = rv.sAlertMessage ;
  iWorkflowState  = rv.iWorkflowState ;
}

NSNewMailObject&
NSNewMailObject::operator=(const NSNewMailObject& rv)
{
  if (&rv == this)
		return *this ;

  pContexte       = rv.pContexte ;

  demographicInformation = rv.demographicInformation ;
  clinicalInformation    = rv.clinicalInformation ;

	sFileName         = rv.sFileName  ;
	sWorkflowFileName = rv.sWorkflowFileName  ;
  iType             = rv.iType  ;

	sReceptionDate  = rv.sReceptionDate ;

	sPatientName    = rv.sPatientName ;
	sPatientSurname = rv.sPatientSurname ;
	sPatientBirth   = rv.sPatientBirth ;
	sExamName       = rv.sExamName ;
  sExamDate       = rv.sExamDate ;
	sExamCode       = rv.sExamCode ;
	sSender         = rv.sSender ;
	sReceiver       = rv.sReceiver ;

  sProcessLog     = rv.sProcessLog ;

  iAlertLevel     = rv.iAlertLevel ;
  sAlertMessage   = rv.sAlertMessage ;
  iWorkflowState  = rv.iWorkflowState ;

  return *this ;
}

void
NSNewMailObject::initFromCapture(NSCaptureArray* pCapture)
{
	if (UINT(pCapture->trouveChemin(string("ZADMI1/LIDET1/LNOM01"), &sPatientSurname))== string::npos)
  	return ;

	pCapture->trouveChemin(string("ZADMI1/LIDET1/LNOM21"), &sPatientName) ;
  pCapture->trouveChemin(string("ZADMI1/LIDET1/KNAIS1"), &sPatientBirth) ;

  pCapture->trouveChemin(string("KCHIR2"), &sExamDate) ;
}

void
NSNewMailObject::initForUnknownPerson()
{
  sPatientName    = string("?") ;
	sPatientSurname = string("?") ;
}

string
NSNewMailObject::getDisplayName()
{
	string sLang = pContexte->getUserLanguage() ;

	string sPatName = string("") ;

	if ((FILETYPE_HPRIM1_MULTI == iType) || (FILETYPE_HPRIM2_MULTI == iType))
	{
		sPatName = pContexte->getSuperviseur()->getText("mailBox", "multipleMessage") ;
    return string("- ") + sPatName ;
	}

	sPatName = sPatientSurname + string(" ") + sPatientName ;

	if (string("") != sPatientBirth)
    sPatName += string(" (") + donne_date(sPatientBirth, sLang) + string(")") ;

  return sPatName ;
}

string
NSNewMailObject::getCurrentPatientDisplayName()
{
	NSPatientChoisi* pPatEnCours = pContexte->getPatient() ;
  if (NULL == pPatEnCours)
		return string("") ;

	string sLang = pContexte->getUserLanguage() ;

	string sPatName = pPatEnCours->getNom() + string(" ") + pPatEnCours->getPrenom() ;

  if (string("") != pPatEnCours->getNaissance())
    sPatName += string(" (") + donne_date(pPatEnCours->getNaissance(), sLang) + string(")") ;

  return sPatName ;
}

bool
NSNewMailObject::createWorkflowFile(string sFileName)
{
try
{
	ofstream outFile ;
  outFile.open(sFileName.c_str()) ;
  if (!outFile)  {
		string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

  // Workflow information
  //
  outFile << (string("[workflow]") + string("\n")) ;

  outFile << (string("version= 0.1") + string("\n")) ;
  switch (iType)
  {
  	case FILETYPE_UNKNOWN      : outFile << (string("type= unknown") + string("\n")) ; break ;
    case FILETYPE_HPRIM1       : outFile << (string("type= hprim1") + string("\n")) ; break ;
    case FILETYPE_HPRIM2       : outFile << (string("type= hprim2") + string("\n")) ; break ;
    case FILETYPE_HPRIM1_MULTI : outFile << (string("type= multi_hprim1") + string("\n")) ; break ;
    case FILETYPE_HPRIM2_MULTI : outFile << (string("type= multi_hprim2") + string("\n")) ; break ;
    case FILETYPE_EPISODUS_4   : outFile << (string("type= episodus_4") + string("\n")) ; break ;
    case FILETYPE_EPISODUS_3   : outFile << (string("type= episodus_3") + string("\n")) ; break ;
  }
  switch (iWorkflowState)
  {
    case mailNew    : outFile << (string("stage= new") + string("\n")) ; break ;
    case mailNotNew : outFile << (string("stage= old") + string("\n")) ; break ;
    case mailRead   : outFile << (string("stage= read") + string("\n")) ; break ;
  }
  switch (iAlertLevel)
  {
    case mailNoAlert     : outFile << (string("alert= none") + string("\n")) ; break ;
    case mailAlertLevel1 : outFile << (string("alert= level1") + string("\n")) ; break ;
    case mailAlertLevel2 : outFile << (string("alert= level2") + string("\n")) ; break ;
    case mailDangerAlert : outFile << (string("alert= danger") + string("\n")) ; break ;
  }

  outFile << string("\n") ;

  // Message header information
  //
  outFile << (string("[header]") + string("\n")) ;

	if (sReceptionDate != string(""))
  	outFile << (string("receptionDate= ") + sReceptionDate + string("\n")) ;
	if (sPatientName != string(""))
  	outFile << (string("patientName= ") + sPatientName + string("\n")) ;
  if (sPatientSurname != string(""))
  	outFile << (string("patientSurname= ") + sPatientSurname + string("\n")) ;
  if (sPatientBirth != string(""))
  	outFile << (string("patientBirth= ") + sPatientBirth + string("\n")) ;
  if (sExamName != string(""))
  	outFile << (string("examName= ") + sExamName + string("\n")) ;
  if (sExamDate != string(""))
  	outFile << (string("examDate= ") + sExamDate + string("\n")) ;
  if (sExamCode != string(""))
  	outFile << (string("examCode= ") + sExamCode + string("\n")) ;
  if (sSender != string(""))
  	outFile << (string("sender= ") + sSender + string("\n")) ;
  if (sReceiver != string(""))
  	outFile << (string("receiver= ") + sReceiver + string("\n")) ;

	outFile << string("\n") ;

	// Log file
  //
  if (sProcessLog != string(""))
	{
  	outFile << (string("[processLog]") + string("\n")) ;
    outFile << (sProcessLog + string("\n")) ;
	}

  // Clinical information
  //
  if (!(clinicalInformation.empty()))
	{
  	outFile << (string("[clinicalInformation]") + string("\n")) ;

    string sClinicalTreeAsXml = clinicalInformation.genereXML() ;
    outFile << (sClinicalTreeAsXml + string("\n")) ;
	}

  outFile.close() ;

  return true ;
}
catch (...)
{
	erreur("Exception NSNewMailObject::createWorkflowFile.", standardError, 0) ;
  return false ;
}
}

bool
NSNewMailObject::initFromWorkflowFile(string sFileName)
{
try
{
	ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

  string sLine ;
  string sCurrentChapter = string("") ;
  string sCurrentElement = string("") ;
  string sCurrentValue   = string("") ;

  while (!inFile.eof())
  {    getline(inFile, sLine) ;    if (sLine != "")    {    	// new chapter      //			if (sLine[0] == '[')      {      	size_t iPos = sLine.find("]") ;        if (iPos != NPOS)        {        	if ((sCurrentChapter == string("clinicalInformation")) && (sCurrentValue != string("")))          	clinicalInformation.initialiseDepuisXML(&sCurrentValue) ;          if ((sCurrentChapter == string("processLog")) && (sCurrentValue != string("")))          	sProcessLog = sCurrentValue ;        	sCurrentChapter = string(sLine, 1, iPos - 1) ;          sCurrentValue   = string("") ;        }      }      // Parsing element and value (if chapter is not "clinical information")      //      else if ((sCurrentChapter != string("clinicalInformation")) &&               (sCurrentChapter != string("processLog")))      {      	size_t iPos = sLine.find("=") ;        if ((iPos != NPOS) && (iPos != 0))        {        	sCurrentElement = string(sLine, 0, iPos) ;          if (iPos < strlen(sLine.c_str()) - 1)          {          	sCurrentValue = string(sLine, iPos+1, strlen(sLine.c_str())-iPos-1) ;          	initValuePairInformation(sCurrentChapter, sCurrentElement, sCurrentValue) ;          }        }      }      else      {      	sCurrentValue += sLine ;        if (sCurrentChapter == string("processLog"))        	sCurrentValue += string("\r\n") ;      }    }  }
  inFile.close() ;

  return true ;
}
catch (...)
{
	erreur("Exception NSNewMailObject::initFromWorkflowFile.", standardError, 0) ;
  return false ;
}
}

void
NSNewMailObject::initValuePairInformation(string sChapter, string sTitle, string sValue)
{
	strip(sChapter, stripBoth) ;
  strip(sValue, stripBoth) ;

  if ((sChapter == string("")) || (sValue == string("")))
		return ;

	// Workflow information
  //
  if (sTitle == string("type"))
	{
  	if      (sValue == string("unknown"))      iType = FILETYPE_UNKNOWN ;
  	else if (sValue == string("hprim1"))       iType = FILETYPE_HPRIM1 ;
    else if (sValue == string("hprim2"))       iType = FILETYPE_HPRIM2 ;
    else if (sValue == string("multi_hprim1")) iType = FILETYPE_HPRIM1_MULTI ;
    else if (sValue == string("multi_hprim2")) iType = FILETYPE_HPRIM2_MULTI ;
    else if (sValue == string("episodus_3"))   iType = FILETYPE_EPISODUS_3 ;
    else if (sValue == string("episodus_4"))   iType = FILETYPE_EPISODUS_4 ;
  }
	else if (sTitle == string("stage"))
	{
  	if      (sValue == string("new"))  iWorkflowState = mailNew ;
    else if (sValue == string("old"))  iWorkflowState = mailNotNew ;
    else if (sValue == string("read")) iWorkflowState = mailRead ;
  }
  else if (sTitle == string("alert"))
	{
  	if      (sValue == string("none"))   iAlertLevel = mailNoAlert ;
    else if (sValue == string("level1")) iAlertLevel = mailAlertLevel1 ;
    else if (sValue == string("level2")) iAlertLevel = mailAlertLevel2 ;
    else if (sValue == string("danger")) iAlertLevel = mailDangerAlert ;
  }
  //
  // Message header information
  //
  else if (sTitle == string("receptionDate"))
  	sReceptionDate = sValue ;
  else if (sTitle == string("patientName"))
  	sPatientName = sValue ;
  else if (sTitle == string("patientSurname"))
  	sPatientSurname = sValue ;
  else if (sTitle == string("patientBirth"))
  	sPatientBirth = sValue ;
  else if (sTitle == string("examName"))
  	sExamName = sValue ;
  else if (sTitle == string("examDate"))
  	sExamDate = sValue ;
  else if (sTitle == string("examCode"))
  	sExamCode = sValue ;
  else if (sTitle == string("sender"))
  	sSender = sValue ;
  else if (sTitle == string("receiver"))
  	sReceiver = sValue ;
}

size_t
NSNewMailObject::getProcessErrorLevel()
{
	if (sProcessLog == string(""))
		return 0 ;

	size_t pos = sProcessLog.find("Error") ;

	if (pos == NPOS)
		return 0 ;

	return pos ;
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
//  Méthodes de PreEnregMailDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(PreEnregMailDialog, TDialog)
  EV_COMMAND(IDOK,     CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------// Function     : PreEnregMailDialog::PreEnregMailDialog()
// Description  : Constructeur
// -----------------------------------------------------------------------------
PreEnregMailDialog::PreEnregMailDialog(TWindow *pere, NSNewMailObject *pMailObj, NSContexte *pCtx)
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
  erreur("Exception PreEnregMailDialog ctor.", standardError, 0) ;
}}


// -----------------------------------------------------------------------------
// Function     : PreEnregMailDialog::~PreEnregMailDialog()
// Description  : Destructeur, enregistre le document
// -----------------------------------------------------------------------------
PreEnregMailDialog::~PreEnregMailDialog()
{
  // Suppression de tous les objets
  delete _pNomDoc ;
  delete _pType ;
  delete _pDateCreation ;
  delete _pHistorique ;
}

// -----------------------------------------------------------------------------
// Function     : PreEnregMailDialog::SetupWindow()
// Arguments    :	Aucun
// Description  : Initialise la boite de dialogue
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregMailDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

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
}

// -----------------------------------------------------------------------------
// Function     : PreEnregMailDialog::CmOk()
// Description  : Réponse au bouton OK
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregMailDialog::CmOk()
{
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

	// Effectue le CmOk
	TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
// Function     : PreEnregMailDialog::CmCancel()
// Description  : Réponse au bouton Cancel
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
PreEnregMailDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

////////////////////////// fin de nsMailSvce.cpp ///////////////////////////////
