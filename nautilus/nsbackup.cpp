// -----------------------------------------------------------------------------
// nsbackup.cpp
// -----------------------------------------------------------------------------
// $Revision: 1.3 $
// $Author: pameline $
// $Date: 2013/07/21 17:05:13 $
// -----------------------------------------------------------------------------
// FLP - aout 2003
// -----------------------------------------------------------------------------

#include <stdio.h>
#include <fstream.h>
#include <process.h>

#include "nautilus\nsbackup.h"
#include "partage\nsdivfct.h"    // fonction erreur
#include "nsldv\nsldvuti.h"

bool nsbackup(NSSuper *pSuper)
{
  string sFileName = string("backup.dat") ;

  // on ouvre le fichier de configuration
  //
  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
  {
    erreur("impossible d'ouvrir le fichier backup.dat", standardError, 0) ;
    return false ;
  }

  // on lit les paramètres de backup
  //
  string sData = string("") ;

	while (!inFile.eof())
	{
    string line ;
		getline(inFile, line) ;
    sData += line + "\n" ;
	}

  inFile.close() ;

  string      sCmdLine  = string("") ;
  int         iInterval = 0 ;
  bool        bLastSave = true ;
  bool        bInterval = true ;
  NVLdVTemps  tLastSave ;
  NVLdVTemps  tNextSave ;

  size_t i = 0 ;
	while (i < strlen(sData.c_str()))
	{
		string sNomAttrib = string("") ;
		string sValAttrib = string("") ;

		while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t'))
			sNomAttrib += pseumaj(sData[i++]) ;
		while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))
			i++ ;

		while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))
			sValAttrib += sData[i++] ;

		i++ ;

		if 		((sNomAttrib == "CMD_LINE") && (sValAttrib != ""))
      sCmdLine = sValAttrib ;
		else if ((sNomAttrib == "LAST_SAVE") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				bLastSave = false ;
      else
        tLastSave.initFromDateHeure(sValAttrib) ;
		}
		else if ((sNomAttrib == "INTERVAL_DAYS") && (sValAttrib != ""))
		{
			for (size_t j = 0 ; j < strlen(sValAttrib.c_str()) ; j++)
				sValAttrib[j] = pseumaj(sValAttrib[j]) ;

			if ((sValAttrib == string("NEVER")) || (sValAttrib == string("JAMAIS")))
				bInterval = false ;
      else
      {
        tNextSave = tLastSave ;
        iInterval = atoi(sValAttrib.c_str()) ;
        tNextSave.ajouteJours(iInterval) ;
      }
		}
	}

  if (false == bInterval)
    return true ;

  NVLdVTemps  tNow ;
  tNow.takeTime() ;

  if ((false == bLastSave) || (tNextSave < tNow))
  {
    // calcul du nombre de jours depuis la dernière sauvegarde
    // ouverture de la boite de dialogue
    char pcCaption[128] ;
    if (bLastSave)
    {
      unsigned long iDays = tNow.daysBetween(tLastSave) + 1 ;
      sprintf(pcCaption, "La sauvegarde n'a pas été faite depuis %d jours. Voulez-vous la faire maintenant ?", iDays) ;
    }
    else
      sprintf(pcCaption, "La sauvegarde n'a jamais été faite ! Voulez-vous la faire maintenant ?") ;
    int iRetVal = MessageBox(0, pcCaption, "Sauvegarde", MB_YESNO) ;

    if (IDYES == iRetVal)
    {
      // excécution
      int iExecError = WinExec(sCmdLine.c_str(), SW_SHOWNORMAL) ;
      if (iExecError > 31)
      {
        // on doit remplacer la date de la dernière sauvegarde dans le fichier backup.dat
        ofstream  outFile ;
        outFile.open(sFileName.c_str(), ios::out) ;

        if (!outFile)
        {
          string sErrorText = pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileName ;
          erreur(sErrorText.c_str(), standardError, 0) ;
          return true ;
        }

        char    outLine[1024] ;
        sprintf(outLine, "CMD_LINE %s\nLAST_SAVE %s\nINTERVAL_DAYS %d\n", sCmdLine.c_str(), tNow.donneDateHeure().c_str(), iInterval) ;
        outFile.write(outLine, strlen(outLine)) ;
        outFile.close() ;
        return true ;
      }
      else
      {
        switch (iExecError)
        {
          case 0                    : erreur("The system is out of memory or resources.", standardError, 0) ;
                                      break ;
          case ERROR_BAD_FORMAT     : erreur("The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).", standardError, 0) ;
                                      break ;
          case ERROR_FILE_NOT_FOUND : erreur("The specified file was not found.", standardError, 0) ;
                                      break ;
          case ERROR_PATH_NOT_FOUND : erreur("The specified path was not found.", standardError, 0) ;
                                      break ;
          default                   : erreur("erreur inconnue lors de l'exécution de la sauvegarde.", standardError, 0) ;
        }
        return false ;
      }
    }
  }
  return true ;
}


