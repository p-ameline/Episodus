#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_contact::Cpmed_contact (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
              :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_contact::Cpmed_contact(const Cpmed_contact& rv)
              :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_contact::parser ()
{
  if (false == parser_attributs())
    return false ;

	int nb_contactid         = 0 ;
	int nb_type_principe     = 0 ;
	int nb_pmed_contact_date = 0 ;

  if (false == vect_attr.empty())
  {
    if (false == vect_attr.traiterDate(ATTRIBUT_PMED_CONTACT_DATE, 8))
    {
      _iParsingError = EPMED_CONTACT_ATTR ;
      return false ; // erreur (attribut incorrect)
    }

    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACT_CONTACTID) // contactid
      {
        nb_contactid++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE) // type_principe
      {
        nb_type_principe++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACT_DATE) // date
      {
        nb_pmed_contact_date++ ;
      }
      else
      {
        _iParsingError = EPMED_CONTACT_ATTR ;
        return false ; // erreur (attribut incorrect)
      }
    }
  }

  if ((nb_contactid > 1) || (nb_type_principe > 1) || (nb_pmed_contact_date != 1))
  {
    _iParsingError = EPMED_CONTACT_NB ;
    return false ; // erreur (nb attributs incorrect)
  }

  if (false == parser_valeurs())
    return false ;

	int nb_pmed_contact_type   = 0 ;
	int nb_pmed_contactelement = 0 ;

	if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
			if ((*ival)->getLabel() == LABEL_PMED_CONTACT_TYPE) // pmed_contact_type
			{
        (*ival)->setObject(new Cpmed_contact_type ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_problem_event
        }
				nb_pmed_contact_type++ ;
			}
			else if ((*ival)->getLabel() == LABEL_PMED_CONTACTELEMENT) // pmed_contactelement
			{
				(*ival)->setObject(new Cpmed_contactelement ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_problem_event
        }
				nb_pmed_contactelement++ ;
			}
			else
      {
        _iParsingError = EPMED_CONTACT_VAL ;
        return false ; // erreur (valeur incorrecte)
      }
    }
  }

  if (nb_pmed_contact_type > 1 || nb_pmed_contactelement < 1)
  {
    _iParsingError = EPMED_CONTACT_NBVAL ;
    return false ; // erreur (nb valeurs incorrect)
  }

	return true ;
}

bool Cpmed_contact::verif ()
{
  if ((vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE)] != "SOAP") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE)] != "SFMG") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE)] != "OTHER"))
  {
    _iParsingError = EPMED_CONTACT_PRINCIPE ;
    return false ;
  }

  return true ;
}

bool Cpmed_contact::compresser()
{
  if (vect_val.empty())
    return true ;

  // Première étape : ne garder que le dernier pmed_contact_type
  //
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
  {
    if ((*ival)->getLabel() == LABEL_PMED_CONTACT_TYPE)
    {
      ValIter iSousVal = ival ;
      iSousVal++ ;

      for ( ; vect_val.end() != iSousVal ; iSousVal++)
        if ((*iSousVal)->getLabel() == LABEL_PMED_CONTACT_TYPE)
          break ;

      // S'il y a un pmed_contact_type plus loin, on tue celui-ci
      //
      if (vect_val.end() != iSousVal)
      {
        delete (*ival) ;
        vect_val.erase(ival) ;
      }
      else
        ival++ ;
    }
    else
      ival++ ;
  }

  // Seconde étape : fusionner les pmed_contactelement
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // Seuls les contact_element avec id sont fusionnables
    //
    if (((*ival)->getLabel() == LABEL_PMED_CONTACTELEMENT) && ((*ival)->getAttribute() != string("")))
    {
      ValIter iSousVal = ival ;
      iSousVal++ ;

      while (vect_val.end() != iSousVal)
      {
        if (((*iSousVal)->getLabel() == LABEL_PMED_CONTACTELEMENT) && ((*ival)->getAttribute() == (*iSousVal)->getAttribute()))
        {
          // (*ival)->pObject->vect_val = (*ival)->pObject->vect_val + (*iSousVal)->pObject->vect_val;

          if (false == (*iSousVal)->getObject()->vect_val.empty())
          {
            ValIter i = (*iSousVal)->getObject()->vect_val.begin() ;
            for ( ; (*iSousVal)->getObject()->vect_val.end() != i ; )
            {
              (*i)->getObject()->parent = (*ival)->getObject() ;
              (*ival)->getObject()->vect_val.push_back(*i) ;
              (*iSousVal)->getObject()->vect_val.erase(i) ;
            }
          }

          delete (*iSousVal) ;
          vect_val.erase(iSousVal) ;
        }
        else
          iSousVal++ ;
      }
    }
  }

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    (*ival)->getObject()->compresser() ;

  return true ;
}

bool Cpmed_contact::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "    Contact" << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACT_CONTACTID)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_PRINCIPE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACT_DATE)] + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("contact", "id_contact", "id_healthdata, contact_contactid, contact_type_principe, contact_date", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_healthdata, contact_contactid, contact_type_principe, contact_date : ") + values + "\n" ;
#endif

  if (vect_val.empty())
    return true ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
		if (false == (*ival)->getObject()->traiter())
			return false ;

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "    Fin contact" << endl ;
#endif
#endif

	return true ;
}

Cpmed_contact&
Cpmed_contact::operator=(const Cpmed_contact& src)
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

