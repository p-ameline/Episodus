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
  #include <owl\applicat.h>
  #include <owl\decmdifr.h>
  #include <owl\docmanag.h>
  #include <owl\olemdifr.h>

  #include <owl\applicat.h>
  #include <owl\window.h>

  #include "partage\nsdivfct.h"
  #include "nautilus\nssuper.h"
  #include "partage\nsglobal.h"
  #include "dcodeur\nsdecode.h"

  #include "partage\nscim10.rh"
  #include "partage\nsmatfic.rh"
#endif

#include "nsbb\nsarc.h"#include "partage\nscim10.h"
#include "nssavoir\nsfilgd.h"

// **************************************************************************
//  Méthodes de NSCim10Data
// **************************************************************************

//---------------------------------------------------------------------------
//  Met à vide les variables de la fiche
//---------------------------------------------------------------------------
void
NSCim10Data::metAZero()
{
	//
  // Met les champs de données à zéro
  //
  memset(code,    0, CIM_CODE_LEN + 1) ;
  memset(libelle, 0, CIM_LIBELLE_LEN + 1) ;
  memset(module,  0, CIM_MODULE_LEN + 1) ;
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCim10Data::NSCim10Data(NSCim10Data& rv)
{
	strcpy(code,    rv.code) ;
	strcpy(libelle, rv.libelle) ;
	strcpy(module,  rv.module) ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCim10Data&
NSCim10Data::operator=(const NSCim10Data& src)
{
  if (this == &src)
  	return *this ;

	strcpy(code,    src.code) ;
	strcpy(libelle, src.libelle) ;
  strcpy(module,  src.module) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCim10Data::operator == (const NSCim10Data& o)
{
	if ((strcmp(module,	 o.module)  == 0) &&
		  (strcmp(code, 	 o.code)	  == 0) &&
		  (strcmp(libelle, o.libelle) == 0))
		return 1;
	else
		return 0;
}

// **************************************************************************
//  Méthodes de NSCim10
// **************************************************************************

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCim10::NSCim10(NSSuper* pSuper) : NSFiche(pSuper)
{
	//
  // Crée un objet de données
  //
  pDonnees = new NSCim10Data() ;
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCim10::~NSCim10()
{
	delete pDonnees ;
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSCim10::alimenteFiche()
{
	alimenteChamp(pDonnees->code, 	 CIM_CODE_FIELD, 	CIM_CODE_LEN) ;
  alimenteChamp(pDonnees->libelle, CIM_LIBELLE_FIELD, CIM_LIBELLE_LEN) ;
	alimenteChamp(pDonnees->module,  CIM_MODULE_FIELD,  CIM_MODULE_LEN) ;
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCim10::videFiche()
{
	videChamp(pDonnees->code, 	 CIM_CODE_FIELD, 	CIM_CODE_LEN) ;
  videChamp(pDonnees->libelle, CIM_LIBELLE_FIELD, CIM_LIBELLE_LEN) ;
  videChamp(pDonnees->module,  CIM_MODULE_FIELD,  CIM_MODULE_LEN) ;
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSCim10::open()
{
	char tableName[] = "CIM10.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_PARAMS) ;
  return(lastError) ;
}
#endif // #ifndef _ENTERPRISE_DLL

// **************************************************************************
//  Méthodes de NSCim10Info
// **************************************************************************

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSCim10Info::NSCim10Info()
{
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSCim10
//---------------------------------------------------------------------------
NSCim10Info::NSCim10Info(NSCim10* pCim)
{
	//
	// Copie les valeurs du NSMateriel
	//
	Donnees = *(pCim->pDonnees) ;
}
#endif // #ifndef _ENTERPRISE_DLL

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCim10Info::NSCim10Info(const NSCim10Info& rv)
{
	//
	// Copie les valeurs du NSMaterielInfo d'origine
	//
	Donnees = rv.Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCim10Info&
NSCim10Info::operator=(const NSCim10Info& src)
{
  if (this == &src)
  	return *this ;

	Donnees = src.Donnees ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCim10Info::operator==(const NSCim10Info& o)
{
	return (Donnees == o.Donnees) ;
}

#ifndef _ENTERPRISE_DLL
// **************************************************************************//  Méthodes de ChoixCIM10Dialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(ChoixCIM10Dialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_SELCHANGE, CmSelectMateriel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_DBLCLK, 	CmMaterielDblClk),
END_RESPONSE_TABLE;

ChoixCIM10Dialog::ChoixCIM10Dialog(TWindow* pere, NSContexte* pCtx,
                                   string* pModu, NSCim10Info* pCodeChoix)
                 :TDialog(pere, "IDD_CIM10", pNSDLLModule), NSRoot(pCtx)
{
try
{
	pMatBox 	= new TListBox(this, IDC_CIM10BOX);
  CodeChoisi  = 0;
	pCodeChoisi = pCodeChoix;

	pModule		= pModu;
}
catch (...)
{
	erreur("Exception ChoixCIM10Dialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Destructeur.
//---------------------------------------------------------------------------
ChoixCIM10Dialog::~ChoixCIM10Dialog()
{
	//
	// Si un matériel a été choisi
	//
	if ((CodeChoisi > 0) && pCodeChoisi)
		*pCodeChoisi = *((*pMatArray)[CodeChoisi - 1]);

	delete pMatBox;
	delete pMatArray;
}

//---------------------------------------------------------------------------//  Initialise la boite de liste des matériels
//---------------------------------------------------------------------------
void
ChoixCIM10Dialog::SetupWindow()
{
try
{
	TDialog::SetupWindow();
	//
	// Remplissage de MaterielBox avec le matériel idoine
	//
	pMatArray = new NSCim10Array;
	NSCim10* pBase = new NSCim10(pContexte->getSuperviseur());
	//
	// Ouverture du fichier
	//
	pBase->lastError = pBase->open();
	if (pBase->lastError != DBIERR_NONE)
	{
		erreur("Erreur à l'ouverture du fichier des codes CIM10.", standardError, pBase->lastError);
		delete pBase;
		return;
	}
	//
	// Recherche des codes correspondant au module
	//
    pBase->lastError = pBase->chercheClef(pModule,
                                          "MODULE_INDEX",
                                          NODEFAULTINDEX,
                                          keySEARCHEQ,
                                          dbiWRITELOCK);
	if (pBase->lastError != DBIERR_NONE)
	{
		erreur("Il n'existe pas de code pour cet examen.", standardError, pBase->lastError);
		pBase->close();
		delete pBase;
		return;
	}
	pBase->lastError = pBase->getRecord();
	if (pBase->lastError != DBIERR_NONE)
	{
		erreur("Erreur à la lecture du 1er code.", standardError, pBase->lastError);
		pBase->close();
		delete pBase;
		return;
	}

    string sLibelleAffiche = "";

	while ((pBase->lastError == DBIERR_NONE) &&
			 (string(pBase->pDonnees->module) == *pModule))
	{
		//
		// Ajout du matériel à la MatBox
		//
        sLibelleAffiche = pBase->pDonnees->code;
        if (pBase->pDonnees->libelle[0] != '\0')
            sLibelleAffiche += string(" : ") + pBase->pDonnees->libelle;

		pMatBox->AddString(sLibelleAffiche.c_str());
		pMatArray->push_back(new NSCim10Info(pBase));
		//
		// Prise du code suivant
		//
		pBase->lastError = pBase->suivant(dbiWRITELOCK);
		if ((pBase->lastError != DBIERR_NONE) &&
			 (pBase->lastError != DBIERR_EOF))
			erreur("Erreur à l'accès au code CDAM suivant.", standardError, pBase->lastError);
		else if(pBase->lastError == DBIERR_NONE)
		{
			pBase->lastError = pBase->getRecord();
			if (pBase->lastError != DBIERR_NONE)
				erreur("Erreur à la lecture du fichier CDAM.", standardError, pBase->lastError);

		}
	}
	pBase->close();
	delete pBase;
	//
	// Si un code avait déjà été sélectionné, on le coche
	//
    if ((pCodeChoisi->Donnees.code[0] != '\0') && (false == pMatArray->empty()))
	{
        int iNumApp = 0;
        CimIter i = pMatArray->begin();
   	    for ( ; (i != pMatArray->end()) &&
      		(strcmp((*i)->Donnees.code, pCodeChoisi->Donnees.code) != 0);
       		i++, iNumApp++);
        if (i != pMatArray->end())
      	    pMatBox->SetSelIndex(iNumApp);
	}
}
catch (...)
{
    erreur("Exception ChoixCIM10Dialog::SetupWindow.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Réponse à selChanged
//  Prend acte de la sélection du matériel
//---------------------------------------------------------------------------
void
ChoixCIM10Dialog::CmSelectMateriel(WPARAM /* Cmd */)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	CodeChoisi = pMatBox->GetSelIndex() + 1;
}

//---------------------------------------------------------------------------
//  Réponse au double click
//  Sélectionne un matériel et effectue CmOk
//---------------------------------------------------------------------------
void
ChoixCIM10Dialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
	CodeChoisi = pMatBox->GetSelIndex() + 1;
	TDialog::CmOk();
}

//---------------------------------------------------------------------------
//  Bouton Cancel
//  Annule MaterielChoisi et appelle Cancel()
//---------------------------------------------------------------------------
void
ChoixCIM10Dialog::CmCancel()
{
	CodeChoisi = 0;
	TDialog::CmCancel();
}
#endif // #ifndef _ENTERPRISE_DLL

// **************************************************************************
//  Méthodes de NSCcamData
// **************************************************************************

//---------------------------------------------------------------------------
//  Met à vide les variables de la fiche
//---------------------------------------------------------------------------
void
NSCcamData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code,    0, CCAM_CODE_LEN + 1);
	memset(libelle, 0, CCAM_LIBELLE_LEN + 1);
	memset(module,  0, CCAM_MODULE_LEN + 1);
	memset(prix,    0, CCAM_PRIX_LEN + 1);
	memset(frequence, 0, CCAM_FREQ_LEN + 1);
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCcamData::NSCcamData(const NSCcamData& rv)
{
	strcpy(code,      rv.code) ;
	strcpy(libelle,   rv.libelle) ;
	strcpy(module,    rv.module) ;
	strcpy(prix,      rv.prix) ;
	strcpy(frequence, rv.frequence) ;
}

//---------------------------------------------------------------------------
//  Opérateur =
//---------------------------------------------------------------------------
NSCcamData&
NSCcamData::operator=(const NSCcamData& src)
{
  if (this == &src)
  	return *this ;

	strcpy(code,      src.code) ;
	strcpy(libelle,   src.libelle) ;
	strcpy(module,    src.module) ;
	strcpy(prix,      src.prix) ;
	strcpy(frequence, src.frequence) ;

	return *this ;
}

//---------------------------------------------------------------------------//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSCcamData::operator == (const NSCcamData& o)
{
	if ((strcmp(module,	o.module)  == 0) &&
		 (strcmp(code, 	o.code)	  == 0) &&
		 (strcmp(libelle, o.libelle) == 0) &&
         (strcmp(prix, o.prix) == 0) &&
         (strcmp(frequence, o.frequence) == 0)
         )
		return 1;
	else
		return 0;
}

// **************************************************************************
//  Méthodes de NSCim10
// **************************************************************************

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur
//---------------------------------------------------------------------------
NSCcam::NSCcam(NSSuper* pSuper)
       :NSFiche(pSuper)
{
}

//---------------------------------------------------------------------------
//  Destructeur
//---------------------------------------------------------------------------
NSCcam::~NSCcam()
{
}

//---------------------------------------------------------------------------
//  Transfère le contenu de pRecBuff dans les variables de la fiche
//---------------------------------------------------------------------------
void
NSCcam::alimenteFiche()
{
	alimenteChamp(_Donnees.code,      CCAM_CODE_FIELD,    CCAM_CODE_LEN) ;
	alimenteChamp(_Donnees.libelle,   CCAM_LIBELLE_FIELD, CCAM_LIBELLE_LEN) ;
	alimenteChamp(_Donnees.module,    CCAM_MODULE_FIELD,  CCAM_MODULE_LEN) ;
	alimenteChamp(_Donnees.prix,      CCAM_PRIX_FIELD,    CCAM_PRIX_LEN) ;
	alimenteChamp(_Donnees.frequence, CCAM_FREQ_FIELD,    CCAM_FREQ_LEN) ;
}

//---------------------------------------------------------------------------
//  Transfère le contenu des valeurs de la fiche dans pRecBuff
//---------------------------------------------------------------------------
void
NSCcam::videFiche()
{
	videChamp(_Donnees.code,      CCAM_CODE_FIELD,    CCAM_CODE_LEN) ;
	videChamp(_Donnees.libelle,   CCAM_LIBELLE_FIELD, CCAM_LIBELLE_LEN) ;
	videChamp(_Donnees.module,    CCAM_MODULE_FIELD,  CCAM_MODULE_LEN) ;
	videChamp(_Donnees.prix,      CCAM_PRIX_FIELD,    CCAM_PRIX_LEN) ;
	videChamp(_Donnees.frequence, CCAM_FREQ_FIELD,    CCAM_FREQ_LEN) ;
}

//---------------------------------------------------------------------------
//  Ouvre la table primaire et les index secondaires
//---------------------------------------------------------------------------
DBIResult
NSCcam::open()
{
	char tableName[] = "CCAM.DB" ;
  //
  // Appelle la fonction open() de la classe de base pour ouvrir
  // l'index primaire
  //
  lastError = NSFiche::open(tableName, NSF_PARTAGE_PARAMS) ;
  return(lastError) ;
}

DBIResult
NSCcam::getRecordByCode(string sCode, NSCcamInfo* pResult, bool bOpenClose, bool bVerbose)
{
	if (((NSCcamInfo*) NULL == pResult) || (string("") == sCode))
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
				erreur("Erreur à l'ouverture du fichier CCAM.", standardError, lastError) ;
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
    	string sError = string("Erreur à la recherche du code CCAM : ") + sCode ;
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
    	string sError = string("Erreur à la lecture du code CCAM : ") + sCode ;
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
NSCcamInfo::NSCcamInfo()
{
}

#ifndef _ENTERPRISE_DLL
//---------------------------------------------------------------------------
//  Constructeur à partir d'un NSCcam
//---------------------------------------------------------------------------
NSCcamInfo::NSCcamInfo(const NSCcam* pCcam)
{
  if (pCcam)
	  _Donnees = pCcam->_Donnees ;
}
#endif // #ifndef _ENTERPRISE_DLL

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSCcamInfo::NSCcamInfo(const NSCcamInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSCcamInfo&
NSCcamInfo::operator=(const NSCcamInfo& src)
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
NSCcamInfo::operator==(const NSCcamInfo& o)
{
	return (_Donnees == o._Donnees) ;
}

bool
CcamSortByCodeInf(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getCode() < pCCAM2->getCode()) ;
}


bool
CcamSortByCodeSup(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getCode() > pCCAM2->getCode()) ;
}

bool
CcamSortByLibelleInf(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getLibelle() < pCCAM2->getLibelle()) ;
}


bool
CcamSortByLibelleSup(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getLibelle() > pCCAM2->getLibelle()) ;
}

bool
CcamSortByPrixInf(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getPrice() > pCCAM2->getPrice()) ;
}


bool
CcamSortByPrixSup(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
  return (pCCAM1->getPrice() < pCCAM2->getPrice()) ;
}

bool
CcamSortByFreqInf(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getFrequency() > pCCAM2->getFrequency()) ;
}


bool
CcamSortByFreqSup(NSCcamInfo *pCCAM1, NSCcamInfo *pCCAM2)
{
	return (pCCAM1->getFrequency() < pCCAM2->getFrequency()) ;
}


#ifndef _ENTERPRISE_DLL
// **************************************************************************//  Méthodes de ChoixCcamDialog
// **************************************************************************
DEFINE_RESPONSE_TABLE1(ChoixCcamDialog, TDialog)
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_SELCHANGE, CmSelectMateriel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MATERIELBOX, LBN_DBLCLK, 	CmMaterielDblClk),
  EV_LVN_GETDISPINFO(IDC_CIM10BOX, LvnGetDispInfo),
  EV_LVN_COLUMNCLICK(IDC_CIM10BOX, LVNColumnclick),
  EV_WM_SIZE,
  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(ID_ELARGIR, CmElargir),
END_RESPONSE_TABLE;

ChoixCcamDialog::ChoixCcamDialog(TWindow* pere, NSContexte* pCtx,
                                   string* pModu, NSCcamInfo* pCodeChoix)
                :TDialog(pere, "IDD_CCAM", pNSDLLModule), NSRoot(pCtx)
{
try
{
  _pChoiceButton = new OWL::TButton(this, ID_ELARGIR) ;
	_pMatBox 	     = new NSListeCCAMWindow(this, IDC_CIM10BOX) ;
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
ChoixCcamDialog::~ChoixCcamDialog()
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
ChoixCcamDialog::SetupWindow()
{
try
{
	TDialog::SetupWindow() ;

  InitListe() ;

  if ((false == InitCCAMArray()) || (_aMatArray.empty()))
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
	CcamIter i = _aMatArray.begin() ;
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
ChoixCcamDialog::EvSize(uint sizeType, ClassLib::TSize& size)
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
ChoixCcamDialog::InitCCAMArray()
{
  if (_bGlobalList)
    return InitGlobalCCAMArray() ;

  return InitCCAMArrayForCode() ;
}

bool
ChoixCcamDialog::InitCCAMArrayForCode()
{
try
{
	_aMatArray.vider() ;

  bool bExistFrequ = false ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSCcam Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes CCAM.", standardError, Base.lastError) ;
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
		_aMatArray.push_back(new NSCcamInfo(&Base)) ;

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
	erreur("Exception ChoixCcamDialog::InitCCAMArray.", standardError, 0) ;
  return false ;
}
}

bool
ChoixCcamDialog::InitGlobalCCAMArray()
{
try
{
	_aMatArray.vider() ;

  bool bExistFrequ = false ;

	//
	// Remplissage de la liste avec les codes correspondant à l'examen
	//
	NSCcam Base(pContexte->getSuperviseur()) ;
	//
	// Ouverture du fichier
	//
	Base.lastError = Base.open() ;
	if (DBIERR_NONE != Base.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes CCAM.", standardError, Base.lastError) ;
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
		_aMatArray.push_back(new NSCcamInfo(&Base)) ;

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
	erreur("Exception ChoixCcamDialog::InitGlobalCCAMArray.", standardError, 0) ;
  return false ;
}
}

void
ChoixCcamDialog::CmElargir()
{
	if (string("") == _sExam)
    return ;

  _bGlobalList = !_bGlobalList ;

  InitCCAMArray() ;

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
ChoixCcamDialog::CmSelectMateriel(WPARAM /* Cmd */)
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
ChoixCcamDialog::CmMaterielDblClk(WPARAM /* Cmd */)
{
	_iCodeChoisi = _pMatBox->IndexItemSelect() + 1 ;
	TDialog::CmOk();
}

void
ChoixCcamDialog::CmOk()
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
ChoixCcamDialog::CmCancel()
{
	_iCodeChoisi = 0 ;
	TDialog::CmCancel() ;
}

void
ChoixCcamDialog::InitListe()
{
	TListWindColumn colCode("Code", 60, TListWindColumn::Left, 0) ;
	_pMatBox->InsertColumn(0, colCode) ;
	TListWindColumn colLibelle("Libelle", 570, TListWindColumn::Left, 1) ;
	_pMatBox->InsertColumn(1, colLibelle) ;
	TListWindColumn colType("Prix", 50, TListWindColumn::Right, 2) ;
	_pMatBox->InsertColumn(2, colType) ;
	TListWindColumn colActif("Freq", 30, TListWindColumn::Left, 3) ;
	_pMatBox->InsertColumn(3, colActif) ;
}

void
ChoixCcamDialog::AfficheListe()
{
	_pMatBox->DeleteAllItems() ;

  if (_aMatArray.empty())
    return ;

	for (CcamReverseIter i = _aMatArray.rbegin() ; _aMatArray.rend() != i ; i++)
	{
    string sLabel = (*i)->getCode() ;
    TListWindItem Item(sLabel.c_str(), 0) ;
    _pMatBox->InsertItem(Item) ;
	}
}

void
ChoixCcamDialog::LvnGetDispInfo(TLwDispInfoNotify& dispInfo)
{
	TListWindItem& dispInfoItem = *(TListWindItem*)&dispInfo.item ;
	int index = dispInfoItem.GetIndex() ;
  if (index < 0)
		return ;

  if ((size_t)index > _aMatArray.size())
  	return ;

  string sBuffer = string("") ;
  size_t iLen ;

  NSCcamInfo* pCCamInfo = _aMatArray[index] ;
  if ((NSCcamInfo*) NULL == pCCamInfo)
    return ;

	// Affiche les informations en fonction de la colonne

	switch (dispInfoItem.GetSubItem())
	{
		case 1 : // Libelle
    	sBuffer = pCCamInfo->getLibelle() ;
      break ;

		case 2 : // Prix
    	sBuffer = pCCamInfo->getPrix() ;
      iLen = strlen(sBuffer.c_str()) ;
      if (iLen > 2)
      	sBuffer = string(sBuffer, 0, iLen - 2) + string(",") + string(sBuffer, iLen - 2, 2) ;
      else if (string("0") == sBuffer)
        sBuffer = string("0,00") ;

      break ;

    case 3 : // Prix
    	sBuffer = pCCamInfo->getFrequence() ;
      break ;
	}

  static char buffer[1024] ;
  strcpy(buffer, sBuffer.c_str()) ;
  dispInfoItem.SetText(buffer) ;
}

void
ChoixCcamDialog::LVNColumnclick(TLwNotify& lwn)
{
  switch ( lwn.iSubItem )
  {
    case 0  : sortByCode() ;    break ;
    case 1  : sortByLibelle() ; break ;
    case 2  : sortByPrice() ;   break ;
    case 3  : sortByFreq() ;    break ;
  }
}

void
ChoixCcamDialog::swapSortOrder(int iColumn)
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
ChoixCcamDialog::sortByCode()
{
	swapSortOrder(0) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByCodeInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByCodeSup) ;

  AfficheListe() ;
}

void
ChoixCcamDialog::sortByLibelle()
{
	swapSortOrder(1) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByLibelleInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByLibelleSup) ;

  AfficheListe() ;
}

void
ChoixCcamDialog::sortByPrice()
{
	swapSortOrder(2) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByPrixInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByPrixSup) ;

  AfficheListe() ;
}

void
ChoixCcamDialog::sortByFreq()
{
	swapSortOrder(3) ;

  if (_aMatArray.empty())
    return ;

  if (_bNaturallySorted)
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByFreqInf) ;
  else
    sort(_aMatArray.begin(), _aMatArray.end(), CcamSortByFreqSup) ;

  AfficheListe() ;
}
#endif // #ifndef _ENTERPRISE_DLL

#ifndef _ENTERPRISE_DLL
// -----------------------------------------------------------------
//
//  Méthodes de NSListeMatWindow
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(NSListeCCAMWindow, TListWindow)
	EV_WM_LBUTTONDBLCLK,
END_RESPONSE_TABLE;

void
NSListeCCAMWindow::SetupWindow()
{
  ListView_SetExtendedListViewStyle(this->HWindow, LVS_EX_FULLROWSELECT) ;
  TListWindow::SetupWindow() ;
}

//---------------------------------------------------------------------------
//  Description: Fonction de réponse au double-click
//---------------------------------------------------------------------------
void
NSListeCCAMWindow::EvLButtonDblClk(uint /* modKeys */, NS_CLASSLIB::TPoint& point)
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
NSListeCCAMWindow::IndexItemSelect()
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

string getBestCcamCodeForExam(string sExam, NSContexte* pContexte, NSPatPathoArray* pPatho)
{
  if ((string("") == sExam) || (NULL == pContexte))
    return string("") ;

  string sExamSens ;
  pContexte->getDico()->donneCodeSens(&sExam, &sExamSens);

  // Opening the referential file
  //
#ifndef _ENTERPRISE_DLL
  string sPathName = pContexte->getSuperviseur()->PathBase("BCPT") ;
  string sReferenceFile = sPathName + sExamSens + string("_") + string("6CCAM.xml") ;
#else
  string sReferenceFile = sExamSens + string("_") + string("6CCAM.xml") ;
#endif

  nsrefParseur Parseur(pContexte->getSuperviseur()) ;
  if (false == Parseur.open(sReferenceFile))
    return string("") ;

  if (NULL == Parseur.getReferentiel())
    return string("") ;

  Valeur_array* pRefArray = Parseur.getReferentiel()->getValArray() ;
  if ((NULL == pRefArray) || pRefArray->empty())
    return string("") ;

  // Initialising global vars
  //
  Cglobalvars *pGVars = Parseur.getReferentiel()->getGVars() ;
  if (pGVars)
    pGVars->process(pContexte, true, 0, pPatho) ;

  VecteurString aExcluded ;
	VecteurString aNeeded ;

  VecteurString aCodes ;
  
  //
  //
  for (ValIter ival = pRefArray->begin() ; pRefArray->end() != ival ; ival++)
  {    if (LABEL_PROPOSITION == (*ival)->getLabel())    {      Cproposition* pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;      if (pCprop)      {
        string sPropID = pCprop->getStringAttribute(ATTRIBUT_PROP_ID) ;
        bool bValid = true ;

        // Excluded
        if ((string("") != sPropID) && (aExcluded.ItemDansUnVecteur(sPropID)))
          bValid = false ;
        // No need to check if Needed
        else if ((string("") == sPropID) || (false == aNeeded.ItemDansUnVecteur(sPropID)))
        {
          for (ValIter i = pCprop->getValArray()->begin() ; bValid && (i != pCprop->getValArray()->end()) ; i++)
          {            if (LABEL_VALIDITE == (*i)->getLabel())            {              Ccontrainte	*pCvalidite = dynamic_cast<Ccontrainte *>((*i)->getObject()) ;              NSValidateur valid(pCvalidite, pContexte) ;              if (false == valid.Validation())              {                bValid = false ;                break ;              }            }          }
        }

        if (bValid)
        {
          pCprop->addExcludedToVector(&aExcluded) ;
          pCprop->addNeededToVector(&aNeeded) ;

          for (ValIter ivalp = pCprop->getValArray()->begin() ; ivalp != pCprop->getValArray()->end() ; ivalp++)
          {
            if (LABEL_TREE == (*ivalp)->getLabel())
            {
              // Creating the tree - On crée l'arbre
              //
              Ctree* pCtree = dynamic_cast<Ctree *>((*ivalp)->getObject()) ;
              string sDestinationPath = pCtree->getLocalisation() ;

              size_t iPos = sDestinationPath.find("6CCAM") ;
              if ((string::npos != iPos) && (strlen(sDestinationPath.c_str())-6 > iPos))
              {
                string sEnd = string(sDestinationPath, iPos + 5, strlen(sDestinationPath.c_str()) - iPos - 5) ;
                if (string("/$") == string(sEnd, 0, 2))
                {
                  string sCode = string(sEnd, 2, strlen(sEnd.c_str()) - 2) ;
                  aCodes.AddString(sCode) ;
                }
              }
            }
          }
        }
      }
    }
  }

  if (aCodes.empty())
    return string("") ;

  string sBestCode  = string("") ;
  int    iBestValue = 0 ;
  int    iBestFrequ = 0 ;

#ifndef _ENTERPRISE_DLL
  //
	// Ouverture du fichier
	//
	NSCcam Code(pContexte->getSuperviseur()) ;

	Code.lastError = Code.open() ;
	if (DBIERR_NONE != Code.lastError)
	{
		erreur("Erreur à l'ouverture du fichier des codes CCAM.", standardError, Code.lastError) ;
		return sBestCode ;
	}

  for (EquiItemIter ival = aCodes.begin() ; aCodes.end() != ival ; ival++)
	{
    NSCcamInfo CCAMinfo ;
    if (DBIERR_NONE == Code.getRecordByCode(**ival, &CCAMinfo, false, true))
    {
      int iValue = StringToInt(CCAMinfo.getPrix()) ;
      int iFrequ = StringToInt(CCAMinfo.getFrequence()) ;

      if ((iValue > iBestValue) ||
          ((iValue == iBestValue) && (iFrequ > iBestFrequ)))
      {
        iBestValue = iValue ;
        iBestFrequ = iFrequ ;
        sBestCode  = CCAMinfo.getCode() ;
      }
    }
  }

	Code.close() ;
#endif // #ifndef _ENTERPRISE_DLL

  return sBestCode ;
}

