// -----------------------------------------------------------------------------
// NSFICHE.CPP
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2015/02/07 11:36:41 $
// -----------------------------------------------------------------------------
// Implémentation des objets PARADOX de NAUTILUS
// -----------------------------------------------------------------------------
// history :
// PA  - juin 94
// PA  - juillet 93
// -----------------------------------------------------------------------------

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
#include "ns_sgbd\nsrepare.h"
#include "ns_sgbd\ns_sgbd.h"
#include "nautilus\nssuper.h"

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSFiche
//
// -----------------------------------------------------------------------------

long NSFiche::_lNSFicheCount = 0 ;

// -----------------------------------------------------------------------------
// Function     : NSFiche::NSFiche(NSSuper *pSuper)
// Description  : Constructeur de la class NSFiche.
// -----------------------------------------------------------------------------
NSFiche::NSFiche(NSSuper* pSuper)
        :NSSuperRoot(pSuper)
{
  _lNSFicheCount++ ;

	// Initialise les données membres privées
	_isOpen		      = false ;
	isClone	        = false ;
	lastError	      = DBIERR_NONE ;
	pRecBuff        = (Byte*) 0 ;
	pBookMark[0]    = (Byte*) 0 ;
	pBookMark[1]    = (Byte*) 0 ;
	indexNom[0]     = '\0' ;
	indexID	        = 0 ;
	statut 	        = 1 ;
  hCursor 	      = 0 ;
  pLdObj 		      = 0 ;
  sNomFichier	    = string("") ;

  sNomTable	      = string("") ;
  sRepareWarning  = string("") ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::NSFiche(NSSuper *pSuper, hDBICur hCurSrc)
// Arguments    : hCurSrc : handle du curseur source pour le clonage
// Description  : Constructeur de la class NSFiche avec curseur clone.
// -----------------------------------------------------------------------------
NSFiche::NSFiche(NSSuper* pSuper, hDBICur hCurSrc)
        :NSSuperRoot(pSuper)
{
try
{
  _lNSFicheCount++ ;

	// Initialise les données membres privées
	_isOpen		      = true ;
	isClone	  	    = true ;
	lastError	      = DBIERR_NONE ;
	pRecBuff        = (Byte*) 0 ;
	pBookMark[0]    = (Byte*) 0 ;
	pBookMark[1]    = (Byte*) 0 ;
	indexNom[0]     = '\0' ;
	indexID	  	    = 0 ;
	statut 	  	    = 1 ;
	hCursor 	      = 0 ;
  pLdObj 		      = 0 ;
  sNomFichier	    = string("") ;

  sNomTable	      = string("") ;
  sRepareWarning  = string("") ;

	// Appel de la fonction de clonage
	// ATTENTION : DbiCloneCursor plante si le fichier n'est pas ouvert,
	//					on vérifie donc avant que le curseur source est valide
	CURProps curProps ;
	lastError = DbiGetCursorProps(hCurSrc, curProps) ;
	if (lastError != DBIERR_NONE)
		return ;

	// Clonage
	lastError = DbiCloneCursor(hCurSrc, FALSE, FALSE, hCursor) ;
	if (lastError != DBIERR_NONE)
		return ;

	// Récupération des propriétés liées à la table en cours
	// Initialisation du Record Buffer (pRecBuff)
	lastError = DbiGetCursorProps(hCursor, curProps) ;
	if (lastError == DBIERR_NONE)
		pRecBuff = new Byte[curProps.iRecBufSize] ;

  if(hCursor != 0)
    lastError = DbiGetLdObj(hCursor, pLdObj) ;

	// Mise à jour des variables indexNom et indexID
	IDXDesc idxDesc ;
	lastError = DbiGetIndexDesc(hCursor, 0, idxDesc) ;
	if (lastError == DBIERR_NONE)
  {
		strcpy(indexNom, idxDesc.szName) ;
		indexID  = idxDesc.iIndexId ;
	}
}
catch (...)
{
	erreur("Exception NSFiche ctor.", standardError, 0) ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::~NSFiche()
// Arguments    : AUCUN
// Description  : Ferme la classe
// Returns      : RIEN
// -----------------------------------------------------------------------------
NSFiche::~NSFiche()
{
	// Détermine si la table a été ouverte ou non
	if (true == _isOpen)
		// Appelle la fonction membre de fermeture
		close() ;

	if (pRecBuff)
		delete[] pRecBuff ;
	if (pBookMark[0])
		delete[] pBookMark[0] ;
	if (pBookMark[1])
		delete[] pBookMark[1] ;

  _lNSFicheCount-- ;
}

// -----------------------------------------------------------------------------
// vérifie la validité du code
// -----------------------------------------------------------------------------
bool
NSFiche::CodeValide(string code)
{
  bool valide = true ;

	if (strlen(code.c_str()) == 0)
    return false ;

  for (size_t i = 0 ; i < strlen(code.c_str()) ; i++)
  {
    if (!(((code[i] >= '0') && (code[i] <= '9')) ||
          ((code[i] >= 'A') && (code[i] <= 'Z'))))
    {
      valide = false ;
      break ;
    }
  }

  return valide ;
}

// -----------------------------------------------------------------------------
//calcul le code
// -----------------------------------------------------------------------------
bool
NSFiche::IncrementeCode(string *pTexte)
{
  if (NULL == pTexte)
    return false ;

  int i = int(strlen(pTexte->c_str())) ;
  if (0 == i)
    return false ;

  bool tourner = true ;

  while((tourner) && (i > 0))
  {
    i-- ;
    if ((((*pTexte)[i] >= '0') && ((*pTexte)[i] < '9')) ||
        (((*pTexte)[i] >= 'A') && ((*pTexte)[i] < 'Z')))
    {
      (*pTexte)[i] = char((*pTexte)[i] + 1) ;
      tourner = false ;
    }
    else if ('9' == (*pTexte)[i])
    {
      (*pTexte)[i] = 'A' ;
      tourner = false ;
    }
    else
      (*pTexte)[i] = '0' ;
  }

  return (!tourner) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::ChangeIndex(pCHAR IndexNom, UINT16 IndexID)
// Arguments    : IndexNom  : nom de l'index
//				        IndexID   : ID de l'index
// Description  : Change d'index
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::ChangeIndex(char *IndexNom, Word IndexID, bool MemeRecord)
{
try
{
  // Vérifie si la table a été ouverte
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Vérifie si on n'est pas déjà sur le bon index
  if (((IndexNom[0] != '\0') && (strcmp(IndexNom, indexNom) == 0)) ||
      ((IndexID != NODEFAULTINDEX) && (IndexID == indexID)))
    return (lastError = DBIERR_NONE) ;

  bool bTenter = true ;
  while (bTenter)
  {
    // Change d'Index ATTENTION PERTE DES BOOKMARKS
    if (IndexID == NODEFAULTINDEX)
      lastError = DbiSwitchToIndex(hCursor, IndexNom, NULL, NULL, MemeRecord) ;
    else if (IndexNom[0] == '\0')
      lastError = DbiSwitchToIndex(hCursor, NULL, NULL, IndexID, MemeRecord) ;
    else
			lastError = DbiSwitchToIndex(hCursor, NULL, NULL, IndexID, MemeRecord) ;

    if (lastError != DBIERR_NONE)
    {
      if (((lastError >= DBIERR_HEADERCORRUPT) && (lastError <= DBIERR_FOREIGNINDEX)) ||
          (lastError == DBIERR_INDEXOUTOFDATE))
      {
        HWND hParentForMessages = 0 ;
        if (NULL != _pSuper->donneMainWindow())
          hParentForMessages = _pSuper->donneMainWindow()->GetHandle() ;

        string sMessage = string("Erreur à l'ouverture de l'index ") + string(IndexNom) ;
        erreur(sMessage.c_str(), standardError, lastError, hParentForMessages) ;

        // On ferme la table avant sa réparation
        bool bOpenShared ;
        CURProps curProps ;
        DbiGetCursorProps(hCursor, curProps) ;
        bOpenShared = curProps.eShareMode ;
        close() ;

        // On lance l'utilitaire de réparation
        NSRepareTable *pRepare = new NSRepareTable(_pSuper->donneMainWindow(),
            										                    _pSuper->getContexte(),
                                                    sNomFichier,
                                                    sRepareWarning,
                                                    pNSResModule) ;

        if ((pRepare->Execute()) == IDCANCEL)
        {
          delete pRepare ;
          return (lastError) ;
        }
        delete pRepare ;

        // On réouvre la table
        lastError = open(sNomFichier, -1, bOpenShared) ;
        if (lastError != DBIERR_NONE)
          return (lastError) ;
      }
      else
        return (lastError) ;
    }
    else
      bTenter = false ;
  }
	// ici on regarde si les bookmarks sont alloués
  // if (pBookMark[0] || pBookMark[1])
  // 	erreur("Attention BookMarks invalides",0,0);


  // Remise à jour des variables indexNom et indexID
  IDXDesc idxDesc ;
  lastError = DbiGetIndexDesc(hCursor, 0, idxDesc) ;
  if (lastError == DBIERR_NONE)
  {
    strcpy(indexNom, idxDesc.szName) ;
    indexID  = idxDesc.iIndexId ;
  }

  return (lastError) ;
}
catch (...)
{
  erreur("Exception NSFiche::ChangeIndex.", standardError, 0) ;
  return (lastError) ;
}
}


DBIResult
NSFiche::chercheClef( const string  *clefATrouver,
                      DBITBLNAME    IndexNom,
									    DBISearchCond searchMode,
                      DBILockType   Blocage)
{
	return chercheClef( (const unsigned char *) clefATrouver->c_str(),
   						        IndexNom,
                      NODEFAULTINDEX,
                      searchMode,
                      Blocage) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::chercheClef( string        *clefATrouver,
//                                      DBITBLNAME    IndexNom,
//	      									            UINT16        IndexID,
//                                      DBISearchCond searchMode,
//												              DBILockType   Blocage)
// -----------------------------------------------------------------------------
// Arguments    : ClefATrouver 		  : Valeur recherchée
//					      IndexNom, IndexID : nom et ID de l'index
//					      searchMode			  : Type de recherche
//					      Blocage				    : Type de Blocage désiré
// -----------------------------------------------------------------------------
// Description  : Cherche une clé dans un index simple
// -----------------------------------------------------------------------------
// Returns      : DBIERR_, s'il y a lieu
//---------------------------------------------------------------------------
DBIResult
NSFiche::chercheClef( const string  *clefATrouver,
                      DBITBLNAME    IndexNom,
							        Word          IndexID,
                      DBISearchCond searchMode,
                      DBILockType   Blocage)
{
	return chercheClef( (const unsigned char *) clefATrouver->c_str(),
   						        IndexNom,
                      IndexID,
                      searchMode,
                      Blocage) ;
}


// -----------------------------------------------------------------------------
// Function:  NSFiche::chercheClef( char          *clefATrouver,
//                                  DBITBLNAME    IndexNom,
//	      									        UINT16        IndexID,
//                                  DBISearchCond searchMode,
//												          DBILockType   Blocage)
// -----------------------------------------------------------------------------
// Arguments    : ClefATrouver 		  : Valeur recherchée
//					      IndexNom, IndexID : nom et ID de l'index
//					      searchMode			  : Type de recherche
//					      Blocage				    : Type de Blocage désiré
// -----------------------------------------------------------------------------
// Description  : Cherche une clé dans un index simple
// -----------------------------------------------------------------------------
// Returns      : DBIERR_, s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::chercheClef( const unsigned char *clefATrouver,
                      DBITBLNAME          IndexNom,
                      Word                IndexID,
                      DBISearchCond       searchMode,
                      DBILockType         Blocage)
{
try
{
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Sélection de l'index
  lastError = ChangeIndex(IndexNom, IndexID, false) ;

  if (lastError != DBIERR_NONE)
    return (lastError) ;

  // on récupère les curProps pour avoir iKeySize
  CURProps curProps ;
  DbiGetCursorProps(hCursor, curProps) ;

  if (_mbslen(clefATrouver) > size_t(curProps.iKeySize))
  {
    erreur("Attention clef trop longue...", standardError, 0) ;
    return (lastError = DBIERR_RECNOTFOUND) ;
  }

  Byte *pKeyBuf = new Byte[curProps.iKeySize + 1] ;
  memset(pKeyBuf, 0, curProps.iKeySize + 1) ;
  _mbscpy(pKeyBuf, clefATrouver) ;

  // Recherche dans l'index
  lastError = DbiSetToKey(hCursor, searchMode, TRUE, 0, 0, pKeyBuf) ;

  // searchMode = keySEARCH + EQ, GT ou GEQ
  // Indique que clefATrouver pointe
  // sur l'index et non sur un RecBuff
	// Utilisés pour les index multiples

  delete[] pKeyBuf ;

  if (DBIERR_NONE != lastError)
    return (lastError) ;


  // DbiSetToKey positionne sur le crack qui précède l'enregistrement
  lastError = suivant(Blocage) ;

  return (lastError) ;
}
catch (...)
{
  erreur("Exception NSFiche::chercheClef.", standardError, 0) ;
  return (lastError) ;
}
}

// -----------------------------------------------------------------------------
// Function:  NSFiche::chercheClef( short         *clefATrouver,
//                                  DBITBLNAME    IndexNom,
//	      									        UINT16        IndexID,
//                                  DBISearchCond searchMode,
//												          DBILockType   Blocage)
// -----------------------------------------------------------------------------
// Arguments    : ClefATrouver 		  : Valeur recherchée
//					      IndexNom, IndexID : nom et ID de l'index
//					      searchMode			  : Type de recherche
//					      Blocage				    : Type de Blocage désiré
// -----------------------------------------------------------------------------
// Description  : Cherche une clé dans un index simple
// -----------------------------------------------------------------------------
// Returns      : DBIERR_, s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::chercheClef( short         clefATrouver,
                      int           index,
                      DBITBLNAME    IndexNom,
							        Word          IndexID,
                      DBISearchCond searchMode,
                      DBILockType   Blocage)
{
try
{
	short pKeyBuf = clefATrouver ;

  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Sélection de l'index
  lastError = ChangeIndex(IndexNom, IndexID, false) ;

  if (DBIERR_NONE != lastError)
    return (lastError) ;

	// on récupère les curProps pour avoir iKeySize
  CURProps curProps ;
  lastError = DbiGetCursorProps(hCursor, curProps) ;

  Byte *pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;
  DbiPutField(hCursor, index, pIndexRec, (Byte *) &pKeyBuf) ;

/*
  if (_mbslen(clefATrouver) > curProps.iKeySize)
    erreur("Attention clef trop longue...", standardError, 0) ;

  pKeyBuf = new Byte[curProps.iKeySize + 1] ;
  memset(pKeyBuf, 0, curProps.iKeySize + 1) ;
  _mbscpy(pKeyBuf, clefATrouver) ;
*/

  // Recherche dans l'index
  lastError = DbiSetToKey(hCursor, searchMode, FALSE, 0, 0, pIndexRec) ;

  delete[] pIndexRec ;

  // searchMode = keySEARCH + EQ, GT ou GEQ
  // Indique que clefATrouver pointe
  // sur l'index et non sur un RecBuff
  // Utilisés pour les index multiples

  if (DBIERR_NONE != lastError)
    return (lastError) ;


  // DbiSetToKey positionne sur le crack qui précède l'enregistrement
  lastError = suivant(Blocage) ;

  return (lastError) ;
}
catch (...)
{
  erreur("Exception NSFiche::chercheClef 2.", standardError, 0) ;
  return (lastError) ;
}
}

DBIResult
NSFiche::chercheClef( int           clefATrouver,
                      int           index,
                      DBITBLNAME    IndexNom,
							        Word          IndexID,
                      DBISearchCond searchMode,
                      DBILockType   Blocage)
{
try
{
  int pKeyBuf = clefATrouver ;

  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Sélection de l'index
  lastError = ChangeIndex(IndexNom, IndexID, false) ;

  if (DBIERR_NONE != lastError)
    return (lastError) ;

	// on récupère les curProps pour avoir iKeySize
  CURProps curProps ;
  lastError = DbiGetCursorProps(hCursor, curProps) ;

  Byte *pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;
  DbiPutField(hCursor, index, pIndexRec, (Byte *)&pKeyBuf);

  // Recherche dans l'index
  lastError = DbiSetToKey(hCursor, searchMode, FALSE, 0, 0, pIndexRec) ;

  delete[] pIndexRec ;

  // searchMode = keySEARCH + EQ, GT ou GEQ
  // Indique que clefATrouver pointe
  // sur l'index et non sur un RecBuff
  // Utilisés pour les index multiples

  if (DBIERR_NONE != lastError)
    return (lastError) ;


  // DbiSetToKey positionne sur le crack qui précède l'enregistrement
  lastError = suivant(Blocage) ;

  return (lastError) ;
}
catch (...)
{
  erreur("Exception NSFiche::chercheClef 3.", standardError, 0) ;
  return (lastError) ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::chercheClefComposite(DBITBLNAME    IndexNom,
//	      									                    UINT16        IndexID,
//                                              DBISearchCond searchMode,
//												                      DBILockType   Blocage)
// -----------------------------------------------------------------------------
// Arguments    : IndexNom, IndexID : nom et ID de l'index
//					      searchMode			  : Type de recherche
//					      Blocage				    : Type de Blocage désiré
// -----------------------------------------------------------------------------
// Description  : Cherche une clé dans un index simple
// -----------------------------------------------------------------------------
// Returns      : DBIERR_, s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::chercheClefComposite(DBITBLNAME    IndexNom,
                              Word          IndexID,
							                DBISearchCond searchMode,
                              DBILockType   Blocage,
                              Byte          *pIndexRec)
{
	if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Sélection de l'index
  lastError = ChangeIndex(IndexNom, IndexID, false) ;

  if (DBIERR_NONE != lastError)
    return (lastError) ;


  // Recherche dans l'index
  lastError = DbiSetToKey(hCursor,
    						          searchMode, // - keySEARCH + EQ, GT ou GEQ
                          FALSE,			// - Indique que clefATrouver pointe sur un RecBuff
                          0, 0,			  // - Utilisés pour les index multiples
                          pIndexRec) ;

  if (DBIERR_NONE != lastError)
    return (lastError) ;

  // DbiSetToKey positionne sur le crack qui précède l'enregistrement
  lastError = suivant(Blocage) ;

  return (lastError) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::precedent(DBILockType Blocage)
// -----------------------------------------------------------------------------
// Arguments    : Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
// -----------------------------------------------------------------------------
// Description  : Déplacement sur l'enregistrement précédent
// -----------------------------------------------------------------------------
// Returns      : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::precedent(DBILockType Blocage)
{
  // Vérifie si la table a été ouverte
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Si elle l'a été : appelle la fonction DbiGetPriorRecord
  lastError = DbiGetPriorRecord(hCursor, Blocage, pRecBuff, NULL) ;
  if (DBIERR_BOF != lastError)
    DbiRelRecordLock(hCursor, FALSE) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::suivant(DBILockType Blocage)
// -----------------------------------------------------------------------------
// Arguments    : Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
// -----------------------------------------------------------------------------
// Description  : Déplacement sur l'enregistrement suivant
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::suivant(DBILockType Blocage)
{
  // Vérifie si la table a été ouverte
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Si elle l'a été : appelle la fonction DbiGetNextRecord
  lastError = DbiGetNextRecord(hCursor, Blocage, pRecBuff, NULL) ;
  if (DBIERR_EOF != lastError)
    DbiRelRecordLock(hCursor, FALSE) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::debut(DBILockType Blocage)
// -----------------------------------------------------------------------------
// Arguments    : Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
// -----------------------------------------------------------------------------
// Description  : Va au premier enregistrement de la table.
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::debut(DBILockType Blocage)
{
  // Vérifie que la table a été ouverte
  // Si elle ne l'est pas, on renvoie une erreur
  if (false == _isOpen)
    lastError = ERROR_TABLENOTOPEN ;
  else
  {
	 	// Appelle la fonction DbiSetToBegin pour déplacer le curseur sur
	 	// une position juste avant le 1er enregistrement.
	 	lastError = DbiSetToBegin(hCursor);

	 	// Vérifie qu'il n'y a pas eu d'erreur.
	 	if (DBIERR_NONE == lastError)
	 	{
			// En avançant d'un cran, lit effectivement sur le 1er Rec.
			lastError = DbiGetNextRecord(hCursor, Blocage, pRecBuff, NULL);
			DbiRelRecordLock(hCursor, FALSE);
	 	}
  }
  // Retourne l'éventuelle erreur.
  return (lastError) ;
}

// -----------------------------------------------------------------------------
// Function     :	NSFiche::fin(DBILockType Blocage)
// -----------------------------------------------------------------------------
// Arguments    :	Type de blocage désiré (NOLOCK, READLOCK, WRITELOCK)
// -----------------------------------------------------------------------------
// Description  :	Va au dernier enregistrement de la table.
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::fin(DBILockType Blocage)
{
 	// Vérifie que la table a été ouverte
 	if (false == _isOpen)
 	{
	 	// Si elle ne l'est pas, on renvoie une erreur
	 	lastError = ERROR_TABLENOTOPEN ;
 		return(lastError) ;
  }
  else
  {
	 	// Appelle la fonction DbiSetToEnd pour déplacer le curseur sur
	 	// une position juste après le dernier enregistrement.
	 	lastError = DbiSetToEnd(hCursor) ;

	 	// Vérifie qu'il n'y a pas eu d'erreur.
	 	if (DBIERR_NONE == lastError)
    {
		 	// En reculant d'un cran, on lit le dernier enregistrement.
		 	lastError = DbiGetPriorRecord(hCursor, Blocage, pRecBuff, NULL) ;
		 	DbiRelRecordLock(hCursor, FALSE) ;
	 	}
	}

  // Retourne l'éventuelle erreur.
  return (lastError) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::emptyTable(DBILockType Blocage)
// -----------------------------------------------------------------------------
// Description  :	Efface tout le contenu de la table
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::emptyTable()
{
  // Vérifie que la table a été ouverte
  if (false == _isOpen)
  {
	 	lastError = ERROR_TABLENOTOPEN ;
    return (lastError) ;
  }

  // Appelle la fonction DbiEmptyTable pour vider la table
  lastError = DbiEmptyTable(_pSuper->getDBHandle(), hCursor, "", "") ;

  // Retourne l'éventuelle erreur.
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Function     :	NSFiche::donneNbTotalRec(pUINT32 pNombre)
// -----------------------------------------------------------------------------
// Arguments    :	pNombre : Nombre d'enregistrements associés au curseur
// -----------------------------------------------------------------------------
// Description  :	Donne le nombre d'enregistrments associés au curseur
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::donneNbTotalRec(int *pNombre)
{
	lastError = DbiGetRecordCount(hCursor, *pNombre) ;
	return(lastError) ;
}


// -----------------------------------------------------------------------------
// Function     :	NSFiche::donneNumRecEnCours(pUINT32 pNumero)
// -----------------------------------------------------------------------------
// Arguments    :	pNumero : Numéro de l'enregistrement en cours
// -----------------------------------------------------------------------------
// Description  :	Donne le numéro de l'enregistrement en cours en fonction de l'index
//						    ATTENTION : ne marche ni pour SQL, ni pour dBASE
// -----------------------------------------------------------------------------
// Returns      : DBIERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::donneNumRecEnCours(int *pNumero)
{
	lastError = DbiGetSeqNo(hCursor, *pNumero) ;
	return(lastError) ;
}


// -----------------------------------------------------------------------------
//  Function    : NSFiche::open(char *tableName, int tableClasse)
// -----------------------------------------------------------------------------
//  Arguments   : tableName   = nom de la table (ex "PATFI.DB"
//				        tableClasse = catégorie de la table (ex Partagée globale...)
// -----------------------------------------------------------------------------
// Description  : Ouvre une table par son nom et initialise pRecBuff
// -----------------------------------------------------------------------------
//  Returns     : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::open(string sTableName, int tableClasse, bool bOpenShared)
{
try
{
  string sPathName = string("") ;
  char pTableName[1024] ;
  DBIShareMode eShareMode ;

  if ((sTableName == string("CHEMINS.DB")) || (sTableName == string("SUPPORTS.DB")))
    sPathName = _pSuper->PathBase("BCHE") ;
  else
  {
    switch (tableClasse)
   	{
      case NSF_PARTAGE_GLOBAL : sPathName = _pSuper->PathBase("BGLO") ;
            	                  break ;
      case NSF_PARTAGE_PARAMS : sPathName = _pSuper->PathBase("BPAR") ;
                                break ;
      case NSF_PERSONNEL      : sPathName = _pSuper->PathBase("BPER") ;
            	                  break ;
      case NSF_COMPTA         : sPathName = _pSuper->PathBase("BCPT") ;
            	                  break ;
      case NSF_GUIDES         : sPathName = _pSuper->PathBase("BGUI") ;
                                break ;
      case NSF_UPGRADE        : sPathName = "" ;
                                break ;
      default                 : sPathName = "" ;
    }
  }
  sPathName += sTableName ;
  strcpy(pTableName, sPathName.c_str()) ;

  // Sélection du mode partagé (OpenShared) ou exclusif (OpenExcl)
  // Mode exclusif principalement utilisé pour la base Critical.db
  if (bOpenShared)
    eShareMode = dbiOPENSHARED;
  else
    eShareMode = dbiOPENEXCL;

  // Vérifie si la table est déjà ouverte
  //if (isOpen == true)
  //	  return(lastError = PXERR_TABLEOPEN) ;

  // Essaie d'ouvrir la table
  hDBIDb hNSdatabase = _pSuper->getDBHandle() ;

  bool bTenter = true ;

  while (bTenter)
  {
    lastError = DbiOpenTable( hNSdatabase,
                              (char*) sPathName.c_str(), // pTableName,
                              0,    //szXBASE
                              0, 0, 0,
                              dbiREADWRITE,
                              eShareMode,
                              xltFIELD,
                              0, 0,
                              hCursor) ;

    if (DBIERR_NONE != lastError)
    {
      if (((lastError >= DBIERR_HEADERCORRUPT) && (lastError <= DBIERR_FOREIGNINDEX)) ||
          (lastError == DBIERR_INDEXOUTOFDATE))
      {
        string sMessage = string("Erreur à l'ouverture du fichier ") + sNomTable ;
        TWindow* pMotherWindow =  _pSuper->donneMainWindow() ;
        if ((pMotherWindow) &&  (pMotherWindow->IsWindow()))
          erreur(sMessage.c_str(), standardError, lastError, pMotherWindow->GetHandle()) ;
        else
          erreur(sMessage.c_str(), standardError, lastError, 0) ;

        NSRepareTable *pRepare = new NSRepareTable( pMotherWindow,
                                                    _pSuper->getContexte(),
                                                    sPathName,
                                                    sRepareWarning,
                                                    pNSResModule) ;
        if ((pRepare->Execute()) == IDCANCEL)
        {
          delete pRepare ;
          return (lastError) ;
        }
        else
          delete pRepare ;
      }
      else
        return (lastError) ;
    }
    else
      bTenter = false ;
  }

  _isOpen = true ;
  sNomFichier = sPathName ;

  // Initialisation du Record Buffer (pRecBuff)
  CURProps curProps ;
  lastError = DbiGetCursorProps(hCursor, curProps) ;

  if (lastError == DBIERR_NONE)
  {
    if (pRecBuff)
      delete[] pRecBuff ;

    pRecBuff = new Byte[curProps.iRecBufSize] ;
  }

  if (0 != hCursor)
    lastError = DbiGetLdObj(hCursor, pLdObj) ;

  // Mise à jour des variables indexNom et indexID
  IDXDesc idxDesc ;
  lastError = DbiGetIndexDesc(hCursor, 0, idxDesc) ;
  if (DBIERR_NONE == lastError)
  {
    strcpy(indexNom, idxDesc.szName) ;
    indexID  = idxDesc.iIndexId ;
  }

  // Retour de l'éventuelle erreur
  return (lastError) ;
}
catch (...)
{
  erreur("Exception NSFiche::open.", standardError, 0) ;
  return (lastError) ;
}
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::close()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  : Ferme la table
// -----------------------------------------------------------------------------
// Returns      : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::close()
{
  // Vérifie si la table a été ouverte
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Ferme la table
  lastError = DbiCloseCursor(hCursor) ;

  // Teste si ça a marché
  if (DBIERR_NONE == lastError)
    _isOpen = false ;

  // Retour de l'éventuelle erreur
  return(lastError) ;
}

// -----------------------------------------------------------------------------
// Function     : NSFiche::getRecord()
// -----------------------------------------------------------------------------
// Arguments    :
// -----------------------------------------------------------------------------
// Description  : Prend l'enregistrement en cours et assigne la valeur des
//                champs aux variables membres de la classe.
// -----------------------------------------------------------------------------
// Returns      : PXERR_, s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::getRecord()
{
  // La table est-elle ouverte ?
  if (false == _isOpen)
    return (lastError = ERROR_TABLENOTOPEN) ;

  // Appelle la fonction de récupération d'enregistrement.
  lastError = getDbiRecord(dbiWRITELOCK) ;

  // Débloque l'enregistrement
  DbiRelRecordLock(PrendCurseur(), FALSE) ;
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::getDbiRecord()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  : Prend l'enregistrement courant et le place dans l'enregistrement
//	              générique associé au BCursor
// -----------------------------------------------------------------------------
// Returns      : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::getDbiRecord(DBILockType Blocage)
{
  // Rafraîchit le curseur
  DbiForceReread(hCursor) ;

  // Récupère l'enregistrement en utilisant la fonction membre
  // getRecord de la classe BCursor
  lastError = DbiGetRecord(hCursor, Blocage, pRecBuff, NULL) ;

  // Transfère le contenu du "Record Buffer" dans la fiche
  if (lastError == DBIERR_NONE)
    alimenteFiche() ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : NSFiche::insertRecord()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  :	Ajoute le contenu de la fiche au fichier en l'insérant
//            		là où est le curseur
// -----------------------------------------------------------------------------
// Returns      : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::insertRecord(DBILockType Blocage)
{
  // Transfére la fiche dans le "Record buffer"
  initialiseRecbuff() ;
  videFiche() ;

  // Insère l'enregistrement dans la table
  lastError = DbiInsertRecord(hCursor, Blocage, pRecBuff) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : NSFiche::appendRecord()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  : Ajoute le contenu de la fiche à la fin du fichier
//                (équivalent à GOTOEND + InsertRecord)
// -----------------------------------------------------------------------------
// Retour       : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::appendRecord()
{
  // Transfère la fiche dans le "Record buffer"
  initialiseRecbuff() ;
  videFiche() ;

  // Insère l'enregistrement dans la table
  lastError = DbiAppendRecord(hCursor, pRecBuff) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : NSFiche::modifyRecord(BOOL relacheBlocage)
// -----------------------------------------------------------------------------
// Arguments    : relacheBlocage : faut-il débloquer la fiche
// -----------------------------------------------------------------------------
// Description  : Met à jour la fiche sur disque à partir du contenu de la fiche
// -----------------------------------------------------------------------------
// Retour       : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::modifyRecord(BOOL relacheBlocage)
{
  // Transfère la fiche dans le "Record buffer"
  initialiseRecbuff() ;
  videFiche() ;

  // Insère l'enregistrement dans la table
  lastError = DbiModifyRecord(hCursor, pRecBuff, relacheBlocage) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : NSFiche::deleteRecord()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  : Détruit l'enregistrement pointé par le curseur
// -----------------------------------------------------------------------------
// Retour       : PXERR s'il y a lieu
// -----------------------------------------------------------------------------
DBIResult
NSFiche::deleteRecord()
{
  // Détruit l'enregistrement
  lastError = DbiDeleteRecord(hCursor, pRecBuff) ;

  // Retour de l'éventuelle erreur
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::initialiseRecbuff()
// -----------------------------------------------------------------------------
// Arguments    : AUCUN
// -----------------------------------------------------------------------------
// Description  : Met à zéro le Record Buffer
// -----------------------------------------------------------------------------
// Returns      : Rien
// -----------------------------------------------------------------------------
DBIResult
NSFiche::initialiseRecbuff()
{
  // Met à blanc le "Record buffer"
  lastError = DbiInitRecord(hCursor, pRecBuff) ;

  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : int NSFiche::creerBookMarks(int nbreBookMarks)
// -----------------------------------------------------------------------------
// Arguments    :	nbreBookMarks -> nombre de BookMarks à créer
// -----------------------------------------------------------------------------
// Description  : Crée des BookMarks
// -----------------------------------------------------------------------------
// Retour       :	0 si Ok, 1 en cas de création impossible
// -----------------------------------------------------------------------------
int
NSFiche::creerBookMarks(int nbreBookMarks)
{
try
{
	CURProps curProps ;
	lastError = DbiGetCursorProps(hCursor, curProps) ;

	if (lastError != DBIERR_NONE)
		return 1 ;

  if (pBookMark[0] || pBookMark[1])
  {
    erreur("BookMarks déjà alloués...", standardError, 0) ;
    return 1 ;
  }

	pBookMark[0] = new Byte[curProps.iBookMarkSize] ;

	if (nbreBookMarks > 1)
		pBookMark[1] = new Byte[curProps.iBookMarkSize] ;

	return 0 ;
}
catch (...)
{
  erreur("Exception NSFiche::creerBookMarks.", standardError, 0) ;
  return 1 ;
}
}


// -----------------------------------------------------------------------------
// Fonction     : int NSFiche::detruireBookMarks(int nbreBookMarks)
// -----------------------------------------------------------------------------
// Arguments    :	nbreBookMarks -> nombre de BookMarks à détruire
// -----------------------------------------------------------------------------
// Description  : Détruire des BookMarks
// -----------------------------------------------------------------------------
// Retour       :	0 si Ok, 1 en cas de destruction impossible
// -----------------------------------------------------------------------------
int
NSFiche::detruireBookMarks(int nbreBookMarks)
{
	if (pBookMark[0] == 0)
  {
    erreur("BookMark déjà détruit...", standardError, 0) ;
    return 1 ;
  }

	delete[] pBookMark[0] ;
  pBookMark[0] = 0 ;

	if (nbreBookMarks > 1)
  {
    if (pBookMark[1] == 0)
    {
      erreur("BookMark déjà détruit...", standardError, 0) ;
      return 1 ;
    }

		delete[] pBookMark[1] ;
    pBookMark[1] = 0 ;
  }

  return 0 ;
}


// -----------------------------------------------------------------------------
// Fonction     : DBIResult NSFiche::placeBookMark(int numBookMark)
// -----------------------------------------------------------------------------
// Arguments    :	numBookMark -> numéro du BookMarks à positionner
// -----------------------------------------------------------------------------
// Description  : Positionne un BookMark pour la fiche en cours
// -----------------------------------------------------------------------------
// Retour       :	DBIResult
// -----------------------------------------------------------------------------
DBIResult
NSFiche::placeBookMark(int numBookMark)
{
	// on doit inclure ici le test de validité des bookmarks (cf ChangeIndex)

	if (pBookMark[numBookMark-1])
		lastError = DbiGetBookMark(hCursor, pBookMark[numBookMark-1]) ;
	else
		lastError = DBIERR_INVALIDHNDL ;

	return (lastError) ;
}


// -----------------------------------------------------------------------------
// Fonction     : DBIResult NSFiche::retrouveBookMark(int numBookMark)
// -----------------------------------------------------------------------------
// Arguments    :	numBookMark -> numéro du BookMarks à retrouver
// -----------------------------------------------------------------------------
// Description  : Replace le curseur sur un BookMark
// -----------------------------------------------------------------------------
// Retour       :	DBIResult
// -----------------------------------------------------------------------------
DBIResult
NSFiche::retrouveBookMark(int numBookMark)
{
	// on doit inclure ici le test de validité des bookmarks (cf ChangeIndex)

	if (pBookMark[numBookMark-1])
		lastError = DbiSetToBookMark(hCursor, pBookMark[numBookMark-1]) ;
	else
		lastError = DBIERR_INVALIDHNDL ;

	return (lastError) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::DebutModeLiaison()
// -----------------------------------------------------------------------------
// Arguments    : Aucun
// -----------------------------------------------------------------------------
// Description  : Met à zéro le Record Buffer
// -----------------------------------------------------------------------------
// Returns      :	Code d'erreur
// -----------------------------------------------------------------------------
DBIResult
NSFiche::DebutModeLiaison()
{
	lastError = DbiBeginLinkMode(hCursor) ;
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::FinModeLiaison()
// -----------------------------------------------------------------------------
// Arguments    :	Aucun
// -----------------------------------------------------------------------------
// Description  : Met à zéro le Record Buffer
// -----------------------------------------------------------------------------
// Returns      :	Code d'erreur
// -----------------------------------------------------------------------------
DBIResult
NSFiche::FinModeLiaison()
{
	lastError = DbiEndLinkMode(hCursor) ;
  return (lastError) ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
DBIResult
NSFiche::NativeToAnsi(char *chaineSource, char *chaineCible)
{
  BOOL bDataLoss ;
	return (DbiNativeToAnsi(pLdObj, chaineCible, chaineSource, 0, bDataLoss)) ;
}


DBIResult
NSFiche::NativeToAnsi(char *chaine)
{
	BOOL bDataLoss ;
	return (DbiNativeToAnsi(pLdObj, chaine, chaine, 0, bDataLoss)) ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
DBIResult
NSFiche::AnsiToNative(char *chaineSource, char *chaineCible)
{
	BOOL bDataLoss ;
	return (DbiAnsiToNative(pLdObj, chaineCible, chaineSource, 0, bDataLoss)) ;
}


DBIResult
NSFiche::AnsiToNative(char *chaine)
{
	BOOL bDataLoss ;
	return (DbiAnsiToNative(pLdObj, chaine, chaine, 0, bDataLoss)) ;
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::alimenteChamp(char *pChamp, int index, int taille)
// -----------------------------------------------------------------------------
// Description  : Remplie un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::alimenteChamp(char *pChamp, int index, int taille)
{
try
{
	BOOL isBlank = FALSE ;
  Byte *pBufChamp = new Byte[taille + 1] ;

  // Prise du libellé du champ
	lastError = DbiGetField(PrendCurseur(), (Word) index, PrendRecBuff(), pBufChamp, isBlank) ;

  if (DBIERR_NONE != lastError)
  {
    erreur("Erreur de lecture d'un champ.", standardError, lastError) ;
    delete[] pBufChamp ;
    return false ;
  }

  strcpy(pChamp, (char*) pBufChamp) ;
  delete[] pBufChamp ;

  // Traduction du libellé
  if (isBlank)
    memset(pChamp, '\0', taille) ;
  else
    lastError = NativeToAnsi(pChamp) ;

  if (DBIERR_NONE != lastError)
    return false ;

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::alimenteChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::alimenteChamp(short pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Remplie un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::alimenteChamp(short *pChamp, int index)
{
try
{
  BOOL  isBlank = FALSE ;
  short pBufChamp ;

 	// Prise du libellé du champ
	lastError = DbiGetField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp, isBlank) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("Erreur de lecture d'un champ.", standardError, lastError) ;
   	return false ;
  }

  *pChamp = pBufChamp ;

  // Traduction du libellé
  if (isBlank)
    *pChamp = 0 ;

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::alimenteChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::alimenteChamp(long int pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Remplie un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::alimenteChamp(long int *pChamp, int index)
{
try
{
  BOOL      isBlank = FALSE ;
  long int  pBufChamp ;

 	// Prise du libellé du champ
	lastError = DbiGetField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp, isBlank) ;

 	if (lastError != DBIERR_NONE)
 	{
   	erreur("Erreur de lecture d'un champ.", standardError, lastError) ;
   	return false ;
  }

  *pChamp = pBufChamp ;

  // Traduction du libellé
  if (isBlank)
    *pChamp = 0 ;

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::alimenteChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::alimenteChamp(bool pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Remplie un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::alimenteChamp(bool *pChamp, int index)
{
try
{
  BOOL  isBlank = FALSE ;
  bool  pBufChamp ;

 	// Prise du libellé du champ
	lastError = DbiGetField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp, isBlank) ;

 	if (lastError != DBIERR_NONE)
 	{
   	erreur("Erreur de lecture d'un champ.", standardError, lastError) ;
   	return false ;
  }


  if ((pBufChamp && TRUE) == TRUE)
    *pChamp = true ;
  else
    *pChamp = false ;


//  *pChamp = pBufChamp ;
  // Traduction du libellé
  if (isBlank)
    *pChamp = false ;

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::alimenteChamp 2.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::alimenteBlobChamp(bool pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Remplie un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::alimenteBlobChamp(string *pChamp, int index)
{
try
{
  int   iBlobSize ;

  // ouverture du blob memo
  lastError = DbiOpenBlob(PrendCurseur(), PrendRecBuff(), (Word) index, dbiREADWRITE) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("alimenteChamp -- Erreur à l'ouverture d'un blob memo.", standardError, lastError) ;
   	return false ;
  }

  // récupération de la taille du blob memo
  lastError = DbiGetBlobSize(PrendCurseur(), PrendRecBuff(), (Word) index, iBlobSize) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("alimenteChamp -- Erreur à la lecture de la taille du blob memo.", standardError, lastError) ;
   	return false ;
  }

  char  *pcBlobInfo = (char *) malloc(iBlobSize * sizeof(BYTE) + 1) ;
  char  *pcBufChamp = (char *) malloc(iBlobSize * sizeof(BYTE) + 1) ;

  // récupération des données contenues dans le blob memo
  lastError = DbiGetBlob(PrendCurseur(), PrendRecBuff(), (Word) index, 0, iBlobSize, (BYTE *)pcBlobInfo, iBlobSize) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("alimenteChamp -- Erreur à la lecture du blob memo.", standardError, lastError) ;
    free(pcBlobInfo) ;
    free(pcBufChamp) ;
   	return false ;
  }

  // reformatage de la chaine de caractères
  pcBlobInfo[iBlobSize] = 0 ;
  OemToCharBuff(pcBlobInfo, pcBufChamp, strlen(pcBlobInfo)) ;
  pcBufChamp[iBlobSize] = 0 ;

  *pChamp = string(pcBufChamp) ;

  // libération du blob en mémoire
  free(pcBlobInfo) ;
  free(pcBufChamp) ;

  lastError = DbiFreeBlob(PrendCurseur(), PrendRecBuff(), (Word) index) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("alimenteChamp -- Erreur à la lecture du blob memo.", standardError, lastError) ;
   	return false ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::alimenteBlobChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::videChamp(char *pChamp, int index, int taille)
// -----------------------------------------------------------------------------
// Description  : Transfère une chaine vers un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::videChamp(char *pChamp, int index, int taille)
{
try
{
  char *pBufChamp ;


  // Traduction du libellé

  // On fait une copie pour éviter de modifier les données
  pBufChamp = new char[taille + 1] ;
  strcpy(pBufChamp, pChamp) ;

  lastError = AnsiToNative(pBufChamp) ;
  if (lastError != DBIERR_NONE)
  {
    delete[] pBufChamp ;
    return false ;
  }

  // Mise en place du libellé du champ
  lastError = DbiPutField(PrendCurseur(), (Word) index, PrendRecBuff(), (Byte *)pBufChamp) ;
  delete[] pBufChamp ;

  if (lastError != DBIERR_NONE)
  {
    erreur("Erreur d'écriture d'un champ.", standardError, lastError) ;
    return false ;
  }
  else
    return true ;
}
catch (...)
{
  erreur("Exception NSFiche::videChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::videChamp(short pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Transfère une chaine vers un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::videChamp(short pChamp, int index)
{
try
{
	short pBufChamp ;

  // On fait une copie pour éviter de modifier les données
  pBufChamp = pChamp ;

  // Mise en place du libellé du champ
  lastError = DbiPutField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("Erreur d'écriture d'un champ.", standardError, lastError) ;
    return false ;
  }
  else
    return true ;
}
catch (...)
{
  erreur("Exception NSFiche::videChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::videChamp(long int pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Transfère une chaine vers un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::videChamp(long int pChamp, int index)
{
try
{
	long int pBufChamp ;

  // On fait une copie pour éviter de modifier les données
  pBufChamp = pChamp ;

  // Mise en place du libellé du champ
  lastError = DbiPutField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("Erreur d'écriture d'un champ.", standardError, lastError) ;
    return false ;
  }
  else
    return true ;
}
catch (...)
{
  erreur("Exception NSFiche::videChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::videChamp(bool pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Transfère une chaine vers un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::videChamp(bool pChamp, int index)
{
try
{
	BOOL pBufChamp ;

  // On fait une copie pour éviter de modifier les données
  if (pChamp == true)
    pBufChamp = TRUE ;
  else
    pBufChamp = FALSE ;

  // Mise en place du libellé du champ
  lastError = DbiPutField(PrendCurseur(), (Word) index, PrendRecBuff(), (BYTE *)&pBufChamp) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("Erreur d'écriture d'un champ.", standardError, lastError) ;
    return false ;
  }
  else
    return true ;
}
catch (...)
{
  erreur("Exception NSFiche::videChamp.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : NSFiche::videBlobChamp(string pChamp, int index)
// -----------------------------------------------------------------------------
// Description  : Transfère une chaine vers un champ du fichier
// -----------------------------------------------------------------------------
// Returns      :	true si ça a marché, false sinon
// -----------------------------------------------------------------------------
bool
NSFiche::videBlobChamp(string pChamp, int index){
try
{
	char  pcBufChamp[1024] ;

  // On fait une copie pour éviter de modifier les données
  strcpy(pcBufChamp, pChamp.c_str()) ;

  // reformatage de la chaine de caractères
  CharToOemBuff(pcBufChamp, (char *)(pChamp.c_str()), strlen(pChamp.c_str())) ;

  // ouverture du blob memo
  lastError = DbiOpenBlob(PrendCurseur(), PrendRecBuff(), (Word) index, dbiREADWRITE) ;
 	if (lastError != DBIERR_NONE)
 	{
   	erreur("videChamp -- Erreur à l'ouverture d'un blob memo.", standardError, lastError) ;
   	return false ;
  }

  // Mise en place du libellé du champ
  lastError = DbiPutBlob(PrendCurseur(), PrendRecBuff(), (Word) index, 0, strlen(pcBufChamp), (BYTE *)(pcBufChamp)) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("videChamp -- Erreur d'écriture d'un champ.", standardError, lastError) ;
    return false ;
  }

  lastError = DbiModifyRecord(PrendCurseur(), PrendRecBuff(), TRUE) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("videChamp -- Erreur à la modification d'un champ.", standardError, lastError) ;
    return false ;
  }

  lastError = DbiFreeBlob(PrendCurseur(), PrendRecBuff(), (Word) index) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("videChamp -- Erreur à la libération du blob memo.", standardError, lastError) ;
    return false ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception NSFiche::videBlobChamp.", standardError, 0) ;
  return false ;
}
}

