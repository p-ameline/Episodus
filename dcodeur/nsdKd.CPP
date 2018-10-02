/*  +-----------------------------------------------------------------+  */
/*  ¦                  Programme secondaire NSDCODE                   ¦  */
/*  ¦                                                                 ¦  */
/*  ¦             Utilitaires utilisés par les décodeurs              ¦  */
/*  ¦                                                                 ¦  */
/*  ¦             Debut d'ecriture : 17 Juillet 1995 (PA)             ¦  */
/*  ¦             Derniere version : 17 Juillet 1995 (PA)             ¦  */
/*  +-----------------------------------------------------------------+  */
#ifndef __linux__
#include <owl\olemdifr.h>
#endif
#include <stdlib.h>
#include <string>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
  #include "partage/nscim10.h"
  #include "partage/nsmatfic.h"
#endif

#include "nssavoir/nsguide.h"
#include "nssavoir/nsfilgd.h"
#include "nssavoir/nspatho.h"
#include "nsbb/nstlibre.h"
#include "nsbb/nspatpat.h"
#include "dcodeur/nsdkd.h"

#ifndef __linux__
#include "dcodeur/nsdecode.h"
#include "dcodeur/nsdecode.rh"
#endif

#include "dcodeur/nsphrase.h"
#include "dcodeur/nsgenlan.h"
#include "nsepisod/eptables.h"
#include "dcodeur/nscr_amx.h"
#include "dcodeur/nscr_ama.h"

long dkdNoyau::lObjectCount = 0 ;
long gereDate::lObjectCount = 0 ;
long numStorage::lObjectCount = 0 ;
long gereNum::lObjectCount = 0 ;
long gereHeure::lObjectCount = 0 ;
long gereCode::lObjectCount = 0 ;
long NSDkdPhrase::lObjectCount = 0 ;
long decodageBase::lObjectCount = 0 ;

//-------------------------------------------------------------------------
//
//                       METHODES DE dkdNoyau
//
//-------------------------------------------------------------------------
dkdNoyau::dkdNoyau(NSContexte* pCtx) : NSRoot(pCtx)
{
#ifndef __linux__
  _hDcode = 0 ;
#endif

  _pPatPathoArray = (NSPatPathoArray*) 0 ;
  _itDcode 	      = (PatPathoIter) 0 ;
  _itMDcode 	    = (PatPathoIter) 0 ;
  _sCodeLexique   = string("") ;
  _sComplement    = string("") ;

  _sLocLesion 	  = string("") ;

  _sCodeLexUtil   = string("") ;
  _bCodeUtilValid = false ;
  _sCodeLexDecal  = string("") ;
  _sComplDecal    = string("") ;

  _pDkdPhrases    = (NSDkdPhraseArray*) 0 ;
  _pPhraLes	      = (NSCRPhraLesArray*) 0 ;
  _pPhrases	      = (NSCRPhraseArray*) 0 ;
  _sDcodeur	      = string("") ;
  _iBon		        = 1 ;
  _iCorrigeAnnule = 0 ;
  _bRecupFenetre  = false ;

  lObjectCount++ ;
}

dkdNoyau::~dkdNoyau()
{
  lObjectCount-- ;

#ifndef __linux__
  if (_hDcode)
    CloseHandle(_hDcode) ;
#endif

  // Ne surtout pas tuer la PatPathoArray, on n'en est pas propriétaire
  //
  //if (pPatPathoArray)
  //	delete pPatPathoArray;

  /*if (pPhraLes)
   	delete pPhraLes;
	if (pPhrases)
   	delete pPhrases; */
}

void
dkdNoyau::addSentenceToDcodeur(const string sC)
{
  if (string("") == sC)
    return ;

  if (string("") != _sDcodeur)
    _sDcodeur += string(" ") ;

  _sDcodeur += sC ;
}

string
dkdNoyau::getNodeAsString(const PatPathoIter it) const
{
  PatPathoIter nodeIt ;
  if ((PatPathoIter) NULL == it)
    nodeIt = _itDcode ;
  else
    nodeIt = it ;

  string sNodeString = (*nodeIt)->getLexique() ;

  if (string("") != (*nodeIt)->getPluriel())
    sNodeString += string(1, cheminSeparationMARK) + (*nodeIt)->getPluriel() ;

  if (string("") != (*nodeIt)->getCertitude())
    sNodeString += string(1, cheminSeparationMARK) + (*nodeIt)->getCertitude() ;

  return sNodeString ;
}

void
dkdNoyau::VaMarque()
{
  _itDcode = _itMDcode ;

  _bCodeUtilValid = false ;

  int ligne = (*_itDcode)->getLigne() ;

  // On est positionné sur le premier élément de la ligne, on traite toute
  // la ligne avec un autre itérateur pour laisser itDcode en 1ère position
  // (sinon getCol serait faux)
  //
  _sCodeLexique = string("") ;

  // On synchronise itDcodeTemp avec itDcode
  //
  PatPathoIter itDcodeTemp = _pPatPathoArray->begin() ;
  while ((*itDcodeTemp)->getLocalisation() != (*_itDcode)->getLocalisation())
  	itDcodeTemp++ ;

  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  while ((_pPatPathoArray->end() != itDcodeTemp) &&
   		                          ((*itDcodeTemp)->getLigne() == ligne))
  {
  	if (string("") != _sCodeLexique)
    	_sCodeLexique += string(1, cheminSeparationMARK) ;

    _sCodeLexique += getNodeAsString(itDcodeTemp) ;

    itDcodeTemp++ ;
  }
}

void
dkdNoyau::refreshCodeLexUtil()
{
  pContexte->getDico()->donneCodeSens(&_sCodeLexique, &_sCodeLexUtil) ;
  _bCodeUtilValid = true ;
}

//-------------------------------------------------------------------------
//
//                       METHODES DE decodageBase
//
//-------------------------------------------------------------------------

//---------------------------------------------------
//  Constructeur initial (crée un noyau)
//---------------------------------------------------
decodageBase::decodageBase(NSContexte* pCtx, const string sLang)
             :NSRoot(pCtx)
{
try
{
	_pNoyau 	  = new dkdNoyau(pContexte) ;
  _bCreateur = true ;

  setDsFichier(true) ;
  _pPhraseur = new NSPhraseur(pCtx) ;
  _pPropos   = new NsProposition(pContexte, &_pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct) ;
  _sLangue   = sLang ;

  _pGenerateur = createGenerator(_pPropos) ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception à la création de l'objet de décodage.", standardError) ;
}
}

//-----------------------------------------------------------------
//  Constructeur complémentaire (utilise le noyau de l'autre objet)
//-----------------------------------------------------------------
decodageBase::decodageBase(const decodageBase* ptr)
             :NSRoot(ptr->pContexte)
{
try
{
	_pNoyau 	 = ptr->_pNoyau ;
  _bCreateur = false ;

  _pPhraseur = new NSPhraseur(pContexte) ;
  _pPropos   = new NsProposition(pContexte, &_pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct) ;
  _sLangue   = ptr->_sLangue ;

  _pGenerateur = createGenerator(_pPropos) ;

  lObjectCount++ ;
}
catch (...)
{
  erreur("Exception à la création de l'objet de décodage.", standardError) ;
}
}

//---------------------------------------------------
//  Destructeur
//---------------------------------------------------
decodageBase::~decodageBase()
{
	if (_bCreateur && _pNoyau)
  	delete _pNoyau ;

  if (_pPropos)
  	delete _pPropos ;

	// if (pPhraseur)
  //    delete pPhraseur ;

  if (_pGenerateur)
  	delete _pGenerateur ;

  lObjectCount-- ;
}

NSGenerateur*
decodageBase::createGenerator(NsProposition* pProp)
{
  //
  // Warning : pGenerateur must be instanciated, we use the french one as default
  //
  if (string("en") == _sLangue)
  	return new NSGenerateurEn(pContexte, pProp, string("en")) ;
  else
  	return new NSGenerateurFr(pContexte, pProp, string("fr")) ;
}

//
// ---------- Avance d'une ou plusieurs lignes -------
//
// ATTENTION : On avance de nbPositions lignes et non de nbPositions éléments
// 				(il peut y avoir plusieurs éléments sur une même ligne)
//
//					l'itérateur reste positionné sur le 1er élément de la ligne
//					afin que getCol puisse renvoyer la bonne colonne
//
void
decodageBase::Avance(const int nbPositions)
{
	if (nbPositions <= 0)
  	return ;

  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;

	if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
  	return ;

	_pNoyau->setCodeUtilValid(false) ;
	//
	// On avance de nbPositions lignes
	//
	// itDcode pointe sur le premier élément de la ligne
	//
  PatPathoIter it = _pNoyau->getitDcode() ;

	int ligne = (*it)->getLigne() ;
	int i     = 0 ;
	while ((i < nbPositions) && (pPPT->end() != it))
	{
  	_pNoyau->nextItDcode() ;
    it = _pNoyau->getitDcode() ;

    if ((pPPT->end() != it) &&
      	    ((*it)->getLigne() != ligne))
    {
    	i++ ;
      ligne = (*it)->getLigne() ;
    }
	}

  _pNoyau->setCodeLexique(string("")) ;

  if (i < nbPositions)
  {
    string ps = string("decodageBase::Avance: Bad tree element on line ") + _pNoyau->getLocLesion() ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return ;
  }

  //
  // On est positionné sur le premier élément de la ligne, on traite toute
  // la ligne avec un autre itérateur pour laisser itDcode en 1ère position
  // (sinon getCol serait faux)
  //

  //
  // On synchronise itDcodeTemp avec itDcode
  //
	PatPathoIter itDcodeTemp = pPPT->begin() ;
	while ((*itDcodeTemp)->getLocalisation() != (*getitDcode())->getLocalisation())
		itDcodeTemp++ ;
  //
  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  while ((pPPT->end() != itDcodeTemp) && ((*itDcodeTemp)->getLigne() == ligne))
	{
		if (string("") != _pNoyau->getCodeLexique())
    	_pNoyau->addToCodeLexique(string(1, cheminSeparationMARK)) ;

    string sNodeAsString = _pNoyau->getNodeAsString(itDcodeTemp) ;

    _pNoyau->addToCodeLexique(sNodeAsString) ;

    itDcodeTemp++ ;
	}
}

void
decodageBase::Recule(const int nbPositions)
{
	if (nbPositions <= 0)
		return ;

	if (_pNoyau->getPatPathoArray()->begin() == getitDcode())
		return ;

	_pNoyau->setCodeUtilValid(false) ;

  // On recule de nbPositions lignes
  //
  // itDcode pointe sur le premier élément de la ligne
  //
	int  ligne = (*getitDcode())->getLigne() ;
	int  i 		 = 0 ;
	bool ontourne = true ;
	while ((i < nbPositions) && ontourne)
	{
  	_pNoyau->previousItDcode() ;
    if ((*getitDcode())->getLigne() != ligne)
    {
    	i++ ;
      ligne = (*getitDcode())->getLigne() ;
    }
    if (_pNoyau->getitDcode() == _pNoyau->getPatPathoArray()->begin())
    	ontourne = false ;
	}

	_pNoyau->setCodeLexique(string("")) ;

	if (i < nbPositions)
		return ;

	//
	// On remplit codeLexique tant qu'on est sur la même ligne
	//
	string sNouvEl = string("") ;
	ontourne = true ;
	while (((*getitDcode())->getLigne() == ligne) && ontourne)
	{
    string sNodeAsString = _pNoyau->getNodeAsString() ;

    if (string("") != _pNoyau->getCodeLexique())
    	sNodeAsString += string(1, cheminSeparationMARK) ;

    _pNoyau->setCodeLexique(sNodeAsString + _pNoyau->getCodeLexique()) ;

    if (_pNoyau->getitDcode() == _pNoyau->getPatPathoArray()->begin())
    	ontourne = false ;
    else
    	_pNoyau->previousItDcode() ;
	}

	if (ontourne)
		_pNoyau->nextItDcode() ;
}

// Passe à l'élément suivant, éventuellement sur la même ligne, alors que
// la fonction Avance() passe à la ligne suivante
void
decodageBase::Suivant()
{
	_pNoyau->nextItDcode() ;
	_pNoyau->setCodeLexique((*getitDcode())->getLexique()) ;
}

// Passe à l'élément précédent, éventuellement sur la même ligne, alors que
// la fonction Recule() se positionne au début de la ligne précédente
void
decodageBase::Precedent()
{
	_pNoyau->previousItDcode() ;
	_pNoyau->setCodeLexique((*getitDcode())->getLexique()) ;
}

//
// ------------- Renvoie le code sens de l'élément en cours ---------
//
string
decodageBase::getSt() const
{
	if (false == _pNoyau->isCodeUtilValid())
		_pNoyau->refreshCodeLexUtil() ;

	return _pNoyau->getCodeLexUtil() ;
}

int
decodageBase::getCol() const
{
	if (_pNoyau->getPatPathoArray()->end() == getitDcode())
    return -1 ;

  return (*getitDcode())->getColonne() ;
}

void
decodageBase::setPPtArray(NSPatPathoArray* pPPtArray)
{
	if ((NSPatPathoArray*) NULL == pPPtArray)
    return ;

  _pNoyau->setPatPathoArray(pPPtArray) ;
  _pNoyau->setitDcode(_pNoyau->getPatPathoArray()->begin()) ;
}

string
decodageBase::getCpl()
{
	_pNoyau->refreshComplement() ;
	return _pNoyau->getComplement() ;
}

void
decodageBase::getCert(string* pCert) const
{
	if ((string*) NULL == pCert)
		return ;

	*pCert = (*getitDcode())->getCertitude() ;
}

void
decodageBase::getPlur(string* pPlur) const
{
	if ((string*) NULL == pPlur)
		return ;

	*pPlur = (*getitDcode())->getPluriel() ;
}

void
decodageBase::MetMarque()
{
	_pNoyau->MetMarque() ;
}

void
decodageBase::VaMarque()
{
	_pNoyau->VaMarque() ;
}

void
decodageBase::fermeFichier()
{
#ifndef __linux__
	CloseHandle(_pNoyau->getHandler()) ;
	_pNoyau->setHandler(0) ;
#endif
}

void
decodageBase::initialiseIterateurs()
{
  if ((dkdNoyau*) NULL == _pNoyau)
    return ;

  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;

  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return ;

	_pNoyau->setitDcode(pPPT->begin()) ;
	_pNoyau->setitMDcode(pPPT->begin()) ;

  if (pPPT->end() != getitDcode())
  	_pNoyau->setCodeLexique(_pNoyau->getNodeAsString()) ;
  else
  	_pNoyau->setCodeLexique(string("")) ;

	set_iBon(1) ;
}

void
decodageBase::ouvreFichier(const char *fichier)
{
#ifndef __linux__
  HANDLE hFileHa = CreateFile(fichier,	              // address of name of the file
                              GENERIC_WRITE,				  // access (read-write) mode
                              0,								      // share mode
                              0,								      // address of security descriptor
                              CREATE_ALWAYS,				  // how to create
                              FILE_ATTRIBUTE_NORMAL, // file attributes
                              0 				              // handle of file with attributes to copy
                             ) ;

  _pNoyau->setHandler(hFileHa) ;
#endif
}

//---------------------------------------------------------------------------
//  Fonction 	 : void metPhrase(char decal[])
//
//  Description : Met sDcodeur[] dans GASTRODATA
//
//	 Arguments : 	decDeb    -> caractères de début
//						decFin    -> caractères de fin
//						sautLigne -> 0 ligne normale
//										 1 saut d'une ligne
//										 -1 ligne collée à la suivante
//
//	 Retour	  :	Rien
//----------------- Créé le 14/11/1988  Dernière mise à jour le 16/11/1988 --
void
decodageBase::metPhrase(const string decDeb, const string decFin, const int sautLigne)
{
#ifndef __linux__
	//
	if (false == _pNoyau->isGood())
		return ;
	//
	// Inscription des lésions
	//
	bool 	bWOK ;
	DWORD nbEcrit ;

    /*metUnCar('<');
	if (!pPhraLes->IsEmpty())
	{
		iMin = pPhraLes->LowerBound();
		iMax = iMin + pPhraLes->GetItemsInContainer();
		for (i = iMin; i < iMax; i++)
		{
			iNumLes = ((*pPhraLes)[i]).iNumeroLesion;
			numacar(cNumLes, iNumLes, 2);
         bWOK = WriteFile(dcode, cNumLes, 2, &nbEcrit,	0);
         metUnCar(((*pPhraLes)[i]).cType);
         metUnCar(((*pPhraLes)[i]).cTypeIndex);
		}
		pPhraLes->Flush();
	}
    metUnCar('>'); */
	//
	// Mise dans le fichier
	//

	size_t nbChar ;
	//
	//
	//
	if (getDsFichier())
	{
  	if ((string("") != _pNoyau->getLocLesion()) ||
        (string("") != _pNoyau->getLocPath()))
    {
    	metUnCar('|') ;
      if (string("") != _pNoyau->getLocLesion())
      {
      	nbChar = strlen(_pNoyau->getLocLesion().c_str()) ;
        bWOK = WriteFile(_pNoyau->getHandler(), _pNoyau->getLocLesion().c_str(), nbChar, &nbEcrit, 0) ;

        _pNoyau->setLocLesion(string("")) ;
      }
      if (string("") != _pNoyau->getLocPath())
      {
      	nbChar = strlen(_pNoyau->getLocPath().c_str()) ;
        bWOK = WriteFile(_pNoyau->getHandler(), _pNoyau->getLocPath().c_str(), nbChar, &nbEcrit, 0) ;

        _pNoyau->setLocPath(string("")) ;
      }
      metUnCar('|') ;
    }
    //
    // Caractères de début
    //
    if ((string("") == decDeb) && (string("") == decFin))
    {
    	metUnCar(27) ;
      metUnCar('T') ;
    }
    else if (string("") != decDeb)
    {
    	metUnCar(27) ;
      nbChar = strlen(decDeb.c_str()) ;
      bWOK = WriteFile(_pNoyau->getHandler(), decDeb.c_str(), nbChar, &nbEcrit, 0) ;
    }
    //
    // Texte
    //
    nbChar = strlen(_pNoyau->getDcodeur().c_str()) ;
    if (nbChar > 0)
      bWOK = WriteFile(_pNoyau->getHandler(), _pNoyau->getDcodeur().c_str(), nbChar, &nbEcrit, 0) ;
    //
    // Caractères de fin
    //
    if (1 == sautLigne)
    {
    	metUnCar(13) ;
      metUnCar(10) ;
    }
    //
    if (sautLigne >= 0)
    {
    	metUnCar(13) ;
      metUnCar(10) ;
    }
    if ((string("") == decDeb) && (string("") == decFin))
    {
    	metUnCar(28) ;
      metUnCar('T') ;
    }
    else if (string("") != decFin)
    {
    	metUnCar(28) ;
      nbChar = strlen(decFin.c_str()) ;
      bWOK = WriteFile(_pNoyau->getHandler(), decFin.c_str(), nbChar, &nbEcrit,	0) ;
    }
	}
	else
	{
try
{
		//
    // Mise dans l'array de phrases
    //
    NSCRPhrase* pPhra = new NSCRPhrase ;
    pPhra->sTexte 	  = _pNoyau->getDcodeur() ;
    getPhA()->push_back(pPhra) ;
}
catch (...)
{
    erreur("Exception à la copie dans l'array de phrases.", standardError) ;
}
	}
	return ;
#endif
}

bool
decodageBase::metUnCar(const char cCar)
{
#ifndef __linux__
	char 	cF[2];
  DWORD nbEcrit;
  cF[0] = cCar; cF[1] = '\0';
  return WriteFile(_pNoyau->getHandler(), cF, 1, &nbEcrit, 0) ;
#else
  return false ;
#endif
}

void
decodageBase::vidangeDkdPhrases()
{
  NSDkdPhraseArray* pArray = getDkA() ;
  if (((NSDkdPhraseArray*) NULL == pArray) || pArray->empty())
    return ;

  // On met de côté les paramètres globaux utilisés
  string sBackupLocLesion = _pNoyau->getLocLesion() ;
  string sBackupDecodeur  = _pNoyau->getDcodeur() ;

  // On traite les phrases
  //
  for (NSDkdPhrArrayIter i = pArray->begin() ; pArray->end() != i ; i++)
  {
    _pNoyau->setLocLesion((*i)->locLesion) ;
    _pNoyau->setDcodeur((*i)->sTexte) ;

    metPhrase((*i)->decDeb, (*i)->decFin, (*i)->sautLigne);
  }

  // On remet en place les paramètres globaux
  _pNoyau->setLocLesion(sBackupLocLesion) ;
  _pNoyau->setDcodeur(sBackupDecodeur) ;
}

//  +-----------------------------------------------------------------+
//  ¦          Prise du code utile decal caractères plus loin         ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
string*
decodageBase::getSt(int decal)
{
	_pNoyau->MetMarque() ;

  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return _pNoyau->getCodeLexDecalPointer() ;

	for (int i = 0 ; (i < decal) && (pPPT->end() != _pNoyau->getitMDcode()) ; i++)
		_pNoyau->nextItMDcode() ;

	if (pPPT->end() != _pNoyau->getitMDcode())
		_pNoyau->setCodeLexDecal((*getitMDcode())->getLexique()) ;
	else
		_pNoyau->setCodeLexDecal(string("")) ;

	return _pNoyau->getCodeLexDecalPointer() ;
}

//  +-----------------------------------------------------------------+
//  ¦         Prise du code complet decal caractères plus loin        ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
string*
decodageBase::getStL(int decal)
{
	_pNoyau->MetMarque() ;

  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return _pNoyau->getCodeLexDecalPointer() ;

	for (int i = 0 ; (i < decal) && (pPPT->end() != _pNoyau->getitMDcode()) ; i++)
		_pNoyau->nextItMDcode() ;

	if (pPPT->end() != _pNoyau->getitMDcode())
		_pNoyau->setCodeLexDecal((*getitMDcode())->getLexique()) ;
	else
		_pNoyau->setCodeLexDecal(string("")) ;

	return _pNoyau->getCodeLexDecalPointer() ;
}

//  +-----------------------------------------------------------------+
//  ¦         Prise de la fiche matériel de la fiche en cours         ¦
//  +-----------------------------------------------------------------+
//  Créé le 30/11/1990 Dernière mise à jour 07/11/1990
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getMateriel(NSMaterielInfo* pMatInfo)
{
	if ((NSMaterielInfo*) NULL == pMatInfo)
  	return DBIERR_NONE ;

	pMatInfo->reset() ;

  if (getSt() != string("£SGMA"))
    return DBIERR_NONE ;

  string sCode = getCpl() ;
	pMatInfo->initialiseDepuisObjet(pContexte->getSuperviseur(), sCode) ;

	return DBIERR_NONE ;
}
#endif

//  +-----------------------------------------------------------------+
//  ¦           Prise de la fiche CDAM de la fiche en cours          ¦
//  +-----------------------------------------------------------------+
//  Créé le 12/10/1998 Dernière mise à jour 12/10/1998
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getCIM10(NSCim10Info* pCimInfo, NSCim10* pCim)
{
  if ((NSCim10Info*) NULL == pCimInfo)
    return DBIERR_NONE ;

	pCimInfo->Donnees.metAZero() ;

	if (getSt() != string("£CI"))
		return DBIERR_NONE ;

	DBIResult Erreur ;

	//
	// Ouverture du fichier
	//
	bool bOuvreFerme ;
	NSCim10* pCim10 ;

	if (pCim)
	{
		bOuvreFerme = false ;
		pCim10 = pCim;
	}
	else
	{
		bOuvreFerme = true ;
		pCim10 = new NSCim10(pContexte->getSuperviseur()) ;
		if (NULL == pCim10)
			return DBIERR_NONE ;

		pCim10->lastError = pCim10->open() ;
		if (pCim10->lastError != DBIERR_NONE)
		{
    	Erreur = pCim10->lastError ;
			erreur("Erreur à l'ouverture du fichier CIM10.", standardError, pCim10->lastError) ;
			delete pCim10 ;
			return Erreur ;
    }
	}
	//
	// Recherche du matériel dont le code est dans le champ complément
	//
  string sMatCode = getCpl() ;
	pCim10->lastError = pCim10->chercheClef(&sMatCode,
                                          "",
                                       		0,
                                       		keySEARCHEQ,
                                       		dbiWRITELOCK) ;
	if (pCim10->lastError != DBIERR_NONE)
	{
  	Erreur = pCim10->lastError ;
		erreur("Erreur à la recherche du code CIM10.", standardError, pCim10->lastError) ;
		if (bOuvreFerme)
    {
    	pCim10->close() ;
      delete pCim10 ;
    }
		return Erreur ;
	}
	pCim10->lastError = pCim10->getRecord() ;
	if (pCim10->lastError != DBIERR_NONE)
	{
  	Erreur = pCim10->lastError ;
		erreur("Erreur à la lecture du code CIM10.", standardError, pCim10->lastError) ;
		if (bOuvreFerme)
    {
    	pCim10->close() ;
      delete pCim10 ;
    }
		return Erreur ;
	}
	//
  // Si tout a bien marché, on met à jour pMatInfo
  //
	pCimInfo->Donnees = *(pCim10->pDonnees) ;

	if (bOuvreFerme)
	{
		pCim10->close() ;
		delete pCim10 ;
	}

	return DBIERR_NONE ;
}
#endif

//  +-----------------------------------------------------------------+
//  ¦           Prise de la fiche CCAM de la fiche en cours          ¦
//  +-----------------------------------------------------------------+
//  Créé le 03/10/2004 Dernière mise à jour 03/10/2004
#ifndef _ENTERPRISE_DLL
DBIResult
decodageBase::getCcam(NSCcamInfo* pCcamInfo, NSCcam* pCcam)
{
	if ((NSCcamInfo*) NULL == pCcamInfo)
		return DBIERR_NONE ;

	pCcamInfo->metAZero() ;

	if (getSt() != string("6CCAM"))
		return DBIERR_NONE ;

	if (pCcam)
  	return pCcam->getRecordByCode(getCpl(), pCcamInfo, false) ;

  NSCcam CCAM(pContexte->getSuperviseur()) ;
  return CCAM.getRecordByCode(getCpl(), pCcamInfo, true) ;
}
#endif

//  +-----------------------------------------------------------------+
//  ¦       Prise de la fiche correspondant de la fiche en cours      ¦
//  +-----------------------------------------------------------------+
//  Créé le 27/03/1998 Dernière mise à jour 27/03/1998

bool
decodageBase::getCorresp(NSPersonInfo* pPersonInfo)
{
#ifndef _ENTERPRISE_DLL
	// pCorInfo->pDonnees->metAZero() ;

	if (getSt() != string("£SGDR"))
  	return false ;

	string sPersonId = getCpl() ;
  if (string("") == sPersonId)
		return false ;

	NSPersonInfo* pPersonInfoPtr = pContexte->getPersonArray()->getPerson(pContexte, sPersonId, pidsCorresp) ;
  if (NULL == pPersonInfoPtr)
  	return false ;

	*pPersonInfo = *pPersonInfoPtr ;
#endif

	return true ;
}

/*
DBIResult
decodageBase::getCorresp(NSCorrespondantInfo* pCorInfo)
{
	pCorInfo->pDonnees->metAZero() ;

	if (*getSt() != "£SGDR")
  	return DBIERR_NONE ;
  //
	// Ouverture du fichier
	//
	NSCorrespondant Cor(pContexte) ;

	Cor.lastError = Cor.open() ;
	if (Cor.lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier des correspondants.", standardError, Cor.lastError) ;
		return Cor.lastError ;
	}
	//
	// Recherche du matériel dont le code est dans le champ complément
	//
	Cor.lastError = Cor.chercheClef(getCpl(),
    									"",
									   	0,
									   	keySEARCHEQ,
									   	dbiWRITELOCK) ;
	if (Cor.lastError != DBIERR_NONE)
	{
    Cor.close() ;
		erreur("Erreur à la recherche du correspondant.", standardError, Cor.lastError) ;
		return Cor.lastError ;
	}
	Cor.lastError = Cor.getRecord() ;
	if (Cor.lastError != DBIERR_NONE)
	{
    Cor.close() ;
		erreur("Erreur à la lecture du correspondant.", standardError, Cor.lastError) ;
		return Cor.lastError ;
	}

	//
  // Si tout a bien marché, on met à jour pCorInfo
  //
  *(pCorInfo->pDonnees) = *(Cor.pDonnees) ;
  Cor.close() ;


	return DBIERR_NONE ;
}
*/

//---------------------------------------------------------------------------
//  Function 	 : void et_du_milieu(type, type1, type2, entre)
//
//  Description : Assemblage du type , , et : 1ère étape
//
//	 ARGUMENTS :
//
//	 Retour	  :	Rien
//
//  Créé le 21/06/1993  Dernière mise à jour le 21/06/1993
void
decodageBase::etDuMilieu(string *type, string *type1, string *type2, const string entre)
{
	if ((NULL == type) || (NULL == type1) || (NULL == type2))
		return ;

	if (string("") == *type2)
		return ;

	if (string("") != *type)
		*type += entre ;
	*type += *type1 ;
	*type1 = *type2 ;

	*type2 = string("") ;
}

//---------------------------------------------------------------------------
//  Function 	 : void et_final(type, type1, fin)
//
//  Description : Assemblage du type , , et : 2ème étape
//
//	 ARGUMENTS :
//
//	 Retour	  :	Rien
//
//  Créé le 21/06/1993  Dernière mise à jour le 21/06/1993
void
decodageBase::etFinal(string *type, string *type1, const string fin)
{
  if ((NULL == type) || (NULL == type1))
    return ;
	if (string("") == *type1)
		return ;

	if (string("") != *type)
		*type += fin ;
	*type += *type1 ;

  *type1 = string("") ;
}

//  +-----------------------------------------------------------------+
//  ¦               Décodage des données numériques                   ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneDimension(int colonne, gereNum* pNum)
{
	if ((gereNum*) NULL == pNum)
		return ;

	if (getCol() <= colonne)
		return ;

	string sUnite  = string("") ;
  string sFormat = string("") ;
  string sValeur = string("") ;

  int iIndice = -1 ;

  // If it is not a numerical information, we have to come back here
  //
  MetMarque() ;

  // On traite les éventuelles valeurs normales
  //
	int refCol = getCol() ;

	while ((getCol() > colonne) && iBon())
	{
    string sNodeSemLex = getSt() ;

  	// Value
    //
  	if 	    ((string("") != sNodeSemLex) && ('£' == sNodeSemLex[0]))
    {
    	sFormat = (*getitDcode())->getLexique() ;
      sValeur = (*getitDcode())->getComplement() ;
      sUnite  = (*getitDcode())->getUnit() ;
      Avance() ;

      // sFormat est du type £N0;03
      //
    	if ((string("") == sFormat) || ('N' != sFormat[1]))
    	{
      	VaMarque() ;
      	Recupere() ;
   	    return ;
    	}

      iIndice++ ;

    	pNum->instancier(&sValeur, &sUnite, &sFormat, iIndice) ;
    }
    //
    // Normal values
    //
    else if (string("VNOMA") == sNodeSemLex)
    {
    	Avance() ;
      gereNum Normale(pContexte->getSuperviseur(), _sLangue) ;
      donneDimension(refCol, &Normale) ;
      pNum->setNormale(&Normale, iIndice) ;
    }
    else if (string("VNOMI") == sNodeSemLex)
    {
    	Avance() ;
      gereNum NormInf(pContexte->getSuperviseur(), _sLangue) ;
      donneDimension(refCol, &NormInf) ;
      pNum->setNormInf(&NormInf, iIndice) ;
    }
    else if (string("VNOMS") == sNodeSemLex)
    {
    	Avance() ;
      gereNum NormSup(pContexte->getSuperviseur(), _sLangue) ;
      donneDimension(refCol, &NormSup) ;
      pNum->setNormSup(&NormSup, iIndice) ;
    }
    //
    // Value time stamp
    //
    else if (string("KDARE") == sNodeSemLex)
    {
    	Avance() ;
      gereDate* pDate = new gereDate(pContexte->getSuperviseur(), _sLangue) ;
      donneDate(refCol, pDate) ;
      pNum->setDate(pDate) ;
    }
    else
    	break ;
  }

  if (-1 == iIndice)
  {
  	VaMarque() ;
  	Recupere() ;
  }
}

//  +-----------------------------------------------------------------+
//  ¦                     Décodage des dates                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneDate(int colonne, gereDate* pDate)
{
	if ((getCol() <= colonne) || (NULL == pDate))
    return ;

	string sUnite  = string("") ;
	string sFormat = string("") ;
	string sValeur = string("") ;

  string sLexique = (*getitDcode())->getLexique() ;

  if ((string("") != sLexique) && ('£' == sLexique[0]))
  {
  	string sTemp = (*getitDcode())->getLexique() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
    sValeur = (*getitDcode())->getComplement() ;
    sTemp   = (*getitDcode())->getUnit() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
	}

	// sFormat est du type £D0;10 ou bien £T0;19
	if ((sValeur == "") || (sFormat == "") || ((sFormat[1] != 'D') && (sFormat[1] != 'T')))
	{
    Recupere() ;
  	return ;
	}

	pDate->setDate(&sValeur) ;
	pDate->setFormat(&sFormat) ;
	pDate->setUnite(&sUnite) ;

	Avance() ;
}

//  +-----------------------------------------------------------------+
//  ¦                     Décodage des heures                         ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneHeure(int colonne, gereHeure* pHeure)
{
	if ((getCol() <= colonne) || (NULL == pHeure))
		return ;

	string sUnite  = string("") ;
	string sFormat = string("") ;
	string sValeur = string("") ;

  string sLexique = (*getitDcode())->getLexique() ;
  if ((string("") != sLexique) && ('£' == sLexique[0]))
  {
  	string sTemp = (*getitDcode())->getLexique() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
    sValeur = (*getitDcode())->getComplement() ;
    sTemp   = (*getitDcode())->getUnit() ;
    pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
  }

  // sFormat est du type £D0;03
  if ((string("") == sFormat) || ('H' != sFormat[1]))
  {
  	Recupere() ;
    return ;
  }

  pHeure->setHeure(&sValeur) ;
  pHeure->setFormat(&sFormat) ;
  pHeure->setUnite(&sUnite) ;

  Avance() ;
}

//
// Libellé issu d'une classification
// Classification : Lib and Code
//
void
decodageBase::donneCode(int colonne, gereCode* pCode)
{
	if ((getCol() <= colonne) || (NULL == pCode))
		return ;

  string sClassif = string("") ;
  string sCode    = string("") ;
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	//
  // Recuperation du code et de la classification
	// Getting code and classification
	//
	string sTemp = (*getitDcode())->getLexique() ;
	NSDico::donneCodeSens(&sTemp, &sClassif);
	sCode = (*getitDcode())->getComplement() ;

	// Il faut un code - Must have a code
  if (string("") == sCode)
	{
		Recupere() ;
    return ;
	}
  // La classification doit etre connue - Must have a known classification
  if (false == pSuper->getFilGuide()->VraiOuFaux(sClassif, string("ES"), string("0CODE")))
	{
  	Recupere() ;
    return ;
	}

	pCode->setCode(&sCode) ;
	pCode->setClassification(&sClassif) ;

	Avance() ;
}

void
decodageBase::getGereDate(gereDate** ppDate)
{
	if      (string("fr") == _sLangue)
		*ppDate = new gereDateFr(pContexte->getSuperviseur(), _sLangue) ;
	else if (string("en") == _sLangue)
		*ppDate = new gereDateEn(pContexte->getSuperviseur(), _sLangue) ;
}

//  +-----------------------------------------------------------------+
//  ¦                 Donne les caractères libres                     ¦
//  +-----------------------------------------------------------------+
//  Créé le 07/11/1990 Dernière mise à jour 07/11/1990
void
decodageBase::donneCarLibre(int colonne, string* carLibre, GENRE* pGenre)
{
  if ((string*) NULL == carLibre)
    return ;
  *carLibre = string("") ;

	while ((getCol() > colonne) && _pNoyau->isGood())
	{
		// Texte libre
		if (getSt() == string("£??"))
    {
    	*carLibre = getTexteLibre() ;
      Avance() ;

      if (pGenre)
      	*pGenre = genreNull ;
    }
    else
    {
    	NSPathologData Data ;
      string sLex = getStL() ;
      bool bFound = pContexte->getDico()->trouvePathologData(_pGenerateur->getLang(), &sLex, &Data) ;
      if (bFound)
      {
      	_pGenerateur->donneLibelleAffiche(carLibre, &Data) ;
        if (pGenre)
          Data.donneGenre(pGenre) ;
      }

      Avance() ;
    }
  }
}

/*  +-----------------------------------------------------------------+  */
/*  ¦                 Donne les caractères libres                     ¦  */
/*  +-----------------------------------------------------------------+  */
/*  Créé le 07/11/1990 Dernière mise à jour 07/11/1990                   */
void
decodageBase::Recupere()
{
#ifndef _ENTERPRISE_DLL
	if (getRecupFen())
	{
  	AideDecode* pAideDecode = new AideDecode(0, this) ;
    pAideDecode->Execute() ;
    delete pAideDecode ;
	}
#endif

	_pNoyau->setGood(0) ;
}

void
decodageBase::ajLL()
{
  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return ;

	if (pPPT->end() == getitDcode())
		return ;
	if (NULL == (*getitDcode())->getDataTank())
		return ;

	if (string("") != _pNoyau->getLocLesion())
		_pNoyau->addLocLesion(string(1, cheminSeparationMARK)) ;
	_pNoyau->addLocLesion((*getitDcode())->getLocalisation()) ;

	string sChemin = pPPT->donneCheminItem(getitDcode()) ;
	if (string("") != sChemin)
		_pNoyau->addLocPath(string(1, '[') + sChemin + string(1, ']')) ;
}

bool
decodageBase::CommenceParVoyelle(const string* pLib) const
{
	if (((string*) NULL == pLib) || (strlen(pLib->c_str()) == 0))
		return false ;

	switch ((*pLib)[0])
	{
     case 'È' ://200
     case 'É' :
     case 'Ê' :
     case 'Ë' :
     case 'é' :
     case 'e' :
     case 'E' :
     case 'ë' :
     case 'è' :
     case 'ê' :
     case 'À' :
     case 'Á' :
     case 'a' :
     case 'A' :
     case 'Â' :
     case 'Ã' :
     case 'Ä' :
     case 'Å' : //197
     case 'Æ' :
     case 'à' : //226
     case 'â' :
     case 'ã' :
     case 'ä' :
     case 'å' :
     case 'æ' :
     case 'á' :
     case 'Ì' :
     case 'Í' :
     case 'Î' :
     case 'Ï' :
     case 'í' :
     case 'i' :
     case 'I' :
     case 'î' :
     case 'ï' :
     case 'Ü' ://220
     case 'Û' :
     case 'Ú' :
     case 'Ù' : //216
     case 'ú' :
     case 'û' :
     case 'u' :
     case 'U' :
     case 'ü' :
     case 'ù' :
     case 'Õ' : //213
     case 'Ö' :
     case 'Ô' :
     case 'o' :
     case 'O' :
     case 'Ó' :
     case 'Ò' :
     case 'ò' : //242
     case 'ó' :
     case 'ô' :
     case 'õ' :
     case 'ö' :
     case 'Ý' :
     case 'y' :
     case 'Y' :
     case 'ÿ' :  return true ;
	}
	return false ;
}

int
decodageBase::donneCertitude(const string sCert) const
{
	if ((string("") == sCert) || (strlen(sCert.c_str()) < 5))
		return 100 ;
	if (string(sCert, 0, 3) != string("WCE"))
		return 100 ;

	string sScore = string(sCert, 3, 2) ;
	if (sScore == string("ZZ"))
		return -1 ;
	if ((sScore > string("99")) || (sScore < string("00")))
		return 100 ;
	return (10 * donneDigit(sScore[0]) + donneDigit(sScore[1])) ;
}

string
decodageBase::intenseAdjectif(AdjIntens* pAdj1, AdjIntens* pAdj2)
{
  if (((AdjIntens*) NULL == pAdj1) || ((AdjIntens*) NULL == pAdj2))
    return string("") ;

  string sPhrase = string("") ;
  string sNom1   = string("") ;
  string sNom2   = string("") ;
  //
  // Cas avec au moins un non
  //
  // Cas ni ni
  //
  if ((string("WCE00") == pAdj1->_sIntensite) &&
      (string("WCE00") == pAdj2->_sIntensite))
  {
    sPhrase = "ne paraît ni " + pAdj1->_sAdjectif + ", ni " + pAdj2->_sAdjectif ;
    return sPhrase ;
  }

  //
  //
  //
  if ((string("WCE00") != pAdj1->_sIntensite) && (string("") != pAdj1->_sIntensite))
  {
    if      (pAdj1->_sIntensite == "WCEA0")
      sNom1 = pAdj1->_sAdjectif ;
    else if (pAdj1->_sIntensite == "1MINI")
      sNom1 = "discrètement " + pAdj1->_sAdjectif ;
    else if (pAdj1->_sIntensite == "1MODE")
      sNom1 = "modérément " + pAdj1->_sAdjectif ;
    else if (pAdj1->_sIntensite == "1MOYE")
      sNom1 = "nettement " + pAdj1->_sAdjectif ;
    else if (pAdj1->_sIntensite == "1IMPO")
      sNom1 = "très " + pAdj1->_sAdjectif ;
  }
  if ((string("WCE00") != pAdj2->_sIntensite) && (string("") != pAdj2->_sIntensite))
  {
    if      (pAdj2->_sIntensite == "WCEA0")
      sNom2 = pAdj2->_sAdjectif ;
    else if (pAdj2->_sIntensite == "1MINI")
      sNom2 = "discrètement " + pAdj2->_sAdjectif ;
    else if (pAdj2->_sIntensite == "1MODE")
      sNom2 = "modérément " + pAdj2->_sAdjectif ;
    else if (pAdj2->_sIntensite == "1MOYE")
      sNom2 = "nettement " + pAdj2->_sAdjectif ;
    else if (pAdj2->_sIntensite == "1IMPO")
      sNom2 = "très " + pAdj2->_sAdjectif ;
  }

  if (string("WCE00") == pAdj1->_sIntensite)
  {
    sPhrase = string("ne paraît pas ") + pAdj1->_sAdjectif ;
    if (string("") != sNom2)
      sPhrase += string(", mais paraît ") + sNom2 ;
    return sPhrase ;
  }
  if (string("WCE00") == pAdj2->_sIntensite)
  {
    sPhrase = string("ne paraît pas ") + pAdj2->_sAdjectif ;
    if (string("") != sNom1)
      sPhrase += string(", mais paraît ") + sNom1 ;
    return sPhrase ;
  }

  if (string("") != sNom1)
  {
    sPhrase = string("paraît ") + sNom1 ;
    if (string("") != sNom2)
      sPhrase += string(" et ") + sNom2 ;
    return sPhrase ;
  }
  if (string("") != sNom2)
    sPhrase = string("paraît ") + sNom2 ;
    
  return sPhrase ;
}

bool
decodageBase::isTexteLibre() const
{
	return (getSt() == string("#TLI#")) ;
}

bool
decodageBase::isChampLibre() const
{
	return (getSt() == string("£C;")) ;
}

bool
decodageBase::isDimension() const
{
	return (isComplementAvecUnite('N')) ;
}

bool
decodageBase::isDate() const
{
	return (isComplementAvecUnite('D')) ;
}

bool
decodageBase::isHeure() const
{
	return (isComplementAvecUnite('H')) ;
}

bool
decodageBase::isCode() const
{
	return (isComplementSansUnite('O')) ;
}

void
decodageBase::setDcodeurFromLexique(string sCodeLexique, string sLang)
{
  _pNoyau->setDcodeur(string("")) ;

  if (string("") == sCodeLexique)
    return ;

  string sSelectedLanguage = sLang ;
  if (string("") == sSelectedLanguage)
    sSelectedLanguage = _sLangue ;

  string sLabel = string("") ;
  pContexte->getDico()->donneLibelle(sSelectedLanguage, &sCodeLexique, &sLabel) ;

  _pNoyau->setDcodeur(sLabel) ;
}

void
decodageBase::setDcodeurFromData(NSPathologData* pData)
{
  _pNoyau->setDcodeur(string("")) ;

  if ((NSPathologData*) NULL == pData)
    return ;

  string sLabel = string("") ;
  pData->donneLibelleAffiche(&sLabel) ;

  _pNoyau->setDcodeur(sLabel) ;
}

void
decodageBase::setPseumajForFirstChar()
{
  string sDecodeur = _pNoyau->getDcodeur() ;

  if (string("") == sDecodeur)
    return ;

  sDecodeur[0] = pseumaj(sDecodeur[0]) ;

  _pNoyau->setDcodeur(sDecodeur) ;
}

void
decodageBase::stripDcodeur(STRIPTYPE n, char c)
{
  string sDecodeur = _pNoyau->getDcodeur() ;

  if (string("") == sDecodeur)
    return ;

  strip(sDecodeur, stripRight, ' ') ;

  _pNoyau->setDcodeur(sDecodeur) ;
}

bool
decodageBase::isComplementAvecUnite(const char cCategorie) const
{
  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return false ;

	int ligne = (*getitDcode())->getLigne() ;

  //
  // On synchronise itDcodeTemp avec itDcode
  //
  PatPathoIter itDcodeTemp = pPPT->begin() ;
  while ((*itDcodeTemp)->getLocalisation() != (*getitDcode())->getLocalisation())
  	itDcodeTemp++ ;
	//
  // On remplit codeLexique tant qu'on est sur la même ligne
  //
  bool bPresenceUnite  = false ;
  bool bPresenceValeur = false ;
	string sFormat = string("") ;

	while ((pPPT->end() != itDcodeTemp) && ((*itDcodeTemp)->getLigne() == ligne))
	{
    string sLexique = (*itDcodeTemp)->getLexique() ;
    if ((string("") != sLexique) && ('£' == sLexique[0]))
    {
    	sFormat = sLexique ;
      if ((*itDcodeTemp)->getComplement() != string(""))
      	bPresenceValeur = true ;
      if ((*itDcodeTemp)->getUnit() != string(""))
      	bPresenceUnite = true ;
      break ;
    }
    itDcodeTemp++ ;
	}
	if ((false == bPresenceUnite) || (false == bPresenceValeur))
		return false ;
	if (sFormat[1] != cCategorie)
		return false ;

	return true ;
}

bool
decodageBase::isComplementSansUnite(const char cCategorie) const
{
  NSPatPathoArray* pPPT = _pNoyau->getPatPathoArray() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return false ;

	int ligne = (*getitDcode())->getLigne() ;

	// On synchronise itDcodeTemp avec itDcode
	//
	PatPathoIter itDcodeTemp = pPPT->begin() ;
	while ((*itDcodeTemp)->getLocalisation() != (*getitDcode())->getLocalisation())
		itDcodeTemp++ ;
	//
	// On remplit codeLexique tant qu'on est sur la même ligne
	//
	bool bPresenceValeur = false ;
	string sFormat = string("") ;

	while ((pPPT->end() != itDcodeTemp) && ((*itDcodeTemp)->getLigne() == ligne))
	{
    string sLexique = (*itDcodeTemp)->getLexique() ;
    if (string("") != sLexique)
    {
		  switch (sLexique[0])
      {
    	  case '£' :
      	  bPresenceValeur = true ;
          sFormat = sLexique ;
          break ;
        default  : return false ;
      }
    }
    itDcodeTemp++ ;
	}
	if (false == bPresenceValeur)
  	return false ;
	if (cCategorie != sFormat[1])
		return false ;

	return true ;
}

// Creates a sentence of the kind "La hauteur est de 3 mm." if bReady is true
//                             or "la hauteur est de 3 mm" if bReady is false
//
string
decodageBase::createSentenceForNum(const string sLabel, const gereNum* pNum, bool bReady)
{
  if (string("") == sLabel)
    return string("") ;

  if (((gereNum*) NULL == pNum) || pNum->estVide())
    return string("") ;

  NSPhraseMot Subject(pContexte, sLabel) ;

  return createSentenceForNum(&Subject, pNum, bReady) ;
}

// Creates a sentence of the kind "La hauteur est de 3 mm." if bReady is true
//                             or "la hauteur est de 3 mm" if bReady is false
//
string
decodageBase::createSentenceForNum(const NSPhraseMot* pSubject, const gereNum* pNum, bool bReady)
{
  if ((NSPhraseMot*) NULL == pSubject)
    return string("") ;

  if (((gereNum*) NULL == pNum) || pNum->estVide())
    return string("") ;

  _pPhraseur->initialise() ;

  NSPhraseMot NumValue(pContexte) ;
  NumValue.initFromNum(pNum) ;

  _pPhraseur->addSubject(pSubject) ;
  _pPhraseur->addCCChiffre(&NumValue) ;

  _pGenerateur->genereProposition(dcPhrase) ;

  if (bReady)
    _pGenerateur->termineProposition() ;

  return _pGenerateur->getPropositionPhrase() ;
}

// Creates a sentence of the kind "Sa hauteur est de 3 mm." if bReady is true
//                             or "sa hauteur est de 3 mm" if bReady is false
//
string
decodageBase::createSentenceForNumRelatedTo(const string sRelatedTo, const string sLabel, const gereNum* pNum, bool bReady, bool bDontShowRelator)
{
  if (string("") == sLabel)
    return string("") ;

  if (string("") == sRelatedTo)
    return createSentenceForNum(sLabel, pNum, bReady) ;

  if (((gereNum*) NULL == pNum) || pNum->estVide())
    return string("") ;

  _pPhraseur->initialise() ;

  NSPhraseMot NumValue(pContexte) ;
  NumValue.initFromNum(pNum) ;

  NSPhraseMot Sujet(pContexte) ;
  Sujet.InitFromEtiquette(sLabel) ;

  NSPhraseur* pCplmtNom = new NSPhraseur(pContexte) ;

  NSPhraseMot motRelatedTo(pContexte, sRelatedTo) ;
  motRelatedTo.setTypeAsCplmt(NSPhraseMot::cpltAppartenance) ;
  if (bDontShowRelator)
    motRelatedTo.setCplmtMgnt(NSPhraseMot::complementAppartenanceNoRepeat) ;

  pCplmtNom->compNom.push_back(new NSPhraseMot(motRelatedTo)) ;
  Sujet.addComplementPhr(pCplmtNom) ;

  _pPhraseur->addSubject(&Sujet) ;
  _pPhraseur->addCCChiffre(&NumValue) ;

  _pGenerateur->genereProposition(dcPhrase) ;

  if (bReady)
    _pGenerateur->termineProposition() ;

  return _pGenerateur->getPropositionPhrase() ;
}

// Creates a sentence of the kind "La hauteur est augmentée." if bReady is true
//                             or "la hauteur est augmentée" if bReady is false
//
string
decodageBase::createSentenceForAttribute(const string sLabel, const string sAttribute, bool bReady)
{
  NSPhraseMot subject(pContexte, sLabel) ;
  
  return createSentenceForAttribute(&subject, sAttribute, bReady) ;
}

string
decodageBase::createSentenceForAttribute(const NSPhraseMot* pSubject, const string sAttribute, bool bReady)
{
  if (((NSPhraseMot*) NULL == pSubject) || (string("") == sAttribute))
    return string("") ;

  _pPhraseur->initialise() ;

  _pPhraseur->addSubject(pSubject) ;
  _pPhraseur->addAttSujet(sAttribute) ;

  _pGenerateur->genereProposition(dcPhrase) ;

  if (bReady)
    _pGenerateur->termineProposition() ;

  return _pGenerateur->getPropositionPhrase() ;
}

// Creates a sentence of the kind "Sa hauteur est augmentée." if bReady is true
//                             or "sa hauteur est augmentée" if bReady is false
//
string
decodageBase::createSentenceForAttributeRelatedTo(const NSPhraseMot* pRelatedTo, const NSPhraseMot* pSubject, const string sAttribute, bool bReady, bool bDontShowRelator)
{
  if (((NSPhraseMot*) NULL == pSubject) || (string("") == sAttribute))
    return string("") ;

  if ((NSPhraseMot*) NULL == pRelatedTo)
    return createSentenceForAttribute(pSubject, sAttribute, bReady) ;

  _pPhraseur->initialise() ;

  NSPhraseur* pCplmtNom = new NSPhraseur(pContexte) ;

  NSPhraseMot motRelatedTo(*pRelatedTo) ;
  motRelatedTo.setTypeAsCplmt(NSPhraseMot::cpltAppartenance) ;
  if (bDontShowRelator)
    motRelatedTo.setCplmtMgnt(NSPhraseMot::complementAppartenanceNoRepeat) ;

  pCplmtNom->compNom.push_back(new NSPhraseMot(motRelatedTo)) ;
  pSubject->addComplementPhr(pCplmtNom) ;

  _pPhraseur->addSubject(pSubject) ;
  _pPhraseur->addAttSujet(sAttribute) ;

  _pGenerateur->genereProposition(dcPhrase) ;

  if (bReady)
    _pGenerateur->termineProposition() ;

  return _pGenerateur->getPropositionPhrase() ;
}


// Creates a sentence of the kind "Sa hauteur est augmentée." if bReady is true
//                             or "sa hauteur est augmentée" if bReady is false
//
string
decodageBase::createSentenceForAttributeRelatedTo(const string sRelatedTo, const string sLabel, const string sAttribute, bool bReady, bool bDontShowRelator)
{
  if ((string("") == sLabel) || (string("") == sAttribute))
    return string("") ;

  if (string("") == sRelatedTo)
    return createSentenceForAttribute(sLabel, sAttribute, bReady) ;

  NSPhraseMot motRelatedTo(pContexte, sRelatedTo) ;
  NSPhraseMot subject(pContexte, sLabel) ;

  return createSentenceForAttributeRelatedTo(&motRelatedTo, &subject, sAttribute, bReady, bDontShowRelator) ;
}

//  -------------------------------------------------------------------
//                      Méthodes de numStorage
//  -------------------------------------------------------------------

numStorage::numStorage(NSSuper* pSuper, string sLangue)
{
  _pNormale = new gereNum(pSuper, sLangue) ;
  _pNormInf = new gereNum(pSuper, sLangue) ;
  _pNormSup = new gereNum(pSuper, sLangue) ;

	initToBlank() ;

  lObjectCount++ ;
}

void
numStorage::initToBlank()
{
  _pNormale->initialise() ;
  _pNormInf->initialise() ;
  _pNormSup->initialise() ;

  _sNum      = string("") ;
  _sNumInf   = string("") ;
  _sNumSup   = string("") ;

  _dValue    = double(0) ;
  _dInfValue = double(0) ;
  _dSupValue = double(0) ;

  _bExact    = false ;
  _bInf      = false ;
  _bSup      = false ;
  _bInfEgal  = false ;
  _bSupEgal  = false ;

	_sFormatage = string("") ;
  _sUnite     = string("") ;
}

numStorage::numStorage(const numStorage& src)
{
	init(&src) ;

  lObjectCount++ ;
}

numStorage::~numStorage()
{
  delete _pNormale ;
  delete _pNormInf ;
  delete _pNormSup ;

  lObjectCount-- ;
}

void
numStorage::init(const numStorage* pSrc)
{
  if ((numStorage*) NULL == pSrc)
    return ;

  _pNormale  = new gereNum(*(pSrc->_pNormale)) ;
  _pNormInf  = new gereNum(*(pSrc->_pNormInf)) ;
  _pNormSup  = new gereNum(*(pSrc->_pNormSup)) ;

  _sNum       = pSrc->_sNum ;
  _sNumInf    = pSrc->_sNumInf ;
  _sNumSup    = pSrc->_sNumSup ;

  _dValue     = pSrc->_dValue ;
  _dInfValue  = pSrc->_dInfValue ;
  _dSupValue  = pSrc->_dSupValue ;

  _bExact     = pSrc->_bExact ;
  _bInf       = pSrc->_bInf ;
  _bSup       = pSrc->_bSup ;
  _bInfEgal   = pSrc->_bInfEgal ;
  _bSupEgal   = pSrc->_bSupEgal ;

	_sFormatage = pSrc->_sFormatage ;
  _sUnite     = pSrc->_sUnite ;
}

void
numStorage::strToDble(const string sNum, string &sBonNum, double &dValue)
{
  sBonNum = string("") ;
  dValue  = (double) 0 ;

  if (string("") == sNum)
    return ;

  string sEntier = string("") ;
  string sDecima = string("") ;

  sBonNum = sNum ;

  size_t i = sNum.find(".") ;
  if (NPOS == i)
  	sEntier = sNum ;
  else
  {
  	if (i > 0)
    	sEntier = string(sNum, 0, i) ;
    sDecima = string(sNum, i + 1, strlen(sNum.c_str()) - i - 1) ;

    sBonNum[i] = ',' ;
  }
  //
  // Calcul de la valeur
  //

  // Partie entière
  //
  for (size_t i = 0 ; i < strlen(sEntier.c_str()) ; i++)
  {
  	if (!isdigit(sEntier[i]))
    	return ;
    dValue = (dValue * double(10)) + double(donneDigit(sEntier[i])) ;
  }

  // Partie décimale
  //
  double multi = double(1) ;
  for (size_t i = 0 ; i < strlen(sDecima.c_str()) ; i++)
  {
  	if (!isdigit(sDecima[i]))
    	return ;
    multi = multi * double(0.1) ;
    dValue += double(donneDigit(sDecima[i])) * multi ;
	}
}

void
numStorage::initValue(const string sRawData)
{
  strToDble(sRawData, _sNum, _dValue) ;
  _bExact = (string("") != _sNum) ;
}

void
numStorage::initInfValue(const string sRawData)
{
  strToDble(sRawData, _sNumInf, _dInfValue) ;
  _bInf = (string("") != _sNumInf) ;
}

void
numStorage::initSupValue(const string sRawData)
{
  strToDble(sRawData, _sNumSup, _dSupValue) ;
  _bSup = (string("") != _sNumSup) ;
}

void
numStorage::setNormale(const gereNum* pNorm)
{
	*_pNormale = *pNorm ;
}

void
numStorage::setNormInf(const gereNum* pNorm)
{
	*_pNormInf = *pNorm ;
}

void
numStorage::setNormSup(const gereNum* pNorm)
{
  *_pNormSup = *pNorm ;
}

numStorage&
numStorage::operator=(const numStorage& src)
{
	if (this == &src)
		return *this ;

  init(&src) ;

  return *this ;
}

//  -------------------------------------------------------------------
//                      Méthodes de gereNum
//  -------------------------------------------------------------------

gereNum::gereNum(NSSuper* pSup, string sLangue)
        :NSSuperRoot(pSup)
{
	_sLang    = sLangue ;
  _pDateRef = (gereDate*) 0 ;
  initialise() ;

  lObjectCount++ ;
}

gereNum::~gereNum()
{
	if (_pDateRef)
		delete _pDateRef ;

  lObjectCount-- ;
}

gereNum::gereNum(const gereNum& src)
        :NSSuperRoot(src._pSuper)
{
	_aValues = src._aValues ;
  _sLang   = src._sLang ;

  if ((gereDate*) NULL == src._pDateRef)
  	_pDateRef = (gereDate*) 0 ;
  else
  	_pDateRef = new gereDate(*(src._pDateRef)) ;

  lObjectCount++ ;
}

gereNum&
gereNum::operator=(const gereNum& src)
{
	if (this == &src)
  	return *this ;

	_aValues = src._aValues ;
  _sLang   = src._sLang ;

  if (_pDateRef)
  	delete _pDateRef ;

  if ((gereDate*) NULL == src._pDateRef)
  	_pDateRef = (gereDate*) 0 ;
  else
  	_pDateRef = new gereDate(*(src._pDateRef)) ;

	return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦       Donne le libellé de l'unité (singulier ou pluriel)        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
string
gereNum::donneLibelleUnite(NSContexte *pContexte, int iIndice, const bool bWithSpacer)
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (((numStorage*) NULL == pStorage) || (string("") == pStorage->getUnit()))
  	return string("") ;

  NSPathologData Data ;
  bool trouve = pContexte->getDico()->trouvePathologData(_sLang, &(pStorage->getUnit()), &Data) ;

  if (false == trouve)
  	return string("") ;

  GENRE iGenre ;
	Data.donneGenre(&iGenre) ;

	bool bPluriel = false ;

	if 	  (pStorage->isExact() && (pStorage->getValue() > 1))
		bPluriel = true ;
	else if ((pStorage->hasInf() || pStorage->hasSup()) &&
           ((pStorage->getInfValue() > 1) || (pStorage->getUppValue() > 1)))
  	bPluriel = true ;

	if (bPluriel)
  	Data.donneGenrePluriel(&iGenre) ;

	string sUniteLibel = string("") ;
	if (Data.estNom())
	{
  	if ((genreMP == iGenre) || (genreFP == iGenre) || (genreNP == iGenre))
    	Data.donneLibelleAffiche(&sUniteLibel, 1 /*declinaisonPluriel*/) ;
    else
    	Data.donneLibelleAffiche(&sUniteLibel, 0 /*declinaisonSingulier*/) ;
	}
	else
		Data.donneLibelleAffiche(&sUniteLibel, 0 /*declinaisonSingulier*/) ;

  if (bWithSpacer && (string("") != sUniteLibel))
    sUniteLibel = string(" ") + sUniteLibel ;

	return sUniteLibel ;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereNum::initialise()
{
	if (_pDateRef)
  	delete _pDateRef ;
	_pDateRef = (gereDate*) 0 ;

  _aValues.vider() ;
}

//  +-----------------------------------------------------------------+
//  ¦                     Initialise l'objet                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereNum::instancier(const string* pNum, const string* pUnit, const string* pFrmt, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if ((numStorage*) NULL == pStorage)
  	return ;

  pStorage->initToBlank() ;

	pStorage->setUnit(*pUnit) ;
	pStorage->setFormat(*pFrmt) ;

  if (string("") == *pNum)
		return ;

	string sValeur = *pNum ;

	size_t pos = sValeur.find("<<") ;
  if (NPOS == pos)
  {
  	pos = sValeur.find("[<") ;
    if (NPOS == pos)
    {
    	pos = sValeur.find("<[") ;
      if (NPOS == pos)
      {
      	pos = sValeur.find("[[") ;
        if (NPOS != pos)
        {
        	pStorage->setInfEgal(true) ;
          pStorage->setSupEgal(true) ;
        }
      }
      else
      	pStorage->setSupEgal(true) ;
    }
    else
    	pStorage->setInfEgal(true) ;
  }

  // cas des intervalles avec borne inf et borne sup
  //
  if (NPOS != pos)
	{
  	string sSSValeur ;

    if (pos > 0)
      pStorage->initInfValue(string(sValeur, 0, pos)) ;

    if (pos < strlen(sValeur.c_str()) - 2)
      pStorage->initSupValue(string(sValeur, pos + 2, strlen(sValeur.c_str())-pos-2)) ;
  }
  //
  // Valeur du type "<1"
  //
  else if (sValeur[0] == '<')
    pStorage->initSupValue(string(sValeur, 1, strlen(sValeur.c_str()) - 1)) ;
  //
  // Valeur du type "<=1"
  //
  else if (sValeur[0] == '[')
  {
    pStorage->initSupValue(string(sValeur, 1, strlen(sValeur.c_str()) - 1)) ;
    pStorage->setSupEgal(true) ;
  }
  //
  // Valeur du type ">1"
  //
  else if (sValeur[0] == '>')
    pStorage->initInfValue(string(sValeur, 1, strlen(sValeur.c_str()) - 1)) ;
  //
  // Valeur du type ">=1"
  //
  else if (sValeur[0] == ']')
  {
    pStorage->initInfValue(string(sValeur, 1, strlen(sValeur.c_str()) - 1)) ;
    pStorage->setInfEgal(true) ;
  }
  //
  // Valeur exacte (sans intervalle)
  //
  else
    pStorage->initValue(sValeur) ;
}

void
gereNum::setNum(const string* nouvNum, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
		pStorage->setNum(*nouvNum) ;
}

void
gereNum::setUnite(const string* nouvUnit, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
		pStorage->setUnit(*nouvUnit) ;
}

void
gereNum::setFormt(const string* nouvFrmt, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
		pStorage->setFormat(*nouvFrmt) ;
}

void
gereNum::setVal(const double nouVal, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
		pStorage->setValue(nouVal) ;
}

bool
gereNum::setNormale(const gereNum* pNorm, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
  {
		pStorage->setNormale(pNorm) ;
    return true ;
  }

  return false ;
}

bool
gereNum::setNormInf(const gereNum* pNorm, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
  {
		pStorage->setNormInf(pNorm) ;
    return true ;
  }

  return false ;
}

bool
gereNum::setNormSup(const gereNum* pNorm, const int iIndice)
{
	numStorage* pStorage = getStorageFromIndiceOrCreate(iIndice) ;
  if (pStorage)
  {
		pStorage->setNormSup(pNorm) ;
    return true ;
  }

  return false ;
}

string
gereNum::getNum(int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNum() ;

  return string("") ;
}

string
gereNum::getNumInf(int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNumInf() ;

  return string("") ;
}

string
gereNum::getNumSup(int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNumSup() ;
  else
  	return string("") ;
}

gereNum*
gereNum::getNorm(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNormale() ;

  return (gereNum*) 0 ;
}

gereNum*
gereNum::getNormInf(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNormInf() ;

  return (gereNum*) 0 ;
}

gereNum*
gereNum::getNormSup(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getNormSup() ;

  return (gereNum*) 0 ;
}

double
gereNum::getValeur(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getValue() ;

  return double(0) ;
}

double
gereNum::getValeurInf(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getInfValue() ;

  return double(0) ;
}

double
gereNum::getValeurSup(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if (pStorage)
		return pStorage->getUppValue() ;

  return double(0) ;
}

bool
gereNum::estExact(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return false ;

	return pStorage->isExact() ;
}

bool
gereNum::estInf(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return false ;

	return pStorage->hasInf() ;
}

bool
gereNum::estSup(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return false ;

	return pStorage->hasSup() ;
}

bool
gereNum::estInfEgal(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return false ;

	return (pStorage->hasInf() && pStorage->isInfEgal())  ;
}

bool
gereNum::estSupEgal(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return false ;

	return (pStorage->hasSup() && pStorage->isSupEgal())  ;
}

string
gereNum::getUnite(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return string("") ;

	return pStorage->getUnit() ;
}

string
gereNum::getFormat(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return string("") ;

	return pStorage->getFormat() ;
}

bool
gereNum::estVide(const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
		return true ;

	return !(pStorage->isExact() || pStorage->hasInf() || pStorage->hasSup())  ;
}

numStorage*
gereNum::getStorageFromIndice(const int iIndice) const
{
	if (_aValues.empty())
    return (numStorage*) 0 ;

  int i = 0 ;

  NumStorageConstIter storeIter = _aValues.begin() ;
  for (; (_aValues.end() != storeIter) && (i < iIndice) ; i++, storeIter++) ;
  if (_aValues.end() != storeIter)
  	return *storeIter ;

  return (numStorage*) 0 ;
}

numStorage*
gereNum::getStorageFromIndiceOrCreate(const int iIndice)
{
  numStorage* pStored = getStorageFromIndice(iIndice) ;
  if (pStored)
    return pStored ;

  _aValues.push_back(new numStorage(_pSuper, _sLang)) ;
  return _aValues.back() ;
}

void
gereNum::setDate(gereDate* pDate)
{
	if ((gereDate*) NULL != _pDateRef)
  	delete _pDateRef ;

	_pDateRef = pDate ;
}

//  +-----------------------------------------------------------------+
//  ¦                   Get object as a string                        ¦
//  ¦                                                                 ¦
//  ¦  eExtensions :                                                  ¦
//  ¦        RIEN     : 8                                             ¦
//  ¦        NORMALES : 8 (4<N<12)                                    ¦
//  ¦        STYLE    : 8 mm (4 mm < normale < 12 mm)                 ¦
//  ¦        SENTENCE : 8 mm (normale entre 4 et 12 mm)               ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
string
gereNum::getNum(NSContexte *pContexte, const string sFormatage, const EXTENSIONS eExtensions, const int iIndice) const
{
	numStorage* pStorage = getStorageFromIndice(iIndice) ;
  if ((numStorage*) NULL == pStorage)
  	return string("") ;

	if (RIEN == eExtensions)
  	return pStorage->getNum() ;

	if ((NORMALES == eExtensions) || (STYLE == eExtensions) || (SENTENCE == eExtensions))
	{
    // Value
    //
  	string sRetour = pStorage->getNum() ;
    string sUnit   = donneLibelleUnite(pContexte, iIndice, true) ;

    if ((STYLE == eExtensions) || (SENTENCE == eExtensions))
      sRetour += sUnit ;

    // Normality range
    //
    if ((pStorage->getNormInf() != 0) || (pStorage->getNormSup() != 0))
    {
    	string sNI     = string("") ;
      string sNIUnit = string("") ;
      if (pStorage->getNormInf() != 0)
      {
      	sNI     = pStorage->getNormInf()->getNum(pContexte, sFormatage) ;
        sNIUnit = pStorage->getNormInf()->donneLibelleUnite(pContexte, 0, true) ;

        // If min value and value don't share the same unit, we must specify it
        //
        if ((STYLE != eExtensions) && (sNIUnit != sUnit))
          sNI += sNIUnit ;
      }

      string sNS     = string("") ;
      string sNSUnit = string("") ;
      if (pStorage->getNormSup() != 0)
      {
      	sNS     = pStorage->getNormSup()->getNum(pContexte, sFormatage) ;
        sNSUnit = pStorage->getNormSup()->donneLibelleUnite(pContexte, 0, true) ;

        // If max value and value don't share the same unit, we must specify it
        //
        if ((STYLE != eExtensions) && (sNSUnit != sUnit))
          sNS += sNSUnit ;
      }

      // There is both a min and a max values
      //
      if ((string("") != sNI) && (string("") != sNS))
      {
        if (NORMALES == eExtensions)
        {
      	  if (sNI == sNS)
        	  sRetour += string(" (N=") + sNI + string(")") ;
          else
        	  sRetour += string(" (") + sNI + string("<N<") + sNS + string(")") ;
        }
        else if (STYLE == eExtensions)
        {
      	  if ((sNI == sNS) && (sNIUnit == sNSUnit))
        	  sRetour += string(" (normale ") + sNI + sNIUnit + string(")") ;
          else
        	  sRetour += string(" (") + sNI + sNIUnit + string(" < normale < ") + sNS + sNSUnit + string(")") ;
        }
        else if (SENTENCE == eExtensions)
        {
      	  if ((sNI == sNS) && (sNIUnit == sNSUnit))
        	  sRetour += string(" (normale ") + sNI + string(")") ;
          else
        	  sRetour += string(" (normale entre ") + sNI + string(" et ") + sNS + string(")") ;
        }
      }
      else if (string("") != sNI)
      {
        if (NORMALES == eExtensions)
      	  sRetour += string(" (N>") + sNI + string(")") ;
        else if (STYLE == eExtensions)
          sRetour += string(" (normale > ") + sNI + sNIUnit + string(")") ;
        else if (SENTENCE == eExtensions)
          sRetour += string(" (normale supérieure à ") + sNI + string(")") ;
      }
      else if (string("") != sNS)
      {
        if (NORMALES == eExtensions)
      	  sRetour += string(" (N<") + sNS + string(")") ;
        else if (STYLE == eExtensions)
          sRetour += string(" (normale < ") + sNS + sNSUnit + string(")") ;
        else if (SENTENCE == eExtensions)
          sRetour += string(" (normale inférieure à ") + sNS + string(")") ;
      }
    }
    return sRetour ;
	}
	return string("") ;
}

//  -------------------------------------------------------------------
//                      Méthodes de gereDate
//  -------------------------------------------------------------------

gereDate::gereDate(NSSuper* pSuper, string sLangue)
         :NSSuperRoot(pSuper)
{
  initialise() ;

  _sLang = sLangue ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
gereDate::gereDate(const gereDate& src)
         :NSSuperRoot(src._pSuper)
{
  _sDate      = src._sDate ;
  _sFormatage = src._sFormatage ;
  _sUnite     = src._sUnite ;
  _sLibelle   = src._sLibelle ;
  _sLang      = src._sLang ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
gereDate&
gereDate::operator=(const gereDate& src)
{
  if (this == &src)
		return *this ;

  _sDate      = src._sDate ;
  _sFormatage = src._sFormatage ;
  _sUnite     = src._sUnite ;
  _sLibelle   = src._sLibelle ;
  _sLang      = src._sLang ;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereDate::initialise()
{
  _sDate      = string("") ;
	_sFormatage = string("") ;
  _sUnite     = string("") ;
	_sLibelle   = string("") ;
  _sLang      = string("") ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDate::donne_date_breve(string* pMessage, string* pIntro)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  if (string("") == _sDate)
    return ;

  // Date au format AAAAMMJJ
	//
  if (string("2DA01") == _sUnite)
  {
    // Date vide
    //
    if (string("00000000") == _sDate)
      return ;

    if (string("fr") == _sLang)
    {
      donne_date_breve_for_french(pMessage, pIntro) ;
      return ;
    }

    if (string("en") == _sLang)
    {
      donne_date_breve_for_english(pMessage, pIntro) ;
      return ;
    }

    // langue non traitée : on renvoie JJ/MM/AAAA
    //
    *pMessage = string(_sDate, 6, 2) + string("/") +
                string(_sDate, 4, 2) + string("/") + string(_sDate, 0, 4) ;

    return ;
  }

  //
  // Date au format AAAAMMJJhhmmss
	//
  else if (string("2DA02") == _sUnite)
  {
    // Date vide
    if (string("00000000000000") == _sDate)
      return ;

    if (string("fr") == _sLang)
    {
      donne_date_breve_for_french(pMessage, pIntro) ;

      size_t iDateLen = strlen(_sDate.c_str()) ;

      if ((iDateLen <= 8) || (string("00") == string(_sDate, 6, 2)))
        return ;

      string sHeure = string("") ;

      // gestion de l'heure
      //
      if      (10 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string("h") ;
      else if (12 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
      else if (14 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

      if (string("") != sHeure)
        *pMessage += string(" à ") + sHeure ;

      return ;
    }

    if (string("en") == _sLang)
    {
      donne_date_breve_for_english(pMessage, pIntro) ;

      size_t iDateLen = strlen(_sDate.c_str()) ;

      if ((iDateLen <= 8) || (string("00") == string(_sDate, 6, 2)))
        return ;

      string sHeure = string("") ;

      // gestion de l'heure
      //
      if      (10 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string("h") ;
      else if (12 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
      else if (14 == iDateLen)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

      if (string("") != sHeure)
        *pMessage += string(" at ") + sHeure ;

      return;
    }

    // langue non traitée : on renvoie JJ/MM/AAAA hh:mm:ss
    //
    *pMessage = string(_sDate, 6, 2) + "/" + string(_sDate, 4, 2) + "/" + string(_sDate, 0, 4) ;

    string sHeure = string("") ;

    // gestion de l'heure
    if      (strlen(_sDate.c_str()) == 10)
        sHeure = string(_sDate, 8, 2) + string(":??:??") ;
    else if (strlen(_sDate.c_str()) == 12)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":??") ;
    else if (strlen(_sDate.c_str()) == 14)
        sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;

    return ;
  }
}

void
gereDate::donne_date_breve_for_french(string* pMessage, string* pIntro)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  if (strlen(_sDate.c_str()) < 8)
    return ;

  // Date vide
  //
  if (string("00000000") == string(_sDate, 0, 8))
    return ;

  // Juste AAAA
  //
  if (string("0000") == string(_sDate, 4, 4))
  {
    *pMessage = string(_sDate, 0, 4) ;
    *pIntro   = string("en ") ;
    return ;
  }

  // MM/AAAA
  //
  if (string("00") == string(_sDate, 6, 2))
  {
    *pMessage = donne_mois(string(_sDate, 4, 2)) + string(" ") + string(_sDate, 0, 4) ;
    *pIntro   = string("en ") ;
    return ;
  }

  // JJ/MM/AAAA
  //
  *pIntro   = string("le ") ;
  *pMessage = string(_sDate, 6, 2) + string("/") + string(_sDate, 4, 2) + string("/") + string(_sDate, 0, 4) ;
}

void
gereDate::donne_date_breve_for_english(string* pMessage, string* pIntro)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  if (strlen(_sDate.c_str()) < 8)
    return ;

  // Date vide
  //
  if (string("00000000") == string(_sDate, 0, 8))
    return ;

  // Juste AAAA
  //
  if (string("0000") == string(_sDate, 4, 4))
  {
    *pMessage = string(_sDate, 0, 4) ;
    *pIntro   = string("in ") ;
    return ;
  }

  // MM/AAAA
  //
  if (string("00") == string(_sDate, 6, 2))
  {
    *pMessage = donne_mois(string(_sDate, 4, 2)) + string(" ") + string(_sDate, 0, 4) ;
    *pIntro   = string("on ") ;
    return ;
  }

  // JJ/MM/AAAA
  //
  *pIntro   = string("on the ") ;
  *pMessage = string(_sDate, 4, 2) + string("/") + string(_sDate, 6, 2) + string("/") + string(_sDate, 0, 4) ;
}

//  +-----------------------------------------------------------------+
//  ¦           Donne la date au format "1er janvier 1994"            ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereDate::donne_date_claire(string* pMessage, string* pIntro)
{
  if (((string*) NULL == pMessage) || ((string*) NULL == pIntro))
    return ;

  *pMessage = string("") ;
  *pIntro   = string("") ;

  if (string("") == _sDate)
    return ;

  // Date au format AAAAMMJJ
  //
  if (string("2DA01") == _sUnite)
  {
    size_t iDateLen = strlen(_sDate.c_str()) ;

    if (iDateLen < 8)
      return ;

    if (string("fr") == _sLang)
    {
      *pIntro = string("le ") ;
      string sHeure = string("") ;

      // Jour
      //
      if (isdigit(_sDate[6]) && isdigit(_sDate[7]))
      {
        if ('0' == _sDate[6])
        {
          if ('1' == _sDate[7])
            *pMessage = string("1er ") ;
          else if ('0' != _sDate[7])
            *pMessage = string(1, _sDate[7]) + string(" ") ;
          else
            *pIntro = string("en ") ;
        }
        else
          *pMessage = string(1, _sDate[6]) + string(1, _sDate[7]) + string(" ") ;

        // gestion de l'heure
        if      (10 == iDateLen)
          sHeure = string(_sDate, 8, 2) + string(" heures") ;
        else if (12 == iDateLen)
          sHeure = string(_sDate, 8, 2) + string(" heures ") + string(_sDate, 10, 2) ;
        else if (14 == iDateLen)
          sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;
      }
      else
        *pMessage = string("?? ") ;

      //
      // Mois
      //
      string sMois = donne_mois(string(_sDate, 4, 2)) ;

      if (string("") != sMois)
        *pMessage += sMois + string(" ") ;
      else
        *pMessage += string("?? ") ;

      // Année
      //
      for (int i = 0 ; i < 4 ; i++)
      {
        if (isdigit(_sDate[i]))
          *pMessage += string(1, _sDate[i]) ;
        else
          *pMessage += string(1, '?') ;
      }

      if (string("") != sHeure)
        *pMessage += string(" à ") + sHeure ;

      return ;
    }

    if (string("en") == _sLang)
    {
      *pIntro = string("on ") ;
      string sHeure = string("") ;

      // Mois
      //
      string sMois = donne_mois(string(_sDate, 4, 2)) ;

      if (string("") != sMois)
        *pMessage += sMois + string(" ") ;
      else
        *pMessage = string("?? ") ;

      // Jour
      //
      if (isdigit(_sDate[6]) && isdigit(_sDate[7]))
      {
        *pMessage = string(" the ") ;

        if ('0' == _sDate[6])
        {
          if      ('1' == _sDate[7])
            *pMessage += string("1st ") ;
          else if ('2' == _sDate[7])
            *pMessage += string("2nd ") ;
          else if ('3' == _sDate[7])
            *pMessage += string("3rd ") ;
          else if ('0' != _sDate[7])
            *pMessage += string(1, _sDate[7]) + string("th ") ;
          else
            *pIntro = string("in ") ;
        }
        else
          *pMessage += string(1, _sDate[6]) + string(1, _sDate[7]) + string("th ") ;

        // gestion de l'heure
        if      (10 == iDateLen)
          sHeure = string(_sDate, 8, 2) ;
        else if (12 == iDateLen)
          sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) ;
        else if (14 == iDateLen)
          sHeure = string(_sDate, 8, 2) + string(":") + string(_sDate, 10, 2) + string(":") + string(_sDate, 12, 2) ;
      }
      else
        *pMessage += string("?? ") ;

      // Année
      //
      for (int i = 0 ; i < 4 ; i++)
      {
        if (isdigit(_sDate[i]))
          *pMessage += string(1, _sDate[i]) ;
        else
          *pMessage += string(1, '?') ;
      }

      if (string("") != sHeure)
        *pMessage += string(" at ") + sHeure ;

      return ;
    }
  }
}

//  +-----------------------------------------------------------------+
//  ¦          Donne le libellé du mois à partir de "MM"              ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
string
gereDate::donne_mois(string mois)
{
    if (strlen(mois.c_str()) < 2)
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
string
gereDate::donne_mois(int iMois)
{
  if (string("fr") == _sLang)
  {
    switch (iMois)
    {
      case  1 : return string("janvier") ;
      case  2 : return string("février") ;
      case  3 : return string("mars") ;
      case  4 : return string("avril") ;
      case  5 : return string("mai") ;
      case  6 : return string("juin") ;
      case  7 : return string("juillet") ;
      case  8 : return string("août") ;
      case  9 : return string("septembre") ;
      case 10 : return string("octobre") ;
      case 11 : return string("novembre") ;
      case 12 : return string("décembre") ;
    }
  }
  else if (string("en") == _sLang)
  {
    switch (iMois)
    {
      case  1 : return string("january") ;
      case  2 : return string("february") ;
      case  3 : return string("march") ;
      case  4 : return string("april") ;
      case  5 : return string("mai") ;
      case  6 : return string("june") ;
      case  7 : return string("jully") ;
      case  8 : return string("august") ;
      case  9 : return string("september") ;
      case 10 : return string("october") ;
      case 11 : return string("november") ;
      case 12 : return string("december") ;
    }
  }

  return string("") ;
}

//  -------------------------------------------------------------------
//                      Méthodes de gereHeure
//  -------------------------------------------------------------------

gereHeure::gereHeure(NSSuper* pSuper, string sLangue)
          :NSSuperRoot(pSuper)
{
  _sLang = sLangue ;
  initialise() ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 23/11/1998 Dernière mise à jour 23/11/1998
gereHeure::gereHeure(const gereHeure& src)
          :NSSuperRoot(src._pSuper)
{
  _sHeure     = src._sHeure ;
	_sFormatage = src._sFormatage ;
  _sLibelle   = src._sLibelle ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 23/11/1998 Dernière mise à jour 01/07/1997
gereHeure&
gereHeure::operator=(const gereHeure& src)
{
  if (this == &src)
		return *this ;

  _sHeure     = src._sHeure ;
  _sFormatage = src._sFormatage ;
  _sLibelle   = src._sLibelle ;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 01/07/1997 Dernière mise à jour 01/07/1997
void
gereHeure::initialise()
{
  _sHeure     = string("") ;
  _sFormatage = string("") ;
  _sLibelle   = string("") ;
}

//  +-----------------------------------------------------------------+
//  ¦               Donne la date au format JJ/MM/AAAA                ¦
//  +-----------------------------------------------------------------+
//  Créé le 14/11/1997 Dernière mise à jour 14/11/1997
void
gereHeure::donne_heure(string* pMessage)
{
  if ((string*) NULL == pMessage)
    return ;

  *pMessage = string("") ;

  //
  // Heure au format HHmm
	//
  if (string("2HE01") == _sUnite)
  {
    // Date vide
    if (string("0000") == _sHeure)
      return ;

    if ('0' == _sHeure[0])
      *pMessage = string(_sHeure, 1, 1) ;
    else
      *pMessage = string(_sHeure, 0, 2) ;

    *pMessage += string("h") + string(_sHeure, 2, 2) ;
  }
}

//  -------------------------------------------------------------------
//                      Méthodes de gereCode
//  -------------------------------------------------------------------

gereCode::gereCode(NSContexte* pCtx, string sLangue)
         :NSRoot(pCtx)
{
  sLang = sLangue ;
  initialise() ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                     constructeur copie                          ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
gereCode::gereCode(const gereCode& src)
         :NSRoot(src.pContexte)
{
  sCode 	        = src.sCode ;
  sClassification = src.sClassification ;

  lObjectCount++ ;
}

//  +-----------------------------------------------------------------+
//  ¦                   opérateur d'affectation                       ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
gereCode&
gereCode::operator=(const gereCode& src)
{
  if (this == &src)
		return *this ;

  sCode 	        = src.sCode;
  sClassification = src.sClassification;

  return *this ;
}

//  +-----------------------------------------------------------------+
//  ¦                   (re)initialise l'objet                        ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
void
gereCode::initialise()
{
	sCode 	  = "";
	sClassification = "";
}

//  +-----------------------------------------------------------------+
//  ¦                 Donne le code avec son libellé                  ¦
//  +-----------------------------------------------------------------+
//  Créé le 24/11/1998 Dernière mise à jour 24/11/1998
void
gereCode::donne_code(string* pMessage, bool bAvecCode)
{
#ifndef _ENTERPRISE_DLL
  string sLibelle = string("") ;

  NSEpiClassif DBcurseur(pContexte->getSuperviseur()) ;

  // open table
  DBcurseur.lastError = DBcurseur.open() ;
  if (DBcurseur.lastError != DBIERR_NONE)
  {
    erreur("classif.db -- Erreur à l'ouverture du fichier.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  // définition de la clé de recherche
  string cle = sClassification + sCode ;

  // recherche dans la base
  DBcurseur.lastError = DBcurseur.chercheClef(&cle,
                                              "",
                                              NODEFAULTINDEX,
                                              keySEARCHEQ,
                                              dbiREADLOCK) ;
  if ((DBcurseur.lastError != DBIERR_NONE) &&
      (DBcurseur.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("classif.db -- erreur à la recherche dans la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle()) ;
    DBcurseur.close() ;
    return ;
  }

  if (DBIERR_RECNOTFOUND == DBcurseur.lastError)
  {
    if (bAvecCode)
      sLibelle = sCode ;
    else
      sLibelle = string("???") ;
  }
  else
  {
    // récupération de l'enregistrement
    DBcurseur.lastError = DBcurseur.getRecord() ;
    if (DBcurseur.lastError != DBIERR_NONE)
    {
      erreur("classif.db -- erreur de lecture dans la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());
      DBcurseur.close() ;
      return ;
    }

    if (bAvecCode)
      sLibelle = sCode + string(" (") + DBcurseur.getLabel() + string(")") ;
    else
      sLibelle = DBcurseur.getLabel() ;
  }

  // on ferme la base
  DBcurseur.lastError = DBcurseur.close() ;
  if (DBcurseur.lastError != DBIERR_NONE)
    erreur("classif.db -- erreur à la fermeture de la base.", standardError, DBcurseur.lastError, pContexte->GetMainWindow()->GetHandle());

  *pMessage = sLibelle ;
#endif
}

#ifndef _ENTERPRISE_DLL
//------------------------------------------------------------------------//
//										TreeView												  //
//------------------------------------------------------------------------//

DEFINE_RESPONSE_TABLE1(AideDecode, TDialog)
  EV_COMMAND(IDC_ANNUL,   CmAnnule),
  EV_COMMAND(IDC_CORRIGER, CmCorrige),
  EV_COMMAND(IDC_AUTOMATIQUE, CmAutomatique),
END_RESPONSE_TABLE;

AideDecode::AideDecode(TWindow* pere,  decodageBase* pDecode)
           :TDialog(pere, IDD_DECODE, pNSDLLModule), NSRoot(pDecode->pContexte)
{
	_pImages = (OWL::TImageList*) 0 ;
  _Aparent = pere ;
  // set client window style
  //
  uint32 style = GetStyle() ;
  SetStyle(style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN) ;

  // set treeview window style
  //
  style = TVS_HASLINES | TVS_HASBUTTONS ;
#if defined(BI_PLAT_WIN32)
	style |= TVS_LINESATROOT ;
#endif

	_pDecodageBase = pDecode ;
	_pTreeWind = new TTreeWindow(this, IDC_TREEV_DECODE, pNSDLLModule) ;
}


AideDecode::~AideDecode()
{
  if (_pImages)
    delete _pImages ;

  if (_pTreeWind)
   	delete _pTreeWind ;
}

void
AideDecode::CmCorrige()
{
	_pDecodageBase->setCorrAnnu(iCorrige) ;
	CloseWindow(IDOK) ;
}

void
AideDecode::CmAnnule()
{
  _pDecodageBase->setCorrAnnu(iAnnule) ;
	CloseWindow(IDOK) ;
}

void
AideDecode::CmAutomatique()
{
  _pDecodageBase->setCorrAnnu(iAutomatique) ;

  NSPatPathoArray* pPatPathoArray = _pDecodageBase->getPPtArray() ;
  if (pPatPathoArray->empty())
  {
    CloseWindow(IDOK) ;
    return ;
  }
  PatPathoIter iterTReeView = pPatPathoArray->begin() ;
  //
  // On passe en revue la PatPathoArray jusqu'à l'élément fautif
  //
	while (pPatPathoArray->end() != iterTReeView)
  {
    if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
      break ;
    iterTReeView++ ;
  }
  if (pPatPathoArray->end() != iterTReeView)
    pPatPathoArray->SupprimerItem(iterTReeView) ;

  CloseWindow(IDOK) ;
}

void
AideDecode::Cmok()
{
  CloseWindow(IDOK) ;
}

void
AideDecode::SetupWindow()
{
	TDialog::SetupWindow() ;

/*   for(int j = IDB_NORMAL; j<= IDB_ERREUR; j++)
   {
   	TBitmap img(*pNSDLLModule, j);
   	if(!Images)
   	{
   		TSize ImgSize = TSize(img.Width(), img.Height());
      	Images = new TImageList(ImgSize, true, 2, 2);
      	Images->SetBkColor(RGB(0xff, 0, 0));
   	}
		Images->Add(img);
   }
*/

 	NSPatPathoArray* pPatPathoArray = _pDecodageBase->getPPtArray() ;
 	if (((NSPatPathoArray*) NULL == pPatPathoArray) || pPatPathoArray->empty())
 		return ;

	string sLang = string("") ;
	if (pContexte)
		sLang = pContexte->getUserLanguage() ;

	_pImages = new TImageList(NS_CLASSLIB::TSize(20, 16), ILC_COLOR4, 15, 5) ;
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_NORMAL)) ;
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_ERREUR)) ;
	_pImages->Add(OWL::TBitmap(*pNSDLLModule, IDB_QUESTION)) ;
	_pTreeWind->SetImageList(TTreeWindow::Normal, *_pImages) ;

	VectTTreeNode vectTTreeNode ;

	PatPathoIter iterTReeView   = pPatPathoArray->begin() ;
	PatPathoIter dernierElement = pPatPathoArray->begin() ;

	//traitement pour GECHY

  string sLibellePatPere = buildLabel(iterTReeView) ;

	//NSTreeNode ayant pour étiquette sEtiquettePatPere et dont la colonne est 1
	TTreeNode root  = _pTreeWind->GetRoot() ;
	TTreeNode GECHY = root.AddChild(TTreeNode(*_pTreeWind, sLibellePatPere.c_str())) ;

	if ((*iterTReeView) != *(_pDecodageBase->getitDcode()))
	{
		GECHY.SetImageIndex(0) ;
		GECHY.SetSelectedImageIndex(0, true) ;
	}
	else
	{
		GECHY.SetImageIndex(1) ;
		GECHY.SetSelectedImageIndex(1, true) ;
	}
	iterTReeView++ ;

	int imageIndex = 0 ;
	//
	// On passe en revue la PatPathoArray
	//
	while (pPatPathoArray->end() != iterTReeView)
	{
  	//
    // Element situé juste sous la racine
    //
    if ((*iterTReeView)->getColonne() == (ORIGINE_PATH_PATHO + 1))
    {
    	//
      // On vide le vecteur de pères
      //
      vectTTreeNode.vider() ;
      dernierElement = iterTReeView ;

      string sEtiquettePatPere = string("") ;
      int refLigne = (*iterTReeView)->getLigne() ;
      //
      // On assemble tous les éléments situés sur la même ligne
      //
      while ((iterTReeView != pPatPathoArray->end()) &&
         		        (refLigne == (*iterTReeView)->getLigne()))
      {
      	//
        // On fabrique l'étiquette
        //
        if (string("") != sEtiquettePatPere)
          sEtiquettePatPere += string(" | ") ;

        sEtiquettePatPere = buildLabel(iterTReeView) ;

        //
        // On regarde si l'élément est l'élément fautif
        //
        if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
        	imageIndex = 1 ;

        //
        // On avance d'un cran
        //
        if (pPatPathoArray->end() != iterTReeView)
        	iterTReeView++ ;
      }
      //
      // On ajoute l'étiquette à la treenode (sous la racine)
      //
      TTreeNode* fils = new TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str()) ;
      *fils = GECHY.AddChild(TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str())) ;
      //
      // On référence le noeud comme futur père
      //
      vectTTreeNode.push_back(fils) ;
      //
      // On fixe son icone
      //
      fils->SetImageIndex(imageIndex) ;
      fils->SetSelectedImageIndex(imageIndex, true) ;

      if (1 == imageIndex)
      	imageIndex++ ;
    }
    //
    // Elément ordinaire (situé à distance de la racine)
    //
    else
    {
    	//
      // On ôte de l'array de pères tous les éléments qui ne sont
      // pas plus anciens que l'élément en cours
      //
      int colonne = (*iterTReeView)->getColonne() ;
      int derniereColonne = (*dernierElement)->getColonne() ;

      if (colonne <= derniereColonne)
      {
        // Take care, if columns are badly set, not to pop_back all fathers
        //
        size_t iDelta = derniereColonne - colonne + 1 ;
        size_t iVect  = vectTTreeNode.size() ;
        size_t iBack  = min(iDelta, iVect - 1) ;

      	for (size_t i = 0; i < iBack; i++)
        	vectTTreeNode.pop_back() ;
      }

      dernierElement = iterTReeView ;
      //
      //
      string sEtiquettePatPere = string("") ;
      int refLigne = (*iterTReeView)->getLigne() ;
      //
      // On assemble tous les éléments situés sur la même ligne
      //
      while ((pPatPathoArray->end() != iterTReeView) &&
         		        (refLigne == (*iterTReeView)->getLigne()))
      {
      	//
        // On fabrique l'étiquette
        //
        if (string("") != sEtiquettePatPere)
          sEtiquettePatPere += string(" | ") ;

        sEtiquettePatPere = buildLabel(iterTReeView) ;
        
        //
        // On regarde si l'élément est l'élément fautif
        //
        if ((*iterTReeView) == *(_pDecodageBase->getitDcode()))
        	imageIndex = 1 ;
        //
        // On avance d'un cran
        //
        if (pPatPathoArray->end() != iterTReeView)
        	iterTReeView++ ;
      }
      //
      // On prend comme père le plus jeune des aïeuls
      //
      TTreeNode* pere = vectTTreeNode.back() ; //dernier élément de pVectTTreeNode
      TTreeNode* fils = new TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str()) ;
      //
      // L'élément est ajouté à l'array des pères
      //
      *fils = pere->AddChild(TTreeNode(*_pTreeWind, sEtiquettePatPere.c_str())) ;
      vectTTreeNode.push_back(fils ) ;
      //
      // On fixe son icone
      //
      fils->SetImageIndex(imageIndex) ;
      fils->SetSelectedImageIndex(imageIndex, true) ;

      if (1 == imageIndex)
      {
      	imageIndex++ ;
        fils->EnsureVisible() ;
      }
    }
  }
}

string
AideDecode::buildLabel(PatPathoIter iterTReeView)
{
  string sLang = string("") ;
	if (pContexte)
		sLang = pContexte->getUserLanguage() ;

  string sEtiquettePatPere = (*iterTReeView)->getLexique() ;

  string sLibellePatPere = string("") ;
  bool bSuccess = false ;
  if ((string("") != sEtiquettePatPere) && ('£' != sEtiquettePatPere[0]))
    bSuccess = pContexte->getDico()->donneLibelle(sLang, &sEtiquettePatPere, &sLibellePatPere) ;
  if (false == bSuccess)
    sLibellePatPere = sEtiquettePatPere ;

	string sComplement = string("") ;
	if ((*iterTReeView)->getComplement() != string(""))
		sComplement = string(1, cheminSeparationMARK) +
      		                                (*iterTReeView)->getComplement() ;

	string sCertitudeProvisoir = string("") ;
	if ((*iterTReeView)->getCertitude() != string(""))
		sCertitudeProvisoir = string(1, cheminSeparationMARK) +
      		                    			        (*iterTReeView)->getCertitude() ;

	string sPlurielProvisoir = string("") ;
	if ((*iterTReeView)->getPluriel() != string(""))
		sPlurielProvisoir = string(1, cheminSeparationMARK) +
                                		         (*iterTReeView)->getPluriel() ;

	sLibellePatPere += sPlurielProvisoir + sCertitudeProvisoir + sComplement ;

  return sLibellePatPere ;
}

#endif

//***************************************************************************
// 							Implémentation des mèthodes VectTTreeNode
//***************************************************************************
/*
VectTTreeNode::VectTTreeNode()
              :VectorTTreeNode()
{}

void
VectTTreeNode::vider()
{
    IterTTreeNode i = begin();
    while (i != end())
    {
        delete *i;
        erase(i);
    }
}

VectTTreeNode::~VectTTreeNode()
{
  	vider();
}

VectTTreeNode::VectTTreeNode(VectTTreeNode& src)
				  :VectorTTreeNode()
{
    for (IterTTreeNode i = src.begin(); i != src.end(); i++)
   	    push_back(new TTreeNode(*(*i)));

}       */

AdjIntens::AdjIntens()
{
	_sAdjectif  = string("") ;
	_sIntensite = string("") ;
}

////////////////// NSDkdPhrase ////////////////////

NSDkdPhrase::NSDkdPhrase()
{
  categorie  = -1 ;
  sTexte     = "" ;
  sEtiquette = "" ;
  decDeb     = "" ;
  decFin     = "" ;
  sautLigne  = -1 ;
  locLesion  = "" ;

  lObjectCount++ ;
}

NSDkdPhrase::NSDkdPhrase(const NSDkdPhrase& rv)
{
  categorie  = rv.categorie ;
  sTexte     = rv.sTexte ;
  sEtiquette = rv.sEtiquette ;
  decDeb     = rv.decDeb ;
  decFin     = rv.decFin ;
  sautLigne  = rv.sautLigne ;
  locLesion  = rv.locLesion ;

  lObjectCount++ ;
}

NSDkdPhrase::~NSDkdPhrase()
{
  lObjectCount-- ;
}

NSDkdPhrase&
NSDkdPhrase::operator=(const NSDkdPhrase& src)
{
	if (this == &src)
		return *this ;

	categorie  = src.categorie ;
  sTexte     = src.sTexte ;
  sEtiquette = src.sEtiquette ;
  decDeb     = src.decDeb ;
  decFin     = src.decFin ;
  sautLigne  = src.sautLigne ;
  locLesion  = src.locLesion ;

	return *this ;
}

int
NSDkdPhrase::operator==(const NSDkdPhrase& o)
{
	if ((sTexte     == o.sTexte) &&
      (sEtiquette == o.sEtiquette))
		return 1 ;
	return 0 ;
}

/*
NSDkdPhraseArray::NSDkdPhraseArray(NSDkdPhraseArray& rv)
                 :NSDkdPhrArray()
{
    if (!(rv.empty()))
        for (NSDkdPhrArrayIter i = rv.begin(); i != rv.end(); i++)
            push_back(new NSDkdPhrase(*(*i))) ;
}

void
NSDkdPhraseArray::vider()
{
    if (empty())
        return ;

    for (NSDkdPhrArrayIter i = begin(); i != end(); )
    {
   	    delete *i ;
        erase(i) ;
    }
}

NSDkdPhraseArray::~NSDkdPhraseArray()
{
	vider() ;
}

NSDkdPhraseArray&
NSDkdPhraseArray::operator=(NSDkdPhraseArray src)
{
    vider() ;
    if (!(src.empty()))
        for (NSDkdPhrArrayIter i = src.begin(); i != src.end(); i++)
            push_back(new NSDkdPhrase(*(*i))) ;

    return *this ;
}    */

