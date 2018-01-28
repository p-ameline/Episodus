#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_patient::Cpmed_patient(string attributs, string values, Cconteneur* pMV)
              :Cbalise(attributs , values)
{
  pMainValues = new Cconteneur ;
  pMainValues->send_id = pMV->send_id ;
  bMVowner = true ;
}

Cpmed_patient::Cpmed_patient(const Cpmed_patient& rv)
              :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;

  bMVowner = false ;
}

Cpmed_patient::~Cpmed_patient ()
{
  if (bMVowner)
    delete pMainValues ;
}

bool Cpmed_patient::parser ()
{
	if (false == parser_attributs())
		return false ;

	if (vect_attr.empty())
  {
    _iParsingError = EPMED_PATIENT_ATTR_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

	int nb_patientid = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
  {
		if (ATTRIBUT_PMED_PATIENT_PATIENTID == (*iattribut)->getLabel()) // patientid
		{
			pMainValues->pat_id = (*iattribut)->getValue() ;
			nb_patientid++ ;
		}
		else
    {
      _iParsingError = EPMED_PATIENT_ATTR ;
      return false ; // erreur (attribut incorrect)
    }
  }

	if (1 != nb_patientid)
  {
    _iParsingError = EPMED_PATIENT_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

	if (false == parser_valeurs())
		return false ;

#if !defined _OWLDLL
//cerr <<"Parse patient ok" <<endl;
#endif

	int nb_pmed_admindata  = 0 ;

	if (false == vect_val.empty())
		for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
			if (LABEL_PMED_ADMINDATA == (*ival)->getLabel()) // pmed_admindata
			{
				(*ival)->setObject(new Cpmed_admindata ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
#if !defined _OWLDLL
//cerr <<"Parse admindata" <<endl;
#endif
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_admindata
        }
#if !defined _OWLDLL
//cerr <<"Parse admindata ok" <<endl;
#endif
				nb_pmed_admindata ++ ;
			}
			else if (LABEL_PMED_HEALTHDATA == (*ival)->getLabel()) // pmed_healthdata
			{
				(*ival)->setObject(new Cpmed_healthdata ((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_healthdata
        }
#if !defined _OWLDLL
//cerr <<"Parse healthdata ok" <<endl;
#endif
			}
			else
      {
        _iParsingError = EPMED_PATIENT_VAL ;
        return false ; // erreur (valeur incorrecte)
      }
    }

  if (nb_pmed_admindata > 1)
  {
    _iParsingError = EPMED_PATIENT_NBVAL ;
    return false ; // erreur (nb valeurs incorrect)
  }

	return true ;
}

bool Cpmed_patient::verif ()
{
  return true ;
}

bool Cpmed_patient::compresser()
{
  if (vect_val.empty())
    return true ;

  // Première étape : ne garder que le dernier pmed_admindata
  for (ValIter ival = vect_val.begin() ; ival != vect_val.end(); )
  {
    if ((*ival)->getLabel() == LABEL_PMED_ADMINDATA)
    {
      ValIter iSousVal = ival ;
      iSousVal++ ;

      for ( ; vect_val.end() != iSousVal ; iSousVal++)
        if ((*iSousVal)->getLabel() == LABEL_PMED_ADMINDATA)
          break ;

      // S'il y a un pmed_admindata plus loin, on tue celui-ci
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

  // Seconde étape : fusionner les pmed_healthdata
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if ((*ival)->getLabel() == LABEL_PMED_HEALTHDATA)
    {
      ValIter iSousVal = ival ;
      iSousVal++ ;

      while (vect_val.end() != iSousVal)
      {
        if ((*iSousVal)->getLabel() == LABEL_PMED_HEALTHDATA)
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

bool Cpmed_patient::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr <<"Patient "  + pMainValues->pat_id <<endl;
#endif
#endif

  string values = string("'") + pMainValues->pat_id + string("', '") + pMainValues->send_id + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("patient_message", "id_patient_message", "patient_message_patientid, patient_message_senderid", values.c_str());
#else
  if (debugFile)
    debugFile << string("patient_message_patientid, patient_message_senderid : ") + values + "\n";
#endif

  if (vect_val.empty())
    return true ;

	// ajouter patient_message_date
  //
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
		if (false == (*ival)->getObject()->traiter())
			return false  ;

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr <<"Fin patient "  + pMainValues->pat_id <<endl;
#endif
#endif

	return true ;
}

Cpmed_patient&
Cpmed_patient::operator=(const Cpmed_patient& src)
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

  return *this;
}

