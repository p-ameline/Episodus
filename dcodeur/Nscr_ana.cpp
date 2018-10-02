//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\dc.h>
#include <owl\gdiobjec.h>
#include <classlib\arrays.h>

#include "partage\nsdivfct.h"#include "nautilus\nscr_ana.h"

// -------------------------------------------------------------------------
// ---------------------- METHODES DES NSLigneArray ------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSLigneArray(NSLigneArray& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
/*
NSLigneArray::NSLigneArray(NSLigneArray& rv)
				 :NSLignesArray()
{
try
{
    if (!(rv.empty()))
        for (NSLignesArrayIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSLigne(*(*i)));
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
//  Fonction:		~NSLigneArray()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
void
NSLigneArray::vider()
{
    if (empty())
        return;

    for (NSLignesArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}
NSLigneArray::~NSLigneArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Fonction	 : NSLigneArray::operator=(NSLigneArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSLigneArray&
NSLigneArray::operator=(NSLigneArray src)
{
try
{
    vider();

    if (!(src.empty()))
        for (NSLignesArrayIter i = src.begin(); i != src.end(); i++)
            push_back(new NSLigne(*(*i)));
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


// -------------------------------------------------------------------------
// ---------------------- METHODES DES NSRectArray -------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Fonction:		NSRectArray(NSRectArray& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSRectArray::NSRectArray(NSRectArray& rv)
            :NSRectsArray()
{
try
{
    if (!(rv.empty()))
        for (NSRectsArrayIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NS_CLASSLIB::TRect(*(*i)));
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
//  Fonction:		~NSRectArray()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
void
NSRectArray:: vider()
{
    if (empty())
        return;

    for(NSRectsArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

NSRectArray::~NSRectArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSRectArray&
NSRectArray::operator=(NSRectArray src)
{
try
{
    vider();
    if (!(src.empty()))
        for (NSRectsArrayIter i = src.begin(); i != src.end(); i++)
            push_back(new NS_CLASSLIB::TRect(*(*i)));
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
}       */

