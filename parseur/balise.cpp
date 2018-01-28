// -----------------------------------------------------------------------------
// balise.cpp
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// $Revision: 1.9 $
// $Author: pameline $
// $Date: 2017/04/08 09:14:30 $
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

#include "balise.h"

#ifndef _OWLDLL
# include "enterpriseLus/nsdivfctForCgi.h"
#else
# include "partage\nsdivfct.h"
#endif

long Cbalise::lObjectCount = 0 ;

Cbalise::Cbalise(string attributs, string values, Cbalise *parentBalise)
        :vect_val(this)
{
  lObjectCount++ ;

  _sAttributs    = attributs ;
  _sValeurs      = values ;

  databaseid     = string("") ;
  parent         = parentBalise ;

  pMainValues    = 0 ;

  _iParsingError = EA_NOERROR ;
}

Cbalise::Cbalise(string attributs, string values, Cconteneur* pMV, Cbalise *parentBalise)
        :vect_val(this)
{
  lObjectCount++ ;

  _sAttributs    = attributs ;
  _sValeurs      = values ;

  databaseid     = string("") ;
  parent         = parentBalise ;

  pMainValues    = pMV ;

  _iParsingError = EA_NOERROR ;
}

Cbalise::Cbalise(const Cbalise& rv)
        :vect_val(rv.vect_val), vect_attr(rv.vect_attr)
{
  lObjectCount++ ;

  _sAttributs    = rv._sAttributs ;
  _sValeurs      = rv._sValeurs ;

  databaseid     = rv.databaseid ;
  parent         = rv.parent ;

  pMainValues    = rv.pMainValues ;

  _iParsingError = rv._iParsingError ;
}

Cbalise&
Cbalise::operator=(const Cbalise& src)
{
	if (this == &src)
		return (*this) ;

  vect_val       = src.vect_val ;
  vect_attr      = src.vect_attr ;

  _sAttributs    = src._sAttributs ;
  _sValeurs      = src._sValeurs ;

  databaseid     = src.databaseid ;
  parent         = src.parent ;

  pMainValues    = src.pMainValues ;

  _iParsingError = src._iParsingError ;

  return (*this) ;
}

// -----------------------------------------------------------------------------
// parsing des attributs
// --
// il y a un oubli dans le code des exceptions pour le ' et la copie pour "
// -----------------------------------------------------------------------------
bool
Cbalise::parser_attributs()
{
  if (string("") == _sAttributs)
    return true ;

  string sCopy = _sAttributs ;

  strip(sCopy, string(" \t")) ;

  if (string("") == sCopy)
    return true ;

  bool bBoucle  = true ;

  while (true == bBoucle)
  {
    size_t counter1 = sCopy.find('=') ;

    if (string::npos == counter1)
    {
      // il n y a pas de signe egal pourtant la string n est pas vide
      // Cannot find any '=' char despite string not being empty
      //
      _iParsingError = EATTR_NOEQUAL ;
      return false ;
    }
    if (0 == counter1)
    {
      // il y a un egal mais en debut de chaine : donc erreur
      // '=' char found, but as the first char, hence missing attribute name
      //
      _iParsingError = EATTR_EQUAL0 ;
      return false ;
    }

    size_t iCopyLen = strlen(sCopy.c_str()) ;

    if (iCopyLen - 1 == counter1)
    {
      // le egal est le dernier caractere de la chaine
      // '=' char found, but as the last char, hence missing attribute value
      //
      _iParsingError = EATTR_EQUALLAST ;
      return false ;
    }

    string sEtiquette = string(sCopy, 0, counter1) ;
    strip(sEtiquette, string(" \t")) ;

    // on regarde le premier caractère suivant le '=' (non espace ni tab)
    // Looking for first char located after the '=' char and no space nor tab
    //
    counter1++ ;
    while ((counter1 < iCopyLen) && ((' '  == sCopy[counter1]) ||
                                     ('\t' == sCopy[counter1])))
      counter1++ ;

    if (counter1 == iCopyLen)
    {
      // il y a un caractere '=' non suivi d'un attribut
      // Only spaces and/or tabs after '=' char -> no attribute value
      //
      _iParsingError = EATTR_NOVALUE ;
      return false ;
    }

    if (('\'' != sCopy[counter1]) && ('\"' != sCopy[counter1]))
    {
      // il y a un caractere '=' non suivi d'un attribut
      // Char '=' is not followed by a ' or a "
      //
      _iParsingError = EATTR_NOVALUE ;
      return false ;
    }

    string sAttribute = getAttributeValue(sCopy, counter1) ;

    // A problem occured
    //
    if (EA_NOERROR != _iParsingError)
      return false ;

    if ((string("") != sEtiquette) && (string("") != sAttribute))
      vect_attr.push_back(new CAttributs(sEtiquette, sAttribute, false)) ;

    strip(sCopy, string(" \t")) ;

    if (string("") == sCopy)
      bBoucle = false ;
  }

  return true ;
}

// iStartChar is the index of the first ' or " enclosing attribute value
//
string
Cbalise::getAttributeValue(string& sCopy, size_t iStartChar)
{
  size_t iCopyLen = strlen(sCopy.c_str()) ;

  // Bad params
  //
  if ((0 == iCopyLen) || (iStartChar >= iCopyLen))
    return string("") ;

  char cSeparator = sCopy[iStartChar] ;

  // Start char as last char, no room left for any value
  //
  if (iCopyLen - 1 == iStartChar)
  {
    _iParsingError = EATTR_DQUOTELAST ;
    return string("") ;
  }

  size_t iEndingSeparator = sCopy.find(cSeparator, iStartChar + 1) ;
  if (string::npos == iEndingSeparator)
  {
    // il n y a pas de ' fermant
    // No closing separator
    //
    _iParsingError = EATTR_QUOTECLOSE ;
    return string("") ;
  }

  string sAttribute = string(sCopy, iStartChar + 1, iEndingSeparator - iStartChar - 1) ;

  sCopy = string(sCopy, iEndingSeparator + 1, iCopyLen - iEndingSeparator - 1) ;

  return sAttribute ;
}

void
Cbalise::addAttribute(string sAttribut, string sText)
{
	vect_attr.push_back(new CAttributs(sAttribut, sText)) ;
}

CAttributs*
Cbalise::getAttributeForValue(string sAttribut)
{
  return getNextAttributeForValue(sAttribut, 0) ;
}

CAttributs*
Cbalise::getNextAttributeForValue(string sAttribut, CAttributs* pPrevious)
{
  if (vect_attr.empty())
    return (CAttributs *) 0 ;

  bool bPreviousFound = false ;

  for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
  {
    if ((*iattribut)->getLabel() == sAttribut)
    {
      if (bPreviousFound || (NULL == pPrevious))
        return *iattribut ;

      if (pPrevious == *iattribut)
        bPreviousFound = true ;
    }
  }

  return (CAttributs *) 0 ;
}

bool
Cbalise::getBooleanAttribute(string sAttribut, bool* bDefaultAndFound)
{
  bool bDefault = *bDefaultAndFound ;

  CAttributs* pAttr = getAttributeForValue(sAttribut) ;
  if ((CAttributs*) NULL == pAttr)
  {
    *bDefaultAndFound = false ;
    return bDefault ;
  }

  while ((CAttributs*) NULL != pAttr)
  {
    string sReponse = pAttr->getValue() ;
    if (string("") != sReponse)
    {
      *bDefaultAndFound = true ;

      return (IsYes(sReponse)) ;
    }
    pAttr = getNextAttributeForValue(sAttribut, pAttr) ;
  }

  *bDefaultAndFound = false ;
  return bDefault ;
}

void
Cbalise::setBooleanAttribute(string sAttribut, bool bValue, bool bAddIfNotThere)
{
  string sText = string("yes") ;
  if (false == bValue)
    sText = string("no") ;

  setStringAttribute(sAttribut, sText, bAddIfNotThere) ;
}

string
Cbalise::getStringAttribute(string sAttribut)
{
  CAttributs* pAttr = getAttributeForValue(sAttribut) ;

  if ((CAttributs*) NULL == pAttr)
    return string("") ;

  return pAttr->getValue() ;
}

string
Cbalise::getUrlStringAttribute(string sAttribut)
{
  CAttributs* pAttr = getAttributeForValue(sAttribut) ;

  if (NULL == pAttr)
    return string("") ;

  return pAttr->getUrlValue() ;
}

void
Cbalise::setStringAttribute(string sAttribut, string sText, bool bAddIfNotThere)
{
  CAttributs* pAttr = getAttributeForValue(sAttribut) ;
  if (NULL != pAttr)
    pAttr->setValue(sText) ;
  else if (true == bAddIfNotThere)
    vect_attr.push_back(new CAttributs(sAttribut, sText)) ;
}

void
Cbalise::setUrlStringAttribute(string sAttribut, string sText, bool bAddIfNotThere)
{
  CAttributs* pAttr = getAttributeForValue(sAttribut) ;
  if (NULL != pAttr)
    pAttr->setUrlValue(sText) ;
  else if (true == bAddIfNotThere)
  {
    CAttributs* pNewAttr = new CAttributs(sAttribut, sText) ;
    pNewAttr->setUrlValue(sText) ;
    vect_attr.push_back(pNewAttr) ;
  }
}

int
Cbalise::getIntAttribute(string sAttribut)
{
  CAttributs* pAttr = getAttributeForValue(sAttribut) ;

  while (NULL != pAttr)
  {
    string sReponse = pAttr->getValue() ;
    if (string("") != sReponse)
    {
      for (size_t i = 0 ; i < strlen(sReponse.c_str()) ; i++)
        if (!isdigit(sReponse[i]))
          return -1 ;

      return atoi(sReponse.c_str()) ;
    }

    pAttr = getNextAttributeForValue(sAttribut, pAttr) ;
  }

  return -1 ;
}

void
Cbalise::setIntAttribute(string sAttribut, int iValue, bool bAddIfNotThere)
{
  // char szValue[35] ;
  // itoa(iValue, szValue, 10) ;

  setStringAttribute(sAttribut, IntToString(iValue), bAddIfNotThere) ;
}

void
Cbalise::cleanValeurs()
{
  if (string("") == _sValeurs)
    return ;

  strip(_sValeurs, " \t\n") ;

/*
  size_t iTaille = strlen(_sValeurs.c_str()) ;
  strip(_sValeurs, stripBoth, '\n') ;
  strip(_sValeurs, stripBoth, ' ') ;
  while (iTaille != strlen(_sValeurs.c_str()))
  {
    iTaille = strlen(_sValeurs.c_str()) ;
    strip(_sValeurs, stripBoth, '\n') ;
    strip(_sValeurs, stripBoth, ' ') ;
  }
*/
}

void
Cbalise::eraseValeur(string sValueLabel)
{
  if (vect_val.empty())
    return ;

  for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
  {
		if (sValueLabel == (*ival)->getLabel())
		{
      delete *ival ;
      vect_val.erase(ival) ;
    }
    else
      ival++ ;
  }
}

void
Cbalise::eraseAttribute(string sAttribut)
{
  if (vect_attr.empty())
    return ;

  for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; )
  {
    if ((*iattribut)->getLabel() == sAttribut)
    {
      delete *iattribut ;
      vect_attr.erase(iattribut) ;
    }
    else
      iattribut++ ;
  }
}

string
Cbalise::toString(int iLevel, string sBalise)
{
  string sDecalage = string(3 * iLevel, ' ') ;

  // Starting tag
  //
  string sOutData = sDecalage + string("<") + sBalise ;

  // Attributs
  if (false == vect_attr.empty())
  {
    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      string sRawValue = (*iattribut)->getRawValue() ;
      if (string("") != sRawValue)
      {
        // Make certain a valid XML is generated
        size_t iIsQuote = sRawValue.find("\"") ;
        size_t iIsLt    = sRawValue.find("<") ;
        size_t iIsGt    = sRawValue.find(">") ;
        if ((string::npos != iIsQuote) || (string::npos != iIsLt) || (string::npos != iIsGt))
          sOutData += string(" ") + (*iattribut)->getRawLabel() + string("=\"") + texteHtml(sRawValue) + string("\"") ;
        else
          sOutData += string(" ") + (*iattribut)->getRawLabel() + string("=\"") + sRawValue + string("\"") ;
      }
    }
  }

  // No content
  //
  if ((vect_val.empty()) && (string("") == _sValeurs))
  {
    sOutData += string("/>") ;
    return sOutData ;
  }

  sOutData += string(">") + NEWLINE ;

  // Valeurs
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
      if ((*ival)->getObject())
        sOutData += (*ival)->getObject()->toString(iLevel+1, (*ival)->getLabel()) + NEWLINE ;
  }
  else
  {
    // Make certain a valid XML is generated
    size_t iIsQuote = _sValeurs.find("\"") ;
    size_t iIsLt    = _sValeurs.find("<") ;
    size_t iIsGt    = _sValeurs.find(">") ;
    if ((string::npos != iIsQuote) || (string::npos != iIsLt) || (string::npos != iIsGt))
      sOutData += string(3 * (iLevel + 1), ' ') + texteHtml(_sValeurs) + NEWLINE ;
    else
      sOutData += string(3 * (iLevel + 1), ' ') + _sValeurs + NEWLINE ;
  }

  // Balise de fin
  sOutData += sDecalage + string("</") + sBalise + string(">") ;

  return sOutData ;
}

void
Cbalise::inscritFichier(ofstream* pOutFile, int iLevel, string sBalise)
{
  string sDecalage = string(3 * iLevel, ' ') ;

  // Balise de début
  string sOutData = sDecalage + string("<") + sBalise ;

  // Attributs
  if (false == vect_attr.empty())
  {
    for (AttrIter iattribut = vect_attr.begin() ; vect_attr.end() != iattribut ; iattribut++)
    {
      string sRawValue = (*iattribut)->getRawValue() ;
      if (string("") != sRawValue)
      {
        // Make certain a valid XML is generated
        size_t iIsQuote = sRawValue.find("\"") ;
        size_t iIsLt    = sRawValue.find("<") ;
        size_t iIsGt    = sRawValue.find(">") ;
        if ((string::npos != iIsQuote) || (string::npos != iIsLt) || (string::npos != iIsGt))
          sOutData += string(" ") + (*iattribut)->getRawLabel() + string("=\"") + texteHtml(sRawValue) + string("\"") ;
        else
          sOutData += string(" ") + (*iattribut)->getRawLabel() + string("=\"") + sRawValue + string("\"") ;
      }
    }
  }

  // Pas de contenu
  if ((vect_val.empty()) && (string("") == _sValeurs))
  {
    sOutData += string("/>") + NEWLINE ;
    pOutFile->write(sOutData.c_str(), strlen(sOutData.c_str())) ;
    return ;
  }

  sOutData += string(">") + NEWLINE ;
  pOutFile->write(sOutData.c_str(), strlen(sOutData.c_str())) ;

  // Valeurs
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getObject())
        (*ival)->getObject()->inscritFichier(pOutFile, iLevel+1, (*ival)->getLabel()) ;
    }
  }
  else
  {
    // Make certain a valid XML is generated
    size_t iIsQuote = _sValeurs.find("\"") ;
    size_t iIsLt    = _sValeurs.find("<") ;
    size_t iIsGt    = _sValeurs.find(">") ;
    if ((string::npos != iIsQuote) || (string::npos != iIsLt) || (string::npos != iIsGt))
      sOutData = string(3 * (iLevel + 1), ' ') + texteHtml(_sValeurs) + NEWLINE ;
    else
      sOutData = string(3 * (iLevel + 1), ' ') + _sValeurs + NEWLINE ;
    pOutFile->write(sOutData.c_str(), strlen(sOutData.c_str())) ;
  }

  // Balise de fin
  sOutData = sDecalage + string("</") + sBalise + string(">") + NEWLINE ;
  pOutFile->write(sOutData.c_str(), strlen(sOutData.c_str())) ;
}

// -----------------------------------------------------------------------------
// parser_valeurs
// -----------------------------------------------------------------------------
bool
Cbalise::parser_valeurs()
{
  cleanValeurs() ;

  if (string("") == _sValeurs)
    return true ;

	size_t tag_start    = _sValeurs.find('<') ;
	size_t last_tag_end = _sValeurs.rfind('>') ;

  // No tag in string
  //
	if (tag_start == string::npos && last_tag_end == string::npos)
		return true ;

  // No endig tag in string
  //
	if (string::npos == last_tag_end)
  {
    _iParsingError = EVAL_NOCLOSE ;
    return false ;
  }

	string sXmlString = string(_sValeurs, 0, last_tag_end + 1) ;
  while (string("") != sXmlString)
	{
    tag_start = sXmlString.find('<') ;
    if (string::npos == tag_start)
    {
      // il manque un caractere de balise ouvrante : a verifier ici pourquoi on ne renvoi pas true ?!
      _iParsingError = EVAL_NOOPEN ;
      return false ;
    }

    if (tag_start > 0)
      sXmlString = string(sXmlString, tag_start, strlen(sXmlString.c_str()) - tag_start) ;

    size_t attributes_end = 0 ;

    size_t cc = 0 ;

    // Looking for the last '>', jumping above every '' or "" block
    //
    bool continuer = true ;
    while (continuer)
    {
      size_t iXmlLen = strlen(sXmlString.c_str()) ;

      while ((cc < iXmlLen) && ('\'' != sXmlString[cc]) &&
                               ('\"' != sXmlString[cc]) &&
                               ('>'  != sXmlString[cc]))
        cc++ ;

      if (cc == iXmlLen)
      {
        // il manque un caractere de balise fermante ('<' sans '>')
        _iParsingError = EVAL_NOOPEN ;
        return false ;
      }

      switch(sXmlString[cc])
      {
        case '\'' :
          cc = sXmlString.find("\'", cc + 1) ;
          if (string::npos == cc)
          {
            _iParsingError = EATTR_QUOTECLOSE ;
            return false ;
          }
          cc++ ;
          break ;
        case '\"' :
          cc = sXmlString.find("\"", cc + 1) ;
          if (string::npos == cc)
          {
            _iParsingError = EATTR_DQUOTECLOSE ;
            return false ;
          }
          cc++ ;
          break ;
        case '>' :
          attributes_end = cc ;
          continuer = false ;
          break ;
      }
    }

    // Is it a comment?
    //
    if ((attributes_end > 3) && (string(sXmlString, 0, 4) == string("<!--")))
    {
      attributes_end = sXmlString.find("-->") ;
      if (attributes_end == string::npos)
      {
        // il manque la balise de fin de commentaire
        _iParsingError = EVAL_CLOSEBALISE ;
        return false ;
      }
      if (attributes_end + 3 < strlen(sXmlString.c_str()))
        sXmlString = string(sXmlString, attributes_end + 3, strlen(sXmlString.c_str()) - attributes_end - 3) ;
      else
        sXmlString = string("") ;
    }
    else
    {
      bool bTest = true ;

      if ('/' == sXmlString[attributes_end - 1])
      {
        attributes_end-- ;
        bTest = false ;
      }

      size_t label_end = sXmlString.find(' ') ;
      if ((string::npos == label_end) || (label_end > attributes_end))
        label_end = attributes_end ;

      if (1 == label_end)
      {
        // il y a un espace juste apres le debut de balise
        _iParsingError = EVAL_SPACE ;
        return false ;
      }

      string sLabel      = string(sXmlString, 1, label_end - 1) ;
      string sAttributes = string("") ;

      if (attributes_end != label_end)
      {
        sAttributes = string(sXmlString, label_end + 1, attributes_end - label_end - 1) ;
        strip(sAttributes, string(" \t")) ;
      }

      string sValues = string("") ;

      size_t values_end = 0 ;
      if ('/' != sXmlString[attributes_end])
      // c est ici qu il faut rajouter le code de test des balises ouvrantes et fermantes
      {
        size_t count  = 1 ;
        size_t pos    = attributes_end ;
        size_t iClose = 0 ;

        while ((0 != count) && (true == bTest))
        {
          size_t open1 = sXmlString.find("<" + sLabel + " ", pos) ;
          size_t open2 = sXmlString.find("<" + sLabel + ">", pos) ;

          size_t open ;
          if      (string::npos == open1)
            open = open2 ;
          else if (string::npos == open2)
            open = open1 ;
          else
            open = min(open1, open2) ;

          string sClosingLabel = string("</") + sLabel + string(">") ;

          iClose = sXmlString.find(sClosingLabel, pos) ;
          if (string::npos == iClose)
          {
            _iParsingError = EVAL_CLOSE ;
            bTest = false ;
          }
          else if ((iClose < open) || (string::npos == open))
          {
            // la premiere balise trouvee est une balise fermante
            count-- ;
            pos = iClose + 1 ;
          }
          else
          {
            // la premiere balise trouvee est une balise ouvrante
            // il y a deux possibilites :
            //   - c'est une balise ouvrante-fermante
            //   - c'est une balise ouvrante
            size_t end_tag = sXmlString.find(">", open + 1) ;
            if (string::npos == end_tag)
            {
              _iParsingError = EVAL_CLOSEBALISE ;
              bTest = false ;
            }
            else
            {
              if ('/' != sXmlString[end_tag-1])
                count++ ;
              pos = open + 1 ;
            }
          }
        }

        if (false == bTest)
          return false ;

        values_end = iClose ;
        if (attributes_end > values_end)
        {
          _iParsingError = EVAL_DEF ;
          return false ;
        }

        sValues = string(sXmlString, attributes_end + 1, values_end - attributes_end - 1) ;
      }

#if !defined _OWLDLL
  // cerr << "label : " << label << endl ;
  // cerr << "attributes : " << attributes << endl ;
  // cerr << "values : " << values <<endl ;
#endif

      vect_val.push_back(new CValeur(sLabel, sValues, sAttributes, this)) ;

      if (sXmlString[attributes_end] == '/')
        sXmlString = string(sXmlString, attributes_end + 2, strlen(sXmlString.c_str()) - attributes_end - 2) ;
      else
        sXmlString = string(sXmlString, values_end + strlen(sLabel.c_str()) + 3, strlen(sXmlString.c_str()) - values_end - strlen(sLabel.c_str()) - 3) ;
      strip(sXmlString, string(" \t")) ;
    }
  }
	return true ;
}

