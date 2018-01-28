#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_problem_event::Cpmed_problem_event (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                    :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_problem_event::Cpmed_problem_event(const Cpmed_problem_event& rv)
                    :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_problem_event::parser()
{
	if (false == parser_attributs())
		return false ;

	if (vect_attr.empty())
  {
    _iParsingError = EPMED_PROBLEM_EVENT_ATTR_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

	int nb_type          = 0 ;
	int nb_related_pb_id = 0 ;
	int nb_title         = 0 ;
	int nb_important     = 0 ;
  int nb_date          = 0 ;

  if (false == vect_attr.traiterDate(ATTRIBUT_PMED_PROBLEM_EVENT_DATE, 8))
  {
    _iParsingError = EPMED_PROBLEM_EVENT_ATTR ;
    return false ; // erreur (attribut incorrect)
  }

  for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
  {
    if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_EVENT_TYPE) // type
		{
			nb_type++;
		}
		else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_EVENT_DATE) // date
		{
			nb_date++ ;
		}
		else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_EVENT_RELATED_PB_ID) // related_pb_id
		{
			nb_related_pb_id++ ;
		}
		else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_EVENT_TITLE) // title
		{
			nb_title++ ;
		}
		else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_PROBLEM_EVENT_IMPORTANT) // important
		{
			nb_important++ ;
		}
		else
    {
      _iParsingError = EPMED_PROBLEM_EVENT_ATTR ;
      return false ; // erreur (attribut incorrect)
    }
  }

  if ((nb_type          != 1) ||
      (nb_related_pb_id > 1)  ||
      (nb_title         > 1)  ||
      (nb_important     != 1) ||
      (nb_date          != 1))
  {
    _iParsingError = EPMED_PROBLEM_EVENT_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

  if (false == parser_valeurs())
    return false ;

  if (false == vect_val.empty())
  {
    _iParsingError = EPMED_PROBLEM_EVENT_VAL ;
    return false ; // erreur (pas de valeur attendue)
  }

	return true ;
}

bool Cpmed_problem_event::verif ()
{
  if ((vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_TYPE)] != "open") ||
      (vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_TYPE)] != "modify") ||
      (vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_TYPE)] != "close"))
  {
    _iParsingError = EPMED_PROBLEM_EVENT_TYPE ;
    return false ;
  }

  if ((vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_IMPORTANT)] != "Oui") ||
      (vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_IMPORTANT)] != "Non"))
  {
    _iParsingError = EPMED_PROBLEM_EVENT_IMPORTANT ;
    return false ;
  }

  return true ;
}

bool Cpmed_problem_event::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Problem_event" << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_TYPE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_RELATED_PB_ID)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_TITLE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_DATE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_PROBLEM_EVENT_IMPORTANT)] + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("problem_event", "id_problem_event", "id_problem,problem_event_type,problem_event_related_pb_id,problem_event_title,problem_event_date,problem_event_important", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_problem,problem_event_type,problem_event_related_pb_id,problem_event_title,problem_event_date,problem_event_important : ") + values + "\n";
#endif

	// manque date

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Fin problem_event" << endl ;
#endif
#endif

	return true ;
}

Cpmed_problem_event&
Cpmed_problem_event::operator=(const Cpmed_problem_event& src)
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

