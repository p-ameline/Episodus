//**************************************************************************//  NOM du FICHIER  : nstrnode.CPP
//  Kaddachi Hafedh 20/03/1997
//**************************************************************************
#include <owl/treewind.h>
//#include <owl/color.h>
#include <winuser.h>
#include <limits.h>

#include "partage\nsdivfct.h"
#include "nsbb\nsbbtran.h"
#include "nsbb\nsbbitem.h"
#include "nsbb\nsbb.h"
#include "nssavoir\nspatho.h"
#include "nssavoir\nsencycl.h"
#include "nssavoir\nsguide.h"
#include "nsbb\nStlibre.h"
#include "nsbb\nsbb_dlg.h"
#include "nsbb\nspardlg.h"
#include "nsbb\nsedcon.h"
#include "nsbb\nstrnode.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsbb.rh"

#include "dcodeur\decoder.h"

#if !defined(OWL_LISTWIND_H)
# include <owl/listwind.h>
#endif

long NSTreeNode::lObjectCount = 0 ;

//-----------------------------------------------------------------------
// fonction globale de tri : selon la ligne
//-----------------------------------------------------------------------
bool
sortTreenodes(NSTreeNode* Node1, NSTreeNode* Node2)
{
	if (Node1->getLigne() < Node2->getLigne())
		return true ;
	return false ;
}

//***************************************************************************
// 							Implémentation des méthodes VecteurChaine
//***************************************************************************
VecteurChaine::~VecteurChaine()
{
	vider() ;
}

VecteurChaine::VecteurChaine()
{}

void
VecteurChaine::vider()
{
	if (empty())
		return ;

	IterString i = begin() ;
	while (end() != i)
	{
  	delete *i ;
    erase(i) ;
	}
}

VecteurChaine&
VecteurChaine::operator=(const VecteurChaine& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;
	if (false == src.empty())
		for (IterStringConst i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (VecteurChaine operator=)", standardError, 0);
    return *this;
}
}

VecteurChaine::VecteurChaine(const VecteurChaine& src)
{
try
{
	if (false == src.empty())
		for (IterStringConst i = src.begin() ; src.end() != i ; i++)
			push_back(new string(*(*i))) ;
}
catch (...)
{
	erreur("Exception (VecteurChaine copy ctor)", standardError, 0) ;
}
}

//---------------------------------------------------------------------------
//  vecteur des fils
//
//  Ce vecteur n'héberge que des pointeurs qui ne lui appartiennent pas :
//  il ne faut pas faire de delete ni de new
//
//  vector of sons
//
//  This vector only stores pointers that he doesn't own by itself :
//  don't make any 'new' or 'delete'
//---------------------------------------------------------------------------
void
NSFilsTreeNodeArray::vider()
{
	if (false == empty())
		for (iterNSTreeNode i = begin() ; end() != i ; )
			erase(i) ;
}

NSFilsTreeNodeArray::~NSFilsTreeNodeArray()
{
	vider() ;
}

NSFilsTreeNodeArray::NSFilsTreeNodeArray(const NSFilsTreeNodeArray& src)
                    :NSTreeNodeArray()
{
try
{
	if (false == src.empty())
		for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception (NSFilsTreeNodeArray copy ctor)", standardError, 0) ;
}
}

NSFilsTreeNodeArray&
NSFilsTreeNodeArray::operator=(const NSFilsTreeNodeArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
			push_back(*i) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (NSFilsTreeNodeArray operator=)", standardError, 0) ;
	return *this ;
}
}

void
NSFilsTreeNodeArray::Referencer(NSTreeNode* pNewNSTreeNode, NSTreeNode* pNSTreeNodeFrere)
{
try
{
  if ((NSTreeNode*) NULL == pNewNSTreeNode)
    return ;

	// Insertion en queue de list
	// Insert as last element
	//
	if (((NSTreeNode*) NULL == pNSTreeNodeFrere) || empty())
  {
		push_back(pNewNSTreeNode) ;
    return ;
  }

	//
	// Insertion avant pNSTreeNodeFrere
	// Insert before pNSTreeNodeFrere
	//
  iterNSTreeNode i = begin() ;
  for (; (end() != i) && (*i != pNSTreeNodeFrere) ; i++) ;
  if (end() != i)
    insert(i, pNewNSTreeNode) ;
}
catch (...)
{
	erreur("Exception (NSFilsTreeNodeArray Referencer)", standardError, 0) ;
}
}

// We don't change line number here (secondary pointer storage)
//
void
NSFilsTreeNodeArray::RemoveElement(NSTreeNode* pNSTreeNodeSuppr)
{
	if (empty() || ((NSTreeNode*) NULL == pNSTreeNodeSuppr))
		return ;

	iterNSTreeNode iterFils = begin() ;
  for (; (end() != iterFils) && (*iterFils != pNSTreeNodeSuppr) ; iterFils++) ;

	if (end() == iterFils)
    return ;

  erase(iterFils) ;
}

//---------------------------------------------------------------------------//  vecteur des frères liés
//
//  Ce vecteur n'héberge que des pointeurs qui ne lui appartiennent pas :
//  il ne faut pas faire de delete ni de new
//
//  vector of linked brothers
//
//  This vector only stores pointers that he doesn't own by itself :
//  don't make any 'new' or 'delete'
//---------------------------------------------------------------------------
void
NSFrereTreeNodeArray::vider()
{
	if (false == empty())
		for (iterNSTreeNode i = begin() ; end() != i ; )
			erase(i) ;
}

NSFrereTreeNodeArray::~NSFrereTreeNodeArray()
{
	vider() ;
}

NSFrereTreeNodeArray::NSFrereTreeNodeArray(const NSFrereTreeNodeArray& src)
                     :NSTreeNodeArray()
{
try
{
	if (false == src.empty())
		for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
			push_back(*i) ;
}
catch (...)
{
	erreur("Exception (NSFrereTreeNodeArray copy ctor)", standardError, 0);
}
}

NSFrereTreeNodeArray&
NSFrereTreeNodeArray::operator=(const NSFrereTreeNodeArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
			push_back(*i) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (NSFrereTreeNodeArray operator=)", standardError, 0) ;
	return *this ;
}
}

// We don't change line number here (secondary pointer storage)
//
void
NSFrereTreeNodeArray::RemoveElement(NSTreeNode* pNSTreeNodeSuppr)
{
	if ((empty()) || ((NSTreeNode*) NULL == pNSTreeNodeSuppr))
		return ;

	iterNSTreeNode iterFils = begin() ;
  for ( ; (end() != iterFils) && (*iterFils != pNSTreeNodeSuppr) ; iterFils++) ;

	if (end() == iterFils)
    return ;

  erase(iterFils) ;
}

//--------------------------------------------------------------------------
//tableau de tous les NSTreeNode
//--------------------------------------------------------------------------
NSTreeNodeArray::NSTreeNodeArray(const NSTreeNodeArray& src)
                :NSNSTreeNodeArray()
{
try
{
	if (false == src.empty())
  	for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSTreeNode(*(*i))) ;
}
catch (...)
{
	erreur("Exception (NSTreeNodeArray copy ctor)", standardError, 0);
}
}

NSTreeNodeArray&
NSTreeNodeArray::operator=(const NSTreeNodeArray& src)
{
try
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
  	for (iterNSTreeNodeConst i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSTreeNode(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception (NSTreeNodeArray copy ctor)", standardError, 0) ;
	return *this ;
}
}

//---------------------------------------------------------------------------
//  Vidange de l'array
//---------------------------------------------------------------------------
void
NSTreeNodeArray::vider()
{
	if (empty())
		return ;

	for (iterNSTreeNode i = begin() ; end() != i ; )
	{
  	delete *i ;
    erase(i) ;
	}
}

void
NSTreeNodeArray::eraseAll()
{
	if (empty())
		return ;

	for (iterNSTreeNode i = begin() ; end() != i ; )
    erase(i) ;
}

NSTreeNodeArray::~NSTreeNodeArray()
{
	vider() ;
  // eraseAll() ;
}

//enlever pNSTreeNodeSuppr de l'array
void
NSTreeNodeArray::RemoveElement(NSTreeNode* pNSTreeNodeSuppr)
{
	if ((empty()) || ((NSTreeNode*) NULL == pNSTreeNodeSuppr))
		return ;

	iterNSTreeNode iterFils = begin() ;
  for ( ; (end() != iterFils) && (*iterFils != pNSTreeNodeSuppr) ; iterFils++) ;

	if (end() == iterFils)
    return ;

  int iLineNb = (*iterFils)->getLigne() ;

  erase(iterFils) ;

  ShiftLines(iLineNb + 1, -1) ;
}

void
NSTreeNodeArray::AddElement(NSTreeNode* pNSTreeNodeAdd)
{
  if ((NSTreeNode*) NULL == pNSTreeNodeAdd)
		return ;

  // Adding the same node twice may lead to awfull bugs ; check it
  //
  if (false == empty())
  {
    iterNSTreeNode iterFils = begin() ;
    for ( ; (end() != iterFils) && (*iterFils != pNSTreeNodeAdd) ; iterFils++) ;
    if (end() != iterFils)
      return ;
  }

  int iNewNodeLine = pNSTreeNodeAdd->getLigne() ;

  if (Contains(iNewNodeLine))
    ShiftLines(iNewNodeLine, 1, false) ;

  push_back(pNSTreeNodeAdd) ;

  Sort() ;
}

//--------------------------------------------------------------------------
// rechercher dans ce vecteur s'il y a un NStreeNode dont le code(sIdentite)
//est sEtiquette
//--------------------------------------------------------------------------
bool
NSTreeNodeArray::Contains(string sEtiquette)
{
	if (empty())
		return false ;

	iterNSTreeNode iterFils = begin() ;

	for ( ; (end() != iterFils) && ((*iterFils)->getEtiquette() != sEtiquette); iterFils++) ;
	if (end() != iterFils)
		return true ;

	return false ;
}

bool
NSTreeNodeArray::Contains(int iLine)
{
	if (empty())
		return false ;

	iterNSTreeNode iterFils = begin() ;

	for ( ; (end() != iterFils) && ((*iterFils)->getLigne() != iLine); iterFils++) ;
	if (end() != iterFils)
		return true ;

	return false ;
}

NSTreeNode*
NSTreeNodeArray::GetNode(int iLine)
{
	if (empty())
		return (NSTreeNode*) 0 ;

	iterNSTreeNode iterFils = begin() ;

	for ( ; (end() != iterFils) && ((*iterFils)->getLigne() != iLine); iterFils++) ;
	if (end() != iterFils)
		return *iterFils ;

	return (NSTreeNode*) 0 ;
}

//--------------------------------------------------------------------------
// Shift iShiftCount lines starting from line iLineFrom (and sort)
//--------------------------------------------------------------------------
void
NSTreeNodeArray::ShiftLines(int iLineFrom, int iShiftCount, bool bSortAfter)
{
	if (empty())
		return ;

	for (iterNSTreeNode iterFils = begin() ; end() != iterFils ; iterFils++)
    if ((*iterFils)->getLigne() >= iLineFrom)
      (*iterFils)->setLigne((*iterFils)->getLigne() + iShiftCount) ;

  if (bSortAfter)
    Sort() ;
}

void
NSTreeNodeArray::Sort()
{
  if (empty())
		return ;

  sort(begin(), end(), sortTreenodes) ;
}

//----------------------------------------------------------------------
//----------------------------------------------------------------------
//									 Classe NSTreeNode
//----------------------------------------------------------------------
//----------------------------------------------------------------------
NSTreeNode::NSTreeNode(TTreeWindow& treeWind, NSContexte* pCtx, HTREEITEM hItem)
           :TTreeNode(treeWind, hItem), NSRoot(pCtx)
{
  lObjectCount++ ;

	initialiser() ;
}

NSTreeNode::NSTreeNode(TTreeWindow& treeWind, NSContexte* pCtx, string text)
           :TTreeNode(treeWind, text.c_str()), NSRoot(pCtx)
{
  lObjectCount++ ;

	initialiser() ;
}

NSTreeNode::NSTreeNode(const TTreeNode& treeNode, TNextCode nextCode, NSContexte* pCtx)
           :TTreeNode(treeNode, nextCode), NSRoot(pCtx)
{
  lObjectCount++ ;

	initialiser() ;
}

NSTreeNode::NSTreeNode(const TTreeNode& treeNode, NSContexte* pCtx)
           :TTreeNode(treeNode), NSRoot(pCtx)
{
  lObjectCount++ ;

	initialiser() ;
}

void
NSTreeNode::initialiser()
{
	_ligne			       = 0 ;
	_colonne			     = 0 ;

  // memset(_szType, 0, 25) ;

	_pControle 		     = (NSControle*) 0 ;
	_sNoeud 	  		   = string("") ;
	_sIdentite 		     = string("") ;
	_sUnit             = string("") ;
	_sLabel 	  		   = string("") ;
	_code 	  		     = string("") ;
	_sVisible  		     = string("1") ;
	_FrerePilote 	     = (NSTreeNode*) 0 ;
  _pere              = (NSTreeNode*) 0 ;
	_sTexteGlobal 	   = string("") ;
	_sInteret 		     = string("A") ;
	_sCertitude 		   = string("") ;
	_sPluriel 		     = string("") ;
	_sNodeRights		   = string("") ;
	_Absence 		       = false ;
	_estPropose 		   = false ;
	_CreateurChampEdit = false ;
	_DateHeure 		     = false ;
	_pParametre 		   = (Parametre*) 0 ;
	_bRecalcul 		     = false ;
	_bEncyclop 		     = false ;
  _bCertif   		     = false ;
	_sUrl 			       = string("") ;
  _sUrlCertif 			 = string("") ;
	_bOuvert 		       = false ;
	_pDocument 		     = 0 ;
	_pTemporaryLinks   = (NSLinkedNodeArray*) 0 ;
}

NSTreeNode::~NSTreeNode()
{
	if (_pControle)
  {
    _pControle->setControle((void*) 0) ;
		delete _pControle ;
  }

	if (_pParametre)
		delete _pParametre ;
	if (_pTemporaryLinks)
		delete _pTemporaryLinks ;

  lObjectCount-- ;
}

//---------------------------------------------------------------------------
//  Description:	Constructeur copie
//---------------------------------------------------------------------------
NSTreeNode::NSTreeNode(const NSTreeNode& src)
           :TTreeNode(src), NSRoot(src.pContexte)
{
	// strcpy(_szType ,src._szType) ;

	_pControle      	 = src._pControle ;
	_bEncyclop 		     = src._bEncyclop ;
  _bCertif   		     = src._bCertif ;
	_sUrl 			       = src._sUrl ;
  _sUrlCertif 			 = src._sUrlCertif ;
	_VectFrereLie	     = src._VectFrereLie ;
	_VectFils 		     = src._VectFils ;
	_sContenuBrut  	   = src._sContenuBrut ;
	_sContenuTransfert = src._sContenuTransfert ;
	_Type  			       = src._Type ;
	_sNoeud            = src._sNoeud ;
	_sIdentite		     = src._sIdentite ;
	_sUnit             = src._sUnit ;
	_code			         = src._code ;
	_sTexteGlobal	     = src._sTexteGlobal ;
	_sInteret		       = src._sInteret ;
	_sPluriel		       = src._sPluriel ;
	_sCertitude		     = src._sCertitude ;
	_sNodeRights		   = src._sNodeRights ;
	_estPropose		     = src._estPropose ;
	_CreateurChampEdit = src._CreateurChampEdit ;
	_DateHeure 		     = src._DateHeure ;	_pParametre		     = src._pParametre ;
	_sVisible		       = src._sVisible ;
	_bRecalcul 		     = src._bRecalcul ;
}

//---------------------------------------------------------------------------//  Description:	Surcharge de l'opérateur d'affectation
//---------------------------------------------------------------------------
NSTreeNode&
NSTreeNode::operator=(const NSTreeNode& src)
{
	if (this == &src)
		return *this ;

  pContexte  		     = src.pContexte ;

	// strcpy(_szType, src._szType) ;

	_pControle  		   = src._pControle ;
  _bEncyclop 		     = src._bEncyclop ;
  _bCertif  		     = src._bCertif ;
  _sUrl 			       = src._sUrl ;
  _sUrlCertif 			 = src._sUrlCertif ;
  _pParametre		     = src._pParametre ;
  _VectFrereLie	     = src._VectFrereLie ;
  _VectFils   		   = src._VectFils ;
	_sContenuBrut  	   = src._sContenuBrut ;
  _sContenuTransfert = src._sContenuTransfert ;
  _Type  			       = src._Type ;
  _sNoeud            = src._sNoeud ;
  _sIdentite		     = src._sIdentite ;
  _sUnit             = src._sUnit ;
  _code			         = src._code ;
  _sTexteGlobal	     = src._sTexteGlobal ;
  _sInteret		       = src._sInteret ;
	_sPluriel		       = src._sPluriel ;
  _sCertitude		     = src._sCertitude ;
  _sNodeRights		   = src._sNodeRights ;
  _estPropose		     = src._estPropose ;
  _CreateurChampEdit = src._CreateurChampEdit ;
  _DateHeure 		     = src._DateHeure ;
  _sVisible		       = src._sVisible ;
  _bRecalcul 		     = src._bRecalcul ;

	return *this ;
}

//----------------------------------------------------------------------
//  transmetre les paramètres au NStreeNode après OK
//----------------------------------------------------------------------
void
NSTreeNode::RecupereParametre()
{
	Message* pMessage = getTransfertMessage() ;
	if ((Parametre*) NULL == _pParametre)
  	_pParametre = new Parametre(0,"NSTREENODE_PARAMETRE", pMessage, this, pNSDLLModule) ;

	if (_pParametre->Execute() == IDOK)
	{
  	fixeInteret(_pParametre->sInteretDlg) ;
    fixePluriel(_pParametre->sPlurielDlg) ;
    fixeCertitude(_pParametre->sCertitudeDlg) ;
	}
	_pParametre = (Parametre*) 0 ;
}

voidNSTreeNode::fixeInteret(string sNewInteret)
{
	if (estFictif() || estLie() || _estPropose)
		return ;

	// Si valeur chiffrée avec un seul fils créateur de champ numérique
	if ((pContexte->getDico()->CodeCategorie(_sIdentite) == "K") ||
      (pContexte->getDico()->CodeCategorie(_sIdentite) == "V"))
	{
  	if (_VectFils.size() == 1)
    {
    	NSTreeNode* pFils = *(_VectFils.begin()) ;
      if (pFils && (pFils->isCreateurChampEdit() || pFils->isDateTime()))
      {
      	pFils->_sInteret = sNewInteret ;
        pFils->Interet() ;
        return ;
      }
    }
  }
  _sInteret = sNewInteret ;
  Interet() ;
}

void
NSTreeNode::fixeCertitude(string sNewCertitude)
{
	if ((estFictif() || estLie()) && (false == _estPropose))
		return ;

	_sCertitude = sNewCertitude ;
	string sChaine = Certitude() ;

	if (string("ABSENCE") != sChaine)
	{
  	if (_Absence == true)
    {
    	size_t pos  = _sLabel.find(string("Absence de")) ;
      if (NPOS != pos)
      	_sLabel = string(_sLabel, pos + 11 , strlen(_sLabel.c_str())) ;
      else
      {
      	pos = _sLabel.find(string("Absence d'")) ;
        if (NPOS != pos)
        	_sLabel = string(_sLabel, pos + 10 , strlen(_sLabel.c_str())) ;
      }
      _Absence = false ;
    }
    if (string("PRESENCE") != sChaine)
    	_sLabel = sChaine + _sLabel ;
  }
  else
  	_Absence = true ;

	SetText(_sLabel.c_str()) ;
}

voidNSTreeNode::fixePluriel(string sNewPluriel)
{
	_sPluriel = sNewPluriel ;
}

string
NSTreeNode::getNodeId()
{
  Message* pMessage = getTransfertMessage() ;
  if ((Message*) NULL == pMessage)
    return string("") ;

  return pMessage->GetTreeID() + pMessage->GetNoeud() ;
}

//---------------------------------------------------------------------------
//  Function: 		NSTreeNode::activeControle(int activation, Message* pMessage)
//
//  Arguments:		activation : BF_CHECKED , BF_UNCHECKED ou BF_DELETE
//						message    : Contenu de la boîte
//
//  Returns:		Rien
//---------------------------------------------------------------------------
void
NSTreeNode::activeControle(int activation, Message* pMessage)
{
	switch (activation)
	{
  	case BF_CHECKED   : _sContenuTransfert = pMessage->GetComplement() ;               			    _sNoeud 		       = pMessage->GetNoeud() ;
                        _sInteret 		  	 = pMessage->GetInteret() ;
                        _sPluriel 		  	 = pMessage->GetPluriel() ;
                        _sCertitude 		   = pMessage->GetCertitude() ;
                        _Type 			  	   = pMessage->GetType() ;
                        _sVisible          = pMessage->GetVisible() ;
                        _sNodeRights		   = pMessage->GetNodeRight() ;
                        SetText(_sLabel.c_str()) ;
                        getMessageIds(pMessage) ;
                        break ;

		case BF_UNCHECKED : _sContenuTransfert = string("") ;
               			 	  _Type 			  	   = string("") ;
                        _sNoeud 		       = string("") ;
                   			_sInteret 		  	 = string("") ;
                   			_sPluriel 		  	 = string("") ;
                   			_sCertitude 		   = string("") ;
                        _sVisible          = string("") ;
                        _sNodeRights			 = string("") ;
                        SetText(_sLabel.c_str()) ;
                        break ;
    case BF_DELETE	  : _sContenuTransfert = string("") ;
                        _Type 			  	   = string("") ;
                        _sNoeud 		       = string("") ;
                   			_sInteret 		  	 = string("") ;
                   			_sPluriel 		  	 = string("") ;
                   			_sCertitude 		   = string("") ;
                        _sVisible          = string("") ;
                        _sNodeRights			 = string("") ;
                        Delete() ;
                        break ;
	}
}

//--------------------------------------------------------------------------// élément fictif ou pas
//--------------------------------------------------------------------------
bool
NSTreeNode::estFictif()
{
 	if (_pControle)
 		return false ;
 	return true ;
}

//--------------------------------------------------------------------------
//élément lié
//--------------------------------------------------------------------------
bool
NSTreeNode::estLie()
{
	if (_FrerePilote)
  	return true ;
 	return false ;
}

//--------------------------------------------------------------------------
//élément lié sans être texte libre
//--------------------------------------------------------------------------
bool
NSTreeNode::estFictifPur()
{
 	if ((false == estLie()) && (estFictif()))
 		return true ;
 	return false ;
}

//--------------------------------------------------------------------------
// Retourne true si le noeud contient un document dans pDocument
//--------------------------------------------------------------------------
bool
NSTreeNode::estUnDocument()
{
	if (_pDocument)
  	return true ;

	return false ;
}

// Activating FilsItem
//
void
NSTreeNode::activateFilsItem()
{
  BBFilsItem* pNewNodeFilsItem = getBBFilsItem() ;
  if (pNewNodeFilsItem)
    pNewNodeFilsItem->estActif = true ;

  NSTransferInfo* pTransferInfo = getTransfert() ;
  if (pTransferInfo)
    pTransferInfo->Active() ;
}

//---------------------------------------------------------------------------
//  Function: 	NSTreeNode::Transfer(TTransferDirection direction,
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
uint
NSTreeNode::Transferer(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (string("") == _sIdentite)
		return 0 ;

	if (tdSetData == direction) //valeurs données par BBitem
	{
  	switch (*pActif)
		{
    	case inactiveCtrl : _sContenuTransfert = string("") ;
                          _Type              = string("") ;
                          _sNoeud 		       = string("") ;
                          _sUnit             = string("") ;
                          _sInteret 		     = string("") ;
                          _sPluriel 		     = string("") ;
                          _sCertitude 		   = string("") ;
                          _sVisible		       = string("") ;
                          _sNodeRights			 = string("") ;
                          break ;

			case activeCtrl :   _sContenuTransfert = pMessage->GetComplement() ;
                          _sInteret 		     = pMessage->GetInteret() ;
                          _sUnit             = pMessage->GetUnit() ;
                          _sPluriel 		     = pMessage->GetPluriel() ;
                          _sCertitude 		   = pMessage->GetCertitude() ;
                          _Type 			       = pMessage->GetType() ;
                          _sVisible		       = pMessage->GetVisible() ;
                          _sNodeRights			 = pMessage->GetNodeRight() ;
                          SetText(_sLabel.c_str()) ;
                          getMessageIds(pMessage) ;
                          break ;
    }
  }	else if (tdGetData == direction) //données envoyées au BBitem
	{
  	if ((NSControle*) NULL == _pControle)
			*pActif = inactiveCtrl ;
		else
			*pActif = activeCtrl ;

    if (pMessage)
    {
    	pMessage->SetInteret(_sInteret) ;
      pMessage->SetLexique(_sIdentite) ;
      pMessage->SetUnit(_sUnit) ;
      pMessage->SetPluriel(_sPluriel) ;
      pMessage->SetCertitude(_sCertitude) ;
      pMessage->SetComplement(_sContenuTransfert) ;
      pMessage->SetType(_Type) ;
      pMessage->SetVisible(_sVisible) ;
      pMessage->SetNodeRight(_sNodeRights) ;
      setMessageIds(pMessage) ;

      pMessage->SetTemporaryLinks(_pTemporaryLinks) ;
    }
	}
	return 1 ;
}

uint
NSTreeNode::TempTransferer(CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (string("") == _sIdentite)
  	return 0 ;

	if ((NSControle*) NULL == _pControle)
  	*pActif = inactiveCtrl ;
	else
  	*pActif = activeCtrl ;

	if (pMessage)
	{
    pMessage->SetInteret(_sInteret) ;
    pMessage->SetLexique(_sIdentite) ;
    pMessage->SetUnit(_sUnit) ;
    pMessage->SetNoeud(_sNoeud) ;
    pMessage->SetPluriel(_sPluriel) ;
    pMessage->SetCertitude(_sCertitude) ;
    pMessage->SetComplement(_sContenuTransfert) ;
    pMessage->SetType(_Type) ;
    pMessage->SetVisible(_sVisible) ;
    pMessage->SetNodeRight(_sNodeRights) ;
    setMessageIds(pMessage) ;

    pMessage->SetTemporaryLinks(_pTemporaryLinks) ;
	}
	return 1 ;
}

//-----------------------------------------------------------------------//insertion d'un texte libre
//-----------------------------------------------------------------------
int
NSTreeNode::InsererTexteLibre(Message* pMessage)
{
	if (string("£?????") != _sIdentite)
		return 1 ;

	pMessage->SetTexteLibre(_sTexteGlobal) ;
	return 1 ;

/*
    bool resultatBD;
    string sTexteLibre;
	NSTlibre* pNSTlibre = new NSTlibre(pContexte);
    if (code == string("")) //code vide
    {
   	    if (!(sTexteGlobal == string(""))) //texte libre tapé
        {
      	    resultatBD = pNSTlibre->InsereTexte(&code, sTexteGlobal);
            if (resultatBD == false)
            {
         	    erreur("Erreur Insertion texte libre.", standardError, 0) ;
                delete pNSTlibre;
                pNSTlibre = 0;
                return 0;
            }
            sContenuTransfert = code;
            pMessage->SetComplement(code);
        }
    }
    else  //MAJ du texte libre
    {
   	    bool texteRecupere = pNSTlibre->RecupereTexte(code, &sTexteLibre);
        if (texteRecupere == false)
        {
      	    erreur("Erreur Récupération texte libre.", standardError, 0) ;
            delete pNSTlibre;
            return 0;
        }
        if (!(sTexteLibre == sTexteGlobal))  //texte a été modifié
        {
      	    resultatBD = pNSTlibre->MettreFicheAJour(code, sTexteGlobal);
            if (resultatBD == false)
            {
         	    erreur("Erreur Mise à jour texte libre.", standardError, 0) ;
                delete pNSTlibre;
                pNSTlibre = 0;
                return 0;
            }
        }
    }
    delete pNSTlibre;
    pNSTlibre = 0;
    return 1; */
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
uint
NSTreeNode::TransfererFinal(TTransferDirection direction, CTRL_ACTIVITY* pActif, Message* pMessage)
{
	if (string("") == _sIdentite)
		return 0 ;

	if (tdSetData == direction) //valeurs données par BBitem
	{
  	switch (*pActif)
		{
    	case inactiveCtrl : _sContenuTransfert = string("") ; //complément
                          _Type              = string("") ;
                          break ;

			case activeCtrl   : _sContenuTransfert = pMessage->GetComplement() ;
                          _sInteret 		  	 = pMessage->GetInteret() ;
                          _sPluriel 		  	 = pMessage->GetPluriel() ;
                          _sCertitude 		   = pMessage->GetCertitude() ;
                          _Type 			  	   = pMessage->GetType() ;
                          _sVisible		  	   = pMessage->GetVisible() ;
                          _sNodeRights       = pMessage->GetNodeRight() ;
                          getMessageIds(pMessage) ;
                          SetText(_sLabel.c_str()) ;
                          break ;
    }
  }
	else if (tdGetData == direction) //données envoyées au BBitem
	{
  	//donneTransfert();
		if ((NSControle*) NULL == _pControle)
    	*pActif = inactiveCtrl ;
		else
			*pActif = activeCtrl ;

    string sTexteLibre ;

		pMessage->SetComplement(_sContenuTransfert) ;
    pMessage->SetType(_Type) ;
    pMessage->SetInteret(_sInteret) ;
    pMessage->SetPluriel(_sPluriel) ;
    pMessage->SetCertitude(_sCertitude) ;
    pMessage->SetLexique(_sIdentite) ;
    pMessage->SetVisible(_sVisible) ;
    pMessage->SetNodeRight(_sNodeRights) ;
    setMessageIds(pMessage) ;

		return InsererTexteLibre(pMessage) ;
	}
	return 1 ;
}

//-------------------------------------------------------------------------
// décaler les lignes des fils et des petits fils de Decalageligne
//-------------------------------------------------------------------------
void
NSTreeNode::MettreAJourLigne(int Decalageligne)
{
	if (_VectFils.empty())
		return ;
	//MAJ des coordonnées des fils et des petits fils de pNSTreeNode
	for (iterNSTreeNode iter = _VectFils.begin(); _VectFils.end() != iter ; iter++)
	{
		(*iter)->setLigne((*iter)->getLigne() + Decalageligne) ; // Decalageligne : éventuellement un fils de plus
    (*iter)->MettreAJourLigne(Decalageligne) ;
	}
}

//-------------------------------------------------------------------------
// savoir si le NSTreeNode est descendant de pNSTreeNode
//-------------------------------------------------------------------------
bool
NSTreeNode::Descendant(NSTreeNode* pNSTreeNode)
{
	if (_pere)
	{
  	if (_pere == pNSTreeNode)
    	return true ;

    return _pere->Descendant(pNSTreeNode) ;
	}
	return false ;
}

//------------------------------------------------------------------------// MAJ étiquette : exemple "grippe" devient "Absence de grippe"
//                          "intensité" devient "Absence d'intensité"
//------------------------------------------------------------------------
void
NSTreeNode::MettreMonEtiquetteAJour()
{
	bool autrecas = true ;
	switch (_sLabel[0])
	{
  	case 'È' ://200
    case 'É' :
    case 'Ê' :
    case 'Ë' :
    case 'é' :
		case 'e' :
		case 'E' :
		case 'ë' :
		case 'è' :
		case 'ê' :  _sLabel = string("Absence d'") + _sLabel ;
         				autrecas = false ;
                break ;
    case 'À' :
    case 'Á' :
    case 'a' :
    case 'A' :
    case 'Â' :
    case 'Ã' :
    case 'Ä' :
    case 'Å' : //197
    case 'Æ' :
    case 'à' : //226
    case 'â' :
    case 'ã' :
    case 'ä' :
    case 'å' :
    case 'æ' :
    case 'á' :  _sLabel = string("Absence d'") + _sLabel ;
         				autrecas = false ;
                break ;

    case 'Ì' :
    case 'Í' :
    case 'Î' :
    case 'Ï' :
    case 'í' :
    case 'i' :
    case 'I' :
    case 'î' :
    case 'ï' :  _sLabel = string("Absence d'") + _sLabel ;
                autrecas = false ;
                break ;

    case 'Ü' ://220
    case 'Û' :
    case 'Ú' :
    case 'Ù' : //216
    case 'ú' :
    case 'û' :
    case 'u' :
    case 'U' :
    case 'ü' :
    case 'ù' :  _sLabel = string("Absence d'") + _sLabel ;
                autrecas = false ;
                break ;

  	case 'Õ' : //213
    case 'Ö' :
		case 'Ô' :
		case 'o' :
    case 'O' :
    case 'Ó' :
    case 'Ò' :
    case 'ò' : //242
    case 'ó' :
    case 'ô' :
    case 'õ' :
    case 'ö' : 	_sLabel = string("Absence d'") + _sLabel ;
         				autrecas = false ;
                break ;

    case 'Ý' :
    case 'y' :
    case 'Y' :
    case 'ÿ' :  _sLabel = string("Absence d'") + _sLabel ; //209
         				autrecas = false ;
                break ;
	}

	if (autrecas)
		_sLabel = string("Absence de ") + _sLabel ;
}

//------------------------------------------------------------------------//------------------------------------------------------------------------
void
NSTreeNode::DicoKillFocus(string* /* pContenu */, string NewType)
{
	int 	  iNotifier = 1;
	Message theMessage(string("")) ;
	//
	// Si une fonction est attachée au contrôle, on l'exécute
	//
	if (false == estFictif())
  	if (_pControle->getFonction())
    	iNotifier = _pControle->getFonction()->execute(NSDLGFCT_EXECUTE) ;

  if (1 == iNotifier)
  {
    _Type = NewType ;
    theMessage.SetType(NewType) ;

    theMessage.SetPluriel(_sPluriel) ;
    theMessage.SetInteret(_sInteret) ;
    theMessage.SetCertitude(_sCertitude) ;
    theMessage.SetComplement(_sContenuTransfert) ;
    theMessage.SetLexique(_sIdentite) ;
    theMessage.SetVisible(_sVisible) ;
    theMessage.SetNodeRight(_sNodeRights) ;
    setMessageIds(&theMessage) ;

    setTransfertMessage(theMessage) ;
  }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
void
NSTreeNode::ctrlNotification()
{
	if (false == estFictif())
  	if (_pControle->getTransfert())
      // Si fils prolongeable, le travail est fait après la récupération
      // des données dans BD
      //if(!pControle->pTransfert->pBBFilsItem->FilsProlongeable)
      _pControle->getTransfert()->ctrlNotification(BF_CHECKED, BF_CHECKED,
                        _pControle->getTransfert()->getTransfertMessage()) ;
}

//------------------------------------------------------------------------
//this se développe s'il est visible puis développe ses fils
//qui eux mêmes développent récursivement leurs fils
//------------------------------------------------------------------------
void
NSTreeNode::ExpandTousNoeud()
{
	if (_VectFils.empty())
		return ;

  iterNSTreeNode fils = _VectFils.begin() ;
  for ( ; _VectFils.end() != fils ; fils++)
  	(*fils)->ExpandTousNoeud() ;

  if (string("1") == _sVisible)
  	ExpandItem(TVE_EXPAND) ;
}

//------------------------------------------------------------------------//this "collapse" ses fils qui eux mêmes "collapse" récursivement leurs fils
//------------------------------------------------------------------------
void
NSTreeNode::CollapseTousNoeud()
{
	if (_VectFils.empty())
		return ;

  iterNSTreeNode fils = _VectFils.begin() ;
  for( ; _VectFils.end() != fils ; fils++)
    (*fils)->CollapseTousNoeud() ;

  ExpandItem(TVE_COLLAPSE) ;
}

//----------------------------------------------------------------------//développer ou non un TReeNode ( + ou -)
//pour chaque fils , lui mettre à jour son affichage en tenant compte des
//étiquettes de ses fils et de son champ sVisible
//----------------------------------------------------------------------
void
NSTreeNode::InitialiseAffichage()
{
	//
	// Nouveau
	//
	NSCutPaste CP(pContexte->getSuperviseur()) ;
	formerPatPatho(&CP) ;
	//
	// Ancien
	//
  if (_VectFils.empty())
		return ;

	iterNSTreeNode GrandFils = _VectFils.begin() ;
	for ( ; _VectFils.end() != GrandFils ; GrandFils++)
	{
  	string sLabel = (*GrandFils)->getLabelString() ;
    if (((*GrandFils)->_sVisible == "0") && (false == (*GrandFils)->_VectFils.empty()))
    {
    	iterNSTreeNode fils = (*GrandFils)->_VectFils.begin() ;
      //
      // Si le fils est lui même père, mettre "..." devant son étiquette
      //
      string sSuite = "";
      if (false == (*fils)->_VectFils.empty())
        sSuite = "...";
      else
        sSuite = "";
      //
      // Il s'agit d'une valeur chiffrée
      //
      if ((*fils)->_CreateurChampEdit)
      {
        if(((*fils)->_sLabel.find("<") != NPOS) || ((*fils)->_sLabel.find(">") != NPOS))
        	sLabel += " " + (*fils)->_sLabel ;
        else
        	sLabel += " : " + (*fils)->_sLabel ;

        (*GrandFils)->SetText(_sLabel.c_str(), true) ;
      }
      //
      // Il s'agit d'une date ou d'une heure
      //
      else if ((*fils)->_DateHeure)
      {
        _sLabel += " : " + (*fils)->_sLabel ;
        (*GrandFils)->SetText(_sLabel.c_str(), true) ;
      }
      //
      // Noeud standard
      //
      else
      {
      	_sLabel += " (" ;
        while ((*GrandFils)->_VectFils.end() != fils)
        {
          if ((!(*fils)->estLie()) &&((*fils)->_sLabel != ""))
          {
          	if (!(*fils)->estLie())
            {
              string sLabelAffiche = "";
              if ((*fils)->_sTexteGlobal != "") //si texte libre
              {
                //premier " "
                size_t pos = (*fils)->_sLabel.find(" ");
                sLabelAffiche = (*fils)->_sLabel;
                if (pos != NPOS)
                {
                  sLabelAffiche = string((*fils)->_sLabel, 0, pos);
                  sLabelAffiche = sLabelAffiche.c_str() + string("...");
                }
              }
              else
              	sLabelAffiche = (*fils)->_sLabel ;
              //
              // Si le fils est lui même père, mettre "..."
              // devant son étiquette sauf s'il s'agit de
              // texte libre
              //              string sSuite = "" ;
              if ((!(*fils)->_VectFils.empty()) &&
                    (sLabelAffiche.find("...") == NPOS))
              {
              	// Si au moins un fils de (*fils) a un sLabel
                // non vide
                bool continuer = true ;
                iterNSTreeNode petitFils = (*fils)->_VectFils.begin() ;
                while ((petitFils != (*fils)->_VectFils.end()) && continuer)
                {
                	if ((*petitFils)->_sLabel != "")
                  {
                  	sSuite = "..." ;
                    continuer = false ;
                  }
                  else
                  	petitFils++ ;
                }
              }
              else
              	sSuite = "" ;
              if ((*fils)->_sLabel != "")
              {
              	if (*fils == (*GrandFils)->_VectFils.back()) //le dernier
                	_sLabel += sLabelAffiche.c_str() + sSuite + string(")") ;
                else
                	_sLabel += sLabelAffiche.c_str() + sSuite + string(", ") ;
              }
            }
          }
          fils++ ;
        }
      }
      (*GrandFils)->CollapseTousNoeud() ;
      if ((_sLabel != " (") &&(_sLabel != ((*GrandFils)->_sLabel + " ()")))
      	(*GrandFils)->SetText(_sLabel.c_str(), true) ;
    }
    else
    	(*GrandFils)->ExpandTousNoeud() ;

    (*GrandFils)->InitialiseAffichage() ;
	}
}

//------------------------------------------------------------------------//fixer l'interêt
//------------------------------------------------------------------------
void
NSTreeNode::Interet()
{
  if      (_sInteret == string("A"))
  {
    SetImageIndex(0) ;
    SetSelectedImageIndex(0,  true) ;  }
  else if (_sInteret == string("B"))  {
    SetImageIndex(1) ;
    SetSelectedImageIndex(1,  true) ;
  }
  else if (_sInteret == string("C"))
  {
    SetImageIndex(2) ;
    SetSelectedImageIndex(2,  true) ;
  }
  else if (_sInteret == string("D"))
  {
    SetImageIndex(3) ;
    SetSelectedImageIndex(3,  true) ;
  }
  else if (_sInteret == string("E"))
  {
    SetImageIndex(4) ;
    SetSelectedImageIndex(4,  true) ;
  }
}

//------------------------------------------------------------------------//intêret max parmi les descendants
//------------------------------------------------------------------------
void
NSTreeNode::donneIneretMax(string* pInteretMax)
{
	if ((string*) NULL == pInteretMax)
		return ;

  if (false == _VectFils.empty())
  {
  	iterNSTreeNode fils = _VectFils.begin() ;
    for ( ; _VectFils.end() != fils ; fils++)
    {
    	if ((*fils)->_sInteret >= *pInteretMax)
      	*pInteretMax = (*fils)->_sInteret ;
      (*fils)->donneIneretMax(pInteretMax) ;
    }
  }
  if (_sInteret >= *pInteretMax)
  	*pInteretMax = _sInteret ;
}
//------------------------------------------------------------------------//fixer la certitude
//------------------------------------------------------------------------
string
NSTreeNode::Certitude()
{
  //retrouver le vrai label brut
  size_t pos1 = _sLabel.find("improbable ");
  size_t pos2 = _sLabel.find("possible ");
  size_t pos3 = _sLabel.find("probable ");
  if      ((pos1 != NPOS) && (pos1 == 0))
  	_sLabel = string(_sLabel, 11, strlen(_sLabel.c_str())) ;
  else if ((pos2 != NPOS) && (pos2 == 0))
    _sLabel = string(_sLabel, 9, strlen(_sLabel.c_str())) ;
  else if ((pos3 != NPOS) && (pos3 == 0))
  	_sLabel = string(_sLabel, 9, strlen(_sLabel.c_str())) ;

  //absence de : placer "pas de" ou "pad d'" devant le sLabel
  if (string("WCE001") == _sCertitude)
  {
  	if (false == _Absence)
    {
    	MettreMonEtiquetteAJour() ;
      _Absence = true ;
    }
    return string("ABSENCE") ;
  }

  if (_sCertitude == string("WCE251")) //25%
  	return "improbable " ;
  if (_sCertitude == string("WCE501")) //50%
  	return "possible " ;
  if (_sCertitude == string("WCE751")) //75%
  	return "probable " ;
  if (_sCertitude == string("WCE751")) //75%
  	return "probable " ;

  return "PRESENCE" ; //100%
}

//-------------------------------------------------------------------------// 				Présence d'élément lexique
//-------------------------------------------------------------------------
void
NSTreeNode::PresenceLexique()
{
	//enlever "pas de" ou "pad d'" de sLabel
	if (string("") == _sTexteGlobal) //non texte libre
	{
  	if (_Absence)
    {
    	size_t pos  = _sLabel.find(string("Absence de")) ;
      if (pos != NPOS)
      	_sLabel = string(_sLabel, pos + 11 , strlen(_sLabel.c_str())) ;
      else
      {
      	pos = _sLabel.find(string("Absence d'")) ;
        if (pos != NPOS)
        	_sLabel = string(_sLabel, pos + 10 , strlen(_sLabel.c_str())) ;
      }
      SetText(_sLabel.c_str()) ;
      _sCertitude = string("") ;
      _Absence = false ;
    }
	}
}

//-------------------------------------------------------------------------// 				Présence de l'élément lexique dans Encyclop.db
//-------------------------------------------------------------------------
void
NSTreeNode::Encyclop()
{}

//-------------------------------------------------------------------------//affiche en fonction de sUnite le label:
//          date sous la forme JJ/MM/AAAA
//				heure sous la forme HH:MM
//-------------------------------------------------------------------------
void
NSTreeNode::AfficheLabel(string sUnite)
{
	if (string("") == _sLabel)
		return ;

	string Label = _sLabel ;	if ((sUnite == "2DA01") && (Label.find("/") == NPOS)) //date
	{
  	if (strlen(Label.c_str()) < 8)
    	return ;
    Label = string(Label,6,2) + "/" + string(Label,4,2) + "/" + string(Label,0,4) ;
	}
	else if((sUnite == "2HE01") && (Label.find(":") == NPOS)) //heure
	{
  	if (strlen(Label.c_str()) < 4)
    	return ;
    Label = string(Label,0,2) + ":" + string(Label,2,2) ;
	}
	_sLabel = Label ;
}

voidNSTreeNode::InstancieUrl(NSEncyclo* pNSEncyclo)
{
	if ((NSEncyclo*) NULL == pNSEncyclo)
		return ;

	if ((string("") == _sIdentite) || (strlen(_sIdentite.c_str()) > BASE_LEXI_LEN))
		return ;

  string sCodeSens ;
  pContexte->getDico()->donneCodeSens(&_sIdentite, &sCodeSens) ;

  size_t iCodeSensLen = strlen(sCodeSens.c_str()) ;

  if (iCodeSensLen > ENCYCLO_LEXIQUE_LEN - 1)
  	return ;

  //
  // Encyclopédie
  //
  string sEncycloCode = string("E") + sCodeSens ;

  DBIResult Resultat = pNSEncyclo->chercheClef(&sEncycloCode,
                                     "",
                                     0,
                                     keySEARCHGEQ,
                                     dbiWRITELOCK);
  if (DBIERR_NONE != Resultat)
  	_bEncyclop = false ;
  else
  {
  	Resultat = pNSEncyclo->getRecord() ;
    if (DBIERR_NONE != Resultat)
    {
    	erreur("Le fichier des encyclopédie est inutilisable.", standardError, Resultat) ;
      return ;
    }
    else
    {
    	if (string(pNSEncyclo->getLexique(), 1, iCodeSensLen) == sCodeSens)
      {
      	string sUrlLocal = pNSEncyclo->getUrl() ;
        if (string("") != sUrlLocal)
        {
        	_bEncyclop = true ;
          _sUrl = sUrlLocal ;
        }
      }
    }
  }

  //
  // Certificat
  //
  sEncycloCode = string("C") + sCodeSens ;

  Resultat = pNSEncyclo->chercheClef(&sEncycloCode,
                                     "",
                                     0,
                                     keySEARCHGEQ,
                                     dbiWRITELOCK);
  if (DBIERR_NONE != Resultat)
  	_bCertif = false ;
  else
  {
  	Resultat = pNSEncyclo->getRecord() ;
    if (DBIERR_NONE != Resultat)
    {
    	erreur("Le fichier des encyclopédie est inutilisable.", standardError, Resultat) ;
      return ;
    }
    else
    {
      if (string(pNSEncyclo->getLexique(), 1, iCodeSensLen) == sCodeSens)
      {
      	string sUrlLocal = pNSEncyclo->getUrl() ;
        if (string("") != sUrlLocal)
        {
        	_bCertif = true ;
          _sUrlCertif = sUrlLocal ;
        }
      }
    }
  }
}

//-------------------------------------------------------------------------// 				Absence d'élément lexique
//-------------------------------------------------------------------------
void
NSTreeNode::AbsenceLexique()
{
	if ((_sTexteGlobal == string("")) && (false == _Absence)) //non texte libre
	{
		MettreMonEtiquetteAJour() ;
    SetText(_sLabel.c_str()) ;
    _sCertitude = string("WCE001") ;
    _Absence = true ;
	}
}

//-------------------------------------------------------------------------// 				Former la patpatho de this (il est le prtemier élément de
//										cette patpatho
//-------------------------------------------------------------------------
void
NSTreeNode::formerPatPatho(NSCutPaste* pBuffer)
{
	if ((NSCutPaste*) NULL == pBuffer)
		return ;

try
{
	Message Msg ;

	if (string("") == _sInteret)
		_sInteret = string("A") ;

  Msg.SetLexique(_sIdentite) ;
  Msg.SetUnit(_sUnit) ;
  Msg.SetCertitude(_sCertitude) ;
  Msg.SetInteret(_sInteret) ;
  Msg.SetComplement(_sContenuTransfert) ;
  Msg.SetPluriel(_sPluriel) ;
  Msg.SetVisible(_sVisible) ;
  Msg.SetNodeRight(_sNodeRights) ;
  setMessageIds(&Msg) ;

  string sLabel      = _sIdentite ;
  string sStockNoeud = _sNoeud ;
  string sChampEdit  = _sContenuTransfert ;
  string sUnite      = "" ;

	// Texte libre £?????
	if (sLabel == string("£?????"))
	{
		string sIndice = "00000";
		if (false == pBuffer->getTL()->empty())
		{
    	NSCutPastTL* pCPTL = pBuffer->getTL()->back() ;
      sIndice = pCPTL->sIndice ;

      int i = strlen(sIndice.c_str()) ;
      bool tourner = true;
      while ((tourner) && (i > 0))
      {
      	i-- ;
        if (((sIndice[i] >= '0') && (sIndice[i] < '9')) ||            ((sIndice[i] >= 'A') && (sIndice[i] < 'Z')))
        {
        	sIndice[i] = char(sIndice[i] + 1) ;
          tourner = false ;
        }
        else if (sIndice[i] == '9')
        {
        	sIndice[i] = 'A' ;
          tourner = false ;
        }
        else
        	sIndice[i] = '0' ;
      }
    }

    NSCutPastTL CPTexte ;
    CPTexte.sIndice = sIndice ;
    CPTexte.sTexte  = _sTexteGlobal ;
    pBuffer->getTL()->push_back(new NSCutPastTL(CPTexte)) ;

    NSPatPathoArray BufferPpt(pContexte->getSuperviseur()) ;
    pBuffer->initFromPatPatho(&BufferPpt) ;

    Msg.SetComplement(sIndice) ;    BufferPpt.ajoutePatho(sLabel, &Msg, 0) ;

    pBuffer->setPatPatho(&BufferPpt) ;
    return ;
	}

	sLabel = _sIdentite ;

  NSPatPathoArray BufferPpt(pContexte->getSuperviseur()) ;
  pBuffer->initFromPatPatho(&BufferPpt) ;

	BufferPpt.ajoutePatho(sLabel, &Msg, 0) ;

  pBuffer->setPatPatho(&BufferPpt) ;

  //
  // Si le noeud a des fils, il les ajoute récursivement
  //
	if (false == _VectFils.empty())
	{
		NSCutPaste CPFils(pContexte->getSuperviseur()) ;
    *(CPFils.getTL()) = *(pBuffer->getTL()) ;

    // ATTENTION : VectFils n'est pas trié
    // Il faut traiter les fils par ordre croissant de ligne
    int  ligneTraitee = -1 ;
    int  ligneMax	    = INT_MAX ;
    bool tourner = true ;
    iterNSTreeNode fils = _VectFils.begin() ;

    while (tourner)
    {
    	// On cherche le fils de ligne immédiatement supérieure à ligneTraitee
      //
      int numLigne = ligneMax ;
      for (iterNSTreeNode i = _VectFils.begin() ; _VectFils.end() != i ; i++)
      {      	if (((*i)->_ligne > ligneTraitee) && ((*i)->_ligne < numLigne))
        {
        	fils = i ;
          numLigne = (*i)->_ligne ;
        }
      }
      if (numLigne == ligneMax)
      	tourner = false ;
      else if ((false == (*fils)->_estPropose) && (false == (*fils)->estFictif()))
      {
      	// On ne réinitialise que la patpatho
        //
        NSPatPathoArray SonPpt(pContexte->getSuperviseur()) ;
        CPFils.setPatPatho(&SonPpt) ;

        (*fils)->formerPatPatho(&CPFils) ;

        NSPatPathoArray BufferPpt(pContexte->getSuperviseur()) ;
        pBuffer->initFromPatPatho(&BufferPpt) ;

        int decaligne = (BufferPpt.back())->getLigne() + 1 ;
        //
        // concaténer les 2 patpathos avec un décalage d'une colonne
        //
        CPFils.initFromPatPatho(&SonPpt) ;

        PatPathoIter j = SonPpt.begin() ;
        for (; SonPpt.end() != j ; j++)
        {
        	(*j)->setLigne((*j)->getLigne() + decaligne) ;
          (*j)->setColonne((*j)->getColonne() + 1) ;          BufferPpt.push_back(new NSPatPathoInfo(*(*j))) ;
        }

        CPFils.setPatPatho(&SonPpt) ;
        pBuffer->setPatPatho(&BufferPpt) ;
      }
      if (tourner)
      	ligneTraitee = (*fils)->_ligne ;
    }
    *(pBuffer->getTL()) = *(CPFils.getTL()) ;
  }
}
catch (...)
{
	erreur("Exception (NSTreeNode::formerPatPatho).", standardError, 0) ;
}
}

string
NSTreeNode::getLabel()
{
  string sEtiqu = _sIdentite ;

  if (string("") != _sPluriel)
    sEtiqu += string(1, intranodeSeparationMARK) + _sPluriel ;
  if (string("") != _sCertitude)
    sEtiqu += string(1, intranodeSeparationMARK) + _sCertitude ;

  return sEtiqu ;
}

string
NSTreeNode::getLabelSens()
{
  string sEtiqu ;
  pContexte->getDico()->donneCodeSens(&_sIdentite, &sEtiqu) ;

  string sCodeSens ;
  if (string("") != _sPluriel)
  {
    pContexte->getDico()->donneCodeSens(&_sPluriel, &sCodeSens) ;
    sEtiqu += string(1, intranodeSeparationMARK) + sCodeSens ;
  }
  if (string("") != _sCertitude)
  {
    pContexte->getDico()->donneCodeSens(&_sCertitude, &sCodeSens) ;
    sEtiqu += string(1, intranodeSeparationMARK) + sCodeSens ;
  }

  return sEtiqu ;
}

// ------------------------------------------------------------------------// Former le chemin CODE SENS constitué par les père, grand-père...
// ------------------------------------------------------------------------
void
NSTreeNode::formerChemin(string* pChemin)
{
	if ((string*) NULL == pChemin)
  	return ;

  *pChemin = string("") ;

  //
  // Si on peut prendre la localisation du BBItem, c'est très bien
  //
  if ((_pControle) && (_pControle->getTransfert()))
  {
  	BBFilsItem* pBBfils = _pControle->getTransfert()->getFilsItem() ;
    if (pBBfils && pBBfils->getItemFather())
    {
      *pChemin = pBBfils->getItemFather()->_sLocalisation ;
      return ;
    }
	}

  //
  // Sinon, on prend le chemin local père, grand-père, etc
  //
  NSTreeNode*	pAieul = _pere ;
  while (pAieul)
  {
    string sEtiqu = pAieul->getLabelSens() ;

    if (string("") != *pChemin)
    	*pChemin = string(1, cheminSeparationMARK) + *pChemin ;
    *pChemin = sEtiqu + *pChemin ;

    pAieul = pAieul->_pere ;
	}
}

// ------------------------------------------------------------------------
// Former le chemin CODE COMPLET local à la treeWindow...
// ------------------------------------------------------------------------
void
NSTreeNode::formerCheminLocal(string* pChemin, bool bSens)
{
	if ((string*) NULL == pChemin)
		return ;

	*pChemin = string("") ;

	NSTreeNode*	pAieul = _pere ;
	while (pAieul)
	{
    string sEtiqu ;
    if (bSens)
  	  sEtiqu = pAieul->getLabelSens() ;
    else
      sEtiqu = pAieul->getLabel() ;

    if (string("") != *pChemin)
    	*pChemin = string(1, cheminSeparationMARK) + *pChemin ;
    *pChemin = sEtiqu + *pChemin ;

    pAieul = pAieul->_pere ;
	}
}

//-------------------------------------------------------------------------// 				Etiquette du noeud : texte libre ou simple étiquette
//
//	pNSTlibre       : pointeur sur la base des textes libres
//	pTexteRecupere  : savoir si on a pu récuperer un texte de la BD ou non
//	pTailleVecteur  : nombre de neouds qui vont supporter le texte
//  ligne           : ligne du noeud
//  colonne         : colonne du noeud
//  sTexteLibre     : texte récuperé
//	sCodeTexteLibre : code du texte libre à récupérer dans la base
//  sIdentitepatho  : code lexique du noeud
//	pTreeWindow     : pointeur sur la TreeWindow en cours
//
//  Cas particulier : si TL et que sCodeTexteLibre == "", c'est qu'on impose
//                    au noeud le contenu de sTexteLibre
//
//-------------------------------------------------------------------------
bool
NSTreeNode::MettreAjourLabel(NSTlibre* /* pNSTlibre */, bool* /* pTexteRecupere */, int* pTailleVecteur,
                             int ligne, int colonne, string sTexteLibre, string sCodeTexteLibre,
                             string sIdentitepatho, NSTreeWindow* pTreeWindow,
                             string langue)
{
	Interet() ;

	// Existence de texte libre
	if ((string("£?????") == sIdentitepatho) || (string("£C;") == string(sIdentitepatho, 0, 3)))
  {
    // SetImageIndex(5) ;
    // SetSelectedImageIndex(5) ;

    _sLabel = sTexteLibre ;
    strip(_sLabel, stripRight) ;
    _sContenuTransfert = sCodeTexteLibre ;
    VecteurChaine VecteurTexte ;   // décomposer pTexteLibre
    pTreeWindow->DecomposeChaineTL(&sTexteLibre, &VecteurTexte) ;

    // Never do this : "ligne" here is already node's line + pTailleVecteur
    //
    // pTreeWindow->CreerNSTreeNode(this, ligne + *pTailleVecteur, colonne,
    //	      			sIdentitepatho, sLabel, sCodeTexteLibre);

    pTreeWindow->CreerNSTreeNode(this, ligne, colonne, sIdentitepatho,
                                                   _sLabel, sCodeTexteLibre) ;

    pTreeWindow->RepartirTexteLibre(this, &VecteurTexte, &sTexteLibre,
                                                          pTailleVecteur) ;
    _sTexteGlobal = sTexteLibre ;
  }
  else
  {
		pTreeWindow->CreerNSTreeNode(this, ligne , colonne, sIdentitepatho, _sLabel,
                                                        sCodeTexteLibre) ;
		string sChemin, sCheminLocal ;
    formerChemin(&sChemin) ;  // Don't take the "sens" in order to be able to conjugate adjectives
    formerCheminLocal(&sCheminLocal, false) ;
    NSCutPaste CP(pContexte->getSuperviseur()) ;
    formerPatPatho(&CP) ;

    NSPatPathoArray CutPastePpt(pContexte->getSuperviseur()) ;
    CP.initFromPatPatho(&CutPastePpt) ;

    GlobalDkd Dcode(pContexte, langue, sChemin, &CutPastePpt) ;
    Dcode.decodeNoeud(sCheminLocal) ;
    _sLabel = Dcode.getDcodeur() ;

		SetText(_sLabel.c_str(), true) ;
	}
	return true ;
}

//-----------------------------------------------------------------//copier la patpatho de this dans pSuper->pPatPathoDragDrop  si
//sCopie vaut "DRAG" ou dans pSuper->pPatPathoCopie si sCopie
//vaut "COPIE"
//-----------------------------------------------------------------
void
NSTreeNode::CopyPatPatho(string sCopie)
{
  //former la patpatho de pNSTreeNodeDrag
  // BBFilsItem* pBBfils = pControle->getTransfert()->pBBFilsItem;
  Message Msg ;
  InsererTexteLibre(&Msg) ;

  NSCutPaste CP(pContexte->getSuperviseur()) ;

  NSSuper* pSuper = pContexte->getSuperviseur() ;

  if      (sCopie == "DRAG")
  	pSuper->pBufDragDrop->vider() ;
  else if (sCopie == "COPIE")
  	pSuper->pBufCopie->vider() ;

  // Mettre la patho correspondant à pNSTreeNodeDrag dans pSuper->pPatPathoDragDrop
  formerPatPatho(&CP) ;

  if      (sCopie == "DRAG")
  	*(pSuper->pBufDragDrop) = CP ;
  else if (sCopie == "COPIE")
  	*(pSuper->pBufCopie) = CP ;
}

//-----------------------------------------------------------------------//exemple si sEtiquette : PFONC1/WCE751
//									---> patpatho : cetritude : WCE751
//recupére le libellé du guide en étudiant le cas des étiquettes multiples
//comportant WC.. et WP...
//-----------------------------------------------------------------------
void
NSTreeNode::setLibelle(string sEtiquette, string langue)
{
  size_t posWPL 	= sEtiquette.find(string("WPL")) ;
  size_t posWCE  	= sEtiquette.find(string("WCE")) ;
  size_t posWCEA0 = sEtiquette.find(string("WCEA0")) ;
  string sEtiqu = "" ;
//si pluriel
  if ((posWPL != NPOS))
  {
    SetLexique(string(sEtiquette, 0, BASE_LEXI_LEN)) ;
    // sEtiqu =sEtiquette(0, PATHO_CODE_LEN);
    SetPluriel(string(sEtiquette, posWPL, BASE_PLURIEL_LEN)) ;
    SetCertitude(string(sEtiquette, posWCE, BASE_CERTITUDE_LEN)) ;
    // pSuper->pDico->donneLibelle(&sEtiqu, &sLabel);
    //return;
	}
  else
  {
  	// Si certitude = présence totale, ne rien mettre dans la case
    // certitude de la patpatho
    //
    if (posWCEA0 != NPOS)
    {
    	SetLexique(string(sEtiquette, 0, BASE_LEXI_LEN)) ;
      //sEtiqu =sEtiquette(0, PATHO_CODE_LEN);
      SetCertitude(string("")) ;
      // pSuper->pDico->donneLibelle(&sEtiqu, &sLabel);
      //return;
    }
    else
    {
    	if ((posWCE != NPOS))
      {
        SetLexique(string(sEtiquette, 0, BASE_LEXI_LEN));
        //sEtiqu =sEtiquette(0, PATHO_CODE_LEN);
        SetCertitude(string(sEtiquette, posWCE, BASE_CERTITUDE_LEN));
        // pSuper->pDico->donneLibelle(&sEtiqu, &sLabel);
        // fixeCertitude(sEtiquette(posWCE,BASE_CERTITUDE_LEN));
        //return;
      }
      else
      	SetLexique(sEtiquette) ;
    }
  }
  string sChemin, sCheminLocal ;
  formerChemin(&sChemin) ;
  formerCheminLocal(&sCheminLocal, false) ;

  NSCutPaste CP(pContexte->getSuperviseur()) ;
  formerPatPatho(&CP);

  NSPatPathoArray CutPastePpt(pContexte->getSuperviseur()) ;
  CP.initFromPatPatho(&CutPastePpt) ;

  GlobalDkd Dcode(pContexte, langue, sChemin, &CutPastePpt) ;
  Dcode.decodeNoeud(sCheminLocal) ;
  _sLabel = Dcode.getDcodeur() ;

  // pSuper->pDico->donneLibelle(&sEtiquette, &sLabel);
}

void
NSTreeNode::trouveLabel(string langue)
{
  string sChemin, sCheminLocal ;
  formerChemin(&sChemin) ;
  formerCheminLocal(&sCheminLocal, false) ;

  NSCutPaste CP(pContexte->getSuperviseur()) ;
  formerPatPatho(&CP) ;

  NSPatPathoArray CutPastePpt(pContexte->getSuperviseur()) ;
  CP.initFromPatPatho(&CutPastePpt) ;

  GlobalDkd Dcode(pContexte, langue, sChemin, &CutPastePpt) ;

  Dcode.decodeNoeud(sCheminLocal) ;
  _sLabel = Dcode.getDcodeur() ;
}

void
NSTreeNode::addLinkedNode(NSLinkedNode* pLnkNode)
{
	if ((NSLinkedNodeArray*) NULL == _pTemporaryLinks)
		_pTemporaryLinks = new NSLinkedNodeArray() ;

	_pTemporaryLinks->push_back(pLnkNode) ;
}

bool
NSTreeNode::canWeClose()
{
	return true ;
}

void
NSTreeNode::addTemporaryLink(string sNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection)
{
	if ((NSLinkedNodeArray*) NULL == _pTemporaryLinks)
	 	_pTemporaryLinks = new NSLinkedNodeArray() ;

	_pTemporaryLinks->push_back(new NSLinkedNode(sNode, iLkType, iLkDirection)) ;
}

void
NSTreeNode::addTemporaryLink(NSPatPathoInfo* pOtherTemporaryNode, NSRootLink::NODELINKTYPES iLkType, NODELINKDIRECTION iLkDirection)
{
	if ((NSLinkedNodeArray*) NULL == _pTemporaryLinks)
  	_pTemporaryLinks = new NSLinkedNodeArray() ;

	_pTemporaryLinks->push_back(new NSLinkedNode(pOtherTemporaryNode, iLkType, iLkDirection)) ;
}

NSTransferInfo*
NSTreeNode::getTransfert()
{
  if ((NSControle*) NULL == _pControle)
    return (NSTransferInfo*) 0 ;

  return _pControle->getTransfert() ;
}

BBFilsItem*
NSTreeNode::getBBFilsItem()
{
  if (((NSControle*) NULL == _pControle) || ((NSTransferInfo*) NULL == _pControle->getTransfert()))
    return (BBFilsItem*) 0 ;

  return _pControle->getTransfert()->getFilsItem() ;
}

Message*
NSTreeNode::getTransfertMessage()
{
  if (((NSControle*) NULL == _pControle) || ((NSTransferInfo*) NULL == _pControle->getTransfert()))
    return (Message*) 0 ;

  return _pControle->getTransfert()->getTransfertMessage() ;
}

void
NSTreeNode::setTransfertMessage(const Message& message)
{
  Message* pMessage = getTransfertMessage() ;
  if ((Message*) NULL == pMessage)
    return ;

  *pMessage = message ;
}

void
NSTreeNode::setMessageIds(Message* pMessage)
{
  if (((Message*) NULL == pMessage) || (string("") == _sNoeud))
    return ;

  size_t iNodeIdLen = strlen(_sNoeud.c_str()) ;

  if (iNodeIdLen == PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN + PPD_NOEUD_LEN)
  {
    pMessage->SetTreeID(string(_sNoeud, 0, PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN)) ;
    pMessage->SetNoeud(string(_sNoeud, PIDS_NSS_LEN + DOC_CODE_DOCUM_LEN, PPD_NOEUD_LEN)) ;
    return ;
  }

  if (iNodeIdLen == PPD_NOEUD_LEN)
  {
    pMessage->SetNoeud(string(_sNoeud, 0, PPD_NOEUD_LEN)) ;
    return ;
  }
}

void
NSTreeNode::getMessageIds(Message* pMessage)
{
  if ((Message*) NULL == pMessage)
    return ;

  _sNoeud = pMessage->GetTreeID() + pMessage->GetNoeud() ;
}

