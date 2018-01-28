#include <owl\combobox.h>
#include "nsbb\nscombo.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsbbitem.h"
#include "nautilus\nsepicap.h"
#include "nsepisod\eptables.h"
#include "dcodeur\nsgen.h"

const char  codeMARK    = 'O';

const char decimSepar = ',';   // Edit double bouton

DEFINE_RESPONSE_TABLE1(NSComboSemantique, TComboBox)
	EV_NOTIFY_AT_CHILD(CBN_SELCHANGE, CBSelChange),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSComboSemantique::NSComboSemantique(NSContexte *pCtx, TWindow* parent, int resId)
                  :TComboBox(parent, resId), NSRoot(pCtx)
{
	DisableTransfer() ;

	//	sContenuBrut        = "" ;
	sContenuTransfert = "" ;
	sContenuAncien    = "";
	pControle         = 0 ;
	ActiveToi         = true ;
	TextLimit         = 15 ;
  pNSEdit           = 0 ;
}

NSComboSemantique::NSComboSemantique(NSContexte *pCtx, TWindow* parent, int resId, int x, int y, int w, int h,
                                uint32 style, uint textLimit,TModule* module)
                  :TComboBox(parent, resId, x, y, w, h, style, textLimit, module), NSRoot(pCtx)
{
	DisableTransfer() ;

	Attr.Style = (Attr.Style ^ CBS_SORT ) | style ;

	//sContenuBrut      = "" ;
	sContenuTransfert   = "" ;
	sContenuAncien      = "" ;
	pControle           = 0 ;
	ActiveToi           = true ;
	TextLimit           = 15 ;
  pNSEdit             = 0 ;
}

//
// Destructeur
//
NSComboSemantique::~NSComboSemantique()
{
  if (pNSEdit)
    delete pNSEdit ;
}

// SetupWindow : on remplit avec les "est un" du concept
void
NSComboSemantique::SetupWindow()
{
  TComboBox::SetupWindow() ;

	ClearList() ;

	if ((NULL == pControle) || (NULL == pControle->getTransfert()))
    return ;

  BBFilsItem* pBBFilsItem = pControle->getTransfert()->getFilsItem() ;
  if (NULL == pBBFilsItem)
    return ;

  string sLang = string("") ;
  if (pContexte)
    sLang = pContexte->getUserLanguage() ;

  // If some sons are proposed, then fill the combo with it, else take the
  // "is a" for the term
  //
  if (true == pBBFilsItem->VectorFils.empty())
	{
		string sCompl = pBBFilsItem->getItemLabel() ;
		pContexte->getDico()->donneCodeSens(sCompl) ;

    while ((string(sCompl, 0, 2) == "£C") || (sCompl == "LTYPA"))
    {
      if ((NULL == pBBFilsItem) || (NULL == pBBFilsItem->getItemFather()))
        break ;

      pBBFilsItem = pBBFilsItem->getItemFather()->_pBBFilsPere ;
      sCompl = pBBFilsItem->getItemLabel() ;
      pContexte->getDico()->donneCodeSens(sCompl) ;
    }

		searchList(sCompl) ;

		if (false == aTermList.empty())
			for (pthIter it = aTermList.begin(); aTermList.end() != it ; it++)
				AddString((*it)->_Donnees.libelle) ;
	}
  else
  {
    BBItem *pItem = *(pBBFilsItem->VectorFils.begin()) ;
    if ((BBItem*) NULL == pItem)
      return ;

    for (BBiter it = pItem->_aBBItemFils.begin() ;
                                     pItem->_aBBItemFils.end() != it ; it++)
    {
      string sCompl = (*it)->getItemLabel() ;

      NSPathologData Data ;
      GENRE          iGenre ;
      pContexte->getDico()->trouvePathologData(sLang, &sCompl, &Data) ;
      Data.donneGenre(&iGenre) ;

      string sLibel ;
      NSGenerateur* pGene = pContexte->getDico()->getGenerateur() ;
      pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

      NSPathologInfo* pPatholog = new NSPathologInfo() ;
      pPatholog->_Donnees = Data ;

      // In order to match select entry with the concept
      //
      strcpy(pPatholog->_Donnees.libelle, sLibel.c_str()) ;

			aTermList.push_back(pPatholog) ;

      AddString(sLibel.c_str()) ;
    }
  }

	//ShowList();
}

void
NSComboSemantique::searchList(string sConcept)
{
	aTermList.vider() ;

	string sLang = pContexte->getUserLanguage() ;

	NSSuper* pSuper = pContexte->getSuperviseur() ;

try
{
	VectString aVecteurString ;
	pSuper->getFilGuide()->TousLesVrais(sConcept, "ES", &aVecteurString, "ENVERS") ;
  if (aVecteurString.empty())
  	return ;

	NSPathologData Data ;

	IterString itStr = aVecteurString.begin() ;
	for ( ; aVecteurString.end() != itStr ; itStr++)	{
    string sCode = **itStr + string("1") ;
    bool bTrouve = pContexte->getDico()->trouvePathologData(sLang, &sCode, &Data) ;
    if (bTrouve)
    {
    	NSPathologInfo* pPatholog = new NSPathologInfo() ;
      pPatholog->_Donnees = Data ;
			aTermList.push_back(pPatholog) ;
  	}
	}
}
catch (...)
{
	erreur("Exception NSComboSemantique::searchList", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::Transfer(TTransferDirection direction,
//															 int* pActif, string* pMessage)
//
//  Description:	Si direction = tdSetData
//							Initialise le contrôle
//						Si direction = tdGetData
//							Initialise pActif et pMessage
//
//  Arguments:		direction : tdGetData ou tdSetData
//
//  Returns:		1 si le transfert a fonctionné
//						0 sinon
//---------------------------------------------------------------------------
void
NSComboSemantique::donneTransfert()
{
	int iIdx = GetSelIndex() ;
	char far* sStringSel = new char[CLASSIF_LIBELLE_LEN] ;
	GetString(sStringSel, iIdx) ;

	if (aTermList.empty())
	{
		delete[]  sStringSel ;
		return ;
	}

	pthIter it = aTermList.begin() ;
	for ( ; (aTermList.end() != it) &&
            (string((*it)->_Donnees.libelle) != string(sStringSel)) ; it++) ;
	if (aTermList.end() != it)
		sContenuTransfert = (*it)->_Donnees.code ;
	delete[]  sStringSel ;
}

uint
NSComboSemantique::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (direction == tdSetData)
	{
		switch (*pActif)
		{
    	case inactiveCtrl :

      	sContenuTransfert = "" ;
        Clear() ;
        SetText(sContenuTransfert.c_str()) ;
        sContenuAncien = "" ;
        //Insert(sContenuBrut.c_str());
        break ;

			case activeCtrl :
      	sContenuTransfert = pMessage->GetLexique();
        Type = pMessage->GetType();
        Clear();
        if ((sContenuTransfert != "") && (!(aTermList.empty())))
        {
        	pthIter it = aTermList.begin() ;
          for ( ; ((aTermList.end() != it) &&
          				(string((*it)->_Donnees.code) != sContenuTransfert)) ;
                                    it++) ;
          if (aTermList.end() != it)
          	SetText((*it)->_Donnees.libelle) ;
      	}
      	break ;
		}
	}
	else if (direction == tdGetData)
	{
		donneTransfert() ;
		sContenuAncien = sContenuTransfert ;
		if (string("") == sContenuTransfert)
			*pActif = inactiveCtrl ;
		else
			*pActif = activeCtrl ;

		if (pMessage)
		{
			pMessage->SetLexique(sContenuTransfert) ;
			pMessage->SetType(Type) ;
		}
	}
	return 1 ;
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::activeControle(int activation, string message)
//
//  Description:	Initialise le contrôle avant affichage de la boîte de
//						dialogue
//
//  Arguments:		activation : BF_CHECKED ou BF_UNCHECKED
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSComboSemantique::activeControle(int activation, Message* pMessage)
{
	// Gestion windows standard
	// Windows standard behaviour
	if (pControle && pControle->getNSDialog())
	{
		if (pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
			bWinStd = true ;
	}

  string sContenu ;

	switch (activation)
  {
    case BF_CHECKED   :

      sContenuTransfert = pMessage->GetLexique() ;
      sContenuAncien = sContenuTransfert ;
      Type = pMessage->GetType() ;
      Clear() ;
      if (string("") != sContenuTransfert)
      {
        pthIter it = aTermList.begin() ;
        for ( ; ((aTermList.end() != it) &&
                        (string((*it)->_Donnees.code) != sContenuTransfert));
                                                                    it++) ;
        if (aTermList.end() != it)
          sContenu = (*it)->_Donnees.libelle ;

        // Note : dans le cas où on vient d'être activé par le pNSEdit (NSEditLexiqueDerive)
        // on ne doit pas faire de SetText car le message transmis est vide.
        if (NULL == pNSEdit)
          SetText(sContenu.c_str()) ;
      }
      //
      //
      else
      {
        if (NULL == pControle)
          return ;

        NSTransferInfo* pTransfert = pControle->getTransfert() ;
        if (NULL == pTransfert)
          return ;

        NSVectFatheredPatPathoArray* pVectPpt = pTransfert->getPatPatho() ;
        if ((NULL == pVectPpt) || (pVectPpt->empty()))
          return ;

        NSFatheredPatPathoArray* pFatherPpt = *(pVectPpt->begin()) ;
        if ((NULL == pFatherPpt) ||
            (NULL == pFatherPpt->getPatPatho()) ||
            (pFatherPpt->getPatPatho()->empty()))
          return ;

        NSPatPathoInfo* pPpt = *(pFatherPpt->getPatPatho()->begin()) ;

        string sCompl = pPpt->getNodeLabel() ;

        string sLang = pContexte->getUserLanguage() ;

        NSPathologData Data ;
        GENRE          iGenre ;
        pContexte->getDico()->trouvePathologData(sLang, &sCompl, &Data) ;
        Data.donneGenre(&iGenre) ;

        string sLibel ;
        NSGenerateur* pGene = pContexte->getDico()->getGenerateur() ;
        pGene->donneLibelleAffiche(&sLibel, &Data, iGenre) ;

        // SetSelString doesn't allow exact comparison
        int iPosit = FindExactString(sLibel.c_str(), 0) ;
        if (iPosit >= 0)
          SetSelIndex(iPosit) ;
      }
      //	Type = pMessage->GetType();

      break ;

		case BF_UNCHECKED :

      sContenuTransfert = string("") ;
      sContenuAncien = sContenuTransfert ;
      Clear() ;
      SetText(sContenuTransfert.c_str()) ;
      break ;
  }
}

bool
NSComboSemantique::canWeClose()
{
	return true ;
}

void
NSComboSemantique::CBSelChange()
{
	if (!ActiveToi)
  {
  	ActiveToi = true ;
    return ;
  }
  int iIdx = GetSelIndex() ;
  char far* sStringSel = new char[100] ;
  GetString(sStringSel, iIdx) ;
  string sCodeSel = "" ;

  if (false == aTermList.empty())
  {
		pthIter it = aTermList.begin() ;
  	for ( ; (aTermList.end() != it) &&
            (string((*it)->_Donnees.libelle) != string(sStringSel)) ; it++) ;
    if (aTermList.end() != it)
        sCodeSel = (*it)->_Donnees.code ;
  }

  delete[] sStringSel ;

  if ((sContenuAncien == sCodeSel) || (sCodeSel == ""))
  	return ;

  BBItem* pBBItemPere = pControle->getTransfert()->getFilsItem()->getItemFather() ;
  //
  // Vider la liste des fils du BBItem sur cet edit
  //
  pControle->getTransfert()->getFilsItem()->VectorFils.vider() ;
  if (pNSEdit)
  {
  	delete pNSEdit ;
    pNSEdit = 0 ;
  }
  if (NULL == pNSEdit)
  {
  	pNSEdit = new NSEditLexiqueDerive(pContexte, this) ;
    pNSEdit->pControle = new NSControle(pContexte, pBBItemPere, sCodeSel, "") ;
    pNSEdit->pControle->setControle(dynamic_cast<void*>(pNSEdit)) ;
    pNSEdit->pControle->setType(pNSEdit->donneType()) ;
    pNSEdit->pContexte = pContexte ;
    pNSEdit->pControle->setNSDialog(pBBItemPere->getDialog()) ;
    pNSEdit->pControle->setMUEView(pBBItemPere->getView()) ;
  }

  pNSEdit->sContenuTransfert = sCodeSel ;
  pNSEdit->pControle->setIdentite(sCodeSel) ;
  CreerBBItem() ;
  if (pNSEdit->pControle->getTransfert())
  {
  	Message MessageNewFils ;
    pNSEdit->pControle->getTransfert()->ctrlNotification(BF_UNCHECKED, BF_CHECKED, &MessageNewFils) ;
	}
}

void
NSComboSemantique::CreerBBItem()
{
  if ((NULL == pControle) || (NULL == pControle->getTransfert()))
    return ;
  //
  //créer un BBfilsItem rattaché à ce nouveau controle
  //
  BBFilsItem* pFilsItemPere = pControle->getTransfert()->getFilsItem() ; //le BBFilsItem rattaché au père
  if (NULL == pFilsItemPere) 
    return ;

  if (NULL == pNSEdit)
    return ;

  pFilsItemPere->CreerFilsManuel(pNSEdit->pControle, 0) ;

  // on ajoute maintenant manuellement le nom du dialogue du BBItem père
  // l'array des fils contient en principe le fils créé ci-dessus.
  if (false == pFilsItemPere->VectorFils.empty())
    for (BBiterFils iter = pFilsItemPere->VectorFils.begin(); iter != pFilsItemPere->VectorFils.end(); iter++)
    {
      (*iter)->setDialogName(pFilsItemPere->getItemFather()->getDialogName()) ;
      (*iter)->_sIdArchetype = pFilsItemPere->getItemFather()->_sIdArchetype ;
    }
}

////////////////////// NSComboClassif

DEFINE_RESPONSE_TABLE1(NSComboClassif, TComboBox)
	EV_NOTIFY_AT_CHILD(CBN_SELCHANGE, CBSelChange),
END_RESPONSE_TABLE;

//
// Constructeur
//
NSComboClassif::NSComboClassif(NSContexte *pCtx, TWindow* parent, int resId)
               :TComboBox(parent, resId), NSRoot(pCtx)
{
  DisableTransfer() ;

  sContenuTransfert = string("") ;
  sContenuAncien    = string("") ;
  pControle         = 0 ;
  ActiveToi         = true ;
  TextLimit         = 15 ;
}

NSComboClassif::NSComboClassif(NSContexte *pCtx, TWindow* parent, int resId, int x, int y, int w, int h,
                                uint32 style, uint textLimit,TModule* module)
               :TComboBox(parent, resId, x, y, w, h, style, textLimit, module), NSRoot(pCtx)
{
  DisableTransfer() ;

  Attr.Style = (Attr.Style ^ CBS_SORT ) | style ;

	sContenuTransfert = string("") ;
  sContenuAncien    = string("") ;
	pControle         = 0 ;
  ActiveToi         = true ;
  TextLimit         = 15 ;
}

//
// Destructeur
//
NSComboClassif::~NSComboClassif()
{
}

void
NSComboClassif::SetupWindow()
{
  ClearList() ;

  if ((NULL == pControle) || (NULL == pControle->getTransfert()) ||
      (NULL == pControle->getTransfert()->getFilsItem()))
  {
    TComboBox::SetupWindow() ;
    return ;
  }

  string sCompl = pControle->getTransfert()->getFilsItem()->getItemLabel() ;
  pContexte->getDico()->donneCodeSens(sCompl) ;

  searchList(sCompl) ;
  if (false == aClassifList.empty())
    for (NSEpiClassifInfoIter iter = aClassifList.begin(); aClassifList.end() != iter ; iter++)
      AddString((*iter)->getLabel().c_str()) ;

  TComboBox::SetupWindow() ;
}

void
NSComboClassif::searchList(string sClassif)
{
try
{
	NSEpiClassif Classif(pContexte->getSuperviseur()) ;

	Classif.lastError = Classif.open() ;
	if (Classif.lastError != DBIERR_NONE)
	{
		erreur("Ouverture du fichier Classif impossible", standardError, Classif.lastError, GetHandle()) ;
		return ;
	}

	CURProps curProps ;
  /* DBIResult lastError = */ DbiGetCursorProps(Classif.PrendCurseur(), curProps) ;
  Byte* pIndexRec = new Byte[curProps.iRecBufSize] ;
  memset(pIndexRec, 0, curProps.iRecBufSize) ;
  DbiPutField(Classif.PrendCurseur(), CLASSIF_CLASSIF_FIELD, pIndexRec, (Byte*) sClassif.c_str()) ;
  DbiPutField(Classif.PrendCurseur(), CLASSIF_CODE_FIELD,    pIndexRec, (Byte*) "") ;

  Classif.lastError = Classif.chercheClefComposite("",
                                                   0,
                                                   keySEARCHGEQ,
                                                   dbiWRITELOCK,
                                                   pIndexRec) ;
  delete[] pIndexRec ;

  if ((Classif.lastError != DBIERR_NONE) && (Classif.lastError != DBIERR_BOF))
  {
  	erreur("Erreur à la recherche dans la base Classif.", standardError, Classif.lastError, GetHandle()) ;
    Classif.close() ;
    return ;
  }

  if (Classif.lastError == DBIERR_BOF)
  {
  	erreur("Aucune fiche ne correspond.", standardError, Classif.lastError, GetHandle()) ;
    Classif.close() ;
    return ;
  }

  Classif.lastError = Classif.getRecord() ;
  if (Classif.lastError != DBIERR_NONE)
  {
  	erreur("Erreur à la lecture d'une fiche Classif.", standardError, Classif.lastError, GetHandle());
    Classif.close() ;
    return ;
  }

  while ((DBIERR_EOF != Classif.lastError) && (Classif.getClassification() == sClassif))
  {
    aClassifList.push_back(new NSEpiClassifInfo(&Classif)) ;

    Classif.lastError = Classif.suivant(dbiWRITELOCK) ;
    if ((Classif.lastError != DBIERR_NONE) && (Classif.lastError != DBIERR_EOF))
    {
    	erreur("Erreur d'acces à une fiche Classif.", standardError, Classif.lastError, GetHandle()) ;
      Classif.close() ;
      return ;
    }
    if (Classif.lastError == DBIERR_NONE)
    {
    	Classif.lastError = Classif.getRecord();
      if (Classif.lastError != DBIERR_NONE)
      {
      	erreur("Erreur à la lecture d'une fiche Classif.", standardError, Classif.lastError, GetHandle()) ;
        Classif.close() ;
        return ;
			}
		}
	}  //while

	Classif.close() ;
	return ;
}
catch (...)
{
	erreur("Exception Exception realisation table Classif", standardError, 0) ;
}
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::Transfer(TTransferDirection direction,
//															 int* pActif, string* pMessage)
//
//  Description:	Si direction = tdSetData
//							Initialise le contrôle
//						Si direction = tdGetData
//							Initialise pActif et pMessage
//
//  Arguments:		direction : tdGetData ou tdSetData
//
//  Returns:		1 si le transfert a fonctionné
//						0 sinon
//---------------------------------------------------------------------------
void
NSComboClassif::donneTransfert()
{
  int iIdx = GetSelIndex() ;
  char far* sStringSel = new char[CLASSIF_LIBELLE_LEN] ;
  GetString(sStringSel, iIdx) ;

   /* for (NSEpiClassifInfoIter iter = aClassifList.begin(); (iter != aClassifList.end()) ; iter++)
    if (string((*iter)->pDonnees->libelle) == string(sStringSel))
    {
        sContenuTransfert = (*iter)->pDonnees->code;
        sClassification =  (*iter)->pDonnees->classification;
    }      */

  if (aClassifList.empty())
  {
    delete[] sStringSel ;
    return ;
  }

  NSEpiClassifInfoIter iter = aClassifList.begin() ;
  for ( ; (aClassifList.end() != iter) &&
            ((*iter)->getLabel() != string(sStringSel)) ; iter++) ;
  if (aClassifList.end() != iter)
    sContenuTransfert = (*iter)->getCode() ;
  delete[]  sStringSel ;
}

uint
NSComboClassif::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (direction == tdSetData)
	{
		switch (*pActif)
		{
			case inactiveCtrl :

        sContenuTransfert = string("") ;
        Clear() ;
        SetText(sContenuTransfert.c_str()) ;
        sContenuAncien = "" ;
        //Insert(sContenuBrut.c_str());
        break ;

			case activeCtrl :

        sContenuTransfert = pMessage->GetComplement() ;
        Type = pMessage->GetType() ;
        Clear() ;

        if (string("") != sContenuTransfert)
        {
          NSEpiClassifInfoIter iter = aClassifList.begin() ;
          for ( ; ((aClassifList.end() != iter) &&
                      ((*iter)->getCode() != sContenuTransfert)) ;
                                    iter++) ;
          if (aClassifList.end() != iter)
            SetText((*iter)->getLabel().c_str()) ;
        }
        break ;
		}
	}
  else if (direction == tdGetData)
	{
    donneTransfert() ;
    sContenuAncien = sContenuTransfert ;
    if (sContenuTransfert == "")
      *pActif = inactiveCtrl ;
    else
      *pActif = activeCtrl ;

    if (pMessage)
    {
      pMessage->SetComplement(sContenuTransfert) ;
      pMessage->SetType(Type) ;
    }
  }
	return 1 ;
}

//---------------------------------------------------------------------------//  Function: 		NSEdit::activeControle(int activation, string message)
//
//  Description:	Initialise le contrôle avant affichage de la boîte de
//						dialogue
//
//  Arguments:		activation : BF_CHECKED ou BF_UNCHECKED
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSComboClassif::activeControle(int activation, Message* pMessage)
{
  // Gestion windows standard
  // Windows standard behaviour
  if (pContexte->getPredi() && pContexte->getPredi()->_bReturnCloses)
    bWinStd = true ;

	switch (activation)
  {
    case BF_CHECKED   :

      sContenuTransfert = pMessage->GetComplement() ;
      Type = pMessage->GetType() ;
      Clear() ;
      if (sContenuTransfert != "")
      {
        NSEpiClassifInfoIter iter = aClassifList.begin() ;
        for ( ; ((aClassifList.end() != iter) &&
                        ((*iter)->getCode() != sContenuTransfert));
                                                                    iter++) ;
        if (aClassifList.end() != iter)
          SetText((*iter)->getLabel().c_str()) ;
      }
      //	Type = pMessage->GetType();

      break ;

		case BF_UNCHECKED :

      sContenuTransfert = "" ;
      sContenuAncien = sContenuTransfert ;
      Clear() ;
      SetText(sContenuTransfert.c_str()) ;
      break ;
  }
}

bool
NSComboClassif::canWeClose()
{
  return true ;
}

void
NSComboClassif::CBSelChange()
{
  Message Msg ;

  if (!ActiveToi)
  {
    ActiveToi = true ;
    return ;
  }

  int iIdx = GetSelIndex() ;
  char far* sStringSel = new char[1024] ;
  GetString(sStringSel, iIdx) ;
  string sCodeSel;
    /*for (NSEpiClassifInfoIter iter = aClassifList.begin(); ((iter != aClassifList.end())) ; iter++)
       if (string((*iter)->pDonnees->libelle) == string(sStringSel))
        sCodeSel = (*iter)->pDonnees->code;  */
  NSEpiClassifInfoIter iter = aClassifList.begin() ;
  for ( ; (aClassifList.end() != iter) &&
         ((*iter)->getLabel() != string(sStringSel)) ; iter++) ;
  if (aClassifList.end() != iter)
    sCodeSel = (*iter)->getCode() ;

  //if (sContenuAncien != aClassifList[iIdx]->pDonnees->code)
  if (sContenuAncien != sCodeSel)
  {
    if (sContenuAncien != "")
    {
      donneTransfert() ;
      if (string("") == sContenuTransfert)
      {
        sContenuAncien = "" ;
        Msg.SetComplement(sContenuTransfert) ;
        Msg.SetType(Type) ;
        if (pControle && pControle->getTransfert())
          pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                            BF_UNCHECKED,
                                                            &Msg) ;
      }
      else
      {
        sContenuAncien = sContenuTransfert ;
        Msg.SetComplement(sContenuTransfert) ;
        Msg.SetType(Type) ;
        if (pControle && pControle->getTransfert())
          pControle->getTransfert()->ctrlNotification(BF_CHECKED,
                                                            BF_CHECKED,
                                                            &Msg) ;
      }
    }
    else
    {
      donneTransfert() ;
      sContenuAncien = sContenuTransfert ;
      Msg.SetComplement(sContenuTransfert) ;
      Msg.SetType(Type) ;
      if (pControle && pControle->getTransfert())
        pControle->getTransfert()->ctrlNotification(BF_UNCHECKED,
                                                            BF_CHECKED,
                                                            &Msg) ;
    }
  }
}

