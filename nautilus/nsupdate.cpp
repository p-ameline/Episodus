// -----------------------------------------------------------------------------
// nsupdate.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2011/10/12 09:12:00 $
// -----------------------------------------------------------------------------
// PA - aout 2007
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <fstream.h>
#include <process.h>

#include "curl\curl.h"
#include "curl\easy.h"
#include "curl\nscurl.h"

#include "nautilus\nsupdate.h"
#include "partage\nsdivfct.h"    // fonction erreur
#include "nsepisod\nsldvuti.h"

// ------------------------------------------------------------------------
//                                 NSUpdate
// ------------------------------------------------------------------------

NSUpdate::NSUpdate(NSContexte * pCtx)
         :NSRoot(pCtx), updateParameters(pCtx)
{
  _bWaitingForUpdateChoice = false ;
  _bWantUpdate             = false ;

  _sUpdateExecFullFileName = string("") ;
  _sExpectedVersionNumber  = string("") ;

  pProgressFunctor = 0 ;

  string ps = string("Init of update params") ;
	pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  curl.init() ;

	if (true == curl.aCurlOptParams.empty())
	{
  	ps = string("No specific curlopt params") ;
		pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
  }
  else
  {
  	ps = string("Specific curlopt params:") ;
		pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

		map<string, string>::iterator iter ;

		for (iter = curl.aCurlOptParams.begin() ; iter != curl.aCurlOptParams.end() ; iter++)
  	{
  		string sOption = (*iter).first ;
    	string sParam  = (*iter).second ;
      ps = string("- ") + sOption + sParam ;
			pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    }
  }
}

NSUpdate::~NSUpdate()
{
}

bool
NSUpdate::init()
{
  if (false == updateParameters.init())
    return false ;

  _sUpdateExecFullFileName = string("Epi5_maj.exe") ;
	if (string("") != updateParameters._sUpdateDir)
  	_sUpdateExecFullFileName = updateParameters._sUpdateDir + _sUpdateExecFullFileName ;

  return true ;
}

bool
NSUpdate::isUpdateAvailable()
{
	string sReleaseFile = string("releases.txt") ;
	if (string("") != updateParameters._sUpdateDir)
		sReleaseFile = updateParameters._sUpdateDir + sReleaseFile ;

	// don't download if sURL is the same as sUpdateDir
  //
  if (updateParameters._sURL != updateParameters._sUpdateDir)
	{
		if (string("") == updateParameters._sURL)
			return false ;

  	if (false == curl.bIsReady())
			return false ;

		curl.setURL(updateParameters._sURL) ;
		curl.setLogin(updateParameters._sLogin) ;
		curl.setPassword(updateParameters._sPassword) ;

  	bool bDownloadSuccess = curl.fileDownload(string("releases.txt"), sReleaseFile) ;

  	if (false == bDownloadSuccess)
    {
    	string ps = string("Curl download error: ") + curl.sLastCurlError ;
			pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
			return false ;
    }
	}

  // Record that we checked
  updateParameters.updateLastCheckDate() ;

  ifstream inFile ;
	// opening releases history file
  //
  inFile.open(sReleaseFile.c_str()) ;
  if (!inFile)
  {
  	string sErrorMsg = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorMsg += string(" ") + sReleaseFile ;
    erreur(sErrorMsg.c_str(), warningError, 0) ;
    return false ;
  }

  string sLine = string("") ;
  // just reading the first non empty line
	while (!inFile.eof() && (string("") == sLine))
		getline(inFile, sLine) ;

  inFile.close() ;

	if (string("") == sLine)
		return false ;

  // Line is of the kind "----> 5.03.0034 28/08/2007"
  //
	size_t pos = sLine.find(' ') ;
	if (NPOS == pos)
		return false ;

	size_t pos2 = sLine.find(' ', pos + 1) ;
  if (NPOS == pos2)
		return false ;

	_sExpectedVersionNumber = string(sLine, pos + 1, pos2 - pos - 1) ;

  if (_sExpectedVersionNumber > pContexte->getSuperviseur()->sNumVersion)
		return true ;

	return false ;
}

bool
NSUpdate::startUpdate(string sExecutionDirectory)
{
	string sRemoteFile = string("Epi5_maj.exe") ;

	string sReleaseFile = sRemoteFile ;
	if (string("") != updateParameters._sUpdateDir)
  	sReleaseFile = updateParameters._sUpdateDir + sReleaseFile ;

	// don't download if sURL is the same as sUpdateDir
  //
  if (updateParameters._sURL != updateParameters._sUpdateDir)
	{
		bool bProgressBar = false ;

    if (pProgressFunctor)
      bProgressBar = true ;

    bool bDownloadSuccess = curl.fileDownload(sRemoteFile, sReleaseFile, bProgressBar, pProgressFunctor) ;

    if (pProgressFunctor)
    {
			delete pProgressFunctor ;
      pProgressFunctor = 0 ;
    }

    if (pFinishedFunctor)
    {
      (*pFinishedFunctor)(1) ;

      delete pFinishedFunctor ;
      pFinishedFunctor = 0 ;
    }

  	if (false == bDownloadSuccess)
			return false ;
	}

  // check file version
  string sVersionInfo = getVersionInformation() ;
  if (sVersionInfo != _sExpectedVersionNumber)
  {
    string ps = string("Update cancelled: file is version ") + sVersionInfo + string(" when expected version is ") + _sExpectedVersionNumber ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
    return false ;
  }

  // /S runs the installer or uninstaller silently
  if (updateParameters._bSilentMode)
    sReleaseFile += string(" /S") ;

  // /D sets the default installation directory ($INSTDIR),
  //    overriding InstallDir and InstallDirRegKey.
  //    It must be the last parameter used in the command line and must not
  //    contain any quotes, even if the path contains spaces.
  //    Only absolute paths are supported.
  if (string("") != sExecutionDirectory)
    sReleaseFile += string(" /D=") + sExecutionDirectory ;

	// execution
	int iExecError = WinExec(sReleaseFile.c_str(), SW_SHOWNORMAL) ;

  // If the function succeeds, the return value is greater than 31
	if (iExecError > 31)
	{
  	// on doit remplacer la date de la dernière màj dans le fichier update.dat
    updateParameters.updateLastUpdateDate() ;
    return true ;
  }

  string sErrMsg ;
	switch (iExecError)
  {
		case 0                    :
    	erreur("The system is out of memory or resources.", standardError, 0) ;
      break ;
    case ERROR_BAD_FORMAT     :
    	sErrMsg = string("The .EXE file ( ") + sReleaseFile + string(" ) is invalid (non-Win32 .EXE or error in .EXE image)") ;
    	erreur(sErrMsg.c_str(), standardError, 0) ;
      break ;
    case ERROR_FILE_NOT_FOUND :
    	sErrMsg = string("The specified file was not found ( ") + sReleaseFile + string(" )") ;
    	erreur(sErrMsg.c_str(), standardError, 0) ;
      break ;
    case ERROR_PATH_NOT_FOUND :
      sErrMsg = string("The specified path was not found ( ") + sReleaseFile + string(" )") ;
    	erreur(sErrMsg.c_str(), standardError, 0) ;
    	break ;
    default                   :
    	sErrMsg = string("Unknown error when starting update ( ") + sReleaseFile + string(" )") ;
    	erreur(sErrMsg.c_str(), standardError, 0) ;
  }
  return true ;
}

string
NSUpdate::getVersionInformation()
{
  if (string("") == _sUpdateExecFullFileName)
    return string("") ;

  DWORD dwHandle = 0 ;
  DWORD dwSize = ::GetFileVersionInfoSize((char*) _sUpdateExecFullFileName.c_str(), &dwHandle) ;
  if (0 == dwSize)
    return string("") ;

  string sMajorVersion = string("") ;
  string sMinorVersion = string("") ;
  string sBuildVersion = string("") ;

  char szVersion[64] = {0} ;

  LPVOID lpData = ::LocalAlloc(LMEM_FIXED, dwSize) ;
  if (lpData)
  {
    if (::GetFileVersionInfo((char*) _sUpdateExecFullFileName.c_str(), dwHandle, dwSize, lpData))
    {
      VS_FIXEDFILEINFO *pVersion = (VS_FIXEDFILEINFO *) 0 ;
      UINT uLen = 0;
      if (::VerQueryValue(lpData, TEXT("\\"), (LPVOID*)&pVersion, &uLen))
      {
/*
        sprintf(szVersion, "%hu.%hu.%hu.%hu", HIWORD(pVersion->dwFileVersionMS),
                                              LOWORD(pVersion->dwFileVersionMS),
                                              HIWORD(pVersion->dwFileVersionLS),
                                              LOWORD(pVersion->dwFileVersionLS));
*/
        sprintf(szVersion, "%hu.%02hu.%04hu", HIWORD(pVersion->dwFileVersionMS),
                                              LOWORD(pVersion->dwFileVersionMS),
                                              HIWORD(pVersion->dwFileVersionLS)) ;
      }
    }
    ::LocalFree(lpData);
  }

  return string(szVersion) ;
}

