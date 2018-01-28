// -----------------------------------------------------------------------------
// nsstructure.cpp
// -----------------------------------------------------------------------------
// gestion de structure (au sens Unités Fonctionnelles)
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2016/12/29 16:27:03 $
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

#include "nsutil/nsstructure.h"
#include "nautilus/nssuper.h"
#include "pilot/NautilusPilot.hpp"
#include "nsbb/tagNames.h"
#include "nsbb/nspatpat.h"

NSStructure::NSStructure(string sOID, NSSuper* pSuper)
{
  NSObjectGraphManager GraphObject(pSuper) ;
  GraphObject.setRootObject(sOID) ;

  NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(OBJECT, sOID)) ;

  bool res = pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), GraphObject.getDataGraph(), &AttrList) ;
  if (false == res)
  {
    string 	sMessage = "" ;
    if 	  (pSuper->getPilot()->getErrorMessage() != "")
      sMessage = pSuper->getPilot()->getErrorMessage() ;
    else if (pSuper->getPilot()->getWarningMessage() != "")
      sMessage = pSuper->getPilot()->getWarningMessage() ;
    else
      sMessage = "Echec du service de récupération du graphe représentant une Unité Fonctionnelle." ;
    erreur(sMessage.c_str(), 0, 0) ;
    return ;
  }

  // Initialisation de la structure à partir du graphe de l'objet UF
  NSPatPathoArray PPTUF(pSuper, graphObject) ;
  NSDataTreeIter iterTree ;
  string sElemLex, sSens;

  if (GraphObject.getDataGraph()->getTrees()->ExisteTree("LUNIF1", pSuper, &iterTree))
  {
    (*iterTree)->getRawPatPatho(&PPTUF) ;

    _objectId = sOID ;

    PatPathoIter iter = PPTUF.begin() ;
    int iColBase = (*iter)->getColonne() ;
    iter++ ;

    while ((iter != PPTUF.end()) && ((*iter)->getColonne() > iColBase))
    {
      string sSens = (*iter)->getLexiqueSens() ;

      // nom de l'UF
      if (string("LNOMA") == sSens)
        _structureLabel = PPTUF.getFreeTextValue(&iter) ;
      // ID de l'UF
      else if (string("LIDEG") == sSens)
      {
        _structureId = (*iter)->getComplement() ;
        iter++ ;
      }
      else
        iter++ ;
    }
  }
}

bool
NSStructureArray::init(NSSuper* pSuper)
{
  string ps = string("NSStructureArray::init - process begins") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;

  string serviceName = NautilusPilot::SERV_SEARCH_OBJECT_HAVING_TRAITS ;

  NSPersonsAttributesArray ResultList ;
  NSBasicAttributeArray    AttrArray ;
  AttrArray.push_back(new NSBasicAttribute("trait", UNIF_LIDEG)) ;

	ps = string("NSStructureArray::init - calling pilot service") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;

  bool listOk = pSuper->getPilot()->objectList(serviceName.c_str(), &ResultList, &AttrArray) ;

	ps = string("NSStructureArray::init - pilot called now going to initialize list") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;

  if ((listOk) && (false == ResultList.empty()))
  {
  	for (NSPersonsAttributeIter personAttrIter = ResultList.begin() ; personAttrIter != ResultList.end() ; personAttrIter++)
    {
    	string sObjectID		= string("") ;
      string sStructureID	= string("") ;
      string sLabel				= string("") ;

      for (NSBasicAttributeIter basicAttrIter = (*personAttrIter)->begin() ; basicAttrIter != (*personAttrIter)->end() ; basicAttrIter++)
      {
        if 			((*basicAttrIter)->getBalise() == UNIF_LIDEG)
          sStructureID	= (*basicAttrIter)->getText() ;
        else if ((*basicAttrIter)->getBalise() == UNIF_LNOMA)
          sLabel				= (*basicAttrIter)->getText() ;
        else if ((*basicAttrIter)->getBalise() == OIDS)
        	sObjectID			= (*basicAttrIter)->getText() ;
      }

      if ((sStructureID != "") && (sObjectID != ""))
      	push_back(NSStructure(sLabel, sObjectID, sStructureID, "")) ;
    }
  }
  else
  {
    ps = string("NSStructureArray::init - process ends but no structure were added") ;
    pSuper->trace(&ps, 1, NSSuper::trError) ;

    return false ;
  }

  ps = string("NSStructureArray::init - process ends all is ok") ;
  pSuper->trace(&ps, 1, NSSuper::trError) ;

  return true ;
}

const NSStructure&
NSStructureArray::getStructureFromObjectID(const string sObjectID) const
{
	if (false == empty())
  	for (NSStructureCIter structureIter = begin() ; structureIter != end() ; structureIter++)
    	if (structureIter->getObjectId() == sObjectID)
      	return (*structureIter) ;
}

const NSStructure&
NSStructureArray::getStructureFromStructureID(const string sStructureID) const
{
	if (false == empty())
  	for (NSStructureCIter structureIter = begin() ; structureIter != end() ; structureIter++)
    	if (structureIter->getStructureId() == sStructureID)
      	return (*structureIter) ;
}

const string
NSStructureArray::getObjectIDFromStructureID(const string sStructureID) const
{
	if (false == empty())
  	for (NSStructureCIter structureIter = begin() ; structureIter != end() ; structureIter++)
    {
    	if (structureIter->getStructureId() == sStructureID)
      	return (structureIter->getObjectId()) ;
    }

  return string("") ;
}

const string
NSStructureArray::getStructureIDFromObjectID(const string sObjectID) const
{
	if (false == empty())
  	for (NSStructureCIter structureIter = begin() ; structureIter != end() ; structureIter++)
    {
    	if (structureIter->getObjectId() == sObjectID)
      	return (structureIter->getStructureId()) ;
    }

  return string("") ;
}

