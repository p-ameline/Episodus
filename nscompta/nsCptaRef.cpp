//---------------------------------------------------------------------------//    NSMATFIC.CPP
//    PA   juillet 95
//  Implémentation des objets materiel
//---------------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring.h>

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsglobalLus.h"
#else
  #include <owl\dialog.h>

  #include <owl\applicat.h>
  #include <owl\window.h>

  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
  #include "partage\nsglobal.h"

  #include "partage\nscim10.rh"
  #include "partage\nsmatfic.rh"
#endif

#include "nscompta\nscompta.h"

#include "nsbb\nsarc.h"#include "nscompta\nsFseUti.h"#include "nscompta\nsCptaRef.h"
#include "nscompta\nsCptaRef.rh"
#include "nssavoir\nsfilgd.h"

// **************************************************************************
//  Méthodes de NSClamData
// **************************************************************************

//---------------------------------------------------------------------------
//  Met à vide les variables de la fiche
//---------------------------------------------------------------------------
void
NSLibreData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code,      0, CLAM_CODE_LEN + 1) ;
	memset(libelle,   0, CLAM_LIBELLE_LEN + 1) ;
	memset(module,    0, CLAM_MODULE_LEN + 1) ;
	memset(prix_f,    0, CLAM_PRIX_LEN + 1) ;
  memset(prix_e,    0, CLAM_PRIX_LEN + 1) ;
	memset(frequence, 0, CLAM_FREQ_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSLibreData::NSLibreData(const NSLibreData& rv)
{
	strcpy(code,      rv.code) ;
	strcpy(libelle,   rv.libelle) ;
	strcpy(module,    rv.module) ;
	strcpy(prix_f,    rv.prix_f) ;
  strcpy(prix_e,    rv.prix_e) ;
	strcpy(frequence, rv.frequence) ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSLibreData&
NSLibreData::operator=(const NSLibreData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(code,      src.code) ;
	strcpy(libelle,   src.libelle) ;
	strcpy(module,    src.module) ;
	strcpy(prix_e,    src.prix_e) ;
  strcpy(prix_f,    src.prix_f) ;
	strcpy(frequence, src.frequence) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLibreData::operator == (const NSLibreData& o) const
{
	if ((strcmp(module,	   o.module)    == 0) &&
		  (strcmp(code, 	   o.code)	    == 0) &&
		  (strcmp(libelle,   o.libelle)   == 0) &&
      (strcmp(prix_e,    o.prix_e)    == 0) &&
      (strcmp(prix_f,    o.prix_f)    == 0) &&
      (strcmp(frequence, o.frequence) == 0)
         )
		return 1 ;
	else
		return 0 ;
}

// **************************************************************************
//  Méthodes de NSCim10
// **************************************************************************

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSClam::NSClam(NSSuper* pSuper)
       :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSClam::~NSClam()
{
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSClam::alimenteFiche()
{
	alimenteChamp(_Donnees.code,      CLAM_CODE_FIELD,    CLAM_CODE_LEN) ;
	alimenteChamp(_Donnees.libelle,   CLAM_LIBELLE_FIELD, CLAM_LIBELLE_LEN) ;
	alimenteChamp(_Donnees.module,    CLAM_MODULE_FIELD,  CLAM_MODULE_LEN) ;
	alimenteChamp(_Donnees.prix_f,    CLAM_PRIX_F_FIELD,  CLAM_PRIX_LEN) ;
  alimenteChamp(_Donnees.prix_e,    CLAM_PRIX_E_FIELD,  CLAM_PRIX_LEN) ;
	alimenteChamp(_Donnees.frequence, CLAM_FREQ_FIELD,    CLAM_FREQ_LEN) ;
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSClam::videFiche()
{
	videChamp(_Donnees.code,      CLAM_CODE_FIELD,    CLAM_CODE_LEN) ;
	videChamp(_Donnees.libelle,   CLAM_LIBELLE_FIELD, CLAM_LIBELLE_LEN) ;
	videChamp(_Donnees.module,    CLAM_MODULE_FIELD,  CLAM_MODULE_LEN) ;
	videChamp(_Donnees.prix_f,    CLAM_PRIX_F_FIELD,  CLAM_PRIX_LEN) ;
  videChamp(_Donnees.prix_e,    CLAM_PRIX_E_FIELD,  CLAM_PRIX_LEN) ;
	videChamp(_Donnees.frequence, CLAM_FREQ_FIELD,    CLAM_FREQ_LEN) ;
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSClam::open()
{
	char tableName[] = "CLAM.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_COMPTA) ;
  return(lastError) ;
}

DBIResult
NSClam::getRecordByCode(string sCode, NSLibreInfo* pResult, bool bOpenClose, bool bVerbose)
{
	if (((NSLibreInfo*) NULL == pResult) || (string("") == sCode))
  	return DBIERR_NONE ;

  pResult->metAZero() ;

	// Opening the table
	//
  if (bOpenClose)
  {
		lastError = open() ;
		if (lastError != DBIERR_NONE)
		{
  		if (bVerbose)
				erreur("Erreur à l'ouverture du fichier CLAM.", standardError, lastError) ;
    	return lastError ;
  	}
  }
	//
	// Recherche du libelle dont le code est dans le champ complément
	//
	lastError = chercheClef(&sCode,
                          "",
                          0,
                          keySEARCHEQ,
                          dbiWRITELOCK) ;
	if (lastError != DBIERR_NONE)
	{
  	DBIResult dbError = lastError ;
  	if (bVerbose)
    {
    	string sError = string("Erreur à la recherche du code CLAM : ") + sCode ;
			erreur(sError.c_str(), standardError, lastError) ;
		}
    if (bOpenClose)
    	close() ;
		return dbError ;
	}
  //
  // Getting the record from table
  //
	lastError = getRecord() ;
	if (lastError != DBIERR_NONE)
	{
  	DBIResult dbError = lastError ;
  	if (bVerbose)
    {
    	string sError = string("Erreur à la lecture du code CLAM : ") + sCode ;
			erreur(sError.c_str(), standardError, lastError) ;
		}
    if (bOpenClose)
    	close() ;
		return dbError ;
	}
	//
	// Si tout a bien marché, on met à jour pMatInfo
	//
	pResult->_Donnees = _Donnees ;

  if (bOpenClose)
		close() ;

	return DBIERR_NONE ;
}

#endif // #ifndef _ENTERPRISE_DLL

// **************************************************************************
//  Méthodes de NSCim10Info
// **************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSLibreInfo::NSLibreInfo(NSContexte* pCtx)
            :NSRoot(pCtx)
{
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSCcam
//---------------------------------------------------------------------------
NSLibreInfo::NSLibreInfo(const NSClam* pClam, NSContexte* pCtx)
            :NSRoot(pCtx)
{
  if (pClam)
	  _Donnees = pClam->_Donnees ;
}
#endif // #ifndef _ENTERPRISE_DLL

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSLibreInfo::NSLibreInfo(const NSLibreInfo& rv)
            :NSRoot(rv.pContexte)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSLibreInfo&
NSLibreInfo::operator=(const NSLibreInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLibreInfo::operator==(const NSLibreInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

bool
ClamSortByCodeInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getCode() < pCCAM2->getCode()) ;
}
bool
ClamSortByCodeSup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getCode() > pCCAM2->getCode()) ;
}

bool
ClamSortByLibelleInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLibelle()) < pseumaj(pCCAM2->getLibelle())) ;
}
bool
ClamSortByLibelleSup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLibelle()) > pseumaj(pCCAM2->getLibelle())) ;
}

bool
ClamSortByPrixEInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getPriceE() > pCCAM2->getPriceE()) ;
}
bool
ClamSortByPrixESup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
  return (pCCAM1->getPriceE() < pCCAM2->getPriceE()) ;
}

bool
ClamSortByPrixFInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getPriceF() > pCCAM2->getPriceF()) ;
}
bool
ClamSortByPrixFSup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
  return (pCCAM1->getPriceF() < pCCAM2->getPriceF()) ;
}

bool
ClamSortByFreqInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getFrequency() > pCCAM2->getFrequency()) ;
}
bool
ClamSortByFreqSup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
	return (pCCAM1->getFrequency() < pCCAM2->getFrequency()) ;
}

bool
ClamSortByExamInf(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getModule() ;
  string sLexiqueCode2  = pCCAM2->getModule() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) < pseumaj(sLexiqueLabel2)) ;
}
bool
ClamSortByExamSup(NSLibreInfo *pCCAM1, NSLibreInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getModule() ;
  string sLexiqueCode2  = pCCAM2->getModule() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) > pseumaj(sLexiqueLabel2)) ;
}

#ifndef _ENTERPRISE_DLL
// **************************************************************************//  Méthodes de ChoixCcamDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(ChoixLibreDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_SELCHANGE, CmSelectMateriel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_DBLCLK, 	CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_CIM10BOX, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_CIM10BOX, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(ID_ELARGIR, CmElargir),
END_RESPONSE_TABLE;

ChoixLibreDialog::ChoixLibreDialog(TWindow* pere, NSContexte* pCtx,
                                   string* pModu, NSLibreInfo* pCodeChoix)
                 :TDialog(pere, "IDD_CLAM", pNSResModule), NSRoot(pCtx)
{
try
{
  _pChoiceButton = new OWL::TButton(this, ID_ELARGIR) ;
	_pMatBox 	     = new NSListeLibreWindow(this, IDC_CIM10BOX) ;
	_iCodeChoisi   = 0 ;
	_pCodeChoisi   = pCodeChoix ;
  _bGlobalList   = false ;

  if (pModu && (string("") != *pModu))
	  _sExam = *pModu ;
  else
  {
    _sExam       = string("") ;
    _bGlobalList = false ;
  }

  _bNaturallySorted = true ;
	_iSortedColumn    = 0 ;
}
catch (...)
{
	erreur("Exception ChoixCIM10Dialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
ChoixLibreDialog::~ChoixLibreDialog()
{
	//
	// Si un matériel a été choisi
	//
	if ((_iCodeChoisi > 0) && _pCodeChoisi)
		*_pCodeChoisi = *(_aMatArray[_iCodeChoisi - 1]) ;

	delete _pMatBox ;
  delete _pChoiceButton ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des codes CCAM
//---------------------------------------------------------------------------
void
ChoixLibreDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitLibreArray()) || (_aMatArray.empty()))
		return ;

	AfficheListe() ;

  if (string("") == _pCodeChoisi->getCode())
	{
  	_pMatBox->SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
    return ;
  }

	//
	// Si un code avait déjà été sélectionné, on le coche
	//
	int iNumApp = 0 ;
	LibreIter i = _aMatArray.begin() ;
	for ( ; (_aMatArray.end() != i) && ((*i)->getCode() != _pCodeChoisi->getCode()) ;
       		                                           i++, iNumApp++) ;

	if (_aMatArray.end() != i)
  	_pMatBox->SetItemState(iNumApp, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}
catch (...)
{
	erreur("Exception ChoixCcamDialog::SetupWindow.", standardError, 0) ;
}
}

void
ChoixLibreDialog::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;

	if (sizeType != SIZE_MINIMIZED)
	{
  	int LEFT_OFFSET  = 8 ;
    int TOP_OFFSET   = 8 ;
    int WIDTH_OFFSET = 24 ;

		ClassLib::TRect rect      = GetWindowRect() ;
    ClassLib::TRect list_rect = _pMatBox->GetWindowRect() ;
		_pMatBox->SetWindowPos(0, LEFT_OFFSET, TOP_OFFSET,
                              rect.Width() - WIDTH_OFFSET, list_rect.Height(),
                              SWP_NOZORDER ) ;
	}
}

bool
ChoixLibreDialog::InitLibreArray()
{
  if (_bGlobalList)
    return InitGlobalLibreArray() ;

  return InitLibreArrayForCode() ;
}

bool
ChoixLibreDialog::InitLibreArrayForCode()
{
try
{
	_aMatArray.vider() ;

  bool bExistFrequ = false ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSClam Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes CLAM.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.chercheClef(&_sExam,
                                    "MODULE_INDEX",
                                    NODEFAULTINDEX,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Il n'existe pas de code pour cet examen.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er code.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while ((DBIERR_NONE == Base.lastError) && (Base.getModule() == _sExam))
	{
		//
		// Ajout du matériel à l'array
		//
    // sLibelleAffiche = pBase->pDonnees->code;
    // if (pBase->pDonnees->libelle[0] != '\0')
    // 	sLibelleAffiche += string(" : ") + pBase->pDonnees->libelle;

		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aMatArray.push_back(new NSLibreInfo(&Base, pContexte)) ;

    if (string("") != Base.getFrequence())
      bExistFrequ = true ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au code suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du code.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

  if (bExistFrequ)
    sortByFreq() ;

	return true ;
}
catch (...)
{
	erreur("Exception ChoixLibreDialog::InitCCAMArray.", standardError, 0) ;
  return false ;
}
}

bool
ChoixLibreDialog::InitGlobalLibreArray()
{
try
{
	_aMatArray.vider() ;

  bool bExistFrequ = false ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSClam Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes CLAM.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des examens est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des examens est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er code.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aMatArray.push_back(new NSLibreInfo(&Base, pContexte)) ;

    if (string("") != Base.getFrequence())
      bExistFrequ = true ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au code suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du code.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception ChoixLibreDialog::InitGlobalCCAMArray.", standardError, 0) ;
  return false ;
}
}

void
ChoixLibreDialog::CmElargir()
{
  _bGlobalList = !_bGlobalList ;

  InitLibreArray() ;

  _bNaturallySorted = false ;

  if (_bGlobalList)
  {
    _pChoiceButton->SetCaption("Réduire le choix") ;
    sortByCode() ;
  }
  else
  {
    _pChoiceButton->SetCaption("Elargir le choix") ;
    sortByFreq() ;
  }
  // AfficheListe() ;  // Déjà fait par les "sort"
}

//---------------------------------------------------------------------------//  Réponse à selChanged
//  Prend acte de la sélection du matériel
//---------------------------------------------------------------------------
void
ChoixLibreDialog::CmSelectMateriel(WPARAM /* Cmd */)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	_iCodeChoisi = _pMatBox->IndexItemSelect() + 1 ;
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
ChoixLibreDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
	_iCodeChoisi = _pMatBox->IndexItemSelect() + 1 ;
	TDialog::CmOk();
}

void
ChoixLibreDialog::CmOk()
{
 	int iSelected = _pMatBox->IndexItemSelect()  ;
  if (-1 == iSelected)
  	return ;

  _iCodeChoisi = iSelected + 1 ;
	TDialog::CmOk() ;
}

//---------------------------------------------------------------------------
//  Bouton Cancel
//  Annule MaterielChoisi et appelle Cancel()
//---------------------------------------------------------------------------
void
ChoixLibreDialog::CmCancel()
{
	_iCodeChoisi = 0 ;
	TDialog::CmCancel() ;
}

void
ChoixLibreDialog::InitListe()
{
	TListWindColumn colCode("Code", 60, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colCode) ;
	TListWindColumn colLibelle("Libelle", 380, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colLibelle) ;
	TListWindColumn colPrixE("Prix €", 50, TListWindColumn::Right, 2) ;
	_pMatBox->InsertColumn(2, colPrixE) ;
  string sSigle = string("Prix ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 50, TListWindColumn::Right, 3) ;
	_pMatBox->InsertColumn(3, colPrixF) ;
	TListWindColumn colActif("Freq", 30, TListWindColumn::Left, 4) ;
	_pMatBox->InsertColumn(4, colActif) ;
}

void
ChoixLibreDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aMatArray.empty())
    return ;

	for (LibreReverseIter i = _aMatArray.rbegin() ; _aMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getCode() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
ChoixLibreDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aMatArray.size())
  	return ;

  string sBuffer = string("") ;
  size_t iLen ;

  NSLibreInfo* pLibreInfo = _aMatArray[index] ;
  if ((NSLibreInfo*) NULL == pLibreInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Libelle
    	sBuffer = pLibreInfo->getLibelle() ;
      break ;

		case 2 : // Prix Euros
    	sBuffer = pLibreInfo->getPrixE() ;
      iLen = strlen(sBuffer.c_str()) ;
      if (iLen > 2)
      	sBuffer = string(sBuffer, 0, iLen - 2) + string(",") + string(sBuffer, iLen - 2, 2) ;
      else if (string("0") == sBuffer)
        sBuffer = string("0,00") ;

      break ;

    case 3 : // Prix seconde monnaie
    	sBuffer = pLibreInfo->getPrixF() ;
      iLen = strlen(sBuffer.c_str()) ;
      if (iLen > 2)
      	sBuffer = string(sBuffer, 0, iLen - 2) + string(",") + string(sBuffer, iLen - 2, 2) ;
      else if (string("0") == sBuffer)
        sBuffer = string("0,00") ;

      break ;

    case 4 : // Prix
    	sBuffer = pLibreInfo->getFrequence() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
ChoixLibreDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0  : sortByCode() ;    break ;
    case 1  : sortByLibelle() ; break ;
    case 2  : sortByPriceE() ;  break ;
    case 3  : sortByPriceF() ;  break ;
    case 4  : sortByFreq() ;    break ;
  }
}

void
ChoixLibreDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
ChoixLibreDialog::sortByCode()
{
	swapSortOrder(0) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByCodeInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByCodeSup) ;

  AfficheListe() ;
}

void
ChoixLibreDialog::sortByLibelle()
{
	swapSortOrder(1) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByLibelleInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByLibelleSup) ;

  AfficheListe() ;
}

void
ChoixLibreDialog::sortByPriceE()
{
	swapSortOrder(2) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByPrixEInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByPrixESup) ;

  AfficheListe() ;
}

void
ChoixLibreDialog::sortByPriceF()
{
	swapSortOrder(3) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByPrixFInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByPrixFSup) ;

  AfficheListe() ;
}

void
ChoixLibreDialog::sortByFreq()
{
	swapSortOrder(4) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByFreqInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), ClamSortByFreqSup) ;

  AfficheListe() ;
}
#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListeLibreWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListeLibreWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSListeLibreWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmOk() ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListeLibreWindow::IndexItemSelect()
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
#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// **************************************************************************
//  Méthodes de LocalActsManagementDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(LocalActsManagementDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_LOTS, LBN_DBLCLK, CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_LOTS, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_LOTS, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK,   CmOk),
  EV_COMMAND(ID_NEW, CmNewAct),
END_RESPONSE_TABLE;

LocalActsManagementDialog::LocalActsManagementDialog(TWindow* pere, NSContexte* pCtx)
                          :TDialog(pere, "IDD_CLAM_MGMT", pNSResModule), NSRoot(pCtx)
{
try
{
	_pMatBox 	        = new NSMngtListeLocalActsWin(this, IDC_CIM10BOX) ;
  _bNaturallySorted = true ;
	_iSortedColumn    = 0 ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog ctor.", standardError, 0) ;
}
}

LocalActsManagementDialog::~LocalActsManagementDialog(){
	delete _pMatBox ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des codes CCAM
//---------------------------------------------------------------------------
void
LocalActsManagementDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitActsArray()) || (_aActsArray.empty()))
		return ;

	AfficheListe() ;

  _pMatBox->SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}
catch (...)
{
	erreur("Exception LocalActsManagementDialog::SetupWindow.", standardError, 0) ;
}
}

void
LocalActsManagementDialog::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;

	if (sizeType != SIZE_MINIMIZED)
	{
  	int LEFT_OFFSET  = 8 ;
    int TOP_OFFSET   = 8 ;
    int WIDTH_OFFSET = 24 ;

		ClassLib::TRect rect      = GetWindowRect() ;
    ClassLib::TRect list_rect = _pMatBox->GetWindowRect() ;
		_pMatBox->SetWindowPos(0, LEFT_OFFSET, TOP_OFFSET,
                              rect.Width() - WIDTH_OFFSET, list_rect.Height(),
                              SWP_NOZORDER ) ;
	}
}

bool
LocalActsManagementDialog::InitActsArray()
{
try
{
	_aActsArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSClam Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des actes libres.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des actes libres est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des actes libres est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er acte libre.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aActsArray.push_back(new NSLibreInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès à l'acte libre suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture de l'acte libre.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception LocalActsManagementDialog::InitActsArray.", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
LocalActsManagementDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
  int iCodeChoisi = _pMatBox->IndexItemSelect() ;
  if (iCodeChoisi < 0)
    return ;

  NSLibreInfo *pSelectedAct = _aActsArray[iCodeChoisi] ;

  EditLocalActDialog actDlg(this, pContexte, pSelectedAct, this) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  updateAct(pSelectedAct) ;

  AfficheListe() ;
}

void
LocalActsManagementDialog::CmOk()
{
	TDialog::CmOk() ;
}

void
LocalActsManagementDialog::CmNewAct()
{
  NSLibreInfo newActInfo(pContexte) ;

  EditLocalActDialog actDlg(this, pContexte, &newActInfo, this) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  if (false == createAct(&newActInfo))
    return ;

  _aActsArray.push_back(new NSLibreInfo(newActInfo)) ;
  AfficheListe() ;
}

void
LocalActsManagementDialog::CmNewActFrom()
{
  int iSel = _pMatBox->IndexItemSelect() ;
  if (iSel < 0)
    return ;

  NSLibreInfo newActInfo(*(_aActsArray[iSel])) ;
  newActInfo.setCode(string("")) ;

  EditLocalActDialog actDlg(this, pContexte, &newActInfo, this) ;
  if ((actDlg.Execute()) != IDOK)
   	return ;

  if (false == createAct(&newActInfo))
    return ;

  _aActsArray.push_back(new NSLibreInfo(newActInfo)) ;
  AfficheListe() ;
}

bool
LocalActsManagementDialog::createAct(NSLibreInfo* pActInfo)
{
  if ((NSLibreInfo*) NULL == pActInfo)
    return false ;

  NSClam Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des actes libres.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  Base._Donnees = pActInfo->_Donnees ;

  Base.lastError = Base.appendRecord() ;
  if (DBIERR_NONE != Base.lastError)
  {
  	erreur("Erreur à l'ajout dans la base des actes libres.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  Base.lastError = Base.close() ;
	if (DBIERR_NONE != Base.lastError)
		erreur("Erreur à la fermeture de la base Clam.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;

  return true ;
}

bool
LocalActsManagementDialog::updateAct(NSLibreInfo* pActInfo)
{
  if ((NSLibreInfo*) NULL == pActInfo)
    return false ;

  if (string("") == pActInfo->getCode())
    return false ;

  NSClam Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des actes libres.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  string sNum = pActInfo->getCode() ;

	Base.lastError = Base.chercheClef(&sNum,
                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la recherche de la fiche d'acte libre.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à lecture du fichier Clam.db", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

	Base._Donnees = pActInfo->_Donnees ;

	Base.lastError = Base.modifyRecord(TRUE) ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la modification de la fiche d'acte libre.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.close() ;
	if (DBIERR_NONE != Base.lastError)
		erreur("Erreur à la fermeture de la base Clam.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;

	return true ;
}

bool
LocalActsManagementDialog::existAct(string sCode)
{
  NSClam Base(pContexte->getSuperviseur()) ;

  Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture de la base des actes libres.", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

	Base.lastError = Base.chercheClef(&sCode,
                                    "",
                                    0,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;

	if (DBIERR_NONE != Base.lastError)
	{
		Base.close() ;
		return false ;
	}

  Base.lastError = Base.getRecord() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à lecture du fichier Clam.db", standardError, Base.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		Base.close() ;
		return false ;
	}

  Base.close() ;

  if (Base.getCode() == sCode)
    return true ;

  return false ;
}

void
LocalActsManagementDialog::InitListe()
{
  TListWindColumn colCode("Code", 60, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colCode) ;
	TListWindColumn colLibelle("Libelle", 400, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colLibelle) ;
  TListWindColumn colExam("Examen", 200, TListWindColumn::Left, 2) ;
	_pMatBox->InsertColumn(2, colExam) ;
	TListWindColumn colPrixE("Montant €", 90, TListWindColumn::Right, 3) ;
	_pMatBox->InsertColumn(3, colPrixE) ;
  string sSigle = string("Montant ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 90, TListWindColumn::Right, 4) ;
	_pMatBox->InsertColumn(4, colPrixF) ;
	TListWindColumn colFreq("Fréquence", 80, TListWindColumn::Right, 5) ;
	_pMatBox->InsertColumn(5, colFreq) ;
}

void
LocalActsManagementDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aActsArray.empty())
    return ;

	for (LibreReverseIter i = _aActsArray.rbegin() ; _aActsArray.rend() != i ; i++)
	{
    string sCode = (*i)->getCode() ;
    TListWindItem Item(sCode.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
LocalActsManagementDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aActsArray.size())
  	return ;

  string sBuffer = string("") ;

  NSLibreInfo* pLibreInfo = _aActsArray[index] ;
  if ((NSLibreInfo*) NULL == pLibreInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Label

    	sBuffer = pLibreInfo->getLibelle() ;
      break ;

    case 2 : // Examen
    {
      string sType = pLibreInfo->getModule() ;
      if (string("") != sType)
        pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sType, &sBuffer) ;
      break ;
    }

		case 3 : // Montant Euros

    	sBuffer = NSBlocFse16::GetSumInUnits(pLibreInfo->getPrixE()) ;
      break ;

    case 4 : // Montant seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pLibreInfo->getPrixF()) ;
      break ;

    case 5 : // Frequency

    	sBuffer = pLibreInfo->getFrequence() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
LocalActsManagementDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0  : sortByCode() ;      break ;
    case 1  : sortByLibelle() ;   break ;
    case 2  : sortByExam() ;      break ;
    case 5  : sortByFrequency() ; break ;
  }
}

void
LocalActsManagementDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn    = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
LocalActsManagementDialog::sortByCode()
{
	swapSortOrder(0) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByCodeInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByCodeSup) ;

  AfficheListe() ;
}

void
LocalActsManagementDialog::sortByLibelle()
{
	swapSortOrder(1) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByLibelleInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByLibelleSup) ;

  AfficheListe() ;
}

void
LocalActsManagementDialog::sortByExam()
{
	swapSortOrder(2) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByExamInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByExamSup) ;

  AfficheListe() ;
}

void
LocalActsManagementDialog::sortByFrequency()
{
	swapSortOrder(5) ;

  if (_aActsArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByFreqInf) ;
  else
    sort(_aActsArray.begin(), _aActsArray.end(), ClamSortByFreqSup) ;

  AfficheListe() ;
}

#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSMngtListeLocalActsWin, TListWindow)
	EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_EDIT_ACT,     CmEditAct),
  EV_COMMAND(CM_NEW_ACT,      CmNewAct),
  EV_COMMAND(CM_NEW_ACT_FROM, CmNewActFrom),
END_RESPONSE_TABLE;

void
NSMngtListeLocalActsWin::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSMngtListeLocalActsWin::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

void
NSMngtListeLocalActsWin::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  // création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  OWL::TPopupMenu menu ;

  NSSuper* pSuper = pDlg->pContexte->getSuperviseur() ;

  string sNew = pSuper->getText("accountingLocalActs", "newLocalAct") ;
  menu.AppendMenu(MF_STRING, CM_NEW_ACT, sNew.c_str()) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;		string sEdit    = pSuper->getText("accountingLocalActs", "editThisLocalAct") ;		string sNewFrom = pSuper->getText("accountingLocalActs", "newLocalActFromThisOne") ;

    menu.AppendMenu(MF_STRING, CM_NEW_ACT_FROM, sNewFrom.c_str()) ;
    menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu.AppendMenu(MF_STRING, CM_EDIT_ACT, sEdit.c_str()) ;
  }

  ClientToScreen(lp) ;
  menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSMngtListeLocalActsWin::CmNewAct()
{
  pDlg->CmNewAct() ;
}

void
NSMngtListeLocalActsWin::CmNewActFrom()
{
  pDlg->CmNewActFrom() ;
}

void
NSMngtListeLocalActsWin::CmEditAct()
{
  pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSMngtListeLocalActsWin::IndexItemSelect()
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
#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------
//
//  Méthodes de EditLotDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

#ifndef _ENTERPRISE_DLL
DEFINE_RESPONSE_TABLE1(EditLocalActDialog, NSUtilDialog)	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

EditLocalActDialog::EditLocalActDialog(TWindow* pere, NSContexte* pCtx, NSLibreInfo* pActInfo, LocalActsManagementDialog* pListMgmtDlg)
                   :NSUtilDialog(pere, pCtx, "IDD_CLAM_EDIT", pNSResModule)
{
	// Initialisation des donnees
  _pListMgmtDlg = pListMgmtDlg ;
  _pLibreInfo   = pActInfo ;	// Création de tous les "objets de contrôle"
  _pCode      = new NSUtilEdit2(pContexte, this, IDC_CLAM_CODE, CLAM_CODE_LEN) ;
  _pLabel     = new NSUtilEdit2(pContexte, this, IDC_CLAM_LABEL, CLAM_LIBELLE_LEN) ;
  _pExam      = new NSUtilLexique(pContexte, this, IDC_CLAM_EXAM, pCtx->getDico()) ;
  _pMontantE  = new NSUtilEditSomme(pContexte, this, IDC_CLAM_PRIX_E) ;
  _pMontantF  = new NSUtilEditSomme(pContexte, this, IDC_CLAM_PRIX_F) ;
  _pFrequency = new NSUtilEdit2(pContexte, this, IDC_CLAM_FREQ, CLAM_FREQ_LEN) ;
  _pSigle		  = new TStatic(this, IDC_LOT_SIGLE) ;}
EditLocalActDialog::~EditLocalActDialog(){
  delete _pLabel ;
  delete _pCode ;
  delete _pExam ;
  delete _pMontantE	;
	delete _pMontantF ;
	delete _pFrequency ;
  delete _pSigle ;
}
void
EditLocalActDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;  _pCode->SetText(_pLibreInfo->getCode()) ;  _pLabel->SetText(_pLibreInfo->getLibelle()) ;  _pExam->setLabel(_pLibreInfo->getModule()) ;
  string sSigle = pContexte->getSuperviseur()->getSigle() ;
	_pSigle->SetText(sSigle.c_str()) ;

  _pMontantE->setSomme(_pLibreInfo->getPrixE()) ;
  _pMontantF->setSomme(_pLibreInfo->getPrixF()) ;

  _pFrequency->SetText(_pLibreInfo->getFrequence().c_str()) ;
}

voidEditLocalActDialog::CmOk(){  string sText ;  _pCode->GetText(sText) ;  if (_pListMgmtDlg->existAct(sText))  {    string sCaption = string("Message ") + pContexte->getSuperviseur()->getAppName() ;    int ret = MessageBox("Ce code d'acte existe déjà dans la base.", sCaption.c_str(), MB_OK) ;    return ;  }	_pLibreInfo->setCode(sText) ;  _pLabel->GetText(sText) ;	_pLibreInfo->setLibelle(sText) ;
  _pLibreInfo->setModule(_pExam->getCodeSens()) ;

	_pMontantE->getSomme(&sText) ;
	_pLibreInfo->setPrixE(sText) ;
  _pMontantF->getSomme(&sText) ;
	_pLibreInfo->setPrixF(sText) ;

  _pFrequency->GetText(sText) ;
	_pLibreInfo->setFrequence(sText) ;

	NSUtilDialog::CmOk() ;
}
voidEditLocalActDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}#endif // #ifndef _ENTERPRISE_DLL

// **************************************************************************
//  Méthodes de NSLotData
// **************************************************************************

//---------------------------------------------------------------------------
//  Met à vide les variables de la fiche
//---------------------------------------------------------------------------
void
NSLotData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(_code,         0, LOT_CODE_LEN + 1) ;
  memset(_status,       0, LOT_STATUS_LEN + 1) ;
  memset(_date_achat,   0, LOT_DATE_LEN + 1) ;
  memset(_barre,        0, LOT_BARRE_LEN + 1) ;
	memset(_libelle,      0, LOT_LIBELLE_LEN + 1) ;
  memset(_lexique,      0, LOT_LEXIQUE_LEN + 1) ;
  memset(_module,       0, LOT_MODULE_LEN + 1) ;
	memset(_prix_vente_f, 0, LOT_PRIX_LEN + 1) ;
  memset(_prix_vente_e, 0, LOT_PRIX_LEN + 1) ;
	memset(_prix_achat_f, 0, LOT_PRIX_LEN + 1) ;
  memset(_prix_achat_e, 0, LOT_PRIX_LEN + 1) ;
  memset(_quantite,     0, LOT_COUNT_LEN + 1) ;
  memset(_reste,        0, LOT_COUNT_LEN + 1) ;
	memset(_alerte,       0, LOT_COUNT_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSLotData::NSLotData(const NSLotData& rv)
{
  strcpy(_code,         rv._code) ;
  strcpy(_status,       rv._status) ;
  strcpy(_date_achat,   rv._date_achat) ;
  strcpy(_barre,        rv._barre) ;
	strcpy(_libelle,      rv._libelle) ;
  strcpy(_lexique,      rv._lexique) ;
  strcpy(_module,       rv._module) ;
	strcpy(_prix_vente_f, rv._prix_vente_f) ;
  strcpy(_prix_vente_e, rv._prix_vente_e) ;
	strcpy(_prix_achat_f, rv._prix_achat_f) ;
  strcpy(_prix_achat_e, rv._prix_achat_e) ;
  strcpy(_quantite,     rv._quantite) ;
  strcpy(_reste,        rv._reste) ;
	strcpy(_alerte,       rv._alerte) ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSLotData&
NSLotData::operator=(const NSLotData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(_code,         src._code) ;
  strcpy(_status,       src._status) ;
  strcpy(_date_achat,   src._date_achat) ;
  strcpy(_barre,        src._barre) ;
	strcpy(_libelle,      src._libelle) ;
  strcpy(_lexique,      src._lexique) ;
  strcpy(_module,       src._module) ;
	strcpy(_prix_vente_f, src._prix_vente_f) ;
  strcpy(_prix_vente_e, src._prix_vente_e) ;
	strcpy(_prix_achat_f, src._prix_achat_f) ;
  strcpy(_prix_achat_e, src._prix_achat_e) ;
  strcpy(_quantite,     src._quantite) ;
  strcpy(_reste,        src._reste) ;
	strcpy(_alerte,       src._alerte) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLotData::operator == (const NSLotData& o) const
{
	if ((strcmp(_code, o._code) == 0))
		return 1 ;
	else
		return 0 ;
}

NSLotData::STATUSLOT
NSLotData::getStatusType() const
{
  switch(_status[0])
  {
    case 'S' : return statusStock ;
    case 'O' : return statusOpen ;
    case 'E' : return statusEmpty ;
  }

  return statusUndefined ;
}

void
NSLotData::setStatusType(STATUSLOT iStatus)
{
  switch(iStatus)
  {
    case statusStock : strcpy(_status, "S") ; return ;
    case statusOpen  : strcpy(_status, "O") ; return ;
    case statusEmpty : strcpy(_status, "E") ; return ;
  }
  strcpy(_status, "") ;
}

// **************************************************************************
//  Méthodes de NSLot
// **************************************************************************

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSLot::NSLot(NSSuper* pSuper)
      :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSLot::~NSLot()
{
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSLot::alimenteFiche()
{
	alimenteChamp(_Donnees._code,         LOT_CODE_FIELD,         LOT_CODE_LEN) ;
  alimenteChamp(_Donnees._status,       LOT_STATUS_FIELD,       LOT_STATUS_LEN) ;
  alimenteChamp(_Donnees._date_achat,   LOT_DATE_ACHAT_FIELD,   LOT_DATE_LEN) ;
	alimenteChamp(_Donnees._barre,        LOT_BARRE_FIELD,        LOT_BARRE_LEN) ;
  alimenteChamp(_Donnees._libelle,      LOT_LIBELLE_FIELD,      LOT_LIBELLE_LEN) ;
	alimenteChamp(_Donnees._lexique,      LOT_LEXIQUE_FIELD,      LOT_LEXIQUE_LEN) ;
  alimenteChamp(_Donnees._module,       LOT_MODULE_FIELD,       LOT_MODULE_LEN) ;
	alimenteChamp(_Donnees._prix_vente_f, LOT_PRIX_VENTE_F_FIELD, LOT_PRIX_LEN) ;
  alimenteChamp(_Donnees._prix_vente_e, LOT_PRIX_VENTE_E_FIELD, LOT_PRIX_LEN) ;
  alimenteChamp(_Donnees._prix_achat_f, LOT_PRIX_ACHAT_F_FIELD, LOT_PRIX_LEN) ;
  alimenteChamp(_Donnees._prix_achat_e, LOT_PRIX_ACHAT_E_FIELD, LOT_PRIX_LEN) ;
	alimenteChamp(_Donnees._quantite,     LOT_QUANTITY_FIELD,     LOT_COUNT_LEN) ;
  alimenteChamp(_Donnees._reste,        LOT_RESTE_FIELD,        LOT_COUNT_LEN) ;
  alimenteChamp(_Donnees._alerte,       LOT_ALERTE_FIELD,       LOT_COUNT_LEN) ;
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSLot::videFiche()
{
  videChamp(_Donnees._code,         LOT_CODE_FIELD,         LOT_CODE_LEN) ;
  videChamp(_Donnees._status,       LOT_STATUS_FIELD,       LOT_STATUS_LEN) ;
  videChamp(_Donnees._date_achat,   LOT_DATE_ACHAT_FIELD,   LOT_DATE_LEN) ;
	videChamp(_Donnees._barre,        LOT_BARRE_FIELD,        LOT_BARRE_LEN) ;
  videChamp(_Donnees._libelle,      LOT_LIBELLE_FIELD,      LOT_LIBELLE_LEN) ;
	videChamp(_Donnees._lexique,      LOT_LEXIQUE_FIELD,      LOT_LEXIQUE_LEN) ;
  videChamp(_Donnees._module,       LOT_MODULE_FIELD,       LOT_MODULE_LEN) ;
	videChamp(_Donnees._prix_vente_f, LOT_PRIX_VENTE_F_FIELD, LOT_PRIX_LEN) ;
  videChamp(_Donnees._prix_vente_e, LOT_PRIX_VENTE_E_FIELD, LOT_PRIX_LEN) ;
  videChamp(_Donnees._prix_achat_f, LOT_PRIX_ACHAT_F_FIELD, LOT_PRIX_LEN) ;
  videChamp(_Donnees._prix_achat_e, LOT_PRIX_ACHAT_E_FIELD, LOT_PRIX_LEN) ;
	videChamp(_Donnees._quantite,     LOT_QUANTITY_FIELD,     LOT_COUNT_LEN) ;
  videChamp(_Donnees._reste,        LOT_RESTE_FIELD,        LOT_COUNT_LEN) ;
  videChamp(_Donnees._alerte,       LOT_ALERTE_FIELD,       LOT_COUNT_LEN) ;
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSLot::open()
{
	char tableName[] = "LOTS.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_COMPTA) ;
  return(lastError) ;
}

DBIResult
NSLot::getRecordByCode(string sCode, NSLotInfo* pResult, bool bOpenClose, bool bVerbose)
{
	if (((NSLotInfo*) NULL == pResult) || (string("") == sCode))
  	return DBIERR_NONE ;

  pResult->metAZero() ;

	// Opening the table
	//
  if (bOpenClose)
  {
		lastError = open() ;
		if (lastError != DBIERR_NONE)
		{
  		if (bVerbose)
				erreur("Erreur à l'ouverture du fichier LOTS.", standardError, lastError) ;
    	return lastError ;
  	}
  }
	//
	// Recherche du libelle dont le code est dans le champ complément
	//
	lastError = chercheClef(&sCode,
                          "",
                          0,
                          keySEARCHEQ,
                          dbiWRITELOCK) ;
	if (lastError != DBIERR_NONE)
	{
  	DBIResult dbError = lastError ;
  	if (bVerbose)
    {
    	string sError = string("Erreur à la recherche du code LOTS : ") + sCode ;
			erreur(sError.c_str(), standardError, lastError) ;
		}
    if (bOpenClose)
    	close() ;
		return dbError ;
	}
  //
  // Getting the record from table
  //
	lastError = getRecord() ;
	if (lastError != DBIERR_NONE)
	{
  	DBIResult dbError = lastError ;
  	if (bVerbose)
    {
    	string sError = string("Erreur à la lecture du code LOTS : ") + sCode ;
			erreur(sError.c_str(), standardError, lastError) ;
		}
    if (bOpenClose)
    	close() ;
		return dbError ;
	}
	//
	// Si tout a bien marché, on met à jour pMatInfo
	//
	pResult->_Donnees = _Donnees ;

  if (bOpenClose)
		close() ;

	return DBIERR_NONE ;
}

bool
NSLot::ChercheNumLot(string& sNumLot)
{
  sNumLot = string("") ;

	//
	// Préparation des compteurs mini et maxi
	//
  string sPremOrdre = string(LOT_CODE_LEN, '0') ;
  string sDernOrdre = string(LOT_CODE_LEN, 'Z') ;

  // Entrée en section critique
#ifndef N_TIERS
  if (false == pContexte->DemandeAcces("LOTS"))
  {
    erreur("Impossible d'obtenir l'accès à la base Lots.", standardError, 0, _pSuper->donneMainWindow()->GetHandle()) ;
    return false ;
  }
#endif
  lastError = open() ;
  if (DBIERR_NONE != lastError)
  {
    erreur("Erreur à l'ouverture de la base lots.db", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
    pContexte->LibereAcces("LOTS");
#endif
    return false ;
  }

	//
	// On tente de se positionner sur le dernier code possible : "zzzzzzzz"
	//
  string sNumero = sDernOrdre ;

  lastError = chercheClef(&sNumero,
                          "",
                          0,
                          keySEARCHGEQ,
                          dbiWRITELOCK) ;

	//	// Si on se trouve positionné en fin de fichier on recule
	//
	if (DBIERR_EOF == lastError)
    lastError = precedent(dbiWRITELOCK) ;
	//
	// Toute autre erreur est anormale
	//
	else if ((DBIERR_NONE != lastError) && (DBIERR_BOF != lastError))
	{
		erreur("Le fichier lots.db est endommagé.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("LOTS") ;#endif
		return false ;	}
	//
	// On récupère l'enregistrement si fichier non vide
	//
  if (DBIERR_BOF != lastError)
  {
    lastError = getRecord() ;

    if (DBIERR_NONE != lastError)		{
      erreur("erreur à la lecture du fichier lots.db", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
      close() ;
#ifndef N_TIERS
      pContexte->LibereAcces("LOTS") ;#endif
			return false ;    }
  }

	//	// Si on est en fin ou début de fichier, c'est qu'il est vide
	//
	if ((DBIERR_BOF       == lastError) ||
      (DBIERR_EOF       == lastError) ||
      (DBIERR_NOCURRREC == lastError))
  {
    sNumLot = sPremOrdre ;
    _Donnees.metAZero() ;
    strcpy(_Donnees._code, sNumLot.c_str()) ;

    lastError = appendRecord() ;
    if (DBIERR_NONE != lastError)    {
      erreur("Erreur à l'ajout d'une fiche Lots vierge.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
      close() ;
#ifndef N_TIERS
      pContexte->LibereAcces("LOTS") ;
#endif
      return false ;    }
    close() ;

#ifndef N_TIERS    pContexte->LibereAcces("LOTS") ;#endif
		return true ;  }

	//	// Toute autre erreur est anormale
	//
	if (DBIERR_NONE != lastError)
	{
		erreur("Le fichier lots.db est endommagé.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("LOTS") ;#endif
		return false ;	}

	//	// On est positionné sur le dernier document du patient, que l'on doit
	// incrémenter pour avoir le nouveau code
	//
	char compteur[LOT_CODE_LEN + 1] ;
	strcpy(compteur, _Donnees._code) ;

	//
	// On incrémente le compteur
	//
	int calculer = 1 ;
	size_t i = strlen(compteur) - 1 ;

	while (calculer)  {
    int j = (int) compteur[i] ;
		j++ ;

		if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))		{
			compteur[i] = (char) j ;
			calculer = 0 ;
		}
		else if ((j > '9') && (j < 'A'))
		{
			compteur[i] = 'A' ;
			calculer = 0 ;
		}
		else
		{
			compteur[i] = '0' ;
			if (0 == i)
			{
				erreur("Compteur de fiches lots saturé.", standardError, 0, _pSuper->donneMainWindow()->GetHandle()) ;
        close() ;
#ifndef N_TIERS
        pContexte->LibereAcces("LOTS");#endif
				return false ;			}
			i-- ;
		}
	}

	sNumLot = string(compteur) ;
  // on stocke une fiche vierge pour etre plus sur  _Donnees.metAZero();
  strcpy(_Donnees._code, sNumLot.c_str()) ;

  lastError = appendRecord() ;
  if (DBIERR_NONE != lastError)
  {
    erreur("Erreur à l'ajout d'une fiche Lots vierge.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
#ifndef N_TIERS
    pContexte->LibereAcces("LOTS") ;#endif
    return false ;  }
  lastError = close() ;  if (DBIERR_NONE != lastError)
  {
    erreur("Erreur à la fermeture de la base lots.db", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
#ifndef N_TIERS
    pContexte->LibereAcces("LOTS") ;#endif
    return false ;  }

  // Sortie de section critique#ifndef N_TIERS  pContexte->LibereAcces("LOTS") ;
#endif

	return true ;
}

void
NSLot::decrementeLot(string sNumLot)
{
  if (string("") == sNumLot)
    return ;

  // Open table
  //
  lastError = open() ;
  if (DBIERR_NONE != lastError)
  {
    erreur("Erreur à l'ouverture de la base lots.db, impossible de mettre à jour le compteur.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    return ;
  }

	// On tente de se positionner sur le lot
	//
  lastError = chercheClef(&sNumLot,
                          "",
                          0,
                          keySEARCHEQ,
                          dbiWRITELOCK) ;

  if (DBIERR_NONE != lastError)
  {
    erreur("Il est impossible de retrouver le lot correspondant dans la base des Lots pour décrémenter son compteur.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
    return ;
  }

  lastError = getRecord() ;

  if (DBIERR_NONE != lastError)  {
    erreur("erreur à la lecture du fichier lots.db", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
    return ;
  }

  if (getCode() != sNumLot)
  {
    erreur("Il est impossible de retrouver le lot correspondant dans la base des Lots pour décrémenter son compteur.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
    close() ;
    return ;
  }

  // Decrement counter
  //
  int iRemains = StringToInt(getReste()) ;
  if (0 == iRemains)
    return ;

  iRemains-- ;
  string sRemains = IntToString(iRemains) ;

  setReste(sRemains) ;

  lastError = modifyRecord(TRUE) ;
	if (DBIERR_NONE != lastError)
	{
		erreur("Erreur à la modification de la fiche Lots pour décrémenter le compteur.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;
		close() ;
		return ;
	}

	lastError = close() ;
	if (DBIERR_NONE != lastError)
		erreur("Erreur à la fermeture de la base Lots.", standardError, lastError, _pSuper->donneMainWindow()->GetHandle()) ;

  // Check alert
  //
  int iAlertThreshold = StringToInt(getAlerte()) ;
  if (iRemains <= iAlertThreshold)
  {
    string sMsg = string("Le stock de ") + getLibelle() + string(" est bas (") + sRemains + string(")") ;
    string sCaption = string("Message ") + _pSuper->getAppName() ;
    int ret = _pSuper->donneMainWindow()->MessageBox(sMsg.c_str(), sCaption.c_str(), MB_OK) ;
  }
}
#endif // #ifndef _ENTERPRISE_DLL

// **************************************************************************
//  Méthodes de NSCim10Info
// **************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSLotInfo::NSLotInfo(NSContexte* pCtx)
          :NSRoot(pCtx)
{
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSCcam
//---------------------------------------------------------------------------
NSLotInfo::NSLotInfo(const NSLot* pLot, NSContexte* pCtx)
          :NSRoot(pCtx)
{
  if (pLot)
	  _Donnees = pLot->_Donnees ;
}
#endif // #ifndef _ENTERPRISE_DLL

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSLotInfo::NSLotInfo(const NSLotInfo& rv)
          :NSRoot(rv.pContexte)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSLotInfo&
NSLotInfo::operator=(const NSLotInfo& src)
{
  if (this == &src)
  	return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSLotInfo::operator==(const NSLotInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

bool
LotSortByLibelleInf(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLibelle()) < pseumaj(pCCAM2->getLibelle())) ;
}
bool
LotSortByLibelleSup(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pseumaj(pCCAM1->getLibelle()) > pseumaj(pCCAM2->getLibelle())) ;
}

bool
LotSortByStatusInf(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pCCAM1->getStatus() < pCCAM2->getStatus()) ;
}
bool
LotSortByStatusSup(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pCCAM1->getStatus() > pCCAM2->getStatus()) ;
}

bool
LotSortByLexiqueInf(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getLexique() ;
  string sLexiqueCode2  = pCCAM2->getLexique() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) < pseumaj(sLexiqueLabel2)) ;
}
bool
LotSortByLexiqueSup(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getLexique() ;
  string sLexiqueCode2  = pCCAM2->getLexique() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) > pseumaj(sLexiqueLabel2)) ;
}

bool
LotSortByExamInf(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getModule() ;
  string sLexiqueCode2  = pCCAM2->getModule() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) < pseumaj(sLexiqueLabel2)) ;
}
bool
LotSortByExamSup(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
  NSContexte *pContexte = pCCAM1->pContexte ;
  string sLexiqueCode1  = pCCAM1->getModule() ;
  string sLexiqueCode2  = pCCAM2->getModule() ;
  string sLexiqueLabel1 = string("") ;
  string sLexiqueLabel2 = string("") ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode1, &sLexiqueLabel1) ;
  pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sLexiqueCode2, &sLexiqueLabel2) ;

	return (pseumaj(sLexiqueLabel1) > pseumaj(sLexiqueLabel2)) ;
}

bool
LotSortByDateInf(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pCCAM1->getDateAchat() < pCCAM2->getDateAchat()) ;
}
bool
LotSortByDateSup(NSLotInfo *pCCAM1, NSLotInfo *pCCAM2)
{
	return (pCCAM1->getDateAchat() > pCCAM2->getDateAchat()) ;
}

#ifndef _ENTERPRISE_DLL
// **************************************************************************//  Méthodes de ChoixCcamDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(ChoixLotsDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_SELCHANGE, CmSelectMateriel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_DBLCLK, 	CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_CIM10BOX, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_CIM10BOX, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(ID_ELARGIR, CmElargir),
END_RESPONSE_TABLE;

ChoixLotsDialog::ChoixLotsDialog(TWindow* pere, NSContexte* pCtx,
                                   string* pModu, NSLotInfo* pCodeChoix)
                :TDialog(pere, "IDD_LOT", pNSResModule), NSRoot(pCtx)
{
try
{
  _pChoiceButton = new OWL::TButton(this, ID_ELARGIR) ;
	_pMatBox 	     = new NSListeLotsWindow(this, IDC_CIM10BOX) ;
	_iCodeChoisi   = 0 ;
	_pLotChoisi    = pCodeChoix ;
  _bGlobalList   = false ;

  if (pModu && (string("") != *pModu))
	  _sExam = *pModu ;
  else
  {
    _sExam       = string("") ;
    _bGlobalList = false ;
  }

  _bNaturallySorted = true ;
	_iSortedColumn    = 0 ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
ChoixLotsDialog::~ChoixLotsDialog()
{
	//
	// Si un matériel a été choisi
	//
	if ((_iCodeChoisi > 0) && _pLotChoisi)
		*_pLotChoisi = *(_aMatArray[_iCodeChoisi - 1]) ;

	delete _pMatBox ;
  delete _pChoiceButton ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des codes CCAM
//---------------------------------------------------------------------------
void
ChoixLotsDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitLotArray()) || (_aMatArray.empty()))
		return ;

	AfficheListe() ;

  if (string("") == _pLotChoisi->getCode())
	{
  	_pMatBox->SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
    return ;
  }

	//
	// Si un code avait déjà été sélectionné, on le coche
	//
	int iNumApp = 0 ;
	LotsIter i = _aMatArray.begin() ;
	for ( ; (_aMatArray.end() != i) && ((*i)->getCode() != _pLotChoisi->getCode()) ;
       		                                           i++, iNumApp++) ;

	if (_aMatArray.end() != i)
  	_pMatBox->SetItemState(iNumApp, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog::SetupWindow.", standardError, 0) ;
}
}

void
ChoixLotsDialog::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;

	if (sizeType != SIZE_MINIMIZED)
	{
  	int LEFT_OFFSET  = 8 ;
    int TOP_OFFSET   = 8 ;
    int WIDTH_OFFSET = 24 ;

		ClassLib::TRect rect      = GetWindowRect() ;
    ClassLib::TRect list_rect = _pMatBox->GetWindowRect() ;
		_pMatBox->SetWindowPos(0, LEFT_OFFSET, TOP_OFFSET,
                              rect.Width() - WIDTH_OFFSET, list_rect.Height(),
                              SWP_NOZORDER ) ;
	}
}

bool
ChoixLotsDialog::InitLotArray()
{
  if (_bGlobalList)
    return InitGlobalLotArray() ;

  return InitLotArrayForCode() ;
}

bool
ChoixLotsDialog::InitLotArrayForCode()
{
try
{
	_aMatArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSLot Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des lots.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.chercheClef(&_sExam,
                                    "MODULE_INDEX",
                                    NODEFAULTINDEX,
                                    keySEARCHEQ,
                                    dbiWRITELOCK) ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Il n'existe pas de code pour ce lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while ((DBIERR_NONE == Base.lastError) && (Base.getModule() == _sExam))
	{
		//
		// Ajout du matériel à l'array
		//
    // sLibelleAffiche = pBase->pDonnees->code;
    // if (pBase->pDonnees->libelle[0] != '\0')
    // 	sLibelleAffiche += string(" : ") + pBase->pDonnees->libelle;

		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aMatArray.push_back(new NSLotInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au lot suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du lot.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog::InitLotArray.", standardError, 0) ;
  return false ;
}
}

bool
ChoixLotsDialog::InitGlobalLotArray()
{
try
{
	_aMatArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSLot Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes LOTS.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des lots est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des lots est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aMatArray.push_back(new NSLotInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au lot suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du lot.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog::InitGlobalLotArray.", standardError, 0) ;
  return false ;
}
}

void
ChoixLotsDialog::CmElargir()
{
	if (string("") == _sExam)
    return ;

  _bGlobalList = !_bGlobalList ;

  InitLotArray() ;

  _bNaturallySorted = false ;

  if (_bGlobalList)
  {
    _pChoiceButton->SetCaption("Réduire le choix") ;
    sortByLibelle() ;
  }
  else
  {
    _pChoiceButton->SetCaption("Elargir le choix") ;
    sortByLibelle() ;
  }
  // AfficheListe() ;  // Déjà fait par les "sort"
}

//---------------------------------------------------------------------------//  Réponse à selChanged
//  Prend acte de la sélection du matériel
//---------------------------------------------------------------------------
void
ChoixLotsDialog::CmSelectMateriel(WPARAM /* Cmd */)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	_iCodeChoisi = _pMatBox->IndexItemSelect() + 1 ;
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
ChoixLotsDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
	_iCodeChoisi = _pMatBox->IndexItemSelect() + 1 ;
	TDialog::CmOk();
}

void
ChoixLotsDialog::CmOk()
{
 	int iSelected = _pMatBox->IndexItemSelect()  ;
  if (-1 == iSelected)
  	return ;

  _iCodeChoisi = iSelected + 1 ;
	TDialog::CmOk() ;
}

//---------------------------------------------------------------------------
//  Bouton Cancel
//  Annule MaterielChoisi et appelle Cancel()
//---------------------------------------------------------------------------
void
ChoixLotsDialog::CmCancel()
{
	_iCodeChoisi = 0 ;
	TDialog::CmCancel() ;
}

void
ChoixLotsDialog::InitListe()
{
	TListWindColumn colCode("Code", 60, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colCode) ;
	TListWindColumn colLibelle("Libelle", 380, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colLibelle) ;
	TListWindColumn colPrixE("Prix €", 50, TListWindColumn::Right, 2) ;
	_pMatBox->InsertColumn(2, colPrixE) ;
  string sSigle = string("Prix ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixF((char*) sSigle.c_str(), 50, TListWindColumn::Right, 3) ;
	_pMatBox->InsertColumn(3, colPrixF) ;
	TListWindColumn colActif("Freq", 30, TListWindColumn::Left, 4) ;
	_pMatBox->InsertColumn(4, colActif) ;
}

void
ChoixLotsDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aMatArray.empty())
    return ;

	for (LotsReverseIter i = _aMatArray.rbegin() ; _aMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getCode() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
ChoixLotsDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aMatArray.size())
  	return ;

  string sBuffer = string("") ;
  size_t iLen ;

  NSLotInfo* pLotInfo = _aMatArray[index] ;
  if ((NSLotInfo*) NULL == pLotInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Libelle
    	sBuffer = pLotInfo->getLibelle() ;
      break ;

		case 2 : // Prix Euros
    	sBuffer = pLotInfo->getPrixVenteE() ;
      iLen = strlen(sBuffer.c_str()) ;
      if (iLen > 2)
      	sBuffer = string(sBuffer, 0, iLen - 2) + string(",") + string(sBuffer, iLen - 2, 2) ;
      else if (string("0") == sBuffer)
        sBuffer = string("0,00") ;

      break ;

    case 3 : // Prix seconde monnaie
    	sBuffer = pLotInfo->getPrixVenteF() ;
      iLen = strlen(sBuffer.c_str()) ;
      if (iLen > 2)
      	sBuffer = string(sBuffer, 0, iLen - 2) + string(",") + string(sBuffer, iLen - 2, 2) ;
      else if (string("0") == sBuffer)
        sBuffer = string("0,00") ;

      break ;

    case 4 : // Prix
    	sBuffer = pLotInfo->getQuantite() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
ChoixLotsDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0  : sortByLibelle() ; break ;
  }
}

void
ChoixLotsDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn    = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
ChoixLotsDialog::sortByLibelle()
{
	swapSortOrder(1) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLibelleInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLibelleSup) ;

  AfficheListe() ;
}

#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListeLotsWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListeLotsWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSListeLotsWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmOk() ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSListeLotsWindow::IndexItemSelect()
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
#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// **************************************************************************
//  Méthodes de LotsManagementDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(LotsManagementDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_LOTS, LBN_DBLCLK, CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_LOTS, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_LOTS, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK,   CmOk),
  EV_COMMAND(ID_NEW, CmNewLot),
END_RESPONSE_TABLE;

LotsManagementDialog::LotsManagementDialog(TWindow* pere, NSContexte* pCtx)
                     :TDialog(pere, "IDD_LOT_MGMT", pNSResModule), NSRoot(pCtx)
{
try
{
	_pMatBox 	        = new NSMngtListeLotsWin(this, IDC_CIM10BOX) ;
  _bNaturallySorted = true ;
	_iSortedColumn    = 0 ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
LotsManagementDialog::~LotsManagementDialog()
{
	delete _pMatBox ;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des codes CCAM
//---------------------------------------------------------------------------
void
LotsManagementDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitLotArray()) || (_aMatArray.empty()))
		return ;

	AfficheListe() ;

  _pMatBox->SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, LVIS_SELECTED) ;
}
catch (...)
{
	erreur("Exception LotsManagementDialog::SetupWindow.", standardError, 0) ;
}
}

void
LotsManagementDialog::EvSize(uint sizeType, ClassLib::TSize& size)
{
	TDialog::EvSize(sizeType, size) ;

	if (sizeType != SIZE_MINIMIZED)
	{
  	int LEFT_OFFSET  = 8 ;
    int TOP_OFFSET   = 8 ;
    int WIDTH_OFFSET = 24 ;

		ClassLib::TRect rect      = GetWindowRect() ;
    ClassLib::TRect list_rect = _pMatBox->GetWindowRect() ;
		_pMatBox->SetWindowPos(0, LEFT_OFFSET, TOP_OFFSET,
                              rect.Width() - WIDTH_OFFSET, list_rect.Height(),
                              SWP_NOZORDER ) ;
	}
}

bool
LotsManagementDialog::InitLotArray()
{
try
{
	_aMatArray.vider() ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSLot Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes LOTS.", standardError, Base.lastError) ;
		return false ;
	}
	//
	// Recherche des codes correspondant au module
	//
	Base.lastError = Base.debut(dbiWRITELOCK) ;

  // si la table est vide debut() renvoie DBIERR_EOF
  if (DBIERR_EOF == Base.lastError)
  {
    erreur("La table des lots est vide.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
  }

  if (DBIERR_NONE != Base.lastError)
	{
		erreur("La table des lots est endommagée.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	Base.lastError = Base.getRecord();
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à la lecture du 1er lot.", standardError, Base.lastError) ;
		Base.close() ;
		return false ;
	}

	string sLibelleAffiche = string("") ;

	while (DBIERR_NONE == Base.lastError)
	{
		// pMatBox->AddString(sLibelleAffiche.c_str());
		_aMatArray.push_back(new NSLotInfo(&Base, pContexte)) ;

		//
		// Prise du code suivant
		//
		Base.lastError = Base.suivant(dbiWRITELOCK) ;
		if ((DBIERR_NONE != Base.lastError) && (DBIERR_EOF != Base.lastError))
			erreur("Erreur à l'accès au lot suivant.", standardError, Base.lastError) ;
		else if (DBIERR_NONE == Base.lastError)
		{
			Base.lastError = Base.getRecord() ;
			if (DBIERR_NONE != Base.lastError)
				erreur("Erreur à la lecture du lot.", standardError, Base.lastError) ;
		}
	}
	Base.close() ;

	return true ;
}
catch (...)
{
	erreur("Exception ChoixLotsDialog::InitGlobalLotArray.", standardError, 0) ;
  return false ;
}
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
LotsManagementDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
  int iCodeChoisi = _pMatBox->IndexItemSelect() ;
  if (iCodeChoisi < 0)
    return ;

  NSLotInfo *pSelectedLot = _aMatArray[iCodeChoisi] ;

  EditLotDialog lotDlg(this, pContexte, pSelectedLot) ;
  if ((lotDlg.Execute()) != IDOK)
   	return ;

  updateLot(pSelectedLot) ;

  AfficheListe() ;
}

void
LotsManagementDialog::CmOk()
{
	TDialog::CmOk() ;
}

void
LotsManagementDialog::CmNewLot()
{
  NSLotInfo newLotInfo(pContexte) ;

  newLotInfo.setStatusType(NSLotData::statusStock) ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;
  newLotInfo.setDateAchat(tNow.donneDate()) ;

  EditLotDialog lotDlg(this, pContexte, &newLotInfo) ;
  if ((lotDlg.Execute()) != IDOK)
   	return ;

  NSLot baseLots(pContexte->getSuperviseur()) ;

  string sNumLot ;
  if (false == baseLots.ChercheNumLot(sNumLot))
    return ;

  newLotInfo.setCode(sNumLot) ;

  updateLot(&newLotInfo) ;

  _aMatArray.push_back(new NSLotInfo(newLotInfo)) ;
  AfficheListe() ;
}

void
LotsManagementDialog::CmNewLotFrom()
{
  int iSel = _pMatBox->IndexItemSelect() ;
  if (iSel < 0)
    return ;

  NSLotInfo newLotInfo(*(_aMatArray[iSel])) ;
  
  newLotInfo.setStatusType(NSLotData::statusStock) ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;
  newLotInfo.setDateAchat(tNow.donneDate()) ;

  EditLotDialog lotDlg(this, pContexte, &newLotInfo) ;
  if ((lotDlg.Execute()) != IDOK)
   	return ;

  NSLot baseLots(pContexte->getSuperviseur()) ;

  string sNumLot ;
  if (false == baseLots.ChercheNumLot(sNumLot))
    return ;

  newLotInfo.setCode(sNumLot) ;

  updateLot(&newLotInfo) ;

  _aMatArray.push_back(new NSLotInfo(newLotInfo)) ;
  AfficheListe() ;
}

bool
LotsManagementDialog::updateLot(NSLotInfo* pLotInfo)
{
  if ((NSLotInfo*) NULL == pLotInfo)
    return false ;

  if (string("") == pLotInfo->getCode())
    return false ;

  NSLot baseLots(pContexte->getSuperviseur()) ;

  baseLots.lastError = baseLots.open() ;
	if (DBIERR_NONE != baseLots.lastError)
	{
		erreur("Erreur à l'ouverture de la base Lots.", standardError, baseLots.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		return false ;
	}

  string sNum = pLotInfo->getCode() ;

	baseLots.lastError = baseLots.chercheClef(&sNum,
                                            "",
                                            0,
                                            keySEARCHEQ,
                                            dbiWRITELOCK) ;

	if (DBIERR_NONE != baseLots.lastError)
	{
		erreur("Erreur à la recherche de la nouvelle fiche Lots.", standardError, baseLots.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		baseLots.close() ;
		return false ;
	}

	baseLots.lastError = baseLots.getRecord() ;
	if (DBIERR_NONE != baseLots.lastError)
	{
		erreur("Erreur à lecture du fichier Lots.db", standardError, baseLots.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		baseLots.close() ;
		return false ;
	}

	baseLots._Donnees = pLotInfo->_Donnees ;

	baseLots.lastError = baseLots.modifyRecord(TRUE) ;
	if (DBIERR_NONE != baseLots.lastError)
	{
		erreur("Erreur à la modification de la fiche Lots.", standardError, baseLots.lastError, pContexte->GetMainWindow()->GetHandle()) ;
		baseLots.close() ;
		return false ;
	}

	baseLots.lastError = baseLots.close() ;
	if (DBIERR_NONE != baseLots.lastError)
		erreur("Erreur à la fermeture de la base Lots.", standardError, baseLots.lastError, pContexte->GetMainWindow()->GetHandle()) ;

	return true ;
}

void
LotsManagementDialog::InitListe()
{
	TListWindColumn colLibelle("Libelle", 200, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colLibelle) ;
	TListWindColumn colStatus("S", 20, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colStatus) ;
  TListWindColumn colLexique("Type", 100, TListWindColumn::Left, 2) ;
	_pMatBox->InsertColumn(2, colLexique) ;
  TListWindColumn colExam("Examen", 100, TListWindColumn::Left, 3) ;
	_pMatBox->InsertColumn(3, colExam) ;
	TListWindColumn colPrixVenteE("Prix vente €", 60, TListWindColumn::Right, 4) ;
	_pMatBox->InsertColumn(4, colPrixVenteE) ;
  string sSigle = string("Prix vente ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixVenteF((char*) sSigle.c_str(), 60, TListWindColumn::Right, 5) ;
	_pMatBox->InsertColumn(5, colPrixVenteF) ;
  TListWindColumn colPrixAchatE("Prix achat €", 60, TListWindColumn::Right, 6) ;
	_pMatBox->InsertColumn(6, colPrixAchatE) ;
  sSigle = string("Prix achat ") + pContexte->getSuperviseur()->getSigle() ;
  TListWindColumn colPrixAchatF((char*) sSigle.c_str(), 60, TListWindColumn::Right, 7) ;
	_pMatBox->InsertColumn(7, colPrixAchatF) ;
  TListWindColumn colDateAchat("Date achat", 80, TListWindColumn::Left, 8) ;
	_pMatBox->InsertColumn(8, colDateAchat) ;
	TListWindColumn colQte("Qté", 50, TListWindColumn::Right, 9) ;
	_pMatBox->InsertColumn(9, colQte) ;
  TListWindColumn colReste("Reste", 50, TListWindColumn::Right, 10) ;
	_pMatBox->InsertColumn(10, colReste) ;
  TListWindColumn colAlerte("Alerte", 50, TListWindColumn::Right, 11) ;
	_pMatBox->InsertColumn(11, colAlerte) ;
}

void
LotsManagementDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aMatArray.empty())
    return ;

	for (LotsReverseIter i = _aMatArray.rbegin() ; _aMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getLibelle() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
LotsManagementDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aMatArray.size())
  	return ;

  string sBuffer = string("") ;

  NSLotInfo* pLotInfo = _aMatArray[index] ;
  if ((NSLotInfo*) NULL == pLotInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Status

    	sBuffer = pLotInfo->getStatus() ;
      break ;

    case 2 : // Type
    {
      string sType = pLotInfo->getLexique() ;
      if (string("") != sType)
        pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sType, &sBuffer) ;
      break ;
    }

    case 3 : // Examen
    {
      string sType = pLotInfo->getModule() ;
      if (string("") != sType)
        pContexte->getDico()->donneLibelle(pContexte->getUserLanguage(), &sType, &sBuffer) ;
      break ;
    }

		case 4 : // Prix vente Euros

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteE()) ;
      break ;

    case 5 : // Prix vente seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixVenteF()) ;
      break ;

    case 6 : // Prix achat Euros

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixAchatE()) ;
      break ;

    case 7 : // Prix achat seconde monnaie

    	sBuffer = NSBlocFse16::GetSumInUnits(pLotInfo->getPrixAchatF()) ;
      break ;

    case 8 : // Date d'achat

    	sBuffer = getFormatedDate(pLotInfo->getDateAchat(), pContexte->getUserLanguage(), string("")) ;
      break ;

    case 9 : // Qté initiale

    	sBuffer = pLotInfo->getQuantite() ;
      break ;

    case 10 : // Qté reste

    	sBuffer = pLotInfo->getReste() ;
      break ;

    case 11 : // Qté reste

    	sBuffer = pLotInfo->getAlerte() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
LotsManagementDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch (lwn.iSubItem)
  {
    case 0  : sortByLibelle() ; break ;
    case 1  : sortByStatus() ;  break ;
    case 2  : sortByLexique() ; break ;
    case 3  : sortByExam() ;    break ;
    case 8  : sortByDate() ;    break ;
  }
}

void
LotsManagementDialog::swapSortOrder(int iColumn)
{
  if (_iSortedColumn == iColumn)
  {
    if (_bNaturallySorted)
      _bNaturallySorted = false ;
    else
      _bNaturallySorted = true ;
  }
  else
  {
    _iSortedColumn    = iColumn ;
    _bNaturallySorted = true ;
  }
}

void
LotsManagementDialog::sortByLibelle()
{
	swapSortOrder(0) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLibelleInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLibelleSup) ;

  AfficheListe() ;
}

void
LotsManagementDialog::sortByStatus()
{
	swapSortOrder(1) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByStatusInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByStatusSup) ;

  AfficheListe() ;
}

void
LotsManagementDialog::sortByLexique()
{
	swapSortOrder(2) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLexiqueInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByLexiqueSup) ;

  AfficheListe() ;
}

void
LotsManagementDialog::sortByExam()
{
	swapSortOrder(3) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByExamInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByExamSup) ;

  AfficheListe() ;
}

void
LotsManagementDialog::sortByDate()
{
	swapSortOrder(8) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByDateInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), LotSortByDateSup) ;

  AfficheListe() ;
}

#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSMngtListeLotsWin, TListWindow)
	EV_WM_LBUTTONDBLCLK,
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(CM_EDIT_ACT,     CmEditLot),
  EV_COMMAND(CM_NEW_ACT,      CmNewLot),
  EV_COMMAND(CM_NEW_ACT_FROM, CmNewLotFrom),
END_RESPONSE_TABLE;

void
NSMngtListeLotsWin::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSMngtListeLotsWin::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

	HitTest(info) ;

  if (info.GetFlags() & LVHT_ONITEM)
  	pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

void
NSMngtListeLotsWin::EvRButtonDown(uint modkeys, NS_CLASSLIB::TPoint& point)
{
	TLwHitTestInfo info(point) ;

  // création d'un menu popup
  NS_CLASSLIB::TPoint lp = point ;

  OWL::TPopupMenu menu ;

  NSSuper* pSuper = pDlg->pContexte->getSuperviseur() ;

  string sNew = pSuper->getText("accountingLocalLots", "newLocalLot") ;
  menu.AppendMenu(MF_STRING, CM_NEW_ACT, sNew.c_str()) ;

  int indexItem = HitTest(info) ;
  if (info.GetFlags() & LVHT_ONITEM)  {  	SetItemState(indexItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED) ;    Invalidate() ;		string sEdit    = pSuper->getText("accountingLocalLots", "editThisLocalLot") ;		string sNewFrom = pSuper->getText("accountingLocalLots", "newLocalLotFromThisOne") ;

    menu.AppendMenu(MF_STRING, CM_NEW_ACT_FROM, sNewFrom.c_str()) ;
    menu.AppendMenu(MF_SEPARATOR, 0, 0) ;
    menu.AppendMenu(MF_STRING, CM_EDIT_ACT, sEdit.c_str()) ;
  }

  ClientToScreen(lp) ;
  menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, lp, 0, HWindow) ;
  TListWindow::EvRButtonDown(modkeys, point) ;
}

void
NSMngtListeLotsWin::CmNewLot()
{
  pDlg->CmNewLot() ;
}

void
NSMngtListeLotsWin::CmNewLotFrom()
{
  pDlg->CmNewLotFrom() ;
}

void
NSMngtListeLotsWin::CmEditLot()
{
  pDlg->CmMaterielDblClk((WPARAM) 0) ;
}

//---------------------------------------------------------------------------
//  Description: Retourne l'index du premier item sélectionné
//  Returns:     index si item est sélectionné, -1 sinon
//---------------------------------------------------------------------------
int
NSMngtListeLotsWin::IndexItemSelect()
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
#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------
//
//  Méthodes de EditLotDialog
//
// -----------------------------------------------------------------

// ---------------------------------------------------------------------------

#ifndef _ENTERPRISE_DLL
DEFINE_RESPONSE_TABLE1(EditLotDialog, NSUtilDialog)	EV_COMMAND(IDOK, CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

EditLotDialog::EditLotDialog(TWindow* pere, NSContexte* pCtx, NSLotInfo* pLotInfo)
              :NSUtilDialog(pere, pCtx, "IDD_LOT_EDIT", pNSResModule)
{
	// Initialisation des donnees

  _pLotInfo = pLotInfo ;	// Création de tous les "objets de contrôle"
  _pLabel         = new NSUtilEdit2(pContexte, this, IDC_LOT_LABEL, LOT_LIBELLE_LEN) ;
  _pLexique       = new NSUtilLexique(pContexte, this, IDC_LOT_LEXIQUE, pCtx->getDico()) ;
  _pExam          = new NSUtilLexique(pContexte, this, IDC_LOT_EXAM, pCtx->getDico()) ;
  _pStatusGrpe	  = new TGroupBox(this, IDC_LOT_STATUS) ;
	_pStatusNew     = new TRadioButton(this, IDC_LOT_NEW,   _pStatusGrpe) ;
	_pStatusOpen    = new TRadioButton(this, IDC_LOT_OPEN,  _pStatusGrpe) ;
  _pStatusClose   = new TRadioButton(this, IDC_LOT_CLOSE, _pStatusGrpe) ;
	_pDateAchat	    = new NSUtilEditDate(pContexte, this, IDC_LOT_DATE) ;  _pMontantAchatE = new NSUtilEditSomme(pContexte, this, IDC_LOT_ACHAT_E) ;  _pMontantAchatF = new NSUtilEditSomme(pContexte, this, IDC_LOT_ACHAT_F) ;
  _pMontantVenteE = new NSUtilEditSomme(pContexte, this, IDC_LOT_VENTE_E) ;
  _pMontantVenteF = new NSUtilEditSomme(pContexte, this, IDC_LOT_VENTE_F) ;
  _pQuantity      = new NSUtilEdit2(pContexte, this, IDC_LOT_QTE,     LOT_COUNT_LEN) ;  _pRemains       = new NSUtilEdit2(pContexte, this, IDC_LOT_REMAINS, LOT_COUNT_LEN) ;  _pAlert         = new NSUtilEdit2(pContexte, this, IDC_LOT_ALERT,   LOT_COUNT_LEN) ;  _pSigleAchat		= new TStatic(this, IDC_LOT_SIGLE_A) ;  _pSigleVente		= new TStatic(this, IDC_LOT_SIGLE_V) ;  _pCodeBarre     = new NSUtilEdit2(pContexte, this, IDC_LOT_BARRE, LOT_BARRE_LEN) ;}
EditLotDialog::~EditLotDialog(){
  delete _pLabel ;
  delete _pLexique ;
  delete _pExam ;
  delete _pStatusGrpe	;
	delete _pStatusNew ;
	delete _pStatusOpen ;
  delete _pStatusClose ;
	delete _pDateAchat ;
  delete _pMontantAchatE ;  delete _pMontantAchatF ;
  delete _pMontantVenteE ;
  delete _pMontantVenteF ;
  delete _pQuantity ;  delete _pRemains ;  delete _pAlert ;
  delete _pSigleAchat	;
  delete _pSigleVente	;
  delete _pCodeBarre ;
}
void
EditLotDialog::SetupWindow()
{
	NSUtilDialog::SetupWindow() ;  _pLabel->SetText(_pLotInfo->getLibelle()) ;  _pLexique->setLabel(_pLotInfo->getLexique()) ;  _pExam->setLabel(_pLotInfo->getModule()) ;
  NSLotData::STATUSLOT iStatus = _pLotInfo->getStatusType() ;

  _pStatusNew->Uncheck() ;
  _pStatusOpen->Uncheck() ;
  _pStatusClose->Uncheck() ;

  switch(iStatus)
  {
    case NSLotData::statusStock : _pStatusNew->Check() ;   break ;
    case NSLotData::statusOpen  : _pStatusOpen->Check() ;  break ;
    case NSLotData::statusEmpty : _pStatusClose->Check() ; break ;
  }

  string sSigle = pContexte->getSuperviseur()->getSigle() ;
	_pSigleAchat->SetText(sSigle.c_str()) ;
  _pSigleVente->SetText(sSigle.c_str()) ;

  _pDateAchat->setDate(_pLotInfo->getDateAchat()) ;
  _pMontantAchatE->setSomme(_pLotInfo->getPrixAchatE()) ;
  _pMontantAchatF->setSomme(_pLotInfo->getPrixAchatF()) ;
  _pMontantVenteE->setSomme(_pLotInfo->getPrixVenteE()) ;
  _pMontantVenteF->setSomme(_pLotInfo->getPrixVenteF()) ;

  _pQuantity->SetText(_pLotInfo->getQuantite().c_str()) ;
  _pRemains->SetText(_pLotInfo->getReste().c_str()) ;
  _pAlert->SetText(_pLotInfo->getAlerte().c_str()) ;

  _pCodeBarre->SetText(_pLotInfo->getBarre()) ;
}
voidEditLotDialog::CmOk(){  string sLabel ;  _pLabel->GetText(sLabel) ;	_pLotInfo->setLibelle(sLabel) ;
  if      (_pStatusNew->GetCheck() == BF_CHECKED)
    _pLotInfo->setStatusType(NSLotData::statusStock) ;
  else if (_pStatusOpen->GetCheck() == BF_CHECKED)
    _pLotInfo->setStatusType(NSLotData::statusOpen) ;
  else if (_pStatusClose->GetCheck() == BF_CHECKED)
    _pLotInfo->setStatusType(NSLotData::statusEmpty) ;

  _pLotInfo->setLexique(_pLexique->getCodeSens()) ;
  _pLotInfo->setModule(_pExam->getCodeSens()) ;

  string sDate ;
	_pDateAchat->getDate(&sDate) ;	_pLotInfo->setDateAchat(sDate) ;

  string sTexte ;
	_pMontantAchatE->getSomme(&sTexte) ;	_pLotInfo->setPrixAchatE(sTexte) ;
  _pMontantAchatF->getSomme(&sTexte) ;
	_pLotInfo->setPrixAchatF(sTexte) ;

  _pMontantVenteE->getSomme(&sTexte) ;
	_pLotInfo->setPrixVenteE(sTexte) ;
  _pMontantVenteF->getSomme(&sTexte) ;
	_pLotInfo->setPrixVenteF(sTexte) ;

  _pQuantity->GetText(sTexte) ;
	_pLotInfo->setQuantite(sTexte) ;
	_pRemains->GetText(sTexte) ;
	_pLotInfo->setReste(sTexte) ;
  _pAlert->GetText(sTexte) ;
	_pLotInfo->setAlerte(sTexte) ;

  _pCodeBarre->GetText(sTexte) ;
  _pLotInfo->setBarre(sTexte) ;

	NSUtilDialog::CmOk() ;}
voidEditLotDialog::CmCancel(){	NSUtilDialog::CmCancel() ;}#endif // #ifndef _ENTERPRISE_DLL

