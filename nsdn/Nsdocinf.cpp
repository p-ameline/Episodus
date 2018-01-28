//---------------------------------------------------------------------------//    NSANNEXE.CPP
//    PA   juillet 94
//  Implémentation des objets annexes de NAUTILUS
//---------------------------------------------------------------------------
#include <cstring.h>
#include <windows.h>
#include <mem.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
#endif

#include "nsbb\nsbbtran.h"
#include "nsbb\nspatpat.h"
#include "nsdn\nsdocnoy.h"
#include "nsdn\nsdocum.h"
// #include "nsdn\nsdocdlg.h"

#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsfilgd.h"

long NSDocumentInfo::lObjectCount = 0 ;

//***************************************************************************
// Implémentation des méthodes NSDocumentData
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:		NSDocumentData::metAZero()
//  Description:	Initialise les champs de données à zéro.
//  Retour:			Aucun
//---------------------------------------------------------------------------
void NSDocumentData::metAZero()
{
/*
	// Met les champs de données à zéro
	memset(codePatient,	 0, PAT_NSS_LEN + 1) ;
	memset(codeDocument, 0, DOC_CODE_DOCUM_LEN + 1) ;
	memset(nom, 	   	   0, DOC_NOM_LEN + 1) ;
	memset(creation,   	 0, DOC_CREATION_LEN + 1) ;
	memset(acces, 		   0, DOC_ACCES_LEN + 1) ;
	memset(createur,   	 0, DOC_CREATEUR_LEN + 1) ;
	memset(type, 	   	   0, DOC_TYPE_LEN + 1) ;
	memset(localisation, 0, DOC_LOCALISATION_LEN + 1) ;
	memset(fichier,   	 0, DOC_FICHIER_LEN + 1) ;
	memset(entete,		   0, DOC_ENTETE_LEN + 1) ;
*/

  _sCodePatient  = string("") ;
	_sCodeDocument = string("") ;
	_sNom          = string("") ;
	_sDateCreation = string("") ;
	_sAcces        = string("") ;
	_sCreateur     = string("") ;
	_sAuteur       = string("") ;
	_sDestinataire = string("") ;
	_sType         = string("") ;
  _sTypeComplet  = string("") ;
	_sLocalisation = string("") ;
	_sFichier         = string("") ;  _sTemplate        = string("") ;  _sEnTete          = string("") ;  _sTypeContenu     = string("") ;  _sTypeContenuSens = string("") ;  _sDateExamen      = string("") ;  _sEmplacement  = string("") ;	_sInteret      = string("") ;
	_sCommentaire  = string("") ;
	_sVisible      = string("") ;
	_sTranNew      = string("") ;
	_sTranDel      = string("") ;
	_sCRC          = string("") ;	_sRights = string("") ;}

//---------------------------------------------------------------------------//  Fonction:  	NSDocumentData::donneDateCreation()
//  Description:	renvoie la date de création du document
//  Retour:			string date
//---------------------------------------------------------------------------
string
NSDocumentData::donneDateCreation(string sLang)
{
	if ((string("") == _sDateCreation) || (strlen(_sDateCreation.c_str()) < 8))
		return string("") ;

	return donne_date(_sDateCreation, sLang) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentData::NSDocumentData(NSDocumentData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSDocumentData::NSDocumentData(const NSDocumentData& rv)
{
	_sCodePatient  = rv._sCodePatient ;
	_sCodeDocument = rv._sCodeDocument ;
	_sNom          = rv._sNom ;
	_sDateCreation = rv._sDateCreation ;
	_sAcces        = rv._sAcces ;
	_sCreateur     = rv._sCreateur ;
	_sAuteur       = rv._sAuteur ;
	_sDestinataire = rv._sDestinataire ;
	_sType         = rv._sType ;
  _sTypeComplet  = rv._sTypeComplet ;
	_sLocalisation = rv._sLocalisation ;

  _sFichier         = rv._sFichier ;  _sTemplate        = rv._sTemplate ;  _sEnTete          = rv._sEnTete ;  _sTypeContenu     = rv._sTypeContenu ;  _sTypeContenuSens = rv._sTypeContenuSens ;  _sDateExamen      = rv._sDateExamen ;  _sEmplacement  = rv._sEmplacement ;	_sInteret      = rv._sInteret ;
	_sCommentaire  = rv._sCommentaire ;
	_sVisible      = rv._sVisible ;
	_sTranNew      = rv._sTranNew ;
	_sTranDel      = rv._sTranDel ;
	_sCRC          = rv._sCRC ;	_sRights = rv._sRights ;}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentData::operator=(NSDocumentData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSDocumentData& NSDocumentData::operator=(const NSDocumentData& src)
{
	if (this == &src)
		return *this ;

	_sCodePatient  = src._sCodePatient ;
	_sCodeDocument = src._sCodeDocument ;
	_sNom          = src._sNom ;
	_sDateCreation = src._sDateCreation ;
	_sAcces        = src._sAcces ;
	_sCreateur     = src._sCreateur ;
	_sAuteur       = src._sAuteur ;
	_sDestinataire = src._sDestinataire ;
	_sType         = src._sType ;
  _sTypeComplet  = src._sTypeComplet ;
	_sLocalisation = src._sLocalisation ;

  _sFichier         = src._sFichier ;  _sTemplate        = src._sTemplate ;  _sEnTete          = src._sEnTete ;  _sTypeContenu     = src._sTypeContenu ;  _sTypeContenuSens = src._sTypeContenuSens ;  _sDateExamen      = src._sDateExamen ;  _sEmplacement  = src._sEmplacement ;	_sInteret      = src._sInteret ;
	_sCommentaire  = src._sCommentaire ;
	_sVisible      = src._sVisible ;
	_sTranNew      = src._sTranNew ;
	_sTranDel      = src._sTranDel ;
	_sCRC          = src._sCRC ;	_sRights = src._sRights ;
	return *this ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentData::operator==( const NSDocumentData& o )
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int NSDocumentData::operator == ( const NSDocumentData& o )
{
	if ((_sCodePatient  == o._sCodePatient)  &&
	    (_sCodeDocument == o._sCodeDocument) &&
	    (_sNom          == o._sNom)          &&
	    (_sDateCreation == o._sDateCreation) &&
	    (_sAcces        == o._sAcces)        &&
	    (_sCreateur     == o._sCreateur)     &&
	    (_sAuteur       == o._sAuteur)       &&
	    (_sDestinataire == o._sDestinataire) &&
	    (_sType         == o._sType)         &&
	    (_sLocalisation == o._sLocalisation) &&

/*
  (strcmp(codePatient,	o.codePatient)	== 0) &&
      (strcmp(codeDocument,	o.codeDocument)	== 0) &&
		  (strcmp(nom,			 	  o.nom) 		  	 	== 0) &&
		  (strcmp(creation, 	 	o.creation)	 		== 0) &&
		  (strcmp(acces, 		 		o.acces)		 	  == 0) &&
		  (strcmp(createur, 	 	o.createur)	 		== 0) &&
		  (strcmp(type,		  	 	o.type)			 	  == 0) &&
      (strcmp(localisation,	o.localisation)	== 0) &&
		  (strcmp(fichier,		 	o.fichier)		 	== 0) &&
      (strcmp(entete,				o.entete)			  == 0) &&
*/
      (_sFichier     == o._sFichier) &&      (_sTemplate    == o._sTemplate) &&      (_sEnTete      == o._sEnTete) &&      (_sTypeContenu == o._sTypeContenu) &&      (_sTypeContenuSens == o._sTypeContenuSens) &&      (_sDateExamen  == o._sDateExamen) &&      (_sRights      == o._sRights) &&/*		  (strcmp(emplacement, o.emplacement) == 0) &&		  (strcmp(interet,		 o.interet)		 	== 0) &&
		  (strcmp(commentaire, o.commentaire) == 0) &&
      (strcmp(visible,     o.visible)			== 0) &&
      (strcmp(tranNew,		 o.tranNew)			== 0) &&
      (strcmp(tranDel,		 o.tranDel)			== 0) &&
      (strcmp(crc,				 o.crc)				  == 0))
*/
	    (_sEmplacement  == o._sEmplacement) &&
      (_sInteret      == o._sInteret)     &&
	    (_sCommentaire  == o._sCommentaire) &&
	    (_sVisible      == o._sVisible)     &&
	    (_sTranNew      == o._sTranNew)     &&
	    (_sTranDel      == o._sTranDel)     &&
	    (_sCRC          == o._sCRC))
		return 1 ;
	else
		return 0 ;
}

void
NSDocumentData::setID(string sId)
{
  if (strlen(sId.c_str()) != PAT_NSS_LEN + DOC_CODE_DOCUM_LEN)
    return ;

  _sCodePatient  = string(sId, 0, PAT_NSS_LEN) ;
  _sCodeDocument = string(sId, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentInfo::NSDocumentInfo(NSSuper* pSuper)
//  Description:	Constructeur avec superviseur (pour l'accès à la base des chemins)
//  Retour:			Rien
//---------------------------------------------------------------------------
NSDocumentInfo::NSDocumentInfo(NSContexte* pCtx)
               :NSRoot(pCtx),
                _Meta(pCtx->getSuperviseur()),
                _Pres(pCtx->getSuperviseur())
{
try
{
	_sCodeDocMeta = string("") ;
	_sCodeDocPres = string("") ;  lObjectCount++ ;}catch (...)
{
	erreur("Exception NSDocumentInfo ctor", standardError) ;
}
}
/*
NSDocumentInfo::NSDocumentInfo(string sMeta, NSContexte* pCtx, NSPatInfo* pPatInfo)
               :NSRoot(pCtx){try{	// Crée l'objet de données	pDonnees		 = new NSDocumentData() ;
	pMeta 			 = new NSPatPathoArray(pCtx->getSuperviseur()) ;
	pPres        = new NSPatPathoArray(pCtx->getSuperviseur()) ;
	sCodeDocMeta = sMeta ;
	sCodeDocPres = "" ;

	if (pPatInfo == 0)
  	pPatInfo = pContexte->getPatient() ;

	if (false == ChargeDocMeta(pPatInfo))
  {
  	char msg[255];
    sprintf(msg, "Impossible de charger le méta-document n°%s.", sCodeDocMeta.c_str());
    erreur(msg, standardError, 0) ;
  }
}
catch (...)
{
	erreur("Exception NSDocumentInfo ctor(sMeta)", standardError, 0) ;
}
}*/
NSDocumentInfo::NSDocumentInfo(string sMeta, NSContexte* pCtx, NSPersonGraphManager* pGraphManager)
               :NSRoot(pCtx),                _Meta(pCtx->getSuperviseur()),                _Pres(pCtx->getSuperviseur()){try{  _sCodeDocMeta = sMeta ;	_sCodeDocPres = string("") ;

	if (false == ChargeDocMeta(pGraphManager))
  {
  	char msg[255];
    sprintf(msg, "Impossible de charger le méta-document n°%s.", _sCodeDocMeta.c_str());
    erreur(msg, standardError) ;
  }

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo ctor(sMeta)", standardError) ;
}
}
//---------------------------------------------------------------------------//  Function:		NSDocumentInfo::~NSDocumentInfo()
//  Description:	Constructeur.
//  Retour:			Aucun
//---------------------------------------------------------------------------
NSDocumentInfo::~NSDocumentInfo()
{
  lObjectCount-- ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentInfo::NSDocumentInfo(NSDocumentInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSDocumentInfo::NSDocumentInfo(const NSDocumentInfo& rv)
               :NSRoot(rv.pContexte),
                _Meta(rv._Meta),
                _Pres(rv._Pres)
{
try
{
	//
	// Copie les valeurs du NSDocumentInfo d'origine
	//
	_Donnees      = rv._Donnees ;

	_sCodeDocMeta = rv._sCodeDocMeta ;
	_sCodeDocPres = rv._sCodeDocPres ;

  lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo copy ctor", standardError) ;
}
}
//---------------------------------------------------------------------------//  Fonction:  	NSDocumentInfo::getNomFichier(sNomFichier)
//  Description:	Renvoie le nom de fichier MSDOS correspondant au document
//  Retour:			false si vide, true si ça a marché
//---------------------------------------------------------------------------
bool
NSDocumentInfo::getNomFichier(string& sNomFichier)
{
	string sLocalis = string("") ;
	string sPath    = string("") ;

	// les noms de fichier complets sont inscrits dans la base
  //
	sNomFichier = string("") ;

	if (string("") == _Donnees._sFichier)
  	return false ;

	sLocalis = _Donnees._sLocalisation ;

#ifndef _ENTERPRISE_DLL
	// Si le fichier est référencé, on récupère son PathName
	if (string("") != sLocalis)
	{
  	if (pContexte == 0)
    {
    	erreur("Erreur dans la fonction getNomFichier : Le contexte n'est pas initialisé.", standardError) ;
      return false ;
    }
    else
    	sPath = pContexte->PathName(sLocalis) ;
  }
#endif

	sNomFichier = sPath + _Donnees._sFichier ;

	return true ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentInfo::operator=(NSDocumentInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSDocumentInfo&
NSDocumentInfo::operator=(const NSDocumentInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees      = src._Donnees ;
	_Meta         = src._Meta ;	_Pres         = src._Pres ;	_sCodeDocMeta = src._sCodeDocMeta ;	_sCodeDocPres = src._sCodeDocPres ;	pContexte 	  = src.pContexte ;
	return *this ;}

void
NSDocumentInfo::setData(const NSDocumentData* pData)
{
  if ((NSDocumentData*) NULL == pData)
    return ;

  _Donnees = *pData ;
}

void
NSDocumentInfo::initFromData(NSDocumentData* pData) const
{
  if ((NSDocumentData*) NULL == pData)
    return ;

  *pData = _Donnees ;
}

void
NSDocumentInfo::setMeta(const NSPatPathoArray* pPpt)
{
  if (NULL == pPpt)
    return ;

  _Meta = *pPpt ;
}

void
NSDocumentInfo::initFromMeta(NSPatPathoArray* pPpt) const
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

   *pPpt = _Meta ;
}

void
NSDocumentInfo::setPres(const NSPatPathoArray* pPpt)
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  _Pres = *pPpt ;
}

void
NSDocumentInfo::initFromPres(NSPatPathoArray* pPpt) const
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

   *pPpt = _Pres ;
}

//---------------------------------------------------------------------------//  Fonction:		NSDocumentInfo::operator == ( const NSDocumentInfo& o )
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSDocumentInfo::operator==(const NSDocumentInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

bool
NSDocumentInfo::LoadMetaAndData(NSPersonGraphManager* pGraphManager)
{
  if (string("") == _sCodeDocMeta)
    return false ;

  // First, get the tree containing document's metadata
  //
  if (false == ChargeDocMeta(pGraphManager))
    return false ;

  // Then init meta information
  //
  ParseMetaDonnees() ;

  // Last step: get data information
  //
  return GetDataInformation(pGraphManager) ;
}

//--------------------------------------------------------------------------//recupérer la patpatho d'un document
//--------------------------------------------------------------------------
bool
NSDocumentInfo::DonnePatPatho(NSPatPathoArray* pPatPathoArray, NSPersonGraphManager* pGraphManager)
{
	if ((NSPatPathoArray*) NULL == pPatPathoArray)
		return false ;
try
{
	pPatPathoArray->vider() ;

	// if (NULL == pPatInfo)
	//	pPatInfo = pContexte->getPatient() ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

	if (pSuper->typeDocument(getTypeSem(), NSSuper::isTree))
	{
  	string sCodeDocC = getID() ;
    string sRosace   = "" ;
    if (pGraphManager)
      /* resultat = */ pGraphManager->getTree(sCodeDocC, pPatPathoArray, &sRosace) ;
  }
  else if (pSuper->typeDocument(getTypeSem(), NSSuper::isText))
  	// pour tous les types de texte
    pPatPathoArray->ajoutePatho("PXXX51", 0) ;
  else if (pSuper->typeDocument(getTypeSem(), NSSuper::isHTML))
  	// pour tous les types d'html statiques (HD traité plus haut)
    pPatPathoArray->ajoutePatho("PTRIQ3", 0) ;
  else if (pSuper->typeDocument(getTypeSem(), NSSuper::isImage))
  	// cas des images : générique pour tous les types d'image
    // y compris les images animées (vidéos)
    pPatPathoArray->ajoutePatho("ZIMA01", 0) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo::DonnePatPatho", standardError) ;
	return false ;
}
}

//--------------------------------------------------------------------------
//
// get document's label tree
// charger la patpatho du méta-document//
//--------------------------------------------------------------------------bool
NSDocumentInfo::ChargeDocMeta(NSPersonGraphManager* pGraphManager)
{
try
{
	_Meta.vider() ;

  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

	string sRosace = string("") ;
	if (false == pGraphManager->getTree(_sCodeDocMeta, &_Meta, &sRosace))
    return false ;

	_Donnees._sRights = sRosace ;

  // Récupération des données de présentation liées au méta
  VecteurString aVecteurString ;
  pGraphManager->getLinkManager()->TousLesVrais(_sCodeDocMeta, NSRootLink::docComposition, &aVecteurString) ;
  if (aVecteurString.empty())
    return true ;

  _sCodeDocPres = *(*(aVecteurString.begin())) ;
  sRosace = string("") ;
  bool resultat = pGraphManager->getTree(_sCodeDocPres, &_Pres, &sRosace) ;
  //
  // Si on ne les a pas récupérées, ce n'est pas considéré comme anormal
  // si l'arbre est collectif ou groupe (et qu'il provient d'un autre site)
  //
  if (false == resultat)
  {
    string sMetaTreeID = getTreeIDFromID(_sCodeDocMeta) ;
    GRAPHELEMTYPE elemType = getGraphElementTypeFromID(sMetaTreeID) ;
    if ((isCollectiveID == elemType) || (isGroupID == elemType))
      resultat = true ;
	}

	return resultat ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo::ChargeDocMeta.", standardError);
	return false ;
}
}
//--------------------------------------------------------------------------//
// parser les données du méta-document pour remplir les pDonnees//
//--------------------------------------------------------------------------bool
NSDocumentInfo::ParseMetaDonnees(){	if (_Meta.empty())		return false ;  PatPathoIter iter = _Meta.begin() ;
  _Donnees._sInteret = (*iter)->getInteret() ;
  _Donnees._sVisible = (*iter)->getVisible() ;

  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase))
  {
  	string sElemLex = (*iter)->getLexique() ;
    string sSens    = (*iter)->getLexiqueSens() ;

    // nom du document
    if (string("0INTI") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
          _Donnees._sNom = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // opérateur
    else if (string("DOPER") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
          _Donnees._sCreateur = (*iter)->getComplement() ;

        iter++ ;
      }
    }
    // auteur
    else if (string("DAUTE") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
          _Donnees._sAuteur = (*iter)->getComplement() ;

        iter++ ;
      }
    }
    // destinataire
    else if (string("DDEST") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
          _Donnees._sDestinataire = (*iter)->getComplement() ;

        iter++ ;
      }
    }
    // type
    else if (string("0TYPE") == sSens)
    {
    	iter++ ;
      if ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        setType((*iter)->getLexique()) ;
      	iter++ ;
      }
    }
    // localisation
    else if (string("0LFIC") == sSens)
    {
      _Donnees._sLocalisation = (*iter)->getComplement() ;
      iter++ ;
    }
    // nom de fichier (pour les fichiers statiques)
    else if (string("0NFIC") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
          _Donnees._sFichier = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // date de rédaction
    else if (string("KREDA") == sSens)
    {
    	iter++ ;
      // int iLigneBase = (*iter)->pDonnees->getLigne() ;

			string sUnite  = (*iter)->getUnitSens() ;
			string sFormat = (*iter)->getLexiqueSens() ;
			string sValeur = (*iter)->getComplement() ;

			if (string("2DA02") == sUnite)
      {
      	if (strlen(sValeur.c_str()) > DOC_CREATION_LEN)
        	sValeur = string(sValeur, 0, DOC_CREATION_LEN) ;

				_Donnees._sDateCreation = sValeur ;
      }

      iter++ ;
    }
    // date d'examen
    else if (string("KCHIR") == sSens)
    {
    	iter++ ;

			string sUnite  = (*iter)->getUnitSens() ;
			string sFormat = (*iter)->getLexiqueSens() ;
			string sValeur = (*iter)->getComplement() ;

			if (sUnite == "2DA02")
      {
      	if (strlen(sValeur.c_str()) > 8)
        	sValeur = string(sValeur, 0, 8) ;

				_Donnees._sDateExamen = sValeur ;
      }

      iter++ ;
    }
    // type de contenu (code lexique)
    else if (string("0TYPC") == sSens)
    {
    	iter++ ;
      while ((_Meta.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        _Donnees._sTypeContenu     = (*iter)->getLexique() ;
        _Donnees._sTypeContenuSens = (*iter)->getLexiqueSens() ;
        iter++ ;
      }
    }
    else
    	iter++ ;
  }  if (_Pres.empty())    return true ;  // Parsing des données de présentation : template et en-tete  //  iter = _Pres.begin() ;  iColBase = (*iter)->getColonne() ;  iter++ ;  while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase))  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = (*iter)->getLexiqueSens() ;

    // template
    if (string("0TPL0") == sSens)
    {
      iter++ ;
      while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
          _Donnees._sTemplate = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // en-tete
    else if (string("0ENTE") == sSens)
    {
      iter++ ;
      while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
          _Donnees._sEnTete = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
      iter++ ;
  }	return true ;}boolNSDocumentInfo::GetDataInformation(NSPersonGraphManager* pGraphManager){  if ((string("") == _sCodeDocMeta) || ((NSPersonGraphManager*) NULL == pGraphManager))    return false ;  VecteurString aVecteurString ;  pGraphManager->getLinkManager()->TousLesVrais(_sCodeDocMeta, NSRootLink::docData, &aVecteurString) ;
  if (aVecteurString.empty())
    return false ;

  string sCodeDocData = *(*(aVecteurString.begin())) ;
  setID(sCodeDocData) ;

  return true ;
}
//--------------------------------------------------------------------------//
// enregistrer un nouveau méta-document à partir des pDonnees//
//--------------------------------------------------------------------------boolNSDocumentInfo::CommitMetaDonnees(){try{	// NOTE : En N_TIERS on prend simplement un pointeur sur la pMeta pour la mettre à jour.	// (Le fait de passer par un pDocNoy était auparavant obligatoire pour la mise à jour)
  //
	NSPatPathoArray* pPptMeta = &_Meta ;

  if (((NSPatPathoArray*) NULL == pPptMeta) || (true == pPptMeta->empty()))
		return true ;

	int iColBase;

	string sLexique ;
	string sNom, sOper, sType, sLocalis ;
	string sFichier, sTemplate, sEnTete, sTypeCont ;
	string sDateReda, sDateExam ;
	bool bExisteDateExam = false ;
	bool bExisteTypeCont = false ;
  bool bExisteAuthor   = false ;

	string sTemp = "" ;

	PatPathoIter iter = pPptMeta->begin() ;
  // interet
  string sInteret = (*iter)->getInteret() ;
  if (sInteret != _Donnees._sInteret)
		(*iter)->setInteret(_Donnees._sInteret) ;
  // visible
  string sVisible = (*iter)->getVisible() ;
  if (sVisible != _Donnees._sVisible)
  	(*iter)->setVisible(_Donnees._sVisible) ;

  iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
  	string sElemLex = (*iter)->getLexique() ;
    string sSens    = (*iter)->getLexiqueSens() ;

    // nom du document
    if (string("0INTI") == sSens)
    {
    	iter++ ;
      sNom = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	sNom = (*iter)->getTexteLibre() ;
          if (sNom != _Donnees._sNom)
          	(*iter)->setTexteLibre(_Donnees._sNom) ;
        }
        iter++ ;
      }
    }
    // opérateur
    else if (string("DOPER") == sSens)
    {
    	iter++ ;
      sOper = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
        {
        	sOper = (*iter)->getComplement() ;
          if (sOper != _Donnees._sCreateur)
          	(*iter)->setComplement(_Donnees._sCreateur) ;
        }
        iter++ ;
      }
    }
    // auteur
    else if (string("DAUTE") == sSens)
    {
    	iter++ ;
      sOper = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
        {
        	sOper = (*iter)->getComplement() ;
          if (sOper != _Donnees._sAuteur)
          	(*iter)->setComplement(_Donnees._sAuteur) ;

          bExisteAuthor = true ;
        }
        iter++ ;
      }
    }
    // destinataire
    else if (string("DDEST") == sSens)
    {
    	iter++ ;
      sOper = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sElemLex = (*iter)->getLexique() ;
        if (string("£SPID1") == sElemLex)
        {
        	sOper = (*iter)->getComplement() ;
          if (sOper != _Donnees._sDestinataire)
          	(*iter)->setComplement(_Donnees._sDestinataire) ;

          bExisteAuthor = true ;
        }
        iter++ ;
      }
    }
    // type
    else if (string("0TYPE") == sSens)
    {
      iter++ ;

      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	sType = (*iter)->getLexique() ;
        if (sType != _Donnees._sTypeComplet)
          (*iter)->setLexique(_Donnees._sTypeComplet) ;

        iter++ ;
      }
    }
    // localisation
    else if (string("0LFIC") == sSens)
    {
    	sLocalis = (*iter)->getComplement() ;
      if (sLocalis != _Donnees._sLocalisation)
      	(*iter)->setComplement(_Donnees._sLocalisation) ;
      iter++ ;
    }
    // nom de fichier (pour les fichiers statiques)
    else if (string("0NFIC") == sSens)
    {
    	iter++ ;
      sFichier = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	sFichier = (*iter)->getTexteLibre() ;
        	if (sFichier != _Donnees._sFichier)
          	(*iter)->setTexteLibre(_Donnees._sFichier) ;
        }
        iter++ ;
      }
    }
    // template
    else if (string("0TPL0") == sSens)
    {
    	iter++ ;
      sTemplate = "" ;
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	sTemplate = (*iter)->getTexteLibre() ;
          if (sTemplate != _Donnees._sTemplate)
          	(*iter)->setTexteLibre(_Donnees._sTemplate) ;
        }
        iter++ ;
      }
    }
    // en-tete
    else if (string("0ENTE") == sSens)
    {
    	iter++ ;
      sEnTete = "";
      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	sEnTete = (*iter)->getTexteLibre() ;
          if (sEnTete != _Donnees._sEnTete)
          (*iter)->setTexteLibre(_Donnees._sEnTete) ;
        }
        iter++ ;
      }
    }
    // date de rédaction
    else if (string("KREDA") == sSens)
    {
    	iter++ ;
      // int iLigneBase = (*iter)->pDonnees->getLigne() ;

			string sUnite  = (*iter)->getUnitSens() ;
			string sFormat = (*iter)->getLexiqueSens() ;
			string sValeur = (*iter)->getComplement() ;

			if (string("2DA02") != sUnite)
      	(*iter)->setUnit("2DA021") ;

      if ((sFormat == "") || (sFormat[0] != '£') || ((sFormat[1] != 'D') && (sFormat[1] != 'T')))
      	(*iter)->setLexique("£T0;10") ;

      // if (strlen(sValeur.c_str()) > 8)
      //	sValeur = string(sValeur, 0, 8) ;

      if (sValeur != _Donnees._sDateCreation)
      {
      	sDateReda = _Donnees._sDateCreation ;
        if (strlen(sDateReda.c_str()) == 8)
        	sDateReda += string("000000") ;
        (*iter)->setComplement(sDateReda) ;
      }

      iter++ ;
    }
    // date d'examen
    else if (string("KCHIR") == sSens)
    {
    	iter++ ;
      // int iLigneBase = (*iter)->pDonnees->getLigne() ;
      bExisteDateExam = true ;

			string sUnite  = (*iter)->getUnitSens() ;
			string sFormat = (*iter)->getLexiqueSens() ;
			string sValeur = (*iter)->getComplement() ;

			if (string("2DA02") != sUnite)
      	(*iter)->setUnit("2DA021") ;

      if ((sFormat == "") || (sFormat[0] != '£') || ((sFormat[1] != 'D') && (sFormat[1] != 'T')))
      	(*iter)->setLexique("£T0;10") ;

      // if (strlen(sValeur.c_str()) > 8)
      //	sValeur = string(sValeur, 0, 8) ;

      if (sValeur != _Donnees._sDateExamen)
      {
      	sDateExam = _Donnees._sDateExamen ;
        if (strlen(sDateExam.c_str()) == 8)
        	sDateExam += string("000000") ;
        (*iter)->setComplement(sDateExam) ;
      }

      iter++ ;
    }
    // type de contenu (code lexique)
    else if (string("0TYPC") == sSens)
    {
    	iter++ ;
      bExisteTypeCont = true ;
      sTypeCont = "" ;

      while ((pPptMeta->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sTypeCont = (*iter)->getLexique() ;
        if (sTypeCont != _Donnees._sTypeContenu)
        	(*iter)->setLexique(_Donnees._sTypeContenu) ;

        iter++ ;
      }
    }
    else
    	iter++ ;
  }

  // Pour les informations dépendantes des données (pPatPathoArray)
	// on doit rajouter les infos manquantes au méta
  if ((false == bExisteDateExam) && (string("") != _Donnees._sDateExamen))
  {
    pPptMeta->ajoutePatho("KCHIR2", 1) ;
    sDateExam = _Donnees._sDateExamen ;
    sDateExam += string("000000") ;
    Message Msg ;
    Msg.SetUnit("2DA021") ;
    Msg.SetComplement(sDateExam.c_str()) ;
    pPptMeta->ajoutePatho("£T0;19", &Msg, 2) ;
  }

  if ((false == bExisteTypeCont) && (string("") != _Donnees._sTypeContenu))
  {
  	pPptMeta->ajoutePatho("0TYPC1", 1) ;
    pPptMeta->ajoutePatho(_Donnees._sTypeContenu, 2) ;
  }

	if ((false == bExisteAuthor) && (string("") != _Donnees._sAuteur))
  {
    pPptMeta->ajoutePatho("DAUTE1", 1) ;
    Message Msg ;
    Msg.SetComplement(_Donnees._sAuteur) ;
    pPptMeta->ajoutePatho("£SPID1", &Msg, 2) ;
  }

  // mise à jour des données de présentation
  if (false == _Pres.empty())
  {
  	iter = _Pres.begin() ;
    iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase))
    {
    	string sElemLex = (*iter)->getLexique() ;
      string sSens    = (*iter)->getLexiqueSens() ;

      // template
      if (string("0TPL0") == sSens)
      {
      	iter++ ;
        sTemplate = "" ;
        while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
        {
        	// on cherche ici un texte libre
          sElemLex = (*iter)->getLexique() ;
          if (sElemLex == string("£?????"))
          {
          	sTemplate = (*iter)->getTexteLibre() ;
            if (sTemplate != _Donnees._sTemplate)
            	(*iter)->setTexteLibre(_Donnees._sTemplate) ;
          }
          iter++ ;
        }
      }
      // en-tete
      else if (sSens == string("0ENTE"))
      {
      	iter++ ;
        sEnTete = "" ;
        while ((_Pres.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
        {
        	// on cherche ici un texte libre
          sElemLex = (*iter)->getLexique() ;
          if (sElemLex == string("£?????"))
          {
          	sEnTete = (*iter)->getTexteLibre() ;
            if (sEnTete != _Donnees._sEnTete)
            	(*iter)->setTexteLibre(_Donnees._sEnTete) ;
          }
          iter++ ;
        }
      }
      else
      	iter++ ;
    }
  }

	return true ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo::CommitMetaDonnees.", standardError) ;
	return false ;
}}boolNSDocumentInfo::InitDocumentBrut(NSDocumentInfo** ppDocument){try{
	if (true == ParseMetaDonnees())
	{
		*ppDocument = new NSDocumentInfo(*this) ;
    return true ;
	}
	return false ;
}
catch (...)
{
	erreur("Exception NSDocumentInfo::InitDocumentBrut.", standardError) ;
	return false ;
}}stringNSDocumentInfo::getDocTitleWithDate(){	string sLang = pContexte->getUserLanguage() ;
	string sDocName = getDocName() ;
  if (string("") == sDocName)
  {
  	string sContent = getContent() ;
    if (string("") != sContent)
  		pContexte->getDico()->donneLibelle(sLang, &sContent, &sDocName) ;
    if (string("") == sDocName)
  		sDocName = string("?") ;
  }  if (string("") != getDateExm())    sDocName += string(" - ") + donne_date(getDateExm(), sLang) ;
  return sDocName ;}
void
NSDocumentInfo::setContent(string sCo)
{
	if (string("") == sCo)
	{
  	_Donnees._sTypeContenu     = string("") ;
    _Donnees._sTypeContenuSens = string("") ;
  }

	string sCoSens = pContexte->getDico()->donneCodeSens(&sCo) ;
  if (sCoSens == sCo)
  {
  	_Donnees._sTypeContenuSens = sCo ;
    _Donnees._sTypeContenu     = sCo ;
    pContexte->getDico()->donneCodeComplet(_Donnees._sTypeContenu) ;
  }
  else
  {
		_Donnees._sTypeContenu     = sCo ;
    _Donnees._sTypeContenuSens = sCoSens ;
  }
}

void
NSDocumentInfo::setType(string sTy)
{
	if (string("") == sTy)
	{
  	_Donnees._sTypeComplet = string("") ;
    _Donnees._sType        = string("") ;
  }

	string sTySens = pContexte->getDico()->donneCodeSens(&sTy) ;
  if (sTySens == sTy)
  {
  	_Donnees._sType        = sTy ;
    _Donnees._sTypeComplet = sTy ;
    pContexte->getDico()->donneCodeComplet(_Donnees._sTypeComplet) ;
  }
  else
  {
		_Donnees._sTypeComplet = sTy ;
    _Donnees._sType        = sTySens ;
  }
}

// fin de nsdocinf.cpp
