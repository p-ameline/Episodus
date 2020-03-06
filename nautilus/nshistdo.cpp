//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>
#include <owl\inputdia.h>
#include <owl\chooseco.h>
#include <owl\gdiobjec.h>
#include <owl\docmanag.h>
#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "nssavoir\nsBdmDriver.h"
#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nautilus\nsrechd2.h"
#include "nautilus\nsmodhtm.h"

#include "nautilus\nshistdo.h"
#include "nsbb\nstrnode.h"
#include "nautilus\nsldvdoc.h"
#include "nssavoir\nsgraphe.h"
#include "nautilus\nshistor.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nscsdoc.h"
#include "nautilus\nsdrugview.h"

long NSHISTODocument::lObjectCount = 0 ;

//-----------------------------------------------------------------------// fonction globale de tri :
//			afficher les documents dans ordre anti-chronologique : selon
//			le numéro de document
//-----------------------------------------------------------------------
bool
inferieur(NSDocumentHisto* s, NSDocumentHisto* b)
{
	if (s->getDateDoc() > b->getDateDoc())
		return true ;
	if (s->getDateDoc() < b->getDateDoc())
		return false ;
	return (s->getDocument() > b->getDocument()) ;
}

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSHISTODocument --------------------------
// --------------------------------------------------------------------------
NSHISTODocument::NSHISTODocument(TDocument* parent, NSContexte* pCtx, NSPatInfo* pPat)
                :TDocument(parent), NSRoot(pCtx), _DocumentsTank(pCtx)
{
  lObjectCount++ ;

  _pPerson = pPat ;

  string sLang = pContexte->getUserLanguage() ;

	string sCode = string("ZHIST1") ;
	pContexte->getDico()->donneLibelle(sLang, &sCode, &_sTitre) ;

	// sTitre = "Historique" ;
	SetTitle(_sTitre.c_str()) ;

	_pHtmlCS = (NSHtml*) 0 ;	AjouteDocument() ;}
//---------------------------------------------------------------------------//  Description : Destructeur
//---------------------------------------------------------------------------
NSHISTODocument::~NSHISTODocument()
{
  string sMsg = string("Entering NSHISTODocument destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  lObjectCount-- ;

/*
	// on ne fait pas de delete de pDocHis car pDocHis == this
	if (pContexte)
		if (pContexte->getUtilisateur())
			if (pContexte->getPatient())
				if (pContexte->getPatient()->getDocHis())
					pContexte->getPatient()->_pDocHis = (NSHISTODocument*) 0 ;
*/
  if (_pPerson)
    _pPerson->_pDocHis = (NSHISTODocument*) 0 ;

	if (_pHtmlCS)
		delete _pHtmlCS ;

/*
	NsHsitorTemplate* pHistTempl = TYPESAFE_DOWNCAST(GetTemplate(), NsHsitorTemplate) ;
	if (pHistTempl)
		delete pHistTempl ;
*/
}

//---------------------------------------------------------------------------//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSHISTODocument::NSHISTODocument(const NSHISTODocument& rv)
                :TDocument(rv), NSRoot(rv.pContexte), _DocumentsTank(rv._DocumentsTank)
{
  lObjectCount++ ;

try
{
	_sTitre  = rv._sTitre ;
  _pPerson = rv._pPerson ;

	if (rv._pHtmlCS)		_pHtmlCS = new NSHtml(*(rv._pHtmlCS)) ;
	else
		_pHtmlCS = (NSHtml*) 0 ;
}
catch (...)
{
  erreur("Exception NSHISTODocument copy ctor.", standardError, 0) ;
}
}
//---------------------------------------------------------------------------//  Description : Operateur =
//---------------------------------------------------------------------------

NSHISTODocument&NSHISTODocument::operator=(const NSHISTODocument& src)
{
try
{
	if (this == &src)
		return *this ;

	_sTitre        = src._sTitre ;
  _DocumentsTank = src._DocumentsTank ;
  _pPerson       = src._pPerson ;

  if (_pHtmlCS)
		delete _pHtmlCS ;

	if (src._pHtmlCS)		_pHtmlCS = new NSHtml(*(src._pHtmlCS)) ;
	else
		_pHtmlCS = (NSHtml*) 0 ;

	return *this ;}catch (...)
{
	erreur("Exception NSHISTODocument = operator.", standardError, 0) ;
	return *this ;
}
}
boolNSHISTODocument::Open(int mode, const char far* path)
{
	return true ;
}

boolNSHISTODocument::Close()
{
	return true ;
}

boolNSHISTODocument::CanClose()
{
	return TDocument::CanClose() ;
}

voidNSHISTODocument::SetTitle(LPCSTR title)
{
	TDocument::SetTitle(_sTitre.c_str()) ;
}

bool
NSHISTODocument::IsAttachedToOpenedPatient()
{
  return (_pPerson == (NSPatInfo*) pContexte->getPatient()) ;
}

NSCSDocument*
NSHISTODocument::GetOpenDocAsCs(NSDocumentInfo* pDocumentInfo)
{
  if ((NSDocumentInfo*) NULL == pDocumentInfo)
    return (NSCSDocument*) 0 ;

  NSNoyauDocument* pDocNoy = EstUnDocumentOuvert(pDocumentInfo) ;
  if ((NSNoyauDocument*) NULL == pDocNoy)
    return (NSCSDocument*) 0 ;

  NSCSDocument* pCSDoc = TYPESAFE_DOWNCAST(pDocNoy, NSCSDocument) ;

  return pCSDoc ;
}

NsHistorique*
NSHISTODocument::getHistorique()
{
  TView* pTView = GetViewList() ;
  if ((TView*) NULL == pTView)
		return (NsHistorique*) 0 ;

	NsHistorique* pNsHistorique = static_cast<NsHistorique*>(pTView) ;

  return pNsHistorique ;
}

bool
NSHISTODocument::ChargeDocInfo(NSDocumentInfo* pDocumentInfo)
{
	if ((NSDocumentInfo*) NULL == pDocumentInfo)
		return false ;

	string sCodeDocData = pDocumentInfo->getID() ;

	// on traite d'abord le cas des documents sans data
	// dans ce cas les pDonnees sont déjà chargées et contiennent
	// le code du document Meta
  //
	if (sCodeDocData == pDocumentInfo->getCodeDocMeta())
		return true ;

  if ((NSPatInfo*) NULL == _pPerson)
    return false ;

  // Looking for document's meta information from graph
  //
  NSPersonGraphManager* pGraphManager = _pPerson->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
  	return false ;

	NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
	VecteurString aVecteurString ;
	pGraphe->TousLesVrais(sCodeDocData, NSRootLink::docData, &aVecteurString, "ENVERS") ;
	if (aVecteurString.empty())
  {
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotAccessDocumentMetaData") ;
    sErrorText += string(" (") + sCodeDocData + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  // Meta information found
  //
  string sCodeDocMeta = *(*(aVecteurString.begin())) ;
  NSDocumentInfo NSDocInfo(sCodeDocMeta, pContexte, pGraphManager) ;

	NSDocInfo.setPatient(pDocumentInfo->getPatient()) ;
	NSDocInfo.setDocument(pDocumentInfo->getDocument()) ;

	if (false == NSDocInfo.ParseMetaDonnees())
  {
	  string sErrorText = pContexte->getSuperviseur()->getText("graphAndTreeErrors", "invalidDocumentLabelTree") ;
    sErrorText += string(" (") + NSDocInfo.getCodeDocMeta() ;
    if (string("") != NSDocInfo.getDocName())
      sErrorText += string(" - ") + NSDocInfo.getDocName() ;
    sErrorText += string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

	*pDocumentInfo = NSDocInfo ;

	return true ;
}
//---------------------------------------------------------------------------------// Ajouter un nouveau document à la fiche historique, l'enlever ou rafraichir
// les infos du document ou sa patpatho.
// Pour les documents nouvellement créés, on aura un pNouveauDocument != 0
// Par contre, si on rajoute à l'historique un document créé antérieurement,
// on aura pNouveauDocument == 0 (exemple NSDocuBox::Parametres dans nsrechdl.cpp)
//---------------------------------------------------------------------------------
void
NSHISTODocument::Rafraichir(NSDocumentInfo* pNSDocumentInfo,
                            const NSPatPathoArray* pNSPatPathoArray,
                            NSNoyauDocument* pNouveauDocument, bool bWarnBBk)
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

	NsHistorique* pNsHistorique = getHistorique() ;
  if ((NsHistorique*) NULL == pNsHistorique)
  	return ;

  //
  // Chercher pNSDocumentInfo dans VectDocument
  // Looking for pNSDocumentInfo inside VectDocument
  //
  DocumentIter iterDoc = getVectDocument()->TrouveDocHisto(pNSDocumentInfo->getID()) ;

  bool bNewDocument = ((NULL == iterDoc) || (getVectDocument()->end() == iterDoc)) ;

  bool bMustLogDoc = pContexte->getPredi()->mustLogPage() ;

  //
  // New document
  //
  if (bNewDocument)
  {
    // note : les NOUVEAUX documents (nouvellement créés) doivent
    // etre déclarés ouverts à la création (d'où le param pNouveauDocument)
    // warn : NEW document (that were just created) must be declared
    // open when created (it is the aim of the pNouveauDocument param)
    //
    if (pNSDocumentInfo->estVisible())
      pNsHistorique->Rafraichir(pNSDocumentInfo, pNSPatPathoArray, pNouveauDocument) ;
    //
    // Il vaut mieux re-trier, sinon la méthode DonnePrevPatPathoDocument ne
    // fonctionne pas
    //
    sort(getVectDocument()->begin(), getVectDocument()->end(), inferieur) ;

    AjouteALdV(pNSDocumentInfo) ;

    if (bMustLogDoc)
      pContexte->getPredi()->LogDocument(pNSDocumentInfo, pNSPatPathoArray, true) ;
  }
  //
  // Old document
  //
  else
  {
    NSDocumentHisto* pDocumentHisto = *iterDoc ;

    //
    // Update document
    //
    NSDocumentData InfoData ;
    pNSDocumentInfo->initFromData(&InfoData) ;
    pDocumentHisto->setData(&InfoData) ;

    // Updating presentation information
    //
    NSPatPathoArray PptPres(pContexte->getSuperviseur()) ;
    pNSDocumentInfo->initFromMeta(&PptPres) ;
    if (false == PptPres.empty())
      pDocumentHisto->setPres(&PptPres) ;

    // Updating meta data
    //
    NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
    pNSDocumentInfo->initFromMeta(&PptMeta) ;
    if (false == PptMeta.empty())
      pDocumentHisto->setMeta(&PptMeta) ;

    // Updating patpatho
    //
    if (pNSPatPathoArray)
      pDocumentHisto->setPatPatho(pNSPatPathoArray) ;

    pDocumentHisto->SetDateDoc() ;

    if (pNSDocumentInfo->estVisible())
    	//
      // Refresh history view
      //
			pNsHistorique->VisualiserPatho(pNSDocumentInfo, pNSPatPathoArray) ;
    else
    	pNsHistorique->EnleverDocument(pNSDocumentInfo) ;

    if (bMustLogDoc)
      pContexte->getPredi()->LogDocument(pNSDocumentInfo, pNSPatPathoArray, false) ;
  }

  // Show the new/updated tree to the blackboard
  //
  if (bWarnBBk && pContexte->getBBinterface() && IsAttachedToOpenedPatient())
  {
    string sErrorText = string("Signal BBK that a patpatho was saved") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trDetails) ;

    pContexte->getBBinterface()->signalThatPatpathoWasSaved(pNSPatPathoArray, bNewDocument) ;
  }

  // Update the drug alert information
  //
  if (pContexte->_pAlertBoxWindow)
  {
    if (pContexte->getPatient())
    {
      NSDrugView* pDrugView = pContexte->getPatient()->getDrugView() ;
      if (pDrugView)
        pDrugView->checkByBdm() ;
    }
  }

  /* Code when non visible documents were not part of VectDocument

  if (pNSDocumentInfo->estVisible())
  {
  	if ((NULL == iterDoc) || (VectDocument.end() == iterDoc))
    {
    	// note : les NOUVEAUX documents (nouvellement créés) doivent
      // etre déclarés ouverts à la création (d'où le param pNouveauDocument)
      // warn : NEW document (that were just created) must be declared
      // open when created (it is the aim of the pNouveauDocument param)
      pNsHistorique->Rafraichir(pNSDocumentInfo, pNSPatPathoArray, pNouveauDocument) ;
      //
      // Il vaut mieux re-trier, sinon la méthode DonnePrevPatPathoDocument ne
      // fonctionne pas
      //
      sort(VectDocument.begin(), VectDocument.end(), inferieur) ;

      AjouteALdV(pNSDocumentInfo) ;
    }
    else
    	//
      // Rafraîchir l'historique en tenant compte des changements
      // (exemple patpatho) de pNSDocumentInfo
      //
			pNsHistorique->VisualiserPatho(pNSDocumentInfo, pNSPatPathoArray) ;
	}
  //
  // Document non visible - Not visible document
  //
  else
  {
  	if ((iterDoc) && (VectDocument.end() != iterDoc))
    {
    	pNsHistorique->EnleverDocument(pNSDocumentInfo) ;
      delete *iterDoc ;
      VectDocument.erase(iterDoc) ;
    }
  } */
}

//-----------------------------------------------------------------------// demander l'ajout d'un nouveau document
//-----------------------------------------------------------------------
void
NSHISTODocument::AjouteDocument(NSDocumentInfo* pNSDocumentInfo)
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

	//
	// Ajout à l'historique
  // Add to history view
	//
  if (pNSDocumentInfo->estVisible())
  {
    NsHistorique* pNsHistorique = getHistorique() ;
    if (pNsHistorique)
      pNsHistorique->AjouterDocument(pNSDocumentInfo) ;
  }

  // Il vaut mieux re-trier, sinon la méthode DonnePrevPatPathoDocument ne
  // fonctionne pas
  //
  sort(getVectDocument()->begin(), getVectDocument()->end(), inferieur) ;

  // Ajout à la Ligne de vie
  //
	AjouteALdV(pNSDocumentInfo) ;
}

voidNSHISTODocument::AjouteALdV(NSDocumentInfo* pNSDocumentInfo)
{
	if (((NSDocumentInfo*) NULL == pNSDocumentInfo) || (false == IsAttachedToOpenedPatient()))
		return ;

	NSLdvDocument* pLdvDoc = pContexte->getPatient()->getLdvDocument() ;
	if ((NSLdvDocument*) NULL == pLdvDoc)
		return ;

	string sCodeDoc = pNSDocumentInfo->getID() ;
  string sDate    = string("") ;
  string sType    = pNSDocumentInfo->getTypeSem() ;
	string sTitle   = pNSDocumentInfo->getDocName() ;

	DocumentIter iterDoc = getVectDocument()->TrouveDocHisto(sCodeDoc) ;
	if ((iterDoc) && (getVectDocument()->end() != iterDoc))
		sDate = (*iterDoc)->getDateDoc() ;

	pLdvDoc->addObjet(sTitle, sDate, sDate, string(""), sCodeDoc, sType, pNSDocumentInfo->getContent()) ;
}

//-----------------------------------------------------------------------// demander l'autorisation de l'ouverture du document
//-----------------------------------------------------------------------
void
NSHISTODocument::AutoriserOuverture(NSDocumentInfo* pDocument)
{
  if ((NSDocumentInfo*) NULL == pDocument)
		return ;

  // No patient or patient being closed -> no way
  //
  NSPatientChoisi* pCurrentPatient = pContexte->getPatient() ;
  if (((NSPatientChoisi*) 0 == pCurrentPatient) || pCurrentPatient->isClosing())
    return ;

	NsHistorique* pNsHistorique = getHistorique() ;
  if ((NsHistorique*) NULL == pNsHistorique)
    return ;

  pNsHistorique->AutoriserOuverture(pDocument) ;
}

//-----------------------------------------------------------------------// demander l'autorisation de l'édition du document (cf fichiers Word...)
//-----------------------------------------------------------------------
void
NSHISTODocument::AutoriserEdition(NSDocumentInfo* pDocument)
{
	if ((NSDocumentInfo*) NULL == pDocument)
		return ;

  // No patient or patient being closed -> no way
  //
  NSPatientChoisi* pCurrentPatient = pContexte->getPatient() ;
  if (((NSPatientChoisi*) 0 == pCurrentPatient) || pCurrentPatient->isClosing())
    return ;

	NsHistorique* pNsHistorique = getHistorique() ;
  if ((NsHistorique*) NULL == pNsHistorique)
    return ;

  pNsHistorique->AutoriserEdition(pDocument) ;
}

/*** Fermeture d'un document - Closing a document*/
void
NSHISTODocument::FermetureDocument(NSDocumentInfo* pNSDocumentInfo)
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

	NsHistorique* pNsHistorique = getHistorique() ;
	if ((NsHistorique*) NULL == pNsHistorique)
    return ;

  pNsHistorique->FermetureDocument(pNSDocumentInfo) ;
}

//--------------------------------------------------------------------------// on affiche tous les documents dans un ordre chronologique
// on commence par touver les documents pour une chemise donnée, on les met dans
// pDocumentArray
//--------------------------------------------------------------------------
void
NSHISTODocument::AjouteDocument()
{
  NSPersonGraphManager* pGraphManager = _pPerson->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return ;

  string sTrace = string("Filling history with documents from the graph") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSteps) ;

	string sDocRoot = pGraphManager->getRootTree() ;
	NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;

  sTrace = string("Filling history with indexes") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Loading Frame indexes
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    VecteurString aFrameVectString ;
    pGraphe->TousLesVrais(sDocRoot, getLinkForFrame(iFrame), &aFrameVectString) ;
	  if (false == aFrameVectString.empty())
	  {
      for (EquiItemIter it = aFrameVectString.begin(); aFrameVectString.end() != it; it++)
      {
		    string sCodeDocIndex = *(*it) ;
        NSDocumentInfo DocInfo(sCodeDocIndex, pContexte, pGraphManager) ;

		    NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

        getVectDocument()->push_back(pNSDocumentHisto) ;
      }
	  }
  }

  // Loading Frame extension
  //
  VecteurString aFrameExtensionVectString ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personIndexExtension, &aFrameExtensionVectString) ;
  if (false == aFrameExtensionVectString.empty())
  {
    for (EquiItemIter it = aFrameExtensionVectString.begin() ; aFrameExtensionVectString.end() != it ; it++)
    {
      string sCodeDocIndex = *(*it) ;
      NSDocumentInfo DocInfo(sCodeDocIndex, pContexte, pGraphManager) ;

      NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

      getVectDocument()->push_back(pNSDocumentHisto) ;
    }
  }

  sTrace = string("Filling history with synthesis") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	// chargement de la synthèse
  //
  VecteurString aVecteurString ;
	pGraphe->TousLesVrais(sDocRoot, NSRootLink::personSynthesis, &aVecteurString) ;
  if (false == aVecteurString.empty())
  {
    for (EquiItemIter i = aVecteurString.begin() ; aVecteurString.end() != i ; i++)
    {
  	  string sCodeDocSynth = *(*i) ;
      NSDocumentInfo DocInfo(sCodeDocSynth, pContexte, pGraphManager) ;

      NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

      getVectDocument()->push_back(pNSDocumentHisto) ;
    }
  }

  sTrace = string("Filling history with demographics") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // chargement de la fiche administrative
  //
  aVecteurString.vider() ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personAdminData, &aVecteurString) ;
  if (false == aVecteurString.empty())
  {
    for (EquiItemIter i = aVecteurString.begin() ; aVecteurString.end() != i ; i++)
    {
  	  string sCodeDocAdmin = *(*i) ;
      NSDocumentInfo DocInfo(sCodeDocAdmin, pContexte, pGraphManager) ;

      NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

      getVectDocument()->push_back(pNSDocumentHisto) ;
    }
  }

  sTrace = string("Filling history with health team") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	// fabTODO
	// chargement de l'Equipe de Santé
	// Loading HealthTeam
	aVecteurString.vider() ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personHealthTeam, &aVecteurString) ;
  if (false == aVecteurString.empty())
  {
    for (EquiItemIter i = aVecteurString.begin() ; aVecteurString.end() != i ; i++)
    {
  	  string	sCodeDocAdmin = *(*i) ;
      NSDocumentInfo DocInfo(sCodeDocAdmin, pContexte, pGraphManager) ;

      NSDocumentHisto *pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

      getVectDocument()->push_back(pNSDocumentHisto) ;
    }
  }

  sTrace = string("Filling history with folders library") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // chargement de la bibliothèque de chemises
  // Note : ce document est stocké à part dans pDocHis
  aVecteurString.vider() ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personFolderLibrary, &aVecteurString) ;
  if (false == aVecteurString.empty())
  {
    for (EquiItemIter i = aVecteurString.begin() ; aVecteurString.end() != i ; i++)
    {
  	  string sCodeDocLibChem = *(*i) ;
      NSDocumentInfo DocInfo(sCodeDocLibChem, pContexte, pGraphManager) ;

      _DocumentsTank.setLibChem(new NSDocumentHisto(&DocInfo)) ;
      if (false == getLibChem()->LoadMetaAndData(pGraphManager))
      {
        string sErrorText = pContexte->getSuperviseur()->getText("folderErrors", "cannotLoadFoldersInformation") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      }
    }
  }

  sTrace = string("Filling history with documents") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // chargement de tous les documents
  //
  aVecteurString.vider() ;
  pGraphe->TousLesVrais(sDocRoot, NSRootLink::personDocument, &aVecteurString) ;
  if (false == aVecteurString.empty())
  {
  	for (EquiItemIter i = aVecteurString.begin(); i != aVecteurString.end(); i++)
    {
      string sCodeDocLibChem = *(*i) ;
      NSDocumentInfo DocInfo(sCodeDocLibChem, pContexte, pGraphManager) ;

    	NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;
      getVectDocument()->push_back(pNSDocumentHisto) ;
    }
  }

	// Chargement de la couche de données
	///////////////////////////////////////

  sTrace = string("Filling history with structured data") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	// on s'occuppe d'abord de la bibliothèque de chemises
  //
	if ((getLibChem()) && (string("") != getLibChem()->getID()))
	{
    NSPatPathoArray PptLibChem(pContexte->getSuperviseur()) ;
    if (false == getLibChem()->DonnePatPatho(&PptLibChem, pGraphManager))
    {
      string sErrorText = pContexte->getSuperviseur()->getText("folderErrors", "cannotLoadFoldersInformation") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    }
    else
      getLibChem()->setPatPatho(&PptLibChem) ;
  }

	// on traite maintenant tous les documents visibles
  //
  DocumentIter iterDoc = getVectDocument()->begin() ;
  bool bOk ;

	while (getVectDocument()->end() != iterDoc)	{
    // Reseting patpatho
    //
    NSPatPathoArray PptIter(pContexte->getSuperviseur()) ;
  	(*iterDoc)->setPatPatho(&PptIter) ;

    bOk = false ;

    // On remonte le lien data du méta-document
    //
    aVecteurString.vider() ;
    pGraphe->TousLesVrais((*iterDoc)->getCodeDocMeta(), NSRootLink::docData, &aVecteurString) ;

    if (false == aVecteurString.empty())
    {
    	string sCodeDocData = *(*(aVecteurString.begin())) ;
      string sCodePat = string(sCodeDocData, 0, PAT_NSS_LEN) ;
      string sCodeDoc = string(sCodeDocData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

      (*iterDoc)->setPatient(sCodePat) ;
      (*iterDoc)->setDocument(sCodeDoc) ;

      if (false == (*iterDoc)->ParseMetaDonnees())
      {
        string sErrorText = pContexte->getSuperviseur()->getText("graphAndTreeErrors", "invalidDocumentLabelTree") ;
        sErrorText += string(" (") + (*iterDoc)->getCodeDocMeta() ;
        if (string("") != (*iterDoc)->getDocName())
          sErrorText += string(" - ") + sCodeDocData ;
        sErrorText += string(")") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      }
      else
      {
        // Modif 20/10/2010 VectDocument now also contains non visible docs
        // if (((*iterDoc)->estVisible()) && ((*iterDoc)->DonnePatPatho((*iterDoc)->pPatPathoArray, pGraphManager)))
        if ((*iterDoc)->DonnePatPatho(&PptIter, pGraphManager))
        {
          (*iterDoc)->setPatPatho(&PptIter) ;

          if (PptIter.empty())
          {
            string sErrorText = pContexte->getSuperviseur()->getText("graphAndTreeErrors", "cannotFindDataTree") ;
            sErrorText += string(" (") + sCodeDocData ;
            if (string("") != (*iterDoc)->getDocName())
              sErrorText += string(" - ") + sCodeDocData ;
            sErrorText += string(")") ;
            pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		        erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          }
          else
          {
      	    if ((*iterDoc)->checkPptConsistency())
            {
      		    bOk = true ;
              if ((*iterDoc)->getContent() == string("ZSYNT1"))
                (*iterDoc)->checkSynthesisConsistency() ;
            }
          }
        }
      }
    }
    else
    {
    	// méta-document sans lien data
      // NOTE : pour ce type de documents on met dans pDonnees le code document du Meta
      // cela permet de les retrouver dans l'historique

      string sCodePat = string((*iterDoc)->getCodeDocMeta(), 0, PAT_NSS_LEN) ;
      string sCodeDoc = string((*iterDoc)->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

      (*iterDoc)->setPatient(sCodePat) ;
      (*iterDoc)->setDocument(sCodeDoc) ;

      if (false == (*iterDoc)->ParseMetaDonnees())
      {
        string sErrorText = pContexte->getSuperviseur()->getText("graphAndTreeErrors", "invalidDocumentLabelTree") ;
        sErrorText += string(" (") + (*iterDoc)->getCodeDocMeta() ;
        if (string("") != (*iterDoc)->getDocName())
          sErrorText += string(" - ") + (*iterDoc)->getCodeDocMeta() ;
        sErrorText += string(")") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      }
      else
      {
        // Modif 20/10/2010 VectDocument now also contains non visible docs
        // if ((*iterDoc)->estVisible())
        // {
        NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
        (*iterDoc)->initFromPatPatho(&PPT) ;

        PPT.ajoutePatho("ZDOCU1", 0) ;

        (*iterDoc)->setPatPatho(&PPT) ;
        bOk = true ;
        // }
      }
    }

    //problème de récupération de la patpatho (ou de parsing) : le document
    //ne figure pas dans l'historique

    if (false == bOk)    {
    	delete *iterDoc ;
      getVectDocument()->erase(iterDoc) ;
    }
    else
    	iterDoc++ ;
	}

  sTrace = string("Sorting history entries") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

	sort(getVectDocument()->begin(), getVectDocument()->end(), inferieur) ;

  sTrace = string("History has been filled") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSteps) ;
}

//-----------------------------------------------------------------------//demander la patpatho de la synthèse
//-----------------------------------------------------------------------
/*
void
NSHISTODocument::DonnePathoSynthese(NSPatPathoArray* pPathoSynthese)
{
  DocumentReverseIter iterReverseDocEnd = VectDocument.rend();
	for (DocumentReverseIter iterReverseDoc = VectDocument.rbegin(); iterReverseDoc != iterReverseDocEnd; iterReverseDoc++)
    {
        PatPathoIter iter = (*iterReverseDoc)->pPatPathoArray->begin();
        if (strcmp((*iter)->pDonnees->lexique, "ZSYNT1") == 0)
        {
      	    *pPathoSynthese = *((*iterReverseDoc)->pPatPathoArray);
            return;
        }
    }
}  */

//--------------------------------------------------------------------------//on affiche tous les documents dans un ordre chronologique
//on commence par touver les documents pour une chemise donnée, on les met dans
//pDocumentArray
//--------------------------------------------------------------------------
void
NSHISTODocument::AjouteDocumentParChemise()
{
}

//-----------------------------------------------------------------------------------
// Activer la vue correspondant au document (qui est ouvert)
// Recherche le type de vue correspondant au document ouvert en fonction
// du type document -- correspond aux vues lancées par NSSuper::OuvreDocument
// Note : si le type document est connu, on peut alors activer une vue
// spécifique du document en passant sViewName != "" (cf NSTreeHis::AutoriserEdition)
// Valuer de retour : retourne un pointeur sur la vue (TView*) si ok, 0 sinon.
//-----------------------------------------------------------------------------------
TView*
NSHISTODocument::ActiveFenetre(NSDocumentInfo* pDocumentInfo, string sViewName)
{
	if ((NSDocumentInfo*) NULL == pDocumentInfo)
		return (TView*) 0 ;

	TView*           pView   = (TView*) 0 ;
	NSNoyauDocument* pDocNoy = 0;

	if (false == getVectDocumentOuvert()->empty())
	{
		string codeDocHisto = pDocumentInfo->getID() ;

    for (DocumentIter iterDoc = getVectDocumentOuvert()->begin() ; getVectDocumentOuvert()->end() != iterDoc ; iterDoc++)
    {
    	if ((*iterDoc)->getID() == codeDocHisto)
      {
      	pDocNoy = (*iterDoc)->getDocNoy() ;
        break ;
      }
    }
  }

	if ((NSNoyauDocument*) NULL == pDocNoy)
		return (TView*) 0 ;

	// on considère ici le type du pDocNoy, qui n'est pas nécéssairement
	// le type du DocHisto où il est stocké (cas des HDHTM)
	// on regarde donc le type de pDocNoy->pDocInfo qui correspond au document brut.
	//
	string sTypeDoc = string("") ;
	if (pDocNoy->_pDocInfo)
		sTypeDoc = string(pDocNoy->_pDocInfo->getTypeSem()) ;
	string sTypeVue = sViewName ;

	if (string("") == sTypeVue)
	{
		// on prend dans ce cas la vue d'ouverture par défaut
		// (cf NSuper::OuvreDocument)
		if      (string("ZCN00") == sTypeDoc)
    	sTypeVue = "NSCRReadOnlyView" ;
    else if (string("ZCS00") == sTypeDoc)
    	sTypeVue = "NSCsVue" ;
    else if (string("ZTRTF") == sTypeDoc)
    	sTypeVue = "NSTtxView" ;
    else if (string("ZTHTM") == sTypeDoc)
    	sTypeVue = "NSVisualView" ;
    else if (pContexte->typeDocument(sTypeDoc, NSSuper::isHTML)) // cas des HSHTM et des HIHTM
    	sTypeVue = "NSVisualView" ;
    else if (pContexte->typeDocument(sTypeDoc, NSSuper::isImage)) // cas des IFxxx et des IAxxx
    	sTypeVue = "NSVisualView" ;
    else
    	return (TView*) 0 ;           // ne correspond à aucun type de document
	}

	// on parcourt la liste des vues de ce document
	//
	for (pView = pDocNoy->GetViewList() ; 0 != pView ; pView = pView->GetNextView())
	{
		if (string(pView->GetViewName()) == sTypeVue)
		{
			// si on a trouvé une vue valide, on active sa fenetre Parent (TMDIChild)
      // car en général la vue elle-même (TWindow) ne concerne que la zone client
      SetFocus(pView->GetWindow()->Parent->GetHandle()) ;
      break ;
    }
  }

	return pView ;
}

//----------------------------------------------------------------------// Enumérer toutes les fenêtres filles et activer celle dont le titre
// est sTitre
// NOTE : depuis la modif du 7/11/00, le document est stocké dans
// VectDocumentOuvert, et cette fonction a été remplacée par
// ::ActiveFenetre(NSDocumentInfo* pDocumentInfo)
// -- on conserve le code pour le fonctionnement de EnumChildWindows
//----------------------------------------------------------------------
void
NSHISTODocument::ActiveFenetre(string sTitre)
{
	/* HINSTANCE hInstance = */ pContexte->getSuperviseur()->getApplication()->GetInstance() ;
	WNDENUMPROC lpEnumDlgFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) NSHISTODocument::EnumereVues, hInstance) ;
	//
	// On passe comme paramètre de EnumChildWindows LPARAM(this), l'adresse de
	// this puisque dans les fonctions static on ne peut pas récupérer
	// le pointeur this.
	//
	EnumChildWindows(pContexte->getSuperviseur()->getApplication()->prendClient()->GetHandle(), lpEnumDlgFunc,
                 LPARAM((string*)(&sTitre))) ;
}

//--------------------------------------------------------------------//--------------------------------------------------------------------
BOOL FAR PASCAL _export NSHISTODocument::EnumereVues(HWND hWnd, LPARAM lParam)
{
	//
	// Adresse de l'objet boîte de dialogue courante
	//
	string* pTitre = reinterpret_cast<string*>(lParam) ;
  string  sTitreFenetre = string("") ;

	if (IsWindow(hWnd))
	{
  	//GetWindowText(hWnd, lpString, 255);
    int length = GetWindowTextLength(hWnd) ;

    // on limite le titre à 255 car GetWindowText renvoie
    // le texte des fenetres RichEdit
    if ((length > 0) && (length <= 255))
    {
      char szTitre[256] ;
    	GetWindowText(hWnd, szTitre, length + 1) ;
      sTitreFenetre = string(szTitre) ;
    }

    if (*pTitre == sTitreFenetre)
    {
    	SetFocus(hWnd) ;
      //      return FALSE;
    }
  }
  return TRUE ;
}

