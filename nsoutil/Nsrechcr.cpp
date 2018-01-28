#include <string.h>#include <cstring.h>
#include <stdio.h>
#include <owl\gauge.h>

#include "nautilus\nssuper.h"
#include "nsoutil\nsrechcr.h"
#include "nsoutil\nsoutil.h"
#include "nsoutil\nsoutil.rh"
#include "nsoutil\nsfilgui.h"
#include "nssavoir\nsguide.h"
#include "nssavoir\nsfilgd.h"
#include "nssavoir\nsgraphe.h"
#include "dcodeur\decoder.h"
#include "dcodeur\nsdkd.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsednum.h"
#include "nsbb\tagNames.h"
#include "nsbb\nslistWind.h"
#include "nsepisod\nsldvuti.h"

#define searchPathSeparator '|'
#define searchNodeSeparator '/'

long NSRequestResult::lObjectCount = 0 ;

/*****************************************************************************//******                 OUTIL DE GESTION DES REQUETES                   ******/
/*****************************************************************************/

NSRequestResult::NSRequestResult(const NSRequestResult& src)
{
	copyValues(&src) ;
}

void
NSRequestResult::reinitForNewSearch()
{
	_aVectDocumentResults.vider() ;
	_aVectPatientResults.vider() ;

	_nbPatResult = 0 ;
	_nbDocResult = 0 ;

	_bNouveauPatResult = true ;
}

void
NSRequestResult::copyValues(const NSRequestResult* pSrc)
{
  if ((NSRequestResult*) NULL == pSrc)
    return ;

	if (false == pSrc->_aVectDocumentResults.empty())
		for (DocumentConstIter i = pSrc->_aVectDocumentResults.begin() ; pSrc->_aVectDocumentResults.end() != i ; i++)
			_aVectDocumentResults.push_back(new NSDocumentHisto(*(*i))) ;

	if (false == pSrc->_aVectPatientResults.empty())
  	for (PatientConstIter i = pSrc->_aVectPatientResults.begin() ; pSrc->_aVectPatientResults.end() != i ; i++)
    	_aVectPatientResults.push_back(new NSPatInfo(*(*i))) ;

	_nbPatResult = pSrc->_nbPatResult ;
	_nbDocResult = pSrc->_nbDocResult ;

	_bNouveauPatResult = pSrc->_bNouveauPatResult ;

  _sRequestName         = pSrc->_sRequestName ;
	_sRequestBooleanLabel = pSrc->_sRequestBooleanLabel ;
}

NSRequestResult&
NSRequestResult::operator=(const NSRequestResult& src)
{
	if (this == &src)
		return *this ;

	reinitForNewSearch() ;
	copyValues(&src) ;

	return *this ;
}

// -----------------------------------------------------------------////  Méthodes de NSRequeteWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRequeteWindow, TListWindow)    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSRequeteWindow::NSRequeteWindow(NSRequeteDialog* pere, int resId) : TListWindow(pere, resId){
	pDlg = pere ;
}

NSRequeteWindow::~NSRequeteWindow(){}voidNSRequeteWindow::SetupWindow(){	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;}//---------------------------------------------------------------------------//  Description: Fonction de réponse au double click
//---------------------------------------------------------------------------
void
NSRequeteWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modKeys, point) ;

	TLwHitTestInfo info(point) ;
	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmInsererChemin() ;
}

//---------------------------------------------------------------------------//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSRequeteWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)
		if (GetItemState(i, LVIS_SELECTED))
		{
    	index = i ;
			break ;
		}

	return index ;
}

// -----------------------------------------------------------------
////  Méthodes de NSBooleanRequestsWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSBooleanRequestsWindow, TListWindow)    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSBooleanRequestsWindow::NSBooleanRequestsWindow(NSRequeteDialog* pere, int resId)                        :TListWindow(pere, resId){
	pDlg = pere ;
}

NSBooleanRequestsWindow::~NSBooleanRequestsWindow(){}voidNSBooleanRequestsWindow::SetupWindow(){	ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
	TListWindow::SetupWindow() ;}//---------------------------------------------------------------------------//  Description: Fonction de réponse au double click
//---------------------------------------------------------------------------
void
NSBooleanRequestsWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modKeys, point) ;

	TLwHitTestInfo info(point) ;
	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmPutRequestInEditControl() ;
}

//---------------------------------------------------------------------------//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSBooleanRequestsWindow::IndexItemSelect()
{
	int count = GetItemCount() ;
	int index = -1 ;

	for (int i = 0; i < count; i++)
		if (GetItemState(i, LVIS_SELECTED))
		{
    	index = i ;
			break ;
		}

	return index ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSRequeteDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRequeteDialog, NSUtilDialog)
  EV_BN_CLICKED(IDC_REQ_NEWREQ,   CmNouveau),
  EV_BN_CLICKED(IDC_REQ_OPEN,     CmOuvrir),
  EV_BN_CLICKED(IDC_REQ_SAVE,     CmEnregistrer),
  EV_BN_CLICKED(IDC_REQ_NEW,      CmNewReq),
  EV_BN_CLICKED(IDC_REQ_MOD,      CmModReq),
  EV_BN_CLICKED(IDC_REQ_DEL,      CmDelReq),
  EV_BN_CLICKED(IDC_REQ_DUP,      CmDupReq),
  EV_BN_CLICKED(IDC_REQ_CRITPAT,  CmCriteresPatient),
  EV_BN_CLICKED(IDC_REQ_CRITDOC,  CmCriteresDocument),
  EV_BN_CLICKED(IDC_REQ_OUVRANTE, CmOuvrante),
  EV_BN_CLICKED(IDC_REQ_ET,       CmET),
  EV_BN_CLICKED(IDC_REQ_OU,       CmOU),
  EV_BN_CLICKED(IDC_REQ_FERMANTE, CmFermante),
  EV_BN_CLICKED(IDC_REQ_MODPAT,   CmModePat),
  EV_BN_CLICKED(IDC_REQ_MODDOC,   CmModeDoc),
  EV_BN_CLICKED(IDC_REQ_LANCER,   CmLancement),
  EV_BN_CLICKED(IDC_REQ_FERMER,   CmFermer),
  EV_BN_CLICKED(IDC_REQ_REQ_NEW,  CmAddRequest),
  EV_BN_CLICKED(IDC_REQ_REQ_MOD,  CmEditRequest),
  EV_BN_CLICKED(IDC_REQ_REQ_DEL,  CmDeleteRequest),
  EV_BN_CLICKED(IDC_REQ_REQ_DUP,  CmDuplicateRequest),
  EV_LVN_GETDISPINFO(IDC_REQ_LW,      LvnGetDispInfo),
  EV_LVN_GETDISPINFO(IDC_REQ_BOOL_LW, LvnGetSearchDispInfo),
END_RESPONSE_TABLE;

NSRequeteDialog::NSRequeteDialog(TWindow* pere, NSContexte* pCtx)                :_Document(pCtx), _Patient(pCtx), NSUtilDialog(pere, pCtx, "IDD_REQUETE", pNSResModule)
{
	_bDirty = false ;

  _pNomReq       = new NSUtilEdit2(pContexte, this, IDC_REQ_NOM, 80) ;
  _pListeChem    = new NSRequeteWindow(this, IDC_REQ_LW) ;
  _pRequestsList = new NSBooleanRequestsWindow(this, IDC_REQ_BOOL_LW) ;
  _pChemArray    = new NSEltArray ;
  _nbChem = 0;
  _pEditReq      = new NSUtilEdit2(pContexte, this, IDC_REQ_EDIT, 255) ;
  _pEditReqLabel = new NSUtilEdit2(pContexte, this, IDC_REQ_NAME_EDIT, 255) ;
  //pMode =         new TGroupBox(this, IDC_REQ_MOD) ;
  _pModePat      = new TRadioButton(this, IDC_REQ_MODPAT /*, pMode */) ;
  _pModeDoc      = new TRadioButton(this, IDC_REQ_MODDOC /*, pMode */) ;
  _pCritPatBox   = new TCheckBox(this, IDC_REQ_CRITPAT) ;
  _pCritDocBox   = new TCheckBox(this, IDC_REQ_CRITDOC) ;
  _pCritPat      = new NSCritReqPatient() ;
  _pCritDoc      = new NSCritReqDocum() ;
  _pLanceur      = new NSLanceReqDialog(this, pCtx, pNSResModule) ;
  _pPersonList   = (NSPersonsAttributesArray*) 0 ;
  _sLibReq       = string("") ;
  _bReqModeDoc   = true ;
  _bReqEnCours   = false ;
  _bCritPat      = false ;
  _bCritDoc      = false ;
  _bCherchePatient  = false ;
  _bChercheDocument = false ;
  _sFileName     = string("") ;
}

NSRequeteDialog::~NSRequeteDialog()
{
  delete _pNomReq ;
  delete _pListeChem ;
  delete _pRequestsList ;
  delete _pChemArray ;
  delete _pEditReq ;
  delete _pEditReqLabel ;
  // delete pMode ;
  delete _pModePat ;
  delete _pModeDoc ;
  delete _pCritPatBox ;
  delete _pCritDocBox ;
  delete _pCritPat ;
  delete _pCritDoc ;
  delete _pLanceur ;

  if (_pPersonList)  	delete _pPersonList ;}

voidNSRequeteDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	InitListeChem() ;
	AfficheListeChem() ;     // pour le cas où on réaffiche des chemins existants

  InitSearchList() ;
	DisplaySearchList() ;

	// La recherche est en mode document par défaut
	if (_bReqModeDoc)
		_pModeDoc->Check() ;
	else
		_pModePat->Check() ;

	// on check les criteres	if (_bCritPat)
		_pCritPatBox->Check() ;
	if (_bCritDoc)
		_pCritDocBox->Check() ;

	// on replace le libellé
	if (string("") != _sLibReq)
		_pEditReq->SetText(_sLibReq.c_str()) ;
	if (string("") != _sFileName)
		_pNomReq->SetText(_sFileName.c_str()) ;

	// Setting various titles
	//
  setTextForControl(IDC_REQ_STEP1_TXT, "searchingRequestForm", "stepOneDefinePatientsCriteria") ;
  setTextForControl(IDC_REQ_STEP2_TXT, "searchingRequestForm", "stepTwoDefineDocumentsCriteria") ;
  setTextForControl(IDC_REQ_STEP3_TXT, "searchingRequestForm", "stepThreeDefineClinicalCriteria") ;
  setTextForControl(IDC_REQ_STEP4_TXT, "searchingRequestForm", "stepFourDefineClinicalRequests") ;
  setTextForControl(IDC_REQ_STEP5_TXT, "searchingRequestForm", "stepFiveDefineHowToValidate") ;
  setTextForControl(IDC_REQ_STEP6_TXT, "searchingRequestForm", "stepSixRun") ;


/************************
    if (InitChemArray())
        AfficheListeChem();
    else
        erreur("Impossible d'afficher la liste des chemins.",0,0);
************************/
}

voidNSRequeteDialog::InitListeChem()
{
	string sLocalText = pContexte->getSuperviseur()->getText("searchingRequestForm", "pathNum") ;
  TListWindColumn colNum((char*) sLocalText.c_str(), 20, TListWindColumn::Left, 0) ;
  _pListeChem->InsertColumn(0, colNum) ;

  sLocalText = pContexte->getSuperviseur()->getText("searchingRequestForm", "path") ;
  TListWindColumn colChem((char*) sLocalText.c_str(), 1000, TListWindColumn::Left, 1) ;
  _pListeChem->InsertColumn(1, colChem) ;
}

NSPatInfo*
NSRequeteDialog::getResultPatient(size_t iIndex)
{
  if (iIndex > _VectPatResultat.size())
    return (NSPatInfo*) 0 ;

  return _VectPatResultat[iIndex] ;
}

voidNSRequeteDialog::AfficheListeChem()
{
	char elt[10] ;

	_pListeChem->DeleteAllItems() ;

	for (int i = _nbChem - 1; i >= 0; i--)
	{
		sprintf(elt, "%d", i+1) ;
    TListWindItem Item(elt, 0) ;
    _pListeChem->InsertItem(Item) ;
	}
}

voidNSRequeteDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  if ((NSEltArray*) NULL == _pChemArray)
    return ;

	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;

  NSElement* pElement = (*_pChemArray)[index] ;
  if ((NSElement*) NULL == pElement)
    return ;

  const int BufLen = 1024 ;
	static char buffer[BufLen] ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1: 	// chemin
    	sprintf(buffer, "%s", (pElement->getLabel()).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
	}
}

void
NSRequeteDialog::InitSearchList()
{
	string sLocalText = pContexte->getSuperviseur()->getText("searchingRequestForm", "searchName") ;
  TListWindColumn colName((char*) sLocalText.c_str(), 150, TListWindColumn::Left, 0) ;
  _pRequestsList->InsertColumn(0, colName) ;

  sLocalText = pContexte->getSuperviseur()->getText("searchingRequestForm", "searchLabel") ;
  TListWindColumn colText((char*) sLocalText.c_str(), 1000, TListWindColumn::Left, 1) ;
  _pRequestsList->InsertColumn(1, colText) ;
}

void
NSRequeteDialog::DisplaySearchList()
{
	_pRequestsList->DeleteAllItems() ;

	if (true == _aRequestResults.empty())
		return ;

  NSRequestResultIter it = _aRequestResults.end() ;
  do
	{
    it-- ;
		string sSearchName = (*it)->getRequestName() ;
    TListWindItem Item(sSearchName.c_str(), 0) ;
    _pRequestsList->InsertItem(Item) ;
	}
  while (it != _aRequestResults.begin()) ;
}

void
NSRequeteDialog::LvnGetSearchDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int BufLen = 1024 ;
	static char buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1: 	// chemin
    	sprintf(buffer, "%s", ((_aRequestResults[index])->getRequestLabel()).c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
	}
}

//// Note : Ce TrouveLibElement appelle la fonction GlobalDkd::decodeNoeud
// pour chaque noeud (élément) du chemin, les noeuds étant séparés par des '|'
// Pour les libellés des chemins de requete, on doit donc passer le chemin de
// recherche, qui a déjà décalé les éléments en +00+00
//

voidNSRequeteDialog::TrouveLibElement(string sElement, string& sLibElement)
{
	string sCode, sLibelle;
	string sCodeLex;

	string sLang = "";
	if (pContexte && pContexte->getUtilisateur())
		sLang = pContexte->getUtilisateur()->donneLang() ;

	// on remet à "" sLibElement
	sLibElement = "" ;

	size_t pos1 = 0 ;
	size_t pos2 = sElement.find(searchPathSeparator) ;

	bool bContinue = true ;
	while (bContinue)
	{
  	if (NPOS != pos2)
    	sCode = string(sElement, pos1, pos2-pos1) ;
    else
    	sCode = string(sElement, pos1, strlen(sElement.c_str())-pos1) ;

    // cas particuliers des codes vides et jokers
    if (string("") == sCode)
    	sLibelle = string("") ;
    else if ('~' == sCode[0])
    	sLibelle = string(sCode, 3, 2) ;
    else
    {
    	// cas général d'un noeud à décoder : on doit former une PatPatho
      // qui contient uniquement ce noeud
      NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

      // cas des valeurs chiffrées : traité à part
      if ('2' == sCode[0])
      {
      	string sUnite, sFormat, sValeur ;

        size_t pos3 = sCode.find(searchNodeSeparator) ;        if (NPOS != pos3)
        {
        	sUnite = string(sCode, 0, pos3) ;
          size_t pos4 = sCode.find(searchNodeSeparator, pos3+1) ;
          if (NPOS != pos4)
          {
          	sFormat = string(sCode, pos3+1, pos4-pos3-1) ;            // attention, on skippe le '$' pour récupérer la valeur
            sValeur = string(sCode, pos4+2, strlen(sCode.c_str())-pos4-2) ;
          }
          else
          {
          	sFormat = string(sCode, pos3+1, strlen(sCode.c_str())-pos3-1) ;
            sValeur = string("") ;
          }
        }
        else
        {
        	sUnite  = sCode ;
          sFormat = string("") ;
          sValeur = string("") ;
        }

        Message Msg ;
        Msg.SetUnit(sUnite) ;
        Msg.SetComplement(sValeur) ;
        PatPatho.ajoutePatho(sFormat, &Msg, 0) ;
      }
      else
      {
      	// on parse le noeud pour former la patpatho
        NSPatPathoInfo Data ;
        Data.setLocalisation(string(BASE_LOCALISATION_LEN, '0')) ;

        bool bParse = true ;
        size_t pos3 = 0 ;
        size_t pos4 = sCode.find(searchNodeSeparator) ;

        while (bParse)
        {
        	if (NPOS != pos4)
          	sCodeLex = string(sCode, pos3, pos4-pos3) ;          else
          	sCodeLex = string(sCode, pos3, strlen(sCode.c_str())-pos3) ;

          if (("WPLUR" == string(sCodeLex, 0, 5)) || ("WPLUS" == string(sCodeLex, 0, 5)))
          	Data.setPluriel(sCodeLex) ;
          else if (string(sCodeLex,0,3) == "WCE")
          	Data.setCertitude(sCodeLex) ;          else
          	Data.setLexique(sCodeLex) ;

          if ((pos4 == NPOS) || (strlen(sCode.c_str())-1 == pos4))
          	bParse = false ;
          else
          {
          	pos3 = pos4 + 1 ;
            pos4 = sCode.find(searchNodeSeparator, pos3) ;
          }
        }

        PatPatho.push_back(new NSPatPathoInfo(Data)) ;
      }

      // On décode le noeud contenu dans pPatPatho :
      // dans ce cas on passe un chemin (contextuel) vide
      GlobalDkd Dcode(pContexte, sLang, "", &PatPatho) ;
      Dcode.decodeNoeud() ;
      sLibelle = Dcode.getDcodeur() ;
    }

    if (string("") != sLibElement)
    	sLibElement += string(1, searchNodeSeparator) ;
    sLibElement += sLibelle ;

    if ((NPOS == pos2) || (strlen(sElement.c_str())-1 == pos2))
    	bContinue = false ;
    else
    {
    	pos1 = pos2 + 1 ;
      pos2 = sElement.find(searchPathSeparator, pos1) ;
    }
  }
}

// fonction laissée pour une utilisation future// (quand on récupèrera des requètes sur disque)
bool
NSRequeteDialog::InitChemArray()
{
  _pChemArray->vider() ;
  _nbChem = 0 ;

  return true ;
}

void
NSRequeteDialog::CmNouveau()
{
  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
  int idRet = MessageBox("Etes-vous sûrs de vouloir prendre une nouvelle requête ?", sCaption.c_str(), MB_YESNO);

  if (IDNO == idRet)
    return ;

  // Dans ce cas, on remet tout à zéro sans aucune pitié...
  _sLibReq = string("") ;
  _pEditReq->SetText("") ;
  _bReqModeDoc = true ;
  _pModeDoc->Check() ;
  _pModePat->Uncheck() ;
  _bReqEnCours = false ;

  _bCritPat = false ;
  _pCritPat->metAZero() ;
  _pCritPatBox->Uncheck() ;

  _bCritDoc = false ;
  _pCritDoc->metAZero() ;
  _pCritDocBox->Uncheck() ;

  _bCherchePatient  = false ;
  _bChercheDocument = false ;
  _sFileName = string("") ;
  _pNomReq->SetText("") ;

  _pChemArray->vider() ;
  _nbChem = 0;
  _pListeChem->DeleteAllItems();
  _VectPatResultat.vider();
  _VectDocResultat.vider();
  _nbPatTotal   = 0 ;
  _nbPatCritPat = 0 ;
  _nbPatCritDoc = 0 ;
  _nbPatResult  = 0 ;
  _nbDocCritPat = 0 ;
  _nbDocCritDoc = 0 ;
  _nbDocResult  = 0 ;
}

bool
NSRequeteDialog::ChargerRequete(string sNomFichier)
{
	if (string("") == sNomFichier)
		return false ;

	ifstream inFile ;
  inFile.open(sNomFichier.c_str()) ;
	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" (") + sNomFichier + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return false ;
	}

	string header, line ;
	string sParam[6] = { string(""), string(""), string(""), string(""), string(""), string("") } ;

	// on récupère le header du fichier
	if (inFile.eof())
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "emptyResearchFile") ;
    sErrorText += string(" (") + sNomFichier + string(")") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
		return false ;
	}

  getline(inFile, header) ;

	if (string("") == header)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "emptyResearchFileHeader") ;
    sErrorText += string(" (") + sNomFichier + string(")") ;
		erreur(sErrorText.c_str(), warningError, 0) ;
    return false;	}

	size_t i = 0, k = 0 ;

	while (i < strlen(header.c_str()))
	{
		if ('\"' == header[i])
    {
    	k++ ;

      // les headers de requete comportent 3 paramètres
      if (3 == k)
      {
      	string sErrorText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "badResearchFileHeader") ;
    		sErrorText += string(" (") + sNomFichier + string(")") ;
				erreur(sErrorText.c_str(), warningError, 0) ;
        return false ;
      }
    }
    else
    	sParam[k] += header[i] ;

    i++ ;
  }

  // mode de la requete
  if (string("1") == sParam[2])
    _bReqModeDoc = true ;
  else
    _bReqModeDoc = false ;

  // au cas où...
  _pChemArray->vider() ;
  _aRequestResults.vider() ;
  _nbChem = 0 ;

  // boucle de chargement de la requete
  while (!inFile.eof())
  {
    getline(inFile,line) ;

    if (strlen(line.c_str()) >= 3)
    {
      string sType = string(line, 0, 3) ;

      size_t pos2 = line.find('\"') ;
      if (NPOS == pos2)
        continue ;

      string sValues = string(line, pos2 + 1, strlen(line.c_str()) - pos2 - 1) ;

      if      (string("pat") == sType)
        ChargerRequeteCritPat(sValues) ;
      else if (string("doc") == sType)
        ChargerRequeteCritDoc(sValues) ;
      else if (string("lib") == sType)
      	ChargerRequeteCritLib(sValues, sParam[0]) ;
      else if (string("req") == sType)
      	ChargerRequeteCritReq(sValues) ;
    }
  } // fin du while

	inFile.close() ;

	AfficheListeChem() ;
  DisplaySearchList() ;

	if (_bReqModeDoc)
	{
		_pModeDoc->Check() ;
		_pModePat->Uncheck() ;
	}
	else
	{
		_pModePat->Check() ;
		_pModeDoc->Uncheck() ;
	}

	// on check les criteres
	if (_bCritPat)
		_pCritPatBox->Check() ;
  else
  	_pCritPatBox->Uncheck() ;

	if (_bCritDoc)
		_pCritDocBox->Check() ;
  else
  	_pCritDocBox->Uncheck() ;

	_bDirty = false ;

	return true ;
}

bool
NSRequeteDialog::ChargerRequeteCritPat(string sLib)
{
	// critères patient
	_pCritPat->metAZero() ;
	_bCritPat = true ;

  if (string("") == sLib)
    return false ;

	size_t pos1 = 0 ;
	size_t pos2 = sLib.find('\"', pos1) ;
  if (NPOS == pos2)
		return false ;

	int iIndice = 0 ;

	bool bKeepTurning = true ;
	while (bKeepTurning)
	{
		string sData = string(sLib, pos1, pos2 - pos1) ;

		switch(iIndice)
    {
    	case 0 : _pCritPat->setNom(sData) ;      break ;
      case 1 : _pCritPat->setPrenom(sData) ;   break ;
      case 2 : _pCritPat->setSexe(sData) ;     break ;
      case 3 : _pCritPat->setDateN1(sData) ;   break ;
      case 4 : _pCritPat->setDateN2(sData) ;   break ;
      case 5 : _pCritPat->setSitfam(sData) ;   break ;
      case 6 : _pCritPat->setCodePost(sData) ; break ;
      case 7 : _pCritPat->setVille(sData) ;    break ;
    }

		pos1 = pos2 + 1 ;
		pos2 = sLib.find('\"', pos1) ;
		if (NPOS == pos2)
			bKeepTurning = false ;
    else
    	iIndice++ ;
	}

	if (7 == iIndice)
		return true ;
	else
  	return false ;
}

bool
NSRequeteDialog::ChargerRequeteCritDoc(string sLib)
{
	_pCritDoc->metAZero() ;
	_bCritDoc = true ;

  if (string("") == sLib)
    return false ;

	size_t pos1 = 0 ;
	size_t pos2 = sLib.find('\"', pos1) ;
  if (NPOS == pos2)
		return false ;

	int iIndice = 0 ;

	bool bKeepTurning = true ;
	while (bKeepTurning)
	{
		string sData = string(sLib, pos1, pos2 - pos1) ;

		switch(iIndice)
    {
    	case 0 : _pCritDoc->sCodeAuteur  = sData ; break ;
      case 1 : _pCritDoc->sTitreAuteur = sData ; break ;
      case 2 : _pCritDoc->sCodeRoot    = sData ; break ;
      case 3 : _pCritDoc->sDate1       = sData ; break ;
      case 4 : _pCritDoc->sDate2       = sData ; break ;
    }

		pos1 = pos2 + 1 ;
		pos2 = sLib.find('\"', pos1) ;
		if (NPOS == pos2)
			bKeepTurning = false ;
    else
    	iIndice++ ;
	}

	if (4 == iIndice)
		return true ;
	else
  	return false ;
}

bool
NSRequeteDialog::ChargerRequeteCritLib(string sLib, string sVerNum)
{
  size_t pos1 = 0 ;
  size_t pos2 = sLib.find('\"', pos1) ;
  if (NPOS == pos2)
    return false ;
  string sData = string(sLib, pos1, pos2 - pos1) ;

  NSRequestResult* pRR = new NSRequestResult ;
  pRR->setRequestLabel(sData) ;

  if (string("1.0") == sVerNum)
    pRR->setRequestName(string("?")) ;
  else  {
    // libellé de requete
    pos1 = pos2 + 1 ;
    pos2 = sLib.find('\"', pos1) ;
    if (NPOS == pos2)
    	pRR->setRequestName(string("?")) ;
    else
    {
    	sData = string(sLib, pos1, pos2 - pos1) ;
    	pRR->setRequestName(sData) ;
    }
  }

  _aRequestResults.push_back(pRR) ;

  return true ;
}

bool
NSRequeteDialog::ChargerRequeteCritReq(string sLib)
{
	string sCheminBrut, sLibelle, sCheminRech;

	// chemin de requete
	size_t pos1 = 0 ;
  size_t pos2 = sLib.find('\"', pos1) ;
  if (pos2 == NPOS)
  	return false ;

  sCheminBrut = string(sLib, pos1, pos2 - pos1) ;

  NSEditReqDialog* pEditReqDlg =
                        new NSEditReqDialog(this, pContexte, pNSResModule) ;

  // on charge le tableau Pere avec le chemin brut
  pEditReqDlg->ParsingChemin(sCheminBrut, true) ;
  // On reconstitue le chemin de recherche équivalent  // grace au dialogue NSEditReqDialog
  pEditReqDlg->ComposeChemin(sCheminRech, pEditReqDlg->pPereArray, true) ;
  delete pEditReqDlg ;

  // on forme enfin le libellé
  TrouveLibElement(sCheminRech, sLibelle) ;
  _pChemArray->push_back(new NSElement(sCheminBrut, sLibelle, sCheminRech)) ;
  _nbChem++ ;

	return true ;
}

void
NSRequeteDialog::CmOuvrir()
{
	char path[1024] ;
	// on choisi d'abord le répertoire par défaut des requetes (NREQ)
	strcpy(path, (pContexte->PathName("NREQ")).c_str()) ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
                                  "Tous fichiers (*.NRQ)|*.nrq|",
                                  0, path, "NRQ") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
		return ;

	_sFileName = string(initData.FileName);

	if (false == ChargerRequete(_sFileName))
	{
		erreur("Impossible de charger le fichier requete spécifié.", standardError, 0, GetHandle()) ;
		return;
	}

	size_t pos = _sFileName.find_last_of('\\') ;
	if (pos != NPOS)
		_sFileName = string(_sFileName, pos+1, strlen(_sFileName.c_str())-pos-1) ;

	_pNomReq->SetText(_sFileName.c_str()) ;
}

void
NSRequeteDialog::EcrireRequete(string& sOut)
{
	struct date dateSys;
	char dateJour[10];

	getdate(&dateSys) ;
	sprintf(dateJour, "%4d%02d%02d", dateSys.da_year, dateSys.da_mon, dateSys.da_day) ;

	// on écrit d'abord le header (1ère ligne) dans sOut
	sOut += string("2.0") + "\"" ;      // numéro de version
	sOut += string(dateJour) + "\"" ;   // date du jour
	if (_bReqModeDoc)
		sOut += string("1\n") ;
	else
		sOut += string("2\n") ;

	// on écrit les critères patient
	if (_bCritPat)
	{
		sOut += string("pat")             + string(1, '\"') ;
    sOut += _pCritPat->getNom()       + string(1, '\"') ;
    sOut += _pCritPat->getPrenom()    + string(1, '\"') ;
    sOut += _pCritPat->getSexe()      + string(1, '\"') ;
    sOut += _pCritPat->getDateN1()    + string(1, '\"') ;
    sOut += _pCritPat->getDateN2()    + string(1, '\"') ;
    sOut += _pCritPat->getSitfam()    + string(1, '\"') ;
    sOut += _pCritPat->getCodePost()  + string(1, '\"') ;
    sOut += _pCritPat->getVille()     + string(1, '\"') ;
    sOut += string("$\n") ;
	}

	// on écrit les critères document
	if (_bCritDoc)
	{
  	sOut += string("doc")           + string(1, '\"') ;
    sOut += _pCritDoc->sCodeAuteur  + string(1, '\"') ;
    sOut += _pCritDoc->sTitreAuteur + string(1, '\"') ;
    sOut += _pCritDoc->sCodeRoot    + string(1, '\"') ;
    sOut += _pCritDoc->sDate1       + string(1, '\"') ;
    sOut += _pCritDoc->sDate2       + string(1, '\"') ;
    sOut += string("$\n") ;
	}

	// on écrit tous les chemins bruts de la requete
  int k = 1 ;  char numChem[5] ;

	if (_pChemArray && (false == _pChemArray->empty()))
	{
  	for (NSEltIter i = _pChemArray->begin() ; _pChemArray->end() != i ; i++)
    {
    	sprintf(numChem, "%d", k) ;
      sOut += string("req")   + string(numChem) + string(1, '\"') ;
      sOut += (*i)->getCode() + string(1, '\"') ;
      sOut += string("$\n") ;
      k++ ;
    }
	}

  if (false == _aRequestResults.empty())
	{
  	NSRequestResultIter it = _aRequestResults.begin() ;
    for ( ; _aRequestResults.end() != it ; it++)
    {
      // on écrit le libellé de requête
      sOut += string("lib") + string(" \"") ;
      sOut += (*it)->_sRequestBooleanLabel + string(1, '\"') ;
      sOut += (*it)->_sRequestName         + string(1, '\"') ;
      sOut += string("$\n") ;
    }
  }
}

voidNSRequeteDialog::CmEnregistrer()
{
	// (on récupère la dernière version du libellé de la requete)	// pEditReq->GetText(sLibReq) ;

	NSNomGdDialog* pNomGdDlg =		new NSNomGdDialog(this, pContexte->PathName("NREQ"), _sFileName, string("nrq"), pContexte) ;

	if (IDOK != pNomGdDlg->Execute())	{
  	delete pNomGdDlg ;
    return ;
	}

	// on peut enregistrer sous le nom de fichier proposé
	string sNomFichier = pNomGdDlg->sFichier ;
  delete pNomGdDlg ;

	if (string("") == sNomFichier)
		return ;

  string sFichierRequete = string("") ;
	EcrireRequete(sFichierRequete) ;

  ofstream outFile ;
	outFile.open(sNomFichier.c_str()) ;
	if (!outFile)
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
    sErrorText += string(" (") + sNomFichier + string(")");
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
  }

	for (size_t i = 0 ; i < strlen(sFichierRequete.c_str()) ; i++)
		outFile.put(sFichierRequete[i]) ;

	outFile.close() ;

  _bDirty = false ;

	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  string sText    = pContexte->getSuperviseur()->getText("searchingRequestForm", "researchSavedOnFile") ;
	MessageBox(sText.c_str(), sCaption.c_str(), MB_OK) ;

	size_t pos = sNomFichier.find_last_of('\\') ;
	if (pos != NPOS)
		_sFileName = string(sNomFichier, pos+1, strlen(sNomFichier.c_str())-pos-1) ;
	else
		_sFileName = sNomFichier ;

	_pNomReq->SetText(_sFileName.c_str()) ;
}

voidNSRequeteDialog::CmNewReq()
{
	if (_bReqEnCours)
		return ;

	NSEditReqDialog* pEditReqDlg = new NSEditReqDialog(this, pContexte, pNSResModule) ;

	if (pEditReqDlg->Execute() == IDOK)	{
  	string sCode     = pEditReqDlg->sCheminPere ;
    string sLibelle  = string("") ;
    string sChemRech = pEditReqDlg->sCheminRechPere ;
    int     lastIndex ;
    TrouveLibElement(sChemRech, sLibelle) ;
    // Note : à ce niveau, la variable décalage est utilisée
    // pour stocker le chemin de recherche (car on est dans une requete et non un élément)
    _pChemArray->push_back(new NSElement(sCode, sLibelle, sChemRech)) ;
    _nbChem++ ;
    AfficheListeChem() ;
    lastIndex = _pListeChem->GetItemCount() - 1 ;
    _pListeChem->SetFocus() ;
    _pListeChem->SetSel(lastIndex, true) ;

    _bDirty = true ;
	}

	delete pEditReqDlg ;
}

voidNSRequeteDialog::CmModReq()
{
	if (_bReqEnCours)
		return ;

	int eltChoisi = _pListeChem->IndexItemSelect() ;

	if (-1 == eltChoisi)
	{
		erreur("Vous devez sélectionner un chemin à modifier.", standardError, 0) ;
		return ;
	}

	NSEditReqDialog* pEditReqDlg =
        new NSEditReqDialog(this, pContexte, pNSResModule) ;

	// on charge le tableau Pere
	// (la donnee code contient le chemin brut avec les codes sens et les décalages)

  NSElement* pElement = (*_pChemArray)[eltChoisi] ;
  if ((NSElement*) NULL == pElement)
  {
    delete pEditReqDlg ;
    return ;
  }

	pEditReqDlg->ParsingChemin(pElement->getCode(), true) ;

	if (pEditReqDlg->Execute() == IDOK)
	{
		string sCode     = pEditReqDlg->sCheminPere ;
    string sChemRech = pEditReqDlg->sCheminRechPere ;

    string sLibelle = string("") ;
    TrouveLibElement(sChemRech, sLibelle) ;

    pElement->setCode(sCode) ;
    pElement->setLabel(sLibelle) ;
    pElement->setDecal(sChemRech) ;

    AfficheListeChem() ;
    _pListeChem->SetFocus() ;
    _pListeChem->SetSel(eltChoisi, true) ;

    _bDirty = true ;
	}

	delete pEditReqDlg ;
}

voidNSRequeteDialog::CmDelReq()
{
	if ((true == _bReqEnCours) || _pChemArray->empty())
		return ;

	int eltChoisi = _pListeChem->IndexItemSelect() ;
	if (-1 == eltChoisi)
	{
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "aPathMustBeSelected") ;
		erreur(sWarningText.c_str(), warningError, 0) ;		return ;
	}

	int j = 0 ;
	for (NSEltIter i = _pChemArray->begin() ; _pChemArray->end() != i ; i++, j++)
	{
		if (j == eltChoisi)
		{
    	delete *i ;
			_pChemArray->erase(i) ;
			_nbChem -= 1 ;
      _bDirty = true ;
			break ;
		}
	}

	AfficheListeChem() ;
}

voidNSRequeteDialog::CmDupReq()
{
	if ((true == _bReqEnCours) || _pChemArray->empty())
		return ;

	int eltChoisi = _pListeChem->IndexItemSelect() ;	if (-1 == eltChoisi)
	{
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "aPathMustBeSelected") ;
		erreur(sWarningText.c_str(), warningError, 0) ;
		return ;
	}

	NSElement* pElementSource = (*_pChemArray)[eltChoisi] ;
	_pChemArray->push_back(new NSElement(*pElementSource)) ;	_nbChem++ ;
  _bDirty = true ;

	AfficheListeChem() ;}

voidNSRequeteDialog::CmCriteresPatient()
{
	if (_bReqEnCours)
	{
		if (_bCritPat)
			_pCritPatBox->Check() ;
		else
			_pCritPatBox->Uncheck() ;

		return ;	}

	NSCritReqPatDialog* pCritPatDlg = new NSCritReqPatDialog(this, pContexte, pNSResModule) ;	if (_bCritPat)
		*(pCritPatDlg->pData) = *_pCritPat ;

	if (pCritPatDlg->Execute() == IDOK)	{
		*_pCritPat = *(pCritPatDlg->pData) ;
		if (pCritPatDlg->bEffacer)
		{
			_pCritPatBox->Uncheck() ;
			_bCritPat = false ;
		}
		else
		{
			_pCritPatBox->Check() ;
			_bCritPat = true ;
		}
    _bDirty = true ;
	}
	else
	{
		if (_bCritPat)
			_pCritPatBox->Check() ;
		else
			_pCritPatBox->Uncheck() ;
	}

	delete pCritPatDlg ;}

voidNSRequeteDialog::CmCriteresDocument()
{
	if (_bReqEnCours)
	{
  	if (_bCritDoc)
    	_pCritDocBox->Check() ;
    else
    	_pCritDocBox->Uncheck() ;
    return ;
	}

	NSCritReqDocDialog* pCritDocDlg = new NSCritReqDocDialog(this, pContexte, pNSResModule) ;
	if (_bCritDoc)
		*(pCritDocDlg->pData) = *_pCritDoc ;

	if (pCritDocDlg->Execute() == IDOK)
	{
  	*_pCritDoc = *(pCritDocDlg->pData) ;
    if (pCritDocDlg->bEffacer)
    {
    	_pCritDocBox->Uncheck() ;
      _bCritDoc = false ;
    }
    else
    {
    	_pCritDocBox->Check() ;
      _bCritDoc = true ;
    }
    _bDirty = true ;
	}
  else
  {
  	if (_bCritDoc)
    	_pCritDocBox->Check() ;
    else
    	_pCritDocBox->Uncheck() ;
	}

	delete pCritDocDlg ;
}

voidNSRequeteDialog::CmModePat()
{
	if (false == _bReqEnCours)
	{
  	if (_pModePat->GetCheck() == BF_CHECKED)
    	_bReqModeDoc = false ;
    else
    	_bReqModeDoc = true ;
	}

	if (_bReqModeDoc)	{
  	_pModeDoc->Check() ;
    _pModePat->Uncheck() ;
	}
	else
	{
		_pModeDoc->Uncheck() ;
		_pModePat->Check() ;
	}

  _bDirty = true ;
}
voidNSRequeteDialog::CmModeDoc()
{
	if (false == _bReqEnCours)
	{
  	if (_pModeDoc->GetCheck() == BF_CHECKED)
    	_bReqModeDoc = true ;
    else
    	_bReqModeDoc = false ;
	}

	if (_bReqModeDoc)
	{
  	_pModeDoc->Check() ;
    _pModePat->Uncheck() ;
	}
  else
  {
  	_pModeDoc->Uncheck() ;
    _pModePat->Check() ;
	}
  _bDirty = true ;
}

void
NSRequeteDialog::addToReq(string sCompl)
{
  if (string("") == sCompl)
    return ;

  _pEditReq->GetText(_sLibReq) ;
	_sLibReq += sCompl ;
	_pEditReq->SetText(_sLibReq.c_str()) ;
}

void
NSRequeteDialog::CmOuvrante()
{
	if (_bReqEnCours)
		return ;

	addToReq(string("(")) ;
}

voidNSRequeteDialog::CmET()
{
	if (_bReqEnCours)
		return ;

  addToReq(string(" * ")) ;
}
voidNSRequeteDialog::CmOU()
{
	if (_bReqEnCours)
		return ;

  addToReq(string(" + ")) ;
}

voidNSRequeteDialog::CmFermante()
{
	if (_bReqEnCours)
		return ;

  addToReq(string(")")) ;
}

voidNSRequeteDialog::CmInsererChemin()
{
	if (_bReqEnCours)
		return ;

	int numReq = _pListeChem->IndexItemSelect() + 1 ;
	char cNumReq[10] = "" ;
	sprintf(cNumReq, "%d", numReq) ;
  addToReq(string(cNumReq)) ;
}

intNSRequeteDialog::CherchePatientSuivant(NSPersonsAttributeIter& iterPatient, NSPatInfo* pPatInfo){	if (((NSPatInfo*) NULL == pPatInfo) ||                         ((NSPersonsAttributesArray*) NULL == _pPersonList))		return 0 ;	if (NULL == iterPatient)  	iterPatient = _pPersonList->begin() ;  else  	iterPatient++ ;  if (_pPersonList->end() == iterPatient)  	return 0 ;	*pPatInfo = NSPatInfo(*iterPatient, pContexte) ;	return 1 ;}int
NSRequeteDialog::ChercheDocumentSuivant(DocumentIter& iterDoc, NSDocumentHisto* pDocHisto)
{
	if (NULL == iterDoc)
		iterDoc = _VectDocument.begin() ;	else		iterDoc++ ;	if (iterDoc == _VectDocument.end())		return 0 ;

	VecteurString VectString ;
	NSLinkManager* pLink = _Patient.getGraphPerson()->getLinkManager() ;

  NSPatPathoArray EmptyPpt(pContexte->getSuperviseur()) ;

	(*iterDoc)->setPatPatho(&EmptyPpt) ;
	bool bOk = false ;

	// On remonte le lien data du méta-document
	pLink->TousLesVrais((*iterDoc)->getCodeDocMeta(), NSRootLink::docData, &VectString) ;
	if (false == VectString.empty())
	{
		string sCodeDocData = *(*(VectString.begin())) ;
    string sCodePat = string(sCodeDocData, 0, PAT_NSS_LEN) ;
    string sCodeDoc = string(sCodeDocData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

    (*iterDoc)->setPatient(sCodePat) ;
    (*iterDoc)->setDocument(sCodeDoc) ;

    NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&DocPpt) ;

    if (((*iterDoc)->ParseMetaDonnees()) &&
            ((*iterDoc)->DonnePatPatho(&DocPpt, _Patient.getGraphPerson())))
    {
      (*iterDoc)->setPatPatho(&DocPpt) ;
    	bOk = true ;
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

    if ((*iterDoc)->ParseMetaDonnees())
    {
      NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
      (*iterDoc)->initFromPatPatho(&DocPpt) ;

    	DocPpt.ajoutePatho("ZDOCU1", 0) ;

      (*iterDoc)->setPatPatho(&DocPpt) ;
      bOk = true ;
    }
	}

	//problème de récupération de la patpatho (ou de parsing) : le document
	//ne figure pas dans l'historique

	if (false == bOk)		return -1 ;

	*pDocHisto = *(*iterDoc) ;
	return 1 ;
}


// Fonction qui compare deux valeurs selon le format du champ complement// on a soit une valeur exacte, et on vérifie si val1 == val2,
// soit un intervalle, et on vérifie si val2 est inclus dans val1
// Note : les string ne contiennent ici que la valeur, non le '$'
// les unités sont traitées en amont pour l'instant. Ici les deux valeurs
// sont considérées de même unité
bool
NSRequeteDialog::ValeurEquivalente(string sVal1, string sVal2)
{
	string sLang = "" ;
	if ((pContexte) && (pContexte->getUtilisateur()))
		sLang = pContexte->getUtilisateur()->donneLang() ;

  gereNum num1 = gereNum(pContexte->getSuperviseur(), sLang) ;
  gereNum num2 = gereNum(pContexte->getSuperviseur(), sLang) ;
  string  sUnit1 = "", sFmt1 = ""; // valeurs bidon pour pouvoir instancier les gereNum
  string  sUnit2 = "", sFmt2 = ""; // valeurs bidon pour pouvoir instancier les gereNum

  num1.instancier(&sVal1, &sUnit1, &sFmt1) ;
  num2.instancier(&sVal2, &sUnit2, &sFmt2) ;

  if (num1.estExact())
	{
  	if (num2.estExact())
    {
    	if (num1.getValeur() == num2.getValeur())
      	return true ;
      else
      	return false ;
    }
    else if (num2.estInfEgal() && num2.estSupEgal())
    {
    	// cas de l'intervalle qui représente une valeur exacte...
      if ((num1.getValeur() == num2.getValeurInf()) &&
          (num1.getValeur() == num2.getValeurSup()))
      	return true ;
      else
      	return false ;
    }
    else
    	return false ;
	}
  else if (num1.estInf() && num1.estSup())
  {
  	// Ici si num2 n'a pas une borne inf ET une borne sup
    // il n'est forcément pas inclus dans num1
    if (num2.estInf() && num2.estSup())
    {
    	if ((num1.getValeurInf() <= num2.getValeurInf()) &&
          (num1.getValeurSup() >= num2.getValeurSup()))
      {
      	// cas de la borne inf
        if (num1.getValeurInf() == num2.getValeurInf())
        	if ((!num1.estInfEgal()) && (num2.estInfEgal()))
          	return false;

        // cas de la borne sup
        if (num1.getValeurSup() == num2.getValeurSup())
        	if ((!num1.estSupEgal()) && (num2.estSupEgal()))
          	return false ;

        return true ;
    	}
      else
      	return false ;
    }
    else if (num2.estExact())
    {
    	// cas de la valeur entre les bornes
      if ((num1.getValeurInf() < num2.getValeur()) &&
          (num1.getValeurSup() > num2.getValeur()))
      	return true ;
      // cas de la valeur égale à la borne inf
      else if ((num1.getValeurInf() == num2.getValeur()) &&
               (num1.estInfEgal()))
      	return true ;
      // cas de la valeur égale à la borne sup
      else if ((num1.getValeurSup() == num2.getValeur()) &&
               (num1.estSupEgal()))
      	return true ;
      // valeur en dehors
      else
      	return false ;
    }
    else
    	return false ;
  }
  else if (num1.estInf())
  {
  	if (num2.estInf())
    {
    	// l'intervalle ou la borne num2 doit etre au-dessus de la borne num1
      if (num1.getValeurInf() <= num2.getValeurInf())
      {
      	// cas de la borne inf
        if (num1.getValeurInf() == num2.getValeurInf())
        	if ((!num1.estInfEgal()) && (num2.estInfEgal()))
          	return false ;

        return true ;
      }
      else
      	return false ;
    }
    else if (num2.estExact())
    {
    	// la valeur num2 doit etre au-dessus de la borne num1
      if (num1.getValeurInf() <= num2.getValeur())
      {
      	// cas de la borne inf
        if ((num1.getValeurInf() == num2.getValeur()) &&        	  (!num1.estInfEgal()))
        	return false ;

        return true;
      }
      else
      	return false ;
    }
    else
    	return false ;
	}
	else if (num1.estSup())
  {
  	if (num2.estSup())
    {
    	// l'intervalle ou la borne num2 doit etre en-dessous de la borne num1
      if (num1.getValeurSup() >= num2.getValeurSup())
      {
      	// cas de la borne sup
        if (num1.getValeurSup() == num2.getValeurSup())
        	if ((!num1.estSupEgal()) && (num2.estSupEgal()))
          	return false ;

        return true ;
      }
      else
      	return false ;
    }
    else if (num2.estExact())
    {
    	// la valeur num2 doit etre en-dessous de la borne num1
      if (num1.getValeurSup() >= num2.getValeur())
      {
      	// cas de la borne sup
        if ((num1.getValeurSup() == num2.getValeur()) && (!num1.estSupEgal()))
        	return false ;

        return true ;
      }
      else
      	return false ;
    }
    else
    	return false ;
	}

	// pour les cas impossibles...
	return false ;
}

// Fonction qui cherche le chemin de numéro numChem// dans la PatPatho du document en cours (variable globale Document)
bool
NSRequeteDialog::CheminDansDocument(int numChemin)
{
	bool   bRes = false ;

	if ((numChemin < 1) || (numChemin > _nbChem))  {
    // on laisse les messages d'erreur de 1er niveau
    erreur("Numéro de chemin erronné dans le libellé de requete.", standardError, 0, GetHandle()) ;
    return false ;
  }

  NSElement* pElement = (*_pChemArray)[numChemin - 1] ;
  if ((NSElement*) NULL == pElement)
    return false ;

	string sChemRech = pElement->getDecal() ;
  if (string("") == sChemRech)
    return false ;

  NSPatPathoArray DocPpt(pContexte->getSuperviseur()) ;
  _Document.initFromPatPatho(&DocPpt) ;

	// on ne signale pas les patpatho vides pour éviter trop de messages d'erreur
  //
	if (DocPpt.empty())
    return false ;

  string sLastElt ;
  // on extrait d'abord le dernier élément du chemin
  size_t pos = sChemRech.find_last_of(searchPathSeparator) ;
  if (NPOS != pos)
    sLastElt = string(sChemRech, pos+1, strlen(sChemRech.c_str())-pos-1) ;
  else // cas en pratique impossible
    sLastElt = sChemRech ;

  // Usual case, not a numerical value
  //
  if ((string("") == sLastElt) || ('2' != sLastElt[0]))
    return DocPpt.CheminDansPatpatho(sChemRech, string(1, searchPathSeparator)) ;

  // Numerical value
  //

  // on reprend le chemin sans le dernier élément
  if (NPOS != pos)
    sChemRech = string(sChemRech, 0, pos) ;
  else
    sChemRech = "" ;

  // on extrait l'unité et la valeur recherchée
  size_t pos1 = sLastElt.find(searchNodeSeparator) ;
  if (NPOS == pos1)
  {
    // on laisse les messages d'erreur de 1er niveau
    erreur("Valeur chiffrée sans format dans un chemin de recherche.", standardError, 0, GetHandle()) ;
    return false ;
  }

  string sUniteElt = string(sLastElt, 0, pos1) ;
  size_t pos2 = sLastElt.find(searchNodeSeparator, pos1+1) ;
  if (NPOS == pos2)
  {
    erreur("Valeur chiffrée non instanciée dans un chemin de recherche.", standardError, 0, GetHandle()) ;
    return false ;
  }

  string sValeurElt = string(sLastElt, pos2+1, strlen(sLastElt.c_str())-pos2-1) ;
  if ((string("") == sValeurElt) || ('$' != sValeurElt[0]))
  {
    // on laisse les messages d'erreur de 1er niveau
    erreur("Valeur chiffrée incorrecte dans un chemin de recherche.", standardError, 0, GetHandle()) ;
    return false ;
  }
  // on enlève le '$' pour pouvoir comparer avec la valeur du complement
  sValeurElt = string(sValeurElt, 1, strlen(sValeurElt.c_str())-1) ;

  string sValeurDoc = string("") ;
  string sUniteDoc  = string("") ;
  bRes = DocPpt.CheminDansPatpatho(0, sChemRech, &sValeurDoc, &sUniteDoc, string(1, searchPathSeparator)) ;

  // si on a trouvé une valeur après sChemRech, on vérifie si
  // elle correspond à la valeur recherchée
  if (bRes)
  {
    if (sUniteElt == sUniteDoc)
      bRes = ValeurEquivalente(sValeurElt, sValeurDoc) ;
    else
      bRes = false ;
  }

	return bRes ;
}

// Fonction qui vérifie si le patient en cours correspond aux critères patientbool
NSRequeteDialog::PatientVerifieCriteres()
{
	if (false == _pCritPat->isValidSexe(_Patient.getSexe()))
    return false ;

  if (false == _pCritPat->isValidNom(_Patient.getNom()))
    return false ;

  if (false == _pCritPat->isValidPrenom(_Patient.getPrenom()))
    return false ;

  if (false == _pCritPat->isValidDate(_Patient.getNaissance()))
    return false ;

  if (false == _pCritPat->isValidSitfam(_Patient.getSitfam()))
    return false ;

	return true ;
}

// Fonction qui vérifie si le document en cours correspond aux critères documentbool
NSRequeteDialog::DocumentVerifieCriteres()
{
	if (string("") != _pCritDoc->sCodeAuteur)
		if (_pCritDoc->sCodeAuteur != _Document.getCreator())
    	return false ;

	if (string("") != _pCritDoc->sCodeRoot)
	{
  	// évidemment on doit ici comparer les codes sens !!!
    //
    string sCodeLexique1 = _pCritDoc->sCodeRoot ;
    string sCodeSens1    = string("") ;
    NSDico::donneCodeSens(&sCodeLexique1, &sCodeSens1) ;

    NSPatPathoInfo* pRootPathoInfo = _Document.getRootPathoInfo() ;

    if ((NSPatPathoInfo*) NULL == pRootPathoInfo)
    	return false ;

    string sCodeSens2 = pRootPathoInfo->getLexiqueSens() ;

		if (sCodeSens1 != sCodeSens2)    	return false ;
	}

	if ((string("") != _pCritDoc->sDate1) || (string("") != _pCritDoc->sDate2))
  {
  	string sDateDoc = _Document.getDateDoc() ;
    NVLdVTemps tpsDateDoc ;
    tpsDateDoc.initFromDate(sDateDoc) ;

    if (string("") != _pCritDoc->sDate1)
		{
    	NVLdVTemps tpsDate1 ;
			tpsDate1.initFromDate(_pCritDoc->sDate1) ;
      if (tpsDateDoc < tpsDate1)
      	return false ;
    }

    if (string("") != _pCritDoc->sDate2)
    {
    	NVLdVTemps tpsDate2 ;
			tpsDate2.initFromDate(_pCritDoc->sDate2) ;
      if (tpsDateDoc > tpsDate2)
      	return false ;
    }
  }

	return true ;
}

// Fonction qui réinitialise les résultats de requete en fonction du modevoid
NSRequeteDialog::ReinitResults(bool bModeDoc)
{
	if (true == _pChemArray->empty())
		return ;

	for (NSEltIter i = _pChemArray->begin(); _pChemArray->end() != i; i++)
	{
		if (bModeDoc)
			(*i)->setResult(-1) ;
    else
    {
    	// en mode patient, on ne remet que les false à -1
      if (0 == (*i)->getResult())
      	(*i)->setResult(-1) ;
    }
	}
}

// Vérifie si le chemin numReq appartient au document en cours,// si le résultat n'est pas déjà dans chemin.result
// => si chemin.result vaut -1, on réévalue le résultat par CheminDansDocument
//    sinon on renvoie directement chemin.result
int
NSRequeteDialog::Interprete(int numReq)
{
	if ((numReq < 1) || (numReq > _nbChem))
	{
		// on laisse les messages d'erreur de 1er niveau
		erreur("Numéro de chemin erronné dans le libellé de requete.", standardError, 0, GetHandle()) ;
		return -1 ;
	}

  NSElement* pElement = (*_pChemArray)[numReq - 1] ;
  if ((NSElement*) NULL == pElement)
    return -1 ;

	int iRes = pElement->getResult() ;
	if (-1 == iRes)	{
  	// on réévalue la requete numReq sur le document en cours
    if (CheminDansDocument(numReq))
    {
    	pElement->setResult(1) ;
      iRes = 1 ;
    }
    else
    {
    	pElement->setResult(0) ;      iRes = 0 ;
    }
	}

	return iRes ;
}

// Fonction d'évaluation du libellé de requete// renvoie 0 ou 1 dans le cas normal, -1 si erreur
// La chaine vide est considérée comme vraie (1)
// pour que l'on puisse chercher uniquement en fonction des critères
int
NSRequeteDialog::Interprete(string sReq, size_t& cc)
{
	int  result = 1 ;
	int  result1 ;
	char oper = ' ' ;

	while (cc < strlen(sReq.c_str()))
	{
		result1 = 1 ;

    while ((cc < strlen(sReq.c_str())) && (' ' == sReq[cc]))
    	cc++ ;

    // on évalue l'opérande en cours
    if (cc < strlen(sReq.c_str()))
    {
    	// on doit avoir ici une '(' ou un numéro de requete
      if ((sReq[cc] >= '0') && (sReq[cc] <= '9'))
      {
      	string sNumReq = "" ;

        while ((cc < strlen(sReq.c_str())) && (sReq[cc] >= '0') && (sReq[cc] <= '9'))
        {
        	sNumReq += string(1, sReq[cc]) ;
          cc++ ;
        }

        if (string("") != sNumReq)
        	result1 = Interprete(atoi(sNumReq.c_str())) ;
      }
      else if ('(' == sReq[cc])
      {
      	cc++ ;
        result1 = Interprete(sReq, cc) ;
      }
      else
      {
      	// on laisse les messages d'erreur de 1er niveau
        erreur("Erreur de syntaxe dans le libellé de la requete.", standardError, 0, GetHandle()) ;
        return -1 ;
      }
    }

    // cas erreur à l'évaluation de l'opérande
    if (-1 == result1)
    	return -1 ;

    // on calcule le résultat selon l'opérateur en cours
    if      (' ' == oper)
    	result = result1 ;
    else if ('+' == oper)
    	result = result || result1 ;
    else if ('*' == oper)
    	result = result && result1 ;

    // on avance à nouveau
    while ((cc < strlen(sReq.c_str())) && (' ' == sReq[cc]))
    	cc++ ;

    // on évalue l'opérateur
    if (cc < strlen(sReq.c_str()))
    {
    	// on doit avoir ici une ')' ou un opérateur
      if (('+' == sReq[cc]) || ('*' == sReq[cc]))
      {
      	oper = sReq[cc] ;
        cc++ ;
      }
      else if (')' == sReq[cc])
      {
      	cc++ ;
        return result ;
      }
      else
      {
      	// on laisse les messages d'erreur de 1er niveau
        erreur("Erreur de syntaxe dans le libellé de la requete.", standardError, 0, GetHandle()) ;
        return -1 ;
      }
    }
	}

	return result ;
}
boolNSRequeteDialog::InitPersonList(){  string ps = string("Loading patients list") ;  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;  NautilusPilot* pPilot = pContexte->getSuperviseur()->getPilot() ;  if ((NautilusPilot*) NULL == pPilot)  {    string sError = string("Pilot is not available, leaving") ;    pContexte->getSuperviseur()->trace(&sError, 1, NSSuper::trError) ;    return false ;  }	string sLocalText = pContexte->getSuperviseur()->getText("searchingRequestForm", "loadingPatientsList") ;	_pLanceur->displayStatusMessage(sLocalText) ;	const char* serviceName = (NautilusPilot::SERV_PATIENT_LIST).c_str() ;	if ((NSPersonsAttributesArray*) NULL == _pPersonList)  	_pPersonList = new NSPersonsAttributesArray() ;  NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;	AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;  bool listOk = pPilot->personList(serviceName, _pPersonList, &AttrArray) ;	//if ((!res) || (pPatiensList->empty()))
	if (false == listOk)
	{
		std::string sTempMessage = pPilot->getWarningMessage() ;
		std::string sTempError   = pPilot->getErrorMessage() ;
		if (string("") != sTempMessage)
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), sTempMessage.c_str(), "Message Nautilus", MB_OK);
		if (string("") != sTempError)
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), sTempError.c_str(), "Message Nautilus", MB_OK);
		return false ;
	}

  ps = string("Patients list loaded") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  pPilot->runGarbageCollector() ;

	return true ;}boolNSRequeteDialog::ChargeGraphePatient(){	SetCursor(0, IDC_WAIT) ;	string user = pContexte->getUtilisateurID() ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;
  if ((NSSuper*) NULL == pSuper)
    return false ;

  NautilusPilot* pPilot = pSuper->getPilot() ;
  if ((NautilusPilot*) NULL == pPilot)
    return false ;

  NSPersonGraphManager* pGraphPerson = _Patient.getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphPerson)
    return false ;

  string ps = string("Loading patient ") + _Patient.getNss() ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	NSBasicAttributeArray    AttrArray ;
	NSPersonsAttributesArray List ;

	// ouverture standard
	AttrArray.push_back(new NSBasicAttribute(PERSON,   _Patient.getNss())) ;
	AttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
	AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pSuper->getConsoleString())) ;
	AttrArray.push_back(new NSBasicAttribute(INSTANCE, pSuper->getInstanceString())) ;

	bool res = pPilot->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(),
                                    pGraphPerson->getDataGraph(), &List, &AttrArray) ;

  ps = string("Patient graph loaded") ;
  pSuper->trace(&ps, 1, NSSuper::trDetails) ;

	if (false == res)
	{
    std::string sTempMessage = pPilot->getWarningMessage() ;
    std::string sTempError   = pPilot->getErrorMessage() ;
    if (string("") != sTempMessage)
      pSuper->trace(&sTempMessage, 1, NSSuper::trError) ;
    if (string("") != sTempError)
      pSuper->trace(&sTempError, 1, NSSuper::trError) ;
		return false ;
	}

	if (((NSDataGraph*) NULL == pGraphPerson->getDataGraph()) ||
    	                             (false == pGraphPerson->graphPrepare()))
	{
    string sWarning = string("Cannot get graph for patient ") + _Patient.getNss() ;
    pSuper->trace(&sWarning, 1, NSSuper::trError) ;
		return false;
	}

	pGraphPerson->setNeedUnlock(false) ;
	pGraphPerson->setReadOnly(true) ;

	if (false == List.empty())
	{
		string sIsLocked = List.getAttributeValue("locked") ;
		if (string("ok") == sIsLocked)
			pGraphPerson->setNeedUnlock(true) ;
		string sOperationType	= List.getAttributeValue("operationType") ;
		if (string("readWrite") == sOperationType)
			pGraphPerson->setReadOnly(false) ;
	}

  ps = string("Unlocking patient") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  _Patient.debloquer() ;

	pGraphPerson->setInfoPids(&AttrArray) ;
	pGraphPerson->getDataGraph()->setLastTree() ;

	string sRootTree = pGraphPerson->getRootTree() ;
	// NSDataGraph* pGraph = Patient.pGraphPerson->pDataGraph;

	SetCursor(0, IDC_ARROW) ;

	// On prépare maintenant le VectDocument en le remplissant	// avec les métas	_VectDocument.vider();	VecteurString VectString ;	NSLinkManager* pLink = pGraphPerson->getLinkManager() ;  ps = string("Loading Frame indexes") ;  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;  // Loading Frame indexes
  //
  for (int i = 0 ; i < FRAMECOUNT ; i++)
  {
    LDVFRAME iFrame = getFrameForIndex(i) ;

    VecteurString aFrameVectString ;
    pLink->TousLesVrais(sRootTree, getLinkForFrame(iFrame), &aFrameVectString) ;
	  if (false == aFrameVectString.empty())
	  {
		  string sCodeDocIndex = *(*(aFrameVectString.begin())) ;
      NSDocumentInfo DocInfo(sCodeDocIndex, pContexte, pGraphPerson) ;

		  NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

      _VectDocument.push_back(pNSDocumentHisto) ;
	  }
  }

  ps = string("Loading synthesis") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	// chargement de la synthèse
	VectString.vider() ;
	pLink->TousLesVrais(sRootTree, NSRootLink::personSynthesis, &VectString) ;
	if (false == VectString.empty())
	{
  	string sCodeDocSynth = *(*(VectString.begin())) ;
    NSDocumentInfo DocInfo(sCodeDocSynth, pContexte, pGraphPerson) ;

    NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

    _VectDocument.push_back(pNSDocumentHisto) ;
	}

  ps = string("Loading demographics") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	// chargement de la fiche administrative
	VectString.vider();
	pLink->TousLesVrais(sRootTree, NSRootLink::personAdminData, &VectString) ;
  if (false == VectString.empty())
  {
  	string sCodeDocAdmin = *(*(VectString.begin())) ;
    NSDocumentInfo DocInfo(sCodeDocAdmin, pContexte, pGraphPerson) ;

    NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

    _VectDocument.push_back(pNSDocumentHisto) ;
  }

  ps = string("Loading health team") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

  // fabTODO
  // chargement de l'Equipe de Santé
  // Loading HealthTeam
  VectString.vider() ;
  pLink->TousLesVrais(sRootTree, NSRootLink::personHealthTeam, &VectString) ;
  if (false == VectString.empty())
	{
  	string sCodeDocHealthTeam = *(*(VectString.begin())) ;
    NSDocumentInfo DocInfo(sCodeDocHealthTeam, pContexte, pGraphPerson) ;

    NSDocumentHisto *pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;

    _VectDocument.push_back(pNSDocumentHisto) ;
  }

  ps = string("Loading documents") ;
  pSuper->trace(&ps, 1, NSSuper::trSubDetails) ;

	// chargement de tous les documents
	VectString.vider() ;
  pLink->TousLesVrais(sRootTree, NSRootLink::personDocument, &VectString) ;
  if (false == VectString.empty())
  {
  	for (EquiItemIter i = VectString.begin() ; VectString.end() != i ; i++)
    {
      NSDocumentInfo DocInfo(*(*i), pContexte, pGraphPerson) ;
    	NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(&DocInfo) ;
      _VectDocument.push_back(pNSDocumentHisto) ;
    }
  }  ps = string("Patient ") + _Patient.getNss() + string(" loaded") ;  pSuper->trace(&ps, 1, NSSuper::trDetails) ;	return true ;}
voidNSRequeteDialog::CmLancement()
{
	_pLanceur->Create() ;
}

voidNSRequeteDialog::CmLancer()
{
	if (true == _bReqEnCours)
		return ;

	_bReqEnCours = true ;

  setModeFileErr(true) ;
	SetCursor(0, IDC_WAIT) ;

  // Reseting research results
  //
	if (false == _aRequestResults.empty())
	{
  	NSRequestResultIter it = _aRequestResults.begin() ;
  	for ( ; _aRequestResults.end() != it ; it++)
    	(*it)->reinitForNewSearch() ;
  }

  int nbTrouves = 0 ;

	_nbPatTotal   = 0 ;
  _nbPatCritPat = 0 ;
  _nbPatCritDoc = 0 ;
  _nbPatResult  = 0 ;
	_nbDocCritPat = 0 ;
  _nbDocCritDoc = 0 ;
  _nbDocResult  = 0 ;

	_bCherchePatient = true ;
  NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sLocalText = pSuper->getText("searchingRequestForm", "researchProcessOnDuty") ;
	_pLanceur->displayStatusMessage(sLocalText) ;
	pSuper->getApplication()->PumpWaitingMessages() ;
  // Load the list of all patients
  //
	InitPersonList() ;

	NSPersonsAttributeIter iterPatient = NULL ;
	DocumentIter iterDoc = NULL ;

  _iPersonListSize = _pPersonList->size() ;
	_iCurrentPatient = 0 ;
  _pLanceur->displayProgress(0) ;

  sLocalText = pSuper->getText("searchingRequestForm", "numberOfPatientsLoaded") ;
  sLocalText += string(" ") + IntToString(_iPersonListSize) ;
	_pLanceur->displayStatusMessage(sLocalText) ;

  // Get Pilot
  //
  NautilusPilot* pPilot = pSuper->getPilot() ;
  if ((NautilusPilot*) NULL == pPilot)  {    string sError = string("Pilot is not available, leaving") ;    pSuper->trace(&sError, 1, NSSuper::trError) ;    return ;  }

  string sCodeDocum, sNssPat ;

	// on récupère la dernière version du libellé de la requete
	//pEditReq->GetText(sLibReq);

  size_t iSubCounter = 0 ;

	while (_bCherchePatient)	{
		int ret = CherchePatientSuivant(iterPatient, &_Patient) ;

  	if (-1 == ret)    {
    	string sErrorText = pSuper->getText("searchingRequestFormErrors", "errorAccessingNextPatient") ;
      pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      break ;
    }
    else if (0 == ret)
    {
    	// fin de la base...
      _bCherchePatient = false ;
      break ;
    }

    _iCurrentPatient++ ;
    _pLanceur->displayProgress(100 * _iCurrentPatient / _iPersonListSize) ;

    sLocalText = pSuper->getText("searchingRequestForm", "analysingPatient") ;
		sLocalText += string(" ") + IntToString(_iCurrentPatient) + string(" / ") + IntToString(_iPersonListSize) ;
    string sFoundText = pSuper->getText("searchingRequestForm", "found") ;
    sLocalText += string(" ") + sFoundText + string(" -> ") + IntToString(nbTrouves) ;
		_pLanceur->displayStatusMessage(sLocalText) ;

    // on lance la recherche sur les documents du patient    // Document est initialisé au premier document du patient
    _pLanceur->pEditPat->SetText(_Patient.getszNss()) ;

    pSuper->getApplication()->PumpWaitingMessages() ;

    _nbPatTotal++ ;
    bool bNouveauPatient   = true ;    bool bNouveauPatResult = true ;

    if (false == _aRequestResults.empty())
		{
  		NSRequestResultIter it = _aRequestResults.begin() ;
  		for ( ; _aRequestResults.end() != it ; it++)
    		(*it)->_bNouveauPatResult = true ;
  	}

    // sélection des critères du patient    if (_pLanceur->bAvecPat && _bCritPat)
    	_bChercheDocument = PatientVerifieCriteres() ;
    else
    	_bChercheDocument = true ;

    if (_bChercheDocument)
    {
    	_bChercheDocument = ChargeGraphePatient() ;

      // iSubCounter++ ;
      // if (iSubCounter >= 1000)
      // {
          pPilot->runGarbageCollector() ;
      //   iSubCounter = 0 ;
      // }
    }

    if (_bChercheDocument)    {
    	_nbPatCritPat++ ;

      iterDoc = NULL ;

    	// en mode patient, on n'oublie pas de réinitialiser      // pour chaque nouveau patient

      if (false == _bReqModeDoc)      	ReinitResults() ;
    }
    else
    {
      _pLanceur->pEditDoc->SetText("! Error !") ;
      pSuper->getApplication()->PumpWaitingMessages() ;
    }

    while (_bChercheDocument)    {
    	ret = ChercheDocumentSuivant(iterDoc, &_Document) ;

      if (-1 == ret)      {
      	// on laisse les messages d'erreur de 1er niveau
        string sErrorText = pSuper->getText("searchingRequestFormErrors", "errorAccessingNextDocument") ;
        sErrorText += string(" : codeDoc = ") + (*iterDoc)->getID() ;
      	pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
        SetCursor(0, IDC_ARROW) ;
        erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
        setModeFileErr(false) ;
        _bReqEnCours = false ;
        return ;
      }
      else if (0 == ret)
      {
      	// fin du dossier patient
        _bChercheDocument = false ;
        break ;
      }

      // on lance la requete sur le document
      _pLanceur->pEditDoc->SetText(_Document.getDocument().c_str()) ;      pSuper->getApplication()->PumpWaitingMessages() ;
      _nbDocCritPat++ ;
      bool bDocumOK = true ;

      // sélection des critères du document
      if (_pLanceur->bAvecDoc && _bCritDoc)      	bDocumOK = DocumentVerifieCriteres() ;

      if (bDocumOK)      {
      	_nbDocCritDoc++ ;

        // on compte le nombre de patients correspondants        if (bNouveauPatient)
        {
        	_nbPatCritDoc++ ;
          bNouveauPatient = false ;
        }

        if ((true == _pLanceur->bAvecLib) && (false == _aRequestResults.empty()))        {
        	ret = 0 ;

        	ReinitResults(_bReqModeDoc) ;

          NSRequestResultIter it = _aRequestResults.begin() ;
          for ( ; _aRequestResults.end() != it ; it++)
          {
          	size_t pos = 0 ; // à ne pas oublier : remettre au début de la string !!!
          	int iResult = Interprete((*it)->getRequestLabel(), pos) ;
            if (1 == iResult)
            {
            	ret = 1 ;

            	if (true == _bReqModeDoc)
              {
              	(*it)->_nbDocResult++ ;

              	(*it)->_aVectDocumentResults.push_back(new NSDocumentHisto(*(*iterDoc))) ;
                // on compte le nombre de patients correspondants
          			if ((*it)->_bNouveauPatResult)
          			{
          				(*it)->_nbPatResult++ ;
            			(*it)->_bNouveauPatResult = false ;
          			}
                // pour l'instant, on charge les patients correspondants avec doublons
          			// en attendant de trouver mieux pour l'affichage des résultats
          			(*it)->_aVectPatientResults.push_back(new NSPatInfo(*iterPatient, pContexte)) ;
              }
              else
              {
              	(*it)->_nbPatResult++ ;
              	(*it)->_aVectPatientResults.push_back(new NSPatInfo(*iterPatient, pContexte)) ;
              }
            }
            pSuper->getApplication()->PumpWaitingMessages() ;
          }
        }
        else
        	ret = 1 ;
      }
      else
      	ret = 0 ;

      if (-1 == ret)      {
      	// on laisse les messages d'erreur de 1er niveau
        string sErrorText = pSuper->getText("searchingRequestFormErrors", "requestCannotBeTested") ;
        pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
        SetCursor(0, IDC_ARROW) ;
        string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
        sErrorText = pSuper->getText("searchingRequestFormWarnings", "searchProcessStopped") ;
        MessageBox(sErrorText.c_str(), sCaption.c_str(), MB_OK) ;
        setModeFileErr(false) ;
        _bReqEnCours = false ;
        return ;
      }
      else if (1 == ret)
      {
      	char cStatus[255] ;

        nbTrouves++ ;        sprintf(cStatus, "Trouvés : %d", nbTrouves) ;

        _pLanceur->displayStatusMessage(cStatus) ;
        pSuper->getApplication()->PumpWaitingMessages() ;
        if (true == _bReqModeDoc)        {
        	_nbDocResult++ ;

          // on compte le nombre de patients correspondants          if (bNouveauPatResult)
          {
          	_nbPatResult++ ;
            bNouveauPatResult = false ;
          }

          // Note : on a pris un NSDocHistoArray pour les résultats          // car cela permet d'utiliser la date de création du document.
          // cependant on ne charge pas ici la PatPatho, pour ne pas
          // encombrer la mémoire
          _VectDocResultat.push_back(new NSDocumentHisto(*(*iterDoc))) ;
          // pour l'instant, on charge les patients correspondants avec doublons          // en attendant de trouver mieux pour l'affichage des résultats
          _VectPatResultat.push_back(new NSPatInfo(*iterPatient, pContexte)) ;
        }        else
        {
        	_nbPatResult++ ;

          // Dans ce cas, on passe directement au patient suivant          _VectPatResultat.push_back(new NSPatInfo(*iterPatient, pContexte)) ;          _bChercheDocument = false ;
          break ;
        }
      }
    } // fin du while(bChercheDocument)
	} // fin du while(bCherchePatient)

	// fin de la requete : on ferme le lanceur	_pLanceur->CloseWindow(IDCANCEL) ;
	SetCursor(0, IDC_ARROW) ;
	string sCaption = string("Message ") + pSuper->getAppName() ;
	MessageBox("Recherche terminée.", sCaption.c_str(), MB_OK) ;

	// on lance la liste des résultats si nbTrouves > 0	if (0 == nbTrouves)
	{
		char cMode[30] = "" ;
    char cMsg[255] = "" ;

    if (_bReqModeDoc)    	strcpy(cMode, "Document") ;
    else
    	strcpy(cMode, "Patient") ;

    sprintf(cMsg, "Aucun %s ne correspond à la requête.", cMode) ;    MessageBox(cMsg, "Resultat de la requête", MB_OK) ;
    return ;
	}

	setModeFileErr(false) ;	_bReqEnCours = false ;

	// on renvoie IDOK pour lancer l'affichage des résultats	NSUtilDialog::CmOk() ;
}

voidNSRequeteDialog::CmFermer()
{
	if (_bReqEnCours)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "areYouSureYouWantToStopTheProcess") ;
		int idRet = MessageBox(sWarningText.c_str(), sCaption.c_str(), MB_YESNO) ;
		if (IDNO == idRet)
			return ;
	}
  else if (true == _bDirty)
  {
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "areYouSureYouWantToQuitWithoutSaving") ;
		int idRet = MessageBox(sWarningText.c_str(), sCaption.c_str(), MB_YESNO) ;
		if (IDNO == idRet)
			return ;
  }

	// on mettra ici une sauvegarde automatique de la requete	// et des résultats obtenus

	// on renvoie IDCANCEL pour sortir de la boucle dans Nautilus.cpp	NSUtilDialog::CmCancel();
}

voidNSRequeteDialog::CmAddRequest(){	if (true == _bReqEnCours)		return ;	_pEditReq->GetText(_sLibReq) ;	if (string("") == _sLibReq)		return ;	string sReqName ;  _pEditReqLabel->GetText(sReqName) ;  if (string("") == sReqName)		return ;	NSRequestResult* pRR = new NSRequestResult ;  pRR->setRequestLabel(_sLibReq) ;  pRR->setRequestName(sReqName) ;	_aRequestResults.push_back(pRR) ;  _bDirty = true ;	DisplaySearchList() ;}void
NSRequeteDialog::CmEditRequest()
{
	if ((true == _bReqEnCours) || (true == _aRequestResults.empty()))
		return ;

	_pEditReq->GetText(_sLibReq) ;
	if (string("") == _sLibReq)		return ;	string sReqName ;  _pEditReqLabel->GetText(sReqName) ;  if (string("") == sReqName)		return ;

	int eltChoisi = _pRequestsList->IndexItemSelect() ;
	if (-1 == eltChoisi)
	{
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "aRequestMustBeSelected") ;
		erreur(sWarningText.c_str(), warningError, 0) ;
		return ;
	}

	NSRequestResult* pSelectedRequestResult = _aRequestResults[eltChoisi] ;
	pSelectedRequestResult->setRequestLabel(_sLibReq) ;
  pSelectedRequestResult->setRequestName(sReqName) ;

  _bDirty = true ;

	DisplaySearchList() ;
}

void
NSRequeteDialog::CmDeleteRequest()
{
	if ((true == _bReqEnCours) || (true == _aRequestResults.empty()))
		return ;

	int eltChoisi = _pRequestsList->IndexItemSelect() ;	if (-1 == eltChoisi)
	{
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "aRequestMustBeSelected") ;
		erreur(sWarningText.c_str(), warningError, 0) ;
		return ;
	}

	NSRequestResultIter it = _aRequestResults.begin() ;
	int j = 0 ;
	for ( ; (_aRequestResults.end() != it) && (j < eltChoisi) ; it++, j++) ;
	if (_aRequestResults.end() == it)
		return ;

	delete *it ;
	_aRequestResults.erase(it) ;

  _bDirty = true ;

  DisplaySearchList() ;
}

voidNSRequeteDialog::CmDuplicateRequest(){	if ((true == _bReqEnCours) || (true == _aRequestResults.empty()))		return ;

	int eltChoisi = _pRequestsList->IndexItemSelect() ;	if (-1 == eltChoisi)
	{
		string sWarningText = pContexte->getSuperviseur()->getText("searchingRequestFormWarnings", "aRequestMustBeSelected") ;
		erreur(sWarningText.c_str(), warningError, 0) ;
		return ;
	}

	NSRequestResult* pSelectedRequestResult = _aRequestResults[eltChoisi] ;
  NSRequestResult* pNewRequestResult = new NSRequestResult ;
  pNewRequestResult->setRequestLabel(pSelectedRequestResult->getRequestLabel()) ;
  pNewRequestResult->setRequestName(pSelectedRequestResult->getRequestName()) ;

	_aRequestResults.push_back(pNewRequestResult) ;
  _bDirty = true ;

	DisplaySearchList() ;}
void
NSRequeteDialog::CmPutRequestInEditControl()
{
	int eltChoisi = _pRequestsList->IndexItemSelect() ;
	if (-1 == eltChoisi)
		return ;

	NSRequestResult* pSelectedRequestResult = _aRequestResults[eltChoisi] ;
  _pEditReq->SetText(pSelectedRequestResult->getRequestLabel().c_str()) ;
  _pEditReqLabel->SetText(pSelectedRequestResult->getRequestName().c_str()) ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSLanceReqDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSLanceReqDialog, NSUtilDialog)    EV_WM_CLOSE,
    EV_BN_CLICKED(IDC_LR_AVECLIB, CmCheckLib),
    EV_BN_CLICKED(IDC_LR_AVECPAT, CmCheckPat),
    EV_BN_CLICKED(IDC_LR_AVECDOC, CmCheckDoc),
    EV_COMMAND(IDC_LR_START,      CmDemarrer),
    EV_COMMAND(IDC_LR_STOP,       CmArreter),
END_RESPONSE_TABLE;

NSLanceReqDialog::NSLanceReqDialog(NSRequeteDialog* pere, NSContexte* pCtx, TModule* mod)
                 :NSUtilDialog(pere, pCtx, "IDD_LANCEREQ", mod)
{
	// Initialisation des donnees
	pReqDlg 	  = pere ;

	// Création de tous les "objets de contrôle"	// on laisse le groupe pour le fun...
  pCritGroup = new TGroupBox(this, IDC_LR_GROUP) ;

  pCritLib	 = new TRadioButton(this, IDC_LR_AVECLIB) ;  pCritPat   = new TRadioButton(this, IDC_LR_AVECPAT) ;
  pCritDoc   = new TRadioButton(this, IDC_LR_AVECDOC) ;
  pStatus    = new TStatic(this, IDC_LR_STATUS) ;
  pProgress  = new OWL::TGauge(this, "%d%%", IDC_PROGRESSGRAV, 18, 253, 252, 20) ;
  pProgress->SetRange(0, 100) ;
  pProgress->SetNativeUse(nuNever) ;

  pEditPat	 = new NSUtilEdit(pContexte, this, IDC_LR_EDITPAT, PAT_NSS_LEN) ;  pEditDoc   = new NSUtilEdit(pContexte, this, IDC_LR_EDITDOC, DOC_CODE_DOCUM_LEN) ;

  bAvecLib = false ;  bAvecPat = false ;
  bAvecDoc = false ;
}

NSLanceReqDialog::~NSLanceReqDialog(){
  delete pCritGroup ;
  delete pCritLib ;
  delete pCritPat ;
  delete pCritDoc ;
  delete pStatus ;
  delete pEditPat ;
  delete pEditDoc ;
}

void
NSLanceReqDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

	pCritLib->Uncheck() ;	pCritPat->Uncheck() ;
	pCritDoc->Uncheck() ;

	// on positionne les choix par défaut
	if (false == pReqDlg->_aRequestResults.empty())
  	pCritLib->Check() ;

	if (pReqDlg->useCritPat())
  	pCritPat->Check() ;

	if (pReqDlg->useCritDoc())
  	pCritDoc->Check() ;
}

// traitement de radiobutton non exclusifs (ils ne sont pas AUTO)void
NSLanceReqDialog::CmCheckLib()
{
	if (pCritLib->GetCheck() == BF_CHECKED)
		pCritLib->Uncheck() ;
	else
		pCritLib->Check() ;
}

// traitement de radiobutton non exclusifs (ils ne sont pas AUTO)
void
NSLanceReqDialog::CmCheckPat()
{
	if (pCritPat->GetCheck() == BF_CHECKED)
		pCritPat->Uncheck() ;
	else
		pCritPat->Check() ;
}

// traitement de radiobutton non exclusifs (ils ne sont pas AUTO)
void
NSLanceReqDialog::CmCheckDoc()
{
	if (pCritDoc->GetCheck() == BF_CHECKED)
		pCritDoc->Uncheck() ;
	else
		pCritDoc->Check() ;
}

voidNSLanceReqDialog::CmDemarrer()
{
	if ((pCritLib->GetCheck() != BF_CHECKED) &&
      (pCritPat->GetCheck() != BF_CHECKED) &&
      (pCritDoc->GetCheck() != BF_CHECKED))
	{
  	char cMode[30] ;
    char cMsg[255] ;

    if (pReqDlg->useModeDoc())
    	strcpy(cMode, "Document") ;
    else
    	strcpy(cMode, "Patient") ;

    sprintf(cMsg, "Attention, vous allez récupérer tous les %ss !! Voulez-vous continuer ?", cMode) ;

    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
    int idRet = MessageBox(cMsg, sCaption.c_str(), MB_YESNO) ;
    if (IDNO == idRet)
    	return ;
	}

  if (pCritLib->GetCheck() == BF_CHECKED)  	bAvecLib = true ;
  else
  	bAvecLib = false ;

  if (pCritPat->GetCheck() == BF_CHECKED)
  	bAvecPat = true ;
  else
  	bAvecPat = false ;

  if (pCritDoc->GetCheck() == BF_CHECKED)
  	bAvecDoc = true ;
  else
  	bAvecDoc = false ;

  pReqDlg->CmLancer() ;
}

voidNSLanceReqDialog::CmArreter()
{
	if (false == pReqDlg->isRunning())
	{
		CloseWindow(IDCANCEL) ;
    return ;
	}

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int idRet = MessageBox("Etes-vous sûrs de vouloir interrompre la recherche ?", sCaption.c_str(), MB_YESNO) ;
  if (IDNO == idRet)
  	return ;

  // on force la sortie de la requete
  pReqDlg->setPatientSearch(false) ;
  pReqDlg->setDocumentSearch(false) ;
}

void
NSLanceReqDialog::EvClose()
{
	CmArreter() ;
}

void
NSLanceReqDialog::displayStatusMessage(string sMessage)
{
	pStatus->SetText(sMessage.c_str()) ;
}

void
NSLanceReqDialog::displayProgress(int iProgressPercent)
{
	pProgress->SetValue(iProgressPercent) ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSEditReqWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditReqWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSEditReqWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSEditReqWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modKeys, point) ;

	TLwHitTestInfo info(point) ;
	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
	{
  	if (iRes == IDC_EDITREQ_LWPERE)
    	pDlg->CmModifier() ;
    else if (iRes == IDC_EDITREQ_LWFILS)
    	pDlg->CmAjouter() ;
	}
}


//---------------------------------------------------------------------------
//  Function: NSEditReqWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSEditReqWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)
   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;
}
// -----------------------------------------------------------------//
//  Méthodes de NSEditReqDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSEditReqDialog, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),

  EV_COMMAND(IDC_EDITREQ_INS, CmInserer),
  EV_COMMAND(IDC_EDITREQ_MOD, CmModifier),
  EV_COMMAND(IDC_EDITREQ_DEL, CmDetruire),
  EV_COMMAND(IDC_EDITREQ_AJOUTER, CmAjouter),
END_RESPONSE_TABLE ;
NSEditReqDialog::NSEditReqDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)                :NSUtilDialog(pere, pCtx, "IDD_EDITREQ", mod)
{
  pListePere = new NSEditReqWindow(this, IDC_EDITREQ_LWPERE) ;
  pListeFils = new NSEditReqWindow(this, IDC_EDITREQ_LWFILS) ;
  pPereArray = new NSEltArray ;
  pFilsArray = new NSEltArray ;

  nbPere = 0 ;  nbFils = 0 ;

  sCheminPere     = string("") ;  sCheminRechPere = string("") ;
}

NSEditReqDialog::~NSEditReqDialog(){
  delete pListePere ;
  delete pListeFils ;
  delete pPereArray ;
  delete pFilsArray ;
}

voidNSEditReqDialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  InitListePere() ;  InitListeFils() ;

  AfficheListePere() ;}

voidNSEditReqDialog::InitListePere()
{
  TListWindColumn colElt("Père", 500, TListWindColumn::Left, 0) ;
  pListePere->InsertColumn(0, colElt) ;
}

voidNSEditReqDialog::InitListeFils()
{
  TListWindColumn colFils("Fils", 500, TListWindColumn::Left, 0) ;
  pListeFils->InsertColumn(0, colFils) ;
}

voidNSEditReqDialog::AfficheListePere()
{
  char elt[255];

  NSEltArray* pRechArray;  NSEltArray* pLastEltArray;

  string sCheminRech, sCodeSens;
  BBItemData bbGuideData;
  // on remet toujours la liste à zéro  pListePere->DeleteAllItems() ;

  if (!nbPere)  {
    pListeFils->DeleteAllItems() ;
    return ;
  }

  for (int i = nbPere - 1; i >= 0; i--)  {
    sprintf(elt, "%s", (((*pPereArray)[i])->getLabel()).c_str()) ;
    TListWindItem Item(elt, 0) ;
    pListePere->InsertItem(Item) ;
  }

  // on réinitialise le tableau fils  pFilsArray->vider() ;
  nbFils = 0 ;

  // pour la recherche : on doit d'abord constituer le chemin pere  // sans le dernier élément, qui est stocké à part
  //
  pRechArray = new NSEltArray(*pPereArray) ;

  NSEltIter k = pRechArray->end() ;  k-- ;    // end() est après le dernier élément

  NSElement lastElt(*(*k)) ;  delete *k ;

  pRechArray->erase(k) ;
  // on utilise ComposeChemin pour récupérer le code sens (avec pluriel, etc.)  pLastEltArray = new NSEltArray ;
  pLastEltArray->push_back(new NSElement(lastElt)) ;

  ComposeChemin(sCodeSens, pLastEltArray) ;
  // on construit le chemin de recherche (sans le dernier élément)  // et on trouve les fils par la recherche sémantique
  // cas particulier : le pere n'a qu'un seul élément (ammorce)
  //
  if (1 == nbPere)
  {
    string sFils;
    string sDecal;

    // cas du chemin vide    RechercheFilsAmmorce(sCodeSens, sFils, sDecal) ;
    if (sFils != "")
      ParsingChemin(sFils, false, sDecal);
  }
  else
  {
    // le chemin contiendra au moins l'ammorce
    ComposeChemin(sCheminRech, pRechArray);

    // on fabrique un vecteur "fils" contenant le dernier élément    VecteurRechercheSelonCritere* pVecteurFils = new VecteurRechercheSelonCritere(GUIDE) ;

        pVecteurFils->AjouteEtiquette(sCodeSens);

        // on lance la recherche proprement dite
        bool trouve;
        pContexte->getSuperviseur()->afficheStatusMessage("Recherche des fils...");
        pContexte->getSuperviseur()->getFilGuide()->chercheCheminReseau(&sCheminRech, pVecteurFils);
        pVecteurFils->SetData(sCodeSens, &trouve, &bbGuideData);
        delete pVecteurFils;

        // on reconstruit maintenant le tableau fils
        if ((trouve) && (strcmp(bbGuideData.fils, "")))
            ParsingChemin(string(bbGuideData.fils), false, string(bbGuideData.decalageNiveau));
    }

    AfficheListeFils();

    delete pLastEltArray;
    delete pRechArray;
}
voidNSEditReqDialog::AfficheListeFils()
{
  char elt[255];

  pListeFils->DeleteAllItems() ;
  for (int i = nbFils - 1; i >= 0; i--)  {
    sprintf(elt, "%s", (((*pFilsArray)[i])->getLabel()).c_str()) ;
    TListWindItem Item(elt, 0) ;

    pListeFils->InsertItem(Item) ;  }

  pListeFils->SetFocus() ;}

voidNSEditReqDialog::RechercheFilsAmmorce(string sAmmorce, string& sFils, string& sDecal)
{
  sFils  = string("") ;
  sDecal = string("") ;

  //
  // Création d'un BBFiche
  //
  BBFiche Fiche(pContexte->getSuperviseur()) ;
  Fiche.lastError = Fiche.open() ;
  if (DBIERR_NONE != Fiche.lastError)
  {
    erreur("Il est impossible d'ouvrir le fichier des Guides.", standardError, Fiche.lastError) ;
    return;
  }

  // recherche de l'ammorce
  Fiche.lastError = Fiche.chercheClef(&sAmmorce,
                                              "CHEMIN",
                                              NODEFAULTINDEX,
                                              keySEARCHEQ,
                                              dbiWRITELOCK);

  if ((Fiche.lastError != DBIERR_NONE) && (Fiche.lastError != DBIERR_RECNOTFOUND))
  {
    erreur("Erreur à la recherche dans le fichier des Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;
  }
  else if (DBIERR_RECNOTFOUND == Fiche.lastError)
  {
    // cas ammorce non trouvée => on renvoie sFils == ""
    Fiche.close() ;
    return ;
  }

  Fiche.lastError = Fiche.getRecord() ;
  if (Fiche.lastError != DBIERR_NONE)
  {
    erreur("Erreur à la lecture d'une fiche Guides.", standardError, Fiche.lastError) ;
    Fiche.close() ;
    return ;
  }

  // on caste à cause de la structure de BBFiche (voir nsguide.cpp)
  //
  BBItemData* pItemData = static_cast<BBItemData*>(Fiche.getData()) ;
  if (pItemData)
  {
    sFils  = pItemData->getSonsList() ;    sDecal = pItemData->getLevelShift() ;
  }
  Fiche.lastError = Fiche.close() ;  if (DBIERR_NONE != Fiche.lastError)
    erreur("Erreur de fermeture de la base des Guides.", standardError, Fiche.lastError) ;
}

////////////////////////////////////////////////////////////////////////////////// Fonction : TrouveLibelle(string sCode, string& sLibelle, bool bPere)
// En entrée : un code lexique ou un ensemble de codes lexiques séparés par '/'
// En sortie : le libellé ou l'ensemble de libellés correspondant
// Note : Cette fonction traite toujours un seul élément, avec ses attributs éventuels
// Elle ne tient pas compte du décalage, qui est traité à part.
////////////////////////////////////////////////////////////////////////////////
void
NSEditReqDialog::TrouveLibelle(string sCode, string& sLibelle, bool bPere)
{

  string sCode1, sLibelle1;
  string sCode2, sLibelle2;

  // on remet à "" sLibelle
  sLibelle = "";

  size_t pos1 = 0 ;
  size_t pos2 = sCode.find(searchNodeSeparator) ;
  if (NPOS != pos2)
  {
    bool bContinue = true ;
    while (bContinue)
    {
      if (NPOS != pos2)
        sCode1 = string(sCode, pos1, pos2 - pos1) ;
      else
        sCode1 = string(sCode, pos1, strlen(sCode.c_str()) - pos1) ;

      TrouveLibelle(sCode1, sLibelle1, bPere) ;
      if (string("") == sLibelle)
        sLibelle = sLibelle1 ;
      else
        sLibelle += string("/") + sLibelle1 ;

      if ((NPOS == pos2) || (pos2 == (strlen(sCode.c_str())-1)))
        bContinue = false ;
      else
      {
        pos1 = pos2 + 1 ;
        pos2 = sCode.find(searchNodeSeparator, pos1) ;
      }
    }

    return ;
  }

  string sLang = "" ;
  if ((pContexte) && (pContexte->getUtilisateur()))
    sLang = pContexte->getUtilisateur()->donneLang() ;

  // on traite d'abord les jokers (codes peres)
  if (bPere && (sCode == "~????1"))
    sLibelle = "Tout élément (??)" ;
  else if (bPere && (sCode == "~~***1"))
    sLibelle = "Toute séquence (**)" ;
  else if (bPere && (sCode != "") && (sCode[0] == '~'))
    sLibelle = string("Joker (") + string(sCode,3,2) + string(")") ;
  else    // cas des codes commun aux fils et aux peres (codes lexiques)
  {
    if (sCode == "")
      sLibelle = "[Vide]" ;
    else
    {
      if (strlen(sCode.c_str()) == BASE_LEXI_LEN)
      {
        sCode2 = sCode; sLibelle2 = "";
        if (pContexte->getDico()->donneLibelle(sLang, &sCode2, &sLibelle2))
        {
                  // pour les codes système
                  if (sLibelle2 == "")
                  {
                      pContexte->getDico()->donneLibelleLexique(sLang, &sCode2, &sLibelle2);

                      // pour les cas non traités
                      if (sLibelle2 == "")
                          sLibelle = sCode;
                      else
                          sLibelle = sLibelle2;
                  }
                  else // cas normal
                      sLibelle = sLibelle2;
              }
              else // cas d'erreur : code introuvable
                  sLibelle = sCode;
          }
          else // cas d'erreur : code trop long ou trop court
              sLibelle = sCode;
      }
  }
}

voidNSEditReqDialog::TrouveLibelleValeur(const string sCode, string& sLibelle, bool bPere)
{
  string sUnite, sFormat, sValeur, sLibUnite ;
  size_t pos1 = sCode.find(searchNodeSeparator) ;
  size_t pos2, pos3 ;

  string sLang = "" ;
  if ((pContexte) && (pContexte->getUtilisateur()))
    sLang = pContexte->getUtilisateur()->donneLang() ;

  if (NPOS != pos1)
  {
    sUnite = string(sCode, 0, pos1) ;

    if (strlen(sUnite.c_str()) == BASE_LEXI_LEN)
      pContexte->getDico()->donneLibelle(sLang, &sUnite, &sLibUnite) ;
    else
    {
      erreur("Valeur chiffrée avec unité incorrecte.", standardError, 0, GetHandle()) ;
      sLibelle = sCode ;
      return ;
    }

    pos2 = sCode.find(searchNodeSeparator, pos1+1) ;
    if (NPOS != pos2)
    {
      sFormat = string(sCode, pos1+1, pos2-pos1-1) ;
      sValeur = string(sCode, pos2+1, strlen(sCode.c_str())-pos2-1) ;
    }
    else
    {
      sFormat = string(sCode, pos1+1, strlen(sCode.c_str())-pos1-1) ;
      sValeur = "" ;
    }

    // analyse du format
    if ('£' != sFormat[0])
    {
      erreur("Valeur chiffrée avec un format incorrect.", standardError, 0, GetHandle()) ;
      sLibelle = sCode ;
      return ;
    }

    if (bPere)  // cas des valeurs pere (instanciées)
    {
      string sVal1, sVal2, sVal, sValBrut;
      size_t    pos;

      // on ne traite que les valeurs numériques
      if ('N' != sFormat[1])
      {
        erreur("Valeur chiffrée avec format non traité.", standardError, 0, GetHandle()) ;
        sLibelle = sCode ;
        return ;
      }

      // on doit obligatoirement avoir une valeur
      if (string("") == sValeur)
      {
        erreur("Valeur chiffrée non instanciée dans un chemin de recherche.", standardError, 0, GetHandle()) ;
        sLibelle = sCode ;
        return ;
      }
      else if ('$' != sValeur[0])
      {
        erreur("Valeur chiffrée incorrecte dans un chemin de recherche.", standardError, 0, GetHandle()) ;
        sLibelle = sCode ;
        return ;
      }

      // cas x < a
      if ((sValeur.find('<') != NPOS) && (sValeur.find("<<") == NPOS) &&
          (sValeur.find("<[") == NPOS) && (sValeur.find("[<") == NPOS))
      {
        pos3 = sValeur.find('<') ;

        sValBrut = string(sValeur, pos3+1, strlen(sValeur.c_str())-pos3-1) ;
        pos = sValBrut.find_first_not_of('0');
        if (NPOS != pos)
          sVal2 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos) ;
        else
          sVal2 = sValBrut ;

        sVal = "[< " + sVal2 + "]" ;
      }
      // cas x <= a
      else if ((sValeur.find('[') != NPOS) && (sValeur.find("[[") == NPOS) &&
               (sValeur.find("[<") == NPOS) && (sValeur.find("<[") == NPOS))
      {
        pos3 = sValeur.find('[') ;

        sValBrut = string(sValeur, pos3+1, strlen(sValeur.c_str())-pos3-1) ;
        pos = sValBrut.find_first_not_of('0') ;
        if (NPOS != pos)
          sVal2 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos) ;
        else
          sVal2 = sValBrut ;

        sVal = "[<= " + sVal2 + "]" ;
      }
          // cas x > a
          else if (sValeur.find('>') != NPOS)
          {
              pos3 = sValeur.find('>');

              sValBrut = string(sValeur, pos3+1, strlen(sValeur.c_str())-pos3-1);
              pos = sValBrut.find_first_not_of('0');
              if (pos != NPOS)
                  sVal2 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos);
              else
                  sVal2 = sValBrut;

              sVal = "[> " + sVal2 + "]";
          }
          // cas x >= a
          else if (sValeur.find(']') != NPOS)
          {
              pos3 = sValeur.find(']');

              sValBrut = string(sValeur, pos3+1, strlen(sValeur.c_str())-pos3-1);
              pos = sValBrut.find_first_not_of('0');
              if (pos != NPOS)
                  sVal2 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos);
              else
                  sVal2 = sValBrut;

              sVal = "[>= " + sVal2 + "]";
          }
          // cas x entre a et b
          else if ((sValeur.find("<<") != NPOS) || (sValeur.find("<[") != NPOS) ||
                   (sValeur.find("[<") != NPOS) || (sValeur.find("[[") != NPOS))
          {
              string sBorne1, sBorne2;

              // cas a < x < b
              if ((pos3 = sValeur.find("<<")) != NPOS)
              {
                  sBorne1 = "]";
                  sBorne2 = "[";
              }
              // cas a < x <= b
              else if ((pos3 = sValeur.find("<[")) != NPOS)
              {
                  sBorne1 = "]";
                  sBorne2 = "]";
              }
              // cas a <= x < b
              else if ((pos3 = sValeur.find("[<")) != NPOS)
              {
                  sBorne1 = "[";
                  sBorne2 = "[";
              }
              // cas a <= x <= b
              else if ((pos3 = sValeur.find("[[")) != NPOS)
              {
                  sBorne1 = "[";
                  sBorne2 = "]";
              }

              // on extrait les deux valeurs brutes
              sValBrut = string(sValeur, 1, pos3-1);
              pos = sValBrut.find_first_not_of('0');
              if (pos != NPOS)
                  sVal1 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos);
              else
                  sVal1 = sValBrut;

              sValBrut = string(sValeur, pos3+2, strlen(sValeur.c_str())-pos3-2);
              pos = sValBrut.find_first_not_of('0');
              if (pos != NPOS)
                  sVal2 = string(sValBrut, pos, strlen(sValBrut.c_str())-pos);
              else
                  sVal2 = sValBrut;

              sVal = sBorne1 + sVal1 + " ; " + sVal2 + sBorne2;
          }
          else // valeur exacte
          {
              sValBrut = string(sValeur, 1, strlen(sValeur.c_str())-1);
              pos = sValBrut.find_first_not_of('0');
              if (pos != NPOS)
                  sVal = string(sValBrut, pos, strlen(sValBrut.c_str())-pos);
              else
                  sVal = sValBrut;
          }

          sLibelle = sVal + " " + sLibUnite;
      }
      else // cas des valeurs fils (considérées non instanciées)
      {
          if (sFormat[1] == 'N')
          {
              sLibelle = "[Valeur";
              if (sLibUnite != "")
                  sLibelle += " en " + sLibUnite;
              sLibelle += "]";
          }
          else if ((sFormat[1] == 'D') || (sFormat[1] == 'T'))
          {
              sLibelle = "[Date";
              if (sLibUnite != "")
                  sLibelle += " (" + sLibUnite + ")";
              sLibelle += "]";
          }
          else if (sFormat[1] == 'H')
          {
              sLibelle = "[Heure";
              if (sLibUnite != "")
                  sLibelle += " (" + sLibUnite + ")";
              sLibelle += "]";
          }
          else
          {
              erreur("Valeur chiffrée avec un format non traité.", standardError, 0, GetHandle()) ;
              sLibelle = sCode;
          }
      }
  }
  else
  {
    erreur("Valeur chiffrée sans format.", standardError, 0, GetHandle()) ;
    sLibelle = sCode ;
  }
}

// Cette fonction traite aussi le décalage// pour les fils, on utilise la valeur de sDecal
// pour les pères, le décalage est contenu dans les éléments du chemin
void
NSEditReqDialog::ParsingChemin(string sChem, bool bPere, string sDecal)
{
	string sUnitesDistinctes = "" ;  // pour traiter les fils valeurs chiffrées
	char   sep = searchPathSeparator ;  // séparateur de blocs pour les pères et les fils

	// bool bVide = true;
	size_t pos1 = 0 ;	size_t pos2 = sChem.find(sep) ;

	bool bParsing = true ;
	while (bParsing)	{
		string sCode = ""  ;
    string sLibelle = "" ;

    if (NPOS != pos2)
    	sCode = string(sChem, pos1, pos2-pos1) ;
    else
    	sCode = string(sChem, pos1, strlen(sChem.c_str())-pos1) ;

    if (bPere)
    {
    	// cas d'un code père : on doit aussi extraire le décalage
      size_t pos3 = sCode.find(':') ;  // Note : le ':' est le séparateur de décalage
      if (NPOS != pos3)
      {
      	// pour éviter les cas d'erreur on met decal à '1' par défaut
        char decal = '1' ;

        if (strlen(sCode.c_str()) == (pos3 + 2))
        	decal = sCode[pos3 + 1] ;

        if ('0' == decal)
        	sDecal = "+00+00" ;
        else
        	sDecal = "+01+01" ;

        // on remet le code avec le code sens uniquement (sans le décalage)
        sCode = string(sCode, 0, pos3) ;
      }
    }

    // on retrouve le libellé correspondant
    if (bPere)
    {
    	if ((string("") != sCode) && ('2' == sCode[0]))
      	TrouveLibelleValeur(sCode, sLibelle, bPere) ;
      else
      	TrouveLibelle(sCode, sLibelle, bPere) ;

      pPereArray->push_back(new NSElement(sCode, sLibelle, sDecal)) ;
      nbPere++ ;
    }
    else
    {
    	if ((string("") != sCode) && ('2' == sCode[0]))
      {
      	// cas des fils valeurs chiffrées : on extrait l'unité
        string sUnite ;
        size_t pos = sCode.find(searchNodeSeparator) ;
        if (NPOS != pos)
        	sUnite = string(sCode, 0, pos) ;
        else
        	sUnite = sCode ;

        // si l'unité existe déjà, on ne traite pas l'élément
        if (sUnitesDistinctes.find(sUnite) == NPOS)
        {
        	// cas d'une nouvelle unité : on ajoute aux fils
          sUnitesDistinctes += sUnite + string(1, searchPathSeparator) ;
          TrouveLibelleValeur(sCode, sLibelle, bPere) ;
          pFilsArray->push_back(new NSElement(sCode, sLibelle, sDecal)) ;
          nbFils++ ;
        }
      }
      else
      {
      	TrouveLibelle(sCode, sLibelle, bPere) ;
        pFilsArray->push_back(new NSElement(sCode, sLibelle, sDecal)) ;
        nbFils++ ;
      }
    }

    // condition d'arret : on traite aussi le cas du '|' à la fin du chemin
    if ((NPOS == pos2) || (strlen(sChem.c_str()) - 1 == pos2))
    	bParsing = false ;
    else
    {
    	pos1 = pos2+1 ;
      pos2 = sChem.find(sep, pos1) ;
    }
  }
}

// Constitution d'un chemin de recherche :// Fonction qui reconstitue un chemin de codes sens à partir d'un array d'éléments
// Si bRequete == true, la fonction fait un chemin d'éléments (ensemble de blocs
// composés de un ou plusieurs codes lexique) - Dans les deux cas, le décalage est
// pris en compte : les éléments en +00+00 écrasent leur père. Le décalage n'est pas
// en revanche contenu dans le chemin comme dans ::ComposeCheminBrut
void
NSEditReqDialog::ComposeChemin(string& sChemin, NSEltArray* pArray, bool bRequete)
{
  string sBoutChemin;
  string sCodeCompose;
  string sCodeLexique;
  string sCodeUtile;

  bool bContinue;
  sChemin = string("") ;

  if (pArray && (false == pArray->empty()))
  {
    // On reconstitue la string sChemin à partir de l'array source
    for (NSEltIter i = pArray->begin(); pArray->end() != i ; i++)
    {
      // chaque code élément est éventuellement composé d'une certitude et d'un pluriel
      string sCodeCompose = (*i)->getCode() ;

      if (bRequete)
        sBoutChemin = sCodeCompose;      else
      {
        // cas des chemins de recherche "Fil Guides" : on constitue sBoutChemin
        // à partir des codes sens de l'élément (sCodeCompose)
        //
        sBoutChemin = string("") ;
        bContinue = true ;
        size_t pos1 = 0 ;
        size_t pos2 = sCodeCompose.find(searchNodeSeparator) ;

        while (bContinue)        {          if (NPOS != pos2)
            sCodeLexique = string(sCodeCompose, pos1, pos2 - pos1) ;
          else
            sCodeLexique = string(sCodeCompose, pos1, strlen(sCodeCompose.c_str())-pos1) ;

          NSDico::donneCodeSens(&sCodeLexique, &sCodeUtile) ;
          if (string("") != sBoutChemin)
            sBoutChemin += string(1, searchNodeSeparator) ;
          sBoutChemin += sCodeUtile ;

          // condition d'arret : on traite aussi le cas du '/' à la fin du chemin
          if ((NPOS == pos2) || (strlen(sCodeCompose.c_str()) - 1 == pos2))
            bContinue = false ;
          else
          {
            pos1 = pos2+1 ;
            pos2 = sCodeCompose.find(searchNodeSeparator, pos1) ;
          }
        }
      }

      // on ajoute chaque boutchemin (élément) au chemin global (ensemble des éléments)
      // on écrase alors le père (élément précédent) si l'élément courant est en +00+00
      if (string("+00+00") == (*i)->getDecal())
      {
        int nbCarsEltPere ;
        size_t pos3 = sChemin.find_last_of(searchPathSeparator) ;

        if (NPOS != pos3)
        {
          // l'élément fils (courant) écrase l'élément pere (précédent)
          nbCarsEltPere = strlen(sChemin.c_str()) - pos3 - 1 ;
          sChemin.replace(pos3 + 1, nbCarsEltPere, sBoutChemin) ;
        }
        else    // cas d'un élément +00+00 en premier ou en second (écrase le premier)
          sChemin = sBoutChemin ;
      }
      else // cas normal (+01+01)
      {
        if (string("") != sChemin)
          sChemin += string(1, searchPathSeparator) ;
        sChemin += sBoutChemin ;
      }
    }
  }

  // cas des chemins "Fil Guide" : on remplace maintenant tous les '|' par des '/'
  // pour pouvoir chercher un chemin dans la base des guides
  if (false == bRequete)
  {
    size_t pos = sChemin.find(searchPathSeparator) ;
    while (NPOS != pos)
    {
      sChemin.replace(pos, 1, string(1, cheminSeparationMARK)) ;
      pos = sChemin.find(searchPathSeparator, pos+1) ;
    }
  }
}

// cas du chemin brut : pour garder toutes les infos dans une string// les éléments sont des blocs de codes lexiques avec un décalage (:0 ou :1)
void
NSEditReqDialog::ComposeCheminBrut(string& sChemin, NSEltArray* pArray)
{
  string sCodeCompose ;

  sChemin = string("") ;

  if (pArray->empty())
    return ;

  // On reconstitue la string sChemin à partir de l'array source
  for (NSEltIter i = pArray->begin(); pArray->end() != i ; i++)
  {
    // chaque code élément est éventuellement composé d'une certitude et d'un pluriel
    sCodeCompose = (*i)->getCode() ;

    // on ajoute chaque boutchemin (élément) au chemin global (ensemble des éléments)
    // on concatène la valeur du décalage
    if (string("+00+00") == (*i)->getDecal())
      sCodeCompose += ":0" ;
    else // cas normal (+01+01)
      sCodeCompose += ":1" ;

    if (string("") != sChemin)
      sChemin += string(1, searchPathSeparator) ;
    sChemin += sCodeCompose ;
  }
}

boolNSEditReqDialog::CalculeValeur(string& sCode)
{
  string sLang = string("") ;
  if ((pContexte) && (pContexte->getUtilisateur()))
    sLang = pContexte->getUtilisateur()->donneLang() ;

  string  sValeur;
  gereNum num = gereNum(pContexte->getSuperviseur(), sLang);
  string  sUnit = "", sFmt = ""; // valeurs bidon pour pouvoir instancier le gereNum

  NSValeurChiffreeDialog VCDlg(this, 255, pContexte, pNSResModule) ;

  // on transmet l'unité et la valeur donnée
  size_t pos1 = sCode.find(searchNodeSeparator) ;
  if (NPOS != pos1)
  {
    VCDlg.sUnite = string(sCode, 0, pos1) ;
    size_t pos2 = sCode.find(searchNodeSeparator, pos1+1) ;
    if (NPOS != pos2)
    {
      if (strlen(sCode.c_str()) > (pos2 + 2))
      {
        // car on doit au moins avoir "$x" comme valeur
        sValeur = string(sCode, pos2+2, strlen(sCode.c_str())-pos2-2) ;
        num.instancier(&sValeur, &sUnit, &sFmt) ;

        // on instancie la valeur
        if (num.estExact())
        {
          VCDlg.sValeurExacte = num.getNum() ;
        }
        else
        {
          if (num.estInf())          {
            VCDlg.sValeurInf = num.getNumInf() ;
            if (num.estInfEgal())
              VCDlg.bAvecInf = true ;
          }

          if (num.estSup())
          {
            VCDlg.sValeurSup = num.getNumSup() ;
            if (num.estSupEgal())
              VCDlg.bAvecSup = true ;
          }
        }
      }
      else // cas valeur bidon
        erreur("Code comportant une valeur incorrecte.", standardError, 0, GetHandle()) ;
    }
  }
  else // normalement cas d'erreur
    VCDlg.sUnite = sCode ;

  if (VCDlg.Execute() == IDCANCEL)
    return false ;

  // on remplace l'unité et on enlève les éventuelles valeurs ($XXXXX) du code
  if (NPOS != pos1)
  {
    sCode.replace(0, pos1, VCDlg.sUnite) ;
    // on garde le format sans la valeur
    size_t pos2 = sCode.find(searchNodeSeparator, pos1+1) ;
    if (NPOS != pos2)
      sCode = string(sCode, 0, pos2) ;
  }
  else // cas d'erreur : on met un format bidon
    sCode = VCDlg.sUnite + "£NXXXX" ;

  // on rajoute la valeur au code
  if (string("") != VCDlg.sValeurExacte)
  {
    sCode += string(1, searchNodeSeparator) + string("$") + VCDlg.sValeurExacte ; // x == a
  }
  else if ((string("") != VCDlg.sValeurInf) && (string("") == VCDlg.sValeurSup))
  {
    if (VCDlg.bAvecInf)
      sCode += string(1, searchNodeSeparator) + string("$]") + VCDlg.sValeurInf ; // x >= a
    else
      sCode += string(1, searchNodeSeparator) + string("$>") + VCDlg.sValeurInf ; // x > a
  }
  else if ((string("") == VCDlg.sValeurInf) && (string("") != VCDlg.sValeurSup))
  {
    if (VCDlg.bAvecSup)
      sCode += string(1, searchNodeSeparator) + string("$[") + VCDlg.sValeurSup ; // x <= a
    else
      sCode += string(1, searchNodeSeparator) + string("$<") + VCDlg.sValeurSup ;    // x < a
  }
  else // cas des intervalles avec deux bornes
  {
    if ((VCDlg.bAvecInf) && (VCDlg.bAvecSup))   // a <= x <= b
      sCode += string(1, searchNodeSeparator) + string("$") + VCDlg.sValeurInf + string("[[") + VCDlg.sValeurSup ;
    else if (VCDlg.bAvecInf)                      // a <= x < b
      sCode += string(1, searchNodeSeparator) + string("$") + VCDlg.sValeurInf + string("[<") + VCDlg.sValeurSup ;
    else if (VCDlg.bAvecSup)                      // a < x <= b
      sCode += string(1, searchNodeSeparator) + string("$") + VCDlg.sValeurInf + string("<[") + VCDlg.sValeurSup ;
    else                                            // a < x < b
      sCode += string(1, searchNodeSeparator) + string("$") + VCDlg.sValeurInf + string("<<") + VCDlg.sValeurSup ;
  }

  return true ;
}

// ************** Controlesvoid
NSEditReqDialog::CmInserer()
{
  // Insertion en queue d'un nouvel élément
  //
  NSModifFilGuideDialog ModDlg(this, "", pContexte, pNSResModule) ;

  if (ModDlg.Execute() != IDOK)
    return ;

  string sCode = ModDlg.sCode ;
  string sLibelle;

  TrouveLibelle(sCode, sLibelle, true) ;

  pPereArray->push_back(new NSElement(sCode, sLibelle)) ;
  nbPere++ ;
  AfficheListePere() ;
}

voidNSEditReqDialog::CmModifier()
{
  int eltChoisi = pListePere->IndexItemSelect() ;

  if (-1 == eltChoisi)
  {
    erreur("Vous devez sélectionner un élément à modifier.", standardError, 0) ;
    return ;
  }

  NSElement element = *((*pPereArray)[eltChoisi]) ;

  string sElmtCode = element.getCode() ;

  if ((string("") != sElmtCode) && ('2' == sElmtCode[0]))
  {
    // cas de la modification d'une valeur chiffrée
    if (CalculeValeur(sElmtCode))
    {
      // on recalcule le nouveau libelle avec bPere == true
      //
      string sElmtLabel = element.getLabel() ;
      TrouveLibelleValeur(sElmtCode, sElmtLabel, true) ;

      // on replace l'élément
      *((*pPereArray)[eltChoisi]) = element ;
      AfficheListePere() ;
    }
  }
  else
  {
    NSModifFilGuideDialog* pModDlg =
            new NSModifFilGuideDialog(this, sElmtCode, pContexte, pNSResModule) ;

    if (pModDlg->Execute() == IDOK)
    {
      string sCode = pModDlg->sCode ;

      string sLibelle = string("") ;
      TrouveLibelle(sCode, sLibelle, true) ;

      element.setCode(sCode) ;
      element.setLabel(sLibelle) ;
      *((*pPereArray)[eltChoisi]) = element ;

      AfficheListePere() ;
    }
    delete pModDlg ;
  }
}

voidNSEditReqDialog::CmDetruire()
{
  int eltChoisi = pListePere->IndexItemSelect() ;
  if (-1 == eltChoisi == -1)
  {
    erreur("Vous devez sélectionner un élément à détruire.", standardError, 0) ;
    return ;
  }

  if (false == pPereArray->empty())
  {
    int j = 0 ;
    for (NSEltIter i = pPereArray->begin() ; pPereArray->end() != i ; i++, j++)
    {
      if (j == eltChoisi)
      {
        delete *i ;
        pPereArray->erase(i) ;
        nbPere-- ;
        break ;
      }
    }
  }

  AfficheListePere() ;
}

voidNSEditReqDialog::CmAjouter()
{
	int eltChoisi = pListeFils->IndexItemSelect() ;
	if (-1 == eltChoisi)
	{
		erreur("Vous devez sélectionner un fils à ajouter.", standardError, 0) ;
    return ;
	}

	string sCodeFils  = ((*pFilsArray)[eltChoisi])->getCode() ;
	string sDecalFils = ((*pFilsArray)[eltChoisi])->getDecal() ;
	string sLibFils ;

	if ((string("") != sCodeFils) && ('2' == sCodeFils[0]))	{
		// cas de l'ajout d'une valeur chiffrée : on doit instancier la valeur
    if (CalculeValeur(sCodeFils))
    {
    	// on recalcule le nouveau libelle avec bPere == true
      TrouveLibelleValeur(sCodeFils, sLibFils, true) ;

      // on push_back l'élément
      NSElement eltValeur ;
      eltValeur.setCode(sCodeFils) ;
      eltValeur.setLabel(sLibFils) ;
      eltValeur.setDecal(sDecalFils) ;
      pPereArray->push_back(new NSElement(eltValeur)) ;
      nbPere++ ;
      AfficheListePere() ;
    }
  }
	else
	{
  	pPereArray->push_back(new NSElement(*((*pFilsArray)[eltChoisi]))) ;
    nbPere++ ;
    AfficheListePere() ;
	}
}

voidNSEditReqDialog::CmOk()
{
  string sFils = string("") ;

  // On reconstitue le chemin pere final
  ComposeCheminBrut(sCheminPere, pPereArray) ;

  // On reconstitue également le chemin de recherche équivalent
  ComposeChemin(sCheminRechPere, pPereArray, true) ;

  // On constitue la string sFils à partir de l'array Fils
  if (false == pFilsArray->empty())
  {
    for (NSEltIter i = pFilsArray->begin() ; pFilsArray->end() != i ; i++)
    {
      if (string("") != sFils)
        sFils += string(1, searchPathSeparator) ;
      sFils += (*i)->getCode() ;
    }
  }

  TDialog::CmOk() ;
}

voidNSEditReqDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

/*****************************************************************************//*                      GESTION DES CRITERES DE REQUETES                     */
/*****************************************************************************/

NSCritReqPatient::NSCritReqPatient(){
  metAZero() ;
}

NSCritReqPatient::NSCritReqPatient(const NSCritReqPatient& rv)
{
  metAZero() ;

  sNom      = rv.sNom ;
  sPrenom   = rv.sPrenom ;
  sSexe     = rv.sSexe ;
  sDateN1   = rv.sDateN1 ;
  sDateN2   = rv.sDateN2 ;
  sSitfam   = rv.sSitfam ;
  sCodePost = rv.sCodePost ;
  sVille    = rv.sVille ;

  initRegExp() ;
}

NSCritReqPatient::~NSCritReqPatient(){
}

void
NSCritReqPatient::reset()
{
  sNom      = string("") ;
  sPrenom   = string("") ;
  sSexe     = string("") ;
  sDateN1   = string("") ;
  sDateN2   = string("") ;
  sSitfam   = string("") ;
  sCodePost = string("") ;
  sVille    = string("") ;

  resetRegExp() ;
}

void
NSCritReqPatient::resetRegExp()
{
  if (pNameRegexp)
    delete pNameRegexp ;
  pNameRegexp = (TRegexp*) 0 ;

  if (pScndNameRegexp)
    delete pScndNameRegexp ;
  pScndNameRegexp = (TRegexp*) 0 ;

  if (pZipCodeRegexp)
    delete pZipCodeRegexp ;
  pZipCodeRegexp = (TRegexp*) 0 ;

  if (pTownRegexp)
    delete pTownRegexp ;
  pTownRegexp = (TRegexp*) 0 ;
}

void
NSCritReqPatient::initRegExp()
{
  resetRegExp() ;

  string sRegExp = getRegExp(sNom) ;
  if (string("") != sRegExp)
    pNameRegexp = new TRegexp(sRegExp.c_str()) ;

  sRegExp = getRegExp(sPrenom) ;
  if (string("") != sRegExp)
    pScndNameRegexp = new TRegexp(sRegExp.c_str()) ;

  sRegExp = getRegExp(sCodePost) ;
  if (string("") != sRegExp)
    pZipCodeRegexp = new TRegexp(sRegExp.c_str()) ;

  sRegExp = getRegExp(sVille) ;
  if (string("") != sRegExp)
    pTownRegexp = new TRegexp(sRegExp.c_str()) ;
}

voidNSCritReqPatient::metAZero()
{
  sNom      = string("") ;
  sPrenom   = string("") ;
  sSexe     = string("") ;
  sDateN1   = string("") ;
  sDateN2   = string("") ;
  sSitfam   = string("") ;
  sCodePost = string("") ;
  sVille    = string("") ;

  pNameRegexp     = (TRegexp*) 0 ;
  pScndNameRegexp = (TRegexp*) 0 ;
  pZipCodeRegexp  = (TRegexp*) 0 ;
  pTownRegexp     = (TRegexp*) 0 ;
}

string
NSCritReqPatient::getRegExp(string sInfo)
{
  if (string("") == sInfo)
    return string("") ;

  size_t iLen = strlen(sInfo.c_str()) ;

  if ((iLen < 3) || (string("$$") != string(sInfo, 0, 2)))
    return string("") ;

  return string(sInfo, 2, iLen - 2) ;
}

NSCritReqPatient&NSCritReqPatient::operator=(const NSCritReqPatient& src)
{
  if (this == &src)
		return *this ;

  reset() ;

  sNom      = src.sNom ;
  sPrenom   = src.sPrenom ;
  sSexe     = src.sSexe ;
  sDateN1   = src.sDateN1 ;
  sDateN2   = src.sDateN2 ;
  sSitfam   = src.sSitfam ;
  sCodePost = src.sCodePost ;
  sVille    = src.sVille ;

  initRegExp() ;

	return *this ;
}

bool
NSCritReqPatient::isValidNom(const string &sN)
{
  if (string("") == sNom)
    return true ;

  if ((TRegexp*) NULL == pNameRegexp)
    return (sN == sNom) ;

  size_t iLen ;
  size_t iPos = pNameRegexp->find(sN.c_str(), &iLen) ;

  return (-1 != iPos) ;
}

bool
NSCritReqPatient::isValidPrenom(const string &sN)
{
  if (string("") == sPrenom)
    return true ;

  if ((TRegexp*) NULL == pScndNameRegexp)
    return (sN == sPrenom) ;

  size_t iLen ;
  size_t iPos = pScndNameRegexp->find(sN.c_str(), &iLen) ;

  return (-1 != iPos) ;
}

bool
NSCritReqPatient::isValidSexe(const string &sN)
{
  if (string("") == sSexe)
    return true ;

  return (sN == sSexe) ;
}

bool
NSCritReqPatient::isValidDate(const string &sN)
{
  if ((string("") == sDateN1) && (string("") == sDateN2))
    return true ;

  NVLdVTemps tpsBirthDate ;
  tpsBirthDate.initFromDate(sN) ;  if (string("") != sDateN1)  {    NVLdVTemps tpsDateN1 ;    tpsDateN1.initFromDate(sDateN1) ;    if (tpsBirthDate < tpsDateN1)
      return false ;
  }
  if (string("") != sDateN2)  {
    NVLdVTemps tpsDateN2 ;
    tpsDateN2.initFromDate(sDateN2) ;
    if (tpsBirthDate > tpsDateN2)
      return false ;
  }

  return true ;
}

bool
NSCritReqPatient::isValidSitfam(const string &sN)
{
  if ((string("") == sSitfam) || (string("I") == sSitfam))
    return true ;

  return (sN == sSitfam) ;
}

bool
NSCritReqPatient::isValidCodePost(const string &sN)
{
  return true ;
}

bool
NSCritReqPatient::isValidVille(const string &sN)
{
  return true ;
}

//// Dialogue de saisie des critères Patient
//

DEFINE_RESPONSE_TABLE1(NSCritReqPatDialog, NSUtilDialog)  EV_WM_CLOSE,
  EV_BN_CLICKED(IDC_CRITREQPAT_SEXE1, CmSexe),
  EV_BN_CLICKED(IDC_CRITREQPAT_SEXE2, CmSexe),
  EV_BN_CLICKED(IDC_CRITREQPAT_CEL, CmSitFam),
  EV_BN_CLICKED(IDC_CRITREQPAT_VEU, CmSitFam),
  EV_BN_CLICKED(IDC_CRITREQPAT_MAR, CmSitFam),
  EV_BN_CLICKED(IDC_CRITREQPAT_CCB, CmSitFam),
  EV_BN_CLICKED(IDC_CRITREQPAT_DIV, CmSitFam),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(IDC_CRITREQPAT_EFFACER, CmEffacer),
END_RESPONSE_TABLE;

NSCritReqPatDialog::NSCritReqPatDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)                   :NSUtilDialog(pere, pCtx, "IDD_CRITREQPAT", mod)
{
  // Initialisation des donnees
  pData 		= new NSCritReqPatient ;
  bEffacer  = false ;

  // Création de tous les "objets de contrôle"  pSexe		  = new TGroupBox(this, IDC_CRITREQPAT_SEXE) ;
  iSexe     = 0 ;
  pSitFam		= new TGroupBox(this, IDC_CRITREQPAT_SITFAM) ;
  iSitFam   = 0 ;

	pNom 	    = new NSUtilEdit(pContexte, this, IDC_CRITREQPAT_NOM, PAT_NOM_LEN) ;
	pPrenom   = new NSUtilEdit(pContexte, this, IDC_CRITREQPAT_PRENOM, PAT_PRENOM_LEN) ;
  pSexeM		= new TRadioButton(this, IDC_CRITREQPAT_SEXE1, pSexe) ;
  pSexeF    = new TRadioButton(this, IDC_CRITREQPAT_SEXE2, pSexe) ;

  // par défaut la date de naissance reste en 00/00/1900 (b2000 == false)
  pDateN1		= new NSUtilEditDate(pContexte, this, IDC_CRITREQPAT_DATEN1, 10, false) ;
  pDateN2		= new NSUtilEditDate(pContexte, this, IDC_CRITREQPAT_DATEN2, 10, false) ;

  pCodePost	= new NSUtilEdit(pContexte, this, IDC_CRITREQPAT_CODEPOST, ADR_CODE_POST_LEN) ;
  pVille    = new NSUtilEdit(pContexte, this, IDC_CRITREQPAT_VILLE, ADR_VILLE_LEN) ;

  pCel		  = new TRadioButton(this, IDC_CRITREQPAT_CEL, pSitFam) ;
  pVeu		  = new TRadioButton(this, IDC_CRITREQPAT_VEU, pSitFam) ;
  pMar		  = new TRadioButton(this, IDC_CRITREQPAT_MAR, pSitFam) ;
  pCcb		  = new TRadioButton(this, IDC_CRITREQPAT_CCB, pSitFam) ;
  pDiv		  = new TRadioButton(this, IDC_CRITREQPAT_DIV, pSitFam) ;
  pNbEnf		= new NSEditNum(pContexte, this, IDC_CRITREQPAT_NBENF, 2, 0) ;

  sHindex = string("") ;
  sHcorps = string("h_rmc_critpat.html") ;
}

NSCritReqPatDialog::~NSCritReqPatDialog(){
  delete pNbEnf;
  delete pDiv;
  delete pCcb;
  delete pMar;
  delete pVeu;
  delete pCel;
	delete pVille;
  delete pCodePost;
  delete pDateN2;
  delete pDateN1;
  delete pSexeF;
  delete pSexeM;
  delete pPrenom;
  delete pNom;
  delete pSitFam;
  delete pSexe;
  delete pData;
}

voidNSCritReqPatDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  pNom->SetText(pData->getNom().c_str()) ;
  pPrenom->SetText(pData->getPrenom().c_str()) ;

  if (string("") != pData->getSexe())
  {
    if (pData->estMasculin())
    {
      pSexeM->Check() ;
      iSexe = 1 ;
    }
    if (pData->estFeminin())
    {
      pSexeF->Check() ;
      iSexe = 2 ;
    }
  }

  pDateN1->setDate(pData->getDateN1()) ;
  pDateN2->setDate(pData->getDateN2()) ;
  pCodePost->SetText(pData->getCodePost()) ;
  pVille->SetText(pData->getVille()) ;

  string sSitfam = pData->getSitfam() ;

  if (string("") != sSitfam)
    switch(sSitfam[0])
    {
      case 'C':
          pCel->Check();
          iSitFam = 1;
          break;
      case 'V':
          pVeu->Check();
          iSitFam = 2;
          break;
      case 'M':
          pMar->Check();
          iSitFam = 3;
          break;
      case 'B':
          pCcb->Check();
          iSitFam = 4;
          break;
      case 'D':
          pDiv->Check();
          iSitFam = 5;
          break;
    }

  if (strlen(sSitfam.c_str()) > 1)
  {
    string sNbEnf = string(sSitfam, 1, 1) ;
    pNbEnf->SetText(sNbEnf.c_str()) ;
  }
  else
    pNbEnf->SetText("") ;
}

// méthode qui marche avec des autoradiobutton type borlandvoid
NSCritReqPatDialog::CmSexe()
{
  if (pSexeM->GetCheck() == BF_CHECKED)
  {
    if (iSexe == 1)
    {
      pSexeM->Uncheck() ;
      iSexe = 0 ;
    }
    else
      iSexe = 1 ;
  }

  if (pSexeF->GetCheck() == BF_CHECKED)
  {
    if (iSexe == 2)
    {
      pSexeF->Uncheck() ;
      iSexe = 0 ;
    }
    else
      iSexe = 2 ;
  }
}

// méthode qui marche avec des autoradiobutton type borlandvoid
NSCritReqPatDialog::CmSitFam()
{
  if (pCel->GetCheck() == BF_CHECKED)
  {
    if (iSitFam == 1)
    {
      pCel->Uncheck() ;
      iSitFam = 0 ;
    }
    else
      iSitFam = 1 ;
  }

  if (pVeu->GetCheck() == BF_CHECKED)
  {
    if (iSitFam == 2)
    {
      pVeu->Uncheck() ;
      iSitFam = 0 ;
    }
    else
      iSitFam = 2 ;
  }

  if (pMar->GetCheck() == BF_CHECKED)
  {
    if (iSitFam == 3)
    {
      pMar->Uncheck() ;
      iSitFam = 0 ;
    }
    else
      iSitFam = 3 ;
  }

  if (pCcb->GetCheck() == BF_CHECKED)
  {
    if (iSitFam == 4)
    {
      pCcb->Uncheck() ;
      iSitFam = 0 ;
    }
    else
      iSitFam = 4 ;
  }

  if (pDiv->GetCheck() == BF_CHECKED)
  {
    if (iSitFam == 5)
    {
      pDiv->Uncheck() ;
      iSitFam = 0 ;
    }
    else
      iSitFam = 5 ;
  }
}

voidNSCritReqPatDialog::CmEffacer()
{
  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int idRet = MessageBox("Etes-vous sûrs de vouloir effacer tous les critères Patient ?", sCaption.c_str(), MB_YESNO) ;
  if (IDNO == idRet)
    return ;

  bEffacer = true ;
  pData->metAZero() ;
  TDialog::CmOk() ;
}

voidNSCritReqPatDialog::CmOk()
{
  string sNom = pNom->GetText(PAT_NOM_LEN) ;
  strip(sNom) ;
  pData->setNom(sNom) ;

  string sPrenom = pPrenom->GetText(PAT_PRENOM_LEN) ;
  strip(sPrenom) ;
  pData->setPrenom(sPrenom) ;

  if (pSexeM->GetCheck() == BF_CHECKED)
    pData->setSexe("1") ;

  if (pSexeF->GetCheck() == BF_CHECKED)
    pData->setSexe("2") ;

  string sDateN1;
  pDateN1->getDate(&sDateN1);
  pData->setDateN1(sDateN1) ;

  string sDateN2;
  pDateN2->getDate(&sDateN2);
  pData->setDateN2(sDateN2) ;

  string sCodePost = pCodePost->GetText(ADR_CODE_POST_LEN) ;
  pData->setCodePost(sCodePost) ;

  string sVille = pVille->GetText(ADR_VILLE_LEN) ;
  pData->setVille(sVille) ;

  pData->setSitfam("I") ;     // cf nombre d'enfants

  if (pCel->GetCheck() == BF_CHECKED)
    pData->setSitfam("C") ;

  if (pVeu->GetCheck() == BF_CHECKED)
    pData->setSitfam("V");

  if (pMar->GetCheck() == BF_CHECKED)
    pData->setSitfam("M");

  if (pCcb->GetCheck() == BF_CHECKED)
    pData->setSitfam("B");

  if (pDiv->GetCheck() == BF_CHECKED)
    pData->setSitfam("D");

  string sNbEnf = pNbEnf->GetText(3) ;
  if (strlen(sNbEnf.c_str()) > 1)              // cas non prévu !!!
    sNbEnf = string("A") ;

  pData->setSitfam(pData->getSitfam() + sNbEnf) ;

  TDialog::CmOk() ;
}

voidNSCritReqPatDialog::CmCancel()
{
  TDialog::CmCancel() ;
}


void
NSCritReqPatDialog::EvClose()
{
    CmCancel();
}

//
// Classe de Critères Document
//
NSCritReqDocum::NSCritReqDocum()
{
    metAZero();
}

NSCritReqDocum::~NSCritReqDocum()
{
}

void
NSCritReqDocum::metAZero()
{
    sCodeAuteur = "";
    sTitreAuteur = "";
    sCodeRoot = "";
    sDate1 = "";
    sDate2 = "";
}
NSCritReqDocum&NSCritReqDocum::operator=(const NSCritReqDocum& src)
{
  if (this == &src)
		return *this ;

  sCodeAuteur  = src.sCodeAuteur ;
  sTitreAuteur = src.sTitreAuteur ;
  sCodeRoot    = src.sCodeRoot ;
  sDate1       = src.sDate1 ;
  sDate2       = src.sDate2 ;

	return *this ;
}

//// Dialogue de saisie des critères Document
//
DEFINE_RESPONSE_TABLE1(NSCritReqDocDialog, NSUtilDialog)
    EV_WM_CLOSE,
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(IDC_CRITREQDOC_AUTEUR, CmAuteur),
    EV_COMMAND(IDC_CRITREQDOC_EFFACER, CmEffacer),
END_RESPONSE_TABLE;

NSCritReqDocDialog::NSCritReqDocDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)
                    :NSUtilDialog(pere, pCtx, "IDD_CRITREQDOC", mod)
{
  // Initialisation des donnees
  pData     = new NSCritReqDocum ;
  bEffacer  = false ;

	// Création de tous les "objets de contrôle"
  pAuteur   = new TStatic(this, IDC_CRITREQDOC_AUTEUR) ;
	pCodeRoot = new NSUtilLexique(pContexte, this, IDC_CRITREQDOC_TYPE, pContexte->getDico()) ;

  // par défaut la date document est en 00/00/2000 (b2000 == true)
  pDate1    = new NSUtilEditDate(pContexte, this, IDC_CRITREQDOC_DATE1, 10, true);
  pDate2    = new NSUtilEditDate(pContexte, this, IDC_CRITREQDOC_DATE2, 10, true);
}

NSCritReqDocDialog::~NSCritReqDocDialog(){
  delete pDate2 ;
  delete pDate1 ;
  delete pCodeRoot ;
  delete pAuteur ;
  delete pData ;
}

void
NSCritReqDocDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  sCodeAuteur = pData->sCodeAuteur ;
  sTitreAuteur = pData->sTitreAuteur ;
  if (sCodeAuteur != "")
    pAuteur->SetCaption(sTitreAuteur.c_str()) ;
  pCodeRoot->setLabel(pData->sCodeRoot) ;
  pDate1->setDate(pData->sDate1) ;
  pDate2->setDate(pData->sDate2) ;
}

voidNSCritReqDocDialog::CmAuteur()
{
/*
	NSReqListUtilDialog* pListeUtilDlg = new NSReqListUtilDialog(this, pContexte, pNSResModule) ;

	if ((pListeUtilDlg->Execute() == IDOK) && (pListeUtilDlg->UtilChoisi >= 0))
	{
		sCodeAuteur = pListeUtilDlg->pUtilSelect->getNss() ;
    sTitreAuteur = pListeUtilDlg->pUtilSelect->donneTitre(pContexte) ;    pAuteur->SetCaption(sTitreAuteur.c_str()) ;
  }

	delete pListeUtilDlg ;
*/

  NSTPersonListDialog Liste(this, pidsUtilisat, false, pContexte) ;
	if ((Liste.Execute() != IDOK) || (NULL == Liste.pPersonSelect))
		return ;

	sCodeAuteur  = Liste.pPersonSelect->getNss() ;
  sTitreAuteur = Liste.pPersonSelect->getNomLong() ;
  pAuteur->SetCaption(sTitreAuteur.c_str()) ;
}

voidNSCritReqDocDialog::CmEffacer()
{
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	int idRet = MessageBox("Etes-vous sûrs de vouloir effacer tous les critères Document ?", sCaption.c_str(), MB_YESNO) ;
	if (IDNO == idRet)
		return ;

	bEffacer = true ;
	pData->metAZero() ;
	TDialog::CmOk() ;
}

voidNSCritReqDocDialog::CmOk()
{
  char far code[255] ;

  pCodeRoot->GetText(code, 255) ;  if (!strcmp(code, ""))
  {
    pData->sCodeRoot = "" ;
  }
  else
  {
    pData->sCodeRoot = pCodeRoot->getCode() ;
    if (pData->sCodeRoot == "£?????")
    {
      erreur("Attention vous ne pouvez pas entrer le type document en texte libre...", standardError, 0) ;
      return ;
    }
  }

  pData->sCodeAuteur = sCodeAuteur ;  pData->sTitreAuteur = sTitreAuteur ;

  pDate1->getDate(&(pData->sDate1)) ;  pDate2->getDate(&(pData->sDate2)) ;

  TDialog::CmOk() ;
}

voidNSCritReqDocDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

voidNSCritReqDocDialog::EvClose()
{
  CmCancel() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSReqListUtilDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSReqListUtilDialog, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_LVN_GETDISPINFO(IDC_LISTUTI_LW, LvnGetDispInfo),
END_RESPONSE_TABLE;

NSReqListUtilDialog::NSReqListUtilDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)                    :NSUtilDialog(pere, pCtx, "IDD_LISTUTI", mod)
{
  pListeUtil  = new NSReqListUtilWindow(this, IDC_LISTUTI_LW) ;
  pUtilArray  = new NSUtiliArray ;
  pUtilSelect = new NSUtiliInfo(pContexte) ;
}

NSReqListUtilDialog::~NSReqListUtilDialog()
{
  delete pUtilSelect ;
  delete pUtilArray ;
  delete pListeUtil ;
}

void
NSReqListUtilDialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  InitUtilArray() ;
  InitListe() ;
  AfficheListe() ;
}

bool
NSReqListUtilDialog::InitUtilArray()
{
	pUtilArray->vider() ;

  NSPersonsAttributesArray AttsList ;
  NSBasicAttributeArray AttrArray ;

  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LOGIN)) ;
  AttrArray.push_back(new NSBasicAttribute(ROLE, USER_ROLE)) ;

  const char* serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;
  bool listOk = pContexte->getSuperviseur()->getPilot()->personList(serviceName, &AttsList, &AttrArray) ;

	nbUtil = 0 ;
  if (listOk && (false == AttsList.empty()))
  {
		for (NSPersonsAttributeIter iterUser = AttsList.begin() ; AttsList.end() != iterUser ; iterUser++)
		{
      NSUtiliInfo* pUtiliInfo = new NSUtiliInfo(*iterUser, pContexte) ;
      pUtilArray->push_back(pUtiliInfo) ;

      nbUtil++ ;
		}
	}

	return true ;
}

voidNSReqListUtilDialog::InitListe()
{
  TListWindColumn colNom("Nom", 100, TListWindColumn::Left, 0) ;
  pListeUtil->InsertColumn(0, colNom) ;
  TListWindColumn colPrenom("Prenom", 100, TListWindColumn::Left, 1) ;
  pListeUtil->InsertColumn(1, colPrenom) ;
}

voidNSReqListUtilDialog::AfficheListe()
{
	char nom[255] ;

	pListeUtil->DeleteAllItems() ;

	for (int i = nbUtil - 1; i >= 0; i--)
	{
  	sprintf(nom, "%s", ((*pUtilArray)[i])->getNom().c_str()) ;
    TListWindItem Item(nom, 0) ;
    pListeUtil->InsertItem(Item) ;
	}
}

voidNSReqListUtilDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int BufLen = 255 ;
	static char buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne	switch (dispInfoItem.GetSubItem())
	{
  	case 1: 	// prénom
    	sprintf(buffer, "%s", ((*pUtilArray)[index])->getPrenom().c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
	}
}

voidNSReqListUtilDialog::CmOk()
{
	UtilChoisi = pListeUtil->IndexItemSelect();
	if (UtilChoisi >= 0)
		*pUtilSelect = *((*pUtilArray)[UtilChoisi]) ;
	else
	{
  	erreur("Vous devez choisir un utilisateur.", standardError, 0, GetHandle()) ;
    return ;
	}

	NSUtilDialog::CmOk() ;
}

voidNSReqListUtilDialog::CmCancel()
{
	UtilChoisi = -1 ;
  NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSReqListUtilWindow
//
// -----------------------------------------------------------------


DEFINE_RESPONSE_TABLE1(NSReqListUtilWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSReqListUtilWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
voidNSReqListUtilWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmOk() ;
}


//---------------------------------------------------------------------------
//  Function: NSReqListUtilWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSReqListUtilWindow::IndexItemSelect()
{
    int count = GetItemCount();
    int index = -1;

    for (int i = 0; i < count; i++)
   	    if (GetItemState(i, LVIS_SELECTED))
        {
      	    index = i;
            break;
        }

    return index;
}

// -----------------------------------------------------------------//
//  Méthodes de NSValeurChiffreeDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSValeurChiffreeDialog, NSUtilDialog)
    EV_BN_CLICKED(IDC_VALEUR_AVECINF, CmAvecInf),
    EV_BN_CLICKED(IDC_VALEUR_AVECSUP, CmAvecSup),
    EV_BN_CLICKED(IDC_VALEUR_EFF, CmEffacer),
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSValeurChiffreeDialog::NSValeurChiffreeDialog(TWindow* pere, int MaxEntree,
                                                NSContexte* pCtx, TModule* mod)
                       :NSUtilDialog(pere, pCtx, "IDD_VALEUR", mod)
{
  sValeurExacte = "" ;
  sValeurSup    = "" ;
  sValeurInf    = "" ;
  sUnite        = "" ;
  bAvecInf      = false ;
  bAvecSup      = false ;
  MaxInPut      = MaxEntree ;
	//
	// Création de tous les "objets de contrôle"
	//
  pValeurExacte = new NSUtilEdit(pContexte, this, IDC_VALEUR_EDITION, MaxInPut) ;
  pValeurSup 	  = new NSUtilEdit(pContexte, this, IDC_VALEUR_SUP, MaxInPut) ;
  pValeurInf 	  = new NSUtilEdit(pContexte, this, IDC_VALEUR_INF, MaxInPut) ;
  pUnite        = new NSUtilLexique(pContexte, this, IDC_VALEUR_UNITE, pCtx->getDico()) ;
  pAvecInf      = new TRadioButton(this, IDC_VALEUR_AVECINF) ;
  pAvecSup      = new TRadioButton(this, IDC_VALEUR_AVECSUP) ;
}

NSValeurChiffreeDialog::~NSValeurChiffreeDialog(){
  delete pValeurExacte ;
  delete pValeurInf ;
  delete pValeurSup ;
  delete pUnite ;
  delete pAvecInf ;
  delete pAvecSup ;
}

void
NSValeurChiffreeDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  pValeurExacte->SetText(sValeurExacte.c_str()) ;
  pValeurSup->SetText(sValeurSup.c_str()) ;
  pValeurInf->SetText(sValeurInf.c_str()) ;
  if (bAvecInf)
    pAvecInf->Check() ;
  if (bAvecSup)
    pAvecSup->Check() ;
  pUnite->setLabel(sUnite) ;
}


void
NSValeurChiffreeDialog::CmAvecInf()
{
    if (bAvecInf)
    {
        bAvecInf = false;
        pAvecInf->Uncheck();
    }
    else
    {
        bAvecInf = true;
        pAvecInf->Check();
    }
}

void
NSValeurChiffreeDialog::CmAvecSup()
{
    if (bAvecSup)
    {
        bAvecSup = false;
        pAvecSup->Uncheck();
    }
    else
    {
        bAvecSup = true;
        pAvecSup->Check();
    }
}
voidNSValeurChiffreeDialog::CmEffacer()
{
  // on ne touche pas à l'unité
  pValeurExacte->SetText("");
  sValeurExacte = "";
  pValeurSup->SetText("");
  sValeurSup = "";
  pAvecSup->Uncheck();
  bAvecSup = false;
  pValeurInf->SetText("");
  sValeurInf = "";
  pAvecInf->Uncheck();
  bAvecInf = false;
}

voidNSValeurChiffreeDialog::CmOk()
{
  size_t pos ;
  sValeurExacte = pValeurExacte->GetText(MaxInPut) ;
  pos = sValeurExacte.find(",") ;
  if (NPOS != pos)
    sValeurExacte.replace(pos, 1, ".") ;

  sValeurSup = pValeurSup->GetText(MaxInPut) ;  pos = sValeurSup.find(",") ;
  if (NPOS != pos)
    sValeurSup.replace(pos, 1, ".") ;

  sValeurInf = pValeurInf->GetText(MaxInPut) ;
  pos = sValeurInf.find(",") ;
  if (NPOS != pos)
    sValeurInf.replace(pos, 1, ".") ;

  if ((sValeurExacte != "") && ((sValeurSup != "") || (sValeurInf != "")))
  {
  	erreur("Il faut choisir la valeur exacte ou l'intervalle", standardError, 0) ;
    pValeurExacte->SetFocus() ;
    return ;
  }

  if (sValeurExacte == "")
  {
      if (pAvecInf->GetCheck() == BF_CHECKED)
          bAvecInf = true;
      else
          bAvecInf = false;

      if (pAvecSup->GetCheck() == BF_CHECKED)
          bAvecSup = true;
      else
          bAvecSup = false;
  }

  char far* unite = new char[MaxInPut + 1];
  pUnite->GetText(unite, MaxInPut + 1);
  if (!strcmp(unite, ""))
  {
      sUnite = "200001";
  }
  else
  {
      sUnite = pUnite->getCode();
      if (sUnite == "£?????")
      {
          erreur("L'unité doit être choisie à partir du lexique, et non en texte libre.", standardError, 0, GetHandle()) ;
          delete[] unite;
          return;
      }
  }
  delete[] unite;

	NSUtilDialog::CmOk();
}


void
NSValeurChiffreeDialog::CmCancel()
{
	NSUtilDialog::CmCancel();
}

// fin de nsrechcr.cpp
////////////////////////////////////////////////////////////////////////////////////

