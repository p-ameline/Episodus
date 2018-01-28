#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"

Cpmed_contactcode::Cpmed_contactcode (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                  :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_contactcode::Cpmed_contactcode(const Cpmed_contactcode& rv)
                  :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_contactcode::parser ()
{
	if (false == parser_attributs())
		return false ;

  int nb_position           = 0 ;
	int nb_classification     = 0 ;
	int nb_code               = 0 ;
	int nb_classification_bis = 0 ;
	int nb_code_bis           = 0 ;

	if (false == vect_attr.empty())
  {
    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTCODE_POSITION) // position
      {
        nb_position++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION) // classification
      {
        nb_classification++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTCODE_CODE) // code
      {
        nb_code++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS) // classification_bis
      {
        nb_classification_bis++ ;
      }
      else if ((*iattribut)->getLabel() == ATTRIBUT_PMED_CONTACTCODE_CODE_BIS) // code_bis
      {
        nb_code_bis++ ;
      }
      else
      {
        _iParsingError = EPMED_CONTACT_CODE_ATTR ;
        return false ; // erreur (attribut incorrect)
      }
    }
  }

  if ((1 != nb_position) || (1 != nb_classification) || (1 != nb_code))
  {
    _iParsingError = EPMED_CONTACT_CODE_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }
  if (((1 == nb_classification_bis) && (1 != nb_code_bis)) ||
      ((1 != nb_classification_bis) && (1 == nb_code_bis)))
  {
    _iParsingError = EPMED_CONTACT_CODE_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

	if (false == parser_valeurs())
		return false ;

  if (false == vect_val.empty())
  {
    _iParsingError = EPMED_CONTACT_CODE_VAL ;
    return false ; // erreur (pas de valeur attendue)
  }

	return true ;
}

bool Cpmed_contactcode::verif ()
{
  if ((vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "M")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "S")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "O")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "1")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "A")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "AC") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "AI") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "2")  ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "2F") ||
      (vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] != "2P"))
  {
    _iParsingError = PMED_CONTACT_CODE_POSITION ;
    return false ;
  }

    /*
    if ((vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION] != "CISP2")    ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION] != "SFMG")     ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION] != "CIM10")    ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION] != "CDAM"))
    {
        iParsingError = PMED_CONTACT_CODE_CLASSIFICATION;
        return false;
    }
    */

    /*
    if ((vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS] != "CISP2") ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS] != "SFMG")  ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS] != "CIM10") ||
        (vect_attr[ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS] != "CDAM"))
    {
        iParsingError = PMED_CONTACT_CODE_CLASSIFICATION_BIS;
        return false;
    }
    */

  return true ;
}

bool Cpmed_contactcode::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "        Contactcode (parentId " + parent->databaseid + ") " + vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] + string(" ") +
          vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION)] + string(" ") +
          vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CODE)] << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CODE)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION_BIS)] + string("', '") +
                  vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CODE_BIS)] + string("'");

#if !defined _OWLDLL
  databaseid = nsmysql_insert("contactcode", "id_contactcode", "id_contactelement, contactcode_position, contactcode_classification, contactcode_code, contactcode_classification_bis, contactcode_code_bis", values.c_str()) ;
#else
  if (debugFile)
    debugFile << string("id_contactelement, contactcode_position, contactcode_classification, contactcode_code, contactcode_classification_bis, contactcode_code_bis : ") + values + "\n" ;
#endif

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "        Fin contactcode "  + vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_POSITION)] + string(" ") +
          vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CLASSIFICATION)] + string(" ") +
          vect_attr[string(ATTRIBUT_PMED_CONTACTCODE_CODE)] << endl ;
#endif
#endif

	return true ;
}

Cpmed_contactcode&
Cpmed_contactcode::operator=(const Cpmed_contactcode& src)
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

