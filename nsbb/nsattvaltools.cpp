// -----------------------------------------------------------------------------
// nsattvaltools.cpp
// -----------------------------------------------------------------------------
// Attributs/Valeurs pour les fichiers de configuration
// -----------------------------------------------------------------------------
// $Revision: 1.7 $
// $Author: pameline $
// $Date: 2013/11/11 22:35:28 $
// -----------------------------------------------------------------------------
// PA  - may 2003
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

#include "nsbb\nsattvaltools.h"#include "nsbb\nsattval.h"#include "nsbb\nsarcParseError.h"#include "nsbb\nsarc.h"#include "pre_parseur.h"// -----------------------------------------------------------------------------// classe nsMenuIniter// -----------------------------------------------------------------------------nsMenuIniter::nsMenuIniter(NSContexte *pCtx, bool bVerb){	pContexte = pCtx ;  bVerbose  = bVerb ;  hAccelerator = 0 ;  iAccelIndex  = 0 ;  memset(&accel_table, 0, sizeof(ACCEL) * 20) ;}nsMenuIniter::~nsMenuIniter()
{
}

boolnsMenuIniter::initMenuDescr(OWL::TMenuDescr *pMenuDescr, string sMenuName){	if ((NULL == pMenuDescr) || (string("") == sMenuName))  	return false ;  string sUserId = string("") ;  NSUtilisateurChoisi* pUtilisat = pContexte->getUtilisateur() ;	if (pUtilisat)		sUserId = pUtilisat->getID() ;	nsMenuParseur menuParser(pContexte, bVerbose) ;  if (false == menuParser.getMenu(sMenuName, sUserId, pContexte->PathName("FGLO")))  	return false ;  if ((NULL == menuParser.pMenuBar) || (menuParser.pMenuBar->vect_val.empty()))  	return false ;  uint uiMenuPos = 0 ;  ValIter ival ;  for (ival = menuParser.pMenuBar->vect_val.begin() ; menuParser.pMenuBar->vect_val.end() != ival ; ival++)  {  	if (LABEL_MENU_MENU == (*ival)->getLabel())		{
      CMenu *pMenu = dynamic_cast<CMenu *>((*ival)->getObject()) ;
      if (pMenu)
      {  			OWL::TMenu *pUIMenu = new OWL::TMenu ;    		OWL::TMenuItemInfo fileMenuInfo ;    		initMenu(pMenu, pUIMenu, &fileMenuInfo) ;    		pMenuDescr->InsertMenuItem(uiMenuPos++, true, fileMenuInfo) ;
    		if (fileMenuInfo.dwTypeData)
  				delete [] fileMenuInfo.dwTypeData ;      }    }  }  createAcceleratorTable() ;  return true ;}boolnsMenuIniter::initMenu(CMenu *pMenu, OWL::TMenu *pOWLmenu, OWL::TMenuItemInfo* pItemInfo){  if (NULL == pMenu)    return false ;	string sFctLabel = pMenu->getLabel() ;  string sFctID    = pMenu->getID() ;  if (string("") == sFctLabel)  	sFctLabel = pContexte->getSuperviseur()->getText("functionsLabels", sFctID) ;  // uint   uiID      = pContexte->getSuperviseur()->IDfromFunctionName(sFctID) ;  pItemInfo->fType      = MFT_STRING ;  pItemInfo->cch        = strlen(sFctLabel.c_str()) + 1 ;  pItemInfo->dwTypeData = new char [pItemInfo->cch] ;
  strcpy(pItemInfo->dwTypeData, sFctLabel.c_str()) ;  if (pMenu->vect_val.empty())  {  	pItemInfo->fMask = MIIM_TYPE ;    return true ;
  }  ValIter ival ;  for (ival = pMenu->vect_val.begin() ; pMenu->vect_val.end() != ival ; ival++)  {  	if (LABEL_MENU_MENUPOPUP == (*ival)->getLabel())		{    	CMenuPopup *pMenuPopup = dynamic_cast<CMenuPopup *>((*ival)->getObject()) ;      if (pMenuPopup)
      	initMenuPopup(pMenuPopup, pOWLmenu) ;    }  }  pItemInfo->fMask    = MIIM_TYPE | MIIM_SUBMENU ;  pItemInfo->hSubMenu = pOWLmenu->GetHandle() ;
	return true ;
}
boolnsMenuIniter::initMenuPopup(CMenuPopup *pMenuPopup, OWL::TMenu *pOWLmenu){	if ((NULL == pMenuPopup) || (pMenuPopup->vect_val.empty()))    return false ;  uint uiMenuPos = 0 ;  ValIter ival ;  for (ival = pMenuPopup->vect_val.begin() ; pMenuPopup->vect_val.end() != ival ; ival++)  {  	if (LABEL_MENU_MENUITEM == (*ival)->getLabel())		{
    	CMenuItem *pMenuItem = dynamic_cast<CMenuItem *>((*ival)->getObject()) ;
      if (pMenuItem)
      {
      	initMenuItem(pMenuItem, pOWLmenu) ;
        uiMenuPos++ ;
      }
		}
    else if (LABEL_MENU_MENU == (*ival)->getLabel())
		{
      CMenu *pMenu = dynamic_cast<CMenu *>((*ival)->getObject()) ;      if (pMenu)
      {
      	OWL::TMenu *pUIMenu = new OWL::TMenu ;
    		OWL::TMenuItemInfo fileMenuInfo ;    		initMenu(pMenu, pUIMenu, &fileMenuInfo) ;    		pOWLmenu->InsertMenuItem(uiMenuPos++, true, fileMenuInfo) ;
    		if (fileMenuInfo.dwTypeData)
  				delete [] fileMenuInfo.dwTypeData ;
      }    }    else if (LABEL_MENU_MENUSEPARATOR == (*ival)->getLabel())		{
      CMenuSeparator *pMenuSepar = dynamic_cast<CMenuSeparator *>((*ival)->getObject()) ;      if (pMenuSepar)
      {
      	initMenuSeparator(pMenuSepar, pOWLmenu) ;        uiMenuPos++ ;      }    }  }  return true ;}boolnsMenuIniter::initMenuItem(CMenuItem *pMenuItem, OWL::TMenu *pOWLmenu){	string sFctLabel = pMenuItem->getLabel() ;  string sFctID    = pMenuItem->getID() ;  if (string("") == sFctLabel)  	sFctLabel = pContexte->getSuperviseur()->getText("functionsLabels", sFctID) ;  else if ('\t' == sFctLabel[0])    sFctLabel = pContexte->getSuperviseur()->getText("functionsLabels", sFctID) + sFctLabel ;  uint uiID = pContexte->getSuperviseur()->IDfromFunctionName(sFctID) ;  if (uiID > 0)  {		pOWLmenu->AppendMenu(MF_ENABLED | MF_STRING, uiID, sFctLabel.c_str()) ;    WORD wAccel = pMenuItem->getAccelKeyAsWord() ;    if (WORD(0x00) != wAccel)    {      accel_table[iAccelIndex].key   = wAccel ;      accel_table[iAccelIndex].cmd   = uiID ;      accel_table[iAccelIndex].fVirt = pMenuItem->getAccelFlagAsByte() ;      iAccelIndex++ ;    }  }  else  	pOWLmenu->AppendMenu(MF_DISABLED | MF_STRING, uiID, sFctLabel.c_str()) ;  return true ;}boolnsMenuIniter::initMenuSeparator(CMenuSeparator *pMenuSepar, OWL::TMenu *pOWLmenu){	pOWLmenu->AppendMenu(MF_SEPARATOR) ;  return true ;}voidnsMenuIniter::createAcceleratorTable(){  if (0 == iAccelIndex)    return ;  hAccelerator = CreateAcceleratorTable(&accel_table[0], iAccelIndex) ;}voidnsMenuIniter::destroyAcceleratorTable(){  if (0 != hAccelerator)    DestroyAcceleratorTable(hAccelerator) ;}// -----------------------------------------------------------------------------// classe nsMenuParseur// -----------------------------------------------------------------------------
nsMenuParseur::nsMenuParseur(NSContexte *pCtx, bool bVerb)
{
  pMenuBar   = 0 ;
  pContexte  = pCtx ;
  bParsingOk = false ;
  bVerbose   = bVerb ;
}

nsMenuParseur::~nsMenuParseur()
{
  if (pMenuBar)
    delete pMenuBar ;
}

bool
nsMenuParseur::getMenu(string sMenuName, string sUserId, string sPersoDirectory)
{
	string sFileName ;

	if ((sUserId != string("")) && (sPersoDirectory != string("")))
	{
  	// sFileName = sPersoDirectory + sMenuName + string("_") + sUserId + string(".xml") ;
    char cDirSeparator = sPersoDirectory[strlen(sPersoDirectory.c_str()) - 1] ;
    sFileName = sPersoDirectory + sUserId + string(1, cDirSeparator) + sMenuName + string(".xml") ;
    if (getMenuFromFile(sFileName))
    	return true ;
  }

  sFileName = sPersoDirectory + sMenuName + string(".xml") ;
  if (getMenuFromFile(sFileName))
    	return true ;

	string sErreur = pContexte->getSuperviseur()->getText("fileErrors", "errorOpeningInputFile") + string(" ") + sMenuName + string(".xml") ;
  erreur(sErreur.c_str(), standardError, 0) ;
  return false ;
}

bool
nsMenuParseur::getMenuFromFile(string sFileName)
{
	if (string("") == sFileName)
		return false ;

	string sData = string("") ;

  if (false == getStringFromFile(sFileName, &sData))
		return false ;

  if (string("") == sData)
		return false ;

	return getMenuFromString(&sData) ;
}

bool
nsMenuParseur::getStringFromFile(string sFileName, string* pData)
{
	if (((string*) NULL == pData) || (string("") == sFileName))
		return false ;

	*pData = string("") ;

try
{
	ifstream inFile ;
  inFile.open(sFileName.c_str()) ;
  if (!inFile)
  	return false ;

  string sLine ;

  while (!inFile.eof())
  {
  	getline(inFile, sLine) ;
    if (string("") != sLine)
    	*pData += sLine + string("\n") ;
  }

  inFile.close() ;

  return true ;
}
catch (...)
{
	string sErreur = string("Exception nsMenuParseur::getStringFromFile : ") + pContexte->getSuperviseur()->getText("fileErrors", "errorReadingFile") + string(" ") + sFileName ;
	erreur(sErreur.c_str(), standardError, 0) ;
  return false ;
}
}

bool
nsMenuParseur::getMenuFromString(string* pXml_string)
{
	if (!pXml_string || (*pXml_string == string("")))
  	return false ;

	bool reponse = pre_parseur(pXml_string) ;
	if (reponse == false)
		return false ;

  int iParsingError ;
	if (parse_message(*pXml_string, string("menubar"), &iParsingError) == false)
  {
    if (bVerbose)
    {
      string sErrParse = parseErrorMessage(iParsingError) ;
      string sErreur = string("Incorrect menu : ") + sErrParse ;
      erreur(sErreur.c_str(), standardError, 0) ;
    }
    return false ;
  }

  bParsingOk = true ;
  return true ;
}


bool
nsMenuParseur::parse_message(string arc, string tag, int *iParsingError)
{
try
{
  size_t  arc_start, arc_end, header_end, attrib_start ;
  string  attributes, values ;

  arc_start = arc.find("<" + tag) ;
  if (arc_start != string::npos)
  {
    arc_end = arc.find("</" + tag + ">") ;
    if (arc_end == string::npos)
    {
      // erreur (pas de balise de fin)
      *iParsingError = EARCHETYPE_BALISE_FIN ;
      return false ;
    }

    header_end = arc.find(">", arc_start) ;
    if (arc_end < header_end)
    {
      // erreur (pas de fin de balise de debut)
      *iParsingError = EARCHETYPE_BALISE_DEBUT_FERMANTE ;
      return false ;
    }

    // attrib_start indique soit header_end('>') soit le premier attribut
    attrib_start  = arc.find_first_not_of(' ', arc_start + tag.length() + 1) ;

    attributes    = string(arc, attrib_start, header_end - attrib_start) ;
    values        = string(arc, header_end + 1, arc_end - header_end - 1) ;
  }
  else
  {
    // erreur (pas de balise de debut)
    *iParsingError = EARCHETYPE_BALISE_DEBUT ;
    return false ;
  }

  pMenuBar = new CMenuBar(attributes, values, pContexte) ;
  if (false == pMenuBar->parser())
  {
    *iParsingError = pMenuBar->getParsingError() ;
    return false ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception nsMenuParseur::parse_message.", standardError, 0) ;
  return false ;
}
}
// -----------------------------------------------------------------------------// classe CMenuBar// -----------------------------------------------------------------------------CMenuBar::CMenuBar(string attributs, string values, NSContexte *pCtx)         :Cbalise(attributs, values)
{
  pContexte     = pCtx ;
}

CMenuBar::~CMenuBar()
{
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
/*
      Cbalise *pBalise = (*ival)->getObject() ;

      CMenu *pCMenu = dynamic_cast<CMenu*>(pBalise) ;
      if (pCMenu)
      {
        delete pCMenu ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
*/
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
}

bool
CMenuBar::parser()
{
  // attributs
  if (false == parser_attributs())
    return false ;

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
		if ((*ival)->getLabel() == LABEL_MENU_MENU) // dialogue
		{
      (*ival)->setObject(new CMenu((*ival)->getAttribute(), (*ival)->getValue(), pContexte, this)) ;
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
}boolCMenuBar::verif(){
  return true ;
}

bool
CMenuBar::compresser()
{
  return true ;
}

bool
CMenuBar::traiter()
{
  return true ;
}
// -----------------------------------------------------------------------------// classe CMenu// -----------------------------------------------------------------------------CMenu::CMenu(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp)      :Cbalise(attributs, values)
{
  pMenuBar  = pExp ;
  pContexte = pCtx ;
}

CMenu::~CMenu()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CMenuPopup *pCPop = dynamic_cast<CMenuPopup*>(pBalise) ;
      if (pCPop)
      {
        delete pCPop ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
CMenu::parser()
{
  // attributs
  if (false == parser_attributs())
    return false ;

	// *** valeurs ***
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

  ValIter ival ;
	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_MENU_MENUPOPUP) // popup
		{
      (*ival)->setObject(new CMenuPopup((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMenuBar)) ;
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
}boolCMenu::verif(){
  return true ;
}

bool
CMenu::compresser()
{
  return true ;
}

bool
CMenu::traiter()
{
  return true ;
}

string
CMenu::getID()
{
  return getStringAttribute(ATTRIBUT_MENU_ID) ;
}

string
CMenu::getLabel()
{
  return getStringAttribute(ATTRIBUT_MENU_LABEL) ;
}


// -----------------------------------------------------------------------------
// classe CMenuPopup// -----------------------------------------------------------------------------CMenuPopup::CMenuPopup(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp)
           :Cbalise(attributs, values)
{
  pMenuBar  = pExp ;
  pContexte = pCtx ;
}

CMenuPopup::~CMenuPopup()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CMenuItem *pCItem = dynamic_cast<CMenuItem*>(pBalise) ;
      if (pCItem)
      {
        delete pCItem ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CMenu *pCMenu = dynamic_cast<CMenu*>(pBalise) ;
        if (pCMenu)
        {
          delete pCMenu ;
          (*ival)->setObject((Cbalise*) 0) ;
        }
        else
        {
          CMenuSeparator *pCSepar = dynamic_cast<CMenuSeparator*>(pBalise) ;
          if (pCSepar)
          {
            delete pCSepar ;
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
CMenuPopup::parser()
{
  // attributs
  if (false == parser_attributs())
    return false ;

  if (false == vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EDIALOGUE_ATTR_EMPTY ;
    return false ;
  }

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (cas normal : pas de valeur)
    _iParsingError = EDIALOGUE_VAL_NOT_EMPTY ;
    return false ;
  }

  ValIter ival ;
  for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_MENU_MENUITEM)
		{
      (*ival)->setObject(new CMenuItem((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMenuBar)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_MENU_MENU)
		{
      (*ival)->setObject(new CMenu((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMenuBar)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_MENU_MENUSEPARATOR)
		{
      (*ival)->setObject(new CMenuSeparator((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMenuBar)) ;
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
}

boolCMenuPopup::verif(){
  return true ;
}

bool
CMenuPopup::compresser()
{
  return true ;
}

bool
CMenuPopup::traiter()
{
  return true ;
}


// -----------------------------------------------------------------------------
// classe CMenuItem// -----------------------------------------------------------------------------CMenuItem::CMenuItem(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp)
          :Cbalise(attributs, values)
{
  pContexte = pCtx ;
  pMenuBar  = pExp ;
}

CMenuItem::~CMenuItem()
{
}

bool
CMenuItem::parser()
{
  // attributs
  if (false == parser_attributs())
    return false ;

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = EARCHETYPE_VAL_EMPTY ;
    return false ;
  }

	return true ;
}boolCMenuItem::verif(){
  return true ;
}

bool
CMenuItem::compresser()
{
  return true ;
}

bool
CMenuItem::traiter()
{
  return true ;
}

string
CMenuItem::getID()
{
  return getStringAttribute(ATTRIBUT_MENU_ID) ;
}

string
CMenuItem::getHelpId()
{
  return getStringAttribute(ATTRIBUT_MENU_HELPID) ;
}

string
CMenuItem::getLabel()
{
  // return getStringAttribute(ATTRIBUT_MENU_LABEL) ;

  string sBasicLabel = getStringAttribute(ATTRIBUT_MENU_LABEL) ;
  string sAccelKey = getStringAttribute(ATTRIBUT_MENU_ACCEL_KEY) ;
  if (string("") == sAccelKey)
    return sBasicLabel ;

  if ('^' == sAccelKey[0])
    sAccelKey = string(sAccelKey, 1, strlen(sAccelKey.c_str()) - 1) ;
  if (string("") == sAccelKey)
    return sBasicLabel ;
  if (strlen(sAccelKey.c_str()) > 1)
  {
    if ((strlen(sAccelKey.c_str()) > 2) && (string("VK_") == string(sAccelKey, 0, 3)))
      sAccelKey = string(sAccelKey, 3, strlen(sAccelKey.c_str())-3) ;
    else
      return sBasicLabel ;
  }

  sBasicLabel += string(1, '\t') ;

  BYTE accelFlag = getAccelFlagAsByte() ;
  if (accelFlag & FSHIFT)
    sBasicLabel += string("Shift+") ;
  if (accelFlag & FALT)
    sBasicLabel += string("Alt+") ;
  if (accelFlag & FCONTROL)
    sBasicLabel += string("Ctrl+") ;

  sBasicLabel += sAccelKey ;

  return sBasicLabel ;
}

string
CMenuItem::getAccelKey()
{
  return getStringAttribute(ATTRIBUT_MENU_ACCEL_KEY) ;
}

string
CMenuItem::getAccelFlag()
{
  return getStringAttribute(ATTRIBUT_MENU_ACCEL_FLAG) ;
}

WORD
CMenuItem::getAccelKeyAsWord()
{
  string sAccelKey = getStringAttribute(ATTRIBUT_MENU_ACCEL_KEY) ;
  if (string("") == sAccelKey)
    return WORD(0x00) ;

  if ('^' == sAccelKey[0])
    sAccelKey = string(sAccelKey, 1, strlen(sAccelKey.c_str()) - 1) ;

  if (string("") == sAccelKey)
    return WORD(0x00) ;

  if (strlen(sAccelKey.c_str()) == 1)
    return WORD(sAccelKey[0]) ;

  return getVirtualKeyFromString(sAccelKey) ;
}

BYTE
CMenuItem::getAccelFlagAsByte()
{
  BYTE iFlag = 0 ;

  string sAccelKey = getStringAttribute(ATTRIBUT_MENU_ACCEL_KEY) ;
  if (string("") != sAccelKey)
  {
    if ('^' == sAccelKey[0])
      iFlag |= FCONTROL	;
    if ((strlen(sAccelKey.c_str()) > 2) && (string("VK") == string(sAccelKey, 0, 2)))
      iFlag |= FVIRTKEY	;
  }

  string sFlag = getAccelFlag() ;
  if (string::npos != sFlag.find("VIRTKEY"))
    iFlag |= FVIRTKEY	;
  if (string::npos != sFlag.find("CONTROL"))
    iFlag |= FCONTROL	;
  if (string::npos != sFlag.find("SHIFT"))
    iFlag |= FSHIFT	;
  if (string::npos != sFlag.find("ALT"))
    iFlag |= FALT ;

  return iFlag ;
}

// -----------------------------------------------------------------------------
// classe CdocumentExport// -----------------------------------------------------------------------------
CMenuSeparator::CMenuSeparator(string attributs, string values, NSContexte *pCtx, CMenuBar *pExp)
               :Cbalise(attributs, values)
{
  pContexte = pCtx ;
  pMenuBar  = pExp ;
}


CMenuSeparator::~CMenuSeparator ()
{
}


bool
CMenuSeparator::parser()
{
  // attributs
  if (false == parser_attributs())
    return false ;

  if (false == vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

	return true ;
}

bool
CMenuSeparator::verif(){
  return true ;
}

bool
CMenuSeparator::compresser()
{
  return true ;
}

bool
CMenuSeparator::traiter()
{
  return true ;
}

// fin de nsattvaltools.cpp
/////////////////////////////////////

