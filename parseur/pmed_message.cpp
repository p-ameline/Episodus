#ifndef FOR_WINDOWS
  #include <sys/types.h>
  #include <sys/socket.h>

  #include <netinet/in.h>
  #include <netinet/tcp.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <unistd.h>
  #include <sys/ioctl.h>
#endif

#include <fstream.h>
#include <iostream.h>
#include <stdlib.h>
#include <stdio.h>

#include <time.h>
#include <string>
#include <errno.h>

#include "balise.h"
#include "pmed_message_labels.h"
#include "ParseError.h"
#include "nsmysql.h"
#include "mail_transfert.h"

Cpmed_message::Cpmed_message(string attributs, string values, int cfd)
              :Cbalise (attributs, values)
{
  connfd      = cfd ;
  pMainValues = new Cconteneur ;
}

Cpmed_message::~Cpmed_message ()
{
  delete pMainValues ;
}

bool Cpmed_message::parser ()
{
  // *** parsing attributs ***

  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    _iParsingError = EPMED_MESSAGE_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

	int nb_senderid = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
  {
		if (ATTRIBUT_PMED_MESSAGE_SENDERID == (*iattribut)->getLabel()) // senderid
		{
			pMainValues->send_id = (*iattribut)->getRawValue() ;
			nb_senderid++ ;
		}
		else
    {
      _iParsingError = EPMED_MESSAGE_ATTR ;
      return false ; // erreur (attribut incorrect)
    }
  }
	if (1 != nb_senderid)
  {
    _iParsingError = EPMED_MESSAGE_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

	// *** parsing valeurs ***

	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    _iParsingError = EPMED_MESSAGE_VAL_EMPTY ;
    return false ; // erreur (pas de valeur)
  }

  int nb_pmed_patient = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if (LABEL_PMED_PATIENT == (*ival)->getLabel()) // pmed_patient
		{
      (*ival)->setObject(new Cpmed_patient((*ival)->getAttribute(), (*ival)->getValue(), pMainValues)) ;
			if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ; // erreur parsing pmed_patient
      }
			nb_pmed_patient++ ;
		}
		else
    {
      _iParsingError = EPMED_MESSAGE_VAL ;
      return false ; // erreur (valeur incorrecte)
    }
  }

	if (nb_pmed_patient < 1)
  {
    _iParsingError = EPMED_MESSAGE_NBVAL ;
    return false ; // erreur (nb valeurs incorrect)
  }

	return true ;
}

bool Cpmed_message::verif ()
{
  // Verification que l'émetteur appartient bien à l'étude

  char* pResult = 0 ;

  string sqlquery = string("SELECT medecin_classification FROM medecin WHERE id_medecin='") + pMainValues->send_id + string("'") ;

#ifndef FOR_WINDOWS

  int iResult = nsmysql_query(sqlquery.c_str(), pResult, true /*bVerbose*/) ;

  if ((0 == iResult) || ((char*) NULL == pResult) || (strcmp(pResult, "1") == 0))
  {
    _iParsingError = EPMED_UNKNOWN_SENDER ;
    return false ;
  }

#endif

  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    if (false == (*ival)->getObject()->verif())
      return false ;

  return true ;
}

bool Cpmed_message::compresser()
{
  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; ival != vect_val.end(); ival++)
  {
    ValIter iSousVal = ival ;
    iSousVal++ ;

    while (vect_val.end() != iSousVal)
    {
      // Même patient - Same patient
      if ((*ival)->getAttribute() == (*iSousVal)->getAttribute())
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

bool Cpmed_message::traiter ()
{
  if (vect_val.empty())
    return true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if (false == (*ival)->getObject()->traiter())
      return false ;

    //
    // Pour rappeler à sendmail qu'on est encore vivant - sorte de heartbeat pour empécher une deconnexion automatique
    //
#if !defined _OWLDLL
#if !defined _MESSAGE_NOVERBOSE
    fprintf(stderr, "Mailserveur file descriptor : %i\n", connfd);
    cerr << "Sendmail noop... ";
#endif

    char buff[255] ;
    int  buffsize = 254 ;

    write(connfd, "NOOP\r\n", 6) ;
    receive_one_line(connfd, buff, buffsize) ;

#if !defined _MESSAGE_NOVERBOSE
    cerr << buff << endl;
#endif
#endif
  }

  return true ;
}

