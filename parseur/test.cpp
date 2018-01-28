#include <fstream>
#include "nsmysql.h"
#include "balise.h"
#include "pre_parseur.h"
#include "mail_transfert.h"
#include <iostream.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include "../module0/module0/lifeline.h"

#include "ParseError.h"

#include "./file_names.h"

string parseErrorMessage (int iError);
bool error_ecriture (int*, lifeLine* );
bool parse_message (string, lifeLine* );
bool log_message_error (int* piError, string sAttribut);
bool log_message_succes (Cpmed_message* pMedMessage, bool bTest);

#define XML_FILE_NAME "test.xml"

bool parse_message_with_tag (string message, string tag, int* iParsingError, int connfd)
{
    int message_start, message_end, header_end;
    string attributes, values;

    message_start = message.find ("<" + tag);
    if (message_start != string::npos)
    {
        message_end = message.find ("</" + tag + ">");
        if (message_end == string::npos)
        {
            *iParsingError = EMESSAGE_BALISE_FIN;
            return false; // erreur (pas de balise de fin)
        }
        header_end = message.find (">", message_start);
        if (message_end < header_end)
        {
            *iParsingError = EMESSAGE_BALISE_DEBUT_FERMANTE;
            return false; // erreur (pas de fin de balise de debut)
        }

        attributes = string (message, message_start + tag.length () + 2, header_end - message_start -  tag.length ()  - 2);
        values = string (message, header_end + 1, message_end - header_end - 1);
    }
    else
    {
        *iParsingError = EMESSAGE_BALISE_DEBUT;
        return false; // erreur (pas de balise de debut)
    }

    Cpmed_message pmed_message (attributes, values, connfd);

    if (! pmed_message.parser ())
    {
        cerr <<"*** ECHEC PARSING ***" <<endl;
        *iParsingError = pmed_message.iParsingError;
        log_message_error (iParsingError, attributes);
        return false;
    }

    //
    //
    //
    bool bTest;

    if (pmed_message.pMainValues->send_id != "$promethe$")
    {

        bTest = false;

        cerr <<"parsing OK" <<endl;

        nsmysql_connect ();

        if (! pmed_message.verif ())
        {
            cerr <<"*** ECHEC VERIFICATION ***" <<endl;
            return false;
        }
        cerr <<"verification OK" <<endl;

        if (! pmed_message.compresser())
        {
            cerr <<"*** ECHEC COMPRESSION ***" <<endl;
            return false;
        }
        cerr <<"compression OK" <<endl;

        if (! pmed_message.traiter ())
        {
            cerr <<"*** ECHEC TRAITEMENT ***" <<endl;
            return false;
        }
        cerr <<"traitement OK" <<endl;

        nsmysql_close ();
    }
    else
        bTest = true;

    log_message_succes (&pmed_message, bTest);

    cerr <<"Message OK" <<endl;

    return true;
}

bool error_ecriture (int* iParsingError, lifeLine* pLifeLine)
{
  ofstream outfile;

  // ecriture dans
  string sName = string ("ErrorParseur.msg");
  outfile.open (sName.c_str(), ofstream::app);

  string sParseError = parseErrorMessage (*iParsingError);

  if (outfile)
    {
      outfile << "****************************** Erreur a ete rencontre lors du parsing : " + sParseError + "*******************************\n";
      outfile.close();

      // ecriture de l'erreur dans trace.log
      string sMess = string("Message incorrect : ") + sParseError + string(" (message stocké dans le fichier : ErrorParseur.msg)");
      pLifeLine->trace(sMess,  1, lifeLine::logError);

      return true;
    }
  else
    return false;
}

bool log_message_error (int* piError, string sAttribut)
{
        ofstream outfile;

        // ecriture dans
        string sName = string ("Messages.log");
        outfile.open (sName.c_str(), ofstream::app);

        if (!outfile)
	        return false;

        struct tm* tp;
        long t;
        char msg[255];

        time (&t);
        tp = localtime (&t);
        strcpy (msg, asctime (tp));

        string sDebut = string(msg, 0, strlen(msg)-1) + string(" : ** ") + sAttribut + string(" Erreur : ") + parseErrorMessage (*piError);

        outfile << sDebut + "\n";

        outfile.close();

        return true;
}

bool log_message_succes (Cpmed_message* pMedMessage, bool bTest)
{
        ofstream outfile;

        // ecriture dans
        string sName = string ("Messages.log");
        outfile.open (sName.c_str(), ofstream::app);

        if (!outfile)
	        return false;

        struct tm* tp;
        long t;
        char msg[255];

        time (&t);
        tp = localtime (&t);
        strcpy (msg, asctime (tp));

        string sResultat;
        if (bTest)
                sResultat = string(" : test ok ");
        else
                sResultat = string(" : ok ");

        string sDebut = string(msg, 0, strlen(msg)-1) + sResultat + pMedMessage->pMainValues->send_id;

        Val_iter ival;

        if (!(pMedMessage->vect_val.empty()))
        {
                sDebut += string(" pat(s) :");
                for (ival = pMedMessage->vect_val.begin (); ival != pMedMessage->vect_val.end (); ival++)
                        sDebut += string(" ") + (*ival)->pObject->pMainValues->pat_id;
        }

        outfile << sDebut + "\n";

        outfile.close();

        return true;
}

bool parse_message (string message, lifeLine* pLifeLine, int connfd)
{
  int iParsingError;
  bool resultat;
  bool resultat_ecriture;

	if (message == "")
	  {
	    iParsingError = EMESSAGE_EMPTY;
	    return false; // erreur (message vide)
	  }
	resultat = parse_message_with_tag (message, "pmed_message", &iParsingError, connfd); // seul balise racine actuellement


	/* retourner resultat a la fin de la fonction , il signifi (true) que le message a ete correctement parse , (false) sinon. */

	if (!resultat)
	  {
	    resultat_ecriture = error_ecriture (&iParsingError, pLifeLine);
	    return false;
	  }
	else
	  {
	    // le parseur n'a rencontre aucun probleme
	    return true;
	  }
}

long int get_noSocket(char **argv)
{
	return (strtol(argv[1], NULL, 10));
}


int main (int argc, char **argv)
{
	struct	tm *tp;
	long		t;
	bool		temps = true;
	bool		boucle = true;
	int		chrono;
	int		chrono2;
	long int	noSocket = -1;		// utilise pour la lecture de message a partir le superviseur
	char 		sMessage[1024];


	lifeLine* pLifeLine = new lifeLine();

	pLifeLine->trace("running", 1, lifeLine::logMessage);

 	if (argc < 2)
 	{
 		pLifeLine->trace("pas assez d'argument erreur dans superviseur -- section scheduler lors du lancement du module",  1, lifeLine::logError);
 		sprintf(sMessage, "argc = %d", argc);
 		pLifeLine->trace(sMessage, 2, lifeLine::logError);
 		if (argc > 1)
 		{
 			for (int i = 1; i < argc; i++)
 			{
 				sprintf(sMessage, "argv[ %d] = %s", i, argv[i]);
 				pLifeLine->trace(sMessage, 2, lifeLine::logError);
	 		}
	 	}
	 	pLifeLine->erreur("ending", 1);
 	}

	noSocket = get_noSocket(argv);											// recupere le no de la socket

 	pLifeLine->setIdentifier(argv[0]);

       	pLifeLine->setSockProtocol(noSocket);
 	pLifeLine->setSockName(string(argv[0]));
 	pLifeLine->setTraceLevel(10);


 	if (!(pLifeLine->openConnection()))
 		pLifeLine->erreur("openConnection failed", 1);

 	pLifeLine->trace("openConnection succeeded", 5, lifeLine::logMessage);



	// indication au superviseur que le module est toujours vivant
	pLifeLine->heartBeat();

	parse_mails (pLifeLine);

	pLifeLine->goodbye();
}

/*
int main ()
{
	ifstream xml_file;
	string tmp_xml_string;
	string xml_string;
	int counter;
	bool reponse;

	xml_file.open (XML_FILE_NAME);
	while (getline (xml_file, tmp_xml_string))
		xml_string += tmp_xml_string + '\n';
	xml_file.close ();

	reponse = pre_parseur (xml_string);
	cerr << "resultat pre-parsing : " <<reponse <<endl;
	if (reponse==false)
		return false;
	if (parse_message (xml_string) == false)
		return false;
	return true;
}
*/
