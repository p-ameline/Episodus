#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"
#include "valeur.h"

Cpmed_admindata::Cpmed_admindata (string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
                :Cbalise (attributs , values, pMV, parentBalise)
{
}

Cpmed_admindata::Cpmed_admindata(const Cpmed_admindata& rv)
                :Cbalise(rv._sAttributs, rv._sValeurs, rv.parent)
{
  databaseid     = rv.databaseid ;
  _iParsingError = rv._iParsingError ;
  vect_attr      = rv.vect_attr ;
  vect_val       = rv.vect_val ;
  pMainValues    = rv.pMainValues ;
}

bool Cpmed_admindata::parser ()
{
	if (false == parser_attributs())
		return false ;

	int nb_sexe           = 0 ;
	int nb_date_naissance = 0 ;
	int nb_code_postal    = 0 ;

	if (false == vect_attr.empty())
  {
    if (false == vect_attr.traiterDate(ATTRIBUT_PMED_ADMINDATA_DATE_NAISSANCE, 6))
    {
      _iParsingError = EPMED_ADMIN_DATA_ATTR ;
      return false ; // erreur (attribut incorrect)
    }

    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      if (ATTRIBUT_PMED_ADMINDATA_SEXE == (*iattribut)->getLabel()) // sexe
      {
        nb_sexe++ ;
      }
      else if (ATTRIBUT_PMED_ADMINDATA_DATE_NAISSANCE == (*iattribut)->getLabel()) // date_naissance
      {
        nb_date_naissance++ ;
      }
      else if (ATTRIBUT_PMED_ADMINDATA_CODE_POSTAL == (*iattribut)->getLabel()) // code_postal
      {
        nb_code_postal++ ;
      }
      else
      {
        _iParsingError = EPMED_ADMIN_DATA_ATTR ;
        return false ; // erreur (attribut incorrect)
      }
    }
  }
  if ((1 != nb_sexe) || (nb_date_naissance > 1) || (nb_code_postal > 1))
  {
    _iParsingError = EPMED_ADMIN_DATA_NB ;
    return false ; // erreur (nb attributs incorrect)
  }

  if (false == parser_valeurs())
    return false ;

  if (false == vect_val.empty())
  {
    _iParsingError = EPMED_ADMIN_DATA_VAL ;
    return false ; // erreur (pas de valeur attendue)
  }

  return true ;
}

bool Cpmed_admindata::verif ()
{
  if ((vect_attr[ATTRIBUT_PMED_ADMINDATA_SEXE] != "1") ||
      (vect_attr[ATTRIBUT_PMED_ADMINDATA_SEXE] != "2"))
  {
    _iParsingError = EPMED_ADMIN_DATA_SEXE ;
    return false ;
  }

  return true ;
}

bool Cpmed_admindata::compresser()
{
  return true ;
}

bool Cpmed_admindata::traiter ()
{
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr << "  Admindata" << endl ;
#endif
#endif

  string values = string("'") + parent->databaseid + string("', '") +
                    vect_attr[string(ATTRIBUT_PMED_ADMINDATA_SEXE)] + string("', '") +
                    vect_attr[string(ATTRIBUT_PMED_ADMINDATA_DATE_NAISSANCE)] + string("', '") +
                    vect_attr[string(ATTRIBUT_PMED_ADMINDATA_CODE_POSTAL)] + string("'") ;

#if !defined _OWLDLL
	databaseid = nsmysql_insert("admindata", "id_admindata", "id_patient_message, admindata_sexe, admindata_naissance, admindata_code_post", values.c_str());
#else
  if (debugFile)
    debugFile << string("id_patient_message, admindata_sexe, admindata_naissance, admindata_code_post : ") + values + "\n";
#endif

#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
  cerr <<"  Fin admindata" <<endl;
#endif
#endif

	return true ;
}

Cpmed_admindata&
Cpmed_admindata::operator=(const Cpmed_admindata& src)
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

