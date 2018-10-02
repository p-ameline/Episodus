//---------------------------------------------------------------------------
//
//  		Implementation des methodes de la recherche semantique (IA)
//   									Kaddachi Hafedh
//
//---------------------------------------------------------------------------

#include <stdlib.h>
#include <algorithm>
// #include <deque.h>
// #include <owl\statusba.h>

#define	SousChemin        string("~~***") //peut remplacer n'importe quel chemin
                                          //, peut �tre vide
#define	SousCheminNonVide string("~~**1") //peut remplacer n'importe quel chemin
                                          //, ne peut pas �tre vide

#define	ItemGenerique     string("~????") // peut remplacer n'importe quel item
#define	ItemPsGenerique   string("~???~") // peut remplacer n'importe quel item

                                          // d'une cat�gorie donnee

#include "nssavoir/nspathor.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsglobalLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "nsdatabaseLus/nsSQL.h"
  #include "mysql/mysql.h"
#else
  #include "partage/nsglobal.h"
  #include "partage/nsdivfct.h"
  #include "nautilus/nssuper.h"
#endif

#include "nssavoir/nsguide.h"
#include "nssavoir/nsfilgd.h"
#include "nssavoir/nspatho.h"

#include "dcodeur/nsphrase.h"
#include "nssavoir/nsfilgdEngine.h"
#include "dcodeur/nsgenlan.h"

//class _CLASSELEXI NSPatholog ;
//***************************************************************************
// 							Impl�mentation des m�thodes NSDico
//***************************************************************************

//---------------------------------------------------------------------------
//   Constructeur
//---------------------------------------------------------------------------
NSDico::NSDico(NSContexte* pCtx)
       :NSRoot(pCtx)
{
  _pDicoDialog = (ChoixPathoDialog*) 0 ;
  //
  // Si pPatho est initialis� � 0, DicoDialog doit ouvrir puis fermer
  // le fichier. Si pPatho est initialis�, il reste ouvert jusqu'� la
  // fin du programme.
  //
  if (pContexte && pContexte->getUtilisateur())
    _sUserLang = pContexte->getUtilisateur()->donneLang() ;
  else
    _sUserLang = string("") ;

  _pPhraseur    = new NSPhraseur(pContexte) ;
  _pProposition = new NsProposition(pContexte, &_pPhraseur, NsProposition::notSetType, NsProposition::notSetConjonct, false) ;
  _pGenerateur  = (NSGenerateur*) 0 ;

  _pPatho       = (NSPatholog*) 0 ;
  _pLexiMed     = (NSLexiMed*)  0 ;

  _sLangBis     = "none" ;
  _pPathoBis    = (NSPatholog*) 0 ;
  _pLexiMedBis  = (NSLexiMed*)  0 ;

  _bOuvreFerme  = true ;
}

//---------------------------------------------------------------------------
//   Destructeur
//---------------------------------------------------------------------------
NSDico::~NSDico()
{
  if (_pGenerateur)
    delete _pGenerateur ;
  if (_pPhraseur)
    delete _pPhraseur ;
  if (_pProposition)
    delete _pProposition ;
  if (_pPatho)
    delete _pPatho ;
  if (_pLexiMed)
    delete _pLexiMed ;
  if (_pPathoBis)
    delete _pPathoBis ;
  if (_pLexiMedBis)
    delete _pLexiMedBis ;
}

//---------------------------------------------------------------------------
//   Appropriation du dictionnaire par un objet
//
//	  Si le dictionnaire est en mode Ouvre/Ferme, il faut ouvrir le fichier
//---------------------------------------------------------------------------
bool
NSDico::Prend()
{
#ifndef _ENTERPRISE_DLL
	// Lexique g�n�ral - All purposes lexicon
	//
	if ((_bOuvreFerme) && ((NSPatholog*) NULL == _pPatho))
	{
		_pPatho = new NSPatholog(pContexte->getSuperviseur(), _sUserLang) ;
		//
		// Ouverture du fichier
		//
		_pPatho->lastError = _pPatho->open() ;
		if (DBIERR_NONE != _pPatho->lastError)
		{
    	string sErrorText = pContexte->getSuperviseur()->getText("lexiqueErrors", "errorOpeningLexique") ;
      sErrorText += string(" (") + _sUserLang + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, _pPatho->lastError) ;

			delete _pPatho ;
      _pPatho = (NSPatholog*) 0 ;

			return false ;
		}
	}

  // M�dicaments - Drugs lexicon
  //
  if ((_bOuvreFerme) && (NULL == _pLexiMed))
  {
  	_pLexiMed = new NSLexiMed(pContexte->getSuperviseur(), _sUserLang) ;
		//
		// Ouverture du fichier
		//
		_pLexiMed->lastError = _pLexiMed->open() ;
		if (DBIERR_NONE != _pLexiMed->lastError)
		{
			string sErrorText = pContexte->getSuperviseur()->getText("lexiqueErrors", "errorOpeningDrugsLexique") ;
      sErrorText += string(" (") + _sUserLang + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, _pPatho->lastError) ;

			delete _pLexiMed ;
      _pLexiMed = (NSLexiMed*) 0 ;

			return false ;
    }
	}
#else

  _pPatho   = new NSPatholog(pContexte->getSuperviseur(), _sUserLang) ;
  _pLexiMed = new NSLexiMed(pContexte->getSuperviseur(), _sUserLang) ;

#endif

	// G�n�rateur - Langage generator
	//
	if ("en" == _sUserLang)
		_pGenerateur = new NSGenerateurEn(pContexte, _pProposition, string("en")) ;
  else
  	_pGenerateur = new NSGenerateurFr(pContexte, _pProposition, string("fr")) ;

	return true ;
}

//---------------------------------------------------------------------------
//   Rel�che du dictionnaire par un objet qui se l'�tait appropri�
//
//	  Si le dictionnaire est en mode Ouvre/Ferme, il faut fermer le fichier
//---------------------------------------------------------------------------
void
NSDico::Relache()
{}

bool
NSDico::UserChanged()
{
  if (NULL == pContexte->getUtilisateur())
    return true ;

  string sNewLang = pContexte->getUtilisateur()->donneLang() ;

  if (sNewLang == _sUserLang)
    return true;

  // Changement de code langue - Langage code changed
  //
  _sUserLang = sNewLang ;

  // Fermeture des lexiques - Lexicons are closed
  //
  if (_pPatho)
  {
    delete _pPatho ;
    _pPatho = (NSPatholog*) 0 ;
  }
  if (_pLexiMed)
  {
    delete _pLexiMed ;
    _pLexiMed = (NSLexiMed*) 0 ;
  }
  //
  // Fermeture du g�n�rateur - Language generator is deleted
  //
  if (_pGenerateur)
  {
    delete _pGenerateur ;
    _pGenerateur = (NSGenerateur*) 0 ;
  }

  //
  // R�ouverture des lexiques - Lexicons are opened again
  return Prend() ;
}

//---------------------------------------------------------------------------
// D�coder  pSousPatPatho selon precision est mettre les phrases dans pVect
// retourner false si le d�codage a �chou�
//---------------------------------------------------------------------------
bool
NSDico::Decoder(string /* sLang */, string /* sChemin */, NSPatPathoArray* /* pSousPatPatho */,
                               ClasseStringVector* /* pVect */, DCODETYPE /* precision */)
{
	return true ;
}

NSPatholog*
NSDico::donnePatholog(string sLang, const string* pCodeLexique)
{
	if (((string*) NULL == pCodeLexique) || (string("") == *pCodeLexique))
		return _pPatho ;

  string sCategorie = CodeCategorie(*pCodeLexique) ;

#ifndef _ENTERPRISE_DLL

	// If required langage is user langage, no problem
	//
	if (sLang == _sUserLang)
	{
		if ("_" == sCategorie) //code lexique m�dicaments
    	return _pLexiMed ;
    else
    	return _pPatho ;
	}
	// If required langage is already opened, no problem either
	//
	if (sLang == _sLangBis)
	{
  	if ("_" == sCategorie) //code lexique m�dicaments
		{
    	if (_pLexiMedBis)
      	return _pLexiMedBis ;
      else
      	return _pLexiMed ;
    }
    else
    {
    	if (_pPathoBis)
      	return _pPathoBis ;
      else
      	return _pPatho ;
    }
	}
	// Fermeture des lexiques bis - Alternative lexicons are closed
	//
	if (_pPathoBis)
	{
		delete _pPathoBis ;
		_pPathoBis = (NSPatholog*) 0 ;
	}
	if (_pLexiMedBis)
	{
		delete _pLexiMedBis ;
		_pLexiMedBis = (NSLexiMed*) 0 ;
	}
	// Puis ouverts avec la langue voulue - Then opened with accurate langage
	//
	_sLangBis = sLang ;

	// Ouverture lexique g�n�ral - Opening general lexicon
	//
	_pPathoBis = new NSPatholog(pContexte->getSuperviseur(), _sLangBis) ;
	_pPathoBis->lastError = _pPathoBis->open() ;
	if (DBIERR_NONE != _pPathoBis->lastError)
	{
  	string sErreur = string("Erreur � l'ouverture du lexique + (Langue = ")
                                + _sLangBis + string(")") ;
    erreur(sErreur.c_str(), standardError, _pPathoBis->lastError) ;

    delete _pPathoBis ;
    _pPathoBis = (NSPatholog*) 0 ;
	}

	// Ouverture lexique m�dicament - Opening drugs lexicon
	//
	_pLexiMedBis = new NSLexiMed(pContexte->getSuperviseur(), _sLangBis) ;
	_pLexiMedBis->lastError = _pLexiMedBis->open() ;
	if (DBIERR_NONE != _pLexiMedBis->lastError)
	{
  	string sErreur = string("Erreur � l'ouverture de la base des m�dicaments + (Langue = ")
                                + _sLangBis + string(")") ;
    erreur(sErreur.c_str(), standardError, _pLexiMedBis->lastError) ;

    delete _pLexiMedBis ;
    _pLexiMedBis = (NSLexiMed*) 0 ;
	}

	if (string("_") == sCategorie) //code lexique m�dicaments
	{
		if (_pLexiMedBis)
			return _pLexiMedBis ;
    else
			return _pLexiMed ;
	}
	else
	{
		if (_pPathoBis)
    	return _pPathoBis ;
    else
    	return _pPatho ;
	}
#else
  if (string("_") == sCategorie) //code lexique m�dicaments
    return _pLexiMed ;
	else
    return _pPatho ;
#endif
}

//---------------------------------------------------------------------------
//   Trouve le libell� brut (du lexique) d'un �l�ment dont on conna�t le code
//---------------------------------------------------------------------------
bool
NSDico::donneLibelleLexique(string sLang, const string* pCodeLexique, string* pLibelleTrouve)
{
	if (((string*) NULL == pCodeLexique) || ((string*) NULL == pLibelleTrouve))
		return false ;

	NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexique) ;

  *pLibelleTrouve = string("") ;

	if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	if (pPathoLexique->trouveCode(pCodeLexique) != DBIERR_NONE)
#else
  if (false == pPathoLexique->trouveCode(sLang, pCodeLexique))
#endif
		return false ;

	*pLibelleTrouve = pPathoLexique->donneLibelleLexique() ;

	return true ;
}

bool
NSDico::donneLibelleChemin(string sLang, string chemin, string* psLibelle, string sep)
{
	if ((string*) NULL == psLibelle)
		return false ;

	string sousChaine ;
  string sLibelleTrouve ;

	size_t fin   = chemin.find(sep) ;
	size_t debut = 0 ;

  string sLibelle = string("") ;

  while (fin != string::npos)
  {
  	sousChaine = string(chemin, debut, fin - debut) ;

    if (strlen(sousChaine.c_str()) < BASE_LEXI_LEN)
      donneCodeComplet(sousChaine) ;

    donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
    if (string("") != sLibelle)
    	sLibelle += sep ;

    sLibelle += sLibelleTrouve ;
    debut = fin + 1 ;
    fin   = chemin.find(sep, debut) ;
	}

	sousChaine = string(chemin, debut, strlen(chemin.c_str()) - debut) ;
  donneLibelle(sLang, &sousChaine, &sLibelleTrouve) ;
  if (string("") != sLibelle)
  	sLibelle += sep ;
  sLibelle += sLibelleTrouve ;

	*psLibelle = sLibelle ;

  return true ;
}

//---------------------------------------------------------------------------
//   Trouve le libell� d'un �l�ment dont on conna�t le code
//---------------------------------------------------------------------------
bool
NSDico::donneLibelle(string sLang, const string* pCodeLexique, string* pLibelleTrouve, LibelleType precision, GENRE *pGenre)
{
	if (((string*) NULL == pCodeLexique) || ((string*) NULL == pLibelleTrouve))
  	return false ;

	*pLibelleTrouve = string("") ;

  if ((*pCodeLexique == string("")) || (strlen(pCodeLexique->c_str()) > PATHO_CODE_LEN))
  	return false ;

  // Use a temporary since donneCodeComplet will change it and pointer is const
  //
  string sCodeLexique = *pCodeLexique ;

	if (strlen(sCodeLexique.c_str()) < PATHO_CODE_LEN)
  {
  	donneCodeComplet(sCodeLexique) ;
    if (strlen(sCodeLexique.c_str()) < PATHO_CODE_LEN)
    	return false ;
	}

	NSPatholog* pPathoLexique = donnePatholog(sLang, &sCodeLexique) ;
  if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	if (pPathoLexique->trouveCode(pCodeLexique) != DBIERR_NONE)
#else
  if (false == pPathoLexique->trouveCode(sLang, &sCodeLexique))
#endif
		return false ;

	if (precision == brut)
		*pLibelleTrouve = pPathoLexique->donneLibelleLexique() ;
	else if(precision == choisi)
		pPathoLexique->_Donnees.donneLibelleChoix(pLibelleTrouve) ;
	else if(precision == affiche)
		pPathoLexique->_Donnees.donneLibelleAffiche(pLibelleTrouve) ;

	size_t positionAccolades = pLibelleTrouve->find("{") ;
	size_t positionVideAccolades = pLibelleTrouve->find(" {") ;
	if (positionVideAccolades != string::npos)
		*pLibelleTrouve = string(*pLibelleTrouve, 0, positionVideAccolades) ;
	else if (positionAccolades != string::npos)
		*pLibelleTrouve = string(*pLibelleTrouve, 0, positionAccolades) ;

	strip(*pLibelleTrouve, stripRight) ;

	if (pGenre)
		_pPatho->_Donnees.donneGenre(pGenre) ;

	return true ;
}

//---------------------------------------------------------------------------
//   Trouve le libell� d'un �l�ment dont on conna�t le code
//---------------------------------------------------------------------------
bool
NSDico::donneShortLibelle(string sLang, const string* pCodeLexique, string* pLibelleTrouve, LibelleType precision, GENRE *pGenre)
{
	if (((string*) NULL == pCodeLexique) || ((string*) NULL == pLibelleTrouve))
  	return false ;

	*pLibelleTrouve = string("") ;

  if ((*pCodeLexique) == string(""))
  	return false ;

	size_t iShortestLength = 1000 ;
  string sShortestLib    = string("") ;
  GENRE  iShortestGenre ;

  string sSens = donneCodeSens(pCodeLexique) ;
	char   cEnd  = '1' ;
  while (cEnd < 'Z')
  {
  	string sTest = sSens + string(1, cEnd) ;
    string sResult ;
    GENRE  iGenreTest ;

    // Prevents error warning
    NSPatholog* pPathoLexique = donnePatholog(sLang, &sTest) ;
    if ((NSPatholog *) NULL == pPathoLexique)
      return false ;

#ifndef _ENTERPRISE_DLL
		if (pPathoLexique && (pPathoLexique->trouveCode(&sTest, keySEARCHEQ, dbiWRITELOCK, false) == DBIERR_NONE))
#else
    if (pPathoLexique && (pPathoLexique->trouveCode(sLang, &sTest, false)))
#endif
    {
    	bool bSuccess = donneLibelle(sLang, &sTest, &sResult, precision, &iGenreTest) ;

    	size_t iResultLength = strlen(sResult.c_str()) ;
    	if (bSuccess && (iResultLength < iShortestLength))
    	{
    		iShortestLength = iResultLength ;
      	sShortestLib    = sResult ;
      	iShortestGenre  = iGenreTest ;
    	}
    }

    cEnd++ ;
    if ((cEnd > '9') && (cEnd < 'A'))
    	cEnd = 'A' ;
  }

  *pLibelleTrouve = sShortestLib ;
  if ((GENRE*) NULL != pGenre)
  	*pGenre = iShortestGenre ;

  return (sShortestLib != string("")) ;
}

//---------------------------------------------------------------------------
//   Trouve le libell� d'un �l�ment dont on conna�t le code
//---------------------------------------------------------------------------
bool
NSDico::trouvePathologData(string sLang, const string* pCodeLexique, NSPathologData* pData, bool afficheErreur)
{
	if ((NSPathologData*) NULL == pData)
		return false ;

  if (((string*) NULL == pCodeLexique) || (string("") == *pCodeLexique))
		return false ;

	if ('�' == (*pCodeLexique)[0])
		return true ;

	NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexique) ;
  if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

#ifndef _ENTERPRISE_DLL
	return pPathoLexique->trouvePathologData(pCodeLexique, pData, afficheErreur) ;
#else
  return pPathoLexique->trouvePathologData(sLang, pCodeLexique, pData, afficheErreur) ;
#endif
}

//---------------------------------------------------------------------------
//   Trouve le code d'un �l�ment dont on conna�t le libell�
//---------------------------------------------------------------------------
bool
NSDico::donneCode(string sLang, const string* pLibelleLexique, string* pCodeTrouve)
{
	if (((string*) NULL == pCodeTrouve) || ((string*) NULL == pLibelleLexique) || (NULL == _pPatho))
		return false ;

	*pCodeTrouve = string("") ;

#ifndef _ENTERPRISE_DLL
	if (_pPatho->trouveLibelle(pLibelleLexique) != DBIERR_NONE)
#else
  if (false == _pPatho->trouveLibelle(sLang, pLibelleLexique))
#endif
  	return false ;

#ifndef _ENTERPRISE_DLL
	*pCodeTrouve = string(_pPatho->donneCode()) ;
#else
  *pCodeTrouve = _pPatho->getCode() ;
#endif
  return true ;
}

//---------------------------------------------------------------------------
//   Donne la cat�gorie
//---------------------------------------------------------------------------
string
NSDico::CodeCategorie(string sChaine)
{
	if (string("") != sChaine)
  	return string(sChaine, 0, 1) ;
  else
  	return string("") ;
}

bool
NSDico::isDrug(const string* pCodeLexiqueOrSens)
{
  if (((string*) NULL == pCodeLexiqueOrSens) || (string("") == *pCodeLexiqueOrSens))
    return false ;

  string sCategoryElement = CodeCategorie(*pCodeLexiqueOrSens) ;
  if ((string("I") == sCategoryElement) || (string("_") == sCategoryElement))
    return true ;

  return false ;
}

bool
NSDico::isDrugOrTreatment(const string* pCodeLexiqueOrSens)
{
  if (((string*) NULL == pCodeLexiqueOrSens) || (string("") == *pCodeLexiqueOrSens))
    return false ;

  if (isDrug(pCodeLexiqueOrSens))
    return true ;

  string sCategoryElement = CodeCategorie(*pCodeLexiqueOrSens) ;
  if ((string("N") == sCategoryElement) ||
      (string("O") == sCategoryElement) ||
      (string("G") == sCategoryElement))
    return true ;

  return false ;
}

/**
 * Is the drug a virtual one (say an international no-name concept)
 */
bool
NSDico::isVirtualDrug(string sLang, const string* pCodeLexiqueOrSens, string* pAfficheLabel)
{
  if (((string*) NULL == pCodeLexiqueOrSens) || (string("") == *pCodeLexiqueOrSens))
    return false ;

  if (false == isDrug(pCodeLexiqueOrSens))
    return false ;

  NSPatholog* pPathoLexique = donnePatholog(sLang, pCodeLexiqueOrSens) ;
  if ((NSPatholog *) NULL == pPathoLexique)
    return false ;

  string sFullCode = *pCodeLexiqueOrSens ;
  donneCodeComplet(sFullCode) ;

  #ifndef _ENTERPRISE_DLL
	if (pPathoLexique->trouveCode(&sFullCode) != DBIERR_NONE)
#else
  if (false == pPathoLexique->trouveCode(sLang, &sFullCode))
#endif
		return false ;

  string sRawLabel = pPathoLexique->donneLibelleLexique() ;
  size_t iPos = sRawLabel.find("{virtual}") ;
  if (NPOS == iPos)
    return false ;

  if (pAfficheLabel)
    pPathoLexique->_Donnees.donneLibelleAffiche(pAfficheLabel) ;

  return true ;
}

//---------------------------------------------------------------------------
//   Donne le code Sens en fonction du code lexique
//---------------------------------------------------------------------------
void
NSDico::donneCodeSens(string& sCode)
{
	string sCodeCat = CodeCategorie(sCode) ;
  size_t iSize    = strlen(sCode.c_str()) ;

  if ((string("�") == sCodeCat) && (iSize > BASE_FORMAT_SENS_LEN))
  {
  	if (string("�SG") != string(sCode, 0, 3))
    {
  		sCode = string(sCode, 0, BASE_FORMAT_SENS_LEN) ;
      return ;
    }
  }

	if ((string("$") != sCodeCat) && (iSize >= BASE_LEXI_LEN))
  {
  	sCode = string(sCode, 0, BASE_SENS_LEN) ;
    return ;
  }
}

//---------------------------------------------------------------------------
//   Donne le code Sens en fonction du code lexique
//---------------------------------------------------------------------------
void
NSDico::donneCodeSens(const string* pCodeLexique, string* pCodeSens)
{
	if ((string*) NULL == pCodeSens)
  	return ;

  *pCodeSens = string("") ;

  if (((string*) NULL == pCodeLexique) || (string("") == *pCodeLexique))
  	return ;

  string sBuffer = string("") ;

  // Chercher la position de cheminSeparationMARK dans la chaine
  size_t positChem = pCodeLexique->find(string(1, cheminSeparationMARK)) ;
  // Chercher la position de nodeSeparationMARK dans la chaine
  size_t positNode = pCodeLexique->find(string(1, nodeSeparationMARK)) ;
  // Chercher la position de intranodeSeparationMARK dans la chaine
  size_t positINod = pCodeLexique->find(string(1, intranodeSeparationMARK)) ;

  if ((string::npos == positNode) && (string::npos == positChem) && (string::npos == positINod))
  {
  	sBuffer = *pCodeLexique ;
    donneCodeSens(sBuffer) ;
    *pCodeSens = sBuffer ;
    return ;
	}

  size_t posit = min(positChem, positNode) ;
  posit = min(posit, positINod) ;
  size_t debut = 0 ;

  char cSepare ;

	while (((string::npos != positNode) || (string::npos != positChem) || (string::npos != positINod)) &&
                                    (posit < strlen(pCodeLexique->c_str())))
  {
  	cSepare = (*pCodeLexique)[posit] ;

    sBuffer = string(*pCodeLexique, debut, posit - debut) ;
    donneCodeSens(sBuffer) ;
    *pCodeSens = *pCodeSens + sBuffer + string(1, cSepare) ;

    debut = posit + 1 ;
    positChem = pCodeLexique->find(string(1, cheminSeparationMARK), debut) ;
    positNode = pCodeLexique->find(string(1, nodeSeparationMARK), debut) ;
    positINod = pCodeLexique->find(string(1, intranodeSeparationMARK), debut) ;

    posit = min(positChem, positNode) ;
    posit = min(posit, positINod) ;
  }

	sBuffer = string(*pCodeLexique, debut, strlen(pCodeLexique->c_str()) - debut) ;
  donneCodeSens(sBuffer) ;
  *pCodeSens = *pCodeSens + sBuffer ;
}

string
NSDico::donneCodeSens(const string* pCodeLexique)
{
	string sCodeSens = string("") ;

  donneCodeSens(pCodeLexique, &sCodeSens) ;

  return sCodeSens ;
}

void
NSDico::donneCodeComplet(string& sCode)
{
	size_t iStringLen = strlen(sCode.c_str()) ;
	if ((BASE_SENS_LEN != iStringLen) && (BASE_FORMAT_SENS_LEN != iStringLen))
		return ;

	if (BASE_SENS_LEN == iStringLen)
  {
  	sCode += string("1") ;
    return ;
  }

  if (BASE_FORMAT_SENS_LEN == iStringLen)
  {
  	sCode += string("001") ;
    return ;
  }
}

//---------------------------------------------------------------------------
//chercher dans la patpatho de synth�se le compl�ment correspondant � sItem
//exemple si sItem = VPOIDS retourner la valeur du poids
//---------------------------------------------------------------------------
string
NSDico::SetData(string sItem, string* pUnite, const NSPatPathoArray* pPathoDocEnCours)
{
  if ((NSPatPathoArray*) NULL == pPathoDocEnCours)
    return string("") ;

	//chercher dans le document en cours
	string sValeur = string("") ;

  pPathoDocEnCours->CheminDansPatpatho(0, sItem, &sValeur, pUnite) ;

  return sValeur ;
}

void
NSDico::separe(const string* psChaine, string* psLexique, string* psCertitude, string* psPluriel)
{
  if (((string*) NULL == psLexique) || ((string*) NULL == psCertitude) || ((string*) NULL == psPluriel))
    return ;

  *psLexique   = string("") ;
	*psCertitude = string("") ;
  *psPluriel   = string("") ;

  if (((string*) NULL == psChaine) || (string("") == *psChaine))
    return ;

  size_t pos = psChaine->find("/") ;
  if (NPOS == pos)
  {
  	*psLexique = *psChaine ;
    return ;
  }
  // Le code lexique est toujours au d�but
  *psLexique = string(*psChaine, 0, pos);

  string sReste = string(*psChaine, pos+1, strlen(psChaine->c_str())-pos-1) ;
  pos = sReste.find("/") ;

  string sChaine1, sChaine2 ;

  if (NPOS == pos)
  {
  	if      (string(sReste, 0, 3) == "WCE")
    	*psCertitude = sReste ;
    else if (string(sReste, 0, 4) == "WPLU")
    	*psPluriel = sReste ;
  }
  else
  {
  	sChaine1 = string(sReste, 0, pos) ;
    if      (string(sChaine1, 0, 3) == "WCE")
    	*psCertitude = sChaine1 ;
    else if (string(sChaine1, 0, 4) == "WPLU")
    	*psPluriel = sChaine1 ;

    sChaine2 = string(sReste, pos+1, strlen(sReste.c_str())-pos-1) ;
    if      (string(sChaine2, 0, 3) == "WCE")
    	*psCertitude = sChaine2 ;
    else if (string(sChaine2, 0, 4) == "WPLU")
    	*psPluriel = sChaine2 ;
	}
}

bool
NSDico::getAllSynonyms(const string sCode, NSPathoInfoArray* paSynonyms, const string sLang)
{
  if (((NSPathoInfoArray*) NULL == paSynonyms) || (string("") == sCode))
    return false ;

  string sSens = donneCodeSens(&sCode) ;

  size_t iSensLen = strlen(sSens.c_str()) ;

  NSPatholog* pPathoLexique = donnePatholog(sLang, &sSens) ;
  if ((NSPatholog*) NULL == pPathoLexique)
    return false ;

  // Search for the semantic root code
  //
  DBIResult lastError = pPathoLexique->chercheClef(&sSens, "CODE_INDEX", keySEARCHGEQ, dbiWRITELOCK) ;
  if ((DBIERR_NONE != lastError) && (DBIERR_EOF != lastError))
    return false ;

  //
	// Si on se trouve positionn� en fin de fichier on recule
	//
	if (DBIERR_EOF == lastError)
    lastError = pPathoLexique->precedent(dbiWRITELOCK) ;

  lastError = pPathoLexique->getRecord() ;

  if (DBIERR_NONE != lastError)  {
    erreur("Erreur � la lecture du Lexique", standardError, lastError, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sRecordCode = pPathoLexique->getCode() ;

  while ((DBIERR_NONE == lastError) && (strlen(sRecordCode.c_str()) > iSensLen) && (string(sRecordCode, 0, iSensLen) == sSens))
	{
    // Add this record to the results array
    //
    paSynonyms->push_back(new NSPathologInfo(pPathoLexique)) ;

    // Get next record
    //
    lastError = pPathoLexique->suivant(dbiWRITELOCK) ;
    if (DBIERR_NONE == lastError)
    {
      lastError = pPathoLexique->getRecord() ;
      if (DBIERR_NONE != lastError)
      {
        erreur("Erreur � la lecture du Lexique", standardError, lastError, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }

      sRecordCode = pPathoLexique->getCode() ;
    }
	}

  return true ;
}

//-----------------------------------------------------------------------
// fonction globale
//-----------------------------------------------------------------------
bool
inferieure(string* s, string* b)
{
	return (*s < *b) ;
}

// -----------------------------------------------------------------------------
//
// Impl�mentation des m�thodes NSFilGuide
//
// -----------------------------------------------------------------------------

NSFilGuide::NSFilGuide(NSSuper* pSuper, string sTypeBD)
           :NSSuperRoot(pSuper)
{
  _sTypeBase = sTypeBD ;
  _pBBFiche  = (BBChem*) 0 ;

#ifndef _ENTERPRISE_DLL
  _pSavoir = (NSSavoir*) 0 ;
#else
  _pSavoir = new NSSavoir(_pSuper) ;
#endif

  _bOuvreFerme = true ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
NSFilGuide::~NSFilGuide()
{
  if (_pBBFiche)
    delete _pBBFiche ;

  if (_pSavoir)
    delete _pSavoir ;
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool
NSFilGuide::Prend()
{
	if (false == _bOuvreFerme)
		return true ;

	// Ouverture du fichier
try
{
	if ((BBChem*) NULL == _pBBFiche)
  {
    if (GUIDE == _sTypeBase)
    {
      _pBBFiche = new BBFiche(_pSuper) ;
      BBFiche* pBBCommeFiche = static_cast<BBFiche *>(_pBBFiche) ;
      pBBCommeFiche->lastError = pBBCommeFiche->open() ;
      if (pBBCommeFiche->lastError != DBIERR_NONE)
      {
        erreur("Erreur � l'ouverture du Fil guide.", standardError, pBBCommeFiche->lastError) ;
        delete _pBBFiche ;
        _pBBFiche = (BBChem*) 0 ;
        return false ;
      }
    }
    else if (DECODE == _sTypeBase)
    {
      _pBBFiche = new BBDecode(_pSuper) ;
      BBDecode* pBBCommeDecode = static_cast<BBDecode *>(_pBBFiche) ;
      pBBCommeDecode->lastError = pBBCommeDecode->open() ;
      if (pBBCommeDecode->lastError != DBIERR_NONE)
      {
        erreur("Erreur � l'ouverture du D�codeur.", standardError, pBBCommeDecode->lastError) ;
        delete _pBBFiche ;
        _pBBFiche = (BBChem*) 0 ;
        return false ;
      }
    }
	}
}
catch (...)
{
	erreur("NSFilGuide::Prend : ouverture de la table 1.", standardError, 0) ;
  return false ;
}

try
{
	if (_pSavoir)
		return true ;

	_pSavoir = new NSSavoir(_pSuper) ;
  _pSavoir->lastError = _pSavoir->open() ;
  if (_pSavoir->lastError != DBIERR_NONE)
  {
  	erreur("Erreur � l'ouverture du r�seau s�mantique : pSavoir", standardError, _pSavoir->lastError) ;
    delete _pSavoir ;
    _pSavoir = (NSSavoir*) 0 ;
    return false ;
  }
}
catch (...)
{
	erreur("NSFilGuide::Prend : ouverture de la table 2.", standardError, 0) ;
  return false ;
}
	return true ;
}
#endif

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
NSFilGuide::Relache()
{
	if (false == _bOuvreFerme)
		return ;

	delete _pBBFiche ;
  _pBBFiche = (BBChem*) 0 ;
  delete _pSavoir ;
  _pSavoir = (NSSavoir*) 0 ;
}


// -----------------------------------------------------------------------------
//Egalit� au sens stricte
// -----------------------------------------------------------------------------
void
NSFilGuide::chercheCheminEgaliteStricte(string* pCheminPere, VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
  if (((VecteurRechercheSelonCritere*) NULL == pVecteurSelonCritere) ||
                                                  pVecteurSelonCritere->empty())
    return ;

#ifndef _ENTERPRISE_DLL
  bool continuer = true ;
  IterCritere iter = pVecteurSelonCritere->begin() ;
  for ( ; pVecteurSelonCritere->end() != iter ; iter++)
  {
    if (false == (*iter)->trouve)
    {
      string sCheminFils ;
      if (string("") == *pCheminPere)
        sCheminFils = (*iter)->sEtiquette ;
      else
        sCheminFils = (*pCheminPere) + string(1, cheminSeparationMARK) + (*iter)->sEtiquette ;

      _pBBFiche->lastError = _pBBFiche->chercheClef(&sCheminFils, "CHEMIN", NODEFAULTINDEX, keySEARCHGEQ, dbiWRITELOCK) ;
      if (_pBBFiche->lastError != DBIERR_NONE)
      {
        (*iter)->trouve = false ;
        continuer = false ; //passer au fils suivant
      }

      if (true == continuer)
      {
        _pBBFiche->lastError = _pBBFiche->getRecord() ;
        if (_pBBFiche->lastError != DBIERR_NONE)
        {
          erreur("Le fichier de script est inutilisable (guides.db).", standardError, _pBBFiche->lastError) ;
          (*iter)->trouve = false ;
          return ;
        }
        // if (strncmp(pBBFiche->pDonnees->chemin, sCheminFils.c_str(), strlen(sCheminFils.c_str())) == 0 )
        if (_pBBFiche->getSemanticPath() == sCheminFils)
        {
          // (*iter)->trouve = true ;

          if      (GUIDE == _sTypeBase)
          {
            BBFiche* pBBCommeFiche = static_cast<BBFiche *>(_pBBFiche) ;
            RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
            BBItemData* pDo = pBBCommeFiche->getData() ;
            if (pDo)
              *(pRSC->pDonnees) = *pDo ;

            string sMsg = string("Path ") + sCheminFils + string(" found straight ") + pDo->getGroupID() + string(" ") + pDo->getID() + string(" ") + pDo->getSonsList() ;
            _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
          }
          else if (DECODE == _sTypeBase)
          {
            BBDecode* pBBCommeDecode = static_cast<BBDecode *>(_pBBFiche) ;
            RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
            BBDecodeData* pDo = pBBCommeDecode->getData() ;
            *(pRSC->pDonnees) = *pDo ;
          }
          (*iter)->trouve = true ;

/*
          //savoir si cette fiche correspond � iter
          EquiItemVectorIter equivalent = (*iter)->pEquivalentBrut->begin() ;
          for ( ; (equivalent != (*iter)->pEquivalentBrut->end()) && ((*iter)->trouve == false) ; equivalent++)
          {
            EquiItemIter iterEqui = (*equivalent)->begin() ;
            string sEquivalent = *(*iterEqui) ;
            string sChemin = pBBFiche->pDonnees->chemin ;
            string sCheminFilGuide ;
            size_t posit = sChemin.find(string(1, cheminSeparationMARK)) ;
            if (posit == string::npos)
            	sCheminFilGuide = sChemin ;
            else
            {
              size_t debut = 0 ;
              while (posit != string::npos)
              {
                debut = posit ;
                posit = sChemin.find(string(1, cheminSeparationMARK), debut + 1) ;
              }
              sCheminFilGuide = string(sChemin, debut + 1, strlen(sChemin.c_str()) - debut - 1) ;
            }
            if (sEquivalent == sCheminFilGuide)
            {
              if (sTypeBase == GUIDE)
              {
                BBFiche* pBBCommeFiche = static_cast<BBFiche *>(pBBFiche) ;
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
                BBItemData* pDo = static_cast<BBItemData *>(pBBCommeFiche->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              else if (sTypeBase == DECODE)
              {
                BBDecode* pBBCommeDecode = static_cast<BBDecode *>(pBBFiche) ;
                RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
                BBDecodeData* pDo = static_cast<BBDecodeData *>(pBBCommeDecode->pDonnees) ;
                *(pRSC->pDonnees) = *pDo ;
              }
              (*iter)->trouve = true ;
            }
          }

          if ((*iter)->trouve == false)
          {
            if (sTypeBase == GUIDE)
            {
              RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData *>(*iter) ;
              BBItemData* pDo = static_cast<BBItemData *>(pRSC->pDonnees) ;
              pRSC->pVecteurData->push_back(new BBItemData(*pDo)) ;
            }
            else if(sTypeBase == DECODE)
            {
              RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode *>(*iter) ;
              BBDecodeData* pDo = static_cast<BBDecodeData *>(pRSC->pDonnees) ;
              pRSC->pVecteurData->push_back(new BBDecodeData(*pDo)) ;
            }
          }
*/
        }
        else
        	(*iter)->trouve = false ;
      }
      continuer = true ;
    }
  }
#endif
}

// -----------------------------------------------------------------------------
// Egalit� au sens r�seau
// -----------------------------------------------------------------------------
// D�composer pChaine et mettre ses items dans le vecteur pVect
// -----------------------------------------------------------------------------
void
NSFilGuide::DecomposeChaine(const string* pChaine, VecteurString* pVect)
{
	if (((string*) NULL == pChaine) || ((VecteurString*) NULL == pVect))
		return ;

  size_t iPosit = pChaine->find(string(1, cheminSeparationMARK)) ;

  if (string::npos == iPosit)
  {
  	pVect->AddString(*pChaine) ;
    return ;
  }

  size_t iStart = 0 ;
  size_t iLen   = strlen(pChaine->c_str()) ;

  while ((string::npos != iPosit) && (iPosit < iLen))
  {
    pVect->AddString(string(*pChaine, iStart, iPosit - iStart)) ;
    iStart = iPosit + 1 ;
    iPosit = pChaine->find(string(1, cheminSeparationMARK), iStart + 1) ;
  }

  pVect->AddString(string(*pChaine, iStart, iLen - iStart)) ;
}


// -----------------------------------------------------------------------------
// Trouver les items �quivalents � sQualifie au sens r�seau s�mantique selon
// le lien sLien et les ranger dans pEquivalentItem
// Par d�faut sensCle == "FLECHE"
// -----------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
void
NSFilGuide::chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem, string sLien, DBITBLNAME sensCle)
{
	if (((VecteurString *) NULL == pEquivalentItem) ||
      ((NSSavoir *)      NULL == _pSavoir) ||
      (string("") == sQualifie) || (string("") == sLien))
  	return ;

try
{
	if (sLien == "ES")
  	pEquivalentItem->AddString(sQualifie) ;
  //
  // Pr�paration de ItemPsGenerique, on remplace le caract�re final par
  // le caract�re qui donne le type de sQualifie
  //
  string sItemPseudoGenerique = ItemPsGenerique;
  sItemPseudoGenerique[strlen(sItemPseudoGenerique.c_str())-1] = sQualifie[0];
  //
  // Recherche dans le fichier
  //
  DBIResult ErrDBI ;
  CURProps  curProps;
  DBIResult lastError = DbiGetCursorProps(_pSavoir->PrendCurseur(), curProps);
  if (lastError != DBIERR_NONE)
  {
  	erreur("Le fichier nssavoir.db est inutilisable.", standardError, lastError);
    return ;
  }
	Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
	memset(pIndexRec, 0, curProps.iRecBufSize) ;
  //
  // Pr�paration de la cl� � trouver
  //
  DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndexRec,(Byte*)(sLien.c_str()));
  if (strcmp(sensCle, "FLECHE") == 0)
		DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD , pIndexRec, (Byte*)(sQualifie.c_str())) ;
  if (strcmp(sensCle, "ENVERS") == 0)
  {
		DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_QUALIFIANT_FIELD , pIndexRec, (Byte*)(sQualifie.c_str())) ;
		DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_DEGRE_FIELD , pIndexRec, (Byte*)("")) ;
  }

  ErrDBI = _pSavoir->chercheClefComposite(sensCle,
                                        NODEFAULTINDEX,
                                        keySEARCHEQ,
                                        dbiWRITELOCK,
                                        pIndexRec) ;
  //
  // Si on ne trouve pas dans le r�seau s�mantique et que le lien cherch� est
  // du type "est un", on met les g�n�riques
  //
	if (ErrDBI != DBIERR_NONE)
	{
  	if ((string("ES") == sLien) && (strcmp(sensCle, "FLECHE") == 0))
    {
      pEquivalentItem->AddString(sItemPseudoGenerique) ;
      pEquivalentItem->AddString(ItemGenerique) ;
      pEquivalentItem->AddString(SousChemin) ;
      pEquivalentItem->AddString(SousCheminNonVide) ;
    }
    if (ErrDBI != DBIERR_RECNOTFOUND)
    	erreur("Le fichier nssavoir.db est inutilisable.", standardError, ErrDBI) ;

    delete[] pIndexRec ;
    return ;
	}

	_pSavoir->lastError = _pSavoir->getRecord() ;
	if (_pSavoir->lastError != DBIERR_NONE)
	{
    erreur("Le fichier nssavoir.db est inutilisable.", standardError, _pSavoir->lastError) ;
    delete[] pIndexRec ;
    return ;
	}
  //
  // Premi�re passe : on stocke les �l�ments en relation directe avec sQualifie
  //
  while ((_pSavoir->lastError == DBIERR_NONE) &&
         (_pSavoir->getLink() == sLien) &&
         (((strcmp(sensCle, "FLECHE") == 0) &&
           (_pSavoir->getQualified() == sQualifie)
          ) ||
          ((strcmp(sensCle, "ENVERS") == 0) &&
           (_pSavoir->getQualifier() == sQualifie)
          )
         )
        )
	{
    // V�rifier que l'�l�ment n'appartient pas au vecteur
    if ((strcmp(sensCle, "FLECHE") == 0) &&
        (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualifier()) == false) &&
        (_pSavoir->getLevel() == string("")))
      pEquivalentItem->AddString(_pSavoir->getQualifier()) ;

    if ((strcmp(sensCle, "ENVERS") == 0) &&
        (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualified()) == false) &&
        (_pSavoir->getLevel() == string("")))
      pEquivalentItem->AddString(_pSavoir->getQualified()) ;

    _pSavoir->lastError = _pSavoir->suivant(dbiWRITELOCK) ;
    if (_pSavoir->lastError == DBIERR_NONE)
    {
    	_pSavoir->lastError = _pSavoir->getRecord() ;
      if (_pSavoir->lastError != DBIERR_NONE)
      {
      	erreur("Le fichier nssavoir.db est inutilisable.", standardError, _pSavoir->lastError) ;
        delete[] pIndexRec ;
        return ;
      }
    }
  }
  //
  // Deuxi�me passe : on prend les �l�ments en relation avec des �l�ments en
  // relation. Comme tout �l�ment nouveau est ajout� � la fin de l'array,
  // on n'atteint end() que lorsqu'on a trouv� tous les �l�ment quelque soit
  // leur niveau
  //
  if ((false == pEquivalentItem->empty()) && (sLien != "E0"))
  {
    EquiItemIter iter = pEquivalentItem->begin();
    while (iter != pEquivalentItem->end())
    {
      //DBIResult lastError = DbiGetCursorProps(pSavoir->PrendCurseur(), curProps);
      DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndexRec,(Byte*)(sLien.c_str())) ;
      if (strcmp(sensCle, "FLECHE") == 0)
        DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD, pIndexRec, (Byte*)((*iter)->c_str())) ;
      if (strcmp(sensCle, "ENVERS") == 0)
      {
        DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_QUALIFIANT_FIELD, pIndexRec, (Byte*)((*iter)->c_str())) ;
        DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_DEGRE_FIELD, pIndexRec, (Byte*)("")) ;
      }

      ErrDBI = _pSavoir->chercheClefComposite(sensCle,
                                            NODEFAULTINDEX,
                                            keySEARCHEQ,
                                            dbiWRITELOCK,
                                            pIndexRec);
      if (DBIERR_NONE == ErrDBI)
      {
        _pSavoir->lastError = _pSavoir->getRecord() ;

        while ((_pSavoir->lastError == DBIERR_NONE) &&
                 (_pSavoir->getLink() == sLien) &&
                 (((strcmp(sensCle, "FLECHE") == 0) &&
                   (_pSavoir->getQualified() == **iter)
                  ) ||
                  ((strcmp(sensCle, "ENVERS") == 0) &&
                   (_pSavoir->getQualifier() == **iter)
                  )
                 )
                )
        {
          // Redimensionnement pr�ventif

          /*
          size_t iCurrentSize  = pEquivalentItem->size() ;
          size_t iCurrentCapa  = pEquivalentItem->capacity() ;
          if (iCurrentCapa < 10 + iCurrentSize)
          {
            string sIndex = **iter ;

            pEquivalentItem->reserve(100 + iCurrentSize) ;

            iter = pEquivalentItem->begin() ;
            for ( ; (iter != pEquivalentItem->end()) && (**iter != sIndex); iter++) ;
            if (iter == pEquivalentItem->end())
            {
              erreur("Erreur lors du redimensionnement : le noeud d'insertion n'est plus pr�sent.", standardError, 0) ;
              return ;
            }
          }
          */

          // V�rifier que l'�l�ment n'appartient pas au vecteur
          if ((strcmp(sensCle, "FLECHE") == 0) &&
                      (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualifier()) == false) &&
                      (_pSavoir->getLevel() == string("")))
            pEquivalentItem->AddString(_pSavoir->getQualifier()) ;

          if ((strcmp(sensCle, "ENVERS") == 0) &&
                      (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualified()) == false) &&
                      (_pSavoir->getLevel() == string("")))
            pEquivalentItem->AddString(_pSavoir->getQualified()) ;

          _pSavoir->lastError = _pSavoir->suivant(dbiWRITELOCK) ;
          if (_pSavoir->lastError == DBIERR_NONE)
          {
            _pSavoir->lastError = _pSavoir->getRecord() ;
            if (_pSavoir->lastError != DBIERR_NONE)
            {
              erreur("Le fichier nssavoir.db est inutilisable.", standardError, _pSavoir->lastError) ;
              delete[] pIndexRec ;
              return ;
            }
          }
        }
      }
      else if (ErrDBI != DBIERR_RECNOTFOUND)
      {
        erreur("Le fichier nssavoir.db est inutilisable.", standardError, ErrDBI) ;
        delete[] pIndexRec ;
        return ;
      }

      iter++ ;
    }
  }

  if ((string("ES") == sLien) && (strcmp(sensCle, "FLECHE") == 0))
  {
    pEquivalentItem->AddString(sItemPseudoGenerique) ;
    pEquivalentItem->AddString(ItemGenerique) ;
    pEquivalentItem->AddString(SousChemin) ;
    pEquivalentItem->AddString(SousCheminNonVide) ;
  }
  delete[] pIndexRec ;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheEquivalent", standardError, 0);
  return ;
}
}

#else

void
NSFilGuide::chercheEquivalent(string sQualifie, VecteurString* pEquivalentItem, string sLien, string sensCle)
{
	if (((VecteurString *) NULL == pEquivalentItem) ||
      ((NSSavoir *) NULL == _pSavoir) ||
      (string("") == sQualifie) || (string("") == sLien) || (string("") == sensCle))
  	return ;

try
{
	if (string("ES") == sLien)
  	pEquivalentItem->AddString(sQualifie) ;
  //
  // Pr�paration de ItemPsGenerique, on remplace le caract�re final par
  // le caract�re qui donne le type de sQualifie
  //
  string sItemPseudoGenerique = ItemPsGenerique ;
  sItemPseudoGenerique[strlen(sItemPseudoGenerique.c_str())-1] = sQualifie[0] ;

  string sQuery = string("SELECT * FROM savoir WHERE ") ;

  if      (string("FLECHE") == sensCle)
  {
    sQuery += string("QUALIFIE = \"") + sQualifie + string("\" AND ") ;
    sQuery += string("LIEN = \"") + sLien + string("\"") ;
  }
  else if (string("ENVERS") == sensCle)
  {
    sQuery += string("QUALIFIANT = \"") + sQualifie + string("\" AND ") ;
    sQuery += string("LIEN = \"") + sLien + string("\" AND ") ;
    sQuery += string("DEGRE = \"\"") ;
  }
  else
    return ;

  MYSQL_RES *pResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, true) ;

  if ((MYSQL_RES *) NULL == pResult)
	{
  	if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
    {
      pEquivalentItem->AddString(sItemPseudoGenerique) ;
      pEquivalentItem->AddString(ItemGenerique) ;
      pEquivalentItem->AddString(SousChemin) ;
      pEquivalentItem->AddString(SousCheminNonVide) ;
    }
    return ;
	}

  MYSQL_ROW Row = mysql_fetch_row(pResult) ;
  if (Row)
    _pSavoir->alimenteFiche(&Row) ;
  else
  {
    string sErreurMsg = string("Error when fetching row for query [") + sQuery + string("] -> ") + mysql_error(_pSuper->getDatabaseManager()->getConnector()) ;
    erreur(sErreurMsg.c_str(), standardError) ;

    if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
    {
      pEquivalentItem->AddString(sItemPseudoGenerique) ;
      pEquivalentItem->AddString(ItemGenerique) ;
      pEquivalentItem->AddString(SousChemin) ;
      pEquivalentItem->AddString(SousCheminNonVide) ;
    }

    mysql_free_result(pResult) ;

    return ;
  }

  //
  // Premi�re passe : on stocke les �l�ments en relation directe avec sQualifie
  //
  while (Row && (_pSavoir->getLink() == sLien) &&
         (((string("FLECHE") == sensCle) &&
           (_pSavoir->getQualified() == sQualifie)
          ) ||
          ((string("ENVERS") == sensCle) &&
           (_pSavoir->getQualifier() == sQualifie)
          )
         )
        )
	{
    // V�rifier que l'�l�ment n'appartient pas au vecteur
    if ((string("FLECHE") == sensCle) &&
        (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualifier()) == false) &&
        (_pSavoir->pDonnees->getLevel() == ""))
      pEquivalentItem->AddString(_pSavoir->getQualifier()) ;

    if ((string("ENVERS") == sensCle) &&
        (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualified()) == false) &&
        (_pSavoir->pDonnees->getLevel() == ""))
      pEquivalentItem->AddString(_pSavoir->getQualified()) ;

    Row = mysql_fetch_row(pResult) ;
    if (Row)
      _pSavoir->alimenteFiche(&Row) ;
  }

  mysql_free_result(pResult) ;

  //
  // Deuxi�me passe : on prend les �l�ments en relation avec des �l�ments en
  // relation. Comme tout �l�ment nouveau est ajout� � la fin de l'array,
  // on n'atteint end() que lorsqu'on a trouv� tous les �l�ment quelque soit
  // leur niveau
  //
  if ((false == pEquivalentItem->empty()) && (string("E0") != sLien))
  {
    EquiItemIter iter = pEquivalentItem->begin() ;
    while (pEquivalentItem->end() != iter)
    {
      sQuery = string("SELECT * FROM savoir WHERE ") ;

      if      (string("FLECHE") == sensCle)
      {
        sQuery += string("QUALIFIE = \"") + **iter + string("\" AND ") ;
        sQuery += string("LIEN = \"") + sLien + string("\"") ;
      }
      else if (string("ENVERS") == sensCle)
      {
        sQuery += string("QUALIFIANT = \"") + **iter + string("\" AND ") ;
        sQuery += string("LIEN = \"") + sLien + string("\" AND ") ;
        sQuery += string("DEGRE = \"\"") ;
      }

      MYSQL_RES *pSubResult = _pSuper->getDatabaseManager()->executeQuery(sQuery, true) ;

      if (pSubResult)
      {
        MYSQL_ROW subRow = mysql_fetch_row(pSubResult) ;
        if (subRow)
          _pSavoir->alimenteFiche(&subRow) ;

        while (subRow && (_pSavoir->getLink() == sLien) &&
                 (((string("FLECHE") == sensCle) &&
                   (_pSavoir->getQualified() == **iter)
                  ) ||
                  ((string("ENVERS") == sensCle) &&
                   (_pSavoir->getQualifier() == **iter)
                  )
                 )
                )
        {
          /*
          // Redimensionnement pr�ventif
          size_t iCurrentSize  = pEquivalentItem->size() ;
          size_t iCurrentCapa  = pEquivalentItem->capacity() ;
          if (iCurrentCapa < 10 + iCurrentSize)
          {
            string sIndex = **iter ;

            pEquivalentItem->reserve(100 + iCurrentSize) ;

            iter = pEquivalentItem->begin() ;
            for ( ; (iter != pEquivalentItem->end()) && (**iter != sIndex); iter++) ;
            if (iter == pEquivalentItem->end())
            {
              erreur("Erreur lors du redimensionnement : le noeud d'insertion n'est plus pr�sent.", standardError) ;
              return ;
            }
          }
          */

                // V�rifier que l'�l�ment n'appartient pas au vecteur
          if ((string("FLECHE") == sensCle) &&
                      (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualifier()) == false) &&
                  (_pSavoir->getLevel() == string("")) )
            pEquivalentItem->AddString(_pSavoir->getQualifier()) ;
          if ((string("ENVERS") == sensCle) &&
                      (pEquivalentItem->ItemDansUnVecteur(_pSavoir->getQualified()) == false) &&
                  (_pSavoir->getLevel() == string("")) )
            pEquivalentItem->AddString(_pSavoir->getQualified()) ;

          subRow = mysql_fetch_row(pSubResult) ;
          if (subRow)
            _pSavoir->alimenteFiche(&subRow) ;
        }
      }
      else if ((MYSQL_RES *) NULL == pSubResult)
        return ;

      mysql_free_result(pSubResult) ;

      iter++ ;
    }
  }

  if ((string("ES") == sLien) && (string("FLECHE") == sensCle))
  {
    pEquivalentItem->AddString(sItemPseudoGenerique) ;
    pEquivalentItem->AddString(ItemGenerique) ;
    pEquivalentItem->AddString(SousChemin) ;
    pEquivalentItem->AddString(SousCheminNonVide) ;
  }
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheEquivalent", standardError) ;
  return ;
}
}
#endif

//-------------------------------------------------------------------
//pour un fils cherch�, choisir dans la liste des fiches trouv�es, celle
//qui correspond � l'�l�ment qui lui est le plus proche dans le guide
//-------------------------------------------------------------------
void
NSFilGuide::ChoisirMeilleureProposition(VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
	if ((NULL == pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
		return ;

	IterCritere iterFils = pVecteurSelonCritere->begin() ;
	for (; iterFils != pVecteurSelonCritere->end(); iterFils++)
	{
  	//
    // On ne cherche que les �l�ments qui n'ont pas d�j� �t� trouv�s
    //
    if (false == (*iterFils)->trouve)
    {
    	EquiItemVectorIter equivalent = (*iterFils)->pEquivalentBrut->begin() ;
      // Choisir la fiche qui correspond � l'�quivalent le plus proche du
      // fils recherch�
      for (; (equivalent != (*iterFils)->pEquivalentBrut->end()) &&
                                ((*iterFils)->trouve == false); equivalent++)
      {
      	if (false == (*equivalent)->empty())
        {
      		EquiItemIter iterEqui = (*equivalent)->begin() ;
          for (; (iterEqui != (*equivalent)->end()) && ((*iterFils)->trouve == false);
				            iterEqui++)
          {
          	string sEquivalent = *(*iterEqui) ;
            if (GUIDE == _sTypeBase)
            {
            	RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iterFils) ;
              EquiDataIter iterData = pRSC->pVecteurData->begin() ;

              for (; (iterData != pRSC->pVecteurData->end()) && (pRSC->trouve == false);
      	         		                                            iterData++)
              {
              	string sChemin = (*iterData)->chemin ;
                string sCheminFilGuide ;
                size_t posit = sChemin.find(string(1, cheminSeparationMARK)) ;
                if (string::npos == posit)
                	sCheminFilGuide = sChemin ;
                else
                {
                	size_t debut = 0 ;
                  while (string::npos != posit)
                  {
                  	debut = posit ;
                    posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
                  }
                  sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1) ;
                }

                if (sEquivalent == sCheminFilGuide)
                {
                	*(pRSC->pDonnees) = *(*iterData) ;
                  (*iterFils)->trouve = true ;
                }
              }
            }
            else if (DECODE == _sTypeBase)
            {
            	RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iterFils) ;
              EquiDecodeIter iterData = pRSC->pVecteurData->begin() ;
              for (; (iterData != pRSC->pVecteurData->end()) && (pRSC->trouve == false);
				      	         		iterData++)
              {
              	string sChemin = (*iterData)->chemin ;
                string sCheminFilGuide ;
                size_t posit = sChemin.find(string(1,cheminSeparationMARK)) ;
    						if (string::npos == posit)
                	sCheminFilGuide = sChemin ;
                else
                {
                	size_t debut = 0 ;
    							while (string::npos != posit)
    							{
                  	debut = posit ;
                    posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
    							}
    							sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1);
                }
                if (sEquivalent == sCheminFilGuide)
                {
                	*(pRSC->pDonnees) = *(*iterData) ;
                  (*iterFils)->trouve = true ;
                }
              }
            }
          }
        }
      }
    }
	}
}

//---------------------------------------------------------------------------
// Comparer les deux items sItem1 et sItem2
// pEquivalentItem : vecteur contenant les �quivalents s�mantiques de sItem1
//---------------------------------------------------------------------------
bool
NSFilGuide::comparerItem(string sItem2, VecteurString* pEquivalentItem)
{
	if (((VecteurString*) NULL == pEquivalentItem) || (pEquivalentItem->empty()))
  	return false ;

  //�galit� au sens r�seau
  for (EquiItemIter i = pEquivalentItem->begin() ; pEquivalentItem->end() != i ; i++)
  	if (**i == sItem2)
    	return true ;

  return false ;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
int
NSFilGuide::NombreSeparateurDansUneChaine(string *pChaine)
{
	if (((string*) NULL == pChaine) || (string("") == *pChaine))
  	return 0 ;

  string sSeparator = string(1, cheminSeparationMARK) ;

  int nb = 0 ;
  size_t posit = pChaine->find(sSeparator) ;

  if (string::npos == posit)
  	return 0 ;

	while (string::npos != posit)
  {
    nb++ ;
    posit = pChaine->find(sSeparator, posit + 1) ;
  }

  return nb ;
}

// Distance between a genuine source path (without jokers) and
// a "generic" target path
//
int
NSFilGuide::pathDistance(string* psTarget, string* psSource, bool bWithSemantic, DBITBLNAME sensCle)
{
	// Trivial cases
	//
	if (((string*) NULL == psSource) || ((string*) NULL == psTarget))
  	return pathDistanceInfinite ;

  if (*psSource == *psTarget)
  	return pathDistanceExact ;

  if ((*psSource == string("")) || (string("") == *psTarget))
  	return pathDistanceInfinite ;

  // More complex cases
  //

  bool bJokerInTarget = false ;

  // Not to waste time : if no joker and no semantics, leave now
  //
  if ((psTarget->find(string("*")) != string::npos) ||
      (psTarget->find(string("?")) != string::npos))
  	bJokerInTarget = true ;

  if (!bWithSemantic && !bJokerInTarget)
  	return pathDistanceInfinite ;

  // Target parsing
  //
  VecteurString VectItemTarget ;
  DecomposeChaine(psTarget, &VectItemTarget) ;
  // Should never occur ; just by security
  if (VectItemTarget.empty())
  	return pathDistanceInfinite ;

  // Source semantic parsing
  //
  VecteurString VectItemSource ;
  DecomposeChaine(psSource, &VectItemSource) ;
  // Should never occur ; just by security
  if (VectItemSource.empty())
  	return pathDistanceInfinite ;

  VecteurItem SourceVecEquivalentItem ;

  // First, if there are jokers in the target, we try a pure generic approach
  // in order not to waste time in the semantic network
  //
  if (bJokerInTarget)
  {
  	// Building the vector with only the generic concepts of source path elements
  	for (EquiItemIter iterSource = VectItemSource.begin() ; VectItemSource.end() != iterSource ; iterSource++)
  	{
  		VecteurString* pEquivalentItem = new VecteurString() ;

      pEquivalentItem->AddString(**iterSource) ;
      pEquivalentItem->AddString(ItemGenerique) ;
      pEquivalentItem->AddString(SousChemin) ;
      pEquivalentItem->AddString(SousCheminNonVide) ;

    	sort(pEquivalentItem->begin(), pEquivalentItem->end(), inferieure) ;
    	SourceVecEquivalentItem.push_back(pEquivalentItem) ;
  	}

  	int iPureGenericDistance = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
    if (iPureGenericDistance < pathDistanceInfinite)
    	return pathDistanceGeneric + iPureGenericDistance ;

    SourceVecEquivalentItem.vider() ;
  }

  if (false == bWithSemantic)
  	return pathDistanceInfinite ;

  // Building the vector with all the semantic related concepts of source path elements
  //
  for (EquiItemIter iterSource = VectItemSource.begin(); iterSource != VectItemSource.end(); iterSource++)
  {
  	VecteurString* pEquivalentItem = new VecteurString() ;

    chercheEquivalent(*(*iterSource), pEquivalentItem, string("ES"), sensCle) ;
    sort(pEquivalentItem->begin(), pEquivalentItem->end(), inferieure) ;

    SourceVecEquivalentItem.push_back(pEquivalentItem) ;
  }

  // If can we find jokers in the target, it means generic + semantic
  //
  if (bJokerInTarget)
  {
		int iGenericSemantic = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
    if (iGenericSemantic < pathDistanceInfinite)
    	return pathDistanceSemanticGeneric + iGenericSemantic ;
    else
    	return pathDistanceInfinite ;
  }

  int iPureSemantic = specificPathDistance(&VectItemTarget, &SourceVecEquivalentItem, &VectItemSource) ;
  if (iPureSemantic < pathDistanceInfinite)
  	return pathDistanceSemantic + iPureSemantic ;
  else
  	return pathDistanceInfinite ;
}

int
NSFilGuide::specificPathDistance(VecteurString* pVectItemTarget, VecteurItem* pEquivalentSource, VecteurString* pVectItemSource)
{
	//
  // Trivial cases
  //
	if (((VecteurString*) NULL == pVectItemTarget) ||
      ((VecteurItem*)   NULL == pEquivalentSource))
  	return pathDistanceInfinite ;

  if ((pVectItemTarget->empty()) && (pEquivalentSource->empty()))
  	return pathDistanceExact ;

  if ((pVectItemTarget->empty()) || (pEquivalentSource->empty()))
  	return pathDistanceInfinite ;

  // Time to work
  //
  EquiItemIter       iterTarget = pVectItemTarget->begin() ;
  EquiItemVectorIter iterVect   = pEquivalentSource->begin() ;

  if (!(testeEgaliteChemin(pVectItemTarget, iterTarget, pEquivalentSource, iterVect)))
  	return pathDistanceInfinite ;

  // A am�liorer
  //
  if ((NULL == pVectItemSource) || (pVectItemSource->empty()))
		return (pEquivalentSource->size() - pVectItemTarget->size()) ;
  else
  	return (pEquivalentSource->size() - pVectItemTarget->size()) ;
}

//---------------------------------------------------------------------------
//
// Tester l'�galit� de deux chemins (stricte ou s�mantique) item par item
//    sChaineCible     : chaine du chemin cible
//    EquivalentSource : vecteur contenant tous les �quivalents des items
//                      (du p�re + du fils)
//---------------------------------------------------------------------------
bool
NSFilGuide::testeEgaliteChemin(VecteurString* pVectItemCible, EquiItemIter IterCible,
                     VecteurItem* pEquivalentSource, EquiItemVectorIter vect)
{
	if (((VecteurString*) NULL == pVectItemCible) || (pVectItemCible->empty()))
  	return false ;
  if (((VecteurItem*) NULL == pEquivalentSource) || (pEquivalentSource->empty()))
  	return false ;

  // ----- just to ease debugging -----
  // string sCible = "" ;
  // EquiItemIter cible = pVectItemCible->begin() ;
  // for (; cible != pVectItemCible->end(); cible++)
  //  	sCible += *(*cible) + string("/") ;

  EquiItemIter IterCibleSuivant = pVectItemCible->begin() ;

  bool encore = true ;
  while ((pVectItemCible->end() != IterCible) && (pEquivalentSource->end() != vect))
  {
  	//
    // The current concept in the target (IterCible) is not a multi-concepts
    // generic concept so, we simply look if it belongs to the current semantic
    // equivalent of the current concept in the source (vect)
    //
  	if ((*(*IterCible)).find(string("*")) == string::npos)  // != de souschemin
    {
      bool bIsGeneric = (ItemGenerique == **IterCible) || (ItemPsGenerique == **IterCible) ;

      // Is the target in the vector of semantic equivalents
      //
    	if (comparerItem(**IterCible, *vect))
      {
      	vect++ ;
        IterCible++ ;

        // Take care that ItemGenerique can replace smthg like PANEV/WCE00
        //
        if (bIsGeneric && (pEquivalentSource->end() != vect))
        {
          if (false == (*vect)->empty())
          {
            string sConcept = **((*vect)->begin()) ;
            if ((strlen(sConcept.c_str()) > 3) &&
                          ((string(sConcept, 0, 3) == string("WCE")) ||
                           (string(sConcept, 0, 4) == string("WPLU"))))
              vect++ ;
          }
        }
      }
      else  //IterCible n'est pas �quivalent � son homologue dans la cha�ne source
      	return false ;
    }
    //
    // The current concept in the target (IterCible) is a multi-concepts generic concept
    //
    else
    {
    	//
      // SouscheminNonVide  : incr�menter vect
      //
      // If the current concept in the target (IterCible) is a generic concept
      // that can't be empty, we consider that it replace at least the current
      // concept in the source, so we skip it
      //
      if ((*(*IterCible)) == SousCheminNonVide)
      	vect++ ;

      IterCible++ ;

      if ((pEquivalentSource->end() == vect) && (pVectItemCible->end() != IterCible))
      	return false ;

      if (pVectItemCible->end() == IterCible)
      	return true ;

      //                   1 2 3 4 5 6 7 8 9 10 11
    	//exemple 	source = A/B/C/D/E/F/G/D/H/C/Z
      //          Cible  = */D/L/N

      //
      // Chercher les �quivalents de IterCible (D) dans source
      //

      //
      // s'il y en a plusieurs, le but est de chercher l'�galit� avec chaque
      // �quivalent de D C.A.D avec (E/F/G/D/H/C/Z, ...) et (H/C/Z, ...)
      //

      //
      // si IterCible = D alors on trouve 2 �quivalents dans source et on prend le 1�
      // (num�ro 4)
      //
      IterCibleSuivant = IterCible ;
      IterCibleSuivant++ ;

      while (encore)
      {
      	while ((pEquivalentSource->end() != vect) &&
                                !(comparerItem((*(*IterCible)),(*vect))))
        	vect++ ;

        if (pEquivalentSource->end() != vect)
        	vect++ ; //E
        else
        	return false ;

        //
        // 1er appel recursif avec *pChaineCible = E/F/G/D/H/C/Z
        // et pEquivalentSource -> vecteurs des �quivalents de E, F, G , H, C et Z
        //
        if (testeEgaliteChemin(pVectItemCible, IterCibleSuivant, pEquivalentSource, vect))
        	return true ;
      }
    }
  }

  if ((pVectItemCible->end() == IterCible) && (pEquivalentSource->end() == vect))
  	return true ;
  else
  	return false ;
}

//---------------------------------------------------------------------------
//		tester l'�galit� de deux chemins (stricte ou s�mantique)
//  		pVectItemSource 	: 	les items du chemin source
//			pCheminPere			:	chemin source
//  		sCible     			:  chaine du chemin cible
//			SousChemin			:	sous chaine
//---------------------------------------------------------------------------
void
NSFilGuide::testeEgaliteChaineCible(VecteurRechercheSelonCritere* pVecteurSelonCritere,
												int NBSeparateur, string* pCheminPere, string sCible)
{
try
{
	if (((VecteurRechercheSelonCritere*) NULL == pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
  	return ;

  IterCritere iter = pVecteurSelonCritere->begin() ;
  int TraiterCefils = 1 ; //continuer � traiter le fils en cours
  int separ ;			        //nombre de s�parateurs total dans cheminsource : pere + fils
  int nbSeparateurChaineCible = NombreSeparateurDansUneChaine(&sCible) ;

  for (; iter != pVecteurSelonCritere->end(); iter++)
  {
    if ((*iter)->trouve == false)
    {
    	string sCheminFils ;
      if (*pCheminPere == string (""))
      	sCheminFils = (*iter)->sEtiquette ;
      else
      	sCheminFils = *pCheminPere + string(1,cheminSeparationMARK) +
                                                        (*iter)->sEtiquette ;

      if (sCheminFils.find(string(1,cheminSeparationMARK)) == string::npos)
      	separ = 0 ;
      else
      	// Calculer le nombre de s�parateurs dans (*iter)->sEtiquette
        separ = NBSeparateur +
                    NombreSeparateurDansUneChaine(&(*iter)->sEtiquette) + 1 ;


      // chaine cible (non sous chemin) plus longue que la chaine source
      //
      // ItemGenerique also tested since it can replace smthg like PANEV/WCE00
      //
      if ((nbSeparateurChaineCible != separ) &&
            (sCible.find(SousChemin)        == string::npos) &&
            (sCible.find(SousCheminNonVide) == string::npos) &&
            (sCible.find(ItemGenerique)     == string::npos) &&
            (sCible.find(ItemPsGenerique)   == string::npos))
      {
      	(*iter)->trouve = false ;
        TraiterCefils = 0 ;
      }
      if (TraiterCefils)
      {
      	VecteurItem EquivalentSource ;
        // sur le p�re
        EquiItemVectorIter vect = _VecEquivalentItemDuPere.begin() ;
        for ( ; _VecEquivalentItemDuPere.end() != vect ; vect++)
        	EquivalentSource.push_back(*vect) ;
        // sur les fils
        EquiItemVectorIter vec = ((*iter)->pEquivalentTrie)->begin() ;
        for ( ; ((*iter)->pEquivalentTrie)->end() != vec ; vec++)
        	EquivalentSource.push_back(*vec) ;

        EquiItemVectorIter VectEqui = EquivalentSource.begin() ;

        VecteurString VectItemCible ;  // contient les items de sChaineCible
        DecomposeChaine(&sCible, &VectItemCible) ;
        EquiItemIter IterCible = VectItemCible.begin() ;

        if (testeEgaliteChemin(&VectItemCible, IterCible, &EquivalentSource, VectEqui))
        {
        	// Savoir si cette fiche correspond � iter
          EquiItemVectorIter equivalent = (*iter)->pEquivalentBrut->begin() ;
          for ( ; ((*iter)->pEquivalentBrut->end() != equivalent) &&
                                                ((*iter)->trouve == false) ;
                                                    equivalent++)
          {
          	EquiItemIter iterEqui = (*equivalent)->begin() ;
            string sEquivalent = *(*iterEqui) ;
            string sChemin = _pBBFiche->getSemanticPath() ;
            string sCheminFilGuide;
            size_t posit = sChemin.find(string(1,cheminSeparationMARK));
            if (string::npos == posit)
            	sCheminFilGuide = sChemin ;
            else
            {
            	size_t debut = 0 ;
              while (string::npos != posit)
              {
              	debut = posit ;
                posit = sChemin.find(string(1,cheminSeparationMARK), debut+1) ;
              }
              sCheminFilGuide = string(sChemin, debut+1, strlen(sChemin.c_str())-debut-1) ;
            }
            if (sEquivalent == sCheminFilGuide)
            {
            	if (GUIDE == _sTypeBase)
              {
              	BBFiche* pBBCommeFiche = static_cast<BBFiche*>(_pBBFiche) ;
                RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter) ;
                BBItemData* pDo = pBBCommeFiche->getData() ;
                *(pRSC->pDonnees) = *pDo ;

                string sMsg = string("Path ") + sCheminFils + string(" found as ") + pDo->getSemanticPath() + string(" (") + pDo->getGroupID() + string(" ") + pDo->getID() + string(" ") + pDo->getSonsList() + string(")") ;
                _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
              }
              else if (DECODE == _sTypeBase)
              {
              	BBDecode* pBBCommeDecode = static_cast<BBDecode*>(_pBBFiche) ;
                RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iter) ;
                BBDecodeData* pDo = pBBCommeDecode->getData() ;
                *(pRSC->pDonnees) = *pDo ;
              }
              //*((*iter)->pDonnees) = *(pBBFiche->pDonnees);
              (*iter)->trouve = true ;
            }
          }
          if (false == (*iter)->trouve)
          {
          	if (GUIDE == _sTypeBase)
            {
            	/*RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter);
                BBItemData* pDo = static_cast<BBItemData*>(pRSC->pDonnees);
                pRSC->pVecteurData->push_back(new BBItemData(*pDo));*/

              BBFiche* pBBCommeFiche = static_cast<BBFiche*>(_pBBFiche) ;
              RechercheSelonCritereData* pRSC = static_cast<RechercheSelonCritereData*>(*iter) ;
              BBItemData* pDo = pBBCommeFiche->getData() ;
              pRSC->pVecteurData->push_back(new BBItemData(*pDo)) ;

              string sMsg = string("Path ") + sCheminFils + string(" found as ") + pDo->getSemanticPath() + string(" (") + pDo->getGroupID() + string(" ") + pDo->getID() + string(" ") + pDo->getSonsList() + string(")") ;
              _pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;
            }
            else if (DECODE == _sTypeBase)
            {
            	BBDecode* pBBCommeDecode = static_cast<BBDecode*>(_pBBFiche);
              RechercheSelonCritereDecode* pRSC = static_cast<RechercheSelonCritereDecode*>(*iter);
              BBDecodeData* pDo = pBBCommeDecode->getData() ;
              pRSC->pVecteurData->push_back(new BBDecodeData(*pDo));
            }
          }
          //if((*iter)->trouve == false)
          //  (*iter)->pVecteurData->push_back(new BBItemData(*(pBBFiche->pDonnees)));
        }

        // comme le vecteur ne contient que des copies de pointeurs
        // il ne faut pas le "deleter" avant de l'avoir "vid�" � la main
        EquiItemVectorIter i = EquivalentSource.begin() ;
        while (EquivalentSource.end() != i)
        	EquivalentSource.erase(i) ;
      }
      TraiterCefils = 1 ;
    }
	}
}
catch (...)
{
	erreur("Exception NSFilGuide::testeEgaliteChaineCible", standardError) ;
  return ;
}
}

//---------------------------------------------------------------------------
// 								Recherche s�mantique
//---------------------------------------------------------------------------
void
NSFilGuide::chercheCheminReseau(string* pCheminPere,
						VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
#ifndef _ENTERPRISE_DLL
try
{
	if (((string*) NULL == pCheminPere) ||
               ((VecteurRechercheSelonCritere*) NULL == pVecteurSelonCritere))
		return ;
	if ((string("") == *pCheminPere) && (pVecteurSelonCritere->empty()))
		return ;

  IterCritere iter ;

  string sMsg = string("Entering chercheCheminReseau for path ") + *pCheminPere + string(" and sons") ;
  for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
    sMsg += string(" ") + (*iter)->sEtiquette ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

	bool bLimitSemanticSearch = false ;

	// Recherche exacte
	chercheCheminEgaliteStricte(pCheminPere, pVecteurSelonCritere) ;

	// autres fils � traiter
	if (false == pVecteurSelonCritere->FilsAtraiter())
  {
    string sMsg = string("Leaving chercheCheminReseau for path ") + *pCheminPere + string(" since all sons have been found from non semantic search.") ;
    _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;
		return ;
  }

  sMsg = string("Keeping on chercheCheminReseau for path ") + *pCheminPere + string(" since some sons have not been found from non semantic search:") ;
  for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
    if (false == (*iter)->trouve)
      sMsg += string(" ") + (*iter)->sEtiquette ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

	string      ch ;
	int         NombreElementEnCommun ;  // nombre d'items en commun entre les deux chemins
   													           // au sens r�seau s�mantique
	string      sCible ; //chemin en cours dans la BD

	_pBBFiche->lastError = _pBBFiche->chercheClef(pCheminPere,
                                                "CHEMIN",
                                                NODEFAULTINDEX,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;
	if (_pBBFiche->lastError != DBIERR_NONE)
	{
  	_pBBFiche->lastError = _pBBFiche->fin(dbiWRITELOCK) ;
    if (_pBBFiche->lastError != DBIERR_NONE)
    {
    	for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
      	(*iter)->trouve = false ;
      erreur("Le fichier des guides semble vide.", standardError, _pBBFiche->lastError) ;
      return ;
    }
	}
	_pBBFiche->lastError = _pBBFiche->getRecord() ;
	if (_pBBFiche->lastError != DBIERR_NONE)
	{
		erreur("Le fichier nsguides.db inutilisable.", standardError, _pBBFiche->lastError);
    for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
    	(*iter)->trouve = false ;
    return ;
	}
  //
  //limite pour la mont�e : 	� partir d'une cible on proc�de � la mont�e,
  //                         on calcule la nouvelle cha�ne sNouvelleChiane et on fait
  //									la descente jusqu'� sLimiteMonter
  //exemple	:
  //
  //
  //
  //                    sNouvelleChaine
  //                           .	^
  //                           .   ^   mont�e
  //                           .   ^
  //				  sLimiteMonter <- sFutureLimiteMonter
  //				sNouvelleChiane <- sFutureLimiteMonter
  //									  .   ^
  //                           .   ^   mont�e
  //                           .   ^
  //							cha�ne cible A/B/C/D/E
  //
  //
	string sLimiteMonter ;
	string sFutureLimiteMonter = _pBBFiche->getSemanticPath() ;

	// Nombre de s�parateurs dans pCheminPere
	int NBSeparateur = NombreSeparateurDansUneChaine(pCheminPere) ;

	// Initialisation
	_VecEquivalentItemDuPere.vider() ;

	// pVectItemPere est un vecteur qui contient les items de pCheminPere
  VecteurString VectItemPere ;
  DecomposeChaine(pCheminPere, &VectItemPere) ;
  EquiItemIter iterPere = VectItemPere.begin() ;

	// Nombre maximal d'items communs � pCheminPere et la cha�ne cible
	VecteurString VectItemCible ;  // contient les items de pCheminPere
  string sSemPath = _pBBFiche->getSemanticPath() ;
	DecomposeChaine(&sSemPath, &VectItemCible) ;
	NombreElementEnCommun = VectItemPere.NbElementCommun(&VectItemCible) ;

	int nbElement = NombreElementEnCommun ;
	int cont      = 1 ;
	// Construire le vecteur contenant tous les �quivalents des items du chemin P�re
	for (iterPere = VectItemPere.begin() ; VectItemPere.end() != iterPere ; iterPere++)
	{
  	VecteurString EquivalentItem ; //les �quivalents au sens r�seau d'un item donn�

    // En cas de limite s�mantique, on ne prend les �quivalents s�mantiques
    // qu'au del� de NombreElementEnCommun
    //
    if ((false == bLimitSemanticSearch) || (cont > NombreElementEnCommun))
    	chercheEquivalent(*(*iterPere), &EquivalentItem, string("ES")) ; // constituer les �quivalents au sens r�seau de (*iter)
    else
    	EquivalentItem.AddString(**iterPere) ;

    sort(EquivalentItem.begin(), EquivalentItem.end(), inferieure) ;
    _VecEquivalentItemDuPere.push_back(new VecteurString(EquivalentItem)) ;

    cont++ ;
	}

	// constituer les �quivalents au sens r�seau des fils
	for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
	{
		if (false == (*iter)->trouve)
    {
    	VecteurString VectItemFils ;  // contient les items de pCheminPere
      DecomposeChaine(&(*iter)->sEtiquette, &VectItemFils) ;
      EquiItemIter iterFils = VectItemFils.begin() ;
      for ( ; VectItemFils.end() != iterFils ; iterFils++)
      {
      	VecteurString EquivalentItem ; //les �quivalents au sens r�seau d'un item donn�
        chercheEquivalent(*(*iterFils), &EquivalentItem, string("ES")) ; // constituer les �quivalents au sens r�seau de (*iter)
        (*iter)->pEquivalentBrut->push_back(new VecteurString(EquivalentItem)) ;
        sort(EquivalentItem.begin(), EquivalentItem.end(), inferieure) ;
        (*iter)->pEquivalentTrie->push_back(new VecteurString(EquivalentItem)) ;
      }
    }
  }

	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                            pCheminPere, _pBBFiche->getSemanticPath()) ;

	// Autres fils � traiter ?
	if (false == pVecteurSelonCritere->FilsAtraiter())
		return ;

	//
	// Initialisation
	//
	nbElement = NombreElementEnCommun ;
	bool result ;
	string sNouvelleCible = string("") ;

  sMsg = string("For path ") + *pCheminPere + string(" starting from ") + _pBBFiche->getSemanticPath() + string(" (") + _pBBFiche->getGroupID() + string(" ") + _pBBFiche->getID() + string(")") ;
  _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

	//
	// Cr�ation de 2 bookMark
	//
	_pBBFiche->creerBookMarks(2) ;
	bool descendre, monter ;
	_pBBFiche->lastError = _pBBFiche->placeBookMark(2) ;
	if (_pBBFiche->lastError == DBIERR_NONE)
  	monter = true ;
	else
		monter = false ;
	//   pBBFiche->lastError = pBBFiche->suivant(dbiWRITELOCK);
	_pBBFiche->lastError = _pBBFiche->placeBookMark(1) ;
	if (_pBBFiche->lastError == DBIERR_NONE)
  	descendre = true ;
	else
		descendre = false ;
	//
	// au moins le premier �l�ment ( � gauche ) en commun
	//
	while ((NombreElementEnCommun >= 0) &&
           (_pBBFiche->lastError == DBIERR_NONE) && (monter || descendre))
	{
		//
    // Commencer par la mont�e pour ne pas tomber sur ??? avant d'avoir
    // scann� tous les �quivalents s�mantiques
    //
		if (monter)
    {
    	//
      // Initialiser sLimiteMonter � la cha�ne limite (sFutureLimiteMonter)
      // � ne pas depasser lors de la descente
      //
			sLimiteMonter = sFutureLimiteMonter ;
      string sNouvelleCible = NouvelleCible(&VectItemPere,
                                            // bLimitSemanticSearch ? NombreElementEnCommun : 0,
                                            NombreElementEnCommun,
                                            pVecteurSelonCritere) ;

      _pBBFiche->lastError = _pBBFiche->chercheClef(&sNouvelleCible,
                                                    "CHEMIN",
                                                    NODEFAULTINDEX,
                                                    keySEARCHGEQ,
                                                    dbiWRITELOCK) ;
			if (DBIERR_NONE == _pBBFiche->lastError)
      {
      	_pBBFiche->lastError = _pBBFiche->getRecord() ;
        if (_pBBFiche->lastError != DBIERR_NONE)
        {
        	erreur("Le fichier de script est inutilisable (guides.db).", standardError, _pBBFiche->lastError) ;
          _pBBFiche->detruireBookMarks(2) ;
          return ;
        }

        sMsg = string("Going up -> ") + _pBBFiche->getSemanticPath() + string(" (") + _pBBFiche->getGroupID() + string(" ") + _pBBFiche->getID() + string(")") ;
        _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

        sCible = _pBBFiche->getSemanticPath() ;
        VecteurString VectItemCible ;  // contient les items de pCheminPere
        DecomposeChaine(&sCible, &VectItemCible) ;
        nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        sFutureLimiteMonter = _pBBFiche->getSemanticPath() ;
      }

      while ((sCible < sLimiteMonter) &&
                   (nbElement >= NombreElementEnCommun) &&
                   (_pBBFiche->lastError == DBIERR_NONE))
      {
      	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                                                        pCheminPere, sCible) ;
      	//
        // Autres fils � traiter ?
        //
        if (false == pVecteurSelonCritere->FilsAtraiter())
        {
        	_pBBFiche->detruireBookMarks(2) ;
          return ;
        }

        result = RechercheFicheSuivante(&VectItemPere, sCible, pVecteurSelonCritere,
                                            NombreElementEnCommun) ;
        if (result)
        {
        	sCible = _pBBFiche->getSemanticPath() ;
          VecteurString VectItemCible ;  // contient les items de pCheminPere
          DecomposeChaine(&sCible, &VectItemCible) ;
          nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        }
        // On n'arrive pas � imaginer un nouveau chemin
        //
        else
        	sCible = sLimiteMonter ;
      }

      if (_pBBFiche->lastError != DBIERR_NONE)
      	monter = false ;
    }
    if (descendre)
    {
    	_pBBFiche->lastError = _pBBFiche->retrouveBookMark(1) ;
      if (DBIERR_NONE == _pBBFiche->lastError)
      {
      	_pBBFiche->lastError = _pBBFiche->getRecord() ;
        sCible = _pBBFiche->getSemanticPath() ;
        VecteurString VectItemCible ;  // contient les items de pCheminPere
        DecomposeChaine(&sCible, &VectItemCible) ;
        nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
      }
      result = true ;
      while ((result) && (nbElement >= NombreElementEnCommun) && (_pBBFiche->lastError == DBIERR_NONE))
      {
      	testeEgaliteChaineCible(pVecteurSelonCritere, NBSeparateur,
                                                        pCheminPere, sCible) ;
        //
        // Autres fils � traiter ?
        //
        if (false == pVecteurSelonCritere->FilsAtraiter())
        {
        	_pBBFiche->detruireBookMarks(2) ;
          return ;
        }
        result = RechercheFicheSuivante(&VectItemPere, sCible, pVecteurSelonCritere,
                                            NombreElementEnCommun);
        if (result)
        {
          sMsg = string("Going down -> ") + _pBBFiche->getSemanticPath() + string(" (") + _pBBFiche->getGroupID() + string(" ") + _pBBFiche->getID() + string(")") ;
          _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;

        	sCible = _pBBFiche->getSemanticPath() ;
          VecteurString VectItemCible ;  // contient les items de pCheminPere
          DecomposeChaine(&sCible, &VectItemCible) ;
          nbElement = VectItemPere.NbElementCommun(&VectItemCible) ;
        }
      }
      if (DBIERR_NONE == _pBBFiche->lastError)
      	_pBBFiche->lastError = _pBBFiche->placeBookMark(1) ;

      if (DBIERR_NONE != _pBBFiche->lastError)
      	descendre = false ;
    }
    ChoisirMeilleureProposition(pVecteurSelonCritere) ;
    //
    // Don't do this because changing pVecEquivalentItemDuPere would give weird
    // results when calling testeEgaliteChaineCible (since the initial element
    // is first to be removed, exact equality no longer works)
    //
    // MettreAjourEquivalent(NombreElementEnCommun) ;
    //
    NombreElementEnCommun-- ;
	}
	_pBBFiche->detruireBookMarks(2) ;

  sMsg = string("Leaving chercheCheminReseau for path ") + *pCheminPere ;
  if (pVecteurSelonCritere->FilsAtraiter())
  {
    sMsg += string(" some son(s) remain unfound:") ;
    for (iter = pVecteurSelonCritere->begin() ; pVecteurSelonCritere->end() != iter ; iter++)
      if (false == (*iter)->trouve)
        sMsg += string(" ") + (*iter)->sEtiquette ;
    _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;
  }
  else
    _pSuper->trace(&sMsg, 1, NSSuper::trDebug) ;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheCheminReseau", standardError, 0) ;
}
#endif
}

NSFilGuideEngine::NSFilGuideEngine(NSSuper* pSuper)
                 :NSSuperRoot(pSuper)
{
	_bCherchePivot      = true ;
  _iRangPivotPere     = 0 ;
	_iRangPivotCible    = 0 ;
  _iNbCommonElements  = 0 ;
	_sCibleAudelaDuPere = string("") ;
  _sDernierItemPere   = string("") ;

	_sPivot             = string("") ;
  _sChaineFils        = string("") ;
	_sPrePivot          = string("") ;

  _sNouvelleCible     = string("") ;

  _pVecteurSelonCritere     = 0 ;
  _pVecEquivalentItemDuPere = 0 ;
  _itEquivalentItemDuPere   = (EquiItemVectorIter) 0 ;
  _pVectItemPere            = 0 ;
  _iterPere                 = (EquiItemIter) 0 ;
}

NSFilGuideEngine::~NSFilGuideEngine()
{
}

void
NSFilGuideEngine::initTargetVector(string sTargetString)
{
	DecomposeChaine(&sTargetString, &_VectItemCible) ;
  _IterCible = _VectItemCible.begin() ;
}

void
NSFilGuideEngine::initVectItemPere(VecteurString* pVectPere)
{
	_pVectItemPere = pVectPere ;

  _sDernierItemPere = _pVectItemPere->GetLast() ;
  _iterPere         = _pVectItemPere->begin() ;
}

void
NSFilGuideEngine::setEquiItemPere(VecteurItem* pEqItemPere)
{
	_pVecEquivalentItemDuPere = pEqItemPere ;

  if (_pVecEquivalentItemDuPere && (false == _pVecEquivalentItemDuPere->empty()))
		_itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;
	else
    _itEquivalentItemDuPere = (EquiItemVectorIter) 0 ;
}

// -----------------------------------------------------------------------------
// Egalit� au sens r�seau
// -----------------------------------------------------------------------------
// D�composer pChaine et mettre ses items dans le vecteur pVect
// -----------------------------------------------------------------------------
void
NSFilGuideEngine::DecomposeChaine(const string* pChaine, VecteurString* pVect)
{
	if (((string*) NULL == pChaine) || ((VecteurString*) NULL == pVect))
		return ;

  size_t iPosit = pChaine->find(string(1, cheminSeparationMARK)) ;

  if (string::npos == iPosit)
  {
  	pVect->AddString(*pChaine) ;
    return ;
  }

  size_t iStart = 0 ;
  size_t iLen   = strlen(pChaine->c_str()) ;

  while ((string::npos != iPosit) && (iPosit < iLen))
  {
    pVect->AddString(string(*pChaine, iStart, iPosit - iStart)) ;
    iStart = iPosit + 1 ;
    iPosit = pChaine->find(string(1, cheminSeparationMARK), iStart + 1) ;
  }

  pVect->AddString(string(*pChaine, iStart, iLen - iStart)) ;
}

//
// Chercher les pivots et leurs rangs dans sCible et dans la cha�ne du p�re
//
void
NSFilGuideEngine::getPivotsAndRanks()
{
	if ((NULL == _pVectItemPere) || (NULL == _pVecEquivalentItemDuPere))
		return ;

	EquiItemIter       PereItereTemp              = _pVectItemPere->begin() ;
  EquiItemVectorIter itEquivalentItemDuPereTemp = _pVecEquivalentItemDuPere->begin() ;

  _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;

	bool avance = true ;

	while ((avance) &&
           (_VectItemCible.end() != _IterCible) &&
           (_pVectItemPere->end() != _iterPere) &&
           (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere))
	{
		if ((*(*_IterCible)).find(string("*")) == string::npos)  // != de souschemin
		{
    	// Si �quivalence s�mantique avancer, sinon sortir
      if ((*_itEquivalentItemDuPere)->ItemDansUnVecteur((*(*_IterCible))))
      {
      	// _IndiceSource.push_back(_iRangPivotPere);
        if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
        	_itEquivalentItemDuPere++ ;
        if (_VectItemCible.end() != _IterCible)
        	_IterCible++ ;
        if (_pVectItemPere->end() != _iterPere)
        	_iterPere++ ;
        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
      }
      else
      	avance = false ;
    }
    else
    {
    	//
      // SouscheminNonVide : �quivalent � l'item du p�re en cours
      //
      if (SousCheminNonVide == (*(*_IterCible)))
      {
      	// Avancer d'un cran chez le p�re
        if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
        	_itEquivalentItemDuPere++ ;
        if (_pVectItemPere->end() != _iterPere)
        	_iterPere++ ;

        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
      }
      _IterCible++ ;
      //
      // Commencer � comparer avec _IterCible en commen�ant _iterPere
      //
      // Avancer tant que pas d'�quivalence s�mantique avec _IterCible
      if (_VectItemCible.end() != _IterCible)
      {
      	// Garder le dernier pivot trouv� chez le p�re
        PereItereTemp = _iterPere ;
        itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
        while ((_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere) &&
               (_pVectItemPere->end() != _iterPere) &&
               (false == (*_itEquivalentItemDuPere)->ItemDansUnVecteur((*(*_IterCible)))))
        {
        	if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
          	_itEquivalentItemDuPere++ ;
          if (_pVectItemPere->end() != _iterPere)
          	_iterPere++ ;
        }
        // � la fin de la cha�ne p�re
        if (_pVectItemPere->end() == _iterPere)
        	//_IndiceSource.push_back(0);
        	avance = false ;
        else
        {
        	_iterPere++ ;
          _itEquivalentItemDuPere++ ;
          PereItereTemp = _iterPere ;
          itEquivalentItemDuPereTemp = _itEquivalentItemDuPere ;
          //_IndiceSource.push_back(_iRangPivotPere);
          if (_IterCible != _VectItemCible.end())
          	_IterCible++ ;
        }
      }
    }
	}

  _iterPere = PereItereTemp ;
	_itEquivalentItemDuPere = itEquivalentItemDuPereTemp ;
}

void
NSFilGuideEngine::getRanks()
{
	EquiItemIter it   = _pVectItemPere->begin() ;
	EquiItemIter iter = _VectItemCible.begin() ;

	if (_iterPere == _pVectItemPere->end())
  	_iRangPivotPere = _pVectItemPere->size() + 1 ;
  else
  	for (_iRangPivotPere = 1;
                (it != _pVectItemPere->end()) && (it != _iterPere);
                            it++, _iRangPivotPere++) ;

	if (_IterCible == _VectItemCible.end())
  	_iRangPivotCible = _VectItemCible.size() + 1 ;
	else
		for (_iRangPivotCible = 1;
                (iter != _VectItemCible.end()) && (iter != _IterCible);
                           iter++, _iRangPivotCible++) ;
}

//
// vecteur des indices des items de la source ayant des �quivalents dans sCible
// mettre 0 si pas d'�quivalence
// exemple : source : A/B/C/D/E/F/G/H
//           sCible : A/J/*/L/O
//				J - is a -> B
//				L - is a -> F
//				donne : IndiceSource = [1,2,0,6,0]
void
NSFilGuideEngine::getPivotsVector()
{
	EquiItemIter iter = _VectItemCible.begin() ;

	size_t i ;
	bool continuer = true ;
	for (i = 1; (iter != _VectItemCible.end()) && (continuer); i++, iter++)
	{
		if (iter != _IterCible)
		{
    	if ((*(*iter)).find(string("*")) != string::npos)
      	_IndiceSource.push_back(0) ;
      else
      	_IndiceSource.push_back(i) ;
    }
    else
    {
    	if (_VectItemCible.end() != _IterCible)
      	_IndiceSource.push_back(_iRangPivotPere) ;
      continuer = false ;
    }
	}
	// Remplir _IndiceSource avec des 0
	for (i = _iRangPivotCible + 1; i <= _VectItemCible.size(); i++)
		_IndiceSource.push_back(0) ;

	if (_iRangPivotCible)
		_bCherchePivot = true ;
}

void
NSFilGuideEngine::getTargetBeyondFather()
{
	_sCibleAudelaDuPere = "" ;

	if (_iterPere == _pVectItemPere->end())
	{
		EquiItemIter iter = _VectItemCible.begin() ;
    iter = _IterCible ;

    if (_VectItemCible.end() != iter)
    {
    	_sCibleAudelaDuPere = *(*iter) ;
      iter++ ;
    }
		while (_VectItemCible.end() != iter)
    {
    	_sCibleAudelaDuPere += "/" + *(*iter) ;
      iter++ ;
    }
	}
}

bool
NSFilGuideEngine::findPivot()
{
	while ((_iRangPivotPere) && (_bCherchePivot))
	{
		//
    // On cherche le pivot au niveau du p�re
    //
    if ((_iRangPivotPere) && (_iterPere != _pVectItemPere->end()))
    {
    	if (_VectItemCible.end() != _IterCible)
      {
      	if (_VectItemCible.begin() != _IterCible)
        {
        	_IterCible-- ;
          _sPrePivot = *(*_IterCible) ;
          _IterCible++ ;
        }
        //
        // ATTENTION : si l'�l�ment ne suit pas un ~~** on cherche
        // l'�l�ment strictement sup�rieur pour le rang en cours.
        //
				if (_sPrePivot.find(string("*")) == string::npos)
        {
        	_sPivot = (*_itEquivalentItemDuPere)->ItemStrictementSuperieur(*(*_IterCible)) ;

          // In order not to start an infinite loop, refuse the exact concept
          //
          if ((_iNbCommonElements == _iRangPivotPere) && (_sPivot == *(*_iterPere)))
          	_sPivot = (*_itEquivalentItemDuPere)->ItemStrictementSuperieur(*(*_iterPere)) ;

          if (_sPivot != "")
          	_bCherchePivot = false ;
        }
        //
        // Si l'�l�ment suit un ~~** (qui peut remplacer plusieurs items),
        // on cherche le plus petit des �l�ments strictement sup�rieurs
        // pour tous les �l�ments situ�s apr�s
        //
        else
        {
        	EquiItemVectorIter Tempvect = _pVecEquivalentItemDuPere->begin() ;
          Tempvect = _itEquivalentItemDuPere ;

          size_t  rangSup = 0 ;
          size_t  rang    = _iRangPivotPere ;
          string  sSup    = "~~~~~~~~~~~~~" ;
          string sPivotSupLocal ;
          //
          // On �num�re tous les �l�ments >= _iRangPivotPere pour
          // trouver celui qui poss�de le plus petit des �quivalents
          // d'_IterCible
          //
          while (rang <= _pVectItemPere->size())
          {
          	sPivotSupLocal = (*Tempvect)->ItemStrictementSuperieur(*(*_IterCible)) ;
            if ((sPivotSupLocal != "") && (sPivotSupLocal < sSup))
            {
            	char chFirst = sPivotSupLocal[0] ;
              char chLast  = sPivotSupLocal[strlen(sPivotSupLocal.c_str())-1] ;

              if ((chFirst != '~') ||
                  ((chLast >= '0') && (chLast <= '9')) ||
                  ((chLast >= 'A') && (chLast <= 'Z')))
              {
              	sSup = sPivotSupLocal ;
                rangSup = rang ;
              }
            }
            if (rang < _pVectItemPere->size())
            	Tempvect++ ;
            rang++ ;
          }
          //
          // m�me recherche chez les fils
          //
          VecteurString VectTemp ;  // contient les items de sChaineCible
					DecomposeChaine((*_IterCible), &VectTemp) ;
          sPivotSupLocal = _pVecteurSelonCritere->ChaineStrictementSuperieur(&VectTemp, SousChemin) ;
          //
          // Le pivot est chez les fils
          //
          if ((string("") != sPivotSupLocal) && (sPivotSupLocal < sSup))
          {
          	char chFirst = sPivotSupLocal[0] ;
            char chLast  = sPivotSupLocal[strlen(sPivotSupLocal.c_str())-1] ;

            if ((chFirst != '~') ||
                ((chLast >= '0') && (chLast <= '9')) ||
                ((chLast >= 'A') && (chLast <= 'Z')))
            {
            	_iRangPivotPere = _pVectItemPere->size() + 1 ;
              _iterPere       = _pVectItemPere->end() ;
              _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->end() ;
              _sChaineFils = sPivotSupLocal ;
              _bCherchePivot = false ;
            }
          }
          //
          // Le pivot est chez le p�re
          //
          else if (sSup != "~~~~~~~~~~~~~")
          {
          	_sPivot = sSup ;
            _bCherchePivot = false ;
            while (_iRangPivotPere < rangSup)
            {
            	_itEquivalentItemDuPere++ ;
              _iRangPivotPere++ ;
              _iterPere++ ;
            }
          }
        }
        //
        // Si on n'a pas trouv� de pivot, on recule d'un cran
        //
        if ((_sPivot == "") && (_bCherchePivot))
        {
        	_iRangPivotCible-- ;
          if (_VectItemCible.begin() != _IterCible)
          	_IterCible-- ;
          else
            return false ;

          //
          // trouver la position de (_iterPere,_iRangPivotPere)
          // correspondant � (_IterCible, _iRangPivotCible)
          //

          indiceIterator Indice = _IndiceSource.begin() ;
          // trouver _iRangPivotPere = _IndiceSource[_iRangPivotCible]
          for (size_t iCible = 1; (Indice != _IndiceSource.end()) &&
                            (iCible != _iRangPivotCible) ; Indice++, iCible++) ;

          if (Indice != _IndiceSource.end())
          {
          	_iRangPivotPere = *Indice ;

            // si on tombe sur 0, il faut reculer encore d'un cran
            while (*Indice == 0)
            {
            	if (Indice != _IndiceSource.begin())
              {
              	Indice-- ;
                _iRangPivotCible-- ;
                _IterCible-- ;
                _iRangPivotPere = *Indice ;
              }
              else
                return false ;
            }

            _itEquivalentItemDuPere = _pVecEquivalentItemDuPere->begin() ;
            _iterPere = _pVectItemPere->begin() ;
            size_t con ;
            // trouver _iterPere correspondant � _iRangPivotPere
            for (con = 1; (_iterPere != _pVectItemPere->end()) && (con != _iRangPivotPere)
                           && (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere);
                           _iterPere++, _itEquivalentItemDuPere++, con++) ;
          }

                    /*iterator Indice = _IndiceSource.begin();
                    // trouver RangPivotPere
                    for (RangPivotPere = 1; (Indice != _IndiceSource.end()) &&
                        (*Indice != _iRangPivotCible) ; Indice++, RangPivotPere++);

                    if (Indice != _IndiceSource.end())
                    {
                        // si on tombe sur 0, il faut reculer encore d'un cran
                        if (*Indice == 0)
                        {
                            if (Indice != _IndiceSource.begin())
                            {
               		            Indice--;
                  	            RangPivotCible--;
               		            IterCible--;
                            }
                            else
                            {
                  	            delete pVectItemCible;
               		            return false;
                            }
                        }

                        _itEquivalentItemDuPere = pVecEquivalentItemDuPere->begin();
               	        _iterPere = _pVectItemPere->begin();
                        int con;
                        // trouver _iterPere correspondant � RangPivotPere
                        for (con = 1; (_iterPere != _pVectItemPere->end()) && (con != RangPivotPere)
                           && (_itEquivalentItemDuPere != pVecEquivalentItemDuPere->end());
                           _iterPere++, _itEquivalentItemDuPere++, con++);
                    } */
        	if (_iRangPivotPere <= _iNbCommonElements)
            return false ;
        }
      }
      //
      // On est dans le p�re mais au del� de la cible : prendre le min des
      // �quivalents du pivot chez le p�re(_iterPere)
      //
      else
      {
      	_sPivot 		 =  *(*(*_itEquivalentItemDuPere)->begin()) ;
        _bCherchePivot = false ;
      }
    }
    else
    {
      // traiter les fils
      // chaine cible
      VecteurString VectItemChaine ;  // contient les items de sChaineCible
      DecomposeChaine(&_sCibleAudelaDuPere, &VectItemChaine) ;
      _sChaineFils = _pVecteurSelonCritere->ChaineStrictementSuperieur(&VectItemChaine, _sDernierItemPere) ;
      if (_sChaineFils != "")
        _bCherchePivot = false ;
      else
      {
      	// passer au p�re
        _iRangPivotPere-- ;
        if (_iterPere != _pVectItemPere->begin())
        {
          _iterPere-- ;
          _itEquivalentItemDuPere-- ;
        }
        else
          return false ;
      }
    }
  }
  return true ;
}

void
NSFilGuideEngine::buildNewTargetString()
{
	_sNouvelleCible = string("") ;

	EquiItemIter iter = _VectItemCible.begin() ;

	//
	// Si le pivot est au niveau du p�re
	//
	if (_sChaineFils == "")
	{
    // on recopie tous les �l�ments de la cible situ�s avant le pivotCible
    //
    for (size_t cont = 1; (_VectItemCible.end() != iter) && (cont < _iRangPivotCible); iter++, cont++)
    	_sNouvelleCible += *(*iter) + string(1,cheminSeparationMARK) ;
		//
    // On ajoute le pivot
    //
    _sNouvelleCible += _sPivot ;
    //
    // On compl�te par les min des �l�ments au dela de pivotPere
    //
    // Rappel : _iterPere et _itEquivalentItemDuPere sont positionn�s
    // 			sur le rang du pivoPere
    //
    size_t rangComplement = _iRangPivotPere ;
    //
    if (_sPivot.find(string("*")) != string::npos)
  	{
    	//
    	// le pivot est un ~~**
    	// On trouve le min des �quivalents s�mantiques situ�s au del� du pivot
    	// et on concat�ne le min des �l�ments situ�s au del�
    	// calcul du Rang du min des mins
    	//
    	EquiItemVectorIter Tempvect = _pVecEquivalentItemDuPere->begin() ;
    	size_t rangMin = 0 ;
    	size_t rang 	 = 1 ;
    	string sMin    = "~~~~~~~~~~~~~" ;
    	string sMinLocal ;
    	while (rang <= _pVectItemPere->size())
    	{
        if (rang > _iRangPivotPere)
        {
        	sMinLocal = *(*((*Tempvect)->begin())) ;
        	if (sMinLocal < sMin)
        	{
        		sMin = sMinLocal ;
        		rangMin = rang ;
        	}
    		}
    		if (rang < _pVectItemPere->size())
    			Tempvect++ ;
    		rang++ ;
    	}
    	sMinLocal = _pVecteurSelonCritere->BorneInf() ;
    	if (sMinLocal < sMin)
      	rangMin = _pVectItemPere->size() + 1 ;

    	while (rangComplement < rangMin-1)
    	{
        _itEquivalentItemDuPere++ ;
        rangComplement++ ;
    		_iterPere++ ;
    	}
    }

    while (rangComplement < _pVectItemPere->size())
    {
    	_itEquivalentItemDuPere++ ;
      _iterPere++ ;
    	rangComplement++ ;
    	if (_pVecEquivalentItemDuPere->end() != _itEquivalentItemDuPere)
      	_sNouvelleCible += string(1,cheminSeparationMARK) + *(*(*_itEquivalentItemDuPere)->begin()) ;
    }
    string sBI = _pVecteurSelonCritere->BorneInf() ;
  	_sNouvelleCible += string(1,cheminSeparationMARK) + sBI ;
	}
  //
  // Si le pivot est chez les fils
  //
	else
	{
    for (size_t cont = 1; (_VectItemCible.end() != iter)
                                && (cont < _iRangPivotCible); iter++, cont++)
    	_sNouvelleCible += *(*iter) + string(1,cheminSeparationMARK) ;

		_sNouvelleCible += _sChaineFils ;
	}
}

//-----------------------------------------------------------------------------------
// 						Trouver la fiche suivante � traiter
//								( au sens s�mantique) entre  deux chaines
//							pVectItemPere : Vecteurdes items du p�re
//                    				sCible : chaine cible
//						pNouvelleCible : nouvelle chaine cible calcul�e
//
//-----------------------------------------------------------------------------------
bool
NSFilGuide::RechercheFicheSuivante(VecteurString* pVectItemPere, string sCible,
						VecteurRechercheSelonCritere* pVecteurSelonCritere, int NombreElementEnCommun)
{
#ifndef _ENTERPRISE_DLL
try
{
	if ((NULL == pVectItemPere) || (true == pVectItemPere->empty()))
		return false ;

	NSFilGuideEngine FGE(_pSuper) ;
  FGE.setEquiItemPere(&_VecEquivalentItemDuPere) ;
  FGE.initTargetVector(sCible) ;
  FGE.initVectItemPere(pVectItemPere) ;
  FGE.setVectSelonCriteres(pVecteurSelonCritere) ;
  FGE.setNbCommonElements(NombreElementEnCommun) ;

	string sPere = "" ;
	EquiItemIter pere = pVectItemPere->begin() ;
	for (; pere != pVectItemPere->end(); pere++)
		sPere += *(*pere) + string("/") ;

  //
  // vecteur des indices des items de la source ayant des �quivalents dans sCible
  // mettre 0 si pas d'�quivalence
  // exemple : source : A/B/C/D/E/F/G/H
  //           sCible : A/J/*/L/O
  //				J - is a -> B
  //				L - is a -> F
  //				donne : IndiceSource = [1,2,0,6,0]

	//
	// Chercher les pivots et leurs rangs dans sCible et dans la cha�ne du p�re
	//
  FGE.getPivotsAndRanks() ;

	//
	// Determiner les rangs des pivots trouv�s
	//
  FGE.getRanks() ;

	//
	// Mettre � jour le vecteur des indices
	//
  FGE.getPivotsVector() ;

	//
	// _sCibleAudelaDuPere n'est utilis�e que dans le cas o� _iterPere est audel�
	// de sPere -> le pivot est dans les fils
	//
  FGE.getTargetBeyondFather() ;

	//
  // Recherche du pivot
  //
  if (false == FGE.findPivot())
  	return false ;

	//
	// Fabrication de sNouvelleCible
	//
	FGE.buildNewTargetString() ;

  string sNouvelleCible = FGE.getNewTarget() ;

  //
  // Si la nouvelle cible form�e est <= � sCible alors c'est qu'on
  // n'est pas capable de trouver une cible plus grande donc sortir
	//
	if (!(sCible < sNouvelleCible))
		return false ;

	_pBBFiche->lastError = _pBBFiche->chercheClef(&sNouvelleCible,
                                                "CHEMIN",
                                                NODEFAULTINDEX,
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;
	if (_pBBFiche->lastError == DBIERR_NONE)
	{
    _pBBFiche->lastError = _pBBFiche->getRecord() ;
    if (_pBBFiche->lastError != DBIERR_NONE)
  	{
    	erreur("Le fichier de script est inutilisable (guides.db).", standardError, _pBBFiche->lastError) ;
    	return false ;
		}
	}
	else
		return false ;

	return true ;
}
catch (...)
{
	erreur("Exception NSFilGuide::RechercheFicheSuivante", standardError, 0) ;
	return false ;
}
#else
  return false ;
#endif
}

//----------------------------------------------------------------------------
//  						Trouver la fiche suivante � traiter
// 			  		NombreElementEnCommun le nombre d'items communs
//								( au sens exact) entre  deux chaines
//                   	 pVectItemPere : items du p�re
//       Retour : sCile(0, NombreElementEnCommun) + min(sur le p�re et les fils)
//----------------------------------------------------------------------------
string
NSFilGuide::NouvelleCible(VecteurString* pVectItemPere, int NombreElementEnCommun,
                               VecteurRechercheSelonCritere* pVecteurSelonCritere)
{
	if (((VecteurString*) NULL == pVectItemPere) || (pVectItemPere->empty()))
		return string("") ;
	if (((VecteurRechercheSelonCritere*) NULL == pVecteurSelonCritere) || (pVecteurSelonCritere->empty()))
		return string("") ;

	if (_VecEquivalentItemDuPere.empty())
		return string("") ;

	string sNouvelleChaine = string("") ;

	//�quivalents des items du p�re
	EquiItemVectorIter vect = _VecEquivalentItemDuPere.begin() ;

	//chaine cible
	EquiItemIter IterPere = pVectItemPere->begin() ;
	for (int i = 1; (IterPere != pVectItemPere->end()) && (i <= NombreElementEnCommun) &&
                  (_VecEquivalentItemDuPere.end() != vect) ;
                   IterPere++, vect++, i++)
		sNouvelleChaine += *(*IterPere) + string(1,cheminSeparationMARK) ;

	// prendre les minimums des �quivalents des items du p�re
	while (_VecEquivalentItemDuPere.end() != vect)
	{
    if (false == (*vect)->empty())
		  sNouvelleChaine += *(*(*vect)->begin()) + string(1, cheminSeparationMARK) ;
    vect++ ;
	}
	sNouvelleChaine += pVecteurSelonCritere->BorneInf() ;

	return sNouvelleChaine ;
}

//------------------------------------------------------------------------------
// mettre � jour les �quivalents de l'item � la position NombreElementEnCommun
//------------------------------------------------------------------------------
void
NSFilGuide::MettreAjourEquivalent(int NombreElementEnCommun)
{
	if (_VecEquivalentItemDuPere.empty())
		return ;

	int cont = 1 ;
	EquiItemVectorIter iter = _VecEquivalentItemDuPere.begin() ;
	for ( ; _VecEquivalentItemDuPere.end() != iter ; iter++)
	{
		if (cont == NombreElementEnCommun)
		{
    	string sItem = *(*((*iter)->begin())) ;
      (*iter)->vider() ;
      chercheEquivalent(sItem, (*iter), string("ES")) ; // constituer les �quivalents au sens r�seau de (*iter)
      sort((*iter)->begin(), (*iter)->end(), inferieure) ;
      return ;
    }
    cont++ ;
	}
}

//-----------------------------------------------------------------------------
// 						recherche par objet
//-----------------------------------------------------------------------------
void
NSFilGuide::chercheCheminObjet(string* /* pChemin */, VecteurRechercheSelonCritere* /* VecteurSelonCriter */)
{
}

//---------------------------------------------------------------------------
// chercher pChemin dans le filguide selon la pr�cision donn�e :
//         	compStrict	:	�galit� strice
//         	compReseau	:	�galit� au sens r�seau
//          compObjet	:  �galit� au sens objet
//	pVecteurSelonCritere :  contient (pour chaque fils � trouver) :
//										->	son �tiquette
//										->	bool�en 	: trouv� ou non
//										-> pDonnees : pointeur sur BBItemData
//										-> ses �quivalents s�mantiques
//---------------------------------------------------------------------------
void
NSFilGuide::chercheChemin(string* pCheminPere, VecteurRechercheSelonCritere* pVecteurSelonCritere,
                                 int precision)
{
	switch (precision)
	{
  	case compStrict	:
    	chercheCheminEgaliteStricte(pCheminPere, pVecteurSelonCritere) ;
      break ;
    case compReseau	:
    	chercheCheminReseau(pCheminPere, pVecteurSelonCritere) ;
      break;
    case compObjet :
    	chercheCheminObjet(pCheminPere, pVecteurSelonCritere) ;
      break;
	}
}

//---------------------------------------------------------------------------
// Inverser une chaine.
//	Exemple : OCONT/GECHY donne GECHY/OCONT
//---------------------------------------------------------------------------
void
NSFilGuide::InverseChaine(string* pLocalisation, string* pInverse)
{
	if (NULL == pInverse)
		return ;
	*pInverse = string("") ;

	if ((NULL == pLocalisation) || (string("") == *pLocalisation))
		return ;

	size_t posit = pLocalisation->find(string(1, cheminSeparationMARK)) ;
  if (posit == string::npos)
	{
		*pInverse = *pLocalisation ;
    return ;
	}

	size_t debut = 0 ;
	int    i = 1 ;

	while (posit != string::npos)
	{
		if (i == 1 )
			*pInverse = string(1, cheminSeparationMARK) + string(*pLocalisation, debut, posit - debut) + *pInverse ;
		else
			*pInverse = string(*pLocalisation, debut, posit - debut) + *pInverse;

		debut = posit ;
		posit = pLocalisation->find(string(1, cheminSeparationMARK), debut+1) ;
		i++ ;
	}
	*pInverse = string(*pLocalisation, debut+1, strlen(pLocalisation->c_str())-debut-1) + *pInverse ;
}


// -----------------------------------------------------------------------------
//
// Fonctions annexes
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Fonction qui renvoie si le code sCode1 est li� par la relation sRelation au
// code sCode2.
// Modif FLP : si les codes sont des codes Lexique, on les transforme en codes
// sens (avant la fonction renvoyait faux syst�matiquement si les codes n'�tait
// pas des codes Sens).
// --
// Does code sCode1 has a relation sRelation to code sCode2.
// Modified by FLP : now it can take both LexicalCode than MeaningCode.
// -----------------------------------------------------------------------------
bool
NSFilGuide::VraiOuFaux(string sCode1, string sRelation, string sCode2, DBITBLNAME sensCle, bool bPumpMessages)
{
try
{
	// r�cup�ration des codes Sens.
	// Retrieving MeaningCodes.
	string sCodeSens1 = string("") ;
	NSDico::donneCodeSens(&sCode1, &sCodeSens1) ;
	string sCodeSens2 = string("") ;
	NSDico::donneCodeSens(&sCode2, &sCodeSens2) ;

  // First, check if there is a ready answer
  //
  VectString* pReadyResult = _aSearchEltsArray.getExistingResult(sCodeSens1, sRelation, sensCle) ;
  if (pReadyResult)
  {
    if (pReadyResult->contains(sCodeSens2))
      return true ;
    else
      return false ;
  }

  // If no ready answer exists, go search in semantic network
  //
	VectString VecteurString ;
	TousLesVrais(sCodeSens1, sRelation, &VecteurString, sensCle, bPumpMessages) ;

  // And add this search to searches buffer
  //
  _aSearchEltsArray.addResult(sCodeSens1, sRelation, sensCle, &VecteurString) ;

	if (VecteurString.contains(sCodeSens2))
    return true ;

	return false ;
}
catch (...)
{
	erreur("Exception NSFilGuide::VraiOuFaux", standardError) ;
	return false ;
}
}

//---------------------------------------------------------------------------
// Mettre tous les items li�s � sCode1 par sRelation dans pVecteurString
//
// Si la relation est "ES", TousLesVrais est �quivalent � chercheEquivalent,
// pour les autres relations, il permet, pour chaque �l�ment valide, d'ajouter
// ses �quivalents � la liste
//---------------------------------------------------------------------------
void
NSFilGuide::TousLesVrais(string sCode, string sRelation, VectString* pVecteurString, DBITBLNAME sensCle, bool bPumpMessages)
{
try
{
	if ((VectString*) NULL == pVecteurString)
		return ;

  string ps = "NSFilGuide::TousLesVrais: entering (" + sCode + " " + sRelation + " " + sensCle + ")" ;
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	bool ok ;
	//�quivalents s�mantiques de sCode
	VecteurString EquivalentsCode ;

  // No transitivity for "E0"
  if (string("E0") == sRelation)
    EquivalentsCode.AddString(sCode) ;
  else
	  chercheEquivalent(sCode, &EquivalentsCode, "ES", sensCle) ;

  if (EquivalentsCode.empty())
  {
    ps = "NSFilGuide::TousLesVrais: nothing found, leaving." ;
    _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
		return ;
  }

  int iVectorSize = 0 ;

	EquiItemIter EquiCode = EquivalentsCode.begin() ;
	for ( ; EquivalentsCode.end() != EquiCode ; EquiCode++)
	{
#ifndef _ENTERPRISE_DLL
    if (bPumpMessages)
      _pSuper->getApplication()->PumpWaitingMessages() ;
#endif

		string sLie ;
    //trouver tous les items li�s � EquiCode par sRelation
    VecteurString EquivalentItemsLie ;
    chercheEquivalent(*(*EquiCode), &EquivalentItemsLie, sRelation, sensCle) ;
    if (false == EquivalentsCode.empty())
    {
    	EquiItemIter EquiItemLie = EquivalentItemsLie.begin() ;
      for ( ; EquivalentItemsLie.end() != EquiItemLie ; EquiItemLie++)
      {
#ifndef _ENTERPRISE_DLL
        if (bPumpMessages)
          _pSuper->getApplication()->PumpWaitingMessages() ;
#endif

      	sLie = *(*EquiItemLie) ;
        //chercher les �quivalents s�mantiques de sLie
        VecteurString EquivalentsLie ;
        chercheEquivalent(sLie, &EquivalentsLie, "ES", sensCle) ;
        if (false == EquivalentsLie.empty())
  			{
        	EquiItemIter EquiLie = EquivalentsLie.begin() ;
          for ( ; EquivalentsLie.end() != EquiLie ; EquiLie++)
          {
          	string sEquivalent = *(*EquiLie) ;
            ok = true ;
            //v�rifier que *(*EquiLie) ne fait pas partie d�j� de pVecteurString
            for (IterString i = pVecteurString->begin(); (i != pVecteurString->end())
                    && ok; i++)
            	if (((*(*i)) == sEquivalent))
              	ok = false ;

            if (ok && (!(sEquivalent == SousChemin)) &&
            		   (!(sEquivalent == ItemGenerique)) &&
                     (!(sEquivalent == SousCheminNonVide)))
            {
            	pVecteurString->push_back(new string(*(*EquiLie))) ;
              iVectorSize++ ;
            }
          }
        }
      }
    }
	}
  ps = "NSFilGuide::TousLesVrais: done, leaving (" + IntToString(iVectorSize) + " elements found)";
  _pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
}
catch (...)
{
	erreur("Exception NSFilGuide::TousLesVrais", standardError) ;
	return ;
}
}

//---------------------------------------------------------------------------
//premier item li� � sCode1 par sRelation
//---------------------------------------------------------------------------
string
NSFilGuide::PremierVrai(string sCode1, string sRelation, DBITBLNAME sensCle)
{
	return chercheQualifiant(sCode1, sRelation, sensCle) ;
}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
string
NSFilGuide::InversePremierVrai(string /* sCode1 */, string /* sRelation */, DBITBLNAME /* sensCle */)
{
	return "" ;
}

//---------------------------------------------------------------------------
//mettre tous les items li�s � sCode1 par sRelation dans pVecteurString
//---------------------------------------------------------------------------
void
NSFilGuide::InverseTousLesVrais(string /* sCode */, string /* sRelation */, VectString* /* pVecteurString */, DBITBLNAME /* sensCle */, bool /* bPumpMessages */)
{
}

//---------------------------------------------------------------------------
//savoir si sCode1 et sCode2 sont li�s par sRelation
//---------------------------------------------------------------------------
bool
NSFilGuide::InverseVraiOuFaux(string /* sCode1 */, string /* sRelation */, string /* sCode2 */, DBITBLNAME /* sensCle */, bool /* bPumpMessages */)
{
  return false ;
}

//--------------------------------------------------------------------------
//recherche composite selon le lien sLien et le qualifi� sQualifie, retourner
//le qualifiant
//---------------------------------------------------------------------------
string
NSFilGuide::chercheQualifiant(string sQualifie, string sLien, DBITBLNAME sensCle)
{
#ifndef _ENTERPRISE_DLL
try
{
  DBIResult ErrDBI ;
  CURProps  curProps ;
  DBIResult lastError = DbiGetCursorProps(_pSavoir->PrendCurseur(), curProps) ;
  if (lastError != DBIERR_NONE)
  {
    erreur("Le fichier nssavoir.db inutilisable.", standardError, lastError);
    return string("") ;
  }
  Byte* pIndex = new Byte[curProps.iRecBufSize];
  memset(pIndex, 0, curProps.iRecBufSize);
  DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_LIEN_FIELD, pIndex,(Byte*)(sLien.c_str()));
  DbiPutField(_pSavoir->PrendCurseur(), SAVOIR_QUALIFIE_FIELD , pIndex, (Byte*)(sQualifie.c_str()));
  ErrDBI = _pSavoir->chercheClefComposite(sensCle,
                                        NODEFAULTINDEX,
                                        keySEARCHEQ,
                                        dbiWRITELOCK,
                                        pIndex);
  if (ErrDBI != DBIERR_NONE)
  {
    delete[] pIndex;
    return string("") ;
  }

  _pSavoir->lastError = _pSavoir->getRecord();
  if (_pSavoir->lastError != DBIERR_NONE)
  {
    erreur("Le fichier nssavoir.db inutilisable.", standardError, _pSavoir->lastError);
    delete[] pIndex;
    return string("") ;
  }
  string sQualifiant = _pSavoir->getQualifier() ;
  delete[] pIndex;
  return sQualifiant;
}
catch (...)
{
	erreur("Exception NSFilGuide::chercheQualifiant", standardError, 0) ;
  return string("") ;
}
#else
  return string("") ;
#endif
}

/**
 * Get closest is_a that is a (0) generic anatomic element
 */
string
NSFilGuide::GetClosestGenericIsA(string sCode)
{
  if (string("") == sCode)
    return string("") ;

  VecteurString EquivalentsCode ;
	chercheEquivalent(sCode, &EquivalentsCode, "ES") ;

  if (EquivalentsCode.empty())
    return string("") ;

	for (EquiItemIter EquiCode = EquivalentsCode.begin() ; EquivalentsCode.end() != EquiCode ; EquiCode++)
	  if ((**EquiCode != sCode) && VraiOuFaux(**EquiCode, string("E0"), string("AGENE")))
      return **EquiCode ;

  return string("") ;
}

/**
 *  Get the virtual drug from a speciality (from speciality -is a-> virtuak drug)
 */
string
NSFilGuide::getVirtualDrug(const string sSpecialityCode, NSContexte* pContexte, bool bReturnLabel)
{
  if ((string("") == sSpecialityCode) || ((NSContexte*) NULL == pContexte))
    return string("") ;

  // Get all elements such as sSpeciality - is a -> element
  //
  string sCodeSens = NSDico::donneCodeSens(&sSpecialityCode) ;

  VecteurString aVecteurString ;
  chercheEquivalent(sCodeSens, &aVecteurString, string("ES")) ;

  if (aVecteurString.empty())
    return string("") ;

  // Find if there is a virtual in the list
  //
  EquiItemIter itCodes = aVecteurString.begin() ;
  for ( ; itCodes != aVecteurString.end() ; itCodes++)
  {
    if (sCodeSens != **itCodes)
    {
      string sLibelleTrouve = string("") ;
      if (pContexte->getDico()->isVirtualDrug(pContexte->getUserLanguage(), *itCodes, &sLibelleTrouve))
      {
        if (bReturnLabel)
          return sLibelleTrouve ;
        return **itCodes ;
      }
    }
  }

  return string("") ;
}

SemanticSearchElement::SemanticSearchElement()
{
  _sCode     = string("") ;
  _sRelation = string("") ;
  strcpy(_sensCle, "FLECHE") ;
}

SemanticSearchElement::SemanticSearchElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString *pVectString)
{
  _sCode     = sCode ;
  _sRelation = sRelation ;
  strcpy(_sensCle, sensCle) ;

  if (pVectString)
    _resultVecteurString = *pVectString ;
}

SemanticSearchElement::SemanticSearchElement(const SemanticSearchElement& rv)
{
  _sCode     = rv._sCode ;
  _sRelation = rv._sRelation ;
  strcpy(_sensCle, rv._sensCle) ;

  _resultVecteurString = rv._resultVecteurString ;
}

SemanticSearchElement::~SemanticSearchElement()
{
}

bool
SemanticSearchElement::isSameSearch(const SemanticSearchElement& o)
{
  return isSameSearch(o._sCode, o._sRelation, o._sensCle) ;
}

bool
SemanticSearchElement::isSameSearch(const string sCode, const string sRelation, const DBITBLNAME sensCle)
{
  return ((_sCode     == sCode)     &&
          (_sRelation == sRelation) &&
          (strcmp(_sensCle, sensCle) == 0)) ;
}

SemanticSearchElement&
SemanticSearchElement::operator=(const SemanticSearchElement& src)
{
  if (&src == this)
  	return *this ;

  _sCode     = src._sCode ;
  _sRelation = src._sRelation ;
  strcpy(_sensCle, src._sensCle) ;

  _resultVecteurString = src._resultVecteurString ;

  return *this ;
}

int
SemanticSearchElement::operator==(const SemanticSearchElement& o)
{
  return ((_sCode     == o._sCode)     &&
          (_sRelation == o._sRelation) &&
          (strcmp(_sensCle, o._sensCle) == 0) &&
          (_resultVecteurString == o._resultVecteurString)) ;
}

SemanticSearchElementArray::SemanticSearchElementArray(const SemanticSearchElementArray& rv)
{
  if (false == rv.empty())
    for (SemanticSearchElementConstIter iter = rv.begin() ; rv.end() != iter ; iter++)
      push_back(new SemanticSearchElement(**iter)) ;
}

SemanticSearchElementArray&
SemanticSearchElementArray::operator=(const SemanticSearchElementArray& src)
{
  if (this == &src)
    return *this ;

  vider() ;

  if (false == src.empty())
    for (SemanticSearchElementConstIter iter = src.begin() ; src.end() != iter ; iter++)
      push_back(new SemanticSearchElement(**iter)) ;

  return *this ;
}

VectString*
SemanticSearchElementArray::getExistingResult(string sCode, string sRelation, DBITBLNAME sensCle)
{
  if (empty())
    return (VectString*) 0 ;

  for (SemanticSearchElementIter iter = begin() ; end() != iter ; iter++)
    if ((*iter)->isSameSearch(sCode, sRelation, sensCle))
      return (*iter)->getResultVecteurString() ;

  return (VectString*) 0 ;
}

void
SemanticSearchElementArray::addResult(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString)
{
  if (NULL == getExistingResult(sCode, sRelation, sensCle))
    addElement(sCode, sRelation, sensCle, pVectString) ;
}

void
SemanticSearchElementArray::addElement(string sCode, string sRelation, DBITBLNAME sensCle, VectString* pVectString)
{
  if (size() >= maxSearchElement)
  {
    SemanticSearchElementIter iter = begin() ;
    delete(*iter) ;
    erase(iter) ;
  }

  push_back(new SemanticSearchElement(sCode, sRelation, sensCle, pVectString)) ;
}

SemanticSearchElementArray::~SemanticSearchElementArray()
{
  vider() ;
}

