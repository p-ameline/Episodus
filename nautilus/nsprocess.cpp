// -----------------------------------------------------------------------------
// nsprocess.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2015/02/07 11:34:44 $
// -----------------------------------------------------------------------------
// classe représentant les processes
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------


#include "nautilus\nsprocess.h"


// -----------------------------------------------------------------------------
// constructeur par défaut
// -----------------------------------------------------------------------------
NSProcess::NSProcess(NSContexte *pCtx)
          :NSRoot(pCtx)
{
  // variables
  sTitre          = "" ;
  sReference      = "" ;
  sMetaReference  = "" ;
  sLexique        = "" ;
  tDateDemande.init() ;
  tDateOuverture.init() ;
  tDateFermeture.init() ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// -----------------------------------------------------------------------------
NSProcess::NSProcess(const NSProcess& rv)
          :NSRoot(rv.pContexte)
{
  // variables
  sTitre          = rv.sTitre ;
  sReference      = rv.sReference ;
  sMetaReference  = rv.sMetaReference ;
  sLexique        = rv.sLexique ;
  tDateDemande    = rv.tDateDemande ;
  tDateOuverture  = rv.tDateOuverture ;
  tDateFermeture  = rv.tDateFermeture ;
  aConcerns       = rv.aConcerns ;
}

// -----------------------------------------------------------------------------
// destructeur
// -----------------------------------------------------------------------------
NSProcess::~NSProcess()
{
}

// -----------------------------------------------------------------------------
// operateur =
// -----------------------------------------------------------------------------
NSProcess&
NSProcess::operator=(const NSProcess& src)
{
	if (this == &src)
		return *this ;

  // variables
  sTitre          = src.sTitre ;
  sReference      = src.sReference ;
  sMetaReference  = src.sMetaReference ;
  sLexique        = src.sLexique ;
  tDateDemande    = src.tDateDemande ;
  tDateOuverture  = src.tDateOuverture ;
  tDateFermeture  = src.tDateFermeture ;
  aConcerns       = src.aConcerns ;

  return (*this) ;
}

bool
NSProcess::isLinkedWithConcern(string sConcern)
{
  if (aConcerns.empty())
    return false ;

  for (EquiItemIter i = aConcerns.begin() ; i != aConcerns.end() ; i++)
    if (*(*i) == sConcern)
      return true ;

  return false ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// -----------------------------------------------------------------------------
ArrayProcesses::ArrayProcesses(const ArrayProcesses& src)
               :ProcessesVector()
{
try
{
  // do not forget NSLdvDoc *pDoc
  pDoc = src.pDoc ;

  if (false == src.empty())
    for (ProcessesConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSProcess(**i)) ;
}
catch (...)
{
	erreur("Exception ArrayProcesses copy ctor.", standardError, 0) ;
}
}

ArrayProcesses::~ArrayProcesses()
{
  vider() ;
}

ArrayProcesses&
ArrayProcesses::operator=(const ArrayProcesses& src)
{
try
{
	if (this == &src)
		return *this ;

	pDoc = src.pDoc ;

	vider() ;

	if (false == src.empty())
		for (ProcessesConstIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSProcess(**i)) ;

	return (*this) ;
}
catch (...)
{
	erreur("Exception ArrayProcesses (=).", standardError, 0) ;
	return (*this) ;
}
}


void
ArrayProcesses::vider()
{
	if (empty())
		return ;

	for (ProcessesIter i = begin() ; i != end() ; )
	{
		delete *i ;
		erase(i) ;
	}
}


/*
void
ArrayProcesses::initialiser()
{
}


void
ArrayProcesses::reinit()
{
}


void
ArrayProcesses::deleteProcess(NSProcess *pProcess)
{
}


NSProcess
ArrayProcesses::*getProcess(string sRef)
{
}

*/

