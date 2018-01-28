//Kaddachi Hafedh
#include <owl\olemdifr.h>#include <owl\applicat.h>
#include <stdexcept>
#include <iostream>

#include "partage\nsdivfct.h"
#include "nautilus\nssuper.h"
#include "partage\nsdivfile.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbbsmal.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbb.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsbb_msg.h"
#include "nsbb\nsdlg.h"
#include "nsbb\nsbbdivfct.h"
#include "nsepisod\nsToDo.h"

#include "ns_ob1\nautilus-bbk.h"

#include "nsbb\nsarc.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nspanesplitter.h"

/*** Constructeur - Constructor
*/

NSSmallBrother::NSSmallBrother(NSContexte* pCtx, NSPatPathoArray* pArray,
                               NSMagicView* pMagVue)
               :NSRoot(pCtx), _TmpPPtArray(pCtx->getSuperviseur())
{
try
{
	_pBBFiche            = (BBFiche*) 0 ;
	_pNSSmallVue         = pMagVue ;
	_pPatPathoArray      = pArray ;
	_pBBItem             = (BBItem*) 0 ;
	_pFenetreMere        = (TWindow*) 0 ;
	_contexteModule      = '0';
	_sLexiqueModule      = string("") ;
	_sNoeudModule        = string("") ;
	_bCorrection         = false ; 		   //Savoir si on corrige une phrase ou non
	_sCheminCorrection   = string("") ;  //Chemin de la correction
	_sPositionConclusion = string("") ;  //Position de conclusion manuelle par rapport
                                       //à la conclusion automatique
	_sFichierSauveRoot   = string("") ;
	_iSeuilSauve         = 10000 ;
	_iNumSauve 	         = 0 ;
	_iNumMaxSauve        = 0 ;

  _bUseAutoConc        = true ;
  _bMustDeleteTmpFile  = false ;
  _sModuleName         = string("") ;
}
catch (...)
{
	erreur("Exception NSSmallBrother ctor.", standardError, 0) ;
}
}

/***  Constructeur copie - Copy constructor
*/
NSSmallBrother::NSSmallBrother(NSSmallBrother& src)
               :NSRoot(src.pContexte),
                _TmpPPtArray(src.pContexte->getSuperviseur()),
                _VectorCalcul(src._VectorCalcul),
                _aResModuleArray(src._aResModuleArray),
                _aCmdMsgArray(src._aCmdMsgArray)
{
try
{
	_pBBFiche 					 = src._pBBFiche ;

	_pPatPathoArray 		 = src._pPatPathoArray ;
	_pBBItem 						 = src._pBBItem ;
	_bCorrection 				 = src._bCorrection ;
	_VectorCalcul 			 = src._VectorCalcul ;
	_sPositionConclusion = src._sPositionConclusion ;
	_sFichierSauveRoot   = src._sFichierSauveRoot;
	_iSeuilSauve         = src._iSeuilSauve;
	_iNumSauve 	         = src._iNumSauve;
	_iNumMaxSauve        = src._iNumMaxSauve;

  _bUseAutoConc        = src._bUseAutoConc ;
  _bMustDeleteTmpFile  = false ;
  _sModuleName         = src._sModuleName ;
}
catch (...)
{
	erreur("Exception NSSmallBrother copy ctor.", standardError, 0);
}
}

//---------------------------------------------------------------------------
// Destructeur
//---------------------------------------------------------------------------
NSSmallBrother::~NSSmallBrother()
{
  if (_pBBItem)
    delete _pBBItem ;

  if (_bMustDeleteTmpFile)
	  NsDeleteTemporaryFile(pContexte->getSuperviseur(), string("arbre.tmp")) ;
}

bool
NSSmallBrother::lanceModule()
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;
	if (false == pSuper->getFilGuide()->Prend())
		return false ;

	_pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;

	_iSeuilSauve = 10 ;

	return lanceScript() ;
}

//---------------------------------------------------------------------------// Lancement de l'élaboration d'un compte rendu
//---------------------------------------------------------------------------
bool
NSSmallBrother::lanceScript()
{
try
{
	if (false == ouvreScript())
		return false ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	//
	// On commence par vider la pile de messages (pour ne pas être pollué par
	// des messages non traités)
	//
	_aCmdMsgArray.vider() ;

  // Populating patpatho with elements from patient record
  //
  InjectHistoryElements() ;

	EnleverTitre() ;
	//
	// Création du BBItem ammorce
	//
  if (false == CreateBBItemFromFiche())
    return false ;

	string sCodeSens ;
	NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;
	_pBBItem->_sLocalisation = sCodeSens ;

	//
	// Si le module ne contient pas la conclusion (pas de 0CONC1 dans le guide)
	// il faut conserver dans une variable temporaire la conclusion, sinon elle
	// sera écrasée
	//
	NSPatPathoArray* pPPT = _pPatPathoArray ;
	NSPatPathoArray PatTempo(pSuper) ;

	PatPathoIter iterConclusion ;

	if (false == pPPT->empty())
	{
		iterConclusion = pPPT->begin() ;
    iterConclusion = ChercherItem(pPPT, "0CONC1") ;
    if (pPPT->end() != iterConclusion)
      pPPT->ExtrairePatPatho(iterConclusion, &PatTempo) ;
  }

  string sTrace = "BB : Lancement du module..." ;
  pContexte->getSuperviseur()->trace(&sTrace, 1) ;
  //
  // Lancement du module
  //
	_pBBItem->creer() ;

	/* int resAct = */ _pBBItem->activer() ;
  sTrace = "BB : ...Fin du module" ;  pContexte->getSuperviseur()->trace(&sTrace, 1) ;
  //
  // Remise en place de la conclusion
  //
  sTrace = "BB : Remise en place de la conclusion..." ;
  pContexte->getSuperviseur()->trace(&sTrace, 1) ;

  if (false == pPPT->empty())
  {
    iterConclusion = pPPT->begin() ;
    iterConclusion = ChercherItem(pPPT, "0CONC1") ;

    _sPositionConclusion = _pBBItem->getItemSonsList() ;
    if ((pPPT->end() == iterConclusion) && (false == PatTempo.empty()))
    {
      //
      // Mettre à jour les localisations des éléments de la cl
      //
      PatPathoIter jter = PatTempo.begin() ;
      for ( ; PatTempo.end() != jter ; jter++)
      {
        (*jter)->setLigne((*jter)->getLigne() + 1) ;
        (*jter)->setColonne((*jter)->getColonne() + 1) ;
      }
      jter = PatTempo.begin() ;
      PatTempo.ajoutePatho(jter, "0CONC1", 0, -1) ;
      //
      // Concaténer les 2 patpathos
      //
      ConcatenerPatpatho("0CONC1", &PatTempo, pPPT, false) ;
    }
  }
  sTrace = "BB : ...Fin de remise en place de la conclusion" ;
  pContexte->getSuperviseur()->trace(&sTrace, 1) ;

  if (false == getPatPatho()->empty())
  {
    // On remet en place le titre à la racine de la patpatho
    //
    sTrace = "BB : Remise en place du titre..." ;
    pContexte->getSuperviseur()->trace(&sTrace, 1) ;

    MetTitre() ;

    sTrace = "BB : Titre remis en place..." ;
    pContexte->getSuperviseur()->trace(&sTrace, 1) ;

    bool retour = true ;

    if (_bUseAutoConc)
    {
      //
      sTrace = "BB : Prise du pointeur sur le module..." ;
      pContexte->getSuperviseur()->trace(&sTrace, 1) ;

      TModule* pModule = TrouverModule(_pBBItem->getItemDialogFile()) ;

      sTrace = "BB : ...Pointeur sur le module pris" ;
      pContexte->getSuperviseur()->trace(&sTrace, 1) ;

      bool (FAR *pAdresseFct) (NSSmallBrother far*, int) ; //on déclare un pointeur
                                                              //sur une fonction
      if (pModule)
      {
        // On cherche le nom de fichier de la dll
        //
        string sModule = string(pModule->GetName()) ;

        sTrace = "BB : Module : " + sModule ;
        pContexte->getSuperviseur()->trace(&sTrace, 1) ;
        if (string("") == sModule)
        {
          delete _pBBItem ;
          _pBBItem = (BBItem*) 0 ;
          return true ;
        }
        string sNom = "" ;
        int i = strlen(sModule.c_str()) - 1 ;
        while ((i >= 0) && (sModule[i] != '\\') && (sModule[i] != ':'))
        {
          sNom = string(1, pseumaj(sModule[i])) + sNom ;
          i-- ;
        }
        if ((sNom == "NSBB.DLL") || (sNom == "NSMBB.DLL"))
        {
          delete _pBBItem ;
          _pBBItem = (BBItem*) 0 ;
          return true ;
        }
        //
        // instancier le pointeur (recherche dans bb_res.def)
        //
        sTrace = "BB : Prise du pointeur sur la conclusion..." ;
        pContexte->getSuperviseur()->trace(&sTrace, 1) ;
        (FARPROC) pAdresseFct = pModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
        sTrace = "BB : ...Pointeur sur la conclusion récupéré" ;
        pContexte->getSuperviseur()->trace(&sTrace, 1) ;
        if (pAdresseFct)
        {
          sTrace = "BB : Lancement de la conclusion automatique..." ;
          pContexte->getSuperviseur()->trace(&sTrace, 1) ;
          retour = (*pAdresseFct)(this, 0) ;
          sTrace = "BB : ...Conclusion automatique terminée" ;
          pContexte->getSuperviseur()->trace(&sTrace, 1) ;
        }
      }
    }
    delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;
    return retour ;
  }
  delete _pBBItem ;
  _pBBItem = (BBItem*) 0 ;
	return false ;
}
catch (...)
{
	erreur("Exception au lancement du script.", standardError, 0) ;
	return false ;
}
}
boolNSSmallBrother::EstFormulaire(){  return _pBBItem->ouvreArchetype() ;}NSDialog*NSSmallBrother::lanceBbkArchetypeInDialog(string sArchetype, nsarcParseur* pParseur, BBFilsItem* pFilsPere, BB1BBInterfaceForKs* pKsInter, bool bModal){try{	NSSuper* pSuper = pContexte->getSuperviseur() ;	if (false == pSuper->getFilGuide()->Prend())		return (NSDialog*) 0 ;
	if (false == pContexte->getDico()->Prend())
		return (NSDialog*) 0 ;	// Création du BBItem ammorce (le père)	//
	_pBBItem = new BBItem(pContexte, this) ;

	// on donne artificellement le fils pere au BBItem
	// pour le cas des archetypes multi-niveaux
	_pBBItem->_pBBFilsPere = pFilsPere ;

	BBItemData Data ;

	// indispensable dans activer :
	strcpy(Data.ouvreDialogue, "A") ;
	strcpy(Data.fils, sArchetype.c_str()) ;
	strcpy(Data.decalageNiveau, "+00+00") ;

	_pBBItem->setItemData(&Data) ;

	if (pKsInter)
  	_pBBItem->_KsInterface = *pKsInter ;
  else
		// Make certain it can be inited by BBK
  	_pBBItem->_KsInterface.setInitFromBbk(true) ;

	if (pParseur)
	{
		_pBBItem->setParseur(pParseur) ;		_pBBItem->_bCreateParseur = false ;	}  else	{    nsarcParseur* pBBItemParser = new nsarcParseur(pSuper) ;		_pBBItem->setParseur(pBBItemParser) ;		_pBBItem->_bCreateParseur = true ;
		string sArchetypeFile = pSuper->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sArchetype) ;
		if (string("") == sArchetypeFile)
		{
      string sErrorMsg = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") + string(" ") + sArchetype ;
			erreur(sErrorMsg.c_str(), standardError, 0) ;
			delete _pBBItem ;
      _pBBItem = (BBItem*) 0 ;
			return (NSDialog*) 0 ;
		}

		if (false == pBBItemParser->open(sArchetypeFile))
		{
      string sErrorMsg = pSuper->getText("archetypesManagement", "archetypeParsingError") + string(" ") + sArchetypeFile ;
			erreur(sErrorMsg.c_str(), standardError, 0) ;
    	delete _pBBItem ;
      _pBBItem = (BBItem*) 0 ;
    	return (NSDialog*) 0 ;
  	}	}
  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pBBItem->getParseur()) ;
  if ((nsarcParseur*) NULL == pArcParseur)
    return (NSDialog*) 0 ;

	Carchetype* pArchetype = pArcParseur->getArchetype() ;
	if ((Carchetype*) NULL == pArchetype)
	{
    string sErrorMsg = pSuper->getText("archetypesManagement", "archetypeExecutionError") ;
    erreur(sErrorMsg.c_str(), standardError, 0) ;
		delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;
    return (NSDialog*) 0 ;
  }

  // on renseigne les champs définissant le dialogue
  // (ces champs seront utilisés par BBItem::activer dans creerDialogue()
  //  qui ne sera appelée que pour le BBItem Root)
  if (pArchetype->getDialog())
  {
  	string sNomDialog = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_NOM) ;
    _pBBItem->setDialogName(sNomDialog) ;
    string sNomDll = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_DLL) ;
    _pBBItem->getItemData()->setDialogFile(sNomDll) ;
    _pBBItem->_sIdArchetype = pArchetype->getName() ;
  }
  else
  {
    if (string("") != _pBBItem->getDialogName())
  	  _pBBItem->setDialogName(string("")) ;
    _pBBItem->getItemData()->setDialogFile(string("")) ;
    _pBBItem->_sIdArchetype = pArchetype->getName() ;
  }

	string sMsg = "lanceBbkArchetype() : Calling creerArchetype" ;
	pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  // on développe tout l'archetype à partir du BBItem root
  _pBBItem->_bModalDialog = bModal ;
  _pBBItem->creerArchetype(pArchetype->getRootItem(), false) ;

	_pBBItem->activer() ;
	if (_pBBItem->getDialog())  {		if (pArchetype)    {    	Creferences* pRef = pArchetype->getReference() ;      if (pRef)      {      	Chead* pHead = pRef->getHead() ;        if (pHead)        {        	string sTitle = pHead->getTitle() ;          if (string("") != sTitle)          	_pBBItem->getDialog()->SetCaption(sTitle.c_str()) ;          string sUrl = pHead->getHelpUrl();          if (string("") != sUrl)          	_pBBItem->getDialog()->_sHelpBodyUrl = sUrl ;        }      }    }  }	return _pBBItem->getDialog() ;}catch (...)
{
	erreur("Exception NSSmallBrother::lanceBbkArchetypeInDialog.", standardError, 0) ;
	return (NSDialog*) 0 ;
}}boolNSSmallBrother::lanceBbkArchetypeInView(string sArchetype, nsarcParseur* pParseur, BBFilsItem* pFilsPere, NSBBMUEView* pView, BB1BBInterfaceForKs* pKsInter){try{	NSSuper* pSuper = pContexte->getSuperviseur() ;	if (false == pSuper->getFilGuide()->Prend())		return false ;
	if (false == pContexte->getDico()->Prend())
		return false ;	// Création du BBItem ammorce (le père)	//
	_pBBItem = new BBItem(pContexte, this) ;

	// on donne artificellement le fils pere au BBItem
	// pour le cas des archetypes multi-niveaux
  //
	_pBBItem->_pBBFilsPere = pFilsPere ;
	_pBBItem->setView(pView)  ;

	BBItemData Data ;

	// indispensable dans activer :
	strcpy(Data.ouvreDialogue, "A") ;
	strcpy(Data.fils, sArchetype.c_str()) ;
	strcpy(Data.decalageNiveau, "+00+00") ;

	_pBBItem->setItemData(&Data) ;

	if (pKsInter)
  	_pBBItem->_KsInterface = *pKsInter ;
  else
		// Make certain it can be inited by BBK
  	_pBBItem->_KsInterface.setInitFromBbk(true) ;

	if (pParseur)
	{
		_pBBItem->setParseur(pParseur) ;		_pBBItem->_bCreateParseur = false ;	}  else	{    nsarcParseur *pNewParseur = new nsarcParseur(pSuper) ;		_pBBItem->setParseur(pNewParseur) ;		_pBBItem->_bCreateParseur = true ;
		string sArchetypeFile = pContexte->getSuperviseur()->getArcManager()->DonneFichierArchetypeParNom(NSArcManager::archetype, sArchetype) ;
		if (string("") == sArchetypeFile)
		{
			string sErrorMsg = pSuper->getText("archetypesManagement", "cannotFindThisArchetypeFile") ;
      sErrorMsg += string(" ") + sArchetype ;
			erreur(sErrorMsg.c_str(), standardError, 0) ;
			delete _pBBItem ;
      _pBBItem = (BBItem*) 0 ;
			return false ;
		}

		if (false == pNewParseur->open(sArchetypeFile))
		{
			string sErrorMsg = pSuper->getText("archetypesManagement", "archetypeParsingError") ;
      sErrorMsg += string(" ") + sArchetypeFile ;
			erreur(sErrorMsg.c_str(), standardError, 0) ;
    	delete _pBBItem ;
      _pBBItem = (BBItem*) 0 ;
    	return false ;
  	}	}
  nsarcParseur *pArcParseur = dynamic_cast<nsarcParseur*>(_pBBItem->getParseur()) ;
  if ((nsarcParseur*) NULL == pArcParseur)
    return false ;

	Carchetype* pArchetype = pArcParseur->getArchetype() ;
	if ((Carchetype*) NULL == pArchetype)
	{
		string sErrorMsg = pSuper->getText("archetypesManagement", "archetypeExecutionError") ;
    erreur(sErrorMsg.c_str(), standardError, 0) ;
		delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;
    return false ;
  }

  // on renseigne les champs définissant le dialogue
  // (ces champs seront utilisés par BBItem::activer dans creerDialogue()
  //  qui ne sera appelée que pour le BBItem Root)
  if ((Cdialogue*) NULL != pArchetype->getDialog())
  {
  	string sNomDialog = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_NOM) ;
    _pBBItem->setDialogName(sNomDialog) ;
    string sNomDll = pArchetype->getDialog()->getStringAttribute(ATTRIBUT_DIALOGUE_DLL) ;
    _pBBItem->getItemData()->setDialogFile(sNomDll) ;
    _pBBItem->_sIdArchetype = pArchetype->getName() ;
  }
  else
  {
    if (string("") != _pBBItem->getDialogName())
  	  _pBBItem->setDialogName(string("")) ;
    _pBBItem->getItemData()->setDialogFile(string("")) ;
    _pBBItem->_sIdArchetype = pArchetype->getName() ;
  }

	string sMsg = "lanceBbkArchetype(): Calling creerArchetype" ;
	pSuper->trace(&sMsg, 1, NSSuper::trSubDetails) ;

  // on développe tout l'archetype à partir du BBItem root
  _pBBItem->_bModalDialog = false ;
  _pBBItem->creerArchetype(pArchetype->getRootItem(), false) ;

  string sLang = pContexte->getUserLanguage() ;

  _sModuleName = pArchetype->getTitle(sLang) ;

	return true ;
}catch (...)
{
	erreur("Exception NSSmallBrother::lanceBbkArchetypeInView.", standardError, 0) ;
	return false ;
}}boolNSSmallBrother::fermeBbkArchetype(int /* iReturn */, TWindow* view){	NSSuper* pSuper = pContexte->getSuperviseur() ;	NSToDoTask* pTask = new NSToDoTask ;
	pTask->setWhatToDo(string("FermeArchetype")) ;
	pTask->setPointer1((void*) this, false) ;
	pTask->setPointer2((void*) view, false) ;
  pTask->setParam1(_sModuleName) ;

	pTask->sendMe(pSuper) ;

	return true ;}voidNSSmallBrother::canCloseArchetype(bool /* bCanClose */, bool bCanCloseDPIO){	if (NULL == _pBBItem)		return ;	NSBBMUEView* pMView = _pBBItem->getView() ;  if ((NULL == pMView) || (false == pMView->IsWindow()))		return ;  // pBBItem->getDialog()->ForceCanClose();  pMView->getPaneSplitter()->youCanCloseNow() ;/*	if (pMView)	{		pBBItem->getDialog()->ForceCanClose();		pMView->getPaneSplitter()->youCanCloseNow() ;	}	else	{		if (bCanClose)			pBBItem->getDialog()->CloseWindow(IDOK) ;	}*/	if (bCanCloseDPIO)	{
		NSSuper* pSuper = pContexte->getSuperviseur() ;
		NSToDoTask* pTask = new NSToDoTask ;
		pTask->setWhatToDo(string("FermeDPIO")) ;
		pTask->setPointer1((void*) pMView, false) ;

		pTask->sendMe(pSuper) ;
	}
}//----------------------------------------------------------------------------//lancement de la conclusion automatique
//----------------------------------------------------------------------------
bool
NSSmallBrother::ConclusionAutomatique()
{
  return true ;
}

//----------------------------------------------------------------------------
//¨Prepare the previous and new conclusions
//----------------------------------------------------------------------------
void
NSSmallBrother::preparePrevAndNewConc(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT)
{
  if (((NSPatPathoArray*) NULL == pPreviousPPT) || ((NSPatPathoArray*) NULL == pNewPPT))
    return ;

  pPreviousPPT->vider() ;
  pNewPPT->vider() ;

  // Get the pathpatho before automatic synchronisation of conclusion with description
  //
  if ((NSPatPathoArray*) NULL == _pPatPathoArray)
    return ;

  // If there is no description and no conclusion, nothing to do
  //
  if (_pPatPathoArray->empty())
    return ;

  //
  // Prepare the new conclusion from the previous one
  //
  PatPathoIter iterConclusion = ChercherItem(_pPatPathoArray, string("0CONC1")) ;
  if ((_pPatPathoArray->end() == iterConclusion) || (NULL == iterConclusion))
  {
    pNewPPT->ajoutePatho("0CONC1", 0) ;
    return ;
  }

  //
  // pPatAncienneCl contains previous conclusion
  //
  _pPatPathoArray->ExtrairePatPathoWithRoot(iterConclusion, pPreviousPPT) ;
  (*pNewPPT) = (*pPreviousPPT) ;
}

//----------------------------------------------------------------------------
// Check if conclusion changed and ask the user to arbitrate
//----------------------------------------------------------------------------
void
NSSmallBrother::arbitrateConcChanges(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT)
{
  if (((NSPatPathoArray*) NULL == pPreviousPPT) || ((NSPatPathoArray*) NULL == pNewPPT))
    return ;

  //
  // If the new conclusion only contains 0CONC1, without anything else,
  // then we must remove this element to make it "plainly empty"
  //
  PatPathoIter iterConclusion = ChercherItem(pNewPPT, string("0CONC1")) ;

  if ((pNewPPT->end() != iterConclusion) && (NULL != iterConclusion))
  {
    PatPathoIter iterSuite = iterConclusion ;
    if (pNewPPT->end() != iterSuite)
      iterSuite++ ;
    if (pNewPPT->end() == iterSuite)
      pNewPPT->SupprimerItem(iterConclusion) ;
  }

  // Get the patpatho
  //
  if ((NSPatPathoArray*) NULL == _pPatPathoArray)
    return ;

  bool bSwitch = false ;

  //
  // comparer la nouvelle et l'ancienne conclusion, et si elles sont différentes
  // proposer à l'utilisateur de tenir compte des modif ou non
  //
  if (false == pPreviousPPT->empty())
  {
    if (areConcDifferent(pPreviousPPT, pNewPPT))
    {
      int rep = MessageBox(0, "Voulez-vous tenir compte des modifications apportées à la conclusion ?", "Message", MB_YESNOCANCEL) ;
      if ((IDCANCEL == rep) || (IDNO == rep))
        return ;
      if (IDYES == rep)
        bSwitch = true ;
    }
  }
  else if (false == pNewPPT->empty())
    bSwitch = true ;

  if (false == bSwitch)
    return ;

  // Remove previous conclusion
  //
  PatPathoIter iterPptConclusion = ChercherItem(_pPatPathoArray, string("0CONC1")) ;
  if ((_pPatPathoArray->end() != iterPptConclusion) && (NULL != iterPptConclusion))
    _pPatPathoArray->SupprimerItem(iterPptConclusion) ;

  // Add new one
  //
  ConcatenerPatpatho(string("0CONC1"), pNewPPT, _pPatPathoArray) ;
}

//----------------------------------------------------------------------------
// Check if the previous and the new conclusions are different
//----------------------------------------------------------------------------
bool
NSSmallBrother::areConcDifferent(NSPatPathoArray* pPreviousPPT, NSPatPathoArray* pNewPPT)
{
  if (((NSPatPathoArray*) NULL == pPreviousPPT) && ((NSPatPathoArray*) NULL == pNewPPT))
    return false ;

  if (((NSPatPathoArray*) NULL == pPreviousPPT) || ((NSPatPathoArray*) NULL == pNewPPT))
    return true ;

  if (pPreviousPPT->empty() && pNewPPT->empty())
    return false ;

  if (pPreviousPPT->empty() || pNewPPT->empty())
    return true ;

  PatPathoIter it = pPreviousPPT->begin() ;
  PatPathoIter jt = pNewPPT->begin() ;

  while ((pPreviousPPT->end() != it) && (pNewPPT->end() != jt))
  {
    // l'ancienne et la nouvelle patpatho doivent avoir
    // les mêmes code patients et document
    (*jt)->setPerson((*it)->getPerson()) ;
    (*jt)->setDocum((*it)->getDocum()) ;

    // if (!(*((*it)->getDataTank()) == *((*jt)->getDataTank())))
    if (false == (*jt)->sameContent(*it))
      return true ;

    // Reinject node Id
    //
    (*jt)->setNodeID((*it)->getNodeID()) ;

    it++ ;
    jt++ ;
  }

  // If a conclusion has the same initial nodes but is shorter
  //
  if ((pPreviousPPT->end() != it) || (pNewPPT->end() != jt))
    return true ;

  return false ;
}

//---------------------------------------------------------------------------//---------------------------------------------------------------------------
bool
NSSmallBrother::lanceScript(NSTreeWindow* pTreeWind)
{
try
{
	//
	// Créer le pemier BBItem (GCONS le père)
	//
  if (false == CreateBBItemFromFiche())
    return false ;

	string sCodeSens ;
	NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;
	_pBBItem->_sLocalisation = sCodeSens ;
	pTreeWind->pBBitemNSTreeWindow = _pBBItem ;
	_pBBItem->creer() ;

	//
	// Créer le deuxième BBItem (GCONS le fils)
	//
	BBFilsItem* pFilsItem = *(_pBBItem->_aBBItemFils.begin()) ;
	int creer = _pBBItem->developperConsultation(pFilsItem) ;
	if (0 != creer)
		return false ;

	return true ;
}
catch (...)
{
	erreur("Exception au lancement du script.", standardError, 0) ;
	return false ;
}
}

//----------------------------------------------------------------------------
//	inject selected record elements in patpatho (only when ppt is empty)
//----------------------------------------------------------------------------
void
NSSmallBrother::InjectHistoryElements()
{
  if ((false == _pPatPathoArray->empty()) || (NULL == _pBBFiche))
    return ;

  BBFiche* pBBCommeFiche = static_cast<BBFiche*>(_pBBFiche) ;
  if ((BBFiche*) NULL == pBBCommeFiche)
    return ;

	BBItemData* pDo = pBBCommeFiche->getData() ;
  if ((BBItemData*) NULL == pDo)
    return ;

  // Get module name
  //
  string sModuleName = pDo->getDialogFile() ;
  string sUpName     = pseumaj(sModuleName) ;

  if ((string("NSBB") == sUpName) || (string("NSMBB") == sUpName))
    return ;

  TModule* pModule = TrouverModule(sModuleName) ;
  if ((TModule*) NULL == pModule)
    return ;

  // Get function pointer
  //
  bool (FAR *pAdresseFct) (HistoryConnectorArray far*, string far*) ;

  string sTrace = "BB : Getting pointer on history connexion for dll " + sModuleName ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  (FARPROC) pAdresseFct = pModule->GetProcAddress(MAKEINTRESOURCE(4)) ;
  if (NULL == pAdresseFct)
    return ;

  sTrace = "BB : ...Pointer found..." ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

  HistoryConnectorArray aHistoryConnector ;

  // Fill HistoryConnector array
  //
  string sRootConcept = pDo->getSemanticPath() ;

  if (false == (*pAdresseFct)(&aHistoryConnector, &sRootConcept))
  {
    sTrace = "BB : Failed getting history elements for module. Leaving." ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return ;
  }

  if (aHistoryConnector.empty())
  {
    sTrace = "BB : There is no history element for this module. Leaving." ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    return ;
  }

  HistoryConnectorIter hcIter = aHistoryConnector.begin() ;
  for ( ; aHistoryConnector.end() != hcIter ; hcIter++)
  {
    switch((*hcIter)->getConnectionType())
    {
      case HistoryConnector::isConcern :
        InjectHistoryElementsFromConcern(*hcIter) ;
        break ;
      case HistoryConnector::isSynthesis :
        InjectHistoryElementsFromSynthesis(*hcIter) ;
        break ;
    }
  }

  sTrace = "BB : ...Conclusion automatique terminée" ;
  pContexte->getSuperviseur()->trace(&sTrace, 1) ;
}

void
NSSmallBrother::InjectHistoryElementsFromConcern(HistoryConnector* pHistConn)
{
  if (((HistoryConnector*) NULL == pHistConn) || (string("") == pHistConn->getItemLabel()))
    return ;

  // Find such an element in health index
  //
  string sHealthIndexPath = string("ZPOMR/0PRO1/") + pHistConn->getItemLabel() ;

  NSSearchStruct searchStruct ;
  bool bOk = pContexte->getPatient()->ChercheChemin(sHealthIndexPath, &searchStruct) ;
  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  _pPatPathoArray->AjouterChemin(pHistConn->getDestinationPath(), pHistConn->getItemLabel(), true) ;
}

void
NSSmallBrother::InjectHistoryElementsFromSynthesis(HistoryConnector* pHistConn)
{
  if (((HistoryConnector*) NULL == pHistConn) || (string("") == pHistConn->getItemLabel()))
    return ;

  // Find such an element in the synthesis
  //
  string sSynthesisPath = pHistConn->getHistoryPath() + string(1, cheminSeparationMARK) + pHistConn->getItemLabel() ;

  NSSearchStruct searchStruct ;
  bool bOk = pContexte->getPatient()->ChercheChemin(sSynthesisPath, &searchStruct) ;
  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  std::string date   = string("") ;
  std::string sNoeud = string("") ;

  MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ;
  searchStruct.getFoundNodes()->fullRData(it, date, sNoeud) ;

  NSPatPathoArray pptAnswer(pContexte->getSuperviseur()) ;

  // Get array from node
  //
  pContexte->getPatient()->DonneArray(sNoeud, &pptAnswer) ;
  if (pptAnswer.empty())
    return ;

  // pptAnswer.clearAllIDs() ;

  // pptAnswer may contain multiple instances of the same information
  //
  // Get a vector of "sister patpathos", each being an instance of desired item
  //
  NSVectPatPathoArray Vect  ;
  PatPathoIter iterRoot = pptAnswer.begin() ;
  pptAnswer.ExtraireVecteurPatPathoFreres(iterRoot, &Vect) ;

  if (Vect.empty())
    return ;

  PatPathoIterVect iterVect = Vect.begin() ;
  while (Vect.end() != iterVect)
  {
    // Get the sub-patpatho to remove root node
    //
    NSPatPathoArray subPpt(**iterVect) ;
    CleanHistoryPatpatho(&subPpt, pHistConn) ;

    // If sub-patpatho has not been totally "cleaned"... we insert it
    //
    if (false == subPpt.empty())
    {
      // Fist, check if needed path is already there
      //
      string sRootPath = pHistConn->getDestinationPath() ;

      string sLocalisation = string("") ;
      PatPathoIter pathIter = _pPatPathoArray->ChaineDansPatpatho(sRootPath, &sLocalisation, string("/")) ;
      if ((NULL == pathIter) || (_pPatPathoArray->end() == pathIter) || (sLocalisation != sRootPath))
        _pPatPathoArray->AjouterChemin(sRootPath, string("")) ;

      PatPathoIter elmtIter = _pPatPathoArray->ChaineDansPatpatho(sRootPath, &sLocalisation, string("/")) ;
      if ((NULL != elmtIter) && (_pPatPathoArray->end() != elmtIter) && (sRootPath == sLocalisation))
      {
        // Get root node ID for model
        //
        PatPathoIter iterData = subPpt.begin() ;
        string sRootNodeId = (*iterData)->getNode() ;

        subPpt.clearAllIDs() ;

        // Connect information to model root node before inserting
        //
        (*iterData)->addTemporaryLink(sRootNodeId, NSRootLink::copyOf, dirFleche) ;

        _pPatPathoArray->InserePatPathoFille(elmtIter, &subPpt) ;
      }
    }

    iterVect++ ;
  }
}

void
NSSmallBrother::CleanHistoryPatpatho(NSPatPathoArray* pHistoPpt, HistoryConnector* pHistConn)
{
  if (((NSPatPathoArray*) NULL == pHistoPpt) || ((HistoryConnector*) NULL == pHistConn))
    return ;

  if (pHistoPpt->empty())
    return ;

  // Get all leaves and check if their path is in HistoryConnector. If not, delete it
  //
  bool bDeletedLeaf = true ;
  while ((false == pHistoPpt->empty()) && (true == bDeletedLeaf))
  {
    bDeletedLeaf = false ;

    bool bParsePatho = true ;
    PatPathoIter iter = pHistoPpt->begin() ;
    for ( ; (pHistoPpt->end() != iter) && (bParsePatho) ; )
    {
      PatPathoIter iterPere = pHistoPpt->ChercherPremierFils(iter) ;

      // This item is a leaf
      //
      if ((NULL == iterPere) || (pHistoPpt->end() == iterPere))
      {
        string sLeafPath = pHistoPpt->donneCheminItem(iter) ;
        if (false == pHistConn->isInSubPath(sLeafPath))
        {
          pHistoPpt->SupprimerItem(iter) ;
          bDeletedLeaf = true ;
        }
        else
          iter++ ;
      }
      else
        iter++ ;
    }
  }
}

//----------------------------------------------------------------------------//	enlever le titre au compte rendu :  exemple : GECHY
//----------------------------------------------------------------------------
void
NSSmallBrother::EnleverTitre()
{
  //
  // enlever titre
  //
  NSPatPathoArray* pPPT = getPatPatho() ;
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return ;

  PatPathoIter gechy = pPPT->begin() ;
  PatPathoIter j     = pPPT->begin() ;
  if (pPPT->end() == gechy)
    return ;

  _sNoeudModule = (*gechy)->getNodeID() ;

  delete *gechy;
  pPPT->erase(gechy) ;

  j = pPPT->begin() ;
  while(pPPT->end() != j)
  {
    (*j)->setColonne((*j)->getColonne() - 1 ) ;
    (*j)->setLigne((*j)->getLigne() - 1 ) ;
    j++ ;
  }
}

//----------------------------------------------------------------------------
//	Remet le titre à la racine de la patpatho
//----------------------------------------------------------------------------
void
NSSmallBrother::MetTitre()
{
  // Take care not to shift all lines and columns for nothing
  //
  if (string("") == _sLexiqueModule)
    return ;

  NSPatPathoArray* pPPTEnCoursTitre = getPatPatho() ;
  if ((NSPatPathoArray*) NULL == pPPTEnCoursTitre)
    return ;

  pPPTEnCoursTitre->ajouteTitre(_sLexiqueModule, _sNoeudModule) ;

  if (pPPTEnCoursTitre->empty())
    return ;

  // on décale tout d'un cran (sauf le titre) après insertion du titre
  PatPathoIter j = pPPTEnCoursTitre->begin() ;
  if (pPPTEnCoursTitre->end() != j)
    j++ ;
  else
    return ;

  while (pPPTEnCoursTitre->end() != j)
  {
    (*j)->setColonne((*j)->getColonne() + 1) ;
    (*j)->setLigne((*j)->getLigne() + 1) ;
    j++ ;
  }
}

//----------------------------------------------------------------------------
//pLocalisation		: localisation du BBItem correspondant à la phrase à corriger
//----------------------------------------------------------------------------
bool
NSSmallBrother::corrigeModule(string* pLocalisation)
{
try
{
  if (((string*) NULL == pLocalisation) || (string("") == *pLocalisation))
    return false ;

  NSPatPathoArray* pPPT = getPatPatho() ;
	if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
		return false ;

	_bCorrection = true ;
	string sCodeSens ;

	//
	// Trouver le noeud ayant pLocalisation comme localisation
  // Find the node with pLocalisation as a location
	//
	bool bTrouve = false ;

  bool bInConclusion      = false ;
  bool bFoundInConclusion = false ;

  // We have to explore all nodes (and not stop when found) to make sure all
  // nodes'IDs are properly set to 0 or 1
  //
  PatPathoIter j = pPPT->begin() ;
	while (pPPT->end() != j)
	{
    if ((*j)->getColonne() == 1)
    {
      if ((*j)->getLexiqueSens() == string("0CONC"))
        bInConclusion = true ;
      else
        bInConclusion = false ;
    }

		if ((*j)->getLocalisation() == *pLocalisation)
		{
			(*j)->_ID = 1 ;
			bTrouve = true ;

      if (bInConclusion)
        bFoundInConclusion = true ;
		}
		else
			(*j)->_ID = 0 ;
		j++ ;
	}

	if (false == bTrouve)
		return false ;

  _bUseAutoConc = true ;
  if (bFoundInConclusion)
    _bUseAutoConc = false ;

	//
	// Création du BBItem ammorce
	//
	ouvreScript() ;
	if (false == CreateBBItemFromFiche())
    return false ;

	EnleverTitre() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;
	_pBBItem->_sLocalisation = sCodeSens ;

	//
  // si le module ne contient pas la conclusion (pas de 0CONC1 dans le guide)
  // il faut conserver dans une variable temporaire la conclusion, sinon elle
  // sera écrasée
  //
	NSPatPathoArray PatTempo(pSuper) ;

  PatPathoIter iterConclusion = pPPT->begin() ;
  iterConclusion = ChercherItem(pPPT, "0CONC1") ;
  if (pPPT->end() != iterConclusion)
  	pPPT->ExtrairePatPatho(iterConclusion, &PatTempo) ;

	_pBBItem->creer() ;
	/* int resAct = */ _pBBItem->activer() ;

	iterConclusion = pPPT->begin() ;
  iterConclusion = ChercherItem(pPPT, string("0CONC1")) ;
  _sPositionConclusion = _pBBItem->getItemSonsList() ;

  //
  // si le module ne contient pas la conclusion (pas de 0CONC1 dans le guide)
  // on la réintroduit
  //
  if (_bUseAutoConc && (pPPT->end() == iterConclusion) && (false == PatTempo.empty()))
  {
  	//
    // Mettre à jour les localisations des éléments de la cl
    //
    PatPathoIter jter = PatTempo.begin() ;
    for(; PatTempo.end() != jter ; jter++)
    {
    	(*jter)->setLigne((*jter)->getLigne() + 1) ;
      (*jter)->setColonne((*jter)->getColonne() + 1) ;
    }
    jter = PatTempo.begin() ;
    PatTempo.ajoutePatho(jter, string("0CONC1"), 0, -1) ;
    //
    //concaténer les 2 patpathos
    //
    ConcatenerPatpatho(string("0CONC1"), &PatTempo, pPPT, false) ;
	}

	if (false == getPatPatho()->empty())
	{
  	// ajoute le titre au compte rendu :  GECHY
    //
    MetTitre() ;

    bool retour = true ;

    // Calling the NSConcAutomatique function in dedicated dll
    //
    if (_bUseAutoConc)
    {
      TModule* pModule = TrouverModule(_pBBItem->getItemDialogFile()) ;
      bool (FAR *pAdresseFct) (NSSmallBrother far*, int) ;    //on déclare un pointeur
                                                                //sur une fonction
      if (pModule)
      {
    	  //
        //instancier le pointeur (recherche dans bb_res.def)
        //
        (FARPROC) pAdresseFct = pModule->GetProcAddress(MAKEINTRESOURCE(2)) ;
        if (pAdresseFct)
      	  retour = (*pAdresseFct)(this, 0) ;
      }
    }

    // If the conclusion is limited to "0CONC1" as a lone element, we have to
    // remove it
    //
    iterConclusion = ChercherItem(pPPT, string("0CONC1")) ;
    if (pPPT->end() != iterConclusion)
    {
      PatPathoIter iterNext = iterConclusion ;
      iterNext++ ;
      if ((pPPT->end() == iterNext) || ((*iterNext)->getColonne() == 1))
        pPPT->SupprimerItem(iterConclusion) ;
    }

    delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;

    return retour ;
	}
	delete _pBBItem ;
	_pBBItem = (BBItem*) 0 ;
	return false ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::corrigeModule.", standardError, 0) ;
	return false ;
}
}

//----------------------------------------------------------------------------
//pLocalisation		: localisation du BBItem correspondant à la phrase à corriger
//----------------------------------------------------------------------------
bool
NSSmallBrother::SupprimerLigneCR(string* pLocalisation)
{
try
{
  if (((string*) NULL == pLocalisation) || (string("") == *pLocalisation))
    return false ;

  NSPatPathoArray* pPPT = getPatPatho() ;
	if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
		return false ;

	//
	// Trouver le noeud ayant pLocalisation comme localisation
	//
	bool trouve = false;
	bool bContinuer = true;
  PatPathoIter Supprimer = pPPT->begin() ;
	while ((pPPT->end() != Supprimer) && (bContinuer))
	{
		if ((*Supprimer)->getLocalisation() == *pLocalisation)
		{
			(*Supprimer)->_ID = 1 ;
			trouve = true ;
			bContinuer = false ;
		}
		else
		{
			(*Supprimer)->_ID = 0 ;
			Supprimer++ ;
		}
	}

	if (false == trouve)
		return false ;

	int ligneSupprimer   = (*Supprimer)->getLigne() ;
	int colonneSupprimer = (*Supprimer)->getColonne() ;

	//
	// Enlever l'élément Supprimer et tous ses descendants
	//
	pPPT->SupprimerItem(Supprimer) ;
	//
	// Supprimer éventuellement le père de Supprimer
	//
	pPPT->SupprimerPere(ligneSupprimer, colonneSupprimer) ;
	if (getPatPatho()->empty())
		return true ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

	if (false == pSuper->getFilGuide()->Prend())
		return false ;

	_pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;
	PatPathoIter iJ = getPatPatho()->begin() ;
	// On prend l'élément
	string sCodeLex = (*iJ)->getLexique() ;
	// Puis son code utile
	// string sCodeUtile = "";
	// pSuper->pDico->donneCodeSens(&sCodeLex, &sCodeUtile);

  bool retour = true ;

  if (_bUseAutoConc)
  {
    _sLexiqueModule = sCodeLex ;
    if (false == ouvreScript())
      return false ;

    BBItemData* pBBitemData = _pBBFiche->getData() ;
    if (NULL == pBBitemData)
      return false ;

    _sPositionConclusion = pBBitemData->getSonsList() ;

    TModule* pModule = TrouverModule(pBBitemData->getDialogFile()) ;
    if (NULL == pModule)
      return false ;

    string sModule = string(pModule->GetName()) ;
    if (string("") == sModule)
      return true ;

    string sNom = string("") ;
    int i = strlen(sModule.c_str()) - 1 ;
    while ((i >= 0) && (sModule[i] != '\\') && (sModule[i] != ':'))
    {
      sNom = string(1, pseumaj(sModule[i])) + sNom ;
      i-- ;
    }
    if ((sNom == "NSBB.DLL") || (sNom == "NSMBB.DLL"))
      return true ;

    bool (FAR *pAdresseFct) (NSSmallBrother far*, int) ; //on déclare un pointeur
                                                              //sur une fonction
    //
    //instancier le pointeur (recherche dans bb_res.def)
    //
    (FARPROC) pAdresseFct = pModule->GetProcAddress(MAKEINTRESOURCE(2)) ;

    if (pAdresseFct)
      retour = (*pAdresseFct)(this, 0) ;
  }

	return retour ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::SupprimerLigneCR.", standardError, 0) ;
	return false;
}
}

//-----------------------------------------------------------------------//***********************************************************************
//							outils pour traiter les conclusions
//***********************************************************************
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
//trouver un item dont le code lexique est sItem
//-----------------------------------------------------------------------
PatPathoIter
NSSmallBrother::ChercherItem(NSPatPathoArray* pPPT, string sItem)
{
  if ((NSPatPathoArray*) NULL == pPPT)
    return (PatPathoIter) NULL ;

  return pPPT->ChercherItem(sItem) ;
}

//------------------------------------------------------------------------//ajoute élément à la patptho
//------------------------------------------------------------------------
/*
void
NSSmallBrother::ajoutePatho(NSPatPathoArray* pPPT, string sItem, int ligne, int colonne)
{
  pPPT->ajoutePatho(sItem, ligne, colonne);
}
*/

//--------------------------------------------------------------------------//donner au fils de pBBItem le bout de patpatho (s'il existe) qui le concerne
//pour qu 'il puisse l'afficher dans la fenêtre des textes libres
//--------------------------------------------------------------------------
void
NSSmallBrother::ActiverPatpathoBBfils(NSPatPathoArray* pPatpathoActuelle, BBItem* pBBItem, PatPathoIter iterPere)
{
  if (((NSPatPathoArray*) NULL == pPatpathoActuelle) || (pPatpathoActuelle->end() == iterPere))
    return ;

  if (((BBItem*) NULL == pBBItem) || (NULL == pBBItem->_pBBFilsPere))
    return ;

  // patpatho à afficher dans la fenêtre des textes libres ou
  // de la conclusion automatique
  NSVectFatheredPatPathoArray* pNSVectPatPathoArray = pBBItem->_pBBFilsPere->getPatPatho() ;
  pNSVectPatPathoArray->vider() ;
  NSPatPathoArray* pPathRes =
        	pBBItem->_pBBFilsPere->getPatpathoActive(pPatpathoActuelle, iterPere) ;

  if (pPathRes)
    delete pPathRes ;
}

//--------------------------------------------------------------------------
//recherche sémantique de l'élément de code lexique sItem et dont le père
//a pour localisation sLocalisation
//--------------------------------------------------------------------------
bool
NSSmallBrother::RechercheSemantique(string sItem, string sLocalisation)
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
  VecteurSelonCritere.AjouteEtiquette(sItem) ;
  pSuper->getFilGuide()->chercheChemin(&sLocalisation ,
                             &VecteurSelonCritere, NSFilGuide::compReseau) ;

  _pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;

  bool       bFound ;
  BBItemData Donnees ;
  VecteurSelonCritere.SetData(sItem, &bFound, &Donnees) ;

  return bFound ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::RechercheSemantique.", standardError, 0) ;
  return false ;
}
}

//-------------------------------------------------------------------------------
// Initialize _pBBItem from _pBBFiche
//-------------------------------------------------------------------------------
bool
NSSmallBrother::CreateBBItemFromFiche()
{
  if (_pBBItem)
    delete _pBBItem ;

  _pBBItem = new BBItem(pContexte, this) ;

	BBFiche* pBBCommeFiche = static_cast<BBFiche*>(_pBBFiche) ;
  if ((BBFiche*) NULL == pBBCommeFiche)
    return false ;

	BBItemData* pDo = pBBCommeFiche->getData() ;
  if ((BBItemData*) NULL == pDo)
    return false ;

	_pBBItem->setItemData(pDo) ;
	_pBBItem->setDialogName(_pBBItem->getItemDialogName()) ;

  return true ;
}

//-------------------------------------------------------------------------------//décaler les lignes et les colonnes des éléments de pPathDestination par rapport
//au dernier élément de pPathSource
//-------------------------------------------------------------------------------
void
NSSmallBrother::MettreAjourPatpatho(NSPatPathoArray* pPathDestination,
											              NSPatPathoArray* pPathSource)
{
  if (((NSPatPathoArray*) NULL == pPathDestination) || (pPathDestination->empty()))
    return ;

  if (((NSPatPathoArray*) NULL == pPathSource) || (pPathSource->empty()))
    return ;

  PatPathoIter j = pPathDestination->begin() ;
  for ( ; pPathDestination->end() != j ; j++)
  {
    (*j)->setLigne((*j)->getLigne() + (pPathSource->back())->getLigne() + 1) ;
    (*j)->setColonne((*j)->getColonne() + (pPathSource->back())->getColonne() + 1) ;  }
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
void
NSSmallBrother::ConcatenerPatpatho(NSPatPathoArray* pPatpathoSource,
                                   NSPatPathoArray* pPatpathoDestination,
                                   bool bDecalage)
{
	if (((NSPatPathoArray*) NULL == pPatpathoDestination) ||
      ((NSPatPathoArray*) NULL == pPatpathoSource))
    return ;

  if (pPatpathoSource->empty())
    return ;

  int decaligne = 0 ;
	if (false == pPatpathoDestination->empty())
  {
    if ((pPatpathoDestination->back())->getDataTank())
      decaligne = (pPatpathoDestination->back())->getLigne() + 1 ;
    else
    {
      erreur("ConcatenerPatpatho : patpatho invalide", standardError, 0) ;
      return;
    }
  }
  //
  //concaténer les 2 patpathos
  //
try
{
  PatPathoIter j = pPatpathoSource->begin() ;
  for ( ; pPatpathoSource->end() != j ; j++)
  {
    (*j)->setLigne((*j)->getLigne() + decaligne) ;
    if (bDecalage)
      (*j)->setColonne((*j)->getColonne() + 1) ;

    pPatpathoDestination->push_back(new NSPatPathoInfo(*(*j))) ;
  }
}
catch (...)
{
	erreur("Exception NSSmallBrother::ConcatenerPatpatho.", standardError, 0) ;
}
}

//----------------------------------------------------------------------//concaténer les deux patpatho selon la position  de sItem dans sPositionConclusion
//----------------------------------------------------------------------
void
NSSmallBrother::ConcatenerPatpatho(string sItem, NSPatPathoArray* pPatpathoSource,
                                   NSPatPathoArray* pPatpathoDestination,
                                   bool bDecalage)

{
	if (((NSPatPathoArray*) NULL == pPatpathoDestination) ||
      ((NSPatPathoArray*) NULL == pPatpathoSource))
		return ;
  if (pPatpathoSource->empty())
  	return ;
  if (string("") == _sPositionConclusion)
  	return ;

try
{
	// ajouter sItem selon sa position dans les fils de GECHY/GDUOB...
  // (données par) sPositionConclusion
  ClasseStringVector Vect ;
  DecomposeChaine(&_sPositionConclusion, &Vect, "|") ;
  if (Vect.empty())
  {
  	ConcatenerPatpatho(pPatpathoSource, pPatpathoDestination) ;
    return ;
  }

  iterClassString iterpVect = Vect.begin() ;
  bool bContinuer = true ;
  while ((Vect.end() != iterpVect) && bContinuer)
  {
  	if ((*iterpVect)->getItem() == sItem)
    	bContinuer = false ;
    else
    	iterpVect++ ;
  }
  //si sItem est le dernier fils de GECHY/GDUOB..., on ajoute pPatpathoSource
  //à la fin de pPatpathoDestination
  //sinon on l'ajoute selon la position de sItem dans sPositionConclusion
  if ((Vect.end() == iterpVect) || (Vect.back() == *iterpVect))
  	ConcatenerPatpatho(pPatpathoSource, pPatpathoDestination, bDecalage) ;
  else
  {
  	iterClassString iterSuivant = iterpVect ;
    iterSuivant++ ;
    if (Vect.end() == iterSuivant)
    	ConcatenerPatpatho(pPatpathoSource, pPatpathoDestination, bDecalage) ;
    else
    {
    	PatPathoIter iterAvant = pPatpathoDestination->ChercherItem((*iterSuivant)->getItem()) ;
      if (pPatpathoDestination->end() == iterAvant)
      	ConcatenerPatpatho(pPatpathoSource, pPatpathoDestination, bDecalage) ;
      else
      	//Insérer pPatpathoSource dans pPatpathoDestination juste avant iterAvant
        pPatpathoDestination->InserePatPatho(iterAvant, pPatpathoSource) ;
    }
  }
}
catch (...)
{
	erreur("Exception NSSmallBrother::ConcatenerPatpatho.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------// Ouverture de la DLL qui contient les ressources de ce type d'examen
//---------------------------------------------------------------------------
bool
NSSmallBrother::ouvreResModule()
{
	return true ;
}

//-------------------------------------------------------------------//sert pour établir la conclusion dans un CR (suite à un click droit)
//-------------------------------------------------------------------
void
NSSmallBrother::Conclusion()
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	// Il faut désactiver la sauvegarde automatique en conclusion, sinon
	// on écrase toutes les données sauvegardées
	int iAncienSeuilSauve = _iSeuilSauve ;
	_iSeuilSauve = -1 ;

	NSPatPathoArray TmpPPT(pSuper) ;

	NSPatPathoArray* pPPT = getPatPatho() ;
	NSPatPathoArray pPPTgag(pSuper) ;
    							  //pBBitem : lanceur du module (GECHY par exemple)
	BBItem* pBBItemPere    = (BBItem*) 0 ;  //BBitem père (conclusion automatique : 0CONC1 le père)
	BBItem* pBBItemLanceur = (BBItem*) 0 ;  //BBitem lanceur de la conclusion automatique
    							  //on a l'arborescence suivante
    							  //	pBBItem
                                  //       		pBBItemPere
                                  //                   pBBItemLanceur

    //
    //savoir si on peut avoir une conclusion automatique : présence dans le guide
    //de 0CONC ->faire une recherche sémantique.
    //Si ce n'est pas le cas, une conclusion manuelle est proposée et elle écrase
    //l'ancien texte libre.
    //Si c'est le cas, la boîte de dialogue de conclusion est activée.
    //
	string sCodeSens;
	string sSens;
	PatPathoIter iterModule = pPPT->begin() ;
	string sLanceurModule = (*iterModule)->getLexique() ; //GECHY par exemple
	NSDico::donneCodeSens(&sLanceurModule, &sSens) ;

	bool bTexteLibre = false ;
	// bool bConclusionAutomatique = false;

	//
	// Mémoriser la patpatho actuelle
	//
	NSPatPathoArray PatpathoActuelle = *pPPT ;
	pPPT->vider() ;
	PatPathoIter iterConclusion = PatpathoActuelle.ChercherItem("0CONC1") ;

	NSPatPathoArray* pPPTEnCours ;

	if (false == RechercheSemantique("0CONC", sSens)) //boîte de texte libre
	{
		_sLexiqueModule = string("#####1") ;
		_contexteModule = ' ' ;
		bTexteLibre = true ;
		//on a l'arborescence suivante
		//			pBBItem = "#####1"
		//       		pBBItemPere = "#####1"
		//                   pBBItemLanceur = "#TLI#1"

		if (false == ouvreScript())
		{
			_iSeuilSauve = iAncienSeuilSauve ;
			return ;
		}
		//
		// Création du BBItem ammorce (le père) pBBItemPere =  "#####1" le père non lanceur de Boîte de dialogue
		//														est dont le seul fils est "#####1"
		//
    if (false == CreateBBItemFromFiche())
      return ;
		NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;
		_pBBItem->_sLocalisation = sCodeSens ;
		_pBBItem->creer(true) ;

    //
    // pBBItemLanceur = "#####1"  père de "#TLI#1"
    //
    BBFilsItem* pFilsBBItem = *(_pBBItem->_aBBItemFils.begin()) ;
    //
    //si le premier élément de de la patpatho de pFilsBBItem est vide, il faut le virer
    //
    NSPatPathoArray* pPPTEnCours = (NSPatPathoArray*) 0 ; //patpatho de pBBItemLanceur
    if (PatpathoActuelle.end() != iterConclusion)
    {
    	//
      //
      //	pPPTEnCours est la patpatho de pBBItemLanceur , elle est du type
      // 		0CONC1
      //				#TLI#1
      //				  	[ce bout de paptho sera affiché dans le texte libre]
      //
      //
      NSVectFatheredPatPathoArray* pNSVectPatPathoArray = pFilsBBItem->getPatPatho() ;
      pNSVectPatPathoArray->vider();
      pPPTEnCours = pFilsBBItem->getPatpathoActive(&PatpathoActuelle,
 								      				                      iterConclusion) ;
      pBBItemLanceur = new BBItem(pContexte, pFilsBBItem, this, pPPTEnCours /*, &TmpPPT*/) ;
    }
    else
    	pBBItemLanceur = new BBItem(pContexte, pFilsBBItem, this, pPPT /*, &TmpPPT*/) ;

    pBBItemLanceur->setItemData(pFilsBBItem->getItemData()) ;
		pBBItemLanceur->setDialogName(pBBItemLanceur->getItemDialogName()) ;
    string sFilsLabel = pFilsBBItem->getItemLabel() ;
    NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
    pBBItemLanceur->_sLocalisation = _pBBItem->_sLocalisation ;
    pBBItemLanceur->creer(false) ;
    //
    // le paramètre true empêche l'appel à DispatcherPatPatho
    // dans creer() (nsbbitem.cpp) car on appelle la même
    // fonction dans nstrnode.cpp
    //

    // Even when no automatic conclusion exists, we still need the position of
    // Conclusion as a chapter
    //
    if (string("") == _sPositionConclusion)
      _sPositionConclusion = pBBItemLanceur->getItemSonsList() ;
  }
  else //conclusion automatique
  {
		//
		// Création du BBItem ammorce (exemple GECHY en écho)
		//
    _sLexiqueModule = sLanceurModule ;
    _contexteModule = ' ' ;
    if (false == ouvreScript())
    {
    	_iSeuilSauve = iAncienSeuilSauve ;
			return ;
    }

    if (false == CreateBBItemFromFiche())
      return ;

    _pBBItem->_sLocalisation = sSens ;
		_pBBItem->creer(true) ;

    if (string("") == _sPositionConclusion)
      _sPositionConclusion = _pBBItem->getItemSonsList() ;

    //0CONC le père
		pBBItemPere = _pBBItem->CreerBBitem(this, string("0CONC1"), pPPT, sSens) ;
    if ((BBItem*) NULL == pBBItemPere)
    {
    	delete _pBBItem ;
      _pBBItem = (BBItem *) 0 ;
      _iSeuilSauve = iAncienSeuilSauve ;
      return ;
    }
    //0CONC le fils
    BBFilsItem* pFilsBBItem = *(pBBItemPere->_aBBItemFils.begin()) ;
    pPPTEnCours = (NSPatPathoArray*) 0 ; //patpatho de pBBItemLanceur
    bool bNePasDispatcher;
    if (PatpathoActuelle.end() != iterConclusion)
    {
    	NSVectFatheredPatPathoArray* pNSVectPatPathoArray = pFilsBBItem->getPatPatho() ;
      pNSVectPatPathoArray->vider() ;
      //
      // Attention, si OCONC1 n'a pas de fils, pPPTEnCours = 0
      //
      pPPTEnCours = pFilsBBItem->getPatpathoActive(&PatpathoActuelle,
 								      				                            iterConclusion) ;

      if (pPPTEnCours)
      {
      	bNePasDispatcher = false;
        pBBItemLanceur = new BBItem(pContexte, pFilsBBItem, this, pPPTEnCours /*, &TmpPPT*/);
      }
      else
      {
      	bNePasDispatcher = true;
        pBBItemLanceur = new BBItem(pContexte, pFilsBBItem, this, pPPT /*, &TmpPPT*/);
      }
    }
    else
    {
    	bNePasDispatcher = true;
      pBBItemLanceur = new BBItem(pContexte, pFilsBBItem, this, pPPT /*, &TmpPPT*/);
    }

    //
    // pPPTEnCours sera dispatchée dans les boîtes de dialogue
    // de pBBItemLanceur et de ses fils
    //

    pBBItemLanceur->_sLocalisation = pBBItemPere->_sLocalisation ;
    pBBItemLanceur->setItemData(pFilsBBItem->getItemData()) ;
    pBBItemLanceur->setDialogName(pBBItemLanceur->getItemDialogName()) ;
    pBBItemLanceur->creer(bNePasDispatcher) ;
    // bConclusionAutomatique = true;
  }

  pBBItemLanceur->activer() ;   	//lancer la boîte de texte libre ou
                             		//la boîte de la conclusion automatique
  //texte libre
  if (/*(pBBItemLanceur->_bConclusion) &&*/ bTexteLibre)
  {
  	//
    //effacer l'ancien texte libre
    //
    PatPathoIter iterConclusion = PatpathoActuelle.ChercherItem(string("0CONC1")) ;
    if (PatpathoActuelle.end() != iterConclusion)
    	PatpathoActuelle.SupprimerItem(iterConclusion) ;

    //
    //mettre à jour les localisations des éléments du texte libre
    //mettre "0CONC1" au début de pBBItemLanceur->pPPTEnCours
    //
    NSPatPathoArray* pStarterPPT = pBBItemLanceur->getPPTEncours() ;

    if (pStarterPPT && (false == pStarterPPT->empty()))
    {
    	PatPathoIter iterPremier = pStarterPPT->begin() ;
      PatPathoIter jter        = pStarterPPT->begin() ;
      jter = iterPremier;
      for ( ; pStarterPPT->end() != jter ; jter++)
      {      	(*jter)->setLigne((*jter)->getLigne() + 1) ;
        (*jter)->setColonne((*jter)->getColonne() + 1) ;
      }
      pStarterPPT->ajoutePatho(iterPremier, string("0CONC1"), 0 , -1) ;
      //
      //concaténer les 2 patpathos
      //
      ConcatenerPatpatho(string("0CONC1"), pStarterPPT, &PatpathoActuelle) ;
    }
  }
  else //conclusion automatique
  {
  	PatPathoIter iterConclusion = PatpathoActuelle.ChercherItem(string("0CONC1")) ;
    if (PatpathoActuelle.end() == iterConclusion)
    {
      NSPatPathoArray* pStarterPPT = pBBItemLanceur->getPPTEncours() ;

    	//une conclusion automatique vient d'être faite
      if (pStarterPPT && (false == pStarterPPT->empty()))
      {
      	//
        // mettre à jour les localisations des éléments du texte libre
        // mettre "0CONC1" au début de pBBItemLanceur->pPPTEnCours
        //
        PatPathoIter iterPremier = pStarterPPT->begin() ;
        PatPathoIter jter        = pStarterPPT->begin() ;
        jter = iterPremier ;
        for ( ; pStarterPPT->end() != jter ; jter++)
        {
        	(*jter)->setLigne((*jter)->getLigne() + 1) ;
          (*jter)->setColonne((*jter)->getColonne() + 1) ;
        }
        pStarterPPT->ajoutePatho(iterPremier, string("0CONC1"), 0 , -1) ;

        //
        // Concaténer les 2 patpathos
        //
        ConcatenerPatpatho(string("0CONC1"), pStarterPPT, &PatpathoActuelle) ;
      }
    }
    else
    {
    	//
      // si on a désactivé tout ce qui est cl automatique, enlever 0CONC1 de
      //la patpatho globale : pas de cl automatqiue
      //

      //
      // effacer l'ancienne conclusion automatqiue
      //
      if (PatpathoActuelle.end() != iterConclusion)
        PatpathoActuelle.SupprimerItem(iterConclusion) ;

      NSPatPathoArray* pStarterPPT = pBBItemLanceur->getPPTEncours() ;
      if (pStarterPPT && (false == pStarterPPT->empty()))
      {
        for (PatPathoIter jter = pStarterPPT->begin() ; pStarterPPT->end() != jter ; jter++)
        {
          (*jter)->setLigne((*jter)->getLigne() + 1) ;
          (*jter)->setColonne((*jter)->getColonne() + 1) ;
        }
        pStarterPPT->ajoutePatho(pStarterPPT->begin(), string("0CONC1"), 0 , -1) ;
        //
        // concaténer les 2 patpathos
        //
        ConcatenerPatpatho(string("0CONC1"), pStarterPPT, &PatpathoActuelle) ;
      }
    }
  }

  *pPPT = PatpathoActuelle ;
  _iSeuilSauve = iAncienSeuilSauve ;
    /*if (iSeuilSauve > 0)
    {
    	pBBItem->pPPTEnCours = pPPT;
    	sauvegarde();
    } */

  if (_pBBItem)
  {
    delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;
  }
    // -------------------- pBBItemPere est inséré dans le VectorFils
    // -------------------- de son BBItemFils par la méthode CreerBBitem
    // -------------------- il est donc détruit automatiquement
    // -------------------- ce serait peut-être bien de faire de même avec
    // -------------------- pBBItemLanceur
    // if (pBBItemPere)
    //	delete pBBItemPere;
  if (pBBItemLanceur)
    delete pBBItemLanceur ;

  PatPathoIter iterPremier = pPPT->begin() ;
  _sLexiqueModule = (*iterPremier)->getLexique() ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::Conclusion.", standardError, 0) ;
}
}

//-------------------------------------------------------------------
// sert pour lancer un texte libre en édition
//-------------------------------------------------------------------
void
NSSmallBrother::EditionTexteLibre()
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	// Il faut désactiver la sauvegarde automatique en conclusion, sinon
	// on écrase toutes les données sauvegardées
	int iAncienSeuilSauve = _iSeuilSauve ;
	_iSeuilSauve = -1 ;

	NSPatPathoArray TmpPPT(pSuper) ;
	NSPatPathoArray* pPPT = getPatPatho() ;

	string sCodeSens;
	string sSens;

	_sLexiqueModule = string("#####1") ;
	_contexteModule = ' ' ;
	// bool bTexteLibre = true;
	//on a l'arborescence suivante
	//			pBBItem = "#####1"
	//       		pBBItemPere = "#####1"
	//                   pBBItemLanceur = "#TLI#1"

	if (!pSuper->getFilGuide()->Prend())
		return ;

	if (!pContexte->getDico()->Prend())
		return ;

	_pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;

	// Positionnement sur la fiche d'ammorce
	if (false == ouvreScript())
	{
		_iSeuilSauve = iAncienSeuilSauve ;
		return ;
	}
  //
  // Création du BBItem ammorce (le père) pBBItemPere =  "#####1" le père non lanceur de Boîte de dialogue
  //														est dont le seul fils est "#####1"
  //
  if (false == CreateBBItemFromFiche())
    return ;

  NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;
  _pBBItem->_sLocalisation = sCodeSens ;
  _pBBItem->creer(true) ;

	if (_pBBItem->_aBBItemFils.empty())
  {
  	delete _pBBItem ;
    _pBBItem = (BBItem*) 0 ;
    return ;
  }
  //
  // pBBItemLanceur = "#####1"  père de "#TLI#1"
  //
  BBFilsItem* pFilsBBItem = *(_pBBItem->_aBBItemFils.begin()) ;

  BBItem BBItemLanceur(pContexte, pFilsBBItem, this, pPPT /*, &TmpPPT*/) ;

  BBItemLanceur.setItemData(pFilsBBItem->getItemData()) ;
  BBItemLanceur.setDialogName(BBItemLanceur.getItemDialogName()) ;
  string sFilsLabel = pFilsBBItem->getItemLabel() ;
  NSDico::donneCodeSens(&sFilsLabel, &sCodeSens) ;
  BBItemLanceur._sLocalisation = _pBBItem->_sLocalisation ;
  BBItemLanceur.creer(false) ;
  //
  // le paramètre true empêche l'appel à DispatcherPatPatho
  // dans creer() (nsbbitem.cpp) car on appelle la même
  // fonction dans nstrnode.cpp
  //

  BBItemLanceur.activer() ;   	//lancer la boîte de texte libre

  *pPPT = *(BBItemLanceur.getPPTEncours()) ;
  _iSeuilSauve = iAncienSeuilSauve ;
  /*if (iSeuilSauve > 0)
  {
    	pBBItem->pPPTEnCours = pPPT;
    	sauvegarde();
    } */

  if (_pBBItem)
  {
  	delete _pBBItem ;
    _pBBItem = (BBItem *) 0 ;
  }
}
catch (...)
{
	erreur("Exception NSSmallBrother::EditionTexteLibre.", standardError, 0) ;
}
}

/*** Choix du code (ex CCAM) dans un CR - Choice of exam classification code
*/
void
NSSmallBrother::Codage()
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur();

	// Il faut désactiver la sauvegarde automatique en conclusion, sinon
	// on écrase toutes les données sauvegardées
	int iAncienSeuilSauve = _iSeuilSauve ;
	_iSeuilSauve = -1 ;

	NSPatPathoArray* pPPT = getPatPatho() ;
	NSPatPathoArray  pPPTgag(pSuper) ;

	NSPatPathoArray  TmpPPT(pSuper) ;

	//pBBitem : lanceur du module (GECHY par exemple)
	BBItem* pBBItemPere    = 0 ;    //BBitem père (conclusion automatique : 0CONC1 le père)
	// BBItem* pBBItemLanceur = 0 ;    //BBitem lanceur de la conclusion automatique
    							    //on a l'arborescence suivante
    							    //	pBBItem
                      //       		pBBItemPere
                      //                   pBBItemLanceur

	//
	//savoir si on peut avoir une conclusion automatique : présence dans le guide
	//de 0CONC ->faire une recherche sémantique.
	//Si ce n'est pas le cas, une conclusion manuelle est proposée et elle écrase
	//l'ancien texte libre.
	//Si c'est le cas, la boîte de dialogue de conclusion est activée.
	//
	string sCodeSens ;
	string sSens ;
	PatPathoIter iterModule = pPPT->begin() ;
  //
	// Exam's root concept
	//
	string sLanceurModule = (*iterModule)->getLexique() ; //GECHY par exemple
	NSDico::donneCodeSens(&sLanceurModule, &sSens) ;

	// bool bTexteLibre = false;

	//
	// Mémoriser la patpatho actuelle
	//
	NSPatPathoArray PatpathoActuelle = *pPPT ;
	NSPatPathoArray* pPatpathoActuelle = &PatpathoActuelle ;
	pPPT->vider() ;
	PatPathoIter iterConclusion = pPatpathoActuelle->ChercherItem("0CODE1") ;

	NSPatPathoArray PPTEnCours(pSuper) ;

/*
	if (!RechercheSemantique("0CODE", sSens)) //boîte de texte libre
	{
		strcpy(lexiqueModule, "#####1") ;
		contexteModule = ' ' ;
		bTexteLibre = true ;
		//on a l'arborescence suivante
		//			pBBItem = "#####1"
		//       		pBBItemPere = "#####1"
		//                   pBBItemLanceur = "#TLI#1"

		if (!ouvreScript())
		{
			iSeuilSauve = iAncienSeuilSauve ;
			delete pTmpPPT ;
			return ;
		}
		//
		// Création du BBItem ammorce (le père) pBBItemPere =  "#####1" le père non lanceur de Boîte de dialogue
		//														est dont le seul fils est "#####1"
		//
		pBBItem = new BBItem(this) ;
		BBFiche* pBBCommeFiche = static_cast<BBFiche*>(pBBFiche) ;
		BBItemData* pDo = static_cast<BBItemData*>(pBBCommeFiche->pDonnees) ;
		*(pBBItem->pDonnees) = *pDo ;
		strcpy(pBBItem->szNomDlg, pDo->nomDialogue) ;
		pSuper->pDico->donneCodeSens(&(string(lexiqueModule)), &sCodeSens) ;
		pBBItem->sLocalisation = sCodeSens ;
		pBBItem->creer(true) ;

		//
		// pBBItemLanceur = "#####1"  père de "#TLI#1"
		//
		BBFilsItem* pFilsBBItem = *(pBBItem->aBBItemFils.begin()) ;
		//
		// si le premier élément de de la patpatho de pFilsBBItem est vide, il faut le virer
		//
		NSPatPathoArray* pPPTEnCours = 0 ; //patpatho de pBBItemLanceur
		if (iterConclusion != pPatpathoActuelle->end())
		{
			//
			//
			//	pPPTEnCours est la patpatho de pBBItemLanceur , elle est du type
			// 		0CONC1
			//				#TLI#1
			//				  	[ce bout de paptho sera affiché dans le texte libre]
			//
			//
			NSVectPatPathoArray* pNSVectPatPathoArray = pFilsBBItem->getPatPatho() ;
			pNSVectPatPathoArray->vider() ;
			pPPTEnCours = pFilsBBItem->getPatpathoActive(pPatpathoActuelle,
 								      				                      iterConclusion) ;
			pBBItemLanceur = new BBItem(pFilsBBItem, this, pPPTEnCours, pTmpPPT) ;
		}
		else
			pBBItemLanceur = new BBItem(pFilsBBItem, this, pPPT, pTmpPPT) ;

		*(pBBItemLanceur->pDonnees) = *(pFilsBBItem->pDonnees);
		strcpy(pBBItemLanceur->szNomDlg, pBBItemLanceur->pDonnees->nomDialogue);
		pSuper->pDico->donneCodeSens(&pFilsBBItem->sEtiquette, &sCodeSens);
		pBBItemLanceur->sLocalisation = pBBItem->sLocalisation;
		pBBItemLanceur->creer(false);
		//
		// le paramètre true empêche l'appel à DispatcherPatPatho
		// dans creer() (nsbbitem.cpp) car on appelle la même
		// fonction dans nstrnode.cpp
		//
	}
	else //conclusion automatique
	{ */
		//
		// Création du BBItem ammorce (exemple GECHY en écho)
		//
		_sLexiqueModule = sLanceurModule ;
		_contexteModule = ' ' ;
		if (false == ouvreScript())
		{
			_iSeuilSauve = iAncienSeuilSauve ;
			return ;
		}

    if (false == CreateBBItemFromFiche())
      return ;

		_pBBItem->_sLocalisation = sSens ;
		_pBBItem->creer(true) ;    if (string("") == _sPositionConclusion)		  _sPositionConclusion = _pBBItem->getItemSonsList() ;
		//0CODE le père

		// ***************** New code **************

		if ((pPatpathoActuelle->end() != iterConclusion) && (NULL != iterConclusion))
    	pPatpathoActuelle->ExtrairePatPatho(iterConclusion, &PPTEnCours) ;

    pBBItemPere = _pBBItem->CreerBBitem(this, "0CODE1", &PPTEnCours, sSens) ;

		if ((BBItem*) NULL == pBBItemPere)
		{
			delete _pBBItem ;
			_pBBItem = (BBItem *) 0 ;
			_iSeuilSauve = iAncienSeuilSauve ;

      *pPPT = *pPatpathoActuelle ;

			return ;
		}
    pBBItemPere->activer() ;

		PatPathoIter iterConc = pPatpathoActuelle->ChercherItem("0CODE1") ;
		//
		// No code was chosen
		//
		if (pPatpathoActuelle->end() == iterConc)
		{
      NSPatPathoArray* pFatherPPT = pBBItemPere->getPPTEncours() ;

			//une conclusion automatique vient d'être faite
			if (pFatherPPT && (false == pFatherPPT->empty()))
			{
				//
				// mettre à jour les localisations des éléments du texte libre
				// mettre "0CODE1" au début de pBBItemLanceur->pPPTEnCours
				//
				PatPathoIter iterPremier = pFatherPPT->begin() ;
				PatPathoIter jter        = pFatherPPT->begin() ;
				jter = iterPremier ;
				for ( ; pFatherPPT->end() != jter ; jter++)
				{
					(*jter)->setLigne((*jter)->getLigne() + 1) ;
					(*jter)->setColonne((*jter)->getColonne() + 1) ;
				}
				pFatherPPT->ajoutePatho(iterPremier, "0CODE1", 0 , -1) ;

				//
				// Concaténer les 2 patpathos
				//
				ConcatenerPatpatho("0CODE1", pFatherPPT, pPatpathoActuelle) ;
			}
		}
		else
		{
			//
			// si on a désactivé tout ce qui est cl automatique, enlever 0CONC1 de
			// la patpatho globale : pas de cl automatqiue
			//
			if (pBBItemPere->getPPTEncours()->empty())
			{
				if (pPatpathoActuelle->end() != iterConc)
					pPatpathoActuelle->SupprimerItem(iterConc) ;
			}
			else
			{
				//
				// effacer l'ancienne conclusion automatqiue
				//
				if (pPatpathoActuelle->end() != iterConc)
					pPatpathoActuelle->SupprimerItem(iterConc) ;

        NSPatPathoArray* pFatherPPT = pBBItemPere->getPPTEncours() ;

				PatPathoIter iterPremier = pFatherPPT->begin() ;
				PatPathoIter jter        = pFatherPPT->begin() ;
				jter = iterPremier ;
				for ( ; pFatherPPT->end() != jter ; jter++)
				{
					(*jter)->setLigne((*jter)->getLigne() + 1) ;
					(*jter)->setColonne((*jter)->getColonne() + 1) ;
				}
				pFatherPPT->ajoutePatho(iterPremier, "0CODE1", 0 , -1) ;
				//
				// concaténer les 2 patpathos
				//
				ConcatenerPatpatho("0CODE1", pFatherPPT, pPatpathoActuelle) ;
			}
		}

		*pPPT = *pPatpathoActuelle ;
    _iSeuilSauve = iAncienSeuilSauve ;

    if (_pBBItem)
    {
			delete _pBBItem ;
     	_pBBItem = (BBItem *) 0 ;
    }
    // -------------------- pBBItemPere est inséré dans le VectorFils
    // -------------------- de son BBItemFils par la méthode CreerBBitem
    // -------------------- il est donc détruit automatiquement
    // -------------------- ce serait peut-être bien de faire de même avec
    // -------------------- pBBItemLanceur
    // if (pBBItemPere)
    //	delete pBBItemPere;


    PatPathoIter iterPremier = pPPT->begin() ;
    string sLexiqueModule = (*iterPremier)->getLexique() ;
    _sLexiqueModule = sLexiqueModule ;

/*
		pBBItemPere = pBBItem->CreerBBitem(this, "0CODE1", pPPT, sSens, pBBItemPere);
		if (!pBBItemPere)
		{
			delete pBBItem ;
			pBBItem = 0 ;
			iSeuilSauve = iAncienSeuilSauve ;
			return ;
		}
		//0CONC le fils
		BBFilsItem* pFilsBBItem = *(pBBItemPere->aBBItemFils.begin());
		pPPTEnCours = 0; //patpatho de pBBItemLanceur
		bool bNePasDispatcher;
		if ((iterConclusion != pPatpathoActuelle->end()) && (iterConclusion != NULL))
		{
			NSVectPatPathoArray* pNSVectPatPathoArray = pFilsBBItem->getPatPatho();
			pNSVectPatPathoArray->vider();
			//
			// Attention, si OCODE1 n'a pas de fils, pPPTEnCours = 0
			//
			pPPTEnCours = pFilsBBItem->getPatpathoActive(pPatpathoActuelle,
 								      				                      iterConclusion);

			if (pPPTEnCours)
			{
				bNePasDispatcher = false;
				pBBItemLanceur = new BBItem(pFilsBBItem, this, pPPTEnCours, pTmpPPT);
			}
			else
			{
				bNePasDispatcher = true;
				pBBItemLanceur = new BBItem(pFilsBBItem, this, pPPT, pTmpPPT);
			}
		}
		else
		{
			bNePasDispatcher = true;
			pBBItemLanceur = new BBItem(pFilsBBItem, this, pPPT, pTmpPPT);
		}

		//
		// pPPTEnCours sera dispatchée dans les boîtes de dialogue
		// de pBBItemLanceur et de ses fils
		//
		pBBItemLanceur->sLocalisation = pBBItemPere->sLocalisation;
		*(pBBItemLanceur->pDonnees) = *(pFilsBBItem->pDonnees);
		strcpy(pBBItemLanceur->szNomDlg, pBBItemLanceur->pDonnees->nomDialogue);
		pBBItemLanceur->creer(bNePasDispatcher);
        // bConclusionAutomatique = true;
//    }

    pBBItemLanceur->activer();   	//lancer la boîte de texte libre ou
                             		//la boîte de la conclusion automatique

    //texte libre
    if ((pBBItemLanceur->bConclusion) && (bTexteLibre))
    {
        //
        //effacer l'ancien texte libre
        //
  		PatPathoIter iterConclusion = pPatpathoActuelle->ChercherItem("0CODE1");
        if (iterConclusion != pPatpathoActuelle->end())
            pPatpathoActuelle->SupprimerItem(iterConclusion);

        //
        //mettre à jour les localisations des éléments du texte libre
        //mettre "0CONC1" au début de pBBItemLanceur->pPPTEnCours
        //
        if (!pBBItemLanceur->pPPTEnCours->empty())
        {
      	    PatPathoIter iterPremier = pBBItemLanceur->pPPTEnCours->begin();
            PatPathoIter jter        = pBBItemLanceur->pPPTEnCours->begin();
            jter = iterPremier;
            for ( ; jter != pBBItemLanceur->pPPTEnCours->end(); jter++)
            {
         	    (*jter)->pDonnees->setLigne((*jter)->pDonnees->getLigne() + 1);
                (*jter)->pDonnees->setColonne((*jter)->pDonnees->getColonne() + 1);
            }
            pBBItemLanceur->pPPTEnCours->ajoutePatho(iterPremier, "0CODE1", 0 , -1);
            //
      	    //concaténer les 2 patpathos
      	    //
      	    ConcatenerPatpatho("0CONC1", pBBItemLanceur->pPPTEnCours, pPatpathoActuelle);
        }
    }
    else //conclusion automatique
    {
   	    PatPathoIter iterConclusion = pPatpathoActuelle->ChercherItem("0CODE1");
        if (iterConclusion == pPatpathoActuelle->end())
        {
            //une conclusion automatique vient d'être faite
            if (!pBBItemLanceur->pPPTEnCours->empty())
            {
         	    //
      		    // mettre à jour les localisations des éléments du texte libre
      		    // mettre "0CONC1" au début de pBBItemLanceur->pPPTEnCours
      		    //
	      	    PatPathoIter iterPremier = pBBItemLanceur->pPPTEnCours->begin();
   	            PatPathoIter jter = pBBItemLanceur->pPPTEnCours->begin();
      	        jter = iterPremier;
         	    for ( ; jter != pBBItemLanceur->pPPTEnCours->end(); jter++)
                {
            	    (*jter)->pDonnees->setLigne((*jter)->pDonnees->getLigne() + 1);
                    (*jter)->pDonnees->setColonne((*jter)->pDonnees->getColonne() + 1);
                }
         	    pBBItemLanceur->pPPTEnCours->ajoutePatho(iterPremier, "0CODE1", 0 , -1);

      		    //
      		    // Concaténer les 2 patpathos
      		    //
      		    ConcatenerPatpatho("0CODE1", pBBItemLanceur->pPPTEnCours, pPatpathoActuelle);
            }
        }
        else
        {
      	    //
      	    // si on a désactivé tout ce qui est cl automatique, enlever 0CONC1 de
      	    //la patpatho globale : pas de cl automatqiue
      	    //
      	    if (pBBItemLanceur->pPPTEnCours->empty())
      	    {
      		    if (iterConclusion != pPatpathoActuelle->end())
      			    pPatpathoActuelle->SupprimerItem(iterConclusion);
      	    }
      	    else
      	    {
      		    //
      		    //effacer l'ancienne conclusion automatqiue
      		    //
      		    if (iterConclusion != pPatpathoActuelle->end())
      			    pPatpathoActuelle->SupprimerItem(iterConclusion);

                PatPathoIter iterPremier = pBBItemLanceur->pPPTEnCours->begin();
                PatPathoIter jter = pBBItemLanceur->pPPTEnCours->begin();
                jter = iterPremier;
                for ( ; jter != pBBItemLanceur->pPPTEnCours->end(); jter++)
                {
            	    (*jter)->pDonnees->setLigne((*jter)->pDonnees->getLigne() + 1);
                    (*jter)->pDonnees->setColonne((*jter)->pDonnees->getColonne() + 1);
                }
                pBBItemLanceur->pPPTEnCours->ajoutePatho(iterPremier, "0CODE1", 0 , -1);
      		    //
      		    //concaténer les 2 patpathos
      		    //
      		    ConcatenerPatpatho("0CODE1", pBBItemLanceur->pPPTEnCours, pPatpathoActuelle);
      	    }
        }
    }

    *pPPT = *pPatpathoActuelle;
    iSeuilSauve = iAncienSeuilSauve;

    if (pBBItem)
    {
        delete pBBItem;
     	pBBItem = 0;
    }
    // -------------------- pBBItemPere est inséré dans le VectorFils
    // -------------------- de son BBItemFils par la méthode CreerBBitem
    // -------------------- il est donc détruit automatiquement
    // -------------------- ce serait peut-être bien de faire de même avec
    // -------------------- pBBItemLanceur
    // if (pBBItemPere)
    //	delete pBBItemPere;
    if (pBBItemLanceur)
    	delete pBBItemLanceur;

    delete pTmpPPT;

    PatPathoIter iterPremier = pPPT->begin();
    string sLexiqueModule = string((*iterPremier)->pDonnees->lexique);
    strcpy(lexiqueModule, sLexiqueModule.c_str());
*/
    return;
}
catch (...)
{
    erreur("Exception NSSmallBrother::Codage.", standardError, 0) ;
}
}


//------------------------------------------------------------------------// sTypeDocument : type de document : exemple consultation, antécédent,...
// lancement de document type consultation
//	pNSTreeWindow : pointeur sur la TreeView qui supportera la vue
//------------------------------------------------------------------------
void
NSSmallBrother::lanceConsult(string sTypeDocument, NSTreeWindow* pNSTreeWindow)
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur() ;

	NSPatPathoArray* pPPT = getPatPatho() ;

	_contexteModule = ' ' ;
	string sLexique ;
	string sCodeSens ;
	if (false == ouvreResModule())
		return ;
	if (false == pSuper->getFilGuide()->Prend())
		return ;
	if (false == pContexte->getDico()->Prend())
		return ;
	//
	// Instancie la fiche guide
	//
	_pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;

	//
  // Création d'un nouveau document
  //
  if (sTypeDocument != "") //ok sur le choix du type
  {
  	pPPT->vider() ;
    sLexique = sTypeDocument ;
    _sLexiqueModule = sLexique ; //choix de l'utilisateur

    // Reprise d'un éventuel fichier temporaire
    if (_pPatPathoArray->empty())
    	reprise() ;
  }
  //
  // Reprise d'un document existant
  //
  else
  {
  	//
    // Si la patpatho n'est pas vide, on initialise noeud et lexique
    // à partir de sa racine
    //
    if (false == pPPT->empty())
    {
    	// strcpy(noeudModule, (*(pPPT->begin()))->pDonnees->noeud);

      sLexique = (*(pPPT->begin()))->getLexique() ;
      EnleverTitre() ;
      _sLexiqueModule = sLexique ; //1er élément de la patpatho
    }
    else
    	return ;
	}

  NSDico::donneCodeSens(&sLexique, &sCodeSens) ;
  _pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;
  ouvreScript() ;  //recherche dans GUIDES.DB

  BBFiche* pBBCommeFiche = static_cast<BBFiche*>(_pBBFiche) ;
  BBItemData* pDo = pBBCommeFiche->getData() ;

  string sFils = string(pDo->fils) ;
  //
  // Tester si la fiche récuperée dans le guide correspond au lanceur
  // de la consultation
  //
  if ((_pBBFiche->getSemanticPath() != sCodeSens) /*pas de fiche dans le guide*/
                    ||
      (sFils.find(sLexique) == NPOS) /* l'élément lanceur ne figure pas en tant que son
                                         propre fils dans le guide, exemple GECHY */)
  {
    //mauvaise fiche par la recherche exacte : faire une recherche sémantique
    VecteurRechercheSelonCritere VecteurSelonCritere(GUIDE) ;
    string sLocalisation;

    sLocalisation = sCodeSens;
    bool trouve;
    VecteurSelonCritere.AjouteEtiquette(sCodeSens);
    pSuper->getFilGuide()->chercheChemin(&sLocalisation ,
                         &VecteurSelonCritere, NSFilGuide :: compReseau);

    VecteurSelonCritere.SetData(sCodeSens, &trouve, _pBBFiche->getData()) ;
    if ((false == trouve) || (sFils.find(sLexique) == NPOS)) /* l'élément lanceur ne figure pas en tant que son
                                       propre fils dans le guide, exemple GECHY */
    // La recherche sémantique à échoué
    {
      //
      // Instancier "à la main" les données
      // Exemple si on choisit antécédents (ANTEC1) et qu'on ne trouve pas
      // dans le guide les données correspondants à ce code, on crée
      // la fiche suivante :  chemin  	    : ANTEC
      //						fils	  		: ANTEC1
      //                      nomDialogue 	: TEXTELIB
      //                      ouvreDialogue   : O
      //                      decalageNiveau	: +00+00
      //                      fichierDialogue : NSBB
      //
      _pBBFiche->getData()->metAZero() ;
      strcpy(pDo->chemin, sCodeSens.c_str()) ;
      strcpy(pDo->fils, sLexique.c_str()) ;
      strcpy(pDo->nomDialogue, "TEXTELIB") ;
      strcpy(pDo->ouvreDialogue, "O") ;
      strcpy(pDo->decalageNiveau, "+00+00") ;
      strcpy(pDo->fichierDialogue, "NSBB") ;
    }
  }
  //
	// Création du BBItem ammorce (le père)
	//
  if (false == CreateBBItemFromFiche())
    return ;

  _pBBItem->_sLocalisation = sCodeSens ;
	_pBBItem->creer(true) ;   // Le paramètre true empêche l'appel à
                            // DispatcherPatPatho dans creer() (nsbbitem.cpp)
                            // car on appelle la même fonction dans nstrnode.cpp
  //
  // Ajout spécifique
  //
  pNSTreeWindow->pBBitemNSTreeWindow = _pBBItem ;

  NSControle* pControl = new NSControle(pContexte, _pBBItem, sLexique, string("")) ; //1er élément de la patpatho

  pControl->setControle(static_cast<void*>(pNSTreeWindow)) ;
  pControl->setType(pNSTreeWindow->donneType()) ;
  pControl->setNSDialog(0) ;

  pNSTreeWindow->setControl(pControl) ;

  //
  // Créer le deuxième BBItem (GCONS le fils en cas de consultation par exemple)
  //
  BBFilsItem* pFilsItem = *(_pBBItem->_aBBItemFils.begin()) ;
  int iCreer = _pBBItem->developperConsultation(pFilsItem) ;
  if (0 != iCreer)
  	return ;

  BBFilsItem* fils = *(pNSTreeWindow->pBBitemNSTreeWindow->_aBBItemFils.begin()) ;
    //   NSCSDocument* CSDoc = dynamic_cast<NSCSDocument*>(pDoc);
    //   *(*(fils->getPatPatho()->begin())) = *(CSDoc->pPatPathoArray);
  NSPatPathoArray* pPatPatho = (*(fils->getPatPatho()->begin()))->getPatPatho() ;
	*pPatPatho = *pPPT ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::lanceConsult.", standardError, 0) ;
}
}

void
NSSmallBrother::lance12(string sNoeud1, string sNoeud2)
{
try
{
	//
	// Création du BBItem ammorce (le père)
	//
	_pBBItem = new BBItem(pContexte, this) ;

	BBItemData* pDo = new BBItemData() ;
	strcpy(pDo->ouvreDialogue, "N") ;
	strcpy(pDo->fils, sNoeud2.c_str()) ;
	strcpy(pDo->decalageNiveau, "+01+01") ;
	strcpy(pDo->fichierDialogue, "NSBB") ;

	_pBBItem->setItemData(pDo) ;
	_pBBItem->setDialogName(string("")) ;

	string sCodeSens ;
	NSDico::donneCodeSens(&sNoeud1, &sCodeSens) ;
	_pBBItem->_sLocalisation = sCodeSens ;
	_pBBItem->creer(false) ;

	//
	// Créer le deuxième BBItem
	//
	BBFilsItem* pFilsItem = *(_pBBItem->_aBBItemFils.begin()) ;
	int creer = _pBBItem->developperConsultation(pFilsItem) ;
	// Si result dialogue OK : 0, si CANCEL : -1
	if (0 != creer)
	{
		delete _pBBItem ;
		_pBBItem = (BBItem*) 0 ;
		return ;
	}

	_pBBItem->okFermerDialogue(true) ;

  if (_pPatPathoArray)
	  *_pPatPathoArray = *(_pBBItem->getPPTEncours()) ;

	delete _pBBItem ;
	_pBBItem = (BBItem *) 0 ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::lanceConsult.", standardError, 0) ;
}
}

/**
* Ouverture du fichier des fils guides et positionnement sur la fiche d'ammorce
* Open Fils guides repository and set the cursor at this module's root
*/
bool
NSSmallBrother::ouvreScript()
{
try
{
	NSSuper* pSuper = pContexte->getSuperviseur();
	//
	// Recherche de l'ammorce
	//
	string sCodeSens;
	NSDico::donneCodeSens(&_sLexiqueModule, &sCodeSens) ;

  if (string("") == sCodeSens)
  	return false ;

  if (NULL == _pBBFiche)
  	_pBBFiche = static_cast<BBFiche*>(pSuper->getFilGuide()->getBBFiche()) ;

	_pBBFiche->lastError = _pBBFiche->chercheClef(&sCodeSens,
                                                "CHEMIN",
                                                keySEARCHGEQ,
                                                dbiWRITELOCK) ;
	if (DBIERR_NONE != _pBBFiche->lastError)
	{
		erreur("Le fichier de script n'est pas ammorçable (guides.db).", standardError, _pBBFiche->lastError) ;
		return false ;
	}
	//
	// On récupère l'enregistrement
	//
	_pBBFiche->lastError = _pBBFiche->getRecord() ;
	if (DBIERR_NONE != _pBBFiche->lastError)
	{
		erreur("Le fichier de script est inutilisable.", standardError, _pBBFiche->lastError) ;
		return false ;
	}
	return true ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::ouvreScript.", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------------//		Méthode qui retourne un pointeur sur le module(pTModule)
//		de l'élément si libelle existe
//---------------------------------------------------------------------------
TModule*
NSSmallBrother::TrouverModule(string sLib, bool bCreateIfMissing)
{
  if (string("") == sLib)
    return (TModule*) 0 ;

  string sLibMaj = pseumaj(sLib) ;

  if (false == _aResModuleArray.empty())
  {
    // Looking for lib in pResModuleArraypour
    //
    for (iterNSTModuleArray i = _aResModuleArray.begin() ;
                                          _aResModuleArray.end() != i ; i++)
    {
      string sMajModuleName = pseumaj(i->getLabel()) ;

      if (sMajModuleName == sLib)
        return i->getModule() ;
    }
  }

  // Not found ; try to create
  //
  if ((false == bCreateIfMissing) || (false == CreerNouveauModule(sLib)))
    return (TModule*) 0 ;

  return TrouverModule(sLib, false) ;
}

//---------------------------------------------------------------------------//  	Méthode de param = libellé , si libelle existe ne fait rien, sinon créee
// 	un nouveau TModule et une nouvelle fiche NSModuleDialogue qu'il ajoute au vecteur.
//---------------------------------------------------------------------------
bool
NSSmallBrother::CreerNouveauModule(string sLib)
{
	if (string("") == sLib)
		return false ;

try
{
  string sMajLabel = pseumaj(sLib) ;

	// If lib already exists, there is nothing to do
  //
  if (false == _aResModuleArray.empty())
  	for (iterNSTModuleArray i = _aResModuleArray.begin() ; _aResModuleArray.end() != i ; i++)
    	if (i->getLabel() == sMajLabel)
      	return true ;

  // lib n'existe pas : on créee alors un nouveau module et un nouveau
  // NSModuleDialogue que l'on rajoute au vecteur pResModuleArray.
  //
  TModule* pNewTModule = (TModule*) 0 ;

  if ((string("NSBB") == sMajLabel) || (string("NSMBB") == sMajLabel))
  	pNewTModule = pNSDLLModule ;
  else
  {
  	string sNomDLL = sMajLabel + string(".DLL") ;
		pNewTModule = new TModule(sNomDLL.c_str(), true) ;
		if ((TModule*) NULL == pNewTModule)
		{
    	string sErreur = string("La DLL spécifique (") + sNomDLL + string(") est introuvable.") ;
			erreur(sErreur.c_str(), standardError, -1) ;
			return false ;
		}
  }

  NSModuleDialogue NewNSModuleDialogue ;
  NewNSModuleDialogue.setModule(pNewTModule) ;
  NewNSModuleDialogue.setLabel(sMajLabel) ;

  _aResModuleArray.push_back(NewNSModuleDialogue) ;

  return true ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::CreerNouveauModule.", standardError, 0) ;
	return false ;
}
}

//---------------------------------------------------------------------
// MAJ de VectorCalcul
//---------------------------------------------------------------------
void
NSSmallBrother::AjouteVecteur(string sLocalisation, string sContenu)
{
try
{
	_VectorCalcul.push_back(classCalcul(sLocalisation, sContenu)) ;
}
catch (...)
{
  erreur("Exception NSSmallBrother::AjouteVecteur.", standardError, 0) ;
}
}

voidNSSmallBrother::RemplirVecteur(string sLocalisation, string sContenu)
{
  if (false == _VectorCalcul.empty())
  {
    iterStringCalcul iter = _VectorCalcul.begin() ;
    for (; _VectorCalcul.end() != iter ; iter++)
    {
      if (iter->sLocalisation == sLocalisation)
      {
        iter->sContenu = sContenu ;
        return ;
      }
    }
  }

  AjouteVecteur(sLocalisation, sContenu) ;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
string
NSSmallBrother::TrouverElement(string sLocalisation)
{
  if (_VectorCalcul.empty())
    return string("") ;

  iterStringCalcul iter = _VectorCalcul.begin() ;
  for ( ; _VectorCalcul.end() != iter ; iter++)
  {
    if (iter->sLocalisation == sLocalisation)
      return iter->sContenu ;
  }

	return string("") ;
}

//--------------------------------------------------------------------------------
// Prend le message en bas de pile (et l'efface)
//
// Il est possible de passer un pointeur pour récupérer le BBCmdMessage
// ou de ne rien passer pour traiter uniquement le code de commande (iCmd)
//--------------------------------------------------------------------------------
int
NSSmallBrother::depile(BBCmdMessage* pCmdMsg)
{
  if (_aCmdMsgArray.empty())
    return NSDLG_AUCUN ;

  iterBBCmdMsg it = _aCmdMsgArray.begin() ;

  int iCommande = (*it)->iCmd ;
  if (pCmdMsg)
    *pCmdMsg = **it ;

  delete *it ;
  _aCmdMsgArray.erase(it) ;

  return iCommande ;
}

//--------------------------------------------------------------------------------
// Ajoute un message en haut de la pile
//--------------------------------------------------------------------------------
void
NSSmallBrother::empile(BBCmdMessage* pCmdMsg, bool tuer)
{
  if ((BBCmdMessage*) NULL == pCmdMsg)
    return ;

try
{
  _aCmdMsgArray.push_back(new BBCmdMessage(*pCmdMsg)) ;

  if (tuer)
  {
    delete pCmdMsg ;
    pCmdMsg = 0 ;
  }
}
catch (...)
{
  erreur("Exception NSSmallBrother::empile.", standardError, 0) ;
}
}

//--------------------------------------------------------------------------------
// Ajoute un message en haut de la pile
//--------------------------------------------------------------------------------
void
NSSmallBrother::reEmpile(BBCmdMessage* pCmdMsg, bool tuer)
{
  if ((BBCmdMessage*) NULL == pCmdMsg)
    return ;

try
{
  _aCmdMsgArray.push_front(new BBCmdMessage(*pCmdMsg)) ;

  if (tuer)
  {
    delete pCmdMsg ;
    pCmdMsg = 0 ;
  }
}
catch (...)
{
  erreur("Exception NSSmallBrother::reEmpile.", standardError, 0) ;
}
}

void
NSSmallBrother::repriseSauvegarde()
{}

void
NSSmallBrother::effacerSauvegarde()
{}

void
NSSmallBrother::sauvegarde()
{
try
{
	// Attention : lors de la modification d'un conclusion, pPPTEnCours est nul
	//
	// if ((!pBBItem) || (!(pBBItem->pPPTEnCours)))
	if (NULL == _pBBItem)
		return ;
/*
    NSPatPathoArray* pPATP = pBBItem->pPPTEnCours;
    if (pPATP->pContexte != pContexte)
    	return;
    if (!pContexte->getPatient())
        return;

    //    // Il faut construire l'arbre
    //

    // On sauvegarde la patpatho initiale
    NSPatPathoArray* pPPTSauve = new NSPatPathoArray(pContexte);
    if (!(pPPTSauve->empty()))
        *pPPTSauve = *pPATP;

    // On met à jour la patpatho
    pBBItem->MiseAjourPatpatho();
*/

  // Modifs PA 16/05/09
  //
	// pBBItem->rapatrieTmpPpt() ;
	// NSPatPathoArray* pPATP = pBBItem->pTempPPT ;

  _TmpPPtArray.vider() ;

  _pBBItem->rapatrieTmpPpt(&_TmpPPtArray, 0) ;
	if (_TmpPPtArray.empty())
		return ;

	// On ajoute le titre
	_TmpPPtArray.ajouteTitre(_sLexiqueModule, _sNoeudModule) ;

	// on décale tout d'un cran (sauf le titre) après insertion du titre
	PatPathoIter j = _TmpPPtArray.begin() ;
	if (_TmpPPtArray.end() != j)
		j++ ;
	else
	{
		// delete pPPTSauve;
    return ;
	}

	while (_TmpPPtArray.end() != j)
	{
		(*j)->setColonne((*j)->getColonne() + 1) ;
		(*j)->setLigne((*j)->getLigne() + 1) ;
    j++ ;
	}

	//
	// Puis le transformer en chaine
	//
	string sChaine;
	_TmpPPtArray.genereChaine(pContexte, &sChaine) ;
	string sEntete = string("{PatPatho CN040|") ;
	if (pContexte->getPatient())
		sEntete += pContexte->getPatient()->getNom() + string("|")
                     + pContexte->getPatient()->getPrenom() + string("|")
                     + pContexte->getPatient()->getNaissance() + string("|")
                     + pContexte->getPatient()->getSexe() + string("}") ;
	else
		sEntete += string(" | |00000000| }") ;

	sChaine = sEntete + sChaine + string("{/PatPatho}") ;
	// On remet la patpatho
	// *pPATP = *pPPTSauve;
	// delete pPPTSauve;

	// On enregistre la chaîne
	ofstream outFile ;
	outFile.open("arbre.tmp", ios::out) ; // écrase le contenu
	if (!outFile)
		return ;

	outFile << sChaine ;	outFile.close() ;

  _bMustDeleteTmpFile = true ;
}
catch(ios_base::failure var)
{
	string sExept = "Exception (" + string(var.what()) + ") lors de la sauvegarde automatique" ;
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Problème inconnu lors de la sauvegarde automatique.", standardError, 0) ;
}
}

voidNSSmallBrother::rapatriePatpatho(BBItem* pItem)
{
	if ((NULL == pItem) || (pItem->_aBBItemFils.empty()))
		return ;

try
{
  // Modifs PA 16/05/09
	// pItem->pTempPPT->vider() ;

  _TmpPPtArray.vider() ;

	if (pItem->_aBBItemFils.empty())
		return ;

	// pour tous les fils
	//
	for (BBiter it = pItem->_aBBItemFils.begin() ; pItem->_aBBItemFils.end() != it ; it++)
	{
		//
		// Demande à l'élément de transfert de se mettre à jour, uniquement si
		// l'élément n'est pas prolongeable
		//
		if (false == (*it)->FilsProlongeable)
			if (0 == (*it)->getItemTransfertData()->TempTransfer())
				return ;
    //
		// Lance la fonction récursivement sur l'éventuel BBItem fils
		//
    if (false == (*it)->VectorFils.empty())
      for (BBiterFils j = (*it)->VectorFils.begin() ; (*it)->VectorFils.end() != j ; j++)
        rapatriePatpatho(*j) ;

		if ((*it)->isActif())
    {
    	//
      //cas particulier : si sEtiquettePatpatho contient £C prendre les données
      //vider le complément
      //
      if (((*it)->getItemLabel().find(string("£C;")) != NPOS) ||
            	((*it)->getItemLabel().find(string("/£C;")) != NPOS))
      	(*it)->getItemTransfertMsg()->SetComplement(string("")) ;

      NSVectFatheredPatPathoArray* pPatpathoItem = (*it)->getTmpPatho() ;

      //si le vecteur est vide on ajoute seulement  l'étiquette
      //if (pPatpathoItem->empty())
      if ((*it)->TmpPptvide())
      	pItem->AjouteTmpEtiquette(&_TmpPPtArray, *it) ;

      // sinon, pour chaque élément du vecteur , on ajoute
      // l'étiquette et cet élément
      else
      {
      	if (string((*it)->getItemData()->decalageNiveau) == string("+00+00"))
        	for (FatheredPatPathoIterVect jt = pPatpathoItem->begin() ; pPatpathoItem->end() != jt ; jt++)
          	// pItem->pTempPPT->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 0) ;
            _TmpPPtArray.ajouteVecteur((*jt)->getPatPatho(), ORIGINE_PATH_PATHO + 0) ;
        else
        {
        	for (FatheredPatPathoIterVect jt = pPatpathoItem->begin() ; pPatpathoItem->end() != jt ; jt++)
          {
          	pItem->AjouteTmpEtiquette(&_TmpPPtArray, *it) ;
            // pItem->pTempPPT->ajouteVecteur((*j)->getPatPatho(), ORIGINE_PATH_PATHO + 1) ;
            _TmpPPtArray.ajouteVecteur((*jt)->getPatPatho(), ORIGINE_PATH_PATHO + 1) ;
          }
        }
      }
    }
  }
}
catch (...)
{
	erreur("Exception NSSmallBrother::rapatriePatpatho.", standardError, 0) ;
}
}

/*
void
NSSmallBrother::rapatriePatpatho(BBItem* pItem)
{
    if (pItem->aBBItemFils.empty())
        return;

try
{
    pItem->pPPTEnCours->vider();

    NSVectPatPathoArray* pPatpathoItem;

    // pour tous les fils
    //
    for (BBiter i = pItem->aBBItemFils.begin(); i != pItem->aBBItemFils.end(); i++)
    {
    	//
		// Demande à l'élément de transfert de se mettre à jour, uniquement si
		// l'élément n'est pas prolongeable
		//
    	if ((*i)->FilsProlongeable == false)
		{
        	if (!((*i)->pTransfert->Transfer(tdGetData)))
            	return;

        	// Utilisé pour le transfert de document type consultation
        	// pour la mise à jour de la base des textes libres
        	//
        	if (!((*i)->pTransfert->TransferFinal(tdGetData)))
            	return;
		}
    	//
		// Lance la fonction récursivement sur l'éventuel BBItem fils
		//
    	for (BBiterFils j = (*i)->VectorFils.begin(); j != (*i)->VectorFils.end(); j++)
        	if (*j)
   	    		rapatriePatpatho((*j));

        if ((*i)->Actif())
        {
        	//
            //cas particulier : si sEtiquettePatpatho contient £C prendre les données
            //vider le complément
            //
            if (((*i)->sEtiquette.find(string("£C;")) != NPOS) ||
            	((*i)->sEtiquette.find(string("/£C;")) != NPOS))
            	(*i)->pTransfert->pTransfertMessage->SetComplement(string(""));

            pPatpathoItem = (*i)->getPatPatho();

            //si le vecteur est vide on ajoute seulement  l'étiquette
            //if (pPatpathoItem->empty())
            if ((*i)->PatPtahovide())
            	pItem->AjouteEtiquette((*i));

            // sinon, pour chaque élément du vecteur , on ajoute
            // l'étiquette et cet élément
            else
            {
            	if (string((*i)->pDonnees->decalageNiveau) == string("+00+00"))
                {
                	for (PatPathoIterVect j = pPatpathoItem->begin(); j != pPatpathoItem->end(); j++)
                    	pItem->pPPTEnCours->ajouteVecteur(*j, ORIGINE_PATH_PATHO + 0);
                }
                else
                {
                	for (PatPathoIterVect j = pPatpathoItem->begin(); j != pPatpathoItem->end(); j++)
                    {
                    	pItem->AjouteEtiquette((*i));
                        pItem->pPPTEnCours->ajouteVecteur(*j, ORIGINE_PATH_PATHO + 1);
                    }
                }
            }
        }
    }
}
catch (const exception& e)
{
    string sExept = "Exception (" + string(e.what()) + ") au rapatriement des données";
    erreur(sExept.c_str(), standardError, 0) ;
}
}
*/

voidNSSmallBrother::showBB(){	rapatriePatpatho(_pBBItem) ;	addDatasFromNautilusOnBlackboard(pContexte, _pBBItem->getPPTEncours()) ;}void
NSSmallBrother::reprise()
{
try
{
	string sChaine = string("") ;

	// On récupère la chaîne  ifstream inFile ;
  inFile.open("arbre.tmp", ios::in) ;
	if (!inFile)
		return ;

	string line ;

  while (!inFile.eof())  {
  	getline(inFile, line) ;
    if (line != "")
    	sChaine += line ;
  }

  // inFile >> sChaine;  inFile.close() ;

  if (sChaine == "")  	return ;

  string sCivilite = "" ;
  if (("{PatPatho CN030" == string(sChaine, 0, 15)) ||      ("{PatPatho CN040" == string(sChaine, 0, 15)))  {
  	size_t n = 16 ;
    string sNom = "" ;
    for ( ; (n < strlen(sChaine.c_str())) && ('|' != sChaine[n]) ; n++)
    	sNom += sChaine[n] ;
    if (n < strlen(sChaine.c_str()))
    	n++ ;
    string sPrenom = "" ;
    for ( ; (n < strlen(sChaine.c_str())) && ('|' != sChaine[n]) ; n++)
    	sPrenom += sChaine[n] ;

    if (n < strlen(sChaine.c_str()))    	n++ ;
    string sNaissance = "" ;
    for ( ; (n < strlen(sChaine.c_str())) && ('|' != sChaine[n]) ; n++)
    	sNaissance += sChaine[n] ;
    sCivilite = sPrenom + string(" ") + sNom ;
  }

	NSPatPathoArray PPTReprise(pContexte->getSuperviseur()) ;  string sPatId = pContexte->getPatient()->getNss() ;  PPTReprise.initialiseDepuisChaine(pContexte, &sChaine, sPatId) ;

  if (PPTReprise.empty())  	return ;

  PatPathoIter j = PPTReprise.begin() ;  if ((*j)->getLexique() != _sLexiqueModule)
  	return ;

  string sMessage ;
  if (string("") == sCivilite)  	sMessage = string("Il existe un compte rendu de ce type en mémoire temporaire.\n Souhaitez vous le récupérer ?") ;
  else
		sMessage = string("Un compte rendu de ce type, réalisé pour ") +
        		   sCivilite +
                   string(" est resté en mémoire temporaire.\n Souhaitez vous le récupérer ?") ;

	int idRet = MessageBox(0, sMessage.c_str(), "Reprise de fichier temporaire", MB_YESNO) ;  if (IDYES != idRet)
  	return ;

  *_pPatPathoArray = PPTReprise ;
	return ;}
catch(ios_base::failure var)
{
	string sExept = "Exception (" + string(var.what()) + ") à la reprise des données" ;
  erreur(sExept.c_str(), standardError, 0) ;
}
catch (...)
{
	erreur("Exception NSSmallBrother::reprise.", standardError, 0) ;
}
}

voidNSSmallBrother::defaire()
{}

voidNSSmallBrother::refaire()
{}

// -------------------------------------------------------------------------
// -------------------- METHODES DE NSModuleArray ---------------------------
// -------------------------------------------------------------------------

NSModuleInstance::~NSModuleInstance()
{
  delete _pTModule ;
}

NSModuleDialogue::NSModuleDialogue(){
	_sLibelle = string("") ;
  _pModule  = (NSModuleInstance *) 0 ;
}

NSModuleDialogue::NSModuleDialogue(const NSModuleDialogue& src)
{
  _sLibelle = src._sLibelle ;

  _pModule  = (NSModuleInstance *) 0 ;
  getModule(src._pModule) ;
}

void
NSModuleDialogue::freeModule()
{
  if ((NSModuleInstance*) NULL == _pModule)
    return ;

  _pModule->_iInstanceCount-- ;
  if (_pModule->_iInstanceCount > 0)
    return ;

  if ((_pModule->_pTModule != pNSDLLModule))
    delete _pModule ;

  _pModule = (NSModuleInstance*) 0 ;
}

void
NSModuleDialogue::getModule(NSModuleInstance* pModuleInstance)
{
  freeModule() ;

  if ((NSModuleInstance*) NULL == pModuleInstance)
    return ;

  _pModule = pModuleInstance ;
  _pModule->_iInstanceCount++ ;
}

NSModuleDialogue::~NSModuleDialogue(){
  freeModule() ;
}

NSModuleDialogue&
NSModuleDialogue::operator=(const NSModuleDialogue& src)
{
  if (this == &src)
		return *this ;

  _sLibelle = src._sLibelle ;
  getModule(src._pModule) ;

  return *this ;
}

TModule*
NSModuleDialogue::getModule()
{
  if ((NSModuleInstance*) NULL == _pModule)
    return (TModule*) 0 ;

  return _pModule->_pTModule ;
}

void
NSModuleDialogue::setModule(TModule* pM)
{
  freeModule() ;

  _pModule = new NSModuleInstance() ;

  _pModule->_pTModule       = pM ;
  _pModule->_iInstanceCount = 1 ;
}

//---------------------------------------------------------------------------//  Description :	Constructeur copie
//  Retour :		Rien
//---------------------------------------------------------------------------
NSResModuleArray::NSResModuleArray(const NSResModuleArray& rv)
                 :NSTModuleArray()
{
try
{
  if (false == rv.empty())
    for (iterNSTModuleArrayConst i = rv.begin() ; rv.end() != i ; i++)      push_back(NSModuleDialogue(*i)) ;
}
catch (...)
{
  erreur("Exception NSResModuleArray copy ctor.", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Fonction:		~NSModuleArray()
//  Description:	Destructeur
//---------------------------------------------------------------------------
void
NSResModuleArray::vider()
{
  if (empty())
    return ;

  for (iterNSTModuleArray i = begin() ; end() != i ; )  {
    TModule* pModule = i->getModule() ;

    if (pModule)
    {
      const char* nomModule = pModule->GetName() ;
      //
      // ne pas deleter le module "nsbb.dll" , il sera deleté à
      // DllEntryPoint(nsbb.cpp)
      //
      if ((string(nomModule).find(string("nsbb")) == NPOS) &&
          (string(nomModule).find(string("NSBB")) == NPOS) &&
          (string(nomModule).find(string("nsmbb")) == NPOS) &&
          (string(nomModule).find(string("NSMBB")) == NPOS) )
      {
        delete pModule ;
        i->setModule((TModule*) 0) ;
      }
    }
    erase(i) ;
  }
}

NSResModuleArray::~NSResModuleArray(){
	vider() ;
}

//---------------------------------------------------------------------------//  Fonction	 : NSResModuleArray::operator=(NSResModuleArray src)
//
//  Description : Opérateur d'affectation
//---------------------------------------------------------------------------
NSResModuleArray&
NSResModuleArray::operator=(const NSResModuleArray& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (iterNSTModuleArrayConst i = src.begin() ; src.end() != i ; i++)			push_back(NSModuleDialogue(*i)) ;

	return *this ;}
catch (...)
{
	erreur("Exception NSResModuleArray (op =).", standardError, 0) ;
	return *this ;
}
}

//***************************************************************************// 							Implémentation des mèthodes ClasseCalculVector
//***************************************************************************
ClasseCalculVector::~ClasseCalculVector()
{
	vider();
}

ClasseCalculVector::ClasseCalculVector()                   :vectclassCalcul()
{}

voidClasseCalculVector::vider()
{
	if (empty())
		return ;

	iterStringCalcul i = begin() ;	while (end() != i)
		erase(i) ;
}

ClasseCalculVector&ClasseCalculVector::operator=(const ClasseCalculVector& src)
{
	if (this == &src)
		return *this ;

try
{
	vider() ;

	if (false == src.empty())
		for (iterStringCalculConst i = src.begin() ; src.end() != i ; i++)			push_back(classCalcul(*i)) ;

	return *this ;
}
catch (...)
{
	erreur("Exception ClasseCalculVector (op =).", standardError, 0) ;
	return *this ;
}
}

ClasseCalculVector::ClasseCalculVector(const ClasseCalculVector& src){
try
{
	if (false == src.empty())
		for (iterStringCalculConst i = src.begin() ; src.end() != i ; i++)
			push_back(classCalcul(*i)) ;
}
catch (...)
{
	erreur("Exception ClasseCalculVector copy ctor.", standardError, 0) ;
}
}

//************************************************************************
// classe classCalcul
//************************************************************************
classCalcul::classCalcul(const classCalcul& src)
{
  sLocalisation = src.sLocalisation ;
  sContenu      = src.sContenu ;
}

//---------------------------------------------------------------------------
//  Opérateur d'affectation
//---------------------------------------------------------------------------
classCalcul&
classCalcul::operator=(const classCalcul& src)
{  if (this == &src)    return *this ;  sLocalisation = src.sLocalisation ;
  sContenu      = src.sContenu ;

  return *this ;
}

bool
classCalcul::operator==(const classCalcul& o)
{
  if ((sLocalisation == o.sLocalisation) &&
      (sContenu      == o.sContenu))
    return true ;

  return false ;
}

classCalcul::classCalcul(string sLoca, string sCont){
  sLocalisation = sLoca ;
  sContenu      = sCont ;
}

//***************************************************************************// 							Implémentation des mèthodes BBCmdMsgVector
//***************************************************************************
BBCmdMsgVector::BBCmdMsgVector()
               :vectbbcmdmsg()
{
}

BBCmdMsgVector::BBCmdMsgVector(const BBCmdMsgVector& src){
try
{
  if (false == src.empty())
    for (iterBBCmdMsgConst it = src.begin() ; src.end() != it ; it++)
      push_back(new BBCmdMessage(**it)) ;
}
catch (...)
{
  erreur("Exception BBCmdMsgVector copy ctor.", standardError, 0) ;
}
}

BBCmdMsgVector::~BBCmdMsgVector()
{
  vider() ;
}

void
BBCmdMsgVector::vider()
{
  if (empty())
    return ;

  for (iterBBCmdMsg it = begin() ; end() != it ; it++)
    delete *it ;

  clear() ;
}

BBCmdMsgVector&BBCmdMsgVector::operator=(const BBCmdMsgVector& src)
{
  if (&src == this)
		return *this ;

try
{
  vider() ;

  if (false == src.empty())
    for (iterBBCmdMsgConst it = src.begin() ; src.end() != it ; it++)      push_back(new BBCmdMessage(**it)) ;
}
catch (...)
{
  erreur("Exception BBCmdMsgVector (op =).", standardError, 0) ;
}
  return *this ;
}

//************************************************************************
// classe BBCmdMessage
//************************************************************************
BBCmdMessage::BBCmdMessage(int cmd, string message)
{
  iCmd     = cmd ;
  sMessage = message ;
}

BBCmdMessage::BBCmdMessage(const BBCmdMessage& src){
  iCmd     = src.iCmd ;
  sMessage = src.sMessage ;
}

//---------------------------------------------------------------------------//  Opérateur d'affectation
//---------------------------------------------------------------------------
BBCmdMessage&
BBCmdMessage::operator=(const BBCmdMessage& src)
{
  if (&src == this)
		return *this ;

  iCmd     = src.iCmd ;
  sMessage = src.sMessage ;

  return *this ;
}
bool
BBCmdMessage::operator==(const BBCmdMessage& o)
{
  if ((iCmd     == o.iCmd) &&
      (sMessage == o.sMessage))
    return true ;

  return false ;
}

