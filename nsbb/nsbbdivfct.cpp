// -----------------------------------------------------------------------------// nsbbdivfct.cpp
// -----------------------------------------------------------------------------
// Fenêtre de liste des Equipes
// -----------------------------------------------------------------------------
// $Revision: 1.32 $
// $Author: pameline $
// $Date: 2016/12/29 16:32:46 $
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
#include <stdio.h>
#include "partage\nsglobal.h"
#include "partage\ns_search.h" // Keep it there or NSSearchStruct is badly initialized

#include "nssavoir\nspathor.h"
#include "nsbb\nsbbitem.h"#include "nsbb\nsbbsmal.h"#include "nsbb\nsbbtran.h"#include "nsbb\nsdlg.h"#include "nsbb\nsradiob.h"
#include "nsbb\nstrewi.h"
#include "nsbb\nsbouton.h"
#include "nsbb\nstrnode.h"
#include "nsbb\nsedit.h"
#include "nsbb\nschkbox.h"
#include "partage\nsdivfct.h"
#include "nsbb\nsbbdivfct.h"
#include "nsbb\nsbb_msg.h" //NSDLGRETOUR_DIRECT ..
#include "partage\nsmatfic.h"
#include "nsldv\nsldvuti.h"
#include "nsbb\nsattval.h"
#include "nsbb\nsarc.h"
#include "nsbb\nsmanager.h"
#include "nautilus\nsepicap.h"
#include "nsbb\nsmpidsobj.h"
#include "nssavoir\nsconver.h"
#include "nssavoir\nsgraphe.h"
#include "nsbb\nsdefArch.h"
#include "nautilus\nssoapview.h"
#include "nsdn\nsdochis.h"
#include "nautilus\nscsvue.h"
#include "nautilus\nshistdo.h"
#include "nsepisod\nsToDo.h"

# include "nsbb\nslistwind.h"
# include "pilot\NautilusPilot.hpp"
# include "nsbb\tagNames.h"

# include "ns_ob1\BB1types.h"

#define DECALAGE_INDEX_ID 200  // Décalage entre un contrôle et son static d'index

struct ltstring
{
  bool operator()(const string s1, const string s2) const
  {
    return s1 < s2 ;
  }
};

//***************************************************************************
//                      Fonctions utilitaires
//***************************************************************************

HistoryConnector::HistoryConnector()
{
  _iConnectionType  = isUndefined ;
  _bIsUnique        = false ;        // Better set to multiple than erase data
  _sElementPath     = string("") ;
  _sItemLabel       = string("") ;
  _sDestinationPath = string("") ;
  _sHistoryPath     = string("") ;
}

HistoryConnector::HistoryConnector(const HistoryConnector& src)
{
  _iConnectionType  = src._iConnectionType ;
  _bIsUnique        = src._bIsUnique ;
  _sElementPath     = src._sElementPath ;
  _sItemLabel       = src._sItemLabel ;
  _sDestinationPath = src._sDestinationPath ;
  _sHistoryPath     = src._sHistoryPath ;

  if (false == src._aSubPaths.empty())
    for (vector<string*>::iterator it = (string**) src._aSubPaths.begin() ; src._aSubPaths.end() != it ; it++)
      _aSubPaths.push_back(new string(**it)) ;
}

HistoryConnector::~HistoryConnector()
{
  emptySubPath() ;
}

HistoryConnector&
HistoryConnector::operator=(const HistoryConnector& src)
{
  if (this == &src)
  	return *this ;

	_iConnectionType  = src._iConnectionType ;
  _bIsUnique        = src._bIsUnique ;
  _sElementPath     = src._sElementPath ;
  _sItemLabel       = src._sItemLabel ;
  _sDestinationPath = src._sDestinationPath ;
  _sHistoryPath     = src._sHistoryPath ;

  emptySubPath() ;

  if (false == src._aSubPaths.empty())
    for (vector<string*>::iterator it = (string**) src._aSubPaths.begin() ; src._aSubPaths.end() != it ; it++)
      _aSubPaths.push_back(new string(**it)) ;

	return (*this) ;
}

int
HistoryConnector::operator==(const HistoryConnector& o)
{
  return ((_sElementPath == o._sElementPath) && (_sItemLabel == o._sItemLabel)) ;
}

void
HistoryConnector::addSubPath(const string sPath)
{
  _aSubPaths.push_back(new string(sPath)) ;
}

void
HistoryConnector::emptySubPath()
{
  if (_aSubPaths.empty())
    return ;

  for (vector<string*>::iterator it = _aSubPaths.begin() ; _aSubPaths.end() != it ; )
  {
    delete *it ;
    _aSubPaths.erase(it) ;
  }
}

bool
HistoryConnector::isInSubPath(const string sSubPath)
{
  // Warning: it is perfectly valid to have sSubPath == ""
  //
  if (_aSubPaths.empty())
    return false ;

  for (vector<string*>::iterator it = _aSubPaths.begin() ; _aSubPaths.end() != it ; it++)
    if (**it == sSubPath)
      return true ;

  return false ;
}

string
HistoryConnector::getPath()
{
  string sLabelSens = string("") ;
  NSDico::donneCodeSens(&_sItemLabel, &sLabelSens) ;

  string sPath = string("") ;

  if (string("") != _sElementPath)
  {
    sPath = _sElementPath ;
    if (string("") != sLabelSens)
      sPath += string(1, cheminSeparationMARK) ;
  }

  sPath += sLabelSens ;

  return sPath ;
}

//***************************************************************************
//                      Fonctions utilitaires
//***************************************************************************

HistoryConnector*
GetHistoryConnectorForPath(NSDlgFonction *pNSFct, HistoryConnectorArray *pHCa)
{
  if (((NSDlgFonction*)          NULL == pNSFct) ||
       ((HistoryConnectorArray*) NULL == pHCa)   || pHCa->empty())
    return (HistoryConnector*) 0 ;

  if (((NSControle*)     NULL == pNSFct->getControl()) ||
      ((NSTransferInfo*) NULL == pNSFct->getControl()->getTransfert()))
    return (HistoryConnector*) 0 ;

  BBFilsItem* pFilsForValue = pNSFct->getControl()->getTransfert()->getFilsItem() ;
  if ((BBFilsItem*) NULL == pFilsForValue)
    return (HistoryConnector*) 0 ;

  string sLocalisation = pFilsForValue->getLocalisation() ;

  HistoryConnectorIter hIt = pHCa->begin() ;
  for ( ; pHCa->end() != hIt ; hIt++)
    if ((*hIt)->getPath() == sLocalisation)
      return *hIt ;

  return (HistoryConnector*) 0 ;
}

void
ActiveChampEdit(BBFilsItem* fils, string sVal)
{
  if ((BBFilsItem*) NULL == fils)
    return ;

  fils->ActiveControle(sVal) ;
}

//
// Find first control that is a descendant of pItem and whose "Identity" contains sEtiquette
//
// This method is recursive and initialize pValeurEdit with control's content is found
// It returns the BBFilsItem attached to this control
//
BBFilsItem*
RetournerValeurEdit(BBItem* pItem, string sEtiquette, string* pValeurEdit, bool bSemanticSearch)
{
	if (((BBItem*) NULL == pItem) || (pItem->_aBBItemFils.empty()))
	{
    if (pValeurEdit)
		  *pValeurEdit = string("") ;
		return (BBFilsItem*) 0 ;
	}

  string sSemanticPath = string("") ;
  if (bSemanticSearch)
    NSDico::donneCodeSens(&sEtiquette, &sSemanticPath) ;

  bool bTexteLibre = false ;
  if (sEtiquette.find(string("£CL")) != NPOS)
  	bTexteLibre = true ;

  BBiter iterFils1 = pItem->_aBBItemFils.begin() ;
	while (pItem->_aBBItemFils.end() != iterFils1)
	{
    NSControle*	pControl = (NSControle*) 0 ;
    if ((*iterFils1)->getItemTransfertData())
      pControl = (*iterFils1)->getItemTransfertData()->getControl() ;

    // If filsItem if connected to a control, check if this control's identity
    // contains the path we are looking for
    //
		if (pControl)
		{
      string sControlIdentity = pControl->getIdentite() ;

      string sSemanticIdentity = string("") ;
      if (bSemanticSearch)
        NSDico::donneCodeSens(&sControlIdentity, &sSemanticIdentity) ;

			if ((sControlIdentity.find(sEtiquette) != NPOS) ||
          (bSemanticSearch && (sSemanticIdentity.find(sSemanticPath) != NPOS)))
			{
        if (pValeurEdit)
        {
          if (bTexteLibre)
        	  *pValeurEdit = (*iterFils1)->getItemTransfertMsg()->GetTexteLibre() ;
          else
					  *pValeurEdit = (*iterFils1)->getItemTransfertMsg()->GetComplement() ;
        }
				return *iterFils1 ;
			}
		}

    // Not found, iterate recursively through BBItems that are sons of this BBFilsItem
    //
		if (false == (*iterFils1)->VectorFils.empty())
		{			BBiterFils petitFils1 = (*iterFils1)->VectorFils.begin() ;
			for ( ; (*iterFils1)->VectorFils.end() != petitFils1 ; petitFils1++)
			{
				BBiter iterChampEdit1 = (*petitFils1)->_aBBItemFils.begin() ;

				while ((*petitFils1)->_aBBItemFils.end() != iterChampEdit1)				{          NSControle*	pSonControl = (NSControle*) 0 ;          if ((*iterChampEdit1)->getItemTransfertData())
            pSonControl = (*iterChampEdit1)->getItemTransfertData()->getControl() ;					if (pSonControl)
					{
            string sSonControlIdentity = pSonControl->getIdentite() ;

            string sSemanticSonIdentity = string("") ;
            if (bSemanticSearch)
              NSDico::donneCodeSens(&sSonControlIdentity, &sSemanticSonIdentity) ;

						if ((sSonControlIdentity.find(sEtiquette) != NPOS) ||
                (bSemanticSearch && (sSemanticSonIdentity.find(sSemanticPath) != NPOS)))
						{
              if (pValeurEdit)
              {
                if (bTexteLibre)
        				  *pValeurEdit = (*iterChampEdit1)->getItemTransfertMsg()->GetTexteLibre() ;
                else
								  *pValeurEdit = (*iterChampEdit1)->getItemTransfertMsg()->GetComplement() ;
              }
							return *iterChampEdit1 ;
						}
					}
					if (false == (*iterChampEdit1)->VectorFils.empty())
					{
						BBiterFils iter = (*iterChampEdit1)->VectorFils.begin() ;
						BBFilsItem* fils = RetournerValeurEdit(*iter, sEtiquette, pValeurEdit, bSemanticSearch) ;
						if (fils)
							return fils ;
					}

					iterChampEdit1++ ;
				}
			}
		}
		iterFils1++ ;
	}
  if (pValeurEdit)
	  *pValeurEdit = string("") ;
    
	return (BBFilsItem*) 0 ;
}

BBFilsItem*
RetournerValeurEdit(BBItem* pItem, string sEtiquette, string* pValeurEdit, bool bFromRoot, bool bSemanticSearch)
{
  if ((BBItem*) NULL == pItem)
  {
    if (pValeurEdit)
		  *pValeurEdit = string("") ;
    return (BBFilsItem*) 0 ;
  }

	// Si on doit repartir de la racine
	//
	if (bFromRoot)
	{
		BBItem* pVRItem = pItem ;
		bool    tourner = true ;
		while (tourner)
		{
			BBFilsItem* pVRFilsItem = pVRItem->_pBBFilsPere ;
			if (NULL == pVRFilsItem)
				tourner = false ;
			else
				pVRItem = pVRFilsItem->getItemFather() ;
		}
		return
			RetournerValeurEdit(pVRItem, sEtiquette, pValeurEdit, false, bSemanticSearch) ;
	}

	if (pItem->_aBBItemFils.empty())	{
    if (pValeurEdit)
		  *pValeurEdit = string("") ;
		return (BBFilsItem*) 0 ;
	}

  string sSemanticPath = string("") ;
  if (bSemanticSearch)
    NSDico::donneCodeSens(&sEtiquette, &sSemanticPath) ;

	// Recherche effective
	//

	//
	// On passe en revue les fils
	//
  BBiter iterFils1 = pItem->_aBBItemFils.begin();
	while (pItem->_aBBItemFils.end() != iterFils1)
	{
    NSControle*	pControl = (NSControle*) 0 ;
    if ((*iterFils1)->getItemTransfertData())
      pControl = (*iterFils1)->getItemTransfertData()->getControl() ;

		// On regarde si le fils est bien celui qu'on cherche
		//
		if (pControl)
		{
      string sControlIdentity = pControl->getIdentite() ;

      string sSemanticIdentity = string("") ;
      if (bSemanticSearch)
        NSDico::donneCodeSens(&sControlIdentity, &sSemanticIdentity) ;

			if ((sControlIdentity.find(sEtiquette) != NPOS) ||
          (bSemanticSearch && (sSemanticIdentity.find(sSemanticPath) != NPOS)))
			{
        if (pValeurEdit)
				  *pValeurEdit = (*iterFils1)->getItemTransfertMsg()->GetComplement() ;
				return *iterFils1 ;
			}
		}
		// Sinon, on lance itérativement sur les BBItem dérivés
		//
		if (false == (*iterFils1)->VectorFils.empty())
		{
			BBiterFils petitFils1 = (*iterFils1)->VectorFils.begin() ;
			for ( ; (*iterFils1)->VectorFils.end() != petitFils1 ; petitFils1++)
			{
				BBiter iterChampEdit1 = (*petitFils1)->_aBBItemFils.begin() ;

				while ((*petitFils1)->_aBBItemFils.end() != iterChampEdit1)
				{
          NSControle*	pSonControl = (NSControle*) 0 ;
          if ((*iterChampEdit1)->getItemTransfertData())
            pSonControl = (*iterChampEdit1)->getItemTransfertData()->getControl() ;
					if (pSonControl)
					{
            string sSonControlIdentity = pSonControl->getIdentite() ;

            string sSemanticSonIdentity = string("") ;
            if (bSemanticSearch)
              NSDico::donneCodeSens(&sSonControlIdentity, &sSemanticSonIdentity) ;

						if ((sSonControlIdentity.find(sEtiquette) != NPOS) ||
                (bSemanticSearch && (sSemanticSonIdentity.find(sSemanticPath) != NPOS)))
						{
              if (pValeurEdit)
							  *pValeurEdit = (*iterChampEdit1)->getItemTransfertMsg()->GetComplement() ;
							return *iterChampEdit1 ;
						}
					}
					if (false == (*iterChampEdit1)->VectorFils.empty())
					{
						BBiterFils iter = (*iterChampEdit1)->VectorFils.begin() ;
						BBFilsItem* fils = RetournerValeurEdit(*iter, sEtiquette, pValeurEdit, false, bSemanticSearch) ;
						if (fils)
							return fils ;
					}

					iterChampEdit1++ ;
				}
			}
		}
		iterFils1++ ;
	}

  if (pValeurEdit)
	  *pValeurEdit = string("") ;
    
	return (BBFilsItem*) 0 ;
}

void
RetournerDateMax(BBItem* pItem, string* pDateMax)
{
  if (((BBItem*) NULL == pItem) || ((string*) NULL == pDateMax))
    return ;

  if (pItem->_aBBItemFils.empty())
	{
    *pDateMax = string("") ;
		return ;
	}

  for (BBiter iterFils = pItem->_aBBItemFils.begin() ; pItem->_aBBItemFils.end() != iterFils ; iterFils++)
  {
    string sRefValeur = (*iterFils)->getItemLabel() ;
    string sValSens   = string("") ;
    NSDico::donneCodeSens(&sRefValeur, &sValSens) ;

    if (string("KDARE") == sValSens)
    {
      if (false == (*iterFils)->VectorFils.empty())
      {
        BBiterFils BBFils = (*iterFils)->VectorFils.begin() ;
        if (false == (*BBFils)->_aBBItemFils.empty())
        {
          BBiter iterPetitFils = (*BBFils)->_aBBItemFils.begin() ;
          if (((*iterPetitFils)->getItemLabel()).find("£T") != NPOS)
          {
            string sValDate = (*iterPetitFils)->getItemTransfertMsg()->GetComplement() ;
            if (sValDate > (*pDateMax))
              *pDateMax = sValDate ;
          }
        }
      }
    }
    else
    {
      for (BBiterFils BBFils = (*iterFils)->VectorFils.begin() ; (*iterFils)->VectorFils.end() != BBFils ; BBFils++)
        RetournerDateMax(*BBFils, pDateMax) ;
    }
  }
}

//
// Recherche dans pDialog d'un contrôle situé après pNSFct, mais avant tout
// autre contrôle ayant une fonction identique à pNSFct, et de fonction sFctName
//
// Looking in pDialog for a control located after pNSFct's control, but before
// any other control with the same function as pNSFct, whose function is sFctName
//
NSControle*
RechercheFctApres(NSControleVector* pNSCtrl, NSDlgFonction *pNSFct, string sFctName)
{
  if ((NULL == pNSFct) || (NULL == pNSCtrl) || (pNSCtrl->empty()))
    return (NSControle*) 0 ;

  string      sThisFct   = pNSFct->getFonction() ;
  NSControle* pThisCtrl  = pNSFct->getControl() ;

  bool        bFoundThis = false ;

  iterNSControle itCtrl ;
  for (itCtrl = pNSCtrl->begin() ; pNSCtrl->end() != itCtrl ; itCtrl++)
  {
    if ((*itCtrl)->getFonction())
    {
      // Au delà du contrôle actuel / Beyond current control
      if (bFoundThis)
      {
        if ((*itCtrl)->getFonction()->containFonction(sFctName))
          break ;
        if ((*itCtrl)->getFonction()->getFonction() == sThisFct)
          return (NSControle*) 0 ;
      }
      else
      {
        if (*itCtrl == pThisCtrl)
          bFoundThis = true ;
      }
    }
  }
  if (itCtrl == pNSCtrl->end())
    return (NSControle*) 0 ;

  return *itCtrl ;
}

//
// Recherche dans pDialog d'un contrôle situé avant pNSFct, mais après tout
// autre contrôle ayant une fonction identique à pNSFct, et de fonction sFctName
//
// Looking in pDialog for a control located before pNSFct's control, but after
// any other control with the same function as pNSFct, whose function is sFctName
//
NSControle*
RechercheFctAvant(NSControleVector* pNSCtrl, NSDlgFonction *pNSFct, string sFctName)
{
  if ((NULL == pNSFct) || (NULL == pNSCtrl) || (pNSCtrl->empty()))
    return (NSControle*) 0 ;

  string      sThisFct   = pNSFct->getFonction() ;
  NSControle* pThisCtrl  = pNSFct->getControl() ;
  NSControle* pCandidate = (NSControle*) 0 ;

  iterNSControle itCtrl ;
  for (itCtrl = pNSCtrl->begin() ; pNSCtrl->end() != itCtrl ; itCtrl++)
  {
    if ((*itCtrl)->getFonction())
    {
      if (*itCtrl == pThisCtrl)
        return pCandidate ;

      if ((*itCtrl)->getFonction()->containFonction(sFctName))
        pCandidate = *itCtrl ;
      if ((*itCtrl)->getFonction()->getFonction() == sThisFct)
        pCandidate = (NSControle*) 0 ;
    }
  }
  return (NSControle*) 0 ;
}

//----------------------------------------------------------------------------
// retourner le BBFilsItem ayant sEtiquette comme étiquette.
//----------------------------------------------------------------------------
BBFilsItem*
TrouverFilsAyantBonneEtiquette(BBItem* pere, string sEtiquette)
{
	if ((NULL == pere) || (pere->_aBBItemFils.empty()))
  	return (BBFilsItem*) 0 ;

  string sEtiq = string("") ;
  NSDico::donneCodeSens(&sEtiquette, &sEtiq) ;

	BBiter Iter ;
	for (Iter = pere->_aBBItemFils.begin() ; pere->_aBBItemFils.end() != Iter ; Iter++)
  {
    string sEtiqIter ;
    string sFilsLabel = (*Iter)->getItemLabel() ;
    NSDico::donneCodeSens(&sFilsLabel, &sEtiqIter) ;

		//égalité stricte
		if (sEtiqIter == sEtiq)
			return *Iter ;
  }

	return (BBFilsItem*) 0 ;
}

BBFilsItem*
FindElementInPathFromRootChapter(BBItem* pRootItem, string sPath)
{
	if (((BBItem*) NULL == pRootItem) || (string("") == sPath))
		return (BBFilsItem*) 0 ;

  ClasseStringVector PathVect ;
	DecomposeChaine(&sPath, &PathVect, string(1, cheminSeparationMARK)) ;
  if (PathVect.empty())
  	return (BBFilsItem*) 0 ;

  BBFilsItem* pFilsItem    = (BBFilsItem*) 0 ;
  BBItem*     pCurrentItem = pRootItem ;

  size_t iLevel = 1 ;
  iterClassString iterpVect = PathVect.begin() ;
  for ( ; PathVect.end() != iterpVect ; iterpVect++, iLevel++)
  {
	  // Find the FilsItem that commands the level
    //
	  pFilsItem = TrouverFilsAyantBonneEtiquette(pCurrentItem, (*iterpVect)->getItem()) ;
    if ((BBFilsItem*) NULL == pFilsItem)
		  return (BBFilsItem*) 0 ;

    // If this element is not the last one
    //
    if (PathVect.size() > iLevel)
    {
      if (pFilsItem->VectorFils.empty())
        return (BBFilsItem*) 0 ;

      // Let's just have a look in first branch
      //
      pCurrentItem = *(pFilsItem->VectorFils.begin()) ;
    }
  }

  return pFilsItem ;
}

string
FindValueInPathFromRootChapter(BBItem* pRootItem, string sPath, string sValueString)
{
  BBFilsItem* pFilsItem = FindElementInPathFromRootChapter(pRootItem, sPath) ;

	if ((BBFilsItem*) NULL == pFilsItem)
		return string("") ;

  // Get its information storage structure
  //
	if ((NULL == pFilsItem->getItemTransfertData()) ||
      (NULL == pFilsItem->getItemTransfertData()->getPatPatho()))
		return string("") ;  NSVectFatheredPatPathoArray *pFatheredPptArray = pFilsItem->getItemTransfertData()->getPatPatho() ;	if (pFatheredPptArray->empty())
		return string("") ;

  // Get needed value inside the storage structure
  //
	string sValue ;
	NSPatPathoArray* pPPT = (*(pFatheredPptArray->begin()))->getPatPatho() ;
	if (false == pPPT->CheminDansPatpatho(pRootItem->_pBigBoss, sValueString, &sValue))
		return string("") ;

	return sValue ;
}

string
FindValueInRootChapter(BBItem* pRootItem, string sChapter, string sValueString)
{
	if ((NULL == pRootItem) || (string("") == sValueString))
		return string("") ;

	// Find the FilsItem that commands the chapter
  //
	BBFilsItem* pVRFilsItem = pRootItem->TrouverFilsAyantBonneEtiquette(sChapter, string("")) ;
	if (NULL == pVRFilsItem)
		return string("") ;

  // Get its information storage structure
  //
	if ((NULL == pVRFilsItem->getItemTransfertData()) || (NULL == pVRFilsItem->getItemTransfertData()->getPatPatho()))
		return string("") ;  NSVectFatheredPatPathoArray *pFatheredPptArray = pVRFilsItem->getItemTransfertData()->getPatPatho() ;	if (pFatheredPptArray->empty())
		return string("") ;

  // Get needed value inside the storage structure
  //
	string sValue ;
	NSPatPathoArray* pPPT = (*(pFatheredPptArray->begin()))->getPatPatho() ;
	if (false == pPPT->CheminDansPatpatho(pRootItem->_pBigBoss, sValueString, &sValue))
		return string("") ;

	return sValue ;
}

/***************************************************************/      						// Données administratives
/***************************************************************/

//-------------------------------------------------------------------
// Afficher la date du jour
//-------------------------------------------------------------------
void
FixerDateDuJour(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	int iCtrlType = pNSFct->getControl()->getType() ;
  if ((iCtrlType != isEditDate) && (iCtrlType != isEditDateHeure))
		return ;

	if (!pNSFct->getControl()->getTransfert() || !pNSFct->getControl()->getTransfert()->getTransfertMessage())
		return ;

	string sDate = pNSFct->getControl()->getTransfert()->getTransfertMessage()->GetComplement() ;
	if (string("") == sDate)
	{
  	NVLdVTemps tpsNow ;
    tpsNow.takeTime() ;
    if      (iCtrlType == isEditDate)
    	sDate = tpsNow.donneDate() ;
    else if (iCtrlType == isEditDateHeure)
			sDate = tpsNow.donneDateHeure() ;
	}
	ActiveChampEdit(pNSFct->getControl()->getTransfert()->getFilsItem(), sDate) ;
}

//-------------------------------------------------------------------
// Synchronisation d'unités entre deux edits frères
// Values synchronization for 2 brother edits with different units
//-------------------------------------------------------------------
void
SynchroNumValues(NSDlgFonction *pNSFct)
{
try
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	NSTransferInfo*	pTrans = pNSFct->getControl()->getTransfert() ;
	if (NULL == pTrans)
  	return ;

  BBFilsItem* pFilsCurrent = pTrans->getFilsItem() ;
  if (NULL == pFilsCurrent)
    return ;

	//
	// Est-ce une étiquette multiple ?
	//
	size_t posit = (pFilsCurrent->getItemLabel()).find(string(1, cheminSeparationMARK));
  if (NPOS == posit)
  	return ;

	string sUnit = string(pFilsCurrent->getItemLabel(), 0, posit) ;
	size_t posit_debut = posit + 1 ;
	string sLexique = string(pFilsCurrent->getItemLabel(), posit_debut, strlen(pFilsCurrent->getItemLabel().c_str()) - posit_debut) ;

  string sValue = pTrans->getTransfertMessage()->GetComplement() ;

	BBItem*	pItemPere = pFilsCurrent->getItemFather() ;
  if (NULL == pItemPere)
		return ;

  string sRefValeur = string("") ;
  if (pItemPere->_pBBFilsPere)
  	sRefValeur = pItemPere->_pBBFilsPere->getItemLabel() ;

	if (pItemPere->_aBBItemFils.empty())
		return ;

	NSCV* pNSCV = NULL ;

  BBiter iter = pItemPere->_aBBItemFils.begin() ;
  for ( ; pItemPere->_aBBItemFils.end() != iter ; iter++)
	{
		if (*iter != pFilsCurrent)
		{
    	posit = ((*iter)->getItemLabel()).find(string(1, cheminSeparationMARK));

			NSTransferInfo*	pIterTrans = (*iter)->getItemTransfertData() ;
			if (pIterTrans && pIterTrans->getTransfertMessage() && (NPOS != posit))
			{
        string sIterLabel = (*iter)->getItemLabel() ;

				string sIterUnit = string(sIterLabel, 0, posit) ;
				posit_debut = posit + 1 ;
				string sIterLexique = string(sIterLabel, posit_debut, strlen(sIterLabel.c_str()) - posit_debut) ;

        //
        // We only synchronize homogeneous data
        //
        if ((sIterLexique == sLexique) && (sIterUnit != ""))
        {
        	if ((NSCV*) NULL == pNSCV)
          {
        		pNSCV = new NSCV(pNSFct->getItem()->_pBigBoss->pContexte->getSuperviseur()) ;
          	DBIResult Resultat = pNSCV->open() ;
            if (Resultat != DBIERR_NONE)
            {
            	erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat) ;
            	delete pNSCV ;
              return ;
            }
        	}
          string sValeur ;
          double dVal = StringToDouble(sValue) ;
          if (pNSCV->ConvertirUnite(&dVal, sIterUnit, sUnit, sRefValeur))
          	sValeur = DoubleToString(&dVal, 0, 2) ;
          else
          	sValeur = "" ;

          if ((pFilsCurrent->getItemTransfertMsg()->GetComplement() != "") && (sValeur != ""))
          	ActiveChampEdit(*iter, sValeur) ;
        }
      }
		}
	}
	if (pNSCV)
  {
  	pNSCV->close() ;
		delete pNSCV ;
  }
}
catch (...)
{
	erreur("Exception SynchroNumValues.", standardError, 0) ;
	return ;
}
}

void
RemplirDateMax(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()) || (NULL == pNSFct->getControl()->getTransfert()))
		return ;

	string sDateMax = "" ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;

	NSTransferInfo*	pTrans = pNSFct->getControl()->getTransfert() ;
	if (NULL == pTrans->getFilsItem())
		return ;

	BBFilsItem* pFilsCurrent = pTrans->getFilsItem() ;

	// on remonte au BBFilsItem pere pour trouver l'étiquette
	BBItem*	pItemPere = pFilsCurrent->getItemFather() ;
	if (NULL == pItemPere)
		return ;

	BBFilsItem* pFilsValue = pItemPere->_pBBFilsPere ;
	if (NULL == pFilsValue)
		return ;

	string sRefValeur = pFilsValue->getItemLabel() ;
	// on rajoute le chemin de la date
	sRefValeur += string("/KDARE1/2DA021/£T0;10");

	if (pFilsCurrent->getItemTransfertMsg()->GetComplement() != "")
	{
  	// on récupère la date max
    RetournerDateMax(pItem, &sDateMax) ;

    string sCurrentDate ;
    BBFilsItem* fils = RetournerValeurEdit(pItem, sRefValeur, &sCurrentDate, true) ;

    if (sDateMax == "")
    {
    	NVLdVTemps tDateJour ;
      tDateJour.takeTime() ;
      sDateMax = tDateJour.donneDateHeure() ;
    }

    ActiveChampEdit(fils, sDateMax) ;
  }
}

//-------------------------------------------------------------------
// Renseigner la durée de la période de temps
//-------------------------------------------------------------------
void
AdaptLastingTime(NSDlgFonction *pNSFct)
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
    return ;
  NSControleVector* pNSCtrl = pNSFct->getControl()->getInterfaceControls() ;
  if ((NULL == pNSCtrl) || (pNSCtrl->empty()))
    return ;

  //
  // On recherche un contrôle, situé après l'actuel et de fonction LASTING_TIME
  // Looking for a control located after this one and whose function is LASTING_TIME
  //
  NSControle* pLastingCtrl = RechercheFctApres(pNSCtrl, pNSFct, "LASTING_TIME") ;
  if (NULL == pLastingCtrl)
    pLastingCtrl = RechercheFctApres(pNSCtrl, pNSFct, "LASTING_TIME_FORCE") ;

  NSControle* pNoEndingCtrl = RechercheFctApres(pNSCtrl, pNSFct, "NOENDING") ;
  if ((NULL == pLastingCtrl) && (NULL == pNoEndingCtrl))
    return ;

  if ((NULL == pNSFct->getControl()->getTransfert()) || (NULL == pNSFct->getControl()->getTransfert()->getTransfertMessage()))
    return ;
  string sDate = pNSFct->getControl()->getTransfert()->getTransfertMessage()->GetComplement() ;

  // Si on est vide, mettre à zéro pLastingCtrl
  // If we are empty, reset pLastingCtrl
  //
  if (string("") == sDate)
  {
    if (pLastingCtrl && (pLastingCtrl->getType() == isEdit))
      (static_cast<NSEdit*>(pLastingCtrl->getControle()))->SetText("") ;
  }

  // Est-on un début ou une fin ? chercher la fonction inverse
  // Are we a time beginning or a time ending ? looking for the opposite function

  if ((pNSFct->containFonction("BEGINNOW")) || (pNSFct->containFonction("BEGIN")))
  {
    if ((NULL == pLastingCtrl) || (string("") == sDate))
      return ;
    NSControle* pEndingCtrl = RechercheFctApres(pNSCtrl, pNSFct, "ENDING") ;
    if ((NULL == pEndingCtrl) || (NULL == pEndingCtrl->getTransfert()) || (NULL == pEndingCtrl->getTransfert()->getTransfertMessage()))
      return ;
    string sEndingDate = pEndingCtrl->getTransfert()->getTransfertMessage()->GetComplement() ;
    if (string("") == sEndingDate)
      return ;

    NVLdVTemps tpsBegin, tpsEnding ;
    tpsBegin.initFromDate(sDate) ;
    tpsEnding.initFromDate(sEndingDate) ;

    unsigned long lDeltaDays = tpsEnding.daysBetween(tpsBegin) + 1 ;

    if (pLastingCtrl->getType() == isEdit)
    {
      char szDelta[35] ;
      (static_cast<NSEdit*>(pLastingCtrl->getControle()))->SetText(ltoa((long)lDeltaDays, szDelta, 10)) ;
    }
    return ;
  }

  if (pNSFct->containFonction("ENDING"))
  {
    if (pNoEndingCtrl)
    {
      if (string("") == sDate)
      {
        if      (pNoEndingCtrl->getType() == isCaseACocher)
          (static_cast<NSCheckBox*>(pLastingCtrl->getControle()))->SetCheck(BF_CHECKED) ;
        else if (pNoEndingCtrl->getType() == isRadioBtn)
          (static_cast<NSRadioButton*>(pLastingCtrl->getControle()))->SetCheck(BF_CHECKED) ;
      }
      else
      {
        if      (pNoEndingCtrl->getType() == isCaseACocher)
          (static_cast<NSCheckBox*>(pLastingCtrl->getControle()))->SetCheck(BF_UNCHECKED) ;
        else if (pNoEndingCtrl->getType() == isRadioBtn)
          (static_cast<NSRadioButton*>(pLastingCtrl->getControle()))->SetCheck(BF_UNCHECKED) ;
      }
    }

    if ((NULL == pLastingCtrl) || (string("") == sDate))
      return ;

    NSControle* pBeginCtrl = RechercheFctAvant(pNSCtrl, pNSFct, "BEGINNOW") ;
    if (NULL == pBeginCtrl)
      pBeginCtrl = RechercheFctAvant(pNSCtrl, pNSFct, "BEGIN") ;
    if ((NULL == pBeginCtrl) || (NULL == pBeginCtrl->getTransfert()) || (NULL == pBeginCtrl->getTransfert()->getTransfertMessage()))
      return ;
    string sBeginDate = pBeginCtrl->getTransfert()->getTransfertMessage()->GetComplement() ;
    if (string("") == sBeginDate)
      return ;

    NVLdVTemps tpsBegin, tpsEnding ;
    tpsBegin.initFromDate(sBeginDate) ;
    tpsEnding.initFromDate(sDate) ;

    unsigned long lDeltaDays = tpsEnding.daysBetween(tpsBegin) + 1 ;

    if (pLastingCtrl->getType() == isEdit)
    {
      char szDelta[35] ;
      (static_cast<NSEdit*>(pLastingCtrl->getControle()))->SetText(ltoa((long)lDeltaDays, szDelta, 10)) ;
    }
    return ;
  }
}

void
AdaptEnding(NSDlgFonction *pNSFct, bool bForceEnding)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	NSControleVector* pNSCtrl = pNSFct->getControl()->getInterfaceControls() ;
  if ((NULL == pNSCtrl) || (pNSCtrl->empty()))
    return ;

	string sNbDays = string("") ;

	if (pNSFct->getControl()->getTransfert())
		sNbDays = pNSFct->getControl()->getTransfert()->getTransfertMessage()->GetComplement() ;
	else
	{
		if (pNSFct->getControl()->getType() == isEdit)
		{
      NSEdit* pEdit = static_cast<NSEdit*>(pNSFct->getControl()->getControle()) ;
      if (pEdit)
        sNbDays = pEdit->GetText() ;
    }
  }
	if (string("") == sNbDays)
		return ;

	int iNbDays = atoi(sNbDays.c_str()) ;
	if (iNbDays <= 0)
		return ;

	NSControle* pBeginCtrl = RechercheFctAvant(pNSCtrl, pNSFct, "BEGINNOW") ;
	if (!pBeginCtrl)
		pBeginCtrl = RechercheFctAvant(pNSCtrl, pNSFct, "BEGIN") ;
	if (!pBeginCtrl)
		return ;
	int iBeginCtrlType = pBeginCtrl->getType() ;
  if ((iBeginCtrlType != isEditDate) && (iBeginCtrlType != isEditDateHeure))
		return ;

	NSControle* pEndingCtrl = RechercheFctAvant(pNSCtrl, pNSFct, "ENDING") ;
	if (!pEndingCtrl)
		return ;
	int iEndingCtrlType = pEndingCtrl->getType() ;
  if ((iEndingCtrlType != isEditDate) && (iEndingCtrlType != isEditDateHeure))
		return ;

	string sEndingDate = pEndingCtrl->getTransfert()->getTransfertMessage()->GetComplement() ;
	string sBeginDate  = pBeginCtrl->getTransfert()->getTransfertMessage()->GetComplement() ;
	if ((sEndingDate == "") && (sBeginDate == ""))
		return ;

	if ((sEndingDate == "") ||
      ((true == bForceEnding) && (string("") != sBeginDate)))
	{
		NVLdVTemps tpsEnding ;
		tpsEnding.initFromDate(sBeginDate) ;
		tpsEnding.ajouteJours(iNbDays - 1) ;
    if      (iEndingCtrlType == isEditDate)
    	sEndingDate = tpsEnding.donneDate() ;
    else if (iEndingCtrlType == isEditDateHeure)
    	sEndingDate = tpsEnding.donneDateHeure() ;
		ActiveChampEdit(pEndingCtrl->getTransfert()->getFilsItem(), sEndingDate) ;
		return ;
	}
  if (sBeginDate == "")
  {
  	NVLdVTemps tpsBegin ;
    tpsBegin.initFromDate(sEndingDate) ;
    tpsBegin.ajouteJours(1 - iNbDays) ;
    if      (iBeginCtrlType == isEditDate)
    	sBeginDate = tpsBegin.donneDate() ;
    else if (iBeginCtrlType == isEditDateHeure)
    	sBeginDate = tpsBegin.donneDateHeure() ;
    ActiveChampEdit(pBeginCtrl->getTransfert()->getFilsItem(), sBeginDate) ;
    return ;
  }
}

void
FixLastingTime(NSDlgFonction* /*pNSFct*/)
{
}

void
FixNoEnding(NSDlgFonction* /*pNSFct*/)
{
}

void
CancelEnding(NSDlgFonction* /*pNSFct*/)
{
}

void FixCaption(NSDlgFonction *pNSFct)
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
    return ;

  BBItem* pBBItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pBBItem)
    return ;

  string sEtiq = "" ;

  if (pBBItem->_pBBFilsPere)
    sEtiq = pBBItem->_pBBFilsPere->getItemLabel() ;
  else
  {
    if (pBBItem->getParseur())
    {
      Citem* pRootItem = pBBItem->getArchetypeRootItem() ;
      if (NULL == pRootItem)
        return ;

      Citem* pFirstSonItem = pRootItem->getFirstSon() ;
      if (NULL == pFirstSonItem)
        return ;

      sEtiq = pFirstSonItem->getCode() ;
    }
    else
      return ;
  }

  string sTitre = "" ;

  NSContexte *pContexte = pBBItem->_pBigBoss->pContexte ;

  string sLang = pContexte->getUserLanguage() ;

  if (sEtiq != string("£?????"))
  {
    pContexte->getDico()->donneLibelle(sLang, &sEtiq, &sTitre) ;
    sTitre[0] = pseumaj(sTitre[0]) ;
  }

  TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;
  pInterface->SetCaption(sTitre.c_str()) ;
}

void
CreatePreoccup(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()) || !(pNSFct->getControl()->getTransfert()))
		return ;

	NSTransferInfo*	pTrans = pNSFct->getControl()->getTransfert() ;
	if (NULL == pTrans->getFilsItem())
  	return ;

  BBFilsItem* pFilsCurrent = pTrans->getFilsItem() ;

  string sDate = "" ;
  string sPreoccup = "" ;

  NSSuper* pSuper = pFilsCurrent->getItemFather()->_pBigBoss->pContexte->getSuperviseur() ;
	//
	// Est-ce une étiquette multiple (date) ?
	//
	size_t posit = (pFilsCurrent->getItemLabel()).find(string(1, cheminSeparationMARK));
  if (posit != NPOS)
	{
		string sUnit = string(pFilsCurrent->getItemLabel(), 0, posit) ;
		size_t posit_debut = posit + 1 ;
		string sLexique = string(pFilsCurrent->getItemLabel(), posit_debut, strlen(pFilsCurrent->getItemLabel().c_str()) - posit_debut) ;

  	string sValue = pTrans->getTransfertMessage()->GetComplement() ;

		BBItem*	pItemPere = pFilsCurrent->getItemFather() ;
  	if (NULL == pItemPere)
			return ;

		BBFilsItem* pFilsValue = pItemPere->_pBBFilsPere ;
  	string sRefValeur = string("") ;
  	if (NULL == pFilsValue)
			return ;

		sRefValeur      = pFilsValue->getItemLabel() ;
    string sValSens = "" ;

    NSDico::donneCodeSens(&sRefValeur, &sValSens) ;

    if (sValSens != "KOUVR")
    	return ;

		sDate = sValue ;
    if (sDate == "")
    	return ;

    BBItem*	pItemPreoccup = pFilsValue->getItemFather() ;
  	if (NULL == pItemPreoccup)
			return ;

  	if (NULL == pItemPreoccup->_pBBFilsPere)
			return ;

		sPreoccup = pItemPreoccup->_pBBFilsPere->getItemLabel() ;
	}
	else
		sPreoccup = pFilsCurrent->getItemLabel() ;

	// NSDialog* pDialog = pNSFct->getControl()->getNSDialog() ;
  TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;

	NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("NewPreoccup")) ;
  pTask->setPointer1((void*) pInterface, false) ;
	pTask->setParam1(sPreoccup) ;
	pTask->setParam2(sDate) ;

	pTask->sendMe(pSuper) ;
}

void
EditAddress(NSDlgFonction *pNSFct)
{
try
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;
  BBItem* pBBItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pBBItem)
  	return ;

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pBBItem->_pBigBoss->pContexte ;
  string sObjectID ;
  BBFilsItem* pBBFilsItem ;

  // La fonction, rattachée à l'item £OBJE1, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // l'identifiant de l'objet à éditer.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
  {
    sObjectID   = pTransfert->getTransfertMessage()->GetComplement() ;
    pBBFilsItem = pTransfert->getFilsItem() ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  NSAddressGraphManager* pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur());
  NSPatPathoArray* pPatPathoAdr = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject);

  // Si on avait déjà un objet adresse, on reload son graphe
  if (sObjectID != "")
  {
    if (!pAddressManager->getGraphAdr(sObjectID, pPatPathoAdr))
    {
      erreur("Impossible de récupérer le graphe dans la fonction d'édition d'adresse.", standardError, 0) ;
      delete pPatPathoAdr ;
      delete pAddressManager ;
      return ;
    }

    // Préparation de la fonction d'énumération
    WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
    string sTitre, sAdresse ;

    sAdresse = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
    sTitre = string("Domicilié|") + sAdresse ;
    // On passe comme paramètre de EnumChildWindows le titre et l'adresse
    TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;
    EnumChildWindows(pInterface->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
    pInterface->Invalidate() ;

    // on active le bouton "modifier", qui contient l'objet
    // pour qu'il soit pris en compte au retour de la fonction de modification
    pTransfert->getFilsItem()->Active() ;

    // dans cette fonction, on ne lance pas l'archetype en modification
    // (cela est réalisé par la fonction ModifyAddress())
    // delete pPatPathoAdr ;
    // delete pGraphManager ;
    // return ;
  }

  // on initialise un pBigBoss pour pouvoir lancer l'archetype de gestion d'adresse
  NSSmallBrother* pBigBoss = new NSSmallBrother(pContexte, pPatPathoAdr) ;
  pBigBoss->setMotherWindow(pNSFct->getControl()->getInterfacePointer()) ;
  // Ici on lance une boite de dialogue modale
	BB1BBInterfaceForKs InterfaceForKs(-1, pContexte->getSuperviseur()->getAddressArchetypeId(), "", false) ;
  pBigBoss->lanceBbkArchetypeInDialog(pContexte->getSuperviseur()->getAddressArchetypeId(), 0, pBBFilsItem, &InterfaceForKs, true) ;

  // on teste le code de retour du dialogue, qui est stocké dans le
  // BBItem root créé par le pBigBoss.
  if (0 == pBigBoss->getBBItem()->_iRetourDlg)     // CmOK
  {
    // on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
    // si elle n'est pas vide.
    if (false == pBigBoss->getPatPatho()->empty())
    {
      *pPatPathoAdr = *(pBigBoss->getPatPatho()) ;

      /*
      if (pGraphManager->setGraphAdr(pPatPathoAdr))
      {
        // Préparation de la fonction d'énumération
        WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
        string sTitre, sAdresse ;

        sAdresse = pGraphManager->trouveLibCourtAdr() ;
        sTitre = string("Domicilié|") + sAdresse ;
        // On passe comme paramètre de EnumChildWindows le titre et l'adresse
        EnumChildWindows(pDialog->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
        pDialog->Invalidate() ;

        // on récupère l'objectID root du graphe pour
        // le stocker dans le complément de l'item £OBJE1
        sObjectID = pGraphManager->getRootObject() ;
        pTransfert->pTransfertMessage->SetComplement(sObjectID) ;
        pTransfert->pBBFilsItem->Active() ;
#ifdef N_TIERS
				pDialog->CmOk() ;
#endif
      }
      */
    }
  }

  delete pPatPathoAdr ;
  delete pBigBoss ;
  delete pAddressManager ;
}
catch (...)
{
	erreur("Exception EditAddress.", standardError, 0) ;
	return ;
}
}

void
ModifyAddress(NSDlgFonction *pNSFct)
{
try
{
  if (!pNSFct || !(pNSFct->getControl()))
  	return ;
  BBItem* pBBItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pBBItem)
  	return ;

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pBBItem->_pBigBoss->pContexte ;
  string sObjectID ;
  BBFilsItem* pBBFilsItem ;
  bool bExistObject = false;

  // La fonction, rattachée à l'item £OBJE1, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // l'identifiant de l'objet à éditer.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
  {
    sObjectID   = pTransfert->getTransfertMessage()->GetComplement() ;
    pBBFilsItem = pTransfert->getFilsItem() ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  NSAddressGraphManager* pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  NSPatPathoArray* pPatPathoAdr = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject) ;

  // Si on avait déjà un objet adresse, on reload son graphe
  if (sObjectID != "")
  {
    if (!pAddressManager->getGraphAdr(sObjectID, pPatPathoAdr))
    {
      erreur("Impossible de récupérer le graphe dans la fonction d'édition d'adresse.", standardError, 0) ;
      delete pPatPathoAdr ;
      delete pAddressManager ;
      return ;
    }

    bExistObject = true;

    // Préparation de la fonction d'énumération
    WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
    string sTitre, sAdresse ;

    sAdresse = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
    sTitre = string("Domicilié|") + sAdresse ;
    // On passe comme paramètre de EnumChildWindows le titre et l'adresse
    TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;
    EnumChildWindows(pInterface->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
    pInterface->Invalidate() ;

    // on active le bouton "modifier", qui contient l'objet
    // pour qu'il soit pris en compte au retour de la fonction de modification
    pTransfert->getFilsItem()->Active() ;

    /*
    // dans cette fonction, on ne lance pas l'archetype en modification
    // quand il existe un objet, on revient sur l'archetype intermédiaire
    // à partir duquel on peut corriger ou changer d'adresse.
    delete pPatPathoAdr ;
    delete pAddressManager ;
    return ;
    */
  }

  TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;

  // on initialise un pBigBoss pour pouvoir lancer l'archetype de gestion d'adresse
  NSSmallBrother* pBigBoss = new NSSmallBrother(pContexte, pPatPathoAdr) ;
  pBigBoss->setMotherWindow(pInterface) ;
  // Ici on lance une boite de dialogue modale
	BB1BBInterfaceForKs InterfaceForKs(-1, pContexte->getSuperviseur()->getAddressArchetypeId(), "", false) ;
  do
  {
    pBigBoss->lanceBbkArchetypeInDialog(pContexte->getSuperviseur()->getAddressArchetypeId(), 0, pBBFilsItem, &InterfaceForKs, true) ;
  }
  while (NSDLGSTATUS_REINIT == pBigBoss->getBBItem()->_iStatusDlg) ;

  // on teste le code de retour du dialogue, qui est stocké dans le
  // BBItem root créé par le pBigBoss.
  if (0 == pBigBoss->getBBItem()->_iRetourDlg)     // CmOK
  {
    // on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
    // si elle n'est pas vide.
    if (!(pBigBoss->getPatPatho()->empty()))
    {
      *pPatPathoAdr = *(pBigBoss->getPatPatho()) ;
      // on remet le graphe à zéro pour éviter la formation de doublons issus d'un merge
      pAddressManager->graphReset() ;
      if (pAddressManager->setGraphAdr(pContexte, pPatPathoAdr, pContexte->getUserLanguage()))
      {
        // Préparation de la fonction d'énumération
        WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
        string sTitre, sAdresse ;

        sAdresse = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
        sTitre = string("Domicilié|") + sAdresse ;
        // On passe comme paramètre de EnumChildWindows le titre et l'adresse
        TWindow* pInterface = pNSFct->getControl()->getInterfacePointer() ;
        EnumChildWindows(pInterface->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
        pInterface->Invalidate() ;

        // on récupère l'objectID root du graphe pour
        // le stocker dans le complément de l'item £OBJE1
        sObjectID = pAddressManager->getRootObject() ;
      }
    }
  }

  if (sObjectID != "")
  {
    pTransfert->getTransfertMessage()->SetComplement(sObjectID) ;
    pTransfert->getFilsItem()->Active() ;
    if (!bExistObject)
    {
      TDialog* pDlg = TYPESAFE_DOWNCAST(pInterface, TDialog) ;
      if (pDlg)
        pDlg->CmOk() ;
    }
  }
  else
  {
    TDialog* pDlg = TYPESAFE_DOWNCAST(pInterface, TDialog) ;
    if (pDlg)
  	  pDlg->CmCancel() ;
  }

  delete pPatPathoAdr ;
  delete pBigBoss ;
  delete pAddressManager ;
}
catch (...)
{
	erreur("Exception ModifyAddress.", standardError, 0) ;
	return ;
}
}


void
EditAddressButton(NSDlgFonction *pNSFct)
{
try
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;

  NSDialog* pDialog = pNSFct->getControl()->getNSDialog() ;
  if (NULL == pDialog)
  	return ;

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pDialog->pContexte ;
  string sObjectID ;
  BBFilsItem* pBBFilsItem ;

  // La fonction, rattachée à l'item £OBJE1, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // l'identifiant de l'objet à éditer.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
  {
    sObjectID   = pTransfert->getTransfertMessage()->GetComplement() ;
    pBBFilsItem = pTransfert->getFilsItem() ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  NSAddressGraphManager* pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  NSPatPathoArray* pPatPathoAdr = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject) ;

  // Si on avait déjà un objet adresse, on reload son graphe
  if (sObjectID != "")
  {
    if (!pAddressManager->getGraphAdr(sObjectID, pPatPathoAdr))
    {
      erreur("Impossible de récupérer le graphe dans la fonction d'édition d'adresse.", standardError, 0) ;
      delete pPatPathoAdr ;
      delete pAddressManager ;
      return ;
    }

    /*
    // Préparation de la fonction d'énumération
    WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
    string sTitre, sAdresse ;

    sAdresse = pGraphManager->trouveLibCourtAdr();
    sTitre = string("Domicilié|") + sAdresse ;
    // On passe comme paramètre de EnumChildWindows le titre et l'adresse
    EnumChildWindows(pDialog->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
    pDialog->Invalidate() ;
    */

    // on active le bouton "modifier", qui contient l'objet
    // pour qu'il soit pris en compte au retour de la fonction de modification
    // (même si on sort par CmCancel() de l'archetype)
    // pTransfert->pBBFilsItem->Active() ;
  }

  // on initialise un pBigBoss pour pouvoir lancer l'archetype de gestion d'adresse
  NSSmallBrother* pBigBoss = new NSSmallBrother(pContexte, pPatPathoAdr) ;
  pBigBoss->setMotherWindow((TWindow*) pDialog) ;
  // Ici on lance une boite de dialogue modale
	BB1BBInterfaceForKs InterfaceForKs(-1, pContexte->getSuperviseur()->getAddressArchetypeId(), "", false) ;
  pBigBoss->lanceBbkArchetypeInDialog(pContexte->getSuperviseur()->getAddressArchetypeId(), 0, pBBFilsItem, &InterfaceForKs, true) ;

  // on teste le code de retour du dialogue, qui est stocké dans le
  // BBItem root créé par le pBigBoss.
  if (0 == pBigBoss->getBBItem()->_iRetourDlg)     // CmOK
  {
    // on enregistre la patpatho du pBigBoss, qui contient les nouvelles données
    // si elle n'est pas vide.
    if (false == pBigBoss->getPatPatho()->empty())
    {
      *pPatPathoAdr = *(pBigBoss->getPatPatho()) ;
      // on remet le graphe à zéro pour éviter la formation de doublons issus d'un merge
      pAddressManager->graphReset() ;
      if (pAddressManager->setGraphAdr(pContexte, pPatPathoAdr, pContexte->getUserLanguage()))
      {
        // Préparation de la fonction d'énumération
        WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
        string sTitre, sAdresse ;

        sAdresse = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
        sTitre = string("Adresse|") + sAdresse ;
        // On passe comme paramètre de EnumChildWindows le titre et l'adresse
        EnumChildWindows(pDialog->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
        pDialog->Invalidate() ;

        // on récupère l'objectID root du graphe pour
        // le stocker dans le complément de l'item £OBJE1
        sObjectID = pAddressManager->getRootObject() ;
        pTransfert->getTransfertMessage()->SetComplement(sObjectID) ;
      }
    }
  }

  delete pPatPathoAdr ;
  delete pBigBoss ;
  delete pAddressManager ;
}
catch (...)
{
	erreur("Exception EditAddressButton.", standardError, 0) ;
	return ;
}
}


void ChooseAddress(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception ChooseAddress.", standardError, 0) ;
	return ;
}
}

void ChoosePatAddress(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception ChoosePatAddress.", standardError, 0) ;
	return ;
}
}

void ChooseCorAddress(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception ChooseCorAddress.", standardError, 0) ;
	return ;
}
}

void ShowCorresp(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception ShowCorresp.", standardError, 0) ;
	return ;
}
}

void EditCorresp(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception EditCorresp.", standardError, 0) ;
	return ;
}
}

void ShowRefCorresp(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception ShowRefCorresp.", standardError, 0) ;
	return ;
}
}

void EditRefCorresp(NSDlgFonction* /* pNSFct */)
{
try
{
}
catch (...)
{
	erreur("Exception EditRefCorresp.", standardError, 0) ;
	return ;
}
}

void
PidsAdrSite(NSDlgFonction *pNSFct)
{
try
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;
  NSDialog* pDialog = pNSFct->getControl()->getNSDialog() ;
  if (NULL == pDialog)
  	return ;

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pDialog->pContexte ;
  string sSearch ;
  BBFilsItem* pBBFilsItem ;

  // La fonction, rattachée à l'item Nom du SITE, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // la valeur du champ Edit pour la recherche.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
  {
    sSearch     = pTransfert->getTransfertMessage()->GetTexteLibre() ;
    pBBFilsItem = pTransfert->getFilsItem() ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  if (sSearch == "")
  	return ;

  NSAddressGraphManager* pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  NSPatPathoArray* pPatPathoAdr = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject) ;
  NSVectPatPathoArray* pVectAdr = new NSVectPatPathoArray() ;
  VecteurString* pFoundObjects = new VecteurString() ;
  VecteurString* pLibelles = new VecteurString() ;
  string sLibelle ;

  NSDataGraph* pGraph = new NSDataGraph(pContexte->getSuperviseur(), graphObject) ;
  NSPersonsAttributesArray* pObjList = new NSPersonsAttributesArray() ;
  NSBasicAttributeArray *pAttrList = new NSBasicAttributeArray() ;
  pAttrList->push_back(new NSBasicAttribute("_LSITE_LSITE", strpids(sSearch))) ;
  pAttrList->push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

  bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), pObjList, pAttrList) ;
  if (!res)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    delete pGraph ;
		delete pObjList ;
    delete pAttrList ;
    delete pAddressManager ;
  	delete pPatPathoAdr ;
  	delete pVectAdr ;
  	delete pFoundObjects ;
  	delete pLibelles ;
    return ;
  }

	if (false == pObjList->empty())
  	for (NSPersonsAttributeIter k = pObjList->begin() ; pObjList->end() != k ; k++)
  	{
    	string sOIDS = (*k)->getAttributeValue(OIDS) ;
    	pFoundObjects->AddString(sOIDS) ;
  	}

  delete pAttrList ;
  delete pObjList ;
  delete pGraph ;

  // Si aucun objet trouvé : on sort
  if (pFoundObjects->empty())
	{
  	delete pAddressManager ;
  	delete pPatPathoAdr ;
  	delete pVectAdr ;
  	delete pFoundObjects ;
  	delete pLibelles ;
  	return ;
	}

  for (EquiItemIter i = pFoundObjects->begin(); i != pFoundObjects->end(); i++)
  {
    // on récupère le graphe de l'objet pointé
    // et on conserve son libellé.
    pAddressManager->graphReset() ;
    pPatPathoAdr->vider() ;
    pAddressManager->getGraphAdr((*(*i)), pPatPathoAdr) ;
    sLibelle = pAddressManager->trouveLibCourtAdr(pContexte, pContexte->getUserLanguage()) ;
    pLibelles->AddString(sLibelle) ;
    pVectAdr->push_back(new NSPatPathoArray(*pPatPathoAdr)) ;
  }

  ChoixDansListeDialog* pListeDialog = new ChoixDansListeDialog(pDialog, pContexte, pLibelles, sSearch) ;
  if ((pListeDialog->Execute() == IDCANCEL) || (pListeDialog->itemChoisi == -1))
  {
  	delete pAddressManager ;
  	delete pPatPathoAdr ;
  	delete pVectAdr ;
  	delete pFoundObjects ;
  	delete pLibelles ;
  	return ;
	}

  // on instancie la pPPTMerge qui fusionne avec pPPTEnCours de BBItem sur okFermerDialogue()
  BBItem* pBBItemRoot = pDialog->_pBBItem ;
  int index = pListeDialog->itemChoisi ;
  pBBItemRoot->_pPPTMerge = new NSPatPathoArray(*((*pVectAdr)[index])) ;
  // on lance la réouverture du dialogue
  pBBItemRoot->donneStatusDlg(NSDLGSTATUS_REINIT);
  pDialog->CmOk() ;

  /*
  // on reconstitue un graphe à partir de la patpatho résultat (dans pBBItemRoot)
  pAddressManager->graphReset() ;
  pAddressManager->setGraphAdr(pBBItemRoot->pPPTEnCours) ;

  // Préparation de la fonction d'énumération
  WNDENUMPROC lpEnumFunc = (WNDENUMPROC) MakeProcInstance((FARPROC) SetStaticCaption, hInstance) ;
  string sTitre, sAdresse, sObjectID ;

  sAdresse = pGraphManager->trouveLibCourtAdr() ;
  sTitre = string("Domicilié|") + sAdresse ;
  sObjectID = pAddressManager->getRootObject() ;
  // On passe comme paramètre de EnumChildWindows le titre et l'adresse
  EnumChildWindows(pDialogSource->HWindow, lpEnumFunc, LPARAM(&sTitre)) ;
  pDialogSource->Invalidate() ;

  // on remet l'objectID à sa place à l'aide du pTransfert de la boite précédente
  // et on réactive l'archetype.
  pBBItemRoot->pBBFilsPere->pTransfert->pTransfertMessage->SetComplement(sObjectID) ;
  pBBItemRoot->pBBFilsPere->pTransfert->pBBFilsItem->Active() ;
  */

  delete pAddressManager ;
  delete pPatPathoAdr ;
  delete pVectAdr ;
  delete pFoundObjects ;
  delete pLibelles ;
}
catch (...)
{
	erreur("Exception PidsAdrSite.", standardError, 0) ;
	return ;
}
}


void
PidsAdrVoie(NSDlgFonction *pNSFct)
{
try
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;

  NSDialog* pDialog = pNSFct->getControl()->getNSDialog() ;
  if (NULL == pDialog)
  	return ;

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pDialog->pContexte ;
  string sSearch ;
  BBFilsItem* pBBFilsItem ;

  // La fonction, rattachée à l'item Nom du SITE, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // la valeur du champ Edit pour la recherche.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
  {
    sSearch     = pTransfert->getTransfertMessage()->GetTexteLibre() ;
    pBBFilsItem = pTransfert->getFilsItem() ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  if (sSearch == "")
  	return ;

  NSAddressGraphManager* pAddressManager = new NSAddressGraphManager(pContexte->getSuperviseur()) ;
  NSPatPathoArray* pPatPathoAdr = new NSPatPathoArray(pContexte->getSuperviseur(), graphObject) ;
  NSVectPatPathoArray* pVectAdr = new NSVectPatPathoArray() ;
  VecteurString* pFoundObjects = new VecteurString() ;
  VecteurString* pLibelles = new VecteurString() ;
  string sLibelle ;

  NSDataGraph* pGraph = new NSDataGraph(pContexte->getSuperviseur(), graphObject) ;
  NSPersonsAttributesArray* pObjList = new NSPersonsAttributesArray() ;
  NSBasicAttributeArray *pAttrList = new NSBasicAttributeArray() ;
  pAttrList->push_back(new NSBasicAttribute("_LVOIE_LVOIE", strpids(sSearch))) ;
  pAttrList->push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

  bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), pObjList, pAttrList) ;
  if (!res)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    delete pAttrList ;
  	delete pObjList ;
  	delete pGraph ;
    delete pAddressManager ;
  	delete pPatPathoAdr ;
  	delete pVectAdr ;
  	delete pFoundObjects ;
  	delete pLibelles ;
    return ;
  }

	if (false == pObjList->empty())
  	for (NSPersonsAttributeIter k = pObjList->begin() ; k != pObjList->end() ; k++)
  	{
    	string sOIDS = (*k)->getAttributeValue(OIDS) ;
    	pFoundObjects->AddString(sOIDS) ;
  	}

  delete pAttrList ;
  delete pObjList ;
  delete pGraph ;

  // Si aucun objet trouvé : on sort
  if (pFoundObjects->empty())
	{
  	delete pAddressManager ;
  	delete pPatPathoAdr ;
  	delete pVectAdr ;
  	delete pFoundObjects ;
  	delete pLibelles ;
  	return ;
	}

  for (EquiItemIter i = pFoundObjects->begin(); i != pFoundObjects->end(); i++)
  {
    // on récupère le graphe de l'objet pointé
    // et on conserve son libellé.
    pAddressManager->graphReset() ;
    pPatPathoAdr->vider() ;
    pAddressManager->getGraphAdr((*(*i)), pPatPathoAdr) ;
    pAddressManager->trouveInfoVoieAdr(pContexte, sLibelle, pContexte->getUserLanguage()) ;
    string sVille, sPays, sZip, sCedex ;
    pAddressManager->trouveInfoVilleAdr(sVille, sPays, sZip, sCedex) ;
    sLibelle += string(" ") + sZip + string(" ") + sVille ;
    pLibelles->AddString(sLibelle) ;
    pVectAdr->push_back(new NSPatPathoArray(*pPatPathoAdr)) ;
  }

  ChoixDansListeDialog* pListeDialog = new ChoixDansListeDialog(pDialog, pContexte, pLibelles, sSearch) ;
  if ((pListeDialog->Execute() == IDCANCEL) || (pListeDialog->itemChoisi == -1))
  	return ;

  // on instancie la pPPTMerge qui fusionne avec pPPTEnCours de BBItem sur okFermerDialogue()
  BBItem* pBBItemRoot = pDialog->_pBBItem ;
  int index = pListeDialog->itemChoisi ;
  pBBItemRoot->_pPPTMerge = new NSPatPathoArray(*((*pVectAdr)[index])) ;
  // on lance la réouverture du dialogue
  pBBItemRoot->donneStatusDlg(NSDLGSTATUS_REINIT);
  pDialog->CmOk() ;

  delete pAddressManager ;
  delete pPatPathoAdr ;
  delete pVectAdr ;
  delete pFoundObjects ;
  delete pLibelles ;
}
catch (...)
{
	erreur("Exception PidsAdrVoie.", standardError, 0) ;
	return ;
}
}

bool
getCityFromZip(string sZipCode, NSPatPathoArray* pCityPpt, NSDialog* pMotherDialog, NSContexte* pContexte)
{
	if ((string("") == sZipCode) || (NULL == pCityPpt) || (NULL == pMotherDialog) || (NULL == pContexte))
		return false ;

  NSPersonsAttributesArray ObjList ;
  NSBasicAttributeArray    AttrList ;
  AttrList.push_back(new NSBasicAttribute("_LVILL_LVILL", strpids(sZipCode))) ;
  AttrList.push_back(new NSBasicAttribute(RESEARCH, CONTAIN_RESEARCH)) ;

  bool res = pContexte->getSuperviseur()->getPilot()->objectList(NautilusPilot::SERV_OBJECT_LIST.c_str(), &ObjList, &AttrList) ;
  if (!res)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("NTIERS", "objectNotFound") ;
    erreur(sErrorText.c_str(), standardError, 0, 0) ;
    return false ;
  }

  VecteurString FoundObjects ;

  if (!(ObjList.empty()))
  	for (NSPersonsAttributeIter k = ObjList.begin() ; k != ObjList.end() ; k++)
  	{
    	string sOIDS = (*k)->getAttributeValue(OIDS) ;
    	FoundObjects.AddString(sOIDS) ;
  	}

  // Si aucun objet trouvé : on sort
  if (FoundObjects.empty())
		return false ;

	NSAddressGraphManager    AddressManager(pContexte->getSuperviseur()) ;
  NSPatPathoArray          PatPathoAdr(pContexte->getSuperviseur(), graphObject) ;
  NSVectPatPathoArray      VectAdr ;

  VecteurString            Libelles ;

  for (EquiItemIter i = FoundObjects.begin(); i != FoundObjects.end(); i++)
  {
    // on récupère le graphe de l'objet pointé
    // et on conserve son libellé.
    AddressManager.graphReset() ;
    PatPathoAdr.vider() ;
    AddressManager.getGraphAdr((*(*i)), &PatPathoAdr) ;
    string sVille, sPays, sZip, sCedex ;
    AddressManager.trouveInfoVilleAdr(sVille, sPays, sZip, sCedex) ;
    string sLibelle = sZip + string(" ") + sVille + string(" ") + sPays ;
    Libelles.AddString(sLibelle) ;
    VectAdr.push_back(new NSPatPathoArray(PatPathoAdr)) ;
  }

  ChoixDansListeDialog* pListeDialog = new ChoixDansListeDialog(pMotherDialog, pContexte, &Libelles, sZipCode) ;
  if ((pListeDialog->Execute() == IDCANCEL) || (pListeDialog->itemChoisi == -1))
  	return false ;

	int index = pListeDialog->itemChoisi ;

  if ((index >= 0) && (index < int(VectAdr.size())))
  {
  	*pCityPpt = *(VectAdr[index]) ;
    return true ;
	}

  return false ;
}

bool
getCityFromZipTxt(string sZipCode, string &sCityName, TWindow* pMotherDialog, NSContexte* pContexte)
{
  //
  // Load the file
  //
  string sFileName = pContexte->PathName("FPER") + string("zipCodes.txt") ;

  ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
    return false ;

  int iCounter = 0 ;
  int iLoopThreshold = 100 ;

  VecteurString Libelles ;

  string sLine ;
  while (false == inFile.eof())
  {
		getline(inFile, sLine) ;

    size_t iPos = sLine.find(";") ;
    if (NPOS != iPos)
    {
      string sCityName = string(sLine, 0, iPos) ;
      string sCityZip  = string(sLine, iPos+1, strlen(sLine.c_str()) - iPos) ;

      strip(sCityName, stripBoth) ;
      strip(sCityZip, stripBoth) ;

      if (strlen(sCityZip.c_str()) < 5)
        sCityZip = string(5 - strlen(sCityZip.c_str()), '0') + sCityZip ;

      vraiemaj(&sCityName) ;

      if (sZipCode == sCityZip)
        Libelles.AddString(sCityName) ;
    }

    iCounter++ ;
    if (iCounter > iLoopThreshold)
    {
      iCounter = 0 ;
      pContexte->getSuperviseur()->getApplication()->PumpWaitingMessages() ;
    }
  }

  inFile.close() ;

  if (Libelles.empty())
    return false ;

  if (Libelles.size() == 1)
  {
    sCityName = **(Libelles.begin()) ;
    return true ;
  }

  ChoixDansListeDialog* pListeDialog = new ChoixDansListeDialog(pMotherDialog, pContexte, &Libelles, sZipCode) ;
  if ((pListeDialog->Execute() == IDCANCEL) || (-1 == pListeDialog->itemChoisi))
  	return false ;

	int index = pListeDialog->itemChoisi ;
  sCityName = Libelles.GetElementAt(index) ;

  return true ;
}

void
PidsAdrZip(NSDlgFonction * /*pNSFct*/)
{
/*
try
{
  if (NULL == pNSFct || (NULL == pNSFct->getControl()))
  	return ;

  BBItem* pBBItemRoot = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pBBItemRoot)

  // on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  NSContexte* pContexte = pNSFct->getControl()->getInterfaceContext() ;
  string sSearch = string("") ;
  BBFilsItem* pBBFilsItem ;

  // La fonction, rattachée à l'item Nom du SITE, a un pointeur
  // sur le pTransfert associé, ce qui permet de récupérer
  // la valeur du champ Edit pour la recherche.
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->pTransfertMessage)
  {
    sSearch = pTransfert->pTransfertMessage->GetTexteLibre() ;
    pBBFilsItem = pTransfert->pBBFilsItem ;
  }
  else
  {
    erreur("Transfert non initialisé dans la fonction d'édition d'adresse.", standardError, 0) ;
    return ;
  }

  if (sSearch == "")
  	return ;

  NSPatPathoArray PatPathoAdr(pContexte, graphObject) ;
	bool bGotCity = getCityFromZip(sSearch, &PatPathoAdr, pDialog, pContexte) ;

  if (false == bGotCity)
		return ;

  // on instancie la pPPTMerge qui fusionne avec pPPTEnCours de BBItem sur okFermerDialogue()
  //
  pBBItemRoot->pPPTMerge = new NSPatPathoArray(PatPathoAdr) ;

  // on lance la réouverture du dialogue
  //
  // pBBItemRoot->donneStatusDlg(NSDLGSTATUS_REINIT) ;
  // pDialog->CmOk() ;
}
catch (...)
{
	erreur("Exception PidsAdrZip.", standardError, 0) ;
	return ;
}
*/
}

void
ChangeToLowCaps(NSDlgFonction *pNSFct)
{
  if (NULL == pNSFct || (NULL == pNSFct->getControl()))
  	return ;

	// First, get zip code from current interface control
	//
	string sSearch = string("") ;

	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if ((NULL == pTransfert) || (NULL == pTransfert->getTransfertMessage()))
    return ;

  sSearch = pTransfert->getTransfertMessage()->GetTexteLibre() ;

  if (string("") == sSearch)
  	return ;

  sSearch = pseuLow(sSearch) ;

  pTransfert->getTransfertMessage()->SetTexteLibre(sSearch) ;
  pNSFct->getControl()->Transfer(tdSetData) ;
}

void
PidsZipFillCity(NSDlgFonction *pNSFct)
{
try
{
  if (((NSDlgFonction*) NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;

	// First, get zip code from current interface control
	//
	string sSearch = string("") ;

	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (pTransfert && pTransfert->getTransfertMessage())
    sSearch = pTransfert->getTransfertMessage()->GetTexteLibre() ;

  if (string("") == sSearch)
  	return ;

	// Then, find if a "city" interface control is available around
  //

	BBFilsItem* pZipFilsItem = pTransfert->getFilsItem() ;
  if ((NULL == pZipFilsItem->getItemFather()) ||
      (NULL == pZipFilsItem->getItemFather()->_pBBFilsPere) ||
      (NULL == pZipFilsItem->getItemFather()->_pBBFilsPere->getItemFather()))
  	return ;

	string sZipPathway = pNSFct->getControl()->getIdentite() ;
  string sZipLabel   = pZipFilsItem->getItemLabel() ;
  size_t iPos        = sZipPathway.rfind("LZIP01/£CL000") ;
  if (iPos == string::npos)
  	return ;

  string sCityPathway = string(sZipPathway, 0, iPos) + string("LCOMU1/£CL000") ;

	BBItem* pDialogItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if ((BBItem*) NULL == pDialogItem)
		return ;

	string retour ;
	BBFilsItem* pCityFils = RetournerValeurEdit(pDialogItem, sCityPathway, &retour, true) ;
	if (NULL == pCityFils)
		return ;

	// on récupère le contexte du pBigBoss du BBItem root du dialogue
  // d'origine (créé à partir de l'archetype d'édition d'adresse)
  //
  NSContexte* pContexte = pDialogItem->_pBigBoss->pContexte ;

  string sCityName = string("") ;
  bool bGotCity = getCityFromZipTxt(sSearch, sCityName, pNSFct->getControl()->getInterfacePointer(), pContexte) ;
  if ((false == bGotCity) || (string("") == sCityName))
		return ;

  ActiveChampEdit(pCityFils, sCityName) ;
}
catch (...)
{
	erreur("Exception PidsZipFillCity.", standardError, 0) ;
	return ;
}
}

void
SurfOnTheMap(NSDlgFonction *pNSFct)
{
try
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;
  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
  	return ;

  NSContexte* pContexte = pItem->_pBigBoss->pContexte ;

  // BBFilsItem* fils ;

	string sLigne1 ;
	/* fils = */ RetournerValeurEdit(pItem, string("LADL11/£CL000"), &sLigne1, true) ;
	string sLigne2 ;
	/* fils = */ RetournerValeurEdit(pItem, string("LADL21/£CL000"), &sLigne2, true) ;
	string sCP ;
	/* fils = */ RetournerValeurEdit(pItem, string("LVILL1/LZIP01/£CL000"), &sCP, true) ;
	string sVille ;
	/* fils = */ RetournerValeurEdit(pItem, string("LVILL1/LCOMU1/£CL000"), &sVille, true) ;
  string sCountry ;
	/* fils = */ RetournerValeurEdit(pItem, string("LVILL1/LPAYS1/£CL000"), &sCountry, true) ;

  // Query string for viamichelin.com (obsolete)
  //
  string sWebLink = string("http://www.viamichelin.com/viamichelin/fra/dyn/controller/mapPerformPage?") ;

  string sAdress ;
  if (sLigne2 == string(""))
  	sAdress = sLigne1 ;
  else
  	sAdress = sLigne2 ;
  sWebLink += string("strAddress=") + texteWebLink(sAdress) ;
  sWebLink += string("&strLocation=") + texteWebLink(sVille) ;
  sWebLink += string("&strCP=") + texteWebLink(sCP) ;
  sWebLink += string("&strCountry=") + texteWebLink(sCountry) ;

  // Query string for maps.google.com
  //
  string sWebLink2 = string("http://maps.google.com/maps?q=") ;

  sWebLink2 += texteWebLink(sAdress) ;
  sWebLink2 += texteWebLink(string("  ") + sVille) ;
  sWebLink2 += texteWebLink(string("  ") + sCP) ;
  sWebLink2 += texteWebLink(string("  ") + sCountry) ;

  // Query string for openstreetmap.org but don't know how to add a marker
  //
  string sWebLink3 = string("http://www.openstreetmap.org/search?query=") ;

  sWebLink3 += texteWebLink(sAdress) ;
  sWebLink3 += texteWebLink(string("  ") + sVille) ;
  sWebLink3 += texteWebLink(string("  ") + sCP) ;
  sWebLink3 += texteWebLink(string("  ") + sCountry) ;

/*
  string sWebLinkToMap = sWebLink ;

  int iRandomNumber = rand() ;
  int iRandomDividedByTwo = iRandomNumber / 2 ;
  if (iRandomDividedByTwo * 2 == iRandomNumber)
  	sWebLinkToMap = sWebLink2 ;
*/

  string sWebLinkToMap = sWebLink2 ;

  // http://maps.google.com/maps?q=15%2C%20Rue%20Hoche%20%20VERSAILLES%20%2078000%20
	// http://www.viamichelin.com/viamichelin/fra/dyn/controller/mapPerformPage?strAddress=143%2C%20rue%20de%20Saussure&strLocation=PARIS&strCP=75017&strCountry=

  ::ShellExecute(pContexte->GetMainWindow()->HWindow,
                  "open",
                  sWebLinkToMap.c_str(),
                  "",
                  "",
                  SW_SHOWNORMAL);
}
catch (...)
{
	erreur("Exception SurfOnTheMap.", standardError, 0) ;
	return ;
}
}

void
ShowQrCode(NSDlgFonction *pNSFct)
{
try
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
  	return ;

  string sSearch = string("") ;

	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if ((NULL == pTransfert) || (NULL == pTransfert->getTransfertMessage()))
    return ;

  sSearch = pTransfert->getTransfertMessage()->GetTexteLibre() ;

  if (string("") == sSearch)
  	return ;

  NSContexte* pContexte = pNSFct->pContexte ;

  string sWebLink = string("http://chart.apis.google.com/chart?cht=qr&chs=300x300&chl=") +
                                              sSearch + string("&chld=H|0") ;

  ::ShellExecute(pContexte->GetMainWindow()->HWindow,
                  "open",
                  sWebLink.c_str(),
                  "",
                  "",
                  SW_SHOWNORMAL) ;
}
catch (...)
{
	erreur("Exception ShowQrCode.", standardError, 0) ;
	return ;
}
}

//
// Calcul de la surface corporelle
//
void CalcSC(NSDlgFonction *pNSFct)
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
    return ;

  double dSC ;
  string sSC = "" ;
  BBFilsItem* fils ;
  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;

	string sPoids ;
  fils = RetournerValeurEdit(pItem, string("VPOID1/2KG001/£N0;03"), &sPoids, true) ;
  string sTaille ;
  fils  = RetournerValeurEdit(pItem, string("VTAIL1/2CM001/£N0;03"), &sTaille, true) ;

  if ((sPoids != "") && (sTaille != ""))
  {
    dSC = 0.0001 * 71.84 * pow(StringToDouble(sPoids), 0.425) *
        								pow(StringToDouble(sTaille), 0.725) ;

    sSC = DoubleToString(&dSC, -1, 2) ;

    //activer le BBFilsItem qui pilote le champ QR
    string retour ;
    fils = RetournerValeurEdit(pItem, string("VSUCO1/2MCAR1/£N0;03"), &retour, true) ;
    if (fils)
      ActiveChampEdit(fils, sSC) ;
  }
}

//
// Prise de l'IPP actif
//
void GetIPP(NSDlgFonction *pNSFct)
{
	if (((NSDlgFonction*) NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  // Get the BigBoss in order to reach the context
  //
	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if ((NSTransferInfo*) NULL == pTransfert)
		return ;

	BBFilsItem* fils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == fils)
  	return ;

	if ((NULL == pNSFct->getItem()) || (NULL == pNSFct->getItem()->_pBigBoss))
		return ;

	NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;

  // Now, get the patient
  //
	// Beware: At creation time, there is no patient
  //
  if ((NSPatientChoisi*) NULL == pContexte->getPatient())
		return ;

  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphPerson)
		return ;

  string sSite = pContexte->getSuperviseur()->getIppSite() ;
  
  // if (string("") == sSite)
	//	return ;

  // Get the patpatho of the identifiers library
  //
	NSPatPathoArray pptIdent(pContexte->getSuperviseur()) ;
	if (false == pGraphPerson->getLibIDsPpt(&pptIdent))
		return ;

  // Now, get the IPP that corresponds to the site
  //
  string sCurrentIpp ;
  if (false == pGraphPerson->IPPEnCours(&pptIdent, sSite, &sCurrentIpp))
  	return ;

	ActiveChampEdit(fils, sCurrentIpp) ;
}

//
// Prise de l'IPP local
//
void GetLocalIPP(NSDlgFonction *pNSFct, bool bManual)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  if ((NULL == pNSFct->getItem()) || (NULL == pNSFct->getItem()->_pBigBoss))
		return ;
  NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;

  string sTrace = string("Entering GetLocalIPP with Manual = ") + (bManual ? string("true") : string("false")) ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (NULL == pTransfert)
		return ;

	BBFilsItem* fils = pTransfert->getFilsItem() ;
  if (NULL == fils)
  	return ;

	// At creation time, there is no patient
  if (NULL == pContexte->getPatient())
  {
    if (false == bManual)
    {
      // Automatic proposal of the next Id to attribute
      //
      string sNewIPP = GetNewIPP(pContexte) ;
      if (string("") == sNewIPP)
        return ;

      ActiveChampEdit(fils, sNewIPP) ;
    }
		return ;
  }

  NSPersonGraphManager* pGraphPerson = pContexte->getPatient()->getGraphPerson() ;
  if (NULL == pGraphPerson)
  {
    sTrace = string("GetLocalIPP: no graph found, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
		return ;
  }

	NSPatPathoArray pptIdent(pContexte->getSuperviseur()) ;
	if (false == pGraphPerson->getLibIDsPpt(&pptIdent))
  {
    sTrace = string("GetLocalIPP: no IPP library found in graph, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
		return ;
  }

  string sCurrentIpp ;
  if (false == pGraphPerson->IPPEnCours(&pptIdent, string(""), &sCurrentIpp))
  {
    sTrace = string("GetLocalIPP: no IPP found in library, leaving") ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	return ;
  }

  sTrace = string("GetLocalIPP: found IPP \"") + sCurrentIpp + string("\", activating edit control") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;

	ActiveChampEdit(fils, sCurrentIpp) ;
}

string GetNewIPP(NSContexte* pContexte)
{
  ifstream inFile ;

  // ATTENTION : ce fichier est obligatoirement avec la base des patients
  string sFichierCompteurs = pContexte->PathName("BGLO") + string("nauti.lus") ;

  inFile.open(sFichierCompteurs.c_str());  if (!inFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return string("") ;
  }

  string sNewValue = string("") ;
  string sRegulExp = string("") ;

  string sLine ;
  while (!inFile.eof())  {
    getline(inFile, sLine) ;
    if (string("") != sLine)
    {
      string sLabel = string("") ;
      string sValue = string("") ;

      size_t i = 0 ;
      while ((i < strlen(sLine.c_str())) && (sLine[i] != ' ') && (sLine[i] != '\t'))
        sLabel += sLine[i++] ;

      while ((i < strlen(sLine.c_str())) && ((sLine[i] == ' ') || (sLine[i] == '\t')))        i++ ;

      while (i < strlen(sLine.c_str()))        sValue += sLine[i++] ;

      sLabel = pseumaj(sLabel) ;
      if      (string("IPP") == sLabel)
        sNewValue = sValue ;
      else if (string("REGEX") == sLabel)
        sRegulExp = sValue ;
    }
  }
  inFile.close() ;

  string sNextVal = string("") ;

  // Elaborate the new value
  //
  if (string("d+") == sRegulExp)
  {
    int iVal = atoi(sNewValue.c_str()) ;
    iVal++ ;
    char szNextVal[34] ;
    itoa(iVal, szNextVal, 10) ;
    sNextVal = string(szNextVal) ;
  }

  ofstream outFile ;
  // On réécrit le nouveau fichier
  outFile.open(sFichierCompteurs.c_str()) ;
  if (!outFile)
  {
    string sErrorText = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningOutputFile") ;
    pContexte->getSuperviseur()->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError, 0) ;
    return sNewValue ;
  }

  outFile << ( string("IPP   ") + sNextVal  + string("\n") ) ;
  outFile << ( string("REGEX ") + sRegulExp + string("\n") ) ;

  outFile.close() ;  return sNewValue ;}

//
// Calcul de l'Index de Masse Corporelle
//
void CalcIMC(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sPoids ;
	fils = RetournerValeurEdit(pItem, string("VPOID1/2KG001/£N0;03"), &sPoids, true) ;
  if (string("") == sPoids)
		return ;
	string sTaille ;
	fils  = RetournerValeurEdit(pItem, string("VTAIL1/2CM001/£N0;03"), &sTaille, true) ;
	if (string("") == sTaille)
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VIMC02/2KGM21/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

	double dTaille = StringToDouble(sTaille) ;

	if (dTaille < 0.02)
		return ;

	double dIMC = StringToDouble(sPoids) / pow(StringToDouble(sTaille) / 100, 2) ;
	string sIMC = DoubleToString(&dIMC, -1, 2) ;

	ActiveChampEdit(fils, sIMC) ;
}

//
// Calcul de l'Index de Pression Systolique
//
void CalcIPS(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  // Right arm or Left arm?
  //
  if (NULL == pNSFct->getControl()->getTransfert())
    return ;

  BBFilsItem*	pFilsItem = pNSFct->getControl()->getTransfert()->getFilsItem() ;
  if (NULL == pFilsItem)
    return ;

  string sLocalisation = pFilsItem->getLocalisation() ;

  string sBodySide = string("") ;
  string sArm      = string("") ;
  string sLeg      = string("") ;
  if      ((sLocalisation.find(string("ACDRO")) != string::npos) ||
           (sLocalisation.find(string("ACHDV")) != string::npos))
  {
    sBodySide = string("ACDRO1") ;
    sArm      = sBodySide + string(1, cheminSeparationMARK) + string("ABRAD1") ;
    sLeg      = sBodySide + string(1, cheminSeparationMARK) + string("AMEID1") ;
  }
  else if ((sLocalisation.find(string("ACGAU")) != string::npos) ||
           (sLocalisation.find(string("ACHGV")) != string::npos))
  {
    sBodySide = string("ACGAU1") ;
    sArm      = sBodySide + string(1, cheminSeparationMARK) + string("ABRAG1") ;
    sLeg      = sBodySide + string(1, cheminSeparationMARK) + string("AMEIG1") ;
  }

  if (string("") == sBodySide)
    return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  string sPAS1Loc = sArm + string("/GPREA1/MCOUC2/GPAL11/VPRE01/2MMHG1/£N0;03") ;
  string sPAS1 ;
	fils = RetournerValeurEdit(pItem, sPAS1Loc, &sPAS1, true) ;

  string sPAS2Loc = sArm + string("/GPREA1/MCOUC2/GPAL21/VPRE01/2MMHG1/£N0;03") ;
  string sPAS2 ;
	fils = RetournerValeurEdit(pItem, sPAS2Loc, &sPAS2, true) ;

  if ((string("") == sPAS1) && (string("") == sPAS2))
		return ;

	string sAnkleSP ;
  string sAnkleLoc = sLeg + string("/VPASD1/2MMHG1/£N0;03") ;
	fils  = RetournerValeurEdit(pItem, sAnkleLoc, &sAnkleSP, true) ;
	if (string("") == sAnkleSP)
		return ;

	string retour ;
  string sIPSLoc = sLeg + string("/VIPSC1/200001/£N0;03") ;
	fils = RetournerValeurEdit(pItem, sIPSLoc, &retour, true) ;
	if (NULL == fils)
		return ;

	double dPAS = max(StringToDouble(sPAS1), StringToDouble(sPAS2)) ;

	if (dPAS < 0.02)
		return ;

	double dIPS = StringToDouble(sAnkleSP) / dPAS ;
	string sIPS = DoubleToString(&dIPS, -1, 2) ;

	ActiveChampEdit(fils, sIPS) ;
}

//
// Calcul de l'Index de Pression Systolique du membre gauche
//
void CalcIPSLeft(NSDlgFonction *pNSFct)
{
  CalcIPSSide(pNSFct, string("ACHGV"), string("AMEIG"), string("ACGAU")) ;
}

void CalcIPSRight(NSDlgFonction *pNSFct)
{
  CalcIPSSide(pNSFct, string("ACHDV"), string("AMEID"), string("ACDRO")) ;
}

void CalcIPSSide(NSDlgFonction *pNSFct, string sAnkle, string sInferiorMember, string sBodySide)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  // Get max SAP at arm level
  //
  string sPAS1 = GetMaxSystolicArterialPressure(pNSFct) ;
  if (string("") == sPAS1)
    return ;

  // Get SAP at leg level
  //
  string sAnkleSP ;
	fils = RetournerValeurEdit(pItem, sAnkle + string("/VPASD1/2MMHG1/£N0;03"), &sAnkleSP, true) ;
	if (string("") == sAnkleSP)
  {
    fils = RetournerValeurEdit(pItem, sInferiorMember + string("/VPASD1/2MMHG1/£N0;03"), &sAnkleSP, true) ;

    if (string("") == sAnkleSP)
    {
      fils = RetournerValeurEdit(pItem, sBodySide + string("/VPASD1/2MMHG1/£N0;03"), &sAnkleSP, true) ;

      if (string("") == sAnkleSP)
		    return ;
    }
  }

  // Get destination control
  //
	string retour ;
	fils = RetournerValeurEdit(pItem, sAnkle + string("/VIPSC1/200001/£N0;03"), &retour, true) ;
	if (NULL == fils)
  {
    fils = RetournerValeurEdit(pItem, sInferiorMember + string("/VIPSC1/200001/£N0;03"), &retour, true) ;

    if (NULL == fils)
    {
      fils = RetournerValeurEdit(pItem, sBodySide + string("/VIPSC1/200001/£N0;03"), &retour, true) ;

      if (NULL == fils)
		    return ;
    }
  }

	double dPAS = StringToDouble(sPAS1) ;
	if (dPAS < 0.02)
		return ;

	double dIPS = StringToDouble(sAnkleSP) / dPAS ;
	string sIPS = DoubleToString(&dIPS, -1, 2) ;

	ActiveChampEdit(fils, sIPS) ;
}

string GetLeftArmSystolicArterialPressure(NSDlgFonction *pNSFct)
{
  return GetArmSystolicArterialPressure(pNSFct, string("ABRAG"), string("ACGAU")) ;
}

string GetRightArmSystolicArterialPressure(NSDlgFonction *pNSFct)
{
  return GetArmSystolicArterialPressure(pNSFct, string("ABRAD"), string("ACDRO")) ;
}

string GetArmSystolicArterialPressure(NSDlgFonction *pNSFct, string sArm, string sSide)
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return string("") ;

	// BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return string("") ;

  // Get SAP at arm level
  //
  string sPAS = string("") ;
	/* fils = */ RetournerValeurEdit(pItem, sArm + string("/VPRE01/2MMHG1/£N0;03"), &sPAS, true) ;
  if (string("") != sPAS)
    return sPAS ;

  // Get SAP for this body side
  //
  /* fils = */ RetournerValeurEdit(pItem, sSide + string("/VPRE01/2MMHG1/£N0;03"), &sPAS, true) ;

  return sPAS ;
}

string GetMaxSystolicArterialPressure(NSDlgFonction *pNSFct)
{
  string sLeftPAS  = GetLeftArmSystolicArterialPressure(pNSFct) ;
  string sRightPAS = GetRightArmSystolicArterialPressure(pNSFct) ;

  if (string("") == sLeftPAS)
    return sRightPAS ;

  if (string("") == sRightPAS)
    return sLeftPAS ;

  double dLeftPAS  = StringToDouble(sLeftPAS) ;
  double dRightPAS = StringToDouble(sRightPAS) ;

  if (dLeftPAS > dRightPAS)
    return sLeftPAS ;

  return sRightPAS ;
}

//
// Calcul du gain de poids de grossesse
//
void CalcWeigthGain(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sPoids ;
	fils = RetournerValeurEdit(pItem, string("VPOID1/2KG001/£N0;03"), &sPoids, true) ;
  if (string("") == sPoids)
		return ;
	string sPoidsDebutGrossesse ;
	fils  = RetournerValeurEdit(pItem, string("VPOIG1/2KG001/£N0;03"), &sPoidsDebutGrossesse, true) ;
	if (string("") == sPoidsDebutGrossesse)
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VGPOI1/2KG001/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	double dGP = StringToDouble(sPoids) - StringToDouble(sPoidsDebutGrossesse) ;
	string sGP = DoubleToString(&dGP, -1, 2) ;

	ActiveChampEdit(fils, sGP) ;
}

//
// Calcul de la consommation hebdomadaire d'alcool
//
void CalcVCHAL(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()))
		return ;

	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sConsoQuot ;
	BBFilsItem* fils = RetournerValeurEdit(pItem, string("VCOAL1/2VVIN1/£N0;03"), &sConsoQuot, true) ;
	if (sConsoQuot == "")
		return ;
	string sFrequency ;
	fils = RetournerValeurEdit(pItem, string("VFREQ1/2JPSE1/£N0;03"), &sFrequency, true) ;
	if (sFrequency == "")
		return ;

  string retour ;
	fils = RetournerValeurEdit(pItem, string("VCHAL1/2VVAS1/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	double dConsoHebdo = StringToDouble(sConsoQuot) * StringToDouble(sFrequency) ;
	string sConsoHebdo = DoubleToString(&dConsoHebdo, -1, 0) ;

	//activer le BBFilsItem qui pilote le champ
	ActiveChampEdit(fils, sConsoHebdo) ;
}

//
// Calcul de la durée d'arrêt du tabac à partir de la date d'arrêt
//
void CalcVDATA(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()))
		return ;

	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sDateArret ;
	BBFilsItem* fils = RetournerValeurEdit(pItem, string("KARRE1/2DA011/£D0;10"), &sDateArret, true) ;
	if (sDateArret == "")
		return ;

  string retour ;
	fils = RetournerValeurEdit(pItem, string("VDATA1/2DAT31/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	NVLdVTemps tEndDate ;
	if (!(tEndDate.initFromDate(sDateArret)))
		return ;

	NVLdVTemps tNow ;
	tNow.takeTime() ;

  unsigned long lStopDays = tNow.daysBetween(tEndDate) ;
	double dNbYearsSinceEnd = double(lStopDays) / double(365.25) ;

	string sDureeArret = DoubleToString(&dNbYearsSinceEnd, -1, 2) ;

	//activer le BBFilsItem qui pilote le champ
	ActiveChampEdit(fils, sDureeArret) ;
}

//
// Calcul le LDL à partir de Friedwald, en grammes par litre
//
void CalcFriedwaldGpl(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sTrigly ;
	fils = RetournerValeurEdit(pItem, string("VTRIG2/2GPAL1/£N0;03"), &sTrigly, true) ;
  if (sTrigly == "")
		return ;

	// Vérifier Trigglycérides < 4 g/l
  //
  double dTrigly = StringToDouble(sTrigly) ;
  if (dTrigly >= 4)
		return ;

	string sCholTotal ;
	fils  = RetournerValeurEdit(pItem, string("VCHOL1/2GPAL1/£N0;03"), &sCholTotal, true) ;
	if (sCholTotal == "")
		return ;

	string sHDLChol ;
	fils  = RetournerValeurEdit(pItem, string("VCHO02/2GPAL1/£N0;03"), &sHDLChol, true) ;
	if (sHDLChol == "")
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VCHO12/2GPAL1/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	double dLDL = StringToDouble(sCholTotal) - StringToDouble(sHDLChol) - (dTrigly / 5) ;
	string sLDL = DoubleToString(&dLDL, -1, 2) ;

	ActiveChampEdit(fils, sLDL) ;

	// Remettre à zéro l'éventuelle valeur exprimée en mmol/l
	fils = RetournerValeurEdit(pItem, string("VCHO12/2MMOL1/£N0;03"), &retour, true) ;
	if (!fils)
		return ;
	ActiveChampEdit(fils, "") ;
}

//
// Calcul la bilirubine libre = bilirubine totale - bilirubine conjuguée
//
void CalcBilirubineLibre(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  BBFilsItem* fils ;

	string sBiliTotale ;
	fils  = RetournerValeurEdit(pItem, string("VBILT1/2MGPL1/£N0;03"), &sBiliTotale, true) ;
	if (string("") == sBiliTotale)
		return ;

	string sBiliConjug ;
	fils  = RetournerValeurEdit(pItem, string("VBILD1/2MGPL1/£N0;03"), &sBiliConjug, true) ;
	if (string("") == sBiliConjug)
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VBILI1/2MGPL1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

	double dBiliLibre = StringToDouble(sBiliTotale) - StringToDouble(sBiliConjug) ;
	string sBiliLibre = DoubleToString(&dBiliLibre, -1, 2) ;

	ActiveChampEdit(fils, sBiliLibre) ;

	// Remettre à zéro l'éventuelle valeur exprimée en µmol/l
	fils = RetournerValeurEdit(pItem, string("VBILI1/2MIML1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;
	ActiveChampEdit(fils, "") ;
}

//
// Calcul la micro-albuminurie par 24 heures
//
void CalcMicroAlb24(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sMicroAlb ;
	fils = RetournerValeurEdit(pItem, string("VM8AL1/2MGPL1/£N0;03"), &sMicroAlb, true) ;
  if (sMicroAlb == "")
		return ;

	string sVolUrine24 ;
	fils = RetournerValeurEdit(pItem, string("VUR241/2ML001/£N0;03"), &sVolUrine24, true) ;
	if (sVolUrine24 == "")
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VM8AL1/2MG241/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	double dMA24 = StringToDouble(sMicroAlb) * (StringToDouble(sVolUrine24) / 1000) ;
	string sMicroAlb24 = DoubleToString(&dMA24, -1, 2) ;

	ActiveChampEdit(fils, sMicroAlb24) ;
}

//
// Calcul de la consommation de sel
//
void CalcConsoSel(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sNatriurie24 ;
	fils = RetournerValeurEdit(pItem, string("VNATQ3/2ME241/£N0;03"), &sNatriurie24, true) ;
  if (sNatriurie24 == "")
		return ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VCSEL1/2GR241/£N0;03"), &retour, true) ;
	if (!fils)
		return ;

	double dConso = StringToDouble(sNatriurie24) / 17 ;
	string sConsoSel24 = DoubleToString(&dConso, -1, 2) ;

	ActiveChampEdit(fils, sConsoSel24) ;
}

//
// Calcul de la formule de Cockroft à partir d'une natriurie en mg/l
//
void CalcCockroftMGPL(NSDlgFonction *pNSFct)
{
	if (!pNSFct || !(pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (!pItem)
		return ;

	string sCreatininemie ;
	fils = RetournerValeurEdit(pItem, string("VCREA1/2MGPL1/£N0;03"), &sCreatininemie, true) ;
  if (sCreatininemie == "")
		return ;

	// On cherche le poids en repartant de la racine
	string sPoids ;
	fils = RetournerValeurEdit(pItem, string("VPOID1/2KG001/£N0;03"), &sPoids, true, true) ;
  if (sPoids == "")
		return ;

  // On cherche l'âge
  NSContexte* pContexte = pItem->_pBigBoss->pContexte ;
  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (NULL == pPat)
		return ;

	string sBirthDate = pPat->donneNaissance() ;
  if (string("00000000") == sBirthDate)
		return ;

	string sCurrentDate = donne_date_duJour() ;

	int	iAge = donne_age(sCurrentDate, sBirthDate) ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VCLAM1/3COCK1/2MLPM1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

	// ANAES
	//
	// chez l’homme :
	// DFG (ml/min) = [(140-âge)] x poids / 7,2 x créatininémie en mg/l]
	// chez la femme :
	// DFG (ml/min) = [(140-âge)] x poids / 7,2 x créatininémie en mg/l] x 0,85

	double dCorrectingFactor = 1 ;
	if (pPat->estMasculin())
		dCorrectingFactor = 1 ;
	else if (pPat->estFeminin())
		dCorrectingFactor = 0.85 ;

	double dClairance = (140 - double(iAge)) * StringToDouble(sPoids) / (StringToDouble(sCreatininemie) * 7.2) ;
  dClairance = dCorrectingFactor * dClairance ;
	string sClairance = DoubleToString(&dClairance, -1, 2) ;

	ActiveChampEdit(fils, sClairance) ;
}

//
// Calcul de la formule de Cockroft à partir d'une natriurie en µmol/l
//
void CalcCockroftMMPL(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sCreatininemie ;
	fils = RetournerValeurEdit(pItem, string("VCREA1/2MIML1/£N0;03"), &sCreatininemie, true) ;
  if (string("") == sCreatininemie)
		return ;

	// On cherche le poids en repartant de la racine
	string sPoids ;
	fils = RetournerValeurEdit(pItem, string("VPOID1/2KG001/£N0;03"), &sPoids, true, true) ;
  if (string("") == sPoids)
		return ;

  // On cherche l'âge
  NSContexte* pContexte = pItem->_pBigBoss->pContexte ;
  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (NULL == pPat)
		return ;

  string sBirthDate = pPat->donneNaissance() ;
  if (string("00000000") == sBirthDate)
		return ;

	string sCurrentDate = donne_date_duJour() ;

	int	iAge = donne_age(sCurrentDate, sBirthDate) ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VCLAM1/3COCK1/2MLPM1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

	double dCorrectingFactor = 1 ;
	if (pPat->estMasculin())
		dCorrectingFactor = 1.23 ;
	else if (pPat->estFeminin())
		dCorrectingFactor = 1.04 ;

	double dClairance = (140 - double(iAge)) * StringToDouble(sPoids) / StringToDouble(sCreatininemie) ;
  dClairance = dCorrectingFactor * dClairance ;
	string sClairance = DoubleToString(&dClairance, -1, 2) ;

	ActiveChampEdit(fils, sClairance) ;
}

//
// Calcul de la formule de MDRD4 à partir d'une natriurie en mg/l
//
void CalcMdrdMGPL(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sCreatininemie ;
	fils = RetournerValeurEdit(pItem, string("VCREA1/2MGPL1/£N0;03"), &sCreatininemie, true) ;
  if (string("") == sCreatininemie)
		return ;

  // On cherche l'âge
  NSContexte* pContexte = pItem->_pBigBoss->pContexte ;
  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (NULL == pPat)
		return ;

  string sBirthDate = pPat->donneNaissance() ;
  if (string("00000000") == sBirthDate)
		return ;

	string sCurrentDate = donne_date_duJour() ;

	int	iAge = donne_age(sCurrentDate, sBirthDate) ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VDFG11/3MDR41/2MLPI1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

  // MDRD formula is expressed in mg/dL, since we express it in mg/L we must
  // divide by 10

  double dConvertedCreat = StringToDouble(sCreatininemie) / 10 ;

	double dCorrectingFactor = 1 ;
	if (pPat->estMasculin())
		dCorrectingFactor = 1 ;
	else if (pPat->estFeminin())
		dCorrectingFactor = 0.742 ;

  double dMDRD = double(186) * pow(dConvertedCreat, -1.154) * pow(iAge, -0.203) ;

  dMDRD = dCorrectingFactor * dMDRD ;
	string sDFG = DoubleToString(&dMDRD, -1, 2) ;

	ActiveChampEdit(fils, sDFG) ;
}

//
// Calcul de la formule de MDRD4 à partir d'une natriurie en µmol/l
//
void CalcMdrdMMPL(NSDlgFonction *pNSFct)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBFilsItem* fils ;
	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sCreatininemie ;
	fils = RetournerValeurEdit(pItem, string("VCREA1/2MIML1/£N0;03"), &sCreatininemie, true) ;
  if (string("") == sCreatininemie)
		return ;

  // On cherche l'âge
  NSContexte* pContexte = pItem->_pBigBoss->pContexte ;
  NSPatientChoisi* pPat = pContexte->getPatient() ;
  if (NULL == pPat)
		return ;

	string sBirthDate = pPat->donneNaissance() ;
  if (string("00000000") == sBirthDate)
		return ;

	string sCurrentDate = donne_date_duJour() ;

	int	iAge = donne_age(sCurrentDate, sBirthDate) ;

	string retour ;
	fils = RetournerValeurEdit(pItem, string("VDFG11/3MDR41/2MLPI1/£N0;03"), &retour, true) ;
	if (NULL == fils)
		return ;

  double dConvertedCreat = StringToDouble(sCreatininemie) / 88.4 ;

	double dCorrectingFactor = 1 ;
	if (pPat->estMasculin())
		dCorrectingFactor = 1 ;
	else if (pPat->estFeminin())
		dCorrectingFactor = 0.742 ;

  double dMDRD = double(186) * pow(1000 * dConvertedCreat, -1.154) * pow(iAge, -0.203) ;

  dMDRD = dCorrectingFactor * dMDRD ;
	string sDFG = DoubleToString(&dMDRD, -1, 2) ;

	ActiveChampEdit(fils, sDFG) ;
}

//
// Classification automatique à partir du contenu d'un champ Edit
//
void Classifying(NSDlgFonction *pNSFct, bool /* bOneTopic */)
{
	if (((NSDlgFonction*) NULL == pNSFct) ||
             (NULL == pNSFct->getControl()) ||
                   (NULL == pNSFct->getControl()->getTransfert()))
		return ;

  BBFilsItem* pFilsItem = pNSFct->getControl()->getTransfert()->getFilsItem() ;
  if ((BBFilsItem*) NULL == pFilsItem)
  	return ;

  //
  // Getting the text from edit control
  //
  NSEdit* pEdit = (static_cast<NSEdit *>(pNSFct->getControl()->getControle())) ;
  if ((NSEdit*) NULL == pEdit)
  	return ;

  string sCapturedText = pEdit->GetText() ;

  if (string("") == sCapturedText)
    return ;

  //
  // Finding the proper classification
  //
  string sChemin = pFilsItem->getItemFather()->_sLocalisation ;

  NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;
  NSEpisodus* pEpisodus = pContexte->getEpisodus() ;

  PrinciplesArray* pPrincipes = pEpisodus->pPrincipes ;
  if ((PrinciplesArray*) NULL == pPrincipes)
  	return ;

  string sPostCase = string("") ;
  ClassificationPrinciple* pPrincipe = pPrincipes->trouvePrincipe(sChemin, sPostCase) ;

  //
  // Ou demander
  //
  if ((ClassificationPrinciple*) NULL == pPrincipe)
  	return ;

  //
  // Puis trouver le code
  //
  string sClassif = pPrincipe->sClassification ;

  string sResO = string("") ;
  string sResP = string("") ;
  string sResI = string("") ;
  string sRes3 = string("") ;

	ParseSOAP SOAPParser(pContexte, sClassif, string("")) ;
  SOAPParser.computeParsing(&sCapturedText, &sChemin, &sResO, &sResP, &sResI, &sRes3) ;

  //
  // On trouve le code
  //
  classifExpert* pExpert = pEpisodus->pClassifExpert ;
  if ((classifExpert*) NULL == pExpert)
    return ;

  NSEpiClassifInfoArray arrayClassif ;

  ElemSetArray* pElemDomain = 0 ;
  //
  // On instancie le domaine
  // Instanciating the domain
  //
  string sDomain  = sResP ;
  ParseCategory Parser(pExpert->donneCodeSize(sClassif), sClassif,
                                             pExpert->donnePattern(sClassif)) ;
  pElemDomain = Parser.DefDomain(sDomain) ;
  //
  // On trouve les codes qui correspondent au domaine
  // Finding the codes that belong to the domain
  //
  string sCaseSens ;
  pContexte->getDico()->donneCodeSens(&(pPrincipe->sCase), &sCaseSens) ;

  pExpert->fillList(sClassif, pElemDomain, &arrayClassif, sCaseSens) ;

  if (pElemDomain)
    delete pElemDomain ;

	SOAPObject* pSOAPObj = new SOAPObject(pFilsItem->getItemLabel(), sResP, sClassif, 0, pFilsItem) ;
#ifndef _EXT_CAPTURE
	NSCapture* pCapture = new NSCapture(pContexte) ;
#else
	NSCapture* pCapture = new NSCapture() ;
#endif

	pCapture->setClassifResultO(sResO) ;
  pCapture->setClassifResultP(sResP) ;
  pCapture->setClassifResultI(sResI) ;

  pSOAPObj->pCaptElemnt = pCapture ;
  pSOAPObj->sCase       = sCaseSens ;

  //
  // Sending to the SOAP tank
  //
  NSToDoTask* pTask = new NSToDoTask ;

	pTask->setWhatToDo(string("AddToSOAP")) ;
	pTask->setPointer1((void*) pSOAPObj, false) ;

  pTask->sendMe(pContexte->getSuperviseur()) ;

/*
  if (!bOneTopic)
  {
    // Traitement des retours chariot
    size_t iCR = sCapturedText.find('\n') ;
    while (iCR != string::npos)
    {
      string sSubText = string(sCapturedText, 0, iCR) ;
      strip(sSubText, stripBoth, '\r') ;
      strip(sSubText, stripBoth) ;
      sCapturedText = string(sCapturedText, iCR+1, strlen(sCapturedText.c_str())-iCR-1) ;
      strip(sCapturedText, stripBoth, '\r') ;
      strip(sCapturedText, stripBoth) ;

      if (sSubText != "")
      {
        NSCapture * pCapt = new NSCapture(pContexte, pModel->sCorrespondence, sSubText, pModel->sClassifier, pModel->sUnit) ;
        pCaptArray->ajouter(pCapt) ;

        // Attention : la fonction "ajouter" peut supprimer pCapt s'il doublonne un élément existant
        if (pCapt && (pCapt->sClassifier != ""))
          addToSOAP(pCapt) ;
      }
      iCR = sCapturedText.find('\n') ;
    }
  }

	if (sCapturedText != "")
	{
		NSCapture * pCapt = new NSCapture(pContexte, pModel->sCorrespondence, sText, pModel->sClassifier, pModel->sUnit) ;
		pCaptArray->ajouter(pCapt) ;

		// Attention : la fonction "ajouter" peut supprimer pCapt s'il doublonne un élément existant
		if (pCapt && (pCapt->sClassifier != ""))
			addToSOAP(pCapt) ;
	}


  SOAPObject* pSOAPObj = new SOAPObject(sLibellePere, szTxtBuff, sLexique, 100, (*pptIt)->pDonnees->getNode()) ;
  pSOAPObj->sCase = sCase ;  pSuper->getEpisodus()->addToSOAP(pSOAPObj) ;

*/
}

void initFromCurrentConcerns(NSDlgFonction *pNSFct)
{
  if ((NSDlgFonction*) NULL == pNSFct)
    return ;

  NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;
  string sTrace = string("Entering initFromCurrentConcerns") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  for (int iFr = 0 ; iFr < FRAMECOUNT ; iFr++)
  {
    LDVFRAME iFrame = getFrameForIndex(iFr) ;
    initFromCurrentFrameConcerns(pNSFct, iFrame) ;
  }

  sTrace = string("Leaving initFromCurrentConcerns") ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;
}

void initFromCurrentHealthProblems(NSDlgFonction *pNSFct)
{
  initFromCurrentFrameConcerns(pNSFct, ldvframeHealth) ;
}

void initFromCurrentSocialProblems(NSDlgFonction *pNSFct)
{
  initFromCurrentFrameConcerns(pNSFct, ldvframeSocial) ;
}

void initFromCurrentRisks(NSDlgFonction *pNSFct)
{
  initFromCurrentFrameConcerns(pNSFct, ldvframeRisk) ;
}

void initFromCurrentFrameConcerns(NSDlgFonction *pNSFct, LDVFRAME iFrame)
{
	if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  // Only treeWindows are accepted
  //
  if (isTreeWindow != pNSFct->getControl()->getType())
		return ;
	NSTreeWindow* pTreeWin = static_cast<NSTreeWindow*>(pNSFct->getControl()->getControle()) ;
  if (NULL == pTreeWin)
		return ;

	// Make certain we can get the context
  //
	if ((NULL == pNSFct->getItem()) || (NULL == pNSFct->getItem()->_pBigBoss))
		return ;

	if (false == pNSFct->getItem()->_pBigBoss->getPatPatho()->empty())
  	return ;

	NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;

  // Getting the frame tree from patient's graph
  //
  string sFrameRoot     = getRootForFrame(iFrame) ;
  string sFrameRootSens = string("") ;
  NSDico::donneCodeSens(&sFrameRoot, &sFrameRootSens) ;

  string sPath = sFrameRootSens + string(1, cheminSeparationMARK) + string("0PRO1") ;

  string sTrace = string("Entering initFromCurrentFrameConcerns for path ") + sPath ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trDetails) ;

  PathsList que ;
  string sAnswerDate ;
  que.push_back(new string(sPath)) ;  // PathsList's destructor makes delete

  BB1BBInterface* interfa = pContexte->getBBinterface() ;

  NSPatPathoArray* respat = interfa->SearchInPatientFolder(&que, &sAnswerDate) ;

  // Nothing found in patient's record - we leave
  //
  if ((NSPatPathoArray*) NULL == respat)
  {
    sTrace = string("initFromCurrentFrameConcerns : no index tree found in record for path ") + sPath ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	return ;
  }

	if (respat->empty())
	{
    sTrace = string("initFromCurrentFrameConcerns : empty index tree for path ") + sPath ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
  	delete respat ;
    return ;
	}

  PatPathoIter iter = respat->begin() ;
  string sRootSens = (*iter)->getLexiqueSens() ;
  if (string("0PRO1") != sRootSens)
  {
    sTrace = string("initFromCurrentFrameConcerns : no 0PRO1 branch in tree for path ") + sPath ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    delete respat ;
		return ;
  }

  iter++ ;
  if (respat->end() == iter)
  {
    sTrace = string("initFromCurrentFrameConcerns : no current concern found for path ") + sPath ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
    delete respat ;
		return ;
  }

  NSPatPathoArray PPT(pContexte->getSuperviseur()) ;

  string sHealthConcernCode = string("") ;
  string sCISPCode          = string("") ;

  NVLdVTemps tDateOuverture ;
	NVLdVTemps tDateFermeture ;
  NVLdVTemps tNow ;
	tNow.takeTime() ;

  int iBaseCol = 0 ;

  for ( ; respat->end() != iter; iter++)
  {
  	int iCol = (*iter)->getColonne() ;
    if (1 == iCol)
    {
    	if (string("") != sHealthConcernCode)
    		if ((tDateOuverture <= tNow) && ((tDateFermeture.estNoLimit()) || (tDateFermeture >= tNow)))
        {
					PPT.ajoutePatho(sHealthConcernCode, iBaseCol) ;
          if (string("") != sCISPCode)
          {
            Message Msg ;
		        Msg.SetComplement(sCISPCode) ;
		        PPT.ajoutePatho("6CISP1", &Msg, iBaseCol + 1) ;
          }
        }

    	sHealthConcernCode = (*iter)->getLexique() ;
      sCISPCode          = string("") ;
      tDateOuverture.init() ;
			tDateFermeture.setNoLimit() ;
    }
    else if (2 == iCol)
    {
      string sSens = (*iter)->getLexiqueSens() ;

    	// Dates
      if ((string("KOUVR") == sSens) || (string("KFERM") == sSens))
      {
      	iter++ ;
        if (respat->end() == iter)
        	break ;

        string sUnite  = string("") ;
        string sFormat = string("") ;
        string sValeur = string("") ;
        string sTemp   = string("") ;
        string sValueLex = (*iter)->getLexique() ;
        if ('£' == sValueLex[0])
        {
        	sFormat = (*iter)->getLexiqueSens() ;
          sValeur = (*iter)->getComplement() ;
          sUnite  = (*iter)->getUnitSens() ;

          // sFormat est du type £D0;03
          if ((string("") != sFormat) &&
              (('D' == sFormat[1]) || ('T' == sFormat[1])) &&
              (string("") != sValeur))
          {
          	if ((sUnite == "2DA01") || (sUnite == "2DA02"))
            {
            	if (string("KOUVR") == sSens)
              	tDateOuverture.initFromDate(sValeur) ;
              else
              	if (string("KFERM") == sSens)
                	tDateFermeture.initFromDate(sValeur) ;
            }
          }
        }
      }
      else if (string("6CISP") == sSens)
        sCISPCode = (*iter)->getComplement() ;
    }
  }

  if (string("") != sHealthConcernCode)
		if ((tDateOuverture <= tNow) && ((tDateFermeture.estNoLimit()) || (tDateFermeture >= tNow)))
    {
    	PPT.ajoutePatho(sHealthConcernCode, iBaseCol) ;
      if (string("") != sCISPCode)
      {
        Message Msg ;
        Msg.SetComplement(sCISPCode) ;
        PPT.ajoutePatho("6CISP1", &Msg, iBaseCol + 1) ;
      }
    }

	delete respat ;

  if (true == PPT.empty())
  {
    sTrace = string("initFromCurrentFrameConcerns : no current concern found for path ") + sPath ;
    pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
		return ;
  }

  // If there is a Frame root, we insert it as a root for the patpatho
  //
  string sFrameRootLabel = getRootLabelForFrame(iFrame) ;
  if (string("") != sFrameRootLabel)
  {
    NSPatPathoArray PPT_copy(pContexte->getSuperviseur()) ;
    PPT_copy = PPT ;
    PPT.vider() ;
    PPT.ajoutePatho(sFrameRootLabel, 0) ;
    PPT.InserePatPathoFille(PPT.begin(), &PPT_copy) ;
  }

	NSCutPaste saveCP(pContexte->getSuperviseur()) ;
  NSCutPaste* pActiveCP = pContexte->getSuperviseur()->pBufCopie ;
  saveCP = *pActiveCP ;

  pActiveCP->vider() ;
	pActiveCP->setPatPatho(&PPT) ;

	pTreeWin->DragApres(NULL, string("COPIE")) ;

	*pActiveCP = saveCP ;

  sTrace = string("initFromCurrentFrameConcerns : done for path ") + sPath ;
  pContexte->getSuperviseur()->trace(&sTrace, 1, NSSuper::trSubDetails) ;
}

void GetPrescriptionCrossover(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering GetPrescriptionCrossover") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sStartingDate ;
	BBFilsItem* fils = RetournerValeurEdit(pItem, string("N00001/KPHAT1/KOUVR1/2DA011/£D0;10"), &sStartingDate, true) ;
	if ((NULL == fils) || (string("") == sStartingDate))
		return ;

  sTraceText = string("GetPrescriptionCrossover : treatment starting date = ") + sStartingDate ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  string sRetour ;
	BBFilsItem* pTargetFils = RetournerValeurEdit(pItem, string("N00001/KPHAT1/VDUCH1/2DAT01/£N0;03"), &sRetour, true) ;
	if (NULL == pTargetFils)
		return ;

  NVLdVTemps tStartingDate ;
	if (false == tStartingDate.initFromDate(sStartingDate))
		return ;

  NSSmallBrother* pBigBoss = pItem->_pBigBoss ;
  if (NULL == pBigBoss)
    return ;

  NSHistoryValManagementArray histoManager(pBigBoss->pContexte, NSHistoryValManagementArray::histTypeDate) ;

  // Getting the accurate path to look for this drug's prescription ending
  //
  // Of course, we could find the KFERM1 element and get its path, but since
  // it is not used otherwise in this function (and could even not exist), we
  // get the KOUVR1 path and adapt it
  //
  string sAccurateClosingPath = fils->getLocalisation() ;
  size_t iDrugPhaseCursor = sAccurateClosingPath.find("N0000/KPHAT/KOUVR") ;
  if (string::npos == iDrugPhaseCursor)
    return ;

  string sItemValue = string(sAccurateClosingPath, 0, iDrugPhaseCursor) + string("N0000/KPHAT/KFERM") ;

  sTraceText = string("GetPrescriptionCrossover : get history for path ") + sItemValue ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  histoManager.ChargeHistoryValueItem(sItemValue) ;
  if (histoManager.empty())
  	return ;

	sort(histoManager.begin(), histoManager.end(), sortByDateSup) ;

  NSHistoryValManagementIter histoIt = histoManager.begin() ;
  string sPreviousEndingDate = (*histoIt)->getValue() ;

  NVLdVTemps tPreviousEndingDate ;
	if (false == tPreviousEndingDate.initFromDate(sPreviousEndingDate))
		return ;

  sTraceText = string("GetPrescriptionCrossover : previous closing date = ") + sPreviousEndingDate ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  int iDeltaDays = 0 ;

  //
  // Let's be accurate here :
  //    - tPreviousEndingDate : last day WITH the drug
  //    - tStartingDate       : first day WITH the drug
  // So, if tPreviousEndingDate == tStartingDate, there is a 1 day crossover
  //
  if (tStartingDate > tPreviousEndingDate)
  {
    iDeltaDays = (int) tStartingDate.daysBetween(tPreviousEndingDate) ;
    iDeltaDays = - iDeltaDays + 1 ;
  }
  else
    iDeltaDays = (int) tPreviousEndingDate.daysBetween(tStartingDate) + 1 ;

  char szCOvalue[33] ;
  itoa(iDeltaDays, szCOvalue, 10) ;

  sTraceText = string("GetPrescriptionCrossover : number of days between those dates = ") + string(szCOvalue) ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

	ActiveChampEdit(pTargetFils, string(szCOvalue)) ;

  sTraceText = string("Leaving GetPrescriptionCrossover") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;
}

void SetPrescriptionCrossover(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering SetPrescriptionCrossover") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

	BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

	string sStartingDate ;
	BBFilsItem* fils = RetournerValeurEdit(pItem, string("N00001/KPHAT1/KOUVR1/2DA011/£D0;10"), &sStartingDate, true) ;
	if ((NULL == fils) || (string("") == sStartingDate))
		return ;

  string sCrossover ;
	BBFilsItem* pTargetFils = RetournerValeurEdit(pItem, string("N00001/KPHAT1/VDUCH1/2DAT01/£N0;03"), &sCrossover, true) ;
	if (NULL == pTargetFils)
		return ;

  sTraceText = string("SetPrescriptionCrossover : crossover is set to ") + sCrossover ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  NVLdVTemps tStartingDate ;
	if (false == tStartingDate.initFromDate(sStartingDate))
		return ;

  NSSmallBrother* pBigBoss = pItem->_pBigBoss ;
  if (NULL == pBigBoss)
    return ;

  NSHistoryValManagementArray histoManager(pBigBoss->pContexte, NSHistoryValManagementArray::histTypeDate) ;

  // Getting the accurate path to look for this drug's prescription ending
  //
  // Of course, we could find the KFERM1 element and get its path, but since
  // it is not used otherwise in this function (and could even not exist), we
  // get the KOUVR1 path and adapt it
  //
  string sAccurateClosingPath = fils->getLocalisation() ;
  size_t iDrugPhaseCursor = sAccurateClosingPath.find("N0000/KPHAT/KOUVR") ;
  if (string::npos == iDrugPhaseCursor)
    return ;

  string sItemValue = string(sAccurateClosingPath, 0, iDrugPhaseCursor) + string("N0000/KPHAT/KFERM") ;

  sTraceText = string("SetPrescriptionCrossover : get history for path ") + sItemValue ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  histoManager.ChargeHistoryValueItem(sItemValue) ;
  if (histoManager.empty())
  	return ;

	sort(histoManager.begin(), histoManager.end(), sortByDateSup) ;

  NSHistoryValManagementIter histoIt = histoManager.begin() ;
  string sPreviousEndingDate = (*histoIt)->getValue() ;

  sTraceText = string("SetPrescriptionCrossover : previous closing date = ") + sPreviousEndingDate ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

  NVLdVTemps tPreviousEndingDate ;
	if (false == tPreviousEndingDate.initFromDate(sPreviousEndingDate))
		return ;

  int iCrossover = atoi(sCrossover.c_str()) ;
  if ((0 == iCrossover) && (string("0") != sCrossover))
    return ;

  // Remember that if tPreviousEndingDate == tStartingDate, there is a 1 day crossover
  //
  int iDeltaDays = 1 - iCrossover ;

  tPreviousEndingDate.ajouteJours(iDeltaDays) ;
  string sNewStartingDate = tPreviousEndingDate.donneDate() ;

  sTraceText = string("SetPrescriptionCrossover : calculated prescription date = ") + sNewStartingDate ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;

	ActiveChampEdit(fils, sNewStartingDate) ;

  sTraceText = string("Leaving SetPrescriptionCrossover") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;
}

// From DDR, set DDR calculated Conception date
//
void SetGestationDDR(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering SetGestationDDR") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  NVLdVTemps tConception ;
  tConception.init() ;

  // First, we check that there is some information available
  //
  string sDDR ;
	BBFilsItem* filsDDR = RetournerValeurEdit(pItem, string("0CAGE1/KDERE1/2DA011/£D0;10"), &sDDR, true) ;
  if ((NULL == filsDDR) || (string("") == sDDR))
  {
    string sTraceText = string("SetGestationDDR : No DDR found... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Then, we check that there is an edit field to host the calculated date
  //
  string sConceptionDate ;
	BBFilsItem* filsConceptionDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/3CDDR1/2DA011/£D0;10"), &sConceptionDate, true) ;
  if (NULL == filsConceptionDate)
  {
    string sTraceText = string("SetGestationDDR : No control found for DRDR conception date... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  NVLdVTemps tDDR ;
  tDDR.initFromDate(sDDR) ;

  tConception = tDDR ;
  tConception.ajouteJours(14) ;

  sConceptionDate = tConception.donneDate() ;
  ActiveChampEdit(filsConceptionDate, sConceptionDate) ;

  sTraceText = string("SetGestationDDR : DRDR conception date set (") + sConceptionDate + string("). Job done. Leaving") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
}

// From DDR calculated Conception date, set the Conception date (if no echo)
//
void SetGestationDatesDDR(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering SetGestationDatesDDR") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  // First, we check that there is a DDR conception date available
  //
  string sDDRConceptionDate ;
	BBFilsItem* filsDDRDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/3CDDR1/2DA011/£D0;10"), &sDDRConceptionDate, true) ;
  if ((NULL == filsDDRDate) || (string("") == sDDRConceptionDate))
  {
    string sTraceText = string("SetGestationDatesDDR : DDR conception date not found... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // First, we check that no echographic information is available
  //
  string sEchoDate ;
	BBFilsItem* filsEchoDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/3EEDC1/2DA011/£D0;10"), &sEchoDate, true) ;
  if ((NULL != filsEchoDate) && (string("") != sEchoDate))
  {
    string sTraceText = string("SetGestationDatesDDR : Echo date found... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Then, we check that there is an edit field to host the calculated date
  //
  string sConceptionDate ;
	BBFilsItem* filsConceptionDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/2DA011/£D0;10"), &sConceptionDate, true) ;
  if (NULL == filsConceptionDate)
  {
    string sTraceText = string("SetGestationDatesDDR : No control found for conception date... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  ActiveChampEdit(filsConceptionDate, sDDRConceptionDate) ;

  sTraceText = string("SetGestationDatesDDR : Conception date set (") + sDDRConceptionDate + string("). Job done. Leaving") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
}

// From Echographic Conception date, set the Conception date
//
void SetGestationDatesEcho(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering SetGestationDatesDDR") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if (NULL == pItem)
		return ;

  // First, we check that echographic information is available
  //
  string sEchoDate ;
	BBFilsItem* filsEchoDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/3EEDC1/2DA011/£D0;10"), &sEchoDate, true) ;
  if ((NULL == filsEchoDate) || (string("") == sEchoDate))
  {
    string sTraceText = string("SetGestationDatesEcho : No echo date found... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  // Then, we check that there is an edit field to host the calculated date
  //
  string sConceptionDate ;
	BBFilsItem* filsConceptionDate = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/2DA011/£D0;10"), &sConceptionDate, true) ;
  if (NULL == filsConceptionDate)
  {
    string sTraceText = string("SetGestationDatesEcho : No control found for conception date... leaving") ;
    pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
    return ;
  }

  ActiveChampEdit(filsConceptionDate, sEchoDate) ;

  sTraceText = string("SetGestationDatesEcho : Conception date set (") + sEchoDate + string("). Job done. Leaving") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trSubDetails) ;
}

void SetGestationDates(NSDlgFonction *pNSFct)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTraceText = string("Entering SetAgeGestSA") ;
  pSuper->trace(&sTraceText, 1, NSSuper::trDetails) ;

  if (((NSDlgFonction*) NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  BBItem* pItem = pNSFct->getControl()->getInterfaceElementItem() ;
  if ((BBItem*) NULL == pItem)
		return ;

  NVLdVTemps tConception ;
  tConception.init() ;

  string sDateConception ;
	BBFilsItem* filsConception = RetournerValeurEdit(pItem, string("0CAGE1/KDATC1/2DA011/£D0;10"), &sDateConception, true) ;

  if (((BBFilsItem*) NULL != filsConception) && (string("") != sDateConception))
    tConception.initFromDate(sDateConception) ;
  else
  {
	  string sDDR ;
	  BBFilsItem* filsDDR = RetournerValeurEdit(pItem, string("0CAGE1/KDERE1/2DA011/£D0;10"), &sDDR, true) ;

    if (((BBFilsItem*) NULL == filsDDR) || (string("") == sDDR))
		  return ;

    NVLdVTemps tDDR ;
    tDDR.initFromDate(sDDR) ;

    tConception = tDDR ;
    tConception.ajouteJours(14) ;
  }

  int iAgeSA    = 0 ;
  int iAgeJours = 0 ;

  NVLdVTemps tNow ;
  tNow.takeTime() ;

  unsigned long lDeltaDays = tNow.daysBetween(tConception) ;

  iAgeJours = int(lDeltaDays) ;

  double dAgeSem = double(iAgeJours) / 7 ;
  int iAgeSem = int(floor(dAgeSem)) ;

  iAgeSA = iAgeSem + 2 ;

  NVLdVTemps tAccouchement = tConception ;
  // tAccouchement.ajouteMois(9) ;
  tAccouchement.ajouteJours(266) ;

  string sBirthDate ;
	BBFilsItem* filsBirth = RetournerValeurEdit(pItem, string("0CAGE1/KACCO1/2DA011/£D0;10"), &sBirthDate, true) ;
	if (NULL != filsBirth)
  {
    sBirthDate = tAccouchement.donneDate() ;
    ActiveChampEdit(filsBirth, sBirthDate) ;
  }

  char szAgeInDays[33] ;
  itoa(iAgeJours, szAgeInDays, 10) ;

  string sAgeInDay ;
	BBFilsItem* filsAgeDays = RetournerValeurEdit(pItem, string("0CAGE1/KAGFO3/2DAA01/£N0;03"), &sAgeInDay, true) ;
	if ((BBFilsItem*) NULL != filsAgeDays)
    ActiveChampEdit(filsAgeDays, string(szAgeInDays)) ;

  char szAgeInSA[33] ;
  itoa(iAgeSA, szAgeInSA, 10) ;

  string sAgeInSA ;
	BBFilsItem* filsAgeSA = RetournerValeurEdit(pItem, string("0CAGE1/KAGFO3/2DAA12/£N0;03"), &sAgeInSA, true) ;
	if (NULL != filsAgeSA)
    ActiveChampEdit(filsAgeSA, string(szAgeInSA)) ;
}

void CheckSurgicalHistory(NSDlgFonction *pNSFct, string sCutConcept)
{
  if (((NSDlgFonction*) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  // Is the control already "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (1 == pTransfert->getActif()))
		return ;

  // Get element's path beyond "surgical history" (QANTC)
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFils)
		return ;

  string sElementPath = pBBFils->getLocalisation() ;

  // Find such an element in a previous report
  //
  NSSearchStruct searchStruct ;
  bool bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sElementPath, &searchStruct) ;
  if ((bOk) && (false == searchStruct.isEmpty()))
  {
    pTransfert->Active() ;
    return ;
  }

  // Find such an element in the synthesis
  //
  size_t iQuantcPos = sElementPath.find(sCutConcept) ;
  if (NPOS == iQuantcPos)
    return ;

  size_t iElmtPathLen = strlen(sElementPath.c_str()) ;
  string sDetails = string("") ;
  iQuantcPos = sElementPath.find(cheminSeparationMARK, iQuantcPos + 1) ;
  if ((NPOS != iQuantcPos) && (iQuantcPos < iElmtPathLen - 1))
    sDetails = string(sElementPath, iQuantcPos + 1, iElmtPathLen - iQuantcPos - 1) ;

  string sSynthesisPath = string("ZSYNT/QANTP/QANTC/") + sDetails ;

  NSSearchStruct searchStruct2 ;
  bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sSynthesisPath, &searchStruct2) ;
  if ((bOk) && (false == searchStruct2.isEmpty()))
    pTransfert->Active() ;
}

void CheckSurgicalHistoryChapter(NSDlgFonction *pNSFct, string sCutConcept)
{
try
{
  if (((NSDlgFonction *) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  // Is the control already "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo *) NULL == pTransfert) || (1 == pTransfert->getActif()))
		return ;

  // Get element's path beyond "surgical history" (QANTC)
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem *) NULL == pBBFils)
		return ;

  string sElementPath = pBBFils->getLocalisation() ;

  // Find such an element in a previous report
  //
  NSSearchStruct searchStruct ;
  bool bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sElementPath, &searchStruct) ;

  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
  {
    // Find such an element in the synthesis
    //
    string sSynthesisPath = string("ZSYNT/QANTP/QANTC/") + pBBFils->getItemLabel() ;

    bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sSynthesisPath, &searchStruct) ;
  }

  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  std::string date   = std::string("") ;
  std::string sNoeud = std::string("") ;

  MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ;
  searchStruct.getFoundNodes()->fullRData(it, date, sNoeud) ;

  NSPatPathoArray pptAnswer(pNSFct->pContexte->getSuperviseur()) ;
  pNSFct->pContexte->getPatient()->DonneArray(sNoeud, &pptAnswer) ;
  if (false == pptAnswer.empty())
  {
    pptAnswer.clearAllIDs() ;

    // pAnswer may contain multiple instances of the same information
    //
    NSVectPatPathoArray Vect  ;
    PatPathoIter iterRoot = pptAnswer.begin() ;
    pptAnswer.ExtraireVecteurPatPathoFreres(iterRoot, &Vect) ;

    if (false == Vect.empty())
    {
      PatPathoIterVect iterVect = Vect.begin() ;
      while (Vect.end() != iterVect)
      {
        // Get the sub-patpatho to remove root node
        //
        NSPatPathoArray *pSubPpt = new NSPatPathoArray(pNSFct->pContexte->getSuperviseur()) ;
        PatPathoIter iter = (*iterVect)->begin() ;
        iter++ ;
        while ((*iterVect)->end() != iter)
        {
          int iColonne = (*iter)->getColonne() ;
          int iLigne   = (*iter)->getLigne() ;

          NSPatPathoInfo* pPatPatho = new NSPatPathoInfo() ;
          *(pPatPatho->getDataTank()) = *((*iter)->getDataTank()) ;

          pPatPatho->setColonne(iColonne - 1) ;
          pPatPatho->setLigne(iLigne - 1) ;

          pSubPpt->push_back(pPatPatho) ;

          iter++ ;
        }
        pTransfert->getPatPatho()->push_back(new NSFatheredPatPathoArray(pNSFct->pContexte->getSuperviseur(), 0, pSubPpt)) ;

        iterVect++ ;
      }
    }
  }

  pTransfert->Active() ;
}
catch (...)
{
	erreur("Exception CheckSurgicalHistoryChapter.", standardError, 0) ;
	return ;
}
}

// Search for a single familly history element in a previous report or in synthesis
//
void CheckFamilyHistory(NSDlgFonction *pNSFct, string sCutConcept)
{
  if (((NSDlgFonction*) NULL == pNSFct) ||
      ((NSControle*)    NULL == pNSFct->getControl()))
		return ;

  // Is the control already "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (1 == pTransfert->getActif()))
		return ;

  // Get element's path beyond "personal history" (QANTF)
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFils)
		return ;

  // Find such an element in a previous report
  //
  string sElementPath = pBBFils->getLocalisation() ;

  NSSearchStruct searchStruct ;
  bool bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sElementPath, &searchStruct) ;
  if ((bOk) && (false == searchStruct.isEmpty()))
  {
    pTransfert->Active() ;
    return ;
  }

  // Find such an element in the synthesis
  //
  size_t iQuantcPos = sElementPath.find(sCutConcept) ;
  if (NPOS == iQuantcPos)
    return ;

  size_t iElmtPathLen = strlen(sElementPath.c_str()) ;
  string sDetails = string("") ;
  iQuantcPos = sElementPath.find(cheminSeparationMARK, iQuantcPos + 1) ;
  if ((NPOS != iQuantcPos) && (iQuantcPos < iElmtPathLen - 1))
    sDetails = string(sElementPath, iQuantcPos + 1, iElmtPathLen - iQuantcPos - 1) ;

  string sSynthesisPath = string("ZSYNT/QANTF/") + sDetails ;

  NSSearchStruct searchStruct2 ;
  bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sSynthesisPath, &searchStruct2) ;
  if ((bOk) && (false == searchStruct2.isEmpty()))
    pTransfert->Active() ;
}

// Search, in a previous report or in synthesis, for all elements from the chapter
//
void CheckFamilyHistoryChapter(NSDlgFonction *pNSFct, string sCutConcept)
{
  if (((NSDlgFonction*) NULL == pNSFct) ||
      ((NSControle*)    NULL == pNSFct->getControl()))
		return ;

  // Is the control already "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  // if (((NSTransferInfo*) NULL == pTransfert) || (1 == pTransfert->getActif()))
  if ((NSTransferInfo*) NULL == pTransfert)
    return ;

  // Get element's path beyond "personal history" (QANTF)
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFils)
		return ;

  if (pBBFils->VectorFils.empty())
    return ;

  BBiterFils petitFils1 = pBBFils->VectorFils.begin() ;

  NSContexte* pContexte = pNSFct->pContexte ;

  // Find such an element in a previous report
  //
  string sElementPath = pBBFils->getLocalisation() ;

  CheckElementsForPath(sElementPath, *petitFils1) ;

  // Find the proper chapter in synthesis
  //
  NSSearchStruct searchStruct ;
  string sSynthesisPath = string("ZSYNT/QANTF") ;

  CheckElementsForPath(sSynthesisPath, *petitFils1) ;
}

// Search, in a previous report or in synthesis, for all elements from the chapter
//
void CheckElementsForPath(string sChapterPath, BBItem* pItem)
{
  if (((BBItem*) NULL == pItem) || (string("") == sChapterPath))
		return ;

  if (pItem->_aBBItemFils.empty())
    return ;

  NSContexte* pContexte = pItem->pContexte ;

  NSSearchStruct searchStruct ;
  bool bOk = pContexte->getPatient()->ChercheChemin(sChapterPath, &searchStruct) ;

  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  // Explore results
  //
  MappingNSSearchResult::MMapIt it = searchStruct.getFoundNodes()->begin() ;
  for ( ; searchStruct.getFoundNodes()->end() != it ; it++)
  {
    std::string date   = string("") ;
    std::string sNoeud = string("") ;

    searchStruct.getFoundNodes()->fullRData(it, date, sNoeud) ;

    // Get the patpatho for this result
    //
    NSPatPathoArray Ppt(pContexte->getSuperviseur()) ;

    pContexte->getPatient()->DonneArray(sNoeud, &Ppt) ;
    if (Ppt.empty())
      return ;

    // For each tree element, check if there is a corresponding control
    //
    for (PatPathoIter iter = Ppt.begin() ; Ppt.end() != iter ;  iter++)
    {
      // Get node label
      //
      string sElemLex = (*iter)->getLexique() ;
      string sSens    = string("") ;
      pContexte->getDico()->donneCodeSens(&sElemLex, &sSens) ;

      BBiter iterChampEdit = pItem->_aBBItemFils.begin() ;
      for ( ; pItem->_aBBItemFils.end() != iterChampEdit ; iterChampEdit++)
      {
        string sLabel     = (*iterChampEdit)->getItemLabel() ;
        string sLabelSens = string("") ;
        pContexte->getDico()->donneCodeSens(&sLabel, &sLabelSens) ;

        if (sSens == sLabelSens)
          (*iterChampEdit)->Active() ;
      }
    }
  }
}

void RecordInSynthesis(NSDlgFonction *pNSFct, string sContextPath, string sCutConcept)
{
  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTrace = string("Entering RecordInSynthesis (") + sContextPath + string(" : ") + sCutConcept + string(")") ;
  pSuper->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  if (((NSDlgFonction*) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  // Is the control "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (false == pTransfert->isActif()))
		return ;

  NSVectFatheredPatPathoArray* pTransPatpatho = pTransfert->getPatPatho() ;
  if (((NSVectFatheredPatPathoArray*) NULL == pTransPatpatho) ||
                                                     pTransPatpatho->estVide())
    return ;

  // Get synthesis document
  //
  NSDocumentHisto* pSynthDoc = pNSFct->pContexte->getPatient()->GetSynthesisDocument() ;
  if ((NSDocumentHisto*) NULL == pSynthDoc)
    return ;

  BBFilsItem*	pBBFilsItem = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFilsItem)
    return ;

  string sItemLabel = pBBFilsItem->getItemLabel() ;

  // NSNoyauDocument* pDocNoy = pSynthDoc->pDocNoy ;

  // If document is opened (pDocNoy not NULL)
  //
  NSHISTODocument* pHistor = pNSFct->pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pHistor)
    return ;

  NSNoyauDocument* pDocNoy = pHistor->EstUnDocumentOuvert((NSDocumentInfo*) pSynthDoc) ;
  if (pDocNoy)
  {
    // iterating into this document's views
    //
    TView* pView = pDocNoy->GetViewList() ;
    for ( ; pView && (string("NSCsVue") != string(pView->GetViewName())) ; pView = pView->GetNextView())
      ;

    // A view has been found, we use it to add/refresh information
    //
    if (pView)
    {
      // ----- NSCsVue is not accessible here ------
      // NSCsVue* pCsView = dynamic_cast<NSCsVue*>(pView) ;
      // NSTreeWindow* pTreeWin = (NSTreeWindow*)(pCsView) ;
      NSTreeWindow* pTreeWin = dynamic_cast<NSTreeWindow*>(pView) ;
      if (pTreeWin)      {
        // Build the patpatho to be synchronized with existing one
        //
        NSPatPathoArray newPpt(pSuper) ;

        Message Msg ;
        newPpt.parseBlock(sItemLabel, &Msg) ;
        newPpt.ajoutePatho(Msg.GetLexique(), &Msg, 0) ;
        for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
          newPpt.InserePatPathoFille(newPpt.begin(), (*vectIt)->getPatPatho()) ;

        pTreeWin->SynchroInformationForPath(sContextPath, &newPpt, string(1, cheminSeparationMARK)) ;

        NSToDoTask* pTask = new NSToDoTask ;
        pTask->setWhatToDo(string("SaveTreeviewInformation")) ;
        pTask->setPointer1(pTreeWin, false /* mustDelete */) ;
        pTask->sendMe(pSuper) ;

        return ;

        // Looking for the node that fits context path
        //
        NSTreeNode* pNode = pTreeWin->GetNodeForPath(sContextPath, string(1, cheminSeparationMARK)) ;
        if (pNode)
        {
          if (pNode->estFictif())
            pTreeWin->NodeRemoteValidation(pNode) ;

          if (pNode->estFictif())
            return ;

          // Synchronize existing information with incoming patpatho
          //
          NSCutPaste CutPaste(pSuper) ;
          CutPaste.setPatPatho(&newPpt) ;
          // pTreeWin->DragFils(pNode, string("DRAG"), &CutPaste) ;
          pTreeWin->DragFils(pNode, string("SYNCH"), &CutPaste) ;

/*
          for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
          {
            *(CutPaste.pPatPatho) = *((*vectIt)->getPatPatho()) ;
            pTreeWin->DragFils(pNode, string("DRAG"), &CutPaste) ;
          }
*/
        }
        else
        {
          NSTreeNode* pRootNode = (NSTreeNode*) 0 ;
          if (false == pTreeWin->_NodeArray.empty())
            pRootNode = *(pTreeWin->_NodeArray.begin()) ;

          NSCutPaste CutPaste(pSuper) ;
          CutPaste.setPatPatho(&newPpt) ;
          pTreeWin->DragFils(pRootNode, string("SYNCH"), &CutPaste) ;
        }

        // Save changes
        //
        if ((pTreeWin->GetDirty()) &&
            (pTreeWin->getControl()) &&
            (pTreeWin->getControl()->getTransfert()) &&
            (pTreeWin->getControl()->getTransfert()->getFilsItem()))
        {
          BBFilsItem* pFilsItem = pTreeWin->getControl()->getTransfert()->getFilsItem() ;
          NSSmallBrother*	pBigBoss = pFilsItem->pBigBoss ;

          pTreeWin->okFermerDialogue() ;
          pTreeWin->EnregistrePatPatho(pBigBoss) ;
          pBigBoss->MetTitre() ;

          NSNoyauDocument noyauDoc(0, pSynthDoc, 0, pNSFct->pContexte, false, true) ;
          noyauDoc.setPatPatho(pBigBoss->getPatPatho()) ;

          noyauDoc.enregistrePatPatho() ;

          NSPatPathoArray PptNoyau(pSuper) ;
          noyauDoc.initFromPatPatho(&PptNoyau) ;
          pSynthDoc->setPatPatho(&PptNoyau) ;

          pTreeWin->SetDirty(false) ;

          NSToDoTask* pTask = new NSToDoTask ;

          pTask->setWhatToDo(string("RefreshSynthesisInformation")) ;
          pTask->setParam1(string("NEW_DATA")) ;

          pTask->sendMe(pSuper) ;
        }

        return ;
      }
    }
  }

  NSPatPathoArray synthesisPpt(pNSFct->pContexte->getSuperviseur()) ;
  pNSFct->pContexte->getPatient()->DonnePathoSynthese(&synthesisPpt) ;
  if (synthesisPpt.empty())
    return ;

  string sElementPathInSynthesis = sContextPath + string(1, cheminSeparationMARK) + sItemLabel ;

  PatPathoIter synthesisIter ;
  bool bFound = synthesisPpt.CheminDansPatpatho(sElementPathInSynthesis, string(1, cheminSeparationMARK), &synthesisIter) ;

  if (false == bFound)
  {
    synthesisPpt.AjouterChemin(sContextPath, sItemLabel, true, string(1, cheminSeparationMARK)) ;

    if (false == synthesisPpt.CheminDansPatpatho(sElementPathInSynthesis, string(1, cheminSeparationMARK), &synthesisIter))
      return ;

    for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
      synthesisPpt.InserePatPathoFille(synthesisIter, (*vectIt)->getPatPatho()) ;
  }

  // Save modifications
  //
  NSNoyauDocument noyauDoc(0, pSynthDoc, 0, pNSFct->pContexte, false, true) ;
  noyauDoc.setPatPatho(&synthesisPpt) ;

  noyauDoc.enregistrePatPatho() ;

  NSPatPathoArray PptNoyau(pNSFct->pContexte->getSuperviseur()) ;
  noyauDoc.initFromPatPatho(&PptNoyau) ;
  pSynthDoc->setPatPatho(&PptNoyau) ;

  // Refresh view(s)
  //
  NSToDoTask* pTask = new NSToDoTask ;
  pTask->setWhatToDo(string("RefreshSynthesisInformation")) ;
  pTask->setParam1(string("NEW_DATA")) ;
  pTask->sendMe(pSuper) ;

/*
  if (pDocNoy)
  {
    // iterating into this document's views
    //
    TView* pView = pDocNoy->GetViewList() ;
    for ( ; NULL != pView ; pView = pView->GetNextView())
    {
      TWindowView* pWindow = static_cast<TWindowView*>(pView) ;
      if (pWindow)
		    pWindow->Invalidate() ;
    }
  }
*/
}

void RecordInSynthesis2(NSDlgFonction *pNSFct, HistoryConnector* pHC)
{
  if (((NSDlgFonction*)    NULL == pNSFct) ||
      ((NSControle*)       NULL == pNSFct->getControl()) ||
      ((HistoryConnector*) NULL == pHC))
		return ;

  NSSuper *pSuper = pNSFct->pContexte->getSuperviseur() ;

  string sTrace = string("Entering RecordInSynthesis2") ;
  pSuper->trace(&sTrace, 1, NSSuper::trSubSteps) ;

  string sContextPath = pHC->getHistoryPath() ;

  // Is the control "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (false == pTransfert->isActif()))
		return ;

  NSVectFatheredPatPathoArray* pTransPatpatho = pTransfert->getPatPatho() ;

  // It is legal to have a concept with no detail
  // if (((NSVectFatheredPatPathoArray*) NULL == pTransPatpatho) || pTransPatpatho->estVide())
  //
  if (((NSVectFatheredPatPathoArray*) NULL == pTransPatpatho) || pTransPatpatho->empty())
    return ;

  // Get synthesis document
  //
  NSDocumentHisto* pSynthDoc = pNSFct->pContexte->getPatient()->GetSynthesisDocument() ;
  if ((NSDocumentHisto*) NULL == pSynthDoc)
    return ;

  BBFilsItem*	pBBFilsItem = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFilsItem)
    return ;

  string sItemLabel = pBBFilsItem->getItemLabel() ;

  // If document is opened (pDocNoy not NULL)
  //
  NSHISTODocument* pHistor = pNSFct->pContexte->getPatient()->getDocHis() ;
  if ((NSHISTODocument*) NULL == pHistor)
    return ;

  NSNoyauDocument* pDocNoy = pHistor->EstUnDocumentOuvert((NSDocumentInfo*) pSynthDoc) ;
  if (pDocNoy)
  {
    // iterating into this document's views
    //
    TView* pView = pDocNoy->GetViewList() ;
    for ( ; (NULL != pView) && (string("NSCsVue") != string(pView->GetViewName())) ; pView = pView->GetNextView())
      ;

    // A view has been found, we use it to add/refresh information
    //
    if (pView)
    {
      // NSCsVue* pCsView = dynamic_cast<NSCsVue*>(pView) ;
      // NSTreeWindow* pTreeWin = (NSTreeWindow*)(pCsView) ;
      NSTreeWindow* pTreeWin = dynamic_cast<NSTreeWindow*>(pView) ;
      if (pTreeWin)      {
        pTreeWin->SynchroInformationForPath(sContextPath, sItemLabel, pDocNoy->getDocId(), pTransPatpatho, pHC, string(1, cheminSeparationMARK)) ;

        return ;


        // Looking for the node that fits context path
        //
        NSTreeNode* pNode = pTreeWin->GetNodeForPath(sContextPath, string(1, cheminSeparationMARK)) ;
        if (pNode)
        {
          if (pNode->estFictif())
            pTreeWin->NodeRemoteValidation(pNode) ;

          if (pNode->estFictif())
            return ;

          for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
          {
            NSPatPathoInfo* pFatherItem = (*vectIt)->getFatherNode() ;
            NSTreeNode*     pFatherNode = (NSTreeNode*) 0 ;

            // Looking for a node in synthesis this information is a model of
            //
            VecteurString aNodes ;
            string sModelNodeId = getModelNodeFromDoc(pFatherItem, pDocNoy->getDocId(), pNSFct->pContexte, &aNodes) ;
            if (string("") != sModelNodeId)
            {
              EquiItemIter vectIter = aNodes.begin() ;
              for ( ; (aNodes.end() != vectIter) && (NULL == pFatherNode) ; vectIter++)
                pFatherNode = pTreeWin->GetNSTreeNode(**vectIter) ;
            }

/*
            if (pFatherItem && pFatherItem->pTemporaryLinks &&
                              (false == pFatherItem->pTemporaryLinks->empty()))
            {
              for (NSLinkedNodeIter i = pFatherItem->pTemporaryLinks->begin() ;
                               (pFatherItem->pTemporaryLinks->end() != i) &&
                               (NULL == pFatherNode) ; i++)
	            {
                if ((dirFleche == (*i)->iLinkDirection) &&
                    (NSRootLink::copyOf == (*i)->iLinkType))
                {
                  string sCopyNodeId = (*i)->sOtherNode ;
                  pFatherNode = pTreeWin->GetNSTreeNode(sCopyNodeId) ;
                }
              }
            }
*/

            // Build patpatho
            //
            NSPatPathoArray newPpt(pSuper) ;
            Message ConceptMsg ;
            newPpt.parseBlock(sItemLabel, &ConceptMsg) ;
            newPpt.ajoutePatho(ConceptMsg.GetLexique(), &ConceptMsg, 0) ;
            newPpt.InserePatPathoFille(newPpt.begin(), (*vectIt)->getPatPatho()) ;
            newPpt.clearAllIDs() ;

            // No existing father node: must create new nodes
            //
            if ((NSTreeNode*) NULL == pFatherNode)
            {
              // Insert node in synthesis
              //
              NSCutPaste CutPaste(pSuper) ;
              CutPaste.setPatPatho(&newPpt) ;
              CutPaste.setUnique(pHC->isUnique()) ;
              pTreeWin->DragFils(pNode, string("COPIE"), &CutPaste) ;

              pNode->ExpandTousNoeud() ;

              // Save synthesis
              //
              SaveSynthesis(pTreeWin, pSynthDoc) ;

              // Looking for the node that fits context path
              //
              NSTreeNode* pNode = pTreeWin->GetNodeForPath(sContextPath, string(1, cheminSeparationMARK)) ;
              if (pNode)
              {
                // Find back just connexted node in order to set a "isCopy" link
                //
                if (false == pNode->getVectSons()->empty())
                {
                  NSTreeNode *pNewNode = pNode->getVectSons()->back() ;
                  if ((NSPatPathoInfo*) NULL == pFatherItem)
                  {
                    NSPatPathoInfo pptInfo ;
                    pptInfo.setLexique(ConceptMsg.GetLexique()) ;
                    pptInfo.setUnit(ConceptMsg.GetUnit()) ;
                    pptInfo.setPluriel(ConceptMsg.GetPluriel()) ;
                    pptInfo.setCertitude(ConceptMsg.GetCertitude()) ;
                    pptInfo.setComplement(ConceptMsg.GetComplement()) ;
                    (*vectIt)->referenceFatherNode(&pptInfo) ;
                    pFatherItem = (*vectIt)->getFatherNode() ;
                  }
                  if (pFatherItem)
                  {
                    string sNewNodeId = pNewNode->getNodeId() ;
                    if (string("") != sNewNodeId)
                      pFatherItem->addTemporaryLink(sNewNodeId, NSRootLink::copyOf, dirFleche) ;
                  }
                }
              }
            }
            //
            // Existing father node: must merge
            //
            else
            {
              // Check if there is something to update
              //
              NSCutPaste TestEqualCutPaste(pSuper) ;
              pFatherNode->formerPatPatho(&TestEqualCutPaste) ;

              NSPatPathoArray TestEqual(pSuper) ;
              TestEqualCutPaste.initFromPatPatho(&TestEqual) ;

              if (false == newPpt.hasSameContent(&TestEqual))
              {
                NSCutPaste CutPaste(pSuper) ;
                CutPaste.setPatPatho(&newPpt) ;
                CutPaste.setUnique(pHC->isUnique()) ;
                pTreeWin->DragFils(pFatherNode, string("SYNCH"), &CutPaste) ;

                pFatherNode->ExpandTousNoeud() ;

                if ((pHC->isUnique()) && (false == pHC->getSubPathArray()->empty()))
                {
                  // Next, remove all paths from this that are in the list and don't appear in Merging
                  //
                  vector<string*>::iterator it = (string**) pHC->getSubPathArray()->begin() ;
                  for ( ; pHC->getSubPathArray()->end() != it ; it++)
                  {
                    string sIt = **it ;

                    string sSubPath = sContextPath ;
                    if (string("") != sIt)
                      sSubPath += string(1, cheminSeparationMARK) + sIt ;

                    if (string("") != sIt)
                    {
                      NSTreeNode* pPathNode = pTreeWin->GetNodeForPath(sSubPath, string(1, cheminSeparationMARK)) ;
                      if (pPathNode && (false == newPpt.CheminDansPatpatho(sIt)))
                        pTreeWin->CouperNode(pPathNode) ;
                    }
                  }
                }

                // Save synthesis
                //
                SaveSynthesis(pTreeWin, pSynthDoc) ;
              }
            }

            // newPpt.InserePatPathoFille(newPpt.begin(), (*vectIt)->getPatPatho()) ;
          }

/*
          // Synchronize existing information with incoming patpatho
          //
          NSCutPaste CutPaste(pNSFct->pContexte->getSuperviseur()) ;
          *(CutPaste.pPatPatho) = newPpt ;
          CutPaste.setUnique(pHC->isUnique()) ;
          pTreeWin->DragFils(pNode, string("SYNCH"), &CutPaste) ;

          if ((pHC->isUnique()) && (false == pHC->getSubPathArray()->empty()))
          {
            // Next, remove all paths from this that are in the list and don't appear in Merging
            //
            vector<string*>::iterator it = (string**) pHC->getSubPathArray()->begin() ;
            for ( ; pHC->getSubPathArray()->end() != it ; it++)
            {
              string sIt = **it ;

              string sSubPath = sContextPath ;
              if (string("") != sIt)
                sSubPath += string(1, cheminSeparationMARK) + sIt ;

              if (string("") != sIt)
              {
                NSTreeNode* pPathNode = pTreeWin->GetNodeForPath(sSubPath, string(1, cheminSeparationMARK)) ;
                if (pPathNode && (false == newPpt.CheminDansPatpatho(sIt)))
                  pTreeWin->CouperNode(pPathNode) ;
              }
            }
          }
*/

/*
          // Save changes
          //
          if ((pTreeWin->GetDirty()) &&
              (pTreeWin->pControle) &&
              (pTreeWin->pControle->getTransfert()) &&
              (pTreeWin->pControle->getTransfert()->pBBFilsItem))
          {
            BBFilsItem* pFilsItem = pTreeWin->pControle->getTransfert()->pBBFilsItem ;
            NSSmallBrother*	pBigBoss = pFilsItem->pBigBoss ;

            pTreeWin->okFermerDialogue() ;
            pTreeWin->EnregistrePatPatho(pBigBoss) ;
            pBigBoss->MetTitre() ;

            NSNoyauDocument noyauDoc(0, pSynthDoc, 0, pNSFct->pContexte, false) ;
            *(noyauDoc.pPatPathoArray) = *(pBigBoss->getPatPatho()) ;
            noyauDoc.enregistrePatPatho() ;

            *(pSynthDoc->pPatPathoArray) = *(noyauDoc.pPatPathoArray) ;

            pTreeWin->SetDirty(false) ;
          }
*/

          return ;
        }
      }
    }
  }

  NSPatPathoArray synthesisPpt(pSuper) ;
  pNSFct->pContexte->getPatient()->DonnePathoSynthese(&synthesisPpt) ;
  if (synthesisPpt.empty())
    return ;

  string sElementPathInSynthesis = sContextPath + string(1, cheminSeparationMARK) + sItemLabel ;

  PatPathoIter synthesisIter ;
  bool bFound = synthesisPpt.CheminDansPatpatho(sElementPathInSynthesis, string(1, cheminSeparationMARK), &synthesisIter) ;

  if (false == bFound)
  {
    synthesisPpt.AjouterChemin(sContextPath, sItemLabel, true, string(1, cheminSeparationMARK)) ;

    if (false == synthesisPpt.CheminDansPatpatho(sElementPathInSynthesis, string(1, cheminSeparationMARK), &synthesisIter))
      return ;

    for (FatheredPatPathoIterVect vectIt = pTransPatpatho->begin() ; pTransPatpatho->end() != vectIt ; vectIt++)
      synthesisPpt.InserePatPathoFille(synthesisIter, (*vectIt)->getPatPatho()) ;
  }

  NSNoyauDocument noyauDoc(0, pSynthDoc, 0, pNSFct->pContexte, false, true) ;
  noyauDoc.setPatPatho(&synthesisPpt) ;

  noyauDoc.enregistrePatPatho() ;

  NSPatPathoArray PptNoyau(pSuper) ;
  noyauDoc.initFromPatPatho(&PptNoyau) ;
  pSynthDoc->setPatPatho(&PptNoyau) ;

  NSToDoTask* pTask = new NSToDoTask ;

  pTask->setWhatToDo(string("RefreshSynthesisInformation")) ;
  pTask->setParam1(string("NEW_DATA")) ;

  pTask->sendMe(pSuper) ;

  if (pDocNoy)
  {
    // iterating into this document's views
    //
    TView* pView = pDocNoy->GetViewList() ;
    for ( ; NULL != pView ; pView = pView->GetNextView())
    {
      TWindowView* pWindow = static_cast<TWindowView*>(pView) ;
      if (pWindow)
		    pWindow->Invalidate() ;
    }
  }
}

void SaveSynthesis(NSTreeWindow* pTreeWin, NSDocumentHisto* pSynthDoc)
{
  if (((NSTreeWindow*) NULL == pTreeWin) || ((NSDocumentHisto*) NULL == pSynthDoc))
    return ;

  if (((NSControle*) NULL == pTreeWin->getControl()) ||
      (NULL == pTreeWin->getControl()->getTransfert()))
    return ;

  BBFilsItem* pFilsItem = pTreeWin->getControl()->getTransfert()->getFilsItem() ;
  if ((BBFilsItem*) NULL == pFilsItem)
    return ;

  NSSmallBrother*	pBigBoss = pFilsItem->pBigBoss ;
  if ((NSSmallBrother*) NULL == pBigBoss)
    return ;

  // Build patpatho
  //
  pTreeWin->okFermerDialogue() ;
  pTreeWin->EnregistrePatPatho(pBigBoss) ;
  pBigBoss->MetTitre() ;

  // Prepare a document for database record
  //
  NSNoyauDocument noyauDoc(0, pSynthDoc, 0, pBigBoss->pContexte, false, true) ;
  noyauDoc.setPatPatho(pBigBoss->getPatPatho()) ;

  noyauDoc.enregistrePatPatho() ;

  // Reinject saved patpatho
  //
  noyauDoc.chargePatPatho() ;

  NSPatPathoArray PptNoyau(pBigBoss->pContexte->getSuperviseur()) ;
  noyauDoc.initFromPatPatho(&PptNoyau) ;
  pSynthDoc->setPatPatho(&PptNoyau) ;

  pTreeWin->SetNodesIdFromPatho(&PptNoyau) ;

  pTreeWin->SetDirty(false) ;
}

// Returns first node (if exists) that belongs to a model document (refered by
// sModelDocId) and is linked to CopyNode by a "copyOf" link.
//
// If pNodes is not NULL, it is filled with the whole list of such nodes
//
string getModelNodeFromDoc(NSPatPathoInfo* pCopyNode, string sModelDocId, NSContexte *pCtx, VecteurString *pNodes)
{
  if ((NSPatPathoInfo*) NULL == pCopyNode)
    return string("") ;

  size_t iDocIdLen = PAT_NSS_LEN + DOC_CODE_DOCUM_LEN ;

  VecteurString aNodes ;

  // First, have a look in temporary nodes - for newly created nodes
  //
  NSLinkedNodeArray* pTempoLinks = pCopyNode->getTemporaryLinks() ;
  if (pTempoLinks && (false == pTempoLinks->empty()))
  {
    for (NSLinkedNodeIter i = pTempoLinks->begin() ; (pTempoLinks->end() != i) ; i++)
    {
      if ((dirFleche == (*i)->iLinkDirection) &&
                                      (NSRootLink::copyOf == (*i)->iLinkType))
      {
        string sCopyNodeId = (*i)->sOtherNode ;
        if (strlen(sCopyNodeId.c_str()) > iDocIdLen)
        {
          string sNodeDocId = string(sCopyNodeId, 0, iDocIdLen) ;
          if ((sNodeDocId == sModelDocId) && (false == aNodes.ItemDansUnVecteur(sCopyNodeId)))
            aNodes.AddString(sCopyNodeId) ;
        }
      }
    }
  }

  if (false == aNodes.empty())
  {
    if (pNodes)
      *pNodes = aNodes ;

    return *(*(aNodes.begin())) ;
  }

  // Now, have a look in graph
  //
  string sCopyNodeId = pCopyNode->getNode() ;
  if (string("") == sCopyNodeId)
    return string("") ;

  if (((NSContexte*) NULL == pCtx) || (NULL == pCtx->getPatient()))
    return string("") ;

  NSPersonGraphManager* pGraphPerson = pCtx->getPatient()->getGraphPerson() ;
  if ((NSPersonGraphManager*) NULL == pGraphPerson)
    return string("") ;

  NSLinkManager* pLinkManager = pGraphPerson->getLinkManager() ;
  if ((NSLinkManager*) NULL == pLinkManager)
    return string("") ;

  VecteurString VString ;
  pLinkManager->TousLesVrais(sCopyNodeId, NSRootLink::copyOf, &aNodes, "FLECHE") ;

  if (aNodes.empty())
    return string("") ;

  if (pNodes)
    *pNodes = aNodes ;

  return *(*(aNodes.begin())) ;
}

void CheckChronicConcern(NSDlgFonction *pNSFct)
{
try
{
  if (((NSDlgFonction*) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  // Is the control already "activated"?
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (true == pTransfert->isActif()))
		return ;

  // Get element's path
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFils)
		return ;

  string sElementPath = pBBFils->getLocalisation() ;

  // Find such an element in a previous report
  //
  NSSearchStruct searchStruct ;
  bool bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sElementPath, &searchStruct) ;

  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
  {
    // Find such an element in health index
    //
    string sHealthIndexPath = string("ZPOMR/0PRO1/") + pBBFils->getItemLabel() ;

    bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sHealthIndexPath, &searchStruct) ;
  }

  if ((false == bOk) || (NSSearchStruct::foundNothing == searchStruct.getFoundStatus()))
    return ;

  pTransfert->Active() ;

  return ;
}
catch (...)
{
	erreur("Exception CheckChronicConcern.", standardError, 0) ;
	return ;
}
}

void RecordChronicConcernInHealthIndex(NSDlgFonction *pNSFct)
{
  if (((NSDlgFonction *) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  // Is the control "activated"? If not, there is nothing to get done
  //
  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (false == pTransfert->isActif()))
		return ;

  // Get element's path
  //
  BBFilsItem*	pBBFils = pTransfert->getFilsItem() ;
  if ((BBFilsItem*) NULL == pBBFils)
		return ;

  string sElementPath = pBBFils->getLocalisation() ;

  // Find such an element in a previous report
  //
  NSSearchStruct searchStruct ;
  string sHealthIndexPath = string("ZPOMR/0PRO1/") + pBBFils->getItemLabel() ;
  bool bOk = pNSFct->pContexte->getPatient()->ChercheChemin(sHealthIndexPath, &searchStruct) ;

  // If element found in health index, there is nothing to do
  //
  if (bOk && (NSSearchStruct::foundNothing != searchStruct.getFoundStatus()))
    return ;

  CreatePreoccup(pNSFct) ;
}

void InitTitleOfMaterial(NSDlgFonction * /* pNSFct */)
{
/*
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;

  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
	if ((NULL == pTransfert) || (NULL == pTransfert->pBBFilsItem))
		return ;

  BBFilsItem* pFilsItem = pTransfert->pBBFilsItem ;
  if (NULL == pFilsItem)
  	return ;

  NSButton* pNSButton = static_cast<NSButton*>(pTransfert->pControle->getControle()) ;
  if (NULL == pNSButton)
		return ;

	string sCode = pFilsItem->sEtiquette ;
  if (string("") == sCode)
    return ;

  NSSmallBrother* pBigBoss = pNSFct->getItem()->pBigBoss ;

	NSPatPathoArray* pPPT = pBigBoss->getPatPatho() ;
  if (NULL == pPPT)
		return ;

	PatPathoIter iter = pPPT->ChercherItem(sCode) ;
	if (pPPT->end() == iter)
		return ;

	string sObjectId = (*iter)->getComplement() ;
  if (string("") == sObjectId)
		return ;

	NSMaterielInfo Materiel ;
  Materiel.initialiseDepuisObjet(pBigBoss->pContexte, sObjectId) ;

	string sNom = Materiel.pDonnees->libelle_complet ;
	if (sNom != "")
	{
  	string sNumSerie = string(Materiel.pDonnees->num_serie) ;
    if (sNumSerie != "")
    	sNom += " n°" + sNumSerie ;
	}

	if (sNom != "")
		pNSButton->SetCaption(sNom.c_str()) ;
	else
		pNSButton->SetCaption("Préciser l'endoscope") ;
*/
}

void SelectMaterial(NSDlgFonction *pNSFct)
{
  if ((NULL == pNSFct) || (NULL == pNSFct->getControl()))
		return ;
}

void InitTitleOfPerson(NSDlgFonction *pNSFct)
{
  if (((NSDlgFonction*) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

  NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (((NSTransferInfo*) NULL == pTransfert) || (NULL == pTransfert->getFilsItem()) || (NULL == pTransfert->getControl()->getControle()))
		return ;

	NSButton* pNSButton = static_cast<NSButton*>(pTransfert->getControl()->getControle()) ;
  if ((NSButton*) NULL == pNSButton)
		return ;

  NSSmallBrother* pBigBoss = pNSFct->getItem()->_pBigBoss ;
  if ((NSSmallBrother*) NULL == pBigBoss)
    return ;

	string sCode = pTransfert->getFilsItem()->getItemLabel() ;
  if (string("") == sCode)
    return ;

  string sCorrespId = string("") ;

  if (pTransfert->getTransfertMessage() && (string("") != pTransfert->getTransfertMessage()->GetComplement()))
    sCorrespId = pTransfert->getTransfertMessage()->GetComplement() ;
  else
  {
	  NSPatPathoArray* pPPT = pBigBoss->getPatPatho() ;
	  if ((NSPatPathoArray*) NULL == pPPT)
  	  return ;

	  //
	  // chercher dans pPPT sCode
	  //
	  PatPathoIter iter = pPPT->ChercherItem(sCode) ;
	  if (((PatPathoIter) NULL == iter) || (pPPT->end() == iter))
		  return ;

	  sCorrespId = (*iter)->getComplement() ;
    if (string("") == sCorrespId)
		  return ;
  }

	NSPersonInfo* pPersonInfo = pBigBoss->pContexte->getPersonArray()->getPerson(pBigBoss->pContexte, sCorrespId, pidsCorresp) ;
  if ((NSPersonInfo*) NULL == pPersonInfo)
  	return ;

	string sIdentite = pPersonInfo->getNom() + " " + pPersonInfo->getPrenom() ;

	if (string("") != sIdentite)
		pNSButton->SetCaption(sIdentite.c_str()) ;
	else
		pNSButton->SetCaption("Préciser l'anesthésiste") ;
}

void SelectPerson(NSDlgFonction *pNSFct)
{
  if (((NSDlgFonction*) NULL == pNSFct) || ((NSControle*) NULL == pNSFct->getControl()))
		return ;

	NSTransferInfo* pTransfert = pNSFct->getControl()->getTransfert() ;
  if (NULL == pTransfert)
  	return ;

  if ((NULL == pNSFct->getItem()) || (NULL == pNSFct->getItem()->_pBigBoss))
		return ;
  NSContexte* pContexte = pNSFct->getItem()->_pBigBoss->pContexte ;

	string sSpecialite = "DANES1" ; //code lexique de l'anesthésiste

	NSTPersonListDialog Liste(pNSFct->getControl()->getInterfacePointer(),
                                        pidsCorresp, false, pContexte) ;

	if (IDOK != Liste.Execute())
		return ;

	if (string("") == Liste.pPersonSelect->getNss())
		return ;

	pTransfert->getTransfertMessage()->SetComplement(Liste.pPersonSelect->getNss()) ;
	pTransfert->getFilsItem()->Active() ;
	string sIdentite = Liste.pPersonSelect->getNom() + " " +
                                Liste.pPersonSelect->getPrenom() ;

	NSButton* pNSButton = static_cast<NSButton*>(pTransfert->getControl()->getControle()) ;
  if (NULL == pNSButton)
		return ;

	if (sIdentite != "")
		pNSButton->SetCaption(sIdentite.c_str()) ;
	else
		pNSButton->SetCaption("Préciser l'anesthésiste") ;
}

// Fonction d'énumération pour la mise à jour des champs Static
bool FAR PASCAL SetStaticCaption(HWND hWnd, LPARAM lParam)
{
try
{
	char szClassName[30];           // Nom de la classe du contrôle
	char szBuffer[255] ;             // Utilisée pour les fonctions WINDOWS

	memset(szClassName, 0, 30) ;
	memset(szBuffer,    0, 255) ;

	HWND    hDlg            = NULL ;    // Handle de la boîte de dialogue Parent.
	UINT    nMaxInput       = 255;      // Nombre maximal de caractères à saisir
	//
	// Adresse de l'objet boîte de dialogue courante
	//
	string* pParam = reinterpret_cast<string*>(lParam) ;
  if (NULL == pParam)
		return true ;
	string sParam = *pParam ;

	size_t pos = sParam.find("|") ;
	if (NPOS == pos)
		return true ;

	string sTitle = string(sParam, 0, pos) ;
	string sName = string(sParam, pos+1, strlen(sParam.c_str())-pos-1) ;
	string sCaption ;

	//
	// Obtention de l'ID du contrôle et de son type WINDOWS
	//
	int ctrlID  = ::GetDlgCtrlID(hWnd) ;
	::GetClassName(hWnd, (LPSTR) szClassName, 30) ;
	pseumaj(szClassName) ;
	//
	// Obtention du handle de la boîte de Dialogue
	//
	hDlg = ::GetParent(hWnd) ;

	// -------------------------------------------------------------------
  //                                  Static
  // -------------------------------------------------------------------
  if ((!strcmp(szClassName, "STATIC")) || (!strcmp(szClassName, "BORSTATIC")) ||
      (!strcmp(szClassName, "BORSHADE")) || (!strcmp(szClassName, "BUTTON")))
	{
  	if ((ctrlID != -1) && (ctrlID != 0xFFFF))
    {
    	// Récupération de la longueur du texte du contrôle
      nMaxInput = ::GetDlgItemText(hDlg, ctrlID, szBuffer, nMaxInput) ;
      string sTitleCaption = string(szBuffer) ;
      pos = sTitleCaption.find(" : ") ;
      if (pos != NPOS)
      	sTitleCaption = string(sTitleCaption, 0, pos) ;

      if (sTitleCaption == sTitle)
      {
      	sCaption = sTitle + string(" : ") + sName ;
        ::SetDlgItemText(hDlg, ctrlID, sCaption.c_str()) ;
      }
    }
  }

	return true ;
}
catch (...)
{
	erreur("Exception NSDialog::SetStaticCaption.", standardError, 0) ;
	return false;
}
}

