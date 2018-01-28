#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_healthdata::Cpmed_healthdata(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                 :Cbalise(attributs , values, pMV, parentBalise)
{
}

Cpmed_healthdata::Cpmed_healthdata(const Cpmed_healthdata& rv)
                 :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_healthdata::parser ()
{
	if (false == parser_attributs())
		return false ;

	if (false == vect_attr.empty())
  {
    _iParsingError = EPMED_HEALTH_DATA_EMPTY ;
    return false ; // erreur (pas d'attribut attendu)
  }

  if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_PMED_PROBLEM) // pmed_problem
			{
        (*ival)->setObject(new Cpmed_problem ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_problem
        }
      }
			else if ((*ival)->getLabel() == LABEL_PMED_CONTACT) // pmed_contact
			{
        (*ival)->setObject(new Cpmed_contact ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_contact
        }
      }
			else
      {
        _iParsingError = EPMED_HEALTH_DATA_VAL ;
        return false ; // erreur (valeur incorrecte)
      }
    }
  }
	return true ;
}

bool Cpmed_healthdata::verif()
{
  return true ;
}

bool Cpmed_healthdata::compresser()
{
  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    ValIter iSousVal = ival ;
    iSousVal++ ;

    while (vect_val.end() != iSousVal)
    {
      // Même acte - Same act
      //
      if (((*ival)->getAttribute() != string("")) &&
          ((*ival)->getAttribute() == (*iSousVal)->getAttribute()))
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

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    (*ival)->getObject()->compresser() ;

  return true ;
}

bool Cpmed_healthdata::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr <<"  Healthdata (parentId " + parent->databaseid + ") "  <<endl;
#endif
#endif

  string values = string("'") + parent->databaseid + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("healthdata", "id_healthdata", "id_patient_message", values.c_str());
#else
  if (debugFile)
    debugFile << string("id_patient_message : ") + values + "\n";
#endif

  if (vect_val.empty())
    return true ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
		if (false == (*ival)->getObject()->traiter())
			return false ;

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "  Fin healthdata"  <<endl ;
#endif
#endif

	return true;
}

Cpmed_healthdata&
Cpmed_healthdata::operator=(const Cpmed_healthdata& src)
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

