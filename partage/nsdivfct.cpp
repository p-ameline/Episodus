// -----------------------------------------------------------------------------// nsdivfct.cpp
// -----------------------------------------------------------------------------
// Pot-pourri de fonctions
// -----------------------------------------------------------------------------
// $Revision: 1.32 $
// $Author: pameline $
// $Date: 2017/05/13 08:31:13 $
// -----------------------------------------------------------------------------
// PA  - 30 juin 1988
// PA  - 28 juillet 1994
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant � g�rer et traiter les
// informations de sant� d'une personne.
//
// Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilit� au code source et des droits de copie, de
// modification et de redistribution accord�s par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limit�e. Pour les m�mes raisons, seule une
// responsabilit� restreinte p�se sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les conc�dants successifs.
//
// A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
// associ�s au chargement, � l'utilisation, � la modification et/ou au
// d�veloppement et � la reproduction du logiciel par l'utilisateur �tant donn�
// sa sp�cificit� de logiciel libre, qui peut le rendre complexe � manipuler et
// qui le r�serve donc � des d�veloppeurs et des professionnels avertis
// poss�dant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invit�s � charger et tester l'ad�quation du logiciel � leurs besoins
// dans des conditions permettant d'assurer la s�curit� de leurs syst�mes et ou
// de leurs donn�es et, plus g�n�ralement, � l'utiliser et l'exploiter dans les
// m�mes conditions de s�curit�.
//
// Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accept� les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#include <owl\applicat.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <time.h>
#include <dos.h>
#include <ctype.h>
#include <math.h>
#include <string.stl>
// #include <sysutils.hpp>		// pour FileExists de nom_sans_doublons

#include "partage\nsglobal.h"#include "partage\nsdivfct.h"#include "partage\nsdivfile.h"
static bool bModeFile = false ;
// -----------------------------------------------------------------------------// Function    : DonneNomFichierDOS(const char *ammorce, const char* suffixe)// Arguments   : 	ammorce  : premiers caract�res du pr�fixe du nom DOS
//				  			suffixe  : suffixe du nom de fichier DOS
//				  			chemin   : r�pertoire o� chercher
//				  			nomLibre : nom du fichier trouv� sous forme PPPPPPPP.SSS
// Description : Cherche le premier nom de fichier libre.
// Returns     : RIEN
// -----------------------------------------------------------------------------
/*
unsigned int
DonneNomFichierDOS(const char *ammorce, const char* suffixe, const char *chemin, char** nomLibre)
{
	int 		i, j, tailleAmmorce, calculer ;
	char 		prefixe[9], numero[9], numeroRef[9], *fichRecherche ;
	struct 	find_t stRecherche ;

	// Pr�paration d'une ammorce g�n�rique du type 100001??.RTF
	tailleAmmorce = strlen(ammorce) ;
	if (tailleAmmorce > 7)
  	return 1 ;
	for (i = 0 ; i < tailleAmmorce ; i++)
		prefixe[i] = ammorce[i] ;
	for ( ; i < 8 ; i++)
		prefixe[i] = '?' ;
	prefixe[i] = '\0' ;

	// Pr�paration du num�ro de r�f�rence
	for (i = 0 ; i < 8 - tailleAmmorce ; i++)
		numeroRef[i] = '0' ;
	numeroRef[i-1] = '1' ;
	numeroRef[i] = '\0' ;

	// Pr�paration du nom de fichier complet
	fichRecherche = new char[strlen(chemin) + 14] ;
	strcpy(fichRecherche, chemin) ;
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(fichRecherche, "\\") ;
	strcat(fichRecherche, prefixe) ;
	strcat(fichRecherche, ".") ;
	strcat(fichRecherche, suffixe) ;

	// Lancement de la recherche du premier fichier correspondant FindFirstFile
	i = _dos_findfirst(fichRecherche, _A_NORMAL | _A_RDONLY | _A_ARCH, &stRecherche) ;
	delete fichRecherche ;

	// Si le fichier n'existe pas on renvoie le num�ro de r�f�rence
	if (i != 0)
	{
		*nomLibre = new char[strlen(chemin) + 14] ;
		strcpy(*nomLibre, chemin) ;
		if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
			strcat(*nomLibre, "\\") ;
		strcat(*nomLibre, ammorce) ;
		strcat(*nomLibre, numeroRef) ;
		strcat(*nomLibre, ".") ;
		strcat(*nomLibre, suffixe) ;
		return (0) ;
	}

	// Traitement du fichier trouv� (r�cup�ration de son num�ro)
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.name, numero) ;
	if (i == 1)
  	return (1) ;

	// Boucle tant qu'on ne trouve pas un num�ro diff�rent de la r�f�rence
	while (strcmp(numero, numeroRef) == 0)
	{
		// On incr�mente la r�f�rence
		calculer = 1 ;
		i = strlen(numeroRef) - 1 ;
		while (calculer)
    {
      j = (int) numeroRef[i] ;
      j++ ;
      if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
      {
        numeroRef[i] = (char) j ;
        calculer = 0 ;
      }
      else if ((j > '9') && (j < 'A'))
      {
        numeroRef[i] = 'A' ;
        calculer = 0 ;
      }
      else
      {
        numeroRef[i] = '0' ;
        i-- ;
      }
		}
		// On appelle le fichier suivant et on le traite
		i = _dos_findnext(&stRecherche) ;
		if (i != 0)
			numero[0] = '\0' ;
		else
		{
			i = DonneNumeroFichier(ammorce, suffixe, stRecherche.name, numero) ;
			if (i != 0)
				numero[0] = '\0' ;
		}
	}
	// On renvoie le num�ro de r�f�rence
	*nomLibre = new char[strlen(chemin) + 14] ;
	strcpy(*nomLibre, chemin) ;
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(*nomLibre, "\\") ;
	strcat(*nomLibre, ammorce) ;
	strcat(*nomLibre, numeroRef) ;
	strcat(*nomLibre, ".") ;
	strcat(*nomLibre, suffixe) ;

	return (0) ;
}
*/


// -----------------------------------------------------------------------------
// Function    : DonneNomFichier(const char *ammorce, const char* suffixe)
// Arguments   :	ammorce  : premiers caract�res du pr�fixe du nom DOS
//				  			suffixe  : suffixe du nom de fichier DOS
//				  			chemin   : r�pertoire o� chercher
//				  			nomLibre : nom du fichier trouv� sous forme PPPPPPPP.SSS
// Description : Cherche le premier nom de fichier libre
// Returns     : RIEN
// -----------------------------------------------------------------------------
unsigned int
DonneNomFichier(const char *ammorce, const char* suffixe, const char *chemin, char** nomLibre)
{
	int 						i, j, calculer ;
	char 						prefixe[9], numero[9], numeroRef[9], *fichRecherche ;
	WIN32_FIND_DATA	stRecherche ;
  HANDLE					hFichierTrouve ;

	// Pr�paration d'une ammorce g�n�rique du type 100001??.RTF
	int tailleAmmorce = strlen(ammorce) ;
	if (tailleAmmorce > 7)
  	return 1 ;
	for (i = 0 ; i < tailleAmmorce ; i++)
		prefixe[i] = ammorce[i] ;
	for ( ; i < 8 ; i++)
		prefixe[i] = '?' ;
	prefixe[i] = '\0' ;

	// Pr�paration du num�ro de r�f�rence
	for (i = 0 ; i < 8 - tailleAmmorce ; i++)
		numeroRef[i] = '0' ;
	numeroRef[i-1] = '1' ;
	numeroRef[i] = '\0' ;

	// Pr�paration du nom de fichier complet
	fichRecherche = new char[strlen(chemin) + 14] ;
	strcpy(fichRecherche, chemin) ;
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(fichRecherche, "\\") ;
	strcat(fichRecherche, prefixe) ;
	strcat(fichRecherche, ".") ;
	strcat(fichRecherche, suffixe) ;

	// Lancement de la recherche du premier fichier correspondant FindFirstFile
	hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche) ;
	delete[] fichRecherche ;

	// Si le fichier n'existe pas on renvoie le num�ro de r�f�rence
	if (hFichierTrouve == INVALID_HANDLE_VALUE)
	{
		*nomLibre = new char[strlen(chemin) + 14] ;
		strcpy(*nomLibre, chemin) ;
		if ((chemin[0] != '\0') && (chemin[strlen(chemin) - 1] != '\\'))
			strcat(*nomLibre, "\\") ;
		strcat(*nomLibre, ammorce) ;
		strcat(*nomLibre, numeroRef) ;
		strcat(*nomLibre, ".") ;
		strcat(*nomLibre, suffixe) ;
		return (0) ;
	}

	// Traitement du fichier trouv� (r�cup�ration de son num�ro)
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero) ;
	if (i == 1)
  	return (1) ;

	// Boucle tant qu'on ne trouve pas un num�ro diff�rent de la r�f�rence
	while (strcmp(numero, numeroRef) == 0)
	{
		// On incr�mente la r�f�rence
		calculer = 1 ;
		i = strlen(numeroRef) - 1 ;
		while (calculer)
    {
      j = (int) numeroRef[i] ;
      j++ ;
      if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
      {
        numeroRef[i] = (char) j ;
        calculer = 0 ;
      }
      else if ((j > '9') && (j < 'A'))
      {
        numeroRef[i] = 'A' ;
        calculer = 0 ;
      }
      else
      {
        numeroRef[i] = '0' ;
        i-- ;
      }
		}

		// On appelle le fichier suivant et on le traite
    //
    bool bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;
		if (false == bTrouve)
			numero[0] = '\0' ;
		else
		{
			i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero) ;
			if (i != 0)
				numero[0] = '\0' ;
		}
	}

	// On renvoie le num�ro de r�f�rence
	*nomLibre = new char[strlen(chemin) + 14] ;
	strcpy(*nomLibre, chemin) ;
	if ((chemin[0] != '\0') && (chemin[strlen(chemin) - 1] != '\\'))
		strcat(*nomLibre, "\\") ;
	strcat(*nomLibre, ammorce) ;
	strcat(*nomLibre, numeroRef) ;
	strcat(*nomLibre, ".") ;
	strcat(*nomLibre, suffixe) ;
	return (0) ;
}
/*** Is this element a collective, group, local or in memory object** This works with a patient ID, a tree ID or a node ID*/GRAPHELEMTYPEgetGraphElementTypeFromID(string sElementID){	if (string("") == sElementID)  	return isIncorrectID ;  char cFirstChar = sElementID[0] ;	if ('#' == cFirstChar)  	return isMemoryID ;	if ('-' == cFirstChar)  	return isLocalID ;	if ('~' == cFirstChar)  	return isGroupID ;  return isCollectiveID ;}stringgetPatientIDFromID(string sAnyID){	if (strlen(sAnyID.c_str()) < 7)		return string("") ;	return string(sAnyID, 0, 7) ;}
string
getTreeIDFromID(string sAnyID)
{
	if (strlen(sAnyID.c_str()) < 13)
		return string("") ;	return string(sAnyID, 7, 6) ;
}
stringgetNodeIDFromID(string sAnyID){	if (strlen(sAnyID.c_str()) < 18)		return string("") ;	return string(sAnyID, 13, 5) ;}stringgetRegularPath(string sMonoSeparatorPath, char cPath, char cIntranode){  if (string("") == sMonoSeparatorPath)    return string("") ;  string result = sMonoSeparatorPath ;  for (register unsigned int i = 0; i < result.size(); i++)
  {
    if (cPath == result[i])
    {
      bool change = false ;
      if (result.size() > i + 1)
      {
        if ('$' == result[i+1])
          change = true ;
        else if (result.size() > i + 2)
        {
          std::string temp = result.substr(i+1, 2) ;
          if ((temp == "�N") || (temp == "�D") || (temp == "�T"))
            change = true ;
          else if (result.size() > i + 3)
          {
            std::string temp = result.substr(i+1, 3) ;
            if (temp == "WCE")
              change = true ;
            else if (result.size() > i + 4)
            {
              std::string temp = result.substr(i+1, 4) ;
              if (temp == "WPLU")
                change = true ;
            }
          }
        }
      }
      if (true == change)
        result[i] = cIntranode ;
    }
  }
  return (result) ;}intStringToInt(string sValeur){	return (string("") != sValeur) ? atoi(sValeur.c_str()) : 0 ;}// -----------------------------------------------------------------------------// Function : Transforme un int en string// -----------------------------------------------------------------------------stringIntToString(int iVal){  // The space allocated for string must be large enough to hold the  // returned string, including the terminating null character (\0).  // itoa can return up to 33 bytes.  //	// char temp[33] ;  // itoa(val, temp, 10) ;  // itoa is not ANSI  const int max_size = std::numeric_limits<int>::digits10 + 1 /*sign*/ + 1 /*0-terminator*/ ;  char buffer[max_size] = {0} ;
  sprintf(buffer, "%d", iVal) ;  return string(buffer) ;}
// -----------------------------------------------------------------------------
// Function : Transforme un long en string// -----------------------------------------------------------------------------stringLongToString(long lVal){  const int n = snprintf(NULL, 0, "%lu", lVal) ;  if (n <= 0)
    return string("") ;

  char* buffer = new char(n+1) ;
  int c = snprintf(buffer, n+1, "%lu", lVal) ;
  if (('\0' != buffer[n]) || (c != n))
  {
    delete[] buffer ;
    return string("") ;
  }
  string sResult = string(buffer) ;  delete[] buffer ;  return sResult ;}

//
// To be improved
//
string
getEpisodusNumValueFromRawCapturedNum(string sRawNum, string sLang)
{
	if (string("") == sRawNum)
		return string("") ;

  string sValue = string("") ;

  string sWorkNum = sRawNum ;

	// The only allowed separator is '.'
  // If we find a ',' we replace it by a '.'

  size_t posCr = sWorkNum.find(",") ;
  if (string::npos != posCr)
		sWorkNum[posCr] = '.' ;

	// first, check if the term is a numerical value
	//

  // the characters must match this generic format:
  //
  // [ws] [sn] [ddd] [.] [ddd] [fmt[sn]ddd]
  //
	// where:
	//
	// [ws]	=	optional whitespace
	// [sn]	=	optional sign (+ or -)
	// [ddd]	=	optional digits
	// [fmt]	=	optional e or E
	//
	// [.]	=	optional decimal point
	//
	// strtod also recognizes +INF and -INF for plus and minus infinity,
  // and +NAN and -NAN for Not-a-Number.
	//
	// For example, here are some character strings that strtod can convert to double:
	//  + 1231.1981 e-1
	//  502.85E2
	//  + 2010.952

	char* endptr ;
	char* szTest = new char[strlen(sWorkNum.c_str()) + 1] ;
	strcpy(szTest, sWorkNum.c_str()) ;
	double value = strtod(szTest, &endptr) ;

  // Success
  //
  if ('\0' == *endptr)
  	sValue = sWorkNum ;

  // never do this before checking endptr value, since endptr points on it
  //
	delete[] szTest ;

  return sValue ;
}

string
getEpisodusDateValueFromRawCapturedDate(string sRawNum, string sLang)
{
	return string("") ;
}

// -----------------------------------------------------------------------------
// Function    : DonneNumeroFichier
// Arguments   :	ammorce  			: premiers caract�res du pr�fixe du nom DOS
//				        suffixe  			: suffixe du nom de fichier DOS
//				        nomFichier  	: nom de fichier DOS
//				        numeroFichier : num�ro du fichier
// Description : Extrait le num�ro du nom de fichier
//				        Exemple : ammorce = 100125 suffixe = RTF
//								nomFichier = 10012522.RTF -> numeroFichier = 22
// Returns     : 0 si OK, 1 si le nom n'est pas coh�rent avec ammorce et suffixe
// -----------------------------------------------------------------------------
unsigned intDonneNumeroFichier(const char *ammorce, const char *suffixe, const char *nomFichier, char *numeroFichier){
	size_t i, j ;

	// V�rification de la conformit� par rapport � l'ammorce
	for (i = 0 ; i < strlen(ammorce) ; i++)
		if (nomFichier[i] != ammorce[i])
			return(1) ;

	// Prise du num�ro qui suit l'ammorce
	for (j = 0 ; (nomFichier[i] != '.') && (nomFichier[i] != '\0') ; i++, j++)
		numeroFichier[j] = nomFichier[i] ;
	numeroFichier[j] = '\0' ;

	// V�rification de la conformit� par rapport au suffixe
	if (nomFichier[i] == '\0')
	{
		if (suffixe[0] != '\0')
			return(1) ;
	}
	else
	{
		i++ ;
		if ((suffixe[0] == '\0') && (nomFichier[i] != '\0'))
			return(1) ;
		for (j = 0 ; (j < strlen(suffixe)) && (nomFichier[i] != '\0') ; i++, j++)
			if (nomFichier[i] != suffixe[j])
				return(1) ;
	}
	return(0) ;
}

// -----------------------------------------------------------------------------
// void strip(string& s, int n)// Fonction : enleve les blancs d'une string// Retour   : rien// -----------------------------------------------------------------------------voidstrip(string& s, STRIPTYPE n, char c){  if (string("") == s)    return ;  size_t pos1 = s.find_first_not_of(c) ;  if (string::npos == pos1)  {		s = string("") ;    return ;  }  size_t pos2 ;  switch (n)  {    case stripBoth  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;    	pos2 = s.find_last_not_of(c) ;      if (pos2 != string::npos)      	s = string(s, 0, pos2 + 1) ;      break ;    case stripLeft  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;      break ;    case stripRight :    	pos2 = s.find_last_not_of(c) ;      if (pos2 != string::npos)      	s = string(s, 0, pos2 + 1) ;      break ;    default         :    	erreur("Fonction strip : option erronn�e", standardError, 0) ;  }}void strip(string& s, string sM, STRIPTYPE n){  if ((string("") == s) || (string("") == sM))    return ;  size_t pos1 = s.find_first_not_of(sM) ;  if (string::npos == pos1)  {		s = string("") ;    return ;  }  size_t pos2 ;  switch (n)  {    case stripBoth  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;    	pos2 = s.find_last_not_of(sM) ;      if (string::npos != pos2)      	s = string(s, 0, pos2 + 1) ;      break ;    case stripLeft  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;      break ;    case stripRight :    	pos2 = s.find_last_not_of(sM) ;      if (string::npos != pos2)      	s = string(s, 0, pos2 + 1) ;      break ;    default         :    	erreur("Fonction strip : option erronn�e", standardError, 0) ;  }}void keepOnlyOne(string& s, char c){  if (string("") == s)    return ;  bool bAcceptChar = true ;  string sSearchCopy = s ;  s = string("") ;  for (size_t i = 0 ; i < strlen(sSearchCopy.c_str()) ; i++)  {    if (c == sSearchCopy[i])    {      if (bAcceptChar)      {        s += sSearchCopy[i] ;        bAcceptChar = false ;      }    }    else    {      s += sSearchCopy[i] ;      bAcceptChar = true ;    }  }}
voidincremente_code(string& code){
	bool    tourner  = true ;
	size_t  i        = strlen(code.c_str()) ;

	while ((tourner) && (i > 0))
  {
    i-- ;
    if      (((code[i] >= '0') && (code[i] < '9')) || ((code[i] >= 'A') && (code[i] < 'Z')))
    {
      code[i] = (char)(code[i] + 1) ;
      tourner = false ;
    }
    else if (code[i] == '9')
    {
      code[i] = 'A' ;
      tourner = false ;
    }
    else if (code[i] == 'Z')
      code[i] = '0' ;
	}

  if (tourner)	// dans ce cas le code ne contient que des z�ros
    code = string("1") + code ;
}

string
nom_sans_doublons(string sServer, string sUnit, string sPathRef, string sName, string sExt)
{
	string sCompteur = string("") ;
	string sPrefix   = string("") ;

  // Attention pathRef est ici un chemin Relatif
  // et non un chemin absolu comme dans NSModHtml
  //
  bool bExist = true ;

  while (bExist)
  {
    incremente_code(sCompteur) ;
    sPrefix = sName + sCompteur ;

    string sCompleteFileName ;
    if (string("") != sServer)
      sCompleteFileName = sServer + sPathRef + sPrefix + string(".") + sExt ;
    else
      sCompleteFileName = sUnit + sPathRef + sPrefix + string(".") + sExt ;

    bExist = NsFileExists(sCompleteFileName) ;
  }

  // tant que le compteur ne d�passe pas ZZ on obtiendra un nom dos � 8 + 3
  if (string("") != sPrefix)
    return sPrefix + string(".") + sExt ;

  return string("") ;
}

// -----------------------------------------------------------------------------
// void numacar(mot, nbre, taille)
// Fonction : Transforme un NOMBRE en CARACTERE
// Retour	  : Rien
// Cr�� le 02/08/1989 -- Derni�re mise � jour le 28/07/1994
// -----------------------------------------------------------------------------
void
numacar(char *mot, unsigned int nbre, unsigned int taille)
{
	if (taille > 5)
    taille = 5 ;

	unsigned int multi = 1 ;

	for (unsigned int i = 1 ; i < taille ; i++)
    multi = multi * 10 ;

	for (unsigned int i = taille ; i > 0 ; i--, multi = multi / 10)
	{
		if (nbre < multi)
      mot[taille-i] = '0' ;
		else
		{
			mot[taille-i] = (char)(char(nbre / multi) + '0') ;
			nbre -= multi * (nbre / multi) ;
		}
	}

	mot[taille] = '\0' ;
}

// Convert an unsigned int to a string for a base (up to 62 (10 + 26 + 26))
//
void
numacarbase(string* pResult, unsigned int nbre, unsigned int size, unsigned int base)
{
  if (NULL == pResult)
    return ;

  if ((0 == size) || (0 == base) || (base > 62))
  {
    *pResult = string("") ;
    return ;
  }

  // Initialisation: size * '0'
  //
  *pResult = string(size, '0') ;

  if (0 == nbre)
    return ;

  unsigned int remainingNbre = nbre ;

  for (unsigned int i = 0 ; i < size ; i++)
  {
    unsigned int iLocalDivide = (unsigned int) pow(base, size - i - 1) ;
    unsigned int iLocalValue  = remainingNbre / iLocalDivide ;

    if      (iLocalValue < 10)
      (*pResult)[i] = iLocalValue + '0' ;
    else if (iLocalValue < 36)
      (*pResult)[i] = iLocalValue - 10 + 'A' ;
    else if (iLocalValue < 62)
      (*pResult)[i] = iLocalValue - 36 + 'a' ;

    remainingNbre -= iLocalDivide * iLocalValue ;

    if (0 == remainingNbre)
      return ;
  }
}

// -----------------------------------------------------------------------------
// Suppression de la partie d�cimale, avec arrondi � l'entier le plus proche
// -----------------------------------------------------------------------------
double
arrondiInt(double dNbre)
{
  double chiffre = floor(dNbre) ;
  if (dNbre - chiffre > 0.5)
    chiffre++ ;
  return chiffre ;
}

// -----------------------------------------------------------------------------
// Conversion d'un double en int, avec arrondi � l'entier le plus proche
// -----------------------------------------------------------------------------
int
dtoi(double dNbre)
{
	return int(arrondiInt(dNbre)) ;
}

// -----------------------------------------------------------------------------
// int donneDigit(char nbre)
// Fonction : Transforme un caract�re type '1', '2' en int
// Retour   : -1 si impossible
//					un entier de 0 � 9 sinon
// Cr�� le 02/08/1989 -- Derni�re mise � jour le 28/07/1994
// -----------------------------------------------------------------------------
int
donneDigit(char nbre)
{
	if (false == isdigit(nbre))
		return -1 ;
    
	return int(nbre) - int('0') ;
}

char
donneChar(int digit)
{
  if ((digit >= 0) && (digit < 10))
  	return char(int('0') + digit) ;
  else
  	return '*' ;
}

// -----------------------------------------------------------------------------
// Function    : int ote_blancs(mot)
// Description : Ote les blancs � droite et � gauche de mot[]
// Return      : Rien
// Created on 13/10/1988 - last update on 11/06/1990
// -----------------------------------------------------------------------------
int
ote_blancs(char* mot)
{
	unsigned int i ;

  // on �te les blancs finaux
  for (i = strlen(mot) ; (mot[i-1] == ' ') && (i > 0) ; i--)
    ;

  if (i == 0)  {    mot[0] = '\0' ;    return (0) ;  }  else
  	mot[i] = '\0' ;

  // On �te les blancs initiaux
  while (mot[0] == ' ')
  	for (i = 0 ; mot[i] != '\0' ; i++)
    	mot[i] = mot[i + 1] ;
  return (0) ;
}

// -----------------------------------------------------------------------------
// Function    : int egalite(mot1, mot2)
// Description : Renvoie l'indice de difference
// Return      : Rien
// Created on 13/10/1988 - last update on 28/07/1994
// -----------------------------------------------------------------------------
int
egalite(char *mot1, char *mot2)
{
	int i, taille1, taille2;

  //  On �te les blancs terminaux
  for (taille1 = strlen(mot1) ; mot1[taille1 - 1] == ' ' ; taille1--)
  	;
  for (taille2 = strlen(mot2) ; mot2[taille2 - 1] == ' ' ; taille2--)
  	;

  //  On compare mot1 et mot2
  if (taille1 <= taille2)
  {
    for (i = 0 ; i < taille1 ; i++)
    	if (pseumaj(mot1[i]) != pseumaj(mot2[i]))
      	return (i + 1) ;
    return (taille1 + 1) ;
  }

  for (i = 0 ; i < taille2 ; i++)
    if (pseumaj(mot1[i]) != pseumaj(mot2[i]))
      return (i + 1) ;
  return (taille2 + 1) ;
}

// -----------------------------------------------------------------------------
// Function   : unsigned int pseumaj(lettre)
// Decription : Renvoie la pseudo-majuscule
// Return     : Rien
// Created on 13/10/1988 - last update on 28/07/1994
// -----------------------------------------------------------------------------
char
pseumaj(char lettre)
{
	if ((lettre >= 'a') && (lettre <= 'z'))
  	return char(lettre - 32) ;

	switch (lettre)
	{
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : //226
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' : return('A') ;
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' : return('E') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('I') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('O') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('U') ;
    case (unsigned) '�' :
		case (unsigned) '�' : return('C') ;
	}

	return (lettre) ;
}

// -----------------------------------------------------------------------------
// Function    : void pseumaj(char *chaine)// Description : Convertit la chaine en pseudo-majuscules
// Return	     : Rien
// Created on 6/10/1999
// -----------------------------------------------------------------------------
void
pseumaj(char *chaine)
{
  if ((char*) NULL == chaine)
    return ;

  for (size_t i = 0 ; i < strlen(chaine) ; i++)
    chaine[i] = pseumaj(chaine[i]) ;
}

// -----------------------------------------------------------------------------
// Function    : void pseumaj(string* sChaine)// Description : Convertit la chaine en pseudo-majuscules
// Return	     : Rien
// Created on 6/10/1999
// -----------------------------------------------------------------------------
void
pseumaj(string* psChaine)
{
  if (((string*) NULL == psChaine) || ('\0' == (*psChaine)[0]))
    return ;

  for (size_t i = 0 ; i < strlen(psChaine->c_str()) ; i++)
    (*psChaine)[i] = pseumaj((*psChaine)[i]) ;
}

string
pseumaj(string sChaine)
{
	string sOutputString = sChaine ;
  pseumaj(&sOutputString) ;

  return sOutputString ;
}

// -----------------------------------------------------------------------------
// Function    : char vraiemaj(lettre)
// Description : Renvoie la vraie-majuscule
// Retour	     : Rien
// Created on 27/04/2004 - last update on 27/04/2004
// -----------------------------------------------------------------------------
char
vraiemaj(char lettre)
{
	if ((lettre >= 'a') && (lettre <= 'z'))
  	return char(lettre - 32) ;

	switch (lettre)	{
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
		case (unsigned) '�' : return('�') ;
	}
	return (lettre) ;
}

// -----------------------------------------------------------------------------// Function    : void vraiemaj(char *chaine)// Description : Convertit la chaine en vraies-majuscules
// Return	     : Rien
// Created on 06/10/1999
// -----------------------------------------------------------------------------
void
vraiemaj(char *chaine)
{
  for (size_t i = 0 ; i < strlen(chaine) ; i++)
    chaine[i] = vraiemaj(chaine[i]) ;
}

// -----------------------------------------------------------------------------
// Function    : void vraiemaj(string* sChaine)// Description : Convertit la chaine en vraies-majuscules
// Return	     : Rien
// Created on 06/10/1999
// -----------------------------------------------------------------------------
void
vraiemaj(string* psChaine)
{
  for (size_t i = 0 ; i < strlen(psChaine->c_str()) ; i++)
    (*psChaine)[i] = vraiemaj((*psChaine)[i]) ;
}

// -----------------------------------------------------------------------------
// Function    : void firstmaj(char *chaine)// Description : Convertit la chaine avec les premieres lettres en vraies-majuscules
// Return      : Rien
// Created on 6/10/1999
// -----------------------------------------------------------------------------
void
firstmaj(char *chaine)
{
  if (strlen(chaine) == 0)
    return ;

  strcpy(chaine, strlwr(chaine)) ;
  bool bMaj = true ;
  char lettre ;

  for (size_t i = 0 ; i < strlen(chaine) ; i++)
  {
    lettre = pseumaj(chaine[i]) ;
    if ((lettre >= 'A') && (lettre <= 'Z'))
    {
      if (bMaj)
      {
        chaine[i] = vraiemaj(chaine[i]) ;
        bMaj = false ;
      }
    }
    else
    	bMaj = true ;
  }
}

// -----------------------------------------------------------------------------
// Function    : void firstmaj(string* sChaine)// Description : Convertit la chaine avec les premieres lettres en vraies-majuscules
// Return      : Rien
// Created on 06/10/1999
// -----------------------------------------------------------------------------
void
firstmaj(string* psChaine)
{
  if (((string*) NULL == psChaine) || (string("") == *psChaine))
    return ;

  int len = strlen(psChaine->c_str()) ;
  char* chaine = new char[len + 1] ;
  strcpy(chaine, psChaine->c_str()) ;

  firstmaj(chaine) ;

  *psChaine = string(chaine) ;
  delete[] chaine ;
}
// -----------------------------------------------------------------------------// Decription : Gets pseudo-lower cap
// -----------------------------------------------------------------------------
char
pseuLow(char lettre)
{
	if ((lettre >= 'A') && (lettre <= 'Z'))
  	return char(lettre + 32) ;

	switch (lettre)
	{
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : //226
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' : return('a') ;
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' :
		case (unsigned) '�' : return('e') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('i') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('o') ;
		case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' :
    case (unsigned) '�' :
		case (unsigned) '�' : return('u') ;
    case (unsigned) '�' :
		case (unsigned) '�' : return('c') ;
	}

	return (lettre) ;
}

string
pseuLow(string sChaine)
{
  if (string("") == sChaine)
    return string("") ;

  size_t iLen = strlen(sChaine.c_str()) ;
  for (size_t i = 0 ; i < iLen ; i++)
    sChaine[i] = pseuLow(sChaine[i]) ;

  return sChaine ;
}
// -----------------------------------------------------------------------------// Function    : string strpids(string sChaine)// Description : Convertit la chaine en chaine de recherche
// Return      : la chaine de recherche
// Created on le 15/04/2004
// -----------------------------------------------------------------------------
stringstrpids(string sChaine){  string sSearch = sChaine ;  pseumaj(&sSearch) ;  // on remplace les \' et les tirets par des blancs  size_t pos = sSearch.find("\'") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))    	pos = sSearch.find("\'", pos + 1) ;    else    	break ;  }  pos = sSearch.find("-") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))      pos = sSearch.find("-", pos + 1) ;    else      break ;  }  // on enl�ve les blancs initiaux et terminaux  strip(sSearch) ;  // on enl�ve les double-blancs  bool bAcceptEspace = true ;  string sSearchCopy = sSearch ;  sSearch = "" ;  for (size_t i = 0 ; i < strlen(sSearchCopy.c_str()) ; i++)  {    if (sSearchCopy[i] == ' ')    {      if (bAcceptEspace)      {        sSearch += sSearchCopy[i] ;        bAcceptEspace = false ;      }    }    else    {      sSearch += sSearchCopy[i] ;      bAcceptEspace = true ;    }  }  return sSearch ;}string replaceAll(const string sInside, const string sThis, const string sByThat){  string sResult = sInside ;  size_t iPos = sResult.find(sThis) ;  if (NPOS == iPos)    return sResult ;  size_t iThisLen = strlen(sThis.c_str()) ;  while (NPOS != iPos)  {  	sResult.replace(iPos, iThisLen, sByThat) ;
    iPos = sResult.find(sThis) ;
  }

  return sResult ;
}string getFormatedTime(string sRawDate, string sLang, string sTimeFormat){
	if (strlen(sRawDate.c_str()) < 8)
		return string("") ;

  if (strlen(sRawDate.c_str()) < 14)
    sRawDate += string(14 - strlen(sRawDate.c_str()), '0') ;

	if (string("") == sTimeFormat)
  {
  	if (string("en-US") == sLang)
    	sTimeFormat = string("MM/DD/AAAA hh:mm:ss") ;
    else
    	sTimeFormat = string("DD/MM/AAAA hh:mm:ss") ;
  }

	string sYear  = string(sRawDate, 0, 4) ;
  string sMonth = string(sRawDate, 4, 2) ;
  string sDays  = string(sRawDate, 6, 2) ;

	string sResult = sTimeFormat ;

  size_t iYearPos = sResult.find("AAAA") ;
  if (NPOS != iYearPos)
  	sResult.replace(iYearPos, 4, sYear) ;

	size_t iMonthPos = sResult.find("MM") ;
  if (NPOS != iMonthPos)
  	sResult.replace(iMonthPos, 2, sMonth) ;

	size_t iDayPos = sResult.find("DD") ;
  if (NPOS != iDayPos)
  	sResult.replace(iDayPos, 2, sDays) ;

  string sHour   = string(sRawDate,  8, 2) ;
  string sMinute = string(sRawDate, 10, 2) ;
  string sSecond = string(sRawDate, 12, 2) ;

  size_t iHourPos = sResult.find("hh") ;
  if (NPOS != iHourPos)
  	sResult.replace(iHourPos, 2, sHour) ;

	size_t iMinutePos = sResult.find("mm") ;
  if (NPOS != iMinutePos)
  	sResult.replace(iMinutePos, 2, sMinute) ;

	size_t iSecondPos = sResult.find("ss") ;
  if (NPOS != iSecondPos)
  	sResult.replace(iSecondPos, 2, sSecond) ;

	return sResult ;
}
string getFormatedTime(string sRawTime, string sLang, string sDateFormat, string sHourFormat){	size_t iRawTimeSize = strlen(sRawTime.c_str()) ;	if (iRawTimeSize < 8)		return string("") ;	string sFormatedHour = string("") ;	string sRawDate = string(sRawTime, 0, 8) ;  string sFormatedDate = getFormatedDate(sRawDate, sLang, sDateFormat) ;  if (14 == iRawTimeSize)  {  	string sRawHour = string(sRawTime, 8, 6) ;    if (string("") == sHourFormat)    {    	sFormatedHour = getFormatedHour(sRawHour, sLang, sFormatedDate) ;      if (string("") == sFormatedHour)      	return sFormatedDate ;      else      	return sFormatedHour ;    }    else    {    	sFormatedHour = getFormatedHour(sRawHour, sLang, sHourFormat) ;    	sFormatedDate += string(" ") + sFormatedHour ;    }  }  return sFormatedDate ;}string getFormatedDate(string sRawDate, string sLang, string sDateFormat)
{
	if (strlen(sRawDate.c_str()) != 8)
		return string("") ;

	if (string("") == sDateFormat)
  {
  	if (string("en-US") == sLang)
    	sDateFormat = string("MM/DD/AAAA") ;
    else
    	sDateFormat = string("DD/MM/AAAA") ;
  }

	string sYear  = string(sRawDate, 0, 4) ;
  string sMonth = string(sRawDate, 4, 2) ;
  string sDays  = string(sRawDate, 6, 2) ;

	string sResult = sDateFormat ;

  size_t iYearPos = sResult.find("AAAA") ;
  if (iYearPos != NPOS)
  	sResult.replace(iYearPos, 4, sYear) ;

	size_t iMonthPos = sResult.find("MM") ;
  if (iMonthPos != NPOS)
  	sResult.replace(iMonthPos, 2, sMonth) ;

	size_t iDayPos = sResult.find("DD") ;
  if (iDayPos != NPOS)
  	sResult.replace(iDayPos, 2, sDays) ;

	return sResult ;
}

string getFormatedHour(string sRawHour, string sLang, string sHourFormat)
{
	if (strlen(sRawHour.c_str()) != 6)
		return string("") ;

	if (string("") == sHourFormat)
  	sHourFormat = string("hh:mm:ss") ;

	string sHour   = string(sRawHour, 0, 2) ;
  string sMinute = string(sRawHour, 2, 2) ;
  string sSecond = string(sRawHour, 4, 2) ;

	string sResult = sHourFormat ;

  size_t iHourPos = sResult.find("hh") ;
  if (iHourPos != NPOS)
  	sResult.replace(iHourPos, 2, sHour) ;

	size_t iMinutePos = sResult.find("mm") ;
  if (iMinutePos != NPOS)
  	sResult.replace(iMinutePos, 2, sMinute) ;

	size_t iSecondPos = sResult.find("ss") ;
  if (iSecondPos != NPOS)
  	sResult.replace(iSecondPos, 2, sSecond) ;

	return sResult ;
}

string getRawTime(string sFormatedDate, string sLang, string sTimeFormat)
{
	if ((string("") == sTimeFormat) && (strlen(sFormatedDate.c_str()) == 19))
  {
  	if (string("en-US") == sLang)
    	sTimeFormat = string("MM/DD/AAAA hh:mm:ss") ;
    else
    	sTimeFormat = string("DD/MM/AAAA hh:mm:ss") ;
  }

	if (strlen(sFormatedDate.c_str()) != strlen(sTimeFormat.c_str()))
  	return string("") ;

	string sYear  = string("") ;
  string sMonth = string("") ;
  string sDays  = string("") ;

  size_t iYearPos = sTimeFormat.find("AAAA") ;
  if (iYearPos != NPOS)
  	sYear = string(sFormatedDate, iYearPos, 4) ;

	size_t iMonthPos = sTimeFormat.find("MM") ;
  if (iMonthPos != NPOS)
  	sMonth = string(sFormatedDate, iMonthPos, 2) ;

	size_t iDayPos = sTimeFormat.find("DD") ;
  if (iDayPos != NPOS)
  	sDays = string(sFormatedDate, iDayPos, 2) ;

  string sHour   = string("") ;
  string sMinute = string("") ;
  string sSecond = string("") ;

  size_t iHourPos = sTimeFormat.find("hh") ;
  if (iHourPos != NPOS)
  	sHour = string(sFormatedDate, iHourPos, 2) ;

	size_t iMinutePos = sTimeFormat.find("mm") ;
  if (iMinutePos != NPOS)
  	sMinute = string(sFormatedDate, iMinutePos, 2) ;

	size_t iSecondPos = sTimeFormat.find("ss") ;
  if (iSecondPos != NPOS)
  	sSecond = string(sFormatedDate, iSecondPos, 2) ;

	if ((string("") != sYear) && (string("") != sMonth) && (string("") != sDays) &&
      (string("") != sHour) && (string("") != sMinute) && (string("") != sSecond))
  	return sYear + sMonth + sDays + sHour + sMinute + sSecond ;

	return string("") ;
}

string getRawTime(string sFormatedTime, string sLang, string sDateFormat, string sHourFormat)
{
	string sDate = getRawDate(sFormatedTime, sLang, sDateFormat) ;
  if (string("") == sDate)
		return string("") ;

	string sHour = getRawHour(sFormatedTime, sLang, sDateFormat) ;
  if (string("") == sHour)
		return string("") ;

	return sDate + sHour ;
}

string getRawDate(string sFormatedDate, string sLang, string sDateFormat)
{
	if ((string("") == sDateFormat) && (strlen(sFormatedDate.c_str()) == 10))
  {
  	if (string("en-US") == sLang)
    	sDateFormat = string("MM/DD/AAAA") ;
    else
    	sDateFormat = string("DD/MM/AAAA") ;
  }

	if (strlen(sFormatedDate.c_str()) != strlen(sDateFormat.c_str()))
  	return string("") ;

	string sYear  = string("") ;
  string sMonth = string("") ;
  string sDays  = string("") ;

  size_t iYearPos = sDateFormat.find("AAAA") ;
  if (iYearPos != NPOS)
  	sYear = string(sFormatedDate, iYearPos, 4) ;

	size_t iMonthPos = sDateFormat.find("MM") ;
  if (iMonthPos != NPOS)
  	sMonth = string(sFormatedDate, iMonthPos, 2) ;

	size_t iDayPos = sDateFormat.find("DD") ;
  if (iDayPos != NPOS)
  	sDays = string(sFormatedDate, iDayPos, 2) ;

	if ((string("") != sYear) && (string("") != sMonth) && (string("") != sDays))
  	return sYear + sMonth + sDays ;

	return string("") ;
}

string getRawHour(string sFormatedHour, string sLang, string sHourFormat)
{
	if ((string("") == sHourFormat) && (strlen(sFormatedHour.c_str()) == 8))
  	sHourFormat = string("hh:mm:ss") ;

	if (strlen(sFormatedHour.c_str()) != strlen(sHourFormat.c_str()))
  	return string("") ;

	string sHour   = string("") ;
  string sMinute = string("") ;
  string sSecond = string("") ;

  size_t iHourPos = sHourFormat.find("hh") ;
  if (iHourPos != NPOS)
  	sHour = string(sFormatedHour, iHourPos, 2) ;

	size_t iMinutePos = sHourFormat.find("mm") ;
  if (iMinutePos != NPOS)
  	sMinute = string(sFormatedHour, iMinutePos, 2) ;

	size_t iSecondPos = sHourFormat.find("ss") ;
  if (iSecondPos != NPOS)
  	sSecond = string(sFormatedHour, iSecondPos, 2) ;

	if ((string("") != sHour) && (string("") != sMinute) && (string("") != sSecond))
  	return sHour + sMinute + sSecond ;

	return string("") ;
}
// -----------------------------------------------------------------------------// Function    : void donne_heure(char* pHeure)// Description : Donne l'heure au format hhmmss
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 26/05/1998
// last update on 26/05/1998
// -----------------------------------------------------------------------------
void
donne_heure(char* heure)
{
  if ((char*) NULL == heure)
    return ;

  strcpy(heure, "hhmmss") ;

  // Prise de temp
  struct tm *tp ;
  long	 t ;
  char 	 temp[26] ;
  time(&t) ;
  tp = localtime(&t) ;
  strcpy(temp, asctime(tp)) ;

  // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
  int i, j;
  for (i = 0, j = 0 ; i < 8 ; i++, j++)
  {
    heure[j] = temp[i + 11] ;
    if ((i == 1) || (i == 4))
	    i++ ;
  }
}

string
donne_heure()
{
  string sHeure = string("hhmmss") ;

  // Prise de temp
  long t ;
  time(&t) ;
  struct tm *tp = localtime(&t) ;

  string sAscTime = string(asctime(tp)) ;

  // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
  int i, j ;
  for (i = 0, j = 0 ; i < 8 ; i++, j++)
  {
    sHeure[j] = sAscTime[i + 11] ;
    if ((i == 1) || (i == 4))
	    i++ ;
  }

  return sHeure ;
}

// -----------------------------------------------------------------------------
// Function    : void donne_date_duJour(date)
// Description : Donne la date du jour au format AAAAMMJJ
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 08/12/1994
// last update on 08/12/1994
// -----------------------------------------------------------------------------
void
donne_date_duJour(char* date)
{
  if ((char*) NULL == date)
    return ;

	int				i ;
	struct tm *tp ;
	long			t ;
	char			mois[12][4] ; //= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" } ;

	strcpy(mois[0], "Jan") ;
	strcpy(mois[1], "Feb") ;
	strcpy(mois[2], "Mar") ;
	strcpy(mois[3], "Apr") ;
	strcpy(mois[4], "May") ;
	strcpy(mois[5], "Jun") ;
	strcpy(mois[6], "Jul") ;
	strcpy(mois[7], "Aug") ;
	strcpy(mois[8], "Sep") ;
	strcpy(mois[9], "Oct") ;
	strcpy(mois[10],"Nov") ;
	strcpy(mois[11],"Dec") ;

	char temp[26], month[4], numero[3] = "  " ;

	// Initialisation de date
	strcpy(date, "") ;

	// Prise de temp
	time(&t) ;
	tp = localtime(&t) ;
	strcpy(temp, asctime(tp)) ;

 	// asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
	// R�cup�ration de l'ann�e
	for(i = 20 ; i < 24 ; i++)
		date[i-20] = temp[i] ;
	date[4] ='\0' ;

	// R�cup�ration du mois
	for (i = 4 ; i < 7 ; i++)
  	month[i-4] = temp[i] ;
	month[3] = '\0' ;
	for (i = 0 ; (i < 12) && (strcmp(month, mois[i]) != 0) ; i++)
  	;
	i++ ;
	if (i < 10)
	{
		numero[0] = '0' ;
		numero[1] = char (i + (int) '0') ;
	}
	else if (i < 13)
	{
		numero[0] = '1' ;
		numero[1] = char ((i - 10) + (int) '0') ;
	}
	strcat(date, numero) ;

	// R�cup�ration du jour
	for (i = 8 ; i < 10 ; i++)
		numero[i - 8] = temp[i] ;
	numero[2] = '\0' ;
	strcat(date, numero) ;
}

string
donne_date_duJour()
{
	string		sMois[12] ;

	sMois[0]  = string("Jan") ;
	sMois[1]  = string("Feb") ;
	sMois[2]  = string("Mar") ;
	sMois[3]  = string("Apr") ;
	sMois[4]  = string("May") ;
	sMois[5]  = string("Jun") ;
	sMois[6]  = string("Jul") ;
	sMois[7]  = string("Aug") ;
	sMois[8]  = string("Sep") ;
	sMois[9]  = string("Oct") ;
	sMois[10] = string("Nov") ;
	sMois[11] = string("Dec") ;

	// Getting local time
  //
	long t ;
	time(&t) ;
	struct tm *tp = localtime(&t) ;

  // asctime renvoit la date au format : Sun Sep 16 01:03:52 1973\n\0
  //                                     012345678901234567890123
  //                                               1         2
  string sAscTime = string(asctime(tp)) ;

	// Getting month
  //
  string sMonth = string(sAscTime, 4, 3) ;

  int i = 0 ;
	for ( ; (i < 12) && (sMonth != sMois[i]) ; i++)
  	;
	i++ ;

  string sNumMonth = string("00") ;
	if (i < 10)
		sNumMonth[1] = char (i + (int) '0') ;
	else if (i < 13)
	{
		sNumMonth[0] = '1' ;
		sNumMonth[1] = char ((i - 10) + (int) '0') ;
	}

  return string(sAscTime, 20, 4) + sNumMonth + string(sAscTime, 8, 2) ;
}

void
get_current_date(string* sDate)
{
  if (NULL == sDate)
    return ;

  *sDate = donne_date_duJour() ;
}

// -----------------------------------------------------------------------------
// Function    : donne_heure(char* pHeure)
// Description : Transforme une heure HHMMSS ou HHMM en string HH:MM:SS ou HH:MM
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 01/03/1999
// -----------------------------------------------------------------------------
void
donne_heure(char* heure, string& message)
{
  message = string("") ;

  if (((char*) NULL == heure) || (strlen(heure) < 4))
    return ;

  message += heure[0] ;
  message += heure[1] ;
  message += ":" ;
  message += heure[2] ;
  message += heure[3] ;

  if (strlen(heure) > 4)
  {
    message += ":" ;
    message += heure[4] ;
    message += heure[5] ;
  }
}

string
donne_heure(const string sHour)
{
  if (strlen(sHour.c_str()) < 4)
    return string("") ;

  string sMessage = string(sHour, 0, 2) + string(":") + string(sHour, 2, 2) ;

  if (strlen(sHour.c_str()) >= 6)
    sMessage += string(":") + string(sHour, 4, 2) ;

  return sMessage ;
}

// Description : Transforme une date AAAAMMJJ en un message JJ/MM/AAAA// -----------------------------------------------------------------------------
// Created on 17/07/2013
// Last update on 17/07/2013
// -----------------------------------------------------------------------------
stringdonne_date(string sDate, string sLang)
{
  if (strlen(sDate.c_str()) < 8)
    return string("") ;

  string sMessage = string("  /  /    ") ;

	// American english notation : MM/JJ/AAAA
  //
	if (sLang == "en-US")
	{
    sMessage[0] = sMessage[4] ; sMessage[1] = sDate[5] ;
  	sMessage[3] = sMessage[6] ; sMessage[4] = sDate[7] ;
		sMessage[6] = sMessage[0] ; sMessage[7] = sDate[1] ;
		sMessage[8] = sMessage[2] ; sMessage[9] = sDate[3] ;
    return sMessage ;
	}

  // Standard notation : JJ/MM/AAAA
  //
  sMessage[0] = sDate[6] ; sMessage[1] = sDate[7] ;
  sMessage[3] = sDate[4] ; sMessage[4] = sDate[5] ;
  sMessage[6] = sDate[0] ; sMessage[7] = sDate[1] ;
  sMessage[8] = sDate[2] ; sMessage[9] = sDate[3] ;

  return sMessage ;
}
// -----------------------------------------------------------------------------
// Function    : void donne_date(date, message, pays)// Description : Transforme une date AAAAMMJJ en un message JJ/MM/AAAA
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last update on 12/04/1991
// -----------------------------------------------------------------------------
void
donne_date(const char *date, char *message, string sLang)
{
  if ((char*) NULL == message)
    return ;

  if (((char*) NULL == date) || (strlen(date) < 8))
  {
    strcpy(message, "") ;
    return ;
  }

  strcpy(message, "  /  /    ") ;

	// American english notation : MM/JJ/AAAA
  //
	if (sLang == "en-US")
	{
    message[0] = date[4] ; message[1] = date[5] ;
  	message[3] = date[6] ; message[4] = date[7] ;
		message[6] = date[0] ; message[7] = date[1] ;
		message[8] = date[2] ; message[9] = date[3] ;
    return ;
	}

  // Standard notation : JJ/MM/AAAA
  //
  message[0] = date[6] ; message[1] = date[7] ;
  message[3] = date[4] ; message[4] = date[5] ;
  message[6] = date[0] ; message[7] = date[1] ;
  message[8] = date[2] ; message[9] = date[3] ;
}

// -----------------------------------------------------------------------------// Function    : donne_date_inverse(date, message, pays)// Description : Transforme une date JJ/MM/AAAA en un message AAAAMMJJ
// Return      : Rien                0123456789               01234567
// -----------------------------------------------------------------------------
// Created on 12/01/1999
// -----------------------------------------------------------------------------
void
donne_date_inverse(string date, string& sMessage, string sLang)
{
  sMessage = string("") ;

  if (strlen(date.c_str()) < 10)
    return ;

  // Etape 1 : on passe au format standardis� AAAAMMJJ
  string sTempo = string(8, ' ') ;

	// From american English notation : MM/JJ/AAAA
	if (sLang == "en-US")
	{
    sTempo[0] = date[6] ; sTempo[1] = date[7] ;
		sTempo[2] = date[8] ; sTempo[3] = date[9] ;
		sTempo[4] = date[0] ; sTempo[5] = date[1] ;
		sTempo[6] = date[3] ; sTempo[7] = date[4] ;
	}
  // From Standard notation : JJ/MM/AAAA
	else
  {
		sTempo[0] = date[6] ; sTempo[1] = date[7] ;
		sTempo[2] = date[8] ; sTempo[3] = date[9] ;
    sTempo[4] = date[3] ; sTempo[5] = date[4] ;
		sTempo[6] = date[0] ; sTempo[7] = date[1] ;
	}

  if (sTempo == string(8, '0'))
  {
    sMessage = sTempo ;
    return ;
  }

  // Etape 2 : v�rifier que la date est valide

  // Digits only - Que des chiffres
  int i = 0 ;
  for ( ; (i < 8) && (isdigit(sTempo[i])) ; i++)
  	;
  if (i < 8)
    return ;

  int iCentury = donneDigit(sTempo[0]) * 10 + donneDigit(sTempo[1]) ;
  if (iCentury < 18)
    return ;

  int iMonth  = donneDigit(sTempo[4]) * 10 + donneDigit(sTempo[5]) ;
  if (iMonth > 12)
    return ;

  int iDay    = donneDigit(sTempo[6]) * 10 + donneDigit(sTempo[7]);
  if (iDay > 31)
    return ;

  // Mois non pr�cis� : le seul contr�le, c'est que le jour ne le soit pas non plus
  // Month not set : we can only check if the day is not set either
  if (iMonth == 0)
  {
    if (iDay > 0)
      return ;
    sMessage = sTempo ;
    return ;
  }

  // Jour non pr�cis� : pas de contr�le possible
  // Day not set : no possible check
  if (iMonth == 0)
  {
    if (iDay > 0)
    	return ;
    sMessage = sTempo ;
    return ;
  }

  // Date compl�te : on peut v�rifier si elle existe
  int iYear = (iCentury * 100) + donneDigit(sTempo[2]) * 10 + donneDigit(sTempo[3]) ;

  // Ne pas utiliser mktime, car cette fonction �choue sur la plupart des
  // syst�mes lorsque la date est inf�rieure au 1/1/1970
  //
  // tm time_check = {0} ;
  // time_check.tm_year  = iYear - 1900 ;
  // time_check.tm_mon   = iMonth - 1 ;
  // time_check.tm_mday  = iDay ;
  // time_t theTime = mktime(&time_check) ;
  // if (theTime == (time_t)-1)
  //     return ;

  if ((iDay > 30) && ((iMonth == 4) || (iMonth == 6) || (iMonth == 9) || (iMonth == 11)))
  	return ;
  if (iMonth == 2)
  {
    if (iDay > 29)
      return ;
    if ((!bissextile(iYear)) && (iDay > 28))
      return ;
  }

  sMessage = sTempo ;
  return ;
}


// -----------------------------------------------------------------------------
// Function    : donne_date_claire(date, message, pays)
// Description : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 30/08/1994
// Last update on 30/08/1994
// -----------------------------------------------------------------------------
//
//  See http://en.wikipedia.org/wiki/Date_and_time_notation_by_country
//
void
donne_date_claire(const string sDate, string *message, string sLang)
{
  if (NULL == message)
    return ;

	unsigned int i ;
	*message = "" ;
	if (strlen(sDate.c_str()) < 8)
		return ;

  // Day / Jour
  //
	string sDayLib = string("??") ;
	if (isdigit(sDate[6]) && isdigit(sDate[7]))
	{
  	if ('0' != sDate[6])
    	sDayLib = string(1, sDate[6]) ;
    else
  		sDayLib = string("") ;

    sDayLib += string(1, sDate[7]) ;
	}

	// Month / Mois
  //
	string sMonthLib = string("??") ;
	if (isdigit(sDate[4]) && isdigit(sDate[5]))
  {
		i = 10 * ((int) sDate[4] - (int) '0') + (int) sDate[5] - (int) '0' ;
		sMonthLib = donne_mois(i, sLang) ;
	}

  // Year / Ann�e
  //
  string sYearLib = string("") ;
	for (i = 0 ; i < 4 ; i++)
	{
		if (isdigit(sDate[i]))
			sYearLib += string(1, sDate[i]) ;
		else
			sYearLib += '?' ;
	}

  //
  // Make the "nice modifications"
  //
  if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
  {
  	if (sDayLib == string("1"))
    	sDayLib = string("1er") ;
  }

  //
  // Put all this together
  //
  if (sLang == string("en-US"))
  	*message = sMonthLib + string(" ") + sDayLib + string(", ") + sYearLib ;
  else
  	*message = sDayLib + string(" ") + sMonthLib + string(" ") + sYearLib ;

	return ;
}

// -----------------------------------------------------------------------------
// Function    : void donne_date_claire(date, message, pays)
// Description : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 30/08/1994
// Last update on 30/08/1994
// -----------------------------------------------------------------------------
void
donne_date_claire(char* date, char* message, string sLang)
{
  if (NULL == message)
    return ;

  if (NULL == date)
  {
    strcpy(message, "") ;
    return ;
  }

	string sMessage = string("") ;

  donne_date_claire(date, &sMessage, sLang) ;

  strcpy(message, sMessage.c_str()) ;

	return ;
}

// -----------------------------------------------------------------------------
// Description : Donne la date au format JJ/MM/AAAA                �
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void
donne_date_breve(string sDate, string* pMessage, string* pIntro, string sLang)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
  	return ;

  // Date vide
  if (string("00000000") == sDate)
  	return ;

  // Juste AAAA
  if (string(sDate, 4, 4) == "0000")
  {
    *pMessage = string(sDate,0, 4) ;

    if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
  		*pIntro = "en " ;
    if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
  		*pIntro = "in " ;

    return ;
  }

  // MM/AAAA
  if (string(sDate, 6, 2) == "00")
  {
    *pMessage = donne_mois(string(sDate, 4, 2), sLang) + string(" ") + string(sDate, 0, 4) ;

    if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
  		*pIntro = string("en ") ;
    if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
  		*pIntro = string("in ") ;
  }

  // JJ/MM/AAAA
  else
  {
  	char szMessage[11] ;
    char szDate[9] ;
    strcpy(szDate, sDate.c_str()) ;
  	donne_date(szDate, szMessage, sLang) ;
    *pMessage = string(szMessage) ;

    if      ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
  		*pIntro = string("le ") ;
    else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
  		*pIntro = string("on the ") ;
  }
}


// -----------------------------------------------------------------------------
// Description : Donne la date au format "1er janvier 1994"            �
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void
donne_date_claire(string sDate, string* pMessage, string* pIntro, string sLang)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage	= string("") ;
  *pIntro		= string("") ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
  	return ;

	// Date vide
  if (string("00000000") == sDate)
  	return ;

	if ((string(sDate, 4, 4) == "0000") || (string(sDate, 4, 4) == "0000"))
  {
  	donne_date_breve(sDate, pMessage, pIntro, sLang) ;
    return ;
	}

  if      ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
		*pIntro = string("le ") ;
  else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
		*pIntro = string("on the ") ;

  char szDate[9] ;
	strcpy(szDate, sDate.c_str()) ;
	donne_date_claire(szDate, pMessage, sLang) ;
}

bool
IsValidDatePattern(const string &sName, const string sLocalDatePattern)
{
  if (string("") == sName)
    return false ;

  string sFormat = sLocalDatePattern ;
  if (string("") == sLocalDatePattern)
    sFormat = string("JJ/MM/AAAA") ;

  if (strlen(sName.c_str()) != strlen(sFormat.c_str()))
    return false ;

  // Digits only - Que des chiffres
  for (size_t i = 0 ; i < strlen(sName.c_str()) ; i++)
    if ((false == isdigit(sName[i])) &&
           (('J' == sFormat[i]) ||
            ('M' == sFormat[i]) ||
            ('A' == sFormat[i])))
      return false ;

  return true ;
}

/**
 * Check if this code looks like LCCLLCC or a valid subpart
 */
bool
IsValidAtcCode(const string sCode)
{
  if (string("") == sCode)
    return false ;

  size_t iCodeLen = strlen(sCode.c_str()) ;
  if ((1 != iCodeLen) && (3 != iCodeLen) && (5 != iCodeLen) && (7 != iCodeLen))
    return false ;

  string sMajCode = pseumaj(sCode) ;

  string sValidFirstCodes = string("ABCDGHJLMNPRSV") ;
  size_t iPosFirst = sValidFirstCodes.find(sMajCode[0]) ;
  if (NPOS == iPosFirst)
    return false ;

  if (1 == iCodeLen)
    return true ;

  if ((false == isdigit(sMajCode[1])) || (false == isdigit(sMajCode[2])))
    return false ;

  if (3 == iCodeLen)
    return true ;

  if ((sMajCode[3] < 'A') || (sMajCode[3] > 'Z') ||
      (sMajCode[4] < 'A') || (sMajCode[4] > 'Z'))
    return false ;

  if (5 == iCodeLen)
    return true ;

  if ((false == isdigit(sMajCode[5])) || (false == isdigit(sMajCode[6])))
    return false ;

  return true ;
}

// -----------------------------------------------------------------------------
// Description : Indique si l'ann�e donn�e est bissextile
// -----------------------------------------------------------------------------
// Created on 28/06/99
// -----------------------------------------------------------------------------
bool
bissextile(int year)
{
  // R�gle : Si l'ann�e est divisible par 4 ET non divisible par 100
  //				OU divisible par 400 ==> elle est bissextile.
  if (((year % 4 == 0) && (!(year % 100 == 0))) || (year % 400 == 0))
  	return true ;
  return false ;
}

// -----------------------------------------------------------------------------
// Description : Incr�mente une date donn�e de un jour
// -----------------------------------------------------------------------------
// Created on 28/06/99
// -----------------------------------------------------------------------------
void
incremente_date(string& sDate)
{
  string sErrMsg = string("Date invalide ") + sDate ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
	  return ;

  string sAnnee = string(sDate, 0, 4) ;
  string sMois  = string(sDate, 4, 2) ;
  string sJour  = string(sDate, 6, 2) ;

  int iAnnee = atoi(sAnnee.c_str()) ;
  int iMois  = atoi(sMois.c_str()) ;
  int iJour  = atoi(sJour.c_str()) ;

  int iJourFin = 31 ;

  switch (iMois)
  {
    case 1	:
    case 3	:
    case 5	:
    case 7	:
    case 8	:
    case 10	:    case 12	:

      break ;

    case 2	:

      if (bissextile(iAnnee))
        iJourFin = 29 ;
      else
        iJourFin = 28 ;
      break ;

    case 4	:
    case 6	:
    case 9	:
    case 11	:

      iJourFin = 30 ;
      break ;

    default	:
    
      erreur(sErrMsg.c_str(), standardError, 0) ;
      return ;
  }

  if (iJour < iJourFin)
    iJour++ ;
  else if (iJour == iJourFin)
  {
    iJour = 1 ;
    if (12 == iMois)
    {
      iMois = 1 ;
      iAnnee++ ;
    }
    else
      iMois++ ;
  }
  else
  {
    erreur(sErrMsg.c_str(), standardError, 0) ;
    return ;
  }

  // on recompose la nouvelle date
  //
  char cDate[9] ;
  sprintf(cDate, "%4d%02d%02d", iAnnee, iMois, iJour) ;
  sDate = string(cDate) ;
}

// -----------------------------------------------------------------------------
// Description : Donne le libell� du mois � partir de "MM"
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
string
donne_mois(string mois, string sLang)
{
  if (strlen(mois.c_str()) < 2)
  	return string("") ;

  if ((false == isdigit(mois[0])) || (false == isdigit(mois[1])))
  	return string("") ;

  int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]) ;

  return donne_mois(i, sLang) ;
}

// -----------------------------------------------------------------------------
// Description : Donne un �cart de temps de type "AAMMJJ"
// -----------------------------------------------------------------------------
// Created on 29/01/2008
// Last update on 29/01/2008
// -----------------------------------------------------------------------------
string
donne_deltaTemps(int iYears, int iMonths, int iDays)
{
	string sDeltaTemps = string("") ;

	char szYearsValue[3] ;
  if (iYears > 0)
		numacar(szYearsValue, iYears, 2) ;
  else
  	strcpy(szYearsValue, "00") ;

  char szMonthsValue[3] ;
  if (iMonths > 0)
		numacar(szMonthsValue, iMonths, 2) ;
  else
  	strcpy(szMonthsValue, "00") ;

  char szDaysValue[3] ;
  if (iDays > 0)
		numacar(szDaysValue, iDays, 2) ;
  else
  	strcpy(szDaysValue, "00") ;

	sDeltaTemps = string(szYearsValue) + string(szMonthsValue) + string(szDaysValue) ;

	return sDeltaTemps ;
}

// -----------------------------------------------------------------------------// Description : Donne le libell� du mois � partir d'un int             �// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last modified on 14/11/1997
// -----------------------------------------------------------------------------
string
donne_mois(int iMois, string sLang)
{
  if ((iMois < 1) || (iMois > 12))
    return string("") ;

	if (sLang == string("fr"))
	{
  	switch (iMois)
  	{
    	case  1 : return string("janvier") ;
			case  2 : return string("f�vrier") ;
			case  3 : return string("mars") ;
			case  4 : return string("avril") ;
			case  5 : return string("mai") ;
			case  6 : return string("juin") ;
			case  7 : return string("juillet") ;
			case  8 : return string("ao�t") ;
			case  9 : return string("septembre") ;
			case 10 : return string("octobre") ;
			case 11 : return string("novembre") ;
			case 12 : return string("d�cembre") ;
   	}
	}
  else
	{
  	switch (iMois)
  	{
    	case  1 : return string("january") ;
			case  2 : return string("february") ;
			case  3 : return string("march") ;
			case  4 : return string("april") ;
			case  5 : return string("may") ;
			case  6 : return string("june") ;
			case  7 : return string("july") ;
			case  8 : return string("august") ;
			case  9 : return string("september") ;
			case 10 : return string("october") ;
			case 11 : return string("november") ;
			case 12 : return string("december") ;
   	}
	}
  return string("") ;
}

// -----------------------------------------------------------------------------
// Function    : void donne_somme_courte(somme, message, pays)
// Description : Transforme une somme XXXXXXXX en un message XXXXXX,XX
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last update on 12/04/1991
// -----------------------------------------------------------------------------
void
donne_somme_courte(char *somme, char *message, string sLang)
{
  if ((char*) NULL == message)
    return ;

  if (((char*) NULL == somme) || (strlen(somme) < 8))
  {
    strcpy(message, "") ;
    return ;
  }

  strcpy(message, "000000,00") ;

  unsigned int i ;
  for (i = 0 ; i < 6 ; i++)
  	message[i] = somme[i] ;
  for (i = 6 ; i < 8 ; i++)
  	message[i + 1] = somme[i] ;
}

// -----------------------------------------------------------------------------
// Function    : void long_to_hex(unsigned long nb, char nb_hex[])
// Description : Convertit un entier long en hexa
// Parameters  : 	nb   	 -> l'entier � convertir
//            		nb_hex -> l'adresse de la cha�ne
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last updated on 12/04/1991
// -----------------------------------------------------------------------------
void
long_to_hex(unsigned long nb, char* nb_hex)
{
  if ((char*) NULL == nb_hex)
    return ;

  unsigned long result, mask = 0xF ;
  unsigned short i ;
  char hex_car[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
  nb_hex[8] = '\0' ;
  for (i = 0 ; i < 8 ; i++)
  {
    result = nb & mask ;
    mask   = mask << 4 ;
    result = result >> (i << 2) ;
    nb_hex[7-i] = hex_car[result] ;
  }
}

// -----------------------------------------------------------------------------
// Function    : void hex_to_long(char nb_hex[], unsigned long *nb)
// Description : Transforme un HHHHHHHH en unsigned long
// Parameters  : 	nb_hex	-> la chaine contenant l'hexa
//            		nb 	 		-> pointeur sur le r�sultat
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 17/09/1992
// Last update on 07/10/1992
// -----------------------------------------------------------------------------
void
hex_to_long(char nb_hex[], unsigned long *nb)
{
  if ((unsigned long*) NULL == nb)
    return ;

  signed   int  i ;
//  unsigned int  chiffre ;
//  unsigned long multi ;
  *nb = 0;
  if (strlen(nb_hex) != 8)
  	return ;
  for (i = 0; i < 8; i++)
  {
    *nb = *nb * 16 ;
    if ((nb_hex[i] >= '0') && (nb_hex[i] <= '9'))
    	*nb += (unsigned long) nb_hex[i] - 48 ;
    if ((nb_hex[i] >= 'A') && (nb_hex[i] <= 'F'))
    	*nb += (unsigned long) nb_hex[i] - 55 ;
  }
}

void
char_to_hex(unsigned char car, char* car_hex)
{
  if ((char*) NULL == car_hex)
    return ;

  unsigned char result, mask = 0xF ;
  unsigned short i ;
  char hex_car[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'} ;
  car_hex[2] = '\0' ;
  for (i = 0 ; i < 2 ; i++)
  {
    result = car & mask ;
    mask   = mask << 4 ;
    result = result >> (i << 2) ;
    car_hex[1-i] = hex_car[result] ;
  }
}

void
hex_to_char(char car_hex[], unsigned char *car)
{
  if ((unsigned char*) NULL == car)
    return ;

  signed   int  i ;
//  unsigned int  chiffre ;
//  unsigned long multi ;
  *car = 0 ;
  if (strlen(car_hex) != 2)
  	return ;
  for (i = 0; i < 2; i++)
  {
    *car = *car * 16 ;
    if ((car_hex[i] >= '0') && (car_hex[i] <= '9'))
    	*car += (unsigned long) car_hex[i] - 48 ;
    if ((car_hex[i] >= 'A') && (car_hex[i] <= 'F'))
    	*car += (unsigned long) car_hex[i] - 55 ;
  }
}

/**
 * Convert from utf8 to local iso-8859-15 equivalent charset
 */
string
FromUTF8ToISO(const string sUTF8)
{
	// Shortcut if empty
	if (string("") == sUTF8)
		return string("") ;

  size_t iUTF8Len = strlen(sUTF8.c_str()) ;

  char* pIn  = new char[iUTF8Len + 1] ;
  strcpy(pIn, sUTF8.c_str()) ;

  char* pOut = new char[iUTF8Len + 1] ;

	const char* p = pIn ;
	// const char* q = pIn + iUTF8Len ;

	unsigned long charlen = 0 ;
	wchar_t       wc      = 0 ;

  int iCursor = 0 ;
	// while (p < q)
  for (size_t i = 0 ; i < iUTF8Len ; i++)
	{
		unsigned char mask = 0x3f;

    // If we are not already processing a multichar UTF8 char
		if (charlen == 0)
		{
			// Determine length of utf8 encoded wchar_t
			if ((*p & 0xf0 ) == 0xe0)
			{
				charlen = 3;
				mask = 0x0f;
			}
			else if ((*p & 0xe0 ) == 0xc0)
			{
				charlen = 2;
				mask = 0x1f;
			}
			else
			{
				charlen = 1;
				mask = 0x7f;
			}

			// Reset char
			wc = 0;
		}

		// Convert the byte
		wc <<= 6;
		wc |= (*p & mask);

		// Bump ptr
		p++;

		// Reduce byte remaining count and write it out if done
		if (0 == --charlen)
		{
			// Special for Euro
			if (wc == 0x20AC)
				// Use iso-8859-15 code for Euro
				pOut[iCursor++] = 0xA4 ;
			else if (wc > 0x00FF)
				pOut[iCursor++] = '?' ;
			else
				pOut[iCursor++] = wc & 0x00FF ;
		}
	}

  pOut[iCursor] = '\0' ;

  string sOut = string(pOut) ;

  delete[] pOut ;
  delete[] pIn ;

  return sOut ;
}

/**
 * Convert from local iso-8859-15  to UTF8 equivalent charset
 */
string
FromISOToUTF8(const string sISO)
{
  if (string("") == sISO)
    return string("") ;

  size_t iIsoLen = strlen(sISO.c_str()) ;

  unsigned char* pIn  = new unsigned char[iIsoLen + 1] ;
  const unsigned char* pIp = pIn ;

  unsigned char* pIt = pIn ;
  for (size_t i = 0 ; i < iIsoLen ; i++)
    *pIt++ = sISO[i] ;
  *pIt = '\0' ;


  // For safety, ensure that output buffer is twice as large as input buffer
  //
  unsigned char* pOut = new unsigned char[(2 * iIsoLen) + 1] ;

  const unsigned char* p = pOut ;

  while (*pIn)
  {
    if (*pIn < 128)
      *pOut++ = *pIn++ ;
    else
      *pOut++ = 0xc2 + (int(*pIn) > 0xbf),
      *pOut++ = (*pIn++ & 0x3f) + 0x80 ;
  }

  *pOut = '\0' ;

  string sOut = string((char*) p) ;

  delete[] p ;
  delete[] pIp ;

  return sOut ;
}

/*
// -----------------------------------------------------------------------------
// Function    : void format_date(char date[])
// Description : Retourne une date au format AAAAMMJJmmss � partir du format de asctime().
// Parameters  : adresse de la cha�ne dans laquelle on d�sire placer la date.
// -----------------------------------------------------------------------------
// Created on 17/09/1992
// Last update on 07/10/1992
// -----------------------------------------------------------------------------
void format_date(char date[])
{
  char mois[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" } ;
  int i ;
  struct tm *tp ;  long t ;
  char temp[26], month[4], numero[3] = "  " ;

  // Prise de temp
  time(&t) ;
  tp = localtime(&t) ;
  strcpy(temp, asctime(tp)) ;

  // R�cup�ration de l'ann�e
  for(i = 20 ; i < 24 ; i++)
  	date[i-20] = temp[i] ;
  date[4] ='\0' ;

  // R�cup�ration du mois
  for (i = 4 ; i < 7 ; i++)
  	month[i-4] = temp[i] ;
  month[3] = '\0' ;
  for (i = 0 ; (i < 12) && (strcmp(month, mois[i]) != 0) ; i++)
  	;
  numacar(numero, i+1, 2) ;
  strcat(date, numero) ;

  // R�cup�ration du jour
  for (i = 8 ; i < 10 ; i++)
  	numero[i-8] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // R�cup�ration de l'heure
  for (i = 11 ; i < 13 ; i++)
  	numero[i-11] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // R�cup�ration des minutes
  for (i = 14 ; i < 16 ; i++)
  	numero[i-14] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // R�cup�ration des secondes
  for (i = 17 ; i < 19 ; i++)
  	numero[i-17] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;
  return ;
}
*/

// -----------------------------------------------------------------------------
// Function    : void carcat(unsigned char *chaine, unsigned char caractere)
// Description : �quivalent de strcat, en ajoute un char en fin de cha�ne
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 15/06/1994
// Last update on 15/06/1994
// -----------------------------------------------------------------------------
void
carcat(char* chaine, char caractere)
{
  if ((char*) NULL == chaine)
    return ;

  char ptgag[2] ;
  ptgag[0] = caractere ; ptgag[1] = '\0' ;
  strcat(chaine, ptgag) ;
}

// -----------------------------------------------------------------------------
// Function    : SetInternational(void)
// -----------------------------------------------------------------------------
// Arguments   : AUCUN
// Description : Va chercher dans WINI.INI les formats d'affichage des dates, heure, ...
// Returns     : RIEN
// -----------------------------------------------------------------------------
void
SetInternational(void)
{
  static char cName [] = "intl" ;

  iDate = GetProfileInt(cName, "iDate", 0) ;
  iTime = GetProfileInt(cName, "iTime", 0) ;

  GetProfileString(cName, "sDate",	"/", sDate,	2) ;
  GetProfileString(cName, "sTime",	":", sTime,	2) ;
  GetProfileString(cName, "s1159", "AM", sAMPM[0], 5) ;
  GetProfileString(cName, "s2359", "PM", sAMPM[1], 5) ;
}

// -----------------------------------------------------------------------------
// Function    : ptrInit(char* &strPtr, unsigned int length)
// Arguments   :	strPtr   ->  pointeur sur une cha�ne qu'il faut initialiser
//             		length   ->  longueur de la cha�ne
// Description : Initialise un pointeur sur une cha�ne (char *)
//             	 Alloue un espace de longueur length. Initialise la cha�ne des blancs.
// Returns     : TRUE si l'initialisation s'est pass�e sans pb. FALSE sinon
// Example     : ptrInit(&Essai, 5, '$') ;
// -----------------------------------------------------------------------------
BOOL
ptrInit(char** strPtr, unsigned int length, char car)
{
	*strPtr = new char[length + 1] ;
 	if (*strPtr == NULL)
  	return FALSE ;
 	else
 	{
    memset(*strPtr, car, length);
    *(*strPtr + length) = '\0';
 	}
 	return TRUE ;
}

// -----------------------------------------------------------------------------
// Function    : ptrInit(char* &strPtr,const char* strInit)
// Arguments   :	strPtr   ->  pointeur sur une cha�ne qu'il faut initialiser
//             		strInit  ->  cha�ne d'initialisation ou NULL si l'on d�sire des blancs
// Description : Initialise un pointeur sur une cha�ne (char *). Initialise la cha�ne avec strInit.
// Returns     : TRUE si l'initialisation s'est pass�e sans pb. FALSE sinon.
// Example     :  ptrInit(&Essai, "AAAAA");
// -----------------------------------------------------------------------------
BOOL
ptrInit(char** strPtr, const char* strInit)
{
	*strPtr = new char[strlen(strInit) + 1] ;
 	if (*strPtr == NULL)
	 	return FALSE ;
 	else
	 	strcpy(*strPtr, strInit) ;
 	return TRUE ;
}

BOOL
ptrInit(unsigned char** strPtr, const unsigned char* strInit)
{
	return ptrInit((char**)strPtr, (char*)strInit);
}

// -----------------------------------------------------------------------------
// Function    : erreur(Pchar texte, int gravite, Retcode ErrPx)
// Arguments   :	texte   -> texte affich� en cas d'erreur.
//            		gravit� -> niveau de gravit�.
// Description : Affiche les messages d'erreur et sort si le niveau de gravit� est 1.
// Returns     : Retourne 0 si le probl�me rencontr� est surmontable.
// -----------------------------------------------------------------------------
int
erreur(const char *texte, ERRORTYPE gravite, DBIResult ErrPx, HWND parent, string sHelp)
{
  if ((char*) NULL == texte)
    return 0 ;

  // NOTE : Si parent == NULL (cas par d�faut)
  // La MessageBox ne sera pas modale !!!
  // Comme on ne peut acc�der au contexte, il faut passer un GetHandle()
  // pour ce param�tre � l'appel de la fonction.
  // Attention GetMainWindow()->GetHandle() ne vaut pas NULL...
  if (('\0' == texte[0]) && (DBIERR_NONE == ErrPx))
  	return(0) ;

  string sBuffer = string(texte) ;

  if ((0 != ErrPx) && (DBIERR_NONE != ErrPx))
  {
    char       dbi_status[DBIMAXMSGLEN * 5] = {'\0'} ;
    DBIMSG     dbi_string = {'\0'} ;
    DBIErrInfo ErrInfo ;

    if (DBIERR_NONE != ErrPx)
    {
      DbiGetErrorInfo(TRUE, ErrInfo) ;
      if (ErrInfo.iError == ErrPx)
      {
        wsprintf(dbi_status, "  ERROR %s", ErrInfo.szErrCode) ;
        if (strcmp(ErrInfo.szContext[0], ""))
          wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[0]) ;
        if (strcmp(ErrInfo.szContext[1], ""))
          wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[1]) ;
        if (strcmp(ErrInfo.szContext[2], ""))
          wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[2]) ;
        if (strcmp(ErrInfo.szContext[3], ""))
          wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[3]) ;
      }
      else
      {
        DbiGetErrorString(ErrPx, dbi_string) ;
        wsprintf(dbi_status, "  ERROR %s", dbi_string) ;
      }
      if (string("") != sBuffer)
        sBuffer += string("\r\n\r\n") ;
      sBuffer += string(dbi_status) ;
    }
    //return ErrorValue ;
  }

  if (getModeFileErr() == true)
  	fileError(sBuffer.c_str()) ;
  else
  {
/*
    if (string("") == sHelp)
    {      if (gravite == warningError)
        MessageBox(parent, sBuffer.c_str(), "AVERTISSEMENT", MB_ICONEXCLAMATION | MB_OK) ;
      else
        MessageBox(parent, sBuffer.c_str(), "ERREUR", MB_ICONEXCLAMATION | MB_OK) ;    }
    else    {
      sBuffer += string("\r\n\r\nSouhaitez vous consulter la marche � suivre face � une telle erreur ?") ;
      int iRetour ;
      if (gravite == warningError)        iRetour = MessageBox(parent, sBuffer.c_str(), "AVERTISSEMENT", MB_YESNO) ;      else        iRetour = MessageBox(parent, sBuffer.c_str(), "ERREUR", MB_YESNO) ;      if (iRetour == IDYES)
        ::ShellExecute(parent, "open", sHelp.c_str(), "", "", SW_SHOWNORMAL) ;
    }
*/

    if (warningError == gravite)      MessageBox(parent, sBuffer.c_str(), "AVERTISSEMENT", MB_ICONEXCLAMATION | MB_OK) ;
    else
      MessageBox(parent, sBuffer.c_str(), "ERREUR", MB_ICONEXCLAMATION | MB_OK) ;    if (string("") != sHelp)      ::ShellExecute(parent, "open", sHelp.c_str(), "", "", SW_SHOWNORMAL) ;
  }
  if (fatalError == gravite)
  	exit(1) ;

  return 0 ;
}

// fonctions de gestion des messages d'erreur
bool
getModeFileErr()
{
	return bModeFile ;
}

void
setModeFileErr(bool bMode)
{
	bModeFile = bMode ;
}

void
fileError(string sError)
{
  ifstream  inFile ;
  ofstream  outFile ;
  string    line ;
  string    sLastError ;

  inFile.open("ferror.log") ;
  if (inFile)
  {
    while (!inFile.eof())
    {
      getline(inFile,line) ;
      if (line != "")
        sLastError += line + "\n" ;
    }
    inFile.close() ;
  }
  else // cas fichier vide
    sLastError = "" ;

  // on ajoute la nouvelle erreur
  sLastError += sError + "\n" ;

  // On �crit le nouveau fichier reqerror.log
  outFile.open("ferror.log") ;
	if (!outFile)
  {
    // cas d'erreur bloquant...
    MessageBox(NULL, "Erreur d'ouverture en �criture du fichier ferror.log.", "Erreur NAUTILUS", MB_ICONEXCLAMATION) ;
    return ;
  }

  for (size_t i = 0 ; i < strlen(sLastError.c_str()) ; i++)
    outFile.put(sLastError[i]) ;

  outFile.close() ;
}

// -----------------------------------------------------------------------------
// convertit  sValeur en un double
// -----------------------------------------------------------------------------
double
StringToDouble(string sValeur, string *psError)
{
  if (string("") == sValeur)
    return 0 ;

  // First, check if it is a fraction
  //
  size_t iPos = sValeur.find("/") ;
  if (NPOS != iPos)
  {
    string sNumerator   = string("0") ;
    string sDenominator = string("1") ;

    if (0 == iPos) // of the kind "/2" for 1/2
    {
      sNumerator   = string("1") ;
      sDenominator = sValeur ;
    }
    else if (strlen(sValeur.c_str()) == iPos + 1) // of the kind "3/" for 3
    {
      sNumerator   = sValeur ;
      sDenominator = string("1") ;
    }
    else
    {
      sNumerator   = string(sValeur, 0, iPos) ;
      sDenominator = string(sValeur, iPos + 1, strlen(sValeur.c_str()) - iPos - 1) ;
    }

    double dNumerator   = StringToDouble(sNumerator) ;
    double dDenominator = StringToDouble(sDenominator) ;

    if (dDenominator > 0)
      return dNumerator / dDenominator ;

    return 0 ;
  }

  // Extraction de la partie enti�re et de la partie d�cimale
  //
  size_t iSepar = sValeur.find(".") ;
  if (string::npos == iSepar)
    iSepar = sValeur.find(",") ;

  string sEntier = string("") ;
  string sDecima = string("") ;

  if ((string::npos == iSepar) || (iSepar > strlen(sValeur.c_str())))
    sEntier = sValeur ;
  else
  {
    if (iSepar > 0)
      sEntier = string(sValeur, 0, iSepar) ;
    sDecima = string(sValeur, iSepar + 1, strlen(sValeur.c_str()) - iSepar - 1) ;
  }

  // Calcul de la valeur
  double dValeur = 0 ;

  // Partie enti�re
  for (size_t i = 0 ; i < strlen(sEntier.c_str()) ; i++)
  {
    if (false == isdigit(sEntier[i]))
    {
      if (psError)
        *psError = string("Non digit char ('") + string(1, sEntier[i]) + string("'") ;
      return 0 ;
    }

    dValeur = (dValeur * double(10)) + double(donneDigit(sEntier[i])) ;
  }

  // Partie d�cimale
  double multi = double(1) ;
  for (size_t i = 0 ; i < strlen(sDecima.c_str()) ; i++)
  {
    if (false == isdigit(sDecima[i]))
    {
      if (psError)
        *psError = string("Non digit char ('") + string(1, sDecima[i]) + string("'") ;

      return 0 ;
    }

    multi = multi * 0.1 ;
    dValeur += double(donneDigit(sDecima[i])) * multi ;
  }

  if (psError)
    *psError = string("") ;

  return dValeur ;
}

//-------------------------------------------------------------------
// convertit un double en une string
//-------------------------------------------------------------------
string
DoubleToString(double* pDouble, int iEntier, int iDecimal)
{
  if ((double*) NULL == pDouble)
    return string("") ;

  double dVal = *pDouble ;
  if (0 == dVal)
  	return string("0") ;

  bool bNegatif = false ; //double n�gatif
  if (dVal < 0)
  {
    bNegatif = true ;
    dVal = -dVal ;
  }
  double dLog = log10(dVal) ;
  int	   iLog = int(dLog) ;
  double dMult = pow(double(10), double(iLog)) ;
  double dResu ;

  string sResult = string("") ;

  // Assemblage de la partie enti�re
  while (iLog >= 0)
  {
    dResu = floor(dVal / dMult) ;
    sResult += string(1, donneChar(int(dResu))) ;

    dVal = dVal - (dResu * dMult) ;
    dMult = dMult / 10 ;
    iLog-- ;
  }
  if ((0 == iDecimal) || ((-1 == iDecimal) && (0 == dVal)))
  {
    if (bNegatif)
    	sResult = string("-") + sResult ;
    return sResult ;
  }

  // Assemblage de la partie d�cimale
  if (string("") == sResult)
  	sResult = string("0") ;
  sResult += string(".") ;
  iLog = 0 ;
  bool tourner = true ;
  while (tourner)
  {
    dResu = floor(10 * dVal) ;
    // Si on en est � inscrire le dernier chiffre significatif, il faut
    // arrondir
    if ((iDecimal > 0) && (iLog == iDecimal-1) && (10 * dVal - dResu > double(0.5)))
    {
      if (dResu < 9)
      	sResult += string(1, donneChar(int(dResu) + 1)) ;
      else
      {
      	int i = strlen(sResult.c_str()) - 1 ;
        bool bIncrementer = true ;
        while ((i >= 0) && bIncrementer)
        {
        	if (isdigit(sResult[i]))
          {
          	int j = donneDigit(sResult[i]) ;
            if (j < 9)
            {
              j++ ;
              sResult[i] = donneChar(j) ;
              bIncrementer = false ;
            }
            else
            {
              sResult[i] = '0' ;
              if (0 == i)
              	sResult = string("1") + sResult ;
            }
          }
          i-- ;
        }
        sResult += string("0") ;
      }
    }
    else
    	sResult += string(1, donneChar(int(dResu))) ;

    dVal = (10 * dVal) - dResu ;
    iLog++ ;
    if ((iDecimal > 0) && (iLog >= iDecimal))
    	tourner = false ;
    if ((-1 == iDecimal) && ((iLog > 10) || (0 == dVal)))
    	tourner = false ;
  }

  if (bNegatif)
  	sResult = string("-") + sResult ;
  return sResult ;
}

// -----------------------------------------------------------------------------
//
// Impl�mentation des m�thodes ClasseStringVector (vecteur de string)
//
// -----------------------------------------------------------------------------

ClasseStringVector::~ClasseStringVector()
{
	vider() ;
}

ClasseStringVector::ClasseStringVector()
	                 :vectclassString()
{
}

void
ClasseStringVector::vider()
{
	if (empty())
		return ;

	iterClassString i = begin() ;
  while (end() != i)
  {
    delete (*i) ;
    erase(i) ;
  }
}

ClasseStringVector&
ClasseStringVector::operator=(const ClasseStringVector& src)
{
	if (this == &src)
		return (*this) ;

	vider() ;

try
{
	if (false == src.empty())
    for (iterClassStringConst i = src.begin() ; src.end() != i ; i++)
      push_back(new classString(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception ClasseStringVector = operator.", standardError, 0) ;
}
	return (*this) ;
}

ClasseStringVector::ClasseStringVector(const ClasseStringVector& src)
{
try
{
	if (false == src.empty())
		for (iterClassStringConst i = src.begin() ; src.end() != i ; i++)
    	push_back(new classString(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception ClasseStringVector copy ctor.", standardError, 0) ;
}
}
intClasseStringVector::operator==(const ClasseStringVector& o) const{  // If one is empty, then both must be empty  //  if (empty() || o.empty())    return (empty() && o.empty()) ;  // Must have same size  //  if (size() != o.size())    return 0 ;  // Compare one to one  //  iterClassStringConst it      = begin() ;  iterClassStringConst otherIt = o.begin() ;  for ( ; (end() != it) && (o.end() != otherIt) ; it++, otherIt++)    if (**it != **otherIt)      return 0 ;  return 1 ;}// -----------------------------------------------------------------------------//// classe classString//// -----------------------------------------------------------------------------

classString::classString(const classString& src){
  sItem      = src.sItem ;
  colonne    = src.colonne ;
  coche      = src.coche ;
  sQualifier = src.sQualifier ;
}

// -----------------------------------------------------------------------------
// Op�rateur d'affectation
// -----------------------------------------------------------------------------
classString&
classString::operator=(const classString& src)
{
	if (this == &src)
		return (*this) ;

  sItem      = src.sItem ;
  colonne    = src.colonne ;
  coche      = src.coche ;
  sQualifier = src.sQualifier ;

  return (*this) ;
}

classString::classString(string sChaine, int Colonne, bool Coche, string sQualif)
{
  sItem 	   = sChaine ;
  colonne    = Colonne ;
  coche 	   = Coche ;
  sQualifier = sQualif ;
}

int
classString::operator==(const classString& o) const
{
  return ((sItem      == o.sItem)   &&
          (colonne    == o.colonne) &&
          (coche 	    == o.coche)   &&
          (sQualifier == o.sQualifier)) ;
}

// -----------------------------------------------------------------------------
// Split sModel into a left part and a right part on each side of first separator
// -----------------------------------------------------------------------------
void
splitString(const string sModel, string *psLeft, string *psRight, char separator)
{
  if (((string*) NULL == psLeft) || ((string*) NULL == psRight))
    return ;

  *psLeft  = string("") ;
  *psRight = string("") ;

  size_t iPosit = sModel.find(separator) ;

  // No separator or at the end of model
  //
  if (string::npos == iPosit)
  {
    *psLeft = sModel ;
    return ;
  }

  size_t iLen = strlen(sModel.c_str()) ;

  // Model starts with separator
  //
  if (0 == iPosit)
  {
    *psRight = string(sModel, 1, iLen - 1) ;
    return ;
  }

  *psLeft = string(sModel, 0, iPosit) ;

  // Separator at the end of model
  //
  if (iPosit == iLen - 1)
    return ;

  *psRight = string(sModel, iPosit + 1, iLen - iPosit - 1) ;
}

// -----------------------------------------------------------------------------
// Parse sModel as starting with qualifiers
//
// We suppose that : sModel = sQualifier + char + sResult
//               or  sModel = char + sResult
//                                      where char belongs to sQualifiersList
//               or  sModel = sResult
//                                      when no char from sQualifiersList found
//
// For example "AVALA/GDOPP/VSURC" ->
//                                     sResult    = "VSURC"
//                                     sQualifier = "AVALA/GDOPP"
// -----------------------------------------------------------------------------
void getQualifiers(string sModel, string &sResult, string &sQualifier, string sQualifiersList)
{
  // This function does nothing right, but the rest of the code is used to it
  //
  sResult    = sModel ;
  sQualifier = string("") ;
  return ;

  sResult    = string("") ;
  sQualifier = string("") ;

  if (string("") == sModel)
    return ;

  if (string("") == sQualifiersList)
  {
    sResult = sModel ;
    return ;
  }

  int i = 0 ;
  bool bKeepGoing = true ;

  size_t iModelLen = strlen(sModel.c_str()) ;

  // Find the first element from sModel that belongs to sQualifier
  //
  while ((i < iModelLen) && bKeepGoing)
  {
    char cAtI = sModel[i] ;
    if (NPOS != sQualifiersList.find(string(1, cAtI)))
      bKeepGoing = false ;
    else
      i++ ;
  }

  if (0 == i)
  {
    sResult = sModel ;
    return ;
  }

  sQualifier = string(sModel, 0, i) ;

  if (i == iModelLen)
    return ;

  sResult = string(sModel, i, iModelLen - i) ;
}

// -----------------------------------------------------------------------------
// Split pChaine according to sSeparator and store parts into pVect
//
// Default value for sSeparator is "|"
//
// sQualifiers is used in case when work with paths and we want to separate the
// information and its path for example "AVALA/GDOPP/VSURC" ->
// sItem      = "VSURC"
// sQualifier = "AVALA/GDOPP"
// -----------------------------------------------------------------------------
void
DecomposeChaine(const string* pChaine, ClasseStringVector* pVect, const string sSeparator, const string sQualifiers)
{
	if (((string*) NULL == pChaine) || ((ClasseStringVector*) NULL == pVect))
		return ;

  if ((string("") == *pChaine) || (string("") == sSeparator))
		return ;

  size_t posit = pChaine->find(sSeparator) ;

try
{
  // No separator found, add the whole string
  //
  if (string::npos == posit)
  {
    pVect->push_back(new classString(*pChaine)) ;
    return ;
  }

  size_t debut     = 0 ;
  size_t iSeparLen = strlen(sSeparator.c_str()) ;

  // Iterate while the separator is found
  //
  while (string::npos != posit)
  {
    // This separator is next to the previous one, insert an empty string
    //
    if (debut == posit)
      pVect->push_back(new classString(string(""))) ;
    else
    {
      string sModel  = string(*pChaine, debut, posit - debut) ;

      string sResult = string("") ;
      string sQualif = string("") ;
      getQualifiers(sModel, sResult, sQualif, sQualifiers) ;

      pVect->push_back(new classString(sResult, -1, false, sQualif)) ;
    }

    debut = posit + iSeparLen ;
    if ((debut + iSeparLen <= strlen(pChaine->c_str())) &&
                           (sSeparator == string(*pChaine, debut, iSeparLen)))
      posit = debut ;
    else
      posit = pChaine->find(sSeparator, debut) ;
  }

  if (strlen(pChaine->c_str()) - debut > 0)
  {
    string sModel  = string(*pChaine, debut, strlen(pChaine->c_str()) - debut) ;
    string sResult = string("") ;
    string sQualif = string("") ;
    getQualifiers(sModel, sResult, sQualif, sQualifiers) ;

    pVect->push_back(new classString(sResult, -1, false, sQualif)) ;
  }
  else
    pVect->push_back(new classString(string(""))) ;

} // try
catch (...)
{
  string sErr = string("Exception DecomposeChaine for \"") + *pChaine + string("\"") ;
  erreur(sErr.c_str(), standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Calcul de l'�ge du patient
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
int
donne_age(char *datex, char *dateNaiss)
{
	if (((char*) NULL == datex) || ((char*) NULL == dateNaiss))
		return -1 ;

	// On ne calcule l'�ge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Ann�e de l'examen
	unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Ann�e de naissance du patient
	unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'ann�e
	signed int age = i - j ;

	// Correctifs en fonction de mois et jour
	unsigned int mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]) ;
	unsigned int jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]) ;
	unsigned int mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]) ;
	unsigned int jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]) ;

	if ((mois_jour < mois_nais) || ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
		age-- ;
	if ((age < 0) || (age > 150))
		return -1 ;
	return age ;
}

int
donne_age(string sDatex, string sDateNaiss)
{
  if ((strlen(sDateNaiss.c_str()) < 8) || (strlen(sDatex.c_str()) < 8))
		return -1 ;

	// On ne calcule l'�ge que si on a la date de naissance
	if ((sDateNaiss == string("00000000")) || (sDateNaiss == string("19000000")))
		return -1 ;

	// Ann�e de l'examen
	unsigned int i = 10 * donneDigit(sDatex[0]) + donneDigit(sDatex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(sDatex[2]) + donneDigit(sDatex[3]) ;

	// Ann�e de naissance du patient
	unsigned int j = 10 * donneDigit(sDateNaiss[0]) + donneDigit(sDateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(sDateNaiss[2]) + donneDigit(sDateNaiss[3]) ;

	// Age qu'aura le patient dans l'ann�e
	signed int age = i - j ;

	// Correctifs en fonction de mois et jour
	unsigned int mois_nais = 10 * donneDigit(sDateNaiss[4]) + donneDigit(sDateNaiss[5]) ;
	unsigned int jour_nais = 10 * donneDigit(sDateNaiss[6]) + donneDigit(sDateNaiss[7]) ;
	unsigned int mois_jour = 10 * donneDigit(sDatex[4]) + donneDigit(sDatex[5]) ;
	unsigned int jour_jour = 10 * donneDigit(sDatex[6]) + donneDigit(sDatex[7]) ;

	if ((mois_jour < mois_nais) || ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
		age-- ;
	if ((age < 0) || (age > 150))
		return -1 ;

	return age ;
}

// -----------------------------------------------------------------------------
// Calcul de l'�ge des b�b�s en mois
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
int
donne_age_mois(char *datex, char *dateNaiss)
{
	if (((char*) NULL == datex) || ((char*) NULL == dateNaiss))
		return -1 ;

	// On ne calcule l'�ge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Ann�e de l'examen
	unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Ann�e de naissance du patient
	unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'ann�e (en mois)
	signed int age = (i - j) * 12 ;

	// Correctifs en fonction de mois et jour
	unsigned int mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]) ;
	unsigned int jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]) ;
	unsigned int mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]) ;
	unsigned int jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]) ;

	age = age + mois_jour - mois_nais ;
	if (jour_jour < jour_nais)
		age-- ;

	return age ;
}

int
donne_age_mois(string sDatex, string sDateNaiss)
{
  if ((strlen(sDateNaiss.c_str()) < 8) || (strlen(sDatex.c_str()) < 8))
		return -1 ;
	// On ne calcule l'�ge que si on a la date de naissance
	if ((sDateNaiss == string("00000000")) || (sDateNaiss == string("19000000")))
		return -1 ;

	// Ann�e de l'examen
	unsigned int i = 10 * donneDigit(sDatex[0]) + donneDigit(sDatex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(sDatex[2]) + donneDigit(sDatex[3]) ;

	// Ann�e de naissance du patient
	unsigned int j = 10 * donneDigit(sDateNaiss[0]) + donneDigit(sDateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(sDateNaiss[2]) + donneDigit(sDateNaiss[3]) ;

	// Age qu'aura le patient dans l'ann�e (en mois)
	signed int age = (i - j) * 12 ;

	// Correctifs en fonction de mois et jour
	unsigned int mois_nais = 10 * donneDigit(sDateNaiss[4]) + donneDigit(sDateNaiss[5]) ;
	unsigned int jour_nais = 10 * donneDigit(sDateNaiss[6]) + donneDigit(sDateNaiss[7]) ;
	unsigned int mois_jour = 10 * donneDigit(sDatex[4]) + donneDigit(sDatex[5]) ;
	unsigned int jour_jour = 10 * donneDigit(sDatex[6]) + donneDigit(sDatex[7]) ;

	age = age + mois_jour - mois_nais ;
	if (jour_jour < jour_nais)
		age-- ;

	return age ;
}

int
get_age(string *datex, string *dateNaiss)
{
  if (((string*) NULL == datex) || ((string*) NULL == dateNaiss))
    return -1 ;
  if ((strlen(datex->c_str()) != 8) || (strlen(dateNaiss->c_str()) != 8))
    return -1 ;

  return donne_age(*datex, *dateNaiss) ;
}

int
get_age_month(string *datex, string *dateNaiss)
{
  if (((string*) NULL == datex) || ((string*) NULL == dateNaiss))
    return -1 ;
  if ((strlen(datex->c_str()) != 8) || (strlen(dateNaiss->c_str()) != 8))
    return -1 ;

  char szDatex[9] ;
  strcpy(szDatex, datex->c_str()) ;
  char szDatNais[9] ;
  strcpy(szDatNais, dateNaiss->c_str()) ;

  return donne_age_mois(szDatex, szDatNais) ;
}

// -----------------------------------------------------------------------------
// Donne l'intitul� (Monsieur, Madame...)
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
void
donne_intitule_patient(string *intitule, int age, bool bFeminin, bool bShort, string sLang)
{
	if ((string*) NULL == intitule)
		return ;

	if (bFeminin)
		*intitule = "Madame " ;
	else
		*intitule = "Monsieur " ;

	// Correctifs en fonction de l'age
	if ((age < 0) || (age > 150))
  	return ;
	if (age <= 15)
	{
		*intitule = "l'enfant " ;
		return ;
	}
	if (bFeminin && (age < 18))
		*intitule = "Mademoiselle " ;
}

// -----------------------------------------------------------------------------
// Donne l'intitul� de l'�ge du patient              �
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
void
donne_intitule_age(string *intitule, char *datex, char *dateNaiss)
{
	if (((string*) NULL == intitule) || ((char*) NULL == datex) || ((char*) NULL == dateNaiss))
		return ;

	*intitule = string("") ;

	// Calcul de l'�ge
	int age = donne_age(datex, dateNaiss) ;

  char ptgag[4] ;

	// Patient d'au moins 2 ans
	if (age >= 2)
	{
		numacar(ptgag, age, 3) ;
		while (ptgag[0] == '0')
			for (int i = 0 ; ptgag[i] != '\0' ; i++)
				ptgag[i] = ptgag[i+1] ;
		*intitule = string(ptgag) + " an" ;
		if (age > 1) *intitule += "s" ;
	}
	// B�b� de moins de 2 ans
	else if (age >= 0)
	{
		age = donne_age_mois(datex, dateNaiss) ;
		if (age > 0)
		{
			numacar(ptgag, age, 2) ;
      if (ptgag[0] == '0')
				for (int i = 0 ; ptgag[i] != '\0' ; i++)
					ptgag[i] = ptgag[i+1] ;
			*intitule = string(ptgag) + " mois" ;
		}
	}
}

string
donne_intitule_age(string sDatex, string sDateNaiss)
{
	string sIntitule = string("") ;

	// Calcul de l'�ge
	int age = donne_age(sDatex, sDateNaiss) ;

  char ptgag[4] ;

	// Patient d'au moins 2 ans
	if (age >= 2)
	{
		numacar(ptgag, age, 3) ;
		while (ptgag[0] == '0')
			for (int i = 0 ; ptgag[i] != '\0' ; i++)
				ptgag[i] = ptgag[i+1] ;
		sIntitule = string(ptgag) + " an" ;
		if (age > 1)
      sIntitule += "s" ;
	}
	// B�b� de moins de 2 ans
	else if (age >= 0)
	{
		age = donne_age_mois(sDatex, sDateNaiss) ;
		if (age > 0)
		{
			numacar(ptgag, age, 2) ;
      if (ptgag[0] == '0')
				for (int i = 0 ; ptgag[i] != '\0' ; i++)
					ptgag[i] = ptgag[i+1] ;
			sIntitule = string(ptgag) + " mois" ;
		}
	}

	return sIntitule ;
}

// -----------------------------------------------------------------------------
// D�code une date AAAAMMJJ
// -----------------------------------------------------------------------------
// Created on 16/10/1992
// Last update on 16/10/1992
// -----------------------------------------------------------------------------
void
decode_date(string *resultat, char *date, string sLang)
{
	if (((string*) NULL == resultat) || ((char*) NULL == date))
		return ;

  if (strlen(date) < 8)
  {
    *resultat = string() ;
    return ;
  }

	char date2[11] ;

	unsigned int jour = 10 * donneDigit(date[6]) + donneDigit(date[7]) ;
	unsigned int mois = 10 * donneDigit(date[4]) + donneDigit(date[5]) ;
	if ((jour < 1) || (jour > 31))
  	jour = 0 ;
	if ((mois < 1) || (mois > 12))
  	mois = 0 ;

	if (0 == jour)
	{
		strcpy(date2, "19XX") ;
		for (int i = 0 ; i < 4 ; i++)
    	date2[i] = date[i] ;
		*resultat = " en " ;
		if (0 != mois)
		{
			switch (mois)
			{
				case 1  : *resultat += "janvier " ;		break ;
				case 2  : *resultat += "f�vrier " ;		break ;
				case 3  : *resultat += "mars " ;			break ;
				case 4  : *resultat += "avril " ;			break ;
				case 5  : *resultat += "mai " ;				break ;
				case 6  : *resultat += "juin " ;			break ;
				case 7  : *resultat += "juillet " ;		break ;
				case 8  : *resultat += "ao�t " ;			break ;
				case 9  : *resultat += "septembre " ;	break ;
				case 10 : *resultat += "octobre " ;		break ;
				case 11 : *resultat += "novembre " ;	break ;
				case 12 : *resultat += "d�cembre " ;	break ;
			}
		}
		*resultat += string(date2) ;
	}
	else
	{
		strcpy(date2, "XX/XX/19XX") ;
		for (int i = 0 ; i < 4 ; i++)
    	date2[i+6] = date[i] ;
		for (int i = 0 ; i < 2 ; i++)
    	date2[i+3] = date[i+4] ;
		for (int i = 0 ; i < 2 ; i++)
    	date2[i]   = date[i+6] ;
		*resultat = " le " ;
		*resultat += string(date2) ;
	}
}

// -----------------------------------------------------------------------------
//
// Impl�mentation des m�thodes VecteurString
//
// -----------------------------------------------------------------------------
VectString::~VectString()
{
	vider() ;
}

VectString::VectString()
{
}

bool
VectString::contains(const string sModele) const
{
	if (empty())
		return false ;

  for (IterStringConst i = begin() ; end() != i ; i++)
  	if (sModele == **i)
			return true ;

	return false ;
}

string
VectString::containsStartWith(const string sModele) const
{
	if (empty() || (string("") == sModele))
		return string("") ;

  size_t iModeleLen = strlen(sModele.c_str()) ;

  for (IterStringConst i = begin() ; end() != i ; i++)
  {
    size_t iInstanceLen = strlen((*i)->c_str()) ;

  	if ((iInstanceLen >= iModeleLen) && (string(**i, 0, iModeleLen) == sModele))
			return **i ;
  }

	return string("") ;
}

void
VectString::remove(const string sModele)
{
	if (empty())
		return ;

  for (IterString i = begin() ; end() != i ; )
  {
  	if (sModele == **i)
    {
			delete (*i) ;
      erase(i) ;
    }
    else
      i++ ;
  }
}

void
VectString::removeStartWith(const string sModele)
{
	if (empty() || (string("") == sModele))
		return ;

  size_t iModeleLen = strlen(sModele.c_str()) ;

  for (IterString i = begin() ; end() != i ; )
  {
    size_t iInstanceLen = strlen((*i)->c_str()) ;

  	if ((iInstanceLen >= iModeleLen) && (string(**i, 0, iModeleLen) == sModele))
    {
			delete (*i) ;
      erase(i) ;
    }
    else
      i++ ;
  }
}

void
VectString::fillFromString(const string& sSource, const string sSepar)
{
  if ((string("") == sSource) || (string("") == sSepar))
    return ;

  size_t iPosit = sSource.find(sSepar) ;

  // No separator
  //
  if (string::npos == iPosit)
  {
    string sToAdd = sSource ;

    strip(sToAdd, stripBoth) ;
    if (string("") == sToAdd)
      return ;

    push_back(new string(sToAdd)) ;
    return ;
  }

  size_t debut     = 0 ;
  size_t iSeparLen = strlen(sSepar.c_str()) ;
  size_t iSrcLen   = strlen(sSource.c_str()) ;

  while (string::npos != iPosit)
  {
    if (debut != iPosit)
    {
      string sToAdd = string(sSource, debut, iPosit - debut) ;

      strip(sToAdd, stripBoth) ;
      if (string("") != sToAdd)
        push_back(new string(sToAdd)) ;
    }

    debut = iPosit + iSeparLen ;
    if ((debut + iSeparLen <= iSrcLen) && (sSepar == string(sSource, debut, iSeparLen)))
      iPosit = debut ;
    else
      iPosit = sSource.find(sSepar, debut) ;
  }

  if (iSrcLen - debut > 0)
  {
    string sToAdd  = string(sSource, debut, iSrcLen - debut) ;

    strip(sToAdd, stripBoth) ;
    if (string("") != sToAdd)
      push_back(new string(sToAdd)) ;
  }
}

void
VectString::vider()
{
	if (empty())
		return ;

  IterString i = begin() ;
  while (end() != i)
  {
    delete (*i) ;
    erase(i) ;
  }
}

VectString&
VectString::operator=(const VectString& src)
{
	if (this == &src)
		return (*this) ;

	vider() ;
try
{
	if (false == src.empty())
		for (IterStringConst i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception VectString::operator=", standardError, 0) ;}
	return (*this) ;
}

VectString::VectString(const VectString& src)
{
try
{
	if (false == src.empty())
		for (IterStringConst i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception VectString copy ctor.", standardError, 0) ;
}
}

int
VectString::operator==(const VectString& o) const
{
  // If one is empty, then both must be empty
  //  if (empty() || o.empty())    return (empty() && o.empty()) ;  // Must have same size  //  if (size() != o.size())    return 0 ;  // Compare one to one  //  IterStringConst it      = begin() ;  IterStringConst otherIt = o.begin() ;  for ( ; (end() != it) && (o.end() != otherIt) ; it++, otherIt++)    if (**it != **otherIt)      return 0 ;  return 1 ;
}

// Valid operators are '!' for negation and '|' and '&' for OR and AND
//
// Pure logical means "just a combination of boolean vars" in which case
// "a b | c" is invalid. On the contrary, when the formula can contains
// calculation and comparison operators, "a - b > c | d" is valid
//
bool
isLogicalFormulaValid(string sFormula, string *psErrorMsg, bool bPureLogical)
{
  if (string("") == sFormula)
    return true ;

  string sOperators = string("!()|&") ;

  size_t cc = 0 ;
  char oper = ' ' ;

  size_t iSize = strlen(sFormula.c_str()) ;

  while (cc < iSize)
  {
    // Skip blanks and tabs
    //
    if ((' ' == sFormula[cc]) || ('\t' == sFormula[cc]))
    {
      while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))        cc++ ;

      // If pure logical, check that 2 'non operators' don't appear in a row
      //
      if (bPureLogical && (sOperators.find(sFormula[cc]) == string::npos) &&
          (sOperators.find(oper) == string::npos))
      {
        if (psErrorMsg)
          *psErrorMsg = string("missingOperatorBetweenTwoVars") ;
        return false ;
      }
    }

    // Checking current operand    //    if (cc < iSize)
    {
      // Is current operand compatible with the previous one?
      //
      if (false == areConsecutiveLogicalCharCompatible(oper, sFormula[cc]))
      {
        if (psErrorMsg)
          *psErrorMsg = string("invalidOperatorsSequence") ;
        return false ;
      }

      // ')' is always invalid
      //
      if (')' == sFormula[cc])
      {
        if (psErrorMsg)
          *psErrorMsg = string("excessOfClosingParenthesis") ;
        return false ;
      }

      // '(' opens a block and leads to a recursive call
      //
      if ('(' == sFormula[cc])
      {
        size_t iStart = cc + 1 ;
        size_t iParenthesisLevel = 1 ;

        cc++ ;

        // Looking for the closing parenthesis
        //
        while ((cc < iSize) && (iParenthesisLevel > 0))
        {
          switch (sFormula[cc])
          {
            case '(' : iParenthesisLevel++ ; break ;
            case ')' : iParenthesisLevel-- ; break ;
          }
          cc++ ;
        }

        if (iParenthesisLevel > 0)
        {
          if (psErrorMsg)
            *psErrorMsg = string("missingClosingParenthesis") ;
          return false ;
        }

        if (cc <= iStart + 1)
        {
          if (psErrorMsg)
            *psErrorMsg = string("emptyLogicalBlock") ;
          return false ;
        }

        string sInsideBlokSentence = string(sFormula, iStart, cc - iStart -1) ;

        if (false == isLogicalFormulaValid(sInsideBlokSentence, psErrorMsg, bPureLogical))
          return false ;

        oper = ')' ;
      }
      else
      {
        oper = sFormula[cc] ;
        cc++ ;
      }
    }
  }
  return true ;
}

// Is current operand compatible with the previous one?
//
bool
areConsecutiveLogicalCharCompatible(char cPrevious, char cNext)
{
  // after '!' operators !)|& are not allowed
  if (('!' == cPrevious) &&
                     (('!' == cNext) ||
                      (')' == cNext) ||
                      ('|' == cNext) ||
                      ('&' == cNext)))
    return false ;
  else
    return true ;

  // after '(' operators )|& are not allowed
  if (('(' == cPrevious) &&
                     ((')' == cNext) ||
                      ('|' == cNext) ||
                      ('&' == cNext)))
    return false ;
  else
    return true ;

  if ((')' == cPrevious) &&
                     (('!' == cNext) ||
                      ('(' == cNext)))
    return false ;
  else
    return true ;

  if ((('|' == cPrevious) || ('&' == cPrevious)) &&
                     ((')' == cNext) ||
                      ('&' == cNext) ||
                      ('|' == cNext)))
    return false ;
  else
    return true ;

  return true ;
}

void
parseLogicalBlocks(string &sFormula, map<string, string> *paBlocks)
{
  if ((string("") == sFormula) || ((map<string, string>*) NULL == paBlocks))
    return ;

  size_t iBlockStart = 0 ;
  size_t cc = 0 ;
  char oper = ' ' ;

  size_t iSize = strlen(sFormula.c_str()) ;

  while (cc < iSize)
  {
    // Skip blanks and tabs
    //
    while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))      cc++ ;

    // Checking current operand    //    if (cc < iSize)
    {
      // ')' is always invalid
      //
      if (')' == sFormula[cc])
        return ;

      // '(' opens a block and leads to a recursive call
      //
      if ('(' == sFormula[cc])
      {
        size_t iStart = cc + 1 ;
        size_t iParenthesisLevel = 1 ;

        cc++ ;

        // Looking for the closing parenthesis
        //
        while ((cc < iSize) && (iParenthesisLevel > 0))
        {
          switch (sFormula[cc])
          {
            case '(' : iParenthesisLevel++ ; break ;
            case ')' : iParenthesisLevel-- ; break ;
          }
          cc++ ;
        }

        if (iParenthesisLevel > 0)
          return ;

        if (cc <= iStart + 1)
          return ;

        string sInsideBlokSentence = string(sFormula, iStart, cc - iStart -1) ;

        strip(sInsideBlokSentence, stripBoth) ;

        // if block contains logical operators, then recurse
        //
        if (isLogicalBlock(sInsideBlokSentence))
          parseLogicalBlocks(sInsideBlokSentence, paBlocks) ;

        oper = ')' ;
      }
      // '|' or '&' separate blocks
      //
      else if (('|' == sFormula[cc]) || ('&' == sFormula[cc]))
      {
        string sInsideBlokSentence = string(sFormula, iBlockStart, cc - iBlockStart) ;

        if (string("") != sInsideBlokSentence)
        {
          string sAlias = replaceVarByAlias(&sFormula, paBlocks, iBlockStart, sInsideBlokSentence, string("block")) ;

          cc = iBlockStart + strlen(sAlias.c_str()) ;
          iSize = strlen(sFormula.c_str()) ;
        }

        iBlockStart = cc + 1 ;

        cc++ ;
      }
      // '!' can start a block
      //
      else if (('!' == sFormula[cc]) &&
                 ((0 == cc) || (cc > 0) && ((' ' == sFormula[cc-1]) ||
                                            (')' == sFormula[cc-1]))))
      {
        iBlockStart = cc + 1 ;

        cc++ ;
      }
      else
      {
        oper = sFormula[cc] ;
        cc++ ;
      }
    }
  }

  string sInsideBlokSentence = string(sFormula, iBlockStart, iSize - iBlockStart) ;

  if (string("") != sInsideBlokSentence)
    replaceVarByAlias(&sFormula, paBlocks, iBlockStart, sInsideBlokSentence, string("block")) ;

/*
  size_t iBlockStart  = 0 ;
  size_t iCurseur     = 0 ;
  size_t iFormulaSize = strlen(sFormula.c_str()) ;
  while (iCurseur < iFormulaSize)
  {
    // This char is a reserved char
    //
    if (sLogicalOperators.find(sFormula[iCurseur]) != string::npos)
    {
      string sBlockContent = string(sFormula, iBlockStart, iCurseur - iBlockStart) ;
      strip(sBlockContent, stripBoth) ;
      if ((string("") != sBlockContent) && (false == paBlocks->contains(sBlockContent)))
        paBlocks->push_back(new string(sBlockContent)) ;

      iCurseur++ ;

      // Looking for the starting char of next block (not a separator)
      //
      while ((iCurseur < strlen(sFormula.c_str())) &&
             (sLogicalOperators.find(sFormula[iCurseur]) != string::npos))
        iCurseur++ ;

      iBlockStart = iCurseur ;
    }
    else
      iCurseur++ ;
  }

  string sBlockContent = string(sFormula, iBlockStart, iCurseur - iBlockStart) ;
  strip(sBlockContent, stripBoth) ;
  if ((string("") != sBlockContent) && (false == paBlocks->contains(sBlockContent)))
    paBlocks->push_back(new string(sBlockContent)) ;
*/
}

// Parse formula into "LValue Operator RValue"
//
// if bSingleBlockAccepted is true, it means that a contiguous block is
// accepted (assuming that the implicit operator is "exists")
//
bool
parseComparisonBlock(string sFormula, string &sLValue, string &sOperator, string &sRValue, string *psErrorMsg, bool bSingleBlockAccepted)
{
  // Blocks initialization
  //
  sLValue   = string("") ;
  sOperator = string("") ;
  sRValue   = string("") ;

  // Is sentence empty?
  //
  strip(sFormula, string(" \t"), stripBoth) ;

  if (string("") == sFormula)
  {
    if (psErrorMsg)
      *psErrorMsg = string("emptyComparisonBlock") ;

    return false ;
  }

  size_t cc   = 0 ;

  // looking for a comparison operator that separates LValue and RValue
  //
  size_t iSize = strlen(sFormula.c_str()) ;

  if (cc == iSize)
  {    if (psErrorMsg)      *psErrorMsg = string("emptyComparisonBlock") ;    return false ;  }
  bool bExistsBlanck = false ;

  string sOper = string("") ;
  size_t iOpNb = 0 ;

  while (cc < iSize)
  {
    bool bAfterBlank = false ;

    // Skip blanks and tabs
    //
    while ((cc < iSize) && ((' ' == sFormula[cc]) || ('\t' == sFormula[cc])))    {      cc++ ;
      bAfterBlank   = true ;
      bExistsBlanck = true ;
    }

    // Checking current operand    //    if (cc < iSize)
    {
      if (bAfterBlank && ('*' == sFormula[cc]))
      {
        size_t iOperatorStart = cc ;

        sOper = string("") ;
        while ((cc < iSize) && (' ' != sFormula[cc]) && ('\t' != sFormula[cc]))
        {
          sOper += string(1, sFormula[cc]) ;
          cc++ ;
        }
        if (isComparisonOperator(sOper))
        {
          iOpNb++ ;

          if (iOpNb > 1)
          {
            if (psErrorMsg)
              *psErrorMsg = string("emptyComparisonBlock") ;            return false ;
          }

          if (cc >= iSize)
          {
            if (psErrorMsg)
              *psErrorMsg = string("noRvalueInComparisonBlock") ;
            return false ;
          }

          sLValue   = string(sFormula, 0, iOperatorStart) ;
          strip(sLValue, string(" \t"), stripBoth) ;

          if (string("") == sLValue)
          {
            if (psErrorMsg)
              *psErrorMsg = string("noLvalueInComparisonBlock") ;
            return false ;
          }

          sOperator = sOper ;

          sRValue   = string(sFormula, cc, iSize - cc) ;
          strip(sRValue, string(" \t"), stripBoth) ;

          if (string("") == sRValue)
          {
            if (psErrorMsg)
              *psErrorMsg = string("noRvalueInComparisonBlock") ;
            return false ;
          }
        }
      }
      else
        cc++ ;
    }
  }

  if (string("") == sOperator)
  {
    if ((false == bExistsBlanck) && bSingleBlockAccepted)
    {
      sLValue = sFormula ;
      return true ;
    }

    if (psErrorMsg)
      *psErrorMsg = string("noComparisonOperatorFound") ;
    return false ;
  }

  return true ;
}

int
evaluate(map<string, bool> *pDatas, string sFormula, size_t& cc)
{
  if (string("") == sFormula)
    return true ;

  if (((map<string, bool>*) NULL == pDatas) || pDatas->empty())
    return false ;

  string sOperators = string("!&|") ;
  string sBlanks    = string(" \t\r\n") ;

  int result = 1 ;
  int result1 ;
  char oper = ' ' ;
  bool bNegation ;

  while (cc < strlen(sFormula.c_str()))  {
    result1 = 1 ;
    bNegation = false ;

    // Skip blanks
    //
    while ((cc < strlen(sFormula.c_str())) && (string::npos != sBlanks.find(sFormula[cc])))      cc++ ;

    // on �value l'op�rande en cours    if (cc < strlen(sFormula.c_str()))
    {
      // on �value d'abord la n�gation
      if ('!' == sFormula[cc])
      {
        cc++ ;
        bNegation = true ;
      }

      if (cc == strlen(sFormula.c_str()))
        return -1 ;

      // on doit avoir ici une '(' ou un nom de contrainte
      //
      if ('(' == sFormula[cc])
      {
        cc++ ;
        result1 = evaluate(pDatas, sFormula, cc) ;
      }
      else
      {
        string sNomContrainte = "" ;

        // Getting the constraint while no blank and no operator
        //
        while ((cc < strlen(sFormula.c_str())) && (string::npos == sOperators.find(sFormula[cc]))
                                               && (string::npos == sBlanks.find(sFormula[cc]))
                                               && (')' != sFormula[cc]))
        {
          sNomContrainte += string(1, sFormula[cc]) ;
          cc++ ;
        }
        if (string("") != sNomContrainte)
          result1 = (*pDatas)[sNomContrainte] ;
        else
          return -1 ;
      }
    }

    // cas erreur � l'�valuation de l'op�rande    if (-1 == result1)
      return -1 ;
    if (bNegation)
      result1 = !result1 ;

    // on calcule le r�sultat selon l'op�rateur en cours    if      (' ' == oper)
      result = result1 ;
    else if ('|' == oper)
      result = result || result1 ;
    else if ('&' == oper)
      result = result && result1 ;

    // on avance � nouveau    while ((cc < strlen(sFormula.c_str())) && (' ' == sFormula[cc]))
      cc++ ;

    // on �value l'op�rateur    if (cc < strlen(sFormula.c_str()))
    {
      // on doit avoir ici une ')' ou un op�rateur
      if (('|' == sFormula[cc]) || ('&' == sFormula[cc]))
      {
        oper = sFormula[cc] ;
        cc++ ;
      }
      else if (')' == sFormula[cc])
      {
        cc++ ;
        return result ;
      }
      else
        return -1 ;
    }
  }
  return result ;
}

bool
compare(double dLValue, string sOperator, double dRValue)
{
  if (string("*!=") == sOperator)
    return (dLValue != dRValue) ;
  if (string("*<")  == sOperator)
    return (dLValue < dRValue) ;
  if (string("*<=") == sOperator)
    return (dLValue <= dRValue) ;
  if (string("*==") == sOperator)
    return (dLValue == dRValue) ;
  if (string("*>")  == sOperator)
    return (dLValue > dRValue) ;
  if (string("*>=") == sOperator)
    return (dLValue >= dRValue) ;

  return false ;
}

double
calculate(map<string, double> *pDatas, string sFormula)
{
  if (string("") == sFormula)
    return 0 ;

  // 100 niveaux de piles
  //
	double val[100] ;
	string ope[100] ;

	int	   nbNiv = 100 ;

	for (int j = 0 ; j < nbNiv ; j++)
	{
		val[j] = 0 ;
		ope[j] = string("") ;
	}

  string sCurrentVar = string("") ;

	size_t i = 0 ;
	int    pileur = 0 ;
	bool   tourner = true ;
	while (tourner)
	{
    switch(sFormula[i])
    {
      case '(' : // Parenth�se ouvrante : on empile
      {
    	  for (int j = nbNiv - 1; j > pileur; j--)
        {
      	  val[j] = val[j-1] ;
          ope[j] = ope[j-1] ;
        }
        val[pileur] = 0 ;
        ope[pileur] = string("") ;
        i++ ;

        break ;
      }
      case '+' : // Op�rateurs + - * / ^x, etc
      case '-' :
      case '*' :
      case '/' :
      case '^' : // power  example 10^var
      case '�' : // log    example 10�var -> log10(var)

        // Processing sCurrentVar
        //
        if (string("") != sCurrentVar)
        {
          if (((map<string, double>*) NULL == pDatas) || (pDatas->empty()))
            return 0 ;

          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (string("") != ope[pileur])
      	    val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
          else
      	    val[pileur] = dValue ;
        }

    	  ope[pileur] = string(1, sFormula[i]) ;
        i++ ;
        break ;

      case ')' : // Parenth�se fermante : on d�pile
      {
        // Processing sCurrentVar
        //
        if (string("") != sCurrentVar)
        {
          if (((map<string, double>*) NULL == pDatas) || (pDatas->empty()))
            return 0 ;

          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (string("") != ope[pileur])
      	    val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
          else
      	    val[pileur] = dValue ;
        }

    	  while (string("") != ope[pileur+1])
        {
      	  val[pileur] = operation(val[pileur+1], val[pileur], ope[pileur+1]) ;
          for (int j = pileur+1; j < nbNiv-1; j++)
          {
        	  val[j] = val[j+1] ;
            ope[j] = ope[j+1] ;
          }
          val[nbNiv-1] = 0 ;
          ope[nbNiv-1] = string("") ;
        }
        i++ ;

        break ;
      }

      case '0' : // Digits ; if not part of a var name, get the whole number
      case '1' :
      case '2' :
      case '3' :
      case '4' :
      case '5' :
      case '6' :
      case '7' :
      case '8' :
      case '9' :
      {
        if (string("") != sCurrentVar)
        {
          sCurrentVar += string(1, sFormula[i]) ;
          i++ ;
        }
        else
        {
          string sNbre = string(1, sFormula[i]) ;
          i++ ;
          while ((i < strlen(sFormula.c_str())) &&
                (((sFormula[i] >= '0') && (sFormula[i] <= '9')) ||
                      ('.' == sFormula[i])))
          {
            sNbre += string(1, sFormula[i]) ;
            i++ ;
          }
          double dNbre = StringToDouble(sNbre) ;
          if (string("") != ope[pileur])
            val[pileur] = operation(val[pileur], dNbre, ope[pileur]) ;
          else
            val[pileur] = dNbre ;
        }

        break ;
      }

      case ' ' :
      {
        if (string("") != sCurrentVar)
        {
          if (((map<string, double>*) NULL == pDatas) || (pDatas->empty()))
            return 0 ;

          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (string("") != ope[pileur])
      	    val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
          else
      	    val[pileur] = dValue ;
        }
        i++ ;

        break ;
      }

      default :

        sCurrentVar += string(1, sFormula[i]) ;
        i++ ;
    }

    if (i >= strlen(sFormula.c_str()))
    	tourner = false ;
	}

  if (string("") != sCurrentVar)
  {
    if (((map<string, double>*) NULL == pDatas) || (pDatas->empty()))
      return 0 ;

    double dValue = (*pDatas)[sCurrentVar] ;
    sCurrentVar = string("") ;

    if (string("") != ope[pileur])
      val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
    else
      val[pileur] = dValue ;
  }

	return val[pileur] ;
}

double
operation(double dOperande, double dOperateur, string sOperation)
{
	if (string("+") == sOperation)
		return dOperande + dOperateur ;
	if (string("-") == sOperation)
		return dOperande - dOperateur ;
	if (string("*") == sOperation)
		return dOperande * dOperateur ;
	if ((string("/") == sOperation) && (0 != dOperateur))
		return dOperande / dOperateur ;
	if (string("^") == sOperation)
		return pow(dOperande, dOperateur) ;
  if (string("�") == sOperation)
  {
    if (double(10) == dOperande)
      return log10(dOperateur) ;
  }
	return 0 ;
}

// Does this string constitute a logical sentence?
//
bool
isLogicalBlock(string sBlock)
{
  if (string("") == sBlock)
    return false ;

  // Chars & and | are dedicated logical operators
  //
  if (sBlock.find('|') != string::npos)
    return true ;
  if (sBlock.find('&') != string::npos)
    return true ;

  // Char ! is a logical operator when first char
  //
  size_t iNegPos = sBlock.find('!') ;
  if (string::npos == iNegPos)
    return false ;

  if (0 == iNegPos)
    return true ;

  while (string::npos != iNegPos)
  {
    if ((' ' == sBlock[iNegPos - 1]) || ('(' == sBlock[iNegPos - 1]))
      return true ;

    iNegPos = sBlock.find('!', iNegPos) ;
  }

  return false ;
}

bool
isComparisonOperator(string sCandidateOperator)
{
  if ((string("*!=") == sCandidateOperator) ||
      (string("*<")  == sCandidateOperator) ||
      (string("*<=") == sCandidateOperator) ||
      (string("*==") == sCandidateOperator) ||
      (string("*>")  == sCandidateOperator) ||
      (string("*>=") == sCandidateOperator))
    return true ;

  return false ;
}

string
getComparisonUnit(string sLValue, string sRValue)
{
  // First, check if RValue is of the kind 35[2KGM2] (only value[unit])
  //
  strip(sRValue, string(" \t"), stripBoth) ;

  // Is it a monolithic block
  //
  if ((sRValue.find(' ') == string::npos) && (sRValue.find('\t') == string::npos))
    return getPathUnit(sRValue) ;

  return string("") ;
}

string
getPathUnit(string sPath)
{
  if (string("") == sPath)
    return string("") ;

  // First case, desired unit is explicitly mentionned inside []
  //
  size_t posStartUnit = sPath.find("[") ;
  if (string::npos != posStartUnit)  {    size_t posEndUnit = sPath.find("]", posStartUnit + 1) ;    if (string::npos != posEndUnit)
      return string(sPath, posStartUnit + 1, posEndUnit - posStartUnit - 1) ;
  }

  return string("") ;
}

string
replaceVarByAlias(string *psFormula, map<string, string> *paAliases, size_t iVarStartPos, string sVar, string sNameInit)
{
  if (((string*) NULL == psFormula) || (string("") == *psFormula) || (string("") == sVar))
    return string("") ;

  string sAlias = string("") ;

  // Elaborate a "cleaned var"
  //
  string sCleanedVar = sVar ;

  // Remove trailing blanks
  strip(sCleanedVar, stripBoth) ;

  // remove useless parenthesis
  while (('(' == sCleanedVar[0]) && (')' == sCleanedVar[strlen(sCleanedVar.c_str())-1]))
  {
    sCleanedVar = string(sCleanedVar, 1, strlen(sCleanedVar.c_str())-2) ;
    strip(sCleanedVar, stripBoth) ;
  }

  if (paAliases)
  {
    // Is this var already known?
    //
    map<string, string>::iterator cur = paAliases->find(sCleanedVar) ;
    if (paAliases->end() != cur)
      sAlias = (*cur).second ;
  }

  // unknown, create a new alias
  //
  if (string("") == sAlias)
  {
    size_t iBlockNb ;
    if (paAliases)
    {
      iBlockNb = paAliases->size() ;
      // check if this alias doesn't already exist
      bool bKeepChecking = true ;
      while (bKeepChecking)
      {
        sAlias = sNameInit + IntToString(iBlockNb) ;
        map<string, string>::iterator cur = paAliases->begin() ;
        while ((paAliases->end() != cur) && ((*cur).second != sAlias))
          cur++ ;

        if (paAliases->end() != cur)
          iBlockNb++ ;
        else
          bKeepChecking = false ;
      }
      (*paAliases)[sCleanedVar] = sAlias ;
    }
    else
      sAlias = sNameInit ;
  }

  psFormula->replace(iVarStartPos, strlen(sVar.c_str()), sAlias) ;

  return sAlias ;
}

string
texteHtml(string texte)
{
  string sHtml("") ;

  for (size_t i = 0 ; i < strlen(texte.c_str()) ; i++)
  {
    switch (texte[i])
    {
      case '!'  :	sHtml = sHtml + "&#33;" ; 		break ;
      case '"'  : sHtml = sHtml + "&quot;" ;		break ;
      case '#'  :	sHtml = sHtml + "&#35;" ; 		break ;
      case '$'  : sHtml = sHtml + "&#36;" ;			break ;
      case '%'  : sHtml = sHtml + "&#37;" ;			break ;
      case '&'  : sHtml = sHtml + "&amp;" ;		  break ;
      case '\'' : sHtml = sHtml + "&#39;" ;			break ;
      case '<'  : sHtml = sHtml + "&lt;" ;			break ;
      case '>'  : sHtml = sHtml + "&gt;" ;			break ;
      case '?'  : sHtml = sHtml + "&#63;" ;			break ;
      case '@'  : sHtml = sHtml + "&#64;" ;			break ;
      case '\\' : sHtml = sHtml + "&#92;" ;			break ;
      case '|'  : sHtml = sHtml + "&#124;" ;		break ;
      case '�'  : sHtml = sHtml + "&#163;" ;		break ;
      case '�'  : sHtml = sHtml + "&euro;" ;		break ;
      case '�'  : sHtml = sHtml + "&sup2;" ;		break ;
      case '�'  :	sHtml = sHtml + "&acirc;" ;		break ;
      case '�'  : sHtml = sHtml + "&agrave;" ;	break ;
      case '�'  : sHtml = sHtml + "&aelig;" ;		break ;
      case '�'  : sHtml = sHtml + "&ccedil;" ;	break ;
      case '�'  : sHtml = sHtml + "&eacute;" ;	break ;
      case '�'  : sHtml = sHtml + "&ecirc;" ;		break ;
      case '�'  : sHtml = sHtml + "&egrave;" ;	break ;
      case '�'  : sHtml = sHtml + "&euml;" ;		break ;
      case '�'  : sHtml = sHtml + "&icirc;" ;		break ;
      case '�'  : sHtml = sHtml + "&iuml;" ;		break ;
      case '�'  : sHtml = sHtml + "&ocirc;" ;		break ;
      case '�'  : sHtml = sHtml + "&ouml;" ;		break ;
      case '�'  : sHtml = sHtml + "&#339;" ;		break ;
      case '�'  : sHtml = sHtml + "&uacute;" ;	break ;
      case '�'  : sHtml = sHtml + "&ugrave;" ;	break ;
      case '�'  : sHtml = sHtml + "&ucirc;" ;		break ;
      case '�'  : sHtml = sHtml + "&uuml;" ;		break ;
      case '�'  : sHtml = sHtml + "&Acirc;" ;		break ;      case '�'  : sHtml = sHtml + "&Agrave;" ;	break ;      case '�'  : sHtml = sHtml + "&Aelig;" ;		break ;      case '�'  : sHtml = sHtml + "&Ccedil;" ;	break ;      case '�'  : sHtml = sHtml + "&Eacute;" ;	break ;      case '�'  : sHtml = sHtml + "&Ecirc;" ;		break ;      case '�'  : sHtml = sHtml + "&Egrave;" ;	break ;      case '�'  : sHtml = sHtml + "&Euml;" ;		break ;      case '�'  : sHtml = sHtml + "&Icirc;" ;		break ;      case '�'  : sHtml = sHtml + "&Iuml;" ;		break ;      case '�'  : sHtml = sHtml + "&Ograve;" ;	break ;      case '�'  : sHtml = sHtml + "&Oacute;" ;	break ;      case '�'  : sHtml = sHtml + "&Ocirc;" ;		break ;      case '�'  : sHtml = sHtml + "&Otilde;" ;	break ;      case '�'  : sHtml = sHtml + "&Ouml;" ;		break ;      case '�'  : sHtml = sHtml + "&#338;" ;		break ;      case '�'  : sHtml = sHtml + "&Uacute;" ;	break ;      case '�'  : sHtml = sHtml + "&Ugrave;" ;	break ;      case '�'  : sHtml = sHtml + "&Ucirc;" ;		break ;      case '�'	: sHtml = sHtml + "&Uuml;" ;		break ;      case '�'	: sHtml = sHtml + "&Yacute;" ;	break ;      case '�'	: sHtml = sHtml + "&THORN;" ;	  break ;      case '�'	: sHtml = sHtml + "&szlig;" ;	  break ;      default   : sHtml = sHtml + string(1, texte[i]) ;
    }
	}
	return sHtml ;}

string
texteCourant(string textHtml)
{
	string textOut = textHtml ;
  size_t pos     = textOut.find("&") ;

  while (string::npos != pos)
  {
    size_t len = strlen(textOut.c_str()) - pos ;
    if 		  ((len >= 7) && (string(textOut, pos, 7) == "&acirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&agrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 7) == "&aelig;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&ccedil;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&eacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ecirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&egrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&euml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&icirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&iuml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ocirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&ouml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&uacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&ugrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ucirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&uuml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 4) && (string(textOut, pos, 4) == "&lt;"))
      textOut.replace(pos, 4, "<") ;
    else if ((len >= 4) && (string(textOut, pos, 4) == "&gt;"))
      textOut.replace(pos, 4, ">") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&amp;"))
      textOut.replace(pos, 6, "&") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&quot;"))
      textOut.replace(pos, 6, "\"") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&sup2;"))
      textOut.replace(pos, 6, "�") ;

    // gestion des majuscules accentu�es
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Acirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Agrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 7) == "&Aelig;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ccedil;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Eacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ecirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Egrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Euml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Icirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Iuml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ograve;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Oacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ocirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Otilde;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Ouml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Uacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ugrave;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ucirc;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Uuml;"))
      textOut.replace(pos, 6, "�") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Yacute;"))
      textOut.replace(pos, 8, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&THORN;"))
      textOut.replace(pos, 7, "�") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&szlig;"))
      textOut.replace(pos, 7, "�") ;

    else if ((len >= 6) && (string(textOut, pos, 6) == "&euro;"))
      textOut.replace(pos, 6, "�") ;

    // gestion des "&#i;" with 0 <= i <= 255
    else if ((len >= 4) && (string(textOut, pos, 2) == "&#"))
    {
      int iDigit  = 0 ;
      int iUnit   = 0 ;
      int iPos    = pos + 2 ;
      while (isdigit(textOut[iPos]) && (iDigit <= 3))
      {
        iUnit = iUnit * 10 + textOut[iPos] - '0' ;
        iDigit++ ;
        iPos++ ;
      }
      if (iUnit != 0)
      {
        char  pcReplace[5] ;
        sprintf(pcReplace, "%c", iUnit) ;
        textOut.replace(pos, 2 + iDigit + 1, pcReplace) ;
      }
    }
/*
      textOut.replace(pos, 6, "|") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&#163;"))      textOut.replace(pos, 6, "�") ;    else if ((len >= 5) && (string(textOut, pos, 5) == "&#36;"))      textOut.replace(pos, 5, "$") ;*/
    pos++ ;
    pos = textOut.find("&",pos) ;
	}
/*
    bool isdigit (charT c, const locale& loc) const;
  isdigit*/
  return textOut ;}

string
texteWebLink(string texte)
{
  string sHtml("") ;

  for (size_t i = 0 ; i < strlen(texte.c_str()) ; i++)
  {
  	if (((texte[i] >= '0') && (texte[i] <= '9')) ||
        ((texte[i] >= 'A') && (texte[i] <= 'Z')) ||
        ((texte[i] >= 'a') && (texte[i] <= 'z')))
  		sHtml += texte[i] ;
    else
    {
    	unsigned char theChar = texte[i] ;
      char szCarHex[3] ;
    	char_to_hex(theChar, szCarHex) ;
    	sHtml += string("%") + string(szCarHex) ;
    }
	}

  return sHtml ;
}

string
texteCourantFromWebLink(string textWL)
{
  string sTexte("") ;

  for (size_t i = 0 ; i < strlen(textWL.c_str()) ; i++)
  {
  	if ((textWL[i] != '%') || (i > strlen(textWL.c_str()) - 2))
  		sTexte += textWL[i] ;
    else
    {
    	unsigned char theChar ;
      char szCarHex[3] ;
      szCarHex[0] = textWL[i+1] ;
      szCarHex[1] = textWL[i+2] ;
      hex_to_char(szCarHex, &theChar) ;
    	sTexte += string(1, theChar) ;
      i += 2 ;
    }
	}

  return sTexte ;
}

void
texteForEditControl(string* pTexte)
{
  if ((string*) NULL == pTexte)
    return ;

	// Remplace les \n et les \r\n par des \r\r\n : uniquement utile pour envoyer dans un edit
  //
  size_t posCr = pTexte->find("\n") ;

  while (string::npos != posCr)
  {
  	int iNumberOfAddedChar = 0 ;

  	if (0 == posCr)
    {
    	*pTexte = string("\r\r") + *pTexte ;
      iNumberOfAddedChar = 2 ;
    }
    else
    {
    	if ('\r' != (*pTexte)[posCr-1])
      {
      	*pTexte = string(*pTexte, 0, posCr) + string(2, '\r') + string(*pTexte, posCr, strlen(pTexte->c_str()) - posCr) ;
        iNumberOfAddedChar = 2 ;
      }
      else if (1 == posCr)
      {
      	*pTexte = string("\r") + *pTexte ;
        iNumberOfAddedChar = 1 ;
      }
      else if ('\r' != (*pTexte)[posCr-2])
      {
      	*pTexte = string(*pTexte, 0, posCr) + string(1, '\r') + string(*pTexte, posCr, strlen(pTexte->c_str()) - posCr) ;
        iNumberOfAddedChar = 1 ;
      }
    }

    posCr = pTexte->find("\n", posCr + iNumberOfAddedChar + 1) ;
  }
}

string
nomSansDoublons(string sPathRef, string sName, string sExtension)
{
  string sNomComplet = string("") ;

  size_t cpt    = 0 ;
  bool   bExist = true ;

  while (bExist)
  {
    char suff[4] ;
    sprintf(suff, "%03d", cpt) ;

    sNomComplet = sName + string("_") + string(suff) ;

    string sFullFileName = sPathRef + sNomComplet + string(".") + sExtension ;
    bExist = NsFileExists(sFullFileName) ;

    if (bExist)
    {
      cpt++ ;
      if (cpt == 1000) // limite du nombre de doublons
      {
        string sErrorMsg = string("Vous avez atteint la limite du nombre de doublons pour les fichiers du type ") + sName + string("*.") + sExtension + string(" dans le r�pertoire \"") + sPathRef + string("\"") ;
        erreur(sErrorMsg.c_str(), standardError, 0) ;
        sNomComplet = string("") ;
        bExist = false ;
      }
    }
  }

  if (string("") != sNomComplet)
    sNomComplet = sNomComplet + string(".") + sExtension ;

  return sNomComplet ;
}
// -----------------------------------------------------------------------------//// classe ParamArray//// -----------------------------------------------------------------------------ParamArray::ParamArray(){}
ParamArray::~ParamArray()
{
	vider() ;
}

ParamArray::ParamArray(const ParamArray& src)
{
  vector<string*>::const_iterator i ;

  if (false == src.aNomParam.empty())
  	for (i = src.aNomParam.begin() ; src.aNomParam.end() != i ; i++)
  		aNomParam.push_back(new string(*(*i))) ;

  if (false == src.aValParam.empty())
  	for (i = src.aValParam.begin() ; src.aValParam.end() != i ; i++)
  		aValParam.push_back(new string(*(*i))) ;
}

ParamArray&
ParamArray::operator=(const ParamArray& src)
{
	if (this == &src)
		return (*this) ;

  vector<string*>::const_iterator i ;

  if (false == src.aNomParam.empty())
  	for (i = src.aNomParam.begin() ; src.aNomParam.end() != i ; i++)
  		aNomParam.push_back(new string(*(*i))) ;

  if (false == src.aValParam.empty())
  	for (i = src.aValParam.begin() ; src.aValParam.end() != i ; i++)
  		aValParam.push_back(new string(*(*i))) ;

  return (*this) ;
}

void
ParamArray::vider()
{
  vector<string*>::iterator i ;

  if (false == aNomParam.empty())
    for (i = aNomParam.begin() ; aNomParam.end() != i ; )
    {
      delete (*i) ;
      aNomParam.erase(i) ;
    }

  if (false == aValParam.empty())
    for (i = aValParam.begin() ; aValParam.end() != i ; )
    {
      delete (*i) ;
      aValParam.erase(i) ;
    }
}

void
ParamArray::PushParam(string sNom, string sVal)
{
  aNomParam.push_back(new string(sNom)) ;
  aValParam.push_back(new string(sVal)) ;
}

stringParamArray::ValParam(const string sParam) const{  if (aNomParam.empty() || aValParam.empty())    return string("") ;  vector<string*>::const_iterator iName = aNomParam.begin() ;  vector<string*>::const_iterator iVal  = aValParam.begin() ;  string sMajParam = pseumaj(sParam) ;  for ( ; (aNomParam.end() != iName) && (aValParam.end() != iVal) ; iVal++, iName++)    if (pseumaj(**iName) == sMajParam)      return **iVal ;  return string("") ;}intParamArray::operator==(const ParamArray& o) const{  // If one is empty, then both must be empty  //  if (aNomParam.empty() || o.aNomParam.empty())    return (aNomParam.empty() && o.aNomParam.empty()) ;  if (aValParam.empty() || o.aValParam.empty())    return (aValParam.empty() && o.aValParam.empty()) ;  // Must have same size  //  if ((aNomParam.size() != o.aNomParam.size()) ||      (aValParam.size() != o.aValParam.size()))    return 0 ;  // Compare one to one  //  vector<string*>::const_iterator it      = aNomParam.begin() ;  vector<string*>::const_iterator otherIt = o.aNomParam.begin() ;  for ( ; (aNomParam.end() != it) && (o.aNomParam.end() != otherIt) ; it++, otherIt++)    if (**it != **otherIt)      return 0 ;  it      = aValParam.begin() ;  otherIt = o.aValParam.begin() ;  for ( ; (aValParam.end() != it) && (o.aValParam.end() != otherIt) ; it++, otherIt++)    if (**it != **otherIt)      return 0 ;  return 1 ;}//  Get data from user specific parameters file, if not successful, try generic//stringgetParams(const string sUser, const string sFichierDat, string* pFileName){  if (string("") == sFichierDat)  {    string sMsg = string("Erreur : Le fichier de param�tres n'est pas sp�cifi�.") ;
    erreur(sMsg.c_str(), standardError, 0) ;
    return string("") ;
  }  size_t pos = sFichierDat.find_last_of(".") ;  if (pos == string::npos)
  {
    string sMsg = string("Erreur : Le fichier de param�tres n'a pas d'extension : ") + sFichierDat ;
    erreur(sMsg.c_str(), standardError, 0) ;
    return string("") ;
  }

  string sName = string(sFichierDat, 0, pos) ;
  string sExt  = string(sFichierDat, pos, strlen(sFichierDat.c_str()) - pos) ;

  string sFileForUser = sName + sUser + sExt ;

  ifstream inFile ;

  // Try to open parameters file for specified user
  //
  inFile.open(sFileForUser.c_str()) ;
  if (!inFile)
  {
    // If not successful, try to open generic file
    //
    inFile.open(sFichierDat.c_str()) ;    if (!inFile)    {      string sMsg = string("Erreur d'ouverture du fichier ") + sFichierDat ;      erreur(sMsg.c_str(), standardError, 0) ;      return string("") ;    }    else if (pFileName)      *pFileName = sFichierDat ;  }  else if (pFileName)    *pFileName = sFileForUser ;
  string sData = string("") ;

  while (!inFile.eof())  {    string sLine = string("") ;    getline(inFile, sLine) ;    if (string("") != sLine)      sData += sLine + string("\n") ;  }
  inFile.close() ;  return sData ;}//  Fill parameters array from user specific parameters file (or generic if no specific one)//boollireParam(const string sUser, const string sFichierDat, ParamArray *pParamArray){  if ((ParamArray*) NULL == pParamArray)    return false ;  pParamArray->vider() ;

  // Get information from file
  //
  string sData = getParams(sUser, sFichierDat) ;

  if (string("") == sData)
    return false ;

  // Parsing lines  //  size_t i = 0 ;
  while (i < strlen(sData.c_str()))
  {    string sNomAttrib = string("") ;    string sValAttrib = string("") ;    // Line starting with "//" is a comment; skip to next line    //    if (('/' == sData[i]) && (((i + 1) < strlen(sData.c_str())) && ('/' == sData[i + 1])))    {      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;      i++ ;    }    else    {      // Fill attribute name until a blank or a tab is encountered      //      while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))
        sNomAttrib += sData[i++] ;
      // Skip separating blanks and tabs
      //
      while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))        i++ ;
      // Fill attribute value until a blank or a tab or nl is encountered
      //
      while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))        sValAttrib += sData[i++] ;
      // Goto nl
      //
      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;
      i++ ;
      pParamArray->PushParam(sNomAttrib, sValAttrib) ;
    }
  }

  return true ;
}// Get a parameter value from user specific parameters file (or generic if no specific one)//boollireParam(const string sUser, const string sFichierDat, const string sParam, string& sResult)
{
  sResult = string("") ;

  if (string("") == sParam)
    return false ;

  // Get information from file
  //
  string sData = getParams(sUser, sFichierDat) ;

  if (string("") == sData)
    return false ;

  string sMajParam = pseumaj(sParam) ;

  // Parsing lines
  //
  size_t i = 0 ; 	while (i < strlen(sData.c_str())) 	{    string sNomAttrib = string("") ;    string sValAttrib = string("") ;    // Line starting with "//" is a comment; skip to next line    //    if (('/' == sData[i]) && (((i + 1) < strlen(sData.c_str())) && ('/' == sData[i + 1])))    {      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;      i++ ;    }    else    {      // Fill attribute name until a blank or a tab is encountered
      //      while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))
        sNomAttrib += sData[i++] ;      // Processing params value only if it is the param we are looking for      //
      if (pseumaj(sNomAttrib) == sMajParam)
      {
        // Skip separating blanks and tabs
        //
        while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))          i++ ;
        // Fill attribute value until a blank or a tab or nl is encountered
        //
        while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))          sValAttrib += sData[i++] ;        sResult = sValAttrib ;        return true ;      }

      // Goto nl
      //
      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;
      i++ ;    }
  }

  return false ;
}

// Replace a parameter value from user specific parameters file (or generic if no specific one)
//
bool
ecrireParam(const string sUser, const string sFichierDat, const string sParam, const string sValeur)
{
  // Get information from file
  //
  string sValidFile = string("") ;

  string sData = getParams(sUser, sFichierDat, &sValidFile) ;

  if ((string("") == sData) || (string("") == sValidFile))
    return false ;

  string sMajParam = pseumaj(sParam) ;

  // Parse lines to find parameter to be modified
  //
  size_t iPos       = 0 ;
  string sValAttrib = string("") ;

  size_t i    = 0 ;  while ((i < strlen(sData.c_str())) && (0 == iPos))  {    string sNomAttrib = string("") ;    // Line starting with "//" is a comment; skip to next line    //    if (('/' == sData[i]) && (((i + 1) < strlen(sData.c_str())) && ('/' == sData[i + 1])))    {      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;      i++ ;    }    else    {      // Fill attribute name until a blank or a tab is encountered      //      while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))
        sNomAttrib += sData[i++] ;      // Processing params value only if it is the param we are looking for      //
      if (pseumaj(sNomAttrib) == sMajParam)
      {        // Skip separating blanks and tabs        //
        while ((i < strlen(sData.c_str())) && ((' ' == sData[i]) || ('\t' == sData[i])))          i++ ;
        // Remembering attribute starting location
        //
        iPos = i ;

        // Fill attribute value until a blank or a tab or nl is encountered
        //
        sValAttrib = string("") ;
        while ((i < strlen(sData.c_str())) && (' ' != sData[i]) && ('\t' != sData[i]) && ('\n' != sData[i]))          sValAttrib += sData[i++] ;
      }

      // Goto nl      //
      while ((i < strlen(sData.c_str())) && ('\n' != sData[i]))        i++ ;
      i++ ;    }
  }

  if (0 == iPos)
    return false ;

  // Replacing value
  //
  sData.replace(iPos, strlen(sValAttrib.c_str()), sValeur) ;

  // Writing change to disk
  //
  ofstream outFile ;  outFile.open(sValidFile.c_str()) ;  if (!outFile)  {    string sMsg = string("Impossible de r��crire le fichier de param�tres ") + sValidFile ;    erreur(sMsg.c_str(), standardError, 0) ;    return false ;  }
  outFile << sData ;  outFile.close() ;

  return true ;
}

// -----------------------------------------------------------------------------
// Fonction qui parse une string pour r�cup�rer les coordonn�es d'une fenetre
// -----------------------------------------------------------------------------
void
getWindowPosit(string sPosition, int& X, int& Y, int& W, int& H, string* psTaille)
{
  if (string("") == sPosition)
    return ;

  size_t  debut = 0 ;  size_t  positVide = sPosition.find("|") ;  //cha�ne vide  int     nbCoords = 0 ;
  vector<string> Coordonnees ;  //contient les coordonn�es sous forme de string
  while (string::npos != positVide)  {    Coordonnees.push_back(string(sPosition, debut, positVide - debut)) ;    nbCoords++ ;    debut = positVide + 1 ;    positVide = sPosition.find("|", debut) ;  }
  Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut)) ;  nbCoords++ ;
  //r�cup�rer les coordonn�es  vector<string> :: iterator i ;  i = Coordonnees.begin() ;  X = StringToDouble(*i) ;  i++ ;
  Y = StringToDouble(*i) ;
  i++ ;
  W = StringToDouble(*i) ;
  i++ ;
  H = StringToDouble(*i) ;

  // cas en icone ou plein ecran
  if ((psTaille) && (nbCoords > 4))  {    i++ ;    *psTaille = *i ;  }  else  	*psTaille = "N" ;
}

//
// http://blogs.media-tips.com/bernard.opic/2006/07/24/algorithme-de-luhn-implementation-c-plus-plus/
//
// L�algorithme de Luhn permet de calculer la cl� de contr�le d�un nombre �
// valider, ce qui s�av�re tr�s utile pour v�rifier la coh�rence de nombres tels
// que des num�ros de cartes de cr�dits.
//
// La proc�dure consiste � d�rouler les phases suivantes :
//
//   1. doubler la valeur d�un chiffre sur deux en commen�ant par le premier � gauche
//   2. calculer la somme des chiffres composant la s�quence ainsi obtenue (pas des nombres : 14 -> 1+4)
//   3. compl�menter � 10 le chiffre des unit�s de la somme pour obtenir la cl�
//
// Prenons par exemple, une carte de cr�dit dont le num�ro est 497010000030052
// et calculons sa cl�.
//
// Phases 	4  9  7  0  1  0  0  0  0  0  3  0  0  5  2
//      1 	8  9 14  0  2  0  0  0  0  0  6  0  0  5  4
//      2 	8+9+1+4+0+2+0+0+0+0+0+6+0+0+5+4 = 3 9
//      3                                       1
//
// On compl�te ensuite le num�ro de la carte de cr�dit avec la cl�,
// ce qui donne le num�ro 4970100000300521.

bool luhn(char* l)
{
  if ((char*) NULL == l)
    return 0 ;

  int s = 0 ;
  int i = strlen(l) ;
  int v ;

  while ( i-- )
    v = *l++ - '0' << i % 2,
    s += v - 9 * ( v > 9 ) ;

  return s % 10 < 1 ;
}

// This function compares text strings, one of which can have wildcards ('*').
//
// From http://www.ddj.com/cpp/210200888?pgno=2
//
BOOL GeneralTextCompare(
        char * pTameText,             // A string without wildcards
        char * pWildText,             // A (potentially) corresponding string with wildcards
        BOOL bCaseSensitive = FALSE,  // By default, match on 'X' vs 'x'
        char cAltTerminator = '\0'    // For function names, for example, you can stop at the first '('
        )
{
  BOOL bMatch = TRUE ;
  char * pAfterLastWild = NULL ; // The location after the last '*', if we've encountered one
  char * pAfterLastTame = NULL ; // The location in the tame string, from which we started after last wildcard
  char t, w ;

  // Walk the text strings one character at a time.
  //
  while (1)
  {
    t = *pTameText ;
    w = *pWildText ;

    // How do you match a unique text string?
    if (!t || t == cAltTerminator)
    {
      // Easy: unique up on it!
      if (!w || w == cAltTerminator)
      {
        break;               // "x" matches "x"
      }
      else if (w == '*')
      {
        pWildText++ ;
        continue ;           // "x*" matches "x" or "xy"
      }
      else if (pAfterLastTame)
      {
        if (!(*pAfterLastTame) || *pAfterLastTame == cAltTerminator)
        {
          bMatch = FALSE ;
          break ;
        }
        pTameText = pAfterLastTame++ ;
        pWildText = pAfterLastWild ;
        continue ;
      }

      bMatch = FALSE ;
      break ;                // "x" doesn't match "xy"
    }
    else
    {
      if (!bCaseSensitive)
      {
        // Lowercase the characters to be compared.
        //
        if (t >= 'A' && t <= 'Z')
        {
          t += ('a' - 'A') ;
        }

        if (w >= 'A' && w <= 'Z')
        {
          w += ('a' - 'A') ;
        }
      }

      // How do you match a tame text string?
      //
      if (t != w)
      {
        // The tame way: unique up on it!
        //
        if (w == '*')
        {
          pAfterLastWild = ++pWildText ;
          pAfterLastTame = pTameText ;
          w = *pWildText ;

          if (!w || w == cAltTerminator)
          {
            break ;          // "*" matches "x"
          }
          continue ;         // "*y" matches "xy"
        }
        else if (pAfterLastWild)
        {
          if (pAfterLastWild != pWildText)
          {
            pWildText = pAfterLastWild ;
            w = *pWildText ;
            if (!bCaseSensitive && w >= 'A' && w <= 'Z')
            {
              w += ('a' - 'A') ;
            }

            if (t == w)
            {
              pWildText++ ;
            }
          }
          pTameText++ ;
          continue ;         // "*sip*" matches "mississippi"
        }
        else
        {
          bMatch = FALSE ;
          break ;            // "x" doesn't match "y"
        }
      }
    }

    pTameText++ ;
    pWildText++ ;
  }
  return bMatch ;
}

WORD
getVirtualKeyFromString(string sVK)
{
  if ((strlen(sVK.c_str()) < 4) && (string("VK_") != string(sVK, 0, 3)))
    return WORD(0x00) ;

  if (string("VK_LBUTTON")             == sVK) return WORD(0x01) ; // Left mouse button
  if (string("VK_RBUTTON")             == sVK) return WORD(0x02) ; // Right mouse button
  if (string("VK_CANCEL")              == sVK) return WORD(0x03) ; // Control-break processing
  if (string("VK_MBUTTON")             == sVK) return WORD(0x04) ; // Middle mouse button (three-button mouse)
  if (string("VK_XBUTTON1")            == sVK) return WORD(0x05) ; // Windows 2000/XP: X1 mouse button
  if (string("VK_XBUTTON2")            == sVK) return WORD(0x06) ; // Windows 2000/XP: X2 mouse button
  if (string("VK_BACK")                == sVK) return WORD(0x08) ; // BACKSPACE key
  if (string("VK_TAB")                 == sVK) return WORD(0x09) ; // TAB key
  if (string("VK_CLEAR")               == sVK) return WORD(0x0C) ; // CLEAR key
  if (string("VK_RETURN")              == sVK) return WORD(0x0D) ; // ENTER key
  if (string("VK_SHIFT")               == sVK) return WORD(0x10) ; // SHIFT key
  if (string("VK_CONTROL")             == sVK) return WORD(0x11) ; // CTRL key
  if (string("VK_MENU")                == sVK) return WORD(0x12) ; // ALT key
  if (string("VK_PAUSE")               == sVK) return WORD(0x13) ; // PAUSE key
  if (string("VK_CAPITAL")             == sVK) return WORD(0x14) ; // CAPS LOCK key
  if (string("VK_KANA")                == sVK) return WORD(0x15) ; // Input Method Editor (IME) Kana mode
  if (string("VK_HANGUEL")             == sVK) return WORD(0x15) ; // IME Hanguel mode (maintained for compatibility; use VK_HANGUL)
  if (string("VK_HANGUL")              == sVK) return WORD(0x15) ; // IME Hangul mode
  if (string("VK_JUNJA")               == sVK) return WORD(0x17) ; // IME Junja mode
  if (string("VK_FINAL")               == sVK) return WORD(0x18) ; // IME final mode
  if (string("VK_HANJA")               == sVK) return WORD(0x19) ; // IME Hanja mode
  if (string("VK_KANJI")               == sVK) return WORD(0x19) ; // IME Kanji mode
  if (string("VK_ESCAPE")              == sVK) return WORD(0x1B) ; // ESC key
  if (string("VK_CONVERT")             == sVK) return WORD(0x1C) ; // IME convert
  if (string("VK_NONCONVERT")          == sVK) return WORD(0x1D) ; // IME nonconvert
  if (string("VK_ACCEPT")              == sVK) return WORD(0x1E) ; // IME accept
  if (string("VK_MODECHANGE")          == sVK) return WORD(0x1F) ; // IME mode change request
  if (string("VK_SPACE")               == sVK) return WORD(0x20) ; // SPACEBAR
  if (string("VK_PRIOR")               == sVK) return WORD(0x21) ; // PAGE UP key
  if (string("VK_NEXT")                == sVK) return WORD(0x22) ; // PAGE DOWN key
  if (string("VK_END")                 == sVK) return WORD(0x23) ; // END key
  if (string("VK_HOME")                == sVK) return WORD(0x24) ; // HOME key
  if (string("VK_LEFT")                == sVK) return WORD(0x25) ; // LEFT ARROW key
  if (string("VK_UP")                  == sVK) return WORD(0x26) ; // UP ARROW key
  if (string("VK_RIGHT")               == sVK) return WORD(0x27) ; // RIGHT ARROW key
  if (string("VK_DOWN")                == sVK) return WORD(0x28) ; // DOWN ARROW key
  if (string("VK_SELECT")              == sVK) return WORD(0x29) ; // SELECT key
  if (string("VK_PRINT")               == sVK) return WORD(0x2A) ; // PRINT key
  if (string("VK_EXECUTE")             == sVK) return WORD(0x2B) ; // EXECUTE key
  if (string("VK_SNAPSHOT")            == sVK) return WORD(0x2C) ; // PRINT SCREEN key
  if (string("VK_INSERT")              == sVK) return WORD(0x2D) ; // INS key
  if (string("VK_DELETE")              == sVK) return WORD(0x2E) ; // DEL key
  if (string("VK_HELP")                == sVK) return WORD(0x2F) ; // HELP key
  if (string("VK_LWIN")                == sVK) return WORD(0x5B) ; // Left Windows key (Microsoft Natural keyboard)
  if (string("VK_RWIN")                == sVK) return WORD(0x5C) ; // Right Windows key (Natural keyboard)
  if (string("VK_APPS")                == sVK) return WORD(0x5D) ; // Applications key (Natural keyboard)
  if (string("VK_SLEEP")               == sVK) return WORD(0x5F) ; // Computer Sleep key
  if (string("VK_NUMPAD0")             == sVK) return WORD(0x60) ; // Numeric keypad 0 key
  if (string("VK_NUMPAD1")             == sVK) return WORD(0x61) ; // Numeric keypad 1 key
  if (string("VK_NUMPAD2")             == sVK) return WORD(0x62) ; // Numeric keypad 2 key
  if (string("VK_NUMPAD3")             == sVK) return WORD(0x63) ; // Numeric keypad 3 key
  if (string("VK_NUMPAD4")             == sVK) return WORD(0x64) ; // Numeric keypad 4 key
  if (string("VK_NUMPAD5")             == sVK) return WORD(0x65) ; // Numeric keypad 5 key
  if (string("VK_NUMPAD6")             == sVK) return WORD(0x66) ; // Numeric keypad 6 key
  if (string("VK_NUMPAD7")             == sVK) return WORD(0x67) ; // Numeric keypad 7 key
  if (string("VK_NUMPAD8")             == sVK) return WORD(0x68) ; // Numeric keypad 8 key
  if (string("VK_NUMPAD9")             == sVK) return WORD(0x69) ; // Numeric keypad 9 key
  if (string("VK_MULTIPLY")            == sVK) return WORD(0x6A) ; // Multiply key
  if (string("VK_ADD")                 == sVK) return WORD(0x6B) ; // Add key
  if (string("VK_SEPARATOR")           == sVK) return WORD(0x6C) ; // Separator key
  if (string("VK_SUBTRACT")            == sVK) return WORD(0x6D) ; // Subtract key
  if (string("VK_DECIMAL")             == sVK) return WORD(0x6E) ; // Decimal key
  if (string("VK_DIVIDE")              == sVK) return WORD(0x6F) ; // Divide key
  if (string("VK_F1")                  == sVK) return WORD(0x70) ; // F1 key
  if (string("VK_F2")                  == sVK) return WORD(0x71) ; // F2 key
  if (string("VK_F3")                  == sVK) return WORD(0x72) ; // F3 key
  if (string("VK_F4")                  == sVK) return WORD(0x73) ; // F4 key
  if (string("VK_F5")                  == sVK) return WORD(0x74) ; // F5 key
  if (string("VK_F6")                  == sVK) return WORD(0x75) ; // F6 key
  if (string("VK_F7")                  == sVK) return WORD(0x76) ; // F7 key
  if (string("VK_F8")                  == sVK) return WORD(0x77) ; // F8 key
  if (string("VK_F9")                  == sVK) return WORD(0x78) ; // F9 key
  if (string("VK_F10")                 == sVK) return WORD(0x79) ; // F10 key
  if (string("VK_F11")                 == sVK) return WORD(0x7A) ; // F11 key
  if (string("VK_F12")                 == sVK) return WORD(0x7B) ; // F12 key
  if (string("VK_F13")                 == sVK) return WORD(0x7C) ; // F13 key
  if (string("VK_F14")                 == sVK) return WORD(0x7D) ; // F14 key
  if (string("VK_F15")                 == sVK) return WORD(0x7E) ; // F15 key
  if (string("VK_F16")                 == sVK) return WORD(0x7F) ; // F16 key
  if (string("VK_F17")                 == sVK) return WORD(0x80) ; // F17 key
  if (string("VK_F18")                 == sVK) return WORD(0x81) ; // F18 key
  if (string("VK_F19")                 == sVK) return WORD(0x82) ; // F19 key
  if (string("VK_F20")                 == sVK) return WORD(0x83) ; // F20 key
  if (string("VK_F21")                 == sVK) return WORD(0x84) ; // F21 key
  if (string("VK_F22")                 == sVK) return WORD(0x85) ; // F22 key
  if (string("VK_F23")                 == sVK) return WORD(0x86) ; // F23 key
  if (string("VK_F24")                 == sVK) return WORD(0x87) ; // F24 key
  if (string("VK_NUMLOCK")             == sVK) return WORD(0x90) ; // NUM LOCK key
  if (string("VK_SCROLL")              == sVK) return WORD(0x91) ; // SCROLL LOCK key
  if (string("VK_LSHIFT")              == sVK) return WORD(0xA0) ; // Left SHIFT key
  if (string("VK_RSHIFT")              == sVK) return WORD(0xA1) ; // Right SHIFT key
  if (string("VK_LCONTROL")            == sVK) return WORD(0xA2) ; // Left CONTROL key
  if (string("VK_RCONTROL")            == sVK) return WORD(0xA3) ; // Right CONTROL key
  if (string("VK_LMENU")               == sVK) return WORD(0xA4) ; // Left MENU key
  if (string("VK_RMENU")               == sVK) return WORD(0xA5) ; // Right MENU key
  if (string("VK_BROWSER_BACK")        == sVK) return WORD(0xA6) ; // Windows 2000/XP: Browser Back key
  if (string("VK_BROWSER_FORWARD")     == sVK) return WORD(0xA7) ; // Windows 2000/XP: Browser Forward key
  if (string("VK_BROWSER_REFRESH")     == sVK) return WORD(0xA8) ; // Windows 2000/XP: Browser Refresh key
  if (string("VK_BROWSER_STOP")        == sVK) return WORD(0xA9) ; // Windows 2000/XP: Browser Stop key
  if (string("VK_BROWSER_SEARCH")      == sVK) return WORD(0xAA) ; // Windows 2000/XP: Browser Search key
  if (string("VK_BROWSER_FAVORITES")   == sVK) return WORD(0xAB) ; // Windows 2000/XP: Browser Favorites key
  if (string("VK_BROWSER_HOME")        == sVK) return WORD(0xAC) ; // Windows 2000/XP: Browser Start and Home key
  if (string("VK_VOLUME_MUTE")         == sVK) return WORD(0xAD) ; // Windows 2000/XP: Volume Mute key
  if (string("VK_VOLUME_DOWN")         == sVK) return WORD(0xAE) ; // Windows 2000/XP: Volume Down key
  if (string("VK_VOLUME_UP")           == sVK) return WORD(0xAF) ; // Windows 2000/XP: Volume Up key
  if (string("VK_MEDIA_NEXT_TRACK")    == sVK) return WORD(0xB0) ; // Windows 2000/XP: Next Track key
  if (string("VK_MEDIA_PREV_TRACK")    == sVK) return WORD(0xB1) ; // Windows 2000/XP: Previous Track key
  if (string("VK_MEDIA_STOP")          == sVK) return WORD(0xB2) ; // Windows 2000/XP: Stop Media key
  if (string("VK_MEDIA_PLAY_PAUSE")    == sVK) return WORD(0xB3) ; // Windows 2000/XP: Play/Pause Media key
  if (string("VK_LAUNCH_MAIL")         == sVK) return WORD(0xB4) ; // Windows 2000/XP: Start Mail key
  if (string("VK_LAUNCH_MEDIA_SELECT") == sVK) return WORD(0xB5) ; // Windows 2000/XP: Select Media key
  if (string("VK_LAUNCH_APP1")         == sVK) return WORD(0xB6) ; // Windows 2000/XP: Start Application 1 key
  if (string("VK_LAUNCH_APP2")         == sVK) return WORD(0xB7) ; // Windows 2000/XP: Start Application 2 key
  if (string("VK_OEM_1")               == sVK) return WORD(0xBA) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ';:' key
  if (string("VK_OEM_PLUS")            == sVK) return WORD(0xBB) ; // Windows 2000/XP: For any country/region, the '+' key
  if (string("VK_OEM_COMMA")           == sVK) return WORD(0xBC) ; // Windows 2000/XP: For any country/region, the ',' key
  if (string("VK_OEM_MINUS")           == sVK) return WORD(0xBD) ; // Windows 2000/XP: For any country/region, the '-' key
  if (string("VK_OEM_PERIOD")          == sVK) return WORD(0xBE) ; // Windows 2000/XP: For any country/region, the '.' key
  if (string("VK_OEM_2")               == sVK) return WORD(0xBF) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '/?' key
  if (string("VK_OEM_3")               == sVK) return WORD(0xC0) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '`~' key
  if (string("VK_OEM_4")               == sVK) return WORD(0xDB) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '[{' key
  if (string("VK_OEM_5")               == sVK) return WORD(0xDC) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the '\|' key
  if (string("VK_OEM_6")               == sVK) return WORD(0xDD) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the ']}' key
  if (string("VK_OEM_7")               == sVK) return WORD(0xDE) ; // Used for miscellaneous characters; it can vary by keyboard. Windows 2000/XP: For the US standard keyboard, the 'single-quote/double-quote' key
  if (string("VK_OEM_8")               == sVK) return WORD(0xDF) ; // Used for miscellaneous characters; it can vary by keyboard.
  if (string("VK_OEM_102")             == sVK) return WORD(0xE2) ; // Windows 2000/XP: Either the angle bracket key or the backslash key on the RT 102-key keyboard
  if (string("VK_PROCESSKEY")          == sVK) return WORD(0xE5) ; // Windows 95/98/Me, Windows NT 4.0, Windows 2000/XP: IME PROCESS key
  if (string("VK_PACKET")              == sVK) return WORD(0xE7) ; // Windows 2000/XP: Used to pass Unicode characters as if they were keystrokes. The VK_PACKET key is the low word of a 32-bit Virtual Key value used for non-keyboard input methods. For more information, see Remark in KEYBDINPUT, SendInput, WM_KEYDOWN, and WM_KEYUP
  if (string("VK_ATTN")                == sVK) return WORD(0xF6) ; // Attn key
  if (string("VK_CRSEL")               == sVK) return WORD(0xF7) ; // CrSel key
  if (string("VK_EXSEL")               == sVK) return WORD(0xF8) ; // ExSel key
  if (string("VK_EREOF")               == sVK) return WORD(0xF9) ; // Erase EOF key
  if (string("VK_PLAY")                == sVK) return WORD(0xFA) ; // Play key
  if (string("VK_ZOOM")                == sVK) return WORD(0xFB) ; // Zoom key
  if (string("VK_NONAME")              == sVK) return WORD(0xFC) ; // Reserved
  if (string("VK_PA1")                 == sVK) return WORD(0xFD) ; // PA1 key
  if (string("VK_OEM_CLEAR")           == sVK) return WORD(0xFE) ; // Clear key

  return WORD(0x00) ;
}

string
getSystemLastErrorMessage()
{
  DWORD dwLastError = GetLastError() ;

  LPTSTR lpszTemp = (LPTSTR) NULL ;
  DWORD  dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
                                NULL,
                                dwLastError,
                                LANG_NEUTRAL,
                                (LPTSTR)&lpszTemp,
                                0,
                                NULL) ;

  string sErrorMessage = string("") ;

  if (lpszTemp)
  {
    if (dwRet > 0)
      sErrorMessage = string(lpszTemp) ;

    LocalFree((HLOCAL) lpszTemp) ;
  }

  return sErrorMessage ;
}

// Get image size for GIF, PNG anf JPEG
//
// From http://www.wischik.com/lu/programmer/get-image-size.html
//
bool GetImageSize(const char *fn, int *x, int *y)
{
  FILE *f = fopen(fn, "rb") ;
  if (0 == f)
    return false ;

  fseek(f, 0, SEEK_END) ;
  long len = ftell(f) ;
  fseek(f, 0, SEEK_SET) ;
  if (len < 24)
  {
    fclose(f);
    return false;
  }

  // Strategy:
  // reading GIF dimensions requires the first 10 bytes of the file
  // reading PNG dimensions requires the first 24 bytes of the file
  // reading JPEG dimensions requires scanning through jpeg chunks
  // In all formats, the file is at least 24 bytes big, so we'll read that always
  unsigned char buf[24] ;
  fread(buf, 1, 24, f) ;

  // For JPEGs, we need to read the first 12 bytes of each chunk.
  // We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
  if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF && buf[3]==0xE0 && buf[6]=='J' && buf[7]=='F' && buf[8]=='I' && buf[9]=='F')
  {
    long pos = 2 ;
    while (buf[2] == 0xFF)
    {
      if (buf[3]==0xC0 || buf[3]==0xC1 || buf[3]==0xC2 || buf[3]==0xC3 || buf[3]==0xC9 || buf[3]==0xCA || buf[3]==0xCB)
        break ;
      pos += 2 + (buf[4] << 8) + buf[5] ;
      if (pos + 12 > len)
        break ;
      fseek(f, pos, SEEK_SET) ;
      fread(buf+2, 1, 12, f) ;
    }
  }

  fclose(f) ;

  // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
  if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
  {
    *y = (buf[7]<<8) + buf[8] ;
    *x = (buf[9]<<8) + buf[10] ;
    return true ;
  }

  // GIF: first three bytes say "GIF", next three give version number. Then dimensions
  if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
  {
    *x = buf[6] + (buf[7]<<8) ;
    *y = buf[8] + (buf[9]<<8) ;
    return true ;
  }

  // PNG: the first frame is by definition an IHDR frame, which gives dimensions
  if ( buf[0]==0x89 && buf[1]=='P' && buf[2]=='N' && buf[3]=='G' && buf[4]==0x0D && buf[5]==0x0A && buf[6]==0x1A && buf[7]==0x0A
    && buf[12]=='I' && buf[13]=='H' && buf[14]=='D' && buf[15]=='R')
  {
    *x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0) ;
    *y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0) ;
    return true ;
  }

  return false ;
}

bool IsNo(string sText)
{
  string sUpperCapText = pseumaj(sText) ;

  if ((string("0")    == sText) ||
      (string("FALSE") == sUpperCapText) ||
      (string("NO")    == sUpperCapText) ||
      (string("NON")   == sUpperCapText) ||
      (string("NIET")  == sUpperCapText) ||
      (string("NEIN")  == sUpperCapText))
    return true ;

  return false ;
}

bool IsYes(string sText)
{
  string sUpperCapText = pseumaj(sText) ;

  if ((string("1")    == sText) ||
      (string("TRUE") == sUpperCapText) ||
      (string("YES")  == sUpperCapText) ||
      (string("OUI")  == sUpperCapText) ||
      (string("DA")   == sUpperCapText) ||
      (string("JA")   == sUpperCapText))
    return true ;

  return false ;
}

/**
 *
 */
string getAllCharsText(const string sTextWithNum, const char cDecimalSeparator, const char cDigitGroupSeparator, size_t iStartFrom)
{
  string sResult = sTextWithNum ;

  bool bInsideNum     = false ;
  bool bSeenDeciSepar = false ;

  size_t iNumStart     = 0 ;
  size_t iBlockLen     = 0 ;
  size_t iLastBlankPos = 0 ;

  // Detect the first number block
  //
  size_t iTxtLen   = strlen(sResult.c_str()) ;

  bool bProcessBlock = false ;

  for (int i = 0 ; i < iTxtLen ; i++)
  {
    char cCurrent = sResult[i] ;

    // Char that cannot be inside a num
    //
    if ((false == isdigit(cCurrent)) && (cCurrent != cDecimalSeparator) &&
                                        (cCurrent != cDigitGroupSeparator))
    {
      if (bInsideNum)
      {
        if (' ' == sResult[i-1])
        {
          iBlockLen = i - iNumStart - 1 ;
          bProcessBlock = true ;
        }
        else if (' ' != sResult[i])
        {
          bInsideNum     = false ;
          bSeenDeciSepar = false ;
        }
      }
    }
    // Char that is valid inside a num
    //
    else
    {
      if (false == bInsideNum)
      {
        if (isdigit(cCurrent))
        {
          bInsideNum = true ;
          iNumStart  = i ;
        }
      }
      else
      {
        if (cCurrent == cDecimalSeparator)
        {
          if (false == bSeenDeciSepar)
            bSeenDeciSepar = true ;
          else
          {
            iBlockLen = i - iNumStart ;
            bProcessBlock = true ;
          }
        }

        // if (cCurrent == cDigitGroupSeparator) TODO check the last one was 3 digits before
      }
    }

    if (bProcessBlock)
    {
      string sNum = string(sResult, iNumStart, iBlockLen) ;
      string sNumAsText = getCharText(sNum, cDecimalSeparator, cDigitGroupSeparator) ;
      if (string("") != sNumAsText)
      {
        sResult.replace(iNumStart, iBlockLen, sNumAsText) ;
        return getAllCharsText(sResult, cDecimalSeparator, cDigitGroupSeparator, iNumStart + strlen(sNumAsText.c_str())) ;
      }
      return getAllCharsText(sResult, cDecimalSeparator, cDigitGroupSeparator, iNumStart + strlen(sNum.c_str())) ;
    }
  }

  return sResult ;
}

string getOneDigitCharText(const string sNum)
{
  if (strlen(sNum.c_str()) != 1)
    return string("") ;

  if (string("1") == sNum)
    return string("un") ;
  if (string("2") == sNum)
    return string("deux") ;
  if (string("3") == sNum)
    return string("trois") ;
  if (string("4") == sNum)
    return string("quatre") ;
  if (string("5") == sNum)
    return string("cinq") ;
  if (string("6") == sNum)
    return string("six") ;
  if (string("7") == sNum)
    return string("sept") ;
  if (string("8") == sNum)
    return string("huit") ;
  if (string("9") == sNum)
    return string("neuf") ;
  return string("") ;
}

string getTwoDigitCharText(const string sNum)
{
  if (strlen(sNum.c_str()) != 2)
    return string("") ;

  // Number as a single word
  //
  if (string("10") == sNum)
    return string("dix") ;
  if (string("11") == sNum)
    return string("onze") ;
  if (string("12") == sNum)
    return string("douze") ;
  if (string("13") == sNum)
    return string("treize") ;
  if (string("14") == sNum)
    return string("quatorze") ;
  if (string("15") == sNum)
    return string("quinze") ;
  if (string("16") == sNum)
    return string("seize") ;

  // Tenth root
  //
  char cTenth = sNum[0] ;
  char cUnit  = sNum[1] ;

  // Prennent un trait d'union tous les nombres compos�s inf�rieurs � 100
  // ne se terminant pas en 1 sauf 81 et 91 :
  //
  // dix-sept, quarante-huit...
  // quatre cent cinquante-trois...
  //
  // Pour les nombres se terminant en 1, on ajoute la conjonction et :
  //
  // soixante et un
  // soixante et onze
  // cinquante et un
  //
  // 81 et 91 sont �crits avec un trait d'union:
  //
  // quatre-vingt-un
  // quatre-vingt-onze
  //
  // Recommandation orthographique de 1990 :
  // Tous les adjectifs num�raux compos�s sont syst�matiquement unis par un
  // trait d'union : trois-cent-vingt-quatre
  //
  string sReturn = string("") ;

  if      ('1' == cTenth)
    sReturn = string("dix") ;
  else if ('2' == cTenth)
    sReturn = string("vingt") ;
  else if ('3' == cTenth)
    sReturn = string("trente") ;
  else if ('4' == cTenth)
    sReturn = string("quarante") ;
  else if ('5' == cTenth)
    sReturn = string("cinquante") ;
  else if ('6' == cTenth)
    sReturn = string("soixante") ;
  else if ('8' == cTenth)
    sReturn = string("quatre-vingt") ;

  if (('0' == cUnit) && (string("") != sReturn))
    return sReturn ;

  // soixante-dix, soixante et onze, soixante-douze, etc
  //
  if ('7' == cTenth)
  {
    sReturn = string("soixante") ;
    if ('1' == cUnit)
      return sReturn + string(" et onze") ;
  }

  // quatre-vingt-dix, quatre-vingt-onze, quatre-vingt-douze, etc
  //
  if ('9' == cTenth)
    sReturn = string("quatre-vingt") ;

  if (('7' == cTenth) || ('9' == cTenth))
  {
    string sComplt = getTwoDigitCharText(string("1") + string(1, cUnit)) ;
    if (string("") == sComplt)
      return string("") ;
    return sReturn + string("-") + sComplt ;
  }

  string sComplt = getOneDigitCharText(string(1, cUnit)) ;
  if (string("") != sComplt)
  {
    // quatre-vingts
    if (('8' == cTenth) && ('0' == cUnit))
      sComplt += string("s") ;

    return sReturn + string("-") + sComplt ;
  }

  return string("") ;
}

string getThreeDigitCharText(const string sNum)
{
  if (strlen(sNum.c_str()) != 3)
    return string("") ;

  // Number as a single word
  //
  if (string("100") == sNum)
    return string("cent") ;

  string sRest = string(sNum, 1, 2) ;

  // Should we add a 's' to "cent"?
  //
  bool bAddS = false ;
  if (string("00") == sRest)
    bAddS = true ;

  // Compose the text
  //
  string sReturn = string("") ;

  char cHundredth = sNum[0] ;
  if ('1' != cHundredth)
  {
    string sHundreds = getOneDigitCharText(string(1, cHundredth)) ;
    if (string("") == sHundreds)
      return string("") ;

    sReturn = sHundreds + string(" cent") ;

    if (bAddS)
      return sReturn + string("s") ;
  }
  else
    sReturn = string("cent") ;

  string sTxtRest = getTwoDigitCharText(sRest) ;
  if (string("") == sTxtRest)
    return string("") ;

  return sReturn + string(" ") + sTxtRest ;
}

/**
 *  The text must have no separator ("1000" Ok, "1 000" not Ok)
 */
string getNDigitsCharText(const string sNum)
{
  size_t iLen = strlen(sNum.c_str()) ;

  if (1 == iLen)
    return getOneDigitCharText(sNum) ;
  if (2 == iLen)
    return getTwoDigitCharText(sNum) ;
  if (3 == iLen)
    return getThreeDigitCharText(sNum) ;

  if (string("1000") == sNum)
    return string("mille") ;

  // Less than a million: count of thousands
  //
  if (iLen < 7)
  {
    string sCount = string(sNum, 0, iLen - 3) ;
    string sRest  = string(sNum, iLen - 3, 3) ;

    string sResult = string("") ;

    string sCountTxt = string("") ;
    if (string("1") != sCount)
    {
      sCountTxt = getNDigitsCharText(sCount) ;
      if (string("") == sCountTxt)
        return string("") ;

      sResult = sCountTxt + string("-") ;
    }
    sResult += string("mille") ;

    string sRestTxt = getNDigitsCharText(sRest) ;

    if (string("") == sRestTxt)
      return sResult ;

    return sResult + string("-") + sRestTxt ;
  }

  // Less than a billion: count of millions
  //
  if (iLen < 10)
  {
    string sCount = string(sNum, 0, iLen - 6) ;
    string sRest  = string(sNum, iLen - 6, 6) ;

    string sResult = string("") ;

    string sCountTxt = string("") ;
    if (string("1") != sCount)
    {
      sCountTxt = getNDigitsCharText(sCount) ;
      if (string("") == sCountTxt)
        return string("") ;

      sResult = sCountTxt + string("-millions") ;
    }
    else
      sResult += string("un-million") ;

    string sRestTxt = getNDigitsCharText(sRest) ;

    if (string("") == sRestTxt)
      return sResult ;

    return sResult + string("-") + sRestTxt ;
  }
}

string getCharText(const string sNum, const char cDecimalSeparator, const char cDigitGroupSeparator)
{
  // Extraction de la partie enti�re et de la partie d�cimale
  //
  size_t iSepar = sNum.find(cDecimalSeparator) ;

  string sEntier = string("") ;
  string sDecima = string("") ;

  if ((string::npos == iSepar) || (iSepar > strlen(sNum.c_str())))
    sEntier = sNum ;
  else
  {
    if (iSepar > 0)
      sEntier = string(sNum, 0, iSepar) ;
    sDecima = string(sNum, iSepar + 1, strlen(sNum.c_str()) - iSepar - 1) ;
  }

  // Remove all digit group separators and blanks
  //
  for (int i = 0 ; i < strlen(sEntier.c_str()) ; i++)
    if ((cDigitGroupSeparator == sEntier[i]) || (' ' == sEntier[i]))
      sEntier = string(sEntier, 0, i) + string(sEntier, i + 1, strlen(sEntier.c_str()) - i - 1) ;

  for (int i = 0 ; i < strlen(sDecima.c_str()) ; i++)
    if ((cDigitGroupSeparator == sDecima[i]) || (' ' == sDecima[i]))
      sDecima = string(sDecima, 0, i) + string(sDecima, i + 1, strlen(sDecima.c_str()) - i - 1) ;

  string sReturn = string("") ;

  if (string("0") == sEntier)
    sReturn = string("z�ro") ;
  else
    sReturn = getNDigitsCharText(sEntier) ;

  if (string("") == sDecima)
    return sReturn ;

  string sDecimaTxt = getNDigitsCharText(sDecima) ;

  if (string("") == sDecimaTxt)
    return string("") ;

  return sReturn + string(" virgule ") + sDecimaTxt ;
}

