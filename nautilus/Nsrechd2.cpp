#include <owl\applicat.h>#include <owl\olemdifr.h>
#include <owl\dialog.h>
#include <owl\edit.h>
#include <owl\color.h>
#include <bde.hpp>
#include <stdio.h>
#include <string.h>

#include "nautilus\nssuper.h"#include "nautilus\nsanxary.h"
#include "nautilus\nsresour.h"
#include "nautilus\nsrechd2.h"
#include "partage\nsdivfct.h"
#include "nautilus\cr.rh"
#include "nautilus\nstypdoc.rh"
#include "nssavoir\nspathor.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsednum.h"
#include "nsbb\tagNames.h"

//***********************************************************************//
//							Classe NSTypeDocument
//***********************************************************************//

/**********************************************************************///  permettre l'édition de champ numérique pour un NSTreeNode
/**********************************************************************/

DEFINE_RESPONSE_TABLE1(NSTypeDocument, NSUtilDialog)  EV_COMMAND(IDOK, CmOk),
  EV_COMMAND(IDCANCEL, CmCancel),
  EV_COMMAND(CONSULTATION_BOUTON, ChoixConsultation),
END_RESPONSE_TABLE;

NSTypeDocument::NSTypeDocument(TWindow* parent, NSContexte* pCtx,                               string* pTypeDocument, TModule* module)
               :NSUtilDialog(parent, pCtx, "TYPE_DOCUMENT", module)
{
  pConsultation = new TButton(this, CONSULTATION_BOUTON) ;
	pType         = new NSUtilLexique(pContexte, this, DOCUMENT_EDIT, pContexte->getDico()) ;
  pTypeDocum    = pTypeDocument ;
}

NSTypeDocument::~NSTypeDocument(){
  delete pConsultation ;
  delete pType ;
}

//----------------------------------------------------------------------------//----------------------------------------------------------------------------
void
NSTypeDocument::CmOk()
{
  // on récupère d'abord un éventuel élément lexique sélectionné par les flèches
  // Le Return n'envoie pas d'EvKeyDown et appelle directement CmOk
  if (pType->getDico()->getDicoDialog()->EstOuvert())
  {
      pType->getDico()->getDicoDialog()->InsererElementLexique();
      return;
  }
  //
  // Ne pas accepter les textes libres
  //
  if (pType->getCode() == string("£?????"))
  {
    erreur("Il faut choisir un code lexique et non pas du texte libre ", standardError, 0, GetHandle()) ;
    pType->SetFocus() ;
    return ;
  }
  *pTypeDocum = pType->getCode() ;

  if (string("") == *pTypeDocum)
    *pTypeDocum = string("GCONS1") ;

	CloseWindow(IDOK);
}


//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
void
NSTypeDocument::CmCancel()
{
    *pTypeDocum = "";
    Destroy(IDCANCEL);
}

void
NSTypeDocument::SetupWindow()
{    // fichiers d'aide

    sHindex = "";
    sHcorps = "Les_Fiches_de.html";

    TDialog::SetupWindow();
}
//---------------------------------------------------------------------
//on a choisi la consultation
//---------------------------------------------------------------------
void
NSTypeDocument::ChoixConsultation()
{
    *pTypeDocum = "GCONS1";
    CloseWindow(IDOK);
}

// -----------------------------------------------------------------//
//  Méthodes de ChoixCRDialog
//
// -----------------------------------------------------------------

#ifdef _INCLUSDEFINE_RESPONSE_TABLE1(ChoixCRDialog, NSDialogWrapper)
#else
DEFINE_RESPONSE_TABLE1(ChoixCRDialog, TDialog)
#endif
	EV_COMMAND(IDOK,		CmOk),
	EV_COMMAND(IDCANCEL,	CmCancel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_SELCHANGE, CmSelectModule),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_DBLCLK, CmModuleDblClk),
END_RESPONSE_TABLE;

//--------------------------------------------------------------------------//      Constructeur
//---------------------------------------------------------------------------

#ifdef _INCLUSChoixCRDialog::ChoixCRDialog(TWindow* pere, string* pChoix, NSContexte* pCtx, TModule* module)
              :NSDialogWrapper(pCtx, pere, "IDD_MODULES", module)
#else
ChoixCRDialog::ChoixCRDialog(TWindow* pere, string* pChoix, string* pImport, NSContexte* pCtx)
              :NSUtilDialog(pere, pCtx, "IDD_MODULES")
#endif
{
try
{
  pContexte     = pCtx ;
	//
	// Création des objets dialogues
	//
	pModulesBox   = new TListBox(this, IDC_MODU_LISTE) ;
	//
	// Initialisation des variables générales
	//
	pModulesArray = new NSModuleArray ;

	lexiqueCR[0]  = '\0' ;
	Contexte 	    = '0' ;
	iModuleChoisi = -1 ;

  pChoisi       = pChoix ;
  pImportDll    = pImport ;
}
catch (const exception& e)
{
  string sExept = "ChoixCRDialog ctor " + string(e.what()) ;
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
  erreur("ChoixCRDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//      Destructeur
//---------------------------------------------------------------------------
ChoixCRDialog::~ChoixCRDialog()
{
	//
	// Détruit les objets de pilotage des dialogues
	//
	delete pModulesBox;
	delete pModulesArray;
}

//---------------------------------------------------------------------------
//  Description: Initialise la boite de liste des modules
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
ChoixCRDialog::SetupWindow()
{
  // fichiers d'aide
  //
  sHindex = string("") ;  sHcorps = string("Compte_Rendus.html") ;

	TDialog::SetupWindow() ;

	string sFichierModules = pContexte->PathName("FPER") + string("MODULES.LUS") ;
	//
	// Ouverture du fichier des modules
	//
	ifstream inFile ;
	inFile.open(sFichierModules.c_str()) ;
	if (!inFile)
	{
		erreur("Erreur à l'ouverture du fichier des modules.", standardError, 0, GetHandle()) ;
		return ;
	}

	//
	// Lecture du fichier
	//
  string sLang = string("") ;
  if (pContexte) 
    sLang = pContexte->getUserLanguage() ;

  int  iNbModules = 0 ;
  char buffer[501] ;

	inFile.unsetf(ios::skipws) ;
  while (inFile.getline(buffer, 500))
	{
    NSModuleInfo moduleInfo ;

    string sString1 = string("") ;
    string sString2 = string("") ;

    int iTailleBuf = strlen(buffer) ;
		if (iTailleBuf >= 8)
		{
			int i = 0 ;
			//
			// Prise du nom du module
			//
      int j = 0 ;

      string sLexiq = string("") ;
			for ( ; (j < MODU_LEXIQUE_LEN) && (i < iTailleBuf) ; j++, i++)
        sLexiq += string(1, buffer[i]) ;

			moduleInfo.setLexique(sLexiq) ;

      // Skip blanks and tabs
      //
			for (; (i < iTailleBuf) && ((' ' == buffer[i]) || ('\t' == buffer[i])); i++) ;

			//
			// Prise du code du module
			//
      string sCode = string("") ;
			for (j = 0 ; (j < MODU_CODE_LEN) && (i < iTailleBuf) ; j++, i++)
				sCode += string(1, buffer[i]) ;

			moduleInfo.setCode(sCode) ;

      // Skip blanks and tabs
      //
			for (; (i < iTailleBuf) && ((' ' == buffer[i]) || ('\t' == buffer[i])) ; i++) ;

      if (i < iTailleBuf)
      {
        for (; (i < iTailleBuf) && (buffer[i] != ' ') && (buffer[i] != '\t'); i++)
          sString1 += buffer[i] ;
        for (; (i < iTailleBuf) && ((buffer[i] == ' ') || (buffer[i] == '\t')); i++) ;
        for (; (i < iTailleBuf) && (buffer[i] != ' ') && (buffer[i] != '\t'); i++)
          sString2 += buffer[i] ;
      }

      if (string("") != sString1)
      {
        if (strlen(sString1.c_str()) == MODU_COMPTE_LEN)
        {
          moduleInfo.setCompte(sString1) ;
          moduleInfo.setImportDll(sString2) ;
        }
        else
        {
          moduleInfo.setImportDll(sString1) ;
          if (strlen(sString2.c_str()) == MODU_COMPTE_LEN)
            moduleInfo.setCompte(sString2) ;
        }
      }
			//
			// Recherche du libellé
			//
      string sLexiqCode = moduleInfo.getLexique() ;
      string sLabel     = string() ;
			pContexte->getDico()->donneLibelle(sLang, &sLexiqCode, &sLabel) ;
      moduleInfo.setLabel(sLabel) ;
			//
			// Ajout du module à la liste
			//
			pModulesBox->AddString(moduleInfo.getLabel().c_str()) ;
			pModulesArray->push_back(new NSModuleInfo(moduleInfo)) ;

      iNbModules++ ;
		}
	}
	inFile.close() ;

  if (1 == iNbModules)
  {
    pModulesBox->SetSelIndex(0) ;
    iModuleChoisi = 1 ;
  }

	return ;
}

//---------------------------------------------------------------------------
//  Arguments :	    Cmd : WPARAM retourné par Windows
//  Description :	Sélectionne un module et sort
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixCRDialog::CmModuleDblClk(WPARAM Cmd)
{
	iModuleChoisi = pModulesBox->GetSelIndex() + 1 ;
	CmOk() ;
}

//---------------------------------------------------------------------------
//  Arguments :	    Cmd : WPARAM retourné par Windows
//  Description :	Sélectionne un module
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixCRDialog::CmSelectModule(WPARAM Cmd)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	iModuleChoisi = pModulesBox->GetSelIndex() + 1 ;
}

//---------------------------------------------------------------------------
//  Description :	Appelle CmOk si un module a été choisi
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixCRDialog::CmOk()
{
	if (iModuleChoisi <= 0)
	{
		erreur("Vous devez sélectionner un module.", standardError, 0, GetHandle()) ;
		return ;
	}

	int indice = iModuleChoisi - 1 ;

	// pour les versions démo
  string sKey   = (*pModulesArray)[indice]->getCompte() ;
	size_t lenKey = strlen(sKey.c_str()) ;

	if (lenKey)
	{
		// la longueur est fixée à deux caractères
		if ((2 != lenKey) || (string("AA") == sKey))
		{
    	erreur("Cette version de démonstration a expiré", standardError, 0) ;
      return ;
    }
    else
    {
    	if ((sKey[0] < 'A') || (sKey[0] > 'Z') || (sKey[1] < 'A') || (sKey[1] > 'Z'))
      {
      	erreur("Cette version de démonstration a expiré", standardError, 0) ;
        return ;
      }
      if ('A' == sKey[1])
      {
      	sKey[0] -= 1 ;
        sKey[1] = 'Z' ;
      }
      else
      	sKey[1] -= 1 ;

      (*pModulesArray)[indice]->setCompte(sKey) ;

      string sOut = "";
      NSModuInfoArrayIter i = pModulesArray->begin();
      for (; i != pModulesArray->end(); i++)
      {
        sOut += (*i)->getLexique() ;
        sOut += string(" ");
        sOut += (*i)->getCode() ;

        string sCompte = (*i)->getCompte() ;
        if (string("") != sCompte)
        {
          sOut += string(" ") ;
          sOut += sCompte ;
        }
        sOut += string("\n") ;
      }

      string sFichierModules = pContexte->PathName("FPER") + string("MODULES.LUS") ;
      ofstream outFile ;
      outFile.open(sFichierModules.c_str()) ;
      if (!outFile)
      {
        erreur("Erreur d'ouverture en écriture du fichier des modules", standardError, 0) ;
        return ;
      }

      for (size_t j = 0; j < strlen(sOut.c_str()); j++)
        outFile.put(sOut[j]) ;

      outFile.close() ;
    }
  }

  //
  // On fixe les données propres au module
  //
  string sLexiqCr = (*pModulesArray)[indice]->getLexique() ;

  strcpy(lexiqueCR, sLexiqCr.c_str()) ;

  *pChoisi    = sLexiqCr ;
  *pImportDll = (*pModulesArray)[indice]->getImportDll() ;

  //
  // On sort
  //
  TDialog::CmOk() ;
}

//---------------------------------------------------------------------------
//  Function :		ChoixCRDialog::CmCancel()
//
//  Arguments :	Aucun
//
//  Description :	Annule ModuleChoisie et appelle Cancel()
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixCRDialog::CmCancel()
{
	iModuleChoisi = -1 ;
  *pChoisi      = string("") ;

	TDialog::CmCancel() ;
}

// -----------------------------------------------------------------//
//  Méthodes de ChoixArchetypeDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(ChoixArchetypeDialog, NSUtilDialog)	EV_COMMAND(IDOK,		 CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_SELCHANGE, CmSelectModule),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_DBLCLK,    CmModuleDblClk),
  EV_WM_RBUTTONDOWN,
END_RESPONSE_TABLE;

//--------------------------------------------------------------------------//      Constructeur
//---------------------------------------------------------------------------

ChoixArchetypeDialog::ChoixArchetypeDialog(TWindow* pere, string* pChoix, NSContexte* pCtx)
                     :NSUtilDialog(pere, pCtx, "IDD_MODULES")
{
try
{
	//
	// Création des objets dialogues
	//
	_pModulesBox   = new ArchetypesListBox(pContexte, this, IDC_MODU_LISTE) ;
	//
	// Initialisation des variables générales
	//
	_pModulesArray = new NSArchetypeArray ;

	_sArchetype    = string("") ;
	_pChoisi       = pChoix ;
	_iModuleChoisi = -1 ;

  sHcorps        = string("dpio_selecteur_exam.htm") ;
}
catch (...)
{
	erreur("ChoixCRDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//      Destructeur
//---------------------------------------------------------------------------
ChoixArchetypeDialog::~ChoixArchetypeDialog()
{
	//
	// Détruit les objets de pilotage des dialogues
	//
	delete _pModulesBox ;
	delete _pModulesArray ;
}

//---------------------------------------------------------------------------
//  Description: Initialise la boite de liste des modules
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
ChoixArchetypeDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

  readFile() ;

  if (1 == _pModulesArray->size())
  {
    _pModulesBox->SetSelIndex(0) ;
    _iModuleChoisi = 1 ;
  }

	return ;
}

//---------------------------------------------------------------------------
//  Arguments :	    Cmd : WPARAM retourné par Windows
//  Description :	Sélectionne un module et sort
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixArchetypeDialog::CmModuleDblClk(WPARAM Cmd)
{
	_iModuleChoisi = _pModulesBox->GetSelIndex() + 1 ;
	CmOk() ;
}

//---------------------------------------------------------------------------
//  Arguments :	    Cmd : WPARAM retourné par Windows
//  Description :	Sélectionne un module
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void
ChoixArchetypeDialog::CmSelectModule(WPARAM Cmd)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	_iModuleChoisi = _pModulesBox->GetSelIndex() + 1 ;
}

//---------------------------------------------------------------------------
//  Description :	Appelle CmOk si un module a été choisi
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixArchetypeDialog::CmOk()
{
  if (_iModuleChoisi <= 0)
  {
    string sWarningText = pContexte->getSuperviseur()->getText("archetypesManagement", "anArchetypeMustBeSelected") ;
    erreur(sWarningText.c_str(), warningError, 0, GetHandle()) ;
    return ;
  }

  int indice = _iModuleChoisi - 1 ;

  //
  // On fixe les données propres au module
  //
  _sArchetype = (*_pModulesArray)[indice]->getArchetype() ;
  *_pChoisi   = _sArchetype ;

  //
  // On sort
  //
  TDialog::CmOk() ;
  return ;
}

//---------------------------------------------------------------------------
//  Function :		ChoixCRDialog::CmCancel()
//
//  Arguments :	Aucun
//
//  Description :	Annule ModuleChoisie et appelle Cancel()
//
//  Retour	: 		Rien
//---------------------------------------------------------------------------
void ChoixArchetypeDialog::CmCancel()
{
  _iModuleChoisi = -1 ;
  *_pChoisi      = string("") ;

	TDialog::CmCancel() ;
}

void
ChoixArchetypeDialog::AddArchetype()
{
  //
	// Get new Archetype
	//
	string sArchId    = string("") ;
  string sArchLabel = string("") ;

	SelectReferencedArchetypeDialog* pChoixArchDlg = new SelectReferencedArchetypeDialog(pContexte->GetMainWindow(),
                                                                &sArchId,
                                                                &sArchLabel,
                                                                pContexte);
	if (pChoixArchDlg->Execute() == IDCANCEL)
		return ;
  if (string("") == sArchId)
    return ;

  // Add it to array and ListBox and update ModuleA.lus
  //
  _pModulesArray->push_back(new NSArchetypeInfo(sArchId, sArchLabel)) ;
  writeFile() ;

  _pModulesBox->AddString(sArchLabel.c_str()) ;

  // Select it and edit label
  //
  _iModuleChoisi = _pModulesArray->size() ;
  _pModulesBox->SetSelIndex(_iModuleChoisi - 1) ;

  EditName() ;
}
voidChoixArchetypeDialog::EditName()
{  if (_iModuleChoisi <= 0)  {
    string sWarningText = pContexte->getSuperviseur()->getText("archetypesManagement", "anArchetypeMustBeSelected") ;
    erreur(sWarningText.c_str(), warningError, 0, GetHandle()) ;
    return ;
  }  // Get selected archetype info  //  NSArchetypeInfo *pArchetypeInfo = (*_pModulesArray)[_iModuleChoisi - 1] ;  if (NULL == pArchetypeInfo)    return ;  // Ask for new label  //  string sLabel = pArchetypeInfo->getLibelle() ;  RenameArchetypeEntryDialog* pEditArchDlg = new RenameArchetypeEntryDialog(pContexte->GetMainWindow(),                                                                &sLabel,
                                                                pContexte) ;
	if (pEditArchDlg->Execute() == IDCANCEL)
		return ;
  if (string("") == sLabel)
    return ;

  // Update moduleA.lus
  //
  pArchetypeInfo->setLibelle(sLabel) ;
  writeFile() ;

  // Update ListBox
  //
  readFile() ;
  _pModulesBox->SetSelIndex(_iModuleChoisi - 1) ;
}boolChoixArchetypeDialog::writeFile(){  string sArchetypesFile = pContexte->PathName("FPER") + string("MODULEA.LUS") ;  ofstream outFile ;  outFile.open(sArchetypesFile.c_str()) ;  if (!outFile)  {  	string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sArchetypesFile ;    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;    return false ;  }  if (_pModulesArray->empty())  {    outFile << string("") ;    outFile.close() ;    return true ;  }  for (NSArchInfoArrayIter it = _pModulesArray->begin() ; _pModulesArray->end() != it ; it++)    outFile << ((*it)->getArchetype() + string(1, '\t') + (*it)->getLibelle() + NEWLINE) ;  outFile.close() ;  return true ;}boolChoixArchetypeDialog::readFile(){  _pModulesArray->vider() ;  _pModulesBox->ClearList() ;  string sArchetypesFile = pContexte->PathName("FPER") + string("MODULEA.LUS") ;	//
	// Ouverture du fichier des modules
	//
	ifstream inFile ;
	inFile.open(sArchetypesFile.c_str()) ;
	if (!inFile)
	{
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sArchetypesFile ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0, GetHandle()) ;
		return false ;
	}
	//
	// Lecture du fichier
	//
  string sLang = string("") ;
  if (pContexte) 
    sLang = pContexte->getUserLanguage() ;

  char buffer[501] ;

  NSArchetypeInfo archInfo ;

	inFile.unsetf(ios::skipws) ;
  while (inFile.getline(buffer, 500))
	{
    archInfo.setArchetype(string("")) ;
    archInfo.setLibelle(string("")) ;

    int iTailleBuf = strlen(buffer) ;

    int j ;

    // Prise de l'Archetype
    //
    string sLabel = string("") ;
    for (j = 0; (j < iTailleBuf) && (' ' != buffer[j]) && ('\t' != buffer[j]); j++)
      sLabel += buffer[j] ;

    archInfo.setArchetype(sLabel) ;

    //
    for (; (j < iTailleBuf) && ((' ' == buffer[j]) || ('\t' == buffer[j])); j++) ;

    // Prise du code du module
    //
    sLabel = string("") ;
    for (; (j < iTailleBuf); j++)
      sLabel += buffer[j] ;

    archInfo.setLibelle(sLabel) ;

    // Ajout du module à la liste
    //
    _pModulesBox->AddString(archInfo.getLibelle().c_str()) ;
    _pModulesArray->push_back(new NSArchetypeInfo(archInfo)) ;
	}

	inFile.close() ;  return true ;}//***************************************************************************
//
// Implémentation des méthodes NSModule
//
//***************************************************************************
//---------------------------------------------------------------------------//  Function:    NSModuleData::metAZero()
//
//  Description: Met à 0 les variables.
//---------------------------------------------------------------------------
void
NSModuleData::metAZero()
{
	//
	// Met les champs de données à zéro
	//
	memset(code, 	  0, MODU_CODE_LEN + 1) ;
	memset(lexique, 0, MODU_LEXIQUE_LEN + 1) ;
  memset(compte, 	0, MODU_COMPTE_LEN + 1) ;

  libelle     = string("") ;  sImportDll  = string("") ;
}

//---------------------------------------------------------------------------
//  Function:    NSModuleData::initialiseLibelle()
//
//  Description: Va chercher le libelle dans le lexique.
//---------------------------------------------------------------------------
void
NSModuleData::initialiseLibelle()
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSModuleData::NSModuleData(const NSModuleData& rv)
{
	strcpy(code,	  rv.code) ;
	strcpy(lexique, rv.lexique) ;
  strcpy(compte,  rv.compte) ;

  libelle     = rv.libelle ;
  sImportDll  = rv.sImportDll ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSModuleData&
NSModuleData::operator=(const NSModuleData& src)
{
  if (this == &src)
		return *this ;

	strcpy(code,	  src.code) ;
	strcpy(lexique, src.lexique) ;
  strcpy(compte,  src.compte) ;

  libelle     = src.libelle ;
  sImportDll  = src.sImportDll ;

	return *this ;
}

//---------------------------------------------------------------------------
//  Opérateur de comparaison
//---------------------------------------------------------------------------
int
NSModuleData::operator==(const NSModuleData& o)
{
  if ((strcmp(code,	  o.code)	 == 0) &&
      (strcmp(lexique, o.lexique) == 0))
    return 1 ;

  return 0;
}

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSModuleInfo::NSModuleInfo()
{
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSModuleInfo::~NSModuleInfo()
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSModuleInfo::NSModuleInfo(const NSModuleInfo& rv)
{
  _Donnees = rv._Donnees ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSModuleInfo&
NSModuleInfo::operator=(const NSModuleInfo& src)
{
  if (this == &src)
		return *this ;

  _Donnees = src._Donnees ;

	return *this ;
}

int
NSModuleInfo::operator == ( const NSModuleInfo& o )
{
  return (_Donnees == o._Donnees) ;
}

//***************************************************************************
// Implémentation des méthodes NSModuleArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSModuleArray::NSModuleArray(const NSModuleArray& rv)
              :NSModuInfoArray()
{
try
{
	if (rv.empty())
		return ;

	for (NSModuInfoArrayConstIter i = rv.begin() ; rv.end() != i ; i++)
  	push_back(new NSModuleInfo(*(*i))) ;
}
catch (...)
{
  erreur("Exception NSModuleArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Vidange
//---------------------------------------------------------------------------
void
NSModuleArray::vider()
{
  if (empty())
    return ;

  for (NSModuInfoArrayIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSModuleArray::~NSModuleArray()
{
	vider() ;
}

NSModuleArray&
NSModuleArray::operator=(const NSModuleArray& src)
{
  if (this == &src)
		return *this ;

  vider() ;

  if (false == src.empty())
	  for (NSModuInfoArrayConstIter i = src.begin() ; src.end() != i ; i++)
  	  push_back(new NSModuleInfo(*(*i))) ;

  return *this ;
}

//////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//  Constructeur par défaut
//---------------------------------------------------------------------------
NSArchetypeInfo::NSArchetypeInfo()
{
  _sArchetype = string("") ;
  _sLibelle   = string("") ;
}

NSArchetypeInfo::NSArchetypeInfo(string sId, string sLabel)
{
  _sArchetype = sId ;
  _sLibelle   = sLabel ;
}

//---------------------------------------------------------------------------
//  Destructeur.
//---------------------------------------------------------------------------
NSArchetypeInfo::~NSArchetypeInfo()
{
}

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSArchetypeInfo::NSArchetypeInfo(const NSArchetypeInfo& rv)
{
  _sArchetype = rv._sArchetype ;
  _sLibelle   = rv._sLibelle ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
NSArchetypeInfo& NSArchetypeInfo::operator=(const NSArchetypeInfo& src)
{
  if (this == &src)
		return *this ;

  _sArchetype = src._sArchetype ;
  _sLibelle   = src._sLibelle ;

	return *this ;
}

int NSArchetypeInfo::operator==(const NSArchetypeInfo& o)
{
  return ((_sArchetype == o._sArchetype) &&
          (_sLibelle   == o._sLibelle)) ;
}

//***************************************************************************
// Implémentation des méthodes NSArchetypeArray
//***************************************************************************

//---------------------------------------------------------------------------
//  Constructeur copie
//---------------------------------------------------------------------------
NSArchetypeArray::NSArchetypeArray(const NSArchetypeArray& rv)
                 :NSArcheInfoArray()
{
try
{
	if (rv.empty())
  	return ;

	for (NSArchInfoArrayConstIter i = rv.begin() ; rv.end() != i ; i++)
  	push_back(new NSArchetypeInfo(*(*i))) ;
}
catch (...)
{
	erreur("Exception NSArchetypeArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  Vidange
//---------------------------------------------------------------------------
void
NSArchetypeArray::vider()
{
  if (empty())
    return ;

	for (NSArchInfoArrayIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSArchetypeArray::~NSArchetypeArray()
{
	vider() ;
}

NSArcheInfoArray&
NSArchetypeArray::operator=(const NSArcheInfoArray& src)
{
  if (this == &src)
		return *this ;

  vider() ;

  if (src.empty())
  	return *this ;

	for (NSArchInfoArrayConstIter i = src.begin() ; src.end() != i ; i++)
  	push_back(new NSArchetypeInfo(*(*i))) ;

  return *this ;
}

#define IDC_ALB_ADD  101
#define IDC_ALB_EDIT 102

DEFINE_RESPONSE_TABLE1(ArchetypesListBox, TListBox)
  EV_WM_RBUTTONDOWN,
  EV_COMMAND(IDC_ALB_ADD,  CmAddArchetype),
  EV_COMMAND(IDC_ALB_EDIT, CmEditName),
END_RESPONSE_TABLE;

ArchetypesListBox::ArchetypesListBox(NSContexte *pCtx, ChoixArchetypeDialog* parent, int id, int x, int y, int w, int h, TModule* module)
                  :TListBox((TWindow*) parent, id, x, y, w, h, module)
{
  pContexte = pCtx ;
  _pParent  = parent ;
}

ArchetypesListBox::ArchetypesListBox(NSContexte *pCtx, ChoixArchetypeDialog* parent, int resourceId, TModule* module)
                  :TListBox((TWindow*) parent, resourceId, module)
{
  pContexte = pCtx ;
  _pParent  = parent ;
}

void
ArchetypesListBox::EvRButtonDown(uint modKeys, NS_CLASSLIB::TPoint& point)
{
  TPopupMenu *menu = new TPopupMenu() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	string sAdd  = pSuper->getText("archetypesLauncher", "addNewArchetype") ;
  string sEdit = pSuper->getText("archetypesLauncher", "editLabel") ;

  if (GetSelIndex() >= 0)
  {
	  menu->AppendMenu(MF_STRING, IDC_ALB_EDIT, sEdit.c_str()) ;
    menu->AppendMenu(MF_SEPARATOR, 0, 0) ;
  }
  menu->AppendMenu(MF_STRING, IDC_ALB_ADD, sAdd.c_str()) ;

	ClientToScreen(point) ;
	menu->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON, point, 0, HWindow) ;
	delete menu ;
}

void
ArchetypesListBox::CmAddArchetype()
{
  _pParent->AddArchetype() ;
}

void
ArchetypesListBox::CmEditName()
{
  _pParent->EditName() ;
}

// -----------------------------------------------------------------
//
//  Méthodes de SelectReferencedArchetypeDialog
//
// -----------------------------------------------------------------

bool
SortArchByIdInf(NSArchetypeInfo *pInfo1, NSArchetypeInfo *pInfo2)
{
	return (pInfo1->getArchetype() < pInfo2->getArchetype()) ;
}

bool
SortArchByIdSup(NSArchetypeInfo *pInfo1, NSArchetypeInfo *pInfo2)
{
	return (pInfo1->getArchetype() > pInfo2->getArchetype()) ;
}

DEFINE_RESPONSE_TABLE1(SelectReferencedArchetypeDialog, NSUtilDialog)	EV_COMMAND(IDOK,		 CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_SELCHANGE, CmSelectModule),
	EV_CHILD_NOTIFY_AND_CODE(IDC_MODU_LISTE, LBN_DBLCLK,    CmModuleDblClk),
END_RESPONSE_TABLE;

//--------------------------------------------------------------------------//      Constructeur
//---------------------------------------------------------------------------

SelectReferencedArchetypeDialog::SelectReferencedArchetypeDialog(TWindow* parent, string* pSelectedId, string* pSelectedName, NSContexte* pCtx)
                                :NSUtilDialog(parent, pCtx, "IDD_MODULES")
{
try
{
	//
	// Création des objets dialogues
	//
	_pModulesBox   = new TListBox(this, IDC_MODU_LISTE) ;
	//
	// Initialisation des variables générales
	//
	_pModulesArray = new NSArchetypeArray ;

	_pSelectedId   = pSelectedId ;
  _pSelectedName = pSelectedName ;
	_iSelected     = -1 ;

  sHcorps        = string("dpio_selecteur_exam.htm") ;
}
catch (...)
{
	erreur("SelectReferencedArchetypeDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//      Destructeur
//---------------------------------------------------------------------------
SelectReferencedArchetypeDialog::~SelectReferencedArchetypeDialog()
{
	//
	// Détruit les objets de pilotage des dialogues
	//
	delete _pModulesBox ;
	delete _pModulesArray ;
}

//---------------------------------------------------------------------------
//  Description: Initialise la boite de liste des modules
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
SelectReferencedArchetypeDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

  string sCaption   = pContexte->getSuperviseur()->getText("archetypesLauncher", "archetypesSelection") ;
  string sListTitle = pContexte->getSuperviseur()->getText("archetypesLauncher", "archetypesList") ;

  SetCaption(sCaption.c_str()) ;
  _pModulesBox->SetCaption(sListTitle.c_str()) ;

  initArchetypesArray() ;

  if (1 == _pModulesArray->size())
  {
    _pModulesBox->SetSelIndex(0) ;
    _iSelected = 1 ;
  }

	return ;
}

void
SelectReferencedArchetypeDialog::CmModuleDblClk(WPARAM Cmd)
{
	_iSelected = _pModulesBox->GetSelIndex() + 1 ;
	CmOk() ;
}

void
SelectReferencedArchetypeDialog::CmSelectModule(WPARAM Cmd)
{
	//
	// Récupération de l'indice de la chemise sélectionnée
	//
	_iSelected = _pModulesBox->GetSelIndex() + 1 ;
}

void SelectReferencedArchetypeDialog::CmOk()
{
  if (_iSelected <= 0)
  {
    string sWarningText = pContexte->getSuperviseur()->getText("archetypesManagement", "anArchetypeMustBeSelected") ;
    erreur(sWarningText.c_str(), warningError, 0, GetHandle()) ;
    return ;
  }

  setSelectedInfo() ;

  //
  // On sort
  //
  TDialog::CmOk() ;
  return ;
}

void SelectReferencedArchetypeDialog::CmCancel()
{
  _iSelected = -1 ;
  setSelectedInfo() ;

	TDialog::CmCancel() ;
}

void SelectReferencedArchetypeDialog::setSelectedInfo()
{
  if (_iSelected < 1)
  {
    if (_pSelectedId)
      *_pSelectedId   = string("") ;
    if (_pSelectedName)
      *_pSelectedName = string("") ;

    return ;
  }

  NSArchetypeInfo *pArchetype = (*_pModulesArray)[_iSelected - 1] ;

  if (_pSelectedId)
    *_pSelectedId   = pArchetype->getArchetype() ;
  if (_pSelectedName)
    *_pSelectedName = pArchetype->getLibelle() ;
}

bool SelectReferencedArchetypeDialog::initArchetypesArray()
{
try
{
  _pModulesArray->vider() ;

	NSPersonsAttributesArray ObjList ;
	NSBasicAttributeArray    AttrList ;

  string sTraitName = string("_0ARCH_0ARID") ;

	AttrList.push_back(new NSBasicAttribute(sTraitName, "")) ;
	AttrList.push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

	bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
	if (false == res)
		return false ;

	if (ObjList.empty())
  	return true ;

  NSDataGraph Graph(pContexte->getSuperviseur(), graphObject) ;

	for (NSPersonsAttributeIter k = ObjList.begin() ; ObjList.end() != k ; k++)
  {
  	string sOIDS = (*k)->getAttributeValue(OIDS) ;

    // Get object from Oids
    //
		NSBasicAttributeArray AttrArray ;
		AttrArray.push_back(new NSBasicAttribute(OBJECT, sOIDS)) ;
    res = pContexte->getSuperviseur()->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), &Graph, &AttrArray) ;
    if (false == res)
    {
    	string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
      if (sErrorText == "")
      	sErrorText = string("Echec service : Impossible de récupérer un objet dans la base") ;
      sErrorText += string(" (") + sOIDS + string(")") ;
      erreur(sErrorText.c_str(), standardError, 0) ;
    }
    else
    {
      NSDataTreeIter iterTree ;
      if (Graph.getTrees()->ExisteTree(string("0ARCH1"), pContexte->getSuperviseur(), &iterTree))
      {
      	string sFichArc = ArchetypeGetFile(pContexte->getSuperviseur(), *iterTree, string("0ARCH")) ;
        string sNomArc  = ArchetypeGetID(pContexte->getSuperviseur(), *iterTree, string("0ARCH")) ;

        _pModulesArray->push_back(new NSArchetypeInfo(sNomArc, sNomArc)) ;
      }
    }
  }

  sort(_pModulesArray->begin(), _pModulesArray->end(), SortArchByIdInf) ;

  for (NSArchInfoArrayIter it = _pModulesArray->begin() ;
                                         _pModulesArray->end() != it ; it++)
    _pModulesBox->AddString(((*it)->getLibelle()).c_str()) ;

	return true ;
}
catch (...)
{
	erreur("Exception ArchetypesListDialog::InitArray.", standardError, 0) ;
	return false ;
}
}

// -----------------------------------------------------------------
//
//  Méthodes de RenameArchetypeEntryDialog
//
// -----------------------------------------------------------------

DEFINE_RESPONSE_TABLE1(RenameArchetypeEntryDialog, NSUtilDialog)
	EV_COMMAND(IDOK,		 CmOk),
	EV_COMMAND(IDCANCEL, CmCancel),
END_RESPONSE_TABLE;

//--------------------------------------------------------------------------//      Constructeur
//---------------------------------------------------------------------------

RenameArchetypeEntryDialog::RenameArchetypeEntryDialog(TWindow* parent, string* pName, NSContexte* pCtx)
                           :NSUtilDialog(parent, pCtx, "IDD_LABEL_EDIT")
{
try
{
	//
	// Création des objets dialogues
	//
	_pEditBox = new TEdit(this, IDC_MODU_LISTE) ;

  _pName    = pName ;

  sHcorps        = string("dpio_selecteur_exam.htm") ;
}
catch (...)
{
	erreur("SelectReferencedArchetypeDialog ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//      Destructeur
//---------------------------------------------------------------------------
RenameArchetypeEntryDialog::~RenameArchetypeEntryDialog()
{
	//
	// Détruit les objets de pilotage des dialogues
	//
	delete _pEditBox ;
}

//---------------------------------------------------------------------------
//  Description: Initialise la boite de liste des modules
//
//  Returns:	  Rien
//---------------------------------------------------------------------------
void
RenameArchetypeEntryDialog::SetupWindow()
{
	TDialog::SetupWindow() ;

  string sCaption = pContexte->getSuperviseur()->getText("archetypesLauncher", "editLabel") ;

  SetCaption(sCaption.c_str()) ;

  string sText = string("") ;
  if (_pName)
    sText = *_pName ;

  _pEditBox->SetText(sText.c_str()) ;
}

void RenameArchetypeEntryDialog::CmOk()
{
  if (_pName)
  {
    int  iBuffLen = _pEditBox->GetTextLen() ;
    char *pszBuff = new char[iBuffLen + 2] ;
    _pEditBox->GetText(pszBuff, iBuffLen + 1) ;

    *_pName = string(pszBuff) ;

    delete[] pszBuff ;
  }
  
  TDialog::CmOk() ;
}

void RenameArchetypeEntryDialog::CmCancel()
{
	TDialog::CmCancel() ;
}

