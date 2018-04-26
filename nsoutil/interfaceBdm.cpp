// -----------------------------------------------------------------------------
// interfaceBdm.cpp
// -----------------------------------------------------------------------------
// classe interface avec la base bdm
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: fabrice $
// $Date: 2004/05/14 16:56:08 $
// -----------------------------------------------------------------------------
// FLP - mai/juin/juillet 2003
// -----------------------------------------------------------------------------
// Tout ce qui concerne la base bdm doit -- à priori -- être fait à partir de
// cette interface
// -----------------------------------------------------------------------------

#include "nsoutil\ibdm.h"

InterfaceBdm::InterfaceBdm(NSContexte *pCtx)
             :NSRoot(pCtx)
{
	init() ;
}


InterfaceBdm::InterfaceBdm(InterfaceBdm& src)
             :NSRoot(src.pContexte)
{
	init() ;
}


InterfaceBdm::~InterfaceBdm()
{
	close() ;
}


InterfaceBdm&
InterfaceBdm::operator=(InterfaceBdm& src)
{
	pContexte	= src.pContexte ;

	return (*this) ;
}


bool
InterfaceBdm::init()
{
	try
	{
		sIdCpt = "0000" ; // initialisation de l'ID

		pBdm = new NSBdm(pContexte) ;

		pBdm->lastError = pBdm->open() ;
		if (pBdm->lastError != DBIERR_NONE)
		{
			erreur("InterfaceBdm::init() -- Erreur à l'ouverture du fichier bdm.db", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
			delete pBdm ;
			return false ;
		}
		return true ;
	}
	catch (const exception& e)
	{
		string sExcept = "Exception InterfaceBdm::init()." + string(e.what()) ;
		erreur(sExcept.c_str(), standardError, 0) ;
		return false ;
	}
	catch (...)
	{
		erreur("Exception InterfaceBdm::init().", standardError, 0) ;
		return false ;
	}
}

bool
InterfaceBdm::isInDB(string sCodeCIP, string sExtension, string sLexiMedSens, string sCodeVidal, string sLeximedOld)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sLexiMedSens, "CODESLEXIMED", NODEFAULTINDEX, keySEARCHEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isInDB(...) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (DBIERR_RECNOTFOUND == pBdm->lastError)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (DBIERR_NONE != pBdm->lastError)
  {
    erreur("InterfaceBdm::isInDB(...) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (((sCodeCIP    != "")  && (sCodeCIP     == pBdm->getCodeCIP()))   &&
      (                         sExtension   == pBdm->getExtension())  &&
      (                         sLexiMedSens == pBdm->getLeximed())    &&
      ((sCodeVidal   != "") && (sCodeVidal   == pBdm->getCodeVidal())) &&
      ((sLeximedOld  != "") && (sLeximedOld  == pBdm->getLeximedOld())))
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed).", standardError, 0) ;
  return false ;
}
}


bool
InterfaceBdm::isCodeCIPInDB(string sCodeCIP)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sCodeCIP, "CODESCIP", NODEFAULTINDEX, keySEARCHGEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isCodeCIPInDB(...) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isCodeCIPInDB(string sCodeCIP) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->getCodeCIP() == sCodeCIP)
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isCodeCIPInDB(string sCodeCIP)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isCodeCIPInDB(string sCodeCIP).", standardError, 0) ;
  return false ;
}
}

bool
InterfaceBdm::isCodeLexiMedInDB(string sLeximed)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sLeximed, "CODESLEXIMED", NODEFAULTINDEX, keySEARCHEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isCodeLexiMedInDB(string sLeximed) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isCodeLexiMedInDB(string sLeximed) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->getLeximed() == sLeximed)
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed).", standardError, 0) ;
  return false ;
}
}

bool
InterfaceBdm::isCodeVidalInDB(string sCodeVidal)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sCodeVidal, "CODESVIDAL", NODEFAULTINDEX, keySEARCHGEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isCodeVidalInDB(string sCodeVidal) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isCodeVidalInDB(string sCodeVidal) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->getCodeVidal() == sCodeVidal)
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isCodeVidalInDB(string sCodeVidal)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isCodeVidalInDB(string sCodeVidal).", standardError, 0) ;
  return false ;
}
}

bool
InterfaceBdm::isIDInDB(string sID)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sID, "", NODEFAULTINDEX, keySEARCHEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isIDInDB(string sID) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isIDInDB(string sID) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->getId() == sID)
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isIDInDB(string sID)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isIDInDB(string sID).", standardError, 0) ;
  return false ;
}
}

bool
InterfaceBdm::isCodeLexiMedOldInDB(string sLeximedOld)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sLeximedOld, "CODESLEXIMEDOLD", NODEFAULTINDEX, keySEARCHGEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isCodeLexiMedOldInDB(string sLeximedOld) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return false ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isCodeLexiMedOldInDB(string sLeximedOld) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  if (pBdm->getLeximedOld() == sLeximedOld)
    return true ;
  else
    return false ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::isCodeLexiMedOldInDB(string sLeximedOld)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::isCodeLexiMedOldInDB(string sLeximedOld).", standardError, 0) ;
  return false ;
}
}

bool
InterfaceBdm::updateCipForLeximed(string sCodeCIP, string sLeximed)
{
  if (string("") == sLeximed)
    return false ;

  try
	{
		pBdm->lastError = pBdm->chercheClef(&sLeximed, "CODESLEXIMED", NODEFAULTINDEX, keySEARCHEQ, dbiREADLOCK) ;
		if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
		{
			erreur("InterfaceBdm::updateCipForLeximed -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
			return false ;
		}

		if (pBdm->lastError == DBIERR_RECNOTFOUND)
			return false ;

    pBdm->lastError = pBdm->getRecord() ;
    if (pBdm->lastError != DBIERR_NONE)
    {
      erreur("InterfaceBdm::updateCipForLeximed -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    if (pBdm->getLeximed() != sLeximed)
    {
      erreur("InterfaceBdm::updateCipForLeximed -- erreur de cohérence à la récupération de l'enregistrement", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    pBdm->setCodeCIP(sCodeCIP) ;

    pBdm->lastError = pBdm->modifyRecord(TRUE) ;
    if (pBdm->lastError != DBIERR_NONE)
    {
      erreur("InterfaceBdm::updateCipForLeximed -- erreur à la mise à jour de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    return true ;
	}
	catch (const exception& e)
	{
		string sExcept = "Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed)." + string(e.what()) ;
		erreur(sExcept.c_str(), standardError, 0) ;
		return false ;
	}
	catch (...)
	{
		erreur("Exception InterfaceBdm::isCodeLexiMedInDB(string sLeximed).", standardError, 0) ;
		return false ;
	}
}

string
InterfaceBdm::getLeximedForCIP(string sCodeCIP)
{
try
{
  pBdm->lastError = pBdm->chercheClef(&sCodeCIP, "CODESCIP", NODEFAULTINDEX, keySEARCHGEQ, dbiREADLOCK) ;
  if ((pBdm->lastError != DBIERR_NONE) && (pBdm->lastError != DBIERR_RECNOTFOUND))
  {
    erreur("InterfaceBdm::isCodeCIPInDB(...) -- erreur à la recherche dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return string("") ;
  }

  if (pBdm->lastError == DBIERR_RECNOTFOUND)
    return string("") ;

  pBdm->lastError = pBdm->getRecord() ;
  if (pBdm->lastError != DBIERR_NONE)
  {
    erreur("InterfaceBdm::isCodeCIPInDB(string sCodeCIP) -- erreur à la récupération de l'enregistrement", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return string("") ;
  }

  if (pBdm->getCodeCIP() == sCodeCIP)
    return pBdm->getLeximed() ;

  return string("") ;
}
catch (const exception& e)
{
  string sExcept = "Exception InterfaceBdm::getLeximedForCIP(string sCodeCIP)." + string(e.what()) ;
  erreur(sExcept.c_str(), standardError, 0) ;
  return string("") ;
}
catch (...)
{
  erreur("Exception InterfaceBdm::getLeximedForCIP(string sCodeCIP).", standardError, 0) ;
  return string("") ;
}
}

bool
InterfaceBdm::insertElem(string sCodeCIP, string sExtension, string sLeximed, string sCodeVidal, string sLeximedOld)
{
  if (string("") == sLeximed)
    return false ;

	try
	{
		string	sId	= getNewFreeID() ;

		pBdm->setId(sId) ;
		pBdm->setCodeCIP(sCodeCIP) ;
		pBdm->setExtension(sExtension) ;
		pBdm->setLeximed(sLeximed) ;
		pBdm->setCodeVidal(sCodeVidal) ;
		pBdm->setLeximedOld(sLeximedOld) ;

		// on l'insère dans la base Messages
		pBdm->lastError = pBdm->appendRecord() ;
		if (pBdm->lastError != DBIERR_NONE)
		{
			erreur("InterfaceBdm::insertElem(...) -- Erreur à l'insertion du message dans la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
			return false ;
		}
		return true ;
	}
	catch (const exception& e)
	{
		string sExcept = "Exception InterfaceBdm::insertElem(...)." + string(e.what()) ;
		erreur(sExcept.c_str(), standardError, 0) ;
		return false ;
	}
	catch (...)
	{
		erreur("Exception InterfaceBdm::insertElem(...)..", standardError, 0) ;
		return false ;
	}
}

string
InterfaceBdm::getNewFreeID()
{
	string	sID	= sIdCpt ;

	int		iUnit		= 3 ;

	while (isIDInDB(sID))
	{
		int		iDigit	= 3 ;
		bool	tourner	= true ;

		(sID[iUnit])++ ;
		while ((iDigit >= 0) && tourner)
		{
			if ((sID[iDigit] > '9') && (sID[iDigit] < 'A'))
			{
				sID[iDigit] = 'A' ;
				tourner = false ;
			}
			else
			{
				if (sID[iDigit] > 'Z')
				{
					sID[iDigit] = '0' ;
					iDigit-- ;
					(sID[iDigit])++ ;
				}
				else
					tourner = false ;
			}
		}
	}

	sIdCpt = sID ;
	return (sID) ;
}

bool
InterfaceBdm::close()
{
	try
	{
		pBdm->lastError = pBdm->close() ;
		if (pBdm->lastError != DBIERR_NONE)
			erreur("InterfaceBdm::close() -- Erreur de fermeture de la base bdm.db.", standardError, pBdm->lastError, pContexte->GetMainWindow()->GetHandle()) ;
		delete pBdm ;

		return true ;
	}
	catch (const exception& e)
	{
		string sExcept = "Exception InterfaceBdm::close()." + string(e.what()) ;
		erreur(sExcept.c_str(), standardError, 0) ;
		return false ;
	}
	catch (...)
	{
		erreur("Exception InterfaceBdm::close().", standardError, 0) ;
		return false ;
	}
}

