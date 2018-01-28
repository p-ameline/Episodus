//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nsbb\nsbbsmal.h"
#include "nautilus\nsrechd2.h"

#include "nautilus\nscsdoc.h"
#include "nautilus\nscsvue.h"

#include "nautilus\nshistdo.h"
#include "nautilus\ns_html.h"
#include "nsepisod\nssoapdiv.h"
#include "nautilus\nsldvdoc.h"
#include "nautilus\nschoisi.h"

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSCSDocument --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
//	Constructeur à partir d'un document existant
//
//---------------------------------------------------------------------------
NSCSDocument::NSCSDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
                           NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                           string sTypeDocum, bool bROnly)
			       :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	_sTitre		     = string("") ;
	_pBigBoss	     = (NSSmallBrother*) 0 ;
	_sTypeDocument = sTypeDocum ;
	_pHtmlCS       = (NSHtml*) 0 ;
}

//---------------------------------------------------------------------------
//
//	Constructeur pour un nouveau document
//
//---------------------------------------------------------------------------
NSCSDocument::NSCSDocument(TDocument* parent, NSContexte* pCtx, string sTypeDocum)
             :NSRefDocument(parent, pCtx)
{
	if (pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))
		_bReadOnly = false ;
  else
		_bReadOnly = true ;

	_sTypeDocument = sTypeDocum ;
	_sTitre		     = string("") ;
	_pBigBoss	     = (NSSmallBrother*) 0 ;
	_pHtmlCS		   = (NSHtml*) 0 ;
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSCSDocument::NSCSDocument(const NSCSDocument& rv)
             :NSRefDocument(rv)
{
try
{
	if (rv._pBigBoss)
		_pBigBoss = new NSSmallBrother(*(rv._pBigBoss)) ;
	else
  	_pBigBoss = (NSSmallBrother*) 0 ;

	if (rv._pHtmlCS)
  	_pHtmlCS = new NSHtml(*(rv._pHtmlCS)) ;
  else
  	_pHtmlCS = (NSHtml*) 0 ;

	_sTitre        = rv._sTitre ;
	_sTypeDocument = rv._sTypeDocument ;
}
catch (...)
{
  erreur("Exception NSCSDocument ctor.", standardError, 0) ;
  return ;
}
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSCSDocument::~NSCSDocument()
{
	if ((pContexte) && (pContexte->getPatient()))
		if (pContexte->getPatient()->_pConsultEnCours == this)
    	pContexte->getPatient()->_pConsultEnCours = (NSCSDocument*) 0 ;

	if (_pHtmlCS)
		delete _pHtmlCS ;
}

//---------------------------------------------------------------------------
//  Description : Operateur =
//---------------------------------------------------------------------------
NSCSDocument&
NSCSDocument::operator=(const NSCSDocument& src)
{
try
{
	if (this == &src)
  	return *this ;

  // Appel de l'operateur = de NSRefDocument
  // (recopie des arguments de la classe NSRefDocument)
  //
  NSRefDocument       *dest   = this ;
  const NSRefDocument *source = (NSRefDocument*) &src ;
  *dest  = *source ;

  if (src._pBigBoss)
  	_pBigBoss = new NSSmallBrother(*(src._pBigBoss)) ;
  else
  	_pBigBoss = (NSSmallBrother*) 0 ;

  if (src._pHtmlCS)
  	_pHtmlCS = new NSHtml(*(src._pHtmlCS)) ;
  else
  	_pHtmlCS = (NSHtml*) 0 ;

	_sTitre        = src._sTitre ;
  _sTypeDocument = src._sTypeDocument ;

	return *this ;
}
catch (...)
{
  erreur("Exception NSCSDocument = operator.", standardError, 0) ;
  return *this ;
}
}

string
NSCSDocument::InitIntituleNoBirth()
{
try
{
	NSHtml htmlTitre(tTitre2) ;

  string sLang = pContexte->getUserLanguage() ;

  string sLibelleDoc = string("") ;

  if (false == _PatPathoArray.empty())
  {
  	string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
    if (string("") != sCode)
    {
    	pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
      if (strlen(sLibelleDoc.c_str()))
      	sLibelleDoc[0] = pseumaj(sLibelleDoc[0]) ;
      else
      	sLibelleDoc = string("Document de synthèse") ;
    }
    else
    	sLibelleDoc = string("Document de synthèse") ;
	}

  if ((NSSmallBrother*) NULL == _pBigBoss)
  	_pBigBoss = new NSSmallBrother(pContexte, &_PatPathoArray) ;

  string sDatex = string("") ;

  // on cherche d'abord dans la patpatho
  //
  string sDateExamen = _PatPathoArray.GetDocDate() ;

  if (string("") == sDateExamen)
  {
    if (_pDocInfo)
    	sDatex = _pDocInfo->getCreDate() ;
    else
    	sDatex = donne_date_duJour() ;
	}
  else
  	sDatex = sDateExamen ;

  if (pContexte->getPatient()->getNomLong() == string(""))
  	pContexte->getPatient()->fabriqueNomLong() ;

	string sIntitulePatient = pContexte->getPatient()->getNomLong() ;	return sLibelleDoc + string(" de ") + sIntitulePatient ; ;
}
catch (...)
{
	erreur("Exception NSCSDocument::InitIntitule.", standardError, 0) ;
	return string("") ;
}
}void
NSCSDocument::activate()
{
	TView* pView = GetViewList() ;
	do
	{
		NSCsVue* pCsView = TYPESAFE_DOWNCAST(pView, NSCsVue) ;
		if (pCsView)
			//pCsView->SetFocus() ;
    	SetFocus(pCsView->GetWindow()->Parent->GetHandle()) ;

		pView = NextView(pView) ;
	}
  while (pView) ;
}

//---------------------------------------------------------------------------//  Function: NSCSDocument::GenereHtml(string sPathHtml, string& sNomHtml, string sAdresseCorresp)
//
//  Arguments:
//				  sPathHtml : 	répertoire destination
//				  sNomHtml :   En entrée -> nom du fichier à générer
//									En sortie -> nom sans doublons
//				  sAdresseCorresp : nom + adresse du correspondant
//  Description:
//				  Génère un fichier html CR dans le répertoire sPathName
//				  Utilise les données sTemplate et sBaseImages
//  Returns:
//            true : OK, false : sinon
//---------------------------------------------------------------------------
bool
NSCSDocument::GenereHtml(string sPathHtml, string& sNomHtml,
                         OperationType typeOperation, string sAdresseCorresp,
                         string sPathDest)
{
	string   sFichierHtml = string("") ;
	NSHtmlCS hcs(typeOperation, this, pContexte, sAdresseCorresp) ;
  string   sBaseImg = string("") ;

	// on trouve le nom du fichier temporaire à visualiser
  sNomHtml = hcs.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
  sFichierHtml = sPathHtml + sNomHtml ;

  // on passe aussi le path destination pour les images
  if (false == hcs.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
  	return false ;

	// Mise à jour de la base d'images
  switch (typeOperation)
  {
  	case toComposer:
    	_sBaseCompo = sBaseImg ;
      break ;

    default:
    	_sBaseImages = sBaseImg ;
	}
	return true ;}

bool
NSCSDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  NSHtmlCS hcs(tCR, this, pContexte, string("")) ;

  string sOut = string("") ;

  for (int i = 0 ; i < hcs.nbHtml ; i++)
  {
    NSHtml* pHtml = hcs.blocHtml[i]->getHtml() ;
    if (pHtml && (pHtml->getType() == string("CS")) && (hcs.blocHtml[i]->occur()))
    {
      pHtml->genere(sHtmlText) ;
      return true ;
    }
  }

  return false ;
}

bool
NSCSDocument::GenereRawText(string& sRawText)
{
  if ((NSHtml*) NULL == _pHtmlCS)
    return false ;

  NSHtmlArray* pSons = _pHtmlCS->getSons() ;
  if (pSons->empty())
    return false ;

  string sDecal = string("") ;

  HtmlIter HtmlIt = pSons->begin() ;
  for ( ; pSons->end() != HtmlIt ; HtmlIt++)
    if (string("CS") == (*HtmlIt)->getType())
      GenereRawTextForBloc(sRawText, (*HtmlIt)->getSons(), sDecal) ;

  return true ;
}

void
NSCSDocument::GenereRawTextForBloc(string& sRawText, NSHtmlArray *pFilsArray, string sDecal)
{
  if (((NSHtmlArray*) NULL == pFilsArray) || (pFilsArray->empty()))
    return ;

  HtmlIter HtmlIt = pFilsArray->begin() ;
  for ( ; pFilsArray->end() != HtmlIt ; HtmlIt++)
	{
    sRawText += (*HtmlIt)->getText() + string(NEWLINE) ;

    GenereRawTextForBloc(sRawText, (*HtmlIt)->getSons(), sDecal + string("   ")) ;
  }
}

//----------------------------------------------------------------//------------------------------------------------------------------
bool
afficheCompteRendu(NSDocumentInfo& Document, NSContexte *pCtx)
{
	/*NSCRReadOnlyTemplate* DocVisuTemplate = new NSCRReadOnlyTemplate("Compte rendu","",0,"",dtAutoDelete|dtUpdateDir);
	NSCRDocument*			 DocVisuDoc 	  = new NSCRDocument(0, &Document, pSuper);
	DocVisuDoc->SetTemplate(DocVisuTemplate);
	DocVisuTemplate->CreateView(*DocVisuDoc);*/
	return false ;
}
//---------------------------------------------------------------------------//  Ouvre le document en mettant la fiche dans pPatPathoArray
//---------------------------------------------------------------------------
bool
NSCSDocument::Open(int mode, const char far* path)
{
	if (NULL == _pDocInfo)
		return true ;

	_sTitre = _pDocInfo->getDocName() ;
	strip(_sTitre) ;

	return _bDocumentValide ;
}

bool
NSCSDocument::Close()
{
	//sCompteRendu = "" ;
	return true ;
}

void
NSCSDocument::SetTitle(LPCSTR title)
{
#ifndef _INCLUS
	TDocument::SetTitle(title);
#endif
}

//---------------------------------------------------------------------------
//  Enregistre le document
//---------------------------------------------------------------------------
bool
NSCSDocument::enregistre(bool bVerbose, bool bSoaping)
{
	if (_bEnregEnCours)
		return false ;

	_bEnregEnCours = true ;

	string sLang = pContexte->getUserLanguage() ;

	pContexte->getSuperviseur()->afficheStatusMessage("Début d'enregistrement");

	// Si c'est une nouvelle fiche, on la crée en tant que document
	bool bNewDoc = false ;
	if (NULL == _pDocInfo)
		bNewDoc = true ;

  bool bExisteInfo ;

	if (bNewDoc)
	{
		string sLibelleDoc ;
    if (string("") != _sTypeDocument)
			pContexte->getDico()->donneLibelle(sLang, &_sTypeDocument, &sLibelleDoc) ;
    else if (false == _PatPathoArray.empty())
    {
    	string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
      pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
    }

		bExisteInfo = Referencer("ZCS00", sLibelleDoc, "", "", true, bVerbose) ;
		if (false == bExisteInfo)
		{
			_bEnregEnCours = false ;
			return bExisteInfo ;
		}
	}

	// On enregistre la patpatho
	bExisteInfo = enregistrePatPatho() ;
	if (false == bExisteInfo)
	{
		_bEnregEnCours = false ;
		return bExisteInfo ;
	}

	bExisteInfo = NSNoyauDocument::chargePatPatho() ;
	if (false == bExisteInfo)
	{
		_bEnregEnCours = false ;
		return bExisteInfo ;
	}

  // The new document must be inserted in "history" before checking if it resets
  // any goal, because this document's date will be asked to "history"

  // On prévient l'historique (fait à part pour les CS et les CN)
	pContexte->getPatient()->getDocHis()->Rafraichir(_pDocInfo, &_PatPathoArray, this) ;

	// Si c'est un nouveau compte rendu on vérifie s'il remet à zéro un objectif
	if ((bNewDoc) && (pContexte->getPatient()->getLdvDocument()))
		pContexte->getPatient()->getLdvDocument()->showNewTree(&_PatPathoArray, GetDateDoc(false)) ;

	pContexte->getSuperviseur()->afficheStatusMessage("Document enregistré") ;

	_bEnregEnCours = false ;

	if (bSoaping)
		makeSOAP() ;

	return bExisteInfo ;
}

void
NSCSDocument::invalidateViews(string sReason)
{
	//
	// Valeurs possibles de sReason - Possible values for sReason
	//
	// INFORMATION_NEW
  //
	TView* pView = GetViewList() ;
  if ((TView*) NULL == pView)
  	return ;

	do
	{
		// Rosace
		//
		NSCsVue* pCsView = TYPESAFE_DOWNCAST(pView, NSCsVue) ;
		if (pCsView)
			pCsView->reloadView(sReason) ;

		pView = NextView(pView) ;
	}
	while (pView) ;
}

