// -----------------------------------------------------------------------------
// valeur.h
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.4 $
// $Author: pameline $
// $Date: 2013/11/11 23:15:51 $
// -----------------------------------------------------------------------------
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
#include <stdlib.h>
#include <string.h>
#include <string>
#include <fstream>

#include "valeur.h"
#include "balise.h"

long CValeur::lObjectCount = 0 ;

CValeur::CValeur(Cbalise *pere)
{
	_pObject = (Cbalise*) 0 ;
	_parent  = pere ;

  _sLabel     = string("") ;
  _sValue     = string("") ;
  _sAttribute = string("") ;

  _bObjectIsCopy = false ;

  lObjectCount++ ;
}

CValeur::CValeur(string label, string value, string attribute, Cbalise *pere)
{
	_sLabel      = label ;
	_sValue      = value ;
	_sAttribute  = attribute ;

	_pObject     = (Cbalise*) 0 ;
	_parent      = pere ;

  _bObjectIsCopy = false ;

  lObjectCount++ ;
}

CValeur::CValeur(const CValeur& rv, Cbalise *pere)
{
  _sLabel      = rv._sLabel ;
  _sValue      = rv._sValue ;
  _sAttribute  = rv._sAttribute ;
  _parent      = pere ;

  _bObjectIsCopy = false ;

  _pObject     = rv._pObject ;
  if (_pObject)
    _bObjectIsCopy = true ;

  lObjectCount++ ;

  /************* En commentaire suite au pb de link pour intégration dans NSChoisi
  Cpmed_patient* pPat = dynamic_cast<Cpmed_patient *>(rv.pObject) ;
  if (pPat)
  {
    pObject = new Cpmed_patient(*pPat) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_admindata* pAdm = dynamic_cast<Cpmed_admindata *>(rv.pObject) ;
  if (pAdm)
  {
    pObject = new Cpmed_admindata(*pAdm) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_healthdata* pHea = dynamic_cast<Cpmed_healthdata *>(rv.pObject) ;
  if (pHea)
  {
    pObject = new Cpmed_healthdata(*pHea) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_problem* pPbr = dynamic_cast<Cpmed_problem *>(rv.pObject) ;
  if (pPbr)
  {
    pObject = new Cpmed_problem(*pPbr) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_problem_event* pPbE = dynamic_cast<Cpmed_problem_event *>(rv.pObject) ;
  if (pPbE)
  {
    pObject = new Cpmed_problem_event(*pPbE) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_contact* pCon = dynamic_cast<Cpmed_contact *>(rv.pObject) ;
  if (pCon)
  {
    pObject = new Cpmed_contact(*pCon) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_contact_type* pCoT = dynamic_cast<Cpmed_contact_type *>(rv.pObject) ;
  if (pCoT)
  {
    pObject = new Cpmed_contact_type(*pCoT) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_contactelement* pCoE = dynamic_cast<Cpmed_contactelement *>(rv.pObject) ;
  if (pCoE)
  {
    pObject = new Cpmed_contactelement(*pCoE) ;
    pObject->parent = parent ;
    return ;
  }

  Cpmed_contactcode* pCoC = dynamic_cast<Cpmed_contactcode *>(rv.pObject) ;
  if (pCoC)
  {
    pObject = new Cpmed_contactcode(*pCoC) ;
    pObject->parent = parent ;
    return ;
  }
  ***********************************************************/
}

CValeur::CValeur(const CValeur& rv)
{
	_sLabel      = rv._sLabel ;
	_sValue      = rv._sValue ;
	_sAttribute  = rv._sAttribute ;
	_parent      = rv._parent ;

	_bObjectIsCopy = false ;

  _pObject     = rv._pObject ;
	if (_pObject)
    _bObjectIsCopy = true ;

  lObjectCount++ ;
}

CValeur&
CValeur::operator=(const CValeur& src)
{
  if (this == &src)
		return *this ;

  _sLabel      = src._sLabel ;
	_sValue      = src._sValue ;
	_sAttribute  = src._sAttribute ;
	_parent      = src._parent ;

	_bObjectIsCopy = false ;

  _pObject     = src._pObject ;
	if (_pObject)
    _bObjectIsCopy = true ;

  return *this ;
}

CValeur::~CValeur()
{
	if (_pObject && (false == _bObjectIsCopy))
		delete _pObject ;

  lObjectCount-- ;
}


Valeur_array::Valeur_array(Cbalise *pere)
             :ValArray()
{
  _parent = pere ;
}

Valeur_array::Valeur_array(const Valeur_array& rv)
             :ValArray()
{
try
{
  _parent = rv._parent ;
  if (false == rv.empty())
    for (ValCIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new CValeur(*(*i), _parent)) ;
}
/*
catch (xalloc)
{
  erreur("Mémoire insuffisante (new NSPatPathoArray copie).", 0, 0) ;
}
*/
catch (...)
{
  //pSuperviseur->erreur( pSuperviseur->tr("Exception (new module_running_array copie)."), 0, 0) ;
}
}


void
Valeur_array::vider()
{
  if (empty())
    return ;

  for (ValIter i = begin() ; end() != i ; )
  {
    delete (*i) ;
    erase(i) ;
  }
}

Valeur_array::~Valeur_array()
{
  vider() ;
}

Valeur_array&
Valeur_array::operator=(const Valeur_array& src)
{
  if (this == &src)
		return *this ;

  // Effacement des éléments déjà contenus dans le vecteur destination
  vider() ;
  _parent = src._parent ;

  // Copie et insertion des éléments de la source
try
{
  if (false == src.empty())
    for (ValCIter i = src.begin() ; src.end() != i ; i++)
      push_back(new CValeur(*(*i), _parent)) ;
}
/*
catch (xalloc)
{
  erreur("Mémoire insuffisante (op = NSPatPathoArray).", 0, 0) ;
}
*/
catch (...)
{
  //pSuperviseur->erreur( pSuperviseur->tr("Exception (op = module_running_array)."), 0, 0) ;
}

  return (*this) ;
}


/*
Valeur_array
Valeur_array::operator+(Valeur_array src)
{
  Valeur_array aTotal(parent) ;

  ValIter i ;

  // Copie et insertion des éléments de la source
try
{
  if (!(empty()))
  {
    for (i = begin() ; i != end() ; i++)
      aTotal.push_back(new CValeur(*(*i), parent)) ;
  }

  if (!(src.empty()))
  {
    for (i = src.begin() ; i != src.end() ; i++)
      aTotal.push_back(new CValeur(*(*i), parent)) ;
  }
}
catch (xalloc)
{
  erreur("Mémoire insuffisante (op = NSPatPathoArray).", 0, 0) ;
}
catch (...)
{
  //pSuperviseur->erreur( pSuperviseur->tr("Exception (op = module_running_array)."), 0, 0) ;
}

  return aTotal ;
}
*/

