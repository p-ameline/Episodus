// Fichier nschemin.cpp - Méthodes de la classe NSChemins - RS Janvier 1998///////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "ns_sgbd\nschemin.h"
#include "nautilus\nssuper.h"

//***************************************************************************
//
// Implémentation des méthodes NSCheminsData
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  NSCheminsData::NSCheminsData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSCheminsData::NSCheminsData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCheminsData::NSCheminsData(NSCheminsData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien//---------------------------------------------------------------------------
NSCheminsData::NSCheminsData(const NSCheminsData& rv){
  _sCode     = rv._sCode ;
  _sNom      = rv._sNom ;
  _sChemin   = rv._sChemin ;
  _sSupport  = rv._sSupport ;
  _sType_doc = rv._sType_doc ;
}

//---------------------------------------------------------------------------//  Fonction:		NSCheminsData::operator=(NSCheminsData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSCheminsData&
NSCheminsData::operator=(const NSCheminsData& src)
{
	if (this == &src)
		return *this ;

  _sCode     = src._sCode ;
  _sNom      = src._sNom ;
  _sChemin   = src._sChemin ;
  _sSupport  = src._sSupport ;
  _sType_doc = src._sType_doc ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCheminsData::operator==(NSCheminsData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSCheminsData::operator==(const NSCheminsData& o)
{
	if ((_sCode     == o._sCode)    &&
      (_sNom      == o._sNom)     &&
      (_sChemin   == o._sChemin)  &&
      (_sSupport  == o._sSupport) &&
      (_sType_doc == o._sType_doc))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Function:		NSCheminsData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void
NSCheminsData::metAZero()
{
  _sCode     = string("") ;
  _sNom      = string("") ;
  _sChemin   = string("") ;
  _sSupport  = string("") ;
  _sType_doc = string("") ;
}

//***************************************************************************//
// Implémentation des méthodes NSCheminsInfo
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Fonction:		NSCheminsInfo::NSCheminsInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCheminsInfo::NSCheminsInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSCheminsInfo::NSCheminsInfo(NSChemins*)
//  Description:	Constructeur à partir d'un NSChemins
//  Retour:			Rien
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------//  Fonction:		NSCheminsInfo::~NSCheminsInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCheminsInfo::~NSCheminsInfo()
{
}

//---------------------------------------------------------------------------//  Fonction:		NSCheminsInfo::NSCheminsInfo(NSCheminsInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSCheminsInfo::NSCheminsInfo(const NSCheminsInfo& rv)
              :_Donnees(rv._Donnees)
{
}

//---------------------------------------------------------------------------//  Fonction:		NSCheminsInfo::operator=(NSCheminsInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSCheminsInfo&
NSCheminsInfo::operator=(const NSCheminsInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSCheminsInfo::operator==(NSCheminsInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSCheminsInfo::operator==(const NSCheminsInfo& o)
{
	 return (_Donnees == o._Donnees) ;
}

///////////////////////////////////////////////////////////////////////////////
// Implémentation des méthodes de la classe NSSupports
//
/////////////////////////////////////////////////////////////////////////////


//***************************************************************************
//
// Implémentation des méthodes NSSupportsData
//
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:  NSSupportsData::NSSupportsData()
//
//  Description: Constructeur.
//---------------------------------------------------------------------------
NSSupportsData::NSSupportsData()
{
	metAZero() ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSSupportsData::NSSupportsData(NSSupportsData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSupportsData::NSSupportsData(const NSSupportsData& rv)
{
  _sCode       = rv._sCode ;
  _sNom        = rv._sNom ;
  _sNomServeur = rv._sNomServeur ;
  _sUnite      = rv._sUnite ;
  _sFixe       = rv._sFixe ;
}

//---------------------------------------------------------------------------//  Fonction:		NSSupportsData::operator=(NSSupportsData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSSupportsData&
NSSupportsData::operator=(const NSSupportsData& src)
{
	if (this == &src)
		return *this ;

  _sCode       = src._sCode ;
  _sNom        = src._sNom ;
  _sNomServeur = src._sNomServeur ;
  _sUnite      = src._sUnite ;
  _sFixe       = src._sFixe ;

  return (*this) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSSupportsData::operator==(NSSupportsData& o)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSSupportsData::operator==(const NSSupportsData& o)
{
	if ((_sCode       == o._sCode)       &&
      (_sNom        == o._sNom)        &&
      (_sNomServeur == o._sNomServeur) &&
      (_sUnite      == o._sUnite)      &&
      (_sFixe       == o._sFixe))
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------//  Function:		NSSupportsData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void
NSSupportsData::metAZero()
{
  _sCode       = string("") ;
  _sNom        = string("") ;
  _sNomServeur = string("") ;
  _sUnite      = string("") ;
  _sFixe       = string("") ;
}

//***************************************************************************//
// Implémentation des méthodes NSSupportsInfo
//
//***************************************************************************
//---------------------------------------------------------------------------
//  Fonction:		NSSupportsInfo::NSSupportsInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSupportsInfo::NSSupportsInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSSupportsInfo::~NSSupportsInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSupportsInfo::~NSSupportsInfo()
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSSupportsInfo::NSSupportsInfo(NSSupportsInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSSupportsInfo::NSSupportsInfo(const NSSupportsInfo& rv)
               :_Donnees(rv._Donnees)
{
}

//---------------------------------------------------------------------------
//  Fonction:		NSSupportsInfo::operator=(NSSupportsInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSSupportsInfo&
NSSupportsInfo::operator=(const NSSupportsInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSSupportsInfo::operator==(NSSupportsInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int NSSupportsInfo::operator==(const NSSupportsInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

//***************************************************************************// Implémentation des méthodes NSSupportsArray//***************************************************************************

//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------

NSSupportsArray::NSSupportsArray(const NSSupportsArray& rv)                :NSSupportsVector()
{
	if (false == rv.empty())
		for (SupportsConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSSupportsInfo(*(*i))) ;
}

//---------------------------------------------------------------------------
//  Operateur =
//---------------------------------------------------------------------------

NSSupportsArray&
NSSupportsArray::operator=(const NSSupportsArray& src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
  	for (SupportsConstIter i = src.begin() ; src.end() != i ; i++)    	push_back(new NSSupportsInfo(*(*i))) ;

	return *this ;}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSSupportsArray::vider()
{
	if (empty())
  	return ;

	for (SupportsIter i = begin() ; end() != i ; )
  {
  	delete *i ;
    erase(i) ;
  }
}

NSSupportsArray::~NSSupportsArray(){
	vider() ;
}
boolNSSupportsArray::initFromFile(string sFileName, NSSuper *pSuper){  if ((string("") == sFileName) || (NULL == pSuper))    return false ;try{
  // Note : on utilise sPathBase[0] qui contient le répertoire Perso
  // (initialisé dans ::InitPathBase())
  // string sFichier = sPathBase[0] + string("supports.dat") ;
  vider() ;

  ifstream inFile ;  inFile.open(sFileName.c_str()) ;  if (!inFile)
  {
  	string sErrorText = pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" (") + sFileName + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  string sData = string("") ;
  string line  = string("") ;
  while (!inFile.eof())  {
    getline(inFile, line) ;
    if (string("") != line)
      sData += line + "\n" ;
  }

  inFile.close() ;
  int nbSupports = 0 ;

  // boucle de chargement des attributs de supports.dat  size_t i = 0 ;  while (i < strlen(sData.c_str()))
  {
    // on récupère d'abord l'identifiant du support (1er champ)
    string sNomSupp = string("") ;
    string sFixSupp = string("") ;
    string sValSupp = string("") ;

    while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))
      sNomSupp += pseumaj(sData[i++]) ;

    if ('\n' != sData[i])
    {
      // On saute les blancs et les tabs
      while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])) && ('\n' != sData[i]))
        i++ ;

      if ('\n' != sData[i])
      {
        // on trouve ici le champ fixe (1 caractère : 0 ou 1)
        if (i < strlen(sData.c_str()))
          sFixSupp += sData[i++] ;

        // On saute les blancs et les tabs
        //
        while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])) && ('\n' != sData[i]))          i++ ;

        if ('\n' != sData[i])
        {
          // dernier champ : la valeur du support (path associé)
          while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))            sValSupp += sData[i++] ;
        }
      }
    }

    strip(sNomSupp, stripBoth) ;
    strip(sFixSupp, stripBoth) ;
    strip(sValSupp, stripBoth) ;

    if (strlen(sNomSupp.c_str()) > SUP_CODE_LEN)
      sNomSupp = "" ;
    if (strlen(sFixSupp.c_str()) > SUP_FIXE_LEN)
      sFixSupp = "" ;

    if ((string("") != sNomSupp) && (string("") != sFixSupp) && (string("") != sValSupp))    {      NSSupportsInfo* pSuppInfo = new NSSupportsInfo() ;      pSuppInfo->setCode(sNomSupp) ;      pSuppInfo->setRemovable(sFixSupp) ;      pSuppInfo->setRootPath(sValSupp) ;      push_back(pSuppInfo) ;      nbSupports++ ;    }    else    {      string sErrorText = string("Le fichier supports.dat est endommagé.") ;      pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }    i++ ;  }

  if (0 == nbSupports)
  {
    string sErrorText = string("Le fichier supports.dat est vide.") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception NSSupportsArray::initFromFile.", standardError, 0) ;
  return false ;
}
}boolNSSupportsArray::writeToFile(string sFileName, NSSuper *pSuper){  if ((string("") == sFileName) || (NULL == pSuper))    return false ;  if (empty())    return false ;try{  ofstream outFile ;	outFile.open(sFileName.c_str(), ios::out) ;
  if (!outFile)
  {
  	string sErrorText = pSuper->getText("fileErrors", "errorOpeningOutputFile") ;
    sErrorText += string(" (") + sFileName + string(")") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }  for (SupportsIter it = begin(); end() != it; it++)    outFile << ((*it)->getCode() + string(" ") + (*it)->getRemovable() + string(" ") + (*it)->getRootPath() + string("\n")) ;

  outFile.close() ;

	return true ;
}
catch (...)
{
  erreur("Exception NSSupportsArray::writeToFile.", standardError, 0) ;
  return false ;
}
}

boolNSSupportsArray::setSupport(string sFileName, NSSuper *pSuper, string sSupportName, string sSupportValue, string sSupportRemoval){  if ((string("") == sFileName) || (string("") == sSupportName) ||      (string("") == sSupportValue) || (string("") == sSupportRemoval) ||      (NULL == pSuper))    return false ;  if (false == empty())  {    for (SupportsIter it = begin() ; end() != it ; it++)    {      if ((*it)->getCode() == sSupportName)      {        (*it)->setRemovable(sSupportRemoval) ;        (*it)->setRootPath(sSupportValue) ;        return writeToFile(sFileName, pSuper) ;      }    }  }  if (false == addSupport(sSupportName, sSupportValue, sSupportRemoval))    return false ;  return writeToFile(sFileName, pSuper) ;}boolNSSupportsArray::addSupport(string sSupportName, string sSupportValue, string sSupportRemoval){  if ((string("") == sSupportName) || (string("") == sSupportValue) || (string("") == sSupportRemoval))    return false ;try{  NSSupportsInfo* pSuppInfo = new NSSupportsInfo() ;  pSuppInfo->setCode(sSupportName) ;  pSuppInfo->setRemovable(sSupportRemoval) ;  pSuppInfo->setRootPath(sSupportValue) ;  push_back(pSuppInfo) ;  return true ;}catch (...)
{
  erreur("Exception NSSupportsArray::addSupport.", standardError, 0) ;
  return false ;
}}// Fin du fichier NSChemin.cpp
///////////////////////////////////////

