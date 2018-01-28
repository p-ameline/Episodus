// -----------------------------------------------------------------------------
// nsattvalex.cpp
// -----------------------------------------------------------------------------
// Attributs/Valeurs d'archétypes XML
// -----------------------------------------------------------------------------
// $Revision: 1.10 $
// $Author: pameline $
// $Date: 2013/12/15 15:40:26 $
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

#ifdef _ENTERPRISE_DLL
  #include "enterpriseLus\superLus.h"
#else
  #include "nautilus\nssuper.h"
  #include "nautilus\nsepicap.h"
  #include "nsepisod\eptables.h"
#endif

#include "nsbb\nsattvalex.h"
#include "nsbb\nsattval.h"#include "nsbb\nsarcParseError.h"#include "nsbb\nsarc.h"#include "nsbb\nspatpat.h"#include "pre_parseur.h"// -----------------------------------------------------------------------------// classe nsExportParseur// -----------------------------------------------------------------------------
nsExportParseur::nsExportParseur(NSContexte *pCtx, string sRelease, bool bVerb)
{
  pMessage      = 0 ;
  pContexte     = pCtx ;
  bParsingOk    = false ;
  bVerbose      = bVerb ;
  sReleaseNb    = sRelease ;
}


nsExportParseur::~nsExportParseur()
{
  if (pMessage)
    delete pMessage ;
}


bool
nsExportParseur::open(string sXmlFileName)
{
  ifstream  xml_file ;
	string    tmp_xml_string ;
	string    xml_string ;
	bool      reponse ;

	xml_file.open(sXmlFileName.c_str()) ;

  if (!xml_file)
  {
    string sErreur = string("Impossible d'ouvrir le fichier XML (") + sXmlFileName + string(")") ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

	while (getline(xml_file, tmp_xml_string))
		xml_string += tmp_xml_string + '\n' ;
	xml_file.close();

  xml_string = string("<export>\n") + xml_string + string("</export>\n") ;

  // xml_string = TexteXmlVersNormal(xml_string);

	reponse = pre_parseur(&xml_string) ;
	if (reponse == false)
		return false ;

  int iParsingError ;
	if (parse_message(xml_string, string("export"), &iParsingError) == false)
  {
    if (bVerbose)
    {
      string sErrParse = parseErrorMessage(iParsingError) ;
      string sErreur = string("Le message est incorrect : ") + sErrParse ;
      erreur(sErreur.c_str(), standardError, 0) ;
    }
    return false ;
  }

  bParsingOk = true ;
  return true ;
}


bool
nsExportParseur::parse_message(string arc, string tag, int *iParsingError)
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

  pMessage = new CmessageExport(attributes, values, pContexte) ;
  pMessage->sReleaseNb = sReleaseNb ;
  if (false == pMessage->parser())
  {
    *iParsingError = pMessage->getParsingError() ;
    return false ;
  }
  return true ;
}
catch (...)
{
  erreur("Exception nsExportParseur::parse_message.", standardError, 0) ;
  return false ;
}
}
// -----------------------------------------------------------------------------// classe CmessageExport// -----------------------------------------------------------------------------CmessageExport::CmessageExport(string attributs, string values, NSContexte *pCtx, string sMedId)               :Cbalise(attributs, values)
{
  pContexte     = pCtx ;
  sUserId       = sMedId ;
  sPathMsg      = "" ;
  sReleaseNb    = "" ;
}

CmessageExport::~CmessageExport()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CpatientExport *pCPatEx = dynamic_cast<CpatientExport*>(pBalise) ;
      if (pCPatEx)
      {
        delete pCPatEx ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
CmessageExport::parser()
{
  int       nb_patients ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

  nb_patients    = 0 ;
	for (ival = vect_val.begin() ; ival != vect_val.end() ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_PATIENT) // dialogue
		{
      (*ival)->setObject(new CpatientExport((*ival)->getAttribute(), (*ival)->getValue(), pContexte, this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_patients++ ;
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

  /*
	if (nb_patients < 1)
  {
    // erreur (nb valeurs incorrect)
    iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }
  */

	return true ;
}boolCmessageExport::verif(){
  return true ;
}


bool
CmessageExport::compresser()
{
  return true ;
}


bool
CmessageExport::traiter()
{
  return true ;
}
boolCmessageExport::addToSFMGfile(string sFileIndex, string sNewLine){  ofstream    outFile ;  string    sFileName = sUserId ;  string    sPathName = sPathMsg ;  if (pContexte)  {    if (sUserId == "")      sFileName = pContexte->getEpisodus()->sPrometheUserId ;    sPathName = pContexte->PathName("BGLO") ;  }

  if (sFileName == "")
    return false ;

  sFileName += string("D") + sFileIndex + string(".txt") ;
  string sFichier = sPathName + sFileName ;

  outFile.open(sFichier.c_str(), ios::ate | ios::app);
  if (!outFile)
  {
    string sErrMess = string("Erreur d'ouverture en écriture du fichier ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return false ;
  }

  outFile << (sNewLine + string("\n")) ;

  outFile.close() ;
  return true ;}// -----------------------------------------------------------------------------// classe CpatientExport// -----------------------------------------------------------------------------CpatientExport::CpatientExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp)               :Cbalise(attributs, values)
{
  pMsgExport  = pExp ;
  pContexte   = pCtx ;
}

CpatientExport::~CpatientExport()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CadminDataExport *pCAdmin = dynamic_cast<CadminDataExport*>(pBalise) ;
      if (pCAdmin)
      {
        delete pCAdmin ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        ChealthDataExport *pCHDE = dynamic_cast<ChealthDataExport*>(pBalise) ;
        if (pCHDE)
        {
          delete pCHDE ;
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
CpatientExport::parser()
{
  int       nb_admindata, nb_healthdata ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

  nb_admindata    = 0 ;
  nb_healthdata   = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_ADMINDATA) // dialogue
		{
      (*ival)->setObject(new CadminDataExport((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_admindata++ ;
		}
    else if ((*ival)->getLabel() == LABEL_EX_HEALTHDATA) // references
    {
      (*ival)->setObject(new ChealthDataExport((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      nb_healthdata++ ;
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

	if ((nb_admindata < 1) || (nb_healthdata < 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }

	return true ;
}boolCpatientExport::verif(){
  return true ;
}


bool
CpatientExport::compresser()
{
  return true ;
}


bool
CpatientExport::traiter()
{
  return true ;
}


string
CpatientExport::getID()
{
  return getStringAttribute(ATTRIBUT_EX_PATIENT_ID) ;
}


string
CpatientExport::getCode()
{
  return getStringAttribute(ATTRIBUT_EX_PATIENT_CODE) ;
}


string
CpatientExport::getContact()
{
  return getStringAttribute(ATTRIBUT_EX_PATIENT_CONTACT) ;
}


bool
CpatientExport::createSFMGfiles()
{
  string sPatLine     = "" ;
  string sPatIndice   = donnePatIndice() ;
  string sActeIndice  = donneActeIndice() ;

  if (vect_val.empty())
    return false ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_ADMINDATA) // dialogue
		{
      CadminDataExport *pAdmin = dynamic_cast<CadminDataExport *>((*ival)->getObject()) ;
      if (pAdmin)
      {
        bool bAlreadyExist = false ;
        if (false == pMsgExport->aVecteurPatients.empty())
        {
          EquiItemIter iStrIter = pMsgExport->aVecteurPatients.begin();
          for ( ; (pMsgExport->aVecteurPatients.end() != iStrIter) && (**iStrIter != sPatIndice) ; iStrIter++)
            ;
          if (iStrIter != pMsgExport->aVecteurPatients.end())
            bAlreadyExist = true ;
        }

        if (false == bAlreadyExist)
        {
          sPatLine = pAdmin->getNaissance() + "\t" + sPatIndice + "\t" + pAdmin->getSexe() ;
          pMsgExport->addToSFMGfile("08", sPatLine) ;
          pMsgExport->aVecteurPatients.push_back(new string(sPatIndice)) ;
        }
      }
		}
    else if ((*ival)->getLabel() == LABEL_EX_HEALTHDATA) // references
    {
      ChealthDataExport *pHealth = dynamic_cast<ChealthDataExport *>((*ival)->getObject()) ;
      if (pHealth)
        pHealth->createSFMGfiles(sPatIndice, sActeIndice) ;
    }
  }
  return true ;
}


string
CpatientExport::donnePatIndice()
{
  string sExtCode = getCode() ;
  if (sExtCode != "")
    return sExtCode ;

  string sIndice = getID() ;
  string sSubIndice = string(sIndice, 1, strlen(sIndice.c_str()) - 1) ;

  int iCompte = 0 ;
  int iMulti = 1 ;
  for (int i = strlen(sSubIndice.c_str()) - 1 ; i >= 0 ; i--)
  {
    if      ((sSubIndice[i] >= '0') && (sSubIndice[i] <= '9'))
      iCompte += (sSubIndice[i] - '0') * iMulti ;
    else if ((sSubIndice[i] >= 'A') && (sSubIndice[i] <= 'Z'))
      iCompte += ((sSubIndice[i] - 'A') + 10) * iMulti ;

    iMulti = iMulti * 36 ;
  }

  if (sIndice[0] == '2')
    iCompte += 50000 ;

  char szNum[6] ;
  numacar(szNum, iCompte, 5) ;

  return string(szNum) ;
}


string
CpatientExport::donneActeIndice()
{
  string sIndice = getContact() ;

  int iCompte = 0 ;
  int iMulti = 1 ;
  for (int i = strlen(sIndice.c_str()) - 1 ; i >= 0 ; i--)
  {
    if      ((sIndice[i] >= '0') && (sIndice[i] <= '9'))
      iCompte += (sIndice[i] - '0') * iMulti ;
    else if ((sIndice[i] >= 'A') && (sIndice[i] <= 'Z'))
      iCompte += ((sIndice[i] - 'A') + 10) * iMulti ;

    iMulti = iMulti * 36 ;
  }
  char szNum[6] ;
  numacar(szNum, iCompte, 5) ;

  return string(szNum) ;
}


// -----------------------------------------------------------------------------
// classe CadminDataExport// -----------------------------------------------------------------------------CadminDataExport::CadminDataExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp)
  : Cbalise(attributs, values)
{
  pMsgExport  = pExp ;
  pContexte   = pCtx ;
}

CadminDataExport::~CadminDataExport()
{
}

bool
CadminDataExport::parser()
{
  // int       nb_dll, nb_nom, nb_ref ;
  // Attr_iter iattribut ;
  // Val_iter  ival ;

  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EDIALOGUE_ATTR_EMPTY ;
    return false ;
  }

  // nb_dll = 0 ;
  // nb_nom = 0 ;
  // nb_ref = 0 ;


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

boolCadminDataExport::verif(){
  return true ;
}

bool
CadminDataExport::compresser()
{
  return true ;
}

bool
CadminDataExport::traiter()
{
  return true ;
}


string
CadminDataExport::getSexe()
{
  return getStringAttribute(ATTRIBUT_EX_ADMIN_SEXE) ;
}


string
CadminDataExport::getNaissance()
{
  return getStringAttribute(ATTRIBUT_EX_ADMIN_NAISSANCE) ;
}


// -----------------------------------------------------------------------------
// classe ChealthDataExport// -----------------------------------------------------------------------------ChealthDataExport::ChealthDataExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp)
                  :Cbalise(attributs, values)
{
  pContexte   = pCtx ;
  pMsgExport  = pExp ;
}

ChealthDataExport::~ChealthDataExport()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CdocumentExport *pCDoc = dynamic_cast<CdocumentExport*>(pBalise) ;
      if (pCDoc)
      {
        delete pCDoc ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CgoalExport *pCGoal = dynamic_cast<CgoalExport*>(pBalise) ;
        if (pCGoal)
        {
          delete pCGoal ;
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
ChealthDataExport::parser()
{
  // int       nb_dialogue, nb_items, nb_contraintes ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_DOCUMENT) // dialogue
		{
      (*ival)->setObject(new CdocumentExport((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_EX_GOAL) // header
		{
      (*ival)->setObject(new CgoalExport((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
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
}boolChealthDataExport::verif(){
  return true ;
}


bool
ChealthDataExport::compresser()
{
  return true ;
}


bool
ChealthDataExport::traiter()
{
  return true ;
}


string
ChealthDataExport::getDate()
{
  return getStringAttribute(ATTRIBUT_EX_HEALTHDATA_DATE) ;
}


bool
ChealthDataExport::createSFMGfiles(string sCodePat, string sCodeActe)
{
  string  sActeLine = "" ;
  string  sDPIOUser = "" ;

  if (pContexte)
  {
		sDPIOUser = pContexte->getUtilisateur()->getNss() ;
    strip(sDPIOUser, stripLeft, '0') ;
  }
  else
    sDPIOUser = pMsgExport->sUserId ;

  sActeLine = string("-9") + "\t" + sCodePat + "\t" + sDPIOUser + "\t" + getDate() + "\t" + string("-9") + "\t" + string("DPIO") + "\t" + sCodeActe ;
  pMsgExport->addToSFMGfile("05", sActeLine) ;

  if (vect_val.empty())
    return false ;

  int iNumDoc     = 0 ;
  int iNumGoalDoc = 0 ;

  string sCurrentDoc = "" ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_DOCUMENT) // dialogue
		{
      CdocumentExport *pDocument = dynamic_cast<CdocumentExport *>((*ival)->getObject()) ;
      if (pDocument)
      {
        iNumDoc++ ;

        char szNumRc[4] ;
        numacar(szNumRc, iNumDoc, 3) ;
        sCurrentDoc = string(szNumRc) ;

        pDocument->createSFMGfiles(sCodePat, sCodeActe, sCurrentDoc, pDocument->getDate()) ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_EX_GOAL) // references
    {
      CgoalExport *pGoal = dynamic_cast<CgoalExport *>((*ival)->getObject()) ;
      if (pGoal)
      {
        iNumGoalDoc++ ;

        char szNumRc[4] ;
        numacar(szNumRc, iNumGoalDoc, 3) ;
        string sCurrentGoal = string(szNumRc) ;

        pGoal->createSFMGfiles(sCodePat, sCodeActe, sCurrentDoc, sCurrentGoal) ;
      }
    }
  }
  return true ;
}


// -----------------------------------------------------------------------------
// classe CdocumentExport// -----------------------------------------------------------------------------
CdocumentExport::CdocumentExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp)
                :Cbalise(attributs, values)
{
  pContexte       = pCtx ;
  pMsgExport      = pExp ;
  pPatPathoArray  = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

CdocumentExport::~CdocumentExport ()
{
  if (pPatPathoArray)
    delete pPatPathoArray ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CnodeExport *pCNode = dynamic_cast<CnodeExport*>(pBalise) ;
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
CdocumentExport::parser()
{
  // int       nb_loc ;
  int       nb_node ;
  // AttrIter iattribut ;
  ValIter  ival ;

  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

  nb_node = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      (*ival)->setObject(new CnodeExport((*ival)->getAttribute(), (*ival)->getValue(), 0)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }

      CnodeExport *pCnode = dynamic_cast<CnodeExport *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPatPathoArray) ;

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


// -----------------------------------------------------------------------------
boolCdocumentExport::verif(){
  return true ;
}

bool
CdocumentExport::compresser()
{
  return true ;
}


bool
CdocumentExport::traiter()
{
  return true ;
}


string
CdocumentExport::getCode()
{
  return getStringAttribute(ATTRIBUT_EX_DOCU_CODE) ;
}


string
CdocumentExport::getDate()
{
  return getStringAttribute(ATTRIBUT_EX_DOCU_DATE) ;
}


bool
CdocumentExport::createSFMGfiles(string sCodePat, string sCodeActe, string sIndexRC, string sDate)
{
/*
  if (pPatPathoArray->empty())
    return true ;

  string sIndexGlobalRC = sCodeActe + sIndexRC ;

  // Traitement des RC
  PatPathoIter pptIter ;
  string sCodeRC = "" ;

  if      (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISC1", "|", &pptIter)) // Côlon
  {
    if (pMsgExport->sReleaseNb == "1")
        sCodeRC = "802" ;
    else
        sCodeRC = "808" ;
  }
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISO1", "|", &pptIter)) // Col
  {
    if (pMsgExport->sReleaseNb == "1")
        sCodeRC = "801" ;
    else
        sCodeRC = "807" ;
  }
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISR1", "|", &pptIter)) // Prostate
    sCodeRC = "805" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISB1", "|", &pptIter)) // Bouche
    sCodeRC = "800" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISS1", "|", &pptIter)) // Sein
    sCodeRC = "806" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRIME1", "|", &pptIter)) // Mélanome
    sCodeRC = "804" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISP1", "|", &pptIter)) // Epithélioma
  {
    if (pMsgExport->sReleaseNb == "1")
        sCodeRC = "803" ;
    else
        sCodeRC = "809" ;
  }

  if (sCodeRC == "")
    return false ;

  pptIter++ ;

  string sCodePD = "" ;

  if      (string((*pptIter)->pDonnees->lexique) == "9NONC1")
    sCodePD = "1" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1RMOY1")
    sCodePD = "2" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1MAJO1")
    sCodePD = "3" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1IMPO1")
    sCodePD = "4" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1IMPT2")
    sCodePD = "5" ;

  string sRCline = sIndexGlobalRC + "\t" + sCodeActe + "\t" + sCodeRC + "\t" + sCodePD + "\t" + string("N") + "\t" + sDate ;
  pMsgExport->addToSFMGfile("35", sRCline) ;

  // Traitement des items
  if (pPatPathoArray->CheminDansPatpatho("0CALC1|QFARI1", "|", &pptIter))
  {
    int iItemCol = (*pptIter)->pDonnees->getColonne() ;
    int iItemId = 0 ;

    pptIter++ ;
    while ((pptIter != pPatPathoArray->end()) && ((*pptIter)->pDonnees->getColonne() > iItemCol))
    {
      if ((string((*pptIter)->pDonnees->lexique) == "6MGIT1") &&
          (string((*pptIter)->pDonnees->complement) != ""))
      {
        iItemId++ ;
        char szNumItem[3] ;
        numacar(szNumItem, iItemId, 2) ;
        string sItemID = sIndexGlobalRC + string(szNumItem) ;
        string sItemLine = sItemID + "\t" + sIndexGlobalRC + "\t" + string((*pptIter)->pDonnees->complement) ;
        pMsgExport->addToSFMGfile("20", sItemLine) ;
      }
      pptIter++ ;
    }
  }

  // Traitement des IMI
  VecteurString *pVecteurString = new VecteurString() ;
  if (pContexte)
  {
    // partie utilisant la table nsreflink (quand la méthode est appelée à
    // partir de Nautilus)
    NSRefLink *pRefLink = new NSRefLink(pContexte) ;
    pRefLink->lastError = pRefLink->open() ;
    if (pRefLink->lastError != DBIERR_NONE)
    {
      erreur("Impossible d'ouvrir la table NSREFLINK.", standardError, pRefLink->lastError) ;
      delete pRefLink ;
      return false ;
    }

    bool bResult = pRefLink->GetAllCodesAndValues(pPatPathoArray, "6MGIM", pVecteurString) ;

    pRefLink->lastError = pRefLink->close() ;
    if (pRefLink->lastError != DBIERR_NONE)
      erreur("Impossible de fermer la table NSREFLINK.", standardError, pRefLink->lastError) ;
    delete pRefLink ;

  }
  else
  {
    // partie utilisant un fichier en texte uniquement remplacant la table
    // nsreflink (quand la méthode est appelée par un exécutbale externe)
    NSRefLinkInfoArray  *pRefLink = new NSRefLinkInfoArray(pContexte, "nsreflink.txt") ;
    if (!pRefLink->fillArray())
    {
      // problème lors de la récupération des données contenues dans
      // nsreflink.txt -- il faudrait rajouter une erreur
      erreur("Impossible d'ouvrir ou de récupérer les données du fichier nsreflink.txt.", standardError, 0) ;
      delete pRefLink ;
      return false ;
    }

    bool  bResult = pRefLink->GetAllCodesAndValues(pPatPathoArray, "6MGIM", pVecteurString) ;
    delete pRefLink ;
  }

  if (!(pVecteurString->empty()))
  {
    int iImiId = 0 ;

    for (EquiItemIter j = pVecteurString->begin(); j != pVecteurString->end(); j++)
    {
      iImiId++ ;
      char szNumItem[3] ;
      numacar(szNumItem, iImiId, 2) ;
      string sImiID = sCodeActe + string(szNumItem) ;

      string sCode        = "" ;
      string sComplement  = "" ;

      size_t iSepar = (*j)->find("|") ;
      if (iSepar == string::npos)
        sCode = **j ;
      else
      {
        sCode       = string(**j, 0, iSepar) ;
        sComplement = string(**j, iSepar + 1, strlen((*j)->c_str()) - iSepar - 1) ;
      }

      string sIMIline = sImiID + "\t" + sCodeActe + "\t" + sCode ;
      if (sComplement != "")
        sIMIline += "\t" + sComplement ;
      pMsgExport->addToSFMGfile("14", sIMIline) ;
    }
  }

  delete pVecteurString ;
*/

  return true ;
}


// -----------------------------------------------------------------------------
// Classe CnodeExport
// -----------------------------------------------------------------------------
CnodeExport::CnodeExport(string attributs, string values, int iCol)
            :Cbalise(attributs, values)
{
  pPatPathoInfo1 = new NSPatPathoInfo ;
  pPatPathoInfo2 = 0 ;
  iLigne         = 0 ;
  iColonne       = iCol ;
}

CnodeExport::~CnodeExport()
{
  if (pPatPathoInfo1)
    delete pPatPathoInfo1 ;
  if (pPatPathoInfo2)
    delete pPatPathoInfo2 ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CnodeExport *pCNode = dynamic_cast<CnodeExport*>(pBalise) ;
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
CnodeExport::parser()
{
  // int       nb_loc ;
  int       nb_type, nb_lex, nb_compl, nb_cert, nb_interet, nb_pluriel, nb_unit ;
  string    sTexteLibre ;
  AttrIter iattribut ;
  ValIter  ival ;

  // attributs
  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = ENODE_ATTR_EMPTY ;
    return false ;
  }

  // nb_loc  = 0 ;
  nb_type     = 0 ; nb_lex      = 0 ; nb_compl  = 0 ;
  nb_cert = 0 ; nb_interet  = 0 ; nb_pluriel  = 0 ; nb_unit   = 0 ;

	for (iattribut = vect_attr.begin() ; iattribut != vect_attr.end() ; iattribut++)
  {
    if      ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_PAT_ID)
      pPatPathoInfo1->setPerson((*iattribut)->getValue()) ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_TREE_ID)
      pPatPathoInfo1->setDocum((*iattribut)->getValue()) ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_NODE_ID)
      pPatPathoInfo1->setNodeID((*iattribut)->getValue()) ;
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_LEXIQUE)
    {
      pPatPathoInfo1->setLexique((*iattribut)->getValue()) ;
      nb_lex++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_COMPLMNT)
    {
      // Attention l'attribut lexique doit etre défini avant complement
      if (nb_lex)
      {
        if (pPatPathoInfo1->getLexique() == string("£?????"))
        {
          sTexteLibre = (*iattribut)->getValue() ;

          if (sTexteLibre == "")            sTexteLibre = "[texte non récupéré]" ;
          else
            pPatPathoInfo1->setTexteLibre(sTexteLibre) ;
        }
        else
          pPatPathoInfo1->setComplement((*iattribut)->getValue()) ;

        nb_compl++ ;
      }
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_NODE_CERTITUDE)
    {
      pPatPathoInfo1->setCertitude((*iattribut)->getValue()) ;
      nb_cert++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_INTERET)
    {
      pPatPathoInfo1->setInteret((*iattribut)->getValue()) ;
      nb_interet++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_PLURIEL)
    {
      pPatPathoInfo1->setPluriel((*iattribut)->getValue()) ;
      nb_pluriel++ ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_UNIT_ID)
    {
      if (NULL == pPatPathoInfo2)
        pPatPathoInfo2 = new NSPatPathoInfo ;
      pPatPathoInfo2->setNodeID((*iattribut)->getValue()) ;
    }
    else if ((*iattribut)->getLabel() == ATTRIBUT_EX_NODE_UNITE)
    {
      if (NULL == pPatPathoInfo2)
        pPatPathoInfo2 = new NSPatPathoInfo ;
      pPatPathoInfo2->setLexique((*iattribut)->getValue()) ;
      nb_unit++ ;
    }
		else
    {
      // erreur (attribut incorrect)
      _iParsingError = ENODE_ATTR ;
      return false ;
    }
  }

	if (/*(nb_loc != 1) ||*/ (nb_type > 1)  || (nb_lex != 1)    || (nb_compl > 1) ||
      (nb_cert > 1) || (nb_interet > 1) || (nb_pluriel > 1) || (nb_unit > 1))
  {
    // erreur (nb attributs incorrect)
    _iParsingError = ENODE_NBATTR ;
    return false ;
  }

  if (pPatPathoInfo2)
  {
    if (string("") != pPatPathoInfo1->getDoc())
    {
      pPatPathoInfo2->setPerson(pPatPathoInfo1->getPerson()) ;
      pPatPathoInfo2->setDocum(pPatPathoInfo1->getDocum()) ;
    }
  }

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (false == vect_val.empty())
  {
    for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      if ((*ival)->getLabel() == LABEL_NODE) // fils
      {
        (*ival)->setObject(new CnodeExport((*ival)->getAttribute(), (*ival)->getValue(), iColonne + 1)) ;
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

boolCnodeExport::verif(){
  return true ;
}

bool
CnodeExport::compresser()
{
  return true ;
}

bool
CnodeExport::traiter()
{
  return true ;
}

bool
CnodeExport::chargePatPatho(NSPatPathoArray *pPPTArray)
{
  ValIter  ival ;
  int       ligne ;

  if (false == pPPTArray->empty())
    ligne = (pPPTArray->back())->getLigne() + 1 ;
  else
    ligne = ORIGINE_PATH_PATHO ;

  if (pPatPathoInfo2)
  {
    pPatPathoInfo2->setLigne(ligne) ;
    pPatPathoInfo2->setColonne(iColonne) ;
    pPPTArray->push_back(new NSPatPathoInfo(*pPatPathoInfo2)) ;

    pPatPathoInfo1->setColonne(iColonne + 1) ;
  }
  else
    pPatPathoInfo1->setColonne(iColonne) ;

  pPatPathoInfo1->setLigne(ligne) ;
  pPPTArray->push_back(new NSPatPathoInfo(*pPatPathoInfo1)) ;

  // on relance sur les node fils
  if (false == vect_val.empty())
  {
    for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
    {
      // apres parsing on est surs qu'il n'y a que des node
      CnodeExport *pCnode = dynamic_cast<CnodeExport *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPPTArray) ;
    }
  }
  return true ;
}


// -----------------------------------------------------------------------------
// classe CgoalExport// -----------------------------------------------------------------------------
CgoalExport::CgoalExport(string attributs, string values, NSContexte *pCtx, CmessageExport *pExp)
            :Cbalise(attributs, values)
{
  pContexte      = pCtx ;
  pMsgExport     = pExp ;
  pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

CgoalExport::~CgoalExport()
{
  if (pPatPathoArray)
    delete pPatPathoArray ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CnodeExport *pCNode = dynamic_cast<CnodeExport*>(pBalise) ;
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
CgoalExport::parser()
{
  int     nb_node ;
  ValIter ival ;

  // attributs
  if (!parser_attributs())
    return false ;

  /*
  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }
  */


	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

  nb_node = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      (*ival)->setObject(new CnodeExport((*ival)->getAttribute(), (*ival)->getValue(), 0)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }

      CnodeExport *pCnode = dynamic_cast<CnodeExport *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPatPathoArray) ;

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

boolCgoalExport::verif(){
  return true ;
}

bool
CgoalExport::compresser()
{
  return true ;
}

bool
CgoalExport::traiter()
{
  return true ;
}

bool
CgoalExport::createSFMGfiles(string sCodePat, string sCodeActe, string sIndexRC, string sIndexGoal)
{
  if (pPatPathoArray->empty())
    return true ;

  string sIndexGlobalRC = sCodeActe + sIndexRC ;
  string sIndexGlobalGoal = sCodeActe + sIndexGoal ;

  // Traitement des items
  PatPathoIter pptIter ;
  if (pPatPathoArray->CheminDansPatpatho("0CODE1", string(1, cheminSeparationMARK), &pptIter))
  {
    int iItemCol = (*pptIter)->getColonne() ;
    // int iItemId = 0 ;

    pptIter++ ;
    if ((pptIter != pPatPathoArray->end()) && ((*pptIter)->getColonne() > iItemCol))
    {
      if ((string("6MGDC1") == (*pptIter)->getLexique()) &&
          (string("") != (*pptIter)->getComplement()))
      {
        string sLibel = "???" ;
        if (pContexte)
        {
          NSEpiClassif *pClassif = new NSEpiClassif(pContexte->getSuperviseur()) ;
          pClassif->lastError = pClassif->open() ;
          if (pClassif->lastError == DBIERR_NONE)
          {
            string sCle = "6MGDC" + (*pptIter)->getComplement() ;
            pClassif->lastError = pClassif->chercheClef(&sCle, "", NODEFAULTINDEX, keySEARCHEQ, dbiREADLOCK) ;
            if (pClassif->lastError == DBIERR_NONE)
            {
              // récupération de l'enregistrement
              pClassif->lastError = pClassif->getRecord() ;
              if (pClassif->lastError == DBIERR_NONE)
                sLibel = pClassif->getLabel() ;
            }
          }
          pClassif->close() ;
          delete pClassif ;
        }
        else
        {
          // partie utilisant un fichier en texte uniquement remplaçant la table
          // classif (quand la méthode est appelée par exécutable externe)
          NSEpiClassifArray *pClassif = new NSEpiClassifArray("classif.txt") ;
          if (!pClassif->fillArray())
          {
            // problème lors de la récupération des données contenues dans
            // classif.txt
            erreur("Impossible d'ouvrir ou de récupérer les données du fichier classif.txt.", standardError, 0) ;
            delete pClassif ;
            return false ;
          }
          string  sClassif  = "6MGDC" ;
          string  sCode     = (*pptIter)->getComplement() ;
          bool  bResult = pClassif->searchKey(sClassif, sCode) ;
          if (bResult)
            sLibel = pClassif->getLabel() ;
          delete pClassif ;
        }

        string sDCLine   = sIndexGlobalGoal + "\t" + string("-999") + "\t" + (*pptIter)->getComplement() + "\t" + sLibel + "\t" + sCodeActe + "\t" ;
        pMsgExport->addToSFMGfile("26", sDCLine) ;
        string sDCRCLine = sIndexGlobalRC + "\t" + sIndexGlobalGoal ;
        pMsgExport->addToSFMGfile("22", sDCRCLine) ;
      }
    }
  }
  return true ;
}

// -----------------------------------------------------------------------------
//
// Implémentation des méthodes NSRefLink
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
//  Constructeur.
// ---------------------------------------------------------------------------
NSRefLinkData::NSRefLinkData()
{
  // Met les champs de données à zéro
  metAZero() ;
}


//---------------------------------------------------------------------------//  Constructeur copie
//---------------------------------------------------------------------------
NSRefLinkData::NSRefLinkData(const NSRefLinkData& rv)
{
  strcpy(classification,  rv.classification) ;
	strcpy(code, 	          rv.code) ;
	strcpy(chemin, 	        rv.chemin) ;
	strcpy(use_value, 	    rv.use_value) ;
	strcpy(value_code, 	    rv.value_code) ;
}


// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkData::operator=(NSRefLinkData src)
// Description  : Opérateur =
// Retour       : Référence sur l'objet cible
// -----------------------------------------------------------------------------
NSRefLinkData&
NSRefLinkData::operator=(const NSRefLinkData& src)
{
  if (&src == this)
  	return *this ;

  strcpy(classification,  src.classification) ;
	strcpy(code, 	          src.code) ;
	strcpy(chemin, 	        src.chemin) ;
	strcpy(use_value, 	    src.use_value) ;
	strcpy(value_code, 	    src.value_code) ;

	return (*this) ;
}

// -----------------------------------------------------------------------------// Fonction     : NSRefLinkData::operator==(NSRefLinkData& o)// Description  : Opérateur de comparaison
// Retour       : 0 ou 1
// -----------------------------------------------------------------------------
int
NSRefLinkData::operator==(const NSRefLinkData& o)
{
	if ((strcmp(classification, o.classification) == 0) &&
      (strcmp(code,           o.code) 		      == 0) &&
      (strcmp(chemin,         o.chemin) 		    == 0) &&
      (strcmp(use_value,      o.use_value) 		  == 0) &&
      (strcmp(value_code,     o.value_code)     == 0))
		return 1 ;
	else
		return 0 ;
}


// -----------------------------------------------------------------------------// Initialise les champs de données à zéro.// -----------------------------------------------------------------------------
void
NSRefLinkData::metAZero()
{
  // Met les champs de données à zéro
  memset(classification,  0, REFLINK_CLASSIFICATION_LEN + 1) ;
  memset(code,            0, REFLINK_CODE_LEN + 1) ;
  memset(chemin,          0, REFLINK_CHEMIN_LEN + 1) ;
  memset(use_value,       0, REFLINK_USE_VALUE_LEN + 1) ;
  memset(value_code,      0, REFLINK_VALUE_CODE_LEN + 1) ;
}


/*
// -----------------------------------------------------------------------------// Constructeur.// -----------------------------------------------------------------------------
NSRefLink::NSRefLink(NSContexte *pCtx)
          :NSFiche(pCtx)
{
  // Création d'un objet de données
  pDonnees = new NSRefLinkData() ;
}


// -----------------------------------------------------------------------------// Destructeur.// -----------------------------------------------------------------------------
NSRefLink::~NSRefLink()
{
  // Détruit l'objet de données
  if (pDonnees)
    delete pDonnees ;
}


// -----------------------------------------------------------------------------
// Transfère le contenu de pRecBuff dans les variables de la fiche
// -----------------------------------------------------------------------------
void
NSRefLink::alimenteFiche()
{
  alimenteChamp(pDonnees->classification, REFLINK_CLASSIFICATION_FIELD,   REFLINK_CLASSIFICATION_LEN) ;
  alimenteChamp(pDonnees->code, 		      REFLINK_CODE_FIELD,		          REFLINK_CODE_LEN) ;
  alimenteChamp(pDonnees->chemin, 		    REFLINK_CHEMIN_FIELD,	          REFLINK_CHEMIN_LEN) ;
  alimenteChamp(pDonnees->use_value, 		  REFLINK_USE_VALUE_FIELD,		    REFLINK_USE_VALUE_LEN) ;
  alimenteChamp(pDonnees->value_code,     REFLINK_VALUE_CODE_FIELD,       REFLINK_VALUE_CODE_LEN) ;
}


// -----------------------------------------------------------------------------// Transfère le contenu des valeurs de la fiche dans pRecBuff// -----------------------------------------------------------------------------
void
NSRefLink::videFiche()
{
  videChamp(pDonnees->classification, REFLINK_CLASSIFICATION_FIELD, REFLINK_CLASSIFICATION_LEN) ;
  videChamp(pDonnees->code, 		      REFLINK_CODE_FIELD,		        REFLINK_CODE_LEN) ;
  videChamp(pDonnees->chemin, 		    REFLINK_CHEMIN_FIELD,	        REFLINK_CHEMIN_LEN) ;
  videChamp(pDonnees->code, 		      REFLINK_USE_VALUE_FIELD,		  REFLINK_USE_VALUE_LEN) ;
  videChamp(pDonnees->chemin, 		    REFLINK_VALUE_CODE_FIELD,	    REFLINK_VALUE_CODE_LEN) ;
}


// -----------------------------------------------------------------------------
// Ouvre la table
// -----------------------------------------------------------------------------
DBIResult
NSRefLink::open()
{
  char tableName[] = "NSREFLINK.DB" ;

  // Appelle la fonction open() de la classe de base pour ouvrir l'index primaire
  lastError = NSFiche::open(tableName, NSF_PARTAGE_GLOBAL) ;
  return (lastError) ;
}


bool
NSRefLink::GetAllCodesAndValues(NSPatPathoArray *pPatPathoArray, string sClassification, VecteurString *pResult, bool bEmptyResult)
{
try
{
  if ((!pPatPathoArray) || (!pResult))
    return false ;

  if (bEmptyResult)
    pResult->empty() ;

  if (sClassification != "")
  {
    // Positionnement sur la première fiche de la classification
    lastError = chercheClef(&sClassification, "", 0, keySEARCHGEQ, dbiWRITELOCK) ;
  }
  else
    lastError = debut(dbiWRITELOCK) ;

  if (lastError != DBIERR_NONE)
    return true ;

  lastError = getRecord() ;
  if (lastError != DBIERR_NONE)
  {
    erreur("Le fichier NSREFLINK est défectueux.", standardError, lastError) ;
    return false ;
  }

  // On vérifie que la fiche trouvée appartient bien à la classification
  if ((sClassification != "") && (strcmp(pDonnees->classification, sClassification.c_str()) != 0))
    return true ;

	string sLang = "" ;
	if (pContexte->getUtilisateur())
		sLang = pContexte->getUserLanguage() ;

  // On avance dans le fichier tant que les fiches trouvées appartiennent à la
  // classification
  while ((lastError != DBIERR_EOF) && ((sClassification == "") || (strcmp(pDonnees->classification, sClassification.c_str()) == 0)))
  {
    string sSearchType = string(pDonnees->use_value) ;

    // Retour d'une valeur chiffrée
    if ((sSearchType == "1") || (sSearchType == "3"))
    {
      string sValeur  = "" ;
      string sUnite   = "" ;
      if (pPatPathoArray->CheminDansPatpatho(0, string(pDonnees->chemin), &sValeur, &sUnite, "|"))
      {
        string sUniteSens = "" ;
        pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
        if ((sValeur != "") &&(strcmp(pDonnees->value_code, sUniteSens.c_str()) == 0))
        {
          if (sSearchType == "3")
          {
            string sDate    = "" ;
            string sIntro   = "" ;
            donne_date_breve(sValeur, &sDate, &sIntro, sLang) ;
            sValeur = sDate ;
          }

          string sResult = string(pDonnees->code) + string("|") + sValeur ;
          pResult->push_back(new string(sResult)) ;
        }
      }
    }
    // Liste
    else if (sSearchType == "2")
    {
      PatPathoIter pptIter ;
      if (pPatPathoArray->CheminDansPatpatho(string(pDonnees->chemin), "|", &pptIter))
      {
        // Parsing de la chaine de liste
        VecteurString *pChaine  = new VecteurString() ;
        VecteurString *pCode    = new VecteurString() ;
        string sListe = string(pDonnees->value_code) ;
        size_t iOpen = sListe.find('(') ;
        while (iOpen != string::npos)
        {
          size_t iClose = sListe.find(')', iOpen) ;
          if (iClose == string::npos)
            break ;

          string sPartListe = string(sListe, iOpen + 1, iClose - iOpen - 1) ;
          size_t iOpenChaine = sPartListe.find('[') ;
          if (iOpenChaine != string::npos)
          {
            size_t iCloseChaine = sPartListe.find(']', iOpenChaine) ;
            if (iCloseChaine != string::npos)
            {
              string  sChaine  = "" ;
              string  sCode    = "" ;
              size_t  iPartLength = strlen(sPartListe.c_str()) ;
              if (iCloseChaine > iOpenChaine + 1)
                sChaine = string(sPartListe, iOpenChaine + 1, iCloseChaine - iOpenChaine - 1) ;
              if (iCloseChaine < iPartLength - 1)
                sCode = string(sPartListe, iCloseChaine + 1, iPartLength - iCloseChaine - 1) ;

              if (sCode != "")
              {
                pChaine->push_back(new string(sChaine)) ;
                pCode->push_back(new string(sCode)) ;
              }
            }
          }

          size_t iLength = strlen(sListe.c_str()) ;
          if (iClose < iLength - 1)
          {
            sListe = string(sListe, iClose + 1, iLength - iClose - 1) ;
            iOpen = sListe.find('(') ;
          }
          else
          {
            sListe = "" ;
            iOpen = string::npos ;
          }
        }

        // Recherche
        if (!(pChaine->empty()))
        {
          string sDefautCode      = "" ;
          string sSelectedCode    = "" ;

          EquiItemIter itChaine   = pChaine->begin();
          EquiItemIter itCode     = pCode->begin();
          for ( ; (sSelectedCode == "") && (itChaine != pChaine->end()) && (itCode != pCode->end()) ; itChaine++, itCode++)
          {
            if (**itChaine == "")
              sDefautCode = **itCode ;
            else
            {
              // valeur chiffrée ?
              size_t iPosValue = (*itChaine)->find("/$") ;
              if (iPosValue == string::npos)
              {
                NSPatPathoArray *pSsPatPatho = new NSPatPathoArray(pContexte) ;
                pPatPathoArray->ExtrairePatPatho(pptIter, pSsPatPatho) ;
                if (!(pSsPatPatho->empty()))
                {
                  PatPathoIter pptSsIter ;
                  if (pSsPatPatho->CheminDansPatpatho(**itChaine, "|", &pptSsIter))
                    sSelectedCode = **itCode ;
                }
                delete pSsPatPatho ;
              }
              else
              {
                string sValue   = string(**itChaine, iPosValue+2, strlen((*itChaine)->c_str())-iPosValue-2) ;
                string sDeb     = string(**itChaine, 0, iPosValue) ;
                string sUnit    = "" ;
                string sPath    = "" ;

                for (iPosValue = strlen(sDeb.c_str()) - 1 ; (iPosValue > 0) && (sDeb[iPosValue] != '|') ; iPosValue--)
                  ;

                if (sDeb[iPosValue] == '|')
                {
                  sPath = string(sDeb, 0, iPosValue) ;
                  sUnit = string(sDeb, iPosValue + 1, strlen(sDeb.c_str()) - iPosValue - 1) ;
                  NSPatPathoArray *pSsPatPatho = new NSPatPathoArray(pContexte) ;
                  pPatPathoArray->ExtrairePatPatho(pptIter, pSsPatPatho) ;
                  if (!(pSsPatPatho->empty()))
                  {
                    // PatPathoIter pptSsIter ;
                    string sValeur  = "" ;
                    string sUnite   = "" ;
                    if (pSsPatPatho->CheminDansPatpatho(0, sPath, &sValeur, &sUnite, "|"))
                    {
                      string sUniteSens = "" ;
                      pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
                      if ((sValeur == sValue) && (sUnit == sUniteSens))
                        sSelectedCode = **itCode ;
                    }
                  }
                  delete pSsPatPatho ;
                }
                else
                {
                  sUnit = sDeb ;
                  string sValeur  = "" ;
                  string sUnite   = "" ;
                  if (pPatPathoArray->CheminDansPatpatho(0, string(pDonnees->chemin), &sValeur, &sUnite, "|"))
                  {
                    string sUniteSens = "" ;
                    pContexte->getDico()->donneCodeSens(&sUnite, &sUniteSens) ;
                    if ((sValeur == sValue) && (sUnit == sUniteSens))
                      sSelectedCode = **itCode ;
                  }
                }
              }
            }
          }
          if      (sSelectedCode != "")
          {
            string sResult = string(pDonnees->code) + string("|") + sSelectedCode ;
            pResult->push_back(new string(sResult)) ;
          }
          else if (sDefautCode != "")
          {
            string sResult = string(pDonnees->code) + string("|") + sDefautCode ;
            pResult->push_back(new string(sResult)) ;
          }
        }
        delete pChaine ;
        delete pCode ;
      }
    }

    lastError = suivant(dbiWRITELOCK) ;
    if ((lastError != DBIERR_NONE) && (lastError != DBIERR_EOF))
    {
      erreur("Pb d'acces à la fiche suivante dans NSREFLINK.", standardError, lastError) ;
      return false ;
    }

    if (lastError != DBIERR_EOF)
    {
      lastError = getRecord() ;
      if (lastError != DBIERR_NONE)
      {
        erreur("Le fichier NSREFLINK est défectueux.", standardError, lastError) ;
        return false ;
			}
    }
  }
  return true ;
}
catch (...)
{
    erreur("Exception NSRefLink::GetAllCodesAndValues.", standardError, 0);
    return false ;
}
}
*/

// -----------------------------------------------------------------------------//// Implémentation des méthodes NSRefLinkInfo
//
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkInfo::NSRefLinkInfo()
// Description  : Constructeur par défaut
// Retour       : Rien
// -----------------------------------------------------------------------------
NSRefLinkInfo::NSRefLinkInfo()
{
	// Crée l'objet de données
	pDonnees = new NSRefLinkData() ;
}


// -----------------------------------------------------------------------------
//  Fonction    : NSRefLinkInfo::NSRefLinkInfo(NSRefLink *)
//  Description : Constructeur à partir d'un NSRefLink
//  Retour      : Rien
// -----------------------------------------------------------------------------
/*
NSRefLinkInfo::NSRefLinkInfo(NSRefLink *pRefLink)
{
	// Crée l'objet de données
	pDonnees = new NSRefLinkData() ;

	// Copie les valeurs du NSRefLink
	*pDonnees = *(pRefLink->pDonnees) ;
}
*/

// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkInfo::~NSRefLinkInfo()
// Description  : Destructeur
// Retour       : Rien
// -----------------------------------------------------------------------------
NSRefLinkInfo::~NSRefLinkInfo()
{
  if (pDonnees)
	  delete pDonnees ;
}

// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkInfo::NSRefLinkInfo(NSRefLinkInfo& rv)
// Description  : Constructeur copie
// Retour       : Rien
// -----------------------------------------------------------------------------
NSRefLinkInfo::NSRefLinkInfo(const NSRefLinkInfo& rv)
{
	// Crée l'objet de données
	pDonnees = new NSRefLinkData() ;

	// Copie les valeurs du NSRefLinkInfo d'origine
	*pDonnees = *(rv.pDonnees) ;
}

// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkInfo::operator=(NSRefLinkInfo src)
// Description  : Opérateur d'affectation
// Retour       : Référence de l'objet cible
// -----------------------------------------------------------------------------
NSRefLinkInfo&
NSRefLinkInfo::operator=(const NSRefLinkInfo& src)
{
  if (&src == this)
  	return *this ;

	*pDonnees = *(src.pDonnees) ;

	return *this ;
}

// -----------------------------------------------------------------------------
// Fonction     : NSRefLinkInfo::operator==(NSRefLinkInfo src)
// Description  : Opérateur de comparaison
// Retour       : Référence de l'objet cible
// -----------------------------------------------------------------------------
int
NSRefLinkInfo::operator==(const NSRefLinkInfo& o)
{
  return (*pDonnees == *(o.pDonnees)) ;
}

// -----------------------------------------------------------------------------
// classe NSRefLinkInfoArray
// -----------------------------------------------------------------------------
/*
typedef vector<NSRefLinkInfo *>       NSRefLinkInfoVector ;
typedef NSRefLinkInfoVector::iterator NSRefLinkInfoIter ;
NSRefLinkData     *pDonnees ;
string            sFileName ;
*/

NSRefLinkInfoArray::NSRefLinkInfoArray(NSContexte *pCtx, string sFile)
                   :NSRefLinkInfoVector()
{
  sFileName = sFile ;
  pContexte = pCtx ;
}

NSRefLinkInfoArray::NSRefLinkInfoArray(NSContexte *pCtx)
                   :NSRefLinkInfoVector()
{
  sFileName = "" ;
  pContexte = pCtx ;
}

NSRefLinkInfoArray::~NSRefLinkInfoArray()
{
  vider() ;
}

bool
NSRefLinkInfoArray::fillArray()
{
  ifstream  inFile ;
  string    line ;

  inFile.open(sFileName.c_str()) ;
  if (!inFile)
    return false ;

  while (!inFile.eof())
  {
    getline(inFile, line) ;
    if (line != "")
    {
      // on ajoute dans la reflinkarray ce qu'on vient de lire dans le fichier
      string  sClassif  = "" ;
      string  sCode     = "" ;
      string  sChemin   = "" ;
      string  sUseVal   = "" ;
      string  sValCode  = "" ;

      // recherche de la classif
      size_t  posBeg = line.find("\t") ;
      size_t  posEnd = line.find("\t", posBeg + 1) ;
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sClassif = string(line, posBeg + 1, posEnd - (posBeg + 1)) ;
      else
        return false ;

      // recherche du code
      posBeg  = posEnd ;
      posEnd  = line.find("\t", posBeg + 1) ;
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sCode = string(line, posBeg + 1, posEnd - (posBeg + 1)) ;
      else
        return false ;

      // recherche du chemin
      posBeg  = posEnd ;
      posEnd  = line.find("\t", posBeg + 1) ;
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sChemin = string(line, posBeg + 1, posEnd - (posBeg + 1)) ;
      else
        return false ;

      // recherche du use_value
      posBeg  = posEnd ;
      posEnd  = line.find("\t", posBeg + 1) ;
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sUseVal = string(line, posBeg + 1, posEnd - (posBeg + 1)) ;
      else
        return false ;

      // recherche du value_code
      posBeg  = posEnd ;
      posEnd  = strlen(line.c_str()) ;
      if ((posEnd != string::npos) && (posBeg != string::npos) && (posBeg < posEnd))
        sValCode = string(line, posBeg + 1, posEnd - (posBeg + 1)) ;
      else
        return false ;

      if ((sClassif != "") && (sCode != "") && (sChemin != "") && (sUseVal != "") && (sValCode != ""))
      {
        NSRefLinkInfo *pRefInfoTemp = new NSRefLinkInfo() ;
        strcpy(pRefInfoTemp->pDonnees->classification, sClassif.c_str()) ;
        strcpy(pRefInfoTemp->pDonnees->code,           sCode.c_str()) ;
        strcpy(pRefInfoTemp->pDonnees->chemin,         sChemin.c_str()) ;
        strcpy(pRefInfoTemp->pDonnees->use_value,      sUseVal.c_str()) ;
        strcpy(pRefInfoTemp->pDonnees->value_code,     sValCode.c_str()) ;

        push_back(new NSRefLinkInfo(*pRefInfoTemp)) ;
        delete pRefInfoTemp ;
      }
      else
        return false ;
    }
  }
  inFile.close() ;
  return true ;
}

bool
NSRefLinkInfoArray::GetAllCodesAndValues(NSPatPathoArray *pPatPathoArray, string sClassification, VecteurString *pResult, bool bEmptyResult)
{
try
{
  if ((NULL == pPatPathoArray) || (NULL == pResult))
    return false ;

  if (bEmptyResult)
    pResult->empty() ;

  if (empty())
    return true ;

  // on créé une array de NSRefLinkInfo temporaire contenant l'ensemble des
  // NSRefLinkInfo correspondant à la classification souhaitée
  NSRefLinkInfoArray  *pRefInfoArrayTemp = new NSRefLinkInfoArray(pContexte) ;
  if (sClassification != "")
  {
    for (NSRefLinkInfoIter iter = begin() ; iter != end() ; iter++)
    {
      if (strcmp((*iter)->pDonnees->classification, sClassification.c_str()) == 0)
        pRefInfoArrayTemp->push_back(new NSRefLinkInfo(**iter)) ;
    }
  }
  else
  {
    // si la classification n'est pas précisé, on prend tous les enregistrements
    for (NSRefLinkInfoIter iter = begin() ; iter != end() ; iter++)
      pRefInfoArrayTemp->push_back(new NSRefLinkInfo(**iter)) ;
  }

  if (pRefInfoArrayTemp->empty())
  {
    pRefInfoArrayTemp->vider() ;
    delete pRefInfoArrayTemp ;
    return true ;
  }

	string sLang = pContexte->getUserLanguage() ;

  // on parcourt l'ensemble des enregistrements qui correspondent à la
  // classification souhaitée
  NSDico *pDico = new NSDico(NULL) ;
  for (NSRefLinkInfoIter iter = pRefInfoArrayTemp->begin() ; pRefInfoArrayTemp->end() != iter ; iter++)
  {
    string sSearchType = string((*iter)->pDonnees->use_value) ;

    if ((sSearchType == "1") || (sSearchType == "3"))
    {
      string  sValeur = "" ;
      string  sUnite  = "" ;
      if (pPatPathoArray->CheminDansPatpatho(0, string((*iter)->pDonnees->chemin), &sValeur, &sUnite, string(1, cheminSeparationMARK)))
      {
        string  sUniteSens  = "" ;
        pDico->donneCodeSens(&sUnite, &sUniteSens) ;
        if ((sValeur != "") && (strcmp((*iter)->pDonnees->value_code, sUniteSens.c_str()) == 0))
        {
          if (sSearchType == "3")
          {
            string  sDate   = "" ;
            string  sIntro  = "" ;
            donne_date_breve(sValeur, &sDate, &sIntro, sLang) ;
            sValeur         = sDate ;
          }
          string  sResult   = string((*iter)->pDonnees->code) + string("|") + sValeur ;
          pResult->AddString(sResult) ;
        }
      }
    }
    else if (sSearchType == "2")
    {
      PatPathoIter pptIter ;
      if (pPatPathoArray->CheminDansPatpatho(string((*iter)->pDonnees->chemin), string(1, cheminSeparationMARK), &pptIter))
      {
        // Parsing de la chaine de liste
        VecteurString aChaine ;
        VecteurString aCode ;
        string sListe = string((*iter)->pDonnees->value_code) ;
        size_t iOpen = sListe.find('(') ;
        while (iOpen != string::npos)
        {
          size_t iClose = sListe.find(')', iOpen) ;
          if (iClose == string::npos)
            break ;

          string sPartListe = string(sListe, iOpen + 1, iClose - iOpen - 1) ;
          size_t iOpenChaine = sPartListe.find('[') ;
          if (iOpenChaine != string::npos)
          {
            size_t iCloseChaine = sPartListe.find(']', iOpenChaine) ;
            if (iCloseChaine != string::npos)
            {
              string  sChaine  = "" ;
              string  sCode    = "" ;
              size_t  iPartLength = strlen(sPartListe.c_str()) ;
              if (iCloseChaine > iOpenChaine + 1)
                sChaine = string(sPartListe, iOpenChaine + 1, iCloseChaine - iOpenChaine - 1) ;
              if (iCloseChaine < iPartLength - 1)
                sCode = string(sPartListe, iCloseChaine + 1, iPartLength - iCloseChaine - 1) ;

              if (string("") != sCode)
              {
                aChaine.AddString(sChaine) ;
                aCode.AddString(sCode) ;
              }
            }
          }

          size_t iLength = strlen(sListe.c_str()) ;
          if (iClose < iLength - 1)
          {
            sListe = string(sListe, iClose + 1, iLength - iClose - 1) ;
            iOpen = sListe.find('(') ;
          }
          else
          {
            sListe = "" ;
            iOpen = string::npos ;
          }
        }

        // Recherche
        if (false == aChaine.empty())
        {
          string sDefautCode   = string("") ;
          string sSelectedCode = string("") ;

          EquiItemIter itChaine = aChaine.begin() ;
          EquiItemIter itCode   = aCode.begin() ;
          for ( ; (sSelectedCode == "") && (aChaine.end() != itChaine) && (aCode.end() != itCode) ; itChaine++, itCode++)
          {
            if (**itChaine == "")
              sDefautCode = **itCode ;
            else
            {
              // valeur chiffrée ?
              size_t iPosValue = (*itChaine)->find("/$") ;
              if (iPosValue == string::npos)
              {
                NSPatPathoArray SsPatPatho((NSSuper*) NULL) ;
                pPatPathoArray->ExtrairePatPatho(pptIter, &SsPatPatho) ;
                if (false == SsPatPatho.empty())
                {
                  PatPathoIter pptSsIter ;
                  if (SsPatPatho.CheminDansPatpatho(**itChaine, string(1, cheminSeparationMARK), &pptSsIter))
                    sSelectedCode = **itCode ;
                }
              }
              else
              {
                string sValue   = string(**itChaine, iPosValue + 2, strlen((*itChaine)->c_str()) - iPosValue - 2) ;
                string sDeb     = string(**itChaine, 0, iPosValue) ;
                string sUnit    = "" ;
                string sPath    = "" ;
                for (iPosValue = strlen(sDeb.c_str()) - 1 ; (iPosValue > 0) && (sDeb[iPosValue] != '|') ; iPosValue--)
                  ;
                if (sDeb[iPosValue] == '|')
                {
                  sPath = string(sDeb, 0, iPosValue) ;
                  sUnit = string(sDeb, iPosValue + 1, strlen(sDeb.c_str()) - iPosValue - 1) ;
                  NSPatPathoArray SsPatPatho((NSSuper*) NULL) ;
                  pPatPathoArray->ExtrairePatPatho(pptIter, &SsPatPatho) ;
                  if (false == SsPatPatho.empty())
                  {
                    // PatPathoIter pptSsIter ;
                    string sValeur  = "" ;
                    string sUnite   = "" ;
                    if (SsPatPatho.CheminDansPatpatho(0, sPath, &sValeur, &sUnite, string(1, cheminSeparationMARK)))
                    {
                      string sUniteSens = "" ;
                      pDico->donneCodeSens(&sUnite, &sUniteSens) ;
                      if ((sValeur == sValue) && (sUnit == sUniteSens))
                        sSelectedCode = **itCode ;
                    }
                  }
                }
                else
                {
                  sUnit = sDeb ;
                  string sValeur  = "" ;
                  string sUnite   = "" ;
                  if (pPatPathoArray->CheminDansPatpatho(0, string((*iter)->pDonnees->chemin), &sValeur, &sUnite, string(1, cheminSeparationMARK)))
                  {
                    string sUniteSens = "" ;
                    pDico->donneCodeSens(&sUnite, &sUniteSens) ;
                    if ((sValeur == sValue) && (sUnit == sUniteSens))
                      sSelectedCode = **itCode ;
                  }
                }
              }
            }
          }

          if      (string("") != sSelectedCode)
          {
            string sResult = string((*iter)->pDonnees->code) + string("|") + sSelectedCode ;
            pResult->AddString(sResult) ;
          }
          else if (string("") != sDefautCode)
          {
            string sResult = string((*iter)->pDonnees->code) + string("|") + sDefautCode ;
            pResult->AddString(sResult) ;
          }
        }
      }
    }
  }
  return true ;
}
catch (...)
{
  erreur("Exception NSRefLink::GetAllCodesAndValues.", standardError, 0) ;
  return false ;
}
}

bool
NSRefLinkInfoArray::vider()
{
  if (empty())
    return true ;

  for (NSRefLinkInfoIter iter = begin() ; end() != iter ; )
  {
    delete (*iter) ;
    erase(iter) ;
  }
  return true ;
}


// -----------------------------------------------------------------------------
// classe nsExportParseurOld
// -----------------------------------------------------------------------------
nsExportParseurOld::nsExportParseurOld(NSContexte *pCtx, bool bVerb)
{
  pMessage    = 0 ;
  pContexte   = pCtx ;
  bParsingOk  = false ;
  bVerbose    = bVerb ;
}

nsExportParseurOld::~nsExportParseurOld()
{
  if (pMessage)
    delete pMessage ;
}

bool
nsExportParseurOld::open(string sXmlFileName)
{
  ifstream  xml_file ;
	string    tmp_xml_string ;
	string    xml_string ;
	// int       counter ;
	bool      reponse ;

	xml_file.open(sXmlFileName.c_str()) ;
  if (!xml_file)
  {
    string sErreur = string("Impossible d'ouvrir le fichier XML (") + sXmlFileName + string(")") ;
    erreur(sErreur.c_str(), standardError, 0) ;
    return false ;
  }

	while (getline(xml_file, tmp_xml_string))
  {
    // <pmed_patient patientid="200001>

    if (string(tmp_xml_string, 0, 24) == "<pmed_patient patientid=")
    {
      size_t  iLen = strlen(tmp_xml_string.c_str()) ;
      tmp_xml_string = string(tmp_xml_string, 0, iLen - 1) + "\">" ;
    }

    bool bTourner = true ;
    while (bTourner)
    {
      size_t  iInf6 = tmp_xml_string.find("complement=\"<6\"") ;
      size_t  i6a10 ;
      size_t  iSu10 ;
      if (iInf6 == string::npos)
        i6a10 = tmp_xml_string.find("complement=\"6<=<=10\"") ;
      if ((iInf6 == string::npos) && (i6a10 == string::npos))
        iSu10 = tmp_xml_string.find("complement=\">10\"") ;

      int     iTailleInf6 = 15 ;
      int     iTaille6a10 = 20 ;
      int     iTailleSu10 = 16 ;

      if ((iInf6 == string::npos) && (i6a10 == string::npos) && (iSu10 == string::npos))
        break ;

      size_t iTaille = strlen(tmp_xml_string.c_str()) ;

      if      (iInf6 != string::npos)
        tmp_xml_string = string(tmp_xml_string, 0, iInf6) + string("complement=\"&lt;6\"")
                          + string(tmp_xml_string, iInf6 + iTailleInf6, iTaille - iInf6 - iTailleInf6) ;

      else if (i6a10 != string::npos)
        tmp_xml_string = string(tmp_xml_string, 0, i6a10) + string("complement=\"6&lt;=&lt;=10\"")
                          + string(tmp_xml_string, i6a10 + iTaille6a10, iTaille - i6a10 - iTaille6a10) ;

      else if (iSu10 != string::npos)
        tmp_xml_string = string(tmp_xml_string, 0, iSu10) + string("complement=\"&gt;10\"")
                          + string(tmp_xml_string, iSu10 + iTailleSu10, iTaille - iSu10 - iTailleSu10) ;
    }

		xml_string += tmp_xml_string + '\n' ;
  }
	xml_file.close() ;

  xml_string = string("<export>\n") + xml_string + string("</export>\n") ;

  // xml_string = TexteXmlVersNormal(xml_string);

	reponse = pre_parseur(&xml_string) ;
	if (reponse == false)
		return false ;

  int iParsingError ;
	if (parse_message(xml_string, string("export"), &iParsingError) == false)
  {
    if (bVerbose)
    {
      string sErrParse = parseErrorMessage(iParsingError) ;
      string sErreur = string("Le message est incorrect : ") + sErrParse ;
      erreur(sErreur.c_str(), standardError, 0) ;
    }
    return false ;
  }

  bParsingOk = true ;
  return true ;
}

bool
nsExportParseurOld::parse_message(string arc, string tag, int *iParsingError)
{
try
{
  size_t  arc_start, arc_end, header_end, attrib_start ;
  string  attributes, values ;

  arc_start = arc.find("<" + tag) ;
  if (arc_start != string::npos)
  {
    arc_end = arc.find ("</" + tag + ">") ;
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
    attrib_start = arc.find_first_not_of(' ', arc_start + tag.length() + 1) ;

    attributes  = string(arc, attrib_start, header_end - attrib_start) ;
    values      = string(arc, header_end + 1, arc_end - header_end - 1) ;
  }
  else
  {
    // erreur (pas de balise de debut)
    *iParsingError = EARCHETYPE_BALISE_DEBUT ;
    return false ;
  }

  pMessage = new CmessageExportOld(attributes, values, pContexte) ;

  if (!pMessage->parser())
  {
    *iParsingError = pMessage->getParsingError() ;
    return false ;
  }

  return true ;
}
catch (...)
{
  erreur("Exception nsExportParseurOld::parse_message.", standardError, 0) ;
  return false ;
}
}


// -----------------------------------------------------------------------------
// classe CmessageExport// -----------------------------------------------------------------------------CmessageExportOld::CmessageExportOld(string attributs, string values, NSContexte *pCtx, string sMedId)                  :Cbalise(attributs, values)
{
  pContexte = pCtx ;
  sUserId   = sMedId ;
}

CmessageExportOld::~CmessageExportOld()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CpatientExportOld *pCPat = dynamic_cast<CpatientExportOld*>(pBalise) ;
      if (pCPat)
      {
        delete pCPat ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      delete (*ival) ;
      vect_val.erase(ival) ;
    }
  }
*/
}

bool
CmessageExportOld::parser()
{
  int       nb_patients ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

  nb_patients    = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_PATIENT) // dialogue
		{
      (*ival)->setObject(new CpatientExportOld((*ival)->getAttribute(), (*ival)->getValue(), pContexte, this)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_patients++ ;
		}
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

	return true ;
}boolCmessageExportOld::verif(){
  return true ;
}


bool
CmessageExportOld::compresser()
{
  return true ;
}


bool
CmessageExportOld::traiter()
{
  return true ;
}
boolCmessageExportOld::addToSFMGfile(string sFileIndex, string sNewLine){  ofstream    outFile ;  string      sFileName = sUserId ;  string      sPathName = sPathMsg ;  if (pContexte)  {    if (sUserId == "")      sFileName = pContexte->getEpisodus()->sPrometheUserId ;    sPathName = pContexte->PathName("BGLO") ;  }
  if (sFileName == "")
    return false ;

  sFileName += string("D") + sFileIndex + string(".txt") ;
  string sFichier = sPathName + sFileName ;

/*
  outFile.open(sFichier.c_str(), ios::ate | ios::app);
  if (pContexte)  {    sFichier = pContexte->getSuperviseur()->pEpisodus->sPrometheUserId + string("D") + sFileIndex + string("_V0.txt") ;    sFichier = pContexte->PathName("BGLO") + sFichier ;
  }
  else
  {
    string  sID = "" ;
    Cbalise *bigFather = parent ;
    while ((bigFather->parent) && (bigFather->databaseid != ""))
      bigFather = bigFather->parent ;

    string  sFichier = "" ;
    if (bigFather->databaseid != "")
      sFichier = bigFather->databaseid + string("D") + sFileIndex + string("_V0.txt") ;
    else
    {
      string sErrMess = string("CmessageExporOld::addToSFMGfile - Impossible de trouver l'identifiant du propriétaire du message.") ;
      erreur(sErrMess.c_str(), 0, 0) ;
      return false ;
    }
  }
*/

  outFile.open(sFichier.c_str(), ios::ate | ios::app) ;
  if (!outFile)
  {
    string sErrMess = string("Erreur d'ouverture en écriture du fichier ") + sFichier ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return false ;
  }

  outFile << (sNewLine + string("\n")) ;

  outFile.close() ;
  return true ;}// -----------------------------------------------------------------------------//                           CpatientExportOld// -----------------------------------------------------------------------------CpatientExportOld::CpatientExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp)                  :Cbalise(attributs, values)
{
  pMsgExport    = pExp ;
  pExp->parent  = this ;
  pContexte     = pCtx ;
}

CpatientExportOld::~CpatientExportOld()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CadminDataExportOld *pCAdm = dynamic_cast<CadminDataExportOld*>(pBalise) ;
      if (pCAdm)
      {
        delete pCAdm ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        ChealthDataExportOld *pCHlth = dynamic_cast<ChealthDataExportOld*>(pBalise) ;
        if (pCHlth)
        {
          delete pCHlth ;
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
CpatientExportOld::parser()
{
  int       nb_admindata, nb_healthdata ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

  nb_admindata    = 0 ;
  nb_healthdata   = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_ADMINDATA) // dialogue
		{
      (*ival)->setObject(new CadminDataExportOld((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
			nb_admindata++ ;
		}
    else if ((*ival)->getLabel() == LABEL_EX_HEALTHDATA) // references
    {
      (*ival)->setObject(new ChealthDataExportOld((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
      nb_healthdata++ ;
    }
		else
    {
      // erreur (valeur incorrecte)
      _iParsingError = EARCHETYPE_VAL ;
      return false ;
    }
  }

	if ((nb_admindata < 1) || (nb_healthdata < 1))
  {
    // erreur (nb valeurs incorrect)
    _iParsingError = EARCHETYPE_NBVAL ;
    return false ;
  }

	return true ;
}boolCpatientExportOld::verif(){
  return true ;
}

bool
CpatientExportOld::compresser()
{
  return true ;
}

bool
CpatientExportOld::traiter()
{
  return true ;
}

string
CpatientExportOld::getID()
{
  return getStringAttribute(ATTRIBUT_EX_PATIENT_ID) ;
}

string
CpatientExportOld::getCode()
{
  return getStringAttribute(ATTRIBUT_EX_PATIENT_CODE) ;
}

bool
CpatientExportOld::createSFMGfiles(int iNumActe)
{
  string sPatLine     = "" ;
  string sPatIndice   = donnePatIndice() ;
  string sActeIndice  = donneActeIndice(iNumActe) ;

  if (vect_val.empty())
    return false ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      ((*ival)->getLabel() == LABEL_EX_ADMINDATA) // dialogue
		{
      CadminDataExportOld *pAdmin = dynamic_cast<CadminDataExportOld *>((*ival)->getObject()) ;
      if (pAdmin)
      {
        bool bAlreadyExist = false ;
        if (false == pMsgExport->aVecteurPatients.empty())
        {
          EquiItemIter iStrIter = pMsgExport->aVecteurPatients.begin() ;
          for ( ; (pMsgExport->aVecteurPatients.end() != iStrIter) && (**iStrIter != sPatIndice) ; iStrIter++)
            ;
          if (iStrIter != pMsgExport->aVecteurPatients.end())
            bAlreadyExist = true ;
        }

        if (false == bAlreadyExist)
        {
          sPatLine = pAdmin->donneNaissance() + "\t" + sPatIndice + "\t" + pAdmin->getSexe() ;
          pMsgExport->addToSFMGfile("08", sPatLine) ;
          pMsgExport->aVecteurPatients.push_back(new string(sPatIndice)) ;
        }
      }
		}
    else if ((*ival)->getLabel() == LABEL_EX_HEALTHDATA) // references
    {
      ChealthDataExportOld *pHealth = dynamic_cast<ChealthDataExportOld *>((*ival)->getObject()) ;
      if (pHealth)
        pHealth->createSFMGfiles(sPatIndice, sActeIndice) ;
    }
  }
  return true ;
}


string
CpatientExportOld::donnePatIndice()
{
  string sIndice = getID() ;

#ifndef _MUE
/*
  NSPatient *pPatient = new NSPatient(pContexte) ;
  pPatient->lastError = pPatient->open() ;
  if (pPatient->lastError != DBIERR_NONE)
  {
    erreur("Erreur à l'ouverture de la base des patients.", 0, 0, pContexte->GetMainWindow()->GetHandle()) ;
    delete pPatient ;
  }
  else
  {
    pPatient->lastError = pPatient->chercheClef(&sIndice, "", 0, keySEARCHEQ, dbiWRITELOCK) ;
    if ((pPatient->lastError != DBIERR_NONE) && (pPatient->lastError != DBIERR_RECNOTFOUND))
    {
      erreur("Erreur à la recherche dans la base des patients.", 0, pPatient->lastError, pContexte->GetMainWindow()->GetHandle()) ;
      pPatient->close() ;
      delete pPatient ;
    }
    else if (pPatient->lastError == DBIERR_RECNOTFOUND)
    {
      erreur("Impossible de retrouver les données administratives du patient.", 0, 0, pContexte->GetMainWindow()->GetHandle()) ;
      pPatient->close() ;
      delete pPatient ;
    }
    else
    {
      pPatient->lastError = pPatient->getRecord() ;
      if (pPatient->lastError != DBIERR_NONE)
      {
        erreur("Erreur à la lecture dans la base des patients.", 0, pPatient->lastError, pContexte->GetMainWindow()->GetHandle()) ;
        pPatient->close() ;
        delete pPatient ;
      }
      else if (pPatient->pDonnees->nss[0] != '\0')
      {
        string sCodePat = string(pPatient->pDonnees->nss) ;
        pPatient->lastError = pPatient->close() ;
        delete pPatient ;
        return sCodePat ;
      }
    }
  }
*/
#endif // ifndef _MUE

  string sSubIndice = string(sIndice, 1, strlen(sIndice.c_str()) - 1) ;

  int iCompte = 0 ;
  int iMulti  = 1 ;

  for (int i = strlen(sSubIndice.c_str()) - 1 ; i >= 0 ; i--)
  {
    if      ((sSubIndice[i] >= '0') && (sSubIndice[i] <= '9'))
      iCompte += (sSubIndice[i] - '0') * iMulti ;
    else if ((sSubIndice[i] >= 'A') && (sSubIndice[i] <= 'Z'))
    iCompte += ((sSubIndice[i] - 'A') + 10) * iMulti ;

    iMulti = iMulti * 36 ;
  }

  if (sIndice[0] == '2')
    iCompte += 50000 ;

  char szNum[6] ;
  numacar(szNum, iCompte, 5) ;

  return string(szNum) ;
}


string
CpatientExportOld::donneActeIndice(int iNumActe)
{
  char szNum[8] ;
  itoa(iNumActe, szNum, 10) ;
  return string(szNum) ;
}


// -----------------------------------------------------------------------------
// classe CadminDataExportOld// -----------------------------------------------------------------------------CadminDataExportOld::CadminDataExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp)                    :Cbalise(attributs, values)
{
  pMsgExport  = pExp ;
  pContexte   = pCtx ;
}


CadminDataExportOld::~CadminDataExportOld ()
{
}


bool
CadminDataExportOld::parser()
{
  // int       nb_dll, nb_nom, nb_ref ;
  // AttrIter iattribut ;
  // ValIter  ival ;

  // attributs

  if (false == parser_attributs())
    return false ;

  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    _iParsingError = EDIALOGUE_ATTR_EMPTY ;
    return false ;
  }

  // nb_dll = 0 ;
  // nb_nom = 0 ;
  // nb_ref = 0 ;

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

boolCadminDataExportOld::verif(){
  return true ;
}


bool
CadminDataExportOld::compresser()
{
  return true ;
}


bool
CadminDataExportOld::traiter()
{
  return true ;
}


string
CadminDataExportOld::getSexe()
{
  return getStringAttribute(ATTRIBUT_EX_ADMIN_SEXE) ;
}


string
CadminDataExportOld::getNaissance()
{
  return getStringAttribute(ATTRIBUT_EX_ADMIN_NAISSANCE) ;
}


string
CadminDataExportOld::donneNaissance()
{
  string sNaissanceOld = getNaissance() ;
  if (sNaissanceOld == "")
    return string("") ;
  string sDateNais = string("01/") + string(sNaissanceOld, 5, 2) + string("/") + string(sNaissanceOld, 0, 4) ;
  return sDateNais ;
}


// -----------------------------------------------------------------------------
// classe ChealthDataExportOld// -----------------------------------------------------------------------------ChealthDataExportOld::ChealthDataExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp)                     :Cbalise(attributs, values)
{
  pContexte   = pCtx ;
  pMsgExport  = pExp ;
}

ChealthDataExportOld::~ChealthDataExportOld()
{
/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CdocumentExportOld *pCDoc = dynamic_cast<CdocumentExportOld*>(pBalise) ;
      if (pCDoc)
      {
        delete pCDoc ;
        (*ival)->setObject((Cbalise*) 0) ;
      }
      else
      {
        CgoalExportOld *pCGoal = dynamic_cast<CgoalExportOld*>(pBalise) ;
        if (pCGoal)
        {
          delete pCGoal ;
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
ChealthDataExportOld::parser()
{
  // int       nb_dialogue, nb_items, nb_contraintes ;
  // AttrIter iattribut ;
  ValIter  ival ;

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

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_EX_DOCUMENT) // dialogue
		{
      (*ival)->setObject(new CdocumentExportOld((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing dialogue
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }
		}
    else if ((*ival)->getLabel() == LABEL_EX_GOAL) // header
		{
      (*ival)->setObject(new CgoalExportOld((*ival)->getAttribute(), (*ival)->getValue(), pContexte, pMsgExport)) ;
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
}boolChealthDataExportOld::verif(){
  return true ;
}

bool
ChealthDataExportOld::compresser()
{
  return true ;
}

bool
ChealthDataExportOld::traiter()
{
  return true ;
}

string
ChealthDataExportOld::getDate()
{
  return getStringAttribute(ATTRIBUT_EX_HEALTHDATA_DATE) ;
}

bool
ChealthDataExportOld::createSFMGfiles(string sCodePat, string sCodeActe)
{
  string  sActeLine = "" ;
  string  sDPIOUser = "" ;

  if (pContexte)
  {
	  sDPIOUser = pContexte->getUtilisateur()->getNss() ;
    strip(sDPIOUser, stripLeft, '0') ;
  }
  else
    sDPIOUser = pMsgExport->sUserId ;


  sActeLine = string("-9") + "\t" + sCodePat + "\t" + sDPIOUser + "\t" + getDate() + "\t" + string("-9") + "\t" + string("DPIO") + "\t" + sCodeActe ;
  pMsgExport->addToSFMGfile("05", sActeLine) ;

  if (vect_val.empty())
    return false ;

  int iNumDoc     = 0 ;
  int iNumGoalDoc = 0 ;

  string sCurrentDoc = "" ;

	for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if      ((*ival)->getLabel() == LABEL_EX_DOCUMENT) // dialogue
		{
      CdocumentExportOld *pDocument = dynamic_cast<CdocumentExportOld *>((*ival)->getObject()) ;
      if (pDocument)
      {
        iNumDoc++ ;

        char szNumRc[4] ;
        numacar(szNumRc, iNumDoc, 3) ;
        sCurrentDoc = string(szNumRc) ;

        pDocument->createSFMGfiles(sCodePat, sCodeActe, sCurrentDoc, pDocument->getDate()) ;
      }
    }
    else if ((*ival)->getLabel() == LABEL_EX_GOAL) // references
    {
      CgoalExport *pGoal = dynamic_cast<CgoalExport *>((*ival)->getObject()) ;
      if (pGoal)
      {
        iNumGoalDoc++ ;

        char szNumRc[4] ;
        numacar(szNumRc, iNumGoalDoc, 3) ;
        string sCurrentGoal = string(szNumRc) ;

        pGoal->createSFMGfiles(sCodePat, sCodeActe, sCurrentDoc, sCurrentGoal) ;
      }
    }
  }
  return true ;
}


// -----------------------------------------------------------------------------
//                           CdocumentExportOld// -----------------------------------------------------------------------------
CdocumentExportOld::CdocumentExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp)
                   :Cbalise(attributs, values)
{
  pContexte      = pCtx ;
  pMsgExport     = pExp ;
  pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

CdocumentExportOld::~CdocumentExportOld()
{
  if (pPatPathoArray)
    delete pPatPathoArray ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CnodeExport *pCNode = dynamic_cast<CnodeExport*>(pBalise) ;
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
CdocumentExportOld::parser()
{
  // int       nb_loc ;
  int       nb_node ;
  // AttrIter iattribut ;
  ValIter  ival ;

  // attributs
  if (false == parser_attributs())
    return false ;

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

  nb_node = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      (*ival)->setObject(new CnodeExport((*ival)->getAttribute(), (*ival)->getValue(), 0)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }

      CnodeExport *pCnode = dynamic_cast<CnodeExport *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPatPathoArray) ;

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

boolCdocumentExportOld::verif(){
  return true ;
}


bool
CdocumentExportOld::compresser()
{
  return true ;
}

bool
CdocumentExportOld::traiter()
{
  return true ;
}


string
CdocumentExportOld::getCode()
{
  return getStringAttribute(ATTRIBUT_EX_DOCU_CODE) ;
}


string
CdocumentExportOld::getDate()
{
  return getStringAttribute(ATTRIBUT_EX_DOCU_DATE) ;
}


bool
CdocumentExportOld::createSFMGfiles(string /* sCodePat */, string /* sCodeActe */, string /* sIndexRC */, string /* sDate */)
{
/*
  if (pPatPathoArray->empty())
    return true ;

  string sIndexGlobalRC = sCodeActe + sIndexRC ;

  // Traitement des RC
  PatPathoIter pptIter ;
  string sCodeRC = "" ;

  if      (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISC1", "|", &pptIter)) // Côlon
    sCodeRC = "802" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISO1", "|", &pptIter)) // Col
    sCodeRC = "801" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISR1", "|", &pptIter)) // Prostate
    sCodeRC = "805" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISB1", "|", &pptIter)) // Bouche
    sCodeRC = "800" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISS1", "|", &pptIter)) // Sein
    sCodeRC = "806" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRIME1", "|", &pptIter)) // Mélanome
    sCodeRC = "804" ;
  else if (pPatPathoArray->CheminDansPatpatho("0CALC1|VRISP1", "|", &pptIter)) // Epithélioma
    sCodeRC = "803" ;

  if (sCodeRC == "")
    return false ;

  pptIter++ ;

  string sCodePD = "" ;

  if      (string((*pptIter)->pDonnees->lexique) == "9NONC1")
    sCodePD = "1" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1RMOY1")
    sCodePD = "2" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1MAJO1")
    sCodePD = "3" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1IMPO1")
    sCodePD = "4" ;
  else if (string((*pptIter)->pDonnees->lexique) == "1IMPT2")
    sCodePD = "5" ;

  string sRCline = sIndexGlobalRC + "\t" + sCodeActe + "\t" + sCodeRC + "\t" + sCodePD + "\t" + string("N") + "\t" + sDate ;
  pMsgExport->addToSFMGfile("35", sRCline) ;

  // Traitement des items
  if (pPatPathoArray->CheminDansPatpatho("0CALC1|QFARI1", "|", &pptIter))
  {
    int iItemCol = (*pptIter)->pDonnees->getColonne() ;
    int iItemId = 0 ;

    pptIter++ ;
    while ((pptIter != pPatPathoArray->end()) && ((*pptIter)->pDonnees->getColonne() > iItemCol))
    {
      if ((string((*pptIter)->pDonnees->lexique) == "6MGIT1") &&
          (string((*pptIter)->pDonnees->complement) != ""))
      {
        iItemId++ ;
        char szNumItem[3] ;
        numacar(szNumItem, iItemId, 2) ;
        string sItemID = sIndexGlobalRC + string(szNumItem) ;
        string sItemLine = sItemID + "\t" + sIndexGlobalRC + "\t" + string((*pptIter)->pDonnees->complement) ;
        pMsgExport->addToSFMGfile("20", sItemLine) ;
      }
      pptIter++ ;
    }
  }

  VecteurString *pVecteurString = new VecteurString() ;
  // Traitement des IMI
  if (pContexte)
  {
    // partie utilisant la table nsreflink (quand la méthode est appelée à
    // partir de Nautilus)

    NSRefLink *pRefLink = new NSRefLink(pContexte) ;
    pRefLink->lastError = pRefLink->open() ;
    if (pRefLink->lastError != DBIERR_NONE)
    {
      erreur("Impossible d'ouvrir la table NSREFLINK.", standardError, pRefLink->lastError) ;
      delete pRefLink ;
      return false ;
    }

    bool bResult = pRefLink->GetAllCodesAndValues(pPatPathoArray, "6MGIM", pVecteurString) ;

    pRefLink->lastError = pRefLink->close() ;
    if (pRefLink->lastError != DBIERR_NONE)
      erreur("Impossible de fermer la table NSREFLINK.", standardError, pRefLink->lastError) ;
    delete pRefLink ;
  }
  else
  {
    // partie utilisant un fichier en texte uniquement remplacant la table
    // nsreflink (quand la méthode est appelée par un exécutbale externe)
    NSRefLinkInfoArray  *pRefLink = new NSRefLinkInfoArray(pContexte, "nsreflink.txt") ;
    if (!pRefLink->fillArray())
    {
      // problème lors de la récupération des données contenues dans
      // nsreflink.txt -- il faudrait rajouter une erreur
      erreur("Impossible d'ouvrir ou de récupérer les données du fichier nsreflink.txt.", standardError, 0) ;
      delete pRefLink ;
      return false ;
    }

    bool  bResult = pRefLink->GetAllCodesAndValues(pPatPathoArray, "6MGIM", pVecteurString) ;
    delete pRefLink ;
  }

  // ---------------------------------------------------------------------------
  if (!(pVecteurString->empty()))
  {
    int iImiId = 0 ;

    for (EquiItemIter j = pVecteurString->begin() ; j != pVecteurString->end() ; j++)
    {
      iImiId++ ;
      char szNumItem[3] ;
      numacar(szNumItem, iImiId, 2) ;
      string sImiID = sCodeActe + string(szNumItem) ;

      string sCode        = "" ;
      string sComplement  = "" ;

      size_t iSepar = (*j)->find("|") ;
      if (iSepar == string::npos)
        sCode = **j ;
      else
      {
        sCode       = string(**j, 0, iSepar) ;
        sComplement = string(**j, iSepar + 1, strlen((*j)->c_str()) - iSepar - 1) ;
      }

      string sIMIline = sImiID + "\t" + sCodeActe + "\t" + sCode ;
      if (sComplement != "")
        sIMIline += "\t" + sComplement ;
      pMsgExport->addToSFMGfile("14", sIMIline) ;
    }
  }

  delete pVecteurString ;
*/

  return true ;
}


// -----------------------------------------------------------------------------
//                           CgoalExportOld
// -----------------------------------------------------------------------------
CgoalExportOld::CgoalExportOld(string attributs, string values, NSContexte *pCtx, CmessageExportOld *pExp)
               :Cbalise(attributs, values)
{
  pContexte      = pCtx ;
  pMsgExport     = pExp ;
  pPatPathoArray = new NSPatPathoArray(pCtx->getSuperviseur()) ;
}

CgoalExportOld::~CgoalExportOld()
{
  if (pPatPathoArray)
    delete pPatPathoArray ;

/*
  if (false == vect_val.empty())
  {
    for (ValIter ival = vect_val.begin() ; vect_val.end() != ival ; )
    {
      Cbalise *pBalise = (*ival)->getObject() ;

      CnodeExport *pCNode = dynamic_cast<CnodeExport*>(pBalise) ;
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
CgoalExportOld::parser()
{
  // int       nb_loc ;
  int       nb_node ;
  // AttrIter iattribut ;
  ValIter  ival ;

  // attributs
  if (!parser_attributs())
    return false ;

  /*
  if (vect_attr.empty())
  {
    // erreur (pas d'attribut)
    iParsingError = ETREE_ATTR_EMPTY ;
    return false ;
  }
  */

	// valeurs
	if (false == parser_valeurs())
		return false ;

	if (vect_val.empty())
  {
    // erreur (pas de valeur)
    _iParsingError = ETREE_VAL_EMPTY ;
    return false ;
  }

  nb_node = 0 ;

	for (ival = vect_val.begin() ; vect_val.end() != ival ; ival++)
  {
		if ((*ival)->getLabel() == LABEL_NODE) // noeud
		{
      (*ival)->setObject(new CnodeExport((*ival)->getAttribute(), (*ival)->getValue(), 0)) ;
      if (false == (*ival)->getObject()->parser())
      {
        // erreur parsing items
        _iParsingError = (*ival)->getObject()->getParsingError() ;
        return false ;
      }

      CnodeExport *pCnode = dynamic_cast<CnodeExport *>((*ival)->getObject()) ;
      if (pCnode)
        pCnode->chargePatPatho(pPatPathoArray) ;

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

boolCgoalExportOld::verif(){
  return true ;
}

bool
CgoalExportOld::compresser()
{
  return true ;
}

bool
CgoalExportOld::traiter()
{
  return true ;
}

bool
CgoalExportOld::createSFMGfiles(string /* sCodePat */, string sCodeActe, string sIndexRC, string sIndexGoal)
{
  if (pPatPathoArray->empty())
    return true ;

  string sIndexGlobalRC   = sCodeActe + sIndexRC ;
  string sIndexGlobalGoal = sCodeActe + sIndexGoal ;

  // Traitement des items
  PatPathoIter pptIter ;
  if (pPatPathoArray->CheminDansPatpatho("0CODE1", string(1, cheminSeparationMARK), &pptIter))
  {
    int iItemCol = (*pptIter)->getColonne() ;
    // int iItemId = 0 ;

    pptIter++ ;
    if ((pptIter != pPatPathoArray->end()) && ((*pptIter)->getColonne() > iItemCol))
    {
      if (((*pptIter)->getLexique() == string("6MGDC1")) && ((*pptIter)->getComplement() != string("")))
      {
        string sDCLine = sIndexGlobalGoal + "\t" + string("-999") + "\t" + (*pptIter)->getComplement() + "\t" + string("Libelle") + "\t" + sCodeActe + "\t" ;
        pMsgExport->addToSFMGfile("26", sDCLine) ;
        string sDCRCLine = sIndexGlobalRC + "\t" + sIndexGlobalGoal ;
        pMsgExport->addToSFMGfile("22", sDCRCLine) ;
      }
    }
  }
  return true ;
}

// fin de nsattval.cpp
/////////////////////////////////////

