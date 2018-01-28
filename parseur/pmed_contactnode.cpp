// -------------------------------------------------------------------------
// pmed_contactnode.cpp  -  description
// -------------------------------------------------------------------------
// $Revision : 1.1 $
// $Author: pameline $
// $Date: 2013/11/11 23:16:19 $
// -------------------------------------------------------------------------
// mai 2001
// -------------------------------------------------------------------------

// -------------------------------------------------------------------------
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at your
// option) any later version.
// -------------------------------------------------------------------------

#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"
#include "valeur.h"

//#define __DEBUG__

Cpmed_contactnode::Cpmed_contactnode(string attributs, string values, Cconteneur *pMV, Cbalise *parentBalise)
                  :Cbalise(attributs, values, pMV, parentBalise)
{
  _sContactTreeID = string("") ;
}

Cpmed_contactnode::Cpmed_contactnode(const Cpmed_contactnode& rv)
                  :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;

  _sContactTreeID = rv._sContactTreeID ;
}

bool
Cpmed_contactnode::parser()
{
  if (false == parser_attributs())
    return false ;

  int	nb_localisation = 0 ;
  int	nb_lexique      = 0 ;
  int	nb_complement   = 0 ;
  int	nb_certitude    = 0 ;
  int	nb_pluriel      = 0 ;
  int	nb_unit         = 0 ;

  if (false == vect_attr.empty())
  {
    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if      ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_LOCALISATION)
        nb_localisation++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_LEXIQUE)
        nb_lexique++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_COMPLEMENT)
        nb_complement++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_CERTITUDE)
        nb_certitude++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_PLURIEL)
        nb_pluriel++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTNODE_UNIT)
        nb_unit++ ;
      else
      {
        // erreur (attribut incorrect)
        _iParsingError = EPMED_CONTACTNODE_ATTR ;
        return false ;
      }
    }
  }

  if ((1 != nb_localisation) && (1 != nb_lexique))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EPMED_CONTACTNODE_NBATTR ;
    return false ;
  }

  if (false == parser_valeurs())
    return false ;

  if (false == vect_val.empty())
  {
    // erreur (pas de valeur attendue)
    _iParsingError = EPMED_CONTACTNODE_VAL ;
    return false ;
  }

  return true ;
}

bool
Cpmed_contactnode::verif()
{
  return true ;
}

bool
Cpmed_contactnode::traiter()
{
#ifndef _OWLDLL
# ifndef _MESSAGE_NOVERBOSE
  cerr << "  Contactnode (parentId " + parent->databaseid + ") " + vect_attr[string()] << endl ;
# endif
#endif

  string sFatherNode  = string("") ;
  string sContactTree = string("") ;

  Cpmed_contactnode	*pFatherNode = dynamic_cast<Cpmed_contactnode *>(parent) ;
  if (pFatherNode)
  {
    sFatherNode  = pFatherNode->databaseid ;
    sContactTree = pFatherNode->_sContactTreeID ;
  }
  else
    sContactTree = parent->databaseid ;

#ifndef __DEBUG__
  cerr << "-- contactnode params : sFatherNode <" << sFatherNode.c_str() << "> sContactTree <" << sContactTree.c_str() << ">" << endl ;
#endif // __DEBUG__

  _sContactTreeID = sContactTree ;

  string values = string("'") + sContactTree + string("', '") +
                  sFatherNode + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_LOCALISATION)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_LEXIQUE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_COMPLEMENT)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_CERTITUDE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_PLURIEL)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTNODE_UNIT)] + string("'") ;

#ifndef __DEBUG__
  cerr << "-- contactnode values are : " << values << endl ;
#endif // __DEBUG__


#ifndef _OWLDLL
  databaseid = nsmysql_insert("contact_node", "id_contactnode", "id_contacttree, id_fathernode, localisation, lexique, complement, certitude, pluriel, unit", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_contacttree, id_fathernode, localisation, lexique, complement, certitude, pluriel, unit : ") + values + "\n" ;
#endif

#ifndef _OWLDLL
# ifndef _MESSAGE_NOVERBOSE
  cerr << "  Fin Contactnode" << endl ;
# endif
#endif

  return true ;
}

Cpmed_contactnode&
Cpmed_contactnode::operator=(const Cpmed_contactnode& src)
{
  if (this == &src)
    return (*this) ;

  parent         = src.parent ;
  _sAttributs    = src._sAttributs ;
  _sValeurs      = src._sValeurs ;
  databaseid     = src.databaseid ;
  _iParsingError = src._iParsingError ;
  vect_attr      = src.vect_attr ;
  vect_val       = src.vect_val ;
  pMainValues    = src.pMainValues ;

  _sContactTreeID = src._sContactTreeID ;

  return *this ;
}

