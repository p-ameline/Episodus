#ifdef _ENTERPRISE_DLL  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "partage\nsdivfct.h"
#endif
#include "nsbb\nsmanager.h"#include "nsdn\nsdochis.h"#include "nsdn\nsdocnoy.h"#include "nsbb\nsbbtran.h"#include "nsbb\nspatpat.h"#include "nsbb\nstrnode.h"#include "nssavoir\nsfilgd.h"#include "partage\ns_search.h"
long NSDocumentHisto::lObjectCount = 0 ;
long NSDocHistoArray::lObjectCount = 0 ;
long NSDocumentsTank::lObjectCount = 0 ;

//---------------------------------------------------------------------------//  Fonction:		NSDocumentHisto::NSDocumentHisto(NSContexte* pCtx)
//  Description:	Constructeur avec contexte (pour l'accès à la base des chemins)
//  Retour:			Rien
//---------------------------------------------------------------------------
NSDocumentHisto::NSDocumentHisto(NSContexte* pCtx)
                :NSDocumentInfo(pCtx), _PatPathoArray(pCtx->getSuperviseur())
{
	_pDocNoy  = (NSNoyauDocument*) 0 ;
	_sDateDoc = string("") ;

  lObjectCount++ ;
}

NSDocumentHisto::NSDocumentHisto(NSDocumentInfo* pNSDocument, bool /* bNoDocnoy */)
                :NSDocumentInfo(*pNSDocument),
                    _PatPathoArray(pNSDocument->pContexte->getSuperviseur())
{
	_pDocNoy  = (NSNoyauDocument*) 0 ;
	_sDateDoc = string("") ;

  lObjectCount++ ;
}

//-----------------------------------------------------------------------------------//  Fonction:		NSDocumentHisto::NSDocumentHisto(NSDocumentInfo* pNSDocumentInfo)
//  Description:	Constructeur avec documentInfo (sans pHtmlInfo) et pDocNoy
//  Retour:			Rien
//-----------------------------------------------------------------------------------
NSDocumentHisto::NSDocumentHisto(NSDocumentInfo* pNSDocumentInfo, NSNoyauDocument* pDoc)
                :NSDocumentInfo(*pNSDocumentInfo),
                    _PatPathoArray(pNSDocumentInfo->pContexte->getSuperviseur())
{
	_pDocNoy  = pDoc ;
	_sDateDoc = string("") ;

  lObjectCount++ ;
}

//---------------------------------------------------------------------------//  Function:		NSDocumentHisto::~NSDocumentHisto()
//  Description:	Constructeur.
//  Retour:			Aucun
//---------------------------------------------------------------------------
NSDocumentHisto::~NSDocumentHisto()
{
	// surtout ne pas deleter le pDocNoy car ce pointeur est détruit
	// par la dernière vue associée (modèle document / vue)
  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSDocumentHisto::NSDocumentHisto(NSDocumentHisto& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSDocumentHisto::NSDocumentHisto(const NSDocumentHisto& rv)
                :NSDocumentInfo(rv.pContexte),
                      _PatPathoArray(rv._PatPathoArray)
{
	_pDocNoy = rv._pDocNoy ;

	_Donnees = rv._Donnees ;

	// strcpy(dateDoc, rv.dateDoc);
	_sDateDoc = string("") ;    // pour forcer la relecture de la date dans la PatPatho

  lObjectCount++ ;
}

NSDocumentHisto&
NSDocumentHisto::operator=(const NSDocumentHisto& src)
{
	if (this == &src)
		return *this ;

	_pDocNoy = src._pDocNoy ;
	_Donnees = src._Donnees ;

	// strcpy(dateDoc, rv.dateDoc);
	_sDateDoc = string("") ;    // pour forcer la relecture de la date dans la PatPatho

	//
	// Copie les valeurs du NSDocumentHisto d'origine
	//
	_PatPathoArray = src._PatPathoArray ;

	return *this ;
}

//---------------------------------------------------------------------------//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int NSDocumentHisto::operator == ( const NSDocumentHisto& o )
{
	return ((_PatPathoArray == o._PatPathoArray) &&
	        (_Donnees       == o._Donnees)) ;
}

//---------------------------------------------------------------------------//  Description:	Récupération de la date du document
//					date de création ou date patpatho si non vide
//  Retour:			La date au format AAAAMMJJ (char*)
//---------------------------------------------------------------------------
string
NSDocumentHisto::getDateDoc()
{
	// si la date n'est pas initialisée on cherche d'abord dans la patpatho
	if (string("") == _sDateDoc)
		SetDateDoc() ;

	return _sDateDoc ;
}

//---------------------------------------------------------------------------
//  Description:	Récupération de la date du document
//					date de création ou date patpatho si non vide
//  		        stockage au format AAAAMMJJ dans dateDoc
//---------------------------------------------------------------------------
void
NSDocumentHisto::SetDateDoc()
{
  // Is there a date in the patpatho?
  //
	string sDate = _PatPathoArray.GetDocDate() ;

  // Is there a date in a connected patpatho?
  //
  if (string("") == sDate)
  {
    NSPersonGraphManager *pGraphManager = pContexte->getPatient()->getGraphPerson() ;
    string sMetaTreeDocId = pGraphManager->getTreeDocId(_sCodeDocMeta) ;
    if (string("") != sMetaTreeDocId)
    {
      string sDataTreeId = pGraphManager->getDataIdFromMetaId(sMetaTreeDocId) ;
      if (string("") != sDataTreeId)
      {
        NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
        string sDocRosace ;
        if (pGraphManager->getTree(sDataTreeId, &PatPatho, &sDocRosace))
          sDate = PatPatho.GetDocDate() ;
      }
    }
  }

	if (string("") != sDate)		_sDateDoc = sDate ;

	// S'il n'y a pas de date dans la patpatho, on prend la date de création	// du document
	else
		_sDateDoc = _Donnees._sDateCreation ;
}

//---------------------------------------------------------------------------//  Description:	Remet à vide la date du document
//---------------------------------------------------------------------------
void
NSDocumentHisto::ResetDateDoc()
{
	_sDateDoc = string("") ;
}

bool
NSDocumentHisto::checkPptConsistency()
{
	if (true == _PatPathoArray.empty())
		return true ;

  string sLang = pContexte->getUserLanguage() ;

	// Check if there is a line 0 column 0 node
  //
  PatPathoIter rootIt = _PatPathoArray.ChercherItem(0, 0) ;
  if ((NULL == rootIt) || (_PatPathoArray.end() == rootIt))
  {
  	// Error message
    //
  	string sErrorText = getDocTitleWithDate() + string(" -> ") ;
  	sErrorText += pContexte->getSuperviseur()->getText("documentManagementErrors", "thisDocumentHasNoRootNode") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;

#ifndef _ENTERPRISE_DLL
    // Automatic fixing proposal
    //
    if (string("") != getContent())
    {
    	string sCaption = pContexte->getSuperviseur()->getAppName().c_str() ;
    	string sMesgText = pContexte->getSuperviseur()->getText("generalLanguage", "tryAnAutomaticFixing?") ;
    	int retVal = ::MessageBox(pContexte->GetMainWindow()->HWindow, sMesgText.c_str(), sCaption.c_str(), MB_YESNO) ;
    	if (retVal != IDYES)
    	{
    		sErrorText = string("No automatic fixing") ;
    		pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      	return false ;
    	}

    	sErrorText = string("Automatic fixing") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;

      NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
			PatPatho.ajoutePatho(getContent(), 0) ;

      // We don't do this, because we suppose that other nodes already have
      // the proper line and column
			// PatPatho.InserePatPathoFille(PatPatho.begin(), pPatPathoArray) ;

      PatPathoIter it = _PatPathoArray.begin() ;
      for ( ; _PatPathoArray.end() != it ; it++)
      	PatPatho.push_back(new NSPatPathoInfo(**it)) ;

      _PatPathoArray = PatPatho ;

      // Before saving the document, we have to check that traits are in the ppt
      //
      it = PatPatho.begin() ;
      string sRoot = (*it)->getLexiqueSens() ;
      if (string("ZADMI") == sRoot)
      {
      	// Sometimes this information disappears and let "holes" in lines
        // numbers... it is better not to make any correction to lines
        // numbers because it could also come from hidden nodes

      	// Familly Name
        //
        Message Msg ;
        Msg.SetLexique(string("£CL000")) ;
        Msg.SetTexteLibre(pContexte->getPatient()->getNom()) ;
      	PatPatho.AjouterChemin(string("ZADMI1/LIDET1/LNOM01"), &Msg, false /* bEcraser */) ;

        // First Name
        //
        Msg.Reset() ;
        Msg.SetLexique(string("£CL000")) ;
        Msg.SetTexteLibre(pContexte->getPatient()->getPrenom()) ;
      	PatPatho.AjouterChemin(string("ZADMI1/LIDET1/LNOM21"), &Msg, false /* bEcraser */) ;

        // Birthdate
        //
        Msg.Reset() ;
        Msg.SetLexique(string("£D0;10")) ;
        Msg.SetUnit("2DA011") ;
        Msg.SetComplement(pContexte->getPatient()->getNaissance()) ;
      	PatPatho.AjouterChemin(string("ZADMI1/LIDET1/KNAIS1"), &Msg, false /* bEcraser */) ;

        // Gender
        //
        Msg.Reset() ;
        Msg.SetLexique(pContexte->getPatient()->getSexe()) ;
      	PatPatho.AjouterChemin(string("ZADMI1/LIDET1/LSEXE1"), &Msg, false /* bEcraser */) ;
      }

      // Can't do this because it starts an infinite loop (this constructor
      // will look into the history to find the pPT
      // NSNoyauDocument noyauDoc(0, (NSDocumentInfo*)this, 0, pContexte, false) ;

      NSNoyauDocument noyauDoc((TDocument*) 0, pContexte, true) ;
      noyauDoc._pDocInfo = new NSDocumentInfo(*((NSDocumentInfo*)this)) ;
      noyauDoc.setPatPatho(&PatPatho) ;
      noyauDoc.setValidDoc(true) ;
      noyauDoc.setReadOnly(false) ;

      bool bSaved = noyauDoc.enregistrePatPatho() ;

      if (true == bSaved)
        noyauDoc.initFromPatPatho(&_PatPathoArray) ;

      return bSaved ;
    }
#endif // #ifndef _ENTERPRISE_DLL
  }
  return true ;
}

// In a previous version, there was a bug in the fonction that automatically
// inserted historical information from report generators to synthesis.
// Synthesis' root node was deleted and we have now to automatically fix it.
//
bool
NSDocumentHisto::checkSynthesisConsistency()
{
	if (string("ZSYNT1") != getContent())
		return false ;

	if (true == _PatPathoArray.empty())
		return true ;

  string sLang = pContexte->getUserLanguage() ;

	// Check if there is a line 0 column 0 node
  //
  PatPathoIter rootIt = _PatPathoArray.ChercherItem(0, 0) ;
  if ((NULL == rootIt) || (_PatPathoArray.end() == rootIt))
    return false ;

  string sRootLexique = (*rootIt)->getLexiqueSens() ;
  if (string("ZSYNT") == sRootLexique)
    return true ;

  string sErrorText = string("Synthesis has no proper root, starting automatic fixing") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  // erreur(sErrorText.c_str(), standardError) ;

#ifndef _ENTERPRISE_DLL

  NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
  PatPatho.ajoutePatho(getContent(), 0) ;

  PatPatho.InserePatPathoFille(PatPatho.begin(), &_PatPathoArray) ;

  _PatPathoArray = PatPatho ;

  // Can't do this because it starts an infinite loop (this constructor
  // will look into the history to find the pPT
  // NSNoyauDocument noyauDoc(0, (NSDocumentInfo*)this, 0, pContexte, false) ;

  NSNoyauDocument noyauDoc((TDocument*) 0, pContexte, true) ;
  noyauDoc._pDocInfo = new NSDocumentInfo(*((NSDocumentInfo*)this)) ;
  noyauDoc.setPatPatho(&_PatPathoArray) ;
  noyauDoc.setValidDoc(true) ;
  noyauDoc.setReadOnly(false) ;

  bool bSaved = noyauDoc.enregistrePatPatho() ;

  if (true == bSaved)
    noyauDoc.initFromPatPatho(&_PatPathoArray) ;
  else
    return false ;

#endif // #ifndef _ENTERPRISE_DLL

  return true ;
}

void
NSDocumentHisto::initDocumentInfo(NSDocumentInfo *pNewDoc)
{
  if ((NSDocumentInfo*) NULL == pNewDoc)
    return ;

  pNewDoc->setData(&_Donnees) ;

  pNewDoc->setMeta(&_Meta) ;
  pNewDoc->setPres(&_Pres) ;

  pNewDoc->setCodeDocMeta(_sCodeDocMeta) ;
  pNewDoc->setCodeDocPres(_sCodeDocPres) ;
}

void
NSDocumentHisto::setPatPatho(const NSPatPathoArray* pPpt)
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  _PatPathoArray = *pPpt ;
}

void
NSDocumentHisto::initFromPatPatho(NSPatPathoArray* pPpt) const
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  *pPpt = _PatPathoArray ;
}

NSPatPathoInfo*
NSDocumentHisto::getRootPathoInfo() const
{
  if (_PatPathoArray.empty())
    return (NSPatPathoInfo*) 0 ;

  return *(_PatPathoArray.begin()) ;
}

bool
NSDocumentHisto::SearchInDocument(string sChemin, NSSearchStruct *pSearchStruct, string sDateDoc)
{
  if (((NSSearchStruct*) NULL == pSearchStruct) || _PatPathoArray.empty())
    return false ;

  if (string("") == sDateDoc)
    sDateDoc = getDateDoc() ;

  PatPathoIter      pptIt      = _PatPathoArray.begin() ;
  PatPathoConstIter pptItBegin = _PatPathoArray.begin() ;  // There is an infinite loop when what is asked is exactly the root node of the ppt  //  string sRootNodeLabel     = (*pptIt)->getNodeLabel() ;  string sRootNodeLabelSens = string("") ;  pContexte->getDico()->donneCodeSens(&sRootNodeLabel, &sRootNodeLabelSens) ;  if ((sRootNodeLabel == sChemin) || (sRootNodeLabelSens == sChemin))  {    string sRefDate = string("") ;    if (isValidRegularNode(pptItBegin, pSearchStruct, sDateDoc, sRefDate))      pSearchStruct->getFoundNodes()->insert(sRefDate, std::string((*pptIt)->getNode())) ;    return true ;  }  // Find all occurences of sChemin in pPPT  //  while (_PatPathoArray.CheminDansPatpatho(sChemin, string(1, cheminSeparationMARK), &pptIt, &pptItBegin))  {    // If node is acceptable by pSearchStruct, we add the node to results list    //    if (pptIt && (_PatPathoArray.end() != pptIt))    {      string sRefDate = string("") ;      if (isValidRegularNode(pptIt, pSearchStruct, sDateDoc, sRefDate))        pSearchStruct->getFoundNodes()->insert(sRefDate, std::string((*pptIt)->getNode())) ;    }    // Pushing iterators one step further    //    if ((NULL == pptIt) || (_PatPathoArray.end() == pptIt))      break ;    // pptItBegin = pptIt++ ;    pptItBegin = ++pptIt ;    if (_PatPathoArray.end() == pptItBegin)      break ;  }  return true ;
}

bool
NSDocumentHisto::isValidRegularNode(PatPathoConstIter pptIter, NSSearchStruct *pSearchStruct, string sDateDoc, string &sRefDate)
{
  if (_PatPathoArray.empty() || (NULL == pptIter) || (_PatPathoArray.end() == pptIter))
    return false ;

  if ((NSSearchStruct*) NULL == pSearchStruct)
    return true ;

  // Check Episode dates
  //  if (NSSearchStruct::episodeUndefined != pSearchStruct->getEpisodeStatus())  {    string sOpeningDate = _PatPathoArray.getNodeOpenDate(pptIter) ;    string sClosingDate = _PatPathoArray.getNodeCloseDate(pptIter) ;    if (false == pSearchStruct->accept_episode(sOpeningDate, sClosingDate))      return false ;  }  // Check archetype  //  if ((string("") != pSearchStruct->getMandatoryArchetype()))  {    string sArchetypeId = _PatPathoArray.getDirectArchetypeId(pptIter) ;    if (sArchetypeId != pSearchStruct->getMandatoryArchetype())      return false ;  }

  // Check peremption date
  //
  string sDate = _PatPathoArray.getNodeDate(pptIter) ;
  if (string("") == sDate)  {    if (false == pSearchStruct->accept_date(sDateDoc))      return false ;    sRefDate = sDateDoc ;  }  else  {    if (false == pSearchStruct->accept_date(sDate))      return false ;    sRefDate = sDate ;  }
  return true ;
}

bool
NSDocumentHisto::CheminDansPatpatho(string sChaine, string separateur, PatPathoIter* pIter, PatPathoConstIter* startIter, PatPathoConstIter* subTreeIter)
{
  return _PatPathoArray.CheminDansPatpatho(sChaine, separateur, pIter, startIter, subTreeIter) ;
}

//***************************************************************************// 					Implémentation des méthodes NSDocHistoArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSDocHistoArray::NSDocHistoArray(const NSDocHistoArray& rv)
					      :NSDocumentHistoArray()
{
  lObjectCount++ ;

	if (empty())
		return ;

	for (DocumentConstIter i = rv.begin() ; rv.end() != i ; i++)
		push_back(new NSDocumentHisto(*(*i))) ;
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
void
NSDocHistoArray::vider()
{
	if (empty())
		return ;

	for (DocumentIter i = begin() ; end() != i ; )	{
		delete *i ;
		erase(i) ;
	}
}

NSDocHistoArray::~NSDocHistoArray(){
  lObjectCount-- ;

	vider() ;
}

DocumentIter
NSDocHistoArray::TrouveDocHisto(const string codeDocBrut)
{
	if (true == empty())
		return NULL ;

	DocumentIter iterNode = begin() ;
	for ( ; end() != iterNode ; iterNode++)
	{
		string sCodeDoc     = (*iterNode)->getID() ;
		string sCodeDocMeta = (*iterNode)->getCodeDocMeta() ;
		if ((sCodeDoc == codeDocBrut) || (sCodeDocMeta == codeDocBrut))
			return iterNode ;
	}

	return NULL ;
}

//***************************************************************************
// 					Implémentation des méthodes NSDocumentsTank
//***************************************************************************

NSDocumentsTank::NSDocumentsTank(NSContexte* pCtx) : NSRoot(pCtx)
{
  _pLibChem = (NSDocumentHisto*) 0 ;

  lObjectCount++ ;
}

NSDocumentsTank::NSDocumentsTank(const NSDocumentsTank& rv)
                :NSRoot(rv.pContexte)
{
  _VectDocument       = rv._VectDocument ;
	_VectDocumentOuvert = rv._VectDocumentOuvert ;
	_VectChemise        = rv._VectChemise ;

  if (rv._pLibChem)
		_pLibChem = new NSDocumentHisto(*(rv._pLibChem)) ;
	else
		_pLibChem = (NSDocumentHisto*) 0 ;

  lObjectCount++ ;
}

NSDocumentsTank::~NSDocumentsTank(){  lObjectCount-- ;  if (_pLibChem)		delete _pLibChem ;}
//-------------------------------------------------------------------------------
// Get the patpatho of a document inside VectDocument//-------------------------------------------------------------------------------/*NSPatPathoArray*NSDocumentsTank::DonnePatPathoDocument(string sCodeDocum){  DocumentIter iter = _VectDocument.TrouveDocHisto(sCodeDocum) ;  if (NULL != iter)    return (*iter)->getPatPatho() ;  return (NSPatPathoArray*) 0 ;}
*/

void
NSDocumentsTank::setPatPathoDocument(string sCodeDocum, const NSPatPathoArray* pPpt)
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  DocumentIter iter = _VectDocument.TrouveDocHisto(sCodeDocum) ;
  if (NULL != iter)    (*iter)->setPatPatho(pPpt) ;}
void
NSDocumentsTank::initFromPatPathoDocument(string sCodeDocum, NSPatPathoArray* pPpt) const
{
  if ((NSPatPathoArray*) NULL == pPpt)
    return ;

  DocumentIter iter = _VectDocument.TrouveDocHisto(sCodeDocum) ;
  if (iter)    (*iter)->initFromPatPatho(pPpt) ;
}

//----------------------------------------------------------------------
//  Si iter == 0, trouve le premier document de ce type, sinon, trouve le
//  premier élément de ce type situé au delà de iter
//
//  Si sLexique contient un code complet, on fait une comparaison exacte,
//  si sLexique contient un code sens, on fait une comparaison sur le concept
//
//  If iter == 0, it finds the first document of the kind, else it finds the
//  first document of the kind beyond iter
//
//----------------------------------------------------------------------
DocumentIter
NSDocumentsTank::DonnePatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter)
{
	if (_VectDocument.empty())
  	return NULL ;

	bool bContinuer = true ;
  DocumentIter iterDoc ;

  if (NULL == iter)
  	iterDoc = _VectDocument.begin() ;
	else if (_VectDocument.end() != iter)
  {
  	iterDoc = iter ;
    iterDoc++ ;
	}
	else
		return NULL ;

	while ((_VectDocument.end() != iterDoc) && bContinuer)
	{
    NSPatPathoArray PptDoc(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&PptDoc) ;

		if (PptDoc.empty())
		{
			iterDoc++ ;
    	continue ;
		}

		PatPathoIter iterPat = PptDoc.begin() ;
		string sRoot = (*iterPat)->getLexique() ;

		//
		// On compare la racine avec le code cherché puis le code sens de
		// la racine
		//
		bool bFound = false ;
		if (sRoot == sLexique)
			bFound = true ;
		else
		{
			string sRootSens ;
			pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
			if (sRootSens == sLexique)
				bFound = true ;
		}
		if (bFound)
		{
			if (pPatPatho)
				*pPatPatho = PptDoc ;

			bContinuer = false ;
		}
		else
			iterDoc++ ;
	}

	return iterDoc ;
}

//-------------------------------------------------------------------------------
//  Si iter == 0, trouve le dernier document de ce type, sinon, trouve le
//  premier élément de ce type situé avant iter (sachant que VectDocument
//  est trié dans l'ordre antichronologique => les derniers seront les premiers)
//
//  If iter == 0, it finds the last document of the kind, else it finds the
//  first document of the kind before iter
//
//-------------------------------------------------------------------------------DocumentIterNSDocumentsTank::DonnePrevPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter)
{
  if ((string("") == sLexique) || (_VectDocument.empty()))
    return NULL ;

  if (_VectDocument.end() == iter)
    return NULL ;

  bool bContinuer = true ;

  // On repositionne iterDoc sur iter (iterDoc = iter)
  //
  DocumentIter iterDoc = _VectDocument.begin() ;

  if (NULL != iter)
  {
    // Go to iterDoc
    //
    for (; (_VectDocument.end() != iterDoc) && (iterDoc != iter) ; iterDoc++) ;

    if (_VectDocument.end() == iterDoc)
      return NULL ;

    // Skip over
    //
    iterDoc++ ;

    if (_VectDocument.end() == iterDoc)
      return NULL ;
  }

  // Now get the first document of the kind
  //
  while (bContinuer)
  {
    bool bFound = false ;

    NSPatPathoArray PptDoc(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&PptDoc) ;

    if (false == PptDoc.empty())
    {
      PatPathoIter iterPat = PptDoc.begin() ;

      string sRoot = (*iterPat)->getLexique() ;

      //
      // On compare la racine avec le code cherché puis le code sens de
      // la racine
      //
      if (sRoot == sLexique)
        bFound = true ;

            /***************************************************************
            else
            {
                string sRootSens ;
                pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
                if (sRootSens == sLexique)
                    bFound = true ;
            }
            ****************************************************************/
    }

    if (bFound)
    {
      if (pPatPatho)
        *pPatPatho = PptDoc ;

      bContinuer = false ;
    }
    else
    {
      iterDoc++ ;
      if (_VectDocument.end() == iterDoc)
        return NULL ;
    }
  }

  return iterDoc ;
}

//-------------------------------------------------------------------------------
//  Si iter == 0, trouve le premier document de ce type, sinon, trouve le
//  dernier élément de ce type situé après iter (sachant que VectDocument
//  est trié dans l'ordre antichronologique => les derniers seront les premiers)
//
//  If iter == 0, it finds the first document of the kind, else it finds the
//  last document of the kind after iter
//
//-------------------------------------------------------------------------------DocumentIterNSDocumentsTank::DonneNextPatPathoDocument(string sLexique, NSPatPathoArray* pPatPatho, DocumentIter iter)
{
  if ((string("") == sLexique) || (_VectDocument.empty()))
    return NULL ;

  DocumentIter returnedIterDoc = NULL ;
  DocumentIter iterDoc         = _VectDocument.begin() ;

  // Get the last document of the kind (before reaching iterDoc if iterDoc exists)
  //
  for ( ; _VectDocument.end() != iterDoc ; iterDoc++)
  {
    if (iterDoc == iter)
      return returnedIterDoc ;

    bool bFound = false ;

    NSPatPathoArray PptDoc(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&PptDoc) ;

    if (false == PptDoc.empty())
    {
      PatPathoIter iterPat = PptDoc.begin() ;

      string sRoot = (*iterPat)->getLexique() ;

      //
      // On compare la racine avec le code cherché puis le code sens de
      // la racine
      //
      if (sRoot == sLexique)
        bFound = true ;

            /***************************************************************
            else
            {
                string sRootSens ;
                pContexte->getDico()->donneCodeSens(&sRoot, &sRootSens) ;
                if (sRootSens == sLexique)
                    bFound = true ;
            }
            ****************************************************************/
    }

    if (bFound)
    {
      returnedIterDoc = iterDoc ;
      if (pPatPatho)
        *pPatPatho = PptDoc ;
    }
  }

  return returnedIterDoc ;
}

//
// Renvoit l'arbre qui correspond à cet identifiant
//
// Deux cas de figure :
//  1) l'arbre est déjà stocké de façon perenne en mémoire
//      *ppNSPPt pointe dessus, *pbDeleteAtEnd vaut false
//      (l'appelant ne doit jamais détruire l'arbre)
//  2) l'arbre est chargé en mémoire par cette fonction
//      *ppNSPPt est instancié, *pbDeleteAtEnd vaut true
//      (l'appelant doit détruire l'arbre en sortant)
//
bool
NSDocumentsTank::donneArbre(const string sIdentifiant, NSPatPathoArray* pNSPPt, string &sDateExam) const
{
	if (_VectDocument.empty())
		return false ;

	//
	// Recherche du document
	//
	DocumentIter DocIt = _VectDocument.TrouveDocHisto(sIdentifiant) ;
	if (NULL == DocIt)
		return false ;

	//
	// Arbre
	//
	(*DocIt)->initFromPatPatho(pNSPPt) ;
	sDateExam = (*DocIt)->getDateDoc() ;

	return true ;
}

void
NSDocumentsTank::RangeDocumentOuvert(NSDocumentInfo* pDocumentInfo, NSNoyauDocument* pDoc)
{
	_VectDocumentOuvert.push_back(new NSDocumentHisto(pDocumentInfo, pDoc)) ;
}

void
NSDocumentsTank::FermeDocumentOuvert(NSDocumentInfo* pDocumentInfo)
{
try
{
	if (((NSDocumentInfo*) NULL == pDocumentInfo) || (_VectDocumentOuvert.empty()))
		return ;

	string codeDocHisto = pDocumentInfo->getID() ;

	for (DocumentIter iterDoc = _VectDocumentOuvert.begin() ; _VectDocumentOuvert.end() != iterDoc ; iterDoc++)
	{
		if ((*iterDoc)->getID() == codeDocHisto)
		{
    	delete (*iterDoc) ; // of course (*iterDoc)->pDocNoy n'est pas deleté ici
      _VectDocumentOuvert.erase(iterDoc) ;
      break ;
    }
  }
}
catch (...)
{
#ifdef _ENTERPRISE_DLL
	erreur("Exception FermeDocumentOuvert.", standardError) ;
#else
  erreur("Exception FermeDocumentOuvert.", standardError, 0) ;
#endif
}
}

NSNoyauDocument*
NSDocumentsTank::EstUnDocumentOuvert(NSDocumentInfo* pDocumentInfo)
{
try
{
	if (((NSDocumentInfo*) NULL == pDocumentInfo) || (_VectDocumentOuvert.empty()))
		return (NSNoyauDocument*) 0 ;

	string codeDocHisto = pDocumentInfo->getID() ;

	for (DocumentIter iterDoc = _VectDocumentOuvert.begin() ; _VectDocumentOuvert.end() != iterDoc ; iterDoc++)
		if ((*iterDoc)->getID() == codeDocHisto)
			return (*iterDoc)->getDocNoy() ;

	return (NSNoyauDocument*) 0 ;
}
catch (...)
{
#ifdef _ENTERPRISE_DLL
  erreur("Exception EstUnDocumentOuvert.", standardError) ;
#else
	erreur("Exception EstUnDocumentOuvert.", standardError, 0) ;
#endif
	return (NSNoyauDocument*) 0 ;
}
}

NSNoyauDocument*NSDocumentsTank::EstUnDocumentOuvert(NSTreeNode* pNode)
{
	if ((NSTreeNode*) NULL == pNode)
		return (NSNoyauDocument*) 0 ;

	NSNoyauDocument* pDocNoy;

	NSDocumentHisto* pDocHisto = static_cast<NSDocumentHisto*>(pNode->getDocument()) ;
  if ((NSDocumentHisto*) NULL == pDocHisto)
		return (NSNoyauDocument*) 0 ;

	NSDocumentInfo DocInfo(pContexte) ;

  NSDocumentData InfoData ;
  pDocHisto->initFromData(&InfoData) ;
  DocInfo.setData(&InfoData) ;

	pDocNoy = EstUnDocumentOuvert(&DocInfo) ;

	return pDocNoy ;
}

NSDocumentsTank&
NSDocumentsTank::operator=(const NSDocumentsTank& src)
{
  if (this == &src)
		return *this ;

  _VectDocument       = src._VectDocument ;
	_VectDocumentOuvert = src._VectDocumentOuvert ;
	_VectChemise        = src._VectChemise ;

  if (_pLibChem)
		delete _pLibChem ;

	if (src._pLibChem)
		_pLibChem = new NSDocumentHisto(*(src._pLibChem)) ;
	else
		_pLibChem = (NSDocumentHisto*) 0 ;

  return *this ;
}

