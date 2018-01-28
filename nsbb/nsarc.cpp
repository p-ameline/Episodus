// -----------------------------------------------------------------------------
// nsarc.cpp
// -----------------------------------------------------------------------------
// Parseur d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.14 $
// $Author: pameline $
// $Date: 2014/10/04 13:14:27 $
// -----------------------------------------------------------------------------
// FLP - april 2005
// RS  - november 2002
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

#include "nsbb\nsarc.h"
#include "partage\nsdivfct.h"
#include "partage\ns_search.h"
#include "pre_parseur.h"
#include "nsbb\nsarcParseError.h"
#include "nsbb\nsbb.h"
#include "nsbb\nspatpat.h"
#include "nsbb\nsbbtran.h"
#include "nssavoir\nsfilgd.h"
#include "nssavoir\nscaptur.h"

#include "ns_sgbd\nsfiche.h"
#include "nssavoir\nsconver.h"

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nautilus\nssuper.h"
#endif

#include "ns_ob1\NautilusType.h"
#include "ns_ob1\Interface.h"

long nsGenericParseur::lGenericObjectCount = 0 ;
long nsarcParseur::lObjectCount = 0 ;
long nsrefParseur::lObjectCount = 0 ;
long nsGuidelineParseur::lObjectCount = 0 ;
long NSValidateur::lObjectCount = 0 ;
long NSDrufRefFileManager::lObjectCount = 0 ;

string
TexteXmlVersNormal(const string texte)
{
  string  copie = texte ;
  bool    boucle = true ;
  size_t  car_start, car_end ;
  size_t  pos = 0 ;

  while ((boucle == true) && (pos < strlen(copie.c_str())))
  {
    car_start = copie.find("&", pos) ;
    if (car_start == string::npos)
      boucle = false ;
    else
    {
      car_end = copie.find(";", car_start) ;
      if (car_end == string::npos)
        // texte mal formaté : on s'arrête
        boucle = false ;
      else
      {
        string caract = string(copie, car_start, car_end - car_start + 1) ;

        if (caract == string("&acirc;"))
          copie.replace(car_start, caract.length(), "â") ;
        else if (caract == string("&agrave;"))
          copie.replace(car_start, caract.length(), "à") ;
        else if (caract == string("&ccedil;"))
          copie.replace(car_start, caract.length(), "ç") ;
        else if (caract == string("&eacute;"))
          copie.replace(car_start, caract.length(), "é") ;
        else if (caract == string("&ecirc;"))
          copie.replace(car_start, caract.length(), "ê") ;
        else if (caract == string("&egrave;"))
          copie.replace(car_start, caract.length(), "è") ;
        else if (caract == string("&euml;"))
          copie.replace(car_start, caract.length(), "ë") ;
        else if (caract == string("&icirc;"))
          copie.replace(car_start, caract.length(), "î") ;
        else if (caract == string("&iuml;"))
          copie.replace(car_start, caract.length(), "ï") ;
        else if (caract == string("&ocirc;"))
          copie.replace(car_start, caract.length(), "ô") ;
        else if (caract == string("&ouml;"))
          copie.replace(car_start, caract.length(), "ö") ;
        else if (caract == string("&lt;"))
          copie.replace(car_start, caract.length(), "<") ;
        else if (caract == string("&gt;"))
          copie.replace(car_start, caract.length(), ">") ;
        else if (caract == string("&ramp;"))
          copie.replace(car_start, caract.length(), "&") ;
        else if (caract == string("&quot;"))
          copie.replace(car_start, caract.length(), "\"") ;
        else if (caract == string("&#124;"))
          copie.replace(car_start, caract.length(), "|") ;
        else if (caract == string("&#163;"))
          copie.replace(car_start, caract.length(), "£") ;
        else if (caract == string("&#36;"))
          copie.replace(car_start, caract.length(), "$") ;

        // on repart après le dernier caractère remplacé
        pos = car_start + 1 ;
      }
    }
  }

  return copie ;
}

// -----------------------------------------------------------------------------
//
// classe nsGenericParseur
//
// -----------------------------------------------------------------------------

nsGenericParseur::nsGenericParseur(NSSuper* pSuper, bool bVerb)
{
  lGenericObjectCount++ ;

  _pRootBalise = (Cbalise*) 0 ;
  _pSuper      = pSuper ;
  _bParsingOk  = false ;
  _bVerbose    = bVerb ;
}

nsGenericParseur::~nsGenericParseur()
{
  lGenericObjectCount-- ;

  if (_pRootBalise)
    delete _pRootBalise ;
}

// -----------------------------------------------------------------------------
//
// classe nsarcParseur
//
// -----------------------------------------------------------------------------

nsarcParseur::nsarcParseur(NSSuper* pSuper, bool bVerb)
             :nsGenericParseur(pSuper, bVerb)
{
  lObjectCount++ ;
}

nsarcParseur::~nsarcParseur()
{
  lObjectCount-- ;
}

nsarcParseur::nsarcParseur(const nsarcParseur& rv)
             :nsGenericParseur(rv._pSuper, rv._bVerbose)
{
  lObjectCount++ ;

  if (rv._pRootBalise)
    _pRootBalise = new Carchetype(*((Carchetype*) rv._pRootBalise)) ;
  else
    _pRootBalise = (Carchetype*) 0 ;

  _bParsingOk = rv._bParsingOk ;
}

nsarcParseur&
nsarcParseur::operator=(const nsarcParseur& src)
{
	if (this == &src)
		return *this ;

	if (_pRootBalise)
		delete _pRootBalise ;

  if (src._pRootBalise)
    _pRootBalise = new Carchetype(*((Carchetype*) src._pRootBalise)) ;
  else
    _pRootBalise = (Carchetype*) 0 ;

  _pSuper     = src._pSuper ;
  _bParsingOk = src._bParsingOk ;
  _bVerbose   = src._bVerbose ;

  return (*this) ;
}

bool
nsarcParseur::open(const string sXmlFileName)
{
  ifstream xml_file ;
	xml_file.open(sXmlFileName.c_str()) ;
  if (!xml_file)
  {
  	string sErrorText = _pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sXmlFileName ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

	string xml_string = string("") ;
  string tmp_xml_string ;
	while (getline(xml_file, tmp_xml_string))
		xml_string += tmp_xml_string + '\n' ;
	xml_file.close() ;

  // xml_string = TexteXmlVersNormal(xml_string) ;

	bool reponse = pre_parseur(&xml_string) ;
	if (false == reponse)
  {
    string sErrorText = _pSuper->getText("archetypesManagement", "archetypePreParsingError") ;
    sErrorText += string(" (") + sXmlFileName + string(")") ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;

		return false ;
  }

  int iParsingError ;
	if (parse_archetype(xml_string, string("archetype"), &iParsingError) == false)
  {
    if (_bVerbose)
    {
    	string sErrParse = parseErrorMessage(iParsingError) ;

    	string sErrorText = _pSuper->getText("archetypesManagement", "archetypeParsingError") ;
    	sErrorText += string(" ") + sErrParse ;
    	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
    }
    return false ;
  }

  _bParsingOk = true ;
  return true ;
}

bool
nsarcParseur::parse_archetype(string arc, string tag, int* iParsingError)
{
try
{
  string attributes, values ;

  size_t arc_start = arc.find ("<" + tag) ;
  if (arc_start != string::npos)
  {
    size_t arc_end = arc.find ("</" + tag + ">") ;
    if (arc_end == string::npos)
    {
      // erreur (pas de balise de fin)
      *iParsingError = EARCHETYPE_BALISE_FIN ;
      return false ;
    }

    size_t header_end = arc.find (">", arc_start) ;
    if (arc_end < header_end)
    {
      // erreur (pas de fin de balise de debut)
      *iParsingError = EARCHETYPE_BALISE_DEBUT_FERMANTE ;
      return false ;
    }

    // attrib_start indique soit header_end('>') soit le premier attribut
    size_t attrib_start = arc.find_first_not_of(' ', arc_start + tag.length() + 1) ;

    attributes = string (arc, attrib_start, header_end - attrib_start) ;
    values = string (arc, header_end + 1, arc_end - header_end - 1) ;
  }
  else
  {
    // erreur (pas de balise de debut)
    *iParsingError = EARCHETYPE_BALISE_DEBUT ;
    return false ;
  }

  _pRootBalise = new Carchetype(attributes, values, NULL, _pSuper) ;

  Carchetype* pArchetype = (Carchetype*) _pRootBalise ;
  if (false == pArchetype->parser())
  {
    *iParsingError = pArchetype->getParsingError() ;
    return false ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception nsarcParseur::parse_archetype.", standardError) ;
  return false ;
}
}

bool
nsarcParseur::createArchetypedPpt(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo)
{
  if (((NSPatPathoArray*) NULL == pPPT) ||
      ((NSCaptureArray*)  NULL == pCapturedInfo) ||
      ((Carchetype*)      NULL == _pRootBalise))
    return false ;

  Citem* pRootItem = getArchetype()->getRootItem() ;
  if ((Citem*) NULL == pRootItem)
    return false ;

  Valeur_array *pValArray = pRootItem->getArrayFils() ;
  if (((Valeur_array*) NULL == pValArray) || (pValArray->empty()))
    return true ;

  // On récupère les fils de premier niveau
	for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)	{
    // Item
		if (LABEL_ITEM == (*ival)->getLabel())		{			Citem* pSonItem = dynamic_cast<Citem *>((*ival)->getObject()) ;      if ((Citem*) NULL == pSonItem)        return false ;

      string sDecal = pSonItem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;
      string sPath  = string("") ;
			if ((string("") == sDecal) || (string("+01+01") == sDecal))      {        string sEtiquette = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;
        if (string("") != sEtiquette)
        {
          if (string("") != sPath)
            sPath += string(1, cheminSeparationMARK) ;
          sPath += sEtiquette ;
        }
      }

      // return fillSonNodes(pPPT, pCapturedInfo, pSonItem, sPath) ;
      fillSonNodes(pPPT, pCapturedInfo, pSonItem, sPath) ;
    }
  }

  return true ;
}

bool
nsarcParseur::updateArchetypedPpt(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo)
{
  if (((NSPatPathoArray*) NULL == pPPT) ||
      ((NSCaptureArray*)  NULL == pCapturedInfo) ||
      ((Carchetype*)      NULL == _pRootBalise))
    return false ;

  // We cannot build the patpatho with createArchetypedPpt and compare it with
  // the current one because new information could have been added. We must
  // only check if information from pCapturedInfo are updating the patpatho
  //

  Citem* pRootItem = getArchetype()->getRootItem() ;
  if ((Citem*) NULL == pRootItem)
    return false ;

  Valeur_array *pValArray = pRootItem->getArrayFils() ;
  if (((Valeur_array*) NULL == pValArray) || (pValArray->empty()))
    return true ;

  // On récupère les fils de premier niveau
	for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)	{
    // Item
		if (LABEL_ITEM == (*ival)->getLabel())		{			Citem* pSonItem = dynamic_cast<Citem *>((*ival)->getObject()) ;      if ((Citem*) NULL == pSonItem)        return false ;

      string sDecal = pSonItem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;
      string sPath  = string("") ;
			if ((string("") == sDecal) || (string("+01+01") == sDecal))      {        string sEtiquette = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;
        if (string("") != sEtiquette)
        {
          if (string("") != sPath)
            sPath += string(1, cheminSeparationMARK) ;
          sPath += sEtiquette ;
        }
      }

      // return fillSonNodes(pPPT, pCapturedInfo, pSonItem, sPath) ;
      updateSonNodes(pPPT, pCapturedInfo, pSonItem, sPath) ;
    }
  }

  return true ;
}

bool
nsarcParseur::fillSonNodes(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Citem* pFather, string sFatherPath)
{
#ifndef _ENTERPRISE_DLL
  if (((NSPatPathoArray*) NULL == pPPT) ||
      ((NSCaptureArray*)  NULL == pCapturedInfo) ||
      ((Citem*)           NULL == pFather))
    return false ;

  Valeur_array *pValArray = pFather->getArrayFils() ;
  if (((Valeur_array*) NULL == pValArray) || (pValArray->empty()))
    return true ;

  // On récupère les fils de premier niveau
	for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)	{
    // Item
		if (LABEL_ITEM == (*ival)->getLabel())		{			Citem* pSonItem = dynamic_cast<Citem *>((*ival)->getObject()) ;      if ((Citem*) NULL == pSonItem)        return false ;      string sPath = sFatherPath ;			// Récupération de l'étiquette attachée au Citem			string sEtiquette = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;      string sDecal     = pSonItem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;			if ((string("") == sDecal) || (string("+01+01") == sDecal))      {        // Check if there is some captured information for this node        //        string sCapturedString = string("") ;        NSCapture* pCaptureEntry = pCapturedInfo->trouveCheminExact(sPath, &sCapturedString) ;        // There is a captured string corresponding to this path, we add it        // to the tree        //        if (pCaptureEntry)          addCapturedNode(pCaptureEntry, pSonItem, pPPT, sPath) ;        // Extend node's path        //        if (string("") != sEtiquette)        {
          if (string("") != sPath)
            sPath += string(1, cheminSeparationMARK) ;
          sPath += sEtiquette ;
        }      }      if (false == fillSonNodes(pPPT, pCapturedInfo, pSonItem, sPath))        return false ;    }  }
#endif
  return true ;
}

void
nsarcParseur::addCapturedNode(NSCapture* pCaptureEntry, Citem* pSonItem, NSPatPathoArray* pPPT, string sPath)
{
  if (((NSCapture*)       NULL == pCaptureEntry) ||
      ((Citem*)           NULL == pSonItem) ||
      ((NSPatPathoArray*) NULL == pPPT))
    return ;

  string sEtiquette      = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;
  string sCapturedString = pCaptureEntry->getLibelle() ;

  if      ('£' == sEtiquette[0])
  {
    Message Msg ;
    Msg.SetLexique(sEtiquette) ;
    Msg.SetTexteLibre(sCapturedString) ;
    pPPT->AjouterChemin(sPath, &Msg, true, string(1, cheminSeparationMARK)) ;
  }
  else if ('2' == sEtiquette[0])
  {
    size_t iChemMark = sEtiquette.find(cheminSeparationMARK) ;
    if (string::npos != iChemMark)
    {
      string sUnit   = string(sEtiquette, 0, iChemMark) ;
      string sLexiq  = string(sEtiquette, iChemMark + 1, strlen(sEtiquette.c_str()) - iChemMark - 1) ;

      string sValeur = sCapturedString ;

      // Conversion d'unités
      if ((string("") != pCaptureEntry->getUnit()) && (sUnit != pCaptureEntry->getUnit()))
      {
        NSCV NsCv(_pSuper) ;
#ifndef _ENTERPRISE_DLL
        DBIResult Resultat = NsCv.open() ;
        if (Resultat != DBIERR_NONE)
        {
          erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat) ;
          sValeur = string("") ;
        }
        else
        {
#endif
          double dVal = StringToDouble(sValeur) ;
          if (NsCv.ConvertirUnite(&dVal, sUnit, pCaptureEntry->getUnit()))
            sValeur = DoubleToString(&dVal, 0, 2) ;
          else
            sValeur = string("") ;

#ifndef _ENTERPRISE_DLL
          NsCv.close();
        }
#endif
      }

      if (string("") != sValeur)
      {
        Message Msg ;
        Msg.SetLexique(sLexiq) ;
        Msg.SetUnit(sUnit) ;
        Msg.SetComplement(sValeur) ;
        pPPT->AjouterChemin(sPath, &Msg, true, string(1, cheminSeparationMARK)) ;
      }
    }
  }
  else
  {
    string sLabel ;
    NSDico::donneCodeSens(&sCapturedString, &sLabel) ;
    string sEtiqSens ;
    NSDico::donneCodeSens(&sEtiquette, &sEtiqSens) ;

    if (sEtiqSens == sLabel)
    {
      Message Msg ;
      Msg.SetLexique(sEtiquette) ;
      pPPT->AjouterChemin(sPath, &Msg, true, string(1, cheminSeparationMARK)) ;
    }
  }
}

bool
nsarcParseur::updateSonNodes(NSPatPathoArray* pPPT, NSCaptureArray* pCapturedInfo, Citem* pFather, string sFatherPath)
{
#ifndef _ENTERPRISE_DLL
  if (((NSPatPathoArray*) NULL == pPPT) ||
      ((NSCaptureArray*)  NULL == pCapturedInfo) ||
      ((Citem*)           NULL == pFather))
    return false ;

  Valeur_array *pValArray = pFather->getArrayFils() ;
  if (((Valeur_array*) NULL == pValArray) || (pValArray->empty()))
    return true ;

  // On récupère les fils de premier niveau
	for (ValIter ival = pValArray->begin() ; pValArray->end() != ival ; ival++)	{
    // Item
		if (LABEL_ITEM == (*ival)->getLabel())		{			Citem* pSonItem = dynamic_cast<Citem *>((*ival)->getObject()) ;      if ((Citem*) NULL == pSonItem)        return false ;      string sPath = sFatherPath ;			// Récupération de l'étiquette attachée au Citem			string sEtiquette = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;      string sDecal     = pSonItem->getStringAttribute(ATTRIBUT_ITEM_DECAL) ;			if ((string("") == sDecal) || (string("+01+01") == sDecal))      {        // Check if there is some captured information for this node        //        string sCapturedString = string("") ;        NSCapture* pCaptureEntry = pCapturedInfo->trouveCheminExact(sPath, &sCapturedString) ;        // There is a captured string corresponding to this path        // we must add it if not in the tree or update it if the path already        // exists in the tree        //        if (pCaptureEntry)        {          // Does this path already exist in the tree?          //          bool bIsPathInTree = pPPT->CheminDansPatpatho(sPath) ;          // Not there: add it          //          if (false == bIsPathInTree)            addCapturedNode(pCaptureEntry, pSonItem, pPPT, sPath) ;          // Already there: update it          //          else            updateCapturedNode(pCaptureEntry, pSonItem, pPPT, sPath) ;        }        // Extend node's path        //        if (string("") != sEtiquette)        {
          if (string("") != sPath)
            sPath += string(1, cheminSeparationMARK) ;
          sPath += sEtiquette ;
        }      }      if (false == updateSonNodes(pPPT, pCapturedInfo, pSonItem, sPath))        return false ;    }  }
#endif

  return true ;
}

void
nsarcParseur::updateCapturedNode(NSCapture* pCaptureEntry, Citem* pSonItem, NSPatPathoArray* pPPT, string sPath)
{
  if (((NSCapture*)        NULL == pCaptureEntry) ||
      ((Citem*)            NULL == pSonItem) ||
       ((NSPatPathoArray*) NULL == pPPT))
    return ;

  string sEtiquette      = pSonItem->getStringAttribute(ATTRIBUT_ITEM_CODE) ;
  string sCapturedString = pCaptureEntry->getLibelle() ;

  PatPathoIter pPptIter ;

  bool bIsPathInTree = pPPT->CheminDansPatpatho(sPath, string(1, cheminSeparationMARK), &pPptIter) ;
  if ((false == bIsPathInTree) || (NULL == pPptIter) || (pPPT->end() == pPptIter))
    return ;

  // Going to the next element, the one to edit
  //
  pPptIter++ ;
  if (pPPT->end() == pPptIter)
    return ;

  if      ('£' == sEtiquette[0])
    (*pPptIter)->setTexteLibre(sCapturedString) ;

  else if ('2' == sEtiquette[0])
  {
    size_t iChemMark = sEtiquette.find(cheminSeparationMARK) ;
    if (string::npos != iChemMark)
    {
      string sUnit   = string(sEtiquette, 0, iChemMark) ;
      string sLexiq  = string(sEtiquette, iChemMark + 1, strlen(sEtiquette.c_str()) - iChemMark - 1) ;

      string sValeur = sCapturedString ;

      // Conversion d'unités
      if ((string("") != pCaptureEntry->getUnit()) && (sUnit != pCaptureEntry->getUnit()))
      {
        NSCV NsCv(_pSuper) ;
#ifndef _ENTERPRISE_DLL
        DBIResult Resultat = NsCv.open() ;
        if (Resultat != DBIERR_NONE)
        {
          erreur("Erreur à l'ouverture de la base convert.", standardError, Resultat) ;
          sValeur = string("") ;
        }
        else
        {
#endif
          double dVal = StringToDouble(sValeur) ;
          if (NsCv.ConvertirUnite(&dVal, sUnit, pCaptureEntry->getUnit()))
            sValeur = DoubleToString(&dVal, 0, 2) ;
          else
            sValeur = string("") ;

#ifndef _ENTERPRISE_DLL
          NsCv.close() ;
        }
#endif
      }

      if (string("") != sValeur)
        (*pPptIter)->setComplement(sValeur) ;
    }
  }
  else
  {
    string sLabel ;
    NSDico::donneCodeSens(&sCapturedString, &sLabel) ;
    string sEtiqSens = (*pPptIter)->getLexiqueSens() ;

    if (sEtiqSens != sLabel)
      (*pPptIter)->setLexique(sCapturedString) ;
  }
}

// -----------------------------------------------------------------------------
//
// classe nsrefParseur
//
// -----------------------------------------------------------------------------

nsrefParseur::nsrefParseur(NSSuper* pSuper, bool bVerb)
             :nsGenericParseur(pSuper, bVerb)
{
  lObjectCount++ ;
}

nsrefParseur::~nsrefParseur()
{
  lObjectCount-- ;

  Creferentiel *pCRef = dynamic_cast<Creferentiel*>(_pRootBalise) ;
  if (pCRef)
  {
    delete pCRef ;
    _pRootBalise = (Cbalise*) 0 ;
  }
}

bool
nsrefParseur::open(string sXmlFileName)
{
  ifstream  xml_file ;
  xml_file.open(sXmlFileName.c_str()) ;
  if (!xml_file)
  {
  	string sErrorText = _pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sXmlFileName ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  string xml_string = string("") ;
  string tmp_xml_string ;
  while (getline(xml_file, tmp_xml_string))
    xml_string += tmp_xml_string + '\n' ;
  xml_file.close() ;

  // xml_string = TexteXmlVersNormal(xml_string) ;

  bool reponse = pre_parseur(&xml_string) ;
  if (reponse == false)
    return false ;

  int iParsingError ;
	if (parse_referentiel(xml_string, LABEL_REFERENTIEL, &iParsingError) == false)
  {
    if (_bVerbose)
    {
      string sErrParse = parseErrorMessage(iParsingError) ;

      string sErrorText = _pSuper->getText("referentialErrors", "referentialParsingError") ;
    	sErrorText += string(" ") + sErrParse ;
    	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
    }
    return false ;
  }
  _bParsingOk = true ;
  return true ;
}

bool
nsrefParseur::parse_referentiel(string ref, string tag, int* iParsingError)
{
try
{
  size_t ref_start, ref_end, header_end, attrib_start ;
  string attributes, values ;

  ref_start = ref.find("<" + tag) ;
  if (ref_start != string::npos)
  {
    ref_end = ref.find("</" + tag + ">") ;
    if (ref_end == string::npos)
    {
      // erreur (pas de balise de fin)
      *iParsingError = EREFERENTIEL_BALISE_FIN ;
      return false ;
    }

    header_end = ref.find(">", ref_start) ;
    if (ref_end < header_end)
    {
      // erreur (pas de fin de balise de debut)
      *iParsingError = EREFERENTIEL_BALISE_DEBUT_FERMANTE ;
      return false ;
    }

    // attrib_start indique soit header_end('>') soit le premier attribut
    attrib_start = ref.find_first_not_of(' ', ref_start + tag.length() + 1) ;
    attributes = string(ref, attrib_start, header_end - attrib_start) ;
    values = string(ref, header_end + 1, ref_end - header_end - 1) ;
  }
  else
  {
    // erreur (pas de balise de debut)
    *iParsingError = EREFERENTIEL_BALISE_DEBUT ;
    return false ;
  }

  _pRootBalise = new Creferentiel(attributes, values, NULL, _pSuper) ;

  if (false == getReferentiel()->parser())
  {
    *iParsingError = getReferentiel()->getParsingError() ;
    return false ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception nsrefParseur::parse_referentiel.", standardError) ;
  return false ;
}
}

void
nsrefParseur::getGroups(VecteurString* pGroups, string sLang)
{
  if ((NULL == pGroups) || (NULL == getReferentiel()))
    return ;

  Creferentiel* pReferentiel = getReferentiel() ;

  for (ValIter ival = pReferentiel->vect_val.begin() ; pReferentiel->vect_val.end() != ival ; ival++)
  {
    // on récupère l'objet dialogue
    if (LABEL_PROPOSITION == (*ival)->getLabel())
    {
      Cproposition *pCprop = dynamic_cast<Cproposition *>((*ival)->getObject()) ;
      if (pCprop)
      {
        string sGroup = pCprop->getGroup(sLang) ;
        if ((string("") != sGroup) && (false == pGroups->ItemDansUnVecteur(sGroup)))
          pGroups->AddString(sGroup) ;
      }
    }
  }
}

// -----------------------------------------------------------------------------
//
// classe nsGuidelineParseur
//
// -----------------------------------------------------------------------------

nsGuidelineParseur::nsGuidelineParseur(NSSuper* pSuper, bool bVerb)
                      :nsGenericParseur(pSuper, bVerb)
{
  lObjectCount++ ;
}

nsGuidelineParseur::~nsGuidelineParseur()
{
  lObjectCount-- ;
}

bool
nsGuidelineParseur::open(string sXmlFileName)
{
  ifstream  xml_file ;
  xml_file.open(sXmlFileName.c_str()) ;
  if (!xml_file)
  {
  	string sErrorText = _pSuper->getText("fileErrors", "errorOpeningInputFile") ;
    sErrorText += string(" ") + sXmlFileName ;
    _pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
    return false ;
  }

  string xml_string = string("") ;
  string tmp_xml_string ;
  while (getline(xml_file, tmp_xml_string))
    xml_string += tmp_xml_string + '\n' ;
  xml_file.close() ;

  // xml_string = TexteXmlVersNormal(xml_string) ;

  bool reponse = pre_parseur(&xml_string) ;
  if (reponse == false)
    return false ;

  int iParsingError ;
	if (parse_guideline(xml_string, LABEL_GUIDELINE, &iParsingError) == false)
  {
    if (_bVerbose)
    {
      string sErrParse = parseErrorMessage(iParsingError) ;

      string sErrorText = _pSuper->getText("referentialErrors", "referentialParsingError") ;
    	sErrorText += string(" ") + sErrParse ;
    	_pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
    }
    return false ;
  }
  _bParsingOk = true ;
  return true ;
}

bool
nsGuidelineParseur::parse_guideline(string ref, string tag, int* iParsingError)
{
try
{
  size_t ref_start, ref_end, header_end, attrib_start ;
  string attributes, values ;

  ref_start = ref.find("<" + tag) ;
  if (ref_start != string::npos)
  {
    ref_end = ref.find("</" + tag + ">") ;
    if (ref_end == string::npos)
    {
      // erreur (pas de balise de fin)
      *iParsingError = EREFERENTIEL_BALISE_FIN ;
      return false ;
    }

    header_end = ref.find(">", ref_start) ;
    if (ref_end < header_end)
    {
      // erreur (pas de fin de balise de debut)
      *iParsingError = EREFERENTIEL_BALISE_DEBUT_FERMANTE ;
      return false ;
    }

    // attrib_start indique soit header_end('>') soit le premier attribut
    attrib_start = ref.find_first_not_of(' ', ref_start + tag.length() + 1) ;
    attributes = string(ref, attrib_start, header_end - attrib_start) ;
    values = string(ref, header_end + 1, ref_end - header_end - 1) ;
  }
  else
  {
    // erreur (pas de balise de debut)
    *iParsingError = EREFERENTIEL_BALISE_DEBUT ;
    return false ;
  }

  _pRootBalise = new Cguideline(attributes, values, NULL, _pSuper) ;

  Cguideline *pGuideline = getGuideline() ;
  if (false == pGuideline->parser())
  {
    *iParsingError = pGuideline->getParsingError() ;
    return false ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception nsGuidelineParseur::parse_decision_tree.", standardError) ;
  return false ;
}
}

// -----------------------------------------------------------------------------
//
// Classe NSValidateur
//
// -----------------------------------------------------------------------------

NSValidateur::NSValidateur(Ccontrainte* pValid, NSContexte* pCtx, NSPatPathoArray* pLocPat)
{
  lObjectCount++ ;

  _pValidity   = pValid ;
  _pContexte   = pCtx ;
  _pLocalPatho = pLocPat ;
  _pGlobalVars = (Cglobalvars *) 0 ;
}

NSValidateur::~NSValidateur()
{
  lObjectCount-- ;
  // on ne detruit rien car la contrainte de validité appartient au parseur
}

bool
NSValidateur::getValeurVariable(string sName, string& sAlias, NSPatPathoArray** ppPatPathoArray)
{
	sAlias = string("") ;

  // At first, we process local variables (those that are defined inside the validator
  //
  // The point here is that if they have been defined specifically, it
  // means that they deserve to be processed specifically too
  //
  if (false == _pValidity->getValArray()->empty())
  {
    for (ValIter ival = _pValidity->getValArray()->begin() ; _pValidity->getValArray()->end() != ival ; ival++)
    {      if (LABEL_VARIABLE == (*ival)->getLabel())      {        Cvariable* pCvar = dynamic_cast<Cvariable *>((*ival)->getObject()) ;
        if (pCvar && (pCvar->getName() == sName) && (false == pCvar->isEmpty()))
        {
          for (ValIter ival = pCvar->vect_val.begin() ; pCvar->vect_val.end() != ival ; ival++)
          {
            // on récupère le père des items de l'archetype

            // Global path: to be asked to the Blackboard
            //
            if (LABEL_ALIAS == (*ival)->getLabel()) // items
            {
              Calias *pAlias = dynamic_cast<Calias *>((*ival)->getObject()) ;
              if (NULL == pAlias)
                break ;

              // La question doit etre un chemin de codes sens (séparateur : '/')
              string sQuestion = string("") ;
              string sParams   = string("") ;
              splitString(pAlias->getValue(), &sQuestion, &sParams, '|') ;

              NSSearchStruct searchStruct ;
              searchStruct.init(sParams) ;

              NSPatPathoArray* pPatPathoLocale = NULL ;

              bool bWaitForBlackBoard = true ;
              // on pose la question au blackboard
              string sAnswerDate ;

              displayThinkingCursor() ;

              while (bWaitForBlackBoard)
              {
                AnswerStatus::ANSWERSTATUS res = _pContexte->getBBinterface()->getAnswer2Question(sQuestion, "", &pPatPathoLocale, sAnswerDate, false, false, (HWND) 0, &searchStruct) ;
                if (AnswerStatus::astatusProcessed == res)
                {
                  bWaitForBlackBoard = false ;

                  // on teste d'abord l'existence de la variable
                  // (la patpatho résultat est non vide ==> la variable existe)
                  if ((pPatPathoLocale) && (false == (pPatPathoLocale)->empty()))
                  {
                    sAlias = sQuestion ;
                    *ppPatPathoArray = pPatPathoLocale ;
                    displayNormalCursor() ;
                    return true ;
                  }
                }
              }
              displayNormalCursor() ;

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

              if (_pLocalPatho)
              {
                PatPathoIter pptIt = _pLocalPatho->begin() ;
                if (_pLocalPatho->CheminDansPatpatho(sVarAlias, string(1, cheminSeparationMARK), &pptIt))
                {
                  NSPatPathoArray* pPatPathoLocale = new NSPatPathoArray(_pContexte->getSuperviseur()) ;
                  _pLocalPatho->ExtrairePatPathoFreres(pptIt, pPatPathoLocale) ;
                  *ppPatPathoArray = pPatPathoLocale ;
                  sAlias = sVarAlias ;
                  return true ;
                }
              }
            }
          } // fin du for sur les alias

          // on sort du for global car on a trouvé le nom de la variable
          return false ;
        } // fin du if (pCvar->getStringAttribute(ATTRIBUT_VAR_NOM) == sName)
      }
    } // fin du for global (sur les variables)
  }

  // parsing des variables globales
  Cglobalvars *gVars = getGlobalVars() ;
  if ((gVars) && (gVars->isVarP(sName, GlobalVar::variable)))
  {
    NSPatPathoArray * pPPT = gVars->getPPT(sName, GlobalVar::variable) ;
    if (pPPT)
    {
      *ppPatPathoArray = new NSPatPathoArray(*pPPT) ;
      return true ;
    }
  }

  return false ;
}

bool
NSValidateur::getValeurContrainte(string sName)
{
try
{
	bool result = false ;

/*
  if ((!pValidity) || (pValidity->getValArray()->empty()))
		return false ;
*/
  if (NULL == _pValidity)
    return false ;

  // At first, we process local variables (those that are defined inside the validator
  //
  // The point here is that if they have been defined specifically, it
  // means that they deserve to be processed specifically too
  //
  if (false == _pValidity->getValArray()->empty())
  {
    for (ValIter ival = _pValidity->getValArray()->begin() ; _pValidity->getValArray()->end() != ival ; ival++)
    {
      if (LABEL_CONTRAINTE == (*ival)->getLabel())
      {        Ccontrainte* pCcont = dynamic_cast<Ccontrainte *>((*ival)->getObject()) ;
        if (pCcont && ((string("") == sName) || (pCcont->getStringAttribute(ATTRIBUT_CONTR_NOM) == sName)))
        {
          if (pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_EXP))
            result = AnalyseExpression(pCcont->getStringAttribute(ATTRIBUT_CONTR_EXP)) ;
          else if (pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_EXIST))
          {
            NSPatPathoArray* pPatPathoArray = NULL ;
            string sAlias ;
            string sVarName = pCcont->getStringAttribute(ATTRIBUT_CONTR_VAR) ;
            result = getValeurVariable(sVarName, sAlias, &pPatPathoArray) ;
            if (pPatPathoArray)
              delete pPatPathoArray ;
          }
          else if (pCcont->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_COND))
          {
            string sContrExp = pCcont->getStringAttribute(ATTRIBUT_CONTR_EXP) ;
            size_t iBookMark = 0 ;
            result = Interprete(sContrExp, iBookMark) ;
          }

          if (string("") != sName)
            break ;

          if (false == result)
            break ;
        }
      }
    }
  }

  // parsing des contraintes globales
  Cglobalvars *gVars = getGlobalVars() ;
  if ((gVars) && (gVars->isVarP(sName, GlobalVar::constraint)))
  {
    switch (gVars->getState(sName, GlobalVar::constraint))
    {
      case GlobalVar::stateNotDef :
      	if      (_pValidity->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_EXP))
        	result = AnalyseExpression(_pValidity->getStringAttribute(ATTRIBUT_CONTR_EXP)) ;
				else if (_pValidity->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_EXIST))
				{
					NSPatPathoArray * pPPT = NULL ;
					string sAlias ;
					string sVarName = _pValidity->getStringAttribute(ATTRIBUT_CONTR_VAR) ;
					result = getValeurVariable(sVarName, sAlias, &pPPT) ;
					if (pPPT)
						delete pPPT ;
				}
				else if (_pValidity->getStringAttribute(ATTRIBUT_CONTR_TYPE) == string(VAL_ATTR_CONTR_TYPE_COND))
        {
          string sContrExp = _pValidity->getStringAttribute(ATTRIBUT_CONTR_EXP) ;
          size_t iBookMark = 0 ;
					result = Interprete(sContrExp, iBookMark) ;
        }
				break ;
      case GlobalVar::found       : return true ;
      case GlobalVar::notFound    :
      default                     : return false ;
    }
  }
	return result ;
}
catch (...)
{
  erreur("Exception NSValidateur::getValeurContrainte.", standardError) ;
  return false ;
}
}

bool
NSValidateur::AnalyseExpression(string sExp)
{
try
{
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  strip(sExp, stripBoth) ;

	// recherche des éléments de l'expression
	size_t pos1 = sExp.find(" ") ;
	if (pos1 == string::npos)
	{
  	string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
    sErrorText += string(" ") + sExp ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}

	// nom de la variable
	string sVarName = string(sExp, 0, pos1) ;

	// operateur
	pos1 = sExp.find_first_not_of(' ', pos1 + 1) ;
	size_t pos2 = sExp.find(" ", pos1) ;
	if (pos2 == string::npos)
	{
  	string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
    sErrorText += string(" ") + sExp ;
    pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    erreur(sErrorText.c_str(), standardError) ;
		return false ;
	}
	string sOper = string(sExp, pos1, pos2 - pos1) ;

	// chemin
	pos2 = sExp.find_first_not_of(' ', pos2 + 1) ;
	string sChemin = string(sExp, pos2, strlen(sExp.c_str()) - pos2) ;

  string  sVarAlias ;
	string  sValeur1, sUnite1, sValeur2, sUnite2 ;
	string  sTemp, sFormat ;
	double  dValue1, dValue2 ;
	bool    bValeur = false ;

	// Si le chemin est composé de Valeur[Unite]
	pos1 = sChemin.find("[") ;
	if (pos1 != string::npos)
	{
		sValeur2 = string(sChemin, 0, pos1) ;
		pos2 = sChemin.find("]", pos1 + 1) ;
		if (pos2 == string::npos)
		{
    	string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
    	sErrorText += string(" ") + sExp ;
    	pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    	erreur(sErrorText.c_str(), standardError) ;
			return false ;
		}
		sUnite2 = string(sChemin, pos1 + 1, pos2 - pos1 - 1) ;
		bValeur = true ;
	}

	NSPatPathoArray* pAnswer = NULL ;
	bool result = false ;

	if (getValeurVariable(sVarName, sVarAlias, &pAnswer))
	{
    NSPatPathoArray* pAnswerToUse = pAnswer ;
    if (NULL == pAnswerToUse)
      pAnswerToUse = _pLocalPatho ;

		if (bValeur && pAnswerToUse)
		{
			PatPathoIter iter = pAnswer->begin() ;
			int iLigneBase = (*iter)->getLigne() ;
			sTemp = "" ;
			sFormat = "" ;

			while ((pAnswer->end() != iter) && ((*iter)->getLigne() == iLigneBase))
			{
        string sLexique = (*iter)->getLexique() ;
				if ((string("") != sLexique) && ('£' == sLexique[0]))
				{
					sTemp       = (*iter)->getLexique() ;
					NSDico::donneCodeSens(&sTemp, &sFormat) ;
					sValeur1    = (*iter)->getComplement() ;
					sTemp       = (*iter)->getUnit() ;
					NSDico::donneCodeSens(&sTemp, &sUnite1) ;
					break ;
			  }
			  iter++ ;
		  }

      if (sUnite1 == sUnite2)
      {
        if (string("") == sFormat)
          return false ;
        else if ('N' == sFormat[1])
        {
          dValue1 = atof(sValeur1.c_str()) ;
          dValue2 = atof(sValeur2.c_str()) ;
        }
      }
      else
      {
        if (pAnswer)
          delete pAnswer ;
        return false ;
      }
	  }

	  // cas de la recherche parmis tous les freres
    if ('*' == sOper[0])
    {
      sOper = string(sOper, 1, strlen(sOper.c_str()) - 1) ;

      if      (string("|=") == sOper)
        result = pAnswer->CheminDansPatpatho(sChemin) ;
      else if (string("<") == sOper)
      {
        if ((bValeur) && ('N' == sFormat[1]))
          result = (dValue1 < dValue2) ;
      }
      else if (string("<=") == sOper)
      {
        if ((bValeur) && ('N' == sFormat[1]))
          result = (dValue1 <= dValue2) ;
      }
      else if (string("==") == sOper)
      {
        if ((bValeur) && ('N' == sFormat[1]))
          result = (dValue1 == dValue2) ;
      }
      else if (string(">") == sOper)
      {
        if ((bValeur) && ('N' == sFormat[1]))
          result = (dValue1 > dValue2) ;
      }
      else if (string(">=") == sOper)
      {
        if ((bValeur) && ('N' == sFormat[1]))
          result = (dValue1 >= dValue2) ;
      }
      else
      {
      	string sErrorText = pSuper->getText("referentialErrors", "invalidOperatorForExpressions") ;
    		sErrorText += string(" -> ") + sOper + string(" in ") + sExp ;
    		pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
    		erreur(sErrorText.c_str(), standardError) ;
        if (pAnswer)
          delete pAnswer ;
        return false ;
      }
    }
    else
    {
    	string sErrorText = pSuper->getText("referentialErrors", "invalidOperatorForExpressions") ;
      sErrorText += string(" -> ") + sOper + string(" in ") + sExp ;
      pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
      erreur(sErrorText.c_str(), standardError) ;
      if (pAnswer)
        delete pAnswer ;
      return false ;
    }
	}

	if (pAnswer)
		delete pAnswer ;

	return result ;
}
catch (...)
{
  erreur("Exception NSValidateur::AnalyseExpression.", standardError) ;
  return false ;
}
}

bool
NSValidateur::carAutorise(char c, bool debut)
{
  if      ((!debut) && (c >= '0') && (c <= '9'))
    return true ;
  else if ((c >= 'A') && (c <= 'Z'))
    return true ;
  else if ((c >= 'a') && (c <= 'z'))
    return true ;
  else if ((!debut) && ((c == '_') || (c == '-') || (c == '+')))
    return true ;

  return false ;
}

// -----------------------------------------------------------------------------
// Fonction d'évaluation du validateur de contraintes
// renvoie 0 ou 1 dans le cas normal, -1 si erreur
// La chaine vide est considérée comme vraie (1) par convention
// -----------------------------------------------------------------------------
int
NSValidateur::Interprete(string sReq, size_t& cc)
{
  NSSuper *pSuper = _pContexte->getSuperviseur() ;

  int  result = 1 ;
  int  result1 ;
  char oper = ' ' ;
  bool bNegation ;

  while (cc < strlen(sReq.c_str()))  {
    result1 = 1 ;
    bNegation = false ;

    while ((cc < strlen(sReq.c_str())) && (' ' == sReq[cc]))      cc++ ;

    // on évalue l'opérande en cours    if (cc < strlen(sReq.c_str()))
    {
      // on évalue d'abord la négation
      if ('!' == sReq[cc])
      {
        cc++ ;
        bNegation = true ;
      }

      if (cc == strlen(sReq.c_str()))
      {
        string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sReq ;
    		pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }

      // on doit avoir ici une '(' ou un nom de contrainte
      if (carAutorise(sReq[cc], true))
      {
        string sNomContrainte = "" ;
        while ((cc < strlen(sReq.c_str())) && carAutorise(sReq[cc]))
        {
          sNomContrainte += string(1, sReq[cc]) ;
          cc++ ;
        }
        if (string("") != sNomContrainte)
          result1 = getValeurContrainte(sNomContrainte) ;
      }
      else if ('(' == sReq[cc])
      {
        cc++ ;
        result1 = Interprete(sReq, cc) ;
      }
      else
      {
        string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sReq ;
    		pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }
    }

    // cas erreur à l'évaluation de l'opérande    if (-1 == result1)
      return -1 ;
    if (bNegation)
      result1 = !result1 ;

    // on calcule le résultat selon l'opérateur en cours    if      (' ' == oper)
      result = result1 ;
    else if ('|' == oper)
      result = result || result1 ;
    else if ('&' == oper)
      result = result && result1 ;

    // on avance à nouveau    while ((cc < strlen(sReq.c_str())) && (' ' == sReq[cc]))
      cc++ ;

    // on évalue l'opérateur    if (cc < strlen(sReq.c_str()))
    {
      // on doit avoir ici une ')' ou un opérateur
      if (('|' == sReq[cc]) || ('&' == sReq[cc]))
      {
        oper = sReq[cc] ;
        cc++ ;
      }
      else if (')' == sReq[cc])
      {
        cc++ ;
        return result ;
      }
      else
      {
        string sErrorText = pSuper->getText("referentialErrors", "validationStringSyntaxError") ;
				sErrorText += string(" ") + sReq ;
    		pSuper->trace(&sErrorText, 1, NSSuper::trError) ;
				erreur(sErrorText.c_str(), standardError) ;
        return -1 ;
      }
    }
  }
  return result ;
}

string
NSValidateur::getValidator()
{
  if (_pValidity->getValArray()->empty())
    return string("") ;

  string sResult = string("") ;

  for (ValIter ival = _pValidity->getValArray()->begin() ; _pValidity->getValArray()->end() != ival ; ival++)
  {
    if (LABEL_VALIDATEUR == (*ival)->getLabel())
    {      Cvalidateur* pCvalidator = dynamic_cast<Cvalidateur *>((*ival)->getObject()) ;
      sResult = pCvalidator->getStringAttribute(ATTRIBUT_VALIDATEUR_COND) ;
      break ;
    }
  }

  return sResult ;
}

bool
NSValidateur::Validation()
{
  string sValidator = getValidator() ;

  if (string("") == sValidator)
    return getValeurContrainte() ;

  return Validation(sValidator) ;
}

bool
NSValidateur::Validation(string sValidator)
{
  size_t iBookMark = 0 ;
  bool bResult = Interprete(sValidator, iBookMark) ;
  return bResult ;
}

Cglobalvars*
NSValidateur::getGlobalVars()
{
  if (_pGlobalVars)
    return _pGlobalVars ;

  // we get the root balise e.g. the proposition balise
  Cbalise *fatherBalise = _pValidity->parent ;
  Cbalise *rootBalise = _pValidity ;
  while (fatherBalise)
  {
    rootBalise = fatherBalise ;
    fatherBalise = rootBalise->parent ;
  }

  Creferentiel *refBalise = dynamic_cast<Creferentiel *>(rootBalise) ;
  if (refBalise)
  {
    Cglobalvars *gVars = refBalise->getGVars() ;
    if (gVars)
    {
      _pGlobalVars = gVars ;
      return gVars ;
    }
  }

  return NULL ;
}

void
NSValidateur::displayThinkingCursor()
{
#ifdef _OWL
  _pSuper->donneMainWindow()->SetCursor(pNSDLLModule, IDC_THINKING_CURSOR) ;
#endif
}

void
NSValidateur::displayNormalCursor()
{
#ifdef _OWL
  _pSuper->donneMainWindow()->SetCursor(0, IDC_ARROW) ;
#endif
}

// -----------------------------------------------------------------------------
//
// Classe NSDrufRefFileManager
//
// -----------------------------------------------------------------------------

NSDrufRefFileManager::NSDrufRefFileManager()
{
  lObjectCount++ ;

	_pRefParseur       = (nsrefParseur*) 0 ;
  _sFullPathFileName = string("") ;
}

NSDrufRefFileManager::~NSDrufRefFileManager()
{
  lObjectCount-- ;

	if (_pRefParseur)
		delete _pRefParseur ;
}

bool
NSDrufRefFileManager::writeFile()
{
	if (((nsrefParseur*) NULL == _pRefParseur) || (string("") == _sFullPathFileName))
		return false ;

	Creferentiel* pReferential = _pRefParseur->getReferentiel() ;
  if ((Creferentiel*) NULL == pReferential)
		return false ;

  return true ;
}

