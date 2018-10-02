//----------------------------------------------------------------------------// NSNoyauDocument : Père des documents NAUTILUS, il permet de leur attacher un
//						 pointeur sur un DocumentInfo.
//----------------------------------------------------------------------------
#define __NSDOCNOY_CPP

#ifndef _ENTERPRISE_DLL
  #include <owl\docmanag.h>
  #include <stdio.h>
  #include <classlib\filename.h>
  #include <sysutils.hpp>
#endif

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
  #include "partage\nsdivfile.h"
  #include "partage\nsperson.h"
  #include "nsoutil\nsoutdlg.h"
#endif

#include "nautilus\nshistdo.h"
#include "nautilus\nsbasimg.h"
#include "nsdn\nsdocnoy.h"
// #include "nsdn\nsintrad.h"

#ifndef _ENTERPRISE_DLL
  #include "nsdn\nsdn_dlg.h"
  #include "nsdn\nsdocdlg.h"
#else
  #include "nspr.h"
#endif

// #include "nsdn\nsdn.h"
#include "nsbb\nstlibre.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsfilgd.h"
// #include "nautilus\nsanxary.h"

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagnames.h"
#include "nssavoir\nsHealthTeam.h"
#include "nssavoir\nsTeamDefs.h"
long NSNoyauDocument::lObjectCount = 0 ;#ifndef _ENTERPRISE_DLL// Classe des paramètres d'impression
// pour WebBrowserPrint
NSPrintParams::NSPrintParams()
{
    reset();
}

NSPrintParams::NSPrintParams(const NSPrintParams& rv)
{
  // Page Setup dialog settings
  sPaperSize      = rv.sPaperSize;
  sPaperSource    = rv.sPaperSource;
  sHeader         = rv.sHeader;
  sFooter         = rv.sFooter;
  sOrientation    = rv.sOrientation;
  sfLeftMargin    = rv.sfLeftMargin;
  sfTopMargin     = rv.sfTopMargin;
  sfRightMargin   = rv.sfRightMargin;
  sfBottomMargin  = rv.sfBottomMargin;

  // Print dialog settings
  sPrinterName    = rv.sPrinterName;
  sbPrintToFile   = rv.sbPrintToFile;
  sPrintRange     = rv.sPrintRange;
  slPrintRangePagesFrom = rv.slPrintRangePagesFrom;
  slPrintRangePagesTo = rv.slPrintRangePagesTo;
  sbCollate       = rv.sbCollate;
  sPrintFrames    = rv.sPrintFrames;
  sbPrintLinks    = rv.sbPrintLinks;
  sbPrintLinkTable = rv.sbPrintLinkTable;
}

NSPrintParams::~NSPrintParams()
{
}

NSPrintParams&
NSPrintParams::operator=(const NSPrintParams& src)
{
  if (this == &src)
    return *this ;

  // Page Setup dialog settings
  sPaperSize      = src.sPaperSize;
  sPaperSource    = src.sPaperSource;
  sHeader         = src.sHeader;
  sFooter         = src.sFooter;
  sOrientation    = src.sOrientation;
  sfLeftMargin    = src.sfLeftMargin;
  sfTopMargin     = src.sfTopMargin;
  sfRightMargin   = src.sfRightMargin;
  sfBottomMargin  = src.sfBottomMargin;

  // Print dialog settings
  sPrinterName    = src.sPrinterName;
  sbPrintToFile   = src.sbPrintToFile;
  sPrintRange     = src.sPrintRange;
  slPrintRangePagesFrom = src.slPrintRangePagesFrom;
  slPrintRangePagesTo = src.slPrintRangePagesTo;
  sbCollate       = src.sbCollate;
  sPrintFrames    = src.sPrintFrames;
  sbPrintLinks    = src.sbPrintLinks;
  sbPrintLinkTable = src.sbPrintLinkTable;

  return *this;
}

/******************
intNSPrintParams::operator==(NSPrintParams& o){}********************/void
NSPrintParams::reset()
{
    // Page Setup dialog settings : initialisation à vide (prise de la valeur par défaut)
    sPaperSize = "", sPaperSource = "", sHeader = "", sFooter = "";
    sOrientation = "", sfLeftMargin = "", sfTopMargin = "", sfRightMargin = "", sfBottomMargin = "";

    // Print dialog settings : initialisation à vide (prise de la valeur par défaut)
    sPrinterName = "", sbPrintToFile = "", sPrintRange = "";
    slPrintRangePagesFrom = "", slPrintRangePagesTo = "", sbCollate = "";
    sPrintFrames = "", sbPrintLinks = "", sbPrintLinkTable = "";
}
#endif

// --------------------------------------------------------------------------// --------------------- METHODES DE NSNoyauDocument --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur
//
//  parent 	  	   : TDocument parent (?)
//	 pDocumentInfo : pointeur sur l'objet NSDocumentInfo qui
//						  permet de référencer le NSDocument
//	 pSuper		   : pointeur sur le superviseur
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
NSNoyauDocument::NSNoyauDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
                                 NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                                 bool bROnly, bool bClone)
                :TDocument(parent), NSRoot(pCtx), _PatPathoArray(pCtx->getSuperviseur())
#else
NSNoyauDocument::NSNoyauDocument(NSDocumentInfo* pDocumentInfo,
                                 NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                                 bool bROnly, bool bClone)
                :NSRoot(pCtx), _PatPathoArray(pCtx->getSuperviseur())
#endif
{
try
{
  lObjectCount++ ;

  _bReadOnly = bROnly ;
  _bIsClone  = bClone ;

	// on duplique le pDocumentInfo pour éviter sa destruction par
	// la ListBox des documents (ce qui n'est pas le cas du pDocHtmlInfo
	// qui est instancié par le superviseur)

	if (pDocumentInfo)
		_pDocInfo = new NSDocumentInfo(*pDocumentInfo) ;
	else
		_pDocInfo = (NSDocumentInfo*) 0 ;

	_pHtmlInfo = pDocHtmlInfo ;

	// pDonneesArray   = new NSPatPaDatArray(pContexte);
	// pLocalisArray   = new NSPatPaLocArray(pContexte);

	_sTemplate       = string("") ;	_sEnTete         = string("") ;
	_sBaseImages     = string("") ;
	_sBaseCompo      = string("") ;
	_sBaseTemp       = string("") ;
	_bConserveBase   = false ;
	_bEnregEnCours   = false ;
  _bAccountingChanged = false ;

	_sCodeDocMeta       = string("") ;
	_sCodeDocPres       = string("") ;
	_bCreerMetaLien     = false ;
	_bEnregPresentation = false ;

	if (_pDocInfo)
	{
  	if (pContexte->getSuperviseur()->typeDocument(_pDocInfo->getTypeSem(), NSSuper::isTree))
    {
    	// En N_TIERS, on récupère en général la patpatho depuis l'historique
      string sCodeDoc = _pDocInfo->getID() ;

      pContexte->getPatient()->getDocHis()->initFromPatPathoDocument(sCodeDoc, &_PatPathoArray) ;

			if (false == _PatPathoArray.empty())
				_bDocumentValide = true ;
			else
				_bDocumentValide = false ;
    }
  }

	// on met à jour le titre
	SetTitreDoc() ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument ctor.", standardError) ;
}
}

#ifndef _ENTERPRISE_DLL
NSNoyauDocument::NSNoyauDocument(TDocument* parent, NSContexte* pCtx, bool bClone)
                :TDocument(parent), NSRoot(pCtx), _PatPathoArray(pCtx->getSuperviseur())
#else
NSNoyauDocument::NSNoyauDocument(NSContexte* pCtx, bool bClone)
                :NSRoot(pCtx), _PatPathoArray(pCtx->getSuperviseur())
#endif
{
try
{
  lObjectCount++ ;

	_bReadOnly = true ;
  _bIsClone  = bClone ;

	_pDocInfo  = (NSDocumentInfo*) 0 ;
	_pHtmlInfo = (NSDocumentInfo*) 0 ;

  _sTemplate          = string("") ;  _sEnTete            = string("") ;
  _sBaseImages        = string("") ;
  _sBaseCompo         = string("") ;
  _sBaseTemp          = string("") ;
  _bConserveBase      = false ;
  _bEnregEnCours      = false ;
  _bAccountingChanged = false ;

	_sCodeDocMeta       = string("") ;
	_sCodeDocPres       = string("") ;
	_bCreerMetaLien     = false ;
	_bEnregPresentation = false ;

	_bDocumentValide    = true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument ctor.", standardError) ;
}
}

//---------------------------------------------------------------------------//   Destructeur
//---------------------------------------------------------------------------
NSNoyauDocument::~NSNoyauDocument()
{
try
{
	if (_pDocInfo)
  {
    if (string("") != _pDocInfo->getDocName())
    {
      string ps = string("Deleting kernel for document \"") + _pDocInfo->getDocName() + string("\"") ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    }

    // Make certain that this document is no longer indexed in history
    //
#ifdef _ENTERPRISE_DLL
    if (false == _bIsClone)
#else
    if ((false == _bIsClone) && pContexte && pContexte->getPatient() && (false == pContexte->getPatient()->_bCanCloseHisto))
#endif
    {
      NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
      if (pDocHis)
      {
        NSNoyauDocument* pDocNoy = pDocHis->EstUnDocumentOuvert(_pDocInfo) ;
        if (pDocNoy)
          pDocHis->FermeDocumentOuvert(_pDocInfo) ;

#ifndef _ENTERPRISE_DLL
        if (false == pContexte->getPatient()->isClosing())
        {
          DocumentIter itDoc = pDocHis->TrouveDocHisto(_pDocInfo->getID()) ;
          if (NULL != itDoc)
          {
            string ps2 = "Deleting a document still indexed in History." ;
            pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trWarning) ;
          }
        }
#endif
      }
    }

		delete _pDocInfo ;
  }
	if (_pHtmlInfo)
		delete _pHtmlInfo ;

  // destruction de la base d'images et de la base temporaire
  if ((false == _bConserveBase) && (string("") != _sBaseImages)) // si la base a été initialisée
  {
  	NSBaseImages Base(_sBaseImages) ;
    Base.lire() ;
    Base.detruire() ;
  }

  if (string("") != _sBaseTemp)  // si la base a été initialisée
  {
  	NSBaseImages Base(_sBaseTemp) ;
    Base.lire() ;
    Base.detruire() ;
  }

  lObjectCount-- ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument destructor.", standardError) ;
}
}

// Constructeur copieNSNoyauDocument::NSNoyauDocument(const NSNoyauDocument& rv)
#ifndef _ENTERPRISE_DLL
                :TDocument(rv.GetParentDoc()), NSRoot(rv.pContexte), _PatPathoArray(rv._PatPathoArray)
#else
                :NSRoot(rv.pContexte), _PatPathoArray(rv._PatPathoArray)
#endif
{
try
{
  lObjectCount++ ;

  _bReadOnly = rv._bReadOnly ;

	if (rv._pDocInfo)
  	_pDocInfo = new NSDocumentInfo(*(rv._pDocInfo)) ;
	else
		_pDocInfo = (NSDocumentInfo*) 0 ;

	if (rv._pHtmlInfo)
		_pHtmlInfo = new NSDocumentInfo(*(rv._pHtmlInfo)) ;
	else
		_pHtmlInfo = (NSDocumentInfo*) 0 ;

	_bDocumentValide    = rv._bDocumentValide ;

	_sCodeDocMeta       = rv._sCodeDocMeta ;
  _sCodeDocPres       = rv._sCodeDocPres ;
	_bCreerMetaLien     = rv._bCreerMetaLien ;
  _bEnregPresentation = rv._bEnregPresentation ;

	_sTemplate          = rv._sTemplate ;
	_sEnTete            = rv._sEnTete ;
	_sBaseImages        = string("") ;
	_sBaseCompo         = string("") ;
	_sBaseTemp          = string("") ;
	_bConserveBase      = false ;
	_bEnregEnCours      = false ;
  _bAccountingChanged = false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument copy ctor.", standardError) ;
}
}

// Operateur =// Pb : comment affecter le document parent destination ?????????????????
NSNoyauDocument&
NSNoyauDocument::operator=(const NSNoyauDocument& src)
{
try
{
	if (this == &src)
		return *this ;

	_bReadOnly = src._bReadOnly ;

	pContexte = src.pContexte ;

  if (_pDocInfo)
    delete _pDocInfo ;
	if (src._pDocInfo)
  	_pDocInfo = new NSDocumentInfo(*(src._pDocInfo)) ;
	else
  	_pDocInfo = (NSDocumentInfo*) 0 ;

  if (_pHtmlInfo)
    delete _pHtmlInfo ;
	if (src._pHtmlInfo)
		_pHtmlInfo = new NSDocumentInfo(*(src._pHtmlInfo)) ;
	else
  	_pHtmlInfo = (NSDocumentInfo*) 0 ;

	_PatPathoArray      = src._PatPathoArray ;
	_bDocumentValide    = src._bDocumentValide ;

	_sCodeDocMeta       = src._sCodeDocMeta ;
  _sCodeDocPres       = src._sCodeDocPres ;
	_bCreerMetaLien     = src._bCreerMetaLien ;
  _bEnregPresentation = src._bEnregPresentation ;

	_sTemplate          = src._sTemplate ;
	_sEnTete            = src._sEnTete ;
	_sBaseImages        = string("") ;
	_sBaseCompo         = string("") ;
	_sBaseTemp          = string("") ;
	_bConserveBase      = false ;
	_bEnregEnCours      = false ;
  _bAccountingChanged = false ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument = operator.", standardError) ;
  return *this ;
}
}

// Fonction de maj du titre du document en fonction du pDocInfo//// MODIFIER//void
NSNoyauDocument::SetTitreDoc()
{
try
{
  string sAffiche = string("") ;

	bool bIsDocPermanent = false ;

	string sLang = pContexte->getUserLanguage() ;

	if (false == _PatPathoArray.empty())
	{
		PatPathoIter iter = _PatPathoArray.begin() ;
    string sCodeSens = (*iter)->getLexiqueSens() ;
    if ((sCodeSens == "ZSYNT") || (sCodeSens == "ZADMI"))
    	bIsDocPermanent = true ;
	}

	if ((NSDocumentInfo *) NULL != _pDocInfo)
	{
		sAffiche = _pDocInfo->getDocName() ;

		if (string("") != sAffiche)
    {
      strip(sAffiche, stripBoth) ;

      if (false == bIsDocPermanent)
      {
        string sDateDoc = string("") ;

      	// si la patpatho n'est pas vide, on stocke dans le titre la date patpatho
        if (false == _PatPathoArray.empty())
        {
        	string sDate = _PatPathoArray.GetDocDate() ;

          if (string("") != sDate)
          	sDateDoc = sDate ;
          else
          	sDateDoc = _pDocInfo->getCreDate() ;

        }
        else // sinon on prend la date de creation
        	sDateDoc = _pDocInfo->getCreDate() ;

        // la date est concaténée au nom du document
        if (string("") != sDateDoc)
          sAffiche += string(" du ") + donne_date(sDateDoc, sLang) ;
      }
		}
    //
    // En mode navigation web, le créateur du document est vide
    //
    if ((false == bIsDocPermanent) && (string("") != _pDocInfo->getCreator()))
    {
    	string sNss = _pDocInfo->getCreator() ;
#ifndef _ENTERPRISE_DLL
      NSPersonInfo* pUtil = pContexte->getPersonArray()->getPerson(pContexte, sNss, pidsUtilisat) ;
      string sCreateur = pUtil->getCivilite() ;
      sAffiche += string(" - ") + sCreateur ;
#endif
    }
	}

	//
	// Instancie la donnee Title de TDocument
	//
#ifndef _ENTERPRISE_DLL
  SetTitle(sAffiche.c_str()) ;
/*
  if (string("") == sAffiche)
    SetTitle("") ;
  else
  {
    char* pTitle = new char[strlen(sAffiche.c_str()) + 1] ;
    strcpy(pTitle, sAffiche.c_str()) ;
    SetTitle(pTitle) ;
    delete[] pTitle ;
  }
*/
#endif
}
catch (...)
{
	erreur("Exception NSNoyauDocument SetTitreDoc.", standardError) ;
}
}

// Fonction qui renvoie la date stockée dans la PatPatho des datas en MUE
// au format jj/mm/aaaa ou au format Nautilus (aaaammjj) selon bDateClaire
string
NSNoyauDocument::GetDateExamen(bool bDateClaire)
{
try
{
	string sDate = _PatPathoArray.GetDocDate() ;
	if ((false == bDateClaire) || (string("") == sDate))
		return sDate ;

	string sLang = pContexte->getUserLanguage() ;

  string sMessage ;
	string sIntro ;
	donne_date_claire(sDate, &sMessage, &sIntro, sLang) ;

	if (string("") != sIntro)
		sDate = sIntro + string(" ") + sMessage ;
	else
		sDate = sMessage ;

	return sDate ;
}
catch (...)
{
	erreur("Exception GetDateExamen.", standardError) ;
	return "" ;
}
}

// Fonction qui remet à jour la pDocInfo en fonction des données
// issues de la patpatho du document (aujourd'hui date d'examen et type de contenu)
// Si la pDocInfo est remise à jour, renvoie true, false sinon.
bool
NSNoyauDocument::DocInfoModified()
{
  if (NULL == _pDocInfo)
    return false ;

	bool bIsModified = false ;

	// date examen valide
	string sDateExamen = GetDateExamen(false) ;
  // Type de contenu : analyse de la patpatho en cours
  string sLexique = string("") ;

  if (false == _PatPathoArray.empty())
  {
		PatPathoIter iter = _PatPathoArray.begin() ;
    sLexique = (*iter)->getLexique() ;
  }
  else if (string("") != _pDocInfo->getTypeSem())
  {
  	switch ((_pDocInfo->getTypeSem())[0])
    {
    	// dans les 3 cas qui restent, les document n'ont pas patpatho, on leur
      // en crée une qui contient seulent le code lexique correspondant :
      // ZIMA01 pour image
      // PXXX51 : code bidon pour texte
      // PTRIQ3 : pour HTML

      case 'T' :  // pour tous les types de texte
      	sLexique = string("PXXX51") ;
        break ;

      case 'H' :  // pour tous les types d'html statiques (HD traité plus haut)
      	sLexique = string("PTRIQ3") ;
        break ;

      case 'I' :  // cas des images : générique pour tous les types d'image
         			    // y compris les images animées (vidéos)
      	sLexique = string("ZIMA01") ;
        break ;
    }
  }

	if ((string("") != sDateExamen) && (_pDocInfo->getDateExm() != sDateExamen))
  {
		_pDocInfo->setDateExm(sDateExamen) ;
    bIsModified = true ;
	}

	if ((string("") != sLexique) && (_pDocInfo->getContent() != sLexique))
	{
		_pDocInfo->setContent(sLexique) ;
    bIsModified = true ;
	}

	return bIsModified ;
}

//  +-----------------------------------------------------------------+//  ¦                    Calcul de l'âge du patient                   ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
int
NSNoyauDocument::donne_age(const string sDatex)
{
	string sDateNaiss = string("00000000") ;
	//
	// On ne calcule l'âge que si on a la date de naissance
	//
#ifndef _ENTERPRISE_DLL
  sDateNaiss = pContexte->getPatient()->donneNaissance() ;
#else
  // if (!pContexte->getPerson()->donneNaissance(dateNaiss))
  //   return -1 ;
#endif

	if (string("00000000") == sDateNaiss)
		return -1 ;

  return ::donne_age(sDatex, sDateNaiss) ;

	//
	// Année de l'examen
	//
/*
  unsigned int i, j;
	signed   int age ;
  unsigned int mois_nais, mois_jour, jour_nais, jour_jour ;

	i = 10 * donneDigit(datex[0]) + donneDigit(datex[1]) ;
	i = (i - 18) * 100 ;
	i += 10 * donneDigit(datex[2]) + donneDigit(datex[3]) ;
	//
	// Année de naissance du patient
	//
	j = 10 * donneDigit(dateNaiss[0]) + donneDigit(dateNaiss[1]) ;
	j = (j - 18) * 100 ;
	j += 10 * donneDigit(dateNaiss[2]) + donneDigit(dateNaiss[3]) ;
	//
	// Age qu'aura le patient dans l'année
	//
	age = i - j ;
	//
	// Correctifs en fonction de mois et jour
	//
	mois_nais = 10 * donneDigit(dateNaiss[4]) + donneDigit(dateNaiss[5]) ;
	jour_nais = 10 * donneDigit(dateNaiss[6]) + donneDigit(dateNaiss[7]) ;
	mois_jour = 10 * donneDigit(datex[4]) + donneDigit(datex[5]) ;
	jour_jour = 10 * donneDigit(datex[6]) + donneDigit(datex[7]) ;
	if ((mois_jour < mois_nais) ||
		 ((mois_jour == mois_nais) && (jour_jour < jour_nais)))
		age-- ;

	if ((age < 0) || (age > 150))
		return -1 ;

	return age ;
*/
}

//  +-----------------------------------------------------------------+
//  ¦              Donne l'intitulé (Monsieur, Madame...)             ¦
//  +-----------------------------------------------------------------+
//  Créé le 17/07/1995 Dernière mise à jour 17/07/1995
//
// MODIFIER
//
void
NSNoyauDocument::donne_intitule_patient(string *intitule, int age)
{
  if ((string*) NULL == intitule)
    return ;

#ifndef _ENTERPRISE_DLL
	if (pContexte->getPatient()->estFeminin())
#else
	if (pContexte->getPatient()->isFemale())
#endif
		*intitule = string("Madame ") ;
	else
		*intitule = string("Monsieur ") ;
	//
	// Correctifs en fonction de l'age
	//
	if ((age < 0) || (age > 150))
    return ;
	if (age <= 15)
	{
		*intitule = string("l'enfant ") ;
		return ;
	}

#ifndef _ENTERPRISE_DLL
	if ((pContexte->getPatient()->estFeminin()) && (age < 18))
#else
  if ((pContexte->getPatient()->isFemale()) && (age < 18))
#endif
		*intitule = string("Mademoiselle ") ;
}

////////////////////////////////////////////////////////////////bool
NSNoyauDocument::CanClose()
{
	if (_bEnregEnCours)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "thisDocumentIsBeingSaved") ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}
	else
#ifndef _ENTERPRISE_DLL
		return (false == IsDirty()) ;
#else
    return true ;
#endif
}

// ---------------------------------------------------------------------------//   Instancie pPatPathoArray
//
//   Va chercher les données dans PATPADAT et les localisations correspondant
//	  à la transaction la plus récente dans PATPALOC
//	  Reconstitue la patpatho à partir de ces deux arrays
// ---------------------------------------------------------------------------
bool
NSNoyauDocument::chargePatPatho()
{
  if ((NULL == pContexte->getPatient()) || (NULL == _pDocInfo))
    return false ;

try
{
  string sCodeDoc = _pDocInfo->getID() ;
  if (string("") == sCodeDoc)
  {
  	erreur("Code document vide dans chargePatPatho().", standardError) ;
		return false ;
	}

	// Note importante : dans un document on utilise obligatoirement le graphe du patient
  //
  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;

  if (NULL == pGraphManager)
    return false ;

	string sRosace = string("") ;
	return pGraphManager->getTree(sCodeDoc, &_PatPathoArray, &sRosace) ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::chargePatPatho.", standardError) ;
	return false ;
}
}

bool
NSNoyauDocument::enregistrePatPatho()
{
try
{
#ifdef _ENTERPRISE_DLL
  if ((Person*) NULL == pContexte->getPatient())
#else
  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
#endif
    return false ;

	string ps2 = string("NSNoyauDocument::enregistrePatPatho : begin") ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

	if (_bReadOnly)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "readOnlyDoc") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	if (_PatPathoArray.empty())
  {
    ps2 = string("NSNoyauDocument::enregistrePatPatho : empty patPatho, leaving") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
		return true ;
  }

	// Note importante : dans un document on utilise obligatoirement le graphe du patient
  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
  {
    ps2 = string("NSNoyauDocument::enregistrePatPatho : can't find GraphManager, leaving") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }

	NSLinkManager* pLink = pGraphManager->getLinkManager() ;

#ifndef _ENTERPRISE_DLL
  ps2 = pContexte->getSuperviseur()->getText("documentManagementStatus", "documentBeingSaved") ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;
	pContexte->getSuperviseur()->afficheStatusMessage((char*)ps2.c_str()) ;
#endif

  if ((NSDocumentInfo*) NULL == _pDocInfo)
  {
    ps2 = string("NSNoyauDocument::enregistrePatPatho : pDocInfo is NULL, leaving") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }

#ifndef _ENTERPRISE_DLL
  NSProgressDlg progressDialog(pContexte->GetMainWindow(), pContexte) ;
  progressDialog.Create() ;
  progressDialog.setCaptionText(string("Enregistrement des données")) ;
  progressDialog.ShowWindow(SW_SHOW) ;
#endif

	// Phase de remise à jour des méta-données :
	// Si on a une patpatho non vide, on vérifie si il faut updater les metas
	// en testant pDocInfo->sCodeDocMeta
  bool bForceUpdateMeta = false ;

	if ((string("") != _pDocInfo->getCodeDocMeta()) && (DocInfoModified()))
  {
    ps2 = string("NSNoyauDocument::enregistrePatPatho : before CommitMetaDonnees") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
    progressDialog.updateTaskText(string("Mise à jour de l'étiquette")) ;
    progressDialog.setPosition(10) ;
#endif

		_pDocInfo->CommitMetaDonnees() ;

    bForceUpdateMeta = true ;
  }

	//
	// On établit ici le lien des données (patpatho en cours) avec le méta-document s'il existe	//	if ((_bCreerMetaLien) && (string("") != _sCodeDocMeta))	{  	string sNodeMeta = _sCodeDocMeta ;		string sNodeData = _pDocInfo->getID() ;		if (false == pLink->etablirLien(sNodeMeta, NSRootLink::docData, sNodeData))		{
      ps2 = string("Impossible d'initialiser le lien vers le document data.") ;
      pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
			erreur(ps2.c_str(), standardError) ;
			return false ;
		}		_bCreerMetaLien = false ;	}  /******************************** travail fait maintenant par signChanges	else if (sCodeDocMeta != "")	{		// Note : le lien contributionModified ne peut s'établir que la deuxième fois		// qu'on enregistre les données, car la première fois bCreerMetaLien est true.		string sNodeMeta = sCodeDocMeta;		string sContribution = pContexte->getPatient()->getContribution();		if (!pLink->existeLien(sNodeMeta, NSRootLink::contributionModified, sContribution))		{			// lien du méta vers la contribution en cours			if (!pLink->etablirLien(sNodeMeta, NSRootLink::contributionModified, sContribution))				erreur("Impossible d'initialiser le lien du document vers la contribution en cours du patient.", standardError, 0) ;		}	}
    ********************************************************************************/

  pGraphManager->initAttributesToCore() ;

  // on doit récupérer les traits du patient dans la patpatho, qui a pu changer
  // et non dans le patient en cours, qui est mis à jour par la suite.
  //
  if ((false == _PatPathoArray.empty()) && ((*(_PatPathoArray.begin()))->getLexique() == "ZADMI1"))
  {
    ps2 = string("NSNoyauDocument::enregistrePatPatho : before ChargeDonneesAdmin") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
    progressDialog.updateTaskText(string("Adaptation des données administratives")) ;
    progressDialog.setPosition(20) ;
#endif

    string sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail ;
  	pGraphManager->ChargeDonneesAdmin(pContexte, &_PatPathoArray, sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail) ;

    // Modification des traits
    //
    ps2 = string("NSNoyauDocument::enregistrePatPatho : before setAdminAttrArray") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
    progressDialog.updateTaskText(string("Adaptation des traits de recherche du patient")) ;
    progressDialog.setPosition(30) ;
#endif

    pGraphManager->setAdminAttrArray(NSPersonGraphManager::attribsChange, sNom, sPrenom, sSexe, sNaissance) ;
  }

/* // *** Traits were always proposed for modification whatever was saved

  else
  {
#ifndef _ENTERPRISE_DLL
  	sNom       = pContexte->getPatient()->getNom() ;
    sPrenom    = pContexte->getPatient()->getPrenom() ;
    sSexe      = pContexte->getPatient()->getSexe() ;
    sNaissance = pContexte->getPatient()->getNaissance() ;
#else
  	sNom       = pContexte->getPatient()->getFamilyName() ;
    sPrenom    = pContexte->getPatient()->getFirstName() ;
    sSexe      = pContexte->getPatient()->getGender() ;
    sNaissance = pContexte->getPatient()->getBirthDate() ;
#endif
  }

  // Modification des traits
  //
  ps2 = "NSNoyauDocument::enregistrePatPatho : before setAdminAttrArray" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
  progressDialog.updateTaskText(string("Adaptation des traits de recherche du patient")) ;
  progressDialog.setPosition(30) ;
#endif

  pGraphManager->setAdminAttrArray(NSPersonGraphManager::attribsChange, sNom, sPrenom, sSexe, sNaissance) ;
*/

  // Vecteur d'update contenant la patpatho en cours et éventuellement les métas
	NSVectPatPathoArray VectUpdate ;

	// on stocke maintenant le document dans le vecteur d'update avec ses metas
	// et on envoie le tout au pilote
	if ((string("") != _sCodeDocMeta) || bForceUpdateMeta)
  {
    NSPatPathoArray *pMetaCopy = new NSPatPathoArray(pContexte->getSuperviseur()) ;
    _pDocInfo->initFromMeta(pMetaCopy) ;
  	VectUpdate.push_back(pMetaCopy) ;
  }

	// On redonne aux noeuds leurs informations de patient et document
	for (PatPathoIter i = _PatPathoArray.begin() ; _PatPathoArray.end() != i ; i++)
	{
		(*i)->setPerson(_pDocInfo->getPatient()) ;
		(*i)->setDocum(_pDocInfo->getDocument()) ;
	}

	VectUpdate.push_back(new NSPatPathoArray(_PatPathoArray)) ;

  ps2 = "NSNoyauDocument::enregistrePatPatho : before updateTrees" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

#ifndef _ENTERPRISE_DLL
  progressDialog.updateTaskText(string("Enregistrement des données")) ;
  progressDialog.setPosition(50) ;
#endif

  string sTreeID ;
	pGraphManager->updateTrees(pContexte, &VectUpdate, &sTreeID, &_sCodeDocMeta, pidsPatient) ;

	if (string("") == sTreeID)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "failedToSave") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

  // commit de l'arbre de présentation (one-shot)
  // sCodeDocPres est comme sCodeDocMeta une variable de la classe NoyauDocument
  if (_bEnregPresentation)
  {
  	pGraphManager->commitGraphTree(pContexte, _sCodeDocPres) ;
    _pDocInfo->setCodeDocPres(_sCodeDocPres) ;

    string          sRightsPres ;
    NSPatPathoArray PatPathoNewPres(pContexte->getSuperviseur()) ;
    pGraphManager->getTree(_sCodeDocPres, &PatPathoNewPres, &sRightsPres) ;
    _pDocInfo->setPres(&PatPathoNewPres) ;

    _bEnregPresentation = false ;
  }

	// pGraphManager->setAdminAttrArray(NSPersonGraphManager::attribsUpdate, sNom, sPrenom, sSexe, sNaissance) ;

	ps2 = "NSNoyauDocument::enregistrePatPatho : before getTree" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

#ifndef _ENTERPRISE_DLL
  progressDialog.updateTaskText(string("Chargement des données enregistrées")) ;
  progressDialog.setPosition(70) ;
#endif

	// Remise à jour des patpatho méta et data
  //
  updateAfterSave(sTreeID, _sCodeDocMeta) ;

#ifndef _ENTERPRISE_DLL
  ps2 = pContexte->getSuperviseur()->getText("documentManagementStatus", "documentSaved") ;
	pContexte->getSuperviseur()->afficheStatusMessage((char*)ps2.c_str()) ;
#endif

/* Now done by NSHISTODocument::Rafraichir because data are fetched into
   VectDocument and this repository is precisely updated by Rafraichir

#ifndef _ENTERPRISE_DLL
  if (NULL != pContexte->getBBinterface())
  {
    progressDialog.updateTaskText(string("Présentation des données au Blackboard")) ;
    progressDialog.setPosition(90) ;

		pContexte->getBBinterface()->signalThatPatpathoWasSaved(pPatPathoArray, false) ;
  }
#endif
*/

#ifndef _ENTERPRISE_DLL
  progressDialog.updateTaskText(string("Enregistrement terminé")) ;
  progressDialog.setPosition(100) ;

  progressDialog.Destroy() ;
  delete progressDialog ;

	ps2 = "NSNoyauDocument::enregistrePatPatho : done" ;
  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;
#endif

	return true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::enregistrePatPatho.", standardError) ;
	return false ;
}
}

void
NSNoyauDocument::updateAfterSave(string sTreeID, string sMetaID)
{
  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
    return ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return ;

  if (string("") != sMetaID)
	{
		_pDocInfo->setCodeDocMeta(sMetaID) ;

    string          sRightsMeta ;
    NSPatPathoArray PatPathoNewMeta(pContexte->getSuperviseur()) ;
		pGraphManager->getTree(sMetaID, &PatPathoNewMeta, &sRightsMeta) ;
    _pDocInfo->setMeta(&PatPathoNewMeta) ;
	}

  // Updating the patpatho
  //
  string sRightsData ;
	pGraphManager->getTree(sTreeID, &_PatPathoArray, &sRightsData) ;

	// Remise à jour de la pDocInfo
	_pDocInfo->setPatient(string(sTreeID, 0, PAT_NSS_LEN)) ;
	_pDocInfo->setDocument(string(sTreeID, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN)) ;
}

boolNSNoyauDocument::DocumentInvisible(string sCodeMeta)
{
try
{
  if (_PatPathoArray.empty())
    return false ;

  PatPathoIter iterDoc = _PatPathoArray.begin() ;
  (*iterDoc)->setVisible("0") ;  return enregistrePatPatho() ;}
catch (...)
{
	erreur("Exception NSNoyauDocument::DocumentInvisible.", standardError) ;
	return false ;
}
}

bool
NSNoyauDocument::DocumentVisible(string /* sCodeMeta */)
{
try
{
	if (_PatPathoArray.empty())
    return false ;

  PatPathoIter iterDoc = _PatPathoArray.begin() ;
  (*iterDoc)->setVisible("1") ;  return enregistrePatPatho() ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::DocumentVisible.", standardError) ;
	return false ;
}
}

//---------------------------------------------------------------------------
//  Fonction:		NSRefDocument::CreeDocument()
//  Description:	Crée et référence un document en mode N_TIERS
//  Retour:			True si ça réussit, False sinon
//---------------------------------------------------------------------------
bool
NSNoyauDocument::CreeDocument(NSDocumentInfo* pDoc, int typeLink,
                              string& sCodeChemise, bool bVerbose,
                              NSPersonGraphManager* pGraphManager,
                              bool bMustConnectToFolder)
{
try
{
  if (NULL == pDoc)
    return false ;

	// on récupère en MUE un node chemise
  //
	string sNumChemise = string("") ;

	// NOTE : En MUE, CreeDocument ne sert qu'à lier une chemise au document
	// la création du document est faite plus tard, lorsqu'on enregistre la patpatho
	// dans Referencer. Cas particulier : les documents Root ne sont liés à aucune
	// chemise. On considère que les chemises ne contiennent que les nouveaux documents

	// le rootDoc pour la racine du graphe patient
  //
	string sRootDocGraph = string(1, INMEMORY_CHAR) + string("00000") ;
	if ((pDoc->getDocument() == sRootDocGraph) ||
        (NSRootLink::personHealthIndex    == typeLink) ||
        (NSRootLink::personRiskManager    == typeLink) ||
        (NSRootLink::personSocialIndex    == typeLink) ||
        (NSRootLink::personIndexExtension == typeLink) ||
        (NSRootLink::personSynthesis      == typeLink) ||
        (NSRootLink::personAdminData      == typeLink) ||
        (NSRootLink::personFolderLibrary  == typeLink) ||
        (NSRootLink::personIdentifiers    == typeLink) ||
        (NSRootLink::personHealthTeam     == typeLink))
		return true ;

	string sRights = pDoc->getRights() ;

	//	// Appel de la boite de dialogue de référencement
	//
  NSDocumentData DocData ;
  pDoc->initFromData(&DocData) ;

#ifndef _ENTERPRISE_DLL
	if (bVerbose)
	{
		EnregDocDialog* pEnregDocDlg =
		            new EnregDocDialog(pContexte->GetMainWindow(), &DocData,
                                            sNumChemise, sRights, pContexte) ;

		int retVal = pEnregDocDlg->Execute() ;
		delete pEnregDocDlg ;

		if (IDCANCEL == retVal)			return false ;

    pDoc->setData(&DocData) ;
	}
	else
	{
    if (bMustConnectToFolder)
    {
		  EnregDocDialog* pEnregDocDlg =
		        new EnregDocDialog(0, &DocData, sNumChemise, sRights, pContexte) ;

		  //
		  // Valeurs par défaut : Dernière chemise et importance de base
		  // Default values : Last filer and basic interest
		  //
		  pEnregDocDlg->RemplirChemises() ;

		  if (false == pEnregDocDlg->_aChemisesArray.empty())		  {
			  NSChemiseInfo* pChemInfo = pEnregDocDlg->_aChemisesArray.back() ;
			  sNumChemise = pChemInfo->_sNodeChemise ;
		  }

		  delete pEnregDocDlg ;      pDoc->setData(&DocData) ;    }    pDoc->setInteret(string("A")) ;	}
#endif // #ifndef _ENTERPRISE_DLL

	//	// Vérification de la présence des éléments obligatoires
	//
	if ((string("") == pDoc->getDocName()) ||
		  (string(strlen(pDoc->getDocName().c_str()), ' ') == pDoc->getDocName()))
		return false ;

	if (string("") == sNumChemise)		return (false == bMustConnectToFolder) ;
	// on conserve le code chemise choisi pour l'importation de fichiers  //  sCodeChemise = sNumChemise ;

	//	// La recherche du code à attribuer à ce nouveau document
	// est faite après CreeDocument (on lie ici le méta à la chemise sélectionnée)

	// Remarque : à ce niveau, le code document est celui du méta-document (cf referencer)
	// on établit le lien entre la chemise et le nouveau document	// la chemise-corbeille ne peut pas être sélectionnée par construction (cf EnregDocDialog)	NSLinkManager* pLink = pGraphManager->getLinkManager() ;	bool bRet = pLink->etablirLien(sNumChemise, NSRootLink::docFolder, _sCodeDocMeta) ;	return bRet ;}#ifndef _ENTERPRISE_DLLcatch(TWindow::TXWindow& e){
  string sErr = string("Exception NSNoyauDocument::CreeDocument : ") + e.why() ;
  erreur(sErr.c_str(), standardError) ;
  return false ;
}#endifcatch (...)
{
  erreur("Exception NSNoyauDocument::CreeDocument.", standardError) ;
  return false ;
}
}

// ---------------------------------------------------------------------------
//   Inscrit le document dans le fichier DOCUMENT
//
//   Lance la boite de dialogue de référencement (choix du titre,
//	 de la chemise, de l'importance du document)
//
//	  Sauvegarde le document dans DOCUMENT//	  Crée un enregistrement dans CHEMDOC pour le ranger dans sa chemise
// ---------------------------------------------------------------------------

// Referencement d'un document dans la base des documents en mode N_TIERS
// (on travaille avec un graphManager qui par défaut est le graphe du patient)// ---------------------------------------------------------------------------bool
NSNoyauDocument::Referencer(string typeDoc, string nomDoc, string nomFichier,
                          string cheminDoc, bool bDocVisible, bool bVerbose,
                          string sCodeDoc, NSRootLink::NODELINKTYPES typeLink,
                          NSPersonGraphManager* pGraphManager,
                          string sAuthorId, string tmplDoc, string enteteDoc,
                          string sDestinataire, string sCreationDate,
                          string sMasterDoc, NSRootLink::NODELINKTYPES masterLink,
                          bool bMustConnectToFolder,
                          NSPatPathoArray* pSpecificInfoPatPatho)
{
try
{
	// Initialisation du GraphManager
	if ((NSPersonGraphManager*) NULL == pGraphManager)		pGraphManager = pContexte->getPatient()->getGraphPerson() ;	// Création d'une fiche Document (comme objet d'interface)
	//
	NSDocumentInfo NewDocument(pContexte) ;

  // Set basic information to the document  //  initDocumentInfo(&NewDocument, typeDoc, sAuthorId, bDocVisible, sDestinataire, sCreationDate) ;	// on fixe la template et l'en-tete des CN et des CS, sauf pour les documents root  //
  NSPatPathoArray PatPathoPresent(pContexte->getSuperviseur()) ;
  createPresentationInfo(&NewDocument, &PatPathoPresent, sCodeDoc, typeLink, typeDoc, tmplDoc, enteteDoc) ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	// le chemin et le nom de fichier sont vides pour les documents dynamiques
	NewDocument.setLocalisa(cheminDoc) ;
	NewDocument.setFichier(nomFichier) ;

	//
	// Demande de création et référencement du document
	//

	// Copie du nom du document par défaut	NewDocument.setNom(nomDoc) ;

	//
	// Calcul du code document des méta-données
	//
  _sCodeDocMeta = getMetaId(pGraphManager, &NewDocument, sCodeDoc) ;

	//	// NOTE : Il s'agit ici nécéssairement d'un NOUVEAU document
	// et le booléen bCreerMetaLien est là pour ne pas oublier
	// d'enregistrer ultérieurement (dans enregistrePatPatho en principe)
	// les datas à travers le graphe, ainsi que de créer le lien méta-data.
	//
	_bCreerMetaLien = true ;

  string sCodeChemise ;

  // Setting document's semantic type from patpatho
  //
  initMetaContentFromPpt(&NewDocument) ;

	// Lancement du dialogue de référencement
  //
	bool documentCree = CreeDocument(&NewDocument, typeLink, sCodeChemise, bVerbose, pGraphManager, bMustConnectToFolder) ;
	if (false == documentCree)
		return false ;

	//
	// On crée maintenant une patpatho spécifique pour enregistrer les méta-données
	//
	NSPatPathoArray PatPathoMeta(pContexte->getSuperviseur()) ;
  createMetaPatpatho(&NewDocument, &PatPathoMeta, sAuthorId, typeDoc) ;

  if (pSpecificInfoPatPatho && (false == pSpecificInfoPatPatho->empty()))
    PatPathoMeta.InserePatPatho(PatPathoMeta.end(), pSpecificInfoPatPatho, 1) ;

  setContent(&NewDocument, &PatPathoMeta, typeDoc) ;

	//
  // Création des pDocInfo du document en cours avec les nouvelles informations
  //
  if (_pDocInfo)
    delete _pDocInfo ;

  _pDocInfo = new NSDocumentInfo(NewDocument) ;
	// On place ici l'arbre des méta-données dans le graphe  pGraphManager->setTree(&PatPathoMeta, NewDocument.getRights(), _sCodeDocMeta) ;	// On conserve la patpatho méta et le sCodeMeta dans la pDocInfo du document en cours	_pDocInfo->setMeta(&PatPathoMeta) ;	_pDocInfo->setCodeDocMeta(_sCodeDocMeta) ;	//	// On crée la liaison du méta-document avec le document root	//	string sNodeRoot = pGraphManager->getRootTree() ;	string sNodeMeta = _sCodeDocMeta ;	NSLinkManager* pLink = pGraphManager->getLinkManager() ;	if (sNodeMeta != sNodeRoot)	{  	if (false == pLink->etablirLien(sNodeRoot, typeLink, sNodeMeta))    {
      string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotEstablishALinkWithTheRoot") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }    // lien du méta vers la contribution en cours : fait maintenant par signChanges()    // if (!pLink->etablirLien(sNodeMeta, NSRootLink::contributionAdded, pContexte->getPatient()->getContribution()))    //     erreur("Impossible d'initialiser le lien du document vers la nouvelle contribution du patient.", standardError, 0) ;
  }

  //
  // If there is a "master document", we create a link with it
  //
  if (string("") != sMasterDoc)
    connectToMasterDocument(sMasterDoc, masterLink, pGraphManager) ;

	// On crée ici l'arbre de présentation dans le graphe
  // (il sera enregistré dans enregistrePatPatho)
  if (false == PatPathoPresent.empty())
  {
  	_sCodeDocPres = pGraphManager->getDataGraph()->getNextTreeID() ;
    // On place ici l'arbre de présentation dans le graphe
    pGraphManager->setTree(&PatPathoPresent, "", _sCodeDocPres) ;
    // On crée directement le lien du méta vers son arbre de présentation
    // car on a déjà placé sa patpatho dans le graphe - à la différence des datas
    if (false == pLink->etablirLien(sNodeMeta, NSRootLink::docComposition, _sCodeDocPres))
    	erreur("Impossible d'initialiser le lien du document vers ses données de présentation.", standardError) ;

    _pDocInfo->setPres(&PatPathoPresent) ;
    _pDocInfo->setCodeDocPres(_sCodeDocPres) ;

    _bEnregPresentation = true ;
	}

	// On recalcule un nouveau code document pour la partie données
  // SAUF pour les documents statiques qui n'ont justement pas de datas  // Dans ce cas, le codeDocument de pDocInfo reste celui du Meta  if ((false == pSuper->typeDocument(typeDoc, NSSuper::isFile)) &&      (false == pSuper->typeDocument(typeDoc, NSSuper::isURL)))  {  	string sCodeDocData = pGraphManager->getDataGraph()->getNextTreeID() ;    // remise à jour du pDocInfo    _pDocInfo->setPatient(string(sCodeDocData, 0, PAT_NSS_LEN)) ;    _pDocInfo->setDocument(string(sCodeDocData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN)) ;	}
	// on met à jour le titre	SetTitreDoc() ;

	// ajout d'un nouveau document type fichier :  // remise à jour de l'historique ...
  // Les documents de type global 'C' (CS ou CN)
  // appellent Rafraichir dans leur fonction enregistrer pour
  // pouvoir passer la patpatho

	if ((pSuper->typeDocument(typeDoc, NSSuper::isFile)) ||      (pSuper->typeDocument(typeDoc, NSSuper::isURL)))	{
		// Pour les documents qui n'ont pas de patpatho, il faut penser
    // à commiter l'arbre des métas et de présentation maintenant car
    // on ne passe pas dans ce cas par ::enregistrePatPatho
    string sDroits ;

    // sDocId will be updated with new Id
    //
    string sDocId = _pDocInfo->getCodeDocMeta() ;
    pGraphManager->commitGraphTree(pContexte, sDocId) ;
    _pDocInfo->setCodeDocMeta(sDocId) ;

    NSPatPathoArray PatPathoNewMeta(pContexte->getSuperviseur()) ;
    pGraphManager->getTree(_pDocInfo->getCodeDocMeta(), &PatPathoNewMeta, &sDroits) ;
    _pDocInfo->setMeta(&PatPathoNewMeta) ;

    _sCodeDocMeta = _pDocInfo->getCodeDocMeta() ;

    // remise à jour du pDocInfo
    _pDocInfo->setPatient(string(_sCodeDocMeta, 0, PAT_NSS_LEN)) ;    _pDocInfo->setDocument(string(_sCodeDocMeta, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN)) ;

    if (false == PatPathoPresent.empty())
    {
      string sCodeDocPres = _pDocInfo->getCodeDocPres() ;
    	pGraphManager->commitGraphTree(pContexte, sCodeDocPres) ;
      _pDocInfo->setCodeDocPres(sCodeDocPres) ;

      NSPatPathoArray PatPathoNewPres(pContexte->getSuperviseur()) ;
      pGraphManager->getTree(_pDocInfo->getCodeDocPres(), &PatPathoNewPres, &sDroits) ;
      _pDocInfo->setPres(&PatPathoNewPres) ;
      _sCodeDocPres = _pDocInfo->getCodeDocPres() ;
    }

    // if (pContexte->getPatient()->pDocHis)
    //	pContexte->getPatient()->pDocHis->Rafraichir(pDocInfo, 0, this) ;	}
	return true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::Referencer.", standardError) ;
	return false ;
}
}

bool
NSNoyauDocument::ReferencerHtml(string /* typeDoc */, string /* nomDoc */, string /* tempDoc */, string /* enteteDoc */, bool /* bIsVisible */)
{
try
{
	return false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::ReferencerHtml.", standardError) ;
  return false ;
}
}

boolNSNoyauDocument::TemplateCompo(string typeDoc, string& tempDoc, string& enteteDoc, bool bVerbose)
{
try
{
  string ps2 = "NSNoyauDocument::TemplateCompo : entering for typeDoc = \"" + typeDoc + string("\"") ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

	VecteurString VectTermeEquivalentRoot ;
	int    degreTermeRoot = -1, degreTermeRootUtil = -1;
	bool   trouve = false;

	// s'il existe une patpatho non vide, on récupère le code root
	// et son array d'équivalents sémantiques
  //
  string sCodeSensDoc = string("") ;

	if (false == _PatPathoArray.empty())
	{
		sCodeSensDoc = (*(_PatPathoArray.begin()))->getLexiqueSens() ;
		// on récupère le vecteur d'équivalents
		pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(sCodeSensDoc, &VectTermeEquivalentRoot, "ES");
	}

	// on appelle ici le pilote pour chercher la liste des template par défaut de type "typeDoc"
	// -----------------------------------------------------------------------------------------
	NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;

	//Il faut creer la variable sTraitName = "_" + nomObjet + "_" + typeobjet
	string sTraitType = string("_0OTPL_0TYPE") ;
	string sTraitDefa = string("_0OTPL_0DEFA") ;
	// On transmet le code lexique pour le type
	string sTypeDoc = typeDoc ;
  pContexte->getDico()->donneCodeComplet(sTypeDoc) ;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(sTraitType, sTypeDoc)) ;
	AttrList.push_back(new NSBasicAttribute(sTraitDefa, string("WCEA01"))) ;

  ps2 = "NSNoyauDocument::TemplateCompo : getting templates traits" ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

	bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT.c_str(),
                                                &Graph, &ObjList, &AttrList) ;

	if ((false == res) || (ObjList.empty()))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "defaultTemplateNotFound");
    sErrorText += string(" (Doc= ") + sTypeDoc + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;

    if (bVerbose)
		  erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

  string sVoid = string("9VOID1") ;

  string sIdGlobal   = string("") ;  // global template
	string sIdRoot     = string("") ;  // template dedicated to this kind of document
  string sIdUtil     = string("") ;  // template dedicated to this user
	string sIdRootUtil = string("") ;  // template dedicated to this user and document

	for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
	{
		string sUtil = (*k)->getAttributeValue("_0OTPL_DOPER");
		string sCons = (*k)->getAttributeValue("_0OTPL_LNUCO");
		string sCodeRootTmpl = (*k)->getAttributeValue("_0OTPL_0TYPC");

    // Generic template
    //
    if ((sUtil == sVoid) && (sCons == sVoid))
    {
    	if ((string("") != sCodeRootTmpl) && (sCodeRootTmpl != sVoid))
      {
      	int degreTermeTmpl = -1 ;
        bool termeExiste = false ;

        string sCodeSensTmpl ;
        pContexte->getDico()->donneCodeSens(&sCodeRootTmpl, &sCodeSensTmpl) ;
        for (EquiItemIter i = VectTermeEquivalentRoot.begin() ; VectTermeEquivalentRoot.end() != i ; i++)
        {
        	degreTermeTmpl++ ;

          if (sCodeSensTmpl == (*(*i)))
          {
          	termeExiste = true ;
            break ;
          }
        }

        if (termeExiste)
        {
        	if ((degreTermeRoot < 0) || (degreTermeRoot > degreTermeTmpl))
          {
          	sIdRoot = (*k)->getAttributeValue(OIDS) ;
            degreTermeRoot = degreTermeTmpl ;
            trouve = true ;

            ps2 = "NSNoyauDocument::TemplateCompo : found template for this document " + sIdRoot ;
	          pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;
          }
        }
      }
      else
      {
      	sIdGlobal = (*k)->getAttributeValue(OIDS) ;
        trouve = true ;

        ps2 = "NSNoyauDocument::TemplateCompo : found generic template " + sIdGlobal ;
	      pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;
			}
		}

    // Specific template (dedicated to some user or workstation)
    //
#ifndef _ENTERPRISE_DLL
		else if (((sUtil == sVoid) || (sUtil == pContexte->getUtilisateurID())) &&
             ((sCons == sVoid) || (sCons == pContexte->getSuperviseur()->getConsole())))
#else
		else if (sUtil == pContexte->getUtilisateurID())
#endif
		{
			if ((string("") != sCodeRootTmpl) && (sCodeRootTmpl != sVoid))
			{
      	int degreTermeTmpl = -1 ;
        bool termeExiste = false ;

        string sCodeSensTmpl ;
        pContexte->getDico()->donneCodeSens(&sCodeRootTmpl, &sCodeSensTmpl) ;
        for (EquiItemIter i = VectTermeEquivalentRoot.begin(); i != VectTermeEquivalentRoot.end(); i++)
        {
        	degreTermeTmpl++ ;

          if (sCodeSensTmpl == (*(*i)))
          {
          	termeExiste = true ;
            break ;
          }
        }

        if (termeExiste)
        {
        	if ((degreTermeRootUtil < 0) || (degreTermeRootUtil > degreTermeTmpl))
          {
          	sIdRootUtil = (*k)->getAttributeValue(OIDS) ;
            degreTermeRootUtil = degreTermeTmpl ;
            trouve = true ;

            ps2 = "NSNoyauDocument::TemplateCompo : found template for user and document " + sIdRootUtil ;
	          pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;
          }
        }
      }
      else
      {
      	sIdUtil = (*k)->getAttributeValue(OIDS) ;
        trouve = true ;

        ps2 = "NSNoyauDocument::TemplateCompo : found template for user " + sIdUtil ;
        pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;
      }
    }

	}

	string sObjectId ;

	if (trouve)
	{    // on prend la template trouvée par ordre de priorité
    //
    if      (string("") != sIdRootUtil)
      sObjectId = sIdRootUtil ;
    else if (string("") != sIdRoot)
      sObjectId = sIdRoot ;
    else if (string("") != sIdUtil)
      sObjectId = sIdUtil ;
    else
      sObjectId = sIdGlobal ;

    ps2 = "NSNoyauDocument::TemplateCompo : selected template " + sObjectId ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

    // Appel du pilote pour retrouver la template en fonction de l'ObjectId
    // --------------------------------------------------------------------
    Graph.graphReset() ; // au cas où le graphe était déjà rempli
    NSBasicAttributeArray AttrArray ;
    //pAttrArray->push_back(new NSBasicAttribute("graphID" , sObjectId));
    AttrArray.push_back(new NSBasicAttribute(OBJECT , sObjectId));
    bool res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                    &Graph,  &AttrArray);
    if (false == res)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") ;
      sErrorText += string(" (ObjectID= ") + sObjectId + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      return "" ;
    }

    NSPatPathoArray TplPatPathoArray(pContexte->getSuperviseur(), graphObject) ;
    NSDataTreeIter iterTree ;
    PatPathoIter iter ;
    string sType, sTypeLex;

    string sFichierTpl = string("") ;
    string sEnTeteTpl  = string("") ;

    if (Graph.getTrees()->ExisteTree("0OTPL1", pContexte->getSuperviseur(), &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&TplPatPathoArray) ;
      iter = TplPatPathoArray.begin() ;
    }
    else
    {
      ps2 = "NSNoyauDocument::TemplateCompo : selected template " + sObjectId + string(" has a wrong graph (no template tree), leaving") ;
      pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;

			return false ;
    }

		PatPathoIter iterEnd = TplPatPathoArray.end() ;

    // Parsing de la template.......
    if ((NULL != iter) && (iter != iterEnd))
    {
    	string sSens = (*iter)->getLexiqueSens() ;

      if (string("0OTPL") == sSens)
      {
      	int iColTpl = (*iter)->getColonne() ;
        iter++ ;

        // on charge les données de l'archetype
        while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl))
        {
        	sSens = (*iter)->getLexiqueSens() ;

          // identifiant (unique)
          if (string("0ENTE") == sSens)
          {
          	iter++ ;
            sEnTeteTpl = string("") ;

            while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
            {
            	// on cherche ici un texte libre
              string sElemLex = (*iter)->getLexique() ;

              if (string("£?????") == sElemLex)
              	sEnTeteTpl = (*iter)->getTexteLibre() ;

              iter++ ;
            }
          }
          // nom du fichier (unique)
          else if (string("0TPL0") == sSens)
          {
          	iter++ ;
            sFichierTpl = string("") ;

            while ((iter != iterEnd) && ((*iter)->getColonne() > iColTpl+1))
            {
            	// on cherche ici un texte libre
              string sElemLex = (*iter)->getLexique() ;

              if (string("£?????") == sElemLex)
              	sFichierTpl = (*iter)->getTexteLibre() ;

              iter++ ;
            }
          }
          else
          	iter++ ;
        }

        if ((string("") != sEnTeteTpl) && (string("") != sFichierTpl))
        {
        	tempDoc = sFichierTpl ;
          enteteDoc = sEnTeteTpl ;
        }
      }
      else
      {
      	erreur("Le noeud template est mal positionné dans ::TemplateCompo().", standardError) ;
        return false ;
      }
    }
    else
    {
    	erreur("Le noeud template est incorrect dans ::TemplateCompo().", standardError) ;
      return false ;
    }

    ps2 = "NSNoyauDocument::TemplateCompo : leaving. Template = \"" + sFichierTpl + string("\" and Header = \"") + sEnTeteTpl + string("\"") ;
    pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trSubDetails) ;

		return true ;
	}

	char msg[255] ;
	sprintf(msg, "Pas de template de composition par defaut pour le type %s.", typeDoc.c_str()) ;
	MessageBox(0, msg, 0, MB_OK) ;
	return false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::TemplateCompo.", standardError) ;
	return false ;
}
}

// Pour récupérer la template du document sans le pathstringNSNoyauDocument::TemplateInfoBrut(bool bCompo){	string fichTmpl ;	if (bCompo)  	fichTmpl = _pHtmlInfo->getTemplate() ;  else  	fichTmpl = _pDocInfo->getTemplate() ;	size_t pos = fichTmpl.find_last_of('\\') ;	if (NPOS != pos)  	fichTmpl = string(fichTmpl, pos+1, strlen(fichTmpl.c_str())-pos-1) ;	return fichTmpl ;}// Pour récupérer l'en-tete du document sans le pathstringNSNoyauDocument::EnTeteInfoBrut(bool bCompo){	string enteteTmpl ;	if (bCompo)  	enteteTmpl = _pHtmlInfo->getEntete() ;  else  	enteteTmpl = _pDocInfo->getEntete() ;	size_t pos = enteteTmpl.find_last_of('\\') ;	if (NPOS != pos)  	enteteTmpl = string(enteteTmpl, pos+1, strlen(enteteTmpl.c_str())-pos-1) ;	return enteteTmpl ;}
boolNSNoyauDocument::ChemiseAvecImages(bool& bResult)
{
try
{
  if (NULL == pContexte->getPatient())
    return false ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

	NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
	VecteurString aVecteurString ;
	string sNodeChemise ;
	bool   chemiseAvecImages = false ;

	// on récupère d'abord la chemise (folder) du document
	pGraphe->TousLesVrais(_pDocInfo->getCodeDocMeta(), NSRootLink::docFolder, &aVecteurString, "ENVERS") ;
	if (aVecteurString.empty())
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "currentDocumentDoesNotBelongToAFolder") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
    bResult = false ;
    return false ;
  }

  sNodeChemise = *(*(aVecteurString.begin())) ;

  // on récupère maintenant la liste de tous les documents de cette chemise
	aVecteurString.vider() ;
  pGraphe->TousLesVrais(sNodeChemise, NSRootLink::docFolder, &aVecteurString) ;

  if (aVecteurString.empty())
  {
  	bResult = chemiseAvecImages ;
    return true ;
  }

  NSDocumentArray aDocusArray ;

  for (EquiItemIter i = aVecteurString.begin(); aVecteurString.end() != i; i++)
  	aDocusArray.push_back(new NSDocumentInfo(*(*i), pContexte, pGraphManager)) ;

  if (aDocusArray.empty())
  {
  	bResult = chemiseAvecImages ;
    return true ;
  }

  // Récupération des informations des documents
  DocInfoIter iterDoc = aDocusArray.begin() ;
  bool bOk ;

  while ((NULL != iterDoc) && (aDocusArray.end() != iterDoc))  {
  	aVecteurString.vider() ;
    bOk = false ;

    string sCodePat = string((*iterDoc)->getCodeDocMeta(), 0, PAT_NSS_LEN) ;
    string sCodeDoc = string((*iterDoc)->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

    (*iterDoc)->setPatient(sCodePat) ;
    (*iterDoc)->setDocument(sCodeDoc) ;

    if ((*iterDoc)->ParseMetaDonnees())
    {
    	if (pContexte->getSuperviseur()->typeDocument((*iterDoc)->getTypeSem(), NSSuper::isImage))
      {
      	chemiseAvecImages = true ;
        break ;
      }
      bOk = true ;
    }

    // problème de récupération de la patpatho (ou de parsing) : le document
    // ne figure pas dans la liste
    if (!bOk)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotAccessDocument") ;
      sErrorText += string(" (") + (*iterDoc)->getID() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      delete *iterDoc ;
      aDocusArray.erase(iterDoc) ;
    }
    else
    	iterDoc++ ;
	}

	bResult = chemiseAvecImages ;
	return true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::ChemiseAvecImages.", standardError) ;
	return false ;
}
}

boolNSNoyauDocument::DonneNouveauCodeDocument(string /* sPatient */, string& /* sCodeDoc */){	return true ;}

voidNSNoyauDocument::IncrementeCode(string& code)
{
  bool tourner = true ;
  int  i = strlen(code.c_str()) ;

  while ((tourner) && (i > 0))
  {
    i-- ;
    if (((code[i] >= '0') && (code[i] < '9')) ||
        ((code[i] >= 'A') && (code[i] < 'Z')))
    {
      code[i] = char(code[i] + 1) ;
      tourner = false ;
    }
    else if ('9' == code[i])
    {
      code[i] = 'A' ;
      tourner = false ;
    }
    else if ('Z' == code[i])
      code[i] = '0' ;
  }

  if (tourner)	// dans ce cas le code ne contient que des zéros
    code = string("1") + code ;
}

stringNSNoyauDocument::nomSansDoublons(string serveur, string unite, string pathRef,
                                                        string nom, string ext)
{
	string sCompteur = string("") ;

	string nomComplet ;
	string sNomFichier ;

	// Attention pathRef est ici un chemin Relatif	// et non un chemin absolu comme dans NSModHtml
  bool exist = true ;
	while (exist)
	{
		IncrementeCode(sCompteur) ;
		nomComplet = nom + sCompteur ;

		if (string("") != serveur)    	sNomFichier = serveur + pathRef + nomComplet + string(".") + ext ;
    else
    	sNomFichier = unite + pathRef + nomComplet + string(".") + ext ;

#ifndef _ENTERPRISE_DLL
		exist = NsFileExists(sNomFichier) ;#else    exist = (PR_SUCCESS == PR_Access(sNomFichier.c_str(), PR_ACCESS_EXISTS)) ;#endif
	}

	// tant que le compteur ne dépasse pas ZZ on obtiendra un nom dos à 8 + 3
	if (string("") != nomComplet)
  	nomComplet = nomComplet + string(".") + ext ;

	return nomComplet ;}

//
// Met à jour le booleen bDocumentValide pour les documents de type fichier
//
void
NSNoyauDocument::ValideFichier(string* psNomFichier)
{
try
{
	string sNomFichier ;

	if (_pDocInfo)
	{
  	if (!_pDocInfo->getNomFichier(sNomFichier))
    	_bDocumentValide = false ;
    else
    {
#ifndef _ENTERPRISE_DLL
		  _bDocumentValide = NsFileExists(sNomFichier) ;#else      _bDocumentValide = (PR_SUCCESS == PR_Access(sNomFichier.c_str(), PR_ACCESS_EXISTS)) ;#endif

      if (_bDocumentValide && psNomFichier)
      	*psNomFichier = sNomFichier ;
    }
  }
  else
  	_bDocumentValide = false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::ValideFichier.", standardError) ;
}
}

boolNSNoyauDocument::RetrouveHtmlInfo()
{
try
{
	return false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::RetrouveHtmlInfo.", standardError) ;
	return false;
}
}

boolNSNoyauDocument::MajTemplate()
{
try
{
	return false ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::MajTemplate.", standardError) ;
	return false ;
}
}

bool
NSNoyauDocument::DetruireComposants(NSPatPathoArray* pPatPatho)
{
	if (((NSPatPathoArray*) NULL == pPatPatho) || (pPatPatho->empty()))
		return false ;
try
{
  NSLinkManager* pLink = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  int iColBase = (*(pPatPatho->begin()))->getColonne() ;

  VecteurString aVecteurString ;

  // Suppression de tous les éléments TAG, de leurs liens et de leurs fils
  string sElemLex = string("0TAG01") ;
  string sSens ;
  pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

  // on supprime tous les fils de code sSens dans pPPTEnCours
  PatPathoIter iterCourant = pPatPatho->ChercherItem(sSens) ;
  // PatPathoIter iterLast    = NULL ;

  while ((NULL != iterCourant) && (pPatPatho->end() != iterCourant))
  {
  	if ((*iterCourant)->getColonne() == iColBase + 1)
    {
    	string sNodeTag = (*iterCourant)->getNode() ;
      aVecteurString.vider() ;
      pLink->TousLesVrais(sNodeTag, NSRootLink::compositionTag, &aVecteurString) ;
      if (false == aVecteurString.empty())
      {
      	string sCodeDocImage = *(*(aVecteurString.begin())) ;
        pLink->detruireLien(sNodeTag, NSRootLink::compositionTag, sCodeDocImage) ;
      }

      pPatPatho->SupprimerFils(iterCourant) ;
      pPatPatho->SupprimerItem(iterCourant) ;

      // iterLast = iterCourant ;

      if ((NULL == iterCourant) || (pPatPatho->end() == iterCourant))
      	break ;

      if ((*iterCourant)->getLexique() != sElemLex)
      	iterCourant = pPatPatho->ChercherItem(sSens, true, iterCourant) ;
    }
    else // cas item trouvé ailleurs qu'en colonne 1
    	iterCourant = pPatPatho->ChercherItem(sSens, true, iterCourant) ;
  }

  return true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::DetruireComposants", standardError) ;
	return false ;
}
}

bool
NSNoyauDocument::EcrireComposants(NSPatPathoArray* pPatPatho)
{
try
{
	if ((NSPatPathoArray*) NULL == pPatPatho)
		return false ;

	// on récupère la base d'image pour insérer les composants image
	// (on suppose ici qu'on a détruit les composants s'il existaient déjà)

	if (string("") == _sBaseCompo)		return false ;
	NSBaseImages BaseImages(_sBaseCompo) ;
	if (false == BaseImages.lire()) // charge la base d'images
		return false ;

	for (int i = 0; i < BaseImages.nbImages; i++)	{
		if (BaseImages.tableSelect[i]) // si l'image a été sélectionnée
    {
    	// Les images et les vidéos ont un type composant différent
      // mais elles font partie de la meme base d'images pour un
      // fichier html donné

			string sTypeImage = BaseImages.tableTypImg[i] ;      char cIndexComp[4] ;      sprintf(cIndexComp, "%03d", BaseImages.tableSelect[i]) ;
      string sCodeImage = BaseImages.tableCompos[i] ;

      if (pContexte->getSuperviseur()->typeDocument(sTypeImage, NSSuper::isImage))      {      	pPatPatho->ajoutePatho("0TAG01", 1) ;
        pPatPatho->ajoutePatho("0TYPC1", 2) ;
        pPatPatho->ajoutePatho("ZIMA01", 3) ;

        pPatPatho->ajoutePatho("VNUMT1", 2) ;
        Message Msg ;
        Msg.SetComplement(string(cIndexComp)) ;
        pPatPatho->ajoutePatho("£N0;03", &Msg, 3) ;
      }    }
  }

	// On remet ici l'arbre de présentation dans le graphe
	// afin d'obtenir des numéros de noeud pour les tags
	pContexte->getPatient()->getGraphPerson()->setTree(pPatPatho, "", _pDocInfo->getCodeDocPres()) ;

	// On doit maintenant retrouver les tags image pour insérer leurs liens
	// avec les documents images
  string sDroits ;
  pContexte->getPatient()->getGraphPerson()->getTree(_pDocInfo->getCodeDocPres(), pPatPatho, &sDroits) ;

  // on doit parcourir la librairie pour charger l'array des chemises
  PatPathoIter iter = pPatPatho->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((pPatPatho->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
  	string sSens = (*iter)->getLexiqueSens() ;

    if (string("0TAG0") == sSens)
    {
    	string sNodeTag = (*iter)->getNode() ;
      string sIndex = string("") ;
      iter++ ;

      // on charge l'index du tag
      while ((pPatPatho->end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
      	sSens = (*iter)->getLexiqueSens() ;

        // tag number
        if (string("VNUMT") == sSens)
        {
        	iter++ ;
          while ((pPatPatho->end() != iter) && ((*iter)->getColonne() > iColBase+2))
          {
          	// on cherche ici un texte libre
            string sElemLex = (*iter)->getLexique() ;
            if (string(sElemLex, 0, 2) == string("£N"))
            	sIndex = (*iter)->getComplement() ;

            iter++ ;
          }
        }
        // Tag type
        else if (string("0TYPC") == sSens)
        	iter++ ;
        else
        	iter++ ;
      }

      if (sIndex != "")
      {
      	for (int i = 0; i < BaseImages.nbImages; i++)
        {
        	if (BaseImages.tableSelect[i] == atoi(sIndex.c_str()))
          {
          	// on retrouve le code du méta associé au composant image
            string sCodeImage = BaseImages.tableCompos[i] ;

            NSLinkManager* pLink = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
            if (false == pLink->etablirLien(sNodeTag, NSRootLink::compositionTag, sCodeImage))
            	erreur("Impossible d'initialiser le lien du document vers un élément de présentation.", standardError) ;
            break ;
          }
        }
      }
    }
    else
    	iter++ ;
	}

	BaseImages.detruire() ;

	if (string("") != _sBaseTemp) // si la base a été initialisée	{
  	NSBaseImages BaseTemp(_sBaseTemp) ;
    BaseTemp.lire() ;
    BaseTemp.detruire() ;
	}

	// on conserve l'ancienne base pour pouvoir détruire les anciennes images	// et on remet la base à zéro pour la recharger à la prochaine opération
	// (on garde la base compo pour l'instant)

	_sBaseTemp   = _sBaseImages ;	_sBaseImages = string("") ;

	return true ;
}
catch (...)
{
	erreur("Exception NSNoyauDocument::EcrireComposants", standardError) ;
	return false ;
}
}

//---------------------------------------------------------------------------//  Fonctions de gestion des propriétés de NSNoyauDocument
//---------------------------------------------------------------------------
#ifndef _ENTERPRISE_DLL
static char* PropNames[] = { "FichierInfo" };	// Fichier de pDocInfo

static int PropFlags[] = { pfGetText };

const char*
NSNoyauDocument::PropertyName(int index)
{
	if (index <= PrevProperty)
		return TDocument::PropertyName(index) ;
	else if (index < NextProperty)
		return PropNames[index-PrevProperty-1] ;
	else
		return 0 ;
}

intNSNoyauDocument::PropertyFlags(int index)
{
	if (index <= PrevProperty)
		return TDocument::PropertyFlags(index) ;
	else if (index < NextProperty)
		return PropFlags[index-PrevProperty-1] ;
	else
		return 0 ;
}

intNSNoyauDocument::FindProperty(const char far* name)
{
	for (int i=0; i < NextProperty-PrevProperty-1; i++)
		if (!strcmp(PropNames[i], name))
			return i+PrevProperty+1 ;

	return 0 ;}

intNSNoyauDocument::GetProperty(int index, void far *dest, int textlen)
{
	switch (index)
	{
		case FichierInfo:
    {
			bool bNomOk = false;
			string nomFichier;

			if (_pDocInfo)				bNomOk = _pDocInfo->getNomFichier(nomFichier) ;

      if (bNomOk)			{
      	strcpy((char far *) dest, nomFichier.c_str()) ;
        return strlen(nomFichier.c_str()) ;
      }
      else
      {
      	strcpy((char far *) dest, "") ;
        return 0 ;
      }
    }
	}

   return TDocument::GetProperty(index, dest, textlen) ;}
#endif

bool
NSNoyauDocument::createGraphRoot(NSPersonGraphManager* pGraphManager)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  string sRootObject = pContexte->getSuperviseur()->getText("rootManagement", "rootObject") ;
  if (string("") == sRootObject)
    sRootObject = string("Root object") ;

  // Ici on crée l'embryon du graphe patient auquel on rattache la fiche admin
  // l'identifiant PIDS in memoire
  string sNss          = string(1, INMEMORY_CHAR) + string("000000") ;
  string sRootDocGraph = string(1, INMEMORY_CHAR) + string("00000") ;
  string sRootGraph    = sNss + sRootDocGraph ;

  _PatPathoArray.vider() ;
  _PatPathoArray.ajoutePatho("HHUMA3", 0) ;

  if (false == Referencer("ZCS00", sRootObject, "", "", false, false, sRootDocGraph, NSRootLink::personDocument, pGraphManager, ""))
  	return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createFoldersLibrary(NSPersonGraphManager* pGraphManager)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  string sFoldersLibrary = pContexte->getSuperviseur()->getText("folderManagement", "foldersLibrary") ;
  string sDefaultFolder  = pContexte->getSuperviseur()->getText("folderManagement", "defaultFolder") ;
  string sTrashFolder    = pContexte->getSuperviseur()->getText("folderManagement", "trashFolder") ;

  if (string("") == sFoldersLibrary)
    sFoldersLibrary = string("Folders Library") ;
  if (string("") == sDefaultFolder)
    sDefaultFolder = string("Default") ;
  if (string("") == sTrashFolder)
    sTrashFolder = string("Trash") ;

  string sDateJour = donne_date_duJour() ;

  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho("0LIBC1", 0) ;

	// ajout de la chemise corbeille
  // Note : cette chemise doit être créée en premier afin que la chemise par défaut
  // soit sélectionnée plus tard dans EnregDocDialog (on sélectionne la dernière chemise)
	_PatPathoArray.ajoutePatho("0CHEM1", 1) ;

	// Intitulé : nom de la chemise
  _PatPathoArray.ajoutePatho("0INTI1", 2) ;
  Message Msg ;
  Msg.SetTexteLibre(sTrashFolder) ;
  _PatPathoArray.ajoutePatho("£?????", &Msg, 3) ;

  // Date d'ouverture
  _PatPathoArray.ajoutePatho("KOUVR1", 2) ;
  string sDateCreation = sDateJour + string("000000") ;
  Message Msg2 ;
  Msg2.SetUnit("2DA021") ;
  Msg2.SetComplement(sDateCreation.c_str()) ;
  _PatPathoArray.ajoutePatho("£T0;19", &Msg2, 3) ;

  // ajout de la chemise par défaut
	_PatPathoArray.ajoutePatho("0CHEM1", 1) ;

	// Intitulé : nom de la chemise
  _PatPathoArray.ajoutePatho("0INTI1", 2) ;
  Message Msg3 ;
  Msg3.SetTexteLibre(sDefaultFolder) ;
  _PatPathoArray.ajoutePatho("£?????", &Msg3, 3) ;

  // Date d'ouverture
  _PatPathoArray.ajoutePatho("KOUVR1", 2) ;
  Message Msg4 ;
  Msg4.SetUnit("2DA021") ;
  Msg4.SetComplement(sDateCreation.c_str()) ;
  _PatPathoArray.ajoutePatho("£T0;19", &Msg4, 3) ;

  if (false == Referencer("ZCS00", sFoldersLibrary, "", "", true, false, "", NSRootLink::personFolderLibrary, pGraphManager, ""))
  	return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  if (string("") == sRootDocData)
    return false ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createHealthTeam(NSPersonGraphManager* pGraphManager, string sPersonId)
{
  if (((NSPersonGraphManager*) NULL == pGraphManager) || (string("") == sPersonId))
    return false ;

  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho(HEALTHTEAM, 0) ;

  NSHealthTeam HealthTeam(&_PatPathoArray) ;
  NSHealthTeamManager	HTManager(&HealthTeam) ;

  NVLdVTemps	ldvCurDate ;
  ldvCurDate.takeTime() ;
  NVLdVTemps	ldvNoLimit ;
  ldvNoLimit.setNoLimit() ;

  // ajout des mandats du patient -- adding Patient mandates
  NSHTMMandateArray	MandatesArray ;
  MandatesArray.push_back(new NSHealthTeamMandate(patDist, 0, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::root)) ;
  MandatesArray.push_back(new NSHealthTeamMandate(patDist, 0, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
  HTManager.addMember(new NSHealthTeamMember(sPersonId, NSHealthTeamMember::person, &MandatesArray), NSHealthTeamMember::person) ;

  // ajout de l'Equipe Médicale du site -- adding Hospital Medical Team mandate
  // il faut d'abord faire un appel au pilot pour trouver l'équipe médicale locale
  // s'il y en a plusieurs, les equipes medicales sont separes par des "|"
  //
  NSTeamSupervisor teamSupervisor(pContexte) ;
  vector<string> vLocalTeams = teamSupervisor.getLocalTeams() ;
  if (false == vLocalTeams.empty())
  {
    for (vector<string>::iterator lTeamIter = vLocalTeams.begin() ; vLocalTeams.end() != lTeamIter ; lTeamIter++)
    {
      string sLocalTeam = *lTeamIter ;
      NSHTMMandateArray MandatesArr ;
      MandatesArr.push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
      HTManager.addMember(new NSHealthTeamMember(sLocalTeam, NSHealthTeamMember::team, &MandatesArr), NSHealthTeamMember::team) ;
    }
  }
/*
  if (sLocalTeamsID != "")
  {
    size_t firstID  = 0 ;
    size_t nextID   = sLocalTeamsID.find("|") ;
    while (nextID != string::npos)
    {
      string sLocalTeam = string(sLocalTeamsID, firstID, nextID - firstID) ;
      pMandatesArray = new NSHTMMandateArray() ;
      pMandatesArray->push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
      pHTManager->addMember(new NSHealthTeamMember(sLocalTeam, NSHealthTeamMember::team, pMandatesArray), NSHealthTeamMember::team) ;
      delete pMandatesArray ;
      firstID = nextID + 1 ;
      nextID  = sLocalTeamsID.find("|", firstID) ;
    }
    string sLocalTeam = string(sLocalTeamsID, firstID, strlen(sLocalTeamsID.c_str()) - firstID) ;
    pMandatesArray = new NSHTMMandateArray() ;
    pMandatesArray->push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
    pHTManager->addMember(new NSHealthTeamMember(sLocalTeam, NSHealthTeamMember::team, pMandatesArray), NSHealthTeamMember::team) ;
    delete pMandatesArray ;
  }
*/
  else
  {
    // ajout des mandats de l'utilisateur -- adding user mandate
    NSHTMMandateArray MandatesArr ;
    MandatesArr.push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;
    HTManager.addMember(new NSHealthTeamMember(pContexte->getUtilisateurID(), NSHealthTeamMember::person, &MandatesArr), NSHealthTeamMember::person) ;
  }

  _PatPathoArray = *(HTManager.getPatho()) ;

  if (false == Referencer("ZCS00", "Equipe de Santé", "", "", true, false, "", NSRootLink::personHealthTeam))
	  return false ;

	// le code est etabli dans la methode referencer
  string sHealthTeamDocData = pGraphManager->getDataGraph()->getLastTree() ;
  if (string("") == sHealthTeamDocData)
    return false ;
  pGraphManager->setTree(&_PatPathoArray, "", sHealthTeamDocData) ;

  return etablirLiensTree(sHealthTeamDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createIdentifiersLibrary(NSPersonGraphManager* pGraphManager, string sIpp)
{
  string ps2 = "NSNoyauDocument::createIdentifiersLibrary : begin" ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

  if (((NSPersonGraphManager*) NULL == pGraphManager) || (string("") == sIpp))
  {
    ps2 = "NSNoyauDocument::createIdentifiersLibrary : bad parameters, end" ;
	  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }

  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho("0LIBI1", 0) ;
  _PatPathoArray.ajoutePatho("LSITE1", 1) ;

  Message Msg ;

  string sSite = pContexte->getSuperviseur()->getIppSite() ;
  if (string("") != sSite)
  {
    Msg.SetComplement(sSite) ;
    _PatPathoArray.ajoutePatho("£OBJE1", &Msg, 2) ;
  }

  _PatPathoArray.ajoutePatho("LIPP01", 2) ;

  Msg.Reset() ;
  Msg.SetTexteLibre(sIpp) ;
  _PatPathoArray.ajoutePatho("£CL000", &Msg, 3) ;

  _PatPathoArray.ajoutePatho("KOUVR1", 3) ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  Msg.Reset() ;
  Msg.SetComplement(tpsNow.donneDateHeure()) ;
  Msg.SetUnit("2DA021") ;
  _PatPathoArray.ajoutePatho("£T0;19", &Msg, 4) ;

  if (false == Referencer("ZCS00", "Identifiants", "", "", true, false, "", NSRootLink::personIdentifiers))
  {
    ps2 = string("NSNoyauDocument::createIdentifiersLibrary : Referencer failed, ending") ;
	  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }

  // le code est etabli dans la methode referencer
  string sIdDocData = pGraphManager->getDataGraph()->getLastTree() ;
  if (string("") == sIdDocData)
  {
    ps2 = string("NSNoyauDocument::createIdentifiersLibrary : Cannot get library ID, ending") ;
	  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }
  pGraphManager->setTree(&_PatPathoArray, "", sIdDocData) ;

  if (false == etablirLiensTree(sIdDocData, pGraphManager))
  {
    ps2 = string("NSNoyauDocument::createIdentifiersLibrary : Cannot connect library, ending") ;
	  pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trError) ;
    return false ;
  }

  ps2 = "NSNoyauDocument::createIdentifiersLibrary : ending successfuly" ;
	pContexte->getSuperviseur()->trace(&ps2, 1, NSSuper::trDetails) ;

  return true ;
}

bool
NSNoyauDocument::createFrame(NSPersonGraphManager* pGraphManager, LDVFRAME iFrame)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  string sRootCode = getRootForFrame(iFrame) ;
  if (string("") == sRootCode)
    return false ;

  // Document's title
  //
  string sIndexLabel = pContexte->getSuperviseur()->getText("LigneDeVieManagement", getLabelForFrame(iFrame)) ;
  if (string("") == sIndexLabel)
    sIndexLabel = string("Health index") ;

  // Build patpatho
  //
  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho(sRootCode, 0) ;

	// ajout des préoccupations de Santé dans l'index de Santé
	_PatPathoArray.ajoutePatho("0PRO11", 1) ;

	// ajout des objectifs de Santé dans l'index de Santé
	_PatPathoArray.ajoutePatho("0OBJE1", 1) ;

  // ajout des traitements dans l'index de Santé
	_PatPathoArray.ajoutePatho("N00001", 1) ;

	if (false == Referencer("ZCS00", sIndexLabel, "", "", true, false, "", getLinkForFrame(iFrame), pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createFrameExtensionTree(NSPersonGraphManager* pGraphManager, string sIndexDocId, string sRootConcept, NSRootLink::NODELINKTYPES rootLink)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  // Document's title
  //
  string sLang = pContexte->getUserLanguage() ;

  string sIndexLabel = string("") ;
  pContexte->getDico()->donneLibelle(sLang, &sRootConcept, &sIndexLabel) ;
  if (string("") != sIndexLabel)
    sIndexLabel[0] = pseumaj(sIndexLabel[0]) ;

  // Build patpatho
  //
  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho(sRootConcept, 0) ;

	if (false == Referencer("ZCS00", sIndexLabel, "", "", true, false, "", NSRootLink::personIndexExtension, pGraphManager, "", "", "", "", "", "", NSRootLink::viewOfDocument, false /* bMustConnectToFolder */))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  // Connecting this tree to health index
  //
  if (string("") != sIndexDocId)
  {
    NSLinkManager Link(pContexte->getSuperviseur(), pGraphManager->getDataGraph()) ;
    if (false == Link.etablirLien(sIndexDocId, rootLink, _sCodeDocMeta))    {
    	erreur("Impossible de faire le lien entre le réservoir de préoccupations et le document index de santé.", standardError) ;
      return false ;
    }
  }

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createFrameConcernsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId)
{
  return createFrameExtensionTree(pGraphManager, sIndexDocId, string("0PRO11"), NSRootLink::indexConcerns) ;
}

bool
NSNoyauDocument::createFrameTreatmentsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId)
{
  return createFrameExtensionTree(pGraphManager, sIndexDocId, string("N00001"), NSRootLink::drugOf) ;
}

bool
NSNoyauDocument::createFrameGoalsTree(NSPersonGraphManager* pGraphManager, string sIndexDocId)
{
  return createFrameExtensionTree(pGraphManager, sIndexDocId, string("0OBJE1"), NSRootLink::indexGoals) ;
}

/*
bool
NSNoyauDocument::createLdvFrame(NSPersonGraphManager* pGraphManager)
{
  if (NULL == pGraphManager)
    return false ;

  string sHealthIndex = pContexte->getSuperviseur()->getText("LigneDeVieManagement", "healthIndex") ;
  if (string("") == sHealthIndex)
    sHealthIndex = string("Health index") ;

  pPatPathoArray->vider() ;

  pPatPathoArray->ajoutePatho("ZPOMR1", 0) ;

	// ajout des préoccupations de Santé dans l'index de Santé
	pPatPathoArray->ajoutePatho("0PRO11", 1) ;

	// ajout des objectifs de Santé dans l'index de Santé
	pPatPathoArray->ajoutePatho("0OBJE1", 1) ;

  // ajout des traitements dans l'index de Santé
	pPatPathoArray->ajoutePatho("N00001", 1) ;

	if (false == Referencer("ZCS00", sHealthIndex, "", "", true, false, "", NSRootLink::personHealthIndex, pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(pPatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createRiskFrame(NSPersonGraphManager* pGraphManager)
{
  if (NULL == pGraphManager)
    return false ;

  string sRiskFollowup = pContexte->getSuperviseur()->getText("riskManagement", "riskFollowup") ;
  if (string("") == sRiskFollowup)
    sRiskFollowup = string("Risks index") ;

  pPatPathoArray->vider() ;

  pPatPathoArray->ajoutePatho("ORISK1", 0) ;

  // adding the "concerns" folder
	pPatPathoArray->ajoutePatho("0PRO11", 1) ;

	if (false == Referencer("ZCS00", sRiskFollowup, "", "", true, false, "", NSRootLink::personRiskManager, pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(pPatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createSocialFrame(NSPersonGraphManager* pGraphManager)
{
  if (NULL == pGraphManager)
    return false ;

  string sDocName = pContexte->getSuperviseur()->getText("LigneDeVieManagement", "socialIndex") ;
  if (string("") == sDocName)
    sDocName = string("Social index") ;

  pPatPathoArray->vider() ;

  pPatPathoArray->ajoutePatho("ZPSOC1", 0) ;

	// adding the "concerns" folder
	pPatPathoArray->ajoutePatho("0PRO11", 1) ;

	// adding the "goals" folder
	pPatPathoArray->ajoutePatho("0OBJE1", 1) ;

  // adding the "treatment" folder
	pPatPathoArray->ajoutePatho("N00001", 1) ;

	if (false == Referencer("ZCS00", sDocName, "", "", true, false, "", NSRootLink::personSocialIndex, pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(pPatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}
*/

bool
NSNoyauDocument::createSynthesisFrame(NSPersonGraphManager* pGraphManager)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  string sSynthesisForm = pContexte->getSuperviseur()->getText("synthesisManagement", "synthesisForm") ;
  if (string("") == sSynthesisForm)
    sSynthesisForm = string("Synthesis form") ;

  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho("ZSYNT1", 0) ;

	if (false == Referencer("ZCS00", sSynthesisForm, "", "", true, false, "", NSRootLink::personSynthesis, pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::createIdentifiersFrame(NSPersonGraphManager* pGraphManager)
{
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  string sIdentifiersLibrary = pContexte->getSuperviseur()->getText("identifiersManagement", "identifiersLibrary") ;
  if (string("") == sIdentifiersLibrary)
    sIdentifiersLibrary = string("Identifiers library") ;

  _PatPathoArray.vider() ;

  _PatPathoArray.ajoutePatho("0LIBI1", 0) ;

	if (false == Referencer("ZCS00", sIdentifiersLibrary, "", "", true, false, "", NSRootLink::personIdentifiers, pGraphManager, ""))
		return false ;

  string sRootDocData = pGraphManager->getDataGraph()->getLastTree() ;
  pGraphManager->setTree(&_PatPathoArray, "", sRootDocData) ;

  return etablirLiensTree(sRootDocData, pGraphManager) ;
}

bool
NSNoyauDocument::etablirLiensTree(string sNodeData, NSPersonGraphManager* pGraphManager)
{
  if (((NSPersonGraphManager*) NULL == pGraphManager) || (string("") == sNodeData))
    return false ;

  if (_bCreerMetaLien)
  {
  	NSLinkManager Link(pContexte->getSuperviseur(), pGraphManager->getDataGraph()) ;
    if (false == Link.etablirLien(_sCodeDocMeta, NSRootLink::docData, sNodeData))    {
    	erreur("Impossible de faire le lien vers le document data.", standardError) ;
      return false ;
    }    _bCreerMetaLien = false ;
  }
  else if (string("") != _sCodeDocMeta)
  {  	// Note : le lien contributionModified ne peut s'établir que la deuxième fois    // qu'on enregistre les données, car la première fois bCreerMetaLien est true.    string sContribution = pContexte->getPatient()->getContribution() ;    NSLinkManager Link(pContexte->getSuperviseur(), pGraphManager->getDataGraph()) ;    if (false == Link.existeLien(_sCodeDocMeta, NSRootLink::contributionModified, sContribution))    {    	// lien du méta vers la contribution en cours      if (false == Link.etablirLien(_sCodeDocMeta, NSRootLink::docData, sNodeData))      {
      	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotEstablishALinkWithTheContribution") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError) ;
        return false ;
      }    }  }
  return true ;
}

bool
NSNoyauDocument::connectToMasterDocument(string sMasterDocId, NSRootLink::NODELINKTYPES masterLink, NSPersonGraphManager* pGraphManager)
{
  if ((string("") == sMasterDocId) || (string("") == _sCodeDocMeta))
    return false ;

  // Initialisation du GraphManager
	if ((NSPersonGraphManager*) NULL == pGraphManager)		pGraphManager = pContexte->getPatient()->getGraphPerson() ;

  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

  NSLinkManager* pLink = pGraphManager->getLinkManager() ;
  if (NULL == pLink)
    return false ;

  if (false == pLink->etablirLien(_sCodeDocMeta, masterLink, sMasterDocId))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotEstablishALinkWithTheDocument") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  return true ;
}

void
NSNoyauDocument::initDocumentInfo(NSDocumentInfo* pNewDocument, string sDocType,
                                  string sAuthorId, bool bDocVisible,
                                  string sDestinataire, string sCreationDate)
{
  if ((NSDocumentInfo*) NULL == pNewDocument)
    return ;

	string sCreation = sCreationDate ;
  if (string("") == sCreation)
    sCreation = donne_date_duJour() + donne_heure() ;
  // Créateur : Utilisateur en cours
  //
	pNewDocument->setCreator(pContexte->getUtilisateurID()) ;
  pNewDocument->setDestinat(sDestinataire) ;
  if ((string("_User_") != sAuthorId) && ((string("") != sAuthorId)))
  	pNewDocument->setAuthor(sAuthorId) ;

	// Document type	pNewDocument->setType(sDocType) ;

	// Intêret 5 = moyen	pNewDocument->setInteret(string("5")) ;

	// Date de creation = date du jour	pNewDocument->setCreDate(sCreation) ;

	// visible est à 1 quand on référence un document (sauf pour les images)	if ((string("") != sDocType) && ('I' == sDocType[1]))
		pNewDocument->rendInvisible() ;
	else
	{
		if (bDocVisible)
			pNewDocument->rendVisible() ;
		else
			pNewDocument->rendInvisible() ;
	}
}

void
NSNoyauDocument::createPresentationInfo(NSDocumentInfo*  pNewDocument,
                                        NSPatPathoArray* pPatPathoPresent,
                                        string sCodeDoc,
                                        NSRootLink::NODELINKTYPES typeLink,
                                        string typeDoc, string tmplDoc,
                                        string enteteDoc)
{
  if (((NSPatPathoArray*) NULL == pPatPathoPresent) ||
      ((NSDocumentInfo*)  NULL == pNewDocument))
    return ;

  string sRootDocGraph = string(1, INMEMORY_CHAR) + string("00000") ;
  if (sCodeDoc == sRootDocGraph)
    return ;

  if (NSRootLink::personDocument != typeLink)
    return ;

  // No presentation info for PDFs
  //
  if (string("ZTPDF") == typeDoc)
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if ((false == pSuper->typeDocument(typeDoc, NSSuper::isTree)) &&
      (false == pSuper->typeDocument(typeDoc, NSSuper::isText)) &&      (string("ZIHTM") != typeDoc))
    return ;

  if ((string("") == tmplDoc) && (string("") == enteteDoc))
  {
    // on récupère les données en fonction de l'utilisateur
    if (false == TemplateCompo(typeDoc, _sTemplate, _sEnTete))
#ifndef _ENTERPRISE_DLL
      erreur("Impossible de fixer la template et l'en-tête du document", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
#else
      erreur("Impossible de fixer la template et l'en-tête du document", standardError) ;
#endif
    else
    {
      pNewDocument->setTemplate(_sTemplate) ;
      pNewDocument->setEnTete(_sEnTete) ;
    }
  }
  else
  {
    _sTemplate = tmplDoc ;
    _sEnTete   = enteteDoc ;
    pNewDocument->setTemplate(_sTemplate) ;
    pNewDocument->setEnTete(_sEnTete) ;
  }

  Message Msg ;
  pPatPathoPresent->ajoutePatho("ZPRES1", &Msg, 0) ;

  // Template
  pPatPathoPresent->ajoutePatho("0TPL01", 1) ;
  Msg.Reset() ;
  Msg.SetTexteLibre(_sTemplate.c_str()) ;
  pPatPathoPresent->ajoutePatho("£?????", &Msg, 2) ;

  // En-tete
  pPatPathoPresent->ajoutePatho("0ENTE1", 1) ;
  Msg.Reset() ;
  Msg.SetTexteLibre(_sEnTete.c_str()) ;
  pPatPathoPresent->ajoutePatho("£?????", &Msg, 2) ;
}

void
NSNoyauDocument::createMetaPatpatho(NSDocumentInfo* pNewDocument, NSPatPathoArray* pPatPathoMeta, string sAuthorId, string typeDoc)
{
  if (((NSPatPathoArray*) NULL == pPatPathoMeta) ||
      ((NSDocumentInfo*)  NULL == pNewDocument))
    return ;

  // noeud racine
	Message Msg ;
	Msg.SetInteret(pNewDocument->getInteret()) ;
	Msg.SetVisible(pNewDocument->getVisible()) ;
	pPatPathoMeta->ajoutePatho("ZDOCU1", &Msg, 0) ;

	// Intitulé : nom du document
	pPatPathoMeta->ajoutePatho("0INTI1", 1) ;
  Msg.Reset() ;
	Msg.SetTexteLibre(pNewDocument->getDocName()) ;
	pPatPathoMeta->ajoutePatho("£?????", &Msg, 2) ;

	if (string("") != pNewDocument->getCreator())
	{
  	pPatPathoMeta->ajoutePatho("DOPER1", 1) ;
		Msg.Reset() ;
		Msg.SetComplement(pNewDocument->getCreator()) ;
		pPatPathoMeta->ajoutePatho("£SPID1", &Msg, 2) ;
	}

	// créateur != opérateur
	if ((string("_User_") != sAuthorId) && (string("") != sAuthorId))
	{
		pPatPathoMeta->ajoutePatho("DAUTE1", 1) ;
		Msg.Reset() ;
    if (string("") != pNewDocument->getAuthor())
    	Msg.SetComplement(pNewDocument->getAuthor()) ;
    else
			Msg.SetComplement(sAuthorId) ;
		pPatPathoMeta->ajoutePatho("£SPID1", &Msg, 2) ;
	}

  // Destinataire
  //
  if (string("") != pNewDocument->getDestinat())
	{
  	pPatPathoMeta->ajoutePatho("DDEST1", 1) ;
		Msg.Reset() ;
		Msg.SetComplement(pNewDocument->getDestinat()) ;
		pPatPathoMeta->ajoutePatho("£SPID1", &Msg, 2) ;
	}

	// Type : code lexique
	string sTypeDocum = pNewDocument->getTypeSem() ;
  pContexte->getDico()->donneCodeComplet(sTypeDocum) ;
	pPatPathoMeta->ajoutePatho("0TYPE1", 1) ;
	pPatPathoMeta->ajoutePatho(sTypeDocum, 2) ;

	// Localisation : champ complement
  if (string("") != pNewDocument->getLocalis())
  {
	  Msg.Reset() ;
	  Msg.SetComplement(pNewDocument->getLocalis()) ;
	  pPatPathoMeta->ajoutePatho("0LFIC1", &Msg, 1) ;
  }

  // Nom de fichier (pour les fichiers statiques)
  if (string("") != pNewDocument->getFichier())
  {
	  pPatPathoMeta->ajoutePatho("0NFIC1", 1) ;
	  Msg.Reset() ;
	  Msg.SetTexteLibre(pNewDocument->getFichier()) ;
	  pPatPathoMeta->ajoutePatho("£?????", &Msg, 2) ;
  }

	// Date de rédaction
	pPatPathoMeta->ajoutePatho("KREDA1", 1) ;
	string sDateCreation = pNewDocument->getCreDate() ;
	if (strlen(sDateCreation.c_str()) == 8)
		sDateCreation += string("000000") ;
	Msg.Reset() ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateCreation) ;
	pPatPathoMeta->ajoutePatho("£T0;19", &Msg, 2) ;

	// Date du document
	string sDateExam = GetDateExamen(false) ;
	if (string("") != sDateExam)
	{
		pNewDocument->setDateExm(sDateExam) ;
		pPatPathoMeta->ajoutePatho("KCHIR2", 1) ;
    if (strlen(sDateExam.c_str()) == 8)
    	sDateExam += string("000000") ;
    Msg.Reset() ;
    Msg.SetUnit("2DA021") ;
    Msg.SetComplement(sDateExam) ;
    pPatPathoMeta->ajoutePatho("£T0;19", &Msg, 2) ;
	}
}

/**
 * Add document type to the metadata tree
 */
void
NSNoyauDocument::setContent(NSDocumentInfo* pNewDocument, NSPatPathoArray* pPatPathoMeta, string typeDoc)
{
  if (((NSPatPathoArray*) NULL == pPatPathoMeta) ||
      ((NSDocumentInfo*)  NULL == pNewDocument))
    return ;

  string sLexique = pNewDocument->getContent() ;

  if (string("") == sLexique)
  {
    // Content type : from
    //
	  if (false == _PatPathoArray.empty())
	  {
  	  PatPathoIter iter = _PatPathoArray.begin() ;
      sLexique = (*iter)->getLexique() ;
	  }
	  else
	  {
  	  //dans les 3 cas qui restent, les document n'ont pas patpatho, on leur
      //en crée une qui contient seulent le code lexique correspondant :
      //ZIMA01 pour image
      //PXXX51 : code bidon pour texte
      //PTRIQ3 : pour HTML

      NSSuper* pSuper = pContexte->getSuperviseur() ;
      if (pSuper->typeDocument(typeDoc, NSSuper::isText))
    	  // pour tous les types de texte
        sLexique = string("PXXX51") ;
      else if (pSuper->typeDocument(typeDoc, NSSuper::isHTML))
    	  // pour tous les types d'html statiques (HD traité plus haut)
        sLexique = string("PTRIQ3") ;
      else if (pSuper->typeDocument(typeDoc, NSSuper::isImage))
    	  // cas des images : générique pour tous les types d'image
        // y compris les images animées (vidéos)
        sLexique = string("ZIMA01") ;
	  }
  }

	if (string("") != sLexique)
	{
  	pNewDocument->setContent(sLexique) ;
    pPatPathoMeta->ajoutePatho("0TYPC1", 1) ;
    pPatPathoMeta->ajoutePatho(sLexique, 2) ;
	}
}

void
NSNoyauDocument::initMetaContentFromPpt(NSDocumentInfo* pNewDocument)
{
  if (_PatPathoArray.empty() || ((NSDocumentInfo*) NULL == pNewDocument))
    return ;

  PatPathoIter iter = _PatPathoArray.begin() ;
  string sLexique = (*iter)->getLexique() ;

  pNewDocument->setContent(sLexique) ;
}

string
NSNoyauDocument::getMetaId(NSPersonGraphManager* pGraphManager, NSDocumentInfo* pNewDocument, string sCodeDoc)
{
  if (((NSPersonGraphManager*) NULL == pGraphManager) || ((NSDocumentInfo*) NULL == pNewDocument))
    return string("") ;

  //
	// Calcul du code document des méta-données
	//
	if (string("") == sCodeDoc) // cas général
	{
		_sCodeDocMeta = pGraphManager->getDataGraph()->getNextTreeID() ;		pNewDocument->setPatient(string(_sCodeDocMeta, 0, PAT_NSS_LEN)) ;		pNewDocument->setDocument(string(_sCodeDocMeta, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN)) ;	}	else // cas où on passe le code méta root notament	{		pNewDocument->setPatient(pGraphManager->getPersonID()) ;		pNewDocument->setDocument(sCodeDoc) ;
		_sCodeDocMeta = pGraphManager->getPersonID() + sCodeDoc ;	}

  return _sCodeDocMeta ;
}

void
NSNoyauDocument::setPatPatho(const NSPatPathoArray* pPpt)
{
  if (NULL == pPpt)
    return ;

  _PatPathoArray = *pPpt ;
}

void
NSNoyauDocument::initFromPatPatho(NSPatPathoArray* pPpt) const
{
  if (NULL == pPpt)
    return ;

  *pPpt = _PatPathoArray ;
}

NSPatPathoInfo*
NSNoyauDocument::getRootPathoInfo() const
{
  if (_PatPathoArray.empty())
    return (NSPatPathoInfo*) 0 ;

  return *(_PatPathoArray.begin()) ;
}

// Fin de NsdocNoy.cpp////////////////////////////////////////////////////////////////////////

