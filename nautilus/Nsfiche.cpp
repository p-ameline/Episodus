//---------------------------------------------------------------------------
//    NSFICHE.CPP
//
//  Implémentation des objets PARADOX de NAUTILUS
//
//  1ère version : PA   juillet 93       Dernière modif : 17/06/94
//---------------------------------------------------------------------------
#include <owl\olemdifr.h>
#include <owl\applicat.h>
#include <mem.h>
#include <string.h>
#include <mbstring.h>
#include <bde.hpp>
#include <stdio.h>

#include "ocf\ocreg.h"
#include "partage\nsdivfct.h"
#include "ns_sgbd\nsfiche.h"
#include "nautilus\nssuper.h"

//***************************************************************************
// Implémentation des méthodes NSFiche
//***************************************************************************

//---------------------------------------------------------------------------
//  Function: 	  NSFiche::NSFiche(NSSuper* pSuper)
//
//  Description: Constructeur de la class NSFiche.
//---------------------------------------------------------------------------
NSFiche::NSFiche(NSContexte* pCtx) : NSRoot(pCtx)
{
    //
	// Initialise les données membres privées
	isOpen		 = false;
	isClone	     = false;
	lastError	 = DBIERR_NONE;
	pRecBuff     = 0;
	pBookMark[0] = 0;
	pBookMark[1] = 0;
	indexNom[0]  = '\0';
	indexID	     = 0;
	statut 	     = 1;
    hCursor 	 = 0;
    pLdObj 		 = 0;
}

//---------------------------------------------------------------------------
//  Function : 	NSFiche::NSFiche(NSSuper* pSuper, hDBICur hCurSrc)
//
//  Arguments :   hCurSrc : handle du curseur source pour le clonage
//
//  Description : Constructeur de la class NSFiche avec curseur clone.
//---------------------------------------------------------------------------
NSFiche::NSFiche(NSContexte* pCtx, hDBICur hCurSrc): NSRoot(pCtx)
{
	//
	// Initialise les données membres privées
	//
	isOpen		= true;
	isClone	  	= true;
	lastError	= DBIERR_NONE;
	pRecBuff    = 0;
	pBookMark[0]= 0;
	pBookMark[1]= 0;
	indexNom[0] = '\0';
	indexID	  	= 0;
	statut 	  	= 1;
	hCursor 	= 0;
    pLdObj 		= 0;
	//
	// Appel de la fonction de clonage
	// ATTENTION : DbiCloneCursor plante si le fichier n'est pas ouvert,
	//					on vérifie donc avant que le curseur source est valide
	//
	CURProps curProps;
	lastError = DbiGetCursorProps(hCurSrc, curProps);
	if (lastError != DBIERR_NONE)
		return;
	//
	// Clonage
   //
	lastError = DbiCloneCursor(hCurSrc, FALSE, FALSE, hCursor);
	if (lastError != DBIERR_NONE)
		return;
	//
	// Récupération des propriétés liées à la table en cours
	//
	// Initialisation du Record Buffer (pRecBuff)
	//
	lastError = DbiGetCursorProps(hCursor, curProps);
	if (lastError == DBIERR_NONE)
		pRecBuff = new Byte[curProps.iRecBufSize];

   if(hCursor != 0)
   	lastError = DbiGetLdObj(hCursor, pLdObj);
	//
	// Mise à jour des variables indexNom et indexID
	//
	IDXDesc idxDesc;
	lastError = DbiGetIndexDesc(hCursor, 0, idxDesc);
	if (lastError == DBIERR_NONE)
   {
		strcpy(indexNom, idxDesc.szName);
		indexID  = idxDesc.iIndexId;
	}
}

//---------------------------------------------------------------------------
//  Function: NSFiche::~NSFiche()
//
//  Arguments:
//            AUCUN
//  Description:
//            Ferme la classe
//  Returns:
//            RIEN
//---------------------------------------------------------------------------
NSFiche::~NSFiche()
{
	//
	// Détermine si la table a été ouverte ou non
	//
	if (isOpen == true)
		// Appelle la fonction membre de fermeture
		close();

	if (pRecBuff)
		delete[] pRecBuff;
	if (pBookMark[0])
		delete[] pBookMark[0];
	if (pBookMark[1])
		delete[] pBookMark[1];
}

//---------------------------------------------------------------------------
// vérifie la validité du code
//---------------------------------------------------------------------------
bool
NSFiche::CodeValide(string code)
{
	bool valide = true;

   if (strlen(code.c_str()) == 0) return false;

	for (int i = 0; i < strlen(code.c_str()); i++)
    {
   	    if (! (((code[i] >= '0') && (code[i] <= '9')) || ((code[i] >= 'A') && (code[i] <= 'Z'))))
        {
      	    valide = false;
            break;
        }
    }

    return valide;
}

//---------------------------------------------------------------------------
//calcul le code
//---------------------------------------------------------------------------
bool
NSFiche::IncrementeCode(string* pTexte)
{
	int i = strlen(pTexte->c_str()) ;
   if(i == 0)
   	return false;
   bool tourner = true;

   while((tourner) && (i > 0) )
   {
      i--;
   	if( (((*pTexte)[i] >= '0') && ((*pTexte)[i] < '9')) ||
      	 (((*pTexte)[i] >= 'A') && ((*pTexte)[i] < 'Z')) )
      {
	      (*pTexte)[i] = (*pTexte)[i] + 1;
         tourner = false;
      }
      else if ((*pTexte)[i] == '9')
      {
	      (*pTexte)[i] = 'A';
         tourner = false;
      }
      else
	      (*pTexte)[i] = '0';
   }

   return(!tourner);
}

//---------------------------------------------------------------------------
//  Function:  NSFiche::LastCode(string* psLastCode, int typeCode)
//
//  Arguments: Pointeur sur la string qui reçoit le nouveau code
//					Entier correspondant au type de compteur à récupérer dans nauti.lus
//
//  Description: Lit les compteurs de nauti.lus et incrémente :
//						1)hommes 2)femmes 3)utilisateurs 4)correspondants 5)adresses
//
//  Returns: true->Succès false->Echec
//
//---------------------------------------------------------------------------
bool NSFiche::LastCode(string* psLastCode, int typeCode)
{
	ifstream inFile;
   	ofstream outFile;
   	string sCompteurs = "", line;
   	string sNomCompteur[5] = {"","","","",""};
   	string sValCompteur[5] = {"","","","",""};
   	string sTab = string(10, ' ');
   	string sFichierCompteurs;
   	int i,j;

   	// ENTREE EN SECTION CRITIQUE
   	if (!pContexte->DemandeAcces("NAUTI.LUS"))
   	{
   	 	erreur("Impossible d'obtenir l'accès au fichier nauti.lus (compteurs)",0,0);
     	return false;
   	}

   	// ATTENTION : ce fichier est obligatoirement avec la base des patients
   	sFichierCompteurs = pContexte->PathName("BGLO") + string("nauti.lus");

   	inFile.open(sFichierCompteurs.c_str());
	if (!inFile)
   	{
   		erreur("Erreur d'ouverture en lecture du fichier nauti.lus.", 0, 0);
    	pContexte->LibereAcces("NAUTI.LUS");
      	return false;
   	}

   	while (!inFile.eof())
   	{
        getline(inFile,line);
        if (line != "")
            sCompteurs += line + "\n";
   	}
   	inFile.close();

   	i=0; j=0;

   	// boucle de chargement des compteurs
   	while (i < strlen(sCompteurs.c_str()))
   	{
   		while ((i < strlen(sCompteurs.c_str())) && (sCompteurs[i] != ' '))
      		sNomCompteur[j] += sCompteurs[i++];

      	while ((i < strlen(sCompteurs.c_str())) && (sCompteurs[i] == ' '))
      		i++;

      	while ((i < strlen(sCompteurs.c_str())) && (sCompteurs[i] != '\n'))
      		sValCompteur[j] += sCompteurs[i++];

      	i++;
      	j++;
   	}

   	for (j=0; j < 5; j++)
   	{
   		if (!CodeValide(sValCompteur[j]))
      	{
      		erreur("Erreur : Code compteur invalide dans le fichier nauti.lus.", 0, 0);
            pContexte->LibereAcces("NAUTI.LUS");
         	return false;
      	}
   	}

   	switch(typeCode)
   	{
    case 1:
    case 2:
   		if (atoi(sValCompteur[0].c_str()) && atoi(sValCompteur[1].c_str()))
   		{
        	if (typeCode == 1)
      			*psLastCode = sValCompteur[0];
      		else
      			*psLastCode = sValCompteur[1];

      		if (!IncrementeCode(psLastCode))
      		{
      			erreur("Erreur : L'incrémentation du compteur des patients a échoué.", 0, 0);
                pContexte->LibereAcces("NAUTI.LUS");
      			return false;
      		}

      		if (typeCode == 1)
      			sValCompteur[0] = *psLastCode;
      		else
      			sValCompteur[1] = *psLastCode;
   		}
   		else // cas d'un compteur unique hommes + femmes
   		{
         	if (typeCode == 1)
            {
            	*psLastCode = sValCompteur[0];
               	if (!IncrementeCode(psLastCode))
      			{
      				erreur("Erreur : L'incrémentation du compteur des patients a échoué.", 0, 0);
                    pContexte->LibereAcces("NAUTI.LUS");
      				return false;
      			}
               	sValCompteur[0] = *psLastCode;
            }
            else if (atoi(sValCompteur[1].c_str()))
            {
            	*psLastCode = sValCompteur[1];
               	if (!IncrementeCode(psLastCode))
      			{
      				erreur("Erreur : L'incrémentation du compteur des patients a échoué.", 0, 0);
                    pContexte->LibereAcces("NAUTI.LUS");
      				return false;
      			}
               	sValCompteur[1] = *psLastCode;
            }
            else // cas du compteur unique : on prend le compteur hommes
            {
            	*psLastCode = sValCompteur[0];
               	if (!IncrementeCode(psLastCode))
      			{
      				erreur("Erreur : L'incrémentation du compteur des patients a échoué.", 0, 0);
                    pContexte->LibereAcces("NAUTI.LUS");
      				return false;
      			}
               	sValCompteur[0] = *psLastCode;
            }
   		}
        break;

    case 3:
      	*psLastCode = sValCompteur[2];
         if (!IncrementeCode(psLastCode))
         {
         	erreur("Erreur : L'incrémentation du compteur des utilisateurs a échoué.", 0, 0);
            pContexte->LibereAcces("NAUTI.LUS");
      		return false;
         }
         sValCompteur[2] = *psLastCode;
         break;

    case 4:
      	*psLastCode = sValCompteur[3];
         if (!IncrementeCode(psLastCode))
         {
         	erreur("Erreur : L'incrémentation du compteur des correspondants a échoué.", 0, 0);
            pContexte->LibereAcces("NAUTI.LUS");
      		return false;
         }
         sValCompteur[3] = *psLastCode;
         break;

    case 5:
      	*psLastCode = sValCompteur[4];
         if (!IncrementeCode(psLastCode))
         {
         	erreur("Erreur : L'incrémentation du compteur des adresses a échoué.", 0, 0);
            pContexte->LibereAcces("NAUTI.LUS");
      		return false;
         }
         sValCompteur[4] = *psLastCode;
         break;
	}


   	// reconstitution de la chaine de sortie (nauti.lus)

   	sCompteurs = "";

   	for (j = 0; j < 5; j++)
   	{
   		sCompteurs += sNomCompteur[j];
      	sCompteurs += sTab;
      	sCompteurs += sValCompteur[j];
      	sCompteurs += string("\n");
   	}

   	// On réécrit le nouveau fichier
   	outFile.open(sFichierCompteurs.c_str());
	if (!outFile)
   	{
   		erreur("Erreur d'ouverture en écriture du fichier nauti.lus.", 0, 0);
        pContexte->LibereAcces("NAUTI.LUS");
   		return false;
   	}

   	for (i=0; i < strlen(sCompteurs.c_str()); i++)
     	 outFile.put(sCompteurs[i]);

    outFile.close();

   	// SORTIE DE LA SECTION CRITIQUE
   	pContexte->LibereAcces("NAUTI.LUS");

   	return true;
}

//---------------------------------------------------------------------------
//  Function: NSFiche::ChangeIndex(pCHAR IndexNom, UINT16 IndexID)
//
//  Arguments:
//            IndexNom : nom de l'index
//				  IndexID  : ID de l'index
//
//  Description:
//            Change d'index
//  Returns:
//            DBIERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult NSFiche::ChangeIndex(char* IndexNom, Word IndexID, bool MemeRecord)
{
  	//
  	// Vérifie si la table a été ouverte
  	//
  	if (isOpen != true)
   	return(lastError = ERROR_TABLENOTOPEN);
  	//
  	// Vérifie si on n'est pas déjà sur le bon index
  	//
  	if (((IndexNom[0] != '\0') && (strcmp(IndexNom, indexNom) == 0)) ||
		((IndexID != NODEFAULTINDEX) && (IndexID == indexID)))
   	return(lastError = DBIERR_NONE);

  	//
  	// Change d'Index ATTENTION PERTE DES BOOKMARKS
  	//
  	if (IndexID == NODEFAULTINDEX)
   	lastError = DbiSwitchToIndex(hCursor, IndexNom, NULL, NULL, MemeRecord);
  	else if (IndexNom[0] == '\0')
		lastError = DbiSwitchToIndex(hCursor, NULL, NULL, IndexID, MemeRecord);
  	else
		lastError = DbiSwitchToIndex(hCursor, NULL, NULL, IndexID, MemeRecord);

  	if (lastError != DBIERR_NONE)
   	return(lastError);

   // ici on regarde si les bookmarks sont alloués
   // if (pBookMark[0] || pBookMark[1])
   // 	erreur("Attention BookMarks invalides",0,0);

  	//
  	// Remise à jour des variables indexNom et indexID
  	//
  	IDXDesc idxDesc;
  	lastError = DbiGetIndexDesc(hCursor, 0, idxDesc);
  	if (lastError == DBIERR_NONE)
   {
   	strcpy(indexNom, idxDesc.szName);
	  	indexID  = idxDesc.iIndexId;
  	}

  	return(lastError);
}

DBIResult
NSFiche::chercheClef(string* clefATrouver, DBITBLNAME IndexNom,
									DBISearchCond searchMode, DBILockType Blocage)
{
	return chercheClef((unsigned char*) clefATrouver->c_str(),
   						 IndexNom, NODEFAULTINDEX, searchMode, Blocage);
}

//---------------------------------------------------------------------------
//  Function:  NSFiche::chercheClef(string* clefATrouver, DBITBLNAME IndexNom,
//	      									UINT16 IndexID, DBISearchCond searchMode,
//												DBILockType Blocage)
//
//  Arguments: ClefATrouver 		: Valeur recherchée
//					IndexNom, IndexID : nom et ID de l'index
//					searchMode			: Type de recherche
//					Blocage				: Type de Blocage désiré
//
//  Description:
//             Cherche une clé dans un index simple
//  Returns:
//             DBIERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::chercheClef(string* clefATrouver, DBITBLNAME IndexNom,
							Word IndexID, DBISearchCond searchMode,
                     DBILockType Blocage)
{
	return chercheClef((unsigned char*) clefATrouver->c_str(),
   						 IndexNom, IndexID, searchMode, Blocage);
}


//---------------------------------------------------------------------------
//  Function:  NSFiche::chercheClef(char *clefATrouver, DBITBLNAME IndexNom,
//	      									UINT16 IndexID, DBISearchCond searchMode,
//												DBILockType Blocage)
//
//  Arguments: ClefATrouver 		: Valeur recherchée
//					IndexNom, IndexID : nom et ID de l'index
//					searchMode			: Type de recherche
//					Blocage				: Type de Blocage désiré
//
//  Description:
//             Cherche une clé dans un index simple
//  Returns:
//             DBIERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::chercheClef(unsigned char *clefATrouver, DBITBLNAME IndexNom,
							Word IndexID, DBISearchCond searchMode,
                     DBILockType Blocage)
{
    Byte* pKeyBuf;

  	if (!isOpen)
   	return(lastError = ERROR_TABLENOTOPEN);

  	//
  	// Sélection de l'index
  	//
  	lastError = ChangeIndex(IndexNom, IndexID, false);

  	if (lastError != DBIERR_NONE)
   	return(lastError);

   // on récupère les curProps pour avoir iKeySize
   CURProps curProps;
  	DbiGetCursorProps(hCursor, curProps);

   if (_mbslen(clefATrouver) > curProps.iKeySize)
   	erreur("Attention clef trop longue...",0,0);

   pKeyBuf = new Byte[curProps.iKeySize + 1];
   memset(pKeyBuf, 0, curProps.iKeySize + 1);
   _mbscpy(pKeyBuf, clefATrouver);

  	//
  	// Recherche dans l'index
  	//
  	lastError = DbiSetToKey(hCursor, searchMode, TRUE, 0, 0, pKeyBuf);

   // searchMode = keySEARCH + EQ, GT ou GEQ
   // Indique que clefATrouver pointe
   // sur l'index et non sur un RecBuff
	// Utilisés pour les index multiples

   delete[] pKeyBuf;

  	if (lastError != DBIERR_NONE)
   	return(lastError);
  	//
  	// DbiSetToKey positionne sur le crack qui précède l'enregistrement
  	//
  	lastError = suivant(Blocage);

  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFiche::chercheClefComposite(DBITBLNAME IndexNom,
//	      									UINT16 IndexID, DBISearchCond searchMode,
//												DBILockType Blocage)
//
//  Arguments:
//					IndexNom, IndexID : nom et ID de l'index
//					searchMode			: Type de recherche
//					Blocage				: Type de Blocage désiré
//
//  Description:
//             Cherche une clé dans un index simple
//  Returns:
//             DBIERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::chercheClefComposite(DBITBLNAME IndexNom, Word IndexID,
										DBISearchCond searchMode,
                              DBILockType Blocage,
                              Byte* pIndexRec)
{
	if (!isOpen)
   	return(lastError = ERROR_TABLENOTOPEN);
  	//
  	// Sélection de l'index
  	//
  	lastError = ChangeIndex(IndexNom, IndexID, false);

  	if (lastError != DBIERR_NONE)
   	return(lastError);

  	//
  	// Recherche dans l'index
  	//
  	lastError = DbiSetToKey(hCursor,
								  searchMode,     // - keySEARCH + EQ, GT ou GEQ
								  FALSE,				// - Indique que clefATrouver pointe sur un RecBuff
								  0, 0,				// - Utilisés pour les index multiples
								  pIndexRec);

  	if (lastError != DBIERR_NONE)
   	return(lastError);

  	//
  	// DbiSetToKey positionne sur le crack qui précède l'enregistrement
  	//
  	lastError = suivant(Blocage);

  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::precedent(DBILockType Blocage)
//
//  Arguments:
//            Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
//
//  Description:
//            Déplacement sur l'enregistrement précédent
//  Returns:
//            PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::precedent(DBILockType Blocage)
{
	//
  	// Vérifie si la table a été ouverte
  	//
  	if (isOpen != true)
   	return(lastError = ERROR_TABLENOTOPEN);
  	//
  	// Si elle l'a été : appelle la fonction DbiGetPriorRecord
  	//
  	lastError = DbiGetPriorRecord(hCursor, Blocage, pRecBuff, NULL);
  	DbiRelRecordLock(hCursor, FALSE);
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::suivant(DBILockType Blocage)
//
//  Arguments:
//            Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
//
//  Description:
//            Déplacement sur l'enregistrement suivant
//  Returns:
//            DBIERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::suivant(DBILockType Blocage)
{
  	//
  	// Vérifie si la table a été ouverte
  	//
  	if (isOpen != true)
   	return(lastError = ERROR_TABLENOTOPEN);
  	//
  	// Si elle l'a été : appelle la fonction DbiGetNextRecord
  	//
  	lastError = DbiGetNextRecord(hCursor, Blocage, pRecBuff, NULL);
  	DbiRelRecordLock(hCursor, FALSE);
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//
// Function : NSFiche::debut(DBILockType Blocage)
//
// Arguments:
//            Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
//
// Description:
//            Va au premier enregistrement de la table.
//
// Returns: DBIERR s'il y a lieu
//
//---------------------------------------------------------------------------
DBIResult
NSFiche::debut(DBILockType Blocage)
{
  	//
  	// Vérifie que la table a été ouverte
  	//
   // Si elle ne l'est pas, on renvoie une erreur
   //
  	if (isOpen != true)
   	lastError = ERROR_TABLENOTOPEN;
  	else
  	{
	 	//
	 	// Appelle la fonction DbiSetToBegin pour déplacer le curseur sur
	 	// une position juste avant le 1er enregistrement.
	 	//
	 	lastError = DbiSetToBegin(hCursor);
	 	//
	 	// Vérifie qu'il n'y a pas eu d'erreur.
	 	//
	 	if(lastError == DBIERR_NONE)
	 	{
			//
			// En avançant d'un cran, lit effectivement sur le 1er Rec.
			//
			lastError = DbiGetNextRecord(hCursor, Blocage, pRecBuff, NULL);
			DbiRelRecordLock(hCursor, FALSE);
	 	}
  	}
  	//
  	// Retourne l'éventuelle erreur.
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
// Function :		NSFiche::fin(DBILockType Blocage)
//
// Arguments:		Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
//
// Description:	Va au dernier enregistrement de la table.
//
// Returns: DBIERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::fin(DBILockType Blocage)
{
	//
  	// Vérifie que la table a été ouverte
  	//
  	if(isOpen != true)
  	{
   	//
	 	// Si elle ne l'est pas, on renvoie une erreur
	 	//
	 	lastError = ERROR_TABLENOTOPEN;
   	return(lastError);
  	}
  	else
  	{
	 	//
	 	// Appelle la fonction DbiSetToEnd pour déplacer le curseur sur
	 	// une position juste après le dernier enregistrement.
	 	//
	 	lastError = DbiSetToEnd(hCursor);
	 	//
	 	// Vérifie qu'il n'y a pas eu d'erreur.
	 	//
	 	if (lastError == DBIERR_NONE)
      {
      	//
		 	// En reculant d'un cran, on lit le dernier enregistrement.
		 	//
		 	lastError = DbiGetPriorRecord(hCursor, Blocage, pRecBuff, NULL);
		 	DbiRelRecordLock(hCursor, FALSE);
	 	}
	}
  	//
  	// Retourne l'éventuelle erreur.
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
// Function :		NSFiche::donneNbTotalRec(pUINT32 pNombre)
//
// Arguments:		pNombre : Nombre d'enregistrements associés au curseur
//
// Description:	Donne le nombre d'enregistrments associés au curseur
//
// Returns: DBIERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::donneNbTotalRec(int* pNombre)
{
	lastError = DbiGetRecordCount(hCursor, *pNombre);
	return(lastError);
}

//---------------------------------------------------------------------------
// Function :		NSFiche::donneNumRecEnCours(pUINT32 pNumero)
//
// Arguments:		pNumero : Numéro de l'enregistrement en cours
//
// Description:	Donne le numéro de l'enregistrement en cours en fonction de
//						l'index
//						ATTENTION : ne marche ni pour SQL, ni pour dBASE
//
// Returns: DBIERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::donneNumRecEnCours(int* pNumero)
{
	lastError = DbiGetSeqNo(hCursor, *pNumero);
	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::open(char *tableName, int tableClasse)
//
//  Arguments:
//            tableName   = nom de la table (ex "PATFI.DB"
//				  tableClasse = catégorie de la table (ex Partagée globale...)
//
//  Description:
//            Ouvre une table par son nom et initialise pRecBuff
//  Returns:
//            PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::open(string sTableName, int tableClasse, bool bOpenShared)
{
   	string sPathName = "";
   	char pTableName[300];
    DBIShareMode eShareMode;

    NSSuper* pSuper = pContexte->getSuperviseur();

   	if ((sTableName == string("CHEMINS.DB")) || (sTableName == string("SUPPORTS.DB")))
   		sPathName = pSuper->PathBase("BCHE");
   	else
   	{
   		switch (tableClasse)
   		{
      		case NSF_PARTAGE_GLOBAL :
         		sPathName = pSuper->PathBase("BGLO");
            	break;
         	case NSF_PARTAGE_PARAMS :
         		sPathName = pSuper->PathBase("BPAR");
            	break;
         	case NSF_PERSONNEL :
         		sPathName = pSuper->PathBase("BPER");
            	break;
         	case NSF_COMPTA :
         		sPathName = pSuper->PathBase("BCPT");
            	break;
         	case NSF_GUIDES :
         		sPathName = pSuper->PathBase("BGUI");
            	break;
         	default :
            	sPathName = "";
   		}
   	}
   	sPathName += sTableName;

   	strcpy(pTableName, sPathName.c_str());

   	// Sélection du mode partagé (OpenShared) ou exclusif (OpenExcl)
    // Mode exclusif principalement utilisé pour la base Critical.db
   	if (bOpenShared)
    	eShareMode = dbiOPENSHARED;
    else
    	eShareMode = dbiOPENEXCL;

	//
  	// Vérifie si la table est déjà ouverte
  	//
  	//if (isOpen == true)
  	//	  return(lastError = PXERR_TABLEOPEN);
  	//
  	// Essaie d'ouvrir la table
  	//
  	hDBIDb hNSdatabase = pSuper->hNSdb;
  	lastError = DbiOpenTable(hNSdatabase,
									pTableName,
									0,    //szXBASE
									0, 0, 0,
									dbiREADWRITE,
									eShareMode,
									xltFIELD,
									0, 0,
									hCursor);

  	if (lastError != DBIERR_NONE)
    	return(lastError);

  	isOpen = true;

  	//
  	// Initialisation du Record Buffer (pRecBuff)
  	//
  	CURProps curProps;
  	lastError = DbiGetCursorProps(hCursor, curProps);

  	if (lastError == DBIERR_NONE)
   		pRecBuff = new Byte[curProps.iRecBufSize];

   	if(hCursor != 0)
   		lastError = DbiGetLdObj(hCursor, pLdObj);

  	//
  	// Mise à jour des variables indexNom et indexID
  	//
  	IDXDesc idxDesc;
  	lastError = DbiGetIndexDesc(hCursor, 0, idxDesc);
  	if (lastError == DBIERR_NONE)
   	{
   		strcpy(indexNom, idxDesc.szName);
	  	indexID  = idxDesc.iIndexId;
    }

  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::close()
//
//  Arguments:
//            AUCUN
//  Description:
//            Ferme la table
//  Returns:
//            PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::close()
{
  	//
  	// Vérifie si la table a été ouverte
  	//
  	if (isOpen != true)
   	return(lastError = ERROR_TABLENOTOPEN);

  	//
  	// Ferme la table
  	//
  	lastError = DbiCloseCursor(hCursor);
  	//
  	// Teste si ça a marché
  	//
  	if (lastError == DBIERR_NONE)
   	isOpen = false;

  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function:  NSFiche::getRecord()
//
//  Arguments:
//
//  Description:
//             Prend l'enregistrement en cours et assigne la valeur des
//             champs aux variables membres de la classe.
//  Returns:
//             PXERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::getRecord()
{
  	//
  	// La table est-elle ouverte ?
	//
  	if (!isOpen)
   	return(lastError = ERROR_TABLENOTOPEN);
  	//
  	// Appelle la fonction de récupération d'enregistrement.
  	//
  	lastError = getDbiRecord(dbiWRITELOCK);
  	//
  	// Débloque l'enregistrement
  	//
  	DbiRelRecordLock(PrendCurseur(), FALSE);
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::getDbiRecord()
//
//  Arguments:
//            AUCUN
//  Description:
//            Prend l'enregistrement courant et le place dans l'enregistrement
//	      générique associé au BCursor
//  Returns:
//            PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::getDbiRecord(DBILockType Blocage)
{
  	//
  	// Rafraîchit le curseur
  	//
  	DbiForceReread(hCursor);
  	//
  	// Récupère l'enregistrement en utilisant la fonction membre
  	// getRecord de la classe BCursor
  	//
  	lastError = DbiGetRecord(hCursor, Blocage, pRecBuff, NULL);
  	//
  	// Transfère le contenu du "Record Buffer" dans la fiche
  	//
  	if (lastError == DBIERR_NONE)
   	alimenteFiche();
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction : NSFiche::insertRecord()
//
//  Arguments: 	AUCUN
//
//  Description:	Ajoute le contenu de la fiche au fichier en l'insérant
//            		là où est le curseur
//
//  Returns:		PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::insertRecord(DBILockType Blocage)
{
  	//
  	// Transfére la fiche dans le "Record buffer"
  	//
  	initialiseRecbuff();
  	videFiche();
  	//
  	// Insère l'enregistrement dans la table
  	//
  	lastError = DbiInsertRecord(hCursor, Blocage, pRecBuff);
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction: 	  NSFiche::appendRecord()
//
//  Arguments:   AUCUN
//
//  Description: Ajoute le contenu de la fiche à la fin du fichier
//               (équivalent à GOTOEND + InsertRecord)
//
//  Retour: 	  PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::appendRecord()
{
  	//
  	// Transfère la fiche dans le "Record buffer"
  	//
  	initialiseRecbuff();
  	videFiche();
  	//
  	// Insère l'enregistrement dans la table
  	//
  	lastError = DbiAppendRecord(hCursor, pRecBuff);
  	//
  	// Retour de l'éventuelle erreur
  	//
   return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction: 	  NSFiche::modifyRecord(BOOL relacheBlocage)
//
//  Arguments:   relacheBlocage : faut-il débloquer la fiche
//
//  Description: Met à jour la fiche sur disque à partir du contenu de la fiche
//
//  Retour: 	  PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::modifyRecord(BOOL relacheBlocage)
{
	//
  	// Transfère la fiche dans le "Record buffer"
  	//
  	initialiseRecbuff();
  	videFiche();
  	//
  	// Insère l'enregistrement dans la table
  	//
  	lastError = DbiModifyRecord(hCursor, pRecBuff, relacheBlocage);
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction: 	  NSFiche::deleteRecord()
//
//  Arguments:   AUCUN
//
//  Description: Détruit l'enregistrement pointé par le curseur
//
//  Retour: 	  PXERR s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::deleteRecord()
{
  	//
  	// Détruit l'enregistrement
  	//
  	lastError = DbiDeleteRecord(hCursor, pRecBuff);
  	//
  	// Retour de l'éventuelle erreur
  	//
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::initialiseRecbuff()
//
//  Arguments:
//            AUCUN
//  Description:
//            Met à zéro le Record Buffer
//
//  Returns:
//            Rien
//---------------------------------------------------------------------------
DBIResult
NSFiche::initialiseRecbuff()
{
  	//
  	// Met à blanc le "Record buffer"
  	//
  	lastError = DbiInitRecord(hCursor, pRecBuff);

  	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction: 		int NSFiche::creerBookMarks(int nbreBookMarks)
//
//  Arguments:		nbreBookMarks -> nombre de BookMarks à créer
//
//  Description : Crée des BookMarks
//
//  Retour:			0 si Ok, 1 en cas de création impossible
//---------------------------------------------------------------------------
int
NSFiche::creerBookMarks(int nbreBookMarks)
{
	CURProps curProps;
	lastError = DbiGetCursorProps(hCursor, curProps);

	if (lastError != DBIERR_NONE)
		return 1;

   if (pBookMark[0] || pBookMark[1])
   {
   	erreur("BookMarks déjà alloués...",0,0);
      return 1;
   }

	pBookMark[0] = new Byte[curProps.iBookMarkSize];

	if (nbreBookMarks > 1)
		pBookMark[1] = new Byte[curProps.iBookMarkSize];

	return 0;
}

//---------------------------------------------------------------------------
//  Fonction: 		int NSFiche::detruireBookMarks(int nbreBookMarks)
//
//  Arguments:		nbreBookMarks -> nombre de BookMarks à détruire
//
//  Description : Détruire des BookMarks
//
//  Retour:			0 si Ok, 1 en cas de destruction impossible
//---------------------------------------------------------------------------
int
NSFiche::detruireBookMarks(int nbreBookMarks)
{
   if (pBookMark[0] == 0)
   {
   	erreur("BookMark déjà détruit...",0,0);
      return 1;
   }

	delete[] pBookMark[0];
   pBookMark[0] = 0;

	if (nbreBookMarks > 1)
   {
   	if (pBookMark[1] == 0)
   	{
   		erreur("BookMark déjà détruit...",0,0);
      	return 1;
   	}

		delete[] pBookMark[1];
      pBookMark[1] = 0;
   }

   return 0;
}

//---------------------------------------------------------------------------
//  Fonction: 		DBIResult NSFiche::placeBookMark(int numBookMark)
//
//  Arguments:		numBookMark -> numéro du BookMarks à positionner
//
//  Description : Positionne un BookMark pour la fiche en cours
//
//  Retour:			DBIResult
//---------------------------------------------------------------------------
DBIResult
NSFiche::placeBookMark(int numBookMark)
{
	// on doit inclure ici le test de validité des bookmarks (cf ChangeIndex)

	if (pBookMark[numBookMark-1])
		lastError = DbiGetBookMark(hCursor, pBookMark[numBookMark-1]);
	else
		lastError = DBIERR_INVALIDHNDL;

	return(lastError);
}

//---------------------------------------------------------------------------
//  Fonction: 		DBIResult NSFiche::retrouveBookMark(int numBookMark)
//
//  Arguments:		numBookMark -> numéro du BookMarks à retrouver
//
//  Description : Replace le curseur sur un BookMark
//
//  Retour:			DBIResult
//---------------------------------------------------------------------------
DBIResult
NSFiche::retrouveBookMark(int numBookMark)
{
	// on doit inclure ici le test de validité des bookmarks (cf ChangeIndex)

	if (pBookMark[numBookMark-1])
		lastError = DbiSetToBookMark(hCursor, pBookMark[numBookMark-1]);
	else
		lastError = DBIERR_INVALIDHNDL;

	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::DebutModeLiaison()
//
//  Arguments :	Aucun
//
//  Description : Met à zéro le Record Buffer
//
//  Returns :		Code d'erreur
//---------------------------------------------------------------------------
DBIResult
NSFiche::DebutModeLiaison()
{
	lastError = DbiBeginLinkMode(hCursor);
  	return(lastError);
}

//---------------------------------------------------------------------------
//  Function: NSFiche::FinModeLiaison()
//
//  Arguments :	Aucun
//
//  Description : Met à zéro le Record Buffer
//
//  Returns :		Code d'erreur
//---------------------------------------------------------------------------
DBIResult
NSFiche::FinModeLiaison()
{
	lastError = DbiEndLinkMode(hCursor);
  	return(lastError);
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
DBIResult
NSFiche::NativeToAnsi(char *chaineSource, char *chaineCible)
{
   BOOL bDataLoss;
	return (DbiNativeToAnsi(pLdObj, chaineCible, chaineSource, 0, bDataLoss));
}


DBIResult
NSFiche::NativeToAnsi(char *chaine)
{
	BOOL bDataLoss;
	return (DbiNativeToAnsi(pLdObj, chaine, chaine, 0, bDataLoss));
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
DBIResult
NSFiche::AnsiToNative(char *chaineSource, char *chaineCible)
{
   BOOL bDataLoss;
	return (DbiAnsiToNative(pLdObj, chaineCible, chaineSource, 0, bDataLoss));
}


DBIResult
NSFiche::AnsiToNative(char *chaine)
{
	BOOL bDataLoss;
	return (DbiAnsiToNative(pLdObj, chaine, chaine, 0, bDataLoss));
}

//---------------------------------------------------------------------------
//  Function: 		NSFiche::alimenteChamp(char* pChamp, int index, int taille)
//
//  Description : Remplie un champ du fichier
//
//  Returns :		true si ça a marché, false sinon
//---------------------------------------------------------------------------
bool
NSFiche::alimenteChamp(char* pChamp, int index, int taille)
{
   BOOL isBlank = FALSE;
   Byte* pBufChamp;

   pBufChamp = new Byte[taille+1];

   //
   // Prise du libellé du champ
   //
	lastError = DbiGetField(PrendCurseur(), (Word) index, PrendRecBuff(), pBufChamp, isBlank);

   if (lastError != DBIERR_NONE)
   {
		erreur("Erreur de lecture d'un champ.", 1, lastError);
      return false;
   }

   strcpy(pChamp,(char *)pBufChamp);

   //
   // Traduction du libellé
   //
   if (isBlank)
   	memset(pChamp, '\0', taille);
   else
   	lastError = NativeToAnsi(pChamp);

   if (lastError != DBIERR_NONE)
   	return false;
   else
   	return true;
}

//---------------------------------------------------------------------------
//  Function: 		NSFiche::videChamp(char* pChamp, int index, int taille)
//
//  Description : Transfère une chaine vers un champ du fichier
//
//  Returns :		true si ça a marché, false sinon
//---------------------------------------------------------------------------
bool
NSFiche::videChamp(char* pChamp, int index, int taille)
{
	char* pBufChamp;
   //
   // Traduction du libellé
   //

   // On fait une copie pour éviter de modifier les données
   pBufChamp = new char[taille + 1];
   strcpy(pBufChamp, pChamp);

   lastError = AnsiToNative(pBufChamp);
   if (lastError != DBIERR_NONE)
   {
   	delete[] pBufChamp;
   	return false;
   }

   //
   // Mise en place du libellé du champ
   //
   lastError = DbiPutField(PrendCurseur(), (Word) index, PrendRecBuff(), (Byte*)pBufChamp);
   delete[] pBufChamp;

	if (lastError != DBIERR_NONE)
   {
		erreur("Erreur d'écriture d'un champ.", 1, lastError);
      return false;
   }
   else
   	return true;
}
