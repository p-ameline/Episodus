#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_contact_type::Cpmed_contact_type (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                   :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_contact_type::Cpmed_contact_type(const Cpmed_contact_type& rv)
                   :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_contact_type::parser ()
{
	if (false == parser_attributs ())
		return false;

	int nb_conditions  = 0 ;
	int nb_rendez_vous = 0 ;

	if (false == vect_attr.empty())
  {
    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACT_TYPE_CONDITIONS) // conditions
      {
        nb_conditions++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS) // rendez-vous
      {
        nb_rendez_vous++ ;
      }
      else
      {
        _iParsingError = EPMED_CONTACT_TYPE_ATTR ;
        return false ; // erreur (attribut incorrect)
      }
    }
  }

  if (1 != nb_conditions)
  {
    _iParsingError = EPMED_CONTACT_TYPE_NB ;
    return false ; // erreur (nb attributs incorrect)
  }

	if (false == parser_valeurs())
		return false ;

  if (false == vect_val.empty())
  {
    _iParsingError = EPMED_CONTACT_TYPE_VAL ;
    return false ; // erreur (pas de valeur attendue)
  }

  return true ;
}

bool Cpmed_contact_type::verif ()
{
  if ((vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_CONDITIONS)] != "C") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_CONDITIONS)] != "V"))
  {
    _iParsingError = EPMED_CONTACT_TYPE_CONDITIONS ;
    return false ;
  }

  if (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS)] != "")
  {
    if ((vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS)] != "O") ||
        (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS)] != "N") ||
        (vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS)] != "U"))
    {
      _iParsingError = EPMED_CONTACT_TYPE_RENDEZ_VOUS ;
      return false ;
    }
  }

  return true ;
}

bool Cpmed_contact_type::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Contact_type" << endl ;
#endif
#endif

	string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_CONDITIONS)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACT_TYPE_RENDEZ_VOUS)] + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("contact_type", "id_contact_type", "id_contact, contact_type_conditions, contact_type_rendez_vous", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_contact, contact_type_conditions, contact_type_rendez_vous : ") + values + "\n";
#endif

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "      Fin contact_type" << endl ;
#endif
#endif

  return true ;
}

Cpmed_contact_type&
Cpmed_contact_type::operator=(const Cpmed_contact_type& src)
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
