//----------------------------------------------------------------------------
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\dc.h>
#include <owl\gdiobjec.h>
#include <classlib\arrays.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif

#include "nautilus\nscr_ama.h"
// -------------------------------------------------------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCRPhraseArray::NSCRPhraseArray(const NSCRPhraseArray& rv)
                :NSCRPhrArray()
{
try
{
	if (false == rv.empty())
		for (NSCRPhrArrayConstIter i = rv.begin() ; rv.end() != i ; i++)
    	push_back(new NSCRPhrase(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSCRPhraseArray copy ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
void
NSCRPhraseArray::vider()
{
	if (empty())
  	return ;

	for (NSCRPhrArrayIter i = begin() ; end() != i ; )
  	delete *i ;
    erase(i) ;
  }
}

NSCRPhraseArray::~NSCRPhraseArray()
	vider() ;
}

//---------------------------------------------------------------------------
//
//  Description : Op�rateur d'affectation
//---------------------------------------------------------------------------

NSCRPhraseArray&
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
    	push_back(new NSCRPhrase(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSCRPhraseArray = operator.", standardError) ;
	return *this ;
}
}
