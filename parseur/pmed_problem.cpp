#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_problem::Cpmed_problem(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
              :Cbalise(attributs , values, pMV, parentBalise)
{
}

Cpmed_problem::Cpmed_problem(const Cpmed_problem& rv)
              :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_problem::parser()
{
  if (false == parser_attributs())
		return false ;

  if (vect_attr.empty())
  {
    _iParsingError = EPMED_PROBLEM_ATTR_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

  int nb_problemid = 0 ;

  for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
  {
    if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_PROBLEMID) // patientid
		{
			nb_problemid ++;
		}
		else
    {
      _iParsingError = EPMED_PROBLEM_ATTR ;
      return false ; // erreur (attribut incorrect)
    }
  }

  if (1 != nb_problemid)
  {
    _iParsingError = EPMED_PROBLEM_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

  if (false == parser_valeurs())
    return false ;

	int nb_pmed_problem_event = 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_PMED_PROBLEM_EVENT) // pmed_admindata
      {
        (*ival)->setObject(new Cpmed_problem_event ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_problem_event
        }
        nb_pmed_problem_event++ ;
      }
			else
      {
        _iParsingError = EPMED_PROBLEM_VAL ;
        return false ; // erreur (valeur incorrecte)
      }
    }
  }

	return true ;
}

bool Cpmed_problem::verif ()
{
  return true ;
}

bool Cpmed_problem::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "    Problem"  << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") + vect_attr[string(ATTRIBUT_PMED_PROBLEM_PROBLEMID)] + string("'") ;

#if !defined _OWLDLL
 	databaseid = nsmysql_insert("problem", "id_problem", "id_healthdata, problem_problemid", values.c_str());
#else
  if (debugFile)
    debugFile << string("id_healthdata, problem_problemid : ") + values + "\n" ;
#endif

  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
		if (false == (*ival)->getObject()->traiter())
			return false ;

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "    Fin problem" << endl ;
#endif
#endif

	return true ;
}

Cpmed_problem&
Cpmed_problem::operator=(const Cpmed_problem& src)
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

  return *this ;
}

