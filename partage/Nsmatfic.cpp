//---------------------------------------------------------------------------//    NSMATFIC.CPP
//    PA   juillet 95
//  Implémentation des objets materiel
//---------------------------------------------------------------------------
#include <owl\applicat.h>
#include <owl\decmdifr.h>
#include <owl\docmanag.h>
#include <owl\olemdifr.h>

#include <owl\applicat.h>
#include <owl\window.h>
#include <owl\dialog.h>
#include <owl\groupbox.h>
#include <owl\listbox.h>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>

#include "partage\nsglobal.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfct.h"
#include "nsoutil\nsoutil.h"
#include "nssavoir\ns_medic.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsednum.h"

#include "partage\nsmatfic.h"
#include "partage\nsmatfic.rh"

#include "pilot\Pilot.hpp"
#include "pilot\JavaSystem.hpp"
#include "nsbb\tagNames.h"
//***************************************************************************
// 				Implémentation des méthodes Materiel
//***************************************************************************

//---------------------------------------------------------------------------
//  Function:    NSMaterielData::NSMaterielData()
//
//  Description: Met à blanc les variables de la fiche
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSMaterielData::metAZero()
{
  _sObjectID = string("") ;

  _sType        = string("") ;
  _sUtilisateur = string("") ;
  _sCode        = string("") ;
  _sLibelle     = string("") ;
  _sNum_serie   = string("") ;
  _sMise_svce   = string("") ;
  _sActif       = string("") ;

  _sLibelle_complet = string("") ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielData::NSMaterielData(NSMaterielData& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMaterielData::NSMaterielData(const NSMaterielData& rv)
{
  _sObjectID    = rv._sObjectID ;

	_sType        = rv._sType ;
  _sUtilisateur = rv._sUtilisateur ;
  _sCode        = rv._sCode ;
  _sLibelle     = rv._sLibelle ;
  _sNum_serie   = rv._sNum_serie ;
  _sMise_svce   = rv._sMise_svce ;
  _sActif       = rv._sActif ;

	_sLibelle_complet = rv._sLibelle_complet ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielData::operator=(NSMaterielData src)
//  Description:	Opérateur =
//  Retour:			Référence sur l'objet cible
//---------------------------------------------------------------------------
NSMaterielData& NSMaterielData::operator=(const NSMaterielData& src)
{
	if (this == &src)
		return *this ;

	_sObjectID    = src._sObjectID ;

	_sType        = src._sType ;
  _sUtilisateur = src._sUtilisateur ;
  _sCode        = src._sCode ;
  _sLibelle     = src._sLibelle ;
  _sNum_serie   = src._sNum_serie ;
  _sMise_svce   = src._sMise_svce ;
  _sActif       = src._sActif ;

	_sLibelle_complet = src._sLibelle_complet ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielData::operator=(NSMaterielData src)
//  Description:	Opérateur de comparaison
//  Retour:			0 ou 1
//---------------------------------------------------------------------------
int
NSMaterielData::operator==(const NSMaterielData& o) const
{
	if (_sObjectID == o._sObjectID)
		return 1 ;
	else
		return 0 ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielInfo::NSMaterielInfo()
//  Description:	Constructeur par défaut
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMaterielInfo::NSMaterielInfo()
{
	_pPatPathoArray = (NSPatPathoArray*) 0 ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielInfo::~NSMaterielInfo()
//  Description:	Destructeur
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMaterielInfo::~NSMaterielInfo()
{
	if (_pPatPathoArray)
		delete _pPatPathoArray ;
}

string
NSMaterielInfo::donneComplement()
{
	return _Donnees._sObjectID ;
}

bool
NSMaterielInfo::initialiseDepuisComplement(string sComplement)
{
	int iTaille = strlen(sComplement.c_str()) ;
	if (iTaille < MAT_TYPE_LEN)
		return false ;

	_Donnees._sType = string(sComplement, 0, MAT_TYPE_LEN) ;

	if (iTaille >= MAT_TYPE_LEN + MAT_UTILISATEUR_LEN)
    _Donnees._sUtilisateur = string(sComplement, MAT_TYPE_LEN, MAT_UTILISATEUR_LEN) ;

	if (iTaille >= MAT_TYPE_LEN + MAT_UTILISATEUR_LEN + MAT_CODE_LEN)
  	_Donnees._sCode = string(sComplement, MAT_TYPE_LEN + MAT_UTILISATEUR_LEN, MAT_CODE_LEN) ;

	return true ;
}

void
NSMaterielInfo::initFromPatpatho(NSSuper* pSuper, string sActif)
{
  reset() ;

  if ((NULL == pSuper) || (NULL == _pPatPathoArray) || _pPatPathoArray->empty())
    return ;

  PatPathoIter iter = _pPatPathoArray->begin() ;

  string sSens = (*iter)->getLexiqueSens() ;

  _Donnees._sType  = sSens ;
  _Donnees._sActif = sActif ;

  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((_pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
  	sSens = (*iter)->getLexiqueSens() ;

    // numéro de série
    if (string("LNUMS") == sSens)
      _Donnees._sNum_serie = _pPatPathoArray->getFreeTextValue(&iter) ;

    // libelle
    else if (string("LNOMA") == sSens)
      _Donnees._sLibelle = _pPatPathoArray->getFreeTextValue(&iter) ;

    // date de mise en service
    else if (string("KSERV") == sSens)
    {
    	iter++ ;
      // int iLigneBase = (*iter)->pDonnees->getLigne() ;

      string sUnite  = (*iter)->getUnitSens() ;
      string sFormat = (*iter)->getLexiqueSens() ;
      string sValeur = (*iter)->getComplement() ;

    	if (string("2DA02") == sUnite)
      {
      	if (strlen(sValeur.c_str()) > MAT_MISE_SVCE_LEN)
        	sValeur = string(sValeur, 0, MAT_MISE_SVCE_LEN) ;

      	_Donnees._sMise_svce = sValeur ;
      }

      iter++ ;
    }
    else
    	iter++ ;
  }
}

bool
NSMaterielInfo::initialiseDepuisObjet(NSSuper* pSuper, string sObjId, string actif)
{
  reset() ;

  if (((NSSuper*) NULL == pSuper) || (string("") == sObjId))
    return false ;

  // ------------ Appel du pilote
  //
  NSDataGraph Graph(pSuper, graphObject) ;
  NSBasicAttributeArray AttrArray ;
	AttrArray.push_back(new NSBasicAttribute(OBJECT, sObjId)) ;

  bool res = pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(),
                                  &Graph,  &AttrArray) ;
  if (false == res)
  {
		string sErrorText = pSuper->getText("NTIERS", "materialNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return false ;
  }

  if (Graph.getTrees()->empty())
		return false ;

	if (NULL == _pPatPathoArray)
		_pPatPathoArray = new NSPatPathoArray(pSuper, graphObject) ;

	NSDataTreeIter iterTree = Graph.getTrees()->begin() ;
	(*iterTree)->getRawPatPatho(_pPatPathoArray) ;

  initFromPatpatho(pSuper, actif) ;

  _Donnees._sObjectID        = sObjId ;
  _Donnees._sLibelle_complet = _Donnees._sLibelle ;

  return true ;
}

bool
NSMaterielInfo::initialiseDepuisSN(NSSuper* pSuper, string sObjId, string actif)
{
  reset() ;

  if ((NULL == pSuper) || (string("") == sObjId))
    return false ;

try
{
	VecteurString FoundObjects ;

	// on récupère la liste des matériels : actifs et non-actifs
	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;
	AttrList.push_back(new NSBasicAttribute("_OMATE_LACTF", actif)) ;
	AttrList.push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

	bool res = pSuper->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
	if (false == res)
	{
		string sErrorText = pSuper->getText("NTIERS", "objectNotFound") ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

	if (ObjList.empty())
    return false ;

  for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
  {
    string sOIDS = (*k)->getAttributeValue(OIDS) ;

    if (initialiseDepuisObjet(pSuper, sOIDS, actif))
		{
			if (getSN() == sObjId)
        return true ;
		}
  }
}
catch (...)
{
	erreur("Exception ListeMaterielDialog::InitMatArrayByActivity.", standardError, 0) ;
	return false ;
}
  return false ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielInfo::NSMaterielInfo(NSMaterielInfo& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMaterielInfo::NSMaterielInfo(const NSMaterielInfo& rv)
{
	_Donnees = rv._Donnees ;

	if (rv._pPatPathoArray)
  	_pPatPathoArray = new NSPatPathoArray(*(rv._pPatPathoArray)) ;
	else
  	_pPatPathoArray = (NSPatPathoArray*) 0 ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielInfo::operator=(NSMaterielInfo src)
//  Description:	Opérateur d'affectation
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
NSMaterielInfo&
NSMaterielInfo::operator=(const NSMaterielInfo& src)
{
	if (this == &src)
		return *this ;

	_Donnees = src._Donnees ;

	if (src._pPatPathoArray)
	{
  	if (_pPatPathoArray)
    	*_pPatPathoArray = *(src._pPatPathoArray) ;
    else
    	_pPatPathoArray = new NSPatPathoArray(*(src._pPatPathoArray)) ;
  }
  else
  {
  	if (_pPatPathoArray)
    	delete _pPatPathoArray ;
    _pPatPathoArray = (NSPatPathoArray*) 0 ;
	}

	return *this ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielInfo::operator==(NSMaterielInfo src)
//  Description:	Opérateur de comparaison
//  Retour:			Référence de l'objet cible
//---------------------------------------------------------------------------
int
NSMaterielInfo::operator==(const NSMaterielInfo& o) const
{
	return (_Donnees == o._Donnees) ;
}

string
NSMaterielInfo::getLexiqueLabel(NSSuper* pSuper)
{
  if ((NULL == pSuper) || (string("") == getType()))
    return string("") ;

  string sLexiqueSens = getType() ;
  NSDico::donneCodeComplet(sLexiqueSens) ;
  return sLexiqueSens ;
}

string
NSMaterielInfo::getLabelAndSN()
{
  string sNom = getFullLabel() ;
  if (string("") == sNom)
    return string("") ;

  // Remove trailing ' ' and '.'
  strip(sNom, string(" ."), stripRight) ;

  string sNumSerie = getSN() ;
  if (string("") != sNumSerie)
    sNom += " n°" + sNumSerie ;

  // Remove trailing ' ' and '.'
  strip(sNom, string(" ."), stripRight) ;

  return sNom ;
}

//---------------------------------------------------------------------------
//  Fonction:		NSMaterielArray(NSMaterielArray& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSMaterielArray::NSMaterielArray(const NSMaterielArray& rv)
                :NSMatInfoArray()
{
try
{
	if (false == rv.empty())
		for (MatosConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSMaterielInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSMaterielArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSMaterielArray::~NSMaterielArray()
{
	vider() ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSMaterielArray::vider()
{
	if (empty())
		return ;

	for (MatosIter i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
	}
}

// -----------------------------------------------------------------
//
//  Méthodes de CreerMaterielDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(CreerMaterielDialog, NSUtilDialog)
	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

CreerMaterielDialog::CreerMaterielDialog(TWindow* pere, NSContexte* pCtx, NSMaterielArray* pMaterielArray,
                                         bool bMode, TModule* /* mod */)
                    :NSUtilDialog(pere, pCtx, "IDD_CREAT_MAT", pNSResModule)
{
try
{
  pLibelle  = new NSUtilEdit2(pContexte, this, IDC_CM_LIBELLE, MAT_LIBELLE_LEN) ;
  pType     = new NSUtilLexique(pContexte, this, IDC_CM_TYPE, pCtx->getDico()) ;
  pNumSerie = new NSUtilEdit2(pContexte, this, IDC_CM_NUM_SERIE, MAT_NUM_SERIE_LEN) ;
  pMiseSvce = new NSUtilEditDate(pContexte, this, IDC_CM_MISE_SVCE) ;
  pActif    = new TGroupBox(this, IDC_CM_ACTIF) ;
  pActifOui = new TRadioButton(this, IDC_CM_ACTIF_OUI, pActif) ;
  pActifNon = new TRadioButton(this, IDC_CM_ACTIF_NON, pActif) ;

  pData     = new NSMaterielData() ;
  bCreation = bMode ;

  // on récupère le pointeur sur l'array des materiels
  pMatArray = pMaterielArray ;
}
catch (...)
{
	erreur("Exception CreerMaterielDialog ctor.", standardError, 0) ;
}
}

CreerMaterielDialog::~CreerMaterielDialog(){
	delete pLibelle ;
	delete pType ;
	delete pNumSerie ;
	delete pMiseSvce ;
	delete pActif ;
	delete pActifOui ;
	delete pActifNon ;
	delete pData ;
}

void
CreerMaterielDialog::SetupWindow()
{
	string sCodeLexique ;

	NSUtilDialog::SetupWindow() ;

	// on affiche dans les champ Edit les valeurs des pData
	if (string("") != pData->_sType)
	{
		sCodeLexique = pData->_sType ;
    pContexte->getDico()->donneCodeComplet(sCodeLexique) ;
    pType->setLabel(sCodeLexique) ;
	}
	else
		pType->SetText("") ;

	if (string("") != pData->_sMise_svce)
  	pMiseSvce->setDate(pData->_sMise_svce) ;
  else
  	pMiseSvce->setDate(string("00000000")) ;

	pLibelle->SetText(pData->_sLibelle.c_str()) ;	pNumSerie->SetText(pData->_sNum_serie.c_str()) ;
	if (false == pData->isActive())
  	pActifNon->Check() ;
	else
		pActifOui->Check() ;
}

void
CreerMaterielDialog::CmOk()
{
	string sTexte ;
	string sCodeLexique ;
  string sDate ;
  char far type[MAT_TYPE_LEN + 1] ;

  // Saisie du code Type
  pType->GetText(type, MAT_TYPE_LEN + 1);
  if (!strcmp(type, ""))
  {
  	erreur("Le champ Type est obligatoire.", standardError, 0) ;
    return ;
	}
  // on sait que le champ Type n'est pas vide
  sTexte = pType->getCode();
  if (string("£?????") == sTexte)		// texte libre
  {
  	erreur("Vous devez choisir le type matériel à partir du lexique, et non en texte libre.", standardError, 0) ;
    return ;
	}

  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&sTexte, &sCodeSens) ;

	// le type n'est pas modifiable
  if (bCreation)
    pData->_sType = sCodeSens ;
  else if (sCodeSens != pData->_sType)  // si la valeur du type a changé
  {
  	erreur("Le champ Type ne peut pas être modifié, car ce matériel est déjà enregistré.", standardError, 0) ;
    if (string("") != pData->_sType)
    {
    	sCodeLexique = pData->_sType ;
      pContexte->getDico()->donneCodeComplet(sCodeLexique) ;
      pType->setLabel(sCodeLexique) ;
    }
    else
    	pType->SetText("") ;
    return ;
	}

	// La date de mise en service est modifiable
	pMiseSvce->getDate(&sDate) ;
	pData->_sMise_svce = sDate ;

	// Le libelle n'est pas modifiable
  pLibelle->GetText(sTexte) ;
  if (sTexte == "")
  {
  	erreur("Le champ Libelle est obligatoire.", standardError, 0) ;
    return ;
	}

	if (bCreation)
    pData->_sLibelle = sTexte ;
	else if (sTexte != pData->_sLibelle)  // si la valeur a changé
  {
  	erreur("Le champ Libellé ne peut pas être modifié, car ce matériel est déjà enregistré.", standardError, 0) ;
    pLibelle->SetText(pData->_sLibelle.c_str()) ;
    return ;
	}

  // Le n° de série est modifiable
  pNumSerie->GetText(sTexte) ;
  pData->_sNum_serie = sTexte ;

  // Le champ actif est modifiable
  if (pActifOui->GetCheck() == BF_CHECKED)
    pData->setActif(true) ;
	if (pActifNon->GetCheck() == BF_CHECKED)
  	pData->setActif(false) ;

	// Contrôle du libelle : il doit etre unique pour chaque materiel
  if (bCreation)
  {
  	for (MatosIter i = pMatArray->begin() ; pMatArray->end() != i ; i++)
    {
    	if (pData->_sLibelle == (*i)->_Donnees._sLibelle)
      {
      	erreur("Ce nom existe déjà dans la base des materiels. Vous devez saisir un autre nom.", standardError, 0) ;
        return ;
      }
    }
	}

	NSUtilDialog::CmOk() ;
}

void
CreerMaterielDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ListeMaterielDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ListeMaterielDialog, NSUtilDialog)
	EV_COMMAND(IDC_LM_CREER,  CmCreer),
	EV_COMMAND(IDC_LM_SUPPR,  CmSupprimer),
  EV_COMMAND(IDC_LM_MODIF,  CmModifier),
  EV_COMMAND(IDC_LM_SETDEF, CmSetDefault),
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_LVN_GETDISPINFO(IDC_LM_LW, LvnGetDispInfo),
END_RESPONSE_TABLE;

ListeMaterielDialog::ListeMaterielDialog(TWindow* pere, NSContexte* pCtx, TModule* /* mod */)
                    :NSUtilDialog(pere, pCtx, "IDD_LISTMAT", pNSResModule)
{
try
{
  pListeMat      = new NSListeMatWindow(this, IDC_LM_LW) ;
  pMatArray      = new NSMaterielArray ;
  pMatAjoutArray = new NSMaterielArray ;
  pMatModifArray = new NSMaterielArray ;
  pMatSelect     = new NSMaterielInfo() ;

  nbMat          = 0 ;
  nbMatAjout     = 0 ;
  MaterielChoisi = -1 ;

	sDefaultEchogID = string("") ;
  readDefaultEchogID() ;
}
catch (...)
{
	erreur("Exception ListeMaterielDialog ctor.", standardError, 0) ;
}
}

ListeMaterielDialog::~ListeMaterielDialog()
{
	delete pMatSelect ;
  delete pMatAjoutArray ;
  delete pMatModifArray ;
  delete pMatArray ;
  delete pListeMat ;
}

voidListeMaterielDialog::SetupWindow()
{
	// fichiers d'aide
	sHindex = "" ;
	sHcorps = "" ;
	NSUtilDialog::SetupWindow() ;
	InitListe() ;

	if (InitMatArray())
		AfficheListe() ;
}

// Initialisation du tableau depuis la base
bool
ListeMaterielDialog::InitMatArray()
{
try
{
	pMatArray->vider() ;
	nbMat = 0 ;

	bool bResultActive   = InitMatArrayByActivity("1") ;
  bool bResultInactive = InitMatArrayByActivity("0") ;

	return bResultActive && bResultInactive ;
}
catch (...)
{
	erreur("Exception ListeMaterielDialog::InitMatArray.", standardError, 0) ;
	return false ;
}
}

bool
ListeMaterielDialog::InitMatArrayByActivity(string sActivityCode)
{
try
{
	VecteurString FoundObjects ;

	// on récupère la liste des matériels : actifs et non-actifs
	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;
	AttrList.push_back(new NSBasicAttribute("_OMATE_LACTF", sActivityCode)) ;
	AttrList.push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

  NSSuper *pSuper = pContexte->getSuperviseur() ;

	bool res = pSuper->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
	if (false == res)
	{
		string sErrorText = pSuper->getText("NTIERS", "objectNotFound") ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
		return false ;
	}

	if (false == ObjList.empty())
		for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
		{
			string sOIDS = (*k)->getAttributeValue(OIDS) ;
			FoundObjects.AddString(sOIDS) ;
		}

	for (EquiItemIter i = FoundObjects.begin() ; FoundObjects.end() != i ; i++)
	{
		NSMaterielInfo MatInfo ;
		if (MatInfo.initialiseDepuisObjet(pSuper, *(*i), sActivityCode))
		{
			pMatArray->push_back(new NSMaterielInfo(MatInfo)) ;
			nbMat++ ;
		}
	}

  return true ;
}
catch (...)
{
	erreur("Exception ListeMaterielDialog::InitMatArrayByActivity.", standardError, 0) ;
	return false ;
}
}

voidListeMaterielDialog::InitListe()
{
	string sLabel = pContexte->getSuperviseur()->getText("devicesManagement", "deviceLabel") ;
  string sType  = pContexte->getSuperviseur()->getText("devicesManagement", "deviceType") ;
  string sAvail = pContexte->getSuperviseur()->getText("devicesManagement", "deviceAvailable") ;

	TListWindColumn colLibelle((char*)sLabel.c_str(), 170, TListWindColumn::Left, 0) ;
  pListeMat->InsertColumn(0, colLibelle) ;
  TListWindColumn colType((char*)sType.c_str(), 230, TListWindColumn::Left, 1) ;
  pListeMat->InsertColumn(1, colType) ;
  TListWindColumn colActif((char*)sAvail.c_str(), 40, TListWindColumn::Left, 2) ;
  pListeMat->InsertColumn(2, colActif) ;
}

void
ListeMaterielDialog::AfficheListe()
{
	char libelle[255] ;

	pListeMat->DeleteAllItems() ;

	for (int i = nbMat - 1; i >= 0; i--)
	{
  	sprintf(libelle, "%s", ((*pMatArray)[i])->_Donnees._sLibelle.c_str()) ;
    TListWindItem Item(libelle, 0) ;
    pListeMat->InsertItem(Item) ;
	}
}

void
ListeMaterielDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
  const int 	BufLen = 255 ;
  static char buffer[BufLen] ;

  string sCodeLexique ;
  string sLibType ;

  string sLang = string("") ;
  if ((pContexte) && (pContexte->getUtilisateur()))
		sLang = pContexte->getUtilisateur()->donneLang() ;

  TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
  int index = dispInfoItem.GetIndex() ;

  NSMaterielInfo* pMatInfo = (*pMatArray)[index] ;
  if ((NSMaterielInfo*) NULL == pMatInfo)
    return ;

  // Affiche les informations en fonction de la colonne

  switch (dispInfoItem.GetSubItem())
  {
  	case 1: 	// Type
    	// si le code type existe, on récupère son libellé
      if (string("") != pMatInfo->_Donnees._sType)
      {
      	sprintf(buffer, "%s1", pMatInfo->_Donnees._sType.c_str()) ;
        sCodeLexique = string(buffer) ;
        pContexte->getDico()->donneLibelle(sLang, &sCodeLexique, &sLibType) ;
        strcpy(buffer, sLibType.c_str()) ;
        if (strlen(buffer))
        	buffer[0] = pseumaj(buffer[0]) ;
      }
      else
      	strcpy(buffer, "") ;

      break ;

    case 2: 	// Actif O/N

    	string sMsg = string("") ;

    	if (false == pMatInfo->isActive())
      	sMsg = pContexte->getSuperviseur()->getText("generalLanguage", "no") ;
      else
      	sMsg = pContexte->getSuperviseur()->getText("generalLanguage", "yes") ;

      // Check if default device
      if (pMatInfo->_Donnees._sObjectID == sDefaultEchogID)
      	sMsg = string("* ") + sMsg ;

      strcpy(buffer, sMsg.c_str()) ;
      break ;
  }

  dispInfoItem.SetText(buffer) ;
}

bool
ListeMaterielDialog::ExisteDansAjouts(NSMaterielInfo* pMatInfo)
{
	if (NULL == pMatInfo)
		return false ;

	if (pMatAjoutArray->empty())
		return false ;

	for (MatosIter i = pMatAjoutArray->begin() ; pMatAjoutArray->end() != i ; i++)
  	if (*pMatInfo == (*(*i)))
    	return true ;

	return false ;
}

boolListeMaterielDialog::InitDataMateriel(NSMaterielInfo* pMatInfo, bool bCreer)
{
	if ((NSMaterielInfo*) NULL == pMatInfo)
		return false ;

	CreerMaterielDialog MatDlg(pContexte->GetMainWindow(), pContexte, pMatArray, bCreer, GetModule()) ;

	// on initialise ici les données du dialogue dans tous les cas
	*(MatDlg.pData) = pMatInfo->_Donnees ;

	if ((MatDlg.Execute()) == IDCANCEL)
    return false ;

	// on stocke les donnees du dialogue dans les Data
	pMatInfo->_Donnees = *(MatDlg.pData) ;

	return true ;
}

bool
ListeMaterielDialog::CreerMateriel(NSMaterielInfo* pMatInfo)
{
	if ((NSMaterielInfo*) NULL == pMatInfo)
		return false ;

	NSPatPathoArray PatPatho(pContexte->getSuperviseur(), graphObject) ;

	// type de matériel : noeud root
  //
  string sLexique = pMatInfo->getLexiqueLabel(pContexte->getSuperviseur()) ;
	PatPatho.ajoutePatho(sLexique, 0) ;

	// date de mise en service
	PatPatho.ajoutePatho("KSERV1", 1) ;

	string sDateService = pMatInfo->getDate() + string("000000") ;
	Message Msg ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateService.c_str()) ;
	PatPatho.ajoutePatho("£D0;10", &Msg, 2) ;

	// libellé
	PatPatho.ajoutePatho("LNOMA1", 1) ;
  Msg.Reset() ;
	Msg.SetTexteLibre(pMatInfo->getLabel()) ;
	PatPatho.ajoutePatho("£CL000", &Msg, 2) ;

	// numéro de série
	PatPatho.ajoutePatho("LNUMS1", 1) ;
  Msg.Reset() ;
	Msg.SetTexteLibre(pMatInfo->getSN()) ;
	PatPatho.ajoutePatho("£CL000", &Msg, 2) ;

	NSObjectGraphManager GraphObject(pContexte->getSuperviseur()) ;
	string sRootId = GraphObject.setTree(&PatPatho, "") ;
	GraphObject.setRootObject(sRootId) ;

	// Appel du pilote
	NSDataGraph* pGraph = GraphObject.getDataGraph() ;

	//pObectsList la liste d'objects qui correspondent a ces criteres
	NSPersonsAttributesArray ObjectsList ;
	string user = "_000000" ;
	if (pContexte->getUtilisateur() != NULL)
  	user = pContexte->getUtilisateurID() ;

	string sTraitName1 = string("_OMATE_LTYPA") ;
	string sTraitName2 = string("_OMATE_LACTF") ;
	string sTraitName3 = string("_OMATE_LNOMA") ;

	NSBasicAttributeArray AttrList ;
	AttrList.push_back(new NSBasicAttribute(sTraitName1, pMatInfo->getType())) ;
	AttrList.push_back(new NSBasicAttribute(sTraitName2, pMatInfo->getActif())) ;
	AttrList.push_back(new NSBasicAttribute(sTraitName3, pMatInfo->getLabel())) ;
	AttrList.push_back(new NSBasicAttribute("user", user)) ;

	pContexte->getSuperviseur()->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(),
                        pGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false) ;

	if (NULL == pGraph)
		return "" ;

	NSDataTreeIter iter ;
	if (pGraph->getTrees()->ExisteTree(sLexique, pContexte->getSuperviseur(), &iter))
  {
    bool bModifyDefault = false ;
  	if (pMatInfo->getObjectID() == sDefaultEchogID)
    	bModifyDefault = true ;

		pMatInfo->setObjectID((*iter)->getTreeID()) ;

    if (bModifyDefault)
    	sDefaultEchogID = pMatInfo->getObjectID() ;
  }

	return true ;
}

bool
ListeMaterielDialog::ModifierMateriel(NSMaterielInfo* pMatInfo)
{
	if (NULL == pMatInfo)
		return false ;

	NSPatPathoArray PatPatho(pContexte->getSuperviseur(), graphObject) ;

	// type de matériel : noeud root
  //
  string sLexique = pMatInfo->getLexiqueLabel(pContexte->getSuperviseur()) ;
	PatPatho.ajoutePatho(sLexique, 0) ;

	// date de mise en service
	PatPatho.ajoutePatho("KSERV1", 1) ;
	string sDateService = pMatInfo->getDate() + string("000000") ;
	Message Msg ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(sDateService.c_str()) ;
	PatPatho.ajoutePatho("£D0;10", &Msg, 2) ;

	// libellé
	PatPatho.ajoutePatho("LNOMA1", 1) ;
  Msg.Reset() ;
	Msg.SetTexteLibre(pMatInfo->getLabel()) ;
	PatPatho.ajoutePatho("£CL000", &Msg, 2) ;

	// numéro de série
	PatPatho.ajoutePatho("LNUMS1", 1) ;
  Msg.Reset() ;
	Msg.SetTexteLibre(pMatInfo->getSN()) ;
	PatPatho.ajoutePatho("£CL000", &Msg, 2) ;

	NSObjectGraphManager GraphObject(pContexte->getSuperviseur()) ;
	string sRootId = GraphObject.setTree(&PatPatho, "") ;
	GraphObject.setRootObject(sRootId) ;

	// Appel du pilote
	NSDataGraph* pGraph = GraphObject.getDataGraph() ;

	//pObectsList la liste d'objects qui correspondent a ces criteres
	string user = "_000000";
	if (NULL != pContexte->getUtilisateur())
		user = pContexte->getUtilisateurID() ;
	string sTraitName1 = string("_OMATE_LTYPA") ;
	string sTraitName2 = string("_OMATE_LACTF") ;
	string sTraitName3 = string("_OMATE_LNOMA") ;

	NSBasicAttributeArray List ;
	List.push_back(new NSBasicAttribute("object",    pMatInfo->getObjectID())) ;
	List.push_back(new NSBasicAttribute(sTraitName1, pMatInfo->getType())) ;
	List.push_back(new NSBasicAttribute(sTraitName2, pMatInfo->getActif())) ;
	List.push_back(new NSBasicAttribute(sTraitName3, pMatInfo->getLabel())) ;
	List.push_back(new NSBasicAttribute("operator",  user)) ;
	NSPersonsAttributesArray ObjectsList ;

	int res = pContexte->getSuperviseur()->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_OBJECT.c_str(), pGraph, &ObjectsList, &List) ;
	if (!res)
	{
		erreur("Echec service à la modification du graphe d'un materiel.", standardError, 0) ;
    return false ;
	}

	return true ;
}

void
ListeMaterielDialog::CmCreer()
{
	NSMaterielInfo Info ;

	if (false == InitDataMateriel(&Info, true))
		return ;

	// on ajoute les infos récupérées aux deux tableaux
  //
	pMatArray->push_back(new NSMaterielInfo(Info)) ;
	nbMat++ ;
	pMatAjoutArray->push_back(new NSMaterielInfo(Info)) ;
	nbMatAjout++ ;
	AfficheListe() ;
}

void
ListeMaterielDialog::CmSupprimer()
{
	MaterielChoisi = pListeMat->IndexItemSelect() ;
	if (MaterielChoisi < 0)
  {
  	string sErrMsg = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
	}

	NSMaterielInfo Info = *((*pMatArray)[MaterielChoisi]) ;

	if (!ExisteDansAjouts(&Info))
	{
		erreur("Vous ne pouvez supprimer un matériel déjà enregistré.", standardError, 0) ;
    return ;
	}

	// Le matériel vient d'etre créé : on le supprime des deux tableaux
	for (MatosIter i = pMatArray->begin(); i != pMatArray->end(); i++)
	{
  	if (Info == (*(*i)))
    {
    	delete *i ;
      pMatArray->erase(i) ;
      nbMat-- ;
      break ;
    }
  }

	for (MatosIter j = pMatAjoutArray->begin(); j != pMatAjoutArray->end(); j++)
	{
  	if (Info == (*(*j)))
    {
			delete *j ;
      pMatAjoutArray->erase(j) ;
      nbMatAjout-- ;
      break ;
    }
  }

	AfficheListe() ;
}

void
ListeMaterielDialog::CmModifier()
{
	MaterielChoisi = pListeMat->IndexItemSelect() ;
	if (MaterielChoisi < 0)
  {
  	string sErrMsg = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
	}

  NSMaterielInfo Info = *((*pMatArray)[MaterielChoisi]) ;

	if (ExisteDansAjouts(&Info))	// ce materiel vient d'etre créé
	{
		int AjoutChoisi = -1 ;

  	// On retrouve l'index dans le tableau des ajouts
    int k = 0 ;
    for (MatosIter i = pMatAjoutArray->begin() ; i != pMatAjoutArray->end(); i++, k++)
    {
    	if (Info == (*(*i)))
      {
      	AjoutChoisi = k ;
        break ;
      }
    }

		// On lance la modif en mode création (cad qu'on autorise les modifs)
    if (!InitDataMateriel(&Info, true))
    	return ;

    // On met à jour les deux tableaux
    *((*pMatArray)[MaterielChoisi])   = Info ;
		*((*pMatAjoutArray)[AjoutChoisi]) = Info ;
  }
  else
  {
		// On lance la modif en mode modification (on interdit de modifier type et libellé)
    if (!InitDataMateriel(&Info, false))
    	return ;

    // On met à jour le tableau principal et on l'ajoute au tableau des materiels à modifier
		*((*pMatArray)[MaterielChoisi]) = Info ;
    pMatModifArray->push_back(new NSMaterielInfo(Info)) ;
	}

	// On tient à jour la liste
	AfficheListe() ;
}

void
ListeMaterielDialog::CmSetDefault()
{
	MaterielChoisi = pListeMat->IndexItemSelect() ;
	if (MaterielChoisi < 0)
  {
  	string sErrMsg = pContexte->getSuperviseur()->getText("generalLanguageForLists", "anElementMustBeSelected") ;
    erreur(sErrMsg.c_str(), warningError, 0) ;
    return ;
	}

  sDefaultEchogID = ((*pMatArray)[MaterielChoisi])->getObjectID() ;

  // On tient à jour la liste
	AfficheListe() ;
}

void
ListeMaterielDialog::CmOk()
{
	if (false == pMatModifArray->empty())
		for (MatosIter i = pMatModifArray->begin() ; pMatModifArray->end() != i ; i++)
			ModifierMateriel(*i) ;

	if (false == pMatAjoutArray->empty())
    for (MatosIter j = pMatAjoutArray->begin() ; pMatAjoutArray->end() != j ; j++)
			CreerMateriel(*j) ;

	string sIDtoSetDefault = sDefaultEchogID ;
  readDefaultEchogID() ;
  if (sIDtoSetDefault != sDefaultEchogID)
  {
  	sDefaultEchogID = sIDtoSetDefault ;
  	storeDefaultEchogID() ;
  }

	NSUtilDialog::CmOk() ;}

voidListeMaterielDialog::CmCancel()
{
	NSUtilDialog::CmCancel() ;
}

// fonction Click droit : menu contextuel
void
ListeMaterielDialog::EvRButtonDown(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sCrea = pSuper->getText("generalLanguageForLists", "add") ;
	string sModi = pSuper->getText("generalLanguageForLists", "modify") ;
	string sSupp = pSuper->getText("generalLanguageForLists", "remove") ;
	string sDefa = pSuper->getText("generalLanguageForLists", "setDefault") ;

	menu->AppendMenu(MF_STRING, IDC_LM_CREER,  sCrea.c_str()) ;
  menu->AppendMenu(MF_STRING, IDC_LM_MODIF,  sModi.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
	menu->AppendMenu(MF_STRING, IDC_LM_SUPPR,  sSupp.c_str()) ;
	menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  menu->AppendMenu(MF_STRING, IDC_LM_SETDEF, sDefa.c_str()) ;

	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}

void
ListeMaterielDialog::EvRButtonDownOut(uint /* modkeys */, NS_CLASSLIB::TPoint& point)
{
	// création d'un menu popup
	NS_CLASSLIB::TPoint lp = point ;

	TPopupMenu *menu = new TPopupMenu();

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sCrea = pSuper->getText("generalLanguageForLists", "add") ;

	menu->AppendMenu(MF_STRING, IDC_LM_CREER,  sCrea.c_str()) ;

	ClientToScreen(lp) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
	delete menu ;
}

void
ListeMaterielDialog::readDefaultEchogID()
{
	//
	//  ------------------- Ouverture de KE.LUS -------------------
	//
	string sFichierAspect = pContexte->PathName("FGLO") + string("KE.LUS") ;

	bool tourner = true ;

	ifstream inFile ;
	inFile.open(sFichierAspect.c_str()) ;
	//
	// Ouverture impossible
	if (!inFile)
		return ;

	char 		 buffer[201] ;
	string	 commande, parametre ;

	inFile.unsetf(ios::skipws) ;
	while (tourner && (inFile.getline(buffer, 200)))
	{
		commande = "" ;
		parametre = "" ;
		//
		//  -------------- Séparation Commande/Paramètres ---------------
		//
		for (int i = 0; i < 11; i++)
			commande += pseumaj(buffer[i]) ;
		for (int i = 0; buffer[i+12] != '\0'; i++)
			parametre += buffer[i+12] ;
		//
		// On enlève les blancs
    while ((strlen(commande.c_str()) > 0) && (commande[strlen(commande.c_str())-1] == ' '))
    	commande = string(commande, 0, strlen(commande.c_str()) - 1) ;
    while ((strlen(parametre.c_str()) > 0) && (parametre[strlen(parametre.c_str())-1] == ' '))
    	parametre = string(parametre, 0, strlen(parametre.c_str()) - 1) ;
    //commande.strip(commande.Both, ' ');
		//parametre.strip(parametre.Both, ' ');
		//
		//  --------------- Interprétation de la commande ---------------
    //  ATTENTION : Les commandes sont converties en MAJUSCULES
		//
		//  --------------------- Type d'Echographe ---------------------
		//
		if ((commande == "ECHOGDEFAU") && (parametre != ""))
    {
    	sDefaultEchogID = string(parametre) ;
      tourner = false;
    }
  }
  //
  //  --------------------- Fermeture du fichier ----------------------
  //
  inFile.close() ;
  return ;
}

void
ListeMaterielDialog::storeDefaultEchogID()
{
	//
	//  ------------------- Ouverture de KE.LUS -------------------
	//
	string sFichierAspect = pContexte->PathName("FGLO") + string("KE.LUS") ;

  //
	// Reading
  //

	ifstream inFile ;
	inFile.open(sFichierAspect.c_str()) ;
	if (!inFile)
		return ;

	VectString aStrings ;
  string     line ;

  while (!inFile.eof())
	{
  	getline(inFile, line) ;
    aStrings.push_back(new string(line)) ;
	}

  inFile.close() ;

  //
  // Modifying
  //

  bool bLineFound = false ;

  if (!(aStrings.empty()))
  {
  	for (IterString itStr = aStrings.begin() ; itStr != aStrings.end() ; itStr++)
    {
    	string sLine = **itStr ;

      // EchogDefau :OECHO___001
      // 01234567890123456789012

      string sField = string(sLine, 0, 11) ;
      string sValue = string(sLine, 12, strlen(sLine.c_str()) - 12) ;

      strip(sField, stripBoth) ;
      pseumaj(&sField) ;

      if (sField == string("ECHOGDEFAU"))
      {
      	**itStr = string(**itStr, 0, 12) + sDefaultEchogID ;
        bLineFound = true ;
        break ;
      }
    }
	}

  if (!bLineFound)
  {
  	string sLine = string("EchogDefau :") + sDefaultEchogID ;
    aStrings.push_back(new string(sLine)) ;
  }

  //
  // Storing
  //

  ofstream outFile ;

  outFile.open(sFichierAspect.c_str(), ios::out) ;
  if (!outFile)
  	return ;

  for (IterString itStr = aStrings.begin() ; itStr != aStrings.end() ; itStr++)
  	outFile << ((**itStr) + string("\n")) ;

  outFile.close() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListeMatWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
END_RESPONSE_TABLE;

//---------------------------------------------------------------------------
//  Function: NSListeMatWindow::EvLButtonDblClk(uint modKeys, TPoint& point)
//
//  Arguments:	  les modKeys et le point double-clické
//
//  Description: Fonction de réponse au double-click
//
//  Returns:     Rien
//---------------------------------------------------------------------------
void
NSListeMatWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

	if (info.GetFlags() & LVHT_ONITEM)
		pDlg->CmModifier() ;
}

void
NSListeMatWindow::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;    pDlg->EvRButtonDown(modkeys, point) ;
  }
  else
  	pDlg->EvRButtonDownOut(modkeys, point) ;
}

//---------------------------------------------------------------------------
//  Function: NSListeMatWindow::IndexItemSelect()
//
//  Arguments:	  Aucun
//
//  Description: Retourne l'index du premier item sélectionné
//
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListeMatWindow::IndexItemSelect()
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

bool
MatSortByLibelleInf(NSBasicAttributeArray *pMat1, NSBasicAttributeArray *pMat2)
{
  string sMat1Lib = pseumaj(pMat1->getAttributeValue("_OMATE_LNOMA")) ;
  string sMat2Lib = pseumaj(pMat2->getAttributeValue("_OMATE_LNOMA")) ;
	return (sMat1Lib > sMat2Lib) ;
}


bool
MatSortByLibelleSup(NSBasicAttributeArray *pMat1, NSBasicAttributeArray *pMat2)
{
	string sMat1Lib = pseumaj(pMat1->getAttributeValue("_OMATE_LNOMA")) ;
  string sMat2Lib = pseumaj(pMat2->getAttributeValue("_OMATE_LNOMA")) ;
	return (sMat1Lib < sMat2Lib) ;
}

// -----------------------------------------------------------------
//
//  Méthodes de ChoixMaterielDialog
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixMaterielDialog, NSUtilDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_SELCHANGE, CmSelectMateriel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_DBLCLK, 	 CmMaterielDblClk),
	EV_COMMAND(ID_ELARGIR, CmElargir),
END_RESPONSE_TABLE;

ChoixMaterielDialog::ChoixMaterielDialog(TWindow* pere, NSContexte* pCtx,
					                     string* pMaterType, string* pMatUtili,
                                         NSMaterielInfo* pMatChoix)
                    :NSUtilDialog(pere, pCtx, "IDD_MATERIEL", pNSResModule)
{
try
{
	_pMatBox         = new OWL::TListBox(this, IDC_MATERIELBOX, pNSResModule) ;
  _pMatBox->DisableTransfer() ;
  _pChoiceButton   = new OWL::TButton(this, ID_ELARGIR) ;

	_iMaterielChoisi = 0 ;
	_pMaterielChoisi = pMatChoix ;

	_pMatType        = pMaterType ;
	_pMatUtilisateur = pMatUtili ;

	_sTypeEnCours    = *pMaterType ;
}
catch (...)
{
	erreur("Exception ChoixMaterielDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Function: ChoixMaterielDialog::~ChoixMaterielDialog()
//
//  Description: Destructeur.
//--------------------------------------------------------------------------
ChoixMaterielDialog::~ChoixMaterielDialog()
{
	//
	// Choix "Non précisé"
	//
	if (1 == _iMaterielChoisi)
    _pMaterielChoisi->reset() ;
	else if ((_iMaterielChoisi > 1) && (false == _aFoundObj.empty()))
	{
  	NSPersonsAttributeIter iterObj = _aFoundObj.begin() ;
    size_t count = 2 ; // MaterielChoisi 2 = first object on the list
    while ((_aFoundObj.end() != iterObj) && (count < _iMaterielChoisi))
    {
    	iterObj++ ;
      count++ ;
    }
    string sOids = string("") ;
    if (_aFoundObj.end() != iterObj)
    	sOids = (*iterObj)->getAttributeValue(OIDS) ;
    _pMaterielChoisi->initialiseDepuisObjet(pContexte->getSuperviseur(), sOids) ;
	}

	delete _pMatBox ;
  delete _pChoiceButton ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des matériels
//---------------------------------------------------------------------------
void
ChoixMaterielDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;

	initialiserListe() ;
	//
	// Si un appareil avait déjà été sélectionné, on le coche
	//
#ifndef N_TIERS
	if ((pMaterielChoisi->pDonnees->code[0] != '\0') && (!(pMatArray->empty())))
	{
        int iNumApp = 0;
        MatosIter i;
   	    for (i = pMatArray->begin(); (i != pMatArray->end()) &&
      		(strcmp((*i)->pDonnees->code, pMaterielChoisi->pDonnees->code) != 0);
       		i++, iNumApp++);
        if (i != pMatArray->end())
            pMatBox->SetSelIndex(iNumApp+1);
	}
#endif
}

voidChoixMaterielDialog::CmElargir()
{
	string sNouvType = string("") ;
	if (string("") != _sTypeEnCours)
		sNouvType = pContexte->getSuperviseur()->getFilGuide()->PremierVrai(_sTypeEnCours,
                                                                    "ES",
                                                                    "FLECHE") ;
	if ((string("") != sNouvType) && (sNouvType != _sTypeEnCours))
	{
  	_sTypeEnCours = sNouvType ;
    initialiserListe() ;
	}
}

void
ChoixMaterielDialog::initialiserListe()
{
try
{
	string sLang = string("") ;
	if ((pContexte) && (pContexte->getUtilisateur()))
		sLang = pContexte->getUtilisateur()->donneLang() ;

  ClasseStringVector aTypes ;
  DecomposeChaine(&_sTypeEnCours, &aTypes) ;

	//
  // Affichage du titre
  //
  string sLibelleType = string("") ;
  if (string("") != _sTypeEnCours)
  {
    for (iterClassStringConst iter = aTypes.begin() ; aTypes.end() != iter ; iter++)
    {
      string sTypeComplet = (*iter)->getItem() ;
      NSDico::donneCodeComplet(sTypeComplet) ;

      string sLabel ;
      pContexte->getDico()->donneLibelle(sLang, &sTypeComplet, &sLabel) ;

      if (string("") != sLabel)
      {
        sLabel[0] = pseumaj(sLabel[0]) ;
        if (string("") != sLibelleType)
          sLibelleType += string(" + ") ;
        sLibelleType += sLabel ;
      }
    }
  }
  else
  	sLibelleType = string("Tous les matériels") ;

  if (string("en") == sLang)
    sLibelleType = string("DEVICES SELECTION: ") + sLibelleType ;
  else
    sLibelleType = string("CHOIX DES MATERIELS : ") + sLibelleType ;
  SetCaption(sLibelleType.c_str()) ;

#ifndef N_TIERS
	//
	// Mise à zéro de l'array et de la boite de liste
	//
	pMatArray->vider();
#endif

	int listeCount = _pMatBox->GetCount() ;
	if (listeCount < 0)
  	erreur("Boite de liste défectueuse.", standardError, 0) ;
  if (listeCount > 0)
  	_pMatBox->ClearList() ;

	_pMatBox->AddString("Non précisé") ;

  _aFoundObj.vider() ;

	//
	// On cherche tous les matériels qui sont un sTypeEnCours
	//
	VecteurString vecteurString;
	if (string("") != _sTypeEnCours)
		pContexte->getSuperviseur()->getFilGuide()->chercheEquivalent(_sTypeEnCours,
                                            &vecteurString, "ES", "ENVERS") ;

  if (false == vecteurString.empty())
  {
  	for (IterString iterVectString = vecteurString.begin(); vecteurString.end() != iterVectString ; iterVectString++)
    {
      NSPersonsAttributesArray ObjList ;
	    NSBasicAttributeArray    AttrList ;

    	AttrList.push_back(new NSBasicAttribute("_OMATE_LACTF", "1")) ;
      AttrList.push_back(new NSBasicAttribute(string("_OMATE_LTYPA"), *(*iterVectString))) ;
      bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST_WITH_TRAITS.c_str(), &ObjList, &AttrList) ;
      if ((false == res) && (*(*iterVectString) == _sTypeEnCours))
      {
        string sTypeCompletNotFound = *(*iterVectString) ;
        NSDico::donneCodeComplet(sTypeCompletNotFound) ;
        string sLabelNotFound ;
        pContexte->getDico()->donneLibelle(sLang, &sTypeCompletNotFound, &sLabelNotFound) ;
        sLabelNotFound[0] = pseumaj(sLabelNotFound[0]) ;

      	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") + string(" (") + sLabelNotFound + string(")") ;
        // erreur(sErrorText.c_str(), standardError, 0, 0) ;
        pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
      }
      else
      {
      	if (false == ObjList.empty())
        	for (NSPersonsAttributeIter iterObj = ObjList.begin() ; iterObj != ObjList.end() ; iterObj++)
          	//string sOIDS = (*iterObj)->getAttributeValue(OIDS) ;
            _aFoundObj.push_back(new NSBasicAttributeArray(**iterObj)) ;
      }
      AttrList.vider() ;
    }
	}

  if (false == _aFoundObj.empty())
  {
    sort(_aFoundObj.begin(), _aFoundObj.end(), MatSortByLibelleSup) ;

	  for (NSPersonsAttributeIter iterObj = _aFoundObj.begin() ; _aFoundObj.end() != iterObj ; iterObj++)
      _pMatBox->AddString((*iterObj)->getAttributeValue("_OMATE_LNOMA").c_str()) ;
  }
}
catch (...)
{
	erreur("Exception ChoixMaterielDialog::initialiserListe.", standardError, 0) ;
}
}


//---------------------------------------------------------------------------//  Prend acte de la sélection du matériel
//---------------------------------------------------------------------------
void ChoixMaterielDialog::CmSelectMateriel(WPARAM /* Cmd */)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
  _iMaterielChoisi = _pMatBox->GetSelIndex() + 1 ;
}

//---------------------------------------------------------------------------//  Sélectionne un matériel
//---------------------------------------------------------------------------
void ChoixMaterielDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
  _iMaterielChoisi = _pMatBox->GetSelIndex() + 1 ;
	TDialog::CmOk() ;
}

//---------------------------------------------------------------------------//  Annule MaterielChoisi et appelle Cancel()
//---------------------------------------------------------------------------
void ChoixMaterielDialog::CmCancel()
{
  _iMaterielChoisi = 0 ;
	TDialog::CmCancel() ;
}

//***************************************************************************
// 				Implémentation des méthodes Protocole
//***************************************************************************

//---------------------------------------------------------------------------//  Fonction:		NSProtocoleArray(NSProtocoleArray& rv)
//  Description:	Constructeur copie
//  Retour:			Rien
//---------------------------------------------------------------------------
NSProtocoleArray::NSProtocoleArray(NSProtocoleArray& rv)
					  :NSProtInfoArray()
{
try
{
    if (!(rv.empty()))
	    for (IterProto i = rv.begin(); i != rv.end(); i++)
   	        push_back(new NSProtocoleInfo(*(*i)));
}
catch (...)
{
    erreur("Exception NSProtocoleArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur
//---------------------------------------------------------------------------
NSProtocoleArray::~NSProtocoleArray()
{
	vider();
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
void
NSProtocoleArray::vider()
{
    if (empty())
        return ;

	for (IterProto i = begin(); i != end(); )
    {
   	    delete *i;
        erase(i);
    }
}

// -----------------------------------------------------------------//
//  Méthodes de ChoixProtocole
//
// -----------------------------------------------------------------
DEFINE_RESPONSE_TABLE1(ChoixProtocole, NSUtilDialog /* TDialog */)
	EV_CHILD_NOTIFY_AND_CODE(IDC_PROTOCOLBOX, LBN_SELCHANGE, CmSelectProto),
	EV_CHILD_NOTIFY_AND_CODE(IDC_PROTOCOLBOX, LBN_DBLCLK, 	CmProtoDblClk),
END_RESPONSE_TABLE;

ChoixProtocole::ChoixProtocole(TWindow* pere, NSContexte* pCtx,
                               string* pRotType, string* pRotoUtili,
                               NSProtocoleInfo* pProtChoix)
               //:TDialog(pere, "IDD_PROTOCOL", pNSResModule), NSRoot(pCtx)
               :NSUtilDialog(pere, pCtx, "IDD_PROTOCOL", pNSResModule)
{
try
{
	pRotocolBox 	    = new TListBox(this, IDC_PROTOCOLBOX) ;
	pProtocoleChoisi  = pProtChoix ;
	ProtocoleChoisi   = 0 ;
	pRotocolType	    = pRotType ;
	pProtoUtilisateur = pRotoUtili ;
  pProtArray        = 0 ;
}
catch (...)
{
    erreur("Exception ChoixProtocole ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Function: ChoixProtocole::~ChoixProtocole()
//
//  Description: Destructeur.
//---------------------------------------------------------------------------
ChoixProtocole::~ChoixProtocole()
{
	//
	// Si un protocole a été choisi
	//
	if (NULL != ProtocoleChoisi)
		*pProtocoleChoisi = *((*pProtArray)[ProtocoleChoisi - 1]) ;

	delete pRotocolBox ;

  if (NULL != pProtArray)
		delete pProtArray ;
}

//---------------------------------------------------------------------------
//  Initialise la boite de liste des protocoles
//---------------------------------------------------------------------------
void
ChoixProtocole::SetupWindow()
{
try
{
	TDialog::SetupWindow();

	//	// Remplissage de MaterielBox avec le protocole idoine
	//
	pProtArray = new NSProtocoleArray;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;

	AttrList.push_back(new NSBasicAttribute(string("_0PROT_LTYPA"), *pRotocolType)) ;
	bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST_WITH_TRAITS.c_str(), &ObjList, &AttrList) ;
	if (!res)
	{
		string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
		erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return ;
	}

	if (ObjList.empty())
		return ;

	for (NSPersonsAttributeIter iterObj = ObjList.begin() ; iterObj != ObjList.end() ; iterObj++)
	{
  	string sOIDS = (*iterObj)->getAttributeValue(OIDS) ;

    NSProtocoleInfo Protocole ;
    if (Protocole.initialiseDepuisObjet(pContexte, sOIDS))
    {
    	string sLib = Protocole.getLibelle() ;
    	pRotocolBox->AddString(sLib.c_str()) ;
    	pProtArray->push_back(new NSProtocoleInfo(Protocole)) ;
    }
	}

	//
	// Si un protocole avait déjà été sélectionné, on le coche
	//
	if ((string("") != pProtocoleChoisi->pDonnees->sObjectID) && (!(pProtArray->empty())))
	{
		int iNumApp = 0 ;
		IterProto i ;
		for (i = pProtArray->begin(); (i != pProtArray->end()) &&
      		((*i)->pDonnees->sObjectID != pProtocoleChoisi->pDonnees->sObjectID);
       		i++, iNumApp++) ;
		if (i != pProtArray->end())
    	pRotocolBox->SetSelIndex(iNumApp) ;
	}
}
catch (...)
{
	erreur("Exception ChoixProtocole::SetupWindow.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Description :	Prend acte de la sélection du protocole
//---------------------------------------------------------------------------
void ChoixProtocole::CmSelectProto(WPARAM /* Cmd */)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	ProtocoleChoisi = pRotocolBox->GetSelIndex() + 1 ;
}
//---------------------------------------------------------------------------//  Description :	Sélectionne un protocole
//---------------------------------------------------------------------------
void ChoixProtocole::CmProtoDblClk(WPARAM /* Cmd */)
{
	ProtocoleChoisi = pRotocolBox->GetSelIndex() + 1 ;
	TDialog::CmOk() ;
}

//---------------------------------------------------------------------------
//  Description :	Annule ProtocoleChoisi et appelle Cancel()
//---------------------------------------------------------------------------
void ChoixProtocole::CmCancel()
{
	ProtocoleChoisi = 0 ;
	TDialog::CmCancel() ;
}

//
// Utility functions
//
void
selectMaterial(NSDlgFonction* pNSDlgFct, NSSmallBrother* pBigBoss, string sMaterialType, string sDefaultTitle, string sLang)
{
try
{
	if ((NULL == pNSDlgFct) || (NULL == pBigBoss) || (NULL == pNSDlgFct->getControl()))
		return ;

  if (string("") == sMaterialType)
    return ;

	//
	// On cherche si un entéro/endoscope avait déjà été choisi
	//
	NSTransferInfo* pTransfert = pNSDlgFct->getControl()->getTransfert() ;
  if (NULL == pTransfert)
    return ;

  string sUtilisateur = string("___") ;

  NSMaterielInfo Materiel ;
	//
	// Lancement de la boîte de dialogue de choix de l'endoscope
	//
  // Warning : doing new ChoixMaterielDialog((TWindow*) pNSDlgFct->pControle->getNSDialog()...
  //           make Execute() fail
  //           take care to include "nsbb\nsdlg.h"
  //
  ChoixMaterielDialog *pMatSelect = new ChoixMaterielDialog(pNSDlgFct->getControl()->getNSDialog(), pBigBoss->pContexte,
										  &sMaterialType, &sUtilisateur, &Materiel) ;
  int iRetId = pMatSelect->Execute() ;
  delete pMatSelect ;

/*
	if (ChoixMaterielDialog((TWindow*)pNSDlgFct->pControle->getNSDialog(), pBigBoss->pContexte,
										  &sMaterialType, &sUtilisateur, &Materiel).Execute()
																							== IDOK)
*/

  if (IDOK == iRetId)
	{
		NSButton* pNSButton = static_cast<NSButton*>(pTransfert->getControl()->getControle()) ;
		string sMatChoisi = Materiel.donneComplement() ;

		if (pNSButton)
    {
    	if (string("") != sMatChoisi)
      {
      	pTransfert->getTransfertMessage()->SetComplement(sMatChoisi) ;
        pTransfert->getFilsItem()->Active() ;

        string sNom = Materiel.getLabelAndSN() ;
        if (string("") != sNom)
          pNSButton->SetCaption(sNom.c_str()) ;
        else
          pNSButton->SetCaption(sDefaultTitle.c_str()) ;
      }
      else
      {
      	pTransfert->getTransfertMessage()->SetComplement("") ;
        pTransfert->getFilsItem()->Desactive() ;
        pNSButton->SetCaption(sDefaultTitle.c_str()) ;
      }
    }
  }
}
catch(TWindow::TXWindow& e)
{
	string sErr = string("Exception selectMaterial : ") + e.why() ;
	erreur(sErr.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception selectMaterial.", standardError, 0) ;
}
}

int InitMaterialTitle(NSDlgFonction* pNSDlgFct, NSSmallBrother* pBigBoss, string sDefaultTitle, string sLang)
{
try
{
	if (((NSDlgFonction*) NULL == pNSDlgFct) ||
      (NULL == pNSDlgFct->getControl()) ||
      ((NSSmallBrother*) NULL == pBigBoss))
		return 1 ;

	NSTransferInfo* pTransfert = pNSDlgFct->getControl()->getTransfert() ;
	if (((NSTransferInfo*) NULL == pTransfert) || (NULL == pTransfert->getFilsItem()))
		return 1 ;

	NSButton* pNSButton = static_cast<NSButton*>(pTransfert->getControl()->getControle()) ;
  if ((NSButton*) NULL == pNSButton)
		return 1 ;

	string sCode = pTransfert->getFilsItem()->getItemLabel() ;

	NSPatPathoArray* pPPT = pBigBoss->getPatPatho() ;
  if ((NSPatPathoArray*) NULL == pPPT)
		return 0 ;

	PatPathoIter iter = pPPT->ChercherItem(sCode) ;
	if (pPPT->end() == iter)
  {
    pNSButton->SetCaption(sDefaultTitle.c_str()) ;
		return 0 ;
  }

	string sObjectId = (*iter)->getComplement() ;
  if (string("") == sObjectId)
		return 1 ;

	NSMaterielInfo Materiel ;
  Materiel.initialiseDepuisObjet(pBigBoss->pContexte->getSuperviseur(), sObjectId) ;

	string sNom = Materiel.getLabelAndSN() ;
	if (string("") != sNom)
		pNSButton->SetCaption(sNom.c_str()) ;
	else
		pNSButton->SetCaption(sDefaultTitle.c_str()) ;

	return 0 ;
}
catch (...)
{
  erreur("Exception InitMaterialTitle.", standardError, 0) ;
  return 1 ;
}
}

void
chercherProtocole(NSDlgFonction* pNSDlgFct, NSSmallBrother* pBigBoss,
							BBFilsItem* pBBFilsItem, string sEtiquette, string sLang)
{
try
{
	if ((NULL == pNSDlgFct) || (NULL == pBigBoss) || (NULL == pNSDlgFct->getControl()))
		return ;

	NSProtocoleInfo Protoco ;
	string sCodeSens ;
	pBigBoss->pContexte->getDico()->donneCodeSens(&sEtiquette, &sCodeSens) ;

	string sType 		    = sCodeSens ;
	string sUtilisateur = "___" ;

    //
	// Lancement de la boîte de dialogue de choix du protocole
	//
	if (ChoixProtocole(pNSDlgFct->getControl()->getNSDialog(), pBigBoss->pContexte,
                       &sType, &sUtilisateur, &Protoco).Execute() != IDOK)
		return ;

	if (string("") == Protoco.pDonnees->sObjectID)
  {
  	ErasePatpatho(pBBFilsItem) ;
		return ;
	}

	pBBFilsItem->getItemTransfertMsg()->SetComplement(Protoco.pDonnees->sObjectID) ;

	string sLibbelle = Protoco.getLibelle() ;

	NSButton* pNSButton = static_cast<NSButton*>(pBBFilsItem->getItemTransfertData()->getControl()->getControle()) ;
	if (pNSButton)
		pNSButton->SetCaption(sLibbelle.c_str()) ;

	FormerPatpatho(pBigBoss, pBBFilsItem, &Protoco, sLang) ;
}
catch (...)
{
	erreur("Exception chercherProtocole.", standardError, 0) ;
}
}

//----------------------------------------------------------------
//former une patpatho à partir de PRESCRIP.DB et la donner à
//pBBFilsItem (bouton)
//----------------------------------------------------------------
void FormerPatpatho(NSSmallBrother* pBigBoss, BBFilsItem* pBBFilsItem, NSProtocoleInfo* pProtocol, string sLang)
{
try
{
	if ((NULL == pProtocol) || (NULL == pBBFilsItem) || (NULL == pBigBoss))
		return ;

	NSPatPathoArray* pPatpathoArray = pProtocol->getPptPtr() ;
  if (pPatpathoArray->empty())
		return ;

	//vider l'ancienne patpatho de pBBFilsItem
	NSVectFatheredPatPathoArray* pPatBouton = pBBFilsItem->getPatPatho() ;
  if (NULL == pPatBouton)
  	return ;
	pPatBouton->vider() ;

	pBBFilsItem->Active() ;

  // Remove object's IDs
  //
  NSPatPathoArray* pNewPatpathoArray = new NSPatPathoArray(*pPatpathoArray) ;
  pNewPatpathoArray->clearAllIDs() ;
  
	pPatBouton->push_back(new NSFatheredPatPathoArray(pBigBoss->pContexte->getSuperviseur(), 0, pNewPatpathoArray)) ;
}
catch (...)
{
	erreur("Exception FormerPatpatho.", standardError, 0) ;
}
}

void ErasePatpatho(BBFilsItem* pBBFilsItem)
{
	if (NULL == pBBFilsItem)
		return ;

	NSVectFatheredPatPathoArray* pPatBouton = pBBFilsItem->getPatPatho() ;
  if (NULL == pPatBouton)
  	return ;
	pPatBouton->vider() ;

	pBBFilsItem->Desactive() ;
}

