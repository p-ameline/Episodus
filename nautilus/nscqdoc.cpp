//----------------------------------------------------------------------------
// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
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

#include "nautilus\nscqdoc.h"
#include "nautilus\nscqvue.h"
#include "nautilus\ns_html.h"
#include "nautilus\nshistdo.h"
#include "dcodeur\decoder.h"
#include "nautilus\nsldvdoc.h"
#include "nsbb\nsarc.h"
#include "nssavoir\nsgraphe.h"

# include "ns_ob1\InterfaceForKs.h"

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSCQDocument --------------------------
// --------------------------------------------------------------------------

NSCQDocument::NSCQDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
    					    NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx, bool bROnly,
                  string sDefaultArc, bool bDefArch)
                :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
  _iFrame       = ldvframeNotSpecified ;
	_pBigBoss     = (NSSmallBrother*) 0 ;
	_pParseur     = (nsarcParseur*) 0 ;
	// Il s'agit ici d'un document enregistré : le nom de l'archetype
	// sera donc récupéré dans la patpatho du document
	_sArchetype         = string("") ;
  _sDefaultArchetype = sDefaultArc ;
  _bForceDefaultArch = bDefArch ;

  _pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;

	_sHtmlView    = string("") ;
  _sSemDocType  = string("") ;

	_bCreate       = false ;
	_bParsingOk    = ouvreArchetype() ;
  if (_bParsingOk)
  {
    string sDocLib = findTitle() ;
    if (string("") != sDocLib)
      SetTitle(sDocLib.c_str()) ;
  }

	_iTypeDoc     = NSCQDocument::standard ;
}

NSCQDocument::NSCQDocument(TDocument* parent, NSContexte* pCtx, string sArc,
                  TYPEDOCARC iTypeFormulaire, string sDefaultArc, bool bDefArch)
             :NSRefDocument(parent, pCtx)
{
	_bReadOnly    = false ;
  _iFrame       = ldvframeNotSpecified ;

	_pBigBoss     = (NSSmallBrother*) 0 ;
	_pParseur     = (nsarcParseur*) 0 ;

	// Il s'agit ici d'un document vierge : on doit donc transmettre le nom
	// de l'archetype (car la patpatho est vide)
	_sArchetype        = sArc ;
  _sDefaultArchetype = sDefaultArc ;
  _bForceDefaultArch = bDefArch ;

  _pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;

	_sHtmlView    = string("") ;
  _sSemDocType  = string("") ;

	_bCreate      = true ;
	_bParsingOk   = ouvreArchetype() ;
  if (_bParsingOk)
  {
    string sDocLib = findTitle() ;
    if (string("") != sDocLib)
      SetTitle(sDocLib.c_str()) ;
  }

	_iTypeDoc     = iTypeFormulaire ;
}

NSCQDocument::NSCQDocument(TDocument* parent, NSContexte* pCtx, BB1BBInterfaceForKs* pKSdesc)
             :NSRefDocument(parent, pCtx)
{
	_bReadOnly = false ;
  _iFrame    = ldvframeNotSpecified ;

	_pBigBoss  = (NSSmallBrother*) 0 ;
	_pParseur  = (nsarcParseur*) 0 ;
	// Il s'agit ici d'un document vierge : on doit donc transmettre le nom
	// de l'archetype (car la patpatho est vide)
	_sArchetype         = pKSdesc->getArchetypeID() ;
  _sDefaultArchetype = string("") ;
  _bForceDefaultArch = false ;
  _pKSdescriptor     = new BB1BBInterfaceForKs(*pKSdesc) ;
	_sHtmlView         = string("") ;
  _sSemDocType       = string("") ;

	_bCreate       = true ;
  _pKSdescriptor->setInitFromBbk(true) ;

	_bParsingOk    = ouvreArchetype() ;
  if (_bParsingOk)
  {
    string sDocLib = findTitle() ;
    if (string("") != sDocLib)
      SetTitle(sDocLib.c_str()) ;
  }

	_iTypeDoc     = NSCQDocument::dpio ;
}

NSCQDocument::NSCQDocument(const NSCQDocument& rv)
             :NSRefDocument(rv)
{
	if (rv._pBigBoss)
		_pBigBoss = new NSSmallBrother(*(rv._pBigBoss)) ;
	else
		_pBigBoss = (NSSmallBrother*) 0 ;

  if (rv._pParseur)
	  _pParseur        = new nsarcParseur(*(rv._pParseur)) ;
  else
    _pParseur        = (nsarcParseur*) 0 ;

	_sArchetype        = rv._sArchetype ;  _sSemDocType       = rv._sSemDocType ;  _sDefaultArchetype = rv._sDefaultArchetype ;  _bForceDefaultArch = rv._bForceDefaultArch ;  _iFrame            = rv._iFrame ;  if (rv._pKSdescriptor)  	_pKSdescriptor = new BB1BBInterfaceForKs(*(rv._pKSdescriptor)) ;  else  	_pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;	_sHtmlView  = rv._sHtmlView ;	_bCreate    = rv._bCreate ;	_bParsingOk = rv._bParsingOk ;	_iTypeDoc   = rv._iTypeDoc ;}

NSCQDocument::~NSCQDocument()
{
	if (_pParseur)
		delete _pParseur ;

  if (_pKSdescriptor)
		delete _pKSdescriptor ;
}

NSCQDocument&
NSCQDocument::operator=(const NSCQDocument& src)
{
  if (this == &src)
		return *this ;

  // Copying the NSRefDocument that is the kernel of NSCQDocument
  //
  NSRefDocument* pNoyau = (NSRefDocument*) this ;
	*pNoyau = (NSCQDocument) src ;

  if (_pBigBoss)
  	delete _pBigBoss ;
	if (src._pBigBoss)
  	_pBigBoss = new NSSmallBrother(*(src._pBigBoss)) ;
	else
		_pBigBoss = (NSSmallBrother*) 0 ;

  if (_pParseur)
    delete _pParseur ;
  if (src._pParseur)
	  _pParseur        = new nsarcParseur(*(src._pParseur)) ;
  else
    _pParseur        = (nsarcParseur*) 0 ;


	_sArchetype        = src._sArchetype ;  _sDefaultArchetype = src._sDefaultArchetype ;  _bForceDefaultArch = src._bForceDefaultArch ;  _iFrame            = src._iFrame ;  if (_pKSdescriptor)  	delete _pKSdescriptor ;  if (src._pKSdescriptor)  	_pKSdescriptor = new BB1BBInterfaceForKs(*(src._pKSdescriptor)) ;  else  	_pKSdescriptor = (BB1BBInterfaceForKs*) 0 ;	_sHtmlView   = src._sHtmlView ;  _sSemDocType = src._sSemDocType ;	_bCreate     = src._bCreate ;	_bParsingOk  = src._bParsingOk ;	_iTypeDoc    = src._iTypeDoc ;
	return *this ;}

bool
NSCQDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
    						string sAdresseCorresp, string sPathDest)
{
	NSHtmlCQ hcq(typeOperation, this, pContexte, sAdresseCorresp) ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = hcq.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images  string sBaseImg = string("") ;	if (false == hcq.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
		return false ;

	// Mise à jour de la base d'images	switch (typeOperation)
	{
		case toComposer:
    	_sBaseCompo = sBaseImg ;
      break ;

    default:    	_sBaseImages = sBaseImg ;
	}

	return true ;
}

bool
NSCQDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  NSHtmlCQ hcq(tCR, this, pContexte, string("")) ;

  string sTitle = string("") ;
  string sDate  = string("") ;

  bool bTextFound = false ;
  for (int i = 0 ; i < hcq.nbHtml ; i++)
  {
    NSBlocHtml* pBlocHtml = hcq.blocHtml[i] ;
    if (pBlocHtml && (pBlocHtml->occur() > 0) && pBlocHtml->getHtml())
    {
      NSHtml*   pHtml = pBlocHtml->getHtml() ;
      HtmlTypes iType = NSHtml::typeTag(pHtml->getType()) ;

      switch(iType)
      {
        case tCQ :
          pHtml->genere(sHtmlText) ;
          if (string("") != sHtmlText)
            bTextFound = true ;
          break ;
        case tTitle :
          // pHtml->genere(sTitle) ;
          sTitle = pHtml->getText() ;
          break ;
        case tDate :
          pHtml->genere(sDate) ;
          break ;
      }
    }
  }

  if (string("") != sTitle)
  {
    if (string("") != sDate)
      sTitle += string(" (") + sDate + string(")") ;

    sHtmlText = string("<h2>") + sTitle + string("</h2><br>") + sHtmlText ;
  }

  return bTextFound ;
}

string
NSCQDocument::InitIntituleNoBirth()
{
	string sLibelleDoc = findTitle() ;

	string sDateDoc = GetDateDoc(false) ;
  char   datex[9] = "" ;
	strcpy(datex, sDateDoc.c_str()) ;
	int age = donne_age(datex) ;

  string sIntitulePatient ;
	donne_intitule_patient(&sIntitulePatient, age) ;
	string sIntituleDocument = sLibelleDoc + string(" de ") + sIntitulePatient + string(" ") ;
	sIntituleDocument += pContexte->getPatient()->getNom() + string(" ") ;
	sIntituleDocument += pContexte->getPatient()->getPrenom() ;	return sIntituleDocument ;
}

bool
NSCQDocument::ouvreArchetype()
{
	if (string("") == _sArchetype)
	{
		// cas des documents enregistrés : l'archetype doit etre
    // récupéré à partir de la patpatho
    if (false == _PatPathoArray.empty())
    	_sArchetype = (*(_PatPathoArray.begin()))->getArchetype() ;

    if (string("") == _sArchetype)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("archetypesManagement", "cannotFindArchetypeIdForDocument") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, 0) ;

      if (string("") == _sDefaultArchetype)
      	return false ;

      _sArchetype = _sDefaultArchetype ;
    }
  }

  if (_pParseur)
    delete _pParseur ;

	_pParseur = new nsarcParseur(pContexte->getSuperviseur()) ;

	string sArchetypeFile = pContexte->getSuperviseur()->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, _sArchetype) ;
	if (string("") == sArchetypeFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
  	sErrorText += string(" (") + _sArchetype + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	if (false == _pParseur->open(sArchetypeFile))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("archetypesManagement", "archetypeParsingError") ;
  	sErrorText += string(" (") + sArchetypeFile + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

  if (_pParseur->getArchetype())
  {
    Citem* pRootItem = _pParseur->getArchetype()->getRootItem() ;
    if (pRootItem)
    {
      string sRootCode = pRootItem->getCode() ;
      pContexte->getDico()->donneCodeSens(&sRootCode, &_sSemDocType) ;
    }

    Creferences *pRef = _pParseur->getArchetype()->getReference() ;
    if (pRef)
    {
      Cconcern *pConcern = pRef->getFirstCconcern() ;
      if (pConcern)
      {
        string sFrameCode = pConcern->getFrame() ;
        if (string("") != sFrameCode)
        {
          string sFrameSens = string("") ;
          pContexte->getDico()->donneCodeSens(&sFrameCode, &sFrameSens) ;

          for (int i = 0 ; i < FRAMECOUNT ; i++)
          {
            LDVFRAME iFrame = getFrameForIndex(i) ;
            string sRootForFrame = getRootForFrame(iFrame) ;
            string sSensForFrame = string("") ;
            pContexte->getDico()->donneCodeSens(&sRootForFrame, &sSensForFrame) ;

            if (sSensForFrame == sFrameSens)
              _iFrame = iFrame ;
          }
        }
      }
    }
  }

  if (ldvframeNotSpecified == _iFrame)
    _iFrame = ldvframeHealth ;

	return true ;
}

// Méthodes de gestion des formulaires
bool
NSCQDocument::enregistre(bool bVerbose, bool bSoaping)
{
  string ps = string("Entering NSCQDocument::enregistre") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  bool existeInfo ;

	if (_bEnregEnCours)
		return false ;

	_bEnregEnCours = true ;

	string sLang = pContexte->getUserLanguage() ;

	pContexte->getSuperviseur()->afficheStatusMessage("Début d'enregistrement") ;

	// Si c'est une nouvelle fiche, on la crée en tant que document
	bool bNewDoc ;
	if (NULL == _pDocInfo)
		bNewDoc = true ;
	else
		bNewDoc = false ;

	if (bNewDoc)
	{
    string sLibelleDoc = findTitle() ;

/*
		string sLibelleDoc = string("") ;

    Creferences *pReferenceBlock = pParseur->getArchetype()->getReference() ;
    if (pReferenceBlock)
    {
      Chead *pReferenceHead = pReferenceBlock->getHead(sLang) ;
      if (NULL != pReferenceHead)
        sLibelleDoc = pReferenceHead->getTitle() ;
    }

    if (string("") == sLibelleDoc)
    {
      if (string("") != _sSemDocType)
      {
        string sDocTypeComplet = _sSemDocType ;
        pContexte->getSuperviseur()->getDico()->donneCodeComplet(sDocTypeComplet) ;
			  pContexte->getDico()->donneLibelle(sLang, &sDocTypeComplet, &sLibelleDoc) ;
      }
      else if (false == pPatPathoArray->empty())
      {
    	  string sCode = (*(pPatPathoArray->begin()))->getLexique() ;
        pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
      }
    }
*/

		existeInfo = Referencer("ZCQ00", sLibelleDoc, "", "", true, bVerbose);
		if (false == existeInfo)
		{
			_bEnregEnCours = false ;
      ps = string("Quitting NSCQDocument::enregistre (Referencer failed)") ;
      pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
			return existeInfo ;
		}
	}

	// On enregistre la patpatho
	existeInfo = enregistrePatPatho() ;
	if (false == existeInfo)
	{
		_bEnregEnCours = false ;
    ps = string("Quitting NSCQDocument::enregistre (enregistrePatPatho failed)") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
		return existeInfo ;
	}

  // Loading the tree in order to get nodes' Ids
  //
	existeInfo = NSNoyauDocument::chargePatPatho() ;
	if (false == existeInfo)
	{
		_bEnregEnCours = false ;
    ps = string("Quitting NSCQDocument::enregistre (chargePatPatho failed)") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
		return existeInfo ;
	}

  connectToHealthConcern() ;

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

  ps = string("NSCQDocument::enregistre before openReferential()") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  openReferential() ;

  ps = string("Quitting NSCQDocument::enregistre") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	return existeInfo ;
}

string
NSCQDocument::findTitle()
{
  string sLang = pContexte->getUserLanguage() ;

  string sLibelleDoc = string("") ;

  if ((_pParseur) && (_pParseur->getArchetype()))
  {
    Creferences *pReferenceBlock = _pParseur->getArchetype()->getReference() ;
    if (pReferenceBlock)
    {
      Chead *pReferenceHead = pReferenceBlock->getHead(sLang) ;
      if (pReferenceHead)
        sLibelleDoc = pReferenceHead->getTitle() ;
      if (string("") != sLibelleDoc)
        return sLibelleDoc ;
    }
  }

  if (string("") != _sSemDocType)
  {
    string sDocTypeComplet = _sSemDocType ;
    pContexte->getDico()->donneCodeComplet(sDocTypeComplet) ;
    pContexte->getDico()->donneLibelle(sLang, &sDocTypeComplet, &sLibelleDoc) ;
    return sLibelleDoc ;
  }

  if (false == _PatPathoArray.empty())
  {
    string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
    pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
    return sLibelleDoc ;
  }

  return sLibelleDoc ;
}

string
NSCQDocument::getHelpUrl()
{
  string sLang = pContexte->getUserLanguage() ;

  string sLibelleDoc = string("") ;

  if (((nsarcParseur*) NULL == _pParseur) || (NULL == _pParseur->getArchetype()))
    return string("") ;

  return _pParseur->getArchetype()->getHelpUrl(sLang) ;
}

void
NSCQDocument::connectToHealthConcern()
{
  if (((nsarcParseur*) NULL == _pParseur) || (NULL == _pParseur->getArchetype()))
    return ;

  Creferences *pReferenceBlock = _pParseur->getArchetype()->getReference() ;
  if (NULL == pReferenceBlock)
    return ;

  Cconcern *pConcernBlock = pReferenceBlock->getFirstCconcern() ;
  if (NULL == pConcernBlock)
    return ;

  string sConcernCode = pConcernBlock->getCode() ;
  if (string("") == sConcernCode)
    return ;

  string sFrameCode = pConcernBlock->getFrame() ;

  bool bCreate = pConcernBlock->getAutoCreate() ;
  int  iSevere = pConcernBlock->getSeverity() ;

  // string ps = string("Connecting the document \"") + sLibelleDoc + string("\" to a line.") ;
  // pContexte->getSuperviseur()->trace(&ps, 1) ;
  NSLdvDocument *pDocLdv = pContexte->getPatient()->getLdvDocument() ;

  string sRacineDoc = DonneDocInfo()->getID() ;

  if (pContexte->getDico()->isDrugOrTreatment(&sConcernCode))
  {
    bool bConnected = false ;
    for (int i = 0 ; (i < FRAMECOUNT) && (false == bConnected) ; i++)
    {
      LDVFRAME iFrame = getFrameForIndex(i) ;
      bConnected = pDocLdv->connectObjectToDrug(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
    }
  }
  else if ('#' != sConcernCode[0])
  {
    LDVFRAME iFrame = ldvframeHealth ;
    if (string("") != sFrameCode)
      iFrame = pDocLdv->getFrameTypeFromRoot(sFrameCode) ;

    /*bConnected =*/ pDocLdv->connectObjectToConcern(iFrame, sConcernCode, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
  }
  // Si sConcernCode[0] == '#', c'est qu'on se connecte à
  // la préoccupation lanceuse du formulaire
  else
  {
    string sPreoType = string(sConcernCode, 1, strlen(sConcernCode.c_str()) - 1) ;
    string sQuestion = string("0PRO1/IsA/") + sPreoType ;
    NSPatPathoArray *pAnswer = NULL ;
#ifdef __OB1__
    string sAnswerDate ;
    if (pContexte->getBBinterface()->getAnswer2Question(sQuestion, "", &pAnswer, sAnswerDate, false))
#else
    if (pContexte->getSuperviseur()->getBlackboard()->getAnswer2Question(sQuestion, "", &pAnswer, false))
#endif // !__OB1__
    {
      if (pAnswer && (false == pAnswer->empty()))
      {
        PatPathoIter iter = pAnswer->begin() ;
        string sLaunchingPreoc = (*iter)->getLexique() ;

        LDVFRAME iFrame ;
        NSConcern* pConcern = pDocLdv->getConcern((*iter)->getNode(), iFrame) ;
        if (NULL != pConcern)
          pDocLdv->connectObjectToConcern(iFrame, sLaunchingPreoc, iSevere, sRacineDoc, NSRootLink::problemRelatedTo, bCreate) ;
      }
    }
    if (pAnswer)
      delete pAnswer ;
  }
}

void
NSCQDocument::openReferential()
{
  if (((nsarcParseur*) NULL == _pParseur) || (NULL == _pParseur->getArchetype()))
    return ;

  Cdialogue *pDialogBlock = _pParseur->getArchetype()->getDialog() ;
  if ((Cdialogue*) NULL == pDialogBlock)
    return ;

  string sNomRef = pDialogBlock->getStringAttribute(ATTRIBUT_DIALOGUE_REF) ;
  if (string("") == sNomRef)
    return ;

  string sRacineDoc = DonneDocInfo()->getID() ;

  pContexte->getPatient()->CmReferentiel(sNomRef, sRacineDoc, _iFrame) ;
}

string
NSCQDocument::donneTexte()
{
	if (_PatPathoArray.empty())
		return "" ;

	string sTexte = "" ;

	string sLangue = pContexte->getUserLanguage() ;
	string sLibelle ;
	string sCodeLex ;

	NSPatPathoArray aTempPatho(pContexte->getSuperviseur()) ;

	PatPathoIter iter = _PatPathoArray.begin() ;
	for ( ; _PatPathoArray.end() != iter ; iter++)
	{
  	// sCodeLex = string((*iter)->pDonnees->lexique) ;
    // pContexte->getDico()->donneLibelle(sLangue, &sCodeLex, &sLibelle) ;

    aTempPatho.vider() ;
    aTempPatho.push_back(new NSPatPathoInfo(**iter)) ;

    GlobalDkd Dcode(pContexte, sLangue, "", &aTempPatho) ;
    Dcode.decodeNoeud("") ;
    sLibelle = Dcode.getDcodeur() ;

    sTexte += sLibelle + string("<br>\n") ;
	}
	return sTexte ;
}

void
NSCQDocument::genereHtmlView()
{
	_sHtmlView = string("") ;

	if ((NULL == _pParseur) || (NULL == _pParseur->getArchetype()))
	{
		erreur("Il n'est pas possible d'accéder à l'archetype du document (qui contient les informations de publication).", standardError, 0) ;
		return ;
	}

	// Le parseur est initialisé par le constructeur
	Cpresentation* pPresent = _pParseur->getArchetype()->getPresentation() ;
	if (NULL == pPresent)
	{
		erreur("L'archetype du document ne possède pas d'informations de publication.", standardError, 0) ;
    return ;
	}

	string sPresent = pPresent->getValue() ;
	if (string("") != sPresent)
		remplaceTagsChemin(sPresent) ;
	_sHtmlView = sPresent ;
}

void
NSCQDocument::remplaceTagsChemin(string& sHtml)
{
  map<string, string> tagsBuffer ;

	// NSPatPathoArray* pPatPathoArray = NULL ;
	pContexte->getPatient()->remplaceTagsChemin(sHtml, &_PatPathoArray, &tagsBuffer) ;
  // if (pPatPathoArray)
	//	delete pPatPathoArray ;
}

// implementation des méthodes virtuelles de TDocument
bool
NSCQDocument::Open(int mode, const char far* path)
{
	if (NULL == _pDocInfo)
		return true ;

	return _bDocumentValide ;}

bool
NSCQDocument::Close()
{
	return true ;
}

void
NSCQDocument::SetTitle(LPCSTR title)
{
	TDocument::SetTitle(title) ;
}

// fin de nscqdoc.cpp
////////////////////////////////////////////////

