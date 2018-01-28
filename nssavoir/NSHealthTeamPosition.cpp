// -----------------------------------------------------------------------------
// nsHealthTeamPosition.cpp
// -----------------------------------------------------------------------------
// Gestion d'un membre d'une Equipe
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2010/02/16 10:27:23 $
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
//#include "nssavoir\nsRightsManager.h"
//#include "nssavoir\GraphicHealthTeam.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nssavoir\GraphicHealthTeam.h"
  #include "nautilus\nssuper.h"
  #include "nsepisod\nsldvuti.h"
#endif

#include "nsbb\nsbbtran.h"

// -----------------------------------------------------------------------------
// class NSHealthTeamPosition
// -----------------------------------------------------------------------------
// classe représentant la position d'un membre de l'Equipe de Santé.
// --
// class that represents the position of an HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::setDistance(int iDist)
{
	if (iDist < 4)
  {
  	switch (iDist)
    {
    	case	0 :	setDistance(patDist) ;			break ;
      case	1 :	setDistance(nearDist) ; 		break ;
      case	2 :	setDistance(farDist) ; 			break ;
      case	3 :	setDistance(farAwayDist) ;	break ;
      default	: setDistance(DTnotDef) ;			break ;
    }
    return true ;
  }
  else
  	return false ;
}


// -----------------------------------------------------------------------------
// initialisation de la position d'un membre de l'Equipoe de Santé à partir
// d'une PatPatho.
// --
// initialization of HealthTeam member from a PatPatho.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::initFromPatho(NSPatPathoArray *pPPT, PatPathoIter *pptIter)
{
	pptNode	= NULL ;
  sNodeID = string("") ;

  if ((NULL == pPPT) || (NULL == pptIter) || pPPT->empty())
  	return false ;

	if ((**pptIter)->getLexique() != HTMEMBERPOSITION)
		return false ;

	pptNode	= (*pptIter) ;
  sNodeID = (**pptIter)->getNode() ;

	int	iColBase = (**pptIter)->getColonne() ;
	(*pptIter)++ ;

	while ((*pptIter != pPPT->end()) && ((**pptIter)->getColonne() > iColBase))
	{
		if 			((**pptIter)->getLexique() == HTMEMBERDISTANCE)
		{
			(*pptIter)++ ;
			if ((string((**pptIter)->getLexique(), 0, 2) == HTVALNUM)	&& ((**pptIter)->getUnit() == HTNOUNIT))
			{
				setDistance(atoi((**pptIter)->getComplement().c_str())) ;
				(*pptIter)++ ;
      }
			else
				return false ;
		}
		else if ((**pptIter)->getLexique() == HTMEMBERANGLE)
    {
			(*pptIter)++ ;
			if ((string((**pptIter)->getLexique(), 0, 2) == HTVALNUM)	&& ((**pptIter)->getUnit() == HTANGUNIT))
			{
				setAngle(atoi((**pptIter)->getComplement().c_str())) ;
				(*pptIter)++ ;
			}
			else
				return false ;
		}
		else
			return false ;
	}
	return true ;
}

// -----------------------------------------------------------------------------
// créé l'arbre correspondant à la position d'un membre de l'Equipe de Santé.
// --
// creates an HealthTeam member position tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::createTree(NSPatPathoArray *pPPT, int iCol)
{
	if (NULL == pPPT)
		return false ;

	pPPT->ajoutePatho(HTMEMBERPOSITION, iCol) ;

	PatPathoIter pptIter = pPPT->end() ;
	pptIter-- ;

	if (((*pptIter)->getLexique() != HTMEMBERPOSITION) || ((*pptIter)->getColonne() != iCol))
		// erreur
		return false ;

	if (getSDistance() != "")
	{
		pPPT->ajoutePatho(HTMEMBERDISTANCE, iCol + 1) ;
    Message Msg ;
    Msg.SetComplement(getSDistance()) ;
    Msg.SetUnit(HTNOUNIT) ;
    pPPT->ajoutePatho(HTVALNUM2, &Msg, iCol + 2) ;
	}
	else
		// erreur - pour qu'un mandat soit valide il faut que la distance soit fixée
		return false ;

	if (getSAngle() != "")
	{
		pPPT->ajoutePatho(HTMEMBERANGLE, iCol + 1) ;
    Message Msg	;
    Msg.SetComplement(getSAngle()) ;
    Msg.SetUnit(HTANGUNIT) ;
    pPPT->ajoutePatho(HTVALNUM2, &Msg, iCol + 2) ;
	}
	else
		// erreur - pour qu'un mandat soit valide il faut que l'angle soit fixé
		return false ;

	pptNode = pptIter ;
  sNodeID = string("") ;

	return true ;
}


// -----------------------------------------------------------------------------
// modifie l'arbre correspondant à la position d'un membre de l'Equipe de Santé.
// --
// modify a HealthTeam member position tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::modifyTree(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol)
{
	// fabTODO
	if ((pptNode == NULL) || (*pptIter != pptNode))
		return false ;

	if (((**pptIter)->getLexique() != HTMEMBERPOSITION) || (**pptIter)->getColonne() != iCol)
		// erreur
		return false ;

	(*pptIter)++ ;
	while ((*pptIter != pPPT->end()) && ((**pptIter)->getColonne() > iCol))
	{
		if 			((**pptIter)->getLexique() == HTMEMBERDISTANCE)
			modifyDistance(pCtx, pPPT, pptIter, iCol) ;
		else if ((**pptIter)->getLexique() == HTMEMBERANGLE)
			modifyAngle(pCtx, pPPT, pptIter, iCol) ;
		else
			// erreur - error
			// il n'y a pas d'autres sous-arbres dans l'arbre représentant un mandat.
			// there are no others subtree in mandate tree.
			return false ;
	}
	return true ;
}

// -----------------------------------------------------------------------------
// modifie la partie d'arbre correspondant à la distance.
// --
// modify the distance in the tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::modifyDistance(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int /* iCol */)
{
	if ((NULL == pPPT) || (NULL == pptIter))
		return false ;

	if ((**pptIter)->getLexique() != HTMEMBERDISTANCE)
		// erreur
		return false ;

  PatPathoIter	pptFather = *pptIter ;
  (*pptIter)++ ;
  if ((string((**pptIter)->getLexique(), 0, 2) == HTVALNUM)	&& ((**pptIter)->getUnit() == HTNOUNIT))
  {
    if ((**pptIter)->getComplement() != getSDistance())
    {
      // création de la nouvelle patpatho
      Message	Msg ;
      Msg.SetComplement(getSDistance()) ;
      Msg.SetUnit(HTNOUNIT) ;

      NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
      PPTTemp.ajoutePatho(HTVALNUM2, &Msg, 0) ;

      // effacer la patpatho
      pPPT->SupprimerFils(pptFather) ;

      // insertion de la nouvelle patpatho
      pPPT->InserePatPathoFille(pptFather, &PPTTemp) ;

      *pptIter = pptFather ;
      (*pptIter)++ ;
    }
    (*pptIter)++ ;
		return true ;
  }
  else
    // erreur d'unité ou de code lexique.
    // unit or lexical code error.
    return false ;
}


// -----------------------------------------------------------------------------
// modifie la partie d'arbre correspondant à l'angle.
// --
// modify the angle in the tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamPosition::modifyAngle(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int /* iCol */)
{
	if ((NULL == pPPT) || (NULL == pptIter))
		return false ;

	if ((**pptIter)->getLexique() != HTMEMBERANGLE)
		// erreur
		return false ;

  PatPathoIter pptFather = *pptIter ;
  (*pptIter)++ ;

  if ((string((**pptIter)->getLexique(), 0, 2) == HTVALNUM)	&& ((**pptIter)->getUnit() == HTANGUNIT))
  {
    if ((**pptIter)->getComplement() != getSAngle())
    {
      // création de la nouvelle patpatho
      Message	Msg ;
      Msg.SetComplement(getSAngle()) ;
      Msg.SetUnit(HTANGUNIT) ;

      NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
      PPTTemp.ajoutePatho(HTVALNUM2, &Msg, 0) ;

      // effacer la patpatho
      pPPT->SupprimerFils(pptFather) ;

      // insertion de la nouvelle patpatho
      pPPT->InserePatPathoFille(pptFather, &PPTTemp) ;

      *pptIter = pptFather ;
      (*pptIter)++ ;
    }
    (*pptIter)++ ;
		return true ;
  }
  else
    // erreur d'unité ou de code lexique.
    // unit or lexical code error.
    return false ;
}

// -----------------------------------------------------------------------------
// donne la distance sous la forme d'une chaine de caractères.
// --
// get distance in a string.
// -----------------------------------------------------------------------------
string
NSHealthTeamPosition::getSDistance()
{
	string 	sLabel	= "" ;

	switch (getDistance())
	{
		case patDist			:	sLabel = "0" ;	break ;
		case nearDist			:	sLabel = "1" ;	break ;
		case farDist			:	sLabel = "2" ;	break ;
		case farAwayDist	:	sLabel = "3" ;	break ;
		default						: break ;
	}
	return sLabel ;
}

// -----------------------------------------------------------------------------
// donne l'angle sous la forme d'une chaine de caractères.
// --
// get angle in a string.
// -----------------------------------------------------------------------------
string
NSHealthTeamPosition::getSAngle()
{
	// l'angle est compris entre 0 et 360
	if ((getAngle() < 100) && (getAngle() >= 0))
	{
		char    szAngle[3] ;
		sprintf(szAngle, "%02d", getAngle()) ;
		string	sAngle = szAngle ;
		return sAngle ;
	}

	// erreur
	return "" ;
}

