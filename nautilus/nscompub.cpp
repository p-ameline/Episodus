// NSCOMPUB.CPP : Méthodes dédiées à la composition et à la publication// RS Septembre 1997
//////////////////////////////////////////////////////////////////////////

#include "nautilus\Lettre.h"#include "nautilus\nsIndySmtp.h"

#include <owl/applicat.h>#include <owl/olemdifr.h>
#include <owl/dialog.h>
#include <owl/edit.h>
#include <owl/color.h>
#include <owl/module.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>
#include <winuser.h>
#include <classlib/date.h>
#include <classlib/time.h>

#include "nautilus\nssuper.h"#include "nautilus\nsanxary.h"
#include "partage\nsdivfct.h"
#include "partage\nsdivfile.h"
#include "partage\nsperson.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nshistdo.h"
#include "nautilus\nsresour.h"
#include "nautilus\nsvisual.h"
#include "nautilus\nsbasimg.h"
#include "nautilus\nsbrowse.h"
#include "nautilus\nautilus.rh"
#include "nautilus\ns_html.h"
#include "nautilus\nscompub.h"
#include "nautilus\nsldvdoc.h"
#include "nsbb\nsmanager.h"
#include "nautilus\nsdocview.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsHealthTeam.h"
#include "nsdn\nsintrad.h"
#include "nsoutil\nsHtmlToPdf.h"
#include "ns_crypt\ns_api_dll.h"

#include "pilot\NautilusPilot.hpp"
#include "nsbb\tagNames.h"

long NSChoixPubli::lObjectCount = 0 ;
long NSMapiCtrl::lObjectCount = 0 ;
long NSLdvPubli::lObjectCount = 0 ;
long NSPublication::lObjectCount = 0 ;

TModule* pCryptoModule = 0 ;
bool bEnvoyerReturn = true ;

// handle de hook
HHOOK hhook ;

// -----------------------------------------------------------------//
//  Méthodes de ChoixTemplateDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixTemplateDialog, NSUtilDialog)	EV_CHILD_NOTIFY_AND_CODE(IDC_TEMPLATEBOX, LBN_SELCHANGE, CmSelectTemplate),
	EV_CHILD_NOTIFY_AND_CODE(IDC_TEMPLATEBOX, LBN_DBLCLK, CmTemplateDblClk),
END_RESPONSE_TABLE;

ChoixTemplateDialog::ChoixTemplateDialog(TWindow* pere, NSContexte* pCtx, string typeDoc, string codeSensRoot)                    :NSUtilDialog(pere, pCtx, "IDD_TEMPLATE")
{
try
{
	pTemplateBox   = new OWL::TListBox(this, IDC_TEMPLATEBOX) ;
	pTemplateArray = new NSTemplateArray ;
	sTypeDoc       = typeDoc ;
	sCodeSensRoot  = codeSensRoot ;

	// on cherche les équivalents sémantiques du code Root	if (sCodeSensRoot != "")
		pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(sCodeSensRoot, &VectTermeEquivalentRoot, "ES") ;

	TemplateChoisi = 0 ;
	// fichiers d'aide	sHindex = "hi_doc.htm" ;
	sHcorps = "h_compos.htm" ;
}
catch (...)
{
	erreur("Exception ChoixTemplateDialog ctor", standardError, 0) ;
}
}

ChoixTemplateDialog::~ChoixTemplateDialog(){
	delete pTemplateBox ;
	delete pTemplateArray ;
}

voidChoixTemplateDialog::SetupWindow()
{
try
{
	string sCodeRootTmpl, sCodeSensTmpl ;
	string sTypeTmpl, sCompoTmpl, sUtilTmpl, sLibTmpl ;
	bool   bTypeTmplOK ;

	NSUtilDialog::SetupWindow() ;
	//	// Remplissage de TemplateBox avec les libelles des fichiers modele
	//
	// ------------ Appel du pilote
	NSObjectGraphManager ObjectManager(pContexte->getSuperviseur()) ;	NSDataGraph* pGraph = ObjectManager.getDataGraph() ;	string sTraitType = string("_0OTPL") + string("_0TYPE");  string sTraitOper = string("_0OTPL") + string("_DOPER");
  string sTraitCons = string("_0OTPL") + string("_LNUCO");
  string sTraitRoot = string("_0OTPL") + string("_0TYPC");
  string sTraitComp = string("_0OTPL") + string("_0COMD");
  string sTraitDefa = string("_0OTPL") + string("_0DEFA");  string sTraitLibelle = string("_0OTPL") + string("_0INTI");	NSPersonsAttributesArray ObjList ;	NSBasicAttributeArray AttrArray ;	string sTypeDocComplet = sTypeDoc ;  NSDico::donneCodeComplet(sTypeDocComplet) ;	AttrArray.push_back(new NSBasicAttribute(sTraitType, sTypeDocComplet)) ;
	bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST_WITH_TRAITS.c_str(),
                                    &ObjList, &AttrArray) ;
	if (false == res)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "templateNotFound") ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return ;
	}	NSPatPathoArray PatPathoArray(pContexte->getSuperviseur(), graphObject) ;	if (false == ObjList.empty())  {  	for (NSPersonsAttributeIter i = ObjList.begin() ; ObjList.end() != i ; i++)    {    	string sOIDS = (*i)->getAttributeValue(OIDS) ;      AttrArray.vider() ;      pGraph->graphReset() ;      if (string("") != sOIDS)      {
      	//pList->push_back(new NSBasicAttribute("graphID", sOIDS)) ;
        AttrArray.push_back(new NSBasicAttribute(OBJECT, sOIDS)) ;
        res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), pGraph, &AttrArray) ;
        if (false == res)
        {
        	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
          if (string("") == sErrorText)
          	sErrorText = string("Echec service : Impossible de récupérer un objet dans la base") ;
          sErrorText = string(" ") + sOIDS ;
          pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
          erreur(sErrorText.c_str(), standardError, 0) ;
          return ;
        }

        NSDataTreeIter iterTree ;
        PatPathoArray.vider() ;

        if (pGraph->getTrees()->ExisteTree("0OTPL1", pContexte->getSuperviseur(), &iterTree))
        	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
        else
        {
        	erreur("Problème à la récupération du graphe d'un objet template.", standardError, 0);
          continue ;
        }

        ObjectManager.ParseTemplate(&PatPathoArray, &AttrArray) ;

        sCodeRootTmpl = AttrArray.getAttributeValue(sTraitRoot) ;
        sTypeTmpl     = AttrArray.getAttributeValue(sTraitType) ;
        sCompoTmpl    = AttrArray.getAttributeValue(sTraitComp) ;
        sUtilTmpl     = AttrArray.getAttributeValue(sTraitOper) ;
        sLibTmpl      = AttrArray.getAttributeValue(sTraitLibelle) ;

        bTypeTmplOK = false ;

        if (sCodeRootTmpl != "")        {
        	// le code root de la base template est-il un équivalent du code root doc ?
          NSDico::donneCodeSens(&sCodeRootTmpl, &sCodeSensTmpl) ;

          for (EquiItemIter i = VectTermeEquivalentRoot.begin(); i != VectTermeEquivalentRoot.end(); i++)          {
          	if (sCodeSensTmpl == (*(*i)))
            {
            	bTypeTmplOK = true ;
              break ;
            }
          }
        }
        else // cas codeRoot == "", on regarde le type
        	if (sTypeDocComplet == sTypeTmpl)
          	bTypeTmplOK = true ;

        // On remplit la TemplateBox et le TemplateArray si le type correspond au type Document        // On regarde si compo == 1 (car utilisé en composition)
        // On regarde si on a le bon utilisateur ou aucun

        if ((bTypeTmplOK)                      &&
            (sCompoTmpl == "1")
                      &&
            ((sUtilTmpl == pContexte->getUtilisateur()->getNss()) ||
                     (sUtilTmpl == "")
              )
            )
        {
        	pTemplateBox->AddString(sLibTmpl.c_str()) ;
          pTemplateArray->push_back(new NSTemplateInfo(&AttrArray)) ;
        }
      }    }  }}catch (...)
{
	erreur("Exception ChoixTemplateDialog::SetupWindow.", standardError, 0) ;
}
}

void ChoixTemplateDialog::CmTemplateDblClk(WPARAM Cmd)
{
	TemplateChoisi = pTemplateBox->GetSelIndex() + 1;
	TDialog::CmOk();
}

void ChoixTemplateDialog::CmSelectTemplate(WPARAM Cmd){
	//
	// Récupération de l'indice du template sélectionné
	//

	TemplateChoisi = pTemplateBox->GetSelIndex() + 1;}

void ChoixTemplateDialog::CmCancel(){
	TemplateChoisi = 0;

	TDialog::CmCancel();}

// -----------------------------------------------------------------//
//  Méthodes de NomCompoDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NomCompoDialog, NSUtilDialog)   EV_BN_CLICKED(IDC_NEWCOMPO, CmClickNewCompo),
   EV_BN_CLICKED(IDC_OLDCOMPO, CmClickOldCompo),
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

NomCompoDialog::NomCompoDialog(TWindow* pere, NSContexte* pCtx)               :NSUtilDialog(pere, pCtx, "IDD_NOMCOMPO")
{
try
{
  pNomDocHtml = new OWL::TEdit(this, IDC_EDITNOMCOMPO, DOC_NOM_LEN) ;
  pNewCompo 	= new OWL::TRadioButton(this, IDC_NEWCOMPO, 0) ;
  pOldCompo 	= new OWL::TRadioButton(this, IDC_OLDCOMPO, 0) ;

  sNomDocHtml = string("") ;}
catch (...)
{
	erreur("Exception NomCompoDialog ctor.", standardError, 0) ;
}
}

NomCompoDialog::~NomCompoDialog(){
  delete pNomDocHtml ;
  delete pNewCompo ;
  delete pOldCompo ;
}

voidNomCompoDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  pOldCompo->Check() ;  pNewCompo->Uncheck() ;
}

voidNomCompoDialog::CmClickNewCompo()
{
	pOldCompo->Uncheck() ;
}

voidNomCompoDialog::CmClickOldCompo()
{
	pNewCompo->Uncheck() ;
}

voidNomCompoDialog::CmOk()
{
	char far cfNomDocHtml[DOC_NOM_LEN + 1] ;

  pNomDocHtml->GetText(cfNomDocHtml,DOC_NOM_LEN) ;	sNomDocHtml = string(cfNomDocHtml) ;

	if ((pNewCompo->GetCheck()) == BF_CHECKED)  {
    if (string("") == sNomDocHtml)
    {
      MessageBox("Erreur : Vous n'avez pas saisi de nom pour le nouveau fichier") ;
      return ;
    }
  }

  TDialog::CmOk() ;}

voidNomCompoDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------//
//  Méthodes de LettreTypeEditDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(LettreTypeEditDialog, NSUtilDialog)   EV_BN_CLICKED(IDC_LTYPEDIT_NEW, CmNouveau),
   EV_BN_CLICKED(IDC_LTYPEDIT_DEL, CmDetruire),
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

LettreTypeEditDialog::LettreTypeEditDialog(TWindow* pere, NSContexte* pCtx)                     :NSUtilDialog(pere, pCtx, "IDD_LTYPEDIT")
{
try
{
  _sTexte = string("") ;

	_pData = new NSFormuleData() ;
	_pEdit = new NSMultiEdit(pContexte, this, IDC_LTYPEDIT_EDIT, FORM_TEXTE1_LEN +
                                                     FORM_TEXTE2_LEN +
                                                     FORM_TEXTE3_LEN +
                                                     FORM_TEXTE4_LEN) ;
  _bNew = false ;
  _bDel = false ;
}
catch (...)
{
	erreur("Exception LettreTypeEditDialog ctor.", standardError, 0) ;
}
}

LettreTypeEditDialog::~LettreTypeEditDialog(){
	delete _pEdit ;
  delete _pData ;
}

voidLettreTypeEditDialog::SetupWindow()
{
  NSUtilDialog::SetupWindow() ;

  if (strcmp(_pData->code, ""))  {
    _sTexte += _pData->texte1 ;
    _sTexte += _pData->texte2 ;
    _sTexte += _pData->texte3 ;
    _sTexte += _pData->texte4 ;
  }
  else
    _bNew = true ;

  _pEdit->FormatLines(true) ;  _pEdit->SetText(_sTexte.c_str()) ;
}

voidLettreTypeEditDialog::CmNouveau()
{
  _sTexte = string("") ;

  _pEdit->SetText("") ;
  _bNew = true ;}

voidLettreTypeEditDialog::CmDetruire()
{
  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();

  int idRet = MessageBox("Etes vous sûr de vouloir détruire cette formule?", sCaption.c_str(), MB_YESNO);

  if (IDYES != idRet)    return ;
  _bDel = true;

  if (_bNew)    TDialog::CmCancel() ;
  else
    TDialog::CmOk() ;
}

voidLettreTypeEditDialog::CmOk()
{
  _pEdit->GetText(_sTexte) ;

  // test préalable  if (strlen(_sTexte.c_str()) > (FORM_TEXTE1_LEN + FORM_TEXTE2_LEN + FORM_TEXTE3_LEN + FORM_TEXTE4_LEN))
  {
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();

    int idRet = MessageBox("Attention le texte est trop long et va être tronqué. Voulez-vous continuer ?", sCaption.c_str(), MB_YESNO);
    if (idRet == IDNO)      return ;
  }

  strcpy(_pData->texte1, "") ;  strcpy(_pData->texte2, "") ;
  strcpy(_pData->texte3, "") ;
  strcpy(_pData->texte4, "") ;

  if (strlen(_sTexte.c_str()) > FORM_TEXTE1_LEN)  {
    strcpy(_pData->texte1, string(_sTexte, 0, FORM_TEXTE1_LEN).c_str()) ;
    _sTexte = string(_sTexte, FORM_TEXTE1_LEN, strlen(_sTexte.c_str()) - FORM_TEXTE1_LEN + 1) ;
  }
  else
  {
    strcpy(_pData->texte1, _sTexte.c_str()) ;
    TDialog::CmOk() ;
    return ;
  }

  if (strlen(_sTexte.c_str()) > FORM_TEXTE2_LEN)  {
    strcpy(_pData->texte2, string(_sTexte, 0, FORM_TEXTE2_LEN).c_str()) ;
    _sTexte = string(_sTexte, FORM_TEXTE2_LEN, strlen(_sTexte.c_str()) - FORM_TEXTE2_LEN + 1) ;
  }
  else
  {
    strcpy(_pData->texte2, _sTexte.c_str()) ;
    TDialog::CmOk() ;
    return ;
  }

  if (strlen(_sTexte.c_str()) > FORM_TEXTE3_LEN)  {
    strcpy(_pData->texte3, string(_sTexte, 0, FORM_TEXTE3_LEN).c_str()) ;
    _sTexte = string(_sTexte, FORM_TEXTE3_LEN, strlen(_sTexte.c_str()) - FORM_TEXTE3_LEN + 1) ;
  }
  else
  {
    strcpy(_pData->texte3, _sTexte.c_str()) ;
    TDialog::CmOk() ;
    return ;
  }

  if (strlen(_sTexte.c_str()) > FORM_TEXTE4_LEN)    strcpy(_pData->texte1, string(_sTexte, 0, FORM_TEXTE4_LEN).c_str()) ;
  else
    strcpy(_pData->texte1, _sTexte.c_str()) ;

  TDialog::CmOk() ;}

voidLettreTypeEditDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de LettreTypeDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(LettreTypeDialog, NSUtilDialog)
   EV_BN_CLICKED(IDC_LTYP_INTRO_SANS, CmClickSansIntro),
   EV_BN_CLICKED(IDC_LTYP_CORPS_SANS, CmClickSansCorps),
   EV_BN_CLICKED(IDC_LTYP_POLIT_SANS, CmClickSansPolit),
   EV_COMMAND(IDC_LTYP_INTRO_PREC, CmIntroPrec),
   EV_COMMAND(IDC_LTYP_INTRO_SUIV, CmIntroSuiv),
   EV_COMMAND(IDC_LTYP_INTRO_EDIT, CmEditIntro),
   EV_COMMAND(IDC_LTYP_CORPS_PREC, CmCorpsPrec),
   EV_COMMAND(IDC_LTYP_CORPS_SUIV, CmCorpsSuiv),
   EV_COMMAND(IDC_LTYP_CORPS_EDIT, CmEditCorps),
   EV_COMMAND(IDC_LTYP_POLIT_PREC, CmPolitPrec),
   EV_COMMAND(IDC_LTYP_POLIT_SUIV, CmPolitSuiv),
   EV_COMMAND(IDC_LTYP_POLIT_EDIT, CmEditPolit),
   EV_COMMAND(IDOK, CmOk),
   EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

LettreTypeDialog::LettreTypeDialog(TWindow* pere, NSContexte* pCtx)                 :NSUtilDialog(pere, pCtx, "IDD_LTYP")
{
try
{
  _pIntro      = new NSUtilEdit(pContexte, this, IDC_LTYP_INTRO) ;
  _pCorps      = new NSUtilEdit(pContexte, this, IDC_LTYP_CORPS) ;
  _pPolit      = new NSUtilEdit(pContexte, this, IDC_LTYP_POLIT) ;
  _pSansIntro  = new OWL::TRadioButton(this, IDC_LTYP_INTRO_SANS, 0) ;
  _pSansCorps  = new OWL::TRadioButton(this, IDC_LTYP_CORPS_SANS, 0) ;
  _pSansPolit  = new OWL::TRadioButton(this, IDC_LTYP_POLIT_SANS, 0) ;
  _pIntroArray = new NSFormuleArray ;
  _pCorpsArray = new NSFormuleArray ;
  _pPolitArray = new NSFormuleArray ;

  _nbIntro = 0 ;
  _nbCorps = 0 ;
  _nbPolit = 0 ;

  _choixIntro = 0 ;
  _choixCorps = 0 ;
  _choixPolit = 0 ;

  _sIntro = string("") ;  _sCorps = string("") ;
  _sPolit = string("") ;
}
catch (...)
{
  erreur("Exception LettreTypeDialog ctor.", standardError, 0) ;
}
}

LettreTypeDialog::~LettreTypeDialog(){
	delete _pIntro ;
  delete _pCorps ;
  delete _pPolit ;
  delete _pSansIntro ;
  delete _pSansCorps ;
  delete _pSansPolit ;
  delete _pIntroArray ;
  delete _pCorpsArray ;
  delete _pPolitArray ;
}

voidLettreTypeDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  _pSansIntro->Uncheck() ;  _pSansCorps->Uncheck() ;
  _pSansPolit->Uncheck() ;

	if (InitFormulesArray())  {
    _pIntro->FormatLines(true) ;
    AfficheIntro() ;

    _pCorps->FormatLines(true) ;    AfficheCorps() ;

    _pPolit->FormatLines(true) ;    AffichePolit() ;
  }
  else
    erreur("La base des formules est inaccessible.", standardError, 0, GetHandle()) ;
}

boolLettreTypeDialog::InitFormulesArray()
{
try
{
  // pour le cas base vide
  _sLastCode = string("000") ;

	NSFormule formule(pContexte->getSuperviseur()) ;
	formule.lastError = formule.open() ;
	if (DBIERR_NONE != formule.lastError)	{
		erreur("Impossible d'ouvrir le fichier Formule.", standardError, formule.lastError, GetHandle()) ;
		return false ;
	}

  formule.lastError = formule.debut(dbiWRITELOCK) ;
  if (DBIERR_BOF == formule.lastError)  {
    formule.close() ;
		return true ;
	}

  while (DBIERR_EOF != formule.lastError)  {
    formule.lastError = formule.getRecord() ;

    if (DBIERR_NONE != formule.lastError)    {
      erreur("Erreur à la lecture d'une formule.", standardError, formule.lastError, GetHandle()) ;
      formule.close() ;
			return false ;
		}

    _sLastCode = formule.pDonnees->code ;
    switch (formule.pDonnees->type[0])    {
      case 'A' :
        _pIntroArray->push_back(new NSFormuleInfo(&formule)) ;
        _nbIntro++ ;
        break ;

      case 'B' :        _pCorpsArray->push_back(new NSFormuleInfo(&formule)) ;
        _nbCorps++ ;
        break ;

      case 'C':        _pPolitArray->push_back(new NSFormuleInfo(&formule)) ;
        _nbPolit++ ;
        break ;

      default:        erreur("Type de formule non prévu dans la base", standardError, 0, GetHandle()) ;
    }

    // on passe à la formule suivante    formule.lastError = formule.suivant(dbiWRITELOCK) ;

    if ((DBIERR_NONE != formule.lastError) && (DBIERR_EOF != formule.lastError))    {
      erreur("Erreur d'accès à la formule suivante.", standardError, formule.lastError, GetHandle()) ;
      formule.close() ;
			return false ;
		}
  } // fin du while

  // on ferme la base Formule
  formule.lastError = formule.close() ;

	if (DBIERR_NONE != formule.lastError)	{
		erreur("Impossible de fermer le fichier Formule.", standardError, formule.lastError, GetHandle()) ;
		return false ;
	}

  return true ;}
catch (...)
{
	erreur("Exception LettreTypeDialog::InitFormulesArray.", standardError, 0) ;
	return false ;
}
}

voidLettreTypeDialog::RemplaceTags(string& sTexte)
{
  string sLang = pContexte->getUserLanguage() ;

	// remplacement du tag [nomPatient]
	//
	string sNomLong    = string("") ;
  string sEpisodusId = string("") ;
  if (pContexte->getPatient())
  {
  	pContexte->getPatient()->fabriqueNomLong() ;
    sNomLong = pContexte->getPatient()->getNomLong() ;
  }
	size_t pos = sTexte.find("[nomPatient]") ;
	while (NPOS != pos)
	{
    sTexte.replace(pos, strlen("[nomPatient]"), sNomLong) ;
		pos = sTexte.find("[nomPatient]", pos + 1) ;
  }
	// remplacement du tag [dateJour]  //  pos = sTexte.find("[dateJour]") ;
	while (NPOS != pos)
	{
  	string sDateJ   = donne_date_duJour() ;
		string sMessage = donne_date(sDateJ, sLang) ;

		sTexte.replace(pos, strlen("[dateJour]"), sMessage) ;

		pos = sTexte.find("[dateJour]", pos + strlen(sMessage.c_str())) ;
	}
}

voidLettreTypeDialog::AfficheIntro()
{
	string sTexte = string("") ;

  if (_nbIntro)  {
    sTexte += ((*_pIntroArray)[_choixIntro])->pDonnees->texte1 ;
    sTexte += ((*_pIntroArray)[_choixIntro])->pDonnees->texte2 ;
    sTexte += ((*_pIntroArray)[_choixIntro])->pDonnees->texte3 ;
    sTexte += ((*_pIntroArray)[_choixIntro])->pDonnees->texte4 ;

    RemplaceTags(sTexte) ;    _sIntro = sTexte ;
    _pIntro->SetText(sTexte.c_str()) ;
  }
  else
    _pIntro->SetText("<vide>") ;
}

voidLettreTypeDialog::AfficheCorps()
{
	string sTexte = string("") ;

  if (_nbCorps)  {
    sTexte += ((*_pCorpsArray)[_choixCorps])->pDonnees->texte1 ;
    sTexte += ((*_pCorpsArray)[_choixCorps])->pDonnees->texte2 ;
    sTexte += ((*_pCorpsArray)[_choixCorps])->pDonnees->texte3 ;
    sTexte += ((*_pCorpsArray)[_choixCorps])->pDonnees->texte4 ;

    RemplaceTags(sTexte) ;
    _sCorps = sTexte ;    _pCorps->SetText(sTexte.c_str()) ;
  }
  else
    _pCorps->SetText("<vide>") ;
}

voidLettreTypeDialog::AffichePolit()
{
	string sTexte = string("") ;

  if (_nbPolit)  {
    sTexte += ((*_pPolitArray)[_choixPolit])->pDonnees->texte1 ;
    sTexte += ((*_pPolitArray)[_choixPolit])->pDonnees->texte2 ;
    sTexte += ((*_pPolitArray)[_choixPolit])->pDonnees->texte3 ;
    sTexte += ((*_pPolitArray)[_choixPolit])->pDonnees->texte4 ;

    RemplaceTags(sTexte) ;
    _sPolit = sTexte ;    _pPolit->SetText(sTexte.c_str()) ;
  }
  else
    _pPolit->SetText("<vide>") ;
}

voidLettreTypeDialog::CmIntroPrec()
{
	if (_nbIntro && (_choixIntro >= 0))
  {
    _choixIntro--;

    if (_choixIntro < 0)      _choixIntro = _nbIntro - 1 ;

    AfficheIntro() ;  }
}

voidLettreTypeDialog::CmIntroSuiv()
{
	if (_nbIntro && (_choixIntro >= 0))
  {
    _choixIntro++ ;

    if (_choixIntro == _nbIntro)      _choixIntro = 0 ;

    AfficheIntro() ;  }
}

voidLettreTypeDialog::CmCorpsPrec()
{
	if (_nbCorps && (_choixCorps >= 0))
  {
    _choixCorps-- ;

    if (_choixCorps < 0)      _choixCorps =  _nbCorps - 1 ;

    AfficheCorps() ;  }
}

voidLettreTypeDialog::CmCorpsSuiv()
{
	if (_nbCorps && (_choixCorps >= 0))
  {
    _choixCorps++ ;

    if (_choixCorps == _nbCorps)      _choixCorps = 0 ;

    AfficheCorps() ;  }
}

voidLettreTypeDialog::CmPolitPrec()
{
	if (_nbPolit && (_choixPolit >= 0))
  {
    _choixPolit-- ;

    if (_choixPolit < 0)      _choixPolit = _nbPolit - 1 ;

    AffichePolit() ;  }
}

voidLettreTypeDialog::CmPolitSuiv()
{
  if (_nbPolit && (_choixPolit >= 0))
  {
    _choixPolit++ ;

    if (_choixPolit == _nbPolit)      _choixPolit = 0 ;

    AffichePolit() ;  }
}

voidLettreTypeDialog::CmClickSansIntro()
{
	if (_choixIntro >= 0)
  {
    _sIntro = string("") ;
    _pIntro->SetText("") ;
    _pSansIntro->Check() ;
    _choixIntro = -1 ;
  }
  else
  {
    _choixIntro = 0 ;
    _pSansIntro->Uncheck() ;
    AfficheIntro() ;
  }
}

voidLettreTypeDialog::CmClickSansCorps()
{
	if (_choixCorps >= 0)
  {
    _sCorps = string("") ;
    _pCorps->SetText("") ;
    _pSansCorps->Check() ;
    _choixCorps = -1 ;
  }
  else
  {
    _choixCorps = 0 ;
    _pSansCorps->Uncheck() ;
    AfficheCorps() ;
  }
}

voidLettreTypeDialog::CmClickSansPolit()
{
	if (_choixPolit >= 0)
  {
    _sPolit = string("") ;
    _pPolit->SetText("") ;
    _pSansPolit->Check() ;
    _choixPolit = -1 ;
  }
  else
  {
    _choixPolit = 0 ;
    _pSansPolit->Uncheck() ;
    AffichePolit() ;
  }
}

voidLettreTypeDialog::CmEditIntro()
{
try
{
  NSFormuleInfo form ;
  if (_pSansIntro->GetCheck() == BF_CHECKED)    return ;

  if (_nbIntro)    *(form.pDonnees) = *(((*_pIntroArray)[_choixIntro])->pDonnees) ;

  string sMode = string("") ;
  CmEditFormule(form.pDonnees, sMode) ;
  if (string("new") == sMode)  {
    _pIntroArray->push_back(new NSFormuleInfo(form)) ;
    _nbIntro++ ;
    _choixIntro = _nbIntro - 1 ;
    AfficheIntro() ;
  }
  else if (string("mod") == sMode)
  {
    *(((*_pIntroArray)[_choixIntro])->pDonnees) = *(form.pDonnees) ;
    AfficheIntro() ;
  }
  else if (string("del") == sMode)
  {
    int j = 0 ;
    if (false == _pIntroArray->empty())
      for (NSFormuleIter it = _pIntroArray->begin() ; _pIntroArray->end() != it ; j++)      {
        if (j == _choixIntro)
        {
          delete *it ;
          _pIntroArray->erase(it) ;
          _nbIntro-- ;
          break ;
        }
        else
          it++ ;
      }

    _choixIntro = 0 ;
    AfficheIntro() ;
  }
}
catch (...)
{
	erreur("Exception LettreTypeDialog::CmEditIntro.", standardError, 0) ;
}
}

voidLettreTypeDialog::CmEditCorps()
{
try
{
  if (_pSansCorps->GetCheck() == BF_CHECKED)
    return ;

  NSFormuleInfo form ;
  if (_nbCorps)    *(form.pDonnees) = *(((*_pCorpsArray)[_choixCorps])->pDonnees) ;

  string sMode = string("") ;
  CmEditFormule(form.pDonnees, sMode) ;
  if (string("new") == sMode)  {
    _pCorpsArray->push_back(new NSFormuleInfo(form)) ;
    _nbCorps++ ;
    _choixCorps = _nbCorps - 1 ;
    AfficheCorps() ;
  }
  else if (string("mod") == sMode)
  {
    *(((*_pCorpsArray)[_choixCorps])->pDonnees) = *(form.pDonnees) ;
    AfficheCorps() ;
  }
  else if (string("del") == sMode)
  {
    int j = 0 ;
    if (false == _pCorpsArray->empty())
      for (NSFormuleIter it = _pCorpsArray->begin() ; _pCorpsArray->end() != it ; j++)
      {
        if (j == _choixCorps)
        {
          delete *it ;
          _pCorpsArray->erase(it) ;
          _nbCorps-- ;
          break ;
        }
        else
          it++ ;
      }

    _choixCorps = 0 ;
    AfficheCorps() ;
  }
}
catch (...)
{
	erreur("Exception LettreTypeDialog::CmEditCorps.", standardError, 0) ;
}
}

voidLettreTypeDialog::CmEditPolit()
{
try
{
	if (_pSansPolit->GetCheck() == BF_CHECKED)		return ;

	NSFormuleInfo FormInfo ;

	if (_nbPolit)  	*(FormInfo.pDonnees) = *(((*_pPolitArray)[_choixPolit])->pDonnees) ;

  string sMode = string("") ;
	CmEditFormule(FormInfo.pDonnees, sMode) ;
	if (string("new") == sMode)	{
  	_pPolitArray->push_back(new NSFormuleInfo(FormInfo)) ;
    _nbPolit++ ;
    _choixPolit = _nbPolit - 1 ;
    AffichePolit() ;
	}
  else if (string("mod") == sMode)
  {
  	*(((*_pPolitArray)[_choixPolit])->pDonnees) = *(FormInfo.pDonnees) ;
    AffichePolit() ;
  }
  else if (string("del") == sMode)
  {
    int j = 0 ;
    if (false == _pPolitArray->empty())
    	for (NSFormuleIter it = _pPolitArray->begin() ; _pPolitArray->end() != it ; j++)    	{
    		if (j == _choixPolit)
      	{
      		delete *it ;
        	_pPolitArray->erase(it) ;
          _nbPolit-- ;
        	break ;
      	}
        else
          it++ ;
    	}

    _choixPolit = 0 ;
    AffichePolit() ;
  }
}
catch (...)
{
	erreur("Exception LettreTypeDialog::CmEditPolit.", standardError, 0) ;
}
}

voidLettreTypeDialog::CmEditFormule(NSFormuleData* pFormData, string& sMode)
{
try
{
  LettreTypeEditDialog* pEditDlg = new LettreTypeEditDialog(this, pContexte) ;
  *(pEditDlg->_pData) = *pFormData ;

  sMode = string("annul") ;
  if (pEditDlg->Execute() != IDOK)  {    delete pEditDlg ;    return ;  }
  NSFormule Formule(pContexte->getSuperviseur()) ;

  Formule.lastError = Formule.open() ;
  if (Formule.lastError != DBIERR_NONE)
  {
    erreur("Impossible d'ouvrir le fichier Formule.", standardError, Formule.lastError, GetHandle()) ;
    delete pEditDlg ;
    return ;
  }

  if (pEditDlg->_bNew)  {
    if (false == Formule.IncrementeCode(&_sLastCode))
    {
      erreur("Erreur à l'attribution d'un code formule.", standardError, Formule.lastError, GetHandle()) ;
      Formule.close() ;
      delete pEditDlg ;
      return ;
    }

    *(Formule.pDonnees) = *(pEditDlg->_pData) ;    strcpy(Formule.pDonnees->code, _sLastCode.c_str()) ;

    Formule.lastError = Formule.appendRecord() ;
    if (Formule.lastError != DBIERR_NONE)    {
      erreur("Erreur à l'ajout de la nouvelle formule.", standardError, Formule.lastError, GetHandle()) ;
      Formule.close() ;
      delete pEditDlg ;
      return ;
    }

    *pFormData = *(pEditDlg->_pData) ;    sMode = "new" ;
  }
  else
  {
    // Attention la base est indexée sur code + type
    string sCode = string(pEditDlg->_pData->code) + string(pEditDlg->_pData->type) ;

    // on recherche la fiche dans la base    Formule.lastError = Formule.chercheClef(&sCode,
                                            "",
                                            0,
                                            keySEARCHEQ,
                                            dbiWRITELOCK) ;

    if (Formule.lastError != DBIERR_NONE)    {
      erreur("Erreur à la recherche de la formule.", standardError, Formule.lastError, GetHandle()) ;
      Formule.close() ;
      delete pEditDlg ;
      return ;
    }

    if (pEditDlg->_bDel)    {
      Formule.lastError = Formule.deleteRecord() ;

      if (Formule.lastError != DBIERR_NONE)      {
        erreur("Erreur à la destruction de la formule.", standardError, Formule.lastError, GetHandle()) ;
        Formule.close() ;
        delete pEditDlg ;
        return ;
      }

      sMode = string("del") ;    }
    else
    {
      *(Formule.pDonnees) = *(pEditDlg->_pData) ;

      Formule.lastError = Formule.modifyRecord(TRUE) ;
      if (Formule.lastError != DBIERR_NONE)      {
        erreur("Erreur à la modification de la formule.", standardError, Formule.lastError, GetHandle()) ;
        Formule.close() ;
        delete pEditDlg ;
        return ;
      }

      *pFormData = *(pEditDlg->_pData) ;      sMode = "mod" ;
    }
  }

  Formule.lastError = Formule.close() ;
  if (Formule.lastError != DBIERR_NONE)    erreur("Impossible de fermer le fichier Formule.", standardError, Formule.lastError, GetHandle()) ;

  delete pEditDlg ;
}
catch (...)
{
	erreur("Exception LettreTypeDialog::CmEditFormule.", standardError, 0) ;
}
}

voidLettreTypeDialog::CmOk()
{
	TDialog::CmOk() ;
}

voidLettreTypeDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSPubliEdit
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSPubliEdit, NSUtilEdit)	EV_WM_CHAR,
END_RESPONSE_TABLE;

NSPubliEdit::NSPubliEdit(NSContexte *pCtx, NSUtilDialog* pUtilDialog, int resId, int iTextLen, bool bReturn)            :NSUtilEdit(pCtx, pUtilDialog, resId, iTextLen, bReturn){
  iResId = resId ;
}

NSPubliEdit::~NSPubliEdit(){
}

void
NSPubliEdit::EvChar(uint key, uint repeatCount, uint flags)
{
	UINT 		correspSelect ;

	PubliCorrespDialog* pPCDialog = dynamic_cast<PubliCorrespDialog*>(pNSUtilDialog) ;	NSPersonInfo* pCorrespSelect = pPCDialog->getSelectedPerson() ;
	NSUtilEdit::EvChar(key,repeatCount,flags) ;
	if (IDC_PUBLIER_NBEXPL == iResId)	{
  	// GetText(sNbExpl, WF_NB_PUBLI_LEN + 1) ;
    string sNbExpl = GetText(2) ;
    pPCDialog->SetNbExpl(pCorrespSelect, sNbExpl.c_str()) ;
  }
  else if (IDC_PUBLIER_URL == iResId)
  {
  	string sUrl = GetText(COR_MESSAGERIE_LEN) ;
    pPCDialog->SetUrl(pCorrespSelect, sUrl.c_str()) ;
	}
}

// -----------------------------------------------------------------
//
//  Méthodes de PubliCorrespDialog
//
// -----------------------------------------------------------------
NSChoixPubli::NSChoixPubli()
{
  lObjectCount++ ;

  _sCorresp      = string("") ;
  _bSelect       = false ;  _bImprimante   = false ;
  _iCpt          = 0 ;
  _iCptDuplicata = 0 ;
  _nb_expl[0]    = '\0' ;
  _bEmail        = false ;
  _sUrl          = string("") ;  _bIntranet     = false ;
  _bHtml         = false ;
  _bPdf          = false ;
  _bLettre       = false ;
}

NSChoixPubli::NSChoixPubli(const NSChoixPubli& rv)
{
  lObjectCount++ ;

  _sCorresp      = rv._sCorresp ;
  _bSelect       = rv._bSelect ;  _bImprimante   = rv._bImprimante ;
  _iCpt          = rv._iCpt ;
  _iCptDuplicata = rv._iCptDuplicata ;
  _bEmail        = rv._bEmail ;
  _sUrl          = rv._sUrl ;  _bIntranet     = rv._bIntranet ;
  _bHtml         = rv._bHtml ;
  _bPdf          = rv._bPdf ;
  _bLettre       = rv._bLettre ;

  strcpy(_nb_expl, rv._nb_expl) ;
}

NSChoixPubli::~NSChoixPubli()
{
  lObjectCount-- ;
}

NSChoixPubli&
NSChoixPubli::operator=(const NSChoixPubli& src)
{
  if (&src == this)
		return *this ;

  _sCorresp      = src._sCorresp ;
  _bSelect       = src._bSelect ;  _bImprimante   = src._bImprimante ;
  _iCpt          = src._iCpt ;
  _iCptDuplicata = src._iCptDuplicata ;
  _bEmail        = src._bEmail ;
  _sUrl          = src._sUrl ;  _bIntranet     = src._bIntranet ;
  _bHtml         = src._bHtml ;
  _bPdf          = src._bPdf ;
  _bLettre       = src._bLettre ;

  strcpy(_nb_expl, src._nb_expl) ;

  return *this ;
}

// -----------------------------------------------------------------//
//  Méthodes de PubliCorrespDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(PubliCorrespDialog, NSUtilDialog)	EV_CHILD_NOTIFY_AND_CODE(IDC_PUBLIER_CORRESPBOX, LBN_SELCHANGE, CmSelectCorresp),
	EV_CHILD_NOTIFY_AND_CODE(IDC_PUBLIER_CORRESPBOX, LBN_DBLCLK, CmCorrespDblClk),
	EV_COMMAND(IDC_PUBLIER_SELECT,        LanceChoixCorrespDialog),
  EV_BN_CLICKED(IDC_PUBLIER_IMPRIMANTE, CmClickImprimante),
  EV_BN_CLICKED(IDC_PUBLIER_EMAIL,      CmClickEmail),
  EV_BN_CLICKED(IDC_PUBLIER_INTRANET,   CmClickIntranet),
  EV_BN_CLICKED(IDC_PUBLIER_HTML,       CmClickHtml),
  EV_BN_CLICKED(IDC_PUBLIER_PDF,        CmClickPdf),
  EV_BN_CLICKED(IDC_PUBLIER_JOINDRE,    CmClickJoindre),
  EV_BN_CLICKED(IDC_PUBLIER_LETTRE,     CmLettreType),
  EV_BN_CLICKED(IDC_PUBLIER_PATIENT,    CmAjouterPatient),
  EV_BN_CLICKED(IDC_PUBLIER_BLANK,      CmAjouterBlank),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

PubliCorrespDialog::PubliCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication)
                   :NSUtilDialog(pere, pCtx, "IDD_PUBLIER"), _aCorrespArray(pCtx->getSuperviseur())
{
try
{
	_pCorrespBox   = new OWL::TListBox(this, IDC_PUBLIER_CORRESPBOX) ;
	_pAdresse      = new NSUtilEdit(pContexte, this, IDC_PUBLIER_ADRESSE) ;
	// pNbExpl       = new NSPubliEdit(this, IDC_PUBLIER_NBEXPL, WF_NB_PUBLI_LEN) ;
  _pNbExpl       = new NSPubliEdit(pContexte, this, IDC_PUBLIER_NBEXPL, 2) ;
	_pUrl          = new NSPubliEdit(pContexte, this, IDC_PUBLIER_URL, COR_MESSAGERIE_LEN) ;
	_pImprimante   = new OWL::TRadioButton(this, IDC_PUBLIER_IMPRIMANTE, 0) ;
	_pEmail        = new OWL::TRadioButton(this, IDC_PUBLIER_EMAIL, 0) ;
	_pIntranet     = 0 ; // new OWL::TRadioButton(this, IDC_PUBLIER_INTRANET, 0) ;
	_pHtml         = new OWL::TRadioButton(this, IDC_PUBLIER_HTML, 0) ;
  _pPdf          = new OWL::TRadioButton(this, IDC_PUBLIER_PDF, 0) ;
	_pLettre       = new OWL::TCheckBox(this, IDC_PUBLIER_LETTRE) ;
	_pJoindre      = new OWL::TRadioButton(this, IDC_PUBLIER_JOINDRE, 0) ;
	_pPubliPatient = new OWL::TRadioButton(this, IDC_PUBLIER_PATIENT, 0) ;
	_pPubliBlank   = new OWL::TRadioButton(this, IDC_PUBLIER_BLANK, 0) ;

	_pLettreType   = 0 ;	_pPubli = pPublication ;
	_pCorrespBaseArray = &(_pPubli->_aCorrespBaseArray) ;
	pContexte->setAideIndex("hi_doc.htm") ;	pContexte->setAideCorps("h_publi.htm#Phase2") ;
}
catch (...)
{
	erreur("Exception PubliCorrespDialog ctor.", standardError, 0) ;
}
}

PubliCorrespDialog::~PubliCorrespDialog(){
	delete _pPubliPatient ;
  delete _pPubliBlank ;
	delete _pJoindre ;
	delete _pLettre ;
	delete _pHtml ;
  delete _pPdf ;
  if (_pIntranet)
	  delete _pIntranet ;
	delete _pEmail ;
	delete _pImprimante ;
	delete _pAdresse ;
	delete _pUrl ;
	delete _pNbExpl ;
	delete _pCorrespBox ;

	// le delete de pLettreType est pris en charge par CmOk et CmCancel	_pPubli = 0 ;					  // ne pas deleter
	_pCorrespBaseArray = 0 ;    // ne pas deleter
}

voidPubliCorrespDialog::InitCorrespArray()
{
try
{
	_aCorrespArray.vider() ;
	_pPubliPatient->Uncheck() ;
	if (_pCorrespBaseArray->empty())
		return ;

  string sPatientId = string("") ;
  if (pContexte->getPatient())
    sPatientId = pContexte->getPatient()->getNss() ;

	for (NSPersonIter i = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != i ; i++)
	{  	// Checking if this person is selected    //    NSChoixPubli* pChoice = _pPubli->getChoixPubliForPerson(*i) ;    if (pChoice && pChoice->isSelected())    {  		// Is this person the current patient ?    	if ((*i)->getNss() == sPatientId)    		_pPubliPatient->Check() ;    	// Adding this person to the "to be published" array
    	_aCorrespArray.push_back(new NSPersonInfo(**i)) ;
    }
	}
}catch (...)
{
  pContexte->traceAndDisplayError(string("Exception PubliCorrespDialog::InitCorrespArray")) ;
}
}

voidPubliCorrespDialog::AfficheCorresp()
{
	// on reprend le tableau à chaque coup à cause des changements de sélection
  //
	InitCorrespArray() ;

	// on vide la liste si elle contient des items  //	if (_pCorrespBox->GetCount())
		_pCorrespBox->ClearList() ;

	if (false == _aCorrespArray.empty())	{
  	for (NSPersonIter i = _aCorrespArray.begin() ; _aCorrespArray.end() != i ; i++)
    {
    	// on remplit la CorrespBox
      if (string("") == (*i)->getNss())
      {
      	string sBlankAddressText = pContexte->getSuperviseur()->getText("publishingManagement", "blankAddress") ;
        _pCorrespBox->AddString(sBlankAddressText.c_str()) ;
      }
      else
      {
      	string sCorresp = (*i)->getNom() + string(" ") + (*i)->getPrenom() ;
      	_pCorrespBox->AddString(sCorresp.c_str()) ;
      }
    }
  }

	_pAdresse->SetText("") ;	_pNbExpl->SetText("") ;

	if (false == _aCorrespArray.empty())	{
  	_pCorrespBox->SetSelIndex(0) ;
    AfficheAdresse(_aCorrespArray[0]) ;
    AfficheChoixPubli(_aCorrespArray[0]) ;
	}
	else
	{
		_pImprimante->Uncheck() ;
		_pEmail->Uncheck() ;
		_pIntranet->Uncheck() ;
		_pHtml->Uncheck() ;
    _pPdf->Uncheck() ;
		_pLettre->Uncheck() ;
		_pJoindre->Uncheck() ;
	}
}

voidPubliCorrespDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;
	_pAdresse->FormatLines(true) ;

	//	// Remplissage de CorrespBox avec le NSCorrespArray
	//
	AfficheCorresp() ;
}

voidPubliCorrespDialog::LanceChoixCorrespDialog()
{
	ChoixCorrespDialog(pContexte->GetMainWindow(), pContexte, _pPubli).Execute() ;

	AfficheCorresp() ;}

voidPubliCorrespDialog::SetNbExpl(NSPersonInfo *pCorrespSelect, const char* sNbExpl){
  NSChoixPubli* pPubliForP = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;

  if (pPubliForP && pPubliForP->wantToPrint())
  {
    strcpy(pPubliForP->_nb_expl, sNbExpl) ;
    pPubliForP->_iCpt = atoi(pPubliForP->_nb_expl) ;
  }
}

voidPubliCorrespDialog::SetUrl(NSPersonInfo *pCorrespSelect, const char* sUrl){
  NSChoixPubli* pPubliForP = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;

  if (pPubliForP && pPubliForP->wantMail())
    pPubliForP->_sUrl = string(sUrl) ;
}

boolPubliCorrespDialog::ExisteLettre(NSPersonInfo *pCorrespSelect){
try
{
	if ((NSPersonInfo*) NULL == pCorrespSelect)
		return false ;

	string sDocId = _pPubli->getCodeLettre(pCorrespSelect->getNss()) ;

  return (string("") != sDocId) ;
}
catch (...)
{
  pContexte->traceAndDisplayError(string("Exception PubliCorrespDialog::ExisteLettre")) ;
	return false ;
}
}

void
PubliCorrespDialog::ResetChoixPubli()
{
  _pImprimante->Check() ;

  _pEmail->Uncheck() ;
  _pUrl->SetText(string("")) ;

  if (_pIntranet)
    _pIntranet->Uncheck() ;
    
  _pHtml->Uncheck() ;
  _pPdf->Uncheck() ;

  _pLettre->Uncheck() ;
  _pJoindre->Uncheck() ;
}

voidPubliCorrespDialog::AfficheChoixPubli(NSPersonInfo *pCorrespSelect){
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pPubliForP = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pPubliForP)
    return ;

  if (pPubliForP->wantToPrint())
  {
    _pImprimante->Check() ;
    _pNbExpl->SetText(pPubliForP->_nb_expl) ;
  }
  else
  {
    _pImprimante->Uncheck() ;
    _pNbExpl->SetText("") ;
  }

  _pUrl->SetText(pPubliForP->getMailAddress()) ;

  if (pPubliForP->wantMail())  {
    if (string("") == pPubliForP->getMailAddress())
      _pEmail->Uncheck() ;
    else
      _pEmail->Check() ;
  }
  else
  {
    _pEmail->Uncheck() ;
  }

  // If unchecked and empty, reinitialize with mail
  //
  if ((_pEmail->GetCheck() != BF_CHECKED) && (string("") == _pUrl->GetText(COR_MESSAGERIE_LEN)))
  {
    PIDSTYPE iTypePids = pidsCorresp ;
    if ((pContexte->getPatient()) && (pCorrespSelect->getNss() == pContexte->getPatient()->getNss()))
      iTypePids = pidsPatient ;

    string sEmail = string("") ;
    pCorrespSelect->getPersonGraph()->trouveEMail(iTypePids, sEmail) ;
    _pUrl->SetText(sEmail) ;

    pPubliForP->_sUrl = sEmail ;
  }

  if (_pIntranet)  {    if (pPubliForP->wantIntranet())      _pIntranet->Check() ;
    else
      _pIntranet->Uncheck() ;
  }

  if (pPubliForP->wantHtmlExport())    _pHtml->Check() ;
  else
    _pHtml->Uncheck() ;

  if (pPubliForP->wantPdfExport())
    _pPdf->Check() ;
  else
    _pPdf->Uncheck() ;

  if (ExisteLettre(pCorrespSelect))  {
    _pLettre->Check() ;

    if (pPubliForP->wantLetter())      _pJoindre->Check() ;
    else
      _pJoindre->Uncheck() ;
  }
  else
  {
    _pLettre->Uncheck() ;
    _pJoindre->Uncheck() ;
    pPubliForP->_bLettre = false ;
  }
}

voidPubliCorrespDialog::AfficheAdresse(NSPersonInfo *pCorrespSelect){
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

	string sAdresse = string("") ;

	// on charge ici en mémoire le graphe de personne et le graphe d'adresse
	string sNss = pCorrespSelect->getNss() ;

	if (string("") != sNss)
	{
    string sLanguage = pContexte->getUserLanguage() ;

		if ((pContexte->getPatient()) && (sNss == pContexte->getPatient()->getNss()))
  		sAdresse = pCorrespSelect->getMainAdr(pidsPatient, sLanguage) ;
		else
			sAdresse = pCorrespSelect->getMainAdr(pidsCorresp, sLanguage) ;
	}

	_pAdresse->SetText(sAdresse.c_str()) ;
}

stringPubliCorrespDialog::CodeCorrespSelect()
{
  NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return string("") ;

	return string(pCorrespSelect->getszPersonID()) ;
}
// Fonction récupérant le code document de la lettre d'accompagnement// selon le code du correspondant sélectionné
/////////////////////////////////////////////////////////////////////

boolPubliCorrespDialog::CodeDocLettreSelect(string& sCode)
{
try
{
  sCode = string("") ;

	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return false ;

	sCode = _pPubli->getCodeLettre(pCorrespSelect->getNss()) ;

  return (string("") != sCode) ;
}
catch (...)
{
	erreur("Exception PubliCorrespDialog::CodeDocLettreSelect.", standardError, 0) ;
	return false ;
}
}

voidPubliCorrespDialog::CheckLettre()
{
  NSPersonInfo* pCorrespSelect = getSelectedPerson() ;

	if (ExisteLettre(pCorrespSelect))	{
		_pLettre->Check() ;

    NSChoixPubli* pPubliForP = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
    if (pPubliForP)
    {
      _pJoindre->Check() ;      pPubliForP->_bLettre = true ;
    }
  }
  else
  {
  	_pLettre->Uncheck() ;
    _pJoindre->Uncheck() ;
  }
}

voidPubliCorrespDialog::CmAjouterPatient()
{
try
{
	NSPatientChoisi* pPatChoisi = pContexte->getPatient() ;
  if (NULL == pPatChoisi)
		return ;

	int i ;
	bool bIsCheck = false ;

	// on vérifie s'il existe déjà un patient dans les correspondants  if (false == _pCorrespBaseArray->empty())		for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; j++)		{
  		if ((*j)->getNss() == pPatChoisi->getNss())
    	{
    		bIsCheck = true ;
      	break ;
    	}
  	}

	if (false == bIsCheck)	{
		// on rajoute le patient au tableau des correspondants
    NSPersonInfo PatInfo(pContexte->getSuperviseur(), pContexte, pPatChoisi->getNss(), pidsPatient) ;
		// on cherche l'indice du dernier corresp dans pPubli->aChoixPubli
    for (i = 0 ; i < MAXSELECT ; i++)
    	if (false == _pPubli->_aChoixPubli[i].isSelected())
    	// if (!strcmp(pPubli->aChoixPubli[i].corresp, ""))
      	break ;

    if (MAXSELECT == i)		{
    	erreur("Vous avez dépassé la limite des sélections possibles. Vous ne pourrez pas publier vers le patient.", standardError, 0, GetHandle()) ;
      return ;
    }

    // on DOIT ajouter le patient directement dans NSPubli    // à cause du fonctionnement de ChoixCorrespDialog
    _pCorrespBaseArray->push_back(new NSPersonInfo(PatInfo)) ;

    NSChoixPubli *pPatPubli = &(_pPubli->_aChoixPubli[i]) ;

    // Charge les valeurs par défaut des choix publication    pPatPubli->_sCorresp    = pPatChoisi->getNss() ;    pPatPubli->_bSelect     = true ;
    pPatPubli->_bImprimante = true ;
    strcpy(pPatPubli->_nb_expl, "1") ;
    pPatPubli->_iCpt        = 1 ;
    pPatPubli->_sUrl        = string("") ;
    pPatPubli->_bEmail      = false ;
    pPatPubli->_bIntranet   = false ;
    pPatPubli->_bHtml       = false ;
    pPatPubli->_bPdf        = false ;
    pPatPubli->_bLettre     = false ;

    _pPubliPatient->Check() ;	}
	else
	{
		// on enlève le patient du tableau des correspondants
    i = 0 ;

		if (false == _pCorrespBaseArray->empty())
			for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; i++,j++)
			{      	if ((*j)->getNss() == pPatChoisi->getNss())
        {
        	delete (*j) ;
          _pCorrespBaseArray->erase(j) ;

          NSChoixPubli *pPatPubli = &(_pPubli->_aChoixPubli[i]) ;

          pPatPubli->_sCorresp    = string("") ;          pPatPubli->_bSelect     = false ;
          pPatPubli->_bImprimante = false ;
          strcpy(pPatPubli->_nb_expl, "") ;
          pPatPubli->_iCpt        = 0 ;
          pPatPubli->_sUrl        = string("") ;
          pPatPubli->_bEmail      = false ;
          pPatPubli->_bIntranet   = false ;
          pPatPubli->_bHtml       = false ;
          pPatPubli->_bPdf        = false ;
          pPatPubli->_bLettre     = false ;
          break;
        }
      }

    _pPubliPatient->Uncheck() ;	}

	// on remet la liste à jour	AfficheCorresp() ;
}
catch (...)
{
	erreur("Exception PubliCorrespDialog::CmAjouterPatient.", standardError, 0) ;
}
}

void
PubliCorrespDialog::CmAjouterBlank()
{
try
{
	int i ;
	bool bIsCheck = false ;

	// on vérifie s'il existe déjà un "blanc" dans les correspondants  if (false == _pCorrespBaseArray->empty())		for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; j++)		{
  		if ('\0' == (*j)->getszPersonID()[0])
    	{
    		bIsCheck = true ;
      	break ;
    	}
  	}

	if (false == bIsCheck)	{
		// on cherche l'indice du dernier corresp dans pPubli->aChoixPubli
    for (i = 0; i < MAXSELECT; i++)
    	if (false == _pPubli->_aChoixPubli[i].isSelected())
      	break ;

    if (MAXSELECT == i)		{
    	erreur("Vous avez dépassé la limite des sélections possibles.", standardError, 0, GetHandle()) ;
      return ;
    }

    // on DOIT ajouter le "blanc" directement dans NSPubli    // à cause du fonctionnement de ChoixCorrespDialog
    _pCorrespBaseArray->push_back(new NSPersonInfo(pContexte->getSuperviseur())) ;

    NSChoixPubli *pBlankPubli = &(_pPubli->_aChoixPubli[i]) ;

    // Charge les valeurs par défaut des choix publication    pBlankPubli->_sCorresp    = string("") ;    pBlankPubli->_bSelect     = true ;
    pBlankPubli->_bImprimante = true ;
    strcpy(pBlankPubli->_nb_expl, "1") ;
    pBlankPubli->_iCpt        = 1 ;
    pBlankPubli->_sUrl        = string("") ;
    pBlankPubli->_bEmail      = false ;
    pBlankPubli->_bIntranet   = false ;
    pBlankPubli->_bHtml       = false ;
    pBlankPubli->_bPdf        = false ;
    pBlankPubli->_bLettre     = false ;

    _pPubliBlank->Check() ;	}
	else
	{
		// on enlève le "blanc" du tableau des correspondants
    i = 0 ;

		if (false == _pCorrespBaseArray->empty())
			for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; i++, j++)
			{      	if ('\0' == (*j)->getszPersonID()[0])
        {
        	delete (*j) ;
          _pCorrespBaseArray->erase(j) ;

          NSChoixPubli *pBlankPubli = &(_pPubli->_aChoixPubli[i]) ;

          pBlankPubli->_sCorresp    = string("") ;          pBlankPubli->_bSelect     = false ;
          pBlankPubli->_bImprimante = false ;
          strcpy(pBlankPubli->_nb_expl, "") ;
          pBlankPubli->_iCpt        = 0 ;
          pBlankPubli->_sUrl        = string("") ;
          pBlankPubli->_bEmail      = false ;
          pBlankPubli->_bIntranet   = false ;
          pBlankPubli->_bHtml       = false ;
          pBlankPubli->_bPdf        = false ;
          pBlankPubli->_bLettre     = false ;
          break ;
        }
      }

    _pPubliBlank->Uncheck() ;	}

	// on remet la liste à jour	AfficheCorresp() ;
}
catch (...)
{
	erreur("Exception PubliCorrespDialog::CmAjouterBlank.", standardError, 0) ;
}
}

voidPubliCorrespDialog::CmSelectCorresp(WPARAM Cmd)
{
  NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

  AfficheAdresse(pCorrespSelect) ;  ResetChoixPubli() ;  AfficheChoixPubli(pCorrespSelect) ;

  pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
}

voidPubliCorrespDialog::CmCorrespDblClk(WPARAM Cmd)
{
}

voidPubliCorrespDialog::CmClickImprimante()
{
  NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantToPrint())  {
    pSelectedPubli->_bImprimante = false ;
    strcpy(pSelectedPubli->_nb_expl, "") ;
    pSelectedPubli->_iCpt = 0 ;
  }
  else
  {
    pSelectedPubli->_bImprimante = true ;
    strcpy(pSelectedPubli->_nb_expl, "1") ;
    pSelectedPubli->_iCpt = 1 ;
  }

	AfficheChoixPubli(pCorrespSelect) ;}

voidPubliCorrespDialog::CmClickEmail()
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantMail())    pSelectedPubli->_bEmail = false ;
  else
    pSelectedPubli->_bEmail = true ;

	AfficheChoixPubli(pCorrespSelect) ;}

voidPubliCorrespDialog::CmClickIntranet()
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantIntranet())
    pSelectedPubli->_bIntranet = false ;
  else
    pSelectedPubli->_bIntranet = true ;

	AfficheChoixPubli(pCorrespSelect) ;}

voidPubliCorrespDialog::CmClickHtml()
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantHtmlExport())
    pSelectedPubli->_bHtml = false ;
  else
    pSelectedPubli->_bHtml = true ;

	AfficheChoixPubli(pCorrespSelect) ;}

void
PubliCorrespDialog::CmClickPdf()
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantPdfExport())
    pSelectedPubli->_bPdf = false ;
  else
    pSelectedPubli->_bPdf = true ;

	AfficheChoixPubli(pCorrespSelect) ;}

voidPubliCorrespDialog::CmClickJoindre()
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

  NSChoixPubli* pSelectedPubli = _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
  if ((NSChoixPubli*) NULL == pSelectedPubli)
    return ;

  if (pSelectedPubli->wantLetter())
    pSelectedPubli->_bLettre = false ;
  else
    pSelectedPubli->_bLettre = true ;

	AfficheChoixPubli(pCorrespSelect) ;}

voidPubliCorrespDialog::CmLettreType()
{
try
{
	NSPersonInfo* pCorrespSelect = getSelectedPerson() ;
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return ;

	if (false == ExisteLettre(pCorrespSelect))
	{
		if ((pContexte->getPatient()) && (true == pContexte->getPatient()->getGraphPerson()->getReadOnly()))
		{
			_pLettre->Uncheck() ;
			string sErrorText = pContexte->getSuperviseur()->getText("privilegeManagement", "patientFileIsReadOnly") ;
			erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return ;
		}

		LettreTypeDialog *pLTDlg = new LettreTypeDialog(this, pContexte) ;

		if (pLTDlg->Execute() == IDOK)
		{
      string sTexte = string("") ;

			if (pLTDlg->existsIntro())
				sTexte += pLTDlg->getIntroText() + string("\n\n") ;

			if (pLTDlg->existsBody())
				sTexte += pLTDlg->getBodyText() + string("\n\n") ;

			if (pLTDlg->existsPolite())
				sTexte += pLTDlg->getPoliteText() + string("\n\n") ;

			// on crée la forme lettre type
			if (_pLettreType)
				delete _pLettreType ;
			_pLettreType = new TLettreType((TComponent*) 0, this, _pPubli->_pDocMaitre) ;
			_pLettreType->Show() ;
			_pLettreType->SetText(sTexte) ;
		}
		else
			_pLettre->Uncheck() ;

		delete pLTDlg ;
	}
	else
	{
		// si le patient est en lecture seule, on autorise la visualisation de la lettre
		if (_pLettreType)
			delete _pLettreType ;
		_pLettreType = new TLettreType(NULL, this, _pPubli->_pDocMaitre) ;
		_pLettreType->Show() ;
    if (pContexte->getPatient())
			_pLettreType->OuvreLettre(pContexte->getPatient()->getReadOnly()) ;
	}
}
catch (...)
{
	erreur("Exception PubliCorrespDialog::CmLettreType().", standardError, 0) ;
}
}

voidPubliCorrespDialog::CmOk()
{
	if (_pLettreType)
	{
		_pLettreType->Close() ;
		delete _pLettreType ;
	}

	TDialog::CmOk() ;}

voidPubliCorrespDialog::CmCancel()
{
	if (_pLettreType)
	{
		_pLettreType->Close() ;
		delete _pLettreType ;
	}

	TDialog::CmCancel() ;}

NSPersonInfo*
PubliCorrespDialog::getSelectedPerson()
{
  if (_aCorrespArray.empty())
		return (NSChoixPubli*) 0 ;

  _CorrespChoisi = _pCorrespBox->GetSelIndex() ;

	return _aCorrespArray[_CorrespChoisi] ;
}

NSChoixPubli*
PubliCorrespDialog::getSelectedPubli()
{
  NSPersonInfo* pCorrespSelect = getSelectedPerson() ;

	if ((NSPersonInfo*) NULL == pCorrespSelect)
    return (NSChoixPubli*) 0 ;

  return _pPubli->getChoixPubliForPerson(pCorrespSelect) ;
}
// -----------------------------------------------------------------//
//  Méthodes de PubliSansCorrespDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(PubliSansCorrespDialog, NSUtilDialog)	EV_BN_CLICKED(IDC_PSC_IMPRIMANTE, CmClickImprimante),
	EV_BN_CLICKED(IDC_PSC_EMAIL,      CmClickEmail),
	EV_BN_CLICKED(IDC_PSC_INTRANET,   CmClickIntranet),
	EV_BN_CLICKED(IDC_PSC_HTML,       CmClickHtml),
  EV_BN_CLICKED(IDC_PSC_PDF,        CmClickPdf),
	EV_BN_CLICKED(IDC_PSC_PATIENT,    CmPublierVersPatient),
	// EV_BN_CLICKED(IDC_PSC_LETTRE, CmClickLettre),
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

PubliSansCorrespDialog::PubliSansCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication)                       :NSUtilDialog(pere, pCtx, "IDD_PUBLISANSCORRESP")
{
try
{
	// pNbExpl       = new NSUtilEdit(this,        IDC_PSC_NBEXPL, WF_NB_PUBLI_LEN);
  _pNbExpl       = new NSUtilEdit(pContexte, this, IDC_PSC_NBEXPL, 2) ;
	_pUrl          = new NSUtilEdit(pContexte, this, IDC_PSC_URL,    COR_MESSAGERIE_LEN) ;
	_pGroupe       = new OWL::TGroupBox(this,    IDC_PSC_GROUPE) ;
	_pImprimante   = new OWL::TRadioButton(this, IDC_PSC_IMPRIMANTE, _pGroupe) ;
	_pEmail        = new OWL::TRadioButton(this, IDC_PSC_EMAIL,      _pGroupe) ;
	// _pIntranet     = new OWL::TRadioButton(this, IDC_PSC_INTRANET,   pGroupe) ;
  _pIntranet     = (OWL::TRadioButton*) 0 ;
	_pHtml         = new OWL::TRadioButton(this, IDC_PSC_HTML,       _pGroupe) ;
  _pPdf          = new OWL::TRadioButton(this, IDC_PSC_PDF,        _pGroupe) ;
	_pGroupePat    = new OWL::TGroupBox(this,    IDC_PSC_GROUPEPAT) ;
	_pPubliPatient = new OWL::TRadioButton(this, IDC_PSC_PATIENT,    _pGroupePat) ;
	// pLettre = 		new TCheckBox(this,IDC_PSC_LETTRE);
	_pPubli = pPublication ;

	pContexte->setAideIndex("hi_doc.htm") ;	pContexte->setAideCorps("h_publi.htm#Phase2b") ;
}
catch (...)
{
	erreur("Exception PubliSansCorrespDialog ctor.", standardError, 0) ;
}
}

PubliSansCorrespDialog::~PubliSansCorrespDialog(){
	// delete pLettre;
  delete _pPubliPatient ;
  delete _pHtml ;
  delete _pPdf ;
  if (_pIntranet)
    delete _pIntranet ;
  delete _pEmail ;
  delete _pImprimante ;
  delete _pUrl ;
  delete _pNbExpl ;

  _pPubli = 0 ;					  // ne pas deleter}

voidPubliSansCorrespDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::AfficheChoixPubli()
{
  NSChoixPubli *pChoixPubli = &(_pPubli->_choixPubli) ;

	if (pChoixPubli->wantToPrint())
  {
    _pImprimante->Check() ;
    _pNbExpl->SetText(pChoixPubli->_nb_expl) ;
  }
  else
  {
    _pImprimante->Uncheck() ;
    _pNbExpl->SetText("") ;
  }

  if (pChoixPubli->wantMail())  {
    _pEmail->Check() ;
    _pUrl->SetText(pChoixPubli->_sUrl.c_str()) ;
  }
  else
  {
    _pEmail->Uncheck() ;
    _pUrl->SetText("") ;
  }

  if (_pIntranet)  {    if (pChoixPubli->wantIntranet())      _pIntranet->Check() ;
    else
      _pIntranet->Uncheck() ;
  }

  if (pChoixPubli->wantHtmlExport())    _pHtml->Check() ;
  else
    _pHtml->Uncheck() ;

  if (pChoixPubli->wantPdfExport())
    _pPdf->Check() ;
  else
    _pPdf->Uncheck() ;
}

voidPubliSansCorrespDialog::CmPublierVersPatient()
{
	// on met un corresp fictif pour Imprimer()
	if (pContexte->getPatient() && (string("") == _pPubli->_choixPubli._sCorresp))
	{
  	_pPubliPatient->Check() ;
    _pPubli->_choixPubli._sCorresp = pContexte->getPatient()->getNss() ;

    if (_pEmail->GetCheck() == BF_CHECKED)    {
      string sUrl = _pUrl->GetText(COR_MESSAGERIE_LEN) ;

      // si dans ce cas il n'y a pas d'url, on propose celle du patient      //      if (string("") == sUrl)
      	_pUrl->SetText(_pPubli->_choixPubli._sUrl.c_str()) ;
    }
	}
	else
	{
  	_pPubli->_choixPubli._sCorresp = string("") ;
    _pPubliPatient->Uncheck() ;
	}
}

voidPubliSansCorrespDialog::CmClickImprimante()
{
  if (_pPubli->_choixPubli.wantToPrint())
  {
    _pPubli->_choixPubli._bImprimante = false ;
    strcpy(_pPubli->_choixPubli._nb_expl, "") ;
    _pPubli->_choixPubli._iCpt = 0 ;
  }
  else
  {
    _pPubli->_choixPubli._bImprimante = true ;
    strcpy(_pPubli->_choixPubli._nb_expl, "1") ;
    _pPubli->_choixPubli._iCpt = 1 ;
  }

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::CmClickEmail()
{
	if (_pPubli->_choixPubli.wantMail())
  {
    _pPubli->_choixPubli._bEmail = false ;
    _pPubli->_choixPubli._sUrl   = string("") ;
  }
  else
    _pPubli->_choixPubli._bEmail = true ;

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::CmClickIntranet()
{
	if (_pPubli->_choixPubli.wantIntranet())
    _pPubli->_choixPubli._bIntranet = false ;
  else
    _pPubli->_choixPubli._bIntranet = true ;

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::CmClickHtml()
{
  if (_pPubli->_choixPubli.wantHtmlExport())
    _pPubli->_choixPubli._bHtml = false ;
  else
    _pPubli->_choixPubli._bHtml = true ;

  AfficheChoixPubli() ;}

void
PubliSansCorrespDialog::CmClickPdf()
{
  if (_pPubli->_choixPubli.wantPdfExport())
    _pPubli->_choixPubli._bPdf = false ;
  else
    _pPubli->_choixPubli._bPdf = true ;

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::CmClickLettre()
{
	if (_pPubli->_choixPubli.wantLetter())
    _pPubli->_choixPubli._bLettre = false ;
  else
    _pPubli->_choixPubli._bLettre = true ;

  AfficheChoixPubli() ;}

voidPubliSansCorrespDialog::CmOk()
{
	// char far nb_expl[WF_NB_PUBLI_LEN + 1];
  char far nb_expl[3] ;

	// pNbExpl->GetText(nb_expl, WF_NB_PUBLI_LEN + 1) ;  string sNbExpl = _pNbExpl->GetText(2) ;
	strcpy(_pPubli->_choixPubli._nb_expl, sNbExpl.c_str()) ;	_pPubli->_choixPubli._iCpt = atoi(_pPubli->_choixPubli._nb_expl) ;

  string sUrl = _pUrl->GetText(COR_MESSAGERIE_LEN) ;
	_pPubli->_choixPubli._sUrl = sUrl ;

	TDialog::CmOk() ;}

voidPubliSansCorrespDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------//
//  Méthodes de ChoixCorrespDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixCorrespDialog, NSUtilDialog)	EV_CHILD_NOTIFY_AND_CODE(IDC_BASEBOX, LBN_SELCHANGE, CmSelectCorrespBase),
	EV_CHILD_NOTIFY_AND_CODE(IDC_BASEBOX, LBN_DBLCLK, CmCorrespBaseDblClk),
    EV_BN_CLICKED(IDC_AJOUTER_PATIENT, CmAjouterPatient),
    EV_COMMAND(IDC_SELECTALL, CmSelectAll),
   	EV_COMMAND(IDOK,CmOk),
   	EV_COMMAND(IDCANCEL,CmCancel),
END_RESPONSE_TABLE;

ChoixCorrespDialog::ChoixCorrespDialog(TWindow* pere, NSContexte* pCtx, NSPublication* pPublication)                   :NSUtilDialog(pere, pCtx, "IDD_SELECTCORR")
{
try
{
	_pPubli = pPublication ;

	_pCorrespBaseBox 	 = new OWL::TListBox(this,     IDC_BASEBOX) ;	_pCorrespSelectBox = new OWL::TListBox(this,     IDC_SELECTBOX) ;
	_pAjouterPatient   = new OWL::TRadioButton(this, IDC_AJOUTER_PATIENT, 0) ;

	_pCorrespBaseArray = &(_pPubli->_aCorrespBaseArray) ;
	pContexte->getSuperviseur()->setAideIndex("hi_doc.htm") ;	pContexte->getSuperviseur()->setAideCorps("h_publi.htm#Phase1") ;
}
catch (...)
{
	erreur("Exception ChoixCorrespDialog ctor.", standardError, 0) ;
}
}

ChoixCorrespDialog::~ChoixCorrespDialog(){
	delete _pCorrespBaseBox ;
	delete _pCorrespSelectBox ;
	delete _pAjouterPatient ;

	_pCorrespBaseArray = 0 ;}

voidChoixCorrespDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	_pAjouterPatient->Uncheck() ;
	if ((pContexte->getPatient()) && (false == _pCorrespBaseArray->empty()))	{
		for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; j++)
		{
    	if (pContexte->getPatient()->getNss() == (*j)->getNss())      {
      	// Il existe déjà un patient
        _pAjouterPatient->Check() ;
        break ;
      }
    }
  }

	SetSelectBase() ;	AfficheSelect() ;
}

voidChoixCorrespDialog::SetSelectBase()
{
	// on vide la liste si elle contient des items
	if (_pCorrespBaseBox->GetCount())
		_pCorrespBaseBox->ClearList() ;

	//	// Mise à jour des sélections de CorrespBaseBox
	//
	if (_pCorrespBaseArray->empty())
		return ;
	int CorrespBaseIndex[MAXSELECT] ;
	int numSelect = 0 ;

	for (NSPersonIter it = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != it ; it++)
	{
		// on remplit la CorrespBox
    string sCorresp = (*it)->getNom() + string(" ") + (*it)->getPrenom() ;
    _pCorrespBaseBox->AddString(sCorresp.c_str()) ;

    int iPubliIndex = getPubliIndexFromPersonID((*it)->getNss()) ;
		if ((iPubliIndex >= 0) && _pPubli->_aChoixPubli[iPubliIndex]._bSelect)
  	{
    	CorrespBaseIndex[numSelect] = iPubliIndex ;
      numSelect++ ;
    }
  }

	_pCorrespBaseBox->SetSelIndexes(CorrespBaseIndex, numSelect, true) ;}

voidChoixCorrespDialog::AfficheSelect()
{
	// on vide la liste si elle contient des items
  // if the list isn't empty, we empty it
  //
	if (_pCorrespSelectBox->GetCount())
		_pCorrespSelectBox->ClearList() ;

	// Récupération des indices des correspondants sélectionnés  // Getting indexes of selected persons	//
  int CorrespBaseIndex[MAXSELECT] ;
	int numSelect = _pCorrespBaseBox->GetSelIndexes(CorrespBaseIndex, MAXSELECT) ;

	for (int i = 0 ; i < numSelect ; i++)	{
  	int index = CorrespBaseIndex[i] ;
    NSPersonInfo* pCorrespInfoChoisi = (*(_pCorrespBaseArray))[index] ;    string sCorresp = pCorrespInfoChoisi->getNom() + string(" ") + pCorrespInfoChoisi->getPrenom() ;
    _pCorrespSelectBox->AddString(sCorresp.c_str()) ;

    int iPubliIndex = getPubliIndexFromPersonID(pCorrespInfoChoisi->getNss()) ;
    if (iPubliIndex >= 0)
    	_pPubli->_aChoixPubli[iPubliIndex]._bSelect = true ;
	}
}

voidChoixCorrespDialog::CmSelectCorrespBase(WPARAM Cmd)
{
	AfficheSelect() ;
}

voidChoixCorrespDialog::CmCorrespBaseDblClk(WPARAM Cmd)
{
}

voidChoixCorrespDialog::CmAjouterPatient()
{
try
{
	int i ;
	NSPatientChoisi* pPatChoisi = pContexte->getPatient() ;
  if (NULL == pPatChoisi)
		return ;

	bool bIsCheck = false ;

	// on vérifie s'il existe déjà un patient dans les correspondants	if (false == _pCorrespBaseArray->empty())
	{
		for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; j++)
		{			if (pPatChoisi->getNss() == (*j)->getNss())
      {      	bIsCheck = true ;
        break ;
      }
    }
	}

	if (false == bIsCheck)	{
		// on rajoute le patient au tableau des correspondants
		NSPersonInfo PatInfo(pContexte->getSuperviseur(), pContexte, pPatChoisi->getNss(), pidsPatient) ;
		// on cherche l'indice du dernier corresp dans pPubli->aChoixPubli
    for (i = 0; i < MAXSELECT; i++)
    	if (string("") == _pPubli->_aChoixPubli[i]._sCorresp)
      	break ;

    if (MAXSELECT == i)    {
    	erreur("Vous avez dépassé la limite des sélections possibles. Vous ne pourrez pas publier vers le patient.", standardError, 0, GetHandle());
      return ;
    }

    // on DOIT ajouter le patient directement dans NSPubli    // à cause du fonctionnement de ChoixCorrespDialog

    _pCorrespBaseArray->push_back(new NSPersonInfo(PatInfo)) ;

    NSChoixPubli *pNewPubli = &(_pPubli->_aChoixPubli[i]) ;

		// Charge les valeurs par défaut des choix publication		pNewPubli->_sCorresp = pPatChoisi->getNss() ;
    pNewPubli->_bSelect     = true ;    pNewPubli->_bImprimante = true ;
    strcpy(pNewPubli->_nb_expl, "1") ;
    pNewPubli->_iCpt        = 1 ;
    pNewPubli->_sUrl        = string("") ;
    pNewPubli->_bEmail      = false ;
    pNewPubli->_bIntranet   = false ;
    pNewPubli->_bHtml       = false ;
    pNewPubli->_bPdf        = false ;
    pNewPubli->_bLettre     = false ;

    _pAjouterPatient->Check() ;	}
  else
	{
  	// on enlève le patient du tableau des correspondants
		for (NSPersonIter j = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != j ; j++)
		{			if (pPatChoisi->getNss() == (*j)->getNss())
      {
        delete (*j) ;
        _pCorrespBaseArray->erase(j) ;

        int iPubliIndex = getPubliIndexFromPersonID(pPatChoisi->getNss()) ;
    		if (iPubliIndex >= 0)
				{
          NSChoixPubli *pNewPubli = &(_pPubli->_aChoixPubli[i]) ;

        	pNewPubli->_sCorresp    = string("") ;        	pNewPubli->_bSelect     = false ;
        	pNewPubli->_bImprimante = false ;
        	strcpy(pNewPubli->_nb_expl, "") ;
        	pNewPubli->_iCpt        = 0 ;
        	pNewPubli->_sUrl        = string("") ;
        	pNewPubli->_bEmail      = false ;
        	pNewPubli->_bIntranet   = false ;
        	pNewPubli->_bHtml       = false ;
          pNewPubli->_bPdf        = false ;
        	pNewPubli->_bLettre     = false ;
        }
        break ;
      }
    }

    _pAjouterPatient->Uncheck() ;  }

	// on remet la liste à jour	SetSelectBase() ;
	AfficheSelect() ;
}
catch (...)
{
	erreur("Exception ChoixCorrespDialog::CmAjouterPatient", standardError, 0) ;
}
}

voidChoixCorrespDialog::CmSelectAll()
{
	int CorrespBaseIndex[MAXSELECT] ;
	int numSelect = 0 ;

	// on vide la liste si elle contient des items	if (_pCorrespSelectBox->GetCount())
		_pCorrespSelectBox->ClearList() ;

	if (false == _pCorrespBaseArray->empty())	{
		for (NSPersonIter i = _pCorrespBaseArray->begin() ; _pCorrespBaseArray->end() != i ; i++)
		{    	NSPersonInfo* pCorrespInfoChoisi = (*i) ;
      string sCorresp = pCorrespInfoChoisi->getNom() + string(" ") + pCorrespInfoChoisi->getPrenom() ;
      _pCorrespSelectBox->AddString(sCorresp.c_str()) ;
      CorrespBaseIndex[numSelect] = numSelect ;
      numSelect++ ;
    }
  }

	_pCorrespBaseBox->SetSelIndexes(CorrespBaseIndex, numSelect, true) ;
	CmOk() ;}

voidChoixCorrespDialog::CmOk()
{
	int index, numSelect, i ;
	int CorrespBaseIndex[MAXSELECT] ;

	// on efface l'ancienne sélection	for (i = 0; i < MAXSELECT; i++)
		_pPubli->_aChoixPubli[i]._bSelect = false ;

	// Récupération des indices des correspondants sélectionnés	//
	numSelect = _pCorrespBaseBox->GetSelIndexes(CorrespBaseIndex, MAXSELECT) ;

	if (0 == numSelect)	{
		erreur("Vous devez sélectionner au moins un correspondant.", warningError, 0, GetHandle()) ;
		return ;
	}

	// mise a jour du tableau de NSPublication	for (i = 0; i < numSelect; i++)
	{
		index = CorrespBaseIndex[i] ;

    NSPersonInfo* pCorrespInfoChoisi = (*(_pCorrespBaseArray))[index] ;

    int iPubliIndex = getPubliIndexFromPersonID(pCorrespInfoChoisi->getNss()) ;
    if (iPubliIndex >= 0)
			_pPubli->_aChoixPubli[iPubliIndex]._bSelect = true ;
	}

	TDialog::CmOk() ;}

voidChoixCorrespDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

int
ChoixCorrespDialog::getPubliIndexFromPersonID(string sPersonID)
{
  for (int i = 0 ; i < MAXSELECT ; i++)
		if (_pPubli->_aChoixPubli[i]._sCorresp == sPersonID)
    	return i ;

	return -1 ;
}

/****************** Hook Procedure ********************************//* Appelée après SendMessage sur le message WH_CALLWNDPROCRET     */
/******************************************************************/

LRESULT WINAPI CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  char cName[30] ;
  string sName ;
  LPCWPRETSTRUCT pMsg ;

  if (nCode < 0) // do not process msg
    return CallNextHookEx(hhook, nCode, wParam, lParam) ;

  switch (nCode)  {
    case HC_ACTION:
    {
      pMsg = (LPCWPRETSTRUCT) lParam ;

      switch (pMsg->message)
      {
        // Attention le seul evenement utilisable est WM_PAINT
        // WM_CREATE arrive trop tot et fait planter le serveur

        case WM_PAINT:

          // On récupère le nom de la classe de fenetre
          if (GetClassName(pMsg->hwnd, cName, 30))
            sName = string(cName) ;
          else
            sName = "" ;

          // La fenetre d'impression et la fenetre d'annulation
          // qui est créee quand on envoie un Return ont toutes
          // les deux le meme nom de classe #32770.
          // On est donc obligé d'envoyer les Return un coup sur
          // deux pour ne pas annuler l'impression.

          if (sName == string("#32770"))
          {
            if (bEnvoyerReturn)
            {
              keybd_event(VK_RETURN, 0, standardError, 0) ;
              keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0) ;
              bEnvoyerReturn = false ;
            }
            else
              bEnvoyerReturn = true ;
          }
          break ;
      }
    }
    break ;

    default:
      break ;
  }
  return CallNextHookEx(hhook, nCode, wParam, lParam) ;
}

NSLdvPubli::NSLdvPubli()
{
  _iPrintType    = printNotSpecified ;
  _pPrintArray   = 0 ;
  _sTemplateFile = string("") ;
  _sHeaderFile   = string("") ;

  lObjectCount++ ;
}

NSLdvPubli::NSLdvPubli(const NSLdvPubli& rv)
{
  _iPrintType    = rv._iPrintType ;
  _pPrintArray   = rv._pPrintArray ;
  _sTemplateFile = rv._sTemplateFile ;
  _sHeaderFile   = rv._sHeaderFile ;

  lObjectCount++ ;
}

NSLdvPubli&
NSLdvPubli::operator=(const NSLdvPubli& src)
{
  if (&src == this)
		return *this ;

  _iPrintType    = src._iPrintType ;
  _pPrintArray   = src._pPrintArray ;
  _sTemplateFile = src._sTemplateFile ;
  _sHeaderFile   = src._sHeaderFile ;

  return *this ;
}

// -----------------------------------------------------------------//
//  Méthodes de NSPublication
//
// -----------------------------------------------------------------

NSPublication::NSPublication(NSPatientChoisi* pPatientChoisi,
                             NSRefDocument* pDocEnCours,
                             NSContexte* pCtx)
              :NSRoot(pCtx), _aCorrespBaseArray(pContexte->getSuperviseur())
{
  lObjectCount++ ;

	// Patient en cours
	_pPatChoisi          = pPatientChoisi ;
	_pDocMaitre          = pDocEnCours ;
  _pDocMaitreLdv       = 0 ;
	_bImpressionEnCours  = false ;

	// Paramètres de tempo.dat	_bUseHook            = false ;
	_bWaitImp            = false ;
	_iTempImp            = 0 ;  //  _pXSMTP              = (TIndySMTP*) 0 ;  _pMapiSMTP           = (NSMapiCtrl*) 0 ;  _pLdvPubliDriver    = (NSLdvPubli*) 0 ;
  // NSChoixPubli has no constructor... so let's make sure there is some initialization
	for (int i = 0 ; i < MAXSELECT ; i++)
	{
  	_aChoixPubli[i]._sCorresp = string("") ;
    _aChoixPubli[i]._bSelect  = false ;
  }

	initParams() ;  _fichTmpl   = string("") ;  _enteteTmpl = string("") ;
	if ((_pDocMaitre) && (_pDocMaitre->_pDocInfo))	{
  	_sCodeDoc = _pDocMaitre->getDocId() ;

    if (string("") == _sCodeDoc)    	_sCodeDoc = string("temp") ;

    if (_pDocMaitre->_pHtmlInfo) // cas composition deja effectuee    {
    	_fichTmpl   = _pDocMaitre->TemplateInfoBrut() ;
      _enteteTmpl = _pDocMaitre->EnTeteInfoBrut() ;
    }
    else // on est en presence d'un document brut
    {
      // First, we get document's presentation information
      //
      if (pContexte->getPatient() && pContexte->getPatient()->getGraphPerson())
        pContexte->getPatient()->getGraphPerson()->getTemplatePres(_pDocMaitre->_pDocInfo->getCodeDocPres(), _fichTmpl, _enteteTmpl) ;

      // If not found, use default templates for this kind of document
      //
      if ((string("") == _fichTmpl) || (string("") == _enteteTmpl))
      {
        // on récupère la template de composition par défaut du type document
    	  string sTypeDoc = string(_pDocMaitre->_pDocInfo->getTypeSem()) ;

        if (pContexte->typeDocument(sTypeDoc, NSSuper::isTree))
        {
      	  // s'il existe une template et un en-tete non vides
          // on les récupère, sinon on prend les fichiers par défaut

          _fichTmpl   = _pDocMaitre->TemplateInfoBrut(false) ;
          _enteteTmpl = _pDocMaitre->EnTeteInfoBrut(false) ;

          if ((string("") == _fichTmpl) || (string("") == _enteteTmpl))
          {        	  string sType = _pDocMaitre->_pDocInfo->getTypeSem() ;            _pDocMaitre->TemplateCompo(sType, _fichTmpl, _enteteTmpl) ;
          }
        }
        else if (string("ZTPDF") != sTypeDoc)
      	  _pDocMaitre->TemplateCompo(sTypeDoc, _fichTmpl, _enteteTmpl) ;
      }
    }
  }
  else
  	_sCodeDoc = string("temp") ;

	_pCrypteMsg = 0 ;	_pCrypteFic = 0 ;
}

NSPublication::NSPublication(NSPatientChoisi* pPatientChoisi,                             NSLdvDocument* pDocEnCours, NSLdvPubli *pDriver,
                             NSContexte* pCtx)
              :NSRoot(pCtx), _aCorrespBaseArray(pContexte->getSuperviseur())
{
  lObjectCount++ ;

	// Patient en cours
	_pPatChoisi          = pPatientChoisi ;
	_pDocMaitre          = 0 ;
  _pDocMaitreLdv       = pDocEnCours ;
	_bImpressionEnCours  = false ;

	// Paramètres de tempo.dat	_bUseHook            = false ;
	_bWaitImp            = false ;
	_iTempImp            = 0 ;  _pLdvPubliDriver     = new NSLdvPubli(*pDriver) ;  //  _pXSMTP              = (TIndySMTP*) 0 ;  _pMapiSMTP           = (NSMapiCtrl*) 0 ;
  // NSChoixPubli has no constructor... so let's make sure there is some initialization
	for (int i = 0; i < MAXSELECT; i++)
	{
  	_aChoixPubli[i]._sCorresp = string("") ;
    _aChoixPubli[i]._bSelect  = false ;
  }

	initParams() ;
  _pCrypteMsg = 0 ;
	_pCrypteFic = 0 ;

  _sCodeDoc   = string("temp") ;
  _fichTmpl   = string("") ;
  _enteteTmpl = string("") ;

  // on récupère la template de composition par défaut du type document  //  string sTypeDoc = string("") ;  switch (pDriver->_iPrintType)  {    case NSLdvPubli::printGoals :      sTypeDoc = string("ZLVGO1") ;      break ;    case NSLdvPubli::printDrugs :      sTypeDoc = string("ZLVTR1") ;      break ;  }  if (string("") == sTypeDoc)    return ;  _pDocMaitreLdv->TemplateCompo(sTypeDoc, _fichTmpl, _enteteTmpl) ;

  _pLdvPubliDriver->_sTemplateFile = _fichTmpl ;
  _pLdvPubliDriver->_sHeaderFile   = _enteteTmpl ;
}
NSPublication::~NSPublication()
{
  lObjectCount-- ;

	// !!! Ne pas deleter ces deux pointeurs
	// (ce qui tuerait le patient et le document)

	_pPatChoisi = 0 ;	_pDocMaitre = 0 ;

	// On ne delete plus les fichiers de publication	// Ce travail est fait directement dans ~NSVisualView
	// DeleteFichPubli();

  if (_pLdvPubliDriver)
    delete _pLdvPubliDriver ;
}

stringNSPublication::tempPubli(char *typePub)
{
	// par defaut pour l'instant :
	// retourne la template de composition
	return _fichTmpl ;
}

stringNSPublication::entetePubli(char *typePub)
{
	// par defaut pour l'instant :
	// retourne l'en-tete de composition
	return _enteteTmpl ;
}

stringNSPublication::dateSysteme()
{
	char cDate[9] ;
	struct date dateSys ;

	getdate(&dateSys) ;	sprintf(cDate, "%4d%02d%02d", dateSys.da_year, dateSys.da_mon, dateSys.da_day) ;
	return string(cDate) ;
}

stringNSPublication::heureSysteme()
{
	char cHeure[7] ;
	struct time heureSys ;

	gettime(&heureSys) ;	sprintf(cHeure, "%02d%02d%02d", heureSys.ti_hour, heureSys.ti_min, heureSys.ti_sec) ;
	return string(cHeure) ;
}

boolNSPublication::ChargeCorrespBase()
{
try
{
	int j = 0 ;
	_aCorrespBaseArray.vider() ;

	// Si on ne passe pas de patient, prend un array vide	if (_pPatChoisi && _pPatChoisi->getHealthTeam())
	{
  	VecteurString* pMembersList = _pPatChoisi->getHealthTeam()->getPIDS() ;
		for (EquiItemIter i = pMembersList->begin() ; pMembersList->end() != i ; i++)
    {
      PIDSTYPE iTypePids ;
      if ((pContexte->getPatient()) && (!strcmp((*i)->c_str(), pContexte->getPatient()->getszNss())))
        iTypePids = pidsPatient ;
      else
        iTypePids = pidsCorresp ;

    	NSPersonInfo* pPersonInfo = pContexte->getPersonArray()->getPerson(pContexte, *(*i), iTypePids) ;
      _aCorrespBaseArray.push_back(new NSPersonInfo(*pPersonInfo)) ;
    	// aCorrespBaseArray.push_back(new NSPersonInfo(pContexte, *(*i), pidsCorresp)) ;
    }
	}

	for (int k = 0 ; k < MAXSELECT ; k++)
	{
		_sFichHtml[k]   = string("") ;
		_sLettreHtml[k] = string("") ;
	}

	if (false == _aCorrespBaseArray.empty())	{
		for (NSPersonIter i = _aCorrespBaseArray.begin() ; _aCorrespBaseArray.end() != i ; i++,j++)
		{    	if (j < MAXSELECT)
			{
      	// Charge les valeurs par défaut des choix publication
        _aChoixPubli[j]._sCorresp    = (*i)->getszPersonID() ;
				_aChoixPubli[j]._bSelect     = false ;        _aChoixPubli[j]._bImprimante = true ;

        // initialisation du nombre d'exemplaires        strcpy(_aChoixPubli[j]._nb_expl, "1") ;
        _aChoixPubli[j]._iCpt        = 1 ;
        _aChoixPubli[j]._sUrl        = string("") ;

        _aChoixPubli[j]._bEmail      = false ;
        _aChoixPubli[j]._bIntranet   = false ;
        _aChoixPubli[j]._bHtml       = false ;
        _aChoixPubli[j]._bPdf        = false ;
        _aChoixPubli[j]._bLettre     = false ;
      }
      else
    	{
      	erreur("Le nombre des correspondants dépasse le maximum des sélectionnés", standardError, 0, pContexte->GetMainWindow()->GetHandle());
        return false ;
      }
    }
  }

	for (int i = j ; i < MAXSELECT ; i++)	{
    _aChoixPubli[i]._sCorresp    = string("") ;
    _aChoixPubli[i]._bSelect     = false ;
    _aChoixPubli[i]._bImprimante = false ;
    strcpy(_aChoixPubli[i]._nb_expl, "") ;
    _aChoixPubli[i]._iCpt        = 0 ;
    // l'url est ici à vide par défaut
    // car le patient en cours n'est pas le cas par défaut
    _aChoixPubli[i]._sUrl        = string("") ;
    _aChoixPubli[i]._bEmail      = false ;
    _aChoixPubli[i]._bIntranet   = false ;
    _aChoixPubli[i]._bHtml       = false ;
    _aChoixPubli[i]._bPdf        = false ;
    _aChoixPubli[i]._bLettre     = false ;
	}

  _choixPubli._sCorresp    = string("") ;  _choixPubli._bSelect     = true ;
  _choixPubli._bImprimante = true ;
  strcpy(_choixPubli._nb_expl, "1") ;
  _choixPubli._iCpt        = 1 ;
  _choixPubli._sUrl        = string("") ;
  _choixPubli._bEmail      = false ;
  _choixPubli._bIntranet   = false ;
  _choixPubli._bHtml       = false ;
  _choixPubli._bPdf        = false ;
  _choixPubli._bLettre     = false ;

	return true ;
}
catch (...)
{
	erreur("Exception NSPublication::ChargeCorrespBase", standardError, 0) ;
	return false;
}
}

boolNSPublication::ExisteCorrespSelect()
{
	for (int i = 0 ; i < MAXSELECT ; i++)
		if (_aChoixPubli[i].isSelected())
			return true ;

	return false ;
}

#ifndef N_TIERSboolNSPublication::AppendWorkflow(char *dest, char *type, char *nb_expl, string entete, string tmpl)
{
try
{
  NSWorkflow* pWorkflow = new NSWorkflow(pContexte);
	pWorkflow->lastError = pWorkflow->open();
	if (pWorkflow->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier Workflow.db", 0, pWorkflow->lastError, pContexte->GetMainWindow()->GetHandle());
		delete pWorkflow;
		return false;
	}

    strcpy(pWorkflow->pDonnees->code_docu,   codeDoc.c_str());
    strcpy(pWorkflow->pDonnees->code_dest,   dest);
    strcpy(pWorkflow->pDonnees->type_publi,  type);
    strcpy(pWorkflow->pDonnees->date_publi,  dateSysteme().c_str());
    strcpy(pWorkflow->pDonnees->heure_publi, heureSysteme().c_str());
    strcpy(pWorkflow->pDonnees->nb_publi,    nb_expl);
    strcpy(pWorkflow->pDonnees->entet_publi, entete.c_str());
    strcpy(pWorkflow->pDonnees->tmpl_publi,  tmpl.c_str());
    strcpy(pWorkflow->pDonnees->ar_publi,    "0");
    strcpy(pWorkflow->pDonnees->valide,      "1");

    pWorkflow->lastError = pWorkflow->appendRecord();

	if (pWorkflow->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ajout d'une fiche Workflow.", 0, pWorkflow->lastError, pContexte->GetMainWindow()->GetHandle());
		pWorkflow->close();
		delete pWorkflow;
		return false;
	}

    pWorkflow->lastError = pWorkflow->close();
    if (pWorkflow->lastError != DBIERR_NONE)
		erreur("Erreur de fermeture du fichier Workflow.", 0, pWorkflow->lastError, pContexte->GetMainWindow()->GetHandle());

    delete pWorkflow;

    return true;
}
catch (...)
{
    erreur("Exception.", standardError, 0) ;
    return false;
}
}
#endif
#ifndef N_TIERS
boolNSPublication::RemplitWorkflow()
{
    for (int i = 0; i < MAXSELECT; i++)
    {
   	    if (aChoixPubli[i].select)
        {
            if (aChoixPubli[i].imprimante)
         	    AppendWorkflow(aChoixPubli[i].corresp, "I", aChoixPubli[i].nb_expl,
            				                entetePubli("I"), tempPubli("I"));

            if (aChoixPubli[i].email)
         	    AppendWorkflow(aChoixPubli[i].corresp, "E", "1",
            				                entetePubli("E"), tempPubli("E"));

            if (aChoixPubli[i].intranet)
         	    AppendWorkflow(aChoixPubli[i].corresp, "R", "1",
            				                entetePubli("R"), tempPubli("R"));

            if (aChoixPubli[i].html)
         	    AppendWorkflow(aChoixPubli[i].corresp, "H", "1",
            				                entetePubli("H"), tempPubli("H"));
        }
    }

    return true;
}
#endif

boolNSPublication::CodeDocLettre(int index, string& sCode)
{
try
{
	sCode = getCodeLettre(_aChoixPubli[index]._sCorresp) ;

  if (string("") == sCode)
		return false ;

	return true ;
}
catch (...)
{
	erreur("Exception NSPublication::CodeDocLettre.", standardError, 0) ;
	return false ;
}
}

string
NSPublication::getCodeLettre(string sPersonID)
{
try
{
	if ((string("") == sPersonID) || (NULL == _pDocMaitre))
		return string("") ;

	if (NULL == pContexte->getPatient())
  	return string("") ;

  NSPersonGraphManager* pGraphManager = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphManager)
  	return string("") ;

  NSLinkManager* pGraphe = pGraphManager->getLinkManager() ;

	VecteurString aVecteurString ;
	pGraphe->TousLesVraisDocument(_pDocMaitre->getDocId(), NSRootLink::letterOfDocument, &aVecteurString, "ENVERS") ;
	if (aVecteurString.empty())
		return string("") ;

	NSHISTODocument* pDocHisPat = pContexte->getPatient()->getDocHis() ;
  if (NULL == pDocHisPat)
		return string("") ;

	for (EquiItemIter i = aVecteurString.begin() ; aVecteurString.end() != i ; i++)
  {
    // Check that it is valid and not "in-memory"
    //
    if ((strlen((*i)->c_str()) == PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) &&
        (INMEMORY_CHAR != (**i)[PAT_NSS_LEN]))
    {
  	  // First attempt, look for the document in history
      //
		  DocumentIter itCurrentDoc = pDocHisPat->TrouveDocHisto(**i) ;
		  if (NULL != itCurrentDoc)
      {
    	  if ((*itCurrentDoc)->getDestinat() == sPersonID)
    		  return (*itCurrentDoc)->getID() ;
      }
      // Second attempt, parse it from graph
      //
      else
      {
    	  NSDocumentInfo docInfo(**i, pContexte, pGraphManager) ;
        if ((true == docInfo.ParseMetaDonnees()) &&
            (docInfo.getDestinat() == sPersonID))
    		  return **i ;
      }
    }
  }

	return string("") ;
}
catch (...)
{
	erreur("Exception NSPublication::getCodeLettre.", standardError, 0) ;
	return string("") ;
}
}

stringNSPublication::StringAdresse(string sCodeCorresp)
{
	if (string("") == sCodeCorresp)
		return string("") ;

	NSPersonInfo *pCorrespSelect = (NSPersonInfo *) 0 ;
	if (false == _aCorrespBaseArray.empty())	{
		for (NSPersonIter k = _aCorrespBaseArray.begin() ; _aCorrespBaseArray.end() != k ; k++)
		{			if (sCodeCorresp == (*k)->getNss())
			{				pCorrespSelect = *k ;
        break ;
      }
    }
	}

  if ((NSPersonInfo*) NULL == pCorrespSelect)
  	return string("") ;

	if (string("") != pCorrespSelect->getMainAdr())
  	return pCorrespSelect->getMainAdr() ;

	// on charge ici en mémoire le graphe de personne et le graphe d'adresse
	string sNss = pCorrespSelect->getNss() ;

  string sLanguage = pContexte->getUserLanguage() ;

  string sAdresse = string("") ;

	if ((pContexte->getPatient()) && (pContexte->getPatient()->getNss() == sNss))
		sAdresse = pCorrespSelect->getMainAdr(pidsPatient, sLanguage) ;
	else
		sAdresse = pCorrespSelect->getMainAdr(pidsCorresp, sLanguage) ;

	return sAdresse ;}

stringNSPublication::NomCorresp(string sCodeCorresp)
{
	string sNom = string("") ;

	if (string("") == sCodeCorresp)
		return sNom ;

	NSPersonInfo *pCorrespSelect = 0 ;
	if (false == _aCorrespBaseArray.empty())
	{
		for (NSPersonIter k = _aCorrespBaseArray.begin() ; _aCorrespBaseArray.end() != k ; k++)
		{			if (sCodeCorresp == (*k)->getNss())
      {      	pCorrespSelect = *k ;
        break ;
      }
    }
	}

	if (pCorrespSelect)
	{
		sNom = pCorrespSelect->getPrenom() ;
		if (string("") != sNom)    	sNom += string(" ") ;		sNom += pCorrespSelect->getNom() ;	}	else // cas, par exemple, de l'impression sans correspondant
		sNom = "XXX" ;

	return sNom ;}

voidNSPublication::Publication()
{
  bool bImpress = false ;
  bool bEMail   = false ;
  bool bExport  = false ;
  //
  // recuperation de la template et de l'en-tete de publication
  //

  // Modif RS du 04-05-04 :
  // on prend dorénavant la template et l'en-tete bruts récupérés dans le constructeur
  // et quel que soit le cas de figure, on reprend le PathName("NTPL") pour tenir compte
  // des différents accès au répertoire NTPL (cas supports <> sur serveur et sur client)
  if (_pDocMaitre)
  {
    if (string("") != _fichTmpl)
      _pDocMaitre->_sTemplate = pContexte->PathName("NTPL") + _fichTmpl ;
    else
      _pDocMaitre->_sTemplate = string("") ;

    if (string("") != _enteteTmpl)
      _pDocMaitre->_sEnTete = pContexte->PathName("NTPL") + _enteteTmpl ;
    else
      _pDocMaitre->_sEnTete = string("") ;
  }

  //
  // La publication se fait par paquets homogènes : e-mails, puis exportation
  // HTML puis impression ; on regarde d'abord pour quels modes il existe
  // au moins un document à publier
  //
  if (_aCorrespBaseArray.empty())
  {
    // publication sans correspondant
    if (_choixPubli.wantToPrint())
      bImpress = true ;

    if (_choixPubli.wantMail())
      bEMail = true ;

    if (_choixPubli.wantHtmlExport())
      bExport = true ;

    if (_choixPubli.wantPdfExport())
      bExport = true ;
  }
  else
  {
    for (int i = 0; i < MAXSELECT; i++)
    {
      if (_aChoixPubli[i].isSelected())
      {
        if ((false == bImpress) && (_aChoixPubli[i].wantToPrint()))
          bImpress = true ;

        if ((false == bEMail) && (_aChoixPubli[i].wantMail()))
          bEMail = true ;

        if ((false == bExport) && (_aChoixPubli[i].wantHtmlExport() || _aChoixPubli[i].wantPdfExport()))
          bExport = true ;
      }
    }
  }

  _numImpress = 0 ;

  //
  // On envoie d'abord les e-mail
  //
  if (bEMail)
  {
    // récupération des paramètres de emailXXX.dat
	  if (false == pContexte->getUtilisateur()->initMail())
    {
  	  erreur("Envoi des e-mails impossible.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
	  }
    else
    {
      // nom du fichier à exporter : nom document + nom patient + prenom patient
      if (_pDocMaitre && _pDocMaitre->_pDocInfo)
      {
  	    _sObjet = _pDocMaitre->_pDocInfo->getDocName() ;
        if (string("") == _sObjet)
    	    _sObjet = string("fichier") ;

        _sObjet += string(" ") ;
      }
      else
  	    _sObjet = string("fichier ") ;

	    if (pContexte->getPatient())
      {
		    _sObjet += pContexte->getPatient()->getNom() + string(" ") ;
  	    _sObjet += pContexte->getPatient()->getPrenom() ;
      }

      NSMailParams* pMail = pContexte->getUtilisateur()->getRegularMail() ;

      if (NSMAIL_MAPI == pMail->getSendingModality())
        SendMapiMails() ;
      else
        SendNsmtpMails() ;
    }
  }
  //
  // Lancement des exportations
  //
  if (bExport)
  {
    if (_aCorrespBaseArray.empty())
    {
      if (_choixPubli.wantHtmlExport() || _choixPubli.wantPdfExport())
      {
        if (false == Exporter(pContexte->PathName("EHTM"), _choixPubli.wantHtmlExport(), _choixPubli.wantPdfExport()))
          erreur("Impossible d'exporter le document.",standardError,0,pContexte->GetMainWindow()->GetHandle());
      }
    }
    else
    {
      for (int i = 0 ; i < MAXSELECT ; i++)
      {
        if (_aChoixPubli[i].isSelected() && (_aChoixPubli[i].wantHtmlExport() || _aChoixPubli[i].wantPdfExport()))
        {
          if (false == Exporter(pContexte->PathName("EHTM"), _aChoixPubli[i].wantHtmlExport(), _aChoixPubli[i].wantPdfExport(), i))
          {
            char msg[255] ;
            string sDest = NomCorresp(_aChoixPubli[i].getCorrespId()) ;
            sprintf(msg, "Impossible d'envoyer le document destiné à %s.", sDest.c_str()) ;
            erreur(msg, standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
          }
        }
      }
    }
    string sMessage = string("Tâche d'exportation terminée (dans le répertoire ") + pContexte->PathName("EHTM") + string(")") ;
    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str();
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_OK);
  }
  //
  // Lancement de l'impression : gérée par le browser
  //
  if (bImpress)
  {
    _indexSelect = 0 ;
    DebutImpression() ;  // fonction de synchronisation
    ImprimerSuivant() ;  // processus d'impression
  }
}

void
NSPublication::GetMailsTypes(bool &bExistStandardMail, bool &bExistCryptedMail)
{
  bExistStandardMail = false ;
  bExistCryptedMail  = false ;

  // No corresps
  //
  if (_aCorrespBaseArray.empty())
  {
    if (_choixPubli.wantMail())
    {
      string sMailAddress = _choixPubli.getMailAddress() ;
      if (NSApicrypt::isApicryptAddress(sMailAddress))
        bExistCryptedMail  = true ;
      else
        bExistStandardMail = true ;
    }
  }
  else
  {
    for (int i = 0 ; i < MAXSELECT ; i++)
    {
      if (_aChoixPubli[i].isSelected() && _aChoixPubli[i].wantMail())
      {
        string sMailAddress = _aChoixPubli[i].getMailAddress() ;
        if (NSApicrypt::isApicryptAddress(sMailAddress))
          bExistCryptedMail  = true ;
        else
          bExistStandardMail = true ;
      }
    }
  }
}

void
NSPublication::SendNsmtpMails()
{
  string sTrace = string("Sending electronic mails using SMTP") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Is there some standard mail and/or some crypted mail to be sent ?
  //
  bool bExistStandardMail = false ;
  bool bExistCryptedMail  = false ;

  GetMailsTypes(bExistStandardMail, bExistCryptedMail) ;

  if (bExistStandardMail)
    SendNsmtpStandardMails() ;

  if (bExistCryptedMail)
    SendNsmtpApicryptMails() ;

  sTrace = string("eMails sent using SMTP") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSPublication::SendNsmtpStandardMails()
{
  string sTrace = string("Sending not crypted mails") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  SendNsmtpSpecificMails(pContexte->getUtilisateur()->getRegularMail(), false) ;
}

void
NSPublication::SendNsmtpApicryptMails()
{
  string sTrace = string("Sending crypted mails") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  SendNsmtpSpecificMails(pContexte->getUtilisateur()->getMailApicrypt(), true) ;
}

void
NSPublication::SendNsmtpSpecificMails(NSMailParams* pMailParams, bool bCrypted)
{
  string sTrace = string("Sending not crypted mails") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  // connexion au serveur SMTP
  ConnectHost(pMailParams) ;

  if ((TIndySMTP*) NULL == _pXSMTP)
  {
    sTrace = string("Cannot connect SMTP host, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
    return ;
  }

  if (_aCorrespBaseArray.empty())
  {
    // publication sans correspondant
    if ((_pXSMTP->Continuer()) && (_choixPubli.wantMail()))
    {
      if (false == SendMail(pContexte->PathName("EHTM")))
        erreur("Impossible d'envoyer le document par e-mail.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;    }
  }
  else
  {
    for (int i = 0 ; i < MAXSELECT ; i++)
    {
      if (false == _pXSMTP->Continuer())
        break ;

      if (_aChoixPubli[i].isSelected() && _aChoixPubli[i].wantMail() &&
           (NSApicrypt::isApicryptAddress(_aChoixPubli[i].getMailAddress()) == bCrypted))
      {
        // boucle d'attente entre chaque SendMail
        while (_pXSMTP->Attendre()) ;

        if (false == SendMail(pContexte->PathName("EHTM"), i))
        {
          string sDest = NomCorresp(_aChoixPubli[i].getCorrespId()) ;
          sTrace = string("Impossible d'envoyer le document destiné à ") + NomCorresp(_aChoixPubli[i].getCorrespId()) ;
          pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trWarning) ;
          erreur(sTrace.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        }
      }
    }
  }

  // boucle d'attente du dernier SendMail
  while (_pXSMTP->Attendre()) ;

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  if (_pXSMTP->Continuer())
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail terminé.", sCaption.c_str(), MB_OK) ;
  else
    ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail interrompu.", sCaption.c_str(), MB_OK) ;

  DisconnectHost() ;
}

void
NSPublication::SendMapiMails()
{
  string sTrace = string("Sending electronic mails using Mapi") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  // Is there some standard mail and/or some crypted mail to be sent ?
  //
  bool bExistStandardMail = false ;
  bool bExistCryptedMail  = false ;

  GetMailsTypes(bExistStandardMail, bExistCryptedMail) ;

  if (bExistStandardMail)
    SendMapiStandardMails() ;

  if (bExistCryptedMail)
    SendMapiApicryptMails() ;

  sTrace = string("eMails sent using Mapi") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void
NSPublication::SendMapiStandardMails()
{
  string sTrace = string("Sending not crypted mails") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  SendMapiSpecificMails(pContexte->getUtilisateur()->getRegularMail(), false) ;
}

void
NSPublication::SendMapiApicryptMails()
{
  string sTrace = string("Sending crypted mails") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  SendMapiSpecificMails(pContexte->getUtilisateur()->getMailApicrypt(), false) ;
}

void
NSPublication::SendMapiSpecificMails(NSMailParams* pMailParams, bool bCrypted)
{
  string sTrace = string("Sending electronic mails using Mapi") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  if (NULL == _pMapiSMTP)
    _pMapiSMTP = new NSMapiCtrl(pContexte, _pDocMaitre, pMailParams) ;

  _pMapiSMTP->Connect() ;
  if (false == _pMapiSMTP->Verify())
  {
    sTrace = string("Cannot use MAPI, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trError) ;
  }

  if (_aCorrespBaseArray.empty())
  {
    // publication sans correspondant
    if (_choixPubli.wantMail())
    {
      if (false == SendMapiMail(pContexte->PathName("EHTM")))
        erreur("Impossible d'envoyer le document par e-mail.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;    }
  }
  else
  {
    for (int i = 0; i < MAXSELECT; i++)
    {
      if (_aChoixPubli[i].isSelected() && _aChoixPubli[i].wantMail() &&
          (NSApicrypt::isApicryptAddress(_aChoixPubli[i].getMailAddress()) == bCrypted))
      {
        if (false == SendMapiMail(pContexte->PathName("EHTM"), i))
        {
          string sDest = NomCorresp(_aChoixPubli[i].getCorrespId()) ;
          sTrace = string("Impossible d'envoyer le document destiné à ") + NomCorresp(_aChoixPubli[i].getCorrespId()) ;
          erreur(sTrace.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        }
      }
    }
  }

  string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Envoi d'E-Mail terminé.", sCaption.c_str(), MB_OK) ;

  _pMapiSMTP->Disconnect() ;
}

// fonction appelée par le browser en fin d'impression
// Retourne false si on relance une impression
// Retourne true si la tache d'impression est terminée
bool
NSPublication::ImprimerSuivant()
{
	int i ;
	bool bFinish = false ;

	if (_aCorrespBaseArray.empty())
  {
  	if (_choixPubli.isSelected() && _choixPubli.wantToPrint())
    {
    	if (false == Imprimer())
      	erreur("Impossible d'imprimer le document.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

      _choixPubli._bSelect = false ;    }
    bFinish = true ;  }
  else
  {
  	for (i = _indexSelect ; i < MAXSELECT ; i++)
    {
    	if (_aChoixPubli[i].isSelected() && _aChoixPubli[i].wantToPrint())
      {
      	_indexSelect = i + 1 ;
      	if (false == Imprimer(i))
        {
        	// dans ce cas on essaie directement d'imprimer le suivant (pas de break)
          char msg[100] ;
          string sDest = NomCorresp(_aChoixPubli[i].getCorrespId()) ;
          sprintf(msg, "Impossible d'imprimer le document destiné à %s.", sDest.c_str()) ;
          erreur(msg, standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        }
        else
        	break ;

/*
         	    if (!Imprimer(i))
                {
            	    // dans ce cas on essaie directement d'imprimer le suivant (pas de break)
                    char msg[100];
                    string sDest = NomCorresp(aChoixPubli[i].corresp);
                    sprintf(msg,"Impossible d'imprimer le document destiné à %s.",sDest.c_str());
                    erreur(msg,0,0,pContexte->GetMainWindow()->GetHandle());
                }
                else
                {
            	    indexSelect = i + 1;
                    break;
                }
*/
			}
		}

		if (MAXSELECT == i) // plus de corresp => on ferme le browser
    	bFinish = true;
	}

	return bFinish ;
}

// Fonction appelée par CmOk et CmCancel de TDIEPubli :
// Destruction des fichiers temporaires d'impression
///////////////////////////////////////////////////////
void
NSPublication::DeleteFichPubli()
{
	string pathHtml = pContexte->PathName("SHTM") ;

	for (int j = 0 ; j < _numImpress ; j++)
	{
  	_sFichHtml[j] = pathHtml + _sFichHtml[j] ;
    if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), _sFichHtml[j]))
    	erreur("Pb de destruction d'un fichier de publication.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

    if (string("") != _sLettreHtml[j])
    {
    	_sLettreHtml[j] = pathHtml + _sLettreHtml[j] ;
      if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), _sLettreHtml[j]))
      	erreur("Pb de destruction d'une lettre jointe.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    }
	}

	_numImpress = 0 ;
}

void
NSPublication::InstalleHook()
{
	if (_bUseHook)
		hhook = SetWindowsHookEx(WH_CALLWNDPROCRET, CallWndRetProc, (HINSTANCE) NULL, GetCurrentThreadId()) ;
}

void
NSPublication::LibereHook()
{
  if (_bUseHook)
    UnhookWindowsHookEx(hhook) ;
}

void
NSPublication::ConnectHost(NSMailParams* pMailParams)
{
  if ((NSMailParams*) NULL == pMailParams)
    return ;

try
{
  // string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  // string sMessage = string("Vous devez être connectés à internet pour que ") + pContexte->getSuperviseur()->getAppName().c_str() + string(" envoie vos e-mails automatiquement. Etes-vous connectés en ce moment ?") ;
  // int idRet = ::MessageBox(pContexte->GetMainWindow()->GetHandle(), sMessage.c_str(), sCaption.c_str(), MB_YESNO) ;

	// if (idRet == IDYES)
	// {
    _pXSMTP = new TIndySMTP(NULL, pContexte, _pDocMaitre, pMailParams) ;
    _pXSMTP->Show() ;
    _pXSMTP->Connect() ;
    if (false == _pXSMTP->IsConnected())
    {
    	_pXSMTP->Close() ;
      delete _pXSMTP ;
      _pXSMTP = (TIndySMTP*) 0 ;
    }
	// }
  // else
  // {
  //	string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
  //  ::MessageBox(pContexte->GetMainWindow()->GetHandle(), "Impossible d'envoyer les e-mails.", sCaption.c_str(), MB_OK) ;
  //  pXSMTP = 0 ;
	// }
}
catch (...)
{
	erreur("Exception NSPublication::ConnectHost", standardError, 0) ;
}
}

void
NSPublication::DisconnectHost()
{
	if ((TIndySMTP*) NULL == _pXSMTP)
		return ;

	_pXSMTP->Disconnect() ;
	_pXSMTP->Close() ;

	delete _pXSMTP ;
  _pXSMTP = (TIndySMTP*) 0 ;
}

bool
NSPublication::Imprimer(int index)
{
try
{
	string sDest       = string("") ;
	string sAdrCorresp = string("") ;
	char   nomFichHtml[1024] ;
	char   msg[255];
	string ps;

  NSPrintControl* pPrintControl ;

  if (_pDocMaitre)
    pPrintControl = _pDocMaitre->getPrintControler() ;
  else if (_pDocMaitreLdv)
    pPrintControl = _pDocMaitreLdv->getPrintControler() ;
  else
    return false ;

  string sPathHtml = pContexte->PathName("SHTM") ;

	//
	// generation du compte-rendu à la bonne adresse
	//
	if (index >= 0)
	{
		// on est certain ici que le corresp existe
		//
		sDest       = NomCorresp(_aChoixPubli[index].getCorrespId()) ;
		sAdrCorresp = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
		sprintf(nomFichHtml, "%s-%s", _sCodeDoc.c_str(), _aChoixPubli[index].getCorrespId().c_str()) ;

    pPrintControl->setNbImpress(_aChoixPubli[index].getCopyNb()) ;
    pPrintControl->setIndexCorresp(index) ;

		//
		// Impression avec lettre
		//
		if (_aChoixPubli[index].wantLetter())
		{
      string sCodeLettre = string("") ;
			if (CodeDocLettre(index, sCodeLettre))
			{
        pPrintControl->setCodeLettre(sCodeLettre) ;
				pPrintControl->setImprimerLettre(true) ;
				_indexLettre = index ;
				// sAdresseCorresp sert pour NSVisualView::ImprimerLettre()
				_sAdresseCorresp = sAdrCorresp ;
			}
			else
			{
				pPrintControl->setImprimerLettre(false) ;
				char msgerr[100] ;
				sprintf(msgerr, "Impossible d'imprimer la lettre destinée à %s.", sDest.c_str()) ;
				erreur(msgerr, standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
			}
		}
		else			pPrintControl->setImprimerLettre(false) ;	}
	else
	{
		if ((pContexte->getPatient()) && (_choixPubli.getCorrespId() == pContexte->getPatient()->getNss()))   // cas du patient
		{			sDest = pContexte->getPatient()->getPrenom() ;			if (string("") != sDest)
				sDest += string(" ") ;
			sDest += pContexte->getPatient()->getNom() ;

			sAdrCorresp = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
		}
		else
		{
			// en général ici corresp == "" (sDest <= "XXX")
			sDest = NomCorresp(_choixPubli.getCorrespId()) ;
			sAdrCorresp = string("") ;
		}

		sprintf(nomFichHtml, "%s-000", _sCodeDoc.c_str()) ;
		pPrintControl->setNbImpress(_choixPubli.getCopyNb()) ;
		pPrintControl->setIndexCorresp(-1) ;
	}

	_sFichHtml[_numImpress] = string(nomFichHtml) ;

  sprintf(msg, "Impression n°%d - %s", _numImpress + 1, sDest.c_str()) ;
  ps = string(msg) ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  ps = string("Génération du fichier : ") + _sFichHtml[_numImpress] ;
  pContexte->getSuperviseur()->trace(&ps, 1) ;

  if (_pDocMaitre)
  {
    if (false == _pDocMaitre->GenereHtml(sPathHtml, _sFichHtml[_numImpress], toImprimer, sAdrCorresp))
    {
      erreur("Impossible de créer le fichier html à imprimer", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
  	  return false ;
    }
    _pDocMaitre->setNomDocHtml(_sFichHtml[_numImpress]) ;
  }
  else if (_pDocMaitreLdv)
  {
    if (false == _pDocMaitreLdv->GenereHtml(sPathHtml, _sFichHtml[_numImpress], toImprimer, _pLdvPubliDriver, sAdrCorresp))
    {
      erreur("Impossible de créer le fichier html à imprimer", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
  	  return false ;
    }
    _pDocMaitreLdv->setNomDocHtml(_sFichHtml[_numImpress]) ;
  }
  else
    return false ;

	if (_iTempImp)
		Tempo(_iTempImp) ;
	else if (_bWaitImp)
	{
		string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName().c_str() ;
		::MessageBox(pContexte->GetMainWindow()->GetHandle(), msg, sCaption.c_str(), MB_OK) ;	}	ps = "Lancement de la VisualView..." ;
	pContexte->getSuperviseur()->trace(&ps, 1) ;

  _numImpress++ ;

  NSDocViewManager dvManager(pContexte) ;
	dvManager.createView(_pDocMaitre, "Visual Format") ;

  // For synch reasons, it is better to do it earlier
	// numImpress++ ;

	// Ancienne méthode de publication :
	// on lance le navigate qui est ensuite intercepté par l'objet browser	// pDocMaitre->pWebBrowserPubli->Navigate(sFichHtml[numImpress++]);
	return true ;
}
catch (...)
{
	erreur("Exception NSPublication::Imprimer", standardError, 0) ;
	return false ;
}
}

bool
NSPublication::GetMailAndLetter(string &sMailAddress, string &sLetterId, string &sTempFileName, string &sPlainAddress, int index)
{
  sMailAddress  = string("") ;
  sLetterId     = string("") ;
  sTempFileName = string("") ;
  sPlainAddress = string("") ;

  // Correspondant
  //  if (index >= 0)
  {
    NSChoixPubli* pPubli = &(_aChoixPubli[index]) ;

    if ((NSChoixPubli*) NULL == pPubli)
      return false ;

    sMailAddress  = pPubli->getMailAddress() ;
    sPlainAddress = StringAdresse(pPubli->getCorrespId()) ;
    sTempFileName = _sCodeDoc + string("-") + pPubli->getCorrespId() ;

    if (string("") == sMailAddress)    {
      string sDest = NomCorresp(pPubli->getCorrespId()) ;
      string sMsg  = string("Le correspondant ") + sDest + string(" n'a pas d'adresse e-mail.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trWarning) ;
      erreur(sMsg.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

		if (pPubli->wantLetter())
    {
      if (false == CodeDocLettre(index, sLetterId))
      {
        string sDest = NomCorresp(pPubli->getCorrespId()) ;
        string sMsg = string("Impossible d'envoyer le corps du message pour ") + sDest + string(".") ;        pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trWarning) ;        erreur(sMsg.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;      }    }

    return true ;
  }  // Patient  //  if ((pContexte->getPatient()) && (_choixPubli.getCorrespId() == pContexte->getPatient()->getNss()))
  {
    sMailAddress  = _choixPubli.getMailAddress() ;
    sPlainAddress = StringAdresse(pContexte->getPatient()->getNss()) ;
    sTempFileName = _sCodeDoc + string("-") + _choixPubli.getCorrespId() ;

    if (string("") == sMailAddress)    {
      string sDest = NomCorresp(_choixPubli.getCorrespId()) ;
      string sMsg = string("Le patient ") + sDest + string(" n'a pas d'adresse e-mail.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trWarning) ;
      erreur(sMsg.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }
  // Not current patient
  //
  else
  {
    sTempFileName = _sCodeDoc ;    sMailAddress  = _choixPubli.getMailAddress() ;
    if (string("") == sMailAddress)    {
      string sMsg = string("Vous n'avez pas mentionné d'adresse e-mail.") ;
      pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trWarning) ;
      erreur(sMsg.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }

  return true ;
}

boolNSPublication::SendMail(string sPathHtml, int index)
{
try
{
  string sNomDoc ;
  string sFichierHtml ;
  string sFichierImage ;

  NSBaseImages* pBase ;

  // generation du compte-rendu à la bonne adresse  //  string sEMail      = string("") ;  string sCodeLettre = string("") ;  string sNomHtml    = string("") ;  string sAdrCorresp = string("") ;  if (false == GetMailAndLetter(sEMail, sCodeLettre, sNomHtml, sAdrCorresp, index))    return false ;  string sNameExp     = string("") ;
  string sMailExp     = string("") ;
  string sMailReplyTo = string("") ;
  string sMailSender  = string("") ;

  _pXSMTP->ClearMessage() ;

  // Apicrypt : HPRIM header + raw text
  //
  NSApicrypt apicryptEngine ;

  if (apicryptEngine.isApicryptAddress(sEMail))
  {
    string sMessage = string("Mail address ") + sEMail + string(" detected as Apicrypt address... crypting") ;
    pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trSubDetails) ;

    string sCryptedRawText = string("") ;

    string sRawText = string("") ;
    if (_pDocMaitre->GenereRawText(sRawText))
      sCryptedRawText = apicryptEngine.getMailText(pContexte, sRawText, sEMail, _aChoixPubli[index].getCorrespId(), (NSPatInfo*) _pPatChoisi, (NSNoyauDocument*) _pDocMaitre) ;

    if (string("") == sCryptedRawText)
    {
      sMessage = string("Crypting failed, cannot send mail.") ;
      pContexte->getSuperviseur()->trace(&sMessage, 1, NSSuper::trError) ;
      erreur("Echec du cryptage ; le message n'a pas été envoyé.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    sFichierHtml = string("") ;
    sNameExp     = pContexte->getUtilisateur()->getMailApicrypt()->getName() ;
    sMailExp     = pContexte->getUtilisateur()->getMailApicrypt()->getFrom() ;
    sMailReplyTo = pContexte->getUtilisateur()->getMailApicrypt()->getReplyTo() ;
    sMailSender  = pContexte->getUtilisateur()->getMailApicrypt()->getSender() ;

    _pXSMTP->SetApicryptMode() ;
    _pXSMTP->ComposeSimpleMessage(sMailSender, sNameExp, sMailExp, sMailReplyTo, sEMail, _sObjet, sCryptedRawText) ;
    _pXSMTP->SendMail() ;

    return true ;
  }
  else
  {
    if (false == _pDocMaitre->GenereHtml(sPathHtml, sNomHtml, toExporter, sAdrCorresp, sPathHtml))    {
      erreur("Impossible de créer le fichier html à expédier", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    sFichierHtml = sPathHtml + sNomHtml ;
    sNameExp     = pContexte->getUtilisateur()->getRegularMail()->getName() ;
    sMailExp     = pContexte->getUtilisateur()->getRegularMail()->getFrom() ;
    sMailReplyTo = pContexte->getUtilisateur()->getRegularMail()->getReplyTo() ;
    sMailSender  = pContexte->getUtilisateur()->getRegularMail()->getSender() ;

    // corps du message
    //
    _pXSMTP->ComposeMessage(sMailSender, sNameExp, sMailExp, sMailReplyTo, sEMail, _sObjet, sCodeLettre) ;
  }

/*
  if (!InitCrypteurs(sEMail))
  {
    erreur("Impossible d'initialiser le module de cryptage", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }
  if (pCrypteMsg != NULL)
  {
    long lErr = ((*pCrypteMsg)
                   ((NSContexte far *) pContexte, (string far *) &sCodeLettre,
                    (string far *) &sEMail, NULL)) ;
    if (lErr != 0)
    {
      erreur("Impossible de crypter le message",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }
  if (pCrypteMsg != NULL)
  {
    long lErr = ((*pCrypteMsg)
                   ((NSContexte far *) pContexte, (string far *) &sCodeLettre,
                    (string far *) &sEMail, NULL));
    if (lErr != 0)
    {
      erreur("Impossible de crypter le message",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }
*/


  // Attachement du document Html
  if (string("") != sFichierHtml)
  {
    if (_pCrypteFic)
    {
      string sFichierInitial = sFichierHtml ;
      long lErr = ((*_pCrypteFic)
                     ((NSContexte far *) pContexte, (string far *) &sFichierHtml,
                        (string far *) &sEMail, NULL)) ;
      if ((double) 0 != lErr)
      {
        erreur("Impossible de crypter le document HTML", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
      _pXSMTP->AttacheMessage(sFichierHtml) ;
      if (sFichierHtml != sFichierInitial)
      {
        NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierHtml) ;
        sFichierHtml = sFichierInitial ;
      }
    }
    else
      _pXSMTP->AttacheMessage(sFichierHtml) ;
  }

  // Attachement des images
  if (string("") != _pDocMaitre->_sBaseImages)
  {
    pBase = new NSBaseImages(_pDocMaitre->_sBaseImages) ;
    pBase->lire() ;

    for (int i = 0; i < pBase->nbImages; i++)
    {
      sFichierImage = pBase->tableDesImg[i] + pBase->tableImages[i] ;
      if (_pCrypteFic)
      {
        string sFichierImageInitial = sFichierImage ;
        long lErr = ((*_pCrypteFic)
                     ((NSContexte far *) pContexte,
                      (string far *) &sFichierImage,
                      	  	 (string far *) &sEMail, NULL));
        if ((double) 0 != lErr)
        {
          erreur("Impossible de crypter une image", standardError, 0, pContexte->GetMainWindow()->GetHandle());
          return false ;
        }
        _pXSMTP->AttacheMessage(sFichierImage) ;
        if (sFichierImage != sFichierImageInitial)
        {
          NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierImage) ;
          sFichierImage = sFichierImageInitial ;
        }
      }
      else
        _pXSMTP->AttacheMessage(sFichierImage) ;
		}
	}
	_pXSMTP->SendMail() ;

  if (string("") != _pDocMaitre->_sBaseImages)
  {
  	pBase->detruire() ;
    _pDocMaitre->_sBaseImages = string("") ;
    delete pBase ;
  }

  if (string("") != sFichierHtml)
	  if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierHtml))
  	  erreur("Pb de destruction du fichier de publication",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;

	return true ;
}catch (...)
{
  erreur("Exception NSPublication::SendMail.", standardError, 0) ;
  return false ;
}
}

bool
NSPublication::SendMapiMail(string sPathHtml, int index)
{
try
{
	string sAdrCorresp = string("") ;
  string sNomHtml ;
  string sNomDoc ;
  string sFichierHtml ;
  string sFichierImage ;
  string sEMail ;
  string sCodeLettre = "" ;
  char   nomFichHtml[1024] ;
  NSBaseImages* pBase ;

  // generation du compte-rendu à la bonne adresse  // Correspondant  //  if (index >= 0)
  {
    sAdrCorresp = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
    sprintf(nomFichHtml, "%s-%s", _sCodeDoc.c_str(), _aChoixPubli[index].getCorrespId().c_str()) ;
    sEMail = _aChoixPubli[index].getMailAddress() ;

    if (string("") == sEMail)    {
      string sErrorText = string("Le correspondant ") + NomCorresp(_aChoixPubli[index].getCorrespId()) + string(" n'a pas d'adresse e-mail.") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

		if (_aChoixPubli[index].wantLetter())
    {
      if (false == CodeDocLettre(index, sCodeLettre))
      {
        string sErrorText = string("Impossible d'envoyer le corps du message pour ") + NomCorresp(_aChoixPubli[index].getCorrespId()) ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;        erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;      }    }
  }  // Patient  //  else
  {
    if ((pContexte->getPatient()) && (_choixPubli.getCorrespId() == pContexte->getPatient()->getNss()))
    {
      sAdrCorresp = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
      sprintf(nomFichHtml, "%s-%s", _sCodeDoc.c_str(), _choixPubli.getCorrespId().c_str()) ;
      sEMail = _choixPubli.getMailAddress() ;

      if (string("") == sEMail)      {
        string sErrorText = string("Le correspondant ") + NomCorresp(_choixPubli.getCorrespId()) + string(" n'a pas d'adresse e-mail") ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
    }
    else
    {
      sAdrCorresp = string("") ;      sprintf(nomFichHtml, "%s", _sCodeDoc.c_str()) ;
      sEMail = _choixPubli.getMailAddress() ;
      if (string("") == sEMail)      {
        erreur("Vous n'avez pas mentionné d'adresse e-mail.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
    }
  }

  string sMailExp = string("") ;

  // Apicrypt : HPRIM header + raw text
  //
  NSApicrypt apicryptEngine ;

  if (apicryptEngine.isApicryptAddress(sEMail))
  {
    string sCryptedRawText = string("") ;

    string sRawText = string("") ;
    if (_pDocMaitre->GenereRawText(sRawText))
      sCryptedRawText = apicryptEngine.getMailText(pContexte, sRawText, sEMail, _aChoixPubli[index].getCorrespId(), (NSPatInfo*) _pPatChoisi, (NSNoyauDocument*) _pDocMaitre) ;

    if (string("") == sCryptedRawText)
      return false ;

    sFichierHtml = string("") ;
    sMailExp     = pContexte->getUtilisateur()->getMailApicrypt()->getSender() ;

    _pMapiSMTP->SetApicryptMode() ;
    _pMapiSMTP->ComposeMessage(sMailExp, sEMail, _sObjet, sCryptedRawText) ;
    _pMapiSMTP->SendMail() ;

    return true ;
  }
  else
  {
    sNomHtml = string(nomFichHtml) ;
    if (false == _pDocMaitre->GenereHtml(sPathHtml, sNomHtml, toExporter, sAdrCorresp, sPathHtml))
    {
      string sErrorText = pContexte->getSuperviseur()->getText("publishingManagement", "cannotCreateHtmlExportFile") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }

    sFichierHtml = sPathHtml + sNomHtml ;
    sMailExp     = pContexte->getUtilisateur()->getRegularMail()->getSender() ;

    // corps du message
    //
    _pMapiSMTP->ComposeMessage(sMailExp, sEMail, _sObjet, sCodeLettre) ;
  }

/*
  if (!InitCrypteurs(sEMail))
  {
    erreur("Impossible d'initialiser le module de cryptage", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return false ;
  }
  if (pCrypteMsg != NULL)
  {
    long lErr = ((*pCrypteMsg)
                   ((NSContexte far *) pContexte, (string far *) &sCodeLettre,
                    (string far *) &sEMail, NULL)) ;
    if (lErr != 0)
    {
      erreur("Impossible de crypter le message",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }
  if (pCrypteMsg != NULL)
  {
    long lErr = ((*pCrypteMsg)
                   ((NSContexte far *) pContexte, (string far *) &sCodeLettre,
                    (string far *) &sEMail, NULL));
    if (lErr != 0)
    {
      erreur("Impossible de crypter le message",standardError,0,pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
    }
  }
*/


  // Attachement du document Html
  if (string("") != sFichierHtml)
  {
    if (_pCrypteFic)
    {
      string sFichierInitial = sFichierHtml ;
      long lErr = ((*_pCrypteFic)
                     ((NSContexte far *) pContexte, (string far *) &sFichierHtml,
                        (string far *) &sEMail, NULL)) ;
      if ((double) 0 != lErr)
      {
        erreur("Impossible de crypter le document HTML", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
      _pMapiSMTP->AttacheMessage(sFichierHtml) ;
      if (sFichierHtml != sFichierInitial)
      {
        NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierHtml) ;
        sFichierHtml = sFichierInitial ;
      }
    }
    else
      _pMapiSMTP->AttacheMessage(sFichierHtml) ;
  }

  // Attachement des images
  if (string("") != _pDocMaitre->_sBaseImages)
  {
    pBase = new NSBaseImages(_pDocMaitre->_sBaseImages) ;
    pBase->lire() ;

    for (int i = 0; i < pBase->nbImages; i++)
    {
      sFichierImage = pBase->tableDesImg[i] + pBase->tableImages[i] ;
      if (_pCrypteFic)
      {
        string sFichierImageInitial = sFichierImage ;
        long lErr = ((*_pCrypteFic)
                     ((NSContexte far *) pContexte,
                      (string far *) &sFichierImage,
                      	  	 (string far *) &sEMail, NULL));
        if ((double) 0 != lErr)
        {
          erreur("Impossible de crypter une image",standardError,0,pContexte->GetMainWindow()->GetHandle());
          return false;
        }
        _pMapiSMTP->AttacheMessage(sFichierImage) ;
        if (sFichierImage != sFichierImageInitial)
        {
          NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierImage) ;
          sFichierImage = sFichierImageInitial ;
        }
      }
      else
        _pMapiSMTP->AttacheMessage(sFichierImage) ;
		}
	}
	_pMapiSMTP->SendMail() ;

  if (string("") != _pDocMaitre->_sBaseImages)
  {
  	pBase->detruire() ;
    _pDocMaitre->_sBaseImages = string("") ;
    delete pBase ;
  }

  if (string("") != sFichierHtml)
	  if (false == NsDeleteTemporaryFile(pContexte->getSuperviseur(), sFichierHtml))
  	  erreur("Pb de destruction du fichier de publication", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;

	return true ;
}catch (...)
{
  erreur("Exception NSPublication::SendMapiMail.", standardError, 0) ;
  return false ;
}
}

boolNSPublication::Exporter(string sPathHtml, bool bHtml, bool bPdf, int index)
{
	string sAdrCorresp  = string("") ;
	string sNomFichHtml = string("") ;

	// generation du compte-rendu à la bonne adresse
	if (index >= 0)
	{
  	sAdrCorresp  = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
    sNomFichHtml = _sCodeDoc + string("-") + _aChoixPubli[index].getCorrespId() ;
	}
	else
	{
		if ((pContexte->getPatient()) && (_choixPubli._sCorresp == pContexte->getPatient()->getNss()))
    {
    	sAdrCorresp  = StringAdresse(_aChoixPubli[index].getCorrespId()) ;
      sNomFichHtml = _sCodeDoc + string("-") + _aChoixPubli[index].getCorrespId() ;
    }
    else
    {
    	sAdrCorresp  = string("") ;
      sNomFichHtml = _sCodeDoc ;
    }
	}

	string sNomHtml = sNomFichHtml ;

	// exportation du fichier html
  if (_pDocMaitre)
  {
	  if (false == _pDocMaitre->GenereHtml(sPathHtml, sNomHtml, toExporter, sAdrCorresp, sPathHtml))
	  {
      string sErrorText = pContexte->getSuperviseur()->getText("publishingManagement", "cannotCreateHtmlExportFile") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
	  }
  }
  else if (_pDocMaitreLdv)
  {
	  if (false == _pDocMaitreLdv->GenereHtml(sPathHtml, sNomHtml, toExporter, _pLdvPubliDriver, sAdrCorresp, sPathHtml))
	  {
  	  string sErrorText = pContexte->getSuperviseur()->getText("publishingManagement", "cannotCreateHtmlExportFile") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
  	  erreur(sErrorText.c_str(), standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
      return false ;
	  }
  }

  if (bPdf)
  {
    string sPdfFileName = nom_sans_doublons(string(""), string(""), sPathHtml, sNomFichHtml, string("pdf")) ;

    // NSHtmlToPdfConverter Converter(pContexte->getSuperviseur()) ;
    // if (Converter.bIsReady())
    //  Converter.convertHtmlToPdf(sPathHtml + sNomHtml, sPathHtml + sPdfFileName) ;

    NSSuper* pSuper = pContexte->getSuperviseur() ;
    NSHtmlToPdfConverter* pHtmlToPdfConverter = pSuper->getHtmlToPdfConverter() ;
    if ((NSHtmlToPdfConverter*) NULL == pHtmlToPdfConverter)
	  {
      string sErrorText = string("Can't initialize Html to Pdf converter") ;
      pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      return false ;
	  }

    if (pHtmlToPdfConverter->bIsReady())
      pHtmlToPdfConverter->convertHtmlToPdf(sPathHtml + sNomHtml, sPathHtml + sPdfFileName) ;

    if (false == bHtml)
      NsDeleteTemporaryFile(pContexte->getSuperviseur(), sPathHtml + sNomHtml) ;
  }

	// exportation des images : fait par GenereHtml
	/*************************************************
	if (pDocMaitre->sBaseImages != "")
    {
   		NSBaseImages* pBase = new NSBaseImages(pDocMaitre->sBaseImages);
   		pBase->lire();
		if (!pBase->copier(sPathHtml))
        	erreur("Erreur à la copie des images dans le répertoire d'exportation.",0,0);
        pBase->detruire();
        delete pBase;
    }
    **************************************************/

	return true ;
}

voidNSPublication::DeleteVisualViews()
{
	for (TView* pView = _pDocMaitre->GetViewList(); pView != 0; pView = pView->GetNextView())
	{
		if (!strcmp(pView->GetViewName(), "NSVisualView"))
		{
			NSVisualView* pVisualView = dynamic_cast<NSVisualView*>(pView) ;
      if (pVisualView)
				pVisualView->CloseWindow() ;
		}
	}
}

voidNSPublication::initParams()
{
  string sFichierTempo = pContexte->PathName("FPER") + string("tempo.dat") ;

  ifstream inFile ;
  inFile.open(sFichierTempo.c_str()) ;  if (!inFile)
  {
    erreur("Erreur d'ouverture du fichier tempo.dat.", standardError, 0, pContexte->GetMainWindow()->GetHandle()) ;
    return ;
  }

  string sData = string("") ;
  while (!inFile.eof())  {
    string sLine = string("") ;
    getline(inFile, sLine) ;
    if (string("") != sLine)
      sData += sLine + "\n" ;
  }

  inFile.close() ;
  // boucle de chargement des attributs de tempo.dat  //  size_t i = 0 ;  while (i < strlen(sData.c_str()))  {
    string sNomAttrib = string("") ;

    while ((i < strlen(sData.c_str())) && (sData[i] != ' ') && (sData[i] != '\t'))      sNomAttrib += pseumaj(sData[i++]) ;

    while ((i < strlen(sData.c_str())) && ((sData[i] == ' ') || (sData[i] == '\t')))      i++ ;

    string sValAttrib = string("") ;
    while ((i < strlen(sData.c_str())) && (sData[i] != '\n'))      sValAttrib += sData[i++] ;

    if (sNomAttrib == "HOOK")
    {
      if (IsYes(sValAttrib))
        _bUseHook = true ;
      else
        _bUseHook = false ;
    }
    else if (sNomAttrib == "WAITIMP")
    {
      if (IsNo(sValAttrib))
        _bWaitImp = false ;
      else
        _bWaitImp = true ;
    }
    else if (sNomAttrib == "TEMPIMP")
    {
      _iTempImp = atoi(sValAttrib.c_str()) ;
    }

    i++ ;  }}

voidNSPublication::Tempo(int iDelay)
{
	// application du délai (unité : centième de seconde)
	if (((NSSuper*) NULL == pContexte->getSuperviseur()) || (0 == iDelay))
		return ;

	pContexte->getSuperviseur()->Delay(iDelay) ;}

voidNSPublication::Publier()
{
	pContexte->GetMainWindow()->SetCursor(0, IDC_WAIT) ;

	bool bCorrespLoaded = ChargeCorrespBase() ;	pContexte->GetMainWindow()->SetCursor(0, IDC_ARROW) ;
	if (false == bCorrespLoaded)
		return ;

  string ps = string("") ;

	if (_aCorrespBaseArray.empty())	{
  	ps = "Opening PubliSansCorrespDialog..." ;
		pContexte->getSuperviseur()->trace(&ps, 1) ;

		if (PubliSansCorrespDialog(pContexte->GetMainWindow(), pContexte, this).Execute() == IDCANCEL)			return ;

      	// voir workflow...	}
	else
	{
  	if (_aCorrespBaseArray.size() > 1)
    {
    	if (ChoixCorrespDialog(pContexte->GetMainWindow(),pContexte, this).Execute() == IDCANCEL)
      	return ;
    }
    else
    {
    	_aChoixPubli[0]._bSelect = true ;
    }

    ps = string("Opening PubliCorrespDialog...") ;    pContexte->getSuperviseur()->trace(&ps, 1) ;

    if (PubliCorrespDialog(pContexte->GetMainWindow(), pContexte, this).Execute() == IDCANCEL)    	return ;

#ifndef _MUE		RemplitWorkflow() ;#endif
	}

	if ((_aCorrespBaseArray.empty()) || (ExisteCorrespSelect()))	{
    if (_pDocMaitre)
    {
      string sDocSemType = _pDocMaitre->_pDocInfo->getTypeSem() ;

  	  // On regarde si on a un document "publiable"
      if ((sDocSemType != string("ZCN00")) &&
          (sDocSemType != string("ZCS00")) &&
          (sDocSemType != string("ZCQ00")) &&          (sDocSemType != string("ZQQOL")) &&          (string(sDocSemType, 0, 2) != string("ZC")) &&
          (string(sDocSemType, 0, 2) != string("ZP")) &&
          (sDocSemType != string("ZHIST")) &&
          (sDocSemType != string("ZTRTF")) &&
          (sDocSemType != string("ZTHTM")) &&
          (sDocSemType != string("ZTTXT")) &&
          (sDocSemType != string("ZRMUL")) &&
          (sDocSemType != string("ZTPDF")))
      {
    	  erreur("Ce type de document n'est pas géré par la publication", warningError, 0, pContexte->GetMainWindow()->GetHandle()) ;
        return ;
      }
    }

		// sinon :    ps = string("Starting Publication...") ;
    pContexte->getSuperviseur()->trace(&ps, 1) ;

		Publication() ;	}
}

bool
NSPublication::InitCrypteurs(string sURL){
try
{
  _pCrypteMsg = 0 ;
  _pCrypteFic = 0 ;

  string sFAI   = string("") ;
  size_t iPosAt = sURL.find('@') ;
  if ((NPOS != iPosAt) && (iPosAt < strlen(sURL.c_str()) - 1))
    sFAI = string(sURL, iPosAt + 1, strlen(sURL.c_str()) - iPosAt -1) ;

  //
  // Est-ce une adresse Apicrypt ?
  //
  if ((string(sFAI, 0, 7) == string("medical")) && (sFAI.find("netinfo.fr") != NPOS))
  {
    // On ferme la dll de cryptage déjà ouverte, sauf si c'est déjà
    // la dll Apicrypt
    //
    if (pCryptoModule)
    {
      const char* nomModule = pCryptoModule->GetName() ;
      if ((string(nomModule).find(string("ns_apicr")) == NPOS) &&
          (string(nomModule).find(string("NS_APICR")) == NPOS))
      {
        delete pCryptoModule ;
        pCryptoModule = (TModule*) 0 ;
      }
    }
    // Si aucune dll de cryptage n'est ouverte, on ouvre la dll Apicrypt
    //
    if ((TModule*) NULL == pCryptoModule)
    {
      pCryptoModule = new TModule("ns_apicr.dll", TRUE) ;
      if ((TModule*) NULL == pCryptoModule)
      {
        erreur("La DLL de cryptage Apicrypt (ns_apicr.dll) est introuvable.", standardError, -1, pContexte->GetMainWindow()->GetHandle()) ;
        return false ;
      }
    }
    // On initialise les pointeurs de cryptage (chaînes et fichiers)
    //
    (FARPROC) _pCrypteMsg = pCryptoModule->GetProcAddress(MAKEINTRESOURCE(1)) ;
    if (NULL == _pCrypteMsg)
      return false ;
    (FARPROC) _pCrypteFic = pCryptoModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
    if (NULL == _pCrypteFic)
      return false ;
    return true ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception à l'ouverture du module de cryptage.", standardError, 0) ;
  return false ;
}
}

NSChoixPubli*
NSPublication::getChoixPubliForPerson(NSPersonInfo *pCorrespSelect)
{
  if ((NSPersonInfo*) NULL == pCorrespSelect)
    return (NSChoixPubli*) 0 ;

  string sCorrespId = pCorrespSelect->getNss() ;

  for (int index = 0 ; index < MAXSELECT ; index++)
  	if (_aChoixPubli[index].getCorrespId() == sCorrespId)
      return &(_aChoixPubli[index]) ;

  return (NSChoixPubli*) 0 ;
}

////////////////////////////// Messagerie via MAPI ////////////////////////

NSMapiCtrl::NSMapiCtrl(NSContexte* ctx, NSRefDocument* doc, NSMailParams* mail)
{
	// on ne fait pas de copie : tous les pointeurs sont résidents
  _pContexte  = ctx ;
  _pDoc       = doc ;
  _pMail      = mail ;
  _encours    = "(vide)" ;
  _bInterrupt = false ;
  _bAttendre  = false ;

  init() ;

  lObjectCount++ ;
}

NSMapiCtrl::~NSMapiCtrl(){
  reset() ;

  lObjectCount-- ;
}

void
NSMapiCtrl::init()
{
  memset(&_msg, 0, sizeof(_msg)) ;
  memset(&_destinataire, 0, sizeof(_destinataire)) ;
  memset(&_emetteur, 0, sizeof(_emetteur)) ;
  _emetteur.ulRecipClass = MAPI_ORIG ;
}

void
NSMapiCtrl::reset()
{
  if (_msg.lpszSubject)
    delete[] _msg.lpszSubject ;
  if (_msg.lpszNoteText)
    delete[] _msg.lpszNoteText ;

  if (_msg.nFileCount > 0)
  {
    for (size_t i = 0 ; i < _msg.nFileCount ; i++)
    {
      if (_msg.lpFiles[i].lpszPathName)
        delete[] _msg.lpFiles[i].lpszPathName ;
      if (_msg.lpFiles[i].lpszFileName)
        delete[] _msg.lpFiles[i].lpszFileName ;
    }

    delete[] _msg.lpFiles ;
  }

  memset(&_msg, 0, sizeof(_msg)) ;

  if (_destinataire.lpszAddress)
    delete[] _destinataire.lpszAddress ;
  memset(&_destinataire, 0, sizeof(_destinataire)) ;

  if (_emetteur.lpszAddress)
    delete[] _emetteur.lpszAddress ;
  memset(&_emetteur, 0, sizeof(_emetteur)) ;

  _emetteur.ulRecipClass = MAPI_ORIG ;
}

void
NSMapiCtrl::Connect()
{
try
{
  // Ouverture de la DLL
  GetProfileInt("Mail", "MAPI", 0 ) ;
  _MapiInst = LoadLibrary("MAPI32.DLL") ;

  LPMAPILOGON MapiLogon = (LPMAPILOGON) GetProcAddress(_MapiInst, "MAPILogon") ;
  ULONG lResult = MapiLogon(0, NULL, NULL, MAPI_LOGON_UI, 0, &_session) ;

  if (SUCCESS_SUCCESS != lResult)
  {
    string sErrorText = string("") ;

    if      (lResult == MAPI_E_FAILURE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_FAILURE") ;
    else if (lResult == MAPI_E_INSUFFICIENT_MEMORY)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_INSUFFICIENT_MEMORY") ;
    else if (lResult == MAPI_E_LOGIN_FAILURE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_LOGIN_FAILURE") ;
    else if (lResult == MAPI_E_TOO_MANY_SESSIONS )
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_TOO_MANY_SESSIONS ") ;
    else if (lResult == MAPI_E_USER_ABORT)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_USER_ABORT") ;

    sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "errorSendingMapiMessage") + string(" : ") + sErrorText ;
    _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
  }
}
catch (...)
{
  erreur("Exception à l'ouverture de MAPI.", standardError, 0) ;
}
}

void
NSMapiCtrl::Disconnect()
{
try
{
  LPMAPILOGOFF MapiLogoff = (LPMAPILOGOFF) GetProcAddress(_MapiInst, "MAPILogoff") ;
  MapiLogoff(_session, 0, standardError, 0) ;
  FreeLibrary(_MapiInst) ;
}
catch (...)
{
  erreur("Exception lors de la déconnexion MAPI.", standardError, 0) ;
}
}

bool
NSMapiCtrl::Verify()
{
  if (_session)
    return true ;
  return false ;
}

void
NSMapiCtrl::ComposeMessage(string sFromAdr, string sToAdr, string sObjet, string sCodeLettre)
{
try
{
  // Raz des paramètres
  reset() ;

  _destinataire.ulRecipClass = MAPI_TO ;
  _destinataire.lpszAddress = new char[strlen(sToAdr.c_str()) + 1] ;
  strcpy(_destinataire.lpszAddress, sToAdr.c_str()) ;

  _emetteur.lpszAddress = new char[strlen(sFromAdr.c_str()) + 1] ;
  strcpy(_emetteur.lpszAddress, sFromAdr.c_str()) ;

  _msg.lpszSubject  = new char[strlen(sObjet.c_str()) + 1] ;
  strcpy(_msg.lpszSubject, sObjet.c_str()) ;

  _msg.lpszNoteText = new char[strlen(sCodeLettre.c_str()) + 1] ;
  strcpy(_msg.lpszNoteText, sCodeLettre.c_str()) ;

  _msg.lpOriginator = &_emetteur ;
  _msg.nRecipCount  = 1 ;
  _msg.lpRecips     = &_destinataire ;
}
catch (...)
{
  erreur("Exception lors du ComposeMessage MAPI.", standardError, 0) ;
}
}

void
NSMapiCtrl::AttacheMessage(string sFileName)
{
  if ((string("") == sFileName) || _attachements.contains(sFileName))
    return ;

  _attachements.push_back(new string(sFileName)) ;
}

ULONGNSMapiCtrl::SendMail()
{
try
{
  // Manage attachements
  //
  if (false == _attachements.empty())
  {
    size_t iAttachementCount = _attachements.size() ;

    _msg.nFileCount = iAttachementCount ;
    _msg.lpFiles = new MapiFileDesc[iAttachementCount] ;

    IterString it = _attachements.begin() ;
    for (int i = 0 ; _attachements.end() != it ; it++, i++)
    {
      string sCompleteFileName = **it ;
      string sPath ;
      string sFileName ;
      NsParseFileName(sCompleteFileName, sPath, sFileName) ;

      memset(&(_msg.lpFiles[i]), 0, sizeof(MapiFileDesc)) ;

      if (string("") != sFileName)
      {
        _msg.lpFiles[i].lpszPathName = new char[strlen(sCompleteFileName.c_str()) + 1] ;
        strcpy(_msg.lpFiles[i].lpszPathName, sCompleteFileName.c_str()) ;

        // _msg.lpFiles[i].lpszFileName = new char[strlen(sFileName.c_str()) + 1] ;
        // strcpy(_msg.lpFiles[i].lpszFileName, sFileName.c_str()) ;

        _msg.lpFiles[i].nPosition = -1 ;
      }
    }
  }

  //
  //
  LPMAPISENDMAIL MapiSendMail = (LPMAPISENDMAIL) GetProcAddress(_MapiInst, "MAPISendMail") ;
  if (NULL == MapiSendMail)
  {
    string sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "errorSendingMapiMessage") + string(" : MAPISendMail function not found") ;
    _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return 0 ;
  }

  ULONG lResult = MapiSendMail(0 /*Session*/, 0 /*ulUIParam*/, &_msg, MAPI_LOGON_UI, 0) ;

  if (lResult != SUCCESS_SUCCESS)
  {
    string sErrorText = string("") ;

    if      (lResult == MAPI_E_AMBIGUOUS_RECIPIENT)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_AMBIGUOUS_RECIPIENT") ;
    else if (lResult == MAPI_E_ATTACHMENT_NOT_FOUND)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_ATTACHMENT_NOT_FOUND") ;
    else if (lResult == MAPI_E_ATTACHMENT_OPEN_FAILURE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_ATTACHMENT_OPEN_FAILURE") ;
    else if (lResult == MAPI_E_BAD_RECIPTYPE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_BAD_RECIPTYPE") ;
    else if (lResult == MAPI_E_FAILURE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_FAILURE") ;
    else if (lResult == MAPI_E_INSUFFICIENT_MEMORY)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_INSUFFICIENT_MEMORY") ;
    else if (lResult == MAPI_E_INVALID_RECIPS )
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_INVALID_RECIPS ") ;
    else if (lResult == MAPI_E_LOGIN_FAILURE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_LOGIN_FAILURE") ;
    else if (lResult == MAPI_E_TEXT_TOO_LARGE)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_TEXT_TOO_LARGE") ;
    else if (lResult == MAPI_E_TOO_MANY_FILES)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_TOO_MANY_FILES") ;
    else if (lResult == MAPI_E_TOO_MANY_RECIPIENTS)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_TOO_MANY_RECIPIENTS") ;
    else if (lResult == MAPI_E_UNKNOWN_RECIPIENT)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_UNKNOWN_RECIPIENT") ;
    else if (lResult == MAPI_E_USER_ABORT)
      sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "MAPI_E_USER_ABORT") ;

    sErrorText = _pContexte->getSuperviseur()->getText("MapiError", "errorSendingMapiMessage") + string(" : ") + sErrorText ;
    _pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;

    return lResult ;
  }

  string sMsg = string("Mail properly sent to ") + string(_msg.lpRecips[0].lpszAddress) ;
  _pContexte->getSuperviseur()->trace(&sMsg, 1, NSSuper::trDetails) ;

  return lResult ;
}
catch (...)
{
  erreur("Exception lors du SendMail MAPI.", standardError, 0) ;
  return 0 ;
}
}

// fin de nscompub.cpp
