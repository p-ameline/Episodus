// -----------------------------------------------------------------------------
// nsHealthTeam.cpp
// -----------------------------------------------------------------------------
// Gestion l'Equipe de Santé
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2015/02/07 14:22:14 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
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


#include "nssavoir\nsHealthTeam.h"

#include "nsbb\nsbbtran.h"
#include "nsbb\tagNames.h"
#include "pilot\NautilusPilot.hpp"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nssavoir\GraphicHealthTeam.h"
  #include "nautilus\nssuper.h"
  #include "nsepisod\nsldvuti.h"
#endif

long NSHealthTeam::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// classe NSTeam
// -----------------------------------------------------------------------------
// classe représentant une Equipe
// --
// class that represents a Team
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// default constructor // only for type == moralPerson
// -----------------------------------------------------------------------------
NSTeam::NSTeam(teamType aType)
{
  _type	= aType ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// --
// copy constructor
// -----------------------------------------------------------------------------
NSTeam::NSTeam(const NSTeam& src)
       :_TeamMembers(src._TeamMembers)
{
  _type = src._type ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSTeam::~NSTeam()
{
}

// -----------------------------------------------------------------------------
// donne la liste des Person IDs des membres de l'Equipe de Santé.
// --
// get the person IDs list of HealthTeam member
// -----------------------------------------------------------------------------
// modification by FLP on 02/03/2005 :
// we check if the member is a person before insering it in the list we return.
// -----------------------------------------------------------------------------
VecteurString*
NSTeam::getPIDs()
{
	VecteurString	*pPIDs = new VecteurString() ;

	if (false == _TeamMembers.empty())
	{
		for (NSHealthTeamMemberIter memberIter = _TeamMembers.begin() ; _TeamMembers.end() != memberIter ; memberIter++)
    	if ((*memberIter)->getType() == NSHealthTeamMember::person)
				pPIDs->AddString((*memberIter)->getID()) ;
	}

	return pPIDs ;
}

// -----------------------------------------------------------------------------
// opérateur =
// --
// operator =
// -----------------------------------------------------------------------------
NSTeam&
NSTeam::operator=(const NSTeam& src)
{
	if (this == &src)
		return *this ;

	_TeamMembers.vider() ;

	_TeamMembers = src._TeamMembers ;
  _type        = src._type ;

	return (*this) ;
}

bool
NSTeam::createTree(NSPatPathoArray *pPPT, int iCol)
{
	if (false == _TeamMembers.empty())
  {
  	for (NSHealthTeamMemberIter memberIter = _TeamMembers.begin() ; _TeamMembers.end() != memberIter ; memberIter++)
    	if (false == (*memberIter)->createTree(pPPT, iCol))
      	return false ;
  }
  return true ;
}

// -----------------------------------------------------------------------------
// ajoute un membre dans l'Equipe de Santé.
// --
// add a member in the HealthTeam.
// -----------------------------------------------------------------------------
void
NSTeam::addMember(NSHealthTeamMember *pMember, NSHealthTeamMember::memberType mType, NSPatPathoArray *pPPT)
{
  if (NULL == pMember)
    return ;

	// fabTODO
	// on met par défaut un membre dans la colonne 1 (équipe de santé en colonne 0)
  pMember->setType(mType) ;
	pMember->createTree(pPPT, 1) ;

	_TeamMembers.push_back(pMember) ;
}

// -----------------------------------------------------------------------------
// modifie un membre de l'Equipe de Santé.
// --
// modify a member in the HealthTeam.
// -----------------------------------------------------------------------------
void
NSTeam::modMember(NSContexte *pCtx, NSHealthTeamMember *pMember, NSPatPathoArray *pPPT)
{
  if (NULL == pMember)
    return ;

	// retrouver l'item dans la liste
	NSHealthTeamMemberIter memberIter = NULL ;

  if (false == _TeamMembers.empty())
	  for (memberIter = _TeamMembers.begin() ; _TeamMembers.end() != memberIter ; memberIter++)
		  if ((*memberIter) == pMember)
			  break ;

	if ((NULL == memberIter) || (_TeamMembers.end() == memberIter))
		return ;

	// fabTODO
	// vérification de son sous-arbre (voir modifier)
	// verification of subtree (perhaps modify)
	// fabTODO
	// on met par défaut un membre dans la colonne 1 (équipe de santé en colonne 0)
	pMember->modifyTree(pCtx, pPPT, 1) ;
}

// -----------------------------------------------------------------------------
// supprime un membre de l'Equipe de Santé.
// --
// delete a member in the HealthTeam.
// -----------------------------------------------------------------------------
void
NSTeam::delMember(NSHealthTeamMember *pMember, NSPatPathoArray *pPPT)
{
	// retrouver l'item dans la liste
	NSHealthTeamMemberIter	memberIter = NULL ;
  if (false == _TeamMembers.empty())
	  for (memberIter = _TeamMembers.begin() ; _TeamMembers.end() != memberIter ; memberIter++)
		  if ((*memberIter) == pMember)
			  break ;

	if ((NULL == memberIter) || (_TeamMembers.end() == memberIter))
		// erreur
		return ;

	// supprimer l'élément et son sous arbre
  // delete item and its subtree in the tree
  //
  PatPathoIter pMemberRootIter = NULL ;
  if (string("") != pMember->sNodeID)
  	pMemberRootIter = pPPT->ChercherNoeud(pMember->sNodeID) ;
	else
  	pMemberRootIter = pMember->pptNode ;

  if (pMemberRootIter)
		pPPT->SupprimerItem(pMemberRootIter) ;

	// effacer l'item - deleting item
	delete pMember ;
	_TeamMembers.erase(memberIter) ;
}

bool
NSTeam::isMemberP(string personId)
{
  if (_TeamMembers.empty())
    return false ;

	for (NSHealthTeamMemberIter mIter = _TeamMembers.begin() ; _TeamMembers.end() != mIter ; mIter++)
  	if ((*mIter)->getID() == personId)
			return true ;

  return false ;
}

bool
NSTeam::isActiveMember(string personId)
{
  if (_TeamMembers.empty())
    return false ;

  for (NSHealthTeamMemberIter mIter = _TeamMembers.begin() ; _TeamMembers.end() != mIter ; mIter++)
  {
    if ((*mIter)->getID() == personId)
    {
      NSHTMMandateArray mMandates ;
      (*mIter)->getActiveMandates(&mMandates) ;
      if (false == mMandates.empty())
        return true ;
    }
  }

  return false ;
}

NSHealthTeamMember*
NSTeam::getFirstMember(string id)
{
  if (_TeamMembers.empty())
    return (NSHealthTeamMember*) 0 ;

	// fabFIXME
  // il faudrait gérer le cas où un même membre est présent plusieurs fois au sein d'une Equipe
	for (NSHealthTeamMemberIter memberIter = _TeamMembers.begin() ; _TeamMembers.end() != memberIter ; memberIter++)
  	if ((*memberIter)->getID() == id)
			return (*memberIter) ;

  return (NSHealthTeamMember*) 0 ;
}

void
NSTeam::closeAllMandates(NSContexte *pCtx, NSHealthTeamMember *pMe, NSPatPathoArray *pPPT)
{
  if (NULL == pMe)
    return ;

  pMe->closeAllMandates(pCtx, pPPT) ;
}

void
NSTeam::closeAllMandates(NSContexte *pCtx, string sPersonId, NSPatPathoArray *pPPT)
{
  if (_TeamMembers.empty())
    return ;

  for (NSHealthTeamMemberIter mIter = _TeamMembers.begin() ; _TeamMembers.end() != mIter ; mIter++)
    if ((*mIter)->getID() == sPersonId)
      (*mIter)->closeAllMandates(pCtx, pPPT) ;
}

// -----------------------------------------------------------------------------
// class NSHealthTeam
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// constructor from tree (patpatho)
// -----------------------------------------------------------------------------
NSHealthTeam::NSHealthTeam(NSPatPathoArray *pPPT)
             :_Team(NSTeam::healthTeam), _ppt(*pPPT)
{
  lObjectCount++ ;

  initFromPatho() ;
}

// -----------------------------------------------------------------------------
// copy constructor
// -----------------------------------------------------------------------------
NSHealthTeam::NSHealthTeam(const NSHealthTeam& src)
             :_Team(NSTeam::healthTeam), _ppt(src._ppt)
{
  lObjectCount++ ;

  initFromPatho() ;
}

// -----------------------------------------------------------------------------
// destructor
// -----------------------------------------------------------------------------
NSHealthTeam::~NSHealthTeam()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// operator =
// -----------------------------------------------------------------------------
NSHealthTeam&
NSHealthTeam::operator=(const NSHealthTeam& src)
{
	if (this == &src)
		return *this ;

  _ppt = src._ppt ;
  initFromPatho() ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
// get member that corresponds to current user
// -----------------------------------------------------------------------------
NSHealthTeamMember*
NSHealthTeam::getUserAsMember(NSContexte *pCtx)
{
	// Can we work ?
	if (NULL == pCtx)
  	return (NSHealthTeamMember*) 0 ;

  string sUserId = pCtx->getUtilisateurID() ;
  if (string("") == sUserId)
  	return (NSHealthTeamMember*) 0 ;

  NSHealthTeamMember *pMember = _Team.getFirstMember(sUserId) ;
  return (pMember) ;
}

bool
NSHealthTeam::initFromPatho()
{
try
{
  _pptNode = NULL ;
  _sNodeID = string("") ;

	_Team.getMembers()->vider() ;

	if (_ppt.empty())
  	return false ;

  PatPathoIter pptIter = _ppt.begin() ;

	// traitement du type de l'Equipe - processing team type
	if ((*pptIter)->getLexique() != HEALTHTEAM)
		// cet arbre ne correspond pas à une Equipe de Santé
		return false ;

  _pptNode = pptIter ;
  _sNodeID = (*pptIter)->getNode() ;

	int		iColBase = (*pptIter)->getColonne() ;
	pptIter++ ;

	while ((_ppt.end() != pptIter) && ((*pptIter)->getColonne() > iColBase))
	{
		if 			((*pptIter)->getLexique() == HTMEMBER)
		{
			NSHealthTeamMember *pNewMember = new NSHealthTeamMember() ;
			if ((pNewMember->initFromPatho(&_ppt, &pptIter)) && (pNewMember->isActive()))
				_Team.getMembers()->push_back(pNewMember) ;
      else
      	delete pNewMember ;
		}
		else
		{
			// erreur - on devrait être sur un item correspondant à un membre de l'Equipe de Santé
			return false ;
		}
	}

	return true ;
}
catch (...)
{
	erreur("Exception NSHealthTeam::initFromPatho.", standardError) ;
  return false ;
}
}

// -----------------------------------------------------------------------------
// Function     : bool isMemberP(string)
// Arguments    : sPersonId : ID of person to check if he is present in the Team
// Description  : Does user identified by sPersonId be present in this Team ?
// Returns      : true if he is present, false if he is not
// -----------------------------------------------------------------------------
/*
bool
NSHealthTeam::isMemberP(string sPersonId)
{
  // TODO FIXME -- FLP

  // Attention aux membres qui se trouvent plusieurs fois dans l'equipe
  // normalement pas d'incidence ici

  return (pTeam->isMemberP(sPersonId)) ;
}
*/


/*
bool
NSHealthTeam::createTree()
{
	// cette fonction n'est valable que s'il n'y a pas de ppt déjà remplie
	if ((ppt == NULL) || (!(ppt->empty())) || (pTeam->getType() != NSTeam::moralPerson))
  	return false ;

  int	iCol = 0 ;
	ppt->ajoutePatho(HEALTHTEAM, iCol, 0) ;

	PatPathoIter pptIter = ppt->end() ;
	pptIter-- ;

	if (((*pptIter)->getLexique() != HEALTHTEAM) || ((*pptIter)->getColonne() != iCol))
  {
		// erreur
		return false ;
	}

	pptNode = pptIter ;

  return (pTeam->createTree(ppt, iCol + 1)) ;
}
*/

// -----------------------------------------------------------------------------
//
// class NSMoralPerson
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------
NSMoralPerson::NSMoralPerson(NSContexte *pCtx)
              :_Team(NSTeam::moralPerson), _ppt(pCtx->getSuperviseur())
{
  _teamId			 = string("") ;
  _sLabel			 = string("") ;
  _sSpeciality = string("") ;
}

NSMoralPerson::NSMoralPerson(NSContexte *pCtx, string id, string label, string speciality)
              :_Team(NSTeam::moralPerson), _ppt(pCtx->getSuperviseur())
{
  _teamId      = id ;
  _sLabel      = label ;
  _sSpeciality = speciality ;
}

NSMoralPerson::NSMoralPerson(NSContexte *pCtx, NSPatPathoArray *pPPT)
              :_Team(NSTeam::moralPerson), _ppt(pCtx->getSuperviseur())
{
  if (pPPT)
    _ppt = *pPPT ;

  _teamId			 = string("") ;
  _sLabel			 = string("") ;
  _sSpeciality = string("") ;

  initFromPatho() ;
}

NSMoralPerson::NSMoralPerson(const NSMoralPerson& src)
              :_Team(NSTeam::moralPerson), _ppt(src._ppt)
{
  _teamId			 = src._teamId ;
  _sLabel			 = src._sLabel ;
  _sSpeciality = src._sSpeciality ;

  initFromPatho() ;
}

NSMoralPerson::~NSMoralPerson()
{
}

bool
NSMoralPerson::initFromPatho()
{
	_pptNode = NULL ;
  _sNodeID = string("") ;

	if (_ppt.empty())
  	return false ;

  PatPathoIter pptIter = _ppt.begin() ;

	// traitement du type de l'Equipe - processing team type
	if ((*pptIter)->getLexique() != HTTEAM)
		// cet arbre ne correspond pas à une Equipe de personnes
		return false ;

  _pptNode = pptIter ;
  _sNodeID = (*pptIter)->getNode() ;

	int iColBase = (*pptIter)->getColonne() ;
	pptIter++ ;

	while ((_ppt.end() != pptIter) && ((*pptIter)->getColonne() > iColBase))
	{
		if 			((*pptIter)->getLexique() == HTMEMBER)
		{
			NSHealthTeamMember *pNewMember = new NSHealthTeamMember() ;
			if (pNewMember->initFromPatho(&_ppt, &pptIter))
				_Team.getMembers()->push_back(pNewMember) ;
      else
      {
      	delete pNewMember ;
        return false ;
      }
		}
		else if ((*pptIter)->getLexique() == TEAMADMIN)
    {
    	int	iColAdminBase = (*pptIter)->getColonne() ;
      pptIter++ ;

    	while ((_ppt.end() != pptIter) && ((*pptIter)->getColonne() > iColAdminBase))
      {
      	if			((*pptIter)->getLexique() == TEAMLABEL)
        {
			   	pptIter++ ;
      		if ((*pptIter)->getLexique() == HTTEAMFREETEXT)
		      {
    				setLabel((*pptIter)->getTexteLibre()) ;
        		pptIter++ ;
        	}
        }
        else if ((*pptIter)->getLexique() == TEAMSPEC)
        {
        	pptIter++ ;

          // when we process a team creation with an archetype, we have a
          // freetext node that appears, and the lexical that we search code is
          // on its son
          if ((*pptIter)->getLexique() == HTTEAMLEXIVAL)
          	pptIter++ ;

          setSpeciality((*pptIter)->getLexique()) ;
          pptIter++ ;
        }
        else if ((*pptIter)->getLexique() == TEAMADDRS)
        {
        }
        else
        {
        	// erreur noeud inconnu à la racine de l'arbre administratif d'une Equipe de Personnes
          return false ;
        }
      }
    }
    else
		{
			// erreur - noeud inconnu à la racine de l'arbre d'une Equipe de Personnes
			return false ;
		}
	}

	return true ;
}

bool
NSMoralPerson::createTree()
{
	// cette fonction n'est valable que s'il n'y a pas de ppt déjà remplie
	if ((false == _ppt.empty()) || (_Team.getType() != NSTeam::moralPerson))
  	return false ;

  int	iCol = 0 ;
	_ppt.ajoutePatho(HTTEAM, iCol) ;

	PatPathoIter pptIter = _ppt.end() ;
	pptIter-- ;

	if (((*pptIter)->getLexique() != HTTEAM) || ((*pptIter)->getColonne() != iCol))
		// erreur
		return false ;

  if ((getLabel() != "") || (getSpeciality() != ""))
  {
  	_ppt.ajoutePatho(TEAMADMIN, iCol + 1) ;

  	if (string("") != getLabel())
    {
    	_ppt.ajoutePatho(TEAMLABEL, iCol + 2) ;
      Message Msg ;
      Msg.SetTexteLibre(getLabel()) ;
      _ppt.ajoutePatho(HTTEAMFREETEXT, &Msg, iCol + 3) ;
    }

    if (string("") != getSpeciality())
    {
    	_ppt.ajoutePatho(TEAMSPEC, iCol + 2) ;
      _ppt.ajoutePatho(getSpeciality(), iCol + 3) ;
    }
  }

  // fabFIXME il reste les adresses

	_pptNode = pptIter ;
  _sNodeID = string("") ;

  return (_Team.createTree(&_ppt, iCol + 1)) ;
}


// -----------------------------------------------------------------------------
// Function     : bool isMemberP(string)
// Arguments    : sPersonId : ID of person to check if he is present in the Team
// Description  : Does user identified by sPersonId be present in this Team ?
// Returns      : true if he is present, false if he is not
// -----------------------------------------------------------------------------
/*
bool
NSMoralPerson::isMemberP(string sPersonId)
{
  // TODO FIXME -- FLP

  // Attention aux membres qui se trouvent plusieurs fois dans l'equipe
  // normalement pas d'incidence ici

  return (pTeam->isMemberP(sPersonId)) ;
}
*/

// -----------------------------------------------------------------------------
// Function     : bool addMember(string)
// Arguments    : sPersonId : ID of person to add to the team
// Description  : add a member if he is not already present in the team
// Returns      : true if member now in the team
// -----------------------------------------------------------------------------
bool
NSMoralPerson::addMember(NSContexte * /* pCtx */, string personId)
{
  // TODO FIXME -- FLP
  if (false == isActiveMember(personId))
  {
    NVLdVTemps	ldvCurDate ;	ldvCurDate.takeTime() ;
    NVLdVTemps	ldvNoLimit ;	ldvNoLimit.setNoLimit() ;

    // ajout des mandats de l'utilisateur -- adding user mandate
    NSHTMMandateArray MandatesArray ;
    MandatesArray.push_back(new NSHealthTeamMandate(nearDist, 3, "", "", ldvCurDate.donneDateHeure(), ldvNoLimit.donneDateHeure(), "", "", NSHealthTeamMandate::user)) ;

    addMember(new NSHealthTeamMember(personId, NSHealthTeamMember::person, &MandatesArray), NSHealthTeamMember::person) ;
  }

  if (isActiveMember(personId))
    return true ;

  return false ;
}

// -----------------------------------------------------------------------------
// Function     : bool delMember(string)
// Arguments    : sPersonId : ID of person to del from the team
// Description  : del a member if he is already present in the team
// Returns      : true if member is not present now in the team
// -----------------------------------------------------------------------------
bool
NSMoralPerson::delMember(NSContexte *pCtx, string personId)
{
  // TODO FIXME -- FLP

  // Attention aux membres qui se trouvent plusieurs fois dans l'equipe
  if (isMemberP(personId))
    _Team.closeAllMandates(pCtx, personId, &_ppt) ;
  return true ;
}

#ifndef _ENTERPRISE_DLL
NSMoralPersonManager&
NSMoralPersonManager::operator=(const RosaceManager& src)
{
	pMPTeam = src.getMPTeam() ;

  return (*this) ;
}

NSHealthTeamManager&
NSHealthTeamManager::operator=(const RosaceManager& src)
{
	HTeam = *(src.getHTeam()) ;

  return (*this) ;
}

#endif

// -----------------------------------------------------------------------------
//
// class NSTeamSupervisor
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// constructor
// -----------------------------------------------------------------------------
NSTeamSupervisor::NSTeamSupervisor(NSContexte *pCtx)
                 :NSRoot(pCtx)
{
}

vector<string>
NSTeamSupervisor::getLocalTeams()
{
  vector<string> vRes ;

  NautilusPilot* pPilot = pContexte->getSuperviseur()->getPilot() ;
  if (NULL == pPilot)
    return vRes ;

  string serviceName = NautilusPilot::SERV_SEARCH_OBJECT_HAVING_TRAITS ;

  NSPersonsAttributesArray ResultList ;
  NSBasicAttributeArray		 AttrArray ;
  AttrArray.push_back(new NSBasicAttribute("trait", LOCALTEAM)) ;

  bool listOk = pPilot->objectList(serviceName.c_str(), &ResultList, &AttrArray) ;
  if ((false == listOk) || (ResultList.empty()))
		return vRes ;

  for (NSPersonsAttributeIter personAttrIter = ResultList.begin() ; personAttrIter != ResultList.end() ; personAttrIter++)
  {
    if (false == (*personAttrIter)->empty())
    {
    	for (NSBasicAttributeIter basicAttrIter = (*personAttrIter)->begin() ; basicAttrIter != (*personAttrIter)->end() ; basicAttrIter++)
    	{
    		if ((*basicAttrIter)->getBalise() == OIDS)
      	{
      		string sObjectID = (*basicAttrIter)->getText() ;
        	vRes.push_back(sObjectID) ;
        }
      }
    }
  }

  return vRes ;
}

