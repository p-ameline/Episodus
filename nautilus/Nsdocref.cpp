//----------------------------------------------------------------------------// NSRefDocument : Père des documents NAUTILUS, il permet de leur attacher un
//						 pointeur sur un DocumentInfo.
//----------------------------------------------------------------------------
#define __NSDOCREF_CPP

#include <owl\docmanag.h>
#include <stdio.h>
#include <classlib\filename.h>

#include "nssavoir\nspathor.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "partage\nsperson.h"
#include "nautilus\nshistdo.h"	// pour rafraichir l'historique
#include "nsdn\nsdocdlg.h"   // pour EnregDocDialog
#include "nsdn\nsintrad.h"   
#include "nautilus\nsmodhtm.h"	// pour InitIntitule
#include "nautilus\nscompub.h"   // pour composer et publier
#include "nautilus\nsvisual.h"	// pour visualiser
#include "nautilus\nsbrowse.h"  // pour composer
#include "nautilus\nsldvdoc.h"
#include "nscompta\nscpta.h"    // pour ComptaAuto
#include "nsbb\nstlibre.h"      // pour l'importation des textes libres
#include "nsbb\nsbbsmal.h"
#include "nssavoir\nsgraphe.h"  // pour la création des liens#include "nautilus\nsdocref.h"
#include "nsepisod\nssoapdiv.h"
#include "dcodeur\decoder.h"
#include "nautilus\nssoapview.h"
#include "nautilus\nsdocview.h"
#include "export\html2word.h"
#include "nsoutil\nsHtmlToPdf.h"
#include "nsoutil\nsoutdlg.h"

long NSRefDocument::lObjectCount = 0 ;

NSPrintControl::NSPrintControl()
{
  _sNomDocHtml     = string("") ;
  _nbImpress       = 0 ;
  _bImprimerLettre = false ;
  _sCodeLettre     = string("") ;  _indexCorresp    = -1 ;
}

// --------------------------------------------------------------------------
// --------------------- METHODES DE NSRefDocument --------------------------
// --------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Constructeur
//
//  parent 	  	   : TDocument parent (?)
//	 pDocumentInfo : pointeur sur l'objet NSDocumentInfo qui
//						  permet de référencer le NSDocument
//	 pSuper		   : pointeur sur le superviseur
//---------------------------------------------------------------------------
NSRefDocument::NSRefDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
                             NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                             bool bROnly)
              :NSNoyauDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
  lObjectCount++ ;

  _pBigBoss = (NSSmallBrother*) 0 ;
  _pPubli   = (NSPublication*) 0 ;

  _sCodeChemise = string("") ;
  _sUrlHtml     = string("") ;
}

NSRefDocument::NSRefDocument(TDocument* parent, NSContexte* pCtx)              :NSNoyauDocument(parent, pCtx)
{
  lObjectCount++ ;

	_pBigBoss = (NSSmallBrother*) 0 ;
	_pPubli   = (NSPublication*) 0 ;

  _sCodeChemise = string("") ;
  _sUrlHtml     = string("") ;
}

//---------------------------------------------------------------------------//   Destructeur
//---------------------------------------------------------------------------
NSRefDocument::~NSRefDocument()
{
	if (pContexte && pContexte->getPatient())
	{
  	NSPatientChoisi* pPatient = pContexte->getPatient() ;

		if (_pHtmlInfo)
		{
			if ((pPatient->getDocHis()) &&
                         pPatient->getDocHis()->EstUnDocumentOuvert(_pHtmlInfo))
				pPatient->getDocHis()->FermetureDocument(_pHtmlInfo) ;
		}
		// on n'utilise pas l'historique pour les documents compta (CP) et publication (PB)
		// ainsi que pour le document de publication de l'historique (HISTO)
		else if ((_pDocInfo) && (string("") != _pDocInfo->getTypeSem()) &&
                        (string("CP") != string(_pDocInfo->getTypeSem(), 0, 2)) &&
                        (string("PB") != string(_pDocInfo->getTypeSem(), 0, 2)) &&
                        (string("HISTO") != _pDocInfo->getTypeSem()))
		{
			if ((false == _bIsClone) && (pPatient->getDocHis()) &&
                      pPatient->getDocHis()->EstUnDocumentOuvert(_pDocInfo))
				pPatient->getDocHis()->FermetureDocument(_pDocInfo) ;
		}
	}

	if (_pBigBoss)
		delete _pBigBoss ;
	if (_pPubli)
  	delete _pPubli ;

  lObjectCount-- ;
}

// Constructeur copieNSRefDocument::NSRefDocument(const NSRefDocument& rv)
							:NSNoyauDocument(rv.GetParentDoc(), rv._pDocInfo, rv._pHtmlInfo, rv.pContexte, rv.isReadOnly())
{
  lObjectCount++ ;

  if (rv._pBigBoss)
    _pBigBoss = new NSSmallBrother(*(rv._pBigBoss)) ;
  else
    _pBigBoss = (NSSmallBrother*) 0 ;

  _pPubli   = (NSPublication*) 0 ;

  _printControl = rv._printControl ;
}

// Operateur =// Pb : comment affecter le document parent destination ?????????????????
NSRefDocument&
NSRefDocument::operator=(const NSRefDocument& src)
{
	if (this == &src)
		return *this ;

	NSNoyauDocument* pNoyau = (NSNoyauDocument*) this ;
	*pNoyau = (NSNoyauDocument) src ;

  if (_pBigBoss)
    delete _pBigBoss ;
  if (src._pBigBoss)
	  _pBigBoss = new NSSmallBrother(*(src._pBigBoss)) ;
  else
    _pBigBoss = (NSSmallBrother*) 0 ;

	_pPubli    = (NSPublication*) 0 ;

	_printControl = src._printControl ;

	return *this ;
}

// Fonction qui renvoie la date stockée dans la PatPatho
// au format jj/mm/aaaa ou au format Nautilus (aaaammjj) selon bDateClaire
//
string
NSRefDocument::GetDateDoc(bool bDateClaire)
{
try
{
	string sDate = string("") ;
	string sLang = pContexte->getUserLanguage() ;

	if (_pDocInfo)
	{
		sDate = _pDocInfo->getDateExm() ;

		if (string("") == sDate)
			sDate = _pDocInfo->getCreDate() ;
	}

	if ((false == bDateClaire) || (string("") == sDate))
		return sDate ;

  string sMessage = string("") ;
	string sIntro   = string("") ;

	donne_date_claire(sDate, &sMessage, &sIntro, sLang) ;

	if (string("") != sIntro)
		sDate = sIntro + string(" ") + sMessage ;
	else
		sDate = sMessage ;

	return sDate ;
}
catch (...)
{
	erreur("Exception NSRefDocument::GetDateDoc.", standardError, 0) ;
	return string("") ;
}
}

// Get the second operator, if any
//
string
NSRefDocument::GetSecondOperator()
{
try
{
	if (_pDocInfo)
    return _pDocInfo->get2ndOper() ;
    
	return string("") ;
}
catch (...)
{
	erreur("Exception NSRefDocument::GetSecondOperator.", standardError, 0) ;
	return string("") ;
}
}

// Modele de la fonction GenereHtml// Cette méthode ne sert que pour les html statiques (ou documents images)// A priori ce type de document ne se compose pas et n'a pas de pHtmlInfobool
NSRefDocument::GenereHtml(string sPathHtml, string& sNomHtml, OperationType typeOperation,
									string sAdresseCorresp, string sPathDest)
{
	NSModHtml html(typeOperation, this, pContexte) ;
	string    sBaseImg ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = html.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

	// generation du fichier html (dans le repertoire du serveur)
	if (_pHtmlInfo)
	{
		// cas des html dynamiques
		if (false == html.genereHtml(sFichierHtml, sBaseImg, _pHtmlInfo))
			return false ;
	}
	else
	{
		// on fournit ici le pDocInfo car le document brut est un html statique
		if (false == html.genereHtml(sFichierHtml, sBaseImg, _pDocInfo))
			return false ;
	}

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

bool
NSRefDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  return true ;
}

bool
NSRefDocument::GenereRawText(string& sRawText)
{
  sRawText = string("") ;

  return true ;
}

voidNSRefDocument::Composer()
{
try
{
	// char   nomModele[TMPL_FICHIER_LEN + 1] ;
	// char   nomEnTete[TMPL_EN_TETE_LEN + 1] ;
	// char   typeModele[TMPL_TYPE_LEN + 1] ;

  string sNomModele  = string("") ;
  string sNomEntete  = string("") ;
  string sTypeModele = string("") ;

	size_t iNumModele ;
	string sNomHtml ;
	string sPathHtml ;
	string sCodeDoc ;
	bool   bExisteImages = false ;

   	// si pas d'utilisateur ou pas de patient en cours : on sort
	if ((NULL == pContexte->getUtilisateur()) || (NULL == pContexte->getPatient()))
		return ;

	if (NULL == _pDocInfo)
	{
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    MessageBox(0, "Vous devez enregistrer le document avant de le composer", sCaption.c_str(), MB_OK) ;
    return ;
	}

	// test préalable : la chemise contient-elle des images ?
	if (!ChemiseAvecImages(bExisteImages))
	{
  	erreur("Impossible de déterminer s'il existe des images dans la chemise du document.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
	}

	if (false == bExisteImages)
	{
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    int idRet = MessageBox(0, "La chemise du document ne contient pas d'images. Voulez-vous continuer ?", sCaption.c_str(), MB_YESNO) ;
    if (idRet == IDNO)
    	return ;
	}

	sCodeDoc = _pDocInfo->getID() ;
	setNomDocHtml(string("")) ;

   	/**************************** création d'un fichier html simple
   	NSHtml htest;
   	NSHtml hsdp(tTexte);
   	NSHtml *pBody;
   	htest.init("testsdp");
   	pBody = htest.GetBody();
   	hsdp.sTexte = string("\x0C");
   	htest.insere(pBody,&hsdp);
   	htest.ecrire("htest.htm");
   	return;
   	**************************************************************/

/*
   	// Cas des documents deja composes (pHtmlInfo != 0)
   	if (pHtmlInfo)
   	{
   		NomCompoDialog* pNomCompoDlg =
      	new NomCompoDialog(pContexte->GetMainWindow(), pContexte);
      	if (pNomCompoDlg->Execute() == IDCANCEL)
      	{
      		delete pNomCompoDlg;
   			return;
      	}
      	sNomDocHtml = pNomCompoDlg->sNomDocHtml;
      	delete pNomCompoDlg;
   	}
*/

	// les HIHTM (Fichiers pannels) sont eux-mêmes des templates
	if (string("ZIHTM") != _pDocInfo->getTypeSem())
	{
		string sCodeLexRoot  = string("") ;
    string sCodeSensRoot = string("") ;

		if (false == _PatPathoArray.empty())
		{
			sCodeLexRoot  = (*(_PatPathoArray.begin()))->getLexique() ;
      sCodeSensRoot = (*(_PatPathoArray.begin()))->getLexiqueSens() ;
		}

		// Dialogue de choix du template
		ChoixTemplateDialog* pChoixTemplateDlg =
		    new ChoixTemplateDialog(pContexte->GetMainWindow(), pContexte, _pDocInfo->getTypeSem(), sCodeSensRoot) ;

		if (IDCANCEL == pChoixTemplateDlg->Execute())
		{
			delete pChoixTemplateDlg ;
			return ;
		}

		iNumModele = pChoixTemplateDlg->getTemplateChoisi() ;

		if (iNumModele > 0)
		{
      int iCursor = iNumModele - 1 ;

      NSTemplateArray* pTemplateArray = pChoixTemplateDlg->getTemplateArray() ;

			sNomModele  = (*pTemplateArray)[iCursor]->getFichier() ;			sNomEntete  = (*pTemplateArray)[iCursor]->getEnTete() ;
			sTypeModele = (*pTemplateArray)[iCursor]->getType() ;
		}
		else
		{
			erreur("Vous devez sélectionner une template", warningError, 0, pContexte->GetMainWindow()->GetHandle());
			return;
		}

		delete pChoixTemplateDlg ;    pChoixTemplateDlg = (ChoixTemplateDialog*) 0 ;
		_sTemplate = pContexte->PathName("NTPL") + sNomModele ;		_sEnTete   = pContexte->PathName("NTPL") + sNomEntete ;
	}
	else
	{
		_sTemplate = pContexte->PathName(_pDocInfo->getLocalis()) + _pDocInfo->getFichier() ;
		_sEnTete = string("") ;
	}

	// On récupère le chemin où on va générer le fichier de composition	sPathHtml = pContexte->PathName("SHTM") ;
	sNomHtml  = sCodeDoc ;

	if (false == GenereHtml(sPathHtml, sNomHtml, toComposer))	{
		erreur("Impossible de créer le fichier html de composition", standardError, 0, pContexte->GetMainWindow()->GetHandle());
		return;
	}

	// on instancie cette variable pour le navigate de la ComposView
	_sUrlHtml = sNomHtml;

	NSDocViewManager dvManager(pContexte) ;	dvManager.createView(this, "Compos Format") ;}catch (...)
{
	erreur("Exception NSRefDocument::Composer", standardError, 0) ;
}
}

voidNSRefDocument::Publier(bool bCorrespPatient)
{
try
{
	string ps;

	// si pas d'utilisateur ou pas de patient en cours : on sort
	if ((pContexte->getUtilisateur() == 0) ||
   	 	(bCorrespPatient && (pContexte->getPatient() == 0)))
		return ;

	if (NULL == _pDocInfo)
	{
  	string sWarningText = pContexte->getSuperviseur()->getText("documentManagement", "youMustSaveBeforePublishing") ;
		MessageBox(0, sWarningText.c_str(), 0, MB_OK) ;
		return ;
	}

	if (_pPubli)
		delete _pPubli ;

	// on publie le document en cours (pointeur this) avec ou sans corresp
	if (bCorrespPatient)
		_pPubli = new NSPublication(pContexte->getPatient(), this, pContexte) ;
	else
		_pPubli = new NSPublication(0, this, pContexte) ;

	_pPubli->Publier();

	// boucle de synchronisation sur le fin du processus d'impression
	// le processus est débloqué par NSVisualView::CmPublier()
	// le seul probleme c'est que ça ne marche pas : l'impression se bloque
	// si on ne rend pas la main ici
	// while (pPubli->ImpressionEnCours())
	// {
	// on applique un délai de 1/10ème de sec.
	// pContexte->getSuperviseur()->Delay(10);
	// }
}
catch (...)
{
	erreur("Exception NSRefDocument::Publier", standardError, 0) ;
}
}

/**
 Function called in order to open a WYSIWYG visualization of a document
 */
void
NSRefDocument::Visualiser()
{
try
{
  string sMsg = string("Entering NSRefDocument::Visualiser (document ID: \"") + getDocId() + string("\").") ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;

	// si pas d'utilisateur ou pas de patient en cours : on sort
	if ((pContexte->getUtilisateur() == 0) || (pContexte->getPatient() == 0))
  {
    sMsg = string("No user or patient, leaving.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return ;
  }

	if ((NSDocumentInfo*) NULL == _pDocInfo)	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "youMustSaveBeforeVisualization") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return ;
	}

  NSHISTODocument* pDocHis = pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pDocHis)
	{
  	string sErrorText = string("NSRefDocument::Visualiser: Error, null histo document.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return ;
	}

	// S'il existe déjà une visualisation du document, on l'active et on sort
	if (pDocHis->ActiveFenetre(_pDocInfo, "NSVisualView"))
  {
    sMsg = string("Document already opened, just activated its window.") ;
    pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;
		return ;
  }

  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;
  if ((NSHISTODocument*) NULL == pGraphPerson)
	{
  	string sErrorText = string("NSRefDocument::Visualiser: Error, null person graph.") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return ;
	}

	pGraphPerson->getTemplatePres(_pDocInfo->getCodeDocPres(), _sTemplate, _sEnTete) ;

	if ((string("") == _sTemplate) || (string("") == _sEnTete))
	{
		if (pContexte->typeDocument(_pDocInfo->getTypeSem(), NSSuper::isTree))
		{
    	// s'il existe une template et un en-tete non vides
			// on les récupère, sinon on prend les fichiers par défaut
			_sTemplate = TemplateInfoBrut(false) ;
			_sEnTete   = EnTeteInfoBrut(false) ;

			if ((string("") == _sTemplate) || (string("") == _sEnTete))
				TemplateCompo(_pDocInfo->getTypeSem(), _sTemplate, _sEnTete) ;		}
		else
			TemplateCompo(_pDocInfo->getTypeSem(), _sTemplate, _sEnTete) ;
	}

	if (string("") != _sTemplate)		_sTemplate = pContexte->PathName("NTPL") + _sTemplate ;
	else
		_sTemplate = string("") ;

	if (string("") != _sEnTete)      // cas important lorsqu'on n'a pas d'en-tete		_sEnTete = pContexte->PathName("NTPL") + _sEnTete ;
	else
		_sEnTete = string("") ;

	setNbImpress(0) ;  sMsg = string("Asking the DocViewManager to open a new window.") ;  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trSubDetails) ;  NSDocViewManager dvManager(pContexte) ;	dvManager.createView(this, "Visual Format") ;}
catch (...)
{
  string sErrorText = string("Exception NSRefDocument::Visualiser.") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
	erreur(sErrorText.c_str(), standardError, 0) ;
}
}

/**
* Exporting the document in a specified file format
*/
bool
NSRefDocument::exportFile(string sFileName, string sFileType)
{
	// s'il existe une template et un en-tete non vides
	// on les récupère, sinon on prend les fichiers par défaut

	string sFichTmpl   = TemplateInfoBrut(false) ;
	string sEnteteTmpl = EnTeteInfoBrut(false) ;

	if ((string("") == sFichTmpl) || (string("") == sEnteteTmpl))
  {
    bool bVerbose = true ;
    if (_pDocInfo && (string("ZSHTM") == _pDocInfo->getTypeSem()))
      bVerbose = false ;
		TemplateCompo(_pDocInfo->getTypeSem(), sFichTmpl, sEnteteTmpl, bVerbose) ;  }
	if (string("") != sFichTmpl)
		_sTemplate = pContexte->PathName("NTPL") + sFichTmpl ;
	else
		_sTemplate = string("") ;

	if (string("") != sEnteteTmpl)
		_sEnTete = pContexte->PathName("NTPL") + sEnteteTmpl ;
	else
		_sEnTete = string("") ;

	string sPathExport = pContexte->PathName("EHTM") ;
	string sNameExport = string("temp") ;

	if (false == GenereHtml(sPathExport, sNameExport, toExporter, "", sPathExport))
		return false ;

  // Required output is a HTML file
  //
	if (string("HTM") == sFileType)
	{
    // Static HTML, just copy stored file
    //
    if (_pDocInfo && (string("ZSHTM") == _pDocInfo->getTypeSem()))
    {
      string sFile = pContexte->PathName(_pDocInfo->getLocalis()) + _pDocInfo->getFichier() ;
      if (0 == ::CopyFile(sFile.c_str(), sFileName.c_str(), FALSE))
      {
        string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") ;
        sErrorText += string(" ") + sFile + string(" -> ") + sFileName ;

        string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
        if (string("") != sSystemLastErrorMessage)
          sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), warningError, 0, 0) ;

        return false ;
      }
    }
    else
    {
		  string sTempFileName = sPathExport + sNameExport ;
      // if (0 == MoveFile(sTempFileName.c_str(), sFileName.c_str()))
      if (0 == MoveFileEx(sTempFileName.c_str(), sFileName.c_str(), MOVEFILE_REPLACE_EXISTING))
      {
        string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorMovingFile") ;
        sErrorText += string(" ") + sTempFileName + string(" -> ") + sFileName ;

        string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
        if (string("") != sSystemLastErrorMessage)
          sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), warningError, 0, 0) ;

        return false ;
      }
    }
		return true ;
	}

	//
	// Export as a word file
	//
	if (string("DOC") == sFileType)
	{
/*
		char* pNomLibre ;
		DonneNomFichier("nau", "doc", sPathExport.c_str(), &pNomLibre) ;
		string sNomFich = string(pNomLibre) ;
		delete[] pNomLibre ;
*/
		//
		// Document
		//
    string sTempFileName = sPathExport + sNameExport ;

		NSTtxDocumentExp* pTxtDoc = new NSTtxDocumentExp(NULL, pContexte) ;
		pTxtDoc->SetNomFichier(sTempFileName) ;

		//
		// View
		//
		NSAutoWordViewExp* pAutoView = new NSAutoWordViewExp(*pTxtDoc) ;

		//
		// Doc/View management
		//
		NSDocViewManager dvManager(pContexte) ;
    dvManager.createView(pTxtDoc, "HTML to Word Format", pAutoView) ;

    pAutoView->saveHTMLtoWORD(sFileName) ;

    NsDeleteTemporaryFile(pContexte->getSuperviseur(), sTempFileName) ;

    return true ;
	}

  //
	// Export as a pdf file
	//
	if (string("PDF") == sFileType)
	{
    string sTempFileName   = sPathExport + sNameExport ;
    string sTargetFileName = sFileName ;

    // NSHtmlToPdfConverter Converter(pContexte->getSuperviseur()) ;
    // if (Converter.bIsReady())
    //   Converter.convertHtmlToPdf(sTempFileName, sTargetFileName) ;

    NSSuper* pSuper = pContexte->getSuperviseur() ;
    NSHtmlToPdfConverter* pHtmlToPdfConverter = pSuper->getHtmlToPdfConverter() ;
    if ((NSHtmlToPdfConverter*) NULL == pHtmlToPdfConverter)
	  {
      string sErrorText = string("Can't initialize Html to Pdf converter") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      NsDeleteTemporaryFile(pSuper, sTempFileName) ;
      return false ;
	  }

    if (pHtmlToPdfConverter->bIsReady())
      pHtmlToPdfConverter->convertHtmlToPdf(sTempFileName, sTargetFileName) ;

    NsDeleteTemporaryFile(pSuper, sTempFileName) ;

    return true ;
  }

	return false ;
}

//-------------------------------------------------------------------------------------//  Fonction:		NSRefDocument::ComptaAuto()
//  Description:	Vérifie s'il existe une fiche compta pour un document du meme type
//  Retour:			True s'il n'en existe pas, False sinon
//-------------------------------------------------------------------------------------
bool NSRefDocument::ComptaAuto()
{
try
{
	return true ;
}
catch (...)
{
	erreur("Exception NSRefDocument::ComptaAuto.", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------//  Fonction:		NSRefDocument::CreeDocument()
//  Description:	Crée et référence un document en mode N_TIERS
//  Retour:			True si ça réussit, False sinon
//---------------------------------------------------------------------------
bool
NSRefDocument::CreeDocument(NSDocumentInfo* pDoc, int typeLink,
                            string &sCodeChemise, bool bVerbose,
                            NSPersonGraphManager* pGraphManager)
{
try
{
  return NSNoyauDocument::CreeDocument(pDoc, typeLink, sCodeChemise, bVerbose,
                                       pGraphManager) ;
}
catch (...){
  erreur("Exception NSRefDocument::CreeDocument.", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------------------//  Fonction:		NSRefDocument::ParamDoc()
//  Description:	Permet de modifier les paramètres (titre, date, importance)
//                  d'un document.
//  Retour:			True si ça réussit, False sinon
//---------------------------------------------------------------------------
bool
NSRefDocument::ParamDoc()
{
try
{
  string ps = "Entering NSRefDocument::ParamDoc()" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

	string sNumChemise = string("") ;

	NSDocumentInfo paramInfo(pContexte) ;

	if (_pHtmlInfo)
  	paramInfo = *_pHtmlInfo ;
	else if (_pDocInfo)
		paramInfo = *_pDocInfo ;
	else
	{
		erreur("Document non initialisé.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	if (_pHtmlInfo)
	{
		erreur("Ce document est un document composé. Vous ne pouvez pas modifier ses paramètres.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	// Note : Dans paramInfo, on remplace codeDocument par le code du meta
	// ce qui permet de voir si on a un lien systeme (IsDocRoot) et permet
	// aussi d'appeler EnregDocDialog avec le code du meta, pour rester
	// cohérent avec Referencer() et pour pouvoir récupérer la chemise du document
	string sNewCodeDoc = string(_pDocInfo->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
	paramInfo.setDocument(sNewCodeDoc) ;

	string sCodeDoc = paramInfo.getID() ;
	if (pContexte->getPatient()->IsDocRoot(sCodeDoc))
	{
		erreur("Ce document est un document système. Vous ne pouvez pas modifier ses paramètres.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	//
	// Appel de la boite de dialogue de référencement
	//
	string         sRights = paramInfo.getRights() ;

  NSDocumentData DocData ;
  paramInfo.initFromData(&DocData) ;

  int iRetVal = 0 ;

  if (string("") != DocData._sTypeContenu)
  {
	  EnregDocDialog EnregDocDl(pContexte->GetMainWindow(), &DocData,
        									                sNumChemise, sRights, pContexte) ;
	  iRetVal = EnregDocDl.Execute() ;
  }
  else
  {
	  EnregDocDialogCombo EnregDocDl(pContexte->GetMainWindow(), &DocData,
        									                sNumChemise, sRights, pContexte) ;
	  iRetVal = EnregDocDl.Execute() ;
  }

	if (IDCANCEL == iRetVal)
		return false ;

  paramInfo.setData(&DocData) ;

	//
	// Vérification de la présence des éléments obligatoires
	//
  if ((string("") == paramInfo.getDocName()) ||
		  (string(strlen(paramInfo.getDocName().c_str()), ' ') == paramInfo.getDocName()))
		return false ;

	if (string("") == sNumChemise)
		return false ;

  NSProgressDlg progressDialog(pContexte->GetMainWindow(), pContexte) ;
  progressDialog.Create() ;
  progressDialog.setCaptionText(string("Enregistrement des modifications")) ;
  progressDialog.ShowWindow(SW_SHOW) ;

	string sElemLex, sSens ;

  // on remet ici le codeDocument du data dans paramInfo
  // pour pouvoir comparer les changements directement avec l'operateur ==
  // on doit le faire de toute façon avant de remettre à jour pDocInfo
  //
	paramInfo.setDocument(_pDocInfo->getDocument()) ;

  bool bMustSaveMeta = false ;

  // on regarde maintenant à quelle chemise est lié le document
  // Si cette chemise est différente de la nouvelle, on change le lien
  // Note : la nouvelle chemise ne peut pas être la corbeille (voir EnregDocDialog)
  //
  // Better do it before sending the tree so that it is saved when tree is saved

  string sPreviousNodeChemise = string("") ;
  string sNewNodeChemise      = sNumChemise ;

  string sCodeDocMeta = paramInfo.getCodeDocMeta() ;
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;
  VecteurString VecteurString ;
	pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::docFolder, &VecteurString, "ENVERS") ;
	if (false == VecteurString.empty())
  	sPreviousNodeChemise = *(*(VecteurString.begin())) ;

  if (sPreviousNodeChemise != sNewNodeChemise)
  {
    if (string("") != sPreviousNodeChemise)
      pGraphe->detruireLien(sPreviousNodeChemise, NSRootLink::docFolder, sCodeDocMeta) ;

    ps = "ParamDoc: connecting to folder " + sNumChemise ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

    progressDialog.updateTaskText(string("Connection à la chemise")) ;
    progressDialog.setPosition(10) ;

	  pGraphe->etablirLien(sNewNodeChemise, NSRootLink::docFolder, sCodeDocMeta) ;

    bMustSaveMeta = true ;
  }

  NSNoyauDocument* pDocMeta = 0 ;

  // Now we take into account the other modifications
  //
	if ((false == (paramInfo == *_pDocInfo)) || bMustSaveMeta)
	{
    // on doit ré-enregistrer le document Meta pour changer les paramètres
    // Les paramètres modifiables sont : visible, interet, le nom et la chemise
    //
    NSDocumentInfo DocInfoMeta(pContexte) ;

    // remise à jour du pDocInfoMeta pour charger le document Meta
    // on précise ici uniquement codePatient et codeDocument sans préciser le type
    // on doit ensuite charger la patpatho à la main (cf NSDocumentInfo::ChargeDocMeta())
    //
    DocInfoMeta.setPatient(_pDocInfo->getPatient()) ;
    string sNewCodeDoc = string(_pDocInfo->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
    DocInfoMeta.setDocument(sNewCodeDoc) ;

    progressDialog.updateTaskText(string("Rechargement des données")) ;
    progressDialog.setPosition(20) ;

    // on recharge la patpatho avec les arrays de données
    pDocMeta = new NSNoyauDocument(0, &DocInfoMeta, 0, pContexte, false) ;
    /* bool resultat = */ pDocMeta->chargePatPatho() ;
  }

  // Now we take into account the other modifications
  //
	if (false == (paramInfo == *_pDocInfo))
	{
    ps = "ParamDoc: information modified..." ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

    progressDialog.updateTaskText(string("Modification de l'étiquette")) ;
    progressDialog.setPosition(30) ;

    // on met à jour les droits du document - pour cela on doit accéder au graphe.
    // ceci est fait par un setTree sur le Meta dans Referencer(). Ici on met à
    // jour directement le NSNodeRightArray pour le Meta, qui est envoyé ensuite au pilote.
    NSDataGraph* pDataGraph = pContexte->getPatient()->getGraphPerson()->getDataGraph() ;
    pDataGraph->getRights()->set(_pDocInfo->getCodeDocMeta(), paramInfo.getRights()) ;

    // on remplace les données visible et interet du noeud racine    //    NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;    pDocMeta->initFromPatPatho(&PptMeta) ;    PatPathoIter iterDoc = PptMeta.begin() ;    int iColBase = (*iterDoc)->getColonne() ;    string sTemp ;    (*iterDoc)->setVisible(paramInfo.getVisible()) ;    (*iterDoc)->setInteret(paramInfo.getInteret()) ;    // on remplace le nom (noeud suivant par construction)    iterDoc++ ;    while ((PptMeta.end() != iterDoc) && ((*iterDoc)->getColonne() > iColBase))
    {
    	sElemLex = (*iterDoc)->getLexique() ;
      sSens    = (*iterDoc)->getLexiqueSens() ;

      if (string("0INTI") == sSens)
      {
      	iterDoc++ ;
        string sNom = string("") ;
        while ((PptMeta.end() != iterDoc) && ((*iterDoc)->getColonne() > iColBase+1))
        {
        	// on cherche ici un texte libre
          sElemLex = (*iterDoc)->getLexique() ;
          if (sElemLex == string("£?????"))
          {
          	sNom = (*iterDoc)->getTexteLibre() ;

            if (paramInfo.getDocName() != sNom)
            	(*iterDoc)->setTexteLibre(paramInfo.getDocName()) ;

            break ;
          }

          iterDoc++ ;
        }
      }
      // date de rédaction
      else if (string("KREDA") == sSens)
      {
      	iterDoc++ ;
        // int iLigneBase = (*iter)->pDonnees->getLigne() ;

        string sUnite  = (*iterDoc)->getUnitSens() ;
        string sFormat = (*iterDoc)->getLexiqueSens() ;
        string sValeur = (*iterDoc)->getComplement() ;

        if (string("2DA02") != sUnite)
        	(*iterDoc)->setUnit("2DA021") ;

        if ((string("") == sFormat) || (sFormat[0] != '£') || (sFormat[1] != 'D'))
        	(*iterDoc)->setLexique("£T0;19") ;

        if (strlen(sValeur.c_str()) > 14)
        	sValeur = string(sValeur, 0, 14) ;

        if (paramInfo.getCreDate() != sValeur)
        {
        	string sDateReda = paramInfo.getCreDate() ;
          if (strlen(sDateReda.c_str()) == 8)
          	sDateReda += string("000000") ;
        	(*iterDoc)->setComplement(sDateReda) ;
        }

        iterDoc++ ;
      }
      else
      	iterDoc++ ;
    }

    pDocMeta->setPatPatho(&PptMeta) ;

    bMustSaveMeta = true ;
  }

  if (bMustSaveMeta)
  {
    // on enregistre enfin le meta et on remet à jour paramInfo

    ps = "ParamDoc: information modified... saving the Meta" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

    progressDialog.updateTaskText(string("Enregistrement des données")) ;
    progressDialog.setPosition(40) ;

    if (pDocMeta->enregistrePatPatho())
    	pDocMeta->NSNoyauDocument::chargePatPatho() ;    ps = "ParamDoc: information modified... Meta saved" ;    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;    NSPatPathoArray PptDocMeta(pContexte->getSuperviseur()) ;    pDocMeta->initFromPatPatho(&PptDocMeta) ;    paramInfo.setMeta(&PptDocMeta) ;
  }

	// En dernier lieu on vérifie si il faut restaurer le document
	// (si le document était auparavant détruit)
	string sNodeRoot = pContexte->getPatient()->getNss() + string(1, LocalPatient) + string(DOC_CODE_DOCUM_LEN - 1, '0') ;

	if (false == pGraphe->existeLien(sNodeRoot, NSRootLink::personDocument, sCodeDocMeta))
  {
    ps = "ParamDoc: connecting to Root" ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

  	pGraphe->etablirLien(sNodeRoot, NSRootLink::personDocument, sCodeDocMeta) ;
  }

	// Remise à jour des pDocInfo ou pHtmlInfo en mémoire d'après les paramInfo
  //
	if (_pHtmlInfo)
  	*(_pHtmlInfo) = paramInfo ;
	else if (_pDocInfo)
  	*(_pDocInfo)  = paramInfo ;

  progressDialog.updateTaskText(string("Opération achevée")) ;
  progressDialog.setPosition(100) ;

  progressDialog.Destroy() ;

  ps = "Leaving NSRefDocument::ParamDoc()" ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubSteps) ;

	return true ;
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception NSRefDocument::ParamDoc : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
	return false ;
}
catch (...)
{
	erreur("Exception NSRefDocument::ParamDoc", standardError, 0) ;
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
NSRefDocument::Referencer(string typeDoc, string nomDoc, string nomFichier,
                          string cheminDoc, bool bDocVisible, bool bVerbose,
                          string sCodeDoc, NSRootLink::NODELINKTYPES typeLink,
                          NSPersonGraphManager* pGraphManager,
                          string sAuthorId, string tmplDoc, string enteteDoc,
                          string sDestinataire, string sCreationDate,
                          string sMasterDoc, NSRootLink::NODELINKTYPES masterLink,
                          NSPatPathoArray* pSpecificInfoPatPatho)
{
try
{
  if (false == NSNoyauDocument::Referencer(typeDoc, nomDoc, nomFichier,
                                           cheminDoc, bDocVisible, bVerbose,
                                           sCodeDoc, typeLink, pGraphManager,
                                           sAuthorId, tmplDoc, enteteDoc,
                                           sDestinataire, sCreationDate,
                                           sMasterDoc, masterLink, true,
                                           pSpecificInfoPatPatho))
    return false ;

	// ajout d'un nouveau document type fichier :  // remise à jour de l'historique ...
  // Les documents de type global 'C' (CS ou CN)
  // appellent Rafraichir dans leur fonction enregistrer pour
  // pouvoir passer la patpatho

	if (pContexte->typeDocument(typeDoc, NSSuper::isFile))	{
    if (pContexte->getPatient()->getDocHis())
    	pContexte->getPatient()->getDocHis()->Rafraichir(_pDocInfo, (NSPatPathoArray*) 0, this, false /* don't warn bbk */) ;	}
	return true ;
}
catch (...)
{
	erreur("Exception NSRefDocument::Referencer.", standardError, 0) ;
	return false ;
}
}

bool
NSRefDocument::ReferencerHtml(string typeDoc, string nomDoc, string tempDoc, string enteteDoc, bool bIsVisible)
{
	return NSNoyauDocument::ReferencerHtml(typeDoc, nomDoc, tempDoc, enteteDoc, bIsVisible);
}

stringNSRefDocument::texteCourant(string textHtml)
{
    string  textOut = textHtml;
    size_t  pos = textOut.find("&");
    int     len;

    while (pos != NPOS)
    {
        len = strlen(textOut.c_str()) - pos;

   	    if ((len >= 7) && (string(textOut,pos,7) == "&acirc;"))
      	    textOut.replace(pos,7,"â");
        else if ((len >= 8) && (string(textOut,pos,8) == "&agrave;"))
      	    textOut.replace(pos,8,"à");
        else if ((len >= 8) && (string(textOut,pos,8) == "&ccedil;"))
      	    textOut.replace(pos,8,"ç");
        else if ((len >= 8) && (string(textOut,pos,8) == "&eacute;"))
      	    textOut.replace(pos,8,"é");
        else if ((len >= 7) && (string(textOut,pos,7) == "&ecirc;"))
      	    textOut.replace(pos,7,"ê");
        else if ((len >= 8) && (string(textOut,pos,8) == "&egrave;"))
      	    textOut.replace(pos,8,"è");
        else if ((len >= 6) && (string(textOut,pos,6) == "&euml;"))
      	    textOut.replace(pos,6,"ë");
        else if ((len >= 7) && (string(textOut,pos,7) == "&icirc;"))
			textOut.replace(pos,7,"î");
        else if ((len >= 6) && (string(textOut,pos,6) == "&iuml;"))
      	    textOut.replace(pos,6,"ï");
        else if ((len >= 7) && (string(textOut,pos,7) == "&ocirc;"))
      	    textOut.replace(pos,7,"ô");
        else if ((len >= 4) && (string(textOut,pos,4) == "&lt;"))
      	    textOut.replace(pos,4,"<");
        else if ((len >= 4) && (string(textOut,pos,4) == "&gt;"))
      	    textOut.replace(pos,4,">");
        else if ((len >= 6) && (string(textOut,pos,6) == "&ramp;"))
      	    textOut.replace(pos,6,"&");
        else if ((len >= 6) && (string(textOut,pos,6) == "&quot;"))
      	    textOut.replace(pos,6,"\"");
        else if ((len >= 6) && (string(textOut,pos,6) == "&#124;"))
            textOut.replace(pos,6,"|");

        pos++;

        pos = textOut.find("&",pos);
    }

    return textOut;
}

// chargement de la PatPatho depuis un tableau de stringbool
NSRefDocument::chargePatPatho(string sVersion, string* sTab)
{
try
{
	string sTexteLibre;
	string sCodeTexte = "";

	NSPatPathoInfo Info ;

	if (pContexte->typeDocument(sVersion, NSSuper::isTree))
	{
		Info.setPerson(pContexte->getPatient()->getszNss()) ;
		Info.setDocum("") ;		// ce code est mis à jour après référencement    Info.setLocalisation(texteCourant(sTab[0])) ;
    Info.setType        (texteCourant(sTab[1])) ;
    Info.setLexique     (texteCourant(sTab[2])) ;

    // cas des textes libres : on récupère le texte situé dans le complement
    // on génère un nouveau TL et on place son pointeur dans complement
    if (Info.getLexique() == string("£?????"))
    {
      sTexteLibre = texteCourant(sTab[3]) ;
      if (sTexteLibre == "")
      	sTexteLibre = "[texte non récupéré]" ;
      else
      	Info.setTexteLibre(sTexteLibre) ;
    }
    else
    	Info.setComplement(texteCourant(sTab[3])) ;

    Info.setCertitude(texteCourant(sTab[4])) ;
    Info.setInteret  (texteCourant(sTab[5])) ;
    Info.setPluriel  (texteCourant(sTab[6])) ;
	}
  else
  	return false ;

  _PatPathoArray.push_back(new NSPatPathoInfo(Info));

  return true ;
}
catch (...)
{
    erreur("Exception chargePatPatho.", standardError, 0);
    return false ;
}
}

boolNSRefDocument::ExtraitNomImage(string sLien, string& sNom, string& sExt)
{
    size_t  i;
    string  sUrl = "";
    string  sTemp;

    sNom = "";
    sExt = "";

    for (i = 0; (i < strlen(sLien.c_str())) && (sLien[i] != '\"'); i++);
    i++;

    // on récupère le nom de l'image

    while ((i < strlen(sLien.c_str())) && (sLien[i] != '.'))
    {
   	    sTemp = "";
   	    while ((i < strlen(sLien.c_str())) && (sLien[i] != '/') &&
   			 (sLien[i] != '\\') && (sLien[i] != '.'))
   		    sTemp += sLien[i++];

        if (i < strlen(sLien.c_str()))
        {
      	    if ((sLien[i] == '/') || (sLien[i] == '\\'))
            {
         	    sUrl += sTemp + sLien[i];
                i++;
            }
            else // cas sLien[i] == '.' => sortie du while
         	    sNom = sTemp;
        }
        else
            return false;
    }
    i++;

    // on récupère l'extension de l'image
    while ((i < strlen(sLien.c_str())) && (sLien[i] != '\"'))
   	    sExt += sLien[i++];

    sNom += string(".") + sExt;

    return true;
}

// Référencement automatique d'un document à l'aide d'un NSDocumentInfo// On ne passe pas par le dialogue qui permet de choisir le nom, la chemise, etc...
// Actuellement utilisé pour référencer les composants des html importés.
bool
NSRefDocument::ReferencerDocument(NSDocumentInfo* pInfo)
{
try
{
	return false ;
}
catch (...)
{
	erreur("Exception ReferencerDocument.", standardError, 0) ;
	return false ;}}

//// Référencement du tableau d'images constitué par ImporterHtml
//
void
NSRefDocument::ReferencerImages(NSDocumentArray* pImageArray)
{
	// on met à jour (d'après pDocInfo) le tableau d'infos images et on référence chaque image
  int    j = 0  ;
  char   cImage[20] ;
	for (DocInfoIter k = pImageArray->begin(); k != pImageArray->end(); k++)	{
  	sprintf(cImage, " (image %d)", j++) ;
    // size_t nbCars = DOC_NOM_LEN - strlen(cImage) ;

    string sImageName = _pDocInfo->getDocName() + string(cImage) ;
    (*k)->setNom(sImageName) ;

    ReferencerDocument(*k) ;
	}
}

//// Referencement du document contenant la PatPatho du fichier html importé
//
bool
NSRefDocument::ReferencerDocumentPatPatho(NSRefDocument* pMasterDoc, NSPatPathoArray* pPPt, string sNomDoc, string sTypeDoc)
{
  if ((NULL == pMasterDoc) || (NULL == pPPt))
    return false ;

try
{
  string sLang = pContexte->getUserLanguage() ;

  NSRefDocument NewDocTree(0, pContexte) ;
  NewDocTree.setReadOnly(false) ;
  NewDocTree.setPatPatho(pPPt) ;

  string sRootCode = (*(pPPt->begin()))->getLexique() ;

	string sLibelleDoc = sNomDoc ;
  if (string("") == sLibelleDoc)
  {
    pContexte->getDico()->donneLibelle(sLang, &sRootCode, &sLibelleDoc) ;
    if (string("") != sLibelleDoc)
  	  sLibelleDoc[0] = pseumaj(sLibelleDoc[0]) ;
  }

  if (string("") == sTypeDoc)
    sTypeDoc = string("ZCS00") ;

  bool bVerbose = false ;
  if (string("") == sLibelleDoc)
    bVerbose = true ;

  if (false == NewDocTree.Referencer(sTypeDoc, sLibelleDoc,
                                    string(""), string(""),  // file and path
                                    false, bVerbose,
                                    string(""),              // doc ID
                                    NSRootLink::personDocument, 0,
                                    string("_User_"),
                                    string(""), string(""),  // tpl and heading
                                    string(""), string(""),  // Dest and content
                                    pMasterDoc->getCodeDocMeta(),  // Master document
                                    NSRootLink::semantizedDocument))
	{
		string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

  // On enregistre la patpatho
	bool bSavePpt = NewDocTree.enregistrePatPatho() ;
	if (false == bSavePpt)
		return false ;

  // The new document must be inserted in "history" before checking if it resets
  // any goal, because this document's date will be asked to "history"

  // On prévient l'historique (fait à part pour les CS et les CN)
	pContexte->getPatient()->getDocHis()->Rafraichir(NewDocTree.DonneDocInfo(), pPPt, &NewDocTree) ;

	// Si c'est un nouveau compte rendu on vérifie s'il remet à zéro un objectif
	if (pContexte->getPatient()->getLdvDocument())
		pContexte->getPatient()->getLdvDocument()->showNewTree(pPPt, pMasterDoc->GetDateDoc(false)) ;

	return true ;
}
catch (...)
{
	erreur("Exception ReferencerDocumentPatPatho.", standardError, 0) ;
	return false ;}
}

boolNSRefDocument::GetModules(NSModuleArray* pModArray)
{
try
{
  if (NULL == pModArray)
    return false ;

  char 	buffer[201];
	string  Chaine, sCode;
  string  sFichierModules;
	int	    i, j;

  string sLang = pContexte->getUserLanguage() ;

  pModArray->vider();

	sFichierModules = pContexte->PathName("FPER") + string("MODULES.LUS");
	//
	// Ouverture du fichier des modules
	//
	ifstream inFile;
	inFile.open(sFichierModules.c_str());
	if (!inFile)
	{
		erreur("Erreur à l'ouverture du fichier des modules.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
		return false;
	}
	//
	// Lecture du fichier
	//
	NSModuleInfo* pModule = new NSModuleInfo;
	inFile.unsetf(ios::skipws);
	while (inFile.getline(buffer, 200))
	{
		if (strlen(buffer) >= 8)
		{
			i = 0 ;
			//
			// Prise du nom du module
			//
      string sLexiq = string("") ;
			for (j = 0 ; j < MODU_LEXIQUE_LEN ; j++, i++)
				sLexiq += string(1, buffer[i]) ;
			pModule->setLexique(sLexiq) ;
			i += 2;
			//
			// Prise du code du module
			//
      string sCodeModule = string("") ;
			for (j = 0; j < MODU_CODE_LEN; j++, i++)
				sCodeModule += string(1, buffer[i]) ;
			pModule->setCode(sCodeModule) ;
			//
			// Recherche du libellé
			//
      sCode = pModule->getLexique() ;
      string sLabel = string("") ;
			pContexte->getDico()->donneLibelle(sLang, &sCode, &sLabel) ;
      pModule->setLabel(sLabel) ;
			//
			// Ajout du module à l'array
			//
			pModArray->push_back(new NSModuleInfo(*pModule)) ;
		}

		pModule->metAZero() ;
	}
	delete pModule ;

	inFile.close() ;
  return true ;
}
catch (...)
{
  erreur("Exception NSRefDocument::GetModules.", standardError, 0) ;
  return false ;
}
}

boolNSRefDocument::PatEnCours(string sParam, string& sVer)
{
    string  sVersion = "", sNom = "", sPrenom = "", sDateN = "", sSexe = "";
    size_t  i = 0;
    int     retVal;
    char    nom[PAT_NOM_LEN + 1] = "";
    char    prenom[PAT_PRENOM_LEN + 1] = "";

    // numéro de version
    while ((i < strlen(sParam.c_str())) && (sParam[i] != '|'))
        sVersion += sParam[i++];
    i++;
    sVer = sVersion;

    while ((i < strlen(sParam.c_str())) && (sParam[i] != '|'))        sNom += sParam[i++];    i++;

    if (strlen(sNom.c_str()) <= PAT_NOM_LEN)
        strcpy(nom, sNom.c_str());
    sNom = string(strupr(nom));

    while ((i < strlen(sParam.c_str())) && (sParam[i] != '|'))
        sPrenom += sParam[i++];
    i++;

    if (strlen(sPrenom.c_str()) <= PAT_PRENOM_LEN)
        strcpy(prenom, sPrenom.c_str());
    sPrenom = string(strupr(prenom));

    while ((i < strlen(sParam.c_str())) && (sParam[i] != '|'))
        sDateN += sParam[i++];
    i++;

    while (i < strlen(sParam.c_str()))
        sSexe += sParam[i++];

    strcpy(nom, pContexte->getPatient()->getszNom());
    strcpy(prenom, pContexte->getPatient()->getszPrenom());

    if ((sNom == string(strupr(nom))) &&        (sPrenom == string(strupr(prenom))) &&
        (sDateN == string(pContexte->getPatient()->getszNaissance())) &&
        (sSexe == string(pContexte->getPatient()->getszSexe())))
    {
        return true;
    }
    else    {
        // cas des anciens compte-rendus
        if ((sNom == "") && (sPrenom == "") && (sDateN == "") && (sSexe == ""))
            return true;

        string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
        retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Attention le patient du document importé ne correspond pas au patient en cours. Voulez-vous continuer ?", sCaption.c_str(), MB_YESNO);

        if (retVal == IDYES)
            return true;
        else
            return false;
    }
}

// Importation des fichiers html statiquesbool
NSRefDocument::ImporterHtml(string sFichierSource)
{
try
{
  string sMsg = string("Entering NSRefDocument::ImporterHtml for file ") + sFichierSource ;
  pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  ifstream inFile ;

  string sLang = pContexte->getUserLanguage() ;

	inFile.open(sFichierSource.c_str()) ;	if (!inFile)
	{
  	erreur("Erreur à l'ouverture du fichier html à importer.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	NSDocumentArray ImageArray ; // pour pouvoir référencer les composants images

  string scr = string("") ;

	while (!inFile.eof())	{
    string line ;
		getline(inFile, line) ;
    if (string("") != line)
    	scr += line + string(1, '\n') ;
	}
	inFile.close() ;
	// on récupère le pathname du fichier source  //  string sPathSource = string("") ;	size_t pos = sFichierSource.find_last_of('\\') ;	if (NPOS != pos)		sPathSource = string(sFichierSource, 0, pos+1) ;	// on récupère l'extension du fichier source  //  string sExt = string("") ;  size_t i = 0 ;	for ( ; (i < strlen(sFichierSource.c_str())) && ('.' != sFichierSource[i]) ; i++) ;

	if (i < strlen(sFichierSource.c_str()))
	{
  	// sFichierSource[i] == '.'
    i++ ;
    while (i < strlen(sFichierSource.c_str()))
    	sExt += sFichierSource[i++] ;
	}

	// on lance l'interpretation du html  //  size_t numImage = 0 ;  string sOut = string("") ;	i = 0 ;
	while (i < strlen(scr.c_str()))
	{
    // Find next tag
    //
  	while ((i < strlen(scr.c_str())) && ('<' != scr[i]))
    	sOut += scr[i++] ;

    if (i < strlen(scr.c_str()))    {
    	// Lecture du Tag ...
			/* scr[i] == '<' */

      i++ ;

      string sNomTag   = string("") ;      string sParamTag = string("") ;

      while ((i < strlen(scr.c_str())) && ('>' != scr[i]) && (' ' != scr[i]))      	sNomTag += string(1, scr[i++]) ;

      if (' ' == scr[i])      {
      	i++ ;

        while ((i < strlen(scr.c_str())) && ('>' != scr[i]))        	sParamTag += string(1, scr[i++]) ;
      }

      /* scr[i] == '>' */
      i++ ;
      // cas des commentaires : on cherche la PatPatho      if ((strlen(sNomTag.c_str()) > 0) && ('!' == sNomTag[0]))
      {
      	// On réécrit le commentaire en entier
        sOut += string("<") + sNomTag + string(" ") + sParamTag + string(">") ;

        string ppt = sParamTag ;
        size_t n = 0 ;

        // on lit le premier tag PatPatho        sNomTag   = string("") ;
        sParamTag = string("") ;

				while ((n < strlen(ppt.c_str())) && ('{' != ppt[n]))        	n++ ;

        if (n < strlen(ppt.c_str()))        {
        	n++ ;

          while ((n < strlen(ppt.c_str())) &&                                    ('}' != ppt[n]) && (' ' != ppt[n]))
          	sNomTag += string(1, ppt[n++]) ;

          if ((n < strlen(ppt.c_str())) && (' ' == ppt[n]))          {
          	n++ ;

            while ((n < strlen(ppt.c_str())) && ('}' != ppt[n]))            	sParamTag += string(1, ppt[n++]) ;
          }

          /* ppt[n] == '}' */
          n++ ;        }

        if (string("PatPatho") == sNomTag)        {
        	// on controle si la PatPatho correspond au patient en cours
          string sVersion ;
          if (false == PatEnCours(sParamTag, sVersion))
          	return false ;

          // on initialise le PatPathoArray          _PatPathoArray.initialiseDepuisChaine(pContexte, &ppt, pContexte->getPatient()->getNss()) ;
        }
      }
      else if (string("IMG") == sNomTag) /* sNomTag != string("PatPatho") */
      {
      	// On traite ensemble le cas des images fixes et animées (vidéos)
        NSDocumentInfo ImageInfo ;

        numImage++ ;
        // On extrait le nom de l'image de sParamTag        //        string sNomSource ;        string sExtImage ;
        if (false == ExtraitNomImage(sParamTag, sNomSource, sExtImage))
        {
        	erreur("Impossible de récupérer une image du fichier à importer", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          return false ;
        }

        // On récupère les infos du type mime lié à l'extension du fichier
        // NSTypeMime 	   ficheTypeMime(pContexte->getSuperviseur()) ;
        NSTypeMimeInfo infoTypeMime ;

        if (!/*ficheTypeMime.*/pContexte->getSuperviseur()->chercheTypeMimeInfo(sExtImage, &infoTypeMime))
        {
          string sErrorText = string("L'extension du fichier image ") + sNomSource + string(" n'appartient pas à la base des types mime.") ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          return false ;
        }

				string sTypeImage = infoTypeMime.getType() ;

        // cas en principe impossible, mais bon...
        if (false == pContexte->typeDocument(sTypeImage, NSSuper::isImage))
        {
          string sErrorText = string("Le fichier image ") + sNomSource + string(" ne correspond pas à un type image Nautilus.") ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          return false ;
        }

        // On lui trouve un nouveau nom
        //
        string sNomImage ;
        string sLocImage = string("") ;

        if (false == TrouveNomFichier(sTypeImage, sExtImage, sNomImage, sLocImage))
        {
        	erreur("Erreur à l'attribution d'un nouveau nom pour une image.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          return false ;
        }

        string sImageSource = string("") ;
        if (string("") == sPathSource)
        	sImageSource = sNomSource ;
        else
        	sImageSource = sPathSource + sNomSource ;

        string sImageDest = pContexte->PathName(sLocImage) + sNomImage ;
        // on la copie depuis le repertoire d'importation sous son nouveau nom
        if (!CopyFile(sImageSource.c_str(), sImageDest.c_str(), false))
        {
          string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorCopyingFile") + string(" \"") + sImageSource + string("\" -> \"") + sImageDest + string("\"") ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          return false ;
        }

        // on copie les données de l'image pour la référencer ultérieurement
        ImageInfo.setType(sTypeImage) ;
        ImageInfo.setLocalisa(sLocImage) ;
        ImageInfo.setFichier(sNomImage) ;
        ImageArray.push_back(new NSDocumentInfo(ImageInfo)) ;

        // on remplace le tag image html par un tag nautilus
        // le tag nautilus [image] est générique pour les images et les vidéos
        sOut += string("[image]") ;
      }      else /* cas des autres Tags : on réécrit le tag */
      {
      	sOut += string("<") + sNomTag + string(" ") + sParamTag + string(">") ;
      }
    }
  }
	// fin de l'analyse syntaxique...

  // Référencement du document principal : deux cas
  // soit CS -> on référence un CS (PatPatho) + un HSHTM
  // soit CN -> on référence un CN
  // les images sont référencées dans les deux cas

  bool bEstDansModules = false ;
  bool bImporterCN ;
  int  retVal ;

	if (false == _PatPathoArray.empty())
	{
    string sCodeExamen   = (*(_PatPathoArray.begin()))->getLexique() ;
    string sCodeExamSens = string("") ;
    pContexte->getDico()->donneCodeSens(&sCodeExamen, &sCodeExamSens) ;

		// on récupère l'array des modules utilisés
  	NSModuleArray ModuleArray ;  // pour l'importation des CN
		if (string("GCONS") == sCodeExamSens)
      bEstDansModules = true ;
    else if (GetModules(&ModuleArray))
		{
      for (NSModuInfoArrayIter k = ModuleArray.begin() ; ModuleArray.end() != k ; k++)
      {
      	// on compare les deux codes sens
        //
        string sLexique  = (*k)->getLexique() ;
        string sCodeSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSens) ;

        if (sCodeSens == sCodeExamSens)
        {
        	bEstDansModules = true ;
          break ;
        }
      }
    }

    if (bEstDansModules)
    {
    	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
      retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Désirez-vous importer ce document en tant que document créé par vous-même ?", sCaption.c_str(), MB_YESNO) ;
      if (retVal == IDYES)
      	bImporterCN = true ;
      else
      	bImporterCN = false ;
    }
    else
    	bImporterCN = false ;
  }
  else
  {
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Désirez-vous importer ce document en tant que fichier HTML statique ?", sCaption.c_str(), MB_YESNO) ;
    if (IDYES == retVal)
    	bImporterCN = false ;
    else
    	return true ;
  }

	// Importation en tant que document CN
	if (bImporterCN)
	{
    string sLibelleDoc   = string("") ;
    string sCodeExamSens = string("") ;

    if (false == _PatPathoArray.empty())
    {
    	string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
      if (string("") != sCode)
      {
      	pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
        pContexte->getDico()->donneCodeSens(&sCode, &sCodeExamSens) ;
      }
    }

    bool bOk ;
    if (string("GCONS") == sCodeExamSens)
    {
      for (PatPathoIter it = _PatPathoArray.begin() ; _PatPathoArray.end() != it ; it++)
        (*it)->setVisible(string("1")) ;

      bOk = Referencer("ZCS00", sLibelleDoc) ;
    }
    else
      bOk = Referencer("ZCN00", sLibelleDoc) ;
    if (false == bOk)
    	return false ;

    //
    // On enregistre la patpatho
    //
    bOk = enregistrePatPatho() ;
    if (false == bOk)
    	return false ;
    //
    // On prévient l'historique
    //
    pContexte->getPatient()->getDocHis()->Rafraichir(_pDocInfo, &_PatPathoArray, this) ;

    // By default, any new document is considered as opened. Since it is not
    // the case here, we have to explicitely close it
    //
    pContexte->getPatient()->getDocHis()->FermetureDocument(_pDocInfo) ;

    // on référence les images qui ont besoin de sCodeChemise mis à jour par Referencer
    ReferencerImages(&ImageArray) ;
	}
  else
  { // Référencement CS (PatPatho) + HSHTM
  	// on trouve un nouveau nom pour le fichier html statique
    // if (!TrouveNomFichier("HSHTM",sExt,sNomFichier,sLocalis))

    string sLocalis    = string("") ;
    string sNomFichier = string("") ;

    if (!TrouveNomFichier("ZSHTM", sExt, sNomFichier, sLocalis))
    {
    	erreur("Erreur à l'attribution d'un nouveau nom pour le fichier html importé.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    ofstream outFile ;
    outFile.open((pContexte->PathName(sLocalis) + sNomFichier).c_str()) ;
    if (!outFile)
    {
    	erreur("Erreur à l'ouverture du fichier de sortie du fichier html importé.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    // on référence le fichier html statique
    if (!Referencer("ZSHTM", "", sNomFichier, sLocalis))
    {
    	erreur("Le fichier html importé n'a pas pu etre référencé", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    /* ecriture du nouveau fichier html */    for (size_t j = 0; j < strlen(sOut.c_str()); j++)
    	outFile.put(sOut[j]) ;
    outFile.close() ;

    // s'il y a une PatPatho : on référence le document PatPatho
    if (false == _PatPathoArray.empty())
    {
      string sNomDocPatPatho = _pDocInfo->getDocName() + string(" (PatPatho)") ;
      if (false == ReferencerDocumentPatPatho(this, &_PatPathoArray, sNomDocPatPatho, "ZCS00"))
      {
      	erreur("La PatPatho du fichier à importer n'a pas pu etre référencée", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
    }

    // on référence les images qui ont besoin de sCodeChemise mis à jour par Referencer
    ReferencerImages(&ImageArray) ;

    // on met à jour la base des composants
    // (les images doivent déjà avoir été référencées)
    //
    string sCodeDocPatPatho = string("") ;
    if (!EcrireComposants(sCodeDocPatPatho, &ImageArray))
    {
    	erreur("Erreur à la sauvegarde des composants du fichier html importé", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;    }
  }

	return true ;}
catch (...)
{
	erreur("Exception ImporterHtml.", standardError, 0) ;
	return false ;
}
}

// Routine d'importation d'un fichier quelconque (non html)bool
NSRefDocument::ImporterFichier(string sTypeNautilus, string sDocTitle, string sFichierSource, string sPathSource, bool bVerbose, bool bDocVisible)
{
try
{
  NSSuper *pSuper = pContexte->getSuperviseur() ;

  string sMsg = string("NSRefDocument: Entering ImporterFichier for file ") + sFichierSource ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

  if (string("") == sFichierSource)
    return false ;

	// Récupération de l'extension
	size_t pos = sFichierSource.find_last_of(".") ;
	if (NPOS == pos)
	{
  	string sErrorText = pSuper->getText("fileErrors", "cantFindExtension") + string(" ") + sFichierSource ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	string sExt = string(sFichierSource, pos+1, strlen(sFichierSource.c_str())-pos-1) ;
  string sNomFichier = string("") ;
  string sLocFichier = string("") ;

	// On trouve un nouveau nom pour le fichier à importer
	if (false == TrouveNomFichier(sTypeNautilus, sExt, sNomFichier, sLocFichier))
	{
		string sErrorText = pSuper->getText("fileErrors", "errorSettingAnImportedName") + string(" ") + sFichierSource ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	// on le copie depuis le repertoire source sous son nouveau nom
	string sNomSource = sPathSource + sFichierSource ;
	string sNomDest   = pContexte->PathName(sLocFichier) + sNomFichier ;

  sMsg = string("NSRefDocument:ImporterFichier ready to copy as ") + sNomDest ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

	if (false == CopyFile(sNomSource.c_str(), sNomDest.c_str(), false))
	{
  	string sErrorText = pSuper->getText("fileErrors", "errorCopyingFile") + string(" ") + sNomSource + string(" -> ") + sNomDest ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

  sMsg = string("NSRefDocument:ImporterFichier ready to start Referencer") ;
  pSuper->trace(&sMsg, 1, NSSuper::trDetails) ;

	// Le document dans ce cas sera visible dans l'historique sauf pour les images
	if (false == Referencer(sTypeNautilus, sDocTitle, sNomFichier, sLocFichier, bDocVisible, bVerbose))
	{
  	string sErrorText = pSuper->getText("importManagement", "referenceFailed") ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  sMsg = string("NSRefDocument: Leaving ImporterFichier") ;
  pSuper->trace(&sMsg, 1, NSSuper::trSubSteps) ;

	return true ;
}
catch (...)
{
	erreur("Exception ImporterFichier.", standardError, 0) ;
	return false ;
}
}

boolNSRefDocument::TrouveNomFichier(string typeDoc, string sExt, string& sNomFichier, string& sLocalis)
{
	if (string("") == typeDoc)
		return false ;

	string sCodePatient = pContexte->getPatient()->getNss() ;
	string sPath        = string("") ;
	string sTypeImage   = string("") ;
	string sTypeCR      = string("") ;
	string sNomServeur  = string("") ;
	string sUnite       = string("") ;

	sNomFichier = "" ;

	// On ne reconnait que les types de fichiers statiques	if ((pContexte->typeDocument(typeDoc, NSSuper::isText)) ||
	    (pContexte->typeDocument(typeDoc, NSSuper::isHTML)) ||	    (pContexte->typeDocument(typeDoc, NSSuper::isImage)))	{
		sExt = string(typeDoc, 2, 3) ;
    //
    // First, check if a specific path has been set for such document
    //
    string sTypePath = typeDoc ;
    sPath = pContexte->getSuperviseur()->PathNameType(sTypePath, sLocalis, sUnite, sNomServeur, pContexte) ;
    //
    // If not, use a "reasonnable" path
    //
		if (string("") == sPath)
    {
    	sTypePath = string("") ;

			// les fichiers HIHTM sont stockés au même endroit que les HSHTM
			// (les HIHTM sont un cas particulier de fichier HTML statique)
			if      (typeDoc == "ZIHTM")
				sTypePath = string("ZSHTM") ;
			else if (typeDoc == "ZTHTM")
				sTypePath = string("ZTRTF") ;
    	else if (typeDoc == "ZTPDF")
				sTypePath = string("ZTRTF") ;
    	else if (typeDoc == "ZTTXT")
				sTypePath = string("ZTRTF") ;
    	//
    	// Images
    	//
    	else if (pContexte->typeDocument(typeDoc, NSSuper::isImageFixe))
				sTypePath = string("ZIF00") ;
    	else if (pContexte->typeDocument(typeDoc, NSSuper::isImageAnimee))
				sTypePath = string("ZIA00") ;

      if (string("") != sTypePath)				sPath = pContexte->getSuperviseur()->PathNameType(sTypePath, sLocalis, sUnite, sNomServeur, pContexte) ;
    }
		if (string("") == sPath)
			return false ;
	}
	else if (typeDoc == string("ZCN00"))	// cas des comptes-rendus : création d'un fichier .tmp
	{
		sTypeCR = typeDoc ;
		sPath = pContexte->getSuperviseur()->PathNameType(sTypeCR, sLocalis, sUnite, sNomServeur, pContexte) ;
		if (sPath == "")
			return false ;
	}
	else
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagement", "cantFindFileNameAccordingToFileType") + string(" (") + typeDoc + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return false ;
	}

	// si l'extension n'est pas spécifiée, on ne prend plus l'extension par défaut :
	// Anciennement, Pour tous les fichiers statiques = les 3 derniers caractères du typeDoc
	if (string("") == sExt)
	{
		// sExt = string(typeDoc,2,3);
		erreur("Extension non précisée dans ::TrouveNomFichier().", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;		return false ;	}	// On trouve un nouveau nom pour le fichier	sNomFichier = nomSansDoublons(sNomServeur, sUnite, sPath, sCodePatient, sExt) ;

	return true ;
}

stringNSRefDocument::InitIntituleNoBirth()
{
try
{
	NSHtml htmlTitre(tTitre2) ;

	string sIntituleDocument = string("") ;

	string sLang = pContexte->getUserLanguage() ;

	string sLibelleDoc = "Document" ;
	if (false == _PatPathoArray.empty())
	{
  	string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
    if (string("") != sCode)
    {
    	pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
      if (strlen(sLibelleDoc.c_str()))
      	sLibelleDoc[0] = pseumaj(sLibelleDoc[0]) ;
    }
  }

	if ((NSSmallBrother*) NULL == _pBigBoss)  	_pBigBoss = new NSSmallBrother(pContexte, &_PatPathoArray) ;
  // At first, have a look into the patpatho
  //
	string sDateExamen = _PatPathoArray.GetDocDate() ;

	if (string("") == sDateExamen)	{
  	sIntituleDocument = sLibelleDoc + string(" de ") ;
    sIntituleDocument += pContexte->getPatient()->getPrenom() + string(" ") ;
		sIntituleDocument += pContexte->getPatient()->getNom() ;
	}
	else
	{
  	char datex[15] ;
  	strcpy(datex, sDateExamen.c_str()) ;
    int age = donne_age(datex) ;

    string sIntitulePatient = string("") ;
    donne_intitule_patient(&sIntitulePatient, age) ;

    sIntituleDocument = sLibelleDoc + string(" de ") + sIntitulePatient + string(" ") ;
    sIntituleDocument += pContexte->getPatient()->getPrenom() + string(" ") ;
		sIntituleDocument += pContexte->getPatient()->getNom() ;
	}
	return sIntituleDocument ;
}
catch (...)
{
	erreur("Exception NSRefDocument::InitIntitule.", standardError, 0) ;
	return "" ;
}
}

string
NSRefDocument::InitIntitule()
{
  string sIntitule = InitIntituleNoBirth() ;
  string sDateNaiss = pContexte->getPatient()->donneNaissance() ;	if (string("00000000") == sDateNaiss)
    return sIntitule ;

  sIntitule += string(NEWLINE) ;
  if (pContexte->getPatient()->estFeminin())
    sIntitule += string("Née le ") ;
  else
    sIntitule += string("Né le ") ;
  sIntitule += donne_date(sDateNaiss, pContexte->getUserLanguage()) ;

  return sIntitule ;
}

bool
NSRefDocument::EcrireComposants(NSPatPathoArray* pPatPatho)
{
	return NSNoyauDocument::EcrireComposants(pPatPatho) ;
}

// Fonction d'écriture des composants html statiquesbool
NSRefDocument::EcrireComposants(string sCodeDocPatPatho, NSDocumentArray* pImageArray)
{
try
{
	return true ;
}
catch (...)
{
	erreur("Exception EcrireComposants.", standardError, 0) ;
	return false ;
}
}

//// On recherche les éléments de classification et on les envoie au SOAPer//boolNSRefDocument::makeSOAP(){try{  // If patpatho is null or empty, no need fo go ahead  //	if (_PatPathoArray.empty())		return true ;  NSSuper* pSuper = pContexte->getSuperviseur() ;  // Check if proper material will allow dispatching inside "baskets"  //  PrinciplesArray* pPrincipes = pContexte->getEpisodus()->pPrincipes ;  if ((NULL == pPrincipes) || (pPrincipes->empty()))    return false ;	VectString aVecteurString ;	pSuper->getFilGuide()->TousLesVrais("0CODE", "ES", &aVecteurString, "ENVERS") ;	if (aVecteurString.empty())		return true ;	string sCodeSens ;	string sLang = pContexte->getUserLanguage() ;  //  // On recherche tous les codes présents dans le document  //  PatPathoIter pptIt = _PatPathoArray.begin() ;  for ( ; _PatPathoArray.end() != pptIt ; pptIt++)  {    string sLexique = (*pptIt)->getLexique() ;    pContexte->getDico()->donneCodeSens(&sLexique, &sCodeSens) ;    IterString itStr = aVecteurString.begin() ;    for ( ; aVecteurString.end() != itStr ; itStr++)    {      if (*(*itStr) == sCodeSens)      {        string sLibellePere = string("") ;        PatPathoIter itPere = _PatPathoArray.ChercherPere(pptIt) ;        if ((NULL != itPere) && (_PatPathoArray.end() != itPere))        {          string sLexiPere = (*itPere)->getLexique() ;          if (string("£?????") != sLexiPere)            pContexte->getDico()->donneLibelle(sLang, &sLexiPere, &sLibellePere) ;          else            sLibellePere = (*itPere)->getTexteLibre() ;          //          // Recherche de la case          //          string sCase = string("") ;          // PrinciplesArray* pPrincipes = pSuper->getEpisodus()->pPrincipes ;          // if ((pPrincipes) && (false == pPrincipes->empty()))          // {            bool         bChercher = true ;            PatPathoIter itCurrent = pptIt ;            while (bChercher)            {              PatPathoIter pereIter = _PatPathoArray.ChercherPere(itCurrent) ;              if ((NULL == pereIter) || (_PatPathoArray.end() == pereIter))                bChercher = false ;              else              {                string sCaseSens = (*pereIter)->getLexiqueSens() ;                for (PrinciplesIter i = pPrincipes->begin(); pPrincipes->end() != i ; i++)                {
                  string sCasePrcp = (*i)->sCase ;
                  string sCasePrcpSens ;
                  pContexte->getDico()->donneCodeSens(&sCasePrcp, &sCasePrcpSens) ;

                  if ((sCaseSens == sCasePrcpSens) && ((*i)->sClassification == sCodeSens))
                  {
                    sCase     = (*i)->sCase ;
                    bChercher = false ;
                  }                }                itCurrent = pereIter ;              }            }          // }          if (string("") != sCase)          {            SOAPObject* pSOAPObj = new SOAPObject(sLibellePere, (*pptIt)->getComplement(), sLexique, 100, (*pptIt)->getNode()) ;            pSOAPObj->sCase = sCase ;            NSEpisodus *pEpisodus = pContexte->getEpisodus() ;            pEpisodus->addToSOAP(pSOAPObj) ;            if (pEpisodus->pSOAPView && pEpisodus->pSOAPView->getDoc())              pEpisodus->pSOAPView->getDoc()->pDocInfo = _pDocInfo ;          }        }      }    }  }  //  // On recherche les 'O' à recenser (optionnel)  //  string sFichierTempo = pContexte->PathName("FPER") + string("soap_o.dat") ;
  ifstream inFile ;
  inFile.open(sFichierTempo.c_str()) ;  if (!inFile)
    return true ;

  VectString aOtoSOAP ;
  string     sLine ;
  while (!inFile.eof())  {
    getline(inFile, sLine) ;
    if (string("") != sLine)
      aOtoSOAP.push_back(new string(sLine)) ;
  }
  inFile.close() ;
  if (aOtoSOAP.empty())    return true ;  //  // On recherche tous les codes présents dans le document  //  pptIt = _PatPathoArray.begin() ;  for ( ; _PatPathoArray.end() != pptIt ; pptIt++)  {    string sChemin = _PatPathoArray.donneCheminItem(pptIt) ;    string sNode = (*pptIt)->getLexique() ;    if ((*pptIt)->getPluriel() != "")
      sNode += string(1, cheminSeparationMARK) + (*pptIt)->getPluriel() ;
    if ((*pptIt)->getCertitude() != "")
      sNode += string(1, cheminSeparationMARK) + (*pptIt)->getCertitude() ;    if (string("") != sChemin)      sChemin += string(1, cheminSeparationMARK) ;    sChemin += sNode ;    string sCheminSens ;    NSDico::donneCodeSens(&sChemin, &sCheminSens) ;    IterString itStr = aOtoSOAP.begin();    for ( ; aOtoSOAP.end() != itStr ; itStr++)    {      if (*(*itStr) == sCheminSens)      {        NSPatPathoArray PT(pContexte->getSuperviseur()) ;        _PatPathoArray.ExtrairePatPathoWithRoot(pptIt, &PT) ;        // Décodage de l'élément lexique        GlobalDkd Dcode(pContexte, sLang, sCheminSens, &PT) ;
        Dcode.decode(pContexte->getDico()->dcTiret) ;
        // pDcode->decodeNoeud(sChemin) ;
        string sLabel = Dcode.getDcodeur() ;

        SOAPObject* pSOAPObj = new SOAPObject(sLabel, "", "", 100, (*pptIt)->getNode()) ;        pSOAPObj->sCase = "0SOA21" ;        pContexte->getEpisodus()->addToSOAP(pSOAPObj) ;      }    }  }  return true ;}catch (...)
{
  erreur("Exception NSCSDocument makeSOAP.", standardError, 0) ;
  return false ;
}}
// Fin de Nsdocref.cpp////////////////////////////////////////////////////////////////////////

