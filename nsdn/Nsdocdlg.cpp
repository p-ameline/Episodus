#include <owl\olemdifr.h>#include <owl\applicat.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "partage\nsdivfct.h"#include "nssavoir\nspathor.h"#include "nautilus\nssuper.h"// #include "nautilus\nsannexe.h"

#include "nautilus\nsresour.h"
#include "nautilus\nshistdo.h"
#include "nsdn\nsdochis.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nspathor.h"
#include "nsbb\nsbbtran.h"
#include "nsdn\nsdn.h"
#include "nsdn\nsdocnoy.h"
#include "nsdn\nsdocdlg.h"
#include "nsdn\nsdocdlg.rh"

// -----------------------------------------------------------------------------
//
//  Méthodes de EnregDocDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(EnregDocDialog, TDialog)
  EV_COMMAND(IDOK,              CmOk),
  EV_COMMAND(IDCANCEL,          CmCancel),
  EV_COMMAND(IDC_NWDOC_CHGCHEM, CmNouvChem),
  EV_BN_CLICKED(IDC_NWDOC_HISTOR, CmHistorique),
  EV_CHILD_NOTIFY(IDC_DOC0P, BN_CLICKED, TraitementInteret0P),
  EV_CHILD_NOTIFY(IDC_DOC1P, BN_CLICKED, TraitementInteret1P),
  EV_CHILD_NOTIFY(IDC_DOC2P, BN_CLICKED, TraitementInteret2P),
  EV_CHILD_NOTIFY(IDC_DOC3P, BN_CLICKED, TraitementInteret3P),
  EV_CHILD_NOTIFY(IDC_DOC4P, BN_CLICKED, TraitementInteret4P),
  EV_WM_PAINT,
END_RESPONSE_TABLE ;

// -----------------------------------------------------------------------------// Function     : EnregDocDialog::EnregDocDialog()
// Description  : Constructeur
// -----------------------------------------------------------------------------
EnregDocDialog::EnregDocDialog(TWindow *pere, NSDocumentData *pDocuDonnees, string &sCodeChemise, string sRights, NSContexte *pCtx)
               :NSUtilDialog(pere, pCtx, "ID_LDV_NWDOC", pNSResModule)
{
try
{
	_pDocData 	         = pDocuDonnees ;
	_pCodeChemiseChoisie = &sCodeChemise ;

	NSHealthTeamMandate* pMdt = (NSHealthTeamMandate*) 0 ;

  NSHealthTeam* pHT = (NSHealthTeam*) 0 ;
  if (pCtx->getPatient())
    pHT = pCtx->getPatient()->getHealthTeam() ;

  if (pHT)
  {
    NSHealthTeamMember* pHTMember = pHT->getUserAsMember(pCtx) ;
  	if (pHTMember)
    {
    	NSHTMMandateArray aMandates ;
      pHTMember->getActiveMandates(&aMandates) ;
    	if (false == aMandates.empty())
      {
      	NSHTMMandateIter mandateIter = aMandates.begin() ;
        pMdt = *mandateIter ;
        //
        // We have to empty aMandates in order to avoid mandates deletion
        //
        for ( ; aMandates.end() != mandateIter ; )
        	aMandates.erase(mandateIter) ;
      }
    }
  }

	_sRightsString = sRights ;
	if (string("") == _sRightsString)
		_pRosace = new NSRosace(pCtx, pMdt) ;
	else
		_pRosace = new NSRosace(pCtx, _sRightsString, pMdt) ;

  _pNomDoc        = new TEdit(this, IDC_NWDOC_NOM, 1024) ;
  _pType          = new NSUtilLexique(pContexte, this, IDC_NWDOC_TYPE, pContexte->getDico()) ;
  _pDateCreation  = new NSUtilEditDateHeure(pContexte, this,  IDC_NWDOC_CREATION) ;

  _pHistorique    = new TRadioButton(this, IDC_NWDOC_HISTOR) ;
	_pChemiseBox    = new TComboBox(this, IDC_NWDOC_CHEM, 1024) ;

	_pInteretGroup  = new TGroupBox(this, IDC_NWDOC_INTGRP) ;	_pImportance    = new TVSlider(this,  IDC_DOCIMPORTANCE) ; //interêt
  _pNSSlider0P    = new TButton(this,   IDC_DOC0P) ;
  _pNSSlider1P    = new TButton(this,   IDC_DOC1P) ;
  _pNSSlider2P    = new TButton(this,   IDC_DOC2P) ;
  _pNSSlider3P    = new TButton(this,   IDC_DOC3P) ;
  _pNSSlider4P    = new TButton(this,   IDC_DOC4P) ;

	_pRosaceGroup   = new NSRosaceGroupBox(pContexte, this, IDC_ROSACE, _pRosace) ;
	_synchro_check  = new TCheckBox(this, IDC_SYNCHRO); // Checkbox pour dire que l'objet est synchronisable
  _note_check     = new TCheckBox(this, IDC_NOTE);
}
catch (...){
  erreur("Exception EnregDocDialog ctor.", standardError, 0) ;
}}

// -----------------------------------------------------------------------------
// Function     : EnregDocDialog::~EnregDocDialog()
// Description  : Destructeur, enregistre le document
// -----------------------------------------------------------------------------
EnregDocDialog::~EnregDocDialog()
{
  // Suppression de tous les objets
  delete _pNomDoc ;
  delete _pType ;
  delete _pDateCreation ;
  delete _pHistorique ;
	delete _pChemiseBox ;
	delete _pInteretGroup ;
  delete _pImportance ;
	delete _pNSSlider0P ;
  delete _pNSSlider1P ;
  delete _pNSSlider2P ;
  delete _pNSSlider3P ;
  delete _pNSSlider4P ;
	delete _pRosaceGroup	;
 	delete _synchro_check ;
  delete _note_check ;
  delete _pRosace ;
}

// -----------------------------------------------------------------------------
// Function     : EnregDocDialog::SetupWindow()
// Arguments    :	Aucun
// Description  : Initialise la boite de dialogue
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregDocDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

	// Mise du nom du document dans le contrôle Edit
	if (strspn(_pDocData->_sNom.c_str(), " ") < strlen(_pDocData->_sNom.c_str()))
		_pNomDoc->SetText(_pDocData->_sNom.c_str()) ;
	else
		_pNomDoc->SetText("") ;

  // Type
  if (string("") != _pDocData->_sTypeContenu)
    _pType->setLabel(_pDocData->_sTypeContenu) ;

	// Date de création
	_pDateCreation->setDate(_pDocData->_sDateCreation) ;

	// Ajout à l'historique
  if (true == _pDocData->estVisible())
  	_pHistorique->Check() ;
  else
  	_pHistorique->Uncheck() ;

	// Initialisation de l'importance
  _pImportance->SetRange(0, 4) ;
  _pImportance->SetRuler(1, false) ;

  int iImpo = 0 ;
  if (string("") != _pDocData->_sInteret)
  {
  	switch (_pDocData->_sInteret[0])
  	{
    	case 'E'  : iImpo = 4 ; break ;
    	case 'D'  : iImpo = 3 ; break ;
    	case 'C'  : iImpo = 2 ; break ;
    	case 'B'  : iImpo = 1 ; break ;
    	case 'A'  : iImpo = 0 ; break ;
		}
  }

	_pImportance->SetPosition(iImpo) ;

	RemplirChemises() ;
	InitChemisesBox() ;

	// Si il n'y a pas de droit, on précoche le checkbox synchro uniquement si c'est un patient global
	if ((_sRightsString == "") && (pContexte->getPatient()->IsGlobal()))
  	_synchro_check->Check() ;
  else    // Sinon on initialise en fonction de la rosace
  {
  	if (_pRosace->IsPersonnalNote())
    	_note_check->Check() ;
    if (_pRosace->IsSynchronisable())
    	_synchro_check->Check() ;
  }
}

/*
void
EnregDocDialog::EvPaint()
{
	DefaultProcessing();
    return;

	NSUtilDialog::EvPaint();

#ifdef N_TIERS

	TPaintDC Dc(*this);
    NS_CLASSLIB::TRect& rect = *(NS_CLASSLIB::TRect*)&Dc.Ps.rcPaint;
    Paint(Dc, true, rect);

#endif
}


void
EnregDocDialog::Paint(TDC& dc, bool erase, NS_CLASSLIB::TRect& RectAPeindre)
{
	NSUtilDialog::Paint(dc, erase, RectAPeindre) ;
#ifdef N_TIERS
	NS_CLASSLIB::TRect dialogRect = GetWindowRect() ;
    NS_CLASSLIB::TRect rosaceRect = pRosaceGroup->GetWindowRect() ;
    ::MapWindowPoints(HWND_DESKTOP, *this, (LPPOINT)&rosaceRect, 2);
    // if (rosaceRect.Touches(RectAPeindre))
    	pRosace->draw(&dc, &rosaceRect) ;
#endif
}
*/

// -----------------------------------------------------------------------------
// Function     : EnregDocDialog::RemplirChemises()
// Description  : Initialise la ChemiseBox
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregDocDialog::RemplirChemises()
{
try
{
	if (NULL == pContexte->getPatient())
		return ;

  pContexte->getPatient()->getFoldersArray(&_aChemisesArray) ;

/*
	NSDocumentHisto* pLibChem = pContexte->getPatient()->getDocHis()->getLibChem() ;
  if ((NULL == pLibChem) || (NULL == pLibChem->pPatPathoArray) ||
      (true == pLibChem->pPatPathoArray->empty()))
  	return ;

  string sElemLex, sSens, sType;
  string sNodeChem = "";
  string sNom = "", sDate = "";

	// on doit parcourir la librairie pour charger l'array des chemises
	PatPathoIter iter = pLibChem->pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

	while ((pLibChem->pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
  	string sSens = (*iter)->getLexiqueSens(pContexte->getSuperviseur()) ;

    if (string("0CHEM") == sSens)
    {
    	sNodeChem = (*iter)->getNode() ;
      sNom = "" ;
      sDate = "" ;
      iter++ ;

      // on charge les données de la chemise
      while ((pLibChem->pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase+1))
      {
      	sSens = (*iter)->getLexiqueSens(pContexte->getSuperviseur()) ;

        // nom de la chemise
        if (string("0INTI") == sSens)
        {
        	iter++ ;

          while ((pLibChem->pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase+2))
          {
          	// on cherche ici un texte libre
            string sElemLex = (*iter)->getLexique() ;

            if (string("£?????") == sElemLex)
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
          string sUnite  = "";
          string sFormat = "";
          string sValeur = "";
          string sTemp   = "";

          while ((pLibChem->pPatPathoArray->end() != iter) &&
                           ((*iter)->getLigne() == iLigneBase))
          {
            string sLexique = (*iter)->getLexique() ;
          	if ((string("") != sLexique) && ('£' == sLexique[0]))
            {
            	sFormat = (*iter)->getLexiqueSens(pContexte->getSuperviseur()) ;
              sValeur = (*iter)->getComplement() ;
              sUnite  = (*iter)->getUnitSens(pContexte->getSuperviseur()) ;

              iter++ ;
              break ;
            }

            iter++ ;
          }

          // sFormat est du type £D0;03
          if ((sFormat != "") && ((sFormat[1] == 'D') || (sFormat[1] == 'T')) &&
            		        (sValeur != ""))
          {
          	if (sUnite == "2DA02")
            	sDate = sValeur ;
          }
        }
        else
        	iter++ ;
      }

      if (string("") != sNom)
      {
      	NSChemiseInfo ChemInfo ;
        ChemInfo.sNodeChemise = sNodeChem ;
        strcpy(ChemInfo.pDonnees->nom, sNom.c_str()) ;
        strcpy(ChemInfo.pDonnees->creation, sDate.c_str()) ;

        pChemisesArray->push_back(new NSChemiseInfo(ChemInfo)) ;
      }
    }
    else
    	iter++ ;
  }
*/
}
catch (...)
{
  erreur("Exception RemplirChemises.", standardError, 0) ;
}
}

void
EnregDocDialog::InitChemisesBox()
{
	if (_aChemisesArray.empty())
		return ;

	string sLang = pContexte->getUserLanguage() ;

  // Filling _pChemiseBox
  //
	for (ChemInfoIter iter = _aChemisesArray.begin() ; _aChemisesArray.end() != iter ; iter++)
	{
  	string sChemLabel = (*iter)->donneIntitule(sLang) ;
		_pChemiseBox->AddString(sChemLabel.c_str()) ;
	}

	// En MUE, on sélectionne la chemise du document s'il en a une
	// (le codeDocument doit etre dans EnregDocDialog celui du meta)
	// sinon, on sélectionne par défaut la dernière chemise du patient
	NSLinkManager* pGraphe = pContexte->getPatient()->getGraphPerson()->getLinkManager() ;

	VecteurString VectString ;
	string codeDocument = _pDocData->getID() ;

	pGraphe->TousLesVrais(codeDocument, NSRootLink::docFolder, &VectString, "ENVERS") ;

	if (false == VectString.empty())
	{
  	string sNodeChemise = *(*(VectString.begin())) ;

    for (ChemInfoIter iter = _aChemisesArray.begin() ; _aChemisesArray.end() != iter ; iter++)
    {
    	if ((*iter)->_sNodeChemise == sNodeChemise)
      {
      	string sChemLabel = (*iter)->donneIntitule(sLang) ;
        _pChemiseBox->SetSelString(sChemLabel.c_str(), 0) ;
        break ;
      }
    }
	}
	else // cas par défaut : sélection de la dernière chemise du patient.
	{
  	string sChemLabel = ((_aChemisesArray)[_aChemisesArray.size() - 1])->donneIntitule(sLang) ;
    _pChemiseBox->SetSelString(sChemLabel.c_str(), 0) ;
	}
}

void
EnregDocDialog::CmHistorique()
{
  if (_pDocData->estVisible())
  {
    _pDocData->rendInvisible() ;
    _pHistorique->Uncheck() ;
  }
  else
  {
    _pDocData->rendVisible() ;
    _pHistorique->Check() ;
  }
}

// -----------------------------------------------------------------------------
// Function     : EnregDocDialog::CmOk()
// Description  : Réponse au bouton OK
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregDocDialog::CmOk()
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flêches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (_pType->getDico()->getDicoDialog()->EstOuvert())
    _pType->getDico()->getDicoDialog()->InsererElementLexique() ;

	char NomDocu[1025] ;

	// Vérifie qu'un nom de document a bien été saisi
	_pNomDoc->Transfer(NomDocu, tdGetData) ;
	if ((NomDocu[0] == '\0') || (strspn(NomDocu, " ") == strlen(NomDocu)))
	{
    string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "youMustProvideANameForTheDocument") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return ;
	}

  // Type
  //
  // Don't accept free text
  if (_pType->getCode() == string("£?????"))
  {
    string sWarningTxt = pContexte->getSuperviseur()->getText("lexiqueManagement", "freeTextNotAllowed") ;
    erreur(sWarningTxt.c_str(), warningError, 0, GetHandle()) ;
    _pType->SetFocus() ;
    return ;
  }
  _pDocData->_sTypeContenu = _pType->getCode() ;
  if (_pType->getCode() != string(""))
    pContexte->getDico()->donneCodeSens(&(_pDocData->_sTypeContenu), &(_pDocData->_sTypeContenuSens)) ;

	// Transfère le nom dans DocumentInfo
	_pDocData->_sNom = NomDocu ;

  // Transfère la date de création dans DocumentInfo
  string sDateCreation ;
  _pDateCreation->getDate(&sDateCreation) ;
  _pDocData->_sDateCreation = sDateCreation ;

	// Récupère le code de la chemise sélectionnée
	_iChemiseChoisie = _pChemiseBox->GetSelIndex() ;

  if ((_iChemiseChoisie < 0) || (size_t(_iChemiseChoisie) >= _aChemisesArray.size()))
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "youMustSelectAFolderToStoreTheDocument") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return ;
	}

	if (0 == _iChemiseChoisie)
	{
  	string sErrorText = pContexte->getSuperviseur()->getText("documentManagementErrors", "noDocumentCanBeStoredInTheRecycleBin") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
    return ;
	}

	NSChemiseInfo *pChemise = (_aChemisesArray)[_iChemiseChoisie] ;

	*_pCodeChemiseChoisie = pChemise->_sNodeChemise ;

	// Récupère l'information concernant l'Intêret
  int iImpo = _pImportance->GetPosition() ;
  switch (iImpo)
  {
    case 4  : _pDocData->_sInteret = string("E") ; break ;
    case 3  : _pDocData->_sInteret = string("D") ; break ;
    case 2  : _pDocData->_sInteret = string("C") ; break ;
    case 1  : _pDocData->_sInteret = string("B") ; break ;
    default : _pDocData->_sInteret = string("A") ;
	}

	if ((_note_check->GetCheck()) == BF_CHECKED)    // On verifie que c'est une note personnelle
		_pRosaceGroup->pRosace->setPersonnalNote(true) ;
	else
  	_pRosaceGroup->pRosace->setPersonnalNote(false) ;

	if ((_synchro_check->GetCheck()) == BF_CHECKED)   // On verifie que le document est synchronisable
		_pRosaceGroup->pRosace->setSynchronalisable(true) ;
	else
		_pRosaceGroup->pRosace->setSynchronalisable(false) ;

	_pDocData->_sRights = _pRosaceGroup->getRigthsString() ;

	// Effectue le CmOk
	TDialog::CmOk() ;
}

// -----------------------------------------------------------------------------
// Function     : EnregDocDialog::CmCancel()
// Description  : Réponse au bouton Cancel
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregDocDialog::CmCancel()
{
	string sRights = _pRosaceGroup->getRigthsString() ;

	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------------------
//  Function: EnregDocDialog::CmNouvChem()
//  Description: Création d'une nouvelle chemise par défaut
//  Returns:	  Rien
// -----------------------------------------------------------------------------
void
EnregDocDialog::CmNouvChem()
{
  string sNomChem ;

  NomChemiseDialog  *pNomChemDlg = new NomChemiseDialog(pContexte->GetMainWindow(), pContexte) ;
  if (pNomChemDlg->Execute() == IDCANCEL)
  {
    delete pNomChemDlg ;
    return ;
  }
  sNomChem = pNomChemDlg->sNomChem ;
  delete pNomChemDlg ;

	// En MUE on constitue un docnoy à partir du document pLibChem
	// pour modifier sa patpatho. On doit ensuite remettre pLibChem à jour
  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  if (NULL == pContexte->getPatient()->getDocHis())
    return ;

  NSDocumentHisto* pLibChem = pContexte->getPatient()->getDocHis()->getLibChem() ;
  if (NULL == pLibChem)
    return ;

	NSDocumentInfo Docum(pContexte) ;

  NSDocumentData InfoData ;
  pLibChem->initFromData(&InfoData) ;
  Docum.setData(&InfoData) ;

	// constitution de la patpatho correspondant à la nouvelle chemise
	NSPatPathoArray PatPatho(pContexte->getSuperviseur()) ;

	// ajout de la nouvelle chemise
	PatPatho.ajoutePatho("0CHEM1", 0) ;

	// Intitulé : nom de la chemise
	PatPatho.ajoutePatho("0INTI1", 1) ;
	Message Msg ;
	Msg.SetTexteLibre(sNomChem.c_str()) ;
	PatPatho.ajoutePatho("£?????", &Msg, 2) ;

	// Date d'ouverture
	PatPatho.ajoutePatho("KOUVR1", 1) ;
	Msg.Reset() ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(tpsNow.donneDateHeure()) ;
	PatPatho.ajoutePatho("£T0;19", &Msg, 2) ;

  NSPatPathoArray PptChem(pContexte->getSuperviseur()) ;
  pLibChem->initFromPatPatho(&PptChem) ;

  // If the Folder Library is empty, we have to set the root concept
  if (true == PptChem.empty())
    PptChem.ajoutePatho("0LIBC1", 0) ;

  // Insertion en queue (iter doit être ici egal à end())
  PptChem.InserePatPatho(PptChem.end(), &PatPatho, 1) ;

  NSNoyauDocument Noyau((TDocument*) 0, &Docum, 0, pContexte, false, true) ;
  Noyau.setPatPatho(&PptChem) ;

	// Enregistrement du document modifié
	Noyau.enregistrePatPatho() ;

	// Mise à jour de l'historique
	/*bool bReload =*/ Noyau.chargePatPatho() ;

  // on rafraichit en mémoire le document pLibChem
	Noyau.initFromPatPatho(&PptChem) ;
  pLibChem->setPatPatho(&PptChem) ;

  // on vide la liste si elle contient des items
  if (_pChemiseBox->GetCount())
    _pChemiseBox->ClearList() ;

  // on vide le tableau
  _aChemisesArray.vider() ;

  RemplirChemises() ;

  if (_aChemisesArray.empty())
    return ;

	string sLang      = pContexte->getUserLanguage() ;
  string sChemLabel = string("") ;

  for (ChemInfoIter iter = _aChemisesArray.begin() ; _aChemisesArray.end() != iter ; iter++)
  {
		sChemLabel = (*iter)->_Donnees.donneIntitule(sLang) ;
		_pChemiseBox->AddString(sChemLabel.c_str()) ;
  }

  // On sélectionne par défaut la dernière chemise (la nouvelle)
  //
  _pChemiseBox->SetSelString(sChemLabel.c_str(), 0) ;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void
EnregDocDialog::TraitementInteret0P()
{
	_pImportance->SetPosition(0) ;
}

void
EnregDocDialog::TraitementInteret1P()
{
	_pImportance->SetPosition(1) ;
}

void
EnregDocDialog::TraitementInteret2P()
{
	_pImportance->SetPosition(2) ;
}

void
EnregDocDialog::TraitementInteret3P()
{
	_pImportance->SetPosition(3) ;
}

void
EnregDocDialog::TraitementInteret4P()
{
	_pImportance->SetPosition(4) ;
}

// -----------------------------------------------------------------------------
//
//  Méthodes de NomChemiseDialog
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(NomChemiseDialog, TDialog)
  EV_BN_CLICKED(IDC_NOMCHEM_DEFAUT, CmClickChemDefaut),
  EV_BN_CLICKED(IDC_NOMCHEM_NEW,    CmClickChemNew),
  EV_EN_CHANGE(IDC_NOMCHEM_EDIT, CmChangeNomChem),
  EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE ;


NomChemiseDialog::NomChemiseDialog(TWindow *pere, NSContexte *pCtx)
                 :NSUtilDialog(pere, pCtx, "IDD_NOMCHEM")
{
  pNomChem 	  = new TEdit(this, IDC_NOMCHEM_EDIT, CHE_NOM_LEN) ;
  pChemDefaut = new TRadioButton(this, IDC_NOMCHEM_DEFAUT, 0) ;
  pChemNew 	  = new TRadioButton(this, IDC_NOMCHEM_NEW, 0) ;
  sNomChem    = "" ;
}


NomChemiseDialog::~NomChemiseDialog()
{
  delete pNomChem ;
  delete pChemDefaut ;
  delete pChemNew ;
}


void
NomChemiseDialog::SetupWindow()
{
  TDialog::SetupWindow() ;
  pChemDefaut->Uncheck() ;
  pChemNew->Check() ;
  pNomChem->SetFocus() ;
}


void
NomChemiseDialog::CmClickChemNew()
{
  pChemDefaut->Uncheck() ;
}


void
NomChemiseDialog::CmClickChemDefaut()
{
  pChemNew->Uncheck() ;
}


void
NomChemiseDialog::CmChangeNomChem()
{
	pChemNew->Check() ;
  pChemDefaut->Uncheck() ;
}


void
NomChemiseDialog::CmOk()
{
	char far cfNomChem[CHE_NOM_LEN + 1] ;

  pNomChem->GetText(cfNomChem,CHE_NOM_LEN) ;
	sNomChem = string(cfNomChem) ;

	if ((pChemNew->GetCheck()) == BF_CHECKED)
  {
    if (sNomChem == "")
    {
      MessageBox("Erreur : Vous n'avez pas saisi de nom pour la nouvelle chemise.") ;
      return ;
    }
  }

  if ((pChemDefaut->GetCheck()) == BF_CHECKED)
    sNomChem = "défaut" ;

  TDialog::CmOk() ;
}


void
NomChemiseDialog::CmCancel()
{
  TDialog::CmCancel() ;
}



// -----------------------------------------------------------------------------
//
//  Méthodes de CréerSejourDialog
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(CreerSejourDialog, TDialog)
  EV_COMMAND(IDOK,      CmOk),
  EV_COMMAND(IDCANCEL,  CmCancel),
END_RESPONSE_TABLE ;


CreerSejourDialog::CreerSejourDialog(TWindow *pere, NSContexte *pCtx, NSSejourInfo *pVenue, string sDefaultVenueId)
                  :NSUtilDialog(pere, pCtx, "IDD_CREAT_SEJOUR")
{
try
{
  _pNumSejour = new TEdit(this, IDC_SEJ_NUM,  CSEJ_NUMERO_LEN) ;
  _pNumUF     = new TEdit(this, IDC_SEJ_UF,   CSEJ_UNITE_LEN) ;
  _pDateDeb   = new NSUtilEditDateHeure(pContexte, this, IDC_SEJ_DEBUT) ;
  _pDateFin   = new NSUtilEditDateHeure(pContexte, this, IDC_SEJ_FIN) ;

  sNumUF = sDefaultVenueId ;

  _pEditedVenue = pVenue ;
}
catch (...)
{
  erreur("Exception (new CreerSejourDialog)", standardError, 0) ;
}
}

CreerSejourDialog::~CreerSejourDialog()
{
	delete _pNumSejour ;
  delete _pNumUF ;
  delete _pDateDeb ;
  delete _pDateFin ;
}

void
CreerSejourDialog::SetupWindow()
{
  TDialog::SetupWindow() ;

  if (_pEditedVenue->getTreeID() == string(""))
	{
  	_pNumUF->SetText(sNumUF.c_str()) ;
  	_pNumSejour->SetFocus() ;

    NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;

    tpsNow.placeHeures(0) ;
    tpsNow.placeMinutes(0) ;
    tpsNow.placeSecondes(0) ;

    _pDateDeb->setDate(tpsNow.donneDateHeure()) ;

  	tpsNow.placeHeures(23) ;
    tpsNow.placeMinutes(59) ;
    tpsNow.placeSecondes(0) ;

    _pDateFin->setDate(tpsNow.donneDateHeure()) ;

    return ;
  }

	_pNumUF->SetText(_pEditedVenue->getUnit().c_str()) ;
	_pNumSejour->SetText(_pEditedVenue->getNumero().c_str()) ;

	_pDateDeb->setDate(_pEditedVenue->getBeginDate()) ;
	_pDateFin->setDate(_pEditedVenue->getEndDate()) ;
}

void
CreerSejourDialog::CmOk()
{
try
{
  int      iBuffLen = _pNumSejour->GetTextLen() ;
  char far *szBuff  = new char[iBuffLen + 1] ;
  _pNumSejour->GetText(szBuff, iBuffLen + 1) ;
  string sNumSej = string(szBuff) ;
  delete[] szBuff ;

  if (string("") == sNumSej)
  {
    MessageBox("Erreur : Vous n'avez pas saisi de numéro de séjour.") ;
    return ;
  }

  iBuffLen = _pNumSejour->GetTextLen() ;
  szBuff  = new char[iBuffLen + 1] ;
  _pNumUF->GetText(szBuff, iBuffLen + 1) ;
  string sNumUF = string(szBuff) ;
  delete[] szBuff ;

  _pEditedVenue->setNumero(sNumSej) ;
  _pEditedVenue->setUnit(sNumUF) ;
  _pEditedVenue->setCarePlace(pContexte->getSuperviseur()->getCarePlaceId()) ;

  string sDateDeb ;
  _pDateDeb->getDate(&sDateDeb) ;
  _pEditedVenue->setBeginDate(sDateDeb) ;
  string sDateFin ;
  _pDateFin->getDate(&sDateFin) ;
  _pEditedVenue->setEndDate(sDateFin) ;

  TDialog::CmOk() ;
}
catch (...)
{
  erreur("Exception (CreerSejourDialog::CmOk)", standardError, 0) ;
}
}

void
CreerSejourDialog::CmCancel()
{
  TDialog::CmCancel() ;
}

// -----------------------------------------------------------------------------
//
//      Méthodes de ListeSejoursDialog
//
// -----------------------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ListeSejoursDialog, NSUtilDialog)
  EV_COMMAND(IDC_SEJOURS_SUPPR, CmSupprimer),
  EV_COMMAND(IDC_SEJOURS_CREER, CmCreer),
  EV_COMMAND(IDC_SEJOURS_MODIF, CmModifier),
  EV_COMMAND(IDOK,              CmOk),
  EV_COMMAND(IDCANCEL,          CmCancel),
  EV_LBN_SELCHANGE(IDC_SEJOURS_LISTE, CmSelectSejour),
	EV_LBN_DBLCLK(IDC_SEJOURS_LISTE, CmSejourDblClk),
END_RESPONSE_TABLE ;


ListeSejoursDialog::ListeSejoursDialog(TWindow *pere, NSContexte *pCtx, TModule *module)
  : NSUtilDialog(pere, pCtx, "IDD_SEJOURS", module)
{
try
{
	pListe 	= new TListBox(this, IDC_SEJOURS_LISTE, module) ;
  sEnCours = "" ;
}
catch (...)
{
  erreur("Exception (new ListeSejoursDialog)", standardError, 0) ;
}
}


ListeSejoursDialog::~ListeSejoursDialog()
{
  delete pListe ;
}


void
ListeSejoursDialog::SetupWindow()
{
	TDialog::SetupWindow() ;
  AfficheListe() ;
}


void
ListeSejoursDialog::AfficheListe()
{
try
{
  // on vide la liste si elle contient des items
  if (pListe->GetCount())
    pListe->ClearList() ;

#ifndef N_TIERS

  // On passe en revue tous les séjours du patient
  NSSejour *pSejour = new NSSejour(pContexte) ;

	// Ouverture du fichier
	pSejour->lastError = pSejour->open() ;
	if (pSejour->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier des séjours.", standardError, pSejour->lastError, GetHandle()) ;
		delete pSejour ;
		return ;
	}

  string  codeSejour ;
	char    PremOrdre[CSEJ_CODE_LEN + 1] ;

	// Préparation du compteur maxi
  int i ;
  for (i = 0 ; i < CSEJ_CODE_LEN ; i++)
    PremOrdre[i] = ' ' ;
	PremOrdre[i] = '\0' ;

  codeSejour = string(pContexte->getPatient()->pDonnees->nss) + string(PremOrdre) ;

	pSejour->lastError = pSejour->chercheClef(&codeSejour, "", 0, keySEARCHGEQ, dbiWRITELOCK) ;
	// Fin de fichier -> pas de séjour
	if (pSejour->lastError == DBIERR_EOF)
  {
    delete pSejour ;
		return ;
  }

	// Toute autre erreur est anormale
	if (pSejour->lastError != DBIERR_NONE)
	{
		erreur("Le fichier des séjours est endommagé.", standardError, pSejour->lastError) ;
		delete pSejour ;
		return ;
	}

	// On récupère l'enregistrement
	pSejour->lastError = pSejour->getRecord() ;
	if (pSejour->lastError != DBIERR_NONE)
	{
		erreur("Le fichier des séjours est endommagé.", standardError, pSejour->lastError) ;
		delete pSejour ;
		return ;
	}

	// Tant que le séjour trouvé appartient au patient, on remplit la liste
	while ( (pSejour->lastError == DBIERR_NONE) &&
          (strcmp(pSejour->pDonnees->nss, pContexte->getPatient()->pDonnees->nss) == 0))
	{
    pListe->AddString(pSejour->pDonnees->numero) ;

		pSejour->lastError = pSejour->suivant(dbiWRITELOCK) ;

    if (pSejour->lastError == DBIERR_NONE)
    {
			pSejour->lastError = pSejour->getRecord() ;
      if (pSejour->lastError != DBIERR_NONE)
      {
        erreur("Le fichier des séjours est endommagé.", standardError, pSejour->lastError) ;
        delete pSejour ;
        return ;
      }
    }
    else if (pSejour->lastError != DBIERR_EOF)
    {
      erreur("Le fichier des séjours est endommagé.", standardError, pSejour->lastError) ;
      delete pSejour ;
      return ;
    }
  }
  delete pSejour ;

#endif

  return ;
}
catch (...)
{
  erreur("Exception (AfficheListe)", standardError, 0) ;
}
}


void
ListeSejoursDialog::CmSelectSejour()
{
  char szNumero[CSEJ_NUMERO_LEN + 1] ;
  int iSel = pListe->GetSelString(szNumero, CSEJ_NUMERO_LEN) ;
  if (iSel > 0)
    sEnCours = string(szNumero) ;
  else
    sEnCours = "" ;
}


void
ListeSejoursDialog::CmSejourDblClk()
{
	char szNumero[CSEJ_NUMERO_LEN + 1] ;
  int iSel = pListe->GetSelString(szNumero, CSEJ_NUMERO_LEN) ;
  if (iSel > 0)
  {
    sEnCours = string(szNumero) ;
    CmModifier() ;
  }
  else
    sEnCours = "" ;
}


void
ListeSejoursDialog::CmOk()
{
  TDialog::CmOk() ;
}


void
ListeSejoursDialog::CmCancel()
{
  TDialog::CmCancel() ;
}


void
ListeSejoursDialog::CmSupprimer()
{
  AfficheListe() ;
}


void
ListeSejoursDialog::CmCreer()
{
#ifndef N_TIERS
  CreerSejourDialog *pCreerSejour = new CreerSejourDialog(this, pContexte) ;
  if (pCreerSejour->Execute() == IDOK)
    AfficheListe() ;
  delete pCreerSejour ;
#endif
}


void
ListeSejoursDialog::CmModifier()
{
    AfficheListe();
}


/*

// -----------------------------------------------------------------------------
//
//  Méthodes de EnregChemDialog
//
// -----------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(EnregChemDialog, TDialog)
	EV_COMMAND(IDOK, CmOk),
END_RESPONSE_TABLE ;


// -----------------------------------------------------------------------------
// Function     : EnregChemDialog::EnregChemDialog()
// Description  : Constructeur
// -----------------------------------------------------------------------------
EnregChemDialog::EnregChemDialog(TWindow *pere, NSChemiseData *pChemDonnees, char *CodeArmoi, NSContexte *pCtx)
  : NSUtilDialog(pere, pCtx, "IDD_NWCHEM")
{
try
{
	pChemData 	        = pChemDonnees ;

	pCodeArmoireChoisie = CodeArmoi ;
	pNomChem 	       	= new TEdit(this, IDC_NWCHEM_NOM, 35) ;
	pArmoireBox         = new TComboBox(this, IDC_NWCHEM_ARMOI, 35) ;
	pCommentaire        = new TEdit(this, IDC_NWCHEM_COMMENT) ;
}
catch (...)
{
    erreur("Exception EnregChemDialog ctor", standardError, 0);
    return false ;
}
}


// -----------------------------------------------------------------------------
// Function     : EnregChemDialog::~EnregChemDialog()
// Description  : Destructeur, enregistre le document
// -----------------------------------------------------------------------------
EnregChemDialog::~EnregChemDialog()
{
	// Suppression de tous les objets
	delete pNomChem ;
	delete pArmoireBox ;
	delete pCommentaire ;
}


// -----------------------------------------------------------------------------
// Function     : EnregChemDialog::SetupWindow()
// Arguments    :	Aucun
// Description  : Initialise la boite de dialogue
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregChemDialog::SetupWindow()
{
	char 	chAffiche[200], dateAffiche[20], *test ;

	TDialog::SetupWindow() ;

	// Mise du nom du document dans le contrôle Edit
	if (strspn(pChemData->nom, " ") < strlen(pChemData->nom))
		pNomChem->Transfer(pChemData->nom, tdSetData) ;
	else
		pNomChem->Transfer("", tdSetData) ;

	// Remplissage de ArmoireBox avec les armoires
	NSArmoire *pArmoire = new NSArmoire(pContexte) ;

	// Ouverture du fichier
	pArmoire->lastError = pArmoire->open() ;
	if (pArmoire->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier des armoires.", standardError, pArmoire->lastError, GetHandle()) ;
		delete pArmoire ;
    pArmoire = 0 ;
		return ;
	}

	// Recherche de la première armoire du patient
	char codeArmoire[ARM_CODE_LEN + 1] ;
	memset(codeArmoire, ' ', ARM_CODE_LEN) ;
	pArmoire->lastError = pArmoire->chercheClef((unsigned char *)codeArmoire, "", 0, keySEARCHGEQ, dbiWRITELOCK) ;
	if (pArmoire->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la recherche de la 1ère armoire.", standardError, pArmoire->lastError, GetHandle()) ;
		pArmoire->close() ;
		delete pArmoire ;
    pArmoire = 0 ;
		return ;
	}
	pArmoire->lastError = pArmoire->getRecord() ;
	if (pArmoire->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la lecture de la 1ère armoire.", standardError, pArmoire->lastError, GetHandle()) ;
		pArmoire->close() ;
		delete pArmoire ;
    pArmoire = 0 ;
		return ;
	}
	while (pArmoire->lastError == DBIERR_NONE)
	{
		// Ajout de l'armoire à la ArmoireBox
		pArmoireBox->AddString(pArmoire->pDonnees->nom) ;
//		pArmoiresArray->Add(*new NSArmoireInfo(pArmoire)) ;
		pArmoiresArray->push_back(new NSArmoireInfo(pArmoire)) ;

		// Prise de l'armoire suivante
		pArmoire->lastError = pArmoire->suivant(dbiWRITELOCK) ;
		if ((pArmoire->lastError != DBIERR_NONE) && (pArmoire->lastError != DBIERR_EOF))
			erreur("Erreur à l'accès à l'armoire suivante.", standardError, pArmoire->lastError, GetHandle()) ;
		else
		{
			pArmoire->lastError = pArmoire->getRecord() ;
			if (pArmoire->lastError != DBIERR_NONE)
				erreur("Erreur à la lecture de l'armoire.", standardError, pArmoire->lastError, GetHandle()) ;
		}
	}
	pArmoire->close() ;
	delete pArmoire ;
  pArmoire = 0 ;
	if (creation)
   	return ;

	// Affichage de l'armoire contenant la chemise
	NSArmChem *pArmChem = new NSArmChem(pContexte) ;
	pArmChem->lastError = pArmChem->open() ;
	if (pArmChem->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier Chem/Doc.", standardError, pArmChem->lastError, GetHandle()) ;
		delete pArmChem ;
		return ;
	}

	// Recherche du document
	//char codeChemise[ACH_CHEMISE_LEN + 1];
	string codeChemise = string(pChemData->code) ;
	pArmChem->lastError = pArmChem->chercheClef(&codeChemise, "CHEMISE", NODEFAULTINDEX, keySEARCHEQ, dbiWRITELOCK) ;
	if (pArmChem->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la recherche de la chemise dans Arm/Chem.", standardError, pArmChem->lastError, GetHandle()) ;
		pArmChem->close() ;
		delete pArmChem ;
		return ;
	}
	pArmChem->lastError = pArmChem->getRecord() ;
	if (pArmChem->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la lecture dans Arm/Chem.", standardError, pArmChem->lastError, GetHandle()) ;
		pArmChem->close() ;
		delete pArmChem ;
		return ;
	}

	// Recherche dans pChemisesArray de la chemise correspondante

	NSArmoireInfo *pArm ;
  int				    i ;
	pArm = &((*pArmoiresArray)[0]) ;
	for (i = 0 ; (i < pArmoiresArray->ArraySize()) && (strcmp(pArm->pDonnees->code, pArmChem->pDonnees->chemise) != 0) ; i++)
    pArm = &((*pArmoiresArray)[i]) ;
	if (i < pArmoiresArray->ArraySize())
		pArmoireBox->SetSelString(pArm->pDonnees->nom, 0) ;


  ArmoiInfoIter   iter, pArm ;
	pArm = pArmoiresArray->begin() ;
  for (iter = pArmoiresArray->begin() ; (iter != pArmoiresArray->end()) && (strcmp((*pArm)->pDonnees->code, pArmChem->pDonnees->chemise) != 0) ; iter++)
  {
    if ( iter != pArmoiresArray->end())
    {
      (*iter)->pDonnees->donneIntitule(chAffiche) ;
      pArmoireBox->SetSelString((*iter)->pDonnees->nom, 0) ;
    }
   }

}


// -----------------------------------------------------------------------------
// Function     : EnregChemDialog::CmOk()
// Description  : Réponse au bouton OK
// Returns      :	Rien
// -----------------------------------------------------------------------------
void
EnregChemDialog::CmOk()
{
    char 	NomChem[36], *test, Interet[2] ;

	// Vérifie qu'un nom de chemise a bien été saisi
	pNomChem->Transfer(NomChem, tdGetData) ;
	if ((NomChem[0] == '\0') || (strspn(NomChem, " ") == strlen(NomChem)))
	{
		erreur("Vous devez saisir un nom de chemise.", warningError, 0, GetHandle()) ;
		return ;
	}

	// Transfère le nom dans DocumentInfo
	strcpy(pChemData->nom, NomChem) ;

	// Récupère le code de l'armoire sélectionnée
	ArmoireChoisie = pArmoireBox->GetSelIndex() ;
	if ((ArmoireChoisie < 0) || (size_t(ArmoireChoisie) >= pArmoiresArray->size()))
	{
		erreur("Vous devez choisir une armoire pour abriter cette chemise.", warningError, 0, GetHandle()) ;
		return ;
	}
	NSArmoireInfo *Armoire = (*pArmoiresArray)[ArmoireChoisie] ;
	strcpy(pCodeArmoireChoisie, Armoire->pDonnees->code) ;

	// Effectue le CmOk
	TDialog::CmOk() ;
}

*/

