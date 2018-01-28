// -----------------------------------------------------------------------------// NsPapyrus.cpp
// -----------------------------------------------------------------------------
// Création d'une page HTML multi-documents
// -----------------------------------------------------------------------------
// $Revision: 1.5 $
// $Author: pameline $
// $Date: 2015/02/07 13:03:06 $
// -----------------------------------------------------------------------------
// PA - 05/2012
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

#include "nsepisod\nsPapyrus.h"

#include "nsdn\nsdocum.h"

// -----------------------------------------------------------------------------
//
// Classe NSPapyrus
//
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Constructeur
// -----------------------------------------------------------------------------
NSPapyrus::NSPapyrus(NSContexte *pCtx, string sFileName, string sTitle)
          :NSRoot(pCtx)
{
  ReinitLogPage() ;

  _sFileName = sFileName ;
  _sTitle    = sTitle ;
  _sLogFile  = string("") ;

  _sPapyrusDocumentID = string("") ;
}

NSPapyrus::~NSPapyrus()
{
}

void
NSPapyrus::ReinitLogPage()
{
  // _sLogPage         = string("") ;
  _aPapyrusDocuments.vider() ;
  _bLogPageModified = false ;
}

void
NSPapyrus::ReinitLogFile()
{
try
{
  ofstream outFile ;

  string sLang = pContexte->getUserLanguage() ;

  string sDate = donne_date_duJour() + donne_heure() ;

  string sNss = pContexte->getPatient()->getNss() ;
  if ('?' == sNss[0])
    sNss.replace(0, 1, "_") ;

	_sLogFile = _sFileName + sDate + pContexte->getUtilisateur()->getNss() + sNss + string(".htm") ;

  _sLogFile = pContexte->PathName("BGLO") + _sLogFile ;
  outFile.open(_sLogFile.c_str(), ios::ate | ios::trunc) ;

  if (!outFile)  {
    string sErrMess = string("Erreur d'ouverture en écriture du fichier ") + _sLogFile ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    _sLogFile = string("") ;
    return ;
  }

  string sLogPage = getPageTitle() ;
  outFile << sLogPage ;

  _bLogPageModified = false ;

  outFile.close() ;
}
catch (...)
{
  erreur("Exception NSDPIO::ReinitLogFile.", standardError, 0) ;
}
}

void
NSPapyrus::AppendLogFile()
{
try
{
  // Resets log file
  ofstream outFile ;
  outFile.open(_sLogFile.c_str(), ios::trunc) ;
  if (!outFile)
  {
    string sErrMess = string("Erreur d'ouverture en écriture du fichier de transfert PREDI ") + _sLogFile ;
    erreur(sErrMess.c_str(), standardError, 0) ;
    return ;
  }

  if (_aPapyrusDocuments.empty())
    return ;

  string sBlocks = getPageTitle() ;
  sBlocks += getLogPage() ;

  outFile << sBlocks ;

  outFile.close() ;
}
catch (...)
{
  erreur("Exception NSPapyrus::AppendLogFile.", standardError, 0) ;
}
}

void
NSPapyrus::addToLogPage(string sAdd, string sBefore, string sAfter)
{
  if ((string("") == sAdd) && (string("") == sBefore) && (string("") == sAfter))
    return ;

  string sLogTxt = sBefore + texteHtml(sAdd) + sAfter ;

  addTextToDoc("", sLogTxt) ;

  _bLogPageModified = true ;
}

string
NSPapyrus::getLogPage()
{
  if (_aPapyrusDocuments.empty())
    return string("") ;

  string sBlock = string("") ;

  for (NSPapyrusDocumentIter docIt = _aPapyrusDocuments.begin() ; _aPapyrusDocuments.end() != docIt ; docIt++)
    sBlock += getDivBlockForDoc(*docIt) ;

  return sBlock ;
}

string
NSPapyrus::getPageTitle()
{
  string sDate = donne_date_duJour() ;
  string sLang = pContexte->getUserLanguage() ;

  string sTitle = string("<h2>") + pContexte->getPatient()->getNomLong() + string("</h2>") + string(NEWLINE) ;

  string sDateClaire ;
  donne_date_claire(sDate, &sDateClaire, sLang) ;
  sTitle += string("<h2>") + _sTitle + string(" du ") + sDateClaire + string("</h2>") + string(NEWLINE) ;

  return sTitle ;
}

void
NSPapyrus::addTextToDoc(string sIndex, string sText)
{
  NSPapyrusDocument *pDoc = getDocumentForId(sIndex) ;
  if (pDoc)
  {
    pDoc->addText(sText) ;
    _bLogPageModified = true ;
    return ;
  }

  if (string("") != sIndex)
    return ;

  LogDocument(string(""), (NSDocumentInfo*) 0, (NSPatPathoArray*) 0, sText) ;
}

void
NSPapyrus::LogDocument(string sIndex, NSDocumentInfo* pNSDocumentInfo,
                       const NSPatPathoArray* pNSPatPathoArray, string sHtmlText)
{
  // Prevent recursivity: don't incorporate itself
  //
  if (pNSDocumentInfo && (pNSDocumentInfo->getID() == _sPapyrusDocumentID))
    return ;

  // Set index
  //
  string sPapyrusIndex = sIndex ;
  if ((string("") == sPapyrusIndex) && pNSDocumentInfo)
    sPapyrusIndex = pNSDocumentInfo->getID() ;

  NSPapyrusDocument *pPapyrusDoc = getDocumentForId(sPapyrusIndex) ;
  if (pPapyrusDoc)
    pPapyrusDoc->updateDocument(pNSDocumentInfo, pNSPatPathoArray, sHtmlText) ;
  else
    _aPapyrusDocuments.push_back(new NSPapyrusDocument(pContexte, sPapyrusIndex, pNSDocumentInfo, pNSPatPathoArray, sHtmlText)) ;

  _bLogPageModified = true ;
}

NSPapyrusDocument*
NSPapyrus::getDocumentForId(string sIndex)
{
  if (_aPapyrusDocuments.empty())
    return (NSPapyrusDocument*) 0 ;

  for (NSPapyrusDocumentIter iDoc = _aPapyrusDocuments.begin() ; _aPapyrusDocuments.end() != iDoc ; iDoc++)
    if ((*iDoc)->getIndex() == sIndex)
      return *iDoc ;

  return (NSPapyrusDocument*) 0 ;
}

string
NSPapyrus::getDivBlockForDoc(NSPapyrusDocument *pPapyrusDoc)
{
  if ((NSPapyrusDocument*) NULL == pPapyrusDoc)
    return string("") ;

  // <div>
  string sBlockText = string("<div style=\"nsPapyrusDoc\"") ;
  if (string("") != pPapyrusDoc->getIndex())
    sBlockText += string(" index=\"") + pPapyrusDoc->getIndex() + string("\"") ;
  sBlockText += string(">") + string(NEWLINE) ;

  NSPatPathoArray PptMeta(pContexte->getSuperviseur()) ;
  if (pPapyrusDoc->getDocument())
    pPapyrusDoc->getDocument()->initFromMeta(&PptMeta) ;

  sBlockText += string("  <!-- <LabelTree>\r\n") ;
  sBlockText += PptMeta.genereXML() ;
  sBlockText += string("  </LabelTree> -->\r\n") ;

  if (pPapyrusDoc->getPatPatho())
  {
    sBlockText += string("  <!-- <DataTree>\r\n") ;
    sBlockText += pPapyrusDoc->getPatPatho()->genereXML() ;
    sBlockText += string("  </DataTree> -->\r\n") ;
  }

  sBlockText += pPapyrusDoc->getDocumentText() ;

  // </div>
  sBlockText += string("</div>") + string(NEWLINE) ;

  return sBlockText ;
}

// -----------------------------------------------------------------------------
//
// Classe NSPapyrusDocument
//
// -----------------------------------------------------------------------------

NSPapyrusDocument::NSPapyrusDocument(NSContexte *pCtx, string sIndex, NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray *pPatPatho, string sDocTxt)
                  :NSRoot(pCtx)
{
  _sIndex = sIndex ;

  _pNSDocumentInfo = (NSDocumentInfo*) 0 ;
  _pPatPatho       = (NSPatPathoArray *) 0 ;

  if (pNSDocumentInfo)
    _pNSDocumentInfo = new NSDocumentInfo(*pNSDocumentInfo) ;

  if (pPatPatho)
    _pPatPatho = new NSPatPathoArray(*pPatPatho) ;

  _sDocumentText  = sDocTxt ;
}

NSPapyrusDocument::~NSPapyrusDocument()
{
  if (_pNSDocumentInfo)
    delete _pNSDocumentInfo ;
  if (_pPatPatho)
    delete _pPatPatho ;
}

void
NSPapyrusDocument::setDocument(NSDocumentInfo *pNSDocumentInfo)
{
  if (NULL == pNSDocumentInfo)
  {
    if (_pNSDocumentInfo)
    {
      delete _pNSDocumentInfo ;
      _pNSDocumentInfo = (NSDocumentInfo*) 0 ;
    }
    return ;
  }
  if (NULL == _pNSDocumentInfo)
    _pNSDocumentInfo = new NSDocumentInfo(*pNSDocumentInfo) ;
  else
    *_pNSDocumentInfo = *pNSDocumentInfo ;
}

void
NSPapyrusDocument::setPatPatho(const NSPatPathoArray *pPatPatho)
{
  if (NULL == pPatPatho)
  {
    if (_pPatPatho)
    {
      delete _pPatPatho ;
      _pPatPatho = (NSPatPathoArray*) 0 ;
    }
    return ;
  }
  if (NULL == _pPatPatho)
    _pPatPatho = new NSPatPathoArray(*pPatPatho) ;
  else
    *_pPatPatho = *pPatPatho ;
}

void
NSPapyrusDocument::updateDocument(NSDocumentInfo* pNSDocumentInfo, const NSPatPathoArray *pPatPatho, string sDocTxt)
{
  _sDocumentText = sDocTxt ;
  setDocument(pNSDocumentInfo) ;
  setPatPatho(pPatPatho) ;
}

NSPapyrusDocument&
NSPapyrusDocument::operator=(const NSPapyrusDocument& src)
{
  if (this == &src)
  	return *this ;

  _sIndex = src._sIndex ;
  updateDocument(src._pNSDocumentInfo, src._pPatPatho, src._sDocumentText) ;

  return (*this) ;
}

int
NSPapyrusDocument::operator==(const NSPapyrusDocument& o)
{
  if (_sIndex == o._sIndex)
    return 1 ;

  return 0 ;
}

