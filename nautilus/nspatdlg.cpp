#include <string.h>#include <cstring.h>
#include <stdio.h>

#include "nautilus\nssuper.h"
#include "nautilus\nschoisi.h"
#include "nautilus\nspatdlg.h"
#include "nautilus\nsrechdl.h"
#include "nautilus\nsresour.h"
#include "partage\nsdivfct.h"
#include "partage\ns_timer.h"
#include "nssavoir\nsbloque.h"
#include "nautilus\nsepicap.h"
#include "nautilus\nscqdoc.h"#include "nsbb\nsarc.h"#include "nsbb\ns_skins.h"#include "nsutil\md5.h"   // Cryptage MD5#include "nssavoir\nsHealthTeamMemberInterface.h"#include "nsbb\logpass.h"#include "nsbb\nsdefArch.h"#include "nsepisod\nsToDo.h"#include "pilot\NautilusPilot.hpp"#include "nsbb\tagNames.h"
#include "nsutil\md5.h"boolpatSortByNameInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	if (pPat1->getNom() != pPat2->getNom())
		return (pseumaj(pPat1->getNom()) < pseumaj(pPat2->getNom())) ;
	return (pseumaj(pPat1->getPrenom()) < pseumaj(pPat2->getPrenom())) ;
}

bool
patSortByNameSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	if (pPat1->getNom() != pPat2->getNom())
		return (pseumaj(pPat1->getNom()) > pseumaj(pPat2->getNom())) ;
	return (pseumaj(pPat1->getPrenom()) > pseumaj(pPat2->getPrenom())) ;
}

bool
patSortByBirthInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	return (pPat1->getNaissance() < pPat2->getNaissance()) ;
}

bool
patSortByBirthSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	return (pPat1->getNaissance() > pPat2->getNaissance()) ;
}

bool
patSortBySexInf(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	return (pPat1->getSexe() < pPat2->getSexe()) ;
}

bool
patSortBySexSup(NSPatInfo *pPat1, NSPatInfo *pPat2)
{
	return (pPat1->getSexe() > pPat2->getSexe()) ;
}

// -----------------------------------------------------------------------------
//
// méthodes de CodeUtilisateurDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(CodeUtilisateurDialog, NSUtilDialog)
  EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE ;

CodeUtilisateurDialog::CodeUtilisateurDialog(TWindow *pere, string sCode, NSContexte *pCtx)
                      :NSUtilDialog(pere, pCtx, "IDD_CODE_UTIL")
{
  sCodeUtil = sCode ;

  pCode 	     = new NSUtilEdit(pContexte, this, IDC_CODE_UTIL_CODE,    UTI_CODE_LEN) ;
  pConfirm     = new NSUtilEdit(pContexte, this, IDC_CODE_UTIL_CONFIRM, UTI_CODE_LEN) ;

  pPassText    = new OWL::TStatic(this, IDC_TEXT_UTIL_CODE) ;
  pConfirmText = new OWL::TStatic(this, IDC_TEXT_UTIL_CONFIRM) ;
}

CodeUtilisateurDialog::~CodeUtilisateurDialog()
{
  delete pCode ;
  delete pConfirm ;
  delete pPassText ;
  delete pConfirmText ;
}

void
CodeUtilisateurDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  string sCaption = pContexte->getSuperviseur()->getText("passwordSetDialog", "userPassword") ;
  if (string("") != sCaption)
    SetCaption(sCaption.c_str()) ;

  pCode->SetText(sCodeUtil.c_str()) ;
  pConfirm->SetText(sCodeUtil.c_str()) ;

  string sSetPass = pContexte->getSuperviseur()->getText("passwordSetDialog", "enterPassword") ;
  pPassText->SetText(sSetPass.c_str()) ;
  string sConfirmPass = pContexte->getSuperviseur()->getText("passwordSetDialog", "confirmPassword") ;
  pConfirmText->SetText(sConfirmPass.c_str()) ;
}

void
CodeUtilisateurDialog::CmOk()
{
  bool   bCode     = false ;
  bool   bConfirm  = false ;

  string sCode = pCode->GetText(UTI_CODE_LEN) ;

  // contrôle préalable : la longueur du code (mot de passe) doit être >= 8 caractères
  if (strlen(sCode.c_str()) < 8)
  {
    erreur("La longueur du mot de passe doit être supérieure ou égale à 8 caractères.", warningError, 0, GetHandle());
    pCode->SetText(sCodeUtil.c_str()) ;
    pConfirm->SetText(sCodeUtil.c_str()) ;
    return;
  }

  // Le code ne doit pas contenir d'espace
  size_t pos = sCode.find(' ') ;
  if (NPOS != pos)
  {
    erreur("Votre mot de passe ne doit pas contenir d'espaces.", warningError, 0, GetHandle());
    pCode->SetText(sCodeUtil.c_str()) ;
    pConfirm->SetText(sCodeUtil.c_str()) ;
    return;
  }

  // chiffrage du mot de passe à l'aide de l'algo MD5
  string  sPass         = sCode ;
  string  sPassEncrypt  = MD5_encrypt(sPass) ;

  sCodeUtil             = sPassEncrypt ;
  size_t  iCodeUtilLen  = strlen(sCodeUtil.c_str()) ;
  if ((iCodeUtilLen > 0) && (iCodeUtilLen <= UTI_CODE_LEN))
    bCode = true ;

  string sPassConfirm = pConfirm->GetText(UTI_CODE_LEN) ;

  // chiffrage du mot de passe confirmé à l'aide de l'algo MD5
  string  sPassConfirmEncrypt   = MD5_encrypt(sPassConfirm) ;

  string sCodeConfirm        = sPassConfirmEncrypt ;
  size_t iCodeUtilConfirmLen = strlen(sCodeConfirm.c_str()) ;
  if ((iCodeUtilConfirmLen > 0) && (iCodeUtilConfirmLen <= UTI_CODE_LEN))
    bConfirm = true ;

  if (false == bCode)
  {
    erreur("Vous n'avez pas saisi le mot de passe.", warningError, 0, GetHandle()) ;
    return ;
  }

  if (false == bConfirm)
  {
    erreur("Vous n'avez pas confirmé le mot de passe.", warningError, 0, GetHandle()) ;
    return ;
  }

  if (sCodeUtil != sCodeConfirm)
  {
    erreur("Le code et la confirmation ne sont pas identiques.", warningError, 0, GetHandle()) ;
    return ;
  }

  TDialog::CmOk() ;
}

void
CodeUtilisateurDialog::CmCancel()
{
	sCodeUtil = string("") ;

	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------------------
//
// méthodes de ModifCodeUtilisateurDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ModifCodeUtilisateurDialog, NSUtilDialog)
  EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE ;


ModifCodeUtilisateurDialog::ModifCodeUtilisateurDialog(TWindow *pere, NSContexte *pCtx)
                           :NSUtilDialog(pere, pCtx, "IDD_OLDCODE_UTIL")
{
  sCodeUtil = "" ;

  pCode = new NSUtilEdit(pContexte, this, IDC_CODE_UTIL_CODE, UTI_CODE_LEN) ;
}


ModifCodeUtilisateurDialog::~ModifCodeUtilisateurDialog()
{
  delete pCode ;
}


void
ModifCodeUtilisateurDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  pCode->SetText(sCodeUtil.c_str()) ;
}

void
ModifCodeUtilisateurDialog::CmOk()
{
  bool bCode = false ;

  string sPass = pCode->GetText(UTI_CODE_LEN) ;

  // chiffrage du mot de passe à l'aide de l'algo MD5
  string  sPassEncrypt  = MD5_encrypt(sPass) ;

  sCodeUtil             = sPassEncrypt ;
  size_t  iCodeUtilLen  = strlen(sCodeUtil.c_str()) ;
  if ((iCodeUtilLen > 0) && (iCodeUtilLen <= UTI_CODE_LEN))
    bCode = true ;

  if (false == bCode)
  {
    erreur("Vous n'avez pas saisi le mot de passe.", warningError, 0, GetHandle()) ;
    return ;
  }

  TDialog::CmOk() ;
}


void
ModifCodeUtilisateurDialog::CmCancel()
{
	sCodeUtil = "" ;
	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------------------
//
//  Méthodes de CreerUtilisateurDialog en mode N_TIERS
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerUtilisateurDialog, NSUtilDialog)
  EV_WM_CLOSE,
  EV_COMMAND(IDOK,                      CmOk),
  EV_COMMAND(IDCANCEL,                  CmCancel),
  EV_BN_CLICKED(IDC_UTIL_TITRE1,        CmTitre),
  EV_BN_CLICKED(IDC_UTIL_TITRE2,        CmTitre),
  EV_BN_CLICKED(IDC_UTIL_TITRE3,        CmCivilProf),
  EV_BN_CLICKED(IDC_UTIL_TITRE4,        CmCivilProf),
  EV_BN_CLICKED(IDC_UTIL_TITRE5,        CmCivilProf),
  EV_BN_CLICKED(IDC_UTIL_TITRE6,        CmCivilProf),
  EV_BN_CLICKED(IDC_UTIL_TITRE7,        CmCivil),
  EV_BN_CLICKED(IDC_UTIL_TITRE8,        CmCivil),
  EV_BN_CLICKED(IDC_UTIL_TITRE9,        CmCivil),
  EV_BN_CLICKED(IDC_UTIL_PWD_MODIFY,    CmCode),
  EV_BN_CLICKED(IDC_UTIL_ROLE_USER,     CmRole),
  EV_BN_CLICKED(IDC_UTIL_ROLE_ADMIN,    CmRole),
  EV_BN_CLICKED(IDC_UTIL_PWD_TEMP,      CmAccount),
  EV_BN_CLICKED(IDC_UTIL_PWD_CHANGE,    CmAccount),
  EV_BN_CLICKED(IDC_UTIL_PWD_FIXE,      CmAccount),
  EV_BN_CLICKED(IDC_UTIL_PWD_NOCHANGE,  CmAccount),
  EV_BN_CLICKED(IDC_UTIL_PWD_CANCEL,    CmAccount),
  EV_BN_CLICKED(IDC_UTIL_PWD_LOCK,      CmAccount),
END_RESPONSE_TABLE ;

CreerUtilisateurDialog::CreerUtilisateurDialog(TWindow *pere, NSContexte *pCtx, TModule *module)
                       :NSUtilDialog(pere, pCtx, "IDD_CREAT_UTIL1_NTIERS", module)
{
  // fichiers d'aide
  sHindex                 = string("") ;
  sHcorps                 = string("Creation_Modification.html") ;

  // Initialisation des donnees
  pData 	                = new NSUtilisateurData ;
  sLogin                  = string("") ;
  sPasswd                 = string("") ;
  sUserType               = string("") ;
  sPasswordType           = string("") ;
  sDatePasswordCreation   = string("") ;
  sValidityDuration       = string("") ;

  // Création de tous les "objets de contrôle"
  pTitre                  = new TGroupBox(this, IDC_UTIL_TITRE) ;
  pCivilProf              = new TGroupBox(this, IDC_UTIL_CIVILPROF) ;
  pCivil                  = new TGroupBox(this, IDC_UTIL_CIVIL) ;
  iTitre                  = 0 ;
  sTitre                  = string("") ;
  sCivilProf              = string("") ;
  sCivil                  = string("") ;

  pNameText               = new OWL::TStatic(this, IDC_UTITEXT_NOM) ;
  pNom                    = new NSUtilEdit(pContexte, this, IDC_UTIL_NOM, UTI_NOM_LEN) ;
  pFirstNameText          = new OWL::TStatic(this, IDC_UTITEXT_PRENOM) ;
  pPrenom                 = new NSUtilEdit(pContexte, this, IDC_UTIL_PRENOM, UTI_PRENOM_LEN) ;
  pRppsText               = new OWL::TStatic(this, IDC_UTITEXT_RPPS) ;
  pRpps                   = new NSUtilEdit(pContexte, this, IDC_UTIL_RPPS, UTI_PRENOM_LEN) ;
  pAdeliText              = new OWL::TStatic(this, IDC_UTITEXT_ADELI) ;
  pAdeli                  = new NSUtilEdit(pContexte, this, IDC_UTIL_ADELI, UTI_PRENOM_LEN) ;

  pUserIdText             = new OWL::TStatic(this, IDC_UTITEXT_USERID) ;
  pLogin                  = new NSUtilEdit(pContexte, this, IDC_UTIL_LOGIN, UTI_LOGIN_LEN) ;
  pJobText                = new OWL::TStatic(this, IDC_UTITEXT_METIER) ;
  pMetier                 = new NSUtilLexique(pContexte, this, IDC_UTIL_METIER, pContexte->getDico()) ;
  pSpecText               = new OWL::TStatic(this, IDC_UTITEXT_SPEC) ;
  pSpec                   = new NSUtilLexique(pContexte, this, IDC_UTIL_SPEC, pContexte->getDico()) ;
  pCode                   = new TButton(this, IDC_UTIL_PWD_MODIFY) ;

  pRole                   = new TGroupBox(this, IDC_UTIL_ROLE) ;
  pUserRole               = new TRadioButton(this, IDC_UTIL_ROLE_USER, pRole) ;
  pAdminRole              = new TRadioButton(this, IDC_UTIL_ROLE_ADMIN, pRole) ;

  pAccount                = new TGroupBox(this, IDC_UTIL_ACCOUNT) ;
  pPwdTemp                = new TRadioButton(this, IDC_UTIL_PWD_TEMP, pAccount) ;
  pPwdChange              = new TRadioButton(this, IDC_UTIL_PWD_CHANGE, pAccount) ;
  pDaysText               = new OWL::TStatic(this, IDC_UTITEXT_DAYS) ;
  pPwdChangeUpDown        = new NSUtilUpDownEdit(this, pContexte, IDC_UTIL_PWD_DUREE, IDC_UTIL_PWD_UPDOWN) ;
  pPwdFix                 = new TRadioButton(this, IDC_UTIL_PWD_FIXE, pAccount) ;
  pPwdNoChange            = new TRadioButton(this, IDC_UTIL_PWD_NOCHANGE, pAccount) ;
  pPwdCancel              = new TRadioButton(this, IDC_UTIL_PWD_CANCEL, pAccount) ;
  pPwdLock                = new TRadioButton(this, IDC_UTIL_PWD_LOCK, pAccount) ;

  pTitre1                 = new TRadioButton(this, IDC_UTIL_TITRE1, pTitre) ;
  pTitre2                 = new TRadioButton(this, IDC_UTIL_TITRE2, pTitre) ;
  pTitre3                 = new TRadioButton(this, IDC_UTIL_TITRE3, pCivilProf) ;
  pTitre4                 = new TRadioButton(this, IDC_UTIL_TITRE4, pCivilProf) ;
  pTitre5                 = new TRadioButton(this, IDC_UTIL_TITRE5, pCivilProf) ;
  pTitre6                 = new TRadioButton(this, IDC_UTIL_TITRE6, pCivilProf) ;
  pTitre7                 = new TRadioButton(this, IDC_UTIL_TITRE7, pCivil) ;
  pTitre8                 = new TRadioButton(this, IDC_UTIL_TITRE8, pCivil) ;
  pTitre9                 = new TRadioButton(this, IDC_UTIL_TITRE9, pCivil) ;

  pSexe                   = new TGroupBox(this, IDC_UTIL_SEXE) ;
  pSexe1                  = new TRadioButton(this, IDC_UTIL_SEXE1, pSexe) ;
  pSexe2                  = new TRadioButton(this, IDC_UTIL_SEXE2, pSexe) ;

  pMailText               = new OWL::TStatic(this, IDC_UTITEXT_EMAIL) ;
  pEMail                  = new NSUtilEdit(pContexte, this, IDC_UTIL_EMAIL, UTI_MESSAGERIE_LEN) ;

  pLanguageText           = new OWL::TStatic(this, IDC_UTITEXT_LANG) ;
  pLangues                = new NSComboBoxClassif(this, IDC_UTIL_COMBO_LANG, pContexte, "66391")  ;
}

CreerUtilisateurDialog::~CreerUtilisateurDialog()
{
  delete pMailText ;
  delete pEMail ;
  delete pUserIdText ;
  delete pLogin ;
  delete pJobText ;
  delete pMetier ;  delete pSpecText ;  delete pSpec ;  delete pSexe1 ;  delete pSexe2 ;  delete pSexe ;  delete pTitre9 ;  delete pTitre8 ;  delete pTitre7 ;  delete pTitre6 ;  delete pTitre5 ;
  delete pTitre4 ;
  delete pTitre3 ;
  delete pTitre2 ;
  delete pTitre1 ;
  delete pPwdTemp ;
  delete pPwdChange ;
  delete pDaysText ;
  delete pPwdChangeUpDown ;
  delete pPwdFix ;
  delete pPwdNoChange ;
  delete pPwdCancel ;
  delete pPwdLock ;
  delete pAccount ;
  delete pUserRole ;
  delete pAdminRole ;
  delete pRole ;
  delete pCode ;
  delete pFirstNameText ;
  delete pPrenom ;
  delete pNameText ;
  delete pNom ;
  delete pRppsText ;
  delete pRpps ;
  delete pAdeliText ;
  delete pAdeli ;
  delete pTitre ;
  delete pCivilProf ;
  delete pCivil ;
  delete pData ;
  delete pLanguageText ;
  delete pLangues ;
}

void
CreerUtilisateurDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  pNom->SetText(pData->_sNom.c_str()) ;
  pPrenom->SetText(pData->_sPrenom.c_str()) ;

  string sPasswordCaption = string("") ;
  if (sPasswd != "")
    sPasswordCaption = pContexte->getSuperviseur()->getText("userDialog", "changePassword") ;
  else
    sPasswordCaption = pContexte->getSuperviseur()->getText("userDialog", "setPassword") ;
  pCode->SetCaption(sPasswordCaption.c_str()) ;

  // titre : voir la fonction StringTitre de nsperson.cpp
  /******************************************************
  switch (pData->type[0])
  {
    case 'O'  : pTitre1->Check() ; iTitre = 1 ; break ;
    case 'P'  : pTitre2->Check() ; iTitre = 2 ; break ;
    case '1'  : pTitre3->Check() ; iTitre = 3 ; break ;
    case '2'  : pTitre4->Check() ; iTitre = 4 ; break ;
    case 'R'  : pTitre5->Check() ; iTitre = 5 ; break ;
    case 'M'  : pTitre6->Check() ; iTitre = 6 ; break ;
    case 'A'  : pTitre7->Check() ; iTitre = 7 ; break ;
    case 'B'  : pTitre8->Check() ; iTitre = 8 ; break ;
    case 'C'  : pTitre9->Check() ; iTitre = 9 ; break;
  }
  ********************************************************/

  if (string("") != sTitre)
  {
    if      (string("HDOCT1") == sTitre)
    	pTitre1->Check() ;
    else if (string("HPROF1") == sTitre)
    	pTitre2->Check() ;
  }

  if (string("") != sCivilProf)
  {
    if      (string("HMOND1") == sCivilProf)
    	pTitre3->Check() ;
    else if (string("HMADD1") == sCivilProf)
    	pTitre4->Check() ;
    else if (string("HMONF1") == sCivilProf)
    	pTitre5->Check() ;
    else if (string("HMADP1") == sCivilProf)
    	pTitre6->Check() ;
  }

  if (string("") != sCivil)
  {
    if      (string("HMONP1") == sCivil)
    	pTitre7->Check() ;
    else if (string("HMADR1") == sCivil)
    	pTitre8->Check() ;
    else if (string("HMADE1") == sCivil)
    	pTitre9->Check() ;
  }

  // le champ login existe que dans la version MUE
  pLogin->SetText(sLogin.c_str()) ;
  if (strlen((pData->_sMetier).c_str()) > 0)
  	pMetier->setLabel((pData->_sMetier).c_str()) ;
  if (strlen((pData->_sSpecialite).c_str()) > 0)
  	pSpec->setLabel((pData->_sSpecialite).c_str()) ;

	if (string("") != pData->_sSexe)
  {
    if      (pData->estMasculin())
    	pSexe1->Check() ;
    else if (pData->estFeminin())
    	pSexe2->Check() ;
  }

  pEMail->SetText(pData->_sMessagerie.c_str()) ;
  pRpps->SetText(pData->_sRpps.c_str()) ;
  pAdeli->SetText(pData->_sProfessionalId.c_str()) ;

  // langue
  pLangues->setCode(pData->_sLang) ;

  initRole() ;
  initAccount() ;
  initTexts() ;
}


void
CreerUtilisateurDialog::CmTitre()
{
  // docteur
  if (pTitre1->GetCheck() == BF_CHECKED)
  {
    if (sTitre == "HDOCT1")
    {
      pTitre1->Uncheck() ;
      sTitre = "" ;
    }
    else
      sTitre = "HDOCT1" ;
  }

  // professeur
  if (pTitre2->GetCheck() == BF_CHECKED)
  {
    if (sTitre == "HPROF1")
    {
      pTitre2->Uncheck() ;
      sTitre = "" ;
    }
    else
      sTitre = "HPROF1" ;
  }
}

void
CreerUtilisateurDialog::CmCivilProf()
{
    // M. le Docteur
    if (pTitre3->GetCheck() == BF_CHECKED)
  {
    if (sCivilProf == "HMOND1")
    {
      pTitre3->Uncheck() ;
      sCivilProf = "" ;
    }
    else
      sCivilProf = "HMOND1" ;
  }

  // Mme le Docteur
  if (pTitre4->GetCheck() == BF_CHECKED)
  {
    if (sCivilProf == "HMADD1")
    {
      pTitre4->Uncheck() ;
      sCivilProf = "" ;
    }
    else
      sCivilProf = "HMADD1" ;
  }

  // M. le Professeur
  if (pTitre5->GetCheck() == BF_CHECKED)
  {
    if (sCivilProf == "HMONF1")
    {
      pTitre5->Uncheck() ;
      sCivilProf = "" ;
    }
    else
     sCivilProf = "HMONF1" ;
  }

  // Mme le professeur
  if (pTitre6->GetCheck() == BF_CHECKED)
  {
    if (sCivilProf == "HMADP1")
    {
      pTitre6->Uncheck() ;
      sCivilProf = "" ;
    }
    else
      sCivilProf = "HMADP1" ;
  }
}

void
CreerUtilisateurDialog::CmCivil()
{
    // Monsieur
    if (pTitre7->GetCheck() == BF_CHECKED)
  {
    if (sCivil == "HMONP1")
    {
      pTitre7->Uncheck() ;
      sCivil = "" ;
    }
    else
      sCivil = "HMONP1" ;
  }

  // Madame
  if (pTitre8->GetCheck() == BF_CHECKED)
  {
    if (sCivil == "HMADR1")
    {
      pTitre8->Uncheck() ;
      sCivil = "" ;
    }
    else
      sCivil = "HMADR1" ;
  }

  // Mademoiselle
  if (pTitre9->GetCheck() == BF_CHECKED)
  {
    if (sCivil == "HMADE1")
    {
      pTitre9->Uncheck() ;
      sCivil = "" ;
    }
    else
      sCivil = "HMADE1" ;
  }
}

void
CreerUtilisateurDialog::CmRole()
{
  if (pUserRole->GetCheck() == BF_CHECKED)
  {
    if (sUserType.find("U") != string::npos)
    {
      pUserRole->Uncheck() ;
      enleveDroit(sUserType, 'U') ;
    }
    else
      ajouteDroit(sUserType, 'U') ;
  }

  if (pAdminRole->GetCheck() == BF_CHECKED)
  {
    if (sUserType.find("A") != string::npos)
    {
      pAdminRole->Uncheck() ;
      enleveDroit(sUserType, 'A') ;
    }
    else
      ajouteDroit(sUserType, 'A') ;
  }

  initRole() ;
}


void
CreerUtilisateurDialog::CmAccount()
{
  if (pPwdTemp->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("T") != string::npos)
    {
      pPwdTemp->Uncheck() ;
      enleveDroit(sPasswordType, 'T') ;
    }
    else
    {
      if (sPasswordType.find("G") != string::npos)
      {
        pPwdNoChange->Uncheck() ;
        enleveDroit(sPasswordType, 'G') ;
      }
      ajouteDroit(sPasswordType, 'T') ;
    }
  }

  if (pPwdChange->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("J") != string::npos)
    {
      pPwdChange->Uncheck() ;
      enleveDroit(sPasswordType, 'J') ;
    }
    else
    {
      if (sPasswordType.find("F") != string::npos)
      {
        pPwdFix->Uncheck() ;
        enleveDroit(sPasswordType, 'F') ;
      }
      ajouteDroit(sPasswordType, 'J') ;
    }
  }

  if (pPwdFix->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("F") != string::npos)
    {
      pPwdFix->Uncheck() ;
      enleveDroit(sPasswordType, 'F') ;
    }
    else
    {
      if (sPasswordType.find("J") != string::npos)
      {
        pPwdChange->Uncheck() ;
        enleveDroit(sPasswordType, 'J') ;
      }
      ajouteDroit(sPasswordType, 'F') ;
    }
  }

  if (pPwdNoChange->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("G") != string::npos)
    {
      pPwdNoChange->Uncheck() ;
      enleveDroit(sPasswordType, 'G') ;
    }
    else
    {
      if (sPasswordType.find("T") != string::npos)
      {
        pPwdTemp->Uncheck() ;
        enleveDroit(sPasswordType, 'T') ;
      }
      ajouteDroit(sPasswordType, 'G') ;
    }
  }

  if (pPwdCancel->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("D") != string::npos)
    {
      pPwdCancel->Uncheck() ;
      enleveDroit(sPasswordType, 'D') ;
    }
    else
    {
      if (sPasswordType.find("V") != string::npos)
      {
        pPwdLock->Uncheck() ;
        enleveDroit(sPasswordType, 'V') ;
      }
      ajouteDroit(sPasswordType, 'D') ;
    }
  }

  if (pPwdLock->GetCheck() == BF_CHECKED)
  {
    if (sPasswordType.find("V") != string::npos)
    {
      pPwdLock->Uncheck() ;
      enleveDroit(sPasswordType, 'V') ;
    }
    else
    {
      if (sPasswordType.find("D") != string::npos)
      {
        pPwdCancel->Uncheck() ;
        enleveDroit(sPasswordType, 'D') ;
      }
      ajouteDroit(sPasswordType, 'V') ;
    }
  }

  initAccount() ;
}


void
CreerUtilisateurDialog::CmCode()
{
  // modification d'un code utilisateur
  // on ne contrôle pas l'ancien mot de passe pour l'administrateur
  // Saisie du nouveau mot de passe
  CodeUtilisateurDialog *pCodeDlg = new CodeUtilisateurDialog(this, "", pContexte) ;

  if (pCodeDlg->Execute() == IDCANCEL)
  {
    delete pCodeDlg ;
    return ;
  }

  sPasswd = pCodeDlg->sCodeUtil ;
  sDatePasswordCreation = donne_date_duJour() ;
  delete pCodeDlg ;

  string sPasswordCaption = string("") ;
  if (string("") != sPasswd)
    sPasswordCaption = pContexte->getSuperviseur()->getText("userDialog", "changePassword") ;
  else
    sPasswordCaption = pContexte->getSuperviseur()->getText("userDialog", "setPassword") ;
  pCode->SetCaption(sPasswordCaption.c_str()) ;
}

void
CreerUtilisateurDialog::CmOk()
{
  bool bNom   = false ;
  bool bCode  = false ;
  bool bLogin = false ;

  string sNom = pNom->GetText(UTI_NOM_LEN) ;
  strip(sNom) ;
  pData->_sNom = sNom ;
  if (string("") != pData->_sNom)
    bNom = true ;

  string sPrenom = pPrenom->GetText(COR_PRENOM_LEN) ;
  strip(sPrenom) ;
  pData->_sPrenom = sPrenom ;

  if (string("") != sPasswd)
    bCode = true ;

  if (pTitre1->GetCheck() == BF_CHECKED)
  	pData->_sType = string("HDOCT1") ;
  if (pTitre2->GetCheck() == BF_CHECKED)
    pData->_sType = string("HPROF1") ;
  if (pTitre3->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMOND1") ;
  if (pTitre4->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMADD1") ;
  if (pTitre5->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMONF1") ;
  if (pTitre6->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMADP1") ;
  if (pTitre7->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMONP1") ;
  if (pTitre8->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMADR1") ;
  if (pTitre9->GetCheck() == BF_CHECKED)
    pData->_sType = string("HMADE1") ;

  // sexe
  if      (pSexe1->GetCheck() == BF_CHECKED)
  	pData->metMasculin() ;
  else if (pSexe2->GetCheck() == BF_CHECKED)
    pData->metFeminin() ;

  pData->_sMessagerie = pEMail->GetText(UTI_MESSAGERIE_LEN) ;

  pData->_sRpps           = pRpps->GetText(255) ;
  pData->_sProfessionalId = pAdeli->GetText(255) ;

  if (sPasswordType.find("J") != NPOS)
  {
    char szDuration[80];

    int iDuration = pPwdChangeUpDown->getValue();
    itoa(iDuration, szDuration, 10);
    sValidityDuration = string(szDuration);
  }
  else
    sValidityDuration = "";

  // on met en dur la langue par défaut à fr
  // à changer "in the future"
  string sLangCode = pLangues->getSelCode() ;
  if (sLangCode == string(""))
  	pData->_sLang = string("fr") ;
  else
  	pData->_sLang = sLangCode ;

  sLogin = pLogin->GetText(UTI_LOGIN_LEN) ;
  if (string("") != sLogin)
    bLogin = true ;

  pData->_sMetier = pMetier->getCode() ;
  pData->_sSpecialite = pSpec->getCode() ;

  if (!bNom)
  {
    erreur("Le champ Nom est obligatoire.", warningError, 0, GetHandle()) ;
    return ;
  }

  if (!bCode)
  {
    erreur("Le mot de passe est obligatoire.", warningError, 0, GetHandle()) ;
    return ;  }

  // en version MUE le login est obligatoire
  if (!bLogin)
  {
    erreur("Le champ Identifiant utilisateur est obligatoire.", warningError, 0, GetHandle()) ;
    return ;
  }

  TDialog::CmOk() ;
}

void
CreerUtilisateurDialog::CmCancel()
{
  string  sCaption  = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  int     retVal    = MessageBox("Voulez-vous vraiment annuler la création/modification de cet utilisateur ?", sCaption.c_str(), MB_YESNO) ;

  if (retVal == IDYES)
    TDialog::CmCancel() ;
}

void
CreerUtilisateurDialog::EvClose()
{
  CmCancel() ;
}

void
CreerUtilisateurDialog::ajouteDroit(string& sTypeDroit, char droit)
{
	sTypeDroit += droit ;
}

void
CreerUtilisateurDialog::enleveDroit(string& sTypeDroit, char droit)
{
  size_t pos = sTypeDroit.find(droit) ;
  string debut = "", fin = "" ;

  if (pos != string::npos)
  {
    debut = string(sTypeDroit, 0, pos) ;
    if (pos < (strlen(sTypeDroit.c_str()) - 1))
    	fin = string(sTypeDroit, pos + 1, strlen(sTypeDroit.c_str()) - pos - 1) ;
    sTypeDroit = debut + fin ;
  }
}

void
CreerUtilisateurDialog::initRole()
{
	bool	bUserTypeDefine	= false ;

	if (sUserType.find("U") != string::npos)
  {
  	pUserRole->Check() ;
    bUserTypeDefine = true ;
  }
  if (sUserType.find("A") != string::npos)
  {
  	pAdminRole->Check() ;
    bUserTypeDefine = true ;
  }

  // traitement par défaut
  if (!bUserTypeDefine)
  {
  	pUserRole->Check() ;
    ajouteDroit(sUserType, 'U');
  }
}

void
CreerUtilisateurDialog::initAccount()
{
  // If no params, set default value to "permanent password"
  //
  if (string("") == sPasswordType)
    sPasswordType = string(1, 'F') ;

  // Compte et mot de passe :
  // PASSWORD_MUST_CHANGE     T
  // PASSWORD_VALID_DURING    J
  // PASSWORD_FIXE            F
  // USER_PRIVILEGE           G
  // LOCKED_USER              V
  // DESEABLED_USER           D

  if (sPasswordType.find("T") != string::npos)
    pPwdTemp->Check() ;

  if (sPasswordType.find("J") != string::npos)
  {
    pPwdChange->Check() ;
    pPwdChangeUpDown->setValue(atoi(sValidityDuration.c_str())) ;
  }

  if (sPasswordType.find("F") != string::npos)
    pPwdFix->Check() ;

  if (sPasswordType.find("G") != string::npos)
    pPwdNoChange->Check() ;

  if (sPasswordType.find("D") != string::npos)
    pPwdCancel->Check() ;

  if (sPasswordType.find("V") != string::npos)
    pPwdLock->Check() ;
}

void
CreerUtilisateurDialog::initTexts()
{
  string sText = pContexte->getSuperviseur()->getText("userDialog", "famillyName") ;
  pNameText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "givenName") ;
  pFirstNameText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "userLogin") ;
  pUserIdText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "gender") ;
  pSexe->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "male") ;
  pSexe1->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "female") ;
  pSexe2->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "title") ;
  pTitre->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "doctor") ;
  pTitre1->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "professor") ;
  pTitre2->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "professionalCivility") ;
  pCivilProf->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "mrDoctor") ;
  pTitre3->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "msDoctor") ;
  pTitre4->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "mrProfessor") ;
  pTitre5->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "msProfessor") ;
  pTitre6->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "civility") ;
  pCivil->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "mister") ;
  pTitre7->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "miss") ;
  pTitre8->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "misses") ;
  pTitre9->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "email") ;
  pMailText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "RppsId") ;
  pRppsText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "professionalId") ;
  pAdeliText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "job") ;
  pJobText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "specialty") ;
  pSpecText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "role") ;
  pRole->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "user") ;
  pUserRole->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "administrator") ;
  pAdminRole->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "accountProperties") ;
  pAccount->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "passwordToChangeAtNextLogin") ;
  pPwdTemp->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "passwordValidFor") ;
  pPwdChange->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "permanentPassword") ;
  pPwdFix->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "userCannotChangePassword") ;
  pPwdNoChange->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "inactivatedAccount") ;
  pPwdCancel->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "lockedAccount") ;
  pPwdLock->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "days") ;
  pDaysText->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("userDialog", "language") ;
  pLanguageText->SetCaption(sText.c_str()) ;
}

// -----------------------------------------------------------------------------
//
//  Méthodes de NSPersonImportDialog en mode N_TIERS
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSUserImportDialog, NSUtilDialog)
  EV_COMMAND(IDOK,                      CmOk),
  EV_COMMAND(IDCANCEL,                  CmCancel),
  EV_EN_CHANGE(IDC_IMPORTUSER_PWDUSER,   PasswordChange),
END_RESPONSE_TABLE ;

NSUserImportDialog::NSUserImportDialog(TWindow * pere, NSContexte *pCtx, TModule *module)
                   :NSUtilDialog(pere, pCtx, "IDD_IMPORTUSERLDV", module)
{
	pLogin  = new NSUtilEdit(pContexte, this, IDC_IMPORTUSER_IDENTUSER, 80) ;
	pPasswd = new NSUtilEdit(pContexte, this, IDC_IMPORTUSER_PWDUSER, 80) ;
	// pNumLdv = new NSUtilEdit(this, IDC_IMPORTLDV_ADELI, 80);
	// pAdeli  = new NSUtilEdit(this, IDC_IMPORTLDV_NUMLDV, 80);

	// pRole    = new TGroupBox(this, IDC_IMPORTLDV_ROLE) ;
	// pRoleUti = new TRadioButton(this, IDC_IMPORTLDV_USER, pRole) ;
	// pRoleCor = new TRadioButton(this, IDC_IMPORTLDV_CORRESP, pRole) ;
}

NSUserImportDialog::~NSUserImportDialog()
{
	delete pLogin ;
	delete pPasswd ;
	// delete pAdeli ;
	// delete pNumLdv ;
	// delete pRoleUti ;
	// delete pRoleCor ;
	// delete pRole ;
}

void
NSUserImportDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	if (pContexte->getUtilisateur()->haveGlobalSessionPassword())
	{
		pLogin->Insert(pContexte->getUtilisateur()->getGlobalLoginSession().c_str()) ;
		pPasswd->Insert(pContexte->getUtilisateur()->getGlobalPasswordSession().c_str()) ;
		isHached = true ;
	}
	else
		isHached = false ;
}

void
NSUserImportDialog::PasswordChange()
{
	isHached = false;
}

void
NSUserImportDialog::CmOk()
{
	sLogin  = pLogin->GetText(80) ;
	sPasswd = pPasswd->GetText(80) ;
	if (false == isHached)
		sPasswd = MD5_encrypt(sPasswd) ;

    // pAdeli->GetText(adeli, 80);
    // sAdeli = string(adeli);

    // pNumLdv->GetText(numldv, 80);
    // sNumLdv = string(numldv);

	if ((sLogin == "") || (sPasswd == ""))
	{
		erreur("Vous devez définir un login et un mot de passe pour la personne à importer.", warningError, 0) ;
		return ;
	}

    /******************************************
    if ((sAdeli == "") && (sNumLdv == ""))
    {
        erreur("Vous devez définir un numéro ADELI ou Ligne de Vie pour la personne à importer.", 0, 0);
        return;
    }
    ********************************************/

    NSUtilDialog::CmOk();
}

void
NSUserImportDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}

// -----------------------------------------------------------------------------
//
//  Méthodes de NSPersonImportDialog en mode N_TIERS
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSCorrespImportDialog, NSUtilDialog)
  EV_COMMAND(IDOK,                      CmOk),
  EV_COMMAND(IDCANCEL,                  CmCancel),
  EV_EN_CHANGE(IDC_IMPORTCOR_PWDUSER,   PasswordChange),
END_RESPONSE_TABLE ;

NSCorrespImportDialog::NSCorrespImportDialog(TWindow * pere, NSContexte *pCtx, TModule *module)
                      :NSUtilDialog(pere, pCtx, "IDD_IMPORTCORRESPLDV", module)
{
	pLogin  = new NSUtilEdit(pContexte, this, IDC_IMPORTCOR_IDENTUSER, 80);
	pPasswd = new NSUtilEdit(pContexte, this, IDC_IMPORTCOR_PWDUSER, 80);
	pAdeli  = new NSUtilEdit(pContexte, this, IDC_IMPORTCOR_ADELI, 80);
	pNumLdv = new NSUtilEdit(pContexte, this, IDC_IMPORTCOR_NUMLDV, 80);

}

NSCorrespImportDialog::~NSCorrespImportDialog()
{
	delete pLogin ;
	delete pPasswd ;
	delete pAdeli ;
	delete pNumLdv ;
}

void
NSCorrespImportDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	if (pContexte->getUtilisateur()->haveGlobalSessionPassword())
	{
		pLogin->Insert(pContexte->getUtilisateur()->getGlobalLoginSession().c_str()) ;
		pPasswd->Insert(pContexte->getUtilisateur()->getGlobalPasswordSession().c_str()) ;
		isHached = true ;
	}
	else
		isHached = false ;
}

void
NSCorrespImportDialog::PasswordChange()
{
	isHached = false;
}

void
NSCorrespImportDialog::CmOk()
{
	sLogin  = pLogin->GetText(80) ;
	sPasswd = pPasswd->GetText(80) ;
	if (isHached  == false)
		sPasswd = MD5_encrypt(sPasswd) ;

	sAdeli  = pAdeli->GetText(80) ;
	sNumLdv = pNumLdv->GetText(80) ;

	if ((sLogin == "") || (sPasswd == ""))
	{
		erreur("Vous devez définir un login et un mot de passe pour l'utilisateur en cours.", warningError, 0) ;
		return;
	}

	if ((sAdeli == "") && (sNumLdv == ""))
	{
		erreur("Vous devez définir un numéro ADELI ou Ligne de Vie pour la personne à importer.", warningError, 0) ;
		return ;
	}

	NSUtilDialog::CmOk() ;
}

void
NSCorrespImportDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}


// -----------------------------------------------------------------------------
//
//  Méthodes de NSPatientImportDialog en mode N_TIERS
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSPatientImportDialog, NSUtilDialog)
  EV_COMMAND(IDOK,                      CmOk),
  EV_COMMAND(IDCANCEL,                  CmCancel),
  EV_EN_CHANGE(IDC_IMPORTPAT_PWDPAT,    PasswordChange),
END_RESPONSE_TABLE ;

NSPatientImportDialog::NSPatientImportDialog(TWindow * pere, NSContexte *pCtx, TModule *module)
  : NSUtilDialog(pere, pCtx, "IDD_IMPORT_PATLDV", module)
{
  pLogin  = new NSUtilEdit(pContexte, this, IDC_IMPORTPAT_LOGINPAT, 80) ;
  pPasswd = new NSUtilEdit(pContexte, this, IDC_IMPORTPAT_PWDPAT, 80) ;
  pNumLdv = new NSUtilEdit(pContexte, this, IDC_IMPORTPAT_NUMLDVPAT, 80) ;
}

NSPatientImportDialog::~NSPatientImportDialog()
{
  delete pLogin ;
  delete pPasswd ;
  delete pNumLdv ; 
}

void
NSPatientImportDialog::SetupWindow()
{
    NSUtilDialog::SetupWindow();
    if (pContexte->getUtilisateur()->haveGlobalSessionPassword())
    {
        pLogin->Insert(pContexte->getUtilisateur()->getGlobalLoginSession().c_str());
        pPasswd->Insert(pContexte->getUtilisateur()->getGlobalPasswordSession().c_str());
        isHached = true;
    }
    else
        isHached = false;
}

/*********************************************
void  NSPatientImportDialog::CmCreateMandat()
{
		GetData(false);
    CreateMandatInterface* create_mandat;
    if ((sLogin != "") && (sPasswd != ""))
   		create_mandat = new CreateMandatInterface(this, pContexte,sLogin, sPasswd );
    else
       create_mandat = new CreateMandatInterface(this, pContexte);
    if (create_mandat != NULL)
    	create_mandat->Execute();
    delete create_mandat;

}
**********************************************/

void NSPatientImportDialog::GetData(bool check)
{
  sLogin  = pLogin->GetText(80) ;
  sPasswd = pPasswd->GetText(200) ;
  if (false == isHached)
    sPasswd = MD5_encrypt(sPasswd) ;

  sNumLdv = pNumLdv->GetText(80) ;

  if (check)
    if ((string("") == sLogin) || (string("") == sPasswd))
    {
      erreur("Vous devez définir un login et un mot de passe pour le patient à importer.", warningError, 0);
      return;
    }
}

void
NSPatientImportDialog::CmOk()
{
		GetData(true);

  /*  if (pMandat->GetCheck() == BF_CHECKED)
        bMandat = true;
    else
        bMandat = false;      */

    NSUtilDialog::CmOk();
}

void
NSPatientImportDialog::PasswordChange()
{
    isHached = false;
}

void
NSPatientImportDialog::CmCancel()
{
    NSUtilDialog::CmCancel();
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListUtilWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListUtilWindow, TListWindow)
    EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

NSListUtilWindow::NSListUtilWindow(ChercheListeUtilDialog* pere, int resId, TModule* module) : TListWindow(pere, resId, module)
{
    pDlg = pere;
}

NSListUtilWindow::~NSListUtilWindow()
{
}

//---------------------------------------------------------------------------
//  Function: NSListUtilWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListUtilWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point);

    HitTest(info);

    if (info.GetFlags() & LVHT_ONITEM)
        pDlg->CmOk();
}

//---------------------------------------------------------------------------
//  Function: NSListUtilWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListUtilWindow::IndexItemSelect()
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
//  Méthodes de NSListeClientGroupDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
#ifdef N_TIERS

DEFINE_RESPONSE_TABLE1(NSListeClientGroupDialog, NSUtilDialog)	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_COMMAND(IDC_LISTGROUP_RECHNOM,  RechercheParNomPrenom),
	EV_COMMAND(IDC_LISTGROUP_RECHIEP,  RechercheParIEP),
  EV_COMMAND(IDC_LISTGROUP_OUVRIR,   CmOk),
  EV_COMMAND(IDC_LISTGROUP_MERGE,    CmOk),
	EV_COMMAND(IDC_LISTGROUP_IMPORTER, Importer),
	EV_COMMAND(IDC_LISTGROUP_GROOP,    Importation),
	EV_COMMAND(IDC_LISTGROUP_BACK,     CmBack),
	EV_LVN_GETDISPINFO(IDC_LISTGROUP_LW, LvnGetDispInfo),
	EV_LVN_COLUMNCLICK(IDC_LISTGROUP_LW, LVNColumnclick),
END_RESPONSE_TABLE;NSListeClientGroupDialog::NSListeClientGroupDialog(TWindow* parent, NSContexte* pCtx, NSPatInfo* pPat)                         :NSUtilDialog(parent, pCtx, "IDD_LISTGROUP")
{
	// Création de tous les "objets de contrôle"
	pNom           = new NSUtilEdit(pContexte, this, IDC_LISTGROUP_NOM, PAT_NOM_LEN) ;
	pPrenom        = new NSUtilEdit(pContexte, this, IDC_LISTGROUP_PRENOM, PAT_PRENOM_LEN) ;
	pIEP           = new NSUtilEdit(pContexte, this, IDC_LISTGROUP_IEP, 30) ;
	pTexte		     = new TStatic(this, IDC_LISTGROUP_TEXT) ;

	pCancel		     = new TStatic(this, IDCANCEL) ;

	pButtonNom     = new TStatic(this, IDC_LISTGROUP_RECHNOM) ;
	pButtonIep     = new TStatic(this, IDC_LISTGROUP_RECHIEP) ;

	pButtonImport  = new TStatic(this, IDC_LISTGROUP_IMPORTER) ;
	pButtonBack    = new TStatic(this, IDC_LISTGROUP_BACK) ;
	pButtonGrpSrch = new TStatic(this, IDC_LISTGROUP_GROOP) ;
	pButtonOpen    = new TStatic(this, IDC_LISTGROUP_OUVRIR) ;
	pButtonMerge   = new TStatic(this, IDC_LISTGROUP_MERGE) ;

	pExactButton   = new TRadioButton(this, IDC_RECH_EXACT) ;
	pStartByButton = new TRadioButton(this, IDC_RECH_APPROCHE) ;

	pListe 	       = new NSListGroupWindow(this, pContexte, IDC_LISTGROUP_LW) ;

	pLastList      = new NSPersonsAttributesArray() ;

    // on recupère le patient en cours
	pPatEnCours = pPat;

	//il faut reinitialiser la liste des patients
	bMustInit = true;

	// Création du tableau de Patients
	pPatientsArray = new NSPatientArray;

	iSortedColumn    = 1 ;
	bNaturallySorted = true ; // in order to have it naturally sorted

	// nom et prénom initiaux
	_sNomPat    = string("") ;
	_sPrenomPat = string("") ;

	// Existe-t-il des données de capture Episodus ?
	if (pContexte->getEpisodus())
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray);

    string sNom     = string("") ;
    string sPrenom  = string("") ;
    string sCode    = string("") ;

    pCapt->getIdentity(sNom, sPrenom) ;
    pCapt->trouveChemin(string("LIDEN1"), &sCode) ;

    if (strlen(sNom.c_str()) > PAT_NOM_LEN)
      sNom = string(sNom, 0, PAT_NOM_LEN) ;
    _sNomPat = sNom ;

    if (strlen(sPrenom.c_str()) > PAT_PRENOM_LEN)
      sPrenom = string(sPrenom, 0, PAT_PRENOM_LEN) ;
    _sPrenomPat = sPrenom ;
  }

	sNssSelect = string("") ;	etat = todoNothing ;  mode = todoNothing ;	bTrouveLocal  = false ;	bTrouveGlobal = false ;	bListeLocal   = true ;	sIdLocal  = string("") ;	sIdGlobal = string("") ;
	// Gestion du blocage patient	verifBloque  = false ;
	donneMessage = true ;

	// fichiers d'aide
	pContexte->getSuperviseur()->setAideIndex("h_index.htm") ;
	pContexte->getSuperviseur()->setAideCorps("h_rchpat.htm") ;
}

NSListeClientGroupDialog::~NSListeClientGroupDialog()
{
	delete pNom ;
	delete pPrenom ;
  delete pIEP ;
	delete pTexte	;
	delete pCancel ;
	delete pButtonNom ;
	delete pButtonIep ;
	delete pButtonImport ;
	delete pButtonBack ;
	delete pButtonGrpSrch ;
	delete pButtonOpen ;
  delete pButtonMerge ;
  delete pExactButton ;
	delete pStartByButton ;

  delete pLastList ;
	delete pListe ;
	delete pPatientsArray ;
}

void
NSListeClientGroupDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	setResearchOn() ;

	pNom->SetText(_sNomPat) ;
	pPrenom->SetText(_sPrenomPat) ;

	InitListe() ;

	pStartByButton->SetCheck(BF_CHECKED) ;
}

void
NSListeClientGroupDialog::RechercheParIEP()
{
  string sCode = pIEP->GetText(PAT_CODE_LEN) ;

	if (string("") == sCode)
		return ;

	RechercheIEP(sCode) ;
}

void prepareString(string &sStr)
{
  size_t iStrLen = strlen(sStr.c_str()) ;

	for (size_t i = 0 ; i < iStrLen ; i++)
		if ('*' == sStr[i])
			sStr[i] = '%' ;
}

void
NSListeClientGroupDialog::PrepareNomPrenom(string& sNomToCall, string& sPrenomToCall)
{
	// on récupère le nom et le prénom pour les passer comme traits au service
	_sNomPat    = pNom->GetText(PAT_NOM_LEN) ;
	_sPrenomPat = pPrenom->GetText(PAT_PRENOM_LEN) ;

  if (string("") != _sPrenomPat)
    prepareString(_sPrenomPat) ;

  sPrenomToCall = _sPrenomPat ;

  if (pStartByButton->GetCheck() == BF_CHECKED)
  {
    if ((sPrenomToCall == "") || (sPrenomToCall[strlen(sPrenomToCall.c_str())-1] != '%'))
      sPrenomToCall += string("%") ;
  }

  if (string("") != _sNomPat)
    prepareString(_sNomPat) ;

  sNomToCall = _sNomPat ;

  // Recherche exacte ou recherche approchée
  //
  if (pStartByButton->GetCheck() == BF_CHECKED)
  {
    if ((sNomToCall == "") || (sNomToCall[strlen(sNomToCall.c_str())-1] != '%'))
      sNomToCall += string("%") ;
  }
}

void
NSListeClientGroupDialog::RechercheParNomPrenom()
{
try
{
	// on récupère le nom et le prénom pour les passer comme traits au service
	string sNomToCall = "" ;
	string sPrenomToCall = "" ;

	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

	PrepareNomPrenom(sNomToCall, sPrenomToCall) ;

  if (sNomToCall == string("%"))
  {
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    MessageBox("Attention vous devez taper comme critère au moins une lettre du nom.", sCaption.c_str(), MB_OK) ;
    return ;
  }

	//on a une liste des patients
	NSPersonsAttributesArray PatiensList ;
	NSBasicAttributeArray    AttrArray ;

	AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
	if (sPrenomToCall != string("%"))
		AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, sPrenomToCall)) ;
	AttrArray.push_back(new NSBasicAttribute(LAST_NAME, sNomToCall)) ;

	if ((sNomToCall == string("")) && (sPrenomToCall == string("")))
		return ;

	AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME));
	AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME));
	AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX));
	AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE));
	AttrArray.push_back(new NSBasicAttribute(TRAIT, IPP)) ;
	// pAttrArray->push_back(new NSBasicAttribute(OPERATOR, user)) ;
	// pAttrArray->push_back(new NSBasicAttribute(CONSOLE, console)) ;
	// pAttrArray->push_back(new NSBasicAttribute(INSTANCE, string(szInstance))) ;

  if (bListeLocal)
  {
  	bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &PatiensList, &AttrArray) ;
    //if ((!res) || (pPatiensList->empty()))
    if (!res)
    {
    	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
      std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if( tempMessage != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
      if( tempError != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
      return ;
    }

    etat = todoSearch ;

    // Initialisation du tableau des patients et des bookmarks
    InitPatArray(&PatiensList) ;

    // on conserve l'état de la liste pour le retour éventuel à la recherche
    *pLastList = PatiensList ;

    // Initialisation de la liste
    AfficheListe(true) ;

    pTexte->SetText("Etat de la recherche nom-prénom sur serveur local.") ;
  }
  else
  {
  	// on est ici dans le cas où on réitère la recherche sur le serveur SIH
    bool res = pContexte->getSuperviseur()->getPilot()->personList((NautilusPilot::SERV_GROUP_PATIENT_LIST).c_str(), &PatiensList, &AttrArray);

    if (!res)
    {
    	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage();
      std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage();
      if( tempMessage != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
      if( tempError != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
      return ;
    }

    // Dans ce contexte, on laisse l'etat inchangé
    // Initialisation du tableau des patients et des bookmarks
    InitPatArray(&PatiensList) ;

    // Initialisation de la liste
    AfficheListe(false) ;
  }

  pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}
catch (...)
{
  erreur("Exception NSListeClientGroupDialog::RechercheParNomPrenom", standardError, 0) ;
}
}

//
// Liste des patients sur le serveur SIH
// Patients list on the HIS server
//
void
NSListeClientGroupDialog::Importation()
{
try
{
	string sNomToCall, sPrenomToCall ;
	// on récupère le nom et le prénom pour les passer comme traits au service
	PrepareNomPrenom(sNomToCall, sPrenomToCall) ;

  if (sNomToCall == string("%"))
  {
  	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    MessageBox("Attention vous devez taper comme critère au moins une lettre du nom.", sCaption.c_str(), MB_OK) ;
    return ;
  }

	bool listOk = false;
	// le serviceName est défini dans le constructeur
	//on a une liste des patients
	NSPersonsAttributesArray PatiensList ;
	NSBasicAttributeArray    AttrArray ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  // sur le serveur de groupe
  serviceName = (NautilusPilot::SERV_GROUP_PATIENT_LIST).c_str() ;

  AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE));
  if (sPrenomToCall != string("%"))
  	AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, sPrenomToCall));
  AttrArray.push_back(new NSBasicAttribute(LAST_NAME, sNomToCall));

  AttrArray.push_back(new NSBasicAttribute(TRAIT, IPP));
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME));
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME));
  AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX));
  AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE));

  listOk = pContexte->getSuperviseur()->getPilot()->personList(serviceName, &PatiensList, &AttrArray) ;

  // we switch to "group mode" even with an empty list
  if ((!listOk) /*|| (pPatiensList->empty())*/)
  {
  	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
    if( tempMessage != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
    if( tempError != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
    return ;
  }

  etat = todoImport ;

  pButtonImport->Show(SW_SHOW) ;
  pButtonBack->Show(SW_SHOW) ;
  pButtonMerge->Show(SW_HIDE) ;
  pButtonGrpSrch->Show(SW_HIDE) ;
  pButtonOpen->Show(SW_HIDE) ;

  Invalidate() ;

  // ::ShowWindow(GetDlgItem(IDC_LISTGROUP_IMPORTER),   SW_HIDE);
  // ::EnableWindow(GetDlgItem(IDC_LISTGROUP_IMPORTER), FALSE );
  // pButtonImport->ShowWindow(SW_HIDE);

  // Initialisation du tableau des patients et des bookmarks
  InitPatArray(&PatiensList) ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;

  // Initialisation de la liste
  AfficheListe(false) ;

  pTexte->SetText("Etat de la recherche nom-prénom sur serveur SIH. Sélectionnez ci-dessous le patient à importer ou Retour pour revenir à la liste locale.") ;
}
catch (...)
{
  erreur("Exception NSListeClientGroupDialog::Importation", standardError, 0) ;
}
}

void
NSListeClientGroupDialog::Importer()
{
	if (!bTrouveGlobal)
		CmOk();
	else
	{
		if (!importPatientGroup(sIPPTrouve))
			return ;

		SetCursor(0, IDC_ARROW) ;
		CloseWindow(IDOK) ;
	}
}

void
NSListeClientGroupDialog::InitListe()
{
	TListWindColumn colIpp("", 15, TListWindColumn::Left, 0);
    pListe->InsertColumn(0, colIpp) ;

	TListWindColumn colPatient("Patient", 200, TListWindColumn::Left, 1) ;
	pListe->InsertColumn(1, colPatient) ;

	TListWindColumn colDateN("Date Naiss", 80, TListWindColumn::Left, 2) ;	pListe->InsertColumn(2, colDateN) ;

	TListWindColumn colSexe("Sexe", 40, TListWindColumn::Left, 3) ;
	pListe->InsertColumn(3, colSexe) ;

	TListWindColumn colStatut("Statut", 40, TListWindColumn::Left, 4) ;
	pListe->InsertColumn(4, colStatut) ;

	TListWindColumn colBloque("Bloqué", 50, TListWindColumn::Left, 5) ;	pListe->InsertColumn(5, colBloque) ;
}

void
NSListeClientGroupDialog::AfficheListe(bool bLocal)
{
	// char flagIpp[5];

	bListeLocal = bLocal ;

	// On vide la liste
	//
	pListe->DeleteAllItems();

	if (bListeLocal)
	{
		SetCaption("PATIENTS DU SERVICE");
        pButtonNom->SetCaption("Rechercher par Nom/Prénom");
		// setResearchOn();
	}
	else
	{
		SetCaption("PATIENTS DU SIH");
        pButtonNom->SetCaption("Rechercher sur SIH");
		// setResearchOff();
	}

	PatientReverseIter iterReverse = pPatientsArray->rbegin() ;

	while(iterReverse != pPatientsArray->rend())
	{
		TListWindItem Item("", 0) ;

        if ((*iterReverse)->haveIpp())
        	Item.SetStateImage(2) ;

		pListe->InsertItem(Item) ;
		iterReverse++ ;
	}
}

void
NSListeClientGroupDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	const int   BufLen = 255 ;
	static char buffer[BufLen] ;
	int         index ;
	static char libelBloqu[3] ;
	static char	libelStatut[5] ;
	static char sexe[2] ;
	string      clef ;

	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	index = dispInfoItem.GetIndex() ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
  	case 1 :
    	sprintf(buffer, "%s %s", ((*pPatientsArray)[index])->getszNom(), ((*pPatientsArray)[index])->getszPrenom()) ;
      dispInfoItem.SetText(buffer) ;
      break ;

    case 2 : // date de naissance
    	if (((*pPatientsArray)[index])->donneDateNaiss() != "")
      {
      	sprintf(buffer, "%s", (((*pPatientsArray)[index])->donneDateNaiss()).c_str()) ;
        dispInfoItem.SetText(buffer) ;
      }
      break ;

    case 3 : // sexe
    	if (((*pPatientsArray)[index])->estMasculin())
      	strcpy(sexe, "M") ;
      else
      	strcpy(sexe, "F") ;
      dispInfoItem.SetText(sexe) ;

      break ;

    case 4 : // Statut
    	strcpy(libelStatut, "  ") ;
      dispInfoItem.SetText(libelStatut) ;
      break ;

    case 5 : // blocage
    	strcpy(libelBloqu, "  ") ;
      dispInfoItem.SetText(libelBloqu) ;
      break ;
	}
}

void
NSListeClientGroupDialog::InitPatArray(NSPersonsAttributesArray *pList)
{
	pPatientsArray->vider();

	if (pList && (false == pList->empty()))
		for (NSPersonsAttributeIter iterPatient = pList->begin(); pList->end() != iterPatient; iterPatient++)
		{
			NSPatInfo *pPatientEnCours = new NSPatInfo(*iterPatient, pContexte);
			pPatientsArray->push_back(pPatientEnCours) ;
		}

  iSortedColumn    = 1 ;
  bNaturallySorted = true ;
	sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByNameInf) ;
}

void
NSListeClientGroupDialog::RechercheIEP(string sIEP)
{
try
{
	bool listOk = false;
  // le serviceName est défini dans le constructeur
  //on a une liste des patients
  NSPersonsAttributesArray PatiensList ;
  NSBasicAttributeArray    AttrArray ;

  // sur le serveur de groupe
  serviceName= (NautilusPilot::SERV_GROUP_PATIENT_LIST).c_str() ;

  AttrArray.push_back(new NSBasicAttribute(ROLE,  PATIENT_ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(IEP,   sIEP)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, IPP)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;

  listOk = pContexte->getSuperviseur()->getPilot()->personList(serviceName, &PatiensList, &AttrArray) ;

  if ((!listOk) || (PatiensList.empty()))
  {
    std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
    if( tempMessage != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
    if( tempError != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
    return ;
	}

  // on prend le premier de la liste (en principe l'IEP est unique)
  NSPatInfo *pPatientEnCours = new NSPatInfo(*(PatiensList.begin()), pContexte) ;

  if (!pPatientEnCours->haveIpp())
  {
  	// cas d'erreur
    erreur("Le patient recherché ne détient pas d'IPP.", standardError, 0) ;
    return ;
	}

  bTrouveGlobal = true ;
  sIPPTrouve    = pPatientEnCours->getIpp() ;
  sIdGlobal     = pPatientEnCours->getNss() ;
  sPatientGlobal = pPatientEnCours->getNom() + string(" ") + pPatientEnCours->getPrenom() ;

  // NSDataGraph* pDataGraph = pPatientEnCours->pGraphPerson->pDataGraph ;
  AttrArray.vider() ;
  PatiensList.vider() ;
  bool bTrouve = false ;

  // on recherche son IPP en local
  //////////////////////////////////////////////////////////////////////////////////////////

  string user = pContexte->getUtilisateurID();
  string console = pContexte->getSuperviseur()->getConsole() ;

  AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(IPP, pPatientEnCours->getIpp())) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;
  // pAttrArray->push_back(new NSBasicAttribute(OPERATOR, user)) ;
  // pAttrArray->push_back(new NSBasicAttribute(CONSOLE, console)) ;
  // pAttrArray->push_back(new NSBasicAttribute(INSTANCE, string(szInstance))) ;

  bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &PatiensList, &AttrArray) ;
  if (!res)
  {
  	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage();
    if( tempMessage != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
    if( tempError != "")
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
    return ;
  }

  if (false == PatiensList.empty())
  	bTrouve = true ;

  pNom->SetText(pPatientEnCours->getszNom()) ;
  pPrenom->SetText(pPatientEnCours->getszPrenom()) ;

  if (bTrouve)
  {
  	bTrouveLocal = true ;

    etat = todoSynchro;

    pButtonImport->Show(SW_HIDE) ;
    pButtonBack->Show(SW_SHOW) ;
    pButtonMerge->Show(SW_HIDE) ;
    pButtonGrpSrch->Show(SW_HIDE) ;
    pButtonOpen->Show(SW_SHOW) ;

    Invalidate() ;

    InitPatArray(&PatiensList) ;
    AfficheListe(true) ;

    pTexte->SetText("Patient trouvé en local. Sélectionnez-le et appuyez sur Ouvrir pour l'ouvrir.");
  }
  else
  {
  	AttrArray.vider() ;
    PatiensList.vider() ;

    AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
    AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, pPatientEnCours->getPrenom())) ;
    AttrArray.push_back(new NSBasicAttribute(LAST_NAME, pPatientEnCours->getNom())) ;
    // pAttrArray->push_back(new NSBasicAttribute(SEX, pPatientEnCours->getSexe())) ;
    // pAttrArray->push_back(new NSBasicAttribute(BIRTHDATE, pPatientEnCours->getNaissance())) ;

    AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME));
    AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME));
    AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX));
    AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE));
    AttrArray.push_back(new NSBasicAttribute(TRAIT, IPP)) ;

    bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &PatiensList, &AttrArray) ;
    if (!res)
    {
      std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage();
      std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage();
      if( tempMessage != "")
          ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
      if( tempError != "")
          ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
      return;
    }

    etat = todoMerge;

    pButtonImport->Show(SW_SHOW) ;
    pButtonBack->Show(SW_SHOW) ;
    pButtonMerge->Show(SW_SHOW) ;
    pButtonGrpSrch->Show(SW_HIDE) ;
    pButtonOpen->Show(SW_HIDE) ;

    Invalidate() ;

    InitPatArray(&PatiensList) ;
    AfficheListe(true) ;

    char msg[255] ;
    char dateNaiss[255] ;

    if (pPatientEnCours->donneDateNaiss() != "")
      sprintf(dateNaiss, " né(e) le %s", (pPatientEnCours->donneDateNaiss()).c_str()) ;
    else
      strcpy(dateNaiss, "") ;

    sprintf(msg, "Patient recherché : %s %s%s. IPP non trouvé sur la base locale. Sélectionnez ci-dessous, sur la base locale, un patient de même nom à fusionner ou importez le patient en cliquant sur Importer.",
        pPatientEnCours->getszNom(), pPatientEnCours->getszPrenom(), dateNaiss) ;
    pTexte->SetText(msg) ;
	}
  SetCursor(0, IDC_ARROW) ;
}
catch (...)
{
  erreur("Exception NSListeClientGroupDialog::RechercheIEP", standardError, 0) ;
}
}

bool
NSListeClientGroupDialog::importPatientGroup(string sIPP)
{
try
{
  NSPersonGraphManager* pGraphPerson = pPatEnCours->getGraphPerson() ;
  if (NULL == pGraphPerson)
    return false ;

	NSDataGraph* pGraph = pGraphPerson->getDataGraph() ;
	NSBasicAttributeArray AttrArray ;

	/*pAttrArray->push_back(new NSBasicAttribute(LOGIN, pContexte->getUtilisateur()->getGlobalLoginSession()));
	pAttrArray->push_back(new NSBasicAttribute(PASSWORD, pContexte->getUtilisateur()->getGlobalPasswordSession()));*/
	AttrArray.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID()));
	AttrArray.push_back(new NSBasicAttribute(IPP, sIPP));

	// lancement du service d'importation
	bool res = pContexte->getSuperviseur()->getPilot()->importPerson(NautilusPilot::SERV_GROUP_PATIENT_IMPORT.c_str(),
                                &AttrArray, pGraphPerson->getDataGraph());

	if (!res)
	{
		std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage();
		std::string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage();
		if( tempMessage != "")
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK);
		if( tempError != "")
			::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK);
		//CloseWindow(IDCANCEL);
		return false ;
	}

   //	pPatEnCours->pGraphPerson->setInfoPids(pAttrArray);
	pGraphPerson->getDataGraph()->setLastTree() ;

	pGraphPerson->parseMainAttributes(pContexte) ;

	string user = pContexte->getUtilisateurID() ;
	serviceName = (NautilusPilot::SERV_CREATE_IMPORTED_GROUP_PATIENT).c_str() ;
	NSPersonsAttributesArray PersonsList ;
	NSBasicAttributeArray AttrList ;

	AttrArray.vider() ;
	AttrArray = *(pPatEnCours->getGraphPerson()->getAttrArray()) ;

	AttrList.push_back(new NSBasicAttribute(LAST_NAME,  AttrArray.getAttributeValue(LAST_NAME))) ;
	AttrList.push_back(new NSBasicAttribute(FIRST_NAME, AttrArray.getAttributeValue(FIRST_NAME))) ;
	AttrList.push_back(new NSBasicAttribute(SEX,        AttrArray.getAttributeValue(SEX))) ;
	AttrList.push_back(new NSBasicAttribute(BIRTHDATE,  AttrArray.getAttributeValue(BIRTHDATE))) ;
	AttrList.push_back(new NSBasicAttribute(OPERATOR,   user)) ;
	AttrList.push_back(new NSBasicAttribute(CONSOLE,    pContexte->getSuperviseur()->getConsoleString())) ;
	AttrList.push_back(new NSBasicAttribute(INSTANCE,   pContexte->getSuperviseur()->getInstanceString())) ;
	AttrList.push_back(new NSBasicAttribute(PERSON,     pGraphPerson->getPersonID())) ;
	AttrList.push_back(new NSBasicAttribute(IPP,        sIPP));
	string sRootDoc = string(pPatEnCours->getGraphPerson()->getRootTree(), PAT_NSS_LEN, DOC_CODE_DOCUM_LEN);
	AttrList.push_back(new NSBasicAttribute(ROOTDOC,    sRootDoc)) ;
	bool bRes = pContexte->getSuperviseur()->getPilot()->createImportedPerson(serviceName, pGraph, &PersonsList, &AttrList) ;

	if (!bRes)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "importedPatientCreationError") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	// import all objects for this patient
	NSBasicAttributeArray AttrObjectsArray ;
	AttrObjectsArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
	AttrObjectsArray.push_back(new NSBasicAttribute(PERSON, pGraphPerson->getPersonID())) ;
	bRes = pContexte->getSuperviseur()->getPilot()->updateObjectList(NautilusPilot::SERV_UPDATE_ALL_LDV_OBJECTS.c_str(), pGraph, &AttrObjectsArray) ;
	if (!bRes)
	{
		erreur(pContexte->getSuperviseur()->getPilot()->getWarningMessage().c_str(), standardError, 0) ;
		string sErrorText = pContexte->getSuperviseur()->getText("patientManagement", "errorImportObjects") ;
		erreur(sErrorText.c_str(), standardError, 0) ;
		return false ;
	}

	AttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
	AttrArray.push_back(new NSBasicAttribute(PERSON, pGraphPerson->getPersonID())) ;

	if ((!pGraphPerson->getDataGraph()) ||
    	(!pGraphPerson->graphPrepare()))
		return false ;

	pGraphPerson->setNeedUnlock(true) ;
	pGraphPerson->setReadOnly(false) ;

	pGraphPerson->setInfoPids(&AttrArray) ;
	pGraphPerson->getDataGraph()->setLastTree() ;
    //debug line
	string sRootTree = pGraphPerson->getRootTree() ;

  mode = todoImport ;

	return true ;
}
catch (...)
{
  erreur("Exception NSListeClientGroupDialog::importPatientGroup", standardError, 0) ;
	return false ;
}
}

void
NSListeClientGroupDialog::GardeNss(int index)
{
	sNssSelect = ((*pPatientsArray)[index])->getNss();
}

void
NSListeClientGroupDialog::RetrouvePatSelect()
{
	for (int i = 0; i < nbNom; i++)
    {
        if (sNssSelect == ((*pPatientsArray)[i])->getNss())
        {      	    pListe->SetSel(i, true);
            break;
        }
    }
}

void
NSListeClientGroupDialog::setResearchOn()
{
	/******************************************** on ne gere plus que les boutons ici
	uint32 uiStyle = pNom->TWindow::GetStyle() ;
	uiStyle &= ~WS_DISABLED ;
	//pNom->TWindow::SetStyle(uiStyle ^ WS_DISABLED) ;
	pNom->TWindow::SetStyle(uiStyle);

	uiStyle = pPrenom->TWindow::GetStyle();
	uiStyle &= ~WS_DISABLED ;
	//pPrenom->TWindow::SetStyle(uiStyle ^ WS_DISABLED) ;
	pPrenom->TWindow::SetStyle(uiStyle);

	uiStyle = pIEP->TWindow::GetStyle();
	//pIEP->TWindow::SetStyle(uiStyle ^ WS_DISABLED) ;
	uiStyle &= ~WS_DISABLED ;
	pIEP->TWindow::SetStyle(uiStyle) ;

	uiStyle = pButtonNom->TWindow::GetStyle();
	// pButtonNom->TWindow::SetStyle(uiStyle ^ WS_DISABLED) ;
	uiStyle &= ~WS_DISABLED ;
	pButtonNom->TWindow::SetStyle(uiStyle);

	uiStyle = pButtonIep->TWindow::GetStyle();
	uiStyle &= ~WS_DISABLED ;
	pButtonIep->TWindow::SetStyle(uiStyle) ;
    **********************************************************************************/

    pButtonImport->Show(SW_HIDE) ;
    pButtonBack->Show(SW_HIDE) ;
    pButtonMerge->Show(SW_HIDE) ;
    pButtonGrpSrch->Show(SW_SHOW) ;
    pButtonOpen->Show(SW_SHOW) ;

	Invalidate() ;
}

void
NSListeClientGroupDialog::setResearchOff()
{
	uint32 uiStyle = pNom->TWindow::GetStyle() ;
	pNom->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

	uiStyle = pPrenom->TWindow::GetStyle();
	pPrenom->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

	uiStyle = pIEP->TWindow::GetStyle();
	pIEP->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

	uiStyle = pButtonNom->TWindow::GetStyle();
	pButtonNom->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

	uiStyle = pButtonIep->TWindow::GetStyle();
	pButtonIep->TWindow::SetStyle(uiStyle | WS_DISABLED) ;

	// pButtonImport->Show(SW_SHOW) ;
	// pButtonBack->Show(SW_SHOW) ;
	// pButtonGrpSrch->Show(SW_HIDE) ;
	// pButtonOpen->Show(SW_HIDE) ;

	Invalidate() ;
}

bool
NSListeClientGroupDialog::CanClose()
{
	return NSUtilDialog::CanClose();
}


voidNSListeClientGroupDialog::CmOk(){	int index = pListe->IndexItemSelect();	if (index == -1)
	{
		// erreur("Vous devez sélectionner un patient dans la liste.",0,0,GetHandle());
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "PatientNotSelected") ;
		erreur(sErrorText.c_str(), warningError, standardError, 0) ;
		return ;
	}

	*pPatEnCours = *((*pPatientsArray)[index]);

	char szPerson[PAT_NSS_LEN + 1] ;
	strcpy(szPerson, ((*pPatientsArray)[index])->getszNss()) ;

  /*
  if ((szPerson[0] != LocalPatient) && (pContexte->getUtilisateur()->haveGlobalSessionPassword() == false))
  {
    LogPassInterface* getGlobPassword = new LogPassInterface(this,pContexte);
    getGlobPassword->Execute();
    delete  getGlobPassword;
  }
  */

	if (bListeLocal)
	{
		bTrouveLocal = true ;
		sIdLocal = ((*pPatientsArray)[index])->getNss() ;
    sPatientLocal = ((*pPatientsArray)[index])->getNom() + string(" ") + ((*pPatientsArray)[index])->getPrenom() ;
	}
	else
	{
		bTrouveGlobal = true ;
		sIdGlobal = ((*pPatientsArray)[index])->getNss() ;
		sIPPTrouve = ((*pPatientsArray)[index])->getIpp() ;
    sPatientGlobal = ((*pPatientsArray)[index])->getNom() + string(" ") + ((*pPatientsArray)[index])->getPrenom() ;
	}

	SetCursor(0, IDC_WAIT) ;
	// pContexte->setPatient(new NSPatientChoisi(pContexte));
	string user = pContexte->getUtilisateurID() ;

	NSBasicAttributeArray    AttrArray ;
	NSPersonsAttributesArray List ;

	if (etat == todoSearch)
	{
		// ouverture en mode Client Group :
		// on vérifie si le patient a un ipp
		if (((*pPatientsArray)[index])->haveIpp())
		{
      AttrArray.push_back(new NSBasicAttribute(IPP,      ((*pPatientsArray)[index])->getIpp())) ;
      AttrArray.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
      AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
      AttrArray.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;

      // lancement du service de synchronisation
      bool res = pContexte->getSuperviseur()->getPilot()->searchPatient(NautilusPilot::SERV_OPEN_PATIENT_DATA_FROM_TRAITS.c_str(), pPatEnCours->getGraphPerson()->getDataGraph(), &List, &AttrArray) ;
			if (!res)
			{
				string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
				string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
				if( tempMessage != "")
					::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
				if( tempError != "")
					::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
				//CloseWindow(IDCANCEL) ;
        SetCursor(0, IDC_ARROW) ;
				return ;
			}

      // Mode NeedUnlock et Read-only
      pPatEnCours->getGraphPerson()->setNeedUnlock(false) ;
      pPatEnCours->getGraphPerson()->setReadOnly(true) ;

      if (!(List.empty()))
      {
        string sIsLocked = List.getAttributeValue("locked") ;
        if (sIsLocked == "ok")
          pPatEnCours->getGraphPerson()->setNeedUnlock(true) ;
        string sOperationType	= List.getAttributeValue("operationType") ;
        if (sOperationType == "readWrite")
          pPatEnCours->getGraphPerson()->setReadOnly(false) ;
      }
		}
		else
		{
      // on affiche les nom et prenom avant la recherche en groupe
      pNom->SetText(pPatEnCours->getszNom()) ;
      pPrenom->SetText(pPatEnCours->getszPrenom()) ;

      // Lancement des services réalisant la procédure de merge
      // sur le serveur de groupe
      string sNomToCall, sPrenomToCall ;
      // on récupère le nom et le prénom pour les passer comme traits au service
      PrepareNomPrenom(sNomToCall, sPrenomToCall) ;

      if (string("%") == sNomToCall)
      {
        string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
        MessageBox("Attention vous devez taper comme critère au moins une lettre du nom.", sCaption.c_str(), MB_OK) ;
        return ;
      }

			bool listOk = false ;
			// le serviceName est défini dans le constructeur
			//on a une liste des patients
			serviceName = (NautilusPilot::SERV_GROUP_PATIENT_LIST).c_str() ;

			AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
      if (sPrenomToCall != string("%"))
				AttrArray.push_back(new NSBasicAttribute(FIRST_NAME, sPrenomToCall)) ;
			AttrArray.push_back(new NSBasicAttribute(LAST_NAME, sNomToCall)) ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, IPP)) ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
			AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;

			listOk = pContexte->getSuperviseur()->getPilot()->personList(serviceName, &List, &AttrArray) ;
			if (!listOk)
			{
        SetCursor(0, IDC_ARROW) ;
				string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
				string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
				if (tempMessage != "")
					::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
				if (tempError != "")
					::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
				return ;
			}

			etat = todoMerge ;

      pButtonImport->Show(SW_HIDE) ;
      pButtonBack->Show(SW_SHOW) ;
      pButtonMerge->Show(SW_SHOW) ;
      pButtonGrpSrch->Show(SW_HIDE) ;
      pButtonOpen->Show(SW_HIDE) ;

      Invalidate() ;

			//::ShowWindow(GetDlgItem(IDC_LISTGROUP_IMPORTER),   SW_HIDE) ;
			//::EnableWindow(GetDlgItem(IDC_LISTGROUP_IMPORTER), FALSE ) ;

			// Initialisation du tableau des patients et des bookmarks
			InitPatArray(&List) ;

			// Initialisation de la liste
			AfficheListe(false) ;

      char msg[255] ;
      char dateNaiss[255] ;

      if (pPatEnCours->donneDateNaiss() != "")
          sprintf(dateNaiss, " né(e) le %s", (pPatEnCours->donneDateNaiss()).c_str()) ;
      else
          strcpy(dateNaiss, "") ;

      sprintf(msg, "IPP non trouvé en local. Vous pouvez maintenant fusionner %s %s%s avec une personne ci-dessous (liste du serveur SIH) ou Retour pour revenir à la liste locale.", pPatEnCours->getszNom(), pPatEnCours->getszPrenom(), dateNaiss) ;

			pTexte->SetText(msg) ;

      SetCursor(0, IDC_ARROW) ;
			return ;
		}
	}
	else if (etat == todoSynchro)
	{
		AttrArray.push_back(new NSBasicAttribute(IPP,      sIPPTrouve)) ;
    AttrArray.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
    AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
    AttrArray.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;

		// lancement du service de synchronisation
		bool res = pContexte->getSuperviseur()->getPilot()->searchPatient(NautilusPilot::SERV_OPEN_PATIENT_DATA_FROM_TRAITS.c_str(), pPatEnCours->getGraphPerson()->getDataGraph(), &List, &AttrArray) ;
		if (!res)
		{
      SetCursor(0, IDC_ARROW) ;
			string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
			string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
			if( tempMessage != "")
				::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
			if( tempError != "")
				::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
			//CloseWindow(IDCANCEL) ;
			return ;
		}

    mode = todoSynchro ;

    // Mode NeedUnlock et Read-only
    pPatEnCours->getGraphPerson()->setNeedUnlock(false) ;
    pPatEnCours->getGraphPerson()->setReadOnly(true) ;

    if (!(List.empty()))
    {
      string sIsLocked = List.getAttributeValue("locked") ;
      if (sIsLocked == "ok")
        pPatEnCours->getGraphPerson()->setNeedUnlock(true) ;
      string sOperationType	= List.getAttributeValue("operationType") ;
      if (sOperationType == "readWrite")
        pPatEnCours->getGraphPerson()->setReadOnly(false) ;
    }
	}
	else if (etat == todoMerge)
	{
		char msg[255] ;

    sprintf(msg, "ATTENTION : Vous allez fusionner %s (base SIH) avec %s (base locale). Voulez-vous continuer ?", sPatientGlobal.c_str(), sPatientLocal.c_str()) ;
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;
    int retVal = MessageBox(msg, sCaption.c_str(), MB_YESNO) ;
    if (retVal == IDNO)
			return ;

    // Dans ce cas on doit vérifier que l'IPP trouvé sur le serveur de groupe
    // n'existe pas déjà en local, auquel cas l'importation plante...
    AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
    AttrArray.push_back(new NSBasicAttribute(IPP, sIPPTrouve)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;

    bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &List, &AttrArray) ;
    if (!res)
    {
    	string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
      string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if (tempMessage != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
      if (tempError != "")
      	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
      return ;
    }

    if (!List.empty())
    {
    	// cas d'erreur : le patient que l'on veut importer a déjà le même IPP en local
      erreur("Fusion impossible : un patient détenant le même IPP que le patient sélectionné sur le SIH existe déjà en local.", standardError, 0) ;
      return ;
    }

    AttrArray.vider() ;

		if (importPatientGroup(sIPPTrouve))
		{
      // NSDataGraph* pGraph = pPatEnCours->pGraphPerson->pDataGraph ;

			// pContexte->setPatient(new NSPatientChoisi(pContexte));
			string user = pContexte->getUtilisateurID() ;

			NSBasicAttributeArray LocalAttrArray ;

			LocalAttrArray.push_back(new NSBasicAttribute(COLLECTIVE, sIdGlobal)) ;
			LocalAttrArray.push_back(new NSBasicAttribute(LOCAL,      sIdLocal)) ;
			LocalAttrArray.push_back(new NSBasicAttribute(OPERATOR,   user)) ;
			LocalAttrArray.push_back(new NSBasicAttribute(CONSOLE,    pContexte->getSuperviseur()->getConsoleString())) ;
			LocalAttrArray.push_back(new NSBasicAttribute(INSTANCE,   pContexte->getSuperviseur()->getInstanceString())) ;

			bool res = pContexte->getSuperviseur()->getPilot()->mergePatient(NautilusPilot::SERV_MERGE_PATIENT.c_str(), pPatEnCours->getGraphPerson()->getDataGraph(), &LocalAttrArray) ;
			if (!res)
			{
      	SetCursor(0, IDC_ARROW) ;
				string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
				string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
				if (tempMessage != "")
        	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
				if (tempError != "")
        	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
				return ;
			}

      LocalAttrArray.vider() ;
      LocalAttrArray.push_back(new NSBasicAttribute(PERSON, sIdGlobal)) ;
      LocalAttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;

      res = pContexte->getSuperviseur()->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_GRAPH_PERSON.c_str(), pPatEnCours->getGraphPerson()->getDataGraph(), &List, &LocalAttrArray) ;
      if (!res)
			{
      	SetCursor(0, IDC_ARROW) ;
				string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
				string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
				if (tempMessage != "")
        	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
				if (tempError != "")
        	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
				return ;
			}

      mode = todoMerge ;

			// string sIdCollectif = pPatEnCours->getNss() ;
		}
    else
			return ;
	}
	else if (etat == todoImport)
	{
    // Dans ce cas on doit vérifier que l'IPP trouvé sur le serveur de groupe
    // n'existe pas déjà en local, auquel cas l'importation plante...
    AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
    AttrArray.push_back(new NSBasicAttribute(IPP, sIPPTrouve)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
    AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;
    // pAttrArray->push_back(new NSBasicAttribute(OPERATOR, user)) ;
    // pAttrArray->push_back(new NSBasicAttribute(CONSOLE, console)) ;
    // pAttrArray->push_back(new NSBasicAttribute(INSTANCE, string(szInstance))) ;

    bool res = pContexte->getSuperviseur()->getPilot()->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &List, &AttrArray) ;
    if (!res)
    {
      string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
      string tempError = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
      if (tempMessage != "")
        ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
      if (tempError != "")
        ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
      return ;
    }

    if (!List.empty())
    {
      // cas d'erreur : le patient que l'on veut importer a déjà le même IPP en local
      erreur("Importation impossible : un patient détenant le même IPP que le patient sélectionné sur le SIH existe déjà en local.", standardError, 0) ;
      return ;
    }

		if (!importPatientGroup(sIPPTrouve))
    {
      SetCursor(0, IDC_ARROW) ;
      return ;
		}
	}

	if ((NULL == pPatEnCours->getGraphPerson()->getDataGraph()) || (!pPatEnCours->getGraphPerson()->graphPrepare()))
	{
    SetCursor(0, IDC_ARROW) ;
		return ;
	}

  if ((etat != todoImport) && (etat != todoMerge))
  {
    AttrArray.push_back(new NSBasicAttribute(PERSON,   string(szPerson))) ;
    AttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
    AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
    AttrArray.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;

    pPatEnCours->getGraphPerson()->setInfoPids(&AttrArray) ;
    pPatEnCours->getGraphPerson()->getDataGraph()->setLastTree() ;
  }

    //debug lines
	string sCodePat = pPatEnCours->getGraphPerson()->getRootTree() ;
	// NSDataGraph* pGraph = pPatEnCours->pGraphPerson->pDataGraph ;

	SetCursor(0, IDC_ARROW) ;
	CloseWindow(IDOK) ;
}voidNSListeClientGroupDialog::CmCancel(){	CloseWindow(IDCANCEL) ;}voidNSListeClientGroupDialog::CmBack(){	// if (bListeLocal)	//  	return ;	bTrouveGlobal = false ;    pTexte->SetText("");    if (!pLastList->empty())    	etat = todoSearch ;    else    	etat = todoNothing ;	InitPatArray(pLastList) ;    AfficheListe(true);	setResearchOn() ;}voidNSListeClientGroupDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByName() ;      break ;
    case 1  : sortByBirthday() ;  break ;
    case 2  : sortBySex() ;       break ;
  }
}
voidNSListeClientGroupDialog::sortByName()
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
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByNameInf) ;
  else
    sort(pPatientsArray->begin(), pPatientsArray->end(), patSortByNameSup) ;

  AfficheListe(bListeLocal) ;
}

void
NSListeClientGroupDialog::sortByBirthday()
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

  AfficheListe(bListeLocal) ;
}

void
NSListeClientGroupDialog::sortBySex()
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

  AfficheListe(bListeLocal) ;
}
// -----------------------------------------------------------------//
//  Méthodes de NSNTiersListePatDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSNTiersRechPatDialog, NSNTiersListePatDialog)	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDC_LISTPA_OUVRIR, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_COMMAND(IDC_NTLISTPA_RECHPAT, Rechercher),
	EV_COMMAND(IDC_NTLISTPA_CREER, Creer),
	EV_LVN_GETDISPINFO(IDC_NTLISTPA_LW, LvnGetDispInfo),
	EV_LVN_COLUMNCLICK(IDC_NTLISTPA_LW, LVNColumnclick),
  EV_WM_TIMER,
  EV_COMMAND_AND_ID(IDC_USENOM,    nameActiver),
  EV_COMMAND_AND_ID(IDC_USEPRENOM, nameActiver),
END_RESPONSE_TABLE;NSNTiersRechPatDialog::NSNTiersRechPatDialog(TWindow* parent, NSContexte* pCtx, NSPatInfo* pPat, char* cResId)                      :NSNTiersListePatDialog(parent, pCtx, pPat, cResId, pCtx->GetMainWindow()->GetModule())
{
try
{
  _bDontEvenTryToOpen = false ;

  if (pContexte->getEpisodus())
	{
    _iAutoSearchDelay   = pContexte->getEpisodus()->_iAutoSearchDelay ;
    _iAutoSearchMinChar = pContexte->getEpisodus()->_iAutoSearchMinChar ;
  }
}
catch (...)
{
	erreur("Exception NSNTiersRechPatDialog ctor.", standardError, 0) ;
}
}

NSNTiersRechPatDialog::~NSNTiersRechPatDialog()
{
}

void
NSNTiersRechPatDialog::SetupWindow()
{
	NSNTiersListePatDialog::SetupWindow() ;
}

void
NSNTiersRechPatDialog::Creer()
{
try
{
  bool bAPatientAlreadyThere = true ;
  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
  	bAPatientAlreadyThere = false ;

  _sNomPat    = pNom->GetText(PAT_NOM_LEN) ;
  _sPrenomPat = pPrenom->GetText(PAT_PRENOM_LEN) ;
  _sCodePat   = pCode->GetText(PAT_CODE_LEN) ;

  pDateNaiss->getDate(&_sDateNaiss) ;

  // on enlève les blancs terminaux des nom et prénom
  strip(_sNomPat,    stripRight) ;
  strip(_sPrenomPat, stripRight) ;
  strip(_sCodePat,   stripRight) ;

  NSCaptureArray* pNewCaptureArray = (NSCaptureArray*) 0 ;
  if (pContexte->getEpisodus())
    pNewCaptureArray = &(pContexte->getEpisodus()->newCaptureArray) ;

  // Make certain not to be poluted by previous captures from search dialog
  //
  pNewCaptureArray->viderForOrigin(FromRechDialog) ;

/*
  // First, we check if this patient doesn't already exist
  //
  NSPersonsAttributesArray PatiensList ;
	NSBasicAttributeArray    AttrArray ;

  AttrArray.push_back(new NSBasicAttribute(ROLE, PATIENT_ROLE)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, FIRST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, LAST_NAME)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, SEX)) ;
  AttrArray.push_back(new NSBasicAttribute(TRAIT, BIRTHDATE)) ;
    // pAttrArray->push_back(new NSBasicAttribute(OPERATOR, user)) ;
    // pAttrArray->push_back(new NSBasicAttribute(CONSOLE, console)) ;
    // pAttrArray->push_back(new NSBasicAttribute(INSTANCE, string(szInstance))) ;

    bool res = pContexte->pPilot->personList(NautilusPilot::SERV_PERSON_LIST_FROM_TRAITS.c_str(), &PatiensList, &AttrArray) ;
    if (!res)
    {
      string tempMessage = pContexte->pPilot->getWarningMessage() ;
      string tempError = pContexte->pPilot->getErrorMessage() ;
      if (tempMessage != "")
        ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
      if (tempError != "")
        ::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
      return ;
    }

    if (!PatiensList.empty())
    {
      // cas d'erreur : le patient que l'on veut importer a déjà le même IPP en local
      erreur("Importation impossible : un patient détenant le même IPP que le patient sélectionné sur le SIH existe déjà en local.", standardError, 0) ;
      return ;
    }
*/

  // on enregistre les données présentes à l'écran dans les variables liées à la capture

  NSSuper *pSuper = pContexte->getSuperviseur() ;

  // 1. cas où il y a déjà des données dans les variables de capture
  //    on vérifie que ce ne sont pas les mêmes
  //    si c'est les mêmes, on laisse tel quel
  //
  if      (pNewCaptureArray && (false == pNewCaptureArray->empty()))
  {
    bool  bAlreadyNameCaptured      = false ;
    bool  bAlreadyFirstNameCaptured = false ;
    bool  bAlreadyBirthdateCaptured = false ;

    // on regarde si on est dans le cas d'une capture : on vérifie que le nom
    for (CaptureIter captIter = pNewCaptureArray->begin() ; captIter != pNewCaptureArray->end() ; captIter++)
    {
    	if (((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM01") && ((*captIter)->getLibelle() == _sNomPat))
      	bAlreadyNameCaptured      = true ;
      if (((*captIter)->getChemin() == "ZADMI1/LIDET1/LNOM21") && ((*captIter)->getLibelle() == _sPrenomPat))
      	bAlreadyFirstNameCaptured = true ;
      if (((*captIter)->getChemin() == "ZADMI1/LIDET1/KNAIS1") && ((*captIter)->getLibelle() == _sDateNaiss))
      	bAlreadyBirthdateCaptured = true ;
    }

    if ((false == bAlreadyNameCaptured) || (false == bAlreadyFirstNameCaptured) || (false == bAlreadyBirthdateCaptured))
    {
    	// pSuper->pEpisodus->CaptureArray.vider() ;

#ifndef _EXT_CAPTURE
			pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", _sNomPat, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", _sPrenomPat, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", _sDateNaiss, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LCODO1", _sCodePat, FromRechDialog)) ;
#else
			pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LNOM01", _sNomPat, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LNOM21", _sPrenomPat, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/KNAIS1", _sDateNaiss, FromRechDialog)) ;
      pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LCODO1", _sCodePat, FromRechDialog)) ;
#endif

			//pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", sNomPat)) ;
      //pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", sPrenomPat)) ;
      //pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LCODO1", sCodePat)) ;
		}
  }

  // 2. il n'y a pas de données dans les variables de capture, on met les données actuelles
  else if (pNewCaptureArray)
  {
#ifndef _EXT_CAPTURE
		pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", _sNomPat, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", _sPrenomPat, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/KNAIS1", _sDateNaiss, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture(pContexte, "ZADMI1/LIDET1/LCODO1", _sCodePat, FromRechDialog)) ;
#else
		pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LNOM01", _sNomPat, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LNOM21", _sPrenomPat, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/KNAIS1", _sDateNaiss, FromRechDialog)) ;
    pNewCaptureArray->ajouter(new NSCapture("ZADMI1/LIDET1/LCODO1", _sCodePat, FromRechDialog)) ;
#endif

		//pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM01", sNomPat)) ;
    //pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LNOM21", sPrenomPat)) ;
    //pSuper->pEpisodus->CaptureArray.push_back(new NSCapture(pContexte, "ZADMI1/LIDET1/LCODO1", sCodePat)) ;
  }

  // S'il n'y a pas de patient en cours, on swappe newCaptureArray et
  // CaptureArray (s'il y a un patient en cours, c'est fait dans
  // NSEpisodus::PatChanged()
  //
  if (false == bAPatientAlreadyThere)
  {
  	// pSuper->getEpisodus()->CaptureArray.vider() ;
    // pSuper->getEpisodus()->CaptureArray.append(&(pSuper->getEpisodus()->newCaptureArray)) ;
    // pSuper->getEpisodus()->newCaptureArray.vider() ;
#ifdef _IN_EXE
		pContexte->getEpisodus()->PutNewCaptureInTank(true /*bEmptyTankFirst*/, true /*bool bResetNew*/) ;
#endif
  }

  // on lance l'archetype de création de patient (voir nom dans nsarc.h)
  NSToDoTask *pTask = new NSToDoTask() ;
  pTask->BbkAskUser(pSuper, pSuper->getDemographicArchetypeId(), NSCQDocument::creatpat) ;
  // pContexte->BbkAskUser(pSuper->getDemographicArchetypeId(), NSCQDocument::creatpat) ;
  CloseWindow(IDCANCEL) ;
}
catch (...)
{
	erreur("Exception NSNTiersListePatDialog::Creer", standardError, 0) ;
}
}

voidNSNTiersRechPatDialog::CmOk(){try{
	int index = pListe->IndexItemSelect() ;
  if (-1 == index)
  {
  	// erreur("Vous devez sélectionner un patient dans la liste.",0,0,GetHandle());
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "PatientNotSelected") ;
    erreur(sErrorText.c_str(), warningError, standardError, 0) ;
    return ;
  }

  // When the list was set up from captured information, users tend to select
  // a patient with correct names without checking birthdate. We issue a
  // warning if capured and patient's birthdate are not the same
  //
  if (pContexte->getEpisodus())
	{
		NSCaptureArray* pCapt = &(pContexte->getEpisodus()->newCaptureArray) ;

    string sNom    = string("") ;
    string sPrenom = string("") ;
    string sNaiss  = string("") ;

    pCapt->getIdentity(sNom, sPrenom) ;
    pCapt->trouveChemin(string("KNAIS1"), &sNaiss) ;

    if ((string("") != sNom) && (string("") != sNaiss))
    {
      NSPatInfo* pSelectedPat = (*pPatientsArray)[index] ;
      if ((sNom == pSelectedPat->getNom()) && (sNaiss != pSelectedPat->getNaissance()))
      {
        string MsgHdr = pContexte->getSuperviseur()->getText("dialog_patientList", "patientChoice") ;
  	    string MsgTxt = pContexte->getSuperviseur()->getText("dialog_patientList", "notSameBirthdate") ;
        int retVal = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), MsgTxt.c_str(), MsgHdr.c_str(), MB_YESNO) ;
        if (IDNO == retVal)
          return ;
      }
    }
  }

  *pPatEnCours = *((*pPatientsArray)[index]) ;

  // In order to select a patient without trying to load her data
  // (by example in order to repair the record)
  //
  if (true == _bDontEvenTryToOpen)
  {
    CloseWindow(IDOK) ;
    return ;
  }

  string sPersonNss = pPatEnCours->getNss() ;

  string ps = string("Opening a new patient: Id ") + sPersonNss ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSteps) ;

  if ((LocalPatient != sPersonNss[0]) && (pContexte->getUtilisateur()->haveGlobalSessionPassword() == false))
  {
  	LogPassInterface* getGlobPassword = new LogPassInterface(this, pContexte) ;
    getGlobPassword->Execute() ;
    delete getGlobPassword ;
  }

/*
  SetCursor(0, IDC_WAIT) ;
  // pContexte->setPatient(new NSPatientChoisi(pContexte));
  string user = pContexte->getUtilisateurID() ;

  NSBasicAttributeArray    AttrArray ;
  NSPersonsAttributesArray List ;

  // ouverture standard
  AttrArray.push_back(new NSBasicAttribute(PERSON,   string(szPerson))) ;
  AttrArray.push_back(new NSBasicAttribute(OPERATOR, user)) ;
  AttrArray.push_back(new NSBasicAttribute(CONSOLE,  pContexte->getSuperviseur()->getConsoleString())) ;
  AttrArray.push_back(new NSBasicAttribute(INSTANCE, pContexte->getSuperviseur()->getInstanceString())) ;

  ps = string("Calling Pilot service \"searchPatient\"") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trDetails) ;

  bool res = pContexte->getSuperviseur()->getPilot()->searchPatient(NautilusPilot::SERV_SEARCH_PATIENT.c_str(),
                                    pPatEnCours->getGraphPerson()->getDataGraph(), &List, &AttrArray) ;
  if (false == res)
  {
  	std::string tempMessage = pContexte->getSuperviseur()->getPilot()->getWarningMessage() ;
    std::string tempError   = pContexte->getSuperviseur()->getPilot()->getErrorMessage() ;
    if (string("") != tempMessage)
    {
      pContexte->getSuperviseur()->trace(&tempMessage, 1, NSSuper::trWarning) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempMessage.c_str(), "Message Nautilus", MB_OK) ;
    }
    if (string("") != tempError)
    {
      pContexte->getSuperviseur()->trace(&tempError, 1, NSSuper::trError) ;
    	::MessageBox(pContexte->GetMainWindow()->GetHandle(), tempError.c_str(), "Message Nautilus", MB_OK) ;
    }
    return ;
  }

	if ((NULL == pPatEnCours->getGraphPerson()->getDataGraph()) ||
    	(false == pPatEnCours->getGraphPerson()->graphPrepare()))
  {
    if (NULL == pPatEnCours->getGraphPerson()->getDataGraph())
      ps = string("Empty graph.") ;
    else
      ps = string("Failure of the graphPrepare function.") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trWarning) ;
		return ;
  }

	pPatEnCours->getGraphPerson()->setNeedUnlock(false) ;
	pPatEnCours->getGraphPerson()->setReadOnly(true) ;

	if (false == List.empty())
	{
		string sIsLocked = List.getAttributeValue("locked") ;
		if (sIsLocked == "ok")
			pPatEnCours->getGraphPerson()->setNeedUnlock(true) ;
		string sOperationType	= List.getAttributeValue("operationType") ;
		if (sOperationType == "readWrite")
			pPatEnCours->getGraphPerson()->setReadOnly(false) ;
	}

  if (true == pPatEnCours->getGraphPerson()->getReadOnly())
  {
    string sCaption = pContexte->getSuperviseur()->getAppName() ;
    string sWarnText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
    MessageBox(sWarnText.c_str(), sCaption.c_str(), MB_OK) ;
  }

	pPatEnCours->getGraphPerson()->setInfoPids(&AttrArray) ;
	pPatEnCours->getGraphPerson()->getDataGraph()->setLastTree() ;

	string sCodePat = pPatEnCours->getGraphPerson()->getRootTree() ;
	// NSDataGraph* pGraph = pPatEnCours->pGraphPerson->pDataGraph;

	SetCursor(0, IDC_ARROW) ;
*/

/*
  if (false == pContexte->getUtilisateur()->OuvrePatient(sPersonNss))
  {
    ps = string("Closing the patient selection dialog (failed opening patient).") ;
    pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;
    CloseWindow(IDCANCEL) ;
    return ;
  }
*/

  ps = string("Closing the patient selection dialog.") ;
  pContexte->getSuperviseur()->trace(&ps, 1, NSSuper::trSubDetails) ;

	CloseWindow(IDOK) ;
}
catch (...)
{
	erreur("Exception NSNTiersListePatDialog::CmOk", standardError, 0) ;
}}#endif // N_TIERS

// -----------------------------------------------------------------
//
//  Méthodes de NSListGroupWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListGroupWindow, NSSkinableListWindow)
   EV_WM_KEYDOWN,
   EV_WM_LBUTTONDBLCLK,
   EV_WM_LBUTTONDOWN,
   EV_WM_KILLFOCUS,
   EV_WM_SETFOCUS,
END_RESPONSE_TABLE ;

NSListGroupWindow::NSListGroupWindow(NSListeClientGroupDialog* pere, NSContexte* pCtx, int resId)
                :NSSkinableListWindow(pere, pCtx, resId)
{
	pDlg = pere ;
    // Attr.Style |= LVS_REPORT | LVS_SHOWSELALWAYS ;    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY ;    Images  = 0 ;}

NSListGroupWindow::~NSListGroupWindow()
{
	if (Images)
        delete Images ;
}

void
NSListGroupWindow::SetupWindow()
{
    ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;

    NSSkinableListWindow::SetupWindow() ;

    skinSwitchOff("patientListOff") ;

    HINSTANCE hInstModule = *GetApplication() ;

    Images = new TImageList(NS_CLASSLIB::TSize(16, 16), ILC_COLOR4, 15, 5) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_EARLY_RED)) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_EARLY_YELLOW)) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_EARLY_GREEN)) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_BLUE)) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_GREEN)) ;
  	Images->Add(OWL::TBitmap(hInstModule, PAT_YELLOW)) ;
    Images->Add(OWL::TBitmap(hInstModule, PAT_RED)) ;
  	SetImageList(*Images, TListWindow::State) ;
}
// on redéfinit EvKeyDown pour pouvoir scroller avec les flèchesvoid
NSListGroupWindow::EvKeyDown(uint key, uint repeatCount, uint flags)
{
    int count, itemSel;

    switch (key)
    {
        case VK_DOWN :

            count = GetItemCount() ;
            itemSel = IndexItemSelect() ;
            if (itemSel == (count - 1))
            {
                pDlg->pVScroll->SBLineDown() ;
                SetSel(itemSel, true) ;
            }
            else if (itemSel != -1)
                SetSel(itemSel + 1, true) ;
            break ;

        case VK_UP :

            itemSel = IndexItemSelect() ;
            if (itemSel == 0)
            {
                pDlg->pVScroll->SBLineUp() ;
                SetSel(itemSel, true) ;
            }
            else if (itemSel != -1)
                SetSel(itemSel - 1, true) ;
            break ;

        case VK_NEXT :
            itemSel = IndexItemSelect() ;            if (itemSel != -1)
            {
                pDlg->pVScroll->SBPageDown() ;
                SetSel(itemSel, true) ;
            }
            break ;

        case VK_PRIOR :
            itemSel = IndexItemSelect() ;            if (itemSel != -1)
            {
                pDlg->pVScroll->SBPageUp() ;
                SetSel(itemSel, true) ;
            }
            break ;

        default :

            TListWindow::EvKeyDown(key, repeatCount, flags) ;    }
}

//---------------------------------------------------------------------------//  Function: NSListGroupWindow::EvLButtonDown(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point clické
//
//  Description: Fonction de réponse au click (désactive la multi-sélection)
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListGroupWindow::EvLButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
    // int count = GetItemCount();
	TLwHitTestInfo info(point);

    int index = HitTest(info);

    if (info.GetFlags() & LVHT_ONITEM)
   	    pDlg->GardeNss(index);

    // On appelle le Button Down de la classe mère
    TListWindow::EvLButtonDown(modKeys, point);
}

//---------------------------------------------------------------------------
//  Function: NSListGroupWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListGroupWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point);

    HitTest(info);

    if (info.GetFlags() & LVHT_ONITEM)
        pDlg->CmOk();
}

//---------------------------------------------------------------------------//  Function: NSListGroupWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListGroupWindow::IndexItemSelect()
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

// Gain du focus
//
void
NSListGroupWindow::EvSetFocus(HWND hWndLostFocus)
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
NSListGroupWindow::EvKillFocus(HWND hWndGetFocus)
{
	skinSwitchOff("patientListOff") ;

	TListWindow::EvKillFocus(hWndGetFocus) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSRechNomEdit
//
// -----------------------------------------------------------------

/*

DEFINE_RESPONSE_TABLE1(NSRechNomEdit, NSUtilEdit)
   EV_WM_CHAR,
   EV_WM_KEYUP,
END_RESPONSE_TABLE;

//// On doit intercepter EvChar pour interdire l'espace et le double-espace
//
void
NSRechNomEdit::EvChar(uint key, uint repeatCount, uint flags)
{
    char Nom[PAT_NOM_LEN + 1] = "";

    pDlg->pNom->GetText(Nom, PAT_NOM_LEN + 1);

    // on interdit l'espace comme premier caractere
    if ((!strcmp(Nom, "")) && (key == VK_SPACE))
        return;
    // on interdit le double espace
    if ((strlen(Nom) > 0) && (Nom[strlen(Nom)-1] == ' ') && (key == VK_SPACE))
        return;

	NSUtilEdit::EvChar(key, repeatCount, flags);
}

//
// On doit intercepter EvKeyUp pour tenir compte de VK_DELETE
// qui n'est pas intercepté par EvChar
//
void
NSRechNomEdit::EvKeyUp(uint key, uint repeatCount, uint flags)
{
    NSUtilEdit::EvKeyUp(key, repeatCount, flags);

    if ((key != VK_RETURN) && (key != VK_TAB))
   	    pDlg->ScrollPatient();

}

// -----------------------------------------------------------------
//
//  Méthodes de NSRechPreEdit
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRechPreEdit, NSUtilEdit)
   EV_WM_CHAR,
   EV_WM_KEYUP,
END_RESPONSE_TABLE;

//
// On doit intercepter EvChar pour interdire l'espace et le double-espace
//
void
NSRechPreEdit::EvChar(uint key, uint repeatCount, uint flags)
{
    char Prenom[PAT_PRENOM_LEN + 1] = "";

    pDlg->pPrenom->GetText(Prenom, PAT_PRENOM_LEN + 1);

    // on interdit l'espace comme premier caractere
    if ((!strcmp(Prenom, "")) && (key == VK_SPACE))
        return;
    // on interdit le double espace
    if ((Prenom[strlen(Prenom)-1] == ' ') && (key == VK_SPACE))
        return;

	NSUtilEdit::EvChar(key, repeatCount, flags);
}

//
// On doit intercepter EvKeyUp pour tenir compte de VK_DELETE
// qui n'est pas intercepté par EvChar
//
void
NSRechPreEdit::EvKeyUp(uint key, uint repeatCount, uint flags)
{
    NSUtilEdit::EvKeyUp(key, repeatCount, flags);

    if ((key != VK_RETURN) && (key != VK_TAB))
   	    pDlg->ScrollPatient();
}

*/

// -----------------------------------------------------------------
//
//  Méthodes de NSRechNumEdit
//
// -----------------------------------------------------------------

/*

DEFINE_RESPONSE_TABLE1(NSRechNumEdit, NSUtilEdit)
   EV_WM_CHAR,
END_RESPONSE_TABLE;


#ifdef N_TIERS

void
NSRechNumEdit::EvChar(uint key, uint repeatCount, uint flags)
{
}

#else

void
NSRechNumEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	DBIResult ErrDBI;
    char      NSS[PAT_NSS_LEN + 1] = "";
    char      Nom[PAT_NOM_LEN + 1] = "";
	char      Prenom[PAT_PRENOM_LEN + 1] = "";
    char      Code[PAT_CODE_LEN + 1] = "";

	NSUtilEdit::EvChar(key, repeatCount, flags);

    if (key == VK_RETURN)
    {
        pDlg->pNum->GetText(NSS, PAT_NSS_LEN + 1);

        if (!strcmp(NSS, ""))
            return;

        ErrDBI = pDlg->pPatEnCours->chercheClef((unsigned char*)NSS,
													   "",
													   0,
													   keySEARCHEQ,
                                                       dbiWRITELOCK);

        if (ErrDBI == DBIERR_RECNOTFOUND)
        {
            erreur("Aucun patient ne possède ce numéro.", standardError, 0) ;
            return;
        }

        ErrDBI = pDlg->pPatEnCours->getRecord();
   	    if (ErrDBI != DBIERR_NONE)
   	    {
   		    erreur("Erreur à lecture du fichier Adresses.db", 0, ErrDBI);
			return;
   	    }

        // on met à jour les champs liés
        strcpy(Nom, pDlg->pPatEnCours->pDonnees->nom);
        strcpy(Prenom, pDlg->pPatEnCours->pDonnees->prenom);
        strcpy(Code, pDlg->pPatEnCours->pDonnees->code);

        pDlg->pNom->SetText(Nom);
        pDlg->pPrenom->SetText(Prenom);
        pDlg->pCode->SetText(Code);

   	    pDlg->ScrollPatient();
    }
}
#endif

// -----------------------------------------------------------------
//
//  Méthodes de NSRechCodeEdit
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSRechCodeEdit, NSUtilEdit)
   EV_WM_KEYUP,
   EV_WM_CHAR,
END_RESPONSE_TABLE;

void
NSRechCodeEdit::EvKeyUp(uint key, uint repeatcount, uint flags)
{
    // on bloque cette fonction pour Return, Down ou Tab
    // pour empecher le NSUtilEdit de lancer CmOk
    // directement apres la recherche par code
    if (!((key == VK_RETURN) || (key == VK_DOWN) || (key == VK_TAB)))
        NSUtilEdit::EvKeyUp(key, repeatcount, flags);
}

void
NSRechCodeEdit::EvChar(uint key, uint repeatCount, uint flags)
{
}

*/

// -----------------------------------------------------------------
//
//  Méthodes de NSLVScrollBar
//
// -----------------------------------------------------------------

NSLVScrollBar::NSLVScrollBar(NSListePatDialog* parent, int resourceId, TModule* module)
				  :TScrollBar(parent, resourceId, module)
{
	pListeDlg = parent;
}

void
NSLVScrollBar::FixeRange(int max, int taille)
{
   // SetRange(1, max);

   // HWND hWnd = HWND();

   SCROLLINFO scInfo;
   scInfo.cbSize = (UINT) sizeof(scInfo);
   scInfo.fMask = SIF_ALL;
   scInfo.nMin = 1;
   scInfo.nMax = max;
   scInfo.nPage = (UINT) (taille);
   scInfo.nPos = 1;

   SetScrollInfo(&scInfo, TRUE);
}

void
NSLVScrollBar::SBLineDown()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineDown();
	pListeDlg->LineDown();
}

void
NSLVScrollBar::SBLineUp()
{
	// Transmet à la boite de Dialogue
	TScrollBar::SBLineUp();
	pListeDlg->LineUp();
}

void
NSLVScrollBar::SBPageDown()
{
	TScrollBar::SBPageDown();
   for (int i = 0; i < PageMagnitude; i++)
   	if (!pListeDlg->LineDown())
      	break;
}

void
NSLVScrollBar::SBPageUp()
{
	TScrollBar::SBPageUp();
   for (int i = 0; i < PageMagnitude; i++)
   	if (!pListeDlg->LineUp())
      	break;
}

void
NSLVScrollBar::PositionneCurseur()
{
#ifndef N_TIERS
	//
	// Prise du nombre total de d'enregistrement et de la position actuelle
	//
	int NumEnCours;

	//
	// Positionnement du curseur
	//
    pListeDlg->pPatEnCours->donneNumRecEnCours(&NumEnCours);
	SetPosition(NumEnCours);
#endif
}

//***********************************************************************////							Classe NSEnregDocPatientDlg
//***********************************************************************//

DEFINE_RESPONSE_TABLE1(NSEnregDocPatientDlg, NSUtilDialog)END_RESPONSE_TABLE;

NSEnregDocPatientDlg::NSEnregDocPatientDlg(TWindow* parent, NSContexte* pCtx,															TModule* module)
#ifndef _MUE
				   :NSUtilDialog(parent, pCtx, "IDD_ENREG_DOC_PATIENT", module)
#else
                   :NSUtilDialog(parent, pCtx, "IDD_ENREG_DOC_PATIENT", module) // à faire en mode MUE
#endif
{
    pChemBox = new TCheckBox(this, IDC_ENREG_DOC_CHEM);
    pIndexBox = new TCheckBox(this, IDC_ENREG_DOC_INDEX);
    pSynthBox = new TCheckBox(this, IDC_ENREG_DOC_SYNTH);
}

NSEnregDocPatientDlg::~NSEnregDocPatientDlg(){
    delete pChemBox;
    delete pIndexBox;
    delete pSynthBox;
}

void
NSEnregDocPatientDlg::SetupWindow()
{
    NSUtilDialog::SetupWindow();
}

voidNSEnregDocPatientDlg::CheckChem(){    pChemBox->Check();}voidNSEnregDocPatientDlg::CheckIndex(){    pIndexBox->Check();}voidNSEnregDocPatientDlg::CheckSynth(){    pSynthBox->Check();}//***********************************************************************//
//							Classe NSEnregPatientDlg
//***********************************************************************//

DEFINE_RESPONSE_TABLE1(NSEnregPatientDlg, NSUtilDialog)END_RESPONSE_TABLE;

NSEnregPatientDlg::NSEnregPatientDlg(TWindow* parent, NSContexte* pCtx,															TModule* module)
                   :NSUtilDialog(parent, pCtx, "IDD_ENREG_PATIENT_MUE", module)
{
  pAdminBox = new TCheckBox(this, IDC_ENREG_ADMIN) ;
  pHistoBox = new TCheckBox(this, IDC_ENREG_HISTO) ;
  pSynthBox = 0 ; // new TCheckBox(this, IDC_ENREG_SYNTH) ;
  pCorrsBox = new TCheckBox(this, IDC_ENREG_CORRS) ;
  pLDVBox   = new TCheckBox(this, IDC_ENREG_LDV) ;
}

NSEnregPatientDlg::~NSEnregPatientDlg(){
  delete pAdminBox ;
  delete pHistoBox ;
  if (NULL != pSynthBox)
    delete pSynthBox ;
  delete pCorrsBox ;
  delete pLDVBox ;
}

void
NSEnregPatientDlg::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;
}

voidNSEnregPatientDlg::CheckAdmin(){  pAdminBox->Check() ;}voidNSEnregPatientDlg::CheckHisto(){  pHistoBox->Check() ;}voidNSEnregPatientDlg::CheckSynth(){  if (NULL != pSynthBox)    pSynthBox->Check() ;}voidNSEnregPatientDlg::CheckCorrs()
{
  pCorrsBox->Check() ;
}void
NSEnregPatientDlg::CheckLDV()
{
  pLDVBox->Check() ;
}
// fin de nspatdlg.cpp

