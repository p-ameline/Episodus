// -----------------------------------------------------------------------------
// nsHealthTeamMember.cpp
// -----------------------------------------------------------------------------
// Gestion d'un membre d'une Equipe
// -----------------------------------------------------------------------------
// $Revision: 1.12 $
// $Author: pameline $
// $Date: 2014/01/29 20:43:31 $
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
  #include "enterpriseLus/superLus.h"
  #include "enterpriseLus/personLus.h"
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "nautilus\nssuper.h"
  #include "nsldv\nsldvuti.h"
#endif

#include "nsbb\nsbbtran.h"
#include "nssavoir\nsfilgd.h"

long NSHealthTeamMandate::lObjectCount = 0 ;

// -----------------------------------------------------------------------------
// class NSHealthTeamMandate
// -----------------------------------------------------------------------------
// classe représentant un mandat d'un membre de l'Equipe de Santé.
// --
// class that represents the mandate of an HealthTeamMember.
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// constructeur par défaut
// --
// default constructor
// -----------------------------------------------------------------------------
NSHealthTeamMandate::NSHealthTeamMandate(mandateType typeUsed)
{
	_sProfession      = string("") ;
	_sSpeciality      = string("") ;
	_mandateBeginDate = string("") ;
	_mandateEndDate   = string("") ;

	_typeOfMandate    = typeUsed ;

	_pptNode          = NULL ;
  _sNodeID          = string("") ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// constructeur qui initialise toutes les variables
// --
// complete constructor
// -----------------------------------------------------------------------------
NSHealthTeamMandate::NSHealthTeamMandate(DistanceType dist, int iAng, string sProf, string sSpec, string begin, string end, string durVal, string durUnit, mandateType typeUsed)
                    :_position(dist, iAng)
{
	_sProfession      = sProf ;
	_sSpeciality      = sSpec ;
	_mandateBeginDate = begin ;
	_mandateEndDate   = end ;

	_typeOfMandate    = typeUsed ;

	_pptNode          = NULL ;
  _sNodeID          = string("") ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// constructeur copie
// --
// copy constructor
// -----------------------------------------------------------------------------
NSHealthTeamMandate::NSHealthTeamMandate(const NSHealthTeamMandate& src)
                    :_position(src._position)
{
	_sProfession      = src._sProfession ;
	_sSpeciality      = src._sSpeciality ;
	_mandateBeginDate = src._mandateBeginDate ;
	_mandateEndDate   = src._mandateEndDate ;

	_typeOfMandate    = src._typeOfMandate ;

	_pptNode          = src._pptNode ;
  _sNodeID          = src._sNodeID ;

  lObjectCount++ ;
}

// -----------------------------------------------------------------------------
// destructeur
// --
// destructor
// -----------------------------------------------------------------------------
NSHealthTeamMandate::~NSHealthTeamMandate()
{
  lObjectCount-- ;
}

// -----------------------------------------------------------------------------
// opérateur =
// --
// operator =
// -----------------------------------------------------------------------------
NSHealthTeamMandate&
NSHealthTeamMandate::operator=(const NSHealthTeamMandate& src)
{
	if (this == &src)
		return *this ;

  _position         = src._position ;

	_sProfession      = src._sProfession ;
	_sSpeciality      = src._sSpeciality ;
	_mandateBeginDate = src._mandateBeginDate ;
	_mandateEndDate   = src._mandateEndDate ;

	_typeOfMandate    = src._typeOfMandate ;

	_pptNode          = src._pptNode ;
  _sNodeID          = src._sNodeID ;

	return (*this) ;
}

void
NSHealthTeamMandate::setDuration(string sDuration, string sUnit)
{
	NVLdVTemps	tEnd ;
  tEnd.initFromDateHeure(_mandateBeginDate) ;

  if 			(sUnit == "2DAT31")		// année
    tEnd.ajouteAns(atoi(sDuration.c_str())) ;
  else if (sUnit == "2DAT21")		// mois
    tEnd.ajouteMois(atoi(sDuration.c_str())) ;
  else if (sUnit == "2DAT11")		// semaine
    tEnd.ajouteJours(7 * atoi(sDuration.c_str())) ;
  else if (sUnit == "2DAT01")		// jour
    tEnd.ajouteJours(atoi(sDuration.c_str())) ;
  else if (sUnit == "2HEUR1")		// heure
    tEnd.ajouteHeures(atoi(sDuration.c_str())) ;
  else if (sUnit == "2MINU1")		// minute
    tEnd.ajouteMinutes(atoi(sDuration.c_str())) ;
  else if (sUnit == "2SEC01")		// seconde
    tEnd.ajouteSecondes(atoi(sDuration.c_str())) ;
  else
  {
    // erreur unité inconnue ou non traitée
  }

  setEndDate(tEnd.donneDateHeure()) ;
}

// -----------------------------------------------------------------------------
// retourne le statut du mandat.
// --
// return mandate status.
// -----------------------------------------------------------------------------
int
NSHealthTeamMandate::getStatus()
{
	// 0 - 0 : utilisateur non actif						- inactive user
	// 1 - 1 : utilisateur actif								- active user
	// A - 2 : administrateur actif							- active root
	// T - 3 : administrateur de tutelle actif	- active super root (like patient)

	switch (_typeOfMandate)
	{
		case user : if (isValid())
									return 1 ;
								break ;

		case root	: if (isValid())
								{
									if (getDistance() == 0)
										return 3 ;
									return 2 ;
								}
								break ;

		default		:	break ;
	}

	return 0 ;
}

// -----------------------------------------------------------------------------
// est-ce que le mandat est valide ?
// --
// is mandate valid ?
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::isValid()
{
	if ((getBeginDate() == "") || (getEndDate() == ""))
		return false ;

  if (getEndDate() >= getBeginDate())
  	return true ;
  return false ;
}

// -----------------------------------------------------------------------------
// est-ce que le mandat est actif ?
// --
// is mandate active ?
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::isActive()
{
	if (false == isValid())
  	return false ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;
  NVLdVTemps tpsBeg ;
  tpsBeg.initFromDateHeure(getBeginDate()) ;
  NVLdVTemps tpsEnd ;
  tpsEnd.initFromDateHeure(getEndDate()) ;

  if ((tpsBeg <= tpsNow) && (tpsEnd >= tpsNow))
		return true ;
  return false ;
}

// -----------------------------------------------------------------------------
// initilisation d'un mandat à partir d'une PatPatho.
// --
// initialization of a mandate from a PatPatho.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::initFromPatho(NSPatPathoArray *pPPT, PatPathoIter *pptIter)
{
	_pptNode	= NULL ;
  _sNodeID = string("") ;

	if (((NSPatPathoArray*) NULL == pPPT) || (NULL == pptIter))
		return false ;

	// fabTODO

	if (((**pptIter)->getLexique() != HTMEMBERUSERMANDATE) && ((**pptIter)->getLexique() != HTMEMBERROOTMANDATE))
		return false ;

	_pptNode	= (*pptIter) ;
  _sNodeID = (**pptIter)->getNode() ;

	int		iColBase = (**pptIter)->getColonne() ;
	(*pptIter)++ ;

	while (((*pptIter) != pPPT->end()) && ((**pptIter)->getColonne() > iColBase))
	{
		if      ((**pptIter)->getLexique() == HTMEMBERPROFESSION)
		{
			(*pptIter)++ ;
      if (string("£?????") != (**pptIter)->getLexique())
			  setProfession((**pptIter)->getLexique()) ;
      else
        setProfession("") ;
			(*pptIter)++ ;
		}
		else if ((**pptIter)->getLexique() == HTMEMBERSPECIALITY)
		{
			(*pptIter)++ ;
      if (string("£?????") != (**pptIter)->getLexique())
			  setSpeciality((**pptIter)->getLexique()) ;
      else
        setSpeciality("") ;
			(*pptIter)++ ;
		}
		else if ((**pptIter)->getLexique() == HTMEMBERPOSITION)
			_position.initFromPatho(pPPT, pptIter) ;
		else if ((**pptIter)->getLexique() == HTMEMBERMANDATEBEGIN)
		{
			(*pptIter)++ ;
			if (((**pptIter)->getLexique() == HTVALDATE)	&& ((**pptIter)->getUnit() == HTDATE))
			{
				setBeginDate((**pptIter)->getComplement()) ;
				(*pptIter)++ ;
      }
			else
				return false ;
		}
		else if ((**pptIter)->getLexique() == HTMEMBERMANDATEEND)
		{
			(*pptIter)++ ;
			if (((**pptIter)->getLexique() == HTVALDATE)	&& ((**pptIter)->getUnit() == HTDATE))
			{
				setEndDate((**pptIter)->getComplement()) ;
				(*pptIter)++ ;
      }
			else
				return false ;
		}
		else
		{
			// erreur
			return false ;
		}
	}
	return true ;
}

// -----------------------------------------------------------------------------
// créé l'arbre correspondant au mandat d'un membre de l'Equipe de Santé.
// --
// creates an HealthTeam member mandate tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::createTree(NSPatPathoArray *pPPT, int iCol)
{
	if (NULL == pPPT)
		return false ;

	// noeud racine du mandat - root node of mandate
	switch (_typeOfMandate)
	{
		case user	:	pPPT->ajoutePatho(HTMEMBERUSERMANDATE, iCol) ;	break ;
		case root	: pPPT->ajoutePatho(HTMEMBERROOTMANDATE, iCol) ;	break ;
		default		: return false ;
	}

	PatPathoIter pptIter = pPPT->end() ;
	pptIter-- ;

	if ((((*pptIter)->getLexique() != HTMEMBERUSERMANDATE) && ((*pptIter)->getLexique() != HTMEMBERROOTMANDATE)) || ((*pptIter)->getColonne() != iCol))
		// erreur
		return false ;

	// métier - profession
	if (getProfession() != "")
	{
		pPPT->ajoutePatho(HTMEMBERPROFESSION, iCol + 1) ;
		pPPT->ajoutePatho(getProfession(), iCol + 2) ;
	}

	// spécialité - speciality
	if (getSpeciality() != "")
	{
		pPPT->ajoutePatho(HTMEMBERSPECIALITY, iCol + 1) ;
		pPPT->ajoutePatho(getSpeciality(), iCol + 2) ;
	}

	// position - position
	_position.createTree(pPPT, iCol + 1) ;

	// date de début - beginning date
	if (getBeginDate() != "")
	{
		pPPT->ajoutePatho(HTMEMBERMANDATEBEGIN, iCol + 1) ;
    Message	Msg ;
    Msg.SetComplement(getBeginDate()) ;
    Msg.SetUnit(HTDATE) ;
    pPPT->ajoutePatho(HTVALDATE, &Msg, iCol + 2) ;
	}
	else
		// erreur la date de début de mandat est obligatoire.
		// error beginning date is necessary.
		return false ;

	if 			(getEndDate() != "")
	{
    // date de fin - ending date.
		pPPT->ajoutePatho(HTMEMBERMANDATEEND, iCol + 1) ;
    Message	Msg	;
    Msg.SetComplement(getEndDate()) ;
    Msg.SetUnit(HTDATE) ;
    pPPT->ajoutePatho(HTVALDATE, &Msg, iCol + 2) ;
	}
	else
		// erreur - il faut soit une date de fin de mandat.
		// error - it must to have a mandate ending date.
		return false ;

	_pptNode = pptIter ;
  _sNodeID = string("") ;

	return true ;
}

// -----------------------------------------------------------------------------
// modifie l'arbre correspondant au mandat d'un membre de l'Equipe de Santé.
// --
// modify a mandate from an HealthTeam member.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::modifyTree(NSContexte *pCtx, NSPatPathoArray *pPPT, int iCol)
{
	// fabTODO
	if ((NULL == pPPT) || ((NULL == _pptNode) && (string("") == _sNodeID)))
		return false ;

	PatPathoIter pptIter = NULL ;
  if (string("") != _sNodeID)
  	pptIter = pPPT->ChercherNoeud(_sNodeID) ;
	else
  	pptIter = _pptNode ;

	if (NULL == pptIter)
		return false ;

	if ((((*pptIter)->getLexique() != HTMEMBERUSERMANDATE) &&
       ((*pptIter)->getLexique() != HTMEMBERROOTMANDATE)) ||
      (*pptIter)->getColonne() != iCol)
		// erreur
		return false ;

	switch (getType())
	{
		// fabTODO
		// voir comment on peut changer un noeud d'une patpatho sans changer ses fils
		case user	:	if ((*pptIter)->getLexique() != HTMEMBERUSERMANDATE)
								{
									// erreur - ce n'est pas le même mandat
									return false ;
                }
								break ;
		case root	: if ((*pptIter)->getLexique() != HTMEMBERROOTMANDATE)
								{
									// erreur - ce n'est pas le même mandat
									return false ;
                }
								break ;
		default		:	return false ;
	}

	pptIter++ ;

	bool bProfession = false ;
	bool bSpeciality = false ;

  bool bIsOk = true ;
	while ((pptIter != pPPT->end()) && ((*pptIter)->getColonne() > iCol) && bIsOk)
	{
		if 			((*pptIter)->getLexique() == HTMEMBERPOSITION)
      bIsOk = _position.modifyTree(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;

		else if ((*pptIter)->getLexique() == HTMEMBERPROFESSION)
		{
			// fabTODO
			bIsOk = modifyProfession(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;
			bProfession = true ;
		}
		else if ((*pptIter)->getLexique() == HTMEMBERSPECIALITY)
		{
			bIsOk = modifySpeciality(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;
			bSpeciality = true ;
		}
		else if ((*pptIter)->getLexique() == HTMEMBERMANDATEBEGIN)
		{
			bIsOk = modifyBeginDate(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;
		}
		else if ((*pptIter)->getLexique() == HTMEMBERMANDATEEND)
		{
			bIsOk = modifyEndDate(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;
    }
		else
		{
			// erreur - error
			// il n'y a pas d'autres sous-arbres dans l'arbre représentant un mandat.
			// there are no others subtree in mandate tree.
			return false ;
		}
	}

	if ((false == bProfession) && (getProfession() != ""))
  {
    NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
		PPTTemp.ajoutePatho(HTMEMBERPROFESSION, 0) ;
		PPTTemp.ajoutePatho(getProfession(), 1) ;

    // insertion de la nouvelle patpatho
    pPPT->InserePatPathoFille(_pptNode, &PPTTemp) ;
  }

	if ((false == bSpeciality) && (getSpeciality() != ""))
	{
    NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
		PPTTemp.ajoutePatho(HTMEMBERSPECIALITY, 0) ;
		PPTTemp.ajoutePatho(getSpeciality(), 1) ;

    // insertion de la nouvelle patpatho
    pPPT->InserePatPathoFille(_pptNode, &PPTTemp) ;
	}

  return true ;
}

// -----------------------------------------------------------------------------
// modifie la partie de l'arbre mandat correspondant au métier.
// --
// modify the profession in the mandate tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::modifyProfession(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol)
{
	if ((NULL == pPPT) || (NULL == pptIter))
  	return false ;

  if (((**pptIter)->getLexique() != HTMEMBERPROFESSION) || ((**pptIter)->getColonne() != iCol))
		// erreur
		return false ;

	if (getProfession() == "")
	{
		pPPT->SupprimerItem(*pptIter) ;
		return true ;
	}

  PatPathoIter	pptFather = *pptIter ;
  (*pptIter)++ ;
  if ((**pptIter)->getLexique() != getProfession())
	{
		// création de la nouvelle patpatho
		// create new patpatho
    NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;

    PPTTemp.ajoutePatho(getProfession(), 0) ;

		// efface la patpatho fille
		// delete subpatpatho
		pPPT->SupprimerFils(pptFather) ;

		// insertion de la nouvelle patpatho
		// inserting new patpatho
		pPPT->InserePatPathoFille(pptFather, &PPTTemp) ;

		*pptIter = pptFather ;
		(*pptIter)++ ;
	}

  return true ;
}

// -----------------------------------------------------------------------------
// modifie la partie de l'arbre mandat correspondant à la spécialité.
// --
// modify the speciality in the mandate tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::modifySpeciality(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol)
{
	if ((NULL == pPPT) || (NULL == pptIter))
  	return false ;

  if (((**pptIter)->getLexique() != HTMEMBERSPECIALITY) || ((**pptIter)->getColonne() != iCol))
		// erreur
		return false ;

	if (getSpeciality() == "")
	{
		pPPT->SupprimerItem(*pptIter) ;
		return true ;
	}

  PatPathoIter	pptFather = *pptIter ;
  (*pptIter)++ ;
  if ((**pptIter)->getLexique() != getSpeciality())
	{
		// création de la nouvelle patpatho - create new patpatho
    NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
    PPTTemp.ajoutePatho(getSpeciality(), 0) ;

		// efface la patpatho fille - delete subpatpatho
		pPPT->SupprimerFils(pptFather) ;

		// insertion de la nouvelle patpatho - inserting new patpatho
		pPPT->InserePatPathoFille(pptFather, &PPTTemp) ;

		*pptIter = pptFather ;
		(*pptIter)++ ;
	}

  return true ;
}

// -----------------------------------------------------------------------------
// modifie la partie de l'arbre mandat correspondant à la date de début.
// --
// modify the beginning date in the mandate tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::modifyBeginDate(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol)
{
	if ((NULL == pPPT) || (NULL == pptIter))
  	return false ;

	if (((**pptIter)->getLexique() != HTMEMBERMANDATEBEGIN) || ((**pptIter)->getColonne() != iCol))
		// erreur
		return false ;

  PatPathoIter	pptFather = *pptIter ;
  (*pptIter)++ ;
  if (((**pptIter)->getLexique() == HTVALDATE)	&& ((**pptIter)->getUnit() == HTDATE))
  {
    if ((**pptIter)->getComplement() != getBeginDate())
    {
      // création de la nouvelle patpatho
      Message	Msg ;
      Msg.SetComplement(getBeginDate()) ;
      Msg.SetUnit(HTDATE) ;

      NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
      PPTTemp.ajoutePatho(HTVALDATE, &Msg, 0) ;

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
// modifie la partie de l'arbre mandat correspondant à la date de fin.
// --
// modify the ending date in the mandate tree.
// -----------------------------------------------------------------------------
bool
NSHealthTeamMandate::modifyEndDate(NSContexte *pCtx, NSPatPathoArray *pPPT)
{
	if ((NULL == pPPT) || (true == pPPT->empty()))
  	return false ;

  PatPathoIter pptIter = NULL ;
  if (string("") != _sNodeID)
  	pptIter = pPPT->ChercherNoeud(_sNodeID) ;
	else
  	pptIter = _pptNode ;

	if (NULL == pptIter)
		return false ;

  for (; (pPPT->end() != pptIter) && ((*pptIter)->getLexique() != HTMEMBERMANDATEEND) ; pptIter++)
  ;

	if (pPPT->end() == pptIter)
		return false ;

	return modifyEndDate(pCtx, pPPT, &pptIter, (*pptIter)->getColonne()) ;
}

bool
NSHealthTeamMandate::modifyEndDate(NSContexte *pCtx, NSPatPathoArray *pPPT, PatPathoIter *pptIter, int iCol)
{
	if ((NULL == pPPT) || (NULL == pptIter))
  	return false ;

	if (((**pptIter)->getLexique() != HTMEMBERMANDATEEND) || ((**pptIter)->getColonne() != iCol))
		// erreur
		return false ;

  PatPathoIter	pptFather = *pptIter ;
  (*pptIter)++ ;
  if (((**pptIter)->getLexique() == HTVALDATE)	&& ((**pptIter)->getUnit() == HTDATE))
  {
    if ((**pptIter)->getComplement() != getEndDate())
    {
      // création de la nouvelle patpatho
      Message	Msg ;
      Msg.SetComplement(getEndDate()) ;
      Msg.SetUnit(HTDATE) ;

      NSPatPathoArray	PPTTemp(pCtx->getSuperviseur()) ;
      PPTTemp.ajoutePatho(HTVALDATE, &Msg, 0) ;

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

string
NSHealthTeamMandate::getProfessionLabel(NSContexte *pCtx)
{
	string	sLanguage 			= pCtx->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getProfession() ;
	string	sLexiCodeLabel	= "" ;

  if (string("") != sLexiCode)
		pCtx->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;
  return sLexiCodeLabel ;
}

string
NSHealthTeamMandate::getSpecialityLabel(NSContexte *pCtx)
{
	string	sLanguage 			= pCtx->getUtilisateur()->donneLang() ;
	string	sLexiCode				= getSpeciality() ;
	string	sLexiCodeLabel	= "" ;

  if ((string("") != sLexiCode) && (string("£?????") != sLexiCode))
		pCtx->getDico()->donneLibelle(sLanguage, &sLexiCode, &sLexiCodeLabel) ;

  return sLexiCodeLabel ;
}

// -----------------------------------------------------------------------------
// récupère le libellé correspondant à un angle donné.
// --
// get the text related to an angle.
// -----------------------------------------------------------------------------
string
NSHealthTeamMandate::getStringAngle(NSContexte *pCtx)
{
	string sLabel = "";

  switch (_position.getAngle())
  {
  	case 0	:	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle0") ;	break ;

  	case 3	:	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle3") ;	break ;

    case 9	:	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle9") ;	break ;

    case 15	:	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle15") ;	break ;

    case 17 :	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle17") ;	break ;

    case 21 :	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "angle21") ;	break ;

    default	:	erreur("Angle non défini dans NSHealthTeamMandate.", standardError) ;
  }
  return sLabel ;
}

// -----------------------------------------------------------------------------
// récupère le libellé correspondant à une distance.
// --
// get the text related to a distance.
// -----------------------------------------------------------------------------
string
NSHealthTeamMandate::getDistanceTxt(NSContexte *pCtx)
{
	string sLabel = string("") ;

  switch (_position.getDistance())
  {
  	case patDist	:
    	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "distance0") ;
      break ;

    case nearDist	:
    	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "distance1") ;
      break ;

    case farDist	:
    	sLabel = pCtx->getSuperviseur()->getText("rosaceManagement", "distance2") ;
      break ;
  }

  return sLabel ;
}

// -----------------------------------------------------------------------------
// récupère la date de début d'un mandat.
// --
// get a mandate beginning date.
// -----------------------------------------------------------------------------
void
NSHealthTeamMandate::getBeginDate(NVLdVTemps *pTps)
{
	if (NULL == pTps)
		return ;

  pTps->initFromDateHeure(_mandateBeginDate) ;
}


// -----------------------------------------------------------------------------
// récupère la date de fin d'un mandat.
// --
// get a mandate ending date.
// -----------------------------------------------------------------------------
void
NSHealthTeamMandate::getEndDate(NVLdVTemps *pTps)
{
	if (NULL == pTps)
		return ;

  pTps->initFromDateHeure(_mandateEndDate) ;
}

void
#ifndef _ENTERPRISE_DLL
NSHealthTeamMandate::autoInitMandate(NSPersonInfo *pPersonInfo)
#else
NSHealthTeamMandate::autoInitMandate(Person *pPersonInfo)
#endif
{
	if (NULL == pPersonInfo)
		return ;

#ifndef _ENTERPRISE_DLL
	_sProfession = pPersonInfo->getMetier() ;
	_sSpeciality = pPersonInfo->getSpecialite() ;

  NSSuper* pSuper = pPersonInfo->_pSuper ;
#else
	_sProfession = pPersonInfo->getProJob() ;
	_sSpeciality = pPersonInfo->getProSpeciality() ;

  NSSuper* pSuper = pPersonInfo->getContext()->getSuperviseur() ;
#endif

  setDistance(nearDist) ;

  NVLdVTemps tpsNow ;
  tpsNow.takeTime() ;
  setBeginDate(tpsNow.donneDateHeure()) ;

  NVLdVTemps tpsEndDate ;
  tpsEndDate.setNoLimit() ;
  setEndDate(tpsEndDate.donneDateHeure()) ;

	if (string("") != _sProfession)
	{
  	string sJobSens ;
  	NSDico::donneCodeSens(&_sProfession, &sJobSens);
    if (string("DMEDE") == sJobSens)
    	setAngle(3) ;
    else
    {
    	VectString VSIsAHealthProfessionnalList ;
			pSuper->getFilGuide()->TousLesVrais("DPROS", "ES", &VSIsAHealthProfessionnalList) ;
  		if (VSIsAHealthProfessionnalList.contains(sJobSens))
      	setAngle(9) ;
    }
	}
  else
		setAngle(3) ;
}

