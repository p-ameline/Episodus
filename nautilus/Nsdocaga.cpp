//---------------------------------------------------------------------------//  NSDOCAGA.CPP
//  RS Octobre 98
//  Objet document AGA
//---------------------------------------------------------------------------

#define __NSDOCAGA_CPP

#include "nautilus\nssuper.h"
#include "nautilus\nsdocaga.h"
#include "nautilus\ns_html.h"
#include "nautilus\nsrechdl.h"      // pour doc convoc

long NSDocCompta::_lObjectCount = 0 ;

NSConvocRefDocument::NSConvocRefDocument(NSVarConvoc* pVar, int* pTab, int n, NSContexte* pCtx)
                    :NSRefDocument(0, 0, 0, pCtx, true)
{
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZTCVC") ;

	pVarConvoc = new NSVarConvoc(*pVar) ;
	pTabSelect = new int[n] ;
	for (int i = 0; i < n; i++)
  	pTabSelect[i] = pTab[i] ;
	nbConvoc = n ;
}

NSConvocRefDocument::~NSConvocRefDocument()
{
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVarConvoc ;
	delete[] pTabSelect ;
}

bool
NSConvocRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
        				string sAdresseCorresp, string sPathDest)
{
	string   	   sFichierHtml ;
	NSHtmlConvoc hconvoc(typeOperation, this, pContexte, sAdresseCorresp) ;
	string 		   sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hconvoc.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images
	if (!hconvoc.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
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

	return true ;
}

//
// NSSecuRefDocument
//

NSSecuRefDocument::NSSecuRefDocument(NSContexte* pCtx)
                  :NSRefDocument(0, 0, 0, pCtx, true)
{
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZSECU") ;

	pAssuInfo 	 = 0 ;
	pCarteMalade = 0 ;
	pCarte 		   = 0 ;

	bInitOk = InitDonneesMalade() ;
}

NSSecuRefDocument::~NSSecuRefDocument()
{
  // pDocInfo is deleted by ~NSNoyauDocument()
	if (pAssuInfo)
		delete pAssuInfo ;
	if (pCarteMalade)
		delete pCarteMalade;
	if (pCarte)
		delete pCarte;
}

boolNSSecuRefDocument::InitDonneesMalade()
{
  string sNumPat = pContexte->getPatient()->getNss();
  NSVitale2* pCarteV2 = new NSVitale2(pContexte->getSuperviseur());  pCarteMalade = new NSVitale2Info();
  bNonAssure = false;

  // on retrouve d'abord la carte vitale du patient
  pCarteV2->lastError = pCarteV2->open();
  if (pCarteV2->lastError != DBIERR_NONE)
  {
  	erreur("Erreur à l'ouverture de la base ayant-droits.", standardError, pCarteV2->lastError,pContexte->GetMainWindow()->GetHandle()) ;
    delete pCarteV2;
    return false;
	}

  pCarteV2->lastError = pCarteV2->chercheClef(&sNumPat,
											  "NUMPATIENT",
											  NODEFAULTINDEX,
											  keySEARCHEQ,
                                              dbiWRITELOCK);

	if ((pCarteV2->lastError != DBIERR_NONE) && (pCarteV2->lastError != DBIERR_RECNOTFOUND))
	{
  	erreur("Erreur à la recherche de la carte vitale.", standardError, pCarteV2->lastError, pContexte->GetMainWindow()->GetHandle()) ;
    pCarteV2->close() ;
		delete pCarteV2 ;
    return false ;
	}
  else if (pCarteV2->lastError == DBIERR_RECNOTFOUND)
  {
  	erreur("Ce patient n'est pas rattaché à une carte vitale.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    pCarteV2->close() ;
		delete pCarteV2 ;
    return false ;
	}

	pCarteV2->lastError = pCarteV2->getRecord();
  if (pCarteV2->lastError != DBIERR_NONE)
  {
  	erreur("Erreur à la modification de la fiche carte vitale.", standardError, pCarteV2->lastError,pContexte->GetMainWindow()->GetHandle()) ;
    pCarteV2->close() ;
    delete pCarteV2 ;
    return false ;
	}

	// on conserve les données du malade
	*(pCarteMalade->pDonnees) = *(pCarteV2->pDonnees) ;

  // Si le malade n'est pas l'assuré :
  // on recherche l'assuré correspondant au même numéro
  if (strcmp(pCarteMalade->pDonnees->qualite, "00"))
  {
    bool   trouve = false ;
    // l'index primaire comprend num sécu + numéro à deux chiffres
    string sNumSS = string(pCarteMalade->pDonnees->immatricul) ;
    string sClef  = sNumSS + string("  ") ;

    bNonAssure = true ;

    pCarteV2->lastError = pCarteV2->chercheClef(&sClef,
                    "",
                    0,
                    keySEARCHGEQ,
                                          dbiWRITELOCK);

    if (pCarteV2->lastError != DBIERR_NONE)
    {
    	erreur("Erreur à la recherche de la carte vitale.", standardError, pCarteV2->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      pCarteV2->close() ;
      delete pCarteV2 ;
      return false ;
    }

    do
    {
      pCarteV2->lastError = pCarteV2->getRecord() ;
      if (pCarteV2->lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture de la fiche carte vitale.", standardError, pCarteV2->lastError,pContexte->GetMainWindow()->GetHandle()) ;
        pCarteV2->close() ;
        delete pCarteV2 ;
        return false ;
      }

      // Si on n'est plus sur la bonne carte : on sort...
      if (!(string(pCarteV2->pDonnees->immatricul) == sNumSS))
      	break ;
      else if (string(pCarteV2->pDonnees->qualite) == "00")
      {
      	// on a trouvé l'assuré
        trouve = true ;
        break ;
      }

      pCarteV2->lastError = pCarteV2->suivant(dbiWRITELOCK) ;
      if ((pCarteV2->lastError != DBIERR_NONE) && (pCarteV2->lastError != DBIERR_EOF))
      {
      	erreur("Erreur d'accès à la fiche carte vitale suivante.", standardError, pCarteV2->lastError, pContexte->GetMainWindow()->GetHandle()) ;
        pCarteV2->close() ;
        delete pCarteV2 ;
        return false ;
      }
    }
    while (pCarteV2->lastError != DBIERR_EOF) ;

    if (trouve)
    {
    	// on conserve le numéro de patient de l'assuré
      sNumPat = string(pCarteV2->pDonnees->patient) ;
    }
    else
    {
    	erreur("La carte vitale du patient ne correspond à aucun assuré.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      pCarteV2->close() ;
      delete pCarteV2 ;
      return false ;
    }
	}

	pCarteV2->lastError = pCarteV2->close() ;
	if (pCarteV2->lastError != DBIERR_NONE)
  	erreur("Erreur à la fermeture de la base carte vitale.", standardError, pCarteV2->lastError, pContexte->GetMainWindow()->GetHandle()) ;

	delete pCarteV2 ;
// A revoir pour MUE...
#ifndef _MUE
    // on recherche maintenant les données de l'assuré    //
    NSPatient* pPatient = new NSPatient(pContexte);

    pPatient->lastError = pPatient->open();
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base des patients.",0,0,pContexte->GetMainWindow()->GetHandle());
        delete pPatient;
        return false;
    }

    pPatient->lastError = pPatient->chercheClef(&sNumPat,
											  "",
											  0,
											  keySEARCHEQ,
                                              dbiWRITELOCK);

    if ((pPatient->lastError != DBIERR_NONE) && (pPatient->lastError != DBIERR_RECNOTFOUND))
   	{
   		erreur("Erreur à la recherche dans la base des patients.", 0, pPatient->lastError, pContexte->GetMainWindow()->GetHandle());
      	pPatient->close();
		delete pPatient;
        return false;
   	}
    else if (pPatient->lastError == DBIERR_RECNOTFOUND)
    {
        erreur("Impossible de retrouver les données de l'assuré.",0,0,pContexte->GetMainWindow()->GetHandle());
        pPatient->close();
		delete pPatient;
        return false;
    }

    pPatient->lastError = pPatient->getRecord();
   	if (pPatient->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la lecture dans la base des patients.",0,pPatient->lastError,pContexte->GetMainWindow()->GetHandle());
      	pPatient->close();
      	delete pPatient;
      	return false;
   	}

    // on stocke les données du malade
    //
    pAssuInfo = new NSPatInfo(pPatient);
    pAssuInfo->initAdresseInfo();

    pPatient->lastError = pPatient->close();
    if (pPatient->lastError != DBIERR_NONE)
   	    erreur("Erreur à la fermeture de la base des patients.",0,pPatient->lastError,pContexte->GetMainWindow()->GetHandle());

    delete pPatient;
#endif

    // on recherche maintenant les données de la carte    //
    NSVitale1* pV1 = new NSVitale1(pContexte->getSuperviseur()) ;

    pV1->lastError = pV1->open();
    if (pV1->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture de la base des cartes vitales.", standardError, pV1->lastError,pContexte->GetMainWindow()->GetHandle()) ;
        delete pV1;
        return false;
    }

    string sNumSS = string(pCarteMalade->pDonnees->immatricul);

    pV1->lastError = pV1->chercheClef(&sNumSS,
    								  "",
                                      0,
                                      keySEARCHEQ,
                                      dbiWRITELOCK);

    if ((pV1->lastError != DBIERR_NONE) && (pV1->lastError != DBIERR_RECNOTFOUND))
   	{
   		erreur("Erreur à la recherche dans la base des ca.", standardError, pV1->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      	pV1->close();
		delete pV1;
        return false;
   	}
    else if (pV1->lastError == DBIERR_RECNOTFOUND)
    {
        erreur("Impossible de retrouver la carte vitale de l'assuré.", standardError, pV1->lastError, pContexte->GetMainWindow()->GetHandle()) ;
        pV1->close();
		delete pV1;
        return false;
    }

    pV1->lastError = pV1->getRecord();
   	if (pV1->lastError != DBIERR_NONE)
   	{
   		erreur("Erreur à la lecture dans la base des patients.", standardError, pV1->lastError,pContexte->GetMainWindow()->GetHandle());
      	pV1->close();
      	delete pV1;
      	return false;
   	}

    // on stocke les données du malade
    pCarte = new NSVitale1Info(pV1);

    pV1->lastError = pV1->close();
    if (pV1->lastError != DBIERR_NONE)
   	    erreur("Erreur à la fermeture de la base des cartes vitales.", standardError, pV1->lastError,pContexte->GetMainWindow()->GetHandle());

    delete pV1;

    return true;
}

boolNSSecuRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string   	  sFichierHtml;
	NSHtmlSecu    hsecu(typeOperation, this, pContexte);
    string 		  sBaseImg;
   	NSBaseImages* pBase;

   	// on trouve le nom du fichier temporaire à visualiser
   	sNomHtml = hsecu.nomSansDoublons(sPathHtml,sNomHtml,"htm");
   	sFichierHtml = sPathHtml + sNomHtml;

   	// on passe aussi le path destination pour les images
   	if (!hsecu.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
   		return false ;

	// Mise à jour de la base d'images
   	switch (typeOperation)
   	{
   		case toComposer:
   			_sBaseCompo = sBaseImg;
         	break;

   		default:
   			_sBaseImages = sBaseImg;
   	}

	return true;
}

//// NSHistoRefDocument
//
NSHistoRefDocument::NSHistoRefDocument(NSHISTODocument* pDocHis, string sImp)
			             :NSRefDocument(pDocHis->GetParentDoc(), 0, 0, pDocHis->pContexte, true)
{
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZHIST") ;

	sImportance = sImp ;

	if (pDocHis->_pHtmlCS)
		pHtmlCS = new NSHtml(*(pDocHis->_pHtmlCS)) ;
	else
		pHtmlCS = (NSHtml*) 0 ;
}

NSHistoRefDocument::~NSHistoRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	if (pHtmlCS)
    delete pHtmlCS ;
}

//---------------------------------------------------------------------------//  Function: NSHistoRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, string sAdresseCorresp)//
//  Arguments:
//				  sPathHtml : 	répertoire destination
//				  sNomHtml :    En entrée -> nom du fichier à générer
//								En sortie -> nom sans doublons
//				  sAdresseCorresp : nom + adresse du correspondant
//  Description:
//				  Génère un fichier html HISTO dans le répertoire sPathName
//				  Utilise les données sTemplate et sBaseImages
//  Returns:
//            true : OK, false : sinon
//---------------------------------------------------------------------------
bool
NSHistoRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string   	    sFichierHtml ;
	NSHtmlHisto   hhisto(typeOperation, this, pContexte, sAdresseCorresp) ;
	string 		    sBaseImg ;
	NSBaseImages* pBase ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hhisto.nomSansDoublons(sPathHtml,sNomHtml,"htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	// on passe aussi le path destination pour les images
	if (!hhisto.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo, sPathDest))
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

	return true ;
}

NSAgaRefDocument::NSAgaRefDocument(NSAgaDocument *pAgaDoc)                 :NSRefDocument(pAgaDoc->GetParentDoc(), 0, 0, pAgaDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCAGA") ;

	// Initialisation des donnees
	pCriteres = new NSCriteres(*(pAgaDoc->pCriteres)) ;
	pAgaArray = new NSAgaArray(*(pAgaDoc->pAgaArray)) ;
	nbAga 	  = pAgaDoc->nbAga ;
	pVar 	  	= new NSVarCompta(pContexte) ;

	pTotaux	 	= new NSEncaissData(*(pAgaDoc->pTotaux)) ;
	pPartiels = new NSEncaissData(*(pAgaDoc->pPartiels)) ;
	bAgaCumules = pAgaDoc->bAgaCumules ;
}NSAgaRefDocument::~NSAgaRefDocument()
{
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pPartiels ;
  delete pTotaux ;
	delete pVar ;
  delete pAgaArray ;
  delete pCriteres ;
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSAgaRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    sFichierHtml ;
	NSHtmlAga haga(typeOperation, this, pContexte, sAdresseCorresp) ;
  string    sBaseImg ;

  // on trouve le nom du fichier temporaire à visualiser
  sNomHtml = haga.nomSansDoublons(sPathHtml,sNomHtml,"htm") ;
  sFichierHtml = sPathHtml + sNomHtml ;

	haga.initTotaux() ;
	haga.ajouteLignes() ;

	// on passe une base d'images à cause de la référence
	// pHtml == 0 et sPathDest == ""
	if (!haga.genereHtml(sFichierHtml,sBaseImg))
		return false ;

	return true ;
}
NSReqRefDocument::NSReqRefDocument(NSResultReqDialog *pDlg)                 :NSRefDocument(0, pDlg->pContexte)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZRMUL") ;

	// Initialisation des donnees
	bReqModeDoc     = pDlg->isReqModeDoc() ;
	nbPatTotal      = pDlg->getNbPatTotal() ;
  nbPatCritPat    = pDlg->getNbPatCritPat() ;
  nbPatCritDoc    = pDlg->getNbPatCritDoc() ;
  nbPatResult     = pDlg->getNbPatResult() ;
  nbDocCritPat    = pDlg->getNbDocCritPat() ;
  nbDocCritDoc    = pDlg->getNbDocCritDoc() ;
  nbDocResult     = pDlg->getNbDocResult() ;
  VectDocResultat = *(pDlg->getVectDocResultat()) ;
  VectPatResultat = *(pDlg->getVectPatResultat()) ;
}NSReqRefDocument::~NSReqRefDocument()
{
  // pDocInfo is deleted by ~NSNoyauDocument()
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSReqRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{

	string    sFichierHtml ;	NSHtmlReq hreq(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = hreq.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;

	sFichierHtml = sPathHtml + sNomHtml ;
	hreq.initTotaux() ;	hreq.ajouteLignes() ;

	// on passe une base d'images à cause de la référence	// pHtml == 0 et sPathDest == ""

	if (!hreq.genereHtml(sFichierHtml, sBaseImg))  	return false ;

	return true ;}

NSDepRefDocument::NSDepRefDocument(NSDepensDocument *pDepDoc)
                 :NSRefDocument(pDepDoc->GetParentDoc(), 0, 0, pDepDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCDEP") ;

	// Initialisation des donnees	pCriteres    = new NSCriteres(*(pDepDoc->pCriteres)) ;
	pDepensArray = new NSAffDepensArray(*(pDepDoc->pDepensArray)) ;
  nbDepens 	   = pDepDoc->nbDepens ;
	pVar 	       = new NSVarCompta(pContexte) ;
  pTotaux	     = new NSEncaissData(*(pDepDoc->pTotaux)) ;
}

NSDepRefDocument::~NSDepRefDocument(){
	delete pTotaux ;
	delete pVar ;
  delete pDepensArray ;
  delete pCriteres ;
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////

boolNSDepRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    sFichierHtml ;
	NSHtmlDep hdep(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = hdep.nomSansDoublons(sPathHtml,sNomHtml,"htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	hdep.initTotaux() ;	hdep.ajouteLignes() ;

	// on passe une base d'images à cause de la référence	// pHtml == 0 et sPathDest == ""
	if (!hdep.genereHtml(sFichierHtml,sBaseImg))
		return false ;

	return true ;}

//// Définition de NSImpRefDocument
//
NSImpRefDocument::NSImpRefDocument(NSImpDocument *pImpDoc)
                 :NSRefDocument(pImpDoc->GetParentDoc(), 0, 0, pImpDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCIMP") ;

	// Initialisation des donnees
  if (pImpDoc)
  {
	  pCriteres	= new NSMultiCriteres(*(pImpDoc->pCriteres)) ;
	  pImpArray = new NSImpArray(*(pImpDoc->pImpArray)) ;
	  nbImp 	  = pImpDoc->nbImp ;
  }
	pVar 	  	= new NSVarCompta(pContexte) ;
	bEuro	  	= pImpDoc->bEuro ;
}

NSImpRefDocument::~NSImpRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVar ;
	delete pImpArray ;
	delete pCriteres ;
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSImpRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    sFichierHtml ;
	NSHtmlImp himp(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = himp.nomSansDoublons(sPathHtml,sNomHtml,"htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	himp.ajouteLignes() ;

	// on passe une base d'images à cause de la référence
	if (!himp.genereHtml(sFichierHtml,sBaseImg))
  	return false ;

	return true ;
}

//// Définition de NSListActRefDocument
//
NSListActRefDocument::NSListActRefDocument(NSActDocument *pActDoc)
                     :NSRefDocument(pActDoc->GetParentDoc(), 0, 0, pActDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCLAC") ;

	// Initialisation des donnees
	pCriteres = new NSMultiCriteres(*(pActDoc->pCriteres)) ;
	pActArray = new NSListActArray(*(pActDoc->pActArray)) ;
	nbAct 	  = pActDoc->nbAct ;
	pVar 	  	= new NSVarCompta(pContexte) ;
}

NSListActRefDocument::~NSListActRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVar ;
  delete pActArray ;
  delete pCriteres ;
}

// Génération du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSListActRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    sFichierHtml ;
	NSHtmlLac hlac(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hlac.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	hlac.ajouteLignes() ;

	// on passe une base d'images à cause de la référence
	if (!hlac.genereHtml(sFichierHtml, sBaseImg))
		return false ;

	return true ;
}

//// Définition de NSSomActRefDocument
//
NSSomActRefDocument::NSSomActRefDocument(NSActDocument *pActDoc)
                    :NSRefDocument(pActDoc->GetParentDoc(), 0, 0, pActDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCSAC") ;

	// Initialisation des donnees
	pCriteres  	= new NSMultiCriteres(*(pActDoc->pCriteres)) ;
	pExamArray 	= new NSExamArray(*(pActDoc->pExamArray)) ;
	nbExam 	   	= pActDoc->nbExam ;
	totaux	   	= pActDoc->totaux ;
	pVar 	   		= new NSVarCompta(pContexte) ;
}

NSSomActRefDocument::~NSSomActRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVar ;
	delete pExamArray ;
	delete pCriteres ;
}

// Génération du html correspondant au document
////////////////////////////////////////////////////////////////
bool
NSSomActRefDocument::GenereHtml(string sPathHtml, string& sNomHtml,
                                OperationType typeOperation,
                                string sAdresseCorresp, string sPathDest)
{
  string ps = string("NSSomActRefDocument::GenereHtml Entering") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	string    sFichierHtml ;
	NSHtmlSac hsac(typeOperation, this, pContexte, sAdresseCorresp) ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hsac.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;
  ps = string("NSSomActRefDocument::GenereHtml file will be ") + sFichierHtml ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  ps = string("NSSomActRefDocument::GenereHtml starting initTotaux()") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
	hsac.initTotaux() ;

  ps = string("NSSomActRefDocument::GenereHtml starting ajouteLignes()") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
	hsac.ajouteLignes() ;

  ps = string("NSSomActRefDocument::GenereHtml starting hsac.genereHtml") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
	// on passe une base d'images à cause de la référence
  string sBaseImg = string("") ;
	if (false == hsac.genereHtml(sFichierHtml, sBaseImg))
		return false ;

  ps = string("NSSomActRefDocument::GenereHtml Leaving") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	return true ;
}

//// Définition de NSSomEncaissRefDocument
//
NSSomEncaissRefDocument::NSSomEncaissRefDocument(NSEncaissDocument *pEncaissDoc)
                        :NSRefDocument(pEncaissDoc->GetParentDoc(), 0, 0, pEncaissDoc->pContexte, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZCSEN") ;

	// Initialisation des donnees
	pCriteres = new NSMultiCriteres(*(pEncaissDoc->pCriteres)) ;

	pTotaux	  = new NSEncaissData(*(pEncaissDoc->pTotaux)) ;
	pPartiels = new NSEncaissData(*(pEncaissDoc->pPartiels)) ;

	pVar 	  	= new NSVarCompta(pContexte) ;
}

NSSomEncaissRefDocument::~NSSomEncaissRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVar ;
  delete pTotaux ;
  delete pPartiels ;
  delete pCriteres ;
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSSomEncaissRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    sFichierHtml ;
	NSHtmlSen hsen(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hsen.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	hsen.initTotaux() ;

	// on passe une base d'images à cause de la référence
	if (!hsen.genereHtml(sFichierHtml, sBaseImg))
		return false ;

	return true ;
}

//***************************************************************************// Implémentation des méthodes NSVoidArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSVoidArray::NSVoidArray(const NSVoidArray& rv)
            :NSVoidVector()
{
	if (false == rv.empty())
		for (VoidConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(*i) ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSVoidArray::vider()
{
	if (!empty())
		for (VoidIter i = begin(); i != end();)
  		// ATTENTION : on ne fait pas de delete *i;
    	// car les variables n'appartiennent pas au tableau
    	erase(i) ;
}

NSVoidArray::~NSVoidArray()
{
	vider() ;
}

//// Définition de NSPubliRefDocument
//
NSPubliRefDocument::NSPubliRefDocument(NSVoidArray* pVarArray, string sType, NSContexte* pCtx)
                   :NSRefDocument(0, 0, 0, pCtx, true)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	// On copie le type de document
	_pDocInfo->setType(sType) ;

	// on copie les pointeurs void de base
	pVoidArray = new NSVoidArray(*pVarArray) ;
}

NSPubliRefDocument::~NSPubliRefDocument(){
  // pDocInfo is deleted by ~NSNoyauDocument()
	delete pVoidArray ;
}

boolNSPubliRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	string    	sFichierHtml ;
	NSHtmlPubli hpubli(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    	sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hpubli.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	sFichierHtml = sPathHtml + sNomHtml ;

	hpubli.ajouteLignes() ;

	// on passe une base d'images à cause de la référence
	if (!hpubli.genereHtml(sFichierHtml, sBaseImg))
		return false ;

	return true ;
}

/****************************************************************************/
// 				                Impression d'une "facturette"
/****************************************************************************/

NSFactDocument::NSFactDocument(NSFactData *pFactData, NSContexte* pContexte)
               :NSRefDocument(0, pContexte)
{
	// on met en place une pDocInfo pour la publication
	_pDocInfo = new NSDocumentInfo(pContexte) ;
	_pDocInfo->setType("ZRECU") ;

	// Initialisation des donnees
	_pFactData = pFactData ;
}NSFactDocument::~NSFactDocument()
{
  // pDocInfo is deleted by ~NSNoyauDocument()
}

// Génération du html correspondant au document////////////////////////////////////////////////////////////////
bool
NSFactDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{

	string    sFichierHtml ;	NSHtmlReq hreq(typeOperation, this, pContexte, sAdresseCorresp) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser	sNomHtml = hreq.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;

	sFichierHtml = sPathHtml + sNomHtml ;
	hreq.initTotaux() ;	hreq.ajouteLignes() ;

	// on passe une base d'images à cause de la référence	// pHtml == 0 et sPathDest == ""

	if (!hreq.genereHtml(sFichierHtml, sBaseImg))  	return false ;

	return true ;}

/****************************************************************************/// 				Classe NSDocCompta pour l'utilisateur choisi
/****************************************************************************/

NSDocCompta::NSDocCompta()
{
	pAgaRefDoc = 0 ;
	pDepRefDoc = 0 ;  pImpRefDoc = 0 ;
  pListActRefDoc = 0 ;
  pSomActRefDoc = 0 ;
  pSomEncaissRefDoc = 0 ;

  _lObjectCount++ ;
}

NSDocCompta::~NSDocCompta(){
	if (pAgaRefDoc)
		delete pAgaRefDoc ;

	if (pDepRefDoc)		delete pDepRefDoc ;	if (pImpRefDoc)
   	delete pImpRefDoc ;

	if (pListActRefDoc)   	delete pListActRefDoc ;

	if (pSomActRefDoc)   	delete pSomActRefDoc ;

	if (pSomEncaissRefDoc)   	delete pSomEncaissRefDoc ;

  _lObjectCount-- ;
}

// fin du fichier nsdocaga.cpp////////////////////////////////////////////////

