// -------------------------------------------------------------------------
// pmed_contacttree.cpp  -  description
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

Cpmed_contacttree::Cpmed_contacttree(string attributs, string values, Cconteneur *pMV, Cbalise *parentBalise)
                  :Cbalise(attributs, values, pMV, parentBalise)
{
}

Cpmed_contacttree::Cpmed_contacttree(const Cpmed_contacttree& rv)
                  :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool
Cpmed_contacttree::parser()
{
  if (false == parser_attributs())
    return false ;

  int nb_position = 0 ;

  if (false == vect_attr.empty())
  {
    for (AttrIter	iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTTREE_POSITION)
        nb_position++ ;
      else
      {
        _iParsingError = EPMED_CONTACTTREE_ATTR ;
        return false ; // erreur (attribut incorrect)
      }
    }
  }

  if (1 != nb_position)
  {
    _iParsingError = EPMED_CONTACTTREE_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

  if (false == parser_valeurs())
    return false ;

  int	nb_pmed_contactnode	= 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_PMED_CONTACTNODE)
      {
#ifdef __DEBUG__
        cerr << "--   on a trouve un label contactnode" << endl ;
#endif
        // pmed_contactnode
        (*ival)->setObject(new Cpmed_contactnode((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing pmed_contactnode
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        nb_pmed_contactnode++ ;
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = EPMED_CONTACTTREE_VAL_FALSE ;
        return false ;
      }
    }
  }

  if (nb_pmed_contactnode < 1)
  {
    // erreur (valeur incorrecte)
    _iParsingError = EPMED_CONTACTTREE_VAL ;
    return false ;
  }

  return true ;
}

bool
Cpmed_contacttree::verif()
{
  /*
  if ((vect_attr[ATTRIBUT_PMED_ADMINDATA_SEXE] != "1") || (vect_attr[ATTRIBUT_PMED_ADMINDATA_SEXE] != "2"))
  {
    iParsingError = EPMED_ADMIN_DATA_SEXE ;
    return false ;
  }
  return true ;
  */
  return true ;
}

bool
Cpmed_contacttree::traiter()
{
#ifndef _OWLDLL
# ifndef _MESSAGE_NOVERBOSE
  cerr << "  Contacttree (parentId " + parent->databaseid + ") " + vect_attr[string()] << endl ;
# endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTTREE_POSITION)] + string("'") ;

#ifndef _OWLDLL
  databaseid = nsmysql_insert("contact_tree", "id_contacttree", "id_contactelement, contacttree_position", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_contactelement, contacttree_position : ") + values + "\n" ;
#endif

  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    if (false == (*ival)->getObject()->traiter())
      return false ;

#ifndef _OWLDLL
# ifndef _MESSAGE_NOVERBOSE
  cerr << "  Fin Contacttree" << endl ;
# endif
#endif

  return true ;
}

Cpmed_contacttree&
Cpmed_contacttree::operator=(const Cpmed_contacttree& src)
{
  if (this == &src)
    return (*this) ;

  parent         = src.parent ;
  _sAttributs	   = src._sAttributs ;
  _sValeurs	     = src._sValeurs ;
  databaseid	   = src.databaseid ;
  _iParsingError = src._iParsingError ;
  vect_attr      = src.vect_attr ;
  vect_val       = src.vect_val ;
  pMainValues    = src.pMainValues ;

  return (*this) ;
}

