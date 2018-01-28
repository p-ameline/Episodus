//-------------------------------------------------------------------------
//                    BIBLIOTHEQUE : NSDIVFCT.C
//
//                     Pot-pourri de fonctions
//
//             Début d'écriture : 30 Juin 1988 (PA)
//             Dernière version : 28 Juillet 1994 (PA)
//-------------------------------------------------------------------------
#include "partage\nsglobal.h"

#include <owl\applicat.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>
#include <time.h>
#include <dos.h>
#include <ctype.h>
#include <math.h>
#include <string.stl>
#include <sysutils.hpp>		// pour FileExists de nom_sans_doublons

#include "partage\nsdivfct.h"

static bool bModeFile = false;
const char cheminSeparationMARK = '/';

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

// void strip(string& s, int n)
// --------------------------------------------------------------------------
// Fonction : enleve les blancs d'une string
// Retour : rien
void strip(string& s, int n, char c)
{
    int pos1, pos2;

    if (s == "")
        return;

    switch (n)
    {
        case stripBoth :
            pos1 = s.find_first_not_of(c);
            pos2 = s.find_last_not_of(c);

            if ((pos1 != NPOS) && (pos2 != NPOS))
                s = string(s, pos1, pos2-pos1 + 1);
            break;

        case stripLeft :
            pos1 = s.find_first_not_of(c);

            if (pos1 != NPOS)
                s = string(s, pos1, strlen(s.c_str()) - pos1);
            break;

        case stripRight :
            pos2 = s.find_last_not_of(c);

            if (pos2 != NPOS)
                s = string(s, 0, pos2+1);
            break;

        default :
            erreur("Fonction strip : option erronnée", 0, 0);
    }
}

void incremente_code(string& code)
{
   bool tourner = true;
   int  i = strlen(code.c_str());

   while ((tourner) && (i > 0) )
   {
      i--;
   	if (((code[i] >= '0') && (code[i] < '9')) ||
      	 ((code[i] >= 'A') && (code[i] < 'Z')))
      {
	      code[i] = code[i] + 1;
         tourner = false;
      }
      else if (code[i] == '9')
      {
	      code[i] = 'A';
         tourner = false;
      }
      else if (code[i] == 'Z')
	      code[i] = '0';
   }

   if (tourner)	// dans ce cas le code ne contient que des zéros
   	code = string("1") + code;
}

string nom_sans_doublons(string serveur, string unite, string pathRef, string nom, string ext)
{
	string sCompteur = "";
    bool exist = true;
	string nomComplet;
    string sNomFichier;

   // Attention pathRef est ici un chemin Relatif
   // et non un chemin absolu comme dans NSModHtml

   while (exist)
   {
   	  incremente_code(sCompteur);
      nomComplet = nom + sCompteur;
      if (serveur != "")
        sNomFichier = serveur + pathRef + nomComplet + string(".") + ext;
      else
        sNomFichier = unite + pathRef + nomComplet + string(".") + ext;
      exist = FileExists(AnsiString(sNomFichier.c_str()));
   }

   // tant que le compteur ne dépasse pas ZZ on obtiendra un nom dos à 8 + 3
   if (nomComplet != "")
   	nomComplet = nomComplet + string(".") + ext;

   return nomComplet;
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

char donneChar(int digit)
{
	if ((digit >= 0) && (digit < 10))
   	return char(int('0') + digit);
   else
   	return '*';
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

// unsigned int pseumaj(lettre)
//---------------------------------------------------------------------------
//  Fonction : Renvoie la pseudo-majuscule
//	 Retour	 : Rien
//  Créé le 13/10/1988  Dernière mise à jour le 28/07/1994
char pseumaj(char lettre)
{
	if ((lettre >= 'a') && (lettre <= 'z')) return char(lettre - 32);
	switch (lettre)
	{
		case (unsigned) 'à' :
		case (unsigned) 'â' :
		case (unsigned) 'ä' : return('A');
		case (unsigned) 'é' :
		case (unsigned) 'è' :
		case (unsigned) 'ê' :
		case (unsigned) 'ë' : return('E');
		case (unsigned) 'î' :
		case (unsigned) 'ï' : return('I');
		case (unsigned) 'ö' :
		case (unsigned) 'ô' : return('O');
		case (unsigned) 'ù' :
		case (unsigned) 'ü' :
		case (unsigned) 'û' : return('U');
		case (unsigned) 'ç' : return('C');
	}
	return(lettre);
}

// void pseumaj(char* chaine)
//---------------------------------------------------------------------------
//  Fonction : Convertit la chaine en pseudo-majuscules
//	 Retour	 : Rien
//  Créé le 6/10/1999
void pseumaj(char* chaine)
{
    for (int i = 0; i < strlen(chaine); i++)
    {
        chaine[i] = pseumaj(chaine[i]);
    }
}

//  donne_heure(char* pHeure)
//---------------------------------------------------------------------------
//  Fonction : Donne l'heure au format hhmmss
//	 Retour	 : Rien
//  Créé le 26/05/1998  Dernière mise à jour le 26/05/1998
void donne_heure(char* heure)
{
	strcpy(heure, "hhmmss");
   //
	// Prise de temp
	//
   struct tm *tp;
	long	 t;
   char 	 temp[26];
	time(&t);
	tp = localtime(&t);
	strcpy(temp, asctime(tp));
   //
   // asctime renvoit la date au format :
   // Sun Sep 16 01:03:52 1973\n\0
   //
   int i, j;
   for (i = 0, j = 0; i < 8; i++, j++)
   {
   	heure[j] = temp[i+11];
      if ((i == 1) || (i == 4))
      	i++;
   }
}

//  donne_date_duJour(date)
//---------------------------------------------------------------------------
//  Fonction : Donne la date du jour au format AAAAMMJJ
//	 Retour	 : Rien
//  Créé le 08/12/1994  Dernière mise à jour le 08/12/1994
void donne_date_duJour(char* date)
{
	int   	i;
	struct tm *tp;
	long	t;
	char mois[12][4]; /*= { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug",
							  "Sep", "Oct", "Nov", "Dec" }; */
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
   // asctime renvoit la date au format :
   // Sun Sep 16 01:03:52 1973\n\0
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
}

//  donne_heure(char* pHeure)
//---------------------------------------------------------------------------
//  Fonction : Transforme une heure HHMMSS ou HHMM en string HH:MM:SS ou HH:MM
//	 Retour	 : Rien
//  Créé le 01/03/1999
void donne_heure(char* heure, string& message)
{
	message = "";

    message += heure[0];
    message += heure[1];
    message += ":";
    message += heure[2];
    message += heure[3];

    if (strlen(heure) > 4)
    {
    	message += ":";
        message += heure[4];
        message += heure[5];
    }
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
}

//  donne_date_inverse(date, message, pays)
//---------------------------------------------------------------------------
//  Fonction : Transforme une date JJ/MM/AAAA en un message AAAAMMJJ
//	 Retour	 : Rien
//  Créé le 12/01/1999
void donne_date_inverse(string date, string& message, unsigned int pays)
{
	message = string(8, ' ');

    message[0] = date[6]; message[1] = date[7];
    message[2] = date[8]; message[3] = date[9];
    message[4] = date[3]; message[5] = date[4];
    message[6] = date[0]; message[7] = date[1];
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
				*message += " ";
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

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void donne_date_breve(string sDate, string* pMessage, string* pIntro)
{
   *pMessage = "";
   *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//

   if (strlen(sDate.c_str()) < 8)
   	return;

      // Date vide
      if (sDate == "00000000")
      	return;
      // Juste AAAA
      else if (string(sDate,4, 4) == "0000")
      {
      	*pMessage = string(sDate,0, 4);
         *pIntro 	 = "en ";
      }
      // MM/AAAA
      else if (string(sDate,6, 2) == "00")
      {
      	*pMessage = donne_mois(string(sDate,4, 2)) + " " + string(sDate, 0, 4);
         *pIntro 	 = "en ";
      }
      // JJ/MM/AAAA
      else
      {
         *pIntro 	 = "le ";
      	*pMessage = string(sDate,6, 2) + "/" + string(sDate,4, 2) + "/" + string(sDate,0, 4);
      }

  	return;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "1er janvier 1994"            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void donne_date_claire(string sDate, string* pMessage, string* pIntro)
{
	unsigned int i;

   *pMessage = "";
   *pIntro 	 = "";
  	//
  	// Date au format AAAAMMJJ
	//
   if (strlen(sDate.c_str()) < 8)
   	return;

   *pIntro = "le ";
   //
   // Jour
   //
   if (isdigit(sDate[6]) && isdigit(sDate[7]))
   {
   	if (sDate[6] == '0')
      {
      	if (sDate[7] == '1')
         	*pMessage = "1er ";
         else if (sDate[7] != '0')
         {
         	*pMessage = sDate[7];
            *pMessage += " ";
         }
         else
         	*pIntro = "en ";
      }
      else
      {
      	*pMessage = sDate[6];
         *pMessage += sDate[7];
         *pMessage += " ";
      }
   }
   else
   	*pMessage = "?? ";
   //
   // Mois
   //
   string sMois = donne_mois(string(sDate, 4, 2));
   if (sMois != "")
   	*pMessage += sMois + " ";
   else
   	*pMessage = "?? ";

   //
   // Année
   //
   for (i = 0; i < 4; i++)
   {
		if (isdigit(sDate[i]))
      	*pMessage += sDate[i];
      else
      	*pMessage += '?';
   }

	return;
}

//  +-----------------------------------------------------------------+
//  ¦             Indique si l'année donnée est bissextile            ¦
//  +-----------------------------------------------------------------+
// Créé le 28/06/99
bool bissextile(int year)
{
    // Règle : Si l'année est divisible par 4 ET non divisible par 100
    //				OU divisible par 400 ==> elle est bissextile.

	if (((year%4 == 0) && (!(year%100 == 0))) || (year%400 == 0))
    	return true;
    else
        return false;
}

//  +-----------------------------------------------------------------+
//  ¦               Incrémente une date donnée de un jour             ¦
//  +-----------------------------------------------------------------+
// Créé le 28/06/99
void incremente_date(string& sDate)
{
    string sAnnee, sMois, sJour;
    int    iAnnee, iMois, iJour, iJourFin;
    char   cDate[9];

    //
  	// Date au format AAAAMMJJ
	//
    if (strlen(sDate.c_str()) < 8)
   	    return;

    sAnnee = string(sDate, 0, 4);
    sMois = string(sDate, 4, 2);
    sJour = string(sDate, 6, 2);

    iAnnee = atoi(sAnnee.c_str());
    iMois = atoi(sMois.c_str());
    iJour = atoi(sJour.c_str());

    switch (iMois)
    {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            if (iJour < 31)
                iJour++;
            else if (iJour == 31)
            {
                iJour = 1;
                if (iMois == 12)
                {
                    iMois = 1;
                    iAnnee++;
                }
                else
                    iMois++;
            }
            else
                erreur("Attention date invalide.",0,0);
            break;

        case 2:
            if (bissextile(iAnnee))
                iJourFin = 29;
            else
                iJourFin = 28;

            if (iJour < iJourFin)
                iJour++;
            else if (iJour == iJourFin)
            {
                iJour = 1;
                iMois = 3;
            }
            else
                erreur("Attention date invalide.",0,0);
            break;

        case 4:
        case 6:
        case 9:
        case 11:
            if (iJour < 30)
                iJour++;
            else if (iJour == 30)
            {
                iJour = 1;
                iMois++;
            }
            else
                erreur("Attention date invalide.",0,0);
            break;

        default:
            erreur("Attention date invalide.",0,0);
    }

    // on recompose la nouvelle date
    sprintf(cDate, "%4d%02d%02d", iAnnee, iMois, iJour);
    sDate = string(cDate);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string donne_mois(string mois)
{
   if (mois.length() < 2)
   	return "";
   if ((!(isdigit(mois[0]))) || (!(isdigit(mois[1]))))
   	return "";
	int i = 10 * donneDigit(mois[0]) + donneDigit(mois[1]);

   return donne_mois(i);
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir d'un int             ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string donne_mois(int iMois)
{
   if ((iMois < 1) || (iMois > 12))
   	return "";

	switch (iMois)
   {
   	case  1 : return "janvier";
		case  2 : return "février";
		case  3 : return "mars";
		case  4 : return "avril";
		case  5 : return "mai";
		case  6 : return "juin";
		case  7 : return "juillet";
		case  8 : return "août";
		case  9 : return "septembre";
		case 10 : return "octobre";
		case 11 : return "novembre";
		case 12 : return "décembre";
   }
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
int erreur(const char *texte, int gravite, DBIResult ErrPx, HWND parent)
{
    // NOTE : Si parent == NULL (cas par défaut)
    // La MessageBox ne sera pas modale !!!
    // Comme on ne peut accéder au contexte, il faut passer un GetHandle()
    // pour ce paramètre à l'appel de la fonction.
    // Attention GetMainWindow()->GetHandle() ne vaut pas NULL...

  	char buffer[2048];

  	if ((texte[0] == '\0') && (ErrPx == DBIERR_NONE))
   	return(0);

  	if (texte[0] != '\0')
    {
        if (getModeFileErr() == true)
            fileError(texte);
        else
	        MessageBox(parent, texte, "Erreur NAUTILUS", MB_ICONEXCLAMATION);
    }

  	if ((ErrPx != 0) && (ErrPx != DBIERR_NONE))
   {
    char        dbi_status[DBIMAXMSGLEN * 5] = {'\0'};
   	DBIMSG      dbi_string = {'\0'};
   	DBIErrInfo  ErrInfo;

   	if (ErrPx != DBIERR_NONE)
   	{
      	DbiGetErrorInfo(TRUE, ErrInfo);

      	if (ErrInfo.iError == ErrPx)
      	{
         	wsprintf(dbi_status, "  ERROR %s", ErrInfo.szErrCode);

         	if (strcmp(ErrInfo.szContext[0], ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[0]);

         	if (strcmp(ErrInfo.szContext[1], ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[1]);
         	if (strcmp(ErrInfo.szContext[2], ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[2]);
         	if (strcmp(ErrInfo.szContext[3], ""))
            	wsprintf(dbi_status, "%s\r\n    %s", dbi_status, ErrInfo.szContext[3]);
      	}
      	else
      	{
         	DbiGetErrorString(ErrPx, dbi_string);
         	wsprintf(dbi_status, "  ERROR %s", dbi_string);
      	}

        if (getModeFileErr() == true)
            fileError(dbi_status);
        else
      	    MessageBox(parent, dbi_status, "BDE Error", MB_OK | MB_ICONEXCLAMATION);
   	}
   	//return ErrorValue;
   }

  	if (gravite == 1) exit(1);

  	return 0;
}

// fonctions de gestion des messages d'erreur
bool getModeFileErr()
{
    return bModeFile;
}

void setModeFileErr(bool bMode)
{
    bModeFile = bMode;
}

void fileError(string sError)
{
    ifstream inFile;
    ofstream outFile;
    string line;
    string sLastError;

    inFile.open("ferror.log");
    if (inFile)
    {
   	    while (!inFile.eof())
        {
            getline(inFile,line);
            if (line != "")
                sLastError += line + "\n";
        }

        inFile.close();
    }
    else // cas fichier vide
        sLastError = "";

    // on ajoute la nouvelle erreur
    sLastError += sError + "\n";

    // On écrit le nouveau fichier reqerror.log
   	outFile.open("ferror.log");
	if (!outFile)
   	{
        // cas d'erreur bloquant...
        MessageBox(NULL, "Erreur d'ouverture en écriture du fichier ferror.log.", "Erreur NAUTILUS", MB_ICONEXCLAMATION);
   		return;
   	}

   	for (int i = 0; i < sLastError.length(); i++)
     	 outFile.put(sLastError[i]);

    outFile.close();
}

//--------------------------------------------------------------------------
// convertit  sValeur en un double
//--------------------------------------------------------------------------

double
StringToDouble(string sValeur)
{
	//
   // Extraction de la partie entière et de la partie décimale
   //
   string sEntier = "";
   string sDecima = "";
   size_t i = sValeur.find(".");
   if(i == string::npos)
   	i = sValeur.find(",");

   if ( (i == string::npos) || ( i > strlen(sValeur.c_str())))
   	sEntier = sValeur;
   else
   {
   	if (i > 0)
      	sEntier = string(sValeur, 0, i);
      sDecima = string(sValeur, i+1, strlen(sValeur.c_str())-i-1);
   }
   //
   // Calcul de la valeur
   //
   double dValeur = 0;
   // Partie entière
   for (i = 0; i < strlen(sEntier.c_str()); i++)
   {
      if (!isdigit(sEntier[i]))
         return 0;
   	dValeur = (dValeur * double(10)) + double(donneDigit(sEntier[i]));
   }
   // Partie décimale
   double multi = double(1);
   for (int i = 0; i < strlen(sDecima.c_str()); i++)
   {
   	if (!isdigit(sDecima[i]))
      	return 0;
      multi = multi * 0.1;
      dValeur += double(donneDigit(sDecima[i])) * multi;
   }

   return dValeur;
}

//-------------------------------------------------------------------
//convertit un double en une string
//-------------------------------------------------------------------
string
DoubleToString(double* pDouble, int iEntier, int iDecimal)
{
   double dVal = *pDouble;
   if(dVal == 0)
   	return "0";

   bool bNegatif = false; //double négatif
   if(dVal < 0)
   {
   	bNegatif = true;
      dVal = -dVal;
   }
	double dLog = log10(dVal);
   int	 iLog = int(dLog);
   double dMult = pow(double(10), double(iLog));
   double dResu;

   string sResult = "";
   //
   // Assemblage de la partie entière
   //
   while (iLog >= 0)
   {
   	dResu = floor(dVal / dMult);
      sResult += string(1, donneChar(int(dResu)));

      dVal = dVal - (dResu * dMult);
      dMult = dMult / 10;
      iLog--;
   }
   if ((iDecimal == 0) || ((iDecimal == -1) && (dVal == 0)))
   	return sResult;
   //
   // Assemblage de la partie entière
   //
   sResult += ".";
   iLog = 0;
   bool tourner = true;
   while (tourner)
   {
   	dResu = floor(10 * dVal);
      sResult += string(1, donneChar(int(dResu)));
      dVal = (10 * dVal) - dResu;
      iLog++;
      if ((iDecimal > 0) && (iLog >= iDecimal))
      	tourner = false;
      if ((iDecimal == -1) && ((iLog > 10) || (dVal == 0)))
      	tourner = false;
   }

   if(bNegatif)
   	sResult = "-" + sResult;
   return sResult;
}


//***************************************************************************
// 							Implémentation des mèthodes ClasseStringVector
//														vecteur de string
//***************************************************************************
ClasseStringVector::~ClasseStringVector()
{
	vider();
}


ClasseStringVector::ClasseStringVector()
                   :vectclassString()
{}

void
ClasseStringVector::vider()
{
   iterString i = begin();
   while(i != end())
   {
   	delete *i;
      erase(i);
   }
}


ClasseStringVector&
ClasseStringVector::operator=(ClasseStringVector src)
{
  vider();
  for (iterString i = src.begin(); i != src.end(); i++)
   	push_back(new classString(*(*i)));
  	return *this;
}


ClasseStringVector::ClasseStringVector(ClasseStringVector& src)
{
	for (iterString i = src.begin(); i != src.end(); i++)
   	push_back(new classString(*(*i)));
 }

//************************************************************************
// classe classString
//************************************************************************
classString::classString(const classString& src)
{
	sItem   = src.sItem;
   colonne = src.colonne;
   coche   = src.coche;
}


//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
classString& classString::operator= (classString src)
{
	sItem   = src.sItem;
   colonne = src.colonne;
   coche   = src.coche;
   return *this;
}


classString::classString( string 	sChaine , int Colonne  , bool Coche )
{
  sItem = sChaine ; colonne = Colonne;  coche =  Coche ;
}

//-------------------------------------------------------------------------
// 					  		Décomposer pChaine et mettre ses
//							  		items dans le vecteur pVect
//											Séparateur = "|" par défaur
//-------------------------------------------------------------------------
void
DecomposeChaine(string* pChaine, ClasseStringVector* pVect, string separateur )
{
    size_t debut = 0;
    size_t posit = pChaine->find(separateur);

    if(posit == string::npos)
       pVect->push_back(new classString(*pChaine));

    else
    {
    	while (posit != string::npos)
    	{
      	    pVect->push_back(new classString( string(*pChaine, debut, posit - debut) ));
      	    debut = posit + 1;
      	    posit = pChaine->find(separateur, debut+1);
    	}
    	pVect->push_back(new classString( string(*pChaine, debut, strlen(pChaine->c_str()) - debut ) ));
    }
}

//  +-----------------------------------------------------------------+
//  ¦                    Calcul de l'âge du patient                   ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
//
int donne_age(char *datex, char *dateNaiss)
{
	unsigned int i, j;
	signed   int age;
	unsigned int mois_nais, mois_jour, jour_nais, jour_jour;
	//
	// On ne calcule l'âge que si on a la date de naissance
	//
	if (strcmp(dateNaiss, "00000000") == 0)
		return -1;
	//
	// Année de l'examen
	//
	i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]);
	i = (i - 18) * 100;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]);
	//
	// Année de naissance du patient
	//
	j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]);
	j = (j - 18) * 100;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]);
	//
	// Age qu'aura le patient dans l'année
	//
	age = i - j;
	//
	// Correctifs en fonction de mois et jour
	//
	mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]);
	jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]);
	mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]);
	jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]);
	if ((mois_jour < mois_nais) ||
		 ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
		age--;
	if ((age < 0) || (age > 150))
		return -1;
	return age;
}

//  +-----------------------------------------------------------------+
//  ¦                Calcul de l'âge des bébés en mois                ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
//
int donne_age_mois(char *datex, char *dateNaiss)
{
	unsigned int i, j;
	signed   int age;
	unsigned int mois_nais, mois_jour, jour_nais, jour_jour;
	//
	// On ne calcule l'âge que si on a la date de naissance
	//
	if (strcmp(dateNaiss, "00000000") == 0)
		return -1;
	//
	// Année de l'examen
	//
	i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]);
	i = (i - 18) * 100;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]);
	//
	// Année de naissance du patient
	//
	j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]);
	j = (j - 18) * 100;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]);
	//
	// Age qu'aura le patient dans l'année (en mois)
	//
	age = (i - j) * 12;
	//
	// Correctifs en fonction de mois et jour
	//
	mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]);
	jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]);
	mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]);
	jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]);
	age = age + mois_jour - mois_nais;
	if (jour_jour < jour_nais)
		age--;
	return age;
}

//  +-----------------------------------------------------------------+
//  ¦              Donne l'intitulé (Monsieur, Madame...)             ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
//
void donne_intitule_patient(string *intitule, int age, bool bFeminin)
{
	if (bFeminin)
		*intitule = "Madame ";
	else
		*intitule = "Monsieur ";
	//
	// Correctifs en fonction de l'age
	//
	if ((age < 0) || (age > 150)) return;
	if (age <= 15)
	{
		*intitule = "l'enfant ";
		return;
	}
	if (bFeminin && (age < 18))
		*intitule = "Mademoiselle ";
	return;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne l'intitulé de l'âge du patient              ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
//
void donne_intitule_age(string *intitule, char *datex, char *dateNaiss)
{
	char ptgag[4];
	int  i, age;
	*intitule = "";
	//
	// Calcul de l'âge
	//
	age = donne_age(datex, dateNaiss);
	/*                                                                     */
	/*  ------------------- Patient d'au moins 2 ans --------------------  */
	/*                                                                     */
	if (age >= 2)
	{
		numacar(ptgag, age, 3);
		while (ptgag[0] == '0')
			for (i = 0; ptgag[i] != '\0'; i++)
				ptgag[i] = ptgag[i+1];
		*intitule = string(ptgag) + " an";
		if (age > 1) *intitule += "s";
	}
	//
	// Bébé de moins de 2 ans
	//
	else if (age >= 0)
	{
		age = donne_age_mois(datex, dateNaiss);
		if (age > 0)
		{
			numacar(ptgag, age, 2);
         if (ptgag[0] == '0')
				for (i = 0; ptgag[i] != '\0'; i++)
					ptgag[i] = ptgag[i+1];
			*intitule = string(ptgag) + " mois";
		}
	}
	return;
}

//  +-----------------------------------------------------------------+
//  ¦                    Décode une date AAAAMMJJ                     ¦
//  +-----------------------------------------------------------------+
//  Créé le 16/10/1992 Dernière mise à jour 16/10/1992
//
//  Date au format AAAAMMJJ
//
void decode_date(string *resultat, char *date)
{
	unsigned int i, jour, mois;
	char 			 date2[11];
	jour = 10 * donneDigit(date[6]) + donneDigit(date[7]);
	mois = 10 * donneDigit(date[4]) + donneDigit(date[5]);
	if ((jour < 1) || (jour > 31)) jour = 0;
	if ((mois < 1) || (mois > 12)) mois = 0;
	if (jour == 0)
	{
		strcpy(date2, "19XX");
		for (i = 0; i < 4; i++) date2[i] = date[i];
		*resultat = " en ";
		if (mois != 0)
		{
			switch (mois)
			{
				case 1  : *resultat += "janvier ";   break;
				case 2  : *resultat += "février ";   break;
				case 3  : *resultat += "mars ";      break;
				case 4  : *resultat += "avril ";     break;
				case 5  : *resultat += "mai ";       break;
				case 6  : *resultat += "juin ";      break;
				case 7  : *resultat += "juillet ";   break;
				case 8  : *resultat += "août ";      break;
				case 9  : *resultat += "septembre "; break;
				case 10 : *resultat += "octobre ";   break;
				case 11 : *resultat += "novembre ";  break;
				case 12 : *resultat += "décembre ";  break;
			}
		}
		*resultat += date2;
	}
	else
	{
		strcpy(date2, "XX/XX/19XX");
		for (i = 0; i < 4; i++) date2[i+6] = date[i];
		for (i = 0; i < 2; i++) date2[i+3] = date[i+4];
		for (i = 0; i < 2; i++) date2[i]   = date[i+6];
		*resultat = " le ";
		*resultat += date2;
	}
	return;
}



//***************************************************************************
// 							Implémentation des mèthodes VecteurString
//***************************************************************************
VectString::~VectString()
{
	vider();
}


VectString::VectString()
{}

void
VectString::vider()
{
   IterString i = begin();
   while(i != end())
   {
   	delete *i;
      erase(i);
   }
}


VectString&
VectString::operator=(VectString src)
{
  vider();
  for (IterString i = src.begin(); i != src.end(); i++)
   	push_back(new string(*(*i)));
  	return *this;
}


VectString::VectString(VectString& src)
{
	for (IterString i = src.begin(); i != src.end(); i++)
   	push_back(new string(*(*i)));
}

string
texteHtml(string texte)
{
	int i;
   	string sHtml("");

   	for (i=0; i < texte.length(); i++)
   	{
   		switch (texte[i])
      	{
      		case 'â':
         		sHtml = sHtml + "&acirc;"; break;
         	case 'à':
         		sHtml = sHtml + "&agrave;"; break;
            case 'æ':
         		sHtml = sHtml + "&aelig;"; break;
         	case 'ç':
         		sHtml = sHtml + "&ccedil;"; break;
         	case 'é':
         		sHtml = sHtml + "&eacute;"; break;
         	case 'ê':
         		sHtml = sHtml + "&ecirc;"; break;
         	case 'è':
         		sHtml = sHtml + "&egrave;"; break;
         	case 'ë':
         		sHtml = sHtml + "&euml;"; break;
         	case 'î':
         		sHtml = sHtml + "&icirc;"; break;
         	case 'ï':
         		sHtml = sHtml + "&iuml;"; break;
         	case 'ô':
         		sHtml = sHtml + "&ocirc;"; break;
            case 'ö':
         		sHtml = sHtml + "&ouml;"; break;
            case 'ú':
         		sHtml = sHtml + "&uacute;"; break;
            case 'ù':
         		sHtml = sHtml + "&ugrave;"; break;
            case 'û':
         		sHtml = sHtml + "&ucirc;"; break;
            case 'ü':
         		sHtml = sHtml + "&uuml;"; break;
         	case '<':
         		sHtml = sHtml + "&lt;"; break;
         	case '>':
         		sHtml = sHtml + "&gt;"; break;
         	case '&':
         		sHtml = sHtml + "&ramp;"; break;
         	case '"':
         		sHtml = sHtml + "&quot;"; break;
         	case '|':
            	sHtml = sHtml + "&#124;"; break;
         	default :
         		sHtml = sHtml + texte[i];
    	}
	}
	return sHtml;
}

string
texteCourant(string textHtml)
{
	string textOut = textHtml;
   	int    pos = textOut.find("&");
   	int    len;

   	while (pos != NPOS)
   	{
    	len = strlen(textOut.c_str()) - pos;

   		if 		((len >= 7) && (string(textOut,pos,7) == "&acirc;"))
      		textOut.replace(pos, 7, "â");
      	else if ((len >= 8) && (string(textOut,pos,8) == "&agrave;"))
      		textOut.replace(pos, 8, "à");
        else if ((len >= 8) && (string(textOut,pos,7) == "&aelig;"))
      		textOut.replace(pos, 7, "æ");
      	else if ((len >= 8) && (string(textOut,pos,8) == "&ccedil;"))
      		textOut.replace(pos, 8, "ç");
      	else if ((len >= 8) && (string(textOut,pos,8) == "&eacute;"))
      		textOut.replace(pos, 8, "é");
      	else if ((len >= 7) && (string(textOut,pos,7) == "&ecirc;"))
      		textOut.replace(pos, 7, "ê");
      	else if ((len >= 8) && (string(textOut,pos,8) == "&egrave;"))
      		textOut.replace(pos, 8, "è");
      	else if ((len >= 6) && (string(textOut,pos,6) == "&euml;"))
      		textOut.replace(pos, 6, "ë");
      	else if ((len >= 7) && (string(textOut,pos,7) == "&icirc;"))
        	textOut.replace(pos, 7, "î");
      	else if ((len >= 6) && (string(textOut,pos,6) == "&iuml;"))
      		textOut.replace(pos, 6, "ï");
      	else if ((len >= 7) && (string(textOut,pos,7) == "&ocirc;"))
      		textOut.replace(pos, 7, "ô");
        else if ((len >= 6) && (string(textOut,pos,6) == "&ouml;"))
      		textOut.replace(pos, 6, "ö");
        else if ((len >= 8) && (string(textOut,pos,8) == "&uacute;"))
      		textOut.replace(pos, 8, "ú");
        else if ((len >= 8) && (string(textOut,pos,8) == "&ugrave;"))
      		textOut.replace(pos, 8, "ù");
        else if ((len >= 7) && (string(textOut,pos,7) == "&ucirc;"))
      		textOut.replace(pos, 7, "û");
        else if ((len >= 6) && (string(textOut,pos,6) == "&uuml;"))
      		textOut.replace(pos, 6, "ü");
      	else if ((len >= 4) && (string(textOut,pos,4) == "&lt;"))
      		textOut.replace(pos, 4, "<");
      	else if ((len >= 4) && (string(textOut,pos,4) == "&gt;"))
      		textOut.replace(pos, 4, ">");
      	else if ((len >= 6) && (string(textOut,pos,6) == "&ramp;"))
      		textOut.replace(pos, 6, "&");
      	else if ((len >= 6) && (string(textOut,pos,6) == "&quot;"))
      		textOut.replace(pos, 6, "\"");
      	else if ((len >= 6) && (string(textOut,pos,6) == "&#124;"))
        	textOut.replace(pos, 6, "|");

    	pos++;

      	pos = textOut.find("&",pos);
	}
   	return textOut;
}
