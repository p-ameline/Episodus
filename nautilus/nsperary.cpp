//---------------------------------------------------------------------------//    NSPERARY.CPP
//    PA   juillet 94
//  Implémentation des Arrays d'objets annexes de NAUTILUS
//---------------------------------------------------------------------------
#include "partage\Nsglobal.h"
#include "partage\nsdivfct.h"

#include "nautilus\nsperary.h"
#include "partage\nsperson.h"

//***************************************************************************
// Implémentation des méthodes NSPatientArray
//***************************************************************************
/*
//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPatientArray::NSPatientArray(NSPatientArray& rv)
					:NSPatInfoArray()
{
try
{
    if (!(rv.empty()))
	    for (PatientIter i = rv.begin(); i != rv.end(); i++)
    	    push_back(new NSPatInfo(*(*i))) ;
}
catch (...)
{
    erreur("Exception NSPatientArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSPatientArray::vider()
{
    if (empty())
        return ;

    for (PatientIter i = begin(); i != end();)
    {
   	    delete *i ;
        erase(i) ;
    }
}

NSPatientArray::~NSPatientArray( ){
	vider();
}        */

//***************************************************************************
// 				Implémentation des méthodes NSCorrespArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCorrespArray::NSCorrespArray(const NSCorrespArray& rv)
               :NSCorrespInfoArray()
{
try
{
  if (false == rv.empty())
    for (CorrespInfoConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSCorrespondantInfo(*(*i))) ;
}
catch (...)
{
  erreur("Exception NSCorrespArray copy ctor.", standardError, 0) ;
}
}

NSCorrespArray&NSCorrespArray::operator=(const NSCorrespArray& src)
{
try
{
  if (&src == this)
		return *this ;

	vider() ;

  if (false == src.empty())
    for (CorrespInfoConstIter i = src.begin() ; src.end() != i ; i++)
      push_back(new NSCorrespondantInfo(**i)) ;

  return *this ;
}
catch (...)
{
  erreur("Exception NSCorrespArray = op.", standardError, 0) ;
  return *this ;
}
}

intNSCorrespArray::operator==(const NSCorrespArray& o )
{
  if ((empty()) || (o.empty()))
  {
    if ((empty()) && (o.empty()))
      return 1 ;
    else
      return 0 ;
  }

  CorrespInfoConstIter i, j ;
	int egal = 1 ;

  for (i = begin(), j = o.begin() ; (end() != i) && (o.end() != j) ; i++, j++)
  {
    if (!( (*(*i)) == (*(*j)) ))
    {
      egal = 0 ;
      break ;
    }  }
  if ((end() != i) || (o.end() != j))
    egal = 0 ;

   return egal ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSCorrespArray::vider()
{
  if (empty())
    return ;

  for (CorrespInfoIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSCorrespArray::~NSCorrespArray(){
	vider() ;
}

//***************************************************************************
// 				Implémentation des méthodes NSUtiliArray
//***************************************************************************
         /*
//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSUtiliArray::NSUtiliArray(NSUtiliArray& rv)
					:NSUtiliInfoArray()
{
try
{
    if (!(rv.empty()))
	    for (UtiliInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSUtiliInfo(*(*i)));
}
catch (...)
{
    erreur("Exception NSUtiliArray copy ctor.", standardError, 0) ;
}
}

NSUtiliArray&
NSUtiliArray::operator=(NSUtiliArray src)
{
try
{
	vider();

    if (!(src.empty()))
	    for (UtiliInfoIter i = src.begin(); i != src.end(); i++)
   	        push_back(new NSUtiliInfo(*(*i)));

    return *this;
}
catch (...)
{
    erreur("Exception NSUtiliArray = op.", standardError, 0) ;
    return *this ;
}
}

int
NSUtiliArray::operator == ( NSUtiliArray& o )
{
    if ((empty()) || (o.empty()))
    {
        if ((empty()) && (o.empty()))
            return 1 ;
        else
            return 0 ;
    }

	UtiliInfoIter i,j;
	int egal = 1;

	for (i = begin(), j = o.begin(); (i != end()) && (j != o.end()); i++, j++)
    {
   	    if (!( (*(*i)) == (*(*j)) ))
        {
      	    egal = 0;
            break;
        }
    }
    if ((i != end()) || (j != o.end()))
        egal = 0;

    return egal;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSUtiliArray::vider()
{
    if (empty())
        return ;

	for (UtiliInfoIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSUtiliArray::~NSUtiliArray()
{
	vider();
}      */

//***************************************************************************
// 				Implémentation des méthodes NSAdressesArray
//***************************************************************************

#ifndef N_TIERS
   /*
//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSAdressesArray::NSAdressesArray(NSAdressesArray& rv)
					:NSAdressesInfoArray()
{
try
{
    if (!(rv.empty()))
	    for (AdressesInfoIter i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSAdressesInfo(*(*i)));
}
catch (...)
{
    erreur("Exception NSAdressesArray copy ctor.", standardError, 0) ;
}
}

NSAdressesArray&
NSAdressesArray::operator=(NSAdressesArray src)
{
try
{
	vider();

    if (!(src.empty()))
	    for (AdressesInfoIter i = src.begin(); i != src.end(); i++)
   	        push_back(new NSAdressesInfo(*(*i))) ;

    return *this ;
}
catch (...)
{
    erreur("Exception NSAdressesArray = operator.", standardError, 0) ;
    return *this ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSAdressesArray::vider()
{
    if (empty())
        return ;

    for (AdressesInfoIter i = begin(); i != end(); )
    {
   	    delete *i ;
        erase(i) ;
    }
}

NSAdressesArray::~NSAdressesArray()
{
	vider() ;
}
  */
#endif

///////////////////////// fin de nsperary.cpp ////////////////////////////////

