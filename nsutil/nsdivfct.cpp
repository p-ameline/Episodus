// -----------------------------------------------------------------------------// nsdivfct.cpp
// -----------------------------------------------------------------------------
// Pot-pourri de fonctions
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2010/10/09 16:21:21 $
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
static bool bModeFile           = false ;const char cheminSeparationMARK = '/' ;

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
stringgetNodeIDFromID(string sAnyID){	if (strlen(sAnyID.c_str()) < 18)		return string("") ;	return string(sAnyID, 13, 5) ;}// -----------------------------------------------------------------------------// Function : Transforme un int en string// -----------------------------------------------------------------------------stringIntToString(int val){	char temp[10] ;  itoa(val, temp, 10) ;  string result(temp) ;  return result ;}
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
// void strip(string& s, int n)
// Fonction : enleve les blancs d'une string
// Retour   : rien
// -----------------------------------------------------------------------------
void
strip(string& s, int n, char c)
{
  if (s == "")
    return ;

  size_t pos1 = s.find_first_not_of(c) ;
  if (pos1 == string::npos)
  {
		s = "" ;
    return ;
  }
  size_t pos2 ;

  switch (n)
  {
    case stripBoth  : pos2 = s.find_last_not_of(c) ;
                      if ((pos1 != string::npos) && (pos2 != string::npos))
                        s = string(s, pos1, pos2 - pos1 + 1) ;
                      break ;
    case stripLeft  : s = string(s, pos1, strlen(s.c_str()) - pos1) ;
                      break ;
    case stripRight : pos2 = s.find_last_not_of(c) ;
                      if (pos2 != string::npos)
                        s = string(s, 0, pos2 + 1) ;
                      break ;
    default         : erreur("Fonction strip : option erronnée", standardError, 0) ;
  }
}


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
		case (unsigned) 'à'	:
		case (unsigned) 'â' :
		case (unsigned) 'ä' : return('A') ;
		case (unsigned) 'é' :
		case (unsigned) 'è' :
		case (unsigned) 'ê' :
		case (unsigned) 'ë' : return('E') ;
		case (unsigned) 'î' :
		case (unsigned) 'ï' : return('I') ;
		case (unsigned) 'ö' :
		case (unsigned) 'ô' : return('O') ;
		case (unsigned) 'ù' :
		case (unsigned) 'ü' :
		case (unsigned) 'û' : return('U') ;
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
stringstrpids(string sChaine){  string sSearch = sChaine ;  pseumaj(&sSearch) ;  // on remplace les \' et les tirets par des blancs  size_t pos = sSearch.find("\'") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))    	pos = sSearch.find("\'", pos + 1) ;    else    	break ;  }  pos = sSearch.find("-") ;  while (pos != string::npos)  {    sSearch[pos] = ' ' ;    if (pos < (strlen(sSearch.c_str()) - 1))      pos = sSearch.find("-", pos + 1) ;    else      break ;  }  // on enlève les blancs initiaux et terminaux  strip(sSearch) ;  // on enlève les double-blancs  bool bAcceptEspace = true ;  string sSearchCopy = sSearch ;  sSearch = "" ;  for (size_t i = 0 ; i < strlen(sSearchCopy.c_str()) ; i++)  {    if (sSearchCopy[i] == ' ')    {      if (bAcceptEspace)      {        sSearch += sSearchCopy[i] ;        bAcceptEspace = false ;      }    }    else    {      sSearch += sSearchCopy[i] ;      bAcceptEspace = true ;    }  }  return sSearch ;}// -----------------------------------------------------------------------------// Function    : void donne_heure(char* pHeure)// Description : Donne l'heure au format hhmmss
// Return      : Rien
// -----------------------------------------------------------------------------
// Created on 26/05/1998
// last update on 26/05/1998
// -----------------------------------------------------------------------------
void
donne_heure(char* heure)
{
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
  message = "" ;
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
  sMessage = "" ;

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
	unsigned int i ;
	*message = "" ;
	if (strlen(date) < 8)
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
  unsigned int i ;

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
  unsigned int i ;
  strcpy(message, "000000,00") ;
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
  // NOTE : Si parent == NULL (cas par défaut)
  // La MessageBox ne sera pas modale !!!
  // Comme on ne peut accéder au contexte, il faut passer un GetHandle()
  // pour ce paramètre à l'appel de la fonction.
  // Attention GetMainWindow()->GetHandle() ne vaut pas NULL...

  char buffer[2048] ;
  if ((texte[0] == '\0') && (ErrPx == DBIERR_NONE))
  	return(0) ;

  strcpy(buffer, texte) ;

  if ((ErrPx != 0) && (ErrPx != DBIERR_NONE))
  {
    char        dbi_status[DBIMAXMSGLEN * 5] = {'\0'} ;
    DBIMSG      dbi_string = {'\0'} ;
    DBIErrInfo  ErrInfo ;

    if (ErrPx != DBIERR_NONE)
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
      if (buffer[0] != '\0')
        strcat(buffer, "\r\n\r\n") ;
      strcat(buffer, dbi_status) ;
    }
    //return ErrorValue ;
  }

  if (getModeFileErr() == true)
  	fileError(buffer) ;
  else
  {
    if (sHelp == "")
    {      if (gravite == warningError)
        MessageBox(parent, buffer, "AVERTISSEMENT", MB_ICONEXCLAMATION | MB_OK) ;
      else
        MessageBox(parent, buffer, "ERREUR", MB_ICONEXCLAMATION | MB_OK) ;    }
    else    {
      strcat(buffer, "\r\n\r\nSouhaitez vous consulter la marche à suivre face à une telle erreur ?") ;
      int iRetour ;
      if (gravite == warningError)        iRetour = MessageBox(parent, buffer, "AVERTISSEMENT", MB_YESNO) ;      else        iRetour = MessageBox(parent, buffer, "ERREUR", MB_YESNO) ;      if (iRetour == IDYES)
        ::ShellExecute(parent, "open", sHelp.c_str(), "", "", SW_SHOWNORMAL) ;
    }
  }
  if (gravite == fatalError)
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

  // Partie entière
  for (int i = 0 ; i < strlen(sEntier.c_str()) ; i++)
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

  return dValeur ;
}


//-------------------------------------------------------------------
// convertit un double en une string
//-------------------------------------------------------------------
string
DoubleToString(double* pDouble, int iEntier, int iDecimal)
{
  double dVal = *pDouble ;
  if (dVal == 0)
  	return "0" ;

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
	:	vectclassString()
{
}

void
ClasseStringVector::vider()
{
	if (empty())
		return ;

	iterString i = begin() ;
  while (i != end())
  {
    delete (*i) ;
    erase(i) ;
  }
}


ClasseStringVector&
ClasseStringVector::operator=(ClasseStringVector src)
{
	vider() ;
try
{
	if (!(src.empty()))
    for (iterString i = src.begin() ; i != src.end() ; i++)
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
	if (!(src.empty()))
		for (iterString i = src.begin() ; i != src.end() ; i++)
    	push_back(new classString(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception ClasseStringVector copy ctor.", standardError, 0) ;
}
}
// -----------------------------------------------------------------------------//// classe classString//// -----------------------------------------------------------------------------

classString::classString(const classString& src){
  sItem   = src.sItem ;
  colonne = src.colonne ;
  coche   = src.coche ;
}


// -----------------------------------------------------------------------------
// Opérateur d'affectation
// -----------------------------------------------------------------------------
classString&
classString::operator=(classString src)
{
  sItem   = src.sItem ;
  colonne = src.colonne ;
  coche   = src.coche ;
  return (*this) ;
}


classString::classString(string sChaine, int Colonne, bool Coche)
{
  sItem 	= sChaine ;
  colonne = Colonne ;
  coche 	= Coche ;
}


// -----------------------------------------------------------------------------
// Décomposer pChaine et mettre ses items dans le vecteur pVect Séparateur = "|" par défaur
// -----------------------------------------------------------------------------
void
DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur )
{
	if (!pChaine || !pVect)
		return ;

  size_t debut = 0 ;
  size_t posit = pChaine->find(separateur) ;

  try
  {
    if(posit == string::npos)
    	pVect->push_back(new classString(*pChaine)) ;
    else
    {
      while (posit != string::npos)
      {
        pVect->push_back(new classString(string(*pChaine, debut, posit - debut))) ;
        debut = posit + 1 ;
        posit = pChaine->find(separateur, debut + 1) ;
      }
      pVect->push_back(new classString(string(*pChaine, debut, strlen(pChaine->c_str()) - debut ))) ;
    }
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
	if (!datex || !dateNaiss)
		return -1 ;

	unsigned int i, j ;
	signed   int age ;
	unsigned int mois_nais, mois_jour, jour_nais, jour_jour ;

	// On ne calcule l'âge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Année de l'examen
	i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Année de naissance du patient
	j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'année
	age = i - j ;

	// Correctifs en fonction de mois et jour
	mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]) ;
	jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]) ;
	mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]) ;
	jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]) ;
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
	if (!datex || !dateNaiss)
		return -1 ;

	unsigned int i, j ;
	signed   int age ;
	unsigned int mois_nais, mois_jour, jour_nais, jour_jour ;

	// On ne calcule l'âge que si on a la date de naissance
	if ((strcmp(dateNaiss, "00000000") == 0) || (strcmp(dateNaiss, "19000000") == 0))
		return -1 ;

	// Année de l'examen
	i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;

	// Année de naissance du patient
	j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;

	// Age qu'aura le patient dans l'année (en mois)
	age = (i - j) * 12 ;

	// Correctifs en fonction de mois et jour
	mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]) ;
	jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]) ;
	mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]) ;
	jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]) ;
	age = age + mois_jour - mois_nais ;
	if (jour_jour < jour_nais)
		age-- ;
	return age ;
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
	if (!intitule)
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
	if (!intitule || !datex || !dateNaiss)
		return ;

	char ptgag[4] ;
	int  i, age ;
	*intitule = "" ;

	// Calcul de l'âge
	age = donne_age(datex, dateNaiss) ;

	// Patient d'au moins 2 ans
	if (age >= 2)
	{
		numacar(ptgag, age, 3) ;
		while (ptgag[0] == '0')
			for (i = 0 ; ptgag[i] != '\0' ; i++)
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
				for (i = 0 ; ptgag[i] != '\0' ; i++)
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
	if (!resultat || !date)
		return ;

	unsigned int i, jour, mois ;
	char 			 date2[11] ;
	jour = 10 * donneDigit(date[6]) + donneDigit(date[7]) ;
	mois = 10 * donneDigit(date[4]) + donneDigit(date[5]) ;
	if ((jour < 1) || (jour > 31))
  	jour = 0 ;
	if ((mois < 1) || (mois > 12))
  	mois = 0 ;
	if (jour == 0)
	{
		strcpy(date2, "19XX") ;
		for (i = 0 ; i < 4 ; i++)
    	date2[i] = date[i] ;
		*resultat = " en " ;
		if (mois != 0)
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
		*resultat += date2 ;
	}
	else
	{
		strcpy(date2, "XX/XX/19XX") ;
		for (i = 0 ; i < 4 ; i++)
    	date2[i+6] = date[i] ;
		for (i = 0 ; i < 2 ; i++)
    	date2[i+3] = date[i+4] ;
		for (i = 0 ; i < 2 ; i++)
    	date2[i]   = date[i+6] ;
		*resultat = " le " ;
		*resultat += date2 ;
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

	IterString i = begin() ;
  for ( ; i != end() ; i++)
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
  while (i != end())
  {
    delete (*i) ;
    erase(i) ;
  }
}

VectString&
VectString::operator=(VectString src)
{
	vider() ;
try
{
	if (!(src.empty()))
		for (IterString i = src.begin() ; i != src.end() ; i++)
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
	if (!(src.empty()))
		for (IterString i = src.begin() ; i != src.end() ; i++)
			push_back(new string(*(*i))) ;
} // try
catch (...)
{
	erreur("Exception VectString copy ctor.", standardError, 0) ;
}
}

string
texteHtml(string texte)
{
  size_t  i ;
  string  sHtml("") ;

  for (i = 0 ; i < strlen(texte.c_str()) ; i++)
  {
    switch (texte[i])
    {
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
      case 'ú'  : sHtml = sHtml + "&uacute;" ;	break ;
      case 'ù'  : sHtml = sHtml + "&ugrave;" ;	break ;
      case 'û'  : sHtml = sHtml + "&ucirc;" ;		break ;
      case 'ü'  : sHtml = sHtml + "&uuml;" ;		break ;
      case '<'  : sHtml = sHtml + "&lt;" ;			break ;
      case '>'  : sHtml = sHtml + "&gt;" ;			break ;
      case '&'  : sHtml = sHtml + "&ramp;" ;		break ;
      case '"'  : sHtml = sHtml + "&quot;" ;		break ;
      case '|'  : sHtml = sHtml + "&#124;" ;		break ;
      case '£'  : sHtml = sHtml + "&#163;" ;		break ;      case '\'' : sHtml = sHtml + "&#39;" ;			break ;      case '$'  : sHtml = sHtml + "&#36;" ;			break ;      case 'Â'  : sHtml = sHtml + "&Acirc;" ;		break ;      case 'À'  : sHtml = sHtml + "&Agrave;" ;	break ;      case 'Æ'  : sHtml = sHtml + "&Aelig;" ;		break ;      case 'Ç'  : sHtml = sHtml + "&Ccedil;" ;	break ;      case 'É'  : sHtml = sHtml + "&Eacute;" ;	break ;      case 'Ê'  : sHtml = sHtml + "&Ecirc;" ;		break ;      case 'È'  : sHtml = sHtml + "&Egrave;" ;	break ;      case 'Ë'  : sHtml = sHtml + "&Euml;" ;		break ;      case 'Î'  : sHtml = sHtml + "&Icirc;" ;		break ;      case 'Ï'  : sHtml = sHtml + "&Iuml;" ;		break ;      case 'Ô'  : sHtml = sHtml + "&Ocirc;" ;		break ;      case 'Ö'  : sHtml = sHtml + "&Ouml;" ;		break ;      case 'Ú'  : sHtml = sHtml + "&Uacute;" ;	break ;      case 'Ù'  : sHtml = sHtml + "&Ugrave;" ;	break ;      case 'Û'  : sHtml = sHtml + "&Ucirc;" ;		break ;      case 'Ü'	: sHtml = sHtml + "&Uuml;" ;		break ;      default   : sHtml = sHtml + texte[i] ;
    }
	}
	return sHtml ;}

string
texteCourant(string textHtml)
{
	string textOut  = textHtml ;
  size_t pos      = textOut.find("&") ;
  size_t len ;

  while (pos != string::npos)
  {
    len = strlen(textOut.c_str()) - pos ;
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
    else if ((len >= 6) && (string(textOut, pos, 6) == "&ramp;"))
      textOut.replace(pos, 6, "&") ;
    else if ((len >= 6) && (string(textOut, pos, 6) == "&quot;"))
      textOut.replace(pos, 6, "\"") ;

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
    else if ((len >= 7) && (string(textOut, pos, 7) == "&Ocirc;"))
      textOut.replace(pos, 7, "Ô") ;
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
  size_t  i ;
  string  sHtml("") ;

  for (i = 0 ; i < strlen(texte.c_str()) ; i++)
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
  size_t  i ;
  string  sTexte("") ;

  for (i = 0 ; i < strlen(textWL.c_str()) ; i++)
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
	// Remplace les \n et les \r\n par des \r\r\n : uniquement utile pour envoyer dans un edit
  //
  size_t posCr = pTexte->find("\n") ;
  
  while (posCr != string::npos)
  {
  	int iNumberOfAddedChar = 0 ;

  	if (posCr == 0)
    {
    	*pTexte = string("\r\r") + *pTexte ;
      iNumberOfAddedChar = 2 ;
    }
    else
    {
    	if ((*pTexte)[posCr-1] != '\r')
      {
      	*pTexte = string(*pTexte, 0, posCr) + string(2, '\r') + string(*pTexte, posCr, strlen(pTexte->c_str()) - posCr) ;
        iNumberOfAddedChar = 2 ;
      }
      else if (posCr == 1)
      {
      	*pTexte = string("\r") + *pTexte ;
        iNumberOfAddedChar = 1 ;
      }
      else if ((*pTexte)[posCr-2] != '\r')
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

  if (!src.aNomParam.empty())
  	for (i = src.aNomParam.begin() ; i != src.aNomParam.end() ; i++)
  		aNomParam.push_back(new string(*(*i))) ;

  if (!src.aValParam.empty())
  	for (i = src.aValParam.begin() ; i != src.aValParam.end() ; i++)
  		aValParam.push_back(new string(*(*i))) ;
}


ParamArray&
ParamArray::operator=(ParamArray src)
{
  vector<string*>::iterator i;

  if (!src.aNomParam.empty())
  	for (i = src.aNomParam.begin() ; i != src.aNomParam.end() ; i++)
  		aNomParam.push_back(new string(*(*i))) ;

  if (!src.aValParam.empty())
  	for (i = src.aValParam.begin() ; i != src.aValParam.end() ; i++)
  		aValParam.push_back(new string(*(*i))) ;

  return (*this) ;
}


void
ParamArray::vider()
{
  vector<string*>::iterator i ;

  if (!aNomParam.empty())
    for (i = aNomParam.begin() ; i != aNomParam.end() ; )
    {
      delete (*i) ;
      aNomParam.erase(i) ;
    }

  if (!aValParam.empty())
    for (i = aValParam.begin() ; i != aValParam.end() ; )
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


stringParamArray::ValParam(string sParam){  char szParam[255] ;  char szNomParam[255] ;  bool trouve = false ;  string sValeur = "" ;  vector<string*>::iterator i ;  int k = 0 ;  strcpy(szParam, sParam.c_str()) ;  pseumaj(szParam) ;  for (i = aNomParam.begin() ; i != aNomParam.end() ; i++)  {    strcpy(szNomParam, (*(*i)).c_str()) ;    pseumaj(szNomParam) ;    if (!strcmp(szParam,szNomParam))    {      trouve = true ;      break ;    }    k++ ;  }  if (trouve)  	sValeur = *(aValParam[k]) ;  return sValeur ;}boollireParam(string sUtil, string sFichierDat, ParamArray *pParamArray){  ifstream  inFile ;  string    line ;  string    sData         = "" ;
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
}bool
lireParam(string sUtil, string sFichierDat, string sParam, string& sResult)
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
  if (sPosition == "")
    return ;

  size_t  debut = 0 ;  size_t  positVide = sPosition.find("|") ;  //chaîne vide  int     nbCoords = 0 ;
  vector<string> Coordonnees ;  //contient les coordonnées sous forme de string
  while (positVide != string::npos)  {    Coordonnees.push_back(string(sPosition, debut, positVide - debut)) ;    nbCoords++ ;    debut = positVide + 1 ;    positVide = sPosition.find("|", debut) ;  }
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
