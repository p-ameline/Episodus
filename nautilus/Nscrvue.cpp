//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------
#include <owl\owlpch.h>
#include <owl\dc.h>

#define __NSCRVUE_CPP

//#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "dcodeur\nsdkd.h"

#include "nautilus\nautilus.rh"
#include "nautilus\nscrvue.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsadmiwd.h"
#include "nsepisod\nsldvuti.h"
#include "nsbb\nstlibre.h"
#include "nsbb\nsfltbtnga.h"

// --------------------------------------------------------------------------
// -------------------- METHODES DE NSCRReadOnlyView ------------------------
// --------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSCRReadOnlyView, NSRefVue)
    EV_WM_VSCROLL,
    EV_WM_HSCROLL,
    EV_WM_LBUTTONDOWN,
    EV_WM_RBUTTONDOWN,
    EV_WM_CLOSE,
    EV_WM_DESTROY,
    EV_WM_SETFOCUS,
    EV_WM_QUERYENDSESSION,
    EV_COMMAND(CM_ENREGISTRE,     CmEnregistre),
    EV_COMMAND(IDC_CORRIGER,      CorrigerCR),
    EV_COMMAND(IDC_REPRENDRE_CR,  ReprendreCR),
    EV_COMMAND(IDC_SUPPRIMER,     SupprimerLigne),
    EV_COMMAND(IDC_CONCLUSION_CR, Conclusion),
    EV_COMMAND(IDC_COMPTA_CR,     Compta),
    EV_COMMAND(IDC_CODAGE_CR,     Codage),
    EV_COMMAND(IDC_TLIB_ORDO,     TLibreOrdonnance),
    EV_COMMAND(IDC_MEDIC_ALD,     MedicEnALD),
    EV_COMMAND(IDC_MEDIC_NONALD,  MedicHorsALD),
    EV_COMMAND(IDC_SELECTALL_CR,  SelectAll),
    EV_COMMAND(IDC_COPYTEXT_CR,   CopyToClipboard),
    EV_COMMAND(IDC_COPY_ARBRE,    CopyArbre),
    EV_COMMAND(CM_FILECLOSE,      EvClose),
END_RESPONSE_TABLE;
//---------------------------------------------------------------------------//  Constructeur
//
//  doc 	: NSCRDocument à afficher
//---------------------------------------------------------------------------
NSCRReadOnlyView::NSCRReadOnlyView(NSCRDocument& doc, TWindow* parent)
                 :NSRefVue(doc, doc.pContexte, parent)
{
try
{
	_HautGcheFenetre.x = _HautGcheFenetre.y = 0 ;
	//
	// Initialise le style avec des ascenseurs horizontaux et verticaux
	//
	Attr.Style = Attr.Style | WS_HSCROLL | WS_VSCROLL;

	// vieux        SetViewMenu(new TMenuDescr(IDM_CRVIEW));
	// moins vieux  SetViewMenu(new TMenuDescr(IDM_MDICMNDS));
	_bSetupToolBar = true ;
	_bEnConclusion = false ;
	_bEnCodage     = false ;

	//Scroller = new TScroller();
	_ToolBar 	 = (OWL::TControlBar*) 0 ;
	_pBigBoss  = (NSSmallBrother*) 0 ;
	_ImpDriver = string("") ;
	_ImpDevice = string("") ;
	_ImpOutput = string("") ;
	_LargeurPolice = 0 ;
	_sLocalisationSelectionne = string("") ;

  TMyApp* pMyApp = pContexte->getSuperviseur()->getApplication() ;
  pMyApp->setMenu(string("menubar_cr"), &_hAccelerator) ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCRReadOnlyView::~NSCRReadOnlyView()
{
  string ps = string("Deleting CrView.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

//
// Destructor for a TWindow: disposes of each window in its ChildList
// and removes itself from a non-0 parent's list of children
//
// Destroys a still-associated MS-Windows interface element and frees
// the instance thunk used for association of an MS-Windows element
// to the TWindow
//
// Disposes of its Scroller if the TScroller object was constructed
//
static void NSshutDown(TWindow* win, void*)
{
	win = 0 ;
}

//---------------------------------------------------------------------------
//  SetupWindow() : Initialise la fenêtre
//---------------------------------------------------------------------------
void
NSCRReadOnlyView::SetupWindow()
{
	// Préparation des éléments de mise en page du document
	if (false == InitAspect('N'))
  {
		string sErrMsg = pContexte->getSuperviseur()->getText("CREdit", "aspectInitFailed") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
  	return ;
  }

	// Elaboration d'un nouveau Compte Rendu ?
  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

	string sLang = string("") ;
  if (pContexte)
  	sLang = pContexte->getUserLanguage() ;

	if (false == pCRDoc->IsOpen())
  {
  	if (false == nouveauCR())
    	return ;
    else
    {
    	string sLibelleDoc = string("") ;

      NSPatPathoInfo* pRootPatInfo = pCRDoc->getRootPathoInfo() ;
      if (pRootPatInfo)
      {
      	string sCode = pRootPatInfo->getLexique() ;
        if (string("") != sCode)
        	pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
      }
      sLibelleDoc += string(" (en cours)") ;

			pCRDoc->SetTitle(sLibelleDoc.c_str()) ;
		}
	}

	// Initialisation de la vue
	_PremLigne = 1 ;
  int decodage = Initialisation() ;

	while ((iAnnule == decodage) || (iCorrige == decodage) || (iAutomatique == decodage))
  {
  	if (iAnnule == decodage)  //iAnnule, iCorrige
    {
    	// CloseWindow(); ne marche pas car la fenetre doit se référencer
		  // proprement au sein du document/vue avant de se fermer
      // Prévoir d'envoyer un message de fermeture WM_CLOSE
      return ;
    }
    if (iCorrige == decodage)
    	ReprendreCR() ;

    decodage = Initialisation() ;
  }

	// Initialisation des barres de défilement
	SetScrollRange(SB_VERT, 1, _Lignes.size()) ;
  if (_LargeurPolice)
		SetScrollRange(SB_HORZ, 1, _RectangleGlobal.Width() / _LargeurPolice) ;
}

//---------------------------------------------------------------------------//  Génère un nouveau compte rendu.//---------------------------------------------------------------------------
bool
NSCRReadOnlyView::nouveauCR()
{
try
{
	//
	_pDoc->SetDirty(true) ;

	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;

	bool bTourner = true ;
	while (bTourner)
	{
		//
		// Lancement de la boite de dialogue de choix de module
		//
		if (false == pContexte->getDico()->Prend())
			return false ;
		//
		// Récupération des paramètres choisis par l'utilisateur
		//
		//
		// Initialisation du BigBrother
		//
		if ((NSSmallBrother*) NULL == _pBigBoss)
			_pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;

		_pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/;
		_pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
		_pBigBoss->_sLexiqueModule = pCRDoc->_lexiqueModule ;

		_pBigBoss->reprise() ;
		if (_pBigBoss->ouvreResModule())
		{
    	_pBigBoss->lanceModule() ;
			bTourner = false ;
		}
	}

	if (pContexte->getSuperviseur()->isCodageAuto())
	{
    NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
    pCRDoc->initFromPatPatho(&DocPatho) ;

    if (false == DocPatho.empty())
    {
      bool bFoundCoding = false ;
      PatPathoIter iter = DocPatho.ChercherPremierFils(DocPatho.begin()) ;
      while ((NULL != iter) && (DocPatho.end() != iter))
      {
        if ((*iter)->getLexiqueSens() == string("0CODE"))
        {
          bFoundCoding = true ;
          break ;
        }
        iter = DocPatho.ChercherFrereSuivant(iter) ;
      }

      if (false == bFoundCoding)
        Codage() ;
    }
	}

	return true ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::nouveauCR", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------//  Initialise les paragraphes et les lignes
//---------------------------------------------------------------------------
int
NSCRReadOnlyView::Initialisation()
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return iErreur ;

  ::SetCursor(::LoadCursor(0, IDC_WAIT)) ; //sablier

  //
	// Demande au document de se décoder dans un fichier
	//
	int iReussi = pCRDoc->decodeCR(&(pCRDoc->_aCRPhrases)) ;

	::SetCursor(::LoadCursor(0, IDC_ARROW)) ;

	if (iOk != iReussi)
	{
		if (iErreur == iReussi)
			erreur("Impossible de décoder le compte-rendu.", warningError, 0, GetHandle()) ;

		return iReussi ;
	}
	//
	// Initialise les paragraphes et les lignes
	//
	BOOL Reussi = InitialiseLignes() ;

	if (_PremLigne > _Lignes.size())
		_PremLigne = _Lignes.size() ;

	return iOk ;
}

//---------------------------------------------------------------------------//  Correction d'un compte rendu.
//---------------------------------------------------------------------------
bool
NSCRReadOnlyView::corrigeCR()
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;

try
{
	//
  // Initialisation du BigBrother
  //
  if ((NSSmallBrother*) NULL == _pBigBoss)
  {
  	_pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;

    if (_pBigBoss)
    {
      NSPatPathoInfo* pRootPathoInfo = pCRDoc->getRootPathoInfo() ;
      if (pRootPathoInfo)
      {
        string sLexiqueModule = pRootPathoInfo->getLexique() ;
        strcpy(pCRDoc->_lexiqueModule, sLexiqueModule.c_str()) ;
      }

      _pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
      _pBigBoss->_sLexiqueModule = pCRDoc->_lexiqueModule ;
    }
  }
  if (false == _pBigBoss->pContexte->getFilGuide()->Prend())
  	return false;
  if (false == _pBigBoss->pContexte->getDico()->Prend())
  	return false;

  _pBigBoss->_pBBFiche = static_cast<BBFiche*>(_pBigBoss->pContexte->getFilGuide()->getBBFiche()) ;

  _pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/;

  if (_pBigBoss->ouvreResModule())
  {
  	if (_pBigBoss->corrigeModule(&_sLocalisationSelectionne))
    {
    	Invalidate() ;
      _pDoc->SetDirty(true) ;
      return true ;
    }
    else
    	return false ;
  }
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::corrigeCR.", standardError, 0, GetHandle()) ;
  return false ;
}
	return false ;
}

//-----------------------------------------------------------------------//-----------------------------------------------------------------------

voidNSCRReadOnlyView::CorrigerCR()
{
  string ps = string("User edits the report") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

  NSPatPathoArray Path(pContexte->getSuperviseur()) ;  pCRDoc->initFromPatPatho(&Path) ;
	if (corrigeCR())  {
  	int iInit = Initialisation() ;

    while (iOk != iInit)    {
    	if (iAnnule == iInit)
      	pCRDoc->setPatPatho(&Path) ;

      if (iCorrige == iInit)      	corrigeCR() ;

      iInit = Initialisation() ;    }

    UpdateWindow() ;    Invalidate() ;
  }
}

voidNSCRReadOnlyView::CopyArbre()
{
try
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

  NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&DocPatho) ;

	if (DocPatho.empty())
		return ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;	pSuper->pBufCopie->vider() ;

	PatPathoIter iter = DocPatho.begin() ;	for ( ; (DocPatho.end() != iter) &&
            ((*iter)->getLocalisation() != _sLocalisationSelectionne) ;
				                                                            iter++) ;
	if (DocPatho.end() == iter)		return ;

	NSCutPaste CP(pContexte->getSuperviseur()) ;
	NSPatPathoArray SousPatho(pContexte->getSuperviseur()) ;	DocPatho.ExtrairePatPathoWithRoot(iter, &SousPatho) ;
  CP.setPatPatho(&SousPatho) ;

	*(pSuper->pBufCopie) = CP ;}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::CopyArbre", standardError, 0) ;
}
}

/*** Fonction de réponse au menu "Supprimer une ligne"* Response function for the "Line delete" menu entry*/
void
NSCRReadOnlyView::SupprimerLigne()
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

  NSPatPathoArray Path(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&Path) ;

	if (false == SupprimerLigneCR())		return ;
	int iInit = Initialisation() ;
	while (iOk != iInit)
	{
		if (iAnnule == iInit)
			pCRDoc->setPatPatho(&Path) ;
		if (iCorrige == iInit)
			ReprendreCR() ;

		iInit = Initialisation() ;	}

	UpdateWindow() ;
	Invalidate() ;
}

/**
* Suppression d'un segment d'arbre au sein d'un compte rendu* Delete a sub-tree inside an exam report*/
boolNSCRReadOnlyView::SupprimerLigneCR()
{
try
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return false ;

	//
	// Initialisation du BigBrother
	//
	if ((NSSmallBrother*) NULL == _pBigBoss)
	{
		_pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;

    NSPatPathoInfo* pPatPathoInfo = pCRDoc->getRootPathoInfo() ;
    if (pPatPathoInfo)
    {
		  string sLexiqueModule = pPatPathoInfo->getLexique() ;
		  strcpy(pCRDoc->_lexiqueModule, sLexiqueModule.c_str()) ;
    }

		_pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
		_pBigBoss->_sLexiqueModule = pCRDoc->_lexiqueModule ;
  }

	_pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/;

	if (false == _pBigBoss->pContexte->getFilGuide()->Prend())
		return false ;
	if (false == _pBigBoss->pContexte->getDico()->Prend())
		return false ;

	_pBigBoss->_pBBFiche = static_cast<BBFiche*>(_pBigBoss->pContexte->getFilGuide()->getBBFiche()) ;

	if (false == _pBigBoss->ouvreResModule())
  	return false ;

	if (_pBigBoss->SupprimerLigneCR(&_sLocalisationSelectionne))
	{
		Invalidate() ;
		_pDoc->SetDirty(true) ;
		return true ;
	}

  return false ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::SupprimerLigneCR", standardError, 0) ;
	return false ;
}
}

//-----------------------------------------------------------------------// Fonction : NSCRReadOnlyView::ReprendreCR()
//-----------------------------------------------------------------------
void
NSCRReadOnlyView::ReprendreCR()
{
  string ps = string("User restarts the report") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

  NSPatPathoArray Path(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&Path) ;

	if (ReprendreCompteRendu())	{
		int iInit = Initialisation() ;

		while (iOk != iInit)		{
			if (iAnnule == iInit)
				pCRDoc->setPatPatho(&Path) ;
			if (iCorrige == iInit)
				ReprendreCR() ;

			iInit = Initialisation() ;		}
		UpdateWindow() ;
		Invalidate() ;
	}
}

boolNSCRReadOnlyView::ReprendreCompteRendu()
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return false ;

try
{
	//
  // Initialisation du BigBrother
  //
  if ((NSSmallBrother*) NULL == _pBigBoss)
  {
  	_pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;
    _pBigBoss->_pBBFiche = static_cast<BBFiche*>(_pBigBoss->pContexte->getFilGuide()->getBBFiche()) ;

    NSPatPathoInfo* pRootPathoInfo = pCRDoc->getRootPathoInfo() ;
    if (pRootPathoInfo)
    {
      string sLexiqueModule = pRootPathoInfo->getLexique() ;
      strcpy(pCRDoc->_lexiqueModule, sLexiqueModule.c_str()) ;
    }

    _pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
    _pBigBoss->_sLexiqueModule = pCRDoc->_lexiqueModule ;
    // strcpy(pBigBoss->noeudModule, (*iterPremier)->pDonnees->noeud);
  }

  if (false == _pBigBoss->pContexte->getFilGuide()->Prend())
  	return false ;
	if (false == _pBigBoss->pContexte->getDico()->Prend())
  	return false ;
	_pBigBoss->_pBBFiche = static_cast<BBFiche*>(_pBigBoss->pContexte->getFilGuide()->getBBFiche()) ;

	_pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/ ;

	if (_pBigBoss->ouvreResModule())
  {
		if (_pBigBoss->lanceModule())
		{
			Invalidate() ;
			_pDoc->SetDirty(true) ;
			return true ;
		}
		else
			return false ;
	}
}
catch (...)
{	erreur("Exception NSCRReadOnlyView::ReprendreCompteRendu", standardError, 0, GetHandle()) ;
	return false ;
}
	return true ;
}

//------------------------------------------------------------------//  Appel automatique d'une fiche Compt à partir du compte-rendu
//------------------------------------------------------------------
void
NSCRReadOnlyView::Compta()
{
try
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string ps = string("User starts accounting for the report") ;
  pSuper->trace(&ps, 1, NSSuper::trSteps) ;

  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
	if (((NSCRDocument*) NULL == pCRDoc) || (NULL == pCRDoc->getRootPathoInfo()))
	{
		erreur("Il n'existe aucune donnée pour ce document", warningError, 0, GetHandle()) ;
		return ;
	}

  if ((NSDocumentInfo*) NULL == pCRDoc->_pDocInfo)
	{
  	string sWarningText = pSuper->getText("documentManagement", "youMustSaveBeforeAccounting") ;
		MessageBox(sWarningText.c_str(), 0, MB_OK) ;
		return ;
	}

  NSPersonInfo    personInfo(pSuper, pContexte, pContexte->getPatient()->getNss()) ;
  NSComptaPatient compta(pContexte, &personInfo) ;

  bool bExistCompt = false ;

  // Check if this document already has accounting infos
  //
  string sNumCompt = string("") ;

  if (_pDoc->_pDocInfo)
  {
    string sPatID = _pDoc->_pDocInfo->getPatient() ;
    string sDocID = _pDoc->_pDocInfo->getDocument() ;

    ps = string("Checking if this report is connected to accounting information") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    compta.GetComptForDoc(sNumCompt, sDocID, sPatID) ;

    if (string("") != sNumCompt)
    {
      bExistCompt = true ;
      ps = string("This report is connected to accounting record ") + sNumCompt ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    }
    else
    {
      ps = string("This report is not connected to any accounting record") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    }
  }

  bool bMustCreate = true ;

  // Edit existing accounting info
  //
  if (bExistCompt)
  {
    NSComptInfo comptInfo ;
    if (compta.getComptInfo(sNumCompt, &comptInfo))
    {
      NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
      pCRDoc->initFromPatPatho(&DocPpt) ;

      NSFse16Array aFseCCAM ;
      compta.InitCcamFromDocument(&comptInfo, &aFseCCAM, &DocPpt) ;

      bMustCreate = false ;

      ps = string("Editing accounting information for this report") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;

      if (compta.EditCompt(sNumCompt, this, NULL, NULL, &aFseCCAM))
        _pDoc->SetAccountingDirty(true) ;
    }
  }

  // Create new accounting info
  //
  if (bMustCreate)
  {
	  NSComptInfo   comptInfo ;
	  NSFse1610Info	fse1610Info ;

	  if (NULL == pCRDoc->getBigBoss())
		  pCRDoc->setBigBoss(new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer())) ;

	  // date de l'examen
    //
    NVLdVTemps tpsDateDoc ;

    NSPatPathoArray PptDocBrut(pContexte->getSuperviseur()) ;
    _pDoc->initFromPatPatho(&PptDocBrut) ;

    string sDateDoc = PptDocBrut.GetDocDate() ;

	  if (string("") == sDateDoc)
	  {
		  if (pCRDoc->_pDocInfo) // on prend la date de création (heure = 0)
        tpsDateDoc.initFromDate(pCRDoc->_pDocInfo->getCreDate()) ;
		  else // on prend la date et l'heure du jour
        tpsDateDoc.takeTime() ;
	  }
	  else // on prend la date du document (heure = 0)
      tpsDateDoc.initFromDate(sDateDoc) ;

    NSFse16Array aFseCCAM ;

    ps = string("Init accounting information from report context") ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    NSPatPathoArray Path(pContexte->getSuperviseur()) ;
    pCRDoc->initFromPatPatho(&Path) ;

    compta.InitComptInfoFromDocument(&comptInfo, &aFseCCAM, &tpsDateDoc, &Path) ;

    // if (aFseCCAM.empty())
    if (pSuper->isNoCcamForReport())
    {
      NSFse1610Info	fse1610Info ;

      // on cale la date de la Fse1610 sur celle de la fiche Compt
		  strcpy(fse1610Info._Donnees.date, comptInfo._Donnees.date) ;
		  strcat(fse1610Info._Donnees.date, comptInfo._Donnees.heure) ;

      ps = string("Creating accounting information, with no CCAM data") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;

		  compta.CmFicheCompt(&comptInfo, &fse1610Info) ;
    }
    else
    {
      ps = string("Creating accounting information, with CCAM data") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  	  compta.CmFicheCompt(&comptInfo, NULL, &aFseCCAM) ;
    }

    if ('\0' != comptInfo._Donnees.numcompt[0])
    {
      _pDoc->SetAccountingDirty(true) ;

      string sDocID = _pDoc->_pDocInfo->getDocument() ;
      string sCptID = string(comptInfo._Donnees.numcompt) ;

      ps = string("Connecting accounting information (") + sCptID + string(") to the report (") + sDocID + string(")") ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;

      compta.SetComptForDoc(sCptID, sDocID) ;
    }
  }

  ps = string("Leaving NSCRReadOnlyView::Compta") ;
  pSuper->trace(&ps, 1, NSSuper::trSteps) ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::Compta", standardError, 0) ;
}
}

//------------------------------------------------------------------//  générer les conclusions manuelle et automatique
//------------------------------------------------------------------
void
NSCRReadOnlyView::Conclusion()
{
try
{
  if (_bEnConclusion)
    return ;

  _bEnConclusion = true ;
  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;  if ((NSCRDocument*) NULL == pCRDoc)    return ;
  if ((NSSmallBrother*) NULL == _pBigBoss)    _pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;

  if (false == _pBigBoss->ouvreResModule())    return ;

  if (false == _pBigBoss->pContexte->getFilGuide()->Prend())    return ;

  if (false == _pBigBoss->pContexte->getDico()->Prend())    return ;

  _pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/;
  _pBigBoss->Conclusion() ;
  _pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
  Initialisation() ;
  UpdateWindow() ;  Invalidate() ;
  _pDoc->SetDirty(true) ;
  _bEnConclusion = false ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::Conclusion", standardError, 0) ;
}
}

//------------------------------------------------------------------//  générer les conclusions manuelle et automatique
//------------------------------------------------------------------
void
NSCRReadOnlyView::Codage()
{
try
{
	if (_bEnCodage)
		return ;
	_bEnCodage = true ;

	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

	if ((NSSmallBrother*) NULL == _pBigBoss)		_pBigBoss = new NSSmallBrother(pContexte, pCRDoc->getPatPathoPointer()) ;

	if (false == _pBigBoss->ouvreResModule())
		return ;
	if (false == _pBigBoss->pContexte->getFilGuide()->Prend())
		return ;
	if (false == _pBigBoss->pContexte->getDico()->Prend())
		return ;

	_pBigBoss->setMotherWindow(GetWindow()) /*pFenetre*/;

	_pBigBoss->Codage() ;	_pBigBoss->_contexteModule = pCRDoc->_contexteModule ;
	Initialisation() ;
	UpdateWindow() ;
	Invalidate() ;
	_pDoc->SetDirty(true) ;
	_bEnCodage = false ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::Codage", standardError, 0) ;
}
}
voidNSCRReadOnlyView::TLibreOrdonnance(){try{  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;  if (NULL == pCRDoc)
    return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return ;

  PatPathoIter iter = PPT.begin() ;

  //
  // Trouver le noeud ayant pLocalisation comme localisation
  //
  bool trouve = false ;
  while (PPT.end() != iter)
  {
    // Note : sLocalisationSelectionne est une variable de classe, initialisée en
    // fonction de la ligne sélectionnée dans le click droit.
    if ((*iter)->getLocalisation() == _sLocalisationSelectionne)
    {
      trouve = true ;
      break ;
    }

    iter++ ;
  }

  if (false == trouve)
    return ;  // Le noeud père retrouvé ici est un noeud médicament.  // Il a une ensemble de fils (forme, phase, etc.) et éventuellement  // un fils qui a comme code sens "#TLI#" qui est le père d'un bloc de  // texte libre, avec lequel on constitue une patpatho  NSPatPathoArray PatPathoTLibre(pContexte->getSuperviseur()) ;  NSPatPathoArray SousPatPatho(pContexte->getSuperviseur()) ;  int iColBase = (*iter)->getColonne() ;  bool bTLibre = false ;  iter++ ;  while ((PPT.end() != iter) && ((*iter)->getColonne() > iColBase))  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if ((*iter)->getColonne() == iColBase+1)
    {
      // Dates
      if (sSens == "#TLI#")
      {
        bTLibre = true;
        PPT.ExtrairePatPatho(iter, &SousPatPatho) ;
        PPT.SupprimerFils(iter) ;
        PPT.SupprimerItem(iter) ;
      }
      else
        iter++ ;
    }
    else
      iter++ ;
  }  if (bTLibre)  {    PatPathoTLibre.ajoutePatho("#TLI#1", 0) ;    // Insertion en queue (iter doit être ici egal à end())    PatPathoTLibre.InserePatPatho(PatPathoTLibre.end(), &SousPatPatho, 1) ;  }  NSPatPathoArray PatPathoCopy(pContexte->getSuperviseur()) ;  PatPathoCopy = PatPathoTLibre ;  NSSmallBrother BigBossTL(pContexte, &PatPathoTLibre) ;  BigBossTL.setMotherWindow(GetWindow()) /*pFenetre*/;  BigBossTL.EditionTexteLibre() ;  PatPathoTLibre = *(BigBossTL.getPatPatho()) ;  if (false == PatPathoTLibre.estEgal(&PatPathoCopy))  {    // Insertion avant iter du nouveau TL    PPT.InserePatPatho(iter, &PatPathoTLibre, iColBase+1) ;    _pDoc->setPatPatho(&PPT) ;    // réinitialisation pour nouvel affichage du document    Initialisation() ;
    UpdateWindow() ;    Invalidate() ;
    _pDoc->SetDirty(true) ;  }}catch (...){
	erreur("Exception TLibreOrdonnance.", standardError, 0) ;
}
}
void
NSCRReadOnlyView::MedicEnALD()
{
try
{
  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if (NULL == pCRDoc)
    return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return ;

  PatPathoIter iter = PPT.begin() ;
  // on note la colonne de base
  int iColBase = (*iter)->getColonne() ;

  //
  // Trouver le noeud ayant pLocalisation comme localisation
  //
  bool bTrouve = false ;
  while (PPT.end() != iter)
  {
    // Note : sLocalisationSelectionne est une variable de classe, initialisée en
    // fonction de la ligne sélectionnée dans le click droit.
    if ((*iter)->getLocalisation() == _sLocalisationSelectionne)
    {
      bTrouve = true ;
      break ;
    }

    iter++  ;
  }

  if (false == bTrouve)
    return ;

  PatPathoIter iterNode = iter ;

  string sCodeSens = string("") ;

  // on remonte en arrière jusqu'au premier chapître ALD ou Hors-ALD
  while ((*iter)->getColonne() > iColBase)
  {
    string sElemLex = (*iter)->getLexique() ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sCodeSens) ;

    if ((string("GPALD") == sCodeSens) || (string("GPCOD") == sCodeSens))
      break ;

    iter-- ;
  }

  int colonne = (*iter)->getColonne() ;

  if (colonne == iColBase)
  {
    // on est remonté au noeud root => chapître ALD ou Hors-ALD introuvables
    erreur("Vous devez passer votre ordonnance en mode ALD avant d'utiliser cette option.", warningError, 0) ;
    return ;
  }

  if (string("GPALD") == sCodeSens)
  {
    erreur("Ce médicament est déjà classé en ALD.", warningError, 0) ;
    return ;
  }

  // on reconstruit la patpatho médicament (à déplacer)
  NSPatPathoArray PPTMedic(pContexte->getSuperviseur()) ;
  // on extrait d'abord la sous patpatho du médicament
  NSPatPathoArray SousPPT(pContexte->getSuperviseur()) ;
  PPT.ExtrairePatPatho(iterNode, &SousPPT) ;

  // on copie le node médicament dans sa nouvelle patpatho
  // et on insère ses fils
  PPTMedic.ajoutePatho(iterNode, standardError, 0) ;
  PPTMedic.InserePatPatho(PPTMedic.end(), &SousPPT, 1) ;

  // on supprime maintenant le médicament de la patpatho source
  PPT.SupprimerFils(iterNode) ;
  PPT.SupprimerItem(iterNode) ;

  // on recherche le chapître source pour voir s'il contient encore des médicaments
  // car sinon, on devra le supprimer
  iter = PPT.ChercherItem("GPCOD1") ;
  PatPathoIter iterChapter = iter;
  int iColChapter = (*iter)->getColonne() ;
  iter++;

  int iNbMedic = 0 ;

  while ((PPT.end() != iter) && ((*iter)->getColonne() > iColChapter))
  {
    if ((*iter)->getColonne() == (iColChapter + 1))
      iNbMedic++ ;

    iter++ ;
  }

  // cas suppression du chapître source
  if (0 == iNbMedic)
    PPT.SupprimerItem(iterChapter);

  // on recherche maintenant le chapître destination pour voir s'il existe
  // sinon, on devra le créer dans l'ordre : 1.GPALD 2.GPCOD
  bool bExisteALD = true;

  iter = PPT.ChercherItem("GPALD1") ;
  if (PPT.end() == iter)
    bExisteALD = false ;

  if (false == bExisteALD)
  {
    // on reconstruit pPPTMedic avec le chapître ALD
    SousPPT.vider() ;
    SousPPT = PPTMedic ;
    PPTMedic.vider() ;
    PPTMedic.ajoutePatho("GPALD1", 0) ;
    PPTMedic.InserePatPatho(PPTMedic.end(), &SousPPT, 1) ;
  }

  // on cherche le chapître suivant pour insertion avant iter
  // car iter est au pire égal à end()
  iter = PPT.ChercherItem("GPCOD1") ;
  if (bExisteALD)
    PPT.InserePatPatho(iter, &PPTMedic, iColBase+2) ;
  else
    PPT.InserePatPatho(iter, &PPTMedic, iColBase+1) ;

  _pDoc->setPatPatho(&PPT) ;

  // réinitialisation pour nouvel affichage du document
  Initialisation() ;
  UpdateWindow() ;  Invalidate() ;
  _pDoc->SetDirty(true) ;
}
catch (...)
{
  erreur("Exception MedicEnALD.", standardError, 0) ;
}
}

void
NSCRReadOnlyView::MedicHorsALD()
{
try
{
  NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if (NULL == pCRDoc)
    return ;

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  pCRDoc->initFromPatPatho(&PPT) ;

  if (PPT.empty())
    return ;

  PatPathoIter iter = PPT.begin() ;
  // on note la colonne de base
  int iColBase = (*iter)->getColonne() ;

  //
  // Trouver le noeud ayant pLocalisation comme localisation
  //
  bool trouve = false ;
  while (PPT.end() != iter)
  {
    // Note : sLocalisationSelectionne est une variable de classe, initialisée en
    // fonction de la ligne sélectionnée dans le click droit.
    if ((*iter)->getLocalisation() == _sLocalisationSelectionne)
    {
      trouve = true ;
      break ;
    }

    iter++ ;
  }

  if (false == trouve)
    return ;

  PatPathoIter iterNode = iter ;

  string sCodeSens = string("") ;

  // on remonte en arrière jusqu'au premier chapître ALD ou Hors-ALD
  while ((*iter)->getColonne() > iColBase)
  {
    string sElemLex = (*iter)->getLexique() ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sCodeSens) ;

    if ((sCodeSens == "GPALD") || (sCodeSens == "GPCOD"))
      break ;

    iter-- ;
  }

  int colonne = (*iter)->getColonne() ;

  if (colonne == iColBase)
  {
    // on est remonté au noeud root => chapître ALD ou Hors-ALD introuvables
    erreur("Vous devez passer votre ordonnance en mode ALD avant d'utiliser cette option.", warningError, 0) ;
    return ;
  }

  if (sCodeSens == "GPCOD")
  {
    erreur("Ce médicament est déjà classé Hors-ALD.", warningError, 0) ;
    return ;
  }

  // on reconstruit la patpatho médicament (à déplacer)
  NSPatPathoArray PPTMedic(pContexte->getSuperviseur()) ;
  // on extrait d'abord la sous patpatho du médicament
  NSPatPathoArray SousPPT(pContexte->getSuperviseur());
  PPT.ExtrairePatPatho(iterNode, &SousPPT);

  // on copie le node médicament dans sa nouvelle patpatho
  // et on insère ses fils
  PPTMedic.ajoutePatho(iterNode, standardError, 0) ;
  PPTMedic.InserePatPatho(PPTMedic.end(), &SousPPT, 1) ;

  // on supprime maintenant le médicament de la patpatho source
  PPT.SupprimerFils(iterNode) ;
  PPT.SupprimerItem(iterNode) ;

  // on recherche le chapître source pour voir s'il contient encore des médicaments
  // car sinon, on devra le supprimer
  iter = PPT.ChercherItem("GPALD1") ;
  PatPathoIter iterChapter = iter;
  int iColChapter = (*iter)->getColonne() ;
  iter++;

  int iNbMedic = 0 ;

  while ((PPT.end() != iter) && ((*iter)->getColonne() > iColChapter))
  {
    if ((*iter)->getColonne() == (iColChapter + 1))
      iNbMedic++ ;

    iter++ ;
  }

  // cas suppression du chapître source
  if (0 == iNbMedic)
    PPT.SupprimerItem(iterChapter) ;

  // on recherche maintenant le chapître destination pour voir s'il existe
  // sinon, on devra le créer dans l'ordre : 1.GPALD 2.GPCOD
  bool bExisteHorsALD = true;

  iter = PPT.ChercherItem("GPCOD1") ;
  if (PPT.end() == iter)
    bExisteHorsALD = false ;

  if (false == bExisteHorsALD)
  {
    // on reconstruit pPPTMedic avec le chapître ALD
    SousPPT.vider() ;
    SousPPT = PPTMedic ;
    PPTMedic.vider() ;
    PPTMedic.ajoutePatho("GPCOD1", 0) ;
    PPTMedic.InserePatPatho(PPTMedic.end(), &SousPPT, 1) ;
  }

  // on insère en queue dans tous les cas
  if (bExisteHorsALD)
    PPT.InserePatPatho(PPT.end(), &PPTMedic, iColBase+2) ;
  else
    PPT.InserePatPatho(PPT.end(), &PPTMedic, iColBase+1) ;

  _pDoc->setPatPatho(&PPT) ;

  // réinitialisation pour nouvel affichage du document
  Initialisation() ;
  UpdateWindow() ;  Invalidate() ;
  _pDoc->SetDirty(true) ;
}
catch (...)
{
    erreur("Exception MedicHorsALD.", standardError, 0) ;
}
}
// Fonction CanClose : Appel de CanClose du document////////////////////////////////////////////////////////////////
bool
NSCRReadOnlyView::CanClose()
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return false ;

  if (pCRDoc->isEnregEnCours())
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "thisDocumentIsBeingSaved") ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
	if (false == pCRDoc->CanClose())
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;

    string sText ;
		if (_pDoc->_pDocInfo)
    	sText = pContexte->getSuperviseur()->getText("documentManagement", "doYouWantToSaveChangesForDocument") ;
		else
    	sText = pContexte->getSuperviseur()->getText("documentManagement", "doYouWantToSaveDocument") ;
    if (pCRDoc->GetTitle())
      sText += string(" ") + pCRDoc->GetTitle() ;
    sText += string(" ?") ;

    int retVal = MessageBox(sText.c_str(), sCaption.c_str(), MB_YESNOCANCEL) ;
		if (IDCANCEL == retVal)
			return false ;
		if (IDYES == retVal)
		{
			bool okEnreg = pCRDoc->enregistre() ;

			if (okEnreg)
			{
				pMyApp->FlushControlBar() ;
				_bSetupToolBar = false ;
			}
			return okEnreg ;
		}
	}
	_pDoc->SetDirty(false) ;

	pMyApp->FlushControlBar() ;
	_bSetupToolBar = false ;
	return true ;
}

//-------------------------------------------------------------------------//--------------------------------------------------------------------------
void
NSCRReadOnlyView::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	bool invalider = NSRefVue::EvLButtonDown(modKeys, point) ;
	if (invalider)
		Invalidate() ;
}

// Click droit//////////////////////////////////////////////////////
voidNSCRReadOnlyView::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
try
{
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;  if ((NSCRDocument*) NULL == pCRDoc)    return ;
	NS_CLASSLIB::TPoint ptClick = point ;
	// pPopupMenuLigne->TrackPopupMenu(TPM_RIGHTBUTTON, point, 0, *this);	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;	pDC->SetMapMode(MM_LOMETRIC) ;

	//	// On transforme les unités physiques d'écran en unités logiques
	//
	pDC->DPtoLP(&ptClick) ;	delete pDC ;

	//	// On positionne le point dans la page
	//
	ptClick.x += _HautGcheFenetre.x ;	ptClick.y += _HautGcheFenetre.y ;

	//	// On cherche si ce point se situe sur une ligne
	// Attention, les ordonnées sont < 0
	//
	NSLignesArrayIter iter ;
	bool tourner = true ;
  if (false == _Lignes.empty())
		for (iter = _Lignes.begin() ; (_Lignes.end() != iter) && (tourner) ; )		{
    	if ((ptClick.x >= (*iter)->Boite.left) && (ptClick.x <= (*iter)->Boite.right) &&
          (ptClick.y <= (*iter)->Boite.top)  && (ptClick.y >= (*iter)->Boite.bottom) )
    		tourner = false ;
      else
      	iter++ ;
    }

	//	// Si ce point se situe sur une ligne, on cherche la phrase correspondante
	//
	if (false == tourner)
	{
		UINT iPhrase = (*iter)->iNumPhrase ;
    NSCRPhrArrayIter it ;
    bool bOrdo = false ;

    NSPatPathoInfo* pRootPathoInfo = pCRDoc->getRootPathoInfo() ;
    if (pRootPathoInfo)
    {
    	string sElemLex = pRootPathoInfo->getLexique() ;
      string sSens ;
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;
      if (string("ZORDO") == sSens)
      	bOrdo = true ;
    }

    for (it = pCRDoc->_aCRPhrases.begin() ; (pCRDoc->_aCRPhrases.end() != it) && ((*it)->iNumero != iPhrase) ; it++) ;

		//		// Si on a trouvé une phrase, on remonte à la (aux) lésion(s)
		//

    TPopupMenu *menu = new TPopupMenu() ;
		if (pCRDoc->_aCRPhrases.end() != it)		{
    	if (false == (*it)->aLesions.empty())
      	_sLocalisationSelectionne = *((*it)->aLesions)[(*it)->aLesions.size() - 1] ;

      if (false == bOrdo)
      {
      	if ((*it)->aLesions.empty())        	menu->AppendMenu(MF_GRAYED, 0, "Corriger") ;
        else
        	menu->AppendMenu(MF_STRING, IDC_CORRIGER, "Corriger") ;

        menu->AppendMenu(MF_STRING, IDC_REPRENDRE_CR, "Reprendre compte rendu") ;        menu->AppendMenu(MF_STRING, IDC_CONCLUSION_CR, "Conclusion") ;
      }
      else
      {
      	menu->AppendMenu(MF_STRING, IDC_TLIB_ORDO, "Texte libre") ;
        menu->AppendMenu(MF_STRING, IDC_MEDIC_ALD, "Passer en ALD") ;
        menu->AppendMenu(MF_STRING, IDC_MEDIC_NONALD, "Passer hors ALD") ;
      }

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
      menu->AppendMenu(MF_STRING, IDC_COPY_ARBRE, "Copier arbre") ;

      // s'il existe une ligne sélectionnée au moins      if (_iLigneEnCours)
      	menu->AppendMenu(MF_STRING, IDC_COPYTEXT_CR, "Copier texte") ;
      else
      	menu->AppendMenu(MF_GRAYED, 0, "Copier texte") ;

      menu->AppendMenu(MF_STRING, IDC_SELECTALL_CR, "Tout sélectionner") ;      menu->AppendMenu(MF_SEPARATOR, 0, 0);

      if ((*it)->aLesions.empty())      	menu->AppendMenu(MF_GRAYED, 0, "Supprimer") ;
      else
      	menu->AppendMenu(MF_STRING, IDC_SUPPRIMER, "Supprimer") ;

      ClientToScreen(point) ;
      menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;		}

    delete menu ;	}
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::EvRButtonDown", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
// Libération de la dernière barre d'outils créee
//---------------------------------------------------------------------------
void
NSCRReadOnlyView::EvSetFocus(THandle hWndLostFocus /* may be 0 */)
{
	activateMUEViewMenu() ;

	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
	NSRefVue::EvSetFocus(hWndLostFocus) ;
	if (_bSetupToolBar && (GetWindow() != pMyApp->GetToolBarWindow()))  {
  	SetupToolBar() ;
    pMyApp->SetToolBarWindow(GetWindow()) ;
  }

  pMyApp->setMenu(string("menubar_cr"), &_hAccelerator) ;
}

voidNSCRReadOnlyView::SetupToolBar()
{
	TMyApp* pMyApp = _pDoc->pContexte->getSuperviseur()->getApplication() ;
	pMyApp->FlushControlBar() ;

	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_COMPTA_CR, IDC_COMPTA_CR, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_CODAGE_CR, IDC_CODAGE_CR, TButtonGadget::Command)) ;
	pMyApp->getSecondaryControlBar()->Insert(*new NSFlatButtonGadget(IDC_CONCLUSION_CR, IDC_CONCLUSION_CR, TButtonGadget::Command)) ;

	pMyApp->getSecondaryControlBar()->LayoutSession() ;}

voidNSCRReadOnlyView::CmEnregistre()
{
  string ps = string("User saves the report") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if ((NSCRDocument*) NULL == pCRDoc)
    return ;

	bool bNewDoc = false ;
	if ((NSDocumentInfo*) NULL == pCRDoc->_pDocInfo)		bNewDoc = true ;

	if (pCRDoc->enregistre())	{
		// on remet à jour le titre
		SetDocTitle(pCRDoc->GetTitle(), 0) ;
		_pDoc->SetDirty(false) ;

		if (bNewDoc && (pContexte->getSuperviseur()->isComptaAuto()) && (pCRDoc->ComptaAuto()))		{
			string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
			int retVal = MessageBox("Il n'existe pas de fiche comptable pour ce document. Voulez-vous lancer la comptabilité ?", sCaption.c_str(), MB_YESNO) ;
			if (IDYES == retVal)
				Compta() ;
		}
	}
	else
		_pDoc->SetDirty(true) ;
}

//---------------------------------------------------------------------------//  Function: NSCRReadOnlyView::InitialiseLignes()
//
//  Arguments: Aucuns
//
//  Description:
//            Initialise le style du document à partir du fichier N.MOD
//  Returns:
//            TRUE/FALSE
//---------------------------------------------------------------------------
bool
NSCRReadOnlyView::InitialiseLignes()
{
try
{
	NSLigne*					  pLigne ;
	string 			  		  Chaine = "", ChaineLigne, sLocLes, sPhrase ;
	NSStyleParagraphe	  StylePara ;
	TFont*						  pPolice ;
	unsigned					  NumeroPara, NumeroPolice, NumeroPage ;
	size_t						  Curseur, NouBlanc, PreBlanc ;
	NS_CLASSLIB::TPoint PointRef, PointTransfert ;
	ifstream 					  inFile ;
	long							  TraiteLigne = 0 ;
  long                LargeurPossible = 0 ;
  long                LargeurGauche = 0 ;
	NS_CLASSLIB::TSize  TailleChaine ;
	LONG 							  lPolicetmHeight ;
	NSFont		 		 		  PoliceVoulue ;
	NSCRPhrase*		  	  pPhrase ;
	unsigned					  iNumPhrase = 0 ;
	//NSCRPhraLes*		  pPhraLes;
	TEXTMETRIC				  TextMetric ;
	NSPage						  stylePage, *pStylePage ;
	bool							  bGerePages = false ;
	string						  line ;

	NSCRPhraLesArray	  aLesBuf ;   // Localisation(s)
	NSCRPhraLesArray	  aLesPath ;  // Chemin(s)
	//
	// Mise à zéro des phrases et des lignes
	//
	NSCRDocument* pCRDoc = dynamic_cast<NSCRDocument*>(_pDoc) ;
  if (NULL == pCRDoc)
    return false ;

	pCRDoc->_aCRPhrases.vider() ;
	_Lignes.vider() ;
	//
	// Tentative d'ouverture du fichier qui contient le compte rendu
	//
	inFile.open((pCRDoc->getTextFile()).c_str()) ;
	if (!inFile)
		return false ;
	//
	// Chargement du fichier dans une string
	//
	while (!inFile.eof())
	{
  	getline(inFile,line) ;
    if (line != "")
    	Chaine += line + "\n" ;
	}
	//
	inFile.close() ;
	//
	// On récupère un "contexte d'information" concernant l'écran
	//
	TIC* pDC = new TIC("DISPLAY", NULL, NULL) ;
	pDC->SetMapMode(MM_LOMETRIC) ;
	//
	// On initialise le style du paragraphe n°1 et la police qui correspond
	//
	NumeroPara 	= 1 ;
	StylePara 	= *(_StylesParagraphe[NumeroPara-1]) ;
	NumeroPolice = StylePara.NumPolice ;
	PoliceVoulue = *(_StylesPolImp[NumeroPolice-1]) ;
	//
	// La taille de la police est stockée en unité physiques d'écran,
	// il faut donc la transformer en unités logiques
	//
	if (PoliceVoulue.logFont.lfHeight >= 0)
	{
		PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
		PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
		pDC->DPtoLP(&PointTransfert) ;
		PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
		PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
	}
	//
	// Si la hauteur est <0, elle est en points.
	//
	//
	// On sélectionne la police dans le DC
	//
	pPolice	= new TFont(&(PoliceVoulue.logFont)) ;
	pDC->SelectObject(*pPolice) ;
	//if ((LargeurPolice == 0) || (NumeroPara == 4))
   	//{
	if (pDC->GetTextMetrics(TextMetric))
	{
  	_LargeurPolice = TextMetric.tmAveCharWidth ;
    lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
	}
   	//}
	//
	// On initialise le rectangle de texte dans la page
	//
	NumeroPage = 1 ;
	pStylePage = donnePage(NumeroPage) ;
	if (pStylePage == 0)
  	pStylePage = donnePage(0) ;

	if (pStylePage != 0)
  	stylePage = *pStylePage ;
	else
  	stylePage.loadDefault() ;

	NS_CLASSLIB::TRect RectanglePage, RectangleTexte ;
	RectanglePage.Set(_RectangleGlobal.Left(),
    				        _RectangleGlobal.Top(),
                    _RectangleGlobal.Right()-stylePage.MargeGauche-stylePage.MargeDroite,
                    _RectangleGlobal.Bottom()+stylePage.MargeHaute+stylePage.MargeBasse) ;

	RectangleTexte.Set(RectanglePage.Left() + stylePage.MargeTxtGch,
                      RectanglePage.Top() - stylePage.MargeTxtHaut,
                      RectanglePage.Right() - stylePage.MargeTxtDte,
                      RectanglePage.Bottom() + stylePage.MargeTxtBas) ;
	//
	// On initialise le point de référence en haut et à gauche
	//
	PointRef.x = RectangleTexte.Left() ;
	PointRef.y = RectangleTexte.Top() ;

	sPhrase = "" ;
	// on appelle metDecor au début pour tenter d'avoir un tableau Lignes
	// avec les lignes d'en-tete AVANT les lignes de texte.
	metDecor(&RectanglePage, pDC, NumeroPage, 1) ;

	//
	// Lecture de la chaine
	//
	size_t ik = 0 ;
	while (ik < strlen(Chaine.c_str()))
	{
		//
    // On avance jusqu'au prochain tag de début (chr(27))
    //
    VaA27(&Chaine, &ik, &aLesBuf, &aLesPath) ;

    if (ik >= strlen(Chaine.c_str()))
    	break ;
    ik++ ;
    //
    // On prépare le style de paragraphe, en cas de texte ordinaire,
    // on prend d'autorité le paragraphe 6
    //
    if (Chaine[ik] == 'T')
    	NumeroPara = 6 ;
    else
    	NumeroPara = donneDigit(Chaine[ik]) ;
    ik++ ;

		//
		// Gestion particulière pour les tableaux
  	//
    // L'ensemble du tableau est situé entre un (27)'9' et un (28)'9'
    //
    if (9 == NumeroPara)
    {
    	pPhrase = new NSCRPhrase ;
      if (!(aLesBuf.empty()))
      {
      	pPhrase->aLesions = aLesBuf ;
        aLesBuf.vider() ;
      }
      if (false == aLesPath.empty())
      {
      	pPhrase->aChemins = aLesPath ;
        aLesPath.vider() ;
      }
      iNumPhrase++ ;
      pPhrase->iNumero = iNumPhrase ;

      string sChaineTableau = "" ;
      bool   tourner  = true ;
      size_t ikLastTr = 0 ;
      size_t ikLastTd = 0 ;
      while (tourner && (ik < strlen(Chaine.c_str())))
      {
      	if (char(28) == Chaine[ik])
        {
        	ik++ ;
          if ('9' == Chaine[ik])
          	tourner = false ;
          else
          {
          	if      ('8' == Chaine[ik])
            	ikLastTr = ik ;
            else if ('7' == Chaine[ik])
            	ikLastTd = ik ;
          	sChaineTableau += char(28) ;
            sChaineTableau += Chaine[ik] ;
          }
        }
        else
        	sChaineTableau += Chaine[ik] ;
        ik++ ;
      }
      // Let's suppose an end of table was not found, close after the last tr
      //
      if (ik >= strlen(Chaine.c_str()))
      {
      	if (ikLastTr > 0)
        {
        	ik = ikLastTr ;
          string sToFind = string(1, char(28)) + string(1, '8') ;
          size_t iLastTr = sChaineTableau.find_last_of(sToFind) ;
          if (iLastTr != string::npos)
          	sChaineTableau[iLastTr + 2] = '\0' ;
        }
        else if (ikLastTd > 0)
        {
        	ik = ikLastTd ;
          string sToFind = string(1, char(28)) + string(1, '7') ;
          size_t iLastTd = sChaineTableau.find_last_of(sToFind) ;
          if (iLastTd != string::npos)
          	sChaineTableau[iLastTd + 2] = '\0' ;
          string sToAdd = string(1, char(28)) + string(1, '8') ;
          sChaineTableau += sToAdd ;
        }
      }
      //
      // On prend d'autorité le paragraphe 7
      //
      StylePara  	 = *(_StylesParagraphe[6]) ;
      NumeroPolice = StylePara.NumPolice ;
      delete pPolice ;
      pPolice = 0 ;
      PoliceVoulue = *(_StylesPolice[NumeroPolice-1]) ;
      if (PoliceVoulue.logFont.lfHeight >= 0)
      {
      	PointTransfert.x = PoliceVoulue.logFont.lfHeight ;
        PointTransfert.y = PoliceVoulue.logFont.lfWidth ;
        pDC->DPtoLP(&PointTransfert) ;
        PoliceVoulue.logFont.lfHeight = PointTransfert.x ;
        PoliceVoulue.logFont.lfWidth  = PointTransfert.y ;
      }
      pPolice = new TFont(&(PoliceVoulue.logFont)) ;
      pDC->SelectObject(*pPolice) ;
      if (pDC->GetTextMetrics(TextMetric))
      {
      	_LargeurPolice  = TextMetric.tmAveCharWidth ;
        lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
      }
      string tableau[50][50] ;
      size_t ikl        = 0 ;
      int    iLigne     = 0 ;
      int    iColonne   = 0 ;
      int    iNbColonne = 0 ;
      string sBuff      = string("") ;

      for (ikl = 0; ikl < strlen(sChaineTableau.c_str()); ikl++)
      {
      	if      (char(27) == sChaineTableau[ikl])
        	ikl++ ;
        else if (char(28) == sChaineTableau[ikl])
        {
        	ikl++ ;
          //
          // Fin colonne
          //
          if 		('7' == sChaineTableau[ikl])
          {
          	tableau[iLigne][iColonne] = sBuff ;
            sBuff = "" ;
            iColonne++ ;
          }
          //
          // Fin ligne
          //
          else if ('8' == sChaineTableau[ikl])
          {
          	if (iColonne > iNbColonne)
            	iNbColonne = iColonne ;
            iLigne++ ;
            iColonne = 0 ;
          }
        }
        else
        	sBuff += sChaineTableau[ikl] ;
      }
      //
      // On cherche la largeur de chaque colonne
      //
      LargeurPossible = LargeurGauche -
                            (RectangleTexte.Left() + StylePara.RetraitGauche) ;
      int LargeurColonne = LargeurPossible / iNbColonne ;
      //
      // On tabule avec des ' ' => connaitre la largeur d'un ' '
      //
      NS_CLASSLIB::TSize sizeBlanc = pDC->GetTextExtent(" ", 1) ;
      int Tailleblanc = sizeBlanc.cx ;

      string sGrandePhrase = "" ;
      int iTailleTexte ;
      for (int iLignTab = 0; iLignTab < iLigne; iLignTab++)
      {
      	sPhrase = "" ;
        for (int jTab = 0; jTab < iNbColonne; jTab++)        {
        	int nbrBlancsAvant = 0 ;
          int nbrBlancsApres = 0 ;
          string sTxtCol = tableau[iLignTab][jTab] ;
          TailleChaine = pDC->GetTextExtent(sTxtCol.c_str(), strlen(sTxtCol.c_str())) ;
          iTailleTexte = TailleChaine.cx ;
          if (iTailleTexte < LargeurColonne)
          {
          	int nbBlanc = (LargeurColonne - iTailleTexte) / Tailleblanc ;
            if (nbBlanc > 1)
            	nbrBlancsAvant = nbBlanc / 2 ;
            nbrBlancsApres = nbBlanc - nbrBlancsAvant ;
          }

          sPhrase += string(nbrBlancsAvant, ' ') +
                               sTxtCol +
                               string(nbrBlancsApres, ' ');

          sGrandePhrase += sPhrase ;
        }
        //
        // Création d'une ligne vierge
        //
        pLigne = new NSLigne() ;
        //
        // Initialisation de tous les paramètres déjà connus
        //
        pLigne->iNumPhrase = iNumPhrase ;
        pLigne->numPage = NumeroPage ;
        if (TraiteLigne == 1)
        	pLigne->Boite.left = RectangleTexte.Left() + StylePara.RetraitPremLigne ;
        else
        	pLigne->Boite.left = RectangleTexte.Left() + StylePara.RetraitGauche ;
        pLigne->Boite.right  = LargeurGauche ;
        pLigne->Boite.top    = PointRef.y ;
        pLigne->Boite.bottom = PointRef.y ;
        // pLigne->CouleurTexte  = *(StylePara.pCouleurTexte);
        pLigne->numParagraphe = 7 ;
        pLigne->numLigne      = TraiteLigne ;
        pLigne->Texte         = sPhrase ;
        LONG lHauteur         = lPolicetmHeight ;
        if (lHauteur < StylePara.HauteurLigne)
        	lHauteur = StylePara.HauteurLigne ;
        pLigne->Boite.bottom -= lHauteur ;
        PointRef.Offset(0, -lHauteur) ;
        //
        //
        //
        if (iLignTab < iLigne-1)
        {
        	TraiteLigne++ ;
          pLigne->DerniereLigne = FALSE ;
        }
        else
        {
        	TraiteLigne = 0 ;
          pLigne->DerniereLigne = TRUE ;
        }
        _Lignes.push_back(new NSLigne(*pLigne)) ;
        delete pLigne ;
        pLigne = 0 ;
      }
      //
      // Création du tableau comme d'une seule "phrase"
      //
      pPhrase->sTexte = sGrandePhrase ;
      pCRDoc->_aCRPhrases.push_back(new NSCRPhrase(*pPhrase)) ;
      delete pPhrase ;
      pPhrase = 0 ;
      sPhrase = "" ;
    }
    //
    // Gestion pour les lignes ordinaires
    //
    else
    {
      StylePara  	 = *(_StylesParagraphe[NumeroPara-1]);
      NumeroPolice = StylePara.NumPolice;
      delete pPolice;
      pPolice = 0;
      PoliceVoulue = *(_StylesPolice[NumeroPolice-1]);
      if (PoliceVoulue.logFont.lfHeight >= 0)
      {
        PointTransfert.x = PoliceVoulue.logFont.lfHeight;
        PointTransfert.y = PoliceVoulue.logFont.lfWidth;
        pDC->DPtoLP(&PointTransfert);
        PoliceVoulue.logFont.lfHeight = PointTransfert.x;
        PoliceVoulue.logFont.lfWidth  = PointTransfert.y;
      }
      pPolice = new TFont(&(PoliceVoulue.logFont));
      pDC->SelectObject(*pPolice);
      //if ((LargeurPolice == 0) || (NumeroPara == 4))
      //{
      if (pDC->GetTextMetrics(TextMetric))
      {
        _LargeurPolice  = TextMetric.tmAveCharWidth ;
        lPolicetmHeight = TextMetric.tmHeight + TextMetric.tmDescent ;
      }
      //}
      //
      // On traite la phrase (ou les phrases) jusqu'au tag de fin (chr(28))
      //
      //
      while ((ik < strlen(Chaine.c_str())) && (char(28) != Chaine[ik]))
      {
      	//
        // CR LF -> changement de paragraphe
        //
				if ((Chaine[ik] == char(13)) || (Chaine[ik] == char(10)))
        {
        	ik++ ;
          if ((Chaine[ik] == char(13)) || (Chaine[ik] == char(10)))
          	ik++ ;
          //
          // On crée un nouveau NSCRPhrase
          //
          pPhrase = new NSCRPhrase ;
					//
          // On lui transmet l'éventuel aLesBuf
          //
          if (false == aLesBuf.empty())
          {
          	pPhrase->aLesions = aLesBuf ;
            aLesBuf.vider() ;
          }
          if (false == aLesPath.empty())
          {
          	pPhrase->aChemins = aLesPath ;
            aLesPath.vider() ;
          }
          pPhrase->sTexte = sPhrase ;
					iNumPhrase++ ;
					pPhrase->iNumero = iNumPhrase ;
          pCRDoc->_aCRPhrases.push_back(new NSCRPhrase(*pPhrase)) ;
					delete pPhrase ;
          pPhrase = 0 ;
					//
					// On prévoit l'espace au dessus du paragraphe
					//
					PointRef.Offset(0, -int(StylePara.EspaceDessus));
					//
					// Calcul de la largeur utilisable sans le retrait gauche
					//
					LargeurGauche = RectangleTexte.Width() - StylePara.RetraitDroit;
					//
					// On traite le paragraphe, ligne par ligne
					//
					Curseur = 0; PreBlanc = 0;
					TraiteLigne = 1;
					while (TraiteLigne > 0)
					{
          	//
						// Création d'une ligne vierge
						//
						pLigne = new NSLigne();
						//
						// Initialisation de tous les paramètres déjà connus
						//
						pLigne->iNumPhrase = iNumPhrase;
						pLigne->numPage = NumeroPage;
						if (TraiteLigne == 1)
							pLigne->Boite.left = RectangleTexte.Left()+StylePara.RetraitPremLigne;
						else
							pLigne->Boite.left = RectangleTexte.Left()+StylePara.RetraitGauche;
						pLigne->Boite.right   = LargeurGauche;
						pLigne->Boite.top	  	 = PointRef.y;
						pLigne->Boite.bottom  = PointRef.y;
						// pLigne->CouleurTexte  = *(StylePara.pCouleurTexte);
						pLigne->numParagraphe = NumeroPara;
						pLigne->numLigne		 = TraiteLigne;
						//
						// Traitement des chaines de taille non nulle
						//
						if (strlen(sPhrase.c_str()) > 0)
						{
							//
							// Détermination de la largeur utilisable
							//
							LargeurPossible = UINT(pLigne->Boite.Width());
							//
							// Recherche du segment de chaîne qui "tient" dans cette largeur
							//
							NouBlanc = Curseur;
							do
							{
								PreBlanc = NouBlanc;
								NouBlanc = sPhrase.find(' ', PreBlanc+1);
								if (NouBlanc == NPOS)
									break;

								TailleChaine = pDC->GetTextExtent(string(sPhrase, Curseur, NouBlanc-Curseur).c_str(),
															 int(NouBlanc-Curseur));
							}
							while (TailleChaine.cx < LargeurPossible);
							//
							// Si on est en fin de chaîne, on regarde
                            // si le dernier mot "tient"
							//
							if (NouBlanc == NPOS)
							{
								TailleChaine = pDC->GetTextExtent(string(sPhrase, Curseur, strlen(sPhrase.c_str())-Curseur).c_str(),
															 int(strlen(sPhrase.c_str())-Curseur));
								if (TailleChaine.cx < LargeurPossible)
									PreBlanc = strlen(sPhrase.c_str());
							}
							//
							// On met le segment de chaine "qui tient" dans la chaine Texte
							// de la ligne en cours
							//
							pLigne->Texte = string(sPhrase, Curseur, PreBlanc-Curseur);
							//
							// On calcule la vraie taille de cette chaine
							//
                        	strip(pLigne->Texte);

							TailleChaine = pDC->GetTextExtent(pLigne->Texte.c_str(),
														 int(strlen(pLigne->Texte.c_str())));

							//
							// On initialise Boite : rectangle exinscrit à la ligne
							//

              LONG lHauteur = lPolicetmHeight ;
              if (lHauteur < StylePara.HauteurLigne)
								lHauteur = StylePara.HauteurLigne ;
							pLigne->Boite.bottom -= lHauteur ;
							PointRef.Offset(0, -lHauteur) ;
							//
							//
							//
							if (PreBlanc < strlen(sPhrase.c_str()))
							{
								Curseur = PreBlanc + 1 ;
								TraiteLigne++ ;
								pLigne->DerniereLigne = FALSE ;
							}
							else
							{
								TraiteLigne = 0 ;
								pLigne->DerniereLigne = TRUE ;
							}
						}
						//
						// Traitement des chaines vides
						//
						else
						{
							pLigne->Texte = "" ;
							//
							// On initialise Boite : rectangle exinscrit à la ligne
							//
							TailleChaine = pDC->GetTextExtent(" ", 1) ;
							if (TailleChaine.cy < StylePara.HauteurLigne)
								TailleChaine.cy = StylePara.HauteurLigne ;
							pLigne->Boite.bottom -= TailleChaine.cy ;
							PointRef.Offset(0, -TailleChaine.cy) ;
							//
							// On précise que le traitement est terminé
							//
							TraiteLigne = 0 ;
							pLigne->DerniereLigne = TRUE ;
						}
            //
            // Si la ligne déborde de la page, elle passe en page suivante
            //
            if ((abs(PointRef.y) > abs(RectangleTexte.Height())) && bGerePages)
            {
            	metDecor(&RectanglePage, pDC, NumeroPage, 1) ;
              NumeroPage++ ;

              pStylePage = donnePage(NumeroPage) ;
              if (pStylePage == 0)
              	pStylePage = donnePage(0) ;

              if (pStylePage != 0)
              	stylePage = *pStylePage ;
              else
              	stylePage.loadDefault() ;

              NS_CLASSLIB::TRect RectanglePage, RectangleTexte ;
              RectanglePage.Set(_RectangleGlobal.Left(),
                                _RectangleGlobal.Top(),
                     						_RectangleGlobal.Right()-stylePage.MargeGauche-stylePage.MargeDroite,
                     						_RectangleGlobal.Bottom()+stylePage.MargeHaute+stylePage.MargeBasse) ;

              RectangleTexte.Set(RectanglePage.Left() + stylePage.MargeTxtGch,
                                 RectanglePage.Top() - stylePage.MargeTxtHaut,
                                 RectanglePage.Right() - stylePage.MargeTxtDte,
                                 RectanglePage.Bottom() + stylePage.MargeTxtBas) ;

              //
							// On initialise le point de référence en haut et à gauche
              //
							PointRef.x = RectangleTexte.Left() ;
              PointRef.y = RectangleTexte.Top() ;
              pLigne->numPage = NumeroPage ;
              pLigne->Boite.top	= PointRef.y ;
              pLigne->Boite.bottom = PointRef.y - TailleChaine.cy ;
							PointRef.Offset(0, -TailleChaine.cy) ;
            }
						//
						// On ajoute la ligne à Lignes
						//
            _Lignes.push_back(new NSLigne(*pLigne)) ;
						delete pLigne ;
            pLigne = 0 ;
					}
					//
					// On prévoit l'espace au dessous du paragraphe
					//
					PointRef.Offset(0, -int(StylePara.EspaceDessous)) ;
					sPhrase = "" ;        }
        else
        {
        	sPhrase += Chaine[ik] ;
          ik++ ;
        }
      }
    }
    if (Chaine[ik] == char(28))
    {
    	ik++ ;
      if (ik < strlen(Chaine.c_str()))
      	ik++ ;
    }
	}

	// on mettait avant le titre à la fin du tableau Lignes
	// et ça fout la zone pour la sélection de texte
	// metDecor(&RectanglePage, pDC, NumeroPage, 1);

	delete pDC ;
	delete pPolice ;
	inFile.close() ;
	return true ;
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::InitialiseLignes", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void
NSCRReadOnlyView::VaA27(string *pChaine, size_t *pik, NSCRPhraLesArray *pLesBuf, NSCRPhraLesArray *pLesPath)
{
try
{
	string sLocLes = "" ;
	//
  // On avance jusqu'au prochain tag de début (chr(27))
  //
	while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != char(27)))
	{
  	//
    // Si on rencontre un indicateur de patpatho, on le traite
    //
    // Si le paragraphe commence par un indicateur de lésions
    //
    if ((*pChaine)[*pik] == '|')
    {
    	pLesBuf->vider() ;
      sLocLes = "" ;
      (*pik)++ ;
      while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != '|'))
      {
      	// Prise des localisations
        //
        while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != '|') && ((*pChaine)[*pik] != '['))
        {
        	while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != '|') && ((*pChaine)[*pik] != '[') && ((*pChaine)[*pik] != '/'))
          {
          	sLocLes += (*pChaine)[*pik] ;
            (*pik)++ ;
          }
          if (sLocLes != "")
          {
          	pLesBuf->push_back(new string(sLocLes)) ;
            sLocLes = "" ;
          }
          if ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] == '/'))
          	(*pik)++ ;
        }
        //
        // Prise des chemins
        //
        if ((*pChaine)[*pik] == '[')
        {
        	(*pik)++ ;
          while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != '|'))
          {
          	while ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] != '|') && ((*pChaine)[*pik] != ']'))
            {
            	sLocLes += (*pChaine)[*pik] ;
              (*pik)++ ;
            }
            if (sLocLes != "")
            {
            	pLesPath->push_back(new string(sLocLes)) ;
              sLocLes = "" ;
            }
            if ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] == ']'))
            	(*pik)++ ;
            if ((*pik < strlen(pChaine->c_str())) && ((*pChaine)[*pik] == '['))
            	(*pik)++ ;
          }
        }
      }
      if (*pik < strlen(pChaine->c_str()))
      	(*pik)++ ;
    }
    else
    	(*pik)++ ;
  }
}
catch (...)
{
	erreur("Exception NSCRReadOnlyView::VaA27.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------// fermeture du document :
// L'historique est averti de la fermeture par le destructeur du document
// car lorsque la derniere vue se ferme, cela tue le document.
//---------------------------------------------------------------------------
void
NSCRReadOnlyView::EvClose()
{
	TWindow::EvClose() ;
}

void
NSCRReadOnlyView::EvDestroy()
{
}

bool
NSCRReadOnlyView::EvQueryEndSession()
{
	return TWindow::EvQueryEndSession() ;
}

