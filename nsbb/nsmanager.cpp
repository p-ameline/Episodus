// -----------------------------------------------------------------------------// nsmanager.cpp
// -----------------------------------------------------------------------------
// Gestion des graphes
// -----------------------------------------------------------------------------
// $Revision: 1.27 $
// $Author: pameline $
// $Date: 2016/12/29 16:21:49 $
// -----------------------------------------------------------------------------
// PA  - septembre 2003
// -----------------------------------------------------------------------------
// FLP - aout 2004
// modification de graphPrepare() - on ne fait plus de getPatho() ou appelle la
// méthode de NSDataTree qui le trie par localisation.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant à gérer et traiter les
// informations de santé d'une personne.
//
// Ce logiciel est régi par la licence CeCILL soumise au droit français et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffusée par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilité au code source et des droits de copie, de
// modification et de redistribution accordés par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limitée. Pour les mêmes raisons, seule une
// responsabilité restreinte pèse sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les concédants successifs.
//
// A cet égard  l'attention de l'utilisateur est attirée sur les risques
// associés au chargement, à l'utilisation, à la modification et/ou au
// développement et à la reproduction du logiciel par l'utilisateur étant donné
// sa spécificité de logiciel libre, qui peut le rendre complexe à manipuler et
// qui le réserve donc à des développeurs et des professionnels avertis
// possédant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invités à charger et tester l'adéquation du logiciel à leurs besoins
// dans des conditions permettant d'assurer la sécurité de leurs systèmes et ou
// de leurs données et, plus généralement, à l'utiliser et l'exploiter dans les
// mêmes conditions de sécurité.
//
// Le fait que vous puissiez accéder à cet en-tête signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accepté les termes.
// -----------------------------------------------------------------------------
// This software is a computer program whose purpose is to manage and process
// a person's health data.
//
// This software is governed by the CeCILL  license under French law and abiding
// by the rules of distribution of free software. You can use, modify and/ or
// redistribute the software under the terms of the CeCILL license as circulated
// by CEA, CNRS and INRIA at the following URL "http://www.cecill.info".
//
// As a counterpart to the access to the source code and  rights to copy, modify
// and redistribute granted by the license, users are provided only with a
// limited warranty and the software's author, the holder of the economic
// rights, and the successive licensors have only limited liability.
//
// In this respect, the user's attention is drawn to the risks associated with
// loading, using, modifying and/or developing or reproducing the software by
// the user in light of its specific status of free software, that may mean that
// it is complicated to manipulate, and that also therefore means that it is
// reserved for developers and experienced professionals having in-depth
// computer knowledge. Users are therefore encouraged to load and test the
// software's suitability as regards their requirements in conditions enabling
// the security of their systems and/or data to be ensured and, more generally,
// to use and operate it in the same conditions as regards security.
//
// The fact that you are presently reading this means that you have had
// knowledge of the CeCILL license and that you accept its terms.
// -----------------------------------------------------------------------------

#include <utility.h>
#include <mem.h>
#include <string.h>
#include <cstring.h>

#include "nsbb\nsmanager.h"
#include "nsbb\nspatbd.h"
#include "nsbb\ns_objs.h"
#include "nssavoir\nsgraphe.h"
#include "nssavoir\nsfilgd.h"
#include "nsdn\nsdn_dlg.h"
#include "nsbb\nsmpidsobj.h"
#include "dcodeur\decoder.h"
#include "nsbb\nsdefArch.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
  #include "enterpriseLus\personLus.h"
  #include "nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "nsbb\nslistwind.h"
  #include "nsbb\nsbbitem.h"
  #include "nsbb\nsbbsmal.h"
  #include "nsbb\nsmpids.h"
  #include "partage\nsdivfct.h"
#endif

# include "pilot\NautilusPilot.hpp"
# include "nsbb\tagNames.h"
# include "nsbb\nsarc.h"

# include "nsbb\nsbbtran.h"
# include "nssavoir\nsHealthTeam.h"

long NSDataTree::_lObjectCount = 0 ;
long NSDataTreeArray::_lObjectCount = 0 ;
long NSArcLink::_lObjectCount = 0 ;
long NSArcLinkArray::_lObjectCount = 0 ;
long NSNodeRight::_lObjectCount = 0 ;
long NSNodeRightArray::_lObjectCount = 0 ;
long NSDataGraph::_lObjectCount = 0 ;
long NSObjectGraphManager::_lObjectCount = 0 ;
long NSPersonGraphManager::_lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// METHODES DE NSDataTree
// -----------------------------------------------------------------------------
NSDataTree::NSDataTree(NSSuper* pSuper, string treeID, GRAPHTYPE iType)
           :NSSuperRoot(pSuper), _PatPathoArray(pSuper, iType)
{
	_iTypeTree = iType ;

	setTreeID(treeID) ;

	_sModelName     = string("") ;
	_sModelFileName = string("") ;

	_lObjectCount++ ;
}

NSDataTree::NSDataTree(const NSDataTree& rv)
           :NSSuperRoot(rv._pSuper), _PatPathoArray(rv._PatPathoArray)
{
	_iTypeTree      = rv._iTypeTree ;
	_sTreeID        = rv._sTreeID ;

	_sModelName     = rv._sModelName ;
	_sModelFileName = rv._sModelFileName ;

	_lObjectCount++ ;
}

NSDataTree::~NSDataTree()
{
	_lObjectCount-- ;
}

NSDataTree&
NSDataTree::operator=(const NSDataTree& src)
{
	if (this == &src)
  	return *this ;

  _iTypeTree      = src._iTypeTree ;
  _sTreeID        = src._sTreeID ;
  _sModelName     = src._sModelName ;
  _sModelFileName = src._sModelFileName ;

  _PatPathoArray  = src._PatPathoArray ;

	return *this ;
}

int
NSDataTree::operator==(const NSDataTree& o)
{
	if ((_iTypeTree     == o._iTypeTree) &&
      (_sTreeID       == o._sTreeID)   &&
      (_PatPathoArray == o._PatPathoArray))
		return 1 ;
	else
		return 0 ;
}

void
NSDataTree::setTreeID(string treeID)
{
	if ((graphObject == _iTypeTree) && (strlen(treeID.c_str()) == OBJECT_ID_LEN))
		_sTreeID = treeID ;
	else if (graphPerson == _iTypeTree)
	{
		if (strlen(treeID.c_str()) == (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN))
			_sTreeID = treeID ;
		else
    {
    	string sErrorText = _pSuper->getText("graphAndTreeErrors", "invalidTreeId") ;
      sErrorText += string(" setTreeID(") + treeID + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      _sTreeID = string("") ;
    }
	}
	else
		_sTreeID = string("") ;
}

void
NSDataTree::developpePatPatho(NSPatPathoArray* pPatPatho)
{
	if (((NSPatPathoArray*) NULL == pPatPatho) || pPatPatho->empty())
		return ;

	PatPathoConstIter pptIt = pPatPatho->begin() ;
	while (pPatPatho->end() != pptIt)
	{
    //
    // Check free texts ; replace \' with '
    //
    	/*************************************
		string sTL = (*pptIt)->getTexteLibre() ;
		size_t pos = sTL.find("\'") ;
		while (pos != string::npos)
		{
            if ((pos > 0) && (sTL[pos-1] == '\\'))
    	        sTL = string(sTL, 0, pos-1) + string(sTL, pos, strlen(sTL.c_str())-pos) ;
    	    pos = sTL.find("\'", pos+1) ;
        }
        (*pptIt)->setTexteLibre(sTL) ;
        ****************************************/

		//
		// Hidden node - Noeud caché
		//
		if ((*pptIt)->getLexique() == "900001")
		{
			if ((*pptIt)->getUnit() == "200001")
			{
				int nbFils = atoi(((*pptIt)->getComplement()).c_str()) - 1;
				if (nbFils > 0)
				{
					int iLig = (*pptIt)->getLigne() ;
					int iCol = (*pptIt)->getColonne() ;

					pptIt++ ;
					if (pPatPatho->end() != pptIt)
					{
						int iLigNext = (*pptIt)->getLigne() ;
						// iColNext = (*pptIt)->getColonne() ;
						if (nbFils != iLigNext - iLig - 1)
						{
							nbFils = iLigNext - iLig - 1 ;
							erreur("NSDataTree::developpePatPatho strange patpatho.", warningError) ;
						}
					}

					for (int i = 0; i < nbFils; i++)
					{
						NSPatPathoInfo* pPatInfo = new NSPatPathoInfo ;
						pPatInfo->setLigne(iLig + i + 1) ;
						pPatInfo->setColonne(iCol + 1) ;
						pPatInfo->setLexique("900002") ;
						pPatPatho->push_back(pPatInfo) ;
					}
          //
          // We need to sort the patpatho, because if we don't do it and
          // the last node is hidden, then it will be followed by the first
          // virtual node we pushed back, and iLigNext and iColNext will be
          // wrong
          //
          pPatPatho->sorting() ;

					// Warning : since the patpatho will grow, we have better repositioning
					// the iterator after
					//
					pptIt = pPatPatho->ChercherItem(iLig, iCol) ;
					if ((NULL == pptIt) || (pPatPatho->end() == pptIt))
					{
						erreur("NSDataTree::developpePatPatho bad patpatho.", standardError) ;
						return ;
					}
				}
			}
		}
		pptIt++ ;
	}

	pPatPatho->sorting() ;
}

void
NSDataTree::contractePatPatho(NSPatPathoArray* pPatPatho)
{
	if ((NULL == pPatPatho) || (pPatPatho->empty()))
		return ;

	PatPathoIter pptIt = pPatPatho->begin();
	while (pPatPatho->end() != pptIt)
	{
		pptIt = pPatPatho->ChercherItem("900002") ;
		if ((pPatPatho->end() != pptIt) && (NULL != pptIt))
		{
			delete (*pptIt) ;
			pPatPatho->erase(pptIt) ;
		}
	}
}

void
NSDataTree::getRawPatPatho(NSPatPathoArray* pPatPatho, bool bModePilot)
{
	if ((NSPatPathoArray*) NULL == pPatPatho)
		return ;

	*pPatPatho = _PatPathoArray ;

	if (false == bModePilot)
		developpePatPatho(pPatPatho) ;
}

void
NSDataTree::setPatPatho(NSPatPathoArray* pPatPatho, bool bModePilot)
{
	if ((NSPatPathoArray*) NULL == pPatPatho)
		return ;

	if (false == bModePilot)
		contractePatPatho(pPatPatho) ;

	_PatPathoArray = *pPatPatho ;
}

string
NSDataTree::genereXML()
{
  return _PatPathoArray.genereXML() ;
}

size_t
NSDataTree::size()
{
  return _PatPathoArray.size() ;
}

void
NSDataTree::sortPPTByLocalisation()
{
  _PatPathoArray.sorting() ;
}

// readParadoxTree vérifie juste qu'il existe un ID valide
// et que la patpatho se charge correctement
bool
NSDataTree::readParadoxTree()
{
	if (string("") == _sTreeID)
	{
		erreur("L'arbre à charger ne possède pas d'identifiant.", standardError) ;
		return false ;
	}

	if ((graphPerson != _iTypeTree) && (graphObject != _iTypeTree))
	{
		erreur("L'arbre à charger ne possède pas un type valide.", standardError) ;
		return false ;
	}

	return false ;
}

// on suppose comme précondition au writeParadoxTree que l'arbre
// est nouveau ou qu'il a été préalablement chargé depuis la base

bool
NSDataTree::writeParadoxTree(NSPatPathoArray* pPatPathoTree)
{
	// modification de la patpatho
	if (pPatPathoTree)
  	_PatPathoArray = *pPatPathoTree ;

	return true ;
}

/*#ifdef N_TIERS
bool
NSDataTree::prepareTree(NSPatPathoArray* pPatPathoTree)
{
     if (iTypeTree == graphObject)
    {
 //................. a ajouter ...............
    }
    else if (iTypeTree == graphPerson)
    {
        if (sTreeID == "")
        {
            if (pPatPathoTree == 0)
               // setTreeID((*(pPatPathoArray->begin()))->getDoc());
                setTreeID((*(pPatPathoArray->begin()))->getDoc());
            else
                setTreeID((*(pPatPathoTree->begin()))->getDoc());
        }
    }
    else
    {
        erreur("Type d'objet incorrect dans l'objet DataTree.", standardError, 0) ;
        return false;
    }
    // modification de la patpatho
    if (pPatPathoTree != 0)
        *pPatPathoArray = *pPatPathoTree;

    return true;
}
#endif      */


// --------------------------------------------------------------------------
// ------------------------ METHODES DE NSDataTreeArray ----------------------
// --------------------------------------------------------------------------

NSDataTreeArray::NSDataTreeArray()
                :NSDataTreeVector()
{
  _lObjectCount++ ;
}

NSDataTreeArray::NSDataTreeArray(const NSDataTreeArray& rv)
                :NSDataTreeVector()
{
try
{
	if (false == rv.empty())
		for (ConstNSDataTreeIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSDataTree(*(*i))) ;

	_lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSDataTreeArray copy ctor.", standardError) ;
}
}

voidNSDataTreeArray::vider()
{
	if (empty())
		return ;

	for (NSDataTreeIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSDataTreeArray::~NSDataTreeArray()
{
	vider() ;

	_lObjectCount-- ;
}

NSDataTreeArray&
NSDataTreeArray::operator=(const NSDataTreeArray& src)
{
try
{
	if (&src == this)
  	return *this ;

	vider() ;

	if (false == src.empty())		for (ConstNSDataTreeIter i = src.begin() ; src.end() != i ; i++)
			push_back(new NSDataTree(*(*i))) ;

	return *this ;
}
catch (...)
{
	erreur("Exception NSDataTreeArray (= operator).", standardError) ;
	return *this ;
}
}

int
NSDataTreeArray::operator==(const NSDataTreeArray& o)
{
try
{
  if (empty())
  {    if (o.empty())      return 1 ;    else      return 0 ;  }

  ConstNSDataTreeIter i = begin() ;
  ConstNSDataTreeIter j = o.begin() ;  while ((end() != i) && (o.end() != j))  {    if (!((*(*i)) == (*(*j))))      return 0 ;    i++ ;    j++ ;  }  if ((end() != i) || (o.end() != j))    return 0 ;  return 1 ;
}
catch (...)
{
	erreur("Exception NSDataTreeArray (== operator).", standardError) ;
	return 0 ;
}
}

bool
NSDataTreeArray::ExisteTreeID(string sObjectID, NSDataTreeIter* pIter)
{
	if ((string("") == sObjectID) || (empty()))
		return false ;

	for (NSDataTreeIter i = begin(); end() != i ; i++)
	{
  	if ((*i)->getTreeID() == sObjectID)
    {
    	if (pIter)
      	*pIter = i ;

      return true ;
    }
  }

	if (pIter)
		*pIter = NULL ;

	return false ;
}

bool
NSDataTreeArray::ExisteTree(string sTypeLex, NSSuper* pSuper, NSDataTreeIter* pIter)
{
  if (pIter)
		*pIter = NULL ;

  if ((NULL == pSuper) || (string("") == sTypeLex) || empty())
    return false ;

  for (NSDataTreeIter i = begin(); end() != i ; i++)
  {
    NSPatPathoArray *pPPT = (*i)->getPatPathoPointer() ;

    if (pPPT && (false == pPPT->empty()))
    {
      PatPathoIter iter = pPPT->begin() ;

      if ((*iter)->getLexique() == sTypeLex)
      {
        if (pIter)
          *pIter = i ;

        return true ;
      }
    }
  }

	return false ;
}

bool
NSDataTreeArray::DetruireTree(string sTypeLex, NSSuper* pSuper)
{
  if ((string("") == sTypeLex) || (empty()))
    return false ;

	PatPathoIter iter ;
	NSPatPathoArray PatPathoArray(pSuper) ;

  for (NSDataTreeIter i = begin(); end() != i ; i++)
  {
    PatPathoArray.vider() ;
    (*i)->getRawPatPatho(&PatPathoArray) ;

    if (false == PatPathoArray.empty())
    {
      iter = PatPathoArray.begin() ;

      if ((*iter)->getLexique() == sTypeLex)
      {
        delete *i ;
        erase(i) ;
        return true ;
      }
    }
  }

	return false ;
}

// *************************************************************************
// *                          METHODES DE NSArcLink                        *
// *************************************************************************

NSArcLink::NSArcLink(NSSuper* pSuper) : NSSuperRoot(pSuper)
{
  _sObject = string("") ;
  _sNode   = string("") ;
  _sType   = string("") ;
  _sModel_object_id = string("") ;

  _lObjectCount++ ;
}

NSArcLink::NSArcLink(NSSuper* pSuper, NSPatLinkInfo* pLinkInfo) : NSSuperRoot(pSuper)
{
	NSRootLink rootLink ;
  NSRootLink::NODELINKTYPES iNodeLink = rootLink.donneRelation(pLinkInfo->getLien()) ;

	if (NSRootLink::archetype == iNodeLink)
	{
		setFullNode(pLinkInfo->getQualifie()) ;
    _sType = pLinkInfo->getLien() ;
    _sModel_object_id = pLinkInfo->getQualifiant() ;
	}
  else if (NSRootLink::referentialOf == iNodeLink)
  {
  	setFullNode(pLinkInfo->getQualifiant()) ;
    _sType = pLinkInfo->getLien() ;
    _sModel_object_id = pLinkInfo->getQualifie() ;
  }
  else
  {
  	erreur("Bad link (should be archetype or referentialOf)", standardError) ;
    _sObject = string("") ;
    _sNode   = string("") ;
    _sType   = string("") ;
    _sModel_object_id = string("") ;
	}

  _lObjectCount++ ;
}

NSArcLink::NSArcLink(NSSuper* pSuper, string sObject, string sNode, string sType, string sModelID)
          :NSSuperRoot(pSuper)
{
  _sObject = sObject ;
  _sNode   = sNode ;
  _sType   = sType ;
  _sModel_object_id = sModelID ;

  _lObjectCount++ ;
}

NSArcLink::NSArcLink(const NSArcLink& rv) : NSSuperRoot(rv._pSuper)
{
  _sObject = rv._sObject ;
  _sNode   = rv._sNode ;
  _sType   = rv._sType ;
  _sModel_object_id = rv._sModel_object_id ;

  _lObjectCount++ ;
}

NSArcLink::~NSArcLink()
{
  _lObjectCount-- ;
}

NSArcLink&
NSArcLink::operator=(const NSArcLink& src)
{
	if (&src == this)
  	return *this ;

  _sObject = src._sObject ;
  _sNode   = src._sNode ;
  _sType   = src._sType ;
  _sModel_object_id = src._sModel_object_id ;

  return *this ;
}

int
NSArcLink::operator==(const NSArcLink& o)
{
  if ((_sObject == o._sObject) &&
      (_sNode   == o._sNode) &&
      (_sType   == o._sType) &&
      (_sModel_object_id == o._sModel_object_id))
    return 1 ;
  else
    return 0 ;
}

void
NSArcLink::setFullNode(string sNode)
{
  if (strlen(sNode.c_str()) > OBJECT_ID_LEN)
  {
    _sObject = string(sNode, 0, OBJECT_ID_LEN) ;
    _sNode   = string(sNode, OBJECT_ID_LEN, strlen(sNode.c_str()) - OBJECT_ID_LEN) ;
  }
  else
  {
    _sObject = sNode ;
    _sNode   = string("") ;
  }
}

string
NSArcLink::getFullNode()
{
  return (_sObject + _sNode) ;
}

// --------------------------------------------------------------------------
// ------------------------ METHODES DE NSArcLinkArray ----------------------
// --------------------------------------------------------------------------

NSArcLinkArray::NSArcLinkArray()
               :NSArcLinkVector()
{
  _lObjectCount++ ;
}

NSArcLinkArray::NSArcLinkArray(const NSArcLinkArray& rv)
               :NSArcLinkVector()
{
try
{
  if (false == rv.empty())
    for (NSArcLinkConstIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new NSArcLink(*(*i))) ;

  _lObjectCount++ ;
}
catch (...)
{
  erreur("Exception NSArcLinkArray copy ctor.", standardError) ;
}
}

voidNSArcLinkArray::vider()
{
  if (empty())
    return ;

  for (NSArcLinkIter i = begin() ; end() != i ; )
  {
    delete *i ;
    erase(i) ;
  }
}

NSArcLinkArray::~NSArcLinkArray()
{
	vider() ;

  _lObjectCount-- ;
}

NSArcLinkArray&
NSArcLinkArray::operator=(const NSArcLinkArray& src)
{
try
{
	if (&src == this)
  	return *this ;

  vider() ;

  if (false == src.empty())  	for (NSArcLinkConstIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSArcLink(*(*i))) ;

  return *this ;
}
catch (...)
{
	erreur("Exception NSArcLinkArray (= operator).", standardError) ;
  return *this ;
}
}

int
NSArcLinkArray::operator==(const NSArcLinkArray& o)
{
try
{
  if (empty())
  {    if (o.empty())      return 1 ;    else      return 0 ;  }  NSArcLinkConstIter i = begin() ;  NSArcLinkConstIter j = o.begin() ;  while ((end() != i) && (o.end() != j))  {    if (!((*(*i)) == (*(*j))))      return 0 ;    i++ ;    j++ ;  }  if ((end() != i) || (o.end() != j))    return 0 ;  return 1 ;
}
catch (...)
{
  erreur("Exception NSArcLinkArray (== operator).", standardError) ;
  return 0 ;
}
}

//**************************************************************************
//*               Methode de NSNodeRight                                   *
//**************************************************************************

/*
** The first element is an id which permit to find the document or the patPatho
** The second element is the right of this node or document
*/
NSNodeRight::NSNodeRight(std::string nod, std::string rig)
{
  _sNode  = nod ;
  _sRight = rig ;

  _lObjectCount++ ;
}

NSNodeRight::NSNodeRight()
{
  _sNode  = string("") ;
  _sRight = string("") ;

  _lObjectCount++ ;
}

NSNodeRight::NSNodeRight(const NSNodeRight& rig)
{
  _sNode  = rig._sNode ;
  _sRight = rig._sRight ;

  _lObjectCount++ ;
}

NSNodeRight::~NSNodeRight()
{
  _lObjectCount-- ;
}

NSNodeRight&
NSNodeRight::operator=(const NSNodeRight& rig)
{
  if (&rig == this)
  	return *this ;

  _sNode  = rig._sNode ;
  _sRight = rig._sRight ;

  return *this ;
}

bool NSNodeRight::operator==(const NSNodeRight& rig)
{
  return ((_sNode == rig._sNode) && (_sRight == rig._sRight)) ;
}

//**************************************************************************
//*         Array of NSNodeRight                                               *
//**************************************************************************

NSNodeRightArray::NSNodeRightArray()
                 :NSNodeRightVector()
{
  _lObjectCount++ ;
}

NSNodeRightArray::NSNodeRightArray(const NSNodeRightArray& rv)
                 :NSNodeRightVector()
{
try
{
	if (false == rv.empty())
		for (NSNodeRightConstIter i = rv.begin() ; rv.end() != i ; i++)
			push_back(new NSNodeRight(*(*i))) ;

  _lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSNodeRightArray copy ctor.", standardError) ;
}
}

NSNodeRightArray::~NSNodeRightArray()
{
	vider() ;

  _lObjectCount-- ;
}

void
NSNodeRightArray::vider()
{
	if (empty())
		return ;

	for (NSNodeRightIter i = begin() ; end() != i ; )
	{
		delete *i ;
		erase(i) ;
	}
}

NSNodeRightArray&
NSNodeRightArray::operator=(const NSNodeRightArray& src)
{
try
{
	if (&src == this)
  	return *this ;

  vider() ;

  if (false == src.empty())  	for (NSNodeRightConstIter i = src.begin() ; src.end() != i ; i++)
    	push_back(new NSNodeRight( *(*i)) ) ;

  return *this ;
}
catch (...)
{
	erreur("Exception NSNodeRightArray (= operator).", standardError) ;
	return *this ;
}
}

int
NSNodeRightArray::operator==(const NSNodeRightArray& o)
{
try
{
  if (empty())
  {  	if (o.empty())    	return 1 ;    else    	return 0 ;  }  NSNodeRightConstIter i = begin() ;  NSNodeRightConstIter j = o.begin() ;  while ((end() != i) && (o.end() != j))  {  	if (!((*(*i)) == (*(*j))))    	return 0 ;    i++ ;    j++ ;  }  if ((end() != i) || (o.end() != j))    return 0 ;  return 1 ;
}
catch (...)
{
	erreur("Exception NSNodeRightArray (== operator).", standardError) ;
	return 0 ;
}
}

/*
**  Function that search a right for a node
**  Result : If the node is fond , we return the right
**           Else return a Null String "";
*/
std::string
NSNodeRightArray::find(std::string temp)
{
	if (empty() == true)
		return std::string("") ;

  std::vector<NSNodeRight*>::iterator iResult ;
  iResult = find_if(begin(), end(), RightStringPredicat(&temp)) ;

  if (end() != iResult)
    return (*iResult)->getRight() ;

  return std::string("") ;
}

/*
**  Function that search a NSRight Struct
*/
std::string
NSNodeRightArray::operator[](std::string temp)
{
  if (empty())
    return std::string("") ;
  return find(temp) ;
}

/*
**  Function that search a NSRight Struct
*/
void NSNodeRightArray::set(std::string node, std::string right)
{
	NSNodeRight* temp = new NSNodeRight(node, right) ;
	if (empty() == false)
	{
		std::vector<NSNodeRight* >::iterator iResult ;
		iResult = find_if(begin(), end(), RightTester(temp)) ;
		if (end() == iResult)
			push_back(temp) ;
		else
		{
			(*iResult)->setRight(right) ;
			if (NULL != temp)
				delete (temp) ;
		}
	}
	else
		push_back(temp) ;
}

/*
** Remove all the document excepted the right of the document itself
*/
void NSNodeRightArray::RemoveAllTreeNodeOfDocument(std::string doc)
{
	if (empty() == true)
		return ;

	std::vector<NSNodeRight*>::iterator i ;
	for (i = begin(); end() != i ; )
	{
		if (((*i)->getNode().compare(0, doc.size(), doc.c_str()) == 0) &&
        ((*i)->getNode().size() != doc.size()))
		{
			delete *i ;
			erase(i) ;
		}
		else
			i++ ;
	}
}

void NSNodeRightArray::RemoveDocument(std::string doc)
{
	if (empty() == true)
		return ;

	std::vector<NSNodeRight* >::iterator i;
	for (i = begin() ; end() != i ; )
	{
		if ( (*i)->getNode().compare(0, doc.size(), doc.c_str()) == 0)
		{
			delete *i ;
			erase(i) ;
		}
		else
			i++ ;
	}
}

// *************************************************************************
// *                          METHODES DE NSDataGraph                      *
// *************************************************************************

NSDataGraph::NSDataGraph(NSSuper* pSuper, GRAPHTYPE typeGraph) : NSSuperRoot(pSuper)
{
	setGraphType(typeGraph) ;
	setGraphID("") ;
	setLastTree("") ;

	_lObjectCount++ ;
}

NSDataGraph::NSDataGraph(const NSDataGraph& rv)
            :NSSuperRoot(rv._pSuper)
{
  _sGraphID   = rv._sGraphID;
  _iTypeGraph = rv._iTypeGraph;
  _sLastTree  = rv._sLastTree;

  _aTrees  = rv._aTrees ;
  _aLinks  = rv._aLinks ;
  _aArchs  = rv._aArchs ;
  _aRights = rv._aRights ;

	_lObjectCount++ ;
}

NSDataGraph::~NSDataGraph()
{
	_lObjectCount-- ;
}

NSDataGraph&
NSDataGraph::operator=(const NSDataGraph& src)
{
	if (&src == this)
  	return *this ;

  _sGraphID   = src._sGraphID ;
  _iTypeGraph = src._iTypeGraph ;
  _sLastTree  = src._sLastTree ;

  _aTrees  = src._aTrees ;
  _aLinks  = src._aLinks ;
  _aArchs  = src._aArchs ;
  _aRights = src._aRights ;

  return *this ;
}

int
NSDataGraph::operator==(const NSDataGraph& o)
{
	return ((_aTrees == o._aTrees) && (_aLinks == o._aLinks) && (_aArchs == o._aArchs)) ;
}

void
NSDataGraph::graphReset()
{
  _sGraphID = string("") ;
  setLastTree("") ;

  _aTrees.vider() ;
  _aLinks.vider() ;
  _aArchs.vider() ;
  _aRights.vider() ;
}

void
NSDataGraph::setLastTree(string sLast)
{
	if (string("") == sLast)
	{
		if (graphPerson == _iTypeGraph)
		{
    	string sPerson ;
      if (string("") != _sGraphID)
      	sPerson = string(_sGraphID, 0, PAT_NSS_LEN) ;
      else
      	sPerson = string(PAT_NSS_LEN, '#') ;
      string sDocum = string(DOC_CODE_DOCUM_LEN, '0') ;
      sDocum[0] = '#' ;
      _sLastTree = sPerson + sDocum ;
    }
    else
    {
    	string sObject = string(OBJECT_ID_LEN, '0') ;
      sObject[0] = '$' ;
      sObject[1] = '#' ;
      _sLastTree = sObject ;
    }
	}
  else if (strlen(sLast.c_str()) == OBJECT_ID_LEN)
  {
		_sLastTree = sLast ;
  }
	else
	{
  	string sErrorText = _pSuper->getText("graphAndTreeErrors", "invalidObjectGraphCounter") + string(" setLastTree(") + sLast + string(")") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		_sLastTree = string("") ;
	}

	if (false == _aRights.empty())
		for (NSNodeRightIter k = _aRights.begin(); _aRights.end() != k ; )
		{
    	if ((*k)->getNode() == string(""))
      {
      	delete (*k) ;
        _aRights.erase(k) ;
      }
      else
      	k++ ;
		}
}

string
NSDataGraph::getLastContribution()
{
	if (_aTrees.empty())
		return string("") ;

	PatPathoIter iter, iterDate ;
	string sLastDate = string("000000000000") ;
	string sLastContribution, sDateContrib, sTemp ;

	NSPatPathoArray PatPathoArray(_pSuper) ;

	for (NSDataTreeIter i = _aTrees.begin(); _aTrees.end() != i ; i++)
	{
  	PatPathoArray.vider() ;
    (*i)->getRawPatPatho(&PatPathoArray) ;

    if (false == PatPathoArray.empty())
    {
    	iter = PatPathoArray.begin() ;

      if (string("LCTRI1") == (*iter)->getLexique())
      {
      	iterDate = PatPathoArray.ChercherItem("KOUVR1") ;
        if (PatPathoArray.end() != iterDate)
        {
        	int iCol = (*iterDate)->getColonne() ;
          iterDate++ ;
          while ((PatPathoArray.end() != iterDate) && ((*iterDate)->getColonne() > iCol))
          {
          	string sUnite  = "" ;
            string sFormat = "" ;
            string sValeur = "" ;

            sTemp = (*iterDate)->getUnit() ;
            NSDico::donneCodeSens(&sTemp, &sUnite) ;
            sTemp = (*iterDate)->getLexique() ;
            NSDico::donneCodeSens(&sTemp, &sFormat) ;
            sValeur = (*iterDate)->getComplement() ;

            if (sUnite == "2DA02")
            	sDateContrib = sValeur ;

            iterDate++ ;
          }

          if (sDateContrib > sLastDate)
          {
          	sLastDate = sDateContrib ;
            sLastContribution = (*i)->getTreeID() ;
          }
        }
      }
    }
	}

	return sLastContribution ;
}

string
NSDataGraph::getNextTreeID()
{
	string sLast = getLastTree() ;

	if (graphPerson == _iTypeGraph)
	{
  	string sPerson = string(sLast, 0, PAT_NSS_LEN) ;
    string sDocum  = string(sLast, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;
    if (false == incrementeCode(sDocum))
    	return string("") ;

    sLast = sPerson + sDocum ;
	}
	else
	{
  	if (false == incrementeCode(sLast))
    	return string("") ;
	}

	setLastTree(sLast) ;
	return getLastTree() ;
}

bool
NSDataGraph::getTree(string sTreeID, NSPatPathoArray* pPatPatho, string* psDocRosace, bool bVerbose)
{
	if ((string("") == sTreeID) || ((string*) NULL == psDocRosace))
		return false ;

  *psDocRosace = _aRights.find(sTreeID) ;

  if ((NSPatPathoArray*) NULL == pPatPatho)
		return false ;

	NSDataTreeIter iterTree ;
//  pPatPatho->empty() ;

	if (false == _aTrees.ExisteTreeID(sTreeID, &iterTree))
	{
		// If we are looking for a collective or a group tree, it is not an error
    // if we can't find it (we may just have the meta)
  	string sLocalTreeID = getTreeIDFromID(sTreeID) ;
    GRAPHELEMTYPE treeType = getGraphElementTypeFromID(sLocalTreeID) ;
    if ((isCollectiveID == treeType) || (isGroupID == treeType))
    	return false ;

		string sErrorText = _pSuper->getText("graphAndTreeErrors", "noTreeWithThisIdInTheGraph") ;
		sErrorText += string(" getTree(") + sTreeID + string(")") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    if (bVerbose)
		  erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	// on récupère d'abord une copie de la patpatho de l'arbre trouvé
	(*iterTree)->getRawPatPatho(pPatPatho) ;
  if (pPatPatho->empty())
  	return true ;

	// on parcours l'array des modèles pour ajouter les sArchetype
	// aux noeuds de la nouvelle patpatho
  if (false == _aArchs.empty())
	{
  	string sArchetypeString = string("AR") ;

    PatPathoIter iBegin = pPatPatho->begin() ;

		for (NSArcLinkIter i = _aArchs.begin() ; _aArchs.end() != i ; i++)
		{
			if ((*i)->getType() == sArchetypeString)
			{
				if ((*i)->getObject() == (*iBegin)->getDoc())
				{
					string sNodeID = (*i)->getFullNode() ;
					PatPathoIter iterPatPat = pPatPatho->ChercherNoeud(sNodeID) ;
					if ((NULL != iterPatPat) && (pPatPatho->end() != iterPatPat))
					{
            if (_pSuper->getArcManager())
            {
						  string sModelName = _pSuper->getArcManager()->DonneNomArchetypeDansLibrairie(NSArcManager::archetype, (*i)->getMOId()) ;
						  (*iterPatPat)->setArchetype(sModelName) ;
            }
					}
				}
			}
		}
	}

  if (false == _aRights.empty())
	  for (PatPathoIter i = pPatPatho->begin() ; pPatPatho->end() != i ; i++)
	  {
		  string rootId  = (*i)->getNode() ;
		  string rightID = _aRights[rootId] ;
		  if (string("") != rightID)
			  (*i)->setNodeRight(rightID) ;
	  }

	return true ;
}

//
// WARNING: setTree doesn't call updateNodesTL... so beware that if free texts
//          have been changed, it will not be taken into account (don't forget
//          to call updateNodesTL before, because once the tree has been set
//          into the graph, updateNodesTL can no longer work
//
string
NSDataGraph::setTree(NSPatPathoArray* pPatPatho, string sDocRosace, string sCode)
{
try
{
	if (((NSPatPathoArray*) NULL == pPatPatho) || (pPatPatho->empty()))
		return string("") ;

	// bool bTreeIsRoot;
	bool   bNewTree  = true ;
	string sTreeID   = string("") ;
	string sLastNode = string(PPD_NOEUD_LEN, '0') ;
	sLastNode[0] = '#' ;
	string sCurrentNode ;

	NSLinkManager LinkManager(_pSuper, this) ;

	// on commence par vérifier si la patpatho est vierge
	// ou si elle détient déjà un treeID
	PatPathoIter itRoot = pPatPatho->begin() ;

  if (string("") != sCode)
  	sTreeID = sCode ;
  else
  	sTreeID = (*itRoot)->getDoc() ;

  if (string("") != sTreeID)
  {
  	if (strlen(sTreeID.c_str()) != OBJECT_ID_LEN)
    {
      string sErrorText = _pSuper->getText("graphAndTreeErrors", "tryingToAddATreeWithAnInvalidId") ;
			sErrorText += string(" setTree(") + sTreeID + string(")") ;
    	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
			erreur(sErrorText.c_str(), standardError) ;
      return string("") ;
    }

    bNewTree = !(_aTrees.ExisteTreeID(sTreeID)) ;
  }

	if (bNewTree)
	{
		// Dans ce cas, soit le graphe est vide et on récupère le compteur
		// initialisé à la création du graphe. Sinon on récupère le prochain
		// numéro d'arbre à instancier
		if ((getGraphID() == string("")) && (string("") == sCode))
		{
			sTreeID = getLastTree() ;
			setGraphID(sTreeID) ;
			// bTreeIsRoot = true ;
		}
		else if (string("") == sTreeID)
		{
			if (string("") == sCode)
				sTreeID = getNextTreeID() ;
			else
				// sTreeID =  pContexte->getPatient()->pGraphPerson->pDataGraph->getLastTree();
				sTreeID = sCode ;
			// bTreeIsRoot = false;
		}
		// else
		//	bTreeIsRoot = false ;
	}

	// Dans tous les cas, on commence par numéroter les nouveaux noeuds
	// de la patpatho et mettre à jour leurs arrays de liens en attente.
	for (PatPathoIter i = pPatPatho->begin() ; pPatPatho->end() != i ; i++)
	{
		sCurrentNode = (*i)->getNodeID() ;

		if ((string("") == (*i)->getDoc()) || ((*i)->getDoc() != sTreeID))
		{
      /*** On ne force plus une erreur ici car le cas NodeId != ""
           se produit fréquemment avec TreeId == "" dans le cas d'un InserePatPatho
           à partir d'une patpatho existante (car mal géré dans nspatpat).
           On choisi de remettre le noeud à "" car on doit avoir ici un nouveau noeud.
           ---------------------------------------------------------------------------
      if (sCurrentNode != "")
			{
				erreur("Un nouveau noeud possède déjà un identifiant.", standardError, 0) ;
				return "" ;
			}
      ***/

    	if (string("") != sCurrentNode)
      	sCurrentNode = string("") ;

			(*i)->setTreeID(sTreeID) ;
		}

		if (string("") == sCurrentNode)
		{
			// on fournit un identifiant temporaire au nouveau noeud.
			if (false == incrementeCode(sLastNode))
			{
        string sErrorText = _pSuper->getText("graphAndTreeErrors", "errorWhenGettingAnIdForANewNode") ;
				sErrorText += string(" incrementeCode(") + sLastNode + string(")") ;
    		_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
				return string("") ;
			}

			sCurrentNode = sLastNode ;

			(*i)->setNodeID(sCurrentNode) ;
			(*i)->numberTemporaryNodes(&LinkManager) ;
		}
		else
		{
			// cas des anciens noeuds : Si l'archetype du noeud est vide, on le
			// retire de l'array des modèles si il s'y trouve un autre pour ce noeud.
			if ((string("") == (*i)->getArchetype()) && (false == _aArchs.empty()))
			{
      	string sArchetypeString = string("AR") ;

				for (NSArcLinkIter j = _aArchs.begin(); _aArchs.end() != j ; )
				{
					if (((*j)->getType() == sArchetypeString) && ((*j)->getFullNode() == (*i)->getNode()))
					{
						delete (*j) ;
						_aArchs.erase(j) ;
					}
					else
						j++ ;
				}
			}
		}

		// Enfin, pour chaque noeud, si l'archetype est non vide, on l'ajoute à l'array des
		// modèles s'il n'existe pas déjà (sinon mise à jour)
		if (string("") != (*i)->getArchetype())
		{
			bool trouve = false ;

			if (false == _aArchs.empty())
			{
      	string sArchetypeString = string("AR") ;

				for (NSArcLinkIter j = _aArchs.begin(); _aArchs.end() != j ; j++)
				{
					if ((*j)->getType() == sArchetypeString)
					{
						if ((*j)->getFullNode() == (*i)->getNode())
						{
							(*j)->setMOId(_pSuper->getArcManager()->DonneNoeudArchetype(NSArcManager::archetype,
                                                                                                (*i)->getArchetype())) ;
							trouve = true ;
							break ;
						}
					}
				}
			}

			if (false == trouve)
			{
      	string sArchetypeString = string("AR") ;

      	NSArcManager *pLocalArcManager = _pSuper->getArcManager() ;
        string sModelID = pLocalArcManager->DonneNoeudArchetype(NSArcManager::archetype, (*i)->getArchetype()) ;
//				string sModelID = pContexte->getSuperviseur()->getArcManager()->DonneNoeudArchetype(NSArcManager::archetype, (*i)->getArchetype());
        if (string("") != sModelID)
				  _aArchs.push_back(new NSArcLink(_pSuper, (*i)->getDoc(), (*i)->getNodeID(), sArchetypeString, sModelID)) ;
			}
		}
	}


	/*
	** Gestion des rosaces de droits - Management of rights rosaces
	*/
	if (false == bNewTree)
		_aRights.RemoveDocument(sTreeID) ;

	if (string("") != sDocRosace)
		_aRights.set(sTreeID, sDocRosace) ;

	for (PatPathoIter i = pPatPatho->begin(); pPatPatho->end() != i ; i++)
	{
		string sRig = (*i)->getNodeRight() ;
		if (string("") != sRig)
			_aRights.set((*i)->getNode(), sRig) ;
	}

	if (bNewTree)
	{
		NSDataTree* pNewTree = new NSDataTree(_pSuper, sTreeID, _iTypeGraph) ;
		pNewTree->setPatPatho(pPatPatho) ;
		_aTrees.push_back(pNewTree) ;
	}
	else
	{
		NSDataTreeIter treeIter ;
		bool bExist = _aTrees.ExisteTreeID(sTreeID, &treeIter) ;
		if (bExist && treeIter)
			(*treeIter)->setPatPatho(pPatPatho) ;
	}

	return sTreeID ;
}
catch (...)
{
	erreur("Exception NSDataGraph::setTree", standardError) ;
	return "" ;
}
}

bool
NSDataGraph::updateNodesTL(NSPatPathoArray* pPatPatho, string sCode)
{
try
{
  if ((NULL == pPatPatho) || (pPatPatho->empty()))
		return false ;

	if (string("") == sCode)
	{
  	string sErrorText = _pSuper->getText("generalLanguageForFunctionCall", "emptyParameter") ;
		sErrorText += string(" updateNodesTL, sCode") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	bool   bNewTree = true ;
	string sTreeID  = string("") ;
	string sLastNode = string(PPD_NOEUD_LEN, '0') ;
	sLastNode[0] = INMEMORY_CHAR ;
	string sCurrentNode ;
	NSDataTreeIter iterTree = NULL ;

	// NSLinkManager* pLinkManager = new NSLinkManager(pContexte, this) ;

	// on commence par vérifier si la patpatho est nouvelle
	// ou si elle appartient déjà au graphe
	sTreeID = sCode ;
	if (strlen(sTreeID.c_str()) != OBJECT_ID_LEN)
	{
    string sErrorText = _pSuper->getText("graphAndTreeErrors", "invalidTreeId") ;
		sErrorText += string(" updateNodesTL - ") + sTreeID ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	bNewTree = !(_aTrees.ExisteTreeID(sTreeID, &iterTree)) ;

	if (bNewTree)
	{
		for (PatPathoIter i = pPatPatho->begin(); pPatPatho->end() != i ; i++)
		{
			sCurrentNode = (*i)->getNodeID() ;

			// Document sans Id, les noeuds n'ont pas d'identifiant, on leur en donne un
			if ((*i)->getDoc() == "")
				(*i)->setTreeID(sTreeID) ;

			// Noeud ancien
			//
			if (string("") != sCurrentNode)
			{
      	string sErrorText = _pSuper->getText("graphAndTreeErrors", "aNewNodeAlreadyOwnsAnId") ;
				sErrorText += string(" updateNodesTL - ") + sCurrentNode ;
    		_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
				return false;
			}
		}
		return true ;
	}

	// ATTENTION : Ici le getPatPatho ne fonctionne pas comme le getTree
	// il ne replace pas notament les sArchetype de la patpatho, qui sont donc vides.
	// Il faut donc éviter de tester les archetypes et les droits dans le changement
	// de noeud par la suite (ici juste en dessous)
	NSPatPathoArray OldPatPathoArray(_pSuper) ;
	(*iterTree)->getRawPatPatho(&OldPatPathoArray) ;

	for (PatPathoIter i = pPatPatho->begin() ; pPatPatho->end() != i ; i++)
	{
		sCurrentNode = (*i)->getNodeID() ;

		// Document sans Id, les noeuds n'ont pas d'identifiant, on leur en donne un
		if ((*i)->getDoc() == string(""))
			(*i)->setTreeID(sTreeID) ;

		// Noeud ancien
		//
		if (string("") != sCurrentNode)
		{
			string sNoeud = sTreeID + sCurrentNode ;
			PatPathoIter iterNode = OldPatPathoArray.ChercherNoeud(sNoeud) ;
			if ((OldPatPathoArray.end() != iterNode) && (NULL != iterNode))
			{
				if (false == (*i)->estMemeNode(*iterNode))
				{
					// if (i == pPatPatho->begin())
					//     ::MessageBox(0, "Attention : Reset du noeud root", "Message Nautilus", MB_OK);

          // We are in the case where a node's content has changed
          // It appears to be a good idea to swap all previous links
          //
          NSLinkManager LinkManager(_pSuper, this) ;
          LinkManager.swapLiens(sNoeud, *i) ;

					(*i)->setNodeID("") ;
				}

				delete *iterNode ;
				OldPatPathoArray.erase(iterNode) ;
			}
			else
			{
        string sErrorText = _pSuper->getText("graphAndTreeErrors", "aNodeDisappearedFromGraph") ;
				sErrorText += string(" updateNodesTL - ") + sNoeud ;
    		_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
				return false ;
			}
		}
	}

	return true ;
}
catch (...)
{
	erreur("Exception NSDataGraph::updateNodesTL", standardError) ;
	return false ;
}
}

/**
* Suppression de l'arbre d'index sTreeID du graphe
* Removing tree whose index is sTreeID from graph
*/
bool
NSDataGraph::removeTree(string sTreeID)
{
try
{
	if (string("") == sTreeID)
		return false ;

	NSPatPathoArray PatPatho(_pSuper) ;

	NSDataTreeIter iterTree ;

	if (_aTrees.ExisteTreeID(sTreeID, &iterTree))
		(*iterTree)->getRawPatPatho(&PatPatho) ;
	else
		return false ;

	NSLinkManager LinkManager(_pSuper, this) ;

	// destruction des liens
	if (false == LinkManager.detruireTousLesLiens(sTreeID))
	{
  	string sErrorText = _pSuper->getText("graphAndTreeErrors", "cannotRemoveLinksForTree") ;
		sErrorText += string(" detruireTousLesLiens(") + sTreeID + string(")") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
		erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	// destruction des archetypes reliés au tree
  if ((false == PatPatho.empty()) && (false == _aArchs.empty()))
	{
  	string sArchetypeString = string("AR") ;

		for (PatPathoIter i = PatPatho.begin() ; PatPatho.end() != i ; i++)
		{
			if ((*i)->getArchetype() != "")
			{
				for (NSArcLinkIter j = _aArchs.begin(); _aArchs.end() != j ; )
				{
					if (((*j)->getType() == sArchetypeString) && ((*j)->getFullNode() == (*i)->getNode()))
					{
						delete (*j) ;
						_aArchs.erase(j) ;
					}
					else
						j++ ;
				}
			}
		}
	}

	// destruction des droits
	_aRights.RemoveAllTreeNodeOfDocument(sTreeID);

	// on détruit l'arbre
	delete (*iterTree) ;
	_aTrees.erase(iterTree) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSDataGraph::removeTree", standardError) ;
	return false ;
}
}

bool
NSDataGraph::replaceTree(string sTreeID, string sNewTreeID, NSPatPathoArray* pNewPatPatho, string sNewDocRosace)
{
try
{
	NSDataTreeIter iterTree ;

	if (_sGraphID == sTreeID)
		setGraphID(sNewTreeID) ;

	// Remplacement de l'arbre
	if (_aTrees.ExisteTreeID(sTreeID, &iterTree))
	{
		(*iterTree)->setPatPatho(pNewPatPatho) ;
		(*iterTree)->setTreeID(sNewTreeID) ;
	}
	else
		return false ;

	// Remplacement des liens
	// Attention : gère les liens au niveau Tree pas au niveau Noeud
	if (false == _aLinks.empty())
	{
		for (NSPatLinkIter i = _aLinks.begin() ; _aLinks.end() != i ; i++)
		{
			if ((*i)->getQualifie() == sTreeID)
      	(*i)->setQualifie(sNewTreeID) ;

			if ((*i)->getQualifiant() == sTreeID)
      	(*i)->setQualifiant(sNewTreeID) ;
		}

		// Suppression des doublons
		// Note : on conserve i++ car on erase forcément l'élément après i
		for (NSPatLinkIter i = _aLinks.begin() ; _aLinks.end() != i ; i++)
		{
			NSPatLinkIter j = i ;
			j++ ;
			for ( ; _aLinks.end() != j ; )
			{
				if ((*(*i)) == (*(*j)))
				{
					delete (*j) ;
					_aLinks.erase(j) ;
				}
				else
					j++ ;
			}
		}
	}

	// Remplacement des modeles : impossible sans table de correspondance

	// Remplacement des droits : seulement au niveau tree
	_aRights.RemoveDocument(sTreeID) ;

	if (string("") != sNewDocRosace)
		_aRights.set(sNewTreeID, sNewDocRosace) ;

	return true ;
}
catch (...)
{
	erreur("Exception NSDataGraph::replaceTree", standardError) ;
	return false ;
}
}

bool
NSDataGraph::signChanges(string sTreeID, string sContributionID)
{
  // if (pContexte->getPatient() == NULL)
  //  return false ;

  if (strlen(sTreeID.c_str()) != (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN))
  {
  	erreur("TreeID de longueur non conforme.", standardError) ;
    return false ;
  }

  NSLinkManager   LinkManager(_pSuper, this) ;
  NSPatPathoArray PatPatho(_pSuper) ;
  NSDataTreeIter  iterTree ;
  string sCurrentNode ;
  string sFatherNode ;
  bool bInMemoryTree = true;

  if (false == _aTrees.ExisteTreeID(sTreeID, &iterTree))
  	return false ;

  (*iterTree)->getRawPatPatho(&PatPatho) ;

  if (PatPatho.empty())
  	return false ;

  // Première passe : on regarde si tous les noeuds sont nouveaux ou pas
  for (PatPathoIter i = PatPatho.begin(); PatPatho.end() != i ; i++)
  {
  	sCurrentNode = (*i)->getNodeID() ;
    if (INMEMORY_CHAR != sCurrentNode[0])
    {
    	bInMemoryTree = false ;
      break ;
    }
  }

  if (bInMemoryTree)
  {
  	// Dans ce cas on crée un nouveau lien contribution avec le TreeID
    if (false == LinkManager.existeLien(sTreeID, NSRootLink::contributionAdded, sContributionID))
    {
    	if (false == LinkManager.etablirLien(sTreeID, NSRootLink::contributionAdded, sContributionID))
      {
      	string sErrorText = _pSuper->getText("documentManagementErrors", "cannotEstablishALinkWithTheContribution") ;
        _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
        erreur(sErrorText.c_str(), standardError) ;
      }
    }
    return true ;
  }

  // Deuxième passe : on est surs ici qu'il s'agit d'un arbre à modifier
  // on regarde pour chaque noeud en # si son pere est en #. Si ce n'est pas le cas,
  // ce nouveau noeud doit etre relié à la contribution en cours.
  for (PatPathoIter i = PatPatho.begin() ; PatPatho.end() != i ; i++)
  {
  	sCurrentNode = (*i)->getNodeID() ;
    if (INMEMORY_CHAR == sCurrentNode[0])
    {
    	PatPathoIter iterPere = PatPatho.ChercherPere(i) ;
      if (iterPere == NULL)
      	sFatherNode = "" ;
      else
      	sFatherNode = (*iterPere)->getNodeID() ;

      if (INMEMORY_CHAR != sFatherNode[0])
      {
      	if (false == LinkManager.existeLien((*i)->getNode(), NSRootLink::contributionModified, sContributionID))
        {
        	if (false == LinkManager.etablirLien((*i)->getNode(), NSRootLink::contributionModified, sContributionID))
          {
          	string sErrorText = _pSuper->getText("documentManagementErrors", "cannotEstablishALinkWithTheContribution") ;
						_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
						erreur(sErrorText.c_str(), standardError) ;
          }
        }
      }
    }
  }

  return true ;
}

string
NSDataGraph::genereXML()
{
	string sXML = string("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\r\n") ;
	sXML += string("<graph id = \"") + (*(_aTrees.begin()))->getTreeID() + string("\">\r\n") ;

	for (NSDataTreeIter i = _aTrees.begin() ; _aTrees.end() != i ; i++)
  {
  	sXML += string("<tree id = \"") + (*i)->getTreeID() + string("\">\r\n") ;
    sXML += (*i)->genereXML() ;
    sXML += string("</tree>\r\n") ;
  }

  string sNodeSource, sNodeTarget ;

	for (NSPatLinkIter j = _aLinks.begin() ; _aLinks.end() != j ; j++)
  {
  	sXML += string("<link source_object_id = \"") + string((*j)->getQualifie(), 0, OBJECT_ID_LEN) + string("\" ") ;
    sNodeSource = string((*j)->getQualifie(), OBJECT_ID_LEN, strlen((*j)->getQualifie().c_str()) - OBJECT_ID_LEN) ;
    if (strlen(sNodeSource.c_str()) == PPD_NOEUD_LEN)
    	sXML += string("source_node_id = \"") + sNodeSource + string("\" ") ;

    sXML += string("lien = \"") + (*j)->getLien() + string("\" ") ;

    sXML += string("target_object_id = \"") + string((*j)->getQualifiant(), 0, OBJECT_ID_LEN) + string("\" ") ;
    sNodeTarget = string((*j)->getQualifiant(), OBJECT_ID_LEN, strlen((*j)->getQualifiant().c_str()) - OBJECT_ID_LEN);
    if (strlen(sNodeTarget.c_str()) == PPD_NOEUD_LEN)
    	sXML += string("target_node_id = \"") + sNodeTarget + string("\" ") ;
    sXML += string("/>\r\n") ;
  }

  string sNode ;

  for (NSArcLinkIter k = _aArchs.begin() ; _aArchs.end() != k ; k++)
  {
  	sXML += string("<model object_id = \"") + (*k)->getObject() + string("\" ") ;
    sNode = (*k)->getNode() ;
    if (strlen(sNode.c_str()) == PPD_NOEUD_LEN)
    	sXML += string("node_id = \"") + sNode + string("\" ") ;
    sXML += string("type = \"") + (*k)->getType() + string("\" ") ;
    sXML += string("mod_object_id = \"") + (*k)->getMOId() + string("\" />\r\n") ;
  }

  sXML += string("</graph>\r\n") ;

	return sXML ;
}

bool
NSDataGraph::incrementeCode(string& sCompteur)
{
	if (string("") == sCompteur)
  	return false ;

	//
	// On incrémente le compteur
	//
	int calculer = 1;

	int i = strlen(sCompteur.c_str()) - 1 ;  int j = 0 ;
	while (calculer)  {
    j = (int) sCompteur[i] ;
    j++ ;

    if (((j >= '0') && (j <= '9')) || ((j >= 'A') && (j <= 'Z')))    {
      sCompteur[i] = (char) j ;
      calculer = 0 ;
    }
    else if ((j > '9') && (j < 'A'))
    {
      sCompteur[i] = 'A' ;
      calculer = 0 ;
    }
    else
    {
      sCompteur[i] = '0' ;

      // on met l'index de fin à 1 à cause de l'identifiant local : '_'
      // ou de l'identifiant temporaire : '#', placé au début du compteur
      // Pour les objets, on s'arrete à 2 à cause de l'identifiant d'objet ($)
      if (((graphPerson == _iTypeGraph) && (i == 1)) ||          ((graphObject == _iTypeGraph) && (i == 2)))      {
        erreur("Compteur saturé.", standardError) ;
        return false ;
      }

      i--;    }
  }

	return true ;}

// -----------------------------------------------------------------------------
// METHODES DE NSObjectGraphManager
// -----------------------------------------------------------------------------
NSObjectGraphManager::NSObjectGraphManager(NSSuper* pSuper)
                     :NSSuperRoot(pSuper)
{
try
{
  _pDataGraph   = new NSDataGraph(_pSuper, graphObject) ;
  _pLinkManager = new NSLinkManager(_pSuper, _pDataGraph) ;

  _lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSObjectGraphManager ctor", standardError) ;
}
}

NSObjectGraphManager::NSObjectGraphManager(const NSObjectGraphManager& rv)
                     :NSSuperRoot(rv._pSuper)
{
try
{
	_pDataGraph   = new NSDataGraph(*(rv._pDataGraph)) ;
  _pLinkManager = new NSLinkManager(_pSuper, _pDataGraph) ;

  _lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSObjectGraphManager copy ctor", standardError) ;
}
}

NSObjectGraphManager::~NSObjectGraphManager()
{
	if (_pDataGraph)
  	delete _pDataGraph ;

  if (_pLinkManager)
  	delete _pLinkManager ;

  _lObjectCount-- ;
}

NSObjectGraphManager&
NSObjectGraphManager::operator=(const NSObjectGraphManager& src)
{
	if (&src == this)
  	return *this ;

	if (_pDataGraph)
  	delete _pDataGraph ;
  _pDataGraph = new NSDataGraph(*(src._pDataGraph)) ;

  if (_pLinkManager)
  	delete _pLinkManager ;
  _pLinkManager = new NSLinkManager(_pSuper, _pDataGraph) ;

  return (*this) ;
}

/*
void
NSObjectGraphManager::graphReset()
{
	pDataGraph->graphReset() ;
}
*/


/*
void
NSObjectGraphManager::setRootObject(string sObjID)
{
	pDataGraph->setGraphID(sObjID) ;
}
*/


/*
string
NSObjectGraphManager::getRootObject()
{
	return (pDataGraph->getGraphID()) ;
}
*/


/*
bool
NSObjectGraphManager::getTree(string sObjectID, NSPatPathoArray *pPatPatho, string *psDocRosace)
{
	return pDataGraph->getTree(sObjectID, pPatPatho, psDocRosace) ;
}


string
NSObjectGraphManager::setTree(NSPatPathoArray *pPatPatho, string sDocRosace, string sCode)
{
	return pDataGraph->setTree(pPatPatho, sDocRosace, sCode) ;
}
*/


bool
NSObjectGraphManager::getGraph()
{
  // if ((sObjectID == "") || (pDataGraph == 0))
  //    return false ;

  // Prise de l'arbre de base
  // appel de getGraph
  // return pContexte->pPilot->getGraph(pDataGraph);
  return false ;
}


bool
NSObjectGraphManager::setGraph()
{
	return false;
}

bool
NSObjectGraphManager::exportXML(string sFichier)
{
	if (string("") == sFichier)
		return false ;

	ofstream outFile ;
  string   sOut ;

#ifdef _ENTERPRISE_DLL
  string sFichierOut = sFichier ;
#else
  string sFichierOut = _pSuper->PathName("EHTM") + sFichier ;
#endif

  outFile.open(sFichierOut.c_str()) ;
  if (!outFile)
  {
    erreur("Erreur d'ouverture en écriture du fichier graphtest.xml", standardError) ;
    outFile.close() ;
    return false ;
  }

  sOut = _pDataGraph->genereXML() ;

  for (size_t i = 0 ; i < strlen(sOut.c_str()) ; i++)
  	outFile.put(sOut[i]) ;

  outFile.close() ;
  return true ;
}

void
NSObjectGraphManager::ParseTemplate(NSPatPathoArray* pPatPathoArray, NSBasicAttributeArray *pTmplArray)
{
	if ((NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return ;

	string sTraitType = string("_0OTPL") + string("_0TYPE") ;
  string sTraitOper = string("_0OTPL") + string("_DOPER") ;
  string sTraitCons = string("_0OTPL") + string("_LNUCO") ;
  string sTraitRoot = string("_0OTPL") + string("_0TYPC") ;
  string sTraitComp = string("_0OTPL") + string("_0COMD") ;
  string sTraitDefa = string("_0OTPL") + string("_0DEFA") ;
  string sTraitFich = string("_0OTPL") + string("_0TPL0") ;
  string sTraitEnte = string("_0OTPL") + string("_0ENTE") ;
  string sTraitLibe = string("_0OTPL") + string("_0INTI") ;

  string sTmplTypeDoc = "", sTmplUtil = "", sTmplCons = "" ;
	string sTmplLexRoot = "", sTmplCompo = "0", sTmplDefaut = "0" ;
  string sTmplFichier = "", sTmplEnTete = "", sTmplLibelle = "" ;

	string sType ;
	string sTemp = ""  ;

	PatPathoIter iter = pPatPathoArray->begin() ;
	int iColBase = (*iter)->getColonne() ;
	iter++ ;

	while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // Type document
    if (string("0TYPE") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        sTmplTypeDoc = sElemLex ;
        iter++ ;
      }
    }
    // Operateur
    else if (string("DOPER") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string(sElemLex, 0, 5) == string("£SPID"))
        	sTmplUtil = (*iter)->getComplement() ;

        iter++ ;
      }
    }
    // numéro de console
    else if (string("LNUCO") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string(sElemLex, 0, 3) == string("£C;"))
        	sTmplCons = (*iter)->getComplement() ;

        iter++ ;
      }
    }
    // lexique root
    else if (string("0TYPC") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        sTmplLexRoot = sElemLex ;
        iter++ ;
      }
    }
    // flag composition
    else if (string("0COMD") == sSens)
    {
    	iter++ ;
      sTmplCompo = "1" ;
    }
    // flag defaut
    else if (string("0DEFA") == sSens)
    {
    	iter++ ;
      sTmplDefaut = "1" ;
    }
    // Fichier Template
    else if (string("0TPL0") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
        	sTmplFichier = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // Fichier En-tete
    else if (string("0ENTE") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
        	sTmplEnTete = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // Libelle
    else if (string("0INTI") == sSens)
    {
    	iter++ ;
      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (sElemLex == string("£?????"))
        	sTmplLibelle = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
    	iter++ ;
	} // boucle principale

  pTmplArray->push_back(new NSBasicAttribute(sTraitType, sTmplTypeDoc)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitOper, sTmplUtil)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitCons, sTmplCons)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitRoot, sTmplLexRoot)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitComp, sTmplCompo)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitDefa, sTmplDefaut)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitFich, sTmplFichier)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitEnte, sTmplEnTete)) ;
  pTmplArray->push_back(new NSBasicAttribute(sTraitLibe, sTmplLibelle)) ;
}

#ifndef _ENTERPRISE_DLL

string
NSAddressGraphManager::CalculeClefLieu(NSContexte *pContexte, NSPatPathoArray *pPatPathoObject, string sLang)
{
	if (((NSPatPathoArray*) NULL == pPatPathoObject) || (pPatPathoObject->empty()))
  	return string("") ;

  string sLieu = string("") ;

  // on récupère le nom du site et le numéro dans la voie
  PatPathoIter iter = pPatPathoObject->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPatPathoObject->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "nom" / name chapter
    if (sSens == string("LTYPA"))
    {
      iter++;
      while ((pPatPathoObject->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string(sElemLex, 0, 3) == string("£C;"))
        {
          iter++ ;
          while ((pPatPathoObject->end() != iter) && ((*iter)->getColonne() > iColBase + 2))
          {
            // on cherche ici un code lexique pour un libelle
            string sCode     = (*iter)->getLexique() ;
            string sTypeLieu = string("") ;
            pContexte->getDico()->donneLibelle(sLang, &sCode, &sTypeLieu) ;
            sLieu += string("|") + sTypeLieu ;

            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    else if (sSens == string("LNOMA"))
    {
      string sNomLieu = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sNomLieu)
        sLieu += string("|") + sNomLieu ;
    }
    else if (sSens == string("LETAG"))
    {
      string sEtag = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sEtag)
        sLieu += string("|") + sEtag ;
    }
    else if (sSens == string("LESCA"))
    {
      string sEsca = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sEsca)
        sLieu += string("|") + sEsca ;
    }
    else if (sSens == string("LCODP"))
    {
      string sCodp = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sCodp)
        sLieu += string("|") + sCodp ;
    }
    else if (sSens == string("LNUMP"))
    {
      string sNump = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sNump)
        sLieu += string("|") + sNump ;
    }
    else if (sSens == string("LCOTP"))
    {
      string sCotp = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sCotp)
        sLieu += string("|") + sCotp ;
    }
    else if (sElemLex == string("LTELE1"))
    {
      string sTele1 = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sTele1)
        sLieu += string("|") + sTele1 ;
    }
    else if (sElemLex == string("LTELE2"))
    {
      string sTele2 = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sTele2)
        sLieu += string("|") + sTele2 ;
    }
    else if (sSens == string("LNPOS"))
    {
      string sNpos = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sNpos)
        sLieu += string("|") + sNpos ;
    }
    else if (sSens == string("LFAX0"))
    {
      string sFax = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sFax)
        sLieu += string("|") + sFax ;
    }
    else
      iter++ ;
  }

  return strpids(sLieu) ;
}

string
NSAddressGraphManager::CalculeClefSite(NSPatPathoArray *pPatPathoObject)
{
	if (((NSPatPathoArray*) NULL == pPatPathoObject) || pPatPathoObject->empty())
    return string("") ;

  string sSite = string("") ;

  // on récupère le nom du site et le numéro dans la voie
  //
  PatPathoIter iter = pPatPathoObject->begin() ;
  int iColBase = (*iter)->getColonne() ;

  iter++ ;

  while ((pPatPathoObject->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "nom" / name chapter
    if (sSens == string("LNOMA"))
    {
      string sNomSite = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sNomSite)
        sSite += string("|") + sNomSite ;
    }
    // lieu pour les adresses simplifiées
    else if (sSens == string("LLIEU"))
    {
      string sLieu = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sLieu)
        sSite += string("|") + sLieu ;
    }
    else if (sSens == string("LVOIE"))
    {
      iter++ ;

      sElemLex = (*iter)->getLexique() ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;

      if (sSens == string("LNUMA"))
      {
        string sNumVoie = pPatPathoObject->getFreeTextValue(&iter) ;

        if (string("") != sNumVoie)
          sSite += string("|") + sNumVoie ;
      }
      else
        iter++ ;
    }
    else if (sSens == string("LBATI"))
    {
      string sBati = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sBati)
        sSite += string("|") + sBati ;
    }
    else if (sSens == string("LESCA"))
    {
      string sEsca = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sEsca)
        sSite += string("|") + sEsca ;
    }
    else if (sSens == string("LCODP"))
    {
      string sCodp = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sCodp)
        sSite += string("|") + sCodp ;
    }
    else if (sSens == string("LCODS"))
    {
      string sCods = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sCods)
        sSite += string("|") + sCods ;
    }
    else if (sSens == string("LBPOA"))
    {
      string sBp = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sBp)
        sSite += string("|") + sBp ;
    }
    else if (sSens == string("LLIED"))
    {
      string sLied = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sLied)
        sSite += string("|") + sLied ;
    }
    // téléphone pour les adresses simplifiées
    else if (sSens == string("LTELE"))
    {
      string sTele = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sTele)
        sSite += string("|") + sTele ;
    }
    else if (sSens == string("LTELA"))
    {
      string sTela = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sTela)
        sSite += string("|") + sTela ;
    }
    else if (sSens == string("LTELS"))
    {
      string sTels = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sTels)
        sSite += string("|") + sTels ;
    }
    else if (sSens == string("LFAX0"))
    {
      string sFax = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sFax)
        sSite += string("|") + sFax ;
    }
    else
      iter++ ;
  }

  return strpids(sSite) ;
}


string
NSAddressGraphManager::CalculeClefVoie(NSContexte *pContexte, NSPatPathoArray *pPatPathoObject, string sLang)
{
  if (((NSPatPathoArray*) NULL == pPatPathoObject) || pPatPathoObject->empty())
    return string("") ;

  string sVoie = string("") ;

  // on récupère le nom du site et le numéro dans la voie
  PatPathoIter iter = pPatPathoObject->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPatPathoObject->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // Chapitre "nom" / name chapter
    if (sSens == string("LTYPA"))
    {
      iter++ ;
      while ((iter != pPatPathoObject->end()) && ((*iter)->getColonne() > iColBase + 1))
      {
        // on cherche ici un texte libre
        sElemLex = (*iter)->getLexique() ;
        if (string(sElemLex, 0, 3) == string("£C;"))
        {
          iter++ ;
          while ((iter != pPatPathoObject->end()) && ((*iter)->getColonne() > iColBase + 2))
          {
            // on cherche ici un code lexique pour un libelle
            string sCode     = (*iter)->getLexique() ;
            string sTypeVoie = string("") ;
            pContexte->getDico()->donneLibelle(sLang, &sCode, &sTypeVoie) ;
            sVoie += string("|") + sTypeVoie ;
            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    else if (sSens == string("LNOMA"))
    {
      string sNomVoie = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sNomVoie)
        sVoie += string("|") + sNomVoie ;
    }
    else
      iter++ ;
  }

  return strpids(sVoie) ;
}

string
NSAddressGraphManager::CalculeClefVille(NSPatPathoArray *pPatPathoObject)
{
  if (((NSPatPathoArray*) NULL == pPatPathoObject) || pPatPathoObject->empty())
    return string("") ;

  string sVille = string("") ;

  // on récupère le nom du site et le numéro dans la voie
  PatPathoIter iter = pPatPathoObject->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((iter != pPatPathoObject->end()) && ((*iter)->getColonne() > iColBase))
  {
    string sElemLex = (*iter)->getLexique() ;
    string sSens    = string("") ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;

    // Code Postal / Zip Code
    if (sSens == string("LZIP0"))
    {
      string sZip = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sZip)
        sVille += string("|") + sZip ;
    }
    else if (sSens == string("LCEDX"))
    {
      string sCedx = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sCedx)
        sVille += string("|") + sCedx ;
    }
    else if (sSens == string("LCOMU"))
    {
      string sComu = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sComu)
        sVille += string("|") + sComu ;
    }
    else if (sSens == string("LPAYS"))
    {
      string sPays = pPatPathoObject->getFreeTextValue(&iter) ;

      if (string("") != sPays)
        sVille += string("|") + sPays ;
    }
    else
      iter++ ;
  }

  return strpids(sVille) ;
}

bool
NSAddressGraphManager::getGraphAdr(string sObjID, NSPatPathoArray *pPatPathoArray)
{
try
{
  // remise à zéro du graphe
  _pDataGraph->graphReset() ;
  string sObject = sObjID ;

  setRootObject(sObjID) ;

	NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(OBJECT, sObjID)) ;
  AttrList.push_back(new NSBasicAttribute(LINK, "OI")) ;
  AttrList.push_back(new NSBasicAttribute(DIRECTION, FORWARD)) ;
//  pAttrList->push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;

  int res = _pSuper->getPilot()->readAdressGraph(NautilusPilot::SERV_READ_ADRESS_GRAPH.c_str(), _pDataGraph, &AttrList) ;
  if (!res)
  {
  	erreur("Echec service à la récupération du graphe d'adresse.", standardError, 0) ;
    return false ;
  }

  // constitution de la patpatho contenant tous les arbres du graphe
  if (pPatPathoArray)
  {
    pPatPathoArray->vider() ;

    if (false == _pDataGraph->getTrees()->empty())
    {
      for (NSDataTreeIter i = _pDataGraph->getTrees()->begin() ; _pDataGraph->getTrees()->end() != i ; i++)
      {
      	NSPatPathoArray Ppt(_pSuper) ;
        (*i)->getRawPatPatho(&Ppt) ;
        pPatPathoArray->ajouteVecteur(&Ppt, ORIGINE_PATH_PATHO) ;
      }
    }
  }
  return true ;
}
catch (...)
{
  erreur("Exception NSObjectGraphManager::getGraphAdr", standardError, 0) ;
  return false ;
}
}

bool
NSAddressGraphManager::setGraphAdr(NSContexte *pContexte, NSPatPathoArray *pPatPathoArray, string sLang, string sOperatorID)
{
	if ((NULL == pPatPathoArray) || (pPatPathoArray->empty()))
  	return false ;

  // pour cette méthode, on suppose qu'on a une patpatho "Adresse"
  // formée d'au plus 3 objets "adresse" distincts (lieu, site, ville).
  // La sous-patpatho de chaque objet se trouve à chaque fois en colonne 0;
  // on récupère chaque sous-patpatho pour créer un nouvel arbre du graphe.

  NSVectPatPathoArray Vect  ;
  PatPathoIter iterRoot = pPatPathoArray->begin() ;
  pPatPathoArray->ExtraireVecteurPatPathoFreres(iterRoot, &Vect) ;

  string sTypeFrere ;
  bool bExisteLieu = false ;
  bool bExisteSite = false ;
  bool bExisteVoie = false ;
  bool bExisteVille = false ;
  string sNodeLieu = "", sNodeSite = "", sNodeVoie = "", sNodeVille = "" ;
  NSBasicAttributeArray AttrList ;

  PatPathoIterVect iterVect = Vect.begin() ;
  // Création des arbres du graphe
  while (Vect.end() != iterVect)
  {
    // Le type de chaque frere est donné par le code lexique de sa racine
    iterRoot   = (*iterVect)->begin() ;
    sTypeFrere = (*iterRoot)->getLexique() ;

    if (((sTypeFrere == string("LLIEU1")) && (!bExisteLieu)) ||
        ((sTypeFrere == string("LSITE1")) && (!bExisteSite)) ||
        ((sTypeFrere == string("LVOIE1")) && (!bExisteVoie)) ||
        ((sTypeFrere == string("LVILL1")) && (!bExisteVille)))
    {
      // on regarde dans le graphe si un élément de ce type
      // existe déjà, auquel cas on doit le mettre à jour
      // NSDataTreeIter iterTree ;
      string         sNodeFrere ;

      /*
      // Note : on suppose maintenant partir d'un graphe vierge à chaque fois
      if (pDataGraph->aTrees.ExisteTree(sTypeFrere, pContexte, &iterTree))
      {
      	pDataGraph->removeTree((*iterTree)->getTreeID()) ;
      }
      */

      sNodeFrere = setTree(*iterVect, "") ;

      if (sTypeFrere == string("LLIEU1"))
      {
        sNodeLieu = sNodeFrere ;
        AttrList.push_back(new NSBasicAttribute("_LLIEU_LLIEU", CalculeClefLieu(pContexte, *iterVect, sLang))) ;
        bExisteLieu = true ;
      }
      else if (sTypeFrere == string("LSITE1"))
      {
        sNodeSite = sNodeFrere ;
        AttrList.push_back(new NSBasicAttribute("_LSITE_LSITE", CalculeClefSite(*iterVect))) ;
        bExisteSite = true ;
      }
      else if (sTypeFrere == string("LVOIE1"))
      {
        sNodeVoie = sNodeFrere ;
        AttrList.push_back(new NSBasicAttribute("_LVOIE_LVOIE", CalculeClefVoie(pContexte, *iterVect, sLang))) ;
        bExisteVoie = true ;
      }
      else if (sTypeFrere == string("LVILL1"))
      {
        sNodeVille = sNodeFrere ;
        AttrList.push_back(new NSBasicAttribute("_LVILL_LVILL", CalculeClefVille(*iterVect))) ;
        bExisteVille = true ;
      }
    }
    iterVect++ ;
  }

  // Si certains objets du graphe ont disparu, il faut les retirer
  /*
  // Note : obsolète on ne gère plus la modification
  NSDataTreeIter iterTree ;

  if ((!bExisteLieu) && (pDataGraph->aTrees.ExisteTree("LLIEU1", pContexte, &iterTree)))
  {
  	pDataGraph->removeTree((*iterTree)->getTreeID()) ;
  }

  if ((!bExisteSite) && (pDataGraph->aTrees.ExisteTree("LSITE1", pContexte, &iterTree)))
  {
  	pDataGraph->removeTree((*iterTree)->getTreeID()) ;
  }

  if ((!bExisteVoie) && (pDataGraph->aTrees.ExisteTree("LVOIE1", pContexte, &iterTree)))
  {
  	pDataGraph->removeTree((*iterTree)->getTreeID()) ;
  }

  if ((!bExisteVille) && (pDataGraph->aTrees.ExisteTree("LVILL1", pContexte, &iterTree)))
  {
  	pDataGraph->removeTree((*iterTree)->getTreeID()) ;
  }
	*/

  // Création des liens du graphe
  // on cherche quel est l'objet "root" et on reconstuit ses liens
  // avec les autres objets du graphe

  if (bExisteLieu)
  {
    // si le lieu existe, il devient le "root" du graphe
    setRootObject(sNodeLieu) ;

    // Le lieu est relié soit à un site, soit à une ville
    if (bExisteSite)
    {
    	if (false == _pLinkManager->etablirLien(sNodeLieu, NSRootLink::objectIn, sNodeSite))
      {
        erreur("Impossible de créer le lien [lieu -> site] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
    else if (bExisteVoie)
    {
    	if (false == _pLinkManager->etablirLien(sNodeLieu, NSRootLink::objectIn, sNodeVoie))
      {
        erreur("Impossible de créer le lien [lieu -> voie] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
    else if (bExisteVille)
    {
			if (false == _pLinkManager->etablirLien(sNodeLieu, NSRootLink::objectIn, sNodeVille))
      {
        erreur("Impossible de créer le lien [lieu -> ville] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
  }

  if (bExisteSite)
  {
    // on regarde s'il existe un lieu
    if (!bExisteLieu)
        setRootObject(sNodeSite) ;

    // Le site est relié à une voie ou à une ville
    if (bExisteVoie)
    {
    	if (false == _pLinkManager->etablirLien(sNodeSite, NSRootLink::objectIn, sNodeVoie))
      {
        erreur("Impossible de créer le lien [site -> voie] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
    else if (bExisteVille)
    {
    	if (false == _pLinkManager->etablirLien(sNodeSite, NSRootLink::objectIn, sNodeVille))
      {
      	erreur("Impossible de créer le lien [site -> ville] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
  }

  if (bExisteVoie)
  {
    // on regarde s'il existe un lieu
    if ((!bExisteLieu) && (!bExisteSite))
    	setRootObject(sNodeVoie) ;

    // La voie est reliée à une ville
    if (bExisteVille)
    {
      if (false == _pLinkManager->etablirLien(sNodeVoie, NSRootLink::objectIn, sNodeVille))
      {
        erreur("Impossible de créer le lien [voie -> ville] de l'objet adresse en cours.", standardError, 0) ;
        return false ;
      }
    }
  }

  // Une ville n'est reliée à rien mais peut
  // devenir le root si elle est unique
  if ((bExisteVille) && (!bExisteVoie) && (!bExisteSite) && (!bExisteLieu))
  	setRootObject(sNodeVille) ;

  // Le graphe est maintenant constitué. on doit appeler le pilote pour
  // trouver successivement les ID de tous les arbres du graphe
  // (et résoudre leurs dépendances)
  NSDataGraph LocalGraph(_pSuper, graphObject) ;
  string sTreeID, sTreeRights, sElemLex, sSens ;
  string sAttr, sValue, sNewTreeID ;
  NSPatPathoArray PatPatho(_pSuper, graphObject) ;
  bool res ;
  NSPersonsAttributesArray ObjectsList ;
//  pAttrList->push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;
  string sPrecObjID = "" ;
  string sCodeObj = string("LVILL1") ;
  bool bExist ;
  bool tourner = true ;
  NSDataTreeIter iterTree ;

  // Première passe : on crée l'objet pour chaque objet-mémoire ('#')
  while (tourner)
  {
    bExist = _pDataGraph->getTrees()->ExisteTree(sCodeObj, _pSuper, &iterTree) ;
    if (bExist) // En principe (sTreeID[0] == '#')
    {
      sTreeID = (*iterTree)->getTreeID() ;
      LocalGraph.graphReset() ;
      PatPatho.vider() ;

      _pDataGraph->getTree(sTreeID, &PatPatho, &sTreeRights) ;

      sElemLex = (*(PatPatho.begin()))->getLexique() ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;
      sAttr = string("_") + sSens + string("_") + sSens;
      sValue = AttrList.getAttributeValue(sAttr) ;
      if (sPrecObjID != "")
      {
        sValue += string("|") + sPrecObjID ;
        AttrList.setAttributeValue(sAttr, sValue) ;
      }

      LocalGraph.setTree(&PatPatho, sTreeRights, sTreeID) ;

      // Appel pilote : création de l'objet en local
      NSBasicAttributeArray List ;
      List.push_back(new NSBasicAttribute(sAttr, sValue)) ;

      // rustine EpiPump
/*
      if (sOperatorID == "")
      	List.push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;
      else
*/
      	List.push_back(new NSBasicAttribute("user", sOperatorID)) ;

      NSPersonsAttributesArray ObjectsLocalList ;

      res = _pSuper->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(), &LocalGraph, &ObjectsLocalList, &List, OBJECT_TYPE, false) ;
      if (!res)
      {
        erreur("Echec service à la création du graphe d'adresse.", standardError, 0) ;
        return false ;
      }

      // cas des doublons
      if (false == ObjectsLocalList.empty())
      {
        string sOIDS = ObjectsLocalList.getAttributeValue(OIDS) ;
        LocalGraph.graphReset() ;
        List.vider() ;

        if (sOIDS != "")
        {
          //pList->push_back(new NSBasicAttribute("graphID", sOIDS)) ;
          List.push_back(new NSBasicAttribute(OBJECT, sOIDS)) ;
          res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), &LocalGraph,  &List) ;
          if (!res)
          {
            string sErrorText = _pSuper->getText("NTIERS", "objectNotFound") ;
            if (sErrorText == "")
              sErrorText = string("Echec service : Impossible de récupérer un objet dans la base") ;
            erreur(sErrorText.c_str(), standardError, 0) ;
            return false ;
          }
        }
      }

      // Remplacement du nouvel ID dans le DataGraph
      NSDataTreeIter iter = LocalGraph.getTrees()->begin() ;
      sNewTreeID = (*iter)->getTreeID() ;
      sPrecObjID = sNewTreeID ;

      NSPatPathoArray Ppt(_pSuper) ;
      (*iter)->getRawPatPatho(&Ppt) ;
      _pDataGraph->replaceTree(sTreeID, sNewTreeID, &Ppt, sTreeRights) ;

      // ajout des liens rattachés au doublon
      // Note : pour un doublon, on doit passer TOUS SES LIENS
      // sinon le pilote effacera les liens manquants
      if (false == ObjectsLocalList.empty())
      {
        for (NSPatLinkIter j = LocalGraph.getLinks()->begin() ; LocalGraph.getLinks()->end() != j ; j++)
        {
          string sQualifie   = (*j)->getQualifie() ;
          string sQualifiant = (*j)->getQualifiant() ;
          NSRootLink::NODELINKTYPES iRelation = _pLinkManager->donneRelation((*j)->getLien()) ;

          if (false == _pLinkManager->existeLien(sQualifie, iRelation, sQualifiant))
          	_pDataGraph->getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;
        }
      }
    }

    if (sCodeObj == "LVILL1")
    	sCodeObj = string("LVOIE1") ;
    else if (sCodeObj == "LVOIE1")
    	sCodeObj = string("LSITE1") ;
    else if (sCodeObj == "LSITE1")
    	sCodeObj = string("LLIEU1") ;
    else if (sCodeObj == "LLIEU1")
    	tourner = false ;
  }

  // Deuxième passe : on montre les liens au pilote objet par objet
  for (NSDataTreeIter i = _pDataGraph->getTrees()->begin() ; i != _pDataGraph->getTrees()->end() ; i++)
  {
    sTreeID = (*i)->getTreeID() ;
    LocalGraph.graphReset() ;
    PatPatho.vider() ;

    _pDataGraph->getTree(sTreeID, &PatPatho, &sTreeRights) ;

    sElemLex = string((*(PatPatho.begin()))->getLexique()) ;
    NSDico::donneCodeSens(&sElemLex, &sSens) ;
    sAttr = string("_") + sSens + string("_") + sSens ;
    sValue = AttrList.getAttributeValue(sAttr) ;

    // on met ici l'arbre et ses droits dans pLocalGraph
    LocalGraph.setTree(&PatPatho, sTreeRights, sTreeID) ;
    LocalGraph.setGraphID(sTreeID) ;

    // pour les liens, on prend seulement les liens de l'arbre
    for (NSPatLinkIter j = _pDataGraph->getLinks()->begin() ; j != _pDataGraph->getLinks()->end() ; j++)
    {
      if (((*j)->getQualifie() == sTreeID) || ((*j)->getQualifiant() == sTreeID))
      	LocalGraph.getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;

      // if (strcmp((*j)->pDonnees->qualifie, sTreeID.c_str()) == 0)
      //   pLocalGraph->aLinks.push_back(new NSPatLinkInfo(*(*j))) ;
    }

    // Appel pilote : modification de l'objet en local
    NSBasicAttributeArray List ;
    List.push_back(new NSBasicAttribute("object", sTreeID)) ;
    List.push_back(new NSBasicAttribute(sAttr, sValue)) ;

    // rustine EpiPump
/*
    if (sOperatorID == "")
    	List.push_back(new NSBasicAttribute("operator", pContexte->getUtilisateurID())) ;
    else
*/
    	List.push_back(new NSBasicAttribute("operator", sOperatorID)) ;

    NSPersonsAttributesArray LocalObjectsList ;

    res = _pSuper->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_OBJECT.c_str(), &LocalGraph, &LocalObjectsList, &List) ;
    if (!res)
    {
      erreur("Echec service à la modification du graphe d'adresse.", standardError, 0) ;
      return false ;
    }
  }

	return true ;
}

bool
NSAddressGraphManager::trouveInfoLieuAdr(string& sNomEspace, string& sTele1, string& sTele2, string& sFax)
{
  // on considère ici qu'on a un graphe d'adresses chargé
  if ((getRootObject() == string("")) || (NULL == _pDataGraph))
  	return false ;

  sNomEspace = string("") ;
  sTele1     = string("") ;
  sTele2     = string("") ;
  sFax       = string("") ;

  NSDataTreeIter iterTree ;
  if (_pDataGraph->getTrees()->ExisteTree("LLIEU1", _pSuper, &iterTree))
  {
    NSPatPathoArray PatPathoArray(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;

    // on récupère le nom du site et le numéro dans la voie
    PatPathoIter iter = PatPathoArray.begin() ;
    int iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
    {
      string sElemLex = (*iter)->getLexique() ;
      string sSens    = string("") ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;

      // Chapitre "nom" / name chapter
      if (sSens == string("LNOMA"))
        sNomEspace = PatPathoArray.getFreeTextValue(&iter) ;

      // Chapitre "téléphone" / telephone chapter
      else if (sElemLex == string("LTELE1"))
        sTele1 = PatPathoArray.getFreeTextValue(&iter) ;

      // Chapitre "téléphone secondaire" / second telephone chapter
      else if (sElemLex == string("LTELE2"))
        sTele2 = PatPathoArray.getFreeTextValue(&iter) ;

      // Chapitre "fax" / fax chapter
      else if (sElemLex == string("LFAX01"))
        sFax = PatPathoArray.getFreeTextValue(&iter) ;

      else
      	iter++ ;
    }

    if ((sNomEspace != "") || (sTele1 != "") || (sTele2 != "") || (sFax != ""))
      return true ;
  }

  return false ;
}

bool
NSAddressGraphManager::trouveInfoSiteAdr(string& sNomSite, string& sNumVoie, string& sBat, string& sEsc, string& sBP, string& sLieuDit)
{
  // on considère ici qu'on a un graphe d'adresses chargé
  if ((getRootObject() == string("")) || (NULL == _pDataGraph))
  	return false ;

  sNomSite = string("") ;
  sNumVoie = string("") ;
  sBat     = string("") ;
  sEsc     = string("") ;
  sBP      = string("") ;
  sLieuDit = string("") ;

  NSDataTreeIter iterTree ;
  if (_pDataGraph->getTrees()->ExisteTree("LSITE1", _pSuper, &iterTree))
  {
    NSPatPathoArray PatPathoArray(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;

    // on récupère le nom du site et le numéro dans la voie
    PatPathoIter iter = PatPathoArray.begin() ;
    int iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
    {
      string sElemLex = (*iter)->getLexique() ;
      string sSens    = string("") ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;

      // Chapitre "nom" / name chapter
      if (sSens == string("LNOMA"))
        sNomSite = PatPathoArray.getFreeTextValue(&iter) ;

      else if (sSens == string("LVOIE"))
      {
        iter++ ;
        sElemLex = (*iter)->getLexique() ;
        string sSens1 = string("") ;
        NSDico::donneCodeSens(&sElemLex, &sSens1) ;

        if (sSens1 == string("LNUMA"))
          sNumVoie = PatPathoArray.getFreeTextValue(&iter) ;
        else
        	iter++ ;
      }
      // Batiment
      else if (sSens == string("LBATI"))
        sBat = PatPathoArray.getFreeTextValue(&iter) ;

      else if (sSens == string("LESCA"))
        sEsc = PatPathoArray.getFreeTextValue(&iter) ;

      else if (sSens == string("LBPOA"))
        sBP = PatPathoArray.getFreeTextValue(&iter) ;

      else if (sSens == string("LLIED"))
        sLieuDit = PatPathoArray.getFreeTextValue(&iter) ;

      else
      	iter++ ;
    }

    if ((sNomSite != "") || (sNumVoie != ""))
      return true ;
  }

  return false ;
}

bool
NSAddressGraphManager::trouveInfoVoieAdr(NSContexte *pContexte, string& sVoie, string sLang)
{
  // on considère ici qu'on a un graphe d'adresses chargé
  if ((getRootObject() == "") || (NULL == _pDataGraph))
  	return false ;

  sVoie = string("") ;

  NSDataTreeIter iterTree ;
  if (_pDataGraph->getTrees()->ExisteTree("LVOIE1", _pSuper, &iterTree))
  {
    string sTypeVoie = string("") ;
    string sNomVoie  = string("") ;

    NSPatPathoArray PatPathoArray(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;

    // on récupère le nom du site et le numéro dans la voie
    PatPathoIter iter = PatPathoArray.begin() ;
    int iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
    {
      string sElemLex = (*iter)->getLexique() ;
      string sSens    = string("") ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;

      // Chapitre "nom" / name chapter
      if (sSens == string("LTYPA"))
      {
        iter++ ;
        while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
        {
          // on cherche ici un texte libre
          sElemLex = (*iter)->getLexique() ;
          if (string(sElemLex, 0, 3) == string("£C;"))
          {
            iter++ ;
            while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 2))
            {
              // on cherche ici un code lexique pour un libelle
              string sCode = (*iter)->getLexique() ;
              pContexte->getDico()->donneLibelle(sLang, &sCode, &sTypeVoie) ;
              iter++ ;
            }
          }
          else
            iter++ ;
        }
      }
      else if (sSens == string("LNOMA"))
        sNomVoie = PatPathoArray.getFreeTextValue(&iter) ;
      else
      	iter++ ;
    }

    // if ((sTypeVoie != "") && (sNomVoie != ""))
    // on ne controle que sur le nom temporairement
    if (string("") != sNomVoie)
    {
      sVoie = sTypeVoie + string(" ") + sNomVoie ;
      return true ;
    }
  }

  return false ;
}

bool
NSAddressGraphManager::trouveInfoVilleAdr(string& sVille, string& sPays, string& sZip, string& sCedex)
{
  // on considère ici qu'on a un graphe d'adresses chargé
  if ((getRootObject() == "") || (NULL == _pDataGraph))
  	return false ;

  sVille = string("") ;
  sPays  = string("") ;
  sZip   = string("") ;
  sCedex = string("") ;

  NSDataTreeIter iterTree ;
  if (_pDataGraph->getTrees()->ExisteTree("LVILL1", _pSuper, &iterTree))
  {
    NSPatPathoArray PatPathoArray(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;

    // on récupère le nom du site et le numéro dans la voie
    PatPathoIter iter = PatPathoArray.begin() ;
    int iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
    {
      string sElemLex = (*iter)->getLexique() ;
      string sSens    = string("") ;
      NSDico::donneCodeSens(&sElemLex, &sSens) ;

      // Code Postal / Zip Code
      if (sSens == string("LZIP0"))
        sZip = PatPathoArray.getFreeTextValue(&iter) ;

      // Cedex
      else if (sSens == string("LCEDX"))
        sCedex = PatPathoArray.getFreeTextValue(&iter) ;

      // Chapitre "ville" / city chapter
      else if (sSens == string("LCOMU"))
        sVille = PatPathoArray.getFreeTextValue(&iter) ;

      // Pays / Country
      else if (sSens == string("LPAYS"))
        sPays = PatPathoArray.getFreeTextValue(&iter) ;

      else
      	iter++ ;
    }

    if ((sVille != "") || (sZip != ""))
      return true ;
  }

  return false ;
}

string
NSAddressGraphManager::trouveLibCourtAdr(NSContexte *pContexte, string sLang)
{
  string sVille, sPays, sZip, sCedex ;
  string sSite, sNum, sVoie, sBat, sEsc, sBP, sLD ;
  string sLibelle = "" ;

  if (trouveInfoSiteAdr(sSite, sNum, sBat, sEsc, sBP, sLD))
  {
    if (sSite != "")
    	sLibelle = sSite ;
  }

  if (trouveInfoVoieAdr(pContexte, sVoie, sLang))
  {
    if (sNum != "")
    	sLibelle += string(" ") + sNum + string(",") ;
    sLibelle += string(" ") + sVoie ;
  }

  if (trouveInfoVilleAdr(sVille, sPays, sZip, sCedex))
  {
    if (sVille != "")
    	sLibelle += string(" ") + sVille ;
  }
  return sLibelle;
}


string
NSAddressGraphManager::trouveLibLongAdr(NSContexte *pContexte, string sLang)
{
  string sEspace, sTele1, sTele2, sFax ;
  string sVille, sPays, sZip, sCedex ;
  string sSite, sNum, sVoie, sBat, sEsc, sBP, sLD ;
  string sLibelle = "" ;

  if (trouveInfoLieuAdr(sEspace, sTele1, sTele2, sFax))
  {
    if (sEspace != "")
    {
      sEspace[0] = pseumaj(sEspace[0]) ;
      sLibelle = sEspace ;
    }
  }

  if (trouveInfoSiteAdr(sSite, sNum, sBat, sEsc, sBP, sLD))
  {
    if (sLibelle != "")
    	sLibelle += string("\r\n") ;

    if (sSite != "")
    {
      sSite[0] = pseumaj(sSite[0]) ;
      sLibelle += sSite + string("\r\n") ;
    }

    if (sBat != "")
    {
      sLibelle += string("Bat ") + sBat ;
      if (sEsc != "")
      	sLibelle += string(" ") ;
      else
      	sLibelle += string("\r\n") ;
    }

    if (sEsc != "")
    {
      sLibelle += string("Esc ") + sEsc ;
      sLibelle += string("\r\n") ;
    }

    if (sBP != "")
    {
      sLibelle += string("BP. ") + sEsc ;
      sLibelle += string("\r\n") ;
    }

    if (sLD != "")
    {
      sLibelle += sLD ;
      sLibelle += string("\r\n") ;
    }
  }

  if (trouveInfoVoieAdr(pContexte, sVoie, sLang))
  {
    if (sNum != "")
      sLibelle += sNum + string(", ") ;

    sLibelle += sVoie ;
    sLibelle += string("\r\n") ;
  }

  if (trouveInfoVilleAdr(sVille, sPays, sZip, sCedex))
  {
    if (sZip != "")
      sLibelle += sZip;

    if (sVille != "")
    {
      if (sZip != "")
        sLibelle += string(" ") ;

      sVille[0] = pseumaj(sVille[0]) ;
      sLibelle += sVille ;

      if (sCedex != "")
      {
      	sLibelle += string(" Cedex ") + sCedex ;
      }
    }

    if (sPays != "")
    {
      if (sLang != "fr")
      {
          sPays[0] = pseumaj(sPays[0]) ;
          sLibelle += string("\r\n") + sPays ;
      }
    }
  }

  return sLibelle ;
}

string
NSAddressGraphManager::trouveTelAdr()
{
  string sEspace, sTele1, sTele2, sFax ;
  if (trouveInfoLieuAdr(sEspace, sTele1, sTele2, sFax))
  {
  	if (sTele1 != "")
    	return sTele1 ;
    else
    	return sTele2 ;
  }
  return "" ;
}

#endif // #ifndef _ENTERPRISE_DLL

// -----------------------------------------------------------------------------
// class NSTeamGraphManager
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool
NSTeamGraphManager::getTeamGraph(string sObjID, NSPatPathoArray *pPatPathoArray)
{
try
{
	// NSLinkManager *pGraphe = pLinkManager ;
	// bool bGetSuivant = true ;

  // remise à zéro du graphe
  _pDataGraph->graphReset() ;
  string sObject = sObjID ;
  setRootObject(sObjID) ;

	NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(OBJECT, sObjID)) ;
//  pAttrList->push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;

  bool res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), _pDataGraph, &AttrList) ;
  if (false == res)
  {
  	string 	sMessage = "" ;
  	if 			(_pSuper->getPilot()->getErrorMessage() != "")
    	sMessage = _pSuper->getPilot()->getErrorMessage() ;
    else if (_pSuper->getPilot()->getWarningMessage() != "")
    	sMessage = _pSuper->getPilot()->getWarningMessage() ;
    else
    	sMessage = "Echec du service de récupération du graphe représentant une Equipe." ;
    erreur(sMessage.c_str(), standardError) ;
    return false ;
  }

  // constitution de la patpatho contenant tous les arbres du graphe
  if (pPatPathoArray)
  {
    pPatPathoArray->vider() ;
    if (false == _pDataGraph->getTrees()->empty())
    {
      for (NSDataTreeIter i = _pDataGraph->getTrees()->begin() ; _pDataGraph->getTrees()->end() != i ; i++)
      {
      	NSPatPathoArray Ppt(_pSuper) ;
        (*i)->getRawPatPatho(&Ppt) ;
        pPatPathoArray->ajouteVecteur(&Ppt, ORIGINE_PATH_PATHO) ;
      }
    }
  }
  return true ;
}
catch (...)
{
  erreur("Exception NSTeamGraphManager::getTeamGraph", standardError) ;
  return false ;
}
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
bool
NSTeamGraphManager::setTeamGraph(NSPatPathoArray *pPPT, string sOperatorID, bool bLocalTeam)
{
	if ((NULL == pPPT) || (true == pPPT->empty()))
		return false ;

  // TODO FIXME FLP
  // il manque la modification de l'objet - FLP

  NSBasicAttributeArray	AttrList ;
	string								sNodeRoot = string("") ;
  string								sCodeObj  = string("LEQUM1") ;
  PatPathoIter					pptIter   = pPPT->begin() ;

  if ((pPPT->end() != pptIter) && ((*pptIter)->getLexique() == sCodeObj))
  {
  	// Penser à passer (*pptIter)->getDoc(), sinon l'arbre perd son numéro de document : gênant en cas d'évolution
    // (*pptIter)->getDoc() must be transmitted, elsewhere the tree's document ID gets lost : big problem when the team grows
		sNodeRoot	= setTree(pPPT, "", (*pptIter)->getDoc()) ;
  	setRootObject(sNodeRoot) ;
  }
  else
  {
  	erreur("L'arbre ne contient d'Equipe.", standardError) ;
    return false ;
  }

  // Le graphe est maintenant constitué. on doit appeler le pilote pour
  // trouver successivement les ID de tous les arbres du graphe
  // (et résoudre leurs dépendances)

  NSDataGraph								LocalGraph(_pSuper, graphObject) ;
  string										sTreeID				= "" ;
  string										sTreeRights		= "" ;
  NSPatPathoArray						PatPatho(_pSuper, graphObject) ;
  NSPersonsAttributesArray	ObjectsList ;
  NSDataTreeIter						iterTree ;

  bool bExist = _pDataGraph->getTrees()->ExisteTree(sCodeObj, _pSuper, &iterTree) ;
  if (bExist)
  {
  	sTreeID	= (*iterTree)->getTreeID() ;
    LocalGraph.graphReset() ;
    PatPatho.vider() ;
    _pDataGraph->getTree(sTreeID, &PatPatho, &sTreeRights) ;
    LocalGraph.setTree(&PatPatho, sTreeRights, sTreeID) ;

    string sNewGraphMask = "$#" ;
    bool res = false ;
    if (strncmp(sNewGraphMask.c_str(), sTreeID.c_str(), strlen(sNewGraphMask.c_str())) == 0)
    {
/*
      // rustine EpiPump
      if (sOperatorID == "")
        AttrList.push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;
      else
*/
        AttrList.push_back(new NSBasicAttribute("user", sOperatorID)) ;

      AttrList.push_back(new NSBasicAttribute(TEAM_LNOMA, getTeamName(&PatPatho))) ;

      if (bLocalTeam)
        AttrList.push_back(new NSBasicAttribute(LOCALTEAM, "1")) ;

      // Appel pilot : création de l'objet en local
      // calling pilot : create local object
      res	= _pSuper->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(), &LocalGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false) ;
    }
    else
    {
      AttrList.push_back(new NSBasicAttribute(OBJECT, sTreeID)) ;
/*
      if (sOperatorID == "")
        AttrList.push_back(new NSBasicAttribute(OPERATOR, pContexte->getUtilisateurID())) ;
      else
*/
        AttrList.push_back(new NSBasicAttribute(OPERATOR, sOperatorID)) ;
      res = _pSuper->getPilot()->modifyPersonOrObject(NautilusPilot::SERV_MODIFY_OBJECT.c_str(), &LocalGraph, &ObjectsList, &AttrList) ;
    }

    if (!res)
    {
      erreur("Echec du service pilot lors de la création/modification du graphe d'une Equipe.", standardError) ;
      return false ;
    }
  }

  // Remplacement du nouvel ID dans le DataGraph
  NSDataTreeIter iter = LocalGraph.getTrees()->begin() ;
  string sNewTreeID = (*iter)->getTreeID() ;
  NSPatPathoArray Ppt(_pSuper) ;
  (*iter)->getRawPatPatho(&Ppt) ;
  _pDataGraph->replaceTree(sTreeID, sNewTreeID, &Ppt, sTreeRights) ;

	return true ;
}

string
NSTeamGraphManager::getTeamName(NSPatPathoArray *pPPT)
{
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return string("") ;

	for (PatPathoIter pptIter = pPPT->begin() ; pPPT->end() != pptIter ; pptIter++)
  	if ((*pptIter)->getLexiqueSens() == string("LNOMA"))
      return pPPT->getFreeTextValue(&pptIter) ;

  return string("") ;
}

NSMoralPerson *
NSTeamGraphManager::createTeam(NSContexte *pCtx)
{
#ifndef _ENTERPRISE_DLL
	NSPatPathoArray PPT(_pSuper, graphObject) ;

  NSSmallBrother BBoss(pCtx, &PPT) ;
  BBoss.setMotherWindow(_pSuper->donneMainWindow()) ;

  // Ici on lance une boite de dialogue modale
#ifdef __OB1__
	BB1BBInterfaceForKs InterfaceForKs(-1, _pSuper->getTeamMemberArchetypeId(), "", false) ;
	BBoss.lanceBbkArchetypeInDialog(_pSuper->getTeamMemberArchetypeId(), 0, 0, &InterfaceForKs, true) ;
#else
	BBoss.lanceBbkArchetypeInDialog(_pSuper->getTeamMemberArchetypeId(), 0, 0, true) ;
#endif

  // on teste le code de retour du dialogue, qui est stocké dans le
  // BBItem root créé par le pBigBoss.
  if (0 == BBoss.getBBItem()->_iRetourDlg)     // CmOK
  {
    // on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
    // si elle n'est pas vide.
    if (false == BBoss.getPatPatho()->empty())
    {
      PPT = *(BBoss.getPatPatho()) ;
      NSMoralPerson *pMPTeam = new NSMoralPerson(pCtx, &PPT) ;
      return pMPTeam ;
    }
  }
#endif

  return (NSMoralPerson*) 0 ;
}

// -----------------------------------------------------------------------------
//
// Classe NSPersonGraphManager
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// default constructor
// -----------------------------------------------------------------------------
NSPersonGraphManager::NSPersonGraphManager(NSSuper* pSuper)
                     :NSSuperRoot(pSuper)
{
try
{
	// NSSuper *pLocalSuper = pContexte->getSuperviseur() ;
  // NSArcManager *pAManager = pLocalSuper->getArcManager() ;

	_bReadOnly    = true ;
  _bNeedUnlock  = false ;

  _pDataGraph   = new NSDataGraph(_pSuper, graphPerson) ;
  _pLinkManager = new NSLinkManager(_pSuper, _pDataGraph) ;
  _pAttrArray   = new NSBasicAttributeArray() ;

  _lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSPersonGraphManager ctor", standardError) ;
}
}

// -----------------------------------------------------------------------------
// copy constructor
// -----------------------------------------------------------------------------
NSPersonGraphManager::NSPersonGraphManager(const NSPersonGraphManager& rv)
                     :NSSuperRoot(rv._pSuper)
{
try
{
	_bReadOnly 		= rv._bReadOnly ;
  _bNeedUnlock 	= rv._bNeedUnlock ;

  _pDataGraph   = new NSDataGraph(*(rv._pDataGraph)) ;
  _pLinkManager	= new NSLinkManager(_pSuper, _pDataGraph) ;
  _pAttrArray   = new NSBasicAttributeArray(*(rv._pAttrArray)) ;

  _lObjectCount++ ;
}
catch (...)
{
	erreur("Exception NSPersonGraphManager copy ctor", standardError) ;
}
}

// -----------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------
NSPersonGraphManager::~NSPersonGraphManager()
{
  // string ps = "Entering NSPersonGraphManager destructor" ;
  // _pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  if (_pDataGraph)
  	delete _pDataGraph ;

  if (_pLinkManager)
  	delete _pLinkManager ;

  if (_pAttrArray)
  	delete _pAttrArray ;

  _lObjectCount-- ;
}


NSPersonGraphManager&
NSPersonGraphManager::operator=(const NSPersonGraphManager& src)
{
	if (this == &src)
  	return (*this) ;

	_bReadOnly   = src._bReadOnly ;
  _bNeedUnlock = src._bNeedUnlock ;

  if (_pDataGraph)
  	delete _pDataGraph ;
  _pDataGraph = new NSDataGraph(*(src._pDataGraph)) ;

  if (_pLinkManager)
  	delete _pLinkManager ;
  _pLinkManager = new NSLinkManager(_pSuper, _pDataGraph) ;

  if (_pAttrArray)
  	delete _pAttrArray ;
  _pAttrArray = new NSBasicAttributeArray(*(src._pAttrArray)) ;
/*
  *pDataGraph 	= *(src.pDataGraph) ;
  *pLinkManager = *(src.pLinkManager) ;
  *pAttrArray  	= *(src.pAttrArray) ;
*/
  return (*this) ;
}

void
NSPersonGraphManager::graphReset()
{
	// NSSuper *pLocalSuper = pContexte->getSuperviseur() ;
  _pDataGraph->graphReset() ;
  _pAttrArray->vider() ;
}

// -----------------------------------------------------------------------------
// Préparation et audit du graphe (tri des arbres par localisation et
// vérification de son homogénéité)
// --
// Preparation and audit of the graph (sort the trees and check graph
// homogeneity)
// -----------------------------------------------------------------------------
bool
NSPersonGraphManager::graphPrepare()
{
	if (NULL == _pDataGraph)
    return false ;

	if (_pDataGraph->getTrees()->empty())
    return true ;

	for (NSDataTreeIter i = _pDataGraph->getTrees()->begin() ; _pDataGraph->getTrees()->end() != i ; i++)
  	(*i)->sortPPTByLocalisation() ;

  return true ;
}

void
NSPersonGraphManager::setRootTree(string sTrID)
{
	_pDataGraph->setGraphID(sTrID) ;
	_pDataGraph->setLastTree() ;
}

string
NSPersonGraphManager::getRootTree()
{
	return (_pDataGraph->getGraphID()) ;
}

string
NSPersonGraphManager::getPersonID()
{
	if (getRootTree() == "")
		return string("") ;

	return string(_pDataGraph->getGraphID(), 0, PAT_NSS_LEN) ;
}

bool
NSPersonGraphManager::incrementeCode(string& sCompteur)
{
	return _pDataGraph->incrementeCode(sCompteur) ;
}

bool
NSPersonGraphManager::getTree(string sObjectID, NSPatPathoArray* pPatPatho, string* psDocRosace, bool bVerbose)
{
	return _pDataGraph->getTree(sObjectID, pPatPatho, psDocRosace, bVerbose) ;
}

string
NSPersonGraphManager::setTree(NSPatPathoArray* pPatPatho, string sDocRosace, string sCode)
{
	// NSSuper *pLocalSuper = pContexte->getSuperviseur() ;
  // NSArcManager *pArcManager = pLocalSuper->getArcManager() ;
  return _pDataGraph->setTree(pPatPatho, sDocRosace, sCode) ;
}

bool
NSPersonGraphManager::updateTrees(NSContexte *pContexte,
                                  NSVectPatPathoArray *	pVectTrees,
                                  string *							psNewDataTreeId,
                                  string *							psNewMetaTreeId,
                                  PIDSTYPE					    iTypePids,
                                  bool								  bInterface)
{
try
{
	if (((NSVectPatPathoArray*) NULL == pVectTrees) || (pVectTrees->empty()))
		return false ;

  if ((NSDataGraph*) NULL == _pDataGraph)
    return false ;

	string ps2 = "NSPersonGraphManager::updateTrees : begin" ;
  _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	NSDataGraph LocalGraph(_pSuper, graphPerson) ;

#ifndef _ENTERPRISE_DLL
	NSEnregTreeDocDlg* pEnregDlg = NULL ;
  bool bEnregIsCreated = false ;
#endif

	string sDocum = string(DOC_CODE_DOCUM_LEN, '0') ;
	sDocum[0] = '#' ;
	string sLocalRootTree = getPersonID() + sDocum ;
	LocalGraph.setLastTree(sLocalRootTree) ;
	NSDataTreeIter iterTree ;
	size_t lenTree = OBJECT_ID_LEN ;
	int nbTrees = pVectTrees->size() ;

	// on extrait ici les arbres (et leurs liens) du DataGraph et on les place dans le graphe local
	for (PatPathoIterVect i = pVectTrees->begin() ; pVectTrees->end() != i ; i++)
	{
		if ((*i) && (false == (*i)->empty()))
		{
			string sTreeID  = (*((*i)->begin()))->getDoc() ;
			string sCodeLex = (*((*i)->begin()))->getLexique() ;

			ps2 = string("NSPersonGraphManager::updateTrees : treeID = ") + sTreeID ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

#ifndef _ENTERPRISE_DLL
			if (bInterface && (string("ZDOCU1") != sCodeLex) && (string("LCTRI1") != sCodeLex))
			{
        pEnregDlg = new NSEnregTreeDocDlg(_pSuper->donneMainWindow(), pContexte) ;
				pEnregDlg->Create() ;
				pEnregDlg->Show(SW_SHOW) ;
				pEnregDlg->SetText(sCodeLex) ;
				bEnregIsCreated = true ;
			}
#endif

			string sTreeRosace = _pDataGraph->getRights()->find(sTreeID) ;

			// mise à jour des nodes textes libres dans la patpatho
			ps2 = string("NSPersonGraphManager::updateTrees : before updateNodesTL") ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

			_pDataGraph->updateNodesTL(*i, sTreeID) ;

      ps2 = string("NSPersonGraphManager::updateTrees : before aRights.RemoveDocument") ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

			if ((string("") != sTreeID) && (_pDataGraph->getTrees()->ExisteTreeID(sTreeID, &iterTree)))
			{
				_pDataGraph->getRights()->RemoveDocument(sTreeID) ;
				delete (*iterTree) ;
				_pDataGraph->getTrees()->erase(iterTree) ;
			}

      ps2 = string("NSPersonGraphManager::updateTrees : before setTree") ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

			// Ne pas faire setTree(*i), qui réattribue des treeID
			LocalGraph.setTree(*i, sTreeRosace, sTreeID) ;

      // on place ici les contributions
      if (string("LCTRI1") != sCodeLex)
        LocalGraph.signChanges(sTreeID, pContexte->getPatient()->getContribution()) ;

      ps2 = string("NSPersonGraphManager::updateTrees : before links moving") ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

      NSPatLinkArray* pLinks = _pDataGraph->getLinks() ;

      if (pLinks && (false == pLinks->empty()))
      {
        for (NSPatLinkIter j = pLinks->begin() ; pLinks->end() != j ; )
        {
          if ((*j) &&
            	((string((*j)->getQualifie(),   0, lenTree) == sTreeID) ||
               (string((*j)->getQualifiant(), 0, lenTree) == sTreeID)))
          {
            LocalGraph.getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;
            delete (*j) ;
            pLinks->erase(j) ;
          }
          else
            j++ ;
        }
      }

			ps2 = string("NSPersonGraphManager::updateTrees : before models moving") ;
			_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

      NSArcLinkArray* pArchs = _pDataGraph->getArchs() ;

      if (pArchs && (false == pArchs->empty()))
      {
        for (NSArcLinkIter k = pArchs->begin() ; pArchs->end() != k ; )
        {
          if ((*k) && ((*k)->getObject() == sTreeID))
          {
            // on ne fait pas ici le push_back car il est fait par le setTree
            // pLocalGraph->aArchs.push_back(new NSArcLink(*(*k)));
            delete (*k) ;
            pArchs->erase(k) ;
          }
          else
            k++ ;
        }
      }
    }
  }

	ps2 = "NSPersonGraphManager::updateTrees : before writeGraph" ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

  // on expédie le graphe au pilote
  bool res = writeGraph(pContexte, iTypePids, &LocalGraph) ;

  if (false == res)
    erreur("erreur d'écriture du graphe à travers le pilote.", standardError) ;

	ps2 = string("NSPersonGraphManager::updateTrees : before re-introducing trees") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

  // on "plug" maintenant le graphe local (updaté par le pilote) sur le DataGraph
  // (même si le pilote ne l'a pas enregistré, il faut le replacer dans le graphe)
  //
  NSDataTreeArray* pTrees = LocalGraph.getTrees() ;

  if (pTrees && (false == pTrees->empty()))
  {
    for (NSDataTreeIter i = pTrees->begin() ; i != pTrees->end() ; i++)
    {
      NSPatPathoArray Ppt(_pSuper) ;
      string sTreeID = (*i)->getTreeID() ;
      string sTreeRosace ;
      LocalGraph.getTree(sTreeID, &Ppt, &sTreeRosace) ;
      _pDataGraph->setTree(&Ppt, sTreeRosace, sTreeID) ;

      if (1 == nbTrees)
      {
        if (psNewDataTreeId)
        	*psNewDataTreeId = sTreeID ;
        else
        {
          erreur("L'identifiant de l'arbre à mettre à jour n'est pas déclaré dans ::updateTrees().", standardError) ;
          return false ;
        }
      }
    }
  }

	ps2 = string("NSPersonGraphManager::updateTrees : before re-introducing links") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

  NSPatLinkArray* pLinks = LocalGraph.getLinks() ;

  if (pLinks && (false == pLinks->empty()))
  {
    for (NSPatLinkIter j = pLinks->begin() ; pLinks->end() != j ; j++)
    {
      // s'il y a un arbre méta et un arbre data, on renvoie le code des datas.
      // (extrémité du lien NSRootLink::docData)
      if ((nbTrees > 1) && ((*j)->getLien() == _pLinkManager->donneString(NSRootLink::docData)))
      {
        if ((psNewMetaTreeId) && (psNewDataTreeId))
        {
          *psNewMetaTreeId = (*j)->getQualifie() ;
          *psNewDataTreeId = (*j)->getQualifiant() ;
        }
        else
        {
          erreur("L'identifiant de l'arbre à mettre à jour n'est pas déclaré dans ::updateTrees().", standardError) ;
          return false ;
        }
      }

      (*j)->setDirty(false) ;
      _pDataGraph->getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;
    }
  }

	ps2 = string("NSPersonGraphManager::updateTrees : before re-introducing models") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

  NSArcLinkArray* pArchs = LocalGraph.getArchs() ;

	if (pArchs && (false == pArchs->empty()))
  {
		for (NSArcLinkIter k = pArchs->begin() ; pArchs->end() != k ; k++)
			_pDataGraph->getArchs()->push_back(new NSArcLink(*(*k))) ;
  }

#ifndef _ENTERPRISE_DLL
	if (bEnregIsCreated)
  {
		pEnregDlg->Destroy() ;
	  delete pEnregDlg ;
  }
#endif

	ps2 = string("NSPersonGraphManager::updateTrees : done") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

  if (false == res)
    return false ;

  return true ;
}
catch (...)
{
  erreur("Exception NSPersonGraphManager::updateTrees", standardError) ;
  return false  ;
}
}

bool
NSPersonGraphManager::commitGraphTree(NSContexte *pContexte,
                                      string& sTreeID,
                                      PIDSTYPE iTypePids)
{
try
{
	if (strlen(sTreeID.c_str()) != (PAT_NSS_LEN + DOC_CODE_DOCUM_LEN))
	{
  	erreur("TreeID de longueur non conforme.", standardError) ;
    return false ;
	}

	string ps2 = "NSPersonGraphManager::commitGraphTree : begin" ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	NSDataGraph LocalGraph(_pSuper, graphPerson) ;

#ifndef _ENTERPRISE_DLL
	NSEnregTreeDocDlg* pEnregDlg = 0 ;
  bool bEnregIsCreated = false ;
#endif

	NSPatPathoArray PatPatho(_pSuper) ;

	string sDocum = string(DOC_CODE_DOCUM_LEN, '0') ;
	sDocum[0] = INMEMORY_CHAR ;
	string sLocalRootTree = getPersonID() + sDocum ;
	LocalGraph.setLastTree(sLocalRootTree) ;
	NSDataTreeIter iterTree ;
	size_t lenTree = OBJECT_ID_LEN ;

	// Ici on va sortir l'arbre à updater (sTreeID, qui en principe est en '#' au départ) du
	// graphe de base (pDataGraph) pour l'introduire dans un graphe local avec ses liens et
	// ses archétypes. Ce graphe local sera ensuite envoyé au pilote.
	if (_pDataGraph->getTrees()->ExisteTreeID(sTreeID, &iterTree))
	{
  	(*iterTree)->getRawPatPatho(&PatPatho) ;

    string sCodeLex = (*(PatPatho.begin()))->getLexique() ;

    ps2 = string("NSPersonGraphManager::commitGraphTree : treeID = ") + sTreeID ;
    _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

#ifndef _ENTERPRISE_DLL
    if ((string("ZDOCU1") != sCodeLex) && (string("LCTRI1") != sCodeLex))
    {
      pEnregDlg = new NSEnregTreeDocDlg(_pSuper->donneMainWindow(), pContexte) ;
    	pEnregDlg->Create() ;
      pEnregDlg->Show(SW_SHOW) ;
      pEnregDlg->SetText(sCodeLex) ;
      bEnregIsCreated = true ;
    }
#endif

    _pDataGraph->updateNodesTL(&PatPatho, sTreeID) ;

    string sTreeRosace = _pDataGraph->getRights()->find(sTreeID) ;

    ps2 = string("NSPersonGraphManager::commitGraphTree : before aRights.RemoveDocument") ;
    _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

    _pDataGraph->getRights()->RemoveDocument(sTreeID) ;
    delete (*iterTree) ;
    _pDataGraph->getTrees()->erase(iterTree) ;

    ps2 = string("NSPersonGraphManager::commitGraphTree : before setTree") ;
    _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

    // Ne pas faire setTree(pPatPatho), qui réattribue des treeID
    LocalGraph.setTree(&PatPatho, sTreeRosace, sTreeID) ;

    // on place ici les contributions
    if (string("LCTRI1") != sCodeLex)
    {
      string sContrib = string("") ;
      if (pContexte->getPatient() && (pContexte->getPatient()->getGraphPerson() == this))
        sContrib = pContexte->getPatient()->getContribution() ;
      else
        sContrib = _pDataGraph->getLastContribution() ;
    	LocalGraph.signChanges(sTreeID, sContrib) ;
    }

    ps2 = string("NSPersonGraphManager::commitGraphTree : before links moving") ;
    _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

    if (false == _pDataGraph->getLinks()->empty())
    {
    	for (NSPatLinkIter j = _pDataGraph->getLinks()->begin(); _pDataGraph->getLinks()->end() != j ; )
      {
      	if ((*j) &&
            ((string((*j)->getQualifie(), 0, lenTree) == sTreeID) ||
             (string((*j)->getQualifiant(), 0, lenTree) == sTreeID)))
        {
        	LocalGraph.getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;
          delete (*j) ;
          _pDataGraph->getLinks()->erase(j) ;
        }
        else
        	j++ ;
      }
    }

    ps2 = string("NSPersonGraphManager::commitGraphTree : before models moving") ;
    _pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

    if (false == _pDataGraph->getArchs()->empty())
    {
    	for (NSArcLinkIter k = _pDataGraph->getArchs()->begin(); _pDataGraph->getArchs()->end() != k ; )
      {
      	if ((*k) && ((*k)->getObject() == sTreeID))
        {
        	// on ne fait pas ici le push_back car il est fait par le setTree
          // pLocalGraph->aArchs.push_back(new NSArcLink(*(*k)));
          delete (*k) ;
          _pDataGraph->getArchs()->erase(k) ;
        }
        else
        	k++ ;
      }
    }
  }
  else
  {
    string sError = string("L'arbre d'identifiant ") + sTreeID + string(" est inexistant dans le graphe.") ;
    _pSuper->trace(&sError, 1, NSSuper::trError) ;
    erreur(sError.c_str(), standardError) ;
  }

	ps2 = "NSPersonGraphManager::commitGraphTree : before writeGraph" ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	// on expédie le graphe au pilote
	bool res = writeGraph(pContexte, iTypePids, &LocalGraph) ;

	if (false == res)
		erreur("Erreur d'écriture du graphe à travers le pilote.", standardError) ;

	ps2 = string("NSPersonGraphManager::commitGraphTree : before re-introducing trees") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	// on "plug" maintenant le graphe local (updaté par le pilote) sur le DataGraph
	// (même si le pilote ne l'a pas enregistré, il faut le replacer dans le graphe)
	if (false == LocalGraph.getTrees()->empty())
	{
  	// on fait ici cette boucle pour la forme, mais en principe pLocalGraph
    // ne contient qu'un seul arbre mis à jour par le writeGraph précédent
    for (NSDataTreeIter i = LocalGraph.getTrees()->begin(); LocalGraph.getTrees()->end() != i; i++)
    {
    	NSPatPathoArray Ppt(_pSuper) ;
      sTreeID = (*i)->getTreeID() ;
      string sTreeRosace ;
      LocalGraph.getTree(sTreeID, &Ppt, &sTreeRosace) ;
      _pDataGraph->setTree(&Ppt, sTreeRosace, sTreeID) ;
    }
  }

	ps2 = string("NSPersonGraphManager::commitGraphTree : before re-introducing links") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	if (false == LocalGraph.getLinks()->empty())
	{
  	for (NSPatLinkIter j = LocalGraph.getLinks()->begin(); LocalGraph.getLinks()->end() != j; j++)
    {
    	(*j)->setDirty(false) ;
      _pDataGraph->getLinks()->push_back(new NSPatLinkInfo(*(*j))) ;
    }
  }

	ps2 = string("NSPersonGraphManager::commitGraphTree : before re-introducing models") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	if (false == LocalGraph.getArchs()->empty())
		for (NSArcLinkIter k = LocalGraph.getArchs()->begin(); LocalGraph.getArchs()->end() != k; k++)
			_pDataGraph->getArchs()->push_back(new NSArcLink(*(*k))) ;

#ifndef _ENTERPRISE_DLL
	if (bEnregIsCreated)
  {
		pEnregDlg->Destroy() ;
	  delete pEnregDlg ;
  }
#endif

	ps2 = string("NSPersonGraphManager::commitGraphTree : done") ;
	_pSuper->trace(&ps2, 1, NSSuper::trDetails) ;

	if (false == res)
		return false ;

	return true ;
}
catch (...)
{
	erreur("Exception NSPersonGraphManager::commitGraphTree", standardError) ;
	return false ;
}
}

bool
NSPersonGraphManager::signChanges(NSContexte *pContexte, string sTreeID)
{
  if ((NULL == pContexte) || (NULL == pContexte->getPatient()))
    return false ;

	return _pDataGraph->signChanges(sTreeID, pContexte->getPatient()->getContribution()) ;
}

bool
NSPersonGraphManager::getGraph()
{
	if ((getRootTree() == "") || (NULL == _pDataGraph))
  	return false ;

	// Prise de l'arbre de base
  // appel de readGraph
  //return pContexte->pPilot->readGraph(pDataGraph);
  return false ;
}

bool
NSPersonGraphManager::setGraph()
{
/* ******************* Obsolete ***************
#ifdef N_TIERS
    return pContexte->pPilot->writeGraph(pDataGraph);
#else
*/
	return true ;
}

#define ALLOW_HETEROGENEOUS_DOUBLONS

bool
NSPersonGraphManager::writeGraph(NSContexte *pContexte, PIDSTYPE iTypePids, NSDataGraph* pGraph)
{
	const char* serviceName ;
	bool bCreatePerson = false ;

	if ((NSDataGraph*) NULL == pGraph)
		bCreatePerson = true ;

	if (pidsPatient == iTypePids)
	{
  	if (bCreatePerson)
    	serviceName = (NautilusPilot::SERV_CREATE_PATIENT).c_str() ;
    else
    	serviceName = (NautilusPilot::SERV_MODIFY_PERSON).c_str() ;
	}
	else if (pidsCorresp == iTypePids)
	{
		if (bCreatePerson)
    	serviceName = (NautilusPilot::SERV_CREATE_CORESPONDENT).c_str() ;
    else
    	serviceName = (NautilusPilot::SERV_MODIFY_PERSON).c_str() ;
	}
	else if (pidsUtilisat == iTypePids)
	{
		if (bCreatePerson)
    	serviceName = (NautilusPilot::SERV_CREATE_USER).c_str() ;
    else
    	serviceName = (NautilusPilot::SERV_MODIFY_USER).c_str() ;
	}
	else if (pidsPatientGroup == iTypePids)
	{
		if (bCreatePerson)
    	serviceName = (NautilusPilot::SERV_CREATE_GROUP_PATIENT).c_str() ;
    else
    	serviceName = (NautilusPilot::SERV_MODIFY_GROUP_PATIENT).c_str() ;
	}

	string sPids = _pAttrArray->getAttributeValue(PERSON) ;
	string sUser = _pAttrArray->getAttributeValue(OPERATOR) ;

	if (bCreatePerson)    // cas de la création (on passe alors le graphe global)
	{
  	// la liste de personnes en cas de doublons
    //
    NSPersonsAttributesArray doublonsList ;
    //la liste des attributes du pids est pAttrArray (variable de classe)

    bool res = _pSuper->getPilot()->createPersonOrObject(serviceName, _pDataGraph, &doublonsList, _pAttrArray, 0, false) ;

    size_t iListSize = 0 ;
    if (false == doublonsList.empty())
    	iListSize = doublonsList.size() ;

		// For patients creation, we have to check if doublonsList really contains
    // patients (=doublons=creation failed) or just the new patient's status
    //
    if ((iTypePids == pidsPatient) && (iListSize > 0))
    {
    	NSPersonsAttributeIter i = doublonsList.begin() ;
      string sRole = (*i)->getAttributeValue(ROLE) ;
      if (sRole == string(""))
      	iListSize = 0 ;
    }

    if ((false == res) && (iListSize > 0))		// pList not empty : cas des doublons
    {
    	string sErrorText = "echec service" ;
      if (iTypePids == pidsPatient)
      	sErrorText = _pSuper->getText("NTIERS", "createPatientFailed") ;
      else if (iTypePids == pidsUtilisat)
      	sErrorText = _pSuper->getText("NTIERS", "createUserFailed") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), warningError) ;
    }
    else if (iListSize > 0)
    {
    	string sRole ;
      if (iTypePids == pidsPatient)
      	sRole = PATIENT_ROLE ;
      else if (iTypePids == pidsUtilisat)
      	sRole = USER_ROLE ;
      else if (iTypePids == pidsCorresp)
      	sRole = CORRESP_ROLE ;

#ifndef _ENTERPRISE_DLL

#ifndef ALLOW_HETEROGENEOUS_DOUBLONS

      NSTDoublonListDialog* pDoublonDlg = new NSTDoublonListDialog(_pSuper->GetMainWindow(), pContexte,
                                                                            &doublonsList, sRole) ;
			if (pDoublonDlg->Execute() == IDOK)
      {
      	if (pDoublonDlg->bCreer)
        {
        	// Appel du service pour forcer la creation
          bool bForceRes = _pSuper->getPilot()->createPersonOrObject(serviceName, _pDataGraph, &doublonsList, _pAttrArray, 0, true) ;
          if (false == bForceRes)
          {
          	delete pDoublonDlg ;
          	return false ;
          }
        }
        else
        {
        	mergeGraph(pDoublonDlg->sPersonSelect, sRole, pDoublonDlg->sRolePersonSelect) ;
        }
        delete pDoublonDlg ;
      }
			else
      {
      	delete pDoublonDlg ;
        return false ;
      }
#else

      // First step, check if the list contains somebody with same role
      //
      bool bSameRoleExist = false ;

      for (NSPersonsAttributeIter i = doublonsList.begin() ; doublonsList.end() != i ; i++)
        if ((*i)->getAttributeValue(ROLE) == sRole)
          bSameRoleExist = true ;

      // Nobody with already there for this role: create a new person
      // (ok this is a doublon but it is a preliminary strategy)
      //
      if (false == bSameRoleExist)
      {
        bool bForceRes = _pSuper->getPilot()->createPersonOrObject(serviceName, _pDataGraph, &doublonsList, _pAttrArray, 0, true) ;
        return bForceRes ;
      }

      // Somebody exists with the same role, ask the user if she wants to
      // create a doublon
      //
      NSTSingleRoleDoublonListDialog* pDoublonDlg = new NSTSingleRoleDoublonListDialog(_pSuper->donneMainWindow(), pContexte,
                                                                            &doublonsList, sRole) ;
			if (pDoublonDlg->Execute() == IDOK)
      {
      	if (pDoublonDlg->bCreer)
        {
        	// Appel du service pour forcer la creation
          bool bForceRes = _pSuper->getPilot()->createPersonOrObject(serviceName, _pDataGraph, &doublonsList, _pAttrArray, 0, true) ;
          if (false == bForceRes)
          {
          	delete pDoublonDlg ;
          	return false ;
          }
        }
        delete pDoublonDlg ;
      }
			else
      {
      	delete pDoublonDlg ;
        return false ;
      }

#endif

#endif
    }
  }
  else
  {
		// modification des données du pids
    // pour l'instant, on repasse tous les traits à chaque fois
		//        changeInfoPids();

    //la liste comprend ici la personne dont on modifie le graphe avec ses attributs de pids
    NSPersonsAttributesArray List ;
    //la liste des attributes du pids est pAttrArray (variable de classe)

    if (false == _pSuper->getPilot()->modifyPersonOrObject(serviceName, pGraph, &List, _pAttrArray))
    {
    	string sErrorText = "echec service" ;
      if      (pidsPatient == iTypePids)
      	sErrorText = _pSuper->getText("NTIERS", "modifyPatientFailed") ;
      else if (pidsUtilisat == iTypePids)
      	sErrorText = _pSuper->getText("NTIERS", "modifyUserFailed") ;
			_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), warningError) ;
    }
	}
	return true ;
}

bool
NSPersonGraphManager::mergeGraph(NSContexte *pContexte, string sPersonID, string sNewRole, string sOldRole)
{
	NSPatPathoArray OtherPPTAdmin(_pSuper, graphPerson) ;
	NSPatPathoArray OtherPPTPDS(_pSuper, graphPerson) ;
	NSPersonGraphManager OtherGraph(_pSuper) ;
	if (false == OtherGraph.getGraphAdmin(sPersonID, true))
  	return false ;

  string sUserID = pContexte->getUtilisateurID() ;

	OtherGraph._pAttrArray->push_back(new NSBasicAttribute(PERSON, sPersonID)) ;
	OtherGraph._pAttrArray->push_back(new NSBasicAttribute(OPERATOR, sUserID)) ;

	NSPatPathoArray PPTAdmin(_pSuper, graphPerson) ;
	NSPatPathoArray PPTPDS(_pSuper, graphPerson) ;
	NSDataTreeIter iterTree ;

	if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
	{
  	(*iterTree)->getRawPatPatho(&PPTAdmin) ;

    if (OtherGraph._pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&OtherPPTAdmin) ;

      if (false == mergeTrees(pContexte, &PPTAdmin, &OtherPPTAdmin))
      	return false ;
    }
    else
    	OtherPPTAdmin = PPTAdmin ;
  }
  else
  {
  	char msg[255] ;
    sprintf(msg, "mergeGraph: Person=[%s] : Missing [ZADMI1] in graph.", getPersonID().c_str()) ;
    string sMsg = string(msg) ;
    _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;
    erreur(msg, standardError) ;
    return false ;
  }

	if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
	{
  	(*iterTree)->getRawPatPatho(&PPTPDS) ;

    if (OtherGraph._pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&OtherPPTPDS) ;

      if (false == mergeTrees(pContexte, &PPTPDS, &OtherPPTPDS))
      	return false ;
    }
    else
    	OtherPPTPDS = PPTPDS ;
  }

	if (OtherGraph._pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
	{
  	string sCodeDocAdmin = (*iterTree)->getTreeID() ;
    OtherGraph.setTree(&OtherPPTAdmin, "", sCodeDocAdmin) ;
  }
  else
  	OtherGraph.createPPTAdmin(&OtherPPTAdmin, sUserID) ;

	if (OtherGraph._pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
  {
  	string sCodeDocPDS = (*iterTree)->getTreeID() ;
    OtherGraph.setTree(&OtherPPTPDS, "", sCodeDocPDS) ;
  }
  else
  	OtherGraph.createPPTPDS(&OtherPPTPDS, sUserID) ;

	//la liste des attributes du pids est pAttrArray (variable de classe)
  _pAttrArray->push_back(new NSBasicAttribute(ROLE, sNewRole)) ;
  _pAttrArray->changeAttributeValue(PERSON, sPersonID) ;
  // on doit retirer la balise PIDS si elle existe avant de l'envoyer au serveur
  if (_pAttrArray->getAttributeValue(PIDS) != string(""))
  	_pAttrArray->changeAttributeValue(PIDS, _pAttrArray->getAttributeValue(PIDS));

  // modification des attributs du PIDS
  if (false == _pSuper->getPilot()->modifyPIDSData((NautilusPilot::SERV_MODIFY_PERSON_ROLE).c_str(), _pAttrArray))
  {
  	string sErrorText = "echec service" ;
    sErrorText = _pSuper->getText("NTIERS", "modifyPersonFailed") ;
    erreur(sErrorText.c_str(), warningError) ;
    return false ;
	}

	// Rustine temporaire en attendant l'unification des pids
	PIDSTYPE iTypePids ;

	if (sOldRole.find(USER_ROLE) != NPOS)
  	iTypePids = pidsUtilisat ;
  else if (sOldRole.find(PATIENT_ROLE) != NPOS)
  	iTypePids = pidsPatient ;
  else if (sOldRole.find(CORRESP_ROLE) != NPOS)
  	iTypePids = pidsCorresp ;    // ATTENTION : non implémenté en modification pour l'instant dans setGraphAdmin

	// passer ici le type de pids dans setGraphAdmin en fonction de l'ancien role.
	OtherPPTAdmin.InserePatPatho(OtherPPTAdmin.end(), &OtherPPTPDS, 0) ;
	OtherGraph.setGraphAdmin(pContexte, &OtherPPTAdmin, iTypePids) ;

	return true ;
}

bool
NSPersonGraphManager::mergeTrees(NSContexte *pContexte, NSPatPathoArray* pSource, NSPatPathoArray* pDest)
{
	if ((NULL == pSource) || pSource->empty() || (NULL == pDest))
		return false ;

	PatPathoIter iterSuiv ;
	string msg ;

	string sLang = string("") ;
	if (pContexte)  	sLang = pContexte->getUserLanguage() ;

	for (PatPathoIter i = pSource->begin() ; pSource->end() != i ; i++)
	{
  	iterSuiv = i ;
    iterSuiv++ ;

    if ((pSource->end() == iterSuiv) || ((*iterSuiv)->getColonne() <= (*i)->getColonne()))
    {
    	// on est sûr ici que iterElement est une feuille
      string sCheminLex = pSource->donneCheminItem(i) ;
      string sCheminFils = sCheminLex ;
      if ((*i)->getUnit() != "")
      	sCheminFils += string(1, cheminSeparationMARK) + (*i)->getUnit() ;
      sCheminFils += string(1, cheminSeparationMARK) + (*i)->getLexique() ;

      PatPathoIter iter ;
      string sElemLex = (*i)->getLexique() ;
      if (string(sElemLex, 0, 3) == "£CL")
      {
      	if (pDest->CheminDansPatpatho(sCheminFils, "/", &iter))
        {
        	string sTLSource = (*i)->getTexteLibre() ;
          string sTLDest = (*iter)->getTexteLibre() ;

          if (sTLSource != sTLDest)
          {
          	msg = string("Voulez-vous remplacer ") +  sTLDest + string(" par ") + sTLSource + string(" ?") ;
            int retVal = ::MessageBox(0, msg.c_str(), "Message Episodus", MB_YESNO) ;

            if (retVal == IDYES)
            	(*iter)->setTexteLibre(sTLSource) ;
          }
        }
        else
        {
        	Message Msg ;
          Msg.SetLexique((*i)->getLexique()) ;
          Msg.SetTexteLibre((*i)->getTexteLibre()) ;
          pDest->AjouterChemin(sCheminLex, &Msg) ;
        }
      }
      else // si ce n'est pas un texte libre
      {
      	if (pDest->CheminDansPatpatho(sCheminLex, "/", &iter))
        {
        	NSPatPathoArray ElemSource(_pSuper, graphPerson) ;
          NSPatPathoArray ElemDest(_pSuper, graphPerson) ;
          ElemSource.ajoutePatho(i, 0, 0) ;
          // on suppose que la feuille trouvée dans pDest est unique
          iter++ ;
          ElemDest.ajoutePatho(iter, 0, 0) ;

          GlobalDkd Dcode(pContexte, sLang, sCheminLex, &ElemSource) ;
          Dcode.decodeNoeud() ;
          string sLibSource = Dcode.getDcodeur() ;

          GlobalDkd Dcode2(pContexte, sLang, sCheminLex, &ElemDest) ;
          Dcode2.decodeNoeud() ;
          string sLibDest = Dcode2.getDcodeur() ;

          if (sLibSource != sLibDest)
          {
          	string sLibChem ;
            pContexte->getDico()->donneLibelleChemin(sLang, sCheminLex, &sLibChem, "/") ;

            msg = string("Dans ") + sLibChem + string(", voulez-vous remplacer ") +
                                sLibDest + string(" par ") + sLibSource + string(" ?");
            int retVal = ::MessageBox(0, msg.c_str(), "Message Episodus", MB_YESNO) ;

            if (retVal == IDYES)
            	*iter = *i ;
          }
        }
        else
        	pDest->AjouterChemin(sCheminLex, (*i)->getLexique()) ;
      }
    }
  }

	return true ;
}

string
NSPersonGraphManager::getDataIdFromMetaId(string sMetaId)
{
  if (string("") == sMetaId)
    return string("") ;

  VecteurString VecteurData ;

  _pLinkManager->TousLesVrais(sMetaId, NSRootLink::docData, &VecteurData) ;
  if (VecteurData.empty())
    return string("") ;

  return *(*(VecteurData.begin())) ;
}

string
NSPersonGraphManager::getMetaIdFromDataId(string sDataId)
{
  if (string("") == sDataId)
    return string("") ;

  VecteurString VecteurData ;

  _pLinkManager->TousLesVrais(sDataId, NSRootLink::docData, &VecteurData, "ENVERS") ;
  if (VecteurData.empty())
    return string("") ;

  return *(*(VecteurData.begin())) ;
}

bool
NSPersonGraphManager::getGraphAdmin(string sID, bool bVerbose, PIDSTYPE iTypePids, NSPatPathoArray* pPatPathoArray)
{
  // Note : En N_TIERS le paramètre sID contient le nss de la personne et non le TreeRoot
  // on appelle ici un service générique qui retrouve le sous-graphe administratif (ZADMI, DPROS)
  // pour une personne quelconque à partir de son nss.
  const char* serviceName ;

  if ((pidsPatient == iTypePids) || (pidsCorresp == iTypePids) || (pidsUtilisat == iTypePids))
  	serviceName = (NautilusPilot::SERV_READ_GRAPH_ADMIN).c_str() ;
  else
  {
  	erreur("Type pids incorrect dans getGraphAdmin().", standardError) ;
    return false ;
  }

  if (strlen(sID.c_str()) != PAT_NSS_LEN)
  {
  	erreur("Identifiant de personne incorrect dans getGraphAdmin().", standardError) ;
    return false ;
  }

  string ps = string("Calling Pilot for readGraphAdmin") ;
  _pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  // On passe ici l'identifiant de la personne et les deux liens à remonter
  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(PERSON, sID)) ;
  AttrList.push_back(new NSBasicAttribute(LINK, _pLinkManager->donneString(NSRootLink::personAdminData))) ;
  AttrList.push_back(new NSBasicAttribute(LINK, _pLinkManager->donneString(NSRootLink::personHealthProData))) ;

  bool res = _pSuper->getPilot()->readGraphAdmin(serviceName, _pDataGraph, &AttrList) ;
  if (false == res)
  {
  	string sErrorText = _pSuper->getText("NTIERS", "personNotFound") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  ps = string("Call for readGraphAdmin succeeded") ;
  _pSuper->trace(&ps, 1, NSSuper::trDetails) ;

  graphPrepare() ;

  string sRoot = getRootTree() ;
  // on met à jour le compteur d'arbres à créer
  _pDataGraph->setLastTree() ;

  // constitution de la patpatho contenant tous les arbres du graphe
  if (pPatPathoArray)
  {
  	NSDataTreeIter iterTree ;
    pPatPathoArray->vider() ;

    if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    	(*iterTree)->getRawPatPatho(pPatPathoArray) ;
    else
    {
    	char msg[255] ;
      sprintf(msg, "getGraphAdmin: Person=[%s] : Missing [ZADMI1] in graph.", sID.c_str()) ;
      string sMsg = string(msg) ;
      _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;
      if (bVerbose)
      	erreur(msg, standardError) ;
      return false;
    }

    if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    {
    	NSPatPathoArray PatPathoPDS(_pSuper) ;
      (*iterTree)->getRawPatPatho(&PatPathoPDS) ;
      pPatPathoArray->InserePatPatho(pPatPathoArray->end(), &PatPathoPDS, 0, true) ;
    }
    else if (pidsPatient != iTypePids)
    {
    	char msg[255] ;
      sprintf(msg, "Person=[%s] : Missing [DPROS1] in graph.", sID.c_str()) ;
      string sMsg = string(msg) ;
      _pSuper->trace(&sMsg, 1, NSSuper::trWarning) ;
      // Ne pas signaler d'erreur car la personne ici peut être un patient
      // ou un utilisateur sans DPROS
      // if (bVerbose)
      //     erreur(msg, standardError, 0) ;
      // return false;
    }
	}

	return true ;
}

bool
NSPersonGraphManager::setGraphAdmin(NSContexte *pContexte, NSPatPathoArray* pPatPathoArray, PIDSTYPE iTypePids)
{
	if (NULL == pPatPathoArray)
		return false ;

	NSPatPathoArray PatPathoAdmin(_pSuper, graphPerson) ;
	NSPatPathoArray PatPathoPDS(_pSuper, graphPerson) ;

  partDualPatho(pPatPathoArray, &PatPathoAdmin, &PatPathoPDS) ;

  string sUserID = pContexte->getUtilisateurID() ;

	// création/mise à jour du graphe
	// A revoir...
	string sNewPids = string(1, INMEMORY_CHAR) + string(PAT_NSS_LEN - 1, '0') ;
	if (getAttributeValue(PERSON) == sNewPids)
	{
  	if (false == createGraphAdmin(sUserID, &PatPathoAdmin, &PatPathoPDS, iTypePids))
      return false ;

    writeGraph(pContexte, iTypePids, 0) ;
	}
	else                         //Modification
	{
  	if (false == modifyGraphAdmin(sUserID, &PatPathoAdmin, &PatPathoPDS, iTypePids))
      return false ;

    string sCodeDocAdmin, sCodeDocPDS ;
    NSDataTreeIter iterTree ;

    if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    {
    	sCodeDocAdmin = (*iterTree)->getTreeID() ;
      commitGraphTree(pContexte, sCodeDocAdmin, iTypePids) ;
    }

    if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    {
      sCodeDocPDS = (*iterTree)->getTreeID() ;

      // Is it a "in-memory" document... so we need to save its Meta too
      //
      string sDocID = (*iterTree)->getDocum() ;
      if (INMEMORY_CHAR == sDocID[0])
      {
        // Get Meta ID
        //
        NSPatLinkArray LinkDataArray ;
        _pLinkManager->TousLesVrais(sCodeDocPDS, NSRootLink::docData, &LinkDataArray, "ENVERS") ;
        if (false == LinkDataArray.empty())
        {
          NSPatLinkIter iterLinkData = LinkDataArray.begin() ;
    	    string sCodeDocMeta = (*iterLinkData)->getQualifie() ;
          if (INMEMORY_CHAR == sCodeDocMeta[PIDS_NSS_LEN])
            commitGraphTree(pContexte, sCodeDocMeta, iTypePids) ;
        }
      }

      commitGraphTree(pContexte, sCodeDocPDS, iTypePids) ;
    }
  }

  // exportXML() ;

  return true ;
}

bool
NSPersonGraphManager::createPPTAdmin(NSPatPathoArray* pPPTAdmin, string sUserID)
{
  if ((NULL == pPPTAdmin) || pPPTAdmin->empty())
    return false ;

	string sRootMeta = getRootTree() ;

  if (strlen(sRootMeta.c_str()) != OBJECT_ID_LEN)
  {
  	erreur("Impossible de créer la fiche administrative pour cette personne.", standardError) ;
    return false ;
  }

  string sAdminMeta = _pDataGraph->getNextTreeID() ;
  NSPatPathoArray PptMeta(_pSuper) ;
  ChargePatPathoMeta(&PptMeta, sUserID, "CQ030", "Fiche administrative", sAdminMeta, "ZADMI1", false) ;
  setTree(&PptMeta, "", sAdminMeta) ;

  string sAdminData = _pDataGraph->getNextTreeID() ;
  setTree(pPPTAdmin, "", sAdminData) ;

  // lien personAdminData du meta root au meta admin
  if (false == _pLinkManager->etablirLien(sRootMeta, NSRootLink::personAdminData, sAdminMeta))
  {
  	erreur("Impossible de créer le lien [personAdminData] pour cette personne", standardError) ;
    return false ;
  }

  // lien docData du meta admin au data admin
  if (false == _pLinkManager->etablirLien(sAdminMeta, NSRootLink::docData, sAdminData))
  {
  	erreur("Impossible de créer le lien des données administratives pour cette personne", standardError) ;
    return false ;
  }
  
  return true ;
}

bool
NSPersonGraphManager::createPPTPDS(NSPatPathoArray* pPPTPDS, string sUserID)
{
  if ((NULL == pPPTPDS) || pPPTPDS->empty())
    return false ;

	string sRootMeta = getRootTree();

  if (strlen(sRootMeta.c_str()) != OBJECT_ID_LEN)
  {
  	erreur("Impossible de créer la fiche professionnel de santé pour cette personne.", standardError) ;
    return false ;
  }

  string sPDSMeta = _pDataGraph->getNextTreeID() ;
  NSPatPathoArray PptMeta(_pSuper) ;
  ChargePatPathoMeta(&PptMeta, sUserID, "CS030", "Fiche professionnel de santé", sPDSMeta, "DPROS1", false) ;
  setTree(&PptMeta, "", sPDSMeta) ;

  string sPDSData = _pDataGraph->getNextTreeID() ;
  setTree(pPPTPDS, "", sPDSData) ;

  // lien personHealthProData du meta root au meta PDS
  if (false == _pLinkManager->etablirLien(sRootMeta, NSRootLink::personHealthProData, sPDSMeta))
  {
  	erreur("Impossible de créer le lien [personHealthProData] pour cette personne", standardError) ;
    return false ;
  }

  // lien docData du meta PDS au data PDS
  if (false == _pLinkManager->etablirLien(sPDSMeta, NSRootLink::docData, sPDSData))
  {
  	erreur("Impossible de créer le lien des données PDS pour cette personne", standardError) ;
    return false ;
  }

  return true ;
}

bool
NSPersonGraphManager::createGraphAdmin(string sUserID, NSPatPathoArray* pPPTAdmin, NSPatPathoArray* pPPTPDS, PIDSTYPE iTypePids)
{
	_pDataGraph->graphReset() ;

  if (!validAttributes(iTypePids))
  {
  	erreur("Personne non identifiée : impossible de créer un graphe pour cette personne", standardError) ;
    return false ;
  }

  string sPids = getAttributeValue(PERSON) ;

  string sRootMeta = sPids + INMEMORY_CHAR + string("00000") ;
  NSPatPathoArray PptMeta(_pSuper) ;
  ChargePatPathoMeta(&PptMeta, sUserID, "CS030", "Noeud racine patient", sRootMeta, "HHUMA3", false) ;
  setTree(&PptMeta, "", sRootMeta) ;

  setRootTree(sRootMeta);

  string sRootData = _pDataGraph->getNextTreeID() ;
  NSPatPathoArray PptData(_pSuper) ;
  PptData.ajoutePatho("HHUMA3", 0) ;
  setTree(&PptData, "", sRootData) ;

  // lien docData du meta root au data root
  if (false == _pLinkManager->etablirLien(sRootMeta, NSRootLink::docData, sRootData))
  {
  	erreur("Impossible de créer le RootGraph pour cette personne", standardError) ;
    return false ;
  }

  if (pPPTAdmin && (false == pPPTAdmin->empty()))
  	createPPTAdmin(pPPTAdmin, sUserID) ;

  if (pPPTPDS && (false == pPPTPDS->empty()))
  	createPPTPDS(pPPTPDS, sUserID) ;

  return true ;
}

bool
NSPersonGraphManager::modifyGraphAdmin(string sUserID, NSPatPathoArray* pPPTAdmin, NSPatPathoArray* pPPTPDS, PIDSTYPE /* iTypePids */)
{
	// Le graphe est supposé ici avoir un graphID
  string sPids = getPersonID();
  if (string("") == sPids)
  {
  	erreur("Identifiant personne introuvable. Impossible de modifier le graphe.", standardError);
    return false ;
  }

  NSDataTreeIter iterTree ;

  if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
  {
  	string sCodeDocAdmin = (*iterTree)->getTreeID() ;
    _pDataGraph->updateNodesTL(pPPTAdmin, sCodeDocAdmin) ;
    setTree(pPPTAdmin, "", sCodeDocAdmin) ;
    // (*iterTree)->setPatPatho(pPatPathoAdmin);
  }
  else if (pPPTAdmin)
  {
  	char msg[255] ;
    strcpy(msg, "Impossible de mettre à jour l'arbre [ZADMI1] dans le graphe administratif.") ;
    erreur(msg, standardError) ;
  }

  if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
  {
  	string sCodeDocPDS = (*iterTree)->getTreeID() ;
    _pDataGraph->updateNodesTL(pPPTPDS, sCodeDocPDS) ;
    setTree(pPPTPDS, "", sCodeDocPDS) ;
    // (*iterTree)->setPatPatho(pPatPathoPDS);
  }
  else if (pPPTPDS && (false == pPPTPDS->empty()))
  	createPPTPDS(pPPTPDS, sUserID) ;

	return true ;
}

bool
NSPersonGraphManager::GetTreesWithLink(string sCodeDocOrigine, NSRootLink::NODELINKTYPES typeLink, bool& bNoLink)
{
	NSLinkManager* pGraphe = _pLinkManager ;
  if (NULL == pGraphe)
    return false ;

	// chargement du document : on remonte le lien spécifié
  //
	NSPatLinkArray LinkArray ;
	pGraphe->TousLesVrais(sCodeDocOrigine, typeLink, &LinkArray) ;

	// cas où il n'existe pas de méta
	if (LinkArray.empty())
	{
  	bNoLink = true ;
    return true ;
	}

  bNoLink = false ;

	for (NSPatLinkIter iterLink = LinkArray.begin() ; LinkArray.end() != iterLink ; iterLink++)
	{
  	// on charge d'abord le document méta
    string sCodeDocMeta = (*iterLink)->getQualifiant() ;
    NSDataTree DataTree(_pSuper, sCodeDocMeta, graphPerson) ;
    if (false == DataTree.readParadoxTree())
    {
    	char msg[255] ;
      sprintf(msg, "Impossible de charger l'arbre [%s] dans le graphe de la personne.", sCodeDocMeta.c_str()) ;
      erreur(msg, standardError) ;
      return false ;
    }

    _pDataGraph->getTrees()->push_back(new NSDataTree(DataTree)) ;

    // chargement des datas du document
    NSPatLinkArray LinkDataArray ;
    pGraphe->TousLesVrais(sCodeDocMeta, NSRootLink::docData, &LinkDataArray) ;

    // lien amorce des datas
    // si un data n'existe pas, on se contente de ne pas le charger
    if (LinkDataArray.empty())
    	continue ;

    for (NSPatLinkIter iterLinkData = LinkDataArray.begin(); LinkDataArray.end() != iterLinkData ; iterLinkData++)
    {
    	string sCodeDocData = (*iterLinkData)->getQualifiant() ;
      NSDataTree DTree(_pSuper, sCodeDocData, graphPerson) ;
      if (false == DTree.readParadoxTree())
      {
      	char msg[255] ;
        sprintf(msg, "Impossible de charger l'arbre [%s] dans le graphe de la personne.", sCodeDocData.c_str()) ;
        erreur(msg, standardError) ;
        return false ;
      }

      _pDataGraph->getTrees()->push_back(new NSDataTree(DTree)) ;
    }
	}

	return true ;
}

void
NSPersonGraphManager::setInfoPids(NSBasicAttributeArray *pAttr)
{
	if (pAttr)
		*_pAttrArray = *pAttr ;   // vider() is already done in = operator
  else
    _pAttrArray->vider() ;
}

void
NSPersonGraphManager::changeInfoPids(NSContexte *pContexte)
{
  initAttributesToCore() ;

	parseMainAttributes(pContexte) ;
}

bool
NSPersonGraphManager::validAttributes(PIDSTYPE iTypePids)
{
	bool bValid = false;

	switch (iTypePids)
	{
  	case pidsUtilisat :

    	if ((getAttributeValue(LOGIN)      != "") &&
          (getAttributeValue(PASSWORD)   != "") &&
          (getAttributeValue(PERSON)     != "") &&
          (getAttributeValue(OPERATOR)   != "") &&
          (getAttributeValue(USERTYPE)   != "") &&
          (getAttributeValue(PASSWDTYPE) != "") &&
          // (getAttributeValue(FIRST_NAME) != "") &&
          (getAttributeValue(LAST_NAME)  != ""))
      	bValid = true ;

      break ;

    case pidsPatient:
    case pidsCorresp :

    	if ((getAttributeValue(OPERATOR)  != "") &&
          (getAttributeValue(PERSON)    != "") &&
          (getAttributeValue(LAST_NAME) != ""))
      	bValid = true ;

      break ;
  }

	return bValid ;
}

void
NSPersonGraphManager::initAttributesToCore()
{
  // Get attributes to be kepy
  //
  NSBasicAttributeArray aAttrs ;
  aAttrs.push_back(new NSBasicAttribute(PERSON,   getAttributeValue(PERSON))) ;
	aAttrs.push_back(new NSBasicAttribute(OPERATOR, getAttributeValue(OPERATOR))) ;
	aAttrs.push_back(new NSBasicAttribute(CONSOLE,  getAttributeValue(CONSOLE))) ;
	aAttrs.push_back(new NSBasicAttribute(INSTANCE, getAttributeValue(INSTANCE))) ;

  *_pAttrArray = aAttrs ;
}

string
NSPersonGraphManager::getAttributeValue(string sBalise)
{
	return _pAttrArray->getAttributeValue(sBalise) ;
}

void
NSPersonGraphManager::setAttributeValue(string sBalise, string sValue)
{
	_pAttrArray->setAttributeValue(sBalise, sValue) ;
}

bool
NSPersonGraphManager::trouveNomCorresp(NSContexte *pContexte, string& sInfo)
{
	NSPatPathoArray PatPathoCor(_pSuper) ;

	NSDataTreeIter iterTree ;
	PatPathoCor.vider() ;

	if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
  	(*iterTree)->getRawPatPatho(&PatPathoCor) ;
  else
  {
  	char msg[255] ;
    strcpy(msg, "Impossible de charger l'arbre [ZADMI1] dans le graphe administratif.") ;
    erreur(msg, standardError) ;
    return false ;
	}

	string sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail ;

	ChargeDonneesAdmin(pContexte, &PatPathoCor, sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail) ;
	sInfo = sNom + string(" ") + sPrenom ;

	return true ;
}

bool
NSPersonGraphManager::trouveCivilite(NSContexte *pContexte, string& sInfo, string sLang)
{
  if (string("") == sLang)
    sLang = string("fr") ;

  NSPatPathoArray PatPathoArray(_pSuper) ;
  NSDataTreeIter iterTree ;

  if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;
  else
  {
    char msg[255] ;
    strcpy(msg, "Impossible de charger l'arbre [ZADMI1] dans le graphe administratif.") ;
    erreur(msg, standardError)  ;
    return false ;
  }

  string sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail ;
  ChargeDonneesAdmin(pContexte, &PatPathoArray, sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail) ;

  // Si la personne est un professionnel de santé
  if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
  {
    PatPathoArray.vider() ;
    (*iterTree)->getRawPatPatho(&PatPathoArray) ;
    string sMetier, sSpec, sActiveJob, sOrient, sCivil, sTitre, sVille, sAdeli, sRpps ;
    ChargeDonneesPDS(pContexte, &PatPathoArray, sMetier, sSpec, sActiveJob, sOrient, sCivil, sTitre, sVille, sAdeli, sRpps) ;

    if (string("") != sCivil)
      sCivilite = sCivil ;

    if (string("") != sTitre)
    {
      // pContexte->getDico()->donneLibelle(sLang, &sCodeTitre, &sTitre) ;
      sTitre[0] = pseumaj(sTitre[0]) ;
      sInfo = sTitre + string(" ") + sPrenom + string(" ") + sNom ;
      return true ;
    }
  }

  if (string("") != sCivilite)
  {
    sCivilite[0] = pseumaj(sCivilite[0]) ;
    sInfo = sCivilite + string(" ") + sPrenom + string(" ") + sNom ;
    return true ;
  }

  // Calcul du nom long
  //
  string sNomLong = string("") ;
  int    iAge     = 0 ;
  bool   bIsChild = false ;
  size_t iBirthdateLen = strlen(sNaissance.c_str()) ;

  if ((string("") != sNaissance) && (string(iBirthdateLen, '0') != sNaissance))
  {
	  string sDateJour = donne_date_duJour() ;

    iAge = donne_age(sDateJour, sNaissance) ;

    if ((iAge >= 0) && (iAge <= 14))
    	bIsChild = true ;
  }

  if (string("fr") == sLang)
  {
    if (bIsChild)
    	sNomLong = string("Enfant ") ;
    else
    {
      if (sSexe[0] == '2')
      {
        if (iAge < 18)
        	sNomLong = string("Mlle ") ;
        else
        	sNomLong = string("Mme ") ;
      }
      else
      	sNomLong = string("M. ") ;
    }
  }

  if (string("en") == sLang)
  {
    if (bIsChild)
    	sNomLong = string("Child ") ;
    else
    {
      if (sSexe[0] == '2')
      {
        if (iAge < 18)
        	sNomLong = string("Miss ") ;
        else
        	sNomLong = string("Mrs ") ;
      }
      else
      	sNomLong = string("M. ") ;
    }
  }

	sNomLong += sPrenom ;
	sNomLong += string(" ") ;
	sNomLong += sNom ;

  sInfo = sNomLong ;

  return true ;
}

bool
NSPersonGraphManager::trouveObjectAdrPrinc(PIDSTYPE iTypePids, string& sObject, string& sChez)
{
	NSPatPathoArray PatPathoArray(_pSuper) ;
	NSDataTreeIter iterTree ;

	if (iTypePids == pidsPatient)
	{
		if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
		else
		{
    	string sErrorText = _pSuper->getText("personErrors", "cannotFindAdminTreeInsideAdminGraph") ;
      sErrorText += string(" (person = ") + getPersonID() + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }
  }
	else if (iTypePids == pidsCorresp)
	{
  	if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
    else
    {
    	string sErrorText = _pSuper->getText("personErrors", "cannotFindProTreeInsideAdminGraph") ;
      sErrorText += string(" (person = ") + getPersonID() + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }
  }
  else // cas utilisateur
		return false ;

	if (PatPathoArray.empty())
		return false ;

	PatPathoIter    iter ;
	string          sElemLex, sSens, sType;
	sObject = "" ;
	sChez   = "" ;

	// Chaque patpatho est une adresse de racine fille d'un élément LADRE
	// Les chapitres recherchés commencent en colonne 0
	iter = PatPathoArray.begin() ;
	int iColBase = (*iter)->getColonne() ;

	bool bPrincipale    = false ;
	bool bObject        = false ;
	bool bObjectDefault = false ;
	bool bChez          = true ;

	iter++ ;
	while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		sSens = (*iter)->getLexiqueSens() ;

		// Chapitre "lieu" / place chapter
    if (((iTypePids == pidsPatient) && (sSens == string("LCOOR"))) ||
        ((iTypePids == pidsCorresp) && (sSens == string("ULIEX"))))
    {
    	iter++ ;
      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un objet
        sSens = (*iter)->getLexiqueSens() ;

        if (sSens == string("LCADR"))
        {
        	iter++ ;
          while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 2))
          {
          	// on cherche ici un objet
            sSens = (*iter)->getLexiqueSens() ;

            if (sSens == string("LADRE"))
            {
            	int iColAdr = (*iter)->getColonne() ;

              iter++ ;
              while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColAdr))
              {
              	sSens = (*iter)->getLexiqueSens() ;

                // Chapitre "type" / Type chapter
                if (sSens == string("LTYPA"))
                {
                	iter++ ;
                  while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColAdr + 1))
                  {
                  	// on cherche ici un code lexique
                    sType = (*iter)->getLexiqueSens() ;

                    if (((iTypePids == pidsPatient) && (sType == string("URESI"))) ||
                        ((iTypePids == pidsCorresp) && (sType == string("LAZRE"))))
                    {
                    	bPrincipale = true ;
                      bChez = true ;
                    }

                    iter++ ;
                  }
                }
                else if (sSens == string("LQUIO"))
                {
                  sChez = PatPathoArray.getFreeTextValue(&iter) ;

                  if (string("") != sChez)
                    sChez = string("Chez ") + sChez ;
                }
                // Chapitre "lieu" / place chapter
                else if (sSens == string("LLIEU"))
                {
                	iter++ ;
                  while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColAdr + 1))
                  {
                  	// on cherche ici un objet
                    sElemLex = (*iter)->getLexique() ;
                    if (sElemLex == string("£OBJE1"))
                    {
                    	if ((bPrincipale) || (!bObjectDefault))
                      {
                      	sObject = (*iter)->getComplement() ;
                        if (string("") != sObject)
                        {
                        	if (bPrincipale)
                          	bObject = true ;
                          bObjectDefault = true ;
                          bChez = false ;
                        }
                      }
                    }
                    iter++ ;
                  }
                }
                else
                	iter++ ;
              }

              if (bObject)
              	return true ;
            }
            else
            	iter++ ;
          }
        }
        else
        	iter++ ;
      }
    }
    else
    	iter++ ;
  }

	// En principe, on n'a pas trouvé ici d'objet "adresse principale"
	// On renvoie la première trouvée s'il en existe une.
	if (bObjectDefault)
		return true ;

	return false ;
}

string
NSPersonGraphManager::trouveLibLongForInGraphAdr(PIDSTYPE iTypePids)
{
	NSPatPathoArray PatPathoArray(_pSuper) ;
	NSDataTreeIter iterTree ;

	if (iTypePids == pidsPatient)
	{
		if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
      return getAddressLabel(&PatPathoArray, iTypePids) ;
    }
		else
		{
    	string sErrorText = _pSuper->getText("personErrors", "cannotFindAdminTreeInsideAdminGraph") ;
      sErrorText += string(" (person = ") + getPersonID() + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      return string("") ;
    }
  }
	else if (iTypePids == pidsCorresp)
	{
  	if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    {
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
      string sAddress = getAddressLabel(&PatPathoArray, iTypePids) ;
      if (string("") != sAddress)
        return sAddress ;
    }

    if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    {
      (*iterTree)->getRawPatPatho(&PatPathoArray) ;
      string sAddress = getAddressLabel(&PatPathoArray, iTypePids) ;
      if (string("") != sAddress)
      {
    	  string sErrorText = _pSuper->getText("personErrors", "cannotFindProTreeInsideAdminGraph") ;
        sErrorText += string(" (person = ") + getPersonID() + string(")") ;
        _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	  erreur(sErrorText.c_str(), standardError) ;

        string sQuestionText = _pSuper->getText("personErrors", "doYouWantToUsePersonalInformation") ;
        int retVal = ::MessageBox(0, sQuestionText.c_str(), "", MB_YESNO) ;
			  if (IDYES == retVal)
      	  return sAddress ;
      }
    }
    return string("") ;
  }
  else // cas utilisateur
		return string("") ;
}

string
NSPersonGraphManager::getAddressLabel(NSPatPathoArray* pPPT, PIDSTYPE iTypePids)
{
	if ((NULL == pPPT) || pPPT->empty())
		return string("") ;

	PatPathoIter iter ;

	string sAdressPath = string("") ;
  if (iTypePids == pidsPatient)
  	sAdressPath = string("LCOOR1/LADRE1") ;
  else if (iTypePids == pidsCorresp)
  	sAdressPath = string("ULIEX1/LADRE1") ;

	bool bAdrFound = pPPT->CheminDansPatpatho(sAdressPath, string(1, cheminSeparationMARK), &iter) ;
  if ((false == bAdrFound) || (NULL == iter) || (pPPT->end() == iter))
		return string("") ;

  string sLine1   = string("") ;
  string sLine2   = string("") ;
  string sLine3   = string("") ;
  string sZipCode = string("") ;
  string sCity    = string("") ;
  string sCountry = string("") ;

	// Chaque patpatho est une adresse de racine fille d'un élément LADRE
	// Les chapitres recherchés commencent en colonne 0
	int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
		string sSens = (*iter)->getLexiqueSens() ;

		// First line
    if (sSens == string("LADL1"))
      sLine1 = pPPT->getFreeTextValue(&iter) ;

    // Second line
    else if (sSens == string("LADL2"))
      sLine2 = pPPT->getFreeTextValue(&iter) ;

    // Third line
    else if (sSens == string("LADL3"))
      sLine3 = pPPT->getFreeTextValue(&iter) ;

    // City
    else if (sSens == string("LVILL"))
    {
    	iter++ ;
      while ((pPPT->end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
				sSens = (*iter)->getLexiqueSens() ;

      	// Zip code
    		if (sSens == string("LZIP0"))
          sZipCode = pPPT->getFreeTextValue(&iter) ;

        // City name
    		else if (sSens == string("LCOMU"))
          sCity = pPPT->getFreeTextValue(&iter) ;

        // Country
    		else if (sSens == string("LPAYS"))
          sCountry = pPPT->getFreeTextValue(&iter) ;

        else
        	iter++ ;
      }
    }
    else
    	iter++ ;
  }

  string sAdress = sLine1 ;
  if (sLine2 != string(""))
  	sAdress += string("\r\n") + sLine2 ;
  if (sLine3 != string(""))
  	sAdress += string("\r\n") + sLine3 ;
	if (sCity != string(""))
  {
  	sAdress += string("\r\n") ;
  	if (sZipCode != string(""))
    	sAdress += sZipCode + string(" ") ;
    sAdress += sCity ;
  }
  if (sCountry != string(""))
  	sAdress += string("\r\n") + sCountry ;

	return sAdress ;
}

bool
NSPersonGraphManager::trouveEMail(PIDSTYPE iTypePids, string& sInfo)
{
	NSPatPathoArray PatPathoArray(_pSuper) ;
	NSDataTreeIter iterTree ;

	if (iTypePids == pidsPatient)
	{
  	if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
    	(*iterTree)->getRawPatPatho(&PatPathoArray) ;
    else
    {
    	string sErrorText = _pSuper->getText("personErrors", "cannotFindAdminTreeInsideAdminGraph") ;
      sErrorText += string(" (person = ") + getPersonID() + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }
  }
  else
	{
  	if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
    	(*iterTree)->getRawPatPatho(&PatPathoArray);
    else
    {
    	string sErrorText = _pSuper->getText("personErrors", "cannotFindProTreeInsideAdminGraph") ;
      sErrorText += string(" (person = ") + getPersonID() + string(")") ;
      _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
      return false ;
    }
  }

	string sTelp, sEmail;

	ChargeContacts(&PatPathoArray, iTypePids, sTelp, sEmail) ;

	sInfo = sEmail ;

	return true ;
}

bool
NSPersonGraphManager::exportXML(string sFichier)
{
  ofstream    outFile ;
  string      sOut ;

#ifndef _ENTERPRISE_DLL
  string sFichierOut = _pSuper->PathName("EHTM") + sFichier ;
#else
  string sFichierOut = sFichier ;
#endif

  outFile.open(sFichierOut.c_str()) ;
  if (!outFile)
  {
    string sErrorText = _pSuper->getText("fileErrors", "errorOpeningOutputFile") + string(" ") + sFichierOut ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
#ifndef _ENTERPRISE_DLL
    erreur(sErrorText.c_str(), standardError, 0) ;
#endif
    outFile.close() ;
  	return false ;
  }

  sOut = _pDataGraph->genereXML() ;

  for (size_t i = 0; i < strlen(sOut.c_str()); i++)
  	outFile.put(sOut[i]) ;

  outFile.close() ;
  return true ;
}

bool
NSPersonGraphManager::createPerson(NSPatPathoArray* /* pPatPathoArray */, int /* iTypePerson */)
{
	return false ;
}

bool
NSPersonGraphManager::createKernelGraph(NSPatPathoArray* /* pPatPathoAdmin */)
{
	return false ;
}


// Ici on renvoie le code d'erreur suivant :
// 0 : OK, pas d'erreur
// 1 : arbre admin non trouvé
// 2 : arbre PDS non trouvé
// 3 : aucun des arbres trouvé
int
NSPersonGraphManager::parseMainAttributes(NSContexte *pContexte)
{
  int iRes = 0 ;
  NSDataTreeIter iterTree ;

  if (_pDataGraph->getTrees()->ExisteTree("ZADMI1", _pSuper, &iterTree))
  {
  	NSPatPathoArray PatPathoAdmin(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoAdmin) ;
    if (false == setAdminAttributes(pContexte, &PatPathoAdmin, attribsInit))
    	iRes = 1 ;
  }
  else
    iRes = 1 ;

  if (_pDataGraph->getTrees()->ExisteTree("DPROS1", _pSuper, &iterTree))
  {
    NSPatPathoArray PatPathoPDS(_pSuper) ;
    (*iterTree)->getRawPatPatho(&PatPathoPDS) ;
    if (false == setPDSAttributes(pContexte, &PatPathoPDS, attribsInit))
    	iRes += 2 ;
  }
  else
    iRes += 2 ;

  return iRes ;
}

bool
NSPersonGraphManager::setAdminAttributes(NSContexte *pContexte, NSPatPathoArray* pPatPathoAdmin, ATTRIB_UPDATER iSettingType)
{
	if ((NULL == pPatPathoAdmin) || pPatPathoAdmin->empty())
		return false ;
	if ((*(pPatPathoAdmin->begin()))->getLexiqueSens() != "ZADMI")
  	return false ;

	string sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail ;
	ChargeDonneesAdmin(pContexte, pPatPathoAdmin, sNom, sPrenom, sCode, sSexe, sNaissance, sCivilite, sPhone, sMobile, sMail) ;
	setAdminAttrArray(iSettingType, sNom, sPrenom, sSexe, sNaissance) ;

  return true ;
}

bool
NSPersonGraphManager::setPDSAttributes(NSContexte *pContexte, NSPatPathoArray* pPatPathoPDS, ATTRIB_UPDATER iSettingType)
{
	if ((NULL == pPatPathoPDS) || pPatPathoPDS->empty())
		return false ;
	if ((*(pPatPathoPDS->begin()))->getLexiqueSens() != "DPROS")
  	return false ;

	string sMetier, sSpecialite, sActiveJob, sOrientation, sCivil, sCodeTitre, sTitre, sVille, sAdeli, sRpps ;
	ChargeDonneesPDS(pContexte, pPatPathoPDS, sMetier, sSpecialite, sActiveJob, sOrientation, sCivil, sTitre, sVille, sAdeli, sRpps) ;
	setPDSAttrArray(iSettingType, sMetier, sSpecialite, sActiveJob, sOrientation, sTitre, sVille, sAdeli) ;

	return true ;
}

bool
NSPersonGraphManager::setBothAttributes(NSContexte *pContexte, NSPatPathoArray* pPatPathoDual, ATTRIB_UPDATER iSettingType)
{
	if ((NULL == pPatPathoDual) || pPatPathoDual->empty())
		return false ;

	NSPatPathoArray PatPathoAdmin(_pSuper, graphPerson) ;
	NSPatPathoArray PatPathoPDS(_pSuper, graphPerson) ;

	partDualPatho(pPatPathoDual, &PatPathoAdmin, &PatPathoPDS) ;

	bool bAdminParse = setAdminAttributes(pContexte, &PatPathoAdmin, iSettingType) ;
  bool bPDSParse = setPDSAttributes(pContexte, &PatPathoPDS, iSettingType) ;

  return (bAdminParse && bPDSParse) ;
}

void
NSPersonGraphManager::ChargePatPathoMeta(NSPatPathoArray* pPatPathoMeta, string sUserId, string typeDoc, string nomDoc,
                                            string /* codeDoc */, string sTypeContenu, bool bVisible)
{
	if (NULL == pPatPathoMeta)
		return ;

	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

	// noeud racine
	Message Msg ;

  Msg.SetInteret("5") ;
	if (bVisible)
		Msg.SetVisible("1") ;
  else
  	Msg.SetVisible("0") ;
	pPatPathoMeta->ajoutePatho("ZDOCU1", &Msg, 0) ;

	// Intitulé : nom du document
	pPatPathoMeta->ajoutePatho("0INTI1", 1) ;
	Msg.Reset() ;
	Msg.SetTexteLibre(nomDoc.c_str()) ;
	pPatPathoMeta->ajoutePatho("£?????", &Msg, 2) ;

	// créateur : opérateur
	pPatPathoMeta->ajoutePatho("DOPER1", 1) ;
	Msg.Reset() ;
	Msg.SetComplement(sUserId) ;
	pPatPathoMeta->ajoutePatho("£SPID1", &Msg, 2) ;

	// Type : champ complement
  Msg.Reset() ;
	Msg.SetComplement(typeDoc.c_str()) ;
	pPatPathoMeta->ajoutePatho("0TYPE1", &Msg, 1) ;

	// Date de rédaction
	pPatPathoMeta->ajoutePatho("KREDA1", 1) ;
  Msg.Reset() ;
	Msg.SetUnit("2DA021") ;
	Msg.SetComplement(tpsNow.donneDateHeure()) ;
	pPatPathoMeta->ajoutePatho("£T0;19", &Msg, 2) ;

	if (sTypeContenu != "")
	{
		pPatPathoMeta->ajoutePatho("0TYPC1", 1) ;
		pPatPathoMeta->ajoutePatho(sTypeContenu, 2) ;
	}
}


void
NSPersonGraphManager::ChargeDonneesAdmin(NSContexte* pContexte,
                                         NSPatPathoArray* pPatPathoArray,
                                         string& sNom, string& sPrenom,
                                         string& sCode, string& sSexe,
                                         string& sNaissance, string& sCivilite,
                                         string& sPhone, string& sMobile,
                                         string& sMail, string sLang)
{
  if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
    return ;

  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LIDET (identité)
  // On pourrait imaginer prendre les premières valeurs "nom", "prénom"...
  // rencontrées
  //
  // We suppose that the values we need are in a sub-chapter LIDET (identity)
  // We could imagine we take the first encoutered "1st name", "2nd name"... values

  if (string("") == sLang)
    sLang = string("fr") ;

  sNom       = string("") ;
  sPrenom    = string("") ;
  sCode      = string("") ;
  sSexe      = string("") ;
  sNaissance = string("") ;
  sCivilite  = string("") ;
  sPhone     = string("") ;
  sMobile    = string("") ;
  sMail      = string("") ;

  PatPathoIter iter = pPatPathoArray->begin() ;
  int iColBase = (*iter)->getColonne() ;
  iter++ ;

  while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
  {
    string sSens = (*iter)->getLexiqueSens() ;

    // Chapitre "identité" / Identity chapter
    if (string("LIDET") == sSens)
    {
      int iColIdentity = (*iter)->getColonne() ;
      iter++ ;

      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // Nom du patient
        // Patient's name
        if ((string("LNOM0") == sSens) && (string("") == sNom))
          sNom = pPatPathoArray->getFreeTextValue(&iter) ;

        // Prénom du patient
        // Patient's second name
        else if ((string("LNOM2") == sSens) && (string("") == sPrenom))
          sPrenom = pPatPathoArray->getFreeTextValue(&iter) ;

        // Code patient
        // Patient's code
        else if ((string("LCOD0") == sSens) && (string("") == sCode))
          sPrenom = pPatPathoArray->getFreeTextValue(&iter) ;

        // Sexe du patient et civilité
        // Patient's sex and civility
        else if (string("LSEXE") == sSens)
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 1))
          {
            // on cherche ici un code lexique
            string sTemp = (*iter)->getLexiqueSens() ;

            if      (string("HMASC") == sTemp)
              sSexe = string("1") ;
            else if (string("HFEMI") == sTemp)
              sSexe = string("2") ;
            iter++ ;

            while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 2))
            {
              sSens = (*iter)->getLexiqueSens() ;

              // Civilité
              if (string("HCIVO") == sSens)
              {
                iter++ ;
                while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 3))
                {
                  // on cherche ici un code lexique pour un libelle
                  string sCodeLex = (*iter)->getLexique() ;
                  pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;
                  iter++ ;
                }
              }
              else
              	iter++ ;
            }
          }
        }
        // Date de naissance
        // Birthdate
        else if (string("KNAIS") == sSens)
        {
          iter++ ;
          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity + 1))
          {
            string sUnite  = (*iter)->getUnitSens() ;
            string sFormat = (*iter)->getLexiqueSens() ;
            string sValeur = (*iter)->getComplement() ;

            if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
              sNaissance = sValeur ;

            iter++ ;
          }
        }
        else
          iter++ ;
      }
    }
    else if (string("LCOOR") == sSens)
    {
      int iColIdentity = (*iter)->getColonne() ;
      iter++ ;

      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentity))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // Nom du patient
        // Patient's name
        if (string("LADRE") == sSens)
        {
          int iColIdentityAdres = (*iter)->getColonne() ;
          iter++ ;

          while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColIdentityAdres))
          {
            sSens = (*iter)->getLexiqueSens() ;

            if (string("LTELE") == sSens)
              sPhone = pPatPathoArray->getFreeTextValue(&iter) ;
            else
              iter++ ;
          }
        }
        else if (string("LTELP") == sSens)
          sMobile = pPatPathoArray->getFreeTextValue(&iter) ;

        else if (string("LMAIL") == sSens)
          sMail = pPatPathoArray->getFreeTextValue(&iter) ;
          
        else
          iter++ ;
      }
    }
    else
    	iter++ ;
  }
}

void
NSPersonGraphManager::setAdminAttrArray(ATTRIB_UPDATER iSettingType, string& sNom, string& sPrenom, string& sSexe, string& sNaissance)
{
	switch(iSettingType)
	{
		case attribsInit :
			_pAttrArray->push_back(new NSBasicAttribute(LAST_NAME,  sNom)) ;
      // Don't do this because the Pilot needs a firstname
      // if (string("") != sPrenom)
      _pAttrArray->push_back(new NSBasicAttribute(FIRST_NAME, sPrenom)) ;
      if (string("") != sSexe)
			  _pAttrArray->push_back(new NSBasicAttribute(SEX,        sSexe)) ;
      if (string("") != sNaissance)
			  _pAttrArray->push_back(new NSBasicAttribute(BIRTHDATE,  sNaissance)) ;
      break ;
    case attribsUpdate :
    	_pAttrArray->setAttributeValue(LAST_NAME,  sNom) ;
			_pAttrArray->setAttributeValue(FIRST_NAME, sPrenom) ;
			_pAttrArray->setAttributeValue(SEX,        sSexe) ;
			_pAttrArray->setAttributeValue(BIRTHDATE,  sNaissance) ;
      break ;
    case attribsChange :
    	_pAttrArray->changeAttributeValue(LAST_NAME,  sNom) ;
			_pAttrArray->changeAttributeValue(FIRST_NAME, sPrenom) ;
			_pAttrArray->changeAttributeValue(SEX,        sSexe) ;
			_pAttrArray->changeAttributeValue(BIRTHDATE,  sNaissance) ;
      break ;
	}
}

void
NSPersonGraphManager::ChargeDonneesPDS(NSContexte *pContexte,
                                       NSPatPathoArray* pPatPathoArray,
                                       string& sMetier, string& sSpec,
                                       string& sActiveJob, string& sOrient,
                                       string& sCivilite, string& sTitre,
                                       string& sVille, string& sAdeli,
                                       string& sRpps, string sLang)
{
	if (((NSPatPathoArray*) NULL == pPatPathoArray) || (pPatPathoArray->empty()))
		return ;
    
	//
	// On part du principe que les données qui nous intéressent sont dans un
	// sous chapitre DPROS (professionnel de santé)
	// On pourrait imaginer prendre les premières valeurs "métier", "spécialité"...
	// rencontrées
	//
	// We suppose that the values we need are in a sub-chapter DPROS (health professional)
	// We could imagine we take the first encoutered "job", "speciality"... values
	//
	if (string("") == sLang)
		sLang = string("") ;

	sMetier    = string("") ;
	sSpec      = string("") ;
  sActiveJob = string("") ;
	sOrient    = string("") ;
	sCivilite  = string("") ;
	sTitre     = string("") ;
  sVille     = string("") ;
  sAdeli     = string("") ;
  sRpps      = string("") ;

	PatPathoIter iter = pPatPathoArray->begin() ;
	int iColBase = (*iter)->getColonne() ;

	while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() >= iColBase))
	{
		string sSens = (*iter)->getLexiqueSens() ;

		// Chapitre "professionnel de santé" / Health professional chapter
		if (string("DPROS") == sSens)
		{
			int iColPros = (*iter)->getColonne() ;
			iter++ ;

			while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColPros))
			{
				sSens = (*iter)->getLexiqueSens() ;

				if (sSens == string("LCOMP"))
				{
					int iColCompetences = (*iter)->getColonne() ;
					iter++ ;

					while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCompetences))
					{
						sSens = (*iter)->getLexiqueSens() ;

						// Métier
						if ((string("LMETI") == sSens) && (string("") == sMetier))
						{
							// on cherche ici un code lexique
							iter++;
							if (string((*iter)->getLexique(), 0, 2) == "£C")
							{
								iter++ ;
								while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                {
                	sMetier = (*iter)->getLexique() ;
                  iter++ ;
                }
							}
							else
              {
              	sMetier = (*iter)->getLexique() ;
                iter++;
              }
						}
						// Spécialité
						else if ((string("LSPEC") == sSens) && (string("") == sSpec))
						{
							// on cherche ici un edit lexique (ayant comme fils un code lexique)
							iter++;
							if (string((*iter)->getLexique(), 0, 2) == "£C")
							{
								iter++ ;
								while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                {
                	sSpec = (*iter)->getLexique() ;
                  iter++ ;
                }
							}
							else
              {
              	sSpec = (*iter)->getLexique() ;
                iter++ ;
              }
						}
            // Spécialité
						else if ((string("LPROF") == sSens) && (string("") == sActiveJob))
						{
							// on cherche ici un edit lexique (ayant comme fils un code lexique)
							iter++;
							if (string((*iter)->getLexique(), 0, 2) == "£C")
							{
								iter++ ;
								while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                {
                	sActiveJob = (*iter)->getLexique() ;
                  iter++ ;
                }
							}
							else
              {
              	sActiveJob = (*iter)->getLexique() ;
                iter++ ;
              }
						}
						// Orientation
						else if ((string("LORIE") == sSens) && (string("") == sOrient))
						{
							// on cherche ici un edit lexique (ayant comme fils un code lexique)
							iter++;
							if (string((*iter)->getLexique(), 0, 2) == "£C")
							{
								iter++ ;
								while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCompetences+2))
                {
                	sOrient = (*iter)->getLexique() ;
                  iter++ ;
                }
							}
							else
              {
								sOrient = (*iter)->getLexique() ;
								iter++ ;
							}
						}
						else
							iter++ ;
					}
				}
        // Civilite
				else if (string("HCIVO") == sSens)
				{
					iter++;
					while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColPros+1))
					{
						// on cherche ici un code lexique pour un libelle
						string sCodeLex = (*iter)->getLexique() ;
						pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sCivilite) ;

						iter++;
					}
				}
        // Identifiers
        else if (string("LIPRO") == sSens)
				{
					iter++;
					while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColPros+1))
					{
						sSens = (*iter)->getLexiqueSens() ;

						// Adeli
						if (sSens == string("LADEL"))
              sAdeli = pPatPathoArray->getFreeTextValue(&iter) ;

            // Rpps
						else if (sSens == string("LRPPS"))
              sRpps = pPatPathoArray->getFreeTextValue(&iter) ;
						
            else
              iter++ ;
					}
				}
				// Titre
				else if (string("LTITR") == sSens)
				{
					iter++;
					while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColPros+1))
					{
						// on cherche ici un code lexique pour un libelle
						string sCodeLex = (*iter)->getLexique() ;
						pContexte->getDico()->donneLibelle(sLang, &sCodeLex, &sTitre) ;

						iter++ ;
					}
				}
        else if (string("ULIEX") == sSens)
				{
					int iColLieux = (*iter)->getColonne() ;
					iter++ ;

					while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColLieux))
					{
						sSens = (*iter)->getLexiqueSens() ;

						// Adresse
						if (sSens == string("LADRE"))
						{
            	int iColAdr = (*iter)->getColonne() ;
							iter++ ;
              while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColAdr))
							{
								sSens = (*iter)->getLexiqueSens() ;

								// Adresse
								if (sSens == string("LVILL"))
								{
                	int iColVille = (*iter)->getColonne() ;
									iter++ ;
              		while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColVille))
									{
										sSens = (*iter)->getLexiqueSens() ;

										// Adresse
										if (sSens == string("LCOMU"))
                      sVille = pPatPathoArray->getFreeTextValue(&iter) ;
                    else
            					iter++ ;
                  }
                }
                else
            			iter++ ;
              }
            }
            else
            	iter++ ;
          }
        }
				else
					iter++ ;
			}
		}
		else
			iter++ ;
	}
}

void
NSPersonGraphManager::setPDSAttrArray(ATTRIB_UPDATER iSettingType, string& sMetier, string& sSpec, string& sActiveJob, string& sOrient, string& sTitre, string& sVille, string& sAdeli)
{
  string sCodeSens ;

	switch(iSettingType)
	{
		case attribsInit :
/*
      These traits don't exist in the database

      if (string("") != sMetier)
      {
        pContexte->getDico()->donneCodeSens(&sMetier, &sCodeSens) ;
			  _pAttrArray->push_back(new NSBasicAttribute(PROFESSION,  sCodeSens)) ;
      }
      if (string("") != sSpec)
      {
        pContexte->getDico()->donneCodeSens(&sSpec, &sCodeSens) ;
			  _pAttrArray->push_back(new NSBasicAttribute(SPECIALITY,  sCodeSens)) ;
      }
      if (string("") != sOrient)
      {
        pContexte->getDico()->donneCodeSens(&sOrient, &sCodeSens) ;
			  _pAttrArray->push_back(new NSBasicAttribute(ORIENTATION, sCodeSens)) ;
      }
      if (string("") != sTitre)
			  _pAttrArray->push_back(new NSBasicAttribute(TITLE,       sTitre)) ;
*/
      if (string("") != sVille)
        _pAttrArray->push_back(new NSBasicAttribute(CITY_PRO,    sVille)) ;
      if (string("") != sActiveJob)
      {
        NSDico::donneCodeSens(&sActiveJob, &sCodeSens) ;
        _pAttrArray->push_back(new NSBasicAttribute(ACTIVE_JOB,  sCodeSens)) ;
      }
      if (string("") != sAdeli)
        _pAttrArray->push_back(new NSBasicAttribute(ADELI_ID,    sAdeli)) ;
      break ;

    case attribsUpdate :
      NSDico::donneCodeSens(&sMetier, &sCodeSens) ;
    	_pAttrArray->setAttributeValue(PROFESSION,  sCodeSens) ;
      NSDico::donneCodeSens(&sSpec, &sCodeSens) ;
			_pAttrArray->setAttributeValue(SPECIALITY,  sCodeSens) ;
      NSDico::donneCodeSens(&sOrient, &sCodeSens) ;
			_pAttrArray->setAttributeValue(ORIENTATION, sCodeSens) ;
			_pAttrArray->setAttributeValue(TITLE,       sTitre) ;
      _pAttrArray->setAttributeValue(CITY_PRO,    sVille) ;
      NSDico::donneCodeSens(&sActiveJob, &sCodeSens) ;
      _pAttrArray->setAttributeValue(ACTIVE_JOB,  sCodeSens) ;
      _pAttrArray->setAttributeValue(ADELI_ID,    sAdeli) ;
      break ;

    case attribsChange :
      NSDico::donneCodeSens(&sMetier, &sCodeSens) ;
    	_pAttrArray->changeAttributeValue(PROFESSION,  sCodeSens) ;
      NSDico::donneCodeSens(&sSpec, &sCodeSens) ;
			_pAttrArray->changeAttributeValue(SPECIALITY,  sCodeSens) ;
      NSDico::donneCodeSens(&sOrient, &sCodeSens) ;
			_pAttrArray->changeAttributeValue(ORIENTATION, sCodeSens) ;
			_pAttrArray->changeAttributeValue(TITLE,       sTitre) ;
      _pAttrArray->changeAttributeValue(CITY_PRO,    sVille) ;
      NSDico::donneCodeSens(&sActiveJob, &sCodeSens) ;
      _pAttrArray->changeAttributeValue(ACTIVE_JOB,  sCodeSens) ;
      _pAttrArray->changeAttributeValue(ADELI_ID,    sAdeli) ;
      break ;
	}
}

void
NSPersonGraphManager::ChargeContacts(NSPatPathoArray* pPatPathoArray, PIDSTYPE iTypePids, string& sTelp, string& sEmail)
{
	if ((NULL == pPatPathoArray) || (pPatPathoArray->empty()))
		return ;
	//
  // On part du principe que les données qui nous intéressent sont dans un
  // sous chapitre LCOOR (coordonnees) ou ULIEX (lieu d'exercice) pour les corresp et les utilisateurs

  sTelp  = string("") ;
  sEmail = string("") ;

	PatPathoIter iter = pPatPathoArray->begin() ;
	int iColBase = (*iter)->getColonne() ;
	iter++ ;

	while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColBase))
	{
    string sSens = (*iter)->getLexiqueSens() ;

    // Chapitre "identité" / Identity chapter
    if (((iTypePids == pidsPatient) && (string("LCOOR") == sSens)) ||
            (string("ULIEX") == sSens))
    {
    	int iColCoor = (*iter)->getColonne() ;
      iter++ ;

      while ((pPatPathoArray->end() != iter) && ((*iter)->getColonne() > iColCoor))
      {
        sSens = (*iter)->getLexiqueSens() ;

        // Nom du patient
        // Patient's name
        if ((string("LTELP") == sSens) && (string("") == sTelp))
          sTelp = pPatPathoArray->getFreeTextValue(&iter) ;

        // Prénom du patient
      	// Patient's second name
        else if ((string("LMAIL") == sSens) && (string("") == sEmail))
          sEmail = pPatPathoArray->getFreeTextValue(&iter) ;
        
        else
        	iter++ ;
      }
    }
    else
    	iter++ ;
	}
}

void
NSPersonGraphManager::buildDualPatho(NSPatPathoArray* pDualPatho, NSPatPathoArray* pAdminPatho, NSPatPathoArray* pPDSPatho)
{
	if ((NULL == pDualPatho) || (NULL == pAdminPatho) || (NULL == pPDSPatho))
		return ;

	pDualPatho->vider() ;

  *pDualPatho = *pAdminPatho ;

  if (false == pPDSPatho->empty())
		pDualPatho->InserePatPatho(pDualPatho->end(), pPDSPatho, 0, true) ;
}

void
NSPersonGraphManager::partDualPatho(NSPatPathoArray* pDualPatho, NSPatPathoArray* pAdminPatho, NSPatPathoArray* pPDSPatho)
{
	if ((NULL == pDualPatho) || (NULL == pAdminPatho) || (NULL == pPDSPatho))
		return ;

	pAdminPatho->vider() ;
  pPDSPatho->vider() ;

  if (pDualPatho->empty())
		return ;

	NSVectPatPathoArray Vect ;
  PatPathoIter iterRoot = pDualPatho->begin() ;
  pDualPatho->ExtraireVecteurPatPathoFreres(iterRoot, &Vect) ;

	if (Vect.empty())
		return ;

	PatPathoIterVect iterVect = Vect.begin() ;
	while (Vect.end() != iterVect)
	{
  	if (false == (*iterVect)->empty())
    {
			// Le type de chaque frere est donné par le code lexique de sa racine
    	iterRoot = (*iterVect)->begin() ;
    	string sTypeFrere = (*iterRoot)->getLexiqueSens() ;

    	if      (string("ZADMI") == sTypeFrere)
    		*pAdminPatho = *(*iterVect) ;
    	else if (string("DPROS") == sTypeFrere)
    		*pPDSPatho = *(*iterVect) ;
    }

    iterVect++ ;
  }
}

string
NSPersonGraphManager::getTreeDocId(string sMasterDocId)
{
  return getLinkedDocId(sMasterDocId, NSRootLink::semantizedDocument) ;
}

string
NSPersonGraphManager::getStructuredDocId(string sMasterDocId)
{
  return getLinkedDocId(sMasterDocId, NSRootLink::structuredVersionOfDocument) ;
}

string
NSPersonGraphManager::getExternalDocId(string sMasterDocId)
{
  return getLinkedDocId(sMasterDocId, NSRootLink::externViewOfDocument) ;
}

string
NSPersonGraphManager::getLinkedDocId(string sMasterDocId, NSRootLink::NODELINKTYPES masterLink)
{
  if ((string("") == sMasterDocId) || (NULL == _pLinkManager))
    return string("") ;

  VecteurString aConnectedNodes ;
  _pLinkManager->TousLesVraisDocument(sMasterDocId, masterLink, &aConnectedNodes, "ENVERS") ;

  if (aConnectedNodes.empty())
    return string("") ;

  return **(aConnectedNodes.begin()) ;
}

bool
NSPersonGraphManager::setTemplatePres(NSContexte *pContexte, string sCodeDocPres, string sTemplate, string sEnTete)
{
	string sRightsPres ;
	NSPatPathoArray PatPathoArray(_pSuper, graphPerson) ;

	if (false == getTree(sCodeDocPres, &PatPathoArray, &sRightsPres))
		return false ;

  if (PatPathoArray.empty())
		return true ;

	PatPathoIter iter = PatPathoArray.begin() ;
	int iColBase = (*iter)->getColonne() ;
	iter++ ;

	while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
	{
    string sSens = (*iter)->getLexiqueSens() ;

    // Fichier Template
    if (string("0TPL0") == sSens)
    {
    	iter++ ;
      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	if ((*iter)->getTexteLibre() != sTemplate)
          {
          	(*iter)->setNodeID(string("")) ;
        		(*iter)->setTexteLibre(sTemplate) ;
          }
        }

        iter++ ;
      }
    }
    // Fichier En-tete
    else if (string("0ENTE") == sSens)
    {
    	iter++ ;
      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        {
        	if ((*iter)->getTexteLibre() != sEnTete)
          {
          	(*iter)->setNodeID(string("")) ;
        		(*iter)->setTexteLibre(sEnTete) ;
          }
        }

        iter++ ;
      }
    }
    else
    	iter++ ;
	} // boucle principale

	setTree(&PatPathoArray, "", sCodeDocPres) ;
	commitGraphTree(pContexte, sCodeDocPres) ;

	return true ;
}

bool
NSPersonGraphManager::getTemplatePres(string sCodeDocPres, string& sTemplate, string& sEnTete)
{
	NSPatPathoArray PatPathoArray(_pSuper, graphPerson) ;

  string sRightsPres ;
	if (false == getTree(sCodeDocPres, &PatPathoArray, &sRightsPres))
		return false ;

  if (PatPathoArray.empty())
		return true ;

  sTemplate = string("") ;
  sEnTete   = string("") ;

	PatPathoIter iter = PatPathoArray.begin() ;
	int iColBase = (*iter)->getColonne() ;
	iter++ ;

	while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase))
	{
    string sSens = (*iter)->getLexiqueSens() ;

    // Fichier Template
    if (string("0TPL0") == sSens)
    {
    	iter++ ;
      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        	sTemplate = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    // Fichier En-tete
    else if (string("0ENTE") == sSens)
    {
    	iter++ ;
      while ((PatPathoArray.end() != iter) && ((*iter)->getColonne() > iColBase + 1))
      {
      	// on cherche ici un texte libre
        string sElemLex = (*iter)->getLexique() ;
        if (string("£?????") == sElemLex)
        	sEnTete = (*iter)->getTexteLibre() ;

        iter++ ;
      }
    }
    else
    	iter++ ;
	} // boucle principale

	return true ;
}

bool
NSPersonGraphManager::getLibIDsPpt(NSPatPathoArray* pPPTIdent) const
{
	if ((NSPatPathoArray*) NULL == pPPTIdent)
		return false ;

  NSDataTreeIter iterTree ;
  if (false == _pDataGraph->getTrees()->ExisteTree("0LIBI1", _pSuper, &iterTree))
		return false ;

	if ((NSDataTreeIter) NULL == iterTree)
		return false ;

	(*iterTree)->getRawPatPatho(pPPTIdent) ;

  return true ;
}

bool
NSPersonGraphManager::IPPEnCours(NSPatPathoArray* pPPTIdent, string sSite, string* psIpp,
                                    string* psOuvre, string* psFerme) const
{
	if (((NSPatPathoArray*) NULL == pPPTIdent) || (pPPTIdent->empty()))
		return false ;

	string sDateJour = donne_date_duJour() + donne_heure() ;
	PatPathoIter iter = pPPTIdent->begin() ;
	// int iColBase = (*iter)->getColonne() ;

	string sIppTrouve = string("") ;
	string sOuvre     = string("") ;
  string sFerme     = string("") ;

	iter++ ;

  bool bIPPEnCours = false ;
	while ((pPPTIdent->end() != iter) && (false == bIPPEnCours))
	{
		iter = pPPTIdent->ChercherItem("LSITE", false, iter) ;

		if (pPPTIdent->end() != iter)
		{
    	int iColSite = (*iter)->getColonne() ;
      iter++ ;

      // 2 kind of site records
      //
      // General IPP
      //
      // LSITE1, , , , , , +0001001, , A, 000000, 000000
      //   LIPP01, , , , , , +0002002, , A, 000000, 000000
      //     £CL000, , , , , 00005, +0003003, 1, A, 000000, 000000
      //     KOUVR1, , , , , , +0004003, , A, 000000, 000000
      //       £T0;19, 20140621210333, , , 2DA021, , +0005004, 1, A, 000000, 000000
      //
      // Site attached IPP
      //
      // LSITE1, , , , , , +0001001, , A, 000001, 000001
      //   £OBJE1, $00000000003R, , , , , +0002002, 1, A, 000001, 000001
      //   LIPP01, , , , , , +0003002, , A, 000001, 000001
      //     £CL000, , , , , 0000D, +0004003, 1, A, 000001, 000001
      //     KOUVR1, , , , , , +0005003, , A, 000001, 000001
      //       £T0;19, 20140621210333, , , 2DA021, , +0006004, 1, A, 000000, 000000

      string sSens = (*iter)->getLexiqueSens() ;

      // Site attached IPP, check if the site is the proper one
      //
      if (string("£OB") == sSens)
      {
      	string sSiteTrouve = (*iter)->getComplement() ;
        iter++ ;

        if ((sSiteTrouve == sSite) || (string("*") == sSite))
        {
        	while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColSite))
          {
          	sSens = (*iter)->getLexiqueSens() ;

            if (string("LIPP0") == sSens)
            {
            	int iColIpp = (*iter)->getColonne() ;

              sIppTrouve = string("") ;
              sOuvre     = string("") ;
              sFerme     = string("") ;

              iter++ ;

              while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp))
              {
              	sSens = (*iter)->getLexiqueSens() ;

                if (string("£CL") == sSens)
                {
                	sIppTrouve = (*iter)->getTexteLibre() ;
                  iter++ ;
                }
                else if (string("KOUVR") == sSens)
                {
                	iter++ ;
                  while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
                  {
                    string sUnite  = (*iter)->getUnitSens() ;
                    string sFormat = (*iter)->getLexiqueSens() ;
                    string sValeur = (*iter)->getComplement() ;

                    if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                    	sOuvre = sValeur ;

                    iter++ ;
                  }
                }
                else if (string("KFERM") == sSens)
                {
                	iter++ ;
                  while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
                  {
                  	string sUnite  = (*iter)->getUnitSens() ;
                    string sFormat = (*iter)->getLexiqueSens() ;
                    string sValeur = (*iter)->getComplement() ;

                    if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                    	sFerme = sValeur ;

                    iter++ ;
                  }
                }
                else
                	iter++ ;
              }
            }
            else
            	iter++ ;
          }

          if (string("") != sIppTrouve)
          {
          	if ((string("") != sOuvre) && (sOuvre <= sDateJour))
            {
            	if ((string("") == sFerme) || (sFerme > sDateJour))
              	bIPPEnCours = true ;
            }
        	}
        }
      }
      // Local IPP
      //
      else if (string("LIPP0") == sSens)
      {
        int iColIpp = (*iter)->getColonne() ;

        sIppTrouve = string("") ;
        sOuvre     = string("") ;
        sFerme     = string("") ;

        iter++ ;

        while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp))
        {
          sSens = (*iter)->getLexiqueSens() ;

          if (string("£CL") == sSens)
          {
            sIppTrouve = (*iter)->getTexteLibre() ;
            iter++ ;
          }
          else if (string("KOUVR") == sSens)
          {
            iter++ ;
            while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
            {
              string sUnite  = (*iter)->getUnitSens() ;
              string sFormat = (*iter)->getLexiqueSens() ;
              string sValeur = (*iter)->getComplement() ;

              if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                sOuvre = sValeur ;

              iter++ ;
            }
          }
          else if (string("KFERM") == sSens)
          {
            iter++ ;
            while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
            {
              string sUnite  = (*iter)->getUnitSens() ;
              string sFormat = (*iter)->getLexiqueSens() ;
              string sValeur = (*iter)->getComplement() ;

              if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                sFerme = sValeur ;

              iter++ ;
            }
          }
          else
            iter++ ;
        }
        if ((string("") != sIppTrouve) && ((string("") == sSite) || (string("*") == sSite)))
        {
          if      ((string("") == sOuvre) && (string("") == sFerme))
            bIPPEnCours = true ;
          else if ((string("") != sOuvre) && (sOuvre <= sDateJour))
          {
            if ((string("") == sFerme) || (sFerme > sDateJour))
              bIPPEnCours = true ;
          }
        }
      }
      else
        while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColSite))
          iter++ ;
    }
  }

	if (bIPPEnCours)
	{
		if (psIpp)
    	*psIpp = sIppTrouve ;

    if (psOuvre)
    	*psOuvre = sOuvre ;

    if (psFerme)
    	*psFerme = sFerme ;
  }

	return bIPPEnCours ;
}

bool
NSPersonGraphManager::ClotureIPP(NSPatPathoArray* pPPTIdent, string sSite, string sIpp, NSPatPathoArray* pPPTIpp)
{
	if ((NULL == pPPTIdent) || pPPTIdent->empty())
		return false ;

	string sDateJour = donne_date_duJour() + donne_heure() ;
	bool bDateTrouvee = false ;
	bool bPremier     = true ;

	PatPathoIter iterFirst, iterPrec ;

	string sIppTrouve = string("") ;
	string sOuvre     = string("") ;
  string sFerme     = string("") ;

  int iColIpp       = 0 ;

  PatPathoIter iter = pPPTIdent->begin() ;
	iter++ ;

  bool bIPPTrouve = false ;
	while ((pPPTIdent->end() != iter) && (false == bIPPTrouve))
	{
  	iter = pPPTIdent->ChercherItem("LSITE", false, iter) ;

    if (pPPTIdent->end() != iter)
    {
    	int iColSite = (*iter)->getColonne() ;
      iter++ ;

      string sSens = (*iter)->getLexiqueSens() ;

      if (string("£OB") == sSens)
      {
      	string sSiteTrouve = (*iter)->getComplement() ;
        iter++ ;

        if (sSiteTrouve == sSite)
        {
        	while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColSite))
          {
          	sSens = (*iter)->getLexiqueSens() ;

            if (string("LIPP0") == sSens)
            {
            	iColIpp = (*iter)->getColonne() ;

              if (bPremier)
              {
              	iterFirst = iter ;
                bPremier = false ;
              }

              sIppTrouve = string("") ;
              sOuvre     = string("") ;
              sFerme     = string("") ;
              iter++ ;

              iterPrec = iter ;

              while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp))
              {
              	sSens = (*iter)->getLexiqueSens() ;

                if (string("£CL") == sSens)
                {
                	sIppTrouve = (*iter)->getTexteLibre() ;
                  iter++ ;
                }
                else if (string("KOUVR") == sSens)
                {
                	iter++ ;
                  while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
                  {
                  	string sUnite  = (*iter)->getUnitSens() ;
                    string sFormat = (*iter)->getLexiqueSens() ;
                    string sValeur = (*iter)->getComplement() ;

                    if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                    	sOuvre = sValeur ;

                    iter++ ;
                  }
                }
                else if (sSens == string("KFERM"))
                {
                	iter++ ;
                  while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColIpp+1))
                  {
                  	string sUnite  = (*iter)->getUnitSens() ;
                    string sFormat = (*iter)->getLexiqueSens() ;
                    string sValeur = (*iter)->getComplement() ;

                    if ((string("2DA01") == sUnite) || (string("2DA02") == sUnite))
                    {
                    	sFerme = sValeur ;

                      if (sIppTrouve == sIpp)
                      {
                      	if (sFerme > sDateJour)
                        	(*iter)->setComplement(sDateJour) ;

                        bDateTrouvee = true ;
                      }
                    }

                    iter++ ;
                  }
                }
                else
                	iter++ ;
              }
            }
            else
            	iter++ ;
          }
          if (sIppTrouve == sIpp)
          	bIPPTrouve = true ;
        }
      }
    }
  }

	if ((bIPPTrouve) && (false == bDateTrouvee))
	{
  	NSPatPathoArray PPTFerme(_pSuper, graphPerson) ;

    PPTFerme.ajoutePatho("KFERM1", 0) ;
    Message Msg ;
    Msg.SetComplement(sDateJour) ;
    Msg.SetUnit("2DA021") ;
    PPTFerme.ajoutePatho("£T0;19", &Msg, 1) ;

    // on cherche l'ipp suivant pour l'insertion
    iter = pPPTIdent->ChercherItem("LIPP0", false, iterPrec) ;

    pPPTIdent->InserePatPatho(iter, &PPTFerme, iColIpp + 1) ;
	}

	if (pPPTIpp)
		pPPTIdent->ExtrairePatPathoFreres(iterFirst, pPPTIpp) ;

	return bIPPTrouve ;
}

bool
NSPersonGraphManager::InsereIPPFusion(NSPatPathoArray* pPPTIdent, string sSite, NSPatPathoArray* pPPTIpp)
{
	if ((NULL == pPPTIdent) || pPPTIdent->empty())
		return false ;

  PatPathoIter iterNext ;

  PatPathoIter iter = pPPTIdent->begin() ;
  int iColSite = (*iter)->getColonne() + 1 ;

  iter++ ;

  string sSiteTrouve = string("") ;

	while ((pPPTIdent->end() != iter) && (sSiteTrouve != sSite))
	{
  	iter = pPPTIdent->ChercherItem("LSITE", false, iter) ;

    if (pPPTIdent->end() != iter)
    {
    	iColSite = (*iter)->getColonne() ;
      iter++ ;

      string sSens = (*iter)->getLexiqueSens() ;

      if (string("£OB") == sSens)
      {
      	sSiteTrouve = (*iter)->getComplement() ;
        iter++ ;

        if (sSiteTrouve == sSite)
        {
        	while ((pPPTIdent->end() != iter) && ((*iter)->getColonne() > iColSite))
          	iter++ ;

          iterNext = iter ;
        }
      }
    }
  }

	if (sSiteTrouve == sSite)
	{
		pPPTIdent->InserePatPatho(iterNext, pPPTIpp, iColSite + 1) ;
	}
  else
  {
  	NSPatPathoArray	pptIdLib(_pSuper) ;

    pptIdLib.ajoutePatho("LSITE1", 0) ;

    Message Msg ;
    Msg.SetComplement(sSite) ;
    pptIdLib.ajoutePatho("£OBJE1", &Msg, 1) ;

    pPPTIdent->InserePatPatho(pPPTIdent->end(), &pptIdLib, iColSite) ;
    pPPTIdent->InserePatPatho(pPPTIdent->end(), pPPTIpp, iColSite + 1) ;
  }

  return true ;
}

bool
NSPersonGraphManager::LastOpenStay(NSContexte *pContexte, string sDateTime, NSSejourInfo* pStayInfo)
{
	string sRootTree = getRootTree() ;

  VecteurString VectString ;
  VecteurString VecteurData  ;

  NSSejourArray SejourArray ;

  if (false == GetStayArray(pContexte, &SejourArray))
    return false ;

  if (SejourArray.empty())
    return false ;

  NSSejourIter lastOpenIter = NULL ;

  // Deuxième passe : on regarde en fonction de la date le dernier
  // séjour ouvert (dans lequel s'inscrit la date passée en paramètre)
  for (NSSejourIter k = SejourArray.begin() ; SejourArray.end() != k ; k++)
  {
  	// on regarde si la date d'ouverture est antérieure à la date du compte-rendu
    if ((*k)->getBeginDate() <= sDateTime)
    {
    	if (lastOpenIter == NULL)
      	lastOpenIter = k ;
      else
      {
      	// Quand deux séjours sont valides, on prend le plus récent
        if ((*k)->getBeginDate() >= (*lastOpenIter)->getBeginDate())
        	lastOpenIter = k ;
      }
    }
  }

  if (NULL == lastOpenIter)
    return false ;

  *pStayInfo = *(*lastOpenIter) ;

  return true ;
}

bool
NSPersonGraphManager::GetStayArray(NSContexte *pContexte, NSSejourArray* pStayArray)
{
  if ((NSSejourArray*) NULL == pStayArray)
    return false ;

  string sRootTree = getRootTree() ;

  VecteurString VectString ;

  // Getting all stays trees linked to patient's root node
  // On cherche l'ensemble des séjours liés au noeud root du patient
  //
  _pLinkManager->TousLesVrais(sRootTree, NSRootLink::fonctionalUnitStay, &VectString) ;
  if (VectString.empty())
    return true ;

  for (EquiItemIter i = VectString.begin() ; VectString.end() != i ; i++)
  {
    NSDocumentInfo DocumInfo(*(*i), pContexte, this) ;

    NSPatPathoArray PatPathoArray(_pSuper, graphPerson) ;

    bool bOk = false ;

    // On remonte le lien data du méta-document
    string sCodeDocData = getDataIdFromMetaId(DocumInfo.getCodeDocMeta()) ;
    if (string("") != sCodeDocData)
    {
      string sCodePat = string(sCodeDocData, 0, PAT_NSS_LEN) ;
      string sCodeDoc = string(sCodeDocData, PAT_NSS_LEN, DOC_CODE_DOCUM_LEN) ;

      DocumInfo.setPatient(sCodePat) ;
      DocumInfo.setDocument(sCodeDoc) ;

      if ((DocumInfo.ParseMetaDonnees()) &&
                    (DocumInfo.DonnePatPatho(&PatPathoArray, this)))
        bOk = true ;
    }

    if (bOk && (false == PatPathoArray.empty()))
    {
      // Parsing de la patpatho et remplissage de la NSSejourInfo
      // que l'on stocke dans une array

      NSSejourInfo SejourInfo ;
      if (SejourInfo.initFromPatPatho(&PatPathoArray))
        pStayArray->push_back(new NSSejourInfo(SejourInfo)) ;
    }
  }

  return true ;
}

void
NSPersonGraphManager::ConnectRootTreeToGoal(NSContexte *pContexte, string sTree, string sGoalNode, NSRootLink::NODELINKTYPES iLink)
{
	// On cherche le méta - Looking for the meta data tree identifier
	//
	string sRootTree = string(sTree, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
  string sMetaTree = getMetaIdFromDataId(sRootTree) ;
	if (string("") == sMetaTree)
		return ;

	// Getting the tree
	//
	NSPatPathoArray PPtMeta(_pSuper) ;
	string sRosace = "" ;
	if ((getTree(sMetaTree, &PPtMeta, &sRosace)) && (false == PPtMeta.empty()))
	{
  	PatPathoIter pptIter ;
    if (PPtMeta.CheminDansPatpatho("ZDOCU/0TYPC", string(1, cheminSeparationMARK), &pptIter))
    {
    	string sNode = (*pptIter)->getNode() ;
      _pLinkManager->etablirLien(sNode, iLink, sGoalNode) ;
    }
	}

	// on enregistre le méta
	setTree(&PPtMeta, sRosace, sMetaTree) ;
	commitGraphTree(pContexte, sMetaTree) ;

	return ;
}

void
NSPersonGraphManager::ConnectNodeToGoal(NSContexte *pContexte, string sTreeNode, string sGoalNode, NSPatPathoArray* pData, NSRootLink::NODELINKTYPES iLink)
{
	// On cherche le méta - Looking for the meta data tree identifier
	//
	string sRootTree = string(sTreeNode, 0, PAT_NSS_LEN + DOC_CODE_DOCUM_LEN) ;
  string sMetaTree = getMetaIdFromDataId(sRootTree) ;
	if (string("") == sMetaTree)
		return ;

	// Getting tree
	//
	NSPatPathoArray PPtMeta(_pSuper) ;
	string sRosace = string("") ;
	if ((getTree(sMetaTree, &PPtMeta, &sRosace)) && (false == PPtMeta.empty()))
	{
  	PatPathoIter pptIter ;
    string sNodePere = string("") ;

    // Looking for "follow-up" datas
    // Follow-up data are duplicated from data tree to meta tree in order to
    // display goals state even if data tree is not available
    //
    if (PPtMeta.CheminDansPatpatho("ZDOCU/9SUIV", string(1, cheminSeparationMARK), &pptIter))
    {
    	VecteurString VString ;
      _pLinkManager->TousLesVrais(sTreeNode, NSRootLink::copyOf, &VString, "ENVERS") ;
      if (false == VString.empty())
      	sNodePere = *(*(VString.begin())) ;

      if (string("") != sNodePere)
      {
      	// cas où il faut remettre à jour les données de suivi dans le méta
        // on remplace seulement les fils pour éviter de refaire les liens
        //
        NSPatPathoArray DataSuiv(_pSuper) ;
        NSPatPathoArray MetaDataSuiv(_pSuper) ;
        pData->ExtrairePatPatho(pData->begin(), &DataSuiv) ;
        pptIter = PPtMeta.ChercherNoeud(sNodePere) ;
        PPtMeta.ExtrairePatPatho(pptIter, &MetaDataSuiv) ;

        // on teste l'egalite du point de vue des données
        if (!MetaDataSuiv.estEgal(&DataSuiv))
        {
        	PPtMeta.SupprimerFils(pptIter) ;
          PPtMeta.InserePatPathoFille(pptIter, &DataSuiv) ;
        }

        _pLinkManager->etablirLien(sNodePere, (NSRootLink::NODELINKTYPES) iLink, sGoalNode) ;
      }
      else
      {
      	// Dans ce cas, on insere les pData à la fin des données de suivi
        // apres avoir placé les liens temporaires
        //
        PatPathoIter iterData = pData->begin() ;
        (*iterData)->addTemporaryLink(sTreeNode, NSRootLink::copyOf, dirFleche) ;
        (*iterData)->addTemporaryLink(sGoalNode, iLink, dirFleche) ;

        int iColSuiv = (*pptIter)->getColonne() ;

        //
        // On se positionne sur l'élément qui suit le dernier fils
        //
        pptIter++ ;
        while ((PPtMeta.end() != pptIter) && ((*pptIter)->getColonne() > iColSuiv))
        	pptIter++ ;

        PPtMeta.InserePatPatho(pptIter, pData, iColSuiv + 1) ;
      }
    }
    else
    {
    	// on doit ici ajouter le chapître "données de suivi"
      // avant d'insérer les pData
      //
      PPtMeta.ajoutePatho("9SUIV1", 1) ;

      PatPathoIter iterData = pData->begin() ;
      (*iterData)->addTemporaryLink(sTreeNode, NSRootLink::copyOf, dirFleche) ;
      (*iterData)->addTemporaryLink(sGoalNode, iLink, dirFleche) ;

      PPtMeta.InserePatPatho(PPtMeta.end(), pData, 2) ;
    }
	}

  // on enregistre le méta
  setTree(&PPtMeta, sRosace, sMetaTree) ;
  commitGraphTree(pContexte, sMetaTree) ;
}

// fin de nsmanager.cpp
////////////////////////////////////////////////////

