// -----------------------------------------------------------------------------
// nsHealthTeamMember.cpp
// -----------------------------------------------------------------------------
// Gestion d'un membre d'une Equipe
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: pameline $
// $Date: 2013/07/21 17:58:22 $
// -----------------------------------------------------------------------------
// FLP - juin 2004
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Copyright Nautilus, 2004
// http://www.nautilus-info.com
// -----------------------------------------------------------------------------
// Ce logiciel est un programme informatique servant � g�rer et traiter les
// informations de sant� d'une personne.
//
// Ce logiciel est r�gi par la licence CeCILL soumise au droit fran�ais et
// respectant les principes de diffusion des logiciels libres. Vous pouvez
// utiliser, modifier et/ou redistribuer ce programme sous les conditions de la
// licence CeCILL telle que diffus�e par le CEA, le CNRS et l'INRIA sur le site
// "http://www.cecill.info".
//
// En contrepartie de l'accessibilit� au code source et des droits de copie, de
// modification et de redistribution accord�s par cette licence, il n'est offert
// aux utilisateurs qu'une garantie limit�e. Pour les m�mes raisons, seule une
// responsabilit� restreinte p�se sur l'auteur du programme, le titulaire des
// droits patrimoniaux et les conc�dants successifs.
//
// A cet �gard  l'attention de l'utilisateur est attir�e sur les risques
// associ�s au chargement, � l'utilisation, � la modification et/ou au
// d�veloppement et � la reproduction du logiciel par l'utilisateur �tant donn�
// sa sp�cificit� de logiciel libre, qui peut le rendre complexe � manipuler et
// qui le r�serve donc � des d�veloppeurs et des professionnels avertis
// poss�dant des connaissances informatiques approfondies. Les utilisateurs sont
// donc invit�s � charger et tester l'ad�quation du logiciel � leurs besoins
// dans des conditions permettant d'assurer la s�curit� de leurs syst�mes et ou
// de leurs donn�es et, plus g�n�ralement, � l'utiliser et l'exploiter dans les
// m�mes conditions de s�curit�.
//
// Le fait que vous puissiez acc�der � cet en-t�te signifie que vous avez pris
// connaissance de la licence CeCILL, et que vous en avez accept� les termes.
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
//#include "nssavoir\nsRightsManager.h"
//#include "nssavoir\GraphicHealthTeam.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "nsldv\nsldvuti.h"
#endif

#include "nsbb\nsbbtran.h"

// -----------------------------------------------------------------------------
// classe NSHealthTeamMember
// -----------------------------------------------------------------------------
// classe repr�sentant un membre de l'Equipe de Sant�.
// --
// class that represents an HealthTeam's member.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// constructeur par d�faut
// --
// default constructor
// -----------------------------------------------------------------------------
NSHealthTeamMember::NSHealthTeamMember()
{
	sID       = string("") ;
	pMandates = new NSHTMMandateArray() ;
	pptNode   = NULL ;
  sNodeID   = string("") ;
  type      = person ;
}


NSHealthTeamMember::NSHealthTeamMember(string sid)
{
	sID       = sid ;
	pMandates = new NSHTMMandateArray() ;
	pptNode   = NULL ;
  sNodeID   = string("") ;
  type      = person ;
}


NSHealthTeamMember::NSHealthTeamMember(string id, memberType mType, NSHTMMandateArray *mandates)
{
	sID       = id ;
  type      = mType ;
  pptNode   = NULL ;
  sNodeID   = string("") ;
  pMandates = new NSHTMMandateArray(*mandates) ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// --
// copy constructor
// -----------------------------------------------------------------------------
NSHealthTeamMember::NSHealthTeamMember(NSHealthTeamMember& src)
{
	sID       = src.sID ;
	pMandates = new NSHTMMandateArray(*(src.pMandates)) ;
	pptNode   = src.pptNode ;
  sNodeID   = src.sNodeID ;
  type      = src.type ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSHealthTeamMember::~NSHealthTeamMember()
{
	pMandates->vider() ;
	delete pMandates ;
}

// -----------------------------------------------------------------------------
// op�rateur =
// --
// operator =
// -----------------------------------------------------------------------------
NSHealthTeamMember&
NSHealthTeamMember::operator=(const NSHealthTeamMember& src)
{
	if (this == &src)
		return *this ;

	pMandates->vider() ;
	delete pMandates ;

	sID       = src.sID ;
	pMandates = new NSHTMMandateArray(*(src.pMandates)) ;
	pptNode   = src.pptNode ;
  sNodeID   = src.sNodeID ;
  type      = src.type ;

	return (*this) ;
}

// -----------------------------------------------------------------------------
// Fonction qui est utiliser depuis la fenetre de cr�ation d 'un mandat.
// -----------------------------------------------------------------------------
void
NSHealthTeamMember::getTree(NSPatPathoArray *pPPT, int iCol)
{
	createTree(pPPT, iCol) ;
}

// -----------------------------------------------------------------------------
// le membre est-il valide ? (est-ce que son ID a 7 caract�re et que le premier
// caract�re est diff�rent de '#' (ID temporaire) et de '_' (ID local)).
// --
// is member valid ? (does his ID have 7 characters and its first is different
// than '#' (temporary ID) and than '_' (local ID)).
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::isValid()
{
	if ((strlen(sID.c_str()) == 7) && (sID[0] != '#'))
		return true ;
	else
		return false ;
}

// -----------------------------------------------------------------------------
// est-ce que le membre � au moins un mandat actif ?
// --
// Does member have at least one mandate active ?
// -----------------------------------------------------------------------------
// NB fabFIXME : Est-ce qu'il faudrait pas v�rifier uniquement les mandats d'utilisation ?
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::isActive()
{
	if ((NULL == pMandates) || (pMandates->empty()))
		return false ;

	for (NSHTMMandateIter mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
  	if ((*mandateIter)->isActive())
    	return true ;

  return false ;
}

// -----------------------------------------------------------------------------
// initialise le membre � partir d'une PatPatho.
// --
// initialize a member from a PatPatho.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::initFromPatho(NSPatPathoArray *pPPT, PatPathoIter *pptIter)
{
	pptNode	= NULL ;
  sNodeID = string("") ;

	if ((NULL == pptIter)  || (NULL == *pptIter) || (pPPT->end() == *pptIter))
		return false ;

	if ((**pptIter)->getLexique() != HTMEMBER)
		return false ;

	pptNode = (*pptIter) ;
  sNodeID = (**pptIter)->getNode() ;

	int	iBaseCol = (**pptIter)->getColonne() ;
	(*pptIter)++ ;

	while (((*pptIter != pPPT->end()) && (**pptIter)->getColonne() > iBaseCol))
	{
		if 			((**pptIter)->getLexique() == HTMEMBERID)
		{
			// on r�cup�re le personId - getting the personId
			setID((**pptIter)->getComplement()) ;
			(*pptIter)++ ;
		}
    else if ((**pptIter)->getLexique() == HTOBJECTID)
    {
			// on r�cup�re l'objectId - getting the objectId
    	setID((**pptIter)->getComplement()) ;
      setType(team) ;
      (*pptIter)++ ;
    }
		else if (((**pptIter)->getLexique() == HTMEMBERUSERMANDATE) || ((**pptIter)->getLexique() == HTMEMBERROOTMANDATE))
		{
			// on r�cup�re un mandat d'utilisation ou d'administration - getting a mandate (user or root)
			NSHealthTeamMandate	*pNewMandate = NULL ;
			if 			((**pptIter)->getLexique() == HTMEMBERUSERMANDATE)
      	pNewMandate = new NSHealthTeamMandate(NSHealthTeamMandate::user) ;
			else if ((**pptIter)->getLexique() == HTMEMBERROOTMANDATE)
				pNewMandate = new NSHealthTeamMandate(NSHealthTeamMandate::root) ;
			else
				// erreur - error
				return false ;

			if (pNewMandate->initFromPatho(pPPT, pptIter))
				pMandates->push_back(pNewMandate) ;
      else
      	delete pNewMandate ;
		}
		else
			// erreur - error
			// pas d'autres sous-arbres - there no else subtree in member
			return false ;
	}

	return true ;
}

// -----------------------------------------------------------------------------
// cr�� l'arbre qui correspond au membre de l'Equipe de Sant�.
// --
// create a HealthTeam member tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::createTree(NSPatPathoArray *pPPT, int iCol)
{
  if (NULL == pPPT)
  	return false ;

	pPPT->ajoutePatho(HTMEMBER, iCol) ;

	PatPathoIter pptIter = pPPT->end() ;
	pptIter-- ;

	if (((*pptIter)->getLexique() != HTMEMBER) || ((*pptIter)->getColonne() != iCol))
		// erreur
		return false ;

	// id
	if (getID() != "")
	{
    Message Msg ;
    Msg.SetComplement(getID()) ;
		if (isTeam())
    	pPPT->ajoutePatho(HTOBJECTID, &Msg, iCol + 1) ;
    else
    	pPPT->ajoutePatho(HTMEMBERID, &Msg, iCol + 1) ;
	}
	else
		// erreur - on ne peut pas rajouter de membre qui n'a pas d'ID
		return false ;

	if (!pMandates->empty())
    for (NSHTMMandateIter mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
    {
      if (!((*mandateIter)->createTree(pPPT, iCol + 1)))
        // erreur - probl�me � l'ajout d'un mandat
        // error - there is a problem in adding a mandate
        return false ;
    }

	pptNode = pptIter ;
	return true ;
}

// -----------------------------------------------------------------------------
// modifie l'arbre d'un membre de l'Equipe de Sant�.
// --
// modify a HealthTeam member tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::modifyTree(NSContexte *pCtx, NSPatPathoArray *pPPT, int iCol)
{
	// fabTODO

	if (pptNode == NULL)
	{
		// erreur
		// on ne peut pas faire de modification sur un membre qui n'a pas �t� cr��
		// we can not do a modification in a member that was not create before.
		return false ;
	}

  PatPathoIter pptIter = NULL ;
  if (sNodeID != string(""))
  	pptIter = pPPT->ChercherNoeud(sNodeID) ;
	else
  	pptIter = pptNode ;

	if (NULL == pptIter)
		return false ;

	if (((*pptIter)->getLexique() != HTMEMBER) || ((*pptIter)->getColonne() != iCol))
		// erreur - on est pas sur un item HTMEMBER
		return false ;

	pptIter++ ;

	// ID
	if (isPerson() && (((*pptIter)->getLexique() != HTMEMBERID) || ((*pptIter)->getColonne() != iCol + 1)))
		// erreur - on est pas sur un item HTMEMBERID, la colonne est incorrect
		return false ;

	if (isTeam() && (((*pptIter)->getLexique() != HTOBJECTID) || ((*pptIter)->getColonne() != iCol + 1)))
		// erreur - on est pas sur un item HTOBJECTID, la colonne est incorrect
		return false ;

	if ((*pptIter)->getComplement() != getID())
		// erreur - si l'ID n'est pas le m�me, le membre n'est pas le m�me
		return false ;

	if (!pMandates->empty())
    for (NSHTMMandateIter mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
      (*mandateIter)->modifyTree(pCtx, pPPT, iCol + 1) ;

	return true ;
}

// -----------------------------------------------------------------------------
// ajoute un mandat � un membre de l'Equipe de Sant�.
// --
// add a mandate at a HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::addMandate(NSHealthTeamMandate *pMandate, NSPatPathoArray *pPPT)
{
	if ((NULL == pMandate) || (NULL == pPPT))
		return false ;

	// fabTODO
	// on met par d�faut un membre dans la colonne 1 (�quipe de sant� en colonne 0)
	pMandate->createTree(pPPT, 2) ;
	pMandates->push_back(pMandate) ;

	return true ;
}

// -----------------------------------------------------------------------------
// modifie un mandat d'un membre de l'Equipe de Sant�.
// --
// modify a mandate of a HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::modMandate(NSContexte *pCtx, NSHealthTeamMandate *pMandate, NSPatPathoArray *pPPT)
{
	if ((NULL == pMandate) || (NULL == pPPT))
		return false ;

	// fabTODO
	return (pMandate->modifyTree(pCtx, pPPT, 2)) ;
}

/*
// -----------------------------------------------------------------------------
// supprime un mandat d'un membre de l'Equipe de Sant�.
// --
// delete a mandate of a HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::delMandate(NSHealthTeamMandate *pMandate, NSPatPathoArray *pPPT)
{
	// fabTODO
	NSHTMMandateIter mandateIter = NULL ;
	if (!pMandates->empty())
		for (mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
			if ((*mandateIter) == pMandate)
				break ;

	if ((mandateIter == NULL) || (mandateIter == pMandates->end()))
		return false ;

	pPPT->SupprimerItem(pMandate->pptNode) ;
	delete pMandate ;
	pMandates->erase(mandateIter) ;
	return true ;
}
*/

// -----------------------------------------------------------------------------
// ferme un mandat d'un membre de l'Equipe de Sant�.
// --
// close a mandate of a HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::closeMandate(NSContexte *pCtx, NSHealthTeamMandate *pM, NSPatPathoArray *pPPT)
{
	if (NULL == pM)
		return false ;

	NSHTMMandateIter mandateIter = NULL ;
  if (!pMandates->empty())
  	for (mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
    	if ((*mandateIter) == pM)
      	break ;

  if ((mandateIter == NULL) || (mandateIter == pMandates->end()))
  	return false ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  pM->setEndDate(tpsNow.donneDateHeure()) ;
  // return (pM->modifyTree(pCtx, pPPT, 2)) ;
  return (pM->modifyEndDate(pCtx, pPPT)) ;
}

// -----------------------------------------------------------------------------
// ferme un mandat d'un membre de l'Equipe de Sant�.
// --
// close a mandate of a HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMember::closeAllMandates(NSContexte *pCtx,	NSPatPathoArray *pPPT)
{
	bool							bResult			= true ;
	NSHTMMandateIter	mandateIter	= NULL ;
  if (!pMandates->empty())
  	for (mandateIter = pMandates->begin() ; mandateIter != pMandates->end() ; mandateIter++)
    {
	  	NVLdVTemps tpsNow ;
  		tpsNow.takeTime() ;
    	if ((*mandateIter)->getEndDate() > tpsNow.donneDateHeure())
      {
      	if ((*mandateIter)->getEndDate() > (*mandateIter)->getBeginDate())
      		(*mandateIter)->setEndDate(tpsNow.donneDateHeure()) ;
        else
        	(*mandateIter)->setEndDate((*mandateIter)->getBeginDate()) ;
        if (!(*mandateIter)->modifyTree(pCtx, pPPT, 2))
        	bResult = false ;
      }
    }
    return (bResult) ;
}

// -----------------------------------------------------------------------------
// r�cup�re les mandats actifs d'un membre de l'Equipe de Sant�.
// --
// get active mandates from a HealthTeam member.
// -----------------------------------------------------------------------------
void
NSHealthTeamMember::getActiveMandates(NSHTMMandateArray *pActiveMandates)
{
	// Can we work ?
	if (NULL == pActiveMandates)
  	return ;

  pActiveMandates->vider() ;

  if (!pMandates || (pMandates->empty()))
  	return ;

	NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;

  NVLdVTemps tpsBegin ;
  NVLdVTemps tpsEnd ;

  NSHTMMandateIter mandateIter = pMandates->begin() ;
  for ( ; mandateIter != pMandates->end() ; mandateIter++)
  {
  	(*mandateIter)->getBeginDate(&tpsBegin) ;
    (*mandateIter)->getEndDate(&tpsEnd) ;

    if ((tpsBegin <= tpsNow) && (tpsEnd >= tpsNow))
    	pActiveMandates->push_back(new NSHealthTeamMandate(**mandateIter)) ;
  }
}

// -----------------------------------------------------------------------------
// r�cup�re tous les mandats d'un membre de l'Equipe de Sant�.
// --
// get all mandates from a HealthTeam member.
// -----------------------------------------------------------------------------
void
NSHealthTeamMember::getHistoryMandates(NSHTMMandateArray *pHistoryMandates)
{
}

// -----------------------------------------------------------------------------
// classe NSHealthTeamMemberArray
// -----------------------------------------------------------------------------
// classe repr�sentant une liste de membres de l'Equipe de Sant�.
// --
// class that represents a HealthTeam's members list.
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// constructeur par d�faut
// --
// default constructor
// -----------------------------------------------------------------------------
NSHealthTeamMemberArray::NSHealthTeamMemberArray()
                        :NSHealthTeamMemberVector()
{
}

// -----------------------------------------------------------------------------
// constructeur copie
// --
// copy constructor
// -----------------------------------------------------------------------------
NSHealthTeamMemberArray::NSHealthTeamMemberArray(NSHealthTeamMemberVector& src)
{
	if (!src.empty())
		for (NSHealthTeamMemberIter iter = src.begin() ; iter != src.end() ; iter++)
			push_back(new NSHealthTeamMember(**iter)) ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSHealthTeamMemberArray::~NSHealthTeamMemberArray()
{
	vider() ;
}

// -----------------------------------------------------------------------------
// op�rateur =
// --
// operator =
// -----------------------------------------------------------------------------
NSHealthTeamMemberArray&
NSHealthTeamMemberArray::operator=(const NSHealthTeamMemberArray& src)
{
	if (this == &src)
		return *this ;

	vider() ;

	if (false == src.empty())
		for (NSHealthTeamMemberConstIter iter = src.begin() ; src.end() != iter ; iter++)
			push_back(new NSHealthTeamMember(**iter)) ;

	return (*this) ;
}

// -----------------------------------------------------------------------------
// vide la liste
// --
// emptyed the list
// -----------------------------------------------------------------------------
bool
NSHealthTeamMemberArray::vider()
{
	if (!empty())
		for (NSHealthTeamMemberIter iter = begin() ; iter != end() ; )
		{
			delete (*iter) ;
			erase(iter) ;
		}

	return true ;
}

