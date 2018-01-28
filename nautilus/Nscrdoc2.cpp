//----------------------------------------------------------------------------// ObjectWindows - (C) Copyright 1991, 1993 by Borland International
//   Tutorial application -- step12dv.cpp
//----------------------------------------------------------------------------

#include <owl\filedoc.h>
#include <iostream.h>
#include <cstring.h>

#include "nautilus\nssuper.h"
#include "nautilus\nsdecode.h"
#include "nsbb\nsbb.h"
#include "nautilus\nsrechd2.h"
#include "dcodeur\nsdkd.h"

#include "nautilus\nscrdoc.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nscrvue.h"
#include "nautilus\ns_html.h"
#include "nautilus\nsldvdoc.h"

#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"

// --------------------------------------------------------------------------
// ---------------------- METHODES DE NSCRDocument --------------------------
// --------------------------------------------------------------------------

bool
afficheCompteRendu(NSDocumentInfo& Document, NSContexte *pCtx)
{
	return false ;
}

//---------------------------------------------------------------------------//
//	Constructeur à partir d'un document existant
//
//---------------------------------------------------------------------------NSCRDocument::NSCRDocument(TDocument* parent, NSDocumentInfo* pDocumentInfo,
                           NSDocumentInfo* pDocHtmlInfo, NSContexte* pCtx,
                           string sTypeCompteRendu, bool bROnly)
             :NSRefDocument(parent, pDocumentInfo, pDocHtmlInfo, pCtx, bROnly)
{
	_EstDecode  = false ;
	_sFichDecod = string("") ;
	_pBigBoss   = (NSSmallBrother*) 0 ;

	strcpy(_lexiqueModule, sTypeCompteRendu.c_str()) ;
}

//---------------------------------------------------------------------------
//
//	Constructeur pour un nouveau document
//
//---------------------------------------------------------------------------
NSCRDocument::NSCRDocument(TDocument* parent, NSContexte* pCtx, string sTypeCompteRendu)
             :NSRefDocument(parent, pCtx)
{
	//sCompteRendu = "";
	if (pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))
		_bReadOnly = false ;
  else
		_bReadOnly = true ;

	_EstDecode  = false ;
	_sFichDecod = string("") ;
	_pBigBoss   = (NSSmallBrother*) 0 ;

	strcpy(_lexiqueModule, sTypeCompteRendu.c_str()) ;
}

//---------------------------------------------------------------------------
//
//	Constructeur pour un nouveau document copie d'un document existant
//
//---------------------------------------------------------------------------
NSCRDocument::NSCRDocument(NSDocumentHisto* pDocumentModele, TDocument* parent, NSContexte* pCtx)
             :NSRefDocument(parent, pCtx)
{
  if (pContexte->userHasPrivilege(NSContexte::createDocument, -1, -1, string(""), string(""), NULL, NULL))
		_bReadOnly = false ;
  else
		_bReadOnly = true ;

	_EstDecode  = false ;
	_sFichDecod = string("") ;
	_pBigBoss   = (NSSmallBrother*) 0 ;

  strcpy(_lexiqueModule, "") ;

  if ((NSDocumentHisto*) NULL == pDocumentModele)
    return ;

  NSPatPathoArray* pModelPpt = pDocumentModele->getPatPathoPointer() ;
  if (((NSPatPathoArray*) NULL == pModelPpt) || pModelPpt->empty())
    return ;

  // Get root concept
  //
  NSPatPathoInfo* pRoot = *(pModelPpt->begin()) ;
  strcpy(_lexiqueModule, pRoot->getLexique().c_str()) ;

  // Inject model document's patpatho inside the new one
  //
  setPatPatho(pModelPpt) ;

  // Clear model document Ids and set date as "now"
  //
  _PatPathoArray.clearDocAndNodeIDs() ;

  string sDatePath = pRoot->getLexiqueSens() + string("/LADMI/KCHIR") ;
  PatPathoIter iDateIt = _PatPathoArray.ChercherItem(sDatePath, string("/")) ;
  if ((NULL != iDateIt) && (_PatPathoArray.end() != iDateIt))
  {
    int iCol = (*iDateIt)->getColonne() ;

    iDateIt++ ;
    while ((_PatPathoArray.end() != iDateIt) && ((*iDateIt)->getColonne() > iCol))
    {
      if ((*iDateIt)->getColonne() == iCol + 1)
      {
        string sSens = (*iDateIt)->getLexiqueSens() ;
        if ((string("£D0") == sSens) || (string("£T0") == sSens))
        {
          NVLdVTemps tNow ;
          tNow.takeTime() ;
          if      (string("£T0") == sSens)
            (*iDateIt)->setComplement(tNow.donneDateHeure()) ;
          else if (string("£D0") == sSens)
            (*iDateIt)->setComplement(tNow.donneDate()) ;
          break ;
        }
      }
      iDateIt++ ;
    }
  }

  SetDirty(true) ;
}

//---------------------------------------------------------------------------
//  Description : Constructeur copie
//---------------------------------------------------------------------------
NSCRDocument::NSCRDocument(const NSCRDocument& rv)
			 			 :NSRefDocument(rv)
{
try
{
	_EstDecode  = rv._EstDecode ;
  _sFichDecod = rv._sFichDecod ;

	if (rv._pBigBoss)
		_pBigBoss = new NSSmallBrother(*(rv._pBigBoss)) ;
	else
    _pBigBoss = (NSSmallBrother*) 0 ;

	strcpy(_lexiqueModule, rv._lexiqueModule) ;
	_aCRPhrases = rv._aCRPhrases ;
}
catch (...)
{
  erreur("Exception NSCRDocument ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Description : Destructeur
//---------------------------------------------------------------------------
NSCRDocument::~NSCRDocument()
{
  // Destruction du fichier temporaire
  if (string("") != _sFichDecod)
    NsDeleteTemporaryFile(pContexte->getSuperviseur(), _sFichDecod) ;

  // La Fiche Historique est prévenue par NSRefDocument
  // Le delete sur pPatPathoArray et sur pBigBoss est fait par NSRefDocument
}

//---------------------------------------------------------------------------//  Description : Operateur =
//---------------------------------------------------------------------------
NSCRDocument&
NSCRDocument::operator=(const NSCRDocument& src)
{
try
{
	if (this == &src)
		return *this ;

	// Appel de l'operateur = de NSRefDocument
	// (recopie des arguments de la classe NSRefDocument)
	NSRefDocument       *dest   = this ;
  const NSRefDocument *source = (NSRefDocument*) &src ;
	*dest = *source ;

	_EstDecode  = src._EstDecode ;
	_sFichDecod = src._sFichDecod ;

	if (src._pBigBoss)
  	_pBigBoss = new NSSmallBrother(*(src._pBigBoss)) ;
	else
		_pBigBoss = (NSSmallBrother*) 0 ;

	strcpy(_lexiqueModule, src._lexiqueModule) ;
	_aCRPhrases = src._aCRPhrases ;

	//	*paLesions 	 = *(src.paLesions);
	//	*paVariables = *(src.paVariables);

	return *this ;
}
catch (...)
{
	erreur("Exception NSCRDocument (= operator).", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------//  Ouvre le document en mettant le CR dans pPatPathoArray
//---------------------------------------------------------------------------
bool
NSCRDocument::Open(int mode, const char far* path)
{
	if ((NSDocumentInfo*) NULL == _pDocInfo)
		return true ;

	return _bDocumentValide ;
}

//---------------------------------------------------------------------------
//  Function: NSCRDocument::GenereHtml(string sPathHtml, string& sNomHtml, string sAdresseCorresp)
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
NSCRDocument::GenereHtml(string sPathHtml, string& sNomHtml,
                         OperationType typeOperation,
                         string sAdresseCorresp, string sPathDest)
{
	NSHtmlCR hcr(typeOperation, this, pContexte, sAdresseCorresp) ;

	// on trouve le nom du fichier temporaire à visualiser
	sNomHtml = hcr.nomSansDoublons(sPathHtml, sNomHtml, "htm") ;
	string sFichierHtml = sPathHtml + sNomHtml ;

  NSDocumentInfo* pComposInfo = _pHtmlInfo ;
  if ((NSDocumentInfo*) NULL == pComposInfo)
  	pComposInfo = _pDocInfo ;

	// on passe aussi le path destination pour les images
  string sBaseImg = string("") ;
	if (false == hcr.genereHtml(sFichierHtml, sBaseImg, pComposInfo, sPathDest))
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

bool
NSCRDocument::GenereHtmlText(string& sHtmlText)
{
  sHtmlText = string("") ;

  if (false == IsDecode())
  {
    if ((decodeCR(&_aCRPhrases)) != 0)
      return false ;
  }

  NSHtmlCR HtmlCR(tCR, this, pContexte, string("")) ;

  string sParams = string("T2(<font color=\"#000000\" face=\"TimesNewRoman\" size=\"4\"><strong>|</strong></font>)") ;
  sParams       += string("T3(<font color=\"#000000\" face=\"TimesNewRoman\" size=\"3\"><strong>|</strong></font>)") ;
  sParams       += string("T4(<font color=\"#000000\" face=\"TimesNewRoman\" size=\"3\">|</font>)") ;
  sParams       += string("T5(<font color=\"#000000\" face=\"TimesNewRoman\" size=\"3\"><strong>|</strong></font>)") ;
  sParams       += string("T7(<td>|</td>)") ;
  sParams       += string("T8(<tr align=\"center\">|</tr>)") ;
  sParams       += string("T9(<table border=2>|</table>)") ;

  string sOut = string("") ;

  for (int i = 0 ; i < HtmlCR.nbHtml ; i++)
  {
    NSHtml* pHtml = HtmlCR.blocHtml[i]->getHtml() ;
    if (pHtml && (pHtml->getType() == string("CR")) && (HtmlCR.blocHtml[i]->occur()))
    {
      if (string("") != sParams)
        pHtml->initTitres(sParams) ;

      pHtml->genere(sHtmlText) ;

      return true ;
    }
  }

  return false ;
}

bool
NSCRDocument::GenereRawText(string& sRawText)
{
  sRawText = string("") ;

  ifstream inFile ;
  inFile.open(_sFichDecod.c_str()) ;
  if (!inFile)
    return false ;

  while (!inFile.eof())
  {
    string sLine ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sRawText += sLine + string(NEWLINE) ;
  }
  inFile.close() ;

  size_t cc = 0 ;

  while (cc < strlen(sRawText.c_str()))
  {
    // lecture jusqu'au prochain marqueur ou fin de fichier
    //
    size_t iRawTextLen = strlen(sRawText.c_str()) ;
    while ((cc < iRawTextLen) && (27  != sRawText[cc]) &&
                                 (28  != sRawText[cc]) &&
                                 ('|' != sRawText[cc]))
      cc++ ;

    // Tree bookmark
    //
    if ('|' == sRawText[cc])
    {
      size_t iDeb = cc ;
      cc++ ;
      while ((cc < strlen(sRawText.c_str())) && (27  != sRawText[cc]) &&
                                                (28  != sRawText[cc]) &&
                                                ('|' != sRawText[cc]))
        cc++ ;

      if ('|' == sRawText[cc])
      {
        if (cc < strlen(sRawText.c_str()) - 1)
          sRawText = string(sRawText, 0, iDeb) + string(sRawText, cc + 1, strlen(sRawText.c_str()) - cc - 1) ;
        else
          sRawText = string(sRawText, 0, iDeb) ;

        cc = iDeb ;
      }
    }

    // Presentation tags
    //
    else if ((27  != sRawText[cc]) || (28  != sRawText[cc]))
    {
      if (cc < strlen(sRawText.c_str()) - 2)
        sRawText = string(sRawText, 0, cc) + string(sRawText, cc + 2, strlen(sRawText.c_str()) - cc - 2) ;
      else
        sRawText = string(sRawText, 0, cc) ;
    }
  }

  return true ;
}

//---------------------------------------------------------------------------//  Function: NSCRDocument::decodeCR(NSCRPhraseArray *pPhrases)
//
//  Arguments:
//
//  Description:
//			  Choisi un nom pour le fichier de décodage s'il n'existe pas déjà
//            Décode le compte rendu en langage naturel
//  Returns:
//            Rien
//---------------------------------------------------------------------------
int
NSCRDocument::decodeCR(NSCRPhraseArray *pPhrases)
{
try
{
	int Reussi ;
	TModule* pDCModule = (TModule*) 0 ;
	string sNomFichierDecode ;
	string sLocFichierDecode ;
	bool (FAR *pAdresseFct) (char, NSPatPathoArray far *, char far *,
                             NSContexte far *, NSCRPhraseArray far *,
                             NSDocumentData far *);

	if (_PatPathoArray.empty())
  	return iErreur ;

  // On choisi un nom pour le fichier de décodage (s'il n'existe pas)
	if (string("") == _sFichDecod)
  {
  	if (false == TrouveNomFichier(string("ZCN00"), string("tmp"), sNomFichierDecode, sLocFichierDecode))
    {
    	erreur("Erreur à l'attribution du nom du fichier temporaire.", standardError, 0, pContexte->GetMainWindow()->GetHandle());
      return iErreur ;
    }

    _sFichDecod = pContexte->PathName(sLocFichierDecode) + sNomFichierDecode ;
  }
  //
  // Le premier élément du PatPathoArray donne le type d'examen
  //
  PatPathoIter iJ = _PatPathoArray.begin() ;
  // On prend l'élément
  string sCodeLex   = (*iJ)->getLexique() ;
  // Puis son code utile
  string sCodeUtile = string("") ;
  pContexte->getDico()->donneCodeSens(&sCodeLex, &sCodeUtile) ;

  //
  // Lancement du décodeur en fonction du code
  //
  pContexte->getSuperviseur()->afficheStatusMessage("Chargement du décodeur") ;

	if ((sCodeUtile == "GECHY") || (sCodeUtile == "GECHZ") ||
      (sCodeUtile == "GECHN") || (sCodeUtile == "GECHX") ||
      (sCodeUtile == "GECHR") || (sCodeUtile == "GECHL") ||
      (sCodeUtile == "GECHF"))
	{
    pDCModule = new TModule("NSMDKE.DLL", TRUE);
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSMDKE.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
	}
  else if ((string("GDUOB") == sCodeUtile) ||
           (string("GNEHA") == sCodeUtile) ||
           (string("GENTG") == sCodeUtile))
  {
    pDCModule = new TModule("NSMDGG.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSMDGG.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
  }
  else if ((string("GCOLL") == sCodeUtile) ||
           (string("GRECX") == sCodeUtile) ||
           (string("GENTF") == sCodeUtile))
  {
    pDCModule = new TModule("NSMDGC.DLL", TRUE);
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSMDGC.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
	}
  else if ((string("GPRCT") == sCodeUtile))
  {
    pDCModule = new TModule("NSMDGP.DLL", TRUE);
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSMDGP.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
	}
  else if (sCodeUtile == "GECHA")
  {
  	pDCModule = new TModule("NSMDEA.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSDEA.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
  }
  else if (sCodeUtile == "GEDOP")
  {
  	pDCModule = new TModule("NSMDKDV.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSDKDV.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
  }
  else if (sCodeUtile == "GEFFO")
	{
  	pDCModule = new TModule("NSMDKF.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSDKF.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
	}
  else if (sCodeUtile == "GHOLT")  {
  	pDCModule = new TModule("NSMDKH.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSDKH.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
  }
  else if (sCodeUtile == "ZORDO")
  {
  	pDCModule = new TModule("NSMDZZ.DLL", TRUE) ;
    if ((TModule*) NULL == pDCModule)
    {
    	erreur("La DLL NSDZZ.DLL est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
      pContexte->getSuperviseur()->afficheStatusMessage("") ;
      return iErreur ;
    }
  }
  else
  {
  	pContexte->getSuperviseur()->afficheStatusMessage("") ;
    return iErreur ;
  }

	(FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
	if (NULL == pAdresseFct)
  	Reussi = iErreur ;
	else
	{
  	NSDocumentData* pDocData ;
    if (_pDocInfo)
    {
      pDocData = new NSDocumentData ;
      _pDocInfo->initFromData(pDocData) ;
    }
    else
    	pDocData = (NSDocumentData*) 0 ;

    pContexte->getSuperviseur()->afficheStatusMessage("Elaboration du compte rendu") ;

    // La fonction de décodage renvoie 0 (iOk) si le décodage a réussi
    Reussi = ((*pAdresseFct)
                    (' ',
                    (NSPatPathoArray far *) &_PatPathoArray,
                    (char far *) _sFichDecod.c_str(),
                    (NSContexte far *) pContexte,
                    (NSCRPhraseArray far *) pPhrases,
                    (NSDocumentData far *) pDocData
                    )
                 ) ;

    if (pDocData)
      delete pDocData ;
	}

	delete pDCModule ;
	pDCModule = 0 ;

	// EstDecode est vrai si Reussi == 0
	_EstDecode = !Reussi ;

	pContexte->getSuperviseur()->afficheStatusMessage("") ;

	return Reussi ;
}
catch (TXInvalidModule e)
{
	string sErreur = string("NSCRDocument::decodeCR ") + e.why() ;
	erreur(sErreur.c_str(), standardError, 0) ;
  return false ;
}
catch (...)
{
	erreur("Exception NSCRDocument::decodeCR.", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------//  Enregistre le compte rendu
//---------------------------------------------------------------------------
bool
NSCRDocument::enregistre()
{
try
{
	bool existeInfo ;

	if (_bEnregEnCours)
		return false ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sLang = pContexte->getUserLanguage() ;

	_bEnregEnCours = true ;

	pSuper->afficheStatusMessage("Début d'enregistrement") ;

	//
	// Si c'est un nouveau compte rendu on le crée en tant que document
	//
	bool bNewDoc ;
	if (NULL == _pDocInfo)
		bNewDoc = true ;
	else
		bNewDoc = false ;

	if (bNewDoc)
	{
		string sLibelleDoc = string("") ;
		if (false == _PatPathoArray.empty())
		{
			string sCode = (*(_PatPathoArray.begin()))->getLexique() ;
			if (string("") != sCode)
				pContexte->getDico()->donneLibelle(sLang, &sCode, &sLibelleDoc) ;
		}
		existeInfo = Referencer("ZCN00", sLibelleDoc) ;
		if (false == existeInfo)
		{
			_bEnregEnCours = false ;
			return existeInfo ;
		}
	}
	//
	// On enregistre la patpatho
	//
	string ps = string("Enregistrement patpatho CR n° ") +
                                      _pDocInfo->getID() + string("\n") ;
	pSuper->trace(&ps, 1) ;

	existeInfo = enregistrePatPatho() ;

  // The new document must be inserted in "history" before checking if it resets
  // any goal, because this document's date will be asked to "history"

  //
	// On prévient l'historique
	//
	ps = "Rafraichissement historique\n" ;
	pSuper->trace(&ps, 1) ;

	pContexte->getPatient()->getDocHis()->Rafraichir(_pDocInfo, &_PatPathoArray, this) ;

	//
	// Si c'est un nouveau compte rendu on vérifie s'il remet à zéro un objectif
	//
	if ((bNewDoc) && (pContexte->getPatient()->getLdvDocument()))
		pContexte->getPatient()->getLdvDocument()->showNewTree(&_PatPathoArray, GetDateDoc(false)) ;

	ps = "Document enregistré.\n" ;
	pSuper->trace(&ps, 1) ;
	pSuper->afficheStatusMessage("Document enregistré") ;

  NSDocStatus docStatus ;
  if (bNewDoc)
    docStatus.setStatus(NSDocStatus::StatusNew) ;
  else
    docStatus.setStatus(NSDocStatus::StatusModified) ;

  exportReport(&docStatus) ;

	_bEnregEnCours = false ;

	return existeInfo ;
}
catch (...)
{
	erreur("Exception NSCRDocument::enregistre", standardError, 0) ;
  return false ;
}
}

//
// Automatic report export
//
bool
NSCRDocument::exportReport(NSDocStatus *pDocStatus)
{
try
{
  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string sExportDll = pSuper->getExportDll() ;
	if (string("") == sExportDll)
    return true ;

  pSuper->afficheStatusMessage("Export") ;

  size_t iPos = sExportDll.find("|") ;

  while (NPOS != iPos)
  {
    string sLocalName = string(sExportDll, 0, iPos) ;
    exportReportForDll(sLocalName, pDocStatus) ;

    sExportDll = string(sExportDll, iPos + 1, strlen(sExportDll.c_str()) - iPos - 1) ;

    iPos = sExportDll.find("|") ;
  }

  exportReportForDll(sExportDll, pDocStatus) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSCRDocument::exportReportForDll", standardError, 0) ;
  return false ;
}
}

bool
NSCRDocument::exportReportForDll(const string sDllName, NSDocStatus *pDocStatus)
{
try
{
  if (string("") == sDllName)
    return true ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  string ps = string("Export using dll \"") + sDllName + string("\"") ;
  pSuper->trace(&ps, 1) ;

  TModule* pDCModule = new TModule(sDllName.c_str(), TRUE) ;
  if ((TModule*) NULL == pDCModule)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") + string(" \"") + sDllName + string("\"") ;
    pSuper->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pSuper->afficheStatusMessage("") ;
    return false ;
  }

  //
  // Is it necessary to export the report as a file ?
  //
  string sExportedFileName = string("") ;

  bool (FAR *pAdrFctNeedFile) (NSContexte far *, NSCRDocument far *, string far *, string far *, NSDocStatus far *) ;
  (FARPROC) pAdrFctNeedFile = pDCModule->GetProcAddress(MAKEINTRESOURCE(3)) ;
  if (pAdrFctNeedFile)
  {
    ps = string("Asking export dll if document must be exported.") ;
    pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

    string sFileName = string("") ;
    string sFileType = string("") ;
    bool bNeedFile = ((*pAdrFctNeedFile)((NSContexte far *)   pContexte,
                                         (NSCRDocument far *) this,
                                         (string far *)       &sFileName,
                                         (string far *)       &sFileType,
                                         (NSDocStatus far *)  pDocStatus )) ;
    //
    // If needed, this is the place where we generate the file
    //
    if (bNeedFile)
    {
      ps = string("Export dll asks for document to be exported as file \"") + sFileName + string("\"") ;
      pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

      bool bExported = exportFile(sFileName, sFileType) ;
      if (bExported)
      {
        sExportedFileName = sFileName ;
        ps = string("Document successfuly exported as a file.") ;
        pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;
      }
      else
      {
        ps = string("Document failed to be exported as a file.") ;
        pSuper->trace(&ps, 1, NSSuper::trError) ;
        return false ;
      }
    }
  }
  else
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + sDllName + string(" -> needExportFile") ;
    pSuper->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    pSuper->afficheStatusMessage("") ;
  }

  // Once file is generated, the export module can do something with it
  //
  bool (FAR *pAdresseFct) (NSContexte far *, NSCRDocument far *, string far *, NSDocStatus far *) ;
  (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  if (NULL == pAdresseFct)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + sDllName + string(" -> exporter") ;
    pSuper->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;

    delete pDCModule ;
    return false ;
  }

  ps = string("Calling main export procedure.") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  /* bool bReussi = */ ((*pAdresseFct)((NSContexte far *)   pContexte,
                                       (NSCRDocument far *) this,
                                       (string far *)       &sExportedFileName,
                                       (NSDocStatus far *)  pDocStatus)) ;

  delete pDCModule ;

  ps = string("Leaving export using dll ") + sDllName ;
  pSuper->trace(&ps, 1) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSCRDocument::exportReportForDll", standardError, 0) ;
  return false ;
}
}

