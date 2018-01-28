#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_contactelement::Cpmed_contactelement (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                     :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_contactelement::Cpmed_contactelement(const Cpmed_contactelement& rv)
                     :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_contactelement::parser ()
{
	if (false == parser_attributs())
		return false ;

	int nb_contactelement = 0 ;
	int nb_problemid      = 0 ;

	if (false == vect_attr.empty())
  {
	  for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
		  if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID) // contactelementid
		  {
			  nb_contactelement++ ;
		  }
		  else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTELEMENT_PROBLEMID) // type_principe
		  {
			  nb_problemid++ ;
		  }
		  else
		  {
		    _iParsingError = EPMED_CONTACT_ELT_ATTR ;
		    return false ; // erreur (attribut incorrect)
		  }
    }
  }

	if ((nb_contactelement > 1) || (nb_problemid > 1))
  {
    _iParsingError = EPMED_CONTACT_ELT_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

	if (false == parser_valeurs())
		return false ;

  int nb_pmed_contactcode = 0 ;

	if (false == vect_val.empty())
  {
		for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
			if ((*ival)->getLabel() == LABEL_PMED_CONTACTCODE) // pmed_contact_code
			{
				(*ival)->setObject(new Cpmed_contactcode((*ival)->getAttribute(), (*ival)->getValue(), pMainValues, this)) ;
				if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ; // erreur parsing pmed_problem_event
        }
				nb_pmed_contactcode++ ;
			}
			else
      {
        _iParsingError = EPMED_CONTACT_ELT_VAL_FALSE ;
        return false ; // erreur (valeur incorrecte)
      }
    }
  }

  if (nb_pmed_contactcode < 1)
  {
    _iParsingError = EPMED_CONTACT_ELT_VAL ;
    return false ; // erreur (nb valeurs incorrect)
  }

  return true ;
}

bool Cpmed_contactelement::verif ()
{
  return true ;
}

bool Cpmed_contactelement::compresser()
{
  if (vect_val.empty())
    return true ;

  // On supprime tous les doublons
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
  {
    ValIter iSousVal = ival ;
    iSousVal++ ;

    while (vect_val.end() != iSousVal)
    {
      if ((*ival)->getAttribute() == (*iSousVal)->getAttribute())
      {
        delete (*iSousVal) ;
        vect_val.erase(iSousVal) ;
      }
      else
        iSousVal++ ;
    }
    if (vect_val.end() != ival)
      ival++ ;
  }

  return true ;
}

bool Cpmed_contactelement::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Contactelement (parentId " + parent->databaseid + ") "  + vect_attr[string(ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID)] << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID)] + string("', '") +
                  vect_attr [string(ATTRIBUT_PMED_CONTACTELEMENT_PROBLEMID)] + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("contactelement", "id_contactelement", "id_contact, contactelement_contactelementid, contactelement_problemid", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_contact, contactelement_contactelementid, contactelement_problemid : ") + values + "\n";
#endif

  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
		if (false == (*ival)->getObject()->traiter())
			return false ;

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Fin contactelement "  + vect_attr[string(ATTRIBUT_PMED_CONTACTELEMENT_CONTACTELEMENTID)] << endl ;
#endif
#endif

  return true ;
}

Cpmed_contactelement&
Cpmed_contactelement::operator=(const Cpmed_contactelement& src)
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
