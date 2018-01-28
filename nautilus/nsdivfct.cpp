//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVFCT.C
//
//                     Pot-pourri de fonctions
//
//             Début d'écriture : 30 Juin 1988 (PA)
//             Dernière version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#include <owl\applicat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dos.h>
#include <ctype.h>

#include "nsglobal.h"
#include "nsdivfct.h"


//---------------------------------------------------------------------------
//  Function: DonneNomFichierDOS(const char *ammorce, const char* suffixe)
//
//  Arguments:
//            ammorce  : premiers caractères du préfixe du nom DOS
//				  suffixe  : suffixe du nom de fichier DOS
//				  chemin   : répertoire où chercher
//				  nomLibre : nom du fichier trouvé sous forme PPPPPPPP.SSS
//  Description:
//            Cherche le premier nom de fichier libre
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
/*unsigned int DonneNomFichierDOS(const char *ammorce, const char* suffixe,
									  const char *chemin, char** nomLibre)
{
	int 		i, j, tailleAmmorce, calculer;
	char 		prefixe[9], numero[9], numeroRef[9], *fichRecherche;
	struct 	find_t stRecherche;
	//
	// Préparation d'une ammorce générique du type 100001??.RTF
	//
	tailleAmmorce = strlen(ammorce);
	if (tailleAmmorce > 7) return 1;
	for (i = 0; i < tailleAmmorce; i++)
		prefixe[i] = ammorce[i];
	for (; i < 8; i++)
		prefixe[i] = '?';
	prefixe[i] = '\0';
	//
	// Préparation du numéro de référence
	//
	for (i = 0; i < 8 - tailleAmmorce; i++)
		numeroRef[i] = '0';
	numeroRef[i-1] = '1';
	numeroRef[i] = '\0';
	//
	// Préparation du nom de fichier complet
	//
	fichRecherche = new char[strlen(chemin)+14];
	strcpy(fichRecherche, chemin);
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(fichRecherche, "\\");
	strcat(fichRecherche, prefixe);
	strcat(fichRecherche, ".");
	strcat(fichRecherche, suffixe);
	//
	// Lancement de la recherche du premier fichier correspondant
	//                  FindFirstFile
	i = _dos_findfirst(fichRecherche, _A_NORMAL | _A_RDONLY | _A_ARCH,
							&stRecherche);
	delete fichRecherche;
	//
	// Si le fichier n'existe pas on renvoie le numéro de référence
	//
	if (i != 0)
	{
		*nomLibre = new char[strlen(chemin)+14];
		strcpy(*nomLibre, chemin);
		if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
			strcat(*nomLibre, "\\");
		strcat(*nomLibre, ammorce);
		strcat(*nomLibre, numeroRef);
		strcat(*nomLibre, ".");
		strcat(*nomLibre, suffixe);
		return(0);
	}
	//
	// Traitement du fichier trouvé (récupération de son numéro)
	//
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.name, numero);
	if (i == 1) return(1);
	//
	// Boucle tant qu'on ne trouve pas un numéro différent de la référence
	//
	while (strcmp(numero, numeroRef) == 0)
	{
		//
		// On incrémente la référence
		//
		calculer = 1;
		i = strlen(numeroRef) - 1;
		while (calculer) {
				j = (int) numeroRef[i];
				j++;
				if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
				{
					numeroRef[i] = (char) j;
					calculer = 0;
				}
				else if ((j > '9') && (j < 'A'))
				{
					numeroRef[i] = 'A';
					calculer = 0;
				}
				else
				{
					numeroRef[i] = '0';
					i--;
				}
		}
		//
		// On appelle le fichier suivant et on le traite
		//
		i = _dos_findnext(&stRecherche);
		if (i != 0)
			numero[0] = '\0';
		else
		{
			i = DonneNumeroFichier(ammorce, suffixe, stRecherche.name, numero);
			if (i != 0)
			numero[0] = '\0';
		}
	}
	//
	// On renvoie le numéro de référence
	//
	*nomLibre = new char[strlen(chemin)+14];
	strcpy(*nomLibre, chemin);
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(*nomLibre, "\\");
	strcat(*nomLibre, ammorce);
	strcat(*nomLibre, numeroRef);
	strcat(*nomLibre, ".");
	strcat(*nomLibre, suffixe);
	return(0);
} */

//---------------------------------------------------------------------------
//  Function: DonneNomFichier(const char *ammorce, const char* suffixe)
//
//  Arguments:
//            ammorce  : premiers caractères du préfixe du nom DOS
//				  suffixe  : suffixe du nom de fichier DOS
//				  chemin   : répertoire où chercher
//				  nomLibre : nom du fichier trouvé sous forme PPPPPPPP.SSS
//  Description:
//            Cherche le premier nom de fichier libre
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
unsigned int DonneNomFichier(const char *ammorce, const char* suffixe,
									  				const char *chemin, char** nomLibre)
{
	int 				 i, j, tailleAmmorce, calculer;
	char 				 prefixe[9], numero[9], numeroRef[9], *fichRecherche;
	WIN32_FIND_DATA stRecherche;
   HANDLE			 hFichierTrouve;
	//
	// Préparation d'une ammorce générique du type 100001??.RTF
	//
	tailleAmmorce = strlen(ammorce);
	if (tailleAmmorce > 7) return 1;
	for (i = 0; i < tailleAmmorce; i++)
		prefixe[i] = ammorce[i];
	for (; i < 8; i++)
		prefixe[i] = '?';
	prefixe[i] = '\0';
	//
	// Préparation du numéro de référence
	//
	for (i = 0; i < 8 - tailleAmmorce; i++)
		numeroRef[i] = '0';
	numeroRef[i-1] = '1';
	numeroRef[i] = '\0';
	//
	// Préparation du nom de fichier complet
	//
	fichRecherche = new char[strlen(chemin)+14];
	strcpy(fichRecherche, chemin);
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(fichRecherche, "\\");
	strcat(fichRecherche, prefixe);
	strcat(fichRecherche, ".");
	strcat(fichRecherche, suffixe);
	//
	// Lancement de la recherche du premier fichier correspondant
	//                  FindFirstFile
	hFichierTrouve = FindFirstFile(fichRecherche, &stRecherche);
	delete fichRecherche;
	//
	// Si le fichier n'existe pas on renvoie le numéro de référence
	//
	if (hFichierTrouve == INVALID_HANDLE_VALUE)
	{
		*nomLibre = new char[strlen(chemin)+14];
		strcpy(*nomLibre, chemin);
		if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
			strcat(*nomLibre, "\\");
		strcat(*nomLibre, ammorce);
		strcat(*nomLibre, numeroRef);
		strcat(*nomLibre, ".");
		strcat(*nomLibre, suffixe);
		return(0);
	}
	//
	// Traitement du fichier trouvé (récupération de son numéro)
	//
	i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero);
	if (i == 1) return(1);
	//
	// Boucle tant qu'on ne trouve pas un numéro différent de la référence
	//
	while (strcmp(numero, numeroRef) == 0)
	{
		//
		// On incrémente la référence
		//
		calculer = 1;
		i = strlen(numeroRef) - 1;
		while (calculer) {
				j = (int) numeroRef[i];
				j++;
				if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))
				{
					numeroRef[i] = (char) j;
					calculer = 0;
				}
				else if ((j > '9') && (j < 'A'))
				{
					numeroRef[i] = 'A';
					calculer = 0;
				}
				else
				{
					numeroRef[i] = '0';
					i--;
				}
		}
		//
		// On appelle le fichier suivant et on le traite
		//
      BOOL bTrouve;
		bTrouve = FindNextFile(hFichierTrouve, &stRecherche);
		if (!bTrouve)
			numero[0] = '\0';
		else
		{
			i = DonneNumeroFichier(ammorce, suffixe, stRecherche.cFileName, numero);
			if (i != 0)
			numero[0] = '\0';
		}
	}
	//
	// On renvoie le numéro de référence
	//
	*nomLibre = new char[strlen(chemin)+14];
	strcpy(*nomLibre, chemin);
	if ((chemin[0] != '\0') && (chemin[strlen(chemin)-1] != '\\'))
		strcat(*nomLibre, "\\");
	strcat(*nomLibre, ammorce);
	strcat(*nomLibre, numeroRef);
	strcat(*nomLibre, ".");
	strcat(*nomLibre, suffixe);
	return(0);
}

//---------------------------------------------------------------------------
//  Function: DonneNumeroFichier
//
//  Arguments:
//            ammorce  			: premiers caractères du préfixe du nom DOS
//				  suffixe  			: suffixe du nom de fichier DOS
//				  nomFichier  		: nom de fichier DOS
//				  numeroFichier 	: numéro du fichier
//  Description:
//            Extrait le numéro du nom de fichier
//				  Exemple : ammorce = 100125 suffixe = RTF
//								nomFichier = 10012522.RTF -> numeroFichier = 22
//  Returns:
//            0 si OK, 1 si le nom n'est pas cohérent avec ammorce et suffixe
//---------------------------------------------------------------------------
unsigned int DonneNumeroFichier(const char *ammorce, const char* suffixe,
										  const char *nomFichier, char* numeroFichier)
{
	int i, j;
	//
	// Vérification de la conformité par rapport à l'ammorce
	//
	for (i = 0; i < strlen(ammorce); i++)
		if (nomFichier[i] != ammorce[i])
			return(1);
	//
	// Prise du numéro qui suit l'ammorce
	//
	for (j = 0; (nomFichier[i] != '.') && (nomFichier[i] != '\0'); i++, j++)
		numeroFichier[j] = nomFichier[i];
	numeroFichier[j] = '\0';
	//
	// Vérification de la conformité par rapport au suffixe
	//
	if (nomFichier[i] == '\0')
	{
		if (suffixe[0] != '\0')
			return(1);
	}
	else
	{
		i++;
		if ((suffixe[0] == '\0') && (nomFichier[i] != '\0'))
			return(1);
		for (j = 0; (j < strlen(suffixe)) && (nomFichier[i] != '\0'); i++, j++)
			if (nomFichier[i] != suffixe[j])
				return(1);
	}
	return(0);
}

// void numacar(mot, nbre, taille)
//---------------------------------------------------------------------------
//  Fonction : Transforme un NOMBRE en CARACTERE
//	 Retour	 : Rien
//  Créé le 02/08/1989  Dernière mise à jour le 28/07/1994
void numacar(char* mot, unsigned int nbre, unsigned int taille)
{
	unsigned int i, multi;
	if (taille > 5) taille = 5;
	multi = 1;
	for (i = 1; i < taille; i++) multi = multi * 10;
	for (i = taille; i > 0; i--, multi = multi / 10)
	{
		if (nbre < multi) mot[taille-i] = '0';
		else
		{
			mot[taille-i] = char(nbre / multi) + '0';
			nbre -= multi * (nbre / multi);
		}
	}
	mot[taille] = '\0';
	return;
}

// int donneDigit(char nbre)
//---------------------------------------------------------------------------
//  Fonction : Transforme un caractère type '1', '2' en int
//	 Retour	 : -1 si impossible
//					un entier de 0 à 9 sinon
//  Créé le 02/08/1989  Dernière mise à jour le 28/07/1994
int donneDigit(char nbre)
{
	if (!isdigit(nbre))
		return -1;
	return int(nbre) - int('0');
}

// int ote_blancs(mot)
//---------------------------------------------------------------------------
//  Fonction : Ote les blancs à droite et à gauche de mot[]
//	 Retour	 : Rien
//  Créé le 13/10/1988  Dernière mise à jour le 11/06/1990
int ote_blancs(char* mot)
{
  unsigned int i;
  //
  //
  // on ôte les blancs finaux                                                                     */
  for (i = strlen(mot); (mot[i-1] == ' ') && (i > 0); i--);
  if (i == 0) { mot[0] = '\0'; return(0); }
  else        mot[i] = '\0';
  //
  // On ôte les blancs initiaux
  //
  while (mot[0] == ' ')
		  for (i = 0; mot[i] != '\0'; i++)
				mot[i] = mot[i+1];
  return(0);
}

// int egalite(mot1, mot2)
//---------------------------------------------------------------------------
//  Fonction : Renvoie l'indice de difference
//	 Retour	 : Rien
//  Créé le 13/10/1988  Dernière mise à jour le 28/07/1994
int egalite(char *mot1, char *mot2)
{
  int i, taille1, taille2;

  //  On ôte les blancs terminaux
  //
  for (taille1 = strlen(mot1); mot1[taille1-1] == ' '; taille1--);
  for (taille2 = strlen(mot2); mot2[taille2-1] == ' '; taille2--);
  //
  //  On compare mot1 et mot2
  //
  if (taille1 <= taille2)
  {
	  for (i = 0; i < taille1; i++)
			if (pseumaj(mot1[i]) != pseumaj(mot2[i])) return(i+1);
	  return(taille1+1);
  }
  else
  {
	  for (i = 0; i < taille2; i++)
			if (pseumaj(mot1[i]) != pseumaj(mot2[i])) return(i+1);
	  return(taille2+1);
  }
}

// -----------------------------------------------------------------------------// unsigned int pseumaj(lettre)// -----------------------------------------------------------------------------
// Fonction : Renvoie la pseudo-majuscule
// Retour   : Rien
// -----------------------------------------------------------------------------
// Créé le 13/10/1988  Dernière mise à jour le 28/07/1994
// -----------------------------------------------------------------------------
char pseumaj(char lettre)
{
	if ((lettre >= 'a') && (lettre <= 'z'))
    return char(lettre - 32) ;
	switch (lettre)
	{
		case (unsigned) 'à' :
		case (unsigned) 'â' :
		case (unsigned) 'ä' : return ('A') ;
		case (unsigned) 'é' :
		case (unsigned) 'è' :
		case (unsigned) 'ê' :
		case (unsigned) 'ë' : return ('E') ;
		case (unsigned) 'î' :
		case (unsigned) 'ï' : return ('I') ;
		case (unsigned) 'ö' :
		case (unsigned) 'ô' : return ('O') ;
		case (unsigned) 'ù' :
		case (unsigned) 'ü' :
		case (unsigned) 'û' : return ('U') ;
		case (unsigned) 'ç' : return ('C') ;
	}
	return (lettre) ;
}


// -----------------------------------------------------------------------------// donne_date_duJour(date)// -----------------------------------------------------------------------------
// Fonction : Donne la date du jour au format AAAAMMJJ
// Retour	 : Rien
// Créé le 08/12/1994  Dernière mise à jour le 08/12/1994
// Modified by fab BBpowered on 2003-04-03
// -----------------------------------------------------------------------------
void donne_date_duJour(char* date)
{
/*
	int   	i;
	struct tm *tp;
	long	t;
	char mois[12][4]; // = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	strcpy(mois[0], "Jan");
	strcpy(mois[1], "Feb");
	strcpy(mois[2], "Mar");
	strcpy(mois[3], "Apr");
	strcpy(mois[4], "May");
	strcpy(mois[5], "Jun");
	strcpy(mois[6], "Jul");
	strcpy(mois[7], "Aug");
	strcpy(mois[8], "Sep");
	strcpy(mois[9], "Oct");
	strcpy(mois[10],"Nov");
	strcpy(mois[11],"Dec");
	char temp[26], month[4], numero[3] = "  ";
	//
	// Initialisation de date
	//
	strcpy(date, "");
	//
	// Prise de temp
	//
	time(&t);
	tp = localtime(&t);
	strcpy(temp, asctime(tp));
	//
	// Récupération de l'année
	//
	for(i = 20; i < 24; i++)
		date[i-20] = temp[i];
	date[4] ='\0';
	//
	// Récupération du mois
	//
	for (i = 4; i < 7; i++) month[i-4] = temp[i];
	month[3] = '\0';
	for (i = 0; (i < 12) && (strcmp(month, mois[i]) != 0); i++);
	i++;
	if (i < 10)
	{
		numero[0] = '0';
		numero[1] = char (i + (int) '0');
	}
	else if (i < 13)
	{
		numero[0] = '1';
		numero[1] = char ((i - 10) + (int) '0');
	}
	strcat(date, numero);
	//
	// Récupération du jour
	//
	for (i = 8; i < 10; i++)
		numero[i-8] = temp[i];
	numero[2] = '\0';
	strcat(date, numero);
	return;
*/
	// ---------------------------------------------------------------------------
	// fab version
	// ---------------------------------------------------------------------------

	// get the current date
	long 				clock = time(0) ;
	struct tm		*now = localtime(&clock) ;

	// place it in the char *
	sprintf(date, "%4d%2d%2d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday) ;

	// replace space caracter by '0' caracter (example "2003 4 3" by "20030403")
	for (int i = 0 ; (i < strlen(date)) && (date[i] != 0) ; i++)
		if (date[i] == ' ')
			date[i] = '0' ;

	return ;
	// ---------------------------------------------------------------------------
}


//  donne_date(date, message, pays)
//---------------------------------------------------------------------------
//  Fonction : Transforme une date AAAAMMJJ en un message JJ/MM/AAAA
//	 Retour	 : Rien
//  Créé le 12/04/1991  Dernière mise à jour le 12/04/1991
void donne_date(char* date, char* message, unsigned int pays)
{
  unsigned int i;
  strcpy(message, "  /  /    ");
  message[0] = date[6]; message[1] = date[7];
  message[3] = date[4]; message[4] = date[5];
  message[6] = date[0]; message[7] = date[1];
  message[8] = date[2]; message[9] = date[3];
  return;
}

//  donne_date_claire(date, message, pays)
//---------------------------------------------------------------------------
//  Fonction : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
//	 Retour	 : Rien
//  Créé le 30/08/1994  Dernière mise à jour le 30/08/1994
void donne_date_claire(char* date, string *message, unsigned int pays)
{
	unsigned int i;
	char 			 ptgag[5];
	*message = "";
	if (strlen(date) < 8)
		return;
	//
	// Jour
	//
	if (isdigit(date[6]) && isdigit(date[7]))
	{
		if (date[6] == '0')
		{
			if (date[7] == '1')
				*message = "1er ";
			else if (date[7] != '0')
			{
				*message = date[7];
				*message += "";
			}
		}
		else
		{
			*message = date[6];
			*message += date[7];
			*message += " ";
		}
	}
	else
		*message = "?? ";
	//
	// Mois
	//
	if (isdigit(date[4]) && isdigit(date[5]))
		i = 10 * ((int) date[4] - (int) '0') + (int) date[5] - (int) '0';
	else
		i = 13;
	switch (i)
	{
		case  0 : *message = "";   	  	 	break;
		case  1 : *message += "janvier ";   break;
		case  2 : *message += "février ";   break;
		case  3 : *message += "mars ";      break;
		case  4 : *message += "avril ";     break;
		case  5 : *message += "mai ";       break;
		case  6 : *message += "juin ";      break;
		case  7 : *message += "juillet ";   break;
		case  8 : *message += "août ";      break;
		case  9 : *message += "septembre "; break;
		case 10 : *message += "octobre ";   break;
		case 11 : *message += "novembre ";  break;
		case 12 : *message += "décembre ";  break;
		default : *message = "?? ";
	}
	//
	// Année
	//
	for (i = 0; i < 4; i++)
	{
		if (isdigit(date[i]))
			*message += date[i];
		else
			*message += '?';
	}
	return;
}

//  donne_date_claire(date, message, pays)
//---------------------------------------------------------------------------
//  Fonction : Transforme une date AAAAMMJJ en un message "1er janvier 1994"
//	 Retour	 : Rien
//  Créé le 30/08/1994  Dernière mise à jour le 30/08/1994
void donne_date_claire(char* date, char* message, unsigned int pays)
{
	unsigned int i;
	char 			 ptgag[5];
	message[0] = '\0';
	//
	// Jour
	//
	if (isdigit(date[6]) && isdigit(date[7]))
	{
		if (date[6] == '0')
		{
			if (date[7] == '1')
				strcpy(message, "1er ");
			else if (date[7] != '0')
			{
				message[0] = date[7]; message[1] = ' '; message[2] = '\0';
			}
		}
		else
		{
			message[0] = date[6]; message[1] = date[7];
			message[2] = ' '; 	 message[3] = '\0';
		}
	}
	else
		strcpy(message, "?? ");
	//
	// Mois
	//
	if (isdigit(date[4]) && isdigit(date[5]))
		i = 10 * ((int) date[4] - (int) '0') + (int) date[5] - (int) '0';
	else
		i = 13;
	switch (i)
	{
		case  0 : strcpy(message, "");   			 break;
		case  1 : strcat(message, "janvier ");   break;
		case  2 : strcat(message, "février ");   break;
		case  3 : strcat(message, "mars ");      break;
		case  4 : strcat(message, "avril ");     break;
		case  5 : strcat(message, "mai ");       break;
		case  6 : strcat(message, "juin ");      break;
		case  7 : strcat(message, "juillet ");   break;
		case  8 : strcat(message, "août ");      break;
		case  9 : strcat(message, "septembre "); break;
		case 10 : strcat(message, "octobre ");   break;
		case 11 : strcat(message, "novembre ");  break;
		case 12 : strcat(message, "décembre ");  break;
		default : strcpy(message, "?? ");
	}
	//
	// Année
	//
	for (i = 0; i < 4; i++)
	{
		if (isdigit(date[i]))
			carcat(message, date[i]);
		else
			carcat(message, '?');
	}
	return;
}

//  donne_somme_courte(somme, message, pays)
//---------------------------------------------------------------------------
//  Fonction : Transforme une somme XXXXXXXX en un message XXXXXX,XX
//	 Retour	 : Rien
//  Créé le 12/04/1991  Dernière mise à jour le 12/04/1991
void donne_somme_courte(char *somme, char *message, unsigned int pays)
{
  unsigned int i;
  strcpy(message, "000000,00");
  for (i = 0; i < 6; i++) message[i]   = somme[i];
  for (i = 6; i < 8; i++) message[i+1] = somme[i];
  return;
}

//  void long_to_hex(unsigned long nb, char nb_hex[])
//---------------------------------------------------------------------------
//  Fonction : Convertit un entier long en hexa
//  Entrée 	 : nb   	 -> l'entier à convertir
//            	nb_hex -> l'adresse de la chaîne
//	 Retour	 : Rien
//  Créé le 12/04/1991  Dernière mise à jour le 12/04/1991
void long_to_hex(unsigned long nb, char* nb_hex)
{
  unsigned long result, mask = 0xF;
  unsigned short i;
  char hex_car[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
  nb_hex[8] = '\0';
  for (i = 0; i < 8; i++){
		result = nb & mask;
		mask   = mask << 4;
		result = result >> (i << 2);
		nb_hex[7-i] = hex_car[result];
  }
  return;
}

//  void hex_to_long(char nb_hex[], unsigned long *nb)
//---------------------------------------------------------------------------
//  Fonction : Transforme un HHHHHHHH en unsigned long
//  Entrée 	 : nb_hex -> la chaine contenant l'hexa
//            	nb 	 -> pointeur sur le résultat
//	 Retour	 : Rien
//  Créé le 17/09/1992  Dernière mise à jour le 07/10/1992
void hex_to_long(char nb_hex[], unsigned long *nb)
{
  signed   int  i;
  unsigned int  chiffre;
  unsigned long multi;
  *nb = 0;
  if (strlen(nb_hex) != 8) return;
  for (i = 0; i < 8; i++) {
		*nb = *nb * 16;
		if ((nb_hex[i] >= '0') && (nb_hex[i] <= '9')) *nb += (unsigned long) nb_hex[i] - 48;
		if ((nb_hex[i] >= 'A') && (nb_hex[i] <= 'F')) *nb += (unsigned long) nb_hex[i] - 55;
  }
  return;
}

/*void format_date(char date[])
//              Retourne une date au format AAAAMMJJmmss
//                 à partir du format de asctime().
//
//   ENTREE : adresse de la chaîne dans laquelle on désire placer
//            la date.
//   Créé le 17/09/1992  Dernière mise à jour le 07/10/1992
{
  char mois[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
							  "Sep", "Oct", "Nov", "Dec" };
  int i;
  struct tm *tp;
  long t;
  char temp[26], month[4], numero[3] = "  ";

  // Prise de temp
  time(&t);
  tp = localtime(&t);
  strcpy(temp, asctime(tp));

  // Récupération de l'année
  //
  for(i = 20; i < 24; i++) date[i-20] = temp[i];
  date[4] ='\0';

  // Récupération du mois
  //
  for (i = 4; i < 7; i++) month[i-4] = temp[i];
  month[3] = '\0';
  for (i = 0; (i < 12) && (strcmp(month, mois[i]) != 0); i++);
  numacar(numero, i+1, 2);
  strcat(date, numero);

  // Récupération du jour
  //
  for (i = 8; i < 10; i++) numero[i-8] = temp[i];
  numero[2] = '\0';
  strcat(date, numero);

  // Récupération de l'heure
  //
  for (i = 11; i < 13; i++) numero[i-11] = temp[i];
  numero[2] = '\0';
  strcat(date, numero);

  // Récupération des minutes
  //
  for (i = 14; i < 16; i++) numero[i-14] = temp[i];
  numero[2] = '\0';
  strcat(date, numero);

  // Récupération des secondes
  //
  for (i = 17; i < 19; i++) numero[i-17] = temp[i];
  numero[2] = '\0';
  strcat(date, numero);
  return;
} */

//  void carcat(unsigned char *chaine, unsigned char caractere)
//---------------------------------------------------------------------------
//  Fonction : équivalent de strcat, en ajoute un char en fin de chaîne
//	 Retour	 : Rien
//  Créé le 15/06/1994  Dernière mise à jour le 15/06/1994
void carcat(char* chaine, char caractere)
{
  char ptgag[2];
  ptgag[0] = caractere; ptgag[1] = '\0';
  strcat(chaine, ptgag);
  return;
}

//---------------------------------------------------------------------------
//  Function: SetInternational(void)
//
//  Arguments:
//            AUCUN
//  Description:
//            Va chercher dans WINI.INI les formats d'affichage des dates
//            heure, ...
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
void SetInternational(void)
{
  static char cName [] = "intl" ;

  iDate = GetProfileInt (cName, "iDate", 0) ;
  iTime = GetProfileInt (cName, "iTime", 0) ;

  GetProfileString (cName, "sDate",	"/", sDate,	2) ;
  GetProfileString (cName, "sTime",	":", sTime,	2) ;
  GetProfileString (cName, "s1159", "AM", sAMPM[0], 5) ;
  GetProfileString (cName, "s2359", "PM", sAMPM[1], 5) ;
}

//---------------------------------------------------------------------------
//  Function:  ptrInit(char* &strPtr, unsigned int length)
//
//  Arguments:
//             strPtr   ->  pointeur sur une chaîne qu'il faut initialiser
//             length   ->  longueur de la chaîne
//  Description:
//             Initialise un pointeur sur une chaîne (char *)
//             Alloue un espace de longueur length
//             Initialise la chaîne des blancs.
//  Returns:
//             TRUE   si l'initialisation s'est passée sans pb.
//             FALSE  sinon
//  Exemple :  ptrInit(&Essai, 5, '$');
//---------------------------------------------------------------------------
BOOL ptrInit(char** strPtr, unsigned int length, char car)
{
 *strPtr = new char[length + 1];
 if (*strPtr == NULL)
	 return FALSE;
 else
 {
	 memset(*strPtr, car, length);
	 *(*strPtr + length) = '\0';
 }
 return TRUE;
}

//---------------------------------------------------------------------------
//  Function:  ptrInit(char* &strPtr,const char* strInit)
//
//  Arguments:
//             strPtr   ->  pointeur sur une chaîne qu'il faut initialiser
//             strInit  ->  chaîne d'initialisation ou NULL si l'on désire
//                          des blancs
//  Description:
//             Initialise un pointeur sur une chaîne (char *)
//             Initialise la chaîne avec strInit.
//  Returns:
//             TRUE   si l'initialisation s'est passée sans pb.
//             FALSE  sinon
//  Exemple :  ptrInit(&Essai, "AAAAA");
//---------------------------------------------------------------------------
BOOL ptrInit(char** strPtr, const char* strInit)
{
 *strPtr = new char[strlen(strInit) + 1];
 if (*strPtr == NULL)
	 return FALSE;
 else
	 strcpy(*strPtr, strInit);
 return TRUE;
}

BOOL ptrInit(unsigned char** strPtr, const unsigned char* strInit)
{
	return ptrInit((char**)strPtr, (char*)strInit);
}

//---------------------------------------------------------------------------
//  Function: erreur(Pchar texte, int gravite, Retcode ErrPx)
//
//  Arguments:
//            texte   -> texte affiché en cas d'erreur
//            gravité -> niveau de gravité
//
//  Description:
//            Affiche les messages d'erreur et sort si le niveau de
//            gravité est 1.
//  Returns:
//            Retourne 0 si le problème rencontré est surmontable.
//---------------------------------------------------------------------------
int erreur(const char far *texte, int gravite, DBIResult ErrPx)
{

  	char buffer[2048];

  	if ((texte[0] == '\0') && (ErrPx == DBIERR_NONE))
   	return(0);

  	if (texte[0] != '\0')
	  MessageBox(0, texte, "Erreur NAUTILUS", MB_ICONEXCLAMATION);

  	if ((ErrPx != 0) && (ErrPx != DBIERR_NONE))
   {
  		char        dbi_status[DBIMAXMSGLEN * 5] = {'\0'};
   	DBIMSG      dbi_string = {'\0'};
   	DBIErrInfo  ErrInfo;

   	if (ErrPx != DBIERR_NONE)
   	{
      	DbiGetErrorInfo(TRUE, &ErrInfo);

      	if (ErrInfo.iError == ErrPx)
      	{
         	wsprintf(dbi_status, "  ERROR %s", ErrInfo.szErrCode);

         	if (strcmp(ErrInfo.szContext1, ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext1);

         	if (strcmp(ErrInfo.szContext2, ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext2);
         	if (strcmp(ErrInfo.szContext3, ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext3);
         	if (strcmp(ErrInfo.szContext4, ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext4);
      	}
      	else
      	{
         	DbiGetErrorString(ErrPx, dbi_string);
         	wsprintf(dbi_status, "  ERROR %s", dbi_string);
      	}
      	MessageBox(NULL, dbi_status, "BDE Error", MB_OK | MB_ICONEXCLAMATION);
   	}
   	//return ErrorValue;
   }

  	if (gravite == 1) exit(1);

  	return 0;
}