#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\gauge.h>
#include <owl\listwind.h>
#include <owl\uihelper.h>
#include <winsys/uimetric.h>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsUpdateDlg.h"
#include "nautilus\nsupdate.h"

#include "nsoutil\nsoutil.rh"

// -----------------------------------------------------------------
//
//                         NSUpdateParameters
//
// -----------------------------------------------------------------

NSUpdateParameters::NSUpdateParameters(NSContexte * pCtx)
                   :NSRoot(pCtx)
{
	_sSettingsFile = string("update.dat") ;

	_sURL        = "" ;
	_sLogin      = "" ;
	_sPassword   = "" ;
	_sProtocol   = "" ;
	_sUpdateDir  = "" ;

  _bLastUpdate      = false ;
	_tLastUpdate.init() ;

  _iInterval   = 0 ;
	_bLastUpdateCheck = true ;
	_bInterval        = true ;
  _tLastUpdateCheck.init() ;
	_tNextUpdateCheck.init() ;

  _bSilentMode      = false ;
}

NSUpdateParameters::~NSUpdateParameters()
{
}

bool
NSUpdateParameters::init()
{
  ifstream inFile ;
	// on ouvre le fichier de configuration
  inFile.open(_sSettingsFile.c_str()) ;
  if (!inFile)
  {
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + _sSettingsFile ;
    pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trWarning) ;

    //
    // Make certain that warning messages won't be hidden by splash screen
    //
    HWND hWnd = 0 ;
    if (pContexte->getSuperviseur()->getApplication())
    {
  	  TSplashWindow* pSplashWin = pContexte->getSuperviseur()->getApplication()->getSplashWindow() ;
      if (pSplashWin)
    	  hWnd = pSplashWin->GetHandle() ;
    }
    erreur(sErrorMsg.c_str(), warningError, 0, hWnd) ;
    return false ;
  }

	string sLine ;
  string sData = string("") ;
  // on lit les paramètres de backup
	while (!inFile.eof())
	{
		getline(inFile, sLine) ;
    sData += sLine + "\n" ;
	}

  inFile.close() ;

  size_t i = 0 ;

  string sNomAttrib ;
  string sValAttrib ;

	while (i < strlen(sData.c_str()))
	{
		sNomAttrib = "" ;
		sValAttrib = "" ;

		while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t'))
			sNomAttrib += pseumaj(sData[i++]) ;
		while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
			i++ ;

		while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))
			sValAttrib += sData[i++] ;

		i++ ;

		if 		((sNomAttrib == "URL") && (sValAttrib != ""))
      _sURL = sValAttrib ;
    else if ((sNomAttrib == "LOGIN") && (sValAttrib != ""))
      _sLogin = sValAttrib ;
    else if ((sNomAttrib == "PASSWORD") && (sValAttrib != ""))
      _sPassword = sValAttrib ;
    else if ((sNomAttrib == "PROTOCOL") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;
      _sProtocol = sValAttrib ;
    }
    else if ((sNomAttrib == "UPDATE_DIR") && (sValAttrib != ""))
      _sUpdateDir = sValAttrib ;
		else if ((sNomAttrib == "LAST_UPDATE") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bLastUpdate = false ;
      else
        _tLastUpdate.initFromDateHeure(sValAttrib) ;
		}
    else if ((sNomAttrib == "LAST_CHECK") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bLastUpdateCheck = false ;
      else
        _tLastUpdateCheck.initFromDateHeure(sValAttrib) ;
		}
		else if ((sNomAttrib == "INTERVAL_DAYS") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				_bInterval = false ;
      else
      {
      	_iInterval = atoi(sValAttrib.c_str()) ;
      	if (true == _bLastUpdate)
        {
        	_tNextUpdateCheck = _tLastUpdate ;
        	_tNextUpdateCheck.ajouteJours(_iInterval) ;
        }
      }
		}
    else if ((sNomAttrib == "SILENT") && (sValAttrib != ""))
		{
			if      (IsYes(sValAttrib))
				_bSilentMode = true ;
      else if (IsNo(sValAttrib))
        _bSilentMode = false ;
		}
	}

  // Validity check
  //
  if ((string("") == _sURL) || (string("") == _sUpdateDir))
  {
    string sErrorMsg = pContexte->getSuperviseur()->getText("updateParametersManagement", "invalidUpdateParameters") ;
    pContexte->getSuperviseur()->trace(&sErrorMsg, 1, NSSuper::trWarning) ;

    //
    // Make certain that warning messages won't be hidden by splash screen
    //
    HWND hWnd = 0 ;
    if (pContexte->getSuperviseur()->getApplication())
    {
  	  TSplashWindow* pSplashWin = pContexte->getSuperviseur()->getApplication()->getSplashWindow() ;
      if (pSplashWin)
    	  hWnd = pSplashWin->GetHandle() ;
    }
    erreur(sErrorMsg.c_str(), warningError, 0, hWnd) ;
    return false ;
  }

  return true ;
}

bool
NSUpdateParameters::mustCheckForUpdateAvailability()
{
  NVLdVTemps  tNow ;
  tNow.takeTime() ;

	if ((false == _bInterval) || ((true == _bLastUpdateCheck) && (_tNextUpdateCheck > tNow)))
		return false ;

	return true ;
}

bool
NSUpdateParameters::updateLastCheckDate()
{
	ofstream  outFile ;
	outFile.open(_sSettingsFile.c_str(), ios::out) ;
	if (!outFile)
		return false ;

	NVLdVTemps tNow ;
	tNow.takeTime() ;

  string sLastUpdateDate ;
	if (true == _tLastUpdate.estVide())
		sLastUpdateDate = string("never") ;
	else
  	sLastUpdateDate = _tLastUpdate.donneDateHeure() ;

  string sSilentMode ;
  if (true == _bSilentMode)
		sSilentMode = string("yes") ;
  else
  	sSilentMode = string("no") ;

  outFile << (string("URL ")         + _sURL                  + string("\n")) ;
  outFile << (string("LOGIN ")       + _sLogin                + string("\n")) ;
  outFile << (string("PASSWORD ")    + _sPassword             + string("\n")) ;
  outFile << (string("PROTOCOL ")    + _sProtocol             + string("\n")) ;
  outFile << (string("UPDATE_DIR ")  + _sUpdateDir            + string("\n")) ;
  outFile << (string("LAST_UPDATE ") + sLastUpdateDate       + string("\n")) ;
  outFile << (string("LAST_CHECK ")  + tNow.donneDateHeure() + string("\n")) ;

  char intervalDay[512] ;
  sprintf(intervalDay, "%d", _iInterval) ;
  outFile << (string("INTERVAL_DAYS ") + string(intervalDay) + string("\n")) ;

  outFile << (string("SILENT ") + sSilentMode + string("\n")) ;

/*
	char outLine[2048] ;
	sprintf(outLine, "URL %s\nLOGIN %s\nPASSWORD %s\nPROTOCOL %s\nUPDATE_DIR %s\nLAST_UPDATE %s\nLAST_CHECK %s\nINTERVAL_DAYS %d\n", sURL.c_str(), sLogin.c_str(), sPassword.c_str(), sProtocol.c_str(), sUpdateDir.c_str(), sLastUpdateDate.c_str(), tNow.donneDateHeure().c_str(), iInterval) ;
	outFile.write(outLine, strlen(outLine)) ;
*/

	outFile.close() ;

	return true ;
}

bool
NSUpdateParameters::updateLastUpdateDate()
{
	ofstream outFile ;
	outFile.open(_sSettingsFile.c_str(), ios::out) ;
	if (!outFile)
		return false ;

	NVLdVTemps  tNow ;
	tNow.takeTime() ;

  string sLastCheckDate ;
	if (true == _tLastUpdateCheck.estVide())
		sLastCheckDate = string("never") ;
	else
  	sLastCheckDate = _tLastUpdateCheck.donneDateHeure() ;

  string sSilentMode ;
  if (true == _bSilentMode)
		sSilentMode = string("yes") ;
  else
  	sSilentMode = string("no") ;

	char outLine[2048] ;
	sprintf(outLine, "URL %s\nLOGIN %s\nPASSWORD %s\nPROTOCOL %s\nUPDATE_DIR %s\nLAST_UPDATE %s\nLAST_CHECK %s\nINTERVAL_DAYS %d\nSILENT %s\n", _sURL.c_str(), _sLogin.c_str(), _sPassword.c_str(), _sProtocol.c_str(), _sUpdateDir.c_str(), tNow.donneDateHeure().c_str(), sLastCheckDate.c_str(), _iInterval, sSilentMode.c_str()) ;
	outFile.write(outLine, strlen(outLine)) ;
	outFile.close() ;

	return true ;
}

string
NSUpdateParameters::getReleaseTxt()
{
  string sReleaseFile = string("releases.txt") ;
  sReleaseFile = _sUpdateDir + sReleaseFile ;

  ifstream inFile ;
	// opening releases history file
  //
  inFile.open(sReleaseFile.c_str()) ;
  if (!inFile)
  {
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + sReleaseFile ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return string("") ;
  }

  string sText = string("") ;
  string sLine = string("") ;
  // just reading the first non empty line
	while (!inFile.eof())
  {
		getline(inFile, sLine) ;

    size_t iLineLen = strlen(sLine.c_str()) ;

    if (iLineLen > 2)
    {
      if (('\r' != sLine[iLineLen-2]) && ('\n' != sLine[iLineLen-1]))
        sLine += string(1, '\r') + string(1, '\n') ;
    }
    else
      sLine = string(1, '\r') + string(1, '\n') ;

    sText += sLine ;
  }

  inFile.close() ;

  return sText ;
}

// -----------------------------------------------------------------
//
//                         NsUpdateYesNoWindow
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsUpdateYesNoWindow, TLayoutWindow)
	EV_COMMAND(UPDATE_YES,     CmYes),
  EV_COMMAND(UPDATE_NO,      CmNo),
  EV_COMMAND(UPDATE_ENLARGE, CmEnlarge),
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsUpdateYesNoWindow::NsUpdateYesNoWindow(TWindow* pere, NSContexte* pCtx, NSUpdate* pUpdater)
	                  :TLayoutWindow(pere, "", pNSResModule)
{
  // Attr.Style = WS_POPUP | WS_VISIBLE | WS_BORDER | WS_CAPTION ;
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_CAPTION | WS_THICKFRAME ;

  pContexte        = pCtx ;
  _pUpdater        = pUpdater ;

  string MsgTxt = pContexte->getSuperviseur()->getText("updateManagement", "aMoreRecentVersionExistsDoYouWantToInstallIt") ;
  string MsgEnl = pContexte->getSuperviseur()->getText("updateManagement", "enlargeView") ;
  string MsgYes = pContexte->getSuperviseur()->getText("generalLanguage", "yes") ;
  string MsgNo  = pContexte->getSuperviseur()->getText("generalLanguage", "no") ;

  MsgYes[0] = pseumaj(MsgYes[0]) ;
  MsgNo[0]  = pseumaj(MsgNo[0]) ;

  _pQuestionText   = new OWL::TStatic(this, UPDATE_QUESTION, MsgTxt.c_str(), 0, 0, 0, 0) ;
  _pQuestionText->Attr.Style |= SS_CENTER ;
  TLayoutMetrics lmQuestion ;
  lmQuestion.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 4) ;
  lmQuestion.Width.PercentOf(lmParent, 98, lmWidth) ;
  lmQuestion.Y.Set(lmTop, lmBelow, lmParent, lmTop, 4) ;
  lmQuestion.Height.Absolute(12) ;
  SetChildLayoutMetrics(*_pQuestionText, lmQuestion) ;

  _pYesButton      = new OWL::TButton(this, UPDATE_YES, MsgYes.c_str(), 85, 35, 80, 30, true) ;
  TLayoutMetrics lmYesButton ;
  lmYesButton.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 30) ;
  lmYesButton.Width.PercentOf(lmParent, 15, lmWidth) ;
  lmYesButton.Y.Set(lmTop, lmBelow, _pQuestionText, lmBottom, 5) ;
  lmYesButton.Height.PercentOf(lmParent, 15, lmHeight) ;
  SetChildLayoutMetrics(*_pYesButton, lmYesButton) ;

  _pNoButton       = new OWL::TButton(this, UPDATE_NO, MsgNo.c_str(), 175, 35, 80, 30) ;
  TLayoutMetrics lmNoButton ;
  lmNoButton.X.Set(lmRight, lmLeftOf, lmParent, lmRight, 30) ;
  lmNoButton.Width.PercentOf(lmParent, 15, lmWidth) ;
  lmNoButton.Y.Set(lmTop, lmBelow, _pQuestionText, lmBottom, 5) ;
  lmNoButton.Height.PercentOf(lmParent, 15, lmHeight) ;
  SetChildLayoutMetrics(*_pNoButton, lmNoButton) ;

  _pReleasesViewer = new OWL::TEdit(this, UPDATE_RELEASE, "", 7, 60, 284, 50, 0, true) ;
  TLayoutMetrics lmReleasesText ;
  lmReleasesText.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 4) ;
  lmReleasesText.Width.PercentOf(lmParent, 98, lmWidth) ;
  lmReleasesText.Y.Set(lmTop, lmBelow, _pYesButton, lmBottom, 5) ;
  lmReleasesText.Height.PercentOf(lmParent, 60, lmHeight) ;
  SetChildLayoutMetrics(*_pReleasesViewer, lmReleasesText) ;

  _pEnlargeButton  = new OWL::TButton(this, UPDATE_ENLARGE, MsgEnl.c_str(), 7, 111, 284, 10) ;
  TLayoutMetrics lmEnlargeButton ;
  lmEnlargeButton.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 4) ;
  lmEnlargeButton.Width.PercentOf(lmParent, 98, lmWidth) ;
  lmEnlargeButton.Y.Set(lmTop, lmBelow, _pReleasesViewer, lmBottom, 1) ;
  lmEnlargeButton.Height.PercentOf(lmParent, 5, lmHeight) ;
  SetChildLayoutMetrics(*_pEnlargeButton, lmEnlargeButton) ;

  Attr.W = 750 ;
  Attr.H = 327 ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsUpdateYesNoWindow::~NsUpdateYesNoWindow()
{
	delete _pYesButton ;
  delete _pNoButton ;
  delete _pEnlargeButton ;
  delete _pReleasesViewer ;
  delete _pQuestionText ;
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsUpdateYesNoWindow::SetupWindow()
{
	TLayoutWindow::SetupWindow() ;

  string MsgHdr = pContexte->getSuperviseur()->getText("updateManagement", "updateManagement") ;
  SetCaption(MsgHdr.c_str()) ;

  string sReleaseText = _pUpdater->updateParameters.getReleaseTxt() ;
  if (string("") != sReleaseText)
    _pReleasesViewer->SetText(sReleaseText.c_str()) ;

  // Center window, make topmost and adjust size to accomidate static
  // and gauge.
  //
  ClassLib::TRect r  = GetWindowRect() ;

  ClassLib::TRect fullRect(0, 0, TUIMetric::CxScreen, TUIMetric::CyScreen) ;

  int x = (fullRect.Width() - r.Width()) / 2 ;
  int y = (fullRect.Height() - r.Height()) / 2 ;
  r.Offset(x, y) ;
  SetWindowPos(HWND_TOPMOST, r, SWP_SHOWWINDOW) ;
}

void
NsUpdateYesNoWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  int iAncDC = dc.SaveDC() ;

  NS_CLASSLIB::TColor BackgroundColor = NS_CLASSLIB::TColor(240, 255, 240) ;
  OWL::TBrush Pinceau(NS_CLASSLIB::TColor::LtGray) ;

  dc.FillRect(RectangleAPeindre, Pinceau) ;

  dc.RestoreDC(iAncDC) ;

  TLayoutWindow::Paint(dc, erase, RectangleAPeindre) ;
}

void
NsUpdateYesNoWindow::CmYes()
{
  _pUpdater->setWantUpdate(true) ;
  _pUpdater->setWaitingForUpdate(false) ;
	CloseWindow(IDOK) ;
}

void
NsUpdateYesNoWindow::CmNo()
{
  _pUpdater->setWantUpdate(false) ;
  _pUpdater->setWaitingForUpdate(false) ;
	Destroy() ;
}

void
NsUpdateYesNoWindow::CmEnlarge()
{
}


// -----------------------------------------------------------------
//
//  Méthodes de NsUpdateYesNoWindow
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsDownloadWindow, TLayoutWindow)
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NsDownloadWindow::NsDownloadWindow(TWindow* pere, NSContexte* pCtx, NSUpdate* pUpdater)
                 :TLayoutWindow(pere, "", pNSResModule)
{
  Attr.Style = WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_CAPTION | WS_THICKFRAME ;

  pContexte        = pCtx ;
  _pUpdater        = pUpdater ;

  _sCaption = pContexte->getSuperviseur()->getText("updateManagement", "downloadingUpdate") ;

/*
  string MsgDown = pContexte->getSuperviseur()->getText("updateManagement", "downloadingUpdate") ;
  _pQuestionText   = new OWL::TStatic(this, UPDATE_QUESTION, MsgDown.c_str(), 0, 0, 0, 0) ;
  _pQuestionText->Attr.Style |= SS_CENTER ;
  TLayoutMetrics lmQuestion ;
  lmQuestion.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 4) ;
  lmQuestion.Width.PercentOf(lmParent, 98, lmWidth) ;
  lmQuestion.Y.Set(lmTop, lmBelow, lmParent, lmTop, 4) ;
  lmQuestion.Height.Absolute(12) ;
  SetChildLayoutMetrics(*_pQuestionText, lmQuestion) ;
*/

  _pGauge = new OWL::TGauge(this, "%d%%", 2, 0, 0, 0, 0, true, 0) ;
  TLayoutMetrics lmGauge ;
  lmGauge.X.Set(lmLeft, lmRightOf, lmParent, lmLeft, 4) ;
  lmGauge.Width.PercentOf(lmParent, 98, lmWidth) ;
  lmGauge.Y.Set(lmTop, lmBelow, lmParent, lmTop, 2) ;
  lmGauge.Height.PercentOf(lmParent, 60, lmHeight) ;
  SetChildLayoutMetrics(*_pGauge, lmGauge) ;

  Attr.W = 564 ;
  Attr.H = 60 ;

  _pUpdater->pProgressFunctor = new MemUnaFunctor<NsDownloadWindow, int>(this, &NsDownloadWindow::SetPercentDone) ;
  _pUpdater->pFinishedFunctor = new MemUnaFunctor<NsDownloadWindow, int>(this, &NsDownloadWindow::IsFinished) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NsDownloadWindow::~NsDownloadWindow()
{
  delete _pGauge ;
  // delete _pQuestionText ;
}

//---------------------------------------------------------------------------
//  Fonction :   NsOutilDialog::SetupWindow()
//
//  Description: Initialise la boite de dialogue
//---------------------------------------------------------------------------
void
NsDownloadWindow::SetupWindow()
{
	TLayoutWindow::SetupWindow() ;

  SetCaption(_sCaption.c_str()) ;

  _pGauge->SetRange(0, 100) ;

  // Center window and adjust size to accomidate static
  // and gauge.
  //
  ClassLib::TRect r  = GetWindowRect() ;

  ClassLib::TRect fullRect(0, 0, TUIMetric::CxScreen, TUIMetric::CyScreen) ;

  int x = (fullRect.Width() - r.Width()) / 2 ;
  int y = (fullRect.Height() - r.Height()) / 2 ;
  r.Offset(x, y) ;
  SetWindowPos(0, r, SWP_SHOWWINDOW) ;
}

//
// Set the percentage done for the gauge control. If the splash screen does
// not have a gauge control, this does not do anything.
//
void
NsDownloadWindow::SetPercentDone(int percent)
{
  if (NULL == _pGauge)
    return ;

  string sTitle = _sCaption + string(" (") + IntToString(percent) + string("%)") ;
  SetCaption(sTitle.c_str()) ;

  _pGauge->SetValue(percent) ;
  GetApplication()->PumpWaitingMessages() ;
}

void
NsDownloadWindow::IsFinished(int iStatus)
{
  if (1 == iStatus)
    SendMessage(WM_CLOSE) ;
}

void
NsDownloadWindow::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectangleAPeindre)
{
  int iAncDC = dc.SaveDC() ;

  NS_CLASSLIB::TColor BackgroundColor = NS_CLASSLIB::TColor(240, 255, 240) ;
  OWL::TBrush Pinceau(NS_CLASSLIB::TColor::LtGray) ;

  dc.FillRect(RectangleAPeindre, Pinceau) ;

  dc.RestoreDC(iAncDC) ;

  TLayoutWindow::Paint(dc, erase, RectangleAPeindre) ;
}

// fin de nsUpdateDlg.cpp ..............................
///////////////////////////////////////////////////////

