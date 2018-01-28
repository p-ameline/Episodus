// -----------------------------------------------------------------------------
// csvParser.cpp
// -----------------------------------------------------------------------------
// Parseur de fichier CSV
// CSV file parser
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


#include <fstream.h>

#include "partage/nsglobal.h"
#include "nautilus/nssuper.h"
// #include "pilot/NautilusPilot.hpp"
#include "nsbb/tagNames.h"
#include "nssavoir/nsgraphe.h"
#include "nsutil/attvalelem.h"
#include "nsutil/csvparser.h"
#include "nsbb/nsbbtran.h"
#include "nsbb/nspatpat.h"

#define CSVBUF_LEN                  512

#define ETABLISSEMENTCODE           "CODE ETA"
#define ETABLISSEMENTLABEL          "LABEL ETA"
#define CENTRERESPONSABILITECODE    "CODE CRESP"
#define CENTRERESPONSABILITELABEL   "LABEL CRESP"
#define SERVICECODE                 "CODE SERV"
#define SERVICELABEL                "LABEL SERV"
#define CENTREACTIVITECODE          "CODE CACT"
#define CENTREACTIVITELABEL         "LABEL CACT"
#define UFCODE                      "CODE UF"
#define UFLABEL                     "LABEL UF"
#define UFTYPE                      "TYPE"

CSVRecord::CSVRecord(const AttributeValueList& src)
{
	pAVList = new AttributeValueList() ;
  *pAVList = src ;
}


CSVRecord::CSVRecord(const CSVRecord& src)
{
	pAVList = new AttributeValueList() ;
  pAVList = src.pAVList ;
}


CSVRecord::~CSVRecord()
{
	delete pAVList ;
}


CSVRecord&
CSVRecord::operator=(const CSVRecord& src)
{
	pAVList = src.pAVList ;
  return (*this) ;
}


int
CSVRecord::operator==(const CSVRecord& src)
{
	if (pAVList == src.pAVList)
  	return 1 ;
  return 0 ;
}


CSVStructure::CSVStructure(string code, string label, CSVStructure *linkAt, string type)
{
  sCode   = code ;
  sLabel  = label ;
  pLinkAt = linkAt ;
  sFUType = type ;
}


CSVStructure::CSVStructure(const CSVStructure& src)
{
	sCode   = src.sCode ;
  sLabel  = src.sLabel ;
  pLinkAt = src.pLinkAt ;
  sFUType = src.sFUType ;
}


CSVStructure::~CSVStructure()
{
	pLinkAt = NULL ;
}


CSVStructure&
CSVStructure::operator=(const CSVStructure& src)
{
	sCode   = src.sCode ;
  sLabel  = src.sLabel ;
  pLinkAt = src.pLinkAt ;
  sFUType = src.sFUType ;
  return (*this) ;
}

int
CSVStructure::operator==(const CSVStructure& src)
{
	if ((sCode   == src.sCode)    &&
      (sLabel  == src.sLabel)   &&
      (pLinkAt == src.pLinkAt)  &&
      (sFUType == src.sFUType))
  	return 1 ;
  else
  	return 0 ;
}

bool
CSVStructure::createObjectPPT(NSSuper* pSuper, string sUserId, string sOType)
{
	// type des UFs :
  //   * Administratif
  //   * Court Séjour
  //   * Ecole
  //   * Ferme
  //   * Labo
  //   * Long Sejour
  //   * Moyen Séjour
  //   * Recherche
	if ((sOType == "LUNIF") && (sFUType != "Long Sejour") && (sFUType != "Court Séjour") && (sFUType != "Moyen Séjour"))
  {
  	return false ;
  }

  string sRootStructure = "" ;

	if      (sOType == "LGRPE")
  	sRootStructure = "LGRPE1" ;
  else if (sOType == "UETAB")
  	sRootStructure = "UETAB1" ;
  else if (sOType == "USERV")
  	sRootStructure = "USERV1" ;
  else if (sOType == "LUNIF")
  	sRootStructure = "LUNIF1" ;
  else
  	return false ;

	NSPatPathoArray Ppt(pSuper) ;

  Ppt.ajoutePatho(sRootStructure, 0) ;

  Ppt.ajoutePatho("LNOMA1", 1) ;
  Message Msg ;
  Msg.SetTexteLibre(sLabel) ;
  Ppt.ajoutePatho("£CL000", &Msg, 2) ;

  if ((sOType == "UETAB") || (sOType == "USERV") || (sOType == "LUNIF"))
  {
    Message Msg2 ;
    Msg2.SetComplement(sCode) ;
  	Ppt.ajoutePatho("LIDEG1", &Msg2, 1) ;
  }

  // adds type only if it is "long séjour"
  if ((sOType == "LUNIF") && (sFUType == "Long Sejour"))
  {
		Ppt.ajoutePatho("LTYPA1", 1) ;
    Ppt.ajoutePatho("LUNLS1", 2) ;
  }

  // create the object
  string sFatherNode = "" ;
  if (pLinkAt != NULL)
  	sFatherNode = pLinkAt->sObjectID ;
	NSStructureGraphManager	*pStructureGraphManager = new NSStructureGraphManager(pSuper) ;
  pStructureGraphManager->setStructureGraph(sUserId, &Ppt, sOType, sFatherNode) ;
  sObjectID = pStructureGraphManager->getRootObject() ;

/*
  // create links between structures
	if (sOType != "LGRPE")
  {
  	// un groupement d'établissement n'a pas de père
  	string  sFatherNode = pLinkAt->sObjectID ;
    string  sChildNode  = sObjectID ;

    if ((sFatherNode != "") && (sChildNode != "") && (!pStructureGraphManager->pLinkManager->etablirLien(sChildNode, NSRootLink::objectIn, sFatherNode)))
    {
      erreur("Impossible de créer le lien [structure -> structure mère] de l'objet structure en cours.", standardError, 0) ;
      return false ;
    }
  }
*/

  return true ;
}

CSVStructureArray::CSVStructureArray()
                  :CSVStructureVector()
{
}

CSVStructureArray::CSVStructureArray(const CSVStructureArray& src)
                  :CSVStructureVector()
{
  if (false == src.empty())
		for (CSVStructureCIter iter = src.begin() ; src.end() != iter ; iter++)
    	push_back(new CSVStructure(**iter)) ;
}

CSVStructureArray::~CSVStructureArray()
{
	vider() ;
}

CSVStructure*
CSVStructureArray::searchStructure(string sCode, string sLabel, CSVStructure *pLinkAt, string sType)
{
  CSVStructure structure2search(sCode, sLabel, pLinkAt, sType) ;

	if (false == empty())
  	for (CSVStructureIter iter = begin() ; end() != iter ; iter++)
    	if ((**iter) == structure2search)
        return (*iter) ;

  return (CSVStructure*) 0 ;
}


CSVStructure *
CSVStructureArray::addStructure(string sCode, string sLabel, CSVStructure *plinkAt, string sType)
{
	CSVStructure *structure = searchStructure(sCode, sLabel, plinkAt, sType) ;
  if (structure == NULL)
  {
  	push_back(new CSVStructure(sCode, sLabel, plinkAt, sType)) ;
    return (*rbegin()) ;
  }
  return structure ;
}


bool
CSVStructureArray::createObjects(NSSuper* pSuper, string sUserId, string sOType)
{
	if (false == empty())
  {
  	for (CSVStructureIter iter = begin() ; iter != end() ; iter++)
    	(*iter)->createObjectPPT(pSuper, sUserId, sOType) ;
    return true ;
  }

  return false ;
}

bool
CSVStructureArray::vider()
{
	if (false == empty())
  	for (CSVStructureIter iter = begin() ; end() != iter ; )
    {
    	delete (*iter) ;
      erase(iter) ;
    }
  return true ;
}

bool
NSStructureGraphManager::getStructureGraph(string sObjID, NSPatPathoArray *pPPT)
{
try
{
  // remise à zéro du graphe
  _pDataGraph->graphReset() ;
  string sObject = sObjID ;
  setRootObject(sObjID) ;

	NSBasicAttributeArray AttrList ;
  AttrList.push_back(new NSBasicAttribute(OBJECT, sObjID)) ;

  bool res = _pSuper->getPilot()->invokeService(NautilusPilot::SERV_SEARCH_OBJECT_FROM_ID.c_str(), _pDataGraph, &AttrList) ;

  if (false == res)
  {
  	string sMessage = string("") ;
  	if 			(_pSuper->getPilot()->getErrorMessage() != string(""))
    	sMessage = _pSuper->getPilot()->getErrorMessage() ;
    else if (_pSuper->getPilot()->getWarningMessage() != string(""))
    	sMessage = _pSuper->getPilot()->getWarningMessage() ;
    else
    	sMessage = string("Echec du service de récupération du graphe représentant une Structure.") ;
    erreur(sMessage.c_str(), standardError, 0) ;
    return false ;
  }

  // constitution de la patpatho contenant tous les arbres du graphe
  if (pPPT)
  {
    pPPT->vider() ;
    if (false == _pDataGraph->getTrees()->empty())
    {
      for (NSDataTreeIter i = _pDataGraph->getTrees()->begin() ; _pDataGraph->getTrees()->end() != i ; i++)
      {
        string sTreeId = (*i)->getTreeID() ;

        NSPatPathoArray tempPPT(_pSuper) ;
        string sRights = string("") ;
        if (_pDataGraph->getTree(sTreeId, &tempPPT, &sRights, true))
          pPPT->ajouteVecteur(&tempPPT, ORIGINE_PATH_PATHO) ;
      }
    }
  }
  return true ;
}
catch (...)
{
  erreur("Exception NSStructureGraphManager::getTeamGraph", standardError, 0) ;
  return false ;
}
}

bool
NSStructureGraphManager::setStructureGraph(string sUserId, NSPatPathoArray *pPPT, string sOType, string sFatherID)
{
  if (((NSPatPathoArray *) NULL == pPPT) || (string("") == sOType))
    return false ;

	string  sLabelTag = string("") ;
  string  sIDTag    = string("") ;
  string  sCodeObj  = string("") ;

	if      (sOType == "LGRPE")
  {
  	sLabelTag = GRPE_LNOMA ;
    sCodeObj  = "LGRPE1" ;
  }
  else if (sOType == "UETAB")
  {
  	sLabelTag = ETAB_LNOMA ;
    sIDTag    = ETAB_LIDEG ;
    sCodeObj  = "UETAB1" ;
  }
  else if (sOType == "USERV")
  {
  	sLabelTag = SERV_LNOMA ;
    sIDTag    = SERV_LIDEG ;
    sCodeObj  = "USERV1" ;
  }
  else if (sOType == "LUNIF")
  {
  	sLabelTag = UNIF_LNOMA ;
    sIDTag    = UNIF_LIDEG ;
    sCodeObj  = "LUNIF1" ;
  }
  else
  	return false ;

	string			 sNodeRoot = "" ;
  PatPathoIter pptIter = pPPT->begin() ;

  if ((pptIter != pPPT->end()) && ((*pptIter)->getLexique() == sCodeObj))
  {
		sNodeRoot	= setTree(pPPT, "") ;
  	setRootObject(sNodeRoot) ;
  }
  else
  {
  	erreur("L'arbre ne contient de Structure.", standardError, 0) ;
    return false ;
  }

  // create links between structures
	if (sOType != "LGRPE")
  {
  	// un groupement d'établissement n'a pas de père
    if ((sFatherID != "") && (sNodeRoot != "") && (false == _pLinkManager->etablirLien(sNodeRoot, NSRootLink::objectIn, sFatherID)))
    {
      erreur("Impossible de créer le lien [structure -> structure mère] de l'objet structure en cours.", standardError, 0) ;
      return false ;
    }
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

    // transfer links to pilot
    for (NSPatLinkIter linkIter = _pDataGraph->getLinks()->begin() ; linkIter != _pDataGraph->getLinks()->end() ; linkIter++)
    {
    	if (((*linkIter)->getQualifie()   == sTreeID) ||
			    ((*linkIter)->getQualifiant() == sTreeID))
    		LocalGraph.getLinks()->push_back(new NSPatLinkInfo(*(*linkIter))) ;
    }

    NSBasicAttributeArray	AttrList ;
	  // AttrList.push_back(new NSBasicAttribute("user", pContexte->getUtilisateurID())) ;
    AttrList.push_back(new NSBasicAttribute("user", sUserId)) ;
    if (sLabelTag != "")
    	AttrList.push_back(new NSBasicAttribute(sLabelTag, getStructureName(&PatPatho))) ;
    if (sIDTag != "")
    	AttrList.push_back(new NSBasicAttribute(sIDTag, getStructureID(&PatPatho))) ;

    // calling pilot : create local object
    bool	res	= _pSuper->getPilot()->createPersonOrObject(NautilusPilot::SERV_CREATE_OBJECT.c_str(), &LocalGraph, &ObjectsList, &AttrList, OBJECT_TYPE, false) ;

    if (!res)
    {
    	erreur("Echec du service pilot lors de la création du graphe d'une Structure.", standardError, 0) ;
      return false ;
    }
  }

  // Replace new ID in DataGraph
  NSDataTreeIter iter = LocalGraph.getTrees()->begin() ;
  string sNewTreeID = (*iter)->getTreeID() ;

  NSPatPathoArray Ppt(_pSuper) ;
  (*iter)->getRawPatPatho(&Ppt) ;
  _pDataGraph->replaceTree(sTreeID, sNewTreeID, &Ppt, sTreeRights) ;

	return true ;
}

string
NSStructureGraphManager::getStructureName(NSPatPathoArray *pPPT)
{
  if (((NSPatPathoArray*) NULL == pPPT) || pPPT->empty())
    return string("") ;

	for (PatPathoIter pptIter = pPPT->begin() ; pPPT->end() != pptIter ; pptIter++)
  	if ((*pptIter)->getLexique() == "LNOMA1")
      return pPPT->getFreeTextValue(&pptIter) ;

  return string("") ;
}


string
NSStructureGraphManager::getStructureID(NSPatPathoArray *pPPT)
{
	string	sResult	= "" ;
	for (PatPathoIter pptIter = pPPT->begin() ; pptIter != pPPT->end() ; pptIter++)
	{
  	if ((*pptIter)->getLexique() == "LIDEG1")
    {
    	sResult = (*pptIter)->getComplement() ;
      break ;
    }
  }
  return sResult ;
}


CSVParser::CSVParser(string file, char cField, char cItem)
{
	sFile           = file ;
	cFieldSeparator	= cField ;
  cItemSeparator  = cItem ;

  csvRecords      = new CSVRecordArray() ;
}


CSVParser::~CSVParser()
{
	delete csvRecords ;
}


bool
CSVParser::importFromFile()
{
	ifstream	inFile ;
  inFile.open(sFile.c_str()) ;
  if (!inFile)
  {
  	// erreur - l'ouverture du fichier a échoué
    return false ;
  }

  char		szBuffer[CSVBUF_LEN] ;
  while (inFile.getline(szBuffer, CSVBUF_LEN))
  {
  	size_t	iBuff     = 0 ;
    int			index     = 0 ;
    string  sItem     = "" ;
    size_t  iBuffLen  = strlen(szBuffer) ;
    AttributeValueList	*pAVlist = new AttributeValueList() ;

    while ((iBuff < CSVBUF_LEN) && (iBuff <= iBuffLen))
    {
			if ((szBuffer[iBuff] == cFieldSeparator) || (szBuffer[iBuff] == '\0'))
      {
        iBuff++ ;

        size_t  iLen  = strlen(sItem.c_str()) ;
        sItem         = string(sItem, 1, iLen - 2) ;

        switch (index)
        {
          case  0	:	pAVlist->addElem(ETABLISSEMENTCODE,         sItem) ;  break ;
          case  1	:	pAVlist->addElem(ETABLISSEMENTLABEL,        sItem) ;  break ;
          case  2	:	pAVlist->addElem(CENTRERESPONSABILITECODE,  sItem) ;  break ;
          case  3	: pAVlist->addElem(CENTRERESPONSABILITELABEL, sItem) ;  break ;
          case  4	: pAVlist->addElem(SERVICECODE,               sItem) ;  break ;
          case  5	: pAVlist->addElem(SERVICELABEL,              sItem) ;  break ;
          case  6 : pAVlist->addElem(CENTREACTIVITECODE,        sItem) ;  break ;
          case  7 : pAVlist->addElem(CENTREACTIVITELABEL,       sItem) ;  break ;
          case  8 : pAVlist->addElem(UFCODE,                    sItem) ;  break ;
          case  9 : pAVlist->addElem(UFLABEL,                   sItem) ;  break ;
          case 10 : pAVlist->addElem(UFTYPE,                    sItem) ;  break ;
        }

      	index++ ;
        sItem = "" ;
      }
      else
      {
      	sItem += szBuffer[iBuff++] ;
      }
    }

    // c'est ce CSVRecord qu'il faut sauvegardé dans la liste
    csvRecords->push_back(new CSVRecord(*pAVlist)) ;

    delete pAVlist ;
  }

  inFile.close() ;
  return true ;
}


bool
CSVParser::run(NSSuper* pSuper, string sUserId)
{
	if (!importFromFile())
  	return false ;

  // create array of structures with links
  CSVStructure      *pCHU                 = new CSVStructure(string(""), string("CHU d'Amiens"), NULL) ;
  CSVStructureArray *pEtablissementsArray = new CSVStructureArray() ;
  CSVStructureArray *pServicesArray       = new CSVStructureArray() ;
  CSVStructureArray *pUFsArray            = new CSVStructureArray() ;

  for (CSVRecordIter iter = csvRecords->begin() ; iter != csvRecords->end() ; iter++)
  {
  	// établissement
    CSVStructure *pEtablissement  = pEtablissementsArray->addStructure((*iter)->getValWithAttr(ETABLISSEMENTCODE), (*iter)->getValWithAttr(ETABLISSEMENTLABEL), pCHU) ;
    CSVStructure *pService        = pServicesArray->addStructure((*iter)->getValWithAttr(SERVICECODE), (*iter)->getValWithAttr(SERVICELABEL), pEtablissement) ;
    CSVStructure *pUF             = pUFsArray->addStructure((*iter)->getValWithAttr(UFCODE), (*iter)->getValWithAttr(UFLABEL), pService, (*iter)->getValWithAttr(UFTYPE)) ;
  }

  // save structure in object bases
	pCHU->createObjectPPT(pSuper, sUserId, "LGRPE") ;
  pEtablissementsArray->createObjects(pSuper, sUserId, "UETAB") ;
  pServicesArray->createObjects(pSuper, sUserId, "USERV") ;
  pUFsArray->createObjects(pSuper, sUserId, "LUNIF") ;

  return true ;
}
