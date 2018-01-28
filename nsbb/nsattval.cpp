// -----------------------------------------------------------------------------
// nsattval.cpp
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.20 $
// $Author: pameline $
// $Date: 2015/02/07 12:50:20 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// FLP - july/august 2003
// RS  - july 2003
// FLP - june 2003
// RS  - november 2002// -----------------------------------------------------------------------------

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

#include <bwcc.h>
#include <commctrl.h>

#include "nsbb\nsattval.h"#include "nsbb\nsarcParseError.h"#include "nsbb\nsarc.h"#include "nsbb\nspatpat.h"#include "partage\ns_search.h"#ifdef _ENTERPRISE_DLL  #include "enterpriseLus\superLus.h"
  #include "..\episodus\nsbb\nsbb.rh"
#else
  #include "nautilus\nssuper.h"
  #include "nautilus\nsepicap.h"
  #include "nsbb\nsbb.rh"
#endif#include "ns_ob1\Interface.h"//#include "nsbb\nsbbtran.h"#include "nsbb\nsattval_glif.h"/*#ifdef __OB1__# include "ns_ob1\nautilus-bbk.h"#else# include "ns_bbk\bb1bb.h"#endif*/Carchetype::Carchetype(string attributs, string values, Cbalise *father, NSSuper* pSuper)           :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}


Carchetype::~Carchetype()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      Cdialogue *pCDlg = dynamic_cast<Cdialogue*>(pBalise) ;
      if (pCDlg)
      {
        delete pCDlg ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Cdialogbox *pCBox = dynamic_cast<Cdialogbox*>(pBalise) ;
        if (pCBox)
        {
          delete pCBox ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Cpresentation *pCPres = dynamic_cast<Cpresentation*>(pBalise) ;
          if (pCPres)
          {
            delete pCPres ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            Creferences *pCRef = dynamic_cast<Creferences*>((*ival)->getObject()) ;
            if (pCRef)
            {
              delete pCRef ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
            else
            {
              Citem *pCItem = dynamic_cast<Citem*>((*ival)->getObject()) ;
              if (pCItem)
              {
                delete pCItem ;
                (*ival)->setObject((Cbalise*) 0) ;
              }
              else
              {
                Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>((*ival)->getObject()) ;
                if (pCCtr)
                {
                  delete pCCtr ;
                  (*ival)->setObject((Cbalise*) 0) ;
                }
              }
            }
          }
        }
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

Carchetype::Carchetype(const Carchetype& rv)
           :Cbalise(rv)
{
  _pSuper = rv._pSuper ;
}


Carchetype&
Carchetype::operator=(const Carchetype& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this;
	Cbalise* pbdata2 = (Carchetype*)&src;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2;

	_pSuper = src._pSuper ;

  return (*this) ;
}


bool
Carchetype::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_ARCHETYPE_NAME) == "")
  {
    // erreur (pas de nom)
    _iParsingError = EARCHETYPE_NONAME ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

  int nb_dialogue     = 0 ;
  int nb_dialogbox    = 0 ;
  int nb_items        = 0 ;
  int nb_contraintes  = 0 ;
  int nb_present      = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_DIALOGUE == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cdialogue((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_dialogue++ ;
		}
    else if (LABEL_DIALOGBOX == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cdialogbox((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_dialogbox++ ;
		}
    else if (LABEL_PRESENTATION == (*ival)->getLabel()) // modèle html de présentation
		{
      (*ival)->setObject(new Cpresentation((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_present++ ;
		}
    else if (LABEL_REFERENCES == (*ival)->getLabel()) // references
    {
      (*ival)->setObject(new Creferences((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
    }
    else if (LABEL_ITEMS == (*ival)->getLabel()) // items
    {
      (*ival)->setObject(new Citem((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper, true)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_items++ ;
    }
    else if (LABEL_CONTRAINTES == (*ival)->getLabel()) // contraintes
    {
      (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this, true)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing contraintes
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_contraintes++ ;
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

	if ((nb_dialogue > 1) /* || (nb_dialogbox > 1)*/ || (nb_items != 1) || (nb_contraintes > 1) || (nb_present > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
	return true ;
}boolCarchetype::verif(){
  return true ;
}

bool
Carchetype::compresser()
{
  return true ;
}

bool
Carchetype::traiter()
{
  return true ;
}

string
Carchetype::getName()
{
  return getStringAttribute(ATTRIBUT_ARCHETYPE_NAME) ;
}

string
Carchetype::getFunction()
{
  return getStringAttribute(ATTRIBUT_ARCHETYPE_FUNCT) ;
}

Citem *
Carchetype::getRootItem()
{
  Citem *pRootItem = (Citem*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère le père des items de l'archetype
      if (LABEL_ITEMS == (*ival)->getLabel()) // items
      {
        pRootItem = dynamic_cast<Citem *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pRootItem ;
}

Cdialogue *
Carchetype::getDialog()
{
  Cdialogue *pDialog = (Cdialogue*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_DIALOGUE == (*ival)->getLabel())
      {
        pDialog = dynamic_cast<Cdialogue *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pDialog ;
}

Creferences *
Carchetype::getReference()
{
  Creferences *pRef = NULL ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_REFERENCES == (*ival)->getLabel())
      {
        pRef = dynamic_cast<Creferences *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pRef ;
}

Chead *
Carchetype::getHead(string sLang)
{
  Creferences *pReference = getReference() ;
  if (NULL == pReference)
    return (Chead *) 0 ;

  return pReference->getHead(sLang) ;
}

string
Carchetype::getTitle(string sLang)
{
  Chead *pHead = getHead(sLang) ;
  if (NULL == pHead)
    return string("") ;

  return pHead->getTitle() ;
}

Cdialogbox *
Carchetype::getDialogBox(string sLang)
{
  Cdialogbox  *pDefDialogBox = (Cdialogbox*) 0 ;
  Cdialogbox  *pDialogBox    = (Cdialogbox*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin (); ival != vect_val.end (); ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_DIALOGBOX == (*ival)->getLabel())
      {
        pDialogBox = dynamic_cast<Cdialogbox *>((*ival)->getObject()) ;
        if (pDialogBox)
        {
        	if (pDialogBox->getLang() == sLang)
        		break ;
          if (pDialogBox->getLang() == "")
          	pDefDialogBox = pDialogBox ;
        }
      }
    }
  }
  if (pDialogBox && (pDialogBox->getLang() == sLang))
  	return pDialogBox ;
  else
  	return pDefDialogBox ;
}

Cpresentation *
Carchetype::getPresentation()
{
  Cpresentation *pPres = (Cpresentation*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet presentation
      if (LABEL_PRESENTATION == (*ival)->getLabel())
      {
        pPres = dynamic_cast<Cpresentation *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pPres ;
}

string
Carchetype::getHelpUrl(string sLang)
{
  Creferences *pRef = getReference() ;
  if (NULL == pRef)
    return string("") ;

  return pRef->getHelpUrl(sLang) ;
}

// -----------------------------------------------------------------------------
//
// Classe Cdialogue
//
// -----------------------------------------------------------------------------

Cdialogue::Cdialogue(string attributs, string values, Cbalise *father)
          :Cbalise(attributs, values, father)
{
}

Cdialogue::~Cdialogue()
{
}

bool
Cdialogue::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EDIALOGUE_ATTR_EMPTY ;
    return false ;
  }

	int nb_dll = 0 ;
  int nb_nom = 0 ;
  int nb_ref = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
    if      (ATTRIBUT_DIALOGUE_NOM == (*iattribut)->getLabel()) // nom du dialogue
			nb_nom++ ;
    else if (ATTRIBUT_DIALOGUE_DLL == (*iattribut)->getLabel()) // dll
			nb_dll++ ;
    else if (ATTRIBUT_DIALOGUE_REF == (*iattribut)->getLabel()) // referentiel
			nb_ref++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EDIALOGUE_ATTR ;
      return false ;
    }
  }

	if ((nb_nom > 1) || (nb_dll > 1) || (nb_ref > 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EDIALOGUE_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = EDIALOGUE_VAL_NOT_EMPTY ;
    return false ;
  }
	return true ;
}

boolCdialogue::verif(){
  return true ;
}

bool
Cdialogue::compresser()
{
  return true ;
}

bool
Cdialogue::traiter()
{
  return true ;
}

// -----------------------------------------------------------------------------
//
// Classe Creferences
//
// -----------------------------------------------------------------------------

Creferences::Creferences(string attributs, string values, Cbalise *father)
            :Cbalise(attributs, values, father)
{
}

Creferences::~Creferences()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cconcern *pCCrn = dynamic_cast<Cconcern*>(pBalise) ;
      if (pCCrn)
      {
        delete pCCrn ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Chead *pCHead = dynamic_cast<Chead*>(pBalise) ;
        if (pCHead)
        {
          delete pCHead ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Creferences::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_CONCERN == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cconcern((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if (LABEL_HEAD == (*ival)->getLabel()) // header
		{
      (*ival)->setObject(new Chead((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }
	return true ;
}boolCreferences::verif(){
  return true ;
}

bool
Creferences::compresser()
{
  return true ;
}

bool
Creferences::traiter()
{
  return true ;
}

Cconcern *
Creferences::getFirstCconcern()
{
  Cconcern *pRef = (Cconcern *) 0 ;

  if (vect_val.empty())
    return pRef ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_CONCERN == (*ival)->getLabel())
    {
      pRef = dynamic_cast<Cconcern *>((*ival)->getObject()) ;
      break ;
    }
  }

  return pRef ;
}

Cconcern *
Creferences::getNextCconcern(Cconcern *pPrevious)
{
  Cconcern *pRef = (Cconcern *) 0 ;

  if (vect_val.empty())
  	return pRef ;

  bool bReturnNext = false ;
  if (NULL == pPrevious)
  	bReturnNext = true ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
  	// on récupère l'objet dialogue
    if (LABEL_CONCERN == (*ival)->getLabel())
    {
    	Cconcern *pBuf = dynamic_cast<Cconcern *>((*ival)->getObject()) ;
      if (pBuf && bReturnNext)
      {
      	pRef = pBuf ;
        break ;
      }
      if (pBuf == pPrevious)
      	bReturnNext = true ;
    }
  }

  return pRef ;
}

Chead *
Creferences::getHead(string sLang)
{
  Chead *pDefRef = NULL ;
  Chead *pRef    = NULL ;

  if (vect_val.empty())
  	return pRef ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
  	// on récupère l'objet dialogue
    if (LABEL_HEAD == (*ival)->getLabel())
    {
    	pRef = dynamic_cast<Chead *>((*ival)->getObject()) ;
      if (pRef)
      {
      	if (pRef->getLang() == sLang)
        	break ;
        if (pRef->getLang() == "")
        	pDefRef = pRef ;
      }
    }
  }
  if (pRef && (pRef->getLang() == sLang))
  	return pRef ;
  else
  	return pDefRef ;
}

string
Creferences::getHelpUrl(string sLang)
{
  Chead *pHeadForLang = getHead(sLang) ;
  if (NULL == pHeadForLang)
  {
    if (string("") == sLang)
      return string("") ;
    else
      return getHelpUrl(sLang) ;
  }

  return pHeadForLang->getHelpUrl() ;
}

// -----------------------------------------------------------------------------
//
// Classe Cconcern
//
// -----------------------------------------------------------------------------

Cconcern::Cconcern(string attributs, string values, Cbalise *father)
         :Cbalise(attributs, values, father)
{
}

Cconcern::~Cconcern()
{
}

bool
Cconcern::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolCconcern::verif(){
  return true ;
}

bool
Cconcern::compresser()
{
  return true ;
}

bool
Cconcern::traiter()
{
  return true ;
}

string
Cconcern::getCode()
{
  return getStringAttribute(ATTRIBUT_CONCERN_CODE) ;
}

string
Cconcern::getFrame()
{
  return getStringAttribute(ATTRIBUT_CONCERN_FRAME) ;
}

string
Cconcern::getCategory()
{
  return getStringAttribute(ATTRIBUT_CONCERN_CATEGORY) ;
}

bool
Cconcern::getAutoCreate()
{
  bool bDefault = false ;
  return getBooleanAttribute(ATTRIBUT_CONCERN_AUTO, &bDefault) ;
}

int
Cconcern::getSeverity()
{
  return getIntAttribute(ATTRIBUT_CONCERN_SEVE) ;
}

// -----------------------------------------------------------------------------
//
// Classe Chead
//
// -----------------------------------------------------------------------------

Chead::Chead(string attributs, string values, Cbalise *father)
      :Cbalise(attributs, values, father)
{
}

Chead::~Chead()
{
}

bool
Chead::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}boolChead::verif(){
  return true ;
}

bool
Chead::compresser()
{
  return true ;
}

bool
Chead::traiter()
{
  return true ;
}

string
Chead::getTitle()
{
  return getStringAttribute(ATTRIBUT_HEAD_TITLE) ;
}

string
Chead::getHelpUrl()
{
  return getStringAttribute(ATTRIBUT_HEAD_HELP) ;
}

string
Chead::getLang()
{
  return getStringAttribute(ATTRIBUT_HEAD_LANG) ;
}

string
Chead::getGroup()
{
  return getStringAttribute(ATTRIBUT_PROP_GROUPE) ;
}

// -----------------------------------------------------------------------------
//
// Classe Citem
//
// -----------------------------------------------------------------------------

Citem::Citem(string attributs, string values, Cbalise *father, NSSuper* pSuper, bool bRoot)
      :Cbalise (attributs, values, father)
{
  bItemRoot = bRoot ;
  _pSuper   = pSuper ;
}


Citem::~Citem()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Citem *pCItem = dynamic_cast<Citem*>(pBalise) ;
      if (pCItem)
      {
        delete pCItem ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>(pBalise) ;
        if (pCCtr)
        {
          delete pCCtr ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Citem::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (false == bItemRoot)
  {
    if (vect_attr.empty())
    {
      // erreur (pas d'attribut)
      _iParsingError = EITEM_ATTR_EMPTY ;
      return false ;
    }

    int nb_code       = 0 ;
    int nb_decal      = 0 ;
    int nb_control    = 0 ;
    int nb_text       = 0 ;
    int nb_complement = 0 ;
    int nb_archetype  = 0 ;

    for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
    {
      if      ((*iattribut)->getLabel() == ATTRIBUT_ITEM_CODE) // chemin
        nb_code++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_ITEM_DECAL)   // décalageNiveau
        nb_decal++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_ITEM_CONTROL)   // Elt de dialogue
        nb_control++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_ITEM_TEXT)   // texte de l'elt de dialogue
        nb_text++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_ITEM_COMPLEMENT)  // complement
        nb_complement++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_ITEM_ARCHETYPE) // archetype
				nb_archetype++ ;
      else
      {
        // erreur (attribut incorrect)
        _iParsingError = EITEM_ATTR ;
        return false ;
      }
    }

    if (1 != nb_code)
    {
      // erreur (nb attributs incorrect)
      _iParsingError = EITEM_NBATTR ;
      return false ;
    }

    if ((nb_decal > 1) || (nb_control > 1) || (nb_text > 1))
    {
      // erreur (nb attributs incorrect)
      _iParsingError = EITEM_NBATTR ;
      return false ;
    }
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

  if (bItemRoot)
  {
    if (vect_val.empty())
    {
      // erreur (pas de valeur)
      _iParsingError = EITEM_VAL_EMPTY ;
      return false ;
    }
  }

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if (LABEL_ITEM == (*ival)->getLabel()) // item
      {
        (*ival)->setObject(new Citem((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing item
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else if (LABEL_CONTRAINTE == (*ival)->getLabel()) // contrainte de saisie
      {
        (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = EITEM_VAL ;
        return false ;
      }
    }
  }
	return true ;
}

bool
Citem::verif()
{
  return true ;
}

bool
Citem::compresser()
{
  return true ;
}

bool
Citem::traiter()
{
  return true ;
}

string
Citem::getCode()
{
  return getStringAttribute(ATTRIBUT_ITEM_CODE) ;
}

void
Citem::setCode(string sCode)
{
  setStringAttribute(ATTRIBUT_ITEM_CODE, sCode) ;
}

string
Citem::getDecalage()
{
  return getStringAttribute(ATTRIBUT_ITEM_DECAL) ;
}

string
Citem::getControl()
{
  return getStringAttribute(ATTRIBUT_ITEM_CONTROL) ;
}

void
Citem::setControl(string sControl)
{
  setStringAttribute(ATTRIBUT_ITEM_CONTROL, sControl, true) ;
}

string
Citem::getText()
{
  return getStringAttribute(ATTRIBUT_ITEM_TEXT) ;
}

void
Citem::setText(string sText)
{
  setStringAttribute(ATTRIBUT_ITEM_TEXT, sText, true) ;
}

string
Citem::getArchetype()
{
  return getStringAttribute(ATTRIBUT_ITEM_ARCHETYPE) ;
}

void
Citem::setArchetype(string sArchetype)
{
  setStringAttribute(ATTRIBUT_ITEM_ARCHETYPE, sArchetype, true) ;
}

Valeur_array *
Citem::getArrayFils()
{
  return (&vect_val) ;
}

// -----------------------------------------------------------------------------
// added by fab -- aout 2003
// -----------------------------------------------------------------------------
bool
Citem::verifConstraintsWithPPatho(NSPatPathoArray *pPPT, string* psMessage)
{
	bool  bNeeded     = false ;
  bool  bSonExist   = false ;
  int   iNbMinExist = -1 ;
  int   iNbMaxExist = -1 ;

  // recherche des contraintes sur l'item
  Valeur_array *pItemSons = this->getArrayFils() ;
  for (ValIter i = pItemSons->begin() ; pItemSons->end() != i ; i++)
  {
    Ccontrainte *pConstraint = dynamic_cast<Ccontrainte *> ((*i)->getObject()) ;
    if      (pConstraint && (pConstraint->getStringAttribute(ATTRIBUT_CONTR_TYPE) == VAL_ATTR_CONTR_TYPE_NEEDED))
      bNeeded = true ;
    else if (pConstraint && (pConstraint->getStringAttribute(ATTRIBUT_CONTR_TYPE) == VAL_ATTR_CONTR_TYPE_SONSEX))
    {
      bSonExist = true ;
      // on récupère le nombre min
      int iNb = pConstraint->getIntAttribute(ATTRIBUT_CONTR_MIN) ;
      if (iNb > 0)
        iNbMinExist = iNb ;

      // on récupère le nombre max
      iNb = pConstraint->getIntAttribute(ATTRIBUT_CONTR_MAX) ;
      if (iNb > 0)
        iNbMaxExist = iNb ;
    }
  }  // vérification que si une contrainte SonsExist existe qu'il y ai un nb min ou un nb max  if (bSonExist && (iNbMinExist <= 0) && (iNbMaxExist <= 0))	{
		// erreur min et/ou max doivent être présent dans une contrainte de type "sonsexist"
		return false ;
	}

	if (!bSonExist && !bNeeded)
		return true ;

	if (searchInPPathoItemPath(pPPT))
	{
		// l'item est présent dans la PPatho --> on va maintenant vérifier ses fils
		if      (bSonExist)
		{
			// dans ce cas on compte les fils qui existe (ou qui ont leurs contraintes vérifiées)
			int     iNb = 0 ;
			Valeur_array  *pItemSons = getArrayFils() ;
			for (ValIter itemIter = pItemSons->begin() ; itemIter != pItemSons->end() ; itemIter++)
			{
				Citem *pItem = dynamic_cast<Citem *> ((*itemIter)->getObject()) ;
				if (pItem)
				{
					if (pItem->verifConstraintsWithPPatho(pPPT, psMessage))
						iNb++ ;
				}
			}
			if      ((iNbMinExist != -1) && (iNb < iNbMinExist))
				return false ;
			else if ((iNbMaxExist != -1) && (iNb > iNbMaxExist))
				return false ;
			/*
			else    // ((iNbMinExist == -1) && (iNb >= iNbMinExist)) || ((iNbMaxExist == -1) && (iNb >= iNbMaxExist))
				return true ;
			*/
    	}
		else if (bNeeded)
		{
			// dans ce cas, on vérifie que tous les fils ont leur contraintes vérifiés
			Valeur_array  *pItemSons = getArrayFils() ;
			for (ValIter itemIter = pItemSons->begin() ; itemIter != pItemSons->end() ; itemIter++)
			{
				Citem *pItem = dynamic_cast<Citem *> ((*itemIter)->getObject()) ;
				if (pItem)
				{
					if (!(pItem->verifConstraintsWithPPatho(pPPT, psMessage)))
						return false ;
				}
			}
		}
		return true ;
	}
	else
		return false ;
}

bool
Citem::searchInPPathoItemPath(NSPatPathoArray *pPPT)
{
	if ((NULL == pPPT) || (pPPT->empty()))
    return false ;

	for (PatPathoIter pptIter = pPPT->begin() ; pptIter != pPPT->end() ; pptIter++)
	{    char  pcPath[128] ;    if (string("") != (*pptIter)->getComplement())      sprintf(pcPath, "%s/$%s", (*pptIter)->getLexique().c_str(), (*pptIter)->getComplement().c_str()) ;    else      sprintf(pcPath, "%s", (*pptIter)->getLexique().c_str()) ;    if (strcmp(pcPath, getCode().c_str()) == 0)      // le path du Citem est présent dans la PPatho      return true ;	}	return false ;}

Citem*
Citem::getFirstSon()
{
  if (vect_val.empty())
    return (Citem*) 0 ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if (LABEL_ITEM == (*ival)->getLabel())
    {      Citem* pFirstItem = dynamic_cast<Citem*>((*ival)->getObject()) ;      if (pFirstItem)        return pFirstItem ;    }  }

  return (Citem*) 0 ;
}

// -----------------------------------------------------------------------------
//
// Classe Ccontrainte
//
// -----------------------------------------------------------------------------

Ccontrainte::Ccontrainte(string attributs, string values, Cbalise *father, bool bRoot)
  : Cbalise(attributs, values, father)
{
  bContrainteRoot = bRoot ;
}


Ccontrainte::~Ccontrainte()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cvariable *pCVar = dynamic_cast<Cvariable*>(pBalise) ;
      if (pCVar)
      {
        delete pCVar ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>(pBalise) ;
        if (pCCtr)
        {
          delete pCCtr ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Cvalidateur *pCValid = dynamic_cast<Cvalidateur*>(pBalise) ;
          if (pCValid)
          {
            delete pCValid ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}


bool
Ccontrainte::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (false == bContrainteRoot)
  {
    if (vect_attr.empty())
    {
      // erreur (pas d'attribut)
      _iParsingError = ECONTR_ATTR_EMPTY ;
      return false ;
    }

    int nb_type   = 0 ;
    int nb_liste  = 0 ;
    int nb_nom    = 0 ;
    int nb_label  = 0 ;
    int nb_exp    = 0 ;
    int nb_min    = 0 ;
    int nb_max    = 0 ;
    int nb_var    = 0 ;
    for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
    {
      if      ((*iattribut)->getLabel() == ATTRIBUT_CONTR_TYPE) // type de contrainte
        nb_type++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_LISTE)
        nb_liste++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_NOM)
        nb_nom++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_LABEL)
        nb_label++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_EXP)
        nb_exp++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_VAR)
        nb_var++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_MIN)
        nb_min++ ;
      else if ((*iattribut)->getLabel() == ATTRIBUT_CONTR_MAX)
        nb_max++ ;
      else if (((*iattribut)->getLabel() != ATTRIBUT_CONTR_PUBLISH) &&
               ((*iattribut)->getLabel() != ATTRIBUT_CONTR_CLASSIFY))
      {
        // erreur (attribut incorrect)
        _iParsingError = ECONTR_ATTR ;
        return false ;
      }
    }

    if      (nb_type != 1)
    {
      // erreur (nb attributs incorrect)
      _iParsingError = ECONTR_NBATTR ;
      return false ;
    }
    else if ((nb_nom > 1) || (nb_label > 1) || (nb_liste > 1) || (nb_exp > 1) || (nb_min > 1) || (nb_max > 1) || (nb_var > 1))
    {
      // erreur (nb attributs incorrect)
      _iParsingError = ECONTR_NBATTR ;
      return false ;
    }
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

  if (bContrainteRoot)
  {
    if (vect_val.empty())
    {
      // erreur (pas de valeur)
      _iParsingError = ECONTR_VAL_EMPTY ;
      return false ;
    }
  }
  else
  {
    if (false == vect_val.empty())
    {
      _iParsingError = ECONTR_VAL ;
      return false ;
    }
  }

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if      ((*ival)->getLabel() == LABEL_CONTRAINTE) // contrainte
      {
        (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing contrainte
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else if ((*ival)->getLabel() == LABEL_VARIABLE)
      {
        (*ival)->setObject(new Cvariable((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing variable
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else if ((*ival)->getLabel() == LABEL_VALIDATEUR)
      {
        (*ival)->setObject(new Cvalidateur((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing validateur
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = ECONTR_VAL ;
        return false ;
      }
    }
  }
	return true ;
}

bool
Ccontrainte::verif()
{
  return true ;
}

bool
Ccontrainte::compresser()
{
  return true ;
}

bool
Ccontrainte::traiter()
{
  return true ;
}

Valeur_array *
Ccontrainte::getValArray()
{
  return (&vect_val) ;
}

bool
Ccontrainte::mustPublish()
{
	string sPubli = getStringAttribute(ATTRIBUT_CONTR_PUBLISH) ;
	if (sPubli == "1")
		return true ;

	return false ;
}

// -----------------------------------------------------------------------------
//
// Classe variable
//
// -----------------------------------------------------------------------------

Cvariable::Cvariable(string attributs, string values, Cbalise *father)
          :Cbalise(attributs, values, father)
{
}

Cvariable::~Cvariable()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Calias *pCAlias = dynamic_cast<Calias*>(pBalise) ;
      if (pCAlias)
      {
        delete pCAlias ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Cvariable::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EVARIABLE_ATTR_EMPTY ;
    return false ;
  }

  int nb_nom    = 0 ;
  int nb_label  = 0 ;
  int nb_unit   = 0 ;
  int nb_expres = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_VAR_NOM) // nom de la variable
			nb_nom++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_VAR_LABEL)
			nb_label++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_VAR_UNIT)
      nb_unit++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_VAR_EXPRESSION)
      nb_expres++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EVARIABLE_ATTR ;
      return false ;
    }
  }

	if ((nb_nom != 1) || (nb_label > 1) || (nb_unit > 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EVARIABLE_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if ((0 == nb_expres) && vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EVARIABLE_VAL_EMPTY ;
    return false ;
  }

  int nb_alias = 0 ;
  if (false == vect_val.empty())
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if (LABEL_ALIAS == (*ival)->getLabel()) // chemin associé
      {
        (*ival)->setObject(new Calias((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        Calias* pAlias = (Calias*) (*ival)->getObject() ;
        if (pAlias)
          pAlias->setLocal(false) ;

        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing contrainte
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        nb_alias++ ;
      }
      else if (LABEL_LOCAL_ALIAS == (*ival)->getLabel()) // chemin associé
      {
        (*ival)->setObject(new Calias((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        Calias* pAlias = (Calias*) (*ival)->getObject() ;
        if (pAlias)
          pAlias->setLocal(true) ;

        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing contrainte
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        nb_alias++ ;
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = EVARIABLE_VAL ;
        return false ;
      }
    }

	if ((0 == nb_expres) && (nb_alias < 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EVARIABLE_NBVAL ;
    return false ;
  }
	return true ;
}

boolCvariable::verif(){
  return true ;
}

bool
Cvariable::compresser()
{
  return true ;
}

bool
Cvariable::traiter()
{
  return true ;
}

string
Cvariable::getName()
{
  return getStringAttribute(ATTRIBUT_VAR_NOM) ;
}

string
Cvariable::getLabel()
{
  return getStringAttribute(ATTRIBUT_VAR_LABEL) ;
}

string
Cvariable::getExpression()
{
  return getStringAttribute(ATTRIBUT_VAR_EXPRESSION) ;
}

string
Cvariable::getUnit()
{
  return getStringAttribute(ATTRIBUT_VAR_UNIT) ;
}

bool
Cvariable::isEmpty()
{
  return vect_val.empty() ;
}

// -----------------------------------------------------------------------------
//
// Classe alias
//
// -----------------------------------------------------------------------------

Calias::Calias(string attributs, string values, Cbalise *father)
       :Cbalise(attributs, values, father)
{
  _bIsLocal = false ;
}

Calias::~Calias()
{
}

bool
Calias::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (!parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (!parser_valeurs())
		return false ;

	return true ;
}

boolCalias::verif(){
  return true ;
}

bool
Calias::compresser()
{
  return true ;
}

bool
Calias::traiter()
{
  return true ;
}

string
Calias::getValue()
{
  return getValeurs() ;
}

// -----------------------------------------------------------------------------
//
// Classe validateur
//
// -----------------------------------------------------------------------------

Cvalidateur::Cvalidateur(string attributs, string values, Cbalise *father)
            :Cbalise(attributs, values, father)
{
}

Cvalidateur::~Cvalidateur()
{
}

bool
Cvalidateur::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EVALIDATEUR_ATTR_EMPTY ;
    return false ;
  }

  int nb_cond = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if ((*iattribut)->getLabel() == ATTRIBUT_VALIDATEUR_COND) // condition du validateur
			nb_cond++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EVALIDATEUR_ATTR ;
      return false ;
    }
  }

	if (1 != nb_cond)
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EVALIDATEUR_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = EVALIDATEUR_VAL_NOT_EMPTY ;
    return false ;
  }

	return true ;
}

boolCvalidateur::verif(){
  return true ;
}

bool
Cvalidateur::compresser()
{
  return true ;
}

bool
Cvalidateur::traiter()
{
    return true ;
}

string
Cvalidateur::getValidationString()
{
  return getStringAttribute(ATTRIBUT_VALIDATEUR_COND) ;
}

// -----------------------------------------------------------------------------
//
// Creferentiel
//
// -----------------------------------------------------------------------------

Creferentiel::Creferentiel(string attributs, string values, Cbalise *father, NSSuper* pSuper)
             :Cbalise(attributs, values, father)
{
  _pSuper      = pSuper ;
  _pGlobalVars = NULL ;
}

Creferentiel::~Creferentiel()
{
  // change 7 july 2008: the object is now pointed by a pObject with
  // _pGlobalVars as an alias... so better don't use delete on it!
  //
  // if (_pGlobalVars != NULL)
  //  delete _pGlobalVars ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cdialogue *pCdlg = dynamic_cast<Cdialogue*>(pBalise) ;
      if (pCdlg)
      {
        delete pCdlg ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Creferences *pCRef = dynamic_cast<Creferences*>(pBalise) ;
        if (pCRef)
        {
          delete pCRef ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Cproposition *pCProp = dynamic_cast<Cproposition*>(pBalise) ;
          if (pCProp)
          {
            delete pCProp ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            Cglobalvars *pGvar = dynamic_cast<Cglobalvars*>(pBalise) ;
            if (pGvar)
            {
              delete pGvar ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Creferentiel::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_REFER_NAME) == "")
  {
    // erreur (pas de nom)
    _iParsingError = EREFERENTIEL_NONAME ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EREFERENTIEL_VAL_EMPTY ;
    return false ;
  }

  int nb_dialogue = 0 ;
  int nb_prop     = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_DIALOGUE == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cdialogue((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_dialogue++ ;
		}
    else if (LABEL_REFERENCES == (*ival)->getLabel()) // references
    {
      (*ival)->setObject(new Creferences((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
    }
    else if (LABEL_PROPOSITION == (*ival)->getLabel()) // proposition
    {
      (*ival)->setObject(new Cproposition((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing props
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_prop++ ;
    }
    else if (LABEL_GLOBALVARS == (*ival)->getLabel()) // variables globales
    {
      (*ival)->setObject(new Cglobalvars((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;

      _pGlobalVars = (Cglobalvars*) (*ival)->getObject() ;
      if (_pGlobalVars && (false == _pGlobalVars->parser()))
      {
        // erreur parsing globalvars
        _iParsingError = _pGlobalVars->getParsingError() ;
        return false ;
      }
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EREFERENTIEL_VAL ;
      return false ;
    }
  }

	if ((nb_dialogue > 1) || (nb_prop < 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EREFERENTIEL_NBVAL ;
    return false ;
  }

	return true ;
}boolCreferentiel::verif(){
  return true ;
}

bool
Creferentiel::compresser()
{
  return true ;
}

bool
Creferentiel::traiter()
{
  return true ;
}

Valeur_array *
Creferentiel::getValArray()
{
  return (&vect_val) ;
}

Cglobalvars *
Creferentiel::getGVars()
{
  return (_pGlobalVars) ;
}

int
Creferentiel::getNbPropGroup(NSContexte *pCtx, string sGroup, bool bWithValidation, string sLang)
{
  if (vect_val.empty())
    return 0 ;

	VecteurString aExcluded ;
	VecteurString aNeeded ;

  int       nb_prop = 0 ;
  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if (LABEL_PROPOSITION == (*ival)->getLabel()) // proposition
    {
      Cproposition *pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;
      if ((pCprop) && (pCprop->getGroup(sLang) == sGroup))
      {
        if (bWithValidation)
        {
        	bool bValid = true ;

        	// Does this proposition belong to the excluded list
          //
        	string sPropID = pCprop->getStringAttribute(ATTRIBUT_PROP_ID) ;
					if ((string("") != sPropID) && (aExcluded.ItemDansUnVecteur(sPropID)))
          	bValid = false ;

          // Else, if this proposition doesn't belong to the needed list,
          // we have to check its validity          //          else if ((string("") == sPropID) || (false == aNeeded.ItemDansUnVecteur(sPropID)))
          	for (ValIter i = pCprop->getValArray()->begin() ; bValid && (pCprop->getValArray()->end() != i) ; i++)
          	{            	if (LABEL_VALIDITE == (*i)->getLabel())              {              	Ccontrainte *pCvalidite = dynamic_cast<Ccontrainte *>((*i)->getObject()) ;                if (pCvalidite)                {              	  NSValidateur valid(pCvalidite, pCtx) ;              	  if (false == valid.Validation())                  {                	  bValid = false ;                    break ;                  }                }              }            }          if (bValid)          {          	nb_prop++ ;            // Take into account the excluded            //            pCprop->addExcludedToVector(&aExcluded) ;            pCprop->addNeededToVector(&aNeeded) ;          }
        }
        else
          nb_prop++ ;
      }
    }
  }
  return nb_prop ;
}

bool
Creferentiel::getNextGroup(string& sGroup, string sLang)
{
  bool trouve = false ;
  sGroup = string("") ;

  if (vect_val.empty())
    return trouve ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if (LABEL_PROPOSITION == (*ival)->getLabel()) // proposition
    {
      Cproposition *pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;
      if ((pCprop) && (false == pCprop->getTraite()))
      {
        sGroup = pCprop->getGroup(sLang) ;
        trouve = true ;
        break ;
      }
    }
  }

  return trouve ;
}

Creferences *
Creferentiel::getReference()
{
  Creferences *pRef = (Creferences *) 0 ;

  if (vect_val.empty())
    return pRef ;

  for (ValIter ival = vect_val.begin() ;  vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_REFERENCES == (*ival)->getLabel())
    {
      pRef = dynamic_cast<Creferences *>((*ival)->getObject()) ;
      break ;
    }
  }

  return pRef ;
}

string
Creferentiel::getTitle(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getTitle() ;
  }
	// Obsolete
  //
  return getStringAttribute(ATTRIBUT_REFER_TITLE) ;
}

string
Creferentiel::getHelpUrl(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getHelpUrl() ;
  }
  return string("") ;
}

string
Creferentiel::getName()
{
  return getStringAttribute(ATTRIBUT_REFER_NAME) ;
}

string
Creferentiel::getEvolutionOf()
{
  return getStringAttribute(ATTRIBUT_REFER_EVOLUTIONOF) ;
}

// -----------------------------------------------------------------------------
//
// Classe Cproposition
//
// -----------------------------------------------------------------------------

Cproposition::Cproposition(string attributs, string values, Cbalise *father, NSSuper* pSuper)
             :Cbalise(attributs, values, father)
{
  _pSuper          = pSuper ;
  traite           = false ;
  check            = false ;
  bInitialyChecked = false ;
  pReasonTree      = 0 ;
}


Cproposition::~Cproposition()
{
  if (pReasonTree)
    delete pReasonTree ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Creferences *pCRef = dynamic_cast<Creferences*>(pBalise) ;
      if (pCRef)
      {
        delete pCRef ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Ccontrainte *pCCont = dynamic_cast<Ccontrainte*>(pBalise) ;
        if (pCCont)
        {
          delete pCCont ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          CFriendsGroup *pCGrp = dynamic_cast<CFriendsGroup*>(pBalise) ;
          if (pCGrp)
          {
            delete pCGrp ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            Ctree *pCtree = dynamic_cast<Ctree*>((*ival)->getObject()) ;
            if (pCtree)
            {
              delete pCtree ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Cproposition::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EPROP_ATTR_EMPTY ;
    return false ;
  }

  int nb_nom      = 0 ;
  int nb_group    = 0 ;
  int nb_help     = 0 ;
  int nb_id       = 0 ;
  int nb_evidence = 0 ;
  int nb_autochk  = 0 ;
  int nb_replace  = 0 ;
  int nb_sameas   = 0 ;
  int nb_unck_Arc = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_PROP_NOM) // nom du dialogue
			nb_nom++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_GROUPE)
      nb_group++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_HELP)
      nb_help++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_ID)
      nb_id++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_EVIDENCE_LVL)
      nb_evidence++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_AUTOCHECK)
      nb_autochk++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_REPLACE)
      nb_replace++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_SAMEAS)
      nb_sameas++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_PROP_UNCHECK_ARCH)
      nb_unck_Arc++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EPROP_ATTR ;
      return false ;
    }
  }

	if (/*(nb_nom != 1) || (nb_group > 1) ||*/ (nb_id > 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EPROP_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // iParsingError = EPROP_VAL_EMPTY;
    // return false; // erreur (pas de valeur)
    return true ;
  }

  int nb_valid  = 0 ;
  int nb_tree   = 0 ;
	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if ((*ival)->getLabel() == LABEL_REFERENCES) // validite
		{
    	(*ival)->setObject(new Creferences((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else if ((*ival)->getLabel() == LABEL_VALIDITE) // validite
		{
      (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this, true)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_valid++ ;
		}
		else if ((*ival)->getLabel() == LABEL_FRIENDSGROUP) // friends
		{
      (*ival)->setObject(new CFriendsGroup((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_TREE) // items
    {
      (*ival)->setObject(new Ctree((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      Ctree *pCtree = dynamic_cast<Ctree *>((*ival)->getObject()) ;
      if (pCtree)
      {
        pCtree->traiteCodeLoc() ;
        pCtree->chargePatPatho() ;
			  nb_tree++ ;
      }
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EPROP_VAL ;
      return false ;
    }
  }

	if (nb_valid > 1)
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EPROP_NBVAL ;
    return false ;
  }
	return true ;
}boolCproposition::verif(){
  return true ;
}


bool
Cproposition::compresser()
{
  return true ;
}

bool
Cproposition::traiter()
{
  return true ;
}


Valeur_array *
Cproposition::getValArray()
{
  return (&vect_val) ;
}


Ccontrainte *
Cproposition::getValidity()
{
  Ccontrainte *pValidity = (Ccontrainte*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_VALIDITE) // validite
      {
        pValidity = dynamic_cast<Ccontrainte *>((*ival)->getObject()) ;
        break ;
      }
    }
  }

  return pValidity ;
}

Creferences *
Cproposition::getReference()
{
  Creferences *pReference = (Creferences*) 0 ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_REFERENCES) // references
      {
        pReference = dynamic_cast<Creferences *>((*ival)->getObject()) ;
        break ;
      }
    }
  }

  return pReference ;
}

bool
Cproposition::getTraite()
{
  return traite ;
}

void
Cproposition::setTraite(bool bTraite)
{
  traite = bTraite ;
}

bool
Cproposition::getCheck()
{
  return check ;
}

void
Cproposition::setCheck(bool bCheck)
{
  check = bCheck ;
}

string
Cproposition::getPropID()
{
	string sLocalID = getStringAttribute(ATTRIBUT_PROP_ID) ;
	if (sLocalID == "")
		return "" ;

	Creferentiel* pRef = dynamic_cast<Creferentiel *>(parent) ;
	if (!pRef)
		return "" ;

	string sRefName = pRef->getName() ;

	if (sRefName == "")
		return "" ;

	return sRefName + string(1, '#') + sLocalID ;
}

string
Cproposition::getReplacedPropID()
{
	return getStringAttribute(ATTRIBUT_PROP_REPLACE) ;
}

string
Cproposition::getSameAsPropID()
{
	return getStringAttribute(ATTRIBUT_PROP_SAMEAS) ;
}

string
Cproposition::getUncheckArchetype()
{
	return getStringAttribute(ATTRIBUT_PROP_UNCHECK_ARCH) ;
}

Cproposition::EVIDENCE_LEVEL
Cproposition::getEvidenceLevel()
{
  string sAttributeValue = getStringAttribute(ATTRIBUT_PROP_EVIDENCE_LVL) ;
  pseumaj(&sAttributeValue) ;

  if (string("A") == sAttributeValue)
    return levelA ;
  if (string("B") == sAttributeValue)
    return levelB ;
  if (string("C") == sAttributeValue)
    return levelC ;
  if ((string("PRO") == sAttributeValue) || (string("P") == sAttributeValue))
    return levelPro ;

  return levelUnknown ;
}

bool
Cproposition::getIsAutocheck()
{
  // Autocheck is true by default
  //
  bool bDefault = true ;
  return getBooleanAttribute(ATTRIBUT_PROP_AUTOCHECK, &bDefault) ;
}

string
Cproposition::getTitle(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getTitle() ;
  }
	// Deprecated
  //
  return getStringAttribute(ATTRIBUT_PROP_NOM) ;
}

string
Cproposition::getGroup(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getGroup() ;
  }
	// Deprecated
  //
  return getStringAttribute(ATTRIBUT_PROP_GROUPE) ;
}

string
Cproposition::getHelpUrl(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getHelpUrl() ;
  }
	// Deprecated
  //
  return getStringAttribute(ATTRIBUT_PROP_HELP) ;
}

string
Cproposition::getRefIDFromPropID()
{
	string sPropId = getPropID() ;
  if (sPropId == "")
		return string("") ;

	size_t separ = sPropId.find("#") ;
  if (separ == string::npos)
    return string("") ;

	return string(sPropId, 0, separ) ;
}

string
Cproposition::getLocalPropIDFromPropID()
{
	string sPropId = getPropID() ;
  if (sPropId == "")
		return string("") ;

	size_t separ = sPropId.find("#") ;
  if ((separ == string::npos) || (separ == strlen(sPropId.c_str()) - 1))
    return string("") ;

	return string(sPropId, separ + 1, strlen(sPropId.c_str()) - separ - 1) ;
}

void
Cproposition::addExcludedToVector(VecteurString* pVector)
{
	if ((NULL == pVector) || (vect_val.empty()))
		return ;

	CFriendsGroup* pFriends = (CFriendsGroup*) 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
	{
		if ((*ival)->getLabel() == LABEL_FRIENDSGROUP) // validite
		{
			pFriends = dynamic_cast<CFriendsGroup*>((*ival)->getObject()) ;
			break ;
		}
	}

	if (NULL == pFriends)
		return ;

	CExcludedFriends* pExclFriends = pFriends->getExcludedFriends() ;
	if (NULL == pExclFriends)
		return ;

	Cfriend* pFriend = pExclFriends->getNextExcludedFriend(NULL) ;
	while (pFriend)
	{
		string sID = pFriend->getId() ;
		if ((string("") != sID) && (false == pVector->ItemDansUnVecteur(sID)))
			pVector->AddString(sID) ;

		pFriend = pExclFriends->getNextExcludedFriend(pFriend) ;
	}
}

void
Cproposition::addNeededToVector(VecteurString* pVector)
{
	if ((NULL == pVector) || (vect_val.empty()))
		return ;

	CFriendsGroup* pFriends = (CFriendsGroup*) 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
	{
		if ((*ival)->getLabel() == LABEL_FRIENDSGROUP) // validite
		{
			pFriends = dynamic_cast<CFriendsGroup*>((*ival)->getObject()) ;
			break ;
		}
	}

	if (NULL == pFriends)
		return ;

	CNeededFriends* pNeededFriends = pFriends->getNeededFriends() ;
	if (NULL == pNeededFriends)
		return ;

	Cfriend* pFriend = pNeededFriends->getNextNeededFriend(NULL) ;
	while (pFriend)
	{
		string sID = pFriend->getId() ;
		if ((string("") != sID) && (false == pVector->ItemDansUnVecteur(sID)))
			pVector->AddString(sID) ;

		pFriend = pNeededFriends->getNextNeededFriend(pFriend) ;
	}
}


// -----------------------------------------------------------------------------
//
// Classe Ctree
//
// -----------------------------------------------------------------------------

Ctree::Ctree(string attributs, string values, Cbalise *father, NSSuper* pSuper)
      :Cbalise(attributs, values, father)
{
  _pSuper        = pSuper ;
  pPatPathoArray = new NSPatPathoArray(_pSuper) ;
}

Ctree::~Ctree()
{
  if (pPatPathoArray)
    delete pPatPathoArray ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cnode *pCNode = dynamic_cast<Cnode*>(pBalise) ;
      if (pCNode)
      {
        delete pCNode ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Ctree::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }

  int nb_loc = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if ((*iattribut)->getLabel() == ATTRIBUT_TREE_LOC) // localisation
			nb_loc++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ETREE_ATTR ;
      return false ;
    }
  }

	if (nb_loc != 1)
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ETREE_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

  int nb_node = 0 ;
	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      (*ival)->setObject(new Cnode((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_node++ ;
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = ETREE_VAL ;
      return false ;
    }
  }

	if (nb_node < 1)
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = ETREE_NBVAL ;
    return false ;
  }
	return true ;
}

boolCtree::verif(){
  return true ;
}

bool
Ctree::chargePatPatho()
{
	if (vect_val.empty())
    return false ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      Cnode *pCnode = dynamic_cast<Cnode *>((*ival)->getObject()) ;
      if (pCnode)
      	pCnode->chargePatPatho(pPatPathoArray) ;
		}
  }
  return true ;
}

bool
Ctree::initFromPatPatho()
{
	if (NULL == pPatPathoArray)
  	return false ;

	vect_val.vider() ;

	if (true == pPatPathoArray->empty())
  	return true ;

	PatPathoIter it = pPatPathoArray->begin() ;
  while ((NULL != it) && (pPatPathoArray->end() != it))
	{
  	// Create CValeur and Cnode
    //
  	CValeur* pValeur = new CValeur(LABEL_NODE, string(""), string(""), this) ;
		vect_val.push_back(pValeur) ;
    pValeur->setObject(new Cnode(string(""), string(""), this)) ;

    // Init Cnode
    //
    Cnode *pCnode = dynamic_cast<Cnode*>(pValeur->getObject()) ;
    if (pCnode)
    {
      *(pCnode->pPatPathoInfo) = **it ;

      // if (string("") != pCnode->pPatPathoInfo->getLocalisation())
      //	pCnode->addAttribute(ATTRIBUT_NODE_LOC,        pCnode->pPatPathoInfo->getLocalisation()) ;
      if (string("") != pCnode->pPatPathoInfo->getType())
    	  pCnode->addAttribute(ATTRIBUT_NODE_TYPE,       pCnode->pPatPathoInfo->getType()) ;
      if (string("") != pCnode->pPatPathoInfo->getLexique())
    	  pCnode->addAttribute(ATTRIBUT_NODE_LEXIQUE,    pCnode->pPatPathoInfo->getLexique()) ;
      if (string("") != pCnode->pPatPathoInfo->getUnit())
    	  pCnode->addAttribute(ATTRIBUT_NODE_UNIT,       pCnode->pPatPathoInfo->getUnit()) ;
      if (string("") != pCnode->pPatPathoInfo->getComplement())
    	  pCnode->addAttribute(ATTRIBUT_NODE_COMPLEMENT, pCnode->pPatPathoInfo->getComplement()) ;
      if (string("") != pCnode->pPatPathoInfo->getTexteLibre())
    	  pCnode->addAttribute(ATTRIBUT_NODE_FREETEXT,   pCnode->pPatPathoInfo->getTexteLibre()) ;
      if (string("") != pCnode->pPatPathoInfo->getCertitude())
    	  pCnode->addAttribute(ATTRIBUT_NODE_CERTITUDE,  pCnode->pPatPathoInfo->getCertitude()) ;
      if (string("") != pCnode->pPatPathoInfo->getInteret())
    	  pCnode->addAttribute(ATTRIBUT_NODE_INTERET,    pCnode->pPatPathoInfo->getInteret()) ;
      if (string("") != pCnode->pPatPathoInfo->getPluriel())
    	  pCnode->addAttribute(ATTRIBUT_NODE_PLURIEL,    pCnode->pPatPathoInfo->getPluriel()) ;

      NSPatPathoArray subPpt(pPatPathoArray->_pSuper) ;
      pPatPathoArray->ExtrairePatPatho(it, &subPpt) ;
      pCnode->initFromPatPatho(&subPpt) ;
    }

    // Next root node ?
    //
    it = pPatPathoArray->ChercherFrereSuivant(it) ;
	}

	return true ;
}

bool
Ctree::compresser()
{
  return true ;
}

bool
Ctree::traiter()
{
  return true ;
}

bool
Ctree::traiteCodeLoc()
{
  int iLigne    = 0 ;
  int iColonne  = 0 ;

  for (ValIter iterVal = vect_val.begin() ; iterVal != vect_val.end() ; iterVal++)
  {
    iColonne = 0 ;
    Cnode *pCnode = dynamic_cast<Cnode *>((*iterVal)->getObject()) ;
    if (pCnode)
      pCnode->traiteCodeLoc(&iLigne, iColonne) ;
  }
  return true ;
}

string
Ctree::getLocalisation()
{
	return getStringAttribute(ATTRIBUT_TREE_LOC) ;
}

// -----------------------------------------------------------------------------
//
// Classe Cnode
//
// -----------------------------------------------------------------------------

Cnode::Cnode(string attributs, string values, Cbalise *father)
      :Cbalise(attributs, values, father)
{
  pPatPathoInfo = new NSPatPathoInfo ;
}

Cnode::~Cnode()
{
  if (pPatPathoInfo)
    delete pPatPathoInfo ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cnode *pCNode = dynamic_cast<Cnode*>(pBalise) ;
      if (pCNode)
      {
        delete pCNode ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Cnode::parser()
{
  //string    sTexteLibre ;

  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ENODE_ATTR_EMPTY ;
    return false ;
  }

  int nb_loc      = 0 ;
  int nb_type     = 0 ;
  int nb_lex      = 0 ;
  int nb_compl    = 0 ;
  int nb_cert     = 0 ;
  int nb_interet  = 0 ;
  int nb_pluriel  = 0 ;
  int nb_freetext = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_NODE_LOC)
		{
      pPatPathoInfo->setLocalisation((*iattribut)->getValue()) ;
			nb_loc++ ;
		}
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_TYPE)
		{
      pPatPathoInfo->setType((*iattribut)->getValue()) ;
			nb_type++ ;
		}
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_LEXIQUE)
    {
      pPatPathoInfo->setLexique((*iattribut)->getValue()) ;
      nb_lex++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_UNIT)
      pPatPathoInfo->setUnit((*iattribut)->getValue()) ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_COMPLEMENT)
    {
      // Attention l'attribut lexique doit etre défini avant complement
      if (nb_lex)
      {
/*
        // fabDOIT
        // maintenant on a une balise freetext pour les textes libres
        // cette rustine n'a plus lieu d'exister
        if (pPatPathoInfo->getLexique() == "£?????")
        {
          sTexteLibre = (*iattribut)->sValue ;

          if (sTexteLibre == "")            sTexteLibre = "[texte non récupéré]" ;
          else
            pPatPathoInfo->pDonnees->setTexteLibre(sTexteLibre) ;
        }
        else
*/
        pPatPathoInfo->setComplement((*iattribut)->getValue()) ;

        nb_compl++ ;
      }
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_FREETEXT)
    {
      pPatPathoInfo->setTexteLibre((*iattribut)->getValue()) ;
      nb_freetext++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_CERTITUDE)
    {
      pPatPathoInfo->setCertitude((*iattribut)->getValue()) ;
      nb_cert++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_INTERET)
    {
      pPatPathoInfo->setInteret((*iattribut)->getValue()) ;
      nb_interet++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_PLURIEL)
    {
      pPatPathoInfo->setPluriel((*iattribut)->getValue()) ;
      nb_pluriel++ ;
    }
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ENODE_ATTR ;
      return false ;
    }
  }

	if ((nb_loc > 1) || (nb_type > 1) || (nb_lex != 1) || (nb_compl > 1) || (nb_cert > 1) || (nb_interet > 1) || (nb_pluriel > 1) || (nb_freetext > 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ENODE_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_NODE) // fils
      {
        (*ival)->setObject(new Cnode((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing node
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = ENODE_VAL ;
        return false ;
      }
    }
  }
	return true ;
}

boolCnode::verif(){
  return true ;
}

bool
Cnode::compresser()
{
  return true ;
}

bool
Cnode::traiter()
{
  return true ;
}

bool
Cnode::chargePatPatho(NSPatPathoArray *pPPTArray)
{
  if (!pPPTArray)
    return false ;

  // on charge la patpathoInfo du node en cours
  pPPTArray->push_back(new NSPatPathoInfo(*pPatPathoInfo)) ;

  // on relance sur les node fils
  if (!vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
    {
      // apres parsing on est surs qu'il n'y a que des node
      Cnode *pCnode = dynamic_cast<Cnode *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPPTArray) ;
    }
  }
  return false ;
}

bool
Cnode::initFromPatPatho(NSPatPathoArray *pPPTArray)
{
	if (NULL == pPPTArray)
  	return false ;

	vect_val.vider() ;

	if (true == pPPTArray->empty())
  	return true ;

	PatPathoIter it = pPPTArray->begin() ;
  while ((NULL != it) && (pPPTArray->end() != it))
	{
  	// Create CValeur and Cnode
    //
  	CValeur* pValeur = new CValeur(LABEL_NODE, string(""), string(""), this) ;
		vect_val.push_back(pValeur) ;
    pValeur->setObject(new Cnode(string(""), string(""), this)) ;

    // Init Cnode
    //
    Cnode *pCnode = dynamic_cast<Cnode*>(pValeur->getObject()) ;
    if (pCnode)
    {
      *(pCnode->pPatPathoInfo) = **it ;

      // if (string("") != pCnode->pPatPathoInfo->getLocalisation())
      //	pCnode->addAttribute(ATTRIBUT_NODE_LOC,        pCnode->pPatPathoInfo->getLocalisation()) ;
      if (string("") != pCnode->pPatPathoInfo->getType())
    	  pCnode->addAttribute(ATTRIBUT_NODE_TYPE,       pCnode->pPatPathoInfo->getType()) ;
      if (string("") != pCnode->pPatPathoInfo->getLexique())
    	  pCnode->addAttribute(ATTRIBUT_NODE_LEXIQUE,    pCnode->pPatPathoInfo->getLexique()) ;
      if (string("") != pCnode->pPatPathoInfo->getUnit())
    	  pCnode->addAttribute(ATTRIBUT_NODE_UNIT,       pCnode->pPatPathoInfo->getUnit()) ;
      if (string("") != pCnode->pPatPathoInfo->getComplement())
    	  pCnode->addAttribute(ATTRIBUT_NODE_COMPLEMENT, pCnode->pPatPathoInfo->getComplement()) ;
      if (string("") != pCnode->pPatPathoInfo->getTexteLibre())
    	  pCnode->addAttribute(ATTRIBUT_NODE_FREETEXT,   pCnode->pPatPathoInfo->getTexteLibre()) ;
      if (string("") != pCnode->pPatPathoInfo->getCertitude())
    	  pCnode->addAttribute(ATTRIBUT_NODE_CERTITUDE,  pCnode->pPatPathoInfo->getCertitude()) ;
      if (string("") != pCnode->pPatPathoInfo->getInteret())
    	  pCnode->addAttribute(ATTRIBUT_NODE_INTERET,    pCnode->pPatPathoInfo->getInteret()) ;
      if (string("") != pCnode->pPatPathoInfo->getPluriel())
    	  pCnode->addAttribute(ATTRIBUT_NODE_PLURIEL,    pCnode->pPatPathoInfo->getPluriel()) ;

      NSPatPathoArray subPpt(pPPTArray->_pSuper) ;
      pPPTArray->ExtrairePatPatho(it, &subPpt) ;
      pCnode->initFromPatPatho(&subPpt) ;
    }

    // Next root node ?
    //
    it = pPPTArray->ChercherFrereSuivant(it) ;
	}

	return true ;
}

bool
Cnode::traiteCodeLoc(int *iLigne, int iColonne)
{
  pPatPathoInfo->setLigne((*iLigne)++) ;
  pPatPathoInfo->setColonne(iColonne) ;

  for (ValIter iterVal = vect_val.begin() ; iterVal != vect_val.end() ; iterVal++)
  {
    Cnode *pCnode = dynamic_cast<Cnode *>((*iterVal)->getObject()) ;
    if (pCnode)
      pCnode->traiteCodeLoc(iLigne, iColonne + 1) ;
  }
  return true ;
}


// -----------------------------------------------------------------------------
//
// Classe CFriendsGroup
//
// -----------------------------------------------------------------------------

CFriendsGroup::CFriendsGroup(string attributs, string values, Cbalise *father, NSSuper* pSuper)
              :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}


CFriendsGroup::~CFriendsGroup()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CExcludedFriends *pCExcl = dynamic_cast<CExcludedFriends*>(pBalise) ;
      if (pCExcl)
      {
        delete pCExcl ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CNeededFriends *pCNeed = dynamic_cast<CNeededFriends*>(pBalise) ;
        if (pCNeed)
        {
          delete pCNeed ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}


bool
CFriendsGroup::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (!parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (!parser_valeurs())
		return false ;

	//
	// Pas grave si vide ; Can be empty
	//
	if (vect_val.empty())
  	return true ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EXCLUDE) // noeud
		{
      (*ival)->setObject(new CExcludedFriends((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else if ((*ival)->getLabel() == LABEL_NEED) // noeud
		{
      (*ival)->setObject(new CNeededFriends((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = ETREE_VAL ;
      return false ;
    }
  }

	return true ;
}

boolCFriendsGroup::verif(){
  return true ;
}

bool
CFriendsGroup::compresser()
{
  return true ;
}

bool
CFriendsGroup::traiter()
{
  return true ;
}

CExcludedFriends*
CFriendsGroup::getExcludedFriends()
{
  for (ValIter iterVal = vect_val.begin() ; vect_val.end() != iterVal ; iterVal++)
  {
		if ((*iterVal)->getLabel() == LABEL_EXCLUDE) // noeud
		{
      CExcludedFriends *pExcluded = dynamic_cast<CExcludedFriends *>((*iterVal)->getObject()) ;
      if (pExcluded)
      	return pExcluded ;
		}
  }
  return (CExcludedFriends*) 0 ;
}

CNeededFriends*
CFriendsGroup::getNeededFriends()
{
  for (ValIter iterVal = vect_val.begin() ; vect_val.end() != iterVal ; iterVal++)
  {
		if ((*iterVal)->getLabel() == LABEL_NEED) // noeud
		{
      CNeededFriends *pNeeded = dynamic_cast<CNeededFriends *>((*iterVal)->getObject()) ;
      if (pNeeded)
      	return pNeeded ;
		}
  }
  return (CNeededFriends*) 0 ;
}

// -----------------------------------------------------------------------------
//
// Classe CExcludedFriends
//
// -----------------------------------------------------------------------------

CExcludedFriends::CExcludedFriends(string attributs, string values, Cbalise *father, NSSuper* pSuper)
                 :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}

CExcludedFriends::~CExcludedFriends()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cfriend *pCFriend = dynamic_cast<Cfriend*>(pBalise) ;
      if (pCFriend)
      {
        delete pCFriend ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
CExcludedFriends::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	//
	// Pas grave si vide ; Can be empty
	//
	if (vect_val.empty())
  	return true ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_FRIEND) // noeud
		{
      (*ival)->setObject(new Cfriend((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = ETREE_VAL ;
      return false ;
    }
  }

	return true ;
}

boolCExcludedFriends::verif(){
  return true ;
}

bool
CExcludedFriends::compresser()
{
  return true ;
}

bool
CExcludedFriends::traiter()
{
  return true ;
}

Cfriend*
CExcludedFriends::getNextExcludedFriend(Cfriend *pPrevious)
{
  Cfriend  *pRef = (Cfriend *) 0 ;
  Cfriend  *pBuf ;

  bool     bReturnNext = false ;
  if (NULL == pPrevious)
  	bReturnNext = true ;

  if (vect_val.empty())
    return pRef ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_FRIEND == (*ival)->getLabel())
    {
      pBuf = dynamic_cast<Cfriend *>((*ival)->getObject()) ;
      if (pBuf)
      {
        if (bReturnNext)
        {
          pRef = pBuf ;
          break ;
        }
        if (pBuf == pPrevious)
          bReturnNext = true ;
      }
    }
  }

  return pRef ;
}

// -----------------------------------------------------------------------------
//
// Classe CNeededFriends
//
// -----------------------------------------------------------------------------

CNeededFriends::CNeededFriends(string attributs, string values, Cbalise *father, NSSuper* pSuper)
               :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}

CNeededFriends::~CNeededFriends()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Cfriend *pCFriend = dynamic_cast<Cfriend*>(pBalise) ;
      if (pCFriend)
      {
        delete pCFriend ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
CNeededFriends::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	//
	// Pas grave si vide ; Can be empty
	//
	if (vect_val.empty())
  	return true ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_FRIEND) // noeud
		{
      (*ival)->setObject(new Cfriend((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = ETREE_VAL ;
      return false ;
    }
  }

	return true ;
}

boolCNeededFriends::verif(){
  return true ;
}

bool
CNeededFriends::compresser()
{
  return true ;
}

bool
CNeededFriends::traiter()
{
  return true ;
}

Cfriend*
CNeededFriends::getNextNeededFriend(Cfriend *pPrevious)
{
  Cfriend  *pRef = (Cfriend *) 0 ;
  Cfriend  *pBuf ;

  bool bReturnNext = false ;
  if (NULL == pPrevious)
  	bReturnNext = true ;

  if (vect_val.empty())
    return pRef ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_FRIEND == (*ival)->getLabel())
    {
      pBuf = dynamic_cast<Cfriend *>((*ival)->getObject()) ;
      if (pBuf)
      {
        if (bReturnNext)
        {
          pRef = pBuf ;
          break ;
        }
        if (pBuf == pPrevious)
          bReturnNext = true ;
      }
    }
  }

  return pRef ;
}

// -----------------------------------------------------------------------------
//
// Classe Cfriend
//
// -----------------------------------------------------------------------------

Cfriend::Cfriend(string attributs, string values, Cbalise *father, NSSuper* pSuper)
        :Cbalise(attributs, values, father)
{
  _pSuper = pSuper ;
}

Cfriend::~Cfriend()
{
}

bool
Cfriend::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }

  int nb_id = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if ((*iattribut)->getLabel() == ATTRIBUT_FRIEND_ID) // localisation
			nb_id++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ETREE_ATTR ;
      return false ;
    }
  }

	if (nb_id != 1)
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ETREE_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	return true ;
}

boolCfriend::verif(){
  return true ;
}

bool
Cfriend::compresser()
{
  return true ;
}

bool
Cfriend::traiter()
{
  return true ;
}

string
Cfriend::getId()
{
  return getStringAttribute(ATTRIBUT_FRIEND_ID) ;
}

// -----------------------------------------------------------------------------
// ajout FLP - juin 2003
// -----------------------------------------------------------------------------
// balises "dialogbox" et "rcdata" dans archetypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// XressourceAttributes
// -----------------------------------------------------------------------------
// Represents Att values for a Window
// -----------------------------------------------------------------------------

XressourceAttributes::XressourceAttributes()
{
  _caption = string("") ;
  _sLang   = string("") ;
  _type    = string("") ;
  _style   = string("") ;
  _iStyle  = 0 ;
  _coords  = string("") ;
  _x       = 0 ;
  _y       = 0 ;
  _w       = 0 ;
  _h       = 0 ;
}

XressourceAttributes::XressourceAttributes(const XressourceAttributes& rv)
{
  _caption = rv._caption ;
  _sLang   = rv._sLang ;
  _type    = rv._type ;
  _style   = rv._style ;
  _iStyle  = rv._iStyle ;
  _coords  = rv._coords ;
  _x       = rv._x ;
  _y       = rv._y ;
  _w       = rv._w ;
  _h       = rv._h ;
}

void
XressourceAttributes::setIStyleFromSStyle(string *pErrors)
{
  _iStyle = 0 ;

	if (string("") == _style)
		return ;

  size_t  i        = 0 ;
  size_t  iStylLen = strlen(_style.c_str()) ;

  // skipping left blanks
  while ((i < iStylLen) && ((' ' == _style[i]) || ('|' == _style[i])))
    i++ ;

  while (i < iStylLen)
  {
    string attr = string("") ;
    bool   bNot = false ;

    bool bCherche = true ;
    while (bCherche)
		{
    	while ((i < iStylLen) && (' ' != _style[i]) && ('|' != _style[i]))
      	attr += _style[i++] ;

      if ((i >= iStylLen) || ('|' == _style[i]))
      	bCherche = false ;
      else
      	while ((i < iStylLen) && (' ' == _style[i]))
    			i++ ;

      pseumaj(&attr) ;
      if (string("NOT") == attr)
      {
      	bNot = true ;
        attr = "" ;
      }
		}
    while ((i < iStylLen) && ((' ' == _style[i]) || ('|' == _style[i])))
    	i++ ;

    uint32 iAttribute = getStyleIntFromString(attr) ;
    if (0 != iAttribute)
      setIStyleAttribute(iAttribute, bNot) ;
    else if (NULL != pErrors)
    {
      if (string("") != *pErrors)
        *pErrors += string(1, '\n') ;
      *pErrors += attr ;
    }
  }
}

void
XressourceAttributes::setIStyleAttribute(uint32 iAttribute, bool bNot)
{
  if (bNot)
    _iStyle &= ~iAttribute ;
  else
		_iStyle |= iAttribute ;
}

void
XressourceAttributes::setSStyleFromIStyle()
{
  _style = string("") ;

  if (styleMap.empty())
    initStyleAttributes() ;

  if (styleMap.empty())
    return ;

  map<string, uint32>::iterator iter ;
  for (iter = styleMap.begin() ; styleMap.end() != iter ; iter++)
    setSStyleAttribute((*iter).first, (*iter).second) ;
}

void
XressourceAttributes::setSStyleAttribute(string sAttribute, uint32 iAttribute, bool bProcessNot)
{
  if ((_iStyle & iAttribute) == iAttribute)
  {
    if (string("") != _style)
      _style += string(" | ") ;
    _style += sAttribute ;

    return ;
  }

  if (bProcessNot && ((_iStyle | ~iAttribute) == ~iAttribute))
  {
    if (string("") != _style)
      _style += string(" | ") ;
    _style += string("NOT ") + sAttribute ;

    return ;
  }
}

XressourceAttributes&
XressourceAttributes::operator=(const XressourceAttributes& src)
{
  if (this == &src)
		return *this ;

  _caption = src._caption ;
  _sLang   = src._sLang ;
  _type    = src._type ;
  _style   = src._style ;
  _iStyle  = src._iStyle ;
  _coords  = src._coords ;
  _x       = src._x ;
  _y       = src._y ;
  _w       = src._w ;
  _h       = src._h ;

  return *this ;
}

void
XressourceAttributes::initStyleAttributes()
{
  string sMajType = pseumaj(_type) ;

  styleMap["BSS_RGROUP"]          = BSS_RGROUP ;
  styleMap["BSS_GROUP"]           = BSS_GROUP ;
  styleMap["BSS_HDIP"]            = BSS_HDIP ;
  styleMap["BSS_VDIP"]            = BSS_VDIP ;
  styleMap["BSS_CAPTION"]         = BSS_CAPTION ;
  styleMap["BSS_CENTER"]          = BSS_CENTER ;
  styleMap["BSS_LEFT"]            = BSS_LEFT ;

  if (string("EDIT") == sMajType)
  {
    styleMap["ES_LEFT"]             = ES_LEFT ;
    styleMap["ES_CENTER"]           = ES_CENTER ;
    styleMap["ES_RIGHT"]            = ES_RIGHT ;
    styleMap["ES_READONLY"]         = ES_READONLY ;
    styleMap["ES_AUTOHSCROLL"]      = ES_AUTOHSCROLL ;
  }
  if (string("STATIC") == sMajType)
  {
    styleMap["SS_LEFT"]             = SS_LEFT ;
    styleMap["SS_CENTER"]           = SS_CENTER ;
    styleMap["SS_ETCHEDFRAME"]      = SS_ETCHEDFRAME ;
  }

  // Windows styles
  //

  styleMap["WS_BORDER"]           = WS_BORDER ;
  styleMap["WS_CAPTION"]          = WS_CAPTION ;
  styleMap["WS_CHILD"]            = WS_CHILD ;
  if (string("WINDOWS") == sMajType)
  {
    styleMap["WS_CHILDWINDOW"]      = WS_CHILDWINDOW ;
    styleMap["WS_CLIPCHILDREN"]     = WS_CLIPCHILDREN ;
    styleMap["WS_CLIPSIBLINGS"]     = WS_CLIPSIBLINGS ;
  }
  styleMap["WS_DISABLED"]         = WS_DISABLED ;
  styleMap["WS_DLGFRAME"]         = WS_DLGFRAME ;
  styleMap["WS_GROUP"]            = WS_GROUP ;
  styleMap["WS_HSCROLL"]          = WS_HSCROLL ;
  styleMap["WS_VSCROLL"]          = WS_VSCROLL ;

  // Window position
  //
  if (string("WINDOWS") == sMajType)
  {
    styleMap["WS_ICONIC"]           = WS_ICONIC ;
    styleMap["WS_MAXIMIZE"]         = WS_MAXIMIZE ;
    styleMap["WS_MAXIMIZEBOX"]      = WS_MAXIMIZEBOX ;
    styleMap["WS_MINIMIZE"]         = WS_MINIMIZE ;
    styleMap["WS_MINIMIZEBOX"]      = WS_MINIMIZEBOX ;
    styleMap["WS_TILED"]            = WS_TILED ;
    styleMap["WS_TILEDWINDOW"]      = WS_TILEDWINDOW ;
    styleMap["WS_OVERLAPPED"]       = WS_OVERLAPPED ;
    styleMap["WS_OVERLAPPEDWINDOW"] = WS_OVERLAPPEDWINDOW ;
    styleMap["WS_POPUP"]            = WS_POPUP ;
    styleMap["WS_POPUPWINDOW"]      = WS_POPUPWINDOW ;
    styleMap["WS_SIZEBOX"]          = WS_SIZEBOX ;
    styleMap["WS_SYSMENU"]          = WS_SYSMENU ;
  }
  styleMap["WS_TABSTOP"]          = WS_TABSTOP ;
  styleMap["WS_THICKFRAME"]       = WS_THICKFRAME ;
  styleMap["WS_VISIBLE"]          = WS_VISIBLE ;

  // Dialog box styles
  //
  if ((string("DIALOG") == sMajType) || (string("BORDLG") == sMajType))
  {
    styleMap["DS_3DLOOK"]           = DS_3DLOOK ;
    styleMap["DS_ABSALIGN"]         = DS_ABSALIGN ;
    styleMap["DS_CENTER"]           = DS_CENTER ;
    styleMap["DS_CENTERMOUSE"]      = DS_CENTERMOUSE ;
    styleMap["DS_CONTEXTHELP"]      = DS_CONTEXTHELP ;
    styleMap["DS_CONTROL"]          = DS_CONTROL ;
    styleMap["DS_FIXEDSYS"]         = DS_FIXEDSYS ;
    styleMap["DS_LOCALEDIT"]        = DS_LOCALEDIT ;
    styleMap["DS_MODALFRAME"]       = DS_MODALFRAME ;
    styleMap["DS_NOFAILCREATE"]     = DS_NOFAILCREATE ;
    styleMap["DS_NOIDLEMSG"]        = DS_NOIDLEMSG ;
    // styleMap[DS_RECURSE"]       = DS_RECURSE ;
    styleMap["DS_SETFONT"]          = DS_SETFONT ;
    styleMap["DS_SETFOREGROUND"]    = DS_SETFOREGROUND ;
    styleMap["DS_SYSMODAL"]         = DS_SYSMODAL ;
  }
  // Button styles
  //
  if ((string("BUTTON") == sMajType) || (string("BORBTN") == sMajType))
  {
    styleMap["BS_3STATE"]           = BS_3STATE ;
    styleMap["BS_AUTO3STATE"]       = BS_AUTO3STATE ;
    styleMap["BS_GROUPBOX"]         = BS_GROUPBOX ;
    styleMap["BS_PUSHBUTTON"]       = BS_PUSHBUTTON ;
    styleMap["BS_DEFPUSHBUTTON"]    = BS_DEFPUSHBUTTON ;
  }
  if ((string("RADIOBUTTON") == sMajType) || (string("BORRADIO") == sMajType) ||
      (string("BUTTON")      == sMajType) || (string("BORBTN")   == sMajType))
  {
    styleMap["BS_RADIOBUTTON"]      = BS_RADIOBUTTON ;
    styleMap["BS_AUTORADIOBUTTON"]  = BS_AUTORADIOBUTTON ;
  }
  if ((string("CHECKBOX") == sMajType) || (string("BORCHECK") == sMajType) ||
      (string("BUTTON")   == sMajType) || (string("BORBTN")   == sMajType))
  {
    styleMap["BS_CHECKBOX"]         = BS_CHECKBOX ;
    styleMap["BS_AUTOCHECKBOX"]     = BS_AUTOCHECKBOX ;
  }
  styleMap["BS_BITMAP"]           = BS_BITMAP ;
  styleMap["BS_ICON"]             = BS_ICON ;
  styleMap["BS_TEXT"]             = BS_TEXT ;
  styleMap["BS_LEFTTEXT"]         = BS_LEFTTEXT ;
  styleMap["BS_RIGHTBUTTON"]      = BS_RIGHTBUTTON ;
  styleMap["BS_TOP"]              = BS_TOP ;
  styleMap["BS_VCENTER"]          = BS_VCENTER ;
  styleMap["BS_BOTTOM"]           = BS_BOTTOM ;
  styleMap["BS_LEFT"]             = BS_LEFT ;
  styleMap["BS_CENTER"]           = BS_CENTER ;
  styleMap["BS_RIGHT"]            = BS_RIGHT ;
  styleMap["BS_MULTILINE"]        = BS_MULTILINE ;
  styleMap["BS_NOTIFY"]           = BS_NOTIFY ;
  styleMap["BS_PUSHLIKE"]         = BS_PUSHLIKE ;
  // List view styles
  //
  if (string("SYSLISTVIEW32") == sMajType)
  {
    styleMap["LVS_REPORT"]          = LVS_REPORT ;
    styleMap["LVS_LIST"]            = LVS_LIST ;
    styleMap["LVS_ICON"]            = LVS_ICON ;
    styleMap["LVS_SMALLICON"]       = LVS_SMALLICON ;
    styleMap["LVS_SINGLESEL"]       = LVS_SINGLESEL ;
    styleMap["LVS_NOCOLUMNHEADER"]  = LVS_NOCOLUMNHEADER ;
    styleMap["LVS_NOSORTHEADER"]    = LVS_NOSORTHEADER ;
    styleMap["LVS_ALIGNLEFT"]       = LVS_ALIGNLEFT ;
    styleMap["LVS_ALIGNTOP"]        = LVS_ALIGNTOP ;
    styleMap["LVS_AUTOARRANGE"]     = LVS_AUTOARRANGE ;
    styleMap["LVS_NOLABELWRAP"]     = LVS_NOLABELWRAP ;
    styleMap["LVS_NOSCROLL"]        = LVS_NOSCROLL ;
    // styleMap["LVS_BUTTON"]       = LVS_BUTTON ;
    styleMap["LVS_SHOWSELALWAYS"]   = LVS_SHOWSELALWAYS ;
  }
  // Combobox styles
  //
  if (string("COMBOBOX") == sMajType)
  {
    styleMap["CBS_DROPDOWNLIST"]    = CBS_DROPDOWNLIST ;
    styleMap["CBS_SORT"]            = CBS_SORT ;
  }
  // TreeView styles
  //
  if (string("SYSTREEVIEW32") == sMajType)
  {
    styleMap["TVS_HASBUTTONS"]      = TVS_HASBUTTONS ;
    styleMap["TVS_HASLINES"]        = TVS_HASLINES ;
    styleMap["TVS_LINESATROOT"]     = TVS_LINESATROOT ;
    styleMap["TVS_EDITLABELS"]      = TVS_EDITLABELS ;
    styleMap["TVS_SHOWSELALWAYS"]   = TVS_SHOWSELALWAYS ;
    styleMap["TVS_NOTOOLTIPS"]      = TVS_NOTOOLTIPS ;
    styleMap["TVS_CHECKBOXES"]      = TVS_CHECKBOXES ;
    styleMap["TVS_TRACKSELECT"]     = TVS_TRACKSELECT ;
    styleMap["TVS_SINGLEEXPAND"]    = TVS_SINGLEEXPAND ;
    styleMap["TVS_INFOTIP"]         = TVS_INFOTIP ;
    styleMap["TVS_FULLROWSELECT"]   = TVS_FULLROWSELECT ;
    styleMap["TVS_NOSCROLL"]        = TVS_NOSCROLL ;
    styleMap["TVS_NONEVENHEIGHT"]   = TVS_NONEVENHEIGHT ;
  }
}

uint32
XressourceAttributes::getStyleIntFromString(string sAttribute)
{
  if (styleMap.empty())
    initStyleAttributes() ;

  if (styleMap.empty())
    return 0 ;

  return styleMap[sAttribute] ;
}

string
XressourceAttributes::getStyleStringFromInt(uint32 iAttribute)
{
  if (styleMap.empty())
    initStyleAttributes() ;

  if (styleMap.empty())
    return string("") ;

  map<string, uint32>::iterator iter ;
  for (iter = styleMap.begin() ; styleMap.end() != iter ; iter++)
    if ((*iter).second == iAttribute)
      return (*iter).first ;

  return string("") ;
}

void
XressourceAttributes::buildCoords()
{
  _coords = IntToString(_x) + string(", ") +
            IntToString(_y) + string(", ") +
            IntToString(_w) + string(", ") +
            IntToString(_h) ;
}

void
XressourceAttributes::parseCoords()
{
  size_t deb = 0 ;
  size_t fin = _coords.find(",", deb + 1) ;
  if (fin == string::npos)
    return ;
  string sX = string(_coords, 0, fin) ;
  setX(atoi(sX.c_str())) ;

  deb = fin ;
  fin = _coords.find(",", deb + 1) ;
  if (fin == string::npos)
    return ;
  string sY = string(_coords, deb + 1, fin - deb - 1) ;
  setY(atoi(sY.c_str())) ;

  deb = fin ;
  fin = _coords.find(",", deb + 1) ;
  if (fin == string::npos)
    return ;
  string sW = string(_coords, deb + 1, fin - deb - 1) ;
  setW(atoi(sW.c_str())) ;

  deb = fin ;
  fin = strlen(_coords.c_str()) ;
  string sH = string(_coords, deb + 1, fin - deb - 1) ;
  setH(atoi(sH.c_str())) ;
}

void
Xcontrol::setStrRefId(string sRefId)
{
  strip(sRefId) ;

  if (string("") == sRefId)
  {
    setRefId(0) ;
    return ;
  }

  int iRefId = 0 ;

  // parsing des ID système
  if      (sRefId == "IDOK")
    iRefId = IDOK ;
  else if (sRefId == "IDCANCEL")
    iRefId = IDCANCEL ;
  else if (sRefId == "IDB_OK")
    iRefId = IDB_OK ;
  else if (sRefId == "IDB_CANCEL")
    iRefId = IDB_CANCEL ;
  else if (sRefId == "IDRETOUR")
    iRefId = IDRETOUR ;
  else if (sRefId == "IDSUITE")
    iRefId = IDSUITE ;
  else if (sRefId == "IDHELP")
    iRefId = IDHELP ;
  else if (sRefId == "IDHELPWWW")
    iRefId = IDHELPWWW ;
  else if (sRefId == "IDHELPNEW")
    iRefId = IDHELPNEW ;
  else if (sRefId == "IDTREEPASTE")
    iRefId = IDTREEPASTE ;
  else if (sRefId == "IDCONCLUSION")
    iRefId = IDCONCLUSION ;
  else if (sRefId == "IDBBKCALCUL")
    iRefId = IDBBKCALCUL ;
  else
    iRefId = atoi(sRefId.c_str()) ;

  setRefId(iRefId) ;
}

bool
Xcontrol::initFromValue(string sValue)
{
  if (string("") == sValue)
    return true ;

  size_t deb, fin ;

  // Parsing de la Caption
  deb = sValue.find("\"") ;
  if (deb == string::npos)
    return false ;
  fin = sValue.find("\"", deb + 1) ;
  if (fin == string::npos)
    return false ;
  setCaption(string(sValue, deb + 1, fin - deb - 1)) ;

  // Parsing du RefId
  deb = sValue.find(",", fin + 1) ;
  if (deb == string::npos)
    return false ;
  fin = sValue.find(",", deb + 1) ;
  if (fin == string::npos)
    return false ;
  string sRefId = string(sValue, deb+1, fin - deb - 1) ;

  setStrRefId(sRefId) ;

  // Parsing du type
  deb = sValue.find("\"", fin + 1) ;
  if (deb == string::npos)
    return false ;
  fin = sValue.find("\"", deb + 1) ;
  if (fin == string::npos)
    return false ;
  setType(string(sValue, deb + 1, fin - deb - 1)) ;

  // Parsing du style
  deb = sValue.find(",", fin + 1) ;
  if (deb == string::npos)
    return false ;
  fin = sValue.find(",", deb + 1) ;
  if (fin == string::npos)
    return false ;
  setStrStyle(string(sValue, deb + 1, fin - deb - 1)) ;

  // Parsing des coordonnées
  deb = fin ;
  fin = strlen(sValue.c_str()) ;
  setCoords(string(sValue, deb + 1, fin - deb - 1)) ;

  return true ;
}

string
Xcontrol::buildValueString()
{
  string sValue = string("CONTROL \"") + _caption + string("\"") ;

  // RefId
  //
  if (-1 == _refId)
    sValue += string(", -1") ;
  else
    sValue += string(", ") + IntToString(_refId) ;

  // Class
  //
  sValue += string(", \"") + _type + string("\"") ;

  // Style
  //
  setSStyleFromIStyle() ;
  sValue += string(", ") + _style ;

  // X, Y, W, H
  //
  sValue += string(", ") + IntToString(_x) ;
  sValue += string(", ") + IntToString(_y) ;
  sValue += string(", ") + IntToString(_w) ;
  sValue += string(", ") + IntToString(_h) ;

  return sValue ;
}

// -----------------------------------------------------------------------------
// balise dialogbox
// -----------------------------------------------------------------------------

Cdialogbox::Cdialogbox(string attributes, string values, Cbalise *father)
           :Cbalise(attributes, values, father)
{
}

Cdialogbox::~Cdialogbox()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Ccontrol *pCCtrl = dynamic_cast<Ccontrol*>(pBalise) ;
      if (pCCtrl)
      {
        delete pCCtrl ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Crcdata *pCRcData = dynamic_cast<Crcdata*>(pBalise) ;
        if (pCRcData)
        {
          delete pCRcData ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Ctabcontrol *pCTab = dynamic_cast<Ctabcontrol*>(pBalise) ;
          if (pCTab)
          {
            delete pCTab ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Cdialogbox::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    _iParsingError = EDIALOGBOX_ATTR_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

  int   nbLang      = 0 ;
  int   nbType      = 0 ;
  int   nbCoords    = 0 ;
  int   nbStyle     = 0 ;
  int   nbClass     = 0 ;
  int   nbCaption   = 0 ;
  int   nbFontSize  = 0 ;
  int   nbFontType  = 0 ;
  int   nbNSTitre   = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_LANG)     // langue de la dialogbox
			nbLang++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_TYPE)     // type de la dialogbox
			nbType++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_COORDS)   // coordonnées de la dialogbox
			nbCoords++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_STYLE)    // style de la dialogbox
			nbStyle++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_CLASS)    // class de la dialogbox
      nbClass++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_CAPTION)  // caption de la dialogbox
      nbCaption++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_FONTSIZE) // fontsize de la dialogbox
      nbFontSize++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_FONTTYPE) // fonttype de la dialogbox
      nbFontType++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_DIALOGBOX_NSTITRE) // titre du rcdata de la dialogbox
      nbNSTitre++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EDIALOGBOX_ATTR ;
      return false ;
    }
  }

	if ((nbType     !=  1)  ||
      (nbCoords   !=  1)  ||
      (nbStyle    !=  1)  ||
      (nbClass    !=  1)  ||
      (nbCaption  !=  1)  ||
      (nbFontSize !=  1)  ||
      (nbFontType !=  1)  ||
      (nbNSTitre  >   1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EDIALOGBOX_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
  parseValue() ;

	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EDIALOGBOX_VAL_EMPTY ;
    return false ;
  }

  int nbControl     = 0 ;
  int nbRCData      = 0 ;
  int nbTabControl  = 0 ;
	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_DIALOGBOX_CONTROL)     // control appartenant à la dialogbox
		{
      (*ival)->setObject(new Ccontrol((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
/*
      if (!(*ival)->pObjectgetObject()->parseValue())
      {
        iParsingError = ECONTROL_ATTR_PARSING;
        return false;
      }
*/
			nbControl++ ;
		}
    else if ((*ival)->getLabel() == LABEL_DIALOGBOX_RCDATA) // rcdata de la dialogbox
    {
      (*ival)->setObject(new Crcdata((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      nbRCData++ ;
    }
    else if ((*ival)->getLabel() == LABEL_DIALOGBOX_ONGLETS) // onglets de la dialogbox
    {
      (*ival)->setObject(new Ctabcontrol((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      nbTabControl++ ;
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EDIALOGBOX_VAL ;
      return false ;
    }
  }

	if ((nbControl < 1) || (nbRCData > 1) || (nbTabControl > 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EDIALOGBOX_NBVAL ;
    return false ;
  }

	return true ;
}

bool
Cdialogbox::verif()
{
  return true ;
}

bool
Cdialogbox::compresser()
{
  return true ;
}

bool
Cdialogbox::traiter()
{
  return true ;
}

int
Cdialogbox::getIStyle()
{
  return value.getStyle() ;
}

void
Cdialogbox::setIStyle()
{
  string sError = string("") ;
  value.setIStyleFromSStyle(&sError) ;
}

bool
Cdialogbox::parseValue()
{
  string    sCoords = getCoords() ;
  size_t    deb     = 0 ;
  size_t    fin ;

  // Parsing du style
  value.setStrStyle(getStyle()) ;
  setIStyle() ;

  // Parsing des coordonnées
  value.setCoords(getCoords()) ;

  fin = sCoords.find(" ") ;
  if (fin == string::npos)
    return false ;
  string sX = string(sCoords, deb, fin - deb) ;
  value.setX(atoi(sX.c_str())) ;

  deb = fin ;
  fin = sCoords.find(" ", deb + 1) ;
  if (fin == string::npos)
    return false ;
  string sY = string(sCoords, deb + 1, fin - (deb + 1)) ;
  value.setY(atoi(sY.c_str())) ;

  deb = fin ;
  fin = sCoords.find(" ", deb + 1) ;
  if (fin == string::npos)
    return false ;
  string sW = string(sCoords, deb + 1, fin - (deb + 1)) ;
  value.setW(atoi(sW.c_str())) ;

  deb = fin ;
  fin = strlen(sCoords.c_str()) ;
  string sH = string(sCoords, deb + 1, fin - (deb + 1)) ;
  value.setH(atoi(sH.c_str())) ;

  return true ;
}

void
Cdialogbox::updateParamsFromValue()
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_CAPTION, value.getCaption(), true) ;

  buildCoords() ;
  setStringAttribute(ATTRIBUT_DIALOGBOX_COORDS,  value.getCoords(), true) ;
}

int
Cdialogbox::getX()
{
  return (value.getX()) ;
}

int
Cdialogbox::getY()
{
  return (value.getY()) ;
}

int
Cdialogbox::getW()
{
  return (value.getW()) ;
}

int
Cdialogbox::getH()
{
  return (value.getH()) ;
}

string
Cdialogbox::getLang()
{
	return getStringAttribute(ATTRIBUT_DIALOGBOX_LANG) ;
}

string
Cdialogbox::getType()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_TYPE) ;
}

string
Cdialogbox::getCoords()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_COORDS) ;
}

string
Cdialogbox::getStyle()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_STYLE) ;
}

string
Cdialogbox::getClass()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_CLASS) ;
}

string
Cdialogbox::getCaption()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_CAPTION) ;
}

string
Cdialogbox::getFontSize()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_FONTSIZE) ;
}

string
Cdialogbox::getFontType()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_FONTTYPE) ;
}

string
Cdialogbox::getNSTitre()
{
  return getStringAttribute(ATTRIBUT_DIALOGBOX_NSTITRE) ;
}

void
Cdialogbox::setLang(string sLang)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_LANG, sLang, true) ;
}

void
Cdialogbox::setCoords(string sC)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_COORDS, sC, true) ;
  parseValue() ;
}

void
Cdialogbox::setType(string sTy)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_TYPE, sTy, true) ;
}

void
Cdialogbox::setStyle(string sSty)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_STYLE, sSty, true) ;
}

void
Cdialogbox::setClass(string sC)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_CLASS, sC, true) ;
}

void
Cdialogbox::setCaption(string sCap)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_CAPTION, sCap, true) ;
}

void
Cdialogbox::setFontSize(string sFS)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_FONTSIZE, sFS, true) ;
}

void
Cdialogbox::setFontType(string sFT)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_FONTTYPE, sFT, true) ;
}

void
Cdialogbox::setNSTitre(string sNST)
{
  setStringAttribute(ATTRIBUT_DIALOGBOX_NSTITRE, sNST, true) ;
}

Ccontrol *
Cdialogbox::getFirstControl()
{
  Ccontrol *pControl = NULL ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if ((*ival)->getLabel() == LABEL_DIALOGBOX_CONTROL)
      {
        pControl = dynamic_cast<Ccontrol *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pControl ;
}

Ccontrol *
Cdialogbox::getNextControl(Ccontrol *pLastControl)
{
  Ccontrol *pControl = (Ccontrol *) 0 ;
  Ccontrol *pBuf ;

  bool bReturnNext = false ;
  if (NULL == pLastControl)
  	bReturnNext = true ;

  if (vect_val.empty())
    return pControl ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_DIALOGBOX_CONTROL == (*ival)->getLabel())
    {
      pBuf = dynamic_cast<Ccontrol *>((*ival)->getObject());
      if (bReturnNext && pBuf)
      {
        pControl = pBuf;
        break;
      }
      if (pBuf == pLastControl)
        bReturnNext = true;
    }
  }

  return pControl ;
}

int
Cdialogbox::getNbControl()
{
  if (vect_val.empty())
    return 0 ;

  int nbControl = 0 ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_DIALOGBOX_CONTROL == (*ival)->getLabel())
    {
      Ccontrol *pControl = dynamic_cast<Ccontrol *>((*ival)->getObject()) ; 
      if (pControl)
        nbControl++ ;
    }
  }

  return nbControl ;
}

Crcdata *
Cdialogbox::getRCData()
{
  Crcdata *pRCData = (Crcdata *) 0 ;

  if (vect_val.empty())
    return pRCData ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_DIALOGBOX_RCDATA == (*ival)->getLabel())
    {
      pRCData = dynamic_cast<Crcdata *>((*ival)->getObject()) ;
      break ;
    }
  }

  return pRCData ;
}

Ctabcontrol *
Cdialogbox::getTabControl()
{
  Ctabcontrol *pTabControl = (Ctabcontrol *) 0 ;

  if (vect_val.empty())
    return pTabControl ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_DIALOGBOX_ONGLETS == (*ival)->getLabel())
    {
      pTabControl = dynamic_cast<Ctabcontrol *>((*ival)->getObject()) ;
      break ;
    }
  }

  return pTabControl ;
}

string
Cdialogbox::getStringRCData()
{
  string   sRCData   = string("") ;
  Ccontrol *pControl = getFirstControl() ;

  while (pControl)
  {
    string sData = pControl->getData() ;

    if (string("") == sData)
      sRCData += string(" |") ;
    else
      sRCData += (sData + string("|")) ;

    pControl = getNextControl(pControl) ;
  }

  return sRCData ;
}

string
Cdialogbox::getValueRCData()
{
  string sNSTitre = getNSTitre() ;
  string sRCData  = getStringRCData() ;

  string sReturn = string("\"NSTITRE\"\n\"") + sNSTitre + string("\"\n\"NSCONTRO\"\n\"") + sRCData + string("\"\n\"FINRCDAT\"\n") ;
  return sReturn ;
}

void
Cdialogbox::addControl(Ccontrol *pControl)
{
  if (NULL == pControl)
    return ;

  CValeur *pValeur = new CValeur(LABEL_DIALOGBOX_CONTROL, string(""), string(""), this) ;
  vect_val.push_back(pValeur) ;
  pValeur->setObject(pControl) ;
}

Ccontrol::Ccontrol(string attributes, string values, Cbalise *father)
         :Cbalise(attributes, values, father)
{
}

Ccontrol::~Ccontrol()
{
}

bool
Ccontrol::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    _iParsingError = ECONTROL_ATTR_EMPTY ;
    return false ; // erreur (pas d'attribut)
  }

  int       nbValue = 0 ;
  int       nbTab = 0 ;
  int       nbData = 0 ;
  int       nbFill = 0;

// for the future
  int       nbCaption = 0 ;
  int       nbRefId   = 0 ;
  int       nbType    = 0 ;
  int       nbStyle   = 0 ;
  int       nbCoords  = 0 ;
  int       nbHelpTxt = 0 ;

	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_VALUE)      // valeur du control
			nbValue++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_ONGLET) // onglet associé
			nbTab++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_DATA)  // chemin du RCData associé
			nbData++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_FILLING)  // chemin du coche associé
			nbFill++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_HELP_TEXT)  // help text
			nbHelpTxt++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_CAPTION) // dll
    {
			nbCaption++ ;
      setCaption((*iattribut)->getValue()) ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_REFID) // referentiel
    {
			nbRefId++ ;
      setStrRefId((*iattribut)->getValue()) ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_TYPE) // referentiel
    {
			nbType++ ;
      setType((*iattribut)->getValue()) ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_STYLE) // referentiel
    {
			nbStyle++ ;
      setStrStyle((*iattribut)->getValue()) ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_CONTROL_COORDS) // referentiel
    {
			nbCoords++ ;
      setCoords((*iattribut)->getValue()) ;
    }
		else
    {
      _iParsingError = ECONTROL_ATTR ;
      return false; // erreur (attribut incorrect)
    }
  }

  bool bExistValues = false ;
  if ((nbValue > 0) || ((nbRefId > 0) && (nbType > 0) && (nbStyle > 0) && (nbCoords > 0)))
    bExistValues = true ;

	if ((false == bExistValues) || (nbTab > 1) || (nbData > 1) || (nbFill > 1))
  {
    _iParsingError = ECONTROL_NBATTR ;
    return false ; // erreur (nb attributs incorrect)
  }

  // ---------------------------------------------------------------------------
	// valeurs
	parseValue() ;

	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = ECONTROL_VAL_NOT_EMPTY ;
    return false ;
  }
	return true ;
}


bool
Ccontrol::verif()
{
  return true ;
}


bool
Ccontrol::compresser()
{
  return true ;
}

bool
Ccontrol::traiter()
{
  return true ;
}

string
Ccontrol::getValue()
{
  return getStringAttribute(ATTRIBUT_CONTROL_VALUE) ;
}

string
Ccontrol::getUpdatedValue()
{
  return value.buildValueString() ;
}

void
Ccontrol::updateParamsFromValue()
{
  setStringAttribute(ATTRIBUT_CONTROL_CAPTION, value.getCaption(),  true) ;
  setStringAttribute(ATTRIBUT_CONTROL_TYPE,    value.getType(),     true) ;
  setStyleString() ;
  setStringAttribute(ATTRIBUT_CONTROL_STYLE,   value.getStrStyle(), true) ;
  buildCoords() ;
  setStringAttribute(ATTRIBUT_CONTROL_COORDS,  value.getCoords(),   true) ;

  setIntAttribute(ATTRIBUT_CONTROL_REFID, value.getRefId(), true) ;

  // Make certain that ATTRIBUT_CONTROL_VALUE is erased
  //
}

string
Ccontrol::getTab()
{
  return getStringAttribute(ATTRIBUT_CONTROL_ONGLET) ;
}

string
Ccontrol::getData()
{
  return getStringAttribute(ATTRIBUT_CONTROL_DATA) ;
}

string
Ccontrol::getDataIdentity()
{
  string sData = getData() ;
  return getDataLeft(sData) ;
}

string
Ccontrol::getDataFunction()
{
  string sData = getData() ;
  string sRight = getDataRight(sData) ;
  return getDataLeft(sRight) ;
}

string
Ccontrol::getDataExtension()
{
  string sData = getData() ;
  string sRight = getDataRight(sData) ;
  string sRightRight = getDataRight(sRight) ;
  return getDataLeft(sRightRight) ;
}

// Return the part that is left of ':' or the whole string if no ':'
//
string
Ccontrol::getDataLeft(string sData)
{
  if (string("") == sData)
    return string("") ;

  size_t fct_posit = sData.find(':') ;
  if (string::npos == fct_posit)
    return sData ;

  return string(sData, 0, fct_posit) ;
}

// Return the part that is right of ':' or "" if no ':'
//
string
Ccontrol::getDataRight(string sData)
{
  if (string("") == sData)
    return string("") ;

  size_t fct_posit = sData.find(':') ;
  if ((string::npos == fct_posit) || (fct_posit == strlen(sData.c_str())-1))
    return string("") ;

  return string(sData, fct_posit + 1, strlen(sData.c_str())) ;
}

string
Ccontrol::getFilling()
{
  return getStringAttribute(ATTRIBUT_CONTROL_FILLING) ;
}

string
Ccontrol::getHelpText()
{
  return getStringAttribute(ATTRIBUT_CONTROL_HELP_TEXT) ;
}

void
Ccontrol::setValue(string sValue)
{
  setStringAttribute(ATTRIBUT_CONTROL_VALUE, sValue, true) ;
}

void
Ccontrol::setTab(string sTab)
{
  setStringAttribute(ATTRIBUT_CONTROL_ONGLET, sTab, true) ;
}

void
Ccontrol::setData(string sData)
{
  setStringAttribute(ATTRIBUT_CONTROL_DATA, sData, true) ;
}

void
Ccontrol::setDataIdentity(string sIdentity)
{
  string sValue = sIdentity ;

  string sFunction = getDataFunction() ;
  if (string("") != sFunction)
    sValue += string(":") + sFunction ;

  setData(sValue) ;
}

void
Ccontrol::setDataFunction(string sFunction)
{
  string sValue = getDataIdentity() ;

  if (string("") != sFunction)
    sValue += string(":") + sFunction ;

  setData(sValue) ;
}

void
Ccontrol::setFilling(string sFilling)
{
  setStringAttribute(ATTRIBUT_CONTROL_FILLING, sFilling, true) ;
}

void
Ccontrol::setHelpText(string sHelpText)
{
  setStringAttribute(ATTRIBUT_CONTROL_HELP_TEXT, sHelpText, true) ;
}

void
Ccontrol::setIStyle()
{
  string sError = string("") ;
  value.setIStyleFromSStyle(&sError) ;
}

void
Ccontrol::setStyleString()
{
  value.setSStyleFromIStyle() ;
}

void
Ccontrol::setStyleStringElement()
{
}

int
Ccontrol::getIStyle()
{
  return value.getStyle() ;
}

bool
Ccontrol::parseValue()
{
  string sValue = getValue() ;

  bool bParseResult = value.initFromValue(sValue) ;
  setIStyle() ;

  return bParseResult ;
}

// for the future - do not care about it for the moment
string
Ccontrol::getCaption()
{
  return texteCourant(value.getCaption()) ;
}

// for the future - do not care about it for the moment
int
Ccontrol::getRefId()
{
  return value.getRefId() ;
}

// for the future - do not care about it for the moment
string
Ccontrol::getType()
{
  return texteCourant(value.getType()) ;
}

// for the future - do not care about it for the moment
string
Ccontrol::getStyle()
{
  return texteCourant(value.getStrStyle()) ;
}

// for the future - do not care about it for the moment
string
Ccontrol::getCoords()
{
  return texteCourant(value.getCoords()) ;
}

int
Ccontrol::getX()
{
  return value.getX() ;
}

int
Ccontrol::getY()
{
  return value.getY() ;
}

int
Ccontrol::getW()
{
  return value.getW() ;
}

int
Ccontrol::getH()
{
  return value.getH() ;
}

void
Ccontrol::setRect(int iX, int iY, int iW, int iH)
{
  setX(iX) ;
  setY(iY) ;
  setW(iW) ;
  setH(iH) ;
}

Crcdata::Crcdata(string attributes, string values, Cbalise *father)
  : Cbalise(attributes, values, father)
{
}

Crcdata::~Crcdata()
{
}

bool
Crcdata::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ERCDATA_ATTR_EMPTY ;
    return false ;
  }

  int nbNSTitre  = 0 ;
  int nbNSContro = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_RCDATA_NSTITRE)      // nstitre du rcdata
			nbNSTitre++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_RCDATA_NSCONTRO)
      nbNSContro++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ERCDATA_ATTR ;
      return false ;
    }
  }

	if ((nbNSTitre > 1) || (nbNSContro != 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ERCDATA_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;
    
	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = ERCDATA_VAL_NOT_EMPTY;
    return false ;
  }
	return true ;
}

bool
Crcdata::verif()
{
  return true ;
}

bool
Crcdata::compresser()
{
  return true ;
}


bool
Crcdata::traiter()
{
  return true ;
}


string
Crcdata::getNSTitre()
{
  return getStringAttribute(ATTRIBUT_RCDATA_NSTITRE) ;
}


string
Crcdata::getNSContro()
{
  return getStringAttribute(ATTRIBUT_RCDATA_NSCONTRO) ;
}


string
Crcdata::getValue()
{
  char    pcReturn[1024] ;
  sprintf(pcReturn, "\"NSTITRE\"\n\"%s\"\n\"NSCONTRO\"\n\"%s\"\n\"FINRCDAT\"\n", getNSTitre().c_str(), getNSContro().c_str()) ;
  string  sReturn = pcReturn ;

  delete [] pcReturn ;
  return sReturn ;
}


// -----------------------------------------------------------------------------
//
// Classe Ctabcontrol
//
// -----------------------------------------------------------------------------

Ctabcontrol::Ctabcontrol(string attributs, string values, Cbalise *father)
  : Cbalise(attributs, values, father)
{
}

Ctabcontrol::~Ctabcontrol()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Ctab *pCTab = dynamic_cast<Ctab*>(pBalise) ;
      if (pCTab)
      {
        delete pCTab ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Ctabcontrol::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETABCONTROL_VAL_EMPTY ;
    return false ;
  }

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    if ((*ival)->getLabel() == LABEL_ONGLET) // dialogue
    {
      (*ival)->setObject(new Ctab((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
    }
    else
    {
      // erreur (valeur incorrecte)
      _iParsingError = ETABCONTROL_VAL ;
      return false ;
    }
  }
	return true ;
}boolCtabcontrol::verif(){
  return true ;
}


bool
Ctabcontrol::compresser()
{
  return true ;
}


bool
Ctabcontrol::traiter()
{
  return true ;
}


Ctab *
Ctabcontrol::getFirstTab()
{
  Ctab      *pTab = NULL ;

  if (!vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if ((*ival)->getLabel() == LABEL_ONGLET)
      {
        pTab = dynamic_cast<Ctab *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pTab ;
}


Ctab *
Ctabcontrol::getNextTab(Ctab *pLastTab)
{
  Ctab      *pTab       = NULL ;
  Ctab      *pBuf ;
  bool      bReturnNext = false ;
  if (!pLastTab)
  	bReturnNext = true ;

  if (!vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if ((*ival)->getLabel() == LABEL_ONGLET)
      {
        pBuf = dynamic_cast<Ctab *>((*ival)->getObject()) ;
        if (bReturnNext && pBuf)
        {
          pTab = pBuf ;
          break ;
        }
        if (pBuf == pLastTab)
          bReturnNext = true ;
      }
    }
  }
  return pTab ;
}


int
Ctabcontrol::getNbTabs()
{
  Ctab      *pTab   = NULL ;
  int       nbTabs  = 0 ;

  if (!vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if ((*ival)->getLabel() == LABEL_ONGLET)
      {
        pTab = dynamic_cast<Ctab *>((*ival)->getObject()) ;
        if (pTab)
          nbTabs++ ;
      }
    }
  }
  return nbTabs ;
}


// -----------------------------------------------------------------------------
//
// Classe Ctab
//
// -----------------------------------------------------------------------------

Ctab::Ctab(string attributes, string values, Cbalise *father)
  : Cbalise(attributes, values, father)
{
}


Ctab::~Ctab()
{
}


bool
Ctab::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ETAB_ATTR_EMPTY ;
    return false ;
  }

  int       nbTitle = 0 ;
  int       nbOrder = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_ONGLET_TITRE)      // titre de l'onglet
			nbTitle++ ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_ONGLET_NUM)       // index
      nbOrder++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ETAB_ATTR ;
      return false ;
    }
  }

	if ((nbTitle != 1) || (nbOrder != 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ETAB_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = ETAB_VAL_NOT_EMPTY;
    return false ;
  }
  
	return true ;
}


bool
Ctab::verif()
{
  return true ;
}


bool
Ctab::compresser()
{
  return true ;
}


bool
Ctab::traiter()
{
  return true ;
}


string
Ctab::getTitle()
{
  return getStringAttribute(ATTRIBUT_ONGLET_TITRE) ;
}


string
Ctab::getOrder()
{
  return getStringAttribute(ATTRIBUT_ONGLET_NUM) ;
}


// -----------------------------------------------------------------------------
//
// Classe Cpresentation
//
// -----------------------------------------------------------------------------

Cpresentation::Cpresentation(string attributes, string values, Cbalise *father)
  : Cbalise(attributes, values, father)
{
}


Cpresentation::~Cpresentation()
{
}


bool
Cpresentation::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EPRESENT_ATTR_EMPTY ;
    return false ;
  }

  int       nbVal   = 0 ;
	for (AttrIter iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
		if      ((*iattribut)->getLabel() == ATTRIBUT_PRESENT_VALUE)      // valeur (string html)
			nbVal++ ;
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = EPRESENT_ATTR ;
      return false ;
    }
  }

	if (nbVal != 1)
  {
    // erreur (nb attributs incorrect)
    _iParsingError = EPRESENT_NBATTR ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = EPRESENT_VAL_NOT_EMPTY ;
    return false ;
  }

	return true ;
}

bool
Cpresentation::verif()
{
  return true ;
}

bool
Cpresentation::compresser()
{
  return true ;
}

bool
Cpresentation::traiter()
{
  return true ;
}

string
Cpresentation::getValue()
{
  return getStringAttribute(ATTRIBUT_PRESENT_VALUE) ;
}


// -----------------------------------------------------------------------------
// classe Cglobalvars
// -----------------------------------------------------------------------------
// Cette classe représente les variables globales qui peuvent être présentes au
// sein d'un référentiel.
// --
// This class contains global vars that can be present in a referential.
// -----------------------------------------------------------------------------

Cglobalvars::Cglobalvars(string attributs, string values, Cbalise *father, NSSuper* pSuper)
            :Cbalise(attributs, values, father)
{
  _pSuper    = pSuper ;
  _pGVars    = new GlobalVarArray() ;
  _processed = false ;
}

Cglobalvars::~Cglobalvars()
{
  if (_pGVars)
    delete _pGVars ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>(pBalise) ;
      if (pCCtr)
      {
        delete pCCtr ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Cvariable *pCVar = dynamic_cast<Cvariable*>(pBalise) ;
        if (pCVar)
        {
          delete pCVar ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Cvalidateur *pCVal = dynamic_cast<Cvalidateur*>(pBalise) ;
          if (pCVal)
          {
            delete pCVal ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
Cglobalvars::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

  if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EGVARS_VAL_EMPTY ;
    return false ;
  }

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if      ((*ival)->getLabel() == LABEL_CONTRAINTE) // contrainte
      {
        (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing contrainte
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        _pGVars->push_back(new GlobalVar((*ival)->getObject(), GlobalVar::constraint, this)) ;
      }
      else if ((*ival)->getLabel() == LABEL_VARIABLE)
      {
        (*ival)->setObject(new Cvariable((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing variable
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        _pGVars->push_back(new GlobalVar((*ival)->getObject(), GlobalVar::variable, this)) ;
      }
      else if ((*ival)->getLabel() == LABEL_VALIDATEUR)
      {
        (*ival)->setObject(new Cvalidateur((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
        if (false == (*ival)->getObject()->parser())
        {
          // erreur parsing validateur
          _iParsingError = (*ival)->getObject()->getParsingError() ;
          return false ;
        }
        _pGVars->push_back(new GlobalVar((*ival)->getObject(), GlobalVar::validator, this)) ;
      }
      else
      {
        // erreur (valeur incorrecte)
        _iParsingError = EGVARS_VAL ;
        return false ;
      }
    }
  }
	return true ;
}

bool
Cglobalvars::verif()
{
  return true ;
}

bool
Cglobalvars::compresser()
{
  return true ;
}

bool
Cglobalvars::traiter()
{
  return true ;
}

Valeur_array *
Cglobalvars::getValArray()
{
  return (&vect_val) ;
}

bool
Cglobalvars::isProcessed()
{
  return (_processed) ;
}

bool
Cglobalvars::process(NSContexte* pCtx, bool bUserIsWaiting, HWND interfaceHandle, NSPatPathoArray* pLocalPatho)
{
  if ((NULL == _pGVars) || _pGVars->empty() || isProcessed())
    return false ;

  // First step, process the real variables
  //
  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if ((GlobalVar::variable == (*gvIter)->getType()) &&
        (GlobalVar::plainVar == (*gvIter)->getSubtype()))
      (*gvIter)->process(pCtx, bUserIsWaiting, interfaceHandle, pLocalPatho) ;

  // Second step, process the variables of variables (compound variables)
  //
  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if ((GlobalVar::variable == (*gvIter)->getType()) &&
        (GlobalVar::compoundVar == (*gvIter)->getSubtype()))
      (*gvIter)->process(pCtx, bUserIsWaiting, interfaceHandle, pLocalPatho) ;

  // Third step, process constraints and validators
  //
  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (GlobalVar::variable != (*gvIter)->getType())
      (*gvIter)->process(pCtx, bUserIsWaiting, interfaceHandle, pLocalPatho) ;

  _processed = true ;

  return true ;
}

void
Cglobalvars::resetProcessState()
{
  _processed = false ;

  if ((NULL == _pGVars) || _pGVars->empty())
    return ;

  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    (*gvIter)->resetProcessState() ;
}

bool
Cglobalvars::isVarP(string sLabel, GlobalVar::gvType typ)
{
  if ((NULL == _pGVars) || _pGVars->empty() || (string("") == sLabel))
    return false ;

  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (((*gvIter)->getName() == sLabel) && ((*gvIter)->getType() == typ))
      return true ;

  return false ;
}

NSPatPathoArray *
Cglobalvars::getPPT(string sLabel, GlobalVar::gvType typ)
{
  if (false == isVarP(sLabel, typ))
    return (NSPatPathoArray *) 0 ;

  if ((NULL == _pGVars) || _pGVars->empty())
    return (NSPatPathoArray *) 0 ;

  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (((*gvIter)->getName() == sLabel) && ((*gvIter)->getType() == typ))
      return ((*gvIter)->getPPT()) ;

  return (NSPatPathoArray *) 0 ;
}

GlobalVar::gvState
Cglobalvars::getState(string sLabel, GlobalVar::gvType typ)
{
  if (false == isVarP(sLabel, typ))
    return (GlobalVar::stateNotDef) ;

  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (((*gvIter)->getName() == sLabel) && ((*gvIter)->getType() == typ))
      return ((*gvIter)->getState()) ;

  return (GlobalVar::stateNotDef) ;
}

Cbalise *
Cglobalvars::getBalise(string sLabel, GlobalVar::gvType typ)
{
  if (false == isVarP(sLabel, typ))
    return (Cbalise *) 0 ;

  if ((NULL == _pGVars) || _pGVars->empty())
    return (Cbalise *) 0 ;

  for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (((*gvIter)->getName() == sLabel) && ((*gvIter)->getType() == typ))
      return ((*gvIter)->getBalise()) ;

  return (Cbalise *) 0 ;
}

string
#ifndef _ENTERPRISE_DLL
Cglobalvars::getPublishingString(NSContexte *pContexte)
#else
Cglobalvars::getPublishingString()
#endif
{
	if ((NULL == _pGVars) || _pGVars->empty())
		return string("") ;

	string sToPublish = string("") ;

	for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
  {
    if (((*gvIter)->getState() == GlobalVar::found) &&
    	  ((*gvIter)->getLabel() != "") && ((*gvIter)->getToPublish()))
		{
    	sToPublish += string("<li>") + (*gvIter)->getLabel() + string("</li>\r\n") ;
#ifndef _ENTERPRISE_DLL
      pContexte->getPredi()->addToLogPage((*gvIter)->getLabel(), string("<li>"), string("</li>\r\n")) ;
#endif
		}
  }

	return sToPublish ;
}

bool
Cglobalvars::existPublishingString()
{
	if ((NULL == _pGVars) || _pGVars->empty())
		return false ;

	for (GlobalVarCIter gvIter = _pGVars->begin() ; _pGVars->end() != gvIter ; gvIter++)
    if (((*gvIter)->getState() == GlobalVar::found) &&
    	  ((*gvIter)->getLabel() != "") && ((*gvIter)->getToPublish()))
    	return true ;

	return false ;
}

// -----------------------------------------------------------------------------
// classe GlobalVar
// -----------------------------------------------------------------------------
// Cette classe représente une variable globale utilisable au sein d'une contrainte.
// --
// This class contains a global var that can be used inside validity constraints
// -----------------------------------------------------------------------------

GlobalVar::GlobalVar(Cbalise *bal, gvType typ, Cglobalvars *pGlobalVars)
{
  _state   = stateNotDef ;
  _balise  = bal ;
  _type    = typ ;
  _subtype = subtypeNotDef ;

  _globalVars = pGlobalVars ;

  switch (_type)
  {
    case variable   :
    {
      Cvariable * pCvariable = dynamic_cast<Cvariable *>(_balise) ;
      if (pCvariable)
      {
        _name  = pCvariable->getStringAttribute(ATTRIBUT_VAR_NOM) ;
        _label = pCvariable->getStringAttribute(ATTRIBUT_VAR_LABEL) ;
        _bPublish       = false ;
        _classification = string("") ;

        string sExpression = pCvariable->getExpression() ;
        if (string("") == sExpression)
          _subtype = plainVar ;
        else
          _subtype = compoundVar ;

        break ;
      }
    }
    case constraint :
    {
      Ccontrainte * pCcontrainte = dynamic_cast<Ccontrainte *>(_balise) ;
      if (pCcontrainte)
      {
        _name  = pCcontrainte->getStringAttribute(ATTRIBUT_CONTR_NOM) ;
        _label = pCcontrainte->getStringAttribute(ATTRIBUT_CONTR_LABEL) ;
        _bPublish       = pCcontrainte->mustPublish() ;
        _classification = pCcontrainte->getStringAttribute(ATTRIBUT_CONTR_CLASSIFY) ;
        break ;
      }
    }
    case validator  :
    default         :
      _name  = "" ;
      _label = "" ;
      _bPublish       = false ;
      _classification = "" ;
      break ;
  }

  _alias   = "" ;
  _ppt     = NULL ;
}

GlobalVar::~GlobalVar()
{
  if (_ppt != NULL)
    delete _ppt ;
}

bool
GlobalVar::process(NSContexte* pCtx, bool bUserIsWaiting, HWND interfaceHandle, NSPatPathoArray* pLocalPatho)
{
  if (notFound == _state)
    return false ;
  if (found == _state)
    return true ;

  // fabFIXME
  _alias = string("") ;
  Cvariable* pCvar = dynamic_cast<Cvariable *>(_balise) ;
  if (pCvar)
  {
    if ((plainVar == getSubtype()) && (pCvar->getStringAttribute(ATTRIBUT_VAR_NOM) == _name) && (false == pCvar->vect_val.empty()))
    {
      // Parcours des alias avec question au blackboard
      for (ValIter ival = pCvar->vect_val.begin() ; pCvar->vect_val.end() != ival ; ival++)
      {
        if (LABEL_ALIAS == (*ival)->getLabel()) // items
        {
          Calias *pAlias = dynamic_cast<Calias *>((*ival)->getObject()) ;
          if (NULL == pAlias)
            break ;

          // pour l'instant le blackboard ne répond qu'à une question oui/non
          // L'alias doit etre un chemin de codes sens (séparateur : '/')
          NSPatPathoArray* pPatPathoLocale = (NSPatPathoArray *) 0 ;
          // on pose la question au blackboard
  #ifdef __OB1__
          // int res = pCtx->getSuperviseur()->getBBinterface()->getAnswer2Question(sVarAlias, "", &pPatPathoLocale, false) ;

          string sQuestion = string("") ;
          string sParams   = string("") ;
          splitString(pAlias->getValue(), &sQuestion, &sParams, '|') ;

          NSSearchStruct searchStruct ;
          searchStruct.init(sParams) ;

          sQuestion = getRegularPath(sQuestion, cheminSeparationMARK, intranodeSeparationMARK) ;

          // It is useful to compute, elswhere if the KS creates some information
          // the referential will ignore it
          string sAnswerDate ;
          bool bFound = pCtx->getBBinterface()->synchronousCall(sQuestion, &pPatPathoLocale, interfaceHandle, &sAnswerDate, &searchStruct) ;
          // Modif PA 2011-03-08 int res = pCtx->getBBinterface()->getAnswer2Question(sVarAlias, "", &pPatPathoLocale, sAnswerDate, true, bUserIsWaiting, interfaceHandle) ;
  #else
          int res = pCtx->getBlackboard()->getAnswer2Question(sQuestion, "", &pPatPathoLocale, false) ;
  #endif
          if (bFound)
          // if (1 == res)
          {
            // on teste d'abord l'existence de la variable
            // (la patpatho résultat est non vide ==> la variable existe)
            if (pPatPathoLocale && (false == pPatPathoLocale->empty()))
            {
              _alias  = sQuestion ;
              _ppt    = pPatPathoLocale ;
              _type   = variable ;
              _state  = found ;
              return true ;
            }
          }
          if (pPatPathoLocale)
            delete pPatPathoLocale ;
        }

        // Local path: to be searched for into _pLocalPatho
        //
        else if (LABEL_LOCAL_ALIAS == (*ival)->getLabel()) // items
        {
          Calias *pAlias = dynamic_cast<Calias *>((*ival)->getObject()) ;
          if (NULL == pAlias)
            break ;

          string sVarAlias = pAlias->getValue() ;
          sVarAlias = getRegularPath(sVarAlias, cheminSeparationMARK, intranodeSeparationMARK) ;

          if (pLocalPatho)
          {
            PatPathoIter pptIt = pLocalPatho->begin() ;
            if (pLocalPatho->CheminDansPatpatho(sVarAlias, string(1, cheminSeparationMARK), &pptIt))
            {
              NSPatPathoArray* pPatPathoLocale = new NSPatPathoArray(pCtx->getSuperviseur()) ;
              pLocalPatho->ExtrairePatPathoFreres(pptIt, pPatPathoLocale) ;

              _alias  = sVarAlias ;
              _ppt    = pPatPathoLocale ;
              _type   = variable ;
              _state  = found ;

              return true ;
            }
          }
        }
      } // fin du for sur les alias

      _type   = variable ;
      _state  = notFound ;

      if (_ppt)
      {
        delete _ppt ;
        _ppt = (NSPatPathoArray *) 0 ;
      }

    } // fin du if (pCvar->getStringAttribute(ATTRIBUT_VAR_NOM) == sName)
    else if ((compoundVar == getSubtype()) && (string("") != pCvar->getExpression()))
    {
      string sReservedOperators = string("()+-*/^£") ;

      string sExpression = pCvar->getExpression() ;

      // First step: get other vars names
      //
      VectString aProcessedVars ;

      size_t iBlockStart = 0 ;
      size_t iCurseur = 0 ;
      while (iCurseur < strlen(sExpression.c_str()))
      {
        // This char is a reserved char
        //
        if (sReservedOperators.find(sExpression[iCurseur]) != string::npos)
        {
          string sBlockContent = string(sExpression, iBlockStart, iCurseur - iBlockStart) ;
          strip(sBlockContent, stripBoth) ;

          // If can be a var or a number
          //
          if ((string("") != sBlockContent) && (false == aProcessedVars.contains(sBlockContent)))
          {
            if (_globalVars->isVarP(sBlockContent, variable))
              aProcessedVars.push_back(new string(sBlockContent)) ;
            else
            {
              double dValue = StringToDouble(sBlockContent) ;
              if (0 == dValue)
              {
                strip(sBlockContent, stripBoth, '0') ;
                if ((string("") != sBlockContent) && (string(".") != sBlockContent))
                  return false ;
              }
            }
          }

          iCurseur++ ;

          // Looking for the starting char of next block (not a separator)
          //
          while ((iCurseur < strlen(sExpression.c_str())) &&
             (sReservedOperators.find(sExpression[iCurseur]) != string::npos))
            iCurseur++ ;

          iBlockStart = iCurseur ;
        }
        else
          iCurseur++ ;
      }
      string sBlockContent = string(sExpression, iBlockStart, iCurseur - iBlockStart) ;
      strip(sBlockContent, stripBoth) ;
      // If can be a var or a number
      //
      if ((string("") != sBlockContent) && (false == aProcessedVars.contains(sBlockContent)))
      {
        if (_globalVars->isVarP(sBlockContent, variable))
          aProcessedVars.push_back(new string(sBlockContent)) ;
        else
        {
          double dValue = StringToDouble(sBlockContent) ;
          if (0 == dValue)
          {
            strip(sBlockContent, stripBoth, '0') ;
            if ((string("") != sBlockContent) && (string(".") != sBlockContent))
              return false ;
          }
        }
      }

      // No vars: compute the expression
      //
      if (aProcessedVars.empty())
      {
        double dResult = calculate(NULL, sExpression) ;
        string sResult = DoubleToString(&dResult, 8, 3) ;
        string sUnit   = pCvar->getUnit() ;
        if (string("") == sUnit)
          sUnit = string("200001") ;

        _ppt = new NSPatPathoArray(pCtx->getSuperviseur()) ;
        Message msg ;
        msg.SetUnit(sUnit.c_str()) ;
        msg.SetComplement(sResult.c_str()) ;
        _ppt->ajoutePatho("£N08;3", &msg, 0) ;

        _state  = found ;
        return true ;
      }

      // Get vars values
      //
      map<string, double> aValues ;

      for (IterString it = aProcessedVars.begin() ; aProcessedVars.end() != it ; it++)
      {
        // Just check if this path exists, or "left value operator right value"?
        //
        string sVarName = **it ;

        // A needed var has not been found; nothing can get done
        //
        gvState varState = _globalVars->getState(sVarName, variable) ;
        if (found != varState)
          return false ;

        NSPatPathoArray *pPPT = _globalVars->getPPT(sVarName, variable) ;
        if ((NULL == pPPT) || (pPPT->empty()))
          return false ;

        string sFormat  = string("") ;
        string sValeur1 = string("") ;
        string sUnite1  = string("") ;
        PatPathoIter iter = pPPT->begin() ;
        while (pPPT->end() != iter)
        {
          string sLexique = (*iter)->getLexique() ;
          if ((string("") != sLexique) && ('£' == sLexique[0]))
          {
            sFormat  = (*iter)->getLexiqueSens() ;
            sValeur1 = (*iter)->getComplement() ;
            sUnite1  = (*iter)->getUnitSens() ;
            break ;
          }
          iter++ ;
        }
        if (string("") == sValeur1)
          return false ;
        double dLatestVarValue = StringToDouble(sValeur1) ;

        aValues[sVarName] = dLatestVarValue ;
      }

      double dResult = calculate(&aValues, sExpression) ;
      string sResult = DoubleToString(&dResult, 8, 3) ;
      string sUnit   = pCvar->getUnit() ;
      if (string("") == sUnit)
        sUnit = string("200001") ;

      _ppt = new NSPatPathoArray(pCtx->getSuperviseur()) ;
      Message msg ;
      msg.SetUnit(sUnit.c_str()) ;
      msg.SetComplement(sResult.c_str()) ;
      _ppt->ajoutePatho("£N08;3", &msg, 0) ;

      _state  = found ;
      return true ;
    }
  }

  Ccontrainte* pCconstraint = dynamic_cast<Ccontrainte *>(_balise) ;
  if ((pCconstraint) && (pCconstraint->getStringAttribute(ATTRIBUT_CONTR_NOM) == _name))
  {
    NSValidateur valid(pCconstraint, pCtx) ;
    valid.setGlobalVars(getGlobalVars()) ;
    if (valid.getValeurContrainte(_name))    {      _type   = constraint ;      _state  = found ;      return true ;    }    _type   = constraint ;    _state  = notFound ;    if (_ppt)    {
      delete _ppt ;
      _ppt = (NSPatPathoArray *) 0 ;
    }  }

  return false ;
}

void
GlobalVar::resetProcessState()
{
  _state = stateNotDef ;
}

NSPatPathoArray *
GlobalVar::getPPT()
{
  if (_type == variable)
    return _ppt ;
  return NULL ;
}

string
GlobalVar::getName()
{
  return _name ;
}

string
GlobalVar::getLabel()
{
  return _label ;
}

bool
GlobalVar::getToPublish()
{
	return _bPublish ;
}

string
GlobalVar::getClassification()
{
  return _classification ;
}

GlobalVar::gvState
GlobalVar::getState()
{
  return _state ;
}


GlobalVar::gvType
GlobalVar::getType()
{
  return _type ;
}

GlobalVar::gvSubType
GlobalVar::getSubtype()
{
  return _subtype ;
}

Cbalise *
GlobalVar::getBalise()
{
  return _balise ;
}

// -----------------------------------------------------------------------------
// classe Cguideline
// -----------------------------------------------------------------------------

Cguideline::Cguideline(string attributs, string values, Cbalise *father, NSSuper* pSuper)
           :Cbalise(attributs, values, father)
{
  _pSuper      = pSuper ;
  _pGlobalVars = 0 ;
}

Cguideline::~Cguideline()
{
  // _pGlobalVars as an alias for a pObject... so better don't use delete on it!
  //
  // if (_pGlobalVars != NULL)
  //  delete _pGlobalVars ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      Csteps *pCStep = dynamic_cast<Csteps*>(pBalise) ;
      if (pCStep)
      {
        delete pCStep ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        Cdialogbox *pCDlg = dynamic_cast<Cdialogbox*>(pBalise) ;
        if (pCDlg)
        {
          delete pCDlg ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          Creferences *pCRef = dynamic_cast<Creferences*>(pBalise) ;
          if (pCRef)
          {
            delete pCRef ;
            (*ival)->setObject((Cbalise*) 0) ;
          }
          else
          {
            Ccontrainte *pCCtr = dynamic_cast<Ccontrainte*>(pBalise) ;
            if (pCCtr)
            {
              delete pCCtr ;
              (*ival)->setObject((Cbalise*) 0) ;
            }
            else
            {
              Cglobalvars *pCVars = dynamic_cast<Cglobalvars*>(pBalise) ;
              if (pCVars)
              {
                delete pCVars ;
                (*ival)->setObject((Cbalise*) 0) ;
              }
            }
          }
        }
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

Cguideline::Cguideline(const Cguideline& rv)
           :Cbalise(rv)
{
  _pSuper      = rv._pSuper ;
  _pGlobalVars = rv._pGlobalVars ;
}

Cguideline&
Cguideline::operator=(const Cguideline& src)
{
  if (&src == this)
		return *this ;

	Cbalise* pbdata1 = this;
	Cbalise* pbdata2 = (Cguideline*)&src;

  // Appel de l'operateur = de Cbalise (recopie des arguments de la classe Cbalise)
	*pbdata1 = *pbdata2;

	_pSuper = src._pSuper ;

  return (*this) ;
}

bool
Cguideline::parser()
{
  // ---------------------------------------------------------------------------
  // attributs
  if (false == parser_attributs())
    return false ;

  if (getStringAttribute(ATTRIBUT_ARCHETYPE_NAME) == "")
  {
    // erreur (pas de nom)
    _iParsingError = EARCHETYPE_NONAME ;
    return false ;
  }

  // ---------------------------------------------------------------------------
	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

  int nb_dialogue     = 0 ;
  int nb_dialogbox    = 0 ;
  int nb_steps        = 0 ;
  int nb_contraintes  = 0 ;
  int nb_present      = 0 ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      (LABEL_STEPS == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Csteps((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_steps++ ;
		}
    else if (LABEL_DIALOGBOX == (*ival)->getLabel()) // dialogue
		{
      (*ival)->setObject(new Cdialogbox((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_dialogbox++ ;
		}
    else if (LABEL_REFERENCES == (*ival)->getLabel()) // references
    {
      (*ival)->setObject(new Creferences((*ival)->getAttribute(), (*ival)->getValue(), this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
    }
    else if (LABEL_CONTRAINTES == (*ival)->getLabel()) // contraintes
    {
      (*ival)->setObject(new Ccontrainte((*ival)->getAttribute(), (*ival)->getValue(), this, true)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing contraintes
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_contraintes++ ;
    }
    else if (LABEL_GLOBALVARS == (*ival)->getLabel()) // variables globales
    {
      (*ival)->setObject(new Cglobalvars((*ival)->getAttribute(), (*ival)->getValue(), this, _pSuper)) ;

      _pGlobalVars = (Cglobalvars*) (*ival)->getObject() ;
      if (false == _pGlobalVars->parser())
      {
        // erreur parsing globalvars
        _iParsingError = _pGlobalVars->getParsingError() ;
        return false ;
      }
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

	if ((1 != nb_dialogbox) || (1 != nb_steps))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
	return true ;
}boolCguideline::verif(){
  return true ;
}

bool
Cguideline::compresser()
{
  return true ;
}

bool
Cguideline::traiter()
{
  return true ;
}

string
Cguideline::getName()
{
  return getStringAttribute(ATTRIBUT_ARCHETYPE_NAME) ;
}

string
Cguideline::getFunction()
{
  return getStringAttribute(ATTRIBUT_ARCHETYPE_FUNCT) ;
}

Citem *
Cguideline::getRootItem()
{
  Citem *pRootItem = NULL ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère le père des items de l'archetype
      if (LABEL_ITEMS == (*ival)->getLabel()) // items
      {
        pRootItem = dynamic_cast<Citem *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pRootItem ;
}

Cglobalvars *
Cguideline::getGVars()
{
  return (_pGlobalVars) ;
}

Creferences *
Cguideline::getReference()
{
  Creferences *pRef = NULL ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_REFERENCES == (*ival)->getLabel())
      {
        pRef = dynamic_cast<Creferences *>((*ival)->getObject()) ;
        break ;
      }
    }
  }
  return pRef ;
}

Csteps *
Cguideline::getSteps()
{
  if (vect_val.empty())
    return (Csteps *) 0 ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_STEPS == (*ival)->getLabel())
    {
      Csteps *pSteps = dynamic_cast<Csteps *>((*ival)->getObject()) ;
      return pSteps ;
    }
  }
  return (Csteps *) 0 ;
}

Cdialogbox *
Cguideline::getDialogBox(string sLang)
{
  Cdialogbox  *pDefDialogBox = NULL ;
  Cdialogbox  *pDialogBox    = NULL ;

  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // on récupère l'objet dialogue
      if (LABEL_DIALOGBOX == (*ival)->getLabel())
      {
        pDialogBox = dynamic_cast<Cdialogbox *>((*ival)->getObject()) ;
        if (pDialogBox)
        {
        	if (pDialogBox->getLang() == sLang)
        		break ;
          if (pDialogBox->getLang() == "")
          	pDefDialogBox = pDialogBox ;
        }
      }
    }
  }
  if (pDialogBox && (pDialogBox->getLang() == sLang))
  	return pDialogBox ;
  else
  	return pDefDialogBox ;
}

string
Cguideline::getTitle(string sLang)
{
	// First : look into the references
  //
  Creferences* pRef = getReference() ;
  if (pRef)
  {
  	Chead* pHead = pRef->getHead(sLang) ;
    if (pHead)
    	return pHead->getTitle() ;
  }
  return string("") ;
}

string
Cguideline::getHelpUrl(string sLang)
{
  Creferences *pRef = getReference() ;
  if (NULL == pRef)
    return string("") ;

  return pRef->getHelpUrl(sLang) ;
}

