#include <owl\olemdifr.h>#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nssavoir\nslangue.h"
#include "nssavoir\Nspatho.h"
#include "nssavoir\Nsfilgd.h"
#include "ns_sgbd\nsfiche.h"
#include "nscompta\nscpta.h"

#include "nscompta\nsnoemie.h"

#include "nscompta\nscompta.h"

#include "nscompta\nsfse16.h"
#include "nsbb\nsednum.h"
#include "nsbb\nspatbd.h"
#include "nsbb\nspatpat.h"

#include "partage\nsperson.h"

#include "nscompta\nscompta.rh"

//********************************************************************//

DEFINE_RESPONSE_TABLE1(NSNoemie, TDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_BN_CLICKED(IDC_LANCER, CmLancer),
END_RESPONSE_TABLE;

NSNoemie::NSNoemie(TWindow* parent, NSContexte* pCtx)         :TDialog(parent, "IDD_NOEMIE", pNSResModule), NSRoot(pCtx)
{
	pFichierRsp = new TEdit(this, IDC_FICHIER_RSP) ;
	pLancer     = new TButton(this, IDC_LANCER) ;
}

NSNoemie::~NSNoemie(){
	delete pFichierRsp ;
	delete pLancer ;
}

voidNSNoemie::CmOk()
{
	CloseWindow(IDOK);
}

void
NSNoemie::CmCancel()
{
	Destroy(IDCANCEL);
}

void
NSNoemie::CmLancer()
{
	Moulinette() ;

	return ;

/*
    char szFichierRsp[1024];
    pFichierRsp->GetText(szFichierRsp, 1024);

    if (szFichierRsp[0] == '\0')
        return;

    string sFichierRsp = string(szFichierRsp);

    Traiter(sFichierRsp);
*/
}

unsigned intNSNoemie::Moulinette()
{
try
{
	string          sPrefixe;
	string          sSuffixe;
	string          sFichierRsp;
	string          sFichierLog;
	string          sChemin = pContexte->PathName("INOE");

	char 		    *fichRecherche;
	WIN32_FIND_DATA stRecherche;
	HANDLE			hFichierTrouve;

	//
	// Préparation d'une ammorce générique du type *.Rsp
	//

	//
	// Préparation du nom de fichier complet
	//
	fichRecherche = new char[strlen(sChemin.c_str())+6];
	strcpy(fichRecherche, sChemin.c_str());
	strcat(fichRecherche, "*.rsp");

	//
	// Lancement de la recherche du premier fichier correspondant
	//                  FindFirstFile
	hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche);
	delete[] fichRecherche;
	//
	// Si le fichier n'existe pas on renvoie le numéro de référence
	//

	if (hFichierTrouve == INVALID_HANDLE_VALUE)	{		string sErreur = "Il n'existe aucun fichier Noémie dans le répertoire " + sChemin;
		erreur(sErreur.c_str(), standardError, 0) ;
		return(0);
	}
	//
	// Boucle tant qu'on trouve des fichiers .rsp
	//
	BOOL bTrouve = TRUE;
	while (bTrouve)
	{
		string sFichierRsp = string(stRecherche.cFileName);

		string sSuffixe = string(sFichierRsp, strlen(sFichierRsp.c_str()) - 4, 4) ;
		for (size_t i = 0; i < strlen(sSuffixe.c_str()); i++)
			sSuffixe[i] = pseumaj(sSuffixe[i]) ;

		if (sSuffixe == ".RSP")
		{
			string sPrefixe = string(sFichierRsp, 0, strlen(sFichierRsp.c_str()) - 4) ;
			//string sFile    = sPrefixe + string(".log");

			DWORD   nBufferLength = 254;	// size, in characters, of buffer
			char    szResult[255];	// address of buffer for found filename
			LPTSTR  lpFilePart;   // address of pointer to file component

			DWORD iLength = ::SearchPath(sChemin.c_str(), sPrefixe.c_str(), ".log",
                                        nBufferLength, szResult, &lpFilePart) ;
			//
			// Le fichier .log n'a pas été trouvé
			//
			if (iLength == 0)
			{
				pFichierRsp->SetText(sFichierRsp.c_str());
				Traiter(sChemin + sFichierRsp);
			}
		}

		/*stRecherche.cFileName = sFichierLog.c_str() ;
		/*noemie sur stRecherche.cFileName */

		bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;
	}
	//
	// On renvoie le numéro de référence
	//
	FindClose(hFichierTrouve) ;

	pFichierRsp->SetText("traitement terminé") ;

	return(0) ;
}
catch (...)
{
	erreur("Exception NSNoemie.", standardError, 0) ;
  return 1 ;
}
}

voidNSNoemie::AjouteErreur(string sErreur, string sFichierRsp){	string sFichierErr;	ofstream outFile;

	size_t pos = sFichierRsp.find('.');
	if (pos != NPOS)		sFichierErr = string(sFichierRsp, 0, pos+1) + string("err") ;	else
		sFichierErr = sFichierRsp + string(".err") ;

	outFile.open(sFichierErr.c_str(), ios::app) ;
	if (!outFile)	{
		string sErr = "Erreur d'ouverture du fichier " + string(sFichierErr) ;
		erreur(sErr.c_str(), standardError, 0) ;
	}
	else
	{
		outFile << sErreur ;
		outFile.close() ;
	}
}
voidNSNoemie::Traiter(string sFichierRsp)
{
try
{
#ifndef _MUE

	ifstream inFile ;
	string sLine = "" ;

	inFile.open(sFichierRsp.c_str()) ;
	if (!inFile)	{
		string sErreur = "Erreur d'ouverture du fichier " + string(sFichierRsp) ;
		erreur(sErreur.c_str(), standardError, 0) ;
		return ;
	}

	string sData ;	string sEntite ;
	string sNiveau ;
	string sDonnees ;
	int    iDataLength ;

	while (!inFile.eof())	{
		getline(inFile, sLine) ;

		if ((strlen(sLine.c_str()) > 3) && (string(sLine, 0, 3) == string("000")))		{
			while (strlen(sLine.c_str()) > 3)
			{
				if ( (string(sLine, 0, 3) == string("000")) ||
				     (string(sLine, 0, 3) == string("999")) )
				{
					sData = string(sLine, 0, 128) ;
					sEntite = string(sData, 0, 3) ;
					sNiveau = string(sData, 3, 2) ;
					sDonnees = string(sData, 0, strlen(sData.c_str())) ;
				}
				else
				{
					size_t pos = sLine.find('@');

					if (pos != NPOS)						sData = string(sLine, 0, pos+1) ;
					else
						break ;

					if (strlen(sData.c_str()) >= 5)					{
						sEntite = string(sData, 0, 3) ;
						sNiveau = string(sData, 3, 2) ;
						sDonnees = string(sData, 0, strlen(sData.c_str())-1 ) ;
					}
					else
					{
						sEntite = "" ;
						sNiveau = "" ;
						sDonnees = "" ;
					}
				}

				if (sEntite != "")					aEntites.push_back(new NSEntiteNoemie(sEntite, sNiveau, sDonnees)) ;

				iDataLength = strlen(sData.c_str()) ;
				sLine = string(sLine, iDataLength, strlen(sLine.c_str())-iDataLength) ;			}
		}
	}

	inFile.close() ;
	if (aEntites.empty())		return ;

	string sFichierLog, sErr = "" ;
	ofstream outFile ;

	size_t pos = sFichierRsp.find('.') ;
	if (pos != NPOS)		sFichierLog = string(sFichierRsp, 0, pos+1) + string("log") ;	else
		sFichierLog = sFichierRsp + string(".log") ;

	bool bLogOK = true ;	outFile.open(sFichierLog.c_str()) ;
	if (!outFile)	{
		string sErreur = "Erreur d'ouverture du fichier " + string(sFichierLog) ;
		erreur(sErreur.c_str(), standardError, 0) ;
		bLogOK = false ;
	}

	string sDate        = "" ;
	string sNom         = "" ;
	string sPrenom      = "" ;
	string sDateNaiss1  = "" ;	string sDateNaiss2  = "" ;	string sPatNumero   = "" ;

	string sMontantRemb = "" ;	string sSigneActe   = "" ;
	string sActe        = "" ;
	string sQteActe     = "" ;
	string sCoefActe    = "" ;

	NSUtilisateurChoisi* pUtilisateur = pContexte->getUtilisateur() ;	TWindow* pMainWin = pContexte->getSuperviseur()->donneMainWindow() ;

	for (NSNoemieArrayIter i = aEntites.begin(); i != aEntites.end(); i++)	{
		if  ((*i)->sEntite == string("120"))
		{
			sNom = string((*i)->sDonnee, 12, 25) ;
			sPrenom = string((*i)->sDonnee, 37, 15) ;

			// Date de naissance : position 6 JJMMAA
			string sDateNaiss =  string((*i)->sDonnee, 9, 2)
                                        + string((*i)->sDonnee, 7, 2)
                                        + string((*i)->sDonnee, 5, 2);

			// on considère 2 dates de naissance à un siècle d'écart
			// car les fichiers noemie n'indiquent pas le siècle...
			sDateNaiss1 = string("19") + sDateNaiss ;
			sDateNaiss2 = string("20") + sDateNaiss ;

			sPatNumero = "" ;
			// trouver le patient			if (!(pUtilisateur->pPatRech))
				pUtilisateur->pPatRech = new NSPatient(pContexte) ;

			NSPatient* pPat = pUtilisateur->pPatRech ;
      string sLog = "" ;
      if (cherchePat(pPat, sNom, sPrenom, sDateNaiss, sErr, sLog))
				sPatNumero = pPat->pDonnees->nss ;
      else
				sPatNumero = "" ;

			if (sErr != "")
			{
				if (bLogOK)
					outFile << sErr ;				AjouteErreur(sErr, sFichierRsp) ;			}
      if ((sLog != "") && bLogOK)
      	outFile << sLog ;

			delete pPat ;
			pUtilisateur->pPatRech = 0 ;

/*
			pPat->lastError = pPat->open() ;
			if (pPat->lastError != DBIERR_NONE)			{
				erreur("Erreur à l'ouverture du fichier Patient.db", 0, pPat->lastError, pMainWin->GetHandle());
				delete pPat;
				pUtilisateur->pPatRech = 0;
				sPatNumero = "";

				sErr = string("Erreur d'ouverture de la base Patient.db => Patient ") + sNom + string(" ") + sPrenom + string(" introuvable") + string(1, '\n');				AjouteErreur(sErr, sFichierRsp);			}
			else
			{
				// Recherche/création

				DBIResult   ErrDBI;				CURProps    curProps;
				Byte*       pIndexRec;
				int         idRet;

				// on enlève les blancs terminaux des nom et prénom				strip(sNom, stripRight);
				strip(sPrenom, stripRight);

				char nomPat[PAT_NOM_LEN + 1];				char prenomPat[PAT_PRENOM_LEN + 1];

				strcpy(nomPat, sNom.c_str());
				strcpy(prenomPat, sPrenom.c_str());

				// on recherche ce patient dans la base
				ErrDBI = DbiGetCursorProps(pPat->PrendCurseur(), curProps);
				pIndexRec = new Byte[curProps.iRecBufSize];
				memset(pIndexRec, 0, curProps.iRecBufSize);
                DbiPutField(pPat->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)nomPat);                DbiPutField(pPat->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)prenomPat);

                // on lance cette fois une recherche exacte
                ErrDBI = pPat->chercheClefComposite("NOM_PRENOM",                                                    NODEFAULTINDEX,
                                                    keySEARCHEQ,
                                                    dbiWRITELOCK,
                                                    pIndexRec);

                delete[] pIndexRec;
                if (ErrDBI == DBIERR_NONE)
                {
                    pPat->lastError = pPat->getRecord();

                    if (pPat->lastError != DBIERR_NONE)                    {
                        erreur("Erreur à lecture du fichier Patient.db", 0, pPat->lastError, pMainWin->GetHandle());
                        pPat->close();
                        delete pPat;
                        pUtilisateur->pPatRech = 0;
                        sPatNumero = "";

                        sErr = string("Erreur de lecture de la base Patient.db => Patient ") + sNom + string(" ") + sPrenom + string(" introuvable") + string(1, '\n');                        AjouteErreur(sErr, sFichierRsp);                    }
                    else
                    {
                        // Vérifier qu'il n'y en a qu'un
                        sPatNumero = pPat->pDonnees->nss;
                        pPat->close();
                        delete pPat;
                        pUtilisateur->pPatRech = 0;

                        if(bLogOK)                            outFile << string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n');
                    }
                }
                //
                // S'il y a des caractères accentués, la recherche peut échouer
                // il faut vérifier que l'enregistrement précédent n'est pas le
                // bon
                //
                else
                {
                    bool bChercher = true;

                    pPat->lastError = pPat->suivant(dbiWRITELOCK);
                    if (pPat->lastError != DBIERR_EOF)
                    {
                        // On vérifie l'enregistrement en cours
                        pPat->lastError = pPat->getRecord();

                        if (pPat->lastError != DBIERR_NONE)                        {
                            erreur("Erreur à lecture du fichier Patient.db", 0, pPat->lastError, pMainWin->GetHandle());
                            pPat->close();
                            delete pPat;
                            pUtilisateur->pPatRech = 0;
                            sPatNumero = "";
                        }
                        else
                        {
                            // Est-ce le bon
                            string sTestNom     = string(pPat->pDonnees->nom);
                            string sTestPrenom  = string(pPat->pDonnees->prenom);
                            strip(sTestNom,     stripRight);
                            strip(sTestPrenom,  stripRight);

                            for (int i = 0; i < strlen(sTestNom.c_str()); i++)                                sTestNom[i] = pseumaj(sTestNom[i]);

                            for (int i = 0; i < strlen(sTestPrenom.c_str()); i++)                                sTestPrenom[i] = pseumaj(sTestPrenom[i]);

                            if (strlen(sTestNom.c_str()) > 25)                                sTestNom = string(sTestNom, 0, 25);

                            if (strlen(sTestPrenom.c_str()) > 15)                                sTestPrenom = string(sTestPrenom, 0, 15);

                            if ((sTestNom == sNom) && (sTestPrenom == sPrenom))                            {
                                // Vérifier qu'il n'y en a qu'un
                                sPatNumero = pPat->pDonnees->nss;
                                pPat->close();
                                delete pPat;
                                pUtilisateur->pPatRech = 0;

                                if(bLogOK)                                    outFile << string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n');

                                bChercher = false;                            }
                        }
                    }

                    if (bChercher)                    {
                        pPat->lastError = pPat->precedent(dbiWRITELOCK);

                        if (pPat->lastError == DBIERR_NONE)                        {
                            pPat->lastError = pPat->getRecord();

                            if (pPat->lastError != DBIERR_NONE)                            {
                                erreur("Erreur à lecture du fichier Patient.db", 0, pPat->lastError, pMainWin->GetHandle());
                                pPat->close();
                                delete pPat;
                                pUtilisateur->pPatRech = 0;
                                sPatNumero = "";
                            }
                            else
                            {
                                // Est-ce le bon
                                string sTestNom     = string(pPat->pDonnees->nom);
                                string sTestPrenom  = string(pPat->pDonnees->prenom);
                                strip(sTestNom,     stripRight);
                                strip(sTestPrenom,  stripRight);

                                for (int i = 0; i < strlen(sTestNom.c_str()); i++)                                    sTestNom[i] = pseumaj(sTestNom[i]);

                                for (int i = 0; i < strlen(sTestPrenom.c_str()); i++)                                    sTestPrenom[i] = pseumaj(sTestPrenom[i]);

                                if (strlen(sTestNom.c_str()) > 25)                                    sTestNom = string(sTestNom, 0, 25);

                                if (strlen(sTestPrenom.c_str()) > 15)                                    sTestPrenom = string(sTestPrenom, 0, 15);

                                if ((sTestNom == sNom) && (sTestPrenom == sPrenom))                                {
                                    // Vérifier qu'il n'y en a qu'un
                                    sPatNumero = pPat->pDonnees->nss;
                                    pPat->close();
                                    delete pPat;
                                    pUtilisateur->pPatRech = 0;

                                    if(bLogOK)                                        outFile << string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n');

                                    bChercher = false;                                }
                            }
                        }
                    }

                    if (bChercher)                    {
                        pPat->close();
                        delete pPat;
                        pUtilisateur->pPatRech = 0;
                        sPatNumero = "";

                        sErr = string("Patient ") + sNom + string(" ") + sPrenom + string(" : Patient inconnu") + string(1, '\n');                        if (bLogOK)                            outFile << sErr;                        AjouteErreur(sErr, sFichierRsp);                    }
                }
            }
*/
		}
		else if (((*i)->sEntite == string("210"))||
						 ((*i)->sEntite == string("212"))||
             ((*i)->sEntite == string("216")) )
		{
			bool bRembourse = false;

			// Montant remboursé : position 53, taille 8
			sMontantRemb = string((*i)->sDonnee, 52, 8) ;

			strip(sMontantRemb, stripLeft, '0');
			// Date des soins ou date début : position 22, taille 6			sDate = string("20") + string((*i)->sDonnee, 25, 2)
                                 + string((*i)->sDonnee, 23, 2)
                                 + string((*i)->sDonnee, 21, 2) ;

			sSigneActe = string((*i)->sDonnee, 60, 1);      sActe = string((*i)->sDonnee, 61, 3);
      sQteActe = string((*i)->sDonnee, 66, 3);
      sCoefActe = string((*i)->sDonnee, 69, 5);

			// Mettre compta à jour			NSCompt* 	pCompt = new NSCompt(pContexte);

			pCompt->lastError = pCompt->open();
			if (pCompt->lastError != DBIERR_NONE)			{
				erreur("Erreur à l'ouverture de la base Compt.", standardError, 0) ;
        delete pCompt ;

        sErr = string("Erreur à l'ouverture de la base Compt.") + string(1, '\n') ;        AjouteErreur(sErr, sFichierRsp) ;        return ;
			}

			if (sPatNumero != "")
				pCompt->lastError = pCompt->chercheClef(&sPatNumero,
                                                    "PATCOMPT",
                                                    NODEFAULTINDEX,
                                                    keySEARCHEQ,
                                                    dbiWRITELOCK);
			else
				// cas où on n'a pas trouvé le patient :
				// on recherche par rapport à la date des soins
        pCompt->lastError = pCompt->chercheClef(&sDate,
                                                    "DATE_COMPT",
                                                    NODEFAULTINDEX,
                                                    keySEARCHEQ,
                                                    dbiWRITELOCK);

			if ((pCompt->lastError != DBIERR_NONE) && (pCompt->lastError != DBIERR_RECNOTFOUND))			{				erreur("Erreur à la recherche de la fiche Compt du patient.", standardError, pCompt->lastError) ;

				sErr = string("Erreur à la recherche de la fiche Compt du patient.") + string(1, '\n') ;
				AjouteErreur(sErr, sFichierRsp) ;
			}
			else if (pCompt->lastError == DBIERR_RECNOTFOUND)
			{
				string sErreur = string("Le patient ") + sNom + string(" ") + sPrenom + string(" n'a pas de fiche comptable.") ;
				erreur(sErreur.c_str(), standardError, 0) ;

        sErr = string("Pas de fiche comptable pour ") + sNom + string(" ") + sPrenom + string(1, '\n') ;        if (bLogOK)        	outFile << sErr ;        AjouteErreur(sErr, sFichierRsp) ;      }
      else
      {
      	pCompt->lastError = pCompt->getRecord() ;

        if (pCompt->lastError != DBIERR_NONE)        {
        	erreur("Erreur à la lecture d'une fiche Compt.", standardError, pCompt->lastError) ;

          sErr = string("Erreur à la lecture d'une fiche Compt.") + string(1, '\n') ;          AjouteErreur(sErr, sFichierRsp) ;        }

        while ((pCompt->lastError == DBIERR_NONE) &&        				(!bRembourse) &&                ((sPatNumero == "") ||                   (string(pCompt->pDonnees->patient) == sPatNumero)))        {

        	if (string(pCompt->pDonnees->date) == sDate)          {
          	double dMontantRemb = StringToDouble(sMontantRemb) ;
            double dduEuroBase = StringToDouble(string(pCompt->pDonnees->duEuro)) ;
            double dpayeEuroBase = StringToDouble(string(pCompt->pDonnees->payeEuro)) ;

            //            // Si un tel acte existe
            //

            if (existeActe(pCompt->pDonnees->numcompt, sActe, sQteActe, sCoefActe))            {
            	//
              // S'il reste cette somme à rembourser
              //

              if (dMontantRemb <= dduEuroBase - dpayeEuroBase)              {
              	bool bPatientValide = false;
                string sNomTrouve = "";
                string sPrenomTrouve = "";

                if (sPatNumero == "")
                {
                	// on vérifie ici par la date de naissance du fichier RSP
                  // si le patient est un patient valide (de même date de naissance)
                  string sNewNss = string(pCompt->pDonnees->patient) ;

                  // trouver le patient
                  if (!(pUtilisateur->pPatRech))
                  	pUtilisateur->pPatRech = new NSPatient(pContexte);

                  NSPatient* pPat = pUtilisateur->pPatRech;
                  pPat->lastError = pPat->open();
                  if (pPat->lastError != DBIERR_NONE)                  {
                  	erreur("Erreur à l'ouverture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle());
                    // pas de log d'erreur
                  }
                  else
                  {
                  	// Recherche/création
                    pPat->lastError = pPat->chercheClef(&sNewNss,
												                            "",
												                            0,
												                            keySEARCHEQ,
												                            dbiWRITELOCK);

                    if ((pPat->lastError != DBIERR_NONE) && (pPat->lastError != DBIERR_RECNOTFOUND))
                    {
                    	erreur("Erreur à la recherche du patient de la fiche Compt.", standardError, pPat->lastError);
                      // pas de log d'erreur
                    }
                    else if (pPat->lastError != DBIERR_RECNOTFOUND)
                    {
                    	pPat->lastError = pPat->getRecord();

                      if (pPat->lastError != DBIERR_NONE)                      {
                      	erreur("Erreur à lecture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle());
                        // pas de log d'erreur
                      }
                      else
                      {
                      	string sDateNaissPat = string(pPat->pDonnees->naissance);

                        if ((sDateNaissPat == sDateNaiss1) || (sDateNaissPat == sDateNaiss2))
                        {
                        	// on a trouvé un patient de même date de naissance
                          // pour un examen effectué le même jour
                          bPatientValide = true;
                          sNomTrouve = string(pPat->pDonnees->nom);
                          sPrenomTrouve = string(pPat->pDonnees->prenom);

                          if (bLogOK)
                          	outFile << string("Patient ") + sNom + string(" ") + sPrenom + string(" : Recherche par date de naissance => Trouvé pour ")
                                                                    + string(sNomTrouve) + string(" ") + string(sPrenomTrouve) + string(1, '\n') ;
                        }
                      }
                    }
                  }

                  pPat->close() ;
                  delete pPat ;
                  pUtilisateur->pPatRech = 0 ;
                }
                else
                {
                	// Le patient a été trouvé dans Nautilus
                  bPatientValide = true;
                }

                if (bPatientValide)
                {
                	CreerFact(string(pCompt->pDonnees->numcompt),
                            string (pContexte->getUtilisateur()->pDonnees->indice),
                            sDate, "", "",
                            sMontantRemb, string("EUR"), string("V"));

                  dpayeEuroBase += dMontantRemb ;
                  itoa(dpayeEuroBase, pCompt->pDonnees->payeEuro, 10) ;
                  //pCompt->pDonnees->payeEuro dpayeEuroBase  ;
                  strcpy(pCompt->pDonnees->okPaye, "1") ;
                  pCompt->lastError = pCompt->modifyRecord(TRUE ) ;
                  bRembourse = true ;
                  if (sPatNumero != "")
                  {
                  	// tout s'est bien passé : on genere un log
                    if (bLogOK)                    	outFile << string("Remboursement effectué ") + sNom + string(" ") + sPrenom + string(" ") + sMontantRemb + string(" ")
                                            + string("EUR") + string(" ") + sSigneActe + string(" ") + sQteActe + string("*") + sActe + sCoefActe + string(" ") + sDate + string(" ")
                                            + string(1, '\n');
                  }
                  else
                  {
                  	// ici le remboursement a été effectué pour le patient
                    // sNomTrouve, sPrenomTrouve, de même date de naissance
                    // on genere donc un log d'erreur
                    sErr = string("Remboursement effectué ") + sNom + string(" ") + sPrenom + string(" ")
                                            + string("pour : ") + sNomTrouve + string(" ") + sPrenomTrouve + string(" [même date de naissance] = ") + sMontantRemb + string(" ")
                                            + string("EUR") + string(" ") + sSigneActe + string(" ") + sQteActe + string("*") + sActe + sCoefActe + string(" ") + sDate + string(" ")
                                            + string(1, '\n');

                    if (bLogOK)
                    	outFile << sErr;

                    AjouteErreur(sErr, sFichierRsp);
                  }
                }
              }
              //
              // Si le montant du remboursement est supérieur au reste du
              // on peut supposer que l'acte a déjà été payé, si on connait le patient.
              // Si on ne connait pas le patient dans Nautilus, on ne genere pas de log.
              //
              else if ((sPatNumero != "") && (bLogOK))
              {
              	if (dduEuroBase > dpayeEuroBase)
                	outFile << string("Un acte similaire existe pour ") + sNom + string(" ") + sPrenom + string(" mais possède un reste dû insuffisant") + string(1, '\n');
                else
                	outFile << string("Un acte similaire existe pour ") + sNom + string(" ") + sPrenom + string(" mais est déjà entièrement réglé") + string(1, '\n');
              }
            }
          }
          else if (sPatNumero == "")          {          	// on fait ici une recherche par date, donc il est            // inutile de continuer car la date COMPT est dépassée            break;          }
          // ... on passe à la fiche compt suivante          pCompt->lastError = pCompt->suivant(dbiWRITELOCK) ;

          if (pCompt->lastError == DBIERR_NONE)          {
          	pCompt->lastError = pCompt->getRecord();

            if (pCompt->lastError != DBIERR_NONE)            {
            	erreur("Erreur à la lecture d'une fiche Compt.", standardError, pCompt->lastError);

              sErr = string("Erreur à la lecture d'une fiche Compt.") + string(1, '\n');              AjouteErreur(sErr, sFichierRsp);            }
          }
          else if (pCompt->lastError != DBIERR_EOF)
          {
          	erreur("Erreur d'acces à une fiche Compt.", standardError, pCompt->lastError);

            sErr = string("Erreur d'acces à une fiche Compt.") + string(1, '\n');            AjouteErreur(sErr, sFichierRsp);          }
        }
      }

      pCompt->close() ;
      if (pCompt->lastError != DBIERR_NONE)      	erreur("Erreur de fermeture du fichier Compt.", standardError, pCompt->lastError) ;

      delete pCompt ;
      if (!bRembourse)      {      	if (sPatNumero != "")        	sErr = sNom + string(" ") + sPrenom  + string(", la somme de ")+  sMontantRemb + string(" ne correspond à aucune fiche ")                            + string("(") + sSigneActe + string(" ") + sQteActe + string("*") + sActe + sCoefActe + string(" ")+ sDate + string(")") + string(1, '\n');
        else
        	sErr = sNom + string(" ") + sPrenom  + string(", la somme de ")+  sMontantRemb + string(" ne correspond à aucune fiche à la date du jour de l'acte ")
                            + string("(") + sSigneActe + string(" ") + sQteActe + string("*") + sActe + sCoefActe + string(" ")+ sDate + string(")") + string(1, '\n');

        if (bLogOK)
        	outFile << sErr;
        AjouteErreur(sErr, sFichierRsp);
      }
    }
		else if (((*i)->sEntite == string("990")) && ((*i)->sNiveau == string("04")))
		{
			sNom = "" ;
			sPrenom = "" ;
		}
		else if (((*i)->sEntite == string("990")) &&
		         ((*i)->sNiveau == string("02")))
		{
			sDate = "" ;
		}
	}

	outFile.close() ;
	//	// On vide l'array d'entités
	//
	aEntites.vider() ;#else	erreur("Fonction NSNoemie::Traiter non intégrée dans MUE.", 0, 0);#endif}
catch (...)
{
	erreur("Exception NSNoemie::Traiter.", standardError, 0) ;
}
}

boolNSNoemie::cherchePat(NSPatient* pPat, string sNom, string sPrenom, string sDateNais, string sErreur, string sLog){#ifndef N_TIERS	sErreur = "" ;  sLog = "" ;	if (!pPat)		return false ;	DBIResult   ErrDBI ;	CURProps    curProps ;
	Byte*       pIndexRec ;
	int         idRet ;

	TWindow* pMainWin = pContexte->getSuperviseur()->donneMainWindow() ;

	pPat->lastError = pPat->open() ;

	if (pPat->lastError != DBIERR_NONE)	{
		erreur("Erreur à l'ouverture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle()) ;

		sErreur = string("Erreur d'ouverture de la base Patient.db => Patient ") + sNom + string(" ") + sPrenom + string(" introuvable") + string(1, '\n') ;    return false ;	}

	// on enlève les blancs terminaux des nom et prénom	strip(sNom, stripRight) ;	strip(sPrenom, stripRight) ;

	char nomPat[PAT_NOM_LEN + 1] ;	char prenomPat[PAT_PRENOM_LEN + 1] ;

	strcpy(nomPat, sNom.c_str()) ;
	strcpy(prenomPat, sPrenom.c_str()) ;

	// on recherche ce patient dans la base
	ErrDBI = DbiGetCursorProps(pPat->PrendCurseur(), curProps) ;
	pIndexRec = new Byte[curProps.iRecBufSize] ;	memset(pIndexRec, 0, curProps.iRecBufSize) ;

	DbiPutField(pPat->PrendCurseur(), PAT_NOM_FIELD, pIndexRec, (Byte*)nomPat) ;	DbiPutField(pPat->PrendCurseur(), PAT_PRENOM_FIELD, pIndexRec, (Byte*)prenomPat) ;

	// on lance cette fois une recherche exacte
	ErrDBI = pPat->chercheClefComposite("NOM_PRENOM",																			NODEFAULTINDEX,
                                      keySEARCHEQ,
                                      dbiWRITELOCK,
                                      pIndexRec) ;

	delete[] pIndexRec;
	if (ErrDBI == DBIERR_NONE)
	{
		pPat->lastError = pPat->getRecord();

		if (pPat->lastError != DBIERR_NONE)		{
			erreur("Erreur à lecture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle()) ;
			pPat->close() ;

			sErreur = string("Erreur de lecture de la base Patient.db => Patient ") + sNom + string(" ") + sPrenom + string(" introuvable") + string(1, '\n') ;			return false ;		}

		// Vérifier qu'il n'y en a qu'un
		pPat->close();

		sLog = string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n') ;
		return true ;
	}
	//
	// S'il y a des caractères accentués, la recherche peut échouer
	// il faut vérifier que l'enregistrement précédent n'est pas le
	// bon
	//
	pPat->lastError = pPat->suivant(dbiWRITELOCK) ;

	if (pPat->lastError != DBIERR_EOF)
	{
		// On vérifie l'enregistrement en cours
		pPat->lastError = pPat->getRecord() ;

		if (pPat->lastError != DBIERR_NONE)		{
			erreur("Erreur à lecture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle()) ;
			pPat->close() ;
			return false ;
		}

		// Est-ce le bon
		if (goodPat(pPat, sNom, sPrenom))
		{
			// Vérifier qu'il n'y en a qu'un
			pPat->close() ;

			sLog = string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n') ;
      return true ;		}
	}

	pPat->lastError = pPat->precedent(dbiWRITELOCK) ;
	if (pPat->lastError == DBIERR_NONE)	{
		pPat->lastError = pPat->getRecord() ;

		if (pPat->lastError != DBIERR_NONE)		{
			erreur("Erreur à lecture du fichier Patient.db", standardError, pPat->lastError, pMainWin->GetHandle()) ;
      pPat->close() ;
      return false ;
    }

		// Est-ce le bon
		if (goodPat(pPat, sNom, sPrenom))
		{
			// Vérifier qu'il n'y en a qu'un
			pPat->close() ;

			sLog = string("Patient ") + sNom + string(" ") + sPrenom + string(" : Bonne recherche") + string(1, '\n') ;
			return true ;
		}
	}

	pPat->close() ;
	sErreur = string("Patient ") + sNom + string(" ") + sPrenom + string(" : Patient inconnu") + string(1, '\n');#endif	return false ;}

bool
NSNoemie::goodPat(NSPatient* pPat, string sNom, string sPrenom)
{
#ifndef N_TIERS
	if (!pPat)
		return false ;

	string sTestNom     = string(pPat->pDonnees->nom) ;
	string sTestPrenom  = string(pPat->pDonnees->prenom) ;
	strip(sTestNom,     stripRight) ;
	strip(sTestPrenom,  stripRight) ;

	for (size_t i = 0; i < strlen(sTestNom.c_str()); i++)		sTestNom[i] = pseumaj(sTestNom[i]) ;

	for (size_t i = 0; i < strlen(sTestPrenom.c_str()); i++)		sTestPrenom[i] = pseumaj(sTestPrenom[i]) ;

	if (strlen(sTestNom.c_str()) > 25)		sTestNom = string(sTestNom, 0, 25) ;

	if (strlen(sTestPrenom.c_str()) > 15)		sTestPrenom = string(sTestPrenom, 0, 15) ;

	if ((sTestNom == sNom) && (sTestPrenom == sPrenom))
		return true ;
#endif

	return false ;
}

boolNSNoemie::existeActe(string sNumCompt, string sActe, string sQteActe, string sCoefActe)
{
	string sNumFse1610 = sNumCompt + string(FSE_NUMPREST_LEN, ' ') ;

	NSFse1610 Fse1610(pContexte->getSuperviseur()) ;

	strip(sActe,     stripBoth, ' ') ;
	strip(sQteActe,  stripLeft, '0') ;
	strip(sCoefActe, stripLeft, '0') ;

  Fse1610.lastError = Fse1610.open() ;
  if (DBIERR_NONE != Fse1610.lastError)
  {
  	erreur("Erreur à l'ouverture de la base Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

	Fse1610.lastError = Fse1610.chercheClef(&sNumFse1610,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK) ;

	if (DBIERR_BOF == Fse1610.lastError)	// cas fichier vide
	{
		Fse1610.close() ;
    return false ;		// le tableau est vide
  }

  if ((DBIERR_NONE != Fse1610.lastError) && (DBIERR_EOF != Fse1610.lastError))
  {
  	erreur("Erreur à la recherche d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
    Fse1610.close() ;
    return false ;
  }

  double dCoeffBase = StringToDouble(sCoefActe) * 0.01 ;

  bool bTrouve = false;
  while (DBIERR_EOF != Fse1610.lastError)
  {
		Fse1610.lastError = Fse1610.getRecord() ;
    if (DBIERR_NONE != Fse1610.lastError)
    {
    	erreur("Erreur à la lecture d'une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
  	}

    // condition d'arret
    if (Fse1610.getNumCompt() != sNumCompt)
    	break ;

    // Attention au diviseur
    //
    // Le nombre de K contenu dans FSE1610 tient déjà compte du diviseur
    //
    if ((string(Fse1610._Donnees.code) == sActe) &&
            (string(Fse1610._Donnees.quantite) == sQteActe))

    {
    	double dCoeff1610 = StringToDouble(string(Fse1610._Donnees.coeff)) * 0.01 ;

      /*
      double dDivis1610 = StringToDouble(string(pFse1610->pDonnees->divis));

      if (dDivis1610 > 0)
      	dCoeff1610 = dCoeff1610 / dDivis1610 ; */

      if  (dCoeff1610 == dCoeffBase )
      {
      	bTrouve = true ;
        break ;
      }
    }

    // ... on passe au composant suivant
    Fse1610.lastError = Fse1610.suivant(dbiWRITELOCK) ;
    if ((DBIERR_NONE != Fse1610.lastError) && (DBIERR_EOF != Fse1610.lastError))
    {
    	erreur("Erreur d'acces à une fiche Fse1610.", standardError, Fse1610.lastError) ;
      Fse1610.close() ;
      return false ;
    }
  } // fin du while (recherche des composants images)

  // on ferme la base Fse1610
  Fse1610.lastError = Fse1610.close() ;
  if (DBIERR_NONE != Fse1610.lastError)
  {
		erreur("Erreur de fermeture du fichier Fse1610.", standardError, Fse1610.lastError) ;
    return false ;
  }

  return bTrouve ;
}

boolNSNoemie::CreerFact(string sNumCompt, string sOperateur,
                    string sDate_paie, string sOrganisme, string sLibelle,
                    string sMontant, string sUnite, string sMode_paie)
{
try
{
    string	sNumFact =  sNumCompt + string(FACT_NUMERO_LEN, ' ');
    string  sFuturNumero;

    NSFact* pFact = new NSFact(pContexte->getSuperviseur());

    pFact->lastError = pFact->open();
    if (pFact->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base FACT.", standardError, 0) ;
        delete pFact;
        return false;
    }

    pFact->lastError = pFact->chercheClef(&sNumFact,
                                          "",
                                          0,
                                          keySEARCHGEQ,
                                          dbiWRITELOCK);

    sFuturNumero = string(FACT_NUMERO_LEN, ' ');

    if (pFact->lastError == DBIERR_BOF)	// cas fichier vide
        sFuturNumero = string(FACT_NUMERO_LEN, ' ');
    else if ((pFact->lastError != DBIERR_NONE) && (pFact->lastError != DBIERR_EOF))
    {
   	    erreur("Erreur à la recherche d'une fiche fact.", standardError, pFact->lastError) ;
        pFact->close();
        delete pFact;
        return false;
    }
    else
    {
        while (pFact->lastError != DBIERR_EOF)
        {
   	        pFact->lastError = pFact->getRecord();
            if (pFact->lastError != DBIERR_NONE)
            {
      	        erreur("Erreur à la lecture d'une fiche fact.", standardError, pFact->lastError) ;
                pFact->close();
                delete pFact;
                return false;
            }

            // condition d'arret
            if (!(string(pFact->_Donnees.numcompt) == sNumCompt))
                break;
            else
                sFuturNumero = pFact->_Donnees.numero;

            // ... on passe au composant suivant
            pFact->lastError = pFact->suivant(dbiWRITELOCK);
            if ((pFact->lastError != DBIERR_NONE) && (pFact->lastError != DBIERR_EOF))
            {
      	        erreur("Erreur d'acces à une fiche fact.", standardError, pFact->lastError) ;
                pFact->close();
                delete pFact;
                return false;
            }
        } // fin du while (recherche des composants images)
    }

    if (sFuturNumero == string(FACT_NUMERO_LEN, ' '))
        sFuturNumero = string(FACT_NUMERO_LEN, '0');
    else
    {
        char szFuturNb[FACT_NUMERO_LEN + 1];
        int iNbFact = atoi(sFuturNumero.c_str()) + 1;
        sprintf(szFuturNb, "%02d", iNbFact);
        sFuturNumero = string(szFuturNb);
    }

    strcpy(pFact->_Donnees.numcompt,  sNumCompt.c_str());
    strcpy(pFact->_Donnees.numero,    sFuturNumero.c_str());
    strcpy(pFact->_Donnees.operateur, sOperateur.c_str());
    strcpy(pFact->_Donnees.date_paie, sDate_paie.c_str());
    strcpy(pFact->_Donnees.organisme, sOrganisme.c_str());
    strcpy(pFact->_Donnees.libelle,   sLibelle.c_str());
    strcpy(pFact->_Donnees.montant,   sMontant.c_str());
    strcpy(pFact->_Donnees.unite,     sUnite.c_str());
    strcpy(pFact->_Donnees.mode_paie, sMode_paie.c_str());

    pFact->lastError = pFact->appendRecord();
    if (pFact->lastError != DBIERR_NONE)
    {
        erreur("Erreur à l'ajout de la fiche Fact.", standardError, pFact->lastError) ;
        pFact->close();
        delete pFact;
        return false;
    }

    pFact->lastError = pFact->close();
    if (pFact->lastError != DBIERR_NONE)
   	    erreur("Erreur à la fermeture de la base Fact.", standardError, pFact->lastError) ;

    delete pFact;

    return true;
}
catch (...)
{
	erreur("Exception NSNoemie::CreerFact.", standardError, 0) ;
  return false ;
}
}

NSNoemieArray::NSNoemieArray(const NSNoemieArray& rv)
              :NSNoeArray()
{
try
{
	if (false == rv.empty())
		for (NSNoemieArrayConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSEntiteNoemie(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSNoemieArray copy ctor.", standardError, 0) ;
}
}
NSEntiteNoemie::NSEntiteNoemie(string sEntit, string sNiv, string sDonn){
	sEntite = sEntit ;
	sNiveau = sNiv ;
	sDonnee = sDonn ;
}

//---------------------------------------------------------------------------//  Description:	Destructeur
//---------------------------------------------------------------------------
void
NSNoemieArray::vider()
{
	if (empty())
		return ;

	for (NSNoemieArrayIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSNoemieArray::~NSNoemieArray()
{
	vider();
}

//---------------------------------------------------------------------------//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSNoemieArray&
NSNoemieArray::operator=(const NSNoemieArray& aNoemie)
{
try
{
  if (this == &aNoemie)
  	return *this ;

	vider() ;

	if (false == aNoemie.empty())
		for (NSNoemieArrayConstIter i = aNoemie.begin() ; aNoemie.end() != i ; i++)
			push_back(new NSEntiteNoemie(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSNoemieArray (=).", standardError, 0) ;
	return *this;
}
}

