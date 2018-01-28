#include <owl\applicat.h>
//#include <cstring.h>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "nsbb\nspatpat.h"

#include "nsepisod\nsepidiv.rh"
#include "nsepisod\nsepidiv.h"
#include "nsepisod\eptables.h"
#include "nsepisod\nsepisod.h"
#include "nsepisod\nsclasser.h"
#include "nsepisod\nssoapdiv.h"
#include "nautilus\nstrihis.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsbbtran.h"

#include "nsbb\nsednum.h"

// -----------------------------------------------------------------------------
//
//  Méthodes de NSFindCode
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CISPFindCodeDlg, NSUtilDialog)
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_CHILD_NOTIFY_AND_CODE(IDC_CODES_LIST, LBN_DBLCLK, CmDblClk),
  EV_BN_CLICKED(IDC_CODES_MORE, CmMore),
  EV_BN_CLICKED(IDC_CODES_LESS, CmLess),
  EV_COMMAND_AND_ID(IDC_CISP_1, evSetCategory),
  EV_COMMAND_AND_ID(IDC_CISP_2, evSetCategory),
  EV_COMMAND_AND_ID(IDC_CISP_3, evSetCategory),
END_RESPONSE_TABLE;

CISPFindCodeDlg::CISPFindCodeDlg(TWindow* pere, NSContexte* pCtx,
                                 SOAPObject* pObj, int iSelLevel,
                                 NSEpiClassifInfoArray* pPossibles,
                                 string* pControlData)
                :NSUtilDialog(pere, pCtx, "CLASSIFY_C2", pNSResModule)
{
try
{
  pObjet          = pObj ;
  iSelectionLevel = iSelLevel ;
  pArrayClassif   = pPossibles ;
  pCtrlData       = pControlData ;

  sChapitre       = string("") ;
  iCategory       = IcpcUndefined ;
  iPathology      = IcpcUndefinedPatho ;

  if ((pContexte->getEpisodus()) &&
      (pContexte->getEpisodus()->pClassifExpert))
    pExpert = pContexte->getEpisodus()->pClassifExpert ;
  else
    pExpert = 0 ;

  pCodeList       = new TListBox(this, IDC_CODES_LIST) ;
  sResult         = "" ;

  pLevelText      = new TStatic(this, IDC_CODES_LEVEL) ;
  pBtnMore        = new TButton(this, IDC_CODES_MORE) ;
  pBtnLess        = new TButton(this, IDC_CODES_LESS) ;

  pChapitres[0]   = new NSChapRadioButton(this, IDC_CISP_A, "A") ;
  pChapitres[1]   = new NSChapRadioButton(this, IDC_CISP_B, "B") ;
  pChapitres[2]   = new NSChapRadioButton(this, IDC_CISP_D, "D") ;
  pChapitres[3]   = new NSChapRadioButton(this, IDC_CISP_F, "F") ;
  pChapitres[4]   = new NSChapRadioButton(this, IDC_CISP_H, "H") ;
  pChapitres[5]   = new NSChapRadioButton(this, IDC_CISP_K, "K") ;
  pChapitres[6]   = new NSChapRadioButton(this, IDC_CISP_L, "L") ;
  pChapitres[7]   = new NSChapRadioButton(this, IDC_CISP_N, "N") ;
  pChapitres[8]   = new NSChapRadioButton(this, IDC_CISP_P, "P") ;
  pChapitres[9]   = new NSChapRadioButton(this, IDC_CISP_R, "R") ;
  pChapitres[10]  = new NSChapRadioButton(this, IDC_CISP_S, "S") ;
  pChapitres[11]  = new NSChapRadioButton(this, IDC_CISP_T, "T") ;
  pChapitres[12]  = new NSChapRadioButton(this, IDC_CISP_U, "U") ;
  pChapitres[13]  = new NSChapRadioButton(this, IDC_CISP_W, "W") ;
  pChapitres[14]  = new NSChapRadioButton(this, IDC_CISP_X, "X") ;
  pChapitres[15]  = new NSChapRadioButton(this, IDC_CISP_Y, "Y") ;
  pChapitres[16]  = new NSChapRadioButton(this, IDC_CISP_Z, "Z") ;

  pCategories[0]  = new OWL::TRadioButton(this, IDC_CISP_1) ;
  pCategories[1]  = new OWL::TRadioButton(this, IDC_CISP_2) ;
  pCategories[2]  = new OWL::TRadioButton(this, IDC_CISP_3) ;
}
catch (...)
{
	erreur("Exception new NSNewPreoccup.", standardError, 0) ;
}
}

CISPFindCodeDlg::~CISPFindCodeDlg()
{
  delete pCodeList ;

  delete pLevelText ;
  delete pBtnMore ;
  delete pBtnLess ;

  for (int i = 0; i < NBCHAPCISP; i++)
    delete pChapitres[i] ;

  for (int i = 0; i < 3; i++)
    delete pCategories[i] ;
}

void
CISPFindCodeDlg::SetupWindow()
{
  tpsDebut.takeTime() ;

  NSUtilDialog::SetupWindow() ;

  string sText = pContexte->getSuperviseur()->getText("classificationInterface", "general") ;
  pChapitres[0]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "blood") ;
  pChapitres[1]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "digestive") ;
  pChapitres[2]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "eye") ;
  pChapitres[3]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "ear") ;
  pChapitres[4]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "circulatory") ;
  pChapitres[5]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "musculoskeletal") ;
  pChapitres[6]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "neurological") ;
  pChapitres[7]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "psychological") ;
  pChapitres[8]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "respiratory") ;
  pChapitres[9]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "skin") ;
  pChapitres[10]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "metabolic") ;
  pChapitres[11]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "urology") ;
  pChapitres[12]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "pregnancy") ;
  pChapitres[13]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "female") ;
  pChapitres[14]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "male") ;
  pChapitres[15]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "social") ;
  pChapitres[16]->SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("classificationInterface", "symptoms") ;
  pCategories[0]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "process") ;
  pCategories[1]->SetCaption(sText.c_str()) ;
  sText = pContexte->getSuperviseur()->getText("classificationInterface", "healthProblems") ;
  pCategories[2]->SetCaption(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("classificationInterface", "level") ;
  pLevelText->SetText(sText.c_str()) ;

  sText = pContexte->getSuperviseur()->getText("classificationInterface", "classify") ;
  SetCaption(sText.c_str()) ;

  // Remplissage de la liste - Filling the listbox
  //
  if (false == pArrayClassif->empty())
  {
    NSEpiClassifInfoIter i = pArrayClassif->begin() ;
    for ( ; i != pArrayClassif->end() ; i++)
    {
      string sLibelle = (*i)->getCode() + string(" ") + (*i)->getLabel() ;
      pCodeList->AddString(sLibelle.c_str()) ;
    }
    // Affichage du niveau
    afficheNiveau() ;
  }
  else
    CmMore() ;

  iInitialLevel = iSelectionLevel ;
}

void
CISPFindCodeDlg::CmMore()
{
  if (iSelectionLevel == classifExpert::niveauTous)
    return ;

  iSelectionLevel++ ;

  if (pExpert)
  {
    pArrayClassif->vider() ;
    pExpert->donneClassifArray(pObjet, pArrayClassif, iSelectionLevel) ;
    listUpdate() ;
  }

  afficheNiveau() ;
}

void
CISPFindCodeDlg::CmLess()
{
  if (iSelectionLevel == classifExpert::niveauPreselection)
    return ;

  iSelectionLevel-- ;

  if (pExpert)
  {
    pArrayClassif->vider() ;
    pExpert->donneClassifArray(pObjet, pArrayClassif, iSelectionLevel) ;
    listUpdate() ;
  }

  afficheNiveau() ;
}

void
CISPFindCodeDlg::CmDblClk(WPARAM /* Cmd */)
{
	CmOk() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
CISPFindCodeDlg::CmOk()
{
  char szLibelle[512] ;
  int iSize = pCodeList->GetSelString(szLibelle, 512) ;
  if (iSize > 1)
  {
    size_t i = 0 ;
    for ( ; (i < strlen(szLibelle)) && (szLibelle[i] != ' '); i++) ;
      sResult = string(szLibelle, 0, i) ;
  }

  tpsFin.takeTime() ;
  buildCtrlString() ;

  TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
CISPFindCodeDlg::CmCancel()
{
  Destroy(IDCANCEL) ;
}

void
CISPFindCodeDlg::evSetChapitre(string sChap)
{
  sChapitre = sChap ;

  for (int i = 0 ; i < NBCHAPCISP ; i++)
    pChapitres[i]->initialise(sChapitre) ;

  listUpdate() ;
}

void
CISPFindCodeDlg::evSetCategory(WPARAM wParam)
{
  switch (wParam)
  {
    case IDC_CISP_1 :
      if (pCategories[0]->GetCheck() == BF_CHECKED)
        iCategory = IcpcUndefined ;
      else
        iCategory = IcpcSymptom ;
      break ;
    case IDC_CISP_2 :
      if (pCategories[1]->GetCheck() == BF_CHECKED)
        iCategory = IcpcUndefined ;
      else
        iCategory = IcpcProcedure ;
      break ;
    case IDC_CISP_3 :
      if (pCategories[2]->GetCheck() == BF_CHECKED)
        iCategory = IcpcUndefined ;
      else
        iCategory = IcpcPathology ;
      break ;
  }
  setCategoryButtonsState() ;
  listUpdate() ;    
}

void
CISPFindCodeDlg::afficheNiveau()
{
  string sSelectionLevelText = string("") ;
  
  if      (iSelectionLevel == classifExpert::niveauPreselection)
    sSelectionLevelText = pContexte->getSuperviseur()->getText("classificationInterface", "preselection") ;
  else if (iSelectionLevel == classifExpert::niveauAutorises)
    sSelectionLevelText = pContexte->getSuperviseur()->getText("classificationInterface", "validCodes") ;
  else if (iSelectionLevel == classifExpert::niveauTous)
    sSelectionLevelText = pContexte->getSuperviseur()->getText("classificationInterface", "allCodes") ;

  pLevelText->SetText(sSelectionLevelText.c_str()) ;
}

void
CISPFindCodeDlg::listUpdate()
{
  if (pCodeList->GetCount() > 0)
    pCodeList->ClearList() ;

  if (pArrayClassif->empty())
    return ;

  NSEpiClassifInfoIter i = pArrayClassif->begin() ;
  for ( ; pArrayClassif->end() != i ; i++)
  {
    // if sChapitre is "P" and current chapter is "PS", it should be ok
    //
    string sCodeChapter = (*i)->getChapter() ;
    size_t iChapitreLen = strlen(sChapitre.c_str()) ;
    if ((iChapitreLen > 0) &&
        (strlen(sChapitre.c_str()) < strlen(sCodeChapter.c_str())))
      sCodeChapter = string(sCodeChapter, 0, strlen(sChapitre.c_str())) ;

    if (((string("") == sChapitre) || (sCodeChapter == sChapitre)) &&
        ((IcpcUndefined == iCategory) || ((*i)->getIcpcCategory() == iCategory)))
    {
      string sLibelle = (*i)->getCode() + string(" ") + (*i)->getLabel() ;
      pCodeList->AddString(sLibelle.c_str()) ;
    }
  }
}

void
CISPFindCodeDlg::buildCtrlString()
{
  pArrayClassif->vider() ;
  if (iInitialLevel == classifExpert::niveauPreselection)
    pExpert->donneClassifArray(pObjet, pArrayClassif, iInitialLevel) ;

  pExpert->setControlString(pCtrlData, pObjet, iInitialLevel, pArrayClassif,
                                &tpsDebut, &tpsFin, sResult) ;
}

void
CISPFindCodeDlg::setCategoryButtonsState()
{
  for (int i = 0; i < 3; i++)
    pCategories[i]->SetCheck(BF_UNCHECKED) ;

  switch(iCategory)
  {
    case IcpcSymptom :
      pCategories[0]->SetCheck(BF_CHECKED) ;
      break ;
    case IcpcProcedure :
      pCategories[1]->SetCheck(BF_CHECKED) ;
      break ;
    case IcpcPathology :
      pCategories[2]->SetCheck(BF_CHECKED) ;
      break ;
  }
}

// -----------------------------------------------------------------------------
//
//  Méthodes de CIMFindCodeDlg
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CIMFindCodeDlg, NSUtilDialog)
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
    EV_CHILD_NOTIFY_AND_CODE(IDC_CODES_LIST, LBN_DBLCLK, CmDblClk),
    EV_BN_CLICKED(IDC_CODES_MORE, CmMore),
    EV_BN_CLICKED(IDC_CODES_LESS, CmLess),
END_RESPONSE_TABLE;


CIMFindCodeDlg::CIMFindCodeDlg(TWindow* pere, NSContexte* pCtx,
                                 SOAPObject* pObj, int iSelLevel,
                                 NSEpiClassifInfoArray* pPossibles,
                                 string* pControlData)
                :NSUtilDialog(pere, pCtx, "CLASSIFY_MA", pNSResModule)
{
try
{
    pObjet          = pObj;
    iSelectionLevel = iSelLevel;
    pArrayClassif   = pPossibles;
    pCtrlData       = pControlData ;

    if ((pContexte->getEpisodus()) &&
        (pContexte->getEpisodus()->pClassifExpert))
        pExpert = pContexte->getEpisodus()->pClassifExpert ;
    else
        pExpert = 0;

    pCodeList       = new TListBox(this, IDC_CODES_LIST);
    sResult         = "";

    pLevelText      = new TStatic(this, IDC_CODES_LEVEL);
    pBtnMore        = new TButton(this, IDC_CODES_MORE);
    pBtnLess        = new TButton(this, IDC_CODES_LESS);

    pChapitres[0]   = new NSChapRadioButton(this, IDC_CIM_I,        "1");
    pChapitres[1]   = new NSChapRadioButton(this, IDC_CIM_II,       "2");
    pChapitres[2]   = new NSChapRadioButton(this, IDC_CIM_III,      "3");
    pChapitres[3]   = new NSChapRadioButton(this, IDC_CIM_IV,       "4");
    pChapitres[4]   = new NSChapRadioButton(this, IDC_CIM_V,        "5");
    pChapitres[5]   = new NSChapRadioButton(this, IDC_CIM_VI,       "6");
    pChapitres[6]   = new NSChapRadioButton(this, IDC_CIM_VII,      "7");
    pChapitres[7]   = new NSChapRadioButton(this, IDC_CIM_VIII,     "8");
    pChapitres[8]   = new NSChapRadioButton(this, IDC_CIM_IX,       "9");
    pChapitres[9]   = new NSChapRadioButton(this, IDC_CIM_X,        "A");
    pChapitres[10]  = new NSChapRadioButton(this, IDC_CIM_XI,       "B");
    pChapitres[11]  = new NSChapRadioButton(this, IDC_CIM_XII,      "C");
    pChapitres[12]  = new NSChapRadioButton(this, IDC_CIM_XIII,     "D");
    pChapitres[13]  = new NSChapRadioButton(this, IDC_CIM_XIV,      "E");
    pChapitres[14]  = new NSChapRadioButton(this, IDC_CIM_XV,       "F");
    pChapitres[15]  = new NSChapRadioButton(this, IDC_CIM_XVI,      "G");
    pChapitres[16]  = new NSChapRadioButton(this, IDC_CIM_XVII,     "H");
    pChapitres[17]  = new NSChapRadioButton(this, IDC_CIM_XVIII1,   "I1");
    pChapitres[18]  = new NSChapRadioButton(this, IDC_CIM_XVIII1,   "I2");
    pChapitres[19]  = new NSChapRadioButton(this, IDC_CIM_XVIII1,   "I3");
    pChapitres[20]  = new NSChapRadioButton(this, IDC_CIM_XIX,      "J");
}
catch (...)
{
	erreur("Exception new NSNewPreoccup.", standardError, 0) ;
}
}


CIMFindCodeDlg::~CIMFindCodeDlg()
{
    delete pCodeList;

    delete pLevelText;
    delete pBtnMore;
    delete pBtnLess;

    for (int i = 0 ; i < NBCHAPCIM ; i++)
        delete pChapitres[i];
}

void
CIMFindCodeDlg::SetupWindow()
{
    tpsDebut.takeTime() ;

    NSUtilDialog::SetupWindow();

    string sLibelle;

    // Remplissage de la liste - Filling the listbox
    if (false == pArrayClassif->empty())
    {
        NSEpiClassifInfoIter i = pArrayClassif->begin();
        for ( ; i != pArrayClassif->end(); i++)
        {
            sLibelle = (*i)->getCode() + string(" ") + (*i)->getLabel() ;
            pCodeList->AddString(sLibelle.c_str());
        }
    }

    // Affichage du niveau
    afficheNiveau();

    iInitialLevel = iSelectionLevel ;
}


void
CIMFindCodeDlg::CmMore()
{
    if (iSelectionLevel == classifExpert::niveauTous)
        return;

    iSelectionLevel++;

    if (pExpert)
    {
        pArrayClassif->vider();
        pExpert->donneClassifArray(pObjet, pArrayClassif, iSelectionLevel);
        listUpdate();
    }

    afficheNiveau();
}


void
CIMFindCodeDlg::CmLess()
{
    if (iSelectionLevel == classifExpert::niveauPreselection)
        return;

    iSelectionLevel--;

    if (pExpert)
    {
        pArrayClassif->vider();
        pExpert->donneClassifArray(pObjet, pArrayClassif, iSelectionLevel);
        listUpdate();
    }

    afficheNiveau();
}


void
CIMFindCodeDlg::CmDblClk(WPARAM /* Cmd */)
{
	CmOk() ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
CIMFindCodeDlg::CmOk()
{
    char szLibelle[512] ;
    int iSize = pCodeList->GetSelString(szLibelle, 512) ;
    if (iSize > 1)
    {
        size_t i = 0 ;
        for ( ; (i < strlen(szLibelle)) && (szLibelle[i] != ' '); i++) ;
        sResult = string(szLibelle, 0, i) ;
    }

    tpsFin.takeTime() ;
    buildCtrlString() ;

    TDialog::CmOk() ;
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
CIMFindCodeDlg::CmCancel()
{
    Destroy(IDCANCEL);
}


void
CIMFindCodeDlg::evSetChapitre(string sChap)
{
    sChapitre = sChap;

    for (int i = 0; i < NBCHAPCIM; i++)
        pChapitres[i]->initialise(sChapitre);

    listUpdate();
}


void
CIMFindCodeDlg::afficheNiveau()
{
    if      (iSelectionLevel == classifExpert::niveauPreselection)
    {
        pLevelText->SetText("pré-sélection");
    }
    else if (iSelectionLevel == classifExpert::niveauAutorises)
    {
        pLevelText->SetText("codes autorisés");
    }
    else if (iSelectionLevel == classifExpert::niveauTous)
    {
        pLevelText->SetText("tous codes");
    }
}


void
CIMFindCodeDlg::listUpdate()
{
  if (pCodeList->GetCount() > 0)
    pCodeList->ClearList();

  if (false == pArrayClassif->empty())
  {
    NSEpiClassifInfoIter i = pArrayClassif->begin();
    for ( ; i != pArrayClassif->end(); i++)
    {
      if (((*i)->getChapter() == sChapitre) || (sChapitre == ""))
      {
        string sLibelle = (*i)->getCode() + string(" ") + (*i)->getLabel() ;
        pCodeList->AddString(sLibelle.c_str());
      }
    }
  }
}

void
CIMFindCodeDlg::buildCtrlString()
{
#ifdef _MUE
    pArrayClassif->vider() ;
    if (iInitialLevel == classifExpert::niveauPreselection)
        pExpert->donneClassifArray(pObjet, pArrayClassif, iInitialLevel) ;

    pExpert->setControlString(pCtrlData, pObjet, iInitialLevel, pArrayClassif,
                                &tpsDebut, &tpsFin, sResult) ;
#endif
}

// -----------------------------------------------------------------------------
//
//     class NSChapRadioButton
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NSChapRadioButton, TRadioButton)
	EV_NOTIFY_AT_CHILD(BN_CLICKED, BNClicked),
END_RESPONSE_TABLE;


// Constructeurs
NSChapRadioButton::NSChapRadioButton(NSUtilDialog* pPere, int resId, string sChap,
                                                        TGroupBox* group)
                  :TRadioButton(pPere, resId, group)
{
    sChapitre = sChap;
    pDlg      = pPere;
}


NSChapRadioButton::NSChapRadioButton(NSUtilDialog* pPere, int resId, string sChap,
                                     const char far* title,
                                     int x, int y, int w, int h,
                                     TGroupBox *group, TModule* module)
                  :TRadioButton(pPere, resId, title, x, y, w, h, group, module)
{
    sChapitre = sChap;

    // Attention, le constructeur de TRadioButton employé ici attribue le style
    // BS_AUTORADIOBUTTON, que nous remplaçons par BS_RADIOBUTTON
    Attr.Style    = (Attr.Style ^ BS_AUTORADIOBUTTON) | BS_RADIOBUTTON;
    Attr.ExStyle |= WS_EX_NOPARENTNOTIFY;
}


void
NSChapRadioButton::SetupWindow()
{
    TRadioButton::SetupWindow();

    SetCheck(BF_UNCHECKED);
    // SetCheck(BF_CHECKED);
    // SetCheck(BF_GRAYED);
}


void
NSChapRadioButton::initialise(string sSelChap)
{
    if (sChapitre == sSelChap)
        SetCheck(BF_CHECKED);
    else
        SetCheck(BF_UNCHECKED);
}


// -----------------------------------------------------------------------------
// Destructeur
// -----------------------------------------------------------------------------
NSChapRadioButton::~NSChapRadioButton()
{
}


// -----------------------------------------------------------------------------
// Fonction déclenché lorsque la boite à cocher est activée.
// -----------------------------------------------------------------------------
void
NSChapRadioButton::BNClicked()
{
    if (!pDlg)
        return;

    CISPFindCodeDlg* pCISPview = TYPESAFE_DOWNCAST(pDlg, CISPFindCodeDlg);
    if ( pCISPview )
    {
        switch (GetCheck())
        {
            case BF_UNCHECKED   : pCISPview->evSetChapitre(sChapitre);
                                  break;
            case BF_CHECKED 	: pCISPview->evSetChapitre("");
                                  break;
            case BF_GRAYED 	    : break;
        }
        return;
    }

    CIMFindCodeDlg* pCIMview = TYPESAFE_DOWNCAST(pDlg, CIMFindCodeDlg);
    if ( pCIMview )
    {
        switch (GetCheck())
        {
            case BF_UNCHECKED   : pCIMview->evSetChapitre(sChapitre);
                                  break;
            case BF_CHECKED 	: pCIMview->evSetChapitre("");
                                  break;
            case BF_GRAYED 	    : break;
        }
        return;
    }

    //TRadioButton::BNClicked();
}


// -----------------------------------------------------------------------------
//
// class PrometheComplementDlg
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(PrometheComplementDlg, NSUtilDialog)
    EV_WM_CLOSE,
    EV_COMMAND(IDOK, CmOk),
    EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

PrometheComplementDlg::PrometheComplementDlg(TWindow* pere, NSContexte* pCtx,
                                             string* pDatN, string* pZip,
                                             string* pRType, string* pRMode,
                                             TModule* module)
                      :NSUtilDialog(pere, pCtx, "IDD_PROMET_PRESEND", module)
{
try
{
	// Initialisation des donnees

	pPatNaiss   = pDatN ;
	pPatCP      = pZip ;
	pExamCond   = pRType ;
	pExamRV     = pRMode ;

	// Valeurs par défaut
	if (*pExamCond == "")
		*pExamCond = "C" ;
	if (*pExamRV == "")
		*pExamRV = "N" ;

	// Création de tous les "objets de contrôle"
  pCodePost       = new NSUtilEdit(pContexte, this, IDC_PROPAT_ZIP, ADR_CODE_POST_LEN) ;
  pDateN          = new NSUtilEditDate(pContexte, this, IDC_PROPAT_DATEN, 10, false) ;

  pTypeRencontre  = new TGroupBox(this, IDC_PROPAT_GR1) ;
  pConsult	      = new TRadioButton(this, IDC_PROPAT_C, pTypeRencontre) ;
  pVisite         = new TRadioButton(this, IDC_PROPAT_V, pTypeRencontre) ;

  pModeRencontre  = new TGroupBox(this, IDC_PROPAT_GR2) ;
  pSsRV	          = new TRadioButton(this, IDC_PROPAT_NRV, pTypeRencontre) ;
  pAvecRV         = new TRadioButton(this, IDC_PROPAT_RV, pTypeRencontre) ;
  pUrgence        = new TRadioButton(this, IDC_PROPAT_URG, pTypeRencontre) ;
}
catch (...)
{
	erreur("Exception (new PrometheComplementDlg)", standardError, 0) ;
}
}


PrometheComplementDlg::~PrometheComplementDlg()
{
    delete pUrgence;
    delete pAvecRV;
    delete pSsRV;
    delete pModeRencontre;
    delete pVisite;
    delete pConsult;
    delete pTypeRencontre;
    delete pDateN;
	delete pCodePost;
}


void
PrometheComplementDlg::SetupWindow()
{
    TDialog::SetupWindow();

    pCodePost->SetText(pPatCP->c_str());
    pDateN->setDate(*pPatNaiss);

    if      (*pExamCond == "C")
   	    pConsult->Check();
    else if (*pExamCond == "V")
   	    pVisite->Check();

    if      (*pExamRV == "O")
   	    pAvecRV->Check();
    else if (*pExamRV == "N")
   	    pSsRV->Check();
    else if (*pExamRV == "U")
   	    pUrgence->Check();
}


// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
PrometheComplementDlg::CmOk()
{
  *pPatCP = pCodePost->GetText() ;
  strip(*pPatCP, stripBoth) ;

  string sDateN ;
  pDateN->getDate(&sDateN) ;
  if ((string("19000000") == sDateN) || (string("00000000") == sDateN))
    *pPatNaiss = string("") ;
  else
    *pPatNaiss = sDateN ;

  if      (pConsult->GetCheck() == BF_CHECKED)
    *pExamCond = string("C");
  else if (pVisite->GetCheck()  == BF_CHECKED)
    *pExamCond = string("V");

  if      (pSsRV->GetCheck()    == BF_CHECKED)
    *pExamRV = string("N");
  else if (pAvecRV->GetCheck()  == BF_CHECKED)
    *pExamRV = string("O");
  else if (pUrgence->GetCheck() == BF_CHECKED)
    *pExamRV = string("U");

	if      (*pExamCond == "")
  	erreur("Le champ Consultation/Visite est obligatoire.", warningError, 0, GetHandle()) ;
	else if (*pPatNaiss == "")
		erreur("Le champ Date de naissance est obligatoire.", warningError, 0, GetHandle()) ;
	else
		TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
PrometheComplementDlg::CmCancel()
{
    int retVal = MessageBox("Voulez-vous vraiment annuler la création de ce message ?", "Message Episodus", MB_YESNO);

    if (retVal == IDYES)
	    TDialog::CmCancel();
}


void
PrometheComplementDlg::EvClose()
{
    CmCancel();
}

