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

#ifndef FOR_WINDOWS
# include "BB1BB.h"
# include "BB1KB.h"        // for BB1BB::CheckDirectSuperclasses(...)
# include "BB1Class.h"     // for BB1BB::OppositeLinkName(...)
# include "BB1Link.h"      // for BB1BB::OppositeLinkName(...)
# include "BB1Exception.h"    // add fab
#else
# include "ns_ob1\BB1BB.h"
# include "ns_ob1\BB1KB.h"        // for BB1BB::CheckDirectSuperclasses(...)
# include "ns_ob1\BB1Class.h"     // for BB1BB::OppositeLinkName(...)
# include "ns_ob1\BB1Link.h"      // for BB1BB::OppositeLinkName(...)
# include "ns_ob1\BB1Exception.h"    // add fab
#endif

// -----------------------------------------------------------------------------
// This file contains the non-exported methods for class BB1BB. The Application
// Programmer's Interface methods are defined in "BB1API.c".
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Check the validity of a class's set of allowed attributes. If this isn't
// going to get more complex, it should either be made an inline in BB1BB.h or,
// better yet, abolished.
// -----------------------------------------------------------------------------
bool
BB1BB::CheckAllowedAttributes(const AVPSet * /* allowedAttributes */)
{
#ifdef _DEBUG_
  trout << "BB1BB::CheckAllowedAttributes(const SetOfAVP &)\n" ;
#endif

  // A class can have any attributes
  // Should we check for duplicates?
  return (true) ;
}

// -----------------------------------------------------------------------------
// Check the validity of a class's set of direct superclasses. Return the links
// in the dcLinks parameter.
// -----------------------------------------------------------------------------
bool
BB1BB::CheckDirectSuperclasses(const StringList	*pDscNames, ObjectSet *pDscObjects)
{
  if (NULL == pDscObjects)
    return false ;

#ifdef _DEBUG_
  trout << "BB1BB::CheckDirectSuperclasses(const ListOfString &, SetOfBB1Object &)\n" ;
#endif

try
{
  pDscObjects->clear() ;
  if (pDscNames && (false == pDscNames->empty()))
  {
    for (StrCIter p = pDscNames->begin() ; pDscNames->end() != p ; p++)
    {
      BB1Object *dsc = _classKB->ObjectNamed(*p) ;
      if (dsc)
        pDscObjects->push_back(dsc) ; // don't worry about duplicates
      else
        ClassDoesNotExist cdne(*this, string("BB1BB:CheckDirectSuperclasses(...)"), *p) ; // throw cdne
    }
  }

  if (pDscNames)
    return (pDscNames->size() == pDscObjects->size()) ;

  return (pDscObjects->size() == 0) ;
}
catch (ClassDoesNotExist& cdne)
{ return (false) ; }
 // return (false) ;
}

// -----------------------------------------------------------------------------
// Clear the data member BB1BB::linkPairs
// -----------------------------------------------------------------------------
Errcode
BB1BB::ClearLinkPairs()
{
#ifdef _DEBUG_
  _trout << "BB1BB::ClearLinkPairs()\n" ;
#endif

  if (false == _linkPairs.empty())
  {
    for (LinkPairIter p = _linkPairs.begin() ; _linkPairs.end() != p ; )
    {
      delete (*p) ;
      _linkPairs.erase(p) ;
    }
  }

  return (SUCCESS) ;
}

// -----------------------------------------------------------------------------
// Return the link name which is the opposite of the given link name
// -----------------------------------------------------------------------------
const	string
BB1BB::OppositeLinkName(const string sName, const BB1Class* pC) const
{
#ifdef _DEBUG_
  trout << "BB1BB::OppositeLinkName(" << name << ", const BB1Class &) const\n" ;
#endif
try
{
  // Handle built-in reserved links separately
  //
  if (string("IsA") == sName)
    return string("CanBeA") ;
  if (string("CanBeA") == sName)
    return string("IsA") ;

  if (string("Exemplifies") == sName)
    return string("ExemplifiedBy") ;
  if (string("ExemplifiedBy") == sName)
    return string("Exemplifies") ;

  if (pC && (false == _linkPairs.empty()))
    for (LinkPairCIter p = _linkPairs.begin() ; _linkPairs.end() != p ; p++)
    {
      const BB1LinkPair* lp = *p ;
      if (lp)
      {
        if ((lp->ForwardLink() == sName) && ((pC->Name() == "Class") || pC->IsAP(lp->FromClassName())))
          return lp->InverseLink() ;
        else
          if ((lp->InverseLink() == sName) && ((pC->Name() == "Class") || pC->IsAP(lp->ToClassName())))
          return lp->ForwardLink() ;
      }
    }

  LinkDoesNotExist ldne(*this, "", /* don't know method yet */ sName) ;
} // end try
catch (LinkDoesNotExist& ldne)
{
  return string("") ;
}

  return string("") ;
}

// -----------------------------------------------------------------------------
// Is the link name one of the built-in reserved links? Since this doesn't use
// any member of class BB1BB, it doesn't have to be a member function.
// -----------------------------------------------------------------------------
bool
BB1BB::ReservedLinkP(const string sLinkName) const
{
#ifdef _DEBUG_
  trout << "-- DEBUG:: BB1BB::ReservedLinkP(" << linkName << ")\n" ;
#endif

  return ((sLinkName == "IsA") || (sLinkName == "CanBeA") ||
          (sLinkName == "Exemplifies") || (sLinkName == "ExemplifiedBy")) ;
}

