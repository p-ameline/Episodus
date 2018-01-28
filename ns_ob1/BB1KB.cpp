/*
** Copyright Nautilus, (10/9/2004)
** fabrice.le_perru@nautilus-info.com
** david.giocanti@nautilus-info.com

** Ce logiciel est un programme informatique servant à [rappeler les
** caractéristiques techniques de votre logiciel].

** Ce logiciel est régi par la licence CeCILL soumise au droit français et
** respectant les principes de diffusion des logiciels libres. Vous pouvez
** utiliser, modifier et/ou redistribuer ce programme sous les conditions
** de la licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA
** sur le site "http://www.cecill.info".

** En contrepartie de l'accessibilité au code source et des droits de copie,
** de modification et de redistribution accordés par cette licence, il n'est
** offert aux utilisateurs qu'une garantie limitée.  Pour les mêmes raisons,
** seule une responsabilité restreinte pèse sur l'auteur du programme,  le
** titulaire des droits patrimoniaux et les concédants successifs.

** A cet égard  l'attention de l'utilisateur est attirée sur les risques
** associés au chargement,  à l'utilisation,  à la modification et/ou au
** développement et à la reproduction du logiciel par l'utilisateur étant
** donné sa spécificité de logiciel libre, qui peut le rendre complexe à
** manipuler et qui le réserve donc à des développeurs et des professionnels
** avertis possédant  des  connaissances  informatiques approfondies.  Les
** utilisateurs sont donc invités à charger  et  tester  l'adéquation  du
** logiciel à leurs besoins dans des conditions permettant d'assurer la
** sécurité de leurs systèmes et ou de leurs données et, plus généralement,
** à l'utiliser et l'exploiter dans les mêmes conditions de sécurité.

** Le fait que vous puissiez accéder à cet en-tête signifie que vous avez
** pris connaissance de la licence CeCILL, et que vous en avez accepté les
** termes.
*/

#ifdef __GNUC__
# pragma implementation
#endif

#include <fstream.h>      // for BB1KB::Load(...) and BB1KB::Save(...)

#ifndef FOR_WINDOWS
#include "BB1KB.h"
#include "AttValPair.h"   // for BB1KB::Save(...)
#include "BB1BB.h"        // for BB1KB::ThisCycle(...)
#include "BB1Class.h"     // for BB1KB::DeleteClassSubtree(...)
#include "BB1Link.h"      // for BB1KB::Save(...)
#include "BB1Exception.h"    // add fab
#else
#include "ns_ob1\BB1KB.h"
#include "ns_ob1\AttValPair.h"   // for BB1KB::Save(...)
#include "ns_ob1\BB1BB.h"        // for BB1KB::ThisCycle(...)
#include "ns_ob1\BB1Class.h"     // for BB1KB::DeleteClassSubtree(...)
#include "ns_ob1\BB1Link.h"      // for BB1KB::Save(...)
#include "ns_ob1\BB1Exception.h"    // add fab

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif

#endif


// -----------------------------------------------------------------------------
// I don't know if KBs will ever be associated with a file
// -----------------------------------------------------------------------------

BB1KB::BB1KB(const string& kbname, BB1BB& BB, Cycle cycle)
      :_name(kbname),
       _filename(NULLSTRING),
       _protectedP(false),
       _bb(BB),
       _cycleCreated(cycle)
{
}

BB1KB::BB1KB(const BB1KB& src)
      :_name(src._name),
       _filename(src._filename),
       _protectedP(src._protectedP),
       _bb(src._bb),
       _cycleCreated(src._cycleCreated),
       _objects(src._objects)
{
}

// -----------------------------------------------------------------------------
// Destroying a KB consists of (1) destroying its objects and then (2) clearing
// its object set. You can't delete elements from the object set while iterating
// over it.
// -----------------------------------------------------------------------------

BB1KB::~BB1KB()
{
  if (string("Class") == _name)
    // Classes must be deleted bottom up
    DeleteClasses() ;

  else
  {
    // Non-classes may be deleted in an arbitrary order
    if (false == _objects.empty())
      for (ObjectIter p = _objects.begin() ; _objects.end() != p ; )
      {
        // Erase before deleting so that RefreshObjectsList doesn't fail
        //
        BB1Object *pObject = *p ;
        _objects.erase(p) ;
        delete pObject ;
      }
  }
}

// inline
Cycle
BB1KB::ThisCycle()
{
  return _bb.ThisCycle() ;
}

Errcode
BB1KB::InsertObject(BB1Object* obj)
{
  if (NULL == obj)
    return INVALIDATTRIBUTE ;

  _objects.push_back(obj) ;

  return SUCCESS ;
}

// -----------------------------------------------------------------------------
// Delete all elements of the KB named "Class"
// -----------------------------------------------------------------------------
void
BB1KB::DeleteClasses()
{
try
{
  BB1Class *root = BB().ClassNamed("Class") ;
  if (NULL == root)
  {
    // throw cnf;
    ClassNotFound cnf(BB(), "BB1KB::DeleteClasses(...)", "Class") ;
    return ;
  }

  ObjectList allClasses ;
  root->Instances(&allClasses) ;

  ObjectList maximalClasses ;

  if (false == allClasses.empty())
    for (ObjectCIter p = allClasses.begin() ; allClasses.end() != p ; p++)
    {
      ObjectList parents ;
      BB1Class *c = (BB1Class *)(*p) ;
      if (c)
      {
        c->DirectSuperclasses(&parents) ;
        if (parents.empty())
          maximalClasses.push_back(c) ;
      }
    }

  if (false == maximalClasses.empty())
    for (ObjectCIter p = maximalClasses.begin() ; maximalClasses.end() != p ; p++)
    {
      BB1Class *c = (BB1Class *)(*p) ;
      if (c)
        DeleteClassSubtree(c) ;
    }

  delete root ;
}
catch (ClassNotFound& cnf)
{ } //
catch (...)
{
	erreur("Exception BB1KB::DeleteClasses", standardError) ;
}
}

// -----------------------------------------------------------------------------
// Delete all the subclasses of root class, and then delete the root class.
// -----------------------------------------------------------------------------
void
BB1KB::DeleteClassSubtree(BB1Class *root)
{
try
{
  if (NULL == root)
  {
    // throw nco;
    NullClassObject nco(BB(), "BB1KB::DeleteClassSubtree(...)") ;
    return ;
  }

  ObjectList ds ;
  root->DirectSubclasses(&ds) ;

  if (false == ds.empty())
    for (ObjectCIter p = ds.begin() ; ds.end() != p ; p++)
      DeleteClassSubtree(Classptr(*p)) ; // The subclass had better be a class

  delete root ;
}
catch (NullClassObject& nco)
{
}
catch (...)
{
	erreur("Exception BB1KB::DeleteClassSubtree", standardError) ;
}
}

// -----------------------------------------------------------------------------
// Return a pointer to the object whose name is the given name. If there is no
// such object, return a null pointer.
// -----------------------------------------------------------------------------
BB1Object*
BB1KB::ObjectNamed(const string sObjectName) const
{
  if (false == _objects.empty())
    for (ObjectCIter i = _objects.begin() ; _objects.end() != i ; i++)
      if ((*i)->Name() == sObjectName)
				return (*i) ;

  return ((BB1Object *) 0) ;
}

// -----------------------------------------------------------------------------
// Delete object whose name is the given name.
// -----------------------------------------------------------------------------
void
BB1KB::DeleteObjectNamed(const string sObjectName)
{
  if (_objects.empty())
    return ;

  for (ObjectIter i = _objects.begin() ; _objects.end() != i ; i++)
    if ((*i)->Name() == sObjectName)
    {
      // Erase before deleting so that RefreshObjectsList doesn't hung
      //
      BB1Object *pObject = *i ;
      _objects.erase(i) ;
      delete pObject ;
      return ;
    }
}

// -----------------------------------------------------------------------------
// Delete object whose name is the given name.
// -----------------------------------------------------------------------------
void
BB1KB::DeleteObjectForPath(const string sPath)
{
  if (_objects.empty())
    return ;

  for (ObjectIter i = _objects.begin() ; _objects.end() != i ; i++)
  {
    TypedVal *pObjectPath = (*i)->PathAttributes() ;

    if (pObjectPath && (pObjectPath->getString() == sPath))
    {
      // Erase before deleting so that RefreshObjectsList doesn't hung
      //
      BB1Object *pObject = *i ;
      _objects.erase(i) ;
      delete pObject ;
      return ;
    }
  }
}

// -----------------------------------------------------------------------------
// Erase (and don't delete) object whose name is the given name.
// -----------------------------------------------------------------------------
void
BB1KB::EraseObjectNamed(const string sObjectName)
{
  if (_objects.empty())
    return ;

  for (ObjectIter i = _objects.begin() ; _objects.end() != i ; )
    if ((*i)->Name() == sObjectName)
      _objects.erase(i) ;
    else
      i++ ;
}

// -----------------------------------------------------------------------------
// Save the contents of the KB to the file. By default, the file name is that
// which was used to load the KB.
// -----------------------------------------------------------------------------
Errcode
BB1KB::Save(const string& fileName)
{
  if (NULLSTRING == fileName)
    return (FAILURE) ;
  else
  {
    ofstream KBstream(fileName.c_str()) ;
    if (!KBstream)
      return (FAILURE) ; // cannot open output file path
    else
    {
      KBstream << "KB " << Name() << "\n\n";
      if (false == _objects.empty())
				for (ObjectCIter p = _objects.begin() ; _objects.end() != p ; p++)
	  			(*p)->Save(KBstream) ; // write the object description to the file

      return (SUCCESS) ;
    }
  }
}

/*
// -----------------------------------------------------------------------------
// Convenience function for accessing BB1BB::ThisCycle(). This has to be defined
// here because "BB1KB.c" includes "BB1BB.h", which can not be included in
// "BB1KB.h".
// -----------------------------------------------------------------------------
// moving in BB1KB.h
Cycle BB1KB::ThisCycle(void) const
{
#ifdef _DEBUG_
	cout << "-- DEBUG:: BB1KB::ThisCycle() const\n\n" ;
#endif
	return (bb.ThisCycle()) ;
};
*/

