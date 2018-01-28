/*! -----------------------------------------------------------------------------
* AttValPair.cpp
* -----------------------------------------------------------------------------
* $Revision: 1.8 $
* $Author: pameline $
* $Date: 2015/02/07 11:35:53 $
* -----------------------------------------------------------------------------
* -----------------------------------------------------------------------------
* FLP  - 2002
* -----------------------------------------------------------------------------   */

//#define _DEBUG_

// #include <iostream.h>   // for operator<<(...)

// #include <prtypes.h>

#include <iostream>
#include <string>

#ifndef FOR_WINDOWS
#include <stl.h>
#else
#include <vector.h>
#endif

#ifndef FOR_WINDOWS
#include "AttValPair.h"
#include "nsbb\nspatpat.h"
#else
#include "ns_ob1\AttValPair.h"
#include "nsbb\nspatpat.h"
#endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif

#ifdef __GNUC__
//#pragma implementation
#endif // __GNUC__

using namespace std ;

long AttValPair::_lAVPCount = 0 ;

AttValPair::AttValPair(const string& a, const TypedVal& tv)
           :_att(a)
{
  _lAVPCount++ ;

  _val = tv ; // Because using an initializer didn't work
}

// -----------------------------------------------------------------------------
// constructeur copie -- add fab
// -----------------------------------------------------------------------------

AttValPair::AttValPair(const AttValPair& avp)
           :_att(avp._att)
{
  _lAVPCount++ ;

	_val = avp._val ;
}

AttValPair&
AttValPair::operator=(const AttValPair& avp)
{
	if (this == &avp)
		return *this ;

	_att = avp._att ;
	_val = avp._val ;

	return *this ;
}

void
AttValPair::swapPatho(NSPatPathoArray *patho, bool bExt)
{
	_val.swapPatho(patho, bExt) ;
}

//AttValPair::~AttValPair()
//{
	/*if (AttributeName() == "node")
	{
		NSPatPathoArray *pPatPatho = val.getPatPatho() ;
    if (pPatPatho != NULL)
		  delete pPatPatho ;
	}

	if (AttributeName() == "paths")
	{
		PathsList	*pPaths = val.getPaths() ;
		if (pPaths)
			delete pPaths ; // le vider se fait dans le destructeur
	}       */
 	//val = NULL ;
//} */


// -----------------------------------------------------------------------------
//  Send to the output stream a textual representation of the attribute-value
//  pair
// -----------------------------------------------------------------------------

std::ostream&
operator <<(std::ostream& os, const AttValPair& avp)
{
	os << "(" << avp.AttributeName() << " " << avp.AttributeValue() << ")" ;
  return (os) ;
}

// -----------------------------------------------------------------------------
//  Empty out a set of AttValPair
// -----------------------------------------------------------------------------

void ClearSetOfAVP(AVPSet *avpset)
{
#ifdef _DEBUG_
  cout << "-- DEBUG:: void ClearSetOfAVP(avpset)\n\n" ;
#endif
  if ((NULL == avpset) || avpset->empty())
    return ;

  register unsigned int end = avpset->size() ;
  for (register unsigned int i = 0 ; i < end ; i++)
  {
    try
    {
      AttValPair* temp = (*avpset)[i] ;
      if (temp)
      {
        delete (temp) ;
        temp = NULL ;
      }
    }
    catch(...)
    {
      break ;
    }
  }
	/*for (AVPIter p = avpset->begin() ; p != avpset->end() ; )
	{
    ///
    if (*p != NULL)  // Add by tabun
    delete (*p) ;
    avpset->erase(p) ;
  }    */
	avpset->clear() ;
}

