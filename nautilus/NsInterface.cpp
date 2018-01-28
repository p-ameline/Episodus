// NSINTERFACE.CPP		Document/View Device interface// -------------------------------------------------
// PA - July 2010

#include <owl\owlpch.h>#include <owl\validate.h>
#include <owl\inputdia.h>
#include <owl\button.h>
#include <owl\window.rh>

#include <fstream.h>

#include "nautilus\nssuper.h"#include "partage\nsdivfct.h"
#include "nsbb\nsutidlg.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nspatpat.h"
#include "nsbb\tagNames.h"
#include "nssavoir\nsRootLink.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nshistdo.h"

#include "nsoutil\nsInterfaces.h"

#include "nautilus\nsInterface.h"
#include "nautilus\interface.rh"

#include "interface\ns_interface.h"
/******************************************************************************/
//
//					METHODES DE NSInterfaceView
//
/******************************************************************************/

DEFINE_RESPONSE_TABLE1(NSInterfaceView, TWindowView)	EV_WM_CLOSE,
  EV_COMMAND(CM_FILECLOSE,     EvClose),
  EV_COMMAND(IDC_DEMOGRAPHICS, CmCreatePatient),
  EV_COMMAND(IDC_STORE_ID,     CmStoreExternalId),
  EV_COMMAND(IDC_NEW_EXAM,     CmStartNewExam),
  EV_COMMAND(IDC_GET_RESULTS,  CmGetPatientResults),
  EV_COMMAND(IDC_PARAMS,       CmParams),
END_RESPONSE_TABLE ;

// Attention il faut passer doc comme NSTtxDocument avec un contexte valideNSInterfaceView::NSInterfaceView(NSInterfaceDocument& doc, TWindow *parent)
                :TWindowView(doc, parent), NSRoot(doc.pContexte), _pDoc(&doc)
{
  int X = 10 ;
  int Y = 10 ;
  int iBtnWidth  = 400 ;
  int iBtnHeigth = 40 ;
  int iInterY = 10 ;

  _iWindowsWidth = iBtnWidth + 2 * X ;

  // Open patient button
  //
  string sOpenText = pContexte->getSuperviseur()->getText("equipmentInterface", "openPatient") ;

  // Get patient name
  string sName = string("") ;

  NSPatientChoisi* pPatient = pContexte->getPatient() ;
  if (NULL != pPatient)
    sName = pPatient->getPrenom() + string(" ") + pPatient->getNom() ;

  size_t iPat = sOpenText.find("%P") ;
  if (NPOS != iPat)
    sOpenText.replace(iPat, 2, sName) ;
  else
    sOpenText += string(" ") + sName ;

  _pCreatePatientButton = new NSInterfaceButton(this, IDC_DEMOGRAPHICS, sOpenText.c_str(), X, Y, iBtnWidth, iBtnHeigth) ;

  Y += iBtnHeigth + iInterY ;

  // Store ID button
  //
  string sStoreIdText = getStoreExernalButtonText() ;
  _pStoreExternalButton = new NSInterfaceButton(this, IDC_STORE_ID, sStoreIdText.c_str(), X, Y, iBtnWidth, iBtnHeigth) ;

  Y += iBtnHeigth + iInterY ;

  // New exam buttons
  //
  string sNewRestText = pContexte->getSuperviseur()->getText("equipmentInterface", "startNewExam") ;
  _pStartNewExamButton = new NSInterfaceButton(this, IDC_NEW_EXAM, sNewRestText.c_str(), X, Y, iBtnWidth, iBtnHeigth) ;

  Y += iBtnHeigth + iInterY ;

  string sGetResultText = pContexte->getSuperviseur()->getText("equipmentInterface", "importResults") ;
  _pGetPatientResultsButton = new NSInterfaceButton(this, IDC_GET_RESULTS, sGetResultText.c_str(), X, Y, iBtnWidth, iBtnHeigth) ;

  _iWindowsHeight = Y + iBtnHeigth + iInterY ;

  Attr.W = _iWindowsWidth ;
  Attr.H = _iWindowsHeight ;

  _bBusy = false ;
}

NSInterfaceView::~NSInterfaceView(){
}

voidNSInterfaceView::PerformCreate(int menuOrId)
{
  TWindowView::PerformCreate(menuOrId) ;
  SetDocTitle(_pDoc->GetDocTitle().c_str(), 0) ;
}

voidNSInterfaceView::SetupWindow()
{
	TWindowView::SetupWindow() ;

  // Parent->SetCaption(_pDoc->GetDocTitle().c_str()) ;

  // La vue sera la fenetre client de la TMDIChild créée dans TMyApp::EvNewView  // Pour appliquer une taille à la vue, on doit donc agir sur la fenetre Parent

  ClassLib::TRect winRect    = Parent->GetWindowRect() ;
  ClassLib::TRect clientRect = Parent->GetClientRect() ;
  int iExtraWidth  = winRect.Width() - clientRect.Width() ;
  int iExtraHeight = winRect.Height() - clientRect.Height() ;

  Parent->SetWindowPos(0, 0, 0, _iWindowsWidth + iExtraWidth, _iWindowsHeight + iExtraHeight, SWP_NOZORDER | SWP_NOMOVE) ;  ModifyStyle(WS_BORDER, WS_CHILD) ;

  // ModifyExStyle(0, WS_EX_TOPMOST | WS_EX_TOOLWINDOW);  // MakeVisible() ;

  // CmLancer() ;}

voidNSInterfaceView::EvClose()
{
	// fonction appelée sur Fichier->Fermer
	CmQuitter() ;
}

boolNSInterfaceView::CanClose()
{
try
{
  if ((false == _bBusy) && (false == _pDoc->isBusy()))
	  return true ;  return false ;}catch (...){
  erreur("Exception NSInterfaceView::CanClose.", standardError, 0) ;
  return true ;
}
}
voidNSInterfaceView::CmQuitter()
{
  CloseWindow() ;
}

void
NSInterfaceView::CmCreatePatient()
{
  _bBusy = true ;

  string sActionResult = string("") ;
  bool bPatCreated = _pDoc->StartAction(this, string("CreatePatient"), sActionResult, (NSPatPathoArray *) 0) ;
  if (bPatCreated)
  {
    NSPatientChoisi* pPatient = pContexte->getPatient() ;
    if (pPatient)
    {
      string sPreviousIEP = _pDoc->getInterfaceIEP() ;

      _pDoc->setInterfaceIEP(pPatient->getNss()) ;
      _pDoc->storeInterfaceIEP(sPreviousIEP) ;
    }
  }

  _bBusy = false ;
}

void
NSInterfaceView::CmStoreExternalId()
{
  _bBusy = true ;

  string sActionResult = string("") ;
  bool bGotID = _pDoc->StartAction(this, string("GetExternalId"), sActionResult, (NSPatPathoArray *) 0) ;

  if ((false == bGotID) || (string("") == sActionResult))
  {
    _bBusy = false ;
    return ;
  }

  string sPreviousIEP = _pDoc->getInterfaceIEP() ;

  _pDoc->setInterfaceIEP(sActionResult) ;
  _pDoc->storeInterfaceIEP(sPreviousIEP) ;

  string sStoreIdText = getStoreExernalButtonText() ;
  _pStoreExternalButton->SetCaption(sStoreIdText.c_str()) ;

  Invalidate() ;

  _bBusy = false ;
}

void
NSInterfaceView::CmStartNewExam()
{
  _bBusy = true ;

  string sActionResult = _pDoc->getInterfaceIEP() ;
  _pDoc->StartAction(this, string("StartNewExam"), sActionResult, (NSPatPathoArray *) 0) ;

  _bBusy = false ;
}

void
NSInterfaceView::CmGetPatientResults()
{
  _bBusy = true ;

  NSVectPatPathoArray vectPatPatho ;
  string sActionResult = _pDoc->getInterfaceIEP() ;
  if (false == _pDoc->StartAction(this, string("GetReport"), sActionResult, &vectPatPatho))
  {
    _bBusy = false ;
    CloseWindow() ;
    return ;
  }

  _pDoc->importDocuments(&vectPatPatho, sActionResult) ;

  _bBusy = false ;

  CloseWindow() ;
}

void
NSInterfaceView::CmParams()
{
}

string
NSInterfaceView::getStoreExernalButtonText()
{
  if (string("") == _pDoc->getInterfaceIEP())
    return pContexte->getSuperviseur()->getText("equipmentInterface", "storeExternalIdentifier") ;

  string sText = pContexte->getSuperviseur()->getText("equipmentInterface", "changeExternalIdentifier") ;
  sText += string(" (") + _pDoc->getInterfaceIEP() + string(")") ;

  return sText ;
}

/******************************************************************************/
//					METHODES DE NSTtx Document / Vue
/******************************************************************************/

NSInterfaceDocument::NSInterfaceDocument(TDocument *parent, NSContexte *pCtx)                    :TDocument(parent), NSRoot(pCtx)
{
  _sModuleName   = string("") ;
  _sDocTitle     = string("") ;
  _sPrefix       = string("") ;
  _sInterfaceIEP = string("") ;

  _pInterfaceModule = 0 ;

  _TestOk = false ;

	Open(0, "") ;
}

// Ouverture du document////////////////////////////////////////////////////////////////

boolNSInterfaceDocument::Open(int /*mode*/, LPCSTR path){
  map<string, string> aInterfaces ;
  map<string, string> aPseudoProtocols ;
  initInterfacesList(&aInterfaces, &aPseudoProtocols) ;

  if (aInterfaces.empty())
    return false ;

  if (aInterfaces.size() > 1)
  {
    string sModuleName = string("") ;

    NsInterfacesDialog interfaceDlg(pContexte->GetMainWindow(),
                                  pContexte,
                                  &sModuleName,
                                  &aInterfaces) ;

    if (IDOK != interfaceDlg.Execute())
      return false ;

    _sModuleName = sModuleName ;
  }
  else
  {
    map<string, string>::iterator iter = aInterfaces.begin() ;
    _sModuleName = (*iter).second ;
    _sDocTitle   = (*iter).first ;
  }

  return OpenModule() ;
}

bool
NSInterfaceDocument::OpenModule()
{
  _pInterfaceModule = new TModule(_sModuleName.c_str(), TRUE) ;

  if (NULL == _pInterfaceModule)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") ;
    sErrorText += string(" ") +  _sModuleName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  SetDocTitle() ;

  string sActionResult = string("") ;
  _TestOk = StartAction(pContexte->GetMainWindow(), string("Test"), sActionResult, (NSPatPathoArray *) 0) ;

  if (_TestOk)
    _TestOk = StartAction(pContexte->GetMainWindow(), string("GetPrefix"), _sPrefix, (NSPatPathoArray *) 0) ;

  if (_TestOk)
    queryInterfaceIEP() ;

  return _TestOk ;
}

// Fermeture du document////////////////////////////////////////////////////////////////
bool
NSInterfaceDocument::Close()
{
  delete _pInterfaceModule ;
	_pInterfaceModule = 0 ;
	return true ;
}

// Indique si document ouvert (il existe un fichier associé)////////////////////////////////////////////////////////////////
bool
NSInterfaceDocument::IsOpen()
{
  return (_pInterfaceModule && _TestOk) ;
}

void
NSInterfaceDocument::initInterfacesList(map<string, string> *paInterfaces, map<string, string> *paPseudoProtocols)
{
  if (NULL == paInterfaces)
    return ;

  // Looking for nsmt_*.dll files in current directory
  //  char szMask[1024] ;  strcpy(szMask, "nsmi_*.dll") ;  WIN32_FIND_DATA FileData ;
  HANDLE hSearch = FindFirstFile(szMask, &FileData) ;
  if (hSearch == INVALID_HANDLE_VALUE)    return ;  // pSuperContext->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  void (FAR *pAdresseFct) (string far *, string far *, string far *) ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  bool bFinish = false ;
  while (false == bFinish)
  {    DWORD dwAttr = FileData.dwFileAttributes;
    if (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))
    {      string sFileName = string(FileData.cFileName) ;      string sToolLabel  ;      string sToolDescription  ;      string sToolPseudoProtocol  ;try{      TModule* pDCModule = new TModule(sFileName.c_str(), TRUE) ;
    	if (NULL == pDCModule)
    	{
        string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") ;
      	sErrorText += string(" ") +  sFileName ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      	erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
    	}
     	else
      {
    		// Récupération du pointeur sur la fonction // Getting function's pointer
    		// (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
        (FARPROC) pAdresseFct = pDCModule->GetProcAddress("@nsmtoolsInformation$qp60std@%basic_string$c19std@%char_traits$c%17std@%allocator$c%%t1t1") ;
    		if (pAdresseFct)
        {
    			((*pAdresseFct)((string far *) &sToolLabel, (string far *) &sToolDescription, (string far *) &sToolPseudoProtocol)) ;

          if (string("") != sToolLabel)
          {
          	(*paInterfaces)[sToolLabel] = sFileName ;

            if (paPseudoProtocols)
              (*paPseudoProtocols)[sToolPseudoProtocol] = sToolLabel ;
          }
        }
        delete pDCModule ;
      }
}
catch (...)
{
  string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotLoadDll") ;
  sErrorText += string(" (") + sFileName + string(")") ;
  pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
  erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
}
    }

    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

    if (!FindNextFile(hSearch, &FileData))
    {      if (GetLastError() != ERROR_NO_MORE_FILES)      {        string sStatusMsg = pContexte->getSuperviseur()->getText("fileErrors", "cantGetNextFile") ;        pContexte->getSuperviseur()->trace(&sStatusMsg, 1) ;        erreur(sStatusMsg.c_str(), standardError, 0) ;        return ;      }      bFinish = true ;    }
  }
  pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

bool
NSInterfaceDocument::StartAction(TWindow *pere, string sActionName, string &sActionResult, NSVectPatPathoArray *pVectPptResult)
{
  void (FAR *pAdresseFct) (TWindow far *, NSContexte far *, bool far *, string far *, string far *, NSVectPatPathoArray far *) ;

  // Récupération du pointeur sur la fonction // Getting function's pointer
  // (FARPROC) pAdresseFct = pDCModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
  (FARPROC) pAdresseFct = _pInterfaceModule->GetProcAddress("@nsmToolsAction$qp11OWL@TWindowp10NSContextep4boolp60std@%basic_string$c19std@%char_traits$c%17std@%allocator$c%%t4p19NSVectPatPathoArray") ;
  if (NULL == pAdresseFct)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("modulesManagement", "cannotAccessMethodInDll") + string(" ") + _sModuleName + string(" -> nsmToolsAction");
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;

    sActionResult = sErrorText ;
    return false ;
  }

  bool bIsSuccess ;

  _bBusy = true ;
  ((*pAdresseFct)((TWindow far *) pere, (NSContexte far *) pContexte, (bool far *) &bIsSuccess, (string far *) &sActionName, (string far *) &sActionResult, (NSVectPatPathoArray far *) pVectPptResult)) ;
  _bBusy = false ;

  return bIsSuccess ;
}

void
NSInterfaceDocument::openRemoteDocument(string sURI)
{
  if (string("") == sURI)
    return ;

  // Find pseudo-protocol and document Id
  //
  size_t iPos = sURI.find(":") ;
  if (NPOS == iPos)
    return ;

  string sPseudoProtocol = string(sURI, 0, iPos) ;
  string sDocumentId     = string(sURI, iPos+1, strlen(sURI.c_str())-iPos-1) ;

  if ((string("") == sDocumentId) || (string("") == sPseudoProtocol))
    return ;

  map<string, string> aInterfaces ;
  map<string, string> aPseudoProtocols ;
  initInterfacesList(&aInterfaces, &aPseudoProtocols) ;

  string sProtocolModule = aPseudoProtocols[sPseudoProtocol] ;
  if (string("") == sProtocolModule)
    return ;

  _sModuleName = aInterfaces[sProtocolModule] ;
  if (string("") == _sModuleName)
    return ;

  if (false == OpenModule())
    return ;

  string sActionResult = _sInterfaceIEP + string(1, filesSeparator) + sDocumentId ;
  StartAction(pContexte->GetMainWindow(), string("OpenDocument"), sActionResult, (NSPatPathoArray *) 0) ;
}

void
NSInterfaceDocument::SetDocTitle()
{
  void (FAR *pAdresseFct) (string far *, string far *) ;

  (FARPROC) pAdresseFct = _pInterfaceModule->GetProcAddress("@nsmtoolsInformation$qp60std@%basic_string$c19std@%char_traits$c%17std@%allocator$c%%t1") ;
  if (NULL == pAdresseFct)
    return ;

  string sToolDescription  ;
  ((*pAdresseFct)((string far *) &_sDocTitle, (string far *) &sToolDescription)) ;
}

void
NSInterfaceDocument::importDocuments(NSVectPatPathoArray *pVectPatPatho, string sFilesName)
{
  ClasseStringVector aFileDescriptors ;
  if (string("") != sFilesName)
    DecomposeChaine(&sFilesName, &aFileDescriptors, docsSeparator) ;

  if ((aFileDescriptors.empty()) && (pVectPatPatho->estVide()))
    return ;

  string           sFileDesc = string("") ;
  NSPatPathoArray* pPatPatho = 0 ;

  iterClassString itStr = 0 ;
  if (false == aFileDescriptors.empty())
  {
    itStr = aFileDescriptors.begin() ;
    sFileDesc = (*itStr)->getItem() ;
  }

  PatPathoIterVect itPpt = 0 ;
  if (false == pVectPatPatho->empty())
  {
    itPpt = pVectPatPatho->begin() ;
    pPatPatho = *itPpt ;
  }

  bool bKeepOn = true ;
  while (bKeepOn)
  {
    // Here, we have a tree and/or a document file name to import
    //
    if (string("") == sFileDesc)
      recordTree(pPatPatho) ;
    else
      importBothDocuments(pPatPatho, sFileDesc) ;

    if (false == aFileDescriptors.empty())
    {
      itStr++ ;
      if (aFileDescriptors.end() != itStr)
        sFileDesc = (*itStr)->getItem() ;
      else
        sFileDesc = string("") ;
    }
    if (false == pVectPatPatho->empty())
    {
      itPpt++ ;
      if (pVectPatPatho->end() != itPpt)
        pPatPatho = *itPpt ;
      else
        pPatPatho = 0 ;
    }

    if ((string("") == sFileDesc) && (NULL == pPatPatho))
      bKeepOn = false ;
  }
}

void
NSInterfaceDocument::queryInterfaceIEP()
{
  if (string("") == _sPrefix)
    return ;

  NSPatientChoisi* pPatient = pContexte->getPatient() ;
  if (NULL == pPatient)
    return ;

  NSPersonsAttributesArray AttsList ;

  NSBasicAttributeArray AttrArray ;
  AttrArray.push_back(new NSBasicAttribute(PERSON, pPatient->getNss())) ;
  // AttrArray.push_back(new NSBasicAttribute(TRAIT, IEP)) ;

  const char* serviceName = (NautilusPilot::SERV_PERSON_TRAITS_LIST).c_str() ;

	bool listOk = pContexte->getSuperviseur()->getPilot()->getUserProperties(serviceName, &AttsList, &AttrArray) ;
  if ((false == listOk) || AttsList.empty())
    return ;

  NSPersonsAttributeIter iterPerson = AttsList.begin() ;

  NSBasicAttributeArray* pAttribute = *iterPerson ;
  if ((NULL == pAttribute) || pAttribute->empty())
    return ;

  size_t iPrefixLen = strlen(_sPrefix.c_str()) ;

  NSBasicAttributeIter iter = pAttribute->begin() ;
	for(iter ; iter != pAttribute->end() ; iter++)
	{
    if ((*iter)->getBalise() == IEP)
    {
    	string sIep = (*iter)->getText() ;         // SEMA9786
      size_t iIepLen = strlen(sIep.c_str()) ;    // 01234567
      if (iIepLen > iPrefixLen)
      {
        size_t iPos = sIep.find(_sPrefix) ;
        if (0 == iPos)
        {
          _sInterfaceIEP = string(sIep, iPrefixLen, iIepLen - iPrefixLen) ;
          break ;
        }
      }
    }
  }
}

void
NSInterfaceDocument::storeInterfaceIEP(string sPreviousValue)
{
  NSPersonsAttributesArray PatiensList ;
  NSBasicAttributeArray    AttrList ;

  string sNewTrait = string("") ;
  if (string("") != sPreviousValue)
    sNewTrait = _sPrefix + sPreviousValue + string("$->$") ;
  sNewTrait += _sPrefix + _sInterfaceIEP ;

  AttrList.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
  AttrList.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;
  AttrList.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
  AttrList.push_back(new NSBasicAttribute(PERSON,   pContexte->getPatient()->getNss())) ;
	AttrList.push_back(new NSBasicAttribute(IEP,      sNewTrait)) ;

  pContexte->getSuperviseur()->getPilot()->modifyTraitsForPersonOrObject(NautilusPilot::SERV_MODIFY_PERSON_TRAITS.c_str(), &PatiensList, &AttrList) ;
}

void
NSInterfaceDocument::recordTree(NSPatPathoArray *pPatPatho)
{
  if (NULL == pPatPatho)
    return ;
}

void
NSInterfaceDocument::importBothDocuments(NSPatPathoArray *pPatPatho, string sFilesName)
{
  if ((NULL == pPatPatho) || pPatPatho->empty() || (string("") == sFilesName))
    return ;

  // First, get document's title from patpatho's root
  //
  string sLang = string("") ;
  if (pContexte) 
  	sLang = pContexte->getUserLanguage() ;

  PatPathoIter it = pPatPatho->begin() ;
  string sDocLexique = (*it)->getLexique() ;

  string sDocTitle = string("") ;
  pContexte->getDico()->donneLibelle(sLang, &sDocLexique, &sDocTitle) ;

  // Then, see what document is available : PDF, structured data, external data
  //
  string sPdfFileName        = string("") ;
  string sStructuredFileName = string("") ;
  string sExternalLink       = string("") ;
  string sExternalLinkType   = string("") ;

  ClasseStringVector filesVector ;
  DecomposeChaine(&sFilesName, &filesVector, string(1, filesSeparator)) ;
  for (iterClassString itStr = filesVector.begin() ; filesVector.end() != itStr ; itStr++)
  {
    string sFileDesc = (*itStr)->getItem() ;
    size_t iPosOpener = sFileDesc.find(blockOpener) ;
    if (NPOS != iPosOpener)
    {
      string sFileType  = string(sFileDesc, 0, iPosOpener) ;
      string sRightPart = string(sFileDesc, iPosOpener+1, strlen(sFileDesc.c_str())-iPosOpener-1) ;

      size_t iPosCloser = sRightPart.find(blockCloser) ;
      if (NPOS != iPosCloser)
      {
        string sFileName = string(sRightPart, 0, iPosCloser) ;
        if      (string("ZSTRU1") == sFileType)
          sStructuredFileName = sFileName ;
        else if (string("ZTPDF1") == sFileType)
          sPdfFileName = sFileName ;
        else if (string("ZDINT1") == sFileType)
        {
          sExternalLink     = sFileName ;
          sExternalLinkType = sFileType ;
        }
        else if (string("ZURL01") == sFileType)
        {
          sExternalLink     = sFileName ;
          sExternalLinkType = sFileType ;
        }
      }
    }
  }

  // Master document is :
  // 1) the pdf file if it exists
  // 2) the external link elsewhere
  //

  // Import file
  //
  NSRefDocument NewDocImport(0, pContexte) ;
  NewDocImport.setReadOnly(false) ;

  NSRefDocument *pMasterDoc = 0 ;

  if (string("") != sPdfFileName)
  {
    // Import the PDF file as master document
    //
    if (false == importFileDocument(&NewDocImport, sDocTitle, sPdfFileName, 0, NSRootLink::badLink))
      return ;

    pMasterDoc = &NewDocImport ;
    sDocTitle = pMasterDoc->_pDocInfo->getDocName() ;
  }

  // Referencing the external link
  //
  if (string("") != sExternalLink)
  {
    NSRefDocument *pCurrentDoc = 0 ;

    NSRefDocument NewExternalDocImport(0, pContexte) ;
    NewExternalDocImport.setReadOnly(false) ;

    if (NULL == pMasterDoc)
      pCurrentDoc = &NewDocImport ;
    else
      pCurrentDoc = &NewExternalDocImport ;

    if (false == importExternalLink(pCurrentDoc, sExternalLinkType, sDocTitle, sExternalLink, pMasterDoc))
      return ;

    if (NULL == pMasterDoc)
    {
      pMasterDoc = &NewDocImport ;
      sDocTitle = pMasterDoc->_pDocInfo->getDocName() ;
    }
  }

  // Referencing the structured document
  //
  if (string("") != sStructuredFileName)
  {
    NSRefDocument *pCurrentDoc = 0 ;

    NSRefDocument NewExternalDocImport(0, pContexte) ;
    NewExternalDocImport.setReadOnly(false) ;

    if (NULL == pMasterDoc)
      pCurrentDoc = &NewDocImport ;
    else
      pCurrentDoc = &NewExternalDocImport ;

    if (false == importFileDocument(pCurrentDoc, sDocTitle, sStructuredFileName, pMasterDoc, NSRootLink::structuredVersionOfDocument))
      return ;

    if (NULL == pMasterDoc)
    {
      pMasterDoc = &NewDocImport ;
      sDocTitle = pMasterDoc->_pDocInfo->getDocName() ;
    }
  }

  //
  // Import tree document
  //
  NewDocImport.ReferencerDocumentPatPatho(pMasterDoc, pPatPatho, sDocTitle, string("ZCS00")) ;

  // Updating history list
  //
  pContexte->getPatient()->getDocHis()->Rafraichir(pMasterDoc->_pDocInfo, 0) ;
}

bool
NSInterfaceDocument::importFileDocument(NSRefDocument *pRefDoc, string sDocTitle, string sFileName, NSRefDocument *pMasterDoc, NSRootLink::NODELINKTYPES masterLink)
{
  if ((NULL == pRefDoc) || (string("") == sFileName))
    return false ;

  //
  // Import file document
  //
  string sPathName  = string("") ;
  string sExtension = string("") ;

  // Get path
  //
  size_t pos = sFileName.find_last_of("\\") ;
  if (NPOS != pos)
  {
    sPathName = string(sFileName, 0, pos + 1) ;
    sFileName = string(sFileName, pos + 1, strlen(sFileName.c_str()) - pos - 1) ;
  }

  // Get Extension
  //
  pos = sFileName.find_last_of(".") ;
  if (NPOS != pos)
    sExtension = string(sFileName, pos + 1, strlen(sFileName.c_str()) -pos - 1) ;

  // Get Mime type
  //
  NSTypeMimeInfo infoTypeMime ;
  if (false == pContexte->getSuperviseur()->chercheTypeMimeInfo(sExtension, &infoTypeMime))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "unreferencedExtension") ;
    sErrorText += string(" (") + sExtension + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }

  string sTypeNautilus = infoTypeMime.getType() ;

  bool bVerbose = true ;
  bool bVisible = true ;
  if (pMasterDoc)
  {
    bVerbose = false ;
    bVisible = false ;
  }

  // Moving file and Referencing it as a document
  //
  if (false == pRefDoc->ImporterFichier(sTypeNautilus, sDocTitle, sFileName, sPathName, bVerbose, bVisible))
  {
    string sErrorText = pContexte->getSuperviseur()->getText("importManagement", "importFailed") ;
    sErrorText += string(" (") + sPathName + sFileName + string(")") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  if (pMasterDoc)
    pRefDoc->connectToMasterDocument(pMasterDoc->getCodeDocMeta(), masterLink) ;

  return true ;
}

bool
NSInterfaceDocument::importExternalLink(NSRefDocument *pRefDoc, string sDocType, string sDocTitle, string sFileName, NSRefDocument *pMasterDoc)
{
  if ((NULL == pRefDoc) || (string("") == sFileName))
    return false ;

  bool   bVerbose     = true ;
  bool   bVisible     = true ;
  string sMasterDocId = string("") ;
  if (pMasterDoc)
  {
    bVerbose     = false ;
    bVisible     = false ;
    sMasterDocId = pMasterDoc->getCodeDocMeta() ;
  }

  if (false == pRefDoc->Referencer(sDocType, sDocTitle, sFileName, string(""),
                                   bVisible, bVerbose, string(""),
                                   NSRootLink::personDocument, 0,
                                   string("_User_"), string(""), string(""),
                                   string(""), string(""), sMasterDocId,
                                   NSRootLink::externViewOfDocument))
    return false ;

  return true ;
}

//
// Constructeur
//
NSInterfaceButton::NSInterfaceButton(TWindow* parent, int resId)
                  :TButton(parent, resId)
{
	DisableTransfer() ;
}

NSInterfaceButton::NSInterfaceButton(TWindow* parent, int resId, const char far* text,
                 int X, int Y, int W, int H, bool isDefault, TModule* module)
                  :TButton(parent, resId, text, X, Y, W, H, isDefault, module)
{
	DisableTransfer() ;
}

//
// Destructeur
//
NSInterfaceButton::~NSInterfaceButton()
{
}

void
NSInterfaceButton::SetupWindow()
{
  OWL::TButton::SetupWindow() ;
}

char far*
NSInterfaceButton::GetClassName()
{
  return "BUTTON" ;
}

