//**************************************************************************//  Traitement de la fiche historique
//**************************************************************************
#include <owl\listwind.h>
#include <owl\treewind.h>
#include <owl\window.h>

#include "nautilus\nssuper.h"
#include "nsbb\nspatpat.h"
#include "nautilus\nshisto.rh"
#include "nautilus\nstrihis.h"
#include "partage\nsdivfct.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsTlibre.h"
#include "nautilus\nsrechdl.h"  // pour le dialogue des propriétés
#include "nautilus\nscrvue.h"    // Document / Vues CN (Compte-Rendu Nautilus)
#include "nautilus\nsttx.h" 	 // Document / Vues TTX
#include "nautilus\nscsdoc.h"    // Document / Vues CS Consultation
#include "nautilus\nscsvue.h"    // Document / Vues CS Consultation
#include "nautilus\nscqvue.h"   // Document / Vues CQ Formulaires#include "nautilus\nscqdoc.h"#include "nautilus\nsCoopChartDV.h"#include "nautilus\nsdocview.h"#include "nautilus\nsinterface.h"#include "nautilus\nspatdlg.h"#include "nautilus\nsresour.h"#include "nsbb\nsbb.rh"
#define NODES_COL_BASE 1
#define NODES_LIN_BASE 1

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//									 Classe NSTreeHistorique
//----------------------------------------------------------------------
//----------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSTreeHistorique, NSTreeWindow)
	EV_NOTIFY_AT_CHILD(TVN_SELCHANGED, SelChanged),
  EV_WM_LBUTTONDBLCLK,  //double click
  EV_WM_CLOSE,
  EV_WM_KEYDOWN,
  EV_WM_KEYUP,
	EV_WM_SETFOCUS,
  EV_WM_VSCROLL,
  EV_WM_HSCROLL,
  NS_TV_KEYDOWN(EvTvKeyDown),
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDC_OUVRIR,     Ouvrir),
  EV_COMMAND(CM_EDITER,      Editer),
  EV_COMMAND(IDC_TOUTOUVRIR, ToutOuvrir),
  EV_COMMAND(IDC_OPEN_EXT,   OpenOutside),
  EV_COMMAND(IDC_OTER,       Oter),
  EV_COMMAND(IDC_PROPRIETE,  Proprietes),
  EV_COMMAND(IDC_PARAMETRES, Parametres),
  EV_COMMAND(IDC_DETRUIRE,   Detruire),
  EV_COMMAND(IDC_XML_EXPORT, XmlExport),
  EV_COMMAND(IDC_TRANSFERT,  Transfert),
  EV_COMMAND(IDC_FORCESIGN,  Reassign),
  EV_COMMAND(IDC_NEW_FROM,   CreateFrom),
END_RESPONSE_TABLE;

//----------------------------------------------------------------------
//----------------------------------------------------------------------
NSTreeHistorique::NSTreeHistorique(TWindow* parent, NSContexte* pCtx,
                                    int id, int x, int y, int w, int h,
                                    NSHISTODocument* pDocu,
                                    TModule* module, TStyle style)
                 :NSTreeWindow(parent, pCtx, id, x, y, w, h, style, module),
                  //
                  // patpatho globale de l'historique formée par les titres des documents
                  //
                  _NSPatPathoArray(pCtx->getSuperviseur())
{
try
{
  _pDoc = pDocu ;

  _sImportance = string("") ;
  _pBigBoss    = (NSSmallBrother*)  0 ;
  _ImagesHisto = (OWL::TImageList*) 0 ;

  NSTreeWindow* pNSTreeWindow = this ;

  _bVisibleManagementTrees = false ;

  skinName = string("historyWindow") ;     // nom de la fenetre de skin
  pNSTreeWindow->ReadOnly = true ;
}
catch (...)
{
  erreur("Exception NSTreeHistorique ctor.", standardError, 0) ;
}
}

//----------------------------------------------------------------------//			Destructeur
//----------------------------------------------------------------------
NSTreeHistorique::~NSTreeHistorique()
{
  string sMsg = string("Entering NSTreeHistorique destructor") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  if (_pBigBoss)
    delete _pBigBoss ;
  if (_ImagesHisto)
    delete _ImagesHisto ;
}

//---------------------------------------------------------------------------//  Description:	Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSTreeHistorique&
NSTreeHistorique::operator=(const NSTreeHistorique& src)
{
  if (this == &src)
    return *this ;

  _pDoc        = src._pDoc ;
  _sImportance = src._sImportance ;

  return *this ;
}

//---------------------------------------------------------------------------// SetupWindow
//---------------------------------------------------------------------------
void
NSTreeHistorique::SetupWindow()
{
try
{
  TTreeWindow::SetupWindow() ;

  skinSwitchOn(skinName) ;

  _ImagesHisto = new OWL::TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;

  HINSTANCE hInstModule = *GetApplication() ;

  //Intêret sur les pathos
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_0PLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_1PLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_2PLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_3PLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_4PLUS)) ;

  //CR fermé et ouvert
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_OUVERT)) ;
  //CS fermé et ouvert
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CONSULTATION)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_OUVERT)) ;
  //document sans bitmap
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_VIDE)) ;
  //HD CR et CS ouverts
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_HD)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_HD)) ;
  //image
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_FERME)) ;
  //texte
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_TLIB)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_INCLUS)) ;
  //CQ fermé et ouvert  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_OUVERT)) ;
  //Acrobat fermé et ouvert
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_OUVERT)) ;

  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_OUVERT_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CONSULTATION_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_OUVERT_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_VIDE_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_HD_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_HD_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_FERME_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_TLIB_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_INCLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_1)) ;  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_OUVERT_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_1)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_OUVERT_1)) ;

  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_OUVERT_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CONSULTATION_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_OUVERT_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_VIDE_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_HD_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_HD_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_FERME_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_TLIB_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_INCLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_2)) ;  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_OUVERT_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_2)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_OUVERT_2)) ;

  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_3)) ;  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_OUVERT_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CONSULTATION_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_OUVERT_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_VIDE_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_HD_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_HD_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_FERME_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_TLIB_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_INCLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_3)) ;  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_OUVERT_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_3)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_OUVERT_3)) ;

  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_OUVERT_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CONSULTATION_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_OUVERT_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_VIDE_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CR_HD_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CS_HD_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_IMAGE_FERME_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_TLIB_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_INCLUS)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_4)) ;  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_CQ_OUVERT_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_4)) ;
  _ImagesHisto->Add(OWL::TBitmap(hInstModule, IDB_AR_OUVERT_4)) ;

  SetImageList(TTreeWindow::Normal, *_ImagesHisto) ;

  #define IndexIcone_0plus     0
  #define IndexIcone_1plus     1
  #define IndexIcone_2plus     2
  #define IndexIcone_3plus     3
  #define IndexIcone_4plus     4

  #define IndexIcone_CR 		   5
  #define IndexIcone_CRouvert  6
  #define IndexIcone_CS 		   7
  #define IndexIcone_CSouvert  8

  #define IndexIcone_vide 	   9

  #define IndexIcone_CRcompo 	10
  #define IndexIcone_CScompo 	11

  #define IndexIcone_Image 	  12  #define IndexIcone_ImageFer 13

  #define IndexIcone_tlib 	  14
  #define IndexIcone_inclus 	15

  #define IndexIcone_CQ       16  #define IndexIcone_CQouvert 17  #define IndexIcone_AR       18  #define IndexIcone_ARouvert 19  #define IndexDecalage		    15
  AfficheDocument() ;
}
catch (...)
{
	erreur("Exception NSTreeHistorique::SetupWindow.", standardError, 0) ;
}
}

//------------------------------------------------------------------------//date de création d'un document (CR ou CS), pPat est sa patpatho
//------------------------------------------------------------------------
void
NSTreeHistorique::DateCreation(NSDocumentHisto* pNSDocumentHisto)
{
	if ((NSDocumentHisto*) NULL == pNSDocumentHisto)
		return ;

	if (string("ZCN00") == pNSDocumentHisto->getTypeSem())
	{		NSPatPathoArray Pat(pContexte->getSuperviseur()) ;
		pNSDocumentHisto->DonnePatPatho(&Pat, pContexte->getPatient()->getGraphPerson()) ;

    // on cherche d'abord dans la patpatho
    //
    string sDate = Pat.GetDocDate() ;

    if (string("") != sDate)
    	pNSDocumentHisto->setCreDate(sDate) ;
	}
}

//-----------------------------------------------------------------------// Afficher l'ensemble des documents visibles.
// Principe 	 :	1. trier les documents dans l'ordre de leurs créations
//					2. extraire la patpatho de chacun d'entre eux
//					3. former pNSPatPathoArray (patpatho de l'historique):
//					   contient les têtes des patpathos
//					4. dispatcher pNSPatPathoArray par NSTreeWindow
//-----------------------------------------------------------------------
void
NSTreeHistorique::AfficheDocument()
{
try
{
	_NodeArray.vider() ;

  uint iItemCount = GetItemCount() ;
  if (iItemCount > 0)
    DeleteAllItems() ;

	//ensemble des documents à afficher
	NSPatPathoArray PatAffiche(pContexte->getSuperviseur()) ;

	if (_pDoc->getVectDocument()->empty())
	{
    if (_pBigBoss)
      delete _pBigBoss ;

  	NSPatPathoArray PAT = PatAffiche ;
    _pBigBoss  = new NSSmallBrother(pContexte, &PatAffiche) ;
    //le patient lance l'historique
		_pBigBoss->lanceConsult("HPATI1", this) ;
    _NSPatPathoArray = PAT ;
    DispatcherPatPatho(&_NSPatPathoArray, 0, 0, "") ;

    if (false == _pBigBoss->getPatPatho()->empty())
			_pBigBoss->MetTitre() ;

    return ;
	}

	// on stocke l'élément 0 de chaque patpatho dans pPatAffiche
	DocumentIter iterDoc = _pDoc->getVectDocument()->begin() ;
	for ( ; _pDoc->getVectDocument()->end() != iterDoc ; iterDoc++)
	{
    if ((*iterDoc)->estVisible())
    {
      NSPatPathoArray IterPPT(pContexte->getSuperviseur()) ;
      (*iterDoc)->initFromPatPatho(&IterPPT) ;

		  if (false == IterPPT.empty())
		  {
			  PatPathoIter iter = IterPPT.begin() ;

        if (_bVisibleManagementTrees || (false == isManagementDoc((*iter)->getLexique())))
			    PatAffiche.push_back(new NSPatPathoInfo(*(*iter))) ;
		  }
		  else
		  {
    	  NSPatPathoInfo* pPpt = new NSPatPathoInfo() ;
        pPpt->setLexique((*iterDoc)->getContent()) ;
    	  PatAffiche.push_back(pPpt) ;
      }
		}
	}

	//adapter les lignes et colonnes des pathos de pPatAffiche
	PatPathoIter Iter = PatAffiche.begin() ;
	int ligne = 0 ;
	for ( ; PatAffiche.end() != Iter ; Iter++)
	{
		(*Iter)->setLigne(ligne) ;
    (*Iter)->setColonne(0) ;
		ligne++ ;
	}

  if (_pBigBoss)
    delete _pBigBoss ;

	// sauvegarder provisoirement
	NSPatPathoArray PAT = PatAffiche ;
	_pBigBoss = new NSSmallBrother(pContexte, &PatAffiche) ;
	//le patient lance l'historique
	_pBigBoss->lanceConsult("HPATI1", this) ;

	_NSPatPathoArray = PAT ;
	DispatcherPatPatho(&_NSPatPathoArray, 0, 0, "") ;

	if (false == _pBigBoss->getPatPatho()->empty())
		_pBigBoss->MetTitre() ;

	//
	// Pour chaque noeud supportant un document changer
	// son label (label + date de création)
  //
  // Warning: connection between a document and it's node in the patpatho is
  // based on the fact that the two lists are identically ordered
	//
	iterDoc = _pDoc->getVectDocument()->begin() ;
	iterNSTreeNode iterNode = _NodeArray.begin() ;

	for (; (_pDoc->getVectDocument()->end() != iterDoc) && (_NodeArray.end() != iterNode)
         				    ; iterDoc++)
	{
    bool bUseThisDoc = true ;

    NSPatPathoArray IterPPT(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&IterPPT) ;

    if ((false == _bVisibleManagementTrees) && (false == IterPPT.empty()))
		{
			PatPathoIter iter = IterPPT.begin() ;
      if (isManagementDoc((*iter)->getLexique()))
        bUseThisDoc = false ;
    }
    if (false == (*iterDoc)->estVisible())
      bUseThisDoc = false ;

    if (bUseThisDoc)
    {
		  (*iterNode)->SetDocument(static_cast<void*>((*iterDoc))) ;
		  //
		  // Ouvrir automatiquement la fiche de synthése : changer la bitmap
		  //
      if (string("ZSYNT") == (*iterDoc)->getSemCont())
			  AfficheIcone(IndexIcone_CSouvert, (*iterNode)) ;
		  else
		  {
      	if      (string("ZCN00") == (*iterDoc)->getTypeSem())
        	AfficheIcone(IndexIcone_CR, (*iterNode)) ;
        else if (string("ZCS00") == (*iterDoc)->getTypeSem())
        	AfficheIcone(IndexIcone_CS, (*iterNode)) ;
        else if (string("ZCQ00") == (*iterDoc)->getTypeSem())
        	AfficheIcone(IndexIcone_CQ, (*iterNode)) ;
        else if (string("ZTPDF") == (*iterDoc)->getTypeSem())
        	AfficheIcone(IndexIcone_AR, (*iterNode)) ;
        else if (pContexte->typeDocument((*iterDoc)->getTypeSem(), NSSuper::isImage))
        	AfficheIcone(IndexIcone_Image, (*iterNode)) ;
        else if (pContexte->typeDocument((*iterDoc)->getTypeSem(), NSSuper::isText))
        	AfficheIcone(IndexIcone_tlib, (*iterNode)) ;
        else
        	AfficheIcone(IndexIcone_vide, (*iterNode)) ;
		  }
		  SetLabelNode(*iterNode, *iterDoc) ;

      iterNode++ ;
    }
	}
}
catch (...)
{
	erreur("Exception NSTreeHistorique::AfficheDocument.", standardError, 0) ;
}
}

//--------------------------------------------------------------------------// Ajoute un document à l'historique, deux cas :
//	1:	il sera placé selon sa date de création.
//		S'il est le plus récent , c'est le Root qui le crée en tant que fils
//		sinon c'est son frère dont la date de création est la plus proche qui le crée
//		en tant que frère.
//	2: s'il est composé, il remplace son document brut.
//
//  Note : Dans cette fonction, le document est déjà ouvert, donc
//		   on met à jour l'historique
//--------------------------------------------------------------------------
void
NSTreeHistorique::AjouteDocument(NSDocumentInfo* pNSDocumentInfo,
                                 const NSPatPathoArray* pNSPatPath,
                                 NSNoyauDocument* pNouveauDocument)
{
try
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

	// Vérifier que ce document n'existe pas dans l'historique
  //
  string codeDocBrut = pNSDocumentInfo->getID() ;
  string typeDocBrut = string("") ;

  iterNSTreeNode iterNode = TrouveNoeud(codeDocBrut) ;
  if (_NodeArray.end() != iterNode)
  {
  	NSDocumentHisto* pDocHisto = static_cast<NSDocumentHisto*>((*iterNode)->getDocument()) ;
    if ((NSDocumentHisto*) NULL == pDocHisto)
    	return ;

    NSDocumentData InfoData ;
    pNSDocumentInfo->initFromData(&InfoData) ;
    pDocHisto->setData(&InfoData) ;

    return ;
	}

	// on crée un nouveau DocumentHisto avec sa patpatho
  NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(pNSDocumentInfo) ;
  if (pNSPatPath)
    pNSDocumentHisto->setPatPatho(pNSPatPath) ;
  else //document type image, HTML qui n'ont pas de patpatho
  {
    NSPatPathoArray PPT(pContexte->getSuperviseur()) ;
  	pNSDocumentHisto->DonnePatPatho(&PPT, pContexte->getPatient()->getGraphPerson()) ;
    pNSDocumentHisto->setPatPatho(&PPT) ;
  }

  //
  // Cas où pNSDocumentHisto est composé, il remplace son document brut
  //
/*
  if (string("HD") == string(pNSDocumentInfo->getType(), 0, 2))
  {
  	NSDocumentInfo* pDocHtml = 0 ;
    NSDocumentInfo* pDocument = new NSDocumentInfo(*pNSDocumentInfo) ;

    // on retrouve le document brut
    if (!pContexte->ChercheComposition(&pDocument, &pDocHtml))
    {
      string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
      sErrorText += string(" (") + pDocument->getPatient() + string(" ") + pDocument->getDocument() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

      delete pDocument ;
      return ;
    }

    codeDocBrut = pDocument->getID() ;
    typeDocBrut = pDocument->getType() ;

    if (NULL != pNouveauDocument)
    {
    	// Trouver le noeud correspondant au document brut
      //
      iterNSTreeNode iterNode = _NodeArray.begin() ;
      iterNode = TrouveNoeud(codeDocBrut) ;
      if (iterNode != _NodeArray.end())
      {
      	(*iterNode)->pDocument = static_cast<void*>(pNSDocumentHisto) ;
        if      (string("ZCN00") == pDocument->getType())
        	AfficheIcone(IndexIcone_CRouvert, (*iterNode));
        else if (string("ZCS00") == pDocument->getType())
        	AfficheIcone(IndexIcone_CSouvert, (*iterNode));
        else if (string("ZCQ00") == pDocument->getType())        	AfficheIcone(IndexIcone_CQouvert, (*iterNode));
        else if (string("ZTPDF") == pDocument->getType())
        	AfficheIcone(IndexIcone_ARouvert, (*iterNode));

        // on déclare ouvert le nouveau document HDHTM
        // qui a remplacé le document brut dans pNodeArray
        // (on ferme le document brut devenu invisible)
        pDoc->FermeDocumentOuvert(pDocument) ;
        pDoc->RangeDocumentOuvert(pDocHtml, pNouveauDocument) ;
      }
      else
      {   // Attention ne pas faire le delete pNSDocumentHisto dans l'autre cas
      	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
        sErrorText += string(" (") + pNSDocumentHisto->getPatient() + string(" ") + pNSDocumentHisto->getDocument() + string(")") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
        erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
        delete pNSDocumentHisto ;
      }

      delete pDocument ;
      delete pDocHtml ;
      return ;
    }
  }
*/

	// Ancienne méthode d'Hafedh qui écrase la date de creation des CN
	// DateCreation(pNSDocumentHisto);

  string sAffiche = pNSDocumentHisto->getDocName() ;
  strip(sAffiche, stripBoth) ;
  if (string("") != pNSDocumentHisto->getDateDoc())
  {
  	string sLang = pContexte->getUserLanguage() ;
    sAffiche += string(" - ") + donne_date(pNSDocumentHisto->getDateDoc(), sLang) ;
  }
  //
  // insertion de ce nouveau document: pNSDocumentHisto sera placé juste après
  // le premier document (s'il existe) le plus recent par rapport à lui , sinon
  // il sera creé par le Root à la tête de l'historique
  //
  iterNSTreeNode iterGrandFrere = NULL ;
  int ligne   = -1 ;
  // char dateAfficheNoeud[20] ;

  bool bOk = false ;

  if (false == _NodeArray.empty())
  {
    iterNode = _NodeArray.begin() ;

    string sDateInfo = pNSDocumentHisto->getDateDoc() ;
    for ( ; _NodeArray.end() != iterNode ; iterNode++)
    {
      int Ligne = (*iterNode)->getLigne() ;

		  NSDocumentHisto* pDocument = static_cast<NSDocumentHisto*>((*iterNode)->getDocument()) ;
      if (pDocument)
      {
    	  string sDateNode = pDocument->getDateDoc() ;
        if ((sDateNode > sDateInfo) && (Ligne > ligne))
        {
      	  iterGrandFrere = iterNode ;
          ligne = (*iterGrandFrere)->getLigne() ;
          bOk = true ;
        }
      }
    }
  }

  OWL::TTreeNode root = GetRoot() ;
  NSTreeNode* pNewNSTreeNode = (NSTreeNode*) 0 ;
  //
  // Il existe un document plus récent, c'est lui qui crée le nouveau
  //
  if (bOk)
  {
  	int colonne = 0 ;
    // Coordonnées du dernier petit fils(s'il existe) de iterGrandFrere
    GetMaxCoordonnees(*iterGrandFrere, &ligne, &colonne) ;

    // Décaler les lignes des autres noeuds d'une unité
    iterNSTreeNode iterN = _NodeArray.begin() ;
    for( ; _NodeArray.end() != iterN ; iterN++)
    	if ((*iterN)->getLigne() > ligne)
      	(*iterN)->setLigne((*iterN)->getLigne() + 1) ;

    // pNewNSTreeNode = new NSTreeNode((*iterGrandFrere)->InsertItem(TTreeNode(*this, "")), pContexte) ;

    OWL::TTreeNode treeNode = (*iterGrandFrere)->InsertItem(OWL::TTreeNode(*this, "")) ;
    if (NULL != treeNode.GetHTreeItem())
      pNewNSTreeNode = new NSTreeNode(treeNode, pContexte) ;

    ligne++ ;
  }
  //
  // Il n'existe pas de document plus récent, c'est le Root qui crée
  //
  else
  {
  	// Décaler les lignes des autres noeuds d'une unité
    if (false == _NodeArray.empty())
    {
      iterNSTreeNode iterN = _NodeArray.begin() ;
      for ( ; _NodeArray.end() != iterN ; iterN++)
    	  (*iterN)->setLigne((*iterN)->getLigne() + 1) ;
    }

    ligne = 1 ;
    OWL::TTreeNode* pAmorce = new OWL::TTreeNode(*this, TVI_FIRST) ;
    *pAmorce = root.InsertChild(*pAmorce, OWL::TTreeNode::First) ;
    pNewNSTreeNode = new NSTreeNode(*pAmorce, pContexte) ;
    delete pAmorce ;
  }
  //
  // Initialisation du nouveau noeud
  //
  pNewNSTreeNode->SetFatherNode((NSTreeNode*) 0) ;
  pNewNSTreeNode->SetPilotNode((NSTreeNode*) 0) ;

  // Declaration d'ouverture du document
  // (dans le cas d'un nouveau document, celui-ci est forcément ouvert à l'écran,
  //  et on doit le signaler à l'historique en appelant RangeDocumentOuvert
  //  sinon on ne doit pas le déclarer ouvert ici - c'est géré ailleurs -)
  if (pNouveauDocument)
  	_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pNouveauDocument) ;

  NSTlibre* pNSTlibre = 0 ;
  bool texteRecupere ;
  int tailleVecteur = 0 ;

  NSPatPathoArray DocPPT(pContexte->getSuperviseur()) ;
  pNSDocumentHisto->initFromPatPatho(&DocPPT) ;

  if (false == DocPPT.empty())
  {
    PatPathoIter iter = DocPPT.begin() ;

  	string sTexteLibre = (*iter)->getTexteLibre() ;
    string sIdentite   = (*iter)->getLexique() ;
    /* bool bTexte = */ pNewNSTreeNode->MettreAjourLabel(pNSTlibre, &texteRecupere, &tailleVecteur,
                                ligne, 0, sTexteLibre, "", sIdentite, this, sLang) ;
	}
  else
    // When a patpatho is involved (above), it is done in MettreAjourLabel/CreerNSTreeNode
    //
    _NodeArray.AddElement(pNewNSTreeNode) ;

  pNewNSTreeNode->SetDocument(static_cast<void*>(pNSDocumentHisto)) ;

  pNewNSTreeNode->SetLabelString(sAffiche) ;
  pNewNSTreeNode->SetText(sAffiche.c_str(), true) ;

  // Mise en place des icones
  //
  string sTypeDoc = pNSDocumentInfo->getTypeSem() ;

  if (sTypeDoc == "ZCN00")
  {
  	if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
    	AfficheIcone(IndexIcone_CRouvert, pNewNSTreeNode) ;
    else
    	AfficheIcone(IndexIcone_CR, pNewNSTreeNode) ;
	}
  else if (sTypeDoc == "ZCS00")
  {
    if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
    	AfficheIcone(IndexIcone_CSouvert, pNewNSTreeNode) ;
    else
    	AfficheIcone(IndexIcone_CS, pNewNSTreeNode) ;
	}
  else if (sTypeDoc == "ZCQ00")
  {
    if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
      AfficheIcone(IndexIcone_CQouvert, pNewNSTreeNode) ;
    else
      AfficheIcone(IndexIcone_CQ, pNewNSTreeNode) ;
	}
  else if (sTypeDoc == "ZTPDF")
  {
    if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
      AfficheIcone(IndexIcone_ARouvert, pNewNSTreeNode) ;
    else
      AfficheIcone(IndexIcone_AR, pNewNSTreeNode) ;
	}
  else if (sTypeDoc == "ZDHTM")
  {
  	if (typeDocBrut == string("ZCN00"))
    {
    	if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
      	AfficheIcone(IndexIcone_CRouvert, pNewNSTreeNode) ;
      else
      	AfficheIcone(IndexIcone_CRcompo, pNewNSTreeNode) ;
    }
    else if (typeDocBrut == string("ZCS00"))
    {
    	if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
      	AfficheIcone(IndexIcone_CSouvert, pNewNSTreeNode) ;
      else
      	AfficheIcone(IndexIcone_CScompo, pNewNSTreeNode) ;
    }
  }
	else if (pContexte->typeDocument(sTypeDoc, NSSuper::isImage))
	{
  	if (_pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
    	AfficheIcone(IndexIcone_ImageFer, pNewNSTreeNode) ;
    else
    	AfficheIcone(IndexIcone_Image, pNewNSTreeNode) ;
	}
	else if (pContexte->typeDocument(sTypeDoc, NSSuper::isText))
	{
  	AfficheIcone(IndexIcone_tlib, pNewNSTreeNode) ;
	}
	else
    AfficheIcone(IndexIcone_vide, pNewNSTreeNode) ;

	//delete pNSTlibre;
	_pDoc->getVectDocument()->push_back(pNSDocumentHisto) ;
}
catch (...)
{
	erreur("Exception NSTreeHistorique::AjouteDocument.", standardError, 0) ;
}
}

//--------------------------------------------------------------------------// Enlever le document pNSDocumentHisto de l'historique
//--------------------------------------------------------------------------
void
NSTreeHistorique::EnleverDocument(NSDocumentInfo* pNSDocumentHisto)
{
	if (false == _NodeArray.empty())
	{
    iterNSTreeNode iterNode = _NodeArray.begin() ;
    string codeDocBrut = pNSDocumentHisto->getID() ;
    iterNode = TrouveNoeud(codeDocBrut) ;
    if (_NodeArray.end() != iterNode)
    {
      int ligne  	 = (*iterNode)->getLigne() ;
      int cardinal = 1 ; //nb fils et petits fils

      // on remove le node de la TreeView
      (*iterNode)->Delete() ;
      // on détruit ses fils
      DetruitFils(&cardinal, *iterNode) ;

      // on enleve le node de l'Array de son père
      if ((*iterNode)->getFatherNode())
        (*iterNode)->getFatherNode()->getVectSons()->erase(iterNode) ;

      delete (*iterNode) ;
      // enlever le node (et ses frères liés ) de pNodeArray
      _NodeArray.erase(iterNode) ;

      //mettre à jour les coordonnées des autres NSTreeNodes
      for (iterNSTreeNode iter = _NodeArray.begin() ; _NodeArray.end() != iter ; iter++)
        if ((*iter)->getLigne() > ligne)
            (*iter)->setLigne((*iter)->getLigne() - cardinal ) ;
    }
	}

	// on enleve le document de l'array des documents ouverts
	if (_pDoc->EstUnDocumentOuvert(pNSDocumentHisto))
		_pDoc->FermeDocumentOuvert(pNSDocumentHisto) ;
}

//-------------------------------------------------------------------------//changer la BITmap d'un document qui vient d'être fermé
//-------------------------------------------------------------------------
void
NSTreeHistorique::FermetureDocument(NSDocumentInfo* pDocumentInfo)
{
  if ((false == _NodeArray.empty()) && pDocumentInfo)
  {
  	string codeDocBrut = pDocumentInfo->getID() ;
    iterNSTreeNode iterNode = TrouveNoeud(codeDocBrut) ;
    if (_NodeArray.end() != iterNode)
    {
    	NSDocumentHisto* pDocument = static_cast<NSDocumentHisto*>((*iterNode)->getDocument()) ;
/*
      if (string("ZDHTM") == pDocumentInfo->getType())
      {
      	NSDocumentInfo* pDocHtml = 0 ;
        NSDocumentInfo* pDocum = new NSDocumentInfo(*pDocumentInfo) ;
        // on retrouve le document brut
        if (!pContexte->ChercheComposition(&pDocum, &pDocHtml))
        {
        	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
    			sErrorText += string(" (") + pDocument->getPatient() + string(" ") + pDocument->getDocument() + string(")") ;
    			pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    			erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
          delete pDocum ;
          return ;
        }

        if      (string("ZCN00") == pDocum->getType())
        	AfficheIcone(IndexIcone_CRcompo, *iterNode);
        else if (string("ZCS00") == pDocum->getType())
        	AfficheIcone(IndexIcone_CScompo, *iterNode);
        delete pDocum;
      	delete pDocHtml;
      }
      else
      {
*/
      	if      (string("ZCN00") == pDocument->getTypeSem())
        	AfficheIcone(IndexIcone_CR, *iterNode) ;
        else if (string("ZCS00") == pDocument->getTypeSem())
        	AfficheIcone(IndexIcone_CS, *iterNode) ;
        else if (string("ZCQ00") == pDocument->getTypeSem())        	AfficheIcone(IndexIcone_CQ, *iterNode) ;
        else if (string("ZTPDF") == pDocument->getTypeSem())
        	AfficheIcone(IndexIcone_AR, *iterNode) ;
        else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isImage, false))
        	AfficheIcone(IndexIcone_Image, *iterNode) ;
        else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isText, false))
        	AfficheIcone(IndexIcone_tlib, *iterNode) ;
        else
        	AfficheIcone(IndexIcone_vide, *iterNode) ;
//      }
    }
  }
  _pDoc->FermeDocumentOuvert(pDocumentInfo) ;
}

//-------------------------------------------------------------------------//  Visualisation d'un document : mise en avant s'il est déjà ouvert,
//                                ouverture sinon//-------------------------------------------------------------------------
void
NSTreeHistorique::VisualiserDocument(NSTreeNode* pNSDocNode)
{
	if ((NSTreeNode*) NULL == pNSDocNode)
		return ;

	NSDocumentHisto* pDocument = getDocumentForNode(pNSDocNode) ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

	//
	// Si le document est déjà ouvert, on le met en premier plan
	//
	if (_pDoc->EstUnDocumentOuvert(pDocument))
	{
  	_pDoc->ActiveFenetre(pDocument) ;
    return ;
	}

	if (!(pContexte->userHasPrivilege(NSContexte::openDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL)))
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
    erreur(sErrorText.c_str(), warningError, 0) ;
    return ;
	}

	//
	// S'il n'était pas déjà ouvert, on l'ouvre
	//
	HCursor = ::SetCursor(::LoadCursor(0, IDC_WAIT)) ;

	/******************* Affichage de l'icône correspondant au document ouvert ***********/

	//	// 1. Cas des documents composés (HTML dynamique)
	//
/*
	if (string("ZDHTM") == pDocument->getType())
	{
  	NSDocumentInfo* pDocHtml = 0 ;
    NSDocumentInfo* pDocum = new NSDocumentInfo(pContexte) ;
    *(pDocum->getData()) = *(pDocument->getData()) ;

    //    // On retrouve le document brut
    //
    if (!pContexte->ChercheComposition(&pDocum, &pDocHtml))
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
      sErrorText += string(" (") + pDocument->getPatient() + string(" ") + pDocument->getDocument() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

      delete pDocum ;
      return ;
    }

    //    // Sélection de l'icone
    //
    if      (string("ZCN00") == pDocum->getType())
    	AfficheIcone(IndexIcone_CRouvert, pNSDocNode);
    else if (string("ZCS00") == pDocum->getType())
    	AfficheIcone(IndexIcone_CSouvert, pNSDocNode);

    delete pDocum ;    delete pDocHtml ;
	}
  //
  // 2. cas des documents non composés (bruts)
  //
  else
  {
*/
  	//
    // Sélection de l'icone
    //
    if      (string("ZCN00") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_CRouvert, pNSDocNode) ;
    else if (string("ZCS00") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_CSouvert, pNSDocNode) ;
    else if (string("ZCQ00") == pDocument->getTypeSem())    	AfficheIcone(IndexIcone_CQouvert, pNSDocNode) ;
    else if (string("ZTPDF") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_ARouvert, pNSDocNode) ;
    else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isImage))
    	AfficheIcone(IndexIcone_ImageFer, pNSDocNode) ;
    else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isText))
    	AfficheIcone(IndexIcone_tlib, pNSDocNode) ;
    else
    	AfficheIcone(IndexIcone_vide, pNSDocNode) ;
//  }

  /************************* Ouverture effective du document (par NSSuper) ************/
  //
  // Double affectation utile dans le cas d'un document Hd
  // sinon pDocum sera transformé en document brut
  //
  NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

  // Ouverture en lecture seule
  NSNoyauDocument* pDocNoy = pContexte->getSuperviseur()->OuvreDocument(Docum, pContexte, true) ;
  if (pDocNoy)
  {
  	// on reprend les donnees pour éviter l'effet de bord de OuvreDocument
    // (switch entre document HD et document brut CN)
    //
    pDocument->initFromData(&InfoData) ;
    Docum.setData(&InfoData) ;

    // on range le pHtmlInfo dans le cas d'un HD
    // (permet de savoir que le document est composé)
    _pDoc->RangeDocumentOuvert(&Docum, pDocNoy) ;
  }
  HCursor = ::SetCursor(::LoadCursor(0, IDC_ARROW)) ;
}

//-------------------------------------------------------------------------
// Rafraîchir l'historique en tenant compte des changements (exemple patpatho)
// de pNSDocumentHisto
//-------------------------------------------------------------------------
void
NSTreeHistorique::VisualiserPatho(NSDocumentInfo* pDocumentInfo, const NSPatPathoArray* pNSPatPathoArray)
{
	if ((NSDocumentInfo*) NULL == pDocumentInfo)
		return ;

	string codeDocBrut = pDocumentInfo->getID() ;
  DocumentIter DocIt = _pDoc->getVectDocument()->TrouveDocHisto(codeDocBrut) ;
  if (NULL == DocIt)
    return ;

  NSDocumentHisto* pDocumentHisto = *DocIt ;

	iterNSTreeNode pNSTreeNodeSelection = TrouveNoeud(codeDocBrut) ;
	if ((_NodeArray.end() == pNSTreeNodeSelection) || (NULL == pNSTreeNodeSelection))
		pNSTreeNodeSelection = 0 ;

	// pDocumentHisto = static_cast<NSDocumentHisto*>((*pNSTreeNodeSelection)->pDocument) ;

/* Now done by NSHISTODocument::Rafraichir since even non visible documents
   can be updated

  // Updating NSDocumentInfo
  if (pDocumentInfo)
  {
  	*(pDocumentHisto->getData()) = *(pDocumentInfo->getData()) ;

    // Updating presentation information
    if (pDocumentInfo->pPres && (false == pDocumentInfo->pPres->empty()))
  	  *(pDocumentHisto->pPres) = *(pDocumentInfo->pPres) ;

    // Updating meta data
    if (pDocumentInfo->pMeta && (false == pDocumentInfo->pMeta->empty()))
  	  *(pDocumentHisto->pMeta) = *(pDocumentInfo->pMeta) ;
  }

  // mise à jour de la patpatho
  if (NULL != pNSPatPathoArray)
  {
  	pDocumentHisto->pPatPathoArray->vider() ;
    *(pDocumentHisto->pPatPathoArray) = *pNSPatPathoArray ;
  }
  pDocumentHisto->TrouveDateDoc() ;
*/

  // on remet à jour le titre et la bitmap d'intérêt
  if (pNSTreeNodeSelection)
    SetLabelNode(*pNSTreeNodeSelection, pDocumentHisto) ;

  //Interet(*pNSTreeNodeSelection);

	//provisoirement pour CS
  if (pNSTreeNodeSelection && (string("ZCS00") == pDocumentInfo->getTypeSem()))
	{
  	if (pNSPatPathoArray)
		{
    	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;
      int DecalageLigneFils = 0 ;
      int ligne  	 = (*pNSTreeNodeSelection)->getLigne() ;
      int cardinal = 0 ; //nb fils et petits fils
      //supprimer l'ancienne patpatho si elle existe
      bool PatpthoNonVide = true ;

      SupprimerFils((*pNSTreeNodeSelection), &cardinal, &PatpthoNonVide) ;
      //mettre à jour les coordonnées des autres NSTreeNodes
      iterNSTreeNode iter = _NodeArray.begin() ;
      if (cardinal)
      {
      	for ( ; _NodeArray.end() != iter ; iter++)
        	if ((*iter)->getLigne() > ligne)
          	(*iter)->setLigne((*iter)->getLigne() - cardinal ) ;
      }

      NSPatPathoArray PptHisto(pContexte->getSuperviseur()) ;
      pDocumentHisto->initFromPatPatho(&PptHisto) ;

      PptHisto.donnePatpatho(&PatPatho, _sImportance) ;
      if (false == PatPatho.empty())
      {
      	DispatcherPatPatho(&PatPatho, (*pNSTreeNodeSelection), &DecalageLigneFils, "") ;

        // Mettre à jour les coordonnées des autres NSTreeNodes
        for ( ; _NodeArray.end() != iter ; iter++)
        	if (((*iter)->getLigne() > ligne) && ((!(*iter)->Descendant((*pNSTreeNodeSelection)))))
          	(*iter)->setLigne((*iter)->getLigne() + DecalageLigneFils ) ;
        //
        // Affiche les bitmaps correspondant aux différents intêrets
        // des fils de pNSTreeNodeSelection
        //
        AfficheInteret(*pNSTreeNodeSelection) ;
        (*pNSTreeNodeSelection)->ExpandItem(TVE_EXPAND) ;
      }
    }
  }
}

//--------------------------------------------------------------------------//affiche le bitmaps correspondant à l'intêrets de pNSTreeNodeSelection
//--------------------------------------------------------------------------
void
NSTreeHistorique::Interet(NSTreeNode* pNSTreeNode)
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	//ne pas changer la bitmap du noeud sur le document lui même
  if (pNSTreeNode->getColonne() == 1)
  	return ;

	string sInteret = pNSTreeNode->getInteret() ;
	if      (sInteret == string("A"))
	{
  	pNSTreeNode->SetImageIndex(0) ;
    pNSTreeNode->SetSelectedImageIndex(0, true) ;
	}
  else if (sInteret == string("B"))
  {
  	pNSTreeNode->SetImageIndex(1) ;
    pNSTreeNode->SetSelectedImageIndex(1, true) ;
  }
  else if (sInteret == string("C"))
  {
  	pNSTreeNode->SetImageIndex(2) ;
    pNSTreeNode->SetSelectedImageIndex(2, true) ;
  }
  else if (sInteret == string("D"))
  {
  	pNSTreeNode->SetImageIndex(3) ;
    pNSTreeNode->SetSelectedImageIndex(3, true) ;
	}
  else if (sInteret == string("E"))
  {
  	pNSTreeNode->SetImageIndex(4) ;
    pNSTreeNode->SetSelectedImageIndex(4, true) ;
	}
}

//--------------------------------------------------------------------------//affiche les bitmaps correspondant aux différents intêrets des fils
//de pNSTreeNodeSelection
//--------------------------------------------------------------------------
void
NSTreeHistorique::AfficheInteret(NSTreeNode* pNSTreeNodePere)
{
	if ((NSTreeNode*) NULL == pNSTreeNodePere)
		return ;

	if (false == pNSTreeNodePere->getVectSons()->empty())
  {
		iterNSTreeNode fils = pNSTreeNodePere->getVectSons()->begin() ;
		for ( ; pNSTreeNodePere->getVectSons()->end() != fils ; fils++)
			AfficheInteret(*fils) ;
	}

	Interet(pNSTreeNodePere) ;
}

//--------------------------------------------------------------------------// Fonction de NSTreeWindow surchargée
// pour éviter que les documents de l'historique (colonne == 1)
// affichent l'intérêt max à la place de leur type
//--------------------------------------------------------------------------
void
NSTreeHistorique::AfficheInteretMaximum(NSTreeNode* pNSTreeNode)
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	if (pNSTreeNode->getColonne() == 1)
  	return ;

  NSTreeWindow::AfficheInteretMaximum(pNSTreeNode) ;
}

//--------------------------------------------------------------------------// Fonction de NSTreeWindow surchargée
// pour éviter que les documents de l'historique (colonne == 1)
// affichent l'intérêt réel à la place de leur type
//--------------------------------------------------------------------------
void
NSTreeHistorique::AfficheInteretReel(NSTreeNode* pNSTreeNode)
{
	if ((NSTreeNode*) NULL == pNSTreeNode)
		return ;

	if (pNSTreeNode->getColonne() > 1)
		NSTreeWindow::AfficheInteretReel(pNSTreeNode) ;
}

NSTreeNode*
NSTreeHistorique::getNodeForPoint(NS_CLASSLIB::TPoint point)
{
	pHitTestInfo->pt = point ;
	return GetNSTreeNode(HitTest(pHitTestInfo)) ;
}

NSTreeNode*
NSTreeHistorique::getSelectedNode()
{
	OWL::TTreeNode node = GetSelection() ;
  return GetNSTreeNode(&node) ;
}

NSDocumentHisto*
NSTreeHistorique::getDocumentForNode(NSTreeNode* pNSTreeNode)
{
	if (((NSTreeNode*) NULL == pNSTreeNode) || (false == pNSTreeNode->estUnDocument()))
		return (NSDocumentHisto*) 0 ;

	NSDocumentHisto* pDocument = static_cast<NSDocumentHisto*>(pNSTreeNode->getDocument()) ;

	return pDocument ;
}

NSDocumentHisto*
NSTreeHistorique::getSelectedDocument()
{
	NSTreeNode* pNSTreeNodeSelection = GetNSTreeNode(HitTest(pHitTestInfo)) ;

	return getDocumentForNode(pNSTreeNodeSelection) ;
}

//-------------------------------------------------------------------------// Remet à jour le titre du node en fonction des DocumentHisto associées
//-------------------------------------------------------------------------
void
NSTreeHistorique::SetLabelNode(NSTreeNode* pNSTreeNode, NSDocumentHisto* pDocHisto)
{
  if (((NSTreeNode*) NULL == pNSTreeNode) || ((NSDocumentHisto*) NULL == pDocHisto))
    return ;

  string sAffiche = pDocHisto->getDocName() ;

	strip(sAffiche, stripBoth) ;

	// if (strcmp(chAffiche, "Synthèse"))
  if (string("ZSYNT") != pDocHisto->getSemCont())
	{
		if (string("") != pDocHisto->getDateDoc())
    {
    	string sLang = pContexte->getUserLanguage() ;
      sAffiche += string(" - ") + donne_date(pDocHisto->getDateDoc(), sLang) ;
    }
	}

	pNSTreeNode->SetLabelString(sAffiche) ;
	pNSTreeNode->SetText(sAffiche.c_str(), true) ;
}

//-------------------------------------------------------------------------// Affiche les patpathos des documents selon leurs importances :
// ON SE LIMITE POUR L'INSTANT AUX DOCUMENTS TYPE CONSULTATION OU CR
//-------------------------------------------------------------------------
void
NSTreeHistorique::AffichePatho()
{
	if (_NodeArray.empty())
  	return ;

	int	iDocEnCours = 1 ;
  bool bTourner = true ;
	while (bTourner)
	{
  	int  iNDoc 	 = 0 ;
    bool bTraite = false ;

    iterNSTreeNode iterNode = _NodeArray.begin() ;
    for ( ; (_NodeArray.end() != iterNode) && (false == bTraite) ; iterNode++)
    {
    	if (((*iterNode)->getColonne()) == NODES_COL_BASE)
      	iNDoc++ ;
      if (iNDoc == iDocEnCours)
      {
      	NSDocumentHisto* pDocumentHisto = static_cast<NSDocumentHisto*>((*iterNode)->getDocument()) ;
        if (pDocumentHisto && isDisplayableDoc(pDocumentHisto))
        {
          // Initialization of common params
          //
          NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

          int DecalageLigneFils = 0 ;
          int ligne  	 = (*iterNode)->getLigne() ;
          int cardinal = 0 ; //nb fils et petits fils

          int iOpenedIcon  = 0 ;
          int iRegularIcon = 0 ;

          // Delete previous patpatho if present
          //
          bool bPatpthoNonVide = true ;
          SupprimerFils((*iterNode), &cardinal, &bPatpthoNonVide) ;

          // Update other NSTreeNodes coordinates if needed
          //
          iterNSTreeNode iter = _NodeArray.begin() ;
          if (cardinal > 0)
          {
          	for ( ; _NodeArray.end() != iter ; iter++)
            {
              int iNodeLine = (*iter)->getLigne() ;
            	if (iNodeLine > ligne)
              	(*iter)->setLigne(iNodeLine - cardinal) ;
            }
          }

          // Get patpatho for given "interest" level
          //
          if (isCsDoc(pDocumentHisto))
          {
            NSPatPathoArray PptHisto(pContexte->getSuperviseur()) ;
            pDocumentHisto->initFromPatPatho(&PptHisto) ;

            PptHisto.donnePatpatho(&PatPatho, _sImportance) ;

            iOpenedIcon  = IndexIcone_CSouvert ;
            iRegularIcon = IndexIcone_CS ;
          }
          else if (isCrDoc(pDocumentHisto))
          {
            GetDisplayablePathoForCr(pDocumentHisto, _sImportance, &PatPatho) ;

            iOpenedIcon  = IndexIcone_CRouvert ;
            iRegularIcon = IndexIcone_CR ;
          }

          if (false == PatPatho.empty())
          {
          	iterNSTreeNode itertransit = iterNode ;
            DispatcherPatPatho(&PatPatho, (*iterNode), &DecalageLigneFils, "") ;
            iterNode = itertransit ;

            //
            // Mettre à jour les coordonnées des autres NSTreeNodes
            //
            iterNSTreeNode itersuite ;
            if (cardinal)
            	itersuite = iter ;
            else
            	itersuite = _NodeArray.begin() ;

            for ( ; _NodeArray.end() != itersuite ; itersuite++)
            {
              int iLine = (*itersuite)->getLigne() ;
            	if ((iLine > ligne) &&
                        		(false == (*itersuite)->Descendant(*iterNode)))
              	(*itersuite)->setLigne(iLine + DecalageLigneFils) ;
            }
            //
            // Affiche les bitmaps correspondant aux différents intêrets
            // des fils de pNSTreeNodeSelection
            //
            AfficheInteret(*iterNode) ;
            (*iterNode)->ExpandItem(TVE_EXPAND) ;

            // On rétablit l'icône du node père (CS)
            if (_pDoc->EstUnDocumentOuvert(*iterNode))
            	AfficheIcone(iOpenedIcon, *iterNode) ;
            else
            	AfficheIcone(iRegularIcon, *iterNode) ;
          }
        }
        iDocEnCours++ ;
        bTraite = true ;
      }
    }

    if (_NodeArray.end() == iterNode)
    	bTourner = false ;
  }
}

bool
NSTreeHistorique::isDisplayableDoc(NSDocumentHisto* pDocumentHisto)
{
  return (isCsDoc(pDocumentHisto) || isCrDoc(pDocumentHisto)) ;
}

bool
NSTreeHistorique::isCsDoc(NSDocumentHisto* pDocumentHisto)
{
  if ((NSDocumentHisto*) NULL == pDocumentHisto)
    return false ;

  return ((string("ZCS00") == pDocumentHisto->getTypeSem()) ||
          (string("ZCQ00") == pDocumentHisto->getTypeSem()) ||
          (string("ZQQOL") == pDocumentHisto->getTypeSem())) ;
}

bool
NSTreeHistorique::isCrDoc(NSDocumentHisto* pDocumentHisto)
{
  if ((NSDocumentHisto*) NULL == pDocumentHisto)
    return false ;

  return ((string("ZCN00") == pDocumentHisto->getTypeSem()) ||
          (string("ZDHTM") == pDocumentHisto->getTypeSem())) ;
}

//
// Cas particulier pour les comptes rendus : on donne à la
// conclusion une valeur Importance document + 2
//
void
NSTreeHistorique::GetDisplayablePathoForCr(NSDocumentHisto* pDocumentHisto, string sImportance, NSPatPathoArray* pPatPatho)
{
  if ((NSPatPathoArray*) NULL == pPatPatho)
    return ;

  pPatPatho->vider() ;

  if ((NSDocumentHisto*) NULL == pDocumentHisto)
    return ;

  NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
  pDocumentHisto->initFromPatPatho(&DocPatho) ;

  DocPatho.donnePatpatho(pPatPatho, sImportance) ;

  // If patpatho already has interest information, then don't artificially
  // apply the document+2 rule to the conclusion
  //
  if (false == pPatPatho->empty())
    return ;

  string sDocInteret = pDocumentHisto->getInteret() ;
  char importDoc  = sDocInteret[0] ;
  char importConc = char(importDoc + 2) ;

  // Interest is still not high enough
  //
  if (importConc < sImportance[0])
    return ;

  PatPathoIter iterPpt = DocPatho.begin() ;
  for ( ; (DocPatho.end() != iterPpt) &&
                  ((*iterPpt)->getLexique() != "0CONC1") ;
                                        iterPpt++) ;

  // There is no conclusion
  //
  if (DocPatho.end() == iterPpt)
    return ;

  DocPatho.ExtrairePatPathoWithRoot(iterPpt, pPatPatho) ;

  // conclusion is empty
  //
  if (1 == pPatPatho->Taille())
    pPatPatho->vider() ;
}

//-------------------------------------------------------------------------// Enlever les patpathos affichées des documents
//-------------------------------------------------------------------------
void
NSTreeHistorique::EnlevePatho()
{
	if (_NodeArray.empty())
		return ;

	iterNSTreeNode iterNode = _NodeArray.begin() ;
	for ( ; _NodeArray.end() != iterNode ; iterNode++)
	{
    int ligne  	 = (*iterNode)->getLigne() ;
    int cardinal = 0 ; //nb fils et petits fils
    //
    // Supprimer l'ancienne patpatho si elle existe
    //
    bool PatpthoNonVide = true ;
    SupprimerFils((*iterNode), &cardinal, &PatpthoNonVide) ;
    //
    // Mettre à jour les coordonnées des autres NSTreeNodes
    //
    iterNSTreeNode iter = _NodeArray.begin() ;
    if (cardinal > 0)
    	for ( ; _NodeArray.end() != iter ; iter++)
      {
        int iNodeLine = (*iter)->getLigne() ;
      	if (iNodeLine > ligne)
        	(*iter)->setLigne(iNodeLine - cardinal) ;
      }
	}
}

voidNSTreeHistorique::AfficheIcone(int iCategorie, NSTreeNode* pNSDocNode)
{
	if ((NSTreeNode*) NULL == pNSDocNode)
		return ;

	NSDocumentHisto* pDocument = getDocumentForNode(pNSDocNode) ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

	string sDocInteret = pDocument->getInteret() ;
	char interet = sDocInteret[0] ;
	int  nbIcone = iCategorie ;
	switch (interet)
	{
  	case 'A' :
    	nbIcone = iCategorie ;
      break ;
    case 'B' :
    	nbIcone = iCategorie + IndexDecalage ;
      break ;
    case 'C' :
    	nbIcone = iCategorie + 2*IndexDecalage ;
      break ;
    case 'D' :
    	nbIcone = iCategorie + 3*IndexDecalage ;
      break ;
		case 'E' :
    	nbIcone = iCategorie + 4*IndexDecalage ;
      break ;
	}

	pNSDocNode->SetImageIndex(nbIcone) ;
	pNSDocNode->SetSelectedImageIndex(nbIcone, true) ;
}

//-------------------------------------------------------------------------// Demander l'autorisation de l'ouverture de ce document, s'il est déjà ouvert,
// il suffit de l'activer
//-------------------------------------------------------------------------
void
NSTreeHistorique::AutoriserOuverture(NSDocumentInfo* pNSDocumentInfo)
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

  // No patient or patient being closed -> no way
  //
  NSPatientChoisi* pCurrentPatient = pContexte->getPatient() ;
  if (((NSPatientChoisi*) 0 == pCurrentPatient) || pCurrentPatient->isClosing())
    return ;

	string codeDocBrut = pNSDocumentInfo->getID() ;
	iterNSTreeNode iterNode = TrouveNoeud(codeDocBrut) ;

  // cas des documents visibles dans l'historique
  //
	if ((iterNode) && (_NodeArray.end() != iterNode))
	{
		OuvertureDocument(*iterNode) ;
    return ;
	}

  // on regarde d'abord le cas des documents visibles
  // qui ne figurent pas dans l'historique
  //
  if (true == pNSDocumentInfo->estVisible())
  {
    string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theDocumentDoesNotBelongToHistory") ;
    sErrorText += string(" (") + pNSDocumentInfo->getDocName() + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), warningError, 0) ;
    return ;
  }

  // cas des documents invisibles
  //
  if ((NSNoyauDocument*) NULL == _pDoc->EstUnDocumentOuvert(pNSDocumentInfo))
  {
    if (false == pContexte->userHasPrivilege(NSContexte::openDocument, -1, -1, pNSDocumentInfo->getAuthor(), string(""), NULL, NULL))
    {
      string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
      erreur(sErrorText.c_str(), warningError, 0) ;
      return ;
    }

    // on fait une copie sinon les HD sont transformés en documents bruts
    NSDocumentInfo CopyDocum(*pNSDocumentInfo) ;

    bool bReadOnlyOpening = true ;
    if (pContexte->userHasPrivilege(NSContexte::modifyDocument, -1, -1, pNSDocumentInfo->getAuthor(), string(""), NULL, NULL))
      bReadOnlyOpening = false ;

    NSNoyauDocument* pDocNoy = pContexte->getSuperviseur()->OuvreDocument(*pNSDocumentInfo, pContexte, bReadOnlyOpening) ;

    if (pDocNoy)      _pDoc->RangeDocumentOuvert(&CopyDocum, pDocNoy) ;  }
}

//----------------------------------------------------------------------------// Demander l'autorisation de l'édition de ce document (cas des fichiers Word)
// En principe, dans cette fonction, on peut changer le fichier word mais
// pas les paramètres du document (nom de fichier, nom de document, etc.)
// on ne vérifie pas l'appartenance à l'historique car on peut très bien
// éditer un document invisible dans l'historique.
//----------------------------------------------------------------------------
void
NSTreeHistorique::AutoriserEdition(NSDocumentInfo* pNSDocumentInfo)
{
try
{
	if ((NSDocumentInfo*) NULL == pNSDocumentInfo)
		return ;

	NSNoyauDocument* pDocNoy = (NSNoyauDocument*) 0 ;
  bool bReadOnlyOpening = true ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	if (pNSDocumentInfo->getSemCont() != "ZSYNT")
  {
  	if (false == pContexte->userHasPrivilege(NSContexte::openDocument, -1, -1, pNSDocumentInfo->getAuthor(), string(""), NULL, NULL))
		{
			string sErrorText = pSuper->getText("privilegeManagement", "insufficientRights") ;
			erreur(sErrorText.c_str(), warningError, 0) ;
			return ;
		}
    if (pContexte->userHasPrivilege(NSContexte::modifyDocument, -1, -1, pNSDocumentInfo->getAuthor(), string(""), NULL, NULL))
    	bReadOnlyOpening = false ;
  }
	else
	{
  	if (false == pContexte->userHasPrivilege(NSContexte::viewSynthesis, -1, -1, string(""), string(""), NULL, NULL))
		{
			string sErrorText = pSuper->getText("privilegeManagement", "insufficientRights") ;
			erreur(sErrorText.c_str(), warningError, 0) ;
			return ;
		}
    if (pContexte->userHasPrivilege(NSContexte::modifySynthesis, -1, -1, string(""), string(""), NULL, NULL))
    	bReadOnlyOpening = false ;
	}

	// on récupère le iterNode pour l'affichage des icônes	iterNSTreeNode iterNode = _NodeArray.begin() ;
	string codeDocBrut = pNSDocumentInfo->getID() ;
	iterNode = TrouveNoeud(codeDocBrut) ;

  bool bDocInHisto = true ;
	if (_NodeArray.end() == iterNode)
		bDocInHisto = false ;

	// on charge à cet endroit la pNSDocumentInfo si elle n'est pas complete	// (cas par exple de la synthèse à l'initialisation). Pour éviter de la charger	// deux fois (si elle provient de l'historique, comme dans EditionDocument),	// on considère qu'elle est chargée si sCodeDocMeta != ""	if ((string("") == pNSDocumentInfo->getCodeDocMeta()) && (false == _pDoc->ChargeDocInfo(pNSDocumentInfo)))	{    string sErrorText = pSuper->getText("documentHistoryManagementErrors", "theDocumentCannotBeEdited") ;    sErrorText += string(" (") + pNSDocumentInfo->getPatient() + string(" ") + pNSDocumentInfo->getDocument() + string(")") ;
    pSuper->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
	}	NSDocViewManager dvManager(pContexte) ;

	if (string("ZCN00") == pNSDocumentInfo->getTypeSem())
	{
		NSCRDocument* pCRDoc = (NSCRDocument*) 0 ;

		// donne le focus à la vue d'édition si elle existe
		TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSCRReadOnlyView") ;
		if (pView == 0)
		{
			// si le document est ouvert, on le récupère pour l'éditer
			pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
			if (pDocNoy)
				pCRDoc = dynamic_cast<NSCRDocument*>(pDocNoy) ;
			else // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
			{
				if (bDocInHisto)
					AfficheIcone(IndexIcone_CRouvert, *iterNode) ;
				pCRDoc = new NSCRDocument(0, pNSDocumentInfo, 0, pContexte, "", bReadOnlyOpening) ;
			}

      dvManager.createView(pCRDoc, "CN Format") ;

			// on déclare éventuellement ouvert le nouveau compte-rendu
			if ((NSNoyauDocument*) NULL == pDocNoy)
				_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pCRDoc) ;
		}
	}
	else if (string("ZCS00") == pNSDocumentInfo->getTypeSem())
	{
    string sSemContent = pNSDocumentInfo->getSemCont() ;

    string sWhat  = string("") ;
    string sWhere = string("") ;
    bool bIsSpecific = pSuper->getWhereToFind(sSemContent, sWhat, sWhere) ;

    if ((false == bIsSpecific) && (string("ZCOOP") == sSemContent))
    {
      bIsSpecific = true ;
      sWhat  = string("QoL") ;
      sWhere = string("CoopCharts") ;
    }

    if (bIsSpecific)
    {
      string sWHAT = pseumaj(sWhat) ;

      // Quality of life cards, à la Coop Charts
      //
      if (string("QOL") == sWHAT)
      {
        NSCoopChartDocument* pCoopDoc = (NSCoopChartDocument*) 0 ;

        // donne le focus à la vue d'édition si elle existe
        TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, sWhere) ;
        if ((TView*) NULL == pView)
        {
          // si le document est ouvert, on le récupère pour l'éditer
          pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
          if (pDocNoy)
            pCoopDoc = dynamic_cast<NSCoopChartDocument*>(pDocNoy) ;
          else // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
          {
            if (bDocInHisto)
              AfficheIcone(IndexIcone_CSouvert, *iterNode) ;
            pCoopDoc = new NSCoopChartDocument((TDocument*) 0, pNSDocumentInfo, (NSDocumentInfo*) 0, pContexte, bReadOnlyOpening, sWhere) ;
          }

          dvManager.createView(pCoopDoc, "Coop Chart") ;

          // on déclare éventuellement ouvert la nouvelle consultation
          if ((NSNoyauDocument*) NULL == pDocNoy)
            _pDoc->RangeDocumentOuvert(pNSDocumentInfo, pCoopDoc) ;
        }

        return ;
      }
    }

    NSCSDocument* pCSDoc = (NSCSDocument*) 0 ;

    // donne le focus à la vue d'édition si elle existe
    TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSCsVue") ;
    if ((TView*) NULL == pView)
    {
      // si le document est ouvert, on le récupère pour l'éditer
      pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
      if (pDocNoy)
        pCSDoc = dynamic_cast<NSCSDocument*>(pDocNoy) ;
      else // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
      {
        if (bDocInHisto)
          AfficheIcone(IndexIcone_CSouvert, *iterNode) ;
        pCSDoc = new NSCSDocument((TDocument*) 0, pNSDocumentInfo, (NSDocumentInfo*) 0, pContexte, string(""), bReadOnlyOpening) ;
      }

      dvManager.createView(pCSDoc, "CS Format") ;

      // on déclare éventuellement ouvert la nouvelle consultation
      if ((NSNoyauDocument*) NULL == pDocNoy)
        _pDoc->RangeDocumentOuvert(pNSDocumentInfo, pCSDoc) ;
    }
	}
	else if (string("ZCQ00") == pNSDocumentInfo->getTypeSem())	{
		// cas des formulaires
		NSCQDocument* pCQDoc = (NSCQDocument*) 0 ;
		// donne le focus à la vue d'édition si elle existe		TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSCQVue") ;
		if (pView == 0)
		{
			// si le document est ouvert, on le récupère pour l'éditer
			pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
			if (pDocNoy)
			{				pCQDoc = dynamic_cast<NSCQDocument*>(pDocNoy) ;
        if ((NSCQDocument*) NULL == pCQDoc)
        	return ;
				pCQDoc->setCreated(false) ;			}			else // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert			{
				if (bDocInHisto)
					AfficheIcone(IndexIcone_CQouvert, *iterNode) ;

        //
        // Declaring default Archetypes
        //
        string sDefaultArchetype = string("") ;
        if (string("ZADMI") == pNSDocumentInfo->getSemCont())
        	sDefaultArchetype = pSuper->getDemographicArchetypeId() ;

				pCQDoc = new NSCQDocument(0, pNSDocumentInfo, 0, pContexte, bReadOnlyOpening, sDefaultArchetype) ;				if (false == pCQDoc->isParsed())				{        	delete pCQDoc ;					return ;				}
			}

      dvManager.createView(pCQDoc, "CQ Format") ;

			// on déclare éventuellement ouvert le nouveau formulaire			if ((NSNoyauDocument*) NULL == pDocNoy)
				_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pCQDoc) ;
		}
	}
  else if (string("ZQQOL") == pNSDocumentInfo->getTypeSem())
	{
    string sSemContent = pNSDocumentInfo->getSemCont() ;

    string sWhat  = string("") ;
    string sWhere = string("") ;
    bool bIsSpecific = pSuper->getWhereToFind(sSemContent, sWhat, sWhere) ;

    if (false == bIsSpecific)
    {
      bIsSpecific = true ;
      sWhat  = string("QoL") ;
      sWhere = string("CoopCharts") ;
    }

    string sWHAT = pseumaj(sWhat) ;

    // Quality of life cards, à la Coop Charts
    //
    if (string("QOL") == sWHAT)
    {
      NSCoopChartDocument* pCoopDoc = (NSCoopChartDocument*) 0 ;

      // donne le focus à la vue d'édition si elle existe
      TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, sWhere) ;
      if ((TView*) NULL == pView)
      {
        // si le document est ouvert, on le récupère pour l'éditer
        pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
        if (pDocNoy)
          pCoopDoc = dynamic_cast<NSCoopChartDocument*>(pDocNoy) ;
        else // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
        {
          if (bDocInHisto)
            AfficheIcone(IndexIcone_CSouvert, *iterNode) ;
          pCoopDoc = new NSCoopChartDocument((TDocument*) 0, pNSDocumentInfo, (NSDocumentInfo*) 0, pContexte, bReadOnlyOpening, sWhere) ;
        }

        dvManager.createView(pCoopDoc, "Coop Chart") ;

        // on déclare éventuellement ouvert la nouvelle consultation
        if ((NSNoyauDocument*) NULL == pDocNoy)
          _pDoc->RangeDocumentOuvert(pNSDocumentInfo, pCoopDoc) ;
      }
    }
  }
	else if (string("ZTRTF") == pNSDocumentInfo->getTypeSem())
	{
		NSTtxDocument* pDocTtx = (NSTtxDocument*) 0 ;

		// donne le focus à la vue d'édition si elle existe
		TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSTtxView") ;
		if (pView == 0)
		{
			// si le document est ouvert, on le récupère pour l'éditer
			pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
			if (pDocNoy)
				pDocTtx = dynamic_cast<NSTtxDocument*>(pDocNoy) ;
			if (!pDocTtx) // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
			{
				if (bDocInHisto)
					AfficheIcone(IndexIcone_tlib, *iterNode) ;
				pDocTtx = new NSTtxDocument(0, bReadOnlyOpening, pNSDocumentInfo, 0, pContexte) ;
			}

      dvManager.createView(pDocTtx, "Rtf Format") ;

			// on déclare éventuellement ouvert le nouveau document texte
			if ((NSNoyauDocument*) NULL == pDocNoy)
				_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pDocTtx) ;
		}
	}
	else if (string("ZTTXT") == pNSDocumentInfo->getTypeSem())	{
		NSSimpleTxtDocument* pDocTxt = (NSSimpleTxtDocument*) 0 ;

		// donne le focus à la vue d'édition si elle existe
		TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSSimpleTxtView") ;
		if ((TView*) NULL == pView)
		{
			// si le document est ouvert, on le récupère pour l'éditer
			pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
			if (pDocNoy)
				pDocTxt = dynamic_cast<NSSimpleTxtDocument*>(pDocNoy) ;
			if ((NSSimpleTxtDocument*) NULL == pDocTxt) // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
			{
				if (bDocInHisto)
					AfficheIcone(IndexIcone_tlib, *iterNode) ;
				pDocTxt = new NSSimpleTxtDocument(0, bReadOnlyOpening, pNSDocumentInfo, 0, pContexte) ;
      }

      dvManager.createView(pDocTxt, "Txt Format") ;

			// on déclare éventuellement ouvert le nouveau document texte
			if ((NSNoyauDocument*) NULL == pDocNoy)
				_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pDocTxt) ;
		}
	}
	else if (string("ZTHTM") == pNSDocumentInfo->getTypeSem())
	{
		NSTtxDocument* pDocTtx = (NSTtxDocument*) 0 ;

		// donne le focus à la vue d'édition si elle existe
		TView* pView = _pDoc->ActiveFenetre(pNSDocumentInfo, "NSAutoWordView") ;
		if ((TView*) NULL == pView)
		{
			// si le document est ouvert, on le récupère pour l'éditer
			pDocNoy = _pDoc->EstUnDocumentOuvert(pNSDocumentInfo) ;
			if (pDocNoy)
				pDocTtx = dynamic_cast<NSTtxDocument*>(pDocNoy) ;
			if ((NSTtxDocument*) NULL == pDocTtx) // on crée ici un nouveau pointeur, qu'on doit déclarer ouvert
			{
				if (bDocInHisto)
					AfficheIcone(IndexIcone_tlib, *iterNode) ;
				pDocTtx = new NSTtxDocument(0, bReadOnlyOpening, pNSDocumentInfo, 0, pContexte) ;
			}

      dvManager.createView(pDocTtx, "Word Format") ;

			// on déclare éventuellement ouvert le nouveau document texte
			if ((NSNoyauDocument*) NULL == pDocNoy)
				_pDoc->RangeDocumentOuvert(pNSDocumentInfo, pDocTtx) ;
		}
	}
	else
	{
  	string sErrorText = pSuper->getText("documentHistoryManagementErrors", "noEditFunctionExistsForTheDocument") ;
    sErrorText += string(" (") + pNSDocumentInfo->getDocName() + string(")") ;
    pSuper->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), warningError, 0) ;
	}
}
catch (...)
{
	erreur("Exception NSTreeHistorique::AutoriserEdition", standardError, 0) ;
}
}

//----------------------------------------------------------------------// un noeud a été sélectionné : ouverture d'un document
//----------------------------------------------------------------------
void
NSTreeHistorique::EvLButtonDblClk(uint /* a */, NS_CLASSLIB::TPoint& point)
{
	NSTreeNode* pNSTreeNodeSelection = getNodeForPoint(point) ;
	if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
		return ;

	::SetCursor(::LoadCursor(0, IDC_WAIT)) ;

	OuvertureDocument(pNSTreeNodeSelection);
	::SetCursor(::LoadCursor(0, IDC_ARROW)) ;}

//---------------------------------------------------------------------//ouverture ou activation d'un document
//---------------------------------------------------------------------
void
NSTreeHistorique::OuvertureDocument(NSTreeNode* pNSDocNode)
{
	if ((NSTreeNode*) NULL == pNSDocNode)
		return ;
    
	NSDocumentHisto* pDocument = getDocumentForNode(pNSDocNode) ;
	if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  // Already open ; put it up
  //
	if (_pDoc->EstUnDocumentOuvert(pDocument))
  {
  	_pDoc->ActiveFenetre(pDocument) ;
    return ;
  }

/*
	if (string("ZDHTM") == pDocument->getType())
	{
		NSDocumentInfo* pDocHtml = 0 ;
		NSDocumentInfo* pNSDocumentHisto = new NSDocumentInfo(*pDocument) ;
		// on retrouve le document brut
		if (!pContexte->ChercheComposition(&pNSDocumentHisto,&pDocHtml))
		{
    	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
      sErrorText += string(" (") + pNSDocumentHisto->getPatient() + string(" ") + pNSDocumentHisto->getDocument() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

			delete pNSDocumentHisto ;
			return ;
		}
		if      (string("ZCN00") == pNSDocumentHisto->getType())
			AfficheIcone(IndexIcone_CRouvert, pNSDocNode) ;
		else if (string("ZCS00") == pNSDocumentHisto->getType())
			AfficheIcone(IndexIcone_CSouvert, pNSDocNode);

		delete pNSDocumentHisto ;
		delete pDocHtml ;
	}
	else
	{
*/
		// cas de la synthèse : on lance l'édition et on sort (à revoir)
		if ((string("ZCS00") == pDocument->getTypeSem()) &&
                (string("00000") == pDocument->getDocument()))
		{
			// AutoriserEdition va déclarer ouverte la synthèse
			AutoriserEdition(pDocument) ;
			return ;
		}

		if (false == pContexte->userHasPrivilege(NSContexte::openDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
		{
			string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "insufficientRights") ;
			erreur(sErrorText.c_str(), warningError, 0) ;
			return ;
		}

		// Affichage des icones "document ouvert"
		if      (string("ZCN00") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_CRouvert, pNSDocNode) ;
    else if (string("ZCS00") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_CSouvert, pNSDocNode) ;
    else if (string("ZCQ00") == pDocument->getTypeSem())    	AfficheIcone(IndexIcone_CQouvert, pNSDocNode) ;
    else if (string("ZTPDF") == pDocument->getTypeSem())
    	AfficheIcone(IndexIcone_ARouvert, pNSDocNode) ;
    else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isImage))
    	AfficheIcone(IndexIcone_ImageFer, pNSDocNode) ;
    else if (pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isText))
    	AfficheIcone(IndexIcone_tlib, pNSDocNode) ;
		else
			//bitmap vide
			AfficheIcone(IndexIcone_vide, pNSDocNode) ;
//	}
	//
	// Ouverture effective du document : appel de pSuper
	//
	NSDocumentInfo Docum(pContexte) ;
  pDocument->initDocumentInfo(&Docum) ;

	bool bReadOnlyOpening = true ;
	if (pContexte->userHasPrivilege(NSContexte::modifyDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
		bReadOnlyOpening = false ;

	NSNoyauDocument* pDocNoy = pContexte->getSuperviseur()->OuvreDocument(Docum, pContexte, bReadOnlyOpening) ;
	if (pDocNoy)
	{
		// on reprend les donnees pour éviter l'effet de bord de OuvreDocument
  	// (switch entre document HD et document brut CN)
    NSDocumentData InfoData ;
    pDocument->initFromData(&InfoData) ;
		Docum.setData(&InfoData) ;

		// on range le pHtmlInfo dans le cas d'un HD
		// (permet de savoir que le document est composé)
		_pDoc->RangeDocumentOuvert(&Docum, pDocNoy) ;
	}
}

//---------------------------------------------------------------------// Edition d'un document
//---------------------------------------------------------------------
void
NSTreeHistorique::EditionDocument(NSTreeNode* pNSDocNode)
{
	if ((NSTreeNode*) NULL == pNSDocNode)
		return ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

	NSDocumentHisto* pDocument = static_cast<NSDocumentHisto*>(pNSDocNode->getDocument());
	if ((NSDocumentHisto*) NULL == pDocument)
  {
    pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
		return ;
  }

	// Cas particulier des documents composés
	//
/*
	if (string("HD") == string(pDocument->getType(), 0, 2))
	{
		NSDocumentInfo* pDocHtml = 0;
		NSDocumentInfo* pDocBrut = new NSDocumentInfo(*pDocument);
		// on retrouve le document brut
		if (!pContexte->ChercheComposition(&pDocBrut, &pDocHtml))
		{
    	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
    	sErrorText += string(" (") + pDocBrut->getPatient() + string(" ") + pDocBrut->getDocument() + string(")") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

			delete pDocBrut;
			return ;
		}
		pDocument = static_cast<NSDocumentHisto*>(pDocBrut);
	}
*/

	AutoriserEdition(pDocument) ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

//----------------------------------------------------------------------
// Réponse à une touche du clavier
//----------------------------------------------------------------------
void
NSTreeHistorique::EvKeyUp(uint key, uint /* repeatcount */, uint /* flags */){
  if ((VK_SPACE == key) || (VK_RETURN == key))
  {
    NSTreeNode* pNSTreeNodeSelection = getSelectedNode() ;
		if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
			return ;

    // VisualiserDocument(pNode) ;

    ::SetCursor(::LoadCursor(0, IDC_WAIT)) ;

	  OuvertureDocument(pNSTreeNodeSelection);
	  ::SetCursor(::LoadCursor(0, IDC_ARROW)) ;

    return ;
  }
}

//----------------------------------------------------------------------// Réponse à une touche du clavier
//----------------------------------------------------------------------
void
NSTreeHistorique::EvTvKeyDown(TTwKeyDownNotify& nhmr)
{
	//ouverture du document
/*
	if (VK_RETURN == nhmr.wVKey)
	{
    NSTreeNode* pNode = getSelectedNode() ;
		if ((NSTreeNode*) NULL == pNode)
			return ;

    VisualiserDocument(pNode) ;

    return ;
	}
*/
  //
  // Ctrl + Touche
  //
  if (GetKeyState(VK_CONTROL) < 0)
  {
    // Ctrl I : Show/Hide indexes
    //
    if      ('I' == nhmr.wVKey)
    {
      _bVisibleManagementTrees = !_bVisibleManagementTrees ;      AfficheDocument() ;
      return ;
    }
  }

  DefaultProcessing() ;
}

//--------------------------------------------------------------------// Trouver un noeud à partir d'un document
//--------------------------------------------------------------------
iterNSTreeNode
NSTreeHistorique::TrouveNoeud(string sCodeDocBrut)
{
	if (_NodeArray.empty())
  	return NULL ;

	iterNSTreeNode iterNode = _NodeArray.begin() ;
	for ( ; _NodeArray.end() != iterNode ; iterNode++)
	{
  	NSDocumentHisto* pDocument = static_cast<NSDocumentHisto*>((*iterNode)->getDocument()) ;
    if ((pDocument) && (sCodeDocBrut == pDocument->getID()))
    	return iterNode ;
	}

	return _NodeArray.end() ;
}

//-------------------------------------------------------------------------//click droit sur un document
//-------------------------------------------------------------------------
void
NSTreeHistorique::EvRButtonDown(uint , NS_CLASSLIB::TPoint& point)
{
	NSTreeNode* pNSTreeNodeSelection = getNodeForPoint(point) ;
  if ((NSTreeNode*) NULL == pNSTreeNodeSelection)
		return ;

	NSDocumentInfo* pDocument = getDocumentForNode(pNSTreeNodeSelection) ;
	if ((NSDocumentInfo*) NULL == pDocument)
		return ;

	OWL::TPopupMenu *menu = new OWL::TPopupMenu() ;

	pNSTreeNodeSelection->SelectItem(TVGN_CARET) ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;
	string sOpen = pSuper->getText("documentManagement", "open") ;
  sOpen[0] = pseumaj(sOpen[0]) ;
  string sOpeA = pSuper->getText("documentManagement", "openEveryDocument") ;
  sOpeA[0] = pseumaj(sOpeA[0]) ;
  string sOpeE = pSuper->getText("documentManagement", "openExternalDocument") ;
  sOpeE[0] = pseumaj(sOpeE[0]) ;
	string sEdit = pSuper->getText("documentManagement", "edit") ;
  sEdit[0] = pseumaj(sEdit[0]) ;
	string sPara = pSuper->getText("documentManagement", "parameters") ;
  sPara[0] = pseumaj(sPara[0]) ;
	string sProp = pSuper->getText("documentManagement", "properties") ;
  sProp[0] = pseumaj(sProp[0]) ;
	string sRemo = pSuper->getText("documentManagement", "removeFromHistory") ;
  sRemo[0] = pseumaj(sRemo[0]) ;
	string sDele = pSuper->getText("documentManagement", "destroy") ;
  sDele[0] = pseumaj(sDele[0]) ;
  string sExpo = pSuper->getText("documentManagement", "exportAsXmlFile") ;
  sExpo[0] = pseumaj(sExpo[0]) ;
  string sReas = pSuper->getText("documentManagement", "reassignToAnotherPatient") ;
  sReas[0] = pseumaj(sReas[0]) ;
  string sSign = pSuper->getText("documentManagement", "forceAttributionOfThisDocument") ;
  sSign[0] = pseumaj(sSign[0]) ;
	//
	// This is not the synthesis
	//
	if (pDocument->getSemCont() != string("ZSYNT"))
	{
    // External document : the only menu is to open externally
    //
    if ((pDocument->getTypeSem() == string("ZDINT")) ||
        (pDocument->getTypeSem() == string("ZURL0")))
      menu->AppendMenu(MF_STRING, IDC_OPEN_EXT, sOpeE.c_str()) ;

    // Regular document
    //
    else
    {
  	  menu->AppendMenu(MF_STRING, IDC_OUVRIR, sOpen.c_str()) ;

      if (string("ZTPDF") == pDocument->getTypeSem())
        menu->AppendMenu(MF_STRING, IDC_OPEN_EXT, sOpeE.c_str()) ;

      //
		  // Does the author have privileges to modify this document
      //
		  if ((pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isTree)) ||
			  	(pContexte->typeDocument(pDocument->getTypeSem(), NSSuper::isText)) ||
            (string("ZDHTM") == pDocument->getTypeSem()))
		  {
			  if (pContexte->userHasPrivilege(NSContexte::modifyDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
			  	menu->AppendMenu(MF_STRING, CM_EDITER, sEdit.c_str()) ;
			  else
				  menu->AppendMenu(MF_GRAYED, 0, sEdit.c_str()) ;
      }
      // Is this document connected to an external document
      //
      string sMetaDocId = pDocument->getCodeDocMeta() ;
      if (pContexte->getPatient()->getGraphPerson()->getExternalDocId(sMetaDocId) != string(""))
        menu->AppendMenu(MF_STRING, IDC_OPEN_EXT, sOpeE.c_str()) ;
    }

		menu->AppendMenu(MF_STRING, IDC_PARAMETRES, sPara.c_str()) ;
		menu->AppendMenu(MF_STRING, IDC_PROPRIETE,  sProp.c_str()) ;
    menu->AppendMenu(MF_STRING, IDC_XML_EXPORT, sExpo.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		menu->AppendMenu(MF_STRING, IDC_OTER,       sRemo.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		// Does the author have privileges to suppres this document
		//
		if (pContexte->userHasPrivilege(NSContexte::suppresDocument, -1, -1, pDocument->getAuthor(), string(""), NULL, NULL))
			menu->AppendMenu(MF_STRING, IDC_DETRUIRE,	sDele.c_str()) ;
    else
    	menu->AppendMenu(MF_GRAYED, 0,           	sDele.c_str()) ;

    // NSDocumentData InfoData ;
    // pDocument->initFromData(&InfoData) ;

    // if (NULL != pDocument->getData())
    // {
      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		  menu->AppendMenu(MF_STRING, IDC_TRANSFERT, sReas.c_str()) ;
      menu->AppendMenu(MF_STRING, IDC_FORCESIGN, sSign.c_str()) ;
    // }
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		menu->AppendMenu(MF_STRING, IDC_TOUTOUVRIR, sOpeA.c_str()) ;

    if (string("ZCN00") == pDocument->getTypeSem())
    {
      string sFrom = pSuper->getText("documentManagement", "createNewDocumentFromThisOne") ;
      sFrom[0] = pseumaj(sFrom[0]) ;

      menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		  menu->AppendMenu(MF_STRING, IDC_NEW_FROM, sFrom.c_str()) ;
    }
	}
  //
	// This is the synthesis
	//
	else
	{
  	menu->AppendMenu(MF_STRING, IDC_OUVRIR, sOpen.c_str()) ;
    //
		// Does the author have privileges to modify this document
    //
		if (pContexte->userHasPrivilege(NSContexte::modifySynthesis, -1, -1, string(""), string(""), NULL, NULL))
			menu->AppendMenu(MF_STRING, CM_EDITER, sEdit.c_str()) ;
		else
			menu->AppendMenu(MF_GRAYED, 0, sEdit.c_str()) ;
		menu->AppendMenu(MF_STRING, IDC_PARAMETRES, sPara.c_str()) ;
		menu->AppendMenu(MF_STRING, IDC_PROPRIETE,  sProp.c_str()) ;
    menu->AppendMenu(MF_STRING, IDC_XML_EXPORT, sExpo.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		menu->AppendMenu(MF_STRING, IDC_OTER,       sRemo.c_str()) ;
		menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
		menu->AppendMenu(MF_STRING, IDC_TOUTOUVRIR, sOpeA.c_str()) ;
	}

  NS_CLASSLIB::TPoint lp = point ;
	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;

	return ;
}

voidNSTreeHistorique::EvHScroll(uint scrollCode, uint thumbPos, HWND hWndCtl)
{
	TWindow::EvHScroll(scrollCode, thumbPos, hWndCtl) ;
}

voidNSTreeHistorique::EvVScroll(uint scrollCode, uint thumbPos, HWND hWndCtl)
{
	TWindow::EvVScroll(scrollCode, thumbPos, hWndCtl) ;
}

void
NSTreeHistorique::EvSetFocus(HWND hWndLostFocus)
{
	TWindow::EvSetFocus(hWndLostFocus) ;
}

void
NSTreeHistorique::EvClose()
{
}

void
NSTreeHistorique::SelChanged()
{
	TWindow::Invalidate() ;
}

int
NSTreeHistorique::donneIndexInteret(char cInteret)
{
	if      (cInteret == 'A')
		return 9 ;
	else if (cInteret == 'B')
		return 10 ;
	else if (cInteret == 'C')
  	return 11 ;
	else if (cInteret == 'D')
  	return 12 ;
	else if (cInteret == 'E')
  	return 13 ;

	return 0 ;
}
voidNSTreeHistorique::Ouvrir()
{
	NSTreeNode* pNSTreeNodeEdit = getSelectedNode() ;
	if ((NSTreeNode*) NULL == pNSTreeNodeEdit)
		return ;

	OuvertureDocument(pNSTreeNodeEdit) ;}

voidNSTreeHistorique::Editer()
{
	NSTreeNode* pNSTreeNodeEdit = getSelectedNode() ;
	if ((NSTreeNode*) NULL == pNSTreeNodeEdit)
		return ;

	EditionDocument(pNSTreeNodeEdit) ;
}

void
NSTreeHistorique::OpenOutside()
{
	NSTreeNode* pNSTreeNodeEdit = getSelectedNode() ;
	if ((NSTreeNode*) NULL == pNSTreeNodeEdit)
		return ;

  NSDocumentInfo* pDocument = getDocumentForNode(pNSTreeNodeEdit) ;
	if ((NSDocumentInfo*) NULL == pDocument)
		return ;

  string sDocUri = string("") ;

  if ((string("ZDINT") != pDocument->getTypeSem()) &&
      (string("ZURL0") != pDocument->getTypeSem()) &&
      (string("ZTPDF") != pDocument->getTypeSem()))
  {
    string sMetaDocId = pDocument->getCodeDocMeta() ;

    NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;

    string sExternalDocId = pContexte->getPatient()->getGraphPerson()->getExternalDocId(sMetaDocId) ;
    if (string("") == sExternalDocId)
      return ;

    NSDocumentInfo externalDocument(sExternalDocId, pContexte, pGraphManager) ;
    externalDocument.LoadMetaAndData(pGraphManager) ;
    sDocUri = externalDocument.getFichier() ;
  }
  else
    sDocUri = pDocument->getFichier() ;

  if (string("") == sDocUri)
    return ;

  if (string("ZTPDF") == pDocument->getTypeSem())
  {
    string sPathTxt = pContexte->PathName("NTTX") ;
    sDocUri = sPathTxt + sDocUri ;

    string sMsg = string("NSTreeHistorique::OpenOutside, open a PDF outside for file ") + sDocUri ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

    ::ShellExecute(pContexte->GetMainWindow()->HWindow,
                 "open",
                 sDocUri.c_str(),
                 "",
                 "",
                 SW_SHOWNORMAL) ;

    return ;
  }

  NSInterfaceDocument interfaceDoc((TDocument*) 0, pContexte) ;
  interfaceDoc.openRemoteDocument(sDocUri) ;
}

voidNSTreeHistorique::ToutOuvrir()
{
	if (_NodeArray.empty())
		return ;

	iterNSTreeNode iter = _NodeArray.end() ;
	while (_NodeArray.begin() != iter)
	{
  	iter-- ;
    if ((*iter)->getColonne() == 0)
    	OuvertureDocument(*iter) ;
	}
}

voidNSTreeHistorique::Oter()
{
	NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
  	return ;

  string ps = "Removing from history document " + pDocument->getDocName() ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
	Docum.setData(&InfoData) ;

	//
  // On rend le document invisible
  //
  NSDocumentInfo DocInfoMeta(pContexte) ;
  // remise à jour du pDocInfoMeta pour charger le document Meta
  // on précise ici uniquement codePatient et codeDocument sans préciser le type
  // on doit ensuite charger la patpatho à la main (cf NSDocumentInfo::ChargeDocMeta())
  DocInfoMeta.setPatient(pDocument->getPatient()) ;
  string sNewCodeDoc = string(pDocument->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
  DocInfoMeta.setDocument(sNewCodeDoc) ;

  NSNoyauDocument NoyDoc(0, &DocInfoMeta, 0, pContexte, false) ;
  bool resultat = NoyDoc.chargePatPatho() ;
  if (false == resultat)
    return ;

  if (NoyDoc.DocumentInvisible(pDocument->getCodeDocMeta()))
  	// on reporte la modif en mémoire
    pDocument->rendInvisible() ;

	//
	// On supprime le document de l'historique
	//
  EnleverDocument(&Docum) ;

	//
	// Chercher pDocum dans VectDocument
	//
	// DocumentIter iterDoc = pDoc->VectDocument.TrouveDocHisto(Docum.getID()) ;

	// on l'enlève de VectDocument
/*
	if ((iterDoc != NULL) && (iterDoc != pDoc->VectDocument.end()))
  {
  	delete *iterDoc ;
    pDoc->VectDocument.erase(iterDoc) ;
	}
*/
}

// méthode appelée depuis ChoixChemiseDialog (nsrechdl) par pDocHis// Ajoute un document invisible (on le rend visible) à l'historique (inverse de Oter)
void
NSTreeHistorique::Ajouter(NSDocumentInfo* pDocumentInfo)
{
  //
  // On rend le document visible
  //
  NSDocumentInfo DocInfoMeta(pContexte) ;
  // remise à jour du pDocInfoMeta pour charger le document Meta
  // on précise ici uniquement codePatient et codeDocument sans préciser le type
  // on doit ensuite charger la patpatho à la main (cf NSDocumentInfo::ChargeDocMeta())
  DocInfoMeta.setPatient(pDocumentInfo->getPatient()) ;
  string sNewCodeDoc = string(pDocumentInfo->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
  DocInfoMeta.setDocument(sNewCodeDoc) ;

  NSNoyauDocument NoyDoc((TDocument*) 0, &DocInfoMeta, 0, pContexte, false, true) ;
  bool resultat = NoyDoc.chargePatPatho() ;
  if (false == resultat)
    return ;

  if (NoyDoc.DocumentVisible(pDocumentInfo->getCodeDocMeta()))
  	// on reporte la modif en mémoire
    pDocumentInfo->rendVisible() ;

	//
	// On ajoute le document à l'historique
	// mais il ne s'agit pas d'un nouveau document
	// (on n'ajoute pas le document à l'array des documents ouvert)
	//
	AjouteDocument(pDocumentInfo, 0) ;
}

void
NSTreeHistorique::Proprietes()
{
  string ps = "Entering the document Proprietes function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSTreeNode* pNSDocNode = getSelectedNode() ;
	if ((NSTreeNode*) NULL == pNSDocNode)
  	return ;

	string sTitre = pNSDocNode->getLabelString() ;

	NSDocumentHisto* pDocument = getDocumentForNode(pNSDocNode) ;
  if ((NSDocumentHisto*) NULL == pDocument)
  	return ;

	NSDocumentInfo Docum(pContexte) ;

  pDocument->initDocumentInfo(&Docum) ;

  // NSDocumentData InfoData ;
  // pDocument->initFromData(&InfoData) ;
	// Docum.setData(&InfoData) ;

	NSPropDocDialog* pPropDlg = new NSPropDocDialog(pContexte->GetMainWindow(),
                                                    sTitre, &Docum, pContexte) ;
	pPropDlg->Execute() ;
	delete pPropDlg ;

  ps = "Leaving the document Proprietes function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}

voidNSTreeHistorique::Parametres()
{
  string ps = "Entering the document Parametres function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

	NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  NSDocumentInfo* pHtmlInfo = (NSDocumentInfo*) 0 ;

	NSDocumentInfo DocInfo(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
	DocInfo.setData(&InfoData) ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  pDocument->initFromMeta(&PptMeta) ;
	DocInfo.setMeta(&PptMeta) ;

	DocInfo.setCodeDocMeta(pDocument->getCodeDocMeta()) ;

/*
	// cas des html dynamiques : on instancie le pHtmlInfo du document
  if (string("HD") == string(pDocument->getType(), 0, 2))
  {
  	// on retrouve le document brut
    if (!pContexte->ChercheComposition(&pDocInfo, &pHtmlInfo))
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
    	sErrorText += string(" (") + pDocInfo->getPatient() + string(" ") + pDocInfo->getDocument() + string(")") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    	erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

      delete pDocInfo ;
      return ;
    }
	}
*/

	// on crée un docref virtuel pour pouvoir lancer le dialogue des paramètres
	// ce document est donc une copie d'un document qui est dans l'historique
	// on ne lance pas validefichier comme dans NSSuper car on n'ouvre pas le document
	NSRefDocument DocRef(0, &DocInfo, pHtmlInfo, pContexte, false) ;
	if (DocRef.ParamDoc())
	{
  	// on doit prendre le pDocInfo du pDocRef pour tenir compte des modifs
    if (DocRef._pHtmlInfo)
    	_pDoc->Rafraichir(DocRef._pHtmlInfo, 0) ;
    else
    	_pDoc->Rafraichir(DocRef._pDocInfo, 0) ;
	}

	// IMPORTANT : on doit remettre pDocInfo et pHtmlInfo à 0
	// pour éviter que ~NSRefDocument ne remette à jour l'historique
	DocRef._pDocInfo = 0 ;
	DocRef._pHtmlInfo = 0 ;

  ps = "Leaving the document Parametres function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}

voidNSTreeHistorique::Detruire()
{
	NSTreeNode* pNSDocNode = getSelectedNode() ;
	if ((NSTreeNode*) NULL == pNSDocNode)
  	return ;

	string sTitre = pNSDocNode->getLabelString() ;

	NSDocumentHisto* pDocument = getDocumentForNode(pNSDocNode) ;
  if ((NSDocumentHisto*) NULL == pDocument)
  	return ;

	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
	Docum.setData(&InfoData) ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  pDocument->initFromMeta(&PptMeta) ;
	Docum.setMeta(&PptMeta) ;

	Docum.setCodeDocMeta(pDocument->getCodeDocMeta()) ;

	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;	string sMessage = pContexte->getSuperviseur()->getText("documentHistoryManagement", "doYouReallyWantToSuppressTheDocument") + string(" ") + sTitre ;

	int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO);
	if (IDYES != retVal)
  	return ;

  string sCodeDoc = Docum.getID() ;

	// Destruction du document
	// Cette méthode rend le document invisible et remplit
	// le champ pDonnees->tranDel pour le signaler détruit
  //
	pContexte->getPatient()->DetruireDocument(&Docum) ;

	// on appelle rafraichir pour enlever le document de l'historique
  //
	_pDoc->Rafraichir(&Docum, 0) ;

  // Finally, remove from documents array
  //
  NSDocHistoArray* pDocuments = _pDoc->getVectDocument() ;
  DocumentIter iter = pDocuments->TrouveDocHisto(sCodeDoc) ;
  if ((pDocuments->end() != iter) && ((DocumentIter) NULL != iter))
  {
    delete *iter ;
    pDocuments->erase(iter) ;
  }
}

void
NSTreeHistorique::XmlExport()
{
  NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  pContexte->getPatient()->DocumentXmlExport(pDocument) ;

/*
  string sXmlContent = string("<Document>\r\n") ;

  if (pDocument->pMeta)
  {
    sXmlContent += string("  <LabelTree>\r\n") ;
    sXmlContent += pDocument->pMeta->genereXML() ;
    sXmlContent += string("  </LabelTree>\r\n") ;
  }

  if (pDocument->pPatPathoArray)
  {
    sXmlContent += string("  <DataTree>\r\n") ;
    sXmlContent += pDocument->pPatPathoArray->genereXML() ;
    sXmlContent += string("  </DataTree>\r\n") ;
  }

  sXmlContent += string("</Document>\r\n") ;

  string sExportPath = pContexte->PathName("EHTM") ;
  string sDocId      = pDocument->getID() ;
  string sFileName   = sExportPath + nomSansDoublons(sExportPath, sDocId, string("xml")) ;

  ofstream outFile ;
  outFile.open(sFileName.c_str()) ;
	if (!outFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
	}

  outFile << sXmlContent ;

  outFile.close() ;

  string sMessage = pContexte->getSuperviseur()->getText("exportManagement", "ExportSucceededInFile") + string(" ") + sFileName ;
  string sCaption = pContexte->getSuperviseur()->getAppName().c_str();
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_OK);
*/
}

void
NSTreeHistorique::Transfert()
{
try
{
  string ps = "Entering the document transfert function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  ps = "Document selected for transfert: " + pDocument->getDocName() ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  // Already open ; leaving
  //
	if (_pDoc->EstUnDocumentOuvert(pDocument))
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "youMustCloseTheDocumentBeforeProceeding") ;
    erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;

    ps = "Document is open. Leaving the document transfert function" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
    return ;
  }

  NsFindOtherPatientDialog FindDialog(this, pContexte, this) ;
  FindDialog.Execute() ;

  ps = "Leaving the document transfert function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTreeHistorique::Transfert : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception NSTreeHistorique::Transfert.", standardError, 0) ;
}
}

void
NSTreeHistorique::TransfertStart(NsFindOtherPatientDialog* pDialog)
{
  if (((NsFindOtherPatientDialog*) NULL == pDialog) ||
                               ((NSPatInfo*) NULL == pDialog->_pPatEncours))
    return ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string ps = "NSTreeHistorique::TransfertStart : entering" ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  NSPersonGraphManager* pGraphManager = pDialog->_pPatEncours->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return ;

	NSLinkManager* pLink = pGraphManager->getLinkManager() ;

  string sNewPatCode = pDialog->_pPatEncours->getNss() ;

  ps = "NSTreeHistorique::TransfertStart : ID of target person for transfer: " + sNewPatCode ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  string sTreeID ;
  string sCodeDocMeta ;
  string sCodeDocPres ;

  // Create a Contribution
  //

	// Adding Metas to the Graph Manager
  //
  NSPatPathoArray MetaPpt(pSuper) ;
  pDocument->initFromMeta(&MetaPpt) ;

	if (false == MetaPpt.empty())
  {
    ps = "NSTreeHistorique::TransfertStart : transfering meta" ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

    sCodeDocMeta = pGraphManager->getDataGraph()->getNextTreeID() ;

    // Allocating new patient Id and new Document Ids to MetaPpt nodes
    for (PatPathoIter i = MetaPpt.begin() ; MetaPpt.end() != i ; i++)
    {
      (*i)->setPerson(sNewPatCode) ;
      (*i)->setDocum(sCodeDocMeta) ;
    }
    pGraphManager->setTree(&MetaPpt, pDocument->getRights(), sCodeDocMeta) ;

    //
	  // Linking the meta-document with root doc	  //	  string sNodeRoot = pGraphManager->getRootTree() ;	  if (sCodeDocMeta != sNodeRoot)	  {  	  if (false == pLink->etablirLien(sNodeRoot, NSRootLink::personDocument, sCodeDocMeta))      {
        string sErrorText = pSuper->getText("documentManagementErrors", "cannotEstablishALinkWithTheRoot") ;
    	  pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
			  erreur(sErrorText.c_str(), standardError, 0) ;
        return ;
      }    }    if (false == pGraphManager->commitGraphTree(pContexte, sCodeDocMeta))    {      ps = "NSTreeHistorique::TransfertStart : error when transfering meta, leaving" ;      pSuper->trace(&ps, 1, NSSuper::trDetails) ;      return ;    }    ps = "NSTreeHistorique::TransfertStart : meta successfuly transfered" ;    pSuper->trace(&ps, 1, NSSuper::trDetails) ;  }  // On crée ici l'arbre de présentation dans le graphe  // (il sera enregistré dans enregistrePatPatho)
  //
  NSPatPathoArray PresPpt(pSuper) ;
  pDocument->initFromPres(&PresPpt) ;

  if (false == PresPpt.empty())
  {
    ps = "NSTreeHistorique::TransfertStart : transfering presentation data" ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  	sCodeDocPres = pGraphManager->getDataGraph()->getNextTreeID() ;

    // Allocating new patient Id and new Document Ids to MetaPpt nodes
    for (PatPathoIter i = PresPpt.begin() ; PresPpt.end() != i ; i++)
    {
      (*i)->setPerson(sNewPatCode) ;
      (*i)->setDocum(sCodeDocPres) ;
    }
    pGraphManager->setTree(&PresPpt, "", sCodeDocPres) ;

    // On crée directement le lien du méta vers son arbre de présentation
    // car on a déjà placé sa patpatho dans le graphe - à la différence des datas
    if (false == pLink->etablirLien(sCodeDocMeta, NSRootLink::docComposition, sCodeDocPres))
    	erreur("Impossible d'initialiser le lien du document vers ses données de présentation.", standardError, 0) ;

    if (false == pGraphManager->commitGraphTree(pContexte, sCodeDocPres))
    {
      ps = "NSTreeHistorique::TransfertStart : error when transfering presentation data" ;
      pSuper->trace(&ps, 1, NSSuper::trDetails) ;
      return ;
    }

    ps = "NSTreeHistorique::TransfertStart : presentation tree successfuly transfered" ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
	}

  ps = "NSTreeHistorique::TransfertStart : transfering data tree" ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  sTreeID = pGraphManager->getDataGraph()->getNextTreeID() ;

  NSPatPathoArray DataPpt(pSuper) ;
  pDocument->initFromPatPatho(&DataPpt) ;
  if (false == DataPpt.empty())
  {
    // Allocating new patient Id and new Document Ids to MetaPpt nodes
    for (PatPathoIter i = DataPpt.begin() ; DataPpt.end() != i ; i++)
    {
      (*i)->setPerson(sNewPatCode) ;
      (*i)->setDocum(sTreeID) ;
    }
    pGraphManager->setTree(&DataPpt, "", sTreeID) ;
  }

  if (false == pLink->etablirLien(sCodeDocMeta, NSRootLink::docData, sTreeID))
  {
    erreur("Impossible d'initialiser le lien vers le document data.", standardError, 0) ;
    return ;
  }

  if (false == pGraphManager->commitGraphTree(pContexte, sTreeID))
  {
    ps = "NSTreeHistorique::TransfertStart : error when transfering data tree" ;
    pSuper->trace(&ps, 1, NSSuper::trDetails) ;
    return ;
  }
  ps = "NSTreeHistorique::TransfertStart : data tree transfered" ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // Unlocking target patient
  //
  pDialog->_pPatEncours->debloquer() ;

  // Destruction de l'ancien document
  //
  NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
	Docum.setData(&InfoData) ;

  NSPatPathoArray PptMeta(pSuper) ;
  pDocument->initFromMeta(&PptMeta) ;
	Docum.setMeta(&PptMeta) ;

	Docum.setCodeDocMeta(pDocument->getCodeDocMeta()) ;

	// Destruction du document	// Cette méthode rend le document invisible et remplit
	// le champ pDonnees->tranDel pour le signaler détruit
	pContexte->getPatient()->DetruireDocument(&Docum) ;
	// on appelle rafraichir pour enlever le document de l'historique
	_pDoc->Rafraichir(&Docum, 0) ;
}

void
NSTreeHistorique::Reassign()
{
try
{
  string ps = "Entering the document signature forcing function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSDocumentHisto* pDocument = getSelectedDocument() ;
  if (((NSDocumentHisto*) NULL == pDocument) ||
                        ((NSPersonArray*) NULL == pContexte->getPersonArray()))
		return ;

  // Already open ; leaving
  //
	if (_pDoc->EstUnDocumentOuvert(pDocument))
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "youMustCloseTheDocumentBeforeProceeding") ;
    erreur(sErrorText.c_str(), warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;

    ps = "Document is open. Leaving the signature forcing function" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
    return ;
  }

  ps = "Document selected for signature forcing: " + pDocument->getDocName() ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSDocumentInfo DocInfo(pContexte) ;

  NSDocumentData InfoData ;
  pDocument->initFromData(&InfoData) ;
	DocInfo.setData(&InfoData) ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  pDocument->initFromMeta(&PptMeta) ;
	DocInfo.setMeta(&PptMeta) ;

	DocInfo.setCodeDocMeta(pDocument->getCodeDocMeta()) ;

  DocInfo.ParseMetaDonnees() ;

  string sPreviousAuthor = DocInfo.getCreator() ;

  NSPersonInfo* pPerson = (NSPersonInfo*) 0 ;
  if (string("") != sPreviousAuthor)
    pPerson = pContexte->getPersonArray()->getPerson(pContexte, sPreviousAuthor, pidsUtilisat) ;

  // Confirmation
  //
  string MsgHdr = pContexte->getSuperviseur()->getText("generalLanguage", "warning") ;
  string MsgTxt = pContexte->getSuperviseur()->getText("documentManagement", "areYouSureYouWantToSignThisDocumentInsteadOf") ;
  if (pPerson)
    MsgTxt += string(" ") + pPerson->getCivilite() ;
  else
    MsgTxt += string(" ????") ;

  int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), MsgTxt.c_str(), MsgHdr.c_str(), MB_YESNO) ;
  if (IDYES != retVal)
  {
    ps = "Leaving the signature forcing function (Canceled)" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
    return ;
  }

  // Action
  //
  DocInfo.setCreator(pContexte->getUtilisateurID()) ;
  DocInfo.CommitMetaDonnees() ;

  // Recording changes
  //
  NSNoyauDocument DocMeta((TDocument*) 0, &DocInfo, 0, pContexte, false, true) ;
  DocMeta.setCodeDocMeta(DocInfo.getCodeDocMeta()) ;
  if (false == DocMeta.enregistrePatPatho())
  {
    ps = "Signature forcing function: enregistrePatPatho failed" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trError) ;
    return ;
  }

  // Reloading updated information
  //
  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return ;

	string sRosace = string("") ;
	pGraphManager->getTree(DocInfo.getCodeDocMeta(), &PptMeta, &sRosace) ;
  DocInfo.initFromMeta(&PptMeta) ;

  _pDoc->Rafraichir(&DocInfo, 0) ;

  ps = "Leaving the signature forcing function" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTreeHistorique::Transfert : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception NSTreeHistorique::Transfert.", standardError, 0) ;
}
}

void
NSTreeHistorique::CreateFrom()
{
try
{
  string ps = "Creating new document from previous one" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  NSDocumentHisto* pDocument = getSelectedDocument() ;
  if ((NSDocumentHisto*) NULL == pDocument)
		return ;

  if (string("ZCN00") != pDocument->getTypeSem())
    return ;

  NSCRDocument* pDocNewCr = new NSCRDocument(pDocument, (TDocument*) 0, pContexte) ;
  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(pDocNewCr, "CN Format") ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSTreeHistorique::CreateFrom : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception NSTreeHistorique::CreateFrom.", standardError, 0) ;
}
}

bool
NSTreeHistorique::isManagementDoc(string sDocRoot)
{
  return pContexte->getSuperviseur()->isManagementDoc(sDocRoot) ;
}

/**************************************************************************/
//						boîte de dialogue pour le changement de titre
/*************************************************************************/

/**********************************************************************/
//  permettre l'édition de champ numérique pour un NSTreeNode
/**********************************************************************/
DEFINE_RESPONSE_TABLE1(NSTitre, TDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSTitre::NSTitre(TWindow* parent, TResId ressource, NSContexte* pCtx, string Titre )
        :TDialog(parent, ressource), NSRoot(pCtx)
{
	sTitre = Titre ;
	//
	// Création de tous les "objets de contrôle"
	//
	pTitre = new OWL::TEdit(this, IDC_TITRE) ;
}

NSTitre::~NSTitre()
{
	delete pTitre ;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void
NSTitre::CmOk()
{
  char far* titre = new char[100] ;
  pTitre->GetText(titre, 100) ;
  sTitre = string(titre) ;
  delete[] titre ;

  if (string("") == sTitre)
  {
  	erreur("votre titre n'est pas valide", standardError, 0, GetHandle()) ;
    pTitre->SetFocus() ;
    return ;
	}
	CloseWindow(IDOK) ;
}

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSTitre::CmCancel()
{
	Destroy(IDCANCEL) ;
}

voidNSTitre::SetupWindow()
{
	TDialog::SetupWindow() ;
	pTitre->SetText(sTitre.c_str()) ;
}

// -----------------------------------------------------------------
//
//  NsFindOtherPatientDialog methods
//
// -----------------------------------------------------------------
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NsFindOtherPatientDialog, NSUtilDialog)
	EV_COMMAND(IDC_GET_PATIENT, GetPatient),
  EV_COMMAND(IDC_REASSIGN,    Reassign),
END_RESPONSE_TABLE;

NsFindOtherPatientDialog::NsFindOtherPatientDialog(TWindow* pere, NSContexte* pCtx, NSTreeHistorique* pHistoTree)
                         :NSUtilDialog(pere, pCtx, "REASSIGN_DOC")
{
	_pNom            = new OWL::TStatic(this, NAME_STATIC) ;
  _pPrenom         = new OWL::TStatic(this, NAME2_STATIC) ;

  _pNomTxt         = new OWL::TStatic(this, IDC_NAME_TXT) ;
  _pPrenomTxt      = new OWL::TStatic(this, IDC_NAME2_TXT) ;

  _pLookForPatient = new OWL::TButton(this, IDC_GET_PATIENT) ;
  _pReassignDoc    = new OWL::TButton(this, IDC_REASSIGN) ;

  _sPatCode    = string("") ;
  _pHistoTree  = pHistoTree ;
  _pPatEncours = (NSPatInfo*) 0 ;
}

NsFindOtherPatientDialog::~NsFindOtherPatientDialog()
{
	delete _pNom ;
	delete _pPrenom ;

  delete _pNomTxt ;
	delete _pPrenomTxt ;

	delete _pLookForPatient ;
	delete _pReassignDoc ;
}

void
NsFindOtherPatientDialog::SetupWindow()
{
  string ps = "NsFindOtherPatientDialog : entering SetupWindow" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	NSUtilDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("dialog_patientList", "famillyName") ;
  _pNomTxt->SetText(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("dialog_patientList", "givenName") ;
  _pPrenomTxt->SetText(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("documentReassignement", "lookForOtherPatient") ;
  _pLookForPatient->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("documentReassignement", "reassignDocument") ;
  _pReassignDoc->SetCaption(sText.c_str()) ;

  // ps = "NsFindOtherPatientDialog : hidding the OK button" ;
  // pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  // TWindow* pControl = ChildWithId(IDOK) ;
  // if (NULL != pControl)
	//	pControl->Show(SW_HIDE) ;

  ps = "NsFindOtherPatientDialog : leaving SetupWindow" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}

void
NsFindOtherPatientDialog::GetPatient()
{
try
{
  if (_pPatEncours)
    delete _pPatEncours ;

  _pPatEncours = new NSPatInfo(pContexte) ;

  NSNTiersListePatDialog* pPatDlg = new NSNTiersListePatDialog(pContexte->GetMainWindow(), pContexte, _pPatEncours) ;

  int idRetour = pPatDlg->Execute() ;

  delete pPatDlg ;

  if (IDOK != idRetour)
    return ;

  _sPatCode = _pPatEncours->getNss() ;
  pContexte->getUtilisateur()->OuvrePatient(_sPatCode, _pPatEncours) ;

  _pNom->SetCaption(_pPatEncours->getNom().c_str()) ;
  _pPrenom->SetCaption(_pPatEncours->getPrenom().c_str()) ;

  if (true == _pPatEncours->getGraphPerson()->getReadOnly())
  {
    string sCaption = pContexte->getSuperviseur()->getAppName() ;
    string sWarnText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    MessageBox(sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
  }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NsFindOtherPatientDialog::GetPatient : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch(...)
{
	erreur("Exception NsFindOtherPatientDialog::GetPatient.", standardError, 0) ;
}
}

void
NsFindOtherPatientDialog::Reassign()
{
try
{
  if (true == _pPatEncours->getGraphPerson()->getReadOnly())
  {
    string sCaption = pContexte->getSuperviseur()->getAppName() ;
    string sWarnText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    MessageBox(sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
    return ;
  }

  HCursor = ::SetCursor(::LoadCursor(0, IDC_WAIT)) ;
  _pHistoTree->TransfertStart(this) ;
  HCursor = ::SetCursor(::LoadCursor(0, IDC_ARROW)) ;

  // TWindow* pControlOk = ChildWithId(IDOK) ;
  // if (NULL != pControlOk)
	// 	pControlOk->Show(SW_SHOW) ;

  // TWindow* pControlCancel = ChildWithId(IDCANCEL) ;
  // if (NULL != pControlCancel)
	// 	pControlCancel->Show(SW_HIDE) ;

  CloseWindow(IDOK) ;
}
catch(...)
{
	erreur("Exception NsFindOtherPatientDialog::GetPatient.", standardError, 0) ;
}
}

