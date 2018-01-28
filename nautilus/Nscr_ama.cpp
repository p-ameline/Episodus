//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
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
// -------------------------------------------------------------------------// -------------------- METHODES DE NSCRPhraseArray ------------------------
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

//---------------------------------------------------------------------------//  Fonction:		~NSCRPhraseArray()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
void
NSCRPhraseArray::vider()
{
	if (empty())
  	return ;

	for (NSCRPhrArrayIter i = begin() ; end() != i ; )  {
  	delete *i ;
    erase(i) ;
  }
}

NSCRPhraseArray::~NSCRPhraseArray(){
	vider() ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhraseArray::operator=(NSCRPhraseArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------

NSCRPhraseArray&NSCRPhraseArray::operator=(const NSCRPhraseArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())		for (NSCRPhrArrayConstIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSCRPhrase(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSCRPhraseArray = operator.", standardError) ;
	return *this ;
}
}

