//***************************************************************************//    NSUTIL.CPP
//    KRZISCH  Ph. janvier 92
// Implémentation des fonctions utiles
//***************************************************************************

//#include "ns_vector.h"

#include "nsutil\nsutifct.h"
#include "partage\nsdivfct.h"

//---------------------------------------------------------------------------
//  Function: Position(char* value, char *array[], int nbElts)
//
//  Arguments:
//            value        ->  valeur cherchée
//            array        ->  matrice dans laquelle on recherche
//            nbElts       ->  nombre d'éléments
//  Description:
//            retourne la position de value dans array.
//  Returns:
//            position de value dans array si value existe
//            -1 sinon
//---------------------------------------------------------------------------
int Position(char* value, pFieldsArray array, int nbElts)
{
  int location = -1;

  for (int i = 0; (i < nbElts) && (location == -1); i++)
	 if(!strcmp(value, array[i]))
		 location = i;

  return location+1;   // Les numéros de champs de PXEngine commencent à 1}

//---------------------------------------------------------------------------//  Function: wspf(char buffer[], char* fmt, ...)
//
//  Arguments:
//            buffer[] -> chaîne de caractère formattée
//            fmt      -> chaîne de format
//            ...      -> liste variable d'arguments
//  Description:
//            wsprint(...) ne semble pas être implémentée dans OWL
//            ce qui oblige à en simuler
//  Returns:
//            Le nombre de caractères saisis
//---------------------------------------------------------------------------
int wspf(char buffer[], char* fmt, ...)
{
  va_list argptr;
  int cnt;

  va_start(argptr, fmt);
  cnt = vsprintf(buffer, fmt, argptr);
  va_end(argptr);
  return cnt;
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
//---------------------------------------------------------------------------/*void SetInternational(void)
{
  static char cName [] = "intl" ;

  iDate = GetProfileInt (cName, "iDate", 0) ;
  iTime = GetProfileInt (cName, "iTime", 0) ;

  GetProfileString (cName, "sDate",	"/", sDate,	2) ;
  GetProfileString (cName, "sTime",	":", sTime,	2) ;
  GetProfileString (cName, "s1159", "AM", sAMPM [0], 5) ;
  GetProfileString (cName, "s2359", "PM", sAMPM [1], 5) ;
} */

//---------------------------------------------------------------------------//  Function:  ptrInit(char* &strPtr, int length)
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
/*BOOL ptrInit(char* *strPtr, int length, char car)
{
 *strPtr = new char[length + 1];
 if (*strPtr == NULL)
	 return FALSE;
 else {
	 memset(*strPtr, car, length);
	 *(*strPtr + length) = '\0';
 }
 return TRUE;
}

//---------------------------------------------------------------------------//  Function:  ptrInit(char* &strPtr,const char* strInit)
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
BOOL ptrInit(char* *strPtr,const char* strInit)
{
 *strPtr = new char[strlen(strInit) + 1];
 if (*strPtr == NULL)
	 return FALSE;
 else
	 strcpy(*strPtr, strInit);
 return TRUE;
} */

//---------------------------------------------------------------------------//  Function:  strAnalyse(const char* input, const char* separateurs, CacheArray& elts,
//                        int nb)
//
//  Arguments:
//             input      -> chaîne à analyser
//             separateur -> chaîne des séparateurs
//             elts       -> array (dynamique) des tokens
//             nb         -> nombre d'élément pour un cacheElement
//  Description:
//             Analyse une chaîne contenant des séparateurs et retourne
//             chaque élt. trouvé dans une array elts[] de CacheElement.
//             nb spécifie le nombre de chaînes que contient un cacheElement.
//  Returns:
//             RIEN
//  Exemple : strAnalyse("a bb;ccc d", " ;",...);
//---------------------------------------------------------------------------
void strAnalyse(const char* input, const char* separateurs,
					 CacheArray& elts, int nb)
{
  ChaineArray szPtr =  ChaineArray(0, 0, 1) ;
  char* szCur;
  char* copie;

  ptrInit(&copie, input);

 // strtok place un \0 à la fin de l'élément si il est trouvé
  szCur = strtok(copie, separateurs);

  while (szCur != NULL) {
	 szPtr.Add(*new string(szCur));       // Le tas

	// un deuxième appel à strtok avec comme premier paramètre NULL
	// retourne un pointeur sur le caractère suivant le token
	 szCur = strtok(NULL, separateurs);
  }
  for (int i = 0; i < szPtr.ArraySize(); i += nb)
	 elts.push_back(new CacheElement(szPtr, i, nb));
  delete copie;
}

//---------------------------------------------------------------------------//  Function:  strAnalyse(char* input, char* separateurs, CacheArray& elts)
//
//  Arguments:
//             input      -> chaîne à analyser
//             separateur -> chaîne des séparateurs
//             elts       -> array (dynamique) des tokens
//             nb         -> nombre d'élément pour un cacheElement
//  Description:
//             Analyse une chaîne contenant des séparateurs et retourne
//             chaque élt. trouvé dans une array elts[] de CacheElement.
//             nb spécifie le nombre de chaînes que contient un cacheElement.
//  Returns:
//             RIEN
//  Exemple : strAnalyse("a bb;ccc d", " ;",...);
//---------------------------------------------------------------------------
/*void strAnalyse(char* input, char* separateurs, CacheArray& elts, int nb)
{
  char* szPtr = NULL;

  // strtok place un \0 à la fin de l'élément si il est trouvé
	szPtr = strtok(input, separateurs);
	while (szPtr != NULL)
	  for(int i = 1; i <= nb; i++) {
		 elts.Add(new CacheElement(szPtr));
		// un deuxième appel à strtok avec comme premier paramètre NULL
		// retourne un pointeur sur le caractère suivant le token
		 szPtr = strtok(NULL, separateurs);
	  }
} */

//---------------------------------------------------------------------------//  Function:  strAnalyse(char* input, char* separateurs, char* elts[],
//                        int nb = 1)
//
//  Arguments:
//             input      -> chaîne à analyser
//             separateur -> chaîne des séparateurs
//             elts       -> array  des tokens
//             nb         -> nombre de chaînes attendu par un CacheElement
//  Description:
//             Analyse une chaîne contenant des séparateurs et retourne
//             chaque élt. trouvé dans une array elts[] de sous-chaînes.
//  Returns:
//             RIEN
//  Exemple : strAnalyse("a bb;ccc d", " ;",...);
//---------------------------------------------------------------------------
/*void strAnalyse(char* input, char* separateurs, char* elts[], int nb)
{
  char* szPtr = NULL;
  int nElts = sizeof elts / sizeof(char *);

  // strtok place un \0 à la fin de l'élément si il est trouvé
	szPtr = strtok(input, separateurs);
	for (int i = 0; i < nElts; i++)
	  for (int j = 1; j <= nb; j++) {
		 ptrInit(&elts[i], szPtr);
	  // un deuxième appel à strtok avec comme premier paramètre NULL
	  // retourne un pointeur sur le caractère suivant le token
		 szPtr = strtok(NULL, separateurs);
	}
} */

//---------------------------------------------------------------------------//  Function:
//      int firstNoWhite(char* s);
//  Arguments:
//      s  -> chaîne à étudier.
//  Description:
//      Retourne la position du premier caractère qui n'est pas un
//      caractère blanc ou une tabulation de la chaîne s.
//  Returns:
//      -1        si la chaîne ne contient pas de blancs au début.
//	strlen(s) si la chaîne ne contient que des blancs
//      la position sinon
//---------------------------------------------------------------------------
int firstNoWhite(const char* s)
{
 	int location = -1;
   int i;
  	for (i = 0; s[i] && (location == -1); ++i)
	 	if ((s[i] != ' ') && (s[i] != '\t'))
			location = i;
  	if (!s[i])
   	return(i);
  	else
   	return location;
}

//---------------------------------------------------------------------------//  Function:
//      int lastNoWhite(char* s);
//  Arguments:
//      s -> chaîne à étudier
//  Description:
//      Retourne la position du dernier caractère qui n'est pas un
//      caractère blanc ou une tabulation de la chaîne s.
//  Returns:
//      -1 si la chaîne ne contient pas de blancs à la fin
//      la position du dernier carctère qui n'est pas un blanc.
//---------------------------------------------------------------------------
int lastNoWhite(const char* s)
{
  int location = -1;

  for (int i = 0; s[i]; ++i)
	 if ((s[i] != ' ') && (s[i] != '\t'))
		location = i;
  return location;
}

//---------------------------------------------------------------------------//  Function:
//      beginPad(const char* s, char* dest, int numBlanks)
//  Arguments:
//      s         -> chaîne à bourrer
//	dest      -> chaîne formattée
//      numBlanks -> nb. de blancs de bourrage
//  Description:
//      Bourre la chaîne s de numBlanks caractères blancs au début
//      et place la chaîne ainsi formattée dans dest.
//  Returns:
//      -1 si il n'y plus assez d'espace mémoire
//       0 si tout a bien marché
//---------------------------------------------------------------------------
int beginPad(const char* s, char* dest, int numBlanks)
{
    int nbChar =  strlen(s) + numBlanks;
    char* temp = new char[nbChar+1];

    if (temp == NULL)
        return -1;
    memset(temp, ' ', numBlanks);
    *(temp + numBlanks) = '\0';    strcat(temp, s);
    strcpy(dest, temp);
    delete[] temp;
    return 0;
}

//---------------------------------------------------------------------------//  Function:
//      int endPad(const char* s, char* dest, int numBlanks)
//  Arguments:
//      s         -> chaîne à bourrer
//      dest      -> chaîne obtenue après bourrage
//      numBlanks -> nombre de caractères blancs de bourrage
//  Description:
//      Bourre la chaîne s de numBlanks caractères blancs à la fin
//      et place la chaîne bourrée dans dest.
//  Returns:
//      -1 si la mémoire est insuffisante pour le bourrage
//       0 si le bourrage est réussi
//---------------------------------------------------------------------------
int endPad(const char* s, char* dest, int numBlanks)
{
    char* temp = new char[numBlanks+1];
    if (temp == NULL)
        return -1;
    memset(temp, ' ', numBlanks);
    *(temp + numBlanks) = '\0';
    strcpy(dest, s);
    strcat(dest, temp);
    delete[] temp;
    return 0;
}

//---------------------------------------------------------------------------//  Function:
//     void beginWhiteCut(char* s);
//  Arguments:
//     s  -> chaîne à traiter
//  Description:
//     Enlève tous les blancs de début de chaîne.
//  Returns:
//     RIEN
//---------------------------------------------------------------------------
void beginWhiteCut(char* s)
{
  	int pos = firstNoWhite(s);
  	if (pos == 0)
        return;
  	char* temp = new char[strlen(s)+1];
  	for (int i = 0; *(temp + i) = *(s+pos+i); i++);
  	strcpy(s, temp);
  	delete[] temp;
}

//---------------------------------------------------------------------------//  Function:
//      void endWhiteCut(char* s);
//  Arguments:
//      s  ->  chaîne à tripoter
//  Description:
//      Enlève tous les blancs en fin de la chaîne s.
//  Returns:
//      RIEN
//---------------------------------------------------------------------------
void endWhiteCut(char* s)
{
    int pos = lastNoWhite(s);
  	if (pos == -1)
   	    strcpy(s, "");
  	else if (pos != strlen(s))
    {   	    char* temp = new char[pos+1];        int i;
		for (i = 0; i < pos+1; i++)
	  		*(temp + i) = s[i];
		*(temp + i) = '\0';
		strcpy(s, temp);
		delete[] temp;
  	}
}

//---------------------------------------------------------------------------//  Function:
//      int centerStr(const char* s, char* dest, int maxChar);
//  Arguments:
//      s       -> chaîne que l'on veut formatter
//      dest    -> chaîne formattée
//      maxChar -> nombre de caractères de la chaîne désirée
//  Description:
//      Centre la chaîne s sur maxChar et place dans dest le résultat
//  Returns:
//      -1  si problème de mémoire
//       0  si OK
//       1  si le nombre de caractères non blancs de s > maxChar
//---------------------------------------------------------------------------
int centerStr(const char* s, char* dest, int maxChar)
{
  strcpy(dest, s);
  endWhiteCut(dest);
  beginWhiteCut(dest);
  int length = strlen(dest);
  if (length > maxChar)
	 return 1;
  length = (maxChar - length) / 2;
  if (beginPad(dest, dest, length) == -1)
	 return -1;
  length = maxChar - strlen(dest);
  return endPad(dest, dest, length);
}

//---------------------------------------------------------------------------//  Function:
//     int leftStr(const char* s, char* dest, int maxChar)
//  Arguments:
//     s       -> chaîne que le désire formatter
//     dest    -> chaîne formattée
//     maxChar -> nombre de caractères que l'on désire obtenir
//  Description:
//     Justifie à gauche une chaîne s et place le résultat dans dest
//  Returns:
//     -1  si problème de mémoire
//      0  si OK
//      1  si le nombre de caractères non blancs de s > maxChar
//---------------------------------------------------------------------------
int leftStr(const char* s, char* dest, int maxChar)
{
  strcpy(dest, s);
  beginWhiteCut(dest);
  endWhiteCut(dest);
  int length = strlen(dest);
  if (length > maxChar)
	 return 1;
  length = maxChar - length;
  return endPad(dest, dest, length);
}

//---------------------------------------------------------------------------//  Function:
//      int rightStr(const char* s, char* dest, int maxChar);
//  Arguments:
//      s       -> chaîne que l'on désire formatter
//      dest    -> chaîne formattée
//      maxChar -> nombre de caractères désirés
//  Description:
//      Justifie à droite la chaîne s et place le résultat dans dest.
//  Returns:
//      -1  si problème de mémoire
//       0  si OK
//       1  si le nombre de caractères non blancs de s > maxChar
//---------------------------------------------------------------------------
int rightStr(const char* s, char* dest, int maxChar)
{
  strcpy(dest, s);
  endWhiteCut(dest);
  beginWhiteCut(dest);
  int length = strlen(dest);
  if (length > maxChar)
	 return 1;
  length = maxChar - length;
  return beginPad(dest, dest, length);
}

//---------------------------------------------------------------------------//  Function: InitSousClass(HWND hWnd, WORD id_Edit, FARPROC lpChangeProc)
//
//  Arguments:
//            hWnd         -> handle de la fenêtre parent
//            id_Edit      -> ID du contrôle EDIT que l'on veut sousclasser
//            lpChangeProc -> Adresse de la nouvelle WndProc que l'on veut
//                            mettre en place.
//  Description:
//            Doit être appeler si l'on désire sousclasser un contrôle EDIT.
//            Cela suppose que l'on stocke l'adresse de l'ancienne adresse
//            de la WndProc du contrôle EDIT soit dans un champ del'objet
//            boîte de dialogue, soit comme variable globale.
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
/*
void InitSousClass(HWND hWnd, WORD id_Edit, FARPROC lpChangeProc)
{
  HWND hWndEdit = GetDlgItem(hWnd, id_Edit);
  FARPROC lpNewProc = MakeProcInstance((FARPROC)lpChangeProc, hInst);
  FARPROC lpOldProc = (WNDPROC) SetWindowLong(hWndEdit, GWL_WNDPROC, (long)lpNewProc) ;
}
*/

//---------------------------------------------------------------------------
//  Function:  FiniSousClass(HWND hWnd, WORD id_Edit, FARPROC lpPrevProc)
//
//  Arguments:
//             hWnd       -> handle de la fenêtre mère
//             id_Edit    -> ID du contrôle EDIT que l'on a sousclassé.
//             lpPrevProc -> adresse de l'ancienne WndProc
//  Description:
//             Doit être appelée pour remettre la bonne adresse de la
//             WndProc du contrôle Edit que l'on a sousclassé.
// ATTENTION : Il faut que soit stocké en variable globale lpOldProc.
//  Returns:
//             RIEN
//---------------------------------------------------------------------------
/*
void FiniSousClass(HWND hWnd, WORD id_Edit, FARPROC lpPrevProc)
{
  HWND hWndEdit = GetDlgItem(hWnd, id_Edit) ;
  FARPROC lpNewProc = (FARPROC)SetWindowLong(hWndEdit, GWL_WNDPROC, (long)lpPrevProc) ;
  FreeProcInstance(lpNewProc) ;
}
*/

//---------------------------------------------------------------------------//  Function:  EditNbWndProc(HWND hWnd, WORD wMessage,
//       		               WORD wParam, LONG lParam)
//  Arguments:
//             hWnd     -> handle du contrôle EDIT que l'on veut sousclasser
//             wMessage -> message WINDOWS que l'on veut traiter
//             wParam   -> contient le caractère saisi
//             lParam   -> non utilisé
//  Description:
//             Permet de limiter la saisie, dans un contrôle EDIT, à des
//             chiffres.
//  ATTENTION : il faut que soit stockée quelque part lpOldProc
//  Returns:
//             Le nombre de chiffres saisis puisque c'est la valeur
//             retournée par lpOldProc.
//---------------------------------------------------------------------------
/*
LRESULT FAR PASCAL _export EditNbWndProc(HWND hWnd, WORD wMessage,
						WORD wParam, LONG lParam)
{
  LRESULT lRes = 0 ;     // Valeur de retour
  bool bTraite = true ;  // Indique commande traitée
//-------------------------------------------------
  switch(wMessage)
  {
    case WM_CHAR :
      if (((wParam < '0') || (wParam > '9')) && wParam != 0x08)
        MessageBeep(0) ;
      else
        bTraite = false ;
      break ;
    default     :
      bTraite = false ;
  }
  // Traitement par défault
  if (false == bTraite)
    lRes = CallWindowProc(lpOldProc, hWnd, wMessage, wParam, lParam);
  return lRes ;
}
*/

//---------------------------------------------------------------------------//  Function:  EditCharWndProc(HWND hWnd, WORD wMessage,
//				WORD wParam, LONG lParam)
//  Arguments:
//	       hWnd     -> handle du contrôle EDIT que l'on veut sousclasser
//             wMessage -> message WINDOWS que l'on veut traiter
//             wParam   -> contient le caractère saisi
//             lParam   -> non utilisé
//  Description:
//             Permet de limiter la saisie, dans un contrôle EDIT, à des
//             caractères autres que des chiffres.
//  ATTENTION : il faut que soit stockée quelque part lpOldProc
//  Returns:
//             Le nombre de caractères saisis puisque c'est la valeur
//             retournée par lpOldProc.
//---------------------------------------------------------------------------
/*
LRESULT FAR PASCAL _export EditCharWndProc(HWND hWnd, WORD wMessage,
				WORD wParam, LONG lParam)
{
  LRESULT lRes = 0;        // Valeur de retour
  BOOL bTraite = TRUE;  // Indique commande traitée
//-------------------------------------------------
  switch(wMessage) {
    case WM_CHAR : if (   ((wParam < 'A')|| ((wParam > 'Z') && (wParam < 'a')))
		       || ((wParam > 'z') && (wParam < 0xC0))
		       || (wParam == 0xD7) || (wParam == 0xF7)&&(wParam != 0x08) )
		      MessageBeep(0);
		   else
		      bTraite = FALSE;
		   break;
    default     :  bTraite = FALSE;
		   break;
    }
    // Traitement par défault
    if(!bTraite)
       lRes = CallWindowProc(lpOldProc, hWnd, wMessage, wParam, lParam);
    return lRes;
}
*/

