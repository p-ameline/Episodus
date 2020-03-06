// -----------------------------------------------------------------------------// nsdivfct.cpp
// -----------------------------------------------------------------------------
// Pot-pourri de fonctions
// -----------------------------------------------------------------------------
// $Revision: 1.6 $
// $Author: pameline $
// $Date: 2016/08/19 06:35:59 $
// -----------------------------------------------------------------------------
// PA  - 3 octobre 2010
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#include <stdio.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include "partage\nsglobal.h"
  #include "nautilus\nssuper.h"
#endif

#include "partage\nsdivfile.h"

// -----------------------------------------------------------------------------
// Function    : NsFileExists(const char *ammorce, const char* suffixe)
// Arguments   :	ammorce  : premiers caractères du préfixe du nom DOS
//				  			suffixe  : suffixe du nom de fichier DOS
//				  			chemin   : répertoire où chercher
//				  			nomLibre : nom du fichier trouvé sous forme PPPPPPPP.SSS
// Description : Cherche le premier nom de fichier libre
// Returns     : RIEN
// -----------------------------------------------------------------------------
bool
NsFileExists(const std::string sFileName)
{
	DWORD fileAttr = GetFileAttributes(sFileName.c_str()) ;

  if (0xFFFFFFFF == fileAttr)
    return false ;

  return true ;
}
boolNsFileExists2(const string sFileNameAndPath){
  WIN32_FIND_DATA findFileData ;
	HANDLE hFileFound = FindFirstFile(sFileNameAndPath.c_str(), &findFileData) ;

  if (INVALID_HANDLE_VALUE == hFileFound)
    return false ;

  FindClose(hFileFound) ;
  return true ;
}voidNsParseFileName(const string sCompleteFileName, string &sPath, string &sName){  sPath = string("") ;  sName = string("") ;  size_t iLen = strlen(sCompleteFileName.c_str()) ;  if (0 == iLen)    return ;  // Récupération du path  //
	size_t pos1 = sCompleteFileName.find_last_of(FILESLASH) ;

	if ((NPOS == pos1) || (iLen - 1 == pos1))
		return ;

	sPath = string(sCompleteFileName, 0, pos1 + 1) ;
  sName = string(sCompleteFileName, pos1 + 1, iLen - pos1 - 1) ;}voidNsParseExtension(const string sCompleteFileName, string &sRoot, string &sExtension){  sRoot      = string("") ;  sExtension = string("") ;  size_t iLen = strlen(sCompleteFileName.c_str()) ;  if (0 == iLen)    return ;  // Récupération du path  //
	size_t pos1 = sCompleteFileName.find_last_of('.') ;

	if ((NPOS == pos1) || (iLen - 1 == pos1))
		return ;

	sRoot      = string(sCompleteFileName, 0, pos1 + 1) ;
  sExtension = string(sCompleteFileName, pos1 + 1, iLen - pos1 - 1) ;}boolNsDeleteTemporaryFile(NSSuper *pSuper, const string sFileName){  if (string("") == sFileName)    return true ;  if (::DeleteFile(sFileName.c_str()))
    return true ;

  DWORD  dwErrorCode = GetLastError() ;
  string sErrorText  = pSuper->getText("fileErrors", "errorDeletingTemporaryFile") + string(" ") + sFileName ;

  if      (ERROR_FILE_NOT_FOUND == dwErrorCode)
    sErrorText += string(" (file not found)") ;
  else if (ERROR_ACCESS_DENIED == dwErrorCode)
    sErrorText += string(" (access denied)") ;

  pSuper->trace(&sErrorText, 1) ;

#ifndef _ENTERPRISE_DLL
  if (ERROR_FILE_NOT_FOUND != dwErrorCode)
    erreur(sErrorText.c_str(), standardError, 0, pSuper->donneMainWindow()->GetHandle()) ;
#endif

  return false ;
}
//// Fournit (rapidement) le contenu fidèle d'un fichier sous forme d'une string
//stringgetFileContent(const NSSuper *pSuper, const string sFileName){
  ifstream in(sFileName.c_str(), std::ios::in | std::ios::binary) ;
  if (in)
  {
    string contents ;
    in.seekg(0, ios::end) ;
    contents.resize(in.tellg()) ;
    in.seekg(0, ios::beg) ;
    in.read(&contents[0], contents.size()) ;
    in.close() ;
    return(contents) ;
  }
  else if (pSuper)
  {
    string sErrorText = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sFileName ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
  }
  return string("") ;
}
string getFileError(){  return string(strerror(errno)) ;}string getErrorText(LONG lOsErr){  // NT Error codes	switch ((UINT)lOsErr)
	{
    case NO_ERROR:
      return string("") ;
    case ERROR_FILE_NOT_FOUND:
      return string("File not found") ;
    case ERROR_PATH_NOT_FOUND:
      return string("Path not found") ;
    case ERROR_TOO_MANY_OPEN_FILES:
      return string("Too many open files") ;
    case ERROR_ACCESS_DENIED:
      return string("Access denied") ;
    case ERROR_INVALID_HANDLE:
      return string("Invalid handle") ;
    case ERROR_BAD_FORMAT:
      return string("Bad format");
    case ERROR_INVALID_ACCESS:
      return string("Invalid access");
    case ERROR_INVALID_DRIVE:
      return string("Invalid drive");
    case ERROR_CURRENT_DIRECTORY:
      return string("Remove current directory");
    case ERROR_NOT_SAME_DEVICE:
      return string("Bad path, not same device");
    case ERROR_NO_MORE_FILES:
      return string("File not found, no more file");
    case ERROR_WRITE_PROTECT:
      return string("Access denied, write protect");
    case ERROR_BAD_UNIT:
      return string("Bad unit");
    case ERROR_NOT_READY:
      return string("Not ready");
    case ERROR_BAD_COMMAND:
      return string("Bad command");
    case ERROR_CRC:
      return string("Wrong CRC");
    case ERROR_BAD_LENGTH:
      return string("Bad length");
    case ERROR_SEEK:
      return string("Bad seek");
    case ERROR_NOT_DOS_DISK:
      return string("Not DOS disk");
    case ERROR_SECTOR_NOT_FOUND:
      return string("Sector not found");
    case ERROR_WRITE_FAULT:
      return string("Write fault");
    case ERROR_READ_FAULT:
      return string("Read fault");
    case ERROR_SHARING_VIOLATION:
      return string("Sharing violation");
    case ERROR_LOCK_VIOLATION:
      return string("Lock violation");
    case ERROR_WRONG_DISK:
      return string("Wrong disk");
    case ERROR_SHARING_BUFFER_EXCEEDED:
      return string("Sharing buffer exceeded");
    case ERROR_HANDLE_EOF:
      return string("Handle end of file");
    case ERROR_HANDLE_DISK_FULL:
      return string("Disk full");
    case ERROR_DUP_NAME:
      return string("Duplicate name");
    case ERROR_BAD_NETPATH:
      return string("Bad net path");
    case ERROR_NETWORK_BUSY:
      return string("Network busy");
    case ERROR_DEV_NOT_EXIST:
      return string("Dev not exist");
    case ERROR_ADAP_HDW_ERR:
      return string("Adap hdw error");
    case ERROR_BAD_NET_RESP:
      return string("Bad net resp");
    case ERROR_UNEXP_NET_ERR:
      return string("Unexpected network error");
    case ERROR_BAD_REM_ADAP:
      return string("Bad rem adap");
    case ERROR_NO_SPOOL_SPACE:
      return string("No spool space");
    case ERROR_NETNAME_DELETED:
      return string("Netname deleted");
    case ERROR_NETWORK_ACCESS_DENIED:
      return string("Network access denied");
    case ERROR_BAD_DEV_TYPE:
      return string("Bad dev type");
    case ERROR_BAD_NET_NAME:
      return string("Bad net name");
    case ERROR_TOO_MANY_NAMES:
      return string("Too many names");
    case ERROR_SHARING_PAUSED:
      return string("Sharing paused");
    case ERROR_REQ_NOT_ACCEP:
      return string("Req not accep") ;
    case ERROR_FILE_EXISTS:
      return string("File exists");
    case ERROR_CANNOT_MAKE:
      return string("Cannot make");
    case ERROR_ALREADY_ASSIGNED:
      return string("Already assigned") ;
    case ERROR_INVALID_PASSWORD:
      return string("Invalid password");
    case ERROR_NET_WRITE_FAULT:
      return string("Network write fault");
    case ERROR_DISK_CHANGE:
      return string("Disk change");
    case ERROR_DRIVE_LOCKED:
      return string("Drive locked");
    case ERROR_BUFFER_OVERFLOW:
      return string("Buffer overflow") ;
    case ERROR_DISK_FULL:
      return string("Disk full");
    case ERROR_NO_MORE_SEARCH_HANDLES:
      return string("No more search handles") ;
    case ERROR_INVALID_TARGET_HANDLE:
      return string("Invalid target handle") ;
    case ERROR_INVALID_CATEGORY:
      return string("Invalid category") ;
    case ERROR_INVALID_NAME:
      return string("Invalid name") ;
    case ERROR_INVALID_LEVEL:
      return string("Invalid level") ;
    case ERROR_NO_VOLUME_LABEL:
      return string("No volume label") ;
    case ERROR_NEGATIVE_SEEK:
      return string("Negative seek") ;
    case ERROR_SEEK_ON_DEVICE:
      return string("Seek on device") ;
    case ERROR_DIR_NOT_ROOT:
      return string("Directory not found") ;
    case ERROR_DIR_NOT_EMPTY:
      return string("Directory not empty") ;
    case ERROR_LABEL_TOO_LONG:
      return string("Label too long") ;
    case ERROR_BAD_PATHNAME:
      return string("Bad path name") ;
    case ERROR_LOCK_FAILED:
      return string("Lock failed") ;
    case ERROR_BUSY:
      return string("Busy") ;
    case ERROR_INVALID_ORDINAL:
      return string("Invalid ordinal") ;
    case ERROR_ALREADY_EXISTS:
      return string("Already exists") ;
    case ERROR_INVALID_EXE_SIGNATURE:
      return string("Invalif exe signature") ;
    case ERROR_BAD_EXE_FORMAT:
      return string("Bad exe format") ;
    case ERROR_FILENAME_EXCED_RANGE:
      return string("Exced range") ;
    case ERROR_META_EXPANSION_TOO_LONG:
      return string("Expansion too long") ;
    case ERROR_DIRECTORY:
      return string("Bad directory") ;
    case ERROR_OPERATION_ABORTED:
      return string("Operation aborted") ;
    case ERROR_IO_INCOMPLETE:
      return string("IO incomplete") ;
    case ERROR_IO_PENDING:
      return string("IO pending") ;
    case ERROR_SWAPERROR:
      return string("Swap error") ;
    default:
      return string("") ;
	}}