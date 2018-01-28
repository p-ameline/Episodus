// -----------------------------------------------------------------------------// nsdivfct.cpp
// -----------------------------------------------------------------------------
// Pot-pourri de fonctions
// -----------------------------------------------------------------------------
// $Revision: 1.12 $
// $Author: pameline $
// $Date: 2012/08/24 10:06:43 $
// -----------------------------------------------------------------------------
// PA  - 30 juin 1988
// PA  - 28 juillet 1994
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2005
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
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
static bool bModeFile           = false ;
// -----------------------------------------------------------------------------// Function    : DonneNomFichierDOS(const char *ammorce, const char* suffixe)// Arguments   : 	ammorce  : premiers caractères du préfixe du nom DOS
//				  			suffixe  : suffixe du nom de fichier DOS
//				  			chemin   : répertoire où chercher
//				  			nomLibre : nom du fichier trouvé sous forme PPPPPPPP.SSS
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

	// Préparation d'une ammorce générique du type 100001??.RTF
	tailleAmmorce = strlen(ammorce) ;
	if (tailleAmmorce > 7)
  	return 1 ;
	for (i = 0 ; i < tailleAmmorce ; i++)
		prefixe[i] = ammorce[i] ;
	for ( ; i < 8 ; i++)
		prefixe[i] = '?' ;
	prefixe[i] = '\0' ;

	// Préparation du numéro de référence
	for (i = 0 ; i < 8 - tailleAmmorce ; i++)
		numeroRef[i] = '0' ;
	numeroRef[i-1] = '1' ;
	numeroRef[i] = '\0' ;

	// Préparation du nom de fichier complet
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

	// Si le fichier n'existe pas on renvoie le numéro de référence
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

	// Traitement du fichier trouvé (récupération de son numéro)
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.name, numero) ;
	if (i == 1)
  	return (1) ;

	// Boucle tant qu'on ne trouve pas un numéro différent de la référence
	while (strcmp(numero, numeroRef) == 0)
	{
		// On incrémente la référence
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
	// On renvoie le numéro de référence
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
// Arguments   :	ammorce  : premiers caractères du préfixe du nom DOS
//				  			suffixe  : suffixe du nom de fichier DOS
//				  			chemin   : répertoire où chercher
//				  			nomLibre : nom du fichier trouvé sous forme PPPPPPPP.SSS
// Description : Cherche le premier nom de fichier libre
// Returns     : RIEN
// -----------------------------------------------------------------------------
unsigned int
DonneNomFichier(const char *ammorce, const char* suffixe, const char *chemin, char** nomLibre)
{
	int 						i, j, tailleAmmorce, calculer ;
	char 						prefixe[9], numero[9], numeroRef[9], *fichRecherche ;
	WIN32_FIND_DATA	stRecherche ;
  HANDLE					hFichierTrouve ;

	// Préparation d'une ammorce générique du type 100001??.RTF
	tailleAmmorce = strlen(ammorce) ;
	if (tailleAmmorce > 7)
  	return 1 ;
	for (i = 0 ; i < tailleAmmorce ; i++)
		prefixe[i] = ammorce[i] ;
	for ( ; i < 8 ; i++)
		prefixe[i] = '?' ;
	prefixe[i] = '\0' ;

	// Préparation du numéro de référence
	for (i = 0 ; i < 8 - tailleAmmorce ; i++)
		numeroRef[i] = '0' ;
	numeroRef[i-1] = '1' ;
	numeroRef[i] = '\0' ;

	// Préparation du nom de fichier complet
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

	// Si le fichier n'existe pas on renvoie le numéro de référence
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

	// Traitement du fichier trouvé (récupération de son numéro)
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero) ;
	if (i == 1)
  	return (1) ;

	// Boucle tant qu'on ne trouve pas un numéro différent de la référence
	while (strcmp(numero, numeroRef) == 0)
	{
		// On incrémente la référence
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
    BOOL bTrouve ;
		bTrouve = FindNextFile(hFichierTrouve, &stRecherche) ;
		if (!bTrouve)
			numero[0] = '\0';
		else
		{
			i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero) ;
			if (i != 0)
				numero[0] = '\0' ;
		}
	}

	// On renvoie le numéro de référence
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
/*** Is this element a collective, group, local or in memory object** This works with a patient ID, a tree ID or a node ID*/GRAPHELEMTYPEgetGraphElementTypeFromID(string sElementID){	if (sElementID == "")  	return isIncorrectID ;  char cFirstChar = sElementID[0] ;	if (cFirstChar == '#')  	return isMemoryID ;	if (cFirstChar == '-')  	return isLocalID ;	if (cFirstChar == '~')  	return isGroupID ;  return isCollectiveID ;}stringgetPatientIDFromID(string sAnyID){	if (strlen(sAnyID.c_str()) < 7)		return string("") ;	return string(sAnyID, 0, 7) ;}
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
          if ((temp == "£N") || (temp == "£D") || (temp == "£T"))
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
  return (result) ;}intStringToInt(string sValeur){	return (string("") != sValeur) ? atoi(sValeur.c_str()) : 0 ;}// -----------------------------------------------------------------------------// Function : Transforme un int en string// -----------------------------------------------------------------------------stringIntToString(int val){	char temp[10] ;  itoa(val, temp, 10) ;  string result(temp) ;  return result ;}
//
// To be improved
//
string
getEpisodusNumValueFromRawCapturedNum(string sRawNum, string sLang)
{
	if (sRawNum == string(""))
		return string("") ;

  string sValue = string("") ;

  string sWorkNum = sRawNum ;

	// The only allowed separator is '.'
  // If we find a ',' we replace it by a '.'

  size_t posCr = sWorkNum.find(",") ;
  if (posCr != string::npos)
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

	char  *endptr ;
	char* szTest = new char[strlen(sWorkNum.c_str())+1] ;
	strcpy(szTest, sWorkNum.c_str()) ;
	double value = strtod(szTest, &endptr) ;

  // Success
  //
  if (*endptr == '\0')
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
// Arguments   :	ammorce  			: premiers caractères du préfixe du nom DOS
//				        suffixe  			: suffixe du nom de fichier DOS
//				        nomFichier  	: nom de fichier DOS
//				        numeroFichier : numéro du fichier
// Description : Extrait le numéro du nom de fichier
//				        Exemple : ammorce = 100125 suffixe = RTF
//								nomFichier = 10012522.RTF -> numeroFichier = 22
// Returns     : 0 si OK, 1 si le nom n'est pas cohérent avec ammorce et suffixe
// -----------------------------------------------------------------------------
unsigned intDonneNumeroFichier(const char *ammorce, const char *suffixe, const char *nomFichier, char *numeroFichier){
	size_t i, j ;

	// Vérification de la conformité par rapport à l'ammorce
	for (i = 0 ; i < strlen(ammorce) ; i++)
		if (nomFichier[i] != ammorce[i])
			return(1) ;

	// Prise du numéro qui suit l'ammorce
	for (j = 0 ; (nomFichier[i] != '.') && (nomFichier[i] != '\0') ; i++, j++)
		numeroFichier[j] = nomFichier[i] ;
	numeroFichier[j] = '\0' ;

	// Vérification de la conformité par rapport au suffixe
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
// void strip(string& s, int n)// Fonction : enleve les blancs d'une string// Retour   : rien// -----------------------------------------------------------------------------voidstrip(string& s, STRIPTYPE n, char c){  if (string("") == s)    return ;  size_t pos1 = s.find_first_not_of(c) ;  if (string::npos == pos1)  {		s = "" ;    return ;  }  size_t pos2 ;  switch (n)  {    case stripBoth  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;    	pos2 = s.find_last_not_of(c) ;      if (pos2 != string::npos)      	s = string(s, 0, pos2 + 1) ;      break ;    case stripLeft  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;      break ;    case stripRight :    	pos2 = s.find_last_not_of(c) ;      if (pos2 != string::npos)      	s = string(s, 0, pos2 + 1) ;      break ;    default         :    	erreur("Fonction strip : option erronnée", standardError, 0) ;  }}void strip(string& s, string sM, STRIPTYPE n){  if ((string("") == s) || (string("") == sM))    return ;  size_t pos1 = s.find_first_not_of(sM) ;  if (string::npos == pos1)  {		s = string("") ;    return ;  }  size_t pos2 ;  switch (n)  {    case stripBoth  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;    	pos2 = s.find_last_not_of(sM) ;      if (string::npos != pos2)      	s = string(s, 0, pos2 + 1) ;      break ;    case stripLeft  :    	if (pos1 > 0)    		s = string(s, pos1, strlen(s.c_str()) - pos1) ;      break ;    case stripRight :    	pos2 = s.find_last_not_of(sM) ;      if (string::npos != pos2)      	s = string(s, 0, pos2 + 1) ;      break ;    default         :    	erreur("Fonction strip : option erronnée", standardError, 0) ;  }}void keepOnlyOne(string& s, char c){  if (string("") == s)    return ;  bool bAcceptChar = true ;  string sSearchCopy = s ;  s = string("") ;  for (size_t i = 0 ; i < strlen(sSearchCopy.c_str()) ; i++)  {    if (c == sSearchCopy[i])    {      if (bAcceptChar)      {        s += sSearchCopy[i] ;        bAcceptChar = false ;      }    }    else    {      s += sSearchCopy[i] ;      bAcceptChar = true ;    }  }}
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

  if (tourner)	// dans ce cas le code ne contient que des zéros
    code = string("1") + code ;
}

string
nom_sans_doublons(string serveur, string unite, string pathRef, string nom, string ext)
{
	string sCompteur = "";
    bool exist = true;
	string nomComplet;
  string sNomFichier;

  // Attention pathRef est ici un chemin Relatif
  // et non un chemin absolu comme dans NSModHtml
  while (exist)
  {
    incremente_code(sCompteur) ;
    nomComplet = nom + sCompteur ;
    if (serveur != "")
      sNomFichier = serveur + pathRef + nomComplet + string(".") + ext ;
    else
      sNomFichier = unite + pathRef + nomComplet + string(".") + ext ;
    exist = NsFileExists(sNomFichier) ;
  }

  // tant que le compteur ne dépasse pas ZZ on obtiendra un nom dos à 8 + 3
  if (nomComplet != "")
  	nomComplet = nomComplet + string(".") + ext ;

  return nomComplet ;
}


// -----------------------------------------------------------------------------
// void numacar(mot, nbre, taille)
// Fonction : Transforme un NOMBRE en CARACTERE
// Retour	  : Rien
// Créé le 02/08/1989 -- Dernière mise à jour le 28/07/1994
// -----------------------------------------------------------------------------
void
numacar(char *mot, unsigned int nbre, unsigned int taille)
{
	unsigned int i, multi ;

	if (taille > 5)
    taille = 5 ;

	multi = 1 ;
	for (i = 1 ; i < taille ; i++)
    multi = multi * 10 ;
	for (i = taille ; i > 0 ; i--, multi = multi / 10)
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

	return ;
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
// Suppression de la partie décimale, avec arrondi à l'entier le plus proche
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
// Conversion d'un double en int, avec arrondi à l'entier le plus proche
// -----------------------------------------------------------------------------
int
dtoi(double dNbre)
{
	return int(arrondiInt(dNbre)) ;
}


// -----------------------------------------------------------------------------
// int donneDigit(char nbre)
// Fonction : Transforme un caractère type '1', '2' en int
// Retour   : -1 si impossible
//					un entier de 0 à 9 sinon
// Créé le 02/08/1989 -- Dernière mise à jour le 28/07/1994
// -----------------------------------------------------------------------------
int
donneDigit(char nbre)
{
	if (!isdigit(nbre))
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
// Description : Ote les blancs à droite et à gauche de mot[]
// Return      : Rien
// Created on 13/10/1988 - last update on 11/06/1990
// -----------------------------------------------------------------------------
int
ote_blancs(char* mot)
{
	unsigned int i ;

  // on ôte les blancs finaux
  for (i = strlen(mot) ; (mot[i-1] == ' ') && (i > 0) ; i--)
    ;

  if (i == 0)  {    mot[0] = '\0' ;    return (0) ;  }  else
  	mot[i] = '\0' ;

  // On ôte les blancs initiaux
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

  //  On ôte les blancs terminaux
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
  else
  {
    for (i = 0 ; i < taille2 ; i++)
      if (pseumaj(mot1[i]) != pseumaj(mot2[i]))
        return (i + 1) ;
    return (taille2 + 1) ;
  }
}


// -----------------------------------------------------------------------------
// Function   : unsigned int pseumaj(lettre)
// Decription : Renvoie la pseudo-majuscule
// Return     : Rien
// Created on 13/10/1988 - last update on 28/07/1994
// -----------------------------------------------------------------------------
char
pseumaj(char lettre)

{	if ((lettre >= 'a') && (lettre <= 'z'))
  	return char(lettre - 32) ;
	switch (lettre)
	{
    case (unsigned) 'À' :
    case (unsigned) 'Â' :
    case (unsigned) 'Ä' :
		case (unsigned) 'à' : //226
    case (unsigned) 'â' :
    case (unsigned) 'ã' :
    case (unsigned) 'ä' :
    case (unsigned) 'å' :
    case (unsigned) 'æ' :
    case (unsigned) 'á' : return('A') ;
    case (unsigned) 'É' :
    case (unsigned) 'È' :
    case (unsigned) 'Ê' :
    case (unsigned) 'Ë' :
		case (unsigned) 'é' :
		case (unsigned) 'è' :
		case (unsigned) 'ê' :
		case (unsigned) 'ë' : return('E') ;
		case (unsigned) 'Î' :
    case (unsigned) 'Ï' :
    case (unsigned) 'î' :
    case (unsigned) 'í' :
		case (unsigned) 'ï' : return('I') ;
		case (unsigned) 'Ö' :
    case (unsigned) 'Ô' :
    case (unsigned) 'ò' :
    case (unsigned) 'ó' :
    case (unsigned) 'õ' :
    case (unsigned) 'ö' :
		case (unsigned) 'ô' : return('O') ;
		case (unsigned) 'Ù' :
    case (unsigned) 'Ü' :
    case (unsigned) 'Û' :
    case (unsigned) 'ù' :
		case (unsigned) 'ú' :
    case (unsigned) 'ü' :
		case (unsigned) 'û' : return('U') ;
    case (unsigned) 'Ç' :
		case (unsigned) 'ç' : return('C') ;
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
		case (unsigned) 'à' : return('À') ;
		case (unsigned) 'â' : return('Â') ;
		case (unsigned) 'ä' : return('Ä') ;
		case (unsigned) 'é' : return('É') ;
		case (unsigned) 'è' : return('È') ;
		case (unsigned) 'ê' : return('Ê') ;
		case (unsigned) 'ë' : return('Ë') ;
		case (unsigned) 'î' : return('Î') ;
		case (unsigned) 'ï' : return('Ï') ;
		case (unsigned) 'ö' : return('Ö') ;
		case (unsigned) 'ô' : return('Ô') ;
		case (unsigned) 'ù' : return('Ù') ;
		case (unsigned) 'ü' : return('Ü') ;
		case (unsigned) 'û' : return('Û') ;
		case (unsigned) 'ç' : return('Ç') ;
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
  char* chaine ;
  int len = strlen(psChaine->c_str()) ;
  chaine = new char[len + 1] ;
  strcpy(chaine, psChaine->c_str()) ;
  firstmaj(chaine) ;
  *psChaine = string(chaine) ;
  delete[] chaine ;
}
// -----------------------------------------------------------------------------// Function    : string strpids(string sChaine)// Description : Convertit la chaine en chaine de recherche
// Return      : la chaine de recherche
// Created on le 15/04/2004
// -----------------------------------------------------------------------------
stringstrpids(string sChaine){  string sSearch = sChaine ;  pseumaj(&sSearch) ;  // on remplace les \' et les tirets par des blancs  size_t pos = sSearch.find("\'") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))    	pos = sSearch.find("\'", pos + 1) ;    else    	break ;  }  pos = sSearch.find("-") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))      pos = sSearch.find("-", pos + 1) ;    else      break ;  }  // on enlève les blancs initiaux et terminaux  strip(sSearch) ;  // on enlève les double-blancs  bool bAcceptEspace = true ;  string sSearchCopy = sSearch ;  sSearch = "" ;  for (size_t i = 0 ; i < strlen(sSearchCopy.c_str()) ; i++)  {    if (sSearchCopy[i] == ' ')    {      if (bAcceptEspace)      {        sSearch += sSearchCopy[i] ;        bAcceptEspace = false ;      }    }    else    {      sSearch += sSearchCopy[i] ;      bAcceptEspace = true ;    }  }  return sSearch ;}string getFormatedTime(string sRawDate, string sLang, string sTimeFormat){
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
  if (iYearPos != NPOS)
  	sResult.replace(iYearPos, 4, sYear) ;

	size_t iMonthPos = sResult.find("MM") ;
  if (iMonthPos != NPOS)
  	sResult.replace(iMonthPos, 2, sMonth) ;

	size_t iDayPos = sResult.find("DD") ;
  if (iDayPos != NPOS)
  	sResult.replace(iDayPos, 2, sDays) ;

  string sHour   = string(sRawDate,  8, 2) ;
  string sMinute = string(sRawDate, 10, 2) ;
  string sSecond = string(sRawDate, 12, 2) ;

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
  if (NULL == heure)
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
  if (NULL == date)
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
	// Récupération de l'année
	for(i = 20 ; i < 24 ; i++)
		date[i-20] = temp[i] ;
	date[4] ='\0' ;

	// Récupération du mois
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

	// Récupération du jour
	for (i = 8 ; i < 10 ; i++)
		numero[i - 8] = temp[i] ;
	numero[2] = '\0' ;
	strcat(date, numero) ;
	return ;
}

void
get_current_date(string* sDate)
{
  if (NULL == sDate)
    return ;

  char szToday[10] ;
  donne_date_duJour(szToday) ;

  *sDate = string(szToday) ;
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

  if ((NULL == heure) || (strlen(heure) < 4))
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

// -----------------------------------------------------------------------------
// Function    : void donne_date(date, message, pays)// Description : Transforme une date AAAAMMJJ en un message JJ/MM/AAAA
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last update on 12/04/1991
// -----------------------------------------------------------------------------
void
donne_date(char *date, char *message, string sLang)
{
  if (NULL == message)
    return ;

  if ((NULL == date) || (strlen(date) < 8))
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

  // Etape 1 : on passe au format standardisé AAAAMMJJ
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

  // Etape 2 : vérifier que la date est valide

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

  // Mois non précisé : le seul contrôle, c'est que le jour ne le soit pas non plus
  // Month not set : we can only check if the day is not set either
  if (iMonth == 0)
  {
    if (iDay > 0)
      return ;
    sMessage = sTempo ;
    return ;
  }

  // Jour non précisé : pas de contrôle possible
  // Day not set : no possible check
  if (iMonth == 0)
  {
    if (iDay > 0)
    	return ;
    sMessage = sTempo ;
    return ;
  }

  // Date complète : on peut vérifier si elle existe
  int iYear = (iCentury * 100) + donneDigit(sTempo[2]) * 10 + donneDigit(sTempo[3]) ;

  // Ne pas utiliser mktime, car cette fonction échoue sur la plupart des
  // systèmes lorsque la date est inférieure au 1/1/1970
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
donne_date_claire(char* date, string *message, string sLang)
{
  if (NULL == message)
    return ;

	unsigned int i ;
	*message = "" ;
	if ((NULL == date) || (strlen(date) < 8))
		return ;

  // Day / Jour
  //
	string sDayLib = string("??") ;
	if (isdigit(date[6]) && isdigit(date[7]))
	{
  	if (date[6] != '0')
    	sDayLib = string(1, date[6]) ;
    else
  		sDayLib = string("") ;

    sDayLib += string(1, date[7]) ;
	}

	// Month / Mois
  //
	string sMonthLib = string("??") ;
	if (isdigit(date[4]) && isdigit(date[5]))
  {
		i = 10 * ((int) date[4] - (int) '0') + (int) date[5] - (int) '0' ;
		sMonthLib = donne_mois(i, sLang) ;
	}

  // Year / Année
  //
  string sYearLib = string("") ;
	for (i = 0 ; i < 4 ; i++)
	{
		if (isdigit(date[i]))
			sYearLib += string(1, date[i]) ;
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
// Description : Donne la date au format JJ/MM/AAAA                ¦
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void
donne_date_breve(string sDate, string* pMessage, string* pIntro, string sLang)
{
  if ((NULL == pMessage) || (NULL == pIntro))
    return ;

  *pMessage = "" ;
  *pIntro   = "" ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
  	return ;

  // Date vide
  if (sDate == string("00000000"))
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
    *pMessage = donne_mois(string(sDate, 4, 2), sLang) + " " + string(sDate, 0, 4) ;

    if ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
  		*pIntro = "en " ;
    if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
  		*pIntro = "in " ;
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
  		*pIntro = "le " ;
    else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
  		*pIntro = "on the " ;
  }
  return ;
}


// -----------------------------------------------------------------------------
// Description : Donne la date au format "1er janvier 1994"            ¦
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
void
donne_date_claire(string sDate, string* pMessage, string* pIntro, string sLang)
{
  if ((NULL == pMessage) || (NULL == pIntro))
    return ;

  *pMessage	= "" ;
  *pIntro		= "" ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
  	return ;

	// Date vide
  if (sDate == string("00000000"))
  	return ;

	if ((string(sDate, 4, 4) == "0000") || (string(sDate, 4, 4) == "0000"))
  {
  	donne_date_breve(sDate, pMessage, pIntro, sLang) ;
    return ;
	}

  if      ((sLang == string("fr")) || (sLang == string("fre")) || (sLang == string("fra")))
		*pIntro = "le " ;
  else if ((sLang == string("en")) || (sLang == string("eng")) || (sLang == string("en-US")))
		*pIntro = "on the " ;

  char szDate[9] ;
	strcpy(szDate, sDate.c_str()) ;
	donne_date_claire(szDate, pMessage, sLang) ;

	return ;
}


// -----------------------------------------------------------------------------
// Description : Indique si l'année donnée est bissextile
// -----------------------------------------------------------------------------
// Created on 28/06/99
// -----------------------------------------------------------------------------
bool
bissextile(int year)
{
  // Règle : Si l'année est divisible par 4 ET non divisible par 100
  //				OU divisible par 400 ==> elle est bissextile.
  if (((year % 4 == 0) && (!(year % 100 == 0))) || (year % 400 == 0))
  	return true;
  else
  	return false;
}

// -----------------------------------------------------------------------------
// Description : Incrémente une date donnée de un jour
// -----------------------------------------------------------------------------
// Created on 28/06/99
// -----------------------------------------------------------------------------
void
incremente_date(string& sDate)
{
  string sAnnee, sMois, sJour ;
  int    iAnnee, iMois, iJour, iJourFin ;
  char   cDate[9] ;

  // Date au format AAAAMMJJ
  if (strlen(sDate.c_str()) < 8)
	  return ;

  sAnnee = string(sDate, 0, 4) ;
  sMois = string(sDate, 4, 2) ;
  sJour = string(sDate, 6, 2) ;

  iAnnee = atoi(sAnnee.c_str()) ;
  iMois = atoi(sMois.c_str()) ;
  iJour = atoi(sJour.c_str()) ;

  switch (iMois)
  {
    case 1	:
    case 3	:
    case 5	:
    case 7	:
    case 8	:
    case 10	:    case 12	:	if (iJour < 31)
            		iJour++ ;
              else if (iJour == 31)
              {
                iJour = 1 ;
                if (iMois == 12)
                {
                	iMois = 1 ;
                  iAnnee++ ;
                }
                else
                	iMois++ ;
              }
              else
                erreur("Attention date invalide.", standardError, 0) ;
              break ;

    case 2	:	if (bissextile(iAnnee))
            		iJourFin = 29 ;
        			else
            		iJourFin = 28 ;

              if (iJour < iJourFin)
                iJour++ ;
              else if (iJour == iJourFin)
              {
                iJour = 1 ;
                iMois = 3 ;
              }
              else
                erreur("Attention date invalide.", standardError, 0) ;
              break ;

    case 4	:
    case 6	:
    case 9	:
    case 11	:	if (iJour < 30)
            		iJour++ ;
              else if (iJour == 30)
              {
                iJour = 1 ;
                iMois++ ;
              }
              else
                erreur("Attention date invalide.", standardError, 0) ;
              break ;

    default	:	erreur("Attention date invalide.", standardError, 0) ;
  }

  // on recompose la nouvelle date
  sprintf(cDate, "%4d%02d%02d", iAnnee, iMois, iJour) ;
  sDate = string(cDate) ;
}

// -----------------------------------------------------------------------------
// Description : Donne le libellé du mois à partir de "MM"
// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last update on 14/11/1997
// -----------------------------------------------------------------------------
string
donne_mois(string mois, string sLang)
{
  if (strlen(mois.c_str()) < 2)
  	return "" ;
  if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
  	return "" ;
  int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]) ;

  return donne_mois(i, sLang) ;
}

// -----------------------------------------------------------------------------
// Description : Donne un écart de temps de type "AAMMJJ"
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

// -----------------------------------------------------------------------------// Description : Donne le libellé du mois à partir d'un int             ¦// -----------------------------------------------------------------------------
// Created on 14/11/1997
// Last modified on 14/11/1997
// -----------------------------------------------------------------------------
string
donne_mois(int iMois, string sLang)
{
  if ((iMois < 1) || (iMois > 12))
    return "" ;

	if (sLang == string("fr"))
	{
  	switch (iMois)
  	{
    	case  1 : return "janvier"  ;
			case  2 : return "février"  ;
			case  3 : return "mars"     ;
			case  4 : return "avril"    ;
			case  5 : return "mai"      ;
			case  6 : return "juin"     ;
			case  7 : return "juillet"  ;
			case  8 : return "août"     ;
			case  9 : return "septembre";
			case 10 : return "octobre"  ;
			case 11 : return "novembre" ;
			case 12 : return "décembre" ;
   	}
	}
  else
	{
  	switch (iMois)
  	{
    	case  1 : return "january"  ;
			case  2 : return "february" ;
			case  3 : return "march"    ;
			case  4 : return "april"    ;
			case  5 : return "may"      ;
			case  6 : return "june"     ;
			case  7 : return "july"     ;
			case  8 : return "august"   ;
			case  9 : return "september";
			case 10 : return "october"  ;
			case 11 : return "november" ;
			case 12 : return "december" ;
   	}
	}
  return "" ;
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
  if (NULL == message)
    return ;

  if ((NULL == somme) || (strlen(somme) < 8))
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
    
  return ;
}


// -----------------------------------------------------------------------------
// Function    : void long_to_hex(unsigned long nb, char nb_hex[])
// Description : Convertit un entier long en hexa
// Parameters  : 	nb   	 -> l'entier à convertir
//            		nb_hex -> l'adresse de la chaîne
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 12/04/1991
// Last updated on 12/04/1991
// -----------------------------------------------------------------------------
void
long_to_hex(unsigned long nb, char* nb_hex)
{
  if (NULL == nb_hex)
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
  return ;
}


// -----------------------------------------------------------------------------
// Function    : void hex_to_long(char nb_hex[], unsigned long *nb)
// Description : Transforme un HHHHHHHH en unsigned long
// Parameters  : 	nb_hex	-> la chaine contenant l'hexa
//            		nb 	 		-> pointeur sur le résultat
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 17/09/1992
// Last update on 07/10/1992
// -----------------------------------------------------------------------------
void
hex_to_long(char nb_hex[], unsigned long *nb)
{
  if (NULL == nb)
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
  return ;
}

void
char_to_hex(unsigned char car, char* car_hex)
{
  if (NULL == car_hex)
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
  return ;
}

void
hex_to_char(char car_hex[], unsigned char *car)
{
  if (NULL == car)
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
  return ;
}

/*
// -----------------------------------------------------------------------------
// Function    : void format_date(char date[])
// Description : Retourne une date au format AAAAMMJJmmss à partir du format de asctime().
// Parameters  : adresse de la chaîne dans laquelle on désire placer la date.
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

  // Récupération de l'année
  for(i = 20 ; i < 24 ; i++)
  	date[i-20] = temp[i] ;
  date[4] ='\0' ;

  // Récupération du mois
  for (i = 4 ; i < 7 ; i++)
  	month[i-4] = temp[i] ;
  month[3] = '\0' ;
  for (i = 0 ; (i < 12) && (strcmp(month, mois[i]) != 0) ; i++)
  	;
  numacar(numero, i+1, 2) ;
  strcat(date, numero) ;

  // Récupération du jour
  for (i = 8 ; i < 10 ; i++)
  	numero[i-8] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // Récupération de l'heure
  for (i = 11 ; i < 13 ; i++)
  	numero[i-11] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // Récupération des minutes
  for (i = 14 ; i < 16 ; i++)
  	numero[i-14] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;

  // Récupération des secondes
  for (i = 17 ; i < 19 ; i++)
  	numero[i-17] = temp[i] ;
  numero[2] = '\0' ;
  strcat(date, numero) ;
  return ;
}
*/

// -----------------------------------------------------------------------------
// Function    : void carcat(unsigned char *chaine, unsigned char caractere)
// Description : équivalent de strcat, en ajoute un char en fin de chaîne
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 15/06/1994
// Last update on 15/06/1994
// -----------------------------------------------------------------------------
void
carcat(char* chaine, char caractere)
{
  if (NULL == chaine)
    return ;

  char ptgag[2] ;
  ptgag[0] = caractere ; ptgag[1] = '\0' ;
  strcat(chaine, ptgag) ;
  return ;
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
// Arguments   :	strPtr   ->  pointeur sur une chaîne qu'il faut initialiser
//             		length   ->  longueur de la chaîne
// Description : Initialise un pointeur sur une chaîne (char *)
//             	 Alloue un espace de longueur length. Initialise la chaîne des blancs.
// Returns     : TRUE si l'initialisation s'est passée sans pb. FALSE sinon
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
// Arguments   :	strPtr   ->  pointeur sur une chaîne qu'il faut initialiser
//             		strInit  ->  chaîne d'initialisation ou NULL si l'on désire des blancs
// Description : Initialise un pointeur sur une chaîne (char *). Initialise la chaîne avec strInit.
// Returns     : TRUE si l'initialisation s'est passée sans pb. FALSE sinon.
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
// Arguments   :	texte   -> texte affiché en cas d'erreur.
//            		gravité -> niveau de gravité.
// Description : Affiche les messages d'erreur et sort si le niveau de gravité est 1.
// Returns     : Retourne 0 si le problème rencontré est surmontable.
// -----------------------------------------------------------------------------
int
erreur(const char *texte, ERRORTYPE gravite, DBIResult ErrPx, HWND parent, string sHelp)
{
  if (NULL == texte)
    return 0 ;

  // NOTE : Si parent == NULL (cas par défaut)
  // La MessageBox ne sera pas modale !!!
  // Comme on ne peut accéder au contexte, il faut passer un GetHandle()
  // pour ce paramètre à l'appel de la fonction.
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
      sBuffer += string("\r\n\r\nSouhaitez vous consulter la marche à suivre face à une telle erreur ?") ;
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

  // On écrit le nouveau fichier reqerror.log
  outFile.open("ferror.log") ;
	if (!outFile)
  {
    // cas d'erreur bloquant...
    MessageBox(NULL, "Erreur d'ouverture en écriture du fichier ferror.log.", "Erreur NAUTILUS", MB_ICONEXCLAMATION) ;
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
  // Extraction de la partie entière et de la partie décimale

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

  // Partie entière
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

  // Partie décimale
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
  if (NULL == pDouble)
    return string("") ;

  double dVal = *pDouble ;
  if (dVal == 0)
  	return string("0") ;

  bool bNegatif = false ; //double négatif
  if (dVal < 0)
  {
    bNegatif = true ;
    dVal = -dVal ;
  }
  double dLog = log10(dVal) ;
  int	   iLog = int(dLog) ;
  double dMult = pow(double(10), double(iLog)) ;
  double dResu ;

  string sResult = "" ;

  // Assemblage de la partie entière
  while (iLog >= 0)
  {
    dResu = floor(dVal / dMult) ;
    sResult += string(1, donneChar(int(dResu))) ;

    dVal = dVal - (dResu * dMult) ;
    dMult = dMult / 10 ;
    iLog-- ;
  }
  if ((iDecimal == 0) || ((iDecimal == -1) && (dVal == 0)))
  {
    if (bNegatif)
    	sResult = "-" + sResult ;
    return sResult ;
  }

  // Assemblage de la partie décimale
  if (sResult == "")
  	sResult = "0" ;
  sResult += "." ;
  iLog = 0 ;
  bool tourner = true ;
  while (tourner)
  {
    dResu = floor(10 * dVal) ;
    // Si on en est à inscrire le dernier chiffre significatif, il faut
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
              if (i == 0)
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
    if ((iDecimal == -1) && ((iLog > 10) || (dVal == 0)))
    	tourner = false ;
  }

  if (bNegatif)
  	sResult = "-" + sResult ;
  return sResult ;
}



// -----------------------------------------------------------------------------
//
// Implémentation des mèthodes ClasseStringVector (vecteur de string)
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
ClasseStringVector::operator=(ClasseStringVector src)
{
	if (this == &src)
		return (*this) ;

	vider() ;

try
{
	if (false == src.empty())
    for (iterClassString i = src.begin() ; src.end() != i ; i++)
      push_back(new classString(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception ClasseStringVector = operator.", standardError, 0) ;
}
	return (*this) ;
}

ClasseStringVector::ClasseStringVector(ClasseStringVector& src)
{
try
{
	if (false == src.empty())
		for (iterClassString i = src.begin() ; src.end() != i ; i++)
    	push_back(new classString(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception ClasseStringVector copy ctor.", standardError, 0) ;
}
}
// -----------------------------------------------------------------------------//// classe classString//// -----------------------------------------------------------------------------

classString::classString(const classString& src){
  sItem      = src.sItem ;
  colonne    = src.colonne ;
  coche      = src.coche ;
  sQualifier = src.sQualifier ;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
classString&
classString::operator=(classString src)
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

// -----------------------------------------------------------------------------
// Split sModel into a left part and a right part on each side of first separator
// -----------------------------------------------------------------------------
void
splitString(string sModel, string *psLeft, string *psRight, char separator)
{
  if ((NULL == psLeft) || (NULL == psRight))
    return ;

  *psLeft  = string("") ;
  *psRight = string("") ;

  size_t iPosit = sModel.find(separator) ;
  size_t iLen   = strlen(sModel.c_str()) ;

  // No separator or at the end of model
  //
  if (string::npos == iPosit)
  {
    *psLeft = sModel ;
    return ;
  }

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
// -----------------------------------------------------------------------------
void getQualifiers(string sModel, string &sResult, string &sQualifier, string sQualifiersList)
{
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

  // go forward while Model starting chars belongs to Qualifiers List
  //
  while ((i < iModelLen) && bKeepGoing)
  {
    char cAtI = sModel[i] ;
    if (NPOS == sQualifiersList.find(string(1, cAtI)))
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
// Décomposer pChaine et mettre ses items dans le vecteur pVect Séparateur = "|" par défaur
// -----------------------------------------------------------------------------
void
DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur, string sQualifiers)
{
	if ((NULL == pChaine) || (NULL == pVect) || (string("") == *pChaine))
		return ;

  size_t debut     = 0 ;
  size_t posit     = pChaine->find(separateur) ;
  size_t iSeparLen = strlen(separateur.c_str()) ;

try
{
  if (string::npos == posit)
  {
    pVect->push_back(new classString(*pChaine)) ;
    return ;
  }

  while (posit != string::npos)
  {
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
    if ((debut + iSeparLen <= strlen(pChaine->c_str())) && (separateur == string(*pChaine, debut, iSeparLen)))
      posit = debut ;
    else
      posit = pChaine->find(separateur, debut) ;
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
  erreur("Exception DecomposeChaine.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Calcul de l'âge du patient
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
int
donne_age(char *datex, char *dateNaiss)
{
	if ((NULL == datex) || (NULL == dateNaiss))
		return -1 ;

	// On ne calcule l'âge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Année de l'examen
	unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Année de naissance du patient
	unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'année
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

// -----------------------------------------------------------------------------
// Calcul de l'âge des bébés en mois
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
int
donne_age_mois(char *datex, char *dateNaiss)
{
	if ((NULL == datex) || (NULL == dateNaiss))
		return -1 ;

	// On ne calcule l'âge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Année de l'examen
	unsigned int i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Année de naissance du patient
	unsigned int j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'année (en mois)
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
get_age(string *datex, string *dateNaiss)
{
  if ((NULL == datex) || (NULL == dateNaiss))
    return -1 ;
  if ((strlen(datex->c_str()) != 8) || (strlen(dateNaiss->c_str()) != 8))
    return -1 ;

  char szDatex[9] ;
  strcpy(szDatex, datex->c_str()) ;
  char szDatNais[9] ;
  strcpy(szDatNais, dateNaiss->c_str()) ;

  return donne_age(szDatex, szDatNais) ;
}

int
get_age_month(string *datex, string *dateNaiss)
{
  if ((NULL == datex) || (NULL == dateNaiss))
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
// Donne l'intitulé (Monsieur, Madame...)
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
void
donne_intitule_patient(string *intitule, int age, bool bFeminin, bool bShort, string sLang)
{
	if (NULL == intitule)
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

	return ;
}

// -----------------------------------------------------------------------------
// Donne l'intitulé de l'âge du patient              ¦
// -----------------------------------------------------------------------------
// Created on 17/07/1995
// Last update on 17/07/1995
// -----------------------------------------------------------------------------
void
donne_intitule_age(string *intitule, char *datex, char *dateNaiss)
{
	if ((NULL == intitule) || (NULL == datex) || (NULL == dateNaiss))
		return ;

	*intitule = "" ;

	// Calcul de l'âge
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
	// Bébé de moins de 2 ans
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
	return ;
}

// -----------------------------------------------------------------------------
// Décode une date AAAAMMJJ
// -----------------------------------------------------------------------------
// Created on 16/10/1992
// Last update on 16/10/1992
// -----------------------------------------------------------------------------
void
decode_date(string *resultat, char *date, string sLang)
{
	if ((NULL == resultat) || (NULL == date))
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
				case 2  : *resultat += "février " ;		break ;
				case 3  : *resultat += "mars " ;			break ;
				case 4  : *resultat += "avril " ;			break ;
				case 5  : *resultat += "mai " ;				break ;
				case 6  : *resultat += "juin " ;			break ;
				case 7  : *resultat += "juillet " ;		break ;
				case 8  : *resultat += "août " ;			break ;
				case 9  : *resultat += "septembre " ;	break ;
				case 10 : *resultat += "octobre " ;		break ;
				case 11 : *resultat += "novembre " ;	break ;
				case 12 : *resultat += "décembre " ;	break ;
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
	return ;
}

// -----------------------------------------------------------------------------
//
// Implémentation des mèthodes VecteurString
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
VectString::contains(string sModele)
{
	if (empty())
		return false ;

  for (IterString i = begin() ; end() != i ; i++)
  	if (sModele == **i)
			return true ;

	return false ;
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
VectString::operator=(VectString src)
{
	if (this == &src)
		return (*this) ;

	vider() ;
try
{
	if (false == src.empty())
		for (IterString i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception VectString::operator=", standardError, 0) ;}
	return (*this) ;
}

VectString::VectString(VectString& src)
{
try
{
	if (false == src.empty())
		for (IterString i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception VectString copy ctor.", standardError, 0) ;
}
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
  if ((string("") == sFormula) || (NULL == paBlocks))
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

  return ;

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

  if ((NULL == pDatas) || pDatas->empty())
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

    // on évalue l'opérande en cours    if (cc < strlen(sFormula.c_str()))
    {
      // on évalue d'abord la négation
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

    // cas erreur à l'évaluation de l'opérande    if (-1 == result1)
      return -1 ;
    if (bNegation)
      result1 = !result1 ;

    // on calcule le résultat selon l'opérateur en cours    if      (' ' == oper)
      result = result1 ;
    else if ('|' == oper)
      result = result || result1 ;
    else if ('&' == oper)
      result = result && result1 ;

    // on avance à nouveau    while ((cc < strlen(sFormula.c_str())) && (' ' == sFormula[cc]))
      cc++ ;

    // on évalue l'opérateur    if (cc < strlen(sFormula.c_str()))
    {
      // on doit avoir ici une ')' ou un opérateur
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
	double val[100] ;
	string ope[100] ;
	int	   nbNiv = 100 ;

	for (int j = 0; j < nbNiv; j++)
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
      case '(' : // Parenthèse ouvrante : on empile
      {
    	  for (int j = nbNiv - 1; j > pileur; j--)
        {
      	  val[j] = val[j-1] ;
          ope[j] = ope[j-1] ;
        }
        val[pileur] = 0 ;
        ope[pileur] = "" ;
        i++ ;

        break ;
      }
      case '+' : // Opérateurs + - * / ^x, etc
      case '-' :
      case '*' :
      case '/' :
      case '^' : // power  example 10^var
      case '£' : // log    example 10£var -> log10(var)

        // Processing sCurrentVar
        //
        if (string("") != sCurrentVar)
        {
          if ((NULL == pDatas) || (pDatas->empty()))
            return 0 ;
          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (ope[pileur] != "")
      	    val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
          else
      	    val[pileur] = dValue ;
        }

    	  ope[pileur] = string(1, sFormula[i]) ;
        i++ ;
        break ;

      case ')' : // Parenthèse fermante : on dépile
      {
        // Processing sCurrentVar
        //
        if (string("") != sCurrentVar)
        {
          if ((NULL == pDatas) || (pDatas->empty()))
            return 0 ;
          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (ope[pileur] != "")
      	    val[pileur] = operation(val[pileur], dValue, ope[pileur]) ;
          else
      	    val[pileur] = dValue ;
        }

    	  while (ope[pileur+1] != "")
        {
      	  val[pileur] = operation(val[pileur+1], val[pileur], ope[pileur+1]) ;
          for (int j = pileur+1; j < nbNiv-1; j++)
          {
        	  val[j] = val[j+1] ;
            ope[j] = ope[j+1] ;
          }
          val[nbNiv-1] = 0 ;
          ope[nbNiv-1] = "" ;
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
                      (sFormula[i] == '.')))
          {
            sNbre += string(1, sFormula[i]) ;
            i++ ;
          }
          double dNbre = StringToDouble(sNbre) ;
          if (ope[pileur] != "")
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
          if ((NULL == pDatas) || (pDatas->empty()))
            return 0 ;
          double dValue = (*pDatas)[sCurrentVar] ;
          sCurrentVar = string("") ;

          if (ope[pileur] != "")
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
    if ((NULL == pDatas) || (pDatas->empty()))
      return 0 ;
    double dValue = (*pDatas)[sCurrentVar] ;
    sCurrentVar = string("") ;

    if (ope[pileur] != "")
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
  if (string("£") == sOperation)
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
  if ((NULL == psFormula) || (string("") == *psFormula) || (string("") == sVar))
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
  string  sHtml("") ;

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
      case '£'  : sHtml = sHtml + "&#163;" ;		break ;
      case '€'  : sHtml = sHtml + "&euro;" ;		break ;
      case '²'  : sHtml = sHtml + "&sup2;" ;		break ;
      case 'â'  :	sHtml = sHtml + "&acirc;" ;		break ;
      case 'à'  : sHtml = sHtml + "&agrave;" ;	break ;
      case 'æ'  : sHtml = sHtml + "&aelig;" ;		break ;
      case 'ç'  : sHtml = sHtml + "&ccedil;" ;	break ;
      case 'é'  : sHtml = sHtml + "&eacute;" ;	break ;
      case 'ê'  : sHtml = sHtml + "&ecirc;" ;		break ;
      case 'è'  : sHtml = sHtml + "&egrave;" ;	break ;
      case 'ë'  : sHtml = sHtml + "&euml;" ;		break ;
      case 'î'  : sHtml = sHtml + "&icirc;" ;		break ;
      case 'ï'  : sHtml = sHtml + "&iuml;" ;		break ;
      case 'ô'  : sHtml = sHtml + "&ocirc;" ;		break ;
      case 'ö'  : sHtml = sHtml + "&ouml;" ;		break ;
      case 'œ'  : sHtml = sHtml + "&#339;" ;		break ;
      case 'ú'  : sHtml = sHtml + "&uacute;" ;	break ;
      case 'ù'  : sHtml = sHtml + "&ugrave;" ;	break ;
      case 'û'  : sHtml = sHtml + "&ucirc;" ;		break ;
      case 'ü'  : sHtml = sHtml + "&uuml;" ;		break ;
      case 'Â'  : sHtml = sHtml + "&Acirc;" ;		break ;      case 'À'  : sHtml = sHtml + "&Agrave;" ;	break ;      case 'Æ'  : sHtml = sHtml + "&Aelig;" ;		break ;      case 'Ç'  : sHtml = sHtml + "&Ccedil;" ;	break ;      case 'É'  : sHtml = sHtml + "&Eacute;" ;	break ;      case 'Ê'  : sHtml = sHtml + "&Ecirc;" ;		break ;      case 'È'  : sHtml = sHtml + "&Egrave;" ;	break ;      case 'Ë'  : sHtml = sHtml + "&Euml;" ;		break ;      case 'Î'  : sHtml = sHtml + "&Icirc;" ;		break ;      case 'Ï'  : sHtml = sHtml + "&Iuml;" ;		break ;      case 'Ò'  : sHtml = sHtml + "&Ograve;" ;	break ;      case 'Ó'  : sHtml = sHtml + "&Oacute;" ;	break ;      case 'Ô'  : sHtml = sHtml + "&Ocirc;" ;		break ;      case 'Õ'  : sHtml = sHtml + "&Otilde;" ;	break ;      case 'Ö'  : sHtml = sHtml + "&Ouml;" ;		break ;      case 'Œ'  : sHtml = sHtml + "&#338;" ;		break ;      case 'Ú'  : sHtml = sHtml + "&Uacute;" ;	break ;      case 'Ù'  : sHtml = sHtml + "&Ugrave;" ;	break ;      case 'Û'  : sHtml = sHtml + "&Ucirc;" ;		break ;      case 'Ü'	: sHtml = sHtml + "&Uuml;" ;		break ;      case 'Ý'	: sHtml = sHtml + "&Yacute;" ;	break ;      case 'Þ'	: sHtml = sHtml + "&THORN;" ;	  break ;      case 'ß'	: sHtml = sHtml + "&szlig;" ;	  break ;      default   : sHtml = sHtml + string(1, texte[i]) ;
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
      textOut.replace(pos, 7, "â") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&agrave;"))
      textOut.replace(pos, 8, "à") ;
    else if ((len >= 8) && (string(textOut, pos, 7) == "&aelig;"))
      textOut.replace(pos, 7, "æ") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&ccedil;"))
      textOut.replace(pos, 8, "ç") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&eacute;"))
      textOut.replace(pos, 8, "é") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ecirc;"))
      textOut.replace(pos, 7, "ê") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&egrave;"))
      textOut.replace(pos, 8, "è") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&euml;"))
      textOut.replace(pos, 6, "ë") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&icirc;"))
      textOut.replace(pos, 7, "î") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&iuml;"))
      textOut.replace(pos, 6, "ï") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ocirc;"))
      textOut.replace(pos, 7, "ô") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&ouml;"))
      textOut.replace(pos, 6, "ö") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&uacute;"))
      textOut.replace(pos, 8, "ú") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&ugrave;"))
      textOut.replace(pos, 8, "ù") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&ucirc;"))
      textOut.replace(pos, 7, "û") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&uuml;"))
      textOut.replace(pos, 6, "ü") ;
    else if ((len >= 4) && (string(textOut, pos, 4) == "&lt;"))
      textOut.replace(pos, 4, "<") ;
    else if ((len >= 4) && (string(textOut, pos, 4) == "&gt;"))
      textOut.replace(pos, 4, ">") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&amp;"))
      textOut.replace(pos, 6, "&") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&quot;"))
      textOut.replace(pos, 6, "\"") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&sup2;"))
      textOut.replace(pos, 6, "²") ;

    // gestion des majuscules accentuées
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Acirc;"))
      textOut.replace(pos, 7, "Â") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Agrave;"))
      textOut.replace(pos, 8, "À") ;
    else if ((len >= 8) && (string(textOut, pos, 7) == "&Aelig;"))
      textOut.replace(pos, 7, "Æ") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ccedil;"))
      textOut.replace(pos, 8, "Ç") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Eacute;"))
      textOut.replace(pos, 8, "É") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ecirc;"))
      textOut.replace(pos, 7, "Ê") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Egrave;"))
      textOut.replace(pos, 8, "È") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Euml;"))
      textOut.replace(pos, 6, "Ë") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Icirc;"))
      textOut.replace(pos, 7, "Î") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Iuml;"))
      textOut.replace(pos, 6, "Ï") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ograve;"))
      textOut.replace(pos, 8, "Ò") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Oacute;"))
      textOut.replace(pos, 8, "Ó") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ocirc;"))
      textOut.replace(pos, 7, "Ô") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Otilde;"))
      textOut.replace(pos, 8, "Õ") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Ouml;"))
      textOut.replace(pos, 6, "Ö") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Uacute;"))
      textOut.replace(pos, 8, "Ú") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Ugrave;"))
      textOut.replace(pos, 8, "Ù") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ucirc;"))
      textOut.replace(pos, 7, "Û") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&Uuml;"))
      textOut.replace(pos, 6, "Ü") ;
    else if ((len >= 8) && (string(textOut, pos, 8) == "&Yacute;"))
      textOut.replace(pos, 8, "Ý") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&THORN;"))
      textOut.replace(pos, 7, "Þ") ;
    else if ((len >= 7) && (string(textOut, pos, 7) == "&szlig;"))
      textOut.replace(pos, 7, "ß") ;

    else if ((len >= 6) && (string(textOut, pos, 6) == "&euro;"))
      textOut.replace(pos, 6, "€") ;

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
    else if ((len >= 6) && (string(textOut, pos, 6) == "&#163;"))      textOut.replace(pos, 6, "£") ;    else if ((len >= 5) && (string(textOut, pos, 5) == "&#36;"))      textOut.replace(pos, 5, "$") ;*/
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
  string  sHtml("") ;

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
  string  sTexte("") ;

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
  if (NULL == pTexte)
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
nomSansDoublons(string pathRef, string nom, string ext)
{
  int     cpt = 0 ;
  bool    exist = true ;
  char    suff[4] ;
  string  suffixe ;
  string  nomComplet ;
  string  sNomFichier ;

  while (exist)
  {
    sprintf(suff, "%03d", cpt) ;
    suffixe = string(suff) ;
    nomComplet = nom + string("_") + suffixe ;
    // nomFichier = TFileName(sServeur.c_str(),cUnite,pathRef.c_str(),nomComplet.c_str(),ext.c_str());
    sNomFichier = pathRef + nomComplet + string(".") + ext ;
    exist = NsFileExists(sNomFichier) ;
    cpt++ ;
    if (cpt == 1000) // limite du nombre de doublons
    {
      erreur("Vous avez atteint la limite du nombre de doublons.", standardError, 0) ;
      nomComplet = "" ;
      exist = false ;
    }
  }

  if (nomComplet != "")
    nomComplet = nomComplet + string(".") + ext ;

  return nomComplet ;
}
// -----------------------------------------------------------------------------//// classe ParamArray//// -----------------------------------------------------------------------------ParamArray::ParamArray(){}
ParamArray::~ParamArray()
{
	vider() ;
}


ParamArray::ParamArray(ParamArray& src)
{
  vector<string*>::iterator i ;

  if (false == src.aNomParam.empty())
  	for (i = src.aNomParam.begin() ; src.aNomParam.end() != i ; i++)
  		aNomParam.push_back(new string(*(*i))) ;

  if (false == src.aValParam.empty())
  	for (i = src.aValParam.begin() ; src.aValParam.end() != i ; i++)
  		aValParam.push_back(new string(*(*i))) ;
}


ParamArray&
ParamArray::operator=(ParamArray src)
{
	if (this == &src)
		return (*this) ;

  vector<string*>::iterator i;

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

stringParamArray::ValParam(string sParam){  char szParam[255] ;  char szNomParam[255] ;  bool trouve = false ;  string sValeur = "" ;  vector<string*>::iterator i ;  int k = 0 ;  strcpy(szParam, sParam.c_str()) ;  pseumaj(szParam) ;  for (i = aNomParam.begin() ; aNomParam.end() != i ; i++)  {    strcpy(szNomParam, (*(*i)).c_str()) ;    pseumaj(szNomParam) ;    if (!strcmp(szParam,szNomParam))    {      trouve = true ;      break ;    }    k++ ;  }  if (trouve)  	sValeur = *(aValParam[k]) ;  return sValeur ;}boollireParam(string sUtil, string sFichierDat, ParamArray *pParamArray){  if (NULL == pParamArray)    return false ;  ifstream  inFile ;  string    line ;  string    sData         = "" ;
  string    sName, sExt ;
  string    sNomAttrib ;
  string    sValAttrib ;  size_t    i             = 0 ;
  size_t    pos ;
  char      msg[255] ;
  char      fichier[255] ;

  pParamArray->vider() ;

  pos = sFichierDat.find(".") ;
  if (pos == string::npos)
  {
    sprintf(msg, "Erreur : Le fichier de paramètres %s n'a pas d'extension", sFichierDat.c_str()) ;
    erreur(msg, standardError, 0) ;
    return false ;
  }

  sName = string(sFichierDat, 0, pos) ;
  sExt = string(sFichierDat, pos, strlen(sFichierDat.c_str()) - pos) ;
  sprintf(fichier, "%s%3s%s", sName.c_str(), sUtil.c_str(), sExt.c_str()) ;

  inFile.open(fichier) ;
  if (!inFile)
  {
    inFile.open(sFichierDat.c_str()) ;    if (!inFile)    {      sprintf(msg, "Erreur d'ouverture du fichier %s.", sFichierDat.c_str()) ;      erreur(msg, standardError, 0) ;      return false ;    }  }
  while (!inFile.eof())  {    getline(inFile,line) ;    if (line != "")      sData += line + "\n" ;  }
  inFile.close() ;  // boucle de chargement des attributs  i = 0 ;
  while (i < strlen(sData.c_str()))
  {    sNomAttrib = "" ;    sValAttrib = "" ;    if ((sData[i] == '/') && (((i + 1) < strlen(sData.c_str())) && (sData[i + 1] == '/')))    {      while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))        i++ ;      i++ ;      continue ;    }
    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sNomAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))      i++ ;
    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sValAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))      i++ ;
    i++ ;
    pParamArray->PushParam(sNomAttrib, sValAttrib) ;
  }

  return true ;
}boollireParam(string sUtil, string sFichierDat, string sParam, string& sResult)
{
  ifstream  inFile ;
  string    line ;  string    sData           = "" ;
  string    sNomAttrib ;
  string    sValAttrib ;  string    sName, sExt ;  size_t    i               = 0 ;
  size_t    pos ;
  char      msg[255] ;
  char      fichier[255] ;
  char      szParam[255] ;
  char      szNomParam[255] ;
  bool      trouve = false ;

  sResult = "" ;

  pos = sFichierDat.find(".") ;
  if (pos == string::npos)
  {
    sprintf(msg, "Erreur : Le fichier de paramètres %s n'a pas d'extension", sFichierDat.c_str()) ;
    erreur(msg, standardError, 0) ;
    return false ;
  }

  sName = string(sFichierDat, 0, pos) ;
  sExt = string(sFichierDat, pos, strlen(sFichierDat.c_str()) - pos) ;
  sprintf(fichier, "%s%3s%s", sName.c_str(), sUtil.c_str(), sExt.c_str()) ;

  inFile.open(fichier) ;

  if (!inFile)
  {
    inFile.open(sFichierDat.c_str()) ;    if (!inFile)    {      sprintf(msg, "Erreur d'ouverture du fichier %s.", sFichierDat.c_str()) ;      erreur(msg, standardError, 0) ;      return false ;    }  }
  while (!inFile.eof())  {    getline(inFile,line) ;    if (line != "")      sData += line + "\n" ;  }
  inFile.close() ;  // boucle de chargement des attributs  strcpy(szParam, sParam.c_str()) ;
  pseumaj(szParam) ;
  i = 0 ;
 	while (i < strlen(sData.c_str())) 	{    sNomAttrib = "" ;    sValAttrib = "" ;    if ((sData[i] == '/') && (((i + 1) < strlen(sData.c_str())) && (sData[i + 1] == '/')))    {      while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))        i++ ;      i++ ;      continue ;    }
    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sNomAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))      i++ ;
    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sValAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))      i++ ;
    i++ ;
    strcpy(szNomParam, sNomAttrib.c_str()) ;
    pseumaj(szNomParam) ;
    if (!strcmp(szParam, szNomParam))
    {
      trouve = true ;
      break ;
    }
  }

  if (!trouve)
    return false ;

  sResult = sValAttrib ;
  return true ;
}

bool
ecrireParam(string sUtil, string sFichierDat, string sParam, string sValeur)
{
  ifstream  inFile ;
  string    sOut = "" ;
  ofstream  outFile ;
  string    line ;  string    sData = "" ;
  string    sNomAttrib ;
  string    sValAttrib ;  string    sName, sExt ;  size_t    i = 0 ;
  size_t    pos ;
  char      msg[255] ;
  char      fichier[255] ;
  char      szParam[255] ;
  char      szNomParam[255] ;
  bool      trouve        = false ;
  bool      bModeUtil     = true ;

  pos = sFichierDat.find(".") ;
  if (pos == string::npos)
  {
    sprintf(msg, "Erreur : Le fichier de paramètres %s n'a pas d'extension", sFichierDat.c_str()) ;
    erreur(msg, standardError, 0) ;
    return false ;
  }

  sName = string(sFichierDat, 0, pos) ;
  sExt = string(sFichierDat, pos, strlen(sFichierDat.c_str()) - pos) ;
  sprintf(fichier, "%s%3s%s", sName.c_str(), sUtil.c_str(), sExt.c_str()) ;

  inFile.open(fichier) ;
  if (!inFile)
  {
    bModeUtil = false ;
    inFile.open(sFichierDat.c_str()) ;    if (!inFile)    {      sprintf(msg, "Erreur d'ouverture du fichier %s.", sFichierDat.c_str()) ;      erreur(msg, standardError, 0) ;      return false ;    }  }
  while (!inFile.eof())  {    getline(inFile,line) ;    if (line != "")      sData += line + "\n" ;  }
  inFile.close() ;  // boucle de chargement des attributs  strcpy(szParam, sParam.c_str()) ;
  pseumaj(szParam) ;
  i = 0 ;
  while (i < strlen(sData.c_str()))  {    sNomAttrib = "" ;    sValAttrib = "" ;    if ((sData[i] == '/') && (((i + 1) < strlen(sData.c_str())) && (sData[i + 1] == '/')))    {      while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))        i++ ;      i++ ;      continue ;    }
    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sNomAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))      i++ ;
    // on retient la position de sValAttrib
    pos = i ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t') && (sData[i] != '\n'))      sValAttrib += sData[i++] ;
    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))      i++ ;
    i++ ;
    strcpy(szNomParam, sNomAttrib.c_str()) ;
    pseumaj(szNomParam) ;
    if (!strcmp(szParam, szNomParam))
    {
      trouve = true ;
      break ;
    }
  }

  if (!trouve)
    return false ;

  sData.replace(pos, strlen(sValAttrib.c_str()), sValeur) ;

  // on réécrit le nouveau fichier dans sOut
  sOut = "" ;
  for (size_t i = 0 ; i < strlen(sData.c_str()) ; i++)
    sOut += sData[i] ;

  // ecriture du fichier
  string sFichierOut ;

  if (bModeUtil)
    sFichierOut = string(fichier) ;  else    sFichierOut = sFichierDat ;  outFile.open(sFichierOut.c_str()) ;  if (!outFile)  {    sprintf(msg, "Impossible de réécrire le fichier de paramètres %s.", sFichierOut.c_str()) ;    erreur(msg, standardError, 0) ;    return false ;  }
  for (size_t i = 0 ; i < strlen(sOut.c_str()) ; i++)    outFile.put(sOut[i]) ;  outFile.close() ;

  return true ;
}

// -----------------------------------------------------------------------------
// Fonction qui parse une string pour récupérer les coordonnées d'une fenetre
// -----------------------------------------------------------------------------
void
getWindowPosit(string sPosition, int& X, int& Y, int& W, int& H, string* psTaille)
{
  if (string("") == sPosition)
    return ;

  size_t  debut = 0 ;  size_t  positVide = sPosition.find("|") ;  //chaîne vide  int     nbCoords = 0 ;
  vector<string> Coordonnees ;  //contient les coordonnées sous forme de string
  while (string::npos != positVide)  {    Coordonnees.push_back(string(sPosition, debut, positVide - debut)) ;    nbCoords++ ;    debut = positVide + 1 ;    positVide = sPosition.find("|", debut) ;  }
  Coordonnees.push_back(string(sPosition, debut, strlen(sPosition.c_str()) - debut)) ;  nbCoords++ ;
  //récupérer les coordonnées  vector<string> :: iterator i ;  i = Coordonnees.begin() ;  X = StringToDouble(*i) ;  i++ ;
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
// L’algorithme de Luhn permet de calculer la clé de contrôle d’un nombre à
// valider, ce qui s’avère très utile pour vérifier la cohérence de nombres tels
// que des numéros de cartes de crédits.
//
// La procédure consiste à dérouler les phases suivantes :
//
//   1. doubler la valeur d’un chiffre sur deux en commençant par le premier à gauche
//   2. calculer la somme des chiffres composant la séquence ainsi obtenue (pas des nombres : 14 -> 1+4)
//   3. complémenter à 10 le chiffre des unités de la somme pour obtenir la clé
//
// Prenons par exemple, une carte de crédit dont le numéro est 497010000030052
// et calculons sa clé.
//
// Phases 	4  9  7  0  1  0  0  0  0  0  3  0  0  5  2
//      1 	8  9 14  0  2  0  0  0  0  0  6  0  0  5  4
//      2 	8+9+1+4+0+2+0+0+0+0+0+6+0+0+5+4 = 3 9
//      3                                       1
//
// On complète ensuite le numéro de la carte de crédit avec la clé,
// ce qui donne le numéro 4970100000300521.

bool luhn(char* l)
{
  if (NULL == l)
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

  LPTSTR lpszTemp = NULL;
  DWORD  dwRet = FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_ARGUMENT_ARRAY,
                                NULL,
                                dwLastError,
                                LANG_NEUTRAL,
                                (LPTSTR)&lpszTemp,
                                0,
                                NULL) ;

  string sErrorMessage = string("") ;

  if (NULL != lpszTemp)
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
  if (f == 0)
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

