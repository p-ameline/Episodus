//---------------------------------------------------------------------------//  NS_HTML.CPP
//  RS Juin 97
//  Génération de fichiers HTML à partir d'un compte-rendu
//---------------------------------------------------------------------------

#define __NS_HTML_CPP

#include <stdio.h>
#include <classlib\filename.h>
#include <sysutils.hpp>

#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "nsdn\nsintrad.h"
#include "nautilus\nsresour.h"
#include "nautilus\nsmodhtm.h"
// #include "nautilus\nsanxary.h"
#include "nsbb\nstlibre.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsdico.h"
#include "nsdn\nsdocnoy.h"
#include "nautilus\nsbasimg.h"
#include "nssavoir\nsHtmBase.h"

long NSModHtml::lObjectCount = 0 ;

//----------------------------------------------------------------------------
//		Classe NSModHtml
// Classe encapsulant un ou plusieurs objets NSHtml et comprenant les
// méthodes permettant de les générer sur fichier à partir d'un modele donné.
//----------------------------------------------------------------------------

// Constructeur : instancie un modèle vide
NSModHtml::NSModHtml(OperationType typeOp, NSNoyauDocument* pDocNoy, NSContexte* pCtx)
          :NSRoot(pCtx)
{
	nbHtml        = 0 ;
	bExisteTable  = false ;
	sModele       = string("") ;
	_sOut         = string("") ;

	if (pDocNoy)
  	sCodeDoc = pDocNoy->getDocId() ;
	else
  	sCodeDoc = string("") ;

  typeOperation = typeOp ;
  sBaseImages   = string("") ;        // base d'images à récupérer
  sBaseCompo    = string("") ;        // idem pour la composition
  pBaseImages   = (NSBaseImages*) 0 ; // pointeur initialisé par chargeBaseImages()
  pBaseCompo    = (NSBaseImages*) 0 ;
  _pDocBrut     = pDocNoy ;
  _pLdvDoc      = (NSLdvDocument*) 0 ;

  lObjectCount++ ;
}

NSModHtml::NSModHtml(OperationType typeOp, NSLdvDocument* pDocLdv, NSContexte* pCtx)
          :NSRoot(pCtx)
{
	nbHtml        = 0 ;
	bExisteTable  = false ;
	sModele       = string("") ;
	_sOut         = string("") ;

  sCodeDoc      = string("") ;

  typeOperation = typeOp ;
  sBaseImages   = string("") ;        // base d'images à récupérer
  sBaseCompo    = string("") ;        // idem pour la composition
  pBaseImages   = (NSBaseImages*) 0 ; // pointeur initialisé par chargeBaseImages()
  pBaseCompo    = (NSBaseImages*) 0 ;
  _pDocBrut     = (NSNoyauDocument*) 0 ;
  _pLdvDoc      = pDocLdv ;

  lObjectCount++ ;
}

// destructeur : delete des pointeurs de NSHtmlNSModHtml::~NSModHtml()
{
	if (pBaseImages)
		delete pBaseImages ;

	if (pBaseCompo)
		delete pBaseCompo ;

  lObjectCount-- ;
}

voidNSModHtml::ajoute(NSHtml* ph, int nbOccur)
{
try
{
	blocHtml.push_back(new NSBlocHtml(ph, nbOccur)) ;
	nbHtml++ ;
}
catch (...)
{
	erreur("Exception (NSModHtml::ajoute)", standardError, 0) ;
}
}

void
NSModHtml::ajoute(HtmlTypes th, string sNonHtmlTxt)
{
  ajoute(new NSHtml(th, texteHtml(sNonHtmlTxt)), 1) ;
}

void
NSModHtml::addSignature()
{
  if ((NSNoyauDocument*) NULL == _pDocBrut)
  {
    ajoute(tSignature, string("")) ;
    ajoute(tOperateur, string("")) ;
    return ;
  }

  NSDocumentInfo* pDocInfo = _pDocBrut->DonneDocInfo() ;

  if ((NSDocumentInfo*) NULL == pDocInfo)
  {
    ajoute(tSignature, string("")) ;
    ajoute(tOperateur, string("")) ;
    return ;
  }

  string sNss = pDocInfo->getAuthor() ;
  if (string("") == sNss)
    sNss = pDocInfo->getCreator() ;

  if (string("") == sNss)
  {
    ajoute(tSignature, string("")) ;
    ajoute(tOperateur, string("")) ;
    return ;
  }

  NSPersonInfo* pUtil = pContexte->getPersonArray()->getPerson(pContexte, sNss, pidsUtilisat) ;
  if (pUtil)
  {
    ajoute(tSignature, pUtil->getCivilite()) ;

    string sOperateur = pUtil->getPrenom() + string(" ") + pUtil->getNom() ;
    strip(sOperateur) ;
    ajoute(tOperateur, sOperateur) ;
  }
  else
  {
    ajoute(tSignature, string("")) ;
    ajoute(tOperateur, string("")) ;
  }
}

void
NSModHtml::addPatient()
{
  string sNomPatient      = string("") ;
  string sPatientNom      = string("") ;
  string sPatientPrenom   = string("") ;
  string sPatientNomNaiss = string("") ;
  string sPatientNaiss    = string("") ;
  string sPatientIPP      = string("") ;
  string sPatientIEP      = string("") ;
  string sPatientAdresse  = string("") ;
  string sPatientGender   = string("") ;

  NSPatientChoisi* pPatient = pContexte->getPatient() ;
  if (pPatient)
  {
    string sLang = pContexte->getUserLanguage() ;

    sNomPatient      = pPatient->getNomLong() ;
    sPatientNom      = pPatient->getNom() ;
    sPatientPrenom   = pPatient->getPrenom() ;
    sPatientNomNaiss = pPatient->getNomJeuneFille() ;
    sPatientNaiss    = donne_date(pPatient->getNaissance(), sLang) ;
    sPatientIPP      = pPatient->getIPPEnCours(string("*")) ;
    sPatientAdresse  = pPatient->getGraphPerson()->trouveLibLongForInGraphAdr(pidsPatient) ;

    NVLdVTemps tNow ;
    tNow.takeTime() ;

    NSSejourInfo SejourInfo ;
    if (pPatient->getGraphPerson()->LastOpenStay(pContexte, tNow.donneDateHeure(), &SejourInfo))
      sPatientIEP = SejourInfo.getNumero() ;

    if (pPatient->estMasculin())
    {
      string sCode = string("HMASC2") ;
      pContexte->getDico()->donneLibelle(sLang, &sCode, &sPatientGender) ;
    }
    else if (pPatient->estFeminin())
    {
      string sCode = string("HFEMI2") ;
      pContexte->getDico()->donneLibelle(sLang, &sCode, &sPatientGender) ;
    }
    if (string("") != sPatientGender)
      sPatientGender[0] = pseumaj(sPatientGender[0]) ;
  }

  ajoute(tNomPatient,      sNomPatient) ;
  ajoute(tPatientNom,      sPatientNom) ;
  ajoute(tPatientPrenom,   sPatientPrenom) ;
  ajoute(tPatientNomNaiss, sPatientNomNaiss) ;
  ajoute(tPatientNaiss,    sPatientNaiss) ;
  ajoute(tPatientIPP,      sPatientIPP) ;
  ajoute(tPatientIEP,      sPatientIEP) ;
  ajoute(tPatientAdresse,  sPatientAdresse) ;
  ajoute(tPatientGender,   sPatientGender) ;
}

bool
NSModHtml::lireModele(string fichier, string entete)
{
try
{
	ifstream inFile ;
	inFile.open(fichier.c_str()) ;
	if (!inFile)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + fichier ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
  }

	sFichierTemplate = fichier ;
	sFichierEnTete = entete ;

  string line ;
	while (!inFile.eof())
	{
  	getline(inFile, line) ;
    if (string("") != line)
    	sModele += line + string("\n") ;
	}
	inFile.close() ;

	scanPrintParams() ;

	nbTagsImage = compteTags("image") ;

	return true ;
}
catch (...)
{
	erreur("Exception (NSModHtml::lireModele)", standardError, 0) ;
	return false ;
}
}

string
NSModHtml::nomSansDoublons(string pathRef, string nom, string ext)
{
try
{
  int    cpt = 0 ;

  char   suff[4] ;
  string suffixe ;
  string nomComplet ;
  string sNomFichier ;

  bool exist = true ;
  while (exist)
  {
    sprintf(suff,"%03d",cpt);
    suffixe = string(suff);
    nomComplet = nom + string("_") + suffixe;
    // nomFichier = TFileName(sServeur.c_str(),cUnite,pathRef.c_str(),nomComplet.c_str(),ext.c_str());
    sNomFichier = pathRef + nomComplet + string(".") + ext ;
    exist = NsFileExists(sNomFichier) ;
    cpt++;
    if (cpt == 1000) // limite du nombre de doublons
    {
      string sErrorText = string("Vous avez atteint la limite du nombre de doublons pour l'ammorce ") + nom + string(" dans le répertoire ") + pathRef ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0) ;

      nomComplet = string("") ;
      exist = false;
    }
  }

  if (string("") != nomComplet)
		nomComplet = nomComplet + string(".") + ext ;

  return nomComplet ;
}
catch (...)
{
  erreur("Exception NSModHtml::nomSansDoublons", standardError, 0) ;
  return string("") ;
}
}

// Méthode d'initialisation d'une base d'images utilisée pour// la composition. Insère les images de la chemise du document brut
// dans la base d'images. pHtmlInfo (pDocInfo) est utilisé en N_TIERS
bool
NSModHtml::chargeBaseImages(string sPathBase, string sPathImages, bool bPreSelect, NSDocumentInfo* pHtmlInfo)
{
try
{
  NSDocumentInfo* pTemplateInfo = pHtmlInfo ;

  // No pHtmlInfo means that the document itself is the template
  //
  if ((NSDocumentInfo*) NULL == pTemplateInfo)
  {
    if (NULL == _pDocBrut)
		  return false ;
    pTemplateInfo = _pDocBrut->_pDocInfo ;
  }

  if ((NSDocumentInfo*) NULL == pTemplateInfo)
    return false ;

  string sCodeChemise      = string("") ;  // code chemise du document brut
  string sCodeChemDoc      = string("") ;  // idem plus blancs pour la recherche
  string sCodeDocChemise   = string("") ;  // code des documents de la chemise
  bool   chemiseAvecImages = false ;
  bool   pathInit          = false ;

  if (sPathImages != "")
		pathInit = true ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphManager)
    return false ;

	NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
	VecteurString  VectString ;
	string         sNodeChemise ;

	// on récupère d'abord la chemise (folder) du document
	pGraphe->TousLesVrais(pTemplateInfo->getCodeDocMeta(), NSRootLink::docFolder, &VectString, "ENVERS") ;
	if (VectString.empty())
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("folderErrors", "cannotFindTheFolderThisDocumentIsPartOf") ;
    sErrorText += string(" (") + pTemplateInfo->getCodeDocMeta() + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

	sNodeChemise = *(*(VectString.begin())) ;

	// on récupère maintenant la liste de tous les documents de cette chemise
	NSDocumentArray DocusArray ;
	VectString.vider() ;
	pGraphe->TousLesVrais(sNodeChemise, NSRootLink::docFolder, &VectString) ;

	if (false == VectString.empty())
	{
  	for (EquiItemIter i = VectString.begin() ; VectString.end() != i ; i++)
    	DocusArray.push_back(new NSDocumentInfo(*(*i), pContexte, pGraphManager)) ;

    // Récupération des informations des documents
    DocInfoIter iterDoc = DocusArray.begin() ;
    bool bOk = true ;

    while (DocusArray.end() != iterDoc)    {
    	VectString.vider() ;

      string sCodePat = string((*iterDoc)->getCodeDocMeta(), 0, PAT_NSS_LEN) ;
      string sCodeDoc = string((*iterDoc)->getCodeDocMeta(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

      (*iterDoc)->setPatient(sCodePat) ;
      (*iterDoc)->setDocument(sCodeDoc) ;

      if ((*iterDoc)->ParseMetaDonnees())
      {
      	if (pContexte->typeDocument((*iterDoc)->getTypeSem(), NSSuper::isImage))
        {
        	if (!chemiseAvecImages)
          {
          	// mise à jour de la donnee membre sBaseImages
            // note : la base d'images est ici composée des images de la chemise
            sBaseCompo = sPathBase + nomSansDoublons(sPathBase, sCodeChemise, "img") ;

            // creation de la base d'images
            pBaseCompo = new NSBaseImages(sBaseCompo) ;
            chemiseAvecImages = true ;
          }
          // On prend l'extension liée au type nautilus pour la copie
          // car elle correspond au type mime (inutile de prendre l'extension source)
          string sTypeImage = (*iterDoc)->getTypeSem() ;
          string sExtImage = string(sTypeImage, 2, 3) ; // extension du fichier

          // si le path images n'est pas précisé : on prend le répertoire serveur lié au type
          // (Ne pas modifier le booleen ici)
          if (!pathInit)
          {
          	if (pContexte->typeDocument(sTypeImage, NSSuper::isImageFixe))
            	sPathImages = pContexte->PathName("SIMG") ;
            else if (pContexte->typeDocument(sTypeImage, NSSuper::isImageAnimee))
            	sPathImages = pContexte->PathName("SVID") ;
          }

          string sNomImage = nomSansDoublons(sPathImages, (*iterDoc)->getCodeDocMeta(), sExtImage) ;
          string sPathSource = pContexte->PathName((*iterDoc)->getLocalis()) ;
          string sNomSource = (*iterDoc)->getFichier() ;

          if (!pBaseCompo->ajouter(sNomImage, sPathImages, sPathSource, sNomSource, sTypeImage))
          {
          	string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCopyAnImageToTheTemporaryLocation") ;
            sErrorText += string(" ") + sPathSource + sNomSource + string(" -> ") + sPathImages + sNomImage ;

            string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
			      if (string("") != sSystemLastErrorMessage)
      	      sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

    				pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
						erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
            return false ;
          }
        }
      }
      // problème de récupération de la patpatho (ou de parsing) : le document
      // ne figure pas dans la liste
      else
      {
      	char msg[255] ;
        sprintf(msg, "Problème de récupération du document [%s%s]", (*iterDoc)->getPatient().c_str(),
                                                                         (*iterDoc)->getDocument().c_str()) ;
        erreur(msg, standardError, 0) ;
        delete *iterDoc ;
        DocusArray.erase(iterDoc) ;
        bOk = false ;
      }

      if (bOk)
      	iterDoc++ ;
    }
  }

	if (chemiseAvecImages)
	{
		// Présélection des images de la base de composition
    if (bPreSelect)
    {
    	for (int i = 0 ; i < pBaseCompo->nbImages ; i++)
      	pBaseCompo->tableSelect[i] = i + 1 ;
    }

    // S'il existe une base d'images : on reproduit la selection dans la base de composition
    if (pBaseImages)
    {
      for (int i = 0 ; i < pBaseImages->nbImages ; i++)
      {
      	for (int j = 0 ; j < pBaseCompo->nbImages ; j++)
        {
        	if (pBaseCompo->tableCompos[j] == pBaseImages->tableCompos[i])
          {
          	pBaseCompo->tableSelect[j] = pBaseImages->tableSelect[i] ;
            break ;
          }
        }
      }
    }

    if (!pBaseCompo->ecrire())
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCreateImagesList") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }

	return true ;
}catch (...)
{
	erreur("Exception ChargeBaseImages", standardError, 0) ;
	return false ;
}
}

// charge la base d'images de la publication// (on récupère les images liées au document à l'aide de pHtmlInfo
//	en allant les chercher dans la base des composants)

boolNSModHtml::chargeBaseImages(string sPathBase, string sPathImages, NSDocumentInfo* pHtmlInfo)
{
try
{
  // If there is already an image index in the right location, we use it
  //
	if ((toImprimer == typeOperation) || (toVisualiser == typeOperation))	{
		if (string("") != _pDocBrut->_sBaseImages)
    {
      size_t iPathBaseLen = strlen(sPathBase.c_str()) ;
      if ((strlen(_pDocBrut->_sBaseImages.c_str()) > iPathBaseLen) &&
          (string(_pDocBrut->_sBaseImages, 0, iPathBaseLen) == sPathBase))
      {
        if ((NSBaseImages*) NULL == pBaseImages)
        {
      	  pBaseImages = new NSBaseImages(sBaseImages) ;
      	  if (pBaseImages->lire())
          {
            sBaseImages = _pDocBrut->_sBaseImages ;
            return true ;
          }
        }
        else
        {
          sBaseImages = _pDocBrut->_sBaseImages ;
          return true ;
        }
      }
    }
	}

  bool pathInit = false ;
	if (string("") != sPathImages)		pathInit = true ;

	if (NULL == pHtmlInfo)
		return false ;

	bool    baseChargee = false ;

	// creation de la base d'images
  if ((NSBaseImages*) NULL == pBaseImages)  {		sBaseImages = sPathBase + nomSansDoublons(sPathBase, pHtmlInfo->getID(), "img") ;
		pBaseImages = new NSBaseImages(sBaseImages) ;
  }

	// En N_TIERS la variable pHtmlInfo est en fait le pDocInfo du document  string sCodeDocMeta = pHtmlInfo->getCodeDocMeta() ;  NSPatPathoArray PptPres(pContexte->getSuperviseur()) ;  pHtmlInfo->initFromPres(&PptPres) ;  if (PptPres.empty())    return false ;  VecteurString VecteurString ;  NSLinkManager* pLink = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;  // On doit maintenant retrouver les tags image pour insérer leurs liens  // avec les documents images
  string sElemLex, sType ;
  string sIndex = "" ;

  // on doit parcourir la librairie pour charger l'array des chemises
  PatPathoConstIter iter = PptPres.begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  int	nbImages = 0 ;

	while ((PptPres.end() != iter) && ((*iter)->getColonne() > iColBase))
	{
  	string sSens = (*iter)->getLexiqueSens() ;

    if (string("0TAG0") == sSens)
    {
      string sCodeImage = string("") ;

      string sNodeTag = (*iter)->getNode() ;
      VecteurString.vider() ;
      pLink->TousLesVrais(sNodeTag, NSRootLink::compositionTag, &VecteurString) ;
      if (false == VecteurString.empty())
      	sCodeImage = *(*(VecteurString.begin())) ;

      sIndex = "" ;
      iter++ ;

      // on charge l'index du tag
      while ((PptPres.end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
      	sSens = (*iter)->getLexiqueSens() ;

        // tag number
        if (string("VNUMT") == sSens)
        {
        	iter++ ;
          while ((PptPres.end() != iter) && ((*iter)->getColonne() > iColBase+2))
          {
          	// on cherche ici un texte libre
            sElemLex = (*iter)->getLexique() ;
            if (string(sElemLex, 0, 2) == string("£N"))
            	sIndex = (*iter)->getComplement() ;
            iter++ ;
          }
        }
        // tag type
        else if (string("0TYPC") == sSens)
        	iter++ ;
        else
        	iter++ ;
      }

      int indexImage = atoi(sIndex.c_str()) ;

      string sTypeImage     = string("") ;
      string sFichierSource = string("") ;
      string sPathSource    = string("") ;

      // on cherche les donnees complementaires dans le graphe
      if ((string("") != sCodeImage) &&
          (true == donneesImages(sCodeImage, sPathSource, sFichierSource, sTypeImage)))
      {
      	// si le path images n'est pas précisé : on prend le répertoire serveur lié au type
      	if (!pathInit)
      	{
      		if (pContexte->typeDocument(sTypeImage, NSSuper::isImageFixe))
        		sPathImages = pContexte->PathName("SIMG") ;
        	else if (pContexte->typeDocument(sTypeImage, NSSuper::isImageAnimee))
        		sPathImages = pContexte->PathName("SVID") ;
      	}

      	// on prend l'extension mime pour la copie   UTILISER sFichierSource      	string sExtImage = string(sTypeImage, 2, 3) ; // extension du fichier
      	string sNomImage = nomSansDoublons(sPathImages,sCodeImage,sExtImage) ;

      	if (!pBaseImages->ajouter(sNomImage, sPathImages, sPathSource, sFichierSource, sTypeImage, indexImage))      	{
      		string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCopyAnImageToTheTemporaryLocation") ;
        	sErrorText += string(" ") + sPathSource + sFichierSource + string(" -> ") + sPathImages + sNomImage ;

          string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
          if (string("") != sSystemLastErrorMessage)
            sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

        	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        	erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        	return false ;
      	}

      	nbImages++ ;
      }
    }
    else
    	iter++ ;
	}	if (baseChargee)	{
  	if (!pBaseImages->ecrire())
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCreateImagesList") ;
    	pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
	}

	return true ;}
catch (...)
{
	erreur("Exception ChargeBaseImages", standardError, 0) ;
	return false ;
}
}

// cas des images (documents Ixxxx) qui sont stockées en tant que composants// d'un document. On récupère ici les données du fichier image correspondant
// pour pouvoir les publier.

boolNSModHtml::donneesImages(string sCodeImage, string& sPathSource, string& sFichierSource, string& sTypeImage)
{
try
{
	if (string("") == sCodeImage)
  	return false ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
    return false ;

	NSDocumentInfo DocImage(sCodeImage, pContexte, pGraphManager) ;

  // Doc recuperation failed
  // if ((NULL == DocImage.getMeta()) || (true == DocImage.getMeta()->empty()))
	//	return false ;

	string sCodePat = string(sCodeImage, 0, PAT_NSS_LEN) ;
	string sCodeDoc = string(sCodeImage, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

	DocImage.setPatient(sCodePat) ;
	DocImage.setDocument(sCodeDoc) ;

	if (false == DocImage.ParseMetaDonnees())
    return false ;

  // on récupere les donnees de l'image
  sPathSource    = pContexte->PathName(DocImage.getLocalis()) ;
  sFichierSource = DocImage.getFichier() ;
  sTypeImage     = DocImage.getTypeSem() ;

	return true ;
}
catch (...)
{
	erreur("Exception NSModHtml::donneesImages", standardError, 0) ;
	return false ;
}
}

// cas des documents images (document Ixxxx)// que l'on veut visualiser => on la charge dans une base pour pouvoir
// l'afficher à l'aide d'un html.
bool
NSModHtml::chargeDocImage(string sPathBase, string sPathImages, NSDocumentInfo* pDocInfo)
{
try
{
	if (NULL == pDocInfo)
		return false ;

	bool pathInit = false ;
	if (sPathImages != "")
		pathInit = true ;

	// on récupère les infos de l'image
	string sCodeImage     = pDocInfo->getID() ;
	string sPathSource    = pContexte->PathName(pDocInfo->getLocalis()) ;
	string sFichierSource = pDocInfo->getFichier() ;
	string sTypeImage     = pDocInfo->getTypeSem() ;
	// creation de la base d'images  if ((NSBaseImages*) NULL == pBaseImages)  {		sBaseImages = sPathBase + nomSansDoublons(sPathBase, sCodeImage, "img") ;
		pBaseImages = new NSBaseImages(sBaseImages) ;
  }

	// si le path images n'est pas précisé : on prend le répertoire serveur lié au type
	if (!pathInit)
	{
  	if (pContexte->typeDocument(sTypeImage, NSSuper::isImageFixe))
    	sPathImages = pContexte->PathName("SIMG") ;
    else // cas des vidéos
    	sPathImages = pContexte->PathName("SVID") ;
	}

	string sExt = string(sTypeImage, 2, 3) ; // extension du fichier
  string sNomImage = nomSansDoublons(sPathImages, sCodeImage, sExt) ;

	if (!pBaseImages->ajouter(sNomImage, sPathImages, sPathSource, sFichierSource, sTypeImage))
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCopyAnImageToTheTemporaryLocation") ;
    sErrorText += string(" ") + sPathSource + sFichierSource + string(" -> ") + sPathImages + sNomImage ;

    string sSystemLastErrorMessage = getSystemLastErrorMessage() ;
    if (string("") != sSystemLastErrorMessage)
      sErrorText += string(" (") + sSystemLastErrorMessage + string(")") ;

    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	if (!pBaseImages->ecrire())
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("imagesManagementErrors", "cannotCreateImagesList") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

	return true ;
}
catch (...)
{
	erreur("Exception NSModHtml::chargeDocImage", standardError, 0) ;
	return false ;
}
}

boolNSModHtml::genereLienImage(int numImage, string sFichCompo, int largeur, int hauteur)
{
try
{
	string  sUrlCgi = pContexte->PathName("UCGI") ;
	char    sNum[4] ;
	char    sLargeur[6], sHauteur[6] ;
	bool    bExisteImage = false ;
	string  sFich, sBase ;

	// on vérifie d'abord que l'image est sélectionnée dans la base	if (pBaseImages)
	{
  	for (int i = 0; i < pBaseImages->nbImages; i++)
    {
    	if (pBaseImages->tableSelect[i] == numImage)
      {
      	bExisteImage = true ;
        break ;
      }
    }
  }

  // On extrait les noms de fichiers de sFichCompo et sBaseCompo
  // que l'on stocke dans les variables sFich et sBase qui sont
  // les paramètres relatifs que l'on envoie à select.exe
  size_t pos = sFichCompo.find_last_of("\\") ;
  if (NPOS == pos)
		sFich = sFichCompo ;
  else
		sFich = string(sFichCompo, pos+1, strlen(sFichCompo.c_str())-pos-1) ;

  pos = sBaseCompo.find_last_of("\\");
  if (NPOS == pos)
  	sBase = sBaseCompo ;
  else
  	sBase = string(sBaseCompo, pos+1, strlen(sBaseCompo.c_str())-pos-1) ;

  itoa(numImage, sNum, 10) ;
  itoa(largeur, sLargeur, 10) ;
  itoa(hauteur, sHauteur, 10) ;

	_sOut += string("<A HREF=\"") ;
	_sOut += sUrlCgi ;
  _sOut += string("select.exe?choix=") ;
  _sOut += string(sNum) ;
  _sOut += string("&largeur=") ;
  _sOut += string(sLargeur) ;
  _sOut += string("&hauteur=") ;
  _sOut += string(sHauteur) ;

  _sOut += string("&index=") ;
  if (bExisteImage)
  	_sOut += string(sNum) ;
	else
  	_sOut += "0" ;

  _sOut += string("&pannel=1") ;
  _sOut += string("&fich=") ;
  _sOut += sFich ;
  _sOut += string("&base=") ;
  _sOut += sBase ;
  _sOut += string("\">") ;

	if (bExisteImage)
  {
  	if (false == genereImageBase(numImage, largeur, hauteur))
    	return false ;
	}
  else
  	_sOut += string("image ") + string(sNum) ;

	_sOut += string("</A>") ;

	return true ;
}
catch (...)
{
	erreur("Exception NSModHtml::genereLienImage", standardError, 0) ;
	return false ;
}
}

voidNSModHtml::genereImage(string sUrlImg, string sNomImage, string sTypeImage,
								int largeur, int hauteur)
{
try
{
	if (pContexte->typeDocument(sTypeImage, NSSuper::isImageFixe))
		_sOut += string("<IMG SRC=\"") ;
	else // cas des vidéos
		_sOut += string("<IMG START=1 LOOP=0 DYNSRC=\"") ;

	_sOut += sUrlImg ;
	_sOut += sNomImage ;
	_sOut += string("\"") ;

	if (largeur && hauteur)
	{
  	char taille[80] ;
		sprintf(taille, " width=\"%d\" height=\"%d\"", largeur, hauteur) ;
		_sOut += string(taille) ;
	}

	_sOut += string(">") ;
}
catch (...)
{
	erreur("Exception NSModHtml::genereImage", standardError, 0) ;
}
}

// Fonction de transformation des PatPatho en stringbool
NSModHtml::generePatPatho(NSPatPathoArray* pPatPathoArray)
{
	if (NULL == pPatPathoArray)
		return false ;

	if (pContexte->getPatient() == 0)
	{
		erreur("Vous devez choisir un patient.",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	_sOut += string("{PatPatho ") +
           string("CN040") + // string(_pDocBrut->pDocInfo->pDonnees->type) +
           string("|") + pContexte->getPatient()->getNom() +
           string("|") + pContexte->getPatient()->getPrenom() +
           string("|") + pContexte->getPatient()->getNaissance() +
           string("|") + pContexte->getPatient()->getSexe() +
           string("}") ;

	if (false == pPatPathoArray->empty())	{  	string sPathoChain = string("") ;    pPatPathoArray->genereChaine(pContexte, &sPathoChain) ;    _sOut += sPathoChain ;	}

	_sOut += string("{/PatPatho}") ;

	return true ;
}

// Generation du fichier en-tete (par extension des fichiers HTML bruts)// cette fonction efface les tags <html> et <body> du fichier original
// et insère le fichier obtenu dans sModele (le fichier à générer) à la
// place du tag sNomTag (qui doit etre dans sModele).
bool
NSModHtml::genereHtmlBrut(string sNomTag, string sNomFichier)
{
	if (string("") == sNomFichier)
		return false ;

  ifstream 	inFile ;
	inFile.open(sNomFichier.c_str()) ;
	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sNomFichier ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
	}

  string fichier = "" ;
  string line ;
	while (!inFile.eof())
	{
  	getline(inFile,line) ;
    if (line != "")
    	fichier += line + string("\n") ;
	}
	inFile.close() ;

  string tag = string("[") + sNomTag + string("]") ;

	size_t pos = fichier.find("<html>");
	if (NPOS == pos)
		pos = fichier.find("<HTML>") ;

	if (NPOS != pos)
  	fichier.replace(pos, 6, "") ;

	pos = fichier.find("<body>") ;
	if (NPOS == pos)
		pos = fichier.find("<BODY>") ;

	if (NPOS != pos)
  	fichier.replace(pos, 6, "") ;

	pos = fichier.find("</body>") ;
	if (NPOS == pos)
		pos = fichier.find("</BODY>") ;

	if (NPOS != pos)
		fichier.replace(pos, 7, "") ;

	pos = fichier.find("</html>") ;
	if (NPOS == pos)
		pos = fichier.find("</HTML>") ;

	if (NPOS != pos)
  	fichier.replace(pos, 7, "") ;

	pos = sModele.find(tag) ;

	// modif RS du 08/04/04 : on remplace éventuellement plusieurs fois le tag
	// (cf cas des en-tete pour l'odonnance bi-zone)
	while (NPOS != pos)	{  	sModele.replace(pos, strlen(tag.c_str()), fichier.c_str()) ;
    pos = sModele.find(tag, pos + strlen(fichier.c_str())) ;
	}
	return true ;
}

// recherche d'une url à partir d'un cheminbool
NSModHtml::chercheUrl(string sChemin, string& sUrl)
{
	if (sChemin == "")
		return false ;

	string sUnite   = "" ;
  string sServeur = "" ;
	string sRacineServeur = pContexte->PathName("RSRV", &sUnite) ;

	size_t pos1 = sChemin.find(sRacineServeur) ;

	// Si le chemin n'est pas sous le serveur :
	// on renvoie le chemin ou vide pour toExporter
	if (NPOS == pos1)
	{
  	if (typeOperation == toExporter)
    	sUrl = "" ;
    else
    	sUrl = sChemin ;
    return true;
	}

	string sSupport ;
	if (sUnite != "")
  	sSupport = sUnite ;
	else
  	sSupport = sServeur ;

	// on enlève le support du chemin car on doit passer un chemin relatif à CodeChemin
  //
  string sCheminRelatif = string("") ;
	if (string("") != sSupport)
	{
  	size_t pos2 = sChemin.find(sSupport) ;

    if (NPOS != pos2)
    {
    	pos2 = pos2 + strlen(sSupport.c_str()) ;
      sCheminRelatif = string(sChemin, pos2, strlen(sChemin.c_str())-pos2) ;
    }
    else // cas d'erreur
    {
    	erreur("Le support du serveur n'a pas été retrouvé dans le chemin cherché.",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
      sCheminRelatif = sChemin ;
    }
  }
  else
  	sCheminRelatif = sChemin ;

	string sLocalis = pContexte->CodeChemin(sCheminRelatif, 'S') ;
	if (string("") == sLocalis)
  	return false ;

	// si le chemin contient la racine du serveur
	string sCodeUrl = sLocalis ;
	sCodeUrl[0] = 'U' ;
	sUrl = pContexte->PathName(sCodeUrl) ;

	return true ;
}

void
NSModHtml::recupTailleImage(int numImage, string sParam, int& largeur, int& hauteur)
{
	// taille par défaut
  if (string("") == sParam)
  {
  	largeur = 150 ;
    hauteur = 140 ;
    return ;
	}

  size_t pos = sParam.find(' ') ;
	if (NPOS != pos)
	{
  	string sLargeur = string(sParam, 0, pos) ;
    string sHauteur = string(sParam, pos+1, strlen(sParam.c_str()) - pos - 1) ;
    largeur = atoi(sLargeur.c_str()) ;
    hauteur = atoi(sHauteur.c_str()) ;
    if (largeur || hauteur)
    {
    	// Attention si les deux sont à 0 on laisse tel quel
      // (pour pouvoir afficher l'image en taille réelle)
      if (0 == largeur)
      	largeur == 150 ;
      if (0 == hauteur)
      	hauteur == 140 ;
    }
  }
  else
  {
  	char msg[255] ;
    sprintf(msg, "Erreur de syntaxe tag image n°%d", numImage) ;
    erreur(msg, standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    largeur = 150 ;
    hauteur = 140 ;
	}
}

bool
NSModHtml::genereImageBase(int numImage, int largeur, int hauteur)
{
	string sUrlImg ;
	string sImage ;
  string sTypeImage ;
  bool   bIsSelect = false ;

	for (int k = 0 ; k < pBaseImages->nbImages ; k++)	{
  	if (pBaseImages->tableSelect[k] == numImage)
    {
    	sImage = pBaseImages->tableImages[k] ;
      sTypeImage = pBaseImages->tableTypImg[k] ;

      // on recherche l'url associée au path de l'image k
      if (!chercheUrl(pBaseImages->tableDesImg[k], sUrlImg))
      {
      	char msg[100] ;
        sprintf(msg,"Erreur à la recherche de l'URL de l'image n°%d de la base %s",k+1,sBaseImages.c_str()) ;
        erreur(msg,standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }

      bIsSelect = true ;
      break ;
    }
  }

  // Dans le cas ou l'image ne correspond pas à une sélection
  // on ne met plus d'alerte, on se contente de ne rien générer
  // Cela règle le problème des feuilles d'images
  /***********************************************************************
  if (!bIsSelect)
  {
      char msg[100];
      char cOut[80];
      sprintf(msg,"L'image n°%d du modele est introuvable dans le document",numImage);
      erreur(msg,0,0);
      sprintf(cOut, "image %d", numImage);
      _sOut += string(cOut);
      return true;
  }
  *************************************************************************/

	if (bIsSelect)
  	genereImage(sUrlImg, sImage, sTypeImage, largeur, hauteur) ;

	return true ;
}

//// Pour les types énumérés qui correspondent à des html structurés
//
void
NSModHtml::remplaceTag(string sTag, string params, string sPath)
{
	// on cherche d'abord les tags génériques
	if (sTag == "nomPatient")
	{
		pContexte->getPatient()->fabriqueNomLong() ;
    _sOut += pContexte->getPatient()->getNomLong() ;
    return ;  }

  string sLang = pContexte->getUserLanguage() ;

  if (string("dateJour") == sTag)
  {
    string sDateJ = donne_date_duJour() ;
    _sOut += donne_date(sDateJ, sLang) ;
    return ;
	}

	// on cherche ensuite les tags structurés
	// si sPath est non vide, il peut y en avoir plusieurs
	//
	if (string("") == sPath)
	{
  	for (int i = 0 ; i < nbHtml ; i++)
    {
      NSHtml* pHtml = blocHtml[i]->getHtml() ;

    	if (pHtml && (pHtml->getType() == sTag) && blocHtml[i]->occur())
      {
      	if (string("") != params)
          pHtml->initTitres(params) ;

        pHtml->genere(_sOut) ;

      	// modif du 08/04/04 : on laisse occur à 1 pour l'ordonnance
        // blocHtml[i]->occur -= 1;
        return ;
      }
    }
  }
  else
  {
  	bool bTagFound = false ;
    for (int i = 0; i < nbHtml; i++)
    {
      NSHtml* pHtml = blocHtml[i]->getHtml() ;

    	if (pHtml && (pHtml->getType() == sTag) && (blocHtml[i]->occur()))
      {
      	bTagFound = true ;

        string sPathSens ;
        string sBlockPath = pHtml->getPath() ;
        pContexte->getDico()->donneCodeSens(&sBlockPath, &sPathSens) ;

        if (sPathSens == sPath)
        {
        	if (string("") != params)
          	pHtml->initTitres(params) ;

          pHtml->genere(_sOut) ;

        	// modif du 08/04/04 : on laisse occur à 1 pour l'ordonnance
          // blocHtml[i]->occur -= 1;
        }
        else
        	remplacePathTag(sPath, params, pHtml) ;
      }
    }
    if (bTagFound)
    	return ;
	}

	// Pour les tags inconnus, on ne fait plus de controle d'erreur
	// à cause des tags XML de type [if ...] [endif].
	// On replace donc simplement le tag tel quel
	_sOut += string("[") + sTag ;
	if (params != "")
		_sOut += string(" ") + params ;
	_sOut += string("]") ;
}

// Replacing a given tag with all fitting elements
//
void
NSModHtml::remplacePathTag(string sPath, string sParams, NSHtml* pBloc)
{
	if (((NSHtml*) NULL == pBloc) || (string("") == sPath))
		return ;
	if (pBloc->_filsArray.empty())
  	return ;

  bool   bFirstGoodEncountered = false ;
  string sSuffix = string("") ;

	bool bGood = false ;
	for (HtmlIter HtmlIt = pBloc->_filsArray.begin() ; pBloc->_filsArray.end() != HtmlIt ; HtmlIt++)
	{
    // Son with no path
    //
  	if (string("") == (*HtmlIt)->getPath())
    {
    	if (bGood)
      {
      	if (string("") != sParams)
        	(*HtmlIt)->initTitres(sParams) ;

        if ((false == bFirstGoodEncountered) && (string("") != (*HtmlIt)->getPrefix()))
        {
          bFirstGoodEncountered = true ;
          (*HtmlIt)->genereTexte((*HtmlIt)->getPrefix(), _sOut) ;
          sSuffix = (*HtmlIt)->getSuffix() ;
        }

        (*HtmlIt)->genere(_sOut) ;
      }
		}
    // Son with a path
    //
    else
    {
    	string sPathSens ;
      string sBlockPath = (*HtmlIt)->getPath() ;
      pContexte->getDico()->donneCodeSens(&sBlockPath, &sPathSens) ;

      // The path is the expected one
      //
      if (sPathSens == sPath)
      {
      	if (string("") != sParams)
        	(*HtmlIt)->initTitres(sParams) ;

        if ((false == bFirstGoodEncountered) && (string("") != (*HtmlIt)->getPrefix()))
        {
          bFirstGoodEncountered = true ;
          (*HtmlIt)->genereTexte((*HtmlIt)->getPrefix(), _sOut) ;
          sSuffix = (*HtmlIt)->getSuffix() ;
        }

        (*HtmlIt)->genere(_sOut) ;
        bGood = true ;
      }
      // Path is not expected one, but does it start by expected one?
      //
      else
      {
      	size_t iPathLen  = strlen(sPath.c_str()) ;
        size_t iLocalLen = strlen(sPathSens.c_str()) ;

        if (iLocalLen == iPathLen)
        	bGood = false ;
        else if (iLocalLen > iPathLen)
        {
        	//
          // Le chemin de l'élément en cours commence par sPath : on le traite
          //
          if (string(sPathSens, 0, strlen(sPath.c_str())) == sPath)
          {
            if (string("") != sParams)
        	    (*HtmlIt)->initTitres(sParams) ;

            if ((false == bFirstGoodEncountered) && (string("") != (*HtmlIt)->getPrefix()))
            {
              bFirstGoodEncountered = true ;
              (*HtmlIt)->genereTexte((*HtmlIt)->getPrefix(), _sOut) ;
              sSuffix = (*HtmlIt)->getSuffix() ;
            }

          	(*HtmlIt)->genere(_sOut) ;
            bGood = true ;
          }
          else
          	bGood = false ;
        }
        else
        {
        	bGood = false ;
          //
          // Le chemin de l'élément en cours est contenu dans sPath, on itère sur ses fils
          //
          if (string(sPath, 0, strlen(sPathSens.c_str())) == sPathSens)
          	remplacePathTag(sPath, sParams, *HtmlIt) ;
        }
      }
    }
	}

  if ((bFirstGoodEncountered) && (string("") != sSuffix))
    NSHtml::genereTexte(sSuffix, _sOut) ;
}

//
// Pour les types string (avec tableaux)
//
bool
NSModHtml::valeurTag(string sTag, string& sValeurTag, NSBlocHtmlMatriceIter iterTable)
{
  for (int i = 0 ; i < nbHtml ; i++)
  {
    NSHtml* pHtml = blocHtml[i]->getHtml() ;

    if (pHtml && (pHtml->getType() == sTag))
    {
      sValeurTag = pHtml->getText() ;
      return true ;
    }
  }

  if (false == (*iterTable)->empty())
  {
    for (NSBlocHtmlArrayIter itCel = (*iterTable)->begin() ; (*iterTable)->end() != itCel ; itCel++)
    {
      NSHtml* pHtml = (*itCel)->getHtml() ;

      if (pHtml && (pHtml->getType() == sTag))
      {
        sValeurTag = pHtml->getText() ;
        return true ;
      }
    }
  }

  return false ;
}

intNSModHtml::compteTags(string sTag)
{
  int nbTags = 0 ;

  // On cherche le tag sous la forme "[image]" puis sous la forme "[image "
  // car il peut y avoir des paramètres (par exemple la taille de l'image
  // comme [image 50 60])
  //
	string sTagR = string("[") + sTag + string("]");

  size_t pos = sModele.find(sTagR) ;
  while ((NPOS != pos) && ((pos + strlen(sTagR.c_str())) <= strlen(sModele.c_str())))
  {
    nbTags++ ;
    pos = sModele.find(sTagR, pos + strlen(sTagR.c_str())) ;
  }

  sTagR = string("[") + sTag + string(" ") ;

  pos = sModele.find(sTagR) ;
  while ((NPOS != pos) && ((pos + strlen(sTagR.c_str())) <= strlen(sModele.c_str())))
  {
    nbTags++;
    pos = sModele.find(sTagR, pos + strlen(sTagR.c_str()));
  }

	return nbTags ;}

// Avril 04 : scan dans la template des paramètres d'impression pour WebBrowserPrint
void
NSModHtml::scanPrintParams()
{
    string  sComment;
    size_t  comment_begin = 0;
    size_t  comment_end;
    bool    bParamsFound = false;
    size_t  params, tag, equal, value_begin, value_end;
    string  sTag, sValue;

    // on extrait au préalable les champs commentaires
    comment_begin = sModele.find("<!--");

    while ((!bParamsFound) && (comment_begin != NPOS))
    {
        comment_end = sModele.find("-->", comment_begin + 4);

        if (comment_end != NPOS)
        {
            sComment = string(sModele, comment_begin + 4, comment_end - comment_begin - 4);
            // on a trouvé un commentaire et on cherche un paramètre d'impression
            params = sComment.find("PrintParams");

            if (NPOS != params)
            {
                // Reset des paramètres d'impression du document
                _pDocBrut->_nspp.reset();

                int iLenTagParams = strlen("PrintParams");
                sComment = string(sComment, iLenTagParams + 1, strlen(sComment.c_str()) - iLenTagParams - 1);
                tag = sComment.find_first_not_of(' ');

                while (tag != NPOS)
                {
                    equal = sComment.find('=', tag);
                    if ((equal != NPOS) && (equal > tag))
                    {
                        sTag = "";
                        sValue = "";
                        bParamsFound = true;

                        for (size_t i = tag; (i < equal) && (sComment[i] != ' '); i++)
                            sTag += sComment[i];

                        value_begin = sComment.find('\"', equal + 1);

                        if (value_begin != NPOS)
                        {
                            value_end = sComment.find('\"', value_begin + 1);

                            if (value_end != NPOS)
                            {
                                sValue = string(sComment, value_begin + 1, value_end - value_begin - 1);
                            }
                            else
                            {
                                erreur("Erreur de syntaxe dans la définition des paramètres d'impression.", standardError, 0);
                                return;
                            }
                        }
                        else
                        {
                            erreur("Erreur de syntaxe dans la définition des paramètres d'impression.", standardError, 0);
                            return;
                        }

                    }
                    else
                    {
                        erreur("Erreur de syntaxe dans la définition des paramètres d'impression.", standardError, 0);
                        return;
                    }

                    // Setting du paramètre d'impression
                    if (bParamsFound)
                    {
                        if (sTag == "PaperSize")
                            _pDocBrut->_nspp.sPaperSize = sValue;
                        else if (sTag == "PaperSource")
                            _pDocBrut->_nspp.sPaperSource = sValue;
                        else if (sTag == "Header")
                            _pDocBrut->_nspp.sHeader = sValue;
                        else if (sTag == "Footer")
                            _pDocBrut->_nspp.sFooter = sValue;
                        else if (sTag == "Orientation")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "UNDEFINED") || (sValue == "PORTRAIT") || (sValue == "LANDSCAPE"))
                                _pDocBrut->_nspp.sOrientation = sValue;
                        }
                        else if (sTag == "LeftMargin")
                            _pDocBrut->_nspp.sfLeftMargin = sValue;
                        else if (sTag == "TopMargin")
                            _pDocBrut->_nspp.sfTopMargin = sValue;
                        else if (sTag == "RightMargin")
                            _pDocBrut->_nspp.sfRightMargin = sValue;
                        else if (sTag == "BottomMargin")
                            _pDocBrut->_nspp.sfBottomMargin = sValue;
                        else if (sTag == "PrinterName")
                            _pDocBrut->_nspp.sPrinterName = sValue;
                        else if (sTag == "PrintToFile")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "TRUE") || (sValue == "FALSE") || (sValue == "YES") || (sValue == "NO"))
                                _pDocBrut->_nspp.sbPrintToFile = sValue;
                        }
                        else if (sTag == "PrintRange")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "UNDEFINED") || (sValue == "ALL") || (sValue == "PAGES") ||
                                (sValue == "SELECTION"))
                                _pDocBrut->_nspp.sPrintRange = sValue;
                        }
                        else if (sTag == "PrintRangePagesFrom")
                            _pDocBrut->_nspp.slPrintRangePagesFrom = sValue;
                        else if (sTag == "PrintRangePagesTo")
                            _pDocBrut->_nspp.slPrintRangePagesTo = sValue;
                        else if (sTag == "Collate")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "TRUE") || (sValue == "FALSE") || (sValue == "YES") || (sValue == "NO"))
                                _pDocBrut->_nspp.sbCollate = sValue;
                        }
                        else if (sTag == "PrintFrames")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "UNDEFINED") || (sValue == "SCREEN") || (sValue == "SELECTED") ||
                                (sValue == "INDIVIDUALLY"))
                                _pDocBrut->_nspp.sPrintFrames = sValue;
                        }
                        else if (sTag == "PrintLinks")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "TRUE") || (sValue == "FALSE") || (sValue == "YES") || (sValue == "NO"))
                                _pDocBrut->_nspp.sbPrintLinks = sValue;
                        }
                        else if (sTag == "PrintLinkTable")
                        {
                            pseumaj(&sValue);

                            if ((sValue == "TRUE") || (sValue == "FALSE") || (sValue == "YES") || (sValue == "NO"))
                                _pDocBrut->_nspp.sbPrintLinkTable = sValue;
                        }
                    }

                    // Recherche du paramètre suivant
                    if (value_end < (strlen(sComment.c_str()) - 1))
                        tag = sComment.find_first_not_of(' ', value_end + 1);
                    else
                        break;
                }
            }

            if (!bParamsFound)
                comment_begin = sModele.find("<!--", comment_end + 3);
        }
        else // erreur de syntaxe dans un commentaire : on sort
            return;
    }
}

voidNSModHtml::processTable()
{
  NSBlocHtmlMatriceIter iterTable = _table.begin() ;

  size_t posTag = sModele.find("[TBL") ;

  while (NPOS != posTag)
  {
    // on cherche la TR englobante

    size_t pos1 = sModele.rfind("<TR", posTag) ;
    size_t pos2 = sModele.find("</TR>", posTag) ;

    pos2 += strlen("</TR>") ;

    string sPaquet = "";
    string sLigneGen = string(sModele, pos1, pos2 - pos1) ;

    while (_table.end() != iterTable)
    {
      string sLigne = sLigneGen ;
      posTag = sLigne.find("[") ;

      while (NPOS != posTag)
      {
        string sNewLigne = string(sLigne, 0, posTag) ;

        size_t posFinTag = sLigne.find("]", posTag) ;
        string sTag = string(sLigne, posTag + 1, posFinTag - posTag - 1) ;

        string sValTag = string("") ;
        valeurTag(sTag, sValTag, iterTable) ;

        sNewLigne += sValTag ;
        sNewLigne += string(sLigne, posFinTag + 1, strlen(sLigne.c_str()) - posFinTag - 1) ;

        sLigne = sNewLigne ;
        posTag = sLigne.find("[") ;
      }

      sPaquet += sLigne + string("\n") ;
      iterTable++ ;
    }

    sModele.replace(pos1, pos2 - pos1, sPaquet) ;

    iterTable = _table.begin() ;

    posTag = sModele.find("[TBL", pos1 + strlen(sPaquet.c_str())) ;
	}
}

// reconstitution du fichier html à partir d'une templatebool
NSModHtml::genereModele(string sFichHtml, NSDocumentInfo* pHtmlInfo, string sPathBase,
						string sPathImages, string sFichierExterne, string sTypeFichier)
{

  int      numImage    = 0 ;
  bool     baseChargee = false ;
  bool     bExterne    = false ;

	// note : si le path images est précisé
	// on accèdera aux images directement par le path (FTP)
	// sans passer par une URL (HTTP)

	// s'il s'agit d'un fichier externe (et non d'un document)
	if (string("") != sFichierExterne)
  	bExterne = true ;

	if (bExisteTable)
  	processTable() ;

	// cas du fichier en-tete
  if (string("") != sFichierEnTete)
	{
  	if (false == genereHtmlBrut("en-tete", sFichierEnTete))
    	return false ;
	}

  ofstream outFile ;
	outFile.open(sFichHtml.c_str());
	if (!outFile)
	{
  	string sErreur = string("Impossible d'ouvrir ou de créer le fichier html à générer : ")
                                + sFichHtml ;
    erreur(sErreur.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
	}

	size_t _il = 0 ;

	while (_il < strlen(sModele.c_str()))
	{
  	while ((_il < strlen(sModele.c_str())) && ('[' != sModele[_il]))
    	_sOut += sModele[_il++] ;

    if (_il < strlen(sModele.c_str()))
    {
      // on analyse le tag
      _il++ ;
      string typeElt = string("") ;
      string params  = string("") ;
      string sPath   = string("") ;

      while ((_il < strlen(sModele.c_str())) && (']' != sModele[_il]) &&
                                                ('|' != sModele[_il]) &&
                                                (' ' != sModele[_il]))
      	// récupération du type
        typeElt += sModele[_il++] ;

      if ((_il < strlen(sModele.c_str())) && ('|' == sModele[_il]))
      {      	_il++ ;
        while ((_il < strlen(sModele.c_str())) && (']' != sModele[_il]) &&
                                                  ('|' != sModele[_il]) &&
                                                  (' ' != sModele[_il]))
        	sPath += sModele[_il++] ;

        if ((_il < strlen(sModele.c_str())) && ('|' == sModele[_il]))
        	_il++ ;
      }

      if ((_il < strlen(sModele.c_str())) && (' ' == sModele[_il]))      {      	_il++ ;
        while ((_il < strlen(sModele.c_str())) && (']' != sModele[_il]))
        	params += sModele[_il++] ;
      }

      if (_il >= strlen(sModele.c_str()))      {
      	erreur("Erreur syntaxique dans le fichier modèle.",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
        outFile.close() ;
        return false ;
      }

      /* modele[i] == ']' */
      _il++ ;

      if (string("image") == typeElt)
      {
      	int iLargeur, iHauteur ;

        if (false == baseChargee) // on vérifie si la base est chargée
        {
        	if (pHtmlInfo)
          {
          	// on charge la base des images du document
            if (false == chargeBaseImages(sPathBase, sPathImages, pHtmlInfo))
            {
            	erreur("Impossible de récupérer la base d'images", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
              outFile.close() ;
              return false ;
            }
          }

          // si on compose : on charge la base de composition
          if (toComposer == typeOperation)
          {
          	if (false == chargeBaseImages(sPathBase, sPathImages, false, pHtmlInfo))
            {
            	erreur("Impossible de créer la base d'images", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
              outFile.close() ;
              return false ;
            }
          }

          baseChargee = true ;
        }

        numImage++ ;

        // on récupère la taille (largeur,hauteur) de l'image
        recupTailleImage(numImage, params, iLargeur, iHauteur) ;

        // on génère l'image ...
        if (toComposer == typeOperation)
        {
        	if (string("") != sBaseCompo)
          {
          	// on genere un lien vers select s'il existe au moins 1 image
            if (false == genereLienImage(numImage, sFichHtml, iLargeur, iHauteur))
            {
              outFile.close() ;
            	return false ;
            }
          }
        }
        else // autres cas : on récupère le composant image
        {
        	if (sBaseImages != "")
          {
          	if (!genereImageBase(numImage, iLargeur, iHauteur))
            {
              outFile.close() ;
            	return false ;
            }
          }
        }
      }
      // cas des documents images ou vidéos (cf visualisation)
      else if (string("docimage") == typeElt)
      {
      	string sImage, sTypeImage ;
        string sPathDestImage, sUrlImg ;

        // cas des fichiers externes
        if (bExterne)
        {
        	// nom de l'image = nom du fichier source
          sImage         = sFichierExterne ;
          sTypeImage     = sTypeFichier ;
          sPathDestImage = sPathImages ;
        }
        else // cas des images référencées en tant que document nautilus
        {
        	if (NULL == pHtmlInfo)
          {
          	erreur("Les données de l'image ne sont pas initialisées", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
            outFile.close() ;
            return false ;
          }

          // on crée une base d'image contenant le document image
          if (false == chargeDocImage(sPathBase, sPathImages, pHtmlInfo))
          {
          	erreur("Impossible de générer le fichier html image", standardError, 0, pContexte->GetMainWindow()->GetHandle());
            outFile.close() ;
            return false ;
          }

          sImage         = pBaseImages->tableImages[0] ;          sTypeImage     = pBaseImages->tableTypImg[0] ;
          sPathDestImage = pBaseImages->tableDesImg[0] ;
        }

        // on recherche l'url associée au path de l'image
        if (false == chercheUrl(sPathDestImage, sUrlImg))
        {
        	/*
          char msg[100];
          sprintf(msg,"Erreur à la recherche de l'URL d'une image de la base %s",sBaseImages.c_str());
          erreur(msg,standardError,0,pContexte->GetMainWindow()->GetHandle());
          */
        }

        // On génère l'image en taille réelle
        genereImage(sUrlImg, sImage, sTypeImage) ;
      }
      // cas des compte-rendus : on ecrit la PatPatho
      else if (string("patpatho") == typeElt)
      {
        NSPatPathoArray PptDocBrut(pContexte->getSuperviseur()) ;
        _pDocBrut->initFromPatPatho(&PptDocBrut) ;
      	if (false == generePatPatho(&PptDocBrut))
        {
        	erreur("Impossible de générer la PatPatho", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          outFile.close() ;
          return false ;
        }
      }
      // cas des html structurés : on remplace le tag par un bloc du tableau blocHtml
      else
      	remplaceTag(typeElt, params, sPath) ;
    }
	}

	for (size_t _i = 0 ; _i < strlen(_sOut.c_str()) ; _i++)
		outFile.put(_sOut[_i]) ;

	outFile.close() ;
	return true ;
}

// appel des fonctions nécéssaires pour générer un fichier html (générique)// Le fichier html est ici constitué à partir d'un document pour
// la composition (pHtmlInfo = 0), la publication		[ cas des HDHTM ]
// ou la visualisation (pHtmlInfo <- pDocInfo)			[ cas des HSHTM ]
// On utilise obligatoirement HTTP - Les images sont au besoin copiées
// dans le répertoire Images du serveur
// Pour le cas de l'exportation, on ne passe pas par HTTP :
// on précise alors sPathDest
bool
NSModHtml::genereHtml(string sPathHtml, string& sBaseImg,
                           NSDocumentInfo* pHtmlInfo, string sPathDest)
{
	string sPathBase = string("") ;
  string sPathImg  = string("") ;

  // Répertoire par défaut de la base d'images
  if (string("") == sPathDest)
  {
    sPathBase = pContexte->PathName("SHTM") ;
    // Par défaut : Retrouve le path en fonction du type image
    sPathImg = string("") ;
  }
  else // cas de l'exportation : on copie les images dans sPathDest
  {
    sPathBase = sPathDest ;
    sPathImg = sPathDest ;
  }

	if ((string("") != _pDocBrut->_sTemplate) &&
      (false == lireModele(_pDocBrut->_sTemplate, _pDocBrut->_sEnTete)))
  {
    char msg[255] ;
    sprintf(msg, "Pb lecture du modele [%s].", (_pDocBrut->_sTemplate).c_str()) ;
    MessageBox(0, msg, 0, MB_OK) ;
    return false ;
  }

  if (false == genereModele(sPathHtml, pHtmlInfo, sPathBase, sPathImg))
  {
    MessageBox(0, "Pb écriture du fichier HTML", 0, MB_OK) ;
    return false ;
  }

  // on renvoie le nom de la base d'images
  if (toComposer == typeOperation)
  {
    sBaseImg = sBaseCompo ;
    _pDocBrut->_sBaseImages = sBaseImages ;
  }
  else
  {
    if (toExporter == typeOperation)
      _pDocBrut->_bConserveBase = true ;

    sBaseImg = sBaseImages ;
  }

  return true ;
}

// This function is dedicated to LdV document publication
//
bool
NSModHtml::genereHtml(string sPathHtml, string& sBaseImg,
   					           string sFichierTemplate, string sFichierEntete)
{
  string sPathBase = pContexte->PathName("SHTM") ;
  // Par défaut : Retrouve le path en fonction du type image
  string sPathImg = string("") ;

  if (string("") != sFichierTemplate)
    sFichierTemplate = pContexte->PathName("NTPL") + sFichierTemplate ;
  if (string("") != sFichierEntete)
    sFichierEntete = pContexte->PathName("NTPL") + sFichierEntete ;

  if (false == lireModele(sFichierTemplate, sFichierEntete))
  {
    char msg[255] ;
    sprintf(msg, "Pb lecture du modele [%s].", (_pDocBrut->_sTemplate).c_str()) ;
    MessageBox(0, msg, 0, MB_OK) ;
    return false ;
  }

  if (false == genereModele(sPathHtml, 0, sPathBase, sPathImg))
  {
    MessageBox(0, "Pb écriture du fichier HTML", 0, MB_OK) ;
    return false ;
  }
  return true ;
}

// appel des fonctions nécéssaires pour générer un fichier html (générique)
// Le fichier html est ici constitué à partir d'un fichier externe
// On transmet le chemin des images, auxquelles on accède directement, sans passer
// par une base d'images
bool
NSModHtml::genereHtml(string sTemplate, string sPathHtml, string sPathSource,
						string sNomFichier, string sTypeFichier)
{
	if (false == lireModele(sTemplate))
  {
    char msg[255] ;
    sprintf(msg, "Pb lecture du modele [%s].", sTemplate.c_str()) ;
    MessageBox(0, msg, 0, MB_OK) ;
    return false ;
  }

  // generation du fichier html
  // (on génère la base d'images et les images au meme endroit pour l'instant)
  if (false == genereModele(sPathHtml, 0, sPathSource, sPathSource, sNomFichier, sTypeFichier))
  {
    MessageBox(0, "Pb écriture du fichier HTML", 0, MB_OK) ;
    return false;
  }

  return true ;
}

// fin du fichier nsmodhtm.cpp

