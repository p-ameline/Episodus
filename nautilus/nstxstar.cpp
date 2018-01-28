//----------------------------------------------------------------------------
// NSTXSTYL.CPP
// Styles utilisés dans le formatage de documents
//
// PA Septembre 1994
//----------------------------------------------------------------------------
#include <owl\dc.h>
#include <owl\gdiobjec.h>

#ifdef _INCLUS
  #include "partage\Nsglobal.h"
  #include "nsbb\nsutidlg.h"
#endif

#include "partage\nsdivfct.h"
#include "nautilus\nsannexe.h"
#include "nautilus\nstxstar.h"

// -------------------------------------------------------------------------
// ---------------------- METHODES DES NSFontArray -------------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
/*
NSFontArray::NSFontArray(NSFontArray& rv)
            :NSFontsArray()
{
try
{
    if (!(rv.empty()))
   	    for (FontIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSFont(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSFontArray::~NSFontArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSFontArray&
NSFontArray::operator=(NSFontArray src)
{
try
{
    vider();
	if (!(src.empty()))
   	    for (FontIter i = src.begin(); i != src.end(); i++)
      	    push_back(new NSFont(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSFontArray::operator==(NSFontArray& o)
{
    FontIter i,j;

	if(!(o.empty()))
    {
        if( (o.size() == size()) )
        {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSFontArray::vider()
{
    if (empty())
        return;

    for (FontIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -------------------------------------------------------------------------
// ------------------- METHODES DES NSStyleCadreArray ----------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSStyleCadreArray::NSStyleCadreArray(NSStyleCadreArray& rv)
                  :NSStylCadrArray()
{
try
{
    if (!(rv.empty()))
   	    for (StylCadrIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSStyleCadre(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSStyleCadreArray::~NSStyleCadreArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleCadreArray&
NSStyleCadreArray::operator=(NSStyleCadreArray src)
{
try
{
	vider();
	if (!(src.empty()))
   	    for (StylCadrIter i = src.begin(); i != src.end(); i++)
   		    push_back(new NSStyleCadre(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleCadreArray::operator==(NSStyleCadreArray& o)
{
    StylCadrIter i,j;

	if(!(o.empty()))
    {
        if( (o.size() == size()) )
        {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSStyleCadreArray::vider()
{
    if (empty())
        return;
	for (StylCadrIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -------------------------------------------------------------------------
// ----------------- METHODES DES NSStyleParagrapheArray -------------------
// -------------------------------------------------------------------------

//--------------------------------------------------------------------------
//  Constructeur copie
//--------------------------------------------------------------------------
NSStyleParagrapheArray::NSStyleParagrapheArray(NSStyleParagrapheArray& rv)
                       :NSStylParaArray()
{
try
{
    if (!(rv.empty()))
   	    for ( NSStylParaArrayIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSStyleParagraphe(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSStyleParagrapheArray::~NSStyleParagrapheArray()
{
	vider();
}


//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleParagrapheArray&
NSStyleParagrapheArray::operator=(NSStyleParagrapheArray src)
{
try
{
	vider();
	if (!(src.empty()))
   	    for (NSStylParaArrayIter i = src.begin(); i != src.end(); i++)
      	    push_back(new NSStyleParagraphe(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleParagrapheArray::operator==(NSStyleParagrapheArray& o)
{
	NSStylParaArrayIter i,j;

	if(!(o.empty()))
    {
        if( (o.size() == size()) )
   	    {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSStyleParagrapheArray::vider()
{
    if (empty())
        return;

    for (NSStylParaArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -------------------------------------------------------------------------
// -------------------- METHODES DES NSStyleDecorArray ---------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSStyleDecorArray::NSStyleDecorArray(NSStyleDecorArray& rv)
                  :NSNSStylDecoArray()
{
try
{
    if (!(rv.empty()))
   	    for ( NSStylDecoArrayIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSStyleDecor(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSStyleDecorArray::~NSStyleDecorArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSStyleDecorArray&
NSStyleDecorArray::operator=(NSStyleDecorArray src)
{
try
{
	vider();
	if (!(src.empty()))
		for (NSStylDecoArrayIter i = src.begin(); i != src.end(); i++)
      	push_back(new NSStyleDecor(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSStyleDecorArray::operator==(NSStyleDecorArray& o)
{
  	NSStylDecoArrayIter i,j;

	if(!(o.empty()))
    {
        if( (o.size() == size()) )
        {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSStyleDecorArray::vider()
{
    if (empty())
        return;

    for (NSStylDecoArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -------------------------------------------------------------------------
// --------------------- METHODES DE NSCadreDecorArray ---------------------
// -------------------------------------------------------------------------

//--------------------------------------------------------------------------
//  Constructeur copie
//--------------------------------------------------------------------------
NSCadreDecorArray::NSCadreDecorArray(NSCadreDecorArray& rv)
						:NSCadreDecoArray()
{
try
{
    if (!(rv.empty()))
        for ( CadreDecoIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSCadreDecor(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSCadreDecorArray::~NSCadreDecorArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCadreDecorArray&
NSCadreDecorArray::operator=(NSCadreDecorArray src)
{
try
{
    vider();
	if (!(src.empty()))
   	    for (CadreDecoIter i = src.begin(); i != src.end(); i++)
      	    push_back(new NSCadreDecor(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCadreDecorArray::operator==(NSCadreDecorArray& o)
{
	CadreDecoIter i,j;
	if (!(o.empty()))
    {
   	    if( (o.size() == size()) )
        {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSCadreDecorArray::vider()
{
    if (empty())
        return;

    for (CadreDecoIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -------------------------------------------------------------------------
// ------------------------ METHODES DES NSPageArray -----------------------
// -------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSPageArray::NSPageArray(NSPageArray& rv)
            :NSStylPageArray()
{
try
{
    if (!(rv.empty()))
   	    for (NSStylPageArrayIter i = rv.begin(); i != rv.end(); i++)
      	    push_back(new NSPage(*(*i)));
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
//  Destructeur
//---------------------------------------------------------------------------
NSPageArray::~NSPageArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSPageArray&
NSPageArray::operator=(NSPageArray src)
{
try
{
	vider();
	if (!(src.empty()))
   	    for (NSStylPageArrayIter i = src.begin(); i != src.end(); i++)
      	    push_back(new NSPage(*(*i)));
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

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSPageArray::operator==(NSPageArray& o)
{
    NSStylPageArrayIter i, j;
	if (!(o.empty()))
    {
   	    if( (o.size() == size()) )
        {
   		    for (i = o.begin(),j = begin(); (j != end()) && (i != o.end()); i++,j++)
            {
                if (*(*j) != *(*i))
            	    return 0;
            }
        }
        return 1;
    }
    if (empty())
   	    return 1;
    else
   	    return 0;
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSPageArray::vider()
{
    if (empty())
        return;

    for (NSStylPageArrayIter i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}         */

