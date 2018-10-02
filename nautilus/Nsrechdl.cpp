#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\static.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <stdio.h>
#include <string.h>
#include <owl\module.h>
#include <bde.hpp>

#include "nssavoir\nsBdmDriver.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nsrechd2.h"
#include "nautilus\nssuper.h"
#include "nautilus\nsresour.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsdocref.h"
#include "nautilus\nsdocaga.h"
#include "partage\nsdivfct.h"
#include "nautilus\nshistdo.h"
#include "nsoutil\mexcel.h"
#include "nsoutil\nsfilgui.h"
#include "nsoutil\nsUpdateDlg.h"
#include "nsldv\nsldvuti.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsedit.h"
#include "nautilus\nshisto.rh"
#include "nautilus\nautilus.rh"
#include "nsbb\nsbb.rh"

#include "nsutil\md5.h"

#include "pilot\Pilot.hpp"
#include "pilot\NautilusPilot.hpp"
#include "pilot\JavaSystem.hpp"#include "nsbb\tagNames.h"using namespace miniexcel ;
//----------------------- class NSUserWithRoleArray ---------------------
//utiliser pour garder le login + role d'utilisateur
NSUserWithRoleArray::NSUserWithRoleArray(const NSUserWithRoleArray& rv)                    :NSUserWithRoleVector(){  if (false == rv.empty())    for (NSUserWithRoleConstIter i = rv.begin() ; rv.end() != i ; i++)      push_back(new NSUserWithRole(*(*i))) ;}voidNSUserWithRoleArray::vider(){  if (empty())    return ;  for (NSUserWithRoleIter i = begin() ; end() != i ; )  {    delete *i ;
    erase(i) ;
  }
}
NSUserWithRoleArray::~NSUserWithRoleArray(){	vider() ;}
NSUserWithRoleArray&NSUserWithRoleArray::operator=(const NSUserWithRoleArray& src){  if (this == &src)		return *this ;	vider() ;  if (false == src.empty())    for (NSUserWithRoleConstIter i = src.begin() ; src.end() != i ; i++)      push_back(new NSUserWithRole(*(*i))) ;
	return *this ;}//----------------------- class UserLoginNTiersDialog --------------------------
DEFINE_RESPONSE_TABLE1(UserLoginNTiersDialog, NSUtilDialog)
	EV_COMMAND(IDOK,	     CmOk),
  EV_COMMAND(NSE_MANUAL, CmManual),
	EV_CHILD_NOTIFY_AND_CODE(NSE_LOGIN_UTILISATEUR, LBN_DBLCLK, CmLoginDblClk),
  EV_CHILD_NOTIFY(NSE_LOGIN_UTILISATEUR, CM_HELP, CmHelp),
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------
UserLoginNTiersDialog::UserLoginNTiersDialog(TWindow *pere, NSContexte *pCtx,
                                             NSUtilisateurChoisi* pUtil)
                      :NSUtilDialog(pere, pCtx, "USER_LOGIN")
{
try
{
  pUtilChoisi = pUtil;
  pLogs  = new OWL::TListBox(this, NSE_LOGIN_UTILISATEUR) ;
  pPass  = new NSUtilEdit(pContexte, this, NSE_CODE_UTILISATEUR,  UTI_CODE_LEN) ;
  pUsers = new NSUserWithRoleArray() ;

  pPassL = new OWL::TStatic(this, NSE_CODE_LIB) ;
  pId    = new OWL::TStatic(this, NSE_CODE_ID) ;

  pCapsLockMode = new NSModeStatic(pContexte, this, NSE_CAPS) ;

  pQuickStart = new OWL::TButton(this, NSE_MANUAL) ;
}
catch (...)
{
	erreur("Exception UserLoginDialog ctor.", standardError, 0) ;
}
}

UserLoginNTiersDialog::~UserLoginNTiersDialog()
{
  delete pLogs ;
  delete pPass ;
  delete pUsers ;
  delete pPassL ;
  delete pId ;
  delete pQuickStart ;
  delete pCapsLockMode ;
}

void
UserLoginNTiersDialog::SetupWindow()
{
	string ps = string("Entering SetupWindow for user login") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	TDialog::SetupWindow() ;

  string titleText = pContexte->getSuperviseur()->getText("loginDialog", "userIdentification") ;
  string loginText = pContexte->getSuperviseur()->getText("loginDialog", "userLogin") ;
  string passwText = pContexte->getSuperviseur()->getText("loginDialog", "userPassword") ;
  string quickText = pContexte->getSuperviseur()->getText("loginDialog", "openQuickStartManual") ;

  pPassL->SetText(passwText.c_str()) ;
  pId->SetText(loginText.c_str()) ;
  SetCaption(titleText.c_str()) ;
  pQuickStart->SetCaption(quickText.c_str()) ;

/*
	VecteurString result ;

	pContexte->pPilot->invokeService(NautilusPilot::SERV_USER_LIST.c_str(), LOGIN, result);
	int iNbUtils = 0 ;
  if (!(result.empty()))
  {
		EquiItemIter iter = result.begin() ;
		while( iter != result.end() )  //loop on the nodes
		{
			const char* aLogin = (*iter)->c_str() ;

      NSBasicAttributeArray AttrArray ;
			AttrArray.push_back(new NSBasicAttribute(LOGIN, *(*iter))) ;
			NSPersonsAttributesArray AttrUserList ;
			bool res = pContexte->pPilot->invokeService2ReturnElements(NautilusPilot::SERV_LOGIN.c_str(),
            pUtilChoisi->pGraphPerson->pDataGraph, &AttrUserList, &AttrArray) ;
           // "login", szLogin, "password", sPassEncrypt.c_str(), NULL);

			if ((res) && (!AttrUserList.empty()))
			{
				NSBasicAttributeArray ResultArray(*(*(AttrUserList.begin()))) ;
				// on remet login et password car ils ne sont pas retournés
				string sPasswordType = ResultArray.getAttributeValue(PASSWDTYPE) ;

        if (sPasswordType.find("D") == string::npos)
      	{
					pLogs->AddString(aLogin) ;
					iNbUtils++ ;
        }
      }
			iter++ ;
    }
	}
*/

	NSPersonsAttributesArray ListArray ;

  ps = string("Calling Pilot for users list") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	pContexte->getSuperviseur()->getPilot()->personsList(NautilusPilot::SERV_USER_LIST_WITH_ROLES.c_str(), "user", &ListArray, NULL) ;
	int iNbUtils = 0 ;
  if (false == ListArray.empty())
  {
		for (NSPersonsAttributeIter iterUser = ListArray.begin() ; ListArray.end() != iterUser ; iterUser++)
		{
    	string sPassType = (*iterUser)->getAttributeValue(PASSWDTYPE) ;

      if ((string("") == sPassType) || (string::npos == sPassType.find("D")))
      {
      	string sLogin = (*iterUser)->getAttributeValue(LOGIN) ;
      	pLogs->AddString(sLogin.c_str()) ;
        iNbUtils++ ;
    	}
		}
	}

  pLogs->SetSelIndex(0) ;

  // iNbUtils == 1 : administrator only : put focus on pswd
  // iNbUtils == 2 : administrator + user : select user and put focus on pswd
	//
  if      (1 == iNbUtils)
		PostMessage(WM_NEXTDLGCTL, (WPARAM)pPass->HWindow, TRUE) ;
  else if (2 == iNbUtils)
	{
		int iLen = pLogs->GetStringLen(0) ;
    char* pcString = new char[iLen + 1] ;
		int iResult = pLogs->GetString(pcString, 0) ;

    if ((iResult > 0) && (string(pcString) == string("Administrateur")))
			pLogs->SetSelIndex(1) ;

    delete[] pcString ;

    // pPass->SetFocus() ;
    PostMessage(WM_NEXTDLGCTL, (WPARAM)pPass->HWindow, TRUE) ;
  }

  ps = string("Leaving SetupWindow for user login") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}

// -----------------------------------------------------------------------------
void
UserLoginNTiersDialog::CmOk()
{
try
{
	string ps = string("Entering Ok response function for user login") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

	char far szLogin[UTI_LOGIN_LEN + 1] ;
  memset(szLogin, 0, UTI_LOGIN_LEN + 1) ;
	pLogs->GetSelString(szLogin, UTI_LOGIN_LEN + 1) ;

	if ('\0' == szLogin[0])
	{
		string sErrorText = pContexte->getSuperviseur()->getText("userManagement", "loginNeeded") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return ;
	}

	string sPass = pPass->GetText(UTI_CODE_LEN) ;
	if (string("") == sPass)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("userManagement", "passwordNeeded") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, 0) ;
		return ;
	}

	// chiffrage du mot de passe à l'aide de l'algo MD5
	string sPassEncrypt = MD5_encrypt(sPass) ;

  ps = string("Checking user login + password validity") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	//char far szPassEncrypt[UTI_CODE_LEN + 1] ;
	//sprintf(szPassEncrypt, "%s", sPassEncrypt.c_str()) ;
	NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(LOGIN , string(szLogin))) ;
	AttrArray.push_back(new NSBasicAttribute(PASSWORD , sPassEncrypt)) ;
	NSPersonsAttributesArray AttrUserList ;

  bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_LOGIN.c_str(),
                                                  &AttrUserList, &AttrArray) ;

	// bool res = pContexte->pPilot->invokeService2ReturnElements(NautilusPilot::SERV_LOGIN.c_str(),
  //          pUtilChoisi->pGraphPerson->pDataGraph, &AttrUserList, &AttrArray) ;
           // "login", szLogin, "password", sPassEncrypt.c_str(), NULL);

	if ((false == res) || (AttrUserList.empty()))
	{
		string sErrorText = pContexte->getSuperviseur()->getPilot()->getErrorMessage() + pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return ;
	}

	NSBasicAttributeArray ResultArray(*(*(AttrUserList.begin()))) ;
	// on remet login et password car ils ne sont pas retournés
	ResultArray.setAttributeValue(LOGIN, string(szLogin)) ;
	ResultArray.setAttributeValue(PASSWORD, sPassEncrypt) ;

	// pResultArray->setAttributeValue(USERTYPE, "UA");

	// Note : à laisser ici pour pouvoir inspecter
	string sLogin                = ResultArray.getAttributeValue(LOGIN);
	string sPasswd               = ResultArray.getAttributeValue(PASSWORD);
	string sUserType             = ResultArray.getAttributeValue(USERTYPE);
	string sPasswordType         = ResultArray.getAttributeValue(PASSWDTYPE);
	string sDatePasswordCreation = ResultArray.getAttributeValue(STARTDATE);
	string sValidityDuration     = ResultArray.getAttributeValue(DAYSNB);

	pUtilChoisi->SetUtilisateurChoisi(&ResultArray) ;

	TDialog::CmOk() ;

  ps = string("Leaving Ok response function for user login") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;
}catch (...){
	erreur("Exception UserLoginNTiersDialog::CmOk", standardError, 0) ;
}
}
void
UserLoginNTiersDialog::CmLoginDblClk(WPARAM Cmd)
{
	pPass->SetFocus() ;
}

void
UserLoginNTiersDialog::CmManual()
{
  bool bCardio = false ;
  bool bGastro = false ;

  pContexte->getSuperviseur()->setAideCorps(string("rapide.htm")) ;

  // Get modules
  //
  NSRefDocument refDoc((TDocument*) 0, pContexte) ;
  NSModuleArray modules ;
  if ((false == refDoc.GetModules(&modules)) || modules.empty())
  {
    pContexte->getSuperviseur()->NavigationAideEnLigne() ;
    return ;
  }

  for (NSModuInfoArrayIter it = modules.begin() ; modules.end() != it ; it++)
  {
    string sCode = (*it)->getLexique() ;
    string sCodS = string("") ;
    pContexte->getDico()->donneCodeSens(&sCode, &sCodS) ;

    if      ((string("GECHN") == sCodS) || (string("GECHZ") == sCodS))
      bCardio = true ;
    else if ((string("GDUOB") == sCodS) || (string("GCOLL") == sCodS))
      bGastro = true ;
  }

  pContexte->getSuperviseur()->setAideIndex(string("")) ;

  if      (bCardio)
    pContexte->getSuperviseur()->setAideCorps(string("rapide_cardio.htm")) ;
  else if (bGastro)
    pContexte->getSuperviseur()->setAideCorps(string("rapide_gastro.htm")) ;

  pContexte->getSuperviseur()->NavigationAideEnLigne() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSPropDocDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSPropDocDialog, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE;

NSPropDocDialog::NSPropDocDialog(TWindow* parent, string titre, NSDocumentInfo* pDocumInfo, NSContexte* pCtx)
                :NSUtilDialog(parent, pCtx, "IDD_PROPRIETES")
{
	sTitre = titre ;
	pInfos = pDocumInfo ;
	pProp = new NSUtilEdit(pContexte, this, IDC_PROP_EDIT) ;  // textLimit = 0
}

NSPropDocDialog::~NSPropDocDialog(){
	delete pProp ;
}

void
NSPropDocDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	string sLang = pContexte->getUserLanguage() ;

	if ((NSDocumentInfo*) NULL == pInfos)
		return ;

	string sText = pContexte->getSuperviseur()->getText("documentProperties", "title") + string(" ") + sTitre + string("\r\n") ;
	sText += pContexte->getSuperviseur()->getText("documentProperties", "documentType") + string(" ") + pInfos->getTypeSem() + string("\r\n") ;
	sText += pContexte->getSuperviseur()->getText("documentProperties", "documentID") + string(" ") + pInfos->getID() + string("\r\n") ;

	string sDateAffiche = donne_date(pInfos->getCreDate(), sLang) ;
	sText += pContexte->getSuperviseur()->getText("documentProperties", "creationDate") + string(" ") + sDateAffiche + string("\r\n") ;

  sDateAffiche = donne_date(pInfos->getDateExm(), sLang) ;
	sText += pContexte->getSuperviseur()->getText("documentProperties", "executionDate") + string(" ") + sDateAffiche + string("\r\n") ;

  string sCreateur = string("") ;
  string sCreatorId = pInfos->getCreator() ;
  if (string("") != sCreatorId)
  {
    NSPersonInfo* pPerson = pContexte->getPersonArray()->getPerson(pContexte, sCreatorId, pidsUtilisat) ;
    if (pPerson)
      sCreateur = pPerson->getCivilite() ;
  }

	sText += pContexte->getSuperviseur()->getText("documentProperties", "author") + string(" ") + sCreateur + string("\r\n") ;

	if (string("") != pInfos->getLocalis())		sText += pContexte->getSuperviseur()->getText("documentProperties", "filePath") + string(" ") + pInfos->getLocalis() + string("\r\n") ;

	if (string("") != pInfos->getFichier())
  	sText += pContexte->getSuperviseur()->getText("documentProperties", "fileName") + string(" ") + pInfos->getFichier() + string("\r\n") ;

  if (string("") != pInfos->getTemplate())
  	sText += pContexte->getSuperviseur()->getText("documentProperties", "templateName") + string(" ") + pInfos->getTemplate() + string("\r\n") ;

  if (string("") != pInfos->getEntete())  	sText += pContexte->getSuperviseur()->getText("documentProperties", "headerName") + string(" ") + pInfos->getEntete() + string("\r\n") ;

  sText += pContexte->getSuperviseur()->getText("documentProperties", "importance") + string(" ") + pInfos->getInteret() + string("\r\n") ;
  sText += pContexte->getSuperviseur()->getText("documentProperties", "Visible") + string(" ") ;
  if (pInfos->estVisible())  	sText += pContexte->getSuperviseur()->getText("generalLanguage", "yes") ;
  else
  	sText += pContexte->getSuperviseur()->getText("generalLanguage", "no") ;

  // Additional information
  //
  ClasseStringVector* pAdditionalInformation = pInfos->getAdditionalInformation() ;
  if (pAdditionalInformation && (false == pAdditionalInformation->empty()))
  {
    sText += string("\r\n") ;

    string sLang = pContexte->getUserLanguage() ;

    for (iterClassString it = pAdditionalInformation->begin() ; pAdditionalInformation->end() != it ; it++)
    {
      string sConcept = (*it)->getItem() ;
      string sLabel   = string("") ;
      pContexte->getDico()->donneLibelle(sLang, &sConcept, &sLabel) ;
      if (string("") != sLabel)
        sLabel[0] = pseumaj(sLabel[0]) ;

      sText += sLabel ;

      string sQualifier = (*it)->getQualifier() ;
      if (string("") != sQualifier)
        sText += string(" : ") + sQualifier + string("\r\n") ;
    }
  }

	pProp->FormatLines(true) ;
	pProp->SetText(sText.c_str()) ;
}

voidNSPropDocDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSDocuBox
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSDocuBox, OWL::TListBox)
	EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDC_OUVRIR, Ouvrir),
  EV_COMMAND(CM_EDITER, Editer),
  EV_COMMAND(IDC_AJOUTER, Ajouter),
  EV_COMMAND(IDC_PARAMETRES, Parametres),
  EV_COMMAND(IDC_PROPRIETE, Proprietes),
  EV_COMMAND(IDC_DETRUIRE, Detruire),
END_RESPONSE_TABLE;

NSDocuBox::NSDocuBox(NSContexte* pCtx, ChoixChemiseDialog* pere)          :OWL::TListBox(pere, IDC_DOCUBOX), NSRoot(pCtx){
	pDlg = pere ;
}

NSDocuBox::~NSDocuBox(){
}

void
NSDocuBox::EvRButtonDown(uint /*modkeys*/, NS_CLASSLIB::TPoint& point)
{
	int hindex, count ;
	bool trouve = false ;
	NS_CLASSLIB::TRect itemRect ;

	// on doit selectionner l'item qui encadre le point clické
  hindex = GetTopIndex() ;
  count = GetCount() ;

	while ((!trouve) && (hindex < count))
	{
  	GetItemRect(hindex, itemRect) ;

    if ((point.y >= itemRect.Top()) && (point.y < itemRect.Bottom()))
    {
    	trouve = true ;
      break ;
    }

    hindex++ ;
	}

	if (trouve)
  	SetSelIndex(hindex) ;
	else
  {
  	// on enleve la selection
    SetSelIndex(-1) ;
    return ;
	}

	TPopupMenu *menu = new TPopupMenu() ;
	NSDocumentInfo* pDocument = (*(pDlg->pDocusArray))[GetSelIndex()] ;

  menu->AppendMenu(MF_STRING, IDC_OUVRIR,    "Ouvrir") ;
  // if (!strcmp(pDocument->pDonnees->type, "TWHTM"))
  menu->AppendMenu(MF_STRING, CM_EDITER, "Editer") ;
  menu->AppendMenu(MF_STRING, IDC_PARAMETRES,"Paramètres") ;
  menu->AppendMenu(MF_STRING, IDC_PROPRIETE, "Propriétés") ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0) ;

  if (false == pDocument->estVisible())
      menu->AppendMenu(MF_STRING, IDC_AJOUTER, "Ajouter à l'historique") ;
  else
      menu->AppendMenu(MF_GRAYED, 0,           "Ajouter à l'historique") ;
  menu->AppendMenu(MF_SEPARATOR, 0, 0);

  menu->AppendMenu(MF_STRING, IDC_DETRUIRE,  "Détruire") ;

  ClientToScreen(point) ;
  menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
  delete menu ;
}

voidNSDocuBox::Ouvrir()
{
	// on est sur qu'il s'agit d'un index valide
	pDlg->DocumentChoisi = GetSelIndex() + 1 ;
	pDlg->CmOk() ;
}

voidNSDocuBox::Editer()
{
	// on est sur qu'il s'agit d'un index valide
	pDlg->DocumentChoisi = GetSelIndex() + 1 ;
	pDlg->bEditer = true ;
	pDlg->CmOk() ;
}
voidNSDocuBox::Ajouter()
{
	// on est sur qu'il s'agit d'un index valide
	int docChoisi = GetSelIndex() + 1 ;
	pDlg->AjouteDocument(docChoisi) ;
	// on rafraichit la listbox
	pDlg->CmSelectChemise(0) ;
}

voidNSDocuBox::Detruire()
{
	// on est sur qu'il s'agit d'un index valide
	NSDocumentInfo* pDocument = (*(pDlg->pDocusArray))[GetSelIndex()] ;
	char far        titre[1024] ;

	GetString(titre, GetSelIndex()) ;
	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
	string sMessage = pContexte->getSuperviseur()->getText("documentHistoryManagement", "doYouReallyWantToSuppressTheDocument") + string(" ") + string(titre) ;

	int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;

	if (retVal != IDYES)		return ;  // Destruction du document
  //
  pContexte->getPatient()->DetruireDocument(pDocument);
  pContexte->getPatient()->getDocHis()->Rafraichir(pDocument, 0);

  //  // Mise à jour de l'array  //
  SetSelIndex(-1);
  pDlg->CmSelectChemise(0);
}

voidNSDocuBox::Parametres()
{
	NSDocumentInfo* pDocInfo = (*(pDlg->pDocusArray))[GetSelIndex()] ;
	NSDocumentInfo* pHtmlInfo = 0 ;

	// cas des html dynamiques : on instancie le pHtmlInfo du document
/*
  if (string("HD") == string(pDocInfo->getType(), 0, 2))
  {
  	// on retrouve le document brut
    if (!pContexte->ChercheComposition(&pDocInfo, &pHtmlInfo))
    {      string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "theReferenceDocumentCannotBeFound") ;
      sErrorText += string(" (") + pDocInfo->getPatient() + string(" ") + pDocInfo->getDocument() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
      return ;
    }
	}
*/

  // on crée un docref virtuel pour pouvoir lancer le dialogue des paramètres  // on ne lance pas validefichier comme dans NSSuper car on n'ouvre pas le document
  NSRefDocument* pDocRef = new NSRefDocument(0, pDocInfo, pHtmlInfo, pContexte, true) ;

	if (pDocRef->ParamDoc())	{
		// on doit prendre le pDocInfo du pDocRef pour tenir compte des modifs
    if (pDocRef->_pHtmlInfo)
    	pContexte->getPatient()->getDocHis()->Rafraichir(pDocRef->_pHtmlInfo, 0) ;
    else
    	pContexte->getPatient()->getDocHis()->Rafraichir(pDocRef->_pDocInfo, 0) ;
    //
    // Mise à jour de l'array
    //
    SetSelIndex(-1) ;
    pDlg->CmSelectChemise(0) ;
	}

  // IMPORTANT : on doit remettre pDocInfo et pHtmlInfo à 0
  // pour éviter que ~NSRefDocument ne remette à jour l'historique
  pDocRef->_pDocInfo  = 0 ;
  pDocRef->_pHtmlInfo = 0 ;
  delete pDocRef ;
}

voidNSDocuBox::Proprietes()
{
	char far titre[255] ;
	NSDocumentInfo* pDocument = (*(pDlg->pDocusArray))[GetSelIndex()] ;

	GetString(titre, GetSelIndex());

	NSPropDocDialog* pPropDlg = new NSPropDocDialog(pContexte->GetMainWindow(),
                                                    string(titre), pDocument, pContexte) ;
	pPropDlg->Execute() ;
	delete pPropDlg ;
}


// -----------------------------------------------------------------//
//  Méthodes de ChoixChemiseDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixChemiseDialog, NSUtilDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_CHEMISEBOX, LBN_SELCHANGE, CmSelectChemise),
	EV_CHILD_NOTIFY_AND_CODE(IDC_DOCUBOX, 	  LBN_SELCHANGE, CmSelectDocument),
	EV_CHILD_NOTIFY_AND_CODE(IDC_CHEMISEBOX, LBN_DBLCLK, CmChemiseDblClk),
	EV_CHILD_NOTIFY_AND_CODE(IDC_DOCUBOX, 	  LBN_DBLCLK, CmDocumentDblClk),
	EV_COMMAND(IDCANCEL,	CmCancel),
END_RESPONSE_TABLE;

ChoixChemiseDialog::ChoixChemiseDialog(TWindow* pere, NSContexte* pCtx)                   :NSUtilDialog(pere, pCtx, "IDD_CHEMISE")
{
	pChemiseBox    = new TListBox(this, IDC_CHEMISEBOX);
	pDocuBox       = new NSDocuBox(pCtx, this) ;
	pChemisesArray = new NSChemiseArray;
	pDocusArray    = new NSDocHistoArray;

	// Edition : remplace ouvrir pour les fichiers word
	bEditer = false ;

	// fichiers d'aide	sHindex = "" ;
	sHcorps = "Dossier_patient.html" ;
}

//---------------------------------------------------------------------------//  Function: ChoixChemiseDialog::~ChoixChemiseDialog()
//
//  Arguments:	  Aucun
//
//  Description: Destructeur, ouvre la chemise ou le document sélectionné
//
//  Returns:     Rien
//---------------------------------------------------------------------------
ChoixChemiseDialog::~ChoixChemiseDialog()
{
	//
	// Si un document a été choisi
	//
	if (DocumentChoisi)
		OuvreDocument(DocumentChoisi);
	// else if (ChemiseChoisie)
	//  	for (int i = 0; i < pDocusArray->size(); i++)
	//  		OuvreDocument(i + 1);

	delete pChemiseBox;
	delete pDocuBox;
	delete pChemisesArray;
	delete pDocusArray;
}

//---------------------------------------------------------------------------
//  Function: ChoixChemiseDialog::OuvreDocument(int NumDoc)
//
//  Arguments:	  Numéro du document à ouvrir
//
//  Description: Attache le document au modèle Document/Visualisation
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
ChoixChemiseDialog::OuvreDocument(int NumDoc)
{
  NSDocumentInfo* pDocument = (*pDocusArray)[NumDoc-1] ;
  if (bEditer)
  {
    // cas des fichiers word...
    pContexte->getPatient()->getDocHis()->AutoriserEdition(pDocument);
  }
  else
  {
    //demander l'autorisation de l'ouverture de ce document, s'il est déjà ouvert,
    //il suffit de l'activer
    pContexte->getPatient()->getDocHis()->AutoriserOuverture(pDocument);
  }
}

//---------------------------------------------------------------------------//  Function: ChoixChemiseDialog::AjouteDocument(int NumDoc)
//
//  Arguments:	  Numéro du document à ajouter
//
//  Description: Ajoute le document à l'historique en le rendant visible
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
ChoixChemiseDialog::AjouteDocument(int NumDoc)
{
	NSDocumentInfo* pDocument = (*pDocusArray)[NumDoc-1] ;

	// On regarde d'abord si le document n'appartient pas déjà à l'historique
	if (pDocument->estVisible())
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentHistoryManagementErrors", "thisDocumentIsAlreadyPartOfTheHistoryView") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), warningError, 0, GetHandle()) ;
    return ;
	}

	// demander l'autorisation de l'ouverture de ce document, s'il est déjà ouvert,
	// il suffit de l'activer
	pContexte->getPatient()->getDocHis()->AjouteDocument(pDocument) ;
}

//---------------------------------------------------------------------------//  Function: ChoixChemiseDialog::SetupWindow()
//  Etat initial : le patient a au moins une chemise
//  Arguments:	Aucun
//
//  Description:
//            Initialise la boite de liste des chemises
//  Returns:
//            Rien
//---------------------------------------------------------------------------
void ChoixChemiseDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
	string sLang = pContexte->getUserLanguage() ;
  if ((NULL == pContexte->getPatient()) || (NULL == pContexte->getPatient()->getDocHis()))
    return ;

  NSDocumentHisto* pLibChem = pContexte->getPatient()->getDocHis()->getLibChem() ;
  if (NULL == pLibChem)
    return ;

  NSPatPathoArray ChemPPT(pContexte->getSuperviseur()) ;
  pLibChem->initFromPatPatho(&ChemPPT) ;

  if (ChemPPT.empty())
    return ;

  // on doit parcourir la librairie pour charger l'array des chemises
  PatPathoIter iter = ChemPPT.begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((ChemPPT.end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens    = string("") ;
    string sElemLex = (*iter)->getLexique() ;
    pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

    if (string("0CHEM") == sSens)
    {
      string sNodeChem = (*iter)->getNode() ;
      string sNom      = string("") ;
      string sDate     = string("") ;
      iter++ ;

      // on charge les données de la chemise
      while ((ChemPPT.end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
        sElemLex = (*iter)->getLexique() ;
        pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

        // nom de la chemise
        if (string("0INTI") == sSens)
        {
          iter++ ;

          while ((ChemPPT.end() != iter) && ((*iter)->getColonne() > iColBase+2))
          {
            // on cherche ici un texte libre
            sElemLex = (*iter)->getLexique() ;

            if (sElemLex == string("£?????"))
              sNom = (*iter)->getTexteLibre() ;

            iter++ ;
          }
        }
        // Dates
        else if (string("KOUVR") == sSens)
        {
          iter++ ;
          int iLigneBase = (*iter)->getLigne() ;
          // gereDate* pDate = new gereDate(pContexte);
          string sUnite  = "" ;
          string sFormat = "" ;
          string sValeur = "" ;
          string sTemp   = "" ;

          while ((ChemPPT.end() != iter) &&
                         ((*iter)->getLigne() == iLigneBase))
          {
            string sLexique = (*iter)->getLexique() ;
            if ((string("") != sLexique) && ('£' == sLexique[0]))
            {
              sTemp   = (*iter)->getLexique() ;
              pContexte->getDico()->donneCodeSens(&sTemp, &sFormat) ;
              sValeur = (*iter)->getComplement() ;
              sTemp   = (*iter)->getUnit() ;
              pContexte->getDico()->donneCodeSens(&sTemp, &sUnite) ;
              iter++ ;
              break ;
            }

            iter++ ;
          }

          // sFormat est du type £D0;03
          if ((sFormat != "") && (sFormat[1] == 'T') &&
                      (sValeur != ""))
          {
            if (sUnite == "2DA02")
              sDate = string(sValeur, 0, 8) ;
          }
        }
        else
          iter++ ;
      }

      if (string("") != sNom)
      {
        NSChemiseInfo *pChemInfo = new NSChemiseInfo() ;

        pChemInfo->_sNodeChemise       = sNodeChem ;
        pChemInfo->_Donnees._sNom      = sNom ;
        pChemInfo->_Donnees._sCreation = sDate ;

        // Setting folder's title
        // Préparation de l'intitulé de la chemise
        //
        string sFolderLabel = pChemInfo->donneIntitule(sLang) ;
        pChemiseBox->AddString(sFolderLabel.c_str()) ;
        pChemisesArray->push_back(pChemInfo) ;
      }
    }
    else
      iter++ ;
  }}

//---------------------------------------------------------------------------//  Function :		ChoixChemiseDialog::CmSelectChemise(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Initialise la boite de liste des documents pour la chemise
//						sélectionnée
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------

void ChoixChemiseDialog::CmSelectChemise(WPARAM Cmd){
	//
	// Effacement de la boite de liste des documents
	//
	pDocuBox->ClearList();

	//	// Récupération de l'indice de la chemise sélectionnée
	//
	ChemiseChoisie = pChemiseBox->GetSelIndex() + 1 ;
	DocumentChoisi = 0 ;

	//	// Récupération de la NSChemiseInfo correspondante
	//
	NSChemiseInfo* ChemiseEnCours = (*pChemisesArray)[ChemiseChoisie - 1] ;

	// vidange de pDocusArray
  //
  if (false == pDocusArray->empty())
	{
		DocumentIter iter ;
		for (iter = pDocusArray->begin() ; pDocusArray->end() != iter ; )
		{
  		delete *iter ;
    	pDocusArray->erase(iter) ;
		}
  }

	if (NULL == ChemiseEnCours)
		return ;

	//	// Remplissage de DocuBox avec les documents du patient
	//

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
  	return ;

	NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;
  VecteurString VectString ;
	string sNodeChemise = ChemiseEnCours->_sNodeChemise ;

	// chargement de tous les documents de la chemise
	VectString.vider() ;
	pGraphe->TousLesVrais(sNodeChemise, NSRootLink::docFolder, &VectString) ;

	if (VectString.empty())
		return ;

	string sLang = pContexte->getUserLanguage() ;

  for (EquiItemIter i = VectString.begin(); VectString.end() != i ; i++)
  {
    NSDocumentHisto* pNSDocumentHisto = new NSDocumentHisto(new NSDocumentInfo(*(*i), pContexte, pGraphManager)) ;
    pDocusArray->push_back(pNSDocumentHisto) ;
  }

  // Récupération des informations des documents
  DocumentIter iterDoc = pDocusArray->begin() ;
  bool bOk ;

  while (pDocusArray->end() != iterDoc)  {
    NSPatPathoArray DocuPPT(pContexte->getSuperviseur()) ;
    (*iterDoc)->initFromPatPatho(&DocuPPT) ;

    DocuPPT.vider() ;

    VectString.vider() ;
    bOk = false ;

    // On remonte le lien data du méta-document
    pGraphe->TousLesVrais((*iterDoc)->getCodeDocMeta(), NSRootLink::docData, &VectString) ;
    if (false == VectString.empty())
    {
      string sCodeDocData = *(*(VectString.begin())) ;
      string sCodePat = string(sCodeDocData, 0, PAT_NSS_LEN) ;
      string sCodeDoc = string(sCodeDocData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

      (*iterDoc)->setPatient(sCodePat) ;
      (*iterDoc)->setDocument(sCodeDoc) ;

      if (((*iterDoc)->ParseMetaDonnees()) &&
                  ((*iterDoc)->DonnePatPatho(&DocuPPT, pGraphManager)))
        bOk = true ;
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
        DocuPPT.ajoutePatho("ZDOCU1", 0) ;
        bOk = true ;
      }
    }

    //problème de récupération de la patpatho (ou de parsing) : le document
    //ne figure pas dans la liste

    if (false == bOk)    {
    	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "cannotFindDocumentInformation") ;
      sErrorText += string(" (") + (*iterDoc)->getPatient() + string(" ") + (*iterDoc)->getDocument() + string(")") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
      erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;

      delete *iterDoc ;
      pDocusArray->erase(iterDoc) ;
    }
    else
    {
      //
      // Préparation de l'intitulé du document
      //
      string sAffiche = (*iterDoc)->getDocName() ;
      strip(sAffiche, stripBoth) ;

      if (string("") != (*iterDoc)->getDateDoc())        sAffiche += string(" - ") + donne_date((*iterDoc)->getDateDoc(), sLang) ;

      //      // Ajout du document à la DocuBox s'il n'est pas détruit
      //

      // Note : on doit rajouter encore la gestion des suppressions de document en MUE
      // if (pDocument->pDonnees->tranDel[0] == '\0')      // {
      pDocuBox->AddString(sAffiche.c_str()) ;
      // }

      (*iterDoc)->setPatPatho(&DocuPPT) ;

      iterDoc++ ;
    }
  }
}

//---------------------------------------------------------------------------//  Function :		ChoixChemiseDialog::CmChemiseDblClk(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Sélectionne une chemise entière
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixChemiseDialog::CmChemiseDblClk(WPARAM Cmd)
{
}

//---------------------------------------------------------------------------
//  Function :		ChoixChemiseDialog::CmSelectDocument(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Sélectionne un document de la chemise en cours
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixChemiseDialog::CmSelectDocument(WPARAM Cmd)
{
	//
	// Récupération de l'indice du document sélectionné
	//
	DocumentChoisi = pDocuBox->GetSelIndex() + 1;
}

//---------------------------------------------------------------------------
//  Function :		ChoixChemiseDialog::CmDocumentDblClk(WPARAM Cmd)
//
//  Arguments :	Cmd : WPARAM retourné par Windows
//
//  Description :	Sélectionne une chemise entière
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixChemiseDialog::CmDocumentDblClk(WPARAM Cmd)
{
	NSUtilDialog::CmOk();
}

//---------------------------------------------------------------------------
//  Function :		ChoixChemiseDialog::CmCancel()
//
//  Arguments :	Aucun
//
//  Description :	Annule ChemiseChoisie et DocumentChoisi et appelle Cancel()
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixChemiseDialog::CmCancel()
{
	DocumentChoisi = 0;
	ChemiseChoisie = 0;
	NSUtilDialog::CmCancel();
}

/****************************************************************/
/**      Classes pour la liste des patients à reconvoquer      **/
/****************************************************************/
DEFINE_RESPONSE_TABLE1(NSDateConvocDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSDateConvocDialog::NSDateConvocDialog(TWindow* parent, NSContexte* pCtx)
                   :NSUtilDialog(parent, pCtx, "IDD_DATECONVOC")
{
	sDate1 = "" ;
	sDate2 = "" ;
	pDate1 = new NSUtilEditDate(pContexte, this, IDC_DATECONVOC_DATE1) ;
	pDate2 = new NSUtilEditDate(pContexte, this, IDC_DATECONVOC_DATE2) ;
}

NSDateConvocDialog::~NSDateConvocDialog()
{
	delete pDate1 ;
	delete pDate2 ;
}

void
NSDateConvocDialog::SetupWindow()
{
	struct date	dateSys ;
	char	dateJour[9] = "" ;

	NSUtilDialog::SetupWindow() ;

	getdate(&dateSys) ;
	sprintf(dateJour, "%4d%02d%02d", dateSys.da_year, dateSys.da_mon, dateSys.da_day) ;

	pDate1->setDate(dateJour) ;
	pDate2->setDate(dateJour) ;
}

void
NSDateConvocDialog::CmOk()
{
	pDate1->getDate(&sDate1) ;
	pDate2->getDate(&sDate2) ;

	NSUtilDialog::CmOk() ;
}

void
NSDateConvocDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

//-----------------------------------------------------------------------
// fonction globale de tri :
//			afficher les patients dans ordre de date de reconvocation
//-----------------------------------------------------------------------
bool
triParDates(NSPersonInfo* a, NSPersonInfo* b)
{
	if (a->getConvoc() < b->getConvoc())
		return true ;
	return false ;
}

/******************************************************************/

DEFINE_RESPONSE_TABLE1(NSListeConvocDialog, NSUtilDialog)
	EV_LV_DISPINFO_NOTIFY(IDC_LISTECONVOC_LW, LVN_GETDISPINFO, DispInfoListe),
	EV_COMMAND(IDC_LISTECONVOC_IMP, CmImprimerListe),
	EV_COMMAND(IDC_LISTECONVOC_LETTRE, CmImprimerLettre),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSListeConvocDialog::NSListeConvocDialog(TWindow* parent, NSContexte* pCtx, NSVarConvoc* pVarConvoc)
                    :NSUtilDialog(parent, pCtx, "IDD_LISTECONVOC")
{
try
{
	pDate1 = new NSUtilEditDate(pContexte, this, IDC_LISTECONVOC_DATE1) ;
	pDate2 = new NSUtilEditDate(pContexte, this, IDC_LISTECONVOC_DATE2) ;
	pListe = new NSListeConvocWindow(this, IDC_LISTECONVOC_LW) ;
	pTabSelect = 0 ;
	nbConvoc = 0 ;

	pVar = pVarConvoc ;
	bImprimerListe = false ;
	bImprimerLettre = false ;

  string _sLang = pContexte->getUserLanguage() ;
}catch (...)
{
	erreur("Exception NSListeConvocDialog ctor.", standardError, 0) ;
}
}

NSListeConvocDialog::~NSListeConvocDialog()
{
	// Attention on ne delete pas l'objet pointé par pVar
	// car cet objet doit rester résidant
	delete pDate1 ;
	delete pDate2 ;
	delete pListe ;
	if (NULL != pTabSelect)
		delete[] pTabSelect ;
}

void
NSListeConvocDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	pDate1->setDate(pVar->_sDate1) ;
	pDate2->setDate(pVar->_sDate2) ;

	if (InitPatientArray())
	{
  	InitListe() ;
    AfficheListe() ;
	}
}

bool
NSListeConvocDialog::InitPatientArray()
{
try
{
	if (NULL == pVar)
		return false ;

	SetCursor(0, IDC_WAIT) ;

	(pVar->_aPatientArray).vider() ;
	pVar->_nbPatient = 0 ;

  NSBasicAttributeArray AttrArray  ;

	// We could do this, but the comparison would not be made on the same node
  //
	// AttrArray.push_back(new NSBasicAttribute(APPOINTMENT_DATE, string("${>=}$") + pVar->sDate1)) ;
  // AttrArray.push_back(new NSBasicAttribute(APPOINTMENT_DATE, string("${<=}$") + pVar->sDate2)) ;

  string sRequest = string(" $[]$ >= '") + pVar->_sDate1 + string("' AND $[]$ <= '") + pVar->_sDate2 + string("'") ;
  AttrArray.push_back(new NSBasicAttribute(APPOINTMENT_DATE, sRequest)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, APPOINTMENT_DATE));

  NSPersonsAttributesArray PatiensList ;

  bool bListOk = pContexte->getSuperviseur()->getPilot()->personList((NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS).c_str(), &PatiensList, &AttrArray) ;

	if (false == bListOk)
	{
  	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    std::string tempError   = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
    if( tempMessage != "")
    	::MessageBox(GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    if( tempError != "")
    	::MessageBox(GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
    SetCursor(0, IDC_ARROW) ;
    return false ;
	}

	// Nobody found
	//
	if (PatiensList.empty())
  {
		erreur("Aucune date de reconvocation dans l'intervalle.", warningError, 0, GetHandle()) ;
    SetCursor(0, IDC_ARROW) ;
    return true ;
	}

  for (NSPersonsAttributeIter iterPatient = PatiensList.begin() ; PatiensList.end() != iterPatient ; iterPatient++)
	{
  	NSPersonInfo bufferInfo(pContexte->getSuperviseur(), *iterPatient) ;
    NSPersonInfo* pNewPerson = new NSPersonInfo(pContexte->getSuperviseur(), pContexte, bufferInfo.getNss(), pidsPatient) ;
    pNewPerson->setConvoc(bufferInfo.getConvoc()) ;
  	(pVar->_aPatientArray).push_back(pNewPerson) ;
    pVar->_nbPatient++ ;
	}

	if (pVar->_aPatientArray.empty())
  {
  	SetCursor(0, IDC_ARROW) ;
		return true ;
  }

  SetCursor(0, IDC_ARROW) ;

	sort((pVar->_aPatientArray).begin(), (pVar->_aPatientArray).end(), triParDates) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSListeConvocDialog::InitPatientArray.", standardError, 0, GetHandle()) ;
  return false ;
}
}

void
NSListeConvocDialog::InitListe()
{
	TListWindColumn colPat("Patient", 200, TListWindColumn::Left, 0) ;
	pListe->InsertColumn(0, colPat) ;
	TListWindColumn colDate("Date", 200, TListWindColumn::Left, 1) ;
	pListe->InsertColumn(1, colDate) ;
}

voidNSListeConvocDialog::AfficheListe()
{
	pListe->DeleteAllItems() ;

  static char buffer[1024] ;

	for (int i = pVar->_nbPatient - 1; i >= 0; i--)
	{
  	strcpy(buffer, (pVar->_aPatientArray)[i]->getNomLong().c_str()) ;
  	TListWindItem Item(buffer, 0) ;
    pListe->InsertItem(Item) ;
	}
}

voidNSListeConvocDialog::DispInfoListe(TLwDispInfoNotify& dispInfo)
{
	const int BufLen = 255 ;
	static char buffer[BufLen] ;
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;

  string sLang = pContexte->getUserLanguage() ;

	int index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())	{
  	case 1: 	// date de reconvocation
    	{
      	string sTimeFormat = pContexte->getSuperviseur()->getText("0localInformation", "timeFormat") ;
        string sFormatedTime = getFormatedTime(((pVar->_aPatientArray)[index])->getConvoc(), sLang, sTimeFormat, "") ;
    		strcpy(buffer, sFormatedTime.c_str()) ;
      	dispInfoItem.SetText(buffer) ;
    	}
    	break ;
	}
}

voidNSListeConvocDialog::CmFichePatient(int itemSel)
{
	NSFichePatientDialog* pFPDlg = new NSFichePatientDialog(this, pContexte,
	                                            (pVar->_aPatientArray)[itemSel]) ;
	pFPDlg->Execute() ;
	delete pFPDlg ;
}

voidNSListeConvocDialog::CmImprimerListe()
{
	bImprimerListe = true;
	CmOk() ;
}

voidNSListeConvocDialog::CmImprimerLettre()
{
	pTabSelect = new int[pVar->_nbPatient] ;
	nbConvoc = pListe->GetSelIndexes(pTabSelect, pVar->_nbPatient) ;
	bImprimerLettre = true ;
	CmOk() ;
}

voidNSListeConvocDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

voidNSListeConvocDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListPatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListeConvocWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSListeConvocWindow::NSListeConvocWindow(NSListeConvocDialog* pere, int resId)                    :TListWindow(pere, resId){
	pDlg = pere ;
}

//---------------------------------------------------------------------------//  Function: NSListeConvocWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListeConvocWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmFichePatient(IndexItemSelect()) ;
}

//---------------------------------------------------------------------------
//  Function: NSListeConvocWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListeConvocWindow::IndexItemSelect()
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

// Retourne le tableau des items sélectionnés avec leurs indexs// Valeur de retour : Nombre d'items sélectionnés
int
NSListeConvocWindow::GetSelIndexes(int* pTabIndex, int tailleTab)
{
	int count = GetItemCount() ;
	int numSelect = 0 ;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    {
    	if (numSelect < tailleTab)
      {
      	pTabIndex[numSelect] = i ;
        numSelect++ ;
      }
    }

	return numSelect ;
}

//// Classe NSFichePatientDialog
//
DEFINE_RESPONSE_TABLE1(NSFichePatientDialog, NSUtilDialog)
	EV_COMMAND(IDC_ADMIN_MODIFDC, CmModifDateConvoc),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NSFichePatientDialog::NSFichePatientDialog(TWindow* parent, NSContexte* pCtx, NSPersonInfo* ppi)
                     :NSUtilDialog(parent, pCtx, "IDD_FICHEPATIENT")
{
	pSitFam  = new NSUtilEdit(pContexte, this, IDC_ADMIN_SITFAM, 15) ;
	pNbEnf 	 = new NSUtilEdit(pContexte, this, IDC_ADMIN_NBENF, 2) ;
	pSexe 	 = new NSUtilEdit(pContexte, this, IDC_ADMIN_SEXE, 8) ;
	pDateN 	 = new NSUtilEditDate(pContexte, this, IDC_ADMIN_DATEN) ;

	pNss 		 = new NSUtilEdit(pContexte, this, IDC_ADMIN_NSS, PAT_NSS_LEN) ;	pCode 	 = new NSUtilEdit(pContexte, this, IDC_ADMIN_CODE, PAT_CODE_LEN) ;
  pTelPor  = new NSUtilEdit(pContexte, this, IDC_ADMIN_TELPOR, PAT_TELEPOR_LEN) ;
  pTelBur  = new NSUtilEdit(pContexte, this, IDC_ADMIN_TELBUR, PAT_TELEBUR_LEN) ;

  pDateC 	 = new NSUtilEditDate(pContexte, this, IDC_ADMIN_DATEC) ;

  pAdresse = new NSUtilEdit(pContexte, this, IDC_ADMIN_ADR) ;  // textLimit = 0
  pTelDom  = new NSUtilEdit(pContexte, this, IDC_ADMIN_TELDOM, ADR_TELEPHONE_LEN) ;
  pEMail   = new NSUtilEdit(pContexte, this, IDC_ADMIN_EMAIL, ADR_MESSAGERIE_LEN) ;
  pFax     = new NSUtilEdit(pContexte, this, IDC_ADMIN_FAX, ADR_FAX_LEN) ;

  pListeCorresp = new TListBox(this, IDC_ADMIN_COR) ;

  pPatInfo = ppi ;
}

NSFichePatientDialog::~NSFichePatientDialog()
{
	delete pListeCorresp ;
  delete pFax ;
  delete pEMail ;
  delete pTelDom ;
  delete pAdresse ;
  delete pDateC ;
  delete pTelBur ;
  delete pTelPor ;
  delete pCode ;
  delete pNss ;
  delete pDateN ;
  delete pSexe ;
  delete pNbEnf ;
  delete pSitFam ;
}

void
NSFichePatientDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	AfficheDonneesPatient() ;
  AfficheDonneesAdresse() ;
  AfficheDonneesCorresp() ;
}

void
NSFichePatientDialog::AfficheDonneesPatient()
{
	SetCaption(pPatInfo->getNomLong().c_str()) ;

	if (pPatInfo->estMasculin())
  	pSexe->SetText("Masculin") ;
	else
  	pSexe->SetText("Féminin") ;

  // pSitFam->SetText((pPatInfo->donneSitFam()).c_str()) ;
  // pNbEnf->SetText((pPatInfo->donneNbEnfants()).c_str()) ;

  pDateN->setDate(pPatInfo->getNaissance()) ;

	pNss->SetText(pPatInfo->getNss().c_str()) ;
  // pCode->SetText(pPatInfo->getCode().c_str()) ;
  // pTelPor->SetText(pPatInfo->getTelepor().c_str()) ;
  // pTelBur->SetText(pPatInfo->getTelebur().c_str()) ;

	pDateC->setDate(pPatInfo->getConvoc()) ;
}

voidNSFichePatientDialog::AfficheDonneesAdresse()
{
/*
	string sAdresse ;

	pAdresse->FormatLines(true);
	pAdresse->SetText("");

	pTelDom->SetText(pPatInfo->pAdresseInfo->pDonnees->telephone);
	pFax->SetText(pPatInfo->pAdresseInfo->pDonnees->fax);
	pEMail->SetText(pPatInfo->pAdresseInfo->pDonnees->messagerie);

	sAdresse = pPatInfo->pAdresseInfo->StringAdresse();
	pAdresse->SetText(sAdresse.c_str());
*/
}

void
NSFichePatientDialog::AfficheDonneesCorresp()
{
/*
	string sCorresp;

	// on vide la liste si elle contient des items
    if (pListeCorresp->GetCount())
   	    pListeCorresp->ClearList();

    for (CorrespInfoIter i = pPatInfo->pCorrespArray->begin();
   							i != pPatInfo->pCorrespArray->end(); i++)
    {
        // on remplit la CorrespBox
        sCorresp = string((*i)->pDonnees->nom) + string(" ") + string((*i)->pDonnees->prenom);
        pListeCorresp->AddString(sCorresp.c_str());
    }
*/
}

void
NSFichePatientDialog::CmModifDateConvoc()
{
/*
	string sDateC;

    pDateC->getDate(&sDateC);
    strcpy(pPatInfo->pDonnees->convoc, sDateC.c_str());

    NSPatient* pPatient = new NSPatient(pContexte);

    pPatient->lastError = pPatient->open();
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à l'ouverture du fichier Patient.db", 0, pPatient->lastError, GetHandle());
		delete pPatient;
		return;
    }

    pPatient->lastError = pPatient->chercheClef((unsigned char*)(pPatInfo->pDonnees->nss),
													 					"",
													 					0,
													 					keySEARCHEQ,
                                                                        dbiWRITELOCK);
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à la recherche du Patient.", 0, pPatient->lastError, GetHandle());
        pPatient->close();
		delete pPatient;
		return;
    }

    pPatient->lastError = pPatient->getRecord();
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à lecture du fichier Patient.db", 0, pPatient->lastError, GetHandle());
        pPatient->close();
		delete pPatient;
		return;
    }

    // On remet à jour la date de reconvocation
    strcpy(pPatient->pDonnees->convoc, sDateC.c_str());

    pPatient->lastError = pPatient->modifyRecord(TRUE);
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à modification du fichier Patient.db", 0, pPatient->lastError, GetHandle());
        pPatient->close();
		delete pPatient;
		return;
    }

    pPatient->lastError = pPatient->close();
    if (pPatient->lastError != DBIERR_NONE)
    {
   	    erreur("Erreur à la fermeture du fichier Patient.db", 0, pPatient->lastError, GetHandle());
		delete pPatient;
		return;
    }

    MessageBox("Date de reconvocation enregistrée");*/
}
voidNSFichePatientDialog::CmOk()
{
	NSUtilDialog::CmOk() ;
}

/*************************************************************
    AFFICHAGE DES RESULTATS DE REQUETE (cf nsrechcr.cpp)
**************************************************************/
// -----------------------------------------------------------------//
//  Méthodes de NSResultReqWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSResultReqWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSResultReqWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

	TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Function: NSResultReqWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSResultReqWindow::EvLButtonDblClk(uint modKeys, NS_CLASSLIB::TPoint& point)
{
	TListWindow::EvLButtonDown(modKeys, point) ;

	TLwHitTestInfo info(point) ;
	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->AfficheItem() ;
}

//---------------------------------------------------------------------------//  Function: NSResultReqWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSResultReqWindow::IndexItemSelect()
{
	int count = GetItemCount();
	int index = -1;

	for (int i = 0; i < count; i++)
  	if (GetItemState(i, LVIS_SELECTED))
    {
    	index = i ;
      break ;
    }

	return index ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSResultReqDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSResultReqDialog, NSUtilDialog)
	EV_BN_CLICKED(IDC_RESULTREQ_FERMER,   CmFermer),
  EV_LVN_GETDISPINFO(IDC_RESULTREQ_LW,  LvnGetDispInfo),
  EV_BN_CLICKED(IDC_RESULTREQ_MASQUER,  CmMasquer),
  EV_BN_CLICKED(IDC_RESULTREQ_OUVRIR,   CmOuvrir),
  EV_BN_CLICKED(IDC_RESULTREQ_ENREG,    CmEnregistrer),
  EV_BN_CLICKED(IDC_RESULTREQ_EXCEL,    CmExcelExport),
  EV_BN_CLICKED(IDC_RESULTREQ_EXP_TXT,  CmTextExport),
  EV_BN_CLICKED(IDC_RESULTREQ_IMPRIMER, CmImprimer),  EV_TCN_KEYDOWN(IDC_RESULTREQ_TABS,    RequestTabKeyDown),	EV_TCN_SELCHANGE(IDC_RESULTREQ_TABS,  RequestTabSelChange), 
END_RESPONSE_TABLE;

NSResultReqDialog::NSResultReqDialog(TWindow* pere, NSContexte* pCtx, NSRequeteDialog* pRequete)
                  :NSUtilDialog(pere, pCtx, "IDD_RESULTREQ")
{
  pReqDlg      = pRequete ;
  pNomRes      = new NSUtilEdit2(pContexte, this, IDC_RESULTREQ_NOM, 80) ;
  pRequests    = new OWL::TTabControl(this, IDC_RESULTREQ_TABS) ;
  pListeRes    = new NSResultReqWindow(this, IDC_RESULTREQ_LW) ;
  pPatTotal    = new TStatic(this, IDC_RESULTREQ_TOTALPAT1) ;
  pPatCritPat  = new TStatic(this, IDC_RESULTREQ_TOTALPAT2) ;
  pPatCritDoc  = new TStatic(this, IDC_RESULTREQ_TOTALPAT3) ;
  pPatResult   = new TStatic(this, IDC_RESULTREQ_RESULTPAT) ;
  pDocCritPat  = new TStatic(this, IDC_RESULTREQ_TOTALDOC2) ;
  pDocCritDoc  = new TStatic(this, IDC_RESULTREQ_TOTALDOC3) ;
  pDocResult   = new TStatic(this, IDC_RESULTREQ_RESULTDOC) ;

  pRequestPatResult = new TStatic(this, IDC_RESULTREQ_PAT_TABS) ;
	pRequestDocResult = new TStatic(this, IDC_RESULTREQ_DOC_TABS) ;

	_sFileNameRes = string("") ;

  if (pReqDlg)  {    // chargement des compteurs et des données
    _bReqModeDoc  = pReqDlg->useModeDoc() ;
    _nbPatTotal   = pReqDlg->getNbPatTotal() ;
    _nbPatCritPat = pReqDlg->getNbPatCritPat() ;
    _nbPatCritDoc = pReqDlg->getNbPatCritDoc() ;
    _nbPatResult  = pReqDlg->getNbPatResult() ;
    _nbDocCritPat = pReqDlg->getNbDocCritPat() ;
    _nbDocCritDoc = pReqDlg->getNbDocCritDoc() ;
    _nbDocResult  = pReqDlg->getNbDocResult() ;

    NSDocHistoArray* pVectDocResult = pReqDlg->getVectDocResultat() ;
    if (pVectDocResult && (false == pVectDocResult->empty()))
    	for (DocumentIter i = pVectDocResult->begin(); pVectDocResult->end() != i ; i++)
    		_VectDocResultat.push_back(new NSDocumentHisto(*(*i))) ;

    NSPatientArray* pVectPatResult = pReqDlg->getVectPatResultat() ;
    if (pVectPatResult && (false == pVectPatResult->empty()))
    	for (PatientIter i = pVectPatResult->begin() ; pVectPatResult->end() != i ; i++)
    		_VectPatResultat.push_back(new NSPatInfo(*(*i))) ;

    NSRequestResultArray* pRequestResults = pReqDlg->getRequestResults() ;
    if (pRequestResults && (false == pRequestResults->empty()))
    	for (NSRequestResultIter i = pRequestResults->begin(); pRequestResults->end() != i ; i++)
    		_VectRequestResults.push_back(new NSRequestResult(*(*i))) ;
  }
}

NSResultReqDialog::~NSResultReqDialog(){
  delete pListeRes ;
  delete pRequests ;
  delete pPatTotal ;
  delete pPatCritPat ;
  delete pPatCritDoc ;
  delete pPatResult ;
  delete pDocCritPat ;
  delete pDocCritDoc ;
  delete pDocResult ;
  delete pRequestPatResult ;
  delete pRequestDocResult ;
}

voidNSResultReqDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
	InitListeRes() ;
	_itemChoisi = -1 ;

  if (NULL == pReqDlg)
    CmOuvrir() ;

  if (false == _VectRequestResults.empty())
  {
  	if (_VectRequestResults.size() > 1)
    {
    	string sTabText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "allResults") ;
  		TTabItem tab(sTabText.c_str()) ;
			pRequests->Add(tab) ;
    }

		for (NSRequestResultIter it = _VectRequestResults.begin() ; _VectRequestResults.end() != it ; it++)
    {
    	string sReqName = (*it)->getRequestName() ;
    	pRequests->Add(TTabItem(sReqName.c_str())) ;
    }

    pRequests->SetSel(0) ;
  }

  if (string("") != _sFileNameRes)
    pNomRes->SetText(_sFileNameRes.c_str()) ;
  AfficheCompteurs() ;
  AfficheListeRes() ;

/*
	if (pReqDlg)
	{
  	if (sFileNameRes != "")
    	pNomRes->SetText(sFileNameRes.c_str()) ;
    AfficheCompteurs() ;
    AfficheListeRes() ;
	}
	else
	{
		// chargement à partir d'un fichier .nrs
		CmOuvrir() ;
	}
*/
}

NSPatInfo*
NSResultReqDialog::getResultPatient(size_t iIndex)
{
  if (iIndex > _VectPatResultat.size())
    return (NSPatInfo*) 0 ;

  return _VectPatResultat[iIndex] ;
}

NSDocumentHisto*
NSResultReqDialog::getResultDocum(size_t iIndex)
{
  if (iIndex > _VectDocResultat.size())
    return (NSDocumentHisto*) 0 ;

  return _VectDocResultat[iIndex] ;
}

voidNSResultReqDialog::AfficheCompteurs()
{
  char cText[255] = "" ;
  char cLib[255] = "" ;

  pPatTotal->GetText(cLib, 200) ;
  sprintf(cText, "%s : %d", cLib, _nbPatTotal) ;
  pPatTotal->SetText(cText) ;

  pPatCritPat->GetText(cLib, 200) ;
  sprintf(cText, "%s : %d", cLib, _nbPatCritPat) ;
  pPatCritPat->SetText(cText) ;

  pPatCritDoc->GetText(cLib, 200) ;
  sprintf(cText, "%s : %d", cLib, _nbPatCritDoc) ;
  pPatCritDoc->SetText(cText) ;

  pPatResult->GetText(cLib, 200) ;
  sprintf(cText, "%s : %d", cLib, _nbPatResult) ;
  pPatResult->SetText(cText) ;

  if (_bReqModeDoc)
  {
    pDocCritPat->GetText(cLib, 200) ;
    sprintf(cText, "%s : %d", cLib, _nbDocCritPat) ;
    pDocCritPat->SetText(cText) ;

    pDocCritDoc->GetText(cLib, 200) ;
    sprintf(cText, "%s : %d", cLib, _nbDocCritDoc) ;
    pDocCritDoc->SetText(cText) ;

    pDocResult->GetText(cLib, 200) ;
    sprintf(cText, "%s : %d", cLib, _nbDocResult) ;
    pDocResult->SetText(cText) ;
  }
  else
  {
    // compteurs non valides en mode Patient
    pDocCritPat->SetText("") ;
    pDocCritDoc->SetText("") ;
    pDocResult->SetText("") ;
  }
}
void
NSResultReqDialog::InitListeRes()
{
	string sLocalText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "patient") ;
  TListWindColumn colPat((char*) sLocalText.c_str(), 50, TListWindColumn::Left, 0);
  pListeRes->InsertColumn(0, colPat);

	sLocalText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "document") ;
  TListWindColumn colDoc((char*) sLocalText.c_str(), 50, TListWindColumn::Left, 1);
  pListeRes->InsertColumn(1, colDoc);

  sLocalText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "name") ;
  TListWindColumn colNom((char*) sLocalText.c_str(), 200, TListWindColumn::Left, 2);
  pListeRes->InsertColumn(2, colNom);

  sLocalText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "title") ;
  TListWindColumn colTitre((char*) sLocalText.c_str(), 200, TListWindColumn::Left, 3);
  pListeRes->InsertColumn(3, colTitre);
}

voidNSResultReqDialog::AfficheListeRes()
{
	pListeRes->DeleteAllItems() ;
  int index = pRequests->GetSel() ;

  int iNbPatResult = _nbPatResult ;
	int iNbDocResult = _nbDocResult ;

  if (_bReqModeDoc)
  {
  	NSDocHistoArray* pCurrentArray ;

    if ((true == _VectRequestResults.empty()) || (1 == _VectRequestResults.size()))
    	pCurrentArray = &_VectDocResultat ;
    else if (0 == index)
    	pCurrentArray = &_VectDocResultat ;
    else
    {
      if (index < 1)
        index = 1 ;

      NSRequestResult* pReqResult = _VectRequestResults[index-1] ;
      if (pReqResult)
      {
    	  pCurrentArray = &(pReqResult->_aVectDocumentResults) ;
        iNbPatResult = pReqResult->_nbPatResult ;
			  iNbDocResult = pReqResult->_nbDocResult ;
      }
    }

    if (false == pCurrentArray->empty())
  	  for (DocumentIter it = pCurrentArray->end() ; pCurrentArray->begin() != it ; )
      {
    	  it-- ;

        string sPatId = string("") ;
        if (*it)
          sPatId = (*it)->getPatient() ;

    	  TListWindItem Item(sPatId.c_str(), 0, strlen(sPatId.c_str())) ;
        pListeRes->InsertItem(Item) ;
      }
  }
  else
  {
  	NSPatientArray* pCurrentArray ;

    if ((true == _VectRequestResults.empty()) || (1 == _VectRequestResults.size()))
    	pCurrentArray = &_VectPatResultat ;
    else if (0 == index)
    	pCurrentArray = &_VectPatResultat ;
    else
    {
      if (index < 1)
        index = 1 ;

      NSRequestResult* pReqResult = _VectRequestResults[index-1] ;
      if (pReqResult)
      {
    	  pCurrentArray = &(pReqResult->_aVectPatientResults) ;
        iNbPatResult = pReqResult->_nbPatResult ;
			  iNbDocResult = pReqResult->_nbDocResult ;
      }
    }

    if (false == pCurrentArray->empty())
  	  for (PatientIter it = pCurrentArray->end(); pCurrentArray->begin() != it; )
      {
    	  it-- ;

        string sPatId = string("") ;
        if (*it)
          sPatId = (*it)->getNss() ;

    	  // TListWindItem Item((*it)->getszNss(), 0) ;
        TListWindItem Item(sPatId.c_str(), 0, strlen(sPatId.c_str())) ;
        pListeRes->InsertItem(Item) ;
      }
  }

  string sPatText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "patientsFound") ;
  string sDocText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "documentsFound") ;

	char cText[255] = "" ;
  sprintf(cText, "%s : %d", sPatText.c_str(), iNbPatResult) ;
  pRequestPatResult->SetText(cText) ;
  sprintf(cText, "%s : %d", sDocText.c_str(), iNbDocResult) ;
  pRequestDocResult->SetText(cText) ;

/*
	char nss[PAT_NSS_LEN + 1] ;
	int  nbTrouves ;
	if (bReqModeDoc)
		nbTrouves = nbDocResult ;
	else
		nbTrouves = nbPatResult ;

	for (int i = nbTrouves - 1; i >= 0; i--)
	{
		if (bReqModeDoc)
			strcpy(nss, ((VectDocResultat)[i])->pDonnees->codePatient) ;
		else
			strcpy(nss, ((VectPatResultat)[i])->getszNss()) ;
		TListWindItem Item(nss, 0) ;		pListeRes->InsertItem(Item) ;
	}
*/
}

voidNSResultReqDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int BufLen = 255 ;
	static char buffer[BufLen] ;
  strcpy(buffer, "") ;

	string sNom    = string("") ;	string sPrenom = string("") ;  string sDate   = string("") ;

	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;	int index = dispInfoItem.GetIndex() ;

	string sLang = pContexte->getUserLanguage() ;
  NSDocHistoArray* pCurrentDocArray ;
  NSPatientArray*  pCurrentPatArray ;

  int iTabIndex = pRequests->GetSel() ;
  if ((true == _VectRequestResults.empty()) || (1 == _VectRequestResults.size()))
  {
  	pCurrentDocArray = &_VectDocResultat ;
  	pCurrentPatArray = &_VectPatResultat ;
  }
  else if (0 == iTabIndex)
  {
  	pCurrentDocArray = &_VectDocResultat ;
  	pCurrentPatArray = &_VectPatResultat ;
  }
  else
  {
    if (iTabIndex < 1)
      iTabIndex = 1 ;

    NSRequestResult* pReqResult = _VectRequestResults[iTabIndex-1] ;
    if (pReqResult)
    {
  	  pCurrentDocArray = &(pReqResult->_aVectDocumentResults) ;
  	  pCurrentPatArray = &(pReqResult->_aVectPatientResults) ;
    }
  }

  NSDocumentHisto *pDoc = (NSDocumentHisto*) 0 ;
  if (pCurrentDocArray->size() > (size_t) index)
    pDoc = (*pCurrentDocArray)[index] ;

	// Affiche les informations en fonction de la colonne
	switch (dispInfoItem.GetSubItem())	{
		case 1: 	// code document

			if (_bReqModeDoc && pDoc)      	sprintf(buffer, "%s", pDoc->getDocument().c_str()) ;
      break ;

		case 2: 	// nom patient
    	if (pReqDlg->useModeDoc() && pDoc)
      {
      	// Chercher le patient
        string sPatId = pDoc->getPatient() ;

        PatientIter it = pCurrentPatArray->begin() ;
        for ( ; (pCurrentPatArray->end() != it) && ((*it)->getNss() != sPatId) ; it++) ;
        if (pCurrentPatArray->end() != it)
        {
        	sNom    = (*it)->getNom() ;
      		sPrenom = (*it)->getPrenom() ;
        }
      }
      else
      {
        NSPatInfo* pPatRes = _VectPatResultat[index] ;
        if (pPatRes)
        {
				  sNom    = pPatRes->getNom() ;
      	  sPrenom = pPatRes->getPrenom() ;
        }
      }

			sprintf(buffer, "%s %s", sNom.c_str(), sPrenom.c_str()) ;
			break ;

		case 3:     // titre document
			if (pReqDlg->useModeDoc() && pDoc)      {
        sDate = donne_date(pDoc->getDateDoc(), sLang) ;
        sprintf(buffer, "%s du %s", pDoc->getDocName().c_str(), sDate.c_str()) ;
      }
      break ;
  }

  dispInfoItem.SetText(buffer) ;
}

voidNSResultReqDialog::AfficheItem()
{
	_itemChoisi = pListeRes->IndexItemSelect() ;

	if (-1 == _itemChoisi)		return ;

	NSUtilDialog::CmOk() ;}

voidNSResultReqDialog::EcrireResultats(string &sOut)
{
	char cCpt[20] ;

	sOut += string("cpt") + "\"" ;

	if (_bReqModeDoc)
		sOut += string("1") + "\"" ;
	else
		sOut += string("0") + "\"" ;

  // on écrit tous les compteurs
  itoa(_nbPatTotal, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbPatCritPat, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbPatCritDoc, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbPatResult, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbDocCritPat, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbDocCritDoc, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  itoa(_nbDocResult, cCpt, 10) ;
  sOut += string(cCpt) + "\"" ;
  sOut += "$\n" ;       // le '$' en fin de chaine sert à simplifier la lecture

	// Vecteur des patients
	sOut += string("vpa") + "\"" ;
	for (PatientIter i = _VectPatResultat.begin() ; _VectPatResultat.end() != i ; i++)
		sOut += (*i)->getNss() + "\"" ;

	sOut += "$\n" ;
	// Vecteur des documents	if (_bReqModeDoc && (false == _VectDocResultat.empty()))
	{
		sOut += string("vdo") + "\"" ;
    for (DocumentIter i = _VectDocResultat.begin() ; _VectDocResultat.end() != i ; i++)
    {
    	sOut += (*i)->getPatient() + "|" ;
      sOut += (*i)->getDocument() + "\"" ;
    }
    sOut += "$\n" ;
	}

  if (_VectRequestResults.empty())
		return ;

	for (NSRequestResultIter it = _VectRequestResults.begin() ; _VectRequestResults.end() != it ; it++)
	{
  	// Vecteur des patients
		sOut += string("vp2") + "\"" ;
    sOut += (*it)->getRequestName() + "\"" ;
    sOut += (*it)->getRequestLabel() + "\"" ;
    itoa((*it)->_nbPatResult, cCpt, 10) ;
  	sOut += string(cCpt) + "\"" ;
  	itoa((*it)->_nbDocResult, cCpt, 10) ;
  	sOut += string(cCpt) + "\"" ;
    if (false == (*it)->_aVectPatientResults.empty())
			for (PatientIter i = (*it)->_aVectPatientResults.begin() ; (*it)->_aVectPatientResults.end() != i ; i++)
				sOut += (*i)->getNss() + "\"" ;

		sOut += "$\n" ;
		// Vecteur des documents		if (_bReqModeDoc && (false == (*it)->_aVectDocumentResults.empty()))
		{
			sOut += string("vd2") + "\"" ;
    	for (DocumentIter i = (*it)->_aVectDocumentResults.begin() ; (*it)->_aVectDocumentResults.end() != i ; i++)
    	{
    		sOut += (*i)->getPatient()  + "|" ;
      	sOut += (*i)->getDocument() + "\"" ;
    	}
    	sOut += "$\n" ;
		}
  }
}

boolNSResultReqDialog::ChargerResultats(string sFichier)
{
	ifstream inFile ;
	inFile.open(sFichier.c_str()) ;
	if (!inFile)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" (") + sFichier + string(")") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return false ;
	}

	while (!inFile.eof())
	{
    string line ;
		getline(inFile, line) ;

		if (strlen(line.c_str()) >= 3)
		{
    	string sType = string(line, 0, 3) ;

      size_t pos1 ;
    	size_t pos2 = line.find('\"');
      if (NPOS == pos2)
      	continue ;

      if (string("cpt") == sType)
      {
        // compteurs
        _nbPatTotal   = 0 ;
        _nbPatCritPat = 0 ;
        _nbPatCritDoc = 0 ;
        _nbPatResult  = 0 ;
        _nbDocCritPat = 0 ;
        _nbDocCritDoc = 0 ;
        _nbDocResult  = 0 ;

        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        if (NPOS == pos2)
          continue ;
        string sData = string(line, pos1, pos2 - pos1) ;
        if (string("1") == sData)
          _bReqModeDoc = true ;
        else
          _bReqModeDoc = false ;

        if (false == GetResultCounter(line, pos1, pos2, _nbPatTotal))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbPatCritPat))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbPatCritDoc))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbPatResult))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbDocCritPat))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbDocCritDoc))
          continue ;
        if (false == GetResultCounter(line, pos1, pos2, _nbDocResult))
          continue ;
      }
      else if (string("vpa") == sType)
      {
        // vecteur des patients résultat
        //
        _VectPatResultat.vider() ;

        // on charge juste les nss pour l'instant
        // on a forcément un '$' en fin de chaine,
        // cela évite un plantage éventuel sur find

        NSPatInfo patinfo(pContexte) ;

        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        while (NPOS != pos2)
        {
          string sData = string(line, pos1, pos2-pos1) ;
          patinfo.setNss(sData) ;
          _VectPatResultat.push_back(new NSPatInfo(patinfo)) ;          pos1 = pos2 + 1 ;
          pos2 = line.find('\"', pos1) ;
      	}
      }
      else if (string("vdo") == sType)
      {
        // vecteur des documents résultat
        //
        _VectDocResultat.vider() ;

        // on charge juste les codes pour l'instant
        // on a forcément un '$' en fin de chaine,
        // cela évite un plantage éventuel sur find

        NSDocumentHisto dochisto(pContexte) ;

        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        while (NPOS != pos2)
        {
          string sData = string(line, pos1, pos2 - pos1) ;
          size_t pos = sData.find('|') ;
          if (NPOS != pos)
          {
            string sCodePat = string(sData, 0, pos) ;
            string sCodeDoc = string(sData, pos + 1, strlen(sData.c_str()) - pos - 1) ;
            dochisto.setPatient(sCodePat) ;
            dochisto.setDocument(sCodeDoc) ;
            _VectDocResultat.push_back(new NSDocumentHisto(dochisto)) ;
          }
          pos1 = pos2 + 1 ;
          pos2 = line.find('\"', pos1) ;        }
      }
    	else if (string("vp2") == sType)
      {
        // vecteur des patients résultat
        NSRequestResult reqResult ;

        // on charge juste les nss pour l'instant
        // on a forcément un '$' en fin de chaine,
        // cela évite un plantage éventuel sur find
        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        if (NPOS == pos2)
        	continue ;

        reqResult.setRequestName(string(line, pos1, pos2 - pos1)) ;

        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        if (NPOS == pos2)
        	continue ;

        reqResult.setRequestLabel(string(line, pos1, pos2 - pos1)) ;

        pos1 = pos2+1 ;
        pos2 = line.find('\"', pos1) ;
        if (NPOS == pos2)
        	continue ;

        string sData = string(line, pos1, pos2-pos1) ;
        reqResult._nbPatResult = atoi(sData.c_str()) ;

        pos1 = pos2+1 ;
        pos2 = line.find('\"', pos1) ;
        if (NPOS == pos2)        	continue ;

        sData = string(line, pos1, pos2-pos1) ;        reqResult._nbDocResult = atoi(sData.c_str()) ;

        while (NPOS != pos2)
        {
          sData = string(line, pos1, pos2-pos1) ;
          NSPatInfo patinfo(pContexte) ;
          patinfo.setNss(sData) ;
          reqResult._aVectPatientResults.push_back(new NSPatInfo(patinfo)) ;          pos1 = pos2 + 1 ;
          pos2 = line.find('\"', pos1) ;
      	}

				_VectRequestResults.push_back(new NSRequestResult(reqResult)) ;
      }
      else if (string("vd2") == sType)
      {
        // vecteur des documents résultat
        //
        NSRequestResult* pReqResult = _VectRequestResults.back() ;

        // on charge juste les codes pour l'instant
        // on a forcément un '$' en fin de chaine,
        // cela évite un plantage éventuel sur find
        pos1 = pos2 + 1 ;
        pos2 = line.find('\"', pos1) ;
        while (NPOS != pos2)
        {
          string sData = string(line, pos1, pos2 - pos1) ;
          size_t pos = sData.find('|') ;
          if (NPOS != pos)
          {
            string sCodePat = string(sData, 0, pos) ;
            string sCodeDoc = string(sData, pos + 1, strlen(sData.c_str()) - pos - 1) ;
            NSDocumentHisto dochisto(pContexte) ;
            dochisto.setPatient(sCodePat) ;
            dochisto.setDocument(sCodeDoc) ;
            pReqResult->_aVectDocumentResults.push_back(new NSDocumentHisto(dochisto)) ;
          }
          pos1 = pos2 + 1 ;
          pos2 = line.find('\"', pos1) ;        }
      }
    }
  } // fin du while

	inFile.close() ;
	return true ;
}

bool
NSResultReqDialog::GetResultCounter(string sLine, size_t &pos1, size_t &pos2, int &iNb)
{
  pos1 = pos2 + 1 ;
  pos2 = sLine.find('\"', pos1) ;
  if (NPOS == pos2)    return false ;
  string sData = string(sLine, pos1, pos2 - pos1) ;  iNb = atoi(sData.c_str()) ;

  return true ;
}

//// On charge les données des patients dont les codes (nss)
// sont contenus dans VectPatResultat
//
bool
NSResultReqDialog::ChargerPatResultat()
{
	if (_VectPatResultat.empty())
		return true ;

  NSProgressDlg progressDlg(this, pContexte) ;
  progressDlg.setCaptionText(pContexte->getSuperviseur()->getText("searchingResultDisplay", "loadingData")) ;
  progressDlg.setTaskText(string("")) ;
  progressDlg.Create() ;
  progressDlg.Show(SW_SHOW) ;

  string sPatText = pContexte->getSuperviseur()->getText("searchingResultDisplay", "patientId") + string(" ") ;

	SetCursor(0, IDC_WAIT) ;
	string user = pContexte->getUtilisateurID() ;

  size_t iResSize = _VectPatResultat.size() ;
  size_t iCurRes  = 0 ;

	for (PatientIter it = _VectPatResultat.begin() ; _VectPatResultat.end() != it ; it++, iCurRes++)
	{
  	string sNss = (*it)->getNss() ;

    progressDlg.updateTaskText(sPatText + sNss) ;
    progressDlg.setPosition(100 * int(iCurRes) / iResSize) ;
    pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;

		NSBasicAttributeArray    AttrArray ;
		NSPersonsAttributesArray List ;

		// ouverture standard
		AttrArray.push_back(new NSBasicAttribute(PERSON,   sNss)) ;
		AttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
		AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
		AttrArray.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;

		bool res = pContexte->getSuperviseur()->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(),
                                    (*it)->getGraphPerson()->getDataGraph(), &List, &AttrArray) ;

		if ((true == res) && (NULL != (*it)->getGraphPerson()->getDataGraph()) &&
    	  (NULL != (*it)->getGraphPerson()->graphPrepare()))
		{
			(*it)->getGraphPerson()->setNeedUnlock(false) ;
			(*it)->getGraphPerson()->setReadOnly(true) ;

			if (false == List.empty())
			{
				string sIsLocked = List.getAttributeValue("locked") ;
				if (sIsLocked == "ok")
					(*it)->getGraphPerson()->setNeedUnlock(true) ;
				string sOperationType	= List.getAttributeValue("operationType") ;
				if (sOperationType == "readWrite")
					(*it)->getGraphPerson()->setReadOnly(false) ;
			}

      // Make certain that the patient has not been locked
      //
      (*it)->debloquer() ;

			(*it)->getGraphPerson()->setInfoPids(&AttrArray) ;
			(*it)->getGraphPerson()->getDataGraph()->setLastTree() ;

			string sRootTree = (*it)->getGraphPerson()->getRootTree() ;
			// NSDataGraph* pGraph = Patient.pGraphPerson->pDataGraph;

      NSPatPathoArray PPTAdmin(pContexte->getSuperviseur(), graphPerson) ;
	    NSDataTreeIter iterTree ;

	    if ((*it)->getGraphPerson()->getDataGraph()->getTrees()->ExisteTree("ZADMI1", pContexte->getSuperviseur(), &iterTree))
	    {
  	    (*iterTree)->getRawPatPatho(&PPTAdmin) ;
        (*it)->ChargeDonneesPatient(&PPTAdmin) ;
      }

      if (false == _VectDocResultat.empty())
      {
      	DocumentIter itDoc = _VectDocResultat.begin() ;
				for ( ; _VectDocResultat.end() != itDoc ; itDoc++)
        {
        	if ((*itDoc)->getPatient() == sNss)
          {
            string sDocRosace ;

            NSPatPathoArray DocPatho(pContexte->getSuperviseur()) ;
      			if ((*it)->getGraphPerson()->getTree((*itDoc)->getID(), &DocPatho, &sDocRosace))
            {
              (*itDoc)->setPatPatho(&DocPatho) ;
            	(*itDoc)->SetDateDoc() ;

              // Get Meta ID
              //
              VecteurString aVecteurString ;
              (*it)->getGraphPerson()->getLinkManager()->TousLesVrais((*itDoc)->getID(), NSRootLink::docData, &aVecteurString, "ENVERS") ;
              if (false == aVecteurString.empty())
              {
                (*itDoc)->setCodeDocMeta(*(*(aVecteurString.begin()))) ;
                (*itDoc)->LoadMetaAndData((*it)->getGraphPerson()) ;
              }
            }
          }
        }
      }
    }

    if (false == progressDlg.keepLoading())
      break ;
  }
  SetCursor(0, IDC_ARROW) ;

  return true ;
}
boolNSResultReqDialog::ChargerDocResultat()
{
	return true ;
}

voidNSResultReqDialog::CmFermer()
{
	NSUtilDialog::CmOk() ;
}

voidNSResultReqDialog::CmMasquer()
{
	NSUtilDialog::CmCancel() ;
}

voidNSResultReqDialog::CmOuvrir()
{
	// on regarde d'abord si une requete est déjà ouverte
	if (pReqDlg)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
		int idRet = MessageBox("Attention une requête est déjà chargée. Etes-vous certain de vouloir en ouvrir une autre ?", sCaption.c_str(), MB_YESNO);
		if (IDNO == idRet)
			return ;
	}

  NSUtilisateurChoisi* pUser = pContexte->getUtilisateur() ;
  if ((NSUtilisateurChoisi*) NULL == pUser)
    return ;

	// on delete éventuellement l'ancienne requete en passant par l'utilisateur
  NSRequeteDialog* pRequete = pUser->getRequeteDialog() ;
	if (pRequete)
	{
		delete pRequete ;
    pUser->setRequeteDialog((NSRequeteDialog*) 0) ;
	}

	pUser->setRequeteDialog(new NSRequeteDialog(pContexte->GetMainWindow(), pContexte)) ;
	pReqDlg = pUser->getRequeteDialog() ;

	// on choisi d'abord le répertoire par défaut des requetes (NREQ)
	string sPath = pContexte->PathName("NREQ") ;

	TFileOpenDialog::TData initData(OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST,
   											"Tous fichiers (*.NRS)|*.nrs|",
                                            0, (char*) sPath.c_str(), "NRS") ;

	int iUserChoice = TFileOpenDialog(pContexte->GetMainWindow(), initData).Execute() ;

	pContexte->reallocateBaseDirectory() ;

	if (IDOK != iUserChoice)
	{
		if (pReqDlg)
		{
			pRequete = pUser->getRequeteDialog() ;
	    if (pRequete)
	    {
		    delete pRequete ;
        pUser->setRequeteDialog((NSRequeteDialog*) 0) ;
	    }
      pReqDlg = (NSRequeteDialog*) 0 ;
    }
    return ;
	}

	string sFileName = string(initData.FileName) ;
	SetCursor(0, IDC_WAIT) ;

	if (false == pReqDlg->ChargerRequete(sFileName))
	{
		SetCursor(0, IDC_ARROW) ;
		erreur("Impossible de charger le fichier requete associé.", standardError, 0, GetHandle()) ;
		return ;
	}

	if (false == ChargerResultats(sFileName))
	{
		SetCursor(0, IDC_ARROW) ;
		erreur("Impossible de charger le fichier résultat spécifié.", standardError, 0, GetHandle()) ;
    return ;
	}

	size_t pos = sFileName.find_last_of('\\') ;
	if (NPOS != pos)
		sFileName = string(sFileName, pos+1, strlen(sFileName.c_str())-pos-1) ;

	pReqDlg->setFileName(sFileName) ;
	// pReqDlg->pNomReq->SetText(sFileName.c_str());

	if (false == ChargerPatResultat())
	{
		erreur("Impossible de charger les patients résultat.", standardError, 0, GetHandle()) ;
	}

	if (false == ChargerDocResultat())
	{
		erreur("Impossible de charger les documents résultat.", standardError, 0, GetHandle()) ;
	}

	_sFileNameRes = sFileName ;

	// pNomRes->SetText(sFileNameRes.c_str()) ;

	// on affiche les résultats
	// AfficheCompteurs() ;
	// AfficheListeRes() ;

	SetCursor(0, IDC_ARROW) ;
}

voidNSResultReqDialog::CmEnregistrer()
{
	if (NULL == pReqDlg)
	{
		erreur("Le fichier résultat est vide, ou n'est associé à aucune requete.", standardError, 0, GetHandle()) ;
    return ;
	}

	string sFichierResult = "" ;
	ofstream outFile ;

	pReqDlg->EcrireRequete(sFichierResult) ;
	EcrireResultats(sFichierResult) ;

	NSNomGdDialog* pNomGdDlg =
        new NSNomGdDialog(this, pContexte->PathName("NREQ"), _sFileNameRes, "nrs", pContexte) ;

	if (pNomGdDlg->Execute() != IDOK)
  {
  	delete pNomGdDlg ;
    return ;
  }

	// on peut enregistrer sous le nom de fichier proposé
  string sNomFichier = pNomGdDlg->sFichier ;

  outFile.open(sNomFichier.c_str());
  if (outFile)
  {
  	for (size_t i = 0; i < strlen(sFichierResult.c_str()); i++)
    	outFile.put(sFichierResult[i]) ;

    outFile.close() ;
  }
  else
  {
  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string("") + sNomFichier ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
  	// erreur("Impossible d'enregistrer le fichier de resultats.", standardError, 0, GetHandle()) ;
    return ;
	}

	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
	MessageBox("Fichier de resultats enregistré.", sCaption.c_str(), MB_OK) ;
	size_t pos = sNomFichier.find_last_of('\\');	if (NPOS != pos)
		_sFileNameRes = string(sNomFichier, pos+1, strlen(sNomFichier.c_str())-pos-1) ;
	else
		_sFileNameRes = sNomFichier ;

	pNomRes->SetText(_sFileNameRes.c_str()) ;

	delete pNomGdDlg ;
}

void
NSResultReqDialog::CmExcelExport()
{
	NSNomGdDialog* pNomGdDlg =
        new NSNomGdDialog(this, pContexte->PathName("NREQ"), _sFileNameRes, "xls", pContexte) ;

	if (pNomGdDlg->Execute() != IDOK)
  {
  	delete pNomGdDlg ;
    return ;
  }

	// on peut enregistrer sous le nom de fichier proposé
  string sFileName = pNomGdDlg->sFichier ;
  delete pNomGdDlg ;

  CMiniExcel miniExcel ;

  unsigned iCriteres = 0 ;

  // Header
	//
  miniExcel(iCriteres, 0) = "Utilisateur" ;
  if (pContexte->getUtilisateur())
  	miniExcel(iCriteres, 1) = pContexte->getUtilisateur()->donneSignature(pContexte).c_str() ;

  miniExcel(iCriteres, 2) = "Date" ;
  NVLdVTemps tNow ;
  tNow.takeTime() ;
  miniExcel(iCriteres, 2) = tNow.donneDateHeure().c_str() ;

  iCriteres++ ;

	// Research criteria
  //
	if (pReqDlg)
  {
  	if (pReqDlg->useCritPat())
		{
    	miniExcel(++iCriteres, 0) = "Critères patients" ;

      NSCritReqPatient* pCritPat = pReqDlg->getCritPat() ;

      if (string("") != pCritPat->getNom())
      {
      	miniExcel(++iCriteres, 1) = "Nom" ;
        miniExcel(iCriteres, 2)   = pCritPat->getNom().c_str() ;
      }
      if (string("") != pCritPat->getPrenom())
      {
      	miniExcel(++iCriteres, 1) = "Prénom" ;
        miniExcel(iCriteres, 2)   = pCritPat->getPrenom().c_str() ;
      }
      if (string("") != pCritPat->getSexe())
      {
      	miniExcel(++iCriteres, 1) = "Sexe" ;
        miniExcel(iCriteres, 2)   = pCritPat->getSexe().c_str() ;
      }
      if (string("") != pCritPat->getDateN1())
      {
      	miniExcel(++iCriteres, 1) = "Date de naissance mini" ;
        miniExcel(iCriteres, 2)   = pCritPat->getDateN1().c_str() ;
      }
      if (string("") != pCritPat->getDateN2())
      {
      	miniExcel(++iCriteres, 1) = "Date de naissance maxi" ;
        miniExcel(iCriteres, 2)   = pCritPat->getDateN2().c_str() ;
      }
      if (string("") != pCritPat->getSitfam())
      {
      	miniExcel(++iCriteres, 1) = "Situation familiale" ;
        miniExcel(iCriteres, 2)   = pCritPat->getSitfam().c_str() ;
      }
      if (string("") != pCritPat->getCodePost())
      {
      	miniExcel(++iCriteres, 1) = "Code postal" ;
        miniExcel(iCriteres, 2)   = pCritPat->getCodePost().c_str() ;
      }
      if (string("") != pCritPat->getVille())
      {
      	miniExcel(++iCriteres, 1) = "Ville" ;
        miniExcel(iCriteres, 2)   = pCritPat->getVille().c_str() ;
      }
		}

		// on écrit les critères document
		if (pReqDlg->useCritDoc())
		{
    	miniExcel(++iCriteres, 0) = "Critères documents" ;

      NSCritReqDocum* pCritDoc = pReqDlg->getCritDoc() ;

      if (string("") != pCritDoc->sTitreAuteur)
      {
      	miniExcel(++iCriteres, 1) = "Auteur" ;
        miniExcel(iCriteres, 2)   = pCritDoc->sTitreAuteur.c_str() ;
      }
      if (string("") != pCritDoc->sCodeRoot)
      {
      	miniExcel(++iCriteres, 1) = "Type" ;
        miniExcel(iCriteres, 2)   = pCritDoc->sCodeRoot.c_str() ;
      }
      if (string("") != pCritDoc->sDate1)
      {
      	miniExcel(++iCriteres, 1) = "Date mini" ;
        miniExcel(iCriteres, 2)   = pCritDoc->sDate1.c_str() ;
      }
      if (string("") != pCritDoc->sDate2)
      {
      	miniExcel(++iCriteres, 1) = "Date maxi" ;
        miniExcel(iCriteres, 2)   = pCritDoc->sDate2.c_str() ;
      }
		}
  }

  iCriteres++ ;

  unsigned iCounters = ++iCriteres ;

  miniExcel(iCounters, 1) = "Patients" ;

  miniExcel(iCounters+1, 0) = "Total" ;
	miniExcel(iCounters+1, 1) = _nbPatTotal ;

	miniExcel(iCounters+2, 0) = "+ critères patient" ;
	miniExcel(iCounters+2, 1) = _nbPatCritPat ;

  miniExcel(iCounters+3, 0) = "+ critères document" ;
	miniExcel(iCounters+3, 1) = _nbPatCritDoc ;

  miniExcel(iCounters+4, 0) = "Résultat" ;
	miniExcel(iCounters+4, 1) = _nbPatResult ;

	if (_bReqModeDoc)
  {
		miniExcel(iCounters,   2) = "Documents" ;
    miniExcel(iCounters+2, 2) = _nbDocCritPat ;
    miniExcel(iCounters+3, 2) = _nbDocCritDoc ;
    miniExcel(iCounters+4, 2) = _nbDocResult ;
	}

	unsigned iCurrentRow = iCounters + 6 ;

  if (false == _VectRequestResults.empty())
  {
  	NSRequestResultIter resIter = _VectRequestResults.begin() ;
    for ( ; _VectRequestResults.end() != resIter ; resIter++)
    {
    	string sReqName = (*resIter)->getRequestName() ;
      miniExcel(iCurrentRow, 0) = sReqName.c_str() ;
      miniExcel(iCurrentRow, 1) = (*resIter)->_nbPatResult ;
      if (_bReqModeDoc)
      	miniExcel(iCurrentRow, 2) = (*resIter)->_nbDocResult ;

      iCurrentRow++ ;
    }
  }

	FILE *f = fopen(sFileName.c_str(), "wb") ;
  if (!f)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFileName ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return ;
  }

  miniExcel.Write(f) ;
}

void
NSResultReqDialog::CmTextExport()
{
	NSNomGdDialog* pNomGdDlg =
        new NSNomGdDialog(this, pContexte->PathName("NREQ"), _sFileNameRes, "txt", pContexte) ;

	if (pNomGdDlg->Execute() != IDOK)
  {
  	delete pNomGdDlg ;
    return ;
  }

  string sLang = pContexte->getUserLanguage() ;

	// on peut enregistrer sous le nom de fichier proposé
  string sFileName = pNomGdDlg->sFichier ;
  delete pNomGdDlg ;

  // Opening file
  //
  ofstream outFile ;
  outFile.open(sFileName.c_str()) ;
  if (!outFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string("") + sFileName ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return ;
  }

  CMiniExcel miniExcel ;

  // Header
	//
  if (pContexte->getUtilisateur())
  	outFile << string("Utilisateur : ") + pContexte->getUtilisateur()->donneSignature(pContexte) + NEWLINE ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  string sReqDate = donne_date(tNow.donneDateHeure(), sLang) ;
  outFile << string("Date : ") + sReqDate + NEWLINE ;
  outFile << NEWLINE ;

	// Research criteria
  //
	if (pReqDlg)
  {
  	if (pReqDlg->useCritPat())
		{
    	outFile << string("Critères patients :") + NEWLINE ;

      NSCritReqPatient* pCritPat = pReqDlg->getCritPat() ;

      if (string("") != pCritPat->getNom())
      	outFile << string("- Nom                    : ") + pCritPat->getNom() + NEWLINE ;
      if (string("") != pCritPat->getPrenom())
        outFile << string("- Prénom                 : ") + pCritPat->getPrenom() + NEWLINE ;
      if (string("") != pCritPat->getSexe())
        outFile << string("- Sexe                   : ") + pCritPat->getSexe() + NEWLINE ;
      if (string("") != pCritPat->getDateN1())
        outFile << string("- Date de naissance mini : ") + pCritPat->getDateN1() + NEWLINE ;
      if (string("") != pCritPat->getDateN2())
        outFile << string("- Date de naissance maxi : ") + pCritPat->getDateN2() + NEWLINE ;
      if (string("") != pCritPat->getSitfam())
        outFile << string("- Situation familiale    : ") + pCritPat->getSitfam() + NEWLINE ;
      if (string("") != pCritPat->getCodePost())
        outFile << string("- Code postal            : ") + pCritPat->getCodePost() + NEWLINE ;
      if (string("") != pCritPat->getVille())
        outFile << string("- Ville                  : ") + pCritPat->getVille() + NEWLINE ;

      outFile << NEWLINE ;
		}

		// on écrit les critères document
		if (pReqDlg->useCritDoc())
		{
      outFile << string("Critères documents :") + NEWLINE ;

      NSCritReqDocum* pCritDoc = pReqDlg->getCritDoc() ;

      if (string("") != pCritDoc->sTitreAuteur)
        outFile << string("- Auteur    : ") + pCritDoc->sTitreAuteur + NEWLINE ;
      if (string("") != pCritDoc->sCodeRoot)
        outFile << string("- Type      : ") + pCritDoc->sCodeRoot + NEWLINE ;
      if (string("") != pCritDoc->sDate1)
        outFile << string("- Date mini : ") + donne_date(pCritDoc->sDate1, sLang) + NEWLINE ;
      if (string("") != pCritDoc->sDate2)
        outFile << string("- Date maxi : ") + donne_date(pCritDoc->sDate2, sLang) + NEWLINE ;

      outFile << NEWLINE ;
		}
  }

  outFile << string("Patients explorés :") + NEWLINE ;

  outFile << string("- Total                  : ") + IntToString(_nbPatTotal) + NEWLINE ;
  outFile << string("- Avec critères patient  : ") + IntToString(_nbPatCritPat) + NEWLINE ;
  outFile << string("- Avec critères document : ") + IntToString(_nbPatCritDoc) + NEWLINE ;
  outFile << string("- Retenus                : ") + IntToString(_nbPatResult) + NEWLINE ;

	if (_bReqModeDoc)
  {
    outFile << string("Documents :") + NEWLINE ;
    outFile << string("- Avec critères patient  : ") + IntToString(_nbDocCritPat) + NEWLINE ;
    outFile << string("- Avec critères document : ") + IntToString(_nbDocCritDoc) + NEWLINE ;
    outFile << string("- Retenus                : ") + IntToString(_nbDocResult) + NEWLINE ;
	}

  outFile << NEWLINE ;

  if (false == _VectRequestResults.empty())
  {
  	NSRequestResultIter resIter = _VectRequestResults.begin() ;
    for ( ; _VectRequestResults.end() != resIter ; resIter++)
    {
      outFile << string("Requête ") + (*resIter)->getRequestName() + NEWLINE ;

      outFile << string("- Nombre de patients retenus : ") + IntToString((*resIter)->_nbPatResult) + NEWLINE ;
      if (_bReqModeDoc)
        outFile << string("- Nombre de documents retenus : ") + IntToString((*resIter)->_nbDocResult) + NEWLINE ;

      if (_bReqModeDoc)
      {
        if (false == (*resIter)->_aVectDocumentResults.empty())
        {
          outFile << string("Résultats :") + NEWLINE ;

          string sCurrentPatientId = string("") ;

          for (DocumentConstIter it = (*resIter)->_aVectDocumentResults.begin() ; (*resIter)->_aVectDocumentResults.end() != it ; it++)
          {
            string sPatId = (*it)->getPatient() ;
            if (sPatId != sCurrentPatientId)
            {
              sCurrentPatientId = sPatId ;
              NSPatInfo* patInfo = (*resIter)->getPatientFromId(sCurrentPatientId) ;
              if (patInfo)
                outFile << patInfo->getNom() + string(" ") + patInfo->getPrenom() + NEWLINE ;
              else
                outFile << string("???? patient inconnu") + NEWLINE ;
            }

            string sDate = donne_date((*it)->getDateDoc(), sLang) ;
            outFile << string("- ") + (*it)->getDocName() + string(" du ") + sDate + NEWLINE ;
          }
        }
      }
      else if (false == (*resIter)->_aVectPatientResults.empty())
      {
        for (PatientConstIter it = (*resIter)->_aVectPatientResults.begin() ; (*resIter)->_aVectPatientResults.end() != it ; it++)
          outFile << (*it)->getNom() + string(" ") + (*it)->getPrenom() + NEWLINE ;
      }

      outFile << NEWLINE ;
    }
  }

  outFile.close() ;
}

void
NSResultReqDialog::CmImprimer()
{
	// On ne fait pas de delete du document
	// car NSVisualView le fait en fin d'impression
	pContexte->getUtilisateur()->setReqRefDoc(new NSReqRefDocument(this)) ;
	pContexte->getUtilisateur()->getReqRefDoc()->Publier(false) ;
}

void
NSResultReqDialog::RequestTabKeyDown(TTabKeyDown& nmHdr)
{
	// int index = pRequests->GetSel() ;
  AfficheListeRes() ;
}

void
NSResultReqDialog::RequestTabSelChange(TNotify far& nm)
{
	// int index = pRequests->GetSel() ;
  AfficheListeRes() ;
}

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(IdentifierDialog, TDialog)
	EV_COMMAND(IDOK,	CmOk),
END_RESPONSE_TABLE;

// -----------------------------------------------------------------
IdentifierDialog::IdentifierDialog(TWindow* pere, NSContexte* pCtx, string sPatId)
                 :NSUtilDialog(pere, pCtx, "IDD_IDENTIFIER")
{
	_pIdText = new TStatic(this, IDC_IDENTIFIER_TXT) ;
  _pIdEdit = new OWL::TEdit(this, IDC_IDENTIFIER) ;

  _sPatientId = sPatId ;
}

IdentifierDialog::~IdentifierDialog()
{
  delete _pIdText ;
  delete _pIdEdit ;
}

void IdentifierDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	_pIdEdit->SetText(_sPatientId.c_str()) ;
}

void IdentifierDialog::CmOk()
{
	TDialog::CmOk() ;
}

// ---------------------------------------------------------------------------DEFINE_RESPONSE_TABLE1(AboutDialog, TDialog)
	EV_COMMAND(IDOK,	CmOk),
END_RESPONSE_TABLE;

// -----------------------------------------------------------------
AboutDialog::AboutDialog(TWindow* pere, NSContexte* pCtx)
            :NSUtilDialog(pere, pCtx, "IDD_NS_ABOUT_MUE")
{
	_pAppName        = new OWL::TStatic(this, IDC_APPNAME) ;
  _pBamName        = new OWL::TStatic(this, IDC_BAM_NAME) ;
  _pHistory        = new OWL::TStatic(this, IDC_HISTORY) ;
  _pReleasesViewer = new OWL::TEdit(this, IDC_RELEASES) ;
}

AboutDialog::~AboutDialog()
{
  delete _pAppName ;
  delete _pBamName ;
  delete _pReleasesViewer ;
  delete _pHistory ;
}

void AboutDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  string sHistory = pContexte->getSuperviseur()->getText("updateManagement", "versionsHistory") ;
  _pHistory->SetText(sHistory.c_str()) ;

  string sAbout = pContexte->getSuperviseur()->getText("updateManagement", "about") ;
	string sCaption = sAbout + string(" ") + pContexte->getSuperviseur()->getAppName() ;
  SetCaption(sCaption.c_str());

  string sText = pContexte->getSuperviseur()->getAppName() + string(" ") + pContexte->getSuperviseur()->sNumVersion ;
	_pAppName->SetText(sText.c_str()) ;

  // Release information
  //
  NSUpdateParameters updateParams(pContexte) ;
  if (updateParams.init())
  {
    string sReleaseText = updateParams.getReleaseTxt() ;
    _pReleasesViewer->SetText(sReleaseText.c_str()) ;
  }

  // Bam version
  //
  NSBdmDriver* pDriver = pContexte->getBdmDriver() ;
  if (pDriver)
  {
    string sDriverVersion = pDriver->getBamVersionId() ;
    _pBamName->SetText(sDriverVersion.c_str()) ;
  }
  else
    _pBamName->SetText("Sana base de données médicamenteuse") ;
}

void AboutDialog::CmOk()
{
	TDialog::CmOk();
}

/****************************************************************************

	ancienne version// -----------------------------------------------------------------
//
//  Méthodes de NSTableauNoir
//

// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSTableauNoir, TControl)
	EV_WM_PAINT,
	EV_WM_ERASEBKGND,
	EV_WM_LBUTTONDOWN,
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSTableauNoir::NSTableauNoir(ChercheListePatDialog* parent,
									  const char far* title,
									  int             id,
									  int x, int y, int w, int h,
									  TModule*        module)
					:TControl(parent, id, title, x, y, w, h, module)
{
	pListeDlg = parent;
	Attr.Style = WS_CHILD | WS_VISIBLE;
	Attr.Style &= ~WS_TABSTOP;     // no input for us
}

// NSTableauNoir::NSTableauNoir(ChercheListePatDialog* parent, int resourceId, TModule* module)
//				  :TGroupBox(parent, resourceId, module)
// {
// 	pListeDlg = parent;
// }

//---------------------------------------------------------------------------
//  Function: ChercheListePatDialog::SetupWindow()
//
//  Arguments:	  Aucun
//
//  Description: Initialise la boite de dialogue
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSTableauNoir::SetupWindow()
{
	HWND         hCtrl;
	HDC          hDC;
	HFONT        hFont;
	LOGFONT 		 logfont;
	TEXTMETRIC	 textMetric;
	long			 iHauteur;

	TWindowAttr WAttr = pListeDlg->pGrpFond->Attr;
	Attr.X = WAttr.X+3;
	Attr.Y = WAttr.Y+3;
	Attr.W = WAttr.W-6;
	Attr.H = WAttr.H-6;
	//
	//
	// Récupération des coordonnées de l'Edit
	//
	GetClientRect(rectFond);
	iHauteur = rectFond.Height();
	//
	// Prise d'un DC
	//
	TDC* pDC;
	// hCtrl = pListeDlg->GetDlgItem(ID_LISTPA_LISTE);
	pDC = new TDC(GetDC(hCtrl));
	pDC->SetMapMode(MM_TEXT);
	//
	// Etablissement des caractéristiques de la police
	//
	logfont.lfHeight 	    	  = -10;
	logfont.lfWidth  	    	  = 0;
	logfont.lfEscapement	     = 0;
	logfont.lfOrientation     = 0;
	logfont.lfWeight          = FW_NORMAL;
	logfont.lfItalic          = 0;
	logfont.lfUnderline       = 0;
	logfont.lfStrikeOut       = 0;
	logfont.lfCharSet         = ANSI_CHARSET;
	logfont.lfOutPrecision    = OUT_TT_PRECIS;
	logfont.lfClipPrecision   = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality         = PROOF_QUALITY;
	logfont.lfPitchAndFamily  = VARIABLE_PITCH | FF_ROMAN;
	strcpy(logfont.lfFaceName, "");
	//
	// Mise en place de la police
	//
	pDC->SelectObject(TFont(&logfont));
	//
	// Prise des caractéristiques de la police EN UNITES LOGIQUES
	//
	pDC->GetTextMetrics(textMetric);
	//
	// Calcul de la hauteur de ligne du texte à afficher
	//
	HauteurLigne = textMetric.tmHeight + textMetric.tmExternalLeading;
	//
	// Calcul du nombre de lignes affichables
	//
	if (HauteurLigne > 0)
   	nbNom = int(long(iHauteur) / HauteurLigne);

	delete pDC;
	iActif = 0;
}

void
NSTableauNoir::EvPaint()
{
	//TGroupBox::EvPaint();
	TPaintDC  dc(*this);
	Paint(dc, dc.Ps.fErase, TRect(dc.Ps.rcPaint));
//	HWND        hCtrl;
//	PAINTSTRUCT ps;
//	hCtrl = pListeDlg->GetDlgItem(ID_LISTPA_LISTE);
//	BeginPaint(hCtrl, &ps);
//	Paint(TDC(ps.hdc), ps.fErase, TRect(ps.rcPaint));
//	EndPaint(hCtrl, &ps);
}

//---------------------------------------------------------------------------
//  Function: 	  NSTableauNoir::EvLButtonDown(UINT modKeys, TPoint& point)
//
//  Arguments:	  point -> Lieu du click souris
//
//  Description: Sélectionne une ligne comme ligne en cours
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSTableauNoir::EvLButtonDown(UINT modKeys, TPoint& point)
{
	int iLigne = point.y / HauteurLigne;
	if (iLigne < nbNom)
	{
		iActif = iLigne;
		Invalidate(true);
	}
}

//---------------------------------------------------------------------------
//  Function: 	  NSTableauNoir::EvLButtonDblClk(UINT modKeys, TPoint& point)
//
//  Arguments:	  point -> Lieu du double click souris
//
//  Description: Sélectionne une ligne comme ligne en cours
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSTableauNoir::EvLButtonDblClk(UINT modKeys, TPoint& point)
{
	int iLigne = point.y / HauteurLigne;
	if (iLigne < nbNom)
		iActif = iLigne;
	pListeDlg->CmOk();
}

void
NSTableauNoir::Paint(TDC& dc, bool erase, TRect& rect)
{
	int  		  iPosition;
	char 		  dateNaiss[15];
	TEXTMETRIC textMetric;

	NSPatientArray*  pPatArray = pListeDlg->pPatientsArray;

	dc.SetMapMode(MM_TEXT);
	dc.GetTextMetrics(textMetric);
	//
	// Calcul de la hauteur de ligne du texte à afficher
	//
	HauteurLigne = textMetric.tmHeight + textMetric.tmExternalLeading;
	//
	// Calcul du nombre de lignes affichables
	//
	int iHauteur = rectFond.Height() - 6;
	if (HauteurLigne > 0)
   	nbNom = int(long(iHauteur) / HauteurLigne);

	TColor CouleurFond;
	dc.SetBkColor(CouleurFond.LtGray);
	dc.SetTextColor(CouleurFond.Black);
	dc.SelectObject(TBrush(CouleurFond.LtGray));
	dc.FillRect(rectFond.left+3, rectFond.top+3, rectFond.left+rectFond.Width()-6, rectFond.top+iHauteur, TBrush(CouleurFond.LtGray));

	for (int i = 0; i < nbNom; i++)
	{
		if (i == iActif)
		{
			dc.SetBkColor(CouleurFond.Gray);
			dc.SetTextColor(CouleurFond.White);
		}
      iPosition = rectFond.top + (i * int(HauteurLigne));

		dc.TextOut(rectFond.left+3, iPosition+1, ((*pPatArray)[i])->pDonnees->nom_long);
		donne_date(((*pPatArray)[i])->pDonnees->naissance, dateNaiss, sLang);
		dc.TextOut(rectFond.left+170, iPosition+1, dateNaiss);
		if (i == iActif)
		{
			dc.SetBkColor(CouleurFond.LtGray);
			dc.SetTextColor(CouleurFond.Black);
		}
	}
	pListeDlg->pGrpSepare->Invalidate(true);
}

//
// We'll always erase as we paint to avoid flicker
//
bool
NSTableauNoir::EvEraseBkgnd(HDC)
{
  return true;
}

void
NSTableauNoir::DrawItem(DRAWITEMSTRUCT far &drawInfo)
{
	Paint(drawInfo.hDC, true, TRect(drawInfo.rcItem));
}

//--------------------------------------------------------------------------
// Implémentation de l'enregistrement d'une classe
//--------------------------------------------------------------------------

//---------------------------------------------------------------------------
//  Function: bool NSBitmapRegister()
//
//  Arguments:
//          AUCUN
//  Description:
//            Vérifie si la classe NSBitmap n'est pas déjà enregistrée.
//            Si elle ne l'est pas l'enregistre. ???
//  Returns:
//          true   si la classe n'était pas enregistrée
//          false  sinon
//---------------------------------------------------------------------------
bool NSTableauNoirRegister()
{
	WNDCLASS  windowClass;
	BOOL gc;   // NE PAS CHANGER CE GRAND BOOL !!!!!!!!!!
	static char className[] = "NSTableauNoir";
	// Only check for globally registered classes if not in an NT WoW box,
	// since WoW plays nasty games with class registration.
#if defined(__WIN32__)
	gc = ::GetClassInfo(NULL, "NSTableauNoir", &windowClass);
#else
	static bool isWoW = bool(::GetWinFlags()&0x4000);
	if (!isWoW)
	 	gc = ::GetClassInfo(NULL,"NSTableauNoir", &windowClass);
  	else
	 	gc = 0;
#endif
  	//if (!gc && !::GetClassInfo(0, "NSTableauNoir", &windowClass)) {
  if ( gc == 0)
  	{
	 	memset(&windowClass, 0, sizeof windowClass);
	 	windowClass.cbClsExtra 	  = 0;
	 	windowClass.cbWndExtra 	  = 0;
	 	windowClass.hInstance 	  = *::Module;
	 	windowClass.hIcon 		  = 0;
	 	windowClass.hCursor 		  = ::LoadCursor(0, IDC_CROSS);
	 	windowClass.hbrBackground = HBRUSH(COLOR_WINDOW + 1);
	 	windowClass.lpszMenuName  = 0;
	 	windowClass.lpszClassName = className;
	 	windowClass.style 		  = CS_HREDRAW|CS_VREDRAW;
	 	windowClass.lpfnWndProc   =::DefWindowProc;  //InitWndProc;
	 	return ::RegisterClass(&windowClass);
  	}
  	return true;
}
*****************************************************************************/

/////////////////////////// fin de nsrechdl.cpp /////////////////////////////////

