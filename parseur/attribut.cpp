// -----------------------------------------------------------------------------
// attribut.cpp
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
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

#include "attribut.h"
#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus/nsdivfctForCgi.h"
#else
  #include "partage\nsdivfct.h"
#endif

long CAttributs::lObjectCount = 0 ;

CAttributs::CAttributs()
{
  _sLabel = string("") ;
  _sValue = string("") ;

  lObjectCount++ ;
}

CAttributs::CAttributs(string etiquette, string attribut, bool bMustConvert)
{
  // Modif 20080724

  if (true == bMustConvert)
  {
    _sLabel = texteHtml(etiquette) ;
    _sValue = texteHtml(attribut) ;
  }
  else
  {
    _sLabel = etiquette ;
	  _sValue = attribut ;
  }

  lObjectCount++ ;
}

CAttributs::CAttributs(const CAttributs& rv)
{
	_sLabel = rv._sLabel ;
	_sValue = rv._sValue ;

  lObjectCount++ ;
}

CAttributs::~CAttributs()
{
  lObjectCount-- ;
}

CAttributs&
CAttributs::operator=(const CAttributs& src)
{
  if (this == &src)
    return (*this) ;

	_sLabel = src._sLabel ;
	_sValue = src._sValue ;

  return (*this) ;
}

Attribut_array::Attribut_array()
               :AttrArray ()
{
}

Attribut_array::Attribut_array(const Attribut_array& rv)
               :AttrArray ()
{
try
{
  if (false == rv.empty())
    for (AttrCIter i = rv.begin() ; rv.end() != i ; i++)
      push_back(new CAttributs(*(*i))) ;
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
Attribut_array::vider()
{
  if (empty())
    return ;

  for (AttrIter i = begin() ; end() != i ; )
  {
    if (*i)
      delete (*i) ;
    erase(i) ;
  }
}

Attribut_array::~Attribut_array()
{
  vider() ;
}

bool
Attribut_array::traiterDate(string sEtiquette, int iTaille)
{
  if (empty())
    return true ;

  // Chercher l'attribut qui possède cette étiquette
  AttrIter i = begin() ;
  for ( ; (end() != i) && ((*i)->getLabel() != sEtiquette) ; i++)
    ;

  if (end() == i)
    return true ;

  string sValeur = (*i)->getValue() ;
  int    iSize   = strlen(sValeur.c_str()) ;
  string sNewValeur = "" ;

  bool bGood = true ;

  // La date doit être du type AAAA ou AAAA-MM ou AAAA-MM-JJ
  // Toute autre valeur doit être éliminée

  if (iSize == 4)
  {
    // type AAAA
    for (int j = 0 ; j < 4 ; j++)
      if (!isdigit(sValeur[j]))
        bGood = false ;


    if (bGood)
    {
      sNewValeur = sValeur ;
      if (iTaille > 4)
        sNewValeur += string(iTaille - 4, '0') ;
    }
  }
  else if (iSize == 7)
  {
    // type AAAA-MM
    for (int j = 0 ; j < 4 ; j++)
      if (!isdigit(sValeur[j]))
        bGood = false ;

    if ((bGood) && (sValeur[4] != '-'))
      bGood = false ;

    if (bGood)
    {
      for (int j = 0 ; j < 2 ; j++)
        if (!isdigit(sValeur[j + 5]))
          bGood = false ;
    }

    if (bGood)
    {
      sNewValeur = string(sValeur, 0, 4) + string(sValeur, 5, 2) ;
      if (iTaille > 6)
        sValeur += string(iTaille - 6, '0') ;
    }
  }
  else if (iSize == 10)
  {
    // type AAAA-MM-JJ
    for (int j = 0 ; j < 4 ; j++)
      if (!isdigit(sValeur[j]))
        bGood = false ;

    if ((bGood) && (sValeur[4] != '-'))
      bGood = false ;

    if (bGood)
    {
      for (int j = 0 ; j < 2 ; j++)
        if (!isdigit(sValeur[j + 5]))
          bGood = false ;
    }

    if ((bGood) && (sValeur[7] != '-'))
      bGood = false ;

    if (bGood)
    {
      for (int j = 0 ; j < 2 ; j++)
        if (!isdigit(sValeur[j + 8]))
          bGood = false ;
    }

    if (bGood)
      sNewValeur = string(sValeur, 0, 4) + string(sValeur, 5, 2) + string(sValeur, 8, 2) ;
  }

  // Test si on n'a que des 0
  if (bGood)
  {
    if (sNewValeur == string(strlen(sNewValeur.c_str()), '0'))
    {
      delete (*i) ;
      erase(i) ;
    }
    else
      (*i)->setValue(sNewValeur) ;
  }

  return bGood ;
}

Attribut_array&
Attribut_array::operator=(const Attribut_array& src)
{
  if (this == &src)
    return (*this) ;

  // Effacement des éléments déjà contenus dans le vecteur destination
  vider() ;

  // Copie et insertion des éléments de la source
try
{
  if (false == src.empty())
    for (AttrCIter i = src.begin() ; src.end() != i ; i++)
      push_back(new CAttributs(*(*i))) ;
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

string
Attribut_array::operator[](string sLabel)
{
  if (string("") == sLabel)
    return string("") ;

	if (false == empty())
		for (AttrIter i = begin() ; end() != i ; i++)
			if ((*i)->getLabel() == sLabel)
				return (*i)->getValue() ;

	return string("") ;
}

