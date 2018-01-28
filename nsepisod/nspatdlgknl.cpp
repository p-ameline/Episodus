#include <string.h>#include <cstring.h>
#include <stdio.h>

#include "nautilus\nssuper.h"
#include "nautilus\nschoisi.h"
#include "nautilus\nspatdlgknl.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nsresour.h"
#include "partage\nsdivfct.h"
#include "partage\ns_timer.h"
#include "nssavoir\nsbloque.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nscqdoc.h"#include "nsbb\nsarc.h"#include "nsbb\ns_skins.h"#include "nsutil\md5.h"   // Cryptage MD5#include "nssavoir\nsHealthTeamMemberInterface.h"#include "nsbb\logpass.h"#include "nsbb\nsdefArch.h"#include "nsepisod\nsepisod.h"#include "pilot\NautilusPilot.hpp"#include "nsbb\tagNames.h"
#include "nsutil\md5.h"boolpatSortByNameInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

  string sMaj1 = pseumaj(pPat1->getNom()) ;
  string sMaj2 = pseumaj(pPat2->getNom()) ;

	if (sMaj1 != sMaj2)
		return (sMaj1 < sMaj2) ;

	return (pseumaj(pPat1->getPrenom()) < pseumaj(pPat2->getPrenom())) ;
}

bool
patSortByNameSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

  string sMaj1 = pseumaj(pPat1->getNom()) ;
  string sMaj2 = pseumaj(pPat2->getNom()) ;

	if (sMaj1 != sMaj2)
		return (sMaj1 > sMaj2) ;

	return (pseumaj(pPat1->getPrenom()) > pseumaj(pPat2->getPrenom())) ;
}

bool
patSortByBirthInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

	return (pPat1->getNaissance() < pPat2->getNaissance()) ;
}

bool
patSortByBirthSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

	return (pPat1->getNaissance() > pPat2->getNaissance()) ;
}

bool
patSortBySexInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

	return (pPat1->getSexe() < pPat2->getSexe()) ;
}

bool
patSortBySexSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
  if (((NSPatInfo*) NULL == pPat1) || ((NSPatInfo*) NULL == pPat2))
    return false ;

	return (pPat1->getSexe() > pPat2->getSexe()) ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSNTiersListePatDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNTiersListePatDialog, NSUtilDialog)	EV_COMMAND(IDOK,                    CmOk),
	EV_COMMAND(IDC_LISTPA_OUVRIR,       CmOk),
	EV_COMMAND(IDCANCEL,                CmCancel),
	EV_COMMAND(IDC_NTLISTPA_RECHPAT,    Rechercher),
	EV_LVN_GETDISPINFO(IDC_NTLISTPA_LW, LvnGetDispInfo),
	EV_LVN_COLUMNCLICK(IDC_NTLISTPA_LW, LVNColumnclick),
  EV_WM_TIMER,
  EV_COMMAND_AND_ID(IDC_USENOM,       nameActiver),
  EV_COMMAND_AND_ID(IDC_USEPRENOM,    nameActiver),
  EV_COMMAND_AND_ID(IDC_USEBIRTHDATE, nameActiver),
END_RESPONSE_TABLE;NSNTiersListePatDialog::NSNTiersListePatDialog(TWindow* parent,                                               NSContexte* pCtx,                                               NSPatInfo* pPat,                                               char* cResId,                                               OWL::TModule* module)                       :NSUtilDialog(parent, pCtx, cResId, module == 0 ? pNSResModule : module)
{
try
{
	// Création de tous les "objets de contrôle"
	pNom 	      = new NSNTiersRechNomEdit(pContexte, this, IDC_NTLISTPA_NOM, PAT_NOM_LEN) ;
	pPrenom     = new NSNTiersRechNomEdit(pContexte, this, IDC_NTLISTPA_PRENOM, PAT_PRENOM_LEN) ;
  pDateNaiss  = new NSUtilEditDate(pContexte, this, IDC_NTLISTPA_NAISS, 10, true) ;
	pNumero     = new NSUtilEdit(pContexte, this, IDC_NTLISTPA_NUMERO, PAT_NSS_LEN) ;
	pCode       = new NSNTiersRechNomEdit(pContexte, this, IDC_NTLISTPA_CODE, PAT_CODE_LEN) ;

  pLibName         = new TStatic(this, IDS_NTLISTPA_NOM) ;
  pLibGivenName    = new TStatic(this, IDS_NTLISTPA_PRENOM) ;
  pLibBirthdate    = new TStatic(this, IDS_NTLISTPA_NAISS) ;
	pLibID           = new TStatic(this, IDS_NTLISTPA_NUMERO) ;
	pLibCode         = new TStatic(this, IDS_NTLISTPA_CODE) ;

  pUseNom          = new TCheckBox(this, IDC_USENOM) ;
  pUsePrenom       = new TCheckBox(this, IDC_USEPRENOM) ;
  pUseBirthdate    = new TCheckBox(this, IDC_USEBIRTHDATE) ;

	pExactButton     = new TRadioButton(this, IDC_RECH_EXACT) ;
	pStartByButton   = new TRadioButton(this, IDC_RECH_APPROCHE) ;

	pListe 	         = new NSNTiersListPatWindow(this, pContexte, IDC_NTLISTPA_LW) ;
	iSortedColumn    = 1 ;
  bNaturallySorted = true ; // in order to have it naturally sorted

	// on recupère le patient en cours
	pPatEnCours = pPat ;

	//il faut reinitialiser la liste des patients
	bMustInit = true ;

	// Création du tableau de Patients
	pPatientsArray = new NSPatientArray ;

	// nom et prénom initiaux
  _sNomPat    = string("") ;
  _sPrenomPat = string("") ;
  _sDateNaiss = string("00000000") ;
  _sNumPat    = string("") ;
  _sCodePat   = string("") ;

  _iAutoSearchDelay   = 1000 ;
  _iAutoSearchMinChar = 1 ;

	// Existe-t-il des données de capture Episodus ?
	if (pContexte->getEpisodus())
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;

    bool bIPPMode = false ;
    if (string("") != pContexte->getSuperviseur()->getIppSite())
    	bIPPMode = true ;

    string sNom     = string("") ;
    string sPrenom  = string("") ;
    string sCode    = string("") ;

    pCapt->getIdentity(sNom, sPrenom) ;

    if (false == bIPPMode)
    	pCapt->trouveChemin(string("LIDEN1"), &sCode) ;
    else
    	pCapt->trouveChemin(string("LIPP01"), &sCode) ;

    if (strlen(sNom.c_str()) > PAT_NOM_LEN)
    	sNom = string(sNom, 0, PAT_NOM_LEN) ;
    _sNomPat = sNom ;

    if (strlen(sPrenom.c_str()) > PAT_PRENOM_LEN)
    	sPrenom = string(sPrenom, 0, PAT_PRENOM_LEN) ;
    _sPrenomPat = sPrenom ;

    pCapt->trouveChemin(string("ZADMI1/LIDET1/KNAIS1"), &_sDateNaiss) ;
    if (string("") == _sDateNaiss)
      _sDateNaiss = string("00000000") ;
  }

  sNssSelect = string("") ;
  // fichiers d'aide  // pContexte->getSuperviseur()->setAideIndex("h_index.htm") ;
  // pContexte->getSuperviseur()->setAideCorps("h_rchpat.htm") ;
}
catch (...)
{
	erreur("Exception NSNTiersListePatDialog ctor.", standardError, 0) ;
}
}

NSNTiersListePatDialog::~NSNTiersListePatDialog()
{
	delete pNom ;
	delete pPrenom ;
  delete pDateNaiss ;
	delete pNumero ;
	delete pCode ;
  delete pExactButton ;
  delete pStartByButton ;
	delete pListe ;
	delete pPatientsArray ;
  delete pLibName ;
  delete pLibGivenName ;
  delete pLibBirthdate ;
	delete pLibID ;
	delete pLibCode ;
  delete pUseNom ;
  delete pUsePrenom ;
  delete pUseBirthdate ;
}

void
NSNTiersListePatDialog::SetupWindow()
{
	string ps = string("Entering SetupWindow for patient selection") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	TDialog::SetupWindow() ;

	pNom->SetText(_sNomPat) ;
	pPrenom->SetText(_sPrenomPat) ;
  pDateNaiss->setDate(_sDateNaiss) ;

  pUseNom->Check() ;
  if (string("") == _sPrenomPat)
  	pUsePrenom->Check() ;
  else
  	pUsePrenom->Uncheck() ;

  string sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "famillyName") ;
  if (sLocalText != "")
  	pLibName->SetText(sLocalText.c_str()) ;
  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "givenName") ;
  if (sLocalText != "")
  	pLibGivenName->SetText(sLocalText.c_str()) ;
  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "birthdate") ;
  if (sLocalText != "")
  	pLibBirthdate->SetText(sLocalText.c_str()) ;
  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "patientId") ;
  if (sLocalText != "")
		pLibID->SetText(sLocalText.c_str()) ;
  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "code") ;
  if (sLocalText != "")
		pLibCode->SetText(sLocalText.c_str()) ;

  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "fuzzySearch") ;
  if (sLocalText != "")
  	pStartByButton->SetCaption(sLocalText.c_str()) ;
  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "exactSearch") ;
  if (sLocalText != "")
		pExactButton->SetCaption(sLocalText.c_str()) ;

	ps = string("Initializing patients list") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	// Initialisation de la liste
	InitListe();
	// AfficheListe();

  pStartByButton->SetCheck(BF_CHECKED) ;

	if (string("") != _sNomPat)
  	Rechercher() ;

	ps = string("Leaving SetupWindow for patient selection") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}

void
NSNTiersListePatDialog::InitListe()
{
	string sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "patient") ;
  if (sLocalText == "")
  	sLocalText = "Patient" ;
	TListWindColumn colPatient((char*) sLocalText.c_str(), 250, TListWindColumn::Left, 0) ;
	pListe->InsertColumn(0, colPatient) ;

  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "birthDate") ;
  if (sLocalText == "")
  	sLocalText = "Date Naiss" ;
	TListWindColumn colDateN((char*) sLocalText.c_str(), 80, TListWindColumn::Left, 1) ;	pListe->InsertColumn(1, colDateN) ;

  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "sex") ;
  if (sLocalText == "")
  	sLocalText = "Sexe" ;
	TListWindColumn colSexe((char*) sLocalText.c_str(), 40, TListWindColumn::Left, 2) ;
	pListe->InsertColumn(2, colSexe) ;

  sLocalText = pContexte->getSuperviseur()->getText("dialog_patientList", "locked") ;
  if (sLocalText == "")
  	sLocalText = "Bloqué" ;
	TListWindColumn colBloque((char*) sLocalText.c_str(), 50, TListWindColumn::Left, 3) ;	pListe->InsertColumn(3, colBloque) ;
}

void
NSNTiersListePatDialog::AfficheListe()
{
	// On vide la liste
	//
	pListe->DeleteAllItems() ;

  if (pPatientsArray->empty())
    return ;

  string ps = string("Entering NSNTiersListePatDialog::AfficheListe") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	PatientReverseIter iterReverse = pPatientsArray->rbegin() ;

	while (pPatientsArray->rend() != iterReverse)
	{
		string sNomLong = (*iterReverse)->getNom() + string(" ") + (*iterReverse)->getPrenom() ;
		TListWindItem Item(sNomLong.c_str(), 0) ;

		pListe->InsertItem(Item) ;
		iterReverse++ ;
	}

  ps = string("Leaving NSNTiersListePatDialog::AfficheListe") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSNTiersListePatDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int 	BufLen = 255 ;
	static char buffer[BufLen] ;
	int 		    index ;
	static char libelBloqu[3] ;
	static char sexe[2] ;
	string 		  clef ;
  string 		  sLockingConsole ;

	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1 : // date de naissance
    	if (((*pPatientsArray)[index])->donneDateNaiss() != "")
      {
      	sprintf(buffer, "%s", (((*pPatientsArray)[index])->donneDateNaiss()).c_str()) ;
        dispInfoItem.SetText(buffer) ;
      }
      break ;

    case 2 : // sexe
    	if (((*pPatientsArray)[index])->estMasculin())
      	strcpy(sexe, "M") ;
      else
      	strcpy(sexe, "F") ;
      dispInfoItem.SetText(sexe) ;

      break ;

    case 3 : // blocage : à faire
      sLockingConsole = ((*pPatientsArray)[index])->getLockingConsoleId()	;
    	strcpy(libelBloqu, sLockingConsole.c_str()) ;
      dispInfoItem.SetText(libelBloqu) ;
      break ;
	}
}

void
NSNTiersListePatDialog::InitPatArray()
{
  string ps = string("Entering NSNTiersListePatDialog::InitPatArray") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	pPatientsArray->vider() ;

	bool listOk = false ;

	// le serviceName est défini dans le constructeur
	//on a une liste des patients
	serviceName = (NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str() ;

	// on récupère le nom et le prénom pour les passer comme traits au service
  //
  _sNomPat = string("") ;
  if (pUseNom->GetCheck() == BF_CHECKED)
		_sNomPat = pNom->GetText() ;

  _sPrenomPat = string("") ;
  if (pUsePrenom->GetCheck() == BF_CHECKED)
    _sPrenomPat = pPrenom->GetText() ;

  _sDateNaiss = string("") ;
  string sEnteredDateNaiss = string("") ;
  pDateNaiss->getDate(&sEnteredDateNaiss) ;
  if (pUseBirthdate->GetCheck() == BF_CHECKED)
    _sDateNaiss = sEnteredDateNaiss ;

  if (string("00000000") == _sDateNaiss)
    _sDateNaiss = string("") ;

  _sCodePat = pCode->GetText() ;

  // No trait -> full patients list will be returned
  //
	if ((string("") == _sNomPat) && (string("") == _sPrenomPat) && (string("") == _sCodePat) && (string("") == _sDateNaiss))
  {
    string sWarningText = pContexte->getSuperviseur()->getText("dialog_patientList", "noTraitEnteredDoYouWishAllPatients") ;
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;

    int idRet = MessageBox(sWarningText.c_str(), sCaption.c_str(), MB_YESNO) ;
    if (IDYES != idRet)
      return ;
		serviceName = (NautilusPilot::SERV_PATIENT_LIST).c_str() ;
  }

	//else if ((strcmp(nomPat, "") != 0) && (IsCharAlpha(nomPat[strlen(nomPat) - 1])))
	//	strcat(nomPat, "*");
	//else if ((strcmp(prenomPat, "") != 0) && (IsCharAlpha(prenomPat[strlen(prenomPat) - 1])))
	//	strcat(prenomPat, "*");

  string sNomToCall    = string("") ;
  string sPrenomToCall = string("") ;
  string sBirthToCall  = string("") ;
  string sCodeToCall   = string("") ;

	NSPersonsAttributesArray PatiensList ;
	NSBasicAttributeArray    AttrArray ;

	AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
	if (string("") != _sPrenomPat)
	{
		prepareString(_sPrenomPat) ;
    sPrenomToCall = _sPrenomPat ;

    // Research is in "close to" mode, add '%' if not empty and not there
    //
    if ((pStartByButton->GetCheck() == BF_CHECKED) && (string("") != sPrenomToCall))
    {
    	if ('%' != sPrenomToCall[strlen(sPrenomToCall.c_str())-1])
      	sPrenomToCall += string("%") ;
    }

		AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, sPrenomToCall)) ;
	}

  if (string("") != _sNomPat)
    prepareString(_sNomPat) ;
  sNomToCall = _sNomPat ;

  if (string("") != _sDateNaiss)
  {
    sBirthToCall = _sDateNaiss ;

    // Treats jokers (for example 19540000 for any date in 1954)
    //
    if (strlen(sBirthToCall.c_str()) == 8)
    {
      if (string("0000") == string(sBirthToCall, 4, 4))
        sBirthToCall = string(sBirthToCall, 0, 4) + string("%") ;
      else if (string("00") == string(sBirthToCall, 6, 2))
        sBirthToCall = string(sBirthToCall, 0, 6) + string("%") ;
    }

    AttrArray.push_back(new NSBasicAttribute(BIRTHDATE, sBirthToCall)) ;
  }

  if (string("") != _sCodePat)
  {
/*
    string sDateFormat = pContexte->getSuperviseur()->getText("0localInformation", "dateFormat") ;
    if (IsValidDatePattern(_sCodePat, sDateFormat))
    {
      string sCodedDate = string("") ;
      donne_date_inverse(_sCodePat, sCodedDate, pContexte->getUserLanguage()) ;

      // Treats jokers (for example 19540000 for any date in 1954)
      //
      if (string("0000") == string(sCodedDate, 4, 4))
        sCodedDate = string(sCodedDate, 0, 4) + string("%") ;
      else if (string("00") == string(sCodedDate, 6, 2))
        sCodedDate = string(sCodedDate, 0, 6) + string("%") ;

      AttrArray.push_back(new NSBasicAttribute(BIRTHDATE, sCodedDate)) ;
    }
    else
    {
*/
  	  prepareString(_sCodePat) ;
      sCodeToCall = _sCodePat ;

      if (string("") != pContexte->getSuperviseur()->getIppSite())
    	  AttrArray.push_back(new NSBasicAttribute(IPP, sCodeToCall)) ;
      else
    	  AttrArray.push_back(new NSBasicAttribute(LIDEN, sCodeToCall)) ;
//    }
  }

  // WARNING: Always keep this as the last chacked entry, since it behaves
  //          differently according to another information being entered or not
  //
  if (pStartByButton->GetCheck() == BF_CHECKED)
  {
    if ((string("") == sNomToCall) || ('%' != sNomToCall[strlen(sNomToCall.c_str())-1]))
      sNomToCall += string("%") ;
  }
  //
  // If another seach parameter has been entered, it is useless to add "%" as
  // a search parameter for name
  //
  if ((string("%") != sNomToCall) || AttrArray.empty())
    AttrArray.push_back(new NSBasicAttribute(LAST_NAME, sNomToCall)) ;

  if ((string("") == sNomToCall)    &&
      (string("") == sPrenomToCall) &&
      (string("") == sBirthToCall)  &&
      (string("") == sCodeToCall))
		return ;

	AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
	AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;

  ps = string("NSNTiersListePatDialog::InitPatArray, calling Pilot...") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	listOk = pContexte->getSuperviseur()->getPilot()->personList(serviceName, &PatiensList, &AttrArray) ;
	if (false == listOk)
	{
		std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
		std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
		if (std::string("") != tempMessage)
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
		if (std::string("") != tempError)
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
		return ;
	}

  if (false == PatiensList.empty())
  {
    ps = string("NSNTiersListePatDialog::InitPatArray, filling list...") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

		for (NSPersonsAttributeIter iterPatient = PatiensList.begin() ; PatiensList.end() != iterPatient ; iterPatient++)
		{
			NSPatInfo *pPatientEnCours = new NSPatInfo(*iterPatient, pContexte) ;
      // ps = string("NSNTiersListePatDialog::InitPatArray, adding ") + pPatientEnCours->getNom() + string(" ") + pPatientEnCours->getPrenom() ;
      // pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
			pPatientsArray->push_back(pPatientEnCours) ;
		}
  }

  ps = string("NSNTiersListePatDialog::InitPatArray, sorting list...") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  sortByName() ;

  ps = string("Leaving NSNTiersListePatDialog::InitPatArray") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSNTiersListePatDialog::InitPatArrayForNss()
{
  pPatientsArray->vider() ;

  if (string("") == _sNumPat)
    return ;

  NSPersonInfo tempInfo(pContexte->getSuperviseur(), pContexte, _sNumPat, pidsPatient) ;

  NSPatInfo *pPatientEnCours = new NSPatInfo(pContexte) ;
  pPatientEnCours->initFromPersonInfo(&tempInfo) ;
  pPatientsArray->push_back(pPatientEnCours) ;
}

void
NSNTiersListePatDialog::Rechercher()
{
	KillTimer(ID_PAT_TIMER) ;
	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  _sNumPat = pNumero->GetText() ;
  if (string("") != _sNumPat)
    InitPatArrayForNss() ;
  else
	  // Initialisation du tableau des patients
	  InitPatArray() ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

	// Affichage de la liste
	AfficheListe() ;

  // Select first patient in the list, so that <Enter> can have her get open
  int iCount = pListe->GetItemCount() ;
  if (iCount > 0)
  	pListe->SetSel(0, true) ;

//	TWindow* pWnd = ChildWithId(IDC_LISTPA_OUVRIR) ;
//  if (NULL != pWnd)
//		SetControlFocus(pWnd->HWindow) ;
}

void
NSNTiersListePatDialog::GardeNss(int index)
{
	sNssSelect = ((*pPatientsArray)[index])->getNss() ;
}

void
NSNTiersListePatDialog::RetrouvePatSelect()
{
	for (int i = 0; i < nbNom; i++)
		if (sNssSelect == ((*pPatientsArray)[i])->getNss())
    {    	pListe->SetSel(i, false) ;
      break;
    }
}

void
NSNTiersListePatDialog::nameActiver(WPARAM wParam)
{
	switch (wParam)
	{
		case (IDC_USENOM) :
    	if (pUseNom->GetCheck() == BF_CHECKED)
      	pUseNom->Uncheck() ;
      else
      	pUseNom->Check() ;
      break ;
    case (IDC_USEPRENOM) :
    	if (pUsePrenom->GetCheck() == BF_CHECKED)
      	pUsePrenom->Uncheck() ;
      else
      	pUsePrenom->Check() ;
      break ;
    case (IDC_USEBIRTHDATE) :
    	if (pUseBirthdate->GetCheck() == BF_CHECKED)
      	pUseBirthdate->Uncheck() ;
      else
      	pUseBirthdate->Check() ;
      break ;
	}

  if (existSearchableInformation())
    Rechercher() ;
  else
  {
    pPatientsArray->vider() ;
    AfficheListe() ;
  }
}

bool
NSNTiersListePatDialog::existSearchableInformation()
{
  if (pUseNom->GetCheck() == BF_CHECKED)
  {
    string sPatName = pNom->GetText() ;
    if (string("") != sPatName)
      return true ;
  }

  if (pUsePrenom->GetCheck() == BF_CHECKED)
  {
    string sPatFirstName = pPrenom->GetText() ;
    if (string("") != sPatFirstName)
      return true ;
  }

  if (pUseBirthdate->GetCheck() == BF_CHECKED)
  {
    string sBirthdate = string("") ;
    pDateNaiss->getDate(&sBirthdate) ;
    if ((string("") != sBirthdate) && (string("00000000") != sBirthdate))
      return true ;
  }

  string sNumPat = pNumero->GetText() ;
  if (string("") != sNumPat)
    return true ;

  string sCodePat = pCode->GetText() ;
  if (string("") != sCodePat)
    return true ;

  return false ;
}

bool
NSNTiersListePatDialog::CanClose()
{
	return TDialog::CanClose() ;
}

voidNSNTiersListePatDialog::CmOk(){try{
	int index = pListe->IndexItemSelect() ;
  if (index == -1)
  {
  	// erreur("Vous devez sélectionner un patient dans la liste.",0,0,GetHandle());
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "PatientNotSelected") ;
    erreur(sErrorText.c_str(), warningError, standardError, 0) ;
    return ;
  }

  *pPatEnCours = *((*pPatientsArray)[index]) ;

  CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSNTiersListePatDialog::CmOk", standardError, 0) ;
}}voidNSNTiersListePatDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByName() ;      break ;
    case 1  : sortByBirthday() ;  break ;
    case 2  : sortBySex() ;       break ;
  }
}
voidNSNTiersListePatDialog::sortByName(bool bChangeOrder)
{
  string ps = string("Entering NSNTiersListePatDialog::sortByName") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	if (0 == iSortedColumn)
	{
    if (bChangeOrder)
  	{
    	if (bNaturallySorted)
      	bNaturallySorted = false ;
    	else
      	bNaturallySorted = true ;
    }
	}
  else
  {
    iSortedColumn = 0 ;
    bNaturallySorted = true ;
  }

  if (pPatientsArray->empty())
  {
    ps = string("Leaving NSNTiersListePatDialog::sortByName, empty patient array") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    return ;
  }

  ps = string("NSNTiersListePatDialog::sortByName sorting...") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  if (bNaturallySorted)
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByNameInf) ;
  else
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByNameSup) ;

  ps = string("NSNTiersListePatDialog::sortByName calling AfficheListe") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

  AfficheListe() ;

  ps = string("Leaving NSNTiersListePatDialog::sortByName") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
}

void
NSNTiersListePatDialog::sortByBirthday()
{
  if (iSortedColumn == 0)
  {
    if (bNaturallySorted)
      bNaturallySorted = false ;
    else
      bNaturallySorted = true ;
  }
  else
  {
    iSortedColumn = 0 ;
    bNaturallySorted = true ;
  }

  if (pPatientsArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByBirthInf) ;
  else
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByBirthSup) ;

  AfficheListe() ;
}

void
NSNTiersListePatDialog::sortBySex()
{
  if (iSortedColumn == 0)
  {
    if (bNaturallySorted)
      bNaturallySorted = false ;
    else
      bNaturallySorted = true ;
  }
  else
  {
    iSortedColumn = 0 ;
    bNaturallySorted = true ;
  }

  if (pPatientsArray->empty())
    return ;

  if (bNaturallySorted)
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortBySexInf) ;
  else
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortBySexSup) ;

  AfficheListe() ;
}

void
NSNTiersListePatDialog::prepareString(string& sStr)
{
  size_t iStrLen = strlen(sStr.c_str()) ;

  for (size_t i = 0 ; i < iStrLen ; i++)
    if ('*' == sStr[i])
      sStr[i] = '%' ;
}

void
NSNTiersListePatDialog::EvTimer(uint id)
{
	if (ID_PAT_TIMER != id)
		return ;

	// Searching patients from an empty string can be very time consuming, prevent it
  //
  string sNomPat = pNom->GetText() ;
  if (strlen(sNomPat.c_str()) < _iAutoSearchMinChar)
  {
  	string sCodePat = pCode->GetText() ;
    if (string("") == sCodePat)
  	{
  		pListe->DeleteAllItems() ;
			return ;
    }
	}

	Rechercher() ;
}

void
NSNTiersListePatDialog::resetTimer()
{
	KillTimer(ID_PAT_TIMER) ;
	SetTimer(ID_PAT_TIMER, _iAutoSearchDelay) ;
}

/***************************************************/
/* CLASSE NSNTiersListPatWindow                    */
/***************************************************/

DEFINE_RESPONSE_TABLE1(NSNTiersListPatWindow, NSSkinableListWindow)
   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_LBUTTONDOWN,
   EV_WM_KILLFOCUS,
   EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

NSNTiersListPatWindow::NSNTiersListPatWindow(NSNTiersListePatDialog* pere, NSContexte* pCtx, int resId)
                      :NSSkinableListWindow(pere, pCtx, resId)
{
	pDlg = pere ;
  // Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;	Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;}

void
NSNTiersListPatWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

  NSSkinableListWindow::SetupWindow() ;

  skinSwitchOff("patientListOff") ;
}
// on redéfinit EvKeyDown pour pouvoir scroller avec les flèchesvoid
NSNTiersListPatWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
	int count, itemSel ;

	switch (key)
	{
  	case VK_DOWN :

      count = GetItemCount() ;
      itemSel = IndexItemSelect() ;
      if (itemSel == (count - 1))
      {
      	// pDlg->pVScroll->SBLineDown() ;
        SetSel(itemSel, true) ;
      }
      else if (itemSel != -1)
          SetSel(itemSel + 1, true) ;
      break ;

    case VK_UP :

      itemSel = IndexItemSelect() ;
      if (itemSel == 0)
      {
      	// pDlg->pVScroll->SBLineUp() ;
        SetSel(itemSel, true) ;
      }
      else if (itemSel != -1)
          SetSel(itemSel - 1, true) ;
      break ;

    case VK_NEXT :
      itemSel = IndexItemSelect() ;      if (itemSel != -1)
      {
      	// pDlg->pVScroll->SBPageDown() ;
        SetSel(itemSel, true) ;
      }
      break ;

    case VK_PRIOR :
      itemSel = IndexItemSelect() ;      if (itemSel != -1)
      {
      	// pDlg->pVScroll->SBPageUp() ;
        SetSel(itemSel, true) ;
      }
      break ;

    default :

  		TListWindow::EvKeyDown(key, repeatCount, flags) ;	}
}

//---------------------------------------------------------------------------//  Function: NSNTiersListePatWindow::EvLButtonDown(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click (désactive la multi-sélection)
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSNTiersListPatWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	// int count = GetItemCount();
	TLwHitTestInfo info(point);

	int index = HitTest(info);

	if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->GardeNss(index) ;

	// On appelle le Button Down de la classe mère
	TListWindow::EvLButtonDown(modKeys, point) ;
}

//---------------------------------------------------------------------------
//  Function: NSNTiersListePatWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSNTiersListPatWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmOk() ;
}

//---------------------------------------------------------------------------//  Function: NSNTiersListePatWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSNTiersListPatWindow::IndexItemSelect()
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

// Gain du focus
//
void
NSNTiersListPatWindow::EvSetFocus(HWND hWndLostFocus)
{
	skinSwitchOn("patientListOn") ;

	TListWindow::EvSetFocus(hWndLostFocus) ;

	int count = GetItemCount() ;
	if (count > 0)
	{
  	for (int i = 0 ; i < count ; i++)
    	if (GetItemState(i, LVIS_SELECTED))
      	return ;

    SetItemState(0, select ? LVIS_FOCUSED | LVIS_SELECTED : 0, LVIS_SELECTED);
        //SetItemState(0 , LVIS_FOCUSED | LVIS_SELECTED, LVIF_STATE) ;
        //SetSel(0, true) ;
	}
}

//
// Perte du focus.
//
void
NSNTiersListPatWindow::EvKillFocus(HWND hWndGetFocus)
{
	skinSwitchOff("patientListOff") ;

	TListWindow::EvKillFocus(hWndGetFocus) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSRechNomEdit
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNTiersRechNomEdit, NSUtilEdit)
	EV_WM_CHAR,
	EV_WM_KEYUP,
END_RESPONSE_TABLE;

//// On doit intercepter EvChar pour interdire l'espace et le double-espace
//
void
NSNTiersRechNomEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	string sNom = GetText() ;

	// on interdit l'espace comme premier caractere
	if ((string("") == sNom) && (VK_SPACE == key))
		return ;

  size_t iNameLen = strlen(sNom.c_str()) ;

	// on interdit le double espace
	if ((iNameLen > 0) && (' ' == sNom[iNameLen - 1]) && (VK_SPACE == key))
		return ;

	NSUtilEdit::EvChar(key, repeatCount, flags) ;
}

//
// On doit intercepter EvKeyUp pour tenir compte de VK_DELETE
// qui n'est pas intercepté par EvChar
//
void
NSNTiersRechNomEdit::EvKeyUp(uint key, uint repeatCount, uint flags)
{
	NSUtilEdit::EvKeyUp(key, repeatCount, flags) ;

	if ((key != VK_RETURN) && (key != VK_TAB))
  	pDlg->resetTimer() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ChercheListePatCorDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChercheListePatCorDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_COMMAND(IDC_LISTCOR_AUTRE, CmAutreCorresp),
    EV_LVN_GETDISPINFO(IDC_LISTCOR_LW, LvnGetDispInfo),
END_RESPONSE_TABLE;

ChercheListePatCorDialog::ChercheListePatCorDialog(TWindow* pere, NSContexte* pCtx, TModule* mod)                         :NSUtilDialog(pere, pCtx, "IDD_LISTCOR", mod)
{
	pListeCorresp  = new NSListPatCorWindow(this, IDC_LISTCOR_LW, mod) ;
	pCorrespArray  = new NSPersonArray(pContexte->getSuperviseur()) ;
	pCorrespSelect = new NSPersonInfo(pContexte->getSuperviseur()) ;
}

ChercheListePatCorDialog::~ChercheListePatCorDialog(){
	delete pCorrespSelect ;
	delete pCorrespArray ;
	delete pListeCorresp ;
}

voidChercheListePatCorDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	InitPatCorArray() ;
  InitListe() ;
  AfficheListe() ;
}

// méthode pour un patient déjà initialisé
bool
ChercheListePatCorDialog::InitPatCorArray()
{
	pCorrespArray->vider() ;
	nbCorresp = 0 ;

	NSPatientChoisi* pPatChoisi = pContexte->getPatient() ;

	// Si on ne passe pas de patient, prend un array vide
	if ((pPatChoisi) && (NULL != pPatChoisi->getHealthTeam()))
	{
  	VecteurString* pMembersList = pPatChoisi->getHealthTeam()->getPIDS() ;
		for (EquiItemIter i = pMembersList->begin(); pMembersList->end() != i ; i++)
    {
      if (pPatChoisi->getNss() != **i)
      {
    	  NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, *(*i), pidsCorresp) ;
        if (pPersonInfo->getNss() != pPatChoisi->getNss())
        {
      	  pCorrespArray->push_back(new NSPersonInfo(*pPersonInfo)) ;
      	  nbCorresp++ ;
        }
      }
    }
	}

	if (0 == nbCorresp)
		return false ;	return true ;}

voidChercheListePatCorDialog::InitListe()
{
	TListWindColumn colNom("Nom", 100, TListWindColumn::Left, 0) ;
	pListeCorresp->InsertColumn(0, colNom) ;
	TListWindColumn colPrenom("Prenom", 100, TListWindColumn::Left, 1) ;
	pListeCorresp->InsertColumn(1, colPrenom) ;
	TListWindColumn colVille("Ville", 90, TListWindColumn::Left, 2) ;
	pListeCorresp->InsertColumn(2, colVille) ;
}

voidChercheListePatCorDialog::AfficheListe()
{
	char nom[255] ;

	pListeCorresp->DeleteAllItems() ;

	if (0 == nbCorresp)
		return ;

	for (int i = nbCorresp - 1; i >= 0; i--)
	{
		sprintf(nom, "%s", ((*pCorrespArray)[i])->getNom().c_str()) ;
    TListWindItem Item(nom, 0) ;
    pListeCorresp->InsertItem(Item) ;
	}
}

void
ChercheListePatCorDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int BufLen = 255 ;
	static char buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1: 	// prénom
    	sprintf(buffer, "%s", ((*pCorrespArray)[index])->getPrenom().c_str()) ;
      dispInfoItem.SetText(buffer) ;
      break ;
	}
}

voidChercheListePatCorDialog::CmAutreCorresp()
{
  NSTPersonListDialog *pListeCorDlg = new NSTPersonListDialog((TWindow *)this, pidsCorresp, false, pContexte, 0, true) ;
	if ((pListeCorDlg->Execute() == IDOK) && pListeCorDlg->pPersonSelect)
	{
		*pCorrespSelect = *(pListeCorDlg->pPersonSelect) ;
    CorrespChoisi = nbCorresp ;
	}
	delete pListeCorDlg ;
	NSUtilDialog::CmOk() ;
}

voidChercheListePatCorDialog::CmOk()
{
	CorrespChoisi = pListeCorresp->IndexItemSelect() ;

	if (CorrespChoisi >= 0)
		*pCorrespSelect = *((*pCorrespArray)[CorrespChoisi]) ;
	else
	{
  	erreur("Vous devez choisir un correspondant.", warningError, 0, GetHandle()) ;
    return ;
	}

	NSUtilDialog::CmOk() ;
}

voidChercheListePatCorDialog::CmCancel()
{
	CorrespChoisi = -1 ;
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListPatCorWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListPatCorWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSListPatCorWindow::NSListPatCorWindow(ChercheListePatCorDialog* pere, int resId, TModule* module) : TListWindow(pere, resId, module){
  pDlg = pere ;
}

NSListPatCorWindow::~NSListPatCorWindow(){
}

//---------------------------------------------------------------------------//  Function: NSListPatCorWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListPatCorWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
  TLwHitTestInfo info(point) ;

  HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
    pDlg->CmOk() ;
}

//---------------------------------------------------------------------------
//  Function: NSListPatCorWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListPatCorWindow::IndexItemSelect()
{
  int count = GetItemCount() ;
  int index = -1 ;

  for (int i = 0 ; i < count ; i++)
    if (GetItemState(i, LVIS_SELECTED))
    {
      index = i ;
      break ;
    }

  return index ;
}
// fin de nspatdlgknl.cpp
