//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "partage\nsdivfct.h"#include "nautilus\nscr_amx.h"

//-------------------------------------------------------------------------
//----------------------- METHODES DE NSCRPhraLesArray --------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//  Description:	Constructeur copie
//  Retour:			Rien
//-------------------------------------------------------------------------
/*
NSCRPhraLesArray::NSCRPhraLesArray(NSCRPhraLesArray& rv)
					  :NSCRPhrLsArray()
{
try
{
    if (!(rv.empty()))
        for (NSCRPhrLsArrayIter i = rv.begin(); i != rv.end(); i++)
            push_back(new string(*(*i)));
}
catch (xalloc)
{
    erreur("Mémoire insuffisante.", 0, 0);
}
catch (const exception& e)
{
    string sExept = "Exception " + string(e.what());
    erreur(sExept.c_str(), 0, 0);
}
catch (...)
{
    erreur("Exception.", 0, 0);
}
}

//---------------------------------------------------------------------------
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
void
NSCRPhraLesArray::vider()
{
    if (empty())
        return;

    for (NSCRPhrLsArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSCRPhraLesArray::~NSCRPhraLesArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSLigneArray::operator=(NSLigneArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSCRPhraLesArray&
NSCRPhraLesArray::operator=(NSCRPhraLesArray src)
{
try
{
    vider();
    if (!(src.empty()))
        for (NSCRPhrLsArrayIter i = src.begin(); i != src.end(); i++)
   	        push_back(new string(*(*i)));
	return *this;
}
catch (xalloc)
{
    erreur("Mémoire insuffisante.", 0, 0);
    return *this;
}
catch (const exception& e)
{
    string sExept = "Exception " + string(e.what());
    erreur(sExept.c_str(), 0, 0);
    return *this;
}
catch (...)
{
    erreur("Exception.", 0, 0);
    return *this;
}
}
 */
// -------------------------------------------------------------------------
// ----------------------- METHODES DE NSCRPhraLes -------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction	 : NSCRPhraLes::NSCRPhraLes()
//
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
/*NSCRPhraLes::NSCRPhraLes()
{
	iNumeroLesion = 0;
	cType 		  = ' ';
	cTypeIndex 	  = ' ';
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCRPhraLes::NSCRPhraLes(NSCRPhraLes& rv)
//
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSCRPhraLes::NSCRPhraLes(NSCRPhraLes& rv)
{
	iNumeroLesion = rv.iNumeroLesion;
	cType 		  = rv.cType;
	cTypeIndex 	  = rv.cTypeIndex;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSCRPhraLes::~NSCRPhraLes()
//
//  Description : ~Destructeur
//---------------------------------------------------------------------------
NSCRPhraLes::~NSCRPhraLes()
{
}

//---------------------------------------------------------------------------
//  Fonction	 : NSLigne::operator=(NSLigne src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSCRPhraLes&
NSCRPhraLes::operator=(NSCRPhraLes src)
{
	iNumeroLesion = src.iNumeroLesion;
	cType 		  = src.cType;
	cTypeIndex 	  = src.cTypeIndex;
	return *this;
}

//---------------------------------------------------------------------------
//  Fonction	 : NSLigne::operator == (const NSLigne& o)
//
//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCRPhraLes::operator == (const NSCRPhraLes& o)
{
	 if ((iNumeroLesion == o.iNumeroLesion) &&
		  (cType 		  == o.cType) 		 	 &&
		  (cTypeIndex 	  == o.cTypeIndex))
		 return 1;
	 return 0;
} */
// -------------------------------------------------------------------------// ----------------------- METHODES DE NSCRPhrase --------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhrase::NSCRPhrase()
//
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSCRPhrase::NSCRPhrase()
{
	iNumero = 0;
	sTexte  = "";
}

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhrase::NSCRPhrase(NSCRPhrase& rv)
//
//  Description : Constructeur par défaut
//---------------------------------------------------------------------------
NSCRPhrase::NSCRPhrase(const NSCRPhrase& rv)
{
	iNumero  = rv.iNumero ;
	sTexte 	 = rv.sTexte ;
  aLesions = rv.aLesions ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhrase::~NSCRPhrase()
//
//  Description : ~Destructeur
//---------------------------------------------------------------------------
NSCRPhrase::~NSCRPhrase()
{
}

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhrase::operator=(NSCRPhrase src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSCRPhrase&
NSCRPhrase::operator=(const NSCRPhrase& src)
{
  if (this == &src)
		return *this ;

  iNumero  = src.iNumero ;  sTexte 	 = src.sTexte ;
  aLesions = src.aLesions ;

	return *this ;}

//---------------------------------------------------------------------------//  Fonction	 : NSCRPhrase::operator == (const NSCRPhrase& o)
//
//  Description : Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCRPhrase::operator == (const NSCRPhrase& o)
{
  if (iNumero == o.iNumero)
    return 1 ;
  return 0 ;
}

